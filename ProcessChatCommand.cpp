#include "precompile.h"
#include "XiahGame_Main.h"
#include "XiahGameObject.h"
#include "XiahCamera.h"
#include "XIahMap.h"
#include "AppData.h"

#include "XiahGame_Handler_Sender.h"
#include "XiahGame_Pet.h"
#include "XiahEnvInfo.h"
#include "XiahCharAniType.h"
#include "interfacedefine.h"

#define MAX_COMMAND 10

sString g_CommandList[ MAX_COMMAND];
extern GAMESTEP_LIST g_GameStep;;
extern  BOOL g_bCheat;

#ifdef _DEBUG_CHEAT
extern  BOOL g_bCheatEtc;
#endif



BOOL ProcessChatCommand(LPCTSTR pCommand)
{
	const TCHAR delimeter[] = _T(" ");

	int nCommand = 0;

	TCHAR* token = _tcstok( (TCHAR*)pCommand, delimeter);

	while( token != NULL && nCommand < MAX_COMMAND)
	{
		g_CommandList[ nCommand] = token;				
		nCommand ++;
		token = _tcstok( NULL, delimeter);
	}

	if( g_CommandList[ 0] == (char*)CHAT_CHATCOMMAND1 || g_CommandList[ 0] == (char*)CHAT_CHATCOMMAND2)
	{
		SendCS_ACTION_REQ(XiahAniType::eLAT_Casual, 1,0);
	}
	else if( g_CommandList[ 0] == CHAT_CHATCOMMAND3 || g_CommandList[ 0] == CHAT_CHATCOMMAND4)
	{
		SendCS_ACTION_REQ(XiahAniType::eLAT_Casual, 6,0);
	}
	else if( g_CommandList[ 0] == CHAT_CHATCOMMAND5 || g_CommandList[ 0] == CHAT_CHATCOMMAND6 || g_CommandList[ 0] == CHAT_CHATCOMMAND7)
	{
		SendCS_ACTION_REQ(XiahAniType::eLAT_Casual, 5,0);
	}
	else if( g_CommandList[ 0] == CHAT_CHATCOMMAND8 || g_CommandList[ 0] == CHAT_CHATCOMMAND9)
	{
		SendCS_ACTION_REQ(XiahAniType::eLAT_Casual, 4,0);
	}
	else if( g_CommandList[ 0] == CHAT_CHATCOMMAND10 || g_CommandList[ 0] == CHAT_CHATCOMMAND11)
	{
		SendCS_ACTION_REQ(XiahAniType::eLAT_Casual, 3,0);
	}

#ifndef MASTER
	// 일단 하나다
	if( g_CommandList[ 0] == _T("/portal_move"))
	{
		int wPosX = _tstoi( (LPCTSTR)g_CommandList[ 1]);
		int wPosY = _tstoi( (LPCTSTR)g_CommandList[ 2]);

		CXiahCharObject* pMainChar = (CXiahCharObject*)g_pMainChar->m_pObject;

		pMainChar->SetPosition( wPosX, wPosY);
		
		CXiahGame_Main *pGameMainStep = (CXiahGame_Main*)g_GameStep[ GAMESTEP_GAME];

		pGameMainStep->m_VisibleXiahObjectListNoAlpha.clear();
		pGameMainStep->m_VisibleXiahObjectListAlphaTest.clear();

		XiahMap::g_XiahMap.m_pMapRender->ReleaseVisibleMapCell();

		XiahObject::g_XiahObjectManager.ReleaseAllObjectExceptMainChar();
		g_PetList.JumpToPlayer();
		g_XiahCamera.m_bNeedUpdate = TRUE;
		g_XiahCamera.Update();

		XiahMap::g_XiahMap.Update();

		SendCS_NV_PORTALMOVE_REQ( g_pMainChar->m_dwServerID, 
								pMainChar->m_Position.x, 
								-pMainChar->m_Position.z,
								pMainChar->m_Position.y,CHARSTATE_NORMAL);
	}
	#ifdef _DEBUG_CHEAT
	else if( g_CommandList[ 0] == _T("/auto_battle"))
	{
		if( g_CommandList[ 1] == _T("-1"))
		{
			g_bCheat = TRUE;
			g_MainCharInfo.m_pScrMsg->SetScrMsg( 0, _T("!!"));
		}
		else if( g_CommandList[ 1] == _T("-2"))
		{
			g_bCheat = FALSE;
			g_MainCharInfo.m_pScrMsg->SetScrMsg( 0, _T("??"));
		}
	}
	else if( g_CommandList[ 0] == _T("/battle_750"))
	{
		if( g_CommandList[ 1] == _T("-155s"))
		{
			g_bCheatEtc = TRUE;
		}
		else if( g_CommandList[ 1] == _T("-off"))
		{
			g_bCheatEtc = FALSE;
		}
	}
	#endif

#endif

	return TRUE;
}
