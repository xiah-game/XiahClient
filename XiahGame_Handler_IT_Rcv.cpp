
bool g_bFirstCharListAck = true;

/**
 * 로그인
 * \param &msg 
 * \return 
 */
int OnCS_IT_LOGIN_ACK( CMsg &msg)
{
	BYTE bResult	=0;	
	BYTE bAge		=0;
	DWORD dwKey		=0;

	msg
		>> bResult
		>> dwKey
		>> bAge;
 
	TCHAR strTemp[128] = {0,};

	if(bResult != ERR_ITLOGIN_SUCCESS)
		g_pUIManager->SetData(LOGIN_1, login1_ok, CURRENT_INDEX, -1);

	switch(bResult)
	{
	case ERR_ITLOGIN_SUCCESS:
		{
			g_AppData.m_dwKey = dwKey;
			g_AppData.m_bAge = bAge;

			CXiahGame_StepObject *pObject = g_GameStep[GAMESTEP_LOGIN];

			((CXiahGame_Login*)pObject)->SetStep(CXiahGame_Login::SERVER_SELECT);

			g_pUIManager->CloseAll();
			g_pUIManager->Show(LOGIN_2);

			XiahNetwork::DisconnectFromServer();
		}
		break;

	case ERR_ITLOGIN_LOGINFAIL:
		{
			if(g_pUIManager)
				g_pUIManager->ShowNotice(IT_FAIL_LOGIN);
			else
				MessageBox( GetForegroundWindow(), IT_FAIL_LOGIN, _T("Xiah"), MB_OK);			
		}
		break;

	case ERR_ITLOGIN_WRONGPASSWORD:
		g_pUIManager->ShowNotice(IDS_LOGIN_WRONGPASSWORD);
		break;

	case ERR_ITLOGIN_DUPLICATE:
		{
			static bool bFirstShow = false;

			if(bFirstShow)
				g_pUIManager->ShowNotice(IDS_LOGIN_DUPLICATE);
			
			bFirstShow = true;
		}
		break;

	case ERR_ITLOGIN_NEEDGAMEPOP:
		g_pUIManager->ShowNotice(IDS_LOGIN_NEEDGAMEPOP);
		break;

	case ERR_ITLOGIN_BLOCKACCOUNT:
		g_pUIManager->ShowNotice(IDS_LOGIN_BLOCKACCOUNT);
		break;

	case ERR_ITLOGIN_MAPSVRCLOSED:
		{
			_stprintf(strTemp, IDS_LOGIN_MAPSVRCLOSED, bResult);
			g_pUIManager->ShowNotice(strTemp);
		}		
		break;

	case ERR_ITLOGIN_INVALIDKEY:
		{
			_stprintf(strTemp, IDS_LOGIN_MAPSVRCLOSED, bResult);
			g_pUIManager->ShowNotice(strTemp);
		}		
		break;

	case ERR_ITLOGIN_LONGACCOUNT:
		g_pUIManager->ShowNotice(IDS_LOGIN_LONGACCOUNT);
		break;

	case ERR_ITLOGIN_WRONGVERSION:
		g_pUIManager->ShowNotice(IDS_LOGIN_WRONGVERSION);
		break;

	case ERR_ITLOGIN_WRONGPROTOCOL:
		{
			_stprintf(strTemp, IDS_LOGIN_MAPSVRCLOSED, bResult);
			g_pUIManager->ShowNotice(strTemp);
		}		
		break;

	case ERR_ITLOGIN_DBCLOSED:
		{
			_stprintf(strTemp, IDS_LOGIN_MAPSVRCLOSED, bResult);
			g_pUIManager->ShowNotice(strTemp);
		}		
		break;
	case ERR_ITLOGIN_CHANGEPW:	// 비밀번호 바꿔라
		{
			g_pUIManager->ShowNotice(IDS_PW_CHANGE_00);			
		}
		break;

	case ERR_ITLOGIN_ERRORIP:	// 인증되지 않은 IP
		{
			g_pUIManager->ShowNotice(IDS_LOGIN_APPROVED);
		}
		break;
	case ERR_ITLOGIN_BEFOREAT:	// 게임팝 인증
		{
			g_pUIManager->ShowNotice(IDS_BEFORE_ATTESTATION);			
		}
		break;

	//case ERR_ITLOGIN_ERRORIP:	// 17
	//	{
	//		g_pUIManager->ShowNotice(IDS_LOGIN_APPROVED);
	//	}
	//	break;

	
	default:
		g_pUIManager->ShowNotice(IT_FAIL_LOGIN);
		break;
	}

	return TRUE;
}

/**
 * 서버 선택
 * \param &msg 
 * \return 
 */
int OnCS_IT_LOGINCHECK_ACK(CMsg &msg)
{
	BYTE bResult	=0;

	msg
		>> bResult
		>> g_AppData.m_bAdult;

	if(bResult == ERR_ITLOGIN_SUCCESS)
	{
		SendCS_IT_CHARACTERLIST_REQ();

		g_pUIManager->Destroy(LOGIN_1);
		g_pUIManager->Destroy(LOGIN_2);		

		return true;
	}

	TCHAR strTemp[128] = {0,};

	g_pUIManager->CloseAll();

	switch(bResult)
	{
	case ERR_ITLOGIN_LOGINFAIL:
		{
			if(g_pUIManager)
				g_pUIManager->ShowNotice(IT_FAIL_LOGIN);
			else
				MessageBox( GetForegroundWindow(), IT_FAIL_LOGIN, _T("Xiah"), MB_OK);
		}
		break;
	case ERR_ITLOGIN_WRONGPASSWORD:
		g_pUIManager->ShowNotice(IDS_LOGIN_WRONGPASSWORD);
		break;
	case ERR_ITLOGIN_DUPLICATE:
		g_pUIManager->ShowNotice(IDS_LOGIN_DUPLICATE);
		break;
	case ERR_ITLOGIN_NEEDGAMEPOP:
		g_pUIManager->ShowNotice(IDS_LOGIN_NEEDGAMEPOP);
		break;
	case ERR_ITLOGIN_BLOCKACCOUNT:
		g_pUIManager->ShowNotice(IDS_LOGIN_BLOCKACCOUNT);
		break;
	case ERR_ITLOGIN_MAPSVRCLOSED:
		{
			_stprintf(strTemp, IDS_LOGIN_MAPSVRCLOSED, bResult);
			g_pUIManager->ShowNotice(strTemp);
		}		
		break;
	case ERR_ITLOGIN_INVALIDKEY:
		{
			_stprintf(strTemp, IDS_LOGIN_MAPSVRCLOSED, bResult);
			g_pUIManager->ShowNotice(strTemp);
		}		
		break;
	case ERR_ITLOGIN_LONGACCOUNT:
		g_pUIManager->ShowNotice(IDS_LOGIN_LONGACCOUNT);
		break;
	case ERR_ITLOGIN_WRONGVERSION:
		g_pUIManager->ShowNotice(IDS_LOGIN_WRONGVERSION);
		break;
	case ERR_ITLOGIN_WRONGPROTOCOL:
		{
			_stprintf(strTemp, IDS_LOGIN_MAPSVRCLOSED, bResult);
			g_pUIManager->ShowNotice(strTemp);
		}		
		break;
	case ERR_ITLOGIN_DBCLOSED:
		{
			_stprintf(strTemp, IDS_LOGIN_MAPSVRCLOSED, bResult);
			g_pUIManager->ShowNotice(strTemp);			
		}		
		break;
	case 15:
		{
			g_pUIManager->ShowNotice( IDS_DISCONNECT_SERVER_2, NOTICE_FRAME_OK, NOTICE_FRAME_UNEXPECTED_TERMINATE);
			Sleep(1000);
			return true;
		}
		break;

	default:
		g_pUIManager->ShowNotice(IT_FAIL_LOGIN);
		break;
	}

	SET_GAMESTEP(GAMESTEP_LOGIN);
	g_pUIManager->Show(LOGIN_2);

	// 꽃잎 떄문에
	((CXiahGame_Login*)g_GameStep[GAMESTEP_LOGIN])->SetStep(CXiahGame_Login::SERVER_SELECT);

	return true;
}

/**
 * 서버월드리스트
 * \param &msg 
 * \return 
*/
int OnCS_IT_WORLDLIST_ACK( CMsg &msg)
{
	BYTE bWorldID			=0;
	BYTE bCount				=0;
	sString	szWorldName;
	sString	szWorldDes;

	BYTE	bChannelID		=0;	
	BYTE	bAge			=0;
	WORD	wMaxUser		=0;	
	DWORD	dwUnitPort		=0;
	sString	szChannelName;
	sString	szChannelDes;
	sString	szUnitAddress;

	msg 
		>> bWorldID
		>> szWorldName
		>> szWorldDes
		>> bCount;

	CXiahGame_Login *pGameMainStep = (CXiahGame_Login*)g_GameStep[ GAMESTEP_LOGIN];
	if(!pGameMainStep)
		return 1;

	pGameMainStep->WorldCreate(bWorldID, szWorldName, szWorldDes);

	for( int i=0; i < bCount; ++i)
	{
		msg
			>> bChannelID
			>> szChannelName
			>> szChannelDes
			>> bAge
			>> wMaxUser
			>> szUnitAddress
			>> dwUnitPort;

		pGameMainStep->ChannelCreate(bWorldID, bChannelID, szChannelName, szChannelDes, bAge, wMaxUser, dwUnitPort, szUnitAddress);
	}

	return 0;
}

/**
 * 서버월드상태
 * \param &msg 
 * \return 
*/
int OnCS_IT_WORLDSTATE_ACK( CMsg &msg)
{
	WORD	wCount		=0;
	WORD	wUserNumber	=0;
	BYTE	bWorldID	=0;
	BYTE	bChannelID	=0;
	BYTE	bState		=0;	// 0 : 서버다운 1 : 서버온	

	CXiahGame_Login *pGameMainStep = (CXiahGame_Login*)g_GameStep[ GAMESTEP_LOGIN];
	if(!pGameMainStep)
		return 1;

	msg 
		>> wCount;

	for(int i=0; i < wCount; ++i)
	{	
		msg
			>> bWorldID
			>> bChannelID
			>> bState
			>> wUserNumber;

		pGameMainStep->ChannelState(bWorldID, bChannelID, bState, wUserNumber);
	}

	return 0;
}

/**
 * 캐릭터 리스트
 * \param &msg 
 * \return 
 */
int OnCS_IT_CHARACTERLIST_ACK( CMsg &msg)
{
	BYTE bCount	=0;

	msg 
		>> bCount;

	// 캐릭터 정보를 처음 받을때 인트로가 게임 스텝으로 바뀐다.
	//if( g_bFirstCharListAck )
	{
		SET_GAMESTEP( GAMESTEP_INTRO);

		//g_bFirstCharListAck = false;
	}

	g_pIntro->SetCharCount( bCount);

	if( bCount > CHARACTER_MAX)
	{
		g_MainCharInfo.ShowHelpMessage( IDS_CHAR_OVER);
	}

	// 이렇게 하면 지워진 캐릭터 데이터가 없어진당.
	g_pIntro->DeleteCharacterData();
	g_pIntro->CreateCharacterData();
	g_pIntro->m_bCharacterSelected = false;

	for( int i=0; i < bCount && i < CHARACTER_MAX; ++i)
	{
		msg 
			>> g_pIntro->m_CharacterList[i]->m_dwMapID
			>> g_pIntro->m_CharacterList[i]->m_dwObjectID
			>> g_pIntro->m_CharacterList[i]->m_szNickName
			>> g_pIntro->m_CharacterList[i]->m_bCharType
			>> g_pIntro->m_CharacterList[i]->m_wLevel
			>> g_pIntro->m_CharacterList[i]->m_dwHpCur
			>> g_pIntro->m_CharacterList[i]->m_dwHpMax
			>> g_pIntro->m_CharacterList[i]->m_wIpCur
			>> g_pIntro->m_CharacterList[i]->m_wIpMax
			>> g_pIntro->m_CharacterList[i]->m_wVit
			>> g_pIntro->m_CharacterList[i]->m_wStr
			>> g_pIntro->m_CharacterList[i]->m_wSus
			>> g_pIntro->m_CharacterList[i]->m_wDex
			>> g_pIntro->m_CharacterList[i]->m_dwBirthDate
			>> g_pIntro->m_CharacterList[i]->m_szMunpaName
			>> g_pIntro->m_CharacterList[i]->m_szOrderName
			>> g_pIntro->m_CharacterList[i]->wEquipVisualID[0]	//sackpos:36
			>> g_pIntro->m_CharacterList[i]->m_bRarity[0]
			>> g_pIntro->m_CharacterList[i]->m_bStxType[0]
			>> g_pIntro->m_CharacterList[i]->wEquipVisualID[1] 	//sackpos:37
			>> g_pIntro->m_CharacterList[i]->m_bRarity[1]
			>> g_pIntro->m_CharacterList[i]->m_bStxType[1]
			>> g_pIntro->m_CharacterList[i]->wEquipVisualID[2] 	//sackpos:38
			>> g_pIntro->m_CharacterList[i]->m_bRarity[2]
			>> g_pIntro->m_CharacterList[i]->m_bStxType[2]
			>> g_pIntro->m_CharacterList[i]->wEquipVisualID[3] 	//sackpos:39
			>> g_pIntro->m_CharacterList[i]->m_bRarity[3]
			>> g_pIntro->m_CharacterList[i]->m_bStxType[3]
			>> g_pIntro->m_CharacterList[i]->wEquipVisualID[4] 	//sackpos:40
			>> g_pIntro->m_CharacterList[i]->m_bRarity[4]
			>> g_pIntro->m_CharacterList[i]->m_bStxType[4]
			>> g_pIntro->m_CharacterList[i]->wEquipVisualID[5] 	//sackpos:41
			>> g_pIntro->m_CharacterList[i]->m_bRarity[5]
			>> g_pIntro->m_CharacterList[i]->m_bStxType[5]
			>> g_pIntro->m_CharacterList[i]->wEquipVisualID[6] 	//sackpos:42
			>> g_pIntro->m_CharacterList[i]->m_bRarity[6]
			>> g_pIntro->m_CharacterList[i]->m_bStxType[6]
			>> g_pIntro->m_CharacterList[i]->wEquipVisualID[7] 	//sackpos:43
			>> g_pIntro->m_CharacterList[i]->m_bRarity[7]
			>> g_pIntro->m_CharacterList[i]->m_bStxType[7]
			>> g_pIntro->m_CharacterList[i]->wEquipVisualID[8] 	//sackpos:44
			>> g_pIntro->m_CharacterList[i]->m_bRarity[8]
			>> g_pIntro->m_CharacterList[i]->m_bStxType[8]
			>> g_pIntro->m_CharacterList[i]->szMapName
			// 옥션
			>> g_pIntro->m_CharacterList[i]->m_bAuction
			// 각성자
			>> g_pIntro->m_CharacterList[i]->m_bRebirth;

	}

	// 일단 무조건 캐릭터 선택창으로 간다.
//	if( bCount)
		g_pIntro->InitIntro( INTROMENU_SELECTCHAR);
//	else
//		g_pIntro->InitIntro( INTROMENU_CREATECHAR);

	return TRUE;
}

//////////////////////////////////
int OnCS_IT_NOTICE_ACK( CMsg &msg)
//////////////////////////////////
{
	return TRUE;
}

//////////////////////////////////////////
int OnCS_IT_NEWCHARACTER_ACK( CMsg &msg)
//////////////////////////////////////////
{
	BYTE	bResult;
	DWORD	dwObjectID;	

	msg 
		>> bResult
		>> dwObjectID;

	switch( bResult)
	{
	case ERR_ITNEWCHARACTER_SUCCESS:
		break;
//	case ERR_ITNEWCHARACTER_NOPARENTS:
//		g_MainCharInfo.ShowHelpMessage( IDS_MAKE_CHAR_FAIL);
//		return 0;
//		break;
	case ERR_ITNEWCHARACTER_DUPLICATE:
		g_MainCharInfo.ShowHelpMessage( IDS_EXIST_ID,TEXTEFFECT_COLOR_WARNING);

		g_pUIManager->Show(INTRO_ACCOUNT, account_button);
		g_pUIManager->Show(INTRO_ACCOUNT, account_name_edit);
		g_pUIManager->SetFocus(INTRO_ACCOUNT, account_name_edit);
		return 0;
		break;
	case ERR_ITNEWCHARACTER_INVALIDPARAM:
	case ERR_ITNEWCHARACTER_OBJECTID:
		{
			TCHAR temp[50];
			_stprintf( temp, IDS_ERROR, bResult);
			g_MainCharInfo.ShowHelpMessage( temp,TEXTEFFECT_COLOR_WARNING);
			return 0;
		}
		break;
	case ERR_ITNEWCHARACTER_INTERNAL:
		g_MainCharInfo.ShowHelpMessage( IDS_LONG_NAME,TEXTEFFECT_COLOR_WARNING);

		g_pUIManager->Show(INTRO_ACCOUNT, account_button);
		g_pUIManager->Show(INTRO_ACCOUNT, account_name_edit);
		g_pUIManager->SetFocus(INTRO_ACCOUNT, account_name_edit);
		return 0;
	case 9:
		g_MainCharInfo.ShowHelpMessage( IDS_NOMORE_CHAR,TEXTEFFECT_COLOR_WARNING);

		g_pUIManager->Show(INTRO_ACCOUNT, account_button);
		g_pUIManager->Show(INTRO_ACCOUNT, account_name_edit);
		g_pUIManager->SetFocus(INTRO_ACCOUNT, account_name_edit);
		return 0;
		break;
	case ERR_ITNEWCHARACTER_BADNAME:
		g_MainCharInfo.ShowHelpMessage( IT_WARNNIG1,TEXTEFFECT_COLOR_WARNING);

		g_pUIManager->Show(INTRO_ACCOUNT, account_button);
		g_pUIManager->Show(INTRO_ACCOUNT, account_name_edit);
		g_pUIManager->SetFocus(INTRO_ACCOUNT, account_name_edit);
		return 0;
		break;
	}
/*
	if( bResult != 1) 
	{
		g_MainCharInfo.ShowHelpMessage( IDS_MAKE_CHAR_FAIL);
		return FALSE;
	}
	else if( bResult == 1)
*/
	/*
	BYTE byCharCount = g_pIntro->GetCharCount();
	g_pIntro->SetCharCount( byCharCount + 1);
	g_pIntro->m_CharacterList[ byCharCount]->m_dwObjectID = dwObjectID;
*/
	//g_MainCharInfo.ShowHelpMessage( IDS_CHAR_MADE);

	if( bResult == 0)
		SendCS_IT_CHARACTERLIST_REQ();

	g_pUIManager->Show(INTRO_ACCOUNT, account_button);
	g_pUIManager->Show(INTRO_ACCOUNT, account_name_edit);

	
//	g_pIntro->GoToMenuSelectChar();
	
	

	return TRUE;
}

////////////////////////////////////////
int OnCS_IT_DELCHARACTER_ACK( CMsg &msg)
////////////////////////////////////////
{
	BYTE bResult;

	msg
		>> bResult;

	if( bResult != 0)
	{
		TCHAR temp[50];
		_stprintf( temp, IDS_ERROR, bResult);
		g_MainCharInfo.ShowHelpMessage(temp);
		return FALSE;
	}

	SendCS_IT_CHARACTERLIST_REQ();

	return TRUE;
}


/**
 * 캐릭터 상태 정보
 * \param &msg 
 * \return 
 */
int OnCS_IT_CHARSTATUSINFO_ACK(CMsg &msg)
{
	msg
		>> g_MainCharInfo.m_wLevel
		>> g_MainCharInfo.m_wStr
		>> g_MainCharInfo.m_wSus
		>> g_MainCharInfo.m_wDex
		>> g_MainCharInfo.m_wVit
		>> g_MainCharInfo.m_bIncrStr
		>> g_MainCharInfo.m_bIncrSus
		>> g_MainCharInfo.m_bIncrDex
		>> g_MainCharInfo.m_bIncrVit
		>> g_MainCharInfo.m_wIpMax
		>> g_MainCharInfo.m_wIpCur
		>> g_MainCharInfo.m_dwHpMax
		>> g_MainCharInfo.m_dwHpCur
		>> g_MainCharInfo.m_i64Exp
		>> g_MainCharInfo.m_i64LevelExp
		>> g_MainCharInfo.m_i64NextLevelUpExp
		>> g_MainCharInfo.m_i64TpExp
		>> g_MainCharInfo.m_i64NextTpUpExp
		>> g_MainCharInfo.m_dwTotalSp
		>> g_MainCharInfo.m_wRemainSp
		>> g_MainCharInfo.m_dwTotalTp
		>> g_MainCharInfo.m_wRemainTp
		>> g_MainCharInfo.m_wBaseAtkPwr
		>> g_MainCharInfo.m_dwTotalAtkPower
		>> g_MainCharInfo.m_wBaseDefPwr
		>> g_MainCharInfo.m_dwTotalDefPower
		>> g_MainCharInfo.m_wBaseAttackRating
		>> g_MainCharInfo.m_dwTotalAttackRating
		>> g_MainCharInfo.m_wAttackRange
		>> g_MainCharInfo.m_bWalkSpeed
		>> g_MainCharInfo.m_bRunSpeed
		>> g_MainCharInfo.m_bPlusSpeed
		>> g_MainCharInfo.m_bJumpLevel
		>> g_MainCharInfo.m_dwPkCnt
		>> g_MainCharInfo.m_dwMoney
		>> g_MainCharInfo.m_wCritical
		>> g_MainCharInfo.m_bAttackSpeed
		>> g_MainCharInfo.m_bState
		>> g_MainCharInfo.m_dwFame
		// 오행		
		>> g_MainCharInfo.m_wFiveElmPoint
		>> g_MainCharInfo.m_dwFiveElmPower
		>> g_MainCharInfo.m_dwFiveElmPowerMax
		>> g_MainCharInfo.m_dwFiveElmGauge
		>> g_MainCharInfo.m_wFiveElmExp[0]
		>> g_MainCharInfo.m_wFiveElmExp[1]
		>> g_MainCharInfo.m_wFiveElmExp[2]
		>> g_MainCharInfo.m_wFiveElmExp[3]
		>> g_MainCharInfo.m_wFiveElmExp[4]
		>> g_MainCharInfo.m_bRebirth;

	g_MainCharInfo.m_bObjectType = 1;

	g_pIntro->Init_MainFrame();
	g_pIntro->Init_SystemButton();
	g_pIntro->Init_WindowCharacter();
	g_pIntro->Init_WindowOutSide();
	g_pIntro->Init_WindowInSide();
	g_pIntro->Init_WindowSkill();			// 각성
	g_pIntro->Init_WindowFiveElement();		// 오행
	
	g_MainCharInfo.RefreshMainFrame();
	g_MainCharInfo.RefreshChracterInfo();
	g_MainCharInfo.RefreshFiveElement();	// 오행

	if( g_MainCharInfo.m_pQuest)
		g_MainCharInfo.m_pQuest->RefreshQuestContent();

	if(g_pMainChar == NULL)
		return false;

	CXiahCharObject *pObject = (CXiahCharObject*)g_pMainChar->m_pObject;

	if(pObject == NULL)
		return false;
	else
		pObject->RefreshFameColor(g_MainCharInfo.m_dwFame);

	DBG_Put(_T("캐릭터 상태 정보수신"));

	return TRUE;
}


/**
 * 맵정보
 * \param &msg 
 * \return 
*/
int OnCS_IT_MAPINFO_ACK( CMsg &msg)
{
	BYTE	bResult	=0;

	msg
		>> bResult;

	if(bResult != 0)
	{
		return FALSE;
	}

	DWORD	dwMapID		=0;
	sString	szMapName;
	WORD	wWidth		=0;
	WORD	wHeight		=0;
	BYTE	bType		=0;
	BYTE	bNumLinkMap	=0;

	msg
		>> dwMapID
		>> szMapName
		>> wWidth
		>> wHeight
		>> bType
		>> bNumLinkMap;

	XiahMap::g_XiahMap.CreateMap( dwMapID, szMapName, bType, wWidth, wHeight);
	XiahMap::g_XiahMap.Update();
	Minimap::UpdateMinimap();		// Minimap용 텍스쳐 로딩

	// 문파전은 기암괴석으로 대치!
	if(dwMapID == 10)
	{
		g_MainCharInfo.m_bIsPvPMap = TRUE;
		//g_SkyBox.ChangeSkyMap(1);	// Skybox 텍스처를 바꾼다.
	}
	else
	{
		//g_SkyBox.ChangeSkyMap(dwMapID);	// Skybox 텍스처를 바꾼다.
		g_MainCharInfo.m_bIsPvPMap = FALSE;
	}


	XiahMap::g_XiahMap.m_PortalInfoList.clear();

	// 현재 맵의 Portal NPC를 제거
	CXiahGame_Main *pGameMainStep = (CXiahGame_Main*)g_GameStep[ GAMESTEP_GAME];
	pGameMainStep->ReleasePortalNPC();

	pGameMainStep->m_nPortalNPCCount = bNumLinkMap;

	for(int i=0; i < bNumLinkMap; ++i) 
	{
		XiahMap::sPortalInfo sPortal;
	
		msg
			>> sPortal.m_dwLinkMapID
			>> sPortal.m_wPortalPosX
			>> sPortal.m_wPortalPosY
			>> sPortal.m_wPortalWidth
			>> sPortal.m_wPortalHeight
			>> sPortal.m_bLinkType;

		XiahMap::g_XiahMap.m_PortalInfoList.push_back( sPortal);

		// 얘는 Functional NPC 닷. Ani없고 Mesh만 있는데, 여기에 이펙트가 붙어있다. 
		CXiahCharObject *pObject = new CXiahCharObject;
		pGameMainStep->m_pPortalNPC[i] = XiahObject::g_XiahObjectManager.CreateXiahObject( 0, OBJTYPE_FUNCTIONALNPC, pObject);

		int nCharID = MUNPA_PORTAL_CHAR_ID;
		if( sPortal.m_bLinkType == 7 || sPortal.m_bLinkType == 6 )
			nCharID = MUNPA_PORTAL_CHAR_ID2;

		pObject->Create( nCharID, 0, 0, -1 );
		pObject->m_CharRender.MakeMeshEffect();

		pObject->m_pAniType = XiahAniType::GetAniType( OBJTYPE_FUNCTIONALNPC, 0);

		pObject->SetAngle( 0 );
		// 위치가 시작점으로 맞춰져 있어서 중앙으로 해주자
		pObject->SetPosition( sPortal.m_wPortalPosX + sPortal.m_wPortalWidth/2, sPortal.m_wPortalPosY + sPortal.m_wPortalHeight/2 );

		LPCTSTR szName;
		if( sPortal.m_bLinkType == 7 || sPortal.m_bLinkType == 6 )
			szName = IDS_MUNPA_PORTAL;
		else
			szName = IDS_PORTAL;

		pObject->m_szObjectName = szName;
		pObject->m_bObjType = OBJTYPE_FUNCTIONALNPC;
		pObject->m_bSubObjType = 0;

		sFunctionalNpcInfo* pInfo = new sFunctionalNpcInfo;
		pInfo->m_dwObjectID = 0;
		pInfo->m_bType		= 0;
		pInfo->m_bKind		= 0;
		pInfo->m_szName		= szName;
		pInfo->m_bOwnType	= 0;
		pInfo->m_dwOwnID	= 0;
		pInfo->m_bSizeX		= sPortal.m_wPortalWidth;
		pInfo->m_bSizeY		= sPortal.m_wPortalHeight;
		pInfo->m_wNumItem	= 0;

		pObject->m_pPrivateData = (DWORD)pInfo;
		pObject->m_PrivateDataDestoryer = ReleaseFunctionalNpcInfo; // 앗싸

	}// for

	if( g_pMainChar != NULL) // 맵간 이동일때
	{
		SendCS_NV_MAPENTER_REQ( g_pMainChar->m_dwServerID, dwMapID);

		CXiahCharObject* pChar = (CXiahCharObject*)g_pMainChar->m_pObject;

		if(pChar)
		{
			pChar->SetAnimation(XiahAniType::eLAT_Stand, -1);
			pChar->m_bRide = FALSE;
		}
	}
	else	// Intro일때
	{
		SendCS_NV_MAPENTER_REQ( g_pIntro->GetCurrentChar()->m_dwObjectID, dwMapID);
	}

	DBG_Put(_T("맵정보수신 MAP ID - %d"), dwMapID);

	g_MainCharInfo.m_pImageScrMsg->AllHide();
	g_MainCharInfo.m_bEvSocketItemUse = false;
	g_MainCharInfo.m_bPortalMove = true;

	return TRUE;
}


////////////////////////////////////////////
int OnCS_IT_GENERALMUGONGLIST_ACK( CMsg &msg)
////////////////////////////////////////////
{
	BYTE bNumOfMugong	=0;

	msg
		>> bNumOfMugong;

	for(int i=0; i < bNumOfMugong; ++i)
	{
		DWORD dwMugongID	=0;
		sString szName;
		BYTE bKind			=0;
		BYTE bMugongLevel	=0;
		BYTE bMacroSeq		=0;

		msg
			>> dwMugongID
			>> szName
			>> bKind
			>> bMugongLevel
			>> bMacroSeq;

        TCHAR szDbg[256];
        _stprintf(szDbg, _T("[MUGONG DEBUG] Parsed 0x4419. ID: %d, Name: %s, Kind: %d, Level: %d"), dwMugongID, szName.c_str(), bKind, bMugongLevel);
        DBG_LogFile(szDbg);

        g_MainCharInfo.m_pMugong->InsertMugong( dwMugongID, 1, bMugongLevel);
		
	}

	g_MainCharInfo.RefreshMugongFrame(TRUE);

	return TRUE;
}


/**
 * 상태 무공 리스트 패킷
 * \param &msg 
 * \return 
 */
int OnCS_IT_PASSIVEMUGONGLIST_ACK( CMsg &msg)
{
	BYTE bNumOfMugong	=0;

	msg
		>> bNumOfMugong;
	
	for(int i=0; i < bNumOfMugong; ++i)
	{
		DWORD dwMugongID	=0;		
		BYTE bKind			=0;
		BYTE bMugongLevel	=0;
		BYTE bMacroSeq		=0;
		
		msg
			>> dwMugongID
			>> bKind
			>> bMugongLevel
			>> bMacroSeq;

		// 오행
		if(bKind >= 80 && bKind <= 84)
			g_MainCharInfo.m_pMugong->InsertMugong( dwMugongID, MUGONGTYPE_FIVEELEMENT, bMugongLevel);
		else
			g_MainCharInfo.m_pMugong->InsertMugong( dwMugongID, 0, bMugongLevel);
	}

	g_MainCharInfo.RefreshMugongFrame(TRUE);

	return TRUE;
}


/**
 * 무공
 * \param &msg 
 * \return 
 */
int OnCS_IT_ACTIVEMUGONGLIST_ACK( CMsg &msg)
{
	BYTE bNumOfMugong	=0;

	msg
		>> bNumOfMugong;
	
	int i, y;

	for(i=0, y = 0; i < bNumOfMugong; ++i)
	{
		DWORD dwMugongID	=0;		
		BYTE bKind			=0;
		BYTE bMugongLevel	=0;
		BYTE bMacroSeq		=0;
		
		msg
			>> dwMugongID
			>> bKind
			>> bMugongLevel
			>> bMacroSeq;

		//HT_0711 : 진각성 무공
		if(dwMugongID >= 191 && dwMugongID <= 198)
		{
			g_MainCharInfo.m_pMugong->InsertMugong( dwMugongID, MUGONGTYPE_2TH_REBIRTH1+y, bMugongLevel);
			y++;
		}
		else
			g_MainCharInfo.m_pMugong->InsertMugong( dwMugongID, 1, bMugongLevel);
	}

	g_MainCharInfo.RefreshMugongFrame( TRUE);

	return TRUE;
}

//YS_0728 : BUGFIX
int OnCS_IT_ITEMLIST_ACK( CMsg &msg)
{
	BYTE bSackID	=	0;
	BYTE bSackPos	=	0;
	BYTE bNumItem	=	0;

	msg
		>> bSackID
		>> bNumItem;

	if (bSackID == 3) {
		g_MainCharInfo.m_nVIPLevel = 5; // 收到第 3 页数据包，激活本地 VIP 状态标志
	}

	for(int i=0; i < bNumItem; ++i) 
	{
		XiahItem::sItemInfo* pItem = new XiahItem::sItemInfo;

		pItem->m_bSackID = bSackID;

		msg
			>> bSackPos;

		XiahItem::GetItemData( pItem, msg);

		switch( pItem->m_bSackID)
		{
		case SACKTYPE__EQUIPMENT:
			g_MainCharInfo.m_pEquipSack->InsertItem( bSackPos, pItem);
			break;
		case SACKTYPE__DEFAULT:
			g_MainCharInfo.m_pMySack[0]->InsertItem( bSackPos, pItem);
			break;
		case SACKTYPE__DEFAULT2:
			pItem->m_bSackCount = 1;
			g_MainCharInfo.m_pMySack[1]->InsertItem( bSackPos, pItem);
			break;
		case 3: // SACKTYPE__DEFAULT3 (VIP专属背包)
			pItem->m_bSackID = SACKTYPE__DEFAULT;
			pItem->m_bSackCount = 2;
			g_MainCharInfo.m_pMySack[2]->InsertItem( bSackPos, pItem);
			break;
		case SACKTYPE_COLLECTION:	// 아이템 수집
			{
				g_MainCharInfo.m_pCollection->InsertItem(bSackPos, pItem);
			}
			break;
		}

		//HT_1116 : 각성자 아이템 추가
		msg
			>> pItem->m_wRebuithValue;
		
	}

	return TRUE;
}

//---------------------------------------------------------------------------------------
//YS_0728 : BUGFIX
int OnCS_IT_CHARINFO_ACK(CMsg &msg)
{
	BYTE	bResult		= 0;	
	BYTE	bWalkSpeed	= 0;
	BYTE	bHeight		= 0;
	BYTE	bDesHeight	= 0;
	BYTE	bState		= 0;
	BYTE	bCharType	= 0;
	BYTE	bShopStatus	 = 0;
	BYTE	bSemiPKStatus= 0;
	BYTE	bWarStatus	 = 0;
	BYTE	bCurFiveElm	 = 0;	// 오행
	BYTE	bFELevel	 = 0;	// 오행

	// CG_2005/01/28 : 변종아이템기능추가
	BYTE	bChangeItemSet	= 0; //(추가) 동일변종 아이템을 모두 장착하고 있을 경우 - 공격력 25% 상승 : 1 완성 0 : 미완성
	BYTE	bPotionEndKeepup= 0; // 설승단약
	BYTE	bSpirit			=0;

	DWORD   dwObjectID		= 0;
	DWORD	dwMapID			= 0;
	DWORD   dwMunpaID		= 0;
	DWORD	dwMunpaMarkID	= 0;
	DWORD   dwMunpaOrder	= 0;
	DWORD	dwFame			= 0;
	DWORD	dwPartyID		= 0;
	DWORD	dwPartyLeaderID = 0;
	DWORD	dwEnemyPartyID	= 0;	
	DWORD	dwEnemyMunpaID	= 0;
	DWORD	dwEnemyStoneID	= 0;

	WORD	wPosX		=	0;
	WORD	wPosY		=	0;
	WORD	wDesPosX	=	0;
	WORD	wDesPosY	=	0;	
	WORD	wDirection	=	0;
	BYTE	bRebirth	=	0;
	BYTE	bPoisonUnderCover = 0;
	
	sString szName;
	sString szMunpaName;	
	sString szMunpaNickName;	
	sString strShopName;
	sString strShopDescription;	
	sString strEnemyMunpaName;

	WORD	wVisualID[9];
	BYTE	bRarity[9];
	BYTE	bStxType[9];

	DWORD	bwGMMark;

	// CG_2005/01/28 : 변종아이템기능추가
	BYTE	bNeedCharType[ 9 ];		// 변종아이템일 경우 - 변종아이템 종류 구분 
	// 251 : 검영변종 , 252 : 연랑변종, 253 : 무투변종, 254 : 야차변종

	ZeroMemory( wVisualID,	sizeof( WORD) * 9);
	ZeroMemory( bRarity,	sizeof( BYTE) * 9);
	ZeroMemory( bStxType,	sizeof( BYTE) * 9);
	// CG_2005/01/28 : 변종아이템기능추가
	ZeroMemory( bNeedCharType,	sizeof( BYTE ) * 9 );

	msg
		>> bResult;

	if( bResult != 0 )
		return TRUE;

	BYTE bInstanceCnt =0;

	msg
		>> dwObjectID
		>> dwMapID
		>> wPosX
		>> wPosY
		>> bHeight
		>> wDesPosX
		>> wDesPosY
		>> bDesHeight
		>> bWalkSpeed
		>> wDirection
		>> bState
		>> bCharType
		>> szName
		>> dwFame
		>> bShopStatus
		>> strShopName
		>> strShopDescription
		>> bSemiPKStatus
		>> bCurFiveElm
		>> bFELevel
		>> bInstanceCnt;

	if ( dwMapID > 15 || wPosX > 2047 || wPosY > 2047 )
		return TRUE;

	for(int i=0; i < bInstanceCnt; ++i)
	{
		BYTE bInstanceType =0;

		msg
			>> bInstanceType;

		switch(bInstanceType)
		{
		case 0:	// 설승단약
			bPotionEndKeepup = 1;	break;
		case 1:	// 기
			bSpirit			 = 1;	break;
		default:
			break;
		}
	}

	msg
		>> bChangeItemSet		// CG_2005/01/28 : 변종아이템 셋트 체크
		>> dwMunpaID;

	if(dwMunpaID)
	{
        msg
			>> szMunpaName
			>> dwMunpaOrder
			>> szMunpaNickName
			>> dwMunpaMarkID
			>> bWarStatus;

		// 문파전
		switch(bWarStatus)
		{
		case 1:	// 대기
		case 2:	// 시작
            {
				msg
					>> dwEnemyMunpaID
					>> strEnemyMunpaName
					>> dwEnemyStoneID;
			}
			break;
		} // switch(bWarStatus)
	} // if(dwMunpaID)

	msg
		>> dwPartyID;

	if(dwPartyID)
	{
		msg
			>> dwPartyLeaderID
			>> dwEnemyPartyID;
	} // if(dwPartyID)

	// CG_2005/01/28 : 변종아이템기능추가

	// 미사용패킷 만들지좀 말아라
	msg
		>> wVisualID[ EQUIPPOS_WEAPON ]
		>> bRarity[ EQUIPPOS_WEAPON ]
		>> bStxType[ EQUIPPOS_WEAPON ]
		>> bNeedCharType[ EQUIPPOS_WEAPON ]
		>> wVisualID[ EQUIPPOS_HAT]
		>> bRarity[ EQUIPPOS_HAT]
		>> bStxType[ EQUIPPOS_HAT]
		>> bNeedCharType[ EQUIPPOS_HAT ]
		>> wVisualID[ EQUIPPOS_CLOTH]
		>> bRarity[ EQUIPPOS_CLOTH]
		>> bStxType[ EQUIPPOS_CLOTH]
		>> bNeedCharType[ EQUIPPOS_CLOTH ]
		>> wVisualID[ EQUIPPOS_SHOE]
		>> bRarity[ EQUIPPOS_SHOE]
		>> bStxType[ EQUIPPOS_SHOE]
		>> bNeedCharType[ EQUIPPOS_SHOE ]
		>> wVisualID[ EQUIPPOS_PROTECTOR]
		>> bRarity[ EQUIPPOS_PROTECTOR]
		>> bStxType[ EQUIPPOS_PROTECTOR]
		>> bNeedCharType[ EQUIPPOS_PROTECTOR ]
		>> wVisualID[ EQUIPPOS_RING]
		>> bRarity[ EQUIPPOS_RING]
		>> bStxType[ EQUIPPOS_RING]
		>> bNeedCharType[ EQUIPPOS_RING ]
		>> wVisualID[ EQUIPPOS_NECLACE]
		>> bRarity[ EQUIPPOS_NECLACE]
		>> bStxType[ EQUIPPOS_NECLACE]
		>> bNeedCharType[ EQUIPPOS_NECLACE ]
		>> wVisualID[ EQUIPPOS_CLOAK]
		>> bRarity[ EQUIPPOS_CLOAK]
		>> bStxType[ EQUIPPOS_CLOAK]
		>> bNeedCharType[ EQUIPPOS_CLOAK ]
		>> wVisualID[ EQUIPPOS_BONGIN]
		>> bRarity[ EQUIPPOS_BONGIN]
		>> bStxType[ EQUIPPOS_BONGIN]
		>> bNeedCharType[ EQUIPPOS_BONGIN ]
		>> bRebirth
		>> bPoisonUnderCover
		>> bwGMMark;						//HT_1023 : 운영자 마크 추가
		
	// 가끔씩 서버에서 쓰레기를 보내는 경우가 있다. 이럴경우를 걸러내자!
	if( dwObjectID == 0 || dwObjectID < 400000000 || wPosX > 2047 || wPosY > 2047 || wPosX < 0 || wPosY < 0)
	{
		DBG_Put("Server sent garbage character!");
		return TRUE;
	}

	// 새로운 캐릭터면 첨부터 생성하지만, 존재하는 거라면 데이타를 바꿔준다.
	bool bCreateChar = true;

	XiahObject::CXiahObject* pXiahObject = XiahObject::g_XiahObjectManager.FindXiahObject( MAKEOBJECTID( 0, dwObjectID,OBJTYPE_PC) );

	if( pXiahObject != NULL ) // 캐릭터 데이타가 있으면 new 하지 않고 데이타만 바꿔준다.
		bCreateChar = false;

	sArrayData *pData = XiahArrayIndex::g_MainCharType.GetData( bCharType, 0);
	if( pData == NULL)
		return TRUE;

	CXiahCharObject* pObject = NULL;
	if( bCreateChar )
	{
		pObject = (CXiahCharObject *)XiahObject::g_XiahCharPool.GetChar(); //new CXiahCharObject;

		if( NULL == pObject )
		{
			pObject = new CXiahCharObject;			
		}
	}
	else
	{
		pObject = reinterpret_cast<CXiahCharObject*>(pXiahObject->m_pObject);

		//YS_0811 : BUGFIX
		if ( !pObject )
			return TRUE;
	}

	int nCharID		= pData->GetInt( 2);
	int nMeshType	= pData->GetInt( 3);
	int nTextureType = pData->GetInt( 4);

	if( bCreateChar )
	{
		pObject->Create( nCharID, nMeshType, nTextureType, 103);
		pObject->m_szObjectName = szName;

		XiahObject::g_XiahObjectManager.CreateXiahObject( dwObjectID, OBJTYPE_PC, pObject);

		pObject->m_pAniType = XiahAniType::GetAniType( OBJTYPE_PC, 0);

		pObject->SetPosition( wPosX, wPosY);
		pObject->SetAngle( wDirection);
		pObject->m_bObjType = OBJTYPE_PC;

		pObject->m_bSubObjType = bCharType;

		// PC만 칼 궤적 이펙트를 생성한다.
		pObject->m_SwordTrace.Init();

		// 야차는 왼손 칼 궤적도 있다.
		if( pObject->m_bSubObjType == 4 )
			pObject->m_SwordTrace2.Init();

		// If the server indicates this character is already moving (dest != pos), start run animation directly
		if (wDesPosX != wPosX || wDesPosY != wPosY)
		{
			pObject->SetAnimation( XiahAniType::eLAT_Run, 1);
			pObject->SetTargetMove( wDesPosX, wDesPosY, eLBP_CharNavigation, 0);
			float fScale = (float)bWalkSpeed / 9.0f;
			if( fScale > 2.0f) fScale = 2.0f;
			if( fScale < 0.1f) fScale = 1.0f;
			pObject->m_CharRender.SetAnimationSpeed( fScale);
		}
		else
		{
			pObject->SetAnimation( XiahAniType::eLAT_Spawn, XiahAniType::eLAT_Stand, 0, 0);
		}

		// 무기에 이펙트가 붙는지 여부는 여기서 하자.
		SetupPC_VisualEquipement( pObject, wVisualID, bRarity, bStxType );

		sPCVisualInfo* pVisualInfo = new sPCVisualInfo;

		memcpy( pVisualInfo->wVisualID, wVisualID, sizeof( WORD) * 9);
		memcpy( pVisualInfo->bRarity, bRarity, sizeof( BYTE) * 9);
		memcpy( pVisualInfo->bStxType, bStxType, sizeof( BYTE) * 9);

		pObject->m_pPrivateData = (DWORD)pVisualInfo;
		pObject->m_PrivateDataDestoryer = ReleasePCVisualnfo;
	}
	else	// 이걸 빼먹었네 ^^;
	{
		// PC만 칼 궤적 이펙트를 생성한다.
		pObject->m_SwordTrace.Init();

		// 야차는 왼손 칼 궤적도 있다.
		if( pObject->m_bSubObjType == 4 )
			pObject->m_SwordTrace2.Init();

		SetupPC_VisualEquipement( pObject, wVisualID, bRarity, bStxType );

		sPCVisualInfo* pVisualInfo = (sPCVisualInfo*)pObject->m_pPrivateData;

		//YS_0811 : BUGFIX
		if (!pVisualInfo )
		{
			pVisualInfo = new sPCVisualInfo;
		}			

		memcpy( pVisualInfo->wVisualID, wVisualID, sizeof( WORD) * 9);
		memcpy( pVisualInfo->bRarity, bRarity, sizeof( BYTE) * 9);
		memcpy( pVisualInfo->bStxType, bStxType, sizeof( BYTE) * 9);
	}

	bool bRelation = false;

	// 캐릭터이름에 색을 입히자
	if( g_MainCharInfo.m_pRelation->FindDanInfoByID( dwObjectID))
	{
		pObject->m_cNameColor = D3DCOLOR_XRGB( 200, 255, 255);
		bRelation = true;
	}
	else if( g_MainCharInfo.m_pRelation->FindShipInfoByID( dwObjectID))
	{
		pObject->m_cNameColor = D3DCOLOR_XRGB( 255, 200, 255);
		bRelation = true;
	}
	else if( g_MainCharInfo.m_pRelation->FindClanInfoByID( dwObjectID))
	{
		pObject->m_cNameColor = D3DCOLOR_XRGB( 200, 255, 200);
		bRelation = true;
	}

	// 명성치
	pObject->m_dwFame			= dwFame;
	pObject->m_dwPartyID		= dwPartyID;
	pObject->m_dwPartyLeaderID	= dwPartyLeaderID;
	pObject->m_dwEnemyPartyID	= dwEnemyPartyID;

	// 상점 상태
	if(bShopStatus == 0)
	{
        pObject->m_bTradeSell = false;
	}
	else if(bShopStatus == 1)
	{
		pObject->m_bTradeSell	= true;
		pObject->m_strShopName	= strShopName;
		pObject->m_strShopDescription = strShopDescription;
	}
	else
	{
		DBG_Put(_T("OnCS_IT_CHARINFO_ACK 개인노점 상태 비정상 결과 %d"),bShopStatus);
		// 비정상 데이터
	}

	if(dwFame >= 127 && bRelation)
	{
	}
	else
	{
		pObject->RefreshFameColor();
	}

	if(dwMunpaID)
	{
		pObject->m_dwMunpaID		= dwMunpaID;
		pObject->m_szMunpaName		= szMunpaName;
		pObject->m_szMunpaNickName	= szMunpaNickName;
		pObject->m_dwMunpaOrder		= dwMunpaOrder;
		pObject->m_dwMunpaMarkID	= dwMunpaMarkID;	// 문파문장 ID

		if(bWarStatus)
		{
			pObject->m_bWarStatus		= bWarStatus;
			pObject->m_dwEnemyMunpaID	= dwEnemyMunpaID;
			pObject->m_dwEnemyStoneID	= dwEnemyStoneID;
			pObject->m_strEnemyMunpaName = strEnemyMunpaName;
		}
	} // if( dwMunpaID)

	// Semi PK 상태를 설정
	pObject->m_bSemiPKStatus = bSemiPKStatus;

	// CG_2005/01/28 : 변종아이템기능추가
	pObject->m_bChangeItemSet = bChangeItemSet;

	// 기
	pObject->m_bSpirit = bSpirit;

	// 설승단약
	pObject->m_bPotionEndKeepup = bPotionEndKeepup;

	// 환생 수
	pObject->m_bRebirth = bRebirth;
	pObject->m_bGameMasterMark = (BYTE)bwGMMark;		//HT_1023 : 운영자 마크 추가

	/////////////////////////////////////////////////////////////////////////////////////////////////////
	// 오행
	pObject->m_bFECur   = bCurFiveElm;
	pObject->m_bFELevel = bFELevel;

	DWORD dwMugongID = static_cast<DWORD>(bCurFiveElm) + FIVEELEMENT_FIRE - 1;
	DWORD dwTime =0;

	sArrayData* pMugongList = XiahArrayIndex::g_MugongList.GetData(dwMugongID, bFELevel);					

	if(pMugongList)
		dwTime = pMugongList->GetInt(9);

	pObject->m_KeepUpMugongList.Add(dwMugongID, bFELevel, dwTime);
	/////////////////////////////////////////////////////////////////////////////////////////////////////

	if(bPotionEndKeepup == 0)
	{
		if(pObject->m_pEventItemEffectPP)
		{
			g_EffectManager.DeqEffectPackagePair(pObject->m_pEventItemEffectPP);
			pObject->m_pEventItemEffectPP = NULL;
		}
	}

	if(bSpirit == 0)
	{
		if(pObject->m_pSpiritEffectPP)
		{
			g_EffectManager.DeqEffectPackagePair(pObject->m_pSpiritEffectPP);
			pObject->m_pSpiritEffectPP = NULL;
		}
	}

	if(bPoisonUnderCover)
		pObject->m_bRenderOK = false;		
	else
		pObject->m_bRenderOK = true;
		

	return TRUE;
}



//---------------------------------------------------------------------------------------
//YS_0728 : BUGFIX
int OnCS_IT_CHARINFOLIST_ACK(CMsg &msg)
{
	BYTE   bResult		=	0;
	
	msg
		>> bResult;

	if ( bResult != 0 )
		return TRUE;
    
	DWORD  dwMapID		=	0;
	WORD   wObjectNum	=	0;
	bool   bPartyExist  = false;
	
	msg
		>> dwMapID
		>> wObjectNum;

	for(int i = 0; i < wObjectNum; ++i)
	{
		WORD	wPosX				= 0;
		WORD	wPosY				= 0;
		WORD	wDesPosX			= 0;
		WORD	wDesPosY			= 0;
		WORD	wDirection			= 0;

		BYTE	bWalkSpeed			= 0;
		BYTE	bHeight				= 0;
		BYTE	bDesHeight			= 0;		
		BYTE	bState				= 0;
		BYTE	bCharType			= 0;
		BYTE	bShopStatus			= 0;
		BYTE	bWarStatus			= 0;
		BYTE	bCurFiveElm			= 0;	// 오행
		BYTE	bFELevel			= 0;	// 오행
		BYTE	bSemiPKStatus		= 0;
		// CG_2005/01/28 : 변종아이템기능추가
		BYTE	bChangeItemSet		= 0; //(추가) 동일변종 아이템을 모두 장착하고 있을 경우 - 공격력 25% 상승 : 1 완성 0 : 미완성
		BYTE	bPotionEndKeepup	= 0; // 설승단약
		BYTE	bSpirit				=0;

		BYTE	bInstanceCnt		=0;
		
		DWORD   dwObjectID			= 0;
		DWORD	dwFame				= 0;
		DWORD   dwMunpaID			= 0;
		DWORD	dwMunpaMarkID		= 0;		
		DWORD   dwMunpaOrder		= 0;
		DWORD	dwPartyID			= 0;
		DWORD	dwPartyLeaderID		= 0;
		DWORD	dwEnemyPartyID		= 0;
		DWORD	dwEnemyMunpaID		= 0;
		DWORD	dwEnemyStoneID		= 0;
		BYTE	bRebirth			= 0;
		BYTE	bPoisonUnderCover = 0;
		
		sString szName;
		sString szMunpaName;
		sString szMunpaNickName;
		sString strShopName;
		sString strShopDescription;
		sString strEnemyMunpaName;

		WORD	wVisualID[ 9];
		BYTE	bRarity[ 9];
		BYTE	bStxType[ 9];
		// CG_2005/01/28 : 변종아이템기능추가
		BYTE	bNeedCharType[ 9 ];		// 변종아이템일 경우 - 변종아이템 종류 구분 
		// 251 : 검영변종 , 252 : 연랑변종, 253 : 무투변종, 254 : 야차변종

		DWORD	bwGMMark;
		ZeroMemory( wVisualID,		sizeof(WORD) * 9);
		ZeroMemory( bRarity,		sizeof(BYTE) * 9);
		ZeroMemory( bStxType,		sizeof(BYTE) * 9);
		// CG_2005/01/28 : 변종아이템기능추가
		ZeroMemory( bNeedCharType,	sizeof(BYTE) * 9);

		msg
			>> dwObjectID // 0
			>> wPosX
			>> wPosY
			>> bHeight
			>> wDesPosX
			>> wDesPosY
			>> bDesHeight
			>> bWalkSpeed
			>> wDirection
			>> bState
			>> bCharType
			>> szName	
			>> dwFame
			>> bShopStatus
			>> strShopName
			>> strShopDescription
			>> bSemiPKStatus
			>> bCurFiveElm
			>> bFELevel
			>> bInstanceCnt;

		for(int i=0; i < bInstanceCnt; ++i)
		{
			BYTE bInstanceType =0;

			msg
				>> bInstanceType;

			switch(bInstanceType)
			{
			case 0:	// 설승단약
				bPotionEndKeepup = 1;	break;
			case 1:	// 기
				bSpirit			 = 1;	break;
			default:
				break;
			}
		}

		msg
			>> bChangeItemSet		// CG_2005/01/28 : 변종아이템 셋트 체크
			>> dwMunpaID;

		if(dwMunpaID)
		{
			msg
				>> szMunpaName
				>> dwMunpaOrder
				>> szMunpaNickName
				>> dwMunpaMarkID
				>> bWarStatus;

			// 문파전
			switch(bWarStatus)
			{
			case 1:	// 대기
			case 2:	// 시작
				{
					msg
						>> dwEnemyMunpaID
						>> strEnemyMunpaName
						>> dwEnemyStoneID;
				}
				break;
			} // switch(bWarStatus)
		} // if(dwMunpaID)

		msg
			>> dwPartyID;

		if(dwPartyID)
		{
			bPartyExist = true;

			msg
				>> dwPartyLeaderID
				>> dwEnemyPartyID;
		}

		// CG_2005/01/28 : 변종아이템기능추가
		msg
			>> wVisualID[ 0 ]
			>> bRarity[ 0 ]
			>> bStxType[ 0 ]
			>> bNeedCharType[ 0 ]
			>> wVisualID[ 1 ]
			>> bRarity[ 1 ]
			>> bStxType[ 1 ]
			>> bNeedCharType[ 1 ]
			>> wVisualID[ 2 ]
			>> bRarity[ 2 ]
			>> bStxType[ 2 ]
			>> bNeedCharType[ 2 ]
			>> wVisualID[ 3 ]
			>> bRarity[ 3 ]
			>> bStxType[ 3 ]
			>> bNeedCharType[ 3 ]
			>> wVisualID[ 4 ]
			>> bRarity[ 4 ]
			>> bStxType[ 4 ]
			>> bNeedCharType[ 4 ]
			>> wVisualID[ 5 ]
			>> bRarity[ 5 ]
			>> bStxType[ 5 ]
			>> bNeedCharType[ 5 ]
			>> wVisualID[ 6 ]
			>> bRarity[ 6 ]
			>> bStxType[ 6 ]
			>> bNeedCharType[ 6 ]
			>> wVisualID[ 7 ]
			>> bRarity[ 7 ]
			>> bStxType[ 7 ]
			>> bNeedCharType[ 7 ]
			>> wVisualID[ 8 ]
			>> bRarity[ 8 ]
			>> bStxType[ 8 ]
			>> bNeedCharType[ 8 ]
            >> bRebirth
			>> bPoisonUnderCover
			>> bwGMMark;					//HT_1023 : 운영자 마크 추가

		if( dwObjectID == 0 || dwMapID > 15 || wPosX > 2047 || wPosY > 2047 )
			continue;
		
		// 새로운 캐릭터면 첨부터 생성하지만, 존재하는 거라면 데이타를 바꿔준다.
		bool bCreateChar = true;

		XiahObject::CXiahObject* pXiahObject = XiahObject::g_XiahObjectManager.FindXiahObject( MAKEOBJECTID( 0, dwObjectID,OBJTYPE_PC) );

		if( pXiahObject != NULL ) // 캐릭터 데이타가 있으면 new 하지 않고 데이타만 바꿔준다.
			bCreateChar = false;

		sArrayData *pData = XiahArrayIndex::g_MainCharType.GetData( bCharType, 0);
		if( pData == NULL)
			continue;

		CXiahCharObject* pObject = NULL;
		if( bCreateChar )
		{
			pObject = (CXiahCharObject *)XiahObject::g_XiahCharPool.GetChar(); //new CXiahCharObject;

			if ( NULL == pObject )
			{
				pObject = new CXiahCharObject;			
			}
		}
		else if ( pXiahObject->m_pObject )
		{
			pObject = reinterpret_cast<CXiahCharObject*>(pXiahObject->m_pObject);

			//YS_0811 : BUGFIX
			if ( !pObject )
				continue;
		}
		else
		{
			continue;
		}

		int nCharID		 = pData->GetInt( 2);
		int nMeshType	 = pData->GetInt( 3);
		int nTextureType = pData->GetInt( 4);

		if( bCreateChar )
		{
			pObject->Create( nCharID, nMeshType, nTextureType, 103);
			pObject->m_szObjectName = szName;

			XiahObject::g_XiahObjectManager.CreateXiahObject( dwObjectID, OBJTYPE_PC, pObject);

			pObject->m_pAniType = XiahAniType::GetAniType( OBJTYPE_PC, 0);

			pObject->SetPosition( wPosX, wPosY);
			pObject->SetAngle( wDirection);
			pObject->m_bObjType = OBJTYPE_PC;

			pObject->m_bSubObjType = bCharType;

			// PC만 칼 궤적 이펙트를 생성한다.
			pObject->m_SwordTrace.Init();

			// 야차는 왼손 칼 궤적도 있다.
			if( pObject->m_bSubObjType == 4 )
				pObject->m_SwordTrace2.Init();

			SetupPC_VisualEquipement( pObject, wVisualID, bRarity, bStxType );

			// 채집 위 셋팅 아래로... (위 셋팅에서 서있는 애니로 설정됨)
			if(ACT_MINE == bState)
				pObject->SetAnimation(XiahAniType::eLAT_Collect, 0);
			else
				pObject->SetAnimation( XiahAniType::eLAT_Spawn, XiahAniType::eLAT_Stand, 0, 0);			

			// If the server indicates this character is already moving (dest != pos), start the run animation immediately
			if (wDesPosX != wPosX || wDesPosY != wPosY)
			{
				pObject->SetAnimation( XiahAniType::eLAT_Run, 1);
				pObject->SetTargetMove( wDesPosX, wDesPosY, eLBP_CharNavigation, 0);
				float fScale = (float)bWalkSpeed / 9.0f;
				if( fScale > 2.0f) fScale = 2.0f;
				if( fScale < 0.1f) fScale = 1.0f;
				pObject->m_CharRender.SetAnimationSpeed( fScale);
			}

			sPCVisualInfo* pVisualInfo = new sPCVisualInfo;

			memcpy( pVisualInfo->wVisualID, wVisualID, sizeof( WORD) * 9);
			memcpy( pVisualInfo->bRarity, bRarity, sizeof( BYTE) * 9);
			memcpy( pVisualInfo->bStxType, bStxType, sizeof( BYTE) * 9);

			pObject->m_pPrivateData = (DWORD)pVisualInfo;
			pObject->m_PrivateDataDestoryer = ReleasePCVisualnfo;
		}
		else	// 이걸 빼먹었네 ^^;
		{
			// PC만 칼 궤적 이펙트를 생성한다.
			pObject->m_SwordTrace.Init();

			// 야차는 왼손 칼 궤적도 있다.
			if( pObject->m_bSubObjType == 4 )
				pObject->m_SwordTrace2.Init();

			SetupPC_VisualEquipement( pObject, wVisualID, bRarity, bStxType );

			sPCVisualInfo* pVisualInfo = (sPCVisualInfo*)pObject->m_pPrivateData;

			//YS_0811 : BUGFIX
			if (!pVisualInfo )
			{
				pVisualInfo = new sPCVisualInfo;
			}

			memcpy( pVisualInfo->wVisualID, wVisualID, sizeof( WORD) * 9);
			memcpy( pVisualInfo->bRarity, bRarity, sizeof( BYTE) * 9);
			memcpy( pVisualInfo->bStxType, bStxType, sizeof( BYTE) * 9);
		}

		// 내 주위에 다른 캐릭터가 포탈로 이동해서 들어오면 그 캐릭터가 무공 지속 이펙트가 있는지 검사.

		bool bRelation = false;

		// 캐릭터이름에 색을 입히자
		if( g_MainCharInfo.m_pRelation->FindDanInfoByID( dwObjectID))
		{
			pObject->m_cNameColor = D3DCOLOR_XRGB( 200, 255, 255);
			bRelation = true;
		}
		else if( g_MainCharInfo.m_pRelation->FindShipInfoByID( dwObjectID))
		{
			pObject->m_cNameColor = D3DCOLOR_XRGB( 255, 200, 255);
			bRelation = true;
		}
		else if( g_MainCharInfo.m_pRelation->FindClanInfoByID( dwObjectID))
		{
			pObject->m_cNameColor = D3DCOLOR_XRGB( 200, 255, 200);
			bRelation = true;
		}

		// 명성치
		pObject->m_dwFame = dwFame;

		pObject->m_dwPartyID = dwPartyID;
		pObject->m_dwPartyLeaderID = dwPartyLeaderID;
		pObject->m_dwEnemyPartyID = dwEnemyPartyID;

		// 상점 상태
		if(bShopStatus == 0)
		{
			pObject->m_bTradeSell = false;
		}
		else if(bShopStatus == 1)
		{
			pObject->m_bTradeSell = true;

			pObject->m_strShopName = strShopName;
			pObject->m_strShopDescription = strShopDescription;
		}
		else
		{
			DBG_Put(_T("OnCS_IT_CHARINFOLIST_ACK 개인노점 상태 비정상 결과 %d"),bShopStatus);
			// 비정상 데이터
		}

		if(dwFame >= 127 && bRelation)
		{
		}
		else
		{
			pObject->RefreshFameColor();
		}

		if( dwMunpaID)
		{
			pObject->m_dwMunpaID		= dwMunpaID;
			pObject->m_szMunpaName		= szMunpaName;
			pObject->m_szMunpaNickName	= szMunpaNickName;
			pObject->m_dwMunpaOrder		= dwMunpaOrder;
			pObject->m_dwMunpaMarkID	= dwMunpaMarkID;

			if(bWarStatus)
			{
				pObject->m_bWarStatus		= bWarStatus;
				pObject->m_dwEnemyMunpaID	= dwEnemyMunpaID;
				pObject->m_dwEnemyStoneID	= dwEnemyStoneID;
				pObject->m_strEnemyMunpaName = strEnemyMunpaName;
			}
		}

		// Semi PK 상태를 설정
		pObject->m_bSemiPKStatus = bSemiPKStatus;

		// CG_2005/01/28 : 변종아이템기능추가
		pObject->m_bChangeItemSet = bChangeItemSet;

		// 기
		pObject->m_bSpirit = bSpirit;

		// 설승단약
		pObject->m_bPotionEndKeepup = bPotionEndKeepup;

		// 환생 수
		pObject->m_bRebirth = bRebirth;
		pObject->m_bGameMasterMark = (BYTE)bwGMMark;//HT_1023 : 운영자 마크 추가

		/////////////////////////////////////////////////////////////////////////////////////////////////////
		// 오행
		pObject->m_bFECur   = bCurFiveElm;
		pObject->m_bFELevel = bFELevel;

		DWORD dwMugongID = static_cast<DWORD>(bCurFiveElm) + FIVEELEMENT_FIRE - 1;
		DWORD dwTime =0;

		sArrayData* pMugongList = XiahArrayIndex::g_MugongList.GetData(dwMugongID, bFELevel);					

		if(pMugongList)
			dwTime = pMugongList->GetInt(9);

		pObject->m_KeepUpMugongList.Add(dwMugongID, bFELevel, dwTime);
		/////////////////////////////////////////////////////////////////////////////////////////////////////
		

		if(bPotionEndKeepup == 0)
		{
			if(pObject->m_pEventItemEffectPP)
			{
				g_EffectManager.DeqEffectPackagePair(pObject->m_pEventItemEffectPP);
				pObject->m_pEventItemEffectPP = NULL;
			}
		}		

		if(bSpirit == 0)
		{
			if(pObject->m_pSpiritEffectPP)
			{
				g_EffectManager.DeqEffectPackagePair(pObject->m_pSpiritEffectPP);
				pObject->m_pSpiritEffectPP = NULL;
			}
		}

		if(bPoisonUnderCover)
			pObject->m_bRenderOK = false;
		else
			pObject->m_bRenderOK = true;
			
	}

	return TRUE;
}

//---------------------------------------------------------------------------------------
int OnCS_IT_IMREADY_ACK( CMsg &msg)
{
	g_MainCharInfo.m_bMainCharDie = FALSE;
	g_MainCharInfo.m_bMainCharMapMoveItemUse = FALSE;

	g_MainCharInfo.m_pHelpMsg->AllDeleteScrMsg();

	return TRUE;
}

/**
 * 캐릭터 퀵 슬롯
 * \param &msg 
 * \return 
 */
int OnCS_IT_CHARSLOT_ACK(CMsg &msg)
{
	// ID 가 20000번 이상이면 물약이나 마약류로 판단 =_=;
	// 그외는 무공아이디로 판단한다. 쩝..
	
	// [5/10/2005] 퀵 슬롯 확장
	DWORD	dwValue	=0;
	//WORD	wVisualID;

	for(int i=0; i < 10; ++i)
	{
		msg
			>> dwValue;
			//>> wVisualID;

		//if( wVisualID)
		//	g_MainCharInfo.m_pSlot->SetSlot( i, (DWORD)wVisualID);
		//else

		g_MainCharInfo.m_pSlot->SetSlot( i, dwValue);
	}

	return TRUE;
}

/**
 * 퀵 슬롯 선택
 * \param &msg 
 * \return 
 */
int OnCS_IT_SETSLOT_ACK(CMsg &msg)
{
	// [5/11/2005] 퀵 슬롯 확장
	BYTE	bResult	=0;
	BYTE	bSlot	=0;
	DWORD	dwValue	=0;
	//WORD	wVisualID;

	msg
		>> bResult
		>> bSlot
		>> dwValue;
		//>> wVisualID;
	
	g_MainCharInfo.m_pHoldItem->SetItemBackToSack();
	
	if( bResult != 0) 
	{
		g_MainCharInfo.m_pSlot->SetSlot( bSlot-1, 0);

		return FALSE;
	}

//	if( wVisualID)
//		g_MainCharInfo.m_pSlot->SetSlot( bSlot-1, (DWORD)wVisualID);
//	else
	g_MainCharInfo.m_pSlot->SetSlot( bSlot-1, dwValue);

	return TRUE;
}

//HO_0404_07 환배 시스템추가
int OnCS_IT_INSTANCE_ACK( CMsg &msg)	
{
	BYTE	bResult		=0;	
	BYTE	bInstanceType = 0;
	DWORD	dwMugongID	=0;
	WORD	wKeepUpTime =0;
	
	msg
		>> bResult
		>> bInstanceType
		>> wKeepUpTime;

	TCHAR strText1[128] = {0,};		
	TCHAR strText2[128] = {0,};		

	switch(bResult)
	{
	case ERR_IT_INSTANCE_FAIL://사용불가
		{			
			switch(bInstanceType)
			{
			case ERR_IT_INSTANCE_RESTRICTION://갑자제한
				{
					g_MainCharInfo.ShowHelpMessage(IDS_IT_INSTANCE_RESTRICTION, TEXTEFFECT_COLOR_WARNING); 
				}
				break;
			case ERR_IT_INSTANCE_ACTIVE://사용중
				{
					g_MainCharInfo.ShowHelpMessage(IDS_IT_INSTANCE_ACTIVE, TEXTEFFECT_COLOR_WARNING); 
				}
				break;
			default:
					break;
			}
		}
		break;

	case ERR_IT_INSTANCE_INSERT://아이콘 추가
		{
			switch(bInstanceType)
			{
				case ERR_IT_INSTANCE_ATTACK:
					{
						dwMugongID = 200;
						_stprintf(strText1, IDS_IT_INSTANCE_START1, IDS_ATTACK_ITEM);
						_stprintf(strText2, IDS_IT_INSTANCE_START2, IDS_STR_PWR);
						g_MainCharInfo.ShowHelpMessage(strText1, TEXTEFFECT_COLOR_GENERAL); 
						g_MainCharInfo.ShowHelpMessage(strText2, TEXTEFFECT_COLOR_GENERAL); 
					}
					break;
				case ERR_IT_INSTANCE_DEFENSE:
					{
						dwMugongID = 201;
						_stprintf(strText1, IDS_IT_INSTANCE_START1, IDS_DEFENSE_ITEM);
						_stprintf(strText2, IDS_IT_INSTANCE_START2, IDS_DEF_PWR);
						g_MainCharInfo.ShowHelpMessage(strText1, TEXTEFFECT_COLOR_GENERAL); 
						g_MainCharInfo.ShowHelpMessage(strText2, TEXTEFFECT_COLOR_GENERAL); 
					}
					break;
				case ERR_IT_INSTANCE_LIFE:
					{						
						dwMugongID = 202;
						_stprintf(strText1, IDS_IT_INSTANCE_START1, IDS_LEFE_ITEM);
						_stprintf(strText2, IDS_IT_INSTANCE_START2, IDS_LIFE_PWR);
						g_MainCharInfo.ShowHelpMessage(strText1, TEXTEFFECT_COLOR_GENERAL); 
						g_MainCharInfo.ShowHelpMessage(strText2, TEXTEFFECT_COLOR_GENERAL); 
					}
					break;
				default:
					break;
			}			
			if(dwMugongID != 0)
			{
				sKEEPUPMUGONGICONLIST *TempList = new sKEEPUPMUGONGICONLIST;
				TempList->m_MugongID = dwMugongID;	//환배 아이디
				TempList->m_CurTime =  g_dwCurTime;
				TempList->m_DrawIcon = true;
				TempList->m_MugongLevel = (wKeepUpTime % 60);		//지속 시간(분)
				g_MainCharInfo.m_vkeepUpMugongIconList.push_back(TempList);		
			}
		}
		break;

	case ERR_IT_INSTANCE_DELETE://아이콘 삭제
		{	
			switch(bInstanceType)
			{
				case ERR_IT_INSTANCE_ATTACK:
					{
						dwMugongID = 200;
						_stprintf(strText1, IDS_IT_INSTANCE_END1, IDS_ATTACK_ITEM);
						g_MainCharInfo.ShowHelpMessage(strText1, TEXTEFFECT_COLOR_GENERAL); 
						g_MainCharInfo.ShowHelpMessage(IDS_IT_INSTANCE_END2, TEXTEFFECT_COLOR_GENERAL); 
					}
					break;
				case ERR_IT_INSTANCE_DEFENSE:
					{
						dwMugongID = 201;
						_stprintf(strText1, IDS_IT_INSTANCE_END1, IDS_DEFENSE_ITEM);
						g_MainCharInfo.ShowHelpMessage(strText1, TEXTEFFECT_COLOR_GENERAL); 
						g_MainCharInfo.ShowHelpMessage(IDS_IT_INSTANCE_END2, TEXTEFFECT_COLOR_GENERAL); 
					}
					break;
				case ERR_IT_INSTANCE_LIFE:
					{
						dwMugongID = 202;
						_stprintf(strText1, IDS_IT_INSTANCE_END1, IDS_LEFE_ITEM);
						g_MainCharInfo.ShowHelpMessage(strText1, TEXTEFFECT_COLOR_GENERAL); 
						g_MainCharInfo.ShowHelpMessage(IDS_IT_INSTANCE_END2, TEXTEFFECT_COLOR_GENERAL); 
					}
					break;
				default:
					break;
			}

			std::vector<sKEEPUPMUGONGICONLIST*>::iterator iter = g_MainCharInfo.m_vkeepUpMugongIconList.begin();

			for(; iter != g_MainCharInfo.m_vkeepUpMugongIconList.end(); ++iter)
			{
				sKEEPUPMUGONGICONLIST *psKeepUpMugong = (*iter);
				if(psKeepUpMugong->m_MugongID == dwMugongID)
				{
					g_MainCharInfo.m_vkeepUpMugongIconList.erase(iter);
					break;
				}
			}	
		}
		break;
	}
	return TRUE;
}

/**
* 비밀번호 변경
* \param &msg 
* \return 
*/
int OnCS_IT_CHANGEPW_ACK(CMsg &msg)
{
	BYTE bResult =0;		

	msg
		>> bResult;

	switch(bResult)
	{
	case ERR_ITCHANGEPW_SUCCESS:
		{
			g_pUIManager->Show(LOGIN_1, login1_ok);
			g_pUIManager->Show(LOGIN_1, login1_exit);
			g_pUIManager->Show(LOGIN_1, login1_pw_change_button);
			g_pUIManager->Show(LOGIN_1, login1_edit_id);
			g_pUIManager->Show(LOGIN_1, login1_edit_password);
			g_pUIManager->Show(LOGIN_1, login1_id_dummy);
			g_pUIManager->Show(LOGIN_1, login1_pw_dummy);

			g_pUIManager->Hide(LOGIN_1, login1_pw_change_back);
			g_pUIManager->Hide(LOGIN_1, login1_pw_change_button_01);
			g_pUIManager->Hide(LOGIN_1, login1_pw_change_button_02);

			g_pUIManager->Hide(LOGIN_1, login1_pw_change_dummy_01);
			g_pUIManager->Hide(LOGIN_1, login1_pw_change_dummy_02);
			g_pUIManager->Hide(LOGIN_1, login1_pw_change_dummy_03);
			g_pUIManager->Hide(LOGIN_1, login1_pw_change_dummy_04);

			g_pUIManager->Hide(LOGIN_1, login1_pw_change_edit_01);
			g_pUIManager->Hide(LOGIN_1, login1_pw_change_edit_02);
			g_pUIManager->Hide(LOGIN_1, login1_pw_change_edit_03);
			g_pUIManager->Hide(LOGIN_1, login1_pw_change_edit_04);

			g_pUIManager->SetString(LOGIN_1, login1_pw_change_edit_01, _T(""));
			g_pUIManager->SetString(LOGIN_1, login1_pw_change_edit_02, _T(""));
			g_pUIManager->SetString(LOGIN_1, login1_pw_change_edit_03, _T(""));
			g_pUIManager->SetString(LOGIN_1, login1_pw_change_edit_04, _T(""));

			g_pUIManager->SetString(LOGIN_1, login1_edit_id, _T(""));
			g_pUIManager->SetString(LOGIN_1, login1_edit_password, _T(""));
		}
		break;
	case ERR_ITCHANGEPW_WRONGPW:	// 비밀번호 틀림
		{
			g_pUIManager->ShowNotice(IDS_LOGIN_WRONGPASSWORD);
		}
		break;
	case ERR_ITCHANGEPW_SAME:		// 기존 비번과 같음
		{
			g_pUIManager->ShowNotice(IDS_PW_CHANGE_04);
		}
		break;
	case ERR_ITCHANGEPW_FAIL:
		{
			g_pUIManager->ShowNotice(IDS_INTERNAL_ERROR);			
		}
		break;	
	}

	g_pUIManager->SetData(LOGIN_1, login1_pw_change_button_01, CURRENT_INDEX, -1);

	return 1;
}
