#include "precompile.h"
#include "XiahGame_Main.h"
#include "XiahCamera.h"
#include "XiahMap.h"
#include "XiahGameObject.h"	
#include "XiahObjectType.h"
#include "XiahGameMain.h"
#include "XiahEnvInfo.h"
#include "Fade.h"
#include "AppData.h"
#include "CharacterInfo.h"

#include "XiahGame_Minimap.h"
#include "XiahGame_BGM.h"
#include "XiahGame_Pet.h"
#include "cEFFECT_SPOT.h"
#include "functionalnpcinfo.h"

#include "munpamark.h"
#include "RebirthMark.h"


//HT_CHEAT : 치트 키
extern BOOL g_bCheat;
extern  BOOL g_bCheatEtc;

extern void RegisterNetworkHandler_Main();
extern BOOL ProcessMainChar();

unsigned long	g_SemiPK_Blink = 0;	// Semi PK 의 경고 깜빡임
CStaticTrigger* g_StaticTriggerList[ XIAH_STATIC_TRIGGER_COUNT];
sMainChar_PreAttackInfo g_MainChar_PreAttackInfo = {0,0,0,0,0,0,0,0,0,0,true,false}; //HT_1026 : 스핵 방지
sMainChar_MugongPreAttackInfo g_MainChar_MugongPreAttackInfo;

CXiahGame_Main::CXiahGame_Main()
{
	RegisterNetworkHandler_Main();

	ZeroMemory( g_StaticTriggerList, sizeof( CStaticTrigger*)*XIAH_STATIC_TRIGGER_COUNT);

	g_StaticTriggerList[ 0] = new CStaticTrigger( OnCollided_MainChar, 0);
	g_StaticTriggerList[ 1] = new CStaticTrigger( OnTimer_MainChar, 0);
	g_StaticTriggerList[ 2] = new CStaticTrigger( OnEndTargetMove_MainChar, 0);

#ifdef TRACE_LOG
	if(g_StaticTriggerList[ 0] == NULL || g_StaticTriggerList[ 1] == NULL || g_StaticTriggerList[ 2] == NULL)
	{
		DBG_LogFile( _T("CXiahGame_Main::CXiahGame_Main() 실패"));
	}
#endif

	InitEnergyGauge();

//	g_RainSnow.Start( eRain );
	g_GameWork.m_nNavigationMode = 0;
	if(!Minimap::CreateMiniMap())
	{
		DBG_LogFile( _T("CXiahGame_Main::CXiahGame_Main() 실패"));
	}

	InitXiahBGM();

	//
	for(int i=0; i<XIAH_PORTAL_NPC_MAX; i++)
		m_pPortalNPC[i] = NULL;

	m_nPortalNPCCount = 0;

	//
	m_bOnlyCameraMoveForTest = FALSE;
	m_fOnlyCameraMoveY = 4;

	m_Timer_UpdateMinimap = 0;	// MINI MAP
	m_Timer_UpdateSkyStar = 0;	// Sky Star
	g_SemiPK_Blink = 0;

	//m_nTestCount = 0;
}

CXiahGame_Main::~CXiahGame_Main()
{
#ifndef MASTER
	for(int i = 0; i < 8; ++i)
		g_tHelp[ i].Release();
#endif

	for(int i = 0; i < XIAH_STATIC_TRIGGER_COUNT; ++i)
	{
		if( g_StaticTriggerList[ i])
		{
			delete g_StaticTriggerList[ i];
			g_StaticTriggerList[ i] = NULL;
		}
	}

	if(!ReleasePortalNPC())
	{
		DBG_LogFile( _T("CXiahGame_Main::~CXiahGame_Main() 실패"));
	}

	ReleaseEnergyGauge();
	if(!Minimap::ReleaseMiniMap())
	{
		DBG_LogFile( _T("CXiahGame_Main::~CXiahGame_Main() 실패"));
	}

	ReleaseXiahBGM();
}

BOOL CXiahGame_Main::ReleasePortalNPC()
{
	if( m_nPortalNPCCount > 0 )
	{
		for(int i=0; i<m_nPortalNPCCount; i++)
		{
			XiahObject::g_XiahObjectManager.ReleaseObject( m_pPortalNPC[i] );
			m_pPortalNPC[i] = NULL;
		}// for
	}

	m_nPortalNPCCount = 0;

	return TRUE;
}

//#if TEST_PERFORMANCE
	static DWORD LastTime = 0;
	 BOOL  bLog = FALSE;
	static DWORD StartTime = 0;

#pragma comment( lib, "winmm.lib")
//#endif


//#if TEST_PERFORMANCE

#ifdef _DEBUG
#define START_PERFORMANCE	\
	StartTime = timeGetTime();

#define CHECK_PERFORMANCE(a)	\
	StartTime = timeGetTime() - StartTime;\
	if( bLog)\
	{\
		if(StartTime > 10)\
		{\
			DBG_LogFile("%s %d", a,StartTime );\
		}\
	}
#else

#define START_PERFORMANCE
#define CHECK_PERFORMANCE(a)

#endif



BOOL CXiahGame_Main::Update()
{
	// test code
	//m_nTestCount = 0;
	for(int i=0; i < 8; ++i)
	{
		g_tHelp[i].SetText(0,0, _T(" "), GetFont("若뗤퐪", 14), D3DCOLOR_XRGB( 0, 255, 255), 15);
	}

#ifndef MASTER
	//////////////////////////////////////////////////////////////////////////
	static long pre_playtime = timeGetTime();
	// 총 플레이시간 계산 (초)
	long g_playtime = (timeGetTime() - pre_playtime) / 1000;

	sString str;
	int min = g_playtime/60;
	int hour = min / 60;
	min = min - hour * 60;
	int sec = g_playtime - min*60 - hour*3600;

	str.printf( "PlayTime %d시간%d분%d초",hour,min,sec);
	g_tHelp[0].SetText( 400,25 ,str, GetFont("若뗤퐪", 12), D3DCOLOR_XRGB( 255, 255, 255), 15);

	//////////////////////////////////////////////////////////////////////////

	// FPS
	//str.printf( "FPS : %d, Last(T) %dms, %d ms",XiahGameEngine::GetFrameTimer(), g_AppData.dwTime, g_AppData.dwTime2);
	str.printf( "FPS : %d",XiahGameEngine::GetFrameTimer());
	g_tHelp[1].SetText( 300,25 ,str, GetFont("若뗤퐪", 12), D3DCOLOR_XRGB( 255, 255, 255), 15);

	if(g_bCheat)
	{
		//str.printf( "FPS : %d",XiahGameEngine::GetFrameTimer());
		g_tHelp[2].SetText( 300,10 ,"1", GetFont("若뗤퐪", 10), D3DCOLOR_XRGB( 255, 255, 255), 15);
	}
	else
	{
		g_tHelp[2].SetText( 300,10 ,"", GetFont("若뗤퐪", 10), D3DCOLOR_XRGB( 255, 255, 255), 15);
	}

	if(g_bCheatEtc)
	{
		g_tHelp[3].SetText( 330,10 ,"2", GetFont("若뗤퐪", 10), D3DCOLOR_XRGB( 255, 255, 255), 15);
		str.printf( "< %d s >", g_MainCharInfo.m_byCheatTime);
		g_tHelp[4].SetText( 350,10 ,str, GetFont("若뗤퐪", 13), D3DCOLOR_XRGB( 255, 255, 255), 15);
	}
	else
	{
		g_tHelp[3].SetText( 330,10 ,"", GetFont("若뗤퐪", 10), D3DCOLOR_XRGB( 255, 255, 255), 15);
		g_tHelp[4].SetText( 350,10 ,"", GetFont("若뗤퐪", 13), D3DCOLOR_XRGB( 255, 255, 255), 15);
	}



#endif

	g_XiahGameStarted = TRUE;
//#if TEST_PERFORMANCE
	bLog = FALSE;

	if( timeGetTime() - LastTime > 5000)
	{
		LastTime = timeGetTime();
		bLog = TRUE;
	}
//#endif

	START_PERFORMANCE;

	// 이건 카메라를 직접 움직이는 부분으로 스크린샷 전용 기능.
	if( m_bOnlyCameraMoveForTest )
	{
		MoveOnlyCameraForTest();

		ProcessXiahBGM(FALSE);
	}
	else
	{
		// 메인 캐릭터만 특별 대우
		ProcessMainChar();
	}

	// 카메라 Update
	g_XiahCamera.Update();
	CHECK_PERFORMANCE( "Camera And MainChar");

	// CG_2005/05/26 : 스카이맵 체인지
	g_Sky.FrameMove( (D3DXVECTOR3&)g_XiahCamera.m_vFrom, 0.05f );

	/*
	// SkyStar Update
	if(m_Timer_UpdateSkyStar + 1000 < g_dwCurTime)
	{
		m_Timer_UpdateSkyStar = g_dwCurTime;
		g_SkyStar.Update();
	}
	*/

	// Object Update
	START_PERFORMANCE;
	UpdateObject(); //HT_TEST : 오래 걸림
	CHECK_PERFORMANCE( "Update Object");

	// 배경은 반드시 카메라가 세팅된다음에 Update시켜준다
	START_PERFORMANCE;
	
	XiahMap::g_XiahMap.Update();

	CHECK_PERFORMANCE( "Update Map"); //HT_TEST : 오래 걸림

	// MINIMAP UPDATE
	if(m_Timer_UpdateMinimap + 50 < g_dwCurTime)
	{
		m_Timer_UpdateMinimap = g_dwCurTime;

		if( g_MainCharInfo.m_bShowMiniMap)
			Minimap::UpdateMinimap();
	}

	// Effect
	START_PERFORMANCE;
	// 경험치 획득 이펙트를 위해서 매번 메인 캐릭터의 중간 위치를 세팅한다.
	CXiahCharObject* pMainChar = (CXiahCharObject*)g_pMainChar->m_pObject;
	Vector3 vMainCharSize = pMainChar->m_LocalBound.Size();
	Vector3 vMainCharPos = pMainChar->m_Position + Vector3( 0, vMainCharSize.y*2.0f/3.0f, 0 );
	g_EffectManager.SetTargetMovePosition( (VECTOR)vMainCharPos );
	g_EffectManager.UpdateEffect(33.0f * g_fFrameScale);
	CHECK_PERFORMANCE( "Update Effect");//HT_TEST : 오래 걸림

	// Interface
	START_PERFORMANCE;
	g_MainCharInfo.Update();
	CHECK_PERFORMANCE( "Update UIManager");

	// 데미지 숫자 이펙트
	START_PERFORMANCE;
	g_HitEffect.Update();
	g_RainSnow.Update(33.0f * g_fFrameScale);
	Fade::UpdateFade();
	CHECK_PERFORMANCE( "Update etc.");

	// Grass Zone
	//XiahMap::g_XiahMap.UpdateGrassZone();

	// 이건 카메라를 직접 움직이는 부분으로 스크린샷 전용 기능.
/*
	if( GetAsyncKeyState( VK_CONTROL ) < 0 )
	if( GetAsyncKeyState( VK_LSHIFT ) < 0 )
	if( GetAsyncKeyState( VK_MULTIPLY ) < 0 )
	{
		m_bOnlyCameraMoveForTest = !m_bOnlyCameraMoveForTest;
        g_XiahCamera.m_bFollowMainChar = !g_XiahCamera.m_bFollowMainChar;

		if( m_bOnlyCameraMoveForTest )
		{
            g_XiahCamera.m_vNewAt = g_XiahCamera.m_vAt;
			g_XiahCamera.m_bOnlyMoveCameraForTest = true;
			m_fOnlyCameraMoveY = 4;
			g_MainCharInfo.HideAllFrame();
		}
		else
		{
			g_XiahCamera.m_bOnlyMoveCameraForTest = false;
			g_MainCharInfo.HideAllFrame();
		}
	}
*/

	return TRUE;
}

BOOL CXiahGame_Main::Render()
{
	//HT_CHEAT : 와이어 화면 보이기
	if( g_AppData.m_bWireframe )
		g_pDirect3DDevice->SetRenderState( D3DRS_FILLMODE, D3DFILL_WIREFRAME);
	else
		g_pDirect3DDevice->SetRenderState( D3DRS_FILLMODE, D3DFILL_SOLID);

//	g_pDirect3DDevice->SetRenderState( D3DRS_FOGENABLE,			TRUE  );
	g_pDirect3DDevice->SetRenderState( D3DRS_ZENABLE,			TRUE  );
//	g_pDirect3DDevice->SetRenderState( D3DRS_ZWRITEENABLE,		TRUE  );
	g_pDirect3DDevice->SetRenderState( D3DRS_ALPHABLENDENABLE,	FALSE );
	g_pDirect3DDevice->SetRenderState( D3DRS_ALPHATESTENABLE,	FALSE );

	// !! 앞으로 이 규칙을 꼭 지킬것 !!
	// 렌더링 모듈을 넣을때 SetRenderState이 중복되거나 괜히 바뀌지 않도록 주의!!
	// 원칙적인 순서 : No Alpha -> Alpha -> Alpha Test
 
	// 1. No Alpha Render
	START_PERFORMANCE;
	// CG_2005/05/26 : 스카이맵 체인지
	g_Sky.Render();
	//g_SkyBox.Render();
	CHECK_PERFORMANCE( "Render Sky");

	// 얘만 예외로 여기에 두자.
	//g_SkyStar.Render();

	START_PERFORMANCE;
	XiahMap::g_XiahMap.RenderTerrain();
//	CHECK_PERFORMANCE( "Render Terrain"); //HT_TTEST : 오래걸림
	// 앞으로 얘는 No Alpha가 될것이다.
	XiahMap::g_XiahMap.RenderWater(); 

	// 메인 캐릭터가 암흑무에 걸리면 아예 안그린다.
	if( !g_XiahEnvInfo.m_bAmhukmuFog )
	{
		// 알파가 없는 맵 오브젝트
		START_PERFORMANCE;
		XiahMap::g_XiahMap.RenderObject(1);
	//	CHECK_PERFORMANCE( "Render Map Object No Alpha");

		// 알파가 없는 캐릭터만 그린다.
		START_PERFORMANCE;
		RenderCharObject(false);
	//	CHECK_PERFORMANCE( "Render Char Object No Alpha");

		// 2. Alpha
		// 알파가 있는 맵 오브젝트
		START_PERFORMANCE;
		XiahMap::g_XiahMap.RenderObject(3);
	//	CHECK_PERFORMANCE( "Render Map Object Alpha");

		// 알파 Test가 있는 맵 오브젝트
		START_PERFORMANCE;
		XiahMap::g_XiahMap.RenderObject(2);
	//	CHECK_PERFORMANCE( "Render Map Object Alpha Test");

		//XiahMap::g_XiahMap.RenderGrassZone();
	}

	START_PERFORMANCE;
	RenderCharObject(true);
//	CHECK_PERFORMANCE( "Render Char Object Alpha Test");

	// 메인 캐릭터가 암흑무에 걸리면 아예 안그린다.
	if( !g_XiahEnvInfo.m_bAmhukmuFog )
	{
		// 4. Alpha Render, 그 다음에 그려져야 할것들.
		//g_SkyStar.Render();

		// 캐릭터 이름.
		RenderCharName();

		START_PERFORMANCE;
		g_EffectManager.RenderEffect( 33.0f * g_fFrameScale );
		g_EffectManager.RenderOthers( 33.0f * g_fFrameScale );
		CHECK_PERFORMANCE( "Render Effect");
	}

	// 데미지 숫자 이펙트
	g_HitEffect.Render();

	// 눈, 비
	g_RainSnow.Render();

	// 5. ETC, 가장 마지막에 그려져야 할것들.
	Fade::RenderFade();


	START_PERFORMANCE;
	if( g_MainCharInfo.m_bShowMiniMap)
		Minimap::RenderMinimap();

	
	g_MainCharInfo.Render();

	g_pDirect3DDevice->SetRenderState( D3DRS_FILLMODE, D3DFILL_SOLID);


	for(int i = 0; i < 8; ++i)
		g_tHelp[i].Render();

//	CHECK_PERFORMANCE( "Render UI"); //HT_TEST : 오래걸림

	Update_Special_Effect();

	return TRUE;
}

BOOL CXiahGame_Main::UpdateObject()
{
	if( g_pCurrentCamera == NULL)
		return TRUE;

	float	fMinDistance = 100000.0f;
	Vector3	vStart = g_pCurrentCamera->GetCursorWorld( 0.0001f);	// 0하고 1주면 끝장이야~
	Vector3 vEnd   = g_pCurrentCamera->GetCursorWorld( 0.9999f);

	CXiahCharObject* pMainCharObject = (CXiahCharObject*)g_pMainChar->m_pObject;

	XiahObject::CXiahObject* pPreMouseOnObject = XiahObject::g_pMouseOnObject;
	XiahObject::g_pMouseOnObject = NULL;

	std::list<XiahObject::CXiahObject*> DeleteObjectList;

	XiahMap::g_XiahMap;

	int nUpdatedObjectCount = 0;

	// 말도 안되는 리스트는 뺀다.
	typedef std::list<XiahObject::CXiahObject*> XIAHOBJECTLIST;
	XIAHOBJECTLIST DeleteXiahObjectList;

//	if( g_pCurrentCamera->m_bUpdate)
	{
		// 이젠 알파 있는 것과 없는 것을 구별해서 찍자.
//		m_VisibleXiahObjectList.clear();
		m_VisibleXiahObjectListNoAlpha.clear();
		m_VisibleXiahObjectListAlphaTest.clear();

		// 화면에 보이는 캐릭터 이름.
		m_VisibleXiahCharObjectNameList.clear();
		m_VisibleXiahCharObjectList.clear();

		XiahObject::CXiahObjectManager::iterator it;

		for(it = XiahObject::g_XiahObjectManager.begin(); it != XiahObject::g_XiahObjectManager.end(); it++)
		{
			XiahObject::CXiahObject *pXiahObject = it->second;
			CXiahCharObject *pCharObject = reinterpret_cast<CXiahCharObject*>(pXiahObject->m_pObject);
			
			DBG_Assert( pXiahObject != NULL);

			if( pXiahObject == NULL ) // 이건 말도 안된다.
			{
				DeleteXiahObjectList.push_back( pXiahObject );
				continue;
			}

			//DBG_Assert( pXiahObject->m_pObject != NULL);

			XiahObject::CXiahObject_Basic *pObjectBasic = pXiahObject->m_pObject;
			if( pObjectBasic == NULL )	// 이것도 말도 안된다. 뭐가 문제야?
			{
				DeleteXiahObjectList.push_back( pXiahObject );
				continue;
			}

			if( !pObjectBasic->IsKindOf( XiahObject::eXOT_3DObject))
				continue; // 여기는 거의 않걸린다

			CXiah3DObject *p3DObject = reinterpret_cast<CXiah3DObject*>(pObjectBasic);


			if( pXiahObject == g_pMainChar)
			{
				m_VisibleXiahObjectListAlphaTest.push_back( pXiahObject);

				m_VisibleXiahCharObjectList.push_back( pXiahObject);

				if(g_info.m_bShowNickname && pCharObject->m_bShowObjectName == TRUE)
					m_VisibleXiahCharObjectNameList.push_back( &p3DObject->m_tObjectName );
				
				continue;
			}

			if(!pCharObject->m_bRenderOK)
			{
				pObjectBasic->Update(false);
				continue;
			}
			
			if( pObjectBasic->m_bDeleteME)
			{
				DeleteObjectList.push_back( pXiahObject);
				continue;
			}

			// PortalNPC.
			bool bIsPortalNPC = false;
			for(int h=0; h<m_nPortalNPCCount; h++)
			{
				if( m_pPortalNPC[h] == pXiahObject )
				{
					bIsPortalNPC = true;
					break;
				}
			}
			if( bIsPortalNPC )
			{
				BOOL bVisible = g_pCurrentCamera->Visible( p3DObject->m_WorldBound);

				pObjectBasic->Update(bVisible);
				nUpdatedObjectCount++;

				if( bVisible )
				{
					// 마우스로 찍고 있는지 검사
					Vector3 vDistance = vStart - p3DObject->m_WorldBound.m_Center;
					float fDistance = vDistance.GetLength();

					if( p3DObject->m_WorldBound.IsIntersect( vStart, vEnd, NULL))
					{
						if( fDistance < fMinDistance)
						{
							fMinDistance = fDistance;
							XiahObject::g_pMouseOnObject = pXiahObject;
						}
					}

//					m_VisibleXiahObjectList.push_back( pXiahObject);
					// 포탈은 No alpha
					m_VisibleXiahObjectListNoAlpha.push_back( pXiahObject);

					m_VisibleXiahCharObjectNameList.push_back( &p3DObject->m_tObjectName );
					m_VisibleXiahCharObjectList.push_back( pXiahObject);
				}// if

/*
				// Portal NPC 무조건 그리기.
				pObjectBasic->Update();
				nUpdatedObjectCount++;

				CXiah3DObject *p3DObject = reinterpret_cast<CXiah3DObject*>(pObjectBasic);
				// 마우스로 찍고 있는지 검사
				Vector3 vDistance = vStart - p3DObject->m_WorldBound.m_Center;
				float fDistance = vDistance.GetLength();

				if( p3DObject->m_WorldBound.IsIntersect( vStart, vEnd, NULL))
				{
					if( fDistance < fMinDistance)
					{
						fMinDistance = fDistance;
						XiahObject::g_pMouseOnObject = pXiahObject;
					}
				}
				m_VisibleXiahObjectList.push_back( pXiahObject);
*/

				continue;
			}// if( bIsPortalNPC )

			// Quest NPC, 모두 파괴되고 이제 지줘준다.
            CXiahCharObject *pQuestNPC = reinterpret_cast<CXiahCharObject*>(pObjectBasic);
			if( pQuestNPC->m_bObjType == OBJTYPE_NPC && pQuestNPC->m_bExSubObjType != 255 )
			{

				if( pQuestNPC->m_CharRender.GetMeshType() == 2 &&
                    pQuestNPC->m_CharRender.m_pMeshEffectPackagePair &&
					!pQuestNPC->m_CharRender.m_pMeshEffectPackagePair->bNowUsing )
				{
					pQuestNPC->SetAnimation( XiahAniType::eLAT_Die, XiahAniType::eLAT_Die, 0, 1);
					pQuestNPC->m_bTargetMove = FALSE;

					// 현재 캐릭터가 Mesh Effect가 있으면 없애준다.
					pQuestNPC->m_CharRender.ClearMeshEffect();
				}
			}

			// Bound넘어가는 넘은 지워준다
			float fMainCharDistance = pMainCharObject->GetDistance( p3DObject->m_Position.x, -p3DObject->m_Position.z);

			// 애완동물은 않지워줌
			if( fMainCharDistance > SERVER_INTERACTION_DISTANCE && g_PetList.Find( pXiahObject->m_dwServerID) == NULL)
			{
				//DBG_Put("Bound 넘어가 Object 지워줌 ID : %d", pXiahObject->m_dwServerID);
				// 지워줄 리스트에 추가해줘 버린다
				DeleteObjectList.push_back( pXiahObject);
				continue;
			}

			CXiahCharObject *pXiahCharObject = reinterpret_cast<CXiahCharObject*>(pObjectBasic);

			// 2004.08.04 Changth
			// 분신격일때, 오랫동안 시체(?)가 남아 있는 경우가 있는데,
			// 죽으면 조금 후에 바로 강제로 지워준다.
			if( pXiahCharObject->m_bObjType		== OBJTYPE_PET &&
				pXiahCharObject->m_bSubObjType	== 1 &&
				pXiahCharObject->m_bObjStatus	== NPCSTATUS_DIE )
			{
				if( g_dwCurTime - pXiahCharObject->m_LastNavigationTime >= 3300 )
				{
					DeleteObjectList.push_back( pXiahObject);
					continue;
				}
			}

			//HT_CHEAT : 응룡도 안 지워질때가 있넹.. ㅡㅡ;
			if( pXiahCharObject->m_bObjType		== OBJTYPE_PET &&
				pXiahCharObject->m_bSubObjType	== 250 &&
				pXiahCharObject->m_bObjStatus	== NPCSTATUS_DIE )
			{
				if( g_dwCurTime - pXiahCharObject->m_LastNavigationTime >= 3300 )
				{
					DeleteObjectList.push_back( pXiahObject);
					continue;
				}
			}

			// Main Work
			BOOL bObjectVisible = TRUE;

			if( fMainCharDistance > g_XiahEnvInfo.m_CameraBoundSize / 2)
				bObjectVisible = FALSE;
			
			if( bObjectVisible)
				bObjectVisible = g_pCurrentCamera->Visible( p3DObject->m_WorldBound);

			pObjectBasic->Update(bObjectVisible);
			nUpdatedObjectCount++;

			if( bObjectVisible)
			{
//				CXiahCharObject *pXiahCharObject = reinterpret_cast<CXiahCharObject*>(pObjectBasic);
#ifdef TRACE_LOG
				if(pXiahCharObject == NULL)
				{
					DBG_LogFile( _T("CXiahGame_Main::UpdateObject 실패"));
				}
#endif

				// 마우스로 찍고 있는지 검사
				Vector3 vDistance = vStart - p3DObject->m_WorldBound.m_Center;
				float fDistance = vDistance.GetLength();

				if( pXiahObject->m_dwServerID != 0)	// 서버와 Interaction되는 넘만 Picking가능하다
				{
					if( p3DObject->m_WorldBound.IsIntersect( vStart, vEnd, NULL))
					{
						if( fDistance < fMinDistance)
						{
							BOOL bPickable = TRUE;

							if( p3DObject->m_bObjType == OBJTYPE_NPC)
							{
								if( pXiahCharObject->m_nCurMotionType == XiahAniType::eLAT_Die || pXiahCharObject->m_bObjStatus == NPCSTATUS_HIDE)
								{
									bPickable = FALSE;
								}
							}
							// [5/17/2005] 남 펫은 액션 완전 제외
							else if(p3DObject->m_bObjType == OBJTYPE_PET)
							{
								if(g_PetList.GetCurrentPet())
								{
									if(pXiahCharObject->m_pPrivateData)
									{
										sPetInfo* pPetInfo = (sPetInfo *)pXiahCharObject->m_pPrivateData;

										if(pPetInfo && pPetInfo->dwOwnID != g_pMainChar->m_dwServerID)
										{
											bPickable = FALSE;
										}
									}
									else
									{
										bPickable = FALSE;
									}
								}
								else
								{
									bPickable = FALSE;
								}
							}

							if( bPickable)
							{
								fMinDistance = fDistance;
								XiahObject::g_pMouseOnObject = pXiahObject;
							}
						}
					}
				}

				/*

				if( pObjectBasic->IsA( XiahObject::eXOT_CharObject))
				{
					float fDetailLevel;

					if( fDistance > (g_XiahEnvInfo.m_CameraBoundSize))
						fDetailLevel = 0;
					else
						fDetailLevel = 1.0f - (fDistance / (g_XiahEnvInfo.m_CameraBoundSize));

					int nDetailLevel = fDetailLevel * 10.0f;
					
					pXiahCharObject->m_CharRender.SetDetailLevel( (float)nDetailLevel / 10.0f );
				}
				*/

				//
//				m_VisibleXiahObjectList.push_back( pXiahObject);
				BOOL bIsAlphaTest = pXiahCharObject->m_CharRender.IsAlphaBlendTestObject();
				if( bIsAlphaTest )
					m_VisibleXiahObjectListAlphaTest.push_back( pXiahObject);
				else
					m_VisibleXiahObjectListNoAlpha.push_back( pXiahObject);

				if(pXiahCharObject->m_bShowObjectName == TRUE)
				{
					m_VisibleXiahCharObjectNameList.push_back( &p3DObject->m_tObjectName );
				}
				m_VisibleXiahCharObjectList.push_back( pXiahObject);

			}// if( bObjectVisible)
		}// for( g_XiahObjectManager )

		//DBG_Put("보이는 넘들수 : %d", m_VisibleXiahObjectList.size());
	}

	if( XiahObject::g_pMouseOnObject != pPreMouseOnObject)
	{
		if( pPreMouseOnObject != NULL)
		{
			if( pPreMouseOnObject->m_pObject->IsA( XiahObject::eXOT_CharObject))
			{
				CXiahCharObject* pCharObject = (CXiahCharObject*)pPreMouseOnObject->m_pObject;

				pCharObject->EnableGlowEffect( FALSE);
			}
		}

		if( XiahObject::g_pMouseOnObject != NULL)
		{
			if( XiahObject::g_pMouseOnObject->m_pObject->IsA( XiahObject::eXOT_CharObject))
			{
				CXiahCharObject* pCharObject = (CXiahCharObject*)XiahObject::g_pMouseOnObject->m_pObject;

				pCharObject->EnableGlowEffect( TRUE);
			}
		}
	}

	//
	std::list<XiahObject::CXiahObject*>::iterator ob_it;
	for(ob_it = DeleteObjectList.begin(); ob_it != DeleteObjectList.end(); ob_it++)
	{
		//XiahObject::CXiahObject *pObject = *ob_it;
#ifdef TRACE_LOG
		if(pObject == NULL)
		{
			DBG_LogFile( _T("CXiahGame_Main::UpdateObject 실패"));
		}
#endif
		//DBG_Put(_T("Bound 넘어가거나 죽은 Object 지워줌 : %d, 현재 총갯수 : %d"), pObject->m_dwServerID, XiahObject::g_XiahObjectManager.size());
		XiahObject::g_XiahObjectManager.ReleaseXiahObject( *ob_it);
	}
	DeleteObjectList.clear();

	// 말도 안되지만, 데이타가 없는 CXiahObject는 지운다.
	XIAHOBJECTLIST::iterator xdit;
	for(xdit=DeleteXiahObjectList.begin(); xdit!=DeleteXiahObjectList.end(); xdit++)
	{
		XiahObject::g_XiahObjectManager.ReleaseXiahObject( *xdit );
	}

	//
	/*
	sString str;
	str.printf( "VideoMemory: %d", g_pDirect3DDevice->GetAvailableTextureMem());
	g_tHelp[ 0].SetText( 0, 16, str, GetFont("若뗤퐪", 14), D3DCOLOR_XRGB( 255, 255, 255), 0);
*/
/*
#if TEST_PERFORMANCE
	
	str.printf( "V Obj : %d  Uptd Ob : %d, MC : %d, TrMB : %d, Map Obj : %d, IsOnUI : %d, Fog : %.6f",
		m_VisibleXiahObjectList.size(), 
		nUpdatedObjectCount,
		XiahMap::g_XiahMap.m_pMapRender->GetVisibleMapCellCount(),
		XiahMap::g_XiahMap.m_pMapRender->GetVisibleMeshBlockCount(),
		XiahMap::g_XiahMap.m_pMapRender->GetVisibleObjectCount(),
		g_pUIManager->IsMouseOnFrame(),
		g_XiahEnvInfo.m_fFogDensity
		);
	g_tHelp[ 5].SetText( 0, 32 + 5 * 16,str, GetFont("若뗤퐪", 14), D3DCOLOR_XRGB( 255, 255, 255), 15);

	str.printf("Position : %d - %d -- Vertex %d Tri %d", 
		(int)g_XiahCamera.m_vAt.x, 
		(int)-g_XiahCamera.m_vAt.z,
		g_EngineInfo.m_nRenderedVertex,
		g_EngineInfo.m_nRenderedFace
		);
	g_tHelp[ 6].SetText( 0, 32 + 6 * 16,str, GetFont("若뗤퐪", 14), D3DCOLOR_XRGB( 255, 255, 255), 15);

	// effect
	str.printf( "PP:%d, ER:%d, PPpool:%d, Ppool:%d, ERpool:%d, PRpool:%d, ELRpool:%d, VRpool:%d, Vc:%d, Fc;%d",
		g_EffectManager.GetCurPackagePairListSize(),
		g_EffectManager.GetCurEffectRenderListSize(),
		g_EffectManager.GetPackagePairPoolSize(),
		g_EffectManager.GetPackagePoolSize(),
		g_EffectManager.GetEffectRenderPoolSize(),
		g_EffectManager.GetParticleRenderPoolSize(),
		g_EffectManager.GetElementRenderPoolSize(),
		g_EffectManager.GetVertexRenderPoolSize(),
		g_EffectManager.GetTotalRenderedVertex(),
		g_EffectManager.GetTotalRenderedFace()
		);
	g_tHelp[ 7].SetText( 0, 32 + 7 * 16,str, GetFont("若뗤퐪", 14), D3DCOLOR_XRGB( 255, 255, 255), 15);
#endif


	// effect
	sString str;
	str.printf( "PP:%d, ER:%d, PPpool:%d, Ppool:%d, ERpool:%d, PRpool:%d, ELRpool:%d, VRpool:%d, Vc:%d, Fc;%d",
		g_EffectManager.GetCurPackagePairListSize(),
		g_EffectManager.GetCurEffectRenderListSize(),
		g_EffectManager.GetPackagePairPoolSize(),
		g_EffectManager.GetPackagePoolSize(),
		g_EffectManager.GetEffectRenderPoolSize(),
		g_EffectManager.GetParticleRenderPoolSize(),
		g_EffectManager.GetElementRenderPoolSize(),
		g_EffectManager.GetVertexRenderPoolSize(),
		g_EffectManager.GetTotalRenderedVertex(),
		g_EffectManager.GetTotalRenderedFace()
		);
	g_tHelp[ 7].SetText( 0, 32 + 7 * 16,str, GetFont("若뗤퐪", 14), D3DCOLOR_XRGB( 255, 255, 255), 15);
*/
	return TRUE;
}

BOOL CXiahGame_Main::RenderCharObject(bool bIsAlphaTest)
{
	if( bIsAlphaTest )
		RenderCharObjectFromList( m_VisibleXiahObjectListAlphaTest );
	else
	{
		m_PetObjectList.clear();
		RenderCharObjectFromList( m_VisibleXiahObjectListNoAlpha );
	}

	return TRUE;
}

BOOL CXiahGame_Main::RenderCharObjectFromList(VISIBLE_XIAHOBJECT_LIST& VisibleXishObjectList)
{
	VISIBLE_XIAHOBJECT_LIST::iterator it;

	for(it = VisibleXishObjectList.begin(); it != VisibleXishObjectList.end(); it++)
	{
		XiahObject::CXiahObject *pObject = *it;
		XiahObject::CXiahObject_Basic *pObjectBasic = pObject->m_pObject;
#ifdef TRACE_LOG
		if(pObject == NULL || pObjectBasic == NULL)
		{
			DBG_LogFile( _T("CXiahGame_Main::RenderCharObjectFromList 실패"));
		}
#endif
		// 일단 3DObject는 않그려 준다
		if( !pObjectBasic->IsA( XiahObject::eXOT_CharObject))
			continue;

		// 메인 캐릭터가 암흑무에 걸리면 아예 안그린다.
		if( g_XiahEnvInfo.m_bAmhukmuFog && g_pMainChar != pObject )
			continue;

		CXiahCharObject *pCharObject = reinterpret_cast<CXiahCharObject*>(pObjectBasic);
#ifdef TRACE_LOG
		if(pCharObject == NULL)
		{
			DBG_LogFile( _T("CXiahGame_Main::RenderCharObjectFromList 실패"));
		}
#endif
		if( XiahObject::g_pMouseOnObject == pObject)
		{
			pCharObject->m_bShowObjectName = TRUE; 

			// 광물은 에너지 통과
			if( pCharObject->m_bObjType == OBJTYPE_NPC && pCharObject->m_bExSubObjType != 3)
				pCharObject->m_bShowGage = TRUE;
		}
		else
		{
			if( pObject->m_pObject->m_bObjType == OBJTYPE_PET && g_PetList.Find( pObject->m_dwServerID) != NULL)
			{
				pCharObject->m_bShowGage = TRUE;
				pCharObject->m_bShowObjectName = TRUE;
			}
			else
			{
				if(pCharObject->m_bObjType == OBJTYPE_NPC)
				{
					if((GetAsyncKeyState( VK_MENU) < 0 || g_info.m_bShowNPCname) && pCharObject->m_bObjStatus != NPCSTATUS_HIDE)
					{
						pCharObject->m_bShowObjectName = TRUE;
					}
					else
					{
						pCharObject->m_bShowObjectName = FALSE;
					}
				}
				else if(pCharObject->m_bObjType == OBJTYPE_PC)
				{
					// 옵션에 따른 별호 보기.
					if(g_info.m_bShowNickname)
					{
						if(pCharObject->m_bSemiPKStatus == 2)
						{
							if(g_SemiPK_Blink + 300 < g_dwCurTime)
							{
								pCharObject->m_bShowObjectName = !pCharObject->m_bShowObjectName;
								g_SemiPK_Blink = g_dwCurTime;
							}
						}
						else
						{
							// 화면에 보인다.
							pCharObject->m_bShowObjectName = TRUE;
						}
					}
					else
					{
						pCharObject->m_bShowObjectName = FALSE;
					}
				}
				else if(pCharObject->m_bObjType == OBJTYPE_FUNCTIONALNPC || pCharObject->m_bObjType == OBJTYPE_ITEM || pCharObject->m_bObjType == OBJTYPE_PET)
				{
					pCharObject->m_bShowObjectName = TRUE;
				}

				if( pObject != g_pMainChar)
					pCharObject->m_bShowGage = FALSE;

				if(pCharObject->m_bObjType == OBJTYPE_FUNCTIONALNPC)
				{
					sFunctionalNpcInfo* pInfo = (sFunctionalNpcInfo*)pCharObject->m_pPrivateData;

					//YS_0811 : BUGFIX
					if ( !pInfo ) 
						continue;

					if(pInfo->m_bKind == 100)
						pCharObject->m_bShowGage = TRUE;
				}
			}
		}
		
		// PET의 이름을 찍고 상태를 찍고
		if( pCharObject->m_bObjType == OBJTYPE_PET && g_PetList.Find( pObject->m_dwServerID) != NULL)
		{
			//pCharObject->m_cGageColor = D3DCOLOR_XRGB( 18, 194, 18);
			pCharObject->m_cNameColor = D3DCOLOR_XRGB( 200, 200, 255);

			// 선택된 PET이면 바닥에 선택된 표시를 그린다
			/*
			sPetInfo* pPetInfo = (sPetInfo*)pCharObject->m_pPrivateData;;
			if(pPetInfo->bSelected == TRUE)
			{
				pet_mark.Create( XiahPak::GetTexture( 50000398), pCharObject->m_Position.x, pCharObject->m_Position.z, 6, COLOR_PICKCURSOR, 1);
				pet_mark.SetRotate( _PI / 90.0f);
				XiahMap::g_XiahMap.m_pMapRender->AddVisibalMapDecal( &pet_mark);
			}
			*/
			m_PetObjectList.push_back( pObject);
		}

		// NPC의 정보를 찍고
		if( pCharObject->m_bObjType == OBJTYPE_NPC)
		{
			pCharObject->m_cNameColor = D3DCOLOR_XRGB( 255, 200, 200);
		}
		else if( pCharObject->m_bObjType == OBJTYPE_NPC && pCharObject->m_bObjStatus == NPCSTATUS_DIE)
		{
			pCharObject->m_cNameColor = D3DCOLOR_XRGB( 255, 0, 0);
		}

		// HIDE 상태 위하여 테스트 코드
		if(pCharObject->m_bObjStatus == NPCSTATUS_HIDE)
		{
			DBG_Put("Hide");
			continue;
		}

		pCharObject->m_pUpdateTargetDecal = NULL;

		if( dwSelObjectType == OBJTYPE_NPC && dwSelObjectID == pObject->m_dwServerID && dwSelObjectType == pCharObject->m_bObjType)
			pCharObject->m_pUpdateTargetDecal = OnUpdateTargetDecal;

		if( dwSelObjectID == pObject->m_dwServerID && dwSelObjectType == pCharObject->m_bObjType && XiahObject::g_pMouseOnObject != pObject)
		{
			D3DCOLOR color;

			switch( dwSelObjectType)
			{
			case OBJTYPE_NPC:
				color = D3DCOLOR_XRGB( 255, 128, 0);
				break;
			default:
				color = D3DCOLOR_XRGB( 128, 128, 255);
				break;
			}
			
			pCharObject->EnableGlowEffect( TRUE, color);
			
			if( dwSelObjectType == OBJTYPE_NPC)
			{
				// 광물은 에너지 통과
				if(pCharObject->m_bExSubObjType != 3)
					pCharObject->m_bShowGage = TRUE;

				pCharObject->m_bShowObjectName = TRUE;
			}
		}
		else if( XiahObject::g_pMouseOnObject != pObject)
		{
			if( pCharObject->m_bGlowEnable)
				pCharObject->EnableGlowEffect( FALSE);
		}

//		DrawBound( pCharObject->m_WorldBound, D3DCOLOR_XRGB( 255, 0, 0));
//		if( pObject == g_pMainChar)
//		{
//			pCharObject->m_bShowObjectName = TRUE;
//		}

		pCharObject->Render();
	}

	return TRUE;
}

BOOL CXiahGame_Main::RenderCharName()
{
	TEXT2DLIST::iterator it = m_VisibleXiahCharObjectNameList.begin();

	// 보이는 이름
	for(; it != m_VisibleXiahCharObjectNameList.end(); ++it)
	{
			CText2D* pText2D = *it;

			if(pText2D == NULL)
				continue;

			pText2D->Render();
	} // for(; it != m_VisibleXiahCharObjectNameList.end(); ++it)

	sVisibleCharLIST::iterator it_pet = m_PetObjectList.begin();

	for(; it_pet != m_PetObjectList.end(); ++it_pet)
	{
		XiahObject::CXiahObject *pXiahObject = *it_pet;
		if(!pXiahObject)
			continue;

		XiahObject::CXiahObject_Basic *pObjectBasic = pXiahObject->m_pObject;
		if(!pObjectBasic)
			continue;
		CXiahCharObject *pXiahCharObject = reinterpret_cast<CXiahCharObject*>(pObjectBasic);
		if(!pXiahCharObject)
			continue;

		Vector3 scPos = g_pCurrentCamera->WorldToScreen( pXiahCharObject->m_Position + Vector3( 0, pXiahCharObject->m_LocalBound.m_vMax.y + 1, 0));

		pXiahCharObject->m_rcObjectScreenPos.left = scPos.x;
		pXiahCharObject->m_rcObjectScreenPos.top = scPos.y;

		sPetInfo* pPetInfo = g_PetList.GetCurrentPet();		

		g_PetList.RenderWild( pXiahCharObject->m_rcObjectScreenPos.left - (pPetInfo->szName.length()*2 + 30),pXiahCharObject->m_rcObjectScreenPos.top-3, pPetInfo->bWildRate);
	}

	if(g_info.m_bShowNickname || g_info.m_bHideChat || g_MainCharInfo.m_pRelation->Am_I_InDan())
	{
		sVisibleCharLIST::iterator it2 = m_VisibleXiahCharObjectList.begin();

		// 보이는 캐릭터 리스트
		for(; it2 != m_VisibleXiahCharObjectList.end(); ++it2)
		{
			XiahObject::CXiahObject *pXiahObject = *it2;

			if(!pXiahObject)
				continue;

			XiahObject::CXiahObject_Basic *pObjectBasic = pXiahObject->m_pObject;
			if(!pObjectBasic)
				continue;

			CXiahCharObject *pXiahCharObject = reinterpret_cast<CXiahCharObject*>(pObjectBasic);
			if(!pXiahCharObject)
				continue;
			
			if(g_info.m_bShowNickname)
			{
				// 문파마크
				if(pXiahCharObject->m_dwMunpaID && pXiahCharObject->m_dwMunpaMarkID)
				{
					g_MunpaMark.RenderMark(pXiahCharObject->m_dwMunpaMarkID,
											pXiahCharObject->m_rcObjectScreenPos.left-17,
											pXiahCharObject->m_rcObjectScreenPos.top);				    
				}

				// 환생 마크
				if(pXiahCharObject->m_bRebirth)
				{
					int nX = pXiahCharObject->m_rcObjectScreenPos.left - 17;
					int nY = pXiahCharObject->m_rcObjectScreenPos.top;

					if(pXiahCharObject->m_dwMunpaID)
						nY += 16;

					g_RebirthMark.RenderMark(pXiahCharObject->m_bRebirth, nX, nY);
				}	
				//HT_1023 : 운영자 마크 추가
				if(pXiahCharObject->m_bGameMasterMark == 1)
				{
					sSize NameSize = pXiahCharObject->m_tObjectName.GetSize();
					
					int nX = pXiahCharObject->m_rcObjectScreenPos.left + 1 + NameSize.cx;
					int nY = pXiahCharObject->m_rcObjectScreenPos.top - 2;
					
					g_RebirthMark.RenderMark(7, nX, nY);
				}			
			}			

			// 채팅 보이는것 처리
			if(g_info.m_bHideChat)
			{
				pXiahCharObject->ShowChatBox();
			}

			// 단 가입시
			if(g_MainCharInfo.m_pRelation->Am_I_InDan())
			{
				sDanInfo* pDanInfo = g_MainCharInfo.m_pRelation->FindDanInfoByID(pXiahObject->m_dwServerID);

				if(pDanInfo)
				{
					// HP 게이지
					if( g_pCurrentCamera)
					{
						Vector3 scPos = g_pCurrentCamera->WorldToScreen( pXiahCharObject->m_Position + Vector3( 0, pXiahCharObject->m_LocalBound.m_vMax.y + 1, 0));

						pXiahCharObject->m_rcObjectScreenPos.left = scPos.x;
						pXiahCharObject->m_rcObjectScreenPos.top = scPos.y;
					}

					RenderEnergyGauge( pXiahCharObject->m_rcObjectScreenPos.left - 40, pXiahCharObject->m_rcObjectScreenPos.top - 10, 80, pDanInfo->m_dwCurHp,  pDanInfo->m_dwMaxHp, D3DCOLOR_XRGB(0, 255, 255), D3DCOLOR_XRGB(0, 0, 0), 5);
				}
			} // if(g_MainCharInfo.m_pRelation->Am_I_InDan())
		} // for(; it2 != m_VisibleXiahCharObjectList.end(); ++it2)
	}

	g_pDirect3DDevice->SetSamplerState( 0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
	g_pDirect3DDevice->SetSamplerState( 0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
	g_pDirect3DDevice->SetRenderState( D3DRS_ZWRITEENABLE, TRUE);
	g_pDirect3DDevice->SetRenderState( D3DRS_FOGENABLE, TRUE);

	return TRUE;
}

BOOL CXiahGame_Main::MoveOnlyCameraForTest()
{
	if( XiahInput::g_bLButtonDown )
	{
		Vector3 vTarget;
		BOOL bOK = XiahMap::g_XiahMap.GetPickPosition(vTarget);
		if( bOK )
		{
			g_XiahCamera.m_vNewAt = vTarget;
			g_XiahCamera.m_vNewAt.y += m_fOnlyCameraMoveY;
			g_XiahCamera.m_bNeedUpdate = TRUE;
		}
	}

	if( GetAsyncKeyState( VK_PRIOR ) < 0 )
	{
		if( m_fOnlyCameraMoveY < 10 )
		{
			m_fOnlyCameraMoveY += 0.2f;
			g_XiahCamera.m_vNewAt.y += 0.2f;
		}
		g_XiahCamera.m_bNeedUpdate = TRUE;
	}

	if( GetAsyncKeyState( VK_NEXT ) < 0 )
	{
		if( m_fOnlyCameraMoveY > 1 )
		{
			m_fOnlyCameraMoveY -= 0.2f;
			g_XiahCamera.m_vNewAt.y -= 0.2f;
		}
		g_XiahCamera.m_bNeedUpdate = TRUE;
	}

	return TRUE;
}
