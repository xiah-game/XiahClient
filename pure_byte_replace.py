import os

def pure_byte_replace(filepath):
    with open(filepath + '.bak2', 'rb') as f:
        data = bytearray(f.read())
        
    # We know .bak2 is UTF-8 without BOM.
    # So the Korean strings we search for must be UTF-8 encoded!
    
    # 1. 获得物品 [%s]
    kr1 = '_T("%s 획득")'.encode('utf-8')
    cn1 = '_T("获得物品 [ %s ]")'.encode('gbk')
    data = data.replace(kr1, cn1)
    
    # 2. 获得金币 [%d两]
    kr2 = '_T("금전 획득 [ %d전 ]")'.encode('utf-8')
    cn2 = '_T("获得金币 [ %d两 ]")'.encode('gbk')
    data = data.replace(kr2, cn2)
    
    # 3. 获得经验值 [%d]
    kr3 = '_T("경험치 획득 [ %d ]")'.encode('utf-8')
    cn3 = '_T("获得经验值 [ %d ]")'.encode('gbk')
    data = data.replace(kr3, cn3)
    
    # 4. 获得经验值 [%d + %d]
    kr4 = '_T("경험치 획득 [ %d + %d ]")'.encode('utf-8')
    cn4 = '_T("获得经验值 [ %d + %d ]")'.encode('gbk')
    data = data.replace(kr4, cn4)
    
    # 5. JUN_MONEY
    kr5 = '#define JUN_MONEY\t\t\t\t\t\t\t_T("전")'.encode('utf-8')
    cn5 = '#define JUN_MONEY\t\t\t\t\t\t\t_T("两")'.encode('gbk')
    data = data.replace(kr5, cn5)
    
    # 6. GULIM FONT -> 宋体
    kr6 = '_T("굴림")'.encode('utf-8')
    cn6 = '_T("宋体")'.encode('gbk')
    data = data.replace(kr6, cn6)

    # Hotkeys
    replacements = {
        '상태(C)': '状态(C)',
        '무공(K)': '武功(K)',
        '관계(R)': '关系(R)',
        '기연(Q)': '奇缘(Q)',
        '미니맵(M)': '小地图(M)',
        '도움말(F1)': '帮助(F1)',
        '옵션(O)': '设置(O)',
        '종료(X)': '退出(X)'
    }
    
    for kr, cn in replacements.items():
        data = data.replace(f'_T("{kr}")'.encode('utf-8'), f'_T("{cn}")'.encode('gbk'))

    with open(filepath, 'wb') as f:
        f.write(data)

pure_byte_replace(r'D:\xiahold\XiahClient\StringDefine.h')
pure_byte_replace(r'D:\xiahold\XiahClient\StringDefine_utf8.h')
print("Pure byte-level hybrid file generated.")
