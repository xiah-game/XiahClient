int OnCS_QS_CHANGE_ACK( CMsg &msg)
{
	DWORD dwQuestID				=0;	
	DWORD dwProcessCurrAmount	=0;
	DWORD dwProcessTotalAmount	=0;	
	DWORD dwResultAmount		=0;

	BYTE bStatus				=0;
	BYTE bRepeat				=0;
	BYTE bProcessNum			=0;
	BYTE bResultNum				=0;
	BYTE bProcessType			=0; 

	sString szQuestName;
	sString szDescription;
	sString szProcessName;
	sString szResultName;
	
	msg
		>> g_MainCharInfo.m_dwFame
		>> dwQuestID
		>> bStatus
		>> szQuestName
		>> szDescription
		>> bRepeat
		>> bProcessType
		>> bProcessNum
		>> bResultNum;

	//HT_0914 : 기연창 및 낭 아이템 개선 사항
	BYTE bReStatus = 0;

	switch(bStatus)	//퀘스트 정렬을 위해 순서를 바꿔준다..(서버와 상태 값이 다름)
	{
	case 0:
		bReStatus = 2;	
		break;
	case 1:
		bReStatus = 1;
		break;
	case 2:
		bReStatus = 0;
	//	g_MainCharInfo.m_pQuest->m_sQuestHelp.m_bStart = true;			//HT_0824 : 퀘스트 도우미 추가
	//	g_MainCharInfo.m_pQuest->m_sQuestHelp.m_dwQuestID = dwQuestID;
		g_MainCharInfo.m_pQuest->SetQuestDestination(dwQuestID,bProcessType);
		break;
	case 3:
		bReStatus = 4;
		break;
	case 4:
		bReStatus = 3;
		break;
	case 5:
		bReStatus = 5;
		break;
	}

	if( g_MainCharInfo.m_pQuest)
	{
		sQuestInfo* pQuest = g_MainCharInfo.m_pQuest->FindQuest( dwQuestID);
		if( pQuest)
		{
			if( pQuest->m_eStatus != eSuccess && bReStatus == eSuccess)
			{
				g_MainCharInfo.ShowHelpMessage( IDS_QUEST_DONE);
				g_MainCharInfo.m_pQuest->m_sQuestHelp.m_bStart = false;			//HT_0824 : 퀘스트 도우미 추가
			}
			else if (bReStatus == eNew)
				g_MainCharInfo.ShowHelpMessage( IDS_QUEST_RE);

			pQuest->m_eStatus = (QUESTSTATUS_TYPE)bReStatus;
			pQuest->m_bRepeat = bRepeat;
			pQuest->m_bProcessType = bProcessType;

			if( pQuest->m_eStatus == eSuccess && bReStatus == eStart)
			{
				g_MainCharInfo.ShowHelpMessage( QS_WARNNING);
				ProcessClickQuestButton();
			}

		}
		else
		{														
			g_MainCharInfo.m_pQuest->InsertQuest( dwQuestID, (QUESTSTATUS_TYPE)bReStatus, szQuestName, szDescription, bRepeat, bProcessNum, bResultNum, bProcessType);
			
			g_MainCharInfo.ShowHelpMessage( IDS_QUEST_REC);
			ProcessClickQuestButton();
		}
	}

	for( int i=0; i<bProcessNum; i++)
	{
		msg
			>> szProcessName
			>> dwProcessCurrAmount
			>> dwProcessTotalAmount;

		if( g_MainCharInfo.m_pQuest)
		{
			g_MainCharInfo.m_pQuest->InsertQuestCondition( dwQuestID, i, szProcessName, dwProcessCurrAmount, dwProcessTotalAmount);
		}
	}

	for( int k=0; k<bResultNum; k++)
	{
		msg
			>> szResultName
			>> dwResultAmount;

		if( g_MainCharInfo.m_pQuest)
		{
			g_MainCharInfo.m_pQuest->InsertQuestReward( dwQuestID, k, szResultName, dwResultAmount);
		}
	}

	g_MainCharInfo.RefreshQuest();
	g_MainCharInfo.RefreshChracterInfo();

	if(!g_pMainChar)
		return 0;

	// 명성치 색
	CXiahCharObject *pObject = (CXiahCharObject*)g_pMainChar->m_pObject;

	if(pObject)
		pObject->RefreshFameColor(g_MainCharInfo.m_dwFame);

	return 0;
}

int OnCS_QS_LIST_ACK( CMsg &msg)
{
	DWORD dwQuestID				=0;
	DWORD dwProcessCurrAmount	=0;
	DWORD dwProcessTotalAmount	=0;
	DWORD dwResultAmount		=0;

	BYTE bNumQuest				=0;	
	BYTE bStatus				=0;	
	BYTE bRepeat				=0;
	BYTE bProcessNum			=0;
	BYTE bResultNum				=0;	
	BYTE bProcessType			=0;

	sString szQuestName;
	sString szDescription;
	sString szProcessName;
	sString szResultName;

	msg 
		>> g_MainCharInfo.m_dwFame
		>> bNumQuest;

	for( int i=0; i<bNumQuest; i++)
	{
		msg
			>> dwQuestID
			>> bStatus
			>> szQuestName
			>> szDescription
			>> bRepeat
			>> bProcessType
			>> bProcessNum
			>> bResultNum;

		//HT_0914 : 기연창 및 낭 아이템 개선 사항
		BYTE bReStatus = 0;

		switch(bStatus)//퀘스트 정렬을 위해 순서를 바꿔준다..(서버와 상태 값이 다름)
		{
		case 0:
			bReStatus = 2;
			break;
		case 1:
			bReStatus = 1;
			break;
		case 2:
			bReStatus = 0;
		//	g_MainCharInfo.m_pQuest->m_sQuestHelp.m_bStart = true;			//HT_0824 : 퀘스트 도우미 추가
		//	g_MainCharInfo.m_pQuest->m_sQuestHelp.m_dwQuestID = dwQuestID;
			g_MainCharInfo.m_pQuest->SetQuestDestination(dwQuestID,bProcessType);
			break;
		case 3:
			bReStatus = 4;
			break;
		case 4:
			bReStatus = 3;
			break;
		case 5:
			bReStatus = 5;
			break;
		}

		if( g_MainCharInfo.m_pQuest)
		{
			g_MainCharInfo.m_pQuest->InsertQuest( dwQuestID, (QUESTSTATUS_TYPE)bReStatus, (LPCTSTR)szQuestName, (LPCTSTR)szDescription, bRepeat, bProcessNum, bResultNum, bProcessType);
		}

		for( int j=0; j<bProcessNum; j++)
		{
			msg
				>> szProcessName
				>> dwProcessCurrAmount
				>> dwProcessTotalAmount;

			if( g_MainCharInfo.m_pQuest)
			{
				g_MainCharInfo.m_pQuest->InsertQuestCondition( dwQuestID, j, (LPCTSTR)szProcessName, dwProcessCurrAmount, dwProcessTotalAmount);
			}
		}

		for( int k=0; k<bResultNum; k++)
		{
			msg
				>> szResultName
				>> dwResultAmount;

			if( g_MainCharInfo.m_pQuest)
			{
				g_MainCharInfo.m_pQuest->InsertQuestReward( dwQuestID, k, (LPCTSTR)szResultName, dwResultAmount);
			}
		}
	}
	
	g_MainCharInfo.RefreshQuest();
	g_MainCharInfo.RefreshChracterInfo();

	if(!g_pMainChar)
		return 0;

	// 명성치 색
	CXiahCharObject *pObject = (CXiahCharObject*)g_pMainChar->m_pObject;

	if(pObject)
		pObject->RefreshFameColor(g_MainCharInfo.m_dwFame);

	return 0;
}

int OnCS_QS_START_ACK( CMsg &msg)
{
	BYTE bResult		=0;
	DWORD dwQuestID		=0;

	msg
		>> bResult
		>> dwQuestID;

	TCHAR temp[100] = {0,};

	if( !bResult)
	{
		_stprintf( temp, IDS_QUEST_START, dwQuestID);
		sQuestInfo* pQuest = g_MainCharInfo.m_pQuest->FindQuest( dwQuestID);
		if( pQuest)
		{
			//g_MainCharInfo.RefreshQuest(); //HO_0820_07 퀘스트 분류 수정 전 코드
			
			pQuest->m_eStatus = eStart; //HO_0820_07 퀘스트 분류 수정
			g_MainCharInfo.m_pQuest->UpdateQuest();
			g_pUIManager->SetData(WINDOW_QUEST_01, quest_window_01_button_01, CURRENT_INDEX, 2);
			g_pUIManager->SetData(WINDOW_QUEST_01, quest_window_01_button_02, CURRENT_INDEX, -1);
			g_pUIManager->SetData(WINDOW_QUEST_01, quest_window_01_button_03, CURRENT_INDEX, -1);
			g_pUIManager->SetData(WINDOW_QUEST_01, quest_window_01_button_04, CURRENT_INDEX, -1);
			
		}

		g_MainCharInfo.ShowHelpMessage(temp);

		return 0;
	}

	switch( bResult)
	{
	case QUESTRESULT_CANT_NEW:	//발생 조건 만족 못함
		_stprintf( temp, IDS_START_CONDITION_NOT, dwQuestID);
		break;
	case QUESTRESULT_CANT_START: //수행 조건 만족 못함
		_stprintf( temp, IDS_DO_CONDITION_NOT, dwQuestID);
		break;
	case QUESTRESULT_CANT_STOP: //멈출수 없는 퀘스트
		_stprintf( temp, IDS_CANNOT_STOP_QUEST, dwQuestID);
		break;
	case QUESTRESULT_CANT_DONE: //완수 조건 만족 못함
		_stprintf( temp, IDS_COMPLETION_NOT, dwQuestID);
		break;
	case QUESTRESULT_CANT_REWARD: //보상 내역 지급 못함
		_stprintf( temp, IDS_REWARD_CANNOT, dwQuestID);
		break;
	case QUESTRESULT_REWARD_DROP:
		_stprintf( temp, IDS_BAG_FULL, dwQuestID);
		break;
	case QUESTRESULT_ERR_INTERNAL: //내부 에러
		_stprintf( temp, IDS_INTERNAL_ERROR, dwQuestID);
		break;
	default:
		_stprintf( temp, IDS_ERROR, bResult);
		break;
	}

	g_MainCharInfo.ShowHelpMessage(temp,TEXTEFFECT_COLOR_WARNING);

	return 0;
}

int OnCS_QS_STOP_ACK( CMsg &msg)
{
	BYTE bResult	=0;
	DWORD dwQuestID	=0;

	msg
		>> bResult
		>> dwQuestID;

	TCHAR temp[100] = {0,};

	if( !bResult)
	{
		g_MainCharInfo.m_pQuest->m_sQuestHelp.m_bStart = false;			//HT_0824 : 퀘스트 도우미 추가

		_stprintf( temp, IDS_QUEST_STOP, dwQuestID);
		sQuestInfo* pQuest = g_MainCharInfo.m_pQuest->FindQuest( dwQuestID);
		if( pQuest)
		{
			pQuest->m_eStatus = ePause;
			g_MainCharInfo.RefreshQuest();
		}

		g_MainCharInfo.ShowHelpMessage(temp);

		return 0;
	}	

	switch( bResult)
	{
	case QUESTRESULT_CANT_NEW:	//발생 조건 만족 못함
		_stprintf( temp, IDS_START_CONDITION_NOT, dwQuestID);
		break;
	case QUESTRESULT_CANT_START: //수행 조건 만족 못함
		_stprintf( temp, IDS_DO_CONDITION_NOT, dwQuestID);
		break;
	case QUESTRESULT_CANT_STOP: //멈출수 없는 퀘스트
		_stprintf( temp, IDS_CANNOT_STOP_QUEST, dwQuestID);
		break;
	case QUESTRESULT_CANT_DONE: //완수 조건 만족 못함
		_stprintf( temp, IDS_COMPLETION_NOT, dwQuestID);
		break;
	case QUESTRESULT_CANT_REWARD: //보상 내역 지급 못함
		_stprintf( temp, IDS_REWARD_CANNOT, dwQuestID);
		break;
	case QUESTRESULT_REWARD_DROP:
		_stprintf( temp, IDS_BAG_FULL, dwQuestID);
		break;
	case QUESTRESULT_ERR_INTERNAL: //내부 에러
		_stprintf( temp, IDS_INTERNAL_ERROR, dwQuestID);
		break;
	default:
		_stprintf( temp, IDS_ERROR, bResult);
		break;
	}

	g_MainCharInfo.ShowHelpMessage(temp,TEXTEFFECT_COLOR_WARNING);

	return 0;
}

int OnCS_QS_DONE_ACK( CMsg &msg)
{
	BYTE bResult	=0;	
	BYTE bStatus	=0;
	DWORD dwQuestID	=0;

	msg
		>> bResult
		>> dwQuestID
		>> bStatus
		>> g_MainCharInfo.m_dwFame;

	TCHAR temp[64] = {0,};

	if( bResult)
	{		
		sQuestInfo* pQuest = g_MainCharInfo.m_pQuest->FindQuest( dwQuestID);
		if( pQuest)
		{
			g_MainCharInfo.m_pQuest->m_sQuestHelp.m_bStart = false;			//HT_0824 : 퀘스트 도우미 추가

			pQuest->m_eStatus = eSuccess;
			g_MainCharInfo.RefreshQuest();
			g_MainCharInfo.RefreshChracterInfo();

			if(g_pMainChar)
			{
				// 명성치 색
				CXiahCharObject *pObject = (CXiahCharObject*)g_pMainChar->m_pObject;

				if(pObject)
					pObject->RefreshFameColor(g_MainCharInfo.m_dwFame);
			}
		}

		g_MainCharInfo.ShowHelpMessage(IDS_QUEST_DONE_);

		return 0;
	}

	switch( bResult)
	{
	case QUESTRESULT_CANT_NEW:		//발생 조건 만족 못함
		_stprintf( temp, IDS_START_CONDITION_NOT, dwQuestID);
		break;
	case QUESTRESULT_CANT_START:	//수행 조건 만족 못함
		_stprintf( temp, IDS_DO_CONDITION_NOT, dwQuestID);
		break;
	case QUESTRESULT_CANT_STOP:		//멈출수 없는 퀘스트
		_stprintf( temp, IDS_CANNOT_STOP_QUEST, dwQuestID);
		break;
	case QUESTRESULT_CANT_DONE:		//완수 조건 만족 못함
		_stprintf( temp, IDS_COMPLETION_NOT, dwQuestID);
		break;
	case QUESTRESULT_CANT_REWARD:	//보상 내역 지급 못함
		_stprintf( temp, IDS_REWARD_CANNOT, dwQuestID);
		break;
	case QUESTRESULT_REWARD_DROP:
		_stprintf( temp, IDS_BAG_FULL, dwQuestID);
		break;
	case QUESTRESULT_ERR_INTERNAL:	//내부 에러
		_stprintf( temp, IDS_INTERNAL_ERROR, dwQuestID);
		break;
	default:
		_stprintf( temp, IDS_ERROR, bResult);
		break;
	}

	g_MainCharInfo.ShowHelpMessage(temp,TEXTEFFECT_COLOR_WARNING);

	return 0;
}


int OnCS_QS_DELETE_ACK(CMsg &msg)
{
	BYTE bResult	=0;
	DWORD dwQuestID	=0;

	msg
		>> bResult
		>> dwQuestID;

	switch( bResult)
	{
	case QUESTRESULT_OK:	//발생 조건 만족 못함
		{
			g_MainCharInfo.ShowHelpMessage(IDS_DELETED);
			g_MainCharInfo.m_pQuest->DeleteQuest(dwQuestID);
			
			//g_MainCharInfo.m_pQuest->UpdateQuest(); //HO_0820_07 퀘스트 분류 수정
			//g_pUIManager->SetData(WINDOW_QUEST_01, quest_window_01_button_01, CURRENT_INDEX, 2);
			//g_pUIManager->SetData(WINDOW_QUEST_01, quest_window_01_button_02, CURRENT_INDEX, -1);
			//g_pUIManager->SetData(WINDOW_QUEST_01, quest_window_01_button_03, CURRENT_INDEX, -1);
			//g_pUIManager->SetData(WINDOW_QUEST_01, quest_window_01_button_04, CURRENT_INDEX, -1);
		}
		break;
	case QUESTRESULT_CANT_DELETE: //수행 조건 만족 못함
		{
			g_MainCharInfo.ShowHelpMessage(IDS_DELETE_FAIL);
		}
		break;
	default:
		{
			TCHAR temp[32] = {0,};
			_stprintf( temp, IDS_ERROR, bResult);
			g_MainCharInfo.ShowHelpMessage(temp, 2);
		}
		break;
	} // switch( bResult)

	return 0;
}