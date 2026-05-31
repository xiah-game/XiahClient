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
	case 51: // �ƺ�ϵͳ���ع���ͼ��������ֵ�������л������ΰ�ť��ͨ������ 51 �ŷ����Ĭ֪ͨ�����������л��ƺ�
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
				// 매품�
				if(g_MainCharInfo.m_pQuickMart)
				{
					g_MainCharInfo.HideSack(SACKTYPE__QUICKMART, FALSE);					
				}

				g_MainCharInfo.HideSack( SACKTYPE__DEFAULT);
				g_MainCharInfo.HideSack( SACKTYPE__EQUIPMENT);
				g_MainCharInfo.HideSack( SACKTYPE__NPC_TRADE);
				g_MainCharInfo.HideSack( SACKTYPE__DEPOSIT);
				g_MainCharInfo.HideSack( SACKTYPE__MODIFY);					// 개조
				g_MainCharInfo.HideSack( SACKTYPE__PERSONAL_TRADE_SET);		// 개인상점설정
				g_MainCharInfo.HideSack( SACKTYPE__PERSONAL_TRADE_SELL);	// 개인상점판매
				g_MainCharInfo.HideSack( SACKTYPE__ITEMMALL);				// 아이템�
				g_MainCharInfo.HideSack( SACKTYPE__SMELT);					// 조합
				g_MainCharInfo.HideSack( SACKTYPE__FIVEELEMENT_CONVERT);	// 오행 제련

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
				else if(g_MainCharInfo.m_bPersonalTradeSell)  // 개인상점 판매중이� 우측으�
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
	case pc_trade_window_button_02:	// ��
		SendCS_EC_TRADEITEM_REQ( 9, g_MainCharInfo.m_dwAskID);
		break;
	case pc_trade_window_button_01:	// 오케�
		SendCS_EC_TRADEITEM_REQ( 0, g_MainCharInfo.m_dwAskID);

		g_pUIManager->Hide(WINDOW_PC_TRADE, pc_trade_window_button_01);
		g_pUIManager->SetString(WINDOW_PC_TRADE, pc_trade_status_dumy, IDS_WATING);

		break;
	}
}

/**
 * NPC� 거래� - 다른용도로도 �용�
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

			// 매품�
			if(g_MainCharInfo.m_pQuickMart) // nEventType == 10
			{				
				g_MainCharInfo.HideSack(SACKTYPE__QUICKMART, FALSE);				
			}			
		}		
		break;
	}
}

/**
 * 개조
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
				// 만일 들고 있는중에 개조하면 들고있는것을 되돌린다.
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

					// 개조자원� 없으� 개조� 불가능하�
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
			//HT_CHEAT : � 상태� 수정
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


// 상점 판매� 입력�
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

			if(nTemp > 2100000000)  // 21� 이상 입력 불가
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
				if(nEventType == WINDOW_MONEY_PCTRADE)	// 개인노점
				{
					XiahItem::sItemInfo* pHoldItem = g_MainCharInfo.m_pHoldItem->GetHoldItemItem();

					if(pHoldItem)
					{
						// 개점� ��� 막기
						if(g_pUIManager->IsShow(WINDOW_PC_STORE) && g_MainCharInfo.m_pPersonalTradeSet && pHoldItem->m_wRefID != 20272)  
						{
							pHoldItem->m_dwPrice = dwAmount;  // 개인 판매 ��

							// 개인노점 아이� 놓기
							SendCS_SH_REGSHOP_REQ(g_MainCharInfo.m_byMySackCurrIdx+1,
												pHoldItem->m_bSackPos,
												pHoldItem->m_dwItemID,
												g_MainCharInfo.m_pHoldItem->m_bBackPosition,
												pHoldItem->m_dwPrice);
						}
					}

				} // if(nEventType == WINDOW_MONEY_PCTRADE)	// 개인노점
				else if(nEventType == WINDOW_MONEY_LOTTO)	// 복권당첨� 수령
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
			case 3:	// 옵션 금전 설정위해 추가
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
			case 3:	// 옵션�
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
			case 1:	// �
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
			case 2:	// 아이�
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
							// setholditem� �기서 하는것이...
						}
						else
						{
							g_MainCharInfo.ShowHelpMessage( IDS_PUT_AMOUNT, TEXTEFFECT_COLOR_WARNING);
							return;	
						}
					}
				}
				break;
			case 3:	// 옵션 금전 설정
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

			// hold 아이� 출력
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

				// hold 아이� 출력
				g_MainCharInfo.m_pHoldItem->SetDrawFlag( TRUE);
			}

			g_MainCharInfo.CloseFrame( WINDOW_VOLUME);
		}		
		break;
	}
}

// � 전투 (� 비�)
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

			if( nTemp > 2100000000)  // 21� 이상 입력 불가
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
 * 문파 호칭 수여
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
			// [3/09/2005] 필터�
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
 * 직위��
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
	case name_channel_button_01:	// 문주
		{
			if(g_MainCharInfo.m_pRelation->Am_I_InClan())
			{
				if(g_MainCharInfo.m_pRelation->Am_I_LeaderInClan())
				{
					// 문주이양
					g_pUIManager->ShowNotice(IDS_MUNJU_RELINQUISH, NOTICE_FRAME_OKCANCEL, NOTICE_FRAME_RELINQUISH);

					/*
					if(g_MainCharInfo.m_dwMunpaFame >= 100)		// 문파명성 100이상
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
	case name_channel_button_02:	// �문주
		SendCS_RL_CHANGEMUNWONORDER_REQ( dwMunwonID, dwOrderID, 2);
		break;
	case name_channel_button_03:	// 장�
		SendCS_RL_CHANGEMUNWONORDER_REQ( dwMunwonID, dwOrderID, 3);
		break;
	case name_channel_button_04:	// 호법
		SendCS_RL_CHANGEMUNWONORDER_REQ( dwMunwonID, dwOrderID, 4);
		break;
	case name_channel_button_05:	// 당주
		SendCS_RL_CHANGEMUNWONORDER_REQ( dwMunwonID, dwOrderID, 5);
		break;
	}

	g_MainCharInfo.CloseFrame( NAME_CHANNEL);
}






//////////////////////////////////////////////////
// Mugong
//////////////////////////////////////////////////
/**
 * 외공
 * \param lParam 
 */
void ProcessWindowOutSide( LPARAM lParam)
{
	int controlID = LOWORD( lParam);
	//int eventType = HIWORD( lParam);

	//HT_1212 : 내공,외공 창에 �이틀 � 수정
	//g_pUIManager->Show(DATA_WINDOW);  //HO_0403_07무공� �이틀� 클릭� 날�표시� 바�어� 안바뀌게 수정� 위한 주석처리
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
	case outside_window_top_button_04:		// 각성
		ProcessClickMugongButton(4);
		break;
	case outside_attack_mode_button_01:		// 무공공격 보호��
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
	//HO_0403_07 내공, 외공� 프레� 클릭� 불필요한 메세�� �던부� 수정
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

		/*HO_0403_07 수정� case outside_attack_mode_button_03:다음� 쓰였� 문구
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
 * 내공
 * \param lParam 
 */
void ProcessWindowInSide( LPARAM lParam)
{
	int controlID = LOWORD( lParam);
	//int eventType = HIWORD( lParam);

	//HT_1212 : 내공,외공 창에 �이틀 � 수정
	//g_pUIManager->Show(DATA_WINDOW);  //HO_0403_07무공� �이틀� 클릭� 날�표시� 바�어� 안바뀌게 수정� 위한 주석처리
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
	case inside_window_top_button_04:	// 각성
		ProcessClickMugongButton(4);
		break;
	
	//HO_0403_07 내공, 외공� 프레� 클릭� 불필요한 메세�� �던부� 수정
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
	case inside_window_mugong_point_up_10://HO_0709_07 : 진각� 내공 프레� : +버튼 눌러�
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
		
		/*HO_0403_07 수정� case outside_attack_mode_button_03:다음� 쓰였� 문구
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
	case dan_window_3button_01:	// �
		g_MainCharInfo.m_pRelation->SetCurrType( eDAN);
		break;
	case dan_window_3button_02:	// 인연
		g_MainCharInfo.m_pRelation->SetCurrType( eShip);
		break;
	case dan_window_3button_03:	// 문파
		g_MainCharInfo.m_pRelation->SetCurrType( eClan);
		break;
	case dan_window_2button_01:
		{
			switch( g_MainCharInfo.m_pRelation->GetCurrType())
			{
			case eDAN:	// 제명
				g_pUIManager->ShowNotice( IDS_Q_JEMYUNG, NOTICE_FRAME_OKCANCEL, NOTICE_FRAME_JEMYUNG);
				break;
			case eShip:	// 전서�
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
			case eDAN:	// 탈퇴
				SendCS_IF_LEAVEPARTY_REQ( g_MainCharInfo.m_pRelation->GetDanID());
				break;
			case eShip:	// 인연끊기
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
 * 문파�
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
	case munpa_window_3button_01:	// �
		g_MainCharInfo.PlayInterfaceSound( ISOUND_SELECT_BUTTON);
		g_MainCharInfo.m_pRelation->SetCurrType( eDAN);
		break;
	case munpa_window_3button_02:	// 인연
		g_MainCharInfo.PlayInterfaceSound( ISOUND_SELECT_BUTTON);
		g_MainCharInfo.m_pRelation->SetCurrType( eShip);
		break;
	case munpa_window_3button_03:	// 문파
		g_MainCharInfo.PlayInterfaceSound( ISOUND_SELECT_BUTTON);
		g_MainCharInfo.m_pRelation->SetCurrType( eClan);
		break;

	case munpa_window_button_01:	// 문파공�
		{
			g_pUIManager->SetString(GAK_MESSAGE_WINDOW, gak_title_dummy, IDS_M_NOTICE_RECORD);

			g_MainCharInfo.OpenFrame(GAK_MESSAGE_WINDOW);
			g_pUIManager->SetPostMsg(GAK_MSG_WINDOW_MUNPA);

			g_pUIManager->SetString(GAK_MESSAGE_WINDOW, gak_message_edit, _T(""));
			g_pUIManager->SetFocus(GAK_MESSAGE_WINDOW);
			g_pUIManager->SetFocus(GAK_MESSAGE_WINDOW, gak_message_edit);
		}
		break;
	case munpa_window_button_02:	// 호칭수여
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
	case munpa_window_button_04:	// 문주� 문파없애�
		{
			if(g_MainCharInfo.m_pRelation->Am_I_2stLeaderInClan()) // �문주� 파�
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
			else // 문주� 폐쇄
			{
				g_pUIManager->ShowNotice( IDS_Q_CLOSE_CLAN, NOTICE_FRAME_OKCANCEL, NOTICE_FRAME_CLOSE_CLAN);
			}
		}		
		break;
	case munpa_window_button_03:
		{
			if( g_MainCharInfo.m_pRelation->Am_I_LeaderInClan())	// 파�시키기
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
			else if( g_MainCharInfo.m_pRelation->Am_I_InClan())		// 탈퇴하기
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
	case found_window_3button_01:	// �
		g_MainCharInfo.PlayInterfaceSound( ISOUND_SELECT_BUTTON);
		g_MainCharInfo.m_pRelation->SetCurrType( eDAN);
		break;
	case found_window_3button_02:	// 인연
		g_MainCharInfo.PlayInterfaceSound( ISOUND_SELECT_BUTTON);
		g_MainCharInfo.m_pRelation->SetCurrType( eShip);
		break;
	case found_window_3button_03:	// 문파
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


// [3/25/2004] 퀘스� 처리
void ProcessWindowQuest( LPARAM lParam)
{
	int controlID = LOWORD( lParam);
	int eventType = HIWORD( lParam);

	switch( controlID)
	{
	case quest_window_01_close_button:
		g_MainCharInfo.CloseFrame( WINDOW_QUEST_01);
		break;
	case quest_window_01_start_button: // 시작
		if( g_MainCharInfo.m_pQuest->GetCurrQuestID())
			SendCS_QS_START_REQ( g_MainCharInfo.m_pQuest->GetCurrQuestID());
		break;
	case quest_window_01_stop_button: // 중�
		if( g_MainCharInfo.m_pQuest->GetCurrQuestID())
			SendCS_QS_STOP_REQ( g_MainCharInfo.m_pQuest->GetCurrQuestID());
		break;
	case quest_window_01_delete_button: // ��
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
 * 게임 옵션
 * \param lParam 
 */
void ProcessWindowOption1( LPARAM lParam)
{
	int controlID = LOWORD( lParam);
	int eventType = HIWORD( lParam);
	
	switch( controlID)
	{
	case option_window_1_close_button:		// ��
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
			// 옵션 ��
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
 * 환경 옵션
 * \param lParam 
 */
void ProcessWindowOption2( LPARAM lParam)
{
	int controlID = LOWORD( lParam);
	int eventType = HIWORD( lParam);

	switch( controlID)
	{
	case option_window_2_close_button:		// ��
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
			// 옵션 ��

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
	case option_window_2_scroll_01:		// �시거�
		{
			g_info_Temp.m_fViewDistance = g_pUIManager->GetData(WINDOW_OPTION_02, option_window_2_scroll_01, GET_SCROLL_CURRENT);
			g_pUIManager->SetString(WINDOW_OPTION_02, option_window_2_scroll_dummy_03, g_info_Temp.m_fViewDistance);
		}
		break;
	case option_window_2_scroll_02:		// �과단� (이펙�)
		{
			g_info_Temp.m_fPolygonDetail = g_pUIManager->GetData(WINDOW_OPTION_02, option_window_2_scroll_02, GET_SCROLL_CURRENT);
			g_pUIManager->SetString(WINDOW_OPTION_02, option_window_2_scroll_dummy_04, g_info_Temp.m_fPolygonDetail);
		}
		break;
	case option_window_2_scroll_03:		// 배경음악 스크�
		{
			g_info_Temp.m_dwBGMVolume = g_pUIManager->GetData(WINDOW_OPTION_02, option_window_2_scroll_03, GET_SCROLL_CURRENT);
			g_pUIManager->SetString(WINDOW_OPTION_02, option_window_2_scroll_dummy_07, g_info_Temp.m_dwBGMVolume);
		}
		break;
	case option_window_2_scroll_04:		// �과음� 스크�
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
 * 거래 옵션
 * \param lParam 
 */
void ProcessWindowOption3(LPARAM lParam)
{
	int nControlID = LOWORD(lParam);

	switch(nControlID)
	{
	case option_window_3_title_close_button:	// ��
	case option_window_3_bottom_button_02:
		{
			g_MainCharInfo.CloseFrame(WINDOW_OPTION_03);
		}
		break;
	case option_window_3_bottom_button_01:		// 횅땍
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
	case window_option_3_up_button_01:			// + � 버튼
		{
			if(g_info_Temp.m_bRarityLimit < 200)
			{
				++g_info_Temp.m_bRarityLimit;

				g_pUIManager->SetString(WINDOW_OPTION_03, window_option_3_sell_dummy_01, g_info_Temp.m_bRarityLimit);
			}
		}
		break;
	case window_option_3_down_button_01:		// + 아래 버튼
		{
			if(g_info_Temp.m_bRarityLimit > 0)
			{
				--g_info_Temp.m_bRarityLimit;

				g_pUIManager->SetString(WINDOW_OPTION_03, window_option_3_sell_dummy_01, g_info_Temp.m_bRarityLimit);
			}
		}
		break;
	case window_option_3_up_button_02:			// � � 버튼
		{
			if(g_info_Temp.m_bStxTypeLimit < 200)
			{
				++g_info_Temp.m_bStxTypeLimit;

				g_pUIManager->SetString(WINDOW_OPTION_03, window_option_3_sell_dummy_02, g_info_Temp.m_bStxTypeLimit);
			}
		}
		break;
	case window_option_3_down_button_02:		// � 아래 버튼
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
 * 종�창
 * \param lParam 
 */
void ProcessWindowClose(LPARAM lParam)
{
	int controlID = LOWORD(lParam);
	//int eventType = HIWORD( lParam);

	switch( controlID)
	{
		case close_window_button_01:	// 캐릭� �선택
			{
				// 상점 �
				g_MainCharInfo.m_bPersonalTradeSell = false;

				CloseAllWindow();

				Stop_BGM();
				// 시작 배경음악
				Play_BGM(_T("sound\\bgm\\intro01.mp3"), 1);

				g_pUIManager->Hide(WINDOW_BUTTON_GROUP_01); // 게임 메뉴
				g_pUIManager->Hide(SYSTEM_BUTTON_GROUP_01); // 시스� 메뉴
				g_pUIManager->Hide(PET_BUTTON_GROUP);		// �
				g_pUIManager->Hide(LARGE_MESSENGER);
				g_pUIManager->Hide(HELP_BUTTON);				//HO_0413_07 � �이드 업데이트

				// 오행 버튼 초기�
				for(int i=0; i < 5; ++i)
					g_pUIManager->SetData(WINDOW_FIVEELEMENTS, fiveelements_window_button_fire+i, CURRENT_INDEX, -1);

				//HT_0720 : 오행 개선 ��
				g_pUIManager->SetData(MAIN_FRAME, main_frame_ok, TEXTURE, 1536);

				// 2004.07.20 이벤트용 로딩화면
				/*
				if( rand() % 2 )
				g_MainCharInfo.OpenFrame( EVENT_LOADING_1 );
				else
				g_MainCharInfo.OpenFrame( EVENT_LOADING_2 );
				*/

				g_MainCharInfo.OpenFrame(LOADING_IMAGE3); //HO_0702_07 등급표시 : 등급표시� �� 스타트�딩� 게임로딩 �분이 동일 이��� 처리된다.
				
				//등급표시 적용� 코드 나�에 �� 버리� ..; 등급표시 전에� 나이 �분이 있엇�...
				//if(g_AppData.m_bAdult)
				//	g_MainCharInfo.OpenFrame(LOADING_IMAGE2);
				//else
				//	g_MainCharInfo.OpenFrame(LOADING_IMAGE);

				g_PetList.Release();

				SendCS_NV_ENDGAME_REQ();
				SET_GAMESTEP( GAMESTEP_INTRO);				
				XiahObject::g_XiahObjectManager.Release();	// 초기�
				g_pMainChar = NULL;
				g_pIntro->Init_Clear();
				g_MainCharInfo.Clear();

				// 배경� SKYBOX 텍스� 설정 (고산)
				//g_SkyBox.ChangeSkyMap(3);

				// 환경 정보 세팅
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

				//HT_0403 : �속형 무공 시전 아이�
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
				g_pUIManager->ShowNotice( IDS_GAME_SELECTCLOSE, NOTICE_FRAME_OKCANCEL, NOTICE_FRAME_ENDGAME); //HO_0816_07 아이� 드랍� 횅땍
			break;
	}
}

#define MAIN_CHAROBJECT	((CXiahCharObject*)(g_pMainChar->m_pObject))

// 개인 상점 설정�
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
	case pc_store_button_02:  // 판매시작&중�
		{
			// 죽었을때 제외
			if(!g_MainCharInfo.m_bMainCharDie)
			{
				g_MainCharInfo.m_bPersonalTradeSell = !g_MainCharInfo.m_bPersonalTradeSell;
				// STOP
				if(MAIN_CHAROBJECT->GetAnimation() == XiahAniType::eLAT_Run)
				{
					MAIN_CHAROBJECT->SetAnimation( XiahAniType::eLAT_Stand, 0);
				}

				// 판매시작/중�� 위치 보정
				SendCS_NV_ENDMOVE_REQ(g_pMainChar->m_dwServerID, MAIN_CHAROBJECT->m_Position.x, -MAIN_CHAROBJECT->m_Position.z, MAIN_CHAROBJECT->m_Position.y, CHARSTATE_NORMAL);

				SendCS_SH_STATUSCHANGE_REQ((BYTE)g_MainCharInfo.m_bPersonalTradeSell);
			}
		}
		break;
		
	case pc_store_button_03: // 금전회수
		{	
			if(g_MainCharInfo.m_dwTradeMoney)
				SendCS_SH_GETMONEY_REQ( g_MainCharInfo.m_dwTradeMoney);
		}
		break;

	case pc_store_button_01:  // 호객문구 ��
		{
			//LPCTSTR strName;
			//LPCTSTR strDescription;

			TCHAR strName[256], strDescription[256];
			memset(strName, 0, 256);
			memset(strDescription, 0, 256);

			g_pUIManager->GetString(WINDOW_PC_STORE, pc_store_passage_edit_01, strName, GET_STRING);
			g_pUIManager->GetString(WINDOW_PC_STORE, pc_store_passage_edit_02, strDescription, GET_STRING);

			SendCS_SH_SETSHOP_REQ(strName, strDescription);								   		// 설정 ��

			//strName = g_pUIManager->GetString(WINDOW_PC_STORE, pc_store_passage_edit_01);        // 노점�
			//strDescription = g_pUIManager->GetString(WINDOW_PC_STORE, pc_store_passage_edit_02); // 호객문구

			//SendCS_SH_SETSHOP_REQ(strName, strDescription);								   		// 설정 ��

			/*

				strName = ((CIEditBox*)pFrame->GetControl( pc_store_passage_edit_01))->m_Text;            // 노점�
				strDescription = ((CIEditBox*)pFrame->GetControl( pc_store_passage_edit_02))->m_Text;     // 호객문구

				SendCS_SH_SETSHOP_REQ(strName, strDescription);
			*/
		}
		break;
	} // switch(controlID)
}

// x각적
// 문파공�
void ProcessWindowGakMessage(LPARAM lParam)
{
	int controlID = LOWORD(lParam);
	int nEventType = HIWORD(lParam);

	switch(nEventType)
	{
	case GAK_MSG_WINDOW_MSG:	// 각적
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
					case 8:	// 황금각적
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
	case GAK_MSG_WINDOW_MUNPA:	// 문파공�
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

// 동신�
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
				// � 비�
				if( pMainChar->m_dwPartyID && pMainChar->m_dwEnemyPartyID )
                    g_MainCharInfo.ShowHelpMessage(IDS_NOTPORTALMOVE_INDANBATTLE);
				else	// 이벤� 아이� �용�
					g_MainCharInfo.ShowHelpMessage(IDS_NOTPORTALMOVE);

				return;
			}

			// 아이템을 �용하� 이동한다.
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
 * 전낭
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

			if( nTemp > 2100000000)  // 21� 이상 입력 불가
			{
				g_MainCharInfo.ShowHelpMessage(IDS_MANY_MONEY, TEXTEFFECT_COLOR_WARNING);
				return;
			} // if( nTemp > 2100000000)

			// 1억전까� 거래 �능하� 서버� �킷날릴때 DWORD�...
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
 * 문파 현황
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

	case munpa_bbs_top_money_button:	// 세금회수
		{
			CXiahCharObject* pMainChar = reinterpret_cast<CXiahCharObject*>(g_pMainChar->m_pObject);

			if(pMainChar)
				SendCS_RL_GETMUNPAMONEY_REQ(pMainChar->m_dwMunpaID, g_MainCharInfo.m_dwTaxMunpaMoney, g_MainCharInfo.m_dwPickedObject);
		}
		break;

	case munpa_bbs_top_button_01:		// 문파 현황
		break;

	case munpa_bbs_top_button_02:		// 공� �� (�스트)
		{
			if(g_MainCharInfo.m_pListClient)
				delete g_MainCharInfo.m_pListClient, g_MainCharInfo.m_pListClient = NULL;

			g_MainCharInfo.m_pListClient = new CListClient(CListClient::MUNPA_BBS_LIST);
			g_MainCharInfo.m_pListClient->Set(760, 80, 240, 20);

			CXiahCharObject* pMainChar = reinterpret_cast<CXiahCharObject*>(g_pMainChar->m_pObject);
			if(pMainChar)
				SendCS_RL_MUNPABBSLIST_REQ(pMainChar->m_dwMunpaID);		// �스트 요청

			g_MainCharInfo.CloseFrame(WINDOW_MUNPA_BBS_TOP);
			g_MainCharInfo.OpenFrame(WINDOW_MUNPA_BBS_LIST);
		}
		break;

	case munpa_bbs_top_button_03:		// 공� 기�
		{
			g_MainCharInfo.CloseFrame(WINDOW_MUNPA_BBS_TOP);

			g_MainCharInfo.OpenFrame(WINDOW_MUNPA_BBS_WRITE);
			g_pUIManager->SetFocus(WINDOW_MUNPA_BBS_WRITE, munpa_bbs_write_edit);
		}
		break;

	case munpa_bbs_top_button_04:		// ��
		break;

	default:
		break;
	}
}

/**
 * 문파 게시� �스트
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

	case munpa_bbs_list_button_01:		// 현황
		{
			g_MainCharInfo.CloseFrame(WINDOW_MUNPA_BBS_LIST);
			g_MainCharInfo.OpenFrame(WINDOW_MUNPA_BBS_TOP);

			if(g_MainCharInfo.m_pListClient)
				delete g_MainCharInfo.m_pListClient, g_MainCharInfo.m_pListClient = NULL;
		}
		break;
	case munpa_bbs_list_button_02:		
		break;
	case munpa_bbs_list_button_03:		// 기�
		{
			g_MainCharInfo.CloseFrame(WINDOW_MUNPA_BBS_LIST);

			if(g_MainCharInfo.m_pListClient)
				delete g_MainCharInfo.m_pListClient, g_MainCharInfo.m_pListClient = NULL;

			g_MainCharInfo.OpenFrame(WINDOW_MUNPA_BBS_WRITE);
			g_pUIManager->SetFocus(WINDOW_MUNPA_BBS_WRITE, munpa_bbs_write_edit);
		}
		break;
	case munpa_bbs_list_button_04:		// ��
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
 * BBS 읽기
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

	case munpa_bbs_read_button_01:			// 현황
		{
			g_MainCharInfo.CloseFrame(WINDOW_MUNPA_BBS_READ);
			g_MainCharInfo.OpenFrame(WINDOW_MUNPA_BBS_TOP);
		}
		break;

	case munpa_bbs_read_button_02:			// 공� �� (�스트)
		{
			if(g_MainCharInfo.m_pListClient)
				delete g_MainCharInfo.m_pListClient, g_MainCharInfo.m_pListClient = NULL;

			g_MainCharInfo.m_pListClient = new CListClient(CListClient::MUNPA_BBS_LIST);
			g_MainCharInfo.m_pListClient->Set(760, 80, 240, 20);

			CXiahCharObject* pMainChar = reinterpret_cast<CXiahCharObject*>(g_pMainChar->m_pObject);
			SendCS_RL_MUNPABBSLIST_REQ(pMainChar->m_dwMunpaID);			// �스트 요청

			g_MainCharInfo.CloseFrame(WINDOW_MUNPA_BBS_READ);
			g_MainCharInfo.OpenFrame(WINDOW_MUNPA_BBS_LIST);
		}
		break;
	case munpa_bbs_read_button_03:			// 기�
		{
			g_MainCharInfo.CloseFrame(WINDOW_MUNPA_BBS_READ);
			g_MainCharInfo.OpenFrame(WINDOW_MUNPA_BBS_WRITE);
			g_pUIManager->SetFocus(WINDOW_MUNPA_BBS_WRITE, munpa_bbs_write_edit);
		}
		break;
	case munpa_bbs_read_button_04:			// ��
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
 * BBS 기�
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
 * 기부� 납부
 * \param lParam 
 */
void ProcessWindowMunpaDonate(LPARAM lParam)
{
	const int nControlID = LOWORD(lParam);
	//const int nEventType = HIWORD(lParam);

	switch(nControlID)
	{
	case munpa_donate_edit:
	case munpa_donate_check_button:		// 기부� 횅땍
		{
			int nTemp = _tstoi(static_cast<LPCTSTR>(g_pUIManager->GetString(WINDOW_MUNPA_DONATE, munpa_donate_edit)));

			if(nTemp > 0)
			{
				CXiahCharObject* pMainChar = reinterpret_cast<CXiahCharObject*>(g_pMainChar->m_pObject);
				SendCS_RL_PREDONATE_REQ(pMainChar->m_dwMunpaID, static_cast<DWORD>(nTemp));
			}
		}
		break;

	case munpa_donate_button_01:		// 납부
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
 * 문파� 신청
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


					// 툴팁
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
 * 전서�
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
	case window_mail_button_01:		// 읽기
			g_pUIManager->SetString(WINDOW_MAIL, window_mail_button_04, IDS_REPLY);
			g_pUIManager->SetString(WINDOW_MAIL, window_mail_input_dummy_01, IDS_MAIL_01);
			g_pUIManager->SetData(WINDOW_MAIL, window_mail_top_edit_01, EDITMODE, NOEDIT);

			g_Mail.Reflash_MAIL();
			g_Mail.Send_ReadMail();

			g_MainCharInfo.CloseFrame(WINDOW_MAIL_RESULT);
			g_MainCharInfo.OpenFrame(WINDOW_MAIL_SELECT);
		break;

	case window_mail_button_02:		// 쓰기
		{
			g_pUIManager->SetData(WINDOW_MAIL, window_mail_top_edit_01, EDITMODE, EDIT);
			g_pUIManager->SetString(WINDOW_MAIL, window_mail_button_04, IDS_SEND);
			// 보낸 �람에� 받는 �람으�
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

	case window_mail_button_03:		// ��
		{
			g_MainCharInfo.CloseFrame(WINDOW_MAIL_SELECT);
			g_pUIManager->ShowNotice(IDS_YESNO_DELETE, NOTICE_FRAME_OKCANCEL, NOTICE_FRAME_MAIL_DELETE);

			g_MainCharInfo.CloseFrame(WINDOW_MAIL_RESULT);
		}
		break;

	case window_mail_button_04:		// 회신&전송
		{
			TCHAR szText[100] = {0,};
			_stprintf(szText,IDS_YESNO_SENDMAIL,g_Mail.Get_Checked_SendList(),g_Mail.Get_Amonut());
			g_pUIManager->ShowNotice(szText, NOTICE_FRAME_OKCANCEL, NOTICE_FRAME_MAIL_SEND);
			g_MainCharInfo.CloseFrame(WINDOW_MAIL_SELECT);
			g_MainCharInfo.CloseFrame(WINDOW_MAIL_RESULT);
		}
		break;

	case window_mail_button_left:	// �
		g_Mail.Back_Recv_Page();
		break;
	case window_mail_button_right:	// �
		g_Mail.Next_Recv_Page();
		break;

/////////////////////////////////////////////////////////////////////////////////////////////////////

	case window_mail_top_edit_01:	// 보낸�� 엔터� 제�으�
		g_pUIManager->SetFocus(WINDOW_MAIL, window_mail_top_edit_02);
		break;
	case window_mail_top_edit_02:	// 제�에� 내용 � �
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
 * 전서� 선택
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
	case window_mail_select_button_left:	// 다음 버튼
		g_Mail.Back_Page();
		break;
	case window_mail_select_button_right:	// 이전
		g_Mail.Next_Page();
		break;
	case window_mail_select_button_01:		// 문파 선택
		g_Mail.CheckedMunpaAll_SendList();
		g_Mail.Reflash_MAIL_Select();
		break;
	case window_mail_select_button_02:		// 전체 선택
		g_Mail.CheckedAll_SendList();
		g_Mail.Reflash_MAIL_Select();
		break;
	case window_mail_select_button_03:		// 전체 해제
		g_Mail.UncheckedAll_SendList();
		g_Mail.Reflash_MAIL_Select();
		break;
	default:
		break;
	}
}

/**
 * 전서� 전송 횅땍
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
	case window_mail_result_button_01:		// 횅땍
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

				if( nTemp > 2100000000)  // 21� 이상 입력 불가
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
* � 거래 정보
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
 * 복권 번호
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
	case bok_number_button_01:	// ��
		{
			int nCount =0;

			// 선택된개� ��
			for(int i=0; i < 25; ++i)
			{
				if(g_pUIManager->GetData(WINDOW_BOK_NUMBER, bok_number_number_button_01 + i, GET_CURRENT_INDEX) == 2)
					++nCount;
			}

			if(nCount == 4)	// 4� 선택이면 OK
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
					// 4� 미선�
					g_MainCharInfo.ShowHelpMessage(IDS_LOTTO_SELECT_NOT, TEXTEFFECT_COLOR_WARNING);
				} // if(nCount < 4)
				else if(nCount > 4)
				{
					// 선택개수 초과
					g_MainCharInfo.ShowHelpMessage(IDS_LOTTO_SELECT_NOT_2, TEXTEFFECT_COLOR_WARNING);
				}
			}
		}
		break;
	case bok_number_button_03:	// 예상당첨금액 횅땍
		{
			SendCS_EC_LOTTOSALEINFO_REQ();
		}
		break;
	default:
		{
			// 복권 번호일때
			if(nControlID >= bok_number_number_button_01 && nControlID <= bok_number_number_button_25)
			{
				int nCount =0;

				// 선택된개� ��
				for(int i=0; i < 25; ++i)
				{
					if(g_pUIManager->GetData(WINDOW_BOK_NUMBER, bok_number_number_button_01 + i, GET_CURRENT_INDEX) == 2)
						++nCount;
				}

				// 4개이상일 경우 �
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
 * 복권 당첨번호
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
	case bok_prize_button_01:	// �난회� 당첨번호
		{
			SendCS_EC_PRIZELOTTOINFO_REQ(0);
		}
		break;

	case bok_prize_button_02:	// 이번회차 당첨번호
		{
			SendCS_EC_PRIZELOTTOINFO_REQ(1);
		}
		break;

	default:
		break;
	}
}

/**
 * 문파 마크 (앞으� 다른용도� 이용��)
 * \param lParam 
 */
void ProcessWindowMark(LPARAM lParam)
{
	int nControlID = LOWORD(lParam);
	
	switch(nControlID)
	{
	case mark_window_button_01:		// 횅땍
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

				// mark.bmp.bmp 파일� �� (일부 유저� 파일� 이렇� 만드� 경우� 있다.)
				_stprintf(strFile, "mark.bmp.bmp");
				bCheck = true;

				goto Check;
			}
			else
			{
				// bmp�
				if(imageinfo.ImageFileFormat != D3DXIFF_BMP)
				{
					g_MainCharInfo.ShowHelpMessage(IDS_MUNPAMARK_NOBMP, TEXTEFFECT_COLOR_WARNING);
					g_MainCharInfo.CloseFrame(MESSAGE_WINDOW_MARK);
					break;					
				}

				// 16x16�기�
				if(imageinfo.Width != 16 || imageinfo.Height != 16)
				{
					g_MainCharInfo.ShowHelpMessage(IDS_MUNPAMARK_MISTAKEN, TEXTEFFECT_COLOR_WARNING);
					g_MainCharInfo.CloseFrame(MESSAGE_WINDOW_MARK);
					break;					
				}

				// 24비트�
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
				// 서버에는 헤드� 제거� 순수 이��� 들어� 있다.				
				// 이유� 이��� 문자열� 보내는데 0이면 문자열에� null이기� 문제� �/서버 � 생긴�.
				// 그래� 헤드� 제거하고
				// 이�� 0� 1� ��

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
	case mark_window_button_02:		// ��
		{
			g_MainCharInfo.CloseFrame(MESSAGE_WINDOW_MARK);
		}
		break;
	default:
		break;
	}
}

/**
 * 조합�
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

			// 행낭 6 * 4 �� ��
			for(int i=0; i < 24; ++i)
			{
				pItemInfo = g_MainCharInfo.m_pSmeltSack->FindSackItemByPos(i);

				if(pItemInfo)
				{
					++nItemCount;

					// 망치 조합
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

					// 이벤� �� � 광���...
					switch(pItemInfo->m_bItemType)
					{
					case ITEMTYPE_EVENT:
					case ITEMTYPE_REBUILDRES:
						// �, �
					case 18:
					case 20:
						{
							if(bItemType)
							{
								if(!(pItemInfo->m_bItemType == 18 || pItemInfo->m_bItemType == 20))
								{
									// 다른 �입인� ��
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

			// 조합� 아이템이 없을�
			if(!nItemCount)
			{
				g_MainCharInfo.ShowHelpMessage(IDS_MIXTURE_ITEM, TEXTEFFECT_COLOR_WARNING);

				return;
			}

			if(bItemType == ITEMTYPE_EVENT)				// 9� ��, 7� 보석
			{
				// [2/1/2005] 설날 �래떡
				if(5 == bItemKind)
				{							
					SendCS_IM_VARIENTITEM_REQ(0);
				}
				else if(8 == bItemKind)	// [1/13/2006] 이�� 조합
				{
					SendCS_IM_EVENTPUZZLE_REQ(g_MainCharInfo.m_dwPickedObject);
				}
				else if(10 == bItemKind)	//HT_0523 선� 상자 조합
				{
					SendCS_IM_VARIENTITEM_REQ(2);
				}
				else
				{
					SendCS_IM_PUZZLEITEM_REQ(0);
				}					
			}
			else if(bItemType == ITEMTYPE_REBUILDRES)	// �
			{	
				if(bIsDividedRes != 1)
				{
					// ��
					SendCS_IM_VARIENTITEM_REQ(1);
				}
				else
				{
					SendCS_IM_REJOINITEM_REQ(0);
				}					
			}
			else if(bItemType == 18 || bItemType == 20)	// �/� 아이� 조합
			{
				SendCS_IM_MIXITEM_REQ(0);
			}
		}	
		break;
	}
}



/**
 * 오행
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

		// 오행 수치 ���
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

		// 오행 선택
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
 * 오행 제련
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
			// 만일 들고 있는중에 개조하면 들고있는것을 되돌린다.
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

				// 개조자원� 없으� 개조� 불가능하�
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
 * �� 내용
 * \param lParam 
 */
//HO_0410_07 상서� �이드 업데이트
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
 * �� 내용1
 * \param lParam 
 */
void ProcessWindowHelperList(LPARAM lParam)//HO_0410_07 상서� �이드 업데이트 
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
 * �� 내용
 * \param lParam 
 */
//HO_0410_07 상서� �이드 업데이트 : 업데이트 적용� 스크립트� 탐랑 스크립트� ��
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

void ProcessWindowQuickScript(LPARAM lParam)//HO_0413 : � �이드 스크립트
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
 * 아이� 복구
 * \param lParam 
 */
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
			int nSize = g_MainCharInfo.m_vRecoveryItem.size();

			if(nSize)
			{
				if(g_MainCharInfo.m_nTempValue < nSize)
				{
					g_pUIManager->SetData(WINDOW_RECOVERY, recovery_window_back_dummy01+g_MainCharInfo.m_nTempValue, COLOR, 0, 0, D3DCOLOR_XRGB(250, 250, 0));

					g_MainCharInfo.m_dwResItemID = g_MainCharInfo.m_vRecoveryItem[g_MainCharInfo.m_nTempValue];
				}
				else
				{
					g_MainCharInfo.m_dwResItemID = 0;
				}
			}
		}
		break;
	case recovery_window_button01:
		{
			if(g_MainCharInfo.m_dwResItemID && g_MainCharInfo.m_vRecoveryItem.size())
			{
				//TCHAR szName[32] = {0,};				
				//g_pUIManager->GetString(WINDOW_RECOVERY, recovery_window_back_dummy01+g_MainCharInfo.m_nTempValue, szName);

				TCHAR szTemp[128] = {0,};
				_stprintf(szTemp, IDS_RECOVERY_INFO, g_MainCharInfo.m_vRecoveryItemName[g_MainCharInfo.m_nTempValue].data());

				g_pUIManager->ShowNotice(szTemp, NOTICE_FRAME_OKCANCEL, NOTICE_FRAME_RECOVERY1, XiahInput::g_ptMouse.x - 270, XiahInput::g_ptMouse.y-90);
			}
		}
		break;
	case recovery_window_button02:
		{
			if(g_MainCharInfo.m_dwResItemID && g_MainCharInfo.m_vRecoveryItem.size())
			{
				//TCHAR szName[32] = {0,};				
				//g_pUIManager->GetString(WINDOW_RECOVERY, recovery_window_back_dummy01+g_MainCharInfo.m_nTempValue, szName);

				TCHAR szTemp[128] = {0,};
				_stprintf(szTemp, IDS_SWEEP_INFO, g_MainCharInfo.m_vRecoveryItemName[g_MainCharInfo.m_nTempValue].data());

				g_pUIManager->ShowNotice(szTemp, NOTICE_FRAME_OKCANCEL, NOTICE_FRAME_RECOVERY2, XiahInput::g_ptMouse.x - 270, XiahInput::g_ptMouse.y-90);
			}
		}
		break;
	case recovery_title_close_button:
	case recovery_window_button03:
		{
			g_MainCharInfo.CloseFrame(WINDOW_RECOVERY);
		}
		break;
	default:
		break;
	}
}

/**
* � 경험� 분배
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
	case window_dan_new_button1:	// �
		//g_MainCharInfo.m_pRelation->SetCurrType( eDAN);
		break;
	case window_dan_new_button2:	// 인연
		g_MainCharInfo.m_pRelation->SetCurrType( eShip);
		break;
	case window_dan_new_button3:	// 문파
		g_MainCharInfo.m_pRelation->SetCurrType( eClan);
		break;
	case window_dan_new_2button_01:
		{
			switch( g_MainCharInfo.m_pRelation->GetCurrType())
			{
			case eDAN:	// 제명
				g_pUIManager->ShowNotice( IDS_Q_JEMYUNG, NOTICE_FRAME_OKCANCEL, NOTICE_FRAME_JEMYUNG);
				break;
			case eShip:	// 전서�				
				break;
			case eClan:
				break;
			}
		}
		break;
	case window_dan_new_2button_02:
		{
			//HT_0423 : �� 위임
			switch( g_MainCharInfo.m_pRelation->GetCurrType())
			{
			case eDAN:	// 탈퇴
				SendCS_IF_LEAVEPARTY_REQ( g_MainCharInfo.m_pRelation->GetDanID());
				break;
			case eShip:	// 인연끊기
				break;
			case eClan:
				break;
			}
		}
		break;
	case window_dan_new_1button:
		{
			//HT_0423 : �� 위임
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
				case eDAN:	// 탈퇴
					SendCS_IF_LEAVEPARTY_REQ( g_MainCharInfo.m_pRelation->GetDanID());
					break;
				case eShip:	// 인연끊기
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
 * NPC �� 이동
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
				// 화산 �� 이동
				SendCS_NV_QUICKMOVE_REQ(2);
			}
			else if(nEventType == WINDOW_NPC_PORTAL__WAR)
			{
				// � 속성 ��
				SendCS_NV_PRIVATEPORTAL_REQ(0);
			}

			g_MainCharInfo.CloseFrame(WINDOW_PORTAL);
		}
		break;
	case window_portal_button2:
		{
			if(nEventType == WINDOW_NPC_PORTAL)
			{
				// � �� 이동
				SendCS_NV_QUICKMOVE_REQ(5);
			}
			else if(nEventType == WINDOW_NPC_PORTAL__WAR)
			{
				// � 속성 ��
				SendCS_NV_PRIVATEPORTAL_REQ(1);
			}

			g_MainCharInfo.CloseFrame(WINDOW_PORTAL);
		}
		break;
	case window_portal_button3:
		{
			if(nEventType == WINDOW_NPC_PORTAL__WAR)
			{
				// � 속성 ��
				SendCS_NV_PRIVATEPORTAL_REQ(2);
			}

			g_MainCharInfo.CloseFrame(WINDOW_PORTAL);
		}
		break;
	case window_portal_button4:
		{
			if(nEventType == WINDOW_NPC_PORTAL__WAR)
			{
				// � 속성 ��
				SendCS_NV_PRIVATEPORTAL_REQ(3);
			}

			g_MainCharInfo.CloseFrame(WINDOW_PORTAL);
		}
		break;
	
	case window_portal_button5: //HO_0906_07 문파�� ��� �탈기� 추가 : 마혈� ��
		{
			if(nEventType == WINDOW_NPC_PORTAL__WAR)
			{
				// 마혈� ��				
				SendCS_NV_PRIVATEPORTAL_REQ(4);
			}

			g_MainCharInfo.CloseFrame(WINDOW_PORTAL);
		}
		break;
	case window_portal_exit_button:		// � ��
		{
			g_MainCharInfo.CloseFrame(WINDOW_PORTAL);			
		}
		break;
	default:
		break;
	}
}

/**
* 아이� 수�
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
* 각성
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
	case skill_window_top_button_04:		// 각성
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
 * HT_0313 : 광명� & 천황� (이동)	
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
 * HT_0313 : 광명� & 천황� (참여)
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
