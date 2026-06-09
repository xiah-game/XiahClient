#include "mail.h"



/**
 * @brief 각성 아이템 상시 이펙트 강제로 연결 함수
 * \param bItemType 아이템 타입
 * \param wVisualID 비주얼 ID
 * \param pObject 오브젝트 포인터
 */
void SetRebirthEffect(BYTE bItemType, WORD wVisualID, CXiahCharObject* pObject)
{
	if(bItemType == ITEMTYPE_REBIRTH)
	{
		eAppearEnum effectType;

		switch(wVisualID)
		{
		case 8650:	// 기린석
			effectType = eRebirthItem1;
			break;
		case 8651:	// 봉황석
			effectType = eRebirthItem2;
			break;
		case 8652:	// 현무석
			effectType = eRebirthItem3;
			break;
		case 8653:	// 청룡석
			effectType = eRebirthItem4;
			break;
		case 8654:	// 건곤석
			effectType = eRebirthItem5;
			break;
		case 8655:	// 음양석
			effectType = eRebirthItem6;
			break;
		default:
			return;
			break;
		}

		// 미리 앞에서 노가다로 예약해둔 이펙트를 가져와서 연결 시켜 주자
		_EFFECTPACKAGE* pPackage = g_EffectManager.EnqAppearEffectImmediately(effectType);

		if(pPackage)
		{
			pObject->UpdateTM();
			pPackage->pEffectRender->pPackagePair->WorldMatrix = (MATRIX)pObject->m_ObjectTM;
			pPackage->pEffectRender->pPackagePair->WorldMatrix._42 += 1.0f;
			pObject->m_pRebirthItemEffectPP = pPackage->pEffectRender->pPackagePair;
		}
	}
}

//---------------------------------------------------------------------------------------
int OnCS_IM_MAPITEMINFOLIST_ACK(CMsg &msg)
{
	BYTE  bResult	=0;
	DWORD dwMapID	=0;
	WORD  wObjectNum	=0;

	msg >> bResult
		>> dwMapID
		>> wObjectNum;

	for(int i = 0; i < wObjectNum; ++i)
	{
		WORD	wPosX		=0;
		WORD	wPosY		=0;
		WORD	wVisualID	=0;
		BYTE	bHeight		=0;		
		BYTE	bItemType	=0;
		DWORD	dwObjectID	=0;
		DWORD   dwItemID	=0;
		DWORD	dwAmount	=0;
		sString	szName;
		// CG_2005/01/28 : 변종아이템기능추가
		BYTE	bChangeItem = 0; 
		
		msg
			>> dwObjectID  
			>> wPosX
			>> wPosY
			>> bHeight
			>> bItemType
			>> wVisualID
			>> szName
			>> dwItemID
			>> dwAmount
			// CG_2005/01/28 : 변종아이템기능추가
			>> bChangeItem;

		if( dwObjectID == 0)
			continue;

		// 새로운 캐릭터면 첨부터 생성하지만, 존재하는 거라면 데이타를 바꿔준다.
		bool bCreateChar = true;

		XiahObject::CXiahObject* pXiahObject = XiahObject::g_XiahObjectManager.FindXiahObject( MAKEOBJECTID( 0, dwObjectID,OBJTYPE_ITEM) );
		if( pXiahObject != NULL )
			bCreateChar = false;

		XiahItem::sItemInfo *pItemInfo = NULL;
		if( bCreateChar )
			pItemInfo = new XiahItem::sItemInfo;
		else
		{
			pItemInfo = (XiahItem::sItemInfo*)(reinterpret_cast<CXiahCharObject*>(pXiahObject->m_pObject))->m_pPrivateData;

			//YS_0811 : BUGFIX
			if ( !pItemInfo )
				pItemInfo = new XiahItem::sItemInfo;
		}

		//YS_0811 : BUGFIX
		if ( !pItemInfo )
			continue;

		pItemInfo->m_dwMapID		= dwMapID;
		pItemInfo->m_dwMapObjectID	= dwObjectID;
		pItemInfo->m_wVisualID		= wVisualID;
		pItemInfo->m_dwAmount		= dwAmount;
		pItemInfo->m_dwItemID		= dwItemID;

		BOOL bVisualOk = XiahItem::SetItemVisualData( pItemInfo);
		CRes_Character* pChar = NULL;
		Res_Mesh* pMesh = NULL;
		Res_CharTexture* pTexture = NULL;

		if (bVisualOk)
		{
			pChar = GetCharacter( pItemInfo->m_nMapCharID);
			if (pChar) pMesh = pChar->GetMesh( pItemInfo->m_nMapMeshType);
			if (pMesh) pTexture = pMesh->GetTexture( pItemInfo->m_nMapTextureType);
		}

		if (pChar == NULL || pMesh == NULL || pTexture == NULL)
		{
			pItemInfo->m_wVisualID = 10050; // Fallback to safe money pouch visual ID
			XiahItem::SetItemVisualData( pItemInfo);
			pChar = GetCharacter( pItemInfo->m_nMapCharID);
			pMesh = pChar ? pChar->GetMesh( pItemInfo->m_nMapMeshType) : NULL;
			pTexture = pMesh ? pMesh->GetTexture( pItemInfo->m_nMapTextureType) : NULL;
		}

		if( bCreateChar )
		{
			if ( pChar == NULL || pMesh == NULL || pTexture == NULL )
			{
				delete pItemInfo;
				pItemInfo = NULL;
				return TRUE;
			}
		}// if

		CXiahCharObject* pObject = NULL;
		if( bCreateChar )
		{
			pObject = new CXiahCharObject;
		}
		else
		{
			pObject = reinterpret_cast<CXiahCharObject*>(pXiahObject->m_pObject);		
		}

		//YS_0811 : BUGFIX
		if ( !pObject )
			continue;
		
		pObject->Create( pItemInfo->m_nMapCharID, pItemInfo->m_nMapMeshType, pItemInfo->m_nMapTextureType, -1);

		pObject->m_pPrivateData = (DWORD)pItemInfo;
		pObject->m_PrivateDataDestoryer = XiahItem::ReleaseItemInfo; // 앗싸
		// CG_2005/01/28 : 변종아이템기능추가
		pObject->m_bChangeItemSet = bChangeItem;

		pObject->SetPosition( wPosX, wPosY);

		if(bItemType == ITEMTYPE_MONEY && pItemInfo->m_bItemKind != 1)
		{
			TCHAR strTemp[64] = {0,};
			_stprintf(strTemp,"%d%s", dwAmount, JUN_MONEY);
			pObject->m_szObjectName = strTemp;
		}
		else
		{
			if(dwAmount > 1 && bItemType != ITEMTYPE_LOTTO)
			{
				TCHAR strTemp[64] = {0,};
				_stprintf(strTemp, IDS_ITEM_COUNT_STRING, szName.data(), dwAmount);
				pObject->m_szObjectName = strTemp;
			}
			else
			{
				pObject->m_szObjectName = szName;
			}
		}

		pObject->m_bObjType = OBJTYPE_ITEM;	
		pObject->m_bCreateItemGroundEffect = true;	// 바닥 아이템 이펙트 생성.

		if( bCreateChar )
			XiahObject::g_XiahObjectManager.CreateXiahObject( dwObjectID, OBJTYPE_ITEM, pObject);

		SetRebirthEffect(bItemType, wVisualID, pObject);
	}

	DBG_Put(_T("CS_IM_MAPITEMINFOLIST_ACK"));

	return TRUE;
}


//---------------------------------------------------------------------------------------


int OnCS_IM_ADDONMAP_ACK(CMsg& msg)
{
	BYTE bResult	=0;
	WORD wItemNum	=0;
	
	msg
		>> bResult
		>> wItemNum;

	for(int i = 0; i < wItemNum; ++i)
	{
		DWORD	dwObjectID	=0;
		DWORD	dwMapID		=0;
		DWORD	dwItemID	=0;
		DWORD	dwAmount	=0;
		DWORD	dwOwnerID	=0;		// 주어도 되는 사람의 ID
		WORD	wPosX		=0;
		WORD	wPosY		=0;
		WORD	wVisualID	=0;
		BYTE	bHeight		=0;	
		BYTE	bItemType	=0;				
		BYTE	bType		=0;		// PC 가 떨어트렸나? 아니면 PET ?
		BYTE	bFESocket	=0;		// 소켓 아이템인지

		// CG_2005/01/28 : 변종아이템기능추가
		BYTE	bChangeItem = 0;			// 변종셋트를 다 차고있냐?

		sString	szName;

		msg
			>> dwMapID
			>> wPosX
			>> wPosY
			>> bHeight
			>> dwObjectID
			>> bItemType
			>> wVisualID
			>> szName
			>> dwItemID
			>> dwAmount
			>> dwOwnerID
			>> bType
			>> bFESocket
			// CG_2005/01/28 : 변종아이템기능추가
			>> bChangeItem;

		XiahItem::sItemInfo *pItemInfo = new XiahItem::sItemInfo;

		pItemInfo->m_dwMapID		= dwMapID;
		pItemInfo->m_dwMapObjectID	= dwObjectID;
		pItemInfo->m_wVisualID		= wVisualID;
		pItemInfo->m_dwAmount		= dwAmount;
		pItemInfo->m_dwItemID		= dwItemID;
		pItemInfo->m_szName			= szName;

		BOOL bVisualOk = XiahItem::SetItemVisualData( pItemInfo);
		CRes_Character* pChar = NULL;
		Res_Mesh* pMesh = NULL;
		Res_CharTexture* pTexture = NULL;

		if (bVisualOk)
		{
			pChar = GetCharacter( pItemInfo->m_nMapCharID);
			if (pChar) pMesh = pChar->GetMesh( pItemInfo->m_nMapMeshType);
			if (pMesh) pTexture = pMesh->GetTexture( pItemInfo->m_nMapTextureType);
		}

		if (pChar == NULL || pMesh == NULL || pTexture == NULL)
		{
			pItemInfo->m_wVisualID = 10050; // Fallback to safe money pouch visual ID
			XiahItem::SetItemVisualData( pItemInfo);
			pChar = GetCharacter( pItemInfo->m_nMapCharID);
			pMesh = pChar ? pChar->GetMesh( pItemInfo->m_nMapMeshType) : NULL;
			pTexture = pMesh ? pMesh->GetTexture( pItemInfo->m_nMapTextureType) : NULL;
		}

		if (pChar == NULL || pMesh == NULL || pTexture == NULL)
		{
			delete pItemInfo;
			pItemInfo = NULL;
			return TRUE;
		}

		CXiahCharObject* pObject = new CXiahCharObject;
		
		pObject->Create( pItemInfo->m_nMapCharID, pItemInfo->m_nMapMeshType, pItemInfo->m_nMapTextureType, -1);

		// 아이템에도 이펙트가 있군. 쩝.. 
		pObject->m_CharRender.MakeMeshEffect();

		pObject->m_pPrivateData = (DWORD)pItemInfo;
		pObject->m_PrivateDataDestoryer = XiahItem::ReleaseItemInfo; // 앗싸
		// CG_2005/01/28 : 변종아이템기능추가
		pObject->m_bChangeItemSet = bChangeItem;

		pObject->SetPosition( wPosX, wPosY);

		if(bItemType == ITEMTYPE_MONEY && pItemInfo->m_bItemKind != 1)
		{
			TCHAR strTemp[64] = {0,};
			_stprintf(strTemp, "%d%s", dwAmount, JUN_MONEY);
			pObject->m_szObjectName = strTemp;
		}
		else
		{
			if(dwAmount > 1 && bItemType != ITEMTYPE_LOTTO && bItemType != ITEMTYPE_EVENT)
			{
				TCHAR strTemp[64] = {0,};
				_stprintf(strTemp, IDS_ITEM_COUNT_STRING, szName.data(), dwAmount);
				pObject->m_szObjectName = strTemp;
			}
			else
			{
				pObject->m_szObjectName = szName;
			}
		}

		pObject->m_bObjType = OBJTYPE_ITEM;
		pObject->m_bCreateItemGroundEffect = true;	// 바닥 아이템 이펙트 생성.

	

		// PET이 떨어뜨림
		if(bType == OBJTYPE_PET)
		{
			sPetInfo* pet = g_PetList.GetCurrentPet();
			if(pet) pObject->m_dwOwnerID	= pet->dwOwnID;
		}
		else
		if(bType == OBJTYPE_PC)		// PC가 떨어뜨림
		{
			pObject->m_dwOwnerID	= dwOwnerID;
		}

		// 소켓 아이템
		if(bFESocket)
			pObject->m_cNameColor = D3DCOLOR_XRGB(100, 255, 255);

		XiahObject::g_XiahObjectManager.CreateXiahObject(dwObjectID, OBJTYPE_ITEM, pObject);

		Vector3	sPos((float)wPosX, 0.0f, -(float)wPosY);
		
		// Drop Item sound
		switch(bItemType)
		{
			case 1:	// 무기
			case 2:	// 옷
			case 3:	// 모자
			case 4:	// 신발
				{
					g_MainCharInfo.PlayInterfaceSoundWithVol(WEAPON_ITEM_SOUND,sPos);
				}
				break;
			case 6:	// 반지
			case 7:	// 목걸이
				{
					g_MainCharInfo.PlayInterfaceSoundWithVol(RING_ITEM_SOUND,sPos);
				}
				break;

			case 21:	// 책
				{
					g_MainCharInfo.PlayInterfaceSoundWithVol(BOOK_ITEM_SOUND,sPos);
				}
				break;

			case 24:	// 돈
				{
					g_MainCharInfo.PlayInterfaceSoundWithVol(MONEY_ITEM_SOUND,sPos);
				}
				break;

			case 25:	// 개조재원 (흑,소,혈정)
				{
					// 제련 자원용 사운드
					if(pItemInfo->m_bItemKind >= 4 && pItemInfo->m_bItemKind <= 6)
					{
						g_MainCharInfo.PlayInterfaceSoundWithVol(FE_ITEM_SOUND, sPos);
					}
					else
					{
						// 일반 개조자원
						g_MainCharInfo.PlayInterfaceSoundWithVol(REBUILD_ITEM_SOUND, sPos);
					}
				}
				break;

			default:	// 나머지 것들
				{
					// 타입이 겹쳐서 따로 처리
					if(19 == bItemType)	// 이벤트 아이템 타입
					{						
						if(wVisualID >= 400 && wVisualID <= 408)		// 중원천도
						{
							g_MainCharInfo.PlayInterfaceSoundWithVol(BOOK_ITEM_SOUND,sPos);
						}
						else if(wVisualID >= 410 && wVisualID <= 416)	// 마노세트
						{
							g_MainCharInfo.PlayInterfaceSoundWithVol(REBUILD_ITEM_SOUND,sPos);
						}
						else
						{
							g_MainCharInfo.PlayInterfaceSoundWithVol(ETC_ITEM_SOUND, sPos);
						}
					}
					else
					{
						g_MainCharInfo.PlayInterfaceSoundWithVol(ETC_ITEM_SOUND, sPos);
					}					
				}
				break;
		}

		SetRebirthEffect(bItemType, wVisualID, pObject);
	}

	return TRUE;
}

//---------------------------------------------------------------------------------------
int OnCS_IM_REMOVEFROMMAP_ACK(CMsg &msg)
{
	DWORD	dwObjectID =0;

	msg
		>> dwObjectID;

	XiahObject::g_XiahObjectManager.ReleaseXiahObject( MAKEOBJECTID( 0, dwObjectID,OBJTYPE_ITEM));

	return TRUE;
}


int OnCS_IM_REMOVELISTFROMMAP_ACK(CMsg &msg)
{
	DWORD numItem;
	DWORD dwObjectID;

	msg >> numItem;

	for(int i = 0; i < numItem; i++)
	{
		msg >> dwObjectID;

		XiahObject::g_XiahObjectManager.ReleaseXiahObject( MAKEOBJECTID( 0, dwObjectID, OBJTYPE_ITEM));
	}

	return 0;
}

//---------------------------------------------------------------------------------------
int OnCS_IM_MAPITEMINFO_ACK(CMsg &msg)
{
	BYTE	bResult		=0;
	BYTE	bHeight		=0;
	BYTE	bItemType	=0;
	WORD	wPosX		=0;
	WORD	wPosY		=0;	
	WORD	wVisualID	=0;
	DWORD	dwObjectID	=0;
	DWORD	dwMapID		=0;	
	DWORD   dwItemID	=0;
	DWORD	dwAmount	=0;
	// CG_2005/01/28 : 변종아이템기능추가
	BYTE	bChangeItem	=0;

	sString	szName;

	msg
		>> bResult
		>> dwObjectID  
		>> dwMapID
		>> wPosX
		>> wPosY
		>> bHeight
		>> bItemType
		>> wVisualID
		>> szName
		>> dwItemID
		>> dwAmount
		// CG_2005/01/28 : 변종아이템기능추가
		>> bChangeItem;

	if( dwObjectID == 0)
		return TRUE;

	// 새로운 캐릭터면 첨부터 생성하지만, 존재하는 거라면 데이타를 바꿔준다.
	bool bCreateChar = true;

    XiahObject::CXiahObject* pXiahObject = XiahObject::g_XiahObjectManager.FindXiahObject( MAKEOBJECTID( 0, dwObjectID,OBJTYPE_ITEM) );
	if( pXiahObject != NULL )
		bCreateChar = false;

	XiahItem::sItemInfo *pItemInfo = NULL;
	if( bCreateChar )
	{
        pItemInfo = new XiahItem::sItemInfo;
	}
	else
	{
		pItemInfo = (XiahItem::sItemInfo*)(reinterpret_cast<CXiahCharObject*>(pXiahObject->m_pObject))->m_pPrivateData;

		//YS_0811 : BUGFIX
		if ( !pItemInfo )
			pItemInfo = new XiahItem::sItemInfo;
	}

	pItemInfo->m_dwMapID		= dwMapID;
	pItemInfo->m_dwMapObjectID	= dwObjectID;
	pItemInfo->m_wVisualID		= wVisualID;
	pItemInfo->m_dwAmount		= dwAmount;
	pItemInfo->m_dwItemID		= dwItemID;

	BOOL bVisualOk = XiahItem::SetItemVisualData( pItemInfo);
	CRes_Character* pChar = NULL;
	Res_Mesh* pMesh = NULL;
	Res_CharTexture* pTexture = NULL;

	if (bVisualOk)
	{
		pChar = GetCharacter( pItemInfo->m_nMapCharID);
		if (pChar) pMesh = pChar->GetMesh( pItemInfo->m_nMapMeshType);
		if (pMesh) pTexture = pMesh->GetTexture( pItemInfo->m_nMapTextureType);
	}

	if (pChar == NULL || pMesh == NULL || pTexture == NULL)
	{
		pItemInfo->m_wVisualID = 10050; // Fallback to safe money pouch visual ID
		XiahItem::SetItemVisualData( pItemInfo);
		pChar = GetCharacter( pItemInfo->m_nMapCharID);
		pMesh = pChar ? pChar->GetMesh( pItemInfo->m_nMapMeshType) : NULL;
		pTexture = pMesh ? pMesh->GetTexture( pItemInfo->m_nMapTextureType) : NULL;
	}

	if( bCreateChar )
	{
		if ( pChar == NULL || pMesh == NULL || pTexture == NULL )
		{
			delete pItemInfo;
			pItemInfo = NULL;
			return TRUE;
		}
	}

	CXiahCharObject* pObject = NULL;
	if( bCreateChar )
	{
		pObject = new CXiahCharObject;
	}
	else
	{
		pObject = reinterpret_cast<CXiahCharObject*>(pXiahObject->m_pObject);
		
		//YS_0811 : BUGFIX
		if ( !pObject )
			return TRUE;
	}

	pObject->Create( pItemInfo->m_nMapCharID, pItemInfo->m_nMapMeshType, pItemInfo->m_nMapTextureType, -1);

	pObject->m_pPrivateData = (DWORD)pItemInfo;
	pObject->m_PrivateDataDestoryer = XiahItem::ReleaseItemInfo; // 앗싸
	// CG_2005/01/28 : 변종아이템기능추가
	pObject->m_bChangeItemSet = bChangeItem;

	pObject->SetPosition( wPosX, wPosY);
	if(bItemType == ITEMTYPE_MONEY && pItemInfo->m_bItemKind != 1)
	{
		TCHAR strTemp[64] = {0,};
		_stprintf(strTemp, "%d%s", dwAmount, JUN_MONEY);
		pObject->m_szObjectName = strTemp;
	}
	else
	{
		pObject->m_szObjectName = pItemInfo->m_szName;
	}

	pObject->m_bObjType = OBJTYPE_ITEM;	

	if( bCreateChar )
        /*XiahObject::CXiahObject* pXiahObject =*/ XiahObject::g_XiahObjectManager.CreateXiahObject( dwObjectID,OBJTYPE_ITEM, pObject);

	return TRUE;
}

/**
 * 행낭에서 제거
 * \param &msg 
 * \return 
 */
int OnCS_IM_REMOVEFROMSACK_ACK(CMsg &msg)
{
	BYTE bSackID	=0;
	BYTE bSackPos	=0;
	BYTE bReson		=0;

	msg
		>> bSackID
		>> bSackPos
		>> bReson;
	
	if( g_MainCharInfo.m_pHoldItem->IsHoldingItemItem())
	{
		g_MainCharInfo.m_pHoldItem->DeleteHoldItemItem();
	}
	else if( bSackID == SACKTYPE__DEFAULT || bSackID == SACKTYPE__DEFAULT2 || bSackID == 3)
	{		
		// 누수 수정
		g_MainCharInfo.m_pMySack[ bSackID -1]->DeleteItem( bSackPos, true);
	}
	else if( bSackID == SACKTYPE__EQUIPMENT)
	{		
		XiahItem::sItemInfo* pItemInfo = g_MainCharInfo.m_pEquipSack->FindSackItemByPos(bSackPos);

		if(pItemInfo)
		{
			if(pItemInfo->m_bItemType == ITEMTYPE_SOCKET)
				g_MainCharInfo.m_bPortalMove = true;

			// 누수 수정
			g_MainCharInfo.m_pEquipSack->DeleteItem( bSackPos, true);
		}		
	}
	else if(bSackID == SACKTYPE_COLLECTION)
	{
		// 누수 수정
		g_MainCharInfo.m_pCollection->DeleteItem(bSackPos, true);
	}

	switch(bReson)
	{
	case 7:		// 내구도 소실깨짐
		{
			g_MainCharInfo.ShowHelpMessage(IDS_ITEM_DESTRYCTION_1, TEXTEFFECT_COLOR_WARNING);
		}
		break;
	case 13:	// 투척무강에 의해 깨짐
		{
			g_MainCharInfo.ShowHelpMessage(IDS_ITEM_DESTRYCTION_2, TEXTEFFECT_COLOR_WARNING);
		}
		break;
	case 15:	// 보험가입중인 아이템 깨짐
		{
			g_MainCharInfo.ShowHelpMessage(IDS_ITEM_DESTRYCTION_3, TEXTEFFECT_COLOR_WARNING);
		}
	    break;
	case 20:	//HT_0914 : 기연창 및 낭 아이템 개선 사항 (낭 아이템 파손)
		{
			g_MainCharInfo.ShowHelpMessage(IDS_ITEM_DESTRYCTION_4, TEXTEFFECT_COLOR_WARNING);
		}
		break;
	case 21:	//HT_0410 : 신규 빙정류 추가 (빙정류 파손)
		{
			g_MainCharInfo.ShowHelpMessage(IDS_ITEM_DESTRYCTION_5, TEXTEFFECT_COLOR_WARNING);
		}
		break;
	default:
		break;
	}

	return 1;
}

int OnCS_IM_ADDONSACK_ACK(CMsg &msg)
{
	BYTE bSackID	=0;
	BYTE bSackPos	=0;
	
	XiahItem::sItemInfo* pItem = new XiahItem::sItemInfo;

	msg
		>> bSackID
		>> bSackPos;
	XiahItem::GetItemData( pItem, msg);
	
	//HT_1116 : 각성자 아이템 추가
	msg
		>> pItem->m_wRebuithValue;

	if( bSackID == SACKTYPE__DEFAULT || bSackID == SACKTYPE__DEFAULT2)
	{
		pItem->m_bSackID = SACKTYPE__DEFAULT;
		//g_MainCharInfo.m_pMySack[g_MainCharInfo.m_byMySackCurrIdx]->InsertItem( bSackPos, pItem);
		pItem->m_bSackCount = bSackID -1;
		g_MainCharInfo.m_pMySack[ bSackID -1]->InsertItem( bSackPos, pItem);
	}
	else if( bSackID == SACKTYPE__EQUIPMENT)
	{
		pItem->m_bSackID = SACKTYPE__EQUIPMENT;
		bSackPos = bSackPos - MAX_SACK_ITEM;
		g_MainCharInfo.m_pEquipSack->InsertItem( bSackPos, pItem);
	}

	g_MainCharInfo.RefreshItemFrame();

	return TRUE;
}

int OnCS_IM_MOVE_ACK( CMsg &msg)
{
	BYTE bResult		=0;
	BYTE bSrcSackID		=0;
	BYTE bSrcSackPos	=0;
	BYTE bDesSackID		=0;
	BYTE bDesSackPos	=0;

	msg
		>> bResult
		>> bSrcSackID
		>> bSrcSackPos
		>> bDesSackID
		>> bDesSackPos;

	// 실패 떨어지면
	if(bResult != 0) 
	{
		switch( bResult)
		{
		case 1:	// 해당 아이템 없음
			g_MainCharInfo.ShowHelpMessage( IDS_NO_SUCH_ITEM, TEXTEFFECT_COLOR_WARNING);
			break;
		case 2:	// 이동 가능 공간 없음
			g_MainCharInfo.ShowHelpMessage( IDS_NO_MOVEABLE_SPACE, TEXTEFFECT_COLOR_WARNING);
			break;
		case 3:	// 능력 부족
			g_MainCharInfo.ShowHelpMessage( IDS_SHORT_ABLE, TEXTEFFECT_COLOR_WARNING);
			break;
		case 4:	// 디비 댄轎
			g_MainCharInfo.ShowHelpMessage(IDS_REL_ERR_INTERNAL, TEXTEFFECT_COLOR_WARNING);
			break;
		case 10:
			g_MainCharInfo.ShowHelpMessage(IDS_NOTMOVERUNEVENT, TEXTEFFECT_COLOR_WARNING);
			break;
		case 11:
			g_MainCharInfo.ShowHelpMessage(IDS_NOTMOVEOVERTIMEEVENT);
			break;
		case 12:
			g_MainCharInfo.ShowHelpMessage(IDS_NOTMOVESAMEEVENTRUN);
			break;
		case 13:
			g_MainCharInfo.ShowHelpMessage(IDS_NOTMOVEDONOTMUNPA);
			break;
		case 14:
			g_MainCharInfo.ShowHelpMessage(IDS_ERROR1);
			break;
		case 15:
			g_MainCharInfo.ShowHelpMessage(IDS_NOTMOVEOVERMAXEVENT);
			break;
		case 16:
			g_MainCharInfo.ShowHelpMessage(IDS_NOTMOVECHANGEEVENTITEM);
			break;
		case 17:	// 거래중인 아이템은 이동할수 없습니다.
			g_MainCharInfo.ShowHelpMessage(IDS_NOTMOVE_TRADETITEM, TEXTEFFECT_COLOR_WARNING);
			break;
			// CG_2005/01/28 : 변종아이템기능추가
		case ERR_CITEM_NOTINSAMECHARTYPE:	//변종 옷과 모자는 동일 계열 일 경우만 장착 가능함.
			g_MainCharInfo.ShowHelpMessage( IDS_NOTINSAMECHARTYPE, TEXTEFFECT_COLOR_WARNING);
			break;
		case ERR_CITEM_NOTOUTSAMECHARTYPE:	//변종 옷에 대하여 탈착 할 경우 변종 모자부터 탈착 해야 함.
			g_MainCharInfo.ShowHelpMessage( IDS_NOTOUTSAMECHARTYPE, TEXTEFFECT_COLOR_WARNING);
			break;
		case 21:	//광명전 & 천황전 착용 금지 
			g_MainCharInfo.ShowHelpMessage( IDS_SECRETNOTUSE, TEXTEFFECT_COLOR_WARNING);
			break;
		}

		g_MainCharInfo.m_pHoldItem->SetItemBackToSack();

		g_MainCharInfo.PlayInterfaceSound( ISOUND_WARNING);

		return FALSE;
	}
	else
	{
		// 성공 하면
		// client에서는 두개를 같은 타입으로 인식시켜야한다.
		if( bSrcSackID == SACKTYPE__DEFAULT2 || bSrcSackID == 3)
			bSrcSackID = SACKTYPE__DEFAULT;
		if( bDesSackID == SACKTYPE__DEFAULT2 || bDesSackID == 3)
			bDesSackID = SACKTYPE__DEFAULT;

		XiahItem::sItemInfo* pDesItem	= NULL;

		if( bDesSackID == SACKTYPE__DEFAULT)
		{
			pDesItem = g_MainCharInfo.m_pMySack[g_MainCharInfo.m_byMySackCurrIdx]->FindSackItemByPos( bDesSackPos);
		}
		else if( bDesSackID == SACKTYPE__EQUIPMENT)
		{
			pDesItem = g_MainCharInfo.m_pEquipSack->FindSackItemByPos( bDesSackPos);
		}

		XiahItem::sItemInfo* pHoldItem = g_MainCharInfo.m_pHoldItem->GetHoldItemItem();

		// 음.. 여기서 holditem없으면 안되는데 -_-ㅋ
		if( !pHoldItem)
			return FALSE;

		if( bSrcSackID == SACKTYPE__EQUIPMENT && bDesSackID == SACKTYPE__EQUIPMENT)
		{
			// item switch			
			if( pDesItem)
			{				
				g_MainCharInfo.m_pEquipSack->DeleteItem(bDesSackPos);

				pDesItem->m_bSackID = SACKTYPE__EQUIPMENT;
				pDesItem->m_bSackPos = bSrcSackPos;
				g_MainCharInfo.m_pEquipSack->InsertItem( bSrcSackPos, pDesItem);				
			}

			pHoldItem->m_bSackID = SACKTYPE__EQUIPMENT;
			g_MainCharInfo.m_pEquipSack->InsertItem( bDesSackPos, pHoldItem);
		}
		else if( bSrcSackID == SACKTYPE__DEFAULT && bDesSackID == SACKTYPE__EQUIPMENT)
		{
			// item switch
			if( pDesItem)
			{
				g_MainCharInfo.m_pEquipSack->DeleteItem( bDesSackPos);

				pDesItem->m_bSackID = SACKTYPE__DEFAULT;
				pDesItem->m_bSackPos = bSrcSackPos;
				pDesItem->m_bSackCount = pHoldItem->m_bSackCount;
				
				g_MainCharInfo.m_pMySack[ pHoldItem->m_bSackCount]->InsertItem( bSrcSackPos, pDesItem);
			}

			pHoldItem->m_bSackID = SACKTYPE__EQUIPMENT;
			g_MainCharInfo.m_pEquipSack->InsertItem( bDesSackPos, pHoldItem);

			if(pHoldItem->m_bItemType == ITEMTYPE_SOCKET)
				g_MainCharInfo.m_bPortalMove = false;
		}		
		else if( bSrcSackID == SACKTYPE__EQUIPMENT && bDesSackID == SACKTYPE__DEFAULT)
		{
			pHoldItem->m_bSackID = SACKTYPE__DEFAULT;
			pHoldItem->m_bSackCount = g_MainCharInfo.m_byMySackCurrIdx;
			g_MainCharInfo.m_pMySack[g_MainCharInfo.m_byMySackCurrIdx]->InsertItem( bDesSackPos, pHoldItem);
		}
		else if( bSrcSackID == SACKTYPE__DEFAULT && bDesSackID == SACKTYPE__DEFAULT)
		{
			//switch
			if( pDesItem)
			{
				g_MainCharInfo.m_pMySack[g_MainCharInfo.m_byMySackCurrIdx]->DeleteItem( bDesSackPos);

				pDesItem->m_bSackID = SACKTYPE__DEFAULT;
				pDesItem->m_bSackPos = bSrcSackPos;
				pDesItem->m_bSackCount = pHoldItem->m_bSackCount;
				
				g_MainCharInfo.m_pMySack[ pHoldItem->m_bSackCount]->InsertItem( bSrcSackPos, pDesItem);
			}
			else
			{
				XiahItem::sItemInfo* pConvertItem = g_MainCharInfo.m_pModifySack->FindSackItemByPosPrev( bDesSackPos);
				if( pConvertItem)
				{
					pConvertItem->m_bSackPosPrev = bSrcSackPos;
					pConvertItem->m_bSackCount = pHoldItem->m_bSackCount;
				}
			}
			
			pHoldItem->m_bSackID = SACKTYPE__DEFAULT;
			pHoldItem->m_bSackCount = g_MainCharInfo.m_byMySackCurrIdx;
			g_MainCharInfo.m_pMySack[g_MainCharInfo.m_byMySackCurrIdx]->InsertItem( bDesSackPos, pHoldItem);
		}

		g_MainCharInfo.m_pHoldItem->EmptyHoldItemItem();

		g_MainCharInfo.PlayInterfaceSound( ISOUND_ITEM_LAY_SACK);

		return TRUE;
	}
	
	return TRUE;
}

/**
 * 무공서 읽기
 * \param &msg 
 * \return 
 */
int OnCS_IM_READRESULT_ACK( CMsg &msg)
{
	BYTE bResult = 0;
	sString szMugongBookName;

	msg
		>> bResult
		>> szMugongBookName;

	switch(bResult)
	{
	case 1:
		g_MainCharInfo.ShowHelpMessage(IDS_SHORT_DEX1, TEXTEFFECT_COLOR_WARNING);
		break;
	case 2:
		g_MainCharInfo.ShowHelpMessage(IDS_SHORT_PWR1, TEXTEFFECT_COLOR_WARNING);
		break;
	case 3:
		g_MainCharInfo.ShowHelpMessage(IDS_SHORT_AGI1, TEXTEFFECT_COLOR_WARNING);
		break;
	case 4:
		g_MainCharInfo.ShowHelpMessage(IDS_SHORT_LIFE1, TEXTEFFECT_COLOR_WARNING);
		break;
	case 5:
		g_MainCharInfo.ShowHelpMessage(IDS_SHORT_LEVEL, TEXTEFFECT_COLOR_WARNING);
		break;
	case 6:
		g_MainCharInfo.ShowHelpMessage(IDS_LERNED_MUGONG, TEXTEFFECT_COLOR_WARNING);
		break;
	case 7:
		g_MainCharInfo.ShowHelpMessage(IDS_SHORT_MUGONGLEVEL, TEXTEFFECT_COLOR_WARNING);
		break;
	case 8:
		g_MainCharInfo.ShowHelpMessage(IDS_SHORT_CLASS, TEXTEFFECT_COLOR_WARNING);
		break;
	case 9:
		g_MainCharInfo.ShowHelpMessage(IDS_SHORT_TP1, TEXTEFFECT_COLOR_WARNING);
		break;
	case 10:
		g_MainCharInfo.ShowHelpMessage(IDS_FIVEELEMENT_EXPLACK, TEXTEFFECT_COLOR_WARNING);		
		break;
	case 11:	// 악명등급이 5 감소
		g_MainCharInfo.ShowHelpMessage(IDS_FAME_DIMINUTION);		
		break;
	case 12:	// 명성등급 사용 불가
		g_MainCharInfo.ShowHelpMessage(IDS_FAME_NOT, TEXTEFFECT_COLOR_WARNING);		
		break;
	case 13:	// 성인서버에서만 사용가능
		g_MainCharInfo.ShowHelpMessage(IDS_ADULT_ITEM, TEXTEFFECT_COLOR_WARNING);		
		break;
	case 14:
		g_MainCharInfo.ShowHelpMessage( IDS_REBIRTH_MUGONG_BOOK, TEXTEFFECT_COLOR_WARNING);
		break;
	case 15:
		g_MainCharInfo.ShowHelpMessage( IDS_REBIRTH_MUGONG_BOOK_02, TEXTEFFECT_COLOR_WARNING);
		g_MainCharInfo.ShowHelpMessage( IDS_REBIRTH_MUGONG_BOOK_03, TEXTEFFECT_COLOR_WARNING);
		break;
	case 16:
		g_MainCharInfo.ShowHelpMessage( IDS_REBIRTH_MUGONG_BOOK_04, TEXTEFFECT_COLOR_WARNING);
		g_MainCharInfo.ShowHelpMessage( IDS_REBIRTH_MUGONG_BOOK_05, TEXTEFFECT_COLOR_WARNING);
		break;
	case 17: // 무공 레벨이 0 일때 중서나 고서를 익힐려구 할 경우
		g_MainCharInfo.ShowHelpMessage( IDS_NO_MUGONGLEVEL, TEXTEFFECT_COLOR_WARNING);
		break;
	case 18: // 진각성자만 익힐 수 있다.
		g_MainCharInfo.ShowHelpMessage( IDS_2TH_REBIRTH_MUGONG_BOOK, TEXTEFFECT_COLOR_WARNING);
		break;
	case 19: // 진각성 차수가 모자를 경우
		g_MainCharInfo.ShowHelpMessage( IDS_2TH_REBIRTH_MUGONG_SHORT, TEXTEFFECT_COLOR_WARNING);
		break;
	case 20: // 흠성 신공을 2개 이상 배웠을 경우
		g_MainCharInfo.ShowHelpMessage( IDS_2TH_REBIRTH_MUGONG_MORE, TEXTEFFECT_COLOR_WARNING);
		break;
    }

	g_MainCharInfo.RefreshMugongFrame();	

	return TRUE;
}

int OnCS_IM_CHANGERES_ACK( CMsg &msg)
{
	BYTE bSackID		=0;
	BYTE bSackPos		=0;	
	WORD wAmount		=0;
	DWORD dwObjectID	=0;

	msg
		>> bSackID
		>> bSackPos
		>> dwObjectID
		>> wAmount;

	// ^^ 보류
	//if( g_MainCharInfo.m_pHoldItem->IsHoldingItemItem())
	//	g_MainCharInfo.m_pHoldItem->SetItemBackToSack();

	if( g_MainCharInfo.m_pMySack[ bSackID-1])
	{
		// desItem의 amount 변경
		XiahItem::sItemInfo* pInfo = g_MainCharInfo.m_pMySack[ bSackID-1]->FindSackItemByPos( bSackPos);
		if( pInfo)
			pInfo->m_dwAmount = wAmount;
		
		g_MainCharInfo.m_pMySack[ bSackID-1]->SetRefreshToolTip( TRUE);
	}

	return 0;
}
/*
int OnCS_IM_CHECKITEMPRICE_ACK( CMsg &msg)
{
	BYTE bResult;
	BYTE bType;
	DWORD dwItemID;
	DWORD dwPrice;

	msg
		>> bResult
		>> bType
		>> dwItemID
		>> dwPrice;

	if( dwPrice > g_MainCharInfo.m_dwMoney)
		g_MainCharInfo.ShowHelpMessage( IDS_SHORT_MONEY, TEXTEFFECT_COLOR_GENERAL);
	else
	{
		TCHAR content[100];
		_stprintf( content, IDS_D_ASK_BOGUAN, dwPrice);
		g_pUIManager->ShowNotice( content, NOTICE_FRAME_OKCANCEL, NOTICE_FRAME_CHECKPRICE);
	}

	return 0;
}
*/
int OnCS_IM_DURABILITY_ACK( CMsg &msg)
{
	DWORD dwItemID;
	BYTE bItemType;
	BYTE bSackID;
	BYTE bSackPos;
	WORD wCurDur;

	msg
		>> dwItemID
		>> bItemType
		>> bSackID
		>> bSackPos
		>> wCurDur;

	XiahItem::sItemInfo* pInfo = NULL;

	if( bSackID == SACKTYPE__DEFAULT || bSackID == SACKTYPE__DEFAULT2 || bSackID == 3)
	{
		pInfo = g_MainCharInfo.m_pMySack[g_MainCharInfo.m_byMySackCurrIdx]->FindSackItemByID( dwItemID);

		if( pInfo)
		{
			pInfo->m_wCurDur = wCurDur;
			g_MainCharInfo.m_pMySack[g_MainCharInfo.m_byMySackCurrIdx]->SetRefreshToolTip( TRUE);
		}	
	}
	else if( bSackID == SACKTYPE__EQUIPMENT)
	{
		pInfo = g_MainCharInfo.m_pEquipSack->FindSackItemByID( dwItemID);
		if( pInfo)
		{
			pInfo->m_wCurDur = wCurDur;
			g_MainCharInfo.m_pEquipSack->SetRefreshToolTip( TRUE);
		}	
	}

	return 0;
}

/**
 * 아이템수리
 * \param &msg 
 * \return 
 */
int OnCS_IM_REPAIRITEM_ACK( CMsg &msg)
{
	BYTE bResult	=0;
	DWORD dwMoney	=0;
	
	msg
		>> bResult
		>> dwMoney;

	switch(bResult)
	{
		case 0:	// 성공
			{
				TCHAR content[50] = {0,};
				_stprintf(content, IDS_REPAIR_COMPLETE_PAY, dwMoney);

				g_MainCharInfo.ShowHelpMessage(content);
				g_MainCharInfo.PlayInterfaceSound(ITEM_REPAIR_SOUND);

				if(g_MainCharInfo.m_ReairSackID == 0)
				{
					XiahItem::sItemInfo* pItemInfo = g_MainCharInfo.m_pEquipSack->FindSackItemByPos(g_MainCharInfo.m_RpairItemPos);

					if(pItemInfo)
						pItemInfo->m_wPrev = 100;
				}
			}
			break;
		case 1:
			{
				g_MainCharInfo.ShowHelpMessage(IDS_ERROR_NO_ITEM, TEXTEFFECT_COLOR_WARNING);
				g_MainCharInfo.m_RpairItemPos = 0;
			}			
			break;
		case 2:
			{
				g_MainCharInfo.ShowHelpMessage( IDS_SHORT_MONEY, TEXTEFFECT_COLOR_WARNING);
				g_MainCharInfo.m_RpairItemPos = 0;
			}			
			break;
		case 3:
			{
				g_MainCharInfo.ShowHelpMessage( IDS_REPAIR_COMPLETE, TEXTEFFECT_COLOR_WARNING);
			}			
			break;
		case 4:
			{
				TCHAR str[50] = {0,};
				_stprintf( str, IDS_ERROR, bResult);
				g_MainCharInfo.ShowHelpMessage(str, TEXTEFFECT_COLOR_WARNING);
				g_MainCharInfo.HideSack( SACKTYPE__MODIFY);

				g_MainCharInfo.m_RpairItemPos = 0;
			}
			break;
		case ERR_REPARIITEM_NOTREPAIR:
			{
				g_MainCharInfo.ShowHelpMessage(IDS_REPAIRWITHITEM_NOTREPAIR, TEXTEFFECT_COLOR_WARNING);
			}			
			break;
		case ERR_REPARIITEM_NOTREPAIRITEM:
			{
				g_MainCharInfo.ShowHelpMessage(IDS_REPARIITEM_NOTREPAIRITEM, TEXTEFFECT_COLOR_WARNING);			
			}			
			break;
			// CG_2005/01/28 : 변종아이템기능추가
		case ERR_REPARIITEM_NOTCITEM_INEQUIP:	//장착 중인 변종아이템은 수리 불가 
			g_MainCharInfo.ShowHelpMessage( IDS_REPARIITEM_NOTCITEM_INEQUIP, TEXTEFFECT_COLOR_WARNING ); 
			break;
		case ERR_REPARIITEM_REBACKITEM:			//변종아이템 수리가능 횟수 초과로 인하여 원아이템으로 복원 되었습니다
			g_MainCharInfo.ShowHelpMessage( IDS_REPARIITEM_REBACKITEM, TEXTEFFECT_COLOR_WARNING ); 
			break;	
		case ERR_REPARIITEM_BONGINITEM:			//HT_0410 : 신규 빙정류 추가
			g_MainCharInfo.ShowHelpMessage( IDS_REPARIITEM_NOTREPAIRITEM, TEXTEFFECT_COLOR_WARNING ); 
			break;	

	} // switch(bResult)

	return 0;
}

int OnCS_IM_REPAIRWITHITEM_ACK(CMsg &msg)
{
	BYTE bResult = -1;
	DWORD dwMoney = 0;

	msg
		>> bResult
		>> dwMoney;

	switch(bResult)
	{
		case ERR_REPAIRWITHITEM_SUCCESS:
			{
				TCHAR content[50] = {0,};
				_stprintf( content, IDS_REPAIR_COMPLETE_PAY, dwMoney);

				g_MainCharInfo.ShowHelpMessage( content);
				g_MainCharInfo.PlayInterfaceSound(ITEM_REPAIR_SOUND);

				// [3/19/2004] 장착 아이템은 나중에 해주자
			}
			break;
		case ERR_REPAIRWITHITEM_NOTFOUND:
			g_MainCharInfo.ShowHelpMessage( IDS_ERROR_NO_ITEM, TEXTEFFECT_COLOR_WARNING);
			break;
		case ERR_REPAIRWITHITEM_NOTFOUNDRES:
			g_MainCharInfo.ShowHelpMessage( IDS_ERROR_NO_ITEM, TEXTEFFECT_COLOR_WARNING);
			break;
		case ERR_REPAIRWITHITEM_INVALIDRES:
			break;
		case ERR_REPAIRWITHITEM_NOMONEY:
			g_MainCharInfo.ShowHelpMessage( IDS_SHORT_MONEY, TEXTEFFECT_COLOR_WARNING);
			break;
		case ERR_REPAIRWITHITEM_FULLDURABILITY:
			break;
		case ERR_REPAIRWITHITEM_DBOPEN:
			break;
		case ERR_REPAIRWITHITEM_FAIL:
			break;
		case ERR_REPAIRWITHITEM_NOTREPAIR:
			g_MainCharInfo.ShowHelpMessage(IDS_REPAIRWITHITEM_NOTREPAIR, TEXTEFFECT_COLOR_WARNING);
			break;
		case ERR_REPARIITEM_NOTREPAIRITEM:
			g_MainCharInfo.ShowHelpMessage(IDS_REPARIITEM_NOTREPAIRITEM, TEXTEFFECT_COLOR_WARNING);
			break;
			// CG_2005/01/28 : 변종아이템기능추가
		case ERR_REPARIITEM_NOTCITEM_INEQUIP:	//장착 중인 변종아이템은 수리 불가 
			g_MainCharInfo.ShowHelpMessage( IDS_REPARIITEM_NOTCITEM_INEQUIP, TEXTEFFECT_COLOR_WARNING); 
			break;
		case ERR_REPARIITEM_REBACKITEM:			//변종아이템 수리가능 횟수 초과로 인하여 원아이템으로 복원 되었습니다
			g_MainCharInfo.ShowHelpMessage( IDS_REPARIITEM_REBACKITEM, TEXTEFFECT_COLOR_WARNING); 
			break;
		case ERR_REPARIITEM_BONGINITEM:			//HT_0410 : 신규 빙정류 추가
			g_MainCharInfo.ShowHelpMessage( IDS_REPARIITEM_NOTREPAIRITEM, TEXTEFFECT_COLOR_WARNING ); 
			break;	
	}

	return 0;
}

int OnCS_IM_REMARKITEM_ACK(CMsg &msg)
{
	BYTE bResult = -1;

	msg	
		>> bResult;		

	switch(bResult)
	{
	case ERR_REMARKITEM_SUCCESS:
		g_MainCharInfo.ShowHelpMessage(IDS_MARKITEM_SUCCESS);
		break;
	case ERR_REMARKITEM_NOTFOUND:
		g_MainCharInfo.ShowHelpMessage(IDS_ERROR_NO_ITEM, 2);
		break;
	case ERR_REMARKITEM_INVALIDITEM:
		g_MainCharInfo.ShowHelpMessage(IDS_ERROR_NO_ITEM, 2);
		break;
	case ERR_REMARKITEM_INVALIDPOSITION:
		g_MainCharInfo.ShowHelpMessage(IDS_MARKITEM_ERROR, 2);
		break;
	case ERR_REMARKITEM_FAIL:
		g_MainCharInfo.ShowHelpMessage(IDS_MARKITEM_ERROR, 2);
		break;
	}

	return 0;
}

/**
 * 아이템 개조/제련
 * \param &msg 
 * \return 
 */
int OnCS_IM_REBUILDITEMTERM_ACK( CMsg &msg)
{
	BYTE bResult =0;

	msg
		>> bResult;

	if( bResult != 0)
	{
		switch( bResult)
		{
		case ERR_REBUILDITEMTERM_NOTFOUNDITEM:
			g_MainCharInfo.ShowHelpMessage( IDS_NO_SUCH_ITEM, TEXTEFFECT_COLOR_WARNING);
			break;
		case ERR_REBUILDITEMTREM_NOTBASICITEM:
			g_MainCharInfo.ShowHelpMessage( IDS_CONVERTRESOURCE_DIABLE, TEXTEFFECT_COLOR_WARNING);
			break;
		case ERR_REBUILDITEMTREM_NOTREBUILDITEM:
			g_MainCharInfo.ShowHelpMessage( IDS_CONVERT_DISABLE, TEXTEFFECT_COLOR_WARNING);
			break;
		default:
			{
				TCHAR str[50] = {0,};
				_stprintf( str, IDS_ERROR, bResult);
				g_MainCharInfo.ShowHelpMessage(str, TEXTEFFECT_COLOR_WARNING);
			}
			break;
		}

		if( g_MainCharInfo.m_pHoldItem->IsHoldingItemItem())
			g_MainCharInfo.m_pHoldItem->SetItemBackToSack();
	}
	else
	{
		if( g_MainCharInfo.m_pHoldItem->IsHoldingItemItem())
		{
			// 뭔가가 있으면 원래 위치로 돌려주고
			XiahItem::sItemInfo* pItem = g_MainCharInfo.m_pModifySack->FindSackItemByPos( 0);
			if(pItem)
			{
				pItem->m_bSackID = SACKTYPE__DEFAULT;
				g_MainCharInfo.m_pMySack[pItem->m_bSackIDPrev]->InsertItem(pItem->m_bSackPosPrev, pItem);

				if(g_pUIManager->IsShow(WINDOW_FIVEELEMENTS_CONVERT))
				{
					if(g_MainCharInfo.m_pFEConvert)
					{
						g_MainCharInfo.m_pFEConvert->DeleteItem(0);
					}
				}
				else
				{
					g_MainCharInfo.m_pModifySack->DeleteItem(0);
				}				
			}

			// 홀드아이템 넣어주기
			g_MainCharInfo.m_pHoldItem->GetHoldItemItem()->m_bSackID = SACKTYPE__MODIFY;
			g_MainCharInfo.m_pHoldItem->GetHoldItemItem()->m_bSackIDPrev = g_MainCharInfo.m_pHoldItem->GetHoldItemItem()->m_bSackCount;
			g_MainCharInfo.m_pHoldItem->GetHoldItemItem()->m_bSackPosPrev = g_MainCharInfo.m_pHoldItem->GetHoldItemItem()->m_bSackPos;

			if(g_pUIManager->IsShow(WINDOW_FIVEELEMENTS_CONVERT))
			{
				if(g_MainCharInfo.m_pFEConvert)
				{
					g_MainCharInfo.m_pFEConvert->InsertItem(0, g_MainCharInfo.m_pHoldItem->GetHoldItemItem());
				}
			}
			else
			{
				g_MainCharInfo.m_pModifySack->InsertItem( 0, g_MainCharInfo.m_pHoldItem->GetHoldItemItem());
			}
			
			g_MainCharInfo.m_pHoldItem->EmptyHoldItemItem();
		}
	}

	g_MainCharInfo.m_bInteractionFlag = FALSE;

	return 0;
}

/**
 * 개조/제련
 * \param &msg 
 * \return 
 */
int OnCS_IM_REBUILDITEM_ACK(CMsg &msg)
{
	BYTE bResult	=0;
	BYTE bItemnum	=0;
	BYTE bFactor	=0;
	int	nValues		=0;

	sString	szItemname;

	msg
		>> bResult
		>> bFactor
		>> nValues
		>> szItemname
		>> bItemnum;

	bool bFEShow = g_pUIManager->IsShow(WINDOW_FIVEELEMENTS_CONVERT);

	switch(bResult)
	{	
	case ERR_REBUILDITEM_SUCCESS:				// 개조 성공
		{
			if(bFEShow)
				g_MainCharInfo.ShowHelpMessage(IDS_FE_ITEMCONVERT_COMPLETE);
			else
				g_MainCharInfo.ShowHelpMessage(IDS_ITEMCONVERT_COMPLETE);

			if(bFactor != 255)
			{
				TCHAR strMsg[128] = {0,};

				if(bFactor == 14)
				{
					_stprintf(strMsg, IDS_FACTOR_15, nValues);
				}
				else if(bFactor == 15)
				{
					_stprintf(strMsg, IDS_FACTOR_16, nValues);
				}
				else
				{
					LPCTSTR lpstr = NULL;

					switch(bFactor)
					{
					case 0: lpstr = IDS_FACTOR_01;		break;
					case 1:	lpstr = IDS_FACTOR_02;		break;
					case 2:	lpstr = IDS_FACTOR_03;		break;
					case 3:	lpstr = IDS_FACTOR_04;		break;
					case 4:	lpstr = IDS_FACTOR_05;		break;
					case 5:	lpstr = IDS_FACTOR_06;		break;
					case 6:	lpstr = IDS_FACTOR_07;		break;
					case 7:	lpstr = IDS_FACTOR_08;		break;
					case 8:	lpstr = IDS_FACTOR_09;		break;
					case 9:	lpstr = IDS_FACTOR_10;		break;
					case 10:lpstr = IDS_FACTOR_11;		break;
					case 11:lpstr = IDS_FACTOR_12;		break;
					case 12:lpstr = IDS_FACTOR_13;		break;
					case 13:lpstr = IDS_FACTOR_14;		break;
					}

					LPCTSTR lpstrValue = NULL;

					if(nValues > 0)
						lpstrValue = IDS_FACTOR_UP;
					else
						lpstrValue = IDS_FACTOR_DOWN;
					
					_stprintf(strMsg, _T("%s %d %s"), lpstr, nValues, lpstrValue);
				}
				
				g_MainCharInfo.ShowHelpMessage(strMsg);
			}			
		}		
		break;
	case ERR_REBUILDITEM_NOMONEY:
		{
			g_MainCharInfo.ShowHelpMessage(IDS_SHORT_MONEY, TEXTEFFECT_COLOR_WARNING);
		}		
		break;
	case ERR_REBUILDITEM_NOTFOUNDITEM:
		{
			g_MainCharInfo.ShowHelpMessage(IDS_NO_SUCH_ITEM, TEXTEFFECT_COLOR_WARNING);
		}		
		break;
	case ERR_REBUILDITEM_NOTFOUNDRES:
		{
			if(bFEShow)
				g_MainCharInfo.ShowHelpMessage(IDS_FE_NO_CONVERT_RESOURCE, TEXTEFFECT_COLOR_WARNING);
			else
				g_MainCharInfo.ShowHelpMessage(IDS_NO_CONVERT_RESOURCE, TEXTEFFECT_COLOR_WARNING);
		}		
		break;
	case ERR_REBUILDITEM_DBOPEN:
		{
			TCHAR str[50] = {0,};
			_stprintf( str, IDS_ERROR, ERR_REBUILDITEM_DBOPEN);
			g_MainCharInfo.ShowHelpMessage(str, TEXTEFFECT_COLOR_WARNING);
		}
		break;
	case ERR_REBUILDITEM_FAIL:
		{	
			if(bFEShow)
				g_MainCharInfo.ShowHelpMessage(IDS_FE_FAIL_CONVERT, TEXTEFFECT_COLOR_WARNING);
			else
				g_MainCharInfo.ShowHelpMessage(IDS_FAIL_CONVERT, TEXTEFFECT_COLOR_WARNING);

			if(bFactor != 255 && bFactor !=15)
			{
				TCHAR strMsg[128] = {0,};

				if(bFactor == 14)
				{
					_stprintf(strMsg, IDS_FACTOR_15, nValues);
				}
	
				else
				{
					LPCTSTR lpstr = NULL;

					switch(bFactor)
					{
					case 0: lpstr = IDS_FACTOR_01;		break;
					case 1:	lpstr = IDS_FACTOR_02;		break;
					case 2:	lpstr = IDS_FACTOR_03;		break;
					case 3:	lpstr = IDS_FACTOR_04;		break;
					case 4:	lpstr = IDS_FACTOR_05;		break;
					case 5:	lpstr = IDS_FACTOR_06;		break;
					case 6:	lpstr = IDS_FACTOR_07;		break;
					case 7:	lpstr = IDS_FACTOR_08;		break;
					case 8:	lpstr = IDS_FACTOR_09;		break;
					case 9:	lpstr = IDS_FACTOR_10;		break;
					case 10:lpstr = IDS_FACTOR_11;		break;
					case 11:lpstr = IDS_FACTOR_12;		break;
					case 12:lpstr = IDS_FACTOR_13;		break;
					case 13:lpstr = IDS_FACTOR_14;		break;
					}

					LPCTSTR lpstrValue = NULL;

					if(nValues > 0)
						lpstrValue = IDS_FACTOR_UP;
					else
						lpstrValue = IDS_FACTOR_DOWN;

					_stprintf(strMsg, _T("%s %d %s"), lpstr, nValues, lpstrValue);
				}

				g_MainCharInfo.ShowHelpMessage(strMsg);
			}
		}
		break;
	case ERR_REBUILDITEM_FULL:
		{
			if(bFEShow)
				g_MainCharInfo.ShowHelpMessage(IDS_FE_CANT_MODIFY, TEXTEFFECT_COLOR_WARNING);
			else
				g_MainCharInfo.ShowHelpMessage(IM_CANT_MODIFY, TEXTEFFECT_COLOR_WARNING);
		}
		break;

	case ERR_REBUILDITEM_CANNOTTREBUILD:		//개조불가
		{
			if(bFEShow)
				g_MainCharInfo.ShowHelpMessage(IDS_FE_NO_CONVERT, TEXTEFFECT_COLOR_WARNING);
			else
				g_MainCharInfo.ShowHelpMessage(IDS_D_NO_CONVERT, TEXTEFFECT_COLOR_WARNING);
		}
		break;

	case ERR_MELTINGITEM_CANNOTTMELTING:		//NIGHT_0512 : MELTING  추출불가아이템
		{
			g_MainCharInfo.ShowHelpMessage(IDS_CANNOTTMELTING);
		}
		break;

	case ERR_MELTINGITEM_NEEDEMPTYSACK:			//NIGHT_0512 : MELTING	행낭공간부족
		{
			g_MainCharInfo.ShowHelpMessage(IDS_FULL_SACK);
		}
		break;

	case ERR_MELTINGITEM_SUCCESS:				//NIGHT_0512 : MELTING	추출성공
		{
			TCHAR str[50] = {0,};
			_stprintf( str, IDS_MELTING_REPORT, szItemname.data(), bItemnum);
			g_MainCharInfo.ShowHelpMessage(str);
			g_MainCharInfo.HideSack(SACKTYPE__MODIFY, TRUE);
		}
		break;

	case ERR_MELTINGITEM_FAIL:					//NIGHT_0512 : MELTING	추출실패
		{
			g_MainCharInfo.ShowHelpMessage(IDS_MELTINGITEM_FAIL);
		}
		break;
	case ERR_REBUILDITEM_NOTSLOTITEM:		//기공(구멍)을 뚫을수 없는 아이템
		{
			g_MainCharInfo.ShowHelpMessage(IDS_REBUILDITEM_NOTSLOTITEM, TEXTEFFECT_COLOR_WARNING);
		}
		break;
	case ERR_REBUILDITEM_MAXSLOT:			//기공을 더이상 뚫을 수 없다.
		{
			g_MainCharInfo.ShowHelpMessage(IDS_REBUILDITEM_MAXSLOT, TEXTEFFECT_COLOR_WARNING);
		}
		break;
	case ERR_REBUILDITEM_NOTFOUNDSLOT:		//기공이 없다
		{
			g_MainCharInfo.ShowHelpMessage(IDS_REBUILDITEM_NOTFOUNDSLOT, TEXTEFFECT_COLOR_WARNING);
		}
		break;
	case ERR_REBUILDITEM_FULLSLOTFIVEELM:	//비어있는 기공이 없다.
		{
			g_MainCharInfo.ShowHelpMessage(IDS_REBUILDITEM_FULLSLOTFIVEELM, TEXTEFFECT_COLOR_WARNING);
		}
		break;
	case ERR_REBUILDITEM_NOTFOUNDFIVEELM:	//오행속성이 없다(오행속성제거시)
		{
			g_MainCharInfo.ShowHelpMessage(IDS_REBUILDITEM_NOTFOUNDFIVEELM, TEXTEFFECT_COLOR_WARNING);
		}
		break;
	case ERR_REBUILDITEM_MADESLOTFAIL:		//기공을 뚫기 실패(+,성,제한요구치 감소시킨다)
		{
			g_MainCharInfo.ShowHelpMessage(IDS_REBUILDITEM_MADESLOTFAIL, TEXTEFFECT_COLOR_WARNING);
		}
		break;

		// CG_2005/01/28 : 변종아이템기능추가
	case ERR_CHANGEITEM_SUCCESS:
		{
			g_MainCharInfo.ShowHelpMessage( IDS_CHANGEITEM_SUCCESS );
			g_MainCharInfo.HideSack(SACKTYPE__MODIFY, TRUE);
		}
		break;
	case ERR_CHANGEITEM_CHANGED:
		{
			g_MainCharInfo.ShowHelpMessage( IDS_CHANGEITEM_CHANGED );
			g_MainCharInfo.HideSack(SACKTYPE__MODIFY, TRUE);
		}
		break;
	case ERR_CHANGEITEM_CANTCHANGE:
		{
			g_MainCharInfo.ShowHelpMessage(IDS_CHANGEITEM_CANTCHANGE, TEXTEFFECT_COLOR_WARNING);
			g_MainCharInfo.HideSack(SACKTYPE__MODIFY, TRUE);
		}
		break;
	case ERR_CHANGEITEM_FAIL:
		{
			g_MainCharInfo.ShowHelpMessage(IDS_CHANGEITEM_FAIL, TEXTEFFECT_COLOR_WARNING);
			g_MainCharInfo.HideSack(SACKTYPE__MODIFY, TRUE);
		}
		break;
	case ERR_CHANGEITEM_DONOTREBUILD:
		{
			g_MainCharInfo.ShowHelpMessage(IDS_CONVERT_DISABLE, TEXTEFFECT_COLOR_WARNING);
			g_MainCharInfo.HideSack(SACKTYPE__MODIFY, TRUE);

		}
		break;
	case ERR_GUARANTEEITEM_SUCCESS:		// 아이템 보험
		{
			g_MainCharInfo.ShowHelpMessage(IDS_ITEMCONVERT_COMPLETE);
			g_MainCharInfo.ShowHelpMessage(IDS_GUARANTEEITEM_SUCCESS);
			g_MainCharInfo.HideSack(SACKTYPE__MODIFY, TRUE);
		}
		break;
	case ERR_REBIRTHITEM_OVERSOURCE:	//HT_1116 : 각성자 아이템 추가
		{
			g_MainCharInfo.ShowHelpMessage(IDS_REBIRTHITEM_OVERSOURCE, TEXTEFFECT_COLOR_WARNING);
		}
		break;
	case ERR_REBIRTHITEM_REMAKEFAIL:	//HT_1116 : 각성자 아이템 추가
		{
			g_MainCharInfo.ShowHelpMessage(IDS_REBIRTHITEM_REMAKEFAIL, TEXTEFFECT_COLOR_WARNING);
		}
		break;
	default:
		{
			TCHAR str[50] = {0,};
			_stprintf( str, IDS_ERROR, bResult);
			g_MainCharInfo.ShowHelpMessage(str, TEXTEFFECT_COLOR_WARNING);
		}
		break;
	}

	g_MainCharInfo.HideSack(SACKTYPE__MODIFY);
	g_MainCharInfo.HideSack(SACKTYPE__FIVEELEMENT_CONVERT);
	
	return 0;
}

int OnCS_IM_ITEMINFO_ACK( CMsg &msg)
{
	BYTE bSackID	=0;
	BYTE bSackPos	=0;

	msg
		>> bSackID
		>> bSackPos;

	XiahItem::sItemInfo* pItem = NULL;

	switch(bSackID)
	{
	case 0:
		{
			pItem = g_MainCharInfo.m_pEquipSack->FindSackItemByPos( bSackPos);
		}
		break;
	case SACKTYPE_COLLECTION:	// 
		{
			pItem = g_MainCharInfo.m_pCollection->FindSackItemByPos(bSackPos);
		}
		break;
	default:
		{
			pItem = g_MainCharInfo.m_pMySack[ bSackID-1]->FindSackItemByPos( bSackPos);
		}
		break;
	}

	if(pItem)
	{
		XiahItem::GetItemData( pItem, msg);
	}
	else
	{
		if(g_MainCharInfo.m_pHoldItem)
		{
			XiahItem::GetItemData( g_MainCharInfo.m_pHoldItem->GetHoldItemItem(), msg);
		}
	}
	//HT_1116 : 각성자 아이템 추가
	msg
		>> pItem->m_wRebuithValue;


	switch(bSackID)
	{
	case 0:
		{
			g_MainCharInfo.m_pEquipSack->SetRefreshToolTip(TRUE);
		}
		break;
	case SACKTYPE_COLLECTION:
		{
			g_MainCharInfo.m_pCollection->SetRefreshToolTip(TRUE);
		}
		break;
	default:
		{
			g_MainCharInfo.m_pMySack[bSackID-1]->SetRefreshToolTip( TRUE);
			g_MainCharInfo.m_pMySack[bSackID-1]->RefreshSackPos();
		}
		break;
	}

	return 0;
}

int OnCS_IM_PICK_ACK( CMsg &msg)
{
	BYTE bResult	=0;

	msg
		>> bResult;

	switch( bResult)
	{
	case ERR_PICK_FULLSACK:
		{
			g_MainCharInfo.ShowHelpMessage( IDS_FULL_SACK, TEXTEFFECT_COLOR_WARNING);
			g_MainCharInfo.PlayInterfaceSound( ISOUND_WARNING);
		}		
		break;
	case ERR_PICK_OVERMONEY:	// 소지금액 초과
		{
			g_MainCharInfo.ShowHelpMessage(IDS_PETTRADE_OVERMONEY, TEXTEFFECT_COLOR_WARNING);			
			g_MainCharInfo.PlayInterfaceSound( ISOUND_WARNING);
		}
		break;
	case ERR_PICK_NOAUTHORITY:	// 획득 권한 없음
		{
			g_MainCharInfo.ShowHelpMessage(IDS_PICK_NOAUTHORITY, TEXTEFFECT_COLOR_WARNING);
		}
		break;
	default:
		break;		
	}
	
	return 0;
}


/**
 *
 * \param &msg 
 * \return 
 */
int OnCS_IM_GIVEITEM_ACK( CMsg &msg)
{
	BYTE bResult	=0;

	msg
		>> bResult;

	if( !bResult)
		return 0;

	TCHAR temp[50] = {0,};

	switch( bResult)
	{
	case ERR_GIVEITEM_NOTFOUNDITEM:
		g_MainCharInfo.ShowHelpMessage(IM_N_ITEM,TEXTEFFECT_COLOR_WARNING);
		break;
	case ERR_GIVEITEM_NOTFOUNDTARGET:
		g_MainCharInfo.ShowHelpMessage(IM_N_WHO,TEXTEFFECT_COLOR_WARNING);
		break;
	case ERR_GIVEITEM_INVALIDITEM:
		g_MainCharInfo.ShowHelpMessage(IM_N_ITEM2,TEXTEFFECT_COLOR_WARNING);
		break;
	case ERR_GIVEITEM_FAIL:			
		_stprintf( temp, IDS_ERROR, bResult);
		g_MainCharInfo.ShowHelpMessage(temp,TEXTEFFECT_COLOR_WARNING);
		break;

	case ERR_GIVEITEM_NOTDECWILDRATE:
		{
			_stprintf( temp, IDS_PET_WILD_NOT, bResult);
			g_MainCharInfo.ShowHelpMessage(temp,TEXTEFFECT_COLOR_WARNING);
			g_MainCharInfo.m_pHoldItem->SetItemBackToSack();
		}		
		break;

	case ERR_GIVEITEM_NOTINCHP:
		{
			_stprintf( temp, IDS_PET_HP_NOT, bResult);
			g_MainCharInfo.ShowHelpMessage(temp,TEXTEFFECT_COLOR_WARNING);
			g_MainCharInfo.m_pHoldItem->SetItemBackToSack();
		}		
		break;
	}

	return 0;
}

// 전낭 ACK
int OnCS_IM_MONEYBAG_ACK(CMsg &msg)
{
	BYTE bResult	= -1;
	BYTE bAct		=0;
	DWORD dwMoney	= 0;

	msg
		>> bResult
		>> bAct
		>> dwMoney;

	switch(bResult)
	{
	case ERR_MONEYBAG_SUCCESS:
		{
			TCHAR strTemp[128] = {0,};

			if(bAct == ACT_MONEYBAG_INPUT)
			{
				_stprintf(strTemp, IDS_PURSE_IN_SUCCESS, MoneyCommaStr(dwMoney).data());
			}
			else
			{
				_stprintf(strTemp, IDS_PURSE_OUT_SUCCESS, MoneyCommaStr(dwMoney).data());
			}

			g_MainCharInfo.ShowHelpMessage(strTemp);
		}
		break;
	case ERR_MONEYBAG_NOTFOUND:
		g_MainCharInfo.ShowHelpMessage(IDS_ERROR_NO_ITEM, TEXTEFFECT_COLOR_WARNING);
		break;
	case ERR_MONEYBAG_NOTBAG:
		g_MainCharInfo.ShowHelpMessage(IDS_ERROR_NO_ITEM, TEXTEFFECT_COLOR_WARNING);
		break;
	case ERR_MONEYBAG_IN_LOWMONEY:
		g_MainCharInfo.ShowHelpMessage(IDS_PURSE_IN_LOWMONEY);
		break;
	case ERR_MONEYBAG_IN_OVERMONEY:
		g_MainCharInfo.ShowHelpMessage(IDS_PURSE_IN_OVERMONEY);
		break;
	case ERR_MONEYBAG_OUT_LOWMONEY:
		g_MainCharInfo.ShowHelpMessage(IDS_PURSE_OUT_LOWMONEY);
		break;
	case ERR_MONEYBAG_OUT_OVERMONEY:
		g_MainCharInfo.ShowHelpMessage(IDS_PURSE_OUT_OVERMONEY);
		break;
	case ERR_MONEYBAG_OUT_OVERDUR:
		g_MainCharInfo.ShowHelpMessage(IDS_PURSE_OUT_OVERDUR);
		break;
	case ERR_MONEYBAG_ERROR:
		g_MainCharInfo.ShowHelpMessage(IDS_ERROR1, TEXTEFFECT_COLOR_WARNING);
		break;
	case ERR_MONEYBAG_IN_OVERMONEY2://HO_0918_07 초보자용 전낭 추가 : 전낭(소)
		g_MainCharInfo.ShowHelpMessage(IDS_PURSE_IN_OVERMONEY2, TEXTEFFECT_COLOR_WARNING);
		break;
	default:
		g_MainCharInfo.ShowHelpMessage(IDS_ERROR1, TEXTEFFECT_COLOR_WARNING);
		break;
	}

	return 0;
}


////////////////////////////////////////////////////////////////////////// 전서구

// 전서구 리스트 받기
int OnCS_IM_MEMOLIST_ACK(CMsg &msg)
{
	BYTE bCount =0;	

	msg
		>> bCount;

	if(bCount > 0)
	{
		BYTE		bRead		=0;
		DWORD		dwMemoID	=0;	
		DWORD		dwDate		=0;

		sString		szTitle;
		sString		szMsg;
		sString		szSenderName;

		for(int i = 0; i < bCount; ++i)
		{
			msg
				>> dwMemoID
				>> szSenderName
				>> dwDate
				>> szTitle
				>> bRead;

			g_Mail.Add_RecvMailList(dwMemoID,szSenderName,dwDate,szTitle,bRead==1?true:false);
		}

		szMsg.printf(IDS_RECV_MAIL_LIST, g_Mail.Get_RecvMailCount());
		g_MainCharInfo.ShowHelpMessage(szMsg.data());
	}

	return 0;		   
}

// 전서구 보내기
int OnCS_IM_SENDMEMO_ACK(CMsg &msg)
{
	BYTE	bResult	=0;
	sString	szName;

	msg 
		>> bResult
		>> szName;

	g_Mail.Assign_Result(szName,bResult);

	return 0;	
}


// 새로운 전서 도착
int OnCS_IM_NEWMEMO_ACK(CMsg &msg)
{
	DWORD		dwMemoID	=0;	
	DWORD		dwDate		=0;
	sString		szSenderName;
	sString		szTitle;
	sString		szMsg;

	msg 
		>> dwMemoID
		>> szSenderName
		>> dwDate
		>> szTitle;

	g_Mail.Add_RecvMailList(dwMemoID,szSenderName,dwDate,szTitle,false);
	
	if(g_pUIManager->IsShow(WINDOW_MAIL))
		g_Mail.Reflash_MAIL();

	szMsg.printf(IDS_RECV_MAIL,szSenderName.data());
	g_MainCharInfo.ShowHelpMessage(szMsg.data(), TEXTEFFECT_COLOR_WARNING);

	return 0;
}

// 해당 전서를 읽자!
int OnCS_IM_READMEMO_ACK(CMsg &msg)
{
	BYTE	bResult		=0;
	DWORD	dwMemoID	=0;
	sString	szContents;

	msg
		>> bResult
		>> dwMemoID
		>> szContents;

	switch(bResult)
	{
		case ERR_READMEMO_SUCCESS :
			{
				g_Mail.Add_RecvMailContents(dwMemoID,szContents);
			}
			break;

		case ERR_READMEMO_FAIL:
			{
			}
			break;
	}

	return 0;
}

// 해당 전서 지워짐
int OnCS_IM_DELETEMEMO_ACK(CMsg &msg)
{
	BYTE	bResult;
	DWORD	dwMemoID;

	msg
		>> bResult
		>> dwMemoID;

	switch(bResult)
	{
		case ERR_DELETEMEMO_SUCCESS :
				g_Mail.Delete_RecvMail(dwMemoID);
			break;

		case ERR_DELETEMEMO_ERROR :
			break;

	}

	return 0;
}


/**
* 장착 아이템 데미지/남은 내구력
* \param &msg 
* \return 
*/
int OnCS_IM_DAMAGE_ACK(CMsg &msg)
{
	BYTE bSackPos = 0;
	WORD wDamageCur = 0;
	WORD wRemainDur = 0;

	msg
		>> bSackPos
		>> wDamageCur
		>> wRemainDur;

	XiahItem::sItemInfo* pItemInfo = g_MainCharInfo.m_pEquipSack->FindSackItemByPos(bSackPos);

	if(pItemInfo != NULL)
	{
		TCHAR strTemp[128] = {0,};

		_stprintf(strTemp, IDS_ITEM_REMAIN, (LPCTSTR)pItemInfo->m_szName, wRemainDur);

		g_MainCharInfo.ShowHelpMessage(strTemp);
	}

	return 0;
}


/**
 * pet 장착용 아이템 내구력 감소할 경우
 * \param &msg 
 * \return 
 */
int OnCS_IM_PETITEMINFO_ACK(CMsg &msg)
{
	DWORD dwPetID	=0;
	BYTE bSackID	=0;
	BYTE bSackPos	=0;

	msg
		>> dwPetID
		>> bSackID
		>> bSackPos;

	sPetInfo* pPetInfo = g_PetList.GetPetInfo(dwPetID);

	if(pPetInfo)
	{
		XiahItem::sItemInfo* pItem = NULL;

		if(bSackID != PETSACKTYPE_EQUIPMENT )
		{
			if( bSackID >= 1 && bSackID <= 3 )
				pItem = pPetInfo->m_pSack[bSackID - 1]->FindSackItemByPos(bSackPos);

			//assert(pItem);
			if(pItem)
			{
				XiahItem::GetItemData(pItem, msg);

				pItem->m_bSackID = SACKTYPE__PET;
				pItem->m_bSackCount = bSackID - 1;

				pPetInfo->m_pSack[bSackID - 1]->SetRefreshToolTip(TRUE);
			}			
		}
		else
		{
			pItem = pPetInfo->m_pEquipSack->FindSackItemByPos(bSackPos);

			//assert(pItem);
			if(pItem)
			{
				XiahItem::GetItemData(pItem, msg);

				pItem->m_bSackID = SACKTYPE__PET_EQUIP;

				pPetInfo->m_pEquipSack->SetRefreshToolTip(TRUE);
			}			
		}
	} // if(pPetInfo)

	return 0;
}

/**
 * 9개지도, 7개 보석 조합 패킷
 * \param &msg 
 * \return 
 */
int OnCS_IM_PUZZLEITEM_ACK(CMsg &msg)
{
	BYTE bResult = 0;
	BYTE bFactor	=0;
	int	nValues		=0;

	msg
		>> bResult
		>> bFactor
		>> nValues;

	switch(bResult)
	{
		case ERR_PUZZLEITEM_SUCCESS:
			{
				g_MainCharInfo.ShowHelpMessage(IDS_MIXTURE_SUCCESS);
				g_MainCharInfo.HideSack(SACKTYPE__SMELT);

				if(bFactor != 255)
				{
					TCHAR strMsg[128] = {0,};

					if(bFactor == 14)
					{
						_stprintf(strMsg, IDS_FACTOR_15, nValues);
					}
					else
					{
						LPCTSTR lpstr = NULL;

						switch(bFactor)
						{
						case 0: lpstr = IDS_FACTOR_01;		break;
						case 1:	lpstr = IDS_FACTOR_02;		break;
						case 2:	lpstr = IDS_FACTOR_03;		break;
						case 3:	lpstr = IDS_FACTOR_04;		break;
						case 4:	lpstr = IDS_FACTOR_05;		break;
						case 5:	lpstr = IDS_FACTOR_06;		break;
						case 6:	lpstr = IDS_FACTOR_07;		break;
						case 7:	lpstr = IDS_FACTOR_08;		break;
						case 8:	lpstr = IDS_FACTOR_09;		break;
						case 9:	lpstr = IDS_FACTOR_10;		break;
						case 10:lpstr = IDS_FACTOR_11;		break;
						case 11:lpstr = IDS_FACTOR_12;		break;
						case 12:lpstr = IDS_FACTOR_13;		break;
						case 13:lpstr = IDS_FACTOR_14;		break;
						}

						LPCTSTR lpstrValue = NULL;

						if(nValues > 0)
							lpstrValue = IDS_FACTOR_UP;
						else
							lpstrValue = IDS_FACTOR_DOWN;

						_stprintf(strMsg, _T("%s %d %s"), lpstr, nValues, lpstrValue);
					}

					g_MainCharInfo.ShowHelpMessage(strMsg);
				}
			}
			break;
		case ERR_PUZZLEITEM_NOTFOUNDITEM:
			{
				g_MainCharInfo.ShowHelpMessage(IDS_ITEM_NOTFIND, TEXTEFFECT_COLOR_WARNING);				
			}
			break;
		case ERR_PUZZLEITEM_NOTFOUNDRES:
			{
				g_MainCharInfo.ShowHelpMessage(IDS_MIXTURE_NOTFOUNDRES, TEXTEFFECT_COLOR_WARNING);
			}
			break;
		case ERR_PUZZLEITEM_NEEDJOINITEM:	//조합이 모자를때
			{
				g_MainCharInfo.ShowHelpMessage(IDS_MIXTURE_NOTFOUNDRES, TEXTEFFECT_COLOR_WARNING);
			}
			break;
		case ERR_PUZZLEITEM_BADITEM:		//다른조합이 있을때
			{
				g_MainCharInfo.ShowHelpMessage(IDS_MIXTURE_BADITEM, TEXTEFFECT_COLOR_WARNING);				
				g_MainCharInfo.HideSack(SACKTYPE__SMELT);
			}
			break;
		case ERR_PUZZLEITEM_NOTJOINITEM:	//조합자원이 아니다
			{
				g_MainCharInfo.ShowHelpMessage(IDS_MIXTURE_BADRESOURCE, TEXTEFFECT_COLOR_WARNING);
				g_MainCharInfo.HideSack(SACKTYPE__SMELT);
			}
			break;
		case ERR_PUZZLEITEM_NOTPUZZLEITEM:	//조합아이템 아니다(bType != 19)
			{
				g_MainCharInfo.ShowHelpMessage(IDS_MIXTURE_BADRESOURCE, TEXTEFFECT_COLOR_WARNING);
				g_MainCharInfo.HideSack(SACKTYPE__SMELT);
			}
			break;
		case ERR_PUZZLEITEM_ALREADYJOIN:	//이미 조합아이템이 되었다
			{
				g_MainCharInfo.ShowHelpMessage(IDS_MIXTURE_ALREADYJOIN, TEXTEFFECT_COLOR_WARNING);				
				g_MainCharInfo.HideSack(SACKTYPE__SMELT);
			}
			break;
		case ERR_PUZZLEITEM_OVERFULLRES:	//자원이 넘칠때
			{
				g_MainCharInfo.ShowHelpMessage(IDS_MIXTURE_OVERFULLRES, TEXTEFFECT_COLOR_WARNING);
				g_MainCharInfo.HideSack(SACKTYPE__SMELT);
			}
			break;
		case ERR_PUZZLEITEM_INTERNALERROR:	// 내부에러
			{
				g_MainCharInfo.ShowHelpMessage(IDS_REL_ERR_INTERNAL, TEXTEFFECT_COLOR_WARNING);
				g_MainCharInfo.HideSack(SACKTYPE__SMELT);				
			}
			break;
		default:
			break;
	}

	return 0;
}

/**
 * 12개 편분 조합 패킷
 * \param &msg 
 * \return 
 */
int OnCS_IM_REJOINITEM_ACK(CMsg &msg)
{
	BYTE bResult = 0;

	msg
		>> bResult;

	switch(bResult)
	{
		case ERR_REJOINITEM_SUCCESS:
			{			
				g_MainCharInfo.ShowHelpMessage(IDS_MIXTURE_SUCCESS);
				g_MainCharInfo.HideSack(SACKTYPE__SMELT);
			}
			break;
		case ERR_REJOINITEM_FAIL:
			{
				g_MainCharInfo.ShowHelpMessage(IDS_MIXTURE_FAIL, TEXTEFFECT_COLOR_WARNING);
				g_MainCharInfo.HideSack(SACKTYPE__SMELT);
			}
			break;
		case ERR_REJOINITEM_NOTFOUNDRES:	//자원이 모자를때
			{
				g_MainCharInfo.ShowHelpMessage(IDS_MIXTURE_NOTFOUNDRES, TEXTEFFECT_COLOR_WARNING);
				g_MainCharInfo.HideSack(SACKTYPE__SMELT);
			}
			break;
		case ERR_REJOINITEM_OTHRERESOURCE:	//서로다른 편분일때
			{
				g_MainCharInfo.ShowHelpMessage(IDS_MIXTURE_OTHRERESOURCE, TEXTEFFECT_COLOR_WARNING);
				g_MainCharInfo.HideSack(SACKTYPE__SMELT);
			}
			break;
		case ERR_REJOINITEM_BADRESOURCE:	//조합 아이템이 아닐때
			{
				g_MainCharInfo.ShowHelpMessage(IDS_MIXTURE_BADRESOURCE, TEXTEFFECT_COLOR_WARNING);
				g_MainCharInfo.HideSack(SACKTYPE__SMELT);				
			}
			break;
		case ERR_REJOINITEM_NOTEMPTYSACK:	//행랑공간이 없다.
			{
				g_MainCharInfo.ShowHelpMessage(IDS_FULL_SACK, TEXTEFFECT_COLOR_WARNING);
				g_MainCharInfo.HideSack(SACKTYPE__SMELT);
			}
			break;
		case ERR_REJOINITEM_OVERFULLRES:	//자원이 넘칠때
			{
				g_MainCharInfo.ShowHelpMessage(IDS_MIXTURE_OVERFULLRES, TEXTEFFECT_COLOR_WARNING);
				g_MainCharInfo.HideSack(SACKTYPE__SMELT);
			}
			break;
		case ERR_REJOINITEM_INTERNALERROR:	// 내부 에러
			{
				g_MainCharInfo.ShowHelpMessage(IDS_REL_ERR_INTERNAL, TEXTEFFECT_COLOR_WARNING);
				g_MainCharInfo.HideSack(SACKTYPE__SMELT);
			}
			break;
		default:
			break;
	}

	return 0;
}

/**
 * 채집패킷
 * \param &msg 
 * \return 
 */
int OnCS_IM_GATHERITEM_ACK(CMsg &msg)
{
	BYTE bResult = 0;
	BYTE bGatherKind  =0;

	msg
		>> bResult
		>> bGatherKind;

	switch(bResult)
	{
		case ERR_GATHERITEM_SUCCESS:
			{
				g_MainCharInfo.ShowHelpMessage(IDS_GATHERITEM_SUCCESS);
			}
			break;
		case ERR_GATHERITEM_FAIL:
			break;
		case ERR_GATHERITEM_LIMITLEVEL:		// 능력치 부족 (10갑자이상만)
			{
				g_MainCharInfo.ShowHelpMessage(IDS_GATHERITEM_LIMITLEVEL, TEXTEFFECT_COLOR_WARNING);				
			}
			break;
		case ERR_GATHERITEM_NOTFOUNDITEM:	// 채집도구가 아니다.
			{
				g_MainCharInfo.ShowHelpMessage(IDS_GATHERITEM_NOTFOUNDITEM, TEXTEFFECT_COLOR_WARNING);
			}
			break;
		case ERR_GATHERITEM_NOTFOUNDOBJECT:	// 채집을 할 수 없다.
			{
				g_MainCharInfo.ShowHelpMessage(IDS_GATHERITEM_NOTFOUNDOBJECT, TEXTEFFECT_COLOR_WARNING);
			}
			break;
		case ERR_GATHERITEM_INTERNALERROR:	// 내부 에러
			{
				g_MainCharInfo.ShowHelpMessage(IDS_REL_ERR_INTERNAL, TEXTEFFECT_COLOR_WARNING);
			}
			break;
		default:
			break;
	}

	return 0;
}

/**
 * 가래떡/변종아이템
 * \param &msg 
 * \return 
 */
int OnCS_IM_VARIENTITEM_ACK(CMsg &msg)

{
	BYTE bResult = 0;

	msg
		>> bResult;

	switch(bResult)
	{
	case ERR_VARIENTITEM_SUCCESS:
		{
			g_MainCharInfo.ShowHelpMessage(IDS_MIXTURE_SUCCESS);
			g_MainCharInfo.HideSack(SACKTYPE__SMELT);
		}
		break;
	case ERR_VARIENTITEM_FAIL:
		{
			g_MainCharInfo.ShowHelpMessage(IDS_MIXTURE_FAIL, TEXTEFFECT_COLOR_WARNING);
			g_MainCharInfo.HideSack(SACKTYPE__SMELT);
		}
		break;
	case ERR_VARIENTITEM_NOTFOUNDRES:		//자원이 모자를때
		{
			g_MainCharInfo.ShowHelpMessage(IDS_MIXTURE_NOTFOUNDRES, TEXTEFFECT_COLOR_WARNING);
			g_MainCharInfo.HideSack(SACKTYPE__SMELT);
		}
		break;
	case ERR_VARIENTITEM_OVERFULLRES:		//자원이 넘칠때
		{
			g_MainCharInfo.ShowHelpMessage(IDS_MIXTURE_OVERFULLRES, TEXTEFFECT_COLOR_WARNING);
			g_MainCharInfo.HideSack(SACKTYPE__SMELT);
		}
		break;

	case ERR_VARIENTITEM_OTHRERESOURCE:		//조합 아이템이 아닐때
		{
			g_MainCharInfo.ShowHelpMessage(IDS_MIXTURE_BADRESOURCE, TEXTEFFECT_COLOR_WARNING);
			g_MainCharInfo.HideSack(SACKTYPE__SMELT);	
		}
		break;

	case ERR_VARIENTITEM_BADRESOURCE:		//종류가 다를때
		{
			g_MainCharInfo.ShowHelpMessage(IDS_MIXTURE_BADITEM, TEXTEFFECT_COLOR_WARNING);
			g_MainCharInfo.HideSack(SACKTYPE__SMELT);
		}
		break;

	case ERR_VARIENTITEM_ALREADY:			// 이미조합된것(더이상 안됨)
		{
			g_MainCharInfo.ShowHelpMessage(IDS_VARIENTITEM_ALREADY, TEXTEFFECT_COLOR_WARNING);
			g_MainCharInfo.HideSack(SACKTYPE__SMELT);
		}
		break;

	case ERR_VARIENTITEM_MAX:				//흑보 변종개조 최대값(더이상 안됨)
		{
			g_MainCharInfo.ShowHelpMessage(IDS_VARIENTITEM_MAX, TEXTEFFECT_COLOR_WARNING);
			g_MainCharInfo.HideSack(SACKTYPE__SMELT);
		}
		break;

	case ERR_VARIENTITEM_ABILITYMAX:		//흑보 변종개조 최대값(능력치 최대값일 경우)
		{
			g_MainCharInfo.ShowHelpMessage(IDS_MIXTURE_SUCCESS);
			g_MainCharInfo.ShowHelpMessage(IDS_VARIENTITEM_ABILITYMAX);
			g_MainCharInfo.HideSack(SACKTYPE__SMELT);
		}
		break;

	case ERR_VARIENTITEM_RATEMAX:		//흑보 변종개조 최대값(추가 성공률 최대값일 경우)
		{
			g_MainCharInfo.ShowHelpMessage(IDS_MIXTURE_SUCCESS);
			g_MainCharInfo.ShowHelpMessage(IDS_VARIENTITEM_RATEMAX);
			g_MainCharInfo.HideSack(SACKTYPE__SMELT);
		}
		break;

	case ERR_VARIENTITEM_INTERNALERROR:		//내부댄轎
		{
			g_MainCharInfo.ShowHelpMessage(IDS_REL_ERR_INTERNAL, TEXTEFFECT_COLOR_WARNING);
			g_MainCharInfo.HideSack(SACKTYPE__SMELT);	
		}
		break;

	case ERR_VARIENTITEM_COUPON_SUCCESS:		//HT_0523 쿠폰등록성공 
		{
			g_MainCharInfo.ShowHelpMessage(IDS_COUPON_SUCCESS);
			g_MainCharInfo.HideSack(SACKTYPE__SMELT);	
		}
		break;

	case ERR_VARIENTITEM_COUPON_TOOMANY:		//쿠폰등록실패(2개 이상일 경우)
		{
			g_MainCharInfo.ShowHelpMessage(IDS_COUPON_OVERFULLRES, TEXTEFFECT_COLOR_WARNING);
			g_MainCharInfo.HideSack(SACKTYPE__SMELT);	
		}
		break;

	case ERR_VARIENTITEM_COUPON_FAIL:
		{
			g_MainCharInfo.ShowHelpMessage(_T("쿠폰등록실패"), TEXTEFFECT_COLOR_WARNING);
			g_MainCharInfo.HideSack(SACKTYPE__SMELT);	
		}
		break;
	default:
		break;
	}

	return 0;
}

/**
 * 보험 아이템 복구,소멸
 * \param &msg 
 * \return 
 */
int OnCS_IM_REWARDGUARANTEE_ACK(CMsg &msg)
{
	BYTE bResult =0;

	msg
		>> bResult;

	switch(bResult)
	{
	case ERR_REWARDGUARANTEE_SUCCESS:		//복구성공
		{
			g_MainCharInfo.ShowHelpMessage(IDS_REWARDGUARANTEE_SUCCESS);
		}
		break;
	case ERR_REWARDGUARANTEE_DELETED:		//영구삭제되었습니다.
		{
			g_MainCharInfo.ShowHelpMessage(IDS_REWARDGUARANTEE_DELETED);
		}
		break;
	case ERR_REWARDGUARANTEE_NOTFOUND:		//존재하지 않는 아이템
		{
			g_MainCharInfo.ShowHelpMessage(IM_N_ITEM, TEXTEFFECT_COLOR_WARNING);
		}
		break;
	case ERR_REWARDGUARANTEE_INSERTFAIL:	//자리가 부족
		{
			g_MainCharInfo.ShowHelpMessage(IDS_REWARDGUARANTEE_INSERTFAIL, TEXTEFFECT_COLOR_WARNING);
		}
		break;
	case ERR_REWARDGUARANTEE_INTERNALERROR:	//시스템 댄轎(관리자에게 문의하십시오)
		{
			g_MainCharInfo.ShowHelpMessage(IDS_REL_ERR_INTERNAL, TEXTEFFECT_COLOR_WARNING);
		}
		break;
	default:
		break;
	}

	return 0;
}

/**
 * 이미지 조합
 * \param &msg 
 * \return 
 */
int OnCS_IM_EVENTPUZZLE_ACK(CMsg &msg)
{
	BYTE bResult = 0;

	msg
		>> bResult;

	switch(bResult)
	{
	case ERR_EVENTPUZZLE_SUCCESS:
		{
			g_MainCharInfo.ShowHelpMessage(IDS_EVENTPUZZLE_SUCCESS);
			g_MainCharInfo.HideSack(SACKTYPE__SMELT);
		}
		break;
	case ERR_EVENTPUZZLE_FAIL:
		{
			g_MainCharInfo.ShowHelpMessage(IDS_EVENTPUZZLE_FAIL, TEXTEFFECT_COLOR_WARNING);
			g_MainCharInfo.HideSack(SACKTYPE__SMELT);
		}
		break;

	default:
		break;	
	}

	return 0;
}

/**
* 조합
* \param &msg 
* \return 
*/
int OnCS_IM_MIXITEM_ACK(CMsg &msg)
{
	BYTE bResult =0;

	msg
		>> bResult;

	switch(bResult)
	{
	case 0: //성공
		{
			g_MainCharInfo.ShowHelpMessage(IDS_MIXTURE_SUCCESS);
			g_MainCharInfo.HideSack(SACKTYPE__SMELT);
		}
		break;
	case 1: // 조합 개수 초과
		{
			g_MainCharInfo.ShowHelpMessage(IDS_MIXTURE_OVERFULLRES);
			g_MainCharInfo.HideSack(SACKTYPE__SMELT);
		}
		break;
	case 2: // 조합 구성 요소 이상
		{
			g_MainCharInfo.ShowHelpMessage(IDS_MIXTURE_BADITEM);
			g_MainCharInfo.HideSack(SACKTYPE__SMELT);
		}
		break;
	default:
		break;
	}

	return 0;
}

/**
* 수집
* \param &msg 
* \return 
*/
int OnCS_IM_MOVEINCOLLECTITEM_ACK(CMsg &msg)
{
	DWORD 	dwItemID		=0;
	BYTE 	bResult			=0;	
	BYTE	bCollectSackPos =0;
	BYTE	bSackID			=0;
	BYTE 	bSackPos		=0;

	msg
		>> bResult
		>> dwItemID
		>> bCollectSackPos
		>> bSackID
		>> bSackPos;

	switch(bResult)
	{
	case ERR_MOVEINCOLLECT_SUCCESS:
		{
			XiahItem::sItemInfo* pHoldItem = g_MainCharInfo.m_pHoldItem->GetHoldItemItem();

			if(pHoldItem)
			{
				if(pHoldItem->m_dwItemID == dwItemID)
				{
					pHoldItem->m_bSackID  = SACKTYPE__COLLECTION;
					pHoldItem->m_bSackPos = bCollectSackPos;

					g_MainCharInfo.m_pCollection->InsertItem(bCollectSackPos, pHoldItem);

					g_MainCharInfo.m_pHoldItem->EmptyHoldItemItem();
				}
			}
			else
			{
				XiahItem::sItemInfo* pItem = NULL;

				pItem = g_MainCharInfo.m_pMySack[bSackID-1]->FindSackItemByID(dwItemID);		

				if(pItem)
				{
					g_MainCharInfo.m_pMySack[bSackID-1]->DeleteItem(bSackPos);

					pItem->m_bSackID  = SACKTYPE__COLLECTION;
					pItem->m_bSackPos = bCollectSackPos;

					g_MainCharInfo.m_pCollection->InsertItem(bCollectSackPos, pItem);
				}
			}

			return 0;
		}
		break;
	case ERR_MOVEINCOLLECT_NOTFINDSACK:		// 행낭에 못찾는다
		g_MainCharInfo.ShowHelpMessage(IDS_ITEM_NOTFIND, TEXTEFFECT_COLOR_WARNING);				
		break;
	case ERR_MOVEINCOLLECT_NOTCOLLECTITEM:	// 수집아이템 x
		g_MainCharInfo.ShowHelpMessage(IDS_MOVEIN_NOTCOLLECTITEM_1, TEXTEFFECT_COLOR_WARNING);		
		break;
	case ERR_MOVEINCOLLECT_FULLSACK:		// 가득
		g_MainCharInfo.ShowHelpMessage(IDS_FULL_SACK, TEXTEFFECT_COLOR_WARNING);		
		break;
	case REE_MOVEINCOLLECT_INTERNALERROR:	// 내부에서 이상한 짓을 한다
		g_MainCharInfo.ShowHelpMessage(IDS_REL_ERR_INTERNAL, TEXTEFFECT_COLOR_WARNING);
		break;
	case ERR_MOVEINCOLLECT_NOVALUE:			// d
		{
			g_MainCharInfo.ShowHelpMessage(IDS_ERR_MOVEINCOLLECT_NOVALUE, TEXTEFFECT_COLOR_WARNING);
		}
		break;
	default:
		break;
	}

	g_MainCharInfo.m_pHoldItem->SetItemBackToSack();

	return 0;
}

/**
* 수집
* \param &msg 
* \return 
*/
int OnCS_IM_MOVEOUTCOLLECTITEM_ACK(CMsg &msg)
{
	DWORD dwItemID	=0;
	BYTE  bResult	=0;	
	BYTE  bSackID	=0;
	BYTE  bSackPos	=0;
	BYTE  bCollectSackPos	=0;

	msg
		>> bResult
		>> dwItemID
		>> bSackID
		>> bSackPos
		>> bCollectSackPos;


	switch(bResult)
	{
	case ERR_MOVEOUTCOLLECT_SUCCESS:
		{
			XiahItem::sItemInfo* pHoldItem = g_MainCharInfo.m_pHoldItem->GetHoldItemItem();

			if(pHoldItem)
			{
				if(pHoldItem->m_dwItemID == dwItemID)
				{
					pHoldItem->m_bSackID  = SACKTYPE__DEFAULT;
					pHoldItem->m_bSackPos = bSackPos;

					g_MainCharInfo.m_pMySack[bSackID-1]->InsertItem(bSackPos, pHoldItem);

					g_MainCharInfo.m_pHoldItem->EmptyHoldItemItem();
				}
			}
			else
			{
				XiahItem::sItemInfo* pItem = NULL;

				pItem = g_MainCharInfo.m_pCollection->FindSackItemByID(dwItemID);		

				if(pItem)
				{
					g_MainCharInfo.m_pCollection->DeleteItem(bSackPos);

					pItem->m_bSackID  = SACKTYPE__DEFAULT;
					pItem->m_bSackPos = bSackPos;

					g_MainCharInfo.m_pMySack[bSackID-1]->InsertItem(bSackPos, pItem);
				}
			}

			return 0;
		}
		break;
	case ERR_MOVEOUTCOLLECT_NOTFINDSACK:
		g_MainCharInfo.ShowHelpMessage(IDS_ITEM_NOTFIND, TEXTEFFECT_COLOR_WARNING);		
		break;
	case ERR_MOVEOUTCOLLECT_NOTEMPTYSACK:	// 
		g_MainCharInfo.ShowHelpMessage(IM_N_ITEM, TEXTEFFECT_COLOR_WARNING);		
		break;
	case ERR_MOVEOUTCOLLECT_FULLSACK:
		g_MainCharInfo.ShowHelpMessage(IDS_FULL_SACK, TEXTEFFECT_COLOR_WARNING);	
		break;
	case REE_MOVEOUTCOLLECT_INTERNALERROR:
		g_MainCharInfo.ShowHelpMessage(IDS_REL_ERR_INTERNAL, TEXTEFFECT_COLOR_WARNING);
		break;
	default:
		break;
	}

	g_MainCharInfo.m_pHoldItem->SetItemBackToSack();

	return 0;
}

/**
 * 망치 조합
 * \param &msg 
 * \return 
 */
int OnCS_IM_MAKEREPAIRHAMMER_ACK(CMsg &msg)
{
	BYTE bResult =0;

	msg
		>> bResult;

	switch(bResult)
	{
	case 0: //성공
		{
			g_MainCharInfo.ShowHelpMessage(IDS_MIXTURE_SUCCESS);
			g_MainCharInfo.HideSack(SACKTYPE__SMELT);
		}
		break;
	case 1: // 조합 개수 초과
		{
			g_MainCharInfo.ShowHelpMessage(IDS_EVENTPUZZLE_FAIL, TEXTEFFECT_COLOR_WARNING);
			g_MainCharInfo.HideSack(SACKTYPE__SMELT);
		}
		break;
	default:
		break;
	}

	return 0;
}

/**
 * 사신셋 조합
 * \param &msg 
 * \return 
 */
int OnCS_IM_MAKEUNIONITEM_ACK(CMsg &msg)
{
	BYTE bResult = 0;

	msg
		>> bResult;

	switch(bResult)
	{
	case ERR_MAKEUNIONITEM_SUCCESS:		//아이템 생성 조합 성공.
		{
			g_MainCharInfo.ShowHelpMessage(IDS_MIXTURE_SUCCESS);
			g_MainCharInfo.HideSack(SACKTYPE__SMELT);
		}
		break;
	case ERR_MAKEUNIONITEM_FAIL:		//아이템 생성 조합 실패.
		{
			g_MainCharInfo.ShowHelpMessage(IDS_MIXTURE_FAIL, TEXTEFFECT_COLOR_WARNING);
			g_MainCharInfo.HideSack(SACKTYPE__SMELT);
		}
		break;
	case ERR_MAKEUNIONITEM_CANT:		//구성요소가 맞지 않습니다.
		{
			g_MainCharInfo.ShowHelpMessage(IDS_MIXTURE_CANT, TEXTEFFECT_COLOR_WARNING);
			g_MainCharInfo.HideSack(SACKTYPE__SMELT);
		}
		break;
	case ERR_MAKEUNIONITEM_INTERNALERROR:	//시스템 댄轎(관리자에게 문의하십시오)
		{
			g_MainCharInfo.ShowHelpMessage(IDS_REL_ERR_INTERNAL, TEXTEFFECT_COLOR_WARNING);
			g_MainCharInfo.HideSack(SACKTYPE__SMELT);
		}
		break;
	default:
		break;
	}

	return 0;
}


/**
 * //HT_0829 : 프리미엄 퀘스트 
 * \param &msg 
 * \return 
 */
int OnCS_IM_QUESTROLL_ACK(CMsg &msg)
{
	BYTE bResult = 0;

	msg
		>> bResult;

	switch(bResult)
	{
	case ERR_QUEST_CONDITION:			//조건 부족으로 사용 불가
		{
			g_MainCharInfo.ShowHelpMessage(IDS_QUEST_CONDITION, TEXTEFFECT_COLOR_WARNING);
		}
		break;
	case ERR_QUEST_HOLD:				//동일한 기연이 존재하여 사용 불가
		{
			g_MainCharInfo.ShowHelpMessage(IDS_QUEST_HOLD, TEXTEFFECT_COLOR_WARNING);
		}
		break;
	case ERR_QUEST_INTERNAL:			//내부 에러
		{
			g_MainCharInfo.ShowHelpMessage(IDS_INTERNAL_ERROR, TEXTEFFECT_COLOR_WARNING);
		}
		break;
	default:
		break;
	}

	return 0;
}


//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////




int OnCS_NV_EVENTTIME_ACK(CMsg &msg)
{	
	BYTE bMax	=0;

	msg
		>> bMax;

	for(register int i=0; i < bMax; ++i)
	{
		BYTE bNum	=0;
		DWORD dwNow	=0;
		DWORD dwMax	=0;

		msg
			>> bNum
			>> dwNow
			>> dwMax;

		TCHAR szTip [64] = {0,};
		_stprintf( szTip, IDS_E_TIME, dwNow, dwMax);

		switch(bNum)
		{
		case 0:
			{
				for(register int j=0; j <= 4; ++j)
					g_MainCharInfo.m_pImageScrMsg->SetScrMsg(j, szTip);
			}			
			break;
		case 1:
			{
				g_MainCharInfo.m_pImageScrMsg->SetScrMsg(5, szTip);
			}			
			break;
		case 2:
			{
				for(register int j=6; j <= 9; ++j)
					g_MainCharInfo.m_pImageScrMsg->SetScrMsg(j, szTip);
			}			
			break;
		case 3:
			{
				for(register int j=10; j <= 11; ++j)
					g_MainCharInfo.m_pImageScrMsg->SetScrMsg(j, szTip);
			}			
			break;
		default:
			break;
		}
	}
	
	return 0;
}

int OnCS_NV_RUNEVENTINFO_ACK(CMsg &msg)
{
	BYTE bRunEvent = 0;	

	msg
		>> bRunEvent;

	g_MainCharInfo.m_pImageScrMsg->AllHide();

	for(register int i=0; i < bRunEvent; ++i)
	{
		BYTE bEventKind		= 0;
		BYTE bCanUser		= 0;
		DWORD dwEventCharID = 0;
		DWORD dwEffectValue = 0;
		DWORD dwUserType	= 0;
		sString strCharName;
		sString strMunpaName;

		msg
			>> bEventKind
			>> dwEventCharID
			>> dwEffectValue
			>> strCharName
			>> bCanUser
			>> dwUserType
			>> strMunpaName;
		
		TCHAR szTip [64] = {0,};
		int nResID = 0;

		switch(bEventKind)
		{
		case 0:
			_stprintf( szTip, IDS_E_ATTK_INC, dwEffectValue);
			nResID = 1116;
			break;
		case 1:
			_stprintf( szTip, IDS_E_DEF_INC, dwEffectValue);
			nResID = 1116;
			break;
		case 2:
			_stprintf( szTip, IDS_E_AGI_INC, dwEffectValue);
			nResID = 1116;
			break;
		case 3:
			_stprintf( szTip, IDS_E_LIFE_INC, dwEffectValue);
			nResID = 1116;
			break;
		case 4:
			_stprintf( szTip, IDS_E_CRITICAL_INC, dwEffectValue);
			nResID = 1116;
			break;
		case 5:
			_stprintf( szTip, IDS_E_DAN_EXP_INC, dwEffectValue);
			nResID = 1117;
			break;
		case 6:
			_stprintf( szTip, IDS_E_MON_ATTK_DIM, dwEffectValue);
			nResID = 1118;
			break;
		case 7:
			_stprintf( szTip, IDS_E_MON_DEF_DIM, dwEffectValue);
			nResID = 1118;
			break;
		case 8:
			_stprintf( szTip, IDS_E_MON_AGI_DIM, dwEffectValue);
			nResID = 1118;
			break;
		case 9:
			_stprintf( szTip, IDS_E_MON_LIFE_DIM, dwEffectValue);
			nResID = 1118;
			break;
		case 10:
			strcpy(szTip, IDS_E_PT_FEE);
			nResID = 1119;
			break;
		case 11:
			_stprintf( szTip, IDS_E_NT_DC, dwEffectValue);
			nResID = 1119;
			break;
		default:
			strcpy(szTip, IDS_ERROR1);
			break;
		}

		g_MainCharInfo.m_pImageScrMsg->SetScrImg(bEventKind, nResID, dwEventCharID);
		
		g_MainCharInfo.m_pImageScrMsg->DelToolTip(bEventKind);
		g_MainCharInfo.m_pImageScrMsg->AddToolTip(bEventKind, szTip);

		switch(bCanUser)
		{
		case 0:
			g_MainCharInfo.m_pImageScrMsg->AddToolTip(bEventKind, IDS_E_ALL);
			break;
		case 1:// 1 : 검영, 2 : 연랑, 3 : 무투
			{
				switch(dwUserType)
				{
				case 1:
					g_MainCharInfo.m_pImageScrMsg->AddToolTip(bEventKind, IDS_E_CALSS_G);
					break;
				case 2:
					g_MainCharInfo.m_pImageScrMsg->AddToolTip(bEventKind, IDS_E_CALSS_Y);
					break;
				case 3:
					g_MainCharInfo.m_pImageScrMsg->AddToolTip(bEventKind, IDS_E_CALSS_M);
					break;
				}
			}
			break;
		case 2:
			{
				_stprintf( szTip, IDS_E_CLAN, strMunpaName.data());
				g_MainCharInfo.m_pImageScrMsg->AddToolTip(bEventKind, szTip);
			}
			break;
		case 3:
			g_MainCharInfo.m_pImageScrMsg->AddToolTip(bEventKind, IDS_E_DAN);
			break;
		default:
			{
				if(bEventKind >= 5 && bEventKind <= 9)
					g_MainCharInfo.m_pImageScrMsg->AddToolTip(bEventKind, IDS_E_MON);
				else
					g_MainCharInfo.m_pImageScrMsg->AddToolTip(bEventKind, IDS_E_PT);
			}
			break;
		}
		
		_stprintf( szTip, IDS_E_CHARNAME, strCharName.data());
		g_MainCharInfo.m_pImageScrMsg->AddToolTip(bEventKind, szTip);

		g_MainCharInfo.m_pImageScrMsg->Show(bEventKind);


/*
		// 이벤트 아이템 이펙트
		XiahObject::CXiahObject* pXiahObject = XiahObject::g_XiahObjectManager.FindXiahObject( MAKEOBJECTID( 0, dwEventCharID,OBJTYPE_PC) );
		if( pXiahObject && pXiahObject->m_pObject )
		{
			CXiahCharObject *pCharObject = reinterpret_cast<CXiahCharObject*>(pXiahObject->m_pObject);

			// 0: 공격계, 1: 성장계, 2: 몬스터계, 3: 경제계
			int nEffectType = 0;

			if( bEventKind >= 0 && bEventKind <= 4 )
				nEffectType = 0;
			else
			if( bEventKind == 5 )
				nEffectType = 1;
			else
			if( bEventKind >= 6 && bEventKind <= 9 )
				nEffectType = 2;
			else
			if( bEventKind >= 10 && bEventKind <= 11 )
				nEffectType = 3;

			// 발동 이펙트는 필요 없다. 진행중인것만 있으면 되니깐.

			// 지속 이펙트
			// 같은 종류의 아이템을 벌써 쓰고 있으면 이펙트가 있는 상태다.
			if( NULL == pCharObject->m_pEventItemEffectPP[nEffectType] )
			{
				_EFFECTPACKAGE* pEffectPackage2 = g_EffectManager.EnqOutGongPersistEffectImmediately( eAttackKindItem_keepup + nEffectType );

				if( pEffectPackage2 && pEffectPackage2->pEffectRender && pEffectPackage2->pEffectRender->pPackagePair )
				{
					pEffectPackage2->pEffectRender->pPackagePair->pWorldMatrix = (MATRIX*)pCharObject->m_CharRender.GetCharTM();

					pCharObject->m_pEventItemEffectPP[nEffectType] = pEffectPackage2->pEffectRender->pPackagePair;
				}// if
			}

		}// if( pXiahObject )
*/


	}	

	return 0;
}
//bRunEvent		- 현재 이벤트 수 
//bEventKind	- 이벤트 종류 
//dwEventCharID - 이벤트 개최자 캐릭터 아이디
//dwEffectValue - 이벤트 효과 값 (단위 %)
//strCharName   - 이벤트 개최자 이름 
//bCanUser      - 이벤트 참가할 수 있느 유저 (0 = ALL, 1 = 해당유파, 2 = 문파만, 3 = 단 )
//dwUserType    - 유파일경우 유파 종류, 문파일 경우 문파 아이디.
//strMunpaName  - 문파일 경우 문파명;

int OnCS_NV_CHANGEEVENTINFO_ACK(CMsg &msg)
{
	BYTE bEventStatus	= 0;
	BYTE bEventKind		= 0;

	msg
		>> bEventStatus
		>> bEventKind;

	if(bEventStatus == 0)
	{
		BYTE bCanUser		= 0;
		DWORD dwEventCharID = 0;
		DWORD dwEffectValue = 0;
		DWORD dwUserType	= 0;
		sString strCharName;
		sString strMunpaName;

		msg
			>> dwEventCharID
			>> dwEffectValue
			>> strCharName
			>> bCanUser
			>> dwUserType
			>> strMunpaName;


		TCHAR szTip [64] = {0,};
		int nResID = 0;

		switch(bEventKind)
		{
		case 0:
			_stprintf( szTip, IDS_E_ATTK_INC, dwEffectValue);
			nResID = 1116;
			break;
		case 1:
			_stprintf( szTip, IDS_E_DEF_INC, dwEffectValue);
			nResID = 1116;
			break;
		case 2:
			_stprintf( szTip, IDS_E_AGI_INC, dwEffectValue);
			nResID = 1116;
			break;
		case 3:
			_stprintf( szTip, IDS_E_LIFE_INC, dwEffectValue);
			nResID = 1116;
			break;
		case 4:
			_stprintf( szTip, IDS_E_CRITICAL_INC, dwEffectValue);
			nResID = 1116;
			break;
		case 5:
			_stprintf( szTip, IDS_E_DAN_EXP_INC, dwEffectValue);
			nResID = 1117;
			break;
		case 6:
			_stprintf( szTip, IDS_E_MON_ATTK_DIM, dwEffectValue);
			nResID = 1118;
			break;
		case 7:
			_stprintf( szTip, IDS_E_MON_DEF_DIM, dwEffectValue);
			nResID = 1118;
			break;
		case 8:
			_stprintf( szTip, IDS_E_MON_AGI_DIM, dwEffectValue);
			nResID = 1118;
			break;
		case 9:
			_stprintf( szTip, IDS_E_MON_LIFE_DIM, dwEffectValue);
			nResID = 1118;
			break;
		case 10:
			strcpy(szTip, IDS_E_PT_FEE);
			nResID = 1119;
			break;
		case 11:
			_stprintf( szTip, IDS_E_NT_DC, dwEffectValue);
			nResID = 1119;
			break;
		default:
			strcpy(szTip, IDS_ERROR1);
			break;
		}

		g_MainCharInfo.m_pImageScrMsg->SetScrImg(bEventKind, nResID, dwEventCharID);

		g_MainCharInfo.m_pImageScrMsg->DelToolTip(bEventKind);
		g_MainCharInfo.m_pImageScrMsg->AddToolTip(bEventKind, szTip);

		switch(bCanUser)
		{
		case 0:
			g_MainCharInfo.m_pImageScrMsg->AddToolTip(bEventKind, IDS_E_ALL);
			break;
		case 1:// 1 : 검영, 2 : 연랑, 3 : 무투
			{
				switch(dwUserType)
				{
				case 1:
					g_MainCharInfo.m_pImageScrMsg->AddToolTip(bEventKind, IDS_E_CALSS_G);
					break;
				case 2:
					g_MainCharInfo.m_pImageScrMsg->AddToolTip(bEventKind, IDS_E_CALSS_Y);
					break;
				case 3:
					g_MainCharInfo.m_pImageScrMsg->AddToolTip(bEventKind, IDS_E_CALSS_M);
					break;
				}
			}
			break;
		case 2:
			{
				_stprintf( szTip, IDS_E_CLAN, strMunpaName.data());
				g_MainCharInfo.m_pImageScrMsg->AddToolTip(bEventKind, szTip);
			}
			break;
		case 3:
			g_MainCharInfo.m_pImageScrMsg->AddToolTip(bEventKind, IDS_E_DAN);
			break;
		default:
			{
				if(bEventKind >= 5 && bEventKind <= 9)
					g_MainCharInfo.m_pImageScrMsg->AddToolTip(bEventKind, IDS_E_MON);
				else
					g_MainCharInfo.m_pImageScrMsg->AddToolTip(bEventKind, IDS_E_PT);
			}
			break;
		}

		_stprintf( szTip, IDS_E_CHARNAME, strCharName.data());
		g_MainCharInfo.m_pImageScrMsg->AddToolTip(bEventKind, szTip);

		g_MainCharInfo.m_pImageScrMsg->Show(bEventKind);

		// 얘도 사운드가 있네.
		// 0: 공격계, 1: 성장계, 2: 몬스터계, 3: 경제계
		int nEventType = 0;
		if( bEventKind >= 0 && bEventKind <= 4 )
			nEventType = 0;
		else
		if( bEventKind == 5 )
			nEventType = 1;
		else
		if( bEventKind >= 6 && bEventKind <= 9 )
			nEventType = 2;
		else
		if( bEventKind >= 10 && bEventKind <= 11 )
			nEventType = 3;

		XiahObject::CXiahObject* pXiahObject = XiahObject::g_XiahObjectManager.FindXiahObject( MAKEOBJECTID( 0, dwEventCharID,OBJTYPE_PC) );
		if( pXiahObject && pXiahObject->m_pObject )
		{
			CXiahCharObject *pCharObject = reinterpret_cast<CXiahCharObject*>(pXiahObject->m_pObject);

			g_MainCharInfo.PlayInterfaceSoundWithVol( EVENT_ITEM_SOUND_START, pCharObject->m_Position );
			g_MainCharInfo.PlayInterfaceSoundWithVol( EVENT_ITEM_SOUND_ATTACK + nEventType, pCharObject->m_Position );
		}


/*
		// 2004_04_19 Changth : EVENT
		// 이벤트 아이템의 이펙트를 호출하기 위해서는 두군데에 코딩을 한다.
		// 하나는 장비를 장착할때 횅땍하는 것이고, 또 하나는 ChangeEventInfo_ack가 올때 이다.
		// 이유는, 이펙트의 시작은 ChangeEventInfo_ack에서 알수 있는데, 장비를 장착하고 나서
		// 이 패킷이 오기때문에 정작 장착할때 이벤트가 시작인지 알수가 없다.
		// 이벤트 시작이다.

		// 이벤트 아이템 이펙트
		XiahObject::CXiahObject* pXiahObject = XiahObject::g_XiahObjectManager.FindXiahObject( MAKEOBJECTID( 0, dwEventCharID,OBJTYPE_PC) );
		if( pXiahObject && pXiahObject->m_pObject )
		{
			CXiahCharObject *pCharObject = reinterpret_cast<CXiahCharObject*>(pXiahObject->m_pObject);

			// 0: 공격계, 1: 성장계, 2: 몬스터계, 3: 경제계
			int nEffectType = 0;

			if( bEventKind >= 0 && bEventKind <= 4 )
				nEffectType = 0;
			else
			if( bEventKind == 5 )
				nEffectType = 1;
			else
			if( bEventKind >= 6 && bEventKind <= 9 )
				nEffectType = 2;
			else
			if( bEventKind >= 10 && bEventKind <= 11 )
				nEffectType = 3;

			// 발동 이펙트
			_EFFECTPACKAGE* pEffectPackage1 = g_EffectManager.EnqAppearEffectImmediately( eAttackKindItem_start + nEffectType );

			if( pEffectPackage1 && pEffectPackage1->pEffectRender && pEffectPackage1->pEffectRender->pPackagePair )
			{
                pEffectPackage1->pEffectRender->pPackagePair->pWorldMatrix = (MATRIX*)pCharObject->m_CharRender.GetCharTM();

				// 이케해야 캐릭터가 없어질때 이펙트도 같이 없어진다.
				pCharObject->m_EffectPPList.push_back( pEffectPackage1->pEffectRender->pPackagePair );
			}

			// 지속 이펙트
			// 같은 종류의 아이템을 벌써 쓰고 있으면 이펙트가 있는 상태다.
			if( NULL == pCharObject->m_pEventItemEffectPP )
			{
				_EFFECTPACKAGE* pEffectPackage2 = g_EffectManager.EnqOutGongPersistEffectImmediately( eAttackKindItem_keepup + nEffectType );

				if( pEffectPackage2 && pEffectPackage2->pEffectRender && pEffectPackage2->pEffectRender->pPackagePair )
				{
					pEffectPackage2->pEffectRender->pPackagePair->pWorldMatrix = (MATRIX*)pCharObject->m_CharRender.GetCharTM();

					pCharObject->m_pEventItemEffectPP = pEffectPackage2->pEffectRender->pPackagePair;
				}// if
			}

		}// if( pXiahObject )
*/
	}
	else
	{
		//g_MainCharInfo.m_pImageScrMsg->DelScrMsg(bEventKind);
		g_MainCharInfo.m_pImageScrMsg->Hide(bEventKind);

/*
		// 시간이 다 됐다. 이벤트 아이템의 이펙트를 지워준다.
		XiahObject::CXiahObject* pXiahObject = XiahObject::g_XiahObjectManager.FindXiahObject( MAKEOBJECTID( 0, dwEventCharID,OBJTYPE_PC) );
		if( pXiahObject && pXiahObject->m_pObject )
		{
			CXiahCharObject *pCharObject = reinterpret_cast<CXiahCharObject*>(pXiahObject->m_pObject);
			if( pCharObject->m_pEventItemEffectPP )
			{
				g_EffectManager.DeqEffectPackagePair( pCharObject->m_pEventItemEffectPP );

				pCharObject->m_pEventItemEffectPP = NULL;
			}
		}// if( pXiahObject )
*/

	}

	return 0;
}
//bEventStatus  - 0, EVENTSTATUS_START, 1, EVENTSTATUS_END
//bEventKind	- 이벤트 종류 
//if ( EVENTSTATUS_START )



/**
 * @brief 각성제 사용 (각성)
 * \param &msg 
 * \return 
 */
int OnCS_IM_REBIRTH_ACK(CMsg &msg)
{
	
	BYTE bResult = 0;	

	msg
		>> bResult;		

	const float fAniSpeed = 0.75f;

    switch(bResult)
	{
	case ERR_REBIRTH_SUCCESS:	//각성제 사용 조건 충족 성공
		{
			DWORD dwCharID = 0;
			BYTE  bRebirth = 0;
			msg
				>> dwCharID
				>> bRebirth;

			CXiahCharObject* pObject = NULL;

			if(dwCharID == g_MainCharInfo.m_dwObjectID)
			{
				if(g_pMainChar)
				{
					CloseAllWindow();

					g_MainCharInfo.m_dwXiahObjectID = g_pMainChar->m_ddwObjectID;

					pObject = (CXiahCharObject*)g_pMainChar->m_pObject;					
				}				
			}
			else
			{
				XiahObject::CXiahObject *pXiahObject = XiahObject::g_XiahObjectManager.FindXiahObject(MAKEOBJECTID(0, dwCharID, OBJTYPE_PC));
				if(pXiahObject == NULL)
					return 0;

				pObject = (CXiahCharObject*)pXiahObject->m_pObject;
			}

			if(pObject)
			{
				pObject->SetAnimation(XiahAniType::eLAT_Rebirth, XiahAniType::eLAT_Rebirth, 0, 1, fAniSpeed);
				pObject->m_CharRender.SetLoopAnimation(FALSE);
				pObject->m_bRebirth = bRebirth;
			}

			return 0;
		}
		break;
	case ERR_REBIRTH_LEVEL:		//요구조건 부족(레벨)
		{
			g_MainCharInfo.ShowHelpMessage(IDS_REBIRTH_LEVEL, TEXTEFFECT_COLOR_WARNING);
		}
		break;
	case ERR_REBIRTH_STEP:		//요구조건 부족(스텝)
		{
			g_MainCharInfo.ShowHelpMessage(IDS_REBIRTH_STEP, TEXTEFFECT_COLOR_WARNING);
		}
		break;
	case ERR_REBIRTH_SPACE:		//요구조건 부족(행낭 여유공간 부족)
		{
			g_MainCharInfo.ShowHelpMessage(IDS_REBIRTH_SPACE_01, TEXTEFFECT_COLOR_WARNING);
			g_MainCharInfo.ShowHelpMessage(IDS_REBIRTH_SPACE_02, TEXTEFFECT_COLOR_WARNING);
		}
		break; 
	case ERR_REBIRTH_INTERNAL:	//내부에러
		{
			g_MainCharInfo.ShowHelpMessage(IDS_INTERNAL_ERROR, TEXTEFFECT_COLOR_WARNING);
		}
		break;
	case 5:
		{
			g_MainCharInfo.ShowHelpMessage(IDS_LEVEL_ERROR, TEXTEFFECT_COLOR_WARNING);
		}
		break;
	case ERR_2TH_REBIRTH_SPACE: //HO_0730_07 진각성 패킷추가 : 요구조건 부족(행낭 여유공간 부족)
		{
			g_MainCharInfo.ShowHelpMessage(IDS_2TH_REBIRTH_SPACE, TEXTEFFECT_COLOR_WARNING);
		}
		break;

	default:
		break;
	}

	// 각성 실패시
//	g_MainCharInfo.m_bRebirthItem_Use = false;

	return 1;
}

//HT_1116 : 각성자 아이템 추가
/**
 * @brief 각성자 아이템 
 * \param &msg 
 * \return 
 */
int OnCS_IM_MAKEREBIRTHITEM_ACK(CMsg &msg)
{
	BYTE bResult = 0;
	BYTE bFactor = 0;
	WORD wValues = 0;

	msg
		>> bResult
		>> bFactor
		>> wValues;

	switch(bResult)
	{
	case ERR_REBIRTH_SUCCESS:
		g_MainCharInfo.ShowHelpMessage(IDS_MIXTURE_SUCCESS);
		g_MainCharInfo.HideSack(SACKTYPE__SMELT);
		break;
	case ERR_REBIRTH_CANT:
		{
			g_MainCharInfo.ShowHelpMessage(IDS_MIXTURE_CANT, TEXTEFFECT_COLOR_WARNING);
			g_MainCharInfo.HideSack(SACKTYPE__SMELT);
			return 0;
		}
		break;
	case ERR_REBIRTH_FAIL:
		{
			if(bFactor != 255)	
			{
				TCHAR strMsg[128] = {0,};

				if(bFactor == 14)
				{
					_stprintf(strMsg, IDS_FACTOR_15, wValues);
				}
				else if(bFactor == 15)
				{
					_stprintf(strMsg, IDS_FACTOR_16, wValues);
				}
				else
				{
					LPCTSTR lpstr = NULL;

					switch(bFactor)
					{
					case 0: lpstr = IDS_FACTOR_01;		break;
					case 1:	lpstr = IDS_FACTOR_02;		break;
					case 2:	lpstr = IDS_FACTOR_03;		break;
					case 3:	lpstr = IDS_FACTOR_04;		break;
					case 4:	lpstr = IDS_FACTOR_05;		break;
					case 5:	lpstr = IDS_FACTOR_06;		break;
					case 6:	lpstr = IDS_FACTOR_07;		break;
					case 7:	lpstr = IDS_FACTOR_08;		break;
					case 8:	lpstr = IDS_FACTOR_09;		break;
					case 9:	lpstr = IDS_FACTOR_10;		break;
					case 10:lpstr = IDS_FACTOR_11;		break;
					case 11:lpstr = IDS_FACTOR_12;		break;
					case 12:lpstr = IDS_FACTOR_13;		break;
					case 13:lpstr = IDS_FACTOR_14;		break;
					}

					LPCTSTR lpstrValue = NULL;

					lpstrValue = IDS_FACTOR_DOWN;
					
					_stprintf(strMsg, _T("%s %d %s"), lpstr, wValues, lpstrValue);
				}
				
				g_MainCharInfo.ShowHelpMessage(strMsg);
				g_MainCharInfo.HideSack(SACKTYPE__SMELT);
				return 0;
			}		
		break;
		}
	case ERR_REBIRTH_OVERCREATE:
		{
			g_MainCharInfo.ShowHelpMessage(IDS_REBIRTHITEM_OVERCREATE, TEXTEFFECT_COLOR_WARNING);
			g_MainCharInfo.HideSack(SACKTYPE__SMELT);
			return 0;
		}
	}
	return 1;
}

int OnCS_IM_GIVEPOWERITEM_ACK(CMsg &msg) //HO_0427_07 영수환골신단
{
	BYTE bResult = 0;	

	msg
		>> bResult;
	if(bResult == 0)
	{
		BYTE bFactor = 0;
		BYTE bValue = 0;
		DWORD dwPetID		=0;//HO_0427_07 영수환골신단 : 추가함
		TCHAR szTip [64] = {0,};
	
	msg
		>> bFactor
		>> bValue
		>> dwPetID;		
		
	g_MainCharInfo.ShowHelpMessage( IDS_FEED);

	switch (bFactor)
		{
		case ERR_GIVEPOWER_ATT:
			{
				_stprintf( szTip, IDS_MONSTER_STATUS_UP, IDS_STR_PWR, bValue);				
				g_MainCharInfo.ShowHelpMessage(szTip, TEXTEFFECT_COLOR_GAIN);				
			}
			break;
		case ERR_GIVEPOWER_DEF:
			{
				_stprintf( szTip, IDS_MONSTER_STATUS_UP, IDS_DEF_PWR, bValue);				
				g_MainCharInfo.ShowHelpMessage(szTip, TEXTEFFECT_COLOR_GAIN);				
			}
			break;
		case ERR_GIVEPOWER_AGI:
			{
				_stprintf( szTip, IDS_MONSTER_STATUS_UP, IDS_AGI_PWR, bValue);				
				g_MainCharInfo.ShowHelpMessage(szTip, TEXTEFFECT_COLOR_GAIN);				
			}
			break;
		case ERR_GIVEPOWER_LIFE:
			{
				_stprintf( szTip, IDS_MONSTER_STATUS_UP, IDS_LIFE_PWR, bValue);				
				g_MainCharInfo.ShowHelpMessage(szTip, TEXTEFFECT_COLOR_GAIN);				
			}
			break;
		case ERR_PET_REBIRTH:	//HO_0913_07 영수 각성신단 추가
			{
				_stprintf( szTip, IDS_MONSTER_REBIRTH);				
				g_MainCharInfo.ShowHelpMessage(szTip, TEXTEFFECT_COLOR_GAIN);				

				//HO_0913_07 영수 각성신단 추가
				XiahObject::CXiahObject* pObject = XiahObject::g_XiahObjectManager.FindXiahObject( MAKEOBJECTID( 0, dwPetID, OBJTYPE_PET));
				
				CXiahCharObject* pCharObject = reinterpret_cast<CXiahCharObject*>(pObject->m_pObject);

				if( NULL == pCharObject )
						return FALSE;	

				_EFFECTPACKAGE* pEffectPackage = g_EffectManager.EnqAppearEffectImmediately( eAttackKindItem_start, 0, 0, 10, 0 );//HO_0913_07 영수 각성신단 추가

				if( pEffectPackage && pEffectPackage->pEffectRender && pEffectPackage->pEffectRender->pPackagePair )
				{
					pEffectPackage->pEffectRender->pPackagePair->WorldMatrix = (MATRIX)pCharObject->m_ObjectTM;
				}			
			}
			break;
		case ERR_MONSTER_NOTREBIRTH: //HO_0913_07 영수 각성신단 추가
		{			
			g_MainCharInfo.ShowHelpMessage(IDS_MONSTER_NOTREBIRTH,TEXTEFFECT_COLOR_WARNING);
			g_MainCharInfo.m_pHoldItem->SetItemBackToSack();
		}		
		default:
			break;
		}
	}
		
	else
		g_MainCharInfo.ShowHelpMessage(IDS_MONSTER_STATUS_FAIL, TEXTEFFECT_COLOR_WARNING);		
	
	return 0;
}

int OnCS_IM_GOLDBOX_ACK(CMsg &msg) //HO_0828_07 황금열쇠 추가
{
	BYTE bResult = 0;	
	msg
		>> bResult;
	
	switch (bResult)
	{

	case ERR_GOLDBOX_SUCCESS:
		{
			sString strCharName;
			WORD wAmount = 0;
			TCHAR szTip [128] = {0,};
			
			if(bResult == 0)
			{
				msg
					>> strCharName
					>> wAmount;		
			}
			_stprintf( szTip, IDS_GOLDBOX_ITEM, strCharName.data(), wAmount);				
			g_MainCharInfo.ShowHelpMessage(szTip, TEXTEFFECT_COLOR_GAIN);				
		}
		break;
	case ERR_GOLDBOX_NOTENOUGH:
		{
			g_MainCharInfo.ShowHelpMessage(IDS_GOLDBOX_NOTENOUGH, TEXTEFFECT_COLOR_WARNING);				
		}
		break;
	case ERR_GOLDBOX_NOTKEY:
		{
			g_MainCharInfo.ShowHelpMessage(IDS_GOLDBOX_NOTKEY1, TEXTEFFECT_COLOR_WARNING);				
			g_MainCharInfo.ShowHelpMessage(IDS_GOLDBOX_NOTKEY2, TEXTEFFECT_COLOR_WARNING);				
		}
		break;
	default:
		break;
	}
	
	return 0;
}

