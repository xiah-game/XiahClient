with open(r'D:\xiahold\XiahClient\XiahObject.cpp', 'rb') as f:
    text = f.read()

# Fix 1: ReleaseXiahObject
old1 = b'iterator it = find( pObject->m_ddwObjectID);\r\n\r\n\tif(false)\r\n\t\tDBG_LogFile( _T("ReleaseXiahObject \xbd\xc7\xc6\xd0"));\r\n\r\n\r\n\terase( it);'
new1 = b'iterator it = find( pObject->m_ddwObjectID);\r\n\r\n\tif(it == end()) return FALSE;\r\n\r\n\terase( it);'
if old1 in text:
    text = text.replace(old1, new1)
    print('Fixed ReleaseXiahObject')

# Fix 2: ChangeToClientObject
old2 = b'DBG_Assert( it != end());\r\n\r\n\terase( it);\t// \xc0\xcf\xb4\xdc \xc1\xf6\xbf\xf6\xc1\xd6\xb0\xed'
new2 = b'DBG_Assert( it != end());\r\n\tif(it != end()) erase( it);\t// \xc0\xcf\xb4\xdc \xc1\xf6\xbf\xf6\xc1\\xd6\xb0\xed'
if old2 in text:
    text = text.replace(old2, new2)
    print('Fixed ChangeToClientObject')

# Fix 3: ChangeObjectID
old3 = b'if( it == end() )\r\n\t{\r\n\t\t//DBG_LogFile( _T("ChangeObjectID error ID - %X %X"), pObject->m_dwClientID, pObject->m_dwServerID);\r\n\t\t//return false;\r\n\t}\r\n\t\r\n\t// \xc0\xcf\xb4\xdc \xc1\xf6\xbf\xec\xb0\xed\r\n\terase( it);'
new3 = b'if( it == end() )\r\n\t{\r\n\t\t//DBG_LogFile( _T("ChangeObjectID error ID - %X %X"), pObject->m_dwClientID, pObject->m_dwServerID);\r\n\t\treturn false;\r\n\t}\r\n\t\r\n\t// \xc0\xcf\xb4\xdc \xc1\xf6\xbf\xec\xb0\xed\r\n\terase( it);'
if old3 in text:
    text = text.replace(old3, new3)
    print('Fixed ChangeObjectID')

with open(r'D:\xiahold\XiahClient\XiahObject.cpp', 'wb') as f:
    f.write(text)
