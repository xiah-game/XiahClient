
int OnCS_OP_OPTIONLIST_ACK( CMsg &msg)
{
	BYTE bWhisper	=0;
	BYTE bRelation	=0;
	BYTE bTrade		=0;
	BYTE bSafeMode	=0;

	msg
		>> bWhisper
		>> bRelation
		>> bTrade
		>> bSafeMode
		>> g_info.m_dwBuyLimit
		>> g_info.m_bRarityLimit
		>> g_info.m_bStxTypeLimit;

	// OPTION Àû¿ë
	g_info.m_bAllowWhisper	= bWhisper;
	g_info.m_bAllowRelation = bRelation;
	g_info.m_bAllowTrade	= bTrade;
	g_info.m_bSafeMode		= bSafeMode;

	g_MainCharInfo.m_dwBuyLimit		= g_info.m_dwBuyLimit;
	g_MainCharInfo.m_bRarityLimit	= g_info.m_bRarityLimit;
	g_MainCharInfo.m_bStxTypeLimit	= g_info.m_bStxTypeLimit;

	return TRUE;
}

int OnCS_OP_SETOPTION_ACK( CMsg &msg)
{
	BYTE bResult	=0;
	BYTE bWhisper	=0;
	BYTE bRelation	=0;
	BYTE bTrade		=0;
	BYTE bSafeMode	=0;
	DWORD dwBuyLimit	=0;
	BYTE bRarityLimit	=0;
	BYTE bStxTypeLimit	=0;

	msg 
		>> bResult
		>> bWhisper
		>> bRelation
		>> bTrade
		>> bSafeMode
		>> dwBuyLimit
		>> bRarityLimit
		>> bStxTypeLimit;

	if(bResult)
	{
		DBG_Put(_T("OPTION Setting Fail!"));
	}
	else
	{
		g_info.m_bAllowWhisper	= bWhisper;
		g_info.m_bAllowRelation = bRelation;
		g_info.m_bAllowTrade	= bTrade;
		g_info.m_bSafeMode		= bSafeMode;

		g_MainCharInfo.m_dwBuyLimit		= g_info.m_dwBuyLimit		= dwBuyLimit;
		g_MainCharInfo.m_bRarityLimit	= g_info.m_bRarityLimit		= bRarityLimit;
		g_MainCharInfo.m_bStxTypeLimit	= g_info.m_bStxTypeLimit	= bStxTypeLimit;

		DBG_Put(_T("OPTION Setting Success!"));
	}

	return TRUE;
}
