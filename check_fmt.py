import re

with open(r'D:\xiahold\XiahClient\StringDefine.h.bak2', 'r', encoding='utf-8', errors='replace') as f:
    lines = f.readlines()

for i, line in enumerate(lines):
    if '_T("' in line:
        try:
            macro = line.split('_T("')[0].strip().split()[-1]
            string = line.split('_T("')[1].split('")')[0]
            # Find all format specifiers (e.g. %d, %s, %ld)
            specifiers = re.findall(r'%[0-9]*[a-zA-Z]', string)
            if specifiers:
                print(f'{i+1}: {macro} -> {specifiers} | {string}')
        except:
            pass
