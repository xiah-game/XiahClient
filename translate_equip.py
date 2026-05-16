import os

def translate_equipment_strings(filepath):
    # Read the utf-8-sig file we already established
    with open(filepath, 'r', encoding='utf-8-sig', errors='replace') as f:
        text = f.read()

    translations = {
        '근접공격 방어 증가 : %d': '近战防御增加 : %d',
        '원거리공격 방어 증가 : %d': '远程防御增加 : %d',
        '전기 내성 증가량 : %d': '雷电抗性增加 : %d',
        '화염 내성 증가량 : %d': '火焰抗性增加 : %d',
        '냉기 내성 증가량 : %d': '冰冻抗性增加 : %d',
        '독 내성 증가량 : %d': '毒素抗性增加 : %d',
        '생명력 회복량: %d': '生命恢复量: %d',
        '다음 레벨 생명력 회복량: %d': '下级生命恢复: %d',
        '최대 생명력 증가: %d': '最大生命增加: %d',
        '내력 회복량: %d': '内力恢复量: %d',
        '다음 레벨 내력 회복: %d': '下级内力恢复: %d',
        '최대 내력 증가: %d': '最大内力增加: %d',
        '자동 생명력 회복: %d': '自动生命恢复: %d',
        '자동 내력 회복: %d': '自动内力恢复: %d',
        '개조 시도 횟수: %d': '改造尝试次数: %d',
        '일격술 확률 증가(%%): %d': '暴击率增加(%%): %d',
        '다음 레벨 일격술 확률 증가(%%): %d': '下级暴击率增加(%%): %d',
        '수량: %d': '数量: %d',
        '봉인된 몬스터 : %d': '封印的怪物 : %d',
        '공격력 증가: %d': '攻击力增加: %d',
        '공격력 증가율(%%): %d': '攻击力增加率(%%): %d',
        '다음 레벨 공격력 증가: %d': '下级攻击力增加: %d',
        '다음 레벨 공격력 증가율(%%): %d': '下级攻击增加率(%%): %d',
        '무기의 공격력 증가: %d': '武器攻击力增加: %d',
        '무기 공격력 증가율(%%): %d': '武器攻击增加率(%%): %d',
        '다음 레벨 무기의 공격력 증가: %d': '下级武器攻击增加: %d',
        '다음 레벨 무기의 공격력 증가(%%): %d': '下级武器攻击增加(%%): %d',
        '애완 몬스터 공격력 증가: %d': '宠物攻击力增加: %d',
        '다음 레벨 애완 몬스터 공격력 증가: %d': '下级宠物攻击增加: %d',
        '방어력 증가: %d': '防御力增加: %d',
        '방어력 증가율(%%): %d': '防御力增加率(%%): %d',
        '다음 레벨 방어력 증가: %d': '下级防御力增加: %d',
        '다음 레벨 방어력 증가율(%%): %d': '下级防御力增加率(%%): %d',
        '생명력 증가: %d': '生命力增加: %d',
        '다음 레벨 생명력 증가: %d': '下级生命力增加: %d',
        '생명력 흡수(%%) : %d': '生命力吸收(%%) : %d',
        '다음 레벨 생명력 흡수(%%) : %d': '下级生命吸收(%%) : %d',
        '정확도 증가: %d': '准确度增加: %d',
        '정확도 증가율(%%): %d': '准确度增加率(%%): %d',
        '다음 레벨 정확도 증가: %d': '下级准确度增加: %d',
        '다음 레벨 정확도 증가율(%%): %d': '下级准确增加率(%%): %d',
        '애완 몬스터 정확도 증가 : %d': '宠物准确度增加 : %d',
        '다음 레벨 애완 몬스터 정확도 증가 : %d': '下级宠物准确增加 : %d',
        '내력 증가 : %d': '内力增加 : %d',
        '다음 레벨 내력 증가: %d': '下级内力增加: %d',
        '지속 시간(초) : %d': '持续时间(秒) : %d',
        '다음 레벨 지속 시간(초) : %d': '下级持续时间(秒) : %d'
    }

    # Backup current state just in case
    with open(filepath + '.bak3', 'w', encoding='utf-8-sig') as f:
        f.write(text)

    # Perform translation
    for kr, cn in translations.items():
        # Replace the specific korean strings
        text = text.replace(f'_T("{kr}")', f'_T("{cn}")')

    with open(filepath, 'w', encoding='utf-8-sig') as f:
        f.write(text)

translate_equipment_strings(r'D:\xiahold\XiahClient\StringDefine.h')
translate_equipment_strings(r'D:\xiahold\XiahClient\StringDefine_utf8.h')
print("Successfully translated equipment strings to Chinese.")
