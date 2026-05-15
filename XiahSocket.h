#pragma once

#include "NetMsg.h"

namespace XiahNetwork
{

/*
	Xiah의 Client용 Socket Class

	c socket 스타일로써

	none-block socket이다
*/

	// Message Receive Handler Function
	typedef BOOL (*XIAH_SOCKET_RECEIVE_FUNCTION)(CMsg &msg);
	// 연결이 갑자기 종료 되었을때 호출되는 함수
	typedef void (*XIAH_SOCKET_ON_UNEXPECTEDLY_DISCONNECTED)(void);
	typedef void (*XIAH_SCOKET_ON_CONNECTED)(void);

	// Client Socket초기화 (WSASocketStartup같은걸 호출해준다)
	BOOL InitializeXiahClientSocket();
	// Client Socket해제
	BOOL UnitializeXiahClientSocket();
	// Network Message를 서버에 보낸다
	BOOL SendNetMsg(CMsg &msg);
	// 서버에 접속한다
	BOOL ConnectToServer(LPCTSTR server_address,int nPort);
	// 서버와의 연결을 끊는다
	BOOL DisconnectFromServer();
	// Network Message처리를 한다. (select해서 메세지를 받으면 처리한다)
	BOOL ProcessNetworkMessage();

	extern XIAH_SCOKET_ON_CONNECTED					 g_XiahSocketOnConnected;
	extern XIAH_SOCKET_RECEIVE_FUNCTION				 g_XiahSocketReceivedFunction;
	extern XIAH_SOCKET_ON_UNEXPECTEDLY_DISCONNECTED  g_XiahSocketOnUnexpectedlyDisconnected;

}