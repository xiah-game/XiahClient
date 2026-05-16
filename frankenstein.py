import os

def byte_replace(filepath):
    with open(filepath + '.bak2', 'rb') as f:
        data = f.read()

    # The file .bak2 is UTF-8 without BOM.
    # We must convert the ENTIRE file to EUC-KR so that Korean strings are compiled exactly as EUC-KR bytes!
    # Because if we leave it as UTF-8, the Chinese compiler will compile UTF-8 bytes, which breaks everything!
    text = data.decode('utf-8', errors='replace')
    
    # Now replace the specific strings we want to be Chinese
    text = text.replace('_T("%s 획득")', '_T("获得物品 [ %s ]")')
    text = text.replace('_T("금전 획득 [ %d전 ]")', '_T("获得金币 [ %d两 ]")')
    text = text.replace('_T("경험치 획득 [ %d ]")', '_T("获得经验值 [ %d ]")')
    text = text.replace('_T("경험치 획득 [ %d + %d ]")', '_T("获得经验值 [ %d + %d ]")')
    text = text.replace('#define JUN_MONEY\t\t\t\t\t\t\t_T("전")', '#define JUN_MONEY\t\t\t\t\t\t\t_T("两")')

    # Now encode the text.
    # We want Korean characters to be encoded as EUC-KR, and Chinese characters to be encoded as GBK.
    # We can do this by manually encoding character by character, or by chunks.
    result_bytes = bytearray()
    for char in text:
        try:
            # If it's ascii, just append
            if ord(char) < 128:
                result_bytes.extend(char.encode('ascii'))
            # If it's Chinese, try GBK
            elif '\u4e00' <= char <= '\u9fff' or char in '两获得物品金币经验值':
                result_bytes.extend(char.encode('gbk'))
            # Otherwise try EUC-KR (for Korean)
            else:
                result_bytes.extend(char.encode('euc-kr'))
        except Exception:
            # Fallback to ? if encoding fails
            result_bytes.extend(b'?')

    with open(filepath, 'wb') as f:
        f.write(result_bytes)

byte_replace(r'D:\xiahold\XiahClient\StringDefine.h')
byte_replace(r'D:\xiahold\XiahClient\StringDefine_utf8.h')
print("Successfully generated Frankenstein ANSI files.")
