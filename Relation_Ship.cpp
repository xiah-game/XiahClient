

///////////////////////////////////////////////
// Ship
///////////////////////////////////////////////
///////////////////////////////////////////////////////
sShipInfo* CRelation::FindShipInfoByID( DWORD dwCharID)
///////////////////////////////////////////////////////
{
	int nSize = m_vShip.size();

	for( int i=0 ;i < nSize; i++)
	{
		if( m_vShip[i]->m_dwCharID == dwCharID)
			return m_vShip[i];
	}
	
	return NULL;
}

sShipInfo* CRelation::FindShipInfoByName( LPCTSTR szName)
{
	int nSize = m_vShip.size();

	for( int i=0 ;i < nSize; i++)
	{
		if( _tcscmp( (LPCTSTR)m_vShip[i]->m_szNickName, szName) == 0)
			return m_vShip[i];
	}

	return NULL;
}

sShipInfo* CRelation::FindShipInfoByIndex( BYTE byIndex)
{
	int nSize = m_vShip.size();

	if( nSize <= byIndex)
		return NULL;

	if( m_vShip[ byIndex])
		return m_vShip[ byIndex];

	return NULL;
}


/**
 *
 * \param bShipType 
 * \param dwCharID 
 * \param szNickName 
 * \param bWorldID 
 * \param dwMapID 
 * \param wPosX 
 * \param wPosY 
 * \param bIsConnect 
 */
void CRelation::InsertShip( BYTE bShipType, DWORD dwCharID, sString szNickName, BYTE bWorldID, DWORD dwMapID, WORD wPosX, WORD wPosY, BOOL bIsConnect)
{
	if( dwCharID == g_MainCharInfo.m_dwObjectID)
		return;

	sShipInfo* pTemp = FindShipInfoByID( dwCharID);
	if( pTemp )
	{
		pTemp->m_bShipType = bShipType;

		return;
	}

	sShipInfo* pShip = new sShipInfo();
//	if(pShip == NULL)
//	{
//		DBG_LogFile( _T("CRelation::InsertShip fail"));
//	}

	pShip->m_bShipType	= bShipType;
	pShip->m_dwCharID	= dwCharID;
	pShip->m_szNickName = szNickName;
	pShip->m_wPosX		= wPosX / 4;
	pShip->m_wPosY		= wPosY / 4;
	pShip->m_bIsConnect = bIsConnect;
	pShip->m_wPriority	= m_vShip.size();
	
	m_vShip.push_back( pShip);

	XiahObject::CXiahObject* pObject = XiahObject::g_XiahObjectManager.FindXiahObject( MAKEOBJECTID( 0, dwCharID, OBJTYPE_PC));

	if( pObject)
	{
		CXiahCharObject* pCharObject = (CXiahCharObject*)pObject->m_pObject;

		if( pCharObject)
		{
			if(pCharObject->m_dwFame >= 127)
				pCharObject->m_cNameColor = D3DCOLOR_XRGB( 255, 200, 255);
			else
				pCharObject->RefreshFameColor();
		}
	}
}

///////////////////////////////////////////
void CRelation::DeleteShip( DWORD dwCharID)
///////////////////////////////////////////
{
	int nSize = m_vShip.size();

	VSHIP::iterator where = m_vShip.begin();

	for(int i=0; i < nSize; ++i)
	{
		if( m_vShip[i]->m_dwCharID == dwCharID)
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
				DBG_LogFile( _T("CRelation::DeleteShip fail"));
			}

			m_vShip.erase( where);
			RefreshShipContent();
			return;
		} // if( m_vShip[i]->m_dwCharID == dwCharID)

		++where;
	}
}

///////////////////////////
void CRelation::ClearShip()
///////////////////////////
{
	int nSize = m_vShip.size();

	for(int i=0; i < nSize; ++i)
	{
		XiahObject::CXiahObject* pObject = XiahObject::g_XiahObjectManager.FindXiahObject( MAKEOBJECTID( 0, m_vShip[i]->m_dwCharID, OBJTYPE_PC));

		if( pObject)
		{
			CXiahCharObject* pCharObject = (CXiahCharObject*)pObject->m_pObject;

			if( pCharObject)
				pCharObject->RefreshFameColor();
			else
			{
				DBG_LogFile( _T("CRelation::ClearShip()2 fail"));
			}
		}
		else
		{
				//DBG_LogFile( _T("CRelation::ClearShip()1 fail"));
		}

		delete m_vShip[i];
		m_vShip[i]  = NULL;
	}

	m_vShip.clear();
}

////////////////////////////////////////////////
void CRelation::RefreshShipContent( BYTE byPage)
////////////////////////////////////////////////
{
	if(!g_pUIManager)
		return;

	if(!g_pUIManager->IsShow(WINDOW_DAN) || m_eCurrType != eShip)
		return;

	// 일단 한번 싹 지워주고
	for(int k=0; k < 5; ++k)
	{
		g_pUIManager->SetString(WINDOW_DAN, dan_window_list_dummy_04 + k, _T(""));
		g_pUIManager->SetString(WINDOW_DAN, dan_window_list_dummy_09 + k, _T(""));
		g_pUIManager->SetString(WINDOW_DAN, dan_window_list_dummy_14 + k, _T(""));						
	}

	// 내용을 적어주기
	g_pUIManager->SetString(WINDOW_DAN, dan_window_list_dummy_01, IDS_NAME);
	g_pUIManager->SetString(WINDOW_DAN, dan_window_list_dummy_02, IDS_RELATION);
	g_pUIManager->SetString(WINDOW_DAN, dan_window_list_dummy_03, IDS_STATUS);

	int nIndex=0;
	for( int i=0; i < 5; ++i)
	{
		nIndex = (GetCurrPage()-1) * 5 + i;

		if( nIndex < 0)
			return;

		if( nIndex > static_cast<int>(m_vShip.size() -1))
			return;

		sShipInfo* pInfo = m_vShip[nIndex];

		if( pInfo)
		{
			TCHAR szShip[128] = {0,};
			TCHAR szConnect[128] = {0,};

			switch( pInfo->m_bShipType)
			{
			case RELATION_TYPE_LOVER:
				_tcscpy( szShip, IDS_SWEETHEART );
				break;
			case RELATION_TYPE_TEACHER:
				_tcscpy( szShip, IDS_TEACHER );
				break;
			case RELATION_TYPE_STUDENT:
				_tcscpy( szShip, IDS_DISCIPLE );
				break;
			case RELATION_TYPE_BUDDY:
				_tcscpy( szShip, IDS_FRIEND);
				break;

//			case 0:
//				_tcscpy( szShip, IDS_FRIEND);
			default:
				_tcscpy( szShip, IDS_NONERELATION );
			} // switch( pInfo->m_bShipType)

			if( pInfo->m_bIsConnect)
				_tcscpy( szConnect, IDS_CONNECT);
			else
				_tcscpy( szConnect, IDS_CONNECT_NOT);

			g_pUIManager->SetString(WINDOW_DAN, dan_window_list_dummy_04 + i, (LPCTSTR)pInfo->m_szNickName);
			g_pUIManager->SetString(WINDOW_DAN, dan_window_list_dummy_09 + i, szShip);
			g_pUIManager->SetString(WINDOW_DAN, dan_window_list_dummy_14 + i, szConnect);
		}
		else
		{
			DBG_LogFile( _T("CRelation::RefreshShipContent fail"));
		}
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////////
void CRelation::ChangeBuddyConnect( DWORD dwCharID, BYTE bWorldID, DWORD dwMapID, BOOL bIsConnected)
////////////////////////////////////////////////////////////////////////////////////////////////////
{
	sShipInfo* pInfo = FindShipInfoByID( dwCharID);

	if( pInfo)
	{
		pInfo->m_bIsConnect = bIsConnected;

		TCHAR szText[128] = {0,};
		LPCTSTR lpstrTemp = NULL;

		switch(pInfo->m_bShipType)
		{
		case RELATION_TYPE_LOVER:
			lpstrTemp = IDS_SWEETHEART;
			break;
		case RELATION_TYPE_TEACHER:
			lpstrTemp = IDS_TEACHER;
			break;
		case RELATION_TYPE_STUDENT:
			lpstrTemp = IDS_DISCIPLE;
			break;
		case RELATION_TYPE_BUDDY:
			lpstrTemp = IDS_FRIEND;
			break;
		default:
			lpstrTemp = IDS_NONERELATION;
			break;
		}

		if( bIsConnected)
			_stprintf( szText, IDS_CONNECT_CONNECT,  lpstrTemp, (LPCTSTR)pInfo->m_szNickName);
		else
			_stprintf( szText, IDS_CONNECT_OUT, lpstrTemp, (LPCTSTR)pInfo->m_szNickName);

		g_MainCharInfo.PlayInterfaceSound( ISOUND_FRIEND_CONNECT);
		g_MainCharInfo.ShowHelpMessage( szText);
		RefreshShipContent();
	}
	else
	{
		DBG_LogFile( _T("CRelation::ChangeBuddyConnect fail"));
	}
}

DWORD CRelation::FindSabuID()
{
	int nSize = m_vShip.size();

	for(int i=0; i < nSize; ++i)
	{
		if(m_vShip[i]->m_bShipType == 20)
			return m_vShip[i]->m_dwCharID;
	}

	return 0;
}

DWORD CRelation::FindJejaID()
{
	int nSize = m_vShip.size();

	for(int i=0; i < nSize; ++i)
	{
		if(m_vShip[i]->m_bShipType == 30)
			return m_vShip[i]->m_dwCharID;
	}

	return 0;
}

DWORD CRelation::FindSweetheart()
{
	int nSize = m_vShip.size();

	for(int i=0; i < nSize; ++i)
	{
		if(m_vShip[i]->m_bShipType == 10)
			return m_vShip[i]->m_dwCharID;
	}

	return 0;
}