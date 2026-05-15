#include "precompile.h"
#include "AppData.h"
#include "XiahSocket.h"
#include "XiahNetworkHandler.h"
#include "InterfaceDefine.h"

using namespace XiahNetwork;



//---------------------------------------------------------------------------------------
// Network Message ID별로 연결되는 함수들을 붙여놓은 hash_map 클래스

namespace XiahNetwork
{

class CMessageMap : public std::hash_map<WORD,XIAH_NETWORK_RECEIVE_FUNCTION>
{
public:
	inline XIAH_NETWORK_RECEIVE_FUNCTION GetHandler(WORD id)
	{
		iterator it = find( id);

		if( it == end())
			return NULL;

		return it->second;
	}
};



CMessageMap	g_MessageMap;
//----------------------------------------------------------------------------------------------------------------------
// Handler 등록
BOOL RegisterHandler(WORD nMessageID,XIAH_NETWORK_RECEIVE_FUNCTION function)
{
	DBG_Assert( g_MessageMap.GetHandler( nMessageID) == NULL);

	g_MessageMap.insert( CMessageMap::value_type( nMessageID, function));

	return TRUE;
}

//----------------------------------------------------------------------------------------------------------------------
// 네트웍으로 부터 받은 메세지를 각각의 Handler로 전달
BOOL DispatchMessage(CMsg &msg)
{
	CMessageMap::iterator it = g_MessageMap.find( msg.ID());

	if( it == g_MessageMap.end())
	{
		DBG_Put( _T("알수 없는 네트웍 메세지 : 0x%X"), msg.ID());
		return  FALSE;
	}
	
//	DBG_Assert( it != g_MessageMap.end());

	// 왜 같은 검사 루틴이 2개 있을까...
	/*
	if( it == g_MessageMap.end())
		return FALSE;
	*/

	XIAH_NETWORK_RECEIVE_FUNCTION Handler = it->second;

	DBG_Assert( Handler);

	Handler(msg);

	return TRUE;
}



//----------------------------------------------------------------------------------------------------------------------
// 예기치 못한 상황에서 연결이 해제 되었다
void Disconnected()
{	
	if( g_pUIManager)
	{
		g_pUIManager->ShowNotice( IDS_DISCONNECT_SERVER, NOTICE_FRAME_OK, NOTICE_FRAME_UNEXPECTED_TERMINATE);
	}
	else
	{
		if(g_pUIManager)
			g_pUIManager->ShowNotice( IDS_DISCONNECT_SERVER, NOTICE_FRAME_OK, NOTICE_FRAME_UNEXPECTED_TERMINATE);
		else
			MessageBox( GetForegroundWindow(), IDS_DISCONNECT_SERVER, _T("Error"), MB_OK );

		PostMessage( g_AppData.m_hWnd, WM_CLOSE, 0, 0);
	}
}

//---------------------------------------------------------------------------------------
// 네트웍 핸들러 초기화
BOOL InitializeNetworkHandler()
{
	g_XiahSocketReceivedFunction			= DispatchMessage;
	g_XiahSocketOnUnexpectedlyDisconnected	= Disconnected;	

	if( !InitializeXiahClientSocket())
		return FALSE;

	return TRUE;
}

//----------------------------------------------------------------------------------------------------------------------
// 네트웍 핸들러 해제
BOOL UninitializeNetworkHandler()
{
	if( !UnitializeXiahClientSocket())
		return FALSE;

	return TRUE;
}

};

//---------------------------------------------------------------------------------------

/*********************************************************************************************************************
......................................................................................................................
.........................SSSS...EEEEEE..PPPPP...EEEEEE..RRRRR.....AA....TTTTTT...OOOO...RRRRR.........................
........................SS..SS..EE......PP..PP..EE......RR..RR...AAAA.....TT....OO..OO..RR..RR........................
........................SS......EE......PP..PP..EE......RR..RR..AA..AA....TT....OO..OO..RR..RR........................
.........................SSSS...EEEEEE..PPPPP...EEEEEE..RRRR....AAAAAA....TT....OO..OO..RRRR..........................
............................SS..EE......PP......EE......RR.RR...AA..AA....TT....OO..OO..RR.RR.........................
........................SS..SS..EE......PP......EE......RR..RR..AA..AA....TT....OO..OO..RR..RR........................
.........................SSSS...EEEEEE..PP......EEEEEE..RR..RR..AA..AA....TT.....OOOO...RR..RR........................
......................................................................................................................
*********************************************************************************************************************/
