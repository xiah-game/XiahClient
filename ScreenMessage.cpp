#include "precompile.h"
#include "ScreenMessage.h"
#include "XiahObject.h"
#include "InterfaceDefine.h"
#include "XiahGame_Handler_Sender.h"
#include "CharacterInfo.h"

#include <assert.h>

CScreenMessage::CScreenMessage( BYTE byType) : m_bShow(false)
{
	m_byType = byType;	// 0:notice, 1:help
	m_dwTimeInterval = timeGetTime();
}

CScreenMessage::~CScreenMessage(void)
{
}

void CScreenMessage::UpdateScrMsg()
{	
	DWORD dwCurrTick = timeGetTime();

	BYTE bySize = m_listMsg.size();
	if( bySize < 1)	
	{
		m_dwTimeInterval = dwCurrTick;
		m_bShow = false; // temp
		return;
	}		

	m_bShow = true;	// temp

	DWORD dwInterval = dwCurrTick - m_dwTimeInterval;

	if( dwInterval > 60000)	// 30 sec
	{
		m_dwTimeInterval = dwCurrTick;
		DeleteScrMsgByTime();
	}
}

void CScreenMessage::SetScrMsg(BYTE type, sString content, BYTE byColorType)
{
	switch(m_byType)
	{
	case 0:
		{
			if( m_listMsg.size() >= 10)
				m_listMsg.DeleteFront();

			m_listMsg.AddScrMsg( type, content, byColorType);
			UpdateTex();
		}
		break;
	case 1:
		{
			if( m_listMsg.size() >= 7)
				m_listMsg.DeleteFront();

			m_listMsg.AddScrMsg( type, content, byColorType);
			UpdateTex();
		}
		break;
	case 2:
		{
			if( m_listMsg.size() >= 3)
				m_listMsg.DeleteFront();

			m_listMsg.AddScrMsg( type, content, byColorType);
			UpdateTex();
		}
		break;
	default:
		assert(0);
		break;
	} // switch(m_byType)
}

void CScreenMessage::SetScrMsgSetRect(BYTE type, sString content, BYTE byColorType)
{
	if( m_listMsg.size() >= 10)
		m_listMsg.DeleteFront();

	m_listMsg.AddScrMsg( type, content, byColorType);
	UpdateTex(true);
}

void CScreenMessage::DeleteScrMsgByTime()
{
	BYTE bySize = m_listMsg.size();
	if( bySize > 0)	
		m_listMsg.DeleteFront();

	if(m_byType == 0 && g_MainCharInfo.m_bEvSocketItemUse)
		UpdateTex(true);
	else
		UpdateTex();
}

/**
 * AllDeleteScrMsg
 * \param void 
 */
void CScreenMessage::AllDeleteScrMsg(void)
{
	BYTE bySize = m_listMsg.size();
	if( bySize > 0)	
		m_listMsg.AllDeleteMsg();

	UpdateTex();
}

D3DCOLOR g_ScrMsgColor[8] =
{
	D3DCOLOR_XRGB( 255, 255, 255),
	D3DCOLOR_XRGB( 255, 255, 0),
	D3DCOLOR_XRGB( 255, 0, 0),
	//D3DCOLOR_XRGB( 255, 255, 0),
	//D3DCOLOR_XRGB( 255, 255, 0),
	
	D3DCOLOR_XRGB( 255, 255, 200),
	D3DCOLOR_XRGB( 255, 255, 120),
	D3DCOLOR_XRGB( 255, 255, 0),
	D3DCOLOR_XRGB( 200, 240, 120),
	D3DCOLOR_XRGB( 255, 175, 96),
};

void CScreenMessage::UpdateTex(bool bSetRect)
{
	RECT rtRect;
	if(bSetRect)
	{
		rtRect.left		= 10;
		rtRect.right	= 200;
		rtRect.top		= 550;
		rtRect.bottom	= 570;
	}
	else
	{
		switch(m_byType)
		{
		case 0:
			{
				rtRect.left		= 10;
				rtRect.right	= 200;
				rtRect.top		= 250;
				rtRect.bottom	= 270;
			}
			break;
		case 1:
			{
				rtRect.left		= 10;
				rtRect.right	= 200;
				rtRect.top		= 550;
				rtRect.bottom	= 570;
			}
			break;
		case 2:
			{
				rtRect.left		= 220;
				rtRect.right	= 600;
				rtRect.top		= 70; //HT_0403 : 지속형 무공 시전 아이콘
				rtRect.bottom	= 190;
			}
			break;
		}
	}

	CScrMsgList::iterator it = m_listMsg.begin();
	
	for(int i = 0; it != m_listMsg.end(); ++it, ++i)
	{
		sScrMsg* pMsg = *it;
		if(pMsg == NULL)
		{
			DBG_LogFile( _T("UpdateTex() fail"));
			break;
		}

		m_rtRegion[i].left	= rtRect.left;
		m_rtRegion[i].right = rtRect.right;
		m_rtRegion[i].top	= rtRect.top + VERTICAL_DISTANCE * i;
		m_rtRegion[i].bottom = rtRect.bottom + VERTICAL_DISTANCE * i;

		m_text2D[i].SetParentRect( &m_rtRegion[i]);
		
		assert(!(pMsg->bColorType >= 8));

		D3DCOLOR color = g_ScrMsgColor[ pMsg->bColorType];
		
		m_text2D[i].SetText(0, 0, (LPCTSTR)pMsg->str, GetFont( IDS_DUDUM, 12), color);
	}

	ScrMsgShow();
}

void CScreenMessage::ScrMsgShow()
{
	BYTE bySize = m_listMsg.size();

	for(int i=0; i < bySize; ++i)
	{
		m_text2D[i].Render();
	}	
}