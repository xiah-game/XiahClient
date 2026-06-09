
#include "Fade.h"
#include "XiahGame_Intro.h"
#include "XiahCamera.h"
#include "XiahEnvInfo.h"
#include "xiahbgmcore.h"
#include ".\cxiahgame_login.h"

extern sString	g_ServerName;

#define FILTER	1966/2

// 이름 필터링
BOOL NameFiltering(sString szNickName)
{
	unsigned short filter[1024] = {0,};
	unsigned char name[256] = {0,};
	unsigned short temp;
	int i = 0;

	FILE *fp;
	fp = fopen("ndata.dat","rb");
	if(fp == NULL) return FALSE;

	for(int k=0; k < FILTER;k++)
	{
		fread(&filter[k],1,2,fp);
		if(filter[k] == 0x0a0d) 
			break;
	}
	fclose(fp);
	strcpy((char*)name,szNickName);

	while(name[i] != 0)
	{
		if(name[i] < 0x7b && name[i] > 0x2f)
		{
			// 가능한 ASCII
			i++;
			continue;
		}

		temp = (unsigned short) name[i+1];
		temp = temp << 8;
		temp = temp | (unsigned char)name[i];
		i+=2;

		for(int j=0;j<FILTER;j++)
		{
			if(filter[j] == temp)
				return FALSE;
		}
	}
	return TRUE;
}

/**
 * 캐릭터 생성
 * \param lParam 
 */
void ProcessIntroAccount( LPARAM lParam)
{
	int controlID = LOWORD( lParam);
	int eventType = HIWORD( lParam);

	switch( controlID)
	{
	case account_name_edit:
		{
			TCHAR strName[128];
			memset(strName, 0, sizeof(strName));

			g_pUIManager->GetString(INTRO_ACCOUNT, account_name_edit, strName);
			g_pUIManager->SetString(INTRO_ACCOUNT, account_name_edit, strName);
		}
	case account_button:
		{
			TCHAR strName[128];
			memset(strName, 0, sizeof(strName));

			g_pUIManager->GetString(INTRO_ACCOUNT, account_name_edit, strName);

			sString szNickName = strName;

			g_pUIManager->Hide(INTRO_ACCOUNT, account_name_edit);
			g_pUIManager->SetReleaseFocus(INTRO_ACCOUNT, account_name_edit);
			g_pUIManager->Hide(INTRO_ACCOUNT, account_button);

			if( g_pIntro->GetCharCount() < CHARACTER_MAX)
			{
				g_MainCharInfo.PlayInterfaceSound( ISOUND_SELECT_BUTTON);
				
				if(NameFiltering(szNickName) == TRUE)
				{
					SendCS_IT_NEWCHARACTER_REQ( g_pIntro->m_nCurrentCharacter, szNickName);
				}
				else
				{
					//g_MainCharInfo.PlayInterfaceSound( ISOUND_WARNING); 아래 메시지 전달시 경고는 사운드 출력함
					g_MainCharInfo.ShowHelpMessage(IT_WARNNIG1,TEXTEFFECT_COLOR_WARNING);

					g_pUIManager->Show(INTRO_ACCOUNT, account_name_edit);
					g_pUIManager->Show(INTRO_ACCOUNT, account_button);
				}
//				SendCS_IT_NEWCHARACTER_REQ( g_pIntro->GetCurrentClassIndex(), szNickName);
			}
			else
			{
				g_MainCharInfo.PlayInterfaceSound( ISOUND_WARNING);
				g_MainCharInfo.ShowHelpMessage( IDS_CANNOT_CREATE_CHAR);
			}
		}
		break;
	}
}

/**
 * 인트로 선택 버튼
 * \param lParam 
 */
void ProcessIntroButtonSet( LPARAM lParam)
{
	int controlID = LOWORD( lParam);
	//int eventType = HIWORD( lParam);

	switch( controlID)
	{
	case INTRO_BUTTON_01:	// Create
		{
			if( g_pIntro && g_pIntro->m_bCharacterSelected)
			{
				// 캐릭터 설명 창
				g_pUIManager->Hide(INTRO_WINDOW01);

				// 캐릭터 만들기
				if( g_pIntro->m_byCharCount < 3 )
				{
					g_pUIManager->Show(INTRO_BUTTONSET, INTRO_BUTTON_01);
					g_pUIManager->SetString(INTRO_BUTTONSET, INTRO_BUTTON_01, IDS_INTRO_BUTTONSET_BUTTON11);
				}
				else
				{
					g_pUIManager->Hide(INTRO_BUTTONSET, INTRO_BUTTON_01);
				}
				// 시작
				g_pUIManager->Hide(INTRO_BUTTONSET, INTRO_BUTTON_04);
				// 채널
				g_pUIManager->Show(INTRO_BUTTONSET, INTRO_BUTTON_03);
				// 뿟릿써監객큐匡俚
				g_pUIManager->SetString(INTRO_BUTTONSET, INTRO_BUTTON_02, IDS_TERMINATE);

				// 기존에 애니메이션되는 것을 정지한다.
				g_pIntro->m_CharRender[g_pIntro->m_byCurrentCharIndex].SetAnimation( XiahAniType::eLAT_Stand, 0 );

				g_pIntro->m_nCharacterSelectStep = 3;//eCST_ZoomOut;
				g_XiahCamera.m_fDestXAngle = -0.43633235f;	//  -_PI / 4.5f;

				g_MainCharInfo.PlayInterfaceSound( ISOUND_ZOOM_OUT );
			}
			else if( g_pIntro->GetCharCount() < CHARACTER_MAX)
			{
				// [12/20/2004] 배경음악 변경
				Stop_BGM();
				Play_BGM(_T("sound\\bgm\\intro02.mp3"), 1);

				g_MainCharInfo.PlayInterfaceSound( ISOUND_SELECT_BUTTON);
				g_pIntro->GoToMenuCreateChar();
			}
			else
			{
				g_MainCharInfo.PlayInterfaceSound( ISOUND_WARNING);
				g_MainCharInfo.ShowHelpMessage( IDS_CANNOT_CREATE_CHAR);
			}
		}
		break;
	case INTRO_BUTTON_04:	// Start
		{
			g_MainCharInfo.PlayInterfaceSound( ISOUND_GAME_START_BUTTON);

			// [1/6/2005] 옥션
			if(g_pIntro->GetCurrentChar()->m_bAuction == 1)
			{
				// 2004.07.20 이벤트용 로딩화면
				/*
				if( rand() % 2 )
				g_MainCharInfo.OpenFrame( EVENT_LOADING_1 );
				else
				g_MainCharInfo.OpenFrame( EVENT_LOADING_2 );
				*/

				
				g_MainCharInfo.OpenFrame(LOADING_IMAGE3); //HO_0702_07 등급표시 : 등급표시와 함게 스타트로딩과 게임로딩 부분이 동일 이미지로 처리된다.
				
				//등급표시 적용전 코드 나중에 지워 버리자 ..; 등급표시 전에는 나이 구분이 있엇다...
				//if(g_AppData.m_bAdult)
				//	g_MainCharInfo.OpenFrame(LOADING_IMAGE2);
				//else
				//	g_MainCharInfo.OpenFrame(LOADING_IMAGE);

				
				g_GameWork.m_nNavigationMode = 0;
				SendCS_NV_STARTGAME_REQ();

				TCHAR strServerState[64] = {0,};
				sprintf(strServerState,"%s : %s", IDS_SERVERNAME, g_ServerName.data());
				g_pUIManager->SetString(WINDOW_CLOSE, close_window_state_dummy, strServerState);

				g_XiahEnvInfo.m_CameraBoundSize = 128 + (1024 - 128) * (g_EngineInfo.m_fViewDistance / 10.0f);
				g_RainSnow.AllStop();


				// 기
				for(int i=0;i < 5; ++i)
				{
					g_pUIManager->Hide(MAIN_FRAME, main_frame_1_gauge_01 + i);
				}

				g_pUIManager->Hide(SPIRIT);
				g_MainCharInfo.m_bStaminaCnt	= 0;
				g_MainCharInfo.m_bSpirit		= 0;
			}			
		}
		break;	
	case INTRO_BUTTON_03:	// 채널 선택
		{
			g_MainCharInfo.PlayInterfaceSound(ISOUND_SELECT_BUTTON);
			SendCS_NV_ENDGAME_REQ(1);			

			// [6/14/2004] 홈페이지에서 삭제

		}
		break;
	case INTRO_BUTTON_02:	// End / Delete Character
		{
			g_MainCharInfo.PlayInterfaceSound( ISOUND_SELECT_BUTTON);

			if( g_pIntro->m_bCharacterSelected)
			{
				// Character is locked - delete character
				g_pIntro->m_bCanSelectCharacter = false;

				TCHAR szContent[256] = {0,};
				_stprintf( szContent, IDS_D_DEL_CHAR, (LPCTSTR)g_pIntro->GetCurrentChar()->m_szNickName);

				g_pUIManager->ShowNotice( szContent, NOTICE_FRAME_OKCANCEL, NOTICE_FRAME_DELETE_CHARACTER);
			}
			else if( g_pIntro->GetCurrentMenu() == INTROMENU_SELECTCHAR)
			{
				SendCS_NV_ENDGAME_REQ();
				PostMessage( g_AppData.m_hWnd, WM_CLOSE, 0, 0);
			}
			else
			{
				g_MainCharInfo.DeleteAllScrMessage();

				Fade::StartFade( 0, 0, NULL, 1500);
				g_pIntro->GoToMenuSelectChar();
			}
		}
		break;
		
	}
}

void ProcessIntroCharacterSelect( LPARAM lParam)
{
	int controlID = LOWORD( lParam);
	int eventType = HIWORD( lParam);

	g_MainCharInfo.PlayInterfaceSound( ISOUND_SELECT_BUTTON);

	if( g_pIntro->GetCurrentMenu() == INTROMENU_SELECTCHAR)
	{
		BYTE byCurrentCharIndex = g_pIntro->GetCurrentCharIndex();

		switch( controlID)
		{
		case intro_character_select_left:
			if( byCurrentCharIndex == 0)
				byCurrentCharIndex = g_pIntro->GetCharCount() - 1;
			else
				byCurrentCharIndex--;
			break;
		case intro_character_select_right:
			if( byCurrentCharIndex == g_pIntro->GetCharCount() - 1)
				byCurrentCharIndex = 0;
			else
				byCurrentCharIndex++;
			break;
		}

		if( byCurrentCharIndex < CHARACTER_MAX)
			g_pIntro->SetCurrentCharInfo( byCurrentCharIndex);
	}
	else
	{
		// 시아 캐릭터들을 로테이션. 검영 -> 연랑 -> 무투 -> 검영 ... 
		switch( controlID )
		{
		case intro_character_select_right:
			g_pIntro->m_nCurrentCharacter++;
			if( g_pIntro->m_nCurrentCharacter > XIAH_CHAR_MAX )
				g_pIntro->m_nCurrentCharacter = 1;
			break;
		case intro_character_select_left:
			g_pIntro->m_nCurrentCharacter--;
			if( g_pIntro->m_nCurrentCharacter == 0 )
				g_pIntro->m_nCurrentCharacter = XIAH_CHAR_MAX;
			break;
		}


/*		Original Code
		BYTE byCurrentClassIndex = g_pIntro->GetCurrentClassIndex();

		switch( controlID)
		{
		case intro_character_select_left:
			if( byCurrentClassIndex == 1)
				byCurrentClassIndex = CLASS_MAX;
			else
				byCurrentClassIndex--;
			break;
		case intro_character_select_right:
			if( byCurrentClassIndex == CLASS_MAX)
				byCurrentClassIndex = 1;
			else
				byCurrentClassIndex++;
			break;
		}

		if( byCurrentClassIndex < CLASS_MAX+1)
			g_pIntro->SetCurrentClassInfo( byCurrentClassIndex);
*/

	}
}

void ProcessIntroWindow( LPARAM lParam)
{
}

// login
// 계정 로그인
void ProcessLogin1(LPARAM lParam)
{
	int controlID = LOWORD(lParam);
	int eventType = HIWORD(lParam);

	switch(controlID)
	{
	case login1_ok:
		{
			g_MainCharInfo.PlayInterfaceSound(ISOUND_SELECT_BUTTON);

			if(!g_pUIManager->IsNotice())
			{
				// 임시
				TCHAR strID[64], strPassWord[64];
				memset(strID, 0, sizeof(strID));
				memset(strPassWord, 0, sizeof(strPassWord));

				g_pUIManager->GetString(LOGIN_1, login1_edit_id, strID);
				g_pUIManager->GetString(LOGIN_1, login1_edit_password, strPassWord);

				SendCS_IT_LOGIN_AUTH_REQ(strID, strPassWord);

				g_pUIManager->SetData(LOGIN_1, login1_ok, CURRENT_INDEX, 2);
			}
		}		
		break;

	case login1_exit:
		{
			g_MainCharInfo.PlayInterfaceSound(ISOUND_SELECT_BUTTON);
			PostMessage( g_AppData.m_hWnd, WM_CLOSE, 0, 0);
		}		
		break;

	case login1_edit_id:
		{
			g_pUIManager->SetFocus(LOGIN_1, login1_edit_password);
		}
		break;

	case login1_edit_password:
		{
			if(g_pUIManager->GetData(LOGIN_1, login1_ok, GET_CURRENT_INDEX) == 2)
				return;

			TCHAR strID[64], strPassWord[64];
			memset(strID, 0, sizeof(strID));
			memset(strPassWord, 0, sizeof(strPassWord));

			g_pUIManager->GetString(LOGIN_1, login1_edit_id, strID);
			g_pUIManager->GetString(LOGIN_1, login1_edit_password, strPassWord);

			if(_tcslen(strID) && _tcslen(strPassWord))
			{
				if(!g_pUIManager->IsNotice())
				{
					SendCS_IT_LOGIN_AUTH_REQ(strID, strPassWord);
					g_pUIManager->SetData(LOGIN_1, login1_ok, CURRENT_INDEX, 2);
				}
			}
		}
		break;

	case 100:
		{
			if(g_pUIManager->IsShow(LOGIN_1, login1_pw_change_back))
			{
				bool bFocus1 = g_pUIManager->IsFocus(LOGIN_1, login1_pw_change_edit_01);
				bool bFocus2 = g_pUIManager->IsFocus(LOGIN_1, login1_pw_change_edit_02);
				bool bFocus3 = g_pUIManager->IsFocus(LOGIN_1, login1_pw_change_edit_03);
				bool bFocus4 = g_pUIManager->IsFocus(LOGIN_1, login1_pw_change_edit_04);

				int nFocusID = login1_pw_change_edit_01;

				if(bFocus1)
					nFocusID = login1_pw_change_edit_02;
				else if(bFocus2)
					nFocusID = login1_pw_change_edit_03;
				else if(bFocus3)
					nFocusID = login1_pw_change_edit_04;
				else if(bFocus4)
					nFocusID = login1_pw_change_edit_01;

				g_pUIManager->SetFocus(LOGIN_1, nFocusID);				
			}
			else
			{
				if(g_pUIManager->IsFocus(LOGIN_1, login1_edit_id))
					g_pUIManager->SetFocus(LOGIN_1, login1_edit_password);
				else
					g_pUIManager->SetFocus(LOGIN_1, login1_edit_id);
			}
		}
		break;

		// RS [11/29/2005] 비밀번호 변경 추가
	case login1_pw_change_button:
		{
			g_pUIManager->Hide(LOGIN_1, login1_ok);
			g_pUIManager->Hide(LOGIN_1, login1_exit);
			g_pUIManager->Hide(LOGIN_1, login1_pw_change_button);
			g_pUIManager->Hide(LOGIN_1, login1_edit_id);
			g_pUIManager->Hide(LOGIN_1, login1_edit_password);
			g_pUIManager->Hide(LOGIN_1, login1_id_dummy);
			g_pUIManager->Hide(LOGIN_1, login1_pw_dummy);


			g_pUIManager->Show(LOGIN_1, login1_pw_change_back);
			g_pUIManager->Show(LOGIN_1, login1_pw_change_button_01);
			g_pUIManager->Show(LOGIN_1, login1_pw_change_button_02);

			g_pUIManager->Show(LOGIN_1, login1_pw_change_dummy_01);
			g_pUIManager->Show(LOGIN_1, login1_pw_change_dummy_02);
			g_pUIManager->Show(LOGIN_1, login1_pw_change_dummy_03);
			g_pUIManager->Show(LOGIN_1, login1_pw_change_dummy_04);

			g_pUIManager->Show(LOGIN_1, login1_pw_change_edit_01);
			g_pUIManager->Show(LOGIN_1, login1_pw_change_edit_02);
			g_pUIManager->Show(LOGIN_1, login1_pw_change_edit_03);
			g_pUIManager->Show(LOGIN_1, login1_pw_change_edit_04);

			g_pUIManager->SetData(LOGIN_1, login1_pw_change_button_01, CURRENT_INDEX, -1);

			g_pUIManager->SetString(LOGIN_1, login1_edit_id, _T(""));
			g_pUIManager->SetString(LOGIN_1, login1_edit_password, _T(""));

			g_pUIManager->SetString(LOGIN_1, login1_pw_change_edit_01, _T(""));
			g_pUIManager->SetString(LOGIN_1, login1_pw_change_edit_02, _T(""));
			g_pUIManager->SetString(LOGIN_1, login1_pw_change_edit_03, _T(""));
			g_pUIManager->SetString(LOGIN_1, login1_pw_change_edit_04, _T(""));

			g_pUIManager->SetFocus(LOGIN_1, login1_pw_change_edit_01);
		}
		break;
	case login1_pw_change_button_01:
	case login1_pw_change_edit_04:
		{
			if(g_pUIManager->GetData(LOGIN_1, login1_pw_change_button_01, GET_CURRENT_INDEX) == 2)
				return;

			if(!g_pUIManager->IsNotice())
			{
				TCHAR strID[64], strCurPassWord[64], strChangePassWord1[64], strChangePassWord2[64];
				memset(strID, 0, sizeof(strID));
				memset(strCurPassWord, 0, sizeof(strCurPassWord));
				memset(strChangePassWord1, 0, sizeof(strChangePassWord1));
				memset(strChangePassWord2, 0, sizeof(strChangePassWord2));

				g_pUIManager->GetString(LOGIN_1, login1_pw_change_edit_01, strID);
				g_pUIManager->GetString(LOGIN_1, login1_pw_change_edit_02, strCurPassWord);
				g_pUIManager->GetString(LOGIN_1, login1_pw_change_edit_03, strChangePassWord1);
				g_pUIManager->GetString(LOGIN_1, login1_pw_change_edit_04, strChangePassWord2);

				if(_tcslen(strID) && _tcslen(strCurPassWord))
				{
					int nChangePW = _tcslen(strChangePassWord1);
					if(nChangePW)
					{
						// 4자리 이상
						if(nChangePW < 4)
						{
							g_pUIManager->ShowNotice(IDS_PW_CHANGE_06);							

							return;
						}

						if(_tcslen(strChangePassWord2))
						{
							if(_tcsicmp(strCurPassWord, strChangePassWord1) == 0)
							{
								// TODO: 기존 비번과 새 비번이 같음
								g_pUIManager->ShowNotice(IDS_PW_CHANGE_04);
								return;
							}

							if(_tcsicmp(strChangePassWord1, strChangePassWord2) != 0)
							{
								// TODO: 새 비번과 재횅땍 새 비번이 틀림
								g_pUIManager->ShowNotice(IDS_PW_CHANGE_05);
								return;
							}

							SendCS_IT_CHANGEPW_REQ(strID, strCurPassWord, strChangePassWord1);

							g_pUIManager->SetData(LOGIN_1, login1_pw_change_button_01, CURRENT_INDEX, 2);
						}
						else
						{
							// TODO: 새 비번 재횅땍
							g_pUIManager->ShowNotice(IDS_PW_CHANGE_03);
						}
					}
					else
					{
						// TODO: 새 비번
						g_pUIManager->ShowNotice(IDS_PW_CHANGE_02);
					}
				}
				else
				{
					// TODO: 비밀 번호 입력
					g_pUIManager->ShowNotice(IDS_PW_CHANGE_01);
				}
			}
		}
		break;
	case login1_pw_change_button_02:
		{
			g_pUIManager->Show(LOGIN_1, login1_ok);
			g_pUIManager->Show(LOGIN_1, login1_exit);
			g_pUIManager->Show(LOGIN_1, login1_pw_change_button);
			g_pUIManager->Show(LOGIN_1, login1_edit_id);
			g_pUIManager->Show(LOGIN_1, login1_edit_password);
			g_pUIManager->Show(LOGIN_1, login1_id_dummy);
			g_pUIManager->Show(LOGIN_1, login1_pw_dummy);


			g_pUIManager->Hide(LOGIN_1, login1_pw_change_back);
			g_pUIManager->Hide(LOGIN_1, login1_pw_change_button_01);
			g_pUIManager->Hide(LOGIN_1, login1_pw_change_button_02);

			g_pUIManager->Hide(LOGIN_1, login1_pw_change_dummy_01);
			g_pUIManager->Hide(LOGIN_1, login1_pw_change_dummy_02);
			g_pUIManager->Hide(LOGIN_1, login1_pw_change_dummy_03);
			g_pUIManager->Hide(LOGIN_1, login1_pw_change_dummy_04);

			g_pUIManager->Hide(LOGIN_1, login1_pw_change_edit_01);
			g_pUIManager->Hide(LOGIN_1, login1_pw_change_edit_02);
			g_pUIManager->Hide(LOGIN_1, login1_pw_change_edit_03);
			g_pUIManager->Hide(LOGIN_1, login1_pw_change_edit_04);

			g_pUIManager->SetString(LOGIN_1, login1_pw_change_edit_01, _T(""));
			g_pUIManager->SetString(LOGIN_1, login1_pw_change_edit_02, _T(""));
			g_pUIManager->SetString(LOGIN_1, login1_pw_change_edit_03, _T(""));
			g_pUIManager->SetString(LOGIN_1, login1_pw_change_edit_04, _T(""));

			g_pUIManager->SetString(LOGIN_1, login1_edit_id, _T(""));
			g_pUIManager->SetString(LOGIN_1, login1_edit_password, _T(""));
		}
		break;

	case login1_pw_change_edit_01:
		{
			g_pUIManager->SetFocus(LOGIN_1, login1_pw_change_edit_02);
		}
		break;
	case login1_pw_change_edit_02:
		{
			g_pUIManager->SetFocus(LOGIN_1, login1_pw_change_edit_03);
		}
		break;
	case login1_pw_change_edit_03:
		{
			g_pUIManager->SetFocus(LOGIN_1, login1_pw_change_edit_04);
		}
		break;
	}
}

// 채널 선택
void ProcessLogin2(LPARAM lParam)
{
	int controlID = LOWORD(lParam);
	int eventType = HIWORD(lParam);

	switch(controlID)
	{
	case login2_ok:
		{
			g_MainCharInfo.PlayInterfaceSound(ISOUND_SELECT_BUTTON);

			if(!g_pUIManager->IsNotice())
			{
				CXiahGame_Login *pGameMainStep = (CXiahGame_Login*)g_GameStep[ GAMESTEP_LOGIN];

				pGameMainStep->ConnectToServer();
			}
		}
		break;

	case login2_exit:
		{
			g_MainCharInfo.PlayInterfaceSound(ISOUND_SELECT_BUTTON);
			PostMessage(g_AppData.m_hWnd, WM_CLOSE, 0, 0);
		}
		break;
	}
}