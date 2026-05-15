//#include <assert.h>

int OnCS_CD_ADDEQUIPMENT_ACK( CMsg &msg)
{
	DWORD dwCharID;
	DWORD dwItemID;
	BYTE bPos;
	WORD wVisualID;
	BYTE bRarity;
	BYTE bStxType;

	msg 
		>> dwCharID
		>> dwItemID
		>> bPos
		>> wVisualID
		>> bRarity
		>> bStxType;

	if(bPos == 4)
		return 0;

	XiahObject::CXiahObject *pObject = XiahObject::g_XiahObjectManager.FindXiahObject( MAKEOBJECTID( 0, dwCharID,OBJTYPE_PC));
	if( pObject == NULL)// || pObject == g_pMainChar)
	{
		return 0;
	}

	CXiahCharObject* pCharObject = (CXiahCharObject*)pObject->m_pObject;

	//YS_0811 : BUGFIX
	if ( pCharObject == NULL )
	{
        return 0;
	}
	
	sPCVisualInfo* pVisualInfo = (sPCVisualInfo*)pCharObject->m_pPrivateData;

	//YS_0811 : BUGFIX
	if ( pVisualInfo == NULL )
	{
        return 0;
	}

	pVisualInfo->wVisualID[ bPos] = wVisualID;
	pVisualInfo->bRarity[ bPos] = bRarity;
	pVisualInfo->bStxType[ bPos] = bStxType;

	XiahObject::CXiahObjectManager::iterator it;
	for(it = XiahObject::g_XiahObjectManager.begin(); it != XiahObject::g_XiahObjectManager.end(); it++)
	{
		XiahObject::CXiahObject* pObj = it->second;
		CXiahCharObject* pC = (CXiahCharObject*) pObj->m_pObject;
		if(pC == NULL) continue;

		// 분신격이면 바꿔준다.
		if(pC->m_bObjType == OBJTYPE_PET && pC->m_dwOwnerID == dwCharID && pC->m_nCurMotionType != XiahAniType::eLAT_Die)
		{
			SetupPC_VisualEquipement( pC, pVisualInfo->wVisualID, pVisualInfo->bRarity, pVisualInfo->bStxType );		
		}
	}

	// 누가 이렇게 했지? ㅡ,.ㅡ;
	// 이걸 주석해서 메인 캐릭터가 아이템을 개조하면 아이템의 데이타를 갱신한다.
//	if( pObject != g_pMainChar)
		SetupPC_VisualEquipement( pCharObject, pVisualInfo->wVisualID, pVisualInfo->bRarity, pVisualInfo->bStxType );

	return 0;
}

int OnCS_CD_CHGEQUIPMENT_ACK( CMsg &msg)
{
	DWORD dwCharID;
	DWORD dwItemID;
	BYTE bPos;
	WORD wVisualID;
	BYTE bRarity;
	BYTE bStxType;

	msg 
		>> dwCharID
		>> dwItemID
		>> bPos
		>> wVisualID
		>> bRarity
		>> bStxType;

	XiahObject::CXiahObject *pObject = XiahObject::g_XiahObjectManager.FindXiahObject( MAKEOBJECTID( 0, dwCharID,OBJTYPE_PC));
	if( pObject == NULL)
	{
		return 0;
	}
	
	CXiahCharObject* pCharObject = (CXiahCharObject*)pObject->m_pObject;

	//YS_0811 : BUGFIX
	if ( pCharObject == NULL )
	{
        return 0;
	}

	sPCVisualInfo* pVisualInfo = (sPCVisualInfo*)pCharObject->m_pPrivateData;

	//YS_0811 : BUGFIX
	if ( pVisualInfo == NULL )
	{
        return 0;
	}

	pVisualInfo->wVisualID[ bPos] = wVisualID;
	pVisualInfo->bRarity[ bPos] = bRarity;
	pVisualInfo->bStxType[ bPos] = bStxType;

	XiahObject::CXiahObjectManager::iterator it;
	for(it = XiahObject::g_XiahObjectManager.begin(); it != XiahObject::g_XiahObjectManager.end(); it++)
	{
		XiahObject::CXiahObject* pObj = it->second;
		CXiahCharObject* pC = (CXiahCharObject*) pObj->m_pObject;
		if(pC == NULL) continue;

		// 분신격이면 바꿔준다.
		if(pC->m_bObjType == OBJTYPE_PET && pC->m_dwOwnerID == dwCharID && pC->m_nCurMotionType != XiahAniType::eLAT_Die)
		{
			SetupPC_VisualEquipement( pC, pVisualInfo->wVisualID, pVisualInfo->bRarity, pVisualInfo->bStxType );		
		}
	}

	// 누가 이렇게 했지? ㅡ,.ㅡ;
	// 이걸 주석해서 메인 캐릭터가 개조된 아이템을 바꾸면 아이템의 데이타를 갱신한다.
//	if( pObject != g_pMainChar)
		SetupPC_VisualEquipement( pCharObject, pVisualInfo->wVisualID, pVisualInfo->bRarity, pVisualInfo->bStxType );

	return 0;
}

int OnCS_CD_DELEQUIPMENT_ACK( CMsg &msg)
{
	DWORD dwCharID	=0;
	DWORD dwItemID	=0;
	WORD wVisualID	=0;
	BYTE bPos		=0;
	BYTE bType		=0;
	BYTE bKind		=0;

	msg 
		>> dwCharID
		>> dwItemID
		>> bPos
		>> wVisualID
		>> bType
		>> bKind;

	if(dwCharID < 400000000) 
	{
		DBG_LogFile("OnCS_CD_DELEQUIPMENT_ACK (OBJID) :%d",dwCharID);
		return FALSE;
	}

	XiahObject::CXiahObject *pObject = XiahObject::g_XiahObjectManager.FindXiahObject( MAKEOBJECTID( 0, dwCharID,OBJTYPE_PC));
	if( pObject == NULL)
	{
		return 0;
	}

	CXiahCharObject* pCharObject = (CXiahCharObject*)pObject->m_pObject;
	sPCVisualInfo* pVisualInfo = (sPCVisualInfo*)pCharObject->m_pPrivateData;

	pVisualInfo->wVisualID[ bPos] = 0;
	pVisualInfo->bRarity[ bPos] = 0;
	pVisualInfo->bStxType[ bPos] = 0;

	XiahObject::CXiahObjectManager::iterator it = XiahObject::g_XiahObjectManager.begin();
	for(; it != XiahObject::g_XiahObjectManager.end(); ++it)
	{
		XiahObject::CXiahObject* pObj = it->second;
		CXiahCharObject* pC = (CXiahCharObject*) pObj->m_pObject;

		if(pC == NULL)
			continue;

		// 분신격이면 바꿔준다.
		if(pC->m_bObjType == OBJTYPE_PET && pC->m_dwOwnerID == dwCharID && pC->m_nCurMotionType != XiahAniType::eLAT_Die)
		{
			SetupPC_VisualEquipement( pC, pVisualInfo->wVisualID, pVisualInfo->bRarity, pVisualInfo->bStxType );		
		}
	}

	if( pObject != g_pMainChar)
		SetupPC_VisualEquipement( pCharObject, pVisualInfo->wVisualID, pVisualInfo->bRarity, pVisualInfo->bStxType );

	return 0;
}

// CASUAL ACTION
int OnCS_CD_ACTION_ACK( CMsg &msg)
{
	DWORD dwObjID		=0;
	BYTE  bActionType	=0;
	BYTE  bActionKind	=0;
	WORD  wDirection	=0;

	msg 
		>> dwObjID
		>> bActionType
		>> bActionKind
		>> wDirection;

	XiahObject::CXiahObject *pObject = XiahObject::g_XiahObjectManager.FindXiahObject( MAKEOBJECTID( 0, dwObjID,OBJTYPE_PC));
	
	if(pObject != NULL)
	{
		CXiahCharObject* pCharObject = (CXiahCharObject*)pObject->m_pObject;

		if(pCharObject != NULL)
		{
			if(bActionKind == 1)	// 앉기는 앉기 유기로 가기 땜시.. 따로 처리한다.
			{
				// 앉기 다음은 -> 앉기 유지로..
				pCharObject->SetAnimation((int)bActionType,(int)bActionType,(int)bActionKind,2,1.0f);
			}
			else
			{
				// 케주얼 다음은 Stand로
				pCharObject->SetAnimation((int)bActionType,XiahAniType::eLAT_Stand,(int)bActionKind,0,1.0f);
			}
		}
	}

	return 0;
}

#define FE_SOUND_1		50002232	// 검영 오행 시전 사운드
#define FE_SOUND_2		50002233	// 연랑/무투/야차 시전 사운드

/**
 * 그때 그때 업데이트 되는것 (명성치등등..)
 * \param &msg 
 * \return 
 */
int OnCS_CD_CHARUPDATE_ACK( CMsg &msg)
{
	BYTE	bType = 0;
	DWORD	dwObjID = 0;
	DWORD	dwData1 = 0;
	DWORD	dwData2 = 0;
	DWORD	dwData3 = 0;
	sString strTemp1, strTemp2, strTemp3;

	msg 
		>> bType
		>> dwObjID
		>> dwData1
		>> dwData2
		>> dwData3
		>> strTemp1
		>> strTemp2
		>> strTemp3;

	XiahObject::CXiahObject *pObject = XiahObject::g_XiahObjectManager.FindXiahObject( MAKEOBJECTID( 0, dwObjID, OBJTYPE_PC));
	if( pObject == NULL)
		return 0;

	CXiahCharObject* pCharObject = (CXiahCharObject*)pObject->m_pObject;
	if(pCharObject == NULL)
		return 0;

	switch(bType)
	{		
		case ACT_FAME:	// 명성치
			{
				pCharObject->m_dwFame = dwData1;

				pCharObject->RefreshFameColor();

				if(g_MainCharInfo.m_dwObjectID == dwObjID)
				{
					if(dwData1 >= 127)
					{   // 노락색 계열
						if(dwData1 >= 128 && dwData1 <= 132)		// 선인 1단계
						{						
							g_MainCharInfo.ShowHelpMessage(IDS_FAME_A1);
						}
						else if(dwData1 >= 133 && dwData1 <= 226)	// 선인 2단계
						{
							g_MainCharInfo.ShowHelpMessage(IDS_FAME_A2);
						}
						else if(dwData1 >= 227)						// 선인 3단계
						{
							g_MainCharInfo.ShowHelpMessage(IDS_FAME_A3);
						}
						else  // 중립
						{
							g_MainCharInfo.ShowHelpMessage(IDS_FAME_A0);
						}
					} // if(dwFame >= 127)
					else
					{
						// 빨간색 계열
						if(dwData1 <= 121 && dwData1 >= 28) // 악인 2단계
						{
							g_MainCharInfo.ShowHelpMessage(IDS_FAME_B2);
						}
						else if(dwData1 <= 27) // 악인 3단계
						{
							g_MainCharInfo.ShowHelpMessage(IDS_FAME_B3);
						}
						else  // 악인 1단계
						{
							g_MainCharInfo.ShowHelpMessage(IDS_FAME_B1);
						}
					}
				}				
			}			
			break;

		case ACT_SHOPCHANGE: // 개인 상점 상태
			{
				pCharObject->SetAnimation(XiahAniType::eLAT_Stand, 0);

				if(dwData1 == 0)
					pCharObject->m_bTradeSell = false;
				else if(dwData1 == 1)
				{
					pCharObject->m_bTradeSell = true;

					pCharObject->m_strShopName = strTemp1;
					pCharObject->m_strShopDescription = strTemp2;
				}
				else
				{
					DBG_Put(_T("OnCS_CD_CHARUPDATE_ACK 개인노점 상태 비정상 결과 %d"),dwData1);
					// 비정상 데이터
					//assert(0);
				}
			}
			break;

		case ACT_TRADEWARNING: // 1:1 거래시 자신의 행낭의 공간이 없을때
			{
				g_pUIManager->ShowNotice( IDS_PCTRADE_FULLSACK, NOTICE_FRAME_OK);
				g_MainCharInfo.ShowHelpMessage(IDS_FULL_SACK, 2);

				SendCS_EC_TRADEITEM_REQ( 9, g_MainCharInfo.m_dwAskID);
			}
			break;

		case ACT_SEMIPK:	// SEMI PK System
			{
				// SEMI PK의 상태를 저장
				// 0 : 정상 1 : 보라색 2 : 깜빡임
				pCharObject->m_bSemiPKStatus = (BYTE)dwData1;
			}
			break;

		case ACT_MINE:		// 채집
			{
				pCharObject->SetAngle(static_cast<WORD>(dwData1));
				pCharObject->SetAnimation(XiahAniType::eLAT_Collect, 0);				
			}
			break;
			
		case ACT_FIVEELM:	// 오행
			{
				if(dwData1)
				{
					// 이전것 지우자 (중간에 책읽는사람있다.)
					for(int i=0; i < 5; ++i)
						pCharObject->m_KeepUpMugongList.Delete(FIVEELEMENT_FIRE + i);					

					if(pCharObject->m_pFEEffectPP)
					{
						g_EffectManager.DeqEffectPackagePair(pCharObject->m_pFEEffectPP);
						pCharObject->m_pFEEffectPP = NULL;
					}

					// 셋팅
					pCharObject->m_bFECur   = static_cast<BYTE>(dwData1);
					pCharObject->m_bFELevel = static_cast<BYTE>(dwData2);

					DWORD dwMugongID = dwData1 + FIVEELEMENT_FIRE - 1;
					DWORD dwTime =0;

					sArrayData* pMugongList = XiahArrayIndex::g_MugongList.GetData(dwMugongID, pCharObject->m_bFELevel);					

					if( pMugongList)
						dwTime = pMugongList->GetInt(9);

					pCharObject->m_KeepUpMugongList.Add(dwMugongID, pCharObject->m_bFELevel, dwTime);					
					
					// 시전 이펙트
					int nType;

					switch(dwData1)
					{
						case 1:	nType = eFEPrepareFire;		break;
						case 2:	nType = eFEPrepareWater;	break;
						case 3:	nType = eFEPrepareTree;		break;
						case 4:	nType = eFEPrepareMetal;	break;
						case 5:	nType = eFEPrepareEarth;	break;
						default:
							return 1;
							break;
					}

					_EFFECTPACKAGE* pEffectPackage = g_EffectManager.EnqOutGongPersistEffectImmediately(nType);

					if(pEffectPackage && pEffectPackage->pEffectRender && pEffectPackage->pEffectRender->pPackagePair)
					{
						pEffectPackage->pEffectRender->pPackagePair->pWorldMatrix = (MATRIX*)pCharObject->m_CharRender.GetCharTM();							

						// 이케해야 캐릭터가 없어질때 이펙트도 같이 없어진다.
						pCharObject->m_EffectPPList.push_back(pEffectPackage->pEffectRender->pPackagePair );
					}

					// 오행 시전 애니
					switch(pCharObject->m_bSubObjType)
					{
						case 1:							
						case 2:
							{
								pCharObject->SetAnimation(XiahAniType::eLAT_Mugong, XiahAniType::eLAT_Stand, 12, -1, 1.0f);
							}
							break;
						case 3:
							{
								pCharObject->SetAnimation(XiahAniType::eLAT_Mugong, XiahAniType::eLAT_Stand, 10, -1, 1.0f);
							}
							break;
						case 4:
							{
								pCharObject->SetAnimation(XiahAniType::eLAT_Mugong, XiahAniType::eLAT_Stand, 0, -1, 1.0f);
							}
							break;
						default:
							break;
					}

					// 시전 사운드
					if(pCharObject->m_bSubObjType == 1)
					{
						g_MainCharInfo.PlayInterfaceSound(FE_SOUND_1);
					}
					else
					{
						g_MainCharInfo.PlayInterfaceSound(FE_SOUND_2);
					}
				}
				else
				{
					for(int i=0; i < 5; ++i)
					{
						pCharObject->m_KeepUpMugongList.Delete(FIVEELEMENT_FIRE + i);									//오행 이펙트 제거 
					}

					pCharObject->m_bFECur   = 0;
					pCharObject->m_bFELevel = 0;

					if(pCharObject->m_pFEEffectPP)
					{
						g_EffectManager.DeqEffectPackagePair(pCharObject->m_pFEEffectPP);
						pCharObject->m_pFEEffectPP = NULL;
					}
				}
			}
			break;

			// CG_2005/01/28 : 변종아이템기능추가
		case ACT_CHANGECOMPLE:	// 같은 유파변종아이템을 셋트로 찼을때
			{				
				pCharObject->m_bChangeItemSet = (BYTE)dwData1;
			}
			break;
		case 11:				// 설승단약, 기
			{
				BYTE bInstanceType = static_cast<BYTE>(dwData1);

				switch(bInstanceType)
				{
				case 0:
					{
						pCharObject->m_bPotionEndKeepup = static_cast<BYTE>(dwData2);

						if(dwData2 == 0)		// 미복용
						{
							if(pCharObject->m_pEventItemEffectPP)
							{
								g_EffectManager.DeqEffectPackagePair(pCharObject->m_pEventItemEffectPP);
								pCharObject->m_pEventItemEffectPP = NULL;
							}
						}
						else if(dwData2 == 1)	// 복용
						{
							_EFFECTPACKAGE* pEffectPackage = g_EffectManager.EnqOutGongPersistEffectImmediately(ePotionBegine);

							if(pEffectPackage && pEffectPackage->pEffectRender && pEffectPackage->pEffectRender->pPackagePair)
							{
								pEffectPackage->pEffectRender->pPackagePair->pWorldMatrix = (MATRIX*)pCharObject->m_CharRender.GetCharTM();							

								// 이케해야 캐릭터가 없어질때 이펙트도 같이 없어진다.
								pCharObject->m_EffectPPList.push_back(pEffectPackage->pEffectRender->pPackagePair );
							}
						}
					}
					break;
				case 1:
					{
						pCharObject->m_bSpirit = static_cast<BYTE>(dwData2);

						if(dwData2 == 0)		// 미복용
						{
							if(pCharObject->m_pSpiritEffectPP)
							{
								g_EffectManager.DeqEffectPackagePair(pCharObject->m_pSpiritEffectPP);
								pCharObject->m_pSpiritEffectPP = NULL;
							}
						}
						else if(dwData2 == 1)	// 복용
						{
							_EFFECTPACKAGE* pEffectPackage = g_EffectManager.EnqOutGongPersistEffectImmediately(eSpiritBegine);

							if(pEffectPackage && pEffectPackage->pEffectRender && pEffectPackage->pEffectRender->pPackagePair)
							{
								pEffectPackage->pEffectRender->pPackagePair->pWorldMatrix = (MATRIX*)pCharObject->m_CharRender.GetCharTM();							

								// 이케해야 캐릭터가 없어질때 이펙트도 같이 없어진다.
								pCharObject->m_EffectPPList.push_back(pEffectPackage->pEffectRender->pPackagePair);
							}
						}
					}
					break;
				default:
					break;
				}
			}
			break;

		default:
			{
				DBG_Put(_T("OnCS_CD_CHARUPDATE_ACK 타입 이상 %d"), bType);
			}
			break;
	}

	return 1;
}
