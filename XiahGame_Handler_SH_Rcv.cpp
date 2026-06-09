
////////////////////////////////////////////////////////////////////////////////////////
// YS_0304 : SHOP - 개인상점 구현을 위해서 추가되는 프로토콜
// OFFSET_CS_SH
////////////////////////////////////////////////////////////////////////////////////////
int OnCS_SH_SHOPINFO_ACK(CMsg &msg)
{	
	BYTE bResult		=0;
	BYTE bKind			=0;			
	BYTE bItemCnt		=0;
	BYTE bSackPos		=0;
	WORD wRemainShop	=0;
	DWORD dwCharID		=0;
	DWORD dwShopMoney	=0;
	DWORD dwItemMoney	=0;
	sString strName;
	sString strDescription;

	msg
		>> bResult
		>> dwCharID
		>> bKind
		>> strName
		>> strDescription
		>> dwShopMoney
		>> wRemainShop
		>> bItemCnt;

	switch(bResult)
	{
	case SHOPINFO_SUCCESS:
		{
			if(g_MainCharInfo.m_pQuickMart)
			{
				g_MainCharInfo.ShowHelpMessage(IDS_QUICKMART_OPEN, TEXTEFFECT_COLOR_WARNING);

				return false;
			}

			CloseAllWindow();
			SAFE_DELETE(g_MainCharInfo.m_pPersonalTradeSet);  // 열린 상태에서 다시 활성화시 대비

			g_pUIManager->SetPosition(WINDOW_PC_STORE, WINDOW_SECOND_XPOS, 0);

			g_MainCharInfo.ShowSack( SACKTYPE__DEFAULT);
			g_MainCharInfo.ShowSack( SACKTYPE__EQUIPMENT);
			g_MainCharInfo.ShowSack( SACKTYPE__PERSONAL_TRADE_SET);

			g_MainCharInfo.m_dwTradeMoney = dwShopMoney;

			g_pUIManager->SetString(WINDOW_PC_STORE, pc_store_passage_edit_01, strName);
			g_pUIManager->SetString(WINDOW_PC_STORE, pc_store_passage_edit_02, strDescription);
			g_pUIManager->SetString(WINDOW_PC_STORE, pc_store_earnings_dummy_03, wRemainShop);
			g_pUIManager->SetString(WINDOW_PC_STORE, pc_store_earnings_dummy_04, MoneyCommaStr(dwShopMoney));
		}
		break;
	case SHOPINFO_INTERNALERROR:
		return false;
		break;
	case SHOPINFO_LOADFAIL:		// 상점정보 읽기 실패		
		return false;
		break;
	case 3:						// 악인이므로 개인노점 개설 불가
		{
			g_MainCharInfo.ShowHelpMessage(IDS_NOTBADFAMESHOPRUN, TEXTEFFECT_COLOR_WARNING);
			return false;
		}
		break;
	default:
		break;
	}

	if(g_MainCharInfo.m_pPersonalTradeSet)
	{
		for(int i=0; i < bItemCnt; i++)
		{
			XiahItem::sItemInfo* pItem = new XiahItem::sItemInfo;

			msg
				>> bSackPos;

			XiahItem::GetItemData( pItem, msg);

			msg
				>> dwItemMoney;

			//HT_1116 : 각성자 아이템 추가
			msg
				>> pItem->m_wRebuithValue;

			pItem->m_dwPrice = dwItemMoney;  // 판매금액
			pItem->m_bSackID = SACKTYPE__PERSONAL_TRADE_SET;

			g_MainCharInfo.m_pPersonalTradeSet->InsertItem(bSackPos, pItem);
		}
	} // if(g_MainCharInfo.m_pPersonalTradeSet)

	return true;
}
//..UNITSVR->CLIENT
//bResult
//dwCharID
//bKind
//strName
//StrDescription
//dwShopMoney
//wRemainShop						//남은 상점용 아이템 내구력 
//bItemCnt
//bSackPos
//GetItemData()
//dwItemMoney


int OnCS_SH_SETSHOP_ACK(CMsg &msg)
{
	BYTE bResult	=0;

	msg
		>> bResult;

	switch(bResult)
	{
	case SETSHOP_SUCCESS:
		g_MainCharInfo.ShowHelpMessage(IDS_SETSHOP_SUCCESS);
		break;
	case SETSHOP_INTERNALERROR:
		break;
	case SETSHOP_SIZEOVER:
		g_MainCharInfo.ShowHelpMessage(IDS_SETSHOP_SIZEOVER);
		break;
	default:
		break;
	}

	return true;
}
//..UNITSVR->CLIENT
//bResult



int OnCS_SH_MOVESHOP_ACK(CMsg &msg)
{
	BYTE bResult	=0;

	msg
		>> bResult;

	switch(bResult)
	{
	case MOVESHOP_SUCCESS:	// 상점내 아이템 이동		
		break;
	case MOVESHOP_NOTFINDITEM:	// 아이템을 찾을수 없음
		{			
			g_MainCharInfo.ShowHelpMessage(IDS_ITEM_NOTFIND);

			if( g_MainCharInfo.m_pHoldItem->IsHoldingItemItem())
				g_MainCharInfo.m_pHoldItem->SetItemBackToSack();		
		}		
		break;
	case MOVESHOP_NOTEMPTY:	// 없는 아이템		
		{
			g_MainCharInfo.ShowHelpMessage(IDS_ITEM_NOTFIND);

			if( g_MainCharInfo.m_pHoldItem->IsHoldingItemItem())
				g_MainCharInfo.m_pHoldItem->SetItemBackToSack();
		}		
		break;
	case MOVESHOP_INTERNALERROR:
		break;
	default:
		break;
	}

	return true;
}
//..UNITSVR->CLIENT
//bResult


// 행낭 -> 상점
int OnCS_SH_REGSHOP_ACK(CMsg &msg)
{
	BYTE bResult	=0;
	DWORD dwMoney	=0;

	msg
		>> bResult
		>> dwMoney;

	switch(bResult)
	{
	case REGSHOP_SUCCESS:
		{
			TCHAR strTemp[50]= {0,};
			_stprintf( strTemp, IDS_REGSHOP_SUCCESS, dwMoney);
			g_MainCharInfo.ShowHelpMessage(strTemp);
		}
		break;
	case REGSHOP_INTERNALERROR:
		break;
	case REGSHOP_NOTFINDSACK:
		g_MainCharInfo.ShowHelpMessage(IM_N_ITEM);
		break;
	case REGSHOP_FULLSACK:
		{
			g_MainCharInfo.ShowHelpMessage(IDS_REGSHOP_FULLSACK);

			if( g_MainCharInfo.m_pHoldItem->IsHoldingItemItem())
				g_MainCharInfo.m_pHoldItem->SetItemBackToSack();
		}		
		break;
	case REGSHOP_DONOTSELLITEM:
		{
			g_MainCharInfo.ShowHelpMessage(IDS_REGSHOP_DONOTSELLITEM, 2);

			if(g_MainCharInfo.m_pHoldItem->IsHoldingItemItem())
                g_MainCharInfo.m_pHoldItem->SetItemBackToSack();
		}
		break;
	default:
	    break;
	}	

	return true;
}
//..UNITSVR->CLIENT
//bResult,  -- 0 : SUCCESS -- 1 : 실패  
//dwMoney


// 설정 상점창 -> 행낭
int OnCS_SH_DELSHOP_ACK(CMsg &msg)
{
	BYTE bResult	=0;

	msg
		>> bResult;

	switch(bResult)
	{
	case DELSHOP_SUCCESS:
		{
			g_MainCharInfo.ShowHelpMessage(IDS_DELSHOP_SUCCESS);
			// 아이템이 행낭으로 이동됨
		}
		break;
	case DELSHOP_INTERNALERROR:
		break;
	case DELSHOP_NOTFINDITEM:	// 아이템을 찾을수 없음
		g_MainCharInfo.ShowHelpMessage(IM_N_ITEM);
		
		break;
	case DELSHOP_FULLSACK:	// 행낭창이 가득
		{
			g_MainCharInfo.ShowHelpMessage(IDS_NO_MOVEABLE_SPACE);
			
			if( g_MainCharInfo.m_pHoldItem->IsHoldingItemItem())
				g_MainCharInfo.m_pHoldItem->SetItemBackToSack();
		}		
		break;
	case DELSHOP_NOTEMPTYSACK:	// 빈 행낭
		{
			g_MainCharInfo.ShowHelpMessage(IDS_NO_MOVEABLE_SPACE);

			if( g_MainCharInfo.m_pHoldItem->IsHoldingItemItem())
				g_MainCharInfo.m_pHoldItem->SetItemBackToSack();
		}		
		break;
	case DELSHOP_NOTDELSHOP:
		g_MainCharInfo.ShowHelpMessage(IM_N_ITEM);
		// 이동 불가능
		break;
	default:
		break;
	}

	return true;
}
//..UNITSVR->CLIENT
//bResult



int OnCS_SH_STATUSCHANGE_ACK(CMsg &msg)
{
	BYTE bResult	=0;
	BYTE bStatus	=0;

	msg
		>> bResult
		>> bStatus;

	switch(bResult)
	{
	case STATUSCHANGE_SUCCESS:
		{
			// 판매 시작
			// 판매 중지

			switch(bStatus)
			{
			case 0:
				{
					g_MainCharInfo.m_bPersonalTradeSell = false;

					g_pUIManager->SetString(WINDOW_PC_STORE, pc_store_button_02, IDS_PT_SET_START);
					g_MainCharInfo.ShowHelpMessage(IDS_PT_END);
				}
				break;
			case 1:
				{
					g_MainCharInfo.m_bPersonalTradeSell = true;

					g_pUIManager->SetString(WINDOW_PC_STORE, pc_store_button_02, IDS_PT_SET_END);
					g_MainCharInfo.ShowHelpMessage(IDS_PT_START);
				}
				break;
			case 2:
				{
					if( g_MainCharInfo.m_pHoldItem->IsHoldingItemItem())
						g_MainCharInfo.m_pHoldItem->SetItemBackToSack();

					g_MainCharInfo.CloseFrame( WINDOW_MONEY);

					g_MainCharInfo.m_bPersonalTradeSell = false;
					g_MainCharInfo.HideSack(SACKTYPE__PERSONAL_TRADE_SET);  // 설정창 닫고

					g_pUIManager->SetString(WINDOW_PC_STORE, pc_store_button_02, IDS_PT_SET_START);
					g_MainCharInfo.ShowHelpMessage(IDS_USABLEEND);
				}
				break;
			default:
				break;
			}

		}
		break;
	case STATUSCHANGE_INTERNALERROR:		
		break;
	case STATUSCHANGE_PREVSAME:
		break;
	case STATUSCHANGE_NOTFINDSHOPRUNITEM:
		{
			g_MainCharInfo.m_bPersonalTradeSell = false;
			g_MainCharInfo.ShowHelpMessage(IDS_NOTFINDSHOPRUNITEM, TEXTEFFECT_COLOR_WARNING);
		}		
		break;
	case STATUSCHANGE_NOTEMPTYSACK:	// 행낭에 아이템이 없음		
		break;
	case STATUSCHANGE_NOTDELSHOP:
		break;
	case 6:							// 악인이므로 개인노점 개설 할수 없음
		{
			g_MainCharInfo.ShowHelpMessage(IDS_NOTBADFAMESHOPRUN, TEXTEFFECT_COLOR_WARNING);
		}
		break;		
	default:
		break;
	}

	return true;
}
//..UNITSVR->CLIENT
//bResult	-- 0 : 성공      , -- 1 : 개점용 아이템 존재하지 않음.
//bStatus	-- 상점 상태 



int OnCS_SH_GETMONEY_ACK(CMsg &msg)
{
	BYTE bResult		=0;
	DWORD dwMoney		=0;
	DWORD dwFeeMoney	=0;
	DWORD dwMunpaTaxMoney =0;
	sString strMunpaName;

	msg
		>> bResult
		>> dwMoney
		>> dwFeeMoney
		>> dwMunpaTaxMoney
		>> strMunpaName;

	switch(bResult)
	{
	case GETMONEY_SUCCESS:
		{
			// 돈 회수 메시지
			TCHAR strTemp[128]= {0,};

			_stprintf(strTemp, IDS_SHOP_GETMONEY, dwFeeMoney, dwMoney);
			g_MainCharInfo.ShowHelpMessage(strTemp);
			
			_stprintf(strTemp, IDS_SHOP_GETMONEY_2, strMunpaName.data(), MoneyCommaStr(dwMunpaTaxMoney).data());
			g_MainCharInfo.ShowHelpMessage(strTemp);
		}
		break;
	case GETMONEY_BADSHOPID:
		g_MainCharInfo.ShowHelpMessage(IDS_INTERNAL_ERROR);
		break;
	case GETMONEY_BADMONEY:
		// 
		break;
	case GETMONEY_INTERNALERROR:
		break;
	case GETMONEY_OVERMONEY:	// 소지금액 초과
		{
			g_MainCharInfo.ShowHelpMessage(IDS_PETTRADE_OVERMONEY, TEXTEFFECT_COLOR_WARNING);			
			g_MainCharInfo.PlayInterfaceSound(ISOUND_WARNING);
		}
		break;		
	default:
		break;
	}

	return true;
}
//..UNITSVR->CLIENT
//bResult -- 0 : 성공, 그 외 실패 
//dwMoney - 회수되는 돈 



int OnCS_SH_GETSHOPINFO_ACK(CMsg &msg)
{	
	BYTE bResult =0;

	msg
		>> bResult;
		
	switch(bResult)
	{
	case GETSHOPINFO_SUCCESS:
		{
			CloseAllWindow();
			SAFE_DELETE(g_MainCharInfo.m_pPersonalTradeSell);

			g_MainCharInfo.ShowSack( SACKTYPE__DEFAULT);
			g_MainCharInfo.ShowSack( SACKTYPE__EQUIPMENT);
			g_MainCharInfo.ShowSack( SACKTYPE__PERSONAL_TRADE_SELL);

			BYTE bItemCnt	=0;
			BYTE bSackPos	=0;	
			DWORD dwCharID	=0;
			DWORD dwPrice	=0;

			msg
				>> dwCharID
				>> bItemCnt;

			for(int i=0; i < bItemCnt; i++)
			{
				XiahItem::sItemInfo* pItem = new XiahItem::sItemInfo;

				msg
					>> bSackPos;

				XiahItem::GetItemData( pItem, msg);

				msg
					>> dwPrice;

				pItem->m_dwPrice = dwPrice;  // 판매금액
				pItem->m_bSackID = SACKTYPE__PERSONAL_TRADE_SELL;

				g_MainCharInfo.m_pPersonalTradeSell->InsertItem(bSackPos, pItem);
			} // for(int i=0; i < bItemCnt; i++)

			// 거래 시작
		}
		break;
	case GETSHOPINFO_NOTFIND:
		{
			g_MainCharInfo.ShowHelpMessage(IDS_INTERNAL_ERROR);
			return false;
		}
		break;
	default:
		break;
	}	
	
	return true;
}
//bResult
//dwCharID
//bItemCnt
//bSackPos
//GetItemData()
//dwItemMoney
//bResult - RETURN CODE



/**
 * 개인 상점 아이템 구입
 * \param &msg 
 * \return 
 */
int OnCS_SH_BUYPCSHOP_ACK(CMsg &msg)
{
	BYTE bResult	=0;

	msg
		>> bResult;

	switch(bResult)
	{
	case BUYPCSHOP_SUCCESS:
		{
			g_MainCharInfo.ShowHelpMessage(IDS_BUY_SHOP);

			// 구입 했음
			if( g_MainCharInfo.m_pHoldItem->IsHoldingItemItem())
			{
				g_MainCharInfo.m_pHoldItem->DeleteHoldItemItem();
			}
		}
		break;
	case BUYPCSHOP_NOTFINDCHAR:
		{
			g_MainCharInfo.ShowHelpMessage(IDS_PCSHOP_NOTFINDCHAR);
			// 개인 상점 캐릭터를 찾지 못함

			// 상점이 없으므로 상점 제거
			if( g_MainCharInfo.m_pHoldItem->IsHoldingItemItem())
				g_MainCharInfo.m_pHoldItem->DeleteHoldItemItem();
			g_MainCharInfo.HideSack(SACKTYPE__PERSONAL_TRADE_SELL);
		}
		break;
	case BUYPCSHOP_NOTFINDITEM:
		{
			g_MainCharInfo.ShowHelpMessage(IDS_PCSHOP_NOTFINDITEM);
			// 아이템이 없음

			if( g_MainCharInfo.m_pHoldItem->IsHoldingItemItem())
				g_MainCharInfo.m_pHoldItem->DeleteHoldItemItem();
		}
		break;
	case BUYPCSHOP_FULLSACK:
		{
			g_MainCharInfo.ShowHelpMessage(IDS_PCSHOP_FULLSACK);
			// 행낭이 가득참

			if( g_MainCharInfo.m_pHoldItem->IsHoldingItemItem())
				g_MainCharInfo.m_pHoldItem->SetItemBackToSack();
		}
		break;
	case BUYPCSHOP_NOTEMPTYSACK:
		{
			// 			
			g_MainCharInfo.ShowHelpMessage(IDS_NO_MOVEABLE_SPACE);

			if( g_MainCharInfo.m_pHoldItem->IsHoldingItemItem())
				g_MainCharInfo.m_pHoldItem->SetItemBackToSack();
		}
		break;
	case BUYPCSHOP_NOTMONEY:
		{
			g_MainCharInfo.ShowHelpMessage(IDS_SHORT_MONEY);
			// 돈 부족

			if( g_MainCharInfo.m_pHoldItem->IsHoldingItemItem())
				g_MainCharInfo.m_pHoldItem->SetItemBackToSack();
		}
		break;
	case BUYPCSHOP_INTERNALERROR:
		{

		}
		break;
	case 7:
		{
			if( g_MainCharInfo.m_pHoldItem->IsHoldingItemItem())
				g_MainCharInfo.m_pHoldItem->SetItemBackToSack();
		}
		break;
	default:
		break;
	}

	return true;
}
//..UNITSVR->CLIENT
//bResult -- 0 : 성공, 그외 실패 




int OnCS_SH_ADDONSHOP_ACK(CMsg &msg)
{
	BYTE bAction	=0;
	BYTE bSackPos	=0;
	DWORD dwPrice	=0;	

	msg
		>> bAction
		>> bSackPos
		>> dwPrice;

	XiahItem::sItemInfo* pItem = new XiahItem::sItemInfo;

	XiahItem::GetItemData( pItem, msg);

	//HT_1116 : 각성자 아이템 추가
	msg
		>> pItem->m_wRebuithValue;

	if(g_MainCharInfo.m_pPersonalTradeSet)
	{
		if( bAction == 1 || bAction == 2)	// bAction == 2 면 내에서의 움직임
		{
			pItem->m_bSackID = SACKTYPE__PERSONAL_TRADE_SET;
			pItem->m_dwPrice = dwPrice;

			g_MainCharInfo.m_pPersonalTradeSet->InsertItem( bSackPos, pItem);
		} // if( bAction == 1 || bAction == 2)	// bAction == 2 면 내에서의 움직임
	}

	return true;
}
//..UNITSVR->Client
//bAction  - ADDONBANK와 동일 2
//bSackPos 
//dwPrice
//Item 정보..

int OnCS_SH_REMOVEFROMSHOP_ACK(CMsg &msg)
{
	BYTE bSackPos	=0;
	DWORD dwItemID	=0;
	DWORD dwAmount	=0;

	msg
		>> bSackPos
		>> dwItemID
		>> dwAmount;

	if( g_MainCharInfo.m_pHoldItem->IsHoldingItemItem())
	{
		g_MainCharInfo.m_pHoldItem->DeleteHoldItemItem();
	}
	else
	{
		// 누수 수정
		if(g_MainCharInfo.m_pPersonalTradeSet)
			g_MainCharInfo.m_pPersonalTradeSet->DeleteItem(bSackPos, true);
	}

	return true;
}
//..UNITSVR->CLIENT
//bSackPos
//dwItemID
//dwAmount

int OnCS_SH_SHOPINFOCHANGE_ACK(CMsg &msg)
{
	DWORD dwMoney		=0;
	WORD wRemainShop	=0;
	int nType			=0;
	
	msg
		>> dwMoney
		>> wRemainShop;

	g_MainCharInfo.m_dwTradeMoney = dwMoney;
	
	//if(dwMoney >= 100000 && dwMoney < 1000000)
	//{
	//	nType=9;
	//}
	//else if(dwMoney >= 1000000 && dwMoney < 10000000)
	//{
	//	nType=10;
	//}
	//else if(dwMoney >= 10000000 && dwMoney < 100000000)
	//{
	//	nType=11;
	//}
	//else if(dwMoney >= 100000000 && dwMoney < 1000000000)
	//{
	//	nType=12;
	//}
	//else if(dwMoney >= 1000000000 && dwMoney < 10000000000)
	//{
	//	nType=13;
	//}
	
	g_pUIManager->SetString(WINDOW_PC_STORE, pc_store_earnings_dummy_04, MoneyCommaStr(dwMoney), g_MainCharInfo.MoneyUnitColor(dwMoney));
	g_pUIManager->SetString(WINDOW_PC_STORE, pc_store_earnings_dummy_03, wRemainShop);

	return true;
}
//..UNITSVR->CLIENT
//dwMoney										//현재 SHOP에 저장되어 있는 금액 
//wRemainShop									//현재 SHOP을 위해 사용되는 아이템의 내구력 






//맵에 진행시 노점 상태인 PC가 존재 할 경우 
// CS_IT_CHARINFO_ACK
/*
// dwFame				-- 기존 
-- bShopStatus			-- 신규 
-- strShopName			-- 신규 
-- strShopDescription	-- 신규 
// dwMunpaID			-- 기존 
*/
