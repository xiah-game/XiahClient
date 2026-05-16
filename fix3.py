with open('d:/xiahold/XiahClient/StringDefine.h', 'rb') as f:
    data = f.read()

# Replace _T(/哭泣") with _T("/哭泣")
# The string in bytes (utf-8):
target = b'_T(/\xe5\x93\xad\xe6\xb3\xa3")'
replacement = b'_T("/\xe5\x93\xad\xe6\xb3\xa3")'

if target in data:
    data = data.replace(target, replacement)
    with open('d:/xiahold/XiahClient/StringDefine.h', 'wb') as f:
        f.write(data)
    print("Fixed target successfully.")
else:
    print("Target not found.")
