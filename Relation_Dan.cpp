#include "XiahMap.h"

///////////////////////////////////////////////
// Dan
///////////////////////////////////////////////
#define DANDRAW_LEFT		250
#define DANDRAW_TOP			25		
#define DANDRAW_BOTTOM		20
#define DANDRAW_GAGETOP		60//HT_0403 : 지속형 무공 시전 아이콘( 단 게이지 수정)
#define DANDRAW_DISTANCE	20
#define DANDRAW_LENGTH		80

sDanInfo* CRelation::FindDanInfoByID( DWORD dwCharID)
{
	int nSize = m_vDan.size();

	for( int i=0 ;i < nSize; i++)
	{
		if( m_vDan[i]->m_dwCharID == dwCharID)
			return m_vDan[i];
	}

	return NULL;
}

sDanInfo* CRelation::FindDanInfoByName( LPCTSTR szName)
{
	int nSize = m_vDan.size();

	for( int i=0 ;i < nSize; i++)
	{
		if( _tcscmp( (LPCTSTR)m_vDan[i]->m_szNickName, szName) == 0)
			return m_vDan[i];
	}

	return NULL;
}

sDanInfo* CRelation::FindDanInfoByIndex( BYTE byIndex)
{
	int nSize = m_vDan.size();

	if( nSize <= byIndex)
		return NULL;

	if( m_vDan[ byIndex])
		return m_vDan[ byIndex];

	return NULL;
}

void CRelation::DrawDanInfo()
{
	if( !Am_I_InDan())
		return;
	
	// 단 경험치 분배

	for( int i=0; i < m_vDan.size(); ++i)
	{		
		if( m_vDan[i] && m_vDan[i]->m_dwCharID != g_MainCharInfo.m_dwObjectID)
		{
			sRect rtRegion;
			rtRegion.left	= DANDRAW_LEFT + (DANDRAW_LENGTH + DANDRAW_DISTANCE)*i;
			rtRegion.right	= rtRegion.left + DANDRAW_LENGTH;
			rtRegion.top	= DANDRAW_GAGETOP - DANDRAW_TOP;//HT_0403 : 지속형 무공 시전 아이콘( 단 게이지 수정 )
			rtRegion.bottom = DANDRAW_BOTTOM;

			m_text2D.SetParentRect( &rtRegion);
			m_text2D.SetText(  0, 0, (LPCTSTR)m_vDan[i]->m_szNickName, GetFont( IDS_GULIM, 12), 4294967295);  // 4294967295 D3DCOLOR_XRGB( 255, 255, 255)

			m_text2D.Render();			

			XiahObject::CXiahObject* pObject = XiahObject::g_XiahObjectManager.FindXiahObject( MAKEOBJECTID( 0, m_vDan[i]->m_dwCharID, OBJTYPE_PC));

			if( pObject)
			{
				// 생명력
				CXiahCharObject* pCharObject = (CXiahCharObject*)pObject->m_pObject;
				if( pCharObject)
				{					
					if(m_nDanType == 0)
					{
						RenderEnergyGauge( DANDRAW_LEFT + (DANDRAW_LENGTH + DANDRAW_DISTANCE)*i, DANDRAW_GAGETOP, DANDRAW_LENGTH, m_vDan[i]->m_dwCurHp, m_vDan[i]->m_dwMaxHp, D3DCOLOR_XRGB(0, 255, 255), D3DCOLOR_XRGB(0, 0, 0), 5);
					}
					else
					{
						RenderEnergyGauge( DANDRAW_LEFT + (DANDRAW_LENGTH + DANDRAW_DISTANCE)*i, DANDRAW_GAGETOP, DANDRAW_LENGTH, m_vDan[i]->m_dwCurHp, m_vDan[i]->m_dwMaxHp, D3DCOLOR_XRGB(255, 125, 255), D3DCOLOR_XRGB(0, 0, 0), 5);
					}
				}

				//if(g_pUIManager->IsShow(WINDOW_DAN) && m_eCurrType == eDAN)
				if(g_pUIManager->IsShow(WINDOW_DAN_NEW) && m_eCurrType == eDAN)
				{	
					// 위치
					g_pUIManager->SetString(WINDOW_DAN, dan_window_list_dummy_14 + i, IDS_ABLE);
				}
			}
			else
			{
				//if(g_pUIManager->IsShow(WINDOW_DAN) && m_eCurrType == eDAN)
				if(g_pUIManager->IsShow(WINDOW_DAN_NEW) && m_eCurrType == eDAN)
				{
					// 위치
					g_pUIManager->SetString(WINDOW_DAN, dan_window_list_dummy_14 + i, IDS_DISABLE);
				}
			}
		}
	}
}

/**
 *
 * \param dwCharID 
 * \param byPriority 
 * \param szNickName 
 * \param byLevel 
 * \param wPosX 
 * \param wPosY 
 * \param wCurHp 
 * \param wMaxHp 
 * \param dwMapID 
 */
void CRelation::InsertDan( DWORD dwCharID, BYTE byPriority, sString szNickName, BYTE byLevel, WORD wPosX, WORD wPosY, DWORD dwCurHp, DWORD dwMaxHp, DWORD dwMapID)
{
	if( dwCharID == g_MainCharInfo.m_dwObjectID)
		return;

	//if( FindDanInfoByID( dwCharID))
	//	return;

	sDanInfo* pDan = FindDanInfoByID(dwCharID);

	bool bExistence = false;

	if(pDan)
	{
		bExistence = true;
	}
	else
	{
		pDan = new sDanInfo();
	}

	pDan->m_dwCharID	= dwCharID;
	pDan->m_byPriority	= byPriority;
	pDan->m_szNickName	= szNickName;
	pDan->m_byLevel		= byLevel;
	pDan->m_wPosX		= wPosX / 4;
	pDan->m_wPosY		= wPosY / 4;
	pDan->m_dwCurHp		= dwCurHp;
	pDan->m_dwMaxHp		= dwMaxHp;
	pDan->m_byPriority	= m_vDan.size();
	pDan->m_dwMapID		= dwMapID;

	if(!bExistence)
		m_vDan.push_back( pDan);

	XiahObject::CXiahObject* pObject = XiahObject::g_XiahObjectManager.FindXiahObject( MAKEOBJECTID( 0, dwCharID, OBJTYPE_PC));
	if( pObject)
	{
		CXiahCharObject* pCharObject = (CXiahCharObject*)pObject->m_pObject;
		if( pCharObject)
		{
			if(pCharObject->m_dwFame >= 127)
				pCharObject->m_cNameColor = D3DCOLOR_XRGB( 200, 255, 255);
			else
				pCharObject->RefreshFameColor();
		}
	}
	else
	{
		DBG_LogFile( _T("CRelation::InsertDan2 fail"));
	}

	RefreshDanContent();
}

void CRelation::DeleteDan( DWORD dwCharID)
{
	if( dwCharID == g_MainCharInfo.m_dwObjectID)
		ClearDan();

	int nSize = m_vDan.size();

	VDAN::iterator where = m_vDan.begin();

	for( int i=0 ;i < nSize; i++)
	{
		if( m_vDan[i]->m_dwCharID == g_MainCharInfo.m_dwObjectID)
		{
			ClearDan();
			RefreshDanContent();
			return;
		}
		else if( m_vDan[i]->m_dwCharID == dwCharID)
		{
			XiahObject::CXiahObject* pObject = XiahObject::g_XiahObjectManager.FindXiahObject( MAKEOBJECTID( 0, dwCharID, OBJTYPE_PC));
			if( pObject)
			{
				CXiahCharObject* pCharObject = (CXiahCharObject*)pObject->m_pObject;
				if( pCharObject)
					pCharObject->RefreshFameColor();
			}
			else
			{
				DBG_LogFile( _T("CRelation::DeleteDan fail"));
			}

			m_vDan.erase( where);
			RefreshDanContent();
			return;
		}
		where++;
	}
}

/**
 *
 */
void CRelation::ClearDan()
{
	if(!g_pUIManager)
		return;

	int nSize = m_vDan.size();

	for( int i=0 ; i < nSize; ++i)
	{
		XiahObject::CXiahObject* pObject = XiahObject::g_XiahObjectManager.FindXiahObject( MAKEOBJECTID( 0, m_vDan[i]->m_dwCharID, OBJTYPE_PC));
		if( pObject)
		{
			CXiahCharObject* pCharObject = (CXiahCharObject*)pObject->m_pObject;
			if( pCharObject)
				pCharObject->RefreshFameColor();
		}
		else
		{
			DBG_LogFile( _T("ClearDan fail"));
		}

		delete m_vDan[i];
		m_vDan[i]  = NULL;
	}

	m_vDan.clear();

	g_pUIManager->Show(WINDOW_DAN, dan_window_1button);
	g_pUIManager->Hide(WINDOW_DAN, dan_window_2button_01);
	g_pUIManager->Hide(WINDOW_DAN, dan_window_2button_02);

	// 단 경험치 분배
	g_MainCharInfo.CloseFrame(WINDOW_DAN_NEW);
	g_pUIManager->Show(WINDOW_DAN_NEW, window_dan_new_1button);
	g_pUIManager->Hide(WINDOW_DAN_NEW, window_dan_new_2button_01);
	g_pUIManager->Hide(WINDOW_DAN_NEW, window_dan_new_2button_02);

	SetDanID( 0);				
	SetDanLeader( 0);
	RefreshDanContent();
}	
	
void CRelation::RefreshDanInfo( DWORD dwCharID, WORD wLevel, DWORD dwHpCur, DWORD dwHpMax, DWORD dwMapID, WORD wPosX, WORD wPosY)
{
	sDanInfo* pInfo = FindDanInfoByID( dwCharID);
	if( pInfo)
	{
		pInfo->m_byLevel	= wLevel;
		pInfo->m_dwCurHp	= dwHpCur;
		pInfo->m_dwMaxHp	= dwHpMax;
		pInfo->m_wPosX		= wPosX;
		pInfo->m_wPosY		= wPosY;
		pInfo->m_dwMapID	= dwMapID;
	}

	RefreshDanContent();
}

////////////////////////////////////////////////
void CRelation::SetDanLeader( DWORD dwDanLeader)
////////////////////////////////////////////////
{ 
	m_dwDanLeader = dwDanLeader;

	if( dwDanLeader == 0)
		return;

	TCHAR content[128] = {0,};

	//HT_0423 : 단주 위임
	if(dwDanLeader == g_MainCharInfo.m_dwObjectID)
		_stprintf( content, IDS_DANCOMMIT_MYSELF);
	else
        _stprintf( content, IDS_D_BE_DANJU, (LPCTSTR)FindRelationNameByID( dwDanLeader));

	g_MainCharInfo.ShowHelpMessage( content);
}

void CRelation::RefreshDanContent()
{
	if(!g_pUIManager)
		return;

	// 경험치 분배
	if(/*!g_pUIManager->IsShow(WINDOW_DAN) ||*/ m_eCurrType != eDAN)
		return;

	// 일단 한번 싹 지워주고
	for( int k=0; k < 5; ++k)
	{
		g_pUIManager->SetString(WINDOW_DAN, dan_window_list_dummy_04 + k, _T(""));
		g_pUIManager->SetString(WINDOW_DAN, dan_window_list_dummy_09 + k, _T(""));
		g_pUIManager->SetString(WINDOW_DAN, dan_window_list_dummy_14 + k, _T(""));

		g_pUIManager->SetString(WINDOW_DAN_NEW, window_dan_new_list_dummy4 + k, _T(""));
		g_pUIManager->SetString(WINDOW_DAN_NEW, window_dan_new_list_dummy9 + k, _T(""));
		g_pUIManager->SetString(WINDOW_DAN_NEW, window_dan_new_list_dummy14 + k, _T(""));
	}

	// 내용을 적어주기
	g_pUIManager->SetString(WINDOW_DAN, dan_window_list_dummy_01, IDS_NAME);
	g_pUIManager->SetString(WINDOW_DAN, dan_window_list_dummy_02, IDS_LEVEL);
	g_pUIManager->SetString(WINDOW_DAN, dan_window_list_dummy_03, IDS_EXP_SHARE);

	// 경험치 분배
	g_pUIManager->SetString(WINDOW_DAN_NEW, window_dan_new_list_dummy1, IDS_NAME);
	g_pUIManager->SetString(WINDOW_DAN_NEW, window_dan_new_list_dummy2, IDS_LEVEL);
	g_pUIManager->SetString(WINDOW_DAN_NEW, window_dan_new_list_dummy3, IDS_EXP_SHARE);

	for( int i=0; i < m_vDan.size(); ++i)
	{
		if( m_vDan[i] && m_vDan[i]->m_dwCharID != g_MainCharInfo.m_dwObjectID)
		{
			// 단 경험치 분배

			// 이름
			if( m_dwDanLeader == m_vDan[i]->m_dwCharID)
				g_pUIManager->SetString(WINDOW_DAN_NEW, window_dan_new_list_dummy4 + i, (LPCTSTR)m_vDan[i]->m_szNickName, 9);//HO_0424_07 파랑색 수정 요청 노랑색으로 변경
			//g_pUIManager->SetString(WINDOW_DAN, dan_window_list_dummy_04 + i, (LPCTSTR)m_vDan[i]->m_szNickName, 1);
			else
				g_pUIManager->SetString(WINDOW_DAN_NEW, window_dan_new_list_dummy4 + i, (LPCTSTR)m_vDan[i]->m_szNickName);
			//g_pUIManager->SetString(WINDOW_DAN, dan_window_list_dummy_04 + i, (LPCTSTR)m_vDan[i]->m_szNickName);

			// 레벨, 150갑자 초과시 
			if(m_vDan[i]->m_byLevel < 150)
			{
                g_pUIManager->SetString(WINDOW_DAN_NEW, window_dan_new_list_dummy9 + i, m_vDan[i]->m_byLevel);
			}
			else
			{
				g_pUIManager->SetString(WINDOW_DAN_NEW, window_dan_new_list_dummy9 + i, IDS_BESTLEVEL, 9);
			}
			//g_pUIManager->SetString(WINDOW_DAN, dan_window_list_dummy_09 + i, m_vDan[i]->m_byLevel);

			// 경험치 공유
			//XiahObject::CXiahObject* pObject = XiahObject::g_XiahObjectManager.FindXiahObject( MAKEOBJECTID( 0, m_vDan[i]->m_dwCharID, OBJTYPE_PC));
			// [6/10/2005] 단 맵

			if(XiahMap::g_XiahMap.m_MapInfo.m_dwMapID == m_vDan[i]->m_dwMapID)
			{
				g_pUIManager->SetString(WINDOW_DAN_NEW, window_dan_new_list_dummy14 + i, IDS_ABLE);
			}
			else
			{
				g_pUIManager->SetString(WINDOW_DAN_NEW, window_dan_new_list_dummy14 + i, IDS_DISABLE);
			}
		}
	}
}

////////////////////////////
BOOL CRelation::Am_I_InDan()
////////////////////////////
{
	if( m_dwDanID)
		return TRUE;

	if( Am_I_LeaderInDan())
		return TRUE;

	return FALSE;
}

//////////////////////////////////
BOOL CRelation::Am_I_LeaderInDan()
//////////////////////////////////
{
	if( m_dwDanLeader == g_MainCharInfo.m_dwObjectID)
		return TRUE;

	return FALSE;
}
