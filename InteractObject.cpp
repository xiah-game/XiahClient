#define MAIN_CHAROBJECT	((CXiahCharObject*)(g_pMainChar->m_pObject))

//---------------------------------------------------------------------------------------
// mode값
/*
	특정 오브젝트를 Type에 따라서 Action될 수 있도록 하는 함수
	Functional NPC : Ring Interface
	PC			   : Ring Interface
	Item		   : 줍기
	NPC			   : 공격(단일 공격)

	0 : 왼쪽 버튼
	1 : 오른쪽 버튼
	2 : 기타 (왼쪽 버튼을 계속 누르고 있거나, 자동 공격등일때)
	-> 나머지는 알아서 추가
*/
BOOL InteractObject(DWORD dwObjectID,BYTE bObjType,int mode)
{
	// Valid체크를 한번 더 해준다
	XiahObject::CXiahObject* pObject = XiahObject::g_XiahObjectManager.FindXiahObject( MAKEOBJECTID( 0, dwObjectID, bObjType));

	if( pObject == NULL)
		return TRUE;

	if( pObject->m_pObject == NULL )
		return TRUE;

	if( !pObject->m_pObject->IsA( XiahObject::eXOT_CharObject))
		return TRUE;

	// 음
	if( bObjType != OBJTYPE_NPC && mode == 2)
	{
		// npc가 아닌 넘들은 연타 같은게 없으니까. 계속 Interaction되는걸 막아준다
		return TRUE;
	}

	if( bObjType == OBJTYPE_PET && mode == 0)
	{
		return TRUE;
	}


	CXiahCharObject* pCharObject = reinterpret_cast<CXiahCharObject*>( pObject->m_pObject);
	CXiahCharObject* pMainChar = reinterpret_cast<CXiahCharObject*>( g_pMainChar->m_pObject);

	if(pCharObject == NULL || pMainChar == NULL)
	{
		DBG_LogFile( _T("InteractObject 실패"));
		return false;
	}

	if(g_dwSelectMugongID == GUM_ILKICHAM)
	{
		XiahObject::g_pMouseOnObject = XiahObject::g_pMouseOnObjectSave;
		ProcessRButtonDown( pMainChar, 0);			

		g_dwSelectMugongID = 0;
		return true;
	}

	if( bObjType == OBJTYPE_PC && mode == 0)
	{
		// 자동 모드에서 PC면 계속 따라다니기만 한다
		return TRUE;
	}

	float fInteractDistance = pCharObject->GetInteractionDistance( pMainChar->m_Position);

	// 연속 공격 가능 시간체크
	if( g_dwCurTime - g_MainChar_PreAttackInfo.dwLastPreAttackTime > 300 &&
		g_MainChar_PreAttackInfo.nRemainAttackCount == 0 && !bMainCharDie && fInteractDistance <= fInteractionRange)
		bAttackable = TRUE;
	else
		bAttackable = FALSE;

	// 필요한건 미리 뽑아둔다
	WORD wPosX;
	WORD wPosY;

	pCharObject->GetPosition( wPosX, wPosY);

	// 선택된 오브젝트 화면좌표
	Vector3 scPos;
	if( g_pCurrentCamera)
	{
		scPos = g_pCurrentCamera->WorldToScreen( pCharObject->m_Position + Vector3( 0, 6, 0));
	}

	switch( pCharObject->m_bObjType)
	{
	case OBJTYPE_PC:	// PC
		{
			if( fInteractDistance <= fInteractionRange)
			{
				if(pCharObject->m_bTradeSell)
				{
					// 개인 상점 캐릭터
					if( GetAsyncKeyState( VK_CONTROL) < 0)
					{
						if( !g_pUIManager->IsPopMenu() && !g_pUIManager->IsShow(WINDOW_NPC_TRADE))	 // 이미 Popup이 떠 있지 않으면
						{							
							g_pUIManager->MakePopMenu(	scPos.x, scPos.y,
														FRAMEID_PC, 3, 
														FALSE, RESID_COMMUNICATION, IDS_CONVERSATION, 
														FALSE, RESID_RELATION, IDS_RELATION,
														TRUE, RESID_TRADE, IDS_DEAL);

							g_MainCharInfo.m_dwPickedObject = pObject->m_dwServerID;
							g_MainCharInfo.m_bPickType = 1; // 개인상점 거래
						}
					}
				}
				else // 일반 캐릭터
				{
					//  일단 거래일건지를 먼저 검사. 그래야 비무에서도 메뉴 뜬다
					if( GetAsyncKeyState( VK_CONTROL) < 0)
					{
						if( g_MainCharInfo.GetCurrSendChatType() == CT_WHISPER)
						{
							g_MainCharInfo.m_strWhisperName = (LPCTSTR)g_MainCharInfo.FindNameByID( dwObjectID);
							g_pUIManager->SetString(MAIN_CHAT, chat_name_edit, g_MainCharInfo.m_strWhisperName);					
						}

						if( !g_pUIManager->IsPopMenu() && !g_pUIManager->IsShow(WINDOW_PC_TRADE))	 // 이미 Popup이 떠 있지 않으면
						{							
							BOOL bDawWar = FALSE;
							if(pCharObject->m_dwPartyLeaderID == dwObjectID && g_MainCharInfo.m_pRelation->Am_I_LeaderInDan())
								bDawWar = TRUE;
							else
								bDawWar = FALSE;

							g_pUIManager->MakePopMenu(scPos.x, scPos.y,
													FRAMEID_PC, 3, 
													bDawWar, RESID_QUEST_FIGHT, IDS_DAWWAR_ICON, 
													TRUE, RESID_RELATION, IDS_RELATION,
													TRUE, RESID_TRADE, IDS_DEAL);

							g_MainCharInfo.m_dwReserveID	= pObject->m_dwServerID;
							g_MainCharInfo.m_dwPickedObject = pObject->m_dwServerID;
							g_MainCharInfo.m_bPickType		= 0;
						}
					}
					else	
					{
						// 공격. 일반적으로 쉬프트를 눌러야 일반 캐릭 공격이 된다.
						if( ( GetAsyncKeyState( VK_SHIFT) < 0) || g_MainCharInfo.m_bIsPvPMap == TRUE)
						{
							if(MAIN_CHAROBJECT->GetAnimation() == XiahAniType::eLAT_Run)
							{
								SendCS_NV_ENDMOVE_REQ( g_pMainChar->m_dwServerID, pMainChar->m_Position.x, -pMainChar->m_Position.z, pMainChar->m_Position.y,CHARSTATE_NORMAL);
							}

							// 메인 케릭이 탈백인이 걸린 상태면 공격을 할 수 없다.
							if( !pMainChar->m_KeepUpMugongList.IsExist(OUTGONGID_TALBAKIN) )
							{
								SendCS_BT_PREATTACK_REQ( OBJTYPE_PC, g_pMainChar->m_dwServerID, pCharObject->m_bObjType, pObject->m_dwServerID,
														wPosX, wPosY, (BYTE)pMainChar->m_Position.y, 0);
								g_MainChar_PreAttackInfo.nRemainAttackCount = 1;
							}
						}
					}
				}

				if(! (GetAsyncKeyState(VK_CONTROL) < 0))
				{
					// 단 비무. 일땐 클릭이 공격이다.
					if( pMainChar->m_dwEnemyPartyID && pCharObject->m_dwPartyID &&
						pMainChar->m_dwEnemyPartyID == pCharObject->m_dwPartyID   )
					{
						if(MAIN_CHAROBJECT->GetAnimation() == XiahAniType::eLAT_Run)
						{
							SendCS_NV_ENDMOVE_REQ( g_pMainChar->m_dwServerID, pMainChar->m_Position.x, -pMainChar->m_Position.z, pMainChar->m_Position.y,CHARSTATE_NORMAL);
						}

						// 메인 케릭이 탈백인이 걸린 상태면 공격을 할 수 없다.
						if( !pMainChar->m_KeepUpMugongList.IsExist(OUTGONGID_TALBAKIN) )
						{
							SendCS_BT_PREATTACK_REQ( OBJTYPE_PC, g_pMainChar->m_dwServerID, pCharObject->m_bObjType, pObject->m_dwServerID,
													wPosX, wPosY, (BYTE)pMainChar->m_Position.y, 0);
							g_MainChar_PreAttackInfo.nRemainAttackCount = 1;
						}
					}

					// 문파전 공격. 일땐 클릭이 공격이다.
					if((pMainChar->m_bWarStatus == 2) && (pMainChar->m_dwEnemyMunpaID == pCharObject->m_dwMunpaID))
					{
						if(MAIN_CHAROBJECT->GetAnimation() == XiahAniType::eLAT_Run)
						{
							SendCS_NV_ENDMOVE_REQ( g_pMainChar->m_dwServerID, pMainChar->m_Position.x, -pMainChar->m_Position.z, pMainChar->m_Position.y,CHARSTATE_NORMAL);
						}

						// 메인 케릭이 탈백인이 걸린 상태면 공격을 할 수 없다.
						if( !pMainChar->m_KeepUpMugongList.IsExist(OUTGONGID_TALBAKIN) )
						{
							SendCS_BT_PREATTACK_REQ(OBJTYPE_PC, g_pMainChar->m_dwServerID, pCharObject->m_bObjType, pObject->m_dwServerID,
													wPosX, wPosY, static_cast<BYTE>(pMainChar->m_Position.y), 0);
							g_MainChar_PreAttackInfo.nRemainAttackCount = 1;
						}
					}

					// 마혈성 에서만 & 문파대전시
					if(XiahMap::g_XiahMap.m_MapInfo.m_dwMapID == 12 && g_MainCharInfo.m_bMunpaFight)
					{					
						if(pMainChar->m_dwMunpaID != pCharObject->m_dwMunpaID)
						{
							// 메인 케릭이 탈백인이 걸린 상태면 공격을 할 수 없다.
							if( !pMainChar->m_KeepUpMugongList.IsExist(OUTGONGID_TALBAKIN) )
							{
								SendCS_BT_PREATTACK_REQ(OBJTYPE_PC, g_pMainChar->m_dwServerID, pCharObject->m_bObjType, pObject->m_dwServerID,
														wPosX, wPosY, static_cast<BYTE>(pMainChar->m_Position.y), 0);
								g_MainChar_PreAttackInfo.nRemainAttackCount = 1;
							}
						}
					}
				} // if(! (GetAsyncKeyState(VK_CONTROL) < 0))
			} // if( fInteractDistance <= fInteractionRange)
		}
		break;

		/////////////////////////////////////////////////////////////////////////////////////////////////////
	case OBJTYPE_NPC:  // NPC 일반 공격 쩝
		{
			XiahItem::sItemInfo* pHoldItem = g_MainCharInfo.m_pHoldItem->GetHoldItemItem();

			if( pHoldItem && pHoldItem->m_bSackID == SACKTYPE__DEFAULT)
			{
				// 펫에 관련된 아이템
				if( pHoldItem->m_bItemType == ITEMTYPE_NPCITEM )
				{
					// 길들이기
					// [6/27/2005] 자소뿔
					if( pHoldItem->m_bItemKind == 0 || pHoldItem->m_bItemKind == 5)
					{
						if( fInteractDistance <= fInteractionRange)
						{
							g_MainCharInfo.m_pHoldItem->SetItemBackToSack();
							SendCS_IM_GIVEITEM_REQ( pHoldItem->m_bSackCount+1, pHoldItem->m_bSackPos, pHoldItem->m_dwItemID, pCharObject->m_bObjType, pObject->m_dwServerID);
							//g_MainCharInfo.m_pHoldItem->DeleteHoldItemItem();
							g_MainCharInfo.ShowHelpMessage( IDS_GIVE_BAIT);
							ChangeXiahCursor( eCT_General);

							// 마우스 초기화
							XiahInput::g_bLButtonDown  = FALSE;
							XiahInput::g_Attack_Button_On = FALSE;
						}
					}
					// 먹이
					else if( pHoldItem->m_bItemKind == 1 || pHoldItem->m_bItemKind == 2)
					{
						g_MainCharInfo.ShowHelpMessage(INTER_WARNNIG1);
					}
				}
			}
			else if( bAttackable && g_CursorType != eCT_Menu)
			{
				if(MAIN_CHAROBJECT->GetAnimation() == XiahAniType::eLAT_Run)
				{
					SendCS_NV_ENDMOVE_REQ( g_pMainChar->m_dwServerID, pMainChar->m_Position.x, -pMainChar->m_Position.z, pMainChar->m_Position.y,CHARSTATE_NORMAL);
				}

				// 메인 케릭이 탈백인이 걸린 상태면 공격을 할 수 없다.
				if( !pMainChar->m_KeepUpMugongList.IsExist(OUTGONGID_TALBAKIN) )
				{
					// 채집
					if(3 == pCharObject->m_bExSubObjType)
					{
						bAutoAttack = false;
						
						pMainChar->SetAngleTarget(wPosX, wPosY);

						WORD wCharAngle = 0;
						pMainChar->GetAngle(wCharAngle);

						// 채집을 알림
						SendCS_ACTION_REQ(17, 0, wCharAngle, pObject->m_dwServerID);
					}
					else
					{
						//HT_1026 : 스핵 방지
					//	if(g_MainChar_PreAttackInfo.bPreAttackReq && g_MainChar_PreAttackInfo.bAttackReq)
							SendCS_BT_PREATTACK_REQ( OBJTYPE_PC, g_pMainChar->m_dwServerID, pCharObject->m_bObjType, pObject->m_dwServerID,
													wPosX, wPosY, (BYTE)pMainChar->m_Position.y, 0);
					}					

					g_MainChar_PreAttackInfo.nRemainAttackCount = 1;
				}
			}

		}
		break;
		/////////////////////////////////////////////////////////////////////////////////////////////////////
	case OBJTYPE_ITEM: // Item
		{
			XiahItem::sItemInfo* pInfo = (XiahItem::sItemInfo*)pCharObject->m_pPrivateData;

			if ( NULL != pInfo )
			{
				SendCS_IM_PICK_REQ( pInfo->m_dwMapID, pInfo->m_dwItemID, wPosX, wPosY, 255, pInfo->m_dwMapObjectID, pInfo->m_dwAmount);
			}
		}
		break;

	case OBJTYPE_PET:
		{			
			XiahItem::sItemInfo* pHoldItem = g_MainCharInfo.m_pHoldItem->GetHoldItemItem();

			// 임시
			if(pHoldItem == NULL)
			{
				g_PetList.SelectPet(dwObjectID);
			}

			if( fInteractDistance <= fInteractionRange && g_PetList.Find( pObject->m_dwServerID) != NULL)
			{
				if( !pHoldItem && GetAsyncKeyState( VK_CONTROL) < 0)
				{
					g_pUIManager->MakePopMenu( scPos.x, scPos.y,
												FRAMEID_PET, 3,
												TRUE, RESID_STATUS, IDS_STATUS,
												TRUE, RESID_AI, IDS_AI,		
												TRUE, RESID_SPECIAL, IDS_MANAGE);	
				
					g_MainCharInfo.m_dwPickedObject = pObject->m_dwServerID;
				}
				else if( pHoldItem && pHoldItem->m_bSackID == SACKTYPE__DEFAULT)
				{					
					sPetInfo* pPetInfo = g_PetList.GetCurrentPet();					

					if(pPetInfo)
					{
						//먹이
						if( (pPetInfo->bWildRate != 0 && pHoldItem->m_bItemKind == 1) || (pPetInfo->dwHpCur != pPetInfo->dwHpMax && pHoldItem->m_bItemKind == 2 ) 
							|| (pHoldItem->m_bItemKind == 4) || (pHoldItem->m_bItemKind == 6) || (pHoldItem->m_bItemKind == 7 )||(pHoldItem->m_bItemKind == 8 )) //HT_0621 : 영수둔갑신단 //HO_0427_07 영수환골신단 pHoldItem->m_bItemKind == 7 영수각성신단8
						{
							SendCS_IM_GIVEITEM_REQ( pHoldItem->m_bSackCount+1, pHoldItem->m_bSackPos, pHoldItem->m_dwItemID, pCharObject->m_bObjType, pObject->m_dwServerID);
							//g_MainCharInfo.m_pHoldItem->DeleteHoldItemItem();
							
							if(pHoldItem->m_bItemKind != 7)
								g_MainCharInfo.ShowHelpMessage( IDS_FEED);

							ChangeXiahCursor( eCT_General);

							// 마우스 초기화
							XiahInput::g_bLButtonDown  = FALSE;
							XiahInput::g_Attack_Button_On = FALSE;
						}
						// 길들이기
						// [6/27/2005] 자소뿔
						else if( pHoldItem->m_bItemType == ITEMTYPE_NPCITEM && (pHoldItem->m_bItemKind == 0 || pHoldItem->m_bItemKind == 5))
						{
							g_MainCharInfo.ShowHelpMessage(INTER_WARNNIG2);
						}
						else
						{
							if(pHoldItem->m_bItemKind == 1)
								g_MainCharInfo.ShowHelpMessage( IDS_PET_WILD_NOT);
							else if(pHoldItem->m_bItemKind == 2)
								g_MainCharInfo.ShowHelpMessage( IDS_PET_HP_NOT);

							g_MainCharInfo.m_pHoldItem->SetItemBackToSack();
							ChangeXiahCursor( eCT_General);
						}
					}
				}
			}
		}
		break;

	case OBJTYPE_FUNCTIONALNPC: // NPC
		{
			if( !g_pUIManager->IsPopMenu() && !g_pUIManager->IsPopSubMenu())	 // 이미 Popup이 떠 있지 않으면
			{
				sFunctionalNpcInfo* pInfo = (sFunctionalNpcInfo*)pCharObject->m_pPrivateData;
				
				if(pInfo == NULL)
				{
					DBG_LogFile( _T("InteractObject/OBJTYPE_FUNCTIONALNPC NULL값"));
					break;
				}
				
				BOOL bSuriFlag = FALSE;
				BOOL bGejoFlag = FALSE;

				switch( pInfo->m_bType)
				{
					case 1: //대장장이
					case 2:	//의류상인
					case 3: //잡화상인
					case 4:	//보석상인
					case 8: //만물상인
						{
							bSuriFlag = TRUE;
							bGejoFlag = TRUE;
						}					
						break;
					case 31://HT_0829 : 프리미엄 퀘스트 (기연궤 판매상)
						{
							g_pUIManager->MakePopMenu(scPos.x, scPos.y,
													FRAMEID_NPC, 3, 
													false, RESID_REPAIR, IDS_SURI, 
													false, RESID_COMMUNICATION,  IDS_CONVERSATION,
													TRUE, RESID_TRADE, IDS_DEAL);

							goto MakePopPass;
						}
					case 5:	//제약사
						break;

					case 6:	//서점주인	// 복권용 임시대용
						{
							g_pUIManager->MakePopMenu(scPos.x, scPos.y,
													FRAMEID_NPC, 3, 
													false, RESID_REPAIR, IDS_SURI, 
													false, RESID_LOTTO, IDS_LOTTO_TITLE,
													TRUE, RESID_TRADE, IDS_DEAL);

							goto MakePopPass;
						}
						break;
					case 7:	//창고지기
						{
							// 복구
							g_pUIManager->MakePopMenu(	scPos.x, scPos.y,
														FRAMEID_NPC, 3, 
														TRUE, RESID_REPAIR, IDS_RECOVERY, 
														TRUE, RESID_ITEMMALL, IDS_ITEMMALL_NAME,
														TRUE, RESID_TRADE, IDS_DEAL);

							goto MakePopPass;
						}					
						break;

					// 복면인들
					case 9:
					case 10:
						{
							g_pUIManager->MakePopMenu(	scPos.x, scPos.y,
														FRAMEID_NPC, 3, 
														false, RESID_TRADE, IDS_DEAL, 
														false, RESID_QUEST_FIGHT, IDS_QUEST_FIGHT,
														true, RESID_QUEST_COMM, IDS_CONVERSATION);
							goto MakePopPass;
						}					
						break;

					case 11:	// 정사관
						{
							g_pUIManager->MakePopMenu(scPos.x, scPos.y,
														FRAMEID_OFFICIAL, 3, 
														false, RESID_COMMUNICATION, IDS_CONVERSATION,	// ?
														true, RESID_DONATE, IDS_GOV_CONTRIBUTE,
														false, RESID_ACQUIT, IDS_GOV_EXONERATION);
							goto MakePopPass;
						}					
						break;

						// 비석 유형
						//	12	997		0	0
						//	13	998		0	0
						//	14	999		0	0
						//	15	1001	0	0
						//	16	1002	0	0
						//	17	1004	0	0
					case 12:	// 문파 비석
					case 15:	// 공용 비석
						{
							g_MainCharInfo.m_dwPickedObject = pInfo->m_dwObjectID;

							if(pInfo->m_bMainStone == 1)
							{
								if(pInfo->m_dwOwnID)
								{
									if(pInfo->m_bWar == 2)		// 1대기 2전쟁	g_MainCharInfo.m_bWar
									{// 문파전 진행중
										if(pMainChar->m_dwMunpaID != pInfo->m_dwOwnID && pMainChar->m_bWarStatus == 2)	// 비석에 저장된 문파와 자신의 문파 비교
										{
											// 적 문파일때 공격
											if(pMainChar->m_dwMunpaID == pInfo->m_dwEnemyMunpaID)
											{
												// 메인 케릭이 탈백인이 걸린 상태면 공격을 할 수 없다.
												if( !pMainChar->m_KeepUpMugongList.IsExist(OUTGONGID_TALBAKIN) )
												{
													SendCS_BT_PREATTACK_REQ(OBJTYPE_PC, g_pMainChar->m_dwServerID, pCharObject->m_bObjType, pObject->m_dwServerID,
																			wPosX, wPosY, static_cast<BYTE>(pMainChar->m_Position.y), 0);
													g_MainChar_PreAttackInfo.nRemainAttackCount = 1;
												}
											}
										}
									} // if(g_MainCharInfo.m_bWar == 1)
									else
									{
											g_pUIManager->MakePopMenu(scPos.x, scPos.y,
																	FRAMEID_STONE, 3, 
																	true, RESID_ADMINISTER, IDS_STONE_GOV,
																	true, RESID_TRADE, IDS_STONE_ECONOMY,
																	true, RESID_TEACHER, IDS_STONE_APPLY);
									}
								}
								else
								{
									// 미소유 비석
									TCHAR strText[256] = {0,};
									_stprintf(strText, IDS_STONE_PAYMENT_DES, MoneyCommaStr(30000000).data());
									g_pUIManager->ShowNotice(strText, NOTICE_FRAME_OKCANCEL, NOTICE_FRAME_STONE_OWN);
								}
							} // if(pInfo->m_bMainStone == 1)
							else
							{
								if(pMainChar->m_bWarStatus == 2)
								{
									// 적 문파일때 공격
									if(pMainChar->m_dwMunpaID == pInfo->m_dwEnemyMunpaID)
									{
										// 메인 케릭이 탈백인이 걸린 상태면 공격을 할 수 없다.
										if( !pMainChar->m_KeepUpMugongList.IsExist(OUTGONGID_TALBAKIN) )
										{
											SendCS_BT_PREATTACK_REQ(OBJTYPE_PC, g_pMainChar->m_dwServerID, pCharObject->m_bObjType, pObject->m_dwServerID,
																	wPosX, wPosY, (BYTE)pMainChar->m_Position.y, 0);
											g_MainChar_PreAttackInfo.nRemainAttackCount = 1;
										}
									}
								}
							}

							return true;
						}
						break;
					case 18:	// 연금술사
						{
							g_pUIManager->MakePopMenu(scPos.x, scPos.y,
													FRAMEID_ALCHEMIST, 3, 
													false, RESID_REPAIR, IDS_CONVERSATION, 
													true, RESID_REFINE, IDS_REFINE,
													true, RESID_MIXTURE, IDS_MIXTURE);


							goto MakePopPass;
						}
						break;
					case 23:	// 상서령
						{
							g_pUIManager->MakePopMenu(scPos.x, scPos.y,
													FRAMEID_HELP, 3, 
													true, RESID_QUEST_COMM, IDS_CONVERSATION, 
													false, RESID_MODIFY, IDS_CONVERT,
													false, RESID_TRADE, IDS_DEAL);

							goto MakePopPass;
						}
						break;
					case 25:	// NPC 포탈 이동 상서령
						{
							g_pUIManager->MakePopMenu(scPos.x, scPos.y,
													FRAMEID_NPC_PORTAL, 3, 
													false, RESID_COMMUNICATION, IDS_CONVERSATION, 
													true, RESID_QUEST_COMM, IDS_MAP_MOVE,
													false, RESID_MODIFY, IDS_CONVERT);

							goto MakePopPass;
						}
						break;
					case 30:	// 문파대전 관리인 NPC (비선 적용중인 녹존 NPC 리소스)
						{
							g_pUIManager->MakePopMenu(scPos.x, scPos.y,
													FRAMEID_CLAN_WAR, 3, 
													true, RESID_QUEST_COMM, IDS_CONVERSATION, 
													true, RESID_TRADE, IDS_WORLDWARREWARD,
													true, RESID_QUEST_FIGHT, IDS_STONE_APPLY);

							goto MakePopPass;

						}
						break;
					case 40:	// 성녀
						{
							g_pUIManager->MakePopMenu(scPos.x, scPos.y,
													FRAMEID_HELP_2, 3,
													true, RESID_QUEST_COMM, IDS_CONVERSATION, 
													false, RESID_REPAIR, IDS_SURI,
													false, RESID_TRADE, IDS_DEAL);

							goto MakePopPass;
						}
						break;
					case 32:	//HT_1116 : 각성자 아이템 추가
						{
							g_pUIManager->MakePopMenu(scPos.x, scPos.y,
													FRAMEID_REBIRTHITEM, 3,
													true, RESID_QUEST_COMM, IDS_CONVERSATION, 
													true, RESID_REFINE, IDS_REFINE,
													true, RESID_MIXTURE, IDS_MIXTURE);
							goto MakePopPass;
						}
					case 33:	//HT_0313 : 광명전 & 천황전
						{							
							g_pUIManager->MakePopSubMenu(1,scPos.x + 40, scPos.y,
										FRAMEID_SECRET, 2,
										TRUE, FRAMEID_POPUP_SUBMENU, IDS_SECRETROOM,
										TRUE, FRAMEID_POPUP_SUBMENU, IDS_DEVILROOM);

							//goto MakePopPass; 
							return true;
						}

						break;
				}

				// 복구 활성화 - 코드 이동

				g_pUIManager->MakePopMenu(	scPos.x, scPos.y,
											FRAMEID_NPC, 3, 
											bSuriFlag, RESID_REPAIR, IDS_SURI, 
											bGejoFlag, RESID_MODIFY, IDS_CONVERT,
											TRUE, RESID_TRADE, IDS_DEAL);

MakePopPass:	// 일반 팝업 통과

				g_pUIManager->SetString(WINDOW_NPC_TRADE, npc_trade_window_title_back, pCharObject->m_szObjectName);

				g_MainCharInfo.m_dwPickedObject = pInfo->m_dwObjectID;
			
				// 상점 거래 성공하게 되면 대화 동작을 play해준다
				// 나중에 아이템을 구입하거나 팔거나 할때마다 이렇게 호출해주면 좋음!!
				pCharObject->SetAnimation( XiahAniType::eLAT_Special, XiahAniType::eLAT_Stand, 0, -1);
				pMainChar->SetAngleTarget( pCharObject);
				pCharObject->SetAngleTarget( pMainChar);
			}
		}
		break;
	}	
	
	return TRUE;
}