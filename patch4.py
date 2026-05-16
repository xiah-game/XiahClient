with open(r'D:\xiahold\XiahClient\XiahObject.cpp', 'rb') as f:
    text = f.read()

old_block = b'\tCXiahObject* pObject = it->second;\r\n\r\n\tif(pObject == NULL)\r\n\t{\r\n\t\tDBG_LogFile( _T("CXiahObjectManager::ChangeObjectID \xbd\xc7\xc6\xd0"));\r\n\r\n\t\t//return false;\r\n\t}'
new_block = b'\tif(it == end()) return FALSE;\r\n\tCXiahObject* pObject = it->second;\r\n\r\n\tif(pObject == NULL)\r\n\t{\r\n\t\tDBG_LogFile( _T("CXiahObjectManager::ChangeObjectID \xbd\xc7\xc6\xd0"));\r\n\t\treturn FALSE;\r\n\t}'

if old_block in text:
    text = text.replace(old_block, new_block)
    print('Fixed ChangeObjectID')
    with open(r'D:\xiahold\XiahClient\XiahObject.cpp', 'wb') as f:
        f.write(text)
