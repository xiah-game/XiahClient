#include "precompile.h"
#include "resource.h"
#include "AppData.h"
#include "Quest.h"
#include "InterfaceDefine.h"
#include "CharacterInfo.h"
#include "XiahArrayIndex.h"

CQuest::CQuest()
{
	m_wCurrQuestIndex = 0;
	m_byCurrContent = eContent;

	m_pVB = NULL;

	g_pDirect3DDevice->CreateVertexBuffer( 4*sizeof(VT_TLVertex),
										0, D3DFVF_TLVERTEX,
										D3DPOOL_MANAGED, &m_pVB, NULL);


	RECT rtRect;

	g_pUIManager->GetRegionData(WINDOW_QUEST_01, quest_window_01_dummy_01, rtRect);

	for( int i=0; i < QUEST_SIZE; ++i)
	{
		m_rtRegion[i].left = rtRect.left;
		m_rtRegion[i].right = rtRect.right;
		m_rtRegion[i].top = rtRect.top + QUEST_VERTICAL_DISTANCE*i;
		m_rtRegion[i].bottom = rtRect.bottom + QUEST_VERTICAL_DISTANCE*i;
		m_text2D[i].SetParentRect( &m_rtRegion[i]);

		m_rtRegion_2[i].left = rtRect.right + 60;
		m_rtRegion_2[i].right = rtRect.right + 70;
		m_rtRegion_2[i].top = rtRect.top + QUEST_VERTICAL_DISTANCE*i;
		m_rtRegion_2[i].bottom = rtRect.bottom + QUEST_VERTICAL_DISTANCE*i;
		m_text2D_2[i].SetParentRect( &m_rtRegion_2[i]);

		m_rtRegion_3[i].left = rtRect.right + 70;
		m_rtRegion_3[i].right = rtRect.right + 80;
		m_rtRegion_3[i].top = rtRect.top + QUEST_VERTICAL_DISTANCE*i;
		m_rtRegion_3[i].bottom = rtRect.bottom + QUEST_VERTICAL_DISTANCE*i;
		m_text2D_3[i].SetParentRect( &m_rtRegion_3[i]);
	}

	g_pUIManager->GetRegionData(WINDOW_QUEST_01, quest_window_01_dummy_03, rtRect);

	for( i=0; i < CONDITION_SIZE; ++i)
	{
		m_rtConditonRegion[i].left = rtRect.left;
		m_rtConditonRegion[i].right = rtRect.right;
		m_rtConditonRegion[i].top = rtRect.top + QUEST_VERTICAL_DISTANCE*i;
		m_rtConditonRegion[i].bottom = rtRect.bottom + QUEST_VERTICAL_DISTANCE*i;
		m_ConditionText2D[i].SetParentRect( &m_rtConditonRegion[i]);

		m_rtConditonRegion_2[i].left = rtRect.right + 70 ;
		m_rtConditonRegion_2[i].right = rtRect.right + 80;
		m_rtConditonRegion_2[i].top = rtRect.top + QUEST_VERTICAL_DISTANCE*i;
		m_rtConditonRegion_2[i].bottom = rtRect.bottom + QUEST_VERTICAL_DISTANCE*i;
		m_ConditionText2D_2[i].SetParentRect( &m_rtConditonRegion_2[i]);
	}
}

CQuest::~CQuest()
{
	int nSize = m_vTotalQuest.size();

	for(register int i=0; i < nSize; ++i)
	{
		delete m_vTotalQuest[i];
		m_vTotalQuest[i]  = NULL;
	}

	m_vTotalQuest.clear();

	if( m_pVB )
	{
		m_pVB->Release();
		m_pVB = NULL;
	}

	//HT_0824 : 퀘스트 도우미 추가
	nSize = m_sQuestHelp.m_vQuestCoordinate.size();

	for(register int i=0; i < nSize; ++i)
	{
		delete m_sQuestHelp.m_vQuestCoordinate[i];
		m_sQuestHelp.m_vQuestCoordinate[i]  = NULL;
	}

	m_sQuestHelp.m_vQuestCoordinate.clear();
}


////////////////////////
void CQuest::MakeVB( sRect rtRect)
////////////////////////
{
	D3DCOLOR d3dcolor = 0x55999999;

	VT_TLVertex Vertex[4];

	rtRect.right += 100;

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
}

//////////////////////////////////////////
void CQuest::DrawCurrSelectedQuest()
//////////////////////////////////////////
{
	if(g_pUIManager->IsShow(WINDOW_QUEST_01))
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

sQuestInfo* CQuest::FindQuest( DWORD dwQuestID)
{
	int nSize = m_vTotalQuest.size();

	for(register int i=0; i < nSize; ++i)
	{
		if( m_vTotalQuest[i]->m_dwID == dwQuestID)
			return m_vTotalQuest[i];
	}

	return NULL;
}

void CQuest::InsertQuest( DWORD dwQuestID, QUESTSTATUS_TYPE eType, sString szQuestName, sString szDescription, BYTE bRepeat, BYTE bProcessNum, BYTE bResultNum, BYTE bProcessType)
{
	//sQuestInfo* pTemp = FindQuest( dwQuestID);
	//if( !pTemp)
	//{
		if(eType == eDeleted || eType == eSuccessDel)
			return;

		sQuestInfo* pQuest = new sQuestInfo();

		pQuest->m_dwID = dwQuestID;
		pQuest->m_eStatus = eType;
		pQuest->m_szName = szQuestName;
		pQuest->m_szContent = szDescription;
		pQuest->m_bRepeat = bRepeat;
		pQuest->m_bConditionNum = bProcessNum;
		pQuest->m_bRewardNum = bResultNum;
		pQuest->m_bProcessType = bProcessType;

		m_vTotalQuest.push_back( pQuest);
		//m_vQuest.push_back( pQuest);
		UpdateQuest();	//HT_0914 : 기연창 및 낭 아이템 개선 사항

		InsertQuestDesc( dwQuestID);

		g_pUIManager->SetData(WINDOW_QUEST_01, quest_window_01_scrollbar1, SCROLL_TOTAL, m_vQuest.size());	
	//}
}

void CQuest::InsertQuestDesc( DWORD dwQuestID)
{
	sQuestInfo* pQuest = FindQuest( dwQuestID);
	if( pQuest)
	{
		sArrayData* pQuestDesc = XiahArrayIndex::g_QuestDesc.GetData( dwQuestID);

		if( pQuestDesc)
			pQuest->m_szContent = pQuestDesc->GetString( 0);
	}
}

void CQuest::InsertQuestCondition( DWORD dwQuestID, BYTE bSeq, sString szProcessName, DWORD dwProcessCurrAmount, DWORD dwProcessTotalAmount)
{
	sQuestInfo* pQuest = FindQuest( dwQuestID);
	if( pQuest)
	{
		pQuest->m_szCondition[bSeq] = szProcessName;
		pQuest->m_dwConditionCurrAmount[bSeq] = dwProcessCurrAmount;
		pQuest->m_dwConditionTotalAmount[bSeq] = dwProcessTotalAmount;
	}
}

void CQuest::InsertQuestReward( DWORD dwQuestID, BYTE bSeq, sString szResultName, DWORD dwResultAmount)
{
	sQuestInfo* pQuest = FindQuest( dwQuestID);
	if( pQuest)
	{
		pQuest->m_szReward[bSeq] = szResultName;
		pQuest->m_dwRewardAmount[bSeq] = dwResultAmount;
	}
}

void CQuest::Show()
{
	if( !g_pUIManager->IsShow(WINDOW_QUEST_01))
		return;
	
	for( int i=0; i < QUEST_SIZE; ++i)
	{
		m_text2D[i].Render();
		m_text2D_2[i].Render();
		m_text2D_3[i].Render();
	}

	for( i=0; i < CONDITION_SIZE; ++i)
	{
		m_ConditionText2D[i].Render();
		m_ConditionText2D_2[i].Render();
	}

	DrawCurrSelectedQuest();
}

void CQuest::CheckCurrQuestIndex()
{
	if(g_pUIManager->IsShow(WINDOW_QUEST_01) && XiahInput::g_bLButtonDown)
	{
		for(register int i=0; i < QUEST_SIZE-1; ++i)
		{
			if( m_rtRegion[i].PtInRect( XiahInput::g_ptMouse))
			{
				SetCurrIndex( i, &m_rtRegion[i]);
			}
		}
	}
}

void CQuest::SetCurrIndex( BYTE byIndex, sRect* rtRect)
{
	int nCurrLine = g_pUIManager->GetData(WINDOW_QUEST_01, quest_window_01_scrollbar1, GET_SCROLL_CURRENT);
	m_wCurrQuestIndex = byIndex + nCurrLine;

	if( m_wCurrQuestIndex >= m_vQuest.size())
		return;

	MakeVB( *rtRect);
	RefreshQuestIndex();
	SetCurrContent( m_byCurrContent);
}

void CQuest::SetCurrContent( QUESTCONTENT_TYPE eType)
{
	m_byCurrContent = eType;
	g_pUIManager->SetData(WINDOW_QUEST_01, quest_window_01_scrollbar2, SCROLL_MOVE, 0);
	RefreshQuestContent();
}

// [3/29/2004]
void CQuest::Refresh()
{
	//sRect* rtRect = g_pUIManager->GetFrame( WINDOW_QUEST_01)->GetControl( quest_window_1_dumy_01)->GetRegion_Pointer();
/*
	if(m_wCurrQuestIndex > m_vQuest.size())
		m_wCurrQuestIndex = m_vQuest.size();

	int nCurrLine = g_pUIManager->GetData(WINDOW_QUEST_01, quest_window_01_scrollbar1, GET_SCROLL_CURRENT);

	m_wCurrQuestIndex -= (BYTE)nCurrLine;

	SetCurrIndex( m_wCurrQuestIndex, &m_rtRegion[m_wCurrQuestIndex]);	
*/
	//if(지금 누른 번트 번호 ==0 )
		
	//else if((==1)
	//	UpdateQuest1();	//HT_0914 : 기연창 및 낭 아이템 개선 사항

	sRect rtRect;
	g_pUIManager->GetRegionData(WINDOW_QUEST_01, quest_window_01_dummy_01, rtRect);

	SetCurrIndex( 0, &rtRect);
	//SetCurrIndex( m_wCurrQuestIndex, rtRect);	
}

void CQuest::RefreshQuestIndex()
{
	for(int i=0; i < QUEST_SIZE; ++i)
	{
		m_text2D[i].SetText( 0, 0, _T(" "), GetFont( IDS_DUDUM, 12), D3DCOLOR_XRGB( 0, 255, 0));
		m_text2D_2[i].SetText( 0, 0, _T(" "), GetFont( IDS_DUDUM, 12), D3DCOLOR_XRGB( 0, 255, 0));
		m_text2D_3[i].SetText( 0, 0, _T(" "), GetFont( IDS_DUDUM, 12), D3DCOLOR_XRGB( 0, 255, 0));
	}

	int nTotalLine = m_vQuest.size();

	g_pUIManager->SetData(WINDOW_QUEST_01, quest_window_01_scrollbar1, SCROLL_TOTAL, nTotalLine);

	int nCurrLine = g_pUIManager->GetData(WINDOW_QUEST_01, quest_window_01_scrollbar1, GET_SCROLL_CURRENT);
	int nMaxLine = g_pUIManager->GetData(WINDOW_QUEST_01, quest_window_01_scrollbar1, GET_SCROLL_MAX);
	
	for( i=0; i < nTotalLine; ++i)
	{
		if( nCurrLine <= i)
		{
			int index = i - nCurrLine;
			if( index < nMaxLine)
			{	
				sQuestInfo* pQuest = m_vQuest[ i];

				if( pQuest)
				{
					D3DCOLOR color;			
					TCHAR szRepeat[10];
					TCHAR szStatus[10];
					_stprintf( szRepeat, _T("%d"), pQuest->m_bRepeat);
					
					switch( pQuest->m_eStatus)
					{
					case eNew:
						color = D3DCOLOR_XRGB( 192, 192, 192);
						_tcscpy( szStatus, IDS_QUEST_NEW);
						break;
					case eStart:
						color = D3DCOLOR_XRGB( 255, 255, 0);
						_tcscpy( szStatus, IDS_QUEST_PROGRESS);
						break;
					case ePause:
						color = D3DCOLOR_XRGB( 255, 255, 128);
						_tcscpy( szStatus, IDS_QUEST_STOP_);
						break;
					case eDeleted:
					case eSuccessDel:
						continue;
						break;
					case eSuccess:
						color = D3DCOLOR_XRGB( 10, 255, 10);
						_tcscpy( szStatus, IDS_QUEST_COMPLETE);
						break;
					default:
						color = D3DCOLOR_XRGB( 10, 255, 10);
						_tcscpy( szStatus, _T("XX"));
						break;
					}

					m_text2D[index].SetText( 0, 0, (LPCTSTR)pQuest->m_szName, GetFont( IDS_DUDUM, 12), color);
					m_text2D_2[index].SetText( 0, 0, szRepeat, GetFont( IDS_DUDUM, 12), color);
					m_text2D_3[index].SetText( 0, 0, szStatus, GetFont( IDS_DUDUM, 12), color);
				}
			}
		}
	}
}

void CQuest::RefreshQuestContent()
{
	TCHAR strFame[64]={0,};	
	//BYTE byType;

	//HT_0911 : 프리미엄 퀘스트 수련치 
	//_stprintf( strFame, IDS_QUEST_SP, g_MainCharInfo.m_dwPremiumSP);
	//g_pUIManager->SetString(WINDOW_QUEST_01, quest_window_fame_dumy_01, strFame, 11);

	//_stprintf( strFame, IDS_QUEST_TP, g_MainCharInfo.m_dwPremiumTP);
	//g_pUIManager->SetString(WINDOW_QUEST_01, quest_window_fame_dumy_02, strFame, 11);

	//if( g_MainCharInfo.m_dwFame > 126)//HO_0810_07 퀘스트 방식 변경으로 더이상 사용하지 않는다. 시작
	//{
	//	g_pUIManager->SetString(WINDOW_QUEST_01, quest_window_fame_dumy_01, IDS_FAME, 0);

	//	_stprintf( strFame, IDS_RATE, g_MainCharInfo.m_dwFame - 127);

	//	byType = 0;
	//}
	//else
	//{
	//	g_pUIManager->SetString(WINDOW_QUEST_01, quest_window_fame_dumy_01, VICE_LEVEL, 1);
	//	
	//	_stprintf( strFame, IDS_RATE, 127 - g_MainCharInfo.m_dwFame);

	//	byType = 1;
	//}
	//g_pUIManager->SetString(WINDOW_QUEST_01, quest_window_fame_dumy_02, strFame, byType);//HO_0810_07 퀘스트 방식 변경으로 더이상 사용하지 않는다. 끝
	_stprintf( strFame, IDS_QUEST_ALL);
	g_pUIManager->SetString(WINDOW_QUEST_01, quest_window_01_button_01, strFame);
	_stprintf( strFame, IDS_QUEST_PROGRESS);
	g_pUIManager->SetString(WINDOW_QUEST_01, quest_window_01_button_02, strFame);
	_stprintf( strFame, IDS_QUEST_NEW);
	g_pUIManager->SetString(WINDOW_QUEST_01, quest_window_01_button_03, strFame);
	_stprintf( strFame, IDS_QUEST_COMPLETE);
	g_pUIManager->SetString(WINDOW_QUEST_01, quest_window_01_button_04, strFame);

	if( m_vQuest.size() < 1)
		return;

	if(m_wCurrQuestIndex >= m_vQuest.size())
		return;

	//if( m_wCurrQuestIndex > m_vQuest.size())
	//{
	//	if(m_vQuest.size() > 0)
	//		m_wCurrQuestIndex = 1;

	//	return;
	//}

	for(register int i=0; i < CONDITION_SIZE; ++i)
	{
		m_ConditionText2D[i].SetText( 0, 0, _T(" "), GetFont( IDS_DUDUM, 12), D3DCOLOR_XRGB( 255, 255, 255));
		m_ConditionText2D_2[i].SetText( 0, 0, _T(" "), GetFont( IDS_DUDUM, 12), D3DCOLOR_XRGB( 255, 255, 255));
	}
	
	TCHAR temp[2048]={0,};

	switch( m_byCurrContent)
	{
	case eContent: // 내용
		{
			// 문자열 설정 ( | 는 다음행 )
			register int len = m_vQuest[ m_wCurrQuestIndex]->m_szContent.length();

			memcpy( temp, m_vQuest[ m_wCurrQuestIndex]->m_szContent, len);
			
			int nHowManyLines = 0;
			for(register int i=0; i < len; ++i)
			{
				if( temp[i] == '|')//'\n')
					nHowManyLines++;
			}

			g_pUIManager->SetData(WINDOW_QUEST_01, quest_window_01_scrollbar2, SCROLL_TOTAL, nHowManyLines);

			int nCurrLine = g_pUIManager->GetData(WINDOW_QUEST_01, quest_window_01_scrollbar2, GET_SCROLL_CURRENT);
			int nMaxLine = g_pUIManager->GetData(WINDOW_QUEST_01, quest_window_01_scrollbar2, GET_SCROLL_MAX);

			int nTempLine = 0;

			char buf[256] = {0,};
			int c = 0;

			for( i=0; i < len; ++i)
			{
				//if(temp[i] == '\r')
				//	continue;
				if(temp[i] == '|')//'\n')
				{
					if(c > 0)
					{
						if( nCurrLine <= nTempLine)
						{
							int index = nTempLine - nCurrLine;
							if( index < nMaxLine)
							{
								m_ConditionText2D[index].SetText( 0, 0, (LPCTSTR)buf, GetFont( IDS_DUDUM, 12), D3DCOLOR_XRGB( 255, 255, 255));
							}
						}

						c = 0;
						memset(buf,0,sizeof(buf));
						++nTempLine;
					}
				}
				else
				{
					buf[c++] = (char)temp[i];
				}
			}
		}
		break;
	case eCondition: // 완수조건
		{	
			// [3/30/2004] 스크롤 문제 수정
			int nTotalLine = m_vQuest[ m_wCurrQuestIndex]->m_bConditionNum;

			g_pUIManager->SetData(WINDOW_QUEST_01, quest_window_01_scrollbar2, SCROLL_TOTAL, nTotalLine);

			int nCurrLine = g_pUIManager->GetData(WINDOW_QUEST_01, quest_window_01_scrollbar2, GET_SCROLL_CURRENT);
			int nMaxLine = g_pUIManager->GetData(WINDOW_QUEST_01, quest_window_01_scrollbar2, GET_SCROLL_MAX);

			for(register int i=0; i < nMaxLine; ++i)  // nTotalLine
			{
				//if( nCurrLine <= i) 
				{
					int index = i + nCurrLine; //i - nCurrLine;

					if(index < nTotalLine) // nMaxLine
					{	
						_tcscpy( temp, (LPCTSTR)m_vQuest[ m_wCurrQuestIndex]->m_szCondition[index]);  // i
						
						m_ConditionText2D[i].SetParentRect( &m_rtConditonRegion[i]);
						m_ConditionText2D[i].SetText( 0, 0, (LPCTSTR)temp, GetFont( IDS_DUDUM, 12), D3DCOLOR_XRGB( 255, 255, 255));

						TCHAR _[20]={0,};
						_stprintf( _, _T("%d / %d"), m_vQuest[ m_wCurrQuestIndex]->m_dwConditionCurrAmount[index], m_vQuest[ m_wCurrQuestIndex]->m_dwConditionTotalAmount[index]);  // i i

						m_ConditionText2D_2[i].SetText( 0, 0, (LPCTSTR)_, GetFont( IDS_DUDUM, 12), D3DCOLOR_XRGB( 255, 255, 255));
					}
				}
			}
		}
		break;
	case eReward: // 보상내역
		{
			// [3/30/2004] 스크롤 문제 수정
			int nTotalLine = m_vQuest[ m_wCurrQuestIndex]->m_bRewardNum;

			g_pUIManager->SetData(WINDOW_QUEST_01, quest_window_01_scrollbar2, SCROLL_TOTAL, nTotalLine);

			int nCurrLine = g_pUIManager->GetData(WINDOW_QUEST_01, quest_window_01_scrollbar2, GET_SCROLL_CURRENT);
			int nMaxLine = g_pUIManager->GetData(WINDOW_QUEST_01, quest_window_01_scrollbar2, GET_SCROLL_MAX);

			for(register int i=0; i < nMaxLine; ++i)  // nTotalLine
			{
				//if( nCurrLine <= i)
				{
					int index = i + nCurrLine; //i - nCurrLine;

					if(index < nTotalLine) //nMaxLine
					{
						_tcscpy( temp, (LPCTSTR)m_vQuest[ m_wCurrQuestIndex]->m_szReward[index]);  // i

						m_ConditionText2D[i].SetParentRect( &m_rtConditonRegion[i]);
						m_ConditionText2D[i].SetText( 0, 0, (LPCTSTR)temp, GetFont( IDS_DUDUM, 12), D3DCOLOR_XRGB( 255, 255, 255));

						TCHAR _[20]={0,};
						_stprintf( _, _T("%d"), m_vQuest[ m_wCurrQuestIndex]->m_dwRewardAmount[index]); // i

						m_ConditionText2D_2[i].SetText( 0, 0, (LPCTSTR)_, GetFont( IDS_DUDUM, 12), D3DCOLOR_XRGB( 255, 255, 255));
					}
				}
			}
		}
		break;
	}
}

void CQuest::ChangeStatus( DWORD dwQuestID, QUESTSTATUS_TYPE eType, BYTE bRepeat)
{
	sQuestInfo* pQuest = FindQuest( dwQuestID);
	if( pQuest)
	{
		pQuest->m_eStatus = eType;
		pQuest->m_bRepeat = bRepeat;
	}
	Refresh();
}

DWORD CQuest::GetCurrQuestID()
{
	// 아무것도 없을시
	if(m_vQuest.size() < 1)
		return 0;

	if( m_vQuest[m_wCurrQuestIndex])
		return m_vQuest[m_wCurrQuestIndex]->m_dwID;
	else
		return 0;
}

void CQuest::DeleteQuest(DWORD dwQuestID)
{

	VQUEST::iterator iter = m_vQuest.begin(); //HO_0820_07 퀘스트 분류 : 지울때는.. m_vQuest를 참조 해야 한다.(삭제시 밀어 올리기위해) 

	for( ; iter != m_vQuest.end(); ++iter)
	{
		sQuestInfo *pInfo = *iter;
		
		if(pInfo->m_dwID == dwQuestID)
		{
			m_vQuest.erase(iter);


			g_pUIManager->SetData(WINDOW_QUEST_01, quest_window_01_scrollbar1, SCROLL_TOTAL, m_vQuest.size());

			Refresh();
			return;
		}
	}	
}

//HT_0914 : 기연창 및 낭 아이템 개선 사항
void CQuest::UpdateQuest()
{
	BYTE i = 0;
	VQUEST::iterator iter;
	
	VQUEST TempQuest;

	for( ; i < 6; i++)
	{
		//for(iter = m_vQuest.begin(); iter != m_vQuest.end(); ++iter)
		for(iter = m_vTotalQuest.begin(); iter != m_vTotalQuest.end(); ++iter)
		{
			sQuestInfo *pInfo = *iter;

			if(pInfo->m_eStatus == i)
			{
				TempQuest.push_back( pInfo);
			}
		}	
	}
	m_vQuest = TempQuest;
	Refresh();
	g_pUIManager->SetData(WINDOW_QUEST_01, quest_window_01_scrollbar1, SCROLL_MOVE, 0);
	RefreshQuestIndex();
	RefreshQuestContent();

}

void CQuest::ProgressUpdateQuest()
{
	BYTE i = 0;
	VQUEST::iterator iter;
	
	VQUEST TempQuest;

	for( ; i < 2; i++)
	{
		//for(iter = m_vQuest.begin(); iter != m_vQuest.end(); ++iter)
		for(iter = m_vTotalQuest.begin(); iter != m_vTotalQuest.end(); ++iter)
		{
			sQuestInfo *pInfo = *iter;

			if(pInfo->m_eStatus == i)
			{
				TempQuest.push_back( pInfo);
			}
		}
	}
	
	m_vQuest = TempQuest;
	Refresh();
	g_pUIManager->SetData(WINDOW_QUEST_01, quest_window_01_scrollbar1, SCROLL_MOVE, 0);
	RefreshQuestIndex();
	RefreshQuestContent();
}

void CQuest::NewUpdateQuest()
{
	BYTE i = 0;
	VQUEST::iterator iter;
	
	VQUEST TempQuest;
	
	//for(iter = m_vQuest.begin(); iter != m_vQuest.end(); ++iter)
	for(iter = m_vTotalQuest.begin(); iter != m_vTotalQuest.end(); ++iter)
	{
		sQuestInfo *pInfo = *iter;

		if(pInfo->m_eStatus == 2)
		{
			TempQuest.push_back( pInfo);
		}
	}	

	m_vQuest = TempQuest;
	Refresh();
	g_pUIManager->SetData(WINDOW_QUEST_01, quest_window_01_scrollbar1, SCROLL_MOVE, 0);
	RefreshQuestIndex();
	RefreshQuestContent();
}

void CQuest::CompleteUpdateQuest()
{
	BYTE i = 0;
	VQUEST::iterator iter;
	
	VQUEST TempQuest;

	//for(iter = m_vQuest.begin(); iter != m_vQuest.end(); ++iter)
	for(iter = m_vTotalQuest.begin(); iter != m_vTotalQuest.end(); ++iter)
	{
		sQuestInfo *pInfo = *iter;

		if(pInfo->m_eStatus == 3)
		{
			TempQuest.push_back( pInfo);
		}
	}	

	m_vQuest = TempQuest;
	Refresh();
	g_pUIManager->SetData(WINDOW_QUEST_01, quest_window_01_scrollbar1, SCROLL_MOVE, 0);
	RefreshQuestIndex();
	RefreshQuestContent();
}

void CQuest::SetQuestDestination(DWORD QuestID, BYTE ProcessCount)
{	
	VQUESTCOORDINATE TempQuestcoordinate;
	m_sQuestHelp.m_bStart = true;

	sArrayData* pQuestHelp = XiahArrayIndex::g_QuestHelpList.GetData(QuestID,ProcessCount);

	if(pQuestHelp)
	{
		int Pline = pQuestHelp->GetInt(2);	
		int ProcessKind = pQuestHelp->GetInt(3);
		int TargetID = pQuestHelp->GetInt(4);

		
		if(Pline == 0)
		{
			SetQuestProcessKind(ProcessKind, TargetID,&TempQuestcoordinate);
		} 
		else //한 단계에 완수 조건이 여러 개인 경우
		{
			for(int i = Pline; i >= 1; i--)
			{
				sArrayData* pQuestHelp = XiahArrayIndex::g_QuestHelpList.GetData(QuestID, ProcessCount,  i);

				if(pQuestHelp)
				{
					int ProcessKind = pQuestHelp->GetInt(3);
					int TargetID = pQuestHelp->GetInt(4);
					SetQuestProcessKind(ProcessKind, TargetID, &TempQuestcoordinate);
				}
					
			} 
		} 
	}
	m_sQuestHelp.m_vQuestCoordinate = TempQuestcoordinate;
}

void CQuest::SetQuestProcessKind(int nProcessKind, int nTergetID, VQUESTCOORDINATE *TempQuestcoordinate)
{
	switch(nProcessKind)
	{
	case 10:	//몬스터 타입
		{
			m_sQuestHelp.m_byShowType = 1;

			sArrayData* pQuestMon = XiahArrayIndex::g_QuestMonList.GetData(nTergetID);

			BYTE MonType = pQuestMon->GetInt(1);

			std::vector<sArrayData*>::iterator it;

			for(it = XiahArrayIndex::g_QuestMonList.begin(); it != XiahArrayIndex::g_QuestMonList.end(); ++it)
			{
				pQuestMon = *it;

				if(pQuestMon)
				{
					if( pQuestMon->m_IntList[1] == MonType)
					{
						sQuestcoordinate* pQuestcoordinate = new sQuestcoordinate();

						pQuestcoordinate->xPos = pQuestMon->GetInt(3);
						pQuestcoordinate->yPos = pQuestMon->GetInt(4);
						pQuestcoordinate->MapID = pQuestMon->GetInt(2);
						
						TempQuestcoordinate->push_back(pQuestcoordinate);
					}
				}		
			}
		}
		break;
	case 11:	//몬스터 아이디
		{
			m_sQuestHelp.m_byShowType = 2;
			sArrayData* pQuestMon = XiahArrayIndex::g_QuestMonList.GetData(nTergetID);

			sQuestcoordinate* pQuestcoordinate = new sQuestcoordinate();

			pQuestcoordinate->xPos = pQuestMon->GetInt(3);
			pQuestcoordinate->yPos = pQuestMon->GetInt(4);
			pQuestcoordinate->MapID = pQuestMon->GetInt(2);
			
			TempQuestcoordinate->push_back(pQuestcoordinate);
		}
		break;
	case 16:	//NPC 타임
		{
			m_sQuestHelp.m_byShowType = 3;
			sArrayData* pQuestNPC = XiahArrayIndex::g_QuestNPCList.GetData(nTergetID);

			BYTE NPCType = pQuestNPC->GetInt(1);

			std::vector<sArrayData*>::iterator it;

			for(it = XiahArrayIndex::g_QuestNPCList.begin(); it != XiahArrayIndex::g_QuestNPCList.end(); ++it)
			{
				pQuestNPC = *it;

				if(pQuestNPC)
				{
					if( pQuestNPC->m_IntList[1] == NPCType)
					{
						sQuestcoordinate* pQuestcoordinate = new sQuestcoordinate();

						pQuestcoordinate->xPos = pQuestNPC->GetInt(3);
						pQuestcoordinate->yPos = pQuestNPC->GetInt(4);
						pQuestcoordinate->MapID = pQuestNPC->GetInt(2);
						
						TempQuestcoordinate->push_back(pQuestcoordinate);
					}
				}		
			}
		}
		break;
	case 17:	//NPC 아이디
		{
			m_sQuestHelp.m_byShowType = 3;
			sArrayData* pQuestNPC = XiahArrayIndex::g_QuestNPCList.GetData(nTergetID);

			sQuestcoordinate* pQuestcoordinate = new sQuestcoordinate();

			pQuestcoordinate->xPos = pQuestNPC->GetInt(3);
			pQuestcoordinate->yPos = pQuestNPC->GetInt(4);
			pQuestcoordinate->MapID = pQuestNPC->GetInt(2);
			
			TempQuestcoordinate->push_back(pQuestcoordinate);

		}
		break;
	default:
		break;
	} //switch
}
