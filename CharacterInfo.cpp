#include "precompile.h"
#include "resource.h"
#include "AppData.h"
#include "CharacterInfo.h"
#include "InterfaceDefine.h"
#include "CharSack.h"
#include "EquipSack.h"
#include "XiahObjectType.h"
#include "XiahCamera.h"
#include "XiahMap.h"
#include "XiahGame_Intro.h"
#include "XiahGame_Main.h"
#include "XiahArrayIndex.h"
#include "XiahGame_Handler_Sender.h"
#include "XiahGame_Pet.h"
#include "XiahGame_Minimap.h"
#include "XiahCursor.h"
#include "CharacterInfo_RefreshFrame.cpp"
#include "mail.h"
#include "Helper.h"

#include ".\spirit.h"

/*#include "SkillTime.h"*/


// SUPPORT FMOD LIB
#pragma comment (lib,"fmodvc.lib")

#define COPY_MEMBER( a) a = data->a
#define DRAWSACK(p) { if(p) (p)->DrawSack();}
#define CHECKITEMSELECTED(p) { if( (p) && (p)->IsShow() && (p)->CheckItemSelected()) return;}
#define CHECKITEMUNSELECTED(p) { if( (p) && (p)->IsShow() && (p)->CheckItemUnSelected()) return TRUE;}



CharacterInfo::CharacterInfo() : m_pImageScrMsg(NULL), m_pPremiumItem(NULL)
{
	m_dwMapID = 0;						// 맴 아이디
	m_dwObjectID = 0;					// 오브젝트 아이디
	m_dwXiahObjectID = 0;
	m_szNickName = _T("");				// 오브젝트 이름
	m_bCharType = 0;					// 클래스 타입
	m_wLevel = 0;						// 레벨
	m_dwHpCur = 0;						// 현재 체력
	m_dwHpMax = 0;						// 최대 체력
	m_wIpCur = 0;						// 현재 마나
	m_wIpMax = 0;						// 최대 마나
	m_wVit = 0;							// 생명력
	m_wStr = 0;							// 근력
	m_wSus = 0;							// 지구력
	m_wDex = 0;							// 민첩성
	m_dwBirthDate = 0;					// 생일
	m_szMunpaName = _T("");				// 문파이름
	m_szOrderName = _T("");	

	for( int i=0; i<VISUALID_NUM; ++i)
		wEquipVisualID[i] = 0;
	for( int i=0; i<VISUALID_NUM; ++i)
		m_bRarity[i] = 0;
	for( int i=0; i<VISUALID_NUM; ++i)
		m_bStxType[i] = 0;
	szMapName = _T("");

	// 게임내에서만 쓰이는 변수들
	m_bIncrStr = 0;						// 늘어난 근력
	m_bIncrSus = 0;						// 늘어난 지구력
	m_bIncrDex = 0;						// 늘어난 민첩성
	m_bIncrVit = 0;						// 늘어난 생명력
	m_i64Exp = 0;						// 경험치
	m_i64LevelExp = 0;					// 레벨 경험치
	m_i64NextLevelUpExp = 0;			// 업하기 위한 필요 경험치
	m_i64TpExp = 0;						// 수련치
	m_i64NextTpUpExp = 0;				// 업하기 위한 필요 수련치
	m_dwTotalSp = 0;					// 총 경험치
	m_wRemainSp = 0;					// 남은 경험치
	m_dwTotalTp = 0;					// 총
	m_wRemainTp = 0;
	m_wBaseAtkPwr = 0;
	m_dwTotalAtkPower = 0;
	m_wBaseDefPwr = 0;
	m_dwTotalDefPower = 0;
	m_wBaseAttackRating = 0;
	m_dwTotalAttackRating = 0;
	m_wAttackRange = 0;
	m_bWalkSpeed = 0;					// 걷기 속도
	m_bRunSpeed = 0;					// 뛰기 속도
	m_bPlusSpeed = 0;					// 보너스 속도
	m_bJumpLevel = 0;					// 점프 레벨
	m_dwPkCnt = 0;						// 피케이한 숫자
	m_bObjectType = 0;					// 오브젝트 타입
	m_dwMoney = m_dwTradeMoney = m_dwBetMoney = 0;
	m_wCritical = 0;
	m_dwFame = 0;						// 명성 수치 

	// 부수적인 데이타
	m_byMySackCurrIdx =0;

	m_pChat = NULL;
	m_pScrMsg = NULL;
	m_pHelpMsg = NULL;
	m_pSpecialChatMsg = NULL;

	for( int i=0; i<2; ++i)
		m_pMySack[i] = NULL;

	m_pEquipSack = m_pNpcSack = m_pDepositSack = m_pPcSackMine = m_pPcSackOther = m_pModifySack = m_pPersonalTradeSet = m_pPersonalTradeSell = NULL;
	

	m_pMugong = NULL;
	m_pSlot = NULL;
	m_pRelation = NULL;
	m_pHoldItem = NULL;
	m_pToolTip = NULL;
	m_pQuest = NULL;

	m_pItemMallSack			= NULL;

	m_dwPickedObject = 0;
	m_dwPickedNpc = 0;
	m_dwAskID = m_dwAskPartyID = 0;
	m_bTradeAgree = FALSE;
	m_dwMoneyOnTradeMine = 0;
	m_dwMoneyOnTradeOther = 0;
	m_bySendChatType = CT_NORMAL;
	m_byUsageVolumFrame = 0;

	m_dwCurrentSelectedBongInItem = 0;
	m_dwVolumeSplitAmount = 0;
	m_bSackCnt = 0;

	m_bDragFrame = FALSE;
	m_bShowMiniMap = FALSE;
	m_bRotateMinimap = TRUE;
	m_bChangMinimap = FALSE;
	m_bShowCharNames = TRUE;
	m_bInteractionFlag = FALSE;

	m_IsStarted = FALSE;
	m_bIsPvPMap = FALSE;

	///////////////
	m_nChatType = 0;
	m_RpairItemPos = 0;
	m_ReairSackID = 0;

	m_strWhisperName = _T("");

	m_bFirstChat = m_bPersonalTradeSell = m_bReairItemUse = m_bReairItemUse2 = false;
	m_bPickType = 0;
	m_dwResItemID =0;

	m_nTempValue = 0;

	m_bMark = m_bChatModeAction = m_bEvSocketItemUse = false;
	m_bCharChange = m_bPortalMove = true;

	m_bNormalChatShow = m_bMunpaChatShow = m_bDanChatShow = true;

	m_byPurseAction = m_byPurseSackID = m_byPurseSackPos = m_dwPurseItemID = 0;	// 전낭용

	m_byRelationType = m_byRelationStep = 0;

	m_pListClient = NULL;

	m_MunpaStonIDList.clear();

	/////////////////////////////////////////////////////////////////////////////////////////////////////
	// 임시
	m_dwMunjuID = m_dwStonID = m_dwStoneMapID = m_dwMunpaFame = 0;
	m_bStoneChannelID = m_bLevel = m_bWar = 0;
	m_dwRanking = m_dwCanBattleTime = m_dwBattleTime = m_dwTotalWar = m_dwWinWar = m_dwDrawWar = m_dwLossWar =0;
	/////////////////////////////////////////////////////////////////////////////////////////////////////

	m_nLastUseItemXPos = m_nLastUseItemYPos = m_dwReserveMoney = m_dwReserveID = 0;

	m_bFastMove = false;
	m_nFastIndex = 0;

	m_bMainCharDie = FALSE;
	m_bMainCharMapMoveItemUse = FALSE;

	m_dwTaxMunpaMoney	=0;

	m_pSmeltSack = m_pFEConvert = NULL;

	m_wFiveElmPoint = m_dwFiveElmPower = m_dwFiveElmPowerMax = m_dwFiveElmGauge = 0;
	ZeroMemory(m_wFiveElmExp, sizeof(WORD)*5);

	m_bAuction = 0;

	m_dwBuyLimit	= 0;
	m_bRarityLimit	= m_bStxTypeLimit = 0;

	m_byLastSackPos		= 0;
	m_dwLastClickTime	= 0;

	// 매품패
	m_pQuickMart = NULL;

	// 기
	m_bStaminaCnt	= 0;
	m_bSpirit		= false;

	// 아이템 수집
	m_pCollection = NULL;

	m_bMunpaFight = false;

	m_dwLordMunpaID = 0; //우승 문파 ID

	m_bRebirth = 0; // 각성자

	//HT_0911 : 프리미엄 퀘스트 수련치
	m_dwPremiumTP = 0;
	m_dwPremiumSP = 0;
	//HT_1023 : 운영자 마크 추가
	m_dwGameMasterMark =0;

	//HT_CHEAT : 치트 키와 오토 팔기 키들
	m_bCheat = false;
	m_bAutoSell = false;
	m_bySellPos = 0;
	m_bySellSackPos = 0;

	m_byCheatTime = 2;

	m_wPosX = 0;
	m_wPosY = 0;

	//HT_0403 : 지속형 무공 시전 아이콘 
	m_vkeepUpMugongIconList.clear();
	m_vkeepUpPetMugongIconList.clear();

	//HO_0413_07 퀵 가이드 업데이트
	m_bQuickIndex = 0;
}

CharacterInfo::~CharacterInfo()
{
	DeleteAll();
}

/**
 *
 * \param data 
 */
void CharacterInfo::operator=(CharacterInfo* data)
{
	COPY_MEMBER(m_dwMapID);
	COPY_MEMBER(m_dwObjectID);
	COPY_MEMBER(m_szNickName);
	COPY_MEMBER(m_bCharType);
	COPY_MEMBER(m_wLevel);
	COPY_MEMBER(m_dwHpCur);
	COPY_MEMBER(m_dwHpMax);
	COPY_MEMBER(m_wIpCur);
	COPY_MEMBER(m_wIpMax);
	COPY_MEMBER(m_wVit);
	COPY_MEMBER(m_wStr);
	COPY_MEMBER(m_wSus);
	COPY_MEMBER(m_wDex);
	COPY_MEMBER(m_dwBirthDate);
	COPY_MEMBER(m_szMunpaName);
	COPY_MEMBER(m_szOrderName);	
//		COPY_MEMBER(wEquipVisualID[VISUALID_NUM]);
	memcpy( wEquipVisualID, data->wEquipVisualID, sizeof( WORD) * VISUALID_NUM);
	memcpy( m_bRarity, data->m_bRarity, sizeof( BYTE) * VISUALID_NUM);
	memcpy( m_bStxType, data->m_bStxType, sizeof( BYTE) * VISUALID_NUM);
	COPY_MEMBER(szMapName);	

	COPY_MEMBER(m_bIncrStr);
	COPY_MEMBER(m_bIncrSus);
	COPY_MEMBER(m_bIncrDex);
	COPY_MEMBER(m_bIncrVit);
	COPY_MEMBER(m_i64Exp);
	COPY_MEMBER(m_i64LevelExp);
	COPY_MEMBER(m_i64NextLevelUpExp);
	COPY_MEMBER(m_i64TpExp);
	COPY_MEMBER(m_i64NextTpUpExp);
	COPY_MEMBER(m_dwTotalSp);
	COPY_MEMBER(m_wRemainSp);
	COPY_MEMBER(m_dwTotalTp);
	COPY_MEMBER(m_wRemainTp);
	COPY_MEMBER(m_wBaseAtkPwr);
	COPY_MEMBER(m_dwTotalAtkPower);
	COPY_MEMBER(m_wBaseDefPwr);
	COPY_MEMBER(m_dwTotalDefPower);
	COPY_MEMBER(m_wBaseAttackRating);
	COPY_MEMBER(m_dwTotalAttackRating);
	COPY_MEMBER(m_wAttackRange);
	COPY_MEMBER(m_bWalkSpeed);
	COPY_MEMBER(m_bRunSpeed);
	COPY_MEMBER(m_bPlusSpeed);
	COPY_MEMBER(m_bJumpLevel);
	COPY_MEMBER(m_dwPkCnt);
	COPY_MEMBER(m_bObjectType);

	COPY_MEMBER(m_fPosX);
	COPY_MEMBER(m_fPosY);
	COPY_MEMBER(m_bHeight);
	COPY_MEMBER(m_wDirection);
	COPY_MEMBER(m_bState);
	COPY_MEMBER(m_bRebirth);
}

/**
 *
 */
void CharacterInfo::Clear()
{
	m_pScrMsg->AllDeleteScrMsg();
	m_pHelpMsg->AllDeleteScrMsg();
	m_pSpecialChatMsg->AllDeleteScrMsg();
	m_pImageScrMsg->AllDeleteScrMsg();
	m_pPremiumItem->AllDeleteScrMsg();

	m_pChat->Clear();
	m_pSlot->Clear();
}

/**
 *
 */
void CharacterInfo::DeleteAll()
{
	DeleteChat();
    DeleteScrMsg();
	DeleteSack();
	DeleteMugong();
	DeleteSlot();
	DeleteRelation();
	DeleteQuest();
	DeleteHoldItem();

	m_bPortalMove = true;

	if(m_pListClient)
		delete m_pListClient, m_pListClient = NULL;

	//HT_0403 : 지속형 무공 시전 아이콘
	m_vkeepUpMugongIconList.clear();
	m_vkeepUpPetMugongIconList.clear();
}

////////////////////////////
void CharacterInfo::Create()
////////////////////////////
{
	DeleteAll();

	CreateChat();
	CreateScrMsg();
	CreateSack();
	CreateMugong();
	CreateSlot();
	CreateRelation();
	CreateQuest();
	CreateHoldItem();
}

////////////////////////////
void CharacterInfo::Update()
////////////////////////////
{
	CheckSpeedPing();	

	// MAP으로 들어가 시작하지 않으면 R버튼을 사용할 수 없다
	//if(g_MainCharInfo.m_IsStarted == FALSE) return;

	UpdateScreen();

	//버튼클릭이 안먹게 막아야 할것들
	if( !m_pHoldItem ||
		g_pUIManager->IsNotice() ||
		g_pUIManager->IsPopMenu() ||
		g_pUIManager->IsPopSubMenu())
		return;
		
	if( g_pUIManager->IsShow(MESSAGE_WINDOW_1BUTTON)
		|| g_pUIManager->IsShow(MESSAGE_WINDOW_2BUTTON)
		|| g_pUIManager->IsShow(WINDOW_VOLUME)
		|| g_pUIManager->IsShow(WINDOW_MONEY)
		|| g_pUIManager->IsShow(WINDOW_PURSE)

		|| g_pUIManager->IsShow(LOADING_IMAGE3) //HO_0702_07 등급표시 : 등급표시와 함게 스타트로딩과 게임로딩 부분이 동일 이미지로 처리된다.

		//|| g_pUIManager->IsShow(LOADING_IMAGE) //등급표시 적용전 코드... 나중에 지워 버리자 ..;
		//|| g_pUIManager->IsShow(LOADING_IMAGE2) //등급표시 적용전 코드... 나중에 지워 버리자 ..;

		|| g_pUIManager->IsShow(A_HELP)
		// 2004.07.20 이벤트용 로딩화면
		//|| g_pUIManager->IsShow(EVENT_LOADING_1)
		//|| g_pUIManager->IsShow(EVENT_LOADING_2)
		)
	{
		return;
	}

	CheckButtonDown();

	UpdateInput();
}

inline void CharacterInfo::CheckSpeedPing()
{
	DWORD dwCurrTick = timeGetTime();
	static DWORD sTimeInterval = timeGetTime();
	DWORD dwInterval2 = dwCurrTick - sTimeInterval;

	if( dwInterval2 > 5000)
	{
		sTimeInterval = dwCurrTick;
		SendCS_IT_SPEEDPING_REQ( dwCurrTick);
		g_MainCharInfo.m_bInteractionFlag = FALSE;
		// DBG_LogFile("PING : %d",dwCurrTick);
	}
}

inline void CharacterInfo::UpdateScreen()
{
	sPetInfo* pPetInfo = g_PetList.GetCurrentPet();

	if( pPetInfo)
        g_pUIManager->Show(PET_BUTTON_GROUP);
	else
		g_pUIManager->Hide(PET_BUTTON_GROUP);

	g_pUIManager->UpDate();
	//RefreshFramePos();	Don't move sack @_@

	RefreshSituation();
	RefreshTime2();
	
	if( m_pChat)
		m_pChat->UpdateChat();

	if( m_pScrMsg)
		m_pScrMsg->UpdateScrMsg();

	if( m_pHelpMsg)
		m_pHelpMsg->UpdateScrMsg();

	if(m_pSpecialChatMsg)
		m_pSpecialChatMsg->UpdateScrMsg();

	if(m_pImageScrMsg)
		m_pImageScrMsg->UpdateScrMsg();

	if( m_pEquipSack)		
		((CEquipSack*)m_pEquipSack)->CheckEquipShortEndu();

	if(m_pListClient)
		m_pListClient->UpDate();

	if(m_pPremiumItem)
		m_pPremiumItem->UpdateScrMsg();

	// 대화 추가
	g_Helper.Update();

	g_Spirit.Update();
}

void CharacterInfo::UpdateInput()
{
	if(GetAsyncKeyState( VK_SHIFT) < 0 )
	{
		int nType = 0;

		if(GetAsyncKeyState( VK_F1) < 0)
		{
			nType = 1;			
		}
		else if(GetAsyncKeyState( VK_F2) < 0)
		{
			nType = 2;			
		}
		else if(GetAsyncKeyState( VK_F3) < 0)
		{
			nType = 3;			
		}
		else if(GetAsyncKeyState( VK_F4) < 0)
		{	
			nType = 4;			
		}		

		if(nType)
		{
			g_pUIManager->SetData(MAIN_CHAT, main_chat_channel_select_01, CURRENT_INDEX, 1);
			g_pUIManager->SetData(MAIN_CHAT, main_chat_channel_select_02, CURRENT_INDEX, 1);
			g_pUIManager->SetData(MAIN_CHAT, main_chat_channel_select_03, CURRENT_INDEX, 1);
			g_pUIManager->SetData(MAIN_CHAT, main_chat_channel_select_04, CURRENT_INDEX, 1);

			switch(nType)
			{
			case 1:
				{
					SetCurrSendChatType( CT_NORMAL);
					g_pUIManager->SetData(MAIN_CHAT, main_chat_channel_select_01, CURRENT_INDEX, 0);
				}
				break;
			case 2:
				{
					SetCurrSendChatType( CT_WHISPER);
					g_pUIManager->SetData(MAIN_CHAT, main_chat_channel_select_02, CURRENT_INDEX, 0);
				}
				break;
			case 3:
				{
					SetCurrSendChatType( CT_MUNPA_BROADCAST);
					g_pUIManager->SetData(MAIN_CHAT, main_chat_channel_select_03, CURRENT_INDEX, 0);
				}
			    break;
			case 4:
				{
					SetCurrSendChatType( CT_DAN);
					g_pUIManager->SetData(MAIN_CHAT, main_chat_channel_select_04, CURRENT_INDEX, 0);
				}
				break;
			}
		}
	}
}

/**
 *
 */
inline void CharacterInfo::CheckButtonDown()
{
	// Check by LButton
	if( XiahInput::g_bLButtonDown)
	{
		// UnSelect HoldItem
		if( m_pHoldItem->IsHoldingItemItem() || m_pHoldItem->IsHoldingItemMoney())
			if( CheckSackItemUnSelected())
				return;

		if( m_pHoldItem->IsHoldingItemMugong() && m_pMugong)
			m_pMugong->CheckMugongUnSelected();

		// Select HoldItem
		if( !m_pHoldItem->IsHoldingItemItem())
			CheckSackItemSelected();

		if( !m_pHoldItem->IsHoldingItemMugong() && m_pMugong)
			m_pMugong->CheckMugongSelected();

		if( !m_pHoldItem->IsHoldingItem() && m_pSlot)
			m_pSlot->CheckSlotSelected();

		// Select others
		if( m_pRelation)
		{
			m_pRelation->CheckRelationIndexSelected();
		}

		if( m_pQuest)
			m_pQuest->CheckCurrQuestIndex();

		CheckOptionFrame();

		CheckChatShowType();

		// 전서구 읽기
		g_Mail.CheckIndexSelected();        
	}

	// Check by RButton
	if( XiahInput::g_bRButtonDown)
	{
		// Select
		if( !m_pHoldItem->IsHoldingItemItem())
			CheckSackItemSelected();
	}
}

////////////////////////////
void CharacterInfo::Render()
////////////////////////////
{
	RenderScreen();

	RenderSack();
	
	g_pUIManager->SpecialDraw();	// 앞쪽에 출력

	if(m_pHoldItem)
		m_pHoldItem->DrawHoldItem();
	
	if(g_GameWork.m_GameStep_0 != GAMESTEP_INTRO) //HO_0703_07 게임내 심의 등급 표기 
		m_pMugong->DrawKeepUpIcon(); 

	//HT_0403 : 지속형 무공 시전 아이콘
	if(m_pMugong)
		m_pMugong->DrawKeepUpMugongIcon();


//	g_SkillTime.UpDate(timeGetTime());
//	if(m_pSlot)
//		m_pSlot->UpDateRender();
}

inline void CharacterInfo::RenderScreen()
{
	g_pUIManager->Draw();

	if( m_pChat)
		m_pChat->ChatShow();

	if( m_pScrMsg)
		m_pScrMsg->ScrMsgShow();

	if( m_pHelpMsg)
		m_pHelpMsg->ScrMsgShow();

	if(m_pSpecialChatMsg)
		m_pSpecialChatMsg->ScrMsgShow();

	if( m_pRelation)
	{
		m_pRelation->DrawDanInfo();
		m_pRelation->DrawCurrSelectedRelation();
	}

	if( m_pQuest && g_pUIManager->IsShow(WINDOW_QUEST_01))
		m_pQuest->Show();

	if( m_pEquipSack)
		//((CEquipSack*)m_pEquipSack)->DrawEquipInfo();
		((CEquipSack*)m_pEquipSack)->DrawEquipShortEndu();

	if(m_pListClient)
		m_pListClient->Render();

	// 전서구
	g_Mail.DrawCurrSelected();
}

/**
 * 행낭 랜더링
 */
inline void CharacterInfo::RenderSack()
{
	DRAWSACK(m_pMySack[m_byMySackCurrIdx]);
	DRAWSACK(m_pEquipSack);
	DRAWSACK(m_pNpcSack);

	DRAWSACK(m_pPersonalTradeSet);  // 개인상점 설정
	DRAWSACK(m_pPersonalTradeSell);	// 개인상점 판매

	DRAWSACK(m_pItemMallSack);		// 아이템몰 SACK
	DRAWSACK(m_pDepositSack);
	DRAWSACK(m_pPcSackMine);
	DRAWSACK(m_pPcSackOther);
	DRAWSACK(m_pModifySack);
	DRAWSACK(m_pSmeltSack);			// 조합
	DRAWSACK(m_pFEConvert);			// 제련

	DRAWSACK(m_pCollection);		// 아이템 수집

	sPetInfo* pPetInfo = g_PetList.GetCurrentPet();

	if(pPetInfo)
	{
		DRAWSACK( pPetInfo->m_pSack[ pPetInfo->m_byMySackCurrIdx]);
		DRAWSACK( pPetInfo->m_pEquipSack);
	} 

	if(m_pImageScrMsg)
		m_pImageScrMsg->ScrMsgShow();

	//JH 01_26_07 프리미엄 아이템의 효과를 감추기 위함....
	//(윈도우창의 위치와 효과이미지의 위치가 겹칠경우 해당 윈도우 창을 계속해서 추가해야할듯...)
	if(m_pPremiumItem)
	{		
		if(g_pUIManager->IsShow(WINDOW_MUNPA) | 
			g_pUIManager->IsShow(WINDOW_DAN) | g_pUIManager->IsShow(DRG_ITEM_WINDOW) |
			g_pUIManager->IsShow(WINDOW_CHARACTER) | g_pUIManager->IsShow(WINDOW_NEW_TAMING) |
			g_pUIManager->IsShow(WINDOW_TAMING_ITEM) | g_pUIManager->IsShow(WINDOW_PC_STORE) |
			g_pUIManager->IsShow(WINDOW_OUTSIDE) | g_pUIManager->IsShow(WINDOW_INSIDE) |
			g_pUIManager->IsShow(WINDOW_FIVEELEMENTS) | g_pUIManager->IsShow(WINDOW_MUNPA_FOUND) |
			g_pUIManager->IsShow(WINDOW_OPTION_01) | g_pUIManager->IsShow(WINDOW_OPTION_02) |
			g_pUIManager->IsShow(WINDOW_OPTION_03) | g_pUIManager->IsShow(WINDOW_QUEST_01) |
			g_pUIManager->IsShow(WINDOW_FIVEELEMENTS_CONVERT) | g_pUIManager->IsShow(WINDOW_DAN_NEW) |
			g_pUIManager->IsShow(WINDOW_SKILL) | g_pUIManager->IsShow(WINDOW_RECOVERY) |
			g_pUIManager->IsShow(WINDOW_MUNPA_BBS_TOP) | g_pUIManager->IsShow(WINDOW_MUNPA_BBS_LIST) |
			g_pUIManager->IsShow(WINDOW_MUNPA_BBS_WRITE) | g_pUIManager->IsShow(WINDOW_MAIL) |
			g_pUIManager->IsShow(WINDOW_HELPER_LIST) | g_pUIManager->IsShow(WINDOW_HELPER_LIST1))
		{
			return;
		}

		if(m_pChat->GetChatType() == LARGECHAT)
			return;

		m_pPremiumItem->ScrMsgShow();

	}
}













//////////////////////////////////////////////////////////
// Chat
//////////////////////////////////////////////////////////
void CharacterInfo::CreateChat()
{
	m_pChat = new CChat();
}

void CharacterInfo::DeleteChat()
{
	if( m_pChat)
	{
		delete m_pChat;
		m_pChat = NULL;
	}
}

//////////////////////////////////////////////////////////
// ScrMsg
//////////////////////////////////////////////////////////
void CharacterInfo::CreateScrMsg()
{
	m_pScrMsg = new CScreenMessage( 0);
	m_pHelpMsg = new CScreenMessage( 1);
	m_pSpecialChatMsg = new CScreenMessage(2);

	m_pImageScrMsg = new CImageScrMsg();

	m_pImageScrMsg->AddTexture(1116);
	m_pImageScrMsg->AddTexture(1117);
	m_pImageScrMsg->AddTexture(1118);
	m_pImageScrMsg->AddTexture(1119);

	m_pPremiumItem = new CImageScrMsg();
	m_pPremiumItem->AddTexture(1413);
	m_pPremiumItem->AddTexture(1416);
	m_pPremiumItem->AddTexture(1415);
	m_pPremiumItem->AddTexture(1414);

	m_pPremiumItem->SetType(CImageScrMsg::RIGHT);
	m_pPremiumItem->SetPos(980, 50);
}

void CharacterInfo::DeleteScrMsg()
{
	if( m_pScrMsg)
	{
		delete m_pScrMsg;
		m_pScrMsg = NULL;
	}
	if( m_pHelpMsg)
	{
		delete m_pHelpMsg;
		m_pHelpMsg = NULL;
	}

	if(m_pSpecialChatMsg)
		delete m_pSpecialChatMsg, m_pSpecialChatMsg = NULL;

	if(m_pImageScrMsg)
		delete m_pImageScrMsg, m_pImageScrMsg = NULL;

	if(m_pPremiumItem)
		delete m_pPremiumItem, m_pPremiumItem = NULL;
}

//////////////////////////////////////////////////////////
// Mugong
//////////////////////////////////////////////////////////
void CharacterInfo::CreateMugong()
{
	m_pMugong = new CMugong();
}

void CharacterInfo::DeleteMugong()
{
	if( m_pMugong)
	{
		delete m_pMugong;
		m_pMugong = NULL;
	}
}

/////////////////////////////////////////////////////////////
//	Sack
/////////////////////////////////////////////////////////////
void CharacterInfo::CreateSack()
{	
	m_pEquipSack = new CEquipSack( SACKTYPE__EQUIPMENT, 9);				// equip sack

	for(int i=0; i < 2; ++i)
		m_pMySack[i] = new CCharSack( SACKTYPE__DEFAULT, 6, 6);			// character sack

	m_pModifySack = new CEquipSack( SACKTYPE__MODIFY, 4);

	if(m_pToolTip == NULL)
		m_pToolTip = new CIPopUp();										// item tool tip

	m_pCollection = new CEquipSack(SACKTYPE__COLLECTION, 10);			// 아이템 수집
}

/**
 *
 */
void CharacterInfo::DeleteSack()
{
	for(register int i=0; i < 2; ++i)
		SAFE_DELETE(m_pMySack[i]);		// 행낭

	SAFE_DELETE(m_pEquipSack);			// 장착
	SAFE_DELETE(m_pNpcSack);			// NPC

	SAFE_DELETE(m_pPersonalTradeSet);	// 개인 노점 설정
	SAFE_DELETE(m_pPersonalTradeSell);	// 개인 노점 판매

	SAFE_DELETE(m_pPcSackMine);
	SAFE_DELETE(m_pPcSackOther);
	SAFE_DELETE(m_pModifySack);
	SAFE_DELETE(m_pSmeltSack);			// 조합
	SAFE_DELETE(m_pQuickMart);			// 매품패

	SAFE_DELETE(m_pToolTip);			// 아이템 툴팁

	SAFE_DELETE(m_pCollection);			// 아이템 수집
}

void CharacterInfo::CheckSackItemSelected()
{
	if( XiahInput::g_bLButtonDown)
	{
		if( g_pUIManager->IsNotice() || 
			g_pUIManager->IsPopMenu() ||
			m_pHoldItem->IsHoldingItemItem())
			return;

		CHECKITEMSELECTED( m_pMySack[m_byMySackCurrIdx]);
		CHECKITEMSELECTED( m_pEquipSack);
		CHECKITEMSELECTED( m_pNpcSack);

		CHECKITEMSELECTED(m_pPersonalTradeSet);		// 개인상점 설정
		CHECKITEMSELECTED(m_pPersonalTradeSell);	// 개인상점 판매

		CHECKITEMSELECTED( m_pItemMallSack);		// item mall sack
		CHECKITEMSELECTED( m_pDepositSack);
		CHECKITEMSELECTED( m_pPcSackMine);
		CHECKITEMSELECTED( m_pPcSackOther);
		CHECKITEMSELECTED( m_pModifySack);
		CHECKITEMSELECTED(m_pSmeltSack);			// 조합
		CHECKITEMSELECTED(m_pFEConvert);			// 제련	

		CHECKITEMSELECTED(m_pCollection);			// 수집 아이템

		sPetInfo* pPetInfo = g_PetList.GetCurrentPet();
		if( pPetInfo)
		{
			CHECKITEMSELECTED( pPetInfo->m_pSack[ pPetInfo->m_byMySackCurrIdx]);
			CHECKITEMSELECTED( pPetInfo->m_pEquipSack);
		}
	}

	if( XiahInput::g_bRButtonDown)
	{
		if( g_pUIManager->IsNotice() || 
			g_pUIManager->IsPopMenu() ||
			m_pHoldItem->IsHoldingItemItem() ||
			g_pUIManager->IsShow(WINDOW_PC_TRADE))
			return;

		CHECKITEMSELECTED( m_pMySack[m_byMySackCurrIdx]);
		// 빠른 구입
		CHECKITEMSELECTED(m_pNpcSack);				// NPC 구입
		CHECKITEMSELECTED(m_pPersonalTradeSell);	// 개인상점 판매
	}
}

BOOL CharacterInfo::CheckSackItemUnSelected()
{
	CHECKITEMUNSELECTED(m_pMySack[m_byMySackCurrIdx]);
	CHECKITEMUNSELECTED(m_pEquipSack);
	CHECKITEMUNSELECTED(m_pNpcSack);

	CHECKITEMUNSELECTED(m_pPersonalTradeSet);	// 개인 상점 설정
	CHECKITEMUNSELECTED(m_pPersonalTradeSell);	// 개인 상점 판매

	CHECKITEMUNSELECTED(m_pItemMallSack);		// 아이템몰 창고
	CHECKITEMUNSELECTED(m_pDepositSack);
	CHECKITEMUNSELECTED(m_pPcSackMine);
	CHECKITEMUNSELECTED(m_pPcSackOther);
	CHECKITEMUNSELECTED(m_pModifySack);
	CHECKITEMUNSELECTED(m_pSmeltSack);			// 조합
	CHECKITEMUNSELECTED(m_pFEConvert);			// 제련
	CHECKITEMUNSELECTED(m_pQuickMart);			// 매품패

	CHECKITEMUNSELECTED(m_pCollection);			// 수집 아이템

	sPetInfo* pPetInfo = g_PetList.GetCurrentPet();
	if( pPetInfo)
	{
		CHECKITEMUNSELECTED( pPetInfo->m_pSack[ pPetInfo->m_byMySackCurrIdx]);
		CHECKITEMUNSELECTED( pPetInfo->m_pEquipSack);
	}

	if( m_pHoldItem->IsHoldingItemItem())
	{		
		if( m_pSlot->CheckSetItemOnSlot())
			return FALSE;

		if( g_pUIManager->IsMouseOnFrame())
			return FALSE;

		if( m_pHoldItem->GetHoldItemItem()->m_bSackID == SACKTYPE__DEFAULT)
		{
			// 임시랍니다
			if( m_pHoldItem->GetHoldItemItem()->m_bItemType == ITEMTYPE_BONGIN && m_pHoldItem->GetHoldItemItem()->m_dwNpcID && m_pHoldItem->GetHoldItemItem()->m_wLevel != 2 && m_pHoldItem->GetHoldItemItem()->m_wLevel != 3)
				ShowHelpMessage( IDS_CANNOT_DEAL_MONSTER);			
			else if( m_pHoldItem->GetHoldItemItem()->m_bItemType != ITEMTYPE_NPCITEM || g_CursorType != eCT_Menu)
			{
				//m_pHoldItem->ThrowItem( 0); //종료기능 추가전 코드
				if(GetAsyncKeyState( VK_MENU) < 0 || !g_info.m_bItemDropChoice)//HO_0816_07 종료기능 추가
					g_MainCharInfo.m_pHoldItem->ThrowItem( 0);
				else
					g_pUIManager->ShowNotice( IDS_ITEM_SELECTDROP, NOTICE_FRAME_OKCANCEL, NOTICE_FRAME_ITEMDROP); 
			}
				
		}
		else if( m_pHoldItem->GetHoldItemItem()->m_bSackID == SACKTYPE__PET)
		{
			m_pHoldItem->ThrowItem( 1);
		}
	}
	else if( m_pHoldItem->IsHoldingItemMoney())
	{
		if( g_pUIManager->IsMouseOnFrame())
			return FALSE;

		m_pHoldItem->ThrowMoney();
	}
	
	return TRUE;
}

//////////////////////////////////////////////////////////
// HoldItem
//////////////////////////////////////////////////////////
void CharacterInfo::CreateHoldItem()
{
	m_pHoldItem = new CHoldItem();
}

void CharacterInfo::DeleteHoldItem()
{
	if( m_pHoldItem)
	{
		delete m_pHoldItem;
		m_pHoldItem = NULL;
	}
}

//////////////////////////////////////////////////////////
// Slot
//////////////////////////////////////////////////////////
void CharacterInfo::CreateSlot()
{
	m_pSlot = new CSlot();
}

void CharacterInfo::DeleteSlot()
{
	if( m_pSlot)
	{
		delete m_pSlot;
		m_pSlot = NULL;
	}
}

//////////////////////////////////////////////////////////
//	Relation
//////////////////////////////////////////////////////////
void CharacterInfo::CreateRelation()
{
	m_pRelation = new CRelation();
}

void CharacterInfo::DeleteRelation()
{
	if( m_pRelation)
	{
		delete m_pRelation;
		m_pRelation = NULL;
	}
}

//////////////////////////////////////////////////////////
// Quest
//////////////////////////////////////////////////////////
void CharacterInfo::CreateQuest()
{
	m_pQuest = new CQuest();
}

void CharacterInfo::DeleteQuest()
{
	if( m_pQuest)
	{
		delete m_pQuest;
		m_pQuest = NULL;
	}
}

//////////////////////////////////////////////////////////
// Etc
//////////////////////////////////////////////////////////
void CharacterInfo::ShowMiniMap( BOOL bFlag)
{ 
	static BYTE currStep = 0; // 0:닫혀있다, 1:작은미니맵, 2:큰미니맵

	if( bFlag)	// 토글
	{
		m_bShowMiniMap = !m_bShowMiniMap;

		if( m_bShowMiniMap)
		{
			OpenFrame( MINIMAP_WINDOW);
			currStep = 1;
		}
		else
		{
			CloseFrame( MINIMAP_WINDOW);
			currStep = 0;
		}
	}
	else	// step에 따라서
	{
		++currStep;

		if( currStep > 2)
			currStep = 0;

		if( currStep == 0)
		{
			CloseFrame( MINIMAP_WINDOW);
			m_bShowMiniMap = FALSE;
		}
		else
		{
			m_bChangMinimap = TRUE;
			OpenFrame( MINIMAP_WINDOW);
			m_bShowMiniMap = TRUE;

			Minimap::UpdateMinimap();
		}
	}	
	//HO_0413_07 퀵 가이드 업데이트
	if(g_pUIManager->IsShow(WINDOW_HELPER_LIST0) && g_MainCharInfo.m_wLevel < 11)
	{
		g_MainCharInfo.CloseFrame(WINDOW_HELPER_LIST0);
		g_pUIManager->Show(HELP_BUTTON); 
	}
}

void CharacterInfo::RotateMiniMap()
{ 
	m_bRotateMinimap = !m_bRotateMinimap;
}

void CharacterInfo::ShowHelpMessage( LPCTSTR szText, BYTE byColorType)
{
	if(m_pScrMsg->m_bShow && (m_bEvSocketItemUse || !m_bPortalMove))
		return;

	//if( byColorType == 1) //획득
	//	PlayInterfaceSound( ISOUND_ITEM_PICKUP);
	if( byColorType == 2) //경고
		PlayInterfaceSound( ISOUND_WARNING);

	m_pHelpMsg->SetScrMsg( 0, szText, byColorType);
}

void CharacterInfo::SpecialChatMessage(LPCTSTR szText, BYTE byColorType)
{
	m_pSpecialChatMsg->SetScrMsg(0, szText, byColorType);
}

void CharacterInfo::DeleteAllScrMessage(void)
{
	m_pHelpMsg->AllDeleteScrMsg();
	m_pScrMsg->AllDeleteScrMsg();
	m_pSpecialChatMsg->AllDeleteScrMsg();
	m_pImageScrMsg->AllDeleteScrMsg();
	m_pPremiumItem->AllDeleteScrMsg();
}

void CharacterInfo::PlayInterfaceSound( int nSoundID)
{
	if(g_info.m_dwFXVolume != 0)
	{
		// FMOD
		FSOUND_SAMPLE* pBuffer = XiahPak::GetSound( nSoundID);
		if(pBuffer)
		{
			int m_Channel = FSOUND_PlaySoundEx(FSOUND_FREE, pBuffer, NULL, TRUE);

			FSOUND_SetVolume(m_Channel, XiahGameEngine::g_VolTbl[g_info.m_dwFXVolume]);
			FSOUND_SetPaused(m_Channel, FALSE);
		}		
	}	
}

void CharacterInfo::PlayInterfaceSoundWithVol( int nSoundID, Vector3 vec)
{
	//	버그 발생가능
	if(g_pMainChar)
	{
		CXiahCharObject *pCharObject = (CXiahCharObject *)g_pMainChar->m_pObject;
		if(pCharObject == NULL)
			return;

		float len = pCharObject->GetInteractionDistance(vec);
		len = ::fabs(len) * 2.5;
		if(len > 255.0f)
			return;	// 이건 플레이 할필요가 없다. 너무 멀다
		else
			if(len < 0.0f) len = 0.0f;

		long vol = XiahGameEngine::g_VolTbl[g_info.m_dwFXVolume] - (long)len;
		if(vol < 0) vol = 0;

		FSOUND_SAMPLE* pBuffer = XiahPak::GetSound( nSoundID);
		if(pBuffer)
		{
			int m_Channel = FSOUND_PlaySoundEx(FSOUND_FREE, pBuffer, NULL, TRUE);
			FSOUND_SetVolume(m_Channel , vol);
			FSOUND_SetPaused(m_Channel , FALSE);
		}	
	}
}


sString CharacterInfo::FindNameByID( DWORD dwCharID)
{
	XiahObject::CXiahObject* pObject = XiahObject::g_XiahObjectManager.FindXiahObject( MAKEOBJECTID( 0, dwCharID, OBJTYPE_PC));

	if( pObject)
	{
		CXiahCharObject* pCharObject = (CXiahCharObject*)pObject->m_pObject;

		if( pCharObject)
			return pCharObject->m_szObjectName;
		else
			return sString(_T(""));
	}
	else
		return sString(_T(""));
}

DWORD CharacterInfo::FindIDByName(LPCTSTR name)
{
	XiahObject::CXiahObjectManager::iterator it;

	for(it = XiahObject::g_XiahObjectManager.begin(); it != XiahObject::g_XiahObjectManager.end(); it++)
	{
		XiahObject::CXiahObject *pXiahObject = it->second;
		XiahObject::CXiahObject_Basic *pObjectBasic = pXiahObject->m_pObject;
	
		if( _tcscmp( (LPCTSTR)pObjectBasic->m_szObjectName, name) == 0)
		{
			return pXiahObject->m_dwServerID;
		}
	}

	return 0;
}

/**
 *
 * \param bySackType 
 */
void CharacterInfo::ShowSack( BYTE bySackType)
{
	switch( bySackType)
	{
		case SACKTYPE__DEFAULT:
			{
				if( m_pMySack[m_byMySackCurrIdx])
					m_pMySack[m_byMySackCurrIdx]->ShowSack();

				if( m_pEquipSack)
					m_pEquipSack->ShowSack();

				OpenFrame( DRG_ITEM_WINDOW);
			}		
			break;
		case SACKTYPE__NPC_TRADE:
			{
				switch(g_MainCharInfo.m_bSackCnt)
				{
					case 0:
						{
							g_pUIManager->SetData(TAP_NPC_TRADE_4, npc_trade_4_tap_button_01, CURRENT_INDEX, 2);
							g_pUIManager->SetData(TAP_NPC_TRADE_4, npc_trade_4_tap_button_02, CURRENT_INDEX, -1);
							g_pUIManager->SetData(TAP_NPC_TRADE_4, npc_trade_4_tap_button_03, CURRENT_INDEX, -1);
							g_pUIManager->SetData(TAP_NPC_TRADE_4, npc_trade_4_tap_button_04, CURRENT_INDEX, -1);
						}					
						break;
					case 1:
						{
							g_pUIManager->SetData(TAP_NPC_TRADE_4, npc_trade_4_tap_button_01, CURRENT_INDEX, -1);
							g_pUIManager->SetData(TAP_NPC_TRADE_4, npc_trade_4_tap_button_02, CURRENT_INDEX, 2);
							g_pUIManager->SetData(TAP_NPC_TRADE_4, npc_trade_4_tap_button_03, CURRENT_INDEX, -1);
							g_pUIManager->SetData(TAP_NPC_TRADE_4, npc_trade_4_tap_button_04, CURRENT_INDEX, -1);
						}					
						break;
					case 2:
						{
							g_pUIManager->SetData(TAP_NPC_TRADE_4, npc_trade_4_tap_button_01, CURRENT_INDEX, -1);
							g_pUIManager->SetData(TAP_NPC_TRADE_4, npc_trade_4_tap_button_02, CURRENT_INDEX, -1);
							g_pUIManager->SetData(TAP_NPC_TRADE_4, npc_trade_4_tap_button_03, CURRENT_INDEX, 2);
							g_pUIManager->SetData(TAP_NPC_TRADE_4, npc_trade_4_tap_button_04, CURRENT_INDEX, -1);
						}					
						break;
					case 3:
						{
							g_pUIManager->SetData(TAP_NPC_TRADE_4, npc_trade_4_tap_button_01, CURRENT_INDEX, -1);
							g_pUIManager->SetData(TAP_NPC_TRADE_4, npc_trade_4_tap_button_02, CURRENT_INDEX, -1);
							g_pUIManager->SetData(TAP_NPC_TRADE_4, npc_trade_4_tap_button_03, CURRENT_INDEX, -1);
							g_pUIManager->SetData(TAP_NPC_TRADE_4, npc_trade_4_tap_button_04, CURRENT_INDEX, 2);
						}				
						break;
				}

				m_pNpcSack = new CCharSack( SACKTYPE__NPC_TRADE, 6, 11);
				m_pNpcSack->ShowSack();

				OpenFrame( WINDOW_NPC_TRADE);
				OpenFrame( TAP_NPC_TRADE_4);
			}
			break;
		case SACKTYPE__PERSONAL_TRADE_SET: // 개인 상점 설정
			{
				m_pPersonalTradeSet = new CCharSack(SACKTYPE__PERSONAL_TRADE_SET, 6, 6);
				m_pPersonalTradeSet->ShowSack();

				OpenFrame(WINDOW_PC_STORE);
			}		
			break;

		case SACKTYPE__PERSONAL_TRADE_SELL: // 개인 상점 판매창
			{
				m_pPersonalTradeSell = new CCharSack(SACKTYPE__PERSONAL_TRADE_SELL, 6, 6);
				m_pPersonalTradeSell->ShowSack();

				OpenFrame(WINDOW_NPC_TRADE);

				g_pUIManager->SetString(WINDOW_NPC_TRADE, npc_trade_window_title_back, IDS_PT_SET_TITLE);
			}		
			break;

		case SACKTYPE__SMELT:				// 조합
			{
				m_pSmeltSack = new CCharSack(SACKTYPE__SMELT, 6, 4);
				m_pSmeltSack->ShowSack();

				OpenFrame(WINDOW_SMELT);
			}
			break;

		case SACKTYPE__FIVEELEMENT_CONVERT:	// 오행 아이템 제련
			{
				m_pFEConvert = new CEquipSack(SACKTYPE__FIVEELEMENT_CONVERT, 4);
				m_pFEConvert->ShowSack();

				OpenFrame(WINDOW_FIVEELEMENTS_CONVERT);

				TCHAR strMoney[32] = {0,};
				_stprintf(strMoney, IDS_MONEY, MoneyCommaStr(0).data());
				g_pUIManager->SetString(WINDOW_FIVEELEMENTS_CONVERT, fiveelements_convert_window_dumy_05, strMoney);
			}
			break;
		
		case SACKTYPE__ITEMMALL:			// 아이템몰
			{
				m_pItemMallSack = new CCharSack( SACKTYPE__ITEMMALL, 6, 11);
				m_pItemMallSack->ShowSack();

				OpenFrame( WINDOW_NPC_TRADE);
			}		
			break;
		
		case SACKTYPE__DEPOSIT:				// 창고지기
			{
				m_pDepositSack = new CCharSack( SACKTYPE__DEPOSIT, 6, 11);
				m_pDepositSack->ShowSack();	

				OpenFrame( WINDOW_NPC_TRADE);
			}		
			break;

		case SACKTYPE__PC_TRADE_MINE:
			{
				m_pPcSackMine = new CCharSack( SACKTYPE__PC_TRADE_MINE, 6, 4);
				m_pPcSackOther = new CCharSack( SACKTYPE__PC_TRADE_OTHER, 6, 4);
				m_pPcSackMine->ShowSack();
				m_pPcSackOther->ShowSack();
				OpenFrame( WINDOW_PC_TRADE);

				m_dwMoneyOnTradeMine = 0;
				m_dwMoneyOnTradeOther = 0;	

				g_pUIManager->SetString(WINDOW_PC_TRADE, pc_trade_window_sub_dummy_03,  _T(""));
				g_pUIManager->SetString(WINDOW_PC_TRADE, pc_trade_window_sub_dummy_04,  _T(""));
				g_pUIManager->SetString(WINDOW_PC_TRADE, pc_trade_status_dumy,  IDS_ON_DEAL);
				g_pUIManager->Show(WINDOW_PC_TRADE, pc_trade_window_button_01);

				if(g_MainCharInfo.m_pChat->GetChatType() == LARGECHAT)
				{
					g_MainCharInfo.m_pChat->SetChatType( SMALLCHAT);
				}
			}
			break;
		case SACKTYPE__MODIFY:				// 개조
			{
				if( m_pModifySack)
					m_pModifySack->ShowSack();

				OpenFrame( WINDOW_CONVERT);
			}		
			break;
		case SACKTYPE__PET:
			{
				sPetInfo* pPetInfo = g_PetList.GetCurrentPet();

				if( pPetInfo)
				{
					DBG_Assert( pPetInfo->m_pSack[ pPetInfo->m_byMySackCurrIdx] != NULL);

					pPetInfo->m_pSack[ pPetInfo->m_byMySackCurrIdx]->ShowSack();
					OpenFrame( WINDOW_TAMING_ITEM);
				}
			}
			break;
		case SACKTYPE__PET_EQUIP:
			{
				sPetInfo* pPetInfo = g_PetList.GetCurrentPet();

				if( pPetInfo)
				{
					pPetInfo->m_pEquipSack->ShowSack();
					OpenFrame( WINDOW_NEW_TAMING);
				}
			}
			break;
		case SACKTYPE__QUICKMART:			// 매품패
			{
				m_pQuickMart = new CCharSack(SACKTYPE__QUICKMART, 6, 11);
				m_pQuickMart->ShowSack();

				OpenFrame(WINDOW_NPC_TRADE);

				g_pUIManager->SetPostMsg(10);
				g_pUIManager->SetString(WINDOW_NPC_TRADE, npc_trade_window_title_back, IDS_QUICKMART_TITLE);
			}
			break;
		case SACKTYPE__COLLECTION:			// 아이템 수집
			{
				if(m_pCollection)
					m_pCollection->ShowSack();

				OpenFrame(WINDOW_COLLECTION);
			}
			break;
	}

	g_MainCharInfo.RefreshItemFrame();			
}


/**
 * 행낭 닫기
 * \param bySackType 
 * \param bFlagForModifySack 
 */
void CharacterInfo::HideSack( BYTE bySackType, BOOL bFlagForModifySack)
{
	switch( bySackType)
	{
		case SACKTYPE__DEFAULT:
			{
				if( m_pMySack[m_byMySackCurrIdx])
					m_pMySack[m_byMySackCurrIdx]->HideSack();

				if( m_pEquipSack)
					m_pEquipSack->HideSack();

				if(!m_pQuickMart)
					CloseFrame( DRG_ITEM_WINDOW);
			}		
			break;

		case SACKTYPE__NPC_TRADE:
			{
				if( m_pNpcSack)
					m_pNpcSack->HideSack();

				if(!m_pQuickMart)
					CloseFrame( WINDOW_NPC_TRADE);

				SAFE_DELETE( m_pNpcSack);

				CloseFrame( TAP_NPC_TRADE_4);

				if( g_MainCharInfo.m_pHoldItem->GetHoldItemItem() &&
					g_MainCharInfo.m_pHoldItem->GetHoldItemItem()->m_bSackID == SACKTYPE__NPC_TRADE)
					g_MainCharInfo.m_pHoldItem->SetItemBackToSack();
			}		
			break;

		case SACKTYPE__PERSONAL_TRADE_SET: // [3/5/2004] 개인 상점 설정
			{
				if(!g_MainCharInfo.m_bPersonalTradeSell)
				{
					if(m_pPersonalTradeSet)
						m_pPersonalTradeSet->HideSack();

					CloseFrame(WINDOW_PC_STORE);

					SAFE_DELETE(m_pPersonalTradeSet);
				}
			}		
			break;
		case SACKTYPE__PERSONAL_TRADE_SELL: // [3/5/2004] 개인 상점 판매창
			{
				if(m_pPersonalTradeSell)
				{
					m_pPersonalTradeSell->HideSack();

					CloseFrame(WINDOW_NPC_TRADE);
					SAFE_DELETE(m_pPersonalTradeSell);
				}
			}		
			break;

		case SACKTYPE__SMELT:				// 조합
			{
				if(m_pHoldItem)
					m_pHoldItem->SetItemBackToSack();

				if(m_pSmeltSack)
					m_pSmeltSack->HideSack();

				CloseFrame(WINDOW_SMELT);
				SAFE_DELETE(m_pSmeltSack);
			}
			break;

		case SACKTYPE__FIVEELEMENT_CONVERT:	// 오행 아이템 제련
			{
				if(m_pHoldItem)
					m_pHoldItem->SetItemBackToSack();
				
				if(m_pFEConvert)
					m_pFEConvert->HideSack();

				CloseFrame(WINDOW_FIVEELEMENTS_CONVERT);
				SAFE_DELETE(m_pFEConvert);
			}
			break;

		case SACKTYPE__ITEMMALL:			// 아이템몰
			{
				if( m_pItemMallSack)
				{
					m_pItemMallSack->HideSack();

					CloseFrame( WINDOW_NPC_TRADE);
					SAFE_DELETE( m_pItemMallSack);
				}
			}		
			break;

		case SACKTYPE__DEPOSIT:
			{
				if( m_pDepositSack)
				{
					m_pDepositSack->HideSack();

					CloseFrame( WINDOW_NPC_TRADE);
					SAFE_DELETE( m_pDepositSack);
				}
			}		
			break;

		case SACKTYPE__PC_TRADE_MINE:
			{
				if( m_pPcSackMine)
					m_pPcSackMine->HideSack();

				if( m_pPcSackOther)
					m_pPcSackOther->HideSack();

				CloseFrame( WINDOW_PC_TRADE);

				SAFE_DELETE( m_pPcSackMine);
				SAFE_DELETE( m_pPcSackOther);

				// 선택된 아이템&돈은 배낭으로 되돌리자. @_@ 주석은 생활화
				g_MainCharInfo.m_pHoldItem->SetItemBackToSack();
				g_MainCharInfo.m_pHoldItem->ReleaseHoldItemMoney();
			}		
			break;

		case SACKTYPE__MODIFY:	// 개조
			{
				if( g_pUIManager->IsShow(WINDOW_CONVERT))
				{
					if( m_pModifySack)
						m_pModifySack->HideSack( bFlagForModifySack);

					CloseFrame( WINDOW_CONVERT);
				}
			}		
			break;
		case SACKTYPE__PET:
			{
				sPetInfo* pPetInfo = g_PetList.GetCurrentPet();
				if( pPetInfo && pPetInfo->m_pSack[ pPetInfo->m_byMySackCurrIdx])
				{
					pPetInfo->m_pSack[ pPetInfo->m_byMySackCurrIdx]->HideSack();
				}

				CloseFrame( WINDOW_TAMING_ITEM);
			}
			break;
		case SACKTYPE__PET_EQUIP:
			{
				sPetInfo* pPetInfo = g_PetList.GetCurrentPet();
				if( pPetInfo && pPetInfo->m_pEquipSack)
				{
					pPetInfo->m_pEquipSack->HideSack();
				}

				//HT_CHEAT : 펫 상태창 수정
				CloseFrame( WINDOW_NEW_TAMING);
			}
			break;
		case SACKTYPE__QUICKMART:		// 매품패
			{
				if(m_pQuickMart)
				{
					if(!bFlagForModifySack)
					{
						m_pQuickMart->HideSack();

						CloseFrame(WINDOW_NPC_TRADE);
						SAFE_DELETE(m_pQuickMart);

						SendCS_EC_QUICKMART_REQ();
					}
					else
					{
						if( m_pMySack[m_byMySackCurrIdx])
							m_pMySack[m_byMySackCurrIdx]->ShowSack();

						if( m_pEquipSack)
							m_pEquipSack->ShowSack();

						g_pUIManager->Show(DRG_ITEM_WINDOW);
						g_pUIManager->Show(WINDOW_NPC_TRADE);
					}
				}
			}
			break;
		case SACKTYPE__COLLECTION:			// 아이템 수집
			{
				if(m_pCollection)
					m_pCollection->HideSack();

				CloseFrame(WINDOW_COLLECTION);
			}
			break;
		case SACKTYPE__SECRETROOM:
			{
				g_MainCharInfo.CloseFrame(WINDOW_SECRET_BASIS);
				g_MainCharInfo.CloseFrame(WINDOW_SECRET_CHECK);
				g_MainCharInfo.CloseFrame(WINDOW_SECRET_INFORMATION);
			}
			break;
	}
}

/**
 *
 * \param nFrameID 
 * \param byUsageVolumFrame 
 */
void CharacterInfo::OpenFrame( int nFrameID, BYTE byUsageVolumFrame)
{
	PlayInterfaceSound( ISOUND_WINDOW_OPEN);
	
	switch(nFrameID)
	{
	case A_HELP:
	
	case LOADING_IMAGE3: //HO_0702_07 등급표시 : 등급표시와 함게 스타트로딩과 게임로딩 부분이 동일 이미지로 처리된다.

	//case LOADING_IMAGE: //등급표시 적용전 코드 나중에 지워 버리자 ..;
	//case LOADING_IMAGE2: //등급표시 적용전 코드 나중에 지워 버리자 ..;

	// 2004.07.20 이벤트용 로딩화면
	//case EVENT_LOADING_1:
	//case EVENT_LOADING_2:
	case MESSAGE_WINDOW_1BUTTON:
	case MESSAGE_WINDOW_2BUTTON:
	case WINDOW_VOLUME:
	case WINDOW_MONEY:
	case WINDOW_CLOSE:
	case WINDOW_PURSE:
	case WINDOW_MUNPA_DONATE:
	case WINDOW_MUNPA_WAR_PETITION:
	case WINDOW_CHARACTER:		
	case WINDOW_HELPER_LIST2:		// 대화 리스트
	//HO_0410_07 상서령, 퀵 가이드 업데이트
	case WINDOW_HELPER_LIST1:		// 상서령 가이드 리스트
	case WINDOW_HELPER_LIST:		// 상서령 가이드 대화 내용
	case WINDOW_HELPER_LIST0:		// 퀵 가이드 대화 내용
		{
			g_pUIManager->ForwardShow(nFrameID);
		}		
		break;
	default:
		{
			g_pUIManager->Show(nFrameID);
		}		
		break;
	}

	switch( nFrameID)
	{
	case WINDOW_VOLUME:
		{
			m_byUsageVolumFrame = byUsageVolumFrame;

			if(g_pUIManager->IsShow(WINDOW_NPC_TRADE))
				g_pUIManager->SetPosition(WINDOW_VOLUME, XiahInput::g_ptMouse.x - 100, XiahInput::g_ptMouse.y);   // TAP_NPC_TRADE_4 와 충돌 방지를 위해
			else
				g_pUIManager->SetPosition(WINDOW_VOLUME, XiahInput::g_ptMouse.x - 300, XiahInput::g_ptMouse.y);

			g_pUIManager->SetFocus(WINDOW_VOLUME);
			g_pUIManager->SetFocus(WINDOW_VOLUME, volume_window_edit);

			switch(m_byUsageVolumFrame)
			{
			case 1:
			case 3:
				{
					g_pUIManager->SetString(WINDOW_VOLUME, volume_window_title_dummy, IDS_MONEY2);
				}
				break;
			case 2:
				{
					g_pUIManager->SetString(WINDOW_VOLUME, volume_window_edit, 10);
					g_pUIManager->SetString(WINDOW_VOLUME, volume_window_title_dummy, IDS_AMOUNT);
				}
				break;
			}
		}
		break;
	case WINDOW_MONEY:
		{
			g_pUIManager->SetPosition(WINDOW_MONEY, XiahInput::g_ptMouse.x - 300, XiahInput::g_ptMouse.y);
			g_pUIManager->SetFocus(WINDOW_MONEY);
			g_pUIManager->SetFocus(WINDOW_MONEY, money_window_edit);
		}
		break;
	case WINDOW_OUTSIDE:	// 외공
		{
			g_pUIManager->SetData(WINDOW_OUTSIDE, outside_attack_mode_button_01, CURRENT_INDEX, -1);
			g_pUIManager->SetData(WINDOW_OUTSIDE, outside_attack_mode_button_02, CURRENT_INDEX, -1);
			g_pUIManager->SetData(WINDOW_OUTSIDE, outside_attack_mode_button_03, CURRENT_INDEX, -1);

			int nCtrlID = outside_attack_mode_button_03;

			switch(g_info.m_bSafeMode)
			{
			case 0:
				nCtrlID = outside_attack_mode_button_01;
				break;
			case 1:
				nCtrlID = outside_attack_mode_button_02;
				break;
			case 2:
				nCtrlID = outside_attack_mode_button_03;
				break;
			}

			g_pUIManager->SetData(WINDOW_OUTSIDE,  nCtrlID, CURRENT_INDEX, 2);
		}
		break;

	case WINDOW_NAME_CONFER:
		{
			if(g_pUIManager->IsShow(WINDOW_CONNECTION_INFO))
				g_pUIManager->Hide(WINDOW_CONNECTION_INFO);

			g_pUIManager->SetFocus(WINDOW_NAME_CONFER);
			g_pUIManager->SetFocus(WINDOW_NAME_CONFER, name_window_edit);
		}
		break;
	case WINDOW_CONNECTION_INFO:
		{
			if(g_pUIManager->IsShow(WINDOW_NAME_CONFER))
				g_pUIManager->Hide(WINDOW_NAME_CONFER);
		}
		break;
	case WINDOW_QUEST_01:
		{
			if(m_pQuest)
			{
				g_pUIManager->SetData(WINDOW_QUEST_01, quest_window_01_scrollbar1, SCROLL_MOVE, 0);
				m_pQuest->Refresh();
			}
				
		}		
		break;
	// 단 경험치 분배
	case WINDOW_DAN_NEW:
		{
			if(m_pRelation)
			{
				if(m_pRelation->m_byExpDivision == 1)
				{
					g_pUIManager->SetData(WINDOW_DAN_NEW, window_dan_new_select_button1, STATICLIST_SHOW, 0);
					g_pUIManager->SetData(WINDOW_DAN_NEW, window_dan_new_select_button2, STATICLIST_SHOW, 1);
				}
				else
				{
					g_pUIManager->SetData(WINDOW_DAN_NEW, window_dan_new_select_button1, STATICLIST_SHOW, 1);
					g_pUIManager->SetData(WINDOW_DAN_NEW, window_dan_new_select_button2, STATICLIST_SHOW, 0);
				}

				if(m_pRelation->m_byFEDivision == 1)
				{
					g_pUIManager->SetData(WINDOW_DAN_NEW, window_dan_new_select_button3, STATICLIST_SHOW, 0);
					g_pUIManager->SetData(WINDOW_DAN_NEW, window_dan_new_select_button4, STATICLIST_SHOW, 1);
				}
				else
				{
					g_pUIManager->SetData(WINDOW_DAN_NEW, window_dan_new_select_button3, STATICLIST_SHOW, 1);
					g_pUIManager->SetData(WINDOW_DAN_NEW, window_dan_new_select_button4, STATICLIST_SHOW, 0);
				}
			}
		}
		break;

	case WINDOW_OPTION_01:
		{

#define SetControlIndex(a,b) if( g_info_Temp.##b == 1)  g_pUIManager->SetData(WINDOW_OPTION_01, a, STATICLIST_SHOW, 0); \
								else g_pUIManager->SetData(WINDOW_OPTION_01, a, STATICLIST_SHOW, 1);

			SetControlIndex( option_window_1_select_01,m_bAllowWhisper);
			SetControlIndex( option_window_1_select_02,m_bAllowRelation);
			SetControlIndex( option_window_1_select_03,m_bAllowTrade);
			SetControlIndex( option_window_1_select_04,m_bHideChat);
			SetControlIndex( option_window_1_select_05,m_bShowNickname);
			SetControlIndex( option_window_1_select_06,m_bShowNPCname);
			SetControlIndex( option_window_1_select_07,m_bItemDropChoice); //HO_0816_07 아이템 드랍시 횅땍
		}
		break;
	case WINDOW_OPTION_02:
		{
			int VD = g_info_Temp.m_fViewDistance;
			int PD = g_info_Temp.m_fPolygonDetail;
			int BV = g_info_Temp.m_dwBGMVolume;
			int FV = g_info_Temp.m_dwFXVolume;

			g_pUIManager->SetData(WINDOW_OPTION_02, option_window_2_scroll_01, SCROLL_MOVE, VD);
			g_pUIManager->SetData(WINDOW_OPTION_02, option_window_2_scroll_02, SCROLL_MOVE, PD);

			g_pUIManager->SetString(WINDOW_OPTION_02, option_window_2_scroll_dummy_03, VD);
			g_pUIManager->SetString(WINDOW_OPTION_02, option_window_2_scroll_dummy_04, PD);			

			g_pUIManager->SetData(WINDOW_OPTION_02, option_window_2_scroll_03, SCROLL_MOVE, BV);
			g_pUIManager->SetData(WINDOW_OPTION_02, option_window_2_scroll_04, SCROLL_MOVE, FV);

			g_pUIManager->SetString(WINDOW_OPTION_02, option_window_2_scroll_dummy_07, BV);
			g_pUIManager->SetString(WINDOW_OPTION_02, option_window_2_scroll_dummy_08, FV);
		}
		break;
	case WINDOW_OPTION_03:
		{
			TCHAR strMoney[32] = {0,};
			_stprintf(strMoney, IDS_MONEY, MoneyCommaStr(g_info_Temp.m_dwBuyLimit).data());

			g_pUIManager->SetString(WINDOW_OPTION_03, option_window_3_money_dummy, strMoney, 8);
			g_pUIManager->SetString(WINDOW_OPTION_03, window_option_3_sell_dummy_01, g_info_Temp.m_bRarityLimit);
			g_pUIManager->SetString(WINDOW_OPTION_03, window_option_3_sell_dummy_02, g_info_Temp.m_bStxTypeLimit);
		}
		break;
	}
}

void CharacterInfo::CloseFrame( int nFrameID)
{	
	if(g_pUIManager->IsShow(nFrameID))
	{
		PlayInterfaceSound( ISOUND_WINDOW_CLOSE);
		g_pUIManager->Hide(nFrameID);
	}

	switch( nFrameID)
	{
	case WINDOW_VOLUME:
		{
			g_pUIManager->SetString(WINDOW_VOLUME, volume_window_edit, 1);
		}		
		break;
	case WINDOW_MONEY:
		{
			g_pUIManager->SetString(WINDOW_MONEY, money_window_edit, 0);
		}
		break;
	case WINDOW_MUNPA:
		{
			if(g_pUIManager->IsShow(WINDOW_CONNECTION_INFO))
				g_MainCharInfo.CloseFrame( WINDOW_CONNECTION_INFO);

			if(g_pUIManager->IsShow(WINDOW_NAME_CONFER))
				g_MainCharInfo.CloseFrame( WINDOW_NAME_CONFER);
		}		
		break;
	case WINDOW_MUNPA_BBS_LIST:
		{
			if(m_pListClient)
				delete m_pListClient, m_pListClient = NULL;	
		}		
		break;
	case WINDOW_CONNECTION_INFO:
		{
			if(g_pUIManager->IsShow(NAME_CHANNEL))
				g_pUIManager->Hide(NAME_CHANNEL);
		}
		break;
	//HO_0410_07 상서령 가이드 업데이트 : 상서령 가이드 창이 종료 될때 초기화 하여줌
	case WINDOW_HELPER_LIST:
		{
			if(g_pUIManager->IsShow(WINDOW_HELPER_LIST))//탐랑 스크립트가 초기화 되지 않도록함
			g_Helper.SetSelectID(0);
		}
	//HO_0413_07 퀵 가이드 업데이트 : 퀵 가이드 창이 종료 될때 m_wLevel를 체크 하여 헬프 버튼을 띄어줌
	case WINDOW_HELPER_LIST0:
		{		
			if(!g_pUIManager->IsShow(HELP_BUTTON) && g_MainCharInfo.m_wLevel < 11)
				g_pUIManager->Show(HELP_BUTTON);			
		}
		break;
	}

}

void CharacterInfo::HideAllFrame()
{
	g_pUIManager->Show(MAIN_FRAME);
	g_pUIManager->Show(SMALL_MESSENGER);
	g_pUIManager->Show(MAIN_CHAT);
	g_pUIManager->Show(SITUATION_BRA);
	g_pUIManager->Show(DATA_WINDOW);
}

#define CheckControl(a,b,c) if( g_pUIManager->IsMouseOn(c, a)) { \
	if( g_pUIManager->GetData(c, a, GET_CURRENT_INDEX) == 1) { \
	g_pUIManager->SetData(c, a, CURRENT_INDEX, 0); \
	b = 1;}\
	  else { \
	  g_pUIManager->SetData(c, a, CURRENT_INDEX, 1);  \
	  b = 0; }}

void CharacterInfo::CheckOptionFrame()
{
	if(g_pUIManager->IsShow(WINDOW_OPTION_01))
	{
		CheckControl( option_window_1_select_01, g_info_Temp.m_bAllowWhisper,	WINDOW_OPTION_01);
		CheckControl( option_window_1_select_02, g_info_Temp.m_bAllowRelation,	WINDOW_OPTION_01);
		CheckControl( option_window_1_select_03, g_info_Temp.m_bAllowTrade,		WINDOW_OPTION_01);
		CheckControl( option_window_1_select_04, g_info_Temp.m_bHideChat,		WINDOW_OPTION_01);
		CheckControl( option_window_1_select_05, g_info_Temp.m_bShowNickname,	WINDOW_OPTION_01);
		CheckControl( option_window_1_select_06, g_info_Temp.m_bShowNPCname,	WINDOW_OPTION_01);
		CheckControl( option_window_1_select_07, g_info_Temp.m_bItemDropChoice,	WINDOW_OPTION_01); //HO_0816_07 아이템 드랍시 횅땍
	}
	else if(g_pUIManager->IsShow(WINDOW_OPTION_03))
	{
		sRect Rect;
		g_pUIManager->GetRegionData(WINDOW_OPTION_03, option_window_3_money_dummy2, Rect);

		if(Rect.PtInRect(XiahInput::g_ptMouse))
		{
			g_MainCharInfo.OpenFrame(WINDOW_VOLUME, 3);
		}
	}
}

#define CheckControl2(a,b,c,d) if( g_pUIManager->IsMouseOn(c, a)) { d = 1;\
	if( g_pUIManager->GetData(c, a, GET_CURRENT_INDEX) == 1) { \
	g_pUIManager->SetData(c, a, CURRENT_INDEX, 0); \
	b = 1;}\
else { \
	g_pUIManager->SetData(c, a, CURRENT_INDEX, 1);  \
	b = 0; }}

#define CheckControl3(a,b,c) if( g_pUIManager->IsMouseOn(c, a)) { \
	if( g_pUIManager->GetData(c, a, GET_CURRENT_INDEX) == 1) { \
	g_pUIManager->SetData(c, a, CURRENT_INDEX, 0); \
	b = a;}}

void CharacterInfo::CheckChatShowType()
{
	if(g_pUIManager->IsShow(MAIN_CHAT))
	{
		bool bPass = false;

		CheckControl2(main_chat_select_01, m_bNormalChatShow, MAIN_CHAT, bPass);
		CheckControl2(main_chat_select_02, g_info.m_bAllowWhisper, MAIN_CHAT, bPass);
		CheckControl2(main_chat_select_03, m_bMunpaChatShow, MAIN_CHAT, bPass);
		CheckControl2(main_chat_select_04, m_bDanChatShow, MAIN_CHAT, bPass);

		CheckControl2(main_chat_mode_select_01, m_bChatModeAction, MAIN_CHAT, bPass);

		if(bPass)
			return;

		int nChatType = -1;		

		CheckControl3(main_chat_channel_select_01, nChatType, MAIN_CHAT);
		CheckControl3(main_chat_channel_select_02, nChatType, MAIN_CHAT);
		CheckControl3(main_chat_channel_select_03, nChatType, MAIN_CHAT);
		CheckControl3(main_chat_channel_select_04, nChatType, MAIN_CHAT);

		if(nChatType != -1)
		{
			for(register int i=0; i < 4; ++i)
			{
				if(main_chat_channel_select_01-i != nChatType)
					g_pUIManager->SetData(MAIN_CHAT, main_chat_channel_select_01-i, CURRENT_INDEX, 1);
			}

			switch(nChatType)
			{
			case main_chat_channel_select_01:
				m_bySendChatType = 0;
				break;
			case main_chat_channel_select_02:
				m_bySendChatType = 1;
				break;
			case main_chat_channel_select_03:
				m_bySendChatType = 6;
				break;
			case main_chat_channel_select_04:
				m_bySendChatType = 5;
				break;
			}
		}
	} // if(g_pUIManager->IsShow(MAIN_CHAT))

	// 전서구 Send 리스트 버튼
	if(g_pUIManager->IsShow(WINDOW_MAIL_SELECT))
		g_Mail.Check_SendList();

}