#include "precompile.h"
#include "XiahCheatConfig.h"
#include "resource.h"
#include "io.h"
#include "AppData.h"
#include "XIahNetworkHandler.h"
#include "XiahSocket.h"
#include "XiahGameMain.h"
#include "InterfaceHandler.h"
#include "InterfaceDefine.h"
#include "CharacterInfo.h"
#include "XiahArrayIndex.h"

#include "XiahEnvInfo.h"
#include "xiahbgm.h"
#include "XiahGame_Handler_Sender.h"
#include "XiahObject.h"
#include "XiahGame_BGM.h"
#include "XiahGame_StepObject.h"
#include "XiahMap.h"
#include "XiahGameObject.h"	
#include "XiahCamera.h"
#include "XiahBGMcore.h"
#include "sysutil.h"
#include "CurseFilter.h"
#include "XiahDebug.h"

#include <time.h>
/*
	MainWindow.CPP

	Window를 생성하고 Message Pump를 동작 시킨다
*/

//using namespace XiahGameEngine;
sXiahGameEngine_CreateInfo g_info;
sXiahGameEngine_CreateInfo g_info_Temp;

//HT_CHEAT : WINDOWSIZE
static	int g_Old_Resx;
static	int g_Old_Resy;
static	int g_Old_Resfreq;
static	int g_Old_Color;
static	bool g_Changed_Resoultion = false;

//int	WIDTHDEFAULT	=	1024;
//int HEIGHTDEFAULT	=	768;
int	WIDTHDEFAULT	=	1600;
int HEIGHTDEFAULT	=	1200;

bool g_IsFocus = true;
#define strAppName "XiahClient"

extern BOOL InitInstance(HINSTANCE hInstance,LPTSTR lpCmdLine);
extern BOOL ExitInstance();
extern BOOL Run();
extern BOOL MainLoop();
extern LRESULT CALLBACK MainWindowProc(HWND hWnd,UINT uMsg,WPARAM wParam,LPARAM lParam);
extern BOOL CheckInstance(LPCSTR appName);
extern LRESULT ProcessXiahWindowMessage(UINT uMsg,WPARAM wParam,LPARAM lParam);
extern BOOL ScreenShot();

int GetSetttingInfo();
void GetSystemInfomation(void);
BOOL ParseInfo_From_Launcher(void);

BOOL ChangeResoultion()
{
	return TRUE;
}
// 윈도우창모드전환.. 

//HT_CHEAT : WINDOWSIZE
void RestoreResoultion()
{
	if(g_Changed_Resoultion == false) return;

	DEVMODE DevMode;
	memset(&DevMode, 0, sizeof(DEVMODE));
	DevMode.dmSize  = sizeof(DEVMODE);

	DevMode.dmPelsHeight = g_Old_Resx;   // Y Resolution
	DevMode.dmPanningWidth = g_Old_Resy;
	DevMode.dmDisplayFrequency = g_Old_Resfreq ;
	DevMode.dmBitsPerPel = g_Old_Color;

	DevMode.dmFields = DM_PELSWIDTH | DM_PELSHEIGHT | DM_BITSPERPEL | DM_DISPLAYFREQUENCY;

	if( g_info.m_bFullscreen )
        long re = ChangeDisplaySettings(&DevMode, CDS_FULLSCREEN);
	else
		long re = ChangeDisplaySettings(&DevMode, 0);
}


//----------------------------------------------------------------------------------------------------------------------
BOOL InitInstance( HINSTANCE hInstance,LPTSTR lpCmdLine)
{
	// 프로그램이 중복되어 실행되고 있는 지를 검사
	if( !CheckInstance( strAppName))
	{
		MessageBox( GetForegroundWindow(), LoadStr( IDS_APP_ERROR_ALREADYEXIST), LoadStr( IDS_APP_NAME), MB_OK | MB_ICONERROR);
		return FALSE;
	}

	//----------------------- Create Main Window

	WNDCLASS wndclass;

	ZeroMemory( &wndclass, sizeof( WNDCLASS));
	wndclass.style = CS_HREDRAW | CS_VREDRAW;
	wndclass.hInstance = hInstance;
	wndclass.hIcon = NULL;
	wndclass.hCursor = NULL;
	wndclass.hbrBackground = NULL;
	wndclass.lpszClassName = strAppName;
	wndclass.lpfnWndProc = MainWindowProc;

	// Window Class 등록
	if( ::RegisterClass( &wndclass) == 0)
		return FALSE;
 
//	// Window 생성
//#ifdef MINI
//	HWND hWnd = ::CreateWindowEx( 0, strAppName, strAppName, WS_OVERLAPPEDWINDOW
//		, 0, 0, G_WIDTH, G_HEIGHT, NULL, NULL, hInstance, NULL);
//#else
//	HWND hWnd = ::CreateWindowEx( 0, strAppName, strAppName, WS_POPUP
//		, 0, 0, 1024, 768, NULL, NULL, hInstance, NULL);
//#endif
//	if( hWnd == NULL)
//	{
//		MessageBox(0,"","윈도우 생성 실패",MB_OK);
//		DBG_LogFile( _T("윈도우 생성 실패"));
//		return FALSE;
//	}

	//HT_CHEAT : WINDOWSIZE
	if( ChangeResoultion() == FALSE )
		return FALSE;

	// Window 생성
	HWND hWnd = ::CreateWindowEx( WS_EX_APPWINDOW/*WS_EX_OVERLAPPEDWINDOW*/,
								  strAppName,
								  strAppName,
								  WS_POPUP | WS_VISIBLE,
								  0, 0,
                                  GetSystemMetrics(SM_CXSCREEN),
                                  GetSystemMetrics(SM_CYSCREEN),
								  NULL,
								  NULL,
								  hInstance,
								  NULL);

	if( hWnd == NULL)
	{
		MessageBox(0,"","윈도우 생성 실패",MB_OK);
		DBG_LogFile( _T("윈도우 생성 실패"));
		return FALSE;
	}
	
	//----------------------------
	// Application기본 데이터 설정

	g_AppData.m_hInstance			= hInstance;
	g_AppData.m_hWnd				= hWnd;
	g_AppData.m_strAppName			= strAppName;
	//HT_CHEAT : WINDOWSIZE
	g_AppData.m_bWireframe			= false;

	// 레지스트리에서 정보를 얻는다.
	GetSetttingInfo();

	SetCurrentDirectory(g_info.strInstallDir);


	// SYStem의 정보를 얻는다.
	GetSystemInfomation();
	// 런처로 부터 얻어온 인증서버의 정보 분석
	ParseInfo_From_Launcher();

	//	게임 엔진의 Loading
	g_info.m_hInstance	= hInstance;
	g_info.m_hWnd		= hWnd;
#ifdef MINI
	g_info.m_nWidth		= G_WIDTH;
	g_info.m_nHeight	= G_HEIGHT;
#else
	g_info.m_nWidth		= 1024;
	g_info.m_nHeight	= 768;
#endif

	// 욕설 필터 파일 로드
	if(LoadCurses(".\\cdata.dat") == false)
	{
		MessageBox(GetForegroundWindow(),"cdata.dat 파일을 찾을수 없습니다", "", MB_OK);
		return FALSE;
	}

	if( !InitializeXiahGameEngine( &g_info))
	{
		DBG_LogFile( _T("Game Engine 초기화 실패"));
		return FALSE;
	}

	// BGM SOUND ENGINE INIT
	IntializeXiahBGM(g_VolTbl[g_info.m_dwBGMVolume]);

	// Game Step 여기서 만들어야 첨부터 쓰지.
	InitGameStepObject();

	if( !InitXiahGame())
	{
		MessageBox(0,"","Xiah 게임 초기화 실패",MB_OK);
		DBG_LogFile( _T("Xiah 게임 초기화 실패"));
		return FALSE;
	}

	ShowWindow( hWnd, SW_SHOW);
	UpdateWindow( hWnd);

	return TRUE;
}


void GetSystemInfomation()
{
	// 시스템 정보를 얻는다.
	cSysutil	sysinfo;
	sysinfo.availableMemory();
	sysinfo.computeProcessorSpeed();
	sysinfo.querySystemInformation();

	/*
	#define	WINDOWS_95				0
	#define	WINDOWS_95_SR2			1
	#define	WINDOWS_98				3
	#define	WINDOWS_ME				4
	#define	WINDOWS_NT				5
	#define	WINDOWS_2K				6
	#define	WINDOWS_XP				7
	#define	WINDOWS_FUTURE			8
	#define	UNKNOWN					10
	*/
	unsigned long OS = sysinfo.GetFlatform();
	unsigned long CPUCLOCK = sysinfo.GetCpuClock();
	unsigned long Total_Memory  = sysinfo.GetTotalMem();
}

//----------------------------------------------------------------------------------------------------------------------
int Decode_Buffer(unsigned char *buffer,int len)
{
	for(int i=0;i<len;i++)
	{
		if(buffer[i] != 0x9 && buffer[i] != 0x0a)
		{
			buffer[i] = buffer[i] ^ 0x1f;
		}
	}
	return 0;
}

// 从 Launcher 的命令行参数中解析服务端连接信息和客户端版本号
// Launcher 传递格式: -auth IP:Port -ver 版本号
// 如果没有命令行参数（直接双击启动），弹窗提示并拒绝启动
// 特殊处理：VS 调试模式（IsDebuggerPresent）下自动使用默认参数
BOOL ParseInfo_From_Launcher(void)
{
	LPTSTR lpCmd = GetCommandLine();

	// 查找 -auth 参数
	char* pAuth = strstr(lpCmd, "-auth");

	// 调试器模式：VS F5 启动时没有命令行参数，自动使用本地默认值
	if (pAuth == NULL && IsDebuggerPresent()) {
		g_AppData.m_NumAuthserver = 1;
		g_AppData.m_AuthServer[0].m_ServerAddress.printf("127.0.0.1");
		g_AppData.m_AuthServer[0].m_ServerPort = 9001;
		g_info.m_version = 1081;
		return TRUE;
	}

	// 非调试模式：没有 -auth 参数则拒绝启动
	if (pAuth == NULL) {
		MessageBoxW(GetForegroundWindow(), L"\x8BF7\x4ECE\x767B\x5F55\x5668(XiahLauncher)\x542F\x52A8\x6E38\x620F\n\nPlease launch the game from XiahLauncher.", L"\x542F\x52A8\x9519\x8BEF", MB_OK | MB_ICONERROR);
		return FALSE;
	}

	// 解析 -auth IP:Port
	char szAuthArg[256] = {0};
	if (sscanf(pAuth, "-auth %255s", szAuthArg) == 1) {
		// 拆分 IP 和 Port
		char szIP[128] = {0};
		int nPort = 9001;
		char* pColon = strchr(szAuthArg, ':');
		if (pColon) {
			int ipLen = (int)(pColon - szAuthArg);
			if (ipLen > 0 && ipLen < 128) {
				strncpy(szIP, szAuthArg, ipLen);
				szIP[ipLen] = '\0';
			}
			nPort = atoi(pColon + 1);
		} else {
			strncpy(szIP, szAuthArg, 127);
		}

		g_AppData.m_NumAuthserver = 1;
		g_AppData.m_AuthServer[0].m_ServerAddress.printf("%s", szIP);
		g_AppData.m_AuthServer[0].m_ServerPort = nPort;
	}

	// 解析 -ver 版本号
	char* pVer = strstr(lpCmd, "-ver");
	if (pVer) {
		int nVer = 0;
		if (sscanf(pVer, "-ver %d", &nVer) == 1 && nVer > 0) {
			g_info.m_version = nVer;
		}
	}

	return TRUE;
}

// 레지스트리로 부터 정보를 읽어 들인다
int GetSetttingInfo(void)
{
	
	
	

	char szExePath[MAX_PATH]; GetModuleFileName(NULL, szExePath, MAX_PATH); char* pLastSlash = strrchr(szExePath, '\\'); if (pLastSlash) *(pLastSlash + 1) = '\0'; strcpy(g_info.strInstallDir, szExePath); char szIniFile[MAX_PATH]; strcpy(szIniFile, g_info.strInstallDir); strcat(szIniFile, "xiah.ini");

	g_info.m_bRGB16 = GetPrivateProfileInt("CONFIG", "DC", 1, szIniFile) == 0;

	int dwValue = GetPrivateProfileInt("CONFIG", "VD", 5, szIniFile);
	if(dwValue > 9) dwValue = 9; else if(dwValue < 0) dwValue = 0;
	g_info.m_fViewDistance = (float)dwValue;

	dwValue = GetPrivateProfileInt("CONFIG", "PD", 9, szIniFile);
	g_info.m_fPolygonDetail = (float)dwValue;
	DWORD dwEffectValue = (9-dwValue) * 60.0f;
	g_EffectManager.SetVertexBufferRenewTime( dwEffectValue );

	g_info.m_nTextureDetail = GetPrivateProfileInt("CONFIG", "TD", 3, szIniFile);
	g_info.m_dwGamepad = GetPrivateProfileInt("CONFIG", "GP", 0, szIniFile);
	g_info.m_bHideChat = GetPrivateProfileInt("CONFIG", "HC", 1, szIniFile);
	g_info.m_bShowNickname = GetPrivateProfileInt("CONFIG", "SN", 1, szIniFile);
	g_info.m_bShowNPCname = GetPrivateProfileInt("CONFIG", "SM", 1, szIniFile);
	g_info.m_bItemDropChoice = GetPrivateProfileInt("CONFIG", "ID", 1, szIniFile);

	dwValue = GetPrivateProfileInt("CONFIG", "BV", 10, szIniFile);
	if(dwValue > 10) dwValue = 10;
	g_info.m_dwBGMVolume = dwValue;

	dwValue = GetPrivateProfileInt("CONFIG", "EV", 10, szIniFile);
	if(dwValue > 10) dwValue = 10;
	g_info.m_dwFXVolume = dwValue;

#ifndef MASTER
	dwValue = GetPrivateProfileInt("CONFIG", "WX", 0, szIniFile);
	if(dwValue == 1) g_info.m_bFullscreen = FALSE;
	else g_info.m_bFullscreen = TRUE;
#else
	g_info.m_bFullscreen = TRUE;
#endif

	// 版本号已从 Launcher 命令行注入，不再从 ini 读取
	// g_info.m_version 由 ParseInfo_From_Launcher() 设置

	LoadCheatConfig();

	return 0;
}


//----------------------------------------------------------------------------------------------------------------------
// XiahClient가 중복되어서 실행되고 있는지 검사
// 또하나가 이미 실행중이면 FALSE를 리턴한다
BOOL CheckInstance(LPCSTR App)
{
	// 允许客户端多开：跳过单实例信号量检查
	(void)App;
	return TRUE;
}

//----------------------------------------------------------------------------------------------------------------------
BOOL ExitInstance()
{
	// 각종 Client모듈 Release
	// 게임 데이터 Release
	
	// INDEX FREE
	XiahArrayIndex::UnLoadXiahArrayIndex();

	UnloadCurses();	// 욕설 제거

	//HT_CHEAT : WINDOWSIZE
	RestoreResoultion();

	/*
		게임 엔진의 Release
	*/
	CloseXiahGame();
	// UNINIT BGM ENGINE
	UninitializeXiahBGM();
	UninitializeXiahGameEngine();


	return TRUE;
}

//----------------------------------------------------------------------------------------------------------------------
BOOL Run()
{	
	MSG msg;
	BOOL bExit = FALSE;

	while( !bExit)
	{
		// Window 메세지 처리
		while( ::PeekMessage( &msg, 0, 0, 0, PM_NOREMOVE))
		{
			if( ::GetMessage( &msg, 0, 0, 0))
			{
				::TranslateMessage( &msg);
				::DispatchMessage( &msg);
			}
			else
			{
				bExit = TRUE;
				break;
			}
		}

//#ifndef MASTER
		//if( XiahGameEngine::m_bGameEnd ) break;
		
		//if( g_cExceptionReport.m_bGameEnd )
		//{
		//	bExit = TRUE;
		//	break;
		//}
//#endif

		if( bExit ) break;
		
		// Xiah Client의 MainLoop호출
		if( !MainLoop())
			break;
	}

	return TRUE;
}

//----------------------------------------------------------------------------------------------------------------------
#if TEST_PERFORMANCE
	extern BOOL bLog;
#endif

extern BOOL g_bScreenShot;

BOOL MainLoop()
{
	//if( GetFocus() != g_AppData.m_hWnd)
		//Sleep( 150);
	
	// 현재 app 상태가 화면에 보이는지 검사
	HRESULT hr = g_pDirect3DDevice->TestCooperativeLevel();
	if( hr == D3DERR_DEVICELOST ) return TRUE;
	if( hr == D3DERR_DEVICENOTRESET )
	{
		hr = g_pDirect3DDevice->Reset( &g_D3DPresent );
		if( hr != S_OK )
			return TRUE;
	}

	g_Device.Clear();

	ClearScene(g_XiahEnvInfo.m_FogColor);
	BeginScene();

	XiahGameEngine::UpdateXiahGameEngine();

#ifndef MASTER
	DWORD d = timeGetTime();
#endif
	XiahNetwork::ProcessNetworkMessage();
	BOOL bRender = LoopXiahGame_BeforeRender();	
#ifndef MASTER
	DWORD d2 = timeGetTime();
	g_AppData.dwTime = d2 - d;
#endif

#ifndef MASTER
	d = timeGetTime();
#endif
	if( bRender)
	{
		LoopXiahGame();

		// FOR SCREEN SHOT
		if( g_bScreenShot)
		{	
			ScreenShot();
			g_bScreenShot = FALSE;
		}
	}
#ifndef MASTER
	d2 = timeGetTime();
	g_AppData.dwTime2 = d2 - d;
#endif

	EndScene();
	PresentScene();
	// GAME SOUND FX PLAY
	LoopXiahGameFX();

#ifdef _DEBUG	// T_T
	//if( GetFocus() != g_AppData.m_hWnd)
		//Sleep( 100);
#endif
	return TRUE;
}

// 이게 Alt + Tab 및 기타 여러가지를 막는 녀석이다.
LRESULT CALLBACK LowLevelKeyboardProc(int nCode, WPARAM wParam, LPARAM lParam)
{
	BOOL fEatKeystroke = FALSE;
	if (nCode == HC_ACTION) 
	{
		switch (wParam) 
		{
		case WM_KEYDOWN:  case WM_SYSKEYDOWN:
		case WM_KEYUP:    case WM_SYSKEYUP: 
			PKBDLLHOOKSTRUCT p = (PKBDLLHOOKSTRUCT) lParam;
			fEatKeystroke = 
				( (p->vkCode == VK_LWIN) ) || 
				( (p->vkCode == VK_RWIN) ) ||
				( (p->vkCode == VK_TAB) && ((p->flags & LLKHF_ALTDOWN) != 0) ) ||
				( (p->vkCode == VK_ESCAPE) && ((p->flags & LLKHF_ALTDOWN) != 0) ) ||
				( (p->vkCode == VK_ESCAPE) && ((GetKeyState(VK_CONTROL) & 0x8000) != 0) )||
				( (p->vkCode == VK_DELETE) && ((p->flags & LLKHF_ALTDOWN) != 0 ) && ( (GetKeyState(VK_CONTROL) & 0x8000) != 0) ||
				( (p->flags & LLKHF_ALTDOWN) != 0 && (p->vkCode == VK_F4))
				);
			//its possible to add other keys....
			//the 46 means del
			break;
		}
	}
	return(fEatKeystroke ? 1 : CallNextHookEx(NULL, nCode, wParam, lParam));
}

//----------------------------------------------------------------------------------------------------------------------
// XiahClient Window Procedure함수
LRESULT CALLBACK MainWindowProc(HWND hWnd,UINT uMsg,WPARAM wParam,LPARAM lParam)
{
	switch( uMsg)
	{

#ifdef MASTER
	case WM_ACTIVATEAPP:	// 프로그램이 비활성화되면 자동종료한다.
		if( wParam == FALSE )
			PostQuitMessage( 0);
		break;
#else
	case WM_ACTIVATEAPP:
		{
			g_IsFocus = (wParam == 1 ? true : false);
		}
		break;
#endif

	case WM_DESTROY:	// 이걸 않보내주면 Message Pumping을 멈출 수 없다
		PostQuitMessage( 0);
		break;
	case WM_XIAH_INTERFACE_MESSAGE:
		return ProcessInterfaceMessage( wParam, lParam);
	case WM_LBUTTONDOWN:
	case WM_LBUTTONUP:
	case WM_RBUTTONDOWN:
	case WM_RBUTTONUP:
	case WM_MOUSEMOVE:
	case WM_SETCURSOR:
	case WM_MOUSEWHEEL:
#ifdef MASTER
		ProcessXiahWindowMessage( uMsg, wParam, lParam);
#else
		if(g_IsFocus) ProcessXiahWindowMessage( uMsg, wParam, lParam);
#endif
		break;

	//HT_CHEAT : WINDOWSIZE
	case WM_SIZE:
	case WM_MOVE:
		if( !g_info.m_bFullscreen )
		{
            RECT rcWindow;
            GetClientRect( hWnd, &rcWindow );
			XiahGameEngine::SetWindowRect( &rcWindow );
		}
		break;

/*
	case WM_SIZE :
			XiahGameEngine::ChangeXiahGameEngine(LOWORD(lParam),HIWORD(lParam));
		break;
*/
	case WM_SYSCHAR:
	case WM_SYSKEYDOWN:
	case WM_KEYDOWN:
	case WM_CHAR:
	case WM_IME_COMPOSITION:
	case WM_IME_NOTIFY:
		{
			g_pUIManager && g_pUIManager->ProcessIME( hWnd, uMsg, wParam, lParam);
			ProcessXiahWindowMessage( uMsg, wParam, lParam);

			// 조합중인 문자 출력
			if(g_pUIManager)
			{
				if(g_GameWork.m_GameStep_0 != GAMESTEP_INTRO)
				{
					TCHAR strSrcCanText[MAX_STRING];
					memset(strSrcCanText, 0, MAX_STRING);
			
					g_pUIManager->GetString(MAIN_CHAT, main_chat_edit, strSrcCanText, GET_CAN_STRING);

					if(strSrcCanText[0] != 0)
					{
						TCHAR strCanText[MAX_STRING];

						TCHAR *pStrCanText = strCanText;
						TCHAR *pStrSrcCanText = strSrcCanText;
						TCHAR *strNum = {"1:2:3:4:5:6:7:8:9:"};

						memset( strCanText, 0, MAX_STRING);

						int nStrLen = strlen(strSrcCanText)/2;

						for(int i=1; i <= nStrLen; i++)
						{
							strncat(pStrCanText,strNum,2);
							pStrCanText+=2, strNum+=2;
							strncat(pStrCanText,pStrSrcCanText,2);
							pStrCanText+=2,	pStrSrcCanText+=2;
							strcat(pStrCanText," ");
							pStrCanText++;
						}

						g_pUIManager->Show(SMALL_MESSENGER, small_messenger_mixture);
						g_pUIManager->SetString(SMALL_MESSENGER, small_messenger_mixture, strCanText, 3);

						g_pUIManager->Show(LARGE_MESSENGER, large_messenger_mixture);
						g_pUIManager->SetString(LARGE_MESSENGER, large_messenger_mixture, strCanText, 3);
					}
					else
					{
						g_pUIManager->Hide(SMALL_MESSENGER, small_messenger_mixture);
						g_pUIManager->SetString(SMALL_MESSENGER, small_messenger_mixture, "");

						g_pUIManager->Hide(LARGE_MESSENGER, large_messenger_mixture);
						g_pUIManager->SetString(LARGE_MESSENGER, large_messenger_mixture, "");
					}
				} // if(g_GameWork.m_GameStep_0 != GAMESTEP_INTRO)
			} // if(g_pUIManager)

			// 한문이나 특수문자 선택창 막기
			if(uMsg == WM_IME_NOTIFY && IMN_OPENCANDIDATE == wParam )
				return 1;
		}
		break;
	case WM_PAINT:
		{
			HDC hdc = GetDC( g_AppData.m_hWnd);
			ReleaseDC( g_AppData.m_hWnd, hdc);
		}
		break;

	// 한글 조합창 막기
	case WM_IME_STARTCOMPOSITION:
		return 1;
		break;
}
	return DefWindowProc( hWnd, uMsg, wParam, lParam);	// Default Window Message Procedure호출
}

void Save_Option(bool bSend)
{
	// 서버에 옵션 저장
	if(bSend)
	{
		BYTE val1 = g_info.m_bAllowWhisper == TRUE ? 1:0;
		BYTE val2 = g_info.m_bAllowRelation == TRUE ? 1:0;
		BYTE val3 = g_info.m_bAllowTrade == TRUE ? 1:0;

		// 옵션 거래
		g_MainCharInfo.m_dwBuyLimit		= g_info.m_dwBuyLimit;
		g_MainCharInfo.m_bRarityLimit	= g_info.m_bRarityLimit;
		g_MainCharInfo.m_bStxTypeLimit	= g_info.m_bStxTypeLimit;

		SendCS_OP_CHANGE_REQ(g_pMainChar->m_dwServerID, val1, val2, val3, g_info.m_bSafeMode,
							g_info.m_dwBuyLimit, g_info.m_bRarityLimit, g_info.m_bStxTypeLimit);	// Whisper
	}	

	HKEY hKey;
	if( RegOpenKeyEx (HKEY_LOCAL_MACHINE, _T("Software\\Xiah Online"), 0, KEY_ALL_ACCESS, &hKey) == ERROR_SUCCESS)
	{
		DWORD dwLen = sizeof( DWORD);
		DWORD dwType = REG_DWORD;

		// 배경음 볼륨
		DWORD dwValue = g_info.m_dwBGMVolume;
		long r = RegSetValueEx (hKey, _T("BV"), NULL, dwType, (BYTE*)&dwValue, dwLen); 
		// 이펙트 볼륨
		dwValue = g_info.m_dwFXVolume;
		r = RegSetValueEx (hKey, _T("EV"), NULL, dwType, (BYTE*)&dwValue, dwLen); 

		// 아이템 드랍 여부 //HO_0816_07 아이템 드랍시 횅땍
		dwValue = g_info.m_bItemDropChoice;
		r = RegSetValueEx (hKey, _T("ID"), NULL, dwType, (BYTE*)&dwValue, dwLen); 

		// 대화메세지 숨기기
		dwValue = g_info.m_bHideChat;
		r = RegSetValueEx (hKey, _T("HC"), NULL, dwType, (BYTE*)&dwValue, dwLen); 
		// 케렉터 별호보기
		dwValue = g_info.m_bShowNickname;
		r = RegSetValueEx (hKey, _T("SN"), NULL, dwType, (BYTE*)&dwValue, dwLen); 
		// 몬스터 이름 보기
		dwValue = g_info.m_bShowNPCname;
		r = RegSetValueEx (hKey, _T("SM"), NULL, dwType, (BYTE*)&dwValue, dwLen); 

		// View Distance
		dwValue = g_info.m_fViewDistance;
		r = RegSetValueEx (hKey, _T("VD"), NULL, dwType, (BYTE*)&dwValue, dwLen); 

		// Polygon Detail
		dwValue = g_info.m_fPolygonDetail;
		r = RegSetValueEx (hKey, _T("PD"), NULL, dwType, (BYTE*)&dwValue, dwLen); 

		DWORD dwEffectValue = (9-dwValue) * 60.0f;
		g_EffectManager.SetVertexBufferRenewTime( dwEffectValue );

		// 거래 옵션
		/*
		// 구매제한
		dwValue = g_info.m_dwBuyLimit;
		r = RegSetValueEx (hKey, _T("BL"), NULL, dwType, (BYTE*)&dwValue, dwLen); 

		// + 판매 제한
		dwValue = g_info.m_bRarityLimit;
		r = RegSetValueEx (hKey, _T("RL"), NULL, dwType, (BYTE*)&dwValue, dwLen); 

		// 성 판매 제한
		dwValue = g_info.m_bStxTypeLimit;
		r = RegSetValueEx (hKey, _T("SL"), NULL, dwType, (BYTE*)&dwValue, dwLen); 
		*/

		RegCloseKey( hKey );
	}

	// BGM 볼륨 조절
	SetBGMVolume(XiahGameEngine::g_VolTbl[g_info.m_dwBGMVolume]);
	ChangeXiahGameOption(&g_info);

	g_XiahEnvInfo.m_CameraBoundSize = 128 + (1024 - 128) * (g_info.m_fViewDistance / 10.0f);
	g_XiahCamera.m_bNeedUpdate = TRUE;
	XiahMap::g_XiahMap.Update();
}


BOOL ScreenShot()
{
	IDirect3DSurface9* pBackBuffer = NULL;
	HRESULT hr = g_pDirect3DDevice->GetBackBuffer( 0, 0, D3DBACKBUFFER_TYPE_MONO, &pBackBuffer);

	if(D3D_OK != hr)
	{
		// TODO: 메시지 출력

		return FALSE;
	}

	sString filename;
	int number = 0;
		
	//HT_0926 ( 스샷에 날짜 찍히게 하기 )
/////////////////////////////////////
	time_t t = time(NULL);
	struct tm* lt = localtime(&t);
	sString str;
	CText2D tScreenShot;

	// tm_year는 1900년부터 세기 시작한 년도입니다.
	// tm_hour는 0시부터 23시까지 24시간제입니다.
	// tm_wday를 사용하면 요일을 알 수 있습니다.
	//   0 - 6, 0 일요일, 1 월요일, ...
	// 기타 tm_yday, tm_isdst등도 있습니다.
	
	str.printf("ScreenShot Date : %4d/%02d/%02d Time : %02d/%02d/%02d", 
		lt->tm_year + 1900, lt->tm_mon + 1, lt->tm_mday, 
		lt->tm_hour, lt->tm_min, lt->tm_sec);
	tScreenShot.SetText(300, 35, str, GetFont(IDS_DUDUM, 14), D3DCOLOR_XRGB(255, 255, 0), 15);
	tScreenShot.Render();
////////////////////////////////////

	CreateDirectory(".\\screenshot",NULL);

	_finddata_t find_data;
	int handle = _findfirst( ".\\screenshot\\*.jpg", &find_data);

	if( handle != -1)
	{
		while(1)
		{
			sscanf( find_data.name, ".\\screenshot\\screenshot%03d.jpg", &number);

			++number; // += 1;

			if( _findnext( handle, &find_data) == -1)
				break;
		}

		_findclose( handle);
	}

	filename.printf(_T(".\\screenshot\\screenshot%03d.jpg"), number);

	// 이전 저장방식의 컬러 포맷에서 문제인것 같다.
	// D3D 함수로 대체 하자
	hr = D3DXSaveSurfaceToFile((char*)filename.data(), D3DXIFF_JPG, pBackBuffer, NULL, NULL);
	pBackBuffer->Release();
	tScreenShot.Release();

	if(D3D_OK != hr)
	{		
		return FALSE;
	}

	return TRUE;
}
// [崩溃转储] 自动生成 .dmp 文件，配合 XiahClient.pdb 用 WinDbg 分析崩溃堆栈
#include <DbgHelp.h>
#pragma comment(lib, "dbghelp.lib")

static LONG WINAPI XiahCrashHandler(EXCEPTION_POINTERS* pExceptionInfo)
{
    // 生成带时间戳的文件名
    SYSTEMTIME st;
    GetLocalTime(&st);
    char szDumpFile[MAX_PATH];
    sprintf(szDumpFile, "XiahCrash_%04d%02d%02d_%02d%02d%02d.dmp",
            st.wYear, st.wMonth, st.wDay, st.wHour, st.wMinute, st.wSecond);

    HANDLE hFile = CreateFileA(szDumpFile, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
    if (hFile != INVALID_HANDLE_VALUE)
    {
        MINIDUMP_EXCEPTION_INFORMATION mdei;
        mdei.ThreadId = GetCurrentThreadId();
        mdei.ExceptionPointers = pExceptionInfo;
        mdei.ClientPointers = FALSE;

        // MiniDumpWithDataSegs 包含全局变量数据，体积小且信息足够
        MiniDumpWriteDump(GetCurrentProcess(), GetCurrentProcessId(), hFile,
                          MiniDumpWithDataSegs, &mdei, NULL, NULL);
        CloseHandle(hFile);
    }

    // 写一行到日志方便快速确认
    FILE* fpCrash = fopen("Xiah.log", "a");
    if (fpCrash) {
        fprintf(fpCrash, "\n*** CRASH *** ExceptionCode=0x%08X Address=0x%p DumpFile=%s\n",
                pExceptionInfo->ExceptionRecord->ExceptionCode,
                pExceptionInfo->ExceptionRecord->ExceptionAddress,
                szDumpFile);
        fclose(fpCrash);
    }

    return EXCEPTION_EXECUTE_HANDLER;
}

int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPTSTR lpCmdLine, int nCmdShow)
{
	SetUnhandledExceptionFilter(XiahCrashHandler);

	FILE* fp;
	if ((fp = _tfopen(_T("Xiah.log"), _T("w"))) != NULL) fclose(fp);
	if ((fp = _tfopen(_T("Xiah_Close.log"), _T("w"))) != NULL) fclose(fp);
	if ((fp = _tfopen(_T("Xiah_TEST.log"), _T("w"))) != NULL) fclose(fp);
	if ((fp = _tfopen(_T("XiahClient_d3d9.log"), _T("w"))) != NULL) fclose(fp);
	if ((fp = _tfopen(_T("XiahClientd_d3d9.log"), _T("w"))) != NULL) fclose(fp);

	// 从 Launcher 命令行解析服务端地址和版本号，无参数则拒绝启动
	if (!ParseInfo_From_Launcher()) {
		return FALSE;
	}
	GetSetttingInfo();

	if ( !InitInstance(hInstance, lpCmdLine) )
	{
		return FALSE;
	}

	Run();

	ExitInstance();
	return 0;
}

