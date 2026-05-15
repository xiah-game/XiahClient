#pragma once

#include "XiahGame_StepObject.h"
#include "XiahGameObject.h"
#include "CharacterInfo.h"

//extern void RegisterAllNetworkHandler_Intro();

#define CLASS_MAX				2	// create시 max class
#define CHARACTER_MAX			3	// 사용자가 가질 수 있는 max char

#define	XIAH_CHAR_MAX			4	// 검영, 연랑, 무투, 야차. 캐릭터 4개.

#define INTROMENU_CREATECHAR	1
#define INTROMENU_SELECTCHAR	2

/////////////////////////////////////////////////////////////////////
//	INTRO RELATED CLASS
/////////////////////////////////////////////////////////////////////

class XiahGame_Intro
	: public CXiahGame_StepObject
{
public:
	XiahGame_Intro();
	virtual ~XiahGame_Intro();


	void Init_Clear();
	void Init_MainFrame();
	void Init_SystemButton();
	void Init_WindowCharacter();
	void Init_WindowOutSide();
	void Init_WindowInSide();
	void Init_WindowFiveElement();			// 오행
	void Init_WindowSkill();		// 각성

private:
	// frame initialize
	void Init_Frames();

	void Init_IntroAccount();
	void Init_IntroButtonSet();
	void Init_IntroCharaterSelect();
	void Init_IntroWindow();

	void Init_MiniMapWindow();
	void Init_DateWindow();	
	void Init_WindowButton();	
	void Init_ChatFrame();
	//void Init_MessengerChannel();
	void Init_PetControlFrame();	
	void Init_WindowItem();	
	void Init_WindowDan();
	void Init_WindowClanFound();
	void Init_WindowClan();
	void Init_WindowTaming();
	void Init_WindowTamingItem();

	void Init_WindowPcTrade();
	void Init_WindowNpcTrade();
	void Init_WindowNpcTradeTab();
	void Init_WindowConvert();
	void Init_WindowVolume();
	void Init_WindowConnectionInfo();
	void Init_WindowNameConfer();
	void Init_NameChannel();
	void Init_WindowQuest();

	void Init_WindowOption();
	void Init_WindowClose();

	void Init_WindowPcStore();				// 개인 상점
	void Init_WindowMoney();				// 돈 입력 인터페이스

	void Init_WindowGakMessage();			// 각적
	void Init_WindowDongsin();				// 동신적

	void Init_WindowDanWar();				// 단 비무
	void Init_WindowPurse();				// 단 전낭

	void Init_WindowMunpaBBSTop();
	void Init_WindowMunpaBBSList();
	void Init_WindowMunpaBBSRead();
	void Init_WindowMunpaBBSWrite();
	void Init_WindowMunpaDonate();
	void Init_WindowMunpaWarPetition();
	void Init_WindowMark();					// 문파마크 (다른것 사용가능)

	void Init_WindowMail();
	void Init_WindowMailSelect();
	void Init_WindowMailResult();

	void Init_WindowCommon();
	void Init_WindowPetTrade();

	void Init_WindowBokNumber();			// 복권번호선택
	void Init_WindowBokPrize();				// 복권당첨번호
	void Init_WindowSmelt();				// 조합창
	void Init_WindowFiveElementConvert();	// 오행 제련

	void Init_WindowHelperList();			// 대화 리스트
	void Init_WindowHelperScript();			// 대화창

	void Init_WindowRecovery();				// 아이템 복구

	void Init_WindowDanNew();				// 단 경험치 분배
	void Init_WindowPortal();				// NPC 포탈 이동

	void Init_WindowCollection();			// 아이템 수집	

	void Init_WindowSecretMove();			//HT_0313 : 광명전 & 천황전	(이동)
	void Init_WindowSecretApplication();	//HT_0313 : 광명전 & 천황전	(참여)
	void Init_WindowSecretBasis();	
public:

	// render
	BOOL Update();
	BOOL Render();

	HRESULT InitIntro( BYTE byCurrentMenu);	// CHARACTERLIST_ACK() 받고
	void	SetCurrentMenu( BYTE byCurrentMenu) { m_byCurrentMenu = byCurrentMenu;};
	BYTE	GetCurrentMenu() { return m_byCurrentMenu;};
	HRESULT	GoToMenuCreateChar();
	HRESULT	GoToMenuSelectChar();
	HRESULT ClearIntro();

	void	CreateCharacterData();
	void	DeleteCharacterData();

	// Create Character
	void	SetCurrentClass( BYTE byIndex);
	void	SetCurrentClassInfo( BYTE byCurrentClassIndex);
	BYTE	GetCurrentClassIndex() { return m_byCurrentClassIndex;};

	// Select Character
	void	SetCharCount( BYTE byCharCount) { m_byCharCount = byCharCount;};		// CHARACTERLIS_ACK(), NEWCHARACTER_ACK() 받고
	void	SetCurrentCharIndex( BYTE byIndex) { m_byCurrentCharIndex = byIndex;};	// 캐릭터 선택할때
	void	SetCurrentCharInfo( BYTE byIndex);										// 캐릭터 선택할때
	void	SetCurrentCharInfoToScr();												// 캐릭터 선택할때
	BYTE	GetCharCount() { return m_byCharCount;};
	BYTE	GetCurrentCharIndex() { return m_byCurrentCharIndex;};
	CharacterInfo*	GetCurrentChar() { return m_CharacterList[ m_byCurrentCharIndex];};

	void	CreateInitCharData();
	bool	m_bIntroInitCharDataCreated;	// 인트로에서 사용할 캐릭터 정보를 생성했는지.

	//
	CharacterInfo*	m_CharacterList[CHARACTER_MAX];

	// Intro메뉴에서 사용되는 캐릭터 객체
//	XiahGameEngine::CCharRender m_CharRender;
	CXiahCharObject				m_CharRender[ CHARACTER_MAX ];
//	Matrix4x4					m_CharTM;

    bool	m_bIntroBGMPlay;

	BYTE	m_byOnMouseCharIndex;		// 온 마우스 캐릭터의 이름을 보여주기 위해서.

	// 캐릭터 선택할때 필요한 변수들.
	bool			m_bCanSelectCharacter;	// 현재 캐릭터를 지우고자 할때엔 다른 캐릭터를 못 고른다. 씨... 
	bool			m_bFirstCharSelect;
	bool			m_bCharacterSelected;
	int				m_nCharacterSelectStep;

	enum	eCharacterSelectType
	{
		eCST_Click,
		eCST_ZoomIn,
		eCST_Stay,
		eCST_ZoomOut
	};

	float			m_fYAngleWhenClicked;
	BOOL			m_bMoveCameraRight;
	float			m_fCameraRotateYAngle;
	float			m_fCameraTargetYAngle;

	// 캐릭터 만들기 할때 필요한 변수들
	CXiahCharObject			m_GyumYung;
	CXiahCharObject			m_YunRang;
	CXiahCharObject			m_MooToo;
	CXiahCharObject			m_YaCha;

	enum	eCharacterCreateType
	{
		eCCType_GyumYung,
		eCCType_GyumYung_GoLong,
		eCCType_GyumYung_TurnArcLeft1,
		eCCType_GyumYung_TurnArcLeft2,
		eCCType_GyumYung_GoXDir,
		eCCType_GyumYung_TurnBackToChar1,
		eCCType_GyumYung_TurnBackToChar2,
		eCCType_GyumYung_Rotate,

		eCCType_YunRang
	};

	DWORD			m_dwCameraMoveStartTime;
	int				m_nCharacterCreateStep;
	bool			m_bOri, m_bFirst;
	float			m_fAddHeight;
	int				m_nCurrentCharacter;	// 1 : 검영, 2 : 연랑, 3 : 무투
	bool			m_bCharacterCreatePreLoaded;

//private:
	// Create Character
	BYTE			m_byCurrentClassIndex;

	// Select Character
	BYTE			m_byCharCount;
	BYTE			m_byCurrentCharIndex;
	BYTE			m_byCurrentMenu;

	// 게임 시작할때 로딩화면 보이게.
	bool	m_bFirstGameLoadScreen;
	bool	m_bIntroFirstCall;	// 이녀석이 있어야 처음에 게임 로딩 화면이 보인다.
	BYTE	m_byIntroFirstCallCurMenu;

};

extern XiahGame_Intro* g_pIntro;

