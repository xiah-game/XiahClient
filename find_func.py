import re

with open(r'D:\xiahold\XiahClient\Sack.cpp', 'r', encoding='euc-kr', errors='replace') as f:
    text = f.read()

lines = text.splitlines()
func_name = ""

for i, line in enumerate(lines):
    if re.match(r'^[\w\:]+\s*\([^\)]*\)', line):
        func_name = line.strip()
    if 'ITEMTYPE_BONGIN' in line:
        clean = ''.join(c for c in line.strip() if ord(c) < 128)
        print(f'{func_name} | {i+1}: {clean}')
