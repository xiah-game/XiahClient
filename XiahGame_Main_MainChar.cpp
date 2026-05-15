#include "precompile.h"
#include "XiahGame_Main.h"
#include "XiahObject.h"
#include "XiahGameObject.h"
#include "XiahArrayIndex.h"
#include "XiahCamera.h"
#include "XiahMap.h"

#include "XiahCharAniType.h"
#include "XiahObjectType.h"
#include "ItemInfo.h"
#include "FunctionalNpcInfo.h"
#include "XiahGame_Handler_Sender.h"
#include "Fade.h"
#include "XiahCursor.h"
#include "InterfaceDefine.h"

#include "XiahGame_BGM.H"
#include "XiahGame_Pet.h"
#include "pcvisualinfo.h"

#include "AppData.h"



/*************************************************************************************************************
..............................................................................................................
......................SSSS...EEEEEE..PPPPP.....AA....RRRRR.....AA....TTTTTT...OOOO...RRRRR....................
.....................SS..SS..EE......PP..PP...AAAA...RR..RR...AAAA.....TT....OO..OO..RR..RR...................
.....................SS......EE......PP..PP..AA..AA..RR..RR..AA..AA....TT....OO..OO..RR..RR...................
......................SSSS...EEEEEE..PPPPP...AAAAAA..RRRR....AAAAAA....TT....OO..OO..RRRR.....................
.........................SS..EE......PP......AA..AA..RR.RR...AA..AA....TT....OO..OO..RR.RR....................
.....................SS..SS..EE......PP......AA..AA..RR..RR..AA..AA....TT....OO..OO..RR..RR...................
......................SSSS...EEEEEE..PP......AA..AA..RR..RR..AA..AA....TT.....OOOO...RR..RR...................
..............................................................................................................
*************************************************************************************************************/

BOOL FadeTrigger_MapMove(DWORD nIndex)
{
	SendCS_NV_MAPMOVE_REQ( XiahMap::g_XiahMap.m_MapInfo.m_dwMapID);

	return TRUE;
};

WORD	g_wDiePosX, g_wDiePosY;	// 앗싸 하드코딩

BOOL FadeTrigger_MainCharDie(DWORD nIndex)
{
	SendCS_NV_MAPMOVE_REQ( XiahMap::g_XiahMap.m_MapInfo.m_dwMapID, g_wDiePosX, g_wDiePosY);
	// 죽었으니까 딴곳으로 이동해 달라고 요청한다
	return TRUE;
};

// 내가 죽으면 몇 초 후에 mapinfo를 보내기 위해서 이걸 사용.
BOOL FadeTrigger_MainCharMapEnterAfterDie(DWORD nIndex)
{
	// 2004.07.20 이벤트용 로딩화면
	/*
	if( rand() % 2 )
		g_MainCharInfo.OpenFrame( EVENT_LOADING_1 );
	else
		g_MainCharInfo.OpenFrame( EVENT_LOADING_2 );
	*/

	g_MainCharInfo.OpenFrame(LOADING_IMAGE3); //HO_0702_07 등급표시 : 등급표시와 함게 스타트로딩과 게임로딩 부분이 동일 이미지로 처리된다.

	//등급표시 적용전 코드 나중에 지워 버리자 ..; 등급표시 전에는 나이 구분이 있엇다...
	// 성인서버용로딩
	//if(g_AppData.m_bAdult)
	//	g_MainCharInfo.OpenFrame(LOADING_IMAGE2);
	//else
	//	g_MainCharInfo.OpenFrame(LOADING_IMAGE);	

	SendCS_IT_MAPINFO_REQ( XiahMap::g_XiahMap.m_MapInfo.m_dwMapID );

	//	포탈 move와 마찬가지로 기존의 active한 map object들을 홀랑 다 지워 준다
	CXiahGame_Main *pGameMainStep = (CXiahGame_Main*)g_GameStep[ GAMESTEP_GAME];
//	pGameMainStep->m_VisibleXiahObjectList.clear();
	pGameMainStep->m_VisibleXiahObjectListNoAlpha.clear();
	pGameMainStep->m_VisibleXiahObjectListAlphaTest.clear();

	pGameMainStep->m_VisibleXiahCharObjectNameList.clear();
	pGameMainStep->m_VisibleXiahCharObjectList.clear();

	XiahObject::g_XiahObjectManager.ReleaseAllObjectExceptMainChar();

	// 현재 비, 눈 상태를 초기화.
	g_RainSnow.AllStop();

	return TRUE;
}


/*************************************************************************************************************
..............................................................................................................
......................SSSS...EEEEEE..PPPPP.....AA....RRRRR.....AA....TTTTTT...OOOO...RRRRR....................
.....................SS..SS..EE......PP..PP...AAAA...RR..RR...AAAA.....TT....OO..OO..RR..RR...................
.....................SS......EE......PP..PP..AA..AA..RR..RR..AA..AA....TT....OO..OO..RR..RR...................
......................SSSS...EEEEEE..PPPPP...AAAAAA..RRRR....AAAAAA....TT....OO..OO..RRRR.....................
.........................SS..EE......PP......AA..AA..RR.RR...AA..AA....TT....OO..OO..RR.RR....................
.....................SS..SS..EE......PP......AA..AA..RR..RR..AA..AA....TT....OO..OO..RR..RR...................
......................SSSS...EEEEEE..PP......AA..AA..RR..RR..AA..AA....TT.....OOOO...RR..RR...................
..............................................................................................................
*************************************************************************************************************/

extern BOOL SetupPC_VisualEquipement(CXiahCharObject* pObject, WORD* pVisualList, BYTE* pRarityList=NULL, BYTE* pStxTypeList=NULL);
extern bool SetupPET_VisualEquipement(CXiahCharObject* pObject, WORD* pVisualList);
extern void SpawnTestPet();
extern CUIManager* g_pUIManager;
extern BOOL InteractObject(DWORD dwObjectID,BYTE bObjType,int mode);


extern BOOL ProcessCursor();
//extern BOOL ProcessClientHelpMessage();
// 자동 이동은 마지막에 Object Interaction도 포함된다
// mode가 0이면 AutoNavigation시작
// 1이면 update
extern BOOL ProcessAutoNavigation(int mode);




#include "CreateMainChar.cpp"
#include "SetupPC_VisaulEquipment.cpp"
#include "ProcessMainChar.cpp"
#include "InteractObject.cpp"
#include "ProcessCursor.cpp"
#include "ProcessAutoNavigation.cpp"


/*************************************************************************************************************
..............................................................................................................
......................SSSS...EEEEEE..PPPPP.....AA....RRRRR.....AA....TTTTTT...OOOO...RRRRR....................
.....................SS..SS..EE......PP..PP...AAAA...RR..RR...AAAA.....TT....OO..OO..RR..RR...................
.....................SS......EE......PP..PP..AA..AA..RR..RR..AA..AA....TT....OO..OO..RR..RR...................
......................SSSS...EEEEEE..PPPPP...AAAAAA..RRRR....AAAAAA....TT....OO..OO..RRRR.....................
.........................SS..EE......PP......AA..AA..RR.RR...AA..AA....TT....OO..OO..RR.RR....................
.....................SS..SS..EE......PP......AA..AA..RR..RR..AA..AA....TT....OO..OO..RR..RR...................
......................SSSS...EEEEEE..PP......AA..AA..RR..RR..AA..AA....TT.....OOOO...RR..RR...................
..............................................................................................................
*************************************************************************************************************/

// 주인공 캐릭터가 먼가랑 부딫혔을때
int OnCollided_MainChar(unsigned long)
{
	if( g_pMainChar == NULL) // 어쭈구리 ㅡ,,ㅡ
		return 0;

	CXiahCharObject* pMainChar = (CXiahCharObject*)g_pMainChar->m_pObject;
#ifdef TRACE_LOG
	if(pMainChar == NULL)
	{
		DBG_LogFile( _T("OnCollided_MainChar 실패"));
	}
#endif
	SendCS_NV_ENDMOVE_REQ( g_pMainChar->m_dwServerID, pMainChar->m_Position.x, -pMainChar->m_Position.z, pMainChar->m_Position.y,CHARSTATE_NORMAL);
	pMainChar->SetAnimation( XiahAniType::eLAT_Stand, 0);

	// 자동 이동 취소
	dwSelObjectID = 0;
	dwSelObjectType = 0;
	bAutoAttack			= FALSE;
	bAutoNavigation		= FALSE;
	bAutoNormalAttack	= FALSE;
	g_MainChar_PreAttackInfo.nRemainAttackCount	 = 0;
	g_MainChar_PreAttackInfo.dwLastPreAttackTime = 0;

	return 1;	// 하던짓 그만둬라~!
}

int OnTimer_MainChar(unsigned long type)
{
	// 
	CXiahCharObject* pMainChar = (CXiahCharObject*)g_pMainChar->m_pObject;

#ifdef TRACE_LOG
	if(pMainChar == NULL)
	{
		DBG_LogFile( _T("OnTimer_MainChar 실패"));
	}
#endif
	if( pMainChar->m_bAttack)
	{
		// 공격용 Trigger닷!

		switch( pMainChar->m_bAttackType)
		{
		case 0: //일반 공격

			//HT_1026 : 스핵 방지
		//	if(g_MainChar_PreAttackInfo.bAttackReq)
		//	{
				SendCS_BT_ATTACK_REQ(g_MainChar_PreAttackInfo.bAttackType, 
					g_MainChar_PreAttackInfo.dwAttackID, 
					g_MainChar_PreAttackInfo.wAttackPosX, 
					g_MainChar_PreAttackInfo.wAttackPosY, 
					g_MainChar_PreAttackInfo.bAttackHeight, 
					g_MainChar_PreAttackInfo.bDefType, 
					g_MainChar_PreAttackInfo.dwDefID, 
					g_MainChar_PreAttackInfo.bAttackMode);

				g_MainChar_PreAttackInfo.nRemainAttackCount --;
				if( g_MainChar_PreAttackInfo.nRemainAttackCount < 0)
					g_MainChar_PreAttackInfo.nRemainAttackCount = 0;

				g_MainChar_PreAttackInfo.dwLastPreAttackTime = g_dwCurTime;
		//	}
			break;
		case 1: // 무공 공격
			SendCS_BT_MUGONGATTACK_REQ( g_MainChar_MugongPreAttackInfo.dwMugongID, 
										g_MainChar_MugongPreAttackInfo.bAttackType, 
										g_MainChar_MugongPreAttackInfo.dwAttackID, 
										g_MainChar_MugongPreAttackInfo.wAttackPosX, 
										g_MainChar_MugongPreAttackInfo.wAttackPosY, 
										g_MainChar_MugongPreAttackInfo.bAttackHeight, 
										g_MainChar_MugongPreAttackInfo.bDefendType, 
										g_MainChar_MugongPreAttackInfo.dwDefendID, 
										g_MainChar_MugongPreAttackInfo.wTargetPosX, 
										g_MainChar_MugongPreAttackInfo.wTargetPosY, 
										g_MainChar_MugongPreAttackInfo.bTargetHeight);
			break;
		}

	}

	return 0;
}

int OnEndTargetMove_MainChar(unsigned long param)
{
	CXiahCharObject *pMainChar = (CXiahCharObject *)g_pMainChar->m_pObject;
#ifdef TRACE_LOG
	if(pMainChar == NULL)
	{
		DBG_LogFile( _T("OnEndTargetMove_MainChar 실패"));
	}
#endif
//	pMainChar->SetAnimation( XiahAniType::eLAT_Stand, 0);

	SendCS_NV_ENDMOVE_REQ( g_pMainChar->m_dwServerID, pMainChar->m_Position.x, -pMainChar->m_Position.z, pMainChar->m_Position.y,CHARSTATE_NORMAL);

	return 0;
}

XiahGameEngine::Map::CMapDecal	g_NpcCursor[ 3];

int OnUpdateTargetDecal(unsigned long object)
{ 
	CXiahCharObject* pObject = (CXiahCharObject*)object;
#ifdef TRACE_LOG
	if(pObject == NULL)
	{
		DBG_LogFile( _T("OnUpdateTargetDecal 실패"));
	}
#endif
	Vector3 pos = pObject->m_Position;

	float size = pObject->m_LocalBound.Size().GetLength() * 0.7f;

	g_NpcCursor[ 0].Create( XiahPak::GetTexture( 50000397), pos.x, pos.z, size, 4294967295); // 4294967295 D3DCOLOR_XRGB( 255, 255, 255)
	g_NpcCursor[ 1].Create( XiahPak::GetTexture( 50000398), pos.x, pos.z, size, 4294967295); // 4294967295 D3DCOLOR_XRGB( 255, 255, 255)
	g_NpcCursor[ 2].Create( XiahPak::GetTexture( 50000399), pos.x, pos.z, size, 4294967295); // 4294967295 D3DCOLOR_XRGB( 255, 255, 255)

	g_NpcCursor[ 0].SetRotate( 0.00872664f ); //  _PI / 360.0f
	g_NpcCursor[ 1].SetRotate(-0.01745329f ); // -_PI / 180.0f
	g_NpcCursor[ 2].SetRotate( 0.03490658f ); //  _PI /  90.0f

	XiahMap::g_XiahMap.m_pMapRender->AddVisibalMapDecal( &g_NpcCursor[ 0]);
	XiahMap::g_XiahMap.m_pMapRender->AddVisibalMapDecal( &g_NpcCursor[ 1]);
	XiahMap::g_XiahMap.m_pMapRender->AddVisibalMapDecal( &g_NpcCursor[ 2]);

	return 0;
} 

/*************************************************************************************************************
..............................................................................................................
......................SSSS...EEEEEE..PPPPP.....AA....RRRRR.....AA....TTTTTT...OOOO...RRRRR....................
.....................SS..SS..EE......PP..PP...AAAA...RR..RR...AAAA.....TT....OO..OO..RR..RR...................
.....................SS......EE......PP..PP..AA..AA..RR..RR..AA..AA....TT....OO..OO..RR..RR...................
......................SSSS...EEEEEE..PPPPP...AAAAAA..RRRR....AAAAAA....TT....OO..OO..RRRR.....................
.........................SS..EE......PP......AA..AA..RR.RR...AA..AA....TT....OO..OO..RR.RR....................
.....................SS..SS..EE......PP......AA..AA..RR..RR..AA..AA....TT....OO..OO..RR..RR...................
......................SSSS...EEEEEE..PP......AA..AA..RR..RR..AA..AA....TT.....OOOO...RR..RR...................
..............................................................................................................
*************************************************************************************************************/
/*
#define CLIENT_HELP_MESSAGE_COUNT 12
LPCTSTR ClientHelpMessage[CLIENT_HELP_MESSAGE_COUNT] =
{
	_T("클라이언트 컨트롤 인터페이스가 대폭 수정 되었습니다."),
	_T("모든 기본 액션은 마우스 왼쪽 버튼으로 할 수 있도록 하였습니다."),
	_T("무공 및 PC간 대화에서만 마우스 오른쪽 버튼이 사용됩니다."),
	_T("NPC를 마우스 왼쪽으로 클릭하면 공격가능 거리까지 이동 후 1번만 공격합니다."),
	_T("이때 마우스 왼쪽 버튼을 누르고 있으면 계속 공격합니다."),
	_T("아이템이나 상점 NPC를 마우스 왼쪽으로 클릭하면, 일정 거리까지 이동 후 줍기 또는 거래가 이루어 집니다."),
	_T("PC(다른 사용자)를 마우스 왼쪽으로 클리하면 PC를 따라다닙니다. 계속~!!"),
	_T("PC를 마우스 오른쪽으로 클릭하면 대화가 가능합니다."),
	_T("무공은 마우스 오른쪽 버튼입니다."),
	_T("CTRL 버튼을 누르고 NPC를 왼쪽 버튼으로 클릭하면 자동 공격이 됩니다."),
	_T("자동 공격시에는 NPC가 죽을때까지 그 NPC만 공격합니다."),
	_T("이 메세지는 클라이언트가 자동으로 보내는메세지 이며, 클라이언트 시작 후 5번만 나옵니다.")
};

BOOL ProcessClientHelpMessage()
{
	static int step = 0;
	static DWORD dwStartTime = 0;
	static int MessageCount = 0;
	static int MessageTime = 0;
	static int GlobalCount = 5;	// 한 10번 보여주고 만다

	if( step == 0)
	{
		dwStartTime = g_dwCurTime - 55000;
		step = 1;
		return TRUE;
	}

	if( GlobalCount == 0)
		return TRUE;

	if( g_dwCurTime - dwStartTime > 60000)	// 1분에 한번씩
	{
		dwStartTime = g_dwCurTime;
		MessageTime = g_dwCurTime;
		MessageCount = CLIENT_HELP_MESSAGE_COUNT;
		GlobalCount --;
	}

	if( MessageCount == 0)
		return TRUE;

	if( g_dwCurTime - MessageTime > 3000)
	{
		MessageTime = g_dwCurTime;

		g_MainCharInfo.m_pScrMsg->SetScrMsg( 0, ClientHelpMessage[ CLIENT_HELP_MESSAGE_COUNT - MessageCount]);

		MessageCount --;
		if( MessageCount < 0)
			MessageCount = 0;
	}

	return TRUE;
}
*/