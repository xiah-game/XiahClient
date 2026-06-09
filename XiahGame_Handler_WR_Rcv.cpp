
extern LPCTSTR GetMapSmallName(DWORD dwMapID);

/**
 * 문파 비석 리스트
 * \param &msg 
 * \return 
 */
int OnCS_WR_MUNPASTONELIST_ACK(CMsg &msg)
{
	BYTE bResult = 0;
	DWORD dwMapID = 0;
	WORD wCount = 0;

	msg
		>> bResult
		>> dwMapID
		>> wCount;

	g_MainCharInfo.m_MunpaStonIDList.clear();

	for(WORD i=0; i < wCount; ++i)
	{
		DWORD dwStoneID =0;
		BYTE bHeight	=0;
		BYTE bWar		=0;
		BYTE bMainStone =0;
		BYTE bType		=0;
		BYTE bKind		=0;
		WORD wPosX		=0;
		WORD wPosY		=0;
		WORD wDirection =0;		
		DWORD dwMaxHP		=0;
		DWORD dwCurHP		=0;
		DWORD dwOwnerMunpaID =0;
		DWORD dwEnemyMunpaID =0;
		sString strName;

		msg
			>> dwStoneID
			>> strName
			>> bMainStone
			>> bType
			>> bKind
			>> wPosX
			>> wPosY
			>> bHeight
			>> wDirection
			>> dwMaxHP
			>> dwCurHP
			>> dwOwnerMunpaID
			>> bWar
			>> dwEnemyMunpaID;

		g_MainCharInfo.m_MunpaStonIDList.push_back( dwStoneID );

		// 비석 만들기
		/////////////////////////////////////////////////////////////////////////////////////////////////////
		int nMeshType = 0;

		XiahObject::CXiahObject* pFindObject = XiahObject::g_XiahObjectManager.FindXiahObject(MAKEOBJECTID( 0, dwStoneID, OBJTYPE_FUNCTIONALNPC));
		if(pFindObject != NULL)
		{
			if(pFindObject->m_pObject == NULL)
				continue;

			CXiahCharObject* pCharObject = reinterpret_cast<CXiahCharObject*>(pFindObject->m_pObject);

			// 문파전이 아닌 상태일떄.
			if( bWar == 0 )
			{
				sArrayData *pData = XiahArrayIndex::g_FunctionalNpcType.GetData(bType);

				if(pData == NULL)
					continue;

				int nCharID = pData->GetInt(1);
				if(XiahGameEngine::GetCharacter(nCharID) == NULL)
					continue;

				CRes_Character* pResChar = XiahGameEngine::GetCharacter( nCharID);

				nMeshType = 0;

				if( pResChar == NULL) continue;
				if( !pResChar->GetMesh( nMeshType )) continue;
				if( pResChar->GetMesh( nMeshType )->GetTexture( 0 ) == NULL) continue;

				pCharObject->Create( nCharID, nMeshType, 0, -1);
				pCharObject->m_pAniType = NULL;

				pCharObject->SetAngle(wDirection);
				pCharObject->SetPosition(wPosX, wPosY);

				pCharObject->m_szObjectName = strName;
				pCharObject->m_bObjType = OBJTYPE_FUNCTIONALNPC;
				pCharObject->m_bSubObjType = bType;
				pCharObject->m_dwCurHP = dwCurHP;
				pCharObject->m_dwMaxHP = dwMaxHP;

				sFunctionalNpcInfo* pInfo = (sFunctionalNpcInfo*)pCharObject->m_pPrivateData;

				if(pInfo)
				{
					pInfo->m_dwObjectID		= dwStoneID;
					pInfo->m_bType			= bType;
					pInfo->m_bKind			= bKind;
					pInfo->m_szName			= strName;
					pInfo->m_dwOwnID		= dwOwnerMunpaID;	// 문파 ID
					pInfo->m_bMainStone		= bMainStone;
					pInfo->m_bWar			= bWar;
					pInfo->m_dwEnemyMunpaID = dwEnemyMunpaID;
				}				
			}
			else	// 문파전일때.
			{
				// 현재 메쉬 상태
				sArrayData *pData = XiahArrayIndex::g_FunctionalNpcType.GetData(bType);

				if(pData == NULL) continue;

				if( dwCurHP == 0 )
					nMeshType = 2;
				else if( dwMaxHP/2 >= dwCurHP )
					nMeshType = 1;

				int nCharID = pData->GetInt(1);
				if(XiahGameEngine::GetCharacter(nCharID) == NULL) continue;

				CRes_Character* pResChar = XiahGameEngine::GetCharacter( nCharID);

				if( pResChar == NULL) continue;
				if( !pResChar->GetMesh( nMeshType )) continue;
				if( pResChar->GetMesh( nMeshType )->GetTexture( 0 ) == NULL) continue;

				pCharObject->Create( nCharID, nMeshType, 0, -1);
				pCharObject->m_pAniType = NULL;

				pCharObject->SetAngle(wDirection);
				pCharObject->SetPosition(wPosX, wPosY);

				pCharObject->m_szObjectName = strName;
				pCharObject->m_bObjType		= OBJTYPE_FUNCTIONALNPC;
				pCharObject->m_bSubObjType  = bType;
				pCharObject->m_dwCurHP = dwCurHP;
				pCharObject->m_dwMaxHP = dwMaxHP;

				sFunctionalNpcInfo* pInfo = (sFunctionalNpcInfo*)pCharObject->m_pPrivateData;

				if(pInfo)
				{
					pInfo->m_dwObjectID = dwStoneID;
					pInfo->m_bType		= bType;
					pInfo->m_bKind		= bKind;
					pInfo->m_szName		= strName;
					pInfo->m_dwOwnID	= dwOwnerMunpaID;	// 문파 ID
					pInfo->m_bMainStone = bMainStone;
					pInfo->m_bWar		= bWar;
					pInfo->m_dwEnemyMunpaID = dwEnemyMunpaID;
				}
			}

			// Effect
			// 캐릭터 툴에 이펙트가 등록되어 있는데, 등록 순서가 중요하다.
			// 첫번째가 Normal, 두번째가 확정, 세번째가 진행1이다.
			// 또한 공용 비석은 Normal이 없다.
			if( nMeshType != 2 )
			{
				switch( bWar )
				{
				case 0:	// Normal
					{
						if( bType >= 12 && bType <= 14 )	// 고유
							pCharObject->m_CharRender.MakeMeshEffect();	// 첫번째 이펙트
					}
					break;
				case 1:	// 확정
					{
						if( bType >= 12 && bType <= 14 )	// 고유
							pCharObject->m_CharRender.MakeMeshEffect(1);
						else	// 공용
							pCharObject->m_CharRender.MakeMeshEffect();
					}
					break;
				case 2:	// 진행1
					{
						if( bType >= 12 && bType <= 14 )
						{
							if( nMeshType == 0 )
								pCharObject->m_CharRender.MakeMeshEffect(2);
							else if( nMeshType == 1 )
								pCharObject->m_CharRender.MakeMeshEffect();
						}
						else
						{
							if( nMeshType == 0 )
								pCharObject->m_CharRender.MakeMeshEffect(1);
							else if( nMeshType == 1 )
								pCharObject->m_CharRender.MakeMeshEffect();
						}
					}
					break;
				};
			}			

			continue;
		} // if(XiahObject::g_XiahObjectManager.FindXiahObject(MAKEOBJECTID( 0, dwStoneID, OBJTYPE_FUNCTIONALNPC)) != NULL)
        
		sArrayData *pData = XiahArrayIndex::g_FunctionalNpcType.GetData(bType);

		if(pData == NULL)
		{
			DBG_Put(_T("넌 누구냐?"));
			continue;
		}

		int nCharID = pData->GetInt(1);
		if(XiahGameEngine::GetCharacter(nCharID) == NULL)
		{
			DBG_Put(_T("푸헐 더 우끼는 넘이네"));
			continue;
		}

		CXiahCharObject *pObject = new CXiahCharObject;
		if(pObject == NULL) continue;

		nMeshType = 0;

		if( dwCurHP == 0 )
			nMeshType = 2;
		else if( dwMaxHP/2 >= dwCurHP )
			nMeshType = 1;

		XiahObject::g_XiahObjectManager.CreateXiahObject(dwStoneID, OBJTYPE_FUNCTIONALNPC, pObject);

		pObject->Create(nCharID, nMeshType, 0, -1);

		pObject->m_pAniType = XiahAniType::GetAniType(OBJTYPE_FUNCTIONALNPC, 0);

		pObject->m_pAniType = NULL;

		pObject->SetAngle(wDirection);
		pObject->SetPosition(wPosX, wPosY);
		pObject->m_bRotatable = FALSE;
		
		pObject->SetAnimation( XiahAniType::eLAT_Stand, -1);

		pObject->m_szObjectName = strName;
		pObject->m_bObjType = OBJTYPE_FUNCTIONALNPC;
		pObject->m_bSubObjType = bType;

		pObject->m_dwCurHP = dwCurHP;
		pObject->m_dwMaxHP = dwMaxHP;

		sFunctionalNpcInfo* pInfo = new sFunctionalNpcInfo;
		pInfo->m_dwObjectID = dwStoneID;
		pInfo->m_bType		= bType;
		pInfo->m_bKind		= bKind;
		pInfo->m_szName		= strName;
 		pInfo->m_dwOwnID	= dwOwnerMunpaID;	// 문파 ID
		pInfo->m_bMainStone = bMainStone;
		pInfo->m_bWar		= bWar;
		pInfo->m_dwEnemyMunpaID = dwEnemyMunpaID;

		pObject->m_pPrivateData = (DWORD)pInfo;
		pObject->m_PrivateDataDestoryer = ReleaseFunctionalNpcInfo;

		/////////////////////////////////////////////////////////////////////////////////////////////////////

		// Effect
		// 캐릭터 툴에 이펙트가 등록되어 있는데, 등록 순서가 중요하다.
		// 첫번째가 Normal, 두번째가 확정, 세번째가 진행1이다.
		// 또한 공용 비석은 Normal이 없다.
		if( nMeshType != 2 )
		{
			switch( bWar )
			{
			case 0:	// Normal
				{
					if( bType >= 12 && bType <= 14 )	// 고유
						pObject->m_CharRender.MakeMeshEffect();	// 첫번째 이펙트
				}
				break;
			case 1:	// 확정
				{
					if( bType >= 12 && bType <= 14 )	// 고유
						pObject->m_CharRender.MakeMeshEffect(1);
					else	// 공용
						pObject->m_CharRender.MakeMeshEffect();
				}
				break;
			case 2:	// 진행1
				{
					if( bType >= 12 && bType <= 14 )
					{
						if( nMeshType == 0 )
							pObject->m_CharRender.MakeMeshEffect(2);
						else if( nMeshType == 1 )
							pObject->m_CharRender.MakeMeshEffect();
					}
					else
					{
						if( nMeshType == 0 )
							pObject->m_CharRender.MakeMeshEffect(1);
						else if( nMeshType == 1 )
							pObject->m_CharRender.MakeMeshEffect();
					}
				}
				break;
			};
		}		
	}

	return 0;
}

/**
 * 문파 비석 갱신
 * \param &msg 
 * \return 
 */
int OnCS_WR_STONESTATUSCHANGE_ACK(CMsg &msg)
{
	// 문파 비석이 없어질때 받는다.
	DWORD dwStoneID = 0;
	BYTE bStatus = 0;
	WORD wMaxHP = 0;
	WORD wCurHP = 0;

	msg
		>> dwStoneID
		>> bStatus
		>> wMaxHP
		>> wCurHP;

	XiahObject::CXiahObject* pObject = XiahObject::g_XiahObjectManager.FindXiahObject(MAKEOBJECTID(0, dwStoneID, OBJTYPE_FUNCTIONALNPC));

	if(pObject == NULL)
		return 0;

	if(pObject->m_pObject == NULL)
		return 0;

	CXiahCharObject* pCharObject = reinterpret_cast<CXiahCharObject*>(pObject->m_pObject);

	pCharObject->m_dwCurHP = wCurHP;
	pCharObject->m_dwMaxHP = wMaxHP;

	// Destroy
	if( wCurHP == 0 )
	{
		sFunctionalNpcInfo* pInfo = (sFunctionalNpcInfo*)pCharObject->m_pPrivateData;

		if ( !pInfo )
			return 0;

		BYTE bInfoType = pInfo->m_bType;

		sArrayData *pData = XiahArrayIndex::g_FunctionalNpcType.GetData(bInfoType);
		if(pData == NULL)
			return TRUE;

		int nCharID = pData->GetInt(1);
		if(XiahGameEngine::GetCharacter(nCharID) == NULL)
			return TRUE;

		CRes_Character* pResChar = XiahGameEngine::GetCharacter( nCharID);

		if( pResChar == NULL) return TRUE;
		if( !pResChar->GetMesh( 2 )) return TRUE;
		if( pResChar->GetMesh( 2 )->GetTexture( 0 ) == NULL) return TRUE;

		// 두번째 메시로 바꿔준다. 그 전에 필요한 데이타 백업.
		float fAngle = pCharObject->m_Angle;
		float fX =  pCharObject->m_Position.x;
		float fZ = -pCharObject->m_Position.z;
		
		// 두번째 메시로 바꿔준다.
		pCharObject->Create( nCharID, 2, 0, -1);
		pCharObject->m_pAniType = NULL;

		pCharObject->SetAngle(fAngle);
		pCharObject->SetPosition(fX, fZ);

		pCharObject->m_bRotatable = FALSE;

		// Effect
		pCharObject->m_CharRender.MakeMeshEffect();
	}

	return 0;
}

/**
 * 문파 전쟁 신청창의 정보
 * \param &msg 
 * \return 
 */
int OnCS_WR_PRECHALLENGEWAR_ACK(CMsg &msg)
{
	BYTE bResult = 0;

	msg
		>> bResult;

	switch(bResult)
	{
	case 0:	// 성공
		{
			std::map<int, sMunpaWarDay*>::iterator iter = g_MainCharInfo.m_pRelation->m_mMunpaWarDayList.begin();

			for(; iter != g_MainCharInfo.m_pRelation->m_mMunpaWarDayList.end(); ++iter)
			{
				sMunpaWarDay *pInfo = iter->second;
				delete pInfo, pInfo = NULL;
			} // for(; iter != g_MainCharInfo.m_pRelation->m_mMunpaWarDayList.end(); ++iter)
			g_MainCharInfo.m_pRelation->m_mMunpaWarDayList.clear();

			DWORD dwDamageMoney = 0;
			WORD wChallengeTimeNum = 0;

			msg
				>> g_MainCharInfo.m_pRelation->m_dwMunpaWarEnemy
				>> dwDamageMoney
				>> wChallengeTimeNum;

			TCHAR strTemp[128]= {0,};
			_stprintf(strTemp, IDS_MONEY, MoneyCommaStr(dwDamageMoney).data());
			g_pUIManager->SetString(WINDOW_MUNPA_WAR_PETITION, munpa_war_petition_dummy_05, strTemp, 5);

			for(WORD i = 0; i < wChallengeTimeNum; ++i)
			{
				sMunpaWarDay *pData = new sMunpaWarDay;

				msg
					>> pData->dwGameTime
					>> pData->dwRealTime;			

				g_MainCharInfo.m_pRelation->m_mMunpaWarDayList.insert(std::map<int, sMunpaWarDay*>::value_type(i, pData));
			} // for(WORD i = 0; i < wChallengeTimeNum; ++i)

			g_MainCharInfo.OpenFrame(WINDOW_MUNPA_WAR_PETITION);
		}
		break;
	case 1:	// 이미 전쟁중
		g_MainCharInfo.ShowHelpMessage(IDS_MUNPA_WAR_ALREADY, TEXTEFFECT_COLOR_WARNING);
		break;
	case 2:	// 명성이 낮아서 전쟁 불가
		g_MainCharInfo.ShowHelpMessage(IDS_MUNPA_FAME_LOW, TEXTEFFECT_COLOR_WARNING);		
		break;
	case 3:	// 시스템 에러
		g_MainCharInfo.ShowHelpMessage(IDS_INTERNAL_ERROR);
		break;
	case 4:	// 전쟁 유예기간
		g_MainCharInfo.ShowHelpMessage(IDS_M_WAR_TERMOFVALIDITY);
		break;
	case 5:	// 문주만 가능
		g_MainCharInfo.ShowHelpMessage(IDS_ONLY_MUNJU, TEXTEFFECT_COLOR_WARNING);
		break;
	case 6:	// 돈 없음
		g_MainCharInfo.ShowHelpMessage(IDS_PURSE_IN_LOWMONEY, TEXTEFFECT_COLOR_WARNING);
		break;
	case 7:	// 해당 채널
		g_MainCharInfo.ShowHelpMessage(IDS_ONLY_CHANNEL, TEXTEFFECT_COLOR_WARNING);
		break;
	case 8:	// 다른 문파만 신청가능
		g_MainCharInfo.ShowHelpMessage(IDS_E_M_WAR, TEXTEFFECT_COLOR_WARNING);
		break;
	default:
		break;
	}

	return 0;
}

/**
 * 문파 전쟁 신청 결과
 * \param &msg 
 * \return 
 */
int OnCS_WR_CHALLENGEWAR_ACK(CMsg &msg)
{
	BYTE bResult = 0;

	msg
		>> bResult;

	switch(bResult)
	{
	case 0:	// 성공
		g_MainCharInfo.ShowHelpMessage(IDS_SEND_BATTLE);
		break;
	case 1:	// 시스템 에러
		g_MainCharInfo.ShowHelpMessage(IDS_INTERNAL_ERROR);
		break;

	default:
		break;
	}

	return 0;
}

/**
 * 문파 전쟁 상태
 * \param &msg 
 * \return 
 */
int OnCS_WR_WARSTATUS_ACK(CMsg &msg)
{
	BYTE bStatus	= 0;
	BYTE bWarResult = 0;
	BYTE bChannel	= 0;
	DWORD dwBattleTime = 0;	
	DWORD dwStone[2];
	sString strChallengeMunpa;
	sString strMunpaName;

	//UnitSvr -> Client
	//bStatus - 0 : 선포, 1: 시작, 2 : 종료 
	//bWarResult : if ( bStatus == 2 ) 0 - Draw, 1 - ChallengeMunpa Win, 2 - ChallengeMunpa Loss
	//strChallengeMunpaName,
	//strMunpaName,

	msg
		>> bStatus
		>> bWarResult
		>> dwBattleTime
		>> strChallengeMunpa
		>> strMunpaName
		>> bChannel
		>> dwStone[0]
		>> dwStone[1];

	switch(bStatus)
	{
	case 0:	// 선포
		{
			TCHAR strTemp[256] = {0,};
			_stprintf(strTemp, IDS_MUNPA_WAR, GETMONTH(dwBattleTime)+1, GETDAY(dwBattleTime)+1, GETHOUR(dwBattleTime), (LPCTSTR)strChallengeMunpa, (LPCTSTR)strMunpaName);
			g_MainCharInfo.SpecialChatMessage(strTemp, 0);
		}
		break;

	case 1:	// 시작
		{
			LPCTSTR lpStrTemp[2];

			for(register int i = 0; i < 2; ++i)
			{
				lpStrTemp[i] = GetMapSmallName(dwStone[i]);
			}

			TCHAR strTemp[256] = {0,};
			_stprintf(strTemp, IDS_MUNPA_WAR_START_2, bChannel, (LPCTSTR)strChallengeMunpa, lpStrTemp[0], (LPCTSTR)strMunpaName, lpStrTemp[1]);
			g_MainCharInfo.SpecialChatMessage(strTemp, 0);
		}
		break;

	case 2:	// 종료
		{
			TCHAR strTemp[256] = {0,};
			_stprintf(strTemp, IDS_MUNPA_WAR_END, (LPCTSTR)strChallengeMunpa, (LPCTSTR)strMunpaName);
			g_MainCharInfo.SpecialChatMessage(strTemp, 0);

			switch(bWarResult)
			{
			case 0:	// 무승부
				g_MainCharInfo.SpecialChatMessage(IDS_MUNPA_WAR_DRAW, 0);
				break;

			case 1:	// 도전문파 승리
				_stprintf(strTemp, IDS_MUNPA_WAR_WIN, (LPCTSTR)strChallengeMunpa);
				g_MainCharInfo.SpecialChatMessage(strTemp, 0);
				break;

			case 2:	// 도전문파 패배 
				_stprintf(strTemp, IDS_MUNPA_WAR_WIN, (LPCTSTR)strChallengeMunpa);
				g_MainCharInfo.SpecialChatMessage(strTemp, 0);
				break;

			default:
				break;
			}
		}
		break;
	case 3:	// 대기
		{
			LPCTSTR lpStrTemp[2];

			for(register int i = 0; i < 2; ++i)
			{
				lpStrTemp[i] = GetMapSmallName(dwStone[i]);
			}

			TCHAR strTemp[256] = {0,};
			_stprintf(strTemp, IDS_MUNPA_WAR_WAIT_2, dwBattleTime, bChannel, (LPCTSTR)strChallengeMunpa, lpStrTemp[0], (LPCTSTR)strMunpaName, lpStrTemp[1]);
			g_MainCharInfo.SpecialChatMessage(strTemp, 0);
		}
		break;

	default:
		break;
	} // switch(bStatus)

	return 0;
}

/**
 * 문파대전 참가 신청
 * \param &msg 
 * \return 
 */
int OnCS_WR_APPLYWAR_ACK(CMsg &msg)
{
	BYTE bResult = 0;

	msg
		>> bResult;

	switch(bResult)
	{
	case ERR_APPLYWAR_SUCCESS:			// 성공
		{
			g_MainCharInfo.ShowHelpMessage(IDS_APPLYWAR_SUCCESS);
		}
		break;
	case ERR_APPLYWAR_WRONGTERM:		// 신청기간아님
		{
			g_MainCharInfo.ShowHelpMessage(IDS_APPLYWAR_WRONGTERM, TEXTEFFECT_COLOR_WARNING);
		}
		break;
	case ERR_APPLYWAR_NOTMUNJU:			// 문주만 신청 가능 (문주아님)
		{
			g_MainCharInfo.ShowHelpMessage(IDS_ONLY_MUNJU, TEXTEFFECT_COLOR_WARNING);
		}
		break;
	case ERR_APPLYWAR_NOTENOUGHMONEY:	// 신청 금액 부족
		{
			g_MainCharInfo.ShowHelpMessage(IDS_APPLYWAR_NOTENOUGHMONEY, TEXTEFFECT_COLOR_WARNING);
		}
		break;
	case ERR_APPLYWAR_NOTENOUGHMUNWON:	// 문파원 부족
		{
			g_MainCharInfo.ShowHelpMessage(IDS_APPLYWAR_NOTENOUGHMUNWON, TEXTEFFECT_COLOR_WARNING);
		}
		break;
	case ERR_APPLYWAR_ALREADY:			// 이미 신청 했음
		{
			g_MainCharInfo.ShowHelpMessage(IDS_APPLYWAR_ALREADY, TEXTEFFECT_COLOR_WARNING);
		}
		break;
	case ERR_APPLYWAR_EXCESSMUNPA:		// 신청 문파수 초과 (50개 문파만 가능)
		{
			g_MainCharInfo.ShowHelpMessage(IDS_APPLYWAR_EXCESSMUNPA, TEXTEFFECT_COLOR_WARNING);
		}
		break;
	case ERR_APPLYWAR_STRAIGHTWIN:		//HT_0911 : 지문전 개선사항 (3연속우승으로 신청 불가)
		{
			g_MainCharInfo.ShowHelpMessage(IDS_APPLYWAR_STRAIGHTWIN, TEXTEFFECT_COLOR_WARNING);
		}
	break;

	default:
		break;
	}

	return 0;
}


/**
 * 전쟁 상태
 * \param &msg 
 * \return 
 */
int OnCS_WR_WORLDWARMESSAGE_ACK(CMsg &msg)
{
	BYTE bType = 0;
	
	msg
		>> bType;

	switch(bType)
	{
	case WWM_PREPARESTART:
		{
			g_MainCharInfo.SpecialChatMessage(IDS_PREPARESTART, 0);			
		}
		break;
	case WWM_PREPAREEND:
		{
			g_MainCharInfo.SpecialChatMessage(IDS_PREPAREEND_01, 0);			
			g_MainCharInfo.SpecialChatMessage(IDS_PREPAREEND_02, 0);			
			g_MainCharInfo.SpecialChatMessage(IDS_PREPAREEND_03, 0);			
		}
		break;
	case WWM_READY:
		{
			g_MainCharInfo.SpecialChatMessage(IDS_READY, 0);			
		}
		break;
	case WWM_WARSTART:
		{
			// 2채널 에서만
			if(g_AppData.m_byChannelID == 2)
			{
				// 문파대전 시작
				g_MainCharInfo.m_bMunpaFight = true;			    
			}			

			g_MainCharInfo.SpecialChatMessage(IDS_WARSTART, 0);			
		}
		break;
	case WWM_WAREND_READY:
		{
			g_MainCharInfo.SpecialChatMessage(IDS_WAREND_READY, 2);			
		}
		break;
	case WWM_WAREND:
		{
			// 문파대전 끝
			g_MainCharInfo.m_bMunpaFight = false;

			sString strLordMunpaName;

			msg
                >> strLordMunpaName;

			if(!strlen(strLordMunpaName.data()))
			{
				g_MainCharInfo.SpecialChatMessage(IDS_WARENDNOTHING, 0);
			}			
			else
			{
				TCHAR strTemp[256] = {0,};
				_stprintf(strTemp, IDS_WAREND, (LPCTSTR)strLordMunpaName);
				g_MainCharInfo.SpecialChatMessage(strTemp, 0);
			}	
		}
		break;
	case WWM_APLLYWAR:
		{
			sString strApplyMunpaName;

			msg
				>> strApplyMunpaName;

			TCHAR strTemp[256] = {0,};
			_stprintf(strTemp, IDS_APPLYWAR, (LPCTSTR)strApplyMunpaName);
			g_MainCharInfo.SpecialChatMessage(strTemp, 0);
		}
		break;
	case WWM_WARTIME:
		{
			// 2채널 에서만
			if(g_AppData.m_byChannelID == 2)
			{
				// 문파대전 시작
				g_MainCharInfo.m_bMunpaFight = true;			    
			}

			g_MainCharInfo.SpecialChatMessage(IDS_WARTIME, 0);
		}
		break;
	case WWM_STONECOMING:		//HT_0911 : 지문전 개선사항 ( 극진 출몰 )
		{
			g_MainCharInfo.SpecialChatMessage(IDS_WORLDWAR_COMING, 0);
		}
		break;

	default:
		break;
	}

	return 0;
}

/**
 * 전쟁 점수
 * \param &msg 
 * \return 
 */
int OnCS_WR_WORLDWARPOINT_ACK(CMsg &msg)
{
	//HT_0911 : 지문전 개선사항

	DWORD dwMyPoint		= 0;
	DWORD dwFirstPoint	= 0;
	DWORD dwSecondPoint = 0;
	DWORD dwThirdPoint	= 0;
	WORD wMin = 0;
    sString strFirstMunpaName;
	sString strSecondMunpaName;
	sString strThirdMunpaName;

	msg
        >> dwMyPoint
		>> dwFirstPoint
		>> strFirstMunpaName	//1등
		>> dwSecondPoint
		>> strSecondMunpaName	//2등
		>> dwThirdPoint
		>> strThirdMunpaName	//3등
		>> wMin;
				
	TCHAR strTemp[256] = {0,};
	TCHAR strTemp1[256] = {0,};

	if(strlen(strThirdMunpaName) == 0 && strlen(strSecondMunpaName) == 0)
	{
		_stprintf(strTemp, IDS_WORLDWARPOINT_00, wMin,(LPCTSTR)g_MainCharInfo.m_szMunpaName, dwMyPoint);
		_stprintf(strTemp1, IDS_WORLDWARPOINT_03, strFirstMunpaName.data(), dwFirstPoint);
		g_MainCharInfo.SpecialChatMessage(strTemp, 0);
		g_MainCharInfo.SpecialChatMessage(strTemp1, 0);
	}
	else if(strlen(strThirdMunpaName) == 0 && strlen(strSecondMunpaName) != 0)
	{
		_stprintf(strTemp, IDS_WORLDWARPOINT_00, wMin,(LPCTSTR)g_MainCharInfo.m_szMunpaName, dwMyPoint);
		_stprintf(strTemp1, IDS_WORLDWARPOINT_02, strFirstMunpaName.data(), dwFirstPoint, strSecondMunpaName.data(), dwSecondPoint);
		g_MainCharInfo.SpecialChatMessage(strTemp, 0);
		g_MainCharInfo.SpecialChatMessage(strTemp1, 0);
	}
	else
	{
		_stprintf(strTemp, IDS_WORLDWARPOINT_00, wMin,(LPCTSTR)g_MainCharInfo.m_szMunpaName, dwMyPoint);
		_stprintf(strTemp1, IDS_WORLDWARPOINT_01, strFirstMunpaName.data(), dwFirstPoint, strSecondMunpaName.data(), dwSecondPoint, 
			strThirdMunpaName.data(), dwThirdPoint);
		g_MainCharInfo.SpecialChatMessage(strTemp, 0);
		g_MainCharInfo.SpecialChatMessage(strTemp1, 0);
	}

	return 0;
}

/**
 * 우승 문파명
 * \param &msg 
 * \return 
 */
int OnCS_WR_LORDMUNPA_ACK(CMsg &msg)
{
	msg
		>> g_MainCharInfo.m_dwLordMunpaID;	//우승 문파 아이디를 저장.
	
	return 0;
}

/**
 * 문파전 보상
 * \param &msg 
 * \return 
 */
int OnCS_WR_REWARD_ACK(CMsg &msg)
{
	BYTE bResult = 0;
	DWORD dwRewardMoney = 0;	

	msg
		>> bResult
		>> dwRewardMoney;

	switch(bResult)
	{
	case ERR_REWARD_SUCCESS:
		{
			g_MainCharInfo.ShowHelpMessage(IDS_WORLDWARREWARD_RESULT_00);	//상금을 수령하였습니다.
		}
		break;
	case ERR_REWARD_NOPRIZE:
		{
			g_pUIManager->ShowNotice(IDS_WORLDWARREWARD_RESULT_01);			//문파대전 종료시 상금 지급
		}
		break;
	case ERR_REWARD_NOTHAVEAUTHORITY:
		{
			g_MainCharInfo.ShowHelpMessage(IDS_WORLDWARREWARD_RESULT_02, TEXTEFFECT_COLOR_WARNING);	//우승 문파의 문주만 수령 가능
		}
		break;
	case ERR_REWARD_MONEYLIMIT:
		{
			g_MainCharInfo.ShowHelpMessage(IDS_WORLDWARREWARD_RESULT_03, TEXTEFFECT_COLOR_WARNING);	//소지금액 초과
		}
		break;
	case ERR_REWARD_ALREADYGET:
		{
			g_pUIManager->ShowNotice(IDS_WORLDWARREWARD_RESULT_04);			//상금수령후 또 링버튼 눌렀을때..(상금 지급 완료)
		}
		break;
	case 10:
		{
			TCHAR strTemp[256] = {0,};
			_stprintf(strTemp, IDS_WORLDWARREWARD_RESULT_10, MoneyCommaStr(dwRewardMoney).data());
			g_pUIManager->ShowNotice(strTemp, NOTICE_FRAME_OKCANCEL, NOTICE_FRAME_CLAN_WAR_REWARD);	//상금 %d전
		}
		break;

	default:
		break;
	}

	return 0;
}

//HO_0227_07 광명전,천황전 이벤트 추가
int OnCS_WR_CHAMBEROFSECRETMESSAGE_ACK(CMsg &msg)//광명전(비밀의 방) 시스템 메세지
{
	BYTE bResult = 0;
	sString senderName		= _T("");	
	
	msg
		>> bResult
		>> senderName;
	
	switch(bResult)
	{
	case WWM_SECRETPREPAREREADY:
		{		
	//		g_MainCharInfo.SpecialChatMessage(IDS_SECRETPREPAREREADY, TEXTEFFECT_COLOR_GAIN); //이벤트 신청 10분전
		}
		break;

	case WWM_SECRETPREPARESTART:
		{
			g_MainCharInfo.SpecialChatMessage(IDS_SECRETPREPARESTART, TEXTEFFECT_COLOR_GAIN); //이벤트 신청 시작
		}
		break;

	case WWM_SECRETPREPAREEND:
		{	
			g_MainCharInfo.SpecialChatMessage(IDS_SECRETPREPAREEND, TEXTEFFECT_COLOR_GAIN); //이벤트 신청 끝
		}
		break;

	case WWM_APPLYSECRET:
		{	
	//		TCHAR szContent[128] = {0,};
	//		_stprintf( szContent, IDS_APPLYSECRET, (LPCTSTR)senderName);
	//		g_MainCharInfo.SpecialChatMessage(szContent, TEXTEFFECT_COLOR_GAIN);//누가신청했다 시스템메시지
		}
		break;

	case WWM_SECRETSTARTREADY:
		{
	//		g_MainCharInfo.SpecialChatMessage(IDS_SECRETSTARTREADY, TEXTEFFECT_COLOR_GAIN); //이벤트 시작 5분전
		}
		break;
	
	case WWM_SECRETSTART:
		{
			g_MainCharInfo.SpecialChatMessage(IDS_SECRETSTART, TEXTEFFECT_COLOR_GAIN); //이벤트 시작
		}
		break;

	case WWM_SECRETENDREADY:
		{
			g_MainCharInfo.SpecialChatMessage(IDS_SECRETENDREADY, TEXTEFFECT_COLOR_GAIN); //이벤트 종료 10분전
		}
		break;

	case WWM_SECRETEND:
		{
			g_MainCharInfo.SpecialChatMessage(IDS_SECRETEND, TEXTEFFECT_COLOR_GAIN); //이벤트 종료
		}
		break;

	case WWM_SECRETTIME:
		{
			g_MainCharInfo.SpecialChatMessage(IDS_SECRETTIME, TEXTEFFECT_COLOR_GAIN); //이벤트 진행중
		}
		break;

	default:
		break;
	}

	return 0;

}

int OnCS_WR_APPLYSECRETREADY_ACK(CMsg &msg)//광명전(비밀의 방) 참여 신청 버튼 클릭시
{
	BYTE SecretCnt;
	TCHAR szTip[256] = {0,};

	msg
		>> SecretCnt;

	_stprintf(szTip, IDS_SECRETROOM_CNT,SecretCnt);
	g_pUIManager->SetString(WINDOW_SECRET_INFORMATION, secret_information_window_dummy02, szTip);

	return 0;
}

int OnCS_WR_APPLYSECRET_ACK(CMsg &msg)//광명전(비밀의 방) 신청
{
	BYTE bResult = 0;

	g_MainCharInfo.CloseFrame(WINDOW_SECRET_INFORMATION);
	
	msg
		>> bResult;

	switch(bResult)
	{
	case ERR_APPLYSECRET_SUCCESS:
		{		
			g_MainCharInfo.ShowHelpMessage(IDS_APPLYSECRET_SUCCESS_01, TEXTEFFECT_COLOR_GENERAL); //성공
			g_MainCharInfo.ShowHelpMessage(IDS_APPLYSECRET_SUCCESS_02, TEXTEFFECT_COLOR_GENERAL); 
		}
		break;

	case ERR_APPLYSECRET_WRONGTERM:
		{
			g_MainCharInfo.ShowHelpMessage(IDS_APPLYSECRET_WRONGTERM, TEXTEFFECT_COLOR_WARNING); //기간아님
		}
		break;

	case ERR_APPLYSECRET_NOTLEVEL:
		{	
			g_MainCharInfo.ShowHelpMessage(IDS_APPLYSECRET_NOTLEVEL, TEXTEFFECT_COLOR_WARNING); //레벨제한
		}
		break;

	case ERR_APPLYSECRET_NOTENOUGHMONEY:
		{	
			g_MainCharInfo.ShowHelpMessage(IDS_APPLYSECRET_NOTENOUGHMONEY, TEXTEFFECT_COLOR_WARNING); //금전부족
		}
		break;

	case ERR_APPLYSECRET_ALREADY:
		{			
			g_MainCharInfo.ShowHelpMessage(IDS_APPLYSECRET_ALREADY, TEXTEFFECT_COLOR_WARNING); //이미신청
		}
		break;

	case ERR_APPLYSECRET_EXCESSCHAR:
		{
			g_MainCharInfo.ShowHelpMessage(IDS_APPLYSECRET_EXCESSCHAR_01, TEXTEFFECT_COLOR_WARNING); //인원제한
			g_MainCharInfo.ShowHelpMessage(IDS_APPLYSECRET_EXCESSCHAR_02, TEXTEFFECT_COLOR_WARNING); 
		}
		break;
	case ERR_APPLYSECRET_SACK:		
		{
			g_MainCharInfo.ShowHelpMessage(IDS_APPLYSECRET_SACKFULL, TEXTEFFECT_COLOR_WARNING); //HT_0417 : 행낭이 가득 차 있을 경우
		}
		break;
	
	default:
		break;
	}

	return 0;
}

int OnCS_WR_BLOODDEVILMESSAGE_ACK(CMsg &msg)//천황전(마혈천황의 방) 시스템메세지
{
	BYTE bResult = 0;
	sString senderName		= _T("");	

	msg
		>> bResult
		>> senderName;
	

	switch(bResult)
	{
	case WWM_DEVILPREPAREREADY:
		{		
	//		g_MainCharInfo.SpecialChatMessage(IDS_DEVILPREPAREREADY, TEXTEFFECT_COLOR_GAIN); //이벤트 신청 10분전
		}
		break;

	case WWM_DEVILPREPARESTART:
		{
			g_MainCharInfo.SpecialChatMessage(IDS_DEVILPREPARESTART, TEXTEFFECT_COLOR_GAIN); //이벤트 신청 시작
		}
		break;

	case WWM_DEVILPREPAREEND:
		{	
			g_MainCharInfo.SpecialChatMessage(IDS_DEVILPREPAREEND, TEXTEFFECT_COLOR_GAIN); //이벤트 신청 끝
		}
		break;

	case WWM_APPLYDEVIL:
		{
	//		TCHAR szContent[128] = {0,};
	//		_stprintf( szContent, IDS_APPLYSECRET, (LPCTSTR)senderName);
	//		g_MainCharInfo.SpecialChatMessage(szContent, TEXTEFFECT_COLOR_GAIN);//누가신청했다 시스템메시지
		}
		break;

	case WWM_DEVILSTARTREADY:
		{			
	//		g_MainCharInfo.SpecialChatMessage(IDS_DEVILSTARTREADY, TEXTEFFECT_COLOR_GAIN); //이벤트 시작 15분 전
		}
		break;

	case WWM_DEVILSTART:
		{
			g_MainCharInfo.SpecialChatMessage(IDS_DEVILSTART, TEXTEFFECT_COLOR_GAIN); //이벤트 시작
		}
		break;
	
	case WWM_DEVILENDREADY:
		{
			g_MainCharInfo.SpecialChatMessage(IDS_DEVILENDREADY, TEXTEFFECT_COLOR_GAIN); //이벤트 종료 5분전
		}
		break;

	case WWM_DEVILEND:
		{
			g_MainCharInfo.SpecialChatMessage(IDS_DEVILEND, TEXTEFFECT_COLOR_GAIN); //이벤트 종료
		}
		break;

	case WWM_DEVILTIME:
		{
			g_MainCharInfo.SpecialChatMessage(IDS_DEVILTIME, TEXTEFFECT_COLOR_GAIN); //이벤트 진행중이다.
		}
		break;
	default:
		break;
	}

	return 0;
}

int OnCS_WR_APPLYDEVILREADY_ACK(CMsg &msg)//천황전(마혈천황의 방) 참여 신청 버튼 클릭시
{
	BYTE DevilCnt;
	TCHAR szTip[256] = {0,};
	
	msg
		>> DevilCnt;

	_stprintf(szTip, IDS_DEVILROOM_CNT,DevilCnt);
	g_pUIManager->SetString(WINDOW_SECRET_INFORMATION, secret_information_window_dummy02, szTip);

	return 0;	
}

int OnCS_WR_APPLYDEVIL_ACK(CMsg &msg)//천황전(마혈천황의 방) 신청
{
	BYTE bResult = 0;
	
	g_MainCharInfo.CloseFrame(WINDOW_SECRET_INFORMATION);

	msg
		>> bResult;

	switch(bResult)
	{
	case ERR_APPLYDEVIL_SUCCESS:
		{		
			g_MainCharInfo.ShowHelpMessage(IDS_APPLYDEVIL_SUCCESS_01, TEXTEFFECT_COLOR_GENERAL); //성공
			g_MainCharInfo.ShowHelpMessage(IDS_APPLYDEVIL_SUCCESS_02, TEXTEFFECT_COLOR_GENERAL);
		}
		break;

	case ERR_APPLYDEVIL_WRONGTERM:
		{
			g_MainCharInfo.ShowHelpMessage(IDS_APPLYDEVIL_WRONGTERM, TEXTEFFECT_COLOR_WARNING); //기간아님
		}
		break;

	case ERR_APPLYDEVIL_NOTLEVEL:
		{	
			g_MainCharInfo.ShowHelpMessage(IDS_APPLYDEVIL_NOTLEVEL, TEXTEFFECT_COLOR_WARNING); //레벨제한
		}
		break;

	case ERR_APPLYDEVIL_HAVENOTITEM:
		{	
			g_MainCharInfo.ShowHelpMessage(IDS_APPLYDEVIL_HAVENOTITEM, TEXTEFFECT_COLOR_WARNING); //마교천패없음
		}
		break;

	case ERR_APPLYDEVIL_ALREADY:
		{			
			g_MainCharInfo.ShowHelpMessage(IDS_APPLYDEVIL_ALREADY, TEXTEFFECT_COLOR_WARNING); //이미신청
		}
		break;

	case ERR_APPLYDEVIL_EXCESSCHAR:
		{
			g_MainCharInfo.ShowHelpMessage(IDS_APPLYDEVIL_EXCESSCHAR_01, TEXTEFFECT_COLOR_WARNING); //인원제한
			g_MainCharInfo.ShowHelpMessage(IDS_APPLYDEVIL_EXCESSCHAR_02, TEXTEFFECT_COLOR_WARNING); 
		}
		break;
	
	default:
		break;
	}

	return 0;
	
}
