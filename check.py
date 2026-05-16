import re
with open('d:/xiahold/XiahClient/StringDefine.h', 'rb') as f:
    lines = f.readlines()
for i, line in enumerate(lines):
    s = line.decode('utf-8', errors='ignore')
    if '_T(' in s and '_T(\"' not in s:
        print(f'Line {i+1}: {s.strip()}')
