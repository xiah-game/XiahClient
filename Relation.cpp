#include "precompile.h"
#include "resource.h"
#include "AppData.h"
#include "relation.h"
#include "InterfaceDefine.h"
#include "InterfaceHandler.h"
#include "XiahGame_Main.h"
#include "XiahGameObject.h"
#include "XiahGame_Handler_Sender.h"
#include <algorithm>
#include <functional>


//////////////////////////
CRelation::CRelation(void)
//////////////////////////
{
	m_eCurrType = eDAN;
	m_byTotalPage = 0;
	m_byCurrPage = 1;
	m_byCurrIndex = 1;
	m_dwCurrRelation = 0;

	m_pVB = NULL;

	g_pDirect3DDevice->CreateVertexBuffer( 4*sizeof(VT_TLVertex),
										0, D3DFVF_TLVERTEX,
										D3DPOOL_MANAGED, &m_pVB, NULL);

	// dan
	m_dwDanID = 0;
	m_dwDanLeader = 0;

	// clan
	m_dwClanID = 0;
	m_szClanName = _T("");
	m_dwClanLeader = 0;

	m_dwMunpaBattleID = 0;
	m_bBattleStatus = 0;

	m_vDan.reserve(6);
	m_vShip.reserve(50);
	m_vClan.reserve(50);
	m_vWhisper.reserve(5);

	m_dwMunpaWarTime = m_dwMunpaWarEnemy = m_dwDonateMoney = 0;
	m_bStealStone = 0;

	m_nDanType	= 0;

	m_byExpDivision = m_byFEDivision = 1;
}

///////////////////////////
CRelation::~CRelation(void)
///////////////////////////
{
	if( m_pVB)
	{
		m_pVB->Release();
		m_pVB = NULL;
	}

	ClearDan();
	ClearShip();
	ClearClan();
	ClearWhisper();	


	std::map<int, sMunpaWarDay*>::iterator iter = m_mMunpaWarDayList.begin();

	for(; iter != m_mMunpaWarDayList.end(); ++iter)
	{
		sMunpaWarDay *pInfo = iter->second;
		delete pInfo, pInfo = NULL;
	} // for(; iter != g_MainCharInfo.m_pRelation->m_mMunpaWarDayList.end(); ++iter)

	m_mMunpaWarDayList.clear();
}

void CRelation::DrawCurrSelectedRelation()
{
	// 단 경험치 분배
	if((g_pUIManager->IsShow(WINDOW_DAN) || g_pUIManager->IsShow(WINDOW_DAN_NEW)) && m_byCurrIndex)
	{
		g_pDirect3DDevice->SetRenderState( D3DRS_ZENABLE, FALSE);
		g_pDirect3DDevice->SetRenderState( D3DRS_FOGENABLE, FALSE);

		g_pDirect3DDevice->SetRenderState( D3DRS_ALPHABLENDENABLE, TRUE);
		g_pDirect3DDevice->SetRenderState( D3DRS_DIFFUSEMATERIALSOURCE, D3DMCS_COLOR1);
		g_pDirect3DDevice->SetRenderState( D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
		g_pDirect3DDevice->SetRenderState( D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
		
		g_pDirect3DDevice->SetSamplerState( 0,  D3DSAMP_ADDRESSU , D3DTADDRESS_CLAMP);
		g_pDirect3DDevice->SetSamplerState( 0,  D3DSAMP_ADDRESSV , D3DTADDRESS_CLAMP);

		g_Device.SetTexture(0, NULL);
		//g_pDirect3DDevice->SetTexture( 0, NULL);
		g_Device.SetStreamSource( m_pVB, sizeof(VT_TLVertex));
		g_Device.SetFVF(D3DFVF_TLVERTEX);
		//g_pDirect3DDevice->SetFVF( D3DFVF_TLVERTEX);
		g_pDirect3DDevice->DrawPrimitive( D3DPT_TRIANGLESTRIP, 0, 2);	
	}

	if(g_pUIManager->IsShow(WINDOW_MUNPA)  && m_byCurrIndex)
	{
		g_pDirect3DDevice->SetRenderState( D3DRS_ZENABLE, FALSE);
		g_pDirect3DDevice->SetRenderState( D3DRS_FOGENABLE, FALSE);

		g_pDirect3DDevice->SetRenderState( D3DRS_ALPHABLENDENABLE, TRUE);
		g_pDirect3DDevice->SetRenderState( D3DRS_DIFFUSEMATERIALSOURCE, D3DMCS_COLOR1);
		g_pDirect3DDevice->SetRenderState( D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
		g_pDirect3DDevice->SetRenderState( D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
		
		g_pDirect3DDevice->SetSamplerState( 0,  D3DSAMP_ADDRESSU , D3DTADDRESS_CLAMP);
		g_pDirect3DDevice->SetSamplerState( 0,  D3DSAMP_ADDRESSV , D3DTADDRESS_CLAMP);

		g_Device.SetTexture(0, NULL);
		//g_pDirect3DDevice->SetTexture( 0, NULL);
		g_Device.SetStreamSource( m_pVB, sizeof(VT_TLVertex));
		g_Device.SetFVF(D3DFVF_TLVERTEX);
		//g_pDirect3DDevice->SetFVF( D3DFVF_TLVERTEX);
		g_pDirect3DDevice->DrawPrimitive( D3DPT_TRIANGLESTRIP, 0, 2);	
	}
}

/**
 *
 */
void CRelation::MakeVB()
{
	if( !m_byCurrIndex)
		return;

	sRect rtRect;

	// 단 경험치 분배
	if(g_pUIManager->IsShow(WINDOW_DAN) || g_pUIManager->IsShow(WINDOW_DAN_NEW))
	{
		g_pUIManager->GetRegionData(WINDOW_DAN, dan_window_list_dummy_04 + m_byCurrIndex - 1, rtRect);

		RECT rtTemp;

		g_pUIManager->GetRegionData(WINDOW_DAN, dan_window_list_dummy_14 + m_byCurrIndex - 1, rtTemp);

		rtRect.right = rtTemp.right;
		rtRect.bottom = rtTemp.bottom;
	}
	else if(g_pUIManager->IsShow(WINDOW_MUNPA))
	{
		g_pUIManager->GetRegionData(WINDOW_MUNPA, munpa_window_list_dummy_01 + m_byCurrIndex - 1, rtRect);

		RECT rtTemp;

		g_pUIManager->GetRegionData(WINDOW_MUNPA, munpa_window_icon_01 + m_byCurrIndex - 1, rtTemp);

		rtRect.right  = rtTemp.right;
		rtRect.bottom = rtTemp.bottom;
	}
	else
	{
		DBG_LogFile( _T("CRelation::MakeVB()  FF"));
	}


	D3DCOLOR d3dcolor = 0x55999999;

	VT_TLVertex Vertex[4];

	Vertex[ 0].pos = Vector4( rtRect.left, rtRect.top, 0, 1);
	Vertex[ 1].pos = Vector4( rtRect.right, rtRect.top, 0, 1);
	Vertex[ 2].pos = Vector4( rtRect.left, rtRect.bottom, 0, 1);
	Vertex[ 3].pos = Vector4( rtRect.right, rtRect.bottom, 0, 1);

	Vertex[ 0].diffuse = d3dcolor;
	Vertex[ 1].diffuse = d3dcolor;
	Vertex[ 2].diffuse = d3dcolor;
	Vertex[ 3].diffuse = d3dcolor;

	Vertex[ 0].tex = Vector2( 0, 0);
	Vertex[ 1].tex = Vector2( 1, 0);
	Vertex[ 2].tex = Vector2( 0, 1);
	Vertex[ 3].tex = Vector2( 1, 1);

	VOID* pVertices;
	if( !FAILED( m_pVB->Lock( 0, sizeof(Vertex), (void**)&pVertices, 0 )))
	{
		memcpy( pVertices, Vertex, sizeof(Vertex) );
		m_pVB->Unlock();
	}
	else
	{
		DBG_LogFile( _T("CRelation::MakeVB() fail"));
	}
}

void CRelation::CheckRelationIndexSelected()
{
	// 단 경험치 분배
	if(g_pUIManager->IsShow(WINDOW_DAN) || g_pUIManager->IsShow(WINDOW_DAN_NEW))
	{
		for( int i=0; i < 5; ++i)
		{
			if( g_pUIManager->IsMouseOn(WINDOW_DAN, dan_window_list_dummy_04 + i)
				|| g_pUIManager->IsMouseOn(WINDOW_DAN, dan_window_list_dummy_09 + i)
				|| g_pUIManager->IsMouseOn(WINDOW_DAN, dan_window_list_dummy_14 + i))
			{
				SetCurrRelation( i + 1);
				return;
			}
		}

		if(Am_I_LeaderInDan())
		{
			BYTE byExpDivision = m_byExpDivision;
			BYTE byFEDivision  = m_byFEDivision;
			bool bChang = false;

			// 경험치
			// 공동 분배
			if(g_pUIManager->IsMouseOn(WINDOW_DAN_NEW, window_dan_new_select_button1))
			{
				if(byExpDivision == 0)
				{
					bChang = true;
					byExpDivision = 1;
				}
			}

			// 개인 분배
			if(g_pUIManager->IsMouseOn(WINDOW_DAN_NEW, window_dan_new_select_button2))
			{
				if(byExpDivision == 1)
				{
					bChang = true;
					byExpDivision = 0;
				}
			}

			// 오행 경험치
			// 공동 분배
			if(g_pUIManager->IsMouseOn(WINDOW_DAN_NEW, window_dan_new_select_button3))
			{
				if(byFEDivision == 0)
				{
					bChang = true;
					byFEDivision = 1;
				}
			}

			// 개인 분배
			if(g_pUIManager->IsMouseOn(WINDOW_DAN_NEW, window_dan_new_select_button4))
			{
				if(byFEDivision == 1)
				{
					bChang = true;
					byFEDivision = 0;
				}
			}

			if(bChang)
				SendCS_IF_PARTYSHARE_REQ(byExpDivision,  byFEDivision);
		}
	}
	else if(g_pUIManager->IsShow(WINDOW_MUNPA))
	{
		for( int i=0; i < 8; ++i)
		{
//			if( g_pUIManager->IsMouseOn(WINDOW_MUNPA, found_window_contents_dummy_001))
//			{
//				if( XiahInput::g_bLButtonDown)
//				{
//					g_pUIManager->SetString(WINDOW_CONNECTION_INFO, info_window_title_dummy, IDS_CLAN_DETAIL_INFO);
//					g_pUIManager->SetString(WINDOW_CONNECTION_INFO, info_window_contents_dummy_01, IDS_RANK);
//					g_pUIManager->SetString(WINDOW_CONNECTION_INFO, info_window_contents_dummy_02, IDS_POINT);
//					g_pUIManager->SetString(WINDOW_CONNECTION_INFO, info_window_contents_dummy_03, IDS_FAME);
//
//					g_MainCharInfo.OpenFrame( WINDOW_CONNECTION_INFO);
//
//					g_pUIManager->Show(WINDOW_CONNECTION_INFO, info_window_button);
//					g_pUIManager->Hide(WINDOW_CONNECTION_INFO, info_window_button_01);
//					g_pUIManager->Hide(WINDOW_CONNECTION_INFO, info_window_button_02);
//
//					g_pUIManager->SetPosition(WINDOW_CONNECTION_INFO, 545, XiahInput::g_ptMouse.y);
//
//					g_pUIManager->SetString(WINDOW_CONNECTION_INFO, info_window_contents_dummy_04, _T("?"));
//					g_pUIManager->SetString(WINDOW_CONNECTION_INFO, info_window_contents_dummy_05, _T("?"));
//					g_pUIManager->SetString(WINDOW_CONNECTION_INFO, info_window_contents_dummy_06, _T("?"));
//
//					SendCS_RL_MUNPAINFO_REQ( GetClanID());
//					return;
//				}
//			}

			if(g_pUIManager->IsMouseOn(WINDOW_MUNPA, munpa_window_list_dummy_01 + i)
				|| g_pUIManager->IsMouseOn(WINDOW_MUNPA, munpa_window_list_dummy_09 + i)
				|| g_pUIManager->IsMouseOn(WINDOW_MUNPA, munpa_window_list_dummy_001 + i)
				|| g_pUIManager->IsMouseOn(WINDOW_MUNPA, munpa_window_icon_01 + i))
			{
				if( XiahInput::g_bLButtonDown)
				{
					SetCurrRelation( i + 1);
					return;
				}
			}
		}
	}

}

////////////////////////////////////////
void CRelation::RefreshRelationContent()
////////////////////////////////////////
{
	switch( m_eCurrType)
	{
	case eDAN:
		RefreshDanContent();
		break;
	case eShip:
		RefreshShipContent();
		break;
	case eClan:
		RefreshClanContent();
		break;
	}
}

void CRelation::SetCurrType( eRELATION_TYPE eType, BYTE byCurrPage)
{ 
	switch( eType)
	{
	case eDAN:
		{
			CloseAllWindow();

			// 단 경험치 분배
			if(g_MainCharInfo.m_pRelation->Am_I_InDan())
			{
				if( g_MainCharInfo.m_pRelation->Am_I_LeaderInDan())
				{
					g_pUIManager->SetString(WINDOW_DAN_NEW, window_dan_new_2button_01, IDS_JEMYUNG);
					g_pUIManager->SetString(WINDOW_DAN_NEW, window_dan_new_2button_02, IDS_TALTE);
					g_pUIManager->SetString(WINDOW_DAN_NEW, window_dan_new_1button, IDS_DANCOMMIT); //HT_0423 : 단주 위임
					g_pUIManager->Show(WINDOW_DAN_NEW, window_dan_new_1button);
					g_pUIManager->Show(WINDOW_DAN_NEW, window_dan_new_2button_01);
					g_pUIManager->Show(WINDOW_DAN_NEW, window_dan_new_2button_02);
				}
				else
				{
					g_pUIManager->Hide(WINDOW_DAN_NEW, window_dan_new_2button_01);
					g_pUIManager->Hide(WINDOW_DAN_NEW, window_dan_new_2button_02);
					g_pUIManager->Show(WINDOW_DAN_NEW, window_dan_new_1button);
					g_pUIManager->SetString(WINDOW_DAN_NEW, window_dan_new_1button, IDS_TALTE);
				}

				g_pUIManager->Hide(WINDOW_DAN_NEW, window_dan_new_button_back);
				g_pUIManager->Hide(WINDOW_DAN_NEW, window_dan_new_button_front);

				g_MainCharInfo.OpenFrame(WINDOW_DAN_NEW);

			}
			else
			{
				g_pUIManager->Hide(WINDOW_DAN, dan_window_2button_01);
				g_pUIManager->Hide(WINDOW_DAN, dan_window_2button_02);
				g_pUIManager->Show(WINDOW_DAN, dan_window_1button);

				g_pUIManager->SetString(WINDOW_DAN, dan_window_1button, IDS_TALTE);

				g_pUIManager->Hide(WINDOW_DAN, dan_window_list_button_left);
				g_pUIManager->Hide(WINDOW_DAN, dan_window_list_button_right);

				g_MainCharInfo.OpenFrame( WINDOW_DAN);
			}

			g_pUIManager->SetData(WINDOW_DAN, dan_window_3button_01, CURRENT_INDEX, 2);
			g_pUIManager->SetData(WINDOW_DAN, dan_window_3button_02, CURRENT_INDEX, -1);
		}
		break;
	case eShip:
		{
			//g_pUIManager->Hide(WINDOW_DAN, dan_window_1button);
			//g_pUIManager->SetString(WINDOW_DAN, dan_window_2button_01, IDS_JUNSUGU);
			//g_pUIManager->SetString(WINDOW_DAN, dan_window_2button_02, IDS_CUTSHIP);
			g_pUIManager->SetString(WINDOW_DAN, dan_window_1button, IDS_CUTSHIP);
			g_pUIManager->Show(WINDOW_DAN, dan_window_1button);

			g_pUIManager->Hide(WINDOW_DAN, dan_window_2button_01);
			g_pUIManager->Hide(WINDOW_DAN, dan_window_2button_02);

			//g_pUIManager->Show(WINDOW_DAN, dan_window_2button_01);
			//g_pUIManager->Show(WINDOW_DAN, dan_window_2button_02);

			g_pUIManager->SetData(WINDOW_DAN, dan_window_3button_01, CURRENT_INDEX, -1);
			g_pUIManager->SetData(WINDOW_DAN, dan_window_3button_02, CURRENT_INDEX, 2);

			g_pUIManager->Show(WINDOW_DAN, dan_window_list_button_left);
			g_pUIManager->Show(WINDOW_DAN, dan_window_list_button_right);

			CloseAllWindow();
			g_MainCharInfo.OpenFrame( WINDOW_DAN);
		}
		break;
	case eClan:
		{
			CloseAllWindow();
			if( g_MainCharInfo.m_pRelation->Am_I_InClan())
			{
				g_MainCharInfo.OpenFrame( WINDOW_MUNPA);
				
				if(g_MainCharInfo.m_pRelation->Am_I_LeaderInClan())
				{
					g_pUIManager->SetString(WINDOW_MUNPA, munpa_window_button_03, IDS_PAMUN);
					g_pUIManager->SetString(WINDOW_MUNPA, munpa_window_button_04, IDS_CLOSE_CLAN);
					// 门主：显示全部按钮
					g_pUIManager->Show(WINDOW_MUNPA, munpa_window_button_01);
					g_pUIManager->Show(WINDOW_MUNPA, munpa_window_button_02);
					g_pUIManager->Show(WINDOW_MUNPA, munpa_window_button_04);
				}
				else
				{
					if(g_MainCharInfo.m_pRelation->Am_I_2stLeaderInClan())
					{
						g_pUIManager->SetString(WINDOW_MUNPA, munpa_window_button_03, IDS_TALTE);
						g_pUIManager->SetString(WINDOW_MUNPA, munpa_window_button_04, IDS_PAMUN);
						// 副门主：显示全部按钮
						g_pUIManager->Show(WINDOW_MUNPA, munpa_window_button_01);
						g_pUIManager->Show(WINDOW_MUNPA, munpa_window_button_02);
						g_pUIManager->Show(WINDOW_MUNPA, munpa_window_button_04);
					} // if(g_MainCharInfo.m_pRelation->Am_I_2stLeaderInClan())
					else
					{
                        g_pUIManager->SetString(WINDOW_MUNPA, munpa_window_button_03, IDS_TALTE);
						// 普通成员：隐藏公告、授予称号、关闭门派按钮，只保留退出
						g_pUIManager->Hide(WINDOW_MUNPA, munpa_window_button_01);
						g_pUIManager->Hide(WINDOW_MUNPA, munpa_window_button_02);
						g_pUIManager->Hide(WINDOW_MUNPA, munpa_window_button_04);
					}
				}
			}
			else
				g_MainCharInfo.OpenFrame( WINDOW_MUNPA_FOUND);
		}		
		break;
	}
	
	m_eCurrType = eType;
	SetTotalPage();
	SetCurrPage( byCurrPage);
	RefreshRelationContent();	
}

//////////////////////////////
void CRelation::SetTotalPage()
//////////////////////////////
{
	BYTE byTotalSize = 0;

	switch( m_eCurrType)
	{
	case eDAN:
		byTotalSize = m_vDan.size();
		break;
	case eShip:
		byTotalSize = m_vShip.size() - 1;
		m_byTotalPage = byTotalSize / 5 + 1;
		break;
	case eClan:
		byTotalSize = m_vClan.size() - 1;
		m_byTotalPage = byTotalSize / 8 + 1;
		break;
	}
}

void CRelation::SetCurrPage( BYTE byPage)
{ 
	if( byPage < 1)
		return;
	else if( byPage > m_byTotalPage)
		return;

	switch( m_eCurrType)
	{
	case eDAN:
		{
			// 단 경험치 분배
			g_pUIManager->SetString(WINDOW_DAN, dan_window_list_button_dummy, _T(""));
			g_pUIManager->SetString(WINDOW_DAN_NEW, window_dan_new_button_dummy, _T(""));
		}
		break;
	case eShip:
		{
			//BYTE m_byTempRelation = MAX_NUM_IN_FRAME * byPage + m_byCurrIndex - 1;

			m_byCurrPage = byPage;

			TCHAR content[32] = {0,};
			_stprintf( content, _T("%d / %d"), m_byCurrPage, m_byTotalPage);
			g_pUIManager->SetString(WINDOW_DAN, dan_window_list_button_dummy, content);
		}
		break;
	case eClan:
		{
			//BYTE m_byTempRelation = MAX_NUM_IN_FRAME_CLAN * byPage + m_byCurrIndex - 1;

			m_byCurrPage = byPage;

			TCHAR content[32] = {0,};
			_stprintf( content, _T("%d / %d"), m_byCurrPage, m_byTotalPage);
			g_pUIManager->SetString(WINDOW_MUNPA,munpa_window_list_button_dummy, content);
		}
		break;
	}

	SetCurrRelation( 0, TRUE);
}

//////////////////////////////////////////////////////////
void CRelation::SetCurrRelation( BYTE byIndex, BOOL bFlag)
//////////////////////////////////////////////////////////
{
	DWORD dwPreSelectedRelation = GetCurrRelation();

	m_byCurrIndex = byIndex;

	m_dwCurrRelation = FindRelationIDByIndex( m_byCurrPage, byIndex);

	if( !m_dwCurrRelation)
		return;
		
	MakeVB();

	if( bFlag)
		return;

	if(g_pUIManager->IsShow(WINDOW_NAME_CONFER))
	{
		DWORD id = GetCurrRelation();
		sClanWonInfo* pInfo = g_MainCharInfo.m_pRelation->FindClanInfoByID( id);
		if( pInfo)
		{
			TCHAR szContent[100];
			_stprintf( szContent, IDS_D_INSERT_HOCHING, (LPCTSTR)pInfo->m_szCharName);

			g_pUIManager->SetString(WINDOW_NAME_CONFER, name_wiandow_contents_dummy_01, szContent);
			g_pUIManager->SetFocus(WINDOW_NAME_CONFER);
			g_pUIManager->SetFocus(WINDOW_NAME_CONFER, name_window_edit);
		}
	}

	if( dwPreSelectedRelation == m_dwCurrRelation)
	{
		switch( m_eCurrType)
		{
		case eDAN:
		case eShip:
			break;
		case eClan:
			{
				sClanWonInfo* pClanInfo = FindClanInfoByID( m_dwCurrRelation);

 				if( pClanInfo->m_wCharLev)
				{
					g_pUIManager->SetString(WINDOW_CONNECTION_INFO, info_window_title_dummy, IDS_CLAN_INFO);
					g_pUIManager->SetString(WINDOW_CONNECTION_INFO, info_window_contents_dummy_01, IDS_NAME);
					g_pUIManager->SetString(WINDOW_CONNECTION_INFO, info_window_contents_dummy_02, IDS_LEVEL);
					g_pUIManager->SetString(WINDOW_CONNECTION_INFO, info_window_contents_dummy_03, IDS_CLASS);

					g_MainCharInfo.OpenFrame( WINDOW_CONNECTION_INFO);

					g_pUIManager->Hide(WINDOW_CONNECTION_INFO, info_window_button);
					g_pUIManager->Show(WINDOW_CONNECTION_INFO, info_window_button_01);
					g_pUIManager->Show(WINDOW_CONNECTION_INFO, info_window_button_02);
					
					g_pUIManager->SetPosition(WINDOW_CONNECTION_INFO, 545, XiahInput::g_ptMouse.y);

					g_pUIManager->SetString(WINDOW_CONNECTION_INFO, info_window_contents_dummy_04, (LPCTSTR)pClanInfo->m_szCharName);

					//150갑자 초과시 
					if(pClanInfo->m_wCharLev < 150)
					{
                        g_pUIManager->SetString(WINDOW_CONNECTION_INFO, info_window_contents_dummy_05, pClanInfo->m_wCharLev);
					}
					else
					{
						g_pUIManager->SetString(WINDOW_CONNECTION_INFO, info_window_contents_dummy_05, IDS_BESTLEVEL, 9);
					}

					LPCTSTR lpStrTemp = NULL;
					switch(pClanInfo->m_bCharType)
					{
					case 1:	// 검영
						lpStrTemp = IDS_GUMYONG; break;
					case 2:	// 연랑
						lpStrTemp = IDS_YUNRANG; break;
					case 3:	// 무투
						lpStrTemp = IDS_MUTU; break;
					case 4:	// 야차
						lpStrTemp = IDS_YACHA; break;
					default:	// 댄轎
						lpStrTemp = START_ERROR0; break;
					} // switch(pClanInfo->m_bCharType)

					g_pUIManager->SetString(WINDOW_CONNECTION_INFO, info_window_contents_dummy_06, lpStrTemp);
				}
				else
				{
					SendCS_RL_MUNWONINFO_REQ( m_dwCurrRelation);
				}
			}
			break;
		}
	} // if( dwPreSelectedRelation == m_dwCurrRelation)

	if( g_MainCharInfo.GetCurrSendChatType() == CT_WHISPER)
		g_pUIManager->SetString(MAIN_CHAT, chat_name_edit, (LPCTSTR)FindRelationNameByID( m_dwCurrRelation));
}

BYTE CRelation::GetCurrPage()
{
	// 단 경험치 분배
	if(!(g_pUIManager->IsShow(WINDOW_DAN_NEW) || g_pUIManager->IsShow(WINDOW_DAN) || g_pUIManager->IsShow(WINDOW_MUNPA)))
		return 0;

	return m_byCurrPage;
}

BYTE CRelation::GetCurrIndex()
{
	// 단 경험치 분배
	if(!(g_pUIManager->IsShow(WINDOW_DAN) || g_pUIManager->IsShow(WINDOW_DAN_NEW)))
		return 0;

	return m_byCurrIndex;
}

DWORD CRelation::GetCurrRelation()
{
	// 단 경험치 분배
	if(g_pUIManager->IsShow(WINDOW_DAN_NEW) || g_pUIManager->IsShow(WINDOW_DAN) || g_pUIManager->IsShow(WINDOW_MUNPA))
		return m_dwCurrRelation;
	else
		return 0;
}

////////////////////////////////////////////////////////
sString CRelation::FindRelationNameByID( DWORD dwCharID)
////////////////////////////////////////////////////////
{
	switch( m_eCurrType)
	{
	case eDAN:
		{
			sDanInfo* pInfo = FindDanInfoByID( dwCharID);
			if( pInfo)
				return pInfo->m_szNickName;
		}
		break;
	case eShip:
		{
			sShipInfo* pInfo = FindShipInfoByID( dwCharID);
			if( pInfo)
				return pInfo->m_szNickName;
		}
		break;
	case eClan:
		{
			sClanWonInfo* pInfo = FindClanInfoByID( dwCharID);
			if( pInfo)
				return pInfo->m_szCharName;
		}
		break;
	}
	
	return sString( _T(""));
}

//////////////////////////////////////////////////////
DWORD CRelation::FindRelationIDByName( LPCTSTR szName)
//////////////////////////////////////////////////////
{
	/*
	// 단에서 찾기
	sDanInfo* pDanInfo = FindDanInfoByName( szName);
	if( pDanInfo)
		return pDanInfo->m_dwCharID;

	// 인연에서 찾기
	sShipInfo* pShipInfo = FindShipInfoByName( szName);
	if( pShipInfo)
		return pShipInfo->m_dwCharID;

	// 문파에서 찾기
	sClanWonInfo* pClanInfo = FindClanInfoByName( szName);
	if( pClanInfo)
		return pClanInfo->m_dwCharID;
*/
	// whisper목록에서 찾기
	int nSize = m_vWhisper.size();

	for( int i=0 ;i < nSize; i++)
	{
		if( _tcscmp( (LPCTSTR)m_vWhisper[i]->m_szNickName, szName) == 0)
			return m_vWhisper[i]->m_dwCharID;
	}

	return 0;
}

//////////////////////////////////////////////////////////////////////
DWORD CRelation::FindRelationIDByIndex( BYTE byPage, BYTE byIndex)
//////////////////////////////////////////////////////////////////////
{
	int nRelationIndex;

	switch( m_eCurrType)
	{
	case eDAN:
		{
			nRelationIndex =  MAX_NUM_IN_FRAME * (byPage-1) + byIndex-1;

			sDanInfo* pInfo = FindDanInfoByIndex( nRelationIndex);
			if( pInfo)
				return pInfo->m_dwCharID;
		}
		break;
	case eShip:
		{
			nRelationIndex =  MAX_NUM_IN_FRAME * (byPage-1) + byIndex-1;

			sShipInfo* pInfo = FindShipInfoByIndex( nRelationIndex);
			if( pInfo)
				return pInfo->m_dwCharID;
		}
		break;
	case eClan:
		{
			nRelationIndex =  MAX_NUM_IN_FRAME_CLAN * (byPage-1) + byIndex-1;

			sClanWonInfo* pInfo = FindClanInfoByIndex( nRelationIndex);
			if( pInfo)
				return pInfo->m_dwCharID;
		}
		break;
	}
	
	return 0;
}

void CRelation::InsertWhisperInfo( DWORD dwCharID, sString szNickName)
{
	int nSize = m_vWhisper.size();

	for( int i=0 ;i < nSize; i++)
	{
		if( m_vWhisper[i]->m_dwCharID == dwCharID)
			return;
	}

	if( m_vWhisper.size() > 5)
		m_vWhisper.pop_back();	// 왜 pop_front가 없는거야 쿵!

	sWhisperInfo* pInfo = new sWhisperInfo;
	pInfo->m_dwCharID = dwCharID;
	pInfo->m_szNickName = szNickName;

	m_vWhisper.push_back( pInfo);
}

void CRelation::ClearWhisper()
{
	int nSize = m_vWhisper.size();

	for( int i=0 ;i < nSize; i++)
	{
		delete m_vWhisper[i];
		m_vWhisper[i]  = NULL;
	}

	m_vWhisper.clear();
}


#include "Relation_Dan.cpp"
#include "Relation_Ship.cpp"
#include "Relation_Clan.cpp"
//#include "Relation_MunpaWar.cpp"
