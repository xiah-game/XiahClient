import re

with open(r'D:\xiahold\XiahClient\StringDefine.h.bak2', 'r', encoding='utf-8') as f:
    text = f.read()

macros = re.findall(r'#define\s+(IDS_[A-Z0-9_]+)\s+_T\(\"(.*?)\"\)', text)

for name, val in macros:
    if '%' in val:
        # Check what happens if we encode as GBK and decode back
        # The C++ compiler transcodes UTF-8 to GBK using replacing on invalid characters.
        val_gbk = val.encode('gbk', errors='replace').decode('gbk', errors='replace')
        
        orig_s = val.count('%s')
        new_s = val_gbk.count('%s')
        orig_d = val.count('%d')
        new_d = val_gbk.count('%d')
        
        if orig_s != new_s or orig_d != new_d:
            print(f'MISMATCH! {name}: {val} -> {val_gbk}')
