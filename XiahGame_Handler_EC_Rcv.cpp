extern sString MoneyCommaStr(INT64 nMoney);

/**
 * 아이템 구입
 * \param &msg 
 * \return 
 */
int OnCS_EC_BUYITEM_ACK( CMsg &msg)
{	
	BYTE bResult = 0;

	msg		
		>> bResult;

	switch(bResult)
	{
	case 0:
		{
			if( g_MainCharInfo.m_pHoldItem->IsHoldingItemItem())
			{
				TCHAR temp[100] = {0,};
				_stprintf( temp, IDS_D_BUY, (LPCTSTR)g_MainCharInfo.m_pHoldItem->GetHoldItemItem()->m_szName,g_MainCharInfo.m_pHoldItem->GetHoldItemItem()->m_dwBuyCount);
				g_MainCharInfo.ShowHelpMessage( temp,TEXTEFFECT_COLOR_GAIN);
				g_MainCharInfo.PlayInterfaceSound( ISOUND_ITEM_LAY_BUY);
			}
		}
		break;
	case 1:
		{
			g_MainCharInfo.ShowHelpMessage( IDS_ERROR_NO_ITEM, TEXTEFFECT_COLOR_WARNING);
		}
		break;
	case 2:
		{
			g_MainCharInfo.ShowHelpMessage( IDS_SHORT_MONEY, TEXTEFFECT_COLOR_WARNING);
		}
		break;
	case 3:
		{
			g_MainCharInfo.ShowHelpMessage( IDS_ERROR_BUYITEM,TEXTEFFECT_COLOR_WARNING);
		}
		break;
	case 4:
		{
			TCHAR str[50] = {0,};
			_stprintf( str, IDS_ERROR, 4);
			g_MainCharInfo.ShowHelpMessage(str,TEXTEFFECT_COLOR_WARNING);
		}
		break;
	case 6:		// 행낭이 가득
		{
			TCHAR strTemp[128] = {0,};
			_stprintf(strTemp, IDS_PC_FULLSACK, g_MainCharInfo.m_byMySackCurrIdx+1);			
			g_MainCharInfo.ShowHelpMessage(strTemp, TEXTEFFECT_COLOR_WARNING);			
		}
		break;
	default:
		break;
	}

	if( g_MainCharInfo.m_pHoldItem->IsHoldingItemItem())
		g_MainCharInfo.m_pHoldItem->EmptyHoldItemItem();

	g_MainCharInfo.m_bInteractionFlag = FALSE;

	return 0;
}


/**
 * 아이템 판매
 * \param &msg 
 * \return 
 */
int OnCS_EC_SELLITEM_ACK( CMsg &msg)
{
	//DWORD dwShopID	=0;
	BYTE bResult	=0;
	//BYTE bSackPos	=0;

	msg
		//>> dwShopID		// 미사용
		>> bResult;
		//>> bSackPos;	// 미사용  패킷 정리해야 겠다

	switch(bResult)
	{
	case 0: // 판매 완료
		{
			g_MainCharInfo.ShowHelpMessage(IDS_SELLITEM_COMPLETE);
			g_MainCharInfo.PlayInterfaceSound(ISOUND_ITEM_LAY_SELL);
		}
		break;
	case 1:	// 아이템 없음
		{
			g_MainCharInfo.ShowHelpMessage(IDS_ERROR_NO_ITEM, TEXTEFFECT_COLOR_WARNING);
		}
		break;
	case 6:	// 아이템 몰에서 구입한것을 팔려고 하였다.
		{			
			g_MainCharInfo.m_pHoldItem->SetItemBackToSack();

			g_MainCharInfo.ShowHelpMessage(IDS_ERROR_SELLMALLITEM, TEXTEFFECT_COLOR_WARNING);			
			g_MainCharInfo.PlayInterfaceSound( ISOUND_WARNING);
		}
		break;
	case 7:	// 소지금 초과
		{
			g_MainCharInfo.m_pHoldItem->SetItemBackToSack();

			g_MainCharInfo.ShowHelpMessage(IDS_PETTRADE_OVERMONEY, TEXTEFFECT_COLOR_WARNING);			
			g_MainCharInfo.PlayInterfaceSound(ISOUND_WARNING);			
		}
		break;
	default:
		{
			TCHAR str[32] = {0,};
			_stprintf( str, IDS_ERROR, bResult);
			g_MainCharInfo.ShowHelpMessage(str, TEXTEFFECT_COLOR_WARNING);
		}
		break;
	}

	//HT_CHEAT : 자동 팔기 
	if(g_MainCharInfo.m_bAutoSell)
	{
		g_MainCharInfo.m_bAutoSell = false;
		g_MainCharInfo.m_bySellPos = 0;
	}

	return 0;
}

int OnCS_EC_ASKTRADE_ACK( CMsg &msg)
{
	BYTE	bResult		=0;
	DWORD	dwAskID		=0;
	DWORD	dwAskedID	=0;

	msg
		>> bResult
		>> dwAskID
		>> dwAskedID;

	if(bResult == 0) 
	{
		g_MainCharInfo.m_dwAskID = dwAskID;

		TCHAR content[100];
		_stprintf( content, IDS_D_REQUEST_TRADE, (LPCTSTR)g_MainCharInfo.FindNameByID( dwAskID));
		
		if( !g_pUIManager->ShowNotice( content, NOTICE_FRAME_OKCANCEL, NOTICE_FRAME_TRADE))
		{
			//취소
			SendCS_EC_ASKTRADE_REQ(9, g_MainCharInfo.m_dwAskID, g_MainCharInfo.m_dwObjectID);
		}
		
	}	
	else
	{
		TCHAR content[100] = {0,};
		_stprintf( content, IDS_D_CANCEL_TRADE, (LPCTSTR)g_MainCharInfo.FindNameByID( g_MainCharInfo.m_dwAskID));
		g_MainCharInfo.ShowHelpMessage( content,TEXTEFFECT_COLOR_WARNING);

		g_pUIManager->DeletePopMenu();//음야

		g_MainCharInfo.m_dwAskID = 0;

	}

	return 0;
}

int OnCS_EC_TRADEOPENSACK_ACK( CMsg &msg)
{
	BYTE	bResult;
	DWORD	dwObjectID;

	msg
		>> bResult
		>> dwObjectID;

	CloseAllWindow();
	g_MainCharInfo.ShowHelpMessage( IDS_START_TRADE);

	g_MainCharInfo.m_dwAskID = dwObjectID;

	g_MainCharInfo.ShowSack( SACKTYPE__PC_TRADE_MINE);

	TCHAR szTitle[50];
	_stprintf( szTitle, IDS_D_TRADE_WINDOW, (LPCTSTR)g_MainCharInfo.FindNameByID( dwObjectID));

	g_pUIManager->SetString(WINDOW_PC_TRADE, pc_trade_window_sub_dummy_01, szTitle);

	if( !g_MainCharInfo.m_pMySack[g_MainCharInfo.m_byMySackCurrIdx]->IsShow())
	{
		g_MainCharInfo.ShowSack( SACKTYPE__DEFAULT);
		g_MainCharInfo.ShowSack( SACKTYPE__EQUIPMENT);
	}

	g_pUIManager->DeletePopMenu(); //음야 -_-
	return 0;
}

int OnCS_EC_TRADESACKONITEM_ACK( CMsg &msg)
{
	BYTE	bResult;
	DWORD	dwCharID;
	BYTE	bSrcSackID;
	BYTE	bSrcPos;
	BYTE	bDesSackID;
	BYTE	bDesPos;
	DWORD	dwItemID;
	DWORD	dwAmount;

	XiahItem::sItemInfo *pItemInfo = new XiahItem::sItemInfo;

	msg
		>> bResult
		>> dwCharID
		>> bSrcSackID
		>> bSrcPos
		>> bDesSackID
		>> bDesPos
		>> dwItemID
		>> dwAmount;
	XiahItem::GetItemData( pItemInfo, msg);

	//HT_1116 : 각성자 아이템 추가
	msg
		>> pItemInfo->m_wRebuithValue;

	pItemInfo->m_dwItemID = dwItemID;
	pItemInfo->m_dwAmount = dwAmount;
	pItemInfo->m_bSackID = bDesSackID;
	pItemInfo->m_bSackPos = bDesPos;

	if( bResult !=0)
		return 0;

	// CharID가 자신이면 내 꺼를 올린경우고
	// RemoveFromSack이 오니까.. 트레이트창에 추가만 하면 된다.
	if( dwCharID == g_MainCharInfo.m_dwObjectID) 
	{
		// [8/13/2004]R BUGFIX
		if(g_MainCharInfo.m_pPcSackMine)
			g_MainCharInfo.m_pPcSackMine->InsertItem( bDesPos, pItemInfo);
	}
	// 다른 넘이면 그넘이 올린 경우겠지.
	else
	{
		// [8/13/2004]R BUGFIX
		if(g_MainCharInfo.m_pPcSackOther)
			g_MainCharInfo.m_pPcSackOther->InsertItem( bDesPos, pItemInfo);
	}

	return 0;
}

int OnCS_EC_TRADESACKOFFITEM_ACK( CMsg &msg)
{
	BYTE	bResult;
	DWORD	dwCharID;
	BYTE	bSrcSackID;
	BYTE	bSrcPos;
	BYTE	bDesSackID;
	BYTE	bDesPos;
	DWORD	dwItemID;
	DWORD	dwAmount;

	msg
		>> bResult
		>> dwCharID
		>> bSrcSackID
		>> bSrcPos
		>> bDesSackID
		>> bDesPos
		>> dwItemID
		>> dwAmount;

	if(bResult != 0) 
		return 0;

	// CharID가 자신이면 내 꺼를 내린경우고
	// AddOnSack이 오니까 Trade창에서만 지우자
	if( dwCharID == g_MainCharInfo.m_dwObjectID) 
	{
		if( g_MainCharInfo.m_pHoldItem)
			g_MainCharInfo.m_pHoldItem->DeleteHoldItemItem();
	}
	// 다른 넘이면 그넘이 내린 경우겠지.
	else 
	{
		// 누수 수정
		if( g_MainCharInfo.m_pPcSackOther)
			g_MainCharInfo.m_pPcSackOther->DeleteItem( bSrcPos, true);
	}

	return 0;
}

int OnCS_EC_TRADEITEM_ACK( CMsg &msg)
{
	BYTE	bResult		=100;
	DWORD	dwTraderID	=0;

	msg
		>> bResult
		>> dwTraderID;		// Trade Accept 한 녀석 아이디

	switch( bResult)
	{
	case 0:
		{
			// 내가 동의
			if( dwTraderID == g_MainCharInfo.m_dwObjectID)
			{
				//g_pUIManager->GetFrame( WINDOW_PC_TRADE)->GetControl( pc_trade_window_button_01)
				g_MainCharInfo.m_bTradeAgree = TRUE;
				g_MainCharInfo.ShowHelpMessage( IDS_AGREE_TRADE);
			}
			// 상대방이 동의
			else if( dwTraderID == g_MainCharInfo.m_dwAskID)
			{
				g_MainCharInfo.m_bTradeAgree = TRUE;

				TCHAR content[100];
				_stprintf( content, IDS_D_TRADE_ACCEPT, (LPCTSTR)g_MainCharInfo.FindNameByID( g_MainCharInfo.m_dwAskID));
				g_MainCharInfo.ShowHelpMessage( content);

				g_pUIManager->SetString(WINDOW_PC_TRADE, pc_trade_status_dumy, IDS_OTHER_AGREE);
			}
		}
		break;
	case 12:
		{
			g_MainCharInfo.m_dwAskID = 0;
			g_MainCharInfo.m_bTradeAgree = FALSE;

			g_MainCharInfo.ShowHelpMessage(IDS_TRADE_CANCEL_2, TEXTEFFECT_COLOR_WARNING);

			// trade 창 닫기
			g_MainCharInfo.HideSack( SACKTYPE__PC_TRADE_MINE);
			g_MainCharInfo.HideSack( SACKTYPE__PC_TRADE_OTHER);

			if( g_MainCharInfo.m_pHoldItem)
			{
				if( g_MainCharInfo.m_pHoldItem->IsHoldingItemItem())
				{
					//g_MainCharInfo.m_pHoldItem->DeleteHoldItemItem();
					g_MainCharInfo.m_pHoldItem->SetItemBackToSack();
				}
			}
		}
		break;
	case 9:
	default:
		{
			g_MainCharInfo.m_dwAskID = 0;
			g_MainCharInfo.m_bTradeAgree = FALSE;

			g_MainCharInfo.ShowHelpMessage(IDS_TRADE_CANCEL,TEXTEFFECT_COLOR_WARNING);
			// trade 창 닫기
			g_MainCharInfo.HideSack( SACKTYPE__PC_TRADE_MINE);
			g_MainCharInfo.HideSack( SACKTYPE__PC_TRADE_OTHER);
			
			if( g_MainCharInfo.m_pHoldItem)
			{
				if( g_MainCharInfo.m_pHoldItem->IsHoldingItemItem())
				{
					//g_MainCharInfo.m_pHoldItem->DeleteHoldItemItem();
					g_MainCharInfo.m_pHoldItem->SetItemBackToSack();
				}
			}
		}
		break;
	}

	g_MainCharInfo.m_bInteractionFlag = FALSE;

	return 0;
}

int OnCS_EC_TRADECOMPLETE_ACK( CMsg &msg)
{
	BYTE	bResult;
	DWORD	dwTraderID;

	msg
		>> bResult
		>> dwTraderID;

	g_MainCharInfo.ShowHelpMessage(IDS_TRADE_COMPLETE);
	// trade 창 닫기
	g_MainCharInfo.m_dwAskID = 0;
	g_MainCharInfo.m_bTradeAgree = FALSE;

	g_MainCharInfo.HideSack( SACKTYPE__PC_TRADE_MINE);
	g_MainCharInfo.HideSack( SACKTYPE__PC_TRADE_OTHER);

	return 0;
}

int OnCS_EC_TRADESACKONMONEY_ACK( CMsg &msg)
{
	BYTE bResult	=0;
	DWORD dwCharID	=0;
	DWORD dwAmount	=0;

	msg
		>> bResult
		>> dwCharID
		>> dwAmount;

	if( bResult)
	{
		g_MainCharInfo.ShowHelpMessage(IDS_CANNOT_MOVE_AFTER_AGREE,TEXTEFFECT_COLOR_WARNING);
	
		return 0;
	}

	if( dwCharID == g_MainCharInfo.m_dwObjectID)
	{
		// 내 돈
		g_MainCharInfo.m_dwMoneyOnTradeMine += dwAmount;
		
		g_pUIManager->SetString(WINDOW_PC_TRADE, pc_trade_window_sub_dummy_04, MoneyCommaStr(g_MainCharInfo.m_dwMoneyOnTradeMine),
								g_MainCharInfo.MoneyUnitColor(g_MainCharInfo.m_dwMoneyOnTradeMine));
	}
	else
	{
		// 상대방 돈
		g_MainCharInfo.m_dwMoneyOnTradeOther += dwAmount;
		
		g_pUIManager->SetString(WINDOW_PC_TRADE, pc_trade_window_sub_dummy_03, MoneyCommaStr(g_MainCharInfo.m_dwMoneyOnTradeOther),
								g_MainCharInfo.MoneyUnitColor(g_MainCharInfo.m_dwMoneyOnTradeOther) );
	}	
	
	g_MainCharInfo.m_pHoldItem->ReleaseHoldItemMoney();

	return 0;
}

int OnCS_EC_TRADESACKOFFMONEY_ACK( CMsg &msg)
{
	BYTE bResult;
	DWORD dwCharID;
	DWORD dwAmount;

	msg
		>> bResult
		>> dwCharID
		>> dwAmount;

	return 0;
}


int OnCS_EC_ITEMLISTINBANK_ACK( CMsg &msg)
{
	BYTE bSackPos;
	BYTE bAction;
	DWORD dwItemNum;
	BOOL bflag;
	msg
		>> bflag;

	//HT_1013 : 행낭 아이템 잘 보인다. 
	if(bflag)
	{
		CloseAllWindow();
		g_MainCharInfo.ShowSack( SACKTYPE__DEPOSIT);
		g_MainCharInfo.ShowSack( SACKTYPE__DEFAULT);
		g_MainCharInfo.ShowSack( SACKTYPE__EQUIPMENT);
	}
//	#define ACT_NORMALOPEN						((BYTE)1)
//	#define ACT_ITEMOPEN						((BYTE)2)

	msg
		>> bAction
		>> dwItemNum;

	// 윈도우 TITLE을 지정한다
	if(bAction == OPEN_NORMAL)
	{
		// none
	}
	else
	if(bAction == OPEN_ITEM)
	{
		g_pUIManager->SetString(WINDOW_NPC_TRADE, npc_trade_window_title_back, IDS_JANGBOO_NAME);
	}


	for(int i=0; i<dwItemNum; i++) 
	{
		XiahItem::sItemInfo* pItem = new XiahItem::sItemInfo;

		msg
			>> bSackPos;
		XiahItem::GetItemData( pItem, msg);

		pItem->m_bSackID = SACKTYPE__DEPOSIT;

		g_MainCharInfo.m_pDepositSack->InsertItem( bSackPos, pItem);
	}

	g_MainCharInfo.m_pDepositSack->SetAction(bAction);

	return 0;
}

int OnCS_EC_ADDONBANK_ACK( CMsg &msg)
{
	BYTE bAction;
	BYTE bSackPos;

	XiahItem::sItemInfo* pItem = new XiahItem::sItemInfo;

	msg
		>> bAction
		>> bSackPos;

	XiahItem::GetItemData( pItem, msg);

	pItem->m_bSackID = SACKTYPE__DEPOSIT;
	g_MainCharInfo.m_pDepositSack->InsertItem( bSackPos, pItem);

	if( bAction == 1)	// bAction == 2 면 창고내에서의 움직임
	{
		TCHAR content[100];

		DWORD theprice = (DWORD)((pItem->m_dwPrice / 100) * pItem->m_dwAmount);

		// 가격이 0이어도 1이 감소한다.
		if(theprice < 1)
		{
			theprice = 1;
		}

		_stprintf( content, IDS_D_BOGUAN_JIBUL, (LPCTSTR)pItem->m_szName, theprice);
		g_MainCharInfo.ShowHelpMessage( content,TEXTEFFECT_COLOR_WARNING);
	}

	return 0;
}

int OnCS_EC_REMOVEFROMBANK_ACK( CMsg &msg)
{
	BYTE bSackPos;
	DWORD dwItemID;

	msg
		>> bSackPos
		>> dwItemID;

	if( g_MainCharInfo.m_pHoldItem->IsHoldingItemItem())
	{
		g_MainCharInfo.m_pHoldItem->DeleteHoldItemItem();
	}
	
	return 0;
}

int OnCS_EC_DRAWINBANK_ACK( CMsg &msg)
{
	BYTE bResult;

	msg
		>> bResult;

	if( !bResult)
		return 0;

	TCHAR str[50];

	switch( bResult)
	{
	case ERR_DRAWINBANK_DONOTFINDSACK:		// 행낭안에 돈없음
		_stprintf( str, IDS_PURSE_IN_LOWMONEY);		
		break;
	case ERR_DRAWINBANK_NOTEMPTYBANK:
		_stprintf( str, CANT_PUTIN, 0);
		break;
	case ERR_DRAWINBANK_FULLBANK:
		_stprintf( str, IDS_FULL_CARGO, 0);
		break;
	case ERR_DRAWINBANK_DONOTADDBANK:
		_stprintf( str, CANT_PUTIN, 0);
		break;
	case ERR_DRAWINBANK_DONOTDELSACK:		
		_stprintf( str, IDS_ERROR, ERR_DRAWINBANK_DONOTDELSACK);
		break;
	}	

	g_MainCharInfo.ShowHelpMessage(str,TEXTEFFECT_COLOR_WARNING);
	if( g_MainCharInfo.m_pHoldItem->IsHoldingItemItem())
		g_MainCharInfo.m_pHoldItem->SetItemBackToSack();

	return 0;
}

int OnCS_EC_DRAWOUTBANK_ACK( CMsg &msg)
{
	BYTE bResult;
	TCHAR str[50] = {0,};

	msg
		>> bResult;

	if( !bResult)
		return 0;

	switch( bResult)
	{
	case ERR_DRAWOUTBANK_DONOTFINDBANK:
		{
			_stprintf( str, IDS_ERROR, ERR_DRAWOUTBANK_DONOTFINDBANK);
			g_MainCharInfo.ShowHelpMessage(str);
		}
		break;
	case ERR_DRAWOUTBANK_NOTEMPTYSACK:	
		{
			_stprintf( str, MISMATCH_POS, ERR_DRAWOUTBANK_NOTEMPTYSACK);
			g_MainCharInfo.ShowHelpMessage(str);
		}
		break;
	case ERR_DRAWOUTBANK_FULLSACK:	
		g_MainCharInfo.ShowHelpMessage( IDS_FULL_SACK, TEXTEFFECT_COLOR_WARNING);
		break;
	case ERR_DRAWOUTBANK_DONOTDELBANK:
		{
			_stprintf( str, IDS_ERROR, ERR_DRAWOUTBANK_DONOTDELBANK);
			g_MainCharInfo.ShowHelpMessage(str);
		}
		break;
	case ERR_DRAWOUTBANK_DONOTADDSACK:	
		{
			_stprintf( str, IDS_ERROR, ERR_DRAWOUTBANK_DONOTADDSACK);
			g_MainCharInfo.ShowHelpMessage(str);
		}
		break;
	}

	if( g_MainCharInfo.m_pHoldItem->IsHoldingItemItem())
		g_MainCharInfo.m_pHoldItem->SetItemBackToSack();

	return 0;
}

int OnCS_EC_DRAWMOVEBANK_ACK( CMsg &msg)
{
	BYTE bResult;

	msg
		>> bResult;

	if( !bResult)
		return 0;

	TCHAR str[50] = {0,};

	switch( bResult)
	{
	case ERR_DRAWMOVEBANK_NOTFINDFIRST:
		_stprintf( str, IDS_ERROR, ERR_DRAWMOVEBANK_NOTFINDFIRST);
		break;
	case ERR_DRAWMOVEBANK_NOTFINDSECOND:
		_stprintf( str, IDS_ERROR, ERR_DRAWMOVEBANK_NOTFINDSECOND);
		break;
	case ERR_DRAWMOVEBANK_CHANGE:
		_stprintf( str, IDS_ERROR, ERR_DRAWMOVEBANK_CHANGE);
		break;

	// 공간부족함
	case ERR_DRAWMOVEBANK_NOTEMPTY:
		_stprintf( str, IDS_BANKERROR4);
		break;
	}

	g_MainCharInfo.ShowHelpMessage(str,TEXTEFFECT_COLOR_WARNING);

	if( g_MainCharInfo.m_pHoldItem->IsHoldingItemItem())
		g_MainCharInfo.m_pHoldItem->SetItemBackToSack();

	return 0;
}

//////////////////////////////////////////////////////////////////////////
//	ITEMMALL
//////////////////////////////////////////////////////////////////////////

int OnCS_EC_ITEMLISTINMALL_ACK( CMsg &msg)
{
	BYTE bSackPos;
	DWORD dwItemNum;

	CloseAllWindow();
	g_MainCharInfo.ShowSack( SACKTYPE__ITEMMALL);
	g_MainCharInfo.ShowSack( SACKTYPE__DEFAULT);
	g_MainCharInfo.ShowSack( SACKTYPE__EQUIPMENT);

	msg
		>> dwItemNum;

	for(int i=0; i<dwItemNum; i++) 
	{
		XiahItem::sItemInfo* pItem = new XiahItem::sItemInfo;

		msg
			>> bSackPos;
		XiahItem::GetItemData( pItem, msg);

		pItem->m_bSackID = SACKTYPE__ITEMMALL;

		g_MainCharInfo.m_pItemMallSack->InsertItem( bSackPos, pItem);
	}	

	return 0;
}

int OnCS_EC_DRAWOUTMALL_ACK( CMsg &msg)
{
	BYTE bResult;
	TCHAR str[50] = {0,};

	msg
		>> bResult;

	if( !bResult)
		return 0;

	switch( bResult)
	{
	case ERR_DRAWOUTBANK_DONOTFINDBANK:
		{
			_stprintf( str, IDS_ERROR, ERR_DRAWOUTBANK_DONOTFINDBANK);
			g_MainCharInfo.ShowHelpMessage(str);
		}
		break;
	case ERR_DRAWOUTBANK_NOTEMPTYSACK:	
		{
			_stprintf( str, MISMATCH_POS, ERR_DRAWOUTBANK_NOTEMPTYSACK);
			g_MainCharInfo.ShowHelpMessage(str);
		}
		break;
	case ERR_DRAWOUTBANK_FULLSACK:	
		g_MainCharInfo.ShowHelpMessage( IDS_FULL_SACK, TEXTEFFECT_COLOR_WARNING);
		break;
	case ERR_DRAWOUTBANK_DONOTDELBANK:
		{
			_stprintf( str, IDS_ERROR, ERR_DRAWOUTBANK_DONOTDELBANK);
			g_MainCharInfo.ShowHelpMessage(str);
		}
		break;
	case ERR_DRAWOUTBANK_DONOTADDSACK:	
		{
			_stprintf( str, IDS_ERROR, ERR_DRAWOUTBANK_DONOTADDSACK);
			g_MainCharInfo.ShowHelpMessage(str);
		}
		break;
	}

	if( g_MainCharInfo.m_pHoldItem->IsHoldingItemItem())
		g_MainCharInfo.m_pHoldItem->SetItemBackToSack();

	return 0;
}

int OnCS_EC_DRAWMOVEMALL_ACK( CMsg &msg)
{
	BYTE bResult;

	msg
		>> bResult;

	if( !bResult)
		return 0;

	TCHAR str[50] = {0,};

	switch( bResult)
	{
	case ERR_DRAWMOVEBANK_NOTFINDFIRST:
		_stprintf( str, IDS_ERROR, ERR_DRAWMOVEBANK_NOTFINDFIRST);
		break;
	case ERR_DRAWMOVEBANK_NOTFINDSECOND:
		_stprintf( str, IDS_ERROR, ERR_DRAWMOVEBANK_NOTFINDSECOND);
		break;
	case ERR_DRAWMOVEBANK_CHANGE:
		_stprintf( str, IDS_ERROR, ERR_DRAWMOVEBANK_CHANGE);
		break;

		// 공간부족함
	case ERR_DRAWMOVEBANK_NOTEMPTY:
		_stprintf( str, IDS_BANKERROR4);
		break;
	}

	g_MainCharInfo.ShowHelpMessage(str,TEXTEFFECT_COLOR_WARNING);

	if( g_MainCharInfo.m_pHoldItem->IsHoldingItemItem())
		g_MainCharInfo.m_pHoldItem->SetItemBackToSack();

	return 0;
}


int OnCS_EC_ADDONMALL_ACK( CMsg &msg)
{
	BYTE bSackPos;
	XiahItem::sItemInfo* pItem = new XiahItem::sItemInfo;

	msg
		>> bSackPos;

	XiahItem::GetItemData( pItem, msg);

	pItem->m_bSackID = SACKTYPE__ITEMMALL;
	g_MainCharInfo.m_pItemMallSack->InsertItem( bSackPos, pItem);

	return 0;
}

int OnCS_EC_REMOVEFROMMALL_ACK( CMsg &msg)
{
	BYTE bSackPos;
	DWORD dwItemID;

	msg
		>> bSackPos
		>> dwItemID;

	if( g_MainCharInfo.m_pHoldItem->IsHoldingItemItem())
	{
		g_MainCharInfo.m_pHoldItem->DeleteHoldItemItem();
	}

	return 0;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////
// 복권
/**
 * 복권구입
 * \param &msg 
 * \return 
 */
int OnCS_EC_BUYLOTTO_ACK(CMsg &msg)
{
	BYTE bResult =0;

	msg
		>> bResult;

	switch(bResult)
	{
	case ERR_BUYLOTTO_SUCCESS:			// 구입성공
		{
			g_pUIManager->ShowNotice(IDS_LOTTO_BUY_3);								
		}
		break;
	case ERR_BUYLOTTO_NOERROR:			// 번호에러
		{			
			g_MainCharInfo.ShowHelpMessage(IDS_LOTTO_NUMERROR, TEXTEFFECT_COLOR_WARNING);
		}
		break;
	case ERR_BUYLOTTO_LESSMONEY:		// 돈부족
		{
			g_MainCharInfo.ShowHelpMessage(IDS_PURSE_IN_LOWMONEY, TEXTEFFECT_COLOR_WARNING);
		}		
		break;
	case ERR_BUYLOTTO_NOTSALE:			// 판매중지
		{
			g_MainCharInfo.ShowHelpMessage(IDS_LOTTO_NOTSELL, TEXTEFFECT_COLOR_WARNING);
		}
		break;
	case ERR_BUYLOTTO_NOTEMPTYSACK:		// 행낭공간없음
		{
			g_MainCharInfo.ShowHelpMessage(IDS_PCSHOP_FULLSACK, TEXTEFFECT_COLOR_WARNING);
		}
		break;
	case ERR_BUYLOTTO_INTERNALERROR:	// 내부에러
		{
			g_MainCharInfo.ShowHelpMessage(IDS_REL_ERR_INTERNAL, TEXTEFFECT_COLOR_WARNING);
		}
		break;
	default:
		break;
	} // switch(bResult)

	return 0;
}

/**
 * 복권 예상당첨금액 횅땍
 * \param &msg 
 * \return 
 */
int OnCS_EC_LOTTOSALEINFO_ACK(CMsg &msg)
{
	__int64 biPreMoney		=0;
	__int64 biPrizeMoney	=0;

	msg
		>> biPreMoney		// 이월금액
		>> biPrizeMoney;	// 예상금액

	g_MainCharInfo.CloseFrame(WINDOW_BOK_NUMBER);

	TCHAR strTemp[256] = {0,};
	_stprintf(strTemp, IDS_LOTTO_EXPECT_INFO, MoneyCommaStr(biPreMoney).data(), MoneyCommaStr(biPrizeMoney).data());
	g_pUIManager->ShowNotice(strTemp);

	return 0;
}

/**
 * 당첨번호 횅땍
 * \param &msg 
 * \return 
 */
int OnCS_EC_PRIZELOTTOINFO_ACK(CMsg &msg)
{
	BYTE bResult	=0;

	msg
		>> bResult;

	switch(bResult)
	{
	case 0:
		{
			DWORD	dwRound		=0;
			BYTE	bNum[4];

			ZeroMemory(&bNum, sizeof(BYTE)*4);
			
			msg
				>> dwRound
				>> bNum[0]	// 당첨번호
				>> bNum[1]
				>> bNum[2]
				>> bNum[3];

			TCHAR strTemp[128] = {0,};
			_stprintf(strTemp, IDS_LOTTO_PRIZEWIN_TITLE, dwRound);
			g_pUIManager->SetString(WINDOW_BOK_PRIZE, bok_prize_dummy_01, strTemp);

			_stprintf(strTemp, IDS_LOTTO_SELECT_NUM, bNum[0], bNum[1], bNum[2], bNum[3]);
			g_pUIManager->SetString(WINDOW_BOK_PRIZE, bok_prize_dummy_02, strTemp);

			__int64 biMoney		=0;
			DWORD	dwPrizer	=0;

			// 1등부터~3등까지
			for(int i=0; i < 3; ++i)
			{
				msg
					>> biMoney		// 당첨금액
					>> dwPrizer;	// 당첨수

				_stprintf(strTemp, IDS_LOTTO_PRIZEWIN_INFO, i+1, MoneyCommaStr(biMoney).data(), dwPrizer);
				g_pUIManager->SetString(WINDOW_BOK_PRIZE, bok_prize_dummy_03+i, strTemp, 5);
			}			

			g_MainCharInfo.OpenFrame(WINDOW_BOK_PRIZE);
		}
		break;
	case 1:	// 정보없음 (첫회)
		{
			g_MainCharInfo.ShowHelpMessage(IDS_LOTTO_FIRST, TEXTEFFECT_COLOR_WARNING);
		}
		break;
	default:
		break;
	}

	return 0;
}

/**
 * 복권 당첨금 회수
 * \param &msg 
 * \return 
 */
int OnCS_EC_GETLOTTOMONEY_ACK(CMsg &msg)
{
	BYTE bResult	=0;
	DWORD dwMoney	=0;

	msg
		>> bResult
		>> dwMoney;

	switch(bResult)
	{
	case ERR_GETLOTTOMONEY_SUCCESS:			// 성공
		{
			TCHAR strTemp[128] = {0,};
			__int64 nTempMoney = dwMoney * 1000;
			_stprintf(strTemp, IDS_LOTTO_MONEY_RECEIVE, MoneyCommaStr(nTempMoney).data());

			g_MainCharInfo.ShowHelpMessage(strTemp);			
		}
		break;
	case ERR_GETLOTTOMONEY_NOTFINDITEM:		// 아이템 찾을수 없음
		{
			g_MainCharInfo.ShowHelpMessage(IDS_ITEM_NOTFIND, TEXTEFFECT_COLOR_WARNING);
		}
		break;
	case ERR_GETLOTTOMONEY_NOTLOTTERY:		// 미추첨
		{
			g_MainCharInfo.ShowHelpMessage(IDS_LOTTERY, TEXTEFFECT_COLOR_WARNING);
		}
		break;
	case ERR_GETLOTTOMONEY_NOTCHECK:		// 당첨미횅땍
		{
			g_MainCharInfo.ShowHelpMessage(IDS_LOTTO_NOTCHECK, TEXTEFFECT_COLOR_WARNING);
		}
		break;
	case ERR_GETLOTTOMONEY_OVERMONEY:		// 행낭금액 초과
		{
			g_MainCharInfo.ShowHelpMessage(IDS_PURSE_OUT_OVERMONEY, TEXTEFFECT_COLOR_WARNING);
		}
		break;
	case ERR_GETLOTTOMONEY_OVERPRIZEMONEY:	// 당첨금액이 많음
		{
			g_MainCharInfo.ShowHelpMessage(IDS_LOTTO_OVERPRIZEMONEY, TEXTEFFECT_COLOR_WARNING);
		}
		break;

	case ERR_GETLOTTOMONEY_INTERNALERROR:	// 내부에러
		{
			g_MainCharInfo.ShowHelpMessage(IDS_REL_ERR_INTERNAL, TEXTEFFECT_COLOR_WARNING);
		}
		break;
	default:
		break;
	} // switch(bResult)

	return 0;
}


/**
 * 복권아이템 당첨여부 횅땍
 * \param &msg 
 * \return 
 */
int OnCS_EC_CHECKLOTTO_ACK(CMsg &msg)
{
	BYTE bResult	=0;
	BYTE bPrizeRank	=0;
	DWORD dwPrizeMoney =0;

	msg
		>> bResult
		>> bPrizeRank
		>> dwPrizeMoney;

	switch(bResult)
	{
	case ERR_CHECKLOTTO_SUCCESS:		// 성공
		{
			TCHAR strTemp[128] = {0,};
			__int64 nTempMoney = dwPrizeMoney * 1000;
			_stprintf(strTemp, IDS_LOTTO_N_PRIZEWIN, bPrizeRank, MoneyCommaStr(nTempMoney).data());
			g_MainCharInfo.ShowHelpMessage(strTemp);
		}
		break;
	case ERR_CHECKLOTTO_NOTFINDITEM:	// 아이템 찾을수 없음
		{
			g_MainCharInfo.ShowHelpMessage(IDS_ITEM_NOTFIND, TEXTEFFECT_COLOR_WARNING);
		}
		break;
	case ERR_CHECKLOTTO_NOTLOTTERY:		// 미당첨
		{
			g_MainCharInfo.ShowHelpMessage(IDS_NOLOTTERY, TEXTEFFECT_COLOR_WARNING);
		}
		break;
	case ERR_CHECKLOTTO_ALREADYCHECK:	// 이미 횅땍했음
		{
			g_MainCharInfo.ShowHelpMessage(IDS_LOTTO_ALREADYCHECK, TEXTEFFECT_COLOR_WARNING);			
		}
		break;
	case ERR_CHECKLOTTO_INTERROR:		// 내부에러
		{
			g_MainCharInfo.ShowHelpMessage(IDS_REL_ERR_INTERNAL, TEXTEFFECT_COLOR_WARNING);
		}
		break;
	case ERR_CHECKLOTTO_NOTJUDGE:		// 미추첨
		{
			g_MainCharInfo.ShowHelpMessage(IDS_LOTTERY, TEXTEFFECT_COLOR_WARNING);
		}
		break;
	default:
		break;
	}

	return 0;
}

/**
 * 복권 공지사항
 * \param &msg 
 * \return 
 */
int OnCS_EC_LOTTONOTICE_ACK(CMsg &msg)
{
	BYTE bType		=0;
	DWORD dwData[4];

	ZeroMemory(&dwData, sizeof(DWORD)*4);

	msg
		>> bType
		>> dwData[0]
		>> dwData[1]
		>> dwData[2]
		>> dwData[3];

	switch(bType)
	{
	case LOTTONOTICE_NONE:
		break;

	case LOTTONOTICE_ENDSALE:		// 판매종료
		{
			TCHAR strTemp[128] = {0,};
			_stprintf(strTemp, IDS_LOTTO_N_END, dwData[0]);
			g_MainCharInfo.SpecialChatMessage(strTemp, 0);

			__int64 nTempMoney = dwData[1] * 1000;
			_stprintf(strTemp, IDS_LOTTO_N_TOTALSELL, MoneyCommaStr(nTempMoney).data());
			g_MainCharInfo.SpecialChatMessage(strTemp,0);
		}
		break;
	case LOTTONOTICE_LOTTERYEND:	// 추첨완료
		{
			TCHAR strTemp[128] = {0,};
			_stprintf(strTemp, IDS_LOTTO_N_PRIZEWIN_NUM, dwData[0], dwData[1], dwData[2], dwData[3]);
			g_MainCharInfo.SpecialChatMessage(strTemp, 0);
		}
		break;
	case LOTTONOTICE_LOTTERYNOTICE:	// 당첨자공지
		{
			TCHAR strTemp[128] = {0,};

			if(dwData[1])
			{
				__int64 nTempMoney = dwData[1] * 1000;

				// 당첨자 공지
				_stprintf(strTemp, IDS_LOTTO_N_RANK1, dwData[0], MoneyCommaStr(nTempMoney).data());
				g_MainCharInfo.SpecialChatMessage(strTemp, 0);							
			}
			else
			{
				// 당첨자 없음
				g_MainCharInfo.SpecialChatMessage(IDS_LOTTO_N_RANK1_NOT, 0);				
			}

			// 이월금
			if(dwData[2])
			{
				__int64 nTempMoney = dwData[2] * 1000;
				_stprintf(strTemp, IDS_LOTTO_N_CARRYFORWARD, MoneyCommaStr(nTempMoney).data());
				g_MainCharInfo.SpecialChatMessage(strTemp, 0);
			}
		}
		break;
	case LOTTONOTICE_STARTSALE:	// 판매시작
		{
			TCHAR strTemp[128] = {0,};
			_stprintf(strTemp, IDS_LOTTO_N_START, dwData[0]);
			g_MainCharInfo.SpecialChatMessage(strTemp, 0);

			if(dwData[1])	// 이월금 있을경우
			{
				__int64 nTempMoney = dwData[1] * 1000;
				_stprintf(strTemp, IDS_LOTTO_N_CARRYFORWARD_2, MoneyCommaStr(nTempMoney).data());
				g_MainCharInfo.SpecialChatMessage(strTemp, 0);
			}
		}
		break;
	default:
		break;
	}

	return 0;
}

/**
 * 보험 아이템
 * \param &msg 
 * \return 
 */
int OnCS_EC_GUARANTEELIST_ACK(CMsg &msg)
{
	BYTE bTotalCount = 0;
	BYTE bItemCount  = 0;

	msg
		>> bTotalCount
		>> bItemCount;

	g_MainCharInfo.m_vRecoveryItem.clear();
	g_MainCharInfo.m_vRecoveryItemName.clear();
	
	g_MainCharInfo.m_dwResItemID = 0;

	CloseAllWindow();

	g_MainCharInfo.OpenFrame(WINDOW_RECOVERY);

	TCHAR szTemp[64] = {0,};
	_stprintf(szTemp, IDS_RECOVERY_SELECT, bItemCount, bTotalCount);
	g_pUIManager->SetString(WINDOW_RECOVERY, recovery_window_top_dummy, szTemp);

	for(int j=0; j < 6; ++j)
	{
		g_pUIManager->SetString(WINDOW_RECOVERY, recovery_window_back_dummy01+j, _T(" "));
		g_pUIManager->SetData(WINDOW_RECOVERY, recovery_window_back_dummy01+j, COLOR, 0, 0, D3DCOLOR_XRGB(255, 255, 255));
	}

	for(BYTE i=0; i < bItemCount; ++i)
	{
		DWORD dwItemID =0;
		sString	szItemName;

		msg
			>> dwItemID
			>> szItemName;

		g_MainCharInfo.m_vRecoveryItem.push_back(dwItemID);
		g_MainCharInfo.m_vRecoveryItemName.push_back(szItemName);

		g_pUIManager->SetString(WINDOW_RECOVERY, recovery_window_back_dummy01+i, szItemName);
	}

	return 0;
}


/**
 * 매품패
 * \param &msg 
 * \return 
 */
int OnCS_EC_QUICKMART_ACK(CMsg &msg)
{
	BYTE bResult = 0;

	msg
		>> bResult;

	switch(bResult)
	{
	case 0:		// 열기
		{
			 CloseAllWindow();

			g_MainCharInfo.ShowSack(SACKTYPE__DEFAULT);
			g_MainCharInfo.ShowSack(SACKTYPE__EQUIPMENT);

			g_MainCharInfo.ShowSack(SACKTYPE__QUICKMART);
		}
		break;
	case 1:		// 이미 열려있음
		{
			g_MainCharInfo.ShowSack(SACKTYPE__QUICKMART);

			// TODO: 문구
		}
		break;
	case 255:	// 시스템 댄轎
		{
			g_MainCharInfo.ShowHelpMessage(IDS_REL_ERR_INTERNAL, TEXTEFFECT_COLOR_WARNING);
		}
	    break;
	default:
		break;
	}
	
	return 0;
}
