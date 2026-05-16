with open(r'D:\xiahold\XiahClient\Sack.cpp', 'r', encoding='euc-kr', errors='replace') as f:
    text = f.read()
lines = text.splitlines()
for i, line in enumerate(lines):
    if 'szTip' in line and ('char' in line or 'TCHAR' in line):
        print(str(i+1) + ': ' + ''.join(c for c in line if ord(c) < 128))
