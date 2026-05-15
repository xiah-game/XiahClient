#include "precompile.h"
#include "AppData.h"
#include "frameDefine.h"
#include "resource.h"
#include "XiahNetworkHandler.h"
#include "XiahSocket.h"

#include "XiahGameMain.h"
#include "XiahEnvInfo.h"
#include "XiahCamera.h"
#include "XiahMap.h"
#include "XiahGame_StepObject.h"
#include "XiahObject.h"
#include "XiahCursor.h"
#include "InterfaceDefine.h"
#include "InterfaceHandler.h"
#include "Fade.h"

#include "XiahGame_Minimap.h"
#include "XiahGameObject.h"
#include "XiahGame_BGM.h"
#include "XiahGameStartLoad.h"

#include "XiahGame_Handler_Sender.h"
//#include "markcube.h"


CharacterInfo					g_MainCharInfo;
CUIManager						*g_pUIManager = NULL;
XiahGameEngine::Map::CMapDecal	g_PickCursor;
BOOL							g_bScreenShot = FALSE;
sString							g_ServerName;	// 서버의 이름.

BOOL	g_XiahGameStarted;
float	g_fix;
long	g_quickslot = 0;		// PAD때문에 넣은 quick slot 번호

extern BOOL bAutoNavigation;
extern BOOL bAutoAttack;
extern BOOL bAutoNormalAttack;
extern BOOL ProcessAutoNavigation(int mode);
extern DWORD g_dwSelectMugongID;

BOOL InitXiahGame()
{
	// 여기서는 적당히 세팅하자.
	//g_fix = 1.94f;
	g_fix = 4.0f;
	g_XiahGameStarted = FALSE;

	g_XiahEnvInfo.m_FogColor = D3DCOLOR_XRGB( 0, 0, 0 );

	Fade::CreateFade();
	Fade::SetFade( 0);

	SET_GAMESTEP( GAMESTEP_START_LOADING);

	// 이제 로딩은 이녀석이 다한다.
	if(!g_StartLoad->Init())
	{
		DBG_LogFile( _T("InitXiahGame 초기화 실패"));
	} 
	
	
	if(!XiahObject::g_XiahCharPool.Create(100))
		return FALSE;

	if(!XiahObject::g_XiahNpcPool.Create(50))
		return FALSE;

	//YS_0812 : BUGFIX
	if(!XiahObject::g_XiahPetPool.Create(30))
		return FALSE;				

	return TRUE;
}
static DWORD StartTime = 0;
//BOOL  bLog = FALSE;

#define START_PERFORMANCE	\
	StartTime = timeGetTime();

#define CHECK_PERFORMANCE_CLOSE(a)	\
	StartTime = timeGetTime() - StartTime;\
	if(StartTime > 60000)\
	{\
		DBG_LogFile_CloseXiah("%s %d", a,StartTime );\
	}\

BOOL CloseXiahGame()
{
	if( g_MainCharInfo.m_pPcSackMine && g_MainCharInfo.m_pPcSackMine->IsShow())
		SendCS_EC_TRADEITEM_REQ( 9, g_MainCharInfo.m_dwAskID);

	if((g_GameWork.m_GameStep_0 != GAMESTEP_LOGIN) && (g_GameWork.m_GameStep_0 != GAMESTEP_START_LOADING))
		SendCS_NV_ENDGAME_REQ();

	//START_PERFORMANCE;
	XiahObject::g_XiahObjectManager.Release();
	CHECK_PERFORMANCE_CLOSE("XiahObject::g_XiahObjectManager");

	START_PERFORMANCE;
	XiahMap::g_XiahMap.ReleaseMap();
	CHECK_PERFORMANCE_CLOSE("XiahMap::g_XiahMap");

	XiahNetwork::DisconnectFromServer();
	CHECK_PERFORMANCE_CLOSE("DisconnectFromServer");


	XiahNetwork::UninitializeNetworkHandler();
	CHECK_PERFORMANCE_CLOSE("UninitializeNetworkHandler");

	// 문파마크 클리어
	//UnInitMupaMark();

	ReleaseGameStepObject();
	CHECK_PERFORMANCE_CLOSE("ReleaseGameStepObject");

	Fade::DestroyFade();
	CHECK_PERFORMANCE_CLOSE("DestroyFade");

	if( g_pUIManager)
	{
		g_pUIManager->Destroy();

		delete g_pUIManager;
		g_pUIManager = NULL;
	} // if( g_pUIManager)
	CHECK_PERFORMANCE_CLOSE("g_pUIManager");

	g_MainCharInfo.DeleteAll();
	CHECK_PERFORMANCE_CLOSE("g_MainCharInfo.DeleteAll");

	//g_SkyBox.Release();

	//g_SkyStar.Release();

	g_RainSnow.Release();
	CHECK_PERFORMANCE_CLOSE("g_RainSnow");

	g_HitEffect.Release();
	CHECK_PERFORMANCE_CLOSE("g_HitEffect");

	Map::UninitializeXiahMap();
	CHECK_PERFORMANCE_CLOSE("UninitializeXiahMap");

	g_EffectManager.Release();
	CHECK_PERFORMANCE_CLOSE("g_EffectManager");

	XiahGameEngine::ReleaseCharacterAll();
	CHECK_PERFORMANCE_CLOSE("ReleaseCharacterAll");
	XiahGameEngine::CloseCharacterPakFileAll();
	CHECK_PERFORMANCE_CLOSE("CloseCharacterPakFileAll");

	XiahPak::UninitializeXiahPak();
	CHECK_PERFORMANCE_CLOSE("UninitializeXiahPak");

	ReleaseXiahCursor();	
	CHECK_PERFORMANCE_CLOSE("ReleaseXiahCursor");

	XiahAniType::ReleaseLogicalAnimationType();
	CHECK_PERFORMANCE_CLOSE("ReleaseLogicalAnimationType");

	return TRUE;
}

BOOL LoopXiahGame_BeforeRender()
{
	CXiahGame_StepObject *pObject = g_GameStep[g_GameWork.m_GameStep_0];

	if( pObject == NULL)
	{
		DBG_LogFile( _T("LoopXiahGame_BeforeRender 실패"));
		return FALSE;
	}

	return pObject->Update();
}

BOOL LoopXiahGameFX()
{
	// 사운드 볼륨이 0 이면 SKIP
	if(g_info.m_dwFXVolume == 0) return TRUE;

	if(g_pMainChar)
	{
		CXiahCharObject *pCharObject = (CXiahCharObject *)g_pMainChar->m_pObject;

		if(pCharObject == NULL)
		{
			DBG_LogFile( _T("LoopXiahGameFX 실패"));
			XiahFX::Clear_FX();
			return FALSE;
		}

		for(int k=0; k< XiahFX::m_fx_number;k++)
		{
			float len = pCharObject->GetInteractionDistance(XiahFX::m_fx[k].m_vec);
			len = ::fabs(len) * 2.5;

			if(len > 255.0f)
			{
				//DBG_Put("무시되는사운드:%f",len);
				continue;	// 이건 플레이 할필요가 없다. 너무 멀다
			}
			else
			{
				if(len < 0.0f) len = 0.0f;
			}
			
			long vol = XiahGameEngine::g_VolTbl[g_info.m_dwFXVolume] - (long)len;
			if(vol < 0) vol = 0;

			long pan  = XiahFX::m_fx[k].m_pan;

			XiahFX::Play_FX (k,vol,pan);
		}
	}
	XiahFX::Clear_FX();

	return TRUE;
}


BOOL LoopXiahGame()
{
	CXiahGame_StepObject *pObject = g_GameStep[g_GameWork.m_GameStep_0];

	if( pObject == NULL)
	{
		DBG_LogFile( _T("LoopXiahGame 실패"));
		return FALSE;
	}

	pObject->Render();

	return TRUE;
}

BOOL LoopXiahGamePreShadow()
{
	CXiahGame_StepObject *pObject = g_GameStep[g_GameWork.m_GameStep_0];

	if( pObject == NULL)
	{
		DBG_LogFile( _T(" 실패"));
		return FALSE;
	}

	pObject->Render();

	return TRUE;
}


D3DCOLOR D3DColorFromRGB( COLORREF c)
{
	return D3DCOLOR_XRGB( (GetRValue( c)), (GetGValue( c)), (GetBValue( c)));
}

COLORREF RGBFromD3DColor( D3DCOLOR c)
{
	return RGB( (GetBValue( c)), (GetGValue( c)), (GetRValue( c)));
}

//test
long	 g_nTestColor = 0;
D3DCOLOR g_TestColor[ 8][ 2] = {
	{ D3DCOLOR_XRGB( 130, 130, 130), D3DCOLOR_XRGB( 208, 217, 215)},
	{ D3DCOLOR_XRGB( 219, 202, 179), D3DCOLOR_XRGB( 228, 248, 242)},
	{ D3DCOLOR_XRGB( 219, 202, 179), D3DCOLOR_XRGB( 225, 242, 251)},
	{ D3DCOLOR_XRGB( 255, 255, 255), D3DCOLOR_XRGB( 173, 187, 218)},
	{ D3DCOLOR_XRGB( 255, 255, 255), D3DCOLOR_XRGB( 245, 235, 205)},
	{ D3DCOLOR_XRGB( 205, 156, 124), D3DCOLOR_XRGB( 211, 166, 135)},
	{ D3DCOLOR_XRGB( 163, 136, 122), D3DCOLOR_XRGB( 123, 110,  95)},
	{ D3DCOLOR_XRGB( 120, 120, 120), D3DCOLOR_XRGB(  34,  54,  60)}
};
float	g_TestFogDensity[ 8] ={
	0.009f,
	0.009f,
	0.006f,
	0.002f,
	0.004f,
	0.006f,
	0.006f,
	0.010f
};
//test

//HT_CHEAT : 치트 키
extern BOOL g_bCheat;
extern  BOOL g_bCheatEtc;

////// 임시
void ChangeChatType(void)
{
	g_MainCharInfo.PlayInterfaceSound( ISOUND_SELECT_BUTTON);

	g_pUIManager->SetFocus(MAIN_CHAT);

	if(g_pUIManager->IsFocus(MAIN_CHAT, chat_name_edit))
	{
		g_pUIManager->SetReleaseFocus(MAIN_CHAT, chat_name_edit);
		g_pUIManager->SetFocus(MAIN_CHAT, main_chat_edit);
	}
	else
	{
		g_pUIManager->SetReleaseFocus(MAIN_CHAT, main_chat_edit);
		g_pUIManager->SetFocus(MAIN_CHAT, chat_name_edit);
	}
}


/**
 * 돈 자릿수 처리
 * \param nMoney 돈값
 * \return 처리된 문자열
 */
sString MoneyCommaStr(INT64 nMoney)
{
	sString szText;
	szText.printf( _T("%d"), nMoney);

	TCHAR strMoney[128] ={0,};
	TCHAR *pStrMoney = strMoney;
	unsigned int nStrLen = szText.length();
	unsigned int nBundle = (nStrLen - 1) / 3;
	unsigned int nSurplus = nStrLen - nBundle * 3;

	if(nSurplus > 0)
	{
		strcpy(pStrMoney, &szText.data()[0]);
		pStrMoney += nSurplus;

		strcpy(pStrMoney, ",");
		++pStrMoney;
	}

	for(unsigned int i=0; i < nBundle; ++i)
	{
		strcpy(pStrMoney, &szText.data()[nSurplus + i * 3]);
		pStrMoney+=3;

		strcpy(pStrMoney, ",");
		++pStrMoney;
	}

	--pStrMoney;
	strcpy(pStrMoney, "\0");

	szText = strMoney;

	return szText;
}

/**
 * 맵이름
 * \param dwMapID 
 * \return 
*/
LPCTSTR GetMapName(DWORD dwMapID)
{
	LPCTSTR lpStrName = NULL;

	switch(dwMapID)
	{
	case 1: // 기암 괴석
		lpStrName = IDS_KIAM;			break;
	case 2: // 화산
		lpStrName = IDS_WHASAN;			break;
	case 3: // 빙하
		lpStrName = IDS_BINGHA;			break;
	case 4: // 사막
		lpStrName = IDS_SAMAK;			break;
	case 5: // 늪지대
		lpStrName = IDS_SULWON;			break;
	case 6:	// 초원지대
		lpStrName = IDS_GRASSLAND;		break;
	case 7:	// 고산지대
		lpStrName = IDS_HIGH_REACHES;	break;
	case 9:	// 신강
		lpStrName = IDS_SG;				break;
	case 10: // 문파대전장
		lpStrName = IDS_MUNPADAEJUN;	break;
	case 11: // 던전1
		lpStrName = IDS_DUNGEON_1;		break;
	case 12: // 던전2 (마혈성)
		lpStrName = IDS_DUNGEON_2;		break;
	case 13: // 던전3 (광명전)
		lpStrName = IDS_DUNGEON_3;		break;
	case 14: // 던전4 (천황전)
		lpStrName = IDS_DUNGEON_4;		break;
	case 15: // HO_0727_07 화염곡추가
		lpStrName = IDS_FIRELAND;		break;
	default:
		lpStrName = _T("?");			break;
	}

	return lpStrName;
}

/**
 * 맵단축이름
 * \param dwMapID 
 * \return 
*/
LPCTSTR GetMapSmallName(DWORD dwMapID)
{
	LPCTSTR lpStrName = NULL;

	switch(dwMapID)
	{
	case 1: // 기암 괴석
		lpStrName = IDS_KIAM_2;			break;
	case 2: // 화산
		lpStrName = IDS_WHASAN_2;		break;
	case 3: // 빙하
		lpStrName = IDS_BINGHA_2;		break;
	case 4: // 사막
		lpStrName = IDS_SAMAK_2;		break;
	case 5: // 늪지대
		lpStrName = IDS_SULWON_2;		break;
	case 6:	// 초원지대
		lpStrName = IDS_GRASSLAND_2;	break;
	case 7:	// 고산지대
		lpStrName = IDS_HIGH_REACHES_2;	break;
	case 9:	// 신강
		lpStrName = IDS_SG_2;			break;
	case 10:// 문파대전장
		lpStrName = IDS_MUNPADAEJUN_2;	break;
	case 11:// 던전1
		lpStrName = IDS_DUNGEON_1;		break;
	case 12: // 던전2 (마혈성)
		lpStrName = IDS_DUNGEON_2;		break;
	case 13: // 던전3 (광명전)
		lpStrName = IDS_DUNGEON_3;		break;
	case 14: // 던전4 (천황전)
		lpStrName = IDS_DUNGEON_4;		break;
	case 15: // HO_0727_07 화염곡추가
		lpStrName = IDS_FIRELAND;		break;
	default:
		lpStrName = _T("?");			break;
	}

	return lpStrName;
}

void PickItem()
{
	static DWORD dwTime = 0;
	if(dwTime+1000 <= g_dwCurTime)
	{
		bool bIsItem = false;
		float fDis = 30.0f;
		// 자신 위치
		Vector3 vMainPos = ((CXiahCharObject*)(g_pMainChar->m_pObject))->m_Position;

		// 맵의 모든 오브젝트
		for(XiahObject::CXiahObjectManager::iterator it = XiahObject::g_XiahObjectManager.begin();
			it != XiahObject::g_XiahObjectManager.end();
			++it)
		{
			XiahObject::CXiahObject *pXiahObject = it->second;

			if( NULL == pXiahObject || NULL == pXiahObject->m_pObject )
				continue;

			CXiahCharObject* pCharObject = reinterpret_cast<CXiahCharObject*>(pXiahObject->m_pObject);			
			
			// 아이템 만( 돈 제외 )
			if(NULL != pCharObject && pCharObject->m_bObjType == OBJTYPE_ITEM )
			{
				XiahItem::sItemInfo* pInfo = (XiahItem::sItemInfo*)pCharObject->m_pPrivateData;

				if(pInfo)
				{
					// 돈 제외
				//	if(pInfo->m_wVisualID != 24000)					
				//	{
						bIsItem = true;

						// 자신과의 거리
						float fRange = pCharObject->GetInteractionDistance(vMainPos);

						// 가장 가까운 거리 찾기
					 	if(fRange < fDis)
						{
							fDis = fRange;

							dwSelObjectID = pXiahObject->m_dwServerID;
							dwSelObjectType = pCharObject->m_bObjType;
						}
			//		}
				}				
			}
		}

		if(bIsItem)
		{
			bAutoNavigation = TRUE;
			bAutoAttack = TRUE;
			bAutoNormalAttack = TRUE;
			ProcessAutoNavigation( 0);
		}

		dwTime = g_dwCurTime;
	}	
}

LRESULT ProcessXiahWindowMessage(UINT uMsg,WPARAM wParam,LPARAM lParam)
{
	switch( uMsg)
	{
	case WM_LBUTTONDOWN:
		break;
	case WM_LBUTTONUP:
		break;
	case WM_RBUTTONDOWN:
		break;
	case WM_RBUTTONUP:
		break;
	case WM_MOUSEMOVE:
		break;
	case WM_MOUSEWHEEL:
		{
			if( g_GameWork.m_nNavigationMode != 0)
				break;

			g_XiahCamera.Zoom(((short)HIWORD(wParam)) > 0);
		}
		break;
	case WM_SETCURSOR:
		SetXiahCursor();
		break;
		/////////////////////////////////////////////////////////////////////////////////////////////////////
	case WM_SYSCHAR:
		if(g_MainCharInfo.m_bChatModeAction)
		{
			int scan_code = (lParam >> 16) & 0xFF;

			// 오행 창 단축키
			switch(scan_code)
			{
			case 0x2E://'C':
			case 0x16://'U'
			case 0x25://'K':
			case 0x26://'L':
			case 0x28://'"':
			case 0x27://';'
			case 0x14://'T': //HO_0410_07 상서령 가이드 업데이트
			case 0x15://'Y': //HT_CHEAT : 펫 상태창 단축키
			case 0x18:// O
			case 0x19://'P':
			case 0x13://'R':
			case 0x22://'G':
			case 0x10://'Q'
				{
					if(g_MainCharInfo.m_bPersonalTradeSell)
						return 0;

					if(g_MainCharInfo.m_pChat->GetChatType() == LARGECHAT)
						g_MainCharInfo.m_pChat->SetChatType( SMALLCHAT);

					// 매품패
					if(g_MainCharInfo.m_pQuickMart)
					{
						return 0;
						//g_MainCharInfo.HideSack(SACKTYPE__QUICKMART, FALSE);						
					}
				}
				break;

			case 0x17://'I':
				{
					if(g_MainCharInfo.m_pChat->GetChatType() == LARGECHAT)
						g_MainCharInfo.m_pChat->SetChatType( SMALLCHAT);

					// 매품패
					if(g_MainCharInfo.m_pQuickMart)
					{
						return 0;
						//g_MainCharInfo.HideSack(SACKTYPE__QUICKMART, FALSE);
					}
				}				
				break;
			case 0x12:	// 기
				{
					if(g_MainCharInfo.m_bStaminaCnt >= 5 && g_MainCharInfo.m_bSpirit == true)
					{
						SendCS_IF_EXECSTAMINA_REQ();
					}
				}
				break;
			}

			switch( scan_code)
			{
			case 0x2E://'C':
				ProcessClickCharInfoButton();
				break;
			case 0x16://'U'
				ProcessClickCollection();
				break;
			case 0x17://'I':
				ProcessClickSackButton();
				break;
			case 0x25://'K':
				ProcessClickMugongButton(1);
				break;
			case 0x26://'L':
				ProcessClickMugongButton(2);
				break;
			case 0x27://';'
				ProcessClickMugongButton(3);
				break;
			case 0x28://'"'
				ProcessClickMugongButton(4);
				break;
			case 0x14://'T':  //HO_0410_07 상서령 가이드 업데이트
				ProcessClickHelperButton();
				break;
			case 0x15://'Y': //HT_CHEAT : 펫 상태창 단축키
				{
					if(g_pUIManager->IsShow(WINDOW_NEW_TAMING))
					{
						g_MainCharInfo.CloseFrame( WINDOW_NEW_TAMING);
					}
					else
					{
						g_MainCharInfo.RefreshPetInfo();
						g_pUIManager->SetPosition(WINDOW_NEW_TAMING, 0, 0);
						g_MainCharInfo.ShowSack( SACKTYPE__PET_EQUIP);
					}
				}
				break;
			case 0x18://O
				ProcessClickOptionButton();
				break;
			case 0x2D://'X'
				ProcessClickCloseButton();
				break;
			case 0x19://'P':
				ProcessClickRelationButton( eDAN);
				break;
			case 0x13://'R':
				ProcessClickRelationButton( eShip);
				break;
			case 0x22://'G':
				ProcessClickRelationButton( eClan);
				break;
			case 0x10://'Q'
				ProcessClickQuestButton();
				break;
			case 0x23://'H':
				ProcessHideMainFrame();
				break;
				// 퀵 슬롯
			case 0x29:
				{
					if(g_MainCharInfo.m_pSlot)
					{
						g_MainCharInfo.m_pSlot->ChangeSlot();
					}
				}
				break;
			case 0x1A://'[':
				{
					// 퀵 슬롯 확장
					if(g_MainCharInfo.m_pSlot)
					{
						BYTE byIndex = g_MainCharInfo.m_pSlot->GetCurrentSlotIndex() - 1;

						if( byIndex < 1)
							byIndex = 1;
						else if( byIndex > 10)
							byIndex = 10;

						g_MainCharInfo.m_pSlot->SelectSlot( byIndex - 1);
					}
				}
				break;
			case 0x1B://']':
				{
					// 퀵 슬롯 확장
					if(g_MainCharInfo.m_pSlot)
					{
						BYTE byIndex = g_MainCharInfo.m_pSlot->GetCurrentSlotIndex() + 1;

						if( byIndex < 1)
							byIndex = 1;
						else if( byIndex > 10)
							byIndex = 10;

						g_MainCharInfo.m_pSlot->SelectSlot( byIndex - 1);
					}
				}
				break;
			case 0x32://'M':
				g_MainCharInfo.ShowMiniMap();
				break;
			case 0x11://'W':			
				if( g_GameWork.m_nNavigationMode == 0)
					g_XiahCamera.Zoom( TRUE);
				break;
			case 0x1F://'S':
				if( g_GameWork.m_nNavigationMode == 0)
					g_XiahCamera.Zoom( FALSE);
				break;
			case 0x01:
				{					
					PickItem();
				}
				break;
			};
		}
		break;
	case WM_KEYDOWN:
		{
			// 게임이 시작되지 않으면 HOT키는 무시다.
			if(!g_XiahGameStarted) break;
			// 각성이 시작되면 HOT키는 무시한다.
//			if(g_MainCharInfo.m_bRebirthItem_Use) break;

			if( g_pUIManager->IsNotice()) 
				//HO_0424_07 단주변경 : 단주 댄轎 수정중 Notice창이 문제가 되어 이창이 있을경우 키입력을 막음
			return 0;

			if(g_MainCharInfo.m_bChatModeAction)	// 채팅모드 고정
			{
				switch( wParam)
				{
				//case VK_F12:	// 도움말
				//	{
				//		if(!(GetAsyncKeyState( VK_SHIFT) < 0))
				//		{
				//			if(g_pUIManager->IsShow(A_HELP))
				//				g_MainCharInfo.CloseFrame(A_HELP);
				//			else
				//				g_MainCharInfo.OpenFrame(A_HELP);
				//		}
				//	}					
				//	break;
				case VK_F9:
					g_bCheat = !g_bCheat;	
					break;

				case VK_F10:
					{
						g_bCheat = !g_bCheat;
						g_bCheatEtc = !g_bCheatEtc;	
					}
					break;
						// 퀵 슬롯 확장
				case VK_OEM_3:	// ` key
					{
						if(!(GetAsyncKeyState(VK_SHIFT) < 0) && g_MainCharInfo.m_pSlot)
						{
							g_MainCharInfo.m_pSlot->ChangeSlot();
						}
					}
					break;

					// 퀵 슬롯 확장 - 수정
					// 퀵슬롯 1~5
				case VK_F1:
				case VK_F2:
				case VK_F3:
				case VK_F4:
				case VK_F5:
					{
						if(!(GetAsyncKeyState(VK_SHIFT) < 0) && g_MainCharInfo.m_pSlot)
						{
							BYTE byIndex = static_cast<BYTE>(wParam) - 112;

							if(g_MainCharInfo.m_pSlot->GetCurrentSlotGroup() >= 1)
								byIndex += 5;

							g_MainCharInfo.m_pSlot->SelectSlot(byIndex);

							g_quickslot = static_cast<long>(byIndex);
						}
					}
					break;

				case VK_ESCAPE:
					{
						CancelInterface();
					}
					break;
				case VK_CONTROL:
					break;

				default:
					{
						if(!g_pUIManager->IsOnEditing())
						{
							g_pUIManager->SetFocus(MAIN_CHAT);
							g_pUIManager->SetFocus(MAIN_CHAT, main_chat_edit);
						}
					}
					break;
				}
				break;
			}

			/////////////////////////////////////////////////////////////////////////////////////////////////////
			if( g_pUIManager && g_pUIManager->IsOnEditing())
			{
				if( wParam == VK_ESCAPE)
				{
					g_pUIManager->SetReleaseFocus(MAIN_CHAT);
					g_pUIManager->SetReleaseFocus(MAIN_CHAT, main_chat_edit);
				}
				break;
			}

			switch( wParam)
			{
#ifdef LIGHTSET
			case VK_F1:
				{
					CHOOSECOLOR cc;                 // common dialog box structure 
					static COLORREF acrCustClr[16]; // array of custom colors 
					static DWORD rgbCurrent;        // initial color selection

					// Initialize CHOOSECOLOR 
					ZeroMemory(&cc, sizeof(CHOOSECOLOR));
					cc.lStructSize = sizeof(CHOOSECOLOR);
					cc.hwndOwner = g_AppData.m_hWnd;
					cc.lpCustColors = (LPDWORD) acrCustClr;

					switch(light_mode)
					{
						case 0 :
							cc.rgbResult = RGBFromD3DColor( g_XiahEnvInfo.m_DiffuseColor);
							break;
						case 1 :
							cc.rgbResult = RGBFromD3DColor( g_XiahEnvInfo.m_FogColor);
							break;
						case 2 :
							cc.rgbResult = RGBFromD3DColor( g_XiahEnvInfo.m_SkyColorBottom);
							break;
						case 3 :
							cc.rgbResult = RGBFromD3DColor( g_XiahEnvInfo.m_SkyColorMiddle);
							break;
						case 4 :
							cc.rgbResult = RGBFromD3DColor( g_XiahEnvInfo.m_SkyColorUp);
							break;
					}
					cc.Flags = CC_FULLOPEN | CC_RGBINIT;
					 
					RedrawWindow( g_AppData.m_hWnd, 0, 0, 0);
					/*
					 if (ChooseColor(&cc)==TRUE)	g_XiahEnvInfo.m_DiffuseColor = D3DColorFromRGB( cc.rgbResult); 
					 */

					ChooseColor(&cc);

					switch(light_mode)
					{
					case 0 :
						lightR = GetRValue(cc.rgbResult);
						lightG = GetGValue(cc.rgbResult);
						lightB = GetBValue(cc.rgbResult);
						break;
					case 1 :
						flightR = GetRValue(cc.rgbResult);
						flightG = GetGValue(cc.rgbResult);
						flightB = GetBValue(cc.rgbResult);
						break;
					case 2 :
						skyR1 = GetRValue(cc.rgbResult);
						skyG1 = GetGValue(cc.rgbResult);
						skyB1 = GetBValue(cc.rgbResult);
						break;
					case 3 :
						skyR2 = GetRValue(cc.rgbResult);
						skyG2 = GetGValue(cc.rgbResult);
						skyB2 = GetBValue(cc.rgbResult);
						break;
					case 4 :
						skyR3 = GetRValue(cc.rgbResult);
						skyG3 = GetGValue(cc.rgbResult);
						skyB3 = GetBValue(cc.rgbResult);
						break;
					}

				}
				break;
			case VK_F2:
				{
					CHOOSECOLOR cc;                 // common dialog box structure 
					static COLORREF acrCustClr[16]; // array of custom colors 
					static DWORD rgbCurrent;        // initial color selection

					// Initialize CHOOSECOLOR 
					ZeroMemory(&cc, sizeof(CHOOSECOLOR));
					cc.lStructSize = sizeof(CHOOSECOLOR);
					cc.hwndOwner = g_AppData.m_hWnd;
					cc.lpCustColors = (LPDWORD) acrCustClr;
					cc.rgbResult = RGBFromD3DColor( g_XiahEnvInfo.m_FogColor);
					cc.Flags = CC_FULLOPEN | CC_RGBINIT;
					 
					RedrawWindow( g_AppData.m_hWnd, 0, 0, 0);
					/*
					if (ChooseColor(&cc)==TRUE) {
						g_XiahEnvInfo.m_FogColor = D3DColorFromRGB( cc.rgbResult); 
						g_SkyBox.SetColor( g_XiahEnvInfo.m_FogColor, g_XiahEnvInfo.m_FogColor, D3DCOLOR_XRGB( 170, 170, 255 ) );
					}
					*/

					ChooseColor(&cc);
					flightR = GetRValue(cc.rgbResult);
					flightG = GetGValue(cc.rgbResult);
					flightB = GetBValue(cc.rgbResult);
				}
				break;
			case VK_F3:
				{
					g_XiahEnvInfo.m_fFogDensity += 0.001f;
					DBG_Put("%f",g_XiahEnvInfo.m_fFogDensity);
				}
				break;
			case  VK_F4:
				{
					g_XiahEnvInfo.m_fFogDensity -= 0.001f;
					DBG_Put("%f",g_XiahEnvInfo.m_fFogDensity);
				}
				break;
			case  VK_F5:
				{
					g_XiahEnvInfo.m_bFog = !g_XiahEnvInfo.m_bFog;
				}
				break;
			case  VK_F6:
				{
					g_XiahEnvInfo.m_fDetailMapRatio += 0.1f;

					if( g_XiahEnvInfo.m_fDetailMapRatio > 1.0f)
						g_XiahEnvInfo.m_fDetailMapRatio = 1.0f;
				}
				break;
			case VK_F7:
				{
					g_XiahEnvInfo.m_fDetailMapRatio -= 0.1f;

					if( g_XiahEnvInfo.m_fDetailMapRatio < 0)
						g_XiahEnvInfo.m_fDetailMapRatio = 0;


				}
				break;
			case VK_F8:
				{
					g_XiahEnvInfo.m_CameraBoundSize += 32;	

					//if( g_XiahEnvInfo.m_CameraBoundSize > XIAH_WIDTH)
					//	g_XiahEnvInfo.m_CameraBoundSize = XIAH_WIDTH;

				}
				break;
			//case VK_F9:
			//	{
			//		g_XiahEnvInfo.m_CameraBoundSize -= 32;
			//		
			//		if( g_XiahEnvInfo.m_CameraBoundSize < 128)
			//			g_XiahEnvInfo.m_CameraBoundSize = 128;

			//	}
			//	break;
			case '9':
				{
					g_nTestColor --;
					if( g_nTestColor < 0)
						g_nTestColor = 0;

					g_XiahEnvInfo.m_DiffuseColor = g_TestColor[ g_nTestColor][ 0];
					g_XiahEnvInfo.m_FogColor	 = g_TestColor[ g_nTestColor][ 1];
					g_XiahEnvInfo.m_fFogDensity  = g_TestFogDensity[ g_nTestColor];
				}
				break;
			case '0':
				{
					g_nTestColor ++;
					if( g_nTestColor > 7)
						g_nTestColor = 7;

					g_XiahEnvInfo.m_DiffuseColor = g_TestColor[ g_nTestColor][ 0];
					g_XiahEnvInfo.m_FogColor	 = g_TestColor[ g_nTestColor][ 1];
					g_XiahEnvInfo.m_fFogDensity  = g_TestFogDensity[ g_nTestColor];
				}
				break;
#else
			case VK_F1:		// 도움말
				if(!(GetAsyncKeyState( VK_SHIFT) < 0))
				{
					if(g_pUIManager->IsShow(A_HELP))
						g_MainCharInfo.CloseFrame(A_HELP);
					else
						g_MainCharInfo.OpenFrame(A_HELP);
				}
				break;
#endif
                // 퀵슬롯 1~5
				// 퀵 슬롯 확장 - 수정
			case 0x31:	// 1
			case 0x32:	// 2
			case 0x33:	// 3
			case 0x34:	// 4
			case 0x35:	// 5
				{
					if(g_MainCharInfo.m_pSlot)
					{
						BYTE byIndex = static_cast<BYTE>(wParam) - 49;

						if(g_MainCharInfo.m_pSlot->GetCurrentSlotGroup() >= 1)
							byIndex += 5;

						g_MainCharInfo.m_pSlot->SelectSlot(byIndex);

						g_quickslot = static_cast<long>(byIndex);
					}
				}
				break;


				// 오행 선택
			case '6':	// 화
				SendCS_IF_CHANGEFIVEELM_REQ(1);
				break;
			case '7':	// 수
				SendCS_IF_CHANGEFIVEELM_REQ(2);
				break;
			case '8':	// 목
				SendCS_IF_CHANGEFIVEELM_REQ(3);
				break;
			case '9':	// 금
				SendCS_IF_CHANGEFIVEELM_REQ(4);
				break;
			case '0':	// 토
				SendCS_IF_CHANGEFIVEELM_REQ(5);
				break;

			case VK_SCROLL:	// 스크린 샷
				g_bScreenShot = TRUE;
			//	g_MainCharInfo.m_bCheat = !g_MainCharInfo.m_bCheat;
				break;

			case VK_F9:
				g_bCheat = !g_bCheat;	
				break;

			case VK_F11://VK_F10
				{
					WORD wPosX = 0;
					WORD wPosY = 0;

					CXiahCharObject *pCharObject = (CXiahCharObject*)g_pMainChar->m_pObject;
					pCharObject->GetPosition( wPosX, wPosY);

					g_MainCharInfo.m_wPosX = wPosX;
					g_MainCharInfo.m_wPosY = wPosY;

					if(g_bCheat && !g_bCheatEtc)
					{
						g_bCheatEtc = !g_bCheatEtc;	
					}
					else
					{
						g_bCheat = !g_bCheat;
						g_bCheatEtc = !g_bCheatEtc;	
					}
				}
				break;

			case VK_ESCAPE:
				{
					CancelInterface();

					if(g_MainCharInfo.m_bPersonalTradeSell)
					{
						g_pUIManager->SetPosition(WINDOW_PC_STORE, WINDOW_FIRST_XPOS, 0);

						if(g_MainCharInfo.m_pPersonalTradeSet)
							g_MainCharInfo.m_pPersonalTradeSet->ShowSack();
					}
				}				
				break;

			case VK_RETURN:
				ProcessFocusOnChat();
				break;

			case VK_TAB:
				{
					if( g_MainCharInfo.m_bShowMiniMap)
						g_MainCharInfo.m_bChangMinimap = TRUE;
				}				
				break;

			case VK_MENU:
				g_MainCharInfo.m_bShowCharNames = !g_MainCharInfo.m_bShowCharNames;
				break;

			case 0x45:	// 기
				{
					if(g_MainCharInfo.m_bStaminaCnt >= 5 && g_MainCharInfo.m_bSpirit == true)
					{
						SendCS_IF_EXECSTAMINA_REQ();
					}
				}
				break;
			case VK_SPACE:
				{
 					PickItem();
				}
				break;
				//HT_CHEAT ; 와이어 화면 보이기
			case VK_DELETE:
				{
					g_AppData.m_bWireframe = !g_AppData.m_bWireframe;
				}
				break;
			case VK_NUMPAD0: //기본 기능만 되게 하기 위해서 (줍기, 수리, 팔기)
				{
					g_MainCharInfo.m_bCheat = !g_MainCharInfo.m_bCheat;
				}
			}


			int scan_code = (lParam >> 16) & 0xFF;

			switch( scan_code)
			{
			case 0x2E://'C':
			case 0x16://'U'
			case 0x25://'K':
			case 0x26://'L':
			case 0x28://'"':
			case 0x27://';'
			case 0x14://'T': //HO_0410_07 상서령 가이드 업데이트
			case 0x15://'Y': //HT_CHEAT : 펫 상태창 단축키
			case 0x18:// O
			case 0x19://'P':
			case 0x13://'R':
			case 0x22://'G':
			case 0x10://'Q'
				{
					if(g_MainCharInfo.m_bPersonalTradeSell)
						return 0;

					if(g_MainCharInfo.m_pChat->GetChatType() == LARGECHAT)
					{
						g_MainCharInfo.m_pChat->SetChatType( SMALLCHAT);
					}

					// 매품패
					if(g_MainCharInfo.m_pQuickMart)
					{
						return 0;
						//g_MainCharInfo.HideSack(SACKTYPE__QUICKMART, FALSE);						
					}
				}break;

			case 0x17://'I':
				{
					if(g_MainCharInfo.m_pChat->GetChatType() == LARGECHAT)
					{
						g_MainCharInfo.m_pChat->SetChatType( SMALLCHAT);
					}

					// 매품패
					if(g_MainCharInfo.m_pQuickMart)
					{
						return 0;
						//g_MainCharInfo.HideSack(SACKTYPE__QUICKMART, FALSE);						
					}
				}				
				break;
			}

			switch( scan_code)
			{

			case 0x4E : //'+'
					g_fix+=0.01f;
				break;
			case 0x0C : // '-'
				
				SendCS_IF_ENDFIVEELM_REQ(); //HT_0720 : 오행 개선 사항
				//if(g_fix>0.0f) g_fix-=0.01f;
				break;
				
#if 0
			case 0x4E : //'+'
				if(Minimap::g_ZoomScale < 3.0f) Minimap::g_ZoomScale+=0.002f;
				break;

			case 0x0C : // '-'
				if(Minimap::g_ZoomScale>0.0f) Minimap::g_ZoomScale-=0.02f;
				break;
#endif
			case 0x2E://'C':
				ProcessClickCharInfoButton();
				break;
			case 0x16://'U'
				ProcessClickCollection();
				break;
			case 0x17://'I':
				ProcessClickSackButton();
				break;
			case 0x25://'K':
				ProcessClickMugongButton( 1);
				break;
			case 0x26://'L':
				ProcessClickMugongButton( 2);
				break;
			case 0x27://';'
				ProcessClickMugongButton(3);
				break;
			case 0x28://'"'
				ProcessClickMugongButton(4);
				break;
			case 0x14://'T':  //HO_0410_07 상서령 가이드 업데이트
				ProcessClickHelperButton();
				break;
				
			//HT_CHEAT : 펫 상태창 단축키
			case 0x15://'Y': 
				{
					if(g_pUIManager->IsShow(WINDOW_NEW_TAMING))
					{
						g_MainCharInfo.CloseFrame( WINDOW_NEW_TAMING);
					}
					else
					{
						g_MainCharInfo.RefreshPetInfo();
						g_pUIManager->SetPosition(WINDOW_NEW_TAMING, 0, 0);
						g_MainCharInfo.ShowSack( SACKTYPE__PET_EQUIP);
					}
				}
				break;
			//HT_CHEAT : 범위 무공 
			case 0x33://'<': 
				{
					if(g_bCheatEtc && g_MainCharInfo.m_byCheatTime < 20)
					{
						g_MainCharInfo.m_byCheatTime++;
					}
				}
				break;
			case 0x34://'>': 
				{
					if(g_bCheatEtc && g_MainCharInfo.m_byCheatTime > 1)
					{
						g_MainCharInfo.m_byCheatTime--;
					}
				}
				break;
			case 0x18://O
				ProcessClickOptionButton();
				break;
			case 0x2D: // 'X'
				ProcessClickCloseButton();
				break;
			case 0x19://'P':
				ProcessClickRelationButton( eDAN);
				break;
			case 0x13://'R':
				ProcessClickRelationButton( eShip);
				break;
			case 0x22://'G':
				ProcessClickRelationButton( eClan);
				break;
			case 0x10://'Q'
				ProcessClickQuestButton();
				break;
			case 0x23://'H':
				ProcessHideMainFrame();
				break;
				// 퀵 슬롯
			case 0x29:
				{
					if(g_MainCharInfo.m_pSlot)
					{
						g_MainCharInfo.m_pSlot->ChangeSlot();
					}
				}
				break;
			case 0x1A://'[':
				{
					// 퀵 슬롯 확장
					if(g_MainCharInfo.m_pSlot)
					{
						BYTE byIndex = g_MainCharInfo.m_pSlot->GetCurrentSlotIndex() - 1;

						if( byIndex < 1)
							byIndex = 1;
						else if( byIndex > 10)
							byIndex = 10;

						g_MainCharInfo.m_pSlot->SelectSlot( byIndex - 1);
					}
				}
				break;
			case 0x1B://']':
				{
					// 퀵 슬롯 확장
					if(g_MainCharInfo.m_pSlot)
					{
						BYTE byIndex = g_MainCharInfo.m_pSlot->GetCurrentSlotIndex() + 1;

						if( byIndex < 1)
							byIndex = 1;
						else if( byIndex > 10)
							byIndex = 10;

						g_MainCharInfo.m_pSlot->SelectSlot( byIndex - 1);
					}
				}
				break;
			case 0x32://'M':
				g_MainCharInfo.ShowMiniMap();
				break;
			case 0x11://'W':			
				if( g_GameWork.m_nNavigationMode == 0)
					g_XiahCamera.Zoom( TRUE);
				break;
			case 0x1F://'S':
				if( g_GameWork.m_nNavigationMode == 0)
					g_XiahCamera.Zoom( FALSE);
				break;	
			};
		}
		break;
	};

	return 0;
}

bool ManageExtraItemEffect()
{
	CRes_Character* pChar = GetCharacter(1018);

	if(pChar == NULL)
		return false;

	XiahGameEngine::CRes_Character::MESHLIST::iterator iter = pChar->MeshList.begin();
	for(; iter != pChar->MeshList.end(); ++iter)
	{
		Res_Mesh* pResMesh = iter->second;

		if(pResMesh)
		{
			for(int i=0; i < pResMesh->effect_count; ++i)
			{
				_EFFECT* pEffect = g_EffectManager.GetEffect(pResMesh->effect_ptr[i].nEffectID);

				if(pEffect != NULL)
				{
					LPCTSTR lpEffectName = pEffect->m_EffectName.data();

					// 각성 아이템
					if(_tcscmp(lpEffectName, _T("기린석발광")) == 0)
						g_EffectManager.SetAppearEffect( eRebirthItem1, pEffect);
					else if(_tcscmp(lpEffectName, _T("봉황석발광")) == 0)
						g_EffectManager.SetAppearEffect( eRebirthItem2, pEffect);
					else if(_tcscmp(lpEffectName, _T("현무석발광")) == 0)
						g_EffectManager.SetAppearEffect( eRebirthItem3, pEffect);
					else if(_tcscmp(lpEffectName, _T("청룡석발광")) == 0)
						g_EffectManager.SetAppearEffect( eRebirthItem4, pEffect);
					else if(_tcscmp(lpEffectName, _T("건곤석발광")) == 0)
						g_EffectManager.SetAppearEffect( eRebirthItem5, pEffect);
					else if(_tcscmp(lpEffectName, _T("음양석발광")) == 0)
						g_EffectManager.SetAppearEffect( eRebirthItem6, pEffect);
				}
			}
		}	    
	}

	return true;
}

bool ManageExtraEffectEtc()
{
	// [12/13/2004] DB 날라가서 이렇게 한다
	CRes_Character* pChar = GetCharacter(1083);

	if( pChar == NULL )
		return false;

	XiahGameEngine::CRes_Character::ANIMATIONLIST::iterator ait = pChar->AnimationList.find(800);

	if( ait == pChar->AnimationList.end() )
		return false;
	else
	{
		Res_Animation* pResAni = ait->second;

		if(pResAni == NULL)
		{
			DBG_LogFile( _T("ManageExtraEffect 실패"));
			return false;
		}

		for(int i=0; i < pResAni->effect_count; ++i)
		{
			_EFFECT* pEffect = g_EffectManager.GetEffect( pResAni->effect_ptr[i].nEffectID );

			if( pEffect != NULL )
			{
				LPCTSTR lpEffectName = pEffect->m_EffectName.data();

				int nX = pResAni->effect_ptr[i].nPosX;
				int nY = pResAni->effect_ptr[i].nPosY;
				int nZ = pResAni->effect_ptr[i].nPosZ;

				// 이름에 맞는 이펙트를 연결시켜준다.

				// 오행 시전
				if(_tcscmp(lpEffectName, _T("오행시전_화_gr")) == 0)
					g_EffectManager.SetOutGongPersistEffect(eFEPrepareFire, pEffect, nX, nY, nZ );
				else if(_tcscmp(lpEffectName, _T("오행시전_수_gr")) == 0)
					g_EffectManager.SetOutGongPersistEffect(eFEPrepareWater, pEffect, nX, nY, nZ );
				else if(_tcscmp(lpEffectName, _T("오행시전_목_gr")) == 0)
					g_EffectManager.SetOutGongPersistEffect(eFEPrepareTree, pEffect, nX, nY, nZ );
				else if(_tcscmp(lpEffectName, _T("오행시전_금_gr")) == 0)
					g_EffectManager.SetOutGongPersistEffect(eFEPrepareMetal, pEffect, nX, nY, nZ );
				else if(_tcscmp(lpEffectName, _T("오행시전_토_gr")) == 0)
					g_EffectManager.SetOutGongPersistEffect(eFEPrepareEarth, pEffect, nX, nY, nZ );
				// 오행 기본
				else if(_tcscmp(lpEffectName, _T("오행_기본_화")) == 0)
					g_EffectManager.SetOutGongPersistEffect(eFEFire, pEffect, nX, nY, nZ );
				else if(_tcscmp(lpEffectName, _T("오행_기본_수")) == 0)
					g_EffectManager.SetOutGongPersistEffect(eFEWater, pEffect, nX, nY, nZ );
				else if(_tcscmp(lpEffectName, _T("오행_기본_목")) == 0)
					g_EffectManager.SetOutGongPersistEffect(eFETree, pEffect, nX, nY, nZ );
				else if(_tcscmp(lpEffectName, _T("오행_기본_금")) == 0)
					g_EffectManager.SetOutGongPersistEffect(eFEMetal, pEffect, nX, nY, nZ );
				else if(_tcscmp(lpEffectName, _T("오행_기본_토")) == 0)
					g_EffectManager.SetOutGongPersistEffect(eFEEarth, pEffect, nX, nY, nZ );
				// 오행 초
				else if(_tcscmp(lpEffectName, _T("오행_유지_초_화")) == 0)
					g_EffectManager.SetOutGongPersistEffect(eFEFire1, pEffect, nX, nY, nZ );
				else if(_tcscmp(lpEffectName, _T("오행_유지_초_수")) == 0)
					g_EffectManager.SetOutGongPersistEffect(eFEWater1, pEffect, nX, nY, nZ );
				else if(_tcscmp(lpEffectName, _T("오행_유지_초_목")) == 0)
					g_EffectManager.SetOutGongPersistEffect(eFETree1, pEffect, nX, nY, nZ );
				else if(_tcscmp(lpEffectName, _T("오행_유지_초_금")) == 0)
					g_EffectManager.SetOutGongPersistEffect(eFEMetal1, pEffect, nX, nY, nZ );
				else if(_tcscmp(lpEffectName, _T("오행_유지_초_토")) == 0)
					g_EffectManager.SetOutGongPersistEffect(eFEEarth1, pEffect, nX, nY, nZ );
				// 오행 중
				else if(_tcscmp(lpEffectName, _T("오행_유지_중_화")) == 0)
					g_EffectManager.SetOutGongPersistEffect(eFEFire2, pEffect, nX, nY, nZ );
				else if(_tcscmp(lpEffectName, _T("오행_유지_중_수")) == 0)
					g_EffectManager.SetOutGongPersistEffect(eFEWater2, pEffect, nX, nY, nZ );
				else if(_tcscmp(lpEffectName, _T("오행_유지_중_목")) == 0)
					g_EffectManager.SetOutGongPersistEffect(eFETree2, pEffect, nX, nY, nZ );
				else if(_tcscmp(lpEffectName, _T("오행_유지_중_금")) == 0)
					g_EffectManager.SetOutGongPersistEffect(eFEMetal2, pEffect, nX, nY, nZ );
				else if(_tcscmp(lpEffectName, _T("오행_유지_중_토")) == 0)
					g_EffectManager.SetOutGongPersistEffect(eFEEarth2, pEffect, nX, nY, nZ );
				// 오행 고
				else if(_tcscmp(lpEffectName, _T("오행_유지_고_화")) == 0)
					g_EffectManager.SetOutGongPersistEffect(eFEFire3, pEffect, nX, nY, nZ );
				else if(_tcscmp(lpEffectName, _T("오행_유지_고_수")) == 0)
					g_EffectManager.SetOutGongPersistEffect(eFEWater3, pEffect, nX, nY, nZ );
				else if(_tcscmp(lpEffectName, _T("오행_유지_고_목")) == 0)
					g_EffectManager.SetOutGongPersistEffect(eFETree3, pEffect, nX, nY, nZ );
				else if(_tcscmp(lpEffectName, _T("오행_유지_고_금")) == 0)
					g_EffectManager.SetOutGongPersistEffect(eFEMetal3, pEffect, nX, nY, nZ );
				else if(_tcscmp(lpEffectName, _T("오행_유지_고_토")) == 0)
					g_EffectManager.SetOutGongPersistEffect(eFEEarth3, pEffect, nX, nY, nZ );
				// 야차 외공
				else if(_tcscmp( lpEffectName, _T("만독불진_지속2")) == 0)
					g_EffectManager.SetOutGongPersistEffect(eMandokbuljin, pEffect, nX, nY, nZ );
				// 성인 서버용 타격
				else if(_tcscmp(lpEffectName, _T("피튀김1")) == 0)
					g_EffectManager.SetHitEffect(eAdultAttack1, pEffect );
				else if(_tcscmp(lpEffectName, _T("피튀김2")) == 0)
					g_EffectManager.SetHitEffect(eAdultAttack2, pEffect );
				else if(_tcscmp(lpEffectName, _T("성인_타격")) == 0)
					g_EffectManager.SetHitEffect(eAdultAttack3, pEffect );
				// 오행 몬스터 타격
				else if(_tcscmp(lpEffectName, _T("염시_타격")) == 0)
					g_EffectManager.SetHitEffect(eFireMonster1, pEffect );
				else if(_tcscmp(lpEffectName, _T("촉음_타격")) == 0)
					g_EffectManager.SetHitEffect(eWaterMonster1, pEffect );
				else if(_tcscmp(lpEffectName, _T("원신수_타격")) == 0)
					g_EffectManager.SetHitEffect(eTreeMonster1, pEffect );
				else if(_tcscmp(lpEffectName, _T("당충_타격")) == 0)
					g_EffectManager.SetHitEffect(eMetalMonster1, pEffect );
				else if(_tcscmp(lpEffectName, _T("비강_타격")) == 0)
					g_EffectManager.SetHitEffect(eEarthMonster1, pEffect );
				// 설승단약
				else if(_tcscmp(lpEffectName, _T("설승단약_시전")) == 0)
					g_EffectManager.SetOutGongPersistEffect(ePotionBegine, pEffect, nX, nY, nZ );				
				else if(_tcscmp(lpEffectName, _T("설승단약_지속")) == 0)
					g_EffectManager.SetOutGongPersistEffect(ePotion, pEffect, nX, nY, nZ );
				// 기
				else if(_tcscmp(lpEffectName, _T("기 발동")) == 0)
					g_EffectManager.SetOutGongPersistEffect(eSpiritBegine, pEffect, nX, nY, nZ );
				else if(_tcscmp(lpEffectName, _T("기_03")) == 0)
					g_EffectManager.SetOutGongPersistEffect(eSpirit, pEffect, nX, nY, nZ );
			}
		}
	}

	return ManageExtraItemEffect();
}

// 앞으로 타 프로젝트를 할때는 이런 방식으로 절대로 하지 말길
// 초기에 누가 했는지는 모르겠으나... 정말로 노가다 예술이다.
// 작업 과정의 복잡성와 성능 저하, 버그를 유발한다.

BOOL ManageExtraEffect()
{
	// 하드 코딩의 결정판!
	// Dummy 캐릭터인 검영의 800번째 애니메이션을 접근한다.
	CRes_Character* pChar = GetCharacter( 790 );

	if( pChar == NULL )
		return false;

	XiahGameEngine::CRes_Character::ANIMATIONLIST::iterator ait = pChar->AnimationList.find( 800 );

	if( ait == pChar->AnimationList.end() )
		return false;
	else
	{
		Res_Animation* pResAni = ait->second;

		if(pResAni == NULL)
		{
			DBG_LogFile( _T("ManageExtraEffect 실패"));
			return false;
		}

		for(int i=0; i < pResAni->effect_count; ++i)
		{
			_EFFECT* pEffect = g_EffectManager.GetEffect( pResAni->effect_ptr[i].nEffectID );

			if( pEffect != NULL )
			{
				LPCTSTR lpEffectName = pEffect->m_EffectName.data();

				int nX = pResAni->effect_ptr[i].nPosX;
				int nY = pResAni->effect_ptr[i].nPosY;
				int nZ = pResAni->effect_ptr[i].nPosZ;

				// 이름에 맞는 이펙트를 연결시켜준다.
				// 타격
				if( _tcscmp( lpEffectName, _T("검영 타격") ) == 0 )
					g_EffectManager.SetHitEffect( eGumYung, pEffect );
				else if( _tcscmp( lpEffectName, _T("연랑 타격") ) == 0 )
					g_EffectManager.SetHitEffect( eYunrang, pEffect );
				else if( _tcscmp( lpEffectName,_T( "야저 타격") ) == 0 )
					g_EffectManager.SetHitEffect( eYager, pEffect );
				else if( _tcscmp( lpEffectName, _T("투우 타격") ) == 0 )
					g_EffectManager.SetHitEffect( eTuo, pEffect );
				else if( _tcscmp( lpEffectName, _T("교 타격") ) == 0 )
					g_EffectManager.SetHitEffect( eGyu, pEffect );
				else if( _tcscmp( lpEffectName, _T("괴인 타격") ) == 0 )
					g_EffectManager.SetHitEffect( eWestGwyin, pEffect );
				else if( _tcscmp( lpEffectName, _T("해골귀 타격") ) == 0 )
					g_EffectManager.SetHitEffect( eHagolgwuy, pEffect );
				else if( _tcscmp( lpEffectName, _T("사갈 타격") ) == 0 )
					g_EffectManager.SetHitEffect( eSagal, pEffect );
				else if( _tcscmp( lpEffectName, _T("요마 타격") ) == 0 )
					g_EffectManager.SetHitEffect( eYuoma, pEffect );
				else if( _tcscmp( lpEffectName, _T("토충 타격") ) == 0 )
					g_EffectManager.SetHitEffect( eToChung, pEffect );
				else if( _tcscmp( lpEffectName, _T("알유 타격") ) == 0 )
					g_EffectManager.SetHitEffect( eAlrue, pEffect );
				else if( _tcscmp( lpEffectName, _T("백랑견 타격") ) == 0 )
					g_EffectManager.SetHitEffect( eBakranggyun, pEffect );
				else if( _tcscmp( lpEffectName, _T("빙조타격") ) == 0 )
					g_EffectManager.SetHitEffect( eBingjo, pEffect );
				else if( _tcscmp( lpEffectName,_T( "금와와 타격") ) == 0 )
					g_EffectManager.SetHitEffect( eGumwawa, pEffect );
				else if( _tcscmp( lpEffectName, _T("금군수장 타격") ) == 0 )
					g_EffectManager.SetHitEffect( eGumgunsujang, pEffect );
				else if( _tcscmp( lpEffectName, _T("마도닌자 타격") ) == 0 )
					g_EffectManager.SetHitEffect( eMadoninja, pEffect );
				else if( _tcscmp( lpEffectName, _T("맹호 타격") ) == 0 )
					g_EffectManager.SetHitEffect( eMangho, pEffect );
				else if( _tcscmp( lpEffectName, _T("광견 타격") ) == 0 )
					g_EffectManager.SetHitEffect( eGwainggyun, pEffect );
				else if( _tcscmp( lpEffectName, _T("건예자타격") ) == 0 )
					g_EffectManager.SetHitEffect( eGunyeja, pEffect );
				else if( _tcscmp( lpEffectName, _T("뇌화 타격") ) == 0 )
					g_EffectManager.SetHitEffect( eNwyhwa, pEffect );
				else if( _tcscmp( lpEffectName, _T("백강잠시타격") ) == 0 )
					g_EffectManager.SetHitEffect( eBackangjamsi, pEffect );
				else if( _tcscmp( lpEffectName, _T("무투 타격") ) == 0 )
					g_EffectManager.SetHitEffect( eMooToo, pEffect );
				else if( _tcscmp( lpEffectName, _T("요희 타격") ) == 0 )
					g_EffectManager.SetHitEffect( eYoihee, pEffect );
				else if( _tcscmp( lpEffectName, _T("흑사봉타격") ) == 0 )
					g_EffectManager.SetHitEffect( eHksabong, pEffect );
				else if( _tcscmp( lpEffectName, _T("저파룡타격") ) == 0 )
					g_EffectManager.SetHitEffect( eJuparyuong, pEffect );
				else if( _tcscmp( lpEffectName, _T("화녀 타격") ) == 0 )
					g_EffectManager.SetHitEffect( eHwanyu, pEffect );
				else if( _tcscmp( lpEffectName, _T("뇌신 타격") ) == 0 )
					g_EffectManager.SetHitEffect( eNwysin, pEffect );
				else if( _tcscmp( lpEffectName, _T("부마도타격") ) == 0 )
					g_EffectManager.SetHitEffect( eBumado, pEffect );
				else if( _tcscmp( lpEffectName, _T("건곤타격") ) == 0 )
					g_EffectManager.SetHitEffect( eGungon, pEffect );
				else if( _tcscmp( lpEffectName, _T("백호 타격") ) == 0 )
					g_EffectManager.SetHitEffect( eBakho, pEffect );
				else if( _tcscmp( lpEffectName, _T("자소타격") ) == 0 )
					g_EffectManager.SetHitEffect( eJaso, pEffect );
				else if( _tcscmp( lpEffectName, _T("관흉인 타격") ) == 0 )
					g_EffectManager.SetHitEffect( eGwanhungin, pEffect );
				else if( _tcscmp( lpEffectName, _T("타격_반탄강기") ) == 0 )
					g_EffectManager.SetHitEffect( eBantankangki_Hit, pEffect );
				else if( _tcscmp( lpEffectName, _T("해골술사타격") ) == 0 )
					g_EffectManager.SetHitEffect( eHaegolSerize, pEffect );
				else if( _tcscmp( lpEffectName, _T("산타관흉타격") ) == 0 )
					g_EffectManager.SetHitEffect( eSantaGwanHung, pEffect );
				else if( _tcscmp( lpEffectName, _T("신조_타격") ) == 0 )
					g_EffectManager.SetHitEffect( eShinjo, pEffect );
				else if( _tcscmp( lpEffectName, _T("야곤_타격") ) == 0 )
					g_EffectManager.SetHitEffect( eYagon, pEffect );
				else if( _tcscmp( lpEffectName, _T("천신술사_타격") ) == 0 )
					g_EffectManager.SetHitEffect( eChunshinsulsa, pEffect );
				else if( _tcscmp( lpEffectName, _T("진모인_타격") ) == 0 )
					g_EffectManager.SetHitEffect( eJinmoin, pEffect );
				else if( _tcscmp( lpEffectName, _T("표_타격") ) == 0 )
					g_EffectManager.SetHitEffect( ePyo, pEffect );
				else if( _tcscmp( lpEffectName, _T("곤룡자_타격") ) == 0 )
					g_EffectManager.SetHitEffect( eGonlyeongja, pEffect );
				else if(_tcscmp(lpEffectName, _T("인면수 타격")) == 0)
					g_EffectManager.SetHitEffect( eTreeMonster, pEffect );
				else if(_tcscmp(lpEffectName, _T("대조귀 타격")) == 0)
					g_EffectManager.SetHitEffect( eMouseMonster, pEffect );
				else if(_tcscmp(lpEffectName, _T("혈기린 타격")) == 0)
					g_EffectManager.SetHitEffect( eFireballTiger, pEffect );
				else if(_tcscmp(lpEffectName, _T("금강동인 타격")) == 0)
					g_EffectManager.SetHitEffect( eMetalMonster, pEffect );
				else if(_tcscmp(lpEffectName, _T("금각거인 타격")) == 0)
					g_EffectManager.SetHitEffect( eArmorGiant, pEffect );
				else if(_tcscmp(lpEffectName, _T("석귀 타격")) == 0)
					g_EffectManager.SetHitEffect( eGolem, pEffect );
				else if( _tcscmp( lpEffectName, _T("야차 타격") ) == 0 )
					g_EffectManager.SetHitEffect( eYacha, pEffect );
				// 레벨 업
				else if( _tcscmp( lpEffectName, _T("Level UP") ) == 0 )
					g_EffectManager.SetLevelUpEffect( LEVELUP_GAPJA, pEffect );
				else if( _tcscmp( lpEffectName, _T("Level UP2") ) == 0 )
					g_EffectManager.SetLevelUpEffect( LEVELUP_TP, pEffect );
				else if( _tcscmp( lpEffectName, _T("수련외공") ) == 0 )
					g_EffectManager.SetLevelUpEffect( LEVELUP_OUTGONG, pEffect );
				else if( _tcscmp( lpEffectName, _T("수련내공") ) == 0 )
					g_EffectManager.SetLevelUpEffect( LEVELUP_INGONG, pEffect );
				// NPC 등장 이펙트
				else if( _tcscmp( lpEffectName, _T("NPC_Spawn_s") ) == 0 )
					g_EffectManager.SetAppearEffect( eSmall, pEffect );
				else if( _tcscmp( lpEffectName, _T("NPC_Spawn_m") ) == 0 )
					g_EffectManager.SetAppearEffect( eMiddle, pEffect );
				else if( _tcscmp( lpEffectName, _T("NPC_Spawn_b") ) == 0 )
					g_EffectManager.SetAppearEffect( eBig, pEffect );
				// 경험치 획득 이펙트
				else if( _tcscmp( lpEffectName, _T("경험치") ) == 0 )
					g_EffectManager.SetExpAcquireEffect( pEffect );
				// 바닥에 있는 아이템 이펙트
				else if( _tcscmp( lpEffectName, _T("바닥아이템_일반") ) == 0 )
					g_EffectManager.SetAppearEffect( eItemGround, pEffect );
				// 텔레보트 이펙트
				else if( _tcscmp( lpEffectName, _T("텔레포트") ) == 0 )
					g_EffectManager.SetAppearEffect( eTeleport, pEffect );
				// 물약 이펙트.
				else if( _tcscmp( lpEffectName, _T("물약_생명") ) == 0 )
					g_EffectManager.SetAppearEffect( eMulYak_HP, pEffect );
				else if( _tcscmp( lpEffectName, _T("물약_내력") ) == 0 )
					g_EffectManager.SetAppearEffect( eMulYak_IP, pEffect );
				else if( _tcscmp( lpEffectName, _T("물약_동시") ) == 0 )
					g_EffectManager.SetAppearEffect( eMulYak_HPIP, pEffect );
				// NPC 죽을때 폭발 이펙트
				else if( _tcscmp( lpEffectName, _T("신조폭파") ) == 0 )
					g_EffectManager.SetAppearEffect( eShinjo_Explode, pEffect );
				// 이벤트 아이템 이펙트
				else if( _tcscmp( lpEffectName, _T("공격계_발동") ) == 0 )
					g_EffectManager.SetAppearEffect( eAttackKindItem_start, pEffect );
				else if( _tcscmp( lpEffectName, _T("성장계_발동") ) == 0 )
					g_EffectManager.SetAppearEffect( eGrowthKindItem_start, pEffect );
				else if( _tcscmp( lpEffectName, _T("몬스터계_발동") ) == 0 )
					g_EffectManager.SetAppearEffect( eMonsterKindItem_start, pEffect );
				else if( _tcscmp( lpEffectName, _T("경제계_발동") ) == 0 )
					g_EffectManager.SetAppearEffect( eEconomiKindItem_start, pEffect );
				else if( _tcscmp( lpEffectName, _T("공격계_유지") ) == 0 )
					g_EffectManager.SetOutGongPersistEffect( eAttackKindItem_keepup, pEffect, nX, nY, nZ );
				else if( _tcscmp( lpEffectName, _T("성장계_유지") ) == 0 )
					g_EffectManager.SetOutGongPersistEffect( eGrowthKindItem_keepup, pEffect, nX, nY, nZ );
				else if( _tcscmp( lpEffectName, _T("몬스터계_유지") ) == 0 )
					g_EffectManager.SetOutGongPersistEffect( eMonsterKindItem_keepup, pEffect, nX, nY, nZ );
				else if( _tcscmp( lpEffectName, _T("경제계_유지") ) == 0 )
					g_EffectManager.SetOutGongPersistEffect( eEconomiKindItem_keepup, pEffect, nX, nY, nZ );
				// 외공 지속 이펙트 및 무공 이펙트.
				// 검영 무공, 외공 지속 이펙트
				else if( _tcscmp( lpEffectName, _T("무수혼지속이펙트") ) == 0 )
					g_EffectManager.SetOutGongPersistEffect( eMusuhon, pEffect, nX, nY, nZ );
				else if( _tcscmp( lpEffectName, _T("폭사혼지속이펙트") ) == 0 )
					g_EffectManager.SetOutGongPersistEffect( ePoksahon, pEffect, nX, nY, nZ );
				else if( _tcscmp( lpEffectName, _T("금강유지") ) == 0 )
					g_EffectManager.SetOutGongPersistEffect( eKuymgangruk, pEffect, nX, nY, nZ );
				else if( _tcscmp( lpEffectName, _T("바닥_일위도강지속2") ) == 0 )
					g_EffectManager.SetOutGongPersistEffect( eIlyuidogang, pEffect, nX, nY, nZ );
				// 연랑 무공.
				else if( _tcscmp( lpEffectName, _T("이광음힐받기") ) == 0 )
					g_EffectManager.SetOutGongPersistEffect( eLeekwangum_heal_recv, pEffect, nX, nY, nZ );
				else if( _tcscmp( lpEffectName, _T("전유음받기") ) == 0 )
					g_EffectManager.SetOutGongPersistEffect( eJunuoum_recv, pEffect, nX, nY, nZ );
				else if( _tcscmp( lpEffectName, _T("연우영받기") ) == 0 )
					g_EffectManager.SetOutGongPersistEffect( eYuenoyueng_recv, pEffect, nX, nY, nZ );
				else if( _tcscmp( lpEffectName, _T("이타생_데미지") ) == 0 )
					g_EffectManager.SetOutGongPersistEffect( eLeetasaeng_damage, pEffect, nX, nY, nZ );
				else if( _tcscmp( lpEffectName, _T("교감수_받기") ) == 0 )
					g_EffectManager.SetOutGongPersistEffect( eKyugamsu_recv, pEffect, nX, nY, nZ );
				else if( _tcscmp( lpEffectName, _T("원기신강받기") ) == 0 )
					g_EffectManager.SetOutGongPersistEffect( eWonkisingang_recv, pEffect, nX, nY, nZ );
				else if( _tcscmp( lpEffectName, _T("미혼술받기") ) == 0 )
					g_EffectManager.SetOutGongPersistEffect( eMihonsul_recv, pEffect, nX, nY, nZ );
				else if( _tcscmp( lpEffectName, _T("원기신강지속이펙트") ) == 0 )
					g_EffectManager.SetOutGongPersistEffect( eWonkisingang, pEffect, nX, nY, nZ );
				else if( _tcscmp( lpEffectName, _T("환수유_출현") ) == 0 )
					g_EffectManager.SetOutGongPersistEffect( eHwansoou_appear, pEffect, nX, nY, nZ );
				else if( _tcscmp( lpEffectName, _T("연우영지속") ) == 0 )
					g_EffectManager.SetOutGongPersistEffect( eYuenoyueng, pEffect, nX, nY, nZ );
				else if( _tcscmp( lpEffectName, _T("교감수_지속") ) == 0 )
					g_EffectManager.SetOutGongPersistEffect( eKyugamsu, pEffect, nX, nY, nZ );
				else if( _tcscmp( lpEffectName, _T("바닥_유수신영지속2") ) == 0 )
					g_EffectManager.SetOutGongPersistEffect( eYuesusinyung, pEffect, nX, nY, nZ );
				// 무투 무공.
				else if( _tcscmp( lpEffectName, _T("지속_반탄강기") ) == 0 )
					g_EffectManager.SetOutGongPersistEffect( eBantankangki, pEffect, nX, nY, nZ );
				else if( _tcscmp( lpEffectName, _T("지속_적운강기") ) == 0 )
					g_EffectManager.SetOutGongPersistEffect( eJukwonkangki, pEffect, nX, nY, nZ );
				else if( _tcscmp( lpEffectName, _T("받기_파천소") ) == 0 )
					g_EffectManager.SetOutGongPersistEffect( ePachunso_recv, pEffect, nX, nY, nZ );
				else if( _tcscmp( lpEffectName, _T("받기_금나수") ) == 0 )
					g_EffectManager.SetOutGongPersistEffect( eKumnasu_recv, pEffect, nX, nY, nZ );
				else if( _tcscmp( lpEffectName, _T("받기_암흑무") ) == 0 )
					g_EffectManager.SetOutGongPersistEffect( eAmhukmu_recv, pEffect, nX, nY, nZ );
				else if( _tcscmp( lpEffectName, _T("받기_탈백인") ) == 0 )
					g_EffectManager.SetOutGongPersistEffect( eTalbacin_recv, pEffect, nX, nY, nZ );
				else if( _tcscmp( lpEffectName, _T("받기_마령각") ) == 0 )
					g_EffectManager.SetOutGongPersistEffect( eMarulkak_recv, pEffect, nX, nY, nZ );
				else if( _tcscmp( lpEffectName, _T("바닥_질풍보지속2") ) == 0 )
					g_EffectManager.SetOutGongPersistEffect( eJilpungbo, pEffect, nX, nY, nZ );
				// 야차 무공
				else if( _tcscmp( lpEffectName, _T("바닥_초상비지속2") ) == 0 )
					g_EffectManager.SetOutGongPersistEffect( eChosangbi, pEffect, nX, nY, nZ );
				else if( _tcscmp( lpEffectName, _T("오독침_지속") ) == 0 )
					g_EffectManager.SetOutGongPersistEffect( eOdokchim, pEffect, nX, nY, nZ );
				else if( _tcscmp( lpEffectName, _T("독무_가루유지") ) == 0 )
					g_EffectManager.SetOutGongPersistEffect( eDokmu, pEffect, nX, nY, nZ );
				else if( _tcscmp( lpEffectName, _T("독혈공_지속") ) == 0 )
					g_EffectManager.SetOutGongPersistEffect( eDokhyulgong, pEffect, nX, nY, nZ );
				else if( _tcscmp( lpEffectName, _T("독내공_지속") ) == 0 )
					g_EffectManager.SetOutGongPersistEffect( eDoknaegong, pEffect, nX, nY, nZ );
				else if( _tcscmp( lpEffectName, _T("쌍도수_지속") ) == 0 )
					g_EffectManager.SetOutGongPersistEffect( eSsangdosu, pEffect, nX, nY, nZ );

				//HT_0523 환생 무공.. '펙'이면 다 '펙'으로 올리지 '팩'은 또 모냐 ㅡㅡ;
				else if( _tcscmp( lpEffectName, _T("화룡대상이펙트") ) == 0 )
					g_EffectManager.SetOutGongPersistEffect( eWha_Dragon, pEffect, nX, nY, nZ );	
				else if( _tcscmp( lpEffectName, _T("빙룡대상이팩트") ) == 0 )
					g_EffectManager.SetOutGongPersistEffect( eBing_Dragon, pEffect, nX, nY, nZ );
				else if( _tcscmp( lpEffectName, _T("독룡대상이펙트_수정") ) == 0 )
					g_EffectManager.SetOutGongPersistEffect( eDok_Dragon, pEffect, nX, nY, nZ );
				else if( _tcscmp( lpEffectName, _T("뇌룡대상이펙트_수정") ) == 0 )
					g_EffectManager.SetOutGongPersistEffect( eNoi_Dragon, pEffect, nX, nY, nZ );
				
				// 환생 특화 무공 지속 이펙트
				else if( _tcscmp( lpEffectName, _T("검영_일기참대상") ) == 0 )
					g_EffectManager.SetHitEffect(eGumyongSpecial, pEffect);
				else if( _tcscmp( lpEffectName, _T("무투_강기포권대상") ) == 0 )
					g_EffectManager.SetHitEffect(eMutuSpecial, pEffect);
				else if( _tcscmp( lpEffectName, _T("연랑_수신기강대상") ) == 0 )
					g_EffectManager.SetOutGongPersistEffect(eYunrangSpecial, pEffect, nX, nY, nZ);

				else if( _tcscmp( lpEffectName, _T("연랑_사장신공유지_수정") ) == 0 )
					g_EffectManager.SetOutGongPersistEffect(eYunSajangsingong, pEffect, nX, nY, nZ);
				else if( _tcscmp( lpEffectName, _T("무투_기흡강기지속") ) == 0 )
					g_EffectManager.SetOutGongPersistEffect(eMuKihubkangki, pEffect, nX, nY, nZ);
				else if( _tcscmp( lpEffectName, _T("야차_은신술유지") ) == 0 )
					g_EffectManager.SetOutGongPersistEffect(eYaEunsinsul, pEffect, nX, nY, nZ);
				else if( _tcscmp( lpEffectName, _T("야차_호정강기유지_수정") ) == 0 )
					g_EffectManager.SetOutGongPersistEffect(eYaHojungkangki, pEffect, nX, nY, nZ);					
			}// if
			else
			{
				DBG_LogFile( _T("ManageExtraEffect 실패"));

//				return false;
			}
		}// for( effect_count )  
	}// if
	
	return ManageExtraEffectEtc();
}
