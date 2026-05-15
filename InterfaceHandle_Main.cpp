

/////////////////////////////////////
void ProcessSituation( LPARAM lParam)
/////////////////////////////////////
{
	int controlID = LOWORD( lParam);
	int eventType = HIWORD( lParam);

	g_MainCharInfo.PlayInterfaceSound( ISOUND_SELECT_BUTTON);

	switch( controlID)
	{
	case situation_bar_toggle_down:
//		pFrame->Show();
		break;
	case situation_bar_toggle_up:	// 미니맵
		g_MainCharInfo.ShowMiniMap();
		break;
	case situation_bar_toggle_rotation:
		g_MainCharInfo.RotateMiniMap();
		break;
	}
}

/**
 *
 * \param lParam 
 */
void ProcessMainFrame( LPARAM lParam)
{
	int nControlID = LOWORD( lParam);
	//int eventType = HIWORD( lParam);

	g_MainCharInfo.PlayInterfaceSound( ISOUND_SELECT_BUTTON);

	switch(nControlID)
	{
	case main_frame_window_button:	// 게임 메뉴
		{
//			if(g_MainCharInfo.m_bRebirthItem_Use) break;
            if(g_pUIManager->IsShow(WINDOW_BUTTON_GROUP_01))
				g_pUIManager->Hide(WINDOW_BUTTON_GROUP_01);
			else
				g_pUIManager->Show(WINDOW_BUTTON_GROUP_01);
		}		
		break;

	case main_frame_system_button:	// 시스템 메뉴
		{
//			if(g_MainCharInfo.m_bRebirthItem_Use) break;
			if(g_pUIManager->IsShow(SYSTEM_BUTTON_GROUP_01))
				g_pUIManager->Hide(SYSTEM_BUTTON_GROUP_01);
			else
				g_pUIManager->Show(SYSTEM_BUTTON_GROUP_01);
		}		
		break;

	case main_frame_message_toggle:	// 채팅
		{
			// 매품패
			if(!g_MainCharInfo.m_bPersonalTradeSell && !g_MainCharInfo.m_pQuickMart)
				ProcessEnterForChat();
		}		
		break;

		// 퀵 슬롯 확장
	case main_frame_socket_up:		// 소켓
		{
			if(g_MainCharInfo.m_pSlot)
			{
				g_MainCharInfo.m_pSlot->ChangeSlot(1);
			}
		}
		break;
	case main_frame_socket_down:	// 소켓
		{
			if(g_MainCharInfo.m_pSlot)
			{
				g_MainCharInfo.m_pSlot->ChangeSlot(0);
			}
		}
		break;
	}

	//HO_0413_07 퀵 가이드 업데이트 : 버튼 추가로 인한 위치 수정
	if(!g_pUIManager->IsShow(WINDOW_BUTTON_GROUP_01))//퀵 가이드 버튼 추가로 인한 윈도우, 시스템 그룹버튼 위치 수정
	{
		RECT rtTemp;

		g_pUIManager->GetRegionData(WINDOW_BUTTON_GROUP_01, new_window_button_02, rtTemp);
		g_pUIManager->SetPosition(SYSTEM_BUTTON_GROUP_01, rtTemp.right-2, WINDOW_BUTTON_GROUP_YPOS); 
	}
	else
	{
		RECT rtTemp;

		g_pUIManager->GetRegionData(WINDOW_BUTTON_GROUP_01, new_window_button_02, rtTemp);
		g_pUIManager->SetPosition(SYSTEM_BUTTON_GROUP_01, rtTemp.right-2, SYSTEM_BUTTON_GROUP_YPOS);
	}
}


/**
 * 원도우 버튼 그룹
 * \param lParam 
 */
void ProcessWindowButtonGroup( LPARAM lParam)
{
	int controlID = LOWORD( lParam);
	//int eventType = HIWORD( lParam);

	switch( controlID)
	{
	case new_window_button_01:	// 행낭
		ProcessClickSackButton();		
		break;
	case new_window_button_02:	// 캐릭터정보
		if(!g_MainCharInfo.m_bPersonalTradeSell)
			ProcessClickCharInfoButton();		
		break;
	case new_window_button_03:	// 무공
		if(!g_MainCharInfo.m_bPersonalTradeSell)
			ProcessClickMugongButton( 1);
		break;
	case new_window_button_04:	// 관계
		if(!g_MainCharInfo.m_bPersonalTradeSell)
			ProcessClickRelationButton( eDAN);
		break;
	case new_window_button_05:	// 기연
		if(!g_MainCharInfo.m_bPersonalTradeSell)
			ProcessClickQuestButton();
		break;
	case new_window_button_06:	// 아이템 수집
		{
			if(!g_MainCharInfo.m_bPersonalTradeSell)
			{
				ProcessClickCollection();
			}
		}
		break;
	}

	if(g_MainCharInfo.m_pChat->GetChatType() == LARGECHAT)
	{
		g_MainCharInfo.m_pChat->SetChatType( SMALLCHAT);
	}
}

/////////////////////////////////////////////
void ProcessSystemButtonGroup( LPARAM lParam)
/////////////////////////////////////////////
{
	int controlID = LOWORD( lParam);
	int eventType = HIWORD( lParam);

	switch( controlID)
	{
	case new_system_button_01:	// 미니맵
		g_MainCharInfo.ShowMiniMap( TRUE);
		break;
	case new_system_button_02:	// 헬프
		g_MainCharInfo.OpenFrame( A_HELP);
		break;
	case new_system_button_03:	// 옵션
		if(!g_MainCharInfo.m_bPersonalTradeSell)
			ProcessClickOptionButton();

		if(g_MainCharInfo.m_pChat->GetChatType() == LARGECHAT)
		{
			g_MainCharInfo.m_pChat->SetChatType( SMALLCHAT);
		}
		break;
	case new_system_button_04:	// 끝내기
		ProcessClickCloseButton();
		break;
	}
}


// PET AI지정 버튼
void ProcessPetButtonGroup( LPARAM lParam)
{
	int controlID = LOWORD( lParam);
	int eventType = HIWORD( lParam);

	switch( controlID)
	{
	case pet_button_01:	// 호출
		g_PetList.Change_PET_AI(PETAI_CALLTOME);
		break;
	case pet_button_02:	// 동반공격
		g_PetList.Change_PET_AI(PETAI_AUTOATTACK);
		break;
	case pet_button_03:	// 지정공격
		g_PetList.Change_PET_AI(PETAI_TARGETATTACK);
		g_bCommandAI = TRUE;
		g_dwCommandType = PETAI_TARGETATTACK;
		break;
	case pet_button_04:	// 무공공격
		break;
	case pet_button_05:	// 아이템수집
		g_PetList.Change_PET_AI(PETAI_TAKEITEM);
		break;
	}
}



extern BOOL ProcessChatCommand(LPCTSTR pCommand);
extern void ChangeChatType();

void ProcessMainChat( LPARAM lParam)
{
	int controlID = LOWORD( lParam);
	int eventType = HIWORD( lParam);

	sString szName = _T("");
	sString szContent = _T("");

	switch( controlID)
	{
	case chat_name_edit:			// 이름입력창
		{
			TCHAR strName[128] = {0,};
			memset(strName, 0, sizeof(strName));
			g_pUIManager->GetString(MAIN_CHAT, chat_name_edit, strName, GET_SEND_STRING);

			g_pUIManager->SetString(MAIN_CHAT, chat_name_edit, strName);

			g_MainCharInfo.m_strWhisperName = strName;
		}
		break;
	case main_chat_edit:			// 채팅 내용입력창
		{
			g_MainCharInfo.m_bFirstChat = false;

			TCHAR strTemp[256];
			memset(strTemp, 0, 256);
			g_pUIManager->GetString(MAIN_CHAT, main_chat_edit, strTemp, GET_SEND_STRING);
			szContent = strTemp;

			BYTE byChatType = g_MainCharInfo.GetCurrSendChatType();

			if( szContent.find(_T("/")) == 0)
			{
				ProcessChatCommand(szContent);
			}
			else
			{
				TCHAR strName[128];
				memset(strName, 0, sizeof(strName));
				g_pUIManager->GetString(MAIN_CHAT, chat_name_edit, strName);

				szName = strName; //g_pUIManager->GetString(MAIN_CHAT, chat_name_edit);

				if( byChatType == CT_WHISPER)
				{
					if( szName != _T(""))
					{
						DWORD dwListnerID = 0;
						// 일단 주변에서 찾고
						//dwListnerID = g_MainCharInfo.FindIDByName( (LPCTSTR)szName);
						// Relation에서 찾는다
						if( !dwListnerID)
							dwListnerID = g_MainCharInfo.m_pRelation->FindRelationIDByName( (LPCTSTR)szName);
						
						if( dwListnerID)
							SendCS_CH_CHAT_REQ( byChatType, dwListnerID, szContent, _T(""));
						else
							//g_MainCharInfo.ShowHelpMessage( IDS_CANNOT_FIND,TEXTEFFECT_COLOR_WARNING);
							SendCS_CH_CHAT_REQ( byChatType, 0, szContent, szName);
					}
					else
						g_MainCharInfo.ShowHelpMessage( IDS_TO_WHO,TEXTEFFECT_COLOR_WARNING);
				}
				else if( byChatType == CT_DAN)
				{
					if( g_MainCharInfo.m_pRelation->Am_I_InDan())
						SendCS_CH_CHAT_REQ( byChatType, 0, szContent, _T(""));
					else
                        g_MainCharInfo.ShowHelpMessage( IDS_NO_DAN,TEXTEFFECT_COLOR_WARNING);
				}
				else if( byChatType == CT_MUNPA_BROADCAST)
				{
					SendCS_RL_MUNPACHAT_REQ( CT_MUNPA_BROADCAST, szContent);
				}
				else
				{
					SendCS_CH_CHAT_REQ( byChatType, 0, szContent, _T(""));
				}
			}
		}
		break;

	case 100:
		{
			ChangeChatType();
		}
		break;
	case 200:
		{
			if(g_pUIManager->IsOnEditing() && !g_MainCharInfo.m_bFirstChat)
			{
				g_pUIManager->SetReleaseFocus(MAIN_CHAT);
				g_pUIManager->SetReleaseFocus(MAIN_CHAT, main_chat_edit);
			} // if(g_pUIManager->IsOnEditing() && !g_MainCharInfo.m_bFirstChat)

			g_MainCharInfo.m_bFirstChat = false;
		}		
		break;
	}
}


/**
* 기
* \param lParam 
*/
void ProcessSpirit(LPARAM lParam)
{
	int nControlID = LOWORD(lParam);
	//int nEventType = HIWORD(lParam);

	switch(nControlID)
	{
	case spirit_button:
		{
			SendCS_IF_EXECSTAMINA_REQ();
		}
		break;
	default:
		break;
	}

}

void ProcessHELPER(LPARAM lParam)//HO_0214_07 퀵 가이드 업데이트
{
	int nControlID = LOWORD(lParam);

	switch(nControlID)
	{
		case help_button_resource:
		{
			g_MainCharInfo.m_bQuickIndex = 0;
			CloseAllWindow();
			if(g_MainCharInfo.m_bShowMiniMap = TRUE)  
				g_MainCharInfo.ShowMiniMap( TRUE);

			g_MainCharInfo.OpenFrame( WINDOW_HELPER_LIST0);
			
			g_pUIManager->Hide(HELP_BUTTON);  

//			g_pUIManager->Show(WINDOW_HELPER_LIST0, helper_window0_button_01);
//			g_pUIManager->Show(WINDOW_HELPER_LIST0, helper_window0_button_02);

			sArrayData* pQuickScript = XiahArrayIndex::g_QuickIndex.GetData(g_MainCharInfo.m_wLevel);
			
			if(pQuickScript)
			{
				sString strTemp = pQuickScript->GetString(g_MainCharInfo.m_bQuickIndex);
				TCHAR szLevel[50]={0,};
				_stprintf( szLevel, IDS_D_GAPJA, g_MainCharInfo.m_wLevel);
			
				g_pUIManager->SetString(WINDOW_HELPER_LIST0, helper_window0_small_title_dummy, szLevel);
				g_pUIManager->SetString(WINDOW_HELPER_LIST0, helper_window0_list_dummy, strTemp.data(), 5);
			}
		}
		break;

		default:
		break;
	}
}
