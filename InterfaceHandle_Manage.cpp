#include "Helper.h"  //HO_0410_07 상서령 가이드 업데이트 g_Helper를 사용하기 위하여 삽입

void ProcessEnterForChat()
// 프레임창을 다음단계로 펼침
{
	if( !g_pUIManager)
		return;

	if( !g_MainCharInfo.m_pChat)
		return;

	switch(g_MainCharInfo.m_pChat->GetChatType())
	{
		case NOCHAT:
			g_MainCharInfo.m_pChat->SetChatType( SMALLCHAT);
			break;

		case SMALLCHAT:
			{
				g_MainCharInfo.m_pChat->SetChatType( LARGECHAT);

				CloseAllWindow();

				g_MainCharInfo.HideSack( SACKTYPE__PC_TRADE_MINE);
				g_MainCharInfo.HideSack( SACKTYPE__PC_TRADE_OTHER);
				g_MainCharInfo.HideSack( SACKTYPE__NPC_TRADE);
			}		
			break;

		case MEDIUMCHAT:
			break;

		case LARGECHAT:
			g_MainCharInfo.m_pChat->SetChatType( NOCHAT);
			break;
	}
}

void ProcessFocusOnChat()
// 프레임창에 포커스만 둠
{	
	if( !g_pUIManager)
		return;

	if( !g_MainCharInfo.m_pChat)
		return;

	if(!g_pUIManager->IsOnEditing())
	{
		g_MainCharInfo.m_bFirstChat = true;

		g_pUIManager->SetFocus(MAIN_CHAT);
		g_pUIManager->SetFocus(MAIN_CHAT, main_chat_edit);

		g_MainCharInfo.m_pChat->SetChatFlag( TRUE); //채팅창이 다음단계로 펼쳐지지 않게

		if( g_MainCharInfo.m_pChat->GetChatType() == NOCHAT)
			g_MainCharInfo.m_pChat->SetChatFlag( FALSE); //채팅창이 다음단계로 펼쳐지지 않게
	}
}


void CloseAllWindow()
{
	if( !g_pUIManager)
		return;

	if( g_pUIManager->IsNotice()) //HO_0424_07 단주변경 : 단주 댄轎 수정중 Notice창이 문제가 되어 이창이 있을경우 클로즈 윈도우를 안먹도록 설정함
		return;

	if(!g_pUIManager->IsShow(DATA_WINDOW))
	{
		g_pUIManager->Show(DATA_WINDOW);
	}

	//Main_MainChar.cpp에서 마우스 move때 해주는 일들..
	//이것 관련해서 하나로 묶는 함수 만들어 주는게 좋겠다( 물론 나중에 -0-;;)
	// navigation하려 할때, pop menu가 떠 있으면 없애주자
	g_pUIManager->DeletePopSubMenu();
	g_pUIManager->DeletePopMenu();

	// navigation하려 할때, 캐릭터가 pick한 object 는 0
	//g_MainCharInfo.m_dwPickedObject = 0;

	if( g_MainCharInfo.m_pHoldItem)
		g_MainCharInfo.m_pHoldItem->SetItemBackToSack();
	//////////////////////////////////////////////////////////////////////여기까지

	g_MainCharInfo.HideSack( SACKTYPE__DEFAULT);
	g_MainCharInfo.HideSack( SACKTYPE__EQUIPMENT);
	g_MainCharInfo.HideSack( SACKTYPE__NPC_TRADE);
	g_MainCharInfo.HideSack( SACKTYPE__PERSONAL_TRADE_SET);
	g_MainCharInfo.HideSack( SACKTYPE__PERSONAL_TRADE_SELL);
	g_MainCharInfo.HideSack( SACKTYPE__ITEMMALL);
	g_MainCharInfo.HideSack( SACKTYPE__DEPOSIT);
	g_MainCharInfo.HideSack( SACKTYPE__MODIFY);	
	g_MainCharInfo.HideSack( SACKTYPE__SMELT);					// 조합
	g_MainCharInfo.HideSack( SACKTYPE__FIVEELEMENT_CONVERT);	// 오행 제련

	if( g_MainCharInfo.m_pPcSackMine)
	{
		SendCS_EC_TRADEITEM_REQ( 9, g_MainCharInfo.m_dwAskID);
	}

	g_MainCharInfo.HideSack( SACKTYPE__PET);
	g_MainCharInfo.HideSack( SACKTYPE__PET_EQUIP);

	// 매품패
	g_MainCharInfo.HideSack(SACKTYPE__QUICKMART);
	g_MainCharInfo.HideSack(SACKTYPE__COLLECTION);			// 아이템 수집

	g_MainCharInfo.CloseFrame( WINDOW_CHARACTER);
	g_MainCharInfo.CloseFrame( WINDOW_OUTSIDE);
	g_MainCharInfo.CloseFrame( WINDOW_INSIDE);
	g_MainCharInfo.CloseFrame( WINDOW_SKILL);
	g_MainCharInfo.CloseFrame( WINDOW_DAN);
	// 단 경험치 분배
	g_MainCharInfo.CloseFrame(WINDOW_DAN_NEW);

	g_MainCharInfo.CloseFrame( WINDOW_MUNPA_FOUND);
	g_MainCharInfo.CloseFrame( WINDOW_MUNPA);

	g_MainCharInfo.CloseFrame( WINDOW_VOLUME);
	g_MainCharInfo.CloseFrame( WINDOW_MONEY);
	g_MainCharInfo.CloseFrame( WINDOW_PURSE);	
	g_MainCharInfo.CloseFrame( NAME_CHANNEL);
	g_MainCharInfo.CloseFrame( WINDOW_QUEST_01);

	g_MainCharInfo.CloseFrame(WINDOW_OPTION_01);
	g_MainCharInfo.CloseFrame(WINDOW_OPTION_02);
	g_MainCharInfo.CloseFrame(WINDOW_OPTION_03);

	g_MainCharInfo.CloseFrame( WINDOW_CLOSE);
	g_MainCharInfo.CloseFrame( GAK_MESSAGE_WINDOW);
	g_MainCharInfo.CloseFrame( WINDOW_DONGSIN);

	g_MainCharInfo.CloseFrame( WINDOW_MUNPA_WAR_PETITION);
	g_MainCharInfo.CloseFrame( WINDOW_MUNPA_DONATE);
	g_MainCharInfo.CloseFrame( WINDOW_MUNPA_BBS_WRITE);
	g_MainCharInfo.CloseFrame( WINDOW_MUNPA_BBS_TOP);
	g_MainCharInfo.CloseFrame( WINDOW_MUNPA_BBS_READ);
	g_MainCharInfo.CloseFrame( WINDOW_MUNPA_BBS_LIST);

	g_MainCharInfo.CloseFrame( WINDOW_MAIL);
	g_MainCharInfo.CloseFrame( WINDOW_MAIL_SELECT);
	g_MainCharInfo.CloseFrame( WINDOW_MAIL_RESULT);

	g_MainCharInfo.CloseFrame(WINDOW_BOK_NUMBER);		// 복권선택
	g_MainCharInfo.CloseFrame(WINDOW_BOK_PRIZE);		// 복권당첨번호
	g_MainCharInfo.CloseFrame(MESSAGE_WINDOW_MARK);		// 문파마크
	g_MainCharInfo.CloseFrame(WINDOW_SMELT);			// 조합
	
	g_MainCharInfo.CloseFrame(WINDOW_FIVEELEMENTS);		// 오행
	g_MainCharInfo.CloseFrame(WINDOW_FIVEELEMENTS_CONVERT);

	//HO_0410_07 상서령 가이드 업데이트
	g_MainCharInfo.CloseFrame(WINDOW_HELPER_LIST2);		// 대화 리스트
	g_MainCharInfo.CloseFrame(WINDOW_HELPER_LIST1);		//HT_0216 : 상서령 대화창 수정
	g_MainCharInfo.CloseFrame(WINDOW_HELPER_LIST);		// 대화 리스트 상서령 가이드
	g_MainCharInfo.CloseFrame(WINDOW_HELPER_LIST0);		// 퀵 가이드 대화내용
	g_MainCharInfo.CloseFrame(WINDOW_HELPER_SCRIPT);	// 대화내용	
	g_MainCharInfo.CloseFrame(WINDOW_RECOVERY);			// 아이템 복구

	g_MainCharInfo.CloseFrame(WINDOW_PORTAL);			// NPC 포탈 이동
	
	g_MainCharInfo.CloseFrame(A_HELP);

	g_MainCharInfo.CloseFrame(WINDOW_SECRET_BASIS);
	g_MainCharInfo.CloseFrame(WINDOW_SECRET_CHECK);
	g_MainCharInfo.CloseFrame(WINDOW_SECRET_INFORMATION);
	
	ChangeXiahCursor(eCT_General);
}

void CancelInterface()
{
	if(g_pUIManager->IsShow(WINDOW_VOLUME))
	{
		g_MainCharInfo.m_byUsageVolumFrame = 0;
		g_MainCharInfo.m_dwVolumeSplitAmount = 0;
	} // if( pVolumeFrame->IsActive())

	CloseAllWindow();

	// 매품패
	if(g_MainCharInfo.m_pQuickMart)
	{
		g_MainCharInfo.HideSack(SACKTYPE__QUICKMART, FALSE);		
	}
}

void ProcessHideMainFrame()
{	
	if(g_pUIManager->IsShow(MAIN_FRAME))
		g_pUIManager->Hide(MAIN_FRAME);
	else
		g_pUIManager->Show(MAIN_FRAME);

	if(g_pUIManager->IsShow(MAIN_FRAME))
	{
		g_pUIManager->SetPosition(SMALL_MESSENGER, SMALL_MESSENGER_XPOS, SMALL_MESSENGER_YPOS);
		g_pUIManager->SetPosition(LARGE_MESSENGER, LARGE_MESSENGER_XPOS, LARGE_MESSENGER_YPOS);
		g_pUIManager->SetPosition(MAIN_CHAT, MAIN_CHAT_XPOS, MAIN_CHAT_YPOS);		

		g_pUIManager->SetPosition(WINDOW_BUTTON_GROUP_01, WINDOW_BUTTON_GROUP_XPOS, WINDOW_BUTTON_GROUP_YPOS);

		RECT rtTemp;
		// 수집 아이콘 때문에 수정
		//HO_0413_07 퀵 가이드 업데이트 : 버튼 추가로 인한 위치 수정
		g_pUIManager->GetRegionData(WINDOW_BUTTON_GROUP_01, new_window_button_02, rtTemp);//퀵 가이드 버튼 추가로 인한 new_window_button_06을 02로 재설정
		g_pUIManager->SetPosition(SYSTEM_BUTTON_GROUP_01, rtTemp.right-2, SYSTEM_BUTTON_GROUP_YPOS);//퀵 가이드 버튼 추가로 인한 rtTemp.right에 -2를 포함해줌

		//g_pUIManager->GetRegionData(WINDOW_BUTTON_GROUP_01, new_window_button_06, rtTemp);//퀵 가이드 버튼 추가전 위치
		//g_pUIManager->SetPosition(SYSTEM_BUTTON_GROUP_01, rtTemp.right, WINDOW_BUTTON_GROUP_YPOS);

		g_pUIManager->SetPosition(PET_BUTTON_GROUP, PET_BUTTON_GROUP_XPOS, WINDOW_BUTTON_GROUP_YPOS);

		g_MainCharInfo.m_pChat->UpdateTex();
	}
	else
	{
		g_pUIManager->SetPosition(SMALL_MESSENGER, SMALL_MESSENGER_XPOS, SMALL_MESSENGER_YPOS+48);
		g_pUIManager->SetPosition(LARGE_MESSENGER, LARGE_MESSENGER_XPOS, LARGE_MESSENGER_YPOS+48);
		g_pUIManager->SetPosition(MAIN_CHAT, MAIN_CHAT_XPOS, MAIN_CHAT_YPOS+48);

		g_pUIManager->SetPosition(WINDOW_BUTTON_GROUP_01, WINDOW_BUTTON_GROUP_XPOS, WINDOW_BUTTON_GROUP_YPOS+48);

		RECT rtTemp;
		// 수집 아이콘 때문에 수정
		//HO_0413_07 퀵 가이드 업데이트 : 버튼 추가로 인한 위치 수정
		g_pUIManager->GetRegionData(WINDOW_BUTTON_GROUP_01, new_window_button_02, rtTemp);//퀵 가이드 버튼 추가로 인한 new_window_button_06을 02로 재설정
		g_pUIManager->SetPosition(SYSTEM_BUTTON_GROUP_01, rtTemp.right-2, SYSTEM_BUTTON_GROUP_YPOS+48);//퀵 가이드 버튼 추가로 인한 rtTemp.right에 -2를 포함해줌
		
		//g_pUIManager->GetRegionData(WINDOW_BUTTON_GROUP_01, new_window_button_06, rtTemp);
		//g_pUIManager->SetPosition(SYSTEM_BUTTON_GROUP_01, rtTemp.right, WINDOW_BUTTON_GROUP_YPOS+48);

		g_pUIManager->SetPosition(PET_BUTTON_GROUP, PET_BUTTON_GROUP_XPOS, WINDOW_BUTTON_GROUP_YPOS+48);

		g_MainCharInfo.m_pChat->UpdateTex();
	}
}

/**
 *
 */
void ProcessClickSackButton()
{
	if( !g_pUIManager)
		return;

	if( !g_MainCharInfo.m_pMySack[g_MainCharInfo.m_byMySackCurrIdx])
		return;

	if(g_pUIManager->IsShow(DRG_ITEM_WINDOW))
	{
		g_MainCharInfo.HideSack( SACKTYPE__DEFAULT);
		g_MainCharInfo.HideSack( SACKTYPE__EQUIPMENT);
		g_MainCharInfo.HideSack( SACKTYPE__NPC_TRADE);
		g_MainCharInfo.HideSack( SACKTYPE__PERSONAL_TRADE_SET);
		g_MainCharInfo.HideSack( SACKTYPE__PERSONAL_TRADE_SELL);
		g_MainCharInfo.HideSack( SACKTYPE__DEPOSIT);
		g_MainCharInfo.HideSack( SACKTYPE__ITEMMALL);
		g_MainCharInfo.HideSack( SACKTYPE__MODIFY);
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
		else if(g_MainCharInfo.m_bPersonalTradeSell) // 개인상점 판매시
		{
			CloseAllWindow();
			g_pUIManager->SetPosition(WINDOW_PC_STORE, WINDOW_FIRST_XPOS, 0);

			if(g_MainCharInfo.m_pPersonalTradeSet)
				g_MainCharInfo.m_pPersonalTradeSet->ShowSack();
		}

		g_MainCharInfo.CloseFrame( GAK_MESSAGE_WINDOW);
		g_MainCharInfo.CloseFrame( WINDOW_DONGSIN);

		g_pUIManager->DeletePopSubMenu();
		g_pUIManager->DeletePopMenu();
	}
	else 
	{
		if(g_pUIManager->IsShow(WINDOW_CHARACTER))
		{
			CloseAllWindow();
			g_MainCharInfo.OpenFrame( WINDOW_CHARACTER);
			g_pUIManager->SetPosition(WINDOW_CHARACTER, WINDOW_SECOND_XPOS, 0);

			g_MainCharInfo.ShowSack( SACKTYPE__DEFAULT);
			g_MainCharInfo.ShowSack( SACKTYPE__EQUIPMENT);
		}
		else if(g_pUIManager->IsShow(WINDOW_NEW_TAMING))
		{
			CloseAllWindow();
			g_pUIManager->SetPosition(WINDOW_NEW_TAMING, WINDOW_SECOND_XPOS, 0);

			g_MainCharInfo.ShowSack( SACKTYPE__PET_EQUIP);			
			g_MainCharInfo.ShowSack( SACKTYPE__DEFAULT);
			g_MainCharInfo.ShowSack( SACKTYPE__EQUIPMENT);
		}
		else if(g_pUIManager->IsShow(WINDOW_TAMING_ITEM))
		{
			CloseAllWindow();
			g_pUIManager->SetPosition(WINDOW_TAMING_ITEM, WINDOW_SECOND_XPOS, 0);

			g_MainCharInfo.ShowSack( SACKTYPE__PET);
			g_MainCharInfo.ShowSack( SACKTYPE__DEFAULT);
			g_MainCharInfo.ShowSack( SACKTYPE__EQUIPMENT);
		}
		else if(g_MainCharInfo.m_bPersonalTradeSell)
		{
			CloseAllWindow();
			g_pUIManager->SetPosition(WINDOW_PC_STORE, WINDOW_SECOND_XPOS, 0);

			g_MainCharInfo.m_pPersonalTradeSet->ShowSack();

			g_MainCharInfo.ShowSack( SACKTYPE__DEFAULT);
			g_MainCharInfo.ShowSack( SACKTYPE__EQUIPMENT);
		}
		else
		{
			CloseAllWindow();
			g_MainCharInfo.ShowSack( SACKTYPE__DEFAULT);
			g_MainCharInfo.ShowSack( SACKTYPE__EQUIPMENT);
		}
	}
}

void ProcessClickCharInfoButton()
{
	if( !g_pUIManager)
	{
		DBG_LogFile( _T("ProcessClickCharInfoButton fail"));
		//return;
	}

	if(g_pUIManager->IsShow(WINDOW_CHARACTER))
	{
		g_MainCharInfo.CloseFrame( WINDOW_CHARACTER);
	}
	else
	{
		CloseAllWindow();

		if(g_pUIManager->IsShow(DRG_ITEM_WINDOW))
		{			
			g_MainCharInfo.OpenFrame( WINDOW_CHARACTER);

			g_pUIManager->SetPosition(WINDOW_CHARACTER, WINDOW_SECOND_XPOS, 0);

			g_MainCharInfo.ShowSack( SACKTYPE__DEFAULT);
			g_MainCharInfo.ShowSack( SACKTYPE__EQUIPMENT);
		}
		else if(g_pUIManager->IsShow(WINDOW_OUTSIDE))
		{
			g_MainCharInfo.OpenFrame( WINDOW_CHARACTER);

			g_pUIManager->SetPosition(WINDOW_CHARACTER, WINDOW_SECOND_XPOS, 0);
			g_MainCharInfo.OpenFrame( WINDOW_OUTSIDE);
		}
		else
		{
			g_MainCharInfo.OpenFrame( WINDOW_CHARACTER);
			g_pUIManager->SetPosition(WINDOW_CHARACTER, WINDOW_FIRST_XPOS, 0);			
		}
	}
}

void ProcessClickMugongButton( BYTE byType)
{
	if( !g_pUIManager)
	{
		DBG_LogFile( _T("ProcessClickMugongButton fail"));
		//return;
	}

	// 2004_06_28 Changth
	// 야차의 외공은 아직 준비가 안되었다.
	// 나중에 이 부분은 주석하자.
//	if( g_MainCharInfo.m_bCharType == 4 && byType == 1 )
//		return;

	int nFrame;
	switch( byType)
	{
		case 1:
			nFrame = WINDOW_OUTSIDE;
			break;
		case 2:
			nFrame = WINDOW_INSIDE;
			break;
		case 3:
			nFrame = WINDOW_FIVEELEMENTS;
			break;
		case 4:	// 각성
			nFrame = WINDOW_SKILL;
			break;
		default:
			nFrame = WINDOW_OUTSIDE;
			break;
	}

	if(g_pUIManager->IsShow(nFrame))
	{
		if(g_pUIManager->IsShow(WINDOW_CHARACTER))
			g_pUIManager->SetPosition(WINDOW_CHARACTER, WINDOW_FIRST_XPOS, 0);

		//HT_1212 : 내공,외공 창에 타이틀 창 수정
		if(g_pUIManager->IsShow(WINDOW_OUTSIDE) || g_pUIManager->IsShow(WINDOW_INSIDE))
			g_pUIManager->Show(DATA_WINDOW);

		g_MainCharInfo.CloseFrame( nFrame);
	}
	else
	{
		CloseAllWindow();	

		if(g_pUIManager->IsShow(WINDOW_CHARACTER))
		{			
			g_MainCharInfo.OpenFrame( WINDOW_CHARACTER);
			g_pUIManager->SetPosition(WINDOW_CHARACTER, WINDOW_SECOND_XPOS, 0);
			g_MainCharInfo.OpenFrame( nFrame);
		}
		else
		{
			g_MainCharInfo.OpenFrame( nFrame);
		}
		//HT_1212 : 내공,외공 창에 타이틀 창 수정
		if(g_pUIManager->IsShow(WINDOW_OUTSIDE) || g_pUIManager->IsShow(WINDOW_INSIDE))
			g_pUIManager->Hide(DATA_WINDOW);
	}
}

void ProcessClickRelationButton( BYTE eType)
{
	if( !g_pUIManager)
	{
		DBG_LogFile( _T("ProcessClickRelationButton fail"));
		//return;
	}

	// 단 경험치 분배
	if(g_pUIManager->IsShow(WINDOW_DAN_NEW) || g_pUIManager->IsShow(WINDOW_DAN) || g_pUIManager->IsShow(WINDOW_MUNPA_FOUND) || g_pUIManager->IsShow(WINDOW_MUNPA))
	{
		g_MainCharInfo.CloseFrame( WINDOW_MUNPA);
		g_MainCharInfo.CloseFrame( WINDOW_MUNPA_FOUND);
		g_MainCharInfo.CloseFrame( WINDOW_DAN);
		g_MainCharInfo.CloseFrame(WINDOW_DAN_NEW);
	}
	else
	{
		g_MainCharInfo.m_pRelation->SetCurrType( (eRELATION_TYPE)eType);
	}
}

//HO_0410_07 상서령 가이드 업데이트
void ProcessClickHelperButton()
{
	if(g_pUIManager->IsShow(WINDOW_HELPER_LIST) || g_pUIManager->IsShow(WINDOW_HELPER_LIST1))
	{
		g_MainCharInfo.CloseFrame( WINDOW_HELPER_LIST1 );
		g_MainCharInfo.CloseFrame( WINDOW_HELPER_LIST );
	}
	else
	{
		CloseAllWindow();
		g_Helper.TalkShow(0);
		g_Helper.TalkContinue();//이곳에서 컨티뉴를 호출한 이유는 TalkListShow(0)으로 호출할경우 이곳은 초원지대입니다. 라는 문구가 뜨기때문임

		if(g_MainCharInfo.m_pChat && g_MainCharInfo.m_pChat->GetChatType() == LARGECHAT)
			g_MainCharInfo.m_pChat->SetChatType(SMALLCHAT);
	}
}

/**
 * 옵션 버튼
 */
void ProcessClickOptionButton()
{
	if(g_pUIManager->IsShow(WINDOW_OPTION_01) || g_pUIManager->IsShow(WINDOW_OPTION_02) || g_pUIManager->IsShow(WINDOW_OPTION_03))
	{
		g_MainCharInfo.CloseFrame( WINDOW_OPTION_01);
		g_MainCharInfo.CloseFrame( WINDOW_OPTION_02);
		g_MainCharInfo.CloseFrame(WINDOW_OPTION_03);
	}
	else
	{
		g_info_Temp = g_info;

		g_info_Temp.m_dwBuyLimit	= g_MainCharInfo.m_dwBuyLimit;
		g_info_Temp.m_bRarityLimit	= g_MainCharInfo.m_bRarityLimit;
		g_info_Temp.m_bStxTypeLimit = g_MainCharInfo.m_bStxTypeLimit;

		CloseAllWindow();
		g_MainCharInfo.OpenFrame( WINDOW_OPTION_01);

		g_pUIManager->SetData(WINDOW_OPTION_01, option_window_1_top_button_01, CURRENT_INDEX, 2);
		g_pUIManager->SetData(WINDOW_OPTION_01, option_window_1_top_button_02, CURRENT_INDEX, -1);
		g_pUIManager->SetData(WINDOW_OPTION_01, option_window_1_top_button_03, CURRENT_INDEX, -1);
	}
}

void ProcessClickCloseButton()
{
	if(!g_pUIManager->IsShow(WINDOW_CLOSE))
	{
		g_MainCharInfo.CloseFrame( GAK_MESSAGE_WINDOW);
		g_MainCharInfo.CloseFrame( WINDOW_DONGSIN);

		g_MainCharInfo.OpenFrame( WINDOW_CLOSE);
	}
	else
	{
		g_MainCharInfo.CloseFrame( WINDOW_CLOSE);
	}
}

void ProcessClickPetSackButton()
{
	CloseAllWindow();
	g_MainCharInfo.ShowSack( SACKTYPE__DEFAULT);
	g_MainCharInfo.ShowSack( SACKTYPE__EQUIPMENT);

	g_pUIManager->SetPosition(WINDOW_TAMING_ITEM, WINDOW_SECOND_XPOS, 0);
	g_MainCharInfo.ShowSack( SACKTYPE__PET);
}

void ProcessClickPetInfoButton()
{
	g_MainCharInfo.RefreshPetInfo();

	CloseAllWindow();

	if(g_pUIManager->IsShow(DRG_ITEM_WINDOW))
	{
		g_pUIManager->SetPosition(WINDOW_NEW_TAMING, WINDOW_SECOND_XPOS, 0);
		g_MainCharInfo.ShowSack( SACKTYPE__PET_EQUIP);
		
		g_MainCharInfo.ShowSack( SACKTYPE__DEFAULT);
		g_MainCharInfo.ShowSack( SACKTYPE__EQUIPMENT);
	}
	else if(g_pUIManager->IsShow(WINDOW_TAMING_ITEM))
	{
		g_pUIManager->SetPosition(WINDOW_TAMING_ITEM, WINDOW_SECOND_XPOS, 0);
		g_MainCharInfo.ShowSack( SACKTYPE__PET);
	}
	else
	{		
		g_pUIManager->SetPosition(WINDOW_NEW_TAMING, WINDOW_FIRST_XPOS, 0);
		g_MainCharInfo.ShowSack( SACKTYPE__PET_EQUIP);
	}
}

void ProcessClickQuestButton()
{
	if(g_pUIManager->IsShow(WINDOW_QUEST_01))
		g_MainCharInfo.CloseFrame( WINDOW_QUEST_01);
	else
	{
		CloseAllWindow();
		g_MainCharInfo.OpenFrame( WINDOW_QUEST_01);
	}
}

void ProcessClickItemConvert()
{
	CloseAllWindow();

	g_MainCharInfo.ShowSack( SACKTYPE__MODIFY);
	g_MainCharInfo.ShowSack( SACKTYPE__DEFAULT);
	g_MainCharInfo.ShowSack( SACKTYPE__EQUIPMENT);
}

void ProcessClickCollection()
{
	if(g_pUIManager->IsShow(WINDOW_COLLECTION))
	{
		g_MainCharInfo.HideSack(SACKTYPE__COLLECTION);
	}
	else
	{
		CloseAllWindow();

		g_MainCharInfo.ShowSack(SACKTYPE__DEFAULT);
		g_MainCharInfo.ShowSack(SACKTYPE__EQUIPMENT);

		g_MainCharInfo.ShowSack(SACKTYPE__COLLECTION);
	}
}