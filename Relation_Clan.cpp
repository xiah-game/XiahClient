
bool UDgreater( sClanWonInfo* elem1, sClanWonInfo* elem2 )
{
	return elem1->m_dwOrderID < elem2->m_dwOrderID;
}



///////////////////////////////////////////////
// Clan
///////////////////////////////////////////////
sClanWonInfo* CRelation::FindClanInfoByID( DWORD dwCharID)
{
	int nSize = m_vClan.size();

	for( int i=0 ;i < nSize; ++i)
	{
		if( m_vClan[i]->m_dwCharID == dwCharID)
			return m_vClan[i];
	}

	return NULL;
}

sClanWonInfo* CRelation::FindClanInfoByName( LPCTSTR szName)
{
	int nSize = m_vClan.size();

	for( int i=0 ;i < nSize; ++i)
	{
		if( _tcscmp( (LPCTSTR)m_vClan[i]->m_szCharName, szName) == 0)
			return m_vClan[i];
	}

	return NULL;
}

sClanWonInfo* CRelation::FindClanInfoByIndex( BYTE byIndex)
{
	int nSize = m_vClan.size();

	if( nSize <= byIndex)
		return NULL;

	if( m_vClan[ byIndex])
		return m_vClan[ byIndex];

	return NULL;
}


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CRelation::InsertClan( DWORD dwOrderID, sString szOrderName, DWORD dwCharID, sString szCharName, sString szMunpaNickName, WORD wService, BYTE bState, BYTE bType)
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
{
	if( FindClanInfoByID( dwCharID))
		return;

	sClanWonInfo* pClan = new sClanWonInfo();
	if(pClan == NULL)
	{
		DBG_LogFile( _T("CRelation::InsertClan fail"));
	}

	pClan->m_dwOrderID		= dwOrderID;
	pClan->m_szOrderName	= szOrderName;
	pClan->m_dwCharID		= dwCharID;
	pClan->m_szCharName		= szCharName;
	pClan->m_szMunpaNickName = szMunpaNickName;
	pClan->m_bState			= bState;
	pClan->m_bCharType		= bType;
		
	m_vClan.push_back( pClan);

	g_pUIManager->SetData(WINDOW_QUEST_01, quest_window_01_scrollbar2, SCROLL_TOTAL, GetClanSize());

	RefreshClanContent();

	if( dwOrderID == 1)
		SetClanLeader( dwCharID);
}

////////////////////////////
void CRelation::DeleteClan( DWORD dwCharID)
////////////////////////////
{
	int nSize = m_vClan.size();

	VCLAN::iterator where = m_vClan.begin();

	for( int i=0 ;i < nSize; ++i)
	{
		if( m_vClan[i]->m_dwCharID == dwCharID)
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
				DBG_LogFile( _T("CRelation::DeleteClan fail"));
			}

			m_vClan.erase( where);
			RefreshClanContent();
			return;
		}

		++where;
	}
}

///////////////////////////
void CRelation::ClearClan()
///////////////////////////
{
	int nSize = m_vClan.size();

	for( int i=0 ;i < nSize; ++i)
	{
		XiahObject::CXiahObject* pObject = XiahObject::g_XiahObjectManager.FindXiahObject( MAKEOBJECTID( 0, m_vClan[i]->m_dwCharID, OBJTYPE_PC));
		if( pObject)
		{
			CXiahCharObject* pCharObject = (CXiahCharObject*)pObject->m_pObject;
			if( pCharObject)
				pCharObject->RefreshFameColor();
		}
		else
		{
			//DBG_LogFile( _T("CRelation::ClearClan() fail"));
		}

		delete m_vClan[i];
		m_vClan[i]  = NULL;
	}

	m_vClan.clear();

	SetClanID( 0);				
	SetClanLeader( 0);
	RefreshClanContent();

	if(g_pUIManager && g_pUIManager->IsShow(WINDOW_MUNPA))
	{
		g_MainCharInfo.CloseFrame(WINDOW_MUNPA);
		g_MainCharInfo.OpenFrame(WINDOW_MUNPA_FOUND);
	}

}

////////////////////////////////////
void CRelation::RefreshClanContent()
////////////////////////////////////
{
	if(!g_pUIManager)
		return;

	if(!g_pUIManager->IsShow(WINDOW_MUNPA))
		return;

	// 일단 한번 싹 지워주고
	for( int k=0; k < MAX_NUM_IN_FRAME_CLAN; ++k)
	{
		g_pUIManager->SetString(WINDOW_MUNPA, munpa_window_list_dummy_01 + k, _T(""));
		g_pUIManager->SetString(WINDOW_MUNPA, munpa_window_list_dummy_09 + k, _T(""));

		g_pUIManager->SetString(WINDOW_MUNPA, munpa_window_list_dummy_001 + k, _T(""));
		g_pUIManager->Hide(WINDOW_MUNPA, munpa_window_icon_01 + k);
	}

	sort( m_vClan.begin(), m_vClan.end(), UDgreater);

	sClanWonInfo* pClanInfo = FindClanInfoByID(g_MainCharInfo.m_dwObjectID);

	if(pClanInfo)
	{
		g_pUIManager->SetString(WINDOW_MUNPA, found_window_contents_dummy_001, m_szClanName);
		g_pUIManager->SetString(WINDOW_MUNPA, found_window_contents_dummy_002, pClanInfo->m_szMunpaNickName);
		g_pUIManager->SetString(WINDOW_MUNPA, found_window_contents_dummy_003, pClanInfo->m_szOrderName);
		g_pUIManager->SetString(WINDOW_MUNPA, found_window_contents_dummy_004, g_MainCharInfo.m_dwMunpaFame);
		g_pUIManager->SetString(WINDOW_MUNPA, found_window_contents_dummy_005, m_vClan.size());
	}

	int nIndex=0;
	for( int i=0; i < MAX_NUM_IN_FRAME_CLAN; ++i)
	{
		nIndex = (GetCurrPage()-1)*8 + i;

		if( nIndex < 0)
			return;

		if( nIndex > (int)(m_vClan.size() -1))
			return;

		sClanWonInfo* pInfo = m_vClan[nIndex];

		if( pInfo)
		{
			// 정보셋팅
			// 안씨 왜 이렇게 비교했지? 
//			if( pInfo->m_dwCharID == g_MainCharInfo.m_dwObjectID)
//			{
//				g_pUIManager->SetString(WINDOW_MUNPA, found_window_contents_dummy_001, m_szClanName);
//				g_pUIManager->SetString(WINDOW_MUNPA, found_window_contents_dummy_002, pInfo->m_szMunpaNickName);
//				g_pUIManager->SetString(WINDOW_MUNPA, found_window_contents_dummy_003, pInfo->m_szOrderName);
//				g_pUIManager->SetString(WINDOW_MUNPA, found_window_contents_dummy_004, g_MainCharInfo.m_dwMunpaFame);
//				g_pUIManager->SetString(WINDOW_MUNPA, found_window_contents_dummy_005, m_vClan.size());
//			}

			// 이름
			g_pUIManager->SetString(WINDOW_MUNPA, munpa_window_list_dummy_01 + i, (LPCTSTR)pInfo->m_szCharName);			

			// 호칭
			g_pUIManager->SetString(WINDOW_MUNPA, munpa_window_list_dummy_09 + i, (LPCTSTR)pInfo->m_szMunpaNickName);

			// 직책
			g_pUIManager->SetString(WINDOW_MUNPA, munpa_window_list_dummy_001 + i, (LPCTSTR)pInfo->m_szOrderName);

			// 접속여부
			g_pUIManager->Show(WINDOW_MUNPA, munpa_window_icon_01 + i);

			if( pInfo->m_bState)
				g_pUIManager->SetData(WINDOW_MUNPA, munpa_window_icon_01 + i, CURRENT_INDEX, 0);
			else
				g_pUIManager->SetData(WINDOW_MUNPA, munpa_window_icon_01 + i, CURRENT_INDEX, 1);	
		}
	}
}

/////////////////////////////
BOOL CRelation::Am_I_InClan()
/////////////////////////////
{
	if( m_dwClanID)
		return TRUE;

	if( Am_I_LeaderInClan())
		return TRUE;

	return FALSE;
}

///////////////////////////////////
BOOL CRelation::Am_I_LeaderInClan()
///////////////////////////////////
{
	if( m_dwClanLeader == g_MainCharInfo.m_dwObjectID)
		return TRUE;

	return FALSE;
}

bool CRelation::Am_I_2stLeaderInClan()
{
	sClanWonInfo* pClanInfo = FindClanInfoByID(g_MainCharInfo.m_dwObjectID);

	if(pClanInfo)
	{
		// 알수 있는 타입도 없기에 서버에서 오는 이름으로 비교 주의요망
		if(stricmp(pClanInfo->m_szOrderName, IDS_BUMUNJU) == 0)
			return true;
	}

	return false;
}
