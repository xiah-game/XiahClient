
//////////////////////////////////////////////////////////
// Notice
//////////////////////////////////////////////////////////

///////////////////
// 1button
///////////////////
void ProcessNoticeFrameTerminate( int controlID)
{
	switch( controlID)
	{
		case message_window_1_button:
			PostMessage( g_AppData.m_hWnd, WM_CLOSE, 0, 0);
		break;
	}
}

void ProcessNoticeWindow1( LPARAM lParam)
{
	int controlID = LOWORD( lParam);
	int eventType = HIWORD( lParam);

	switch( eventType)
	{
	case NOTICE_FRAME_UNEXPECTED_TERMINATE:
		ProcessNoticeFrameTerminate( controlID);
		break;

/*	case NOTICE_FRAME_REBIRTH_SUCCESS:
		ProcessNoticeReBirthSuccess(controlID);
		break;
*/
	}

	g_pUIManager->HideNotice( MESSAGE_WINDOW_1BUTTON);
	g_pUIManager->DeletePopSubMenu();
	g_pUIManager->DeletePopMenu();
}

/*
void ProcessNoticeReBirthSuccess( int controlID)
{
	switch( controlID)
	{
		case message_window_1_button:
			ProcessWindowClose(4);
			g_MainCharInfo.m_bRebirthItem_Use = false;
		break;
	}
}
*/





///////////////////
// 2button
///////////////////
void ProcessNoticeFrameTrade( int controlID)
{
	switch( controlID)
	{
	case message_window_2_button_01:
		SendCS_EC_ASKTRADE_REQ(1, g_MainCharInfo.m_dwAskID, g_MainCharInfo.m_dwObjectID);
		break;
	case message_window_2_button_02:
		SendCS_EC_ASKTRADE_REQ(9, g_MainCharInfo.m_dwAskID, g_MainCharInfo.m_dwObjectID);
		break;
	}

	// [7/18/2005] 
	if( g_MainCharInfo.m_pHoldItem)
		g_MainCharInfo.m_pHoldItem->SetItemBackToSack();
}

void ProcessNoticeFrameAskParty( int controlID)
{
	// 1 : 수락
	// 9 : 거절
	switch( controlID)
	{
	case message_window_2_button_01:
		SendCS_IF_ASKPARTY_REQ( g_MainCharInfo.m_dwAskID, g_MainCharInfo.m_dwObjectID, 1);
		break;
	case message_window_2_button_02:
		SendCS_IF_ASKPARTY_REQ( g_MainCharInfo.m_dwAskID, g_MainCharInfo.m_dwObjectID, 9);
		break;
	}
}

void ProcessNoticeFrameInviteParty( int controlID)
{
	// 1 : 수락
	// 9 : 거절
	switch( controlID)
	{
	case message_window_2_button_01:
		SendCS_IF_INVITEPARTY_REQ( g_MainCharInfo.m_dwAskID, g_MainCharInfo.m_dwObjectID, 1);
		break;
	case message_window_2_button_02:
		SendCS_IF_INVITEPARTY_REQ( g_MainCharInfo.m_dwAskID, g_MainCharInfo.m_dwObjectID, 9);
		break;
	}
}

void ProcessNoticeFrameAskBuddy( int controlID)
{
	// 1 : 수락
	// 9 : 거절
	switch( controlID)
	{
	case message_window_2_button_01:
//		SendCS_IF_ASKADDBUDDY_REQ( g_MainCharInfo.m_dwAskID, g_MainCharInfo.m_dwObjectID, 1);
        SendCS_RL_ASKRELATION_REQ(g_MainCharInfo.m_byRelationType, RELATION_STEP_ACCEPT, g_MainCharInfo.m_dwAskID, g_MainCharInfo.m_dwObjectID);
		break;
	case message_window_2_button_02:
//		SendCS_IF_ASKADDBUDDY_REQ( g_MainCharInfo.m_dwAskID, g_MainCharInfo.m_dwObjectID, 9);
		SendCS_RL_ASKRELATION_REQ(g_MainCharInfo.m_byRelationType, RELATION_STEP_DECLINE, g_MainCharInfo.m_dwAskID, g_MainCharInfo.m_dwObjectID);

		break;
	}
}

void ProcessNoticeFrameClan( int controlID)
{
	switch( controlID)
	{
	case message_window_2_button_01:
		SendCS_RL_ASKMUNWON_REQ( ACT_ASKMUNWON_OK, g_MainCharInfo.m_dwAskID, g_MainCharInfo.m_dwObjectID);
		break;
	case message_window_2_button_02:
		SendCS_RL_ASKMUNWON_REQ( ACT_ASKMUNWON_CANCEL, g_MainCharInfo.m_dwAskID, g_MainCharInfo.m_dwObjectID);
		break;
	}
}

void ProcessNoticeFrameBuyItem( int controlID)
{
	XiahItem::sItemInfo* pHoldItem = g_MainCharInfo.m_pHoldItem->GetHoldItemItemFromNpcSack();

	if( !pHoldItem)
	{
		DBG_LogFile( _T("ProcessNoticeFrameBuyItem fail"));
		//return;
	}

	switch( controlID)
	{
	case message_window_2_button_01:
		{
			SendCS_EC_BUYITEM_REQ(	g_MainCharInfo.m_dwPickedObject, 
									pHoldItem->m_wRefID,		// CH_02_18 : Item spec change
									1,
									0,
									pHoldItem->m_bSackPos, 
									g_MainCharInfo.m_byMySackCurrIdx+1,
									255);
		}
		break;
	case message_window_2_button_02:	
		break;
	}
}

void ProcessNoticeDeleteCharacter( int controlID)
{
	switch( controlID)
	{
	case message_window_2_button_01:
		SendCS_IT_DELCHARACTER_REQ( g_pIntro->GetCurrentChar()->m_dwObjectID);
		g_pIntro->m_bCanSelectCharacter = true;
		break;
	case message_window_2_button_02:	
		g_pIntro->m_bCanSelectCharacter = true;
		break;
	}
}

void ProcessNoticeFramePamun(int controlID)
{
	switch( controlID)
	{
	case message_window_2_button_01:
		{
			DWORD dwCharID = g_MainCharInfo.m_pRelation->GetCurrRelation();
			sClanWonInfo* pClan = g_MainCharInfo.m_pRelation->FindClanInfoByID( dwCharID);
			if( pClan)
			{
				DWORD dwOrderID = pClan->m_dwOrderID;
				SendCS_RL_DELMUNWON_REQ( dwCharID, dwOrderID);
			} // if( pClan)
		}
		break;
	case message_window_2_button_02:
		break;
	}
}

void ProcessNoticeFrameJemyung(int controlID)
{
	switch( controlID)
	{
	case message_window_2_button_01:
		{
			SendCS_IF_BANISHPARTY_REQ( g_MainCharInfo.m_pRelation->GetDanID(), g_MainCharInfo.m_pRelation->GetCurrRelation());
		}
		break;
	case message_window_2_button_02:
		break;
	}
}


void ProcessNoticeFrameCloseClan(int controlID)
{
	switch( controlID)
	{
	case message_window_2_button_01:
		{
			SendCS_RL_DELETEMUNPA_REQ();
		}
		break;
	case message_window_2_button_02:
		break;
	}
}

void ProcessNoticeFrameFound(int controlID)
{
	switch( controlID)
	{
	case message_window_2_button_01:
		{
			if(g_pUIManager->IsShow(WINDOW_MUNPA_FOUND))
			{
				TCHAR strName[128] = {0,};
				memset(strName, 0, sizeof(strName));
				g_pUIManager->GetString(WINDOW_MUNPA_FOUND, found_window_name_edit, strName);

				SendCS_RL_CREATEMUNPA_REQ( strName);
			}			
		}
		break;
	case message_window_2_button_02:
		break;
	}
}

void ProcessNoticeFrameQuestDel(const int controlID)  // 퀘스트 삭제시
{
	switch( controlID)
	{
	case message_window_2_button_01:
		{
			if(g_MainCharInfo.m_pQuest->GetCurrQuestID())
				SendCS_QS_DELETE_REQ( g_MainCharInfo.m_pQuest->GetCurrQuestID());	
		}
		break;
	case message_window_2_button_02:
		break;
	}
}


void ProcessNoticeDanWarAsk(const int controlID)		// 단 전투 (단비무) 여부
{
	CXiahCharObject* pMainChar = reinterpret_cast<CXiahCharObject*>( g_pMainChar->m_pObject);	

	switch(controlID)
	{
	case message_window_2_button_01:
		SendCS_BT_ASKPARTYBATTLE_REQ(1, g_MainCharInfo.m_dwAskPartyID, g_MainCharInfo.m_dwAskID, pMainChar->m_dwPartyID, pMainChar->m_dwPartyLeaderID, g_MainCharInfo.m_dwBetMoney);
		break;
	case message_window_2_button_02:
		SendCS_BT_ASKPARTYBATTLE_REQ(9, g_MainCharInfo.m_dwAskPartyID, g_MainCharInfo.m_dwAskID, pMainChar->m_dwPartyID, pMainChar->m_dwPartyLeaderID, g_MainCharInfo.m_dwBetMoney);
		break;
	}
}

void ProcessNoticeStoneOwn(const int nControlID)		// 문파 비석 소유
{
	switch(nControlID)
	{
	case message_window_2_button_01:
		{
			CXiahCharObject* pMainChar = reinterpret_cast<CXiahCharObject*>(g_pMainChar->m_pObject);

			SendCS_RL_GAINSTONE_REQ(pMainChar->m_dwMunpaID, g_MainCharInfo.m_dwPickedObject);
		}
		break;
	case message_window_2_button_02:
		break;
	default:
		break;
	}
}

void ProcessNoticeStoneResign(const int nControlID)		// 문파 비석 포기
{
	switch(nControlID)
	{
	case message_window_2_button_01:
		{
			//CXiahCharObject* pMainChar = reinterpret_cast<CXiahCharObject*>(g_pMainChar->m_pObject);

			XiahObject::CXiahObject* pObject = XiahObject::g_XiahObjectManager.FindXiahObject(MAKEOBJECTID( 0, g_MainCharInfo.m_dwPickedObject, OBJTYPE_FUNCTIONALNPC));
			if(pObject == NULL) return;
			if(pObject->m_pObject == NULL) return;
			CXiahCharObject* pCharObject = reinterpret_cast<CXiahCharObject*>(pObject->m_pObject);
			if(pCharObject == NULL) return;
			sFunctionalNpcInfo* pInfo = (sFunctionalNpcInfo*)pCharObject->m_pPrivateData;
			if(pInfo == NULL) return;

			SendCS_WR_STONEDELETE_REQ(pInfo->m_dwOwnID);
		}
		break;
	case message_window_2_button_02:
		break;
	default:
		break;
	}
}


/**
 * 관계
 * \param nControlID 
 */
void ProcessNoticeAskRelation(const int nControlID)
{
	switch(nControlID)
	{
	case message_window_2_button_01:
		{
			switch( g_MainCharInfo.m_byRelationStep )
			{
			case 1:	// 인연 만들때
				SendCS_RL_ASKRELATION_REQ(g_MainCharInfo.m_byRelationType, RELATION_STEP_ACCEPT, g_MainCharInfo.m_dwAskID, g_MainCharInfo.m_dwObjectID);
				break;
			case 2:	// 인연 없앨떄.
				SendCS_RL_BREAKRELATION_REQ(g_MainCharInfo.m_byRelationType, RELATION_STEP_ACCEPT, g_MainCharInfo.m_dwAskID, g_MainCharInfo.m_dwObjectID);
				break;
			case 3:	// 인연 없앨때 강행. 
				SendCS_RL_BREAKRELATION_REQ(g_MainCharInfo.m_byRelationType, RELATION_STEP_CONFIRM, g_MainCharInfo.m_dwObjectID, g_MainCharInfo.m_dwAskID );
				break;
			};
		}
		break;
	case message_window_2_button_02:	// 거절이군.
		switch( g_MainCharInfo.m_byRelationStep )
		{
		case 1:
			SendCS_RL_ASKRELATION_REQ(g_MainCharInfo.m_byRelationType, RELATION_STEP_DECLINE, g_MainCharInfo.m_dwAskID, g_MainCharInfo.m_dwObjectID);
			break;
		case 2:
			SendCS_RL_BREAKRELATION_REQ(g_MainCharInfo.m_byRelationType, RELATION_STEP_DECLINE, g_MainCharInfo.m_dwAskID, g_MainCharInfo.m_dwObjectID);
			break;
		};
		break;
	} // switch(nControlID)
}

void ProcessNoticeAffinity(const int nControlID)
{
	switch(nControlID)
	{
	case message_window_2_button_01:
		{
		}
		break;
	case message_window_2_button_02:
		break;
	default:
		break;
	}
}

/**
 * 개인 노점 횅땍
 * \param nControlID 
 */
void ProcessNoticePtBuy(const int nControlID)
{
	switch(nControlID)
	{
	case message_window_2_button_01:
		{
			if(g_MainCharInfo.m_pPersonalTradeSell)
			{
				XiahItem::sItemInfo* pHoldItem = g_MainCharInfo.m_pHoldItem->GetHoldItemItem();

				// [1/12/2005] 거래 횅땍
				if(pHoldItem)
				{
					if(pHoldItem->m_dwPrice >= g_MainCharInfo.m_dwBuyLimit && g_MainCharInfo.m_dwBuyLimit)	// 초과
					{
						g_pUIManager->ShowNotice(IDS_LIMIT_PTBUY, NOTICE_FRAME_OKCANCEL, NOTICE_FRAME_LIMIT_PT_BUY, XiahInput::g_ptMouse.x - 270, XiahInput::g_ptMouse.y-90);

						return;
					}
					else	// 구입가능
					{
						SendCS_SH_BUYPCSHOP_REQ(g_MainCharInfo.m_dwPickedObject,
												pHoldItem->m_bSackPos,
												pHoldItem->m_dwItemID,
												g_MainCharInfo.m_byMySackCurrIdx+1,
												g_MainCharInfo.m_pHoldItem->m_bBackPosition,
												pHoldItem->m_dwPrice);
					}					
				}				
			} // if(g_MainCharInfo.m_pPersonalTradeSell)
			else
			{
				g_MainCharInfo.ShowHelpMessage(IDS_PT_CLOSE, TEXTEFFECT_COLOR_WARNING);

				if(g_MainCharInfo.m_pHoldItem)
					g_MainCharInfo.m_pHoldItem->SetItemBackToSack();
			}
		}
		break;
	case message_window_2_button_02:
		{
			if(g_MainCharInfo.m_pHoldItem)
				g_MainCharInfo.m_pHoldItem->SetItemBackToSack();
		}
		break;
	default:
		break;
	} // switch(nControlID)

	g_MainCharInfo.m_pHoldItem->SetDrawFlag(true);
}

/**
 * 전서구 삭제 여부
 * \param nControlID 
 */
void ProcessNoticeMailDelete(const int nControlID)
{
	switch(nControlID)
	{
	case message_window_2_button_01:	// 횅땍
		{
			g_MainCharInfo.OpenFrame(WINDOW_MAIL_SELECT);
			g_Mail.Delete_RecvMail();
		}
		break;
	case message_window_2_button_02:	// NO
		{
			CloseAllWindow();
			g_MainCharInfo.OpenFrame(WINDOW_MAIL);
			g_MainCharInfo.OpenFrame(WINDOW_MAIL_SELECT);
		}
		break;
	default:
		break;
	} // switch(nControlID)
}


/**
* 전서구 전송 여부
* \param nControlID 
*/
void ProcessNoticeMailSend(const int nControlID)
{
	switch(nControlID)
	{
	case message_window_2_button_01:	// 횅땍
		{
			g_MainCharInfo.OpenFrame(WINDOW_MAIL_SELECT);

			// 전송일때
			TCHAR strTo[64] = {0,};
			TCHAR strTitle[64] = {0,};
			g_pUIManager->GetString(WINDOW_MAIL, window_mail_top_edit_01, strTo, GET_STRING);
			g_pUIManager->GetString(WINDOW_MAIL, window_mail_top_edit_02, strTitle, GET_STRING);

			if(_tcslen(strTitle))
			{
				TCHAR strMix[320] = {0,};

				g_pUIManager->GetString(WINDOW_MAIL, window_mail_edit_01, strMix, GET_STRING);

				for(int i=0; i < 7; ++i)
				{
					TCHAR strTemp[128] = {0,};
					g_pUIManager->GetString(WINDOW_MAIL, window_mail_edit_02 - i, strTemp, GET_STRING);

					_ftcscat(strMix, _T("|"));
					_ftcscat(strMix, strTemp);
				} // for(int i=0; i < 7; ++i)

				// Check된 사람들 한테 전서를 날린다.
				g_Mail.Assign_Content(strTitle,strMix);
				g_Mail.SendMail();

				// 수동으로 입력한 사람에게 날린다
				g_Mail.SendMailToOne(strTo);

				// 윈도를 지우고 닫는다.
				g_pUIManager->SetString(WINDOW_MAIL, window_mail_top_edit_01, _T(""));
				g_pUIManager->SetString(WINDOW_MAIL, window_mail_top_edit_02, _T(""));

				for(int j=0; j < 8; ++j)
					g_pUIManager->SetString(WINDOW_MAIL, window_mail_edit_01 - j, _T(""));

				//g_MainCharInfo.CloseFrame(WINDOW_MAIL);
				g_MainCharInfo.CloseFrame(WINDOW_MAIL_SELECT);
			}
			else
				g_MainCharInfo.ShowHelpMessage(IDS_M_TITLE_NOT, TEXTEFFECT_COLOR_WARNING);

		}
		break;
	case message_window_2_button_02:	// NO
		{
			CloseAllWindow();
			g_MainCharInfo.OpenFrame(WINDOW_MAIL);
			g_MainCharInfo.OpenFrame(WINDOW_MAIL_SELECT);
		}
		break;
	default:
		break;
	} // switch(nControlID)
}

/**
* 펫 부활 여부
* \param nControlID 
*/
void ProcessNoticePetRevival(int nControlID)
{
	switch(nControlID)
	{
	case message_window_2_button_01:	// 횅땍
		{
			map<BYTE, sPetRevival*>::iterator iter = g_PetList.m_mPetRevivalList.find(g_MainCharInfo.m_nTempValue);

			if(iter != g_PetList.m_mPetRevivalList.end())
			{
				sPetRevival *pPetInfo = iter->second;

				if(pPetInfo != NULL)
                    SendCS_NC_PETRESTORE_REQ(pPetInfo->dwID, g_MainCharInfo.m_ReairSackID, g_MainCharInfo.m_RpairItemPos);
			}	
		}
		break;
	case message_window_2_button_02:	// NO
		break;
	default:
		break;
	} // switch(nControlID)
}

/**
* 빙정에 펫 봉인 여부
* \param nControlID 
*/
void ProcessNoticePetBongin(int nControlID)
{
	switch(nControlID)
	{
	case message_window_2_button_01:	// 횅땍
		{
			XiahItem::sItemInfo* pInfo = g_MainCharInfo.m_pMySack[ g_MainCharInfo.m_byMySackCurrIdx]->FindSackItemByID( g_MainCharInfo.m_dwCurrentSelectedBongInItem);

			if(!pInfo)
			{
				DBG_LogFile( _T("ProcessPopMenuBongInItem fail"));
				return;
			}

			if(g_PetList.size() > 0)
			{
				SendCS_NC_PETBONGIN_REQ(g_PetList.GetPetInfoByIndex(0)->dwID, pInfo->m_bSackCount+1, pInfo->m_bSackPos);
			}
		}
		break;
	case message_window_2_button_02:	// 취소
		{
		}
		break;
	default:
		break;
	} // switch(nControlID)
}

/**
 * 관계 단
 * \param nControlID 
 */
void ProcessNoticeRelAskParty(int nControlID)
{
	switch(nControlID)
	{
	case message_window_2_button_01:	// 승인
		{
			SendCS_IF_ASKPARTY_REQ(g_MainCharInfo.m_dwAskID, g_MainCharInfo.m_dwObjectID, 1, 1);
		}
		break;
	case message_window_2_button_02:	// 거절
		{
			SendCS_IF_ASKPARTY_REQ(g_MainCharInfo.m_dwAskID, g_MainCharInfo.m_dwObjectID, 9, 1);
		}
		break;

	default:
		break;
	} // switch(nControlID)
}

/**
 * 문주이양
 * \param nControlID 
 */
void ProcessNoticeRelinquish(int nControlID)
{
	switch(nControlID)
	{
	case message_window_2_button_01:	// 승인
		{
			DWORD dwMunwonID = g_MainCharInfo.m_pRelation->GetCurrRelation();
			sClanWonInfo* pInfo = g_MainCharInfo.m_pRelation->FindClanInfoByID(dwMunwonID);

			if(pInfo)
			{
				SendCS_RL_CHANGEMUNWONORDER_REQ(dwMunwonID, pInfo->m_dwOrderID, 1);
			}
		}
		break;
	case message_window_2_button_02:
		break;

	default:
		break;
	} // switch(nControlID)
}

/**
 * 문파탈퇴
 * \param nControlID 
 */
void ProcessNoticeLeave(int nControlID)
{
	switch(nControlID)
	{
	case message_window_2_button_01:	// 탈퇴
		{
			sClanWonInfo* pClan = g_MainCharInfo.m_pRelation->FindClanInfoByID(g_MainCharInfo.m_dwObjectID);

			if(pClan)
			{
				SendCS_RL_DELMUNWON_REQ(g_MainCharInfo.m_dwObjectID, pClan->m_dwOrderID);
			}
		}
		break;
	case message_window_2_button_02:
		break;

	default:
		break;
	} // switch(nControlID)
}

/**
 * 문파문장 삭제여부
 * \param nControlID 
 */
void ProcessNoticeMarkDel(int nControlID)
{
	switch(nControlID)
	{
	case message_window_2_button_01:	// 삭제
		{
			CXiahCharObject* pMainChar = reinterpret_cast<CXiahCharObject*>(g_pMainChar->m_pObject);

			sString strTemp = _T("NOT");
			if(pMainChar)
				SendCS_RL_MUNPAMARKREG_REQ(1, pMainChar->m_dwMunpaID, (LPCTSTR)strTemp, g_MainCharInfo.m_dwPickedObject);
		}
		break;
	case message_window_2_button_02:
		break;

	default:
		break;
	} // switch(nControlID)
}

/**
 * 일반 구매 제한시
 * \param nControlID 
 */
void ProcessNoticeLimitBuy(int nControlID)
{
	switch(nControlID)
	{
	case message_window_2_button_01:
		{
			XiahItem::sItemInfo* pHoldItem = g_MainCharInfo.m_pHoldItem->GetHoldItemItem();

			if(pHoldItem)
			{
				if( pHoldItem->m_bItemType == ITEMTYPE_POTION || pHoldItem->m_bItemType == ITEMTYPE_PORTAL || pHoldItem->m_bItemType == ITEMTYPE_GOLDKEY) //HO_0828_07 황금열쇠 추가
				{
					SendCS_EC_BUYITEM_REQ(	g_MainCharInfo.m_dwPickedObject, 
											pHoldItem->m_wRefID,
											pHoldItem->m_dwBuyCount,
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
		}
		break;
	case message_window_2_button_02:
		{
			if(g_MainCharInfo.m_pHoldItem)
				g_MainCharInfo.m_pHoldItem->SetItemBackToSack();
		}
		break;
	default:
		break;
	} // switch(nControlID)

	g_MainCharInfo.m_pHoldItem->SetDrawFlag(true);	
}

/**
 * 개인 노점 구매 제한시
 * \param nControlID 
 */
void ProcessNoticeLimitPtBuy(int nControlID)
{
	switch(nControlID)
	{
	case message_window_2_button_01:
		{
			XiahItem::sItemInfo* pHoldItem = g_MainCharInfo.m_pHoldItem->GetHoldItemItem();

			if(pHoldItem)
			{
				SendCS_SH_BUYPCSHOP_REQ(g_MainCharInfo.m_dwPickedObject,
										pHoldItem->m_bSackPos,
										pHoldItem->m_dwItemID,
										g_MainCharInfo.m_byMySackCurrIdx+1,
										g_MainCharInfo.m_pHoldItem->m_bBackPosition,
										pHoldItem->m_dwPrice);
			}

		}
		break;
	case message_window_2_button_02:
		{
			if(g_MainCharInfo.m_pHoldItem)
				g_MainCharInfo.m_pHoldItem->SetItemBackToSack();
		}
		break;
	default:
		break;
	} // switch(nControlID)

	g_MainCharInfo.m_pHoldItem->SetDrawFlag(true);	
}

/**
 * 판매 제한시
 * \param nControlID 
 */
void ProcessNoticeLimitSell(int nControlID)
{
	switch(nControlID)
	{
	case message_window_2_button_01:
		{
			XiahItem::sItemInfo* pHoldItem = g_MainCharInfo.m_pHoldItem->GetHoldItemItem();

			if(pHoldItem)
			{
				SendCS_EC_SELLITEM_REQ( g_MainCharInfo.m_dwPickedObject, 
										pHoldItem->m_dwItemID,
										pHoldItem->m_bSackCount+1,
										pHoldItem->m_bSackPos);
			}
		}
		break;
	case message_window_2_button_02:
		{
			if(g_MainCharInfo.m_pHoldItem)
				g_MainCharInfo.m_pHoldItem->SetItemBackToSack();
		}
		break;
	default:
		break;
	} // switch(nControlID)

	g_MainCharInfo.m_pHoldItem->SetDrawFlag(true);
}

/**
 * 아이템 복구
 * \param nControlID 
 */
void ProcessNoticeRecovery1(int nControlID)
{
	switch(nControlID)
	{
	case message_window_2_button_01:
		{
			SendCS_IM_REWARDGUARANTEE_REQ(0, g_MainCharInfo.m_dwResItemID);
		}
		break;
	case message_window_2_button_02:
		break;
	default:
		break;
	}
}

/**
 * 아이템 복구 - 소멸
 * \param nControlID 
 */
void ProcessNoticeRecovery2(int nControlID)
{
	switch(nControlID)
	{
	case message_window_2_button_01:
		{
			SendCS_IM_REWARDGUARANTEE_REQ(1, g_MainCharInfo.m_dwResItemID);
		}
		break;
	case message_window_2_button_02:
		break;
	default:
		break;
	}
}

/**
 * 절연부 선택 (사부사제, 연인)
 * \param nControlID 
 */
void ProcessNoticeServerRelation1(int nControlID)
{
	switch(nControlID)
	{
	case message_window_2_button_01:
		{
			g_MainCharInfo.m_byRelationType = 100;
			g_pUIManager->ShowNotice(IDS_SERVER_REL2, 61, NOTICE_FRAME_SEVER_RELATION2, XiahInput::g_ptMouse.x - 270, XiahInput::g_ptMouse.y-90);
		}
		break;
	case message_window_2_button_02:
		{
			g_MainCharInfo.m_byRelationType = 10;
			g_pUIManager->ShowNotice(IDS_SERVER_REL2, 61, NOTICE_FRAME_SEVER_RELATION2, XiahInput::g_ptMouse.x - 270, XiahInput::g_ptMouse.y-90);
		}
		break;
	default:
		break;
	}
}

/**
 * 절연부 횅땍
 * \param nControlID 
 */
void ProcessNoticeServerRelation2(int nControlID)
{
	switch(nControlID)
	{
	case message_window_2_button_01:
		{			
			DWORD dwCharID = 0;
			
			if(g_MainCharInfo.m_byRelationType == 100)
			{
				if(dwCharID = g_MainCharInfo.m_pRelation->FindSabuID())
				{
					g_MainCharInfo.m_byRelationType = 20;
				}
				else if(dwCharID = g_MainCharInfo.m_pRelation->FindJejaID())
				{
					g_MainCharInfo.m_byRelationType = 30;
				}
			}
			else if(g_MainCharInfo.m_byRelationType == 10)
			{
				dwCharID = g_MainCharInfo.m_pRelation->FindSweetheart();
			}

			
			SendCS_RL_BREAKRELATIONITEM_REQ(g_MainCharInfo.m_ReairSackID,
											g_MainCharInfo.m_RpairItemPos,
											g_MainCharInfo.m_dwResItemID,
											g_MainCharInfo.m_byRelationType,
											dwCharID);
		}
		break;
	case message_window_2_button_02:
		break;
	default:
		break;
	}
}

/**
 * 문파대전 참여 신청
 * \param nControlID 
 */
void ProcessNoticeClanWarApply(int nControlID)
{
	switch(nControlID)
	{
	case message_window_2_button_01:
		{
			SendCS_WR_APPLYWAR_REQ(g_MainCharInfo.m_dwPickedObject);
		}
		break;
	case message_window_2_button_02:
		break;
	default:
		break;
	}
}


void ProcessNoticeClanWarReward(int nControlID)
{
    switch(nControlID)
    {
    case message_window_2_button_01:
        {
            SendCS_WR_REWARD_REQ(1);
        }
        break;
    case message_window_2_button_02:
        break;
    default:
        break;
    }
}

void ProcessNoticeMuGongApply(int nControlID)	//HO_0329_07 무공 습득 여부 추가
{
	switch(nControlID)
	{
	case message_window_2_button_01:
		{			
			SendCS_IM_USEITEM_REQ(g_MainCharInfo.m_ReairSackID,g_MainCharInfo.m_RpairItemPos, g_MainCharInfo.m_dwResItemID);			
		}
		break;
	case message_window_2_button_02:
		break;
	default:
		break;
	}
}
/*
void ProcessNoticeReBirthApply(int nControlID)
{
	switch(nControlID)
	{
	case message_window_2_button_01:
		{
			SendCS_IM_REBIRTH_REQ(g_MainCharInfo.m_ReairSackID,
								  g_MainCharInfo.m_RpairItemPos,
                                  g_MainCharInfo.m_dwResItemID);
		}
		break;
	case message_window_2_button_02:
		{
			g_MainCharInfo.m_bRebirthItem_Use = false;
		}
		break;

	
	default:
		break;
	}
}
*/

void ProcessNoticeDanCommit(int nControlID)	//HT_0423 : 단주 위임
{
	switch(nControlID)
	{
	case message_window_2_button_01:
		{			
			SendCS_IF_CHANGEPARTYLEADER_REQ(g_MainCharInfo.m_dwObjectID ,g_MainCharInfo.m_pRelation->GetCurrRelation());
			CloseAllWindow();
		}
		break;
	case message_window_2_button_02:
		{
			g_MainCharInfo.ShowHelpMessage(IDS_DANCOMMIT_CANCEL, TEXTEFFECT_COLOR_WARNING);
		}
		break;
	default:
		break;
	}
}

/**
 * //HO_0816_07 종료 기능 추가
 * \param nControlID 
 */
void ProcessNoticeEndGame(int nControlID)
{
	switch(nControlID)
	{
	case message_window_2_button_01:
		PostMessage( g_AppData.m_hWnd, WM_CLOSE, 0, 0);
		break;
	case message_window_2_button_02:
		break;
	}
}

/**
 * //HO_0816_07 아이템 드랍시 횅땍
 * \param nControlID 
 */
void ProcessNoticeItemDrop(int nControlID)
{
	switch(nControlID)
	{
	case message_window_2_button_01:
		g_MainCharInfo.m_pHoldItem->ThrowItem( 0);
		break;
	case message_window_2_button_02:		
		g_MainCharInfo.ShowHelpMessage(IDS_ITEM_CANCLEDROP, TEXTEFFECT_COLOR_WARNING);
		g_MainCharInfo.m_pHoldItem->SetItemBackToSack();
		break;
	}
}

void ProcessNoticeWindow2( LPARAM lParam)
{
	const int controlID = LOWORD(lParam);
	const int eventType = HIWORD(lParam);

	switch( eventType)
	{
	case NOTICE_FRAME_PT_BUY:					// 개인 노점 구입 여부
		{
			g_pUIManager->HideNotice(MESSAGE_WINDOW_2BUTTON);
			g_pUIManager->DeletePopMenu();
			g_pUIManager->DeletePopSubMenu();

			ProcessNoticePtBuy(controlID);

			return;
		}		
		break;
	case NOTICE_FRAME_LIMIT_BUY:				// 일반 구매 제한시
		ProcessNoticeLimitBuy(controlID);
		break;
	case NOTICE_FRAME_LIMIT_PT_BUY:				// 개인 노점 구매 제한시
		ProcessNoticeLimitPtBuy(controlID);
		break;
	case NOTICE_FRAME_LIMIT_SELL:				// 판매 제한시
		ProcessNoticeLimitSell(controlID);
		break;

	case NOTICE_FRAME_TRADE:					// 거래
		ProcessNoticeFrameTrade( controlID);		
		break;
	case NOTICE_FRAME_ASKPARTY:					// 일반단
		ProcessNoticeFrameAskParty( controlID);
		break;
	case NOTICE_FRAME_INVITEPARTY:
		ProcessNoticeFrameInviteParty( controlID);
		break;
	case NOTICE_FRAME_ASKBUDDY:
		ProcessNoticeFrameAskBuddy( controlID);
		break;
	case NOTICE_FRAME_CLAN:
		ProcessNoticeFrameClan( controlID);
		break;
	case NOTICE_FRAME_BUYITEM:
		ProcessNoticeFrameBuyItem( controlID);
		break;
	case NOTICE_FRAME_DELETE_CHARACTER:
		ProcessNoticeDeleteCharacter( controlID);
		break;
	case NOTICE_FRAME_PAMUN:
		ProcessNoticeFramePamun(controlID);
		break;
	case NOTICE_FRAME_JEMYUNG:
		ProcessNoticeFrameJemyung(controlID);
		break;
	case NOTICE_FRAME_CLOSE_CLAN:
		ProcessNoticeFrameCloseClan(controlID);
		break;
	case NOTICE_FRAME_FOUNT:
		ProcessNoticeFrameFound(controlID);
		break;
	case NOTICE_FRAME_QUEST_DEL:				// 퀘스트 삭제
		ProcessNoticeFrameQuestDel(controlID);
		break;
	case NOTICE_FRAME_DAN_WAR_ASK:				// 단 전투 여부
		ProcessNoticeDanWarAsk(controlID);
		break;
	case NOTICE_FRAME_STONE_OWN:				// 문파 비석 소유
		ProcessNoticeStoneOwn(controlID);
		break;
	case NOTICE_FRAME_STONE_RESIGN:				// 문파 비석 포기
		ProcessNoticeStoneResign(controlID);
		break;
	case NOTICE_FRAME_ASKRELATION:
		ProcessNoticeAskRelation(controlID);
		break;
	case NOTICE_FRAME_AFFINITY:
		ProcessNoticeAffinity(controlID);
		break;
	case NOTICE_FRAME_MAIL_DELETE:				// 전서구 삭제 여부
		ProcessNoticeMailDelete(controlID);
		break;
	case NOTICE_FRAME_MAIL_SEND:
		ProcessNoticeMailSend(controlID);
		break;
	case NOTICE_FRAME_PET_REVIVAL:				// 펫 부활 여부
		ProcessNoticePetRevival(controlID);
		break;
	case NOTICE_FRAME_PET_BONGIN:				// 빙정에 펫 봉인 여부
		ProcessNoticePetBongin(controlID);
		break;
	case NOTICE_FRAME_RELATION_ASKPARTY:		// 관계단
		ProcessNoticeRelAskParty(controlID);
		break;
	case NOTICE_FRAME_RELINQUISH:				// 문주이양
		ProcessNoticeRelinquish(controlID);
		break;
	case NOTICE_FRAME_LEAVE:					// 문파탈퇴
		ProcessNoticeLeave(controlID);
		break;
	case NOTICE_FRAME_MARK_DEL:					// 문파문장 삭제여부
		ProcessNoticeMarkDel(controlID);
		break;
	case NOTICE_FRAME_RECOVERY1:				// 아이템 복구
		ProcessNoticeRecovery1(controlID);
		break;
	case NOTICE_FRAME_RECOVERY2:				// 아이템 복구 - 소멸
		ProcessNoticeRecovery2(controlID);
		break;
	case NOTICE_FRAME_SEVER_RELATION1:			// 절연부 선택
		{
			g_pUIManager->HideNotice(MESSAGE_WINDOW_2BUTTON);
			ProcessNoticeServerRelation1(controlID);

			return;
		}		
		break;
	case NOTICE_FRAME_SEVER_RELATION2:			// 절연부 - 횅땍
		{
			g_pUIManager->HideNotice(MESSAGE_WINDOW_2BUTTON);
			ProcessNoticeServerRelation2(controlID);

			return;
		}		
		break;
	case NOTICE_FRAME_CLAN_WAR_APPLY:			// 문파대전 참여 신청
		{
			ProcessNoticeClanWarApply(controlID);
		}
		break;
	case NOTICE_FRAME_CLAN_WAR_REWARD:			// 문파대전 우승 상금
		{
			ProcessNoticeClanWarReward(controlID);
		}
		break;
	case NOTICE_FRAME_MUGONG:	//HO_0329_07 무공 습득 여부 추가
		{
			ProcessNoticeMuGongApply(controlID);
		}
		break;
/*	case NOTICE_FRAME_REBIRTH:
		{
			ProcessNoticeReBirthApply(controlID);
		}
		break;*/
	case NOTiCE_FRAME_DANCOMMIT:
		{
			ProcessNoticeDanCommit(controlID);
		}
		break;
	case NOTICE_FRAME_ENDGAME:					//HO_0816_07 종료 기능 추가
		ProcessNoticeEndGame(controlID);
		break;
	case NOTICE_FRAME_ITEMDROP:					//HO_0816_07 아이템 드랍시 횅땍
		ProcessNoticeItemDrop(controlID);
		break;
	//default:
	//	return;									//HO_0816_07 제목을 누르면 switch문을 거친후 밑의 명령이 수행된다(팝메뉴가 지워진다 그러면 안될텐데...그래서 default를 추가했다..)
	//	break;									//HO_0827_07 디폴트로 막기에는 다른 에러 사항이 몇가지 있다... 주석처리 ..;
	}	

	g_pUIManager->HideNotice(MESSAGE_WINDOW_2BUTTON);
	g_pUIManager->DeletePopMenu();
	g_pUIManager->DeletePopSubMenu();
}



