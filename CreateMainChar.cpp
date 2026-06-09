BOOL CreateMainChar()
{
	CXiahCharObject *pObject = new CXiahCharObject;

	if(pObject == NULL)
	{
		DBG_LogFile( _T("CreateMainChar 1 실패"));
		//return false;
	}

	sArrayData *pData = XiahArrayIndex::g_MainCharType.GetData( g_MainCharInfo.m_bCharType, 0);

	if(pData == NULL)
	{
		DBG_LogFile( _T("CreateMainChar 2 실패"));
		//return false;
	}

	pObject->m_pAniType = XiahAniType::GetAniType( OBJTYPE_PC, 0);

	int nCharID = pData->GetInt( 2);
	int nMeshType = pData->GetInt( 3);
	int nTextureType = pData->GetInt( 4);

	pObject->Create( nCharID, nMeshType, nTextureType, 103);

	g_pMainChar = XiahObject::g_XiahObjectManager.CreateXiahObject( g_MainCharInfo.m_dwObjectID, OBJTYPE_PC, pObject);

	pObject->SetPosition( g_MainCharInfo.m_fPosX, g_MainCharInfo.m_fPosY);
	pObject->m_szObjectName = g_MainCharInfo.m_szNickName;
	pObject->m_bObjType = OBJTYPE_PC;
	pObject->m_bRebirth = g_MainCharInfo.m_bRebirth;

	//HT_1023 : 운영자 마크 추가
	pObject->m_bGameMasterMark = g_MainCharInfo.m_dwGameMasterMark;

	// 명성치에 따른 색 변경
	pObject->RefreshFameColor(g_MainCharInfo.m_dwFame);

	pObject->m_bSubObjType = g_MainCharInfo.m_bCharType;

	// PC만 칼 궤적 이펙트를 생성한다.
	pObject->m_SwordTrace.Init();

	// 야차는 왼손 칼 궤적도 있다.
	if( pObject->m_bSubObjType == 4 )
		pObject->m_SwordTrace2.Init();
	
	pObject->SetAnimation( XiahAniType::eLAT_Spawn, XiahAniType::eLAT_Stand, 0, 0);
	pObject->m_CollisionEnable = TRUE;
	
	pObject->m_pParentTrigger[ eXCT_OnCollision] = g_StaticTriggerList[ 0];
	pObject->m_pParentTrigger[ eXCT_OnTimer]	 = g_StaticTriggerList[ 1];
	pObject->m_pParentTrigger[ eXCT_OnEndTargetMove] = g_StaticTriggerList[ 2];


	if( g_MainCharInfo.m_pRelation)
	{
		pObject->m_dwMunpaID = g_MainCharInfo.m_pRelation->m_dwClanID;
		pObject->m_szMunpaName = g_MainCharInfo.m_pRelation->m_szClanName;

		sClanWonInfo* pInfo = g_MainCharInfo.m_pRelation->FindClanInfoByID( g_MainCharInfo.m_dwObjectID);
		if( pInfo)
			pObject->m_szMunpaNickName = pInfo->m_szMunpaNickName;

		SendCS_RL_MUNPAINFO_REQ(0, pObject->m_dwMunpaID);
	}

	SetupPC_VisualEquipement( pObject, g_MainCharInfo.wEquipVisualID, g_MainCharInfo.m_bRarity, g_MainCharInfo.m_bStxType );

	sPCVisualInfo* pVisualInfo = new sPCVisualInfo;

	if(pVisualInfo == NULL)
	{
		DBG_LogFile( _T("CreateMainChar 4 실패"));
//		return false;
	}

	memcpy( pVisualInfo->wVisualID, g_MainCharInfo.wEquipVisualID, sizeof( WORD) * 9);
	memcpy( pVisualInfo->bRarity, g_MainCharInfo.m_bRarity, sizeof( BYTE) * 9);
	memcpy( pVisualInfo->bStxType, g_MainCharInfo.m_bStxType, sizeof( BYTE) * 9);

	pObject->m_pPrivateData = (DWORD)pVisualInfo;
	pObject->m_PrivateDataDestoryer = ReleasePCVisualnfo;

	g_XiahCamera.m_bFollowMainChar = TRUE;
	g_XiahCamera.m_bNeedUpdate = TRUE;
	g_XiahCamera.Update();

	XiahMap::g_XiahMap.Update();

	// 가라로 애완동물 만들어 주기

	//SpawnTestPet();

	return TRUE;
}