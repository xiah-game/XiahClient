import os

def fix_utf8(filepath):
    # Read the original UTF-8 file
    with open(filepath + '.bak2', 'r', encoding='utf-8', errors='replace') as f:
        content = f.read()
        
    # Replace the target strings
    content = content.replace('_T("%s 획득")', '_T("获得物品 [ %s ]")')
    content = content.replace('_T("금전 획득 [ %d전 ]")', '_T("获得金币 [ %d两 ]")')
    content = content.replace('_T("경험치 획득 [ %d ]")', '_T("获得经验值 [ %d ]")')
    content = content.replace('_T("경험치 획득 [ %d + %d ]")', '_T("获得经验值 [ %d + %d ]")')
    
    # We must be careful not to replace '전' in Korean words unless it's the specific JUN_MONEY macro
    content = content.replace('#define JUN_MONEY\t\t\t\t\t\t\t_T("전")', '#define JUN_MONEY\t\t\t\t\t\t\t_T("两")')

    # Save it back as exactly UTF-8 WITHOUT BOM, just like it originally was!
    with open(filepath, 'w', encoding='utf-8') as f:
        f.write(content)

fix_utf8(r'D:\xiahold\XiahClient\StringDefine.h')
fix_utf8(r'D:\xiahold\XiahClient\StringDefine_utf8.h')
