import os

filepath = r'D:\xiahold\XiahClient\Mugong.cpp'

with open(filepath, 'rb') as f:
    content = f.read()

old_logic = b"""		//  Desc
		sString szMugongTip = pData->GetString(nIndex);

		//   
		int nLen = szMugongTip.length();

		TCHAR Desc[128] = {0,};
		memcpy(Desc, szMugongTip, nLen);

		TCHAR buf[64] = {0,};
		int c = 0;
		int nCount = 0;

		for(int i=0; i < nLen; ++i)
		{
			if( Desc[i] == '|')
			{
				if( c > 0)
				{
					c = 0;
					g_pUIManager->AddToolTip(nFrameID, nControlID, nCount, (LPCTSTR)buf, 1);

					++nCount;
					memset(buf, 0, sizeof(buf));
				}
			}
			else
			{
				buf[c++] = (char)Desc[i];
			}
		}

		if( c > 0)
		{
			g_pUIManager->AddToolTip(nFrameID, nControlID, nCount, (LPCTSTR)buf, 1);
		}"""

new_logic = b"""		//  Desc
		sString szMugongTip = pData->GetString(nIndex);

		//   
		int nLen = szMugongTip.length();
		if (nLen >= 1024) nLen = 1023;

		TCHAR Desc[1024] = {0,};
		memcpy(Desc, szMugongTip, nLen);

		TCHAR buf[1024] = {0,};
		int c = 0;
		int nCount = 0;

		for(int i=0; i < nLen; ++i)
		{
			if( Desc[i] == '|' || Desc[i] == '\\n' || Desc[i] == '\\r')
			{
				if( c > 0)
				{
					c = 0;
					g_pUIManager->AddToolTip(nFrameID, nControlID, nCount, (LPCTSTR)buf, 1);

					++nCount;
					memset(buf, 0, sizeof(buf));
				}
			}
			else
			{
			    if (c < 1023)
				    buf[c++] = (char)Desc[i];
			}
		}

		if( c > 0)
		{
			g_pUIManager->AddToolTip(nFrameID, nControlID, nCount, (LPCTSTR)buf, 1);
		}"""

old_logic_crlf = old_logic.replace(b'\n', b'\r\n')
old_logic_lf = old_logic.replace(b'\r\n', b'\n')

if old_logic_crlf in content:
    content = content.replace(old_logic_crlf, new_logic.replace(b'\n', b'\r\n'))
    print("Replaced Mugong.cpp with CRLF")
elif old_logic_lf in content:
    content = content.replace(old_logic_lf, new_logic)
    print("Replaced Mugong.cpp with LF")
elif old_logic in content:
    content = content.replace(old_logic, new_logic)
    print("Replaced Mugong.cpp exact")
else:
    print("Could not find old_logic in Mugong.cpp!")

with open(filepath, 'wb') as f:
    f.write(content)
