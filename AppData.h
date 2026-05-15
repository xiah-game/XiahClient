#pragma once
//---------------------------------------------------------------------------------------

#define MAX_AUTHSERVER	16

// 인증 서버들의 정보
struct sAUTHSERVERLIST 
{
	sString		m_ServerAddress;
	DWORD		m_ServerPort;
};

//---------------------------------------------------------------------------------------
// 기본 프로그램 데이터
struct sAPPData
{
	// [12/16/2004] 성인 서버

	HINSTANCE m_hInstance;			// Window Instance
	HWND	  m_hWnd;				// Window Handle
	sString   m_strAppName;			// Client프로그램 이름

	sString	  m_strUserName;		// 사용자 이름	
	sString	  m_strServerAddress;	// UNIT 서버 주소
	DWORD	  m_dwPort;				// UNIT 서버 포트
	DWORD	  m_dwKey;				// 인증 키

	// 월드와 채널 변수 제대로 사용하자!!! (처음에 월드를 채널로 사용되고 있었다.)
	BYTE	  m_byWorldID;			// 월드 아이디
	BYTE	  m_byChannelID;		// 채널 ID

	BYTE	  m_bAge;				// 나이
	BYTE	  m_bAdult;				// 성인 서버 (0-일반서버, 1-성인서버)

	DWORD			m_NumAuthserver;				// 인증 서버의 갯수
	sAUTHSERVERLIST m_AuthServer[MAX_AUTHSERVER];	// 인증 서버의 주소정보

	//HT_CHEAT : 와이어 화면 보이기 
	BOOL		m_bWireframe;
#ifndef MASTER
	DWORD dwTime;
	DWORD dwTime2;
#endif
};

extern sAPPData g_AppData;

//---------------------------------------------------------------------------------------
// 전반적인 게임 진행상의 데이터
// 가끔가다 보면 enum데이터를 배열 Index로 쓰다가 잘못되는 경우가 종종있다
enum eGameStep
{
	GAMESTEP_START_LOADING		= 0,
	GAMESTEP_LOGIN,
	GAMESTEP_INTRO,
	GAMESTEP_GAME,

	GAMESTEP_COUNT,
};

#define DEF_POS( pre) int  (##pre_x);\
					  int  (##pre_y);
					  

struct sGameWorkData
{
	//-----------------------------------------------------	
	int		m_GameStep_0;	// 최상위 Step
	int		m_GameStep_1;	// 나머지는 각각 내부적으로 쓰기위해
	int		m_GameStep_2;
	int		m_GameStep_3;

	//-----------------------------------------------------

	int		  m_nNavigationMode;		// 0 마우스 , 1 키보드	, 2 게임패드
};

extern sGameWorkData g_GameWork;

#define SET_GAMESTEP( step) g_GameWork.m_GameStep_0 = step;\
							g_GameWork.m_GameStep_1	= 0;\
							g_GameWork.m_GameStep_2 = 0;\
							g_GameWork.m_GameStep_3 = 0;

//---------------------------------------------------------------------------------------
// Resource String Table에서 String을 얻어 온다
inline sString LoadStr(UINT id)
{
	sString str;

	str.resize( 255);

	::LoadString( g_AppData.m_hInstance, id, (LPTSTR)(LPCTSTR)str, (int)str.size());

	return str;
}
