#include "precompile.h"
#include "AppData.h"
#include "InterfaceDefine.h"
#include "resource.h"
#include "XiahNetworkHandler.h"
#include "XiahSocket.h"
#include "XiahGame_Intro.h"
#include "XiahCamera.h"
#include "XiahMap.h"
#include "XiahArrayIndex.h"
#include "XiahGameMain.h"
#include "XiahGame_Handler_Sender.h"
#include "Fade.h"
#include "XiahEnvInfo.h"
#include "XiahCursor.h"

#include "XiahGame_Pet.h"
#include ".\munpamark.h"
#include "RebirthMark.h"


extern BOOL SetupPC_VisualEquipement(CXiahCharObject* pObject, WORD* pVisualList, BYTE* pRarityList=NULL, BYTE* pStxTypeList=NULL);
extern bool SetupPET_VisualEquipement(CXiahCharObject* pObject, WORD* pVisualList);


// 雱ろ姼鞗?鞝戩啀鞚?靹标车頃橁碃, 鞎旐樃韨り箤歆� 靹疙寘鞚?霅橃柎靹?氅旍劯歆�毳?氤措偞霃?膦嬱潃 靸來儨鞚措┐
void OnConnectedToServer()
{
	static bool bFirstConnected = false;

	if(!bFirstConnected)
	{
		SET_GAMESTEP(GAMESTEP_LOGIN);

		g_pUIManager->CloseAll();

		g_pUIManager->Show(LOGIN_1);

		bFirstConnected = true;
	}
	else
	{
		SendCS_IT_LOGIN_REQ();

		g_pUIManager->CloseAll();

		// 2004.07.20 鞚措菠韸胳毄 搿滊敥頇旊┐
		/*
		if( rand() % 2 )
			g_pUIManager->ForwardShow( EVENT_LOADING_1 );
		else
			g_pUIManager->ForwardShow( EVENT_LOADING_2 );
		*/

		g_pUIManager->ForwardShow(LOADING_IMAGE3);

		//SET_GAMESTEP(GAMESTEP_INTRO);
	}
}

////////////////////////////////////////////////////////////////////////////////
// CharacterInfo
// 鞚疙姼搿滌棎 鞊办澊電?旌愲Ν韯?鞝曤炒 韥措灅鞀?鞝曥潣
////////////////////////////////////////////////////////////////////////////////

XiahGame_Intro*				g_pIntro = NULL;


#define	SELECT_GUMYUNG_SOUND	50001314
#define	SELECT_YUNRANG_SOUND	50001315
#define	SELECT_MOOTOO_SOUND		50001316
#define	SELECT_YACHA_SOUND		50001857



////////////////////////////////////////////////////////////////////////////////
// XiahGame_Intro
// 鞚疙姼搿?甏�霠?韥措灅鞀?鞝曥潣
////////////////////////////////////////////////////////////////////////////////

XiahGame_Intro::XiahGame_Intro()
{
	CreateCharacterData();

	m_byCurrentClassIndex = 0;

	m_byCharCount = 0;
	m_byCurrentCharIndex = 0;
	m_byCurrentMenu = 0;

	g_pIntro = this;

	m_bIntroBGMPlay = true;

	m_byOnMouseCharIndex = 255;

	//
	m_nCharacterSelectStep = -1;
	m_bCharacterSelected = false;
	m_nCharacterCreateStep = eCCType_GyumYung;
	m_bCharacterCreatePreLoaded = false;
	m_bFirstCharSelect = true;
	m_bCanSelectCharacter = true;
	m_bIntroInitCharDataCreated = false;

	m_bFirstGameLoadScreen = true;
	m_bIntroFirstCall = true;

//	RegisterAllNetworkHandler_Intro();
//	RegisterAllNetworkHandler_Interface();

}

XiahGame_Intro::~XiahGame_Intro()
{
	DeleteCharacterData();
}

void XiahGame_Intro::Init_Clear()
{
	//CreateCharacterData();

	m_byCurrentClassIndex = 0;

	m_byCharCount = 0;
	m_byCurrentCharIndex = 0;
	m_byCurrentMenu = 0;

	g_pIntro = this;

	m_bIntroBGMPlay = true;

	m_byOnMouseCharIndex = 255;

	//
	m_nCharacterSelectStep = -1;
	m_bCharacterSelected = false;
	m_nCharacterCreateStep = eCCType_GyumYung_Rotate; //eCCType_GyumYung;
	m_bCharacterCreatePreLoaded = false;
	m_bFirstCharSelect = true;
	m_bCanSelectCharacter = true;
	m_bIntroInitCharDataCreated = false;

	m_bFirstGameLoadScreen = true;
	m_bIntroFirstCall = true;

	g_XiahCamera.m_bIntro = true;

	//Vector3 vAt( 630, 134, -1450);
	//Vector3 vFrom( 3, 1, 0 );
	//vFrom = vAt + vFrom;

	Vector3 vAt; //( x, height + 30, y);
	Vector3 vFrom; //( 5, 3, -20);

	// test code
	vAt.x = 1626.0448f;
	vAt.y = 85.000000f;
	vAt.z = -1243.9867f;

	vFrom.x = 1628.9955f;
	vFrom.y = 104.23999f;
	vFrom.z = -1202.4309f;

	g_XiahCamera.SetView( vFrom, vAt, Vector3( 0, 1, 0));

	g_pUIManager->Show(INTRO_BUTTONSET);
	g_pUIManager->Show(INTRO_CHARACTER_SELECT);
	g_pUIManager->Show(INTRO_WINDOW01);

	g_pUIManager->Hide(MAIN_FRAME);

	g_pUIManager->Hide(SMALL_MESSENGER);
	g_pUIManager->Hide(MAIN_CHAT);
	g_pUIManager->Hide(SITUATION_BRA);
	g_pUIManager->Hide(DATA_WINDOW);
	g_pUIManager->Hide(PET_BUTTON_GROUP);

	// 頇橁步 鞝曤炒 靹疙寘
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
}

void XiahGame_Intro::CreateInitCharData()
{
	// intro menu character
	// 瓴�鞓?
	m_GyumYung.Create(790, 3, 2, 103);
	m_GyumYung.m_bSubObjType = 1;
	m_GyumYung.m_bGray		 = false;

	WORD EquipVID[9];
	memset( EquipVID, 0, sizeof(EquipVID) );

	// 鞓? 氍搓赴 膦嬱潃 瓯鸽（.
	EquipVID[ EQUIPPOS_CLOTH ]	= 2011;
	EquipVID[ EQUIPPOS_WEAPON ]	= 1013; //1102;
	EquipVID[ EQUIPPOS_SHOE ]	= 4006;
	EquipVID[ EQUIPPOS_HAT ]	= 3009;
	SetupPC_VisualEquipement( &m_GyumYung, EquipVID );

	// 鞐半瀾
	m_YunRang.Create( 867, 3, 2, 103);
	m_YunRang.m_bSubObjType = 2;
	m_YunRang.m_bGray		= false;

	memset( EquipVID, 0, sizeof(EquipVID) );

	EquipVID[ EQUIPPOS_CLOTH ]	= 2111;
	EquipVID[ EQUIPPOS_WEAPON ]	= 1310; //1303;
	EquipVID[ EQUIPPOS_SHOE ]	= 4106;
	EquipVID[ EQUIPPOS_HAT ]	= 3108;
	SetupPC_VisualEquipement( &m_YunRang, EquipVID );

	// 氍错埇
	m_MooToo.Create( 891, 3, 2, 103 );
	m_MooToo.m_bSubObjType = 3;
	m_MooToo.m_bGray	   = false;

	memset( EquipVID, 0, sizeof(EquipVID) );

	EquipVID[ EQUIPPOS_CLOTH ]	= 2211;
	EquipVID[ EQUIPPOS_WEAPON ]	= 1510; //1303;
	EquipVID[ EQUIPPOS_SHOE ]	= 4206;
	EquipVID[ EQUIPPOS_HAT ]	= 3209;
	SetupPC_VisualEquipement( &m_MooToo, EquipVID );

	// 鞎检皑
	m_YaCha.Create( 906, 3, 2, 103 );
	m_YaCha.m_bSubObjType = 4;
	m_YaCha.m_bGray		  = false;

	memset( EquipVID, 0, sizeof(EquipVID) );

	EquipVID[ EQUIPPOS_CLOTH ]	= 2311;
	EquipVID[ EQUIPPOS_WEAPON ]	= 1710;
	EquipVID[ EQUIPPOS_SHOE ]	= 4306;
	EquipVID[ EQUIPPOS_HAT ]	= 3309;
	SetupPC_VisualEquipement( &m_YaCha, EquipVID );

	m_bIntroInitCharDataCreated = true;

	g_PetList.InitWild();
	g_MunpaMark.Init();
	g_RebirthMark.Init();
}

void XiahGame_Intro::CreateCharacterData()
{
	for( int i=0; i<CHARACTER_MAX; i++)
	{
		m_CharacterList[i] = new CharacterInfo();

		if(m_CharacterList[i] == NULL)
		{
			DBG_LogFile( _T("XiahGame_Intro::CreateCharacterData 鞁ろ尐"));

//			return;
		}
	}
}

void XiahGame_Intro::DeleteCharacterData()
{
	for( int i=0; i<CHARACTER_MAX; i++)
	{
		if(m_CharacterList[ i])
		{
			delete m_CharacterList[ i];
			m_CharacterList[ i] = NULL;
		}
	}
}

/**
 *
 * \return 
 */
// 霠岆崝毵侅澊 霅橁赴 鞝勳棎 Update霅犼矁霌?
BOOL XiahGame_Intro::Update()
{
	// 鞎勳�� 觳橃潓 瓴岇瀯 搿滊敥 頇旊┐鞚茧晫.
	if( m_bFirstGameLoadScreen )
	{
		g_pUIManager->SpecialDraw();

		if( m_bIntroFirstCall )
		{
			ChangeXiahCursor( eCT_General);

			m_bIntroFirstCall = false;
			InitIntro( m_byIntroFirstCallCurMenu );
		}
	}
	else
	{
		static BOOL bFade = FALSE;	// 鞚措煱 鞖半個鞚检澊 銋?,銋?

		if( !bFade)
		{
			Fade::StartFade( 0, 0, NULL, 500000 );
			bFade = TRUE;
		}

		DWORD dwTime = 33.0f * g_fFrameScale;
		if( dwTime > 100 ) dwTime = 95;
		float fTime = (float)dwTime / 1000.0f;

#ifdef MINI
		// 氙鸽媹 鞁滌晞鞖?鞛呺媹雼?
		if(GetAsyncKeyState(VK_RETURN) < 0)
		{
			g_MainCharInfo.PlayInterfaceSound( ISOUND_GAME_START_BUTTON);
			
			g_MainCharInfo.OpenFrame( LOADING_IMAGE3); //HO_0702_07 霌标笁響滌嫓 : 霌标笁響滌嫓鞕� 頃�瓴� 鞀ろ儉韸鸽�滊敥瓿� 瓴岇瀯搿滊敥 攵�攵勳澊 霃欖澕 鞚措�胳��搿?觳橂Μ霅滊嫟.
			//g_MainCharInfo.OpenFrame( LOADING_IMAGE); //霌标笁響滌嫓 鞝侅毄鞝?旖旊摐 雮橃�戩�?歆�鞗?氩勲Μ鞛?..; 霌标笁響滌嫓 鞝勳棎電?雮橃澊 甑�攵勳�?鞛堨棁雼?..

			g_GameWork.m_nNavigationMode = 0;
			SendCS_NV_STARTGAME_REQ();
		}
#endif

		// 牍欗檾歆�雽� 雸?
		g_RainSnow.Update(33.0f * g_fFrameScale);

		switch( m_byCurrentMenu )
		{
		case INTROMENU_CREATECHAR:
			{
				/*
				// test
				if( GetAsyncKeyState( VK_ESCAPE ) < 0 )
				{
				m_nCharacterCreateStep = eCCType_GyumYung;
				GoToMenuCreateChar();
				return TRUE;
				}
				*/

				float fCameraMoveTotalSpeed = 1.0f;

				// 頇旊┐鞚?鞏措憪鞖鸽晫 旃措�旊澕毳� 鞗�歆侅棳靹?毵奠潉 氙鸽Μ 鞚诫姅 須�瓿茧�?雮挫瀽. 
				if( !m_bCharacterCreatePreLoaded )
				{
					fCameraMoveTotalSpeed = 2.8f;

					if( m_nCharacterCreateStep == eCCType_GyumYung_Rotate )
					{
						fCameraMoveTotalSpeed = 1.0f;

						if(!Fade::StartFade( 0, 0, NULL, 1200 ))
						{
							DBG_LogFile( _T("XiahGame_Intro::Update / Fade::StartFade 鞁ろ尐"));

							//return false;
						}

						m_dwCameraMoveStartTime = 0;

						m_nCharacterCreateStep = eCCType_GyumYung;
						m_bCharacterCreatePreLoaded = true;

						// camera setting
						g_XiahCamera.m_fXAngle   = -0.0052664680f; //-_PI / 2.2f; //-_PI / 12; //-_PI / 6;
						g_XiahCamera.m_fYAngle   = -1185.1012f; //-_PI / 2;
						g_XiahCamera.m_fDistance = 13.5f; //300;

						g_XiahCamera.m_fDestXAngle	 = g_XiahCamera.m_fXAngle;
						g_XiahCamera.m_fDestDistance = g_XiahCamera.m_fDistance;

						// 旃措�旊�?靹れ爼
						//float x,y;
						//x = 1292; //994;
						//y = -1292; //1527;
						//float height = Map::g_MapRes.GetHeight( x, y);
						//Vector3 vAt( x, height, y);
						//Vector3 vFrom( 3, 1, 0 );
						//vFrom = vAt + vFrom;
						Vector3 vAt; //( x, height + 20, y);
						Vector3 vFrom; //( 3, 1, 0 );

						vAt.x = 1400.8101f;
						vAt.y = 145.00000f;
						vAt.z = -754.49518f;

						vFrom.x = 1390.1307f;
						vFrom.y = 145.00000f;
						vFrom.z = -742.58215f;

						g_XiahCamera.SetView( vFrom, vAt, Vector3( 0, 1, 0));
#ifdef MINI
						g_XiahCamera.SetProjection( _PI / 4, (float)G_HEIGHT / (float)G_WIDTH, 1.0f, 256.0f);
#else
						g_XiahCamera.SetProjection( _PI / 4, 768.0f / 1024.0f, 1.0f, 256.0f);
#endif

						// 氚旊�?旃措�旊�?靹れ爼鞚?毵奠棎 鞝侅毄
						if(!XiahMap::g_XiahMap.m_pMapRender->Update())
						{
							DBG_LogFile( _T("XiahGame_Intro::Update / XiahMap::g_XiahMap.m_pMapRender->Update 鞁ろ尐"));

//							return false;
						}
					}// if

				}// if

				//
				float fXMoveSpeed;

				m_dwCameraMoveStartTime += dwTime;

				// 瓿�霑岆Μ電� 頃橂摐旖旊敥鞚搓蛋.
				// Now Fade Out, Camera Move Start
				switch( m_nCharacterCreateStep )
				{
				case eCCType_GyumYung:
					{
						if( m_dwCameraMoveStartTime > 900 )
						{
							m_nCharacterCreateStep = eCCType_GyumYung_GoLong;
							g_XiahCamera.m_fDestXAngle = (-_PI / 8);

							if( m_bCharacterCreatePreLoaded )
								g_MainCharInfo.PlayInterfaceSound( ISOUND_CAMERA_MOVE );

							// 氤�靾橂ゼ 霕?靹犾柛頃橃�� 鞎婈碃 鞛�靷�鞖╉晿鞛? 銋嬨厠
							m_fYAngleWhenClicked = g_XiahCamera.m_fYAngle;
							m_bOri = false;
							m_fAddHeight = 0;
							m_bFirst = true;
						}
					}
					break;
				case eCCType_GyumYung_GoLong:
					{
						if( g_XiahCamera.m_fDistance > 20 )
							g_XiahCamera.m_fDistance -= fTime * 220 * fCameraMoveTotalSpeed;
						else
						{
							if( !m_bCharacterCreatePreLoaded )
								g_XiahCamera.m_fDistance = 20;
						}

						if( g_XiahCamera.m_fDistance > 160 )
						{
							if( g_XiahCamera.m_fYAngle > m_fYAngleWhenClicked - 0.6f )
								g_XiahCamera.m_fYAngle -= fTime * 1.8f * fCameraMoveTotalSpeed;
						}
						else
							if( g_XiahCamera.m_fDistance > 20 )
							{
								if( g_XiahCamera.m_fYAngle < m_fYAngleWhenClicked + 0.6f )
									g_XiahCamera.m_fYAngle += fTime * 2.8f * fCameraMoveTotalSpeed;
								else
									m_bOri = true;
							}

							if( m_bOri )
							{
								if( g_XiahCamera.m_vAt.x < 770 )
								{
									if( g_XiahCamera.m_fYAngle > m_fYAngleWhenClicked )
										g_XiahCamera.m_fYAngle -= fTime * 1.2f * fCameraMoveTotalSpeed;
									else
										m_bOri = false;
								}
							}

							// x move speed
							if( g_XiahCamera.m_fDistance > 200 )
								fXMoveSpeed = 10;
							else
								if( g_XiahCamera.m_fDistance > 160 )
									fXMoveSpeed = 55;
								else
									fXMoveSpeed = 225;

							if( g_XiahCamera.m_vAt.x > 500 ) // 435
								g_XiahCamera.m_vAt.x -= fTime * fXMoveSpeed * fCameraMoveTotalSpeed;
							else
							{
								m_nCharacterCreateStep = eCCType_GyumYung_TurnArcLeft1;
							}

							// z
							if( g_XiahCamera.m_fDistance < 60 )
							{
								if( g_XiahCamera.m_vAt.z < -1400 )
									g_XiahCamera.m_vAt.z += fTime * 140 * fCameraMoveTotalSpeed;
							}

							// y
							float height = Map::g_MapRes.GetHeight(g_XiahCamera.m_vAt.x, g_XiahCamera.m_vAt.z);

							if( g_XiahCamera.m_vAt.x < 850 && m_bFirst )
							{
								m_bFirst = false;
								m_fAddHeight = height + 65;
							}

							if( m_bFirst )
								g_XiahCamera.m_vAt.y = height;
							else
							{
								if( g_XiahCamera.m_vAt.y < m_fAddHeight )
									g_XiahCamera.m_vAt.y += fTime * 50 * fCameraMoveTotalSpeed;
								else
									g_XiahCamera.m_vAt.y = m_fAddHeight;
							}
					}
					break;
				case eCCType_GyumYung_TurnArcLeft1:
					{
						float fR = 84.8528f;
						int nCount=0;
						// x
						if( g_XiahCamera.m_vAt.x > 500-fR )
							g_XiahCamera.m_vAt.x -= fTime * 240 * fCameraMoveTotalSpeed;
						else
							if( g_XiahCamera.m_vAt.x > 380 )
								g_XiahCamera.m_vAt.x -= fTime * 240 * fCameraMoveTotalSpeed;
							else nCount++;

							// z
							if( g_XiahCamera.m_vAt.z > -1520+fR )
								g_XiahCamera.m_vAt.z -= fTime * 240 * fCameraMoveTotalSpeed;
							else
								if( g_XiahCamera.m_vAt.z > -1520 )
									g_XiahCamera.m_vAt.z -= fTime * 240 * fCameraMoveTotalSpeed;
								else nCount++;

								// Y Angle
								if( g_XiahCamera.m_fYAngle > m_fYAngleWhenClicked - (_PI/2) )
									g_XiahCamera.m_fYAngle -= fTime * 3.1415f * fCameraMoveTotalSpeed;
								else nCount++;

								if( nCount == 3 )
									m_nCharacterCreateStep = eCCType_GyumYung_TurnArcLeft2;
					}
					break;
				case eCCType_GyumYung_TurnArcLeft2:
					{
						float fR = 84.8528f;
						int nCount=0;
						// x
						if( g_XiahCamera.m_vAt.x < 380+fR )
							g_XiahCamera.m_vAt.x += fTime * 240 * fCameraMoveTotalSpeed;
						else
							if( g_XiahCamera.m_vAt.x < 500 )
								g_XiahCamera.m_vAt.x += fTime * 240 * fCameraMoveTotalSpeed;
							else nCount++;

							// z
							if( g_XiahCamera.m_vAt.z > -1640+fR )
								g_XiahCamera.m_vAt.z -= fTime * 240 * fCameraMoveTotalSpeed;
							else
								if( g_XiahCamera.m_vAt.z > -1640 )
									g_XiahCamera.m_vAt.z -= fTime * 240 * fCameraMoveTotalSpeed;
								else nCount++;

								// Y Angle
								if( g_XiahCamera.m_fYAngle > m_fYAngleWhenClicked - (_PI) )
									g_XiahCamera.m_fYAngle -= fTime * 3.1415f * fCameraMoveTotalSpeed;
								else nCount++;

								if( nCount == 3 )
								{
									m_nCharacterCreateStep = eCCType_GyumYung_GoXDir;
									m_fAddHeight = g_XiahCamera.m_vAt.y + 130;
									m_bOri = true;

									g_XiahCamera.m_fDestXAngle = -_PI / 5.5f;
								}
					}
					break;
				case eCCType_GyumYung_GoXDir:
					{
						// Height - JUMP
						if( g_XiahCamera.m_vAt.x > 550 )
						{
							if( m_bOri )
							{
								if( g_XiahCamera.m_vAt.y < m_fAddHeight )
									g_XiahCamera.m_vAt.y += fTime * 220 * fCameraMoveTotalSpeed;
								else
									m_bOri = false;
							}
							else
							{
								if( g_XiahCamera.m_vAt.y > m_fAddHeight - 140 )
									g_XiahCamera.m_vAt.y -= fTime * 220 * fCameraMoveTotalSpeed;
							}
						}

						if( g_XiahCamera.m_vAt.x < 810 )
							g_XiahCamera.m_vAt.x += fTime * 240 * fCameraMoveTotalSpeed;
						else
							if( g_XiahCamera.m_vAt.x < 859 )
								g_XiahCamera.m_vAt.x += fTime * 150 * fCameraMoveTotalSpeed;
							else
							{
								m_nCharacterCreateStep = eCCType_GyumYung_TurnBackToChar1;
								m_fYAngleWhenClicked = g_XiahCamera.m_fYAngle;
								m_fAddHeight = g_XiahCamera.m_vAt.y - 169;
								m_fAddHeight /= 2;
								m_fAddHeight *= 1.6f;
							}
					}
					break;
				case eCCType_GyumYung_TurnBackToChar1:
					{
						int nCount=0;

						if( g_XiahCamera.m_vAt.x < 875 )
							g_XiahCamera.m_vAt.x += fTime * 25.6f * fCameraMoveTotalSpeed;
						else nCount++;

						if( g_XiahCamera.m_vAt.z > -1656 )
							g_XiahCamera.m_vAt.z -= fTime * 25.6f * fCameraMoveTotalSpeed;
						else nCount++;

						if( g_XiahCamera.m_vAt.y > 169 )
							g_XiahCamera.m_vAt.y -= fTime * m_fAddHeight * fCameraMoveTotalSpeed;

						// Y Angle
						if( g_XiahCamera.m_fYAngle < m_fYAngleWhenClicked + (_PI) )
							g_XiahCamera.m_fYAngle += fTime * 2.512f * fCameraMoveTotalSpeed; //1.57f; // 3.1415f;

						if( nCount==2 )
							m_nCharacterCreateStep = eCCType_GyumYung_TurnBackToChar2;
					}
					break;
				case eCCType_GyumYung_TurnBackToChar2:
					{
						int nCount=0;

						if( g_XiahCamera.m_vAt.x > 859 )
							g_XiahCamera.m_vAt.x -= fTime * 25.6f * fCameraMoveTotalSpeed;
						else nCount++;

						if( g_XiahCamera.m_vAt.z > -1672 )
							g_XiahCamera.m_vAt.z -= fTime * 25.6f * fCameraMoveTotalSpeed;
						else nCount++;

						if( g_XiahCamera.m_vAt.y > 169 )
							g_XiahCamera.m_vAt.y -= fTime * m_fAddHeight * fCameraMoveTotalSpeed;

						// Y Angle
						if( g_XiahCamera.m_fYAngle < m_fYAngleWhenClicked + (_PI) )
							g_XiahCamera.m_fYAngle += fTime * 2.512f * fCameraMoveTotalSpeed;

						// distance
						if( g_XiahCamera.m_fDistance < 39 )
							g_XiahCamera.m_fDistance += fTime * 20;
						g_XiahCamera.m_fDestDistance = g_XiahCamera.m_fDistance;

						if( nCount==2 )
						{
							m_nCharacterCreateStep = eCCType_GyumYung_Rotate;
							m_bOri = true;

							if( !m_bCharacterCreatePreLoaded )
								m_bOri = false;
						}
					}
					break;
				case eCCType_GyumYung_Rotate:
					{
						// distance
						if( g_XiahCamera.m_fDistance < 18 )
							g_XiahCamera.m_fDistance += fTime * 5;

						g_XiahCamera.m_fDestDistance = g_XiahCamera.m_fDistance;

						//g_XiahCamera.RotateY( TRUE, 0.3f );

						// interface menu
						if( m_bOri )
						{
							Fade::StartFade( 0, 0, NULL, 2000 );

							g_pUIManager->Show(INTRO_CHARACTER_SELECT);
							g_pUIManager->Show(INTRO_BUTTONSET);
							g_pUIManager->Show(INTRO_ACCOUNT);

							g_pUIManager->Hide(INTRO_BUTTONSET, INTRO_BUTTON_01);
							g_pUIManager->Hide(INTRO_BUTTONSET, INTRO_BUTTON_04);
							g_pUIManager->Hide(INTRO_BUTTONSET, INTRO_BUTTON_03);

							if( m_byCharCount )
							{
								g_pUIManager->Show(INTRO_BUTTONSET, INTRO_BUTTON_02);
								g_pUIManager->SetString(INTRO_BUTTONSET, INTRO_BUTTON_02, IDS_GO_BACK);
							}

							g_pUIManager->Hide(INTRO_WINDOW01); //->Show( FALSE);

							m_bOri = false;
						}
					}
					break;

				};// switch

				g_XiahCamera.Update();

				// 瓴�鞓?
				if( m_GyumYung.m_CharRender.IsValid())
				{
					m_GyumYung.m_Position = Vector3(1400.8101f, 141.00000f, -754.49518);
					m_GyumYung.m_Angle = 1185.1012f; //(-_PI / 2);
					m_GyumYung.Update(TRUE);
				}
				// 鞐半瀾
				if( m_YunRang.m_CharRender.IsValid())
				{
					m_YunRang.m_Position = Vector3(1400.8101f, 141.00000f, -754.49518);
					m_YunRang.m_Angle = 1185.1012f; //(-_PI / 2);
					m_YunRang.Update(TRUE);
				}
				// 氍错埇
				if( m_MooToo.m_CharRender.IsValid())
				{
					m_MooToo.m_Position = Vector3(1400.8101f, 141.00000f, -754.49518);
					m_MooToo.m_Angle = 1185.1012f; //(-_PI / 2);
					m_MooToo.Update(TRUE);
				}
				// 鞎检皑
				if( m_YaCha.m_CharRender.IsValid())
				{
					m_YaCha.m_Position = Vector3(1400.8101f, 141.00000f, -754.49518);
					m_YaCha.m_Angle = 1185.1012f; //(-_PI / 2);
					m_YaCha.Update(TRUE);
				}
			}
			break;
		case INTROMENU_SELECTCHAR:
			{
				// character position
				float fAngle = -3.0999999f;
				Vector3 vPos[3];

				vPos[0] = Vector3(  0, -4, 3.5f );
				vPos[1] = Vector3( -4, -4, -3 );
				vPos[2] = Vector3(  4, -4, -3 );

				for(int i=0; i < CHARACTER_MAX; ++i)
				{
					if( m_CharRender[i].m_CharRender.IsValid())
					{
						m_CharRender[i].m_Position  = g_XiahCamera.m_vAt + vPos[i];
						m_CharRender[i].m_Angle		= fAngle;
						m_CharRender[i].Update(TRUE);
					}
					
					fAngle += 1.9800001f;
				}// for

				// Camera Action
				if( !m_bCharacterSelected )	// 旌愲Ν韯?靹犿儩頃犽晫
				{
					// 觳�鞐� 氩勲矃 瓯半Μ歆� 鞎婋弰搿?氙鸽Μ 鞚诫姅 須�瓿�
					if( m_bFirstCharSelect )
					{
						static DWORD dwCurTime = 0;
						dwCurTime += dwTime;

						g_XiahCamera.RotateY( FALSE, 8.5f );

						if( dwCurTime > 400 )
							g_XiahCamera.m_fDestXAngle = -_PI / 15;

						if( dwCurTime > 1300 )
						{
							Fade::StartFade( 0, 0, NULL, 350 );

							m_bFirstCharSelect = false;
							g_XiahCamera.m_fXAngle = -0.43633235f; //-_PI / 4.5f;
							g_XiahCamera.m_fDestXAngle = g_XiahCamera.m_fXAngle;
						}
					}
					else
					{
						g_XiahCamera.RotateY( FALSE, 0.25f );

						// Mouse Picking
						Vector3	vStart = g_pCurrentCamera->GetCursorWorld( 0.0001f);	// 0頃橁碃 1欤茧┐ 雭濎灔鞚挫暭~
						Vector3 vEnd   = g_pCurrentCamera->GetCursorWorld( 0.9999f);

						m_byOnMouseCharIndex = 255;

						//					for(i=0; i<CHARACTER_MAX; i++)
						for(i=1; i<=m_byCharCount; i++)
						{
							if( !m_CharRender[i-1].m_bGray )
							{
								Vector3 vDistance = vStart - m_CharRender[i-1].m_WorldBound.m_Center;
								//float fDistance = vDistance.GetLength();

								// OnMouse
								if( m_CharRender[i-1].m_WorldBound.IsIntersect( vStart, vEnd, NULL))
								{
									m_CharRender[i-1].EnableGlowEffect(TRUE);
									m_CharRender[i-1].m_bShowObjectName = true;

									m_byOnMouseCharIndex = i-1;

									// ClickMouse
									if( XiahInput::g_bLButtonDown )
									{
										m_byCurrentCharIndex = i-1;
										m_byOnMouseCharIndex = 255;

										m_CharRender[i-1].m_bShowObjectName = false;
										m_bCharacterSelected = true;
										m_nCharacterSelectStep = eCST_Click;
									}
								}
								else
								{
									m_CharRender[i-1].EnableGlowEffect(FALSE);
									m_CharRender[i-1].m_bShowObjectName = false;
								}
							}// if
						}// for

						m_bMoveCameraRight = FALSE;
					}
				}
				else	// 旌愲Ν韯瓣皜 靹犿儩霅愳潉霑?
				{
					float fYAngleSpeed = 3.5f;
					float fDistSpeed = 45.0f;
					// 旌愲Ν韯瓣皜 靹犿儩霅橂┐ 牍犽ゴ瓴?欷岇澑霅滊嫟.
					switch( m_nCharacterSelectStep )
					{
					case eCST_Click:
						{
							// 鞓るジ飒?鞎勲嫎 鞕检�届溂耄� 須岇爠頃犼箤
							Vector3 vV1 = m_CharRender[m_byCurrentCharIndex].m_Position - g_XiahCamera.m_vAt;
							Vector3 vV2 = g_XiahCamera.m_vFrom - g_XiahCamera.m_vAt;

							vV1.Normalize();
							vV2.Normalize();

							Vector3 vTemp = vV2.Cross( vV1 );
							float fDot = vTemp.Dot( Vector3( 0, 1, 0) );

							if( fDot > 0.0f ) m_bMoveCameraRight = TRUE;
							else m_bMoveCameraRight = FALSE;

							// 旃措�旊澕鞐� 雽�頃?旌愲Ν韯办潣 鞙勳箻 臧侂弰
							vV1 = m_CharRender[m_byCurrentCharIndex].m_Position - g_XiahCamera.m_vAt;
							vV2 = g_XiahCamera.m_vFrom - g_XiahCamera.m_vAt;
							vV1.y = 0;
							vV2.y = 0;
							vV1.Normalize();
							vV2.Normalize();
							m_fYAngleWhenClicked = vV2.GetAngle( vV1 );

							if( m_bMoveCameraRight )	// 鞙勳箻 氤挫爼鞚?鞙勴暅 氇╉憸 臧?
							{
								// 旃措�旊澕臧� 霃岇晞臧堧晫 2氩堨Ц, 3氩堨Ц 旌愲Ν韯瓣皜 旃措�旊�?鞝曤┐鞚?氤措弰搿?臧侂弰毳?靾橃爼.
								if( m_byCurrentCharIndex == 1 )
									m_fYAngleWhenClicked += 0.15f;
								else
								if( m_byCurrentCharIndex == 2 )
									m_fYAngleWhenClicked += 0.035f;

								m_fCameraTargetYAngle = g_XiahCamera.m_fYAngle + m_fYAngleWhenClicked;
							}
							else
							{
								if( m_byCurrentCharIndex == 1 )
									m_fYAngleWhenClicked -= 0.19f;
								else
								if( m_byCurrentCharIndex == 2 )
									m_fYAngleWhenClicked -= 0.055f;

								m_fCameraTargetYAngle = g_XiahCamera.m_fYAngle - m_fYAngleWhenClicked;
							}

							m_fCameraRotateYAngle = 0;
							g_XiahCamera.m_fDestXAngle = -_PI / 15;
							m_nCharacterSelectStep = eCST_ZoomIn;

							g_MainCharInfo.PlayInterfaceSound( ISOUND_ZOOM_IN );
						}
						break;
					case eCST_ZoomIn:
						{
							int nCount = 0;
							// Y於曥溂搿?須岇爠霅橂┐靹?
							if( fabs(m_fYAngleWhenClicked-m_fCameraRotateYAngle) > 0.3f )
							{
								float fValue = fTime * fYAngleSpeed;
								m_fCameraRotateYAngle += fValue;
								if( !m_bMoveCameraRight ) fValue = -fValue;

								g_XiahCamera.m_fYAngle += fValue;
							}
							else
							{
								g_XiahCamera.m_fYAngle = m_fCameraTargetYAngle;
								nCount++;
							}

							// X於曤弰 須岇爠霅橁碃 
							// 臧�旯岇泴歆勲嫟.
							if( g_XiahCamera.m_fDistance > 20 )
								g_XiahCamera.m_fDistance -= (fTime * fDistSpeed);
							else
							{
								g_XiahCamera.m_fDistance = 20.0f;
								nCount++;
							}

							g_XiahCamera.m_fDestDistance = g_XiahCamera.m_fDistance;

							if( nCount == 2 )
							{
								m_nCharacterSelectStep = eCST_Stay;

								// 靺�霠夗姼 鞎犽媹氅旍澊靺?
								m_CharRender[m_byCurrentCharIndex].SetAnimation( XiahAniType::eLAT_Casual, XiahAniType::eLAT_Stand, 0, 0 );
								SetCurrentCharInfoToScr();

								//  Select SoUND
								switch(m_CharRender[m_byCurrentCharIndex].m_bSubObjType)
								{
									case 1 :
										g_MainCharInfo.PlayInterfaceSound( SELECT_GUMYUNG_SOUND	);
										break;
									case 2 :
										g_MainCharInfo.PlayInterfaceSound( SELECT_YUNRANG_SOUND	);
										break;
									case 3 :
										g_MainCharInfo.PlayInterfaceSound( SELECT_MOOTOO_SOUND	);
										break;
									case 4 :
										g_MainCharInfo.PlayInterfaceSound( SELECT_YACHA_SOUND	);
										break;
								}
							}
						}
						break;
					case eCST_Stay:
						{
							// 鞐嗠崢瓯措嵃,, 雼るジ 旌愲Ν韯半ゼ 靹犿儩頃?靾?鞛堧弰搿?頃橃瀽.
							if( m_bCanSelectCharacter )
							{
								// Mouse Picking
								Vector3	vStart = g_pCurrentCamera->GetCursorWorld( 0.0001f);	// 0頃橁碃 1欤茧┐ 雭濎灔鞚挫暭~
								Vector3 vEnd   = g_pCurrentCamera->GetCursorWorld( 0.9999f);

								// 順勳灛 旌愲Ν韯瓣皜 靹犿儩霅愳潉霑?毵堨毎鞀る�� 鞗�歆侅澊氅?雼るジ 旌愲Ν韯办潣 鞚措�勳�?氤挫澊歆�毵?
								// 鞝曥瀾 攴?旌愲Ν韯瓣皜 靹犿儩鞚?鞎堧悳雼? 臧�旯岇毚 瓯半Μ鞐愳瀳電?旌愲Ν韯半ゼ 靹犿儩頃橁矊 頃橂┐ 頃搓舶霅?

								// 旌愲Ν韯?靹犿儩 靾滌劀毳?鞝曧暅雼?鞗愱卑毽�攵�韯? 鞚搓贝 臧勲嫧頃橂媹旯?瓯?頃橂摐旖旊敥. ^^;
								int nCharSelectOrderArray[3];
								switch( m_byCurrentCharIndex )	// 鞛愳嫚鞚� 毵?雮橃�戩溂搿�.
								{
								case 0:
									nCharSelectOrderArray[0] = 1;
									nCharSelectOrderArray[1] = 2;
									nCharSelectOrderArray[2] = 0;
									break;
								case 1:
									nCharSelectOrderArray[0] = 0;
									nCharSelectOrderArray[1] = 2;
									nCharSelectOrderArray[2] = 1;
									break;
								case 2:
									nCharSelectOrderArray[0] = 0;
									nCharSelectOrderArray[1] = 1;
									nCharSelectOrderArray[2] = 2;
									break;
								};

								// Check OnMouse
								BYTE byCurOnMouse = m_byCurrentCharIndex;
								m_byOnMouseCharIndex = 255;

								for(i=0; i<3; i++)
								{
									int nIndex = nCharSelectOrderArray[i];

									if( !m_CharRender[nIndex].m_bGray )
									{
										Vector3 vDistance = vStart - m_CharRender[nIndex].m_WorldBound.m_Center;
										//float fDistance = vDistance.GetLength();

										// OnMouse
										if( m_CharRender[nIndex].m_WorldBound.IsIntersect( vStart, vEnd, NULL) )
											byCurOnMouse = nIndex;
									}// if

									m_CharRender[ i ].EnableGlowEffect( FALSE );
									m_CharRender[ i ].m_bShowObjectName = false;
								}// for

								m_CharRender[ m_byCurrentCharIndex ].EnableGlowEffect( TRUE );

								// 順勳灛 靹犿儩霅?旌愲Ν韯瓣皜 鞎勲媹氅?鞚措�� 於滊牓瓿?韥措Ν鞚?頃?靾?鞛堧嫟.
								if( byCurOnMouse != m_byCurrentCharIndex )
								{
									// 順勳灛 靹犿儩霅?旌愲Ν韯办棎 glow臧� 鞝侅毄霅橁碃 OnMouse霅?旌愲Ν韯瓣皜 鞛堨溂氅?鞚搓矁鞚?glow霅滊嫟.
									m_CharRender[ m_byCurrentCharIndex ].EnableGlowEffect( FALSE );
									m_CharRender[ byCurOnMouse ].EnableGlowEffect(TRUE);
									m_CharRender[ byCurOnMouse ].m_bShowObjectName = true;

									m_byOnMouseCharIndex = byCurOnMouse;

									// ClickMouse
									if( XiahInput::g_bLButtonDown )
									{
										// 旮办〈鞐?鞎犽媹氅旍澊靺橂悩電?瓴冹潉 鞝曥��頃滊嫟.
										m_CharRender[m_byCurrentCharIndex].SetAnimation( XiahAniType::eLAT_Stand, 0 );

										g_MainCharInfo.PlayInterfaceSound( ISOUND_ZOOM_IN );

										m_byCurrentCharIndex = byCurOnMouse;
										m_byOnMouseCharIndex = 255;

										m_CharRender[ m_byCurrentCharIndex ].m_bShowObjectName = false;
										m_bCharacterSelected = true;
										m_nCharacterSelectStep = eCST_ZoomIn;

										// 鞓るジ飒?鞎勲嫎 鞕检�届溂耄� 須岇爠頃犼箤
										Vector3 vV1 = m_CharRender[m_byCurrentCharIndex].m_Position - g_XiahCamera.m_vAt;
										Vector3 vV2 = g_XiahCamera.m_vFrom - g_XiahCamera.m_vAt;

										vV1.Normalize();
										vV2.Normalize();

										Vector3 vTemp = vV2.Cross( vV1 );
										float fDot = vTemp.Dot( Vector3( 0, 1, 0) );

										if( fDot > 0.0f ) m_bMoveCameraRight = TRUE;
										else m_bMoveCameraRight = FALSE;

										// 旃措�旊澕鞐� 雽�頃?旌愲Ν韯办潣 鞙勳箻 臧侂弰
										vV1 = m_CharRender[m_byCurrentCharIndex].m_Position - g_XiahCamera.m_vAt;
										vV2 = g_XiahCamera.m_vFrom - g_XiahCamera.m_vAt;
										vV1.y = 0;
										vV2.y = 0;
										vV1.Normalize();
										vV2.Normalize();
										m_fYAngleWhenClicked = vV2.GetAngle( vV1 );

										if( m_bMoveCameraRight )	// 鞙勳箻 氤挫爼鞚?鞙勴暅 氇╉憸 臧?
										{
											// 旃措�旊澕臧� 霃岇晞臧堧晫 2氩堨Ц, 3氩堨Ц 旌愲Ν韯瓣皜 旃措�旊�?鞝曤┐鞚?氤措弰搿?臧侂弰毳?靾橃爼.
											if( m_byCurrentCharIndex == 1 )
												m_fYAngleWhenClicked += 0.15f;
											else
											if( m_byCurrentCharIndex == 2 )
												m_fYAngleWhenClicked += 0.035f;

											m_fCameraTargetYAngle = g_XiahCamera.m_fYAngle + m_fYAngleWhenClicked;
										}
										else
										{
											if( m_byCurrentCharIndex == 1 )
												m_fYAngleWhenClicked -= 0.19f;
											else
											if( m_byCurrentCharIndex == 2 )
												m_fYAngleWhenClicked -= 0.055f;

											m_fCameraTargetYAngle = g_XiahCamera.m_fYAngle - m_fYAngleWhenClicked;
										}

										m_fCameraRotateYAngle = 0;
										g_XiahCamera.m_fDestXAngle = -_PI / 15;
									}// if

								}// if

							}// if( m_bCanSelectCharacter )

							// 旌愲Ν韯?靹犿儩 旆�靻�. ESC Key毳?雸勲ゴ氅?鞚措爣瓴?霅橂姅雿?瓯?鞓奠厴 韨?
							if( GetAsyncKeyState( VK_ESCAPE ) < 0)
							{
								// 旮办〈鞐?鞎犽媹氅旍澊靺橂悩電?瓴冹潉 鞝曥��頃滊嫟.
								m_CharRender[m_byCurrentCharIndex].SetAnimation( XiahAniType::eLAT_Stand, 0 );

								// 旌愲Ν韯?靹る獏 彀?
								g_pUIManager->Hide(INTRO_WINDOW01);

								// 旌愲Ν韯?毵岆摛旮?
								if( m_byCharCount < 3 )
								{
									g_pUIManager->Show(INTRO_BUTTONSET, INTRO_BUTTON_01);
									g_pUIManager->SetString(INTRO_BUTTONSET, INTRO_BUTTON_01, IDS_INTRO_BUTTONSET_BUTTON11);
								}
								else
								{
									g_pUIManager->Hide(INTRO_BUTTONSET, INTRO_BUTTON_01);
								}
								// 鞁滌瀾
								g_pUIManager->Hide(INTRO_BUTTONSET, INTRO_BUTTON_04);
								// 歆�鞖瓣赴
								g_pUIManager->Hide(INTRO_BUTTONSET, INTRO_BUTTON_03);

								m_nCharacterSelectStep = eCST_ZoomOut;
								g_XiahCamera.m_fDestXAngle = -0.43633235f; //-_PI / 4.5f;

								g_MainCharInfo.PlayInterfaceSound( ISOUND_ZOOM_OUT );
							}// if
						}
						break;
					case eCST_ZoomOut:	// 雼れ嫓 氅�鞏挫�勲�?
						{
							if( g_XiahCamera.m_fDistance < 38 )
								g_XiahCamera.m_fDistance += (fTime * fDistSpeed);
							else
							{
								g_XiahCamera.m_fDistance = 38;
								m_bCharacterSelected = false;
							}

							g_XiahCamera.m_fDestDistance = g_XiahCamera.m_fDistance;
							g_XiahCamera.RotateY( FALSE, 0.25f );
						}
						break;
					};// switch

				}// if

				if(!g_XiahCamera.Update())
				{
					DBG_LogFile( _T("g_XiahCamera.Update 鞁ろ尐"));
				}

			}
			break;
		}; // switch

		// 氚瓣步鞚� 氚橂摐鞁?旃措�旊澕臧� 靹疙寘霅滊嫟鞚岇棎 Update鞁滌紲欷�雼?
		if(!XiahMap::g_XiahMap.Update())
		{
			DBG_LogFile( _T("XiahMap::g_XiahMap.Update 鞁ろ尐"));

//			return false;
		}

		g_EffectManager.UpdateEffect(33.0f * g_fFrameScale);

		g_MainCharInfo.Update();
	}  //if( m_bFirstGameLoadScreen )

	// test
	//    g_XiahCamera.ProcessKeyboard();
	// test

	if(!Fade::UpdateFade())
	{
		DBG_LogFile( _T("Fade::UpdateFade 鞁ろ尐"));

//		return false;
	}

	return TRUE;
}

/////////////////////////////
BOOL XiahGame_Intro::Render()
/////////////////////////////
// BeginScene, EndScene鞎堨棎靹?攴鸽Μ旮?頃�靾�
{
	// 鞎勳�� 觳橃潓 瓴岇瀯 搿滊敥 頇旊┐鞚茧晫.
	if( m_bFirstGameLoadScreen )
	{
		// 2004.07.20 鞚措菠韸胳毄 搿滊敥頇旊┐
		/*
		if( rand() % 2 )
			g_pUIManager->ForwardShow( EVENT_LOADING_1 );
		else
			g_pUIManager->ForwardShow( EVENT_LOADING_2 );
		*/

		g_pUIManager->ForwardShow(LOADING_IMAGE3); //HO_0702_07 霌标笁響滌嫓 : 霌标笁響滌嫓鞕� 頃�瓴� 鞀ろ儉韸鸽�滊敥瓿� 瓴岇瀯搿滊敥 攵�攵勳澊 霃欖澕 鞚措�胳��搿?觳橂Μ霅滊嫟.

		//霌标笁響滌嫓 鞝侅毄鞝?旖旊摐 雮橃�戩�?歆�鞗?氩勲Μ鞛?..; 霌标笁響滌嫓 鞝勳棎電?雮橃澊 甑�攵勳�?鞛堨棁雼?..
		// 靹膘澑 靹滊矂鞖?搿滊敥
		//if(g_AppData.m_bAdult)
		//	g_pUIManager->ForwardShow(LOADING_IMAGE2);
		//else
		//	g_pUIManager->ForwardShow(LOADING_IMAGE);

		if( m_bIntroFirstCall )
		{
			ChangeXiahCursor( eCT_General);

			m_bIntroFirstCall = false;
			InitIntro( m_byIntroFirstCallCurMenu );
		}
	}
	else
	{
		// CG_2005/05/26 : 鞀れ勾鞚措У 觳挫澑歆�
		g_Sky.Render();
		//g_SkyBox.Render();

		if(!XiahMap::g_XiahMap.RenderTerrain())
		{
			DBG_LogFile( _T("XiahGame_Intro::Render 鞁ろ尐"));

//			return false;
		}

		//XiahMap::g_XiahMap.RenderWater();

		if(!XiahMap::g_XiahMap.RenderObject(1))	// no alpha
		{
			DBG_LogFile( _T("XiahGame_Intro::Render 鞁ろ尐"));

//			return false;
		}

		if(!XiahMap::g_XiahMap.RenderObject(3))	// alpha
		{
			DBG_LogFile( _T("XiahGame_Intro::Render 鞁ろ尐"));

//			return false;
		}

		if(!XiahMap::g_XiahMap.RenderObject(2))	// alpha test
		{
			DBG_LogFile( _T("XiahGame_Intro::Render 鞁ろ尐"));

//			return false;
		}

		switch( m_byCurrentMenu )
		{
		case INTROMENU_CREATECHAR:
			{
				Matrix4x4 iTM;
				g_pDirect3DDevice->SetTransform( D3DTS_WORLD, (D3DMATRIX *)&iTM);
				g_pDirect3DDevice->SetRenderState( D3DRS_FOGENABLE, TRUE);
				g_pDirect3DDevice->SetRenderState( D3DRS_ZENABLE, TRUE);
				g_pDirect3DDevice->SetRenderState( D3DRS_ZWRITEENABLE, TRUE);

				switch( m_nCurrentCharacter )
				{
				case 1:
					if( m_GyumYung.m_CharRender.IsValid() )
						m_GyumYung.Render();
					break;
				case 2:
					if( m_YunRang.m_CharRender.IsValid() )
						m_YunRang.Render();
					break;
				case 3:
					if( m_MooToo.m_CharRender.IsValid() )
						m_MooToo.Render();
					break;
				case 4:
					if( m_YaCha.m_CharRender.IsValid() )
						m_YaCha.Render();
					break;
				};// switch
			}
			break;
		case INTROMENU_SELECTCHAR:
			g_pDirect3DDevice->SetRenderState( D3DRS_FOGENABLE, TRUE);
			g_pDirect3DDevice->SetRenderState( D3DRS_ZENABLE, TRUE);
			g_pDirect3DDevice->SetRenderState( D3DRS_ZWRITEENABLE, TRUE);

			// 靸濎劚霅?氇�霌� 旌愲Ν韯半ゼ 攴鸽Π雼?
			for(int i=1; i<=m_byCharCount; i++)
			{
				Matrix4x4 iTM;
				if( m_CharRender[i-1].m_CharRender.IsValid())
				{
					g_pDirect3DDevice->SetTransform( D3DTS_WORLD, (D3DMATRIX *)&iTM);

					if(!m_CharRender[i-1].Render())
					{
						DBG_LogFile( _T("XiahGame_Intro::Render 鞁ろ尐"));

//						return false;
					}
				}
			}// for

			break;
		};

		g_pDirect3DDevice->SetRenderState( D3DRS_ALPHATESTENABLE, FALSE );

		// 旌愲Ν韯?鞚措��
		if( m_byCurrentMenu == INTROMENU_SELECTCHAR && m_byOnMouseCharIndex != 255 )
		{
			m_CharRender[ m_byOnMouseCharIndex ].m_tObjectName.Render();
		}

		g_EffectManager.RenderEffect( 33.0f * g_fFrameScale );
		g_EffectManager.RenderOthers( 33.0f * g_fFrameScale );

		g_pDirect3DDevice->SetRenderState( D3DRS_FOGENABLE, FALSE);

		g_MainCharInfo.Render();

		// 牍欗檾歆�雽� 雸?
		g_RainSnow.Render();
	} // if( m_bFirstGameLoadScreen )

	Fade::RenderFade();

	return TRUE;
}

/**
 *
 * \param byCurrentMenu 
 * \return 
 */
HRESULT XiahGame_Intro::InitIntro( BYTE byCurrentMenu)
{
	// 觳�鞐� 霌れ柎鞓る┐ 頃滊矆 Update, Render毳?瓯办硱鞎?瓴岇瀯 搿滊敥 頇旊┐鞚?雮橃槰雼? 攴鸽灅靹?頃滊皵雬?霃堧嫟.
	if( m_bIntroFirstCall )
	{
		m_byIntroFirstCallCurMenu = byCurrentMenu;
	}
	else
	{
		// 鞚疙姼搿滌棎 靷�鞖╇�?旌愲Ν韯瓣皜 靸濎劚霅橃�� 鞎婌晿雼る┐.
		if( !m_bIntroInitCharDataCreated )
			CreateInitCharData();

		g_MainCharInfo.CloseFrame(LOADING_IMAGE3); //HO_0702_07 霌标笁響滌嫓 : 霌标笁響滌嫓鞕� 頃�瓴� 鞀ろ儉韸鸽�滊敥瓿� 瓴岇瀯搿滊敥 攵�攵勳澊 霃欖澕 鞚措�胳��搿?觳橂Μ霅滊嫟.
		
		//霌标笁響滌嫓 鞝侅毄鞝?旖旊摐 雮橃�戩�?歆�鞗?氩勲Μ鞛?..; 鞚挫牅電?頃滌灔毵?靷�鞖╉晿雼堦�?..
		//g_MainCharInfo.CloseFrame(LOADING_IMAGE);
		//g_MainCharInfo.CloseFrame(LOADING_IMAGE2);
		

		// 2004.07.20 鞚措菠韸胳毄 搿滊敥頇旊┐
		//g_MainCharInfo.CloseFrame( EVENT_LOADING_1 );
		//g_MainCharInfo.CloseFrame( EVENT_LOADING_2 );

		// 毵?搿滊敥
		XiahMap::g_XiahMap.CreateMap(3, IDS_FIRST_MAP, 0, 2048, 2048);

		g_XiahCamera.m_bIntro		 = true;
		g_GameWork.m_nNavigationMode = 1;

		/*
		g_XiahCamera.m_fXAngle = -_PI / 4.5f;
		g_XiahCamera.m_fYAngle = -_PI;
		g_XiahCamera.m_fDistance = 38;
		g_XiahCamera.m_fDestXAngle = g_XiahCamera.m_fXAngle;
		g_XiahCamera.m_fDestDistance = g_XiahCamera.m_fDistance;

		// 旃措�旊�?靹れ爼
		float x,y;
		x = 1432; //630; //570;
		y = -792; //1450;

		float height = Map::g_MapRes.GetHeight( x, y);

		Vector3 vAt( x, height + 4, y);
		Vector3 vFrom( 5, 3, -20);

		vFrom = vAt + vFrom;
		*/
		g_XiahCamera.m_fXAngle		 = -0.43633235f; //-_PI;// / 1.5f;
		g_XiahCamera.m_fYAngle		 = -870.81592f; //-_PI;
		g_XiahCamera.m_fDistance	 = 45;//38;
		g_XiahCamera.m_fDestXAngle	 = g_XiahCamera.m_fXAngle;
		g_XiahCamera.m_fDestDistance = g_XiahCamera.m_fDistance;

		// 旃措�旊�?靹れ爼
		//float x,y;
		//x = 1299; //630; //570;
		//y = -1299; //1450;

		//float height = Map::g_MapRes.GetHeight(x, y);

		Vector3 vAt; //( x, height + 30, y);
		Vector3 vFrom; //( 5, 3, -20);

		//vFrom = vAt + vFrom;

		// test code
		vAt.x = 1626.0448f;
		vAt.y = 85.000000f;
		vAt.z = -1243.9867f;

		vFrom.x = 1628.9955f;
		vFrom.y = 104.23999f;
		vFrom.z = -1202.4309f;

		/*
		g_XiahCamera.m_fXAngle = -0.26179945f; //-_PI / 4.5f;
		g_XiahCamera.m_fYAngle = -_PI;
		g_XiahCamera.m_fDistance = 38;
		g_XiahCamera.m_fDestXAngle = g_XiahCamera.m_fXAngle;
		g_XiahCamera.m_fDestDistance = g_XiahCamera.m_fDistance;

		float x,y;
		x = 1292; //994;
		y = -1292; //1527;

		float height = Map::g_MapRes.GetHeight( x, y);

		Vector3 vAt( x, height + 20, y);
		Vector3 vFrom( 3, 1, 0 );

		vFrom = vAt + vFrom;
		*/

		g_XiahCamera.SetView( vFrom, vAt, Vector3( 0, 1, 0));
#ifdef MINI
		g_XiahCamera.SetProjection( _PI / 4, (float)G_HEIGHT / (float)G_WIDTH, 1.0f, 256.0f);
#else
		g_XiahCamera.SetProjection( _PI / 4, 768.0f / 1024.0f, 1.0f, 256.0f);
#endif

		//
		g_XiahEnvInfo.m_bFog = TRUE;
		g_XiahEnvInfo.m_FogColor = D3DCOLOR_XRGB(236, 239, 255);
		g_pDirect3DDevice->SetRenderState( D3DRS_FOGENABLE, FALSE );

		// 氚旊�?旃措�旊�?靹れ爼鞚?毵奠棎 鞝侅毄
		XiahMap::g_XiahMap.m_pMapRender->Update();

		static BOOL oncecall = TRUE;
		if( oncecall)
		{
			Init_Frames();
			oncecall = FALSE;
		}

		// 觳橃潓 搿滊敥頃犽晫 搿滊敥 鞚措�胳�� 雭?
		m_bFirstGameLoadScreen = false;

		g_pUIManager->Hide(LOADING_IMAGE3); //HO_0702_07 霌标笁響滌嫓 : 霌标笁響滌嫓鞕� 頃�瓴� 鞀ろ儉韸鸽�滊敥瓿� 瓴岇瀯搿滊敥 攵�攵勳澊 霃欖澕 鞚措�胳��搿?觳橂Μ霅滊嫟.
		//霌标笁響滌嫓 鞝侅毄鞝?旖旊摐 雮橃�戩�?歆�鞗?氩勲Μ鞛?..; 鞚挫牅電?頃滌灔毵?靷�鞖╉晿雼堦�?..
		//g_pUIManager->Hide(LOADING_IMAGE);
		//g_pUIManager->Hide(LOADING_IMAGE2);		

		// 2004.07.20 鞚措菠韸胳毄 搿滊敥頇旊┐
		//g_pUIManager->Hide( EVENT_LOADING_1 );
		//g_pUIManager->Hide( EVENT_LOADING_2 );

		switch( byCurrentMenu)
		{
		case INTROMENU_CREATECHAR:
			return GoToMenuCreateChar();
		case INTROMENU_SELECTCHAR:
			return GoToMenuSelectChar();
		}
	}// if( m_bIntroFirstCall )

	return 0;
}

/**
 *
 * \return 
 */
HRESULT XiahGame_Intro::ClearIntro()
{
	g_pUIManager->Hide(INTRO_BUTTONSET);
	g_pUIManager->Hide(INTRO_CHARACTER_SELECT);
	g_pUIManager->Hide(INTRO_WINDOW01);

	g_pUIManager->Show(MAIN_FRAME);
	g_pUIManager->SetPosition(SMALL_MESSENGER, SMALL_MESSENGER_XPOS, SMALL_MESSENGER_YPOS);
	g_pUIManager->SetPosition(LARGE_MESSENGER, LARGE_MESSENGER_XPOS, LARGE_MESSENGER_YPOS);
	g_pUIManager->SetPosition(MAIN_CHAT, MAIN_CHAT_XPOS, MAIN_CHAT_YPOS);

	g_MainCharInfo.RefreshMainFrame();//鞚错伂

	g_pUIManager->Show(SMALL_MESSENGER);
	g_pUIManager->ForwardShow(MAIN_CHAT);
	g_pUIManager->Show(SITUATION_BRA);
	g_pUIManager->Show(DATA_WINDOW);
	g_pUIManager->Show(PET_BUTTON_GROUP);

	g_XiahCamera.m_bIntro = false;

	// 鞚疙姼搿滌棎靹?靸濎劚頃?旌愲Ν韯?雿办澊韯半ゼ 歆�鞖措嫟.
	for(int i=0; i<CHARACTER_MAX; i++)
		m_CharRender[i].Release();

//	m_GyumYung.Release();
//	m_YunRang.Release();
//	m_MooToo.Release();
//	m_YaCha.Release();

	return TRUE;
}

/**
 *
 * \return 
 */
HRESULT XiahGame_Intro::GoToMenuCreateChar()
{
	SetCurrentMenu( INTROMENU_CREATECHAR);

	// XiahBGM::Stop_BGM();

	m_bIntroBGMPlay = false;

	// 旌愲Ν韯半ゼ 靸濎劚頃橂┐ 旌愲Ν韯半ゼ 靹犿儩頃犽晫 氙鸽Μ 鞚诫姅 瓴冹潉 鞐嗢暊鞛?
	m_bFirstCharSelect = false;

	// 旌愲Ν韯?靹る獏 彀?
	g_pUIManager->Hide(INTRO_WINDOW01);
	// 旌愲Ν韯?毵岆摛旮?
	g_pUIManager->Hide(INTRO_BUTTONSET, INTRO_BUTTON_01);
	// 鞁滌瀾
	g_pUIManager->Hide(INTRO_BUTTONSET, INTRO_BUTTON_04);
	// 歆�鞖瓣赴
	g_pUIManager->Hide(INTRO_BUTTONSET, INTRO_BUTTON_03);
	// 醫낅즺
	g_pUIManager->Hide(INTRO_BUTTONSET, INTRO_BUTTON_02);

	g_pUIManager->SetFocus(INTRO_ACCOUNT);
	g_pUIManager->SetFocus(INTRO_ACCOUNT, account_name_edit);

	// interface menu
	g_pUIManager->Hide(INTRO_CHARACTER_SELECT);
	g_pUIManager->Hide(INTRO_ACCOUNT);

	//static BOOL bFade1 = FALSE;
	//bFade1 = FALSE;

//	SetCurrentClassInfo( 1);

//	if( !bFade1)
	{
		m_bCharacterCreatePreLoaded = true;
		m_bOri = true;	// interface menu

		m_dwCameraMoveStartTime = 0;
		m_nCharacterCreateStep = eCCType_GyumYung_Rotate;
		m_nCurrentCharacter = 1;	// 瓴�鞓?

		// camera setting
		g_XiahCamera.m_fXAngle   = -0.0052664680f; //-_PI / 2.2f; //-_PI / 12; //-_PI / 6;
		g_XiahCamera.m_fYAngle   = -1185.1012f; //-_PI / 2;
		g_XiahCamera.m_fDistance = 13.5f; //300;

		g_XiahCamera.m_fDestXAngle = g_XiahCamera.m_fXAngle;
		g_XiahCamera.m_fDestDistance = g_XiahCamera.m_fDistance;

		// 旃措�旊�?靹れ爼
		//Vector3 vAt( 859, 169, -1672 );
		//Vector3 vFrom( 3, 1, 0 );

		/*
		float x,y;
		x = 1292; //994;
		y = -1292; //1527;

		float height = Map::g_MapRes.GetHeight( x, y);
		*/

		Vector3 vAt; //( x, height + 20, y);
		Vector3 vFrom; //( 3, 1, 0 );

		//vFrom = vAt + vFrom;

		vAt.x = 1400.8101f;
		vAt.y = 145.00000f;
		vAt.z = -754.49518f;

		vFrom.x = 1390.1307f;
		vFrom.y = 145.00000f;
		vFrom.z = -742.58215f;

		g_XiahCamera.SetView( vFrom, vAt, Vector3( 0, 1, 0));
#ifdef MINI
		g_XiahCamera.SetProjection( _PI / 4, (float)G_HEIGHT / (float)G_WIDTH, 1.0f, 256.0f);
#else
		g_XiahCamera.SetProjection( _PI / 4, 768.0f / 1024.0f, 1.0f, 256.0f);
#endif


		// Original Code
/*
		// 頇旊┐鞚?鞏措憪鞖鸽晫 旃措�旊澕毳� 鞗�歆侅棳靹?毵奠潉 氙鸽Μ 鞚诫姅 須�瓿茧�?雮挫瀽. 
		// 攴鸽灅靹?鞚茧秬霟?旮戈矊 韼橃澊霌?鞎勳泝.
		DWORD dwTime;
		if( !m_bCharacterCreatePreLoaded )
			dwTime = 420000;	// 7攵?
		else
			dwTime = 1100;
		Fade::StartFade( 0, 0, NULL, dwTime );
		bFade1 = TRUE;

		//
		m_dwCameraMoveStartTime = 0;
		m_nCharacterCreateStep = eCCType_GyumYung;
		m_nCurrentCharacter = 1;	// 瓴�鞓?

		// camera setting
		g_XiahCamera.m_fXAngle = -_PI / 2.2f; //-_PI / 12; //-_PI / 6;
		g_XiahCamera.m_fYAngle = -_PI / 2;
		g_XiahCamera.m_fDistance = 300;

		g_XiahCamera.m_fDestXAngle = g_XiahCamera.m_fXAngle;
		g_XiahCamera.m_fDestDistance = g_XiahCamera.m_fDistance;

		// 旃措�旊�?靹れ爼
		float x,y;
		x = 994;
		y = -1527;

		float height = Map::g_MapRes.GetHeight( x, y);

		Vector3 vAt( x, height + 20, y);
		Vector3 vFrom( 3, 1, 0 );

		vFrom = vAt + vFrom;

		g_XiahCamera.SetView( vFrom, vAt, Vector3( 0, 1, 0));
#ifdef MINI
		g_XiahCamera.SetProjection( _PI / 4, (float)G_HEIGHT / (float)G_WIDTH, 1.0f, 256.0f);
#else
		g_XiahCamera.SetProjection( _PI / 4, 768.0f / 1024.0f, 1.0f, 256.0f);
#endif
*/


		// 氚旊�?旃措�旊�?靹れ爼鞚?毵奠棎 鞝侅毄
		if(!XiahMap::g_XiahMap.m_pMapRender->Update())
		{
			DBG_LogFile( _T("XiahGame_Intro::GoToMenuCreateChar 鞁ろ尐"));

//			return 0;
		}
	}

	return 0;
}

/**
 *
 * \return 
 */
HRESULT XiahGame_Intro::GoToMenuSelectChar()
{
//	if( m_byCharCount == 0)
//		return 0;

	if( !m_bIntroBGMPlay )
	{
		// 旒�霠夗劙 毵岆摛旮办棎靹?氅旍澑 氅旊壌搿?霃岇晞鞓�瓴届�?
		//XiahBGM::Play_BGM("sound\\bgm\\intro.mp3",1);
		m_bIntroBGMPlay = true;
	}

	m_bCanSelectCharacter = true; 

	// camera
	g_XiahCamera.m_bIntro = true;

	g_XiahCamera.m_fXAngle		 = -0.43633235f; //-_PI;// / 1.5f;
	g_XiahCamera.m_fYAngle		 = -870.81592f; //-_PI;
	g_XiahCamera.m_fDistance	 = 45;//38;
	g_XiahCamera.m_fDestXAngle	 = g_XiahCamera.m_fXAngle;
	g_XiahCamera.m_fDestDistance = g_XiahCamera.m_fDistance;

	// 旃措�旊�?靹れ爼
	//float x,y;
	//x = 1299; //630; //570;
	//y = -1299; //1450;

	//float height = Map::g_MapRes.GetHeight(x, y);

	Vector3 vAt; //( x, height + 30, y);
	Vector3 vFrom; //( 5, 3, -20);

	//vFrom = vAt + vFrom;

	// test code
	vAt.x = 1626.0448f;
	vAt.y = 85.000000f;
	vAt.z = -1243.9867f;

	vFrom.x = 1628.9955f;
	vFrom.y = 104.23999f;
	vFrom.z = -1202.4309f;

	g_XiahCamera.SetView( vFrom, vAt, Vector3( 0, 1, 0));
#ifdef MINI
	g_XiahCamera.SetProjection( _PI / 4, (float)G_HEIGHT / (float)G_WIDTH, 1.0f, 256.0f);
#else
	g_XiahCamera.SetProjection( _PI / 4, 768.0f / 1024.0f, 1.0f, 256.0f);
#endif

	//
	g_pUIManager->SetString(INTRO_ACCOUNT, account_name_edit, _T(""));

	SetCurrentMenu( INTROMENU_SELECTCHAR);

	//
	g_pUIManager->Hide(INTRO_CHARACTER_SELECT);
	g_pUIManager->Hide(INTRO_ACCOUNT);
	g_pUIManager->Show(INTRO_BUTTONSET);

	// 靸堧�� 毵岆摛旮?
	if( m_byCharCount < 3 )
	{
		g_pUIManager->Show(INTRO_BUTTONSET, INTRO_BUTTON_01);
		g_pUIManager->SetString(INTRO_BUTTONSET, INTRO_BUTTON_01, IDS_INTRO_BUTTONSET_BUTTON11);
	}
	else
		g_pUIManager->Hide(INTRO_BUTTONSET, INTRO_BUTTON_01);

	// 鞁滌瀾
	g_pUIManager->Hide(INTRO_BUTTONSET, INTRO_BUTTON_04);// Show( FALSE );
	// 毂勲剱靹犿儩
	g_pUIManager->Show(INTRO_BUTTONSET, INTRO_BUTTON_03);
	// 醫낅즺
	g_pUIManager->Show(INTRO_BUTTONSET, INTRO_BUTTON_02);
	g_pUIManager->SetString(INTRO_BUTTONSET, INTRO_BUTTON_02, IDS_TERMINATE);
	// 旌愲Ν韯?靹る獏 彀届潃 觳�鞐� 靾�旮措�?
	g_pUIManager->Hide(INTRO_WINDOW01);
	//g_pUIManager->Show(INTRO_WINDOW01);


	SetCurrentCharInfo( 0);
	SetCurrentCharInfo( 1);
	SetCurrentCharInfo( 2);

	static BOOL bFade2 = FALSE;
	bFade2 = FALSE;

	if( !bFade2 && m_bFirstCharSelect )
	{
		Fade::StartFade( 0, 0, NULL, 50000 );
		bFade2 = TRUE;
		m_bFirstCharSelect = true;
	}
	else
	if( !m_bFirstCharSelect )
		Fade::StartFade( 0, 0, NULL, 450 );

	g_MainCharInfo.DeleteAllScrMessage();

	// 牍欗檾歆�雽� 雸?
	g_RainSnow.Start(eSnow);

	return 0;
}













////////////////////////////////////////////////////////////
// Create Character
////////////////////////////////////////////////////////////
void XiahGame_Intro::SetCurrentClass( BYTE byIndex)
{
	WORD wEquipVisualID[VISUALID_NUM];
	ZeroMemory( wEquipVisualID, sizeof( WORD) * VISUALID_NUM);
		
	switch( byIndex)
	{
		// 瓴�鞓?
	case 1:
		m_byCurrentClassIndex = 1;
		m_CharRender[m_byCurrentCharIndex].Create( 790, 1, 0, 103);
		m_CharRender[m_byCurrentCharIndex].m_bSubObjType = 1;
		SetupPC_VisualEquipement(&m_CharRender[m_byCurrentCharIndex], wEquipVisualID);
		break;

		//鞐半瀾
	case 2:
		m_byCurrentClassIndex = 2;
		m_CharRender[m_byCurrentCharIndex].Create( 867, 2, 0, 103);
		m_CharRender[m_byCurrentCharIndex].m_bSubObjType = 2;
		SetupPC_VisualEquipement(&m_CharRender[m_byCurrentCharIndex], wEquipVisualID);
		break;
	}
	
}

void XiahGame_Intro::SetCurrentClassInfo( BYTE byCurrentClassIndex)
{
	g_MainCharInfo.Create();
	m_byCurrentClassIndex = byCurrentClassIndex;

	SetCurrentClass( byCurrentClassIndex);

}











////////////////////////////////////////////////////////////
// Select Character
////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////
void	XiahGame_Intro::SetCurrentCharInfo( BYTE byIndex)
/////////////////////////////////////////////////////////
{
//	g_MainCharInfo.Create();
	SetCurrentCharIndex( byIndex);
//	SetCurrentCharInfoToScr();

	// 3D Character 靹疙寘
	CharacterInfo *pCharInfo = m_CharacterList[ byIndex];

	if(pCharInfo == NULL)
	{
		DBG_LogFile( _T("XiahGame_Intro::SetCurrentCharInfo 鞁ろ尐"));

		return;
	}

	sArrayData *pVisualData = XiahArrayIndex::g_MainCharType.GetData( pCharInfo->m_bCharType, 0/*pCharInfo->m_wLevel / (50 / 3)*/);

	if( pVisualData == NULL )
	{
		m_CharRender[byIndex].Create( 790, 1, 0, 103);
		m_CharRender[byIndex].m_bSubObjType = 1;
		m_CharRender[byIndex].m_bGray = true;

		WORD EquipVID[9];
		memset( EquipVID, 0, sizeof(EquipVID) );

		SetupPC_VisualEquipement( &m_CharRender[byIndex], EquipVID );
		return;
	}

	int nCharID = pVisualData->GetInt( 2);
	int nMeshType = pVisualData->GetInt( 3);
	int nTextureType = pVisualData->GetInt( 4);
	
	m_CharRender[m_byCurrentCharIndex].Create( nCharID, nMeshType, nTextureType, 103);
	m_CharRender[m_byCurrentCharIndex].m_bSubObjType = pCharInfo->m_bCharType;
	m_CharRender[m_byCurrentCharIndex].m_bGray = false;
	m_CharRender[m_byCurrentCharIndex].m_szObjectName = pCharInfo->m_szNickName;

	SetupPC_VisualEquipement( &m_CharRender[m_byCurrentCharIndex], pCharInfo->wEquipVisualID, pCharInfo->m_bRarity, pCharInfo->m_bStxType );

	m_CharRender[m_byCurrentCharIndex].m_pAniType = XiahAniType::GetAniType( OBJTYPE_PC, 0);
}

/////////////////////////////////////////////////
void	XiahGame_Intro::SetCurrentCharInfoToScr()
/////////////////////////////////////////////////
{
	// 旌愲Ν韯?靹る獏 彀?
	g_pUIManager->Show(INTRO_WINDOW01);
	
	// 旌愲Ν韯?毵岆摛旮?
	if( m_bCharacterSelected)
	{
		g_pUIManager->Show(INTRO_BUTTONSET, INTRO_BUTTON_01);
		g_pUIManager->SetString(INTRO_BUTTONSET, INTRO_BUTTON_01, IDS_RESELECT);
	}

	// 鞁滌瀾
	g_pUIManager->Show(INTRO_BUTTONSET, INTRO_BUTTON_04);
	// 毂勲剱靹犿儩
	g_pUIManager->Show(INTRO_BUTTONSET, INTRO_BUTTON_03);
	// 删除角色按钮 (锁定角色后显示)
	g_pUIManager->Show(INTRO_BUTTONSET, INTRO_BUTTON_02);
	g_pUIManager->SetString(INTRO_BUTTONSET, INTRO_BUTTON_02, IDS_INTRO_BUTTONSET_BUTTON13);

	TCHAR strName[64] = {0,};
	TCHAR strLevel[64] = {0,};
	
	// 臧侅劚 彀�靾� 雮橅儉雮搓赴
	if(GetCurrentChar()->m_bRebirth)
	{
		if(GetCurrentChar()->m_bRebirth < 7)	//HT_0702 : 臧侅劚鞛?
			_stprintf( strName, IDS_REBIRTH_COUNT, (LPCTSTR)GetCurrentChar()->m_szNickName, GetCurrentChar()->m_bRebirth);
		else				//歆勱皝靹膘瀽
			_stprintf( strName, IDS_2TH_REBIRTH_COUNT, (LPCTSTR)GetCurrentChar()->m_szNickName, GetCurrentChar()->m_bRebirth - 6);

		g_pUIManager->SetString(INTRO_WINDOW01, intro_window_dummy_01, strName);
	}
	else
	{
		_stprintf( strName, IDS_D_NAME, (LPCTSTR)GetCurrentChar()->m_szNickName);
        g_pUIManager->SetString(INTRO_WINDOW01, intro_window_dummy_01, strName);
	}
//	_stprintf( strName, IDS_D_NAME, (LPCTSTR)GetCurrentChar()->m_szNickName);

	//150臧戩瀽 齑堦臣鞁?
	if(GetCurrentChar()->m_wLevel < 150)
	{
		_stprintf( strLevel, IDS_D_GABJA, GetCurrentChar()->m_wLevel);
        g_pUIManager->SetString(INTRO_WINDOW01, intro_window_dummy_02, strLevel);
	}
	else
	{
		_stprintf( strLevel, IDS_BESTLEVEL);
        g_pUIManager->SetString(INTRO_WINDOW01, intro_window_dummy_02, strLevel, 9);
	}



	// 鞓レ厴
	if(GetCurrentChar()->m_bAuction == 7)
	{
		g_pUIManager->SetString(INTRO_WINDOW01, intro_window_dummy_03, IDS_AUCTION, 7);
	}
	else
	{
		LPCTSTR lpStrMap = GetMapName(GetCurrentChar()->m_dwMapID);
		g_pUIManager->SetString(INTRO_WINDOW01, intro_window_dummy_03, lpStrMap);
	}
	
}


