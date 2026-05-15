#include "precompile.h"
#include "Helper.h"
#include "ArrayIndexData.h"
#include "InterfaceDefine.h"
#include "CharacterInfo.h"
#include "XiahGame_Handler_Sender.h"

#include <assert.h>


Helper g_Helper;

Helper::Helper() : m_nSelectID(0), m_bSelect(false)
{

}

Helper::~Helper()
{
}

void Helper::Update()
{
	//HO_0410_07 상서령 가이드 업데이트
	if(g_pUIManager->IsShow(WINDOW_HELPER_LIST1))
	{
		sRect rtRect;

		g_pUIManager->GetRegionData(WINDOW_HELPER_LIST1, -1, rtRect);

		if(rtRect.PtInRect(XiahInput::g_ptMouse))
		{
			sArrayData* pHelperScript = XiahArrayIndex::g_HelperScript.GetData(m_nSelectID);

			if(pHelperScript)
			{
				int nChildCount = pHelperScript->GetInt(3);

				for(int i=0; i < nChildCount; ++i)
				{
					g_pUIManager->GetRegionData(WINDOW_HELPER_LIST1, helper_window1_list_dummy3 + i, rtRect);

					if(rtRect.PtInRect(XiahInput::g_ptMouse))
					{
						if(XiahInput::g_bLButtonDown && !m_bSelect)
						{
							int nChildID = pHelperScript->GetInt(2) + i;

							sArrayData* pTalk = XiahArrayIndex::g_HelperScript.GetData(nChildID);

							if(pTalk)
							{
								int nChild = pTalk->GetInt(2);

								if(nChild)
								{
									TalkListShow(nChildID);
								}
								else
								{
									g_MainCharInfo.CloseFrame(WINDOW_HELPER_LIST1);

									TalkShow(nChildID);
								}

								m_bSelect = true;							
							}							
						} // if(XiahInput::g_bLButtonDown && !m_bSelect)

						if(XiahInput::g_bLButtonUp)
							m_bSelect = false;

						return;
					}
				} // for(int i=0; i < nChildCount; ++i)

				m_bSelect = false;
			}
		} // if(rtRect.PtInRect(XiahInput::g_ptMouse))
	}
	//HO_0410_07 상서령 가이드 업데이트 : 상서령 업데이트 전 코드를 탐랑 용을 위하여 사용
	else if(g_pUIManager->IsShow(WINDOW_HELPER_LIST2))
	{
		sRect rtRect;

		g_pUIManager->GetRegionData(WINDOW_HELPER_LIST2, -1, rtRect);

		if(rtRect.PtInRect(XiahInput::g_ptMouse))
		{
			sArrayData* pHelperScript = XiahArrayIndex::g_HelperScript.GetData(m_nSelectID);

			if(pHelperScript)
			{
				int nChildCount = pHelperScript->GetInt(3);

				for(int i=0; i < nChildCount; ++i)
				{
					g_pUIManager->GetRegionData(WINDOW_HELPER_LIST2, helper_window_list_dummy_03 + i, rtRect);

					if(rtRect.PtInRect(XiahInput::g_ptMouse))
					{
						if(XiahInput::g_bLButtonDown && !m_bSelect)
						{
							int nChildID = pHelperScript->GetInt(2) + i;

							sArrayData* pTalk = XiahArrayIndex::g_HelperScript.GetData(nChildID);

							if(pTalk)
							{
								int nChild = pTalk->GetInt(2);

								if(nChild)
								{
									TalkListShow(nChildID);
								}
								else
								{
									g_MainCharInfo.CloseFrame(WINDOW_HELPER_LIST2);

									TalkShow(nChildID);
								}

								m_bSelect = true;							
							}							
						} // if(XiahInput::g_bLButtonDown && !m_bSelect)

						if(XiahInput::g_bLButtonUp)
							m_bSelect = false;

						return;
					}
				} // for(int i=0; i < nChildCount; ++i)

				m_bSelect = false;
			}
		} // if(rtRect.PtInRect(XiahInput::g_ptMouse))
	}
	else if(g_pUIManager->IsShow(WINDOW_SECRET_BASIS))
	{
		sRect rtRect;

		g_pUIManager->GetRegionData(WINDOW_SECRET_BASIS, -1, rtRect);

		if(rtRect.PtInRect(XiahInput::g_ptMouse))
		{
			sArrayData* pHelperScript = XiahArrayIndex::g_HelperScript.GetData(m_nSelectID);

			if(pHelperScript)
			{
				for(int i=0; i < 2; ++i)
				{
					g_pUIManager->GetRegionData(WINDOW_SECRET_BASIS, secret_basis_window_dummy02 + i, rtRect);

					if(rtRect.PtInRect(XiahInput::g_ptMouse))
					{
						if(XiahInput::g_bLButtonDown && !m_bSelect)
						{
							int nChildID = pHelperScript->GetInt(2) + i;

							sArrayData* pTalk = XiahArrayIndex::g_HelperScript.GetData(nChildID);

							if(pTalk)
							{
								switch(nChildID)
								{
								case 113:
									SendCS_WR_APPLYSECRETREADY_REQ();
									break;
								case 116:
									SendCS_WR_APPLYDEVILREADY_REQ();
									break;
								}
								g_MainCharInfo.CloseFrame(WINDOW_SECRET_BASIS);

								TalkShow(nChildID);

								m_bSelect = true;							
							}							
						} // if(XiahInput::g_bLButtonDown && !m_bSelect)

						if(XiahInput::g_bLButtonUp)
							m_bSelect = false;

						return;
					}
				} // for(int i=0; i < nChildCount; ++i)

				m_bSelect = false;
			}
		} // if(rtRect.PtInRect(XiahInput::g_ptMouse))
	}
}

bool Helper::TalkShow(const int nOriginalID)
{
	sArrayData* pHelperScript = XiahArrayIndex::g_HelperScript.GetData(nOriginalID);

	//HO_0410_07 상서령 가이드 업데이트
	if(pHelperScript && nOriginalID <= 110)
	{
		if(nOriginalID >= 99)// 상서령 업데이트 가이드 전 코드를 탐랑 대화용으로 변환
		{
			g_MainCharInfo.OpenFrame(WINDOW_HELPER_SCRIPT);

			sString strTemp = pHelperScript->GetString(1);

			g_pUIManager->SetString(WINDOW_HELPER_SCRIPT, helper_window_script_dummy, _T(""));
			g_pUIManager->SetString(WINDOW_HELPER_SCRIPT, helper_window_script_dummy, strTemp.data(),0);

			// 버튼 보이기
			g_pUIManager->Show(WINDOW_HELPER_SCRIPT, helper_window_script_button1);
			g_pUIManager->Show(WINDOW_HELPER_SCRIPT, helper_window_script_button2);
		}
		else
		{
			//HO_0410_07 상서령 가이드 업데이트
			g_MainCharInfo.OpenFrame(WINDOW_HELPER_LIST);

			m_nSelectID = nOriginalID;

			sString strTitle =pHelperScript->GetString(0);
			sString strTemp = pHelperScript->GetString(1);


			int nParentID = pHelperScript->GetInt(1);

			if(nParentID < 0)
				strTitle = _T("");
			
			g_pUIManager->SetString(WINDOW_HELPER_LIST, helper_window_small_title_dummy, strTitle.data());
			
			g_pUIManager->SetString(WINDOW_HELPER_LIST, helper_window_list_dummy, _T(""));
			g_pUIManager->SetString(WINDOW_HELPER_LIST, helper_window_list_dummy, strTemp.data(), 5);

			g_pUIManager->Hide(WINDOW_HELPER_LIST1, helper_window1_button_01);
			g_pUIManager->Hide(WINDOW_HELPER_LIST1, helper_window1_button_02);
			// 버튼 보이기
			g_pUIManager->Show(WINDOW_HELPER_LIST, helper_window_button_01);
			g_pUIManager->Show(WINDOW_HELPER_LIST, helper_window_button_02);
		}		
	}
	else if(pHelperScript)
	{
		switch(nOriginalID)
		{
		case 112:
			{
				g_MainCharInfo.OpenFrame(WINDOW_SECRET_CHECK);

				sString strTemp = pHelperScript->GetString(0);

				g_pUIManager->SetString(WINDOW_SECRET_CHECK, secret_check_window_dummy01, _T(""));
				g_pUIManager->SetString(WINDOW_SECRET_CHECK, secret_check_window_dummy01, strTemp.data());
				
				strTemp = pHelperScript->GetString(1);
				g_pUIManager->SetString(WINDOW_SECRET_CHECK, secret_check_window_dummy02, _T(""));
				g_pUIManager->SetString(WINDOW_SECRET_CHECK, secret_check_window_dummy02, strTemp.data());
			}
			break;
		case 113:
			{
				CHAR szTip [128] = {0,};

				g_MainCharInfo.OpenFrame(WINDOW_SECRET_INFORMATION);

				sString strTemp = pHelperScript->GetString(0);
				g_pUIManager->SetString(WINDOW_SECRET_INFORMATION, secret_information_window_dummy01, _T(""));
				g_pUIManager->SetString(WINDOW_SECRET_INFORMATION, secret_information_window_dummy01, strTemp.data(),0);
				
				strTemp = pHelperScript->GetString(1);
				g_pUIManager->SetString(WINDOW_SECRET_INFORMATION, secret_information_window_dummy03, _T(""));
				g_pUIManager->SetString(WINDOW_SECRET_INFORMATION, secret_information_window_dummy03, strTemp.data(),0);
			}
			break;
		case 115:
			{
				g_MainCharInfo.OpenFrame(WINDOW_SECRET_CHECK);

				sString strTemp = pHelperScript->GetString(0);

				g_pUIManager->SetString(WINDOW_SECRET_CHECK, secret_check_window_dummy01, _T(""));
				g_pUIManager->SetString(WINDOW_SECRET_CHECK, secret_check_window_dummy01, strTemp.data());
				
				strTemp = pHelperScript->GetString(1);
				g_pUIManager->SetString(WINDOW_SECRET_CHECK, secret_check_window_dummy02, _T(""));
				g_pUIManager->SetString(WINDOW_SECRET_CHECK, secret_check_window_dummy02, strTemp.data());
			}
			break;
		case 116:
			{
				CHAR szTip [128] = {0,};

				g_MainCharInfo.OpenFrame(WINDOW_SECRET_INFORMATION);

				sString strTemp = pHelperScript->GetString(0);
				g_pUIManager->SetString(WINDOW_SECRET_INFORMATION, secret_information_window_dummy01, _T(""));
				g_pUIManager->SetString(WINDOW_SECRET_INFORMATION, secret_information_window_dummy01, strTemp.data());
				
				strTemp = pHelperScript->GetString(1);
				g_pUIManager->SetString(WINDOW_SECRET_INFORMATION, secret_information_window_dummy03, _T(""));
				g_pUIManager->SetString(WINDOW_SECRET_INFORMATION, secret_information_window_dummy03, strTemp.data());
			}
			break;
		}
	}
	
	else
	{
		return false;
	}

	return true;
}




bool Helper::TalkListShow(const int nID)
{
	sArrayData* pHelperScript = XiahArrayIndex::g_HelperScript.GetData(nID);

	//HO_0410_07 상서령 가이드 업데이트 : 상서령 업데이트 전 코드를 (99)조건을 주어 탐랑 전용으로 만듬
	if(pHelperScript && nID <= 110)
	{
		if(nID == 99 || nID == 105)
		{	
			g_pUIManager->Hide(WINDOW_HELPER_SCRIPT, helper_window_script_button1);
			g_pUIManager->Hide(WINDOW_HELPER_SCRIPT, helper_window_script_button2);

			for(int j=0; j < 11; ++j)
			{
				g_pUIManager->SetString(WINDOW_HELPER_LIST2, helper_window_list_dummy_01 + j, _T(" "));
			}

			m_nSelectID = nID;

			int nChildCount = pHelperScript->GetInt(3);

			// 인터페이스 개수 12개 초과는... 문제가 생기지...
			assert(nChildCount <= 12);

			if(nChildCount)
			{
				int nChildID = pHelperScript->GetInt(2);

				for(int i=0; i < nChildCount; ++i)
				{
					sArrayData* pTalk = XiahArrayIndex::g_HelperScript.GetData(nChildID + i);

					if(pTalk)
					{
						sString strTitle = pTalk->GetString(0);

						g_pUIManager->SetString(WINDOW_HELPER_LIST2, helper_window_list_dummy_03 + i, strTitle.data(), 3);
					}
				}

				g_MainCharInfo.OpenFrame(WINDOW_HELPER_LIST2);
			}

			else
			{
				return false;
			}
		}
		else
		{
			// 버튼 닫기
			g_pUIManager->Hide(WINDOW_HELPER_LIST, helper_window_button_01);
			g_pUIManager->Hide(WINDOW_HELPER_LIST, helper_window_button_02);

			g_pUIManager->Show(WINDOW_HELPER_LIST1, helper_window1_button_01);
			g_pUIManager->Show(WINDOW_HELPER_LIST1, helper_window1_button_02);

			for(int j=0; j < 11; ++j)
			{
				g_pUIManager->SetString(WINDOW_HELPER_LIST1, helper_window1_list_dummy + j, _T(" "));
			}

			m_nSelectID = nID;

			int nChildCount = pHelperScript->GetInt(3);

			// 인터페이스 개수 12개 초과는... 문제가 생기지...
			assert(nChildCount <= 12);

			sString strTitle = _T("");
			sString strSmallTitle = _T("");

			if(nChildCount)
			{
				int nChildID = pHelperScript->GetInt(2);

				strSmallTitle = pHelperScript->GetString(0);

				if(strlen(strSmallTitle) <= 1)
					strSmallTitle = _T("");

				g_pUIManager->SetString(WINDOW_HELPER_LIST1, helper_window1_small_title_dummy, strSmallTitle.data());

				for(int i=0; i < nChildCount; ++i)
				{
					sArrayData* pTalk = XiahArrayIndex::g_HelperScript.GetData(nChildID + i);

					if(pTalk)
					{				
						strTitle = pTalk->GetString(0);
						g_pUIManager->SetString(WINDOW_HELPER_LIST1, helper_window1_list_dummy3 + i, strTitle.data(), 3);
					}
				}

				g_MainCharInfo.OpenFrame(WINDOW_HELPER_LIST1);
			}
			else
			{
				return false;
			}
		}		
	}
	else if(pHelperScript)
	{
		g_pUIManager->SetString(WINDOW_SECRET_BASIS, secret_basis_window_dummy02, _T(" "));
		g_pUIManager->SetString(WINDOW_SECRET_BASIS, secret_basis_window_dummy03, _T(" "));

		m_nSelectID = nID;
		sArrayData* pTalk;
		sString strTitle;

		
		
		int nChildCount = pHelperScript->GetInt(3);

		// 인터페이스 개수 12개 초과는... 문제가 생기지...
		assert(nChildCount <= 12);

		if(nChildCount)
		{
			pTalk = XiahArrayIndex::g_HelperScript.GetData(nID);
			strTitle = pTalk->GetString(1);
			g_pUIManager->SetString(WINDOW_SECRET_BASIS, secret_basis_window_dummy01, strTitle.data());
			
			int nChildID = pHelperScript->GetInt(2);

			for(int i=0; i < nChildCount; ++i)
			{
				pTalk = XiahArrayIndex::g_HelperScript.GetData(nChildID + i);

				if(pTalk)
				{
					strTitle = pTalk->GetString(0);

					g_pUIManager->SetString(WINDOW_SECRET_BASIS, secret_basis_window_dummy02 + i, strTitle.data() , 9);
				}
			}

			g_MainCharInfo.OpenFrame(WINDOW_SECRET_BASIS);
		}
		else
		{
			return false;
		}
	}
	else
	{
		return false;
	}

	return true;
}


bool Helper::TalkContinue()
{
	//HO_0410_07 상서령 가이드 업데이트
	
	
	if(0 == m_nSelectID)
	{
		g_pUIManager->SetString(WINDOW_HELPER_LIST, helper_window_list_dummy, IDS_TALK_MIDDLE, 5);
		return TalkListShow(0);
	}
	if(99 == m_nSelectID || 105 == m_nSelectID)
	{
		g_pUIManager->SetString(WINDOW_HELPER_SCRIPT, helper_window_script_dummy, IDS_TALK_MIDDLE, 5);
		return TalkListShow(99);
	}
	else
	{
		g_pUIManager->SetString(WINDOW_HELPER_LIST, helper_window_list_dummy, IDS_TALK_MIDDLE, 5);
		sArrayData* pHelperScript = XiahArrayIndex::g_HelperScript.GetData(m_nSelectID);

		if(pHelperScript)
		{
			int nParentID = pHelperScript->GetInt(1);

			return TalkListShow(nParentID);
		}
	}	
	return true;
	/*상서령 업데이트 적용 전 코드
	g_pUIManager->SetString(WINDOW_HELPER_SCRIPT, helper_window_script_dummy, IDS_TALK_MIDDLE, 5);

	if(0 == m_nSelectID)
	{
		return TalkListShow(0);
	}
	else if(19 == m_nSelectID)
	{
		return TalkListShow(19);
	}
	else
	{
		sArrayData* pHelperScript = XiahArrayIndex::g_HelperScript.GetData(m_nSelectID);

		if(pHelperScript)
		{
			int nParentID = pHelperScript->GetInt(1);

			return TalkListShow(nParentID);
		}
	}	

	return true;
	*/
}

void Helper::TalkStop()
{
	//HO_0410_07 상서령 가이드 업데이트
	m_nSelectID = 0;

	g_MainCharInfo.CloseFrame(WINDOW_HELPER_LIST1);
	g_MainCharInfo.CloseFrame(WINDOW_HELPER_LIST);

	//HO_0410_07 상서령 가이드 업데이트 : 상서령 가이드 업데이트 적용 전 코드 탐랑용으로 쓰이기에 남겨둠
   	g_MainCharInfo.CloseFrame(WINDOW_HELPER_LIST2);
	g_MainCharInfo.CloseFrame(WINDOW_HELPER_SCRIPT);
}

//HO_0413_07 퀵 가이드 업데이트
void Helper::TalkBack()
{
	TCHAR szLevel[50]={0,};
	_stprintf( szLevel, IDS_D_GAPJA, g_MainCharInfo.m_wLevel);
	sArrayData* pQuickScript = XiahArrayIndex::g_QuickIndex.GetData(g_MainCharInfo.m_wLevel);
	sString strTemp = _T("");
	
	if(g_MainCharInfo.m_bQuickIndex > 0)
		strTemp = pQuickScript->GetString(--g_MainCharInfo.m_bQuickIndex);
	else
	{
		g_MainCharInfo.m_bQuickIndex = 0;
		strTemp = pQuickScript->GetString(g_MainCharInfo.m_bQuickIndex);
	}

	g_pUIManager->SetString(WINDOW_HELPER_LIST0, helper_window0_small_title_dummy, szLevel);	
	g_pUIManager->SetString(WINDOW_HELPER_LIST0, helper_window0_list_dummy, _T(""));	
	g_pUIManager->SetString(WINDOW_HELPER_LIST0, helper_window0_list_dummy, strTemp.data(), 5);
}

void Helper::TalkNext()
{
	TCHAR szLevel[50]={0,};
	_stprintf( szLevel, IDS_D_GAPJA, g_MainCharInfo.m_wLevel);
	sArrayData* pQuickScript = XiahArrayIndex::g_QuickIndex.GetData(g_MainCharInfo.m_wLevel);
	
	sString strTemp = _T("");
	if(g_MainCharInfo.m_bQuickIndex < 2)
		strTemp = pQuickScript->GetString(++g_MainCharInfo.m_bQuickIndex);
	else
	{
		g_MainCharInfo.m_bQuickIndex = 2;
		strTemp = pQuickScript->GetString(g_MainCharInfo.m_bQuickIndex);
	}

	g_pUIManager->SetString(WINDOW_HELPER_LIST0, helper_window0_small_title_dummy, szLevel);	
	g_pUIManager->SetString(WINDOW_HELPER_LIST0, helper_window0_list_dummy, _T(""));	
	g_pUIManager->SetString(WINDOW_HELPER_LIST0, helper_window0_list_dummy, strTemp.data(), 5);

}

bool Helper::SecretApplication()
{
	if(m_nSelectID == 111)
		SendCS_WR_APPLYSECRET_REQ();
	else if(m_nSelectID == 114)
		SendCS_WR_APPLYDEVIL_REQ();
	else
	{
		SecretCancle();
		return false;
	}

	return true;
}

bool Helper::SecretMove()
{
	if(m_nSelectID == 111)
		SendCS_NV_SECRETADVENTURE_REQ();
	else if(m_nSelectID == 114)
		SendCS_NV_DEVILADVENTURE_REQ();
	else
	{
		SecretCancle();
		return false;
	}

	return true;

	return true;
}

void Helper::SecretCancle()
{
	m_nSelectID = 0;

	g_MainCharInfo.CloseFrame(WINDOW_SECRET_CHECK);
	g_MainCharInfo.CloseFrame(WINDOW_SECRET_INFORMATION);
}