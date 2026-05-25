#include "XiahCheatConfig.h"
#include <tchar.h>
#include <stdio.h>
#include <commctrl.h>
#include "XiahArrayIndex.h"
#include "Mugong.h"
#include "CharacterInfo.h"
#include "XiahGameObject.h"

#pragma comment(lib, "comctl32.lib")

// Link common controls v6 for modern visual styles
#pragma comment(linker,"\"/manifestdependency:type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")

#ifndef GB2132_CHARSET
#define GB2132_CHARSET 134
#endif
#ifndef CLEARTYPE_QUALITY
#define CLEARTYPE_QUALITY 5
#endif

// Global variables with default values
BOOL g_bAutoAttack = TRUE;
BOOL g_bAutoLoot = TRUE;

BOOL g_bAutoHP = TRUE;
int  g_nHPPercent = 50;
TCHAR g_szHPPotionName[32] = _T("\xc8\xab\xb4\xb4\xd2\xa9\x28\xd0\xa1\x29");
BOOL g_bAutoBuyHP = FALSE;

BOOL g_bAutoMP = TRUE;
int  g_nMPPercent = 10;
TCHAR g_szMPPotionName[32] = _T("\xc4\xfd\xc6\xf8\xb5\xa4\x28\xb4\xf3\x29");
BOOL g_bAutoBuyMP = FALSE;

BOOL g_bAutoPet = TRUE;
int  g_nPetWildRate = 50;
DWORD g_dwPetFoodID = 9102;
DWORD g_dwPetSealID = 9210;
int  g_nBuffInterval = 10;

TCHAR g_szFilterList[4096] = _T("");

// Skill Settings
DWORD g_dwAttackSkillID = 0;
int g_nAttackSkillInterval = 5;

DWORD g_dwBuffSkillID1 = 0;
int g_nBuffSkillInterval1 = 30;

DWORD g_dwBuffSkillID2 = 0;
int g_nBuffSkillInterval2 = 30;

DWORD g_dwTeammateSkillID1 = 0;
int g_nTeammateSkillInterval1 = 10;

DWORD g_dwTeammateSkillID2 = 0;
int g_nTeammateSkillInterval2 = 10;

// Last cast times
DWORD g_dwLastAttackSkillTime = 0;
DWORD g_dwLastBuffSkillTime1 = 0;
DWORD g_dwLastBuffSkillTime2 = 0;
DWORD g_dwLastTeammateSkillTime1 = 0;
DWORD g_dwLastTeammateSkillTime2 = 0;
BOOL g_bIsAutoCasting = FALSE;

// Sell settings
BOOL g_bAutoSellFull = FALSE;
BOOL g_bAutoSellAll = FALSE;
BOOL g_bBanSellFilter = TRUE;
TCHAR g_szBanSellList[4096] = _T("");

// Control tracking arrays
static HWND g_hTab0Controls[40];
static int g_nTab0Count = 0;

static HWND g_hTab1Controls[40];
static int g_nTab1Count = 0;

static HWND g_hTab2Controls[20];
static int g_nTab2Count = 0;

static void ShowTabControls(int iTab) {
    for (int i = 0; i < g_nTab0Count; ++i) ShowWindow(g_hTab0Controls[i], iTab == 0 ? SW_SHOW : SW_HIDE);
    for (int i = 0; i < g_nTab1Count; ++i) ShowWindow(g_hTab1Controls[i], iTab == 1 ? SW_SHOW : SW_HIDE);
    for (int i = 0; i < g_nTab2Count; ++i) ShowWindow(g_hTab2Controls[i], iTab == 2 ? SW_SHOW : SW_HIDE);
}

void PopulateSkillComboBox(HWND hCombo, DWORD dwSelectedID) {
    SendMessage(hCombo, CB_RESETCONTENT, 0, 0);
    int idx = SendMessage(hCombo, CB_ADDSTRING, 0, (LPARAM)_T("\xce\xde"));
    SendMessage(hCombo, CB_SETITEMDATA, idx, 0);
    
    int selectedIdx = 0;
    
    if (g_MainCharInfo.m_pMugong) {
        for (MugongMap::iterator it = g_MainCharInfo.m_pMugong->m_mapMugong.begin(); 
             it != g_MainCharInfo.m_pMugong->m_mapMugong.end(); ++it) {
            DWORD dwID = it->first;
            sArrayData* pMugongData = XiahArrayIndex::g_MugongTemplate.GetData(dwID);
            if (pMugongData) {
                sString szName = pMugongData->GetString(1);
                TCHAR szLabel[64];
                _sntprintf(szLabel, 64, _T("%s (ID: %d)"), (LPCTSTR)szName, dwID);
                int curIdx = SendMessage(hCombo, CB_ADDSTRING, 0, (LPARAM)szLabel);
                SendMessage(hCombo, CB_SETITEMDATA, curIdx, dwID);
                if (dwID == dwSelectedID) {
                    selectedIdx = curIdx;
                }
            }
        }
        for (MugongMap::iterator it = g_MainCharInfo.m_pMugong->m_map2ThRebirthMugong.begin(); 
             it != g_MainCharInfo.m_pMugong->m_map2ThRebirthMugong.end(); ++it) {
            DWORD dwID = it->first;
            sArrayData* pMugongData = XiahArrayIndex::g_MugongTemplate.GetData(dwID);
            if (pMugongData) {
                sString szName = pMugongData->GetString(1);
                TCHAR szLabel[64];
                _sntprintf(szLabel, 64, _T("%s (ID: %d)"), (LPCTSTR)szName, dwID);
                int curIdx = SendMessage(hCombo, CB_ADDSTRING, 0, (LPARAM)szLabel);
                SendMessage(hCombo, CB_SETITEMDATA, curIdx, dwID);
                if (dwID == dwSelectedID) {
                    selectedIdx = curIdx;
                }
            }
        }
    }
    SendMessage(hCombo, CB_SETCURSEL, selectedIdx, 0);
}

// Handle to config window thread
static HANDLE g_hConfigThread = NULL;
static HWND g_hwndConfig = NULL;

// Control IDs
enum {
    ID_TABCONTROL = 1000,
    ID_CHECK_AUTOATTACK = 1001,
    ID_CHECK_AUTOLOOT,
    ID_CHECK_AUTOHP,
    ID_EDIT_HPPERCENT,
    ID_EDIT_HPPOTIONNAME,
    ID_CHECK_AUTOBUYHP,
    ID_CHECK_AUTOMP,
    ID_EDIT_MPPERCENT,
    ID_EDIT_MPPOTIONNAME,
    ID_CHECK_AUTOBUYMP,
    ID_CHECK_AUTOPET,
    ID_EDIT_PETWILDRATE,
    ID_EDIT_PETFOODID,
    ID_EDIT_PETSEALID,
    ID_EDIT_FILTERLIST,
    
    // New Skill tab control IDs
    ID_COMBO_ATTACKSKILL,
    ID_EDIT_ATTACKINTERVAL,
    ID_COMBO_BUFFSKILL1,
    ID_EDIT_BUFFINTERVAL1,
    ID_COMBO_BUFFSKILL2,
    ID_EDIT_BUFFINTERVAL2,
    ID_COMBO_TEAMMATESKILL1,
    ID_EDIT_TEAMMATEINTERVAL1,
    ID_COMBO_TEAMMATESKILL2,
    ID_EDIT_TEAMMATEINTERVAL2,
    
    // New Sell tab control IDs
    ID_CHECK_AUTOSELL_FULL,
    ID_CHECK_AUTOSELL_ALL,
    ID_CHECK_BANSELL_FILTER,
    ID_EDIT_BANSELL_LIST,
    
    ID_BUTTON_TOGGLE,
    ID_BUTTON_SAVE,
    ID_BUTTON_CANCEL
};

// Get path to standalone cheat.ini
static void GetIniPath(TCHAR* szPath, DWORD dwSize) {
    TCHAR szExePath[MAX_PATH];
    GetModuleFileName(NULL, szExePath, MAX_PATH);
    TCHAR* pLastSlash = _tcsrchr(szExePath, _T('\\'));
    if (pLastSlash) {
        *(pLastSlash + 1) = _T('\0');
    }
    _sntprintf(szPath, dwSize, _T("%scheat.ini"), szExePath);
}

void LoadCheatConfig() {
    TCHAR szIniFile[MAX_PATH];
    GetIniPath(szIniFile, MAX_PATH);
    
    g_bAutoAttack = GetPrivateProfileInt(_T("CHEAT"), _T("AutoAttack"), 1, szIniFile);
    g_bAutoLoot = GetPrivateProfileInt(_T("CHEAT"), _T("AutoLoot"), 1, szIniFile);
    
    g_bAutoHP = GetPrivateProfileInt(_T("CHEAT"), _T("AutoHP"), 1, szIniFile);
    g_nHPPercent = GetPrivateProfileInt(_T("CHEAT"), _T("HPPercent"), 50, szIniFile);
    GetPrivateProfileString(_T("CHEAT"), _T("HPPotionName"), _T("\xc8\xab\xb4\xb4\xd2\xa9\x28\xd0\xa1\x29"), g_szHPPotionName, 32, szIniFile);
    g_bAutoBuyHP = GetPrivateProfileInt(_T("CHEAT"), _T("AutoBuyHP"), 0, szIniFile);
    
    g_bAutoMP = GetPrivateProfileInt(_T("CHEAT"), _T("AutoMP"), 1, szIniFile);
    g_nMPPercent = GetPrivateProfileInt(_T("CHEAT"), _T("MPPercent"), 10, szIniFile);
    GetPrivateProfileString(_T("CHEAT"), _T("MPPotionName"), _T("\xc4\xfd\xc6\xf8\xb5\xa4\x28\xb4\xf3\x29"), g_szMPPotionName, 32, szIniFile);
    g_bAutoBuyMP = GetPrivateProfileInt(_T("CHEAT"), _T("AutoBuyMP"), 0, szIniFile);
    
    g_bAutoPet = GetPrivateProfileInt(_T("CHEAT"), _T("AutoPet"), 1, szIniFile);
    g_nPetWildRate = GetPrivateProfileInt(_T("CHEAT"), _T("PetWildRate"), 50, szIniFile);
    g_dwPetFoodID = GetPrivateProfileInt(_T("CHEAT"), _T("PetFoodID"), 9102, szIniFile);
    g_dwPetSealID = GetPrivateProfileInt(_T("CHEAT"), _T("PetSealID"), 9210, szIniFile);
    g_nBuffInterval = GetPrivateProfileInt(_T("CHEAT"), _T("BuffInterval"), 10, szIniFile);
    
    // Load filter list
    TCHAR szRawFilter[4096] = {0};
    GetPrivateProfileString(_T("FILTER"), _T("List"), _T(""), szRawFilter, 4096, szIniFile);
    int dst = 0;
    for (int src = 0; szRawFilter[src] != _T('\0') && dst < 4094; ++src) {
        if (szRawFilter[src] == _T('|')) {
            g_szFilterList[dst++] = _T('\r');
            g_szFilterList[dst++] = _T('\n');
        } else {
            g_szFilterList[dst++] = szRawFilter[src];
        }
    }
    g_szFilterList[dst] = _T('\0');

    // Load Skill Configs
    g_dwAttackSkillID = GetPrivateProfileInt(_T("SKILL"), _T("AttackSkillID"), 0, szIniFile);
    g_nAttackSkillInterval = GetPrivateProfileInt(_T("SKILL"), _T("AttackSkillInterval"), 5, szIniFile);
    if (g_nAttackSkillInterval < 1) g_nAttackSkillInterval = 1;

    g_dwBuffSkillID1 = GetPrivateProfileInt(_T("SKILL"), _T("BuffSkillID1"), 0, szIniFile);
    g_nBuffSkillInterval1 = GetPrivateProfileInt(_T("SKILL"), _T("BuffSkillInterval1"), 30, szIniFile);
    if (g_nBuffSkillInterval1 < 1) g_nBuffSkillInterval1 = 1;

    g_dwBuffSkillID2 = GetPrivateProfileInt(_T("SKILL"), _T("BuffSkillID2"), 0, szIniFile);
    g_nBuffSkillInterval2 = GetPrivateProfileInt(_T("SKILL"), _T("BuffSkillInterval2"), 30, szIniFile);
    if (g_nBuffSkillInterval2 < 1) g_nBuffSkillInterval2 = 1;

    g_dwTeammateSkillID1 = GetPrivateProfileInt(_T("SKILL"), _T("TeammateSkillID1"), 0, szIniFile);
    g_nTeammateSkillInterval1 = GetPrivateProfileInt(_T("SKILL"), _T("TeammateSkillInterval1"), 10, szIniFile);
    if (g_nTeammateSkillInterval1 < 1) g_nTeammateSkillInterval1 = 1;

    g_dwTeammateSkillID2 = GetPrivateProfileInt(_T("SKILL"), _T("TeammateSkillID2"), 0, szIniFile);
    g_nTeammateSkillInterval2 = GetPrivateProfileInt(_T("SKILL"), _T("TeammateSkillInterval2"), 10, szIniFile);
    if (g_nTeammateSkillInterval2 < 1) g_nTeammateSkillInterval2 = 1;

    // Load Sell Configs
    g_bAutoSellFull = GetPrivateProfileInt(_T("SELL"), _T("AutoSellFull"), 0, szIniFile);
    g_bAutoSellAll = GetPrivateProfileInt(_T("SELL"), _T("AutoSellAll"), 0, szIniFile);
    g_bBanSellFilter = GetPrivateProfileInt(_T("SELL"), _T("BanSellFilter"), 1, szIniFile);
    
    // Load bansell list
    TCHAR szRawBanSell[4096] = {0};
    GetPrivateProfileString(_T("FILTER"), _T("BanSellList"), _T(""), szRawBanSell, 4096, szIniFile);
    int dstB = 0;
    for (int src = 0; szRawBanSell[src] != _T('\0') && dstB < 4094; ++src) {
        if (szRawBanSell[src] == _T('|')) {
            g_szBanSellList[dstB++] = _T('\r');
            g_szBanSellList[dstB++] = _T('\n');
        } else {
            g_szBanSellList[dstB++] = szRawBanSell[src];
        }
    }
    g_szBanSellList[dstB] = _T('\0');
}

void SaveCheatConfig() {
    TCHAR szIniFile[MAX_PATH];
    GetIniPath(szIniFile, MAX_PATH);
    
    TCHAR szVal[32];
    
    _sntprintf(szVal, 32, _T("%d"), g_bAutoAttack);
    WritePrivateProfileString(_T("CHEAT"), _T("AutoAttack"), szVal, szIniFile);
    _sntprintf(szVal, 32, _T("%d"), g_bAutoLoot);
    WritePrivateProfileString(_T("CHEAT"), _T("AutoLoot"), szVal, szIniFile);
    
    _sntprintf(szVal, 32, _T("%d"), g_bAutoHP);
    WritePrivateProfileString(_T("CHEAT"), _T("AutoHP"), szVal, szIniFile);
    _sntprintf(szVal, 32, _T("%d"), g_nHPPercent);
    WritePrivateProfileString(_T("CHEAT"), _T("HPPercent"), szVal, szIniFile);
    WritePrivateProfileString(_T("CHEAT"), _T("HPPotionName"), g_szHPPotionName, szIniFile);
    _sntprintf(szVal, 32, _T("%d"), g_bAutoBuyHP);
    WritePrivateProfileString(_T("CHEAT"), _T("AutoBuyHP"), szVal, szIniFile);
    
    _sntprintf(szVal, 32, _T("%d"), g_bAutoMP);
    WritePrivateProfileString(_T("CHEAT"), _T("AutoMP"), szVal, szIniFile);
    _sntprintf(szVal, 32, _T("%d"), g_nMPPercent);
    WritePrivateProfileString(_T("CHEAT"), _T("MPPercent"), szVal, szIniFile);
    WritePrivateProfileString(_T("CHEAT"), _T("MPPotionName"), g_szMPPotionName, szIniFile);
    _sntprintf(szVal, 32, _T("%d"), g_bAutoBuyMP);
    WritePrivateProfileString(_T("CHEAT"), _T("AutoBuyMP"), szVal, szIniFile);
    
    _sntprintf(szVal, 32, _T("%d"), g_bAutoPet);
    WritePrivateProfileString(_T("CHEAT"), _T("AutoPet"), szVal, szIniFile);
    _sntprintf(szVal, 32, _T("%d"), g_nPetWildRate);
    WritePrivateProfileString(_T("CHEAT"), _T("PetWildRate"), szVal, szIniFile);
    _sntprintf(szVal, 32, _T("%u"), g_dwPetFoodID);
    WritePrivateProfileString(_T("CHEAT"), _T("PetFoodID"), szVal, szIniFile);
    _sntprintf(szVal, 32, _T("%u"), g_dwPetSealID);
    WritePrivateProfileString(_T("CHEAT"), _T("PetSealID"), szVal, szIniFile);
    _sntprintf(szVal, 32, _T("%d"), g_nBuffInterval);
    WritePrivateProfileString(_T("CHEAT"), _T("BuffInterval"), szVal, szIniFile);
    
    // Save filter list
    TCHAR szRawFilter[4096] = {0};
    int dst = 0;
    for (int src = 0; g_szFilterList[src] != _T('\0') && dst < 4094; ++src) {
        if (g_szFilterList[src] == _T('\r')) {
            // skip
        } else if (g_szFilterList[src] == _T('\n')) {
            szRawFilter[dst++] = _T('|');
        } else {
            szRawFilter[dst++] = g_szFilterList[src];
        }
    }
    szRawFilter[dst] = _T('\0');
    WritePrivateProfileString(_T("FILTER"), _T("List"), szRawFilter, szIniFile);

    // Save Skill Configs
    _sntprintf(szVal, 32, _T("%u"), g_dwAttackSkillID);
    WritePrivateProfileString(_T("SKILL"), _T("AttackSkillID"), szVal, szIniFile);
    _sntprintf(szVal, 32, _T("%d"), g_nAttackSkillInterval);
    WritePrivateProfileString(_T("SKILL"), _T("AttackSkillInterval"), szVal, szIniFile);

    _sntprintf(szVal, 32, _T("%u"), g_dwBuffSkillID1);
    WritePrivateProfileString(_T("SKILL"), _T("BuffSkillID1"), szVal, szIniFile);
    _sntprintf(szVal, 32, _T("%d"), g_nBuffSkillInterval1);
    WritePrivateProfileString(_T("SKILL"), _T("BuffSkillInterval1"), szVal, szIniFile);

    _sntprintf(szVal, 32, _T("%u"), g_dwBuffSkillID2);
    WritePrivateProfileString(_T("SKILL"), _T("BuffSkillID2"), szVal, szIniFile);
    _sntprintf(szVal, 32, _T("%d"), g_nBuffSkillInterval2);
    WritePrivateProfileString(_T("SKILL"), _T("BuffSkillInterval2"), szVal, szIniFile);

    _sntprintf(szVal, 32, _T("%u"), g_dwTeammateSkillID1);
    WritePrivateProfileString(_T("SKILL"), _T("TeammateSkillID1"), szVal, szIniFile);
    _sntprintf(szVal, 32, _T("%d"), g_nTeammateSkillInterval1);
    WritePrivateProfileString(_T("SKILL"), _T("TeammateSkillInterval1"), szVal, szIniFile);

    _sntprintf(szVal, 32, _T("%u"), g_dwTeammateSkillID2);
    WritePrivateProfileString(_T("SKILL"), _T("TeammateSkillID2"), szVal, szIniFile);
    _sntprintf(szVal, 32, _T("%d"), g_nTeammateSkillInterval2);
    WritePrivateProfileString(_T("SKILL"), _T("TeammateSkillInterval2"), szVal, szIniFile);

    // Save Sell Configs
    _sntprintf(szVal, 32, _T("%d"), g_bAutoSellFull);
    WritePrivateProfileString(_T("SELL"), _T("AutoSellFull"), szVal, szIniFile);
    _sntprintf(szVal, 32, _T("%d"), g_bAutoSellAll);
    WritePrivateProfileString(_T("SELL"), _T("AutoSellAll"), szVal, szIniFile);
    _sntprintf(szVal, 32, _T("%d"), g_bBanSellFilter);
    WritePrivateProfileString(_T("SELL"), _T("BanSellFilter"), szVal, szIniFile);
    
    // Save bansell list
    TCHAR szRawBanSell[4096] = {0};
    int dstB = 0;
    for (int src = 0; g_szBanSellList[src] != _T('\0') && dstB < 4094; ++src) {
        if (g_szBanSellList[src] == _T('\r')) {
            // skip
        } else if (g_szBanSellList[src] == _T('\n')) {
            szRawBanSell[dstB++] = _T('|');
        } else {
            szRawBanSell[dstB++] = g_szBanSellList[src];
        }
    }
    szRawBanSell[dstB] = _T('\0');
    WritePrivateProfileString(_T("FILTER"), _T("BanSellList"), szRawBanSell, szIniFile);
}

BOOL IsItemFiltered(const TCHAR* szItemName) {
    if (!szItemName || _tcslen(szItemName) == 0) return FALSE;
    TCHAR szListCopy[4096];
    _tcscpy_s(szListCopy, 4096, g_szFilterList);
    TCHAR* context = NULL;
    TCHAR* token = _tcstok_s(szListCopy, _T("\r\n|,"), &context);
    while (token != NULL) {
        TCHAR szTrimmed[128] = {0};
        int len = _tcslen(token);
        int start = 0;
        while (start < len && (token[start] == _T(' ') || token[start] == _T('\t'))) {
            start++;
        }
        int end = len - 1;
        while (end >= start && (token[end] == _T(' ') || token[end] == _T('\t'))) {
            end--;
        }
        if (end >= start) {
            _tcsncpy(szTrimmed, token + start, end - start + 1);
            szTrimmed[end - start + 1] = _T('\0');
            if (_tcscmp(szTrimmed, szItemName) == 0) {
                return TRUE;
            }
        }
        token = _tcstok_s(NULL, _T("\r\n|,"), &context);
    }
    return FALSE;
}

void ApplyCheatConfigToMainChar() {
}

LRESULT CALLBACK ConfigWndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
    static HFONT hFont = NULL;
    
    switch (msg) {
    case WM_CREATE: {
        hFont = CreateFont(15, 0, 0, 0, FW_NORMAL, FALSE, FALSE, FALSE,
                           GB2132_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
                           CLEARTYPE_QUALITY, DEFAULT_PITCH | FF_DONTCARE, _T("Microsoft YaHei"));
        if (!hFont) {
            hFont = (HFONT)GetStockObject(DEFAULT_GUI_FONT);
        }
        
        g_nTab0Count = 0;
        g_hTab0Controls[g_nTab0Count++] = CreateWindow(_T("STATIC"), _T(""), WS_CHILD, 0, 0, 0, 0, hwnd, NULL, NULL, NULL);
        
        g_nTab1Count = 0;
        g_nTab2Count = 0;
        
        TCHAR szTmp[64];
        HWND hCtrl;
        
        HWND hTab = CreateWindow(WC_TABCONTROL, _T(""), WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS, 
                                 10, 10, 595, 375, hwnd, (HMENU)ID_TABCONTROL, NULL, NULL);
        SendMessage(hTab, WM_SETFONT, (WPARAM)hFont, TRUE);
        
        TCITEM tie;
        tie.mask = TCIF_TEXT;
        tie.pszText = _T("\xbb\xf9\xb4\xa1\xc9\xe8\xd6\xc3");
        TabCtrl_InsertItem(hTab, 0, &tie);
        tie.pszText = _T("\xbc\xbc\xc4\xdc\xc9\xe8\xd6\xc3");
        TabCtrl_InsertItem(hTab, 1, &tie);
        tie.pszText = _T("\xca\xdb\xc2\xf4\xc9\xe8\xd6\xc3");
        TabCtrl_InsertItem(hTab, 2, &tie);
        
        hCtrl = CreateWindow(_T("BUTTON"), _T("\xd7\xd4\xb6\xaf\xd1\xb0\xd5\xd2\xb9\xd6\xce\xef\xb4\xf2\xb9\xd6\x20\x28\xd6\xf7\xb9\xd2\xbb\xfa\xbf\xaa\xb9\xd8\x20\x46\x39\x29"), WS_VISIBLE | WS_CHILD | BS_AUTOCHECKBOX, 25, 45, 300, 20, hwnd, (HMENU)ID_CHECK_AUTOATTACK, NULL, NULL);
        SendMessage(hCtrl, BM_SETCHECK, g_bAutoAttack ? BST_CHECKED : BST_UNCHECKED, 0);
        SendMessage(hCtrl, WM_SETFONT, (WPARAM)hFont, TRUE);
        g_hTab0Controls[g_nTab0Count++] = hCtrl;
        
        hCtrl = CreateWindow(_T("BUTTON"), _T("\xd7\xd4\xb6\xaf\xca\xb0\xc8\xa1\xb7\xb6\xce\xa7\xce\xef\xc6\xb7"), WS_VISIBLE | WS_CHILD | BS_AUTOCHECKBOX, 25, 70, 300, 20, hwnd, (HMENU)ID_CHECK_AUTOLOOT, NULL, NULL);
        SendMessage(hCtrl, BM_SETCHECK, g_bAutoLoot ? BST_CHECKED : BST_UNCHECKED, 0);
        SendMessage(hCtrl, WM_SETFONT, (WPARAM)hFont, TRUE);
        g_hTab0Controls[g_nTab0Count++] = hCtrl;
        
        hCtrl = CreateWindow(_T("BUTTON"), _T("\xd7\xd4\xb6\xaf\xb3\xd4\xd2\xa9\xd3\xeb\xd4\xb6\xb3\xcc\xb2\xb9\xb8\xf8"), WS_VISIBLE | WS_CHILD | BS_GROUPBOX, 20, 100, 335, 130, hwnd, NULL, NULL, NULL);
        SendMessage(hCtrl, WM_SETFONT, (WPARAM)hFont, TRUE);
        g_hTab0Controls[g_nTab0Count++] = hCtrl;
        
        hCtrl = CreateWindow(_T("BUTTON"), _T("\xba\xec\xd2\xa9"), WS_VISIBLE | WS_CHILD | BS_AUTOCHECKBOX, 30, 125, 50, 20, hwnd, (HMENU)ID_CHECK_AUTOHP, NULL, NULL);
        SendMessage(hCtrl, BM_SETCHECK, g_bAutoHP ? BST_CHECKED : BST_UNCHECKED, 0);
        SendMessage(hCtrl, WM_SETFONT, (WPARAM)hFont, TRUE);
        g_hTab0Controls[g_nTab0Count++] = hCtrl;
        
        HWND hHPPotionName = CreateWindow(_T("EDIT"), _T(""), WS_VISIBLE | WS_CHILD | WS_BORDER | ES_AUTOHSCROLL, 85, 123, 100, 20, hwnd, (HMENU)ID_EDIT_HPPOTIONNAME, NULL, NULL);
        SendMessage(hHPPotionName, WM_SETFONT, (WPARAM)hFont, TRUE);
        SetWindowText(hHPPotionName, g_szHPPotionName);
        g_hTab0Controls[g_nTab0Count++] = hHPPotionName;
        
        hCtrl = CreateWindow(_T("STATIC"), _T("\xe3\xd0\xd6\xb5\x3a"), WS_VISIBLE | WS_CHILD, 195, 125, 35, 20, hwnd, NULL, NULL, NULL);
        SendMessage(hCtrl, WM_SETFONT, (WPARAM)hFont, TRUE);
        g_hTab0Controls[g_nTab0Count++] = hCtrl;
        
        HWND hHPPercent = CreateWindow(_T("EDIT"), _T(""), WS_VISIBLE | WS_CHILD | WS_BORDER | ES_NUMBER, 235, 123, 30, 20, hwnd, (HMENU)ID_EDIT_HPPERCENT, NULL, NULL);
        SendMessage(hHPPercent, WM_SETFONT, (WPARAM)hFont, TRUE);
        _sntprintf(szTmp, 16, _T("%d"), g_nHPPercent);
        SetWindowText(hHPPercent, szTmp);
        g_hTab0Controls[g_nTab0Count++] = hHPPercent;
        
        hCtrl = CreateWindow(_T("STATIC"), _T("%"), WS_VISIBLE | WS_CHILD, 270, 125, 15, 20, hwnd, NULL, NULL, NULL);
        SendMessage(hCtrl, WM_SETFONT, (WPARAM)hFont, TRUE);
        g_hTab0Controls[g_nTab0Count++] = hCtrl;
        
        hCtrl = CreateWindow(_T("BUTTON"), _T("\xb2\xb9\xbb\xf5"), WS_VISIBLE | WS_CHILD | BS_AUTOCHECKBOX, 285, 123, 60, 20, hwnd, (HMENU)ID_CHECK_AUTOBUYHP, NULL, NULL);
        SendMessage(hCtrl, BM_SETCHECK, g_bAutoBuyHP ? BST_CHECKED : BST_UNCHECKED, 0);
        SendMessage(hCtrl, WM_SETFONT, (WPARAM)hFont, TRUE);
        g_hTab0Controls[g_nTab0Count++] = hCtrl;
        
        hCtrl = CreateWindow(_T("BUTTON"), _T("\xc0\xb6\xd2\xa9"), WS_VISIBLE | WS_CHILD | BS_AUTOCHECKBOX, 30, 175, 50, 20, hwnd, (HMENU)ID_CHECK_AUTOMP, NULL, NULL);
        SendMessage(hCtrl, BM_SETCHECK, g_bAutoMP ? BST_CHECKED : BST_UNCHECKED, 0);
        SendMessage(hCtrl, WM_SETFONT, (WPARAM)hFont, TRUE);
        g_hTab0Controls[g_nTab0Count++] = hCtrl;
        
        HWND hMPPotionName = CreateWindow(_T("EDIT"), _T(""), WS_VISIBLE | WS_CHILD | WS_BORDER | ES_AUTOHSCROLL, 85, 173, 100, 20, hwnd, (HMENU)ID_EDIT_MPPOTIONNAME, NULL, NULL);
        SendMessage(hMPPotionName, WM_SETFONT, (WPARAM)hFont, TRUE);
        SetWindowText(hMPPotionName, g_szMPPotionName);
        g_hTab0Controls[g_nTab0Count++] = hMPPotionName;
        
        hCtrl = CreateWindow(_T("STATIC"), _T("\xe3\xd0\xd6\xb5\x3a"), WS_VISIBLE | WS_CHILD, 195, 175, 35, 20, hwnd, NULL, NULL, NULL);
        SendMessage(hCtrl, WM_SETFONT, (WPARAM)hFont, TRUE);
        g_hTab0Controls[g_nTab0Count++] = hCtrl;
        
        HWND hMPPercent = CreateWindow(_T("EDIT"), _T(""), WS_VISIBLE | WS_CHILD | WS_BORDER | ES_NUMBER, 235, 173, 30, 20, hwnd, (HMENU)ID_EDIT_MPPERCENT, NULL, NULL);
        SendMessage(hMPPercent, WM_SETFONT, (WPARAM)hFont, TRUE);
        _sntprintf(szTmp, 16, _T("%d"), g_nMPPercent);
        SetWindowText(hMPPercent, szTmp);
        g_hTab0Controls[g_nTab0Count++] = hMPPercent;
        
        hCtrl = CreateWindow(_T("STATIC"), _T("%"), WS_VISIBLE | WS_CHILD, 270, 175, 15, 20, hwnd, NULL, NULL, NULL);
        SendMessage(hCtrl, WM_SETFONT, (WPARAM)hFont, TRUE);
        g_hTab0Controls[g_nTab0Count++] = hCtrl;
        
        hCtrl = CreateWindow(_T("BUTTON"), _T("\xb2\xb9\xbb\xf5"), WS_VISIBLE | WS_CHILD | BS_AUTOCHECKBOX, 285, 173, 60, 20, hwnd, (HMENU)ID_CHECK_AUTOBUYMP, NULL, NULL);
        SendMessage(hCtrl, BM_SETCHECK, g_bAutoBuyMP ? BST_CHECKED : BST_UNCHECKED, 0);
        SendMessage(hCtrl, WM_SETFONT, (WPARAM)hFont, TRUE);
        g_hTab0Controls[g_nTab0Count++] = hCtrl;
        
        hCtrl = CreateWindow(_T("BUTTON"), _T("\xb3\xe8\xce\xef\xd6\xfa\xca\xd6\xc9\xe8\xd6\xc3"), WS_VISIBLE | WS_CHILD | BS_GROUPBOX, 20, 240, 335, 130, hwnd, NULL, NULL, NULL);
        SendMessage(hCtrl, WM_SETFONT, (WPARAM)hFont, TRUE);
        g_hTab0Controls[g_nTab0Count++] = hCtrl;
        
        hCtrl = CreateWindow(_T("BUTTON"), _T("\xc6\xf4\xd3\xc3\xd7\xd4\xb6\xaf\xb3\xe8\xce\xef\xd6\xfa\xca\xd6\x20\x28\xce\xb9\xca\xb3\x2f\xb2\xb9\xd1\xaa\x2f\x42\x75\x66\x66\x2f\xd7\xd4\xb6\xaf\xd5\xd9\xbb\xd8\x29"), WS_VISIBLE | WS_CHILD | BS_AUTOCHECKBOX, 30, 265, 310, 20, hwnd, (HMENU)ID_CHECK_AUTOPET, NULL, NULL);
        SendMessage(hCtrl, BM_SETCHECK, g_bAutoPet ? BST_CHECKED : BST_UNCHECKED, 0);
        SendMessage(hCtrl, WM_SETFONT, (WPARAM)hFont, TRUE);
        g_hTab0Controls[g_nTab0Count++] = hCtrl;
        
        hCtrl = CreateWindow(_T("STATIC"), _T("\xd2\xb0\xd0\xd4\xe3\xd0\xd6\xb5\x3a"), WS_VISIBLE | WS_CHILD, 30, 305, 60, 20, hwnd, NULL, NULL, NULL);
        SendMessage(hCtrl, WM_SETFONT, (WPARAM)hFont, TRUE);
        g_hTab0Controls[g_nTab0Count++] = hCtrl;
        
        HWND hPetWildRate = CreateWindow(_T("EDIT"), _T(""), WS_VISIBLE | WS_CHILD | WS_BORDER | ES_NUMBER, 90, 303, 30, 20, hwnd, (HMENU)ID_EDIT_PETWILDRATE, NULL, NULL);
        SendMessage(hPetWildRate, WM_SETFONT, (WPARAM)hFont, TRUE);
        _sntprintf(szTmp, 16, _T("%d"), g_nPetWildRate);
        SetWindowText(hPetWildRate, szTmp);
        g_hTab0Controls[g_nTab0Count++] = hPetWildRate;
        
        hCtrl = CreateWindow(_T("STATIC"), _T("\xcb\xc7\xc1\xcf\x49\x44\x3a"), WS_VISIBLE | WS_CHILD, 130, 305, 50, 20, hwnd, NULL, NULL, NULL);
        SendMessage(hCtrl, WM_SETFONT, (WPARAM)hFont, TRUE);
        g_hTab0Controls[g_nTab0Count++] = hCtrl;
        
        HWND hPetFoodID = CreateWindow(_T("EDIT"), _T(""), WS_VISIBLE | WS_CHILD | WS_BORDER | ES_NUMBER, 180, 303, 40, 20, hwnd, (HMENU)ID_EDIT_PETFOODID, NULL, NULL);
        SendMessage(hPetFoodID, WM_SETFONT, (WPARAM)hFont, TRUE);
        _sntprintf(szTmp, 16, _T("%u"), g_dwPetFoodID);
        SetWindowText(hPetFoodID, szTmp);
        g_hTab0Controls[g_nTab0Count++] = hPetFoodID;
        
        hCtrl = CreateWindow(_T("STATIC"), _T("\xb7\xe2\xd3\xa1\x49\x44\x3a"), WS_VISIBLE | WS_CHILD, 230, 305, 50, 20, hwnd, NULL, NULL, NULL);
        SendMessage(hCtrl, WM_SETFONT, (WPARAM)hFont, TRUE);
        g_hTab0Controls[g_nTab0Count++] = hCtrl;
        
        HWND hPetSealID = CreateWindow(_T("EDIT"), _T(""), WS_VISIBLE | WS_CHILD | WS_BORDER | ES_NUMBER, 280, 303, 40, 20, hwnd, (HMENU)ID_EDIT_PETSEALID, NULL, NULL);
        SendMessage(hPetSealID, WM_SETFONT, (WPARAM)hFont, TRUE);
        _sntprintf(szTmp, 16, _T("%u"), g_dwPetSealID);
        SetWindowText(hPetSealID, szTmp);
        g_hTab0Controls[g_nTab0Count++] = hPetSealID;
        
        hCtrl = CreateWindow(_T("BUTTON"), _T("\xca\xb0\xc8\xa1\xb9\xfd\xc2\xcb\xc1\xd0\xb1\xed\x20\x28\xc3\xbf\xd0\xd0\xd2\xbb\xb8\xf6\x29"), WS_VISIBLE | WS_CHILD | BS_GROUPBOX, 365, 40, 230, 330, hwnd, NULL, NULL, NULL);
        SendMessage(hCtrl, WM_SETFONT, (WPARAM)hFont, TRUE);
        g_hTab0Controls[g_nTab0Count++] = hCtrl;
        
        HWND hFilterEdit = CreateWindow(_T("EDIT"), _T(""), WS_VISIBLE | WS_CHILD | WS_BORDER | ES_MULTILINE | ES_AUTOVSCROLL | ES_WANTRETURN | WS_VSCROLL, 375, 65, 210, 295, hwnd, (HMENU)ID_EDIT_FILTERLIST, NULL, NULL);
        SendMessage(hFilterEdit, WM_SETFONT, (WPARAM)hFont, TRUE);
        SetWindowText(hFilterEdit, g_szFilterList);
        g_hTab0Controls[g_nTab0Count++] = hFilterEdit;
        
        
        hCtrl = CreateWindow(_T("BUTTON"), _T("\xd7\xd4\xb6\xaf\xca\xa9\xb7\xa8\xbc\xbc\xc4\xdc\xc9\xe8\xd6\xc3"), WS_VISIBLE | WS_CHILD | BS_GROUPBOX, 20, 40, 575, 330, hwnd, NULL, NULL, NULL);
        SendMessage(hCtrl, WM_SETFONT, (WPARAM)hFont, TRUE);
        g_hTab1Controls[g_nTab1Count++] = hCtrl;
        
        hCtrl = CreateWindow(_T("STATIC"), _T("\xb9\xa5\xbb\xf7\xbc\xbc\xc4\xdc\x3a"), WS_VISIBLE | WS_CHILD, 40, 70, 70, 20, hwnd, NULL, NULL, NULL);
        SendMessage(hCtrl, WM_SETFONT, (WPARAM)hFont, TRUE);
        g_hTab1Controls[g_nTab1Count++] = hCtrl;
        
        HWND hComboAttack = CreateWindow(_T("COMBOBOX"), _T(""), WS_VISIBLE | WS_CHILD | CBS_DROPDOWNLIST | WS_VSCROLL, 120, 68, 220, 200, hwnd, (HMENU)ID_COMBO_ATTACKSKILL, NULL, NULL);
        SendMessage(hComboAttack, WM_SETFONT, (WPARAM)hFont, TRUE);
        PopulateSkillComboBox(hComboAttack, g_dwAttackSkillID);
        g_hTab1Controls[g_nTab1Count++] = hComboAttack;
        
        hCtrl = CreateWindow(_T("STATIC"), _T("\xca\xa9\xb7\xc5\xbc\xe4\xb8\xf4\x3a"), WS_VISIBLE | WS_CHILD, 360, 70, 70, 20, hwnd, NULL, NULL, NULL);
        SendMessage(hCtrl, WM_SETFONT, (WPARAM)hFont, TRUE);
        g_hTab1Controls[g_nTab1Count++] = hCtrl;
        
        HWND hEditAttackInt = CreateWindow(_T("EDIT"), _T(""), WS_VISIBLE | WS_CHILD | WS_BORDER | ES_NUMBER, 430, 68, 50, 20, hwnd, (HMENU)ID_EDIT_ATTACKINTERVAL, NULL, NULL);
        SendMessage(hEditAttackInt, WM_SETFONT, (WPARAM)hFont, TRUE);
        _sntprintf(szTmp, 16, _T("%d"), g_nAttackSkillInterval);
        SetWindowText(hEditAttackInt, szTmp);
        g_hTab1Controls[g_nTab1Count++] = hEditAttackInt;
        
        hCtrl = CreateWindow(_T("STATIC"), _T("\xc3\xeb\x20\x28\xd7\xee\xb5\xcd\x31\xc3\xeb\x29"), WS_VISIBLE | WS_CHILD, 490, 70, 90, 20, hwnd, NULL, NULL, NULL);
        SendMessage(hCtrl, WM_SETFONT, (WPARAM)hFont, TRUE);
        g_hTab1Controls[g_nTab1Count++] = hCtrl;
        
        hCtrl = CreateWindow(_T("STATIC"), _T("\xd7\xb4\xcc\xac\xbc\xbc\xc4\xdc\x20\x31\x3a"), WS_VISIBLE | WS_CHILD, 40, 120, 70, 20, hwnd, NULL, NULL, NULL);
        SendMessage(hCtrl, WM_SETFONT, (WPARAM)hFont, TRUE);
        g_hTab1Controls[g_nTab1Count++] = hCtrl;
        
        HWND hComboBuff1 = CreateWindow(_T("COMBOBOX"), _T(""), WS_VISIBLE | WS_CHILD | CBS_DROPDOWNLIST | WS_VSCROLL, 120, 118, 220, 200, hwnd, (HMENU)ID_COMBO_BUFFSKILL1, NULL, NULL);
        SendMessage(hComboBuff1, WM_SETFONT, (WPARAM)hFont, TRUE);
        PopulateSkillComboBox(hComboBuff1, g_dwBuffSkillID1);
        g_hTab1Controls[g_nTab1Count++] = hComboBuff1;
        
        hCtrl = CreateWindow(_T("STATIC"), _T("\xca\xa9\xb7\xc5\xbc\xe4\xb8\xf4\x3a"), WS_VISIBLE | WS_CHILD, 360, 120, 70, 20, hwnd, NULL, NULL, NULL);
        SendMessage(hCtrl, WM_SETFONT, (WPARAM)hFont, TRUE);
        g_hTab1Controls[g_nTab1Count++] = hComboBuff1; // wait, let's keep track of original controls
        g_hTab1Controls[g_nTab1Count++] = hCtrl;
        
        HWND hEditBuffInt1 = CreateWindow(_T("EDIT"), _T(""), WS_VISIBLE | WS_CHILD | WS_BORDER | ES_NUMBER, 430, 118, 50, 20, hwnd, (HMENU)ID_EDIT_BUFFINTERVAL1, NULL, NULL);
        SendMessage(hEditBuffInt1, WM_SETFONT, (WPARAM)hFont, TRUE);
        _sntprintf(szTmp, 16, _T("%d"), g_nBuffSkillInterval1);
        SetWindowText(hEditBuffInt1, szTmp);
        g_hTab1Controls[g_nTab1Count++] = hEditBuffInt1;
        
        hCtrl = CreateWindow(_T("STATIC"), _T("\xc3\xeb\x20\x28\xd7\xee\xb5\xcd\x31\xc3\xeb\x29"), WS_VISIBLE | WS_CHILD, 490, 120, 90, 20, hwnd, NULL, NULL, NULL);
        SendMessage(hCtrl, WM_SETFONT, (WPARAM)hFont, TRUE);
        g_hTab1Controls[g_nTab1Count++] = hCtrl;
        
        hCtrl = CreateWindow(_T("STATIC"), _T("\xd7\xb4\xcc\xac\xbc\xbc\xc4\xdc\x20\x32\x3a"), WS_VISIBLE | WS_CHILD, 40, 170, 70, 20, hwnd, NULL, NULL, NULL);
        SendMessage(hCtrl, WM_SETFONT, (WPARAM)hFont, TRUE);
        g_hTab1Controls[g_nTab1Count++] = hCtrl;
        
        HWND hComboBuff2 = CreateWindow(_T("COMBOBOX"), _T(""), WS_VISIBLE | WS_CHILD | CBS_DROPDOWNLIST | WS_VSCROLL, 120, 168, 220, 200, hwnd, (HMENU)ID_COMBO_BUFFSKILL2, NULL, NULL);
        SendMessage(hComboBuff2, WM_SETFONT, (WPARAM)hFont, TRUE);
        PopulateSkillComboBox(hComboBuff2, g_dwBuffSkillID2);
        g_hTab1Controls[g_nTab1Count++] = hComboBuff2;
        
        hCtrl = CreateWindow(_T("STATIC"), _T("\xca\xa9\xb7\xc5\xbc\xe4\xb8\xf4\x3a"), WS_VISIBLE | WS_CHILD, 360, 170, 70, 20, hwnd, NULL, NULL, NULL);
        SendMessage(hCtrl, WM_SETFONT, (WPARAM)hFont, TRUE);
        g_hTab1Controls[g_nTab1Count++] = hCtrl;
        
        HWND hEditBuffInt2 = CreateWindow(_T("EDIT"), _T(""), WS_VISIBLE | WS_CHILD | WS_BORDER | ES_NUMBER, 430, 168, 50, 20, hwnd, (HMENU)ID_EDIT_BUFFINTERVAL2, NULL, NULL);
        SendMessage(hEditBuffInt2, WM_SETFONT, (WPARAM)hFont, TRUE);
        _sntprintf(szTmp, 16, _T("%d"), g_nBuffSkillInterval2);
        SetWindowText(hEditBuffInt2, szTmp);
        g_hTab1Controls[g_nTab1Count++] = hEditBuffInt2;
        
        hCtrl = CreateWindow(_T("STATIC"), _T("\xc3\xeb\x20\x28\xd7\xee\xb5\xcd\x31\xc3\xeb\x29"), WS_VISIBLE | WS_CHILD, 490, 170, 90, 20, hwnd, NULL, NULL, NULL);
        SendMessage(hCtrl, WM_SETFONT, (WPARAM)hFont, TRUE);
        g_hTab1Controls[g_nTab1Count++] = hCtrl;
        
        hCtrl = CreateWindow(_T("STATIC"), _T("\xb8\xa8\xd6\xfa\xb6\xd3\xd3\xd1\x20\x31\x3a"), WS_VISIBLE | WS_CHILD, 40, 220, 70, 20, hwnd, NULL, NULL, NULL);
        SendMessage(hCtrl, WM_SETFONT, (WPARAM)hFont, TRUE);
        g_hTab1Controls[g_nTab1Count++] = hCtrl;
        
        HWND hComboTeam1 = CreateWindow(_T("COMBOBOX"), _T(""), WS_VISIBLE | WS_CHILD | CBS_DROPDOWNLIST | WS_VSCROLL, 120, 218, 220, 200, hwnd, (HMENU)ID_COMBO_TEAMMATESKILL1, NULL, NULL);
        SendMessage(hComboTeam1, WM_SETFONT, (WPARAM)hFont, TRUE);
        PopulateSkillComboBox(hComboTeam1, g_dwTeammateSkillID1);
        g_hTab1Controls[g_nTab1Count++] = hComboTeam1;
        
        hCtrl = CreateWindow(_T("STATIC"), _T("\xca\xa9\xb7\xc5\xbc\xe4\xb8\xf4\x3a"), WS_VISIBLE | WS_CHILD, 360, 220, 70, 20, hwnd, NULL, NULL, NULL);
        SendMessage(hCtrl, WM_SETFONT, (WPARAM)hFont, TRUE);
        g_hTab1Controls[g_nTab1Count++] = hCtrl;
        
        HWND hEditTeamInt1 = CreateWindow(_T("EDIT"), _T(""), WS_VISIBLE | WS_CHILD | WS_BORDER | ES_NUMBER, 430, 218, 50, 20, hwnd, (HMENU)ID_EDIT_TEAMMATEINTERVAL1, NULL, NULL);
        SendMessage(hEditTeamInt1, WM_SETFONT, (WPARAM)hFont, TRUE);
        _sntprintf(szTmp, 16, _T("%d"), g_nTeammateSkillInterval1);
        SetWindowText(hEditTeamInt1, szTmp);
        g_hTab1Controls[g_nTab1Count++] = hEditTeamInt1;
        
        hCtrl = CreateWindow(_T("STATIC"), _T("\xc3\xeb\x20\x28\xd7\xee\xb5\xcd\x31\xc3\xeb\x29"), WS_VISIBLE | WS_CHILD, 490, 220, 90, 20, hwnd, NULL, NULL, NULL);
        SendMessage(hCtrl, WM_SETFONT, (WPARAM)hFont, TRUE);
        g_hTab1Controls[g_nTab1Count++] = hCtrl;
        
        hCtrl = CreateWindow(_T("STATIC"), _T("\xb8\xa8\xd6\xfa\xb6\xd3\xd3\xd1\x20\x32\x3a"), WS_VISIBLE | WS_CHILD, 40, 270, 70, 20, hwnd, NULL, NULL, NULL);
        SendMessage(hCtrl, WM_SETFONT, (WPARAM)hFont, TRUE);
        g_hTab1Controls[g_nTab1Count++] = hCtrl;
        
        HWND hComboTeam2 = CreateWindow(_T("COMBOBOX"), _T(""), WS_VISIBLE | WS_CHILD | CBS_DROPDOWNLIST | WS_VSCROLL, 120, 268, 220, 200, hwnd, (HMENU)ID_COMBO_TEAMMATESKILL2, NULL, NULL);
        SendMessage(hComboTeam2, WM_SETFONT, (WPARAM)hFont, TRUE);
        PopulateSkillComboBox(hComboTeam2, g_dwTeammateSkillID2);
        g_hTab1Controls[g_nTab1Count++] = hComboTeam2;
        
        hCtrl = CreateWindow(_T("STATIC"), _T("\xca\xa9\xb7\xc5\xbc\xe4\xb8\xf4\x3a"), WS_VISIBLE | WS_CHILD, 360, 270, 70, 20, hwnd, NULL, NULL, NULL);
        SendMessage(hCtrl, WM_SETFONT, (WPARAM)hFont, TRUE);
        g_hTab1Controls[g_nTab1Count++] = hCtrl;
        
        HWND hEditTeamInt2 = CreateWindow(_T("EDIT"), _T(""), WS_VISIBLE | WS_CHILD | WS_BORDER | ES_NUMBER, 430, 268, 50, 20, hwnd, (HMENU)ID_EDIT_TEAMMATEINTERVAL2, NULL, NULL);
        SendMessage(hEditTeamInt2, WM_SETFONT, (WPARAM)hFont, TRUE);
        _sntprintf(szTmp, 16, _T("%d"), g_nTeammateSkillInterval2);
        SetWindowText(hEditTeamInt2, szTmp);
        g_hTab1Controls[g_nTab1Count++] = hEditTeamInt2;
        
        hCtrl = CreateWindow(_T("STATIC"), _T("\xc3\xeb\x20\x28\xd7\xee\xb5\xcd\x31\xc3\xeb\x29"), WS_VISIBLE | WS_CHILD, 490, 270, 90, 20, hwnd, NULL, NULL, NULL);
        SendMessage(hCtrl, WM_SETFONT, (WPARAM)hFont, TRUE);
        g_hTab1Controls[g_nTab1Count++] = hCtrl;
        
        hCtrl = CreateWindow(_T("STATIC"), _T("\xcc\xe1\xca\xbe\x3a\x20\xb8\xa8\xd6\xfa\xb6\xd3\xd3\xd1\xbc\xbc\xc4\xdc\xbb\xe1\xd4\xda\xb9\xd2\xbb\xfa\xc6\xda\xbc\xe4\xd7\xd4\xb6\xaf\xce\xaa\xb4\xa6\xd3\xda\xb4\xe6\xbb\xee\xd7\xb4\xcc\xac\xb5\xc4\xb6\xd3\xd3\xd1\xca\xa9\xb7\xc5\xa1\xa3"), WS_VISIBLE | WS_CHILD, 40, 325, 520, 20, hwnd, NULL, NULL, NULL);
        SendMessage(hCtrl, WM_SETFONT, (WPARAM)hFont, TRUE);
        g_hTab1Controls[g_nTab1Count++] = hCtrl;
        
        
        hCtrl = CreateWindow(_T("BUTTON"), _T("\xd7\xd4\xb6\xaf\xd4\xb6\xb3\xcc\xca\xdb\xc2\xf4\xc9\xe8\xd6\xc3"), WS_VISIBLE | WS_CHILD | BS_GROUPBOX, 20, 40, 575, 330, hwnd, NULL, NULL, NULL);
        SendMessage(hCtrl, WM_SETFONT, (WPARAM)hFont, TRUE);
        g_hTab2Controls[g_nTab2Count++] = hCtrl;
        
        hCtrl = CreateWindow(_T("BUTTON"), _T("\xc6\xf4\xd3\xc3\xb0\xfc\xb9\xfc\xc2\xfa\xca\xb1\xd7\xd4\xb6\xaf\xd4\xb6\xb3\xcc\xca\xdb\xc2\xf4\xce\xef\xc6\xb7"), WS_VISIBLE | WS_CHILD | BS_AUTOCHECKBOX, 35, 70, 310, 20, hwnd, (HMENU)ID_CHECK_AUTOSELL_FULL, NULL, NULL);
        SendMessage(hCtrl, BM_SETCHECK, g_bAutoSellFull ? BST_CHECKED : BST_UNCHECKED, 0);
        SendMessage(hCtrl, WM_SETFONT, (WPARAM)hFont, TRUE);
        g_hTab2Controls[g_nTab2Count++] = hCtrl;
        
        hCtrl = CreateWindow(_T("BUTTON"), _T("\xd7\xd4\xb6\xaf\xb3\xf6\xca\xdb\xce\xef\xc6\xb7"), WS_VISIBLE | WS_CHILD | BS_AUTOCHECKBOX, 35, 100, 310, 20, hwnd, (HMENU)ID_CHECK_AUTOSELL_ALL, NULL, NULL);
        SendMessage(hCtrl, BM_SETCHECK, g_bAutoSellAll ? BST_CHECKED : BST_UNCHECKED, 0);
        SendMessage(hCtrl, WM_SETFONT, (WPARAM)hFont, TRUE);
        g_hTab2Controls[g_nTab2Count++] = hCtrl;

        hCtrl = CreateWindow(_T("BUTTON"), _T("\xc6\xf4\xd3\xc3\xbd\xfb\xca\xdb\xb9\xfd\xc2\xcb"), WS_VISIBLE | WS_CHILD | BS_AUTOCHECKBOX, 35, 130, 310, 20, hwnd, (HMENU)ID_CHECK_BANSELL_FILTER, NULL, NULL);
        SendMessage(hCtrl, BM_SETCHECK, g_bBanSellFilter ? BST_CHECKED : BST_UNCHECKED, 0);
        SendMessage(hCtrl, WM_SETFONT, (WPARAM)hFont, TRUE);
        g_hTab2Controls[g_nTab2Count++] = hCtrl;
        
        hCtrl = CreateWindow(_T("STATIC"), _T("\xcb\xb5\xc3\xf7\x3a\x5c\x72\x5c\x6e\x31\x2e\x20\xbf\xaa\xc6\xf4\xd4\xb6\xb3\xcc\xca\xdb\xc2\xf4\xba\xf3\xa3\xac\xb5\xb1\xb9\xd2\xbb\xfa\xb9\xfd\xb3\xcc\xd6\xd0\xb0\xfc\xb9\xfc\xc8\xab\xc2\xfa\xa3\xac\xb8\xa8\xd6\xfa\xbb\xe1\xd7\xd4\xb6\xaf\xb5\xf7\xd3\xc3\xd4\xb6\xb3\xcc\xc9\xcc\xb5\xea\xb9\xa6\xc4\xdc\xa3\xac\xbd\xab\xb0\xfc\xb9\xfc\xc4\xda\xb5\xc4\xce\xef\xc6\xb7\xbd\xf8\xd0\xd0\xca\xdb\xc2\xf4\xd2\xd4\xcc\xda\xb3\xf6\xbf\xd5\xbc\xe4\xa1\xa3\x5c\x72\x5c\x6e\x32\x2e\x20\xb9\xb4\xd1\xa1\xa1\xb8\xc6\xf4\xd3\xc3\xbd\xfb\xca\xdb\xb9\xfd\xc2\xcb\xa1\xb9\xba\xf3\xa3\xac\xbd\xfb\xca\xdb\xc1\xd0\xb1\xed\xc4\xda\xb5\xc4\xce\xef\xc6\xb7\xd4\xda\xca\xdb\xc2\xf4\xca\xb1\xbb\xe1\xb1\xbb\xb1\xa3\xc1\xf4\xa3\xac\xb2\xbb\xb9\xb4\xd1\xa1\xd4\xf2\xc8\xab\xc2\xf4\xa1\xa3\x5c\x72\x5c\x6e\x33\x2e\x20\xbb\xf9\xb4\xa1\xc9\xe8\xd6\xc3\xd6\xd0\xb5\xc4\xa1\xb8\xd7\xd4\xb6\xaf\xb3\xd4\xd2\xa9\xd3\xeb\xd4\xb6\xb3\xcc\xb2\xb9\xb8\xf8\xa1\xb9\xbf\xc9\xd7\xd4\xb6\xaf\xb9\xba\xc2\xf2\xd2\xa9\xc6\xb7\xa3\xac\xd3\xeb\xb4\xcb\xb9\xa6\xc4\xdc\xc5\xe4\xba\xcf\xbf\xc9\xca\xb5\xcf\xd6\xce\xde\xb8\xc9\xd4\xa4\xb5\xc4\xb9\xd2\xbb\xfa\xa1\xa3"), WS_VISIBLE | WS_CHILD, 35, 170, 310, 190, hwnd, NULL, NULL, NULL);
        SendMessage(hCtrl, WM_SETFONT, (WPARAM)hFont, TRUE);
        g_hTab2Controls[g_nTab2Count++] = hCtrl;

        hCtrl = CreateWindow(_T("BUTTON"), _T("\xbd\xfb\xca\xdb\xce\xef\xc6\xb7\xc1\xd0\xb1\xed\x20\x28\xc3\xbf\xd0\xd0\xd2\xbb\xb8\xf6\x29"), WS_VISIBLE | WS_CHILD | BS_GROUPBOX, 360, 60, 220, 295, hwnd, NULL, NULL, NULL);
        SendMessage(hCtrl, WM_SETFONT, (WPARAM)hFont, TRUE);
        g_hTab2Controls[g_nTab2Count++] = hCtrl;
        
        HWND hBanSellEdit = CreateWindow(_T("EDIT"), _T(""), WS_VISIBLE | WS_CHILD | WS_BORDER | ES_MULTILINE | ES_AUTOVSCROLL | ES_WANTRETURN | WS_VSCROLL, 370, 85, 200, 260, hwnd, (HMENU)ID_EDIT_BANSELL_LIST, NULL, NULL);
        SendMessage(hBanSellEdit, WM_SETFONT, (WPARAM)hFont, TRUE);
        SetWindowText(hBanSellEdit, g_szBanSellList);
        g_hTab2Controls[g_nTab2Count++] = hBanSellEdit;
        
        ShowTabControls(0);
        
        LPCTSTR pszToggleText = g_bAutoAttack ? _T("\xbd\xe1\xca\xf8\xb9\xd2\xbb\xfa") : _T("\xbf\xaa\xca\xbc\xb9\xd2\xbb\xfa");
        HWND hBtnToggle = CreateWindow(_T("BUTTON"), pszToggleText, WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON, 130, 395, 110, 30, hwnd, (HMENU)ID_BUTTON_TOGGLE, NULL, NULL);
        SendMessage(hBtnToggle, WM_SETFONT, (WPARAM)hFont, TRUE);
        
        HWND hBtnSave = CreateWindow(_T("BUTTON"), _T("\xb1\xa3\xb4\xe6\xc9\xe8\xd6\xc3"), WS_VISIBLE | WS_CHILD, 260, 395, 110, 30, hwnd, (HMENU)ID_BUTTON_SAVE, NULL, NULL);
        SendMessage(hBtnSave, WM_SETFONT, (WPARAM)hFont, TRUE);
        
        HWND hBtnCancel = CreateWindow(_T("BUTTON"), _T("\xc8\xa1\xcf\xfb"), WS_VISIBLE | WS_CHILD, 390, 395, 110, 30, hwnd, (HMENU)ID_BUTTON_CANCEL, NULL, NULL);
        SendMessage(hBtnCancel, WM_SETFONT, (WPARAM)hFont, TRUE);
        break;
    }
    
    case WM_NOTIFY: {
        LPNMHDR pnmhdr = (LPNMHDR)lParam;
        if (pnmhdr->idFrom == ID_TABCONTROL && pnmhdr->code == TCN_SELCHANGE) {
            int iSel = TabCtrl_GetCurSel(pnmhdr->hwndFrom);
            ShowTabControls(iSel);
        }
        break;
    }
    
    case WM_COMMAND: {
        int id = LOWORD(wParam);
        if (id == ID_BUTTON_TOGGLE || id == ID_BUTTON_SAVE) {
            TCHAR szTmp[32];
            
            BOOL bOldAutoAttack = g_bAutoAttack;
            
            g_bAutoAttack = (SendMessage(GetDlgItem(hwnd, ID_CHECK_AUTOATTACK), BM_GETCHECK, 0, 0) == BST_CHECKED);
            g_bAutoLoot = (SendMessage(GetDlgItem(hwnd, ID_CHECK_AUTOLOOT), BM_GETCHECK, 0, 0) == BST_CHECKED);
            g_bAutoHP = (SendMessage(GetDlgItem(hwnd, ID_CHECK_AUTOHP), BM_GETCHECK, 0, 0) == BST_CHECKED);
            g_bAutoBuyHP = (SendMessage(GetDlgItem(hwnd, ID_CHECK_AUTOBUYHP), BM_GETCHECK, 0, 0) == BST_CHECKED);
            g_bAutoMP = (SendMessage(GetDlgItem(hwnd, ID_CHECK_AUTOMP), BM_GETCHECK, 0, 0) == BST_CHECKED);
            g_bAutoBuyMP = (SendMessage(GetDlgItem(hwnd, ID_CHECK_AUTOBUYMP), BM_GETCHECK, 0, 0) == BST_CHECKED);
            g_bAutoPet = (SendMessage(GetDlgItem(hwnd, ID_CHECK_AUTOPET), BM_GETCHECK, 0, 0) == BST_CHECKED);
            
            if (id == ID_BUTTON_TOGGLE) {
                g_bAutoAttack = !bOldAutoAttack;
            }
            
            extern BOOL g_bCheat;
            g_bCheat = g_bAutoAttack;
            
            GetWindowText(GetDlgItem(hwnd, ID_EDIT_HPPOTIONNAME), g_szHPPotionName, 32);
            GetWindowText(GetDlgItem(hwnd, ID_EDIT_MPPOTIONNAME), g_szMPPotionName, 32);
            
            GetWindowText(GetDlgItem(hwnd, ID_EDIT_HPPERCENT), szTmp, 32);
            g_nHPPercent = _ttoi(szTmp);
            if (g_nHPPercent < 0) g_nHPPercent = 0;
            if (g_nHPPercent > 100) g_nHPPercent = 100;
            
            GetWindowText(GetDlgItem(hwnd, ID_EDIT_MPPERCENT), szTmp, 32);
            g_nMPPercent = _ttoi(szTmp);
            if (g_nMPPercent < 0) g_nMPPercent = 0;
            if (g_nMPPercent > 100) g_nMPPercent = 100;
            
            GetWindowText(GetDlgItem(hwnd, ID_EDIT_PETWILDRATE), szTmp, 32);
            g_nPetWildRate = _ttoi(szTmp);
            
            GetWindowText(GetDlgItem(hwnd, ID_EDIT_PETFOODID), szTmp, 32);
            g_dwPetFoodID = (DWORD)_ttoi64(szTmp);
            
            GetWindowText(GetDlgItem(hwnd, ID_EDIT_PETSEALID), szTmp, 32);
            g_dwPetSealID = (DWORD)_ttoi64(szTmp);
            
            GetWindowText(GetDlgItem(hwnd, ID_EDIT_FILTERLIST), g_szFilterList, 4096);
            
            HWND hCombo;
            int curSel;
            
            hCombo = GetDlgItem(hwnd, ID_COMBO_ATTACKSKILL);
            curSel = SendMessage(hCombo, CB_GETCURSEL, 0, 0);
            if (curSel != CB_ERR) {
                g_dwAttackSkillID = (DWORD)SendMessage(hCombo, CB_GETITEMDATA, curSel, 0);
            }
            GetWindowText(GetDlgItem(hwnd, ID_EDIT_ATTACKINTERVAL), szTmp, 32);
            g_nAttackSkillInterval = _ttoi(szTmp);
            if (g_nAttackSkillInterval < 1) g_nAttackSkillInterval = 1;
            
            hCombo = GetDlgItem(hwnd, ID_COMBO_BUFFSKILL1);
            curSel = SendMessage(hCombo, CB_GETCURSEL, 0, 0);
            if (curSel != CB_ERR) {
                g_dwBuffSkillID1 = (DWORD)SendMessage(hCombo, CB_GETITEMDATA, curSel, 0);
            }
            GetWindowText(GetDlgItem(hwnd, ID_EDIT_BUFFINTERVAL1), szTmp, 32);
            g_nBuffSkillInterval1 = _ttoi(szTmp);
            if (g_nBuffSkillInterval1 < 1) g_nBuffSkillInterval1 = 1;
            
            hCombo = GetDlgItem(hwnd, ID_COMBO_BUFFSKILL2);
            curSel = SendMessage(hCombo, CB_GETCURSEL, 0, 0);
            if (curSel != CB_ERR) {
                g_dwBuffSkillID2 = (DWORD)SendMessage(hCombo, CB_GETITEMDATA, curSel, 0);
            }
            GetWindowText(GetDlgItem(hwnd, ID_EDIT_BUFFINTERVAL2), szTmp, 32);
            g_nBuffSkillInterval2 = _ttoi(szTmp);
            if (g_nBuffSkillInterval2 < 1) g_nBuffSkillInterval2 = 1;
            
            hCombo = GetDlgItem(hwnd, ID_COMBO_TEAMMATESKILL1);
            curSel = SendMessage(hCombo, CB_GETCURSEL, 0, 0);
            if (curSel != CB_ERR) {
                g_dwTeammateSkillID1 = (DWORD)SendMessage(hCombo, CB_GETITEMDATA, curSel, 0);
            }
            GetWindowText(GetDlgItem(hwnd, ID_EDIT_TEAMMATEINTERVAL1), szTmp, 32);
            g_nTeammateSkillInterval1 = _ttoi(szTmp);
            if (g_nTeammateSkillInterval1 < 1) g_nTeammateSkillInterval1 = 1;
            
            hCombo = GetDlgItem(hwnd, ID_COMBO_TEAMMATESKILL2);
            curSel = SendMessage(hCombo, CB_GETCURSEL, 0, 0);
            if (curSel != CB_ERR) {
                g_dwTeammateSkillID2 = (DWORD)SendMessage(hCombo, CB_GETITEMDATA, curSel, 0);
            }
            GetWindowText(GetDlgItem(hwnd, ID_EDIT_TEAMMATEINTERVAL2), szTmp, 32);
            g_nTeammateSkillInterval2 = _ttoi(szTmp);
            if (g_nTeammateSkillInterval2 < 1) g_nTeammateSkillInterval2 = 1;
            
            g_bAutoSellFull = (SendMessage(GetDlgItem(hwnd, ID_CHECK_AUTOSELL_FULL), BM_GETCHECK, 0, 0) == BST_CHECKED);
            g_bAutoSellAll = (SendMessage(GetDlgItem(hwnd, ID_CHECK_AUTOSELL_ALL), BM_GETCHECK, 0, 0) == BST_CHECKED);
            g_bBanSellFilter = (SendMessage(GetDlgItem(hwnd, ID_CHECK_BANSELL_FILTER), BM_GETCHECK, 0, 0) == BST_CHECKED);
            GetWindowText(GetDlgItem(hwnd, ID_EDIT_BANSELL_LIST), g_szBanSellList, 4096);
            
            ApplyCheatConfigToMainChar();
            SaveCheatConfig();
            DestroyWindow(hwnd);
        } else if (id == ID_BUTTON_CANCEL) {
            DestroyWindow(hwnd);
        }
        break;
    }
    
    case WM_CLOSE:
        DestroyWindow(hwnd);
        break;
        
    case WM_DESTROY:
        if (hFont) DeleteObject(hFont);
        g_hwndConfig = NULL;
        PostQuitMessage(0);
        break;
        
    default:
        return DefWindowProc(hwnd, msg, wParam, lParam);
    }
    return 0;
}

DWORD WINAPI ConfigThreadProc(LPVOID lpParam) {
    INITCOMMONCONTROLSEX icex;
    icex.dwSize = sizeof(INITCOMMONCONTROLSEX);
    icex.dwICC = ICC_TAB_CLASSES;
    InitCommonControlsEx(&icex);

    HWND hwndParent = (HWND)lpParam;
    HINSTANCE hInst = GetModuleHandle(NULL);
    
    WNDCLASSEX wc = {0};
    wc.cbSize = sizeof(WNDCLASSEX);
    wc.style = CS_HREDRAW | CS_VREDRAW;
    wc.lpfnWndProc = ConfigWndProc;
    wc.hInstance = hInst;
    wc.hCursor = LoadCursor(NULL, IDC_ARROW);
    wc.hbrBackground = (HBRUSH)(COLOR_BTNFACE + 1);
    wc.lpszClassName = _T("XiahConfigClass");
    
    RegisterClassEx(&wc);
    
    int w = 630;
    int h = 480;
    int x = CW_USEDEFAULT;
    int y = CW_USEDEFAULT;
    if (hwndParent) {
        RECT rect;
        GetWindowRect(hwndParent, &rect);
        x = rect.left + ((rect.right - rect.left) - w) / 2;
        y = rect.top + ((rect.bottom - rect.top) - h) / 2;
    }
    
    g_hwndConfig = CreateWindowEx(WS_EX_DLGMODALFRAME, _T("XiahConfigClass"), _T("\xb9\xd2\xbb\xfa\xb8\xa8\xd6\xfa"),
                                  WS_POPUP | WS_CAPTION | WS_SYSMENU | WS_VISIBLE,
                                  x, y, w, h, hwndParent, NULL, hInst, NULL);
    
    if (!g_hwndConfig) {
        UnregisterClass(_T("XiahConfigClass"), hInst);
        return 0;
    }
    
    ShowWindow(g_hwndConfig, SW_SHOW);
    UpdateWindow(g_hwndConfig);
    
    MSG msg;
    while (GetMessage(&msg, NULL, 0, 0)) {
        if (!IsDialogMessage(g_hwndConfig, &msg)) {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }
    
    UnregisterClass(_T("XiahConfigClass"), hInst);
    return 0;
}

void ShowCheatConfigWindow(HWND hwndParent) {
    if (g_hwndConfig && IsWindow(g_hwndConfig)) {
        SendMessage(g_hwndConfig, WM_CLOSE, 0, 0);
        return;
    }
    
    if (g_hConfigThread) {
        CloseHandle(g_hConfigThread);
        g_hConfigThread = NULL;
    }
    
    g_hConfigThread = CreateThread(NULL, 0, ConfigThreadProc, (LPVOID)hwndParent, 0, NULL);
}

BOOL IsBanSellFiltered(const TCHAR* szItemName) {
    if (!szItemName || _tcslen(szItemName) == 0) return FALSE;
    TCHAR szListCopy[4096];
    _tcscpy_s(szListCopy, 4096, g_szBanSellList);
    TCHAR* context = NULL;
    TCHAR* token = _tcstok_s(szListCopy, _T("\r\n|,"), &context);
    while (token != NULL) {
        TCHAR szTrimmed[128] = {0};
        int len = _tcslen(token);
        int start = 0;
        while (start < len && (token[start] == _T(' ') || token[start] == _T('\t'))) {
            start++;
        }
        int end = len - 1;
        while (end >= start && (token[end] == _T(' ') || token[end] == _T('\t'))) {
            end--;
        }
        if (end >= start) {
            _tcsncpy(szTrimmed, token + start, end - start + 1);
            szTrimmed[end - start + 1] = _T('\0');
            if (_tcscmp(szTrimmed, szItemName) == 0) {
                return TRUE;
            }
        }
        token = _tcstok_s(NULL, _T("\r\n|,"), &context);
    }
    return FALSE;
}

static DWORD s_dwLastSellTime = 0;
static BOOL s_bSellingProcessActive = FALSE;

BOOL IsSackFull(CSack* pSack) {
    if (!pSack) return FALSE;
    for (int i = 12; i < 36; ++i) {
        if (pSack->FindSackItemByPos(i) == NULL) {
            return FALSE;
        }
    }
    return TRUE;
}

BOOL IsBagFull() {
    return IsSackFull(g_MainCharInfo.m_pMySack[0]) || IsSackFull(g_MainCharInfo.m_pMySack[1]);
}

void ProcessAutoSell() {
    if (!g_bCheat || !g_bAutoSellFull) {
        s_bSellingProcessActive = FALSE;
        return;
    }

    static DWORD s_dwLastScanTime = 0;

    if (!s_bSellingProcessActive) {
        if (g_dwCurTime - s_dwLastScanTime < 5000) {
            return;
        }
        if (IsBagFull()) {
            s_bSellingProcessActive = TRUE;
            s_dwLastScanTime = g_dwCurTime;
        }
    }

    if (s_bSellingProcessActive) {
        if (g_dwCurTime - s_dwLastSellTime < 500) {
            return;
        }

        BOOL bFoundSellable = FALSE;
        CSack* pSacks[2] = { g_MainCharInfo.m_pMySack[0], g_MainCharInfo.m_pMySack[1] };
        for (int sackIdx = 0; sackIdx < 2; ++sackIdx) {
            CSack* pSack = pSacks[sackIdx];
            if (pSack) {
                for (int i = 0; i < 36; ++i) {
                    XiahItem::sItemInfo* pItem = pSack->FindSackItemByPos(i);
                    if (pItem) {
                        BYTE type = pItem->m_bItemType;
                        BOOL bNativelySellable = TRUE;
                        
                        // Exclude non-sellable types: potions, portals, quest items, event items, lottos, sockets
                        if (type == ITEMTYPE_POTION || type == ITEMTYPE_QUEST || 
                            type == ITEMTYPE_PORTAL || type == ITEMTYPE_EVENT || 
                            type == ITEMTYPE_LOTTO || type == ITEMTYPE_SOCKET) {
                            bNativelySellable = FALSE;
                        }
                        
                        // Weapons/armors/accessories shouldn't be upgraded (m_bModifyCnt < 1)
                        if (type >= ITEMTYPE_WEAPON && type <= ITEMTYPE_NECKLACE) {
                            if (pItem->m_bModifyCnt >= 1) {
                                bNativelySellable = FALSE;
                            }
                        }

                        if (bNativelySellable) {
                            BOOL bSellThis = FALSE;
                            if (g_bAutoSellAll) {
                                if (g_bBanSellFilter) {
                                    if (!IsBanSellFiltered((LPCTSTR)pItem->m_szName)) {
                                        bSellThis = TRUE;
                                    }
                                } else {
                                    bSellThis = TRUE;
                                }
                            }

                            if (bSellThis) {
                                SendCS_EC_SELLITEM_REQ(984, pItem->m_dwItemID, pItem->m_bSackID, pItem->m_bSackPos);
                                s_dwLastSellTime = g_dwCurTime;
                                bFoundSellable = TRUE;
                                break;
                            }
                        }
                    }
                }
            }
            if (bFoundSellable) {
                break;
            }
        }

        if (!bFoundSellable) {
            s_bSellingProcessActive = FALSE;
            s_dwLastScanTime = g_dwCurTime; // Start cooldown
        }
    }
}

DWORD GetSkillCD(DWORD dwMugongID) {
    if (dwMugongID == g_dwAttackSkillID && g_dwAttackSkillID > 0) {
        return (DWORD)g_nAttackSkillInterval * 1000;
    }
    if (dwMugongID == g_dwBuffSkillID1 && g_dwBuffSkillID1 > 0) {
        return (DWORD)g_nBuffSkillInterval1 * 1000;
    }
    if (dwMugongID == g_dwBuffSkillID2 && g_dwBuffSkillID2 > 0) {
        return (DWORD)g_nBuffSkillInterval2 * 1000;
    }
    if (dwMugongID == g_dwTeammateSkillID1 && g_dwTeammateSkillID1 > 0) {
        return (DWORD)g_nTeammateSkillInterval1 * 1000;
    }
    if (dwMugongID == g_dwTeammateSkillID2 && g_dwTeammateSkillID2 > 0) {
        return (DWORD)g_nTeammateSkillInterval2 * 1000;
    }
    
    // Default fallback values for manual casting of specific skills:
    if (dwMugongID == 32 || dwMugongID == 36 || dwMugongID == 34 || dwMugongID == 180 || dwMugongID == 181) {
        return 1500; // Attack skills
    }
    if (dwMugongID == 39 || dwMugongID == 30 || dwMugongID == 41 || dwMugongID == 69 || dwMugongID == 68 || dwMugongID == 124 || dwMugongID == 129) {
        return 3000; // Buff/utility skills
    }
    
    return 1500; // Default fallback
}
