// 이미지 포함 스크린 메시지
// 계획 실패로 현재 이벤트 아이템 출력 처리로 사용
#pragma once

using namespace std;

#define MAX_IMGMSG_SIZE 20

struct sImageScrMsg
{
	sString str;
	BYTE bColorType;
	DWORD dwEventCharID;
	int nResID;
	int nToolTipCount;
	bool bShow;

	map<int, sString> mStrToolTip;

	sImageScrMsg() : bColorType(0), nResID(0), nToolTipCount(0), bShow(false), dwEventCharID(0)
	{
		str = _T("");
	}

	~sImageScrMsg()
	{
		mStrToolTip.clear();
	}
};

/**
 * \ingroup XiahClient
 *
 * \date 2004-07-15
 */
class CImageScrMsgList : public std::map<int, sImageScrMsg*>
{
public:
	~CImageScrMsgList()
	{
		Release();
	}

	void Release()
	{
		register iterator it;

		for(it = begin(); it != end(); ++it)
		{
			sImageScrMsg* pMsg = (*it).second;
			delete pMsg;
		}

		clear();
	}

	void AddScrMsg(const int nID, sString &str, BYTE colorType=0)
	{
		iterator iter = find(nID);

		if(iter == end())
		{
			sImageScrMsg* pMsg = new sImageScrMsg;

			pMsg->str = str;
			pMsg->bColorType = colorType;

			//if(size() >= MAX_IMGMSG_SIZE)
			//	DeleteFront();

			insert(map<int, sImageScrMsg*>::value_type(nID, pMsg));
		}
		else
		{
			sImageScrMsg* pMsg = (*iter).second;

			pMsg->str = str;
			pMsg->bColorType = colorType;            
		}	
	}

	void AddScrImg(const int nID, const int nResID, const DWORD dwEventCharID)
	{
		iterator iter = find(nID);

		if(iter == end())
		{
			sImageScrMsg* pMsg = new sImageScrMsg;
			pMsg->nResID = nResID;
			pMsg->dwEventCharID = dwEventCharID;
			insert(map<int, sImageScrMsg*>::value_type(nID, pMsg));
		}
		else
		{
			sImageScrMsg* pMsg = (*iter).second;
			pMsg->nResID = nResID;      
			pMsg->dwEventCharID = dwEventCharID;
		}	
	}

	void DelScrMsg(const int nID)
	{
		iterator iter = find(nID);

		sImageScrMsg* pMsg = (*iter).second;

		delete pMsg;

		erase(iter);
	}

	void DeleteFront()
	{
		iterator it = begin();

		if(it != end())
		{
			sImageScrMsg* pDelMsg = (*it).second;
			delete pDelMsg;

			//pop_front();
		}
	}

	void AllDeleteMsg(void)
	{
		register iterator it ;

		for(it=begin(); it != end(); ++it)
		{
			sImageScrMsg* pDelMsg = (*it).second;

			delete pDelMsg;
		}
		clear();
	}

	void AddToolTip(const int nID, sString strData)
	{
		iterator iter = find(nID);

		if(iter == end())
		{
			sImageScrMsg* pMsg = new sImageScrMsg;

			pMsg->mStrToolTip.insert(map<int, sString>::value_type(pMsg->nToolTipCount++, strData));

			insert(map<int, sImageScrMsg*>::value_type(nID, pMsg));			
		}
		else
		{
			sImageScrMsg* pMsg = (*iter).second;

			pMsg->mStrToolTip.insert(map<int, sString>::value_type(pMsg->nToolTipCount++, strData));
		}
	}

	void SetToolTip(const int nID, const int nLine, sString strData)
	{
		iterator iter = find(nID);

		if(iter == end())
			return;

		sImageScrMsg* pMsg = (*iter).second;

		pMsg->mStrToolTip.erase(nLine);

		pMsg->mStrToolTip.insert(map<int, sString>::value_type(nLine, strData));
	}

	void DelToolTip(const int nID)
	{
		iterator iter = find(nID);

		if(iter == end())
		{	
		}
		else
		{
			sImageScrMsg* pMsg = (*iter).second;

			pMsg->mStrToolTip.clear();
		}
	}
};

/**
 * \ingroup XiahClient
 *
 * \date 2004-07-15
 */
class CImageScrMsg
{
public:
	enum Type
	{
		LEFT = 0,
		RIGHT,
	};

protected:

private:
	
	CText2D	m_text2D[MAX_IMGMSG_SIZE];
	sRect m_rtRegion[MAX_IMGMSG_SIZE];
	sRect m_rtImageRegion[MAX_IMGMSG_SIZE];
	
	LPDIRECT3DVERTEXBUFFER9 m_pScrImageVB[MAX_IMGMSG_SIZE];
	map<int, LPDIRECT3DTEXTURE9> m_mTexList;

	//DWORD		m_dwTimeInterval;

	int m_nImageWidth;	
	int m_nImageHeight;

	int m_nPosX, m_nPosY;

	int	m_nPrevToolTipPos;

	void SetVB();
	void DrawToolTip(const int nPos);
	void SetToolTip(const int nPos);

	// temp
	int m_nCount;
public:
	CImageScrMsgList m_ScrMsgList;

	CImageScrMsg(void);
	~CImageScrMsg(void);

	void AddTexture(const int nResID);

	void SetScrMsg(const int nID, sString sContent, BYTE byColorType=0);
	void SetScrImg(const int nID, const int nResID, const DWORD dwEventCharID);
	void DelScrMsg(const int nID);

	void Show(const int nID);
	void Hide(const int nID);

	void AllHide();

	void AddToolTip(const int nID, sString strData);
	void SetToolTip(const int nID, const int nLine, sString strData);
	void DelToolTip(const int nID);

	void UpdateScrMsg();
	void UpdateTex();

	void ScrMsgShow();

	void DeleteScrMsgByTime();
	void AllDeleteScrMsg();

	//HT_0122 : 기간제 프리미엄 아이템 추가
	Type m_eType;

	inline void SetType(Type eType);
	inline void SetPos(const int nX, const int nY);
};


/**
*
* \param eType 
*/
inline void CImageScrMsg::SetType(Type eType)
{
	m_eType = eType;
}

/**
*
* \param nX 
* \param nY 
*/
inline void CImageScrMsg::SetPos(const int nX, const int nY)
{
	m_nPosX = nX;
	m_nPosY = nY;

	SetVB();
}
