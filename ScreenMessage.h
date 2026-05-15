#pragma once

#include <deque>

using namespace std;

struct sScrMsg
{
	BYTE bType;
	sString str;
	BYTE bColorType;

	sScrMsg() 
	{
		bType = 0;
		str = _T("");
		bColorType = 0;
	}
};

#define MAXMSGSIZE		10

//typedef deque<sString> ScrMsgList;
class CScrMsgList : public std::list<sScrMsg*>
{
public:
	~CScrMsgList(){ Release();}

	void Release()
	{
		iterator it;

		for(it = begin(); it != end(); it++)
		{
			sScrMsg* pMsg = *it;

			delete pMsg;
		}

		clear();
	}

	void AddScrMsg(BYTE type,sString &str, BYTE colorType=0)
	{
		sScrMsg* pMsg = new sScrMsg;

		pMsg->bType = type;
		pMsg->str = str;
		pMsg->bColorType = colorType;

		iterator it;

		// �
		if( size() >= MAXMSGSIZE)
		{
			DeleteFront();
		}
	
		push_back( pMsg);
	}

	void DeleteFront()
	{
		iterator it = begin();
		if( it != end())
		{
			sScrMsg* pDelMsg = *it;

			delete pDelMsg;

			pop_front();
		}
	}

	void AllDeleteMsg(void)
	{
		iterator it;

		for(it=begin(); it != end(); it++)
		{
			sScrMsg* pDelMsg = *it;

			delete pDelMsg;
		}

		clear();
	}
};

class CScreenMessage
{
private:
	CScrMsgList	m_listMsg;
	BYTE		m_byType;
	sRect		m_rtRegion[ MAXMSGSIZE];
	CText2D		m_text2D[ MAXMSGSIZE];

	DWORD		m_dwTimeInterval;
public:
	// set
	void UpdateScrMsg();
	void SetChatType( BYTE byType);
	void SetScrMsg( BYTE byType, sString sContent, BYTE byColorType=0);
	void SetScrMsgSetRect(BYTE byType, sString sContent, BYTE byColorType=0);
	void UpdateTex(bool bSetRect = false);
	void ScrMsgShow();
	void DeleteScrMsgByTime();
	void AllDeleteScrMsg();

	CScreenMessage( BYTE byType);
	~CScreenMessage(void);

	bool m_bShow;
};
