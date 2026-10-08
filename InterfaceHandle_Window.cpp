#include "mail.h"
#include "XiahEnvInfo.h"
#include "xiahbgmcore.h"
#include "Helper.h"
#include "CurseFilter.h"
#include <io.h>

extern sString MoneyCommaStr(INT64 nMoney);
extern BOOL NameFiltering(sString szNickName);


//////////////////////////////////////////////////
// Character Information
//////////////////////////////////////////////////
void ProcessWindowCharacter( LPARAM lParam)
{
	int controlID = LOWORD( lParam);
	int eventType = HIWORD( lParam);

	switch( controlID)
	{
	case character_close_button:
		g_MainCharInfo.CloseFrame( WINDOW_CHARACTER);
		break;
	case character_window_button_up_01:
		SendCS_BT_EXECSP_REQ( SP_STR, 1);
		break;
	case character_window_button_up_02:
		SendCS_BT_EXECSP_REQ( SP_SUS, 1);		
		break;
	case character_window_button_up_03:
		SendCS_BT_EXECSP_REQ( SP_DEX, 1);
		break;
	case character_window_button_up_04:
		SendCS_BT_EXECSP_REQ( SP_VIT, 1);
		break;
	case 51: // 称号系统：重构意图：在能力值界面点击切换三角形按钮，通过发送 51 号封包静默通知服务器进行切换称号
		SendCS_IM_USEITEM_REQ(0, 0, 51);
		break;
	}
}











//////////////////////////////////////////////////
// Sack
//////////////////////////////////////////////////
void ProcessWindowItem( LPARAM lParam)
{
	int controlID = LOWORD(lParam);

	switch( controlID)
	{
		case drg_item_window_close_button:
			{
				// 毵ろ拡韺
				if(g_MainCharInfo.m_pQuickMart)
				{
					g_MainCharInfo.HideSack(SACKTYPE__QUICKMART, FALSE);					
				}

				g_MainCharInfo.HideSack( SACKTYPE__DEFAULT);
				g_MainCharInfo.HideSack( SACKTYPE__EQUIPMENT);
				g_MainCharInfo.HideSack( SACKTYPE__NPC_TRADE);
				g_MainCharInfo.HideSack( SACKTYPE__DEPOSIT);
				g_MainCharInfo.HideSack( SACKTYPE__MODIFY);					// 臧滌“
				g_MainCharInfo.HideSack( SACKTYPE__PERSONAL_TRADE_SET);		// 臧滌澑靸侅爯靹れ爼
				g_MainCharInfo.HideSack( SACKTYPE__PERSONAL_TRADE_SELL);	// 臧滌澑靸侅爯韺愲Г
				g_MainCharInfo.HideSack( SACKTYPE__ITEMMALL);				// 鞎勳澊韰滊
				g_MainCharInfo.HideSack( SACKTYPE__SMELT);					// 臁绊暕
				g_MainCharInfo.HideSack( SACKTYPE__FIVEELEMENT_CONVERT);	// 鞓ろ枆 鞝滊牗

				if( g_MainCharInfo.m_pPcSackMine && g_MainCharInfo.m_pPcSackMine->IsShow())
					SendCS_EC_TRADEITEM_REQ( 9, g_MainCharInfo.m_dwAskID);

				if(g_pUIManager->IsShow(WINDOW_CHARACTER))
				{
					g_pUIManager->SetPosition(WINDOW_CHARACTER, WINDOW_FIRST_XPOS, 0);
				}
				else if(g_pUIManager->IsShow(WINDOW_NEW_TAMING))
				{
					CloseAllWindow();
					g_pUIManager->SetPosition(WINDOW_NEW_TAMING, WINDOW_FIRST_XPOS, 0);
					g_MainCharInfo.ShowSack( SACKTYPE__PET_EQUIP);
				}
				else if(g_pUIManager->IsShow(WINDOW_TAMING_ITEM))
				{
					CloseAllWindow();
					g_pUIManager->SetPosition(WINDOW_TAMING_ITEM, WINDOW_FIRST_XPOS, 0);
					g_MainCharInfo.ShowSack( SACKTYPE__PET);
				}
				else if(g_MainCharInfo.m_bPersonalTradeSell)  // 臧滌澑靸侅爯 韺愲Г欷戩澊氅 鞖办浮鞙茧
				{
					CloseAllWindow();
					g_pUIManager->SetPosition(WINDOW_PC_STORE, WINDOW_FIRST_XPOS, 0);
					g_MainCharInfo.m_pPersonalTradeSet->ShowSack();
				}
			}		
			break;
		case drg_item_window_sack_1_button:
			{
				g_MainCharInfo.m_byMySackCurrIdx = 0;

				g_pUIManager->SetData(DRG_ITEM_WINDOW, drg_item_window_sack_1_button, CURRENT_INDEX, 2);
				g_pUIManager->SetData(DRG_ITEM_WINDOW, drg_item_window_sack_2_button, CURRENT_INDEX, -1);
				g_pUIManager->SetData(DRG_ITEM_WINDOW, drg_item_window_sack_3_button, CURRENT_INDEX, -1);

				g_MainCharInfo.ShowSack( SACKTYPE__DEFAULT);
			}		
			break;
		case drg_item_window_sack_2_button:
			{
				g_MainCharInfo.m_byMySackCurrIdx = 1;

				g_pUIManager->SetData(DRG_ITEM_WINDOW, drg_item_window_sack_1_button, CURRENT_INDEX, -1);
				g_pUIManager->SetData(DRG_ITEM_WINDOW, drg_item_window_sack_2_button, CURRENT_INDEX, 2);
				g_pUIManager->SetData(DRG_ITEM_WINDOW, drg_item_window_sack_3_button, CURRENT_INDEX, -1);

				g_MainCharInfo.ShowSack( SACKTYPE__DEFAULT);
			}		
			break;
		case drg_item_window_sack_3_button:
			{
				g_MainCharInfo.m_byMySackCurrIdx = 2;

				g_pUIManager->SetData(DRG_ITEM_WINDOW, drg_item_window_sack_1_button, CURRENT_INDEX, -1);
				g_pUIManager->SetData(DRG_ITEM_WINDOW, drg_item_window_sack_2_button, CURRENT_INDEX, -1);
				g_pUIManager->SetData(DRG_ITEM_WINDOW, drg_item_window_sack_3_button, CURRENT_INDEX, 2);

				g_MainCharInfo.ShowSack( SACKTYPE__DEFAULT);
			}		
			break;
	}
}

void ProcessWindowPcTrade( LPARAM lParam)
{
	int controlID = LOWORD( lParam);
	int eventType = HIWORD( lParam);

	switch( controlID)
	{
	case pc_trade_window_close_button:
	case pc_trade_window_button_02:	// 旆靻
		SendCS_EC_TRADEITEM_REQ( 9, g_MainCharInfo.m_dwAskID);
		break;
	case pc_trade_window_button_01:	// 鞓れ紑鞚
		SendCS_EC_TRADEITEM_REQ( 0, g_MainCharInfo.m_dwAskID);

		g_pUIManager->Hide(WINDOW_PC_TRADE, pc_trade_window_button_01);
		g_pUIManager->SetString(WINDOW_PC_TRADE, pc_trade_status_dumy, IDS_WATING);

		break;
	}
}

/**
 * NPC鞖 瓯半灅彀 - 雼るジ鞖╇弰搿滊弰 靷鞖╈
 * \param lParam 
 */
void ProcessWidnowNpcTrade( LPARAM lParam)
{
	int nControlID = LOWORD(lParam);
	int nEventType = HIWORD(lParam);

	switch(nControlID)
	{
	case npc_trade_window_close_button:
	case npc_trade_window_button:
		{
			g_MainCharInfo.HideSack( SACKTYPE__NPC_TRADE);
			g_MainCharInfo.HideSack( SACKTYPE__DEPOSIT);
			g_MainCharInfo.HideSack( SACKTYPE__ITEMMALL);
			g_MainCharInfo.HideSack( SACKTYPE__PERSONAL_TRADE_SELL);

			// 毵ろ拡韺
			if(g_MainCharInfo.m_pQuickMart) // nEventType == 10
			{				
				g_MainCharInfo.HideSack(SACKTYPE__QUICKMART, FALSE);				
			}			
		}		
		break;
	}
}

/**
 * 臧滌“
 * \param lParam 
 */
void ProcessWindowConvert( LPARAM lParam)
{
	int controlID = LOWORD( lParam);
	int eventType = HIWORD( lParam);

	switch(controlID)
	{
		case convert_window_close_button:
		case convert_window_button_02:
			{
				g_MainCharInfo.HideSack( SACKTYPE__MODIFY);
			}			
			break;

		case convert_window_button_01:
			{
				// 毵岇澕 霌り碃 鞛堧姅欷戩棎 臧滌“頃橂┐ 霌り碃鞛堧姅瓴冹潉 霅橂弻毽半嫟.
				if(NULL != g_MainCharInfo.m_pHoldItem)
				{
					g_MainCharInfo.m_pHoldItem->SetItemBackToSack();
				}

				XiahItem::sItemInfo* pItem = g_MainCharInfo.m_pModifySack->FindSackItemByPos( 0);

				DWORD dwItemID = 0;
				DWORD dwSackID = 0;
				BYTE bItemPos = 255;

				DWORD dwResourceID1 = 0;
				BYTE bResourceSackID1 =01;
				BYTE bResourcePos1 = 0;

				DWORD dwResourceID2 = 0;
				BYTE bResourceSackID2 = 0;
				BYTE bResourcePos2 = 255;

				DWORD dwResourceID3 = 0;
				BYTE bResourceSackID3 = 0;
				BYTE bResourcePos3 = 255;

				if( pItem)
				{
					dwItemID = pItem->m_dwItemID;
					dwSackID = pItem->m_bSackIDPrev + 1;
					bItemPos = pItem->m_bSackPosPrev;

					XiahItem::sItemInfo* pResourceItem1 = g_MainCharInfo.m_pModifySack->FindSackItemByPos( 1);
					XiahItem::sItemInfo* pResourceItem2 = g_MainCharInfo.m_pModifySack->FindSackItemByPos( 2);
					XiahItem::sItemInfo* pResourceItem3 = g_MainCharInfo.m_pModifySack->FindSackItemByPos( 3);

					if( pResourceItem1)
					{
						dwResourceID1 = pResourceItem1->m_dwItemID;
						bResourceSackID1 = pResourceItem1->m_bSackIDPrev + 1;
						bResourcePos1 = pResourceItem1->m_bSackPosPrev;
					}

					if( pResourceItem2)
					{
						dwResourceID2 = pResourceItem2->m_dwItemID;
						bResourceSackID2 = pResourceItem2->m_bSackIDPrev + 1;
						bResourcePos2 = pResourceItem2->m_bSackPosPrev;
					}

					if( pResourceItem3)
					{
						dwResourceID3 = pResourceItem3->m_dwItemID;
						bResourceSackID3 = pResourceItem3->m_bSackIDPrev + 1;
						bResourcePos3 = pResourceItem3->m_bSackPosPrev;
					}

					// 臧滌“鞛愳洂鞚 鞐嗢溂氅 臧滌“臧 攵堦皜電ロ晿雼
					if(NULL != pResourceItem1)
					{
						SendCS_IM_REBUILDITEM_REQ( g_MainCharInfo.m_dwPickedObject,
													dwItemID,
													dwSackID,
													bItemPos,
													dwResourceID1,
													bResourceSackID1,
													bResourcePos1,
													dwResourceID2,
													bResourceSackID2,
													bResourcePos2,
													dwResourceID3,
													bResourceSackID3,
													bResourcePos3);
					}
				}
				else
					g_MainCharInfo.ShowHelpMessage( (IDS_PUT_CONVERTITEM), TEXTEFFECT_COLOR_WARNING);
			}

			break;
	}
}

void ProcessWindowTaming( LPARAM lParam)
{
	int controlID = LOWORD( lParam);
	int eventType = HIWORD( lParam);

	switch( controlID)
	{
	case taming_window_close_button:

		if(g_pUIManager->IsShow(WINDOW_TAMING_ITEM))
		{
			CloseAllWindow();
			g_pUIManager->SetPosition(WINDOW_TAMING_ITEM, WINDOW_FIRST_XPOS, 0);
			g_MainCharInfo.ShowSack( SACKTYPE__PET);
		}
		else
		{
			//HT_CHEAT : 韼 靸來儨彀 靾橃爼
			//g_MainCharInfo.HideSack( SACKTYPE__PET_EQUIP);
			sPetInfo* pPetInfo = g_PetList.GetCurrentPet();
			if( pPetInfo && pPetInfo->m_pEquipSack)
			{
				pPetInfo->m_pEquipSack->HideSack();
			}

			g_MainCharInfo.CloseFrame( WINDOW_NEW_TAMING);
		}
		break;
	case taming_window_button:
		{
			TCHAR strPetName[128];
			memset(strPetName, 0, sizeof(strPetName));
			g_pUIManager->GetString(WINDOW_NEW_TAMING, taming_window_name_edit, strPetName);

			//sString szPetName = g_pUIManager->GetString(WINDOW_NEW_TAMING, taming_window_name_edit);

			sPetInfo* pPet = g_PetList.GetCurrentPet();
			if( pPet)
			{
				SendCS_NC_PETRENAME_REQ( g_MainCharInfo.m_dwMapID,
										pPet->dwID,
										strPetName);
			}
		}
		break;
	}
}

void ProcessWindowTamingItem( LPARAM lParam)
{
	int controlID = LOWORD( lParam);
	int eventType = HIWORD( lParam);

	switch( controlID)
	{
	case taming_item_window_close_button:
		g_MainCharInfo.HideSack( SACKTYPE__PET);
		break;
	}
}


// 靸侅爯 韺愲Г旮 鞛呺牓彀
void ProcessWindowMoney( LPARAM lParam)
{
	int controlID	= LOWORD(lParam);
	int nEventType	= HIWORD(lParam);

	switch( controlID)
	{
	case money_window_edit:
	case money_window_button_01:
		{
			__int64 nTemp = _tstoi64( (LPCTSTR) g_pUIManager->GetString(WINDOW_MONEY, money_window_edit));

			if(nTemp > 2100000000)  // 21鞏 鞚挫儊 鞛呺牓 攵堦皜
			{
				g_MainCharInfo.ShowHelpMessage( IDS_MANY_MONEY, TEXTEFFECT_COLOR_WARNING);

				if(nEventType == WINDOW_MONEY_PCTRADE)
				{
					g_pUIManager->SetPostMsg(WINDOW_MONEY_PCTRADE);
				}

				return;
			} // if( nTemp > 2100000000)

			DWORD dwAmount = static_cast<DWORD>(nTemp);

			if(dwAmount > 0)
			{
				if(nEventType == WINDOW_MONEY_PCTRADE)	// 臧滌澑雲胳爯
				{
					XiahItem::sItemInfo* pHoldItem = g_MainCharInfo.m_pHoldItem->GetHoldItemItem();

					if(pHoldItem)
					{
						// 臧滌爯韺 鞓毽旮 毵夑赴
						if(g_pUIManager->IsShow(WINDOW_PC_STORE) && g_MainCharInfo.m_pPersonalTradeSet && pHoldItem->m_wRefID != 20272)  
						{
							pHoldItem->m_dwPrice = dwAmount;  // 臧滌澑 韺愲Г 臧瓴

							// 臧滌澑雲胳爯 鞎勳澊韰 雴撽赴
							SendCS_SH_REGSHOP_REQ(g_MainCharInfo.m_byMySackCurrIdx+1,
												pHoldItem->m_bSackPos,
												pHoldItem->m_dwItemID,
												g_MainCharInfo.m_pHoldItem->m_bBackPosition,
												pHoldItem->m_dwPrice);
						}
					}

				} // if(nEventType == WINDOW_MONEY_PCTRADE)	// 臧滌澑雲胳爯
				else if(nEventType == WINDOW_MONEY_LOTTO)	// 氤店秾雼轨波旮 靾橂牴
				{
					SendCS_EC_GETLOTTOMONEY_REQ(g_MainCharInfo.m_dwResItemID,
												g_MainCharInfo.m_ReairSackID,
												g_MainCharInfo.m_RpairItemPos,
												dwAmount);

				}
			}
			else
			{
				g_MainCharInfo.ShowHelpMessage(IDS_WINDOW_MONEY, TEXTEFFECT_COLOR_WARNING);

				return;
			}

			if(nEventType == WINDOW_MONEY_PCTRADE)
				g_MainCharInfo.m_pHoldItem->SetDrawFlag(TRUE);

			g_MainCharInfo.CloseFrame(WINDOW_MONEY);
		}
		break;

	case money_window_button_02:
		{
			g_MainCharInfo.CloseFrame( WINDOW_MONEY);

			if(nEventType == WINDOW_MONEY_PCTRADE)
			{
				if( g_MainCharInfo.m_pHoldItem)
					g_MainCharInfo.m_pHoldItem->SetItemBackToSack();

				g_MainCharInfo.m_pHoldItem->SetDrawFlag(TRUE);
			}			
		}
		break;
	} // switch( controlID)

}



void ProcessWindowVolume( LPARAM lParam)
{
	int controlID = LOWORD( lParam);
	int eventType = HIWORD( lParam);

	DWORD dwAmount = 0;

	switch(controlID)
	{
	case volume_window_up:
		{
			dwAmount = _tstoi((LPCTSTR)g_pUIManager->GetString(WINDOW_VOLUME, volume_window_edit));

			++dwAmount;

			switch( g_MainCharInfo.m_byUsageVolumFrame)
			{
			case 1:
			case 3:	// 鞓奠厴 旮堨爠 靹れ爼鞙勴暣 於旉皜
				{
					g_pUIManager->SetFocus(WINDOW_VOLUME, volume_window_edit);
					g_pUIManager->SetString(WINDOW_VOLUME, volume_window_edit, dwAmount);
				}
				break;

			case 2:
				{
					if( dwAmount > 10)
					{
						g_MainCharInfo.m_dwVolumeSplitAmount = 10;
						g_MainCharInfo.ShowHelpMessage(IDS_CANNAOT_PICK,TEXTEFFECT_COLOR_WARNING);
					}
					else
					{
						g_pUIManager->SetFocus(WINDOW_VOLUME, volume_window_edit);
						g_pUIManager->SetString(WINDOW_VOLUME, volume_window_edit, dwAmount);
					}
				}
				break;
			}
		}
		break;
	case volume_window_down:
		{
			sString strTemp = g_pUIManager->GetString(WINDOW_VOLUME, volume_window_edit);

			if(_tcslen(strTemp) != 0)
			{
				dwAmount = _tstoi( (LPCTSTR)strTemp);
				--dwAmount;
			}

			switch( g_MainCharInfo.m_byUsageVolumFrame)
			{
			case 1:
			case 3:	// 鞓奠厴鞐
				{
					if(dwAmount <= 0)
					{
						g_MainCharInfo.ShowHelpMessage(IDS_PUT_AMOUNT, TEXTEFFECT_COLOR_WARNING);
						return;
					}
					else
					{
						g_pUIManager->SetFocus(WINDOW_VOLUME, volume_window_edit);
						g_pUIManager->SetString(WINDOW_VOLUME, volume_window_edit, dwAmount);
					}
				}
				break;
			case 2:
				{
					if( dwAmount <= 0)
					{
						g_MainCharInfo.ShowHelpMessage( IDS_PUT_AMOUNT, TEXTEFFECT_COLOR_WARNING);
						return;
					}
					else
					{
						g_pUIManager->SetFocus(WINDOW_VOLUME, volume_window_edit);
						g_pUIManager->SetString(WINDOW_VOLUME, volume_window_edit, dwAmount);
					}
				}
				break;
			}
		}
		break;
	case volume_window_edit:
	case volume_window_title_button_01:
		{
			dwAmount = _tstoi( (LPCTSTR) g_pUIManager->GetString(WINDOW_VOLUME, volume_window_edit));

			switch( g_MainCharInfo.m_byUsageVolumFrame)
			{
			case 1:	// 霃
				{
					if( dwAmount > 0)
					{
						XiahItem::sItemInfo* pHoldItem = g_MainCharInfo.m_pHoldItem->GetHoldItemItem();

						if( g_MainCharInfo.m_dwMoney >= dwAmount) //if( g_MainCharInfo.m_i64Money >= dwAmount)
						{
							TCHAR content[100] = {0,};
							_stprintf( content, IDS_D_PICK_MONEY, dwAmount);
							g_MainCharInfo.ShowHelpMessage( content);

							g_MainCharInfo.m_pHoldItem->SetHoldItemMoneySack( SACKTYPE__DEFAULT);
							g_MainCharInfo.m_pHoldItem->SetHoldItemMoney( dwAmount);
						}
						else
						{
							g_MainCharInfo.ShowHelpMessage( IDS_SHORT_MONEY, TEXTEFFECT_COLOR_WARNING);
						}
					}
					else
					{
						g_MainCharInfo.ShowHelpMessage( IDS_PUT_AMOUNT, TEXTEFFECT_COLOR_WARNING);
						return;
					}
				}
				break;
			case 2:	// 鞎勳澊韰
				{
					XiahItem::sItemInfo* pHoldItem = g_MainCharInfo.m_pHoldItem->GetHoldItemItem();
					if( pHoldItem)
					{
						//pHoldItem->m_dwAmount = dwAmount;
						//g_MainCharInfo.m_pHoldItem->SetItemBackToSack();
						if( dwAmount > 0)
						{
							if( dwAmount > 10)
							{
								g_MainCharInfo.m_dwVolumeSplitAmount = 10;
								g_MainCharInfo.ShowHelpMessage(IDS_CANNAOT_PICK,TEXTEFFECT_COLOR_WARNING);
							}
							else
								g_MainCharInfo.m_dwVolumeSplitAmount = dwAmount;
							// setholditem鞚 鞐旮办劀 頃橂姅瓴冹澊...
						}
						else
						{
							g_MainCharInfo.ShowHelpMessage( IDS_PUT_AMOUNT, TEXTEFFECT_COLOR_WARNING);
							return;	
						}
					}
				}
				break;
			case 3:	// 鞓奠厴 旮堨爠 靹れ爼
				{
					if(dwAmount >= 0)
					{
						if(dwAmount > 1000000000)
						{
							g_info_Temp.m_dwBuyLimit = 1000000000;

							TCHAR strMoney[32] = {0,};
							_stprintf(strMoney, IDS_MONEY, MoneyCommaStr(1000000000).data());

							g_pUIManager->SetString(WINDOW_OPTION_03, option_window_3_money_dummy, strMoney, 8);
						}
						else
						{
							g_info_Temp.m_dwBuyLimit = dwAmount;

							TCHAR strMoney[32] = {0,};
							_stprintf(strMoney, IDS_MONEY, MoneyCommaStr(dwAmount).data());

							g_pUIManager->SetString(WINDOW_OPTION_03, option_window_3_money_dummy, strMoney, 8);
						}
					}
					else
					{
						g_MainCharInfo.ShowHelpMessage( IDS_PUT_AMOUNT, TEXTEFFECT_COLOR_WARNING);
						return;
					}
				}
				break;
			}

			// hold 鞎勳澊韰 於滊牓
			if(g_MainCharInfo.m_pHoldItem)
				g_MainCharInfo.m_pHoldItem->SetDrawFlag(TRUE);

			g_MainCharInfo.CloseFrame( WINDOW_VOLUME);
		}
		break;
	case volume_window_title_button_02:
		{
			g_MainCharInfo.m_byUsageVolumFrame		= 0;
			g_MainCharInfo.m_dwVolumeSplitAmount	= 0;			

			if(g_MainCharInfo.m_pHoldItem)
			{
				g_MainCharInfo.m_pHoldItem->SetItemBackToSack();

				// hold 鞎勳澊韰 於滊牓
				g_MainCharInfo.m_pHoldItem->SetDrawFlag( TRUE);
			}

			g_MainCharInfo.CloseFrame( WINDOW_VOLUME);
		}		
		break;
	}
}

// 雼 鞝勴埇 (雼 牍勲)
void ProcessWindowDanWar(LPARAM lParam)
{
	int controlID = LOWORD( lParam);
	//int eventType = HIWORD( lParam);

	switch( controlID)
	{
	case window_dan_war_edit:
	case window_dan_war_button_01:
		{
			__int64 nTemp = _tstoi64((LPCTSTR)g_pUIManager->GetString(WINDOW_DAN_WAR, window_dan_war_edit));

			if( nTemp > 2100000000)  // 21鞏 鞚挫儊 鞛呺牓 攵堦皜
			{
				g_MainCharInfo.ShowHelpMessage(IDS_MANY_MONEY, TEXTEFFECT_COLOR_WARNING);
				return;
			} // if( nTemp > 2100000000)

			DWORD dwAmount = static_cast<DWORD>(nTemp);

			if(dwAmount > 0)
			{
				XiahObject::CXiahObject* pObject = XiahObject::g_XiahObjectManager.FindXiahObject( MAKEOBJECTID( 0, g_MainCharInfo.m_dwPickedObject, OBJTYPE_PC));

				if(pObject == NULL)
					return;

				if(pObject->m_pObject == NULL)
					return;

				CXiahCharObject* pCharObject = reinterpret_cast<CXiahCharObject*>( pObject->m_pObject);
				CXiahCharObject* pMainChar = reinterpret_cast<CXiahCharObject*>( g_pMainChar->m_pObject);			 

				SendCS_BT_ASKPARTYBATTLE_REQ(0, pMainChar->m_dwPartyID, pMainChar->m_dwPartyLeaderID, pCharObject->m_dwPartyID, pCharObject->m_dwPartyLeaderID, dwAmount);
			}
			else
			{
				g_MainCharInfo.ShowHelpMessage(IDS_DANWAR_MONEY, TEXTEFFECT_COLOR_WARNING);
				return;
			}
			
			g_MainCharInfo.CloseFrame(WINDOW_DAN_WAR);
		}
		break;
	case window_dan_war_button_02:
		{
			g_MainCharInfo.CloseFrame(WINDOW_DAN_WAR);
		}
		break;
	}

	g_pUIManager->SetString(WINDOW_DAN_WAR, window_dan_war_edit, 0);
}

void ProcessWindowConnectionInfo( LPARAM lParam)
{
	int controlID = LOWORD( lParam);
	int eventType = HIWORD( lParam);

	switch( controlID)
	{
	case info_window_close_button:
	case info_window_button:
		g_MainCharInfo.CloseFrame( WINDOW_CONNECTION_INFO);
		break;
	case info_window_button_01:
		{
			g_MainCharInfo.OpenFrame( NAME_CHANNEL);

			sRect rtRect;
			g_pUIManager->GetRegionData(WINDOW_CONNECTION_INFO, info_window_button_01, rtRect);
			g_pUIManager->SetPosition(NAME_CHANNEL, rtRect.left, rtRect.bottom);
		}
		break;
	case info_window_button_02:
		{
			DWORD dwMunwonID = g_MainCharInfo.m_pRelation->GetCurrRelation();
			sClanWonInfo* pInfo = g_MainCharInfo.m_pRelation->FindClanInfoByID( dwMunwonID);
			if( pInfo)
			{
				DWORD dwOrderID = pInfo->m_dwOrderID;
				SendCS_RL_CHANGEMUNWONORDER_REQ( dwMunwonID, dwOrderID, 6);
			}
		}
		break;
	}
}

/**
 * 氍疙寣 順胳弓 靾橃棳
 * \param lParam 
 */
void ProcessWindowNameConfer( LPARAM lParam)
{
	int controlID = LOWORD( lParam);
	//int eventType = HIWORD( lParam);

	switch( controlID)
	{
	case name_window_edit:
	case name_window_button_01:
		{
			// [3/09/2005] 頃勴劙毵
			TCHAR strName[64] = {0,};
			memset(strName, 0, sizeof(strName));
			g_pUIManager->GetString(WINDOW_NAME_CONFER, name_window_edit, strName);

			sString szNickName;			
			
			szNickName.printf("%s", ConvertString(strName, 64));

			DWORD dwMunpaID = g_MainCharInfo.m_pRelation->GetClanID();
			DWORD dwCharID  = g_MainCharInfo.m_pRelation->GetCurrRelation();

			SendCS_RL_MUNPANICK_REQ(dwMunpaID, dwCharID, szNickName);
			
			g_MainCharInfo.CloseFrame(WINDOW_NAME_CONFER);
		}
		break;
	case name_window_button_02:
	case name_wiandow_close_button:
		{
			g_MainCharInfo.CloseFrame( WINDOW_NAME_CONFER);
		}		
		break;
	}
}

/**
 * 歆侅渼攵鞐
 * \param lParam 
 */
void ProcessNameChannel( LPARAM lParam)
{
	int controlID = LOWORD( lParam);
	//int eventType = HIWORD( lParam);

	DWORD dwMunwonID = g_MainCharInfo.m_pRelation->GetCurrRelation();
	DWORD dwOrderID;
	sClanWonInfo* pInfo = g_MainCharInfo.m_pRelation->FindClanInfoByID( dwMunwonID);

	if( pInfo)
		dwOrderID = pInfo->m_dwOrderID;
	else
		return;

	switch( controlID)
	{
	case name_channel_button_01:	// 氍胳＜
		{
			if(g_MainCharInfo.m_pRelation->Am_I_InClan())
			{
				if(g_MainCharInfo.m_pRelation->Am_I_LeaderInClan())
				{
					// 氍胳＜鞚挫枒
					g_pUIManager->ShowNotice(IDS_MUNJU_RELINQUISH, NOTICE_FRAME_OKCANCEL, NOTICE_FRAME_RELINQUISH);

					/*
					if(g_MainCharInfo.m_dwMunpaFame >= 100)		// 氍疙寣氇呾劚 100鞚挫儊
					{						
					}
					else
					{
						g_MainCharInfo.ShowHelpMessage(IDS_MUNPA_FAME_LOW, TEXTEFFECT_COLOR_WARNING);
					}
					*/
				} // if(g_MainCharInfo.m_pRelation->Am_I_LeaderInClan())
				else
				{
					SendCS_RL_CHANGEMUNWONORDER_REQ(dwMunwonID, dwOrderID, 1);
				}							
			}
		}		
		break;
	case name_channel_button_02:	// 攵氍胳＜
		SendCS_RL_CHANGEMUNWONORDER_REQ( dwMunwonID, dwOrderID, 2);
		break;
	case name_channel_button_03:	// 鞛ル
		SendCS_RL_CHANGEMUNWONORDER_REQ( dwMunwonID, dwOrderID, 3);
		break;
	case name_channel_button_04:	// 順鸽矔
		SendCS_RL_CHANGEMUNWONORDER_REQ( dwMunwonID, dwOrderID, 4);
		break;
	case name_channel_button_05:	// 雼轨＜
		SendCS_RL_CHANGEMUNWONORDER_REQ( dwMunwonID, dwOrderID, 5);
		break;
	}

	g_MainCharInfo.CloseFrame( NAME_CHANNEL);
}






//////////////////////////////////////////////////
// Mugong
//////////////////////////////////////////////////
/**
 * 鞕戈车
 * \param lParam 
 */
void ProcessWindowOutSide( LPARAM lParam)
{
	int controlID = LOWORD( lParam);
	//int eventType = HIWORD( lParam);

	//HT_1212 : 雮搓车,鞕戈车 彀届棎 韮鞚错媭 彀 靾橃爼
	//g_pUIManager->Show(DATA_WINDOW);  //HO_0403_07氍搓车彀 韮鞚错媭氚 韥措Ν鞁 雮犾滍憸鞁滊 氚旊岇柎靹 鞎堧皵雬岅矊 靾橃爼鞚 鞙勴暅 欤检劃觳橂Μ
	switch( controlID)
	{
	case outside_window_close_button:
		g_MainCharInfo.CloseFrame( WINDOW_OUTSIDE);

		if(g_pUIManager->IsShow(WINDOW_CHARACTER))
		{
			g_pUIManager->SetPosition(WINDOW_CHARACTER, WINDOW_FIRST_XPOS, 0);
		}
		break;
	case outside_window_top_button_01:
		break;
	case outside_window_top_button_02:
		ProcessClickMugongButton(2);
		break;
	case outside_window_top_button_03:
		ProcessClickMugongButton(3);
		break;
	case outside_window_top_button_04:		// 臧侅劚
		ProcessClickMugongButton(4);
		break;
	case outside_attack_mode_button_01:		// 氍搓车瓿店博 氤错樃雽靸
	case outside_attack_mode_button_02:
	case outside_attack_mode_button_03:
		{
			g_pUIManager->SetData(WINDOW_OUTSIDE, outside_attack_mode_button_01, CURRENT_INDEX, -1);
			g_pUIManager->SetData(WINDOW_OUTSIDE, outside_attack_mode_button_02, CURRENT_INDEX, -1);
			g_pUIManager->SetData(WINDOW_OUTSIDE, outside_attack_mode_button_03, CURRENT_INDEX, -1);

			g_pUIManager->SetData(WINDOW_OUTSIDE, controlID, CURRENT_INDEX, 2);

			g_info.m_bSafeMode = (controlID - outside_attack_mode_button_01);

			Save_Option(true);
		}
		break;
	//HO_0403_07 雮搓车, 鞕戈车彀 頂勲爤鞛 韥措Ν鞁 攵堩晞鞖旐暅 氅旍劯歆臧 霚雿橂秬攵 靾橃爼
	case outside_window_mugong_point_up_01:
	case outside_window_mugong_point_up_02:
	case outside_window_mugong_point_up_03:
	case outside_window_mugong_point_up_04:
	case outside_window_mugong_point_up_05:
	case outside_window_mugong_point_up_06:
	case outside_window_mugong_point_up_07:
	case outside_window_mugong_point_up_08:
	case outside_window_mugong_point_up_09:
	case outside_window_mugong_point_up_10:
	case outside_window_mugong_point_up_11:
	case outside_window_mugong_point_up_12:
		{
		int nMugongID = g_MainCharInfo.m_pMugong->FindMugongByIndex(  MUGONGTYPE_ACTIVE, controlID - outside_window_mugong_point_up_01 + 1);

			if(nMugongID)
			{
				g_MainCharInfo.PlayInterfaceSound( ISOUND_SELECT_BUTTON);
				SendCS_BT_LEARNMUGONG_REQ( nMugongID);
			}
			else
				g_MainCharInfo.ShowHelpMessage( IDS_NO_MUGONG, TEXTEFFECT_COLOR_WARNING);
		}
		break;

	default:			
		break;

		/*HO_0403_07 靾橃爼鞝 case outside_attack_mode_button_03:雼れ潓鞐 鞊办榾雿 氍戈惮
	default:
		{
			int nMugongID = g_MainCharInfo.m_pMugong->FindMugongByIndex(  MUGONGTYPE_ACTIVE, controlID - outside_window_mugong_point_up_01 + 1);

			if(nMugongID)
			{
				g_MainCharInfo.PlayInterfaceSound( ISOUND_SELECT_BUTTON);
				SendCS_BT_LEARNMUGONG_REQ( nMugongID);
			}
			else
				g_MainCharInfo.ShowHelpMessage( IDS_NO_MUGONG, TEXTEFFECT_COLOR_WARNING);
		}	
		break;
		*/
	}	
}

/**
 * 雮搓车
 * \param lParam 
 */
void ProcessWindowInSide( LPARAM lParam)
{
	int controlID = LOWORD( lParam);
	//int eventType = HIWORD( lParam);

	//HT_1212 : 雮搓车,鞕戈车 彀届棎 韮鞚错媭 彀 靾橃爼
	//g_pUIManager->Show(DATA_WINDOW);  //HO_0403_07氍搓车彀 韮鞚错媭氚 韥措Ν鞁 雮犾滍憸鞁滊 氚旊岇柎靹 鞎堧皵雬岅矊 靾橃爼鞚 鞙勴暅 欤检劃觳橂Μ
	switch( controlID)
	{
	case inside_window_close_button:
		g_MainCharInfo.CloseFrame( WINDOW_INSIDE);
		break;
	case inside_window_top_button_01:
		ProcessClickMugongButton(1);
		break;
	case inside_window_top_button_02:
		break;
	case inside_window_top_button_03:
		ProcessClickMugongButton(3);
		break;
	case inside_window_top_button_04:	// 臧侅劚
		ProcessClickMugongButton(4);
		break;
	
	//HO_0403_07 雮搓车, 鞕戈车彀 頂勲爤鞛 韥措Ν鞁 攵堩晞鞖旐暅 氅旍劯歆臧 霚雿橂秬攵 靾橃爼
	case inside_window_mugong_point_up_01:
	case inside_window_mugong_point_up_02:
	case inside_window_mugong_point_up_03:
	case inside_window_mugong_point_up_04:
	case inside_window_mugong_point_up_05:
	case inside_window_mugong_point_up_06:
	case inside_window_mugong_point_up_07:
	case inside_window_mugong_point_up_08:
	case inside_window_mugong_point_up_09:
		{
			int nMugongID = g_MainCharInfo.m_pMugong->FindMugongByIndex(  MUGONGTYPE_PASSIVE, controlID - inside_window_mugong_point_up_01 + 1);

			if(nMugongID)
			{
				g_MainCharInfo.PlayInterfaceSound( ISOUND_SELECT_BUTTON);
				SendCS_BT_LEARNMUGONG_REQ( nMugongID);
			}
			else
				g_MainCharInfo.ShowHelpMessage( IDS_NO_MUGONG, TEXTEFFECT_COLOR_WARNING);
		}
		break;
	case inside_window_mugong_point_up_10://HO_0709_07 : 歆勱皝靹 雮搓车 頂勲爤鞛 : +氩勴娂 雸岆煬歆
	case inside_window_mugong_point_up_11:
		{
			int nMugongID = g_MainCharInfo.m_pMugong->Find2ThRebirthMugongByIndex( controlID - inside_window_mugong_point_up_10 + 3 );

			if(nMugongID)
			{
				g_MainCharInfo.PlayInterfaceSound( ISOUND_SELECT_BUTTON);
				SendCS_BT_LEARNMUGONG_REQ( nMugongID);
			}
			else
				g_MainCharInfo.ShowHelpMessage( IDS_NO_MUGONG, TEXTEFFECT_COLOR_WARNING);
		}
		break;

	default:		
		break;
		
		/*HO_0403_07 靾橃爼鞝 case outside_attack_mode_button_03:雼れ潓鞐 鞊办榾雿 氍戈惮
	default:
		{
			int nMugongID = g_MainCharInfo.m_pMugong->FindMugongByIndex(  MUGONGTYPE_PASSIVE, controlID - inside_window_mugong_point_up_01 + 1);

			if(nMugongID)
			{
				g_MainCharInfo.PlayInterfaceSound( ISOUND_SELECT_BUTTON);
				SendCS_BT_LEARNMUGONG_REQ( nMugongID);
			}
			else
				g_MainCharInfo.ShowHelpMessage( IDS_NO_MUGONG, TEXTEFFECT_COLOR_WARNING);
		}	
		break;
		*/
	}
}















//////////////////////////////////////////////////
// Relation
//////////////////////////////////////////////////
void ProcessWindowDan( LPARAM lParam)
{
	int controlID = LOWORD( lParam);
	int eventType = HIWORD( lParam);

	g_MainCharInfo.PlayInterfaceSound( ISOUND_SELECT_BUTTON);

	switch( controlID)
	{
	case dan_window_close_button:
		g_MainCharInfo.CloseFrame( WINDOW_DAN);
		break;
	case dan_window_3button_01:	// 雼
		g_MainCharInfo.m_pRelation->SetCurrType( eDAN);
		break;
	case dan_window_3button_02:	// 鞚胳棸
		g_MainCharInfo.m_pRelation->SetCurrType( eShip);
		break;
	case dan_window_3button_03:	// 氍疙寣
		g_MainCharInfo.m_pRelation->SetCurrType( eClan);
		break;
	case dan_window_2button_01:
		{
			switch( g_MainCharInfo.m_pRelation->GetCurrType())
			{
			case eDAN:	// 鞝滊獏
				g_pUIManager->ShowNotice( IDS_Q_JEMYUNG, NOTICE_FRAME_OKCANCEL, NOTICE_FRAME_JEMYUNG);
				break;
			case eShip:	// 鞝勳劀甑
				g_MainCharInfo.ShowHelpMessage( IDS_NO_SUPPORT, TEXTEFFECT_COLOR_WARNING);
				break;
			case eClan:
				break;
			}
		}
		break;
	case dan_window_2button_02:
	case dan_window_1button:
		{
			switch( g_MainCharInfo.m_pRelation->GetCurrType())
			{
			case eDAN:	// 韮堩嚧
				SendCS_IF_LEAVEPARTY_REQ( g_MainCharInfo.m_pRelation->GetDanID());
				break;
			case eShip:	// 鞚胳棸雭婈赴
				{
					DWORD dwAnsCharID = g_MainCharInfo.m_pRelation->GetCurrRelation();

					sShipInfo* pShipInfo = g_MainCharInfo.m_pRelation->FindShipInfoByID( dwAnsCharID );
					if( pShipInfo == NULL ) break;

					SendCS_RL_BREAKRELATION_REQ( pShipInfo->m_bShipType, RELATION_STEP_ASK, g_MainCharInfo.m_dwObjectID, dwAnsCharID );
					g_MainCharInfo.ShowHelpMessage( ( IDS_REL_SEND_ASK_BREAK ) );
//					SendCS_IF_DELBUDDY_REQ( g_MainCharInfo.m_pRelation->GetCurrRelation());
				}
				break;
			case eClan:
				break;
			}
		}
		break;
	case dan_window_list_button_left:
		{
			if( g_MainCharInfo.m_pRelation)
			{
				eRELATION_TYPE eCurrType = g_MainCharInfo.m_pRelation->GetCurrType();
				BYTE byPrePage = g_MainCharInfo.m_pRelation->GetCurrPage() - 1;
				g_MainCharInfo.m_pRelation->SetCurrType( eCurrType, byPrePage);
			}
		}
		break;
	case dan_window_list_button_right:
		{
			if( g_MainCharInfo.m_pRelation)
			{
				eRELATION_TYPE eCurrType = g_MainCharInfo.m_pRelation->GetCurrType();
				BYTE byNextPage = g_MainCharInfo.m_pRelation->GetCurrPage() + 1;
				g_MainCharInfo.m_pRelation->SetCurrType( eCurrType, byNextPage);
			}
		}
		break;
	}
}

/**
 * 氍疙寣彀
 * \param lParam 
 */
void ProcessWindowClan( LPARAM lParam)
{
	int controlID = LOWORD( lParam);
	//int eventType = HIWORD( lParam);

	switch( controlID)
	{
	case munpa_window_close_button:
		g_MainCharInfo.CloseFrame( WINDOW_MUNPA);
		break;
	case munpa_window_3button_01:	// 雼
		g_MainCharInfo.PlayInterfaceSound( ISOUND_SELECT_BUTTON);
		g_MainCharInfo.m_pRelation->SetCurrType( eDAN);
		break;
	case munpa_window_3button_02:	// 鞚胳棸
		g_MainCharInfo.PlayInterfaceSound( ISOUND_SELECT_BUTTON);
		g_MainCharInfo.m_pRelation->SetCurrType( eShip);
		break;
	case munpa_window_3button_03:	// 氍疙寣
		g_MainCharInfo.PlayInterfaceSound( ISOUND_SELECT_BUTTON);
		g_MainCharInfo.m_pRelation->SetCurrType( eClan);
		break;

	case munpa_window_button_01:	// 氍疙寣瓿奠
		{
			g_pUIManager->SetString(GAK_MESSAGE_WINDOW, gak_title_dummy, IDS_M_NOTICE_RECORD);

			g_MainCharInfo.OpenFrame(GAK_MESSAGE_WINDOW);
			g_pUIManager->SetPostMsg(GAK_MSG_WINDOW_MUNPA);

			g_pUIManager->SetString(GAK_MESSAGE_WINDOW, gak_message_edit, _T(""));
			g_pUIManager->SetFocus(GAK_MESSAGE_WINDOW);
			g_pUIManager->SetFocus(GAK_MESSAGE_WINDOW, gak_message_edit);
		}
		break;
	case munpa_window_button_02:	// 順胳弓靾橃棳
		{
			DWORD id = g_MainCharInfo.m_pRelation->GetCurrRelation();
			sClanWonInfo* pInfo = g_MainCharInfo.m_pRelation->FindClanInfoByID( id);

			if(pInfo)
			{
				TCHAR szContent[100] = {0,};
				_stprintf( szContent, IDS_D_INSERT_HOCHING, (LPCTSTR)pInfo->m_szCharName);
				g_pUIManager->SetString(WINDOW_NAME_CONFER, name_wiandow_contents_dummy_01, szContent);
			} // if(pInfo)

			g_MainCharInfo.OpenFrame( WINDOW_NAME_CONFER);
		}
		break;
	case munpa_window_button_04:	// 氍胳＜氅 氍疙寣鞐嗢暊旮
		{
			if(g_MainCharInfo.m_pRelation->Am_I_2stLeaderInClan()) // 攵氍胳＜鞖 韺岆
			{
				DWORD id = g_MainCharInfo.m_pRelation->GetCurrRelation();
				sClanWonInfo* pInfo = g_MainCharInfo.m_pRelation->FindClanInfoByID( id);

				if(pInfo)
				{
					TCHAR szContent[100] = {0,};
					_stprintf( szContent, IDS_Q_PAMUN, (LPCTSTR)pInfo->m_szCharName);

					g_pUIManager->ShowNotice( szContent, NOTICE_FRAME_OKCANCEL, NOTICE_FRAME_PAMUN);
				}
			}
			else // 氍胳＜電 韽愳噭
			{
				g_pUIManager->ShowNotice( IDS_Q_CLOSE_CLAN, NOTICE_FRAME_OKCANCEL, NOTICE_FRAME_CLOSE_CLAN);
			}
		}		
		break;
	case munpa_window_button_03:
		{
			if( g_MainCharInfo.m_pRelation->Am_I_LeaderInClan())	// 韺岆胳嫓韨り赴
			{
				DWORD id = g_MainCharInfo.m_pRelation->GetCurrRelation();
				sClanWonInfo* pInfo = g_MainCharInfo.m_pRelation->FindClanInfoByID( id);

				if(pInfo)
				{
					TCHAR szContent[100] = {0,};
					_stprintf( szContent, IDS_Q_PAMUN, (LPCTSTR)pInfo->m_szCharName);

					g_pUIManager->ShowNotice( szContent, NOTICE_FRAME_OKCANCEL, NOTICE_FRAME_PAMUN);
				}
			}
			else if( g_MainCharInfo.m_pRelation->Am_I_InClan())		// 韮堩嚧頃橁赴
			{
				sClanWonInfo* pClan = g_MainCharInfo.m_pRelation->FindClanInfoByID( g_MainCharInfo.m_dwObjectID);

				if(pClan)
				{
					g_pUIManager->ShowNotice(IDS_MUNPA_LEAVE, NOTICE_FRAME_OKCANCEL, NOTICE_FRAME_LEAVE);
				}
			}
		}
		break;
	case munpa_window_list_button_left:
		{
			if( g_MainCharInfo.m_pRelation)
			{
				eRELATION_TYPE eCurrType = g_MainCharInfo.m_pRelation->GetCurrType();
				BYTE byPrePage = g_MainCharInfo.m_pRelation->GetCurrPage() - 1;
				g_MainCharInfo.m_pRelation->SetCurrType( eCurrType, byPrePage);
			}
		}
		break;
	case munpa_window_list_button_right:
		{
			if( g_MainCharInfo.m_pRelation)
			{
				eRELATION_TYPE eCurrType = g_MainCharInfo.m_pRelation->GetCurrType();
				BYTE byNextPage = g_MainCharInfo.m_pRelation->GetCurrPage() + 1;
				g_MainCharInfo.m_pRelation->SetCurrType( eCurrType, byNextPage);
			}
		}
		break;
	}
}

void ProcessWindowClanFound( LPARAM lParam)
{
	int controlID = LOWORD( lParam);
	int eventType = HIWORD( lParam);

	switch( controlID)
	{
	case found_window_close_button:
		g_MainCharInfo.CloseFrame( WINDOW_MUNPA_FOUND);
		break;
	case found_window_3button_01:	// 雼
		g_MainCharInfo.PlayInterfaceSound( ISOUND_SELECT_BUTTON);
		g_MainCharInfo.m_pRelation->SetCurrType( eDAN);
		break;
	case found_window_3button_02:	// 鞚胳棸
		g_MainCharInfo.PlayInterfaceSound( ISOUND_SELECT_BUTTON);
		g_MainCharInfo.m_pRelation->SetCurrType( eShip);
		break;
	case found_window_3button_03:	// 氍疙寣
		g_MainCharInfo.PlayInterfaceSound( ISOUND_SELECT_BUTTON);
		g_MainCharInfo.m_pRelation->SetCurrType( eClan);
		break;
	case found_window_found_button:
		{
			sString szName = _T("");
			//szName = g_pUIManager->GetString(WINDOW_MUNPA_FOUND, found_window_name_edit);
			TCHAR strName[128];
			memset(strName, 0, sizeof(strName));
			g_pUIManager->GetString(WINDOW_MUNPA_FOUND, found_window_name_edit, strName);

			szName = strName;

			if( szName != _T(""))
			{
				g_pUIManager->ShowNotice( IDS_Q_CLAN_FOUNT, NOTICE_FRAME_OKCANCEL, NOTICE_FRAME_FOUNT);
			}
			else
				g_MainCharInfo.ShowHelpMessage( IDS_INSERT_CLAN_NAME, TEXTEFFECT_COLOR_WARNING);		
		}
		break;
	}
}


// [3/25/2004] 韤橃姢韸 觳橂Μ
void ProcessWindowQuest( LPARAM lParam)
{
	int controlID = LOWORD( lParam);
	int eventType = HIWORD( lParam);

	switch( controlID)
	{
	case quest_window_01_close_button:
		g_MainCharInfo.CloseFrame( WINDOW_QUEST_01);
		break;
	case quest_window_01_start_button: // 鞁滌瀾
		if( g_MainCharInfo.m_pQuest->GetCurrQuestID())
			SendCS_QS_START_REQ( g_MainCharInfo.m_pQuest->GetCurrQuestID());
		break;
	case quest_window_01_stop_button: // 欷戩
		if( g_MainCharInfo.m_pQuest->GetCurrQuestID())
			SendCS_QS_STOP_REQ( g_MainCharInfo.m_pQuest->GetCurrQuestID());
		break;
	case quest_window_01_delete_button: // 靷鞝
		if( g_MainCharInfo.m_pQuest->GetCurrQuestID())
		{
			g_pUIManager->ShowNotice( IDS_FRAME_QUEST_DEL, NOTICE_FRAME_OKCANCEL, NOTICE_FRAME_QUEST_DEL);			
		}
		break;
			//ho_test
	case quest_window_01_button_01:
		g_pUIManager->SetData(WINDOW_QUEST_01, quest_window_01_button_01, CURRENT_INDEX, 2);
		g_pUIManager->SetData(WINDOW_QUEST_01, quest_window_01_button_02, CURRENT_INDEX, -1);
		g_pUIManager->SetData(WINDOW_QUEST_01, quest_window_01_button_03, CURRENT_INDEX, -1);
		g_pUIManager->SetData(WINDOW_QUEST_01, quest_window_01_button_04, CURRENT_INDEX, -1);

		g_MainCharInfo.m_pQuest->m_wCurrQuestIndex = 0;
		g_MainCharInfo.m_pQuest->UpdateQuest();
		break;
	case quest_window_01_button_02:
		g_pUIManager->SetData(WINDOW_QUEST_01, quest_window_01_button_01, CURRENT_INDEX, -1);
		g_pUIManager->SetData(WINDOW_QUEST_01, quest_window_01_button_02, CURRENT_INDEX, 2);
		g_pUIManager->SetData(WINDOW_QUEST_01, quest_window_01_button_03, CURRENT_INDEX, -1);
		g_pUIManager->SetData(WINDOW_QUEST_01, quest_window_01_button_04, CURRENT_INDEX, -1);

		g_MainCharInfo.m_pQuest->m_wCurrQuestIndex = 0;
		g_MainCharInfo.m_pQuest->ProgressUpdateQuest();
		break;

	case quest_window_01_button_03:
		g_pUIManager->SetData(WINDOW_QUEST_01, quest_window_01_button_01, CURRENT_INDEX, -1);
		g_pUIManager->SetData(WINDOW_QUEST_01, quest_window_01_button_02, CURRENT_INDEX, -1);
		g_pUIManager->SetData(WINDOW_QUEST_01, quest_window_01_button_03, CURRENT_INDEX, 2);
		g_pUIManager->SetData(WINDOW_QUEST_01, quest_window_01_button_04, CURRENT_INDEX, -1);

		g_MainCharInfo.m_pQuest->m_wCurrQuestIndex = 0;
		g_MainCharInfo.m_pQuest->NewUpdateQuest();
		break;
		
	case quest_window_01_button_04:
		g_pUIManager->SetData(WINDOW_QUEST_01, quest_window_01_button_01, CURRENT_INDEX, -1);
		g_pUIManager->SetData(WINDOW_QUEST_01, quest_window_01_button_02, CURRENT_INDEX, -1);
		g_pUIManager->SetData(WINDOW_QUEST_01, quest_window_01_button_03, CURRENT_INDEX, -1);
		g_pUIManager->SetData(WINDOW_QUEST_01, quest_window_01_button_04, CURRENT_INDEX, 2);

		g_MainCharInfo.m_pQuest->m_wCurrQuestIndex = 0;
		g_MainCharInfo.m_pQuest->CompleteUpdateQuest();
		break;

	case quest_window_01_button_05:
		g_pUIManager->SetData(WINDOW_QUEST_01, quest_window_01_button_05, CURRENT_INDEX, 2);
		g_pUIManager->SetData(WINDOW_QUEST_01, quest_window_01_button_06, CURRENT_INDEX, -1);
		g_pUIManager->SetData(WINDOW_QUEST_01, quest_window_01_button_07, CURRENT_INDEX, -1);

		g_MainCharInfo.m_pQuest->SetCurrContent( eContent);
		break;
	case quest_window_01_button_06:
		g_pUIManager->SetData(WINDOW_QUEST_01, quest_window_01_button_05, CURRENT_INDEX, -1);
		g_pUIManager->SetData(WINDOW_QUEST_01, quest_window_01_button_06, CURRENT_INDEX, 2);
		g_pUIManager->SetData(WINDOW_QUEST_01, quest_window_01_button_07, CURRENT_INDEX, -1);

		g_MainCharInfo.m_pQuest->SetCurrContent( eCondition);
		break;
	case quest_window_01_button_07:
		g_pUIManager->SetData(WINDOW_QUEST_01, quest_window_01_button_05, CURRENT_INDEX, -1);
		g_pUIManager->SetData(WINDOW_QUEST_01, quest_window_01_button_06, CURRENT_INDEX, -1);
		g_pUIManager->SetData(WINDOW_QUEST_01, quest_window_01_button_07, CURRENT_INDEX, 2);

		g_MainCharInfo.m_pQuest->SetCurrContent( eReward);
		break;
	case quest_window_01_toggle_up_01:
		{
			g_pUIManager->SetData(WINDOW_QUEST_01, quest_window_01_scrollbar1, SCROLL_ADD, -1);
			
			g_MainCharInfo.m_pQuest->RefreshQuestIndex();
		}
		break;
	case quest_window_01_toggle_down_01:
		{
			g_pUIManager->SetData(WINDOW_QUEST_01, quest_window_01_scrollbar1, SCROLL_ADD, 1);
			
			g_MainCharInfo.m_pQuest->RefreshQuestIndex();
		}		
		break;
	case qeust_window_01_toggle_up_02:
		{
			g_pUIManager->SetData(WINDOW_QUEST_01, quest_window_01_scrollbar2, SCROLL_ADD, -1);
			
			g_MainCharInfo.m_pQuest->RefreshQuestContent();
		}
		break;
	case quest_window_01_toggle_down_02:
		{
			g_pUIManager->SetData(WINDOW_QUEST_01, quest_window_01_scrollbar2, SCROLL_ADD, 1);
			
			g_MainCharInfo.m_pQuest->RefreshQuestContent();
		}
		break;
		// temp
	case quest_window_01_scrollbar1:
		g_MainCharInfo.m_pQuest->Refresh();
		break;
	case quest_window_01_scrollbar2:
		g_MainCharInfo.m_pQuest->RefreshQuestContent();
		break;
	}
}


/**
 * 瓴岇瀯 鞓奠厴
 * \param lParam 
 */
void ProcessWindowOption1( LPARAM lParam)
{
	int controlID = LOWORD( lParam);
	int eventType = HIWORD( lParam);
	
	switch( controlID)
	{
	case option_window_1_close_button:		// 雼旮
	case option_window_1_bottom_button_02:
		{
			g_MainCharInfo.CloseFrame( WINDOW_OPTION_01);
		}		
		break;
	case option_window_1_top_button_01:
		{
			/*
			g_pUIManager->SetData(WINDOW_OPTION_01, option_window_1_top_button_01, CURRENT_INDEX, 2);
			g_pUIManager->SetData(WINDOW_OPTION_01, option_window_1_top_button_02, CURRENT_INDEX, -1);
			g_pUIManager->SetData(WINDOW_OPTION_01, option_window_1_top_button_03, CURRENT_INDEX, -1);
			*/
		}
		break;
	case option_window_1_top_button_02:
		{
			g_MainCharInfo.CloseFrame( WINDOW_OPTION_01);

			g_pUIManager->SetData(WINDOW_OPTION_02, option_window_2_top_button_01, CURRENT_INDEX, -1);
			g_pUIManager->SetData(WINDOW_OPTION_02, option_window_2_top_button_02, CURRENT_INDEX, 2);
			g_pUIManager->SetData(WINDOW_OPTION_02, option_window_2_top_button_03, CURRENT_INDEX, -1);

			g_MainCharInfo.OpenFrame( WINDOW_OPTION_02);
		}
		break;
	case option_window_1_top_button_03:
		{
			// 鞓奠厴 氤瓴
			g_MainCharInfo.CloseFrame( WINDOW_OPTION_01);

			g_pUIManager->SetData(WINDOW_OPTION_03, option_window_3_top_button_01, CURRENT_INDEX, -1);
			g_pUIManager->SetData(WINDOW_OPTION_03, option_window_3_top_button_02, CURRENT_INDEX, -1);
			g_pUIManager->SetData(WINDOW_OPTION_03, option_window_3_top_button_03, CURRENT_INDEX, 2);

			g_MainCharInfo.OpenFrame(WINDOW_OPTION_03);
		}
		break;
	case option_window_1_bottom_button_01:
		{
			bool bSend = false;
			if(g_info.m_bAllowWhisper != g_info_Temp.m_bAllowWhisper ||
				g_info.m_bAllowRelation != g_info_Temp.m_bAllowRelation ||
				g_info.m_bAllowTrade != g_info_Temp.m_bAllowTrade ||
				g_info.m_dwBuyLimit != g_info_Temp.m_dwBuyLimit ||
				g_info.m_bRarityLimit != g_info_Temp.m_bRarityLimit ||
				g_info.m_bStxTypeLimit != g_info_Temp.m_bStxTypeLimit ||
				g_info.m_bItemDropChoice != g_info_Temp.m_bItemDropChoice)
			{
				bSend = true;
			}

			g_info = g_info_Temp;
			Save_Option(bSend);

			g_MainCharInfo.CloseFrame( WINDOW_OPTION_01);
		}
		break;
	}
}

/**
 * 頇橁步 鞓奠厴
 * \param lParam 
 */
void ProcessWindowOption2( LPARAM lParam)
{
	int controlID = LOWORD( lParam);
	int eventType = HIWORD( lParam);

	switch( controlID)
	{
	case option_window_2_close_button:		// 雼旮
	case option_window_2_bottom_button_02:
		{
			g_MainCharInfo.CloseFrame(WINDOW_OPTION_02);
		}		
		break;
	case option_window_2_top_button_01:
		{		
			g_MainCharInfo.CloseFrame( WINDOW_OPTION_02);

			g_pUIManager->SetData(WINDOW_OPTION_01, option_window_1_top_button_01, CURRENT_INDEX, 2);
			g_pUIManager->SetData(WINDOW_OPTION_01, option_window_1_top_button_02, CURRENT_INDEX, -1);
			g_pUIManager->SetData(WINDOW_OPTION_01, option_window_1_top_button_03, CURRENT_INDEX, -1);

			g_MainCharInfo.OpenFrame( WINDOW_OPTION_01);
		}
		break;
	case option_window_2_top_button_02:
		{
			/*
			if(g_pUIManager->GetData(WINDOW_OPTION_02, option_window_2_top_button_02, GET_CURRENT_INDEX) != 2)
			{
				g_MainCharInfo.CloseFrame( WINDOW_OPTION_02);				

				g_pUIManager->SetData(WINDOW_OPTION_02, option_window_2_top_button_01, CURRENT_INDEX, -1);
				g_pUIManager->SetData(WINDOW_OPTION_02, option_window_2_top_button_02, CURRENT_INDEX, 2);
				g_pUIManager->SetData(WINDOW_OPTION_02, option_window_2_top_button_03, CURRENT_INDEX, -1);

				g_MainCharInfo.OpenFrame( WINDOW_OPTION_02);
			}
			*/
		}
		break;
	case option_window_2_top_button_03:
		{
			// 鞓奠厴 氤瓴

			//if(g_pUIManager->GetData(WINDOW_OPTION_02, option_window_2_top_button_03, GET_CURRENT_INDEX) != 2)
			{
				g_MainCharInfo.CloseFrame(WINDOW_OPTION_02);		

				g_pUIManager->SetData(WINDOW_OPTION_03, option_window_3_top_button_01, CURRENT_INDEX, -1);
				g_pUIManager->SetData(WINDOW_OPTION_03, option_window_3_top_button_02, CURRENT_INDEX, -1);
				g_pUIManager->SetData(WINDOW_OPTION_03, option_window_3_top_button_03, CURRENT_INDEX, 2);

				g_MainCharInfo.OpenFrame(WINDOW_OPTION_03);
			}
		}
		break;
	case option_window_2_scroll_01:		// 臧鞁滉卑毽
		{
			g_info_Temp.m_fViewDistance = g_pUIManager->GetData(WINDOW_OPTION_02, option_window_2_scroll_01, GET_SCROLL_CURRENT);
			g_pUIManager->SetString(WINDOW_OPTION_02, option_window_2_scroll_dummy_03, g_info_Temp.m_fViewDistance);
		}
		break;
	case option_window_2_scroll_02:		// 須瓿茧嫧瓿 (鞚错帣韸)
		{
			g_info_Temp.m_fPolygonDetail = g_pUIManager->GetData(WINDOW_OPTION_02, option_window_2_scroll_02, GET_SCROLL_CURRENT);
			g_pUIManager->SetString(WINDOW_OPTION_02, option_window_2_scroll_dummy_04, g_info_Temp.m_fPolygonDetail);
		}
		break;
	case option_window_2_scroll_03:		// 氚瓣步鞚岇晠 鞀ろ伂搿
		{
			g_info_Temp.m_dwBGMVolume = g_pUIManager->GetData(WINDOW_OPTION_02, option_window_2_scroll_03, GET_SCROLL_CURRENT);
			g_pUIManager->SetString(WINDOW_OPTION_02, option_window_2_scroll_dummy_07, g_info_Temp.m_dwBGMVolume);
		}
		break;
	case option_window_2_scroll_04:		// 須瓿检潓鞎 鞀ろ伂搿
		{
			g_info_Temp.m_dwFXVolume = g_pUIManager->GetData(WINDOW_OPTION_02, option_window_2_scroll_04, GET_SCROLL_CURRENT);
			g_pUIManager->SetString(WINDOW_OPTION_02, option_window_2_scroll_dummy_08, g_info_Temp.m_dwFXVolume);
		}
		break;
	case option_window_2_bottom_button_01:
		{
			bool bSend = false;
			if(g_info.m_bAllowWhisper != g_info_Temp.m_bAllowWhisper ||
				g_info.m_bAllowRelation != g_info_Temp.m_bAllowRelation ||
				g_info.m_bAllowTrade != g_info_Temp.m_bAllowTrade ||
				g_info.m_dwBuyLimit != g_info_Temp.m_dwBuyLimit ||
				g_info.m_bRarityLimit != g_info_Temp.m_bRarityLimit ||
				g_info.m_bStxTypeLimit != g_info_Temp.m_bStxTypeLimit)
			{
				bSend = true;
			}

			g_info = g_info_Temp;

			Save_Option(bSend);
			g_MainCharInfo.CloseFrame( WINDOW_OPTION_02);
		}
		break;
	}
}

/**
 * 瓯半灅 鞓奠厴
 * \param lParam 
 */
void ProcessWindowOption3(LPARAM lParam)
{
	int nControlID = LOWORD(lParam);

	switch(nControlID)
	{
	case option_window_3_title_close_button:	// 雼旮
	case option_window_3_bottom_button_02:
		{
			g_MainCharInfo.CloseFrame(WINDOW_OPTION_03);
		}
		break;
	case option_window_3_bottom_button_01:		// 須呺晬
		{
			bool bSend = false;
			if(g_info.m_bAllowWhisper != g_info_Temp.m_bAllowWhisper ||
				g_info.m_bAllowRelation != g_info_Temp.m_bAllowRelation ||
				g_info.m_bAllowTrade != g_info_Temp.m_bAllowTrade ||
				g_info.m_dwBuyLimit != g_info_Temp.m_dwBuyLimit ||
				g_info.m_bRarityLimit != g_info_Temp.m_bRarityLimit ||
				g_info.m_bStxTypeLimit != g_info_Temp.m_bStxTypeLimit)
			{
				bSend = true;
			}

			g_info = g_info_Temp;

			Save_Option(bSend);
			g_MainCharInfo.CloseFrame(WINDOW_OPTION_03);
		}
		break;
	case window_option_3_up_button_01:			// + 鞙 氩勴娂
		{
			if(g_info_Temp.m_bRarityLimit < 200)
			{
				++g_info_Temp.m_bRarityLimit;

				g_pUIManager->SetString(WINDOW_OPTION_03, window_option_3_sell_dummy_01, g_info_Temp.m_bRarityLimit);
			}
		}
		break;
	case window_option_3_down_button_01:		// + 鞎勲灅 氩勴娂
		{
			if(g_info_Temp.m_bRarityLimit > 0)
			{
				--g_info_Temp.m_bRarityLimit;

				g_pUIManager->SetString(WINDOW_OPTION_03, window_option_3_sell_dummy_01, g_info_Temp.m_bRarityLimit);
			}
		}
		break;
	case window_option_3_up_button_02:			// 靹 鞙 氩勴娂
		{
			if(g_info_Temp.m_bStxTypeLimit < 200)
			{
				++g_info_Temp.m_bStxTypeLimit;

				g_pUIManager->SetString(WINDOW_OPTION_03, window_option_3_sell_dummy_02, g_info_Temp.m_bStxTypeLimit);
			}
		}
		break;
	case window_option_3_down_button_02:		// 靹 鞎勲灅 氩勴娂
		{
			if(g_info_Temp.m_bStxTypeLimit > 0)
			{
				--g_info_Temp.m_bStxTypeLimit;

				g_pUIManager->SetString(WINDOW_OPTION_03, window_option_3_sell_dummy_02, g_info_Temp.m_bStxTypeLimit);
			}
		}
		break;
	case option_window_3_top_button_01:
		{
			g_MainCharInfo.CloseFrame(WINDOW_OPTION_03);
			g_MainCharInfo.OpenFrame(WINDOW_OPTION_01);

			g_pUIManager->SetData(WINDOW_OPTION_01, option_window_1_top_button_01, CURRENT_INDEX, 2);
			g_pUIManager->SetData(WINDOW_OPTION_01, option_window_1_top_button_02, CURRENT_INDEX, -1);
			g_pUIManager->SetData(WINDOW_OPTION_01, option_window_1_top_button_03, CURRENT_INDEX, -1);
		}
		break;
	case option_window_3_top_button_02:
		{
			g_MainCharInfo.CloseFrame(WINDOW_OPTION_03);
			g_MainCharInfo.OpenFrame(WINDOW_OPTION_02);

			g_pUIManager->SetData(WINDOW_OPTION_02, option_window_2_top_button_01, CURRENT_INDEX, -1);
			g_pUIManager->SetData(WINDOW_OPTION_02, option_window_2_top_button_02, CURRENT_INDEX, 2);
			g_pUIManager->SetData(WINDOW_OPTION_02, option_window_2_top_button_03, CURRENT_INDEX, -1);	
		}
		break;
	case option_window_3_top_button_03:
		{
		}
		break;
	default:
		break;
	}

}

void ProcessTabNpcTramde4( LPARAM lParam)
{
	int controlID = LOWORD( lParam);
	int eventType = HIWORD( lParam);

	if( !g_MainCharInfo.m_pNpcSack)
		return;

	if( !g_MainCharInfo.m_pNpcSack->IsShow())
		return;
	
	// Fallback if m_dwPickedObject was cleared: recover from currently selected NPC
	extern DWORD dwSelObjectID;
	extern DWORD dwSelObjectType;
	if( g_MainCharInfo.m_dwPickedObject == 0 && dwSelObjectID != 0 && dwSelObjectType == OBJTYPE_FUNCTIONALNPC )
	{
		g_MainCharInfo.m_dwPickedObject = dwSelObjectID;
	}

	XiahObject::CXiahObject* pObject = XiahObject::g_XiahObjectManager.FindXiahObject( MAKEOBJECTID( 0, g_MainCharInfo.m_dwPickedObject, OBJTYPE_FUNCTIONALNPC));

	if( pObject == NULL ) return;
	if( pObject->m_pObject == NULL ) return;

	CXiahCharObject* pCharObject = reinterpret_cast<CXiahCharObject*>( pObject->m_pObject);

	//YS_0811 : BUGFIX
	if ( !pCharObject ) return;

	sFunctionalNpcInfo* pInfo = (sFunctionalNpcInfo*)pCharObject->m_pPrivateData;

	//YS_0811 : BUGFIX
	if ( !pInfo ) return;

	if( pInfo->m_bType == 7)
		return;

	switch( controlID)
	{
	case npc_trade_4_tap_button_01:
		g_MainCharInfo.m_bSackCnt = 0;
		break;
	case npc_trade_4_tap_button_02:
		g_MainCharInfo.m_bSackCnt = 1;
		break;
	case npc_trade_4_tap_button_03:
		g_MainCharInfo.m_bSackCnt = 2;
		break;
	case npc_trade_4_tap_button_04:
		g_MainCharInfo.m_bSackCnt = 3;
		break;
	}
		
	SendCS_NC_FUNCTIONALNPCITEMLIST_REQ( XiahMap::g_XiahMap.m_MapInfo.m_dwMapID, g_MainCharInfo.m_dwPickedObject, g_MainCharInfo.m_bSackCnt);
}

extern BOOL	g_XiahGameStarted;

/**
 * 膦呺岇敖
 * \param lParam 
 */
void ProcessWindowClose(LPARAM lParam)
{
	int controlID = LOWORD(lParam);
	//int eventType = HIWORD( lParam);

	switch( controlID)
	{
		case close_window_button_01:	// 旌愲Ν韯 鞛靹犿儩
			{
				// 靸侅爯 雭
				g_MainCharInfo.m_bPersonalTradeSell = false;

				CloseAllWindow();

				Stop_BGM();
				// 鞁滌瀾 氚瓣步鞚岇晠
				Play_BGM(_T("sound\\bgm\\intro01.mp3"), 1);

				g_pUIManager->Hide(WINDOW_BUTTON_GROUP_01); // 瓴岇瀯 氅旊壌
				g_pUIManager->Hide(SYSTEM_BUTTON_GROUP_01); // 鞁滌姢韰 氅旊壌
				g_pUIManager->Hide(PET_BUTTON_GROUP);		// 韼
				g_pUIManager->Hide(LARGE_MESSENGER);
				g_pUIManager->Hide(HELP_BUTTON);				//HO_0413_07 韤 臧鞚措摐 鞐呺嵃鞚错姼

				// 鞓ろ枆 氩勴娂 齑堦赴頇
				for(int i=0; i < 5; ++i)
					g_pUIManager->SetData(WINDOW_FIVEELEMENTS, fiveelements_window_button_fire+i, CURRENT_INDEX, -1);

				//HT_0720 : 鞓ろ枆 臧滌劆 靷頃
				g_pUIManager->SetData(MAIN_FRAME, main_frame_ok, TEXTURE, 1536);

				// 2004.07.20 鞚措菠韸胳毄 搿滊敥頇旊┐
				/*
				if( rand() % 2 )
				g_MainCharInfo.OpenFrame( EVENT_LOADING_1 );
				else
				g_MainCharInfo.OpenFrame( EVENT_LOADING_2 );
				*/

				g_MainCharInfo.OpenFrame(LOADING_IMAGE3); //HO_0702_07 霌标笁響滌嫓 : 霌标笁響滌嫓鞕 頃瓴 鞀ろ儉韸鸽滊敥瓿 瓴岇瀯搿滊敥 攵攵勳澊 霃欖澕 鞚措胳搿 觳橂Μ霅滊嫟.
				
				//霌标笁響滌嫓 鞝侅毄鞝 旖旊摐 雮橃戩棎 歆鞗 氩勲Μ鞛 ..; 霌标笁響滌嫓 鞝勳棎電 雮橃澊 甑攵勳澊 鞛堨棁雼...
				//if(g_AppData.m_bAdult)
				//	g_MainCharInfo.OpenFrame(LOADING_IMAGE2);
				//else
				//	g_MainCharInfo.OpenFrame(LOADING_IMAGE);

				g_PetList.Release();

				SendCS_NV_ENDGAME_REQ();
				SET_GAMESTEP( GAMESTEP_INTRO);				
				XiahObject::g_XiahObjectManager.Release();	// 齑堦赴頇
				g_pMainChar = NULL;
				g_pIntro->Init_Clear();
				g_MainCharInfo.Clear();

				// 氚瓣步鞚 SKYBOX 韰嶌姢觳 靹れ爼 (瓿犾偘)
				//g_SkyBox.ChangeSkyMap(3);

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

				SendCS_IT_CHARACTERLIST_REQ();

				g_XiahGameStarted			 = FALSE;
				g_MainCharInfo.m_bCharChange = true;
				g_MainCharInfo.m_bFastMove	 = false;

				//HT_0403 : 歆靻嶍槙 氍搓车 鞁滌爠 鞎勳澊旖
				g_MainCharInfo.m_vkeepUpMugongIconList.clear();
				g_MainCharInfo.m_vkeepUpPetMugongIconList.clear();
			}
			break;

		case close_window_button_02:
			g_MainCharInfo.CloseFrame( WINDOW_CLOSE);
			break;

		case close_window_button_03:
			//PostMessage( g_AppData.m_hWnd, WM_CLOSE, 0, 0);
				g_MainCharInfo.CloseFrame( WINDOW_CLOSE);
				g_pUIManager->ShowNotice( IDS_GAME_SELECTCLOSE, NOTICE_FRAME_OKCANCEL, NOTICE_FRAME_ENDGAME); //HO_0816_07 鞎勳澊韰 霌滊瀺鞁 須呺晬
			break;
	}
}

#define MAIN_CHAROBJECT	((CXiahCharObject*)(g_pMainChar->m_pObject))

// 臧滌澑 靸侅爯 靹れ爼彀
void ProcessWindowPcStore( LPARAM lParam)
{
	int controlID = LOWORD( lParam);
	int eventType = HIWORD( lParam);

	switch(controlID)
	{
	case pc_store_close_button:
		{
			if(!g_MainCharInfo.m_bPersonalTradeSell)
			{
				g_MainCharInfo.HideSack( SACKTYPE__PERSONAL_TRADE_SET);
			}

			if( g_MainCharInfo.m_pHoldItem)
				g_MainCharInfo.m_pHoldItem->SetItemBackToSack();
		}		
		break;
	case pc_store_button_02:  // 韺愲Г鞁滌瀾&欷戩
		{
			// 欤届棃鞚勲晫 鞝滌櫢
			if(!g_MainCharInfo.m_bMainCharDie)
			{
				g_MainCharInfo.m_bPersonalTradeSell = !g_MainCharInfo.m_bPersonalTradeSell;
				// STOP
				if(MAIN_CHAROBJECT->GetAnimation() == XiahAniType::eLAT_Run)
				{
					MAIN_CHAROBJECT->SetAnimation( XiahAniType::eLAT_Stand, 0);
				}

				// 韺愲Г鞁滌瀾/欷戩鞁 鞙勳箻 氤挫爼
				SendCS_NV_ENDMOVE_REQ(g_pMainChar->m_dwServerID, MAIN_CHAROBJECT->m_Position.x, -MAIN_CHAROBJECT->m_Position.z, MAIN_CHAROBJECT->m_Position.y, CHARSTATE_NORMAL);

				SendCS_SH_STATUSCHANGE_REQ((BYTE)g_MainCharInfo.m_bPersonalTradeSell);
			}
		}
		break;
		
	case pc_store_button_03: // 旮堨爠須岇垬
		{	
			if(g_MainCharInfo.m_dwTradeMoney)
				SendCS_SH_GETMONEY_REQ( g_MainCharInfo.m_dwTradeMoney);
		}
		break;

	case pc_store_button_01:  // 順戈皾氍戈惮 氤瓴
		{
			//LPCTSTR strName;
			//LPCTSTR strDescription;

			TCHAR strName[256], strDescription[256];
			memset(strName, 0, 256);
			memset(strDescription, 0, 256);

			g_pUIManager->GetString(WINDOW_PC_STORE, pc_store_passage_edit_01, strName, GET_STRING);
			g_pUIManager->GetString(WINDOW_PC_STORE, pc_store_passage_edit_02, strDescription, GET_STRING);

			SendCS_SH_SETSHOP_REQ(strName, strDescription);								   		// 靹れ爼 氤瓴

			//strName = g_pUIManager->GetString(WINDOW_PC_STORE, pc_store_passage_edit_01);        // 雲胳爯氇
			//strDescription = g_pUIManager->GetString(WINDOW_PC_STORE, pc_store_passage_edit_02); // 順戈皾氍戈惮

			//SendCS_SH_SETSHOP_REQ(strName, strDescription);								   		// 靹れ爼 氤瓴

			/*

				strName = ((CIEditBox*)pFrame->GetControl( pc_store_passage_edit_01))->m_Text;            // 雲胳爯氇
				strDescription = ((CIEditBox*)pFrame->GetControl( pc_store_passage_edit_02))->m_Text;     // 順戈皾氍戈惮

				SendCS_SH_SETSHOP_REQ(strName, strDescription);
			*/
		}
		break;
	} // switch(controlID)
}

// x臧侅爜
// 氍疙寣瓿奠
void ProcessWindowGakMessage(LPARAM lParam)
{
	int controlID = LOWORD(lParam);
	int nEventType = HIWORD(lParam);

	switch(nEventType)
	{
	case GAK_MSG_WINDOW_MSG:	// 臧侅爜
		{
			switch(controlID)
			{
			case gak_button_01:
				{
					BYTE bTemp = 0;

					switch(g_MainCharInfo.m_nTempValue)
					{
					case 2:
						bTemp = CT_SAYITEM_CELL;
						break;
					case 3:
						bTemp = CT_SAYITEM_MAP;
						break;
					case 4:
						bTemp = CT_SAYITEM_CHANNEL;
						break;
					case 8:	// 頇╆笀臧侅爜
						bTemp = 15;
						break;
					} // switch(g_MainCharInfo.m_nTempValue)

					TCHAR strChat[256] = {0,};
					g_pUIManager->GetString(GAK_MESSAGE_WINDOW, gak_message_edit, strChat);

					SendCS_CH_CHAT_REQ( bTemp, 0, strChat, _T(""));

					g_pUIManager->SetString(GAK_MESSAGE_WINDOW, gak_message_edit, _T(""));

					g_pUIManager->SetFocus(GAK_MESSAGE_WINDOW, gak_message_edit);
				}		
				break;

			case gak_button_02:
				g_MainCharInfo.CloseFrame(GAK_MESSAGE_WINDOW);
				break;
			} // switch(controlID)
		}
		break;
	case GAK_MSG_WINDOW_MUNPA:	// 氍疙寣瓿奠
		{
			switch(controlID)
			{
			case gak_button_01:	// ok
				{
					TCHAR strChat[256] = {0,};
					g_pUIManager->GetString(GAK_MESSAGE_WINDOW, gak_message_edit, strChat);

					SendCS_RL_MUNPANOTICE_REQ(strChat);

					g_pUIManager->SetString(GAK_MESSAGE_WINDOW, gak_message_edit, _T(""));
				}
				break;
			case gak_button_02:				
				break;
			} // switch(controlID)

			g_MainCharInfo.CloseFrame(GAK_MESSAGE_WINDOW);
		}
		break;
	default:
		break;
	}
}

// 霃欖嫚鞝
void ProcessWindowDongSin(LPARAM lParam)
{
	int controlID = LOWORD( lParam);
	int eventType = HIWORD( lParam);

	switch(controlID)
	{
	case dongsin_window_button_01:
		if(g_MainCharInfo.m_bMark)
		{
			CXiahCharObject* pMainChar = (CXiahCharObject*)g_pMainChar->m_pObject;
			if(!g_MainCharInfo.m_bPortalMove)
			{
				// 门派战对决期间无法使用
				if( pMainChar->m_dwPartyID && pMainChar->m_dwEnemyPartyID )
				{
                    g_MainCharInfo.ShowHelpMessage(IDS_NOTPORTALMOVE_INDANBATTLE);
					return;
				}
				// 移除活动物品无法移动限制，允许正常传送
			}

			// 鞎勳澊韰滌潉 靷鞖╉晿鞐 鞚措彊頃滊嫟.
			g_MainCharInfo.m_bMainCharMapMoveItemUse = TRUE;

			g_MainCharInfo.ShowHelpMessage( IDS_MOVE_ITEMUSE );

			SendCS_IM_USEITEM_REQ( g_MainCharInfo.m_ReairSackID, g_MainCharInfo.m_RpairItemPos, g_MainCharInfo.m_dwResItemID);

			g_MainCharInfo.CloseFrame(WINDOW_DONGSIN);
//			g_MainCharInfo.OpenFrame( LOADING_IMAGE);
		}
		break;

	case dongsin_window_button_02:
		SendCS_IM_REMARKITEM_REQ(g_MainCharInfo.m_dwResItemID, g_MainCharInfo.m_ReairSackID, g_MainCharInfo.m_RpairItemPos);
		g_MainCharInfo.CloseFrame(WINDOW_DONGSIN);
		break;
	}
}


/**
 * 鞝勲偔
 * \param lParam 
 */
void ProcessWindowPurse(LPARAM lParam)
{
	const int nControlID = LOWORD(lParam);
	const int nEventType = HIWORD(lParam);

	switch(nControlID)
	{
	case window_purse_edit:
	case window_purse_button_01:
		{
			__int64 nTemp = _tstoi64(static_cast<LPCTSTR>(g_pUIManager->GetString(WINDOW_PURSE, window_purse_edit)));

			if( nTemp > 2100000000)  // 21鞏 鞚挫儊 鞛呺牓 攵堦皜
			{
				g_MainCharInfo.ShowHelpMessage(IDS_MANY_MONEY, TEXTEFFECT_COLOR_WARNING);
				return;
			} // if( nTemp > 2100000000)

			// 1鞏奠爠旯岇 瓯半灅 臧電ロ晿瓿 靹滊矂鞐 韺韨冯偁毽措晫 DWORD搿...
			DWORD dwAmount = static_cast<DWORD>(nTemp);

			if(dwAmount > 0)
			{
				SendCS_IM_MONEYBAG_REQ( g_MainCharInfo.m_byPurseAction,
										g_MainCharInfo.m_dwPurseItemID,
										g_MainCharInfo.m_byPurseSackID,
										g_MainCharInfo.m_byPurseSackPos,
										dwAmount );
			}
			else
			{
				g_MainCharInfo.ShowHelpMessage(IDS_DANWAR_MONEY, TEXTEFFECT_COLOR_WARNING);
				return;
			}
		}
		break;
		
	case window_purse_button_02:
		break;

	default:
		return;
		break;
	}

	g_MainCharInfo.CloseFrame(WINDOW_PURSE);
	g_pUIManager->SetString(WINDOW_PURSE, window_purse_edit, 0);
}

/**
 * 氍疙寣 順勴櫓
 * \param lParam 
 */
void ProcessWindowMunpaBBSTop(LPARAM lParam)
{
	const int nControlID = LOWORD(lParam);
	const int nEventType = HIWORD(lParam);

	switch(nControlID)
	{
	case munpa_bbs_top_close_button:
		{
			g_MainCharInfo.CloseFrame(WINDOW_MUNPA_BBS_TOP);
		}
		break;

	case munpa_bbs_top_money_button:	// 靹戈笀須岇垬
		{
			CXiahCharObject* pMainChar = reinterpret_cast<CXiahCharObject*>(g_pMainChar->m_pObject);

			if(pMainChar)
				SendCS_RL_GETMUNPAMONEY_REQ(pMainChar->m_dwMunpaID, g_MainCharInfo.m_dwTaxMunpaMoney, g_MainCharInfo.m_dwPickedObject);
		}
		break;

	case munpa_bbs_top_button_01:		// 氍疙寣 順勴櫓
		break;

	case munpa_bbs_top_button_02:		// 瓿奠 靷頃 (毽鞀ろ姼)
		{
			if(g_MainCharInfo.m_pListClient)
				delete g_MainCharInfo.m_pListClient, g_MainCharInfo.m_pListClient = NULL;

			g_MainCharInfo.m_pListClient = new CListClient(CListClient::MUNPA_BBS_LIST);
			g_MainCharInfo.m_pListClient->Set(760, 80, 240, 20);

			CXiahCharObject* pMainChar = reinterpret_cast<CXiahCharObject*>(g_pMainChar->m_pObject);
			if(pMainChar)
				SendCS_RL_MUNPABBSLIST_REQ(pMainChar->m_dwMunpaID);		// 毽鞀ろ姼 鞖旍箔

			g_MainCharInfo.CloseFrame(WINDOW_MUNPA_BBS_TOP);
			g_MainCharInfo.OpenFrame(WINDOW_MUNPA_BBS_LIST);
		}
		break;

	case munpa_bbs_top_button_03:		// 瓿奠 旮半
		{
			g_MainCharInfo.CloseFrame(WINDOW_MUNPA_BBS_TOP);

			g_MainCharInfo.OpenFrame(WINDOW_MUNPA_BBS_WRITE);
			g_pUIManager->SetFocus(WINDOW_MUNPA_BBS_WRITE, munpa_bbs_write_edit);
		}
		break;

	case munpa_bbs_top_button_04:		// 靷鞝
		break;

	default:
		break;
	}
}

/**
 * 氍疙寣 瓴岇嫓韺 毽鞀ろ姼
 * \param lParam 
 */
void ProcessWindowMunpaBBSList(LPARAM lParam)
{
	const int nControlID = LOWORD(lParam);

	switch(nControlID)
	{
	case munpa_bbs_list_close_button:
		{
			g_MainCharInfo.CloseFrame(WINDOW_MUNPA_BBS_LIST);

			if(g_MainCharInfo.m_pListClient)
				delete g_MainCharInfo.m_pListClient, g_MainCharInfo.m_pListClient = NULL;			
		}
		break;

	case munpa_bbs_list_button_01:		// 順勴櫓
		{
			g_MainCharInfo.CloseFrame(WINDOW_MUNPA_BBS_LIST);
			g_MainCharInfo.OpenFrame(WINDOW_MUNPA_BBS_TOP);

			if(g_MainCharInfo.m_pListClient)
				delete g_MainCharInfo.m_pListClient, g_MainCharInfo.m_pListClient = NULL;
		}
		break;
	case munpa_bbs_list_button_02:		
		break;
	case munpa_bbs_list_button_03:		// 旮半
		{
			g_MainCharInfo.CloseFrame(WINDOW_MUNPA_BBS_LIST);

			if(g_MainCharInfo.m_pListClient)
				delete g_MainCharInfo.m_pListClient, g_MainCharInfo.m_pListClient = NULL;

			g_MainCharInfo.OpenFrame(WINDOW_MUNPA_BBS_WRITE);
			g_pUIManager->SetFocus(WINDOW_MUNPA_BBS_WRITE, munpa_bbs_write_edit);
		}
		break;
	case munpa_bbs_list_button_04:		// 靷鞝
		{
			if(g_MainCharInfo.m_pListClient)
			{
				DWORD dwTemp = g_MainCharInfo.m_pListClient->DelString(0, 1);
				CXiahCharObject* pMainChar = reinterpret_cast<CXiahCharObject*>(g_pMainChar->m_pObject);

				SendCS_RL_MUNPABBSDEL_REQ(pMainChar->m_dwMunpaID, dwTemp);
			}
		}
		break;
	}
}

/**
 * BBS 鞚疥赴
 * \param lParam 
 */
void ProcessWindowMunpaBBSRead(LPARAM lParam)
{
	const int nControlID = LOWORD(lParam);
	// const int nEventType = HIWORD(lParam);

	switch(nControlID)
	{
	case munpa_bbs_read_close_button:
		g_MainCharInfo.CloseFrame(WINDOW_MUNPA_BBS_READ);
		break;

	case munpa_bbs_read_button_01:			// 順勴櫓
		{
			g_MainCharInfo.CloseFrame(WINDOW_MUNPA_BBS_READ);
			g_MainCharInfo.OpenFrame(WINDOW_MUNPA_BBS_TOP);
		}
		break;

	case munpa_bbs_read_button_02:			// 瓿奠 靷頃 (毽鞀ろ姼)
		{
			if(g_MainCharInfo.m_pListClient)
				delete g_MainCharInfo.m_pListClient, g_MainCharInfo.m_pListClient = NULL;

			g_MainCharInfo.m_pListClient = new CListClient(CListClient::MUNPA_BBS_LIST);
			g_MainCharInfo.m_pListClient->Set(760, 80, 240, 20);

			CXiahCharObject* pMainChar = reinterpret_cast<CXiahCharObject*>(g_pMainChar->m_pObject);
			SendCS_RL_MUNPABBSLIST_REQ(pMainChar->m_dwMunpaID);			// 毽鞀ろ姼 鞖旍箔

			g_MainCharInfo.CloseFrame(WINDOW_MUNPA_BBS_READ);
			g_MainCharInfo.OpenFrame(WINDOW_MUNPA_BBS_LIST);
		}
		break;
	case munpa_bbs_read_button_03:			// 旮半
		{
			g_MainCharInfo.CloseFrame(WINDOW_MUNPA_BBS_READ);
			g_MainCharInfo.OpenFrame(WINDOW_MUNPA_BBS_WRITE);
			g_pUIManager->SetFocus(WINDOW_MUNPA_BBS_WRITE, munpa_bbs_write_edit);
		}
		break;
	case munpa_bbs_read_button_04:			// 靷鞝
		{
			if(g_MainCharInfo.m_pListClient)
			{
				DWORD dwTemp = g_MainCharInfo.m_pListClient->DelString(0, 1);
				CXiahCharObject* pMainChar = reinterpret_cast<CXiahCharObject*>(g_pMainChar->m_pObject);

				SendCS_RL_MUNPABBSDEL_REQ(pMainChar->m_dwMunpaID, dwTemp);
			}
		}
		break;
	default:
		break;
	}
}

/**
 * BBS 旮半
 * \param lParam 
 */
void ProcessWindowMunpaBBSWrite(LPARAM lParam)
{
	const int nControlID = LOWORD(lParam);
	const int nEventType = HIWORD(lParam);

	switch(nControlID)
	{
	case munpa_bbs_write_edit:
		g_pUIManager->SetFocus(WINDOW_MUNPA_BBS_WRITE, munpa_bbs_write_edit_01);
		break;

	case munpa_bbs_write_edit_01:
	case munpa_bbs_write_edit_01+190:
		g_pUIManager->SetFocus(WINDOW_MUNPA_BBS_WRITE, munpa_bbs_write_edit_02);
		break;
	case munpa_bbs_write_edit_02:
	case munpa_bbs_write_edit_02+190:
		g_pUIManager->SetFocus(WINDOW_MUNPA_BBS_WRITE, munpa_bbs_write_edit_03);
		break;
	case munpa_bbs_write_edit_03:
	case munpa_bbs_write_edit_03+190:
		g_pUIManager->SetFocus(WINDOW_MUNPA_BBS_WRITE, munpa_bbs_write_edit_04);
		break;
	case munpa_bbs_write_edit_04:
	case munpa_bbs_write_edit_04+190:
		g_pUIManager->SetFocus(WINDOW_MUNPA_BBS_WRITE, munpa_bbs_write_edit_05);
		break;
	case munpa_bbs_write_edit_05:
	case munpa_bbs_write_edit_05+190:
		g_pUIManager->SetFocus(WINDOW_MUNPA_BBS_WRITE, munpa_bbs_write_edit_06);
		break;
	case munpa_bbs_write_edit_06:
	case munpa_bbs_write_edit_06+190:
		g_pUIManager->SetFocus(WINDOW_MUNPA_BBS_WRITE, munpa_bbs_write_edit_07);
		break;
	case munpa_bbs_write_edit_07:
	case munpa_bbs_write_edit_07+190:
		g_pUIManager->SetFocus(WINDOW_MUNPA_BBS_WRITE, munpa_bbs_write_edit_08);
		break;
//	case munpa_bbs_write_edit_08:
//		g_pUIManager->SetFocus(WINDOW_MUNPA_BBS_WRITE, munpa_bbs_write_edit_09);
//		break;
//	case munpa_bbs_write_edit_09:
//		g_pUIManager->SetFocus(WINDOW_MUNPA_BBS_WRITE, munpa_bbs_write_edit_10);
//		break;

	case munpa_bbs_write_edit_02+130:
		g_pUIManager->SetFocus(WINDOW_MUNPA_BBS_WRITE, munpa_bbs_write_edit_01);
		break;
	case munpa_bbs_write_edit_03+130:
		g_pUIManager->SetFocus(WINDOW_MUNPA_BBS_WRITE, munpa_bbs_write_edit_02);
		break;
	case munpa_bbs_write_edit_04+130:
		g_pUIManager->SetFocus(WINDOW_MUNPA_BBS_WRITE, munpa_bbs_write_edit_03);
		break;
	case munpa_bbs_write_edit_05+130:
		g_pUIManager->SetFocus(WINDOW_MUNPA_BBS_WRITE, munpa_bbs_write_edit_04);
		break;
	case munpa_bbs_write_edit_06+130:
		g_pUIManager->SetFocus(WINDOW_MUNPA_BBS_WRITE, munpa_bbs_write_edit_05);
		break;
	case munpa_bbs_write_edit_07+130:
		g_pUIManager->SetFocus(WINDOW_MUNPA_BBS_WRITE, munpa_bbs_write_edit_06);
		break;
	case munpa_bbs_write_edit_08+130:
		g_pUIManager->SetFocus(WINDOW_MUNPA_BBS_WRITE, munpa_bbs_write_edit_07);
		break;
//	case munpa_bbs_write_edit_09+130:
//		g_pUIManager->SetFocus(WINDOW_MUNPA_BBS_WRITE, munpa_bbs_write_edit_08);
//		break;
//	case munpa_bbs_write_edit_10+130:
//		g_pUIManager->SetFocus(WINDOW_MUNPA_BBS_WRITE, munpa_bbs_write_edit_09);
//		break;

	case munpa_bbs_write_close_button:
	case munpa_bbs_write_button_02:
		{
			g_MainCharInfo.CloseFrame(WINDOW_MUNPA_BBS_WRITE);

			g_pUIManager->SetString(WINDOW_MUNPA_BBS_WRITE, munpa_bbs_write_edit, _T(""));

			for(int j=0; j < 8; ++j)
				g_pUIManager->SetString(WINDOW_MUNPA_BBS_WRITE, munpa_bbs_write_edit_01 + j, _T(""));
		}
		break;
	case munpa_bbs_write_button_01:
		{			
			TCHAR strTitle[64] = {0,};

			g_pUIManager->GetString(WINDOW_MUNPA_BBS_WRITE, munpa_bbs_write_edit, strTitle, GET_STRING);

			if(_tcslen(strTitle))
			{
				TCHAR strMix[320] = {0,};

				g_pUIManager->GetString(WINDOW_MUNPA_BBS_WRITE, munpa_bbs_write_edit_01, strMix, GET_STRING);

				for(int i=0; i < 7; ++i)
				{
					TCHAR strTemp[256] = {0,};
					g_pUIManager->GetString(WINDOW_MUNPA_BBS_WRITE, munpa_bbs_write_edit_02 + i, strTemp, GET_STRING);

					_ftcscat(strMix, _T("|"));
					_ftcscat(strMix, strTemp);
				} // for(int i=0; i < 9; ++i)

				CXiahCharObject* pMainChar = reinterpret_cast<CXiahCharObject*>(g_pMainChar->m_pObject);

				SendCS_RL_MUNPABBSWRITE_REQ(pMainChar->m_dwMunpaID, strTitle, strMix);


				g_MainCharInfo.CloseFrame(WINDOW_MUNPA_BBS_WRITE);

				g_pUIManager->SetString(WINDOW_MUNPA_BBS_WRITE, munpa_bbs_write_edit, _T(""));

				for(int j=0; j < 8; ++j)
					g_pUIManager->SetString(WINDOW_MUNPA_BBS_WRITE, munpa_bbs_write_edit_01 + j, _T(""));
			}
			else
			{
				g_MainCharInfo.ShowHelpMessage(IDS_M_TITLE_NOT, TEXTEFFECT_COLOR_WARNING);
			}
		}
		break;
	
	default:
		break;
	}

}

/**
 * 旮半秬旮 雮╇秬
 * \param lParam 
 */
void ProcessWindowMunpaDonate(LPARAM lParam)
{
	const int nControlID = LOWORD(lParam);
	//const int nEventType = HIWORD(lParam);

	switch(nControlID)
	{
	case munpa_donate_edit:
	case munpa_donate_check_button:		// 旮半秬旮 須呺晬
		{
			int nTemp = _tstoi(static_cast<LPCTSTR>(g_pUIManager->GetString(WINDOW_MUNPA_DONATE, munpa_donate_edit)));

			if(nTemp > 0)
			{
				CXiahCharObject* pMainChar = reinterpret_cast<CXiahCharObject*>(g_pMainChar->m_pObject);
				SendCS_RL_PREDONATE_REQ(pMainChar->m_dwMunpaID, static_cast<DWORD>(nTemp));
			}
		}
		break;

	case munpa_donate_button_01:		// 雮╇秬
		{
			int nTemp = _tstoi(static_cast<LPCTSTR>(g_pUIManager->GetString(WINDOW_MUNPA_DONATE, munpa_donate_edit)));

			if(nTemp > 0)
			{
				CXiahCharObject* pMainChar = reinterpret_cast<CXiahCharObject*>(g_pMainChar->m_pObject);

				SendCS_RL_DONATE_REQ(pMainChar->m_dwMunpaID, static_cast<DWORD>(nTemp), g_MainCharInfo.m_pRelation->m_dwDonateMoney);

				g_MainCharInfo.CloseFrame(WINDOW_MUNPA_DONATE);
				g_pUIManager->SetString(WINDOW_MUNPA_DONATE, munpa_donate_dummy_06, _T(" "));
			}
		}
		break;

	case munpa_donate_button_02:
		{
			g_MainCharInfo.CloseFrame(WINDOW_MUNPA_DONATE);
			g_pUIManager->SetString(WINDOW_MUNPA_DONATE, munpa_donate_dummy_06, _T(" "));
		}
		break;

	default:
		break;
	}
}

/**
 * 氍疙寣鞝 鞁犾箔
 * \param lParam 
 */
void ProcessWindowMunpaWarPetition(LPARAM lParam)
{
	const int nControlID = LOWORD(lParam);

	switch(nControlID)
	{
	case munpa_war_petition_combo_button_01:
		{
			TCHAR strTemp[7][64] = {0,};
			TCHAR strToolTip[7][64] = {0,};

			std::map<int, sMunpaWarDay*>::iterator iter = g_MainCharInfo.m_pRelation->m_mMunpaWarDayList.begin();

			for(register int i = 0; (i < 7) && (iter != g_MainCharInfo.m_pRelation->m_mMunpaWarDayList.end()) ; ++i, ++iter)
			{
				sMunpaWarDay *pInfo = iter->second;

				if(pInfo)
				{
					TCHAR strDay[64] = {0,};
					LPSTR lpstrTemp;

					int nTime = GETHOUR(pInfo->dwGameTime);

					if(nTime <= 11)
						lpstrTemp = IDS_MORNING;
					else
					{
						lpstrTemp = IDS_AFTERNOON;
						nTime -= 12;
					}

					_stprintf(strDay, IDS_M_WAR_DAY, GETMONTH(pInfo->dwGameTime)+1, GETDAY(pInfo->dwGameTime)+1,
														lpstrTemp, nTime);
					_tcscpy(strTemp[i], strDay);


					// 韴错寔
					nTime = pInfo->dwRealTime;
					
					if(nTime <= 11)
						lpstrTemp = IDS_MORNING;
					else
					{
						lpstrTemp = IDS_AFTERNOON;
						nTime -= 12;
					}

					_stprintf(strDay, IDS_MUNPA_WAR_DAY, lpstrTemp, nTime);
					_tcscpy(strToolTip[i], strDay);
				}
				else
				{
					return;
				}
			}

			RECT rtRegion;
			g_pUIManager->GetRegionData(WINDOW_MUNPA_WAR_PETITION, munpa_war_petition_select_dummy_01, rtRegion);
			
			g_pUIManager->MakePopComboMenu(100, rtRegion.right, rtRegion.bottom, FRAMEID_WAR_DAY, 7,
											TRUE, RESID_COMBO_2, strTemp[0],
											TRUE, RESID_COMBO_2, strTemp[1],
											TRUE, RESID_COMBO_2, strTemp[2],
											TRUE, RESID_COMBO_2, strTemp[3],
											TRUE, RESID_COMBO_2, strTemp[4],
											TRUE, RESID_COMBO_2, strTemp[5],
											TRUE, RESID_COMBO_2, strTemp[6]);

			for(register int x = 0; x < 7; ++x)
				g_pUIManager->SetToolTip(FRAMEID_WAR_DAY, x, 3, strToolTip[x], 2);

		}
		break;
	case munpa_war_petition_combo_button_02:
		{
			RECT rtRegion;
			g_pUIManager->GetRegionData(WINDOW_MUNPA_WAR_PETITION, munpa_war_petition_select_dummy_02, rtRegion);			

			g_pUIManager->MakePopComboMenu(100, rtRegion.right, rtRegion.bottom, FRAMEID_STONE_MOVE, 2,
											TRUE, RESID_COMBO_2, IDS_MUNPA_WAR_STONE_1,
											TRUE, RESID_COMBO_2, IDS_MUNPA_WAR_STONE_2);
		}
		break;

	case munpa_war_petition_button_01:
		{
			if(g_MainCharInfo.m_pRelation->m_dwMunpaWarTime != 999)
			{
				SendCS_WR_CHALLENGEWAR_REQ(g_MainCharInfo.m_pRelation->m_dwMunpaWarEnemy,
											g_MainCharInfo.m_pRelation->m_dwMunpaWarTime,
											g_MainCharInfo.m_pRelation->m_bStealStone);

				g_MainCharInfo.CloseFrame(WINDOW_MUNPA_WAR_PETITION);
			}
		}
		break;
	case munpa_war_petition_button_02:
		{
			g_MainCharInfo.CloseFrame(WINDOW_MUNPA_WAR_PETITION);
		}
		break;

	default:
		break;
	}
}


/**
 * 鞝勳劀甑
 * \param lParam 
 */
void ProcessWindowMail(LPARAM lParam)
{
	const int nControlID = LOWORD(lParam);

	switch(nControlID)
	{
	case window_mail_close:
		CloseAllWindow();
		//g_MainCharInfo.CloseFrame(WINDOW_MAIL);
		break;
	case window_mail_button_01:		// 鞚疥赴
			g_pUIManager->SetString(WINDOW_MAIL, window_mail_button_04, IDS_REPLY);
			g_pUIManager->SetString(WINDOW_MAIL, window_mail_input_dummy_01, IDS_MAIL_01);
			g_pUIManager->SetData(WINDOW_MAIL, window_mail_top_edit_01, EDITMODE, NOEDIT);

			g_Mail.Reflash_MAIL();
			g_Mail.Send_ReadMail();

			g_MainCharInfo.CloseFrame(WINDOW_MAIL_RESULT);
			g_MainCharInfo.OpenFrame(WINDOW_MAIL_SELECT);
		break;

	case window_mail_button_02:		// 鞊瓣赴
		{
			g_pUIManager->SetData(WINDOW_MAIL, window_mail_top_edit_01, EDITMODE, EDIT);
			g_pUIManager->SetString(WINDOW_MAIL, window_mail_button_04, IDS_SEND);
			// 氤措偢 靷霝岇棎靹 氚涬姅 靷霝岇溂搿
			g_pUIManager->SetString(WINDOW_MAIL, window_mail_input_dummy_01, IDS_MAIL_TITLE_02);

			g_pUIManager->SetString(WINDOW_MAIL, window_mail_top_edit_01, _T(""));
			g_pUIManager->SetString(WINDOW_MAIL, window_mail_top_edit_02, _T(""));
		
			for(int j=0; j < 8; ++j)
				g_pUIManager->SetString(WINDOW_MAIL, window_mail_edit_01 - j, _T(""));

			g_pUIManager->SetFocus(WINDOW_MAIL, window_mail_top_edit_01);

			g_MainCharInfo.CloseFrame(WINDOW_MAIL_RESULT);
			g_MainCharInfo.OpenFrame(WINDOW_MAIL_SELECT);

		}
		break;

	case window_mail_button_03:		// 靷鞝
		{
			g_MainCharInfo.CloseFrame(WINDOW_MAIL_SELECT);
			g_pUIManager->ShowNotice(IDS_YESNO_DELETE, NOTICE_FRAME_OKCANCEL, NOTICE_FRAME_MAIL_DELETE);

			g_MainCharInfo.CloseFrame(WINDOW_MAIL_RESULT);
		}
		break;

	case window_mail_button_04:		// 須岇嫚&鞝勳啞
		{
			TCHAR szText[100] = {0,};
			_stprintf(szText,IDS_YESNO_SENDMAIL,g_Mail.Get_Checked_SendList(),g_Mail.Get_Amonut());
			g_pUIManager->ShowNotice(szText, NOTICE_FRAME_OKCANCEL, NOTICE_FRAME_MAIL_SEND);
			g_MainCharInfo.CloseFrame(WINDOW_MAIL_SELECT);
			g_MainCharInfo.CloseFrame(WINDOW_MAIL_RESULT);
		}
		break;

	case window_mail_button_left:	// 霋
		g_Mail.Back_Recv_Page();
		break;
	case window_mail_button_right:	// 鞎
		g_Mail.Next_Recv_Page();
		break;

/////////////////////////////////////////////////////////////////////////////////////////////////////

	case window_mail_top_edit_01:	// 氤措偢靷霝 鞐旐劙鞁 鞝滊╈溂搿
		g_pUIManager->SetFocus(WINDOW_MAIL, window_mail_top_edit_02);
		break;
	case window_mail_top_edit_02:	// 鞝滊╈棎靹 雮挫毄 觳 欷
		g_pUIManager->SetFocus(WINDOW_MAIL, window_mail_edit_01);
		break;

		// enter
	case window_mail_edit_01:
	case window_mail_edit_01+190:
		g_pUIManager->SetFocus(WINDOW_MAIL, window_mail_edit_02);
		break;
	case window_mail_edit_02:
	case window_mail_edit_02+190:
		g_pUIManager->SetFocus(WINDOW_MAIL, window_mail_edit_03);
		break;
	case window_mail_edit_03:
	case window_mail_edit_03+190:
		g_pUIManager->SetFocus(WINDOW_MAIL, window_mail_edit_04);
		break;
	case window_mail_edit_04:
	case window_mail_edit_04+190:
		g_pUIManager->SetFocus(WINDOW_MAIL, window_mail_edit_05);
		break;
	case window_mail_edit_05:
	case window_mail_edit_05+190:
		g_pUIManager->SetFocus(WINDOW_MAIL, window_mail_edit_06);
		break;
	case window_mail_edit_06:
	case window_mail_edit_06+190:
		g_pUIManager->SetFocus(WINDOW_MAIL, window_mail_edit_07);
		break;
	case window_mail_edit_07:
	case window_mail_edit_07+190:
		g_pUIManager->SetFocus(WINDOW_MAIL, window_mail_edit_08);
		break;

		// backspace
	case window_mail_edit_02+130:
		g_pUIManager->SetFocus(WINDOW_MAIL, window_mail_edit_01);
		break;
	case window_mail_edit_03+130:
		g_pUIManager->SetFocus(WINDOW_MAIL, window_mail_edit_02);
		break;
	case window_mail_edit_04+130:
		g_pUIManager->SetFocus(WINDOW_MAIL, window_mail_edit_03);
		break;
	case window_mail_edit_05+130:
		g_pUIManager->SetFocus(WINDOW_MAIL, window_mail_edit_04);
		break;
	case window_mail_edit_06+130:
		g_pUIManager->SetFocus(WINDOW_MAIL, window_mail_edit_05);
		break;
	case window_mail_edit_07+130:
		g_pUIManager->SetFocus(WINDOW_MAIL, window_mail_edit_06);
		break;
	case window_mail_edit_08+130:
		g_pUIManager->SetFocus(WINDOW_MAIL, window_mail_edit_07);
		break;

	default:
		break;
	} // switch(nControlID)
}

/**
 * 鞝勳劀甑 靹犿儩
 * \param lParam 
 */
void ProcessWindowMailSelect(LPARAM lParam)
{
	const int nControlID = LOWORD(lParam);

	switch(nControlID)
	{
	case window_mail_select_close :
		g_MainCharInfo.CloseFrame(WINDOW_MAIL_SELECT);
		break;
	case window_mail_select_button_left:	// 雼れ潓 氩勴娂
		g_Mail.Back_Page();
		break;
	case window_mail_select_button_right:	// 鞚挫爠
		g_Mail.Next_Page();
		break;
	case window_mail_select_button_01:		// 氍疙寣 靹犿儩
		g_Mail.CheckedMunpaAll_SendList();
		g_Mail.Reflash_MAIL_Select();
		break;
	case window_mail_select_button_02:		// 鞝勳泊 靹犿儩
		g_Mail.CheckedAll_SendList();
		g_Mail.Reflash_MAIL_Select();
		break;
	case window_mail_select_button_03:		// 鞝勳泊 頃挫牅
		g_Mail.UncheckedAll_SendList();
		g_Mail.Reflash_MAIL_Select();
		break;
	default:
		break;
	}
}

/**
 * 鞝勳劀甑 鞝勳啞 須呺晬
 * \param lParam 
 */
void ProcessWindowMailResult(LPARAM lParam)
{
	const int nControlID = LOWORD(lParam);

	switch(nControlID)
	{
	case window_mail_result_close:
		g_MainCharInfo.CloseFrame(WINDOW_MAIL_RESULT);
		break;
	case window_mail_result_button_01:		// 須呺晬
		g_MainCharInfo.CloseFrame(WINDOW_MAIL_RESULT);
		g_MainCharInfo.OpenFrame(WINDOW_MAIL_SELECT);
		break;
	case window_mail_result_button_left:
		g_Mail.Back_Result_Page();
		break;
	case window_mail_result_button_right:
		g_Mail.Next_Result_Page();
		break;

	default:
		break;
	}
}

/**
*
* \param lParam 
*/
void ProcessWindowCommon(LPARAM lParam)
{
	int nControlID = LOWORD(lParam);

	switch(nControlID)
	{
		case window_common_edit:
		case winodw_common_button_01:
			{
				__int64 nTemp = _tstoi64(static_cast<LPCTSTR>(g_pUIManager->GetString(WINDOW_COMMON, window_common_edit)));

				if( nTemp > 2100000000)  // 21鞏 鞚挫儊 鞛呺牓 攵堦皜
				{
					g_MainCharInfo.ShowHelpMessage(IDS_MANY_MONEY, TEXTEFFECT_COLOR_WARNING);
					return;
				} // if( nTemp > 2100000000)

				DWORD dwMoney = static_cast<DWORD>(nTemp);

				if(dwMoney > 0)
				{
					if(g_PetList.GetPetInfoByIndex(0))
					{
						SendCS_NC_PETTRADE_REQ(0, g_MainCharInfo.m_dwObjectID, g_MainCharInfo.m_dwReserveID, g_PetList.GetPetInfoByIndex(0)->dwID, dwMoney);
						g_MainCharInfo.ShowHelpMessage(IDS_SEND_TRADE);
					}					

					g_MainCharInfo.CloseFrame(WINDOW_COMMON);
					g_pUIManager->SetReleaseFocus(WINDOW_COMMON, window_common_edit);
				}
			}
			break;

		case winodw_common_button_02:
			g_MainCharInfo.CloseFrame(WINDOW_COMMON);
			break;
		default:
			break;
	}
}

/**
* 韼 瓯半灅 鞝曤炒
* \param lParam 
*/
void ProcessWindowPetTrade(LPARAM lParam)
{
	int nControlID = LOWORD(lParam);

	switch(nControlID)
	{
	case window_pet_trade_button_01:
		SendCS_NC_PETTRADE_REQ(1, g_MainCharInfo.m_dwAskID, g_MainCharInfo.m_dwObjectID, g_MainCharInfo.m_dwReserveID, g_MainCharInfo.m_dwReserveMoney);
		break;
	case window_pet_trade_button_02:
		SendCS_NC_PETTRADE_REQ(9, g_MainCharInfo.m_dwAskID, g_MainCharInfo.m_dwObjectID, g_MainCharInfo.m_dwReserveID, g_MainCharInfo.m_dwReserveMoney);
		break;
	default:
		break;
	}

	g_MainCharInfo.CloseFrame(WINDOW_PET_TRADE);
}

/**
 * 氤店秾 氩堩樃
 * \param lParam 
 */
void ProcessWindowBokNumber(LPARAM lParam)
{
	int nControlID = LOWORD(lParam);

	switch(nControlID)
	{
	case bok_number_close_button:
	case bok_number_button_02:
		{
			g_MainCharInfo.CloseFrame(WINDOW_BOK_NUMBER);
		}
		break;
	case bok_number_button_01:	// 甑鞛
		{
			int nCount =0;

			// 靹犿儩霅滉皽靾 瓴靷
			for(int i=0; i < 25; ++i)
			{
				if(g_pUIManager->GetData(WINDOW_BOK_NUMBER, bok_number_number_button_01 + i, GET_CURRENT_INDEX) == 2)
					++nCount;
			}

			if(nCount == 4)	// 4臧 靹犿儩鞚措┐ OK
			{
				BYTE bySelect[4];
				nCount =0;

				for(int i=0; i < 25; ++i)
				{
					if(g_pUIManager->GetData(WINDOW_BOK_NUMBER, bok_number_number_button_01 + i, GET_CURRENT_INDEX) == 2)
					{
						bySelect[nCount] = static_cast<BYTE>(i+1);
						++nCount;
					}
				}

				SendCS_EC_BUYLOTTO_REQ(bySelect[0], bySelect[1], bySelect[2], bySelect[3]);

				g_MainCharInfo.CloseFrame(WINDOW_BOK_NUMBER);
			}
			else
			{
				if(nCount < 4)
				{
					// 4臧 氙胳劆韮
					g_MainCharInfo.ShowHelpMessage(IDS_LOTTO_SELECT_NOT, TEXTEFFECT_COLOR_WARNING);
				} // if(nCount < 4)
				else if(nCount > 4)
				{
					// 靹犿儩臧滌垬 齑堦臣
					g_MainCharInfo.ShowHelpMessage(IDS_LOTTO_SELECT_NOT_2, TEXTEFFECT_COLOR_WARNING);
				}
			}
		}
		break;
	case bok_number_button_03:	// 鞓堨儊雼轨波旮堨暋 須呺晬
		{
			SendCS_EC_LOTTOSALEINFO_REQ();
		}
		break;
	default:
		{
			// 氤店秾 氩堩樃鞚茧晫
			if(nControlID >= bok_number_number_button_01 && nControlID <= bok_number_number_button_25)
			{
				int nCount =0;

				// 靹犿儩霅滉皽靾 瓴靷
				for(int i=0; i < 25; ++i)
				{
					if(g_pUIManager->GetData(WINDOW_BOK_NUMBER, bok_number_number_button_01 + i, GET_CURRENT_INDEX) == 2)
						++nCount;
				}

				// 4臧滌澊靸侅澕 瓴届毎 雭
				if(nCount >= 4)
					break;

				if(g_pUIManager->GetData(WINDOW_BOK_NUMBER, nControlID, GET_CURRENT_INDEX) == -1)
                    g_pUIManager->SetData(WINDOW_BOK_NUMBER, nControlID, CURRENT_INDEX, 2);
			}
		}
		break;
	} // switch(nControlID)
}

/**
 * 氤店秾 雼轨波氩堩樃
 * \param lParam 
 */
void ProcessWindowBokPrize(LPARAM lParam)
{
	int nControlID = LOWORD(lParam);

	switch(nControlID)
	{
	case bok_prize_close_button:
		{
			g_MainCharInfo.CloseFrame(WINDOW_BOK_PRIZE);
		}
		break;
	case bok_prize_button_01:	// 歆雮滍殞彀 雼轨波氩堩樃
		{
			SendCS_EC_PRIZELOTTOINFO_REQ(0);
		}
		break;

	case bok_prize_button_02:	// 鞚措矆須岇皑 雼轨波氩堩樃
		{
			SendCS_EC_PRIZELOTTOINFO_REQ(1);
		}
		break;

	default:
		break;
	}
}

/**
 * 氍疙寣 毵堩伂 (鞎烄溂搿 雼るジ鞖╇弰搿 鞚挫毄臧電)
 * \param lParam 
 */
void ProcessWindowMark(LPARAM lParam)
{
	int nControlID = LOWORD(lParam);
	
	switch(nControlID)
	{
	case mark_window_button_01:		// 須呺晬
		{
			TCHAR strFile[128] = {0,};
			_stprintf(strFile, "mark.bmp");

			bool bCheck = false;
			D3DXIMAGE_INFO imageinfo;

Check:	// goto

			if(FAILED(D3DXGetImageInfoFromFile(strFile, &imageinfo)))
			{
				if(bCheck)
				{
					g_MainCharInfo.ShowHelpMessage(IDS_MUNPAMARK_NOTFILE, TEXTEFFECT_COLOR_WARNING);
					g_MainCharInfo.CloseFrame(MESSAGE_WINDOW_MARK);
					break;
				}

				// mark.bmp.bmp 韺岇澕霃 瓴靷 (鞚茧秬 鞙犾爛臧 韺岇澕鞚 鞚措爣瓴 毵岆摐電 瓴届毎臧 鞛堧嫟.)
				_stprintf(strFile, "mark.bmp.bmp");
				bCheck = true;

				goto Check;
			}
			else
			{
				// bmp毵
				if(imageinfo.ImageFileFormat != D3DXIFF_BMP)
				{
					g_MainCharInfo.ShowHelpMessage(IDS_MUNPAMARK_NOBMP, TEXTEFFECT_COLOR_WARNING);
					g_MainCharInfo.CloseFrame(MESSAGE_WINDOW_MARK);
					break;					
				}

				// 16x16韥旮半
				if(imageinfo.Width != 16 || imageinfo.Height != 16)
				{
					g_MainCharInfo.ShowHelpMessage(IDS_MUNPAMARK_MISTAKEN, TEXTEFFECT_COLOR_WARNING);
					g_MainCharInfo.CloseFrame(MESSAGE_WINDOW_MARK);
					break;					
				}

				// 24牍勴姼毵
				if(imageinfo.Format != D3DFMT_R8G8B8)
				{
					g_MainCharInfo.ShowHelpMessage(IDS_MUNPAMARK_24BPP, TEXTEFFECT_COLOR_WARNING);
					g_MainCharInfo.CloseFrame(MESSAGE_WINDOW_MARK);
					break;
				}				
			}

			FILE* fp = NULL;
			if((fp = _tfopen(strFile, _T("rb"))) != NULL)
			{
				// 靹滊矂鞐愲姅 項る摐毳 鞝滉卑頃 靾滌垬 鞚措胳毵 霌れ柎臧 鞛堧嫟.				
				// 鞚挫湢電 鞚措胳毳 氍胳瀽鞐措 氤措偞電旊嵃 0鞚措┐ 氍胳瀽鞐挫棎靹 null鞚搓赴鞐 氍胳牅臧 韥/靹滊矂 雼 靸濌复雼.
				// 攴鸽灅靹 項る摐電 鞝滉卑頃橁碃
				// 鞚措胳 0鞚 1搿 氤瓴

				BITMAPFILEHEADER BMPfileHeader;
				BITMAPINFOHEADER BMPinfoHeader;

				try
				{
					fread(&BMPfileHeader, sizeof(BITMAPFILEHEADER), 1, fp);
					fread(&BMPinfoHeader, sizeof(BITMAPINFOHEADER), 1, fp);

					fseek(fp, BMPfileHeader.bfOffBits, SEEK_SET);

					char buffer[770];
					fread(buffer, 768, 1, fp);
					fclose(fp);

					for(int i=0; i < 768; ++i)
					{
						if(buffer[i] == 0)
							buffer[i] = 1;
					}

					CXiahCharObject* pMainChar = reinterpret_cast<CXiahCharObject*>(g_pMainChar->m_pObject);

					if(pMainChar)
					{
						SendCS_RL_MUNPAMARKREG_REQ(0, pMainChar->m_dwMunpaID, (LPCTSTR)buffer, g_MainCharInfo.m_dwPickedObject);
						g_MainCharInfo.CloseFrame(MESSAGE_WINDOW_MARK);
					}
				}
				catch(...)
				{
					
				}							
			} // if((fp = _tfopen("mark.bmp", _T("rb"))) != NULL)
			else
			{
				g_MainCharInfo.ShowHelpMessage(IDS_MUNPAMARK_NOTOPEN, TEXTEFFECT_COLOR_WARNING);
				g_MainCharInfo.CloseFrame(MESSAGE_WINDOW_MARK);
			}
		}
		break;
	case mark_window_button_02:		// 旆靻
		{
			g_MainCharInfo.CloseFrame(MESSAGE_WINDOW_MARK);
		}
		break;
	default:
		break;
	}
}

/**
 * 臁绊暕彀
 * \param lParam 
 */
void ProcessWindowSmelt(LPARAM lParam)
{
	int nControlID = LOWORD(lParam);

	switch(nControlID)
	{
	case smelt_window_close_button:
	case smelt_window_button_02:
		{
			g_MainCharInfo.HideSack(SACKTYPE__SMELT);
		}
		break;
	case smelt_window_button_01:
		{
			if(!g_MainCharInfo.m_pSmeltSack)
				return;

			XiahItem::sItemInfo* pItemInfo = NULL;
			BYTE bItemType = 0;
			BYTE bItemKind = 0;
			BYTE bIsDividedRes = 0;
			int nItemCount = 0;

			// 頄夒偔 6 * 4 韥旮 瓴靷
			for(int i=0; i < 24; ++i)
			{
				pItemInfo = g_MainCharInfo.m_pSmeltSack->FindSackItemByPos(i);

				if(pItemInfo)
				{
					++nItemCount;

					// 毵濎箻 臁绊暕
					switch(pItemInfo->m_wRefID)
					{
					case 21039:
					case 21040:
					case 21041:
					case 20507:
						{
							SendCS_IM_MAKEREPAIRHAMMER_REQ(0);
							return;
						}
						break;
					default:
						break;
					}

					// 鞚措菠韸 韮鞛 氚 甏戨检诫...
					switch(pItemInfo->m_bItemType)
					{
					case ITEMTYPE_EVENT:
					case ITEMTYPE_REBUILDRES:
						// 雮, 靾
					case 18:
					case 20:
						{
							if(bItemType)
							{
								if(!(pItemInfo->m_bItemType == 18 || pItemInfo->m_bItemType == 20))
								{
									// 雼るジ 韮鞛呾澑歆 瓴靷
									if(bItemType != pItemInfo->m_bItemType)
									{
										g_MainCharInfo.ShowHelpMessage(IDS_MIXTURE_BADITEM, TEXTEFFECT_COLOR_WARNING);				
										g_MainCharInfo.HideSack(SACKTYPE__SMELT);

										return;
									}
								}
							}
							else
							{
								bItemType = pItemInfo->m_bItemType;
								bItemKind = pItemInfo->m_bItemKind;
								bIsDividedRes = pItemInfo->m_bIsDividedRes;
							}
						}
						break;
					case ITEMTYPE_MANUAL:
						{
							if(pItemInfo->m_wRefID >= 21064 && pItemInfo->m_wRefID <= 21083)
								SendCS_IM_MAKEUNIONITEM_REQ(0);
							else
								SendCS_IM_MAKEREBIRTHITEM_REQ();
							return;
						}
						break;
					}
				}
			}

			// 臁绊暕頃 鞎勳澊韰滌澊 鞐嗢潉鞁
			if(!nItemCount)
			{
				g_MainCharInfo.ShowHelpMessage(IDS_MIXTURE_ITEM, TEXTEFFECT_COLOR_WARNING);

				return;
			}

			if(bItemType == ITEMTYPE_EVENT)				// 9臧 歆霃, 7臧 氤挫劃
			{
				// [2/1/2005] 靹る偁 臧霝橂枴
				if(5 == bItemKind)
				{							
					SendCS_IM_VARIENTITEM_REQ(0);
				}
				else if(8 == bItemKind)	// [1/13/2006] 鞚措胳 臁绊暕
				{
					SendCS_IM_EVENTPUZZLE_REQ(g_MainCharInfo.m_dwPickedObject);
				}
				else if(10 == bItemKind)	//HT_0523 靹犽 靸侅瀽 臁绊暕
				{
					SendCS_IM_VARIENTITEM_REQ(2);
				}
				else
				{
					SendCS_IM_PUZZLEITEM_REQ(0);
				}					
			}
			else if(bItemType == ITEMTYPE_REBUILDRES)	// 攵
			{	
				if(bIsDividedRes != 1)
				{
					// 氤膦
					SendCS_IM_VARIENTITEM_REQ(1);
				}
				else
				{
					SendCS_IM_REJOINITEM_REQ(0);
				}					
			}
			else if(bItemType == 18 || bItemType == 20)	// 雮/靾 鞎勳澊韰 臁绊暕
			{
				SendCS_IM_MIXITEM_REQ(0);
			}
		}	
		break;
	}
}



/**
 * 鞓ろ枆
 * \param lParam 
 */
void ProcessWindowFiveElement(LPARAM lParam)
{
	int nControlID = LOWORD(lParam);	

	switch(nControlID)
	{
	case fiveelements_window_close_button:
		{
			g_MainCharInfo.CloseFrame(WINDOW_FIVEELEMENTS);

			if(g_pUIManager->IsShow(WINDOW_CHARACTER))
			{
				g_pUIManager->SetPosition(WINDOW_CHARACTER, WINDOW_FIRST_XPOS, 0);
			}
		}
		break;
	case fiveelements_window_top_button_01:
		ProcessClickMugongButton(1);
		break;
	case fiveelements_window_top_button_02:
		ProcessClickMugongButton(2);
		break;
	case fiveelements_window_top_button_03:
		break;
	case fiveelements_window_top_button_04:
		ProcessClickMugongButton(4);
		break;

		// 鞓ろ枆 靾橃箻 鞓毽旮
	case fiveelements_window_exp_up_01:
		{
			SendCS_IF_EXECFIVEELM_REQ(1);
		}		
		break;
	case fiveelements_window_exp_up_02:
		{
			SendCS_IF_EXECFIVEELM_REQ(2);
		}		
		break;
	case fiveelements_window_exp_up_03:
		{
			SendCS_IF_EXECFIVEELM_REQ(3);
		}		
		break;
	case fiveelements_window_exp_up_04:
		{
			SendCS_IF_EXECFIVEELM_REQ(4);
		}		
		break;
	case fiveelements_window_exp_up_05:
		{
			SendCS_IF_EXECFIVEELM_REQ(5);
		}		
		break;

		// 鞓ろ枆 靹犿儩
	case fiveelements_window_button_fire:
		{
			SendCS_IF_CHANGEFIVEELM_REQ(1);
		}
		break;
	case fiveelements_window_button_water:
		{
			SendCS_IF_CHANGEFIVEELM_REQ(2);
		}
		break;
	case fiveelements_window_button_tree:
		{
			SendCS_IF_CHANGEFIVEELM_REQ(3);
		}
		break;
	case fiveelements_window_button_metal:
		{
			SendCS_IF_CHANGEFIVEELM_REQ(4);
		}
		break;
	case fiveelements_window_button_earth:
		{
			SendCS_IF_CHANGEFIVEELM_REQ(5);
		}
		break;
	case fiveelements_window_button_end:
		{
			SendCS_IF_ENDFIVEELM_REQ();
		}

	default:
		{

		}	
		break;
	}
}

/**
 * 鞓ろ枆 鞝滊牗
 * \param lParam 
 */
void ProcessWindowFiveElementConvert(LPARAM lParam)
{
	int nControlID = LOWORD(lParam);	

	switch(nControlID)
	{
	case fiveelements_convert_window_close_button:
	case fiveelements_convert_window_button_02:
		{
			g_MainCharInfo.HideSack(SACKTYPE__FIVEELEMENT_CONVERT);
		}
		break;
	case fiveelements_convert_window_button_01:	
		{
			// 毵岇澕 霌り碃 鞛堧姅欷戩棎 臧滌“頃橂┐ 霌り碃鞛堧姅瓴冹潉 霅橂弻毽半嫟.
			if(NULL != g_MainCharInfo.m_pHoldItem)
			{
				g_MainCharInfo.m_pHoldItem->SetItemBackToSack();
			}

			if(!g_MainCharInfo.m_pFEConvert)
				return;

			XiahItem::sItemInfo* pItem = g_MainCharInfo.m_pFEConvert->FindSackItemByPos(0);

			if(pItem)
			{
				DWORD dwResourceID1 = 0;
				BYTE bResourceSackID1 =01;
				BYTE bResourcePos1 = 0;

				DWORD dwResourceID2 = 0;
				BYTE bResourceSackID2 = 0;
				BYTE bResourcePos2 = 255;

				DWORD dwResourceID3 = 0;
				BYTE bResourceSackID3 = 0;
				BYTE bResourcePos3 = 255;

				XiahItem::sItemInfo* pResourceItem = g_MainCharInfo.m_pFEConvert->FindSackItemByPos(1);
				if(pResourceItem)
				{
					dwResourceID1 = pResourceItem->m_dwItemID;
					bResourceSackID1 = pResourceItem->m_bSackIDPrev + 1;
					bResourcePos1 = pResourceItem->m_bSackPosPrev;
				}

				pResourceItem = g_MainCharInfo.m_pFEConvert->FindSackItemByPos(2);
				if(pResourceItem)
				{
					dwResourceID2 = pResourceItem->m_dwItemID;
					bResourceSackID2 = pResourceItem->m_bSackIDPrev + 1;
					bResourcePos2 = pResourceItem->m_bSackPosPrev;
				}

				pResourceItem = g_MainCharInfo.m_pFEConvert->FindSackItemByPos(3);
				if(pResourceItem)
				{
					dwResourceID3 = pResourceItem->m_dwItemID;
					bResourceSackID3 = pResourceItem->m_bSackIDPrev + 1;
					bResourcePos3 = pResourceItem->m_bSackPosPrev;
				}

				// 臧滌“鞛愳洂鞚 鞐嗢溂氅 臧滌“臧 攵堦皜電ロ晿雼
				if(NULL != g_MainCharInfo.m_pFEConvert->FindSackItemByPos(1))
				{					
					DWORD dwSackID = pItem->m_bSackIDPrev + 1;					

					SendCS_IM_REBUILDITEM_REQ(g_MainCharInfo.m_dwPickedObject,
											pItem->m_dwItemID, dwSackID, pItem->m_bSackPosPrev,
											dwResourceID1, bResourceSackID1, bResourcePos1,
											dwResourceID2, bResourceSackID2, bResourcePos2,
											dwResourceID3, bResourceSackID3, bResourcePos3);
				}
			}
			else
			{				
				g_MainCharInfo.ShowHelpMessage(IDS_PUT_CONVERTITEM, TEXTEFFECT_COLOR_WARNING);
			}
		}
		break;
	}
}

/**
 * 雽頇 雮挫毄
 * \param lParam 
 */
//HO_0410_07 靸侅劀霠 臧鞚措摐 鞐呺嵃鞚错姼
void ProcessWindowHelperScript(LPARAM lParam)
{
	int nControlID = LOWORD(lParam);

	switch(nControlID)
	{
	case helper_window_close_button:
		g_MainCharInfo.CloseFrame( WINDOW_HELPER_LIST);
		g_MainCharInfo.CloseFrame( WINDOW_HELPER_LIST1);
		break;
	case helper_window_button_01:
		g_Helper.TalkContinue();
		break;
	case helper_window_button_02:
		g_Helper.TalkStop();
		break;
	default:
		break;
	}
}

/**
 * 雽頇 雮挫毄1
 * \param lParam 
 */
void ProcessWindowHelperList(LPARAM lParam)//HO_0410_07 靸侅劀霠 臧鞚措摐 鞐呺嵃鞚错姼 
{
	int nControlID = LOWORD(lParam);

	switch(nControlID)
	{
	case helper_window1_close_button:
		g_MainCharInfo.CloseFrame( WINDOW_HELPER_LIST);
		g_MainCharInfo.CloseFrame( WINDOW_HELPER_LIST1);
		break;
	case helper_window1_button_01:
		g_Helper.TalkContinue();
		break;
	case helper_window1_button_02:
		g_Helper.TalkStop();
		break;
	default:
		break;
	}
}

/**
 * 雽頇 雮挫毄
 * \param lParam 
 */
//HO_0410_07 靸侅劀霠 臧鞚措摐 鞐呺嵃鞚错姼 : 鞐呺嵃鞚错姼 鞝侅毄鞝 鞀ろ伂毽巾姼毳 韮愲瀾 鞀ろ伂毽巾姼搿 氤頇
void ProcessWindowTamRangScript(LPARAM lParam)
{
	int nControlID = LOWORD(lParam);

	switch(nControlID)
	{
	case helper_window_script_button1:
		{
			g_Helper.TalkContinue();
		}
		break;
	case helper_window_script_button2:
		{
			g_Helper.TalkStop();
		}
		break;
	default:
		break;
	}
}

void ProcessWindowQuickScript(LPARAM lParam)//HO_0413 : 韤 臧鞚措摐 鞀ろ伂毽巾姼
{
	int nControlID = LOWORD(lParam);

	switch(nControlID)
	{
	case helper_window0_close_button:
		g_MainCharInfo.CloseFrame( WINDOW_HELPER_LIST0);
		g_pUIManager->Show(HELP_BUTTON);
		break;
	case helper_window0_button_01:
		g_Helper.TalkBack();
		break;
	case helper_window0_button_02:
		g_Helper.TalkNext();
		break;
	default:
		break;
	}
}


/**
 * 鞎勳澊韰 氤店惮
 * \param lParam 
 */
#include <vector>

struct sPetDisplayInfo {
	DWORD dwID;
	sString szName;
	WORD wLevel;
	BOOL isActive;
	BOOL isFromSack;
	BYTE bSackID;
	BYTE bSackPos;
	BYTE bSackCount;
	DWORD dwHpCur;
};

std::vector<sPetDisplayInfo> g_PetDisplayList;

void ScanSackForPets(CSack* pSack)
{
	if (!pSack) return;
	const ItemList& items = pSack->GetItemList();
	for (size_t i = 0; i < items.size(); ++i)
	{
		XiahItem::sItemInfo* pItem = items[i];
		if (pItem && pItem->m_bItemType == 9 && pItem->m_bItemKind == 5)
		{
			if (pItem->m_dwNpcID > 0 && pItem->m_wTamingLevel > 0)
			{
				BOOL alreadyActive = FALSE;
				for (size_t k = 0; k < g_PetDisplayList.size(); ++k)
				{
					if (g_PetDisplayList[k].dwID == pItem->m_dwItemID)
					{
						alreadyActive = TRUE;
						break;
					}
				}
				if (!alreadyActive)
				{
					sPetDisplayInfo disp;
					disp.dwID = pItem->m_dwItemID;
					disp.szName = pItem->m_szName;
					disp.wLevel = pItem->m_wLevel;
					disp.isActive = FALSE;
					disp.isFromSack = TRUE;
					disp.bSackID = pItem->m_bSackID;
					disp.bSackPos = pItem->m_bSackPos;
					disp.bSackCount = pItem->m_bSackCount;
					disp.dwHpCur = 1; // 封印中设为默认非零值
					g_PetDisplayList.push_back(disp);
				}
			}
		}
	}
}

std::vector<sPetInfo> g_MyPetList;

void RebuildPetDisplayList()
{
	g_PetDisplayList.clear();

	int nMySize = g_MyPetList.size();
	for(int i = 0; i < nMySize; ++i)
	{
		sPetInfo& myPet = g_MyPetList[i];
		sPetDisplayInfo disp;
		disp.dwID = myPet.dwID;
		disp.szName = myPet.szName;
		disp.wLevel = myPet.wLevel;
		disp.isActive = (g_PetList.Find(myPet.dwID) != NULL || g_PetList.Find(myPet.dwID + 800000000) != NULL);
		disp.isFromSack = FALSE;
		disp.bSackID = 0;
		disp.bSackPos = 0;
		disp.bSackCount = 0;
		disp.dwHpCur = myPet.dwHpCur;
		g_PetDisplayList.push_back(disp);
	}

	for (int s = 0; s < 3; ++s)
	{
		if (g_MainCharInfo.m_pMySack[s])
		{
			ScanSackForPets(g_MainCharInfo.m_pMySack[s]);
		}
	}
}

void UpdateRecoveryButtons()
{
	int selIdx = g_MainCharInfo.m_nTempValue;
	int nSize = g_PetDisplayList.size();

	if (selIdx >= 0 && selIdx < nSize)
	{
		sPetDisplayInfo& disp = g_PetDisplayList[selIdx];
		if (disp.isActive)
		{
			g_pUIManager->Show(WINDOW_RECOVERY, recovery_window_button01);
			g_pUIManager->SetString(WINDOW_RECOVERY, recovery_window_button01, _T("\xd5\xd9\xbb\xd8")); // "召回"
		}
		else if (disp.isFromSack)
		{
			g_pUIManager->Hide(WINDOW_RECOVERY, recovery_window_button01);
		}
		else
		{
			g_pUIManager->Show(WINDOW_RECOVERY, recovery_window_button01);
			g_pUIManager->SetString(WINDOW_RECOVERY, recovery_window_button01, _T("\xb3\xf6\xd5\xbd")); // "出战"
		}
	}
	else
	{
		g_pUIManager->Hide(WINDOW_RECOVERY, recovery_window_button01);
	}
}

void UpdatePetManagerList()
{
	if (!g_pUIManager->IsShow(WINDOW_RECOVERY)) return;

	RebuildPetDisplayList();

	for(int i=0; i < 6; ++i)
	{
		g_pUIManager->SetString(WINDOW_RECOVERY, recovery_window_back_dummy01+i, _T(" "));
		g_pUIManager->SetData(WINDOW_RECOVERY, recovery_window_back_dummy01+i, COLOR, 0, 0, D3DCOLOR_XRGB(255, 255, 255));
	}

	int nSize = g_PetDisplayList.size();
	for(int i=0; i < nSize && i < 6; ++i)
	{
		sPetDisplayInfo& disp = g_PetDisplayList[i];
		LPCTSTR szStatus;
		D3DCOLOR textColor = D3DCOLOR_XRGB(255, 255, 255);
		if (disp.dwHpCur == 0 && !disp.isFromSack) {
			szStatus = _T("[\xcb\xc0\xcd\xf6]"); // "[死亡]"
			textColor = D3DCOLOR_XRGB(255, 64, 64);
		} else {
			szStatus = disp.isActive ? _T("[\xb3\xf6\xd5\xbd]") : (disp.isFromSack ? _T("[\xb7\xe2\xd3\xa1\xd6\xd0]") : _T("[\xd0\xdd\xcf\xa2]"));
		}
		TCHAR szTemp[128] = {0,};
		_stprintf(szTemp, _T("%s %s   Lv.%d"), szStatus, disp.szName.data(), disp.wLevel);
		g_pUIManager->SetString(WINDOW_RECOVERY, recovery_window_back_dummy01+i, szTemp);
		g_pUIManager->SetData(WINDOW_RECOVERY, recovery_window_back_dummy01+i, COLOR, 0, 0, textColor);
	}

	if (g_MainCharInfo.m_nTempValue >= 0 && g_MainCharInfo.m_nTempValue < nSize)
	{
		g_pUIManager->SetData(WINDOW_RECOVERY, recovery_window_back_dummy01+g_MainCharInfo.m_nTempValue, COLOR, 0, 0, D3DCOLOR_XRGB(250, 250, 0));
	}
	UpdateRecoveryButtons();
}

void ProcessWindowRecovery(LPARAM lParam)
{
	int nControlID = LOWORD(lParam);

	switch(nControlID)
	{
	case recovery_window_back_dummy01:
	case recovery_window_back_dummy02:
	case recovery_window_back_dummy03:
	case recovery_window_back_dummy04:
	case recovery_window_back_dummy05:
	case recovery_window_back_dummy06:
		{
			for(int i=0; i < 6; ++i)
			{
				g_pUIManager->SetData(WINDOW_RECOVERY, recovery_window_back_dummy01+i, COLOR, 0, 0, D3DCOLOR_XRGB(255, 255, 255));
			}

			g_MainCharInfo.m_nTempValue = nControlID - recovery_window_back_dummy01;
			int nSize = g_PetDisplayList.size();

			if(nSize)
			{
				if(g_MainCharInfo.m_nTempValue < nSize)
				{
					g_pUIManager->SetData(WINDOW_RECOVERY, recovery_window_back_dummy01+g_MainCharInfo.m_nTempValue, COLOR, 0, 0, D3DCOLOR_XRGB(250, 250, 0));
					g_MainCharInfo.m_dwResItemID = g_PetDisplayList[g_MainCharInfo.m_nTempValue].dwID;
				}
				else
				{
					g_MainCharInfo.m_dwResItemID = 0;
				}
			}
			UpdateRecoveryButtons();
		}
		break;
	case recovery_window_button01:
		{
			if(g_MainCharInfo.m_dwResItemID && g_PetDisplayList.size())
			{
				if(g_MainCharInfo.m_nTempValue >= 0 && g_MainCharInfo.m_nTempValue < (int)g_PetDisplayList.size())
				{
					sPetDisplayInfo& disp = g_PetDisplayList[g_MainCharInfo.m_nTempValue];
					if (disp.isActive)
					{
						extern void SendCS_NC_PET_CONTROL_REQ(DWORD dwPetID, BYTE bAction);
						SendCS_NC_PET_CONTROL_REQ(disp.dwID, 0);
					}
					else if (disp.isFromSack)
					{
						extern void SendCS_NC_PETBONGOUT_REQ(BYTE bSackID, BYTE bSackPos);
						SendCS_NC_PETBONGOUT_REQ(disp.bSackID, disp.bSackPos);
					}
					else
					{
						extern void SendCS_NC_PET_CONTROL_REQ(DWORD dwPetID, BYTE bAction);
						SendCS_NC_PET_CONTROL_REQ(disp.dwID, 1);
					}
				}
			}
		}
		break;
	case recovery_window_button02:
		{
			if(g_MainCharInfo.m_dwResItemID && g_PetDisplayList.size())
			{
				if(g_MainCharInfo.m_nTempValue >= 0 && g_MainCharInfo.m_nTempValue < (int)g_PetDisplayList.size())
				{
					sPetDisplayInfo& disp = g_PetDisplayList[g_MainCharInfo.m_nTempValue];
					TCHAR szTemp[256] = {0,};
					_stprintf(szTemp, _T("\xc8\xb7\xb6\xa8\xd2\xaa\xb7\xc5\xc9\xfa\xd5\xbd\xb3\xe8\x20%s\x20\xc2\xf0\xa3\xbf\xb7\xc5\xc9\xfa\xba\xf3\xbd\xab\xd3\xc0\xd4\xb6\xca\xa7\xc8\xa5\xcb\xfc\xa1\xa3"), disp.szName.data());
					g_pUIManager->ShowNotice(szTemp, NOTICE_FRAME_OKCANCEL, NOTICE_FRAME_RECOVERY2, XiahInput::g_ptMouse.x - 270, XiahInput::g_ptMouse.y-90);
				}
			}
		}
		break;
	case recovery_title_close_button:
		{
			g_MainCharInfo.CloseFrame(WINDOW_RECOVERY);
		}
		break;
	case recovery_window_button03:
		{
			if(g_MainCharInfo.m_dwResItemID && g_PetDisplayList.size())
			{
				if(g_MainCharInfo.m_nTempValue >= 0 && g_MainCharInfo.m_nTempValue < (int)g_PetDisplayList.size())
				{
					sPetDisplayInfo& disp = g_PetDisplayList[g_MainCharInfo.m_nTempValue];
					if (disp.isActive)
					{
						g_pUIManager->SetPosition(WINDOW_NEW_TAMING, 0, 0);
						g_MainCharInfo.OpenFrame(WINDOW_NEW_TAMING);
						g_MainCharInfo.ShowSack(SACKTYPE__PET_EQUIP);
						g_MainCharInfo.RefreshPetInfo();
					}
					else
					{
						TCHAR szErr[128];
						_stprintf(szErr, _T("\xd6\xbb\xc4\xdc\xb2\xe9\xbf\xb4\xd2\xd1\xb3\xf6\xd5\xbd\xd5\xbd\xb3\xe8\xb5\xc4\xca\xf4\xd0\xd4"));
						g_MainCharInfo.ShowHelpMessage(szErr, TEXTEFFECT_COLOR_WARNING);
					}
				}
			}
		}
		break;
	default:
		break;
	}
}


/**
* 雼 瓴巾棙旃 攵勲鞍
* \param lParam 
*/
void ProcessWindowDanNew(LPARAM lParam)
{
	int nControlID = LOWORD(lParam);

	g_MainCharInfo.PlayInterfaceSound(ISOUND_SELECT_BUTTON);

	switch( nControlID)
	{
	case window_dan_new_close_button:
		{
			g_MainCharInfo.CloseFrame(WINDOW_DAN_NEW);
		}		
		break;
	case window_dan_new_button1:	// 雼
		//g_MainCharInfo.m_pRelation->SetCurrType( eDAN);
		break;
	case window_dan_new_button2:	// 鞚胳棸
		g_MainCharInfo.m_pRelation->SetCurrType( eShip);
		break;
	case window_dan_new_button3:	// 氍疙寣
		g_MainCharInfo.m_pRelation->SetCurrType( eClan);
		break;
	case window_dan_new_2button_01:
		{
			switch( g_MainCharInfo.m_pRelation->GetCurrType())
			{
			case eDAN:	// 鞝滊獏
				g_pUIManager->ShowNotice( IDS_Q_JEMYUNG, NOTICE_FRAME_OKCANCEL, NOTICE_FRAME_JEMYUNG);
				break;
			case eShip:	// 鞝勳劀甑				
				break;
			case eClan:
				break;
			}
		}
		break;
	case window_dan_new_2button_02:
		{
			//HT_0423 : 雼欤 鞙勳瀯
			switch( g_MainCharInfo.m_pRelation->GetCurrType())
			{
			case eDAN:	// 韮堩嚧
				SendCS_IF_LEAVEPARTY_REQ( g_MainCharInfo.m_pRelation->GetDanID());
				break;
			case eShip:	// 鞚胳棸雭婈赴
				break;
			case eClan:
				break;
			}
		}
		break;
	case window_dan_new_1button:
		{
			//HT_0423 : 雼欤 鞙勳瀯
			if( g_MainCharInfo.m_pRelation->Am_I_LeaderInDan())
			{
				if(g_MainCharInfo.m_pRelation->GetCurrRelation())
				{
					TCHAR szText[100];

					_stprintf( szText, IDS_DANCOMMIT_ASK, (LPCTSTR)g_MainCharInfo.FindNameByID( g_MainCharInfo.m_pRelation->GetCurrRelation() ) );
					g_pUIManager->ShowNotice( szText, NOTICE_FRAME_OKCANCEL, NOTiCE_FRAME_DANCOMMIT );
				}
			}
			else
			{
				switch( g_MainCharInfo.m_pRelation->GetCurrType())
				{
				case eDAN:	// 韮堩嚧
					SendCS_IF_LEAVEPARTY_REQ( g_MainCharInfo.m_pRelation->GetDanID());
					break;
				case eShip:	// 鞚胳棸雭婈赴
					break;
				case eClan:
					break;
				}
			}
		}
		break;
	case window_dan_new_button_back:
		{
		}
		break;
	case window_dan_new_button_front:
		{
		}
		break;
	}
}


/**
 * NPC 韽韮 鞚措彊
 * \param lParam 
 */
void ProcessWindowPortal(LPARAM lParam)
{
	int nControlID = LOWORD(lParam);
	int nEventType = HIWORD(lParam);

	switch(nControlID)
	{
	case window_portal_button1:
		{
			if(nEventType == WINDOW_NPC_PORTAL)
			{
				// 頇旍偘 歆雽 鞚措彊
				SendCS_NV_QUICKMOVE_REQ(2);
			}
			else if(nEventType == WINDOW_NPC_PORTAL__WAR)
			{
				// 韱 靻嶌劚 歆鞐
				SendCS_NV_PRIVATEPORTAL_REQ(0);
			}

			g_MainCharInfo.CloseFrame(WINDOW_PORTAL);
		}
		break;
	case window_portal_button2:
		{
			if(nEventType == WINDOW_NPC_PORTAL)
			{
				// 電 歆雽 鞚措彊
				SendCS_NV_QUICKMOVE_REQ(5);
			}
			else if(nEventType == WINDOW_NPC_PORTAL__WAR)
			{
				// 氇 靻嶌劚 歆鞐
				SendCS_NV_PRIVATEPORTAL_REQ(1);
			}

			g_MainCharInfo.CloseFrame(WINDOW_PORTAL);
		}
		break;
	case window_portal_button3:
		{
			if(nEventType == WINDOW_NPC_PORTAL__WAR)
			{
				// 旮 靻嶌劚 歆鞐
				SendCS_NV_PRIVATEPORTAL_REQ(2);
			}

			g_MainCharInfo.CloseFrame(WINDOW_PORTAL);
		}
		break;
	case window_portal_button4:
		{
			if(nEventType == WINDOW_NPC_PORTAL__WAR)
			{
				// 頇 靻嶌劚 歆鞐
				SendCS_NV_PRIVATEPORTAL_REQ(3);
			}

			g_MainCharInfo.CloseFrame(WINDOW_PORTAL);
		}
		break;
	
	case window_portal_button5: //HO_0906_07 氍疙寣雽鞝 甏毽鞚 韽韮堦赴電 於旉皜 : 毵堩槇歆 歆鞐
		{
			if(nEventType == WINDOW_NPC_PORTAL__WAR)
			{
				// 毵堩槇歆 歆鞐				
				SendCS_NV_PRIVATEPORTAL_REQ(4);
			}

			g_MainCharInfo.CloseFrame(WINDOW_PORTAL);
		}
		break;
	case window_portal_exit_button:		// 彀 雼旮
		{
			g_MainCharInfo.CloseFrame(WINDOW_PORTAL);			
		}
		break;
	default:
		break;
	}
}

/**
* 鞎勳澊韰 靾橃
* \param lParam 
*/
void ProcessWindowCollection(LPARAM lParam)
{
	int nControlID = LOWORD(lParam);

	switch(nControlID)
	{
	case collection_window_close_button:
		{
			g_MainCharInfo.HideSack(SACKTYPE__COLLECTION);
		}
		break;
	default:
		break;
	}
}
/**
* 臧侅劚
* \param lParam 
*/
void ProcessWindowSkill( LPARAM lParam)
{
	int controlID = LOWORD( lParam);

	switch( controlID)
	{
	case skill_window_close_button:
		{
			g_MainCharInfo.CloseFrame( WINDOW_SKILL);

			if(g_pUIManager->IsShow(WINDOW_CHARACTER))
			{
				g_pUIManager->SetPosition(WINDOW_CHARACTER, WINDOW_FIRST_XPOS, 0);
			}
		}
		break;
	case skill_window_top_button_01:
		ProcessClickMugongButton(1);
		break;
	case skill_window_top_button_02:
		ProcessClickMugongButton(2);
		break;
	case skill_window_top_button_03:
		ProcessClickMugongButton(3);
		break;
	case skill_window_top_button_04:		// 臧侅劚
		break;

	case skill_window_megong_point_up_01:
	case skill_window_megong_point_up_02:
	case skill_window_megong_point_up_03:

	case skill_window_mugong_point_up_01:
	case skill_window_mugong_point_up_02:
	case skill_window_mugong_point_up_03:
	case skill_window_mugong_point_up_04:
		{
			BYTE bySeq = 0;
			/* 1 ~ 3 */
			if(controlID >= skill_window_megong_point_up_01 && controlID <= skill_window_megong_point_up_03)
			{
				bySeq = controlID - 36;
			}
			/* 4 ~ 7 */
			else if(controlID >= skill_window_mugong_point_up_01 && controlID <= skill_window_mugong_point_up_04)
			{
				bySeq = controlID - skill_window_mugong_point_up_01;
				bySeq += 4;
			}			

			DWORD dwMugongID = g_MainCharInfo.m_pMugong->FindRebirthMugongByIndex(bySeq);

			if(dwMugongID)
			{
				g_MainCharInfo.PlayInterfaceSound( ISOUND_SELECT_BUTTON);
				SendCS_BT_LEARNMUGONG_REQ( dwMugongID);
			}
			else
			{
				g_MainCharInfo.ShowHelpMessage( IDS_NO_MUGONG, TEXTEFFECT_COLOR_WARNING);
			}
		}
		break;

	default:
		break;
	}
}

/**
 * HT_0313 : 甏戨獏鞝 & 觳滍櫓鞝 (鞚措彊)	
 * \param lParam 
 */
void ProcessWindowSecretMove(LPARAM lParam)
{
	int nControlID = LOWORD(lParam);

	switch(nControlID)
	{
	case secret_check_window_button01:
		{
			g_Helper.SecretMove();
		}
		break;
	case secret_check_window_button02:
		{
			g_Helper.SecretCancle();
		}
		break;
	default:
		break;
	}
}

/**
 * HT_0313 : 甏戨獏鞝 & 觳滍櫓鞝 (彀胳棳)
 * \param lParam 
 */
void ProcessWindowSecretApplication(LPARAM lParam)
{
	int nControlID = LOWORD(lParam);

	switch(nControlID)
	{
	case secret_information_window_button01:
		{
			g_Helper.SecretApplication();
		}
		break;
	case secret_information_window_button02:
		{
			g_Helper.SecretCancle();
		}
		break;
	default:
		break;
	}
}


// =========================================================================
// =========================================================================
// 拍卖行窗口 (Frame 193) 控制与端服协同实现
// =========================================================================

#include "XiahGame_Handler_Sender.h"

extern void RefreshAuctionWindowDisplay();
extern int g_nAuctionSelectedIndex;

void ProcessWindowAuction(LPARAM lParam)
{
	int nControlID = LOWORD(lParam);

	// 1. 物品列表行点击事件 (实例 2: auction_window_list_01)
	if (nControlID == auction_window_list_01)
	{
		if (!g_AuctionClientList.empty())
		{
			if (g_nAuctionSelectedIndex < 0)
				g_nAuctionSelectedIndex = 0;
			else
				g_nAuctionSelectedIndex = (g_nAuctionSelectedIndex + 1) % (int)g_AuctionClientList.size();

			g_pUIManager->SetData(WINDOW_AUCTION, auction_window_list_01, CURRENT_INDEX, 1);
		}
		else
		{
			g_pUIManager->SetData(WINDOW_AUCTION, auction_window_list_01, CURRENT_INDEX, 0);
		}

		// 刷新拍卖行数据展示
		RefreshAuctionWindowDisplay();
	}
	// 2. 物品类型分类切换 (实例 10: auction_window_filter_0, 19~23: auction_window_filter_1~5)
	else if (nControlID == auction_window_filter_0)
	{
		g_bAuctionFilterType = 0;
		g_wAuctionCurrentPage = 0;
		RefreshAuctionWindowDisplay();
		SendCS_AH_QUERY_REQ(0, g_bAuctionFilterType, g_strAuctionKeyword);
	}
	else if (nControlID == auction_window_filter_1)
	{
		g_bAuctionFilterType = 1;
		g_wAuctionCurrentPage = 0;
		RefreshAuctionWindowDisplay();
		SendCS_AH_QUERY_REQ(0, g_bAuctionFilterType, g_strAuctionKeyword);
	}
	else if (nControlID == auction_window_filter_2)
	{
		g_bAuctionFilterType = 2;
		g_wAuctionCurrentPage = 0;
		RefreshAuctionWindowDisplay();
		SendCS_AH_QUERY_REQ(0, g_bAuctionFilterType, g_strAuctionKeyword);
	}
	else if (nControlID == auction_window_filter_3)
	{
		g_bAuctionFilterType = 3;
		g_wAuctionCurrentPage = 0;
		RefreshAuctionWindowDisplay();
		SendCS_AH_QUERY_REQ(0, g_bAuctionFilterType, g_strAuctionKeyword);
	}
	else if (nControlID == auction_window_filter_4)
	{
		g_bAuctionFilterType = 4;
		g_wAuctionCurrentPage = 0;
		RefreshAuctionWindowDisplay();
		SendCS_AH_QUERY_REQ(0, g_bAuctionFilterType, g_strAuctionKeyword);
	}
	else if (nControlID == auction_window_filter_5)
	{
		g_bAuctionFilterType = 5;
		g_wAuctionCurrentPage = 0;
		RefreshAuctionWindowDisplay();
		SendCS_AH_QUERY_REQ(0, g_bAuctionFilterType, g_strAuctionKeyword);
	}
	// 3. 竞价按钮 (实例 15: auction_window_btn_bid)
	else if (nControlID == auction_window_btn_bid)
	{
		if (g_AuctionClientList.empty() || g_nAuctionSelectedIndex < 0 || g_nAuctionSelectedIndex >= (int)g_AuctionClientList.size())
		{
			g_MainCharInfo.ShowHelpMessage(_T("请先选择要竞价的拍卖品"), TEXTEFFECT_COLOR_WARNING);
			return;
		}

		const sAuctionClientItem& it = g_AuctionClientList[g_nAuctionSelectedIndex];

		// 从实例 14 (auction_window_bid_edit) 获取玩家输入的竞价金额
		sString strBid = g_pUIManager->GetString(WINDOW_AUCTION, auction_window_bid_edit);
		DWORD dwMinBid = (it.dwCutPrice > 0 ? it.dwCutPrice + 1 : it.dwBasicPrice);
		DWORD dwBidPrice = 0;

		// 若输入框为空，则自动以最低有效出价参与竞拍；否则按玩家填写的金额
		if (strBid.empty())
		{
			dwBidPrice = dwMinBid;
		}
		else
		{
			dwBidPrice = (DWORD)_tstoi64(strBid.c_str());
		}

		if (dwBidPrice < dwMinBid)
		{
			TCHAR szErr[128];
			_stprintf(szErr, _T("竞价金额过低，当前最低出价为 %s 金币"), MoneyCommaStr(dwMinBid).c_str());
			g_MainCharInfo.ShowHelpMessage(szErr, TEXTEFFECT_COLOR_WARNING);
			return;
		}

		if (g_MainCharInfo.m_dwMoney < dwBidPrice)
		{
			g_MainCharInfo.ShowHelpMessage(_T("金币不足，无法参与竞价"), TEXTEFFECT_COLOR_WARNING);
			return;
		}

		SendCS_AH_BID_REQ(it.dwAuctionID, dwBidPrice);
	}
	// 5. 一口价购买按钮 (实例 16: auction_window_btn_buyout)
	else if (nControlID == auction_window_btn_buyout)
	{
		if (g_AuctionClientList.empty() || g_nAuctionSelectedIndex < 0 || g_nAuctionSelectedIndex >= (int)g_AuctionClientList.size())
		{
			g_MainCharInfo.ShowHelpMessage(_T("请先选择要购买的物品"), TEXTEFFECT_COLOR_WARNING);
			return;
		}

		const sAuctionClientItem& it = g_AuctionClientList[g_nAuctionSelectedIndex];

		if (it.dwOnePrice == 0)
		{
			g_MainCharInfo.ShowHelpMessage(_T("该物品未设置一口价，仅供竞拍"), TEXTEFFECT_COLOR_WARNING);
			return;
		}

		if (g_MainCharInfo.m_dwMoney < it.dwOnePrice)
		{
			g_MainCharInfo.ShowHelpMessage(_T("金币不足，无法一口价购买"), TEXTEFFECT_COLOR_WARNING);
			return;
		}

		SendCS_AH_BUYOUT_REQ(it.dwAuctionID);
	}
	// 5. 上一页翻页按钮 (实例 17: auction_window_btn_prev_page)
	else if (nControlID == auction_window_btn_prev_page)
	{
		if (g_wAuctionCurrentPage > 0)
		{
			g_wAuctionCurrentPage--;
			SendCS_AH_QUERY_REQ(g_wAuctionCurrentPage, g_bAuctionFilterType, g_strAuctionKeyword);
		}
		else
		{
			g_MainCharInfo.ShowHelpMessage(_T("已经是第一页了"));
		}
	}
	// 6. 下一页翻页按钮 (实例 18: auction_window_btn_next_page)
	else if (nControlID == auction_window_btn_next_page)
	{
		int nPageSize = 10;
		int nTotalPages = (g_wAuctionTotalCount + nPageSize - 1) / nPageSize;
		if (nTotalPages <= 0) nTotalPages = 1;

		if ((int)g_wAuctionCurrentPage + 1 < nTotalPages)
		{
			g_wAuctionCurrentPage++;
			SendCS_AH_QUERY_REQ(g_wAuctionCurrentPage, g_bAuctionFilterType, g_strAuctionKeyword);
		}
		else
		{
			g_MainCharInfo.ShowHelpMessage(_T("已经是最后一页了"));
		}
	}
}

void OpenAuctionWindow()
{
	if (!g_pUIManager) return;

	if (g_pUIManager->IsShow(WINDOW_AUCTION))
	{
		g_pUIManager->Hide(WINDOW_AUCTION);
		return;
	}

	// 1. 设置窗口坐标 (居中偏左：X=50, Y=100)
	g_pUIManager->SetPosition(WINDOW_AUCTION, 50, 100);
	g_pUIManager->ForwardShow(WINDOW_AUCTION);

	// 2. 初始化列表行状态 (实例 2 默认高亮状态 1)
	g_pUIManager->SetData(WINDOW_AUCTION, auction_window_list_01, CURRENT_INDEX, 1);

	// 3. 初始设置分类按钮、操作按钮文字与输入框默认文字
	g_pUIManager->SetString(WINDOW_AUCTION, auction_window_filter_0, _T("全部物品"));
	g_pUIManager->SetString(WINDOW_AUCTION, auction_window_filter_1, _T("武器装备"));
	g_pUIManager->SetString(WINDOW_AUCTION, auction_window_filter_2, _T("防具防具"));
	g_pUIManager->SetString(WINDOW_AUCTION, auction_window_filter_3, _T("首饰宝物"));
	g_pUIManager->SetString(WINDOW_AUCTION, auction_window_filter_4, _T("药品秘籍"));
	g_pUIManager->SetString(WINDOW_AUCTION, auction_window_filter_5, _T("其它物品"));
	g_pUIManager->SetString(WINDOW_AUCTION, auction_window_bid_label, _T("竞价"));
	g_pUIManager->SetString(WINDOW_AUCTION, auction_window_btn_bid, _T("竞价"));
	g_pUIManager->SetString(WINDOW_AUCTION, auction_window_btn_buyout, _T("一口价"));
	g_pUIManager->SetString(WINDOW_AUCTION, auction_window_bid_edit, _T(""));

	// 4. 刷新金币与控件状态
	RefreshAuctionWindowDisplay();

	// 5. 向服务端发送请求拉取最新实时拍卖列表 (CS_AH_QUERY_REQ 0x3121)
	SendCS_AH_QUERY_REQ(g_wAuctionCurrentPage, g_bAuctionFilterType, g_strAuctionKeyword);
}

