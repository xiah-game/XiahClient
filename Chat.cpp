#include "precompile.h"
#include "resource.h"
#include "AppData.h"
#include "chat.h"
#include "XiahObject.h"
#include "XiahGameObject.h"
#include "InterfaceDefine.h"
#include "XiahObjectType.h"
#include "CharacterInfo.h"


CChat::CChat(void)
{
	m_byType = SMALLCHAT;
	m_bChatFlag = FALSE;
	m_dwTimeInterval = 0;
}

CChat::~CChat(void)
{
	ChatList::iterator	it;

	if(m_listChat.size() > 0)
	{
		for(it = m_listChat.begin();it != m_listChat.end();it++)
		{
			sChat *temp = NULL;
			temp = (sChat*)(*it);
			delete temp;
			temp = NULL;
		}

		m_listChat.clear();
	}
}

void CChat::UpdateChat()
{
	DWORD dwCurrTick = timeGetTime();

	BYTE bySize = m_listChat.size();
	if( bySize < 1)	
	{
		m_dwTimeInterval = dwCurrTick;
		return;
	}	

	DWORD dwInterval = dwCurrTick - m_dwTimeInterval;
	
	if( dwInterval > 30000)	// 30 sec
	{
		m_dwTimeInterval = dwCurrTick;
		DeleteChatByTime();
	}
}

void CChat::SetChatType( BYTE byType) 
{ 
	m_byType = byType;

	switch( byType)
	{
	case NOCHAT:
		g_pUIManager->Hide(SMALL_MESSENGER);
		g_pUIManager->Hide(LARGE_MESSENGER);
		g_pUIManager->Hide(MAIN_CHAT);

		g_pUIManager->SetReleaseFocus(MAIN_CHAT);
		g_pUIManager->SetReleaseFocus(MAIN_CHAT, main_chat_edit);
		break;
	case SMALLCHAT:
		g_pUIManager->Show(SMALL_MESSENGER);
		g_pUIManager->Hide(LARGE_MESSENGER);
		g_pUIManager->ForwardShow(MAIN_CHAT);

		g_pUIManager->SetFocus(MAIN_CHAT);
		g_pUIManager->SetFocus(MAIN_CHAT, main_chat_edit);
		break;
	case MEDIUMCHAT:
		break;
	case LARGECHAT:
		g_pUIManager->Hide(SMALL_MESSENGER);
		g_pUIManager->Show(LARGE_MESSENGER);
		g_pUIManager->ForwardShow(MAIN_CHAT);

		g_pUIManager->SetFocus(MAIN_CHAT);
		g_pUIManager->SetFocus(MAIN_CHAT, main_chat_edit);
		break;
	}

	UpdateTex();
};

void CChat::SetChatMsg( DWORD dwSender, BYTE type, sString content, sString SenderName, DWORD listner, sString listnerName)
{	
	// 내 귓말 리스트에 없는 넘은 넣어주기(안넣어줘도 되지만, 그럼 귓말 보낼때마다 서버에서 계속 찾겠지? 서버 퍼포먼스 위하여)
	if( type == CT_WHISPER)
	{
		if( !listner)
		{
			g_MainCharInfo.ShowHelpMessage( IDS_NOT_CONNECT_ID,TEXTEFFECT_COLOR_WARNING);
			return;
		}

		g_MainCharInfo.m_pRelation->InsertWhisperInfo( listner, listnerName);
	}

	if( type == CT_MUNPA_BROADCAST)
	{
		if( dwSender == g_MainCharInfo.m_dwObjectID)
			SenderName = g_MainCharInfo.m_szNickName;
		else
		{
			sClanWonInfo* pInfo = g_MainCharInfo.m_pRelation->FindClanInfoByID( dwSender);
			if( pInfo)
			{
				SenderName = pInfo->m_szCharName;
			}
		}
	}

	// 말풍선
	if( type == CT_NORMAL && g_info.m_bHideChat)
	{
		XiahObject::CXiahObject *pXiahObject = XiahObject::g_XiahObjectManager.FindXiahObject( MAKEOBJECTID( 0, dwSender, OBJTYPE_PC));
		if( pXiahObject)
		{
			CXiahCharObject* pCharObject = reinterpret_cast<CXiahCharObject*>(pXiahObject->m_pObject);
			if( pCharObject)
			{
				pCharObject->SetChatBox( g_dwCurTime, content, type);
			}
		}
	}
	
	// listChat에 chat message 넣기
	sString buff;
	switch( type)
	{
	case CT_WHISPER:
		buff.printf( _T("[\xc3\xdc\xd3\xef] %s : %s"), (LPCTSTR)SenderName, (LPCTSTR)content);
		break;
	case CT_DAN:
		buff.printf( _T("[\xd7\xe9\xb6\xd3] %s : %s"), (LPCTSTR)SenderName, (LPCTSTR)content);
		break;
	case CT_MUNPA_BROADCAST:
	case CT_MUNPA_MUNJUSHOUT:
		buff.printf( _T("[\xc3\xc5\xc5\xc9] %s : %s"), (LPCTSTR)SenderName, (LPCTSTR)content);
		break;
	default:
		buff.printf( _T("[\xc6\xd5\xcd\xa8] %s : %s"), (LPCTSTR)SenderName, (LPCTSTR)content);
		break;
	}

	if(m_listChat.size() >= MAXCHATSIZE)
	{
		sChat	*deadChat;
		deadChat = m_listChat[0];
		delete deadChat;

		m_listChat.pop_front();
	}

	sChat* pChat = new sChat;

	pChat->dwSender = dwSender;
	pChat->szContent = buff;
	pChat->byType = type;

	// 마지막 귓속말 상대 닉네임 기억 , 현재 귓속말 상태일때는 무시
	if(type == CT_WHISPER && g_MainCharInfo.m_szNickName != SenderName && g_MainCharInfo.GetCurrSendChatType() != CT_WHISPER)
	{
		g_MainCharInfo.m_strWhisperName = SenderName;
		g_pUIManager->SetString(MAIN_CHAT, chat_name_edit, g_MainCharInfo.m_strWhisperName);
	}

	m_listChat.push_back( pChat);

	UpdateTex();	
}

void CChat::UpdateTex()
{
	BYTE bySize = m_listChat.size();

	// m_text2D update
	BYTE byloopFirst = 0;
	RECT rtRect;

	switch( m_byType)
	{
	case NOCHAT:
		return;
	case SMALLCHAT:
		g_pUIManager->GetRegionData(SMALL_MESSENGER, small_messenger_content, rtRect);

		if( bySize < SMALLCHATSIZE)
			byloopFirst = 0;
		else
			byloopFirst = bySize - SMALLCHATSIZE;
		break;
	case MEDIUMCHAT:
		if( bySize < MEDIUMCHATSIZE)
			byloopFirst = 0;
		else
			byloopFirst = bySize - MEDIUMCHATSIZE;	
		break;
	case LARGECHAT:
		g_pUIManager->GetRegionData(LARGE_MESSENGER, large_messenger_content, rtRect);

		if( bySize < LARGECHATSIZE)
			byloopFirst = 0;
		else
			byloopFirst = bySize - LARGECHATSIZE;
		break;
	}

	BYTE loop = bySize - byloopFirst;

	for( int i=0; i< loop; i++)
	{
		m_rtRegion[i].left = rtRect.left;
		m_rtRegion[i].right = rtRect.right;
		m_rtRegion[i].top = rtRect.top + CHAT_VERTICAL_DISTANCE*i;
		m_rtRegion[i].bottom = rtRect.bottom + CHAT_VERTICAL_DISTANCE*i;

		m_text2D[i].SetParentRect( &m_rtRegion[i]);
		sChat* pChat = m_listChat[ i+byloopFirst];
		D3DCOLOR color;
		switch( pChat->byType)
		{
		case CT_WHISPER:
			color = D3DCOLOR_XRGB( 128, 255, 128);			
			break;
		case CT_DAN:
			color = D3DCOLOR_XRGB( 100, 200, 255);
			break;
		case CT_BATTLE:
			break;
		case CT_MUNPA_BROADCAST:
		case CT_MUNPA_MUNJUSHOUT:
			color = D3DCOLOR_XRGB( 255, 180, 80);
			break;
		default:
			color = D3DCOLOR_XRGB( 255, 255, 255);
			break;
		}
		m_text2D[i].SetText( 0, 0, (LPCTSTR)pChat->szContent, DEFAULT_FONT /*GetFont( IDS_DUDUM, 12)*/, color);
	}	

	ChatShow();
}

void CChat::ChatShow()
{
	if( m_byType == NOCHAT)
		return;

	BYTE bySize = m_listChat.size();
	BYTE byloopFirst = 0;

	switch( m_byType)
	{
	case NOCHAT:
		return;
	case SMALLCHAT:
		if( bySize < SMALLCHATSIZE)
			byloopFirst = 0;
		else
			byloopFirst = bySize - SMALLCHATSIZE;
		break;
	case MEDIUMCHAT:
		if( bySize < MEDIUMCHATSIZE)
			byloopFirst = 0;
		else
			byloopFirst = bySize - MEDIUMCHATSIZE;	
		break;
	case LARGECHAT:
		if( bySize < LARGECHATSIZE)
			byloopFirst = 0;
		else
			byloopFirst = bySize - LARGECHATSIZE;
		break;
	}

	BYTE loop = bySize - byloopFirst;

	for( int i=0; i< loop; i++)
	{
		m_text2D[i].Render();
	}	
}

void CChat::DeleteChatByTime()
{
	BYTE bySize = m_listChat.size();

	if( bySize > 0)	
	{
		sChat	*deadChat;
		deadChat = m_listChat[0];
		delete deadChat;

		m_listChat.pop_front();
	}

	UpdateTex();
}

void CChat::Clear()
{
	register ChatList::iterator it;

	if(m_listChat.size() > 0)
	{
		for(it = m_listChat.begin();it != m_listChat.end(); ++it)
		{
			sChat *temp = NULL;
			temp = (sChat*)(*it);
			delete temp;
			temp = NULL;
		}

		m_listChat.clear();
	}

	UpdateTex();
}