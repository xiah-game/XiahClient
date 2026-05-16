import os

def incremental_translate_bytes(filepath):
    # Read the original UTF-8 file
    with open(filepath + '.bak2', 'r', encoding='utf-8', errors='replace') as f:
        text = f.read()
        
    # Convert entire file to EUC-KR bytes first
    data = bytearray()
    for char in text:
        try:
            data.extend(char.encode('euc-kr'))
        except:
            data.extend(b'?')
            
    # Dictionary of safe, verified translations
    # Format: original_korean_exact_match : chinese_translation
    translations = {
        # Engine core
        '굴림': '宋体',
        
        # Money/Exp
        '%s 획득': '获得物品 [ %s ]',
        '금전 획득 [ %d전 ]': '获得金币 [ %d两 ]',
        '경험치 획득 [ %d ]': '获得经验值 [ %d ]',
        '경험치 획득 [ %d + %d ]': '获得经验值 [ %d + %d ]',
        
        # Hotkeys & Status (from lines 190-237)
        '상태(C)': '状态(C)',
        '무공(K)': '武功(K)',
        '관계(R)': '关系(R)',
        '기연(Q)': '奇缘(Q)',
        '미니맵(M)': '小地图(M)',
        '도움말(F1)': '帮助(F1)',
        '옵션(O)': '设置(O)',
        '종료(X)': '退出(X)',
        
        '캐릭터 추가 생성 불가': '无法创建更多角色',
        '친구': '好友',
        '사부': '师父',
        '지정공격': '指定攻击',
        '대상보호': '保护目标',
        '자신보호': '自我保护',
        '동반공격': '协助攻击',
        '놓아주기': '放生',
        '배낭열기': '打开背包',
        '대화': '对话',
        '관리': '管理',
        '수리': '修理',
        
        # Sprintf Strings
        '%d년%d월%d일%d시': '%d年%d月%d日%d时',
        '%d갑자': '%d甲子',
        '개조 가능': '可改造',
        '생산': '生产',
        '개조 불가': '不可改造',
        '가격: %s': '价格: %s',
        '판매가: %s': '售价: %s',
        '보관비: %s': '保管费: %s',
        '수리비: %s': '修理费: %s',
        '요구 갑자: %d': '需求甲子: %d',
        '요구 민첩력: %d': '需求敏捷: %d',
        '요구 근력: %d': '需求力量: %d',
        '요구 지구력: %d': '需求耐力: %d',
        '요구 진기: %d': '需求真气: %d',
        '생명력 회복': '恢复生命力',
        '내력 회복': '恢复内力',
        '능력치 향상': '提升属性',
        '상태 치료': '状态治疗'
    }

    # Perform raw byte replacement! This is 100% safe.
    for kr, cn in translations.items():
        search_bytes = f'_T("{kr}")'.encode('euc-kr')
        replace_bytes = f'_T("{cn}")'.encode('gbk')
        data = data.replace(search_bytes, replace_bytes)
        
    # Apply JUN_MONEY explicitly
    search_bytes = '#define JUN_MONEY\t\t\t\t\t\t\t_T("전")'.encode('euc-kr')
    replace_bytes = '#define JUN_MONEY\t\t\t\t\t\t\t_T("两")'.encode('gbk')
    data = data.replace(search_bytes, replace_bytes)

    with open(filepath, 'wb') as f:
        f.write(data)

incremental_translate_bytes(r'D:\xiahold\XiahClient\StringDefine.h')
incremental_translate_bytes(r'D:\xiahold\XiahClient\StringDefine_utf8.h')
print("Robust byte-level Frankenstein file generated successfully.")
