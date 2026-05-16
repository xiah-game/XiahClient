import os

def convert_to_euckr(filepath):
    # Read pure UTF-8 source
    with open(filepath + '.bak2', 'r', encoding='utf-8', errors='replace') as f:
        text = f.read()

    data = bytearray()
    for char in text:
        try:
            data.extend(char.encode('euc-kr'))
        except:
            data.extend(b'?')

    # Save as raw EUC-KR bytes without BOM
    with open(filepath, 'wb') as f:
        f.write(data)

convert_to_euckr(r'D:\xiahold\XiahClient\StringDefine.h')
convert_to_euckr(r'D:\xiahold\XiahClient\StringDefine_utf8.h')
print("Successfully saved as EUC-KR.")
