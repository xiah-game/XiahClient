import os

def apply_bom_and_translations(filepath):
    # Read pure UTF-8 source
    with open(filepath + '.bak2', 'r', encoding='utf-8', errors='replace') as f:
        text = f.read()

    # Add pragma for safety
    text = '#pragma execution_character_set("utf-8")\n' + text

    # Translations
    translations = {
        '굴림': '宋体',
        '%s 획득': '获得物品 [ %s ]',
        '금전 획득 [ %d전 ]': '获得金币 [ %d两 ]',
        '경험치 획득 [ %d ]': '获得经验值 [ %d ]',
        '경험치 획득 [ %d + %d ]': '获得经验值 [ %d + %d ]',
        '상태(C)': '状态(C)',
        '무공(K)': '武功(K)',
        '관계(R)': '关系(R)',
        '기연(Q)': '奇缘(Q)',
        '미니맵(M)': '小地图(M)',
        '도움말(F1)': '帮助(F1)',
        '옵션(O)': '设置(O)',
        '종료(X)': '退出(X)'
    }
    
    for kr, cn in translations.items():
        text = text.replace('_T("' + kr + '")', '_T("' + cn + '")')
        
    text = text.replace('#define JUN_MONEY\t\t\t\t\t\t\t_T("전")', '#define JUN_MONEY\t\t\t\t\t\t\t_T("两")')

    # Explicitly write BOM
    with open(filepath, 'wb') as f:
        f.write(b'\xef\xbb\xbf')
        f.write(text.encode('utf-8'))

apply_bom_and_translations(r'D:\xiahold\XiahClient\StringDefine.h')
apply_bom_and_translations(r'D:\xiahold\XiahClient\StringDefine_utf8.h')
print("Successfully saved with explicit BOM.")
