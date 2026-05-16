#include "xiahbgmcore.h"

extern BOOL FadeTrigger_MainCharMapEnterAfterDie(DWORD nIndex);


/**
 *
 * \param &msg 
 * \return 
 */
int OnCS_NV_STARTGAME_ACK( CMsg &msg)
{
	BYTE	bErrCode	=0;
	BYTE	byCurrentCharIndex = g_pIntro->GetCurrentCharIndex();	
	
	// 스트링 객체는 memory copy해버리면 뻑남
	g_MainCharInfo = g_pIntro->m_CharacterList[ byCurrentCharIndex];
	g_MainCharInfo.Create();

	msg
		>> bErrCode;

	if(bErrCode != ERR_NVSTARTGAME_SUCCESS || !g_MainCharInfo.m_bCharChange) 
	{
		if(g_pUIManager)
			g_pUIManager->ShowNotice( IT_FAIL_LOGIN, NOTICE_FRAME_OK, NOTICE_FRAME_UNEXPECTED_TERMINATE);
		else
			MessageBox( GetForegroundWindow(), IT_FAIL_LOGIN, START_ERROR0, MB_OK);

		PostMessage( g_AppData.m_hWnd, WM_CLOSE, 0, 0);

		return FALSE;
	}
	else 
	{
		DWORD	dwMapID	= 0;

		msg
			>> dwMapID;
			
		SendCS_IT_MAPINFO_REQ(dwMapID);	// 맵정보 요청
		SendCS_IT_CHARSTATUSINFO_REQ();	// Character 상세정보 요청
		SendCS_IT_MUGONGLIST_REQ(0);	// 무공 리스트 요청(general)
		SendCS_IT_MUGONGLIST_REQ(1);	// 무공 리스트 요청(passive)
		//SendCS_IT_MUGONGLIST_REQ(2);	// 무공 리스트 요청(active)
		//SendCS_IT_MUGONGLIST_REQ(3);	// 각성 무공 리스트 요청(active)
		SendCS_IT_ITEMLIST_REQ( SACKTYPE__EQUIPMENT);
		SendCS_IT_ITEMLIST_REQ( SACKTYPE__DEFAULT);
		SendCS_IT_ITEMLIST_REQ( SACKTYPE__DEFAULT2);
		SendCS_IT_ITEMLIST_REQ(SACKTYPE_COLLECTION);	// 아이템 수집
		SendCS_IT_CHARSLOT_REQ();
		SendCS_RL_RELATIONLIST_REQ();
		SendCS_RL_MUNWONLIST_REQ();
		SendCS_QS_LIST_REQ();
		SendCS_OP_OPTIONLIST_REQ();		// OPTION 설정요청

		g_MainCharInfo.m_bCharChange = false;

		// 처음 로딩시
		Stop_BGM();
		g_bBGMForce = true;
	}
	
	return TRUE;
}


/**
 *
 * \param &msg 
 * \return 
 */
int OnCS_NV_ENDGAME_ACK( CMsg &msg)
{
	BYTE bStartOption	= 0;
	BYTE bChChange		= 0;

	msg	
		>> bStartOption
		>> bChChange;

	if ( bChChange )
	{		
		SET_GAMESTEP (GAMESTEP_LOGIN);

		CXiahGame_StepObject* pObject = g_GameStep[GAMESTEP_LOGIN];

		if ( pObject )
		{
			((CXiahGame_Login*)pObject)->SetStep(CXiahGame_Login::SERVER_SELECT);

			// 로그인 화면 리소스 재활당
			g_pUIManager->ReCreate(LOGIN_2);

			g_pUIManager->CloseAll();
			g_pUIManager->Show(LOGIN_2);

			// 꽃잎 떄문에
			((CXiahGame_Login*)g_GameStep[GAMESTEP_LOGIN])->SetStep(CXiahGame_Login::SERVER_SELECT);

			XiahNetwork::DisconnectFromServer();
		}	
	}

	DBG_Put(_T("EndGameACK"));

	return TRUE;
}



//---------------------------------------------------------------------------------------
/**
 *
 * \param &msg 
 * \return 
 */
int OnCS_NV_MAPOBJECTLIST_ACK( CMsg &msg)
{
	BYTE	bResult		=0;
	WORD	wNumObject	=0;
	
	msg
		>> bResult
		>> wNumObject;

	DBG_Put(_T("CS_NV_MAPOBJECTLIST_ACK : %d"), wNumObject);

	DWORD dwMapID = XiahMap::g_XiahMap.m_MapInfo.m_dwMapID;

#define CHAR_LIST			0
#define NPC_LIST			1
#define ITEM_LIST			2
#define FUNCNPC_LIST		3
#define PET_LIST			4
#define OBJECT_LIST_COUNT	5

#define ADD_OBJECT_LIST( a, b)	ObjectList[ a].push_back( b)

	typedef std::vector<DWORD> SERVER_ID_LIST;

	SERVER_ID_LIST ObjectList[ OBJECT_LIST_COUNT];
	int	MsgList[ OBJECT_LIST_COUNT]	=
	{
		CS_IT_CHARINFOLIST_REQ,
		CS_NC_NPCINFOLIST_REQ,
		CS_IM_MAPITEMINFOLIST_REQ,
		CS_NC_FUNCTIONALNPCINFOLIST_REQ,
		CS_NC_PETINFOLIST_REQ
	};

	for(int i = 0; i < wNumObject; ++i)
	{
		DWORD dwObjectID	=0;		
		WORD  wPosX			=0;
		WORD  wPosY			=0;
		BYTE  bObjectType	=0;
		BYTE  bHeight		=0;

		msg
			>> dwObjectID
			>> bObjectType
			>> wPosX
			>> wPosY
			>> bHeight;

		if(wPosX < 0 || wPosX > 2047 || wPosY < 0 || wPosY > 2047)
		{
			DBG_LogFile("Invalid Position in OnCS_NV_MAPOBJECTLIST_ACK %d %d %d %d %d",dwObjectID,bObjectType,wPosX,wPosY,bHeight);

			continue;
		}

		// 기존에 정보를 가지고 있는 애들이면 SKIP
		if( XiahObject::g_XiahObjectManager.FindXiahObject( MAKEOBJECTID( 0, dwObjectID,OBJTYPE_PC)) != NULL)
		{
			//DBG_Put("기존에 가지고있는애들이라 무시(total:%d)",wNumObject);
			continue;
		}

		switch(bObjectType) 
		{
			case OBJTYPE_PC:
				ADD_OBJECT_LIST( CHAR_LIST, dwObjectID);
				break;
			case OBJTYPE_NPC:
				ADD_OBJECT_LIST( NPC_LIST, dwObjectID);
				break;
			case OBJTYPE_ITEM:
				ADD_OBJECT_LIST( ITEM_LIST, dwObjectID);
				break;
			case OBJTYPE_FUNCTIONALNPC:
				ADD_OBJECT_LIST( FUNCNPC_LIST, dwObjectID);
				break;
			case OBJTYPE_PET:
				ADD_OBJECT_LIST( PET_LIST, dwObjectID);
				break;
		}
	}

	for(int i = 0; i < OBJECT_LIST_COUNT; ++i)
	{
		SERVER_ID_LIST &list = ObjectList[ i];

		while( list.size() > 0)
		{
			CMsg msg;

			msg.ID( MsgList[ i]);
			msg
				<< dwMapID;

			if( list.size() > 16)
				msg << (WORD)16;
			else
				msg << (WORD)list.size();

			for(int j = 0; j < list.size() && j < 16; ++j)
			{
				msg
					<< list.back();

				list.pop_back();
			}

			XiahNetwork::SendNetMsg( msg);
		}
	}

	return TRUE;
}



//---------------------------------------------------------------------------------------
/**
 *
 * \param &msg 
 * \return 
 */
int OnCS_NV_STARTMOVE_ACK( CMsg &msg)
{
	BYTE	bResult;
	DWORD	dwObjectID;
	WORD	wPosX;
	WORD	wPosY;
	BYTE	bHeight;
	WORD	wDesPosX;
	WORD	wDesPosY;
	BYTE	bDesHeight;
	WORD	wDirection;
	BYTE	bStatus;
	BYTE	bSpeed;
	BYTE	bFastMove;

	msg
		>> bResult
		>> dwObjectID
		>> wPosX
		>> wPosY
		>> bHeight
		>> wDesPosX
		>> wDesPosY
		>> bDesHeight
		>> wDirection
		>> bStatus
		>> bSpeed
		>> bFastMove;

	if(wPosX < 0 || wPosX > 2047 || wPosY < 0 || wPosY > 2047 || wDesPosX < 0 || wDesPosX > 2047 || wDesPosY < 0 || wDesPosY > 2047 || dwObjectID < 400000000) 
	{
		DBG_LogFile("Invalid Position in OnCS_NV_STARTMOVE_ACK %d %d %d %d %d",dwObjectID,wPosX,wPosY,wDesPosX,wDesPosY);
		return FALSE;
	}

	XiahObject::CXiahObject *pXiahObject = XiahObject::g_XiahObjectManager.FindXiahObject( MAKEOBJECTID( 0, dwObjectID, OBJTYPE_PC));

	//  it's my own.
	if(pXiahObject == g_pMainChar)
	{
		return TRUE;
	}

	if( pXiahObject == NULL)
	{
		ValidateObject( OBJTYPE_PC, dwObjectID, wPosX, wPosY);
		return TRUE;
	}
	
	if( pXiahObject == g_pMainChar)
		return TRUE;

	CXiahCharObject *pObject = reinterpret_cast<CXiahCharObject*>(pXiahObject->m_pObject);
	if(pObject == NULL) return TRUE;

	pObject->SetPosition( wPosX, wPosY);
	pObject->SetAngle( wDirection);
	pObject->Update();

	if(bFastMove == 6)
		pObject->SetAnimation( XiahAniType::eLAT_Mugong, 4, 0.7f);
	else
	{
		pObject->SetAnimation( XiahAniType::eLAT_Run, 1);

		float fScale = (float)bSpeed / 9.0f;

		if( fScale > 2.0f)
			fScale = 2.0f;

		pObject->m_CharRender.SetAnimationSpeed( fScale);
	}
	
	pObject->SetTargetMove( wDesPosX, wDesPosY, eLBP_CharNavigation, 0);

	//bSpeed = 1;

	if(pObject->m_bTradeSell)
		pObject->m_bTradeSell = false;

	return TRUE;
}

//---------------------------------------------------------------------------------------
int OnCS_NV_SYNCMOVE_ACK( CMsg &msg)
////////////////////////////////////
{
	BYTE	bResult;
	DWORD	dwObjectID;
	WORD	wPosX;
	WORD	wPosY;
	BYTE	bHeight;
	WORD	wDesPosX;
	WORD	wDesPosY;
	BYTE	bDesHeight;
	WORD	wDirection;
	BYTE	bStatus;
	BYTE	bSpeed;
	WORD	wDiffTime;

	msg
		>> bResult
		>> dwObjectID
		>> wPosX
		>> wPosY
		>> bHeight
		>> wDesPosX
		>> wDesPosY
		>> bDesHeight
		>> wDirection
		>> bStatus
		>> bSpeed
		>> wDiffTime;

	if(wPosX < 0 || wPosX > 2047 || wPosY < 0 || wPosY > 2047 || wDesPosX < 0 || wDesPosX > 2047 || wDesPosY < 0 || wDesPosY > 2047 || dwObjectID < 400000000) 
	{
		DBG_LogFile("Invalid Position in OnCS_NV_SYNCMOVE_ACK %d %d %d %d %d",dwObjectID,wPosX,wPosY,wDesPosX,wDesPosY);
		return FALSE;
	}

	XiahObject::CXiahObject *pXiahObject = XiahObject::g_XiahObjectManager.FindXiahObject( MAKEOBJECTID( 0, dwObjectID, OBJTYPE_PC));
	//  it's my own.
	if(pXiahObject == g_pMainChar)
	{
		return TRUE;
	}

	if( pXiahObject == NULL)
	{
		ValidateObject( OBJTYPE_PC, dwObjectID, wPosX, wPosY);
		return TRUE;
	}
	if( pXiahObject == g_pMainChar)
		return TRUE;

	// g_MainCharInfo.ShowHelpMessage("Sync MOVE");
	CXiahCharObject *pObject = reinterpret_cast<CXiahCharObject*>(pXiahObject->m_pObject);
	if(pObject == NULL) return TRUE;

	if( pObject->GetDistance( wPosX, wPosY) > ADJUST_SYNCMOVE_THRESOLD)
	{
		pObject->SetPosition( wPosX, wPosY);
		pObject->SetTargetMove( wDesPosX, wDesPosY, eLBP_CharNavigation, 0);
		float fSpeed = (float)bSpeed / 9.0f;
		if( fSpeed > 2.0f)
			fSpeed = 2.0f;
		pObject->m_CharRender.SetAnimationSpeed( fSpeed);

//		float fScale = pObject->GetSyncMoveScale( wPosX, wPosY);
//		pObject->m_CharRender.SetAnimationSpeed( fScale);
	}
	else
	{
		float fSpeed = (float)bSpeed / 9.0f;
		/*if( fSpeed > 2.0f)
			fSpeed = 2.0f;
		*/
		pObject->m_CharRender.SetAnimationSpeed( fSpeed);
	}

	if(pObject->m_bTradeSell)
		pObject->m_bTradeSell = false;

	return TRUE;
}

//---------------------------------------------------------------------------------------
/**
 * 이동정지
 * \param &msg 
 * \return 
 */
int OnCS_NV_ENDMOVE_ACK( CMsg &msg)
{
	BYTE	bResult		=0;
	BYTE	bHeight		=0;
	BYTE	bStatus		=0;
	WORD	wPosX		=0;
	WORD	wPosY		=0;
	DWORD	dwObjectID	=0;

	msg
		>> bResult
		>> dwObjectID
		>> wPosX
		>> wPosY
		>> bHeight
		>> bStatus;

	if(wPosX < 0 || wPosX > 2047 || wPosY < 0 || wPosY > 2047 || dwObjectID < 400000000) 
	{
		DBG_LogFile("Invalid Position in OnCS_NV_ENDMOVE_ACK %d %d %d",dwObjectID,wPosX,wPosY);
		return TRUE;
	}

	XiahObject::CXiahObject *pXiahObject = XiahObject::g_XiahObjectManager.FindXiahObject( MAKEOBJECTID( 0, dwObjectID, OBJTYPE_PC));
	//  it's my own.
	if(pXiahObject == g_pMainChar)
	{
		if(2 == bStatus)	// 채집도중 아이템 제거시
		{
			CXiahCharObject *pObject = reinterpret_cast<CXiahCharObject*>(pXiahObject->m_pObject);
			if(pObject == NULL)
				return TRUE;
			pObject->SetAnimation( XiahAniType::eLAT_Stand, 0);
		}

		return TRUE;
	}

	if( pXiahObject == NULL)
	{
		ValidateObject( OBJTYPE_PC, dwObjectID, wPosX, wPosY);
		return TRUE;
	}

	CXiahCharObject *pObject = reinterpret_cast<CXiahCharObject*>(pXiahObject->m_pObject);
	if(pObject == NULL) return TRUE;

	if( pObject->GetDistance( wPosX, wPosY) > ADJUST_SYNCMOVE_THRESOLD)
	{
		pObject->SetPosition( wPosX, wPosY);
		pObject->Update();
	}

	pObject->SetAnimation( XiahAniType::eLAT_Stand, 0);

	return TRUE;
}




/**
 *
 * \param &msg 
 * \return 
 */
int OnCS_NV_MAPENTER_ACK( CMsg &msg)
{
	BYTE	bResult	=0;

	msg
		>> bResult;

	if(bResult != 0)
		return FALSE;

	DWORD	dwMapID			=0;
	DWORD	dwObjectID		=0;
	DWORD	dwMunpaBattleID	=0;	
	WORD	wPosX			=0;
	WORD	wPosY			=0;
	WORD	wDirection		=0;
	BYTE	bObjectType		=0;
	BYTE	bHeight			=0;	
	BYTE	bState			=0;
	BYTE	bSpeed			=0;	
	BYTE	bBattleType		=0;
	BYTE	bBattleTeam		=0;
	BYTE	bHpPortionCnt	=0;
	BYTE	bIpPortionCnt	=0;

	msg
		>> dwMapID
		>> dwObjectID
		>> bObjectType
		>> wPosX
		>> wPosY
		>> bHeight
		>> wDirection
		>> bState
		>> bSpeed
		>> dwMunpaBattleID
		>> bBattleType
		>> bBattleTeam
		>> bHpPortionCnt
		>> bIpPortionCnt;

	if(wPosX < 0 || wPosX > 2047 || wPosY < 0 || wPosY > 2047) 
	{
		DBG_LogFile("Invalid Position in OnCS_NV_MAPENTER_ACK");

		return TRUE;
	}


	if( g_MainCharInfo.m_dwObjectID == dwObjectID)
	{
		g_MainCharInfo.m_fPosX = wPosX;
		g_MainCharInfo.m_fPosY = wPosY;

		g_MainCharInfo.m_bHeight = bHeight;
		g_MainCharInfo.m_wDirection = wDirection;
		g_MainCharInfo.m_bState = bState;
		g_MainCharInfo.m_bWalkSpeed = 11;

		//YS_0728 : BUGFIX
		typedef std::list<XiahObject::CXiahObject*> XIAHOBJECTLIST;
		XIAHOBJECTLIST DeleteXiahPool;

		XiahObject::CXiahObjectManager::iterator it;

		for( it = XiahObject::g_XiahObjectManager.begin(); it != XiahObject::g_XiahObjectManager.end(); ++it)
		{
			XiahObject::CXiahObject *pXiahObject = it->second;

			if ( NULL == pXiahObject || NULL == pXiahObject->m_pObject )
				continue;

			CXiahCharObject* pCharObject = reinterpret_cast<CXiahCharObject*>(pXiahObject->m_pObject);

			//YS_0812 : BUGFIX
			if ( NULL != pCharObject && pCharObject->m_bPoolClass && false == pCharObject->m_bPetPool )
			{
				DeleteXiahPool.push_back( pXiahObject );				
			}
		}

		XIAHOBJECTLIST::iterator xdit;
		for(xdit=DeleteXiahPool.begin(); xdit!=DeleteXiahPool.end(); xdit++)
		{
			XiahObject::g_XiahObjectManager.ReleaseXiahObject( *xdit );
		}

		//YS_0812 : BUGFIX
        XiahObject::g_XiahCharPool.AllClear();
		XiahObject::g_XiahNpcPool.AllClear();
		XiahObject::g_XiahPetPool.AllClear();		
		
		SendCS_IT_IMREADY_REQ(dwObjectID, dwMapID);
		
		if( g_GameWork.m_GameStep_0 == GAMESTEP_INTRO)	// 맵에 처음 들어갈때
		{
			g_pIntro->ClearIntro();

			// 이제 MainCharacter를 만들어 준다
			CreateMainChar();

			// 게임이 시작되면 카메라 세팅.
			g_XiahCamera.m_fXAngle		= -0.69813170f; // -0.6981317f; // -_PI / 4.5f;
			g_XiahCamera.m_fDistance	= 45;
			g_XiahCamera.m_fDestXAngle	= g_XiahCamera.m_fXAngle;
			g_XiahCamera.m_fDestDistance = g_XiahCamera.m_fDistance;
			g_XiahCamera.m_bNeedUpdate	= TRUE;
			g_XiahCamera.Update();

			//
			SET_GAMESTEP( GAMESTEP_GAME);

			ProcessXiahBGM( TRUE);

			Fade::StartFade( 0, 0, NULL);
			SendCS_IF_PETLIST_REQ( g_pMainChar->m_dwServerID);
			g_MainCharInfo.m_IsStarted = TRUE;
		}
		else
		{
			// 이건 포탈을 통해 이동했을 경우다
			CXiahCharObject* pChar = (CXiahCharObject*)g_pMainChar->m_pObject;
			pChar->SetPosition( wPosX, wPosY);
			pChar->SetAnimation(XiahAniType::eLAT_Stand, -1);

			g_XiahCamera.m_bFollowMainChar = TRUE;
			g_XiahCamera.m_bNeedUpdate = TRUE;

			// 게임이 시작되면 카메라 세팅.
			g_XiahCamera.m_fXAngle		= -0.69813170f; // -0.6981317f; // -_PI / 4.5f;
			g_XiahCamera.m_fDistance	= 45;
			g_XiahCamera.m_fDestXAngle	= g_XiahCamera.m_fXAngle;
			g_XiahCamera.m_fDestDistance = g_XiahCamera.m_fDistance;
			g_XiahCamera.Update();

			XiahMap::g_XiahMap.Update();

			Fade::StartFade( 0, 0, NULL);
			g_PetList.JumpToPlayer();

			CloseAllWindow();
		}
	}
	else
	{
		SendCS_IT_CHARINFO_REQ( dwObjectID);
	}

	DBG_Put(_T("MapEnterACK 수신 : %d %d"), g_MainCharInfo.m_dwObjectID == dwObjectID, dwObjectID);

	g_MainCharInfo.RefreshSituation();
	
	g_MainCharInfo.CloseFrame(LOADING_IMAGE3); //HO_0702_07 등급표시 : 등급표시와 함게 스타트로딩과 게임로딩 부분이 동일 이미지로 처리된다.
	
	//등급표시 적용전 코드 나중에 지워 버리자 ..; 이제는 한장만 사용하니까...
	//g_MainCharInfo.CloseFrame(LOADING_IMAGE);
	//g_MainCharInfo.CloseFrame(LOADING_IMAGE2);
	

	// 2004.07.20 이벤트용 로딩화면
	//g_MainCharInfo.CloseFrame( EVENT_LOADING_1 );
	//g_MainCharInfo.CloseFrame( EVENT_LOADING_2 );

	return TRUE;
}


//---------------------------------------------------------------------------------------
/**
 *
 * \param &msg 
 * \return 
 */
int OnCS_NV_MAPLEAVE_ACK(CMsg &msg)
{

	BYTE	bResult;
	DWORD	dwObjectID;
	BYTE	bObjectType;
	DWORD	dwMapID;
	BYTE	bType;

	msg
		>> bResult
		>> dwObjectID
		>> bObjectType
		>> dwMapID
		>> bType;


	if(g_pMainChar == NULL || dwObjectID == g_pMainChar->m_dwServerID)
		return TRUE;

	// Use bObjectType from packet to support both PC and NPC object cleanup
	XiahObject::CXiahObject *pXiahObject = XiahObject::g_XiahObjectManager.FindXiahObject( MAKEOBJECTID( 0, dwObjectID, bObjectType) );
	if( !pXiahObject || !pXiahObject->m_pObject ) return FALSE;

    CXiahCharObject *pCharObject = reinterpret_cast<CXiahCharObject*>(pXiahObject->m_pObject);
	if(pCharObject == NULL) return TRUE;

	switch( bType )
	{
	case 0:		// 게임에서 완전히 나갔다.
		break;
	case 1:		// 이형부
	case 2:		// 동신주.
		{
			_EFFECTPACKAGE* pEffectPackage = g_EffectManager.EnqAppearEffectImmediately( eTeleport );

			if( pEffectPackage && pEffectPackage->pEffectRender && pEffectPackage->pEffectRender->pPackagePair )
                pEffectPackage->pEffectRender->pPackagePair->WorldMatrix = (MATRIX)pCharObject->m_ObjectTM;

			g_MainCharInfo.PlayInterfaceSoundWithVol(USE_PORTAL_SOUND ,pCharObject->m_Position);
		}
		break;
	};// switch

	// Clean up effects before releasing NPC objects to prevent dangling pointers
	if(bObjectType == OBJTYPE_NPC)
	{
		pCharObject->m_CharRender.ClearMeshEffect();
		pCharObject->ClearMugongEffect();
		pCharObject->m_KeepUpMugongList.clear();
	}

	XiahObject::g_XiahObjectManager.ReleaseXiahObject( MAKEOBJECTID( 0, dwObjectID, bObjectType));

	return TRUE;
}
/*
 
// 다른 맵으로 이동하자

*/
//---------------------------------------------------------------------------------------
// 맵 이동을 성공했으니까 맵 로딩을 다시하고 주변 오브젝트 정보로 다시 받아서 리스트를 만들어야 겠다.
int OnCS_NV_MAPMOVE_ACK(CMsg &msg)
{
	BYTE	bResult		=0;
	BYTE	bLinkMapType=0;	
	WORD	wStartPosX	=0;
	WORD	wStartPosY	=0;
	DWORD	dwMapID		=0;
		
	msg
		>> bResult
		>> dwMapID
		>> wStartPosX
		>> wStartPosY
		>> bLinkMapType;


	if(wStartPosX < 0 || wStartPosX > 2047 || wStartPosY < 0 || wStartPosY > 2047) 
	{
		DBG_LogFile("Invalid Position in OnCS_NV_MAPMOVE_ACK");
		return TRUE;
	}

	switch(bResult)
	{
	case 0:
		{
			XiahMap::g_XiahMap.m_MapInfo.m_dwMapID = dwMapID;

			// 죽으면 8초 후에 MapInfo를 날린다.
			if( g_MainCharInfo.m_bMainCharDie )
			{
				Fade::StartFade( 0, TRUE, FadeTrigger_MainCharMapEnterAfterDie, 8000 ); // 2000
			}
			// 아이템을 사용하여 이동할때는 2.5초 후에 MapInfo를 날린다.
			else if( g_MainCharInfo.m_bMainCharMapMoveItemUse )
			{
				Fade::StartFade( 0, TRUE, FadeTrigger_MainCharMapEnterAfterDie, 3000 ); // 2000
			}
			else
			{
				SendCS_IT_MAPINFO_REQ(dwMapID);

				// 포탈 move와 마찬가지로 기존의 active한 map object들을 홀랑 다 지워 준다
				CXiahGame_Main *pGameMainStep = (CXiahGame_Main*)g_GameStep[ GAMESTEP_GAME];
				//			pGameMainStep->m_VisibleXiahObjectList.clear();
				pGameMainStep->m_VisibleXiahObjectListNoAlpha.clear();
				pGameMainStep->m_VisibleXiahObjectListAlphaTest.clear();

				pGameMainStep->m_VisibleXiahCharObjectNameList.clear();
				pGameMainStep->m_VisibleXiahCharObjectList.clear();

				XiahObject::g_XiahObjectManager.ReleaseAllObjectExceptMainChar();

				// 현재 비, 눈 상태를 초기화.
				g_RainSnow.AllStop();
			}
		}
		break;
	case 5:
		{
			Fade::StartFade( 0, 0, NULL);
			//등급표시 적용전 코드 나중에 지워 버리자 ..;
			//g_MainCharInfo.CloseFrame(LOADING_IMAGE);
			//g_MainCharInfo.CloseFrame(LOADING_IMAGE2);
			g_MainCharInfo.CloseFrame(LOADING_IMAGE3);
			g_MainCharInfo.ShowHelpMessage(IDS_ENTER_MUNPAMAP);

			// 2004.07.20 이벤트용 로딩화면
			//g_MainCharInfo.CloseFrame( EVENT_LOADING_1 );
			//g_MainCharInfo.CloseFrame( EVENT_LOADING_2 );
		}
		break;
	case 10:	// CG_2005/01/20 : 던전출입안됨
		{
			Fade::StartFade( 0, 0, NULL);
			//등급표시 적용전 코드 나중에 지워 버리자 ..;
			//g_MainCharInfo.CloseFrame(LOADING_IMAGE);
			//g_MainCharInfo.CloseFrame(LOADING_IMAGE2);
			g_MainCharInfo.CloseFrame(LOADING_IMAGE3);

			TCHAR strTemp[ 256 ] = { 0, };
			_stprintf( strTemp, IDS_DUNJEON_NOTENTER );
			g_MainCharInfo.SpecialChatMessage( strTemp, 0 );
		}
		break;
	case 11:						// 대련장 출입 불가
		{
			Fade::StartFade( 0, 0, NULL);
			//등급표시 적용전 코드 나중에 지워 버리자 ..;
			//g_MainCharInfo.CloseFrame(LOADING_IMAGE);
			//g_MainCharInfo.CloseFrame(LOADING_IMAGE2);
			g_MainCharInfo.CloseFrame(LOADING_IMAGE3);
			
			TCHAR strTemp[ 256 ] = { 0, };
			_stprintf( strTemp, IDS_MUNPAMAP_NOTENTER );
			g_MainCharInfo.SpecialChatMessage( strTemp, 0 );
		}
		break;
	case ERR_CANNOTENTER_MABLOOD:	// 신규추가(갑자제한으로 이동불가)
		{
			Fade::StartFade( 0, 0, NULL);

			//등급표시 적용전 코드 나중에 지워 버리자 ..;
			//g_MainCharInfo.CloseFrame(LOADING_IMAGE);
			//g_MainCharInfo.CloseFrame(LOADING_IMAGE2);
			g_MainCharInfo.CloseFrame(LOADING_IMAGE3);

			g_MainCharInfo.ShowHelpMessage(IDS_CANNOTENTER_MABLOOD);

			g_MainCharInfo.m_bMainCharMapMoveItemUse = FALSE;
		}
		break;
	case ERR_CANNOTMOVE_MABLOOD:	// 신규추가(이 지역에서 사용할 수 없습니다.)	
		{
			Fade::StartFade( 0, 0, NULL);

			//등급표시 적용전 코드 나중에 지워 버리자 ..;
			//g_MainCharInfo.CloseFrame(LOADING_IMAGE);
			//g_MainCharInfo.CloseFrame(LOADING_IMAGE2);
			g_MainCharInfo.CloseFrame(LOADING_IMAGE3);

			g_MainCharInfo.ShowHelpMessage(IDS_CANNOTMOVE_MABLOOD);

			g_MainCharInfo.m_bMainCharMapMoveItemUse = FALSE;
		}
		break;
	case ERR_CANNOTENTER_NOAUTHORITY:
		{
			Fade::StartFade( 0, 0, NULL);

			//등급표시 적용전 코드 나중에 지워 버리자 ..;
			//g_MainCharInfo.CloseFrame(LOADING_IMAGE);
			//g_MainCharInfo.CloseFrame(LOADING_IMAGE2);
			g_MainCharInfo.CloseFrame(LOADING_IMAGE3);

			g_MainCharInfo.ShowHelpMessage(IDS_MOVE_NOTINENTRY);

			g_MainCharInfo.m_bMainCharMapMoveItemUse = FALSE;
		}
		break;
	case ERR_CANNOTENTER_WARREADY:
		{
			Fade::StartFade( 0, 0, NULL);

			//등급표시 적용전 코드 나중에 지워 버리자 ..;
			//g_MainCharInfo.CloseFrame(LOADING_IMAGE);
			//g_MainCharInfo.CloseFrame(LOADING_IMAGE2);
			g_MainCharInfo.CloseFrame(LOADING_IMAGE3);

			g_MainCharInfo.ShowHelpMessage(IDS_MOVE_WARPERIOD);

			g_MainCharInfo.m_bMainCharMapMoveItemUse = FALSE;
		}
		break;
	//HO_0227_07 광명전,천황전 이벤트 추가
	case ERR_CANNOTENTER_DEVILREADY://천황전(마혈천황의 방) 동신주 사용
		{		
			g_MainCharInfo.SpecialChatMessage(IDS_CANNOTENTER_DEVILREADY, 0); //이벤트시작 15분전부터 끝날대까지
		}
		break;
        
	default:
		{
			//DBG_Put(_T("맵 이동 실패??"));
			//Fade::StartFade( 0, TRUE, NULL, 1500 );
			Fade::StartFade( 0, 0, NULL);

			//등급표시 적용전 코드 나중에 지워 버리자 ..;
			//g_MainCharInfo.CloseFrame(LOADING_IMAGE);
			//g_MainCharInfo.CloseFrame(LOADING_IMAGE2);
			g_MainCharInfo.CloseFrame(LOADING_IMAGE3);

			g_MainCharInfo.ShowHelpMessage(IDS_MAP_NO);

			// 2004.07.20 이벤트용 로딩화면
			//g_MainCharInfo.CloseFrame( EVENT_LOADING_1 );
			//g_MainCharInfo.CloseFrame( EVENT_LOADING_2 );

			g_MainCharInfo.m_bMainCharMapMoveItemUse = false;

		}
		break;
	}

	return TRUE;
}

int OnCS_NV_SETPOSITION_ACK(CMsg &msg)
{
	BYTE	bResult;
	DWORD	dwObjectID;
	WORD	wPosX;
	WORD	wPosY;
	BYTE	bHeight;
	BYTE	bStatus;

	msg
		>> bResult
		>> dwObjectID
		>> wPosX
		>> wPosY
		>> bHeight
		>> bStatus;

	if(wPosX < 0 || wPosX > 2047 || wPosY < 0 || wPosY > 2047 || dwObjectID < 400000000) 
	{
		DBG_LogFile("Invalid Position in OnCS_NV_SETPOSITION_ACK");
		return TRUE;
	}

	XiahObject::CXiahObject *pXiahObject = XiahObject::g_XiahObjectManager.FindXiahObject( MAKEOBJECTID( 0, dwObjectID, OBJTYPE_PC));
	if( pXiahObject == NULL)
	{
		ValidateObject( OBJTYPE_PC, dwObjectID, wPosX, wPosY);
		return TRUE;
	}

	if( pXiahObject == g_pMainChar)
	{
		CXiahCharObject *pObject = reinterpret_cast<CXiahCharObject*>(pXiahObject->m_pObject);
		if(pObject == NULL) return TRUE;

		if( pObject->GetDistance( wPosX, wPosY) > ADJUST_SYNCMOVE_THRESOLD)
		{
			pObject->SetPosition( wPosX, wPosY);
			pObject->Update();
			g_XiahCamera.Update();
			XiahMap::g_XiahMap.Update();
		}

		pObject->SetAnimation( XiahAniType::eLAT_Stand, 0);
	}

	return TRUE;
}

/**
 * NPC 포탈 이동
 * \param &msg 
 * \return 
*/
int OnCS_NV_QUICKMOVE_ACK(CMsg &msg)
{
	BYTE bResult =0;

	msg
		>> bResult;

	switch(bResult)
	{
	case ERR_QUICKMOVE_SUCCESS:		// 성공
		{
			CloseAllWindow();

			//g_MainCharInfo.OpenFrame(LOADING_IMAGE);
			g_MainCharInfo.OpenFrame(LOADING_IMAGE3); //HO_0702_07 등급표시 : 등급표시와 함게 스타트로딩과 게임로딩 부분이 동일 이미지로 처리된다.
		}
		break;
	case ERR_QUICKMOVE_NEEDMONEY:	// 돈 부족
		{
			g_MainCharInfo.ShowHelpMessage(IDS_SHORT_MONEY, TEXTEFFECT_COLOR_WARNING);
		}		
		break;
	case ERR_QUICKMOVE_BADAREA:		// 이동 가능한 지역 아님
		{
			g_MainCharInfo.ShowHelpMessage(IDS_MAP_NO, TEXTEFFECT_COLOR_WARNING);			
		}
		break;
	case ERR_QUICKMOVE_INTERNAL:	// 내부 댄轎
		{
			g_MainCharInfo.ShowHelpMessage(IDS_INTERNAL_ERROR, TEXTEFFECT_COLOR_WARNING);
		}
		break;
	default:
		break;
	}

	return 0;
}

/**
 * 문파대전 NPC 포탈 이동
 * \param &msg 
 * \return 
 */
int OnCS_NV_PRIVATEPORTAL_ACK(CMsg &msg)
{
	BYTE bResult =0;

	msg
		>> bResult;

	switch(bResult)
	{
	case ERR_MOVE_SUCCESS:			// 성공
		{
			CloseAllWindow();

			//g_MainCharInfo.OpenFrame(LOADING_IMAGE);
			g_MainCharInfo.OpenFrame(LOADING_IMAGE3); //HO_0702_07 등급표시 : 등급표시와 함게 스타트로딩과 게임로딩 부분이 동일 이미지로 처리된다.
		}
		break;
	case ERR_MOVE_WARPERIOD:		// 전쟁시작 5분전
		{
			g_MainCharInfo.ShowHelpMessage(IDS_MOVE_WARPERIOD, TEXTEFFECT_COLOR_WARNING);
		}		
		break;
	case ERR_MOVE_NOTLORDMUNPA:		// 우승문파원아님
		{
			g_MainCharInfo.ShowHelpMessage(IDS_MOVE_NOTLORDMUNPA, TEXTEFFECT_COLOR_WARNING);			
		}
		break;
	case ERR_MOVE_NOTINENTRY:		// 관계 없는 사람 이동 불가
		{
			g_MainCharInfo.ShowHelpMessage(IDS_MOVE_NOTINENTRY, TEXTEFFECT_COLOR_WARNING);

		}
		break;
	case ERR_MOVE_NOTENOUGHLEVEL:	// 갑자 미만
		{
			g_MainCharInfo.ShowHelpMessage(IDS_MOVE_NOTENOUGHLEVEL, TEXTEFFECT_COLOR_WARNING);
		}
		break;
	case ERR_MOVE_NOTWARCHANNELID:	//2번채널이 아닌 채널에서 사용
		{
			g_MainCharInfo.ShowHelpMessage(IDS_MOVE_NOTWARCHANNELID, TEXTEFFECT_COLOR_WARNING);
		}
		break;
	
	case ERR_MOVE_NOTWAR:	//HO_0906_07 문파대전 관리인 포탈기능 추가 : 마혈진 지역
		{
			g_MainCharInfo.ShowHelpMessage(IDS_MOVE_NOTWAR, TEXTEFFECT_COLOR_WARNING); //전쟁 중에는 마혈진 이동 포탈을 사용하실 수 없습니다.
		}
		break;

	default:
		{
			g_MainCharInfo.ShowHelpMessage(IDS_MAP_NO, TEXTEFFECT_COLOR_WARNING);
		}
		break;
	}

	return 0;
}

/**
* //HT_0122 : 기간제 프리미엄 아이템 추가
* \param &msg 
* \return 
*/
int OnCS_NV_CHARPREMIUM_ACK(CMsg &msg)
{
	BYTE bCount = 0;

	msg
		>> bCount;

	g_MainCharInfo.m_pPremiumItem->AllDeleteScrMsg();

	for(BYTE i=0; i < bCount; ++i)
	{
		WORD wRefID		= 0;
		BYTE bEndDay	= 0;
		BYTE bEndHour	= 0;
		BYTE bEndMin	= 0;

		msg
			>> wRefID
			>> bEndDay
			>> bEndHour
			>> bEndMin;

		int nResID =0;
		TCHAR szTip [128] = {0,};

		switch(wRefID)
		{
		case 1:	// 천수배
		case 2:
		case 3:
		case 4:
		case 5:
		case 6:
			{
				nResID = 1416;
				_stprintf(szTip, IDS_PREMIUM_ITEM_01, 50, bEndDay, bEndHour, bEndMin); 
			}
			break;
		case 7:	// 오행진
		case 8:
		case 9:
		case 10:
		case 11:
		case 12:
			{
				nResID = 1414;
				_stprintf(szTip, IDS_PREMIUM_ITEM_02, 50, bEndDay, bEndHour, bEndMin);
			}
			break;
		case 13:	// 영물패
		case 14:
		case 15:
			{
				nResID = 1413;
				_stprintf(szTip, IDS_PREMIUM_ITEM_03,100, bEndDay, bEndHour, bEndMin);
			}
			break;
		//HO_0608_07 프리미엄 아이템 2종 추가 
		case 16:	//천수배 중복 적용가능
			{
				nResID = 1416;
				_stprintf(szTip, IDS_PREMIUM_ITEM_01, 50, bEndDay, bEndHour, bEndMin); 
			}
			break;
		case 17:	//오행진 중복 적용가능
			{
				nResID = 1414;
				_stprintf(szTip, IDS_PREMIUM_ITEM_02, 50, bEndDay, bEndHour, bEndMin); 
			}
			break;
		//HO_0803_07 천수배 오행진 중복가능 시작
		case 18:	//천수배 중복 적용가능
			{
				nResID = 1416;
				_stprintf(szTip, IDS_PREMIUM_ITEM_01, 50, bEndDay, bEndHour, bEndMin); 
			}
			break;
		case 19:	//천수배 중복 적용가능
			{
				nResID = 1416;
				_stprintf(szTip, IDS_PREMIUM_ITEM_01, 100, bEndDay, bEndHour, bEndMin); 
			}
			break;
		case 20:	//천수배 중복 적용가능
			{
				nResID = 1416;
				_stprintf(szTip, IDS_PREMIUM_ITEM_01, 150, bEndDay, bEndHour, bEndMin); 
			}
			break;
		case 21:	//천수배 중복 적용가능
			{
				nResID = 1416;
				_stprintf(szTip, IDS_PREMIUM_ITEM_01, 400, bEndDay, bEndHour, bEndMin); 
			}
			break;
		case 26:	//오행진 중복 적용가능
			{
				nResID = 1414;
				_stprintf(szTip, IDS_PREMIUM_ITEM_02, 50, bEndDay, bEndHour, bEndMin); 
			}
			break;
		case 27:	//오행진 중복 적용가능
			{
				nResID = 1414;
				_stprintf(szTip, IDS_PREMIUM_ITEM_02, 100, bEndDay, bEndHour, bEndMin); 
			}
			break;
		case 28:	//오행진 중복 적용가능
			{
				nResID = 1414;
				_stprintf(szTip, IDS_PREMIUM_ITEM_02, 150, bEndDay, bEndHour, bEndMin); 
			}
			break;
		case 29:	//오행진 중복 적용가능
			{
				nResID = 1414;
				_stprintf(szTip, IDS_PREMIUM_ITEM_02, 400, bEndDay, bEndHour, bEndMin); 
			}
			break;
		//HO_0803_07 천수배 오행진 중복가능 끝
		default:
			continue;
			break;
		}
		g_MainCharInfo.m_pPremiumItem->SetScrImg(wRefID, nResID, 0);		
		g_MainCharInfo.m_pPremiumItem->SetScrMsg(wRefID, szTip);
		g_MainCharInfo.m_pPremiumItem->Show(wRefID);

	}

	return 0;
}

//HO_0227_07 광명전,천황전 이벤트 추가
int OnCS_NV_SECRETADVENTURE_ACK(CMsg &msg)//광명전(비밀의방) 입장
{
	BYTE bResult = 0;
	
	g_MainCharInfo.CloseFrame(WINDOW_SECRET_CHECK);

	msg
		>> bResult;

	switch(bResult)
	{
	case ERR_SECRETMOVE_SUCCESS:
		{		
			g_MainCharInfo.ShowHelpMessage(IDS_SECRETMOVE_SUCCESS, TEXTEFFECT_COLOR_GENERAL); //성공
		}
		break;

	case ERR_SECRETMOVE_PERIOD:
		{
			g_MainCharInfo.ShowHelpMessage(IDS_SECRETMOVE_PERIOD, TEXTEFFECT_COLOR_WARNING); //기간아님
		}
		break;

	case ERR_SECRETMOVE_NOTCHANNELID:
		{	
			g_MainCharInfo.SpecialChatMessage(IDS_SECRETMOVE_NOTCHANNELID, TEXTEFFECT_COLOR_WARNING); //다른채널에서 신청함
		}
		break;

	case ERR_SECRETMOVE_NOTINENTRY:
		{	
			g_MainCharInfo.ShowHelpMessage(IDS_SECRETMOVE_NOTINENTRY, TEXTEFFECT_COLOR_WARNING); //신청아니함
		}
		break;

	case ERR_SECRETMOVE_NOTENOUGHLEVEL:
		{			
	//		g_MainCharInfo.SpecialChatMessage(IDS_SECRETMOVE_NOTENOUGHLEVEL, TEXTEFFECT_COLOR_GENERAL); //레벨제한
		}
		break;

	case ERR_SECRETMOVE_INTERNAL:
		{
			g_MainCharInfo.ShowHelpMessage(IDS_SECRETMOVE_INTERNAL, TEXTEFFECT_COLOR_WARNING); //내부에러
		}
		break;
	
	default:
		break;
	}

	return 0;
}

int OnCS_NV_DEVILADVENTURE_ACK(CMsg &msg)//천황전(마혈천황의 방) 입장
{
	BYTE bResult = 0;
	
	g_MainCharInfo.CloseFrame(WINDOW_SECRET_CHECK);

	msg
		>> bResult;

	switch(bResult)
	{
	case ERR_DEVILMOVE_SUCCESS:
		{		
			g_MainCharInfo.ShowHelpMessage(IDS_DEVILMOVE_SUCCESS, TEXTEFFECT_COLOR_GENERAL); //성공
		}
		break;

	case ERR_DEVILMOVE_PERIOD:
		{		
			g_MainCharInfo.ShowHelpMessage(IDS_DEVILMOVE_PERIOD, TEXTEFFECT_COLOR_WARNING); //기간아님
		}
		break;

	case ERR_DEVILMOVE_NOTCHANNELID:
		{		
			g_MainCharInfo.SpecialChatMessage(IDS_DEVILMOVE_NOTCHANNELID, TEXTEFFECT_COLOR_WARNING); //다른채널에서 신청함
		}
		break;

	case ERR_DEVILMOVE_NOTINENTRY:
		{		
			g_MainCharInfo.ShowHelpMessage(IDS_DEVILMOVE_NOTINENTRY, TEXTEFFECT_COLOR_WARNING); //신청아니함
		}
		break;

	case ERR_DEVILMOVE_NOTENOUGHLEVEL:
		{		
	//		g_MainCharInfo.SpecialChatMessage(IDS_DEVILMOVE_NOTENOUGHLEVEL, TEXTEFFECT_COLOR_GENERAL); //레벨제한
		}
		break;

	case ERR_DEVILMOVE_INTERNAL:
		{		
			g_MainCharInfo.ShowHelpMessage(IDS_DEVILMOVE_INTERNAL, TEXTEFFECT_COLOR_WARNING); //내부에러
		}
		break;
	
	default:
		break;
	}

	return 0;
}