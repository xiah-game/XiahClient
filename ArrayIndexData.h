#pragma once

struct sArrayData
{
	std::vector<int>		m_IntList;
	std::vector<sString>	m_StringList;

	int GetInt(int nIndex)
	{
		if (nIndex < 0 || nIndex >= m_IntList.size())
			return 0;

		return m_IntList[ nIndex];
	}
	
	sString GetString(int nIndex)
	{
		if (nIndex < 0 || nIndex >= m_StringList.size())
			return sString(_T(""));

		try {
			if (m_StringList[nIndex].size() > 8192) {
				return sString(_T("")); // Return empty string if corrupted
			}
			return m_StringList[nIndex];
		} catch (const std::exception&) {
			return sString(_T(""));
		}
	}
};

/**
 * \ingroup XiahClient
 *
 * \date 2004-07-15
 */
class CArrayIndexData : public std::vector<sArrayData *>
{
public:
	CArrayIndexData();
	~CArrayIndexData();

	BOOL Create(LPCTSTR filename);
	BOOL Release();
	
	// Interger로만 Indexing해준다, 속도가 좀 느림, 매번 검색 방식이라서.
	sArrayData *GetData(int index);
	sArrayData *GetData(int index_1,int index_2);
	sArrayData *GetData(int index_1,int index_2,int index_3);
	sArrayData *GetData(int index_1,int index_2,int index_3,int index_4);

	sArrayData *GetSkipData(int nSkip,int index);

protected:
	int m_nIntCount;
	int m_nStringCount;
};