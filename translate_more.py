import os

def translate_more_equip(filepath):
    # Read the file
    with open(filepath, 'r', encoding='utf-8-sig', errors='replace') as f:
        text = f.read()

    translations = {
        '내구력: %d / %d': '耐久度: %d / %d',
        '공격력: %d': '攻击力: %d',
        '방어력: %d': '防御力: %d',
        '정확도: %d': '准确度: %d',
        '요구 갑자: %d': '要求甲子: %d',
        '요구 민첩력: %d': '要求敏捷力: %d',
        '요구 근력: %d': '要求力量: %d',
        '요구 지구력: %d': '要求耐力: %d',
        '요구 진기: %d': '要求真气: %d',
        '추가 속성 공격력  : %d': '追加属性攻击力  : %d',
        '이동 속도 증가: %d': '移动速度增加: %d',
        '다음 레벨 이동 속도 증가: %d': '下级移动速度增加: %d',
        '공격 속도 증가: %d': '攻击速度增加: %d',
        '이동 속도 감소: %d': '移动速度减少: %d'
    }
    
    for kr, cn in translations.items():
        text = text.replace(f'_T("{kr}")', f'_T("{cn}")')

    with open(filepath, 'w', encoding='utf-8-sig') as f:
        f.write(text)

translate_more_equip(r'D:\xiahold\XiahClient\StringDefine.h')
translate_more_equip(r'D:\xiahold\XiahClient\StringDefine_utf8.h')
print("Translated more equip strings.")
