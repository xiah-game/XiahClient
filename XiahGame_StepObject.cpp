#include "precompile.h"
#include "AppData.h"
#include "XiahGame_Intro.h"
#include "XiahGame_Main.h"
#include "XiahGameStartLoad.h"
#include ".\cxiahgame_login.h"

GAMESTEP_LIST g_GameStep;

CXiahGame_StepObject::CXiahGame_StepObject()
{
}

CXiahGame_StepObject::~CXiahGame_StepObject()
{
}

BOOL InitGameStepObject()
{
	g_GameStep.resize( GAMESTEP_COUNT);

	g_GameStep[ GAMESTEP_START_LOADING]	= new CXiahGameStartLoad;
	g_GameStep[ GAMESTEP_LOGIN]	= new CXiahGame_Login;
	g_GameStep[ GAMESTEP_INTRO]	= new XiahGame_Intro;
	g_GameStep[ GAMESTEP_GAME]	= new CXiahGame_Main;

	if(!g_GameStep[0] || !g_GameStep[1] || !g_GameStep[2] || !g_GameStep[3])
	{
		DBG_LogFile( _T("InitGameStepObject 실패"));

		//return false;
	}

	return TRUE;
}

BOOL ReleaseGameStepObject()
{
	GAMESTEP_LIST::iterator it;

	for(it = g_GameStep.begin(); it != g_GameStep.end(); it++)
	{
		CXiahGame_StepObject *pGameStep = *it;

		if( pGameStep)
		{
			delete pGameStep;
			pGameStep = NULL;
		}
		else
		{
			//DBG_LogFile( _T("ReleaseGameStepObject 실패"));

			//return false;
		}
	}

	g_GameStep.clear();

	return TRUE;
}
