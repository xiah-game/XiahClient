#include ".\munpamark.h"

int OnCS_RL_CREATEMUNPA_ACK( CMsg &msg)
{
	BYTE bResult	=0;	

	msg
		>> bResult;

	switch( bResult)
	{
	case ERR_CREATEMUNPA_SUCC:
		{
			DWORD dwClanID	=0;
			sString szClanName;

			msg
				>> dwClanID
				>> szClanName;

			g_MainCharInfo.m_pRelation->SetClanInfo( dwClanID, szClanName);

			g_pUIManager->Hide(WINDOW_MUNPA_FOUND);
			g_pUIManager->Show(WINDOW_MUNPA);

			g_MainCharInfo.ShowHelpMessage( IDS_CLAN_MADE);
		}		
		break;
	case ERR_CREATEMUNPA_FAIL:
		g_MainCharInfo.ShowHelpMessage( IDS_CLAN_CREATE_FAIL);
		break;
	case ERR_YOU_CANNOT_CREATEMUNPA:
		g_MainCharInfo.ShowHelpMessage( IDS_CLAN_CREATE_FAIL);
		break;
	case ERR_DUPLICATE_MUNPANAME:
		g_MainCharInfo.ShowHelpMessage( IDS_EXIST_CLAN);
		break;
	case ERR_YOU_NOT_LEVEL:
		g_MainCharInfo.ShowHelpMessage( IDS_NO_CLAN_GABJA);
		break;
	case ERR_YOU_NOT_MONEY:
		g_MainCharInfo.ShowHelpMessage( IDS_NO_CLAN_MOENY);
		break;
	case ERR_YOU_NOT_SKILLPOINT:
		g_MainCharInfo.ShowHelpMessage( RL_WARNNING1);
		break;
	case 7:
		g_MainCharInfo.ShowHelpMessage( RL_WARNNING2);
		break;
	}

	return 0;
}

/**
 * 문파폐쇄
 * \param &msg 
 * \return 
 */
int OnCS_RL_DELETEMUNPA_ACK( CMsg &msg)
{
	BYTE bResult =0;

	msg
		>> bResult;

	switch( bResult)
	{
	case ERR_DELETEMUNPA_SUCC:
		g_MainCharInfo.m_pRelation->ClearClan();
		g_MainCharInfo.ShowHelpMessage(IDS_CLAN_CLOSE);
		break;
	case ERR_DELETEMUNPA_NOMUNPA:
		{
			g_MainCharInfo.ShowHelpMessage(IDS_DELETEMUNPA_NOMUNPA, TEXTEFFECT_COLOR_WARNING);

			/*
			TCHAR str[50];
			_stprintf( str, IDS_ERROR, ERR_DELETEMUNPA_NOMUNPA);
			g_MainCharInfo.ShowHelpMessage(str,TEXTEFFECT_COLOR_WARNING);
			*/
		}
		break;
	case ERR_DELETEMUNPA_NOPOWER:
		g_MainCharInfo.ShowHelpMessage(IDS_NO_AUTH,TEXTEFFECT_COLOR_WARNING);
		break;
	case ERR_DELETEMUNPA_HASMUNWON:
		g_MainCharInfo.ShowHelpMessage(IDS_CANNOT_CLOSE_MUNWON,TEXTEFFECT_COLOR_WARNING);
		break;
	case ERR_DELETEMUNPA_FAIL:
		{
			TCHAR str[50];
			_stprintf( str, IDS_ERROR, ERR_DELETEMUNPA_FAIL);
			g_MainCharInfo.ShowHelpMessage(str,TEXTEFFECT_COLOR_WARNING);
		}
		break;
	case 5:	// 전쟁중
		g_MainCharInfo.ShowHelpMessage(IDS_MUNPADEL_WARFAIL, TEXTEFFECT_COLOR_WARNING);		
		break;
	case 6:	// 비석소유
		g_MainCharInfo.ShowHelpMessage(IDS_MUNPADEL_STONE, TEXTEFFECT_COLOR_WARNING);		
		break;
	}

	return 0;
}

/**
 * 문파원가입
 * \param &msg 
 * \return 
 */
int OnCS_RL_ASKMUNWON_ACK( CMsg &msg)
{
	BYTE bResult	=0;
	DWORD dwAskID	=0;
	DWORD dwAskedID	=0;

	msg
		>> bResult
		>> dwAskID
		>> dwAskedID;

	switch( bResult)
	{
	case ACT_ASKMUNWON_REQUEST:
		if( dwAskedID == g_MainCharInfo.m_dwObjectID) 
		{
			g_MainCharInfo.m_dwAskID = dwAskID;

			TCHAR szText[100] = {0,};

			_stprintf( szText, IDS_D_ASK_CLAN, (LPCTSTR)g_MainCharInfo.FindNameByID( dwAskID));
			if( !g_pUIManager->ShowNotice( szText, NOTICE_FRAME_OKCANCEL, NOTICE_FRAME_CLAN))
			{
				// 거절
				SendCS_RL_ASKMUNWON_REQ( ACT_ASKMUNWON_CANCEL, g_MainCharInfo.m_dwAskID, g_MainCharInfo.m_dwObjectID);
			}
		}
		break;
	case ACT_ASKMUNWON_OK:
		break;
	case ACT_ASKMUNWON_CANCEL:
		if( dwAskID == g_MainCharInfo.m_dwObjectID)
		{
			g_MainCharInfo.ShowHelpMessage( IDS_REJECT_CLAN, TEXTEFFECT_COLOR_WARNING);
		}
		break;
	case 4:
		g_MainCharInfo.ShowHelpMessage( IDS_CLAN_ALREADY_JOIN,TEXTEFFECT_COLOR_WARNING);
		break;
	case 5:
		g_MainCharInfo.ShowHelpMessage( IDS_REQUEST_PERSON_NOT_JOIN,TEXTEFFECT_COLOR_WARNING);
		break;
	case ACT_ASKMUNWONSTONE_REQUEST:
		{
			sString strName;

			msg
				>> strName;

			if(dwAskedID == g_MainCharInfo.m_dwObjectID) 
			{
				g_MainCharInfo.m_dwAskID = dwAskID;

				TCHAR szText[100] = {0,};

				_stprintf( szText, IDS_D_ASK_CLAN, (LPCTSTR)strName);
				if(!g_pUIManager->ShowNotice( szText, NOTICE_FRAME_OKCANCEL, NOTICE_FRAME_CLAN))
				{
					// 거절
					SendCS_RL_ASKMUNWON_REQ( ACT_ASKMUNWON_CANCEL, g_MainCharInfo.m_dwAskID, g_MainCharInfo.m_dwObjectID);
				}
			}
		}
		break;
	case 7:		// 현재 미 접속
		g_MainCharInfo.ShowHelpMessage(IDS_MUNPA_MUNJU_DONOT, TEXTEFFECT_COLOR_WARNING);	
		break;
	case 8:		// 옵션으로 관계 거절
		g_MainCharInfo.ShowHelpMessage(IDS_MUNPA_JOIN_OPTION_NO, TEXTEFFECT_COLOR_WARNING);		
		break;
	}
	
	return 0;
}

/**
 * 문원추가
 * \param &msg 
 * \return 
 */
int OnCS_RL_ADDMUNWON_ACK(CMsg &msg)
{
	BYTE bResult		=0;

	msg
		>> bResult;

	if(bResult)
	{
		switch(bResult)
		{
		case ERR_ADDMUNWON_YOU_HAVENOT_MUNPA:
			g_MainCharInfo.ShowHelpMessage(IDS_NO_CLAN, TEXTEFFECT_COLOR_WARNING);
			break;
		case ERR_ADDMUNWON_YOU_HAVENOT_POWER:
			g_MainCharInfo.ShowHelpMessage(IDS_NO_AUTH, TEXTEFFECT_COLOR_WARNING);
			break;
		case ERR_ADDMUNWON_DUPLICATION:
			g_MainCharInfo.ShowHelpMessage(IDS_EXIST_MUNWON, TEXTEFFECT_COLOR_WARNING);
			break;
		case ERR_ADDMUNWON_DONOT_FIND:
			g_MainCharInfo.ShowHelpMessage(IDS_CANNOT_FIND, TEXTEFFECT_COLOR_WARNING);
			break;
		case ERR_ADDMUNWON_ERROR:
			g_MainCharInfo.ShowHelpMessage(IDS_ERROR1, TEXTEFFECT_COLOR_WARNING);
			break;
		case ERR_ADDMUNWON_MAX:	// 문원수 많을때
			g_MainCharInfo.ShowHelpMessage(IDS_ADDMUNWON_MAX, TEXTEFFECT_COLOR_WARNING);
			break;
		case ERR_ADDMUNWON_PENALTY:	//HO_0507_07 문파 패킷 추가 : 탈퇴한지 1주일이 안지남
			g_MainCharInfo.ShowHelpMessage(IDS_ADDMUNWON_PENALTY, TEXTEFFECT_COLOR_WARNING);
			break;
		} // switch( bResult)

		return 0;
	}

	DWORD dwMunpaID		=0;
	DWORD dwOrderID		=0;
	DWORD dwCharID		=0;
	sString szOrderName;
	sString szCharName;

	msg
		>> dwMunpaID
		>> dwOrderID
		>> szOrderName
		>> dwCharID
		>> szCharName;

	if( dwCharID != g_MainCharInfo.m_dwObjectID)
	{
		g_MainCharInfo.m_pRelation->InsertClan( dwOrderID, szOrderName, dwCharID, szCharName, _T(""), 0, 1, 99);

		// 更新新成员角色对象的门派视觉信息（头顶门派名）
		XiahObject::CXiahObject* pObject = XiahObject::g_XiahObjectManager.FindXiahObject( MAKEOBJECTID( 0, dwCharID, OBJTYPE_PC));
		if(pObject && pObject->m_pObject)
		{
			CXiahCharObject* pCharObject = (CXiahCharObject*)pObject->m_pObject;
			pCharObject->m_dwMunpaID = dwMunpaID;
			pCharObject->m_szMunpaName = g_MainCharInfo.m_pRelation->GetClanName();
			pCharObject->m_dwMunpaOrder = dwOrderID;
			pCharObject->RefreshFameColor();
		}

		// 전서구 (문파 추가) 나를 제외하고 넣자
		if( dwCharID != g_MainCharInfo.m_dwObjectID)
			g_Mail.Add_SendList(dwCharID,szCharName,MAIL_MUNPA,(BYTE)dwOrderID);

		TCHAR szText[100] = {0,};
		_stprintf( szText, IDS_D_JOIN_CLAN, (LPCTSTR)szCharName);
		
		g_MainCharInfo.ShowHelpMessage( szText);
	}
	else
	{
		SendCS_RL_MUNWONLIST_REQ();

		TCHAR szText[100] = {0,};
		_stprintf( szText, IDS_D_JOINED_CLAN, (LPCTSTR)szCharName);
		
		g_MainCharInfo.ShowHelpMessage( szText);

		if( g_pUIManager->IsShow(WINDOW_MUNPA_FOUND))
		{
			g_pUIManager->Hide(WINDOW_MUNPA_FOUND);
			g_pUIManager->SetString(WINDOW_MUNPA, munpa_window_button_03, IDS_TALTE);
			g_pUIManager->Show(WINDOW_MUNPA);
		}
	}

	return 0;
}

int OnCS_RL_DELMUNWON_ACK( CMsg &msg)
{
	BYTE bResult;
	DWORD dwClanID;
	DWORD dwOrderID;
	DWORD dwCharID;
	sString szCharName;

	msg
		>> bResult;

	if( bResult)
	{
		switch( bResult)
		{
		case ERR_DELMUNWON_YOU_HAVENOT_MUNPA:
			g_MainCharInfo.ShowHelpMessage( IDS_NO_CLAN,TEXTEFFECT_COLOR_WARNING);
			break;
		case ERR_DELMUNWON_YOU_HAVENOT_POWER:
			g_MainCharInfo.ShowHelpMessage( IDS_NO_AUTH,TEXTEFFECT_COLOR_WARNING);
			break;
		case ERR_DELMUNWON_DONOT_FIND:
			g_MainCharInfo.ShowHelpMessage( IDS_CANNOT_FIND_MUNWON,TEXTEFFECT_COLOR_WARNING);
			break;
		case ERR_DELMUNWON_ERROR:
			g_MainCharInfo.ShowHelpMessage( IDS_ERROR1,TEXTEFFECT_COLOR_WARNING);
			break;
		case ERR_DELMUNWON_DELETED:
			g_MainCharInfo.ShowHelpMessage( IDS_DELETED_MUNWON,TEXTEFFECT_COLOR_WARNING);
			break;
		case ERR_DELMUNWON_WARENTRY:
			g_MainCharInfo.ShowHelpMessage( IDS_WARENTRY,TEXTEFFECT_COLOR_WARNING);
			break;
		}
		return 0;
	}

	msg
		>> dwClanID
		>> dwOrderID
		>> dwCharID;

	if( dwCharID == g_MainCharInfo.m_dwObjectID)
	{
		g_MainCharInfo.m_pRelation->ClearClan();
		g_MainCharInfo.ShowHelpMessage( IDS_CLAN_TALTED,TEXTEFFECT_COLOR_WARNING);

		// 清除角色3D对象上的门派视觉信息（头顶名称、光圈等）
		XiahObject::CXiahObject* pObject = XiahObject::g_XiahObjectManager.FindXiahObject( MAKEOBJECTID( 0, g_MainCharInfo.m_dwObjectID, OBJTYPE_PC));
		if(pObject && pObject->m_pObject)
		{
			CXiahCharObject* pCharObject = (CXiahCharObject*)pObject->m_pObject;
			pCharObject->m_dwMunpaID = 0;
			pCharObject->m_szMunpaName = _T("");
			pCharObject->m_szMunpaNickName = _T("");
			pCharObject->m_dwMunpaOrder = 0;
			pCharObject->m_dwMunpaMarkID = 0;
		}

		g_pUIManager->Hide(WINDOW_MUNPA);
		g_pUIManager->Show(WINDOW_MUNPA_FOUND);
	}
	else
	{
		g_MainCharInfo.m_pRelation->DeleteClan( dwCharID);
		//g_MainCharInfo.ShowHelpMessage( IDS_CLAN_TALTE);

		// 전서구 리스트에서 뺀다
		g_Mail.Delete_SendMailList(dwCharID);
	}

	return 0;
}

/**
 *
 * \param &msg 
 * \return 
 */
int OnCS_RL_MUNWONINFO_ACK( CMsg &msg)
{	
	DWORD dwCharID		=0;
	DWORD dwOrderID		=0;	
	WORD wCharLev		=0;
	BYTE bState			=0;
	BYTE bCharType		=0;
    sString szOrderName;
	sString szCharName;
	sString szMunpaNickName;

	msg
		>> dwCharID
		>> szCharName
		>> dwOrderID
		>> szOrderName
		>> szMunpaNickName	
		>> bState
		>> wCharLev
		>> bCharType;

	sClanWonInfo* pInfo = g_MainCharInfo.m_pRelation->FindClanInfoByID( dwCharID);

	if(pInfo)
	{
		pInfo->m_dwOrderID		= dwOrderID;
		pInfo->m_szOrderName	= szOrderName;
		pInfo->m_wCharLev		= wCharLev;
		pInfo->m_szMunpaNickName = szMunpaNickName;
		pInfo->m_bState			= bState;
		pInfo->m_bCharType		= bCharType;		// 유파
	}

	g_MainCharInfo.m_pRelation->RefreshClanContent();

	return 0;
}

/**
 *
 * \param &msg 
 * \return 
 */
int OnCS_RL_MUNWONINFO2_ACK( CMsg &msg)
{
	DWORD	dwCharID		=0;
	DWORD	dwMunpaID		=0;	
	DWORD	dwMunpaOrder	=0;		
	DWORD	dwEnemyMunpaID	=0;
	DWORD	dwEnemyStoneID	=0;
	DWORD	dwMarkID		=0;
	BYTE	bWarStatus		=0;
	sString szMunpaName;
	sString szMunpaNick;
	sString szNickName;
	sString strEnemyMunpaName;


	msg
		>> dwCharID
		>> dwMunpaID
		>> szMunpaName
		>> dwMunpaOrder
		>> szMunpaNick
		>> szNickName
		>> bWarStatus
		>> dwEnemyMunpaID
		>> strEnemyMunpaName
		>> dwEnemyStoneID
		>> dwMarkID;

	XiahObject::CXiahObject* pObject = XiahObject::g_XiahObjectManager.FindXiahObject( MAKEOBJECTID( 0, dwCharID, OBJTYPE_PC));

	if( pObject)
	{
		CXiahCharObject* pCharObject = (CXiahCharObject*)pObject->m_pObject;

		if( pCharObject)
		{
			pCharObject->m_dwMunpaID		= dwMunpaID;
			pCharObject->m_szMunpaName		= szMunpaName;
			pCharObject->m_dwMunpaOrder		= dwMunpaOrder;
			pCharObject->m_szMunpaNickName	= szMunpaNick;

			pCharObject->m_bWarStatus		= bWarStatus;
			pCharObject->m_dwEnemyMunpaID	= dwEnemyMunpaID;
			pCharObject->m_strEnemyMunpaName = strEnemyMunpaName;
			pCharObject->m_dwEnemyStoneID	= dwEnemyStoneID;

			pCharObject->m_dwMunpaMarkID	= dwMarkID;
		}
	}

	return 0;
}


int OnCS_RL_MUNWONLIST_ACK( CMsg &msg)
{
	DWORD dwMunpaID			=0;
	sString szMunpaName;
	DWORD dwCount			=0;
	DWORD dwOrderID			=0;
	sString szOrderName;
	DWORD dwCharID			=0;
	sString szCharName;
	sString szMunpaNickName;
	BYTE bState				=0;
	BYTE bCharType			=0;

	msg
		>> dwMunpaID
		>> szMunpaName
		>> dwCount;

	g_MainCharInfo.m_pRelation->SetClanInfo( dwMunpaID, szMunpaName);

	for(int i=0; i < dwCount; ++i)
	{
		msg
			>> dwOrderID
			>> szOrderName
			>> dwCharID
			>> szCharName
			>> szMunpaNickName
			>> bState
			>> bCharType;

		g_MainCharInfo.m_pRelation->InsertClan( dwOrderID, szOrderName, dwCharID, szCharName, szMunpaNickName, 0, bState, bCharType);

		// 전서구 (문파 추가) 나를 제외하고 넣자
		if( dwCharID != g_MainCharInfo.m_dwObjectID)
			g_Mail.Add_SendList(dwCharID,szCharName,MAIL_MUNPA,(BYTE)dwOrderID);

		XiahObject::CXiahObject* pObject = XiahObject::g_XiahObjectManager.FindXiahObject( MAKEOBJECTID( 0, dwCharID, OBJTYPE_PC));
		if( pObject)
		{
			CXiahCharObject* pCharObject = (CXiahCharObject*)pObject->m_pObject;

			if( pCharObject)
			{
				// 명성 등급 검사
				if(pCharObject->m_dwFame >= 127)
				{
					pCharObject->m_cNameColor = D3DCOLOR_XRGB( 200, 255, 200);
					pCharObject->RefreshFameColor();
				}
				else
				{
					pCharObject->RefreshFameColor();
				}
				
				pCharObject->m_dwMunpaID = dwMunpaID;
				pCharObject->m_szMunpaName = szMunpaName;
				pCharObject->m_dwMunpaOrder = dwOrderID;
			}
		}
	}

	return 0;
}

/**
 * 직위부여
 * \param &msg 
 * \return 
 */
int OnCS_RL_CHANGEMUNWONORDER_ACK( CMsg &msg)
{
	BYTE bResult	=0;
	DWORD dwCharID	=0;
	DWORD dwOrderID	=0;
	sString szOrderName;

	msg
		>> bResult
		>> dwCharID
		>> dwOrderID
		>> szOrderName;

	switch( bResult)
	{
	case ERR_CHANGEORDER_SUCCESS:
		{
			sClanWonInfo* pInfo = g_MainCharInfo.m_pRelation->FindClanInfoByID( dwCharID);

			if(pInfo)
			{
				if(dwOrderID == 1)
					g_MainCharInfo.m_pRelation->SetClanLeader(dwCharID);

				pInfo->m_dwOrderID = dwOrderID;
				pInfo->m_szOrderName = szOrderName;

				TCHAR temp[100] = {0,};
				_stprintf( temp, IDS_JOB_CHANGE, (LPCTSTR)pInfo->m_szCharName, (LPCTSTR)szOrderName);

				g_MainCharInfo.ShowHelpMessage(temp);
				g_MainCharInfo.m_pRelation->RefreshClanContent();
			}
		}
		break;
	case ERR_CHANGEORDER_YOU_HAVENOT_MUNPA:
		g_MainCharInfo.ShowHelpMessage(IDS_ERROR_NO_MUNPA,TEXTEFFECT_COLOR_WARNING);
		break;
	case ERR_CHANGEORDER_YOU_HAVENOT_POWER:
		g_MainCharInfo.ShowHelpMessage(IDS_NO_AUTH_JOB,TEXTEFFECT_COLOR_WARNING);
		break;
	case ERR_CHANGEORDER_NOTFIND_MUNWON:
		g_MainCharInfo.ShowHelpMessage(IDS_ERROR_NO_MUNWON,TEXTEFFECT_COLOR_WARNING);
		break;
	case ERR_CHANGEORDER_FULLORDER:
		g_MainCharInfo.ShowHelpMessage(IDS_OVER_NUMBER_JOB,TEXTEFFECT_COLOR_WARNING);
		break;
	case ERR_CHANGEORDER_ERROR:
		{
			TCHAR str[50] = {0,};
			_stprintf( str, IDS_ERROR, ERR_CHANGEORDER_ERROR);
			g_MainCharInfo.ShowHelpMessage(str,TEXTEFFECT_COLOR_WARNING);
		}
		break;
	case 6:	// 문파명성 100 이하
		g_MainCharInfo.ShowHelpMessage(IDS_MUNPA_FAME_LOW, TEXTEFFECT_COLOR_WARNING);
		break;
	case 7:	// 대상자 갑자 30이하시
		g_MainCharInfo.ShowHelpMessage(IDS_TARGET_LEVEL_LOW, TEXTEFFECT_COLOR_WARNING);
		break;
	case 8:	// 대상자 명성 10이하시
		g_MainCharInfo.ShowHelpMessage(IDS_TARGET_FAME_LOW, TEXTEFFECT_COLOR_WARNING);
		break;
	case 9: // 이양세 부족
		g_MainCharInfo.ShowHelpMessage(IDS_PURSE_IN_LOWMONEY, TEXTEFFECT_COLOR_WARNING);
		break;
	}
	
	return 0;
}


/**
 * 문파정보
 * \param &msg 
 * \return 
 */
int OnCS_RL_MUNPAINFO_ACK( CMsg &msg)
{
	BYTE bResult			=0;
	BYTE bStoneChannelID	=0;
	BYTE bLevel				=0;
	BYTE bWar				=0;
	DWORD dwMunpaID			=0;	
	DWORD dwMunjuID			=0;	
	DWORD dwStonID			=0;	
	DWORD dwMapID			=0;
	DWORD dwMunpaFame		=0;	
	DWORD dwRanking			=0;
	DWORD dwCanBattleTime	=0;	
	DWORD dwBattleTime		=0;	
	DWORD dwTotalWar		=0;
	DWORD dwWinWar			=0;
	DWORD dwDrawWar			=0;
	DWORD dwLossWar			=0;
	DWORD dwMunwonNum		=0;
	DWORD dwMunpaMarkID		=0;
	DWORD dwTaxMunpaMoney	=0;
	sString szMunpaName;
	sString strMunjuName;
	sString strEnemyMunpaName;
	sString strFriendMunpaName;
	sString strNotice;
    
	msg
		>> bResult
		>> dwMunpaID
		>> szMunpaName
		>> dwMunjuID
		>> strMunjuName
		>> dwStonID
		>> bStoneChannelID
		>> dwMapID
		>> dwMunpaFame
		>> bLevel
		>> dwRanking
		>> dwCanBattleTime
		>> strEnemyMunpaName
		>> strFriendMunpaName
		>> dwBattleTime
		>> bWar
		>> dwTotalWar
		>> dwWinWar
		>> dwDrawWar
		>> dwLossWar
		>> dwMunwonNum
		>> strNotice
		>> dwMunpaMarkID
		>> dwTaxMunpaMoney;

	if(g_MainCharInfo.m_pRelation->m_dwClanID == dwMunpaID)
	{
		g_MainCharInfo.m_dwMunpaFame = dwMunpaFame;

		if(bWar == 1)
		{
			TCHAR strTemp[256] = {0,};
			_stprintf(strTemp, IDS_MUNPA_WAR_WAIT, (LPCTSTR)szMunpaName, (LPCTSTR)strEnemyMunpaName);
			g_MainCharInfo.ShowHelpMessage(strTemp);
		}
		else if(bWar == 2)
		{
			TCHAR strTemp[256] = {0,};
			_stprintf(strTemp, IDS_MUNPA_WAR_START, (LPCTSTR)szMunpaName, (LPCTSTR)strEnemyMunpaName);
			g_MainCharInfo.ShowHelpMessage(strTemp);
		}
	} // if(g_MainCharInfo.m_pRelation->m_dwClanID == dwMunpaID)

	if(bResult == 0)
		return 0;

	g_MainCharInfo.OpenFrame(WINDOW_MUNPA_BBS_TOP);

	// 문파명
	g_pUIManager->SetString(WINDOW_MUNPA_BBS_TOP, munpa_bbs_top_dummy_02, szMunpaName, 5);

	//문주명
	g_pUIManager->SetString(WINDOW_MUNPA_BBS_TOP, munpa_bbs_top_dummy_04, strMunjuName, 5);

	TCHAR strText[256] = {0,};

	// 문원수
	_stprintf(strText, IDS_M_BBS_NUMBER, dwMunwonNum);
	g_pUIManager->SetString(WINDOW_MUNPA_BBS_TOP, munpa_bbs_top_dummy_06, strText, 5);

	int nRate = 20;
	if(dwMunpaFame <= 9)
		nRate = 20;
	else if(dwMunpaFame >= 10 && dwMunpaFame <= 19)
		nRate = 19;
	else if(dwMunpaFame >= 20 && dwMunpaFame <= 29)
		nRate = 18;
	else if(dwMunpaFame >= 30 && dwMunpaFame <= 39)
		nRate = 17;
	else if(dwMunpaFame >= 40 && dwMunpaFame <= 49)
		nRate = 16;
	else if(dwMunpaFame >= 50 && dwMunpaFame <= 59)
		nRate = 15;
	else if(dwMunpaFame >= 60 && dwMunpaFame <= 69)
		nRate = 14;
	else if(dwMunpaFame >= 70 && dwMunpaFame <= 79)
		nRate = 13;
	else if(dwMunpaFame >= 80 && dwMunpaFame <= 89)
		nRate = 12;
	else if(dwMunpaFame >= 90 && dwMunpaFame <= 99)
		nRate = 11;
	else if(dwMunpaFame >= 100 && dwMunpaFame <= 199)
		nRate = 10;
	else if(dwMunpaFame >= 200 && dwMunpaFame <= 299)
		nRate = 9;
	else if(dwMunpaFame >= 300 && dwMunpaFame <= 399)
		nRate = 8;
	else if(dwMunpaFame >= 400 && dwMunpaFame <= 499)
		nRate = 7;
	else if(dwMunpaFame >= 500 && dwMunpaFame <= 599)
		nRate = 6;
	else if(dwMunpaFame >= 600 && dwMunpaFame <= 699)
		nRate = 5;
	else if(dwMunpaFame >= 700 && dwMunpaFame <= 799)
		nRate = 4;
	else if(dwMunpaFame >= 800 && dwMunpaFame <= 899)
		nRate = 3;
	else if(dwMunpaFame >= 900 && dwMunpaFame <= 999)
		nRate = 2;
	else if(dwMunpaFame >= 1000)
		nRate = 1;

	// 등급
	_stprintf(strText, IDS_RATE, nRate);
	g_pUIManager->SetString(WINDOW_MUNPA_BBS_TOP, munpa_bbs_top_dummy_08, strText, 5);
	// 명성
	g_pUIManager->SetString(WINDOW_MUNPA_BBS_TOP, munpa_bbs_top_dummy_10, dwMunpaFame, 1);

	// 순위
	if(dwRanking == 0)		// 등외
		g_pUIManager->SetString(WINDOW_MUNPA_BBS_TOP, munpa_bbs_top_dummy_12, IDS_M_BBS_RANK_UNDER, 5);
	else
	{
		_stprintf(strText, IDS_M_BBS_RANK_NUM, dwRanking);
		g_pUIManager->SetString(WINDOW_MUNPA_BBS_TOP, munpa_bbs_top_dummy_12, strText, 5);
	}

	// 전적
	_stprintf(strText, IDS_M_BBS_RECORD	, dwTotalWar, dwWinWar, dwDrawWar, dwLossWar);
	g_pUIManager->SetString(WINDOW_MUNPA_BBS_TOP, munpa_bbs_top_dummy_14, strText, 5);

	LPSTR lpstrTemp;
	int nTime = GETHOUR(dwCanBattleTime);

	if(nTime <= 11)
		lpstrTemp = IDS_MORNING;
	else
	{
		lpstrTemp = IDS_AFTERNOON;
		nTime -= 12;
	}

	// 소유기간
	_stprintf(strText, IDS_M_BBS_WAR_DAY, GETMONTH(dwCanBattleTime)+1, GETDAY(dwCanBattleTime)+1,
		lpstrTemp, nTime);
	g_pUIManager->SetString(WINDOW_MUNPA_BBS_TOP, munpa_bbs_top_dummy_16, strText, 5);

	// 예정일시
	nTime = GETHOUR(dwBattleTime);

	if(nTime <= 11)
		lpstrTemp = IDS_MORNING;
	else
	{
		lpstrTemp = IDS_AFTERNOON;
		nTime -= 12;
	}

	int nMonth =0, nDay =0;

	if(GETMONTH(dwBattleTime))
		nMonth = GETMONTH(dwBattleTime)+1;

	if(GETDAY(dwBattleTime))
		nDay = GETDAY(dwBattleTime)+1;

	_stprintf(strText, IDS_M_BBS_WAR_DAY, nMonth, nDay,	lpstrTemp, nTime);
	g_pUIManager->SetString(WINDOW_MUNPA_BBS_TOP, munpa_bbs_top_dummy_18, strText, 5);

	// 상대편 문파
	g_pUIManager->SetString(WINDOW_MUNPA_BBS_TOP, munpa_bbs_top_dummy_20, strEnemyMunpaName, 5);

	// 문파자금(세금)
	g_MainCharInfo.m_dwTaxMunpaMoney = dwTaxMunpaMoney;
	_stprintf(strText, IDS_MONEY, MoneyCommaStr(dwTaxMunpaMoney).data());
	g_pUIManager->SetString(WINDOW_MUNPA_BBS_TOP, munpa_bbs_top_dummy_22, strText, 4);
	

	if(g_MainCharInfo.m_pRelation->m_dwClanID == dwMunpaID)
	{
		sClanWonInfo *pClanWon = g_MainCharInfo.m_pRelation->FindClanInfoByID(g_MainCharInfo.m_dwObjectID);

		if(pClanWon)
		{
			if((pClanWon->m_dwOrderID == 1 || pClanWon->m_dwOrderID == 2) && g_MainCharInfo.m_dwMunpaFame >= 200)  // 게시판 기능
			{
				g_pUIManager->SetData(WINDOW_MUNPA_BBS_TOP, munpa_bbs_top_button_02, CURRENT_INDEX, -1);
				g_pUIManager->SetData(WINDOW_MUNPA_BBS_TOP, munpa_bbs_top_button_03, CURRENT_INDEX, -1);
				g_pUIManager->SetData(WINDOW_MUNPA_BBS_TOP, munpa_bbs_top_button_04, CURRENT_INDEX, -1);

				g_pUIManager->SetData(WINDOW_MUNPA_BBS_READ, munpa_bbs_read_button_02, CURRENT_INDEX, -1);
				g_pUIManager->SetData(WINDOW_MUNPA_BBS_READ, munpa_bbs_read_button_03, CURRENT_INDEX, -1);
				g_pUIManager->SetData(WINDOW_MUNPA_BBS_READ, munpa_bbs_read_button_04, CURRENT_INDEX, -1);

				g_pUIManager->SetData(WINDOW_MUNPA_BBS_LIST, munpa_bbs_list_button_02, CURRENT_INDEX, -1);
				g_pUIManager->SetData(WINDOW_MUNPA_BBS_LIST, munpa_bbs_list_button_03, CURRENT_INDEX, -1);
				g_pUIManager->SetData(WINDOW_MUNPA_BBS_LIST, munpa_bbs_list_button_04, CURRENT_INDEX, -1);
			} // if((pClanWon->m_dwOrderID == 1 || pClanWon->m_dwOrderID == 2) && g_MainCharInfo.m_dwMunpaFame >= 200)  // 게시판 기능
			else
			{
				if(g_MainCharInfo.m_dwMunpaFame >= 200)
				{
					g_pUIManager->SetData(WINDOW_MUNPA_BBS_TOP, munpa_bbs_top_button_02, CURRENT_INDEX, -1);
					g_pUIManager->SetData(WINDOW_MUNPA_BBS_READ, munpa_bbs_read_button_02, CURRENT_INDEX, -1);
					g_pUIManager->SetData(WINDOW_MUNPA_BBS_LIST, munpa_bbs_list_button_02, CURRENT_INDEX, -1);
				} // if(g_MainCharInfo.m_dwMunpaFame >= 200)
				else
				{
					g_pUIManager->SetData(WINDOW_MUNPA_BBS_TOP, munpa_bbs_top_button_02, CURRENT_INDEX, 2);
					g_pUIManager->SetData(WINDOW_MUNPA_BBS_READ, munpa_bbs_read_button_02, CURRENT_INDEX, 2);
					g_pUIManager->SetData(WINDOW_MUNPA_BBS_LIST, munpa_bbs_list_button_02, CURRENT_INDEX, 2);
				}

				g_pUIManager->SetData(WINDOW_MUNPA_BBS_TOP, munpa_bbs_top_button_03, CURRENT_INDEX, 2);
				g_pUIManager->SetData(WINDOW_MUNPA_BBS_TOP, munpa_bbs_top_button_04, CURRENT_INDEX, 2);

				g_pUIManager->SetData(WINDOW_MUNPA_BBS_READ, munpa_bbs_read_button_03, CURRENT_INDEX, 2);
				g_pUIManager->SetData(WINDOW_MUNPA_BBS_READ, munpa_bbs_read_button_04, CURRENT_INDEX, 2);

				g_pUIManager->SetData(WINDOW_MUNPA_BBS_LIST, munpa_bbs_list_button_03, CURRENT_INDEX, 2);
				g_pUIManager->SetData(WINDOW_MUNPA_BBS_LIST, munpa_bbs_list_button_04, CURRENT_INDEX, 2);
			}
		}
	} // if(g_MainCharInfo.m_pRelation->m_dwClanID == dwMunpaID)
	else
	{
		g_pUIManager->SetData(WINDOW_MUNPA_BBS_TOP, munpa_bbs_top_button_02, CURRENT_INDEX, 2);
		g_pUIManager->SetData(WINDOW_MUNPA_BBS_TOP, munpa_bbs_top_button_03, CURRENT_INDEX, 2);
		g_pUIManager->SetData(WINDOW_MUNPA_BBS_TOP, munpa_bbs_top_button_04, CURRENT_INDEX, 2);

		g_pUIManager->SetData(WINDOW_MUNPA_BBS_READ, munpa_bbs_read_button_02, CURRENT_INDEX, 2);
		g_pUIManager->SetData(WINDOW_MUNPA_BBS_READ, munpa_bbs_read_button_03, CURRENT_INDEX, 2);
		g_pUIManager->SetData(WINDOW_MUNPA_BBS_READ, munpa_bbs_read_button_04, CURRENT_INDEX, 2);

		g_pUIManager->SetData(WINDOW_MUNPA_BBS_LIST, munpa_bbs_list_button_02, CURRENT_INDEX, 2);
		g_pUIManager->SetData(WINDOW_MUNPA_BBS_LIST, munpa_bbs_list_button_03, CURRENT_INDEX, 2);
		g_pUIManager->SetData(WINDOW_MUNPA_BBS_LIST, munpa_bbs_list_button_04, CURRENT_INDEX, 2);
	}

	return 0;
}

/**
 * 문파채팅
 * \param &msg 
 * \return 
 */
int OnCS_RL_MUNPACHAT_ACK( CMsg &msg)
{
	DWORD sender	= 0;
	DWORD listner	= 0;
	BYTE bType		= CT_MUNPA_BROADCAST;

	sString content		= _T("");
	sString senderName	= _T("");	
	sString listnerName = _T("");

	msg
		>> sender
		>> bType
		>> content;

	if(bType == 8)	// 문주이양 
	{
		TCHAR strTemp[128] = {0,};
		if(g_pMainChar)
		{
			CXiahCharObject* pMainChar = (CXiahCharObject*)g_pMainChar->m_pObject;
			if(pMainChar)
			{
				_stprintf(strTemp, IDS_MUNJU_RELINQUISH_MSG, (LPCTSTR)pMainChar->m_szMunpaName, (LPCTSTR)content);
				g_MainCharInfo.SpecialChatMessage(strTemp, 7);
			}
		} // if(g_pMainChar)
		else
		{
			_stprintf(strTemp, IDS_MUNJU_RELINQUISH_MSG, (LPCTSTR)g_MainCharInfo.m_szMunpaName, (LPCTSTR)content);
			g_MainCharInfo.SpecialChatMessage(strTemp, 7);
		}

		return 0;
	} // if(bType == 8)	// 문주이양
	else if(bType == 9) // 문주공지
	{
		TCHAR strTemp[128] = {0,};
		if(g_pMainChar)
		{
			CXiahCharObject* pMainChar = (CXiahCharObject*)g_pMainChar->m_pObject;
			if(pMainChar)
			{
				_stprintf(strTemp, IDS_M_NOTICE, (LPCTSTR)pMainChar->m_szMunpaName, (LPCTSTR)content);
				g_MainCharInfo.SpecialChatMessage(strTemp, 7);
			}
		} // if(g_pMainChar)
		else
		{
			_stprintf(strTemp, IDS_M_NOTICE, (LPCTSTR)g_MainCharInfo.m_szMunpaName, (LPCTSTR)content);
			g_MainCharInfo.SpecialChatMessage(strTemp, 7);
		}			

		return 0;
	}

	// 욕설 방지
	TCHAR strTemp[128] = {0,};	
	memcpy(strTemp, content.data(), strlen(content.data()));
	content.printf("%s", ConvertString(strTemp, 128));

	g_MainCharInfo.RefreshChatFrame( sender, bType, content, senderName, listner, listnerName);

	return 0;
}

/**
 * 문파 문원 닉네임 지정
 * \param &msg 
 * \return 
 */
int OnCS_RL_MUNPANICK_ACK( CMsg &msg)
{
	BYTE bResult	=0;
	DWORD dwMunpaID	=0;
	DWORD dwCharID	=0;
	sString szNickName=_T("");

	msg
		>> bResult
		>> dwMunpaID
		>> dwCharID
		>> szNickName;

	switch( bResult)
	{
	case ERR_MUNPANICK_SUCCESS:
		{
			sClanWonInfo* pInfo = g_MainCharInfo.m_pRelation->FindClanInfoByID( dwCharID);
			if( pInfo)
			{
				pInfo->m_szMunpaNickName = szNickName;

				TCHAR szContent[100] = {0,};
				_stprintf( szContent, IDS_D_CHANGE_NICK, (LPCTSTR)pInfo->m_szCharName, (LPCTSTR)szNickName);
				g_MainCharInfo.ShowHelpMessage( szContent);
				g_pUIManager->Hide(WINDOW_NAME_CONFER);

				g_MainCharInfo.m_pRelation->RefreshClanContent();
			}
		}
		break;
	case ERR_MUNPANICK_YOU_HAVENOT_MUNPA:
		g_MainCharInfo.ShowHelpMessage( IDS_ERROR_NO_MUNPA, TEXTEFFECT_COLOR_WARNING);
		break;
	case ERR_MUNPANICK_YOU_HAVENOT_POWER:
		g_MainCharInfo.ShowHelpMessage( IDS_NO_AUTH_NICK, TEXTEFFECT_COLOR_WARNING);
		break;
	case ERR_MUNPANICK_NOTFIND_MUNWON:
		g_MainCharInfo.ShowHelpMessage(IDS_ERROR_NO_MUNWON, TEXTEFFECT_COLOR_WARNING);
		break;
	case 4:
		{
			g_MainCharInfo.ShowHelpMessage(IT_WARNNIG1, TEXTEFFECT_COLOR_WARNING);
		}
		break;
	case ERR_MUNPANICK_ERROR:
		{
			TCHAR str[32] = {0,};
			_stprintf( str, IDS_ERROR, ERR_MUNPANICK_ERROR);
			g_MainCharInfo.ShowHelpMessage(str,TEXTEFFECT_COLOR_WARNING);
		}
		break;
	}

	return 0;
}

/**
 * 관계
 * \param &msg 
 * \return 
 */
int OnCS_RL_ASKRELATION_ACK(CMsg &msg)
{
	BYTE bRelType		=0;
	BYTE bRelStep		=0;
	DWORD dwAskCharID	=0;
	DWORD dwAnsCharID	=0;

	msg
		>> bRelType
		>> bRelStep
		>> dwAskCharID
		>> dwAnsCharID;

	g_MainCharInfo.m_byRelationStep = 1;

	TCHAR szText[100] = {0,};

	switch( bRelStep )
	{
		// Fail
	case RELATION_ERR_INTERNAL:
		g_MainCharInfo.ShowHelpMessage(IDS_REL_ERR_INTERNAL,TEXTEFFECT_COLOR_WARNING);
		break;
	case RELATION_ERR_SAMESEX:
		g_MainCharInfo.ShowHelpMessage(IDS_REL_ERR_SAMESEX,TEXTEFFECT_COLOR_WARNING);
		break;
	case RELATION_ERR_DIFFTYPE:
		g_MainCharInfo.ShowHelpMessage(IDS_REL_ERR_DIFFTYPE,TEXTEFFECT_COLOR_WARNING);
		break;
	case RELATION_ERR_ASKHAVE:
		g_MainCharInfo.ShowHelpMessage(IDS_REL_ERR_ASKHAVE,TEXTEFFECT_COLOR_WARNING);
		break;
	case RELATION_ERR_ANSHAVE:
		g_MainCharInfo.ShowHelpMessage(IDS_REL_ERR_ANSHAVE,TEXTEFFECT_COLOR_WARNING);
		break;
	case RELATION_ERR_NOTFOUND:
		g_MainCharInfo.ShowHelpMessage(IDS_REL_ERR_NOTFOUND,TEXTEFFECT_COLOR_WARNING);
		break;
	case RELATION_ERR_NOTIMPLEMENTED:
		g_MainCharInfo.ShowHelpMessage(IDS_REL_ERR_NOTIMPLEMENTED,TEXTEFFECT_COLOR_WARNING);
		break;
	case RELATION_ERR_CLOSEOPTION:
		g_MainCharInfo.ShowHelpMessage(IDS_REL_ERR_CLOSEOPTION,TEXTEFFECT_COLOR_WARNING);
		break;

		// 절연부
	case 9:		// 다른 사람과 거래중
		g_MainCharInfo.ShowHelpMessage(IDS_PETTRADE_TRADEOTHER, TEXTEFFECT_COLOR_WARNING);
		break;

		// Success
	case RELATION_STEP_DONE:
		{

		}
		break;
	case RELATION_STEP_ASK:
		// 상대방이 어떻할꺼냐구 물어보는거니깐 인터페이스 추가됨.
		g_MainCharInfo.m_dwAskID = dwAskCharID;
		g_MainCharInfo.m_byRelationType = bRelType;
 
		switch( bRelType )
		{
		case RELATION_TYPE_LOVER:
			_stprintf( szText, IDS_REL_ASK_LOVER, (LPCTSTR)g_MainCharInfo.FindNameByID( dwAskCharID));
			break;
		case RELATION_TYPE_TEACHER:
			_stprintf( szText, IDS_REL_ASK_DISCIPLE, (LPCTSTR)g_MainCharInfo.FindNameByID( dwAskCharID));
			break;
		case RELATION_TYPE_BUDDY:
			_stprintf( szText, IDS_D_ASK_FRIEND, (LPCTSTR)g_MainCharInfo.FindNameByID( dwAskCharID));
			break;
		};

		if( !g_pUIManager->ShowNotice( szText, NOTICE_FRAME_OKCANCEL, NOTICE_FRAME_ASKRELATION) )
		{
			// 거절
			SendCS_RL_ASKRELATION_REQ(bRelType, RELATION_STEP_DECLINE, dwAskCharID, dwAnsCharID);
		}
		break;
	case RELATION_STEP_ACCEPT:
		break;
	case RELATION_STEP_DECLINE:
		switch( bRelType )
		{
		case RELATION_TYPE_LOVER:
			g_MainCharInfo.ShowHelpMessage(IDS_REL_DECLINE_LOVER,TEXTEFFECT_COLOR_WARNING);
			break;
		case RELATION_TYPE_TEACHER:
			g_MainCharInfo.ShowHelpMessage(IDS_REL_DECLINE_DISCIPLE,TEXTEFFECT_COLOR_WARNING);
			break;
		case RELATION_TYPE_BUDDY:
			g_MainCharInfo.ShowHelpMessage(IDS_REJECT_FRIEND,TEXTEFFECT_COLOR_WARNING);
			break;
		};
		break;
	case RELATION_STEP_NOTLOGIN:
		break;
	case RELATION_STEP_CONFIRM:
		break;
	};// switch

	return 0;
}

int OnCS_RL_BREAKRELATION_ACK(CMsg &msg)
{
	BYTE bRelType;
	BYTE bRelStep;
	DWORD dwAskCharID;
	DWORD dwAnsCharID;

	msg
		>> bRelType
		>> bRelStep
		>> dwAskCharID
		>> dwAnsCharID;

	g_MainCharInfo.m_byRelationStep = 2;
	g_MainCharInfo.m_dwAskID = dwAskCharID;
	g_MainCharInfo.m_byRelationType = bRelType;

	switch( bRelStep )
	{
	case RELATION_STEP_ASK:		// 상대방이 물어보는군.
		TCHAR szText[100];
		szText[0] = 0;

		switch( bRelType )
		{
		case RELATION_TYPE_LOVER:
			_stprintf( szText, IDS_REL_BREAK_LOVER, (LPCTSTR)g_MainCharInfo.FindNameByID( dwAskCharID));
			break;
		case RELATION_TYPE_TEACHER:
		case RELATION_TYPE_STUDENT:
			_stprintf( szText, IDS_REL_BREAK_DISCIPLE, (LPCTSTR)g_MainCharInfo.FindNameByID( dwAskCharID));
			break;
		};

		if( !g_pUIManager->ShowNotice( szText, NOTICE_FRAME_OKCANCEL, NOTICE_FRAME_ASKRELATION) )
		{
			// 거절
			SendCS_RL_BREAKRELATION_REQ(bRelType, RELATION_STEP_DECLINE, dwAskCharID, dwAnsCharID);
		}
		break;
	case RELATION_STEP_ACCEPT:
		{
/*
			DWORD dwCharID;
			if( g_MainCharInfo.m_dwObjectID == dwAskCharID )
				dwCharID = dwAnsCharID;
			else
				dwCharID = dwAskCharID;

			g_MainCharInfo.m_pRelation->DeleteShip( dwCharID );
			g_MainCharInfo.m_pRelation->RefreshShipContent();

			g_MainCharInfo.ShowHelpMessage( IDS_CUT_SHIP );
*/
		}
		break;
	case RELATION_STEP_DECLINE:	// 상대방이 현재 거절상태인데 계속 진행할껴?
		{
			g_MainCharInfo.m_byRelationStep = 3;

			DWORD dwCharID;
			if( g_MainCharInfo.m_dwObjectID == dwAskCharID )
				dwCharID = dwAnsCharID;
			else
				dwCharID = dwAskCharID;

			g_MainCharInfo.m_dwAskID = dwCharID;

			g_pUIManager->ShowNotice(IDS_REL_BREAK_CONFIRM_DECLINE, NOTICE_FRAME_OKCANCEL, NOTICE_FRAME_ASKRELATION);
		}
		break;
	case RELATION_STEP_NOTLOGIN: // 상대방이 현재 없는데 계속 진행할껴?
		{
			g_MainCharInfo.m_byRelationStep = 3;

			DWORD dwCharID;
			if( g_MainCharInfo.m_dwObjectID == dwAskCharID )
				dwCharID = dwAnsCharID;
			else
				dwCharID = dwAskCharID;

			g_MainCharInfo.m_dwAskID = dwCharID;

			g_pUIManager->ShowNotice(IDS_REL_BREAK_CONFIRM_NOTLOGIN, NOTICE_FRAME_OKCANCEL, NOTICE_FRAME_ASKRELATION);
		}
		break;
	case RELATION_STEP_CONFIRM:
		{
/*
			DWORD dwCharID;
			if( g_MainCharInfo.m_dwObjectID == dwAskCharID )
				dwCharID = dwAnsCharID;
			else
				dwCharID = dwAskCharID;

			g_MainCharInfo.m_pRelation->DeleteShip( dwCharID );
			g_MainCharInfo.m_pRelation->RefreshShipContent();

			g_MainCharInfo.ShowHelpMessage( IDS_CUT_SHIP );
*/
		}
		break;

	case 8:	// 단중에는 끊을수 없음.
		{
			g_MainCharInfo.ShowHelpMessage(IDS_DAN_CUT_RELATION, TEXTEFFECT_COLOR_WARNING);			

		}
		break;
	};

	return 0;
}

int OnCS_RL_RELATIONLIST_ACK(CMsg &msg)
{
	BYTE bNumBuddy;
	DWORD dwCharID;
	sString szNickName;
	BYTE bWorldID;
	DWORD dwMapID;
	BYTE bIsConnected;
	BYTE bType;

	msg 
		>> bNumBuddy;

	for( int i=0; i<bNumBuddy; i++)
	{
		msg
			>> bType
			>> dwCharID
			>> szNickName
			>> bWorldID
			>> dwMapID
			>> bIsConnected;

		if( g_MainCharInfo.m_pRelation)
		{
			if(!g_MainCharInfo.m_pRelation->FindShipInfoByID(dwCharID))
                g_MainCharInfo.m_pRelation->InsertShip( bType, dwCharID, szNickName, bWorldID, dwMapID, 0, 0, (BOOL)bIsConnected);
		}

		// 전서구
		// bType은 csProtocol 참조
		if(bType == RELATION_TYPE_BUDDY)
			g_Mail.Add_SendList(dwCharID,szNickName,MAIL_BUDDY,bType);		// 친구들
		else	
			g_Mail.Add_SendList(dwCharID,szNickName,MAIL_RELATION,bType);	// 친구를 제외한 인연
	}

	return 0;
}

int OnCS_RL_ADDRELATION_ACK(CMsg &msg)
{
	BYTE bType;
	DWORD dwCharID;
	sString szNickName;
	BYTE bConnect;

	msg
		>> bType
		>> dwCharID
		>> szNickName
		>> bConnect;

	TCHAR szText[100];
	szText[0] = 0;

	switch( bType )
	{
	case RELATION_TYPE_LOVER:
		{
			_stprintf( szText, IDS_REL_DONE_LOVER, (LPCTSTR)szNickName );
			g_MainCharInfo.ShowHelpMessage(szText);
			g_MainCharInfo.m_pRelation->InsertShip( RELATION_TYPE_LOVER, dwCharID, szNickName, 0, 0, 0, 0, TRUE);
			g_MainCharInfo.m_pRelation->RefreshShipContent();

			g_Mail.Add_SendList(dwCharID,szNickName,MAIL_RELATION,bType);	// 친구를 제외한 인연
		}
		break;
	case RELATION_TYPE_TEACHER:
	case RELATION_TYPE_STUDENT:
		{
			_stprintf( szText, IDS_REL_DONE_TEACHER, (LPCTSTR)szNickName );

			g_MainCharInfo.ShowHelpMessage(szText);
			g_MainCharInfo.m_pRelation->InsertShip( bType, dwCharID, szNickName, 0, 0, 0, 0, TRUE);
			g_MainCharInfo.m_pRelation->RefreshShipContent();

			g_Mail.Add_SendList(dwCharID,szNickName,MAIL_RELATION,bType);	// 친구를 제외한 인연
		}
		break;
	case RELATION_TYPE_BUDDY:
		{
			_stprintf( szText, IDS_D_REGIST_FRIEND, (LPCTSTR)szNickName );

			g_MainCharInfo.ShowHelpMessage(szText);
			g_MainCharInfo.m_pRelation->InsertShip( RELATION_TYPE_BUDDY, dwCharID, szNickName, 0, 0, 0, 0, TRUE);
			g_MainCharInfo.m_pRelation->RefreshShipContent();

			g_Mail.Add_SendList(dwCharID,szNickName,MAIL_BUDDY,bType);		// 친구들
		}
		break;
	default:
		g_MainCharInfo.ShowHelpMessage(IDS_REL_COMPLETE);
		break;
	};

	return 0;
}

int OnCS_RL_DELRELATION_ACK(CMsg &msg)
{
	BYTE bType;
	DWORD dwCharID;

	msg
		>> bType
		>> dwCharID;

	g_MainCharInfo.m_pRelation->DeleteShip( dwCharID );
	g_MainCharInfo.m_pRelation->RefreshShipContent();

	g_MainCharInfo.ShowHelpMessage( IDS_CUT_SHIP );

	// 전서 리스트에서도 삭제한다.
	g_Mail.Delete_SendMailList(dwCharID );

	return 0;
}

int OnCS_RL_CHGRELATION_ACK(CMsg &msg)
{
    DWORD dwCharID;
	BYTE bWorldID;
	DWORD dwMapID;
	BYTE bConnect;

	msg
		>> dwCharID
		>> bWorldID
		>> dwMapID
		>> bConnect;

	if( g_MainCharInfo.m_pRelation)
		g_MainCharInfo.m_pRelation->ChangeBuddyConnect( dwCharID, bWorldID, dwMapID, (BOOL)bConnect);

	return 0;
}

// [5/18/2004] 문파전
/**
 * 문파 돈 기부
 * \param &msg 
 * \return 
 */
int OnCS_RL_DONATE_ACK(CMsg &msg)
{
	BYTE bResult = 0;

	msg
		>> bResult;

	switch(bResult)
	{
	case 0:	// 성공
		break;
	case 1:	// 문주만 가능
		g_MainCharInfo.ShowHelpMessage(IDS_ONLY_MUNJU, TEXTEFFECT_COLOR_WARNING);
		break;
	case 2:	// 돈 부족
		g_MainCharInfo.ShowHelpMessage(IDS_PURSE_IN_LOWMONEY, TEXTEFFECT_COLOR_WARNING);
		break;
	case 3:	// 제한 500 까지만 가능
		g_MainCharInfo.ShowHelpMessage(IDS_MUNPA_FAME_500, TEXTEFFECT_COLOR_WARNING);
		break;
	case 4:	// 시스템 댄轎
		g_MainCharInfo.ShowHelpMessage(IDS_INTERNAL_ERROR);
		break;

	default:
		break;
	}

	return 0;
}

/**
 * 기부금 횅땍
 * \param &msg 
 * \return 
 */
int OnCS_RL_PREDONATE_ACK(CMsg &msg)
{
	BYTE bResult = 0;
	//DWORD dwDonateMoney = 0;

	msg
		>> bResult
		>> g_MainCharInfo.m_pRelation->m_dwDonateMoney;// dwDonateMoney;

	switch(bResult)
	{
	case ERR_PREDONATE_SUCCESS:
		{
			TCHAR strTemp[128]= {0,};
			_stprintf(strTemp, IDS_MONEY, MoneyCommaStr(g_MainCharInfo.m_pRelation->m_dwDonateMoney).data());
			
			g_pUIManager->SetString(WINDOW_MUNPA_DONATE, munpa_donate_dummy_06, strTemp);
			g_pUIManager->SetData(WINDOW_MUNPA_DONATE, munpa_donate_button_01, CURRENT_INDEX, -1);
		}
		break;
	case ERR_PREDONATE_ONLYMUNJU:		// 문주만 가능
		g_MainCharInfo.ShowHelpMessage(IDS_ONLY_MUNJU, TEXTEFFECT_COLOR_WARNING);
		break;
	case ERR_PREDONATE_INTERNALERROR:
		g_MainCharInfo.ShowHelpMessage(IDS_INTERNAL_ERROR);
		break;
	case 3:
		g_MainCharInfo.ShowHelpMessage(IDS_MUNPA_FAME_500, TEXTEFFECT_COLOR_WARNING);
		break;
	default:
		break;
	}

	return 0;
}

/**
 * 기부금 납부시 채널 모든사람에게 뿌리기
 * \param &msg 
 * \return 
 */
int OnCS_RL_CHANGEMUNPAFAME_ACK(CMsg &msg)
{
	sString strMunpaName;
	DWORD dwFame = 0;
	BYTE bLevel = 0;

	msg
		>> strMunpaName
		>> dwFame
		>> bLevel;

	TCHAR strTemp[128]= {0,};
	_stprintf(strTemp, IDS_MUNPA_CONTRIBUTE, (LPCTSTR)strMunpaName, dwFame);

	g_MainCharInfo.SpecialChatMessage(strTemp);	

	return 0;
}

/**
 *	문파 비석 얻기
 * \param &msg 
 * \return 
 */
int OnCS_RL_GAINSTONE_ACK(CMsg &msg)
{
	BYTE bResult = 0;

	msg
		>> bResult;

	switch(bResult)
	{
	case 0:	// 성공
		g_MainCharInfo.ShowHelpMessage(IDS_STONE_POSSESSION);		
		break;
	case 1:	// 돈 부족
		g_MainCharInfo.ShowHelpMessage(IDS_PURSE_IN_LOWMONEY, TEXTEFFECT_COLOR_WARNING);
		break;
	case 2:	// 이미 다른 문파 소유
		g_MainCharInfo.ShowHelpMessage(IDS_STONE_OTHER_POSSESSION, TEXTEFFECT_COLOR_WARNING);
		break;
	case 3:	// 시스템 댄轎
		g_MainCharInfo.ShowHelpMessage(IDS_INTERNAL_ERROR);
		break;
	case 4:	// 문주만 가능
		g_MainCharInfo.ShowHelpMessage(IDS_ONLY_MUNJU, TEXTEFFECT_COLOR_WARNING);
		break;
	case 5:	// 이중 소유 금지
		g_MainCharInfo.ShowHelpMessage(IDS_STONE_DOUBLE_POSSESSION, TEXTEFFECT_COLOR_WARNING);
		break;
	case 6:	// 자격 미달 (명성등급)
		g_MainCharInfo.ShowHelpMessage(IDS_MUNPA_FAME_LOW, TEXTEFFECT_COLOR_WARNING);
		break;

	default:
		break;
	}

	return 0;
}

/**
 *	문파 게시판 리스트
 * \param &msg 
 * \return 
 */
int OnCS_RL_MUNPABBSLIST_ACK(CMsg &msg)
{
	BYTE bResult = 0;

	msg
		>> bResult;

	switch(bResult)
	{
	case 0:	
		{
			BYTE bBBSCount = 0;

			msg
				>> bBBSCount;

			for(BYTE i = 0; i < bBBSCount; ++i)
			{
				DWORD dwBBSID = 0;
				DWORD dwRecordTime = 0;
				sString strTitle;

				msg
					>> dwBBSID
					>> dwRecordTime
					>> strTitle;

				if(g_MainCharInfo.m_pListClient)
				{
					TCHAR strTemp[128]= {0,};
					_stprintf(strTemp, _T("[%d. %2d. %2d] %s"), GETYEAR(dwRecordTime), GETMONTH(dwRecordTime)+1, GETDAY(dwRecordTime)+1, (LPCTSTR)strTitle);

					g_MainCharInfo.m_pListClient->AddString(dwBBSID, strTemp);
				}
			}
		}
		break;

	case 1:	// 문파 자격 미달 (명성등급 200 이상만 가능)
		g_MainCharInfo.ShowHelpMessage(IDS_MUNPA_FAME_LOW, TEXTEFFECT_COLOR_WARNING);
		break;

	case 2:	// 시스템 댄轎
		g_MainCharInfo.ShowHelpMessage(IDS_INTERNAL_ERROR);
		break;

	default:
		break;
	}
	return 0;
}

/**
 * 문파 게시판 읽기
 * \param &msg 
 * \return 
 */
int OnCS_RL_MUNPABBSREAD_ACK(CMsg &msg)
{
	BYTE bResult = 0;

	msg
		>> bResult;

	switch(bResult)
	{
	case 0:
		{
			DWORD	dwBBSID = 0;
			sString strBBSTitle;
			sString strBBSContents;

			msg
				>> dwBBSID
				>> strBBSTitle
				>> strBBSContents;

			for(register int i=0; i < 9; ++i)
				g_pUIManager->SetString(WINDOW_MUNPA_BBS_READ, munpa_bbs_read_dummy_01 + i, _T(""));

			g_pUIManager->SetString(WINDOW_MUNPA_BBS_READ, munpa_bbs_read_dummy_01, strBBSTitle);

			TCHAR strMsg[8][64] = {0,};
			TCHAR strMix[320];
			strcpy(strMix, strBBSContents.data());

			TCHAR buf[128] = {0,};
			int c = 0;
			int nHowmanylines = 0;

			int len = _tcslen(strMix);
			for(int i=0; i<len; ++i)
			{
				if(strMix[i] == SEPARATE_MARK)
				{
					if(c > 0 && i != len-1)
					{
						_tcscpy(strMsg[nHowmanylines++], buf);
						c = 0;
						memset(buf,0,sizeof(buf));
					}
				}
				else
				{
					buf[c++] = (TCHAR)strMix[i];
				}
			}
			_tcscpy(strMsg[nHowmanylines++], buf);

			for(register int j=0; j < 8; ++j)
			{
				g_pUIManager->SetString(WINDOW_MUNPA_BBS_READ, munpa_bbs_read_dummy_02 + j, strMsg[j], 5);
			}

			if(g_MainCharInfo.m_pListClient)
				delete g_MainCharInfo.m_pListClient, g_MainCharInfo.m_pListClient = NULL;	
			
			g_MainCharInfo.CloseFrame(WINDOW_MUNPA_BBS_LIST);
			g_MainCharInfo.OpenFrame(WINDOW_MUNPA_BBS_READ);
		}
		break;

	case 1:	// 이미 삭제
		g_MainCharInfo.ShowHelpMessage(IDS_M_BBS_ALREADY_DEL, TEXTEFFECT_COLOR_WARNING);
		break;
	case 2: // 시스템 댄轎
		g_MainCharInfo.ShowHelpMessage(IDS_INTERNAL_ERROR);
		break;
	default:
		break;
	}
	return 0;
}

/**
 * 문파 게시판 쓰기
 * \param &msg 
 * \return 
 */
int OnCS_RL_MUNPABBSWRITE_ACK(CMsg &msg)
{
	BYTE bResult = 0;

	msg
		>> bResult;
	
	switch(bResult)
	{
	case 0:	// 성공
		g_MainCharInfo.ShowHelpMessage(IDS_M_BBS_WRITE_SUCCESS);
		break;
	case 1: // 권한 없음
		g_MainCharInfo.ShowHelpMessage(IDS_NO_AUTH, TEXTEFFECT_COLOR_WARNING);
		break;
	case 2:	// 글 길이 초과
		g_MainCharInfo.ShowHelpMessage(IDS_M_BBS_WRITING_MANY, TEXTEFFECT_COLOR_WARNING);
		break;
	case 3:	// 게시판 사용 개수 초과 (5개)
		g_MainCharInfo.ShowHelpMessage(IDS_M_BBS_COUNT_OVER, TEXTEFFECT_COLOR_WARNING);		
		break;
	case 4:	// 시스템 댄轎
		g_MainCharInfo.ShowHelpMessage(IDS_INTERNAL_ERROR);
		break;
	default:
		break;
	} // switch()

	return 0;
}

/**
 * 문파 게시판 삭제
 * \param &msg 
 * \return 
 */
int OnCS_RL_MUNPABBSDEL_ACK(CMsg &msg)
{
	BYTE bResult = 0;

	msg
		>> bResult;

	switch(bResult)
	{
	case 0: // 성공
		g_MainCharInfo.ShowHelpMessage(IDS_DELETED);		
		break;
	case 1: // 권한 없음
		g_MainCharInfo.ShowHelpMessage(IDS_NO_AUTH, TEXTEFFECT_COLOR_WARNING);
		break;
	case 2: // 시스템 댄轎
		g_MainCharInfo.ShowHelpMessage(IDS_INTERNAL_ERROR);
		break;
	default:
		break;
	} // switch(bResult)

	return 0;
}

/**
* 문파비석 제거
* \param &msg 
* \return 
*/
int OnCS_RL_STONEDELETE_ACK(CMsg &msg)
{
	BYTE bResult = 0;

	msg
		>> bResult;

	switch(bResult)
	{
	case 0:	// 성공
		g_MainCharInfo.ShowHelpMessage(IDS_STONE_DELETE_SUCCESS);
		break;
	case 1:	// 전쟁중
		g_MainCharInfo.ShowHelpMessage(IDS_STONE_DELETE_WAR, TEXTEFFECT_COLOR_WARNING);
		break;
	case 2:	// 문주만 가능
		g_MainCharInfo.ShowHelpMessage(IDS_STONE_DELETE_MUNJU, TEXTEFFECT_COLOR_WARNING);
		break;
	case 3:	// 시스템 댄轎
		g_MainCharInfo.ShowHelpMessage(IDS_INTERNAL_ERROR);
		break;
	case 4:	// 해당 문파만 포기 가능
		g_MainCharInfo.ShowHelpMessage(IDS_STONE_DELETE_ONLY, TEXTEFFECT_COLOR_WARNING);		
		break;

	default:
		break;
	} // switch(bResult)

	return 0;
}


/**
 * 문파공지
 * \param &msg 
 * \return 
 */
int OnCS_RL_MUNPANOTICE_ACK(CMsg &msg)
{
	BYTE bResult=0;

	msg
		>> bResult;

	switch(bResult)
	{
	case ERR_MUNPANOTICE_SUCCESS:
		g_MainCharInfo.ShowHelpMessage(IDS_M_BBS_WRITE_SUCCESS);
		break;
	case ERR_MUNPANOTICE_LESSFAME:		// 문파명성 80이하
		g_MainCharInfo.ShowHelpMessage(IDS_MUNPA_FAME_LOW, TEXTEFFECT_COLOR_WARNING);
		break;
	case ERR_MUNPANOTICE_ONLYMUNJU:		// 문주만사용가능
		g_MainCharInfo.ShowHelpMessage(IDS_ONLY_MUNJU_USE, TEXTEFFECT_COLOR_WARNING);
		break;
	case ERR_MUNPANOTICE_NOTICEOVER:	// 공지사항 초과
		g_MainCharInfo.ShowHelpMessage(IDS_MUNPA_NOTICEOVER, TEXTEFFECT_COLOR_WARNING);		
		break;
	case ERR_MUNPANOTICE_INTERNALERROR:	// 내부댄轎
		g_MainCharInfo.ShowHelpMessage(IDS_INTERNAL_ERROR, TEXTEFFECT_COLOR_WARNING);
		break;
	default:
		break;
	}

	return 0;
}

/**
 * 문파문장 등록
 * \param &msg 
 * \return 
 */
int OnCS_RL_MUNPAMARKREG_ACK(CMsg &msg)
{
	BYTE bType		=0;
	BYTE bResult	=0;

	msg
		>> bType
		>> bResult;

	switch(bResult)
	{
	case ERR_MUNPAMARKREG_SUCCESS:		// 성공
		{
			if(bType == 0)	// 등록
			{
				g_MainCharInfo.ShowHelpMessage(IDS_MUNPAMARKREG);
			}
			else if(bType == 1)	// 삭제
			{
				g_MainCharInfo.ShowHelpMessage(IDS_MUNPAMARKDEL);
			}
		}
		break;
	case ERR_MUNPAMARKREG_NEEDSTONE:	// 비석없음
		g_MainCharInfo.ShowHelpMessage(IDS_NOT_MUNPASTONE, TEXTEFFECT_COLOR_WARNING);
		break;
	case ERR_MUNPAMARKREG_LESSMONEY:	// 돈없음
		g_MainCharInfo.ShowHelpMessage(IDS_PURSE_IN_LOWMONEY, TEXTEFFECT_COLOR_WARNING);
		break;
	case ERR_MUNPAMARKREG_ONLYMUNJU:	// 문주만
		g_MainCharInfo.ShowHelpMessage(IDS_STONE_ONLY_MUNJU, TEXTEFFECT_COLOR_WARNING);
		break;
	case ERR_MUNPAMARKREG_NOTFINDMARK:	// 등록된 마크 없어서 삭제 불가
		g_MainCharInfo.ShowHelpMessage(IDS_NOTFINDMARK, TEXTEFFECT_COLOR_WARNING);		
		break;
	case ERR_MARKREG_ALREADYREG:		// 이미 등록되었음
		g_MainCharInfo.ShowHelpMessage(IDS_MUNPAMARK_ALREADYREG, TEXTEFFECT_COLOR_WARNING);		
		break;
	case ERR_MARKREG_OVERIMAGESIZE:		// 이미지 크기가 틀릴때
		g_MainCharInfo.ShowHelpMessage(IDS_MUNPAMARK_MISTAKEN, TEXTEFFECT_COLOR_WARNING);
		break;
	case ERR_MARKREG_INTERNALERROR:		// 내부댄轎
		g_MainCharInfo.ShowHelpMessage(IDS_INTERNAL_ERROR, TEXTEFFECT_COLOR_WARNING);
		break;

	default:
		break;
	}

	return 0;
}

/**
 * 문파마크 이미지 받기
 * \param &msg 
 * \return 
 */
int OnCS_RL_MUNPAMARK_ACK(CMsg &msg)
{
	BYTE bType =0;
	DWORD dwMarkID =0;
	sString strImage;

	msg
		>> bType
		>> dwMarkID
		>> strImage;

	g_MunpaMark.SaveMarkFile(dwMarkID, strImage.data());

	return 0;
}

/**
 * 문파마크 받기
 * \param &msg 
 * \return 
 */
int OnCS_RL_GAINMARKIMAGE_ACK(CMsg &msg)
{
	BYTE bResult	=0;

	msg
		>> bResult;

	switch(bResult)
	{
	case ERR_GAINMARKIMAGE_SUCCESS:
		{
			DWORD dwMarkID =0;
			sString strImage;

			msg
				>> dwMarkID
				>> strImage;

			g_MunpaMark.SaveMarkFile(dwMarkID, strImage.data());
		}
		break;
	case ERR_GAINMARKIMAGE_NOTFINDMARK:	// 찾을수 없음
		{
			// TODO: 문구와 마크 처리
		}
		break;
	default:
		break;
	} // switch(bResult)

	return 0;
}

/**
 * 비석 세금 회수
 * \param &msg 
 * \return 
 */
int OnCS_RL_GETMUNPAMONEY_ACK(CMsg &msg)
{
	BYTE bResult	=0;

	msg
		>> bResult;

	switch(bResult)
	{
	case ERR_GETMONEY_SUCCESS:		// 성공
		{
			g_MainCharInfo.ShowHelpMessage(IDS_GETMUNPAMONEY);

			TCHAR strText[64] = {0,};
			_stprintf(strText, IDS_MONEY, MoneyCommaStr(0).data());
			g_pUIManager->SetString(WINDOW_MUNPA_BBS_TOP, munpa_bbs_top_dummy_22, strText, 4);
		}
		break;
	case ERR_GETMONEY_ONLYMUNJU:	// 문주만 가능
		g_MainCharInfo.ShowHelpMessage(IDS_STONE_ONLY_MUNJU, TEXTEFFECT_COLOR_WARNING);
		break;
	case ERR_GETMONEY_ISWARFAIL:	// 전쟁중에는 회수 불가
		g_MainCharInfo.ShowHelpMessage(IDS_GETMUNPAMONEY_WARFAIL, TEXTEFFECT_COLOR_WARNING);
		break;
	case ERR_GETMONEY_NOTSAMEMONEY:	// 금액이 틀림
		g_MainCharInfo.ShowHelpMessage(IDS_GETMUNPAMONEY_NOTSAMEMONEY, TEXTEFFECT_COLOR_WARNING);
		break;
	case ERR_GETMONEY_OVERMONEY:	// 행낭금액초과
		g_MainCharInfo.ShowHelpMessage(IDS_PURSE_OUT_OVERMONEY, TEXTEFFECT_COLOR_WARNING);
		break;
	case ERR_GETMONEY_INTERNALERROR: // 내부댄轎
		g_MainCharInfo.ShowHelpMessage(IDS_INTERNAL_ERROR, TEXTEFFECT_COLOR_WARNING);
		break;

	default:
		break;
	} // switch(bResult)

	return 0;
}

/**
 * 절연부
 * \param &msg 
 * \return 
 */
int OnCS_RL_BREAKRELATIONITEM_ACK(CMsg &msg)
{
	BYTE bResult =0;

	msg
		>> bResult;

	switch(bResult)
	{
	case ERR_BREAKRELATIONITEM_SUCCESS:		// 성공
		{
			g_MainCharInfo.ShowHelpMessage(IDS_BREAKRELATIONITEM_SUCCESS);
		}
		break;
	case ERR_BREAKRELATIONITEM_NOTFOUND:	// 아이템을 찾을수 없음
		{
			g_MainCharInfo.ShowHelpMessage(IDS_ITEM_NOTFIND, TEXTEFFECT_COLOR_WARNING);
		}		
		break;
	case ERR_BREAKRELATIONITEM_ONTRADE:		// 거래중인 아이템은 사용할수 없음
		{
			g_MainCharInfo.ShowHelpMessage(IDS_BREAKRELATIONITEM_ONTRADE, TEXTEFFECT_COLOR_WARNING);
		}
	    break;
	case ERR_BREAKRELATIONITEM_NOTUSE:		// 사용할수 없는 아이템
		{
			g_MainCharInfo.ShowHelpMessage(IM_N_WHO, TEXTEFFECT_COLOR_WARNING);
		}
		break;
	case 16:								// 관계 없음
		{
			g_MainCharInfo.ShowHelpMessage(IDS_REL_NOT, TEXTEFFECT_COLOR_WARNING);
		}
		break;
	case ERR_BREAKRELATIONITEM_INTERNALERROR:	// 댄轎
		{
			g_MainCharInfo.ShowHelpMessage(IDS_INTERNAL_ERROR, TEXTEFFECT_COLOR_WARNING);
		}
		break;
	default:
		break;
	}

	return 0;
}