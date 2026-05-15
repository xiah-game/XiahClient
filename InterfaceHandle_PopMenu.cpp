#include "XiahArrayIndex.h"
#include "Helper.h"

extern sString MoneyCommaStr(INT64 nMoney);

//////////////////////////////////////////////////////////
// PopMenu Frame
//////////////////////////////////////////////////////////
/**
 * 상대방 팝메뉴
 * \param lParam 
 */
void ProcessPopMenuPc( LPARAM lParam)
{
	int controlID = LOWORD( lParam);
	//int eventType = HIWORD( lParam);

	// 매품패
	if(g_MainCharInfo.m_pQuickMart)
	{		
		g_MainCharInfo.HideSack(SACKTYPE__QUICKMART, FALSE);		
	}

	switch( controlID)
	{
	case 0:	// 단 비무
		{
			if(g_MainCharInfo.m_pRelation->Am_I_LeaderInDan())
			{
				g_MainCharInfo.OpenFrame(WINDOW_DAN_WAR);

				g_pUIManager->DeletePopMenu();
				g_pUIManager->DeletePopSubMenu();
			}
		}
		break;

	case 1:	// 관계
		{
			sRect rtTemp;
			g_pUIManager->GetRegionData(FRAMEID_PC, 1, rtTemp);

			g_pUIManager->MakePopSubMenu(	1, rtTemp.right, rtTemp.bottom,
											FRAMEID_PC_RELATION, 6, 
											TRUE, FRAMEID_POPUP_SUBMENU, IDS_DAN,
											TRUE, FRAMEID_POPUP_SUBMENU, IDS_DAN_RELATION,
											TRUE, FRAMEID_POPUP_SUBMENU, IDS_CLAN,
											TRUE, FRAMEID_POPUP_SUBMENU, IDS_FRIEND,
											TRUE, FRAMEID_POPUP_SUBMENU, IDS_TEACHERDISCIPLE,
											TRUE, FRAMEID_POPUP_SUBMENU, IDS_SWEETHEART);
		}
		break;

	case 2:	// 거래
		if(!g_MainCharInfo.m_dwAskID)
		{
			if(g_MainCharInfo.m_bPickType == 0)
			{
				sRect rtTemp;
				g_pUIManager->GetRegionData(FRAMEID_PC, 2, rtTemp);

				g_pUIManager->MakePopSubMenu(2, rtTemp.left, rtTemp.bottom,
											FRAMEID_PC_TRADE, 2, 
											TRUE, FRAMEID_POPUP_SUBMENU, IDS_NORMAL_TRADE, 
											TRUE, FRAMEID_POPUP_SUBMENU, IDS_PET_TRADE);
			}
			else
			{
				if(g_MainCharInfo.m_pChat->GetChatType() == LARGECHAT)
					g_MainCharInfo.m_pChat->SetChatType( SMALLCHAT);

				// 개인 상점 거래
				SendCS_SH_GETSHOPINFO_REQ(g_MainCharInfo.m_dwPickedObject);

				g_pUIManager->DeletePopMenu();
				g_pUIManager->DeletePopSubMenu();
			}
		}
		else
		{
			if(g_MainCharInfo.m_bPickType == 1)
			{
				if(g_MainCharInfo.m_pChat->GetChatType() == LARGECHAT)
					g_MainCharInfo.m_pChat->SetChatType( SMALLCHAT);

				// 개인 상점 거래
				SendCS_SH_GETSHOPINFO_REQ(g_MainCharInfo.m_dwPickedObject);
			} // if(g_MainCharInfo.m_bPickType == 1)
			else
			{
				g_MainCharInfo.ShowHelpMessage(IDS_ON_DEAL);
			}

			g_pUIManager->DeletePopMenu();
			g_pUIManager->DeletePopSubMenu();
		}
		break;
	}
}

/**
 * NPC 팝업
 * \param lParam 
 */
void ProcessPopMenuNpc( LPARAM lParam)
{
	int controlID = LOWORD( lParam);
	int eventType = HIWORD( lParam);

	XiahObject::CXiahObject* pObject = XiahObject::g_XiahObjectManager.FindXiahObject( MAKEOBJECTID( 0, g_MainCharInfo.m_dwPickedObject, OBJTYPE_FUNCTIONALNPC));

	if( pObject == NULL ) 
	{
		DBG_LogFile( _T("ProcessPopMenuNpc1 fail"));
		return;
	}
	if( pObject->m_pObject == NULL ) 
	{
		DBG_LogFile( _T("ProcessPopMenuNpc2 fail"));
		return;
	}

	CXiahCharObject* pCharObject = reinterpret_cast<CXiahCharObject*>( pObject->m_pObject);
	if(pCharObject == NULL)
	{
		DBG_LogFile( _T("ProcessPopMenuNpc3 fail"));
		return;
	}
	sFunctionalNpcInfo* pInfo = (sFunctionalNpcInfo*)pCharObject->m_pPrivateData;

	if( !pInfo)
	{
		DBG_LogFile( _T("ProcessPopMenuNpc4 fail"));
		return;
	}
	
	// 매품패
	if(g_MainCharInfo.m_pQuickMart)
	{		
		g_MainCharInfo.HideSack(SACKTYPE__QUICKMART, FALSE);		
	}

	switch( controlID)
	{
	case 0:	// 수리
		{
			bool bTemp = true;

			switch( pInfo->m_bType)
			{
			case 1: //대장장이
				{
					g_MainCharInfo.ShowHelpMessage( IDS_REPAIR_WEAPON);
					//pCharObject->SetChatBox(timeGetTime(), IDS_REPAIR_WEAPON, 111);
				}				
				break;
			case 2:	//의류상인
				{
					g_MainCharInfo.ShowHelpMessage( IDS_REPAIR_CLOTH);
					//pCharObject->SetChatBox(timeGetTime(), IDS_REPAIR_CLOTH, 111);
				}				
				break;		
			case 3: //잡화상인
				{
					g_MainCharInfo.ShowHelpMessage( IDS_REPAIR_JABWHA);
					//pCharObject->SetChatBox(timeGetTime(), IDS_REPAIR_JABWHA, 111);
				}				
				break;
			case 4:	//보석상인
				{
					g_MainCharInfo.ShowHelpMessage( IDS_REPAIR_JEWERY);
					//pCharObject->SetChatBox(timeGetTime(), IDS_REPAIR_JEWERY, 111);
				}
				break;
			case 7:	// 창고지기 - 보험 아이템 복구
				{
					bTemp = false;
					SendCS_EC_GUARANTEELIST_REQ();
				}
				break;
			default:
				{
					g_MainCharInfo.ShowHelpMessage( IDS_REPAIR_ANYTHING);
					//pCharObject->SetChatBox(timeGetTime(), IDS_REPAIR_ANYTHING, 111);
				}
				break;
			}

			CloseAllWindow();

			if(bTemp)
			{
				ChangeXiahCursor( eCT_Repair);
				g_MainCharInfo.ShowSack( SACKTYPE__DEFAULT);
				g_MainCharInfo.ShowSack( SACKTYPE__EQUIPMENT);

				g_MainCharInfo.CloseFrame(WINDOW_QUEST_01);
				g_MainCharInfo.HideSack(SACKTYPE__PERSONAL_TRADE_SET);
			}			
		}
		break;
	case 1:	// 개조
		{
			bool bItemMall = false;

			switch( pInfo->m_bType)
			{
			case 1: //대장장이
				g_MainCharInfo.ShowHelpMessage( IDS_CONVERT_SWORD);
				break;
			case 2:	//의류상인
				g_MainCharInfo.ShowHelpMessage( IDS_CONVERT_CLOTH);
				break;	
			case 3: //잡화상인
				g_MainCharInfo.ShowHelpMessage( IDS_CONVERT_JABWHA);
				break;
			case 4:	//보석상인
				g_MainCharInfo.ShowHelpMessage( IDS_CONVERT_JEWERY);
				break;

			case 6:	//서점주인 - 복권
				{
					pCharObject->SetChatBox(timeGetTime(), IDS_LOTTO_NPC, 111);

					sRect rtTemp;
					g_pUIManager->GetRegionData(FRAMEID_NPC, 1, rtTemp);

					g_pUIManager->MakePopSubMenu(1, rtTemp.right, rtTemp.bottom,
												FRAMEID_LOTTO, 2, 
												TRUE, FRAMEID_POPUP_SUBMENU, IDS_LOTTO_BUY, 
												TRUE, FRAMEID_POPUP_SUBMENU, IDS_LOTTO_DEFINITE);

					return;

				}
				break;
			case 7: // 창고지기 - 아이템 몰
				{
					bItemMall = true;

					g_pUIManager->SetString(WINDOW_NPC_TRADE, npc_trade_window_title_back, IDS_ITEMMALL_NAME);
					SendCS_EC_ITEMLISTINMALL_REQ( g_MainCharInfo.m_dwPickedObject);
				}				
				break;
			default:
				g_MainCharInfo.ShowHelpMessage( IDS_CONVERT_ANYTHING);
				break;
			} // switch( pInfo->m_bType)

			if(!bItemMall)
				ProcessClickItemConvert();
		}
		break;

	case 2:	// 거래
		switch( pInfo->m_bType)
		{
		case 7:	//창고지기
			{
				SendCS_EC_ITEMLISTINBANK_REQ( g_MainCharInfo.m_dwPickedObject);
				g_MainCharInfo.ShowHelpMessage(IDS_START_NPC_DEAL);
			}			
			break;

		default:
			{
				/////////////////////////////////////////////////////////////////////////////////////////////////////
				sArrayData* pQuestScript = XiahArrayIndex::g_QuestScript.GetData(g_MainCharInfo.m_dwPickedObject);

				if(pQuestScript)
				{
					bool bSuccess = false;
					int nTotal = pQuestScript->GetInt(3);

					for(int i=0; i < nTotal; ) // 같은 npc가 있기에 돌면서 검사
					{
						sQuestInfo* pQuestInfo = g_MainCharInfo.m_pQuest->FindQuest(pQuestScript->GetInt(1));

						if(pQuestInfo)
						{
							if(pQuestInfo->m_bProcessType == pQuestScript->GetInt(2)) // 현재 상태와 같은것
							{
								TCHAR strTemp[256]= {0,};

								_stprintf( strTemp, _T("%s|%s"), (LPCTSTR)pQuestScript->GetString(0), (LPCTSTR)pQuestScript->GetString(1));
								
								pCharObject->SetChatBox( timeGetTime(), strTemp, 111);
								bSuccess = true;
								break;
							}
						}

						++i;
						pQuestScript = XiahArrayIndex::g_QuestScript.GetSkipData(i, g_MainCharInfo.m_dwPickedObject); // i번째 이후 인덱스
					}

					if(pInfo->m_bType == 9 || pInfo->m_bType == 10) // 복면인 전용 루틴
					{
						if(!bSuccess)
							pCharObject->SetChatBox( timeGetTime(), IDS_QUEST_NPC, 111);
						else
							// 서버에서 퀘스트 검사 때문에 전송
							SendCS_NC_FUNCTIONALNPCITEMLIST_REQ( XiahMap::g_XiahMap.m_MapInfo.m_dwMapID, g_MainCharInfo.m_dwPickedObject, 0);
						break;
					}
				} // if(pQuestScript)
				//else
				//	pCharObject->SetChatBox(timeGetTime(), IDS_NPC_TRADE, 111);
					
				/////////////////////////////////////////////////////////////////////////////////////////////////////

				g_MainCharInfo.m_bSackCnt = 0;
				SendCS_NC_FUNCTIONALNPCITEMLIST_REQ( XiahMap::g_XiahMap.m_MapInfo.m_dwMapID, g_MainCharInfo.m_dwPickedObject, 0);
				g_MainCharInfo.ShowHelpMessage(IDS_START_NPC_DEAL);

				break;
			}
		}		
		break;
	} // switch( controlID)

	if(g_MainCharInfo.m_pChat->GetChatType() == LARGECHAT)
	{
		g_MainCharInfo.m_pChat->SetChatType( SMALLCHAT);
	}

	g_pUIManager->DeletePopMenu();
}

/**
 *
 * \param lParam 
 */
void ProcessPopMenuPet( LPARAM lParam)
{
	int controlID = LOWORD( lParam);
	int eventType = HIWORD( lParam);

	// 매품패
	if(g_MainCharInfo.m_pQuickMart)
	{		
		g_MainCharInfo.HideSack(SACKTYPE__QUICKMART, FALSE);		
	}

	switch( controlID)
	{
	case 0:	// 상태
		ProcessClickPetInfoButton();
		g_pUIManager->DeletePopMenu();
		break;
	case 1:
		{
			sRect rtTemp;
			g_pUIManager->GetRegionData(FRAMEID_PET, 1, rtTemp);

			g_pUIManager->MakePopSubMenu(	1, rtTemp.right, rtTemp.bottom,
											FRAMEID_PET_AI, 5, 
											TRUE, FRAMEID_POPUP_SUBMENU, IDS_CALL_PET_M,
											TRUE, FRAMEID_POPUP_SUBMENU, IDS_AUTO_ATTK, 
											TRUE, FRAMEID_POPUP_SUBMENU, IDS_OBJ_ATTK, 
											FALSE, FRAMEID_POPUP_SUBMENU, IDS_MUGONG_ATTK, 
											TRUE, FRAMEID_POPUP_SUBMENU, IDS_ITEM_COLLECT_M);
		}
		break;
	case 2:	// 특수
		{
			sRect rtTemp;

			g_pUIManager->GetRegionData(FRAMEID_PET, 2, rtTemp);

			sPetInfo* pPet = g_PetList.GetCurrentPet();
			if( pPet)
			{
				if( pPet->m_pEquipSack->FindSackItemByPos( PETEQUIP_BAG))
				{
					g_pUIManager->MakePopSubMenu(	2, rtTemp.left, rtTemp.bottom,
													FRAMEID_PET_SPECIAL, 2, 
													TRUE, FRAMEID_POPUP_SUBMENU, IDS_SACK_OPEN, 
													TRUE, FRAMEID_POPUP_SUBMENU, IDS_SETFREE);
				}
				else
				{
					g_pUIManager->MakePopSubMenu(	2, rtTemp.left, rtTemp.bottom,
													FRAMEID_PET_SPECIAL, 2, 
													FALSE, FRAMEID_POPUP_SUBMENU, IDS_SACK_OPEN, 
													TRUE, FRAMEID_POPUP_SUBMENU, IDS_SETFREE);
				}
			}
		}
		break;
	}
}







/**
 * 문파 비석 링 메뉴
 * \param lParam 
 */
void ProcessPopMenuStone(LPARAM lParam)
{
	int nControlID = LOWORD(lParam);
	//int nEventType = HIWORD(lParam);

	// 매품패
	if(g_MainCharInfo.m_pQuickMart)
	{		
		g_MainCharInfo.HideSack(SACKTYPE__QUICKMART, FALSE);		
	}

	switch(nControlID)
	{
	case 0:		// 관리
		{
			sRect rtTemp;
			g_pUIManager->GetRegionData(FRAMEID_STONE, 0, rtTemp);

			g_pUIManager->MakePopSubMenu(1, rtTemp.right, rtTemp.top - 96,
										FRAMEID_STONE_SUB_1, 4,
										TRUE, FRAMEID_POPUP_SUBMENU, IDS_STONE_MUNPA_MARK_DEL,
										TRUE, FRAMEID_POPUP_SUBMENU, IDS_STONE_MUNPA_NAME,
										TRUE, FRAMEID_POPUP_SUBMENU, IDS_STONE_MUNPA_ADAN,
										TRUE, FRAMEID_POPUP_SUBMENU, IDS_STONE_MUNPA_BBS);
		}
		break;
	case 1:		// 경제
		{
			sRect rtTemp;
			g_pUIManager->GetRegionData(FRAMEID_STONE, 1, rtTemp);

			g_pUIManager->MakePopSubMenu(1, rtTemp.right, rtTemp.bottom,
											FRAMEID_STONE_SUB_2, 2, 
											FALSE, FRAMEID_POPUP_SUBMENU, IDS_STONE_MUNPA_BANK,		// 문파 창고
											FALSE, FRAMEID_POPUP_SUBMENU, IDS_STONE_MUNPA_SHOP);	// 문파 상점
		}
		break;
	case 2:		// 신청
		{
			sRect rtTemp;
			g_pUIManager->GetRegionData(FRAMEID_STONE, 2, rtTemp);

			g_pUIManager->MakePopSubMenu(2, rtTemp.left, rtTemp.bottom,
											FRAMEID_STONE_SUB_3, 3, 
											TRUE, FRAMEID_POPUP_SUBMENU, IDS_WAR_AGREE, 
											TRUE, FRAMEID_POPUP_SUBMENU, IDS_STONE_MUNPA_JOIN,
											FALSE, FRAMEID_POPUP_SUBMENU, IDS_STONE_MUNPA_ALLIANCE);
		}
		break;
	}
}

/**
 * 정사관 NPC 링 메뉴
 * \param lParam 
 */
void ProcessPopMenuOfficial(LPARAM lParam)
{
	const int nControlID = LOWORD(lParam);
	const int nEventType = HIWORD(lParam);

	// 매품패
	if(g_MainCharInfo.m_pQuickMart)
	{		
		g_MainCharInfo.HideSack(SACKTYPE__QUICKMART, FALSE);		
	}

	switch(nControlID)
	{
	case 0:		// 대화
		break;
	case 1:		// 기부
		{
			g_MainCharInfo.OpenFrame(WINDOW_MUNPA_DONATE);

			g_pUIManager->SetString(WINDOW_MUNPA_DONATE, munpa_donate_dummy_05, g_MainCharInfo.m_dwMunpaFame, 1);
			g_pUIManager->SetString(WINDOW_MUNPA_DONATE, munpa_donate_edit, _T(""));
			g_pUIManager->SetFocus(WINDOW_MUNPA_DONATE, munpa_donate_edit);

			g_pUIManager->SetData(WINDOW_MUNPA_DONATE, munpa_donate_button_01, CURRENT_INDEX, 2);
		}
		break;
	case 2:		// 면죄
		break;
	default:
		break;
	} // switch(nControlID)

	g_pUIManager->DeletePopMenu();
}

/**
 * 연금술사 NPC 링 메뉴
 * \param lParam 
 */
void ProcessPopMenuAlchemist(LPARAM lParam)
{
	const int nControlID = LOWORD(lParam);
	const int nEventType = HIWORD(lParam);

	// 매품패
	if(g_MainCharInfo.m_pQuickMart)
	{		
		g_MainCharInfo.HideSack(SACKTYPE__QUICKMART, FALSE);		
	}

	switch(nControlID)
	{
		case 0:		// 대화
			break;
		case 1:		// 제련
			{
				// 오행 제련
				CloseAllWindow();

				g_MainCharInfo.ShowSack(SACKTYPE__FIVEELEMENT_CONVERT);
				g_MainCharInfo.ShowSack(SACKTYPE__DEFAULT);
				g_MainCharInfo.ShowSack(SACKTYPE__EQUIPMENT);
			}
			break;
		case 2:		// 조합
			{
				CloseAllWindow();

				g_MainCharInfo.ShowSack(SACKTYPE__SMELT);
				g_MainCharInfo.ShowSack(SACKTYPE__DEFAULT);
				g_MainCharInfo.ShowSack(SACKTYPE__EQUIPMENT);
			}
			break;
		default:
			break;
	} // switch(nControlID)

	g_pUIManager->DeletePopMenu();
}


/**
 * 상서령 NPC 링 메뉴
 * \param lParam 
 */
void ProcessPopMenuHelp(LPARAM lParam)
{
	const int nControlID = LOWORD(lParam);

	// 매품패
	if(g_MainCharInfo.m_pQuickMart)
	{		
		g_MainCharInfo.HideSack(SACKTYPE__QUICKMART, FALSE);		
	}

	switch(nControlID)
	{
	case 0:		// 대화
		{
			CloseAllWindow();

			if(g_MainCharInfo.m_pChat && g_MainCharInfo.m_pChat->GetChatType() == LARGECHAT)
			{
				g_MainCharInfo.m_pChat->SetChatType(SMALLCHAT);
			}

			g_Helper.TalkShow(0);
			g_Helper.SetSelectID(0);
		}
		break;
	case 1:		// 파발
		{
		}
		break;
	case 2:		// 거래
		{
		}
		break;
	default:
		break;
	} // switch(nControlID)

	g_pUIManager->DeletePopMenu();
}


/**
 * NPC 포탈 이동 상서령
 * \param lParam 
 */
void ProcessPopMenuNpcPortal(LPARAM lParam)
{
	const int nControlID = LOWORD(lParam);

	switch(nControlID)
	{
	case 0:
		break;
	case 1:	// 이동
		{
			CloseAllWindow();

			if(g_MainCharInfo.m_pChat->GetChatType() == LARGECHAT)
			{
				g_MainCharInfo.m_pChat->SetChatType(SMALLCHAT);
			}

			//
			g_pUIManager->SetPosition(WINDOW_PORTAL, WINDOW_FIRST_XPOS-100, 240);

			g_pUIManager->SetString(WINDOW_PORTAL, window_portal_back_dummy, IDS_NPC_PORTAL);

			g_pUIManager->SetString(WINDOW_PORTAL, window_portal_button1, IDS_NPC_PORTAL_1);
			g_pUIManager->SetString(WINDOW_PORTAL, window_portal_button2, IDS_NPC_PORTAL_2);			

			g_pUIManager->Hide(WINDOW_PORTAL, window_portal_button3);
			g_pUIManager->Hide(WINDOW_PORTAL, window_portal_button4);

			g_pUIManager->Hide(WINDOW_PORTAL, window_portal_button5);
			g_pUIManager->Hide(WINDOW_PORTAL, window_portal_button6);
			g_pUIManager->Hide(WINDOW_PORTAL, window_portal_button7);			
			//

			g_pUIManager->SetPostMsg(WINDOW_NPC_PORTAL);

			g_MainCharInfo.OpenFrame(WINDOW_PORTAL);
		}
		break;
	case 2:
		break;
	default:
		break;
	}

	g_pUIManager->DeletePopMenu();
}

/**
 * 성녀
 * \param lParam 
 */
void ProcessPopMenuHelp2(LPARAM lParam)
{
	const int nControlID = LOWORD(lParam);

	XiahObject::CXiahObject* pObject = XiahObject::g_XiahObjectManager.FindXiahObject( MAKEOBJECTID( 0, g_MainCharInfo.m_dwPickedObject, OBJTYPE_FUNCTIONALNPC));

	if( pObject == NULL || pObject->m_pObject == NULL) 
	{
		DBG_LogFile( _T("ProcessPopMenuHelp2 1 fail"));
		return;
	}

	CXiahCharObject* pCharObject = reinterpret_cast<CXiahCharObject*>( pObject->m_pObject);
	if(pCharObject == NULL)
	{
		DBG_LogFile( _T("ProcessPopMenuHelp2 2 fail"));
		return;
	}

	switch(nControlID)
	{
	case 0:		// 대화
		{
			sArrayData* pQuestScript = XiahArrayIndex::g_QuestScript.GetData(g_MainCharInfo.m_dwPickedObject);

			if(pQuestScript)
			{
				bool bSuccess = false;
				int nTotal = pQuestScript->GetInt(3);

				for(int i=0; i < nTotal; ) // 같은 npc가 있기에 돌면서 검사
				{
					sQuestInfo* pQuestInfo = g_MainCharInfo.m_pQuest->FindQuest(pQuestScript->GetInt(1));

					if(pQuestInfo)
					{
						if(pQuestInfo->m_bProcessType == pQuestScript->GetInt(2)) // 현재 상태와 같은것
						{
							TCHAR strTemp[256]= {0,};

							_stprintf( strTemp, _T("%s|%s"), (LPCTSTR)pQuestScript->GetString(0), (LPCTSTR)pQuestScript->GetString(1));

							pCharObject->SetChatBox( timeGetTime(), strTemp, 111);
							bSuccess = true;
							break;
						}
					}

					++i;
					pQuestScript = XiahArrayIndex::g_QuestScript.GetSkipData(i, g_MainCharInfo.m_dwPickedObject); // i번째 이후 인덱스
				}

				sQuestInfo* pQuestInfo = g_MainCharInfo.m_pQuest->FindQuest(1008);

				if(pQuestInfo)
				{
					if(pQuestInfo->m_bProcessType == 0)
					{
						pCharObject->SetChatBox( timeGetTime(), IDS_QUEST_NPC_2, 111);
					}
					else
					{
						//if(!bSuccess && (pQuestInfo->m_bProcessType >= 2 && pQuestInfo->m_bProcessType <= 8))
						//{
							//pCharObject->SetChatBox( timeGetTime(), IDS_QUEST_NPC_2_2, 111);
						//}
						//else
						//{
						// 서버에서 퀘스트 검사 때문에 전송
						SendCS_NC_FUNCTIONALNPCITEMLIST_REQ( XiahMap::g_XiahMap.m_MapInfo.m_dwMapID, g_MainCharInfo.m_dwPickedObject, 0);
						//}
					}					
				}
				else
				{
					pCharObject->SetChatBox( timeGetTime(), IDS_QUEST_NPC_2, 111);
				}
			} // if(pQuestScript)
		}
		break;
	case 1:
		break;
	case 2:
		break;
	default:
		break;
	} // switch(nControlID)

	g_pUIManager->DeletePopMenu();
}

/**
 * 문파대전 관리인
 */
void ProcessPopMenuClanWar(LPARAM lParam)
{
	const int nControlID = LOWORD(lParam);

	TCHAR strTemp[128] = {0,};

	switch(nControlID)
	{
	case 0:	// 맵 지역 이동
		{
			CloseAllWindow();

			if(g_MainCharInfo.m_pChat->GetChatType() == LARGECHAT)
			{
				g_MainCharInfo.m_pChat->SetChatType(SMALLCHAT);
			}

			g_pUIManager->SetPosition(WINDOW_PORTAL, WINDOW_SECOND_XPOS-100, 240);

			g_pUIManager->SetString(WINDOW_PORTAL, window_portal_back_dummy, IDS_CLANWAR_NPC_PORTAL_00);

			g_pUIManager->SetString(WINDOW_PORTAL, window_portal_button1, IDS_CLANWAR_NPC_PORTAL_01);
			g_pUIManager->SetString(WINDOW_PORTAL, window_portal_button2, IDS_CLANWAR_NPC_PORTAL_02);
			g_pUIManager->SetString(WINDOW_PORTAL, window_portal_button3, IDS_CLANWAR_NPC_PORTAL_03);
			g_pUIManager->SetString(WINDOW_PORTAL, window_portal_button4, IDS_CLANWAR_NPC_PORTAL_04);
			
			g_pUIManager->SetString(WINDOW_PORTAL, window_portal_button5, IDS_CLANWAR_NPC_PORTAL_05); //HO_0906_07 문파대전 관리인 포탈기능 추가 : 마혈진 지역

			g_pUIManager->Show(WINDOW_PORTAL, window_portal_button3);
			g_pUIManager->Show(WINDOW_PORTAL, window_portal_button4);
			g_pUIManager->Show(WINDOW_PORTAL, window_portal_button5);
			
			g_pUIManager->Hide(WINDOW_PORTAL, window_portal_button6);
			g_pUIManager->Hide(WINDOW_PORTAL, window_portal_button7);
			//

			g_pUIManager->SetPostMsg(WINDOW_NPC_PORTAL__WAR);

			g_MainCharInfo.OpenFrame(WINDOW_PORTAL);
		}
		break;
	case 1:		// 문파대전 우승 상금
		{
			SendCS_WR_REWARD_REQ(0);
		}
		break;
	case 2:		// 문파대전 참가 신청
		{	
			g_pUIManager->ShowNotice(IDS_CLAN_WAR_APPLY, NOTICE_FRAME_OKCANCEL, NOTICE_FRAME_CLAN_WAR_APPLY);
		}
		break;
	default:
		break;
	}

	g_pUIManager->DeletePopMenu();
}

/**
 * //HT_1116
 * \param lParam (황궁무관 탐랑)
 */
void ProcessPopMenuRebirthItem(LPARAM lParam)
{
	const int nControlID = LOWORD(lParam);

	switch(nControlID)
	{
	case 0:	// 대화	
		{
			CloseAllWindow();

			if(g_MainCharInfo.m_pChat && g_MainCharInfo.m_pChat->GetChatType() == LARGECHAT)
			{
				g_MainCharInfo.m_pChat->SetChatType(SMALLCHAT);
			}

			g_Helper.TalkShow(99);//HO_0410_07 상서령 가이드 업데이트 : 기존 (19)에서 리스트 추가로 99로 변경
			g_Helper.SetSelectID(99);
		}
		break;
	case 1: // 제련
		{
			CloseAllWindow();

			g_MainCharInfo.ShowSack(SACKTYPE__FIVEELEMENT_CONVERT);
			g_MainCharInfo.ShowSack(SACKTYPE__DEFAULT);
			g_MainCharInfo.ShowSack(SACKTYPE__EQUIPMENT);
		}
		break;
	case 2: // 조합	
		{
			CloseAllWindow();

			g_MainCharInfo.ShowSack(SACKTYPE__SMELT);
			g_MainCharInfo.ShowSack(SACKTYPE__DEFAULT);
			g_MainCharInfo.ShowSack(SACKTYPE__EQUIPMENT);
		}
		break;
	default:
		break;
	}

	g_pUIManager->DeletePopMenu();
}


/**
 * //HT_0313 : 광명전 & 천황전
 * \param lParam 
 */
void ProcessPopMenuSecretRoom(LPARAM lParam)
{
	const int nControlID = LOWORD(lParam);

	switch(nControlID)
	{
	case 0:	// 광명전	
		{
			CloseAllWindow();

			if(g_MainCharInfo.m_pChat && g_MainCharInfo.m_pChat->GetChatType() == LARGECHAT)
			{
				g_MainCharInfo.m_pChat->SetChatType(SMALLCHAT);
			}
			g_Helper.SetSelectID(111);
			g_Helper.TalkListShow(111);
		}
		break;
	case 1: // 천황전
		{
			CloseAllWindow();

			if(g_MainCharInfo.m_pChat && g_MainCharInfo.m_pChat->GetChatType() == LARGECHAT)
			{
				g_MainCharInfo.m_pChat->SetChatType(SMALLCHAT);
			}
			g_Helper.SetSelectID(114);
			g_Helper.TalkListShow(114);
		}
		break;
	default:
		break;
	}

	g_pUIManager->DeletePopMenu();
}

//////////////////////////////////////////////////////////
// PopSubMenu Frame
//////////////////////////////////////////////////////////
void ProcessPopMenuRelation( LPARAM lParam)
{
	int controlID = LOWORD( lParam);
	//int eventType = HIWORD( lParam);

	switch( controlID)
	{
	case 0:	// 단
		{
			if( g_MainCharInfo.m_pRelation->Am_I_InDan())
			{
				if( g_MainCharInfo.m_pRelation->Am_I_LeaderInDan())
				{
					SendCS_IF_INVITEPARTY_REQ( g_MainCharInfo.m_dwObjectID, g_MainCharInfo.m_dwPickedObject, 0);
					g_MainCharInfo.ShowHelpMessage(IDS_SEND_INVITE_DAN_MSG);
				}
				else
				{
					g_MainCharInfo.ShowHelpMessage(IDS_DANJU_INVITE, TEXTEFFECT_COLOR_WARNING);
				}
			}
			else
			{
				SendCS_IF_ASKPARTY_REQ( g_MainCharInfo.m_dwObjectID, g_MainCharInfo.m_dwPickedObject, 0);
				g_MainCharInfo.ShowHelpMessage(IDS_SEND_INVITE_DAN_MSG);
			}
		}
		break;

	case 1:	// 관계단
		{
			if(g_MainCharInfo.m_pRelation->Am_I_InDan())
			{
				if(g_MainCharInfo.m_pRelation->Am_I_LeaderInDan())
				{
					SendCS_IF_INVITEPARTY_REQ( g_MainCharInfo.m_dwObjectID, g_MainCharInfo.m_dwPickedObject, 0, 1);
					g_MainCharInfo.ShowHelpMessage(IDS_SEND_INVITE_DAN_MSG);
				}
				else
				{
					g_MainCharInfo.ShowHelpMessage(IDS_DANJU_INVITE, TEXTEFFECT_COLOR_WARNING);
				}
			}
			else
			{
				SendCS_IF_ASKPARTY_REQ( g_MainCharInfo.m_dwObjectID, g_MainCharInfo.m_dwPickedObject, 0, 1);
				g_MainCharInfo.ShowHelpMessage(IDS_SEND_INVITE_DAN_MSG);
			}
		}
		break;

	case 2:	// 문파
		{
			SendCS_RL_ASKMUNWON_REQ( ACT_ASKMUNWON_REQUEST, g_MainCharInfo.m_dwObjectID, g_MainCharInfo.m_dwPickedObject);
			g_MainCharInfo.ShowHelpMessage( ( IDS_SEND_INVITE_CLAN_MSG));
		}
		break;

	case 3:	// 친구
		{
			SendCS_RL_ASKRELATION_REQ( RELATION_TYPE_BUDDY, RELATION_STEP_ASK, g_MainCharInfo.m_dwObjectID, g_MainCharInfo.m_dwPickedObject );
			g_MainCharInfo.ShowHelpMessage( ( IDS_SEND_INVITE_FRIEND_MSG ) );
		}		
		break;

	case 4:	// 제자
		{
			SendCS_RL_ASKRELATION_REQ( RELATION_TYPE_TEACHER, RELATION_STEP_ASK, g_MainCharInfo.m_dwObjectID, g_MainCharInfo.m_dwPickedObject );
			g_MainCharInfo.ShowHelpMessage( ( IDS_REL_SEND_ASK_DISCIPLE_MSG ) );
		}		
		break;

	case 5:	// 연인
		{
			SendCS_RL_ASKRELATION_REQ( RELATION_TYPE_LOVER, RELATION_STEP_ASK, g_MainCharInfo.m_dwObjectID, g_MainCharInfo.m_dwPickedObject );
			g_MainCharInfo.ShowHelpMessage( ( IDS_REL_SEND_ASK_LOVER_MSG ) );
		}		
		break;	
	}

	g_pUIManager->DeletePopSubMenu();
	g_pUIManager->DeletePopMenu();	
}

void ProcessPopMenuAI( LPARAM lParam)
{
	int controlID = LOWORD( lParam);
	int eventType = HIWORD( lParam);

/*
#define	PETAI_NONE				0	// 아무것도 없슴 (PC를 따라 다님)
#define	PETAI_SELFGUARD			1	// 자기 보호	(기본상태임)
#define	PETAI_AUTOATTACK		2	// 자동공격
#define	PETAI_GUARD				3	// 대상보호
#define	PETAI_TARGETATTACK		4	// 대상공격
#define	PETAI_TAKEITEM			5	// 아이템 수집
#define	PETAI_SPECIALATTACK		6	// 무공공격
*/

	switch( controlID)
	{
	case 0:	// 호출
		g_PetList.Change_PET_AI(PETAI_CALLTOME);
		break;
	case 1: // 동반공격
		g_PetList.Change_PET_AI(PETAI_AUTOATTACK);		
		break;
	case 2: // 지정공격
		g_PetList.Change_PET_AI(PETAI_TARGETATTACK);
		g_bCommandAI = TRUE;
		g_dwCommandType = PETAI_TARGETATTACK;
		break;
	case 3:	// 무공공격
		break;
	case 4:	// 아이템수집
		g_PetList.Change_PET_AI(PETAI_TAKEITEM);
		break;
	}

	g_pUIManager->DeletePopSubMenu();
	g_pUIManager->DeletePopMenu();	
}

void ProcessPopMenuSpecial( LPARAM lParam)
{
	int controlID = LOWORD( lParam);
	//int eventType = HIWORD( lParam);

	switch( controlID)
	{
	case 0:	// 배낭열기
		ProcessClickPetSackButton();		
		break;

	case 1:	// 놓아주기
		SendCS_NC_STATUSCHANGE_REQ( OBJTYPE_PET, g_MainCharInfo.m_dwPickedObject, NPCSTATUS_DIE, 0);
		//g_MainCharInfo.ShowHelpMessage( IDS_FREE_PET);	

		// 봉인하기
		/*
		XiahItem::sItemInfo* pInfo = g_MainCharInfo.m_pMySack[g_MainCharInfo.m_byMySackCurrIdx]->FindSackItemByVisualID( 9200);
		if( pInfo)
		{
			SendCS_NC_PETBONGIN_REQ( g_MainCharInfo.m_dwPickedObject, pInfo->m_bSackPos);
		}
		else
		{
			g_MainCharInfo.ShowHelpMessage( IDS_NO_BONGIN_ITEM,TEXTEFFECT_COLOR_WARNIN);
		}*/
		break;
	}

	g_pUIManager->DeletePopSubMenu();
	g_pUIManager->DeletePopMenu();
}


/**
 * 문파전 비석 서브 관리
 * \param lParam 
 */
void ProcessPopMenuStoneSub1(LPARAM lParam)
{
	int nControlID = LOWORD(lParam);
	//int nEventType = HIWORD(lParam);

	switch(nControlID)
	{
	case 0:		// 문파 문장 삭제
		{
			g_pUIManager->ShowNotice(IDS_MUNPAMARK_DEL, NOTICE_FRAME_OKCANCEL, NOTICE_FRAME_MARK_DEL);
		}
		break;
	case 1:		// 문파 문장 등록
		{
			TCHAR strFile[128] = {0,};
			_stprintf(strFile, "mark.bmp");

			bool bCheck = false;
			D3DXIMAGE_INFO imageinfo;

Check:	// goto
			// 파일 검사			
			if(FAILED(D3DXGetImageInfoFromFile(strFile, &imageinfo)))
			{
				if(bCheck)
				{
					g_MainCharInfo.ShowHelpMessage(IDS_MUNPAMARK_NOTFILE, TEXTEFFECT_COLOR_WARNING);
					break;
				}

				// mark.bmp.bmp 파일도 검사 (일부 유저가 파일을 이렇게 만드는 경우가 있다.)
				_stprintf(strFile, "mark.bmp.bmp");
				bCheck = true;

				goto Check;
			}
			else
			{
				if(imageinfo.ImageFileFormat != D3DXIFF_BMP)
				{
					g_MainCharInfo.ShowHelpMessage(IDS_MUNPAMARK_NOBMP, TEXTEFFECT_COLOR_WARNING);
					break;
				}

				if(imageinfo.Width != 16 || imageinfo.Height != 16)
				{
					g_MainCharInfo.ShowHelpMessage(IDS_MUNPAMARK_MISTAKEN, TEXTEFFECT_COLOR_WARNING);
					break;
				}

				if(imageinfo.Format != D3DFMT_R8G8B8)
				{
					g_MainCharInfo.ShowHelpMessage(IDS_MUNPAMARK_24BPP, TEXTEFFECT_COLOR_WARNING);
					break;
				}
			}

			LPDIRECT3DTEXTURE9 pTexture = NULL;

			if(FAILED(D3DXCreateTextureFromFileEx(g_pDirect3DDevice, strFile, 
									D3DX_DEFAULT, D3DX_DEFAULT, D3DX_DEFAULT, 0, D3DFMT_UNKNOWN, 
									D3DPOOL_MANAGED, D3DX_FILTER_POINT , D3DX_FILTER_LINEAR,
									D3DCOLOR_ARGB(0xFF, 255, 0, 255), NULL, NULL, 
									&pTexture)))
			{
				g_MainCharInfo.ShowHelpMessage(IDS_MUNPAMARK_NOTOPEN, TEXTEFFECT_COLOR_WARNING);
				break;
			}

			g_pUIManager->SetData(MESSAGE_WINDOW_MARK, mark_window_mark_dummy, TYPE, STATIC);
			g_pUIManager->SetData(MESSAGE_WINDOW_MARK, mark_window_mark_dummy, OUTSIDE_TEXTURE, 1, 0, (DWORD)pTexture);

			g_MainCharInfo.OpenFrame(MESSAGE_WINDOW_MARK);
		}
		break;
	case 2:		// 문파비석 포기
		{
			g_pUIManager->ShowNotice(IDS_STONE_DELETE, NOTICE_FRAME_OKCANCEL, NOTICE_FRAME_STONE_RESIGN);
		}
		break;
	case 3:		// 문파 게시판
		{
			XiahObject::CXiahObject* pObject = XiahObject::g_XiahObjectManager.FindXiahObject(MAKEOBJECTID( 0, g_MainCharInfo.m_dwPickedObject, OBJTYPE_FUNCTIONALNPC));
			if(pObject == NULL) break;
			if(pObject->m_pObject == NULL) break;
			CXiahCharObject* pCharObject = reinterpret_cast<CXiahCharObject*>(pObject->m_pObject);
			if(pCharObject == NULL) break;
			sFunctionalNpcInfo* pInfo = (sFunctionalNpcInfo*)pCharObject->m_pPrivateData;

			//YS_0811 : BUGFIX
			if ( !pInfo ) break;

			SendCS_RL_MUNPAINFO_REQ(1, pInfo->m_dwOwnID);

			CloseAllWindow();
		}
		break;
	} // switch(nControlID)

	g_pUIManager->DeletePopSubMenu();
	g_pUIManager->DeletePopMenu();	
}

/**
 * 문파전 비석 서브 경제	- 현재 기능 미사용
 * \param lParam 
 */
void ProcessPopMenuStoneSub2(LPARAM lParam)
{
	int nControlID = LOWORD(lParam);
	int nEventType = HIWORD(lParam);

	switch(nControlID)
	{
	case 0:		// 문파 창고
		break;
	case 1:		// 문파 상점
		break;
	} // switch(nControlID)

	g_pUIManager->DeletePopSubMenu();
	g_pUIManager->DeletePopMenu();	
}

/**
 * 문파전 비석 서브 신청
 * \param lParam 
 */
void ProcessPopMenuStoneSub3(LPARAM lParam)
{
	int nControlID = LOWORD(lParam);
	int nEventType = HIWORD(lParam);

	switch(nControlID)
	{
	case 0:		// 문파전 신청
		{
			// 횅땍
			XiahObject::CXiahObject* pObject = XiahObject::g_XiahObjectManager.FindXiahObject(MAKEOBJECTID( 0, g_MainCharInfo.m_dwPickedObject, OBJTYPE_FUNCTIONALNPC));
			if(pObject == NULL) return;
			if(pObject->m_pObject == NULL) return;
			CXiahCharObject* pCharObject = reinterpret_cast<CXiahCharObject*>( pObject->m_pObject);
			sFunctionalNpcInfo* pInfo = (sFunctionalNpcInfo*)pCharObject->m_pPrivateData;
			if(pInfo == NULL) return;

			SendCS_WR_PRECHALLENGEWAR_REQ(pInfo->m_dwOwnID);

			g_MainCharInfo.m_pRelation->m_dwMunpaWarTime = 999;
			g_MainCharInfo.m_pRelation->m_bStealStone = 0;

			g_pUIManager->SetString(WINDOW_MUNPA_WAR_PETITION, munpa_war_petition_select_dummy_01, _T(" "));
			g_pUIManager->SetString(WINDOW_MUNPA_WAR_PETITION, munpa_war_petition_select_dummy_02, IDS_MUNPA_WAR_STONE_1, 5);
		}
		break;
	case 1:		// 문원 가입 신청
		{
			XiahObject::CXiahObject* pObject = XiahObject::g_XiahObjectManager.FindXiahObject(MAKEOBJECTID( 0, g_MainCharInfo.m_dwPickedObject, OBJTYPE_FUNCTIONALNPC));
			if(pObject == NULL) return;
			if(pObject->m_pObject == NULL) return;
			CXiahCharObject* pCharObject = reinterpret_cast<CXiahCharObject*>( pObject->m_pObject);
			sFunctionalNpcInfo* pInfo = (sFunctionalNpcInfo*)pCharObject->m_pPrivateData;
			if(pInfo == NULL) return;


			SendCS_RL_ASKMUNWON_REQ(ACT_ASKSTONE_REQUEST, g_MainCharInfo.m_dwObjectID, pInfo->m_dwOwnID);
			g_MainCharInfo.ShowHelpMessage( ( IDS_SEND_INVITE_CLAN_MSG));
		}
		break;
	case 2:		// 동맹 신청	- 미사용
		break;
	} // switch(nControlID)

	g_pUIManager->DeletePopSubMenu();
	g_pUIManager->DeletePopMenu();	
}


//////////////////////////////////////////////////////////
// PopSubMenu Frame(Combo)
//////////////////////////////////////////////////////////
void ProcessPopMenuBongInItem( LPARAM lParam)
{
	int controlID = LOWORD( lParam);
	int eventType = HIWORD( lParam);

	XiahItem::sItemInfo* pInfo = g_MainCharInfo.m_pMySack[ g_MainCharInfo.m_byMySackCurrIdx]->FindSackItemByID( g_MainCharInfo.m_dwCurrentSelectedBongInItem);
	if( !pInfo)
	{
		DBG_LogFile( _T("ProcessPopMenuBongInItem fail"));
	}

	// 봉인하려면 펫이 있어야 한다.
	//YS_0812 : BUGFIX
	if( g_PetList.size() > controlID )
	{		
		switch( controlID)
		{
		case 0:
			SendCS_NC_PETBONGIN_REQ( g_PetList.GetPetInfoByIndex(0)->dwID, pInfo->m_bSackCount+1, pInfo->m_bSackPos);
			break;
		case 1:
			SendCS_NC_PETBONGIN_REQ( g_PetList.GetPetInfoByIndex(1)->dwID, pInfo->m_bSackCount+1, pInfo->m_bSackPos);
			break;
		case 2:
			SendCS_NC_PETBONGIN_REQ( g_PetList.GetPetInfoByIndex(2)->dwID, pInfo->m_bSackCount+1, pInfo->m_bSackPos);
			break;
		}	
	}

	g_pUIManager->DeletePopSubMenu();
}

/**
 * 아이템몰 전낭 콤보
 * \param lParam 
 */
void ProcessPopMenuPurseItem(LPARAM lParam)
{
	const int nControlID = LOWORD(lParam);
	const int nEventType = HIWORD(lParam);

	// TODO: 아래 입/출금 이벤트 타입으로 바꿀것
	switch(nControlID)
	{
	case 0:	// 입금
		{
			g_pUIManager->SetString(WINDOW_PURSE, window_purse_dummy_02, IDS_PURSE_IN);
			g_pUIManager->Hide(WINDOW_PURSE, window_purse_dummy_03);

			g_MainCharInfo.m_byPurseAction = ACT_MONEYBAG_INPUT;
		}
		break;

	case 1:	// 출금
		{
			g_pUIManager->SetString(WINDOW_PURSE, window_purse_dummy_01, IDS_PURSE_OUT_DES);
			g_pUIManager->SetString(WINDOW_PURSE, window_purse_dummy_02, IDS_PURSE_OUT);
			g_pUIManager->Show(WINDOW_PURSE, window_purse_dummy_03);

			g_MainCharInfo.m_byPurseAction = ACT_MONEYBAG_OUTPUT;
		}
		break;

	default:	// 에러 메시지
		break;
	}

	g_MainCharInfo.OpenFrame(WINDOW_PURSE);

	g_pUIManager->SetFocus(WINDOW_PURSE);
	g_pUIManager->SetFocus(WINDOW_PURSE, window_purse_edit);

	g_pUIManager->DeletePopSubMenu();
}


/**
 * 대전 시간 설정
 * \param lParam 
 */
void ProcessPopMenuWarDay(LPARAM lParam)
{
	const int nControlID = LOWORD(lParam);

	std::map<int, sMunpaWarDay*>::iterator iter = g_MainCharInfo.m_pRelation->m_mMunpaWarDayList.find(nControlID);

	if(iter != g_MainCharInfo.m_pRelation->m_mMunpaWarDayList.end())
	{
		sMunpaWarDay *pInfo = iter->second;

		g_MainCharInfo.m_pRelation->m_dwMunpaWarTime = pInfo->dwGameTime;

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

		g_pUIManager->SetString(WINDOW_MUNPA_WAR_PETITION, munpa_war_petition_select_dummy_01, strDay);
	}

	g_pUIManager->DeletePopSubMenu();
}

/**
 * 문파 비석 이전 설정
 * \param lParam 
 */
void ProcessPopMenuStoneMove(LPARAM lParam)
{
	const int nControlID = LOWORD(lParam);

	g_MainCharInfo.m_pRelation->m_bStealStone = nControlID;

	switch(nControlID)
	{
	case 0:
		g_pUIManager->SetString(WINDOW_MUNPA_WAR_PETITION, munpa_war_petition_select_dummy_02, IDS_MUNPA_WAR_STONE_1);
		break;
	case 1:
		g_pUIManager->SetString(WINDOW_MUNPA_WAR_PETITION, munpa_war_petition_select_dummy_02, IDS_MUNPA_WAR_STONE_2);
		break;
	default:
		break;
	} // switch(nControlID)

	g_pUIManager->DeletePopSubMenu();
}

/**
* 개인 거래 종류 선택
* \param lParam 
*/
void ProcessPopMenuPcTrade(LPARAM lParam)
{
	int nControlID = LOWORD(lParam);

	switch(nControlID)
	{
	case 0:		// 일반 거래
		SendCS_EC_ASKTRADE_REQ( 0, g_MainCharInfo.m_dwObjectID, g_MainCharInfo.m_dwPickedObject);
		g_MainCharInfo.ShowHelpMessage(IDS_SEND_TRADE);
		break;
	case 1:		// 펫 거래
		{
			if(g_PetList.size())
			{
				SendCS_NC_PREPETTRADE_REQ(g_MainCharInfo.m_dwPickedObject, g_PetList.GetPetInfoByIndex(0)->dwID);
			}
			else
				g_MainCharInfo.ShowHelpMessage(IDS_PETTRADE_NOTHASPET, TEXTEFFECT_COLOR_WARNING);
		}
		break;
	default:
		break;
	}

	g_pUIManager->DeletePopMenu();
	g_pUIManager->DeletePopSubMenu();
}

/**
* 펫 복구 리스트 (5개)
* \param lParam 
*/
void ProcessPopMenuRevival(LPARAM lParam)
{
	int nControlID = LOWORD(lParam);

	g_MainCharInfo.m_nTempValue = nControlID;

	map<BYTE, sPetRevival*>::iterator iter = g_PetList.m_mPetRevivalList.find(nControlID);

	CloseAllWindow();

	if(iter != g_PetList.m_mPetRevivalList.end())
	{
		sPetRevival *pPetInfo = iter->second;

		if(pPetInfo != NULL)
		{
			TCHAR strTemp[64] = {0, };
			_stprintf(strTemp, IDS_PET_REVIAL, (LPCTSTR)(pPetInfo->strName));

			g_pUIManager->ShowNotice(strTemp, NOTICE_FRAME_OKCANCEL, NOTICE_FRAME_PET_REVIVAL);
		}
	}

	g_pUIManager->DeletePopSubMenu();
}

/**
 * 빙정 봉인
 * \param lParam 
 */
void ProcessPopMenuCryolite(LPARAM lParam)
{
	//int nControlID = LOWORD(lParam);
	CloseAllWindow();

	g_pUIManager->ShowNotice(IDS_PET_CRYOLITE, NOTICE_FRAME_OKCANCEL, NOTICE_FRAME_PET_BONGIN);
}

/**
 * 복권 - 구매,당첨번호횅땍
 * \param lParam 
 */
void ProcessPopMenuLotto(LPARAM lParam)
{
	int nControlID = LOWORD(lParam);

	switch(nControlID)
	{
	case 0:	// 구매
		{
			g_MainCharInfo.OpenFrame(WINDOW_BOK_NUMBER);

			for(int i=0; i < 25; ++i)
				g_pUIManager->SetData(WINDOW_BOK_NUMBER, bok_number_number_button_01 + i, CURRENT_INDEX, -1);

		}
		break;
	case 1:	// 당첨번호횅땍
		{
			SendCS_EC_PRIZELOTTOINFO_REQ(1);
		}
		break;
	default:
		break;
	}

	g_pUIManager->DeletePopMenu();
	g_pUIManager->DeletePopSubMenu();
}

/**
 * 복권아이템 당첨 횅땍/당첨금 수령
 * \param lParam 
 */
void ProcessPopMenuLottoCheck(LPARAM lParam)
{
	int nControlID = LOWORD(lParam);

	switch(nControlID)
	{
	case 0:	// 복권아이템 당첨 횅땍
		{
			SendCS_EC_CHECKLOTTO_REQ(g_MainCharInfo.m_dwResItemID,
									g_MainCharInfo.m_ReairSackID,
									g_MainCharInfo.m_RpairItemPos);

		}
		break;
	case 1:	// 당첨금 수령
		{
			g_pUIManager->SetString(WINDOW_MONEY, money_window_dummy_01, IDS_LOTTO_RECEIVE);
			g_pUIManager->SetString(WINDOW_MONEY, money_window_dummy_02, IDS_LOTTO_RECEIVE_INFO);

			g_MainCharInfo.OpenFrame(WINDOW_MONEY);
			g_pUIManager->SetPostMsg(WINDOW_MONEY_LOTTO);
		}
		break;
	default:
		break;
	} // switch(nControlID)

	g_pUIManager->DeletePopMenu();
	g_pUIManager->DeletePopSubMenu();
}