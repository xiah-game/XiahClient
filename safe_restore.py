import os

def safe_restore(filepath):
    # Read the original UTF-8 file
    with open(filepath + '.bak2', 'r', encoding='utf-8', errors='replace') as f:
        content = f.read()
        
    # ONLY replace the proven safe strings
    content = content.replace('_T("굴림")', '_T("宋体")')
    content = content.replace('_T("%s 획득")', '_T("获得物品 [ %s ]")')
    content = content.replace('_T("금전 획득 [ %d전 ]")', '_T("获得金币 [ %d两 ]")')
    content = content.replace('_T("경험치 획득 [ %d ]")', '_T("获得经验值 [ %d ]")')
    content = content.replace('_T("경험치 획득 [ %d + %d ]")', '_T("获得经验值 [ %d + %d ]")')
    content = content.replace('#define JUN_MONEY\t\t\t\t\t\t\t_T("전")', '#define JUN_MONEY\t\t\t\t\t\t\t_T("两")')

    # Save it as UTF-8 with BOM so Chinese VS compiles it perfectly
    with open(filepath, 'w', encoding='utf-8-sig') as f:
        f.write(content)

safe_restore(r'D:\xiahold\XiahClient\StringDefine.h')
safe_restore(r'D:\xiahold\XiahClient\StringDefine_utf8.h')
