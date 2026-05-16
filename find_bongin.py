with open(r'D:\xiahold\XiahClient\Sack.cpp', 'r', encoding='euc-kr', errors='replace') as f:
    lines = f.readlines()

for i, line in enumerate(lines):
    if 'ITEMTYPE_BONGIN' in line:
        for j in range(max(0, i-5), min(len(lines), i+30)):
            clean = ''.join(c for c in lines[j].strip() if ord(c) < 128)
            print(f'{j+1}: {clean}')
        break
