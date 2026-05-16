with open(r'D:\xiahold\XiahClient\Sack.cpp', 'r', encoding='euc-kr', errors='replace') as f:
    text = f.read()
for i, line in enumerate(text.splitlines()):
    if 'Desc' in line or 'desc' in line:
        print(str(i+1) + ': ' + ''.join(c for c in line.strip() if ord(c) < 128))
