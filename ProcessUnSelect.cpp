
/**
 *
 * \param byPosition 
 * \return 
 */
BOOL CSack::ProcessItemUnSelected( BYTE byPosition)
{
	if( !g_MainCharInfo.m_pHoldItem)
		return TRUE;

	if( g_MainCharInfo.m_pHoldItem->IsHoldingItemItem())
		return ProcessSackUnSelected( byPosition);
	else if( g_MainCharInfo.m_pHoldItem->IsHoldingItemMoney())
		return ProcessMoneyUnSelected( byPosition);
	
	return TRUE;
}

/**
 *
 * \param byPosition 
 * \return 
 */
BOOL CSack::ProcessSackUnSelected( BYTE byPosition)
{
	XiahItem::sItemInfo* pHoldItem = g_MainCharInfo.m_pHoldItem->GetHoldItemItem();

	if(!pHoldItem)
		return TRUE;

	BYTE bSrcID = pHoldItem->m_bSackID;
	BYTE bDesID = this->m_bySackType;

	if( bDesID == bSrcID && pHoldItem->m_bSackPos == byPosition)
	{
        if( bDesID == SACKTYPE__DEFAULT)
		{
			if( pHoldItem->m_bSackCount == g_MainCharInfo.m_byMySackCurrIdx)
			{
				g_MainCharInfo.m_pHoldItem->SetItemBackToSack();

				if(g_pUIManager->IsShow(WINDOW_VOLUME))
					g_MainCharInfo.CloseFrame( WINDOW_VOLUME);

				return TRUE;
			}
		}
		else
		{			
			g_MainCharInfo.m_pHoldItem->SetItemBackToSack();

			if(g_pUIManager->IsShow(WINDOW_VOLUME))
				g_MainCharInfo.CloseFrame( WINDOW_VOLUME);

			return TRUE;
		}
	}		

	if(g_pUIManager->IsShow(WINDOW_VOLUME))
	{
		g_MainCharInfo.ShowHelpMessage( IDS_PUT_AMOUNT);

		g_pUIManager->SetFocus(WINDOW_VOLUME);
		g_pUIManager->SetFocus(WINDOW_VOLUME, volume_window_edit);

		return TRUE;
	} // if(g_pUIManager->IsShow(WINDOW_VOLUME))

	// [3/8/2004]
	if(g_pUIManager->IsShow(WINDOW_MONEY))
	{
		g_MainCharInfo.ShowHelpMessage( IDS_PUT_AMOUNT);

		g_pUIManager->SetFocus(WINDOW_MONEY);
		g_pUIManager->SetFocus(WINDOW_MONEY, money_window_edit);

		return TRUE;
	} // if(g_pUIManager->IsShow(WINDOW_MONEY))

	if( bDesID == SACKTYPE__PC_TRADE_MINE && g_MainCharInfo.m_bTradeAgree)
	{
		g_MainCharInfo.ShowHelpMessage( IDS_CANNOT_MOVE_AFTER_AGREE, TEXTEFFECT_COLOR_WARNING);

		return TRUE;
	}

	// 임시랍니다
	if( !(bDesID == SACKTYPE__DEFAULT || bDesID == SACKTYPE__EQUIPMENT))
	{
		if( pHoldItem->m_bItemType == ITEMTYPE_BONGIN && pHoldItem->m_dwNpcID && (pHoldItem->m_wLevel != 2 && pHoldItem->m_wLevel != 3))
		{
			g_MainCharInfo.ShowHelpMessage(IDS_CANNOT_DEAL_MONSTER, TEXTEFFECT_COLOR_WARNING);

			return TRUE;
		}
	}
	
	/////////////////////////////////////////////////////////////////////////////////////////////////////
	// 여기는 기본 행낭에서 다른 쪽으로 이동

	if( bSrcID == SACKTYPE__DEFAULT && bDesID == SACKTYPE__DEFAULT)
		return ProcessItemUnSelected_Default_Default( byPosition, pHoldItem);
	else if( bSrcID == SACKTYPE__DEFAULT && bDesID == SACKTYPE__NPC_TRADE)
		return ProcessItemUnSelected_Default_Shop( byPosition, pHoldItem);
	else if( bSrcID == SACKTYPE__DEFAULT && bDesID == SACKTYPE__PC_TRADE_MINE)		// 본인행낭->거래창
		return ProcessItemUnSelected_Default_TradeMine( byPosition, pHoldItem);
	else if( bSrcID == SACKTYPE__DEFAULT && bDesID == SACKTYPE__DEPOSIT)
		return ProcessItemUnSelected_Default_Deposit( byPosition, pHoldItem);
	else if( bSrcID == SACKTYPE__DEFAULT && bDesID == SACKTYPE__MODIFY)
		return ProcessItemUnSelected_Default_Modify( byPosition, pHoldItem);
	else if( bSrcID == SACKTYPE__DEFAULT && bDesID == SACKTYPE__EQUIPMENT)
		return ProcessItemUnSelected_Default_Equip( byPosition, pHoldItem);
	else if( bSrcID == SACKTYPE__DEFAULT && bDesID == SACKTYPE__PET)
		return ProcessItemUnSelected_Default_Pet( byPosition, pHoldItem);
	else if( bSrcID == SACKTYPE__DEFAULT && bDesID == SACKTYPE__PET_EQUIP)
		return ProcessItemUnSelected_Default_PetEquip( byPosition, pHoldItem);
	else if( bSrcID == SACKTYPE__DEFAULT && bDesID == SACKTYPE__PERSONAL_TRADE_SET) // 본인->개인상점설정
		return ProcessItemUnSelected_Default_Personal_Trade_Set(byPosition, pHoldItem);
	else if( bSrcID == SACKTYPE__DEFAULT && bDesID == SACKTYPE__ITEMMALL)			// DEFAULT -> ITEMMALL
		return ProcessItemUnSelected_Default_To_Itemmall(byPosition, pHoldItem);
	else if(bSrcID == SACKTYPE__DEFAULT && bDesID == SACKTYPE__SMELT)				// 본인 -> 조합
		return ProcessItemUnSelected_Default_Smelt(byPosition, pHoldItem);
	else if(bSrcID == SACKTYPE__DEFAULT && bDesID == SACKTYPE__FIVEELEMENT_CONVERT)	// 본인 -> 오행 제련
		return ProcessItemUnSelected_Default_FE_Convert(byPosition, pHoldItem);
	else if(bSrcID == SACKTYPE__DEFAULT && bDesID == SACKTYPE__QUICKMART)			// 본인 -> 매품패
		return ProcessItemUnSelected_Default_QuickMart(byPosition, pHoldItem);
	else if(bSrcID == SACKTYPE__DEFAULT && bDesID == SACKTYPE__COLLECTION)			// 본인 -> 아이템 수집
		return ProcessItemUnSelected_Default_Collection(byPosition, pHoldItem);
	
	/////////////////////////////////////////////////////////////////////////////////////////////////////
	// 위에서 한거 이외의 것

	else if( bSrcID == SACKTYPE__EQUIPMENT && bDesID == SACKTYPE__EQUIPMENT)
		return ProcessItemUnSelected_Equip_Equip( byPosition, pHoldItem);
	else if( bSrcID == SACKTYPE__NPC_TRADE && bDesID == SACKTYPE__DEFAULT)
		return ProcessItemUnSelected_Shop_Default( byPosition, pHoldItem);
	else if( bSrcID == SACKTYPE__NPC_TRADE && bDesID == SACKTYPE__NPC_TRADE)
		return ProcessItemUnSelected_Shop_Shop( byPosition, pHoldItem);
	else if( bSrcID == SACKTYPE__PC_TRADE_MINE && bDesID == SACKTYPE__DEFAULT)
		return ProcessItemUnSelected_TradeMine_Default( byPosition, pHoldItem);
	else if( bSrcID == SACKTYPE__DEPOSIT && bDesID == SACKTYPE__DEFAULT)
		return ProcessItemUnSelected_Deposit_Default( byPosition, pHoldItem);
	else if( bSrcID == SACKTYPE__DEPOSIT && bDesID == SACKTYPE__DEPOSIT)
		return ProcessItemUnSelected_Deposit_Deposit( byPosition, pHoldItem);
	else if( bSrcID == SACKTYPE__EQUIPMENT && bDesID == SACKTYPE__DEFAULT)
		return ProcessItemUnSelected_Equip_Default( byPosition, pHoldItem);
	else if( bSrcID == SACKTYPE__PET && bDesID == SACKTYPE__DEFAULT)
		return ProcessItemUnSelected_Pet_Default( byPosition, pHoldItem);
	else if( bSrcID == SACKTYPE__PET_EQUIP && bDesID == SACKTYPE__DEFAULT)
		return ProcessItemUnSelected_PetEquip_default( byPosition, pHoldItem);
	else if( bSrcID == SACKTYPE__PET && bDesID == SACKTYPE__PET)
		return ProcessItemUnSelected_Pet_Pet( byPosition, pHoldItem);
	else if( bSrcID == SACKTYPE__PERSONAL_TRADE_SELL && bDesID == SACKTYPE__DEFAULT)			// 타 개인상점=>본인 (구입)
		return ProcessItemUnSelected_Personal_TradeSell_Default(byPosition, pHoldItem);
	else if( bSrcID == SACKTYPE__PERSONAL_TRADE_SET && bDesID == SACKTYPE__DEFAULT)				// 개인상점설정 => 본인행낭
		return ProcessItemUnSelected_Personal_Trade_Set_Default(byPosition, pHoldItem);
	else if( bSrcID == SACKTYPE__PERSONAL_TRADE_SET && bDesID == SACKTYPE__PERSONAL_TRADE_SET)  // 개인상점설정 => 개인상점설정
		return ProcessItemUnSelected_Personal_Trade_Set_Trade_Set(byPosition, pHoldItem);
	else if( bSrcID == SACKTYPE__ITEMMALL && bDesID == SACKTYPE__DEFAULT)						// 아이템 몰에서 행낭으로
		return ProcessItemUnSelected_Itemmall_To_Default(byPosition, pHoldItem);
	else if( bSrcID == SACKTYPE__ITEMMALL && bDesID == SACKTYPE__ITEMMALL)						// 아이템 몰에서 아이멜몰로 자리이동
		return ProcessItemUnSelected_Itemmall_To_Itemmall(byPosition, pHoldItem);
	else if(bSrcID == SACKTYPE__COLLECTION && bDesID == SACKTYPE__DEFAULT)						// 아이템 수집 -> 행낭
		return ProcessItemUnSelected_Collection_Default(byPosition, pHoldItem);

	return TRUE;
}

/**
 *
 * \param byPosition 
 * \return 
 */
BOOL CSack::ProcessMoneyUnSelected( BYTE byPosition)
{
	XiahItem::sHoldMoney* pMoney = g_MainCharInfo.m_pHoldItem->GetHoldItemMoney();
		
	BYTE bSrcID = pMoney->m_bySrcSackID;
	BYTE bDesID = this->m_bySackType;

	if( bSrcID == SACKTYPE__DEFAULT && bDesID == SACKTYPE__DEFAULT)
	{
		g_MainCharInfo.m_pHoldItem->ReleaseHoldItemMoney();
	}
	else if( bSrcID == SACKTYPE__DEFAULT && bDesID == SACKTYPE__PC_TRADE_MINE)
	{
		if( g_MainCharInfo.m_dwPickedObject)
			SendCS_EC_TRADESACKONMONEY_REQ( g_MainCharInfo.m_dwPickedObject, pMoney->m_dwAmount);
		else
			SendCS_EC_TRADESACKONMONEY_REQ( g_MainCharInfo.m_dwAskID, pMoney->m_dwAmount);
	}
	else if( bSrcID == SACKTYPE__PC_TRADE_MINE && bDesID == SACKTYPE__DEFAULT)
	{
		if( g_MainCharInfo.m_dwPickedObject)
			SendCS_EC_TRADESACKOFFMONEY_REQ( g_MainCharInfo.m_dwPickedObject, pMoney->m_dwAmount);
		else
			SendCS_EC_TRADESACKOFFMONEY_REQ( g_MainCharInfo.m_dwAskID, pMoney->m_dwAmount);
	}

	return TRUE;
}























////////////////////////////////////////////////////////////////////////////////////////////////////
BOOL CSack::ProcessItemUnSelected_Default_Default( BYTE byPosition, XiahItem::sItemInfo* pHoldItem)
////////////////////////////////////////////////////////////////////////////////////////////////////
{
	DWORD dwDesItemID = 0;
	XiahItem::sItemInfo* pDesItem = FindSackItemByPos( byPosition);					
	if( pDesItem)
	{
		dwDesItemID = pDesItem->m_dwItemID;

		// 물약, 동신주일때.
		if( ( pHoldItem->m_bItemType == ITEMTYPE_POTION && pDesItem->m_bItemType == ITEMTYPE_POTION  && pHoldItem->m_wRefID == pDesItem->m_wRefID   ) ||
            ( pHoldItem->m_bItemType == ITEMTYPE_PORTAL && pDesItem->m_bItemType == ITEMTYPE_PORTAL  && pHoldItem->m_wRefID == pDesItem->m_wRefID   ) ||
			( pHoldItem->m_bItemType == ITEMTYPE_GOLDKEY && pDesItem->m_bItemType == ITEMTYPE_GOLDKEY  && pHoldItem->m_wRefID == pDesItem->m_wRefID   ) ) //HO_0828_07 황금열쇠 추가
		{
			SendCS_IM_MERGERES_REQ( pHoldItem->m_bSackCount+1,
									pHoldItem->m_bSackPos, 
									pHoldItem->m_dwItemID,
									g_MainCharInfo.m_byMySackCurrIdx+1,
									byPosition, 
									dwDesItemID);
		}
		else
		{
			SendCS_IM_MOVE_REQ( pHoldItem->m_bSackCount+1,//pHoldItem->m_bSackID, 
								pHoldItem->m_bSackPos, 
								pHoldItem->m_dwItemID,
								g_MainCharInfo.m_byMySackCurrIdx + 1,//SACKTYPE__DEFAULT, 
								pDesItem->m_bSackPos,//byPosition, 
								dwDesItemID);
		}

/*
		if( pHoldItem->m_dwItemID == pDesItem->m_dwItemID)
		{
			SendCS_IM_MERGERES_REQ( pHoldItem->m_bSackCount+1,
									pHoldItem->m_bSackPos, 
									pHoldItem->m_dwItemID,
									g_MainCharInfo.m_byMySackCurrIdx+1,
									byPosition, 
									dwDesItemID);
		}
		else
		{
			SendCS_IM_MOVE_REQ( pHoldItem->m_bSackCount+1,//pHoldItem->m_bSackID, 
								pHoldItem->m_bSackPos, 
								pHoldItem->m_dwItemID,
								g_MainCharInfo.m_byMySackCurrIdx + 1,//SACKTYPE__DEFAULT, 
								pDesItem->m_bSackPos,//byPosition, 
								dwDesItemID);
		}
*/

	}
	else			
	{
		DWORD dwAmount = g_MainCharInfo.m_dwVolumeSplitAmount;

		if( dwAmount > 0)
		{
			if( dwAmount >= pHoldItem->m_dwAmount)
			{
				//g_MainCharInfo.ShowHelpMessage( IDS_OVER_AMOUNT,TEXTEFFECT_COLOR_WARNING);
				//g_MainCharInfo.OpenFrame( WINDOW_VOLUME, 2);
				// warning 내지 말고 move 시키기
				SendCS_IM_MOVE_REQ( pHoldItem->m_bSackCount+1,//pHoldItem->m_bSackID, 
								pHoldItem->m_bSackPos, 
								pHoldItem->m_dwItemID,
								g_MainCharInfo.m_byMySackCurrIdx + 1,//SACKTYPE__DEFAULT, 
								byPosition, 
								dwDesItemID);
			}
			else
			{
				SendCS_IM_SPLITRES_REQ( pHoldItem->m_bSackCount+1,
										pHoldItem->m_bSackPos, 
										pHoldItem->m_dwItemID,
										g_MainCharInfo.m_byMySackCurrIdx+1,
										byPosition, 
										dwAmount);

				pHoldItem->m_dwAmount -= dwAmount;
				g_MainCharInfo.m_pHoldItem->SetItemBackToSack();
			}
		}
		else
		{
			SendCS_IM_MOVE_REQ( pHoldItem->m_bSackCount+1,//pHoldItem->m_bSackID, 
								pHoldItem->m_bSackPos, 
								pHoldItem->m_dwItemID,
								g_MainCharInfo.m_byMySackCurrIdx + 1,//SACKTYPE__DEFAULT, 
								byPosition, 
								dwDesItemID);
		}

		g_MainCharInfo.m_dwVolumeSplitAmount = 0;
	}

	return TRUE;
}

/**
 * 본인행낭->NPC행낭 (팔기)
 * \param byPosition 
 * \param pHoldItem 
 * \return 
 */
BOOL CSack::ProcessItemUnSelected_Default_Shop( BYTE byPosition, XiahItem::sItemInfo* pHoldItem)
{
	if((pHoldItem->m_bRarity >= g_MainCharInfo.m_bRarityLimit && g_MainCharInfo.m_bRarityLimit)
		|| (pHoldItem->m_bStxType >= g_MainCharInfo.m_bStxTypeLimit && g_MainCharInfo.m_bStxTypeLimit))
	{
		g_MainCharInfo.m_pHoldItem->SetDrawFlag(false);

		g_pUIManager->ShowNotice(IDS_LIMIT_SELL, NOTICE_FRAME_OKCANCEL, NOTICE_FRAME_LIMIT_SELL, XiahInput::g_ptMouse.x - 270, XiahInput::g_ptMouse.y-90);
	}
	else
	{	
		SendCS_EC_SELLITEM_REQ( g_MainCharInfo.m_dwPickedObject, 
								pHoldItem->m_dwItemID,
								pHoldItem->m_bSackCount+1,
								pHoldItem->m_bSackPos);
	}

	return TRUE;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
BOOL CSack::ProcessItemUnSelected_Default_Equip( BYTE byPosition, XiahItem::sItemInfo* pHoldItem)
////////////////////////////////////////////////////////////////////////////////////////////////////
{
	switch( pHoldItem->m_bItemType)
	{
	case ITEMTYPE_WEAPON:
		byPosition = EQUIPPOS_WEAPON;
		break;
	case ITEMTYPE_CLOTH:
		byPosition = EQUIPPOS_CLOTH;
		break;
	case ITEMTYPE_HAT:
		byPosition = EQUIPPOS_HAT;
		break;
	case ITEMTYPE_SHOE:
		byPosition = EQUIPPOS_SHOE;
		break;
	case ITEMTYPE_CLOAK:
		byPosition = EQUIPPOS_CLOAK;
		break;
	case ITEMTYPE_RING:
		byPosition = EQUIPPOS_RING;
		break;
	case ITEMTYPE_NECKLACE:
		byPosition = EQUIPPOS_NECLACE;
		break;
//	case ITEMTYPE_WEAPON1:
//		byPosition = EQUIPPOS_BONGIN;
//		break;
	}
	
	DWORD dwDesItemID = 0;
	XiahItem::sItemInfo* pDesItem = FindSackItemByPos( byPosition);					
	if( pDesItem)
		dwDesItemID = pDesItem->m_dwItemID;

	SendCS_IM_MOVE_REQ( pHoldItem->m_bSackCount+1,//pHoldItem->m_bSackID, 
						pHoldItem->m_bSackPos, 
						pHoldItem->m_dwItemID,
						SACKTYPE__EQUIPMENT, 
						byPosition, 
						dwDesItemID);
	return TRUE;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
BOOL CSack::ProcessItemUnSelected_Equip_Equip( BYTE byPosition, XiahItem::sItemInfo* pHoldItem)
////////////////////////////////////////////////////////////////////////////////////////////////////
{
	DWORD dwDesItemID = 0;
	XiahItem::sItemInfo* pDesItem = FindSackItemByPos( byPosition);					
	if( pDesItem)
		dwDesItemID = pDesItem->m_dwItemID;

	SendCS_IM_MOVE_REQ( pHoldItem->m_bSackID, 
						pHoldItem->m_bSackPos, 
						pHoldItem->m_dwItemID,
						SACKTYPE__EQUIPMENT, 
						byPosition, 
						dwDesItemID);
	return TRUE;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
BOOL CSack::ProcessItemUnSelected_Default_TradeMine( BYTE byPosition, XiahItem::sItemInfo* pHoldItem)
////////////////////////////////////////////////////////////////////////////////////////////////////
{
	if( g_MainCharInfo.m_dwPickedObject)
	{
		SendCS_EC_TRADESACKONITEM_REQ(	pHoldItem->m_bSackCount+1,//pHoldItem->m_bSackID, 
			pHoldItem->m_bSackPos, 
			SACKTYPE__PC_TRADE_MINE,
			byPosition,
			pHoldItem->m_dwItemID,
			pHoldItem->m_dwAmount,
			g_MainCharInfo.m_dwPickedObject);
	}
	else
	{
		SendCS_EC_TRADESACKONITEM_REQ(	pHoldItem->m_bSackCount+1,//pHoldItem->m_bSackID, 
			pHoldItem->m_bSackPos, 
			SACKTYPE__PC_TRADE_MINE,
			byPosition,
			pHoldItem->m_dwItemID,
			pHoldItem->m_dwAmount,
			g_MainCharInfo.m_dwAskID);
	}

	return TRUE;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
BOOL CSack::ProcessItemUnSelected_Default_Deposit( BYTE byPosition, XiahItem::sItemInfo* pHoldItem)
////////////////////////////////////////////////////////////////////////////////////////////////////
{
	/*
	SendCS_IM_CHECKITEMPRICE_REQ( 1,
								g_MainCharInfo.m_dwObjectID,
								pHoldItem->m_dwItemID,
								pHoldItem->m_bSackCount+1,
								pHoldItem->m_bSackPos);
*/

	XiahItem::sItemInfo* pItem = g_MainCharInfo.m_pHoldItem->GetHoldItemItem();

	SendCS_EC_DRAWINBANK_REQ( g_MainCharInfo.m_dwObjectID,
							pItem->m_dwItemID,
							pItem->m_bSackCount+1,
							pItem->m_bSackPos,
							byPosition,//255,
							pItem->m_dwAmount);

	return TRUE;
}

/**
 * 개조
 * \param byPosition 
 * \param pHoldItem 
 * \return 
 */
BOOL CSack::ProcessItemUnSelected_Default_Modify( BYTE byPosition, XiahItem::sItemInfo* pHoldItem)
{
	// 개조 하고자 하는 아이템 체크
	if( byPosition == 0)
	{
		XiahObject::CXiahObject* pObject = XiahObject::g_XiahObjectManager.FindXiahObject( MAKEOBJECTID( 0, g_MainCharInfo.m_dwPickedObject, OBJTYPE_FUNCTIONALNPC));

		if( pObject == NULL )
			return TRUE;

		if( pObject->m_pObject == NULL )
			return TRUE;

		CXiahCharObject* pCharObject = reinterpret_cast<CXiahCharObject*>( pObject->m_pObject);
		sFunctionalNpcInfo* pInfo = (sFunctionalNpcInfo*)pCharObject->m_pPrivateData;

		if( !pInfo)
			return TRUE;

		XiahItem::sItemInfo* pItem = g_MainCharInfo.m_pModifySack->FindSackItemByPos(0);
		if(pItem)
		{
			g_MainCharInfo.m_pHoldItem->SetItemBackToSack();
			return true;
		}

		switch( pInfo->m_bType)
		{
		case 1: //대장장이
			if( pHoldItem->m_bItemType != ITEMTYPE_WEAPON)
			{
				g_MainCharInfo.ShowHelpMessage(IDS_ONLY_SWORD, TEXTEFFECT_COLOR_WARNING);
				g_MainCharInfo.m_pHoldItem->SetItemBackToSack();
				return TRUE;
			}
			break;
		case 2: //의류상인
			if( pHoldItem->m_bItemType != ITEMTYPE_CLOTH)
			{
				g_MainCharInfo.ShowHelpMessage(IDS_ONLY_CLOTH, TEXTEFFECT_COLOR_WARNING);
				g_MainCharInfo.m_pHoldItem->SetItemBackToSack();
				return TRUE;
			}
			break;
		case 3: //잡화상인
			if( !(pHoldItem->m_bItemType == ITEMTYPE_HAT ||
				pHoldItem->m_bItemType == ITEMTYPE_SHOE))
			{
				g_MainCharInfo.ShowHelpMessage( IDS_CONVERT_JABWHA, TEXTEFFECT_COLOR_WARNING);
				g_MainCharInfo.m_pHoldItem->SetItemBackToSack();
				return TRUE;
			}
			break;
		case 4:	//보석상인
			if( !(pHoldItem->m_bItemType == ITEMTYPE_RING ||
				pHoldItem->m_bItemType == ITEMTYPE_NECKLACE))
			{
				g_MainCharInfo.ShowHelpMessage( IDS_CONVERT_JEWERY, TEXTEFFECT_COLOR_WARNING);
				g_MainCharInfo.m_pHoldItem->SetItemBackToSack();
				return TRUE;
			}
			break;
		}

		SendCS_IM_REBUILDITEMTERM_REQ( pHoldItem->m_dwItemID, pHoldItem->m_bSackCount+1, pHoldItem->m_bSackPos);
	}
	// 개조재료 체크
	else
	{
		// 메인 재료가 있는가?
		XiahItem::sItemInfo* pItem = g_MainCharInfo.m_pModifySack->FindSackItemByPos( 0);
		if( pItem)
		{
			DWORD dwResourceIDConstraint =0;
			BYTE bIsDividedRes = 0;

			// 자기보다 순위가 낮은 창에 뭔가가 없으면 리턴
			for( int i=1; i < byPosition; ++i)
			{
				XiahItem::sItemInfo* pItem = g_MainCharInfo.m_pModifySack->FindSackItemByPos( i);
				if( !pItem)
				{
					g_MainCharInfo.ShowHelpMessage( IDS_UPPER_NONE, TEXTEFFECT_COLOR_WARNING);
					g_MainCharInfo.m_pHoldItem->SetItemBackToSack();
					return TRUE;
				}
				// 뭔가가 있으면 그 넘의 아이디만 다른 창에 들어갈 수 있다
				else
				{
					dwResourceIDConstraint = pItem->m_wRefID; // m_dwItemID;
					bIsDividedRes = pItem->m_bIsDividedRes;
				}
			}

			if( !dwResourceIDConstraint)
			{
				// [10/26/2004] 개조 가능여부
				// [5/18/2005] 보험 아이템
				// HT_1116 : 각성자 아이템 추가 
				if( (pHoldItem->m_bItemType == ITEMTYPE_REBUILDRES && 
					1 != pHoldItem->m_bIsDividedRes && 
					!(pHoldItem->m_bItemKind >= 5 && pHoldItem->m_bItemKind <= 6 ) &&
					pHoldItem->m_bItemKind != 16 &&
					pHoldItem->m_bItemKind != 17) 
					|| (pHoldItem->m_bItemType == ITEMTYPE_POTION && pHoldItem->m_bItemKind == 1) )
				{
					// 개조재료 넣을창에 뭔가가 있으면 원래 위치로 돌려주고
					XiahItem::sItemInfo* pItem = g_MainCharInfo.m_pModifySack->FindSackItemByPos( byPosition);
					if( pItem)
					{
						pItem->m_bSackID = SACKTYPE__DEFAULT;
						g_MainCharInfo.m_pMySack[ pItem->m_bSackIDPrev]->InsertItem( pItem->m_bSackPosPrev, pItem);
						g_MainCharInfo.m_pModifySack->DeleteItem( byPosition);
					}

					// 홀드아이템 넣어주기
					g_MainCharInfo.m_pHoldItem->GetHoldItemItem()->m_bSackID = SACKTYPE__MODIFY;
					g_MainCharInfo.m_pHoldItem->GetHoldItemItem()->m_bSackIDPrev = g_MainCharInfo.m_pHoldItem->GetHoldItemItem()->m_bSackCount;
					g_MainCharInfo.m_pHoldItem->GetHoldItemItem()->m_bSackPosPrev = g_MainCharInfo.m_pHoldItem->GetHoldItemItem()->m_bSackPos;

					g_MainCharInfo.m_pModifySack->InsertItem( byPosition, pHoldItem);
					g_MainCharInfo.m_pHoldItem->EmptyHoldItemItem();
				}
				else
				{
					g_MainCharInfo.ShowHelpMessage( IDS_NOT_RESOURCETYPE_FOR_CONVERT, TEXTEFFECT_COLOR_WARNING);
					g_MainCharInfo.m_pHoldItem->SetItemBackToSack();
				}
			}
			else
			{
				if( pHoldItem->m_wRefID == dwResourceIDConstraint && pHoldItem->m_bIsDividedRes == bIsDividedRes)
				{
					// 개조재료 넣을창에 뭔가가 있으면 원래 위치로 돌려주고
					XiahItem::sItemInfo* pItem = g_MainCharInfo.m_pModifySack->FindSackItemByPos( byPosition);
					if( pItem)
					{
						pItem->m_bSackID = SACKTYPE__DEFAULT;
						g_MainCharInfo.m_pMySack[ pItem->m_bSackIDPrev]->InsertItem( pItem->m_bSackPosPrev, pItem);
						g_MainCharInfo.m_pModifySack->DeleteItem( byPosition);
					}

					// 홀드아이템 넣어주기
					g_MainCharInfo.m_pHoldItem->GetHoldItemItem()->m_bSackID = SACKTYPE__MODIFY;
					g_MainCharInfo.m_pHoldItem->GetHoldItemItem()->m_bSackIDPrev = g_MainCharInfo.m_pHoldItem->GetHoldItemItem()->m_bSackCount;
					g_MainCharInfo.m_pHoldItem->GetHoldItemItem()->m_bSackPosPrev = g_MainCharInfo.m_pHoldItem->GetHoldItemItem()->m_bSackPos;

					g_MainCharInfo.m_pModifySack->InsertItem( byPosition, pHoldItem);
					g_MainCharInfo.m_pHoldItem->EmptyHoldItemItem();
				}
				else
				{
					g_MainCharInfo.ShowHelpMessage( IDS_SAME_SOURCE_ABLE, TEXTEFFECT_COLOR_WARNING);
					g_MainCharInfo.m_pHoldItem->SetItemBackToSack();
				}
			}
		}
		else
		{
			g_MainCharInfo.ShowHelpMessage( IDS_PUT_CONVERTITEM, TEXTEFFECT_COLOR_WARNING);
			g_MainCharInfo.m_pHoldItem->SetItemBackToSack();
		}
	}

	return TRUE;
}

BOOL CSack::ProcessItemUnSelected_Default_Pet( BYTE byPosition, XiahItem::sItemInfo* pHoldItem)
{
	sPetInfo* pPetInfo = g_PetList.GetCurrentPet();

	if( pPetInfo)
	{
		SendCS_NC_PETITEMPUT_REQ( pPetInfo->dwID, 
								pHoldItem->m_dwItemID,
								pHoldItem->m_bSackCount+1, // SACKTYPE__DEFAULT,
								pHoldItem->m_bSackPos,
								pPetInfo->m_byMySackCurrIdx + 1,
								byPosition);
	}

	return TRUE;
}

BOOL CSack::ProcessItemUnSelected_Default_PetEquip( BYTE byPosition, XiahItem::sItemInfo* pHoldItem)
{
	sPetInfo* pPetInfo = g_PetList.GetCurrentPet();

	if( pPetInfo)
	{
		SendCS_NC_PETITEMPUT_REQ( pPetInfo->dwID, 
								pHoldItem->m_dwItemID,
								pHoldItem->m_bSackCount+1, // SACKTYPE__DEFAULT,
								pHoldItem->m_bSackPos,
								PETSACKTYPE_EQUIPMENT,//pPetInfo->m_byMySackCurrIdx,
								byPosition);
	}

	return TRUE;
}

// [3/5/2004]
bool CSack::ProcessItemUnSelected_Default_Personal_Trade_Set(BYTE byPosition, XiahItem::sItemInfo* pHoldItem)
{
	g_MainCharInfo.m_pHoldItem->m_bBackPosition = byPosition;
	g_MainCharInfo.m_pHoldItem->SetDrawFlag( false);

	g_pUIManager->SetString(WINDOW_MONEY, money_window_dummy_01, IDS_WINDOW_MONEY);
	g_pUIManager->SetString(WINDOW_MONEY, money_window_dummy_02, IDS_PT_SET_SELL_MONEY);

	g_MainCharInfo.OpenFrame(WINDOW_MONEY);
	g_pUIManager->SetPostMsg(WINDOW_MONEY_PCTRADE);

	return true;
}


// Default -> Itemm mall
BOOL CSack::ProcessItemUnSelected_Default_To_Itemmall(BYTE byPosition, XiahItem::sItemInfo* pHoldItem)
{
	// 이것은 100% 에러다! 이러면 안된다.
	g_MainCharInfo.m_pHoldItem->SetItemBackToSack();
	g_MainCharInfo.PlayInterfaceSound( ISOUND_WARNING);

	return TRUE;
}



/**
 * 개인상점에서 구입
 * \param byPosition 
 * \param pHoldItem 
 * \return 
 */
bool CSack::ProcessItemUnSelected_Personal_TradeSell_Default(BYTE byPosition, XiahItem::sItemInfo* pHoldItem)
{
	g_MainCharInfo.m_pHoldItem->m_bBackPosition = byPosition;
	g_MainCharInfo.m_pHoldItem->SetDrawFlag(false);

	TCHAR strText[256] = {0,};

	_stprintf(strText, IDS_PT_BUY, (LPCTSTR)pHoldItem->m_szName, MoneyCommaStr(pHoldItem->m_dwPrice).data());
	
	g_pUIManager->ShowNotice(strText, NOTICE_FRAME_OKCANCEL, NOTICE_FRAME_PT_BUY, XiahInput::g_ptMouse.x - 270, XiahInput::g_ptMouse.y-90);

	return true;
}

/**
 * 개인상점 설정에서 본인행낭
 * \param byPosition 
 * \param pHoldItem 
 * \return 
 */
bool CSack::ProcessItemUnSelected_Personal_Trade_Set_Default(BYTE byPosition, XiahItem::sItemInfo* pHoldItem)
{
	g_MainCharInfo.m_pHoldItem->m_bBackPosition = byPosition;

	// 개인상점설정=>본인행낭
	SendCS_SH_DELSHOP_REQ(pHoldItem->m_bSackPos,
						  pHoldItem->m_dwItemID,
						  g_MainCharInfo.m_byMySackCurrIdx+1,
						  byPosition);

	return true;
}

// 개인상점내 아이템 이동
bool CSack::ProcessItemUnSelected_Personal_Trade_Set_Trade_Set(BYTE byPosition, XiahItem::sItemInfo* pHoldItem)
{
	XiahItem::sItemInfo* pDesItem = FindSackItemByPos( byPosition);

	if(pDesItem)
	{
		SendCS_SH_MOVESHOP_REQ(pHoldItem->m_bSackPos, pHoldItem->m_dwItemID,
								pDesItem->m_bSackPos, pDesItem->m_dwItemID);
	}
	else
	{
		SendCS_SH_MOVESHOP_REQ(pHoldItem->m_bSackPos, pHoldItem->m_dwItemID,
								byPosition, 0);
	}

	return true;
}

BOOL CSack::ProcessItemUnSelected_Equip_Default( BYTE byPosition, XiahItem::sItemInfo* pHoldItem)
{
	SendCS_IM_MOVE_REQ( pHoldItem->m_bSackID, 
						pHoldItem->m_bSackPos, 
						pHoldItem->m_dwItemID,
						g_MainCharInfo.m_byMySackCurrIdx + 1,//SACKTYPE__DEFAULT,
						byPosition, 
						0);	

	return TRUE;
}

BOOL CSack::ProcessItemUnSelected_Pet_Default( BYTE byPosition, XiahItem::sItemInfo* pHoldItem)
{
	sPetInfo* pPetInfo = g_PetList.GetCurrentPet();

	if( pPetInfo)
	{
		SendCS_NC_PETITEMOUT_REQ( pPetInfo->dwID,
								pHoldItem->m_dwItemID,
								pPetInfo->m_byMySackCurrIdx+1,
								pHoldItem->m_bSackPos,
								g_MainCharInfo.m_byMySackCurrIdx + 1,// SACKTYPE__DEFAULT,
								byPosition);
	}

	return TRUE;
}

BOOL CSack::ProcessItemUnSelected_PetEquip_default( BYTE byPosition, XiahItem::sItemInfo* pHoldItem)
{
	sPetInfo* pPetInfo = g_PetList.GetCurrentPet();

	if( pPetInfo)
	{
		SendCS_NC_PETITEMOUT_REQ( pPetInfo->dwID,
								pHoldItem->m_dwItemID,
								PETSACKTYPE_EQUIPMENT,
								pHoldItem->m_bSackPos,
								g_MainCharInfo.m_byMySackCurrIdx + 1,
								byPosition);
	}

	return TRUE;
}

BOOL CSack::ProcessItemUnSelected_Pet_Pet( BYTE byPosition, XiahItem::sItemInfo* pHoldItem)
{
	sPetInfo* pPetInfo = g_PetList.GetCurrentPet();

	if( pPetInfo)
	{
		SendCS_NC_PETITEMMOVE_REQ( pPetInfo->dwID,
								pHoldItem->m_dwItemID,
								pHoldItem->m_bSackCount,
								pHoldItem->m_bSackPos,
								pPetInfo->m_byMySackCurrIdx + 1,
								byPosition);
	}

	return TRUE;
}

/**
 * NPC -> 구입
 * \param byPosition 
 * \param pHoldItem 
 * \return 
 */
BOOL CSack::ProcessItemUnSelected_Shop_Default( BYTE byPosition, XiahItem::sItemInfo* pHoldItem)
{
	DWORD Amount = g_MainCharInfo.m_dwVolumeSplitAmount;
	if(0 == Amount)
	{
		// 한개 살때 무조건 1개로 집는다
		Amount = 1;
	}

	// 수량을 넣자!
	//if(pHoldItem->m_dwAmount != 10)
	//	g_MainCharInfo.m_pHoldItem->GetHoldItemItem()->m_dwAmount = Amount;
	if(pHoldItem->m_dwAmount == 10)
		g_MainCharInfo.m_pHoldItem->GetHoldItemItem()->m_dwBuyCount = 10;
	else
		g_MainCharInfo.m_pHoldItem->GetHoldItemItem()->m_dwBuyCount = Amount;

	// 거래 옵션
	if(pHoldItem->m_dwPrice >= g_MainCharInfo.m_dwBuyLimit && g_MainCharInfo.m_dwBuyLimit)
	{
		g_MainCharInfo.m_pHoldItem->SetDrawFlag(false);

		g_pUIManager->ShowNotice(IDS_LIMIT_PTBUY, NOTICE_FRAME_OKCANCEL, NOTICE_FRAME_LIMIT_BUY, XiahInput::g_ptMouse.x - 270, XiahInput::g_ptMouse.y-90);
	}
	else
	{
		if( pHoldItem->m_bItemType == ITEMTYPE_POTION || pHoldItem->m_bItemType == ITEMTYPE_PORTAL || pHoldItem->m_bItemType == ITEMTYPE_GOLDKEY || (pHoldItem->m_bItemType == ITEMTYPE_EVENT && pHoldItem->m_bItemKind == 11)) //HO_0828_07 황금열쇠 추가
		{
			SendCS_EC_BUYITEM_REQ(	g_MainCharInfo.m_dwPickedObject, 
									pHoldItem->m_wRefID,
									Amount,
									g_MainCharInfo.m_bSackCnt,
									pHoldItem->m_bSackPos,
									g_MainCharInfo.m_byMySackCurrIdx+1,
									255);
		}

		else
		{
			SendCS_EC_BUYITEM_REQ(	g_MainCharInfo.m_dwPickedObject, 
									pHoldItem->m_wRefID, 
									1,
									g_MainCharInfo.m_bSackCnt,
									pHoldItem->m_bSackPos, 
									g_MainCharInfo.m_byMySackCurrIdx+1,
									255);
		}

		g_MainCharInfo.m_dwVolumeSplitAmount = 0;
	}	

	return TRUE;
}

BOOL CSack::ProcessItemUnSelected_Shop_Shop( BYTE byPosition, XiahItem::sItemInfo* pHoldItem)
{
	g_MainCharInfo.m_pHoldItem->SetItemBackToSack();

	return TRUE;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
BOOL CSack::ProcessItemUnSelected_TradeMine_Default( BYTE byPosition, XiahItem::sItemInfo* pHoldItem)
////////////////////////////////////////////////////////////////////////////////////////////////////
{
	SendCS_EC_TRADESACKOFFITEM_REQ( pHoldItem->m_bSackCount+1,//pHoldItem->m_bSackID,
									pHoldItem->m_bSackPos,
									SACKTYPE__DEFAULT,
									byPosition,
									pHoldItem->m_dwItemID, 
									pHoldItem->m_dwAmount);

	return TRUE;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
BOOL CSack::ProcessItemUnSelected_Deposit_Default( BYTE byPosition, XiahItem::sItemInfo* pHoldItem)
////////////////////////////////////////////////////////////////////////////////////////////////////
{

	if(g_MainCharInfo.m_pDepositSack->m_bAction != OPEN_NORMAL)
	{
		g_MainCharInfo.ShowHelpMessage(IDS_ERROR1_JANGBOO, TEXTEFFECT_COLOR_WARNING);
		g_MainCharInfo.m_pHoldItem->SetItemBackToSack();
		g_MainCharInfo.PlayInterfaceSound( ISOUND_WARNING);
		return FALSE;
	}

	SendCS_EC_DRAWOUTBANK_REQ( g_MainCharInfo.m_dwObjectID,
								pHoldItem->m_dwItemID,
								pHoldItem->m_bSackPos,
								g_MainCharInfo.m_byMySackCurrIdx+1,								
								byPosition,
								pHoldItem->m_dwAmount);

	return TRUE;
}


// ITEM MALL -> DEFAULT SACK
////////////////////////////////////////////////////////////////////////////////////////////////////
BOOL CSack::ProcessItemUnSelected_Itemmall_To_Default( BYTE byPosition, XiahItem::sItemInfo* pHoldItem)
////////////////////////////////////////////////////////////////////////////////////////////////////
{
	SendCS_EC_DRAWOUTMALL_REQ( g_MainCharInfo.m_dwObjectID,
		pHoldItem->m_dwItemID,
		pHoldItem->m_bSackPos,
		g_MainCharInfo.m_byMySackCurrIdx+1,								
		byPosition,
		pHoldItem->m_dwAmount);

	return TRUE;
}


// ITEMMALL -> ITEMMALL 이동
////////////////////////////////////////////////////////////////////////////////////////////////////
BOOL CSack::ProcessItemUnSelected_Itemmall_To_Itemmall( BYTE byPosition, XiahItem::sItemInfo* pHoldItem)
////////////////////////////////////////////////////////////////////////////////////////////////////
{
	DWORD dwDesItemID = 0;

	XiahItem::sItemInfo* pDesItem = FindSackItemByPos( byPosition);
	if( pDesItem)
		dwDesItemID = pDesItem->m_dwItemID;

	SendCS_EC_DRAWMOVEMALL_REQ( pHoldItem->m_bSackPos,
		pHoldItem->m_dwItemID,
		byPosition,
		dwDesItemID);

	return TRUE;
}



////////////////////////////////////////////////////////////////////////////////////////////////////
BOOL CSack::ProcessItemUnSelected_Deposit_Deposit( BYTE byPosition, XiahItem::sItemInfo* pHoldItem)
////////////////////////////////////////////////////////////////////////////////////////////////////
{
	//g_MainCharInfo.m_pHoldItem->SetItemBackToSack();
	DWORD dwDesItemID = 0;

	XiahItem::sItemInfo* pDesItem = FindSackItemByPos( byPosition);
	if( pDesItem)
		dwDesItemID = pDesItem->m_dwItemID;

	SendCS_EC_DRAWMOVEBANK_REQ( pHoldItem->m_bSackPos,
								pHoldItem->m_dwItemID,
								byPosition,
								dwDesItemID);

	return TRUE;
}

/**
 * 행낭 -> 조합 창
 * \param byPosition 
 * \param pHoldItem 
 * \return 
 */
bool CSack::ProcessItemUnSelected_Default_Smelt(BYTE byPosition, XiahItem::sItemInfo* pHoldItem)
{	
	XiahItem::sItemInfo* pItem = g_MainCharInfo.m_pSmeltSack->FindSackItemByPos(byPosition);

	// 해당 위치에 아이템이 있을시 돌려주기
	if(pItem)
	{	
		g_MainCharInfo.m_pHoldItem->SetItemBackToSack();

		return false;
	}

	// 모서리 검사
	if(5 == byPosition || 11 == byPosition || 17 == byPosition || (18 <= byPosition && 23 >= byPosition))
	{
		if(2 == pHoldItem->m_bSackSizeX || 2 == pHoldItem->m_bSackSizeY)
		{
			g_MainCharInfo.m_pHoldItem->SetItemBackToSack();

			return false;
		}
	}

	// 1 x 1 제외한 아이템 주위 아이템과 겹치는지 검사
	if(1 != pHoldItem->m_bSackSizeX || 1 != pHoldItem->m_bSackSizeY)
	{
		// 옆
		if(5 != byPosition || 11 != byPosition || 17 != byPosition || 23 != byPosition)
		{
			pItem = g_MainCharInfo.m_pSmeltSack->FindSackItemByPos(byPosition + 1);

			if(pItem)
			{
				g_MainCharInfo.m_pHoldItem->SetItemBackToSack();

				return false;
			}
		}

		// 아래
		pItem = g_MainCharInfo.m_pSmeltSack->FindSackItemByPos(byPosition + 6);

		if(pItem)
		{
			g_MainCharInfo.m_pHoldItem->SetItemBackToSack();

			return false;
		}

		// 대각선 아래
		pItem = g_MainCharInfo.m_pSmeltSack->FindSackItemByPos(byPosition + 7);

		if(pItem)
		{
			g_MainCharInfo.m_pHoldItem->SetItemBackToSack();

			return false;
		}
	}

	// 사신셋 조합을 위해서 삭제
	//// 변종 아이템 검사 (2개 이상 검사) 
	//int nCount =0;
	//for(int i=0; i < 24; ++i)
	//{
	//	pItem = g_MainCharInfo.m_pSmeltSack->FindSackItemByPos(i);

	//	if(pItem)
	//	{
	//		// 흑정,소정,혈정, 흑보, 소보, 혈보, 흑편, 소편, 혈편
	//		if(pItem->m_bItemType == ITEMTYPE_REBUILDRES && (pItem->m_bItemKind >= 1 && pItem->m_bItemKind <= 3))
	//		{
	//			++nCount;

	//			if(nCount >= 1 && pHoldItem->m_bItemType == ITEMTYPE_REBUILDRES && (pHoldItem->m_bItemKind >= 1 && pHoldItem->m_bItemKind <= 3))
	//			{
	//				g_MainCharInfo.ShowHelpMessage(IDS_MIXTURE_OVERFULLRES, TEXTEFFECT_COLOR_WARNING);

	//				g_MainCharInfo.m_pHoldItem->SetItemBackToSack();

	//				return true;
	//			}
	//		}
	//	}
	//}

	//// 이미 변종 아이템 올렸는데... 다른 종류 올릴시
	//if(nCount)
	//{
	//	g_MainCharInfo.ShowHelpMessage(IDS_MIXTURE_BADITEM, TEXTEFFECT_COLOR_WARNING);					

	//	g_MainCharInfo.m_pHoldItem->SetItemBackToSack();

	//	return true;
	//}


	// 홀드아이템 넣어주기
	pHoldItem->m_bSackID	  = SACKTYPE__SMELT;
	pHoldItem->m_bSackIDPrev  = pHoldItem->m_bSackCount;
	pHoldItem->m_bSackPosPrev = pHoldItem->m_bSackPos;

	if(g_MainCharInfo.m_pSmeltSack->InsertItem(byPosition, pHoldItem))
	{
		g_MainCharInfo.m_pHoldItem->EmptyHoldItemItem();
	}
	else	// 이상한 위치일시
	{
		pHoldItem->m_bSackID = SACKTYPE__DEFAULT;
		g_MainCharInfo.m_pHoldItem->SetItemBackToSack();
	}	

	return true;
}


/**
 * 행낭->제련   오행 제련
 * \param byPosition 
 * \param pHoldItem 
 * \return 
 */
bool CSack::ProcessItemUnSelected_Default_FE_Convert(BYTE byPosition, XiahItem::sItemInfo* pHoldItem)
{
	// 개조 하고자 하는 아이템 체크
	if(byPosition == 0)
	{
		switch(pHoldItem->m_bItemType)
		{		
			case ITEMTYPE_WEAPON:
			case ITEMTYPE_CLOTH:
			case ITEMTYPE_HAT:
			case ITEMTYPE_SHOE:
				{
					if(!pHoldItem->m_bSocketCount)
					{
						if(!pHoldItem->m_bSocketItem[3])
						{
							g_MainCharInfo.ShowHelpMessage(IDS_FE_CONVERT_DISABLE, TEXTEFFECT_COLOR_WARNING);
							g_MainCharInfo.m_pHoldItem->SetItemBackToSack();

							return true;
						}
					}

					XiahItem::sItemInfo* pItem = g_MainCharInfo.m_pFEConvert->FindSackItemByPos( 0);

					if(pItem)
					{
						g_MainCharInfo.m_pHoldItem->SetItemBackToSack();
						return true;
					}
					/*if(!pHoldItem->m_wRBSocketItem)
					{	
						g_MainCharInfo.ShowHelpMessage(IDS_FE_CONVERT_DISABLE, TEXTEFFECT_COLOR_WARNING);
						g_MainCharInfo.m_pHoldItem->SetItemBackToSack();

						return true;
					}*/
				}
				break;
			default:
				{					
					g_MainCharInfo.ShowHelpMessage(IDS_FE_CONVERT_DISABLE, TEXTEFFECT_COLOR_WARNING);
					g_MainCharInfo.m_pHoldItem->SetItemBackToSack();

					return true;
				}
				break;
		}

		SendCS_IM_REBUILDITEMTERM_REQ(pHoldItem->m_dwItemID, pHoldItem->m_bSackCount+1, pHoldItem->m_bSackPos);
	}
	// 개조재료 체크
	else
	{		
		if(!g_MainCharInfo.m_pFEConvert)
			return true;

		// 제련 아이템 횅땍
		XiahItem::sItemInfo* pOriginalItem = g_MainCharInfo.m_pFEConvert->FindSackItemByPos(0);

		if(pOriginalItem)
		{
			DWORD dwResourceIDConstraint =0;

			int nCount = 1;
			// 자기보다 순위가 낮은 창에 뭔가가 없으면 리턴
			for(int i=1; i < byPosition; ++i)
			{
				XiahItem::sItemInfo* pItem = g_MainCharInfo.m_pFEConvert->FindSackItemByPos(i);

				if(!pItem)
				{
					g_MainCharInfo.ShowHelpMessage(IDS_UPPER_NONE, TEXTEFFECT_COLOR_WARNING);

					g_MainCharInfo.m_pHoldItem->SetItemBackToSack();
					return true;
				}
				// 뭔가가 있으면 그 넘의 아이디만 다른 창에 들어갈 수 있다
				else
				{
					dwResourceIDConstraint = pItem->m_wRefID;
					++nCount;
				}
			}

			if(!dwResourceIDConstraint)
			{
				// [10/26/2004] 개조 가능여부
				if( pHoldItem->m_bItemType == ITEMTYPE_REBUILDRES && 0 == pHoldItem->m_bIsDividedRes
					&& pHoldItem->m_bItemKind >= 5 &&  pHoldItem->m_bItemKind <= 6)
				{
					// 개조재료 넣을창에 뭔가가 있으면 원래 위치로 돌려주고
					XiahItem::sItemInfo* pItem = g_MainCharInfo.m_pFEConvert->FindSackItemByPos(byPosition);

					if(pItem)
					{
						pItem->m_bSackID = SACKTYPE__DEFAULT;
						g_MainCharInfo.m_pMySack[pItem->m_bSackIDPrev]->InsertItem(pItem->m_bSackPosPrev, pItem);
						g_MainCharInfo.m_pFEConvert->DeleteItem(byPosition);
					}

					// 홀드아이템 넣어주기
					pHoldItem->m_bSackID		= SACKTYPE__MODIFY;
					pHoldItem->m_bSackIDPrev	= pHoldItem->m_bSackCount;
					pHoldItem->m_bSackPosPrev	= pHoldItem->m_bSackPos;

					g_MainCharInfo.m_pFEConvert->InsertItem(byPosition, pHoldItem);
					g_MainCharInfo.m_pHoldItem->EmptyHoldItemItem();

					if(pHoldItem->m_bItemKind != 6)
					{
						TCHAR strMoney[128] = {0,};
						INT64 n64Money = pOriginalItem->m_dwPrice * 0.4 * nCount;
						_stprintf(strMoney, IDS_MONEY, MoneyCommaStr(n64Money).data());
						g_pUIManager->SetString(WINDOW_FIVEELEMENTS_CONVERT, fiveelements_convert_window_dumy_05, strMoney);
					}					
				}
				//HT_1116 : 각성자 아이템 추가
				else if(pHoldItem->m_bItemType == ITEMTYPE_REBUILDRES && (pHoldItem->m_bItemKind == 17 || pHoldItem->m_bItemKind == 16))//&& 0 == pHoldItem->m_bIsDividedRes && pHoldItem->m_bItemKind == 17)
				{
					// 개조재료 넣을창에 뭔가가 있으면 원래 위치로 돌려주고
					XiahItem::sItemInfo* pItem = g_MainCharInfo.m_pFEConvert->FindSackItemByPos(byPosition);

					if(pItem)
					{
						pItem->m_bSackID = SACKTYPE__DEFAULT;
						g_MainCharInfo.m_pMySack[pItem->m_bSackIDPrev]->InsertItem(pItem->m_bSackPosPrev, pItem);
						g_MainCharInfo.m_pFEConvert->DeleteItem(byPosition);
					}

					// 홀드아이템 넣어주기
					pHoldItem->m_bSackID		= SACKTYPE__MODIFY;
					pHoldItem->m_bSackIDPrev	= pHoldItem->m_bSackCount;
					pHoldItem->m_bSackPosPrev	= pHoldItem->m_bSackPos;

					g_MainCharInfo.m_pFEConvert->InsertItem(byPosition, pHoldItem);
					g_MainCharInfo.m_pHoldItem->EmptyHoldItemItem();

					if(pHoldItem->m_bItemKind != 6 && pHoldItem->m_bItemKind != 16)
					{
						TCHAR strMoney[128] = {0,};
						INT64 n64Money = 0;
						
						//HT_1116 : 각성자 아이템 추가
						if(pHoldItem->m_bItemType == 25 && pHoldItem->m_bItemKind == 17)
							n64Money = 2000000;
						else
							n64Money = pOriginalItem->m_dwPrice * 0.4 * nCount;
						
						_stprintf(strMoney, IDS_MONEY, MoneyCommaStr(n64Money).data());
						g_pUIManager->SetString(WINDOW_FIVEELEMENTS_CONVERT, fiveelements_convert_window_dumy_05, strMoney);
					}			
				}
				else
				{
					g_MainCharInfo.ShowHelpMessage(IDS_FE_NOT_TYPE_FOR_CONVERT, TEXTEFFECT_COLOR_WARNING);

					g_MainCharInfo.m_pHoldItem->SetItemBackToSack();
				}
			}
			else
			{
				if(pHoldItem->m_wRefID == dwResourceIDConstraint)
				{
					// 개조재료 넣을창에 뭔가가 있으면 원래 위치로 돌려주고
					XiahItem::sItemInfo* pItem = g_MainCharInfo.m_pFEConvert->FindSackItemByPos(byPosition);
					if(pItem)
					{
						pItem->m_bSackID = SACKTYPE__DEFAULT;
						g_MainCharInfo.m_pMySack[pItem->m_bSackIDPrev]->InsertItem(pItem->m_bSackPosPrev, pItem);
						g_MainCharInfo.m_pFEConvert->DeleteItem(byPosition);
					}

					// 홀드아이템 넣어주기
					pHoldItem->m_bSackID		= SACKTYPE__MODIFY;
					pHoldItem->m_bSackIDPrev	= pHoldItem->m_bSackCount;
					pHoldItem->m_bSackPosPrev	= pHoldItem->m_bSackPos;

					g_MainCharInfo.m_pFEConvert->InsertItem(byPosition, pHoldItem);
					g_MainCharInfo.m_pHoldItem->EmptyHoldItemItem();

					if(pHoldItem->m_bItemKind != 6)
					{
						TCHAR strMoney[128] = {0,};
						INT64 n64Money = pOriginalItem->m_dwPrice * 0.4 * nCount;
						_stprintf(strMoney, IDS_MONEY, MoneyCommaStr(n64Money).data());
						g_pUIManager->SetString(WINDOW_FIVEELEMENTS_CONVERT, fiveelements_convert_window_dumy_05, strMoney);
					}					
				}
				else
				{
					g_MainCharInfo.ShowHelpMessage(IDS_FE_SAME_SOURCE_ABLE, TEXTEFFECT_COLOR_WARNING);

					g_MainCharInfo.m_pHoldItem->SetItemBackToSack();
				}
			}
		}
		else
		{
			g_MainCharInfo.ShowHelpMessage(IDS_FE_PUT_CONVERTITEM, TEXTEFFECT_COLOR_WARNING);
			g_MainCharInfo.m_pHoldItem->SetItemBackToSack();
		}
	}

	return true;
}

/**
 * 매품패 ( 행낭 -> 매품패)
 * \param byPosition 
 * \param pHoldItem 
 * \return 
 */
bool CSack::ProcessItemUnSelected_Default_QuickMart(BYTE byPosition, XiahItem::sItemInfo* pHoldItem)
{
	if((pHoldItem->m_bRarity >= g_MainCharInfo.m_bRarityLimit && g_MainCharInfo.m_bRarityLimit)
		|| (pHoldItem->m_bStxType >= g_MainCharInfo.m_bStxTypeLimit && g_MainCharInfo.m_bStxTypeLimit))
	{
		g_MainCharInfo.m_pHoldItem->SetDrawFlag(false);

		g_MainCharInfo.m_dwPickedObject = 0;
		g_pUIManager->ShowNotice(IDS_LIMIT_SELL, NOTICE_FRAME_OKCANCEL, NOTICE_FRAME_LIMIT_SELL, XiahInput::g_ptMouse.x - 270, XiahInput::g_ptMouse.y-90);
	}
	else
	{
		SendCS_EC_SELLITEM_REQ( 0, pHoldItem->m_dwItemID,
									pHoldItem->m_bSackCount+1,
									pHoldItem->m_bSackPos);
	}

	return true;
}

/**
* 행낭 -> 아이템 수집
* \param byPosition 
* \param pHoldItem 
* \return 
*/
bool CSack::ProcessItemUnSelected_Default_Collection(BYTE byPosition, XiahItem::sItemInfo* pHoldItem)
{
	SendCS_IM_MOVEINCOLLECTITEM_REQ(pHoldItem->m_bSackCount+1, pHoldItem->m_bSackPos, pHoldItem->m_dwItemID,
									byPosition);

	return true;
}

/**
* 아이템 수집 -> 행낭
* \param byPosition 
* \param pHoldItem 
* \return 
*/
bool CSack::ProcessItemUnSelected_Collection_Default(BYTE byPosition, XiahItem::sItemInfo* pHoldItem)
{
	SendCS_IM_MOVEOUTCOLLECTITEM_REQ(pHoldItem->m_bSackPos, pHoldItem->m_dwItemID,
									g_MainCharInfo.m_byMySackCurrIdx+1, byPosition);

	return true;
}
