extern sString MoneyCommaStr(INT64 nMoney);
extern LPCTSTR GetMapName(DWORD dwMapID);

//////////////////////////////////////////////////////////
//	Refresh Frames
//////////////////////////////////////////////////////////
void CharacterInfo::RefreshFramePos()
{

}

void CharacterInfo::RefreshChracterInfo()
{
	TCHAR strHp[64]={0,};
	TCHAR strIp[64]={0,};
	TCHAR atk[64]={0,};
	TCHAR def[64]={0,};
	TCHAR atkrat[64]={0,};
	TCHAR Exp[64]={0,};
	TCHAR plusspeed[64]={0,};
	TCHAR critical[64]={0,};
	TCHAR strFame[64]={0,};
	TCHAR strName[128]={0,};

	BYTE byType;
	
	int plusAtk = m_dwTotalAtkPower - m_wBaseAtkPwr;
	int plusDef = m_dwTotalDefPower - m_wBaseDefPwr;
	int plusAtkrat = m_dwTotalAttackRating - m_wBaseAttackRating;
	
	_stprintf( strHp, _T("%d / %d"),	m_dwHpCur, m_dwHpMax);
	_stprintf( strIp, _T("%d / %d"),	m_wIpCur, m_wIpMax);
	if( plusAtk)
		_stprintf( atk,   _T("%d + %d"),	m_wBaseAtkPwr, plusAtk);
	else
		_stprintf( atk,   _T("%d"),	m_wBaseAtkPwr);
	if( plusDef)
		_stprintf( def,   _T("%d + %d"),	m_wBaseDefPwr, plusDef);
	else
		_stprintf( def,	_T("%d"),	m_wBaseDefPwr);
	if( plusAtkrat)
		_stprintf( atkrat,_T("%d + %d"),	m_wBaseAttackRating, plusAtkrat);
	else
		_stprintf( atkrat,_T("%d"),	m_wBaseAttackRating);
	
	_stprintf( plusspeed, _T("+ %d"), m_bPlusSpeed);
	_stprintf( critical, _T("+ %d"), m_wCritical);

	//sprintf( Exp, "%d / %d", m_dwExp, m_dwNextLevelUpExp);
	//int dwMAXLevelExp = g_MainCharInfo.m_dwNextLevelUpExp - g_MainCharInfo.m_dwLevelExp;
	//int dwSUBLevelExp = g_MainCharInfo.m_i64Exp - g_MainCharInfo.m_dwLevelExp;

	//150갑자 초과�
	//HT_0621 ; 경험� 수치 수정
	if(m_wLevel<150)
	{
        INT64 i64MAXLevelExp = g_MainCharInfo.m_i64NextLevelUpExp - g_MainCharInfo.m_i64LevelExp;
        INT64 i64SUBLevelExp = g_MainCharInfo.m_i64Exp - g_MainCharInfo.m_i64LevelExp;

		_stprintf( Exp, _T("%I64d / %I64d"), i64SUBLevelExp, i64MAXLevelExp);
	}
	else
	{
		_stprintf( Exp, _T("-"));
	}


#ifdef TRACE_LOG
	if(pFrame == NULL)
	{
		DBG_LogFile( _T("RefreshChracterInfo에서 g_pUIManager->GetFrame 실패"));
	}
#endif

	if( g_MainCharInfo.m_dwFame > 126)
	{
		g_pUIManager->SetString(WINDOW_CHARACTER, character_window_contents_dummy_12, IDS_FAME, 0);
		_stprintf( strFame, IDS_RATE, g_MainCharInfo.m_dwFame - 127);

		byType = 0;
	}
	else
	{
		g_pUIManager->SetString(WINDOW_CHARACTER, character_window_contents_dummy_12, VICE_LEVEL, 1);
		_stprintf( strFame, IDS_RATE, 127 - g_MainCharInfo.m_dwFame);

		byType = 1;
	}


	g_pUIManager->SetString(WINDOW_CHARACTER, character_window_contents_dummy_004, Exp);
	g_pUIManager->SetString(WINDOW_CHARACTER, character_window_contents_dummy_012, strFame, byType);

	// 각성했을�
	if(m_bRebirth)
	{	
		if(m_bRebirth < 7)	//HT_0702 : 각성�
			_stprintf( strName, IDS_REBIRTH_COUNT_01, (LPCTSTR)m_szNickName, m_bRebirth);
		else				//진각성자
			_stprintf( strName, IDS_2TH_REBIRTH_COUNT_01, (LPCTSTR)m_szNickName, m_bRebirth-6 );

		g_pUIManager->SetString(WINDOW_CHARACTER, character_window_contents_dummy_001, strName);
	}
	
	//150갑자 초과� 
	if(m_wLevel < 150)
	{
        g_pUIManager->SetString(WINDOW_CHARACTER, character_window_contents_dummy_003, m_wLevel);
	}
	else
	{
		g_pUIManager->SetString(WINDOW_CHARACTER, character_window_contents_dummy_003, IDS_BESTLEVEL, 9);
	}

	g_pUIManager->SetString(WINDOW_CHARACTER, character_window_dummy_05, m_wStr);
	g_pUIManager->SetString(WINDOW_CHARACTER, character_window_dummy_06, m_wSus);
	g_pUIManager->SetString(WINDOW_CHARACTER, character_window_dummy_07, m_wDex);
	g_pUIManager->SetString(WINDOW_CHARACTER, character_window_dummy_08, m_wVit);
	g_pUIManager->SetString(WINDOW_CHARACTER, haracter_window_dummy_10, m_wRemainSp);

	g_pUIManager->SetString(WINDOW_CHARACTER, character_window_contents_dummy_005, atk);
	g_pUIManager->SetString(WINDOW_CHARACTER, character_window_contents_dummy_006, def);
	g_pUIManager->SetString(WINDOW_CHARACTER, character_window_contents_dummy_007, atkrat);
	g_pUIManager->SetString(WINDOW_CHARACTER, character_window_contents_dummy_008, plusspeed);
	g_pUIManager->SetString(WINDOW_CHARACTER, character_window_contents_dummy_009, critical);
	g_pUIManager->SetString(WINDOW_CHARACTER, character_window_contents_dummy_010, strHp);
	g_pUIManager->SetString(WINDOW_CHARACTER, character_window_contents_dummy_011, strIp);

	if (g_pMainChar)
	{
		int activeTitleID = (int)g_MainCharInfo.wEquipVisualID[8];

		TCHAR szTitleName[128] = _T("");
		if (activeTitleID > 0)
		{
			TCHAR szKey[32];
			_stprintf(szKey, _T("%d"), activeTitleID);
			GetPrivateProfileString(_T("TITLE_NAME"), szKey, _T(""), szTitleName, 128, _T(".\\config.ini"));

			if (_tcslen(szTitleName) == 0)
			{
				_stprintf(szKey, _T("%d"), activeTitleID - 400);
				GetPrivateProfileString(_T("TITLE_NAME"), szKey, _T(""), szTitleName, 128, _T(".\\config.ini"));
			}

			if (_tcslen(szTitleName) == 0)
			{
				_stprintf(szTitleName, _T("TitleIcon_%d"), activeTitleID);
			}
		}

		g_pUIManager->SetString(WINDOW_CHARACTER, 50, szTitleName);

		// TCHAR szDbg[256];
		// _stprintf(szDbg, _T("[DebugTitle] ID:%d, Name:%s"), activeTitleID, szTitleName);
		// g_MainCharInfo.ShowHelpMessage(szDbg);
	}

	RefreshTime2();
}

void CharacterInfo::RefreshPetInfo()
{	
	sPetInfo* pPetInfo = g_PetList.GetCurrentPet();
	
	if( !pPetInfo)
		return;

	TCHAR szHp[50]={0,};
	TCHAR szExp[64]={0,};
	
	_stprintf( szHp, _T("%d / %d"), pPetInfo->dwHpCur, pPetInfo->dwHpMax);

	INT64 i64MAXLevelExp = pPetInfo->i64NextLevelUpExp - pPetInfo->i64LevelExp;
	INT64 i64SUBLevelExp = pPetInfo->i64Exp - pPetInfo->i64LevelExp;

	//HT_0625 : � 경험� 150갑자 � 경우(우선 이렇� � 놓자.. 기획에서 확정 � 되었�)
	if(pPetInfo->m_dwIsHwan != 0) //HO_0820_07 분신�, 환수� 일경� 무조� -/- 경치 표시�..
	{
		_stprintf( szExp, _T("- / -"));
	}
	else
	{
		if(pPetInfo->wLevel == 150)
			_stprintf( szExp, _T("- / -"));
		else
			_stprintf( szExp, _T("%I64d / %I64d"), i64SUBLevelExp, i64MAXLevelExp);
	}

#ifdef TRACE_LOG
	if(pFrame == NULL)
	{
		DBG_LogFile( _T("RefreshPetInfo에서 g_pUIManager->GetFrame 실패"));
	}
#endif

	if(g_pUIManager->IsShow(WINDOW_NEW_TAMING))
		g_pUIManager->SetReleaseFocus(WINDOW_NEW_TAMING, taming_window_name_edit);

	g_pUIManager->SetString(WINDOW_NEW_TAMING, taming_window_name_edit, (LPCTSTR)pPetInfo->szName);
	g_pUIManager->SetString(WINDOW_NEW_TAMING, taming_window_contents_2_dummy_01, pPetInfo->wLevel);
	g_pUIManager->SetString(WINDOW_NEW_TAMING, taming_window_contents_2_dummy_02, szHp);
	g_pUIManager->SetString(WINDOW_NEW_TAMING, taming_window_contents_2_dummy_03, szExp);
	g_pUIManager->SetString(WINDOW_NEW_TAMING, taming_window_contents_2_dummy_04, pPetInfo->bWildRate);
	g_pUIManager->SetString(WINDOW_NEW_TAMING, taming_window_contents_2_dummy_05, pPetInfo->wAtkPwr);
	g_pUIManager->SetString(WINDOW_NEW_TAMING, taming_window_contents_2_dummy_06, pPetInfo->wDefPwr);
	g_pUIManager->SetString(WINDOW_NEW_TAMING, taming_window_contents_2_dummy_07, pPetInfo->wAtkRating);
	g_pUIManager->SetString(WINDOW_NEW_TAMING, taming_window_contents_2_dummy_08, _T("0"));
}

/**
 * 정보 갱신
 */
void CharacterInfo::RefreshMainFrame()
{
	BYTE bHpPer = 0;
	BYTE bIpPer = 0;
	int bLevelExpPer = 0;
	int bTPExpPer = 0;

	if( m_dwHpMax == 0)
		bHpPer = 100;
	else
		bHpPer	= 100 * m_dwHpCur / m_dwHpMax;

	if( m_wIpMax == 0)
		bIpPer = 100;
	else
		bIpPer	= 100 * m_wIpCur / m_wIpMax;
			
	INT64 i64MAXLevelExp = m_i64NextLevelUpExp - m_i64LevelExp;
	INT64 i64SUBLevelExp = m_i64Exp - m_i64LevelExp;

	INT64 i64MAXTPExp = m_i64NextTpUpExp - m_i64TpExp;
	INT64 i64SUBTPExp = m_i64Exp - m_i64TpExp;

	if( i64SUBLevelExp < 1)
		i64SUBLevelExp = 0;

	if( i64SUBTPExp < 1)
		i64SUBTPExp = 0;

	//150갑자 초과� 
	if( i64MAXLevelExp < 1)
		bLevelExpPer = 100;
	else if(m_wLevel >= 150)
		bLevelExpPer = 0;
	else
		bLevelExpPer = 100 * i64SUBLevelExp / i64MAXLevelExp;	

	//150갑자 초과� 
	if( i64MAXTPExp < 1)
		bTPExpPer = 100;
	else if(m_wLevel >= 150)
		bTPExpPer = 0;
	else
		bTPExpPer = 100 * i64SUBTPExp / i64MAXTPExp;	

	if( bHpPer > 100)
		bHpPer = 100;
	if( bIpPer > 100)
		bIpPer = 100;

	if( bLevelExpPer >= 100)
		bLevelExpPer = 100;
	else if( bLevelExpPer < 1)
		bLevelExpPer = 0;
		
	if( bTPExpPer >= 100)
		bTPExpPer = 100;
	else if( bTPExpPer < 1)
		bTPExpPer = 0;

	g_pUIManager->SetData(MAIN_FRAME, main_frame_outside_gauge, VALUE1, bHpPer);
	g_pUIManager->SetData(MAIN_FRAME, main_frame_inside_gauge, VALUE1, bIpPer);

	g_pUIManager->SetData(MAIN_FRAME, main_frame_level_gauge, VALUE1, bLevelExpPer);
	g_pUIManager->SetData(MAIN_FRAME, main_frame_skill_gauge, VALUE1, bTPExpPer);
	
	TCHAR szIpPer[50] = {0,};
	TCHAR szHpPer[50] = {0,};
	TCHAR szLevelExpPer[50] = {0,};
	TCHAR szTPExpPer[50] = {0,};
	TCHAR strName[128] = {0,};

	if(m_bRebirth)
	{		
		if(m_bRebirth < 7)	//HT_0702 : 각성�
			_stprintf( strName, IDS_REBIRTH_COUNT_01, (LPCTSTR)m_szNickName, m_bRebirth);
		else				//진각성자
			_stprintf( strName, IDS_2TH_REBIRTH_COUNT_01, (LPCTSTR)m_szNickName, m_bRebirth-6);

		g_pUIManager->SetString(WINDOW_CHARACTER, character_window_contents_dummy_001, strName);
	}

	float fLevelExpPer  = 100.0 * (float)i64SUBLevelExp / (float)i64MAXLevelExp;
	float fTPExpPer		= 100.0 * (float)i64SUBTPExp / (float)i64MAXTPExp;

	_stprintf( szIpPer, _T("%d / %d"), m_wIpCur, m_wIpMax);
	_stprintf( szHpPer, _T("%d / %d"), m_dwHpCur, m_dwHpMax);
	_stprintf( szLevelExpPer, _T("%3.2f %%"), fLevelExpPer);
	_stprintf( szTPExpPer, _T("%3.2f %%"), fTPExpPer);

	g_pUIManager->SetString(MAIN_FRAME, main_frame_gauge_point_dummy_01, szIpPer, GetFont(IDS_FONT_GULIM, 12));
	g_pUIManager->SetString(MAIN_FRAME, main_frame_gauge_point_dummy_02, szHpPer, GetFont(IDS_FONT_GULIM, 12));

#ifdef _CHINA_
	pFrame->GetControl( percentmain_frame_percent_gauge_01)->SetString( szLevelExpPer, GetFont( IDS_FONT_GULIM, 12));
	pFrame->GetControl( percentmain_frame_percent_gauge_02)->SetString( szTPExpPer, GetFont( IDS_FONT_GULIM, 12));
#else

	//150갑자 초과�
	if(m_wLevel < 150)
	{
		g_pUIManager->SetString(MAIN_FRAME, percentmain_frame_percent_gauge_01, szLevelExpPer, GetFont(IDS_FONT_GULIM, 10));
		g_pUIManager->SetString(MAIN_FRAME, percentmain_frame_percent_gauge_02, szTPExpPer, GetFont(IDS_FONT_GULIM, 10));
	}
#endif

}

void CharacterInfo::RefreshChatFrame( DWORD sender, BYTE type, sString content, sString senderName, DWORD listner, sString listnerName)
{
	switch( type)
	{
	case CT_BROADCAST:
		{
			if(m_bEvSocketItemUse)
			{
				m_pHelpMsg->AllDeleteScrMsg();

				if(m_pScrMsg)
					m_pScrMsg->SetScrMsgSetRect(type, content);
			}
			else
			{
				if(m_pScrMsg)
					m_pScrMsg->SetScrMsg( type, content);
			}
		}
		return;
	case CT_GETEVENTITEM:
		{
			if(m_pScrMsg)
				m_pScrMsg->SetScrMsg(CT_BROADCAST, content, 1);
		}		
		return;
	case CT_NORMAL:
		{
			if(!m_bNormalChatShow)
				return;
		}		
		break;
	case CT_WHISPER:
		{
			if(!g_info.m_bAllowWhisper)
				return;
		}		
		break;
	case CT_MUNPA_BROADCAST:
	case CT_MUNPA_MUNJUSHOUT:
		{
			if(!m_bMunpaChatShow)
				return;
		}		
		break;
	case CT_DAN:
		{
			if(!m_bDanChatShow)
				return;
		}		
		break;
	}

	if(m_pChat)
		m_pChat->SetChatMsg( sender, type, content, senderName, listner, listnerName);
}

int CharacterInfo::MoneyUnitColor(DWORD dwMoney)
{
	int nType=0;
	{
		if(dwMoney >= 100000 && dwMoney < 1000000)
		{
			nType=9;
		}
		else if(dwMoney >= 1000000 && dwMoney < 10000000)
		{
			nType=10;
		}
		else if(dwMoney >= 10000000 && dwMoney < 100000000)
		{
			nType=11;
		}
		else if(dwMoney >= 100000000 && dwMoney < 1000000000)
		{
			nType=12;
		}
		else if(dwMoney >= 1000000000 && dwMoney < 10000000000)
		{
			nType=13;
		}
	}
    return nType;
}


void CharacterInfo::RefreshItemFrame()
{
	g_pUIManager->SetString(DRG_ITEM_WINDOW, drg_item_window_money_dummy_01, MoneyCommaStr(m_dwMoney), MoneyUnitColor(m_dwMoney));
	
	for( int i=0; i < 2; ++i)
	{
		if( m_pMySack[i])
			m_pMySack[i]->SetRefreshToolTip( TRUE);
	}

	if( m_pEquipSack)
		m_pEquipSack->SetRefreshToolTip( TRUE);
}

void CharacterInfo::RefreshMugongFrame( BOOL bSmallChange)
{
	g_pUIManager->SetString(WINDOW_OUTSIDE, outside_window_training_point_name_02, m_wRemainTp);
	g_pUIManager->SetString(WINDOW_INSIDE, inside_window_training_point_dummy_02, m_wRemainTp);
	g_pUIManager->SetString(WINDOW_SKILL, skill_window_training_point_name_02, m_wRemainTp);

	if( m_pMugong && bSmallChange)
		m_pMugong->RefreshMugongContent();
}

void CharacterInfo::RefreshSituation()
{
	if( g_pMainChar)
	{
		CXiahCharObject* pMainChar = (CXiahCharObject*)g_pMainChar->m_pObject;

		if(pMainChar)
		{
			LPCTSTR lpStrName = GetMapName(XiahMap::g_XiahMap.m_MapInfo.m_dwMapID);

			// 좌표
			WORD nPosX;
			WORD nPosY;

			pMainChar->GetPosition( nPosX, nPosY);

			TCHAR str[50]={0,};
			_stprintf( str, _T(" %s %d,%d"), lpStrName, nPosX/4, nPosY/4);

			g_pUIManager->SetString(SITUATION_BRA, situation_bar_dummy, str);
		}
		else
		{
			DBG_LogFile( _T("RefreshSituation 실패"));
		}
	}
}

void CharacterInfo::RefreshTime1( WORD wYear, BYTE bMonth, BYTE bDay, BYTE bHour)
{

	TCHAR szDate[50]={0,};
	
	_stprintf( szDate, IDS_D_DATE, wYear, bMonth, bDay, bHour);

	g_pUIManager->SetString(DATA_WINDOW, date_window_data_dummy, szDate);
}

void CharacterInfo::RefreshTime2()
{
	if(!g_pUIManager->IsShow(DATA_WINDOW))
		return;

	TCHAR szLevel[50]={0,};
	TCHAR strName[128]={0,};

	//150갑자 초과� 
	if(m_wLevel<150)
	{
        _stprintf( szLevel, IDS_D_GAPJA, m_wLevel);
		g_pUIManager->SetString(DATA_WINDOW, date_window_level_dummy, szLevel);
	}
	else
	{
		m_wLevel=150;
		_stprintf( szLevel, IDS_BESTLEVEL);
		g_pUIManager->SetString(DATA_WINDOW, date_window_level_dummy, szLevel,9);
	}

}



void CharacterInfo::RefreshQuest()
{
	if(!g_pUIManager->IsShow(WINDOW_QUEST_01))
		return;

	m_pQuest->Refresh();
}

/**
 * 오행 갱신
 */
void CharacterInfo::RefreshFiveElement()
{
	g_pUIManager->SetString(WINDOW_FIVEELEMENTS, fiveelements_window_exp_dummy, m_wFiveElmPoint);

	g_pUIManager->SetString(WINDOW_FIVEELEMENTS, fiveelements_window_exp_dummy_01, m_wFiveElmExp[0]);
	g_pUIManager->SetString(WINDOW_FIVEELEMENTS, fiveelements_window_exp_dummy_02, m_wFiveElmExp[1]);
	g_pUIManager->SetString(WINDOW_FIVEELEMENTS, fiveelements_window_exp_dummy_03, m_wFiveElmExp[2]);
	g_pUIManager->SetString(WINDOW_FIVEELEMENTS, fiveelements_window_exp_dummy_04, m_wFiveElmExp[3]);
	g_pUIManager->SetString(WINDOW_FIVEELEMENTS, fiveelements_window_exp_dummy_05, m_wFiveElmExp[4]);

	int nExp =0;
	TCHAR strTemp[32] = {0,};
	WORD FiveElmTemp = 0;

	for(int i=0; i<5; i++)
	{
		FiveElmTemp+=m_wFiveElmExp[i];
	}
	FiveElmTemp+=m_wFiveElmPoint;
    
	//HT_0711 : 진각� 무공( 오행 MAX 2000� )
	if(FiveElmTemp>=10000)
	{
		g_pUIManager->SetData(WINDOW_FIVEELEMENTS, fiveelements_window_exp_bar,VALUE1,0);
		g_pUIManager->SetString(WINDOW_FIVEELEMENTS, fiveelements_window_exp_percent_dummy, _T(" "));
		g_pUIManager->SetString(WINDOW_FIVEELEMENTS, fiveelements_window_exp_dummy, m_wFiveElmPoint);
	}
	else
	{
		if(m_dwFiveElmPowerMax)	// 0 나누� 무섭�
		{
			nExp = 100 * m_dwFiveElmPower / m_dwFiveElmPowerMax;
			g_pUIManager->SetData(WINDOW_FIVEELEMENTS, fiveelements_window_exp_bar, VALUE1, nExp);

			float fExp = 100.0f * (float)m_dwFiveElmPower / (float)m_dwFiveElmPowerMax;
			_stprintf(strTemp, _T("%.2f%%"), fExp);
			g_pUIManager->SetString(WINDOW_FIVEELEMENTS, fiveelements_window_exp_percent_dummy, strTemp);
		}
	}

	// 필살�
	nExp = 100 * m_dwFiveElmGauge / 5000;
	g_pUIManager->SetData(WINDOW_FIVEELEMENTS, fiveelements_window_skill_bar, VALUE1, nExp);
	
	_stprintf(strTemp, _T("%d %%"), nExp);
	g_pUIManager->SetString(WINDOW_FIVEELEMENTS, fiveelements_window_skill_percent_dummy, strTemp);
}

