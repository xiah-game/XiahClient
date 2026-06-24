#include "XiahCheatConfig.h"
/*
	우웩~~~ 떡대 쟁이 코드 됐다~~
*/
#include "XiahGameMain.h"
#include "XiahInput.h"
#include "cjoystic.h"
#include "XiahGame_Pet.h"
#include "InterfaceHandler.h"
#include "AppData.h"
#include "XiahEnvInfo.h"
#include "TargetInfoPanel.h"

#pragma comment(lib,"winmm.lib")

BOOL		bMove = FALSE;
Vector3		vTarget;
DWORD		MoveTime = 0;
DWORD		MoveTime2 = 0;
DWORD		ClickTime = 0;

Vector3		vPos;
float		fMoveLength;
BOOL		bAttackable = FALSE;
BOOL		bMainCharDie;

DWORD		dwSelObjectID		= 0;
DWORD		dwSelObjectType		= 0;

BOOL		bAutoAttack			= FALSE;	// 자동공격
BOOL		bAutoNavigation		= FALSE;	// 오브젝트 따라가기
BOOL		bAutoNormalAttack	= FALSE;	// 자동 공격 플래그가 아니라 버튼을 누르고 있는 동안에 계속 공격하도록 
float		fInteractionRange	= 9;
BOOL		bCursorOnField		= FALSE;
CXiahCharObject* pMouseOnCharObject = NULL;
BYTE			 bMouseOnObjectType = 255;

DWORD		g_dwSelectMugongID = 0;

// 라이트 셋팅용
#ifdef LIGHTSET
// 라이트 테스트용 빼야함
unsigned char lightR = 255;
unsigned char lightG = 255;
unsigned char lightB = 255;

unsigned char flightR = 255;
unsigned char flightG = 255;
unsigned char flightB = 255;

unsigned char skyR1 = 255;
unsigned char skyG1 = 255;
unsigned char skyB1 = 255;

unsigned char skyR2 = 255;
unsigned char skyG2 = 255;
unsigned char skyB2 = 255;

unsigned char skyR3 = 255;
unsigned char skyG3 = 255;
unsigned char skyB3 = 255;


int light_mode = 0;
#endif

// AUTO TARGET용
static BOOL	g_AutoTarget = FALSE;
static XiahObject::CXiahObject* AutoTargetObj = NULL;

// HP 관련하여 진동
static	DWORD	LastRumble = 0;

// 패드로 메뉴열기
static BOOL	PadMenuOpend = FALSE;
static long SelPadMenu = 0;

BOOL		g_bCheat	= FALSE;
BOOL		g_bCheatEtc = FALSE;
DWORD		g_dwCheatTime = 0;

#ifdef _DEBUG_CHEAT

#endif

//HT_CHEAT
DWORD dwAutoTime = 0;

BOOL CheckPortalMove( CXiahCharObject *pMainChar);
void ProcessLbuttonUp();
void ProcessLButtonDown( CXiahCharObject *pMainChar, CXiahCharObject* pMouseOnCharObject);
void ProcessCharIsMoving( CXiahCharObject *pMainChar);
void ChangingMoving(CXiahCharObject *pMainChar, Vector3 vTarget);

void ProcessRButtonDown( CXiahCharObject *pMainChar, BOOL bMouseOnObjectType);
void ProcessAutoAttack( CXiahCharObject *pMainChar);
void ProcessAutoTarget(CXiahCharObject *pMainChar);	// AUTO TARGET
void ProcessQuickSlot();	// PAD용 Quick Slot
void ProcessUseHPMP();		// HP,MP 사용(PAD용)
void ProcessRumble();		// 진동!
void ProcessMenu();			// PAD로 메뉴 호출
void LIghtSetup();

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

BOOL ProcessMainChar()
{

	if(g_MainCharInfo.m_bPersonalTradeSell) // 개인상점 개설중이면 캐릭터 갱신만
	{
		// 케렉터의 상태를 Update
		((CXiahCharObject *)g_pMainChar->m_pObject)->Update(true);

		return true;
	}

	static Vector3 pre_pos;
	// 임시 라이트 셋팅용
#ifdef LIGHTSET
	LIghtSetup();
#endif

	CXiahCharObject *pMainChar = (CXiahCharObject *)g_pMainChar->m_pObject;

	if(!g_pUIManager->IsShow(MAIN_FRAME))
	{
		pMainChar->m_bShowGage = TRUE;
		pMainChar->m_bShowManaGage = TRUE;
	}
	else
	{
		pMainChar->m_bShowGage = FALSE;
		pMainChar->m_bShowManaGage = FALSE;
	}

	// 캐릭터가 죽었는가?
	bMainCharDie = pMainChar->m_nCurMotionType == XiahAniType::eLAT_Die && (pMainChar->m_nCurAniIndex == 0 || Fade::g_bFadeStart);
	bMainCharDie = g_MainCharInfo.m_bMainCharDie;

	if( bMainCharDie )
	{
		dwSelObjectID = 0;
		dwSelObjectType = 0;
		bAutoAttack			= FALSE;
		bAutoNavigation		= FALSE;
		bAutoNormalAttack	= FALSE;
	}

	// 캐릭터가 움직이고 있는중인가?
	bMove = pMainChar->m_bTargetMove 
		    && (pMainChar->m_nCurMotionType == XiahAniType::eLAT_Run || pMainChar->m_nCurMotionType == XiahAniType::eLAT_Mugong)
			&& !bMainCharDie;

	// 선택된 오브젝트
	XiahObject::CXiahObject* pSelObject	= NULL;
	CXiahCharObject* pSelCharObject = NULL;
	
	if( dwSelObjectID != 0)
	{
		pSelObject = XiahObject::g_XiahObjectManager.FindXiahObject( MAKEOBJECTID( 0, dwSelObjectID, dwSelObjectType));
		
		if( pSelObject)
		{
			if( pSelObject->m_pObject->IsA( XiahObject::eXOT_CharObject))
			{
				pSelCharObject = (CXiahCharObject*)pSelObject->m_pObject;
			}
		}
		else
		{
			// 사라져 버렸군 초기화를 확실해 해주자
			dwSelObjectID = 0;
			dwSelObjectType = 0;
			bAutoAttack			= FALSE;
			bAutoNavigation		= FALSE;
			bAutoNormalAttack	= FALSE;
		}
	}

	// Sync target info panel with current selection
	if (g_pTargetInfoPanel)
	{
		if (dwSelObjectID != 0)
		{
			g_pTargetInfoPanel->SetTarget(dwSelObjectID, (BYTE)dwSelObjectType);
		}
		else if (g_pTargetInfoPanel->IsActive())
		{
			g_pTargetInfoPanel->Clear();
		}
	}

	// 마우스 올려진 오브젝트
	if( XiahObject::g_pMouseOnObject != NULL)
	{
		if( XiahObject::g_pMouseOnObject->m_pObject->IsA( XiahObject::eXOT_CharObject))
		{
			pMouseOnCharObject = (CXiahCharObject*)XiahObject::g_pMouseOnObject->m_pObject;
			bMouseOnObjectType = pMouseOnCharObject->m_bObjType;
		}
	}
	else
	{
		pMouseOnCharObject = NULL;
		bMouseOnObjectType = 255;
	}

	// Portal이동 체크
	if(	CheckPortalMove( pMainChar))
		return TRUE;
	
	// 카메라 Update여부
	if( bMove) 
		g_XiahCamera.m_bNeedUpdate = TRUE;

	/***********************************************************
		논리적 동작 단위로 처리되던 코드를 입력단위로 정리하겟다

		무지 헷갈려서 주석을 열심히 달기로 했음
	*************************************************************/

	//////////////////////////////////////////////////////////////////////////////////////////////////////
	// PAD용 Autotarget Update
	//////////////////////////////////////////////////////////////////////////////////////////////////////

	//if(XiahGameEngine::g_cj)
	//{
	//	XiahGameEngine::g_cj->GetJoyState();
	//	ProcessAutoTarget(pMainChar);
	//	ProcessUseHPMP();
	//	ProcessRumble();

	//	ProcessMenu();
	//	ProcessQuickSlot();	// 이건 MENU 다음으로 와야한다. 메뉴에서 슬롯사용 버튼을 막아야하는 경우가 있다.
	//}

	//////////////////////////////////////////////////////////////////////////////////////////////////////
	// mouse action
	//////////////////////////////////////////////////////////////////////////////////////////////////////

	BOOL bCursorOnRepair = FALSE;
		
	// 커서 변경
	if( !g_pUIManager->IsMouseOnFrame())
	{
		if( g_CursorType != eCT_Repair)
		{
			ProcessCursor();
		}
		else if( XiahInput::g_bLButtonUp)
		{
			ChangeXiahCursor( eCT_General);
			bCursorOnRepair = TRUE;		// 고치는중인 아이콘은 바닥에 한번 찍었을때 움직이지 않고 커서만 바꿔주기 위해
		}
	}
	else if( g_CursorType != eCT_General)	//프레임밖에서 커서가 일반커서가 아니다가 재빠르게 프레임으로 위치하면 일반커서로 되돌아가지 아니하므로
	{
		if( g_CursorType != eCT_Repair)
			ChangeXiahCursor( eCT_General);
	}

	bCursorOnField = !g_pUIManager->IsMouseOnFrame() &&					// 마우스가 인터페이스 상에 있는지
					 !g_MainCharInfo.m_pHoldItem->IsHoldingItem() &&	// 아이템 드레깅 중인지
					 !g_pUIManager->IsNotice() &&						// 메세지 박스가 떠 있는지
					 !bCursorOnRepair;	 								// 뭔가를 고치는 중인지

	if( g_MainCharInfo.m_pHoldItem->GetHoldItemItem() &&
		g_MainCharInfo.m_pHoldItem->GetHoldItemItem()->m_bItemType == ITEMTYPE_NPCITEM &&
		g_CursorType == eCT_Menu)
	{
		bCursorOnField = TRUE;
	}

	// LButtonUp.버튼 눌림이 끝났을때, 버튼을 띄었을때
	if( XiahInput::g_bLButtonUp) 
		ProcessLbuttonUp();

	//if((XiahInput::g_bLButtonDown || XiahInput::g_Attack_Button_On) && bCursorOnField && !bMainCharDie && !g_AutoTarget)
	//HT_CHEAT : 게임 패드 삭제
	if((XiahInput::g_bLButtonDown ) && bCursorOnField && !bMainCharDie && !g_AutoTarget)
	{
		g_dwSelectMugongID = 0;

		if(g_IsFocus)
			ProcessLButtonDown( pMainChar, pMouseOnCharObject);
	}


	// LbuttonOn. 마우스를 계속 누르고 있을때는 Interaction을 여러번 시켜준다
	if( XiahInput::g_bLButtonOn && bCursorOnField && bAutoNormalAttack && !bAutoAttack && !bMainCharDie)
		InteractObject( dwSelObjectID, dwSelObjectType, 2);

	// RButtonDown.을 눌렀을때
	//HT_CHEAT : 게임 패드 삭제
	if((XiahInput::g_bRButtonDown) && bCursorOnField && !bMainCharDie)
	{
		if(g_IsFocus)
		{
			if(GUM_ILKICHAM == g_MainCharInfo.m_pSlot->GetActiveSlot())
			{
				if(XiahObject::g_pMouseOnObject)
				{
					XiahObject::g_pMouseOnObjectSave = XiahObject::g_pMouseOnObject;

					dwSelObjectID = XiahObject::g_pMouseOnObject->m_dwServerID;
					dwSelObjectType = pMouseOnCharObject->m_bObjType;

					g_dwSelectMugongID = GUM_ILKICHAM;					

					bAutoNavigation = TRUE;

					ProcessAutoNavigation( 0);
				}
			}
			else
			{
				g_dwSelectMugongID = 0;

				ProcessRButtonDown( pMainChar, bMouseOnObjectType);	
			}			
		}
	}

	//////////////////////////////////////////////////////////////////////////////////////////////////////
	// others need update
	//////////////////////////////////////////////////////////////////////////////////////////////////////

	// 그림자
	if( (g_PickCursor.IsValid() && pMainChar->m_bTargetMove) | g_AutoTarget)
		XiahMap::g_XiahMap.m_pMapRender->AddVisibalMapDecal( &g_PickCursor);

	// 움직이는 도중이라면 서버에 SyncMove를 보내줌
	if( g_dwCurTime - MoveTime > 1400 && bMove && pMainChar->m_nCurMotionType != XiahAniType::eLAT_Mugong)
	{
		ProcessCharIsMoving( pMainChar);
	}
	
	// 자동 이동
	if( bAutoNavigation)
		ProcessAutoNavigation(1);


//HT_CHEAT : 자동 공격
	// 挂机中心点初始化：在所有挂机逻辑之前记录，确保是玩家点击“开始挂机”时的真实位置
	if (g_bCheat && g_bUseHomePoint && g_wHomeX == 0 && g_wHomeY == 0) {
		pMainChar->GetPosition(g_wHomeX, g_wHomeY);
		g_dwLastAttackTime = g_dwCurTime;
		SaveCheatConfig();
	}
	// 자동 공격&& g_bCheatEtc&& g_bCheatEtc
	if(g_bCheatEtc)
	{
		if(g_dwCurTime - g_dwCheatTime > (1000 * g_MainCharInfo.m_byCheatTime))
		{
			// 优先用辅助面板配置的攻击技能，未配置则回退到S槽位
			extern DWORD g_dwAttackSkillID;
			DWORD dwMugongID = (g_dwAttackSkillID > 0) ? g_dwAttackSkillID : g_MainCharInfo.m_pSlot->GetActiveSlot();

			if(dwMugongID)
			{
				g_dwCheatTime = g_dwCurTime;
				g_dwLastAttackTime = g_dwCurTime; // 技能施法也算攻击行为，刷新空闲计时器

				// 从渲染对象获取实时坐标（而非 g_MainCharInfo 缓存值）
				CXiahCharObject *pCharObject = (CXiahCharObject*)g_pMainChar->m_pObject;
				WORD wPosX, wPosY;
				pCharObject->GetPosition(wPosX, wPosY);
				BYTE bAttackHeight = (int)pCharObject->m_Position.y;

				SendCS_NV_ENDMOVE_REQ(g_pMainChar->m_dwServerID, pCharObject->m_Position.x, -pCharObject->m_Position.z, pCharObject->m_Position.y, CHARSTATE_NORMAL);

				SendCS_BT_MUGONGPREATTACK_REQ(dwMugongID,
												OBJTYPE_PC,
												g_pMainChar->m_dwServerID,
												wPosX,
												wPosY,
												bAttackHeight,
												0,
												0,
												wPosX,
												wPosY,
												bAttackHeight);
			}
			else
			{
			}
		}
	}
	else if(g_bCheat  && g_dwCurTime - g_dwCheatTime > 500 && dwSelObjectID == 0 && dwSelObjectType == 0)
	{
		// 空闲回归中心点检测：当启用中心点且超时未攻击时，自动走回中心点
		if (g_bUseHomePoint && g_nIdleReturnSec > 0 && g_wHomeX > 0 && g_wHomeY > 0) {
			DWORD dwIdleMs = (DWORD)g_nIdleReturnSec * 1000;
			float dx = pMainChar->m_Position.x - (float)g_wHomeX;
			float dy = (-pMainChar->m_Position.z) - (float)g_wHomeY;
			float fDistSq = dx*dx + dy*dy;
			if (!g_bReturningHome && g_dwLastAttackTime > 0 && (g_dwCurTime - g_dwLastAttackTime) > dwIdleMs) {
				if (fDistSq > 25.0f) {
					g_bReturningHome = TRUE;
					// 构造3D目标点（与引擎原生移动流程一致）
					Vector3 vHome((float)g_wHomeX, 0, -(float)g_wHomeY);
					WORD angle;
					pMainChar->SetAngleTarget(vHome);
					pMainChar->GetAngle(angle);
					pMainChar->Update(1);
					pMainChar->SetTargetMove(g_wHomeX, g_wHomeY, eLBP_CharNavigation, 0);
					pMainChar->SetAnimation(XiahAniType::eLAT_Run, 1);
					SendCS_NV_STARTMOVE_REQ(g_pMainChar->m_dwServerID,
						(WORD)pMainChar->m_Position.x, (WORD)(-pMainChar->m_Position.z), (BYTE)pMainChar->m_Position.y,
						g_wHomeX, g_wHomeY, (BYTE)pMainChar->m_Position.y, (WORD)angle, CHARSTATE_NORMAL, 9);
				} else {
					// 已在中心点附近，重置空闲计时器继续寻怪
					g_dwLastAttackTime = g_dwCurTime;
				}
			}
			// 回归中：检测是否到达中心点
			if (g_bReturningHome) {
				if (fDistSq <= 25.0f || !pMainChar->m_bTargetMove) {
					// 到达中心点或停止移动，重置状态开始重新寻怪
					g_bReturningHome = FALSE;
					g_dwLastAttackTime = g_dwCurTime;
					pMainChar->SetAnimation(XiahAniType::eLAT_Stand, 0);
					SendCS_NV_ENDMOVE_REQ(g_pMainChar->m_dwServerID, pMainChar->m_Position.x, -pMainChar->m_Position.z, pMainChar->m_Position.y, CHARSTATE_NORMAL);
				}
				// 回归路上不找怪，跳过下面的 ProcessAutoAttack
			} else {
				ProcessAutoAttack( pMainChar);
			}
		} else {
			ProcessAutoAttack( pMainChar);
		}

		//펫 자동 먹이 및 야생성 
		if(g_PetList.size() > 0)
		{
			WORD wPosX, wPosY;
			sPetInfo* pPetInfo = g_PetList.GetCurrentPet();
			XiahObject::CXiahObject* pObject= g_PetList.Find( pPetInfo->dwID );
			CXiahCharObject *pCharObject = (CXiahCharObject*)pObject->m_pObject;
			pCharObject->GetPosition( wPosX, wPosY);

			if(pPetInfo->m_dwIsHwan == 0) //똥개 일 경우만 처리 
			{
				if( (pPetInfo->dwHpCur < pPetInfo->dwHpMax) )
				{
					SendCS_BT_MUGONGPREATTACK_REQ(OUTGONGID_JUNYUUM, 1, g_MainCharInfo.m_dwObjectID, 1,1,1, OBJTYPE_PET, pPetInfo->dwID, wPosX, wPosY, 1);//펫 체력은 전유음으로 회복 시킨다.
				}

				if( pPetInfo->bWildRate > g_nPetWildRate ) //야생성
				{
					XiahItem::sItemInfo* pItem = g_MainCharInfo.m_pMySack[0]->FindSackItemByVisualID(g_dwPetFoodID); //독수리간만
					if( pItem) 
					{
						SendCS_IM_GIVEITEM_REQ( pItem->m_bSackCount+1, pItem->m_bSackPos, pItem->m_dwItemID, OBJTYPE_PET, pPetInfo->dwID);
					}
					else
					{
						pItem = g_MainCharInfo.m_pMySack[1]->FindSackItemByVisualID(g_dwPetFoodID);
						if( pItem)
						{
							SendCS_IM_GIVEITEM_REQ( pItem->m_bSackCount+1, pItem->m_bSackPos, pItem->m_dwItemID, OBJTYPE_PET, pPetInfo->dwID);
						}
						else //행낭에 독수리간이 없을때 봉인을 하자
						{
							XiahItem::sItemInfo* pBongInItem = g_MainCharInfo.m_pMySack[0]->FindSackItemByVisualID(g_dwPetSealID); //우선 공혼경
							if(pBongInItem)
							{
								g_MainCharInfo.ShowHelpMessage(_T("没有饲料，正在封印宠物.."), TEXTEFFECT_COLOR_GAIN);
								SendCS_NC_PETBONGIN_REQ( g_PetList.GetPetInfoByIndex(0)->dwID, pBongInItem->m_bSackCount+1, pBongInItem->m_bSackPos);
								dwSelObjectID = 0;
								dwSelObjectType = 0;
								ProcessAutoAttack( pMainChar);
							}
							else
							{
								g_MainCharInfo.ShowHelpMessage(_T("没有封印石，无法封印宠物.."), TEXTEFFECT_COLOR_WARNING);
								dwSelObjectID = 0;
								dwSelObjectType = 0;
								ProcessAutoAttack( pMainChar);
							}
						}
					}
				}
				
				if(g_MainCharInfo.m_vkeepUpPetMugongIconList.size() < 2)
				{
					BYTE byMugongLevel = g_MainCharInfo.m_pMugong->GetMugongLevelOfLearnedMugong( YUN_SUSINKIKANG);

					if(byMugongLevel > 0)
						SendCS_BT_MUGONGPREATTACK_REQ(YUN_SUSINKIKANG,1,g_pMainChar->m_dwServerID,
													1,1,1, OBJTYPE_PET, pPetInfo->dwID, wPosX, wPosY, 1);
					else
						SendCS_BT_MUGONGPREATTACK_REQ(OUTGONGID_YUNOYUNG,1,g_pMainChar->m_dwServerID,
													1,1,1, OBJTYPE_PET, pPetInfo->dwID, wPosX, wPosY, 1);

					SendCS_BT_MUGONGPREATTACK_REQ(OUTGONGID_KYOKANSU,1,g_pMainChar->m_dwServerID,
												1,1,1, OBJTYPE_PET, pPetInfo->dwID, wPosX, wPosY, 1);
				}
			}

		}
	}

	if(g_bCheat)
	{
		//아이템 줍기 
		for(XiahObject::CXiahObjectManager::iterator it = XiahObject::g_XiahObjectManager.begin();
			it != XiahObject::g_XiahObjectManager.end();
			++it)
		{
			XiahObject::CXiahObject *pXiahObject = it->second;

			if( NULL == pXiahObject || NULL == pXiahObject->m_pObject )
				continue;

			CXiahCharObject* pCharObject = reinterpret_cast<CXiahCharObject*>(pXiahObject->m_pObject);

			if(NULL != pCharObject && pCharObject->m_bObjType == OBJTYPE_ITEM)
			{
				WORD wPosX;
				WORD wPosY;
				pCharObject->GetPosition( wPosX, wPosY);

				XiahItem::sItemInfo* pInfo = (XiahItem::sItemInfo*)pCharObject->m_pPrivateData;

				if(NULL != pInfo)
				{
					if (IsItemFiltered((LPCTSTR)pInfo->m_szName))
					{
						// Skip filtered item
					}
					else if (g_bAutoLoot)
					{
						if(pCharObject->m_dwOwnerID ==  g_MainCharInfo.m_dwObjectID)
						{
							SendCS_IM_PICK_REQ( pInfo->m_dwMapID, pInfo->m_dwItemID, wPosX, wPosY, 255, pInfo->m_dwMapObjectID, pInfo->m_dwAmount);
							break;
						}
						else if(pCharObject->m_dwOwnerID == 0)
						{
							SendCS_IM_PICK_REQ( pInfo->m_dwMapID, pInfo->m_dwItemID, wPosX, wPosY, 255, pInfo->m_dwMapObjectID, pInfo->m_dwAmount);
							break;
						}
					}
				}
			}
		}

		//물약 먹기 
		if(g_bAutoHP && g_MainCharInfo.m_dwHpCur < ((g_MainCharInfo.m_dwHpMax * g_nHPPercent) / 100))
		{
			XiahItem::sItemInfo* pItem = FindSackItemByName(g_szHPPotionName);
			if( pItem)
			{
				SendCS_IM_USEITEM_REQ( pItem->m_bSackCount+1, pItem->m_bSackPos, pItem->m_dwItemID);
			}
			else
			{
				pItem = g_MainCharInfo.m_pMySack[0]->FindSackItemByVisualID(22200); //무구영단
				if( pItem)
					SendCS_IM_USEITEM_REQ( pItem->m_bSackCount+1, pItem->m_bSackPos, pItem->m_dwItemID);
			}
		}

		if(g_bAutoMP && g_MainCharInfo.m_wIpCur < ((g_MainCharInfo.m_wIpMax * g_nMPPercent) / 100))
		{
			XiahItem::sItemInfo* pItem = FindSackItemByName(g_szMPPotionName);
			if( pItem)
			{
				SendCS_IM_USEITEM_REQ( pItem->m_bSackCount+1, pItem->m_bSackPos, pItem->m_dwItemID);
			}
			else
			{
				pItem = g_MainCharInfo.m_pMySack[0]->FindSackItemByVisualID( 22200);//무구영단
				if( pItem)
					SendCS_IM_USEITEM_REQ( pItem->m_bSackCount+1, pItem->m_bSackPos, pItem->m_dwItemID);
			}
		}
		
		//HT_CHEAT : 자동 판매
		/*if(g_MainCharInfo.m_bAutoSell)
		{
			XiahItem::sItemInfo* pItemInfo = NULL;
			pItemInfo = g_MainCharInfo.m_pMySack[g_MainCharInfo.m_bySellSackPos]->FindSackItemByPos(g_MainCharInfo.m_bySellPos);
			if(pItemInfo)
			{
				g_MainCharInfo.ShowHelpMessage(pItemInfo->m_szName);
				SendCS_EC_SELLITEM_REQ( 984, 
											pItemInfo->m_dwItemID,
											pItemInfo->m_bSackCount+1,
											pItemInfo->m_bSackPos);

			}
			else
			{
				g_MainCharInfo.m_bAutoSell = false;
				g_MainCharInfo.m_bySellPos = 0;
			}
		}*/
		// Auto remote purchase
		extern void ProcessAutoBuyPotions();
		ProcessAutoBuyPotions();
		// Auto cast skills
		extern void ProcessAutoCastSkills();
		ProcessAutoCastSkills();
		// Auto remote sell
		extern void ProcessAutoSell();
		ProcessAutoSell();
	}

	//HT_CHEAT : 자동 판매
	if(g_MainCharInfo.m_bAutoSell)
	{
		XiahItem::sItemInfo* pItemInfo = NULL;
		pItemInfo = g_MainCharInfo.m_pMySack[g_MainCharInfo.m_bySellSackPos]->FindSackItemByPos(g_MainCharInfo.m_bySellPos);

		if(pItemInfo)
		{
			SendCS_EC_SELLITEM_REQ( 984, 
									pItemInfo->m_dwItemID,
									pItemInfo->m_bSackCount+1,
									pItemInfo->m_bSackPos);
		}
		else
		{
			g_MainCharInfo.m_bAutoSell = false;
			g_MainCharInfo.m_bySellPos = 0;
		}
	}

	// 龟息大法持续倒地：倒地动画播完后用eLAT_Died(12)保持倒地姿态
	if( pMainChar->m_KeepUpMugongList.IsExist(OUTGONGID_GYUISIKDAEBUB) &&
		pMainChar->m_nCurMotionType != XiahAniType::eLAT_Mugong &&
		pMainChar->m_nCurMotionType != XiahAniType::eLAT_Died )
	{
		pMainChar->SetAnimation( XiahAniType::eLAT_Died, 0 );
	}

	pMainChar->Update(TRUE);

	// Xiah BGM Update
	ProcessXiahBGM(FALSE);

	// PET AI처리
	//HT_CHEAT : 펫 사냥 할게 없을 경우 초기화
	if(!g_PetList.UpdatePet())
	{
		dwSelObjectID = 0;
		dwSelObjectType = 0;
	}

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


///////////////////////////////////////////////////
BOOL CheckPortalMove( CXiahCharObject *pMainChar)
///////////////////////////////////////////////////
{
	if( bMove && Fade::g_bFadeStart == FALSE && g_MainCharInfo.m_bPortalMove)
	{
		WORD wPosX;
		WORD wPosY;
		
		pMainChar->GetPosition( wPosX, wPosY);
		XiahMap::sPortalInfo* pPortal = XiahMap::g_XiahMap.IntersectPortal(wPosX,wPosY);

		if( pPortal)
		{
			SendCS_NV_ENDMOVE_REQ( g_pMainChar->m_dwServerID, pMainChar->m_Position.x, -pMainChar->m_Position.z, pMainChar->m_Position.y,CHARSTATE_NORMAL);
			// 포탈에 닿았으니 멈추자
			pMainChar->SetAnimation( XiahAniType::eLAT_Stand, 0);
			bMove = FALSE;

			Fade::StartFade( 0, 1, FadeTrigger_MapMove);

			return TRUE;
		}
	}

	return FALSE;
}

///////////////////////
void ProcessLbuttonUp()
///////////////////////
{
	if( bMove)
	{	
		// navigation하려 할때, pop menu가 떠 있으면 없애주자
		g_pUIManager->DeletePopMenu();
		g_pUIManager->DeletePopSubMenu();

		// navigation하려 할때, 캐릭터가 pick한 object 는 0
		g_MainCharInfo.m_dwPickedObject = 0;

		g_MainCharInfo.HideSack( SACKTYPE__NPC_TRADE);
		g_MainCharInfo.HideSack( SACKTYPE__PERSONAL_TRADE_SELL);
		g_MainCharInfo.HideSack( SACKTYPE__ITEMMALL);
		g_MainCharInfo.HideSack( SACKTYPE__DEPOSIT);
		g_MainCharInfo.HideSack( SACKTYPE__MODIFY);
		// 매품패
		g_MainCharInfo.HideSack(SACKTYPE__QUICKMART);
		g_MainCharInfo.HideSack(SACKTYPE__SECRETROOM);
		g_MainCharInfo.HideSack(SACKTYPE__SMELT);
		g_MainCharInfo.HideSack(SACKTYPE__FIVEELEMENT_CONVERT);
	}

	//dwSelObjectID		= 0;
	//dwSelObjectType		= 0;
	bAutoNormalAttack = FALSE;	
}



// PET에게 AI Commnad시의 처리
BOOL SubProcessCommandAI(DWORD dwSelObjectID,DWORD dwSelObjectType)
{
	if(g_bCommandAI == TRUE && g_dwCommandType != PETAI_NONE)
	{
		switch(g_dwCommandType)
		{
			// 대상공격
			case PETAI_TARGETATTACK :
                // 2004.07.30 Changth
				// 아직은 펫을 공격할 수 없다.
				if(dwSelObjectType == OBJTYPE_NPC || dwSelObjectType == OBJTYPE_PC ) // || dwSelObjectType == OBJTYPE_PET )
				{
					g_PetList.Change_PET_AI(PETAI_TARGETATTACK,dwSelObjectID,NULL,dwSelObjectType,NULL);
					g_bCommandAI = FALSE;
					g_dwCommandType = PETAI_NONE;
					ChangeXiahCursor( eCT_General);
				}
				else
				{
					g_bCommandAI = FALSE;
					g_dwCommandType = PETAI_AUTOATTACK;
					ChangeXiahCursor( eCT_General);
				}
				return TRUE;
			break;
		}
	}

	g_bCommandAI = FALSE;
	g_dwCommandType = PETAI_NONE;
	return FALSE;
}


/////////////////////////////////////////////////////////////////////////////////////////
void ProcessLButtonDown( CXiahCharObject *pMainChar, CXiahCharObject* pMouseOnCharObject)
/////////////////////////////////////////////////////////////////////////////////////////
{
	// 메인 캐릭이 금나수에 걸려서 움직일 수 없는 상태다.
	if( pMainChar->m_KeepUpMugongList.IsExist(OUTGONGID_KUMNASU) )
	{
		bMove = FALSE;
		return;
	}

	//HT_0530 각성 빙룡 무공이 걸리면 못 움직인다.
	if( pMainChar->m_KeepUpMugongList.IsExist(BING_DRAGONSINJANG ) || pMainChar->m_KeepUpMugongList.IsExist(BING_DRAGONSUNGCHEON ) )
	{
		bMove = FALSE;
		return;
	}

	if( g_MainCharInfo.m_bMainCharDie || g_MainCharInfo.m_bMainCharMapMoveItemUse )
		return;

	// 2004.08.03 Changth
	// 야차의 자기 상태 무공에 타임 트리거가 들어가서 이렇게 해줘야 한다.
	// 시전 중에는 아직 무공이 걸린 상태가 아니므로 클라이언트가 자체적으로 해결.
	if( pMainChar->m_bSubObjType == 4 )
	{
		// 야차가 광마독공일때, 시전 중에 움직이면 다시 해제한다.
		if( pMainChar->m_bNowGwangmadokgong &&
			!pMainChar->m_KeepUpMugongList.IsExist(OUTGONGID_GWANGMADOKGONG) ) // 무공 리스트에 없으니깐 아직 안걸린 상태다.
			pMainChar->m_bNowGwangmadokgong = false;

		// 귀식대법, 시전 중에 클릭 안된다.
		if( pMainChar->m_bNowGyuisikdaebub &&
			!pMainChar->m_KeepUpMugongList.IsExist(OUTGONGID_GYUISIKDAEBUB) )
			return;
	}

 	// 이동한다던지, 공격이 시작된다던지, 아이템을 줍는다던지
	bAutoNavigation = FALSE;
	bAutoAttack		= FALSE;

	dwSelObjectID = 0;
	dwSelObjectType = 0;
	g_MainChar_PreAttackInfo.dwLastPreAttackTime = 0;
	g_MainChar_PreAttackInfo.nRemainAttackCount = 0;

	// 0.3초 안에 클릭을 막는다!
	if(g_dwCurTime - ClickTime < 300) return;
	ClickTime = g_dwCurTime;

	 // 커서가 오브젝트를 가리키고 있을때
	if( XiahObject::g_pMouseOnObject && pMouseOnCharObject)
	{
		// 현재 에니메이션이 경공중일때의 Click을 원천봉쇄 한다.
		// 경공이 계속나가는 것을 막는데 사용! (Anitype 305 -> 경공이다.)
		CXiahCharObject* pMainChar = (CXiahCharObject*)g_pMainChar->m_pObject;
		if(pMainChar->m_nCurMotionType == XiahAniType::eLAT_Mugong && pMainChar->m_nCurAniType == 305)
			return;

		// 2004.08.03 Changth
		// 귀식 대법 중에는 공격이 안된다. 아템도 못줍는다.
		if( pMainChar->m_bSubObjType == 4 && 
			pMainChar->m_KeepUpMugongList.IsExist(OUTGONGID_GYUISIKDAEBUB) )
			return;

		// 타겟을 바꿔준다
		dwSelObjectID		= XiahObject::g_pMouseOnObject->m_dwServerID;
		dwSelObjectType		= pMouseOnCharObject->m_bObjType;

		// 공격일때는 범위를 바꿔준다
		if( dwSelObjectType == OBJTYPE_NPC)
			fInteractionRange = g_MainCharInfo.m_wAttackRange;
		else
			fInteractionRange = 9;

		
		if( !(GetAsyncKeyState( VK_MENU) < 0)) 
		{
			// ALT를 안누름
			if(SubProcessCommandAI(dwSelObjectID,dwSelObjectType) == FALSE)
			{
				bAutoNavigation = TRUE;
				if(/* GetAsyncKeyState( VK_CONTROL) < 0 &&*/ dwSelObjectType == OBJTYPE_NPC)	// 원클릭으로 자동 공격
				{
					bAutoAttack = TRUE;
				}
				if( dwSelObjectType == OBJTYPE_NPC)	// npc일경우 자동 공격모드전환
				{
					bAutoNormalAttack = TRUE;
				}
				if( bAutoNavigation)
				{
					ProcessAutoNavigation( 0);
				}

				g_PickCursor.Create( XiahPak::GetTexture( 50000396), pMouseOnCharObject->m_Position.x, pMouseOnCharObject->m_Position.z, 6, COLOR_PICKCURSOR);
				g_PickCursor.SetRotate(0.03490658f); // _PI / 90.0f => 0.03490658f

				if( dwSelObjectType == OBJTYPE_PC)
				{
					InteractObject( XiahObject::g_pMouseOnObject->m_dwServerID, OBJTYPE_PC, 1);	 // OBJTYPE_PC
				}
				else if( dwSelObjectType == OBJTYPE_PET && g_PetList.Find( XiahObject::g_pMouseOnObject->m_dwServerID))
				{
					InteractObject( XiahObject::g_pMouseOnObject->m_dwServerID, OBJTYPE_PET, 1);	
				}
			}
		}
		else
		{
			// ALT 누름
			// 2004.07.30 Changth
			// 아직은 펫을 공격할 수 없다.
			if( (dwSelObjectType == OBJTYPE_NPC ||
				 dwSelObjectType == OBJTYPE_PC   ) && // ||
//				 dwSelObjectType == OBJTYPE_PET  ) &&
				 g_PetList.size() > 0)
			{
				// 지정 공격
				g_PetList.Change_PET_AI(PETAI_TARGETATTACK);
				g_bCommandAI = TRUE;
				g_dwCommandType = PETAI_TARGETATTACK;
				SubProcessCommandAI(dwSelObjectID,dwSelObjectType);
			}
			else
			if( dwSelObjectType == OBJTYPE_ITEM && g_PetList.size() > 0)
			{
				// 아이템 수집
				g_PetList.Change_PET_AI(PETAI_TAKEITEM);
			}
		}
	}
	else
	{
		if(SubProcessCommandAI(dwSelObjectID,dwSelObjectType) == FALSE)
		{
			XiahMap::g_XiahMap.GetPickPosition(vTarget);
			// 그 위치로 이동
			if( pMainChar->m_bTargetMove)
			{
				// 경공중일 경우에 땅을 찍으면 일단 멈추어 서버에 위치를 보정하게 한다
				CXiahCharObject* pMainChar = (CXiahCharObject*)g_pMainChar->m_pObject;
				if(pMainChar->m_nCurMotionType == XiahAniType::eLAT_Mugong && pMainChar->m_nCurAniType == 305)
				{
					SendCS_NV_ENDMOVE_REQ( g_pMainChar->m_dwServerID, pMainChar->m_Position.x, -pMainChar->m_Position.z, pMainChar->m_Position.y,CHARSTATE_NORMAL);
				}

				ChangingMoving(pMainChar,vTarget);
				pMainChar->m_bTargetMove = FALSE;
			}

			WORD angle;
			// ALT를 누르면 애완동물 조정모드
			if( GetAsyncKeyState( VK_MENU) < 0 && g_PetList.size() > 0)
			{
				// 애완동물 호출
				g_PetList.Change_PET_AI(PETAI_CALLTOME);

				if(pMainChar->m_nCurMotionType == XiahAniType::eLAT_Run || pMainChar->m_nCurMotionType == XiahAniType::eLAT_Mugong)
				{
					// RS [7/5/2005] 버그 수정
					if(g_MainCharInfo.m_bFastMove)
					{
						pMainChar->SetAnimation( XiahAniType::eLAT_Mugong, g_MainCharInfo.m_nFastIndex, 0.7f);
					}
					else
					{
						pMainChar->SetAnimation( XiahAniType::eLAT_Run, 1);
						float fMoveSpeed = (float)(g_MainCharInfo.m_bWalkSpeed + g_MainCharInfo.m_bPlusSpeed) / 9.0f;
						pMainChar->m_CharRender.SetAnimationSpeed( fMoveSpeed);
					}

					// 달리고 있던중
					pMainChar->SetAngleTarget( vTarget);
					pMainChar->GetAngle( angle);
					pMainChar->Update(1);
					pMainChar->SetTargetMove( vTarget.x, -vTarget.z, eLBP_CharNavigation, 0);

					// 움직임을 날려준다.
					{
						ChangingMoving(pMainChar,vTarget);
					}
				}
			}
			else
			{
				bMove = TRUE;

				// POINT를 MARK하고!
//				g_PickCursor.Create( XiahPak::GetTexture( 50000396), vTarget.x, vTarget.z, 6, COLOR_PICKCURSOR);
				// 위치 커서가 맵 오브젝트 위에 존재하는지 검사한다.
				Vector3 vMainCharSize = pMainChar->m_LocalBound.Size();
				g_PickCursor.Create( XiahPak::GetTexture( 50000396), vTarget.x, vTarget.z, 6, COLOR_PICKCURSOR, TRUE, 0, TRUE, vTarget.y, vMainCharSize.y, pMainChar->m_Position.y );
				g_PickCursor.SetRotate(0.03490658f); // _PI / 90.0f

				// PC를 움직이도록 한다.
				if(pMainChar->m_nCurMotionType != XiahAniType::eLAT_Run)
				{
					// 멈춰있었다
					if(g_MainCharInfo.m_bFastMove)
					{
						pMainChar->SetAnimation( XiahAniType::eLAT_Mugong, g_MainCharInfo.m_nFastIndex, 0.7f);
					}
					else
					{
						// 현재 야차가 귀식 대법을 쓰고 있는지 횅땍한다.
						if( pMainChar->m_bSubObjType == 4 &&
							pMainChar->m_KeepUpMugongList.IsExist(OUTGONGID_GYUISIKDAEBUB) )
						{
							pMainChar->m_KeepUpMugongList.Delete(OUTGONGID_GYUISIKDAEBUB);
							pMainChar->m_bNowGyuisikdaebub = false;

							int ani_index;
							sArrayData* pData = XiahArrayIndex::g_MugongTemplate.GetData(OUTGONGID_GYUISIKDAEBUB);
							if( pData )
							{
								ani_index = pData->GetInt(2);

								pMainChar->SetAnimation( XiahAniType::eLAT_Mugong, XiahAniType::eLAT_Run, ani_index, 1, 1.0f);
								pMainChar->m_CharRender.StopEffect();
								pMainChar->m_CharRender.SetReverseAnimation();
								pMainChar->m_CharRender.SetLoopAnimation( FALSE);

								float fMoveSpeed = (float)(g_MainCharInfo.m_bWalkSpeed + g_MainCharInfo.m_bPlusSpeed) / 9.0f;
								pMainChar->m_CharRender.SetAnimationSpeed( fMoveSpeed );	// 좀 느리네.
							}
						}
						else
						{
							pMainChar->SetAnimation( XiahAniType::eLAT_Run, 1);
							float fMoveSpeed = (float)(g_MainCharInfo.m_bWalkSpeed + g_MainCharInfo.m_bPlusSpeed) / 9.0f;
							pMainChar->m_CharRender.SetAnimationSpeed( fMoveSpeed);
						}
					}
		
					pMainChar->SetAngleTarget( vTarget);
					pMainChar->GetAngle( angle);
					pMainChar->Update(1);
					pMainChar->SetTargetMove( vTarget.x, -vTarget.z, eLBP_CharNavigation, 0);
					SendCS_NV_STARTMOVE_REQ( g_pMainChar->m_dwServerID, pMainChar->m_Position.x, -pMainChar->m_Position.z, pMainChar->m_Position.y,
											vTarget.x, -vTarget.z, vTarget.y, (WORD)angle, CHARSTATE_NORMAL, 9);
					MoveTime = g_dwCurTime;
				}
				else if(pMainChar->m_nCurMotionType == XiahAniType::eLAT_Run)
				{
					if(g_MainCharInfo.m_bFastMove)
					{
						pMainChar->SetAnimation( XiahAniType::eLAT_Mugong, g_MainCharInfo.m_nFastIndex, 0.7f);
					}

					// 달리고 있던중
					pMainChar->SetAngleTarget( vTarget);
					pMainChar->GetAngle( angle);
					pMainChar->Update(1);
					pMainChar->SetTargetMove( vTarget.x, -vTarget.z, eLBP_CharNavigation, 0);
					// 움직임을 날려준다.
					{
						ChangingMoving(pMainChar,vTarget);
					}
				}
			}
		}
	}	
}

/////////////////////////////////////////////////////
void ProcessCharIsMoving( CXiahCharObject *pMainChar)
/////////////////////////////////////////////////////
{
	// 움직일때 맵간 이동을 구현해주는게 제일 좋을듯 하다
	float angle = 90-pMainChar->m_Angle * 57.29577951f;  // 180.0f / _PI => 57.29577951f

	if( angle < 0)
		angle += 360;

	// 움직임의 Sync를 날려준다.
	if(!g_bCheat)
		SendCS_NV_SYNCMOVE_REQ( g_pMainChar->m_dwServerID, pMainChar->m_Position.x, -pMainChar->m_Position.z, pMainChar->m_Position.y,
								vTarget.x, -vTarget.z, vTarget.y, angle, CHARSTATE_NORMAL, 9);
	

	MoveTime = g_dwCurTime;	
}

/////////////////////////////////////////////////////
void ChangingMoving(CXiahCharObject *pMainChar, Vector3 vTarget)
/////////////////////////////////////////////////////
{
	// 움직일때 맵간 이동을 구현해주는게 제일 좋을듯 하다
	float angle = 90-pMainChar->m_Angle * 57.29577951f;


	if( angle < 0)
		angle += 360;

	SendCS_NV_ENDMOVE_REQ( g_pMainChar->m_dwServerID, pMainChar->m_Position.x, -pMainChar->m_Position.z, pMainChar->m_Position.y,
		CHARSTATE_NORMAL);

	SendCS_NV_STARTMOVE_REQ( g_pMainChar->m_dwServerID, pMainChar->m_Position.x, -pMainChar->m_Position.z, pMainChar->m_Position.y,
		vTarget.x, -vTarget.z, vTarget.y, (WORD)angle, CHARSTATE_NORMAL, 9);

	MoveTime = g_dwCurTime;	
}


//////////////////////////////////////////////////////////////////////////////
void ProcessRButtonDown( CXiahCharObject *pMainChar, BOOL bMouseOnObjectType)
//////////////////////////////////////////////////////////////////////////////
{
	if( g_MainCharInfo.m_bMainCharDie || g_MainCharInfo.m_bMainCharMapMoveItemUse )
		return;

	static DWORD lastMugongTime = 0;

	DWORD dwMugongID = 0;	// 선택된 무공

	//dwMugongID = 7;// 일단 기세배강으롯!
	// dwMugongID = 0 이면 activemugong 없음.
	dwMugongID = g_MainCharInfo.m_pSlot->GetActiveSlot();

	if( !dwMugongID)
		return;

	CloseAllWindow();

	if( g_dwCurTime - lastMugongTime > 500)
	{
		WORD wPosX;
		WORD wPosY;
		BYTE bAttackHeight;
		WORD wTargetPosX;
		WORD wTargetPosY;
		BYTE bTargetHeight;
		
		BYTE bDefType = 0;
		DWORD dwDefID = 0;

		CXiahCharObject *pCharObject = (CXiahCharObject*)g_pMainChar->m_pObject;
		pCharObject->GetPosition( wPosX, wPosY);
		bAttackHeight = (int)pCharObject->m_Position.y;

		Vector3 vPickPos;
		XiahMap::g_XiahMap.GetPickPosition(vPickPos);

		g_PickCursor.Create( XiahPak::GetTexture( 50000396), vPickPos.x, vPickPos.z, 6, COLOR_PICKCURSOR);
		g_PickCursor.SetRotate(0.03490658f); // _PI / 90.0f

		lastMugongTime = g_dwCurTime;

		wTargetPosX = vPickPos.x;
		wTargetPosY = -vPickPos.z;
		bTargetHeight = bAttackHeight;

		// Auto모드 초기화
		dwSelObjectID = 0;
		dwSelObjectType = 0;
		bAutoAttack			= FALSE;
		bAutoNavigation		= FALSE;
		bAutoNormalAttack	= FALSE;
		g_MainChar_PreAttackInfo.nRemainAttackCount	 = 0;
		g_MainChar_PreAttackInfo.dwLastPreAttackTime = 0;


		// 타켓 정보가 있으면 넣어준다
		if( XiahObject::g_pMouseOnObject)
		{
			XiahObject::CXiahObject_Basic *pBasicObject = XiahObject::g_pMouseOnObject->m_pObject;

			if( pBasicObject->IsA( XiahObject::eXOT_CharObject))
			{
				switch( pBasicObject->m_bObjType)
				{
				case OBJTYPE_PC:
				case OBJTYPE_NPC:
				case OBJTYPE_PET:
					{
						CXiahCharObject *pCharObject = (CXiahCharObject*)pBasicObject;

						bDefType = pBasicObject->m_bObjType;
						dwDefID = XiahObject::g_pMouseOnObject->m_dwServerID;

						pCharObject->GetPosition( wTargetPosX, wTargetPosY);
						bTargetHeight = (int)pCharObject->m_Position.y;
					}
					break;
				case OBJTYPE_FUNCTIONALNPC:
					{
						CXiahCharObject* pCharObject = (CXiahCharObject*)pBasicObject;

						//YS_0811 : BUGFIX
						if ( pCharObject != NULL )
						{
							sFunctionalNpcInfo* pInfo = (sFunctionalNpcInfo*)pCharObject->m_pPrivateData;

							if ( NULL != pInfo && pInfo->m_bKind == 100 )	// 문파 비석일떄
							{
								bDefType = pBasicObject->m_bObjType;
								dwDefID = XiahObject::g_pMouseOnObject->m_dwServerID;

								pCharObject->GetPosition( wTargetPosX, wTargetPosY);
								bTargetHeight = (int)pCharObject->m_Position.y;
							}
						}
					}
					break;
				}
			}
		};

		// 경공
		if( dwMugongID == OUTGONGID_ILYUIDOGANG	 || 
			dwMugongID == OUTGONGID_YUESUSINYUNG || 
			dwMugongID == OUTGONGID_JILPUNGBO	 ||
			dwMugongID == OUTGONGID_CHOSANGBI      )
		{

		}
		else
		{
			pCharObject->SetAngleTarget( wTargetPosX, wTargetPosY);
		}

		if( dwMugongID == OUTGONGID_UNKIHAENG ||	 // 운기행
			dwMugongID == OUTGONGID_JOSIKSUL ||		// 조식술
			dwMugongID == OUTGONGID_WHANSUYUO ||	// 환수유
			dwMugongID == OUTGONGID_KIYOESUL )		// 기요술
		{
			bDefType = OBJTYPE_PC;
			dwDefID = g_pMainChar->m_dwServerID;
			wTargetPosX = 0;
			wTargetPosY = 0;
			bTargetHeight = 0;
		}

		BOOL bAttackAvailable = TRUE;

		// 메인 케릭이 탈백인이 걸린 상태면 공격을 할 수 없다.
		if( pMainChar->m_KeepUpMugongList.IsExist(OUTGONGID_TALBAKIN) )
			bAttackAvailable = FALSE;

		// 메인 케릭이 귀식대법을 쓰고 있으면 곧바로 공격을 할 수 없다.
		if( pMainChar->m_KeepUpMugongList.IsExist(OUTGONGID_GYUISIKDAEBUB) )
			bAttackAvailable = FALSE;

		// 메인 케릭이 광마독공을 쓰고 있으면 또다시 할 필요가 없다.
		if( dwMugongID == OUTGONGID_GWANGMADOKGONG &&
			pMainChar->m_KeepUpMugongList.IsExist(OUTGONGID_GWANGMADOKGONG) )
			bAttackAvailable = FALSE;

		if( bAttackAvailable )
		{
			SendCS_NV_ENDMOVE_REQ( g_pMainChar->m_dwServerID, pMainChar->m_Position.x, -pMainChar->m_Position.z, pMainChar->m_Position.y,CHARSTATE_NORMAL);

			SendCS_BT_MUGONGPREATTACK_REQ(dwMugongID,
											OBJTYPE_PC,
											g_pMainChar->m_dwServerID,
											wPosX,
											wPosY,
											bAttackHeight,
											bDefType,
											dwDefID,
											wTargetPosX,
											wTargetPosY,
											bTargetHeight);

		}
	}
}

///////////////////////////////////////////////////
void ProcessAutoAttack( CXiahCharObject *pMainChar)
///////////////////////////////////////////////////
{
	g_dwCheatTime = g_dwCurTime;
	// 注意：g_dwLastAttackTime 只在实际找到怪物时才刷新，不在此处刷新
	// 否则空闲计时器永远不会超时，回中心点功能失效

	XiahObject::CXiahObjectManager::iterator it;
	CXiahCharObject* pSelCharObject = NULL;

	float fDistance = 70;
	
	for(it = XiahObject::g_XiahObjectManager.begin(); it != XiahObject::g_XiahObjectManager.end(); it++)
	{
		XiahObject::CXiahObject* pObject = it->second;
		CXiahCharObject* pCharObject = (CXiahCharObject*) pObject->m_pObject;
		
		//YS_0728 : BUGFIX
		if( !pCharObject || pCharObject->m_bObjType != OBJTYPE_NPC )
			continue;
		
		if( pCharObject->m_bObjType == OBJTYPE_NPC && pCharObject->m_nCurMotionType == XiahAniType::eLAT_Die )
			continue;

		if( pCharObject->m_bObjType == OBJTYPE_NPC && pCharObject->m_bSubObjType == 187 ) //불사조는 삭제
			continue;

		if( pCharObject->m_bObjType == OBJTYPE_NPC && pCharObject->m_bExSubObjType != 255) //파괴용 NPC 삭제
			continue;
		
		
		float fDis_Temp = pCharObject->GetInteractionDistance( pMainChar->m_Position);
		
		if( fDis_Temp < fDistance )
		{
			fDistance = fDis_Temp;
			dwSelObjectID = pObject->m_dwServerID;
			dwSelObjectType = pCharObject->m_bObjType;
			pSelCharObject = pCharObject;
		}
	}
	
	// 找到怪物才刷新空闲计时器（没找到不刷新，让空闲超时逻辑可以触发回中心点）
	if (pSelCharObject) {
		g_dwLastAttackTime = g_dwCurTime;
	}
	
	//HT_CHEAT : 펫 사냥 처리
	sPetInfo* pPetInfo = g_PetList.GetCurrentPet();
	
	if( pPetInfo && dwSelObjectID != 0 && dwSelObjectType != 0 )
	{
		if( g_PetList.size() == 0 )//|| pPetInfo->bWildRate > 60 && pPetInfo->m_dwIsHwan == 0) //펫이 없으면 내가 사냥중. 만약 펫은 있는데 먹이 부족으로 야생성 증가하면 내가 사냥해야지..
		{
			if(pSelCharObject)
			{
				fInteractionRange = g_MainCharInfo.m_wAttackRange;
				
				bAutoNavigation = TRUE;
				bAutoAttack = TRUE;
				bAutoNormalAttack = TRUE;
				g_PickCursor.Create( XiahPak::GetTexture( 50000396), pSelCharObject->m_Position.x, pSelCharObject->m_Position.z, 6, COLOR_PICKCURSOR);
				g_PickCursor.SetRotate(0.03490658f); // _PI / 90.0f);

				ProcessAutoNavigation( 0);
			}
		}
		else //펫이 있을 경우 
		{
			if( pPetInfo->m_dwIsHwan == 2) //분신격은 같이 사냥 좀 해주지..
			{
				if(g_MainCharInfo.m_bCheat && pSelCharObject)
				{
					fInteractionRange = g_MainCharInfo.m_wAttackRange;
					
					bAutoNavigation = TRUE;
					bAutoAttack = TRUE;
					bAutoNormalAttack = TRUE;
					g_PickCursor.Create( XiahPak::GetTexture( 50000396), pSelCharObject->m_Position.x, pSelCharObject->m_Position.z, 6, COLOR_PICKCURSOR);
					g_PickCursor.SetRotate(0.03490658f); // _PI / 90.0f);

					g_PetList.Change_PET_AI(PETAI_TARGETATTACK);
					g_bCommandAI = TRUE;
					g_dwCommandType = PETAI_TARGETATTACK;
					SubProcessCommandAI(dwSelObjectID,dwSelObjectType);

					ProcessAutoNavigation( 0);
				}
			}
			else if( pPetInfo->m_dwIsHwan == 1) //환수 일 때는 가만히 있자
			{
				g_PetList.Change_PET_AI(PETAI_TARGETATTACK);
				g_bCommandAI = TRUE;
				g_dwCommandType = PETAI_TARGETATTACK;
				SubProcessCommandAI(dwSelObjectID,dwSelObjectType);
			}
			else //펫 사냥 모드다 (똥개)
			{
				if(pPetInfo->bWildRate < 60)
				{
					g_PetList.Change_PET_AI(PETAI_TARGETATTACK);
					g_bCommandAI = TRUE;
					g_dwCommandType = PETAI_TARGETATTACK;
					SubProcessCommandAI(dwSelObjectID,dwSelObjectType);

					ProcessAutoNavigation( 0);
				}
				else if(pSelCharObject)
				{
					fInteractionRange = g_MainCharInfo.m_wAttackRange;
					
					bAutoNavigation = TRUE;
					bAutoAttack = TRUE;
					bAutoNormalAttack = TRUE;
					g_PickCursor.Create( XiahPak::GetTexture( 50000396), pSelCharObject->m_Position.x, pSelCharObject->m_Position.z, 6, COLOR_PICKCURSOR);
					g_PickCursor.SetRotate(0.03490658f); // _PI / 90.0f);

					ProcessAutoNavigation( 0);
				}
			}
		}
	}
	else if(pSelCharObject)//펫이 없으면 내가 사냥
	{
		fInteractionRange = g_MainCharInfo.m_wAttackRange;
		
		bAutoNavigation = TRUE;
		bAutoAttack = TRUE;
		bAutoNormalAttack = TRUE;
		g_PickCursor.Create( XiahPak::GetTexture( 50000396), pSelCharObject->m_Position.x, pSelCharObject->m_Position.z, 6, COLOR_PICKCURSOR);
		g_PickCursor.SetRotate(0.03490658f); // _PI / 90.0f);

		ProcessAutoNavigation( 0);
	}
}


////////////////////////////////////////////////////////////////////////////////////////////////


// GAME PAD지원을 위한 AUTO TARGET을 만든다.
void UpdateAutoTarget(CXiahCharObject *pMainChar)
{
	float min_len = 1000.0f;
	AutoTargetObj = NULL;

	// 돌면서 뽑아내자!
	XiahObject::CXiahObjectManager::iterator it;
	for(it = XiahObject::g_XiahObjectManager.begin(); it != XiahObject::g_XiahObjectManager.end(); it++)
	{

		XiahObject::CXiahObject* pObject = it->second;
		CXiahCharObject* pCharObject = (CXiahCharObject*) pObject->m_pObject;
		float Len;
		switch( pCharObject->m_bObjType)
		{
		// 에너미이다. 추가!
		case OBJTYPE_NPC:
			{
				if(pCharObject->m_nCurMotionType == XiahAniType::eLAT_Die) continue;

				Len = pCharObject->GetInteractionDistance( pMainChar->m_Position);
				if(min_len > Len && Len < 40.f)
				{
					min_len = Len;
					//DBG_Put("inset auto %d",AutoTarget_count);
					AutoTargetObj = pObject;
				}

			}
			break;
		// 아이템이다. 추가!
		case OBJTYPE_FUNCTIONALNPC:
		case OBJTYPE_ITEM:
			{
				Len = pCharObject->GetInteractionDistance( pMainChar->m_Position);
				if(min_len > Len && Len < 40.f)
				{
					min_len = Len;
					//DBG_Put("inset auto %d",AutoTarget_count);
					AutoTargetObj = pObject;
				}
			}
			break;
		}
	}
}

// AUTO Target 진행
//HT_CHEAT : 게임 패드 삭제
void ProcessAutoTarget(CXiahCharObject *pMainChar)
{
	//CXiahCharObject* pCharObject = NULL;

	//
	////if(XiahInput::g_Lock_Button_Up) 
	////{
	////	g_AutoTarget = FALSE;
	////	AutoTargetObj = NULL;
	////	return; 
	////}

	//// target 버튼 클릭하면 다음을 찾는다. (찾아서 표시)
	//if(XiahInput::g_Lock_Button_On && !XiahInput::g_Attack_Button_On)
	//{
	////	UpdateAutoTarget(pMainChar); //게임 패드 
	//	if(AutoTargetObj == NULL)
	//	{
	//		g_AutoTarget = FALSE;
	//		//g_MainCharInfo.ShowHelpMessage( IDS_CANNOT_TARGET,TEXTEFFECT_COLOR_WARNING);
	//		return;
	//	}

	//	g_AutoTarget = TRUE;
	//	//g_MainCharInfo.ShowHelpMessage( "타겟 지정!");
	//	pCharObject = (CXiahCharObject*) AutoTargetObj->m_pObject;
	//	g_PickCursor.Create( XiahPak::GetTexture( 50000396), pCharObject->m_Position.x, pCharObject->m_Position.z, 6, COLOR_PICKCURSOR);
	//	g_PickCursor.SetRotate(0.03490658f); // _PI / 90.0f
	//	//return;
	//	//DBG_Put("TARGET %d",AutoTargetObj);
	//}

	//if(!AutoTargetObj) return;

	//// target을 공격한다.
	//pCharObject = (CXiahCharObject*) AutoTargetObj->m_pObject;
	//if(pCharObject != NULL && AutoTargetObj != NULL && XiahInput::g_Lock_Button_On && XiahInput::g_Attack_Button_On)
	//{
	//	g_AutoTarget = TRUE;
	//	dwSelObjectID		= AutoTargetObj->m_dwServerID;
	//	dwSelObjectType		= pCharObject->m_bObjType;

	//	// npc일경우 자동 공격모드전환
	//	if( dwSelObjectType == OBJTYPE_NPC && pCharObject->m_nCurMotionType != XiahAniType::eLAT_Die)
	//	{
	//		bAutoNavigation = TRUE;
	//		bAutoAttack = TRUE;
	//		bAutoNormalAttack = TRUE;
	//		fInteractionRange = g_MainCharInfo.m_wAttackRange;
	//		ProcessAutoNavigation( 0);
	//	}
	//	
	//	// FUNC NPC
	//	if(dwSelObjectType == OBJTYPE_FUNCTIONALNPC)
	//	{
	//		bAutoNavigation = TRUE;
	//		bAutoAttack = FALSE;
	//		bAutoNormalAttack = FALSE;
	//		ProcessAutoNavigation( 0);
	//	}

	//	// item 일경우 자동 수집모드
	//	if( dwSelObjectType == OBJTYPE_ITEM )
	//	{
	//		bAutoNavigation = TRUE;
	//		bAutoAttack = TRUE;
	//		bAutoNormalAttack = TRUE;
	//		ProcessAutoNavigation( 0);
	//	}
	//}

	//if(g_AutoTarget == TRUE && pCharObject != NULL)
	//{
	//	g_PickCursor.Create( XiahPak::GetTexture( 50000396), pCharObject->m_Position.x, pCharObject->m_Position.z, 6, COLOR_PICKCURSOR);
	//	g_PickCursor.SetRotate(0.03490658f); // _PI / 90.0f
	//}
}

// PAD용 HP,MP 물약 사용
//void ProcessUseHPMP()
//{
//	XiahItem::sItemInfo* pItem = NULL;
//	// HP 사용
//	if(XiahInput::g_HP_Button_On)
//	{
//		for(int i=0;i<2;i++)
//		{
//			pItem = g_MainCharInfo.m_pMySack[i]->FindSackItemByVisualID( POTION_VISUALID_1);
//			if( pItem)
//			{
//				SendCS_IM_USEITEM_REQ( pItem->m_bSackCount+1, pItem->m_bSackPos, pItem->m_dwItemID);
//				goto F1;
//			}
//		}
//	}
//
//	// MP 사용
//F1 :
//	if(XiahInput::g_MP_Button_On)
//	{
//		for(int i=0;i<2;i++)
//		{
//			pItem = g_MainCharInfo.m_pMySack[i]->FindSackItemByVisualID( POTION_VISUALID_2);
//			if( pItem)
//			{
//				SendCS_IM_USEITEM_REQ( pItem->m_bSackCount+1, pItem->m_bSackPos, pItem->m_dwItemID);
//				return;
//			}
//		}	
//	}
//}


// HP 관련하여 진동!
void ProcessRumble()
{
	//if(g_MainCharInfo.m_dwHpCur == 0 || g_cj == NULL)
	//	return;

	//DWORD Rumbletime = 0;

	//float rate = (float)g_MainCharInfo.m_dwHpCur / (float)g_MainCharInfo.m_dwHpMax;

	//if(rate < 0.5f)
	//{
	//	Rumbletime = 800;
	//}
	//else if(rate < 0.3f)
	//{
	//	Rumbletime = 500;
	//}
	//else
	//{
	//	return;
	//}

	//if(LastRumble + Rumbletime < g_dwCurTime)
	//{
	//	HRESULT hr = g_cj->Change_Para(300,SMALL_WEAK);

	//	hr = g_cj->Rumble_Start();

	//	LastRumble = g_dwCurTime;
	//}
}

const int m_x_pos[9] = { 96, 130, 163, 198 , 232, 271, 306, 340, 374 };

// PAD로 메뉴를 연다!
void ProcessMenu()
{
	//if(XiahInput::g_Menu_Button_On)
	//	PadMenuOpend = !PadMenuOpend;

	//if(XiahInput::g_Sel_Right_Down && PadMenuOpend)
	//{
	//	SelPadMenu++;
	//	if(SelPadMenu > 8) SelPadMenu = 0;
	//	XiahInput::g_Sel_Right_Down = 0;
	//}
	//else
	//if(XiahInput::g_Sel_Left_Down && PadMenuOpend)
	//{
	//	SelPadMenu--;
	//	if(SelPadMenu < 0 ) SelPadMenu = 8;
	//	XiahInput::g_Sel_Left_Down = 0;
	//}

	//if(XiahInput::g_Menu_Button_On && PadMenuOpend)
	//{
	//	// OPEN MENU
	//	g_pUIManager->Show(WINDOW_BUTTON_GROUP_01); // 게임 메뉴
	//	g_pUIManager->Show(SYSTEM_BUTTON_GROUP_01); // 시스템 메뉴
	//}
	//else
	//if(XiahInput::g_Menu_Button_On && !PadMenuOpend)
	//{
	//	// CLOSE MENU
	//	g_pUIManager->Hide(WINDOW_BUTTON_GROUP_01); // 게임 메뉴
	//	g_pUIManager->Hide(SYSTEM_BUTTON_GROUP_01); // 시스템 메뉴
	//}

	//// 커서를 고정!
	//if(PadMenuOpend)
	//{
	//	SetCursorPos(m_x_pos[SelPadMenu],700);	
	//
	//	if(XiahInput::g_Attack_Button_On) 
	//	{
	//		switch(SelPadMenu)
	//		{
	//			case 0:	// 행낭
	//				ProcessClickSackButton();
	//				g_pUIManager->Hide(WINDOW_BUTTON_GROUP_01); // 게임 메뉴
	//				g_pUIManager->Hide(SYSTEM_BUTTON_GROUP_01); // 시스템 메뉴
	//				PadMenuOpend = FALSE;
	//				break;
	//			case 1:	// 캐릭터정보
	//				if(!g_MainCharInfo.m_bPersonalTradeSell)
	//				{
	//					ProcessClickCharInfoButton();
	//					g_pUIManager->Hide(WINDOW_BUTTON_GROUP_01); // 게임 메뉴
	//					g_pUIManager->Hide(SYSTEM_BUTTON_GROUP_01); // 시스템 메뉴
	//					PadMenuOpend = FALSE;
	//				} // if(!g_MainCharInfo.m_bPersonalTradeSell)
	//				break;
	//			case 2:	// 무공
	//				if(!g_MainCharInfo.m_bPersonalTradeSell)
	//				{					
	//					ProcessClickMugongButton( 1);
	//					g_pUIManager->Hide(WINDOW_BUTTON_GROUP_01); // 게임 메뉴
	//					g_pUIManager->Hide(SYSTEM_BUTTON_GROUP_01); // 시스템 메뉴
	//					PadMenuOpend = FALSE;
	//				} // if(!g_MainCharInfo.m_bPersonalTradeSell)
	//				break;
	//			case 3:	// 관계
	//				if(!g_MainCharInfo.m_bPersonalTradeSell)
	//				{					
	//					ProcessClickRelationButton( eDAN);
	//					g_pUIManager->Hide(WINDOW_BUTTON_GROUP_01); // 게임 메뉴
	//					g_pUIManager->Hide(SYSTEM_BUTTON_GROUP_01); // 시스템 메뉴
	//					PadMenuOpend = FALSE;
	//				} // if(!g_MainCharInfo.m_bPersonalTradeSell)
	//				break;
	//			case 4:	// 기연
	//				if(!g_MainCharInfo.m_bPersonalTradeSell)
	//				{
	//					ProcessClickQuestButton();
	//					g_pUIManager->Hide(WINDOW_BUTTON_GROUP_01); // 게임 메뉴
	//					g_pUIManager->Hide(SYSTEM_BUTTON_GROUP_01); // 시스템 메뉴
	//					PadMenuOpend = FALSE;
	//				}
	//				break;

	//			case 5:	// 미니맵
	//				g_MainCharInfo.ShowMiniMap( TRUE);
	//				g_pUIManager->Hide(WINDOW_BUTTON_GROUP_01); // 게임 메뉴
	//				g_pUIManager->Hide(SYSTEM_BUTTON_GROUP_01); // 시스템 메뉴
	//				PadMenuOpend = FALSE;
	//				break;
	//			case 6:	// 헬프
	//				g_MainCharInfo.OpenFrame( A_HELP);
	//				g_pUIManager->Hide(WINDOW_BUTTON_GROUP_01); // 게임 메뉴
	//				g_pUIManager->Hide(SYSTEM_BUTTON_GROUP_01); // 시스템 메뉴
	//				PadMenuOpend = FALSE;
	//				break;
	//			case 7:	// 옵션
	//				if(!g_MainCharInfo.m_bPersonalTradeSell)
	//				{					
	//					ProcessClickOptionButton();
	//					g_pUIManager->Hide(WINDOW_BUTTON_GROUP_01); // 게임 메뉴
	//					g_pUIManager->Hide(SYSTEM_BUTTON_GROUP_01); // 시스템 메뉴
	//					PadMenuOpend = FALSE;
	//				} // if(!g_MainCharInfo.m_bPersonalTradeSell)
	//				break;
	//			case 8:	// 끝내기
	//				g_pUIManager->Hide(WINDOW_BUTTON_GROUP_01); // 게임 메뉴
	//				g_pUIManager->Hide(SYSTEM_BUTTON_GROUP_01); // 시스템 메뉴
	//				PadMenuOpend = FALSE;
	//				PostMessage( g_AppData.m_hWnd, WM_CLOSE, 0, 0);
	//				break;
	//		}
	//	}
	//}
}

// PAD사용시의 Quick Slot
void ProcessQuickSlot()
{
	// <- 방향전환
	//if(XiahInput::g_Sel_Right_Down)
	//{
	//	DBG_Put(_T("FUCK1"));
	//	for(int i=0;i<5;i++)
	//	{
	//		g_quickslot++;
	//		if(g_quickslot > 4) g_quickslot = 0;

	//		if(g_MainCharInfo.m_pSlot->CheckQuickSlot(g_quickslot) == 2)
	//		{
	//			if( g_MainCharInfo.m_pSlot)
	//				g_MainCharInfo.m_pSlot->SelectSlot(g_quickslot);
	//			break;
	//		}
	//		else
	//			continue;
	//	}
	//}

	//// -> 방향전환
	//if(XiahInput::g_Sel_Left_Down)
	//{
	//	DBG_Put(_T("FUCK2"));
	//	for(int i=0;i<5;i++)
	//	{
	//		g_quickslot--;
	//		if(g_quickslot < 0) g_quickslot = 4;

	//		if(g_MainCharInfo.m_pSlot->CheckQuickSlot(g_quickslot) == 2)
	//		{
	//			if( g_MainCharInfo.m_pSlot)
	//				g_MainCharInfo.m_pSlot->SelectSlot(g_quickslot);
	//			break;
	//		}
	//		else
	//			continue;
	//	}
	//}
}

#ifdef LIGHTSET
// 라이트 설정 임시 코드
void LIghtSetup()
{
	// Mode
	// 0 : groballight
	// 1 : fog light
	// 2 : sky 1
	// 3 : sky 2
	// 4 : sky 3
	static bool setuplight = false;

	if(GetAsyncKeyState(VK_F12) < 0) setuplight = !setuplight;
	if(setuplight == false) return;

	if(GetAsyncKeyState(VK_F10) < 0) light_mode = 0;
	if(GetAsyncKeyState(VK_F11) < 0) light_mode = 1;

	if(GetAsyncKeyState(VK_INSERT) < 0) light_mode = 2;	
	if(GetAsyncKeyState(VK_HOME) < 0) light_mode = 3;
	if(GetAsyncKeyState(VK_PRIOR) < 0) light_mode = 4;

	// reset
	if(GetAsyncKeyState(VK_NUMPAD0) < 0)
	{
		lightR = 255;
		lightG = 255;
		lightB = 255;

		flightR = 255;
		flightG = 255;
		flightB = 255;

		skyR1 = 255;
		skyG1 = 255;
		skyB1 = 255;

		skyR2 = 255;
		skyG2 = 255;
		skyB2 = 255;

		skyR3 = 255;
		skyG3 = 255;
		skyB3 = 255;
	}

	// +R
	if(GetAsyncKeyState(VK_NUMPAD4) < 0)
	{
		if(lightR < 255 && light_mode == 0) lightR++;
		if(flightR < 255 && light_mode == 1) flightR++;

		if(skyR1 < 255 && light_mode == 2) skyR1++;
		if(skyR2 < 255 && light_mode == 3) skyR2++;
		if(skyR3 < 255 && light_mode == 4) skyR3++;
	}
	// -R
	if(GetAsyncKeyState(VK_NUMPAD1) < 0)
	{
		if(lightR > 0 && light_mode == 0) lightR--;
		if(flightR > 0 && light_mode == 1) flightR--;

		if(skyR1 > 0 && light_mode == 2) skyR1--;
		if(skyR2 > 0 && light_mode == 3) skyR2--;
		if(skyR3 > 0 && light_mode == 4) skyR3--;
	}

	// +G
	if(GetAsyncKeyState(VK_NUMPAD5) < 0)
	{
		if(lightG < 255 && light_mode == 0) lightG++;
		if(flightG < 255 && light_mode == 1) flightG++;

		if(skyG1 < 255 && light_mode == 2) skyG1++;
		if(skyG2 < 255 && light_mode == 3) skyG2++;
		if(skyG3 < 255 && light_mode == 4) skyG3++;
	}
	// -G
	if(GetAsyncKeyState(VK_NUMPAD2) < 0)
	{
		if(lightG > 0 && light_mode == 0) lightG--;
		if(flightG > 0 && light_mode == 1) flightG--;

		if(skyG1 > 0 && light_mode == 2) skyG1--;
		if(skyG2 > 0 && light_mode == 3) skyG2--;
		if(skyG3 > 0 && light_mode == 4) skyG3--;
	}

	// +B
	if(GetAsyncKeyState(VK_NUMPAD6) < 0)
	{
		if(lightB < 255 && light_mode == 0) lightB++;
		if(flightB < 255 && light_mode == 1) flightB++;

		if(skyB1 < 255 && light_mode == 2) skyB1++;
		if(skyB2 < 255 && light_mode == 3) skyB2++;
		if(skyB3 < 255 && light_mode == 4) skyB3++;
	}
	// -R
	if(GetAsyncKeyState(VK_NUMPAD3) < 0)
	{
		if(lightB > 0 && light_mode == 0) lightB--;
		if(flightB > 0 && light_mode == 1) flightB--;

		if(skyB1 > 0 && light_mode == 2) skyB1--;
		if(skyB2 > 0 && light_mode == 3) skyB2--;
		if(skyB3 > 0 && light_mode == 4) skyB3--;
	}

	sString strFPS;
	switch(light_mode)
	{
		case 0 :
		strFPS.printf( _T("(라이트) R(%d) G(%d) B(%d) F(%f)"), lightR,lightG,lightB ,g_XiahEnvInfo.m_fFogDensity);
		break;

		case 1 :
		strFPS.printf( _T("(포그) R(%d) G(%d) B(%d) F(%f)"), flightR,flightG,flightB ,g_XiahEnvInfo.m_fFogDensity);
		break;

		case 2 :
			strFPS.printf( _T("(바닥) R(%d) G(%d) B(%d)"), skyR1,skyG1,skyB1);
			break;

		case 3 :
			strFPS.printf( _T("(중간) R(%d) G(%d) B(%d)"), skyR2,skyG2,skyB2);
			break;
		case 4 :
			strFPS.printf( _T("(위) R(%d) G(%d) B(%d)"), skyR3,skyG3,skyB3);
			break;
	}

	g_MainCharInfo.ShowHelpMessage((LPCTSTR)strFPS);
	g_XiahEnvInfo.m_DiffuseColor = D3DCOLOR_XRGB( lightR,lightG,lightB );
	g_XiahEnvInfo.m_FogColor= D3DCOLOR_XRGB( flightR,flightG,flightB );
	g_XiahEnvInfo.m_SkyColorBottom = D3DCOLOR_XRGB( skyR1,skyG1,skyB1);
	g_XiahEnvInfo.m_SkyColorMiddle = D3DCOLOR_XRGB( skyR2,skyG2,skyB2);
	g_XiahEnvInfo.m_SkyColorUp = D3DCOLOR_XRGB( skyR3,skyG3,skyB3);
}

#endif



// Helper functions for name-based lookup and remote auto-buy
XiahItem::sItemInfo* FindSackItemByName(const TCHAR* szName) {
    if (!szName || _tcslen(szName) == 0) return NULL;
    for (int sackIdx = 0; sackIdx < 2; ++sackIdx) {
        CSack* pSack = g_MainCharInfo.m_pMySack[sackIdx];
        if (!pSack) continue;
        for (int i = 0; i < 255; ++i) {
            XiahItem::sItemInfo* pItem = pSack->FindSackItemByPos(i);
            if (pItem) {
                if (_tcscmp((LPCTSTR)pItem->m_szName, szName) == 0) {
                    return pItem;
                }
            }
        }
    }
    return NULL;
}

int CountSackItemByName(const TCHAR* szName) {
    if (!szName || _tcslen(szName) == 0) return 0;
    int totalCount = 0;
    for (int sackIdx = 0; sackIdx < 2; ++sackIdx) {
        CSack* pSack = g_MainCharInfo.m_pMySack[sackIdx];
        if (!pSack) continue;
        for (int i = 0; i < 255; ++i) {
            XiahItem::sItemInfo* pItem = pSack->FindSackItemByPos(i);
            if (pItem) {
                if (_tcscmp((LPCTSTR)pItem->m_szName, szName) == 0) {
                    totalCount += pItem->m_dwAmount;
                }
            }
        }
    }
    return totalCount;
}

struct DefaultShopItem {
    const TCHAR* szName;
    DWORD dwItemID;
    BYTE bShopSackPos;
};

static DefaultShopItem s_DefaultShopItems[] = {
    { _T("\xc8\xab\xb4\xb4\xd2\xa9\x28\xd0\xa1\x29"), 20100, 0 },
    { _T("\xc8\xab\xb4\xb4\xd2\xa9\x28\xb4\xf3\x29"), 20200, 1 },
    { _T("\xbd\xf0\xb4\xb4\xd2\xa9\x28\xd0\xa1\x29"), 20300, 2 },
    { _T("\xbd\xf0\xb4\xb4\xd2\xa9\x28\xb4\xf3\x29"), 20400, 3 },
    { _T("\xc4\xfd\xc6\xf8\xb5\xa4\x28\xd0\xa1\x29"), 21100, 4 },
    { _T("\xc4\xfd\xc6\xf8\xb5\xa4\x28\xb4\xf3\x29"), 21200, 5 },
    { _T("\xb4\xf3\xc4\xfd\xc6\xf8\xb5\xa4"), 21300, 6 },
    { _T("\xc4\xfd\xc6\xf8\xb5\xa4\x28\xcc\xd8\xb4\xf3\x29"), 21400, 7 },
    { _T("\xce\xde\xcb\xae\xc9\xf1\xb9\xa6"), 22200, 8 },
    { _T("\xcb\xc7\xc1\xcf"), 9102, 9 },
    { _T("\xd2\xb0\xc9\xfa\xcb\xc7\xc1\xcf"), 9102, 9 },
    { _T("\xb7\xe2\xd3\xa1\xbe\xb5"), 9210, 10 }
};
const int s_DefaultShopItemsCount = sizeof(s_DefaultShopItems) / sizeof(s_DefaultShopItems[0]);

BOOL FindNpcShopItemInfo(const TCHAR* szName, DWORD& dwItemID, BYTE& bShopSackPos) {
    if (g_MainCharInfo.m_pNpcSack) {
        for (int i = 0; i < 255; ++i) {
            XiahItem::sItemInfo* pItem = g_MainCharInfo.m_pNpcSack->FindSackItemByPos(i);
            if (pItem) {
                if (_tcscmp((LPCTSTR)pItem->m_szName, szName) == 0) {
                    dwItemID = pItem->m_wRefID;
                    bShopSackPos = pItem->m_bSackPos;
                    return TRUE;
                }
            }
        }
    }
    for (int i = 0; i < s_DefaultShopItemsCount; ++i) {
        if (_tcscmp(s_DefaultShopItems[i].szName, szName) == 0) {
            dwItemID = s_DefaultShopItems[i].dwItemID;
            bShopSackPos = s_DefaultShopItems[i].bShopSackPos;
            return TRUE;
        }
    }
    return FALSE;
}

static DWORD s_dwLastBuyTime = 0;

void ProcessAutoBuyPotions() {
    if (g_dwCurTime - s_dwLastBuyTime < 5000) {
        return;
    }

    DWORD dwShopID = 100038; // Pharmacy NPC ID (38 + 100000)

    if (g_bAutoHP && g_bAutoBuyHP && _tcslen(g_szHPPotionName) > 0) {
        int hpCount = CountSackItemByName(g_szHPPotionName);
        if (hpCount < 5) {
            DWORD dwItemID = 0;
            BYTE bShopSackPos = 0;
            if (FindNpcShopItemInfo(g_szHPPotionName, dwItemID, bShopSackPos)) {
                SendCS_EC_BUYITEM_REQ(dwShopID, dwItemID, 50, 0, bShopSackPos, g_MainCharInfo.m_byMySackCurrIdx + 1, 255);
                s_dwLastBuyTime = g_dwCurTime;
                return;
            }
        }
    }

    if (g_bAutoMP && g_bAutoBuyMP && _tcslen(g_szMPPotionName) > 0) {
        int mpCount = CountSackItemByName(g_szMPPotionName);
        if (mpCount < 5) {
            DWORD dwItemID = 0;
            BYTE bShopSackPos = 0;
            if (FindNpcShopItemInfo(g_szMPPotionName, dwItemID, bShopSackPos)) {
                SendCS_EC_BUYITEM_REQ(dwShopID, dwItemID, 50, 0, bShopSackPos, g_MainCharInfo.m_byMySackCurrIdx + 1, 255);
                s_dwLastBuyTime = g_dwCurTime;
                return;
            }
        }
    }
}

XiahObject::CXiahObject* FindTeammateByName(const TCHAR* szName) {
    if (!szName || _tcslen(szName) == 0) return NULL;
    XiahObject::CXiahObjectManager::iterator it;
    for (it = XiahObject::g_XiahObjectManager.begin(); it != XiahObject::g_XiahObjectManager.end(); it++) {
        XiahObject::CXiahObject* pObject = it->second;
        if (!pObject || !pObject->m_pObject) continue;
        if (_tcscmp((LPCTSTR)pObject->m_pObject->m_szObjectName, szName) == 0) {
            if (pObject->m_pObject->IsA(XiahObject::eXOT_CharObject)) {
                CXiahCharObject* pCharObject = (CXiahCharObject*)pObject->m_pObject;
                if (pCharObject->m_bObjType == OBJTYPE_PC) {
                    return pObject;
                }
            }
        }
    }
    return NULL;
}

void CastSkillOnTeammate(DWORD dwMugongID) {
    if (!g_MainCharInfo.m_pRelation || !g_MainCharInfo.m_pRelation->Am_I_InDan()) return;
    
    XiahObject::CXiahObject* pBestTeammate = NULL;
    DWORD dwMinHpPercent = 101;
    
    for (int i = 0; ; i++) {
        sDanInfo* pDanInfo = g_MainCharInfo.m_pRelation->FindDanInfoByIndex(i);
        if (!pDanInfo) break;
        
        XiahObject::CXiahObject* pTeammate = FindTeammateByName((LPCTSTR)pDanInfo->m_szNickName);
        if (pTeammate && pTeammate->m_pObject) {
            CXiahCharObject* pTeammateChar = (CXiahCharObject*)pTeammate->m_pObject;
            if (pTeammateChar->m_nCurMotionType != XiahAniType::eLAT_Die) {
                DWORD dwHpPercent = 100;
                if (pDanInfo->m_dwMaxHp > 0) {
                    dwHpPercent = (pDanInfo->m_dwCurHp * 100) / pDanInfo->m_dwMaxHp;
                }
                if (dwHpPercent < dwMinHpPercent) {
                    dwMinHpPercent = dwHpPercent;
                    pBestTeammate = pTeammate;
                }
            }
        }
    }
    
    if (pBestTeammate && pBestTeammate->m_pObject) {
        CXiahCharObject* pTeammateChar = (CXiahCharObject*)pBestTeammate->m_pObject;
        CXiahCharObject* pMainChar = (CXiahCharObject*)g_pMainChar->m_pObject;
        WORD wPosX, wPosY;
        pMainChar->GetPosition(wPosX, wPosY);
        BYTE bAttackHeight = (int)pMainChar->m_Position.y;
        
        WORD wTargetPosX, wTargetPosY;
        pTeammateChar->GetPosition(wTargetPosX, wTargetPosY);
        BYTE bTargetHeight = (int)pTeammateChar->m_Position.y;
        
        SendCS_NV_ENDMOVE_REQ(g_pMainChar->m_dwServerID, pMainChar->m_Position.x, -pMainChar->m_Position.z, pMainChar->m_Position.y, CHARSTATE_NORMAL);
        
        SendCS_BT_MUGONGPREATTACK_REQ(dwMugongID,
            OBJTYPE_PC,
            g_pMainChar->m_dwServerID,
            wPosX, wPosY, bAttackHeight,
            OBJTYPE_PC,
            pBestTeammate->m_dwServerID,
            wTargetPosX, wTargetPosY, bTargetHeight
        );
    }
}

void ProcessAutoCastSkills() {
    g_bIsAutoCasting = TRUE;
    if (g_dwBuffSkillID1 > 0 && g_dwCurTime - g_dwLastBuffSkillTime1 > ((DWORD)g_nBuffSkillInterval1 * 1000)) {
        if (g_MainCharInfo.m_pMugong && g_MainCharInfo.m_pMugong->IsLearnedMugong(g_dwBuffSkillID1)) {
            CXiahCharObject* pMainCharObj = (CXiahCharObject*)g_pMainChar->m_pObject;
            WORD wPosX, wPosY;
            pMainCharObj->GetPosition(wPosX, wPosY);
            BYTE bAttackHeight = (int)pMainCharObj->m_Position.y;
            SendCS_NV_ENDMOVE_REQ(g_pMainChar->m_dwServerID, pMainCharObj->m_Position.x, -pMainCharObj->m_Position.z, pMainCharObj->m_Position.y, CHARSTATE_NORMAL);
            SendCS_BT_MUGONGPREATTACK_REQ(g_dwBuffSkillID1, OBJTYPE_PC, g_pMainChar->m_dwServerID, wPosX, wPosY, bAttackHeight, 0, 0, 0, 0, 0);
            g_dwLastBuffSkillTime1 = g_dwCurTime;
        }
    }
    if (g_dwBuffSkillID2 > 0 && g_dwCurTime - g_dwLastBuffSkillTime2 > ((DWORD)g_nBuffSkillInterval2 * 1000)) {
        if (g_MainCharInfo.m_pMugong && g_MainCharInfo.m_pMugong->IsLearnedMugong(g_dwBuffSkillID2)) {
            CXiahCharObject* pMainCharObj = (CXiahCharObject*)g_pMainChar->m_pObject;
            WORD wPosX, wPosY;
            pMainCharObj->GetPosition(wPosX, wPosY);
            BYTE bAttackHeight = (int)pMainCharObj->m_Position.y;
            SendCS_NV_ENDMOVE_REQ(g_pMainChar->m_dwServerID, pMainCharObj->m_Position.x, -pMainCharObj->m_Position.z, pMainCharObj->m_Position.y, CHARSTATE_NORMAL);
            SendCS_BT_MUGONGPREATTACK_REQ(g_dwBuffSkillID2, OBJTYPE_PC, g_pMainChar->m_dwServerID, wPosX, wPosY, bAttackHeight, 0, 0, 0, 0, 0);
            g_dwLastBuffSkillTime2 = g_dwCurTime;
        }
    }
    if (g_dwTeammateSkillID1 > 0 && g_dwCurTime - g_dwLastTeammateSkillTime1 > ((DWORD)g_nTeammateSkillInterval1 * 1000)) {
        if (g_MainCharInfo.m_pMugong && g_MainCharInfo.m_pMugong->IsLearnedMugong(g_dwTeammateSkillID1)) {
            CastSkillOnTeammate(g_dwTeammateSkillID1);
            g_dwLastTeammateSkillTime1 = g_dwCurTime;
        }
    }
    if (g_dwTeammateSkillID2 > 0 && g_dwCurTime - g_dwLastTeammateSkillTime2 > ((DWORD)g_nTeammateSkillInterval2 * 1000)) {
        if (g_MainCharInfo.m_pMugong && g_MainCharInfo.m_pMugong->IsLearnedMugong(g_dwTeammateSkillID2)) {
            CastSkillOnTeammate(g_dwTeammateSkillID2);
            g_dwLastTeammateSkillTime2 = g_dwCurTime;
        }
    }
    
    // Auto attack skill casting
    if (g_dwAttackSkillID > 0 && dwSelObjectID > 0 && g_dwCurTime - g_dwLastAttackSkillTime > ((DWORD)g_nAttackSkillInterval * 1000)) {
        if (g_MainCharInfo.m_pMugong && g_MainCharInfo.m_pMugong->IsLearnedMugong(g_dwAttackSkillID)) {
            CXiahCharObject* pMainCharObj = (CXiahCharObject*)g_pMainChar->m_pObject;
            WORD wPosX, wPosY;
            pMainCharObj->GetPosition(wPosX, wPosY);
            BYTE bAttackHeight = (int)pMainCharObj->m_Position.y;
            
            WORD wTargetPosX = 0, wTargetPosY = 0;
            BYTE bTargetHeight = 0;
            CXiahCharObject* pSelCharObject = NULL;
            XiahObject::CXiahObject* pSelObject = XiahObject::g_XiahObjectManager.FindXiahObject(MAKEOBJECTID(0, dwSelObjectID, dwSelObjectType));
            if (pSelObject && pSelObject->m_pObject && pSelObject->m_pObject->IsA(XiahObject::eXOT_CharObject)) {
                pSelCharObject = (CXiahCharObject*)pSelObject->m_pObject;
                pSelCharObject->GetPosition(wTargetPosX, wTargetPosY);
                bTargetHeight = (int)pSelCharObject->m_Position.y;
            }
            
            SendCS_NV_ENDMOVE_REQ(g_pMainChar->m_dwServerID, pMainCharObj->m_Position.x, -pMainCharObj->m_Position.z, pMainCharObj->m_Position.y, CHARSTATE_NORMAL);
            
            SendCS_BT_MUGONGPREATTACK_REQ(g_dwAttackSkillID,
                OBJTYPE_PC,
                g_pMainChar->m_dwServerID,
                wPosX, wPosY, bAttackHeight,
                dwSelObjectType,
                dwSelObjectID,
                wTargetPosX, wTargetPosY, bTargetHeight
            );
            g_dwLastAttackSkillTime = g_dwCurTime;
        }
    }
    g_bIsAutoCasting = FALSE;
}

#include "XiahCheatConfig.cpp"
