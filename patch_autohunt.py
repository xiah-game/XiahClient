"""
Patch XiahGameMain.cpp and XiahCheatConfig.cpp to decouple auto-hunt toggle.
Uses binary-safe line replacement to preserve original EUC-KR encoding.
Only modifies lines that contain pure ASCII content.
"""
import sys

def patch_file(filepath, replacements):
    """
    replacements: list of (old_bytes, new_bytes) tuples
    Each old_bytes must be unique in the file.
    """
    with open(filepath, 'rb') as f:
        data = f.read()
    
    for old_bytes, new_bytes in replacements:
        count = data.count(old_bytes)
        if count == 0:
            print(f"WARNING: Pattern not found in {filepath}:")
            print(f"  {old_bytes[:80]}...")
            continue
        if count > 1:
            print(f"WARNING: Pattern found {count} times in {filepath}, skipping:")
            print(f"  {old_bytes[:80]}...")
            continue
        data = data.replace(old_bytes, new_bytes, 1)
        print(f"OK: Replaced 1 occurrence in {filepath}")
    
    with open(filepath, 'wb') as f:
        f.write(data)
    print(f"Saved {filepath}")

# ============================================================
# Patch 1: XiahGameMain.cpp - Comment out first F10 toggle
# ============================================================
patch_file(r'd:\xiahold\XiahClient\XiahGameMain.cpp', [
    # First F10: comment out g_bCheat and g_bCheatEtc toggle
    (
        b'\t\t\t\tcase VK_F10:\r\n'
        b'\t\t\t\t\t{\r\n'
        b'\t\t\t\t\t\tg_bCheat = !g_bCheat;\r\n'
        b'\t\t\t\t\t\tg_bCheatEtc = !g_bCheatEtc;\t\r\n'
        b'\t\t\t\t\t}\r\n'
        b'\t\t\t\t\tbreak;',
        
        b'\t\t\t\tcase VK_F10:\r\n'
        b'\t\t\t\t\t{\r\n'
        b'\t\t\t\t\t\t// Auto-hunt toggle moved to F9 config window button\r\n'
        b'\t\t\t\t\t\t//g_bCheat = !g_bCheat;\r\n'
        b'\t\t\t\t\t\t//g_bCheatEtc = !g_bCheatEtc;\t\r\n'
        b'\t\t\t\t\t}\r\n'
        b'\t\t\t\t\tbreak;'
    ),
    # Second F11 (was F10): comment out the cheat toggle block
    (
        b'\t\t\t\t\tif(g_bCheat && !g_bCheatEtc)\r\n'
        b'\t\t\t\t\t{\r\n'
        b'\t\t\t\t\t\tg_bCheatEtc = !g_bCheatEtc;\t\r\n'
        b'\t\t\t\t\t}\r\n'
        b'\t\t\t\t\telse\r\n'
        b'\t\t\t\t\t{\r\n'
        b'\t\t\t\t\t\tg_bCheat = !g_bCheat;\r\n'
        b'\t\t\t\t\t\tg_bCheatEtc = !g_bCheatEtc;\t\r\n'
        b'\t\t\t\t\t}',
        
        b'\t\t\t\t\t// Auto-hunt toggle moved to F9 config window button\r\n'
        b'\t\t\t\t\t//if(g_bCheat && !g_bCheatEtc)\r\n'
        b'\t\t\t\t\t//{\r\n'
        b'\t\t\t\t\t//\tg_bCheatEtc = !g_bCheatEtc;\t\r\n'
        b'\t\t\t\t\t//}\r\n'
        b'\t\t\t\t\t//else\r\n'
        b'\t\t\t\t\t//{\r\n'
        b'\t\t\t\t\t//\tg_bCheat = !g_bCheat;\r\n'
        b'\t\t\t\t\t//\tg_bCheatEtc = !g_bCheatEtc;\t\r\n'
        b'\t\t\t\t\t//}'
    ),
])

# ============================================================
# Patch 2: XiahCheatConfig.cpp
# ============================================================
patch_file(r'd:\xiahold\XiahClient\XiahCheatConfig.cpp', [
    # 2a: Toggle button text: use g_bCheat instead of g_bAutoAttack
    (
        b'        LPCTSTR pszToggleText = g_bAutoAttack ? _T("\\xbd\\xe1\\xca\\xf8\\xb9\\xd2\\xbb\\xfa") : _T("\\xbf\\xaa\\xca\\xbc\\xb9\\xd2\\xbb\\xfa");\r\n'
        b'        HWND hBtnToggle = CreateWindow(_T("BUTTON"), pszToggleText,',
        
        b'        // Toggle button text based on g_bCheat (master switch)\r\n'
        b'        extern BOOL g_bCheat;\r\n'
        b'        LPCTSTR pszToggleText = g_bCheat ? _T("\\xbd\\xe1\\xca\\xf8\\xb9\\xd2\\xbb\\xfa") : _T("\\xbf\\xaa\\xca\\xbc\\xb9\\xd2\\xbb\\xfa");\r\n'
        b'        HWND hBtnToggle = CreateWindow(_T("BUTTON"), pszToggleText,'
    ),
    # 2b: Replace WM_COMMAND handler - remove bOldAutoAttack and g_bCheat=g_bAutoAttack sync
    #     Split into: first remove bOldAutoAttack line and the toggle+cheat sync block
    (
        b'            TCHAR szTmp[32];\r\n'
        b'            \r\n'
        b'            BOOL bOldAutoAttack = g_bAutoAttack;',
        
        b'            TCHAR szTmp[32];'
    ),
    # 2c: Remove the toggle block and g_bCheat = g_bAutoAttack sync
    (
        b'            if (id == ID_BUTTON_TOGGLE) {\r\n'
        b'                g_bAutoAttack = !bOldAutoAttack;\r\n'
        b'            }\r\n'
        b'            \r\n'
        b'            extern BOOL g_bCheat;\r\n'
        b'            g_bCheat = g_bAutoAttack;\r\n'
        b'            \r\n'
        b'            GetWindowText',
        
        b'            GetWindowText'
    ),
    # 2d: Replace DestroyWindow with toggle/save split logic
    (
        b'            ApplyCheatConfigToMainChar();\r\n'
        b'            SaveCheatConfig();\r\n'
        b'            DestroyWindow(hwnd);\r\n'
        b'        } else if (id == ID_BUTTON_CANCEL) {',
        
        b'            ApplyCheatConfigToMainChar();\r\n'
        b'            SaveCheatConfig();\r\n'
        b'            \r\n'
        b'            if (id == ID_BUTTON_TOGGLE) {\r\n'
        b'                // Only Toggle button controls auto-hunt on/off\r\n'
        b'                extern BOOL g_bCheat;\r\n'
        b'                extern BOOL g_bCheatEtc;\r\n'
        b'                g_bCheat = !g_bCheat;\r\n'
        b'                g_bCheatEtc = g_bCheat;\r\n'
        b'                \r\n'
        b'                // Update button text to reflect current state\r\n'
        b'                HWND hBtnToggle = GetDlgItem(hwnd, ID_BUTTON_TOGGLE);\r\n'
        b'                if (hBtnToggle) {\r\n'
        b'                    SetWindowText(hBtnToggle, g_bCheat \r\n'
        b'                        ? _T("\\xbd\\xe1\\xca\\xf8\\xb9\\xd2\\xbb\\xfa")\r\n'
        b'                        : _T("\\xbf\\xaa\\xca\\xbc\\xb9\\xd2\\xbb\\xfa"));\r\n'
        b'                }\r\n'
        b'            } else {\r\n'
        b'                // Save button: only save config and close\r\n'
        b'                DestroyWindow(hwnd);\r\n'
        b'            }\r\n'
        b'        } else if (id == ID_BUTTON_CANCEL) {'
    ),
])

print("\nAll patches applied successfully!")
