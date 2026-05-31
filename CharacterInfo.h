#pragma once

#include "XiahObjectType.h"
#include "Mugong.h"
#include "chat.h"
#include "ScreenMessage.h"
#include "cimagescrmsg.h"
#include "Sack.h"
#include "Slot.h"
#include "Relation.h"
#include "HoldItem.h"
#include "Quest.h"
#include ".\listclient.h"

#define VISUALID_NUM	9

typedef std::list<DWORD> DWORDLIST;

struct sKEEPUPMUGONGICONLIST
{
	WORD		m_MugongID;
	DWORD		m_CurTime;
	bool		m_DrawIcon;
	BYTE		m_MugongLevel;
};

/////////////////////////////////////////////////////////////////////
//	INTRO RELATED CHARACTER INFOMATION CLASS
/////////////////////////////////////////////////////////////////////
/**
 * \ingroup XiahClient
 *
 * \date 2004-07-15
 *
 * 게임 진행시 본인 캐릭터와 관련된 정보 저장
 */
class CharacterInfo
{
public:
	int m_nChatType;

public:
	// 인트로에 쓰이는 변수들
	DWORD	m_dwMapID;
	DWORD	m_dwObjectID;
	DWORD	m_dwXiahObjectID;
	DWORD	m_dwBirthDate;
	BYTE	m_bCharType;
	WORD	m_wLevel;
	DWORD	m_dwHpCur;
	DWORD	m_dwHpMax;
	DWORD	m_wIpCur;
	DWORD	m_wIpMax;
	WORD	m_wVit;
	WORD	m_wStr;
	WORD	m_wSus;
	WORD	m_wDex;
	
	sString m_szNickName;
	sString	m_szMunpaName;
	sString	m_szOrderName;	
	sString szMapName;
	WORD	wEquipVisualID[VISUALID_NUM];
	BYTE	m_bRarity[VISUALID_NUM];
	BYTE	m_bStxType[VISUALID_NUM];
	
	BYTE	m_bAuction;				// 옥션
	BYTE	m_bRebirth;				// 각성자

	// 게임내에서만 쓰이는 변수들
	BYTE	m_bIncrStr;
	BYTE	m_bIncrSus;
	BYTE	m_bIncrDex;
	BYTE	m_bIncrVit;
	INT64	m_i64Exp;
	INT64	m_i64LevelExp;
	INT64	m_i64NextLevelUpExp;
	INT64	m_i64TpExp;
	INT64	m_i64NextTpUpExp;
	DWORD	m_dwTotalSp;
	WORD	m_wRemainSp;
	DWORD	m_dwTotalTp;
	WORD	m_wRemainTp;
	WORD	m_wBaseAtkPwr;
	DWORD	m_dwTotalAtkPower;
	WORD	m_wBaseDefPwr;
	DWORD	m_dwTotalDefPower;
	WORD	m_wBaseAttackRating;
	DWORD	m_dwTotalAttackRating;
	WORD	m_wAttackRange;
	BYTE	m_bWalkSpeed;
	BYTE	m_bRunSpeed;
	BYTE	m_bPlusSpeed;
	BYTE	m_bJumpLevel;
	DWORD	m_dwPkCnt;
	BYTE	m_bObjectType;

	DWORD	m_dwMoney;

	WORD	m_wCritical;
	BYTE	m_bAttackSpeed;
	DWORD	m_dwFame;
	WORD	m_wFiveElmPoint;		// 보유 오행 포인트
	DWORD	m_dwFiveElmPower;		// 오행 숙련도
	DWORD	m_dwFiveElmPowerMax;	// 오행 숙력도 최대치
	DWORD	m_dwFiveElmGauge;		// 오행 필살기
	WORD	m_wFiveElmExp[5];		// 오행별 수치

	float	m_fPosX;
	float	m_fPosY;
	BYTE	m_bHeight;
	WORD	m_wDirection;
	BYTE	m_bState;
	BOOL	m_IsStarted;
	BOOL	m_bIsPvPMap;

	BYTE	m_ReairSackID;
	BYTE	m_RpairItemPos;

	DWORD	m_dwResItemID;
	bool	m_bReairItemUse;

	bool	m_bFirstChat;

	int		m_nTempValue; // 각종 여러곳에서 사용할것

	sString m_strWhisperName;

	// 2004.07.02 Changth
	BOOL	m_bMainCharDie;
	BOOL	m_bMainCharMapMoveItemUse;	// 이형부, 동신주, 동신적을 사용하여 이동중이다.

	DWORD	m_dwTaxMunpaMoney;


	// 캐릭터가 가지는 부수적인 데이타들
	BYTE					m_byMySackCurrIdx;

	CMugong*				m_pMugong;	
	CRelation*				m_pRelation;
	CHoldItem*				m_pHoldItem;
	CChat*					m_pChat;
	CScreenMessage*			m_pScrMsg;
	CScreenMessage*			m_pHelpMsg;
	CScreenMessage*			m_pSpecialChatMsg;
	CImageScrMsg*			m_pImageScrMsg;
	CImageScrMsg*			m_pPremiumItem;		//HT_0122 : 기간제 프리미엄 아이템 추가

	CSack*					m_pMySack[3];
	CSack*					m_pEquipSack;
	CSack*					m_pNpcSack;
	CSack*					m_pDepositSack;
	CSack*					m_pPcSackMine;
	CSack*					m_pPcSackOther;
	CSack*					m_pModifySack;
	CSack*					m_pItemMallSack;	// Item mall 용
	CSlot*					m_pSlot;			// 퀵 슬롯
	CSack*					m_pCollection;		// 아이템 수집

	// 개인 상점 관련
	CSack*					m_pPersonalTradeSet;
	CSack*					m_pPersonalTradeSell; 
	DWORD					m_dwTradeMoney;  // 개인 상점 총 판매금액
	
	CSack*					m_pSmeltSack;	// 조합
	CSack*					m_pFEConvert;	// 오행 아이템 제련
	CSack*					m_pQuickMart;	// 매품패
	
	CIPopUp*				m_pToolTip;			// item tool tip
	CQuest*					m_pQuest;

	DWORD					m_dwPickedObject;	// 캐릭터가 pick 한 넘(pet,func npc)
	DWORD					m_dwPickedNpc;		// 캐릭터가 pick 한 넘(npc)
	DWORD					m_dwAskID;			// 나를 pick한 캐릭터(나한테 뭔가를 요청하는 캐릭터)	
	DWORD					m_dwAskPartyID;		// 나를 피킹한 파티(단) 아이디 (단전투)
	DWORD					m_dwBetMoney;		// 단전투 내기 금액

	BOOL					m_bTradeAgree;
	DWORD					m_dwMoneyOnTradeMine;
	DWORD					m_dwMoneyOnTradeOther;
	BYTE					m_bySendChatType;
	BYTE					m_byUsageVolumFrame;

	DWORD					m_dwCurrentSelectedBongInItem;
	DWORD					m_dwVolumeSplitAmount;
	BYTE					m_bSackCnt;
	int                     m_nVIPLevel = 0;

	BOOL					m_bDragFrame;		// Frame이 drag 될지
	BOOL					m_bShowMiniMap;		// 미니맵 Show 여부
	BOOL					m_bRotateMinimap;
	BOOL					m_bChangMinimap;
	BOOL					m_bShowCharNames;
	BOOL					m_bInteractionFlag;	// ^^*

	bool					m_bPersonalTradeSell; // 개인상점 판매 유무
	BYTE					m_bPickType;  // 캐릭터 선택한 유형
	
	bool					m_bChatModeAction;
	bool					m_bCharChange;

	bool					m_bNormalChatShow;
	bool					m_bMunpaChatShow;
	bool					m_bDanChatShow;

	bool					m_bPortalMove;
	bool					m_bEvSocketItemUse;

	bool					m_bMark;

	// 경공.
	bool					m_bFastMove;
	int						m_nFastIndex;

	// 2004.05.14 Changth : 전낭용
	BYTE	m_byPurseAction;		// 전낭에 돈 넣을때, 1 이면 입금이고, 2 이면 출금이다.
	BYTE	m_byPurseSackID;
	BYTE	m_byPurseSackPos;
	DWORD	m_dwPurseItemID;

	// 2004.05.28 사부, 연인 관계
	BYTE	m_byRelationType;

	// 하나의 인터페이스를 같이 쓰기위한 방법으로, 
	// 1 이면 인연 만들때고, 
	// 2 이면 인연 끓을때이다.
	// 3 이면 인연 끓는 것을 강행할때 쓴다.
	BYTE	m_byRelationStep;

	CListClient *m_pListClient;

	// 문파 비석의 서버 아이디를 저장해 놓고 충돌 처리할때 사용한다.
	DWORDLIST	m_MunpaStonIDList;

	/////////////////////////////////////////////////////////////////////////////////////////////////////
	// 임시
	DWORD m_dwMunjuID, m_dwStonID, m_dwStoneMapID;	
	DWORD m_dwMunpaFame;
	DWORD m_dwRanking, m_dwCanBattleTime, m_dwBattleTime, m_dwTotalWar, m_dwWinWar, m_dwDrawWar, m_dwLossWar;
	BYTE m_bStoneChannelID;
	BYTE m_bLevel, m_bWar;	
	sString m_strEnemyMunpaName, m_strFriendMunpaName, m_strNotice, m_strMunjuName;
	/////////////////////////////////////////////////////////////////////////////////////////////////////

	int					m_nLastUseItemXPos;
	int					m_nLastUseItemYPos;

	DWORD				m_dwReserveMoney;				// 각종 금액 예약 변수
	DWORD				m_dwReserveID;					// 각종 ID 예약 변수

	// 거래사고 방지
	DWORD				m_dwBuyLimit;				// 구매제한금액
	BYTE				m_bRarityLimit;				// 판매제한 +
	BYTE				m_bStxTypeLimit;			// 판매제한 성

	// 빠른 구입
	BYTE				m_byLastSackPos;			// 오른쪽 버튼 클릭한 아이템 위치
	DWORD				m_dwLastClickTime;			// 오른쪽 버튼 마지막 클릭한 시간

	// 복구
	std::vector<DWORD>		m_vRecoveryItem;
	std::vector<sString>	m_vRecoveryItemName;

	// 기
	BYTE m_bStaminaCnt;
	bool m_bSpirit;

	bool m_bReairItemUse2;
	bool m_bMunpaFight;		// 문파대전 상태 (마혈 쟁탈전)

	DWORD m_dwLordMunpaID;	// 우승 문파 ID

	//HT_0911 : 프리미엄 퀘스트 수련치
	DWORD m_dwPremiumTP;
	DWORD m_dwPremiumSP;

	//HT_1023 : 운영자 마크 추가
	DWORD m_dwGameMasterMark;

	//HT_CHEAT : 치트 키와 오토 팔기 키들
	BOOL m_bCheat;
	BOOL m_bAutoSell;
	BYTE m_bySellPos;
	BYTE m_bySellSackPos;

	BYTE m_byCheatTime; //범위 무공 시간
	WORD m_wPosX;
	WORD m_wPosY;

	//HT_0403 : 지속형 무공 시전 아이콘
	std::vector<sKEEPUPMUGONGICONLIST*>	m_vkeepUpMugongIconList;
	std::vector<sKEEPUPMUGONGICONLIST*>	m_vkeepUpPetMugongIconList;

	//HO_0413_07 : 퀵 가이드 업데이트	
	BYTE m_bQuickIndex;

public:

	CharacterInfo();
	~CharacterInfo();

	void operator=(CharacterInfo* data);

	void DeleteAll();
	void Create();
	void Update();

	void UpdateInput();

	inline void CheckSpeedPing();
	inline void UpdateScreen();
	inline void CheckButtonDown();

	void Render();
	inline void RenderScreen();
	inline void RenderSack();

	void Clear();

	// Chat
	void CreateChat();
	void DeleteChat();
	// ScrMsg
	void CreateScrMsg();
	void DeleteScrMsg();
	// Mugong
	void CreateMugong();
	void DeleteMugong();	
	// Sack
	void CreateSack();
	void DeleteSack();	
	void CheckSackItemSelected();
	BOOL CheckSackItemUnSelected();	
	// HoldItem
	void CreateHoldItem();
	void DeleteHoldItem();
	// Slot
	void CreateSlot();
	void DeleteSlot();
	// Relation
	void CreateRelation();
	void DeleteRelation();
	// Quest
	void CreateQuest();
	void DeleteQuest();
	// Etc
	void CheckOptionFrame();

	void CheckChatShowType();

	void ShowMiniMap( BOOL bFlag = FALSE);
	void RotateMiniMap();
	void ShowHelpMessage( LPCTSTR szText, BYTE byColorType=0);
	void SpecialChatMessage( LPCTSTR szText, BYTE byColorType=0);

	void DeleteAllScrMessage(void);
	void PlayInterfaceSound( int nSoundID);
	void PlayInterfaceSoundWithVol( int nSoundID,Vector3 vec);
	sString FindNameByID( DWORD dwCharID);
	DWORD   FindIDByName(LPCTSTR name);
	void SetCurrSendChatType( BYTE byType) { m_bySendChatType = byType;};
	BYTE GetCurrSendChatType(){ return m_bySendChatType;};
	void ShowSack( BYTE bySackType);
	void HideSack( BYTE bySackType, BOOL bFlagForModifySack = TRUE);
	void OpenFrame( int nFrameID, BYTE byUsageVolumFrame=0);
	void CloseFrame( int nFrameID);
	void HideAllFrame();

	// 캐릭터 관련 프레임 업데이트
	void RefreshFramePos();
	void RefreshChracterInfo();
	void RefreshPetInfo();
	void RefreshMainFrame();
	void RefreshChatFrame( DWORD sender, BYTE type, sString content, sString senderName, DWORD listner, sString listnerName);
	void RefreshItemFrame();
	void RefreshMugongFrame( BOOL bSmallChange = FALSE);
	void RefreshSituation();
	void RefreshTime1( WORD wYear, BYTE bMonth, BYTE bDay, BYTE bHour);
	void RefreshTime2();
	void RefreshQuest();
	// 오행
	void RefreshFiveElement();
	
	//금전 단위 색
	int MoneyUnitColor(DWORD dwMoney);
	
};

extern CharacterInfo g_MainCharInfo;