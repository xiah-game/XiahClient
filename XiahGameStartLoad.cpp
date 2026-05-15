#include "precompile.h"
#include "XiahGameStartLoad.h"
#include "Fade.h"
#include "XiahCursor.h"
#include "XiahArrayIndex.h"
#include "XIahCharAniType.h"
#include "InterfaceDefine.h"
#include "InterfaceHandler.h"
#include "resource.h"
#include "XiahEnvInfo.h"
#include "AppData.h"
#include "XiahGameMain.h"
#include "XiahNetworkHandler.h"
#include "XiahSocket.h"
#include "CharacterInfo.h"
#include "xiahbgmcore.h"


#pragma comment(lib, "d3dx9.lib")

// 로고의 사운드
#define SOUNDFX_LOGO1	50001214
#define SOUNDFX_LOGO2	50001215
#define LOGO1			50001038
#define LOGO2			50001039
#define	LOGO3_CHINA		50001317	// 중국용 경고메세지

CXiahGameStartLoad*			g_StartLoad = NULL;

//////////////////////////////////////////////////////////////////////////////////
CXiahGameStartLoad::CXiahGameStartLoad()
{
	g_StartLoad = this;

	m_pVB1 = NULL;
	m_pVB2 = NULL;
	m_pTexture1 = NULL;
	m_pTexture2 = NULL;
	m_pTexture3 = NULL;
	m_byCurStep = eStartLoad_Start;
	m_dwElapsedTime = 0;
}

CXiahGameStartLoad::~CXiahGameStartLoad()
{
	Release();
}

BOOL CXiahGameStartLoad::Release()
{
	if( m_pVB1 )
	{
		m_pVB1->Release();
		m_pVB1 = NULL;
	}
	if( m_pVB2 )
	{
		m_pVB2->Release();
		m_pVB2 = NULL;
	}

	m_pTexture1 = NULL;
	m_pTexture2 = NULL;
	m_pTexture3 = NULL;

	return TRUE;
}

BOOL CXiahGameStartLoad::Init()
{
	SetCursor( NULL );

	// vertex buffer
	g_pDirect3DDevice->CreateVertexBuffer( 4 * sizeof( VT_TLVertex),
					0, D3DFVF_TLVERTEX, D3DPOOL_MANAGED, &m_pVB1, NULL);

	VT_TLVertex* pVertex = NULL;
	m_pVB1->Lock( 0, 0, (void**)&pVertex, 0 );

	pVertex[0].pos = Vector4( (float)162-0.5f, (float)534-0.5f, 0, 1 );
	pVertex[1].pos = Vector4( (float)162-0.5f, (float)234-0.5f, 0, 1 );
	pVertex[2].pos = Vector4( (float)862-0.5f, (float)534-0.5f, 0, 1 );
	pVertex[3].pos = Vector4( (float)862-0.5f, (float)234-0.5f, 0, 1 );

	pVertex[0].tex = Vector2( 0, 1 );
	pVertex[1].tex = Vector2( 0, 0 );
	pVertex[2].tex = Vector2( 1, 1 );
	pVertex[3].tex = Vector2( 1, 0 );

	for(int i=0; i<4; i++)
        pVertex[i].diffuse = D3DCOLOR_XRGB( 255, 255, 255 );

	m_pVB1->Unlock();

	// vertex buffer
	g_pDirect3DDevice->CreateVertexBuffer( 4 * sizeof( VT_TLVertex),
					0, D3DFVF_TLVERTEX, D3DPOOL_MANAGED, &m_pVB2, NULL);

	m_pVB2->Lock( 0, 0, (void**)&pVertex, 0 );

	pVertex[0].pos = Vector4( (float)212-0.5f, (float)534-0.5f, 0, 1 );
	pVertex[1].pos = Vector4( (float)212-0.5f, (float)234-0.5f, 0, 1 );
	pVertex[2].pos = Vector4( (float)812-0.5f, (float)534-0.5f, 0, 1 );
	pVertex[3].pos = Vector4( (float)812-0.5f, (float)234-0.5f, 0, 1 );

	pVertex[0].tex = Vector2( 0, 1 );
	pVertex[1].tex = Vector2( 0, 0 );
	pVertex[2].tex = Vector2( 1, 1 );
	pVertex[3].tex = Vector2( 1, 0 );

	for(int i=0; i<4; i++)
        pVertex[i].diffuse = D3DCOLOR_XRGB( 255, 255, 255 );

	m_pVB2->Unlock();

	// texture
	// 여기서 텍스쳐를 단독으로 읽자.
	int nPakFileCount = 1;

	LPCTSTR pPakFileList[] =
	{
		_T("logo.XPK")
	};

	if( !XiahPak::InitializeXiahPak(pPakFileList,nPakFileCount))
	{
		MessageBox( GetForegroundWindow(), START_ERROR1, START_ERROR0, MB_OK);
		return FALSE;
	}

	m_pTexture1 = XiahPak::GetTexture( LOGO1, TRUE );
	m_pTexture2 = XiahPak::GetTexture( LOGO2, TRUE );

#ifdef _CHINA_
	m_pTexture3 = XiahPak::GetTexture( LOGO3_CHINA, TRUE );
#endif

	//D3DXCreateTextureFromFile( g_pDirect3DDevice, "1.tga", &m_pTexture1 );
	//D3DXCreateTextureFromFile( g_pDirect3DDevice, "2.tga", &m_pTexture2 );

	if( !m_pTexture1 || !m_pTexture2 )
		return FALSE;

	m_dwPrevTime = timeGetTime();

	return TRUE;
}

BOOL CXiahGameStartLoad::Update()
{
	static BOOL logo1 = FALSE;
	static BOOL logo2 = FALSE;

	// time
	DWORD dwCurTime = timeGetTime();

	m_dwElapsedTime += ( dwCurTime - m_dwPrevTime );
	m_dwPrevTime = dwCurTime;

	bool bFailToLoad = false;

	//
	switch( m_byCurStep )
	{
	case eStartLoad_Start:
		Fade::StartFade( 0, 0, NULL, 3000 );			// Fade out
		break;
	case eStartLoad_TaewoolLogoFadeOut:
		if(logo1 == FALSE )
		{
			g_MainCharInfo.PlayInterfaceSound(SOUNDFX_LOGO1);
			logo1 = TRUE;
		}

		if( m_dwElapsedTime >= 3000 )
		{
			Fade::StartFade( 0, TRUE, NULL, 1500 );		// Fade in
			m_byCurStep = eStartLoad_TaewoolLogoFadeIn;
		}

		if( GetAsyncKeyState( VK_ESCAPE ) < 0 )			// Esc키가 들어오면 스킵.
		{
			Fade::StartFade( 0, 0, NULL, 3000 );		// 일단 거멓게.

			m_byCurStep = eStartLoad_XiahLogoFadeIn;
			m_dwElapsedTime = 9200;
		}
		break;
	case eStartLoad_TaewoolLogoFadeIn:
		if( m_dwElapsedTime >= 4600 )
		{
			Fade::StartFade( 0, 0, NULL, 3000 );		// Fade out
			m_byCurStep = eStartLoad_XiahLogoFadeOut;
		}

		if( GetAsyncKeyState( VK_ESCAPE ) < 0 )			// Esc키가 들어오면 스킵.
		{
			Fade::StartFade( 0, 0, NULL, 500 );		// 일단 거멓게.

			m_byCurStep = eStartLoad_XiahLogoFadeIn;
			m_dwElapsedTime = 9200;
		}
		break;

	case eStartLoad_XiahLogoFadeOut:
		if(logo2 == FALSE && m_dwElapsedTime >= 5300)
		{
			g_MainCharInfo.PlayInterfaceSound(SOUNDFX_LOGO2);
			logo2 = TRUE;
		}

		if( m_dwElapsedTime >= 7700 )
		{
			Fade::StartFade( 0, TRUE, NULL, 1500 );		// Fade in
			m_byCurStep = eStartLoad_XiahLogoFadeIn;
		}

#ifdef _CHINA_
		// FOR Chinese version
		if( GetAsyncKeyState( VK_ESCAPE ) < 0 )			// Esc키가 들어오면 스킵.
		{
			Fade::StartFade( 0, 0, NULL, 500 );		// 일단 거멓게.

			m_byCurStep = eStartLoad_China_WarnningIn;
			m_dwElapsedTime = 9200;
		}
#else
		if( GetAsyncKeyState( VK_ESCAPE ) < 0 )			// Esc키가 들어오면 스킵.
		{
			Fade::StartFade( 0, 0, NULL, 3000 );		// 일단 거멓게.

			m_byCurStep = eStartLoad_XiahLogoFadeIn;
			m_dwElapsedTime = 9200;
		}
#endif
		break;

	case eStartLoad_XiahLogoFadeIn:
#ifdef _CHINA_
		// FOR Chinese version
		if( m_dwElapsedTime >= 9200 )
		{
			Fade::StartFade( 0, 0, NULL, 3000 );		// Fade out
			m_byCurStep = eStartLoad_China_WarnningIn;
		}

		if( GetAsyncKeyState( VK_ESCAPE ) < 0 )			// Esc키가 들어오면 스킵.
		{
			Fade::StartFade( 0, 0, NULL, 3000 );		// 일단 거멓게.

			m_byCurStep = eStartLoad_China_WarnningIn;
			m_dwElapsedTime = 9200;
		}
#else
		if( m_dwElapsedTime >= 9200 )
		{
			Fade::StartFade( 0, 0, NULL, 3000 );		// Fade out
			m_byCurStep = eStartLoad_XiahLogoFadeIn;
		}

		if( GetAsyncKeyState( VK_ESCAPE ) < 0 )			// Esc키가 들어오면 스킵.
		{
			Fade::StartFade( 0, 0, NULL, 3000 );		// 일단 거멓게.

			m_byCurStep = eStartLoad_XiahLogoFadeIn;
			m_dwElapsedTime = 9200;
		}
#endif
		break;

	///////////////////////////////////////
	// 중국용 경고 메세지 추가
	///////////////////////////////////////
	case eStartLoad_China_WarnningIn:
		if( m_dwElapsedTime >= 9200 )
		{
			Fade::StartFade( 0, 0, NULL, 3000 );		// Fade in
			m_byCurStep = eStartLoad_China_WarnningOut;
		}

		break;

	case eStartLoad_China_WarnningOut:
		if( m_dwElapsedTime >= 12500 )
		{
			Fade::StartFade( 0, TRUE, NULL, 3000 );		// 일단 거멓게.

			m_byCurStep = eStartLoad_China_WarnningOut;
		}

		break;
	///////////////////////////////////////
	// 중국용 경고 메세지 추가
	///////////////////////////////////////


	case eStartLoad_Load1:
		{	// 이제 실질적인 로딩을 시작하자.

			// 시작 배경음악
			Play_BGM(_T("sound\\bgm\\intro01.mp3"),1);

			if( !InitXiahCursor())
			{
				MessageBox( GetForegroundWindow(), START_ERROR2, START_ERROR0, MB_OK);
				bFailToLoad = true;
				break;
			}

			if( !XiahArrayIndex::LoadXiahArrayIndex())
			{
				MessageBox( GetForegroundWindow(), START_ERROR3, START_ERROR0, MB_OK);
				bFailToLoad = true;
				break;
			}

			if( !XiahAniType::InitializeLogicalAnimationType())
			{
				MessageBox( GetForegroundWindow(), START_ERROR3, START_ERROR0, MB_OK);
				bFailToLoad = true;
				break;
			}

			// Pak File List는 임시로
			int nPakFileCount = 0;
			LPCTSTR pPakFileList[512];
			FILE* fpPak = fopen("packages_xpk.txt", "r");
			if(fpPak) {
				char line[256];
				while(fgets(line, sizeof(line), fpPak)) {
					if (line[0] == '\n' || line[0] == '\r' || line[0] == '/' || line[0] == '#') continue;
					char* nl = strchr(line, '\n'); if(nl) *nl = 0;
					char* cr = strchr(line, '\r'); if(cr) *cr = 0;
					if(strlen(line) > 0) pPakFileList[nPakFileCount++] = _strdup(line);
				}
				fclose(fpPak);
			} else {
				MessageBox(GetForegroundWindow(), _T("packages_xpk.txt not found"), _T("Error"), MB_OK);
				bFailToLoad = true;
				break;
			}
			
			if( !XiahPak::InitializeXiahPak((LPCTSTR*)pPakFileList, nPakFileCount))
			{
				MessageBox( GetForegroundWindow(), START_ERROR5, START_ERROR3, MB_OK);
				bFailToLoad = true;
				break;
			}

			g_XiahEnvInfo.m_FogColor = D3DCOLOR_XRGB( 0, 0, 0 );

			// 인터페이스
			g_pUIManager = new XiahGameEngine::CUIManager();
			if(!g_pUIManager->FileLoad(LoadStr(IDS_DATA_FILE_XIAH_FMR), LoadStr(IDS_DATA_FILE_XIAH_CMR)))
			{
				bFailToLoad = true;
				MessageBox( GetForegroundWindow(), _T("인터페이스 로딩 실패"), START_ERROR0, MB_OK);
				break;
			}

			g_pUIManager->CloseAll();
			
			RegisterInterfaceHandler();

			// 2004.07.20 이벤트용 로딩화면
			/*
			if( rand() % 2 )
				g_pUIManager->ForwardShow( EVENT_LOADING_1 );
			else
				g_pUIManager->ForwardShow( EVENT_LOADING_2 );
			*/

			g_pUIManager->ForwardShow(LOADING_IMAGE3);; //HO_0702_07 등급표시 : 등급표시와 함게 스타트로딩과 게임로딩 부분이 동일 이미지로 처리된다.

			//등급표시 적용전 코드 나중에 지워 버리자 ..; 등급표시 전에는 나이 구분이 있엇다...
			//if(g_AppData.m_bAdult)
			//	g_pUIManager->ForwardShow(LOADING_IMAGE2);
			//else
			//	g_pUIManager->ForwardShow(LOADING_IMAGE);

			//g_pUIManager->ForwardShow(LOADING_IMAGE3);

			// 로딩 화면이 서서히 보이도록 한다.
			m_byCurStep = eStartLoad_LoadLogoFadeIn;
			m_dwBackTime = m_dwElapsedTime;
			Fade::StartFade( 0, 0, NULL, 2000 );
		}
		break;
	case eStartLoad_LoadLogoFadeIn:
		break;
	case eStartLoad_Load2:
		{
			// [12/13/2004] DB날라갔기에..
			// 캐릭터 정보
			int nXpcFileCount = 0;
			LPCTSTR pCharacterFileList[256];
			FILE* fpXpc = fopen("packages_xpc.txt", "r");
			if(fpXpc) {
				char line[256];
				while(fgets(line, sizeof(line), fpXpc)) {
					if (line[0] == '\n' || line[0] == '\r' || line[0] == '/' || line[0] == '#') continue;
					char* nl = strchr(line, '\n'); if(nl) *nl = 0;
					char* cr = strchr(line, '\r'); if(cr) *cr = 0;
					if(strlen(line) > 0) pCharacterFileList[nXpcFileCount++] = _strdup(line);
				}
				fclose(fpXpc);
			} else {
				MessageBox(GetForegroundWindow(), _T("packages_xpc.txt not found"), _T("Error"), MB_OK);
				bFailToLoad = true;
				break;
			}

			if( !XiahGameEngine::InitializeCharacter( (LPCTSTR*)pCharacterFileList, nXpcFileCount))
			{
				bFailToLoad = true;
				MessageBox( GetForegroundWindow(), START_ERROR6, START_ERROR0, MB_OK);
				break;
			}

			// [12/17/2004] DB 날라가서 사운드 따로 간다
			LPCTSTR pSoundPakList[] =
			{
				_T("sound\\character\\sound.xps"),
				_T("sound\\character\\sound2.xps")
			};

			if( !XiahGameEngine::XiahSoundPak::InitializeSoundPak( pSoundPakList, 2))
			{
				bFailToLoad = true;
				MessageBox( GetForegroundWindow(), START_ERROR7, START_ERROR0, MB_OK);
				break;
			}

			// Char Effect정보
			g_EffectManager.Initialize( g_pDirect3DDevice, 0, 0 );
			if( !g_EffectManager.LoadXiahEffectPackage(_T("character\\chareffect.xpe")))
			{
				MessageBox( GetForegroundWindow(), START_ERROR8, START_ERROR0, MB_OK);
				bFailToLoad = true;
				break;
			}

			// [12/10/2004] DB가 날라가서 이펙트파일을 따로 간다.
			if( !g_EffectManager.LoadXiahEffectPackage(_T("character\\chareffect2.xpe"), 2))
			{
				MessageBox( GetForegroundWindow(), START_ERROR8, START_ERROR0, MB_OK);
				bFailToLoad = true;
				break;
			}

			// [03/20/2007] DB고장으로 이펙트 새로 묶어서 처리한다.
			if( !g_EffectManager.LoadXiahEffectPackage(_T("character\\chareffect3.xpe"), 3))
			{
				MessageBox( GetForegroundWindow(), START_ERROR8, START_ERROR0, MB_OK);
				bFailToLoad = true;
				break;
			}

			// Tile Effect정보
			if( !g_EffectManager.LoadXiahTileEffectPackage(_T("map\\tileeffect.xpe")))
			{
				MessageBox( GetForegroundWindow(), START_ERROR9, START_ERROR0, MB_OK);
				bFailToLoad = true;
				break;
			}

			// Load Dummy Ani character and load extra effect
			ManageExtraEffect();

			// 맵 기본 정보
			LPCTSTR pMapeFileList[ 5] =
			{
				_T("map\\tile.idx"),
				_T("map\\tile.pac"),
				_T("map\\mesh.idx"),
				_T("map\\mesh.xpm"),
				_T("map\\detail.tex")
			};

			if( !Map::InitializeXiahMap( pMapeFileList))
			{
				MessageBox( GetForegroundWindow(), START_ERROR10, START_ERROR0, MB_OK);
				bFailToLoad = true;
				break;
			}

			// CG_2005/05/26 : 스카이맵 체인지
			if( !g_Sky.Load(/* "map\\Sky.txt" */) )
			{
				DBG_LogFile( _T("g_Sky.Create 실패"));
			}

			/*
			if(!g_SkyBox.Create())
			{
				DBG_LogFile( _T("g_SkyBOX.Create 실패"));
			}

			// 배경의 SKYBOX 텍스처 설정 ()
			g_SkyBox.ChangeSkyMap(3);

			// 해, 달, 별
			if(!g_SkyStar.Create())
			{
				DBG_LogFile( _T("g_SkyStar.Create 실패"));
			}
			*/

			// RAIN INIT
			if(!g_RainSnow.Init())
			{
				DBG_LogFile( _T("g_RainSnow.Init 실패"));
			}

			// 타격 숫자, Miss, Hit 이펙트.
			if(!g_HitEffect.Init())
			{
				DBG_LogFile( _T("g_HitEffect.Init 실패"));
			}

			// Game Step

			if( !XiahNetwork::InitializeNetworkHandler())
			{
				MessageBox( GetForegroundWindow(), START_ERROR11, START_ERROR0, MB_OK);
				bFailToLoad = true;
				break;
			}

			XiahNetwork::g_XiahSocketOnConnected = OnConnectedToServer;

			// 환경 정보 세팅
			g_XiahEnvInfo.m_bFog			= TRUE;
			g_XiahEnvInfo.m_bAmhukmuFog		= FALSE;
			g_XiahEnvInfo.m_DiffuseColor	= D3DCOLOR_XRGB(255, 255, 255);
			g_XiahEnvInfo.m_fFogDensity		= 0.005f;
			g_XiahEnvInfo.m_CameraBoundSize = 128 + (1024 - 128) * (3 /*g_EngineInfo.m_fViewDistance*/ / 10.0f);
			g_XiahEnvInfo.m_fDetailMapRatio = 1.0f;
			g_XiahEnvInfo.m_FogColor		= D3DCOLOR_XRGB(236, 239, 255); //D3DCOLOR_XRGB( 189, 198, 202);
			g_XiahEnvInfo.m_SkyColorBottom	= D3DCOLOR_XRGB(255, 255, 255); //D3DCOLOR_XRGB(80, 80, 80);
			g_XiahEnvInfo.m_SkyColorMiddle	= D3DCOLOR_XRGB(255, 255, 255); //D3DCOLOR_XRGB( 62, 67, 68 );
			g_XiahEnvInfo.m_SkyColorUp		= D3DCOLOR_XRGB(255, 255, 255); //D3DCOLOR_XRGB( 0, 0, 0 );

			g_XiahChangeEnvInfo.bChangeStart = false;			

			/*
			g_XiahEnvInfo.m_bFog = TRUE;
			g_XiahEnvInfo.m_bAmhukmuFog = FALSE;
			g_XiahEnvInfo.m_DiffuseColor = D3DCOLOR_XRGB( 255, 255, 255);
			g_XiahEnvInfo.m_fFogDensity = 0.01f;
			g_XiahEnvInfo.m_CameraBoundSize = 128 + (1024 - 128) * (g_EngineInfo.m_fViewDistance / 10.0f);
			g_XiahEnvInfo.m_fDetailMapRatio = 1.0f;
			g_XiahEnvInfo.m_FogColor = D3DCOLOR_XRGB( 189, 198, 202);
			g_XiahEnvInfo.m_SkyColorBottom	= D3DCOLOR_XRGB( 80, 80, 80 );
			g_XiahEnvInfo.m_SkyColorMiddle	= D3DCOLOR_XRGB( 62, 67, 68 );
			g_XiahEnvInfo.m_SkyColorUp		= D3DCOLOR_XRGB( 0, 0, 0 );
			*/

			// 비가 올때 포그가 시간에 따라서 서서히 변하도록 하기 위해.
			g_XiahChangeEnvInfo.bChangeStart = false;
			g_MainCharInfo.Create();

			// Auth Server에 접속			
			if(ConnectAuthServer() == FALSE)
			{
				bFailToLoad = true;
				break;
			}

			// 마지막으로 자신의 데이타 삭제
			if(!Release())
			{
				DBG_LogFile( _T("CXiahGameStartLoad::Update / Release 실패"));
			}

			m_byCurStep = eStartLoad_End;
		}
		break;
	case eStartLoad_End:
		break;
	};// switch

	if(!Fade::UpdateFade())
	{
		DBG_LogFile( _T("CXiahGameStartLoad::Update / Fade::UpdateFade 실패"));
	}

	if( bFailToLoad )
	{
		MessageBox( GetForegroundWindow(), START_ERROR13, START_ERROR0, MB_OK);
		DBG_LogFile( _T("Xiah 게임 초기화 실패"));
		PostQuitMessage( 0);
		return FALSE;
	}


	return TRUE;
}

BOOL CXiahGameStartLoad::Render()
{
	g_pDirect3DDevice->SetRenderState( D3DRS_FOGENABLE, FALSE);
	g_pDirect3DDevice->SetRenderState( D3DRS_LIGHTING, FALSE);
	g_pDirect3DDevice->SetRenderState( D3DRS_ALPHABLENDENABLE, FALSE );

	switch( m_byCurStep )
	{
	case eStartLoad_Start:
		m_byCurStep = eStartLoad_TaewoolLogoFadeOut;
		break;
	case eStartLoad_TaewoolLogoFadeOut:
	case eStartLoad_TaewoolLogoFadeIn:
		{
			g_Device.SetTexture(0, m_pTexture1);
			//g_pDirect3DDevice->SetTexture( 0, m_pTexture1 );
			g_Device.SetStreamSource( m_pVB1, sizeof(VT_TLVertex) );
			g_Device.SetFVF(D3DFVF_TLVERTEX);
			//g_pDirect3DDevice->SetFVF( D3DFVF_TLVERTEX );
			g_pDirect3DDevice->DrawPrimitive( D3DPT_TRIANGLESTRIP, 0, 2 );
		}		
		break;
	case eStartLoad_XiahLogoFadeOut:
	case eStartLoad_XiahLogoFadeIn:
		{
			g_Device.SetTexture(0, m_pTexture2);
			//g_pDirect3DDevice->SetTexture( 0, m_pTexture2 );
			g_Device.SetStreamSource( m_pVB2, sizeof(VT_TLVertex) );
			g_Device.SetFVF(D3DFVF_TLVERTEX);
			//g_pDirect3DDevice->SetFVF( D3DFVF_TLVERTEX );
			g_pDirect3DDevice->DrawPrimitive( D3DPT_TRIANGLESTRIP, 0, 2 );
		}
// NOT!
#ifndef _CHINA_
		if( m_dwElapsedTime >= 9200 )
		{
			m_byCurStep = eStartLoad_Load1;
			//			Fade::StartFade( 0, 0, NULL, 1500 );	// 다시 Fade Out을 해야 Diffuse땜시 로딩화면이 보인다.
			// 이때 로고 텍스쳐를 릴리즈한다.
			if(!XiahPak::UninitializeXiahPak())
			{
				DBG_LogFile( _T("CXiahGameStartLoad::Render 실패"));
			}
		}
#endif
	break;

	case eStartLoad_China_WarnningIn :
	case eStartLoad_China_WarnningOut:
		{
			g_Device.SetTexture(0, m_pTexture3);
			//g_pDirect3DDevice->SetTexture( 0, m_pTexture3 );
			g_Device.SetStreamSource( m_pVB2, sizeof(VT_TLVertex) );
			g_Device.SetFVF(D3DFVF_TLVERTEX);
			//g_pDirect3DDevice->SetFVF( D3DFVF_TLVERTEX );
			g_pDirect3DDevice->DrawPrimitive( D3DPT_TRIANGLESTRIP, 0, 2 );
		}		

#ifdef _CHINA_
		if( m_dwElapsedTime >= 16000 )
		{
			m_byCurStep = eStartLoad_Load1;
			//			Fade::StartFade( 0, 0, NULL, 1500 );	// 다시 Fade Out을 해야 Diffuse땜시 로딩화면이 보인다.
			// 이때 로고 텍스쳐를 릴리즈한다.
			if(!XiahPak::UninitializeXiahPak())
			{
				DBG_LogFile( _T("CXiahGameStartLoad::Render 실패"));
			}
		}
#endif
		break;

	case eStartLoad_LoadLogoFadeIn:
		if( m_dwElapsedTime >= m_dwBackTime + 2000 )
		{
			m_byCurStep = eStartLoad_Load2;
		}

		// 2004.07.20 이벤트용 로딩화면
		/*
		if( rand() % 2 )
			g_pUIManager->ForwardShow( EVENT_LOADING_1 );
		else
			g_pUIManager->ForwardShow( EVENT_LOADING_2 );
		*/

		g_pUIManager->ForwardShow(LOADING_IMAGE3);

		g_pUIManager->SpecialDraw();
		break;

	case eStartLoad_Load2:
	case eStartLoad_End:
		// 2004.07.20 이벤트용 로딩화면
		/*
		if( rand() % 2 )
			g_pUIManager->ForwardShow( EVENT_LOADING_1 );
		else
			g_pUIManager->ForwardShow( EVENT_LOADING_2 );
		*/

		g_pUIManager->ForwardShow(LOADING_IMAGE3);
		g_pUIManager->SpecialDraw();
		break;
	};// switch

	if(!Fade::RenderFade())
	{
		DBG_LogFile( _T("CXiahGameStartLoad::Render 실패"));

		//		return false;
	}

	return TRUE;
}

#include <time.h>

// AuthServer로의 접속
BOOL CXiahGameStartLoad::ConnectAuthServer()
{
	BOOL ret;
	int pos;
	int Count = 0;
	int	Order[64] = {0,};
	int num = g_AppData.m_NumAuthserver;

	// 차레대로 시도!
	while(1)
	{
		if(Count >= g_AppData.m_NumAuthserver)
		{
			MessageBox( GetForegroundWindow(), START_ERROR12, START_ERROR0, MB_OK);
			DBG_Put(_T("륩蛟포젯쌈呵겨"));
			ret = FALSE;
			break;
		}

		srand((unsigned)time(NULL));
		pos = rand()%num;
		if(Order[pos] == 1) continue;

		if( !XiahNetwork::ConnectToServer( g_AppData.m_AuthServer[pos].m_ServerAddress, g_AppData.m_AuthServer[pos].m_ServerPort))
		{
			Order[pos] = 1;
			++Count;
			continue;
		}
		else
		{
			ret = TRUE;
			break;
		}
		++Count;
	}

	return ret;
}
