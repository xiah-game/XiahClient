#pragma once

#include "NetMsg.h"

/*
	namespace XiahNetwork

	Network관련한 처리는 모두 XiahNetwork이라는 namespace에 포함되어 정의되고 선언된다
*/



namespace XiahNetwork
{
	typedef int (*XIAH_NETWORK_RECEIVE_FUNCTION)(CMsg &msg);

	BOOL InitializeNetworkHandler();		// Network Handler초기화
	BOOL UninitializeNetworkHandler();	// Network Handler해제

	// Handler 등록
	BOOL RegisterHandler(WORD nMessageID,XIAH_NETWORK_RECEIVE_FUNCTION function);
};
