#pragma once

#include <map>


class CMunpaMark
{
public:
	struct sMark
	{
		int					nDemandCount;	// 요청수
		bool				bDemand;		// 서버에 요청 여부
		LPDIRECT3DTEXTURE9	pTexture;		// 마크 텍스쳐

		sMark() : nDemandCount(0), bDemand(false), pTexture(NULL)
		{
		}
	};

	typedef std::map<DWORD, sMark*>		MarkMap;

public:

	CMunpaMark();
	~CMunpaMark();

	void Init();
	void RenderMark(DWORD dwMarkID, int nX, int nY);

	bool SaveMarkFile(DWORD dwMarkID, LPCTSTR strData);

protected:

	void Release();

	bool IsMarkFile(DWORD dwMarkID);
	int LoadMarkFile(DWORD dwMarkID);

	void Render(DWORD dwMarkID, int nX, int nY);

protected:

	MarkMap	m_mMark;
	LPDIRECT3DVERTEXBUFFER9	m_pVB;

};


extern CMunpaMark g_MunpaMark;