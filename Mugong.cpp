#include "precompile.h"
#include "mugong.h"
#include "XiahGame_Main.h"
#include "XiahGame_Handler_Sender.h"
#include "InterfaceDefine.h"
#include "XiahGameObject.h"


CMugong::CMugong(void)
{
	for( int i=0; i < 15; ++i)
		m_pKeepUpVB[i] = NULL;

	m_pKeepUpIconVB = NULL;

	CreateKeepUpMugongIcon(); //HT_0403 : 지속형 무공 시전 아이콘
	CreateKeepUpIcon(); //HO_0703_07 게임내 심의 등급 표기	
	KeepUpIconTimer = 0; //HO_0703_07 게임내 심의 등급 표기
}


CMugong::~CMugong(void)
{
	for(MugongMap::iterator it = m_mapMugong.begin(); it != m_mapMugong.end(); ++it)
	{
		sMugongInfo* pMugong = it->second;

		if(pMugong)
			delete pMugong;
	}

	//HT_0711 : 진각성 무공
	for(MugongMap::iterator it = m_map2ThRebirthMugong.begin(); it != m_map2ThRebirthMugong.end(); ++it)
	{
		sMugongInfo* pMugong = it->second;

		if(pMugong)
			delete pMugong;
	}

	for(int i=0; i < 15; ++i)
	{
		// VERTEX BUFFER
		if( m_pKeepUpVB[i] )
		{
			m_pKeepUpVB[i]->Release();
		}

		// TEXTURE는 XIAH PAK에 맞긴다.
	}

	m_pKeepUpIconVB->Release();

	m_mapMugong.clear();
}


/**
 *
 * \param dwMugongID 
 * \param bType 
 * \param bLevel 
 */
void CMugong::InsertMugong(DWORD dwMugongID, BYTE bType, BYTE bLevel)
{
	//HT_0711 : 진각성 무공
	if(dwMugongID >= 191 && dwMugongID <= 198)
	{
		MugongMap::iterator it = m_map2ThRebirthMugong.find(dwMugongID);

		sMugongInfo* pMugong = new sMugongInfo(); 
		
		if(it != m_map2ThRebirthMugong.end())
		{
			pMugong= it->second;
			
			if(pMugong)
				pMugong->m_byMugongLevel = bLevel;
		}
		else
		{
			pMugong->m_dwMugongID	 = dwMugongID;
			pMugong->m_byMugongType	 = bType;
			pMugong->m_byMugongLevel = bLevel;

			m_map2ThRebirthMugong.insert(MugongMap::value_type(dwMugongID, pMugong));		
		}

		SetMugongGUI(dwMugongID, pMugong->m_byMugongType, bLevel);
	}
	else
	{
		MugongMap::iterator it = m_mapMugong.find(dwMugongID);

		if(it != m_mapMugong.end())
		{
			sMugongInfo* pMugong = it->second;
		
			if(pMugong)
				pMugong->m_byMugongLevel = bLevel;
		}
		else
		{
			sMugongInfo* pMugong = new sMugongInfo();

			pMugong->m_dwMugongID	 = dwMugongID;
			pMugong->m_byMugongType	 = bType;
			pMugong->m_byMugongLevel = bLevel;

			m_mapMugong.insert(MugongMap::value_type(dwMugongID, pMugong));		
		}

		SetMugongGUI(dwMugongID, bType, bLevel);
	}
}

void CMugong::SetMugongGUI(DWORD dwMugongID, BYTE bType, BYTE bLevel)
{
	// 환생 내공/외공
	if(dwMugongID >= SUNSINGONG && dwMugongID <= YA_HOJUNGKANGKI)
	{
		sArrayData* pRebirthMugong = XiahArrayIndex::g_RebirthMugong_List.GetData(dwMugongID, bLevel);

		if(pRebirthMugong)
		{			
			int nResID	= pRebirthMugong->GetInt(2);
			int nSeq	= pRebirthMugong->GetInt(55);

			if(dwMugongID >= SUNSINGONG && dwMugongID <= SUNGAPSUL)
			{
				--nSeq;

				// 레벨
				g_pUIManager->SetString(WINDOW_SKILL, skill_window_negong_skill_01 + nSeq, bLevel);
				// 아이콘
				g_pUIManager->SetData(WINDOW_SKILL, skill_window_negong_01 + nSeq, TEXTURE, nResID);
				// 툴팁
				SetRebirthToolTip(dwMugongID, WINDOW_SKILL, skill_window_negong_01 + nSeq);
			}
			else if(dwMugongID >= WHA_DRAGONSINJANG && dwMugongID <= YA_HOJUNGKANGKI)
			{
				nSeq -= 4;

				// 이름
				sString szMugongName = pRebirthMugong->GetString( 1);
				g_pUIManager->SetString(WINDOW_SKILL, skill_window_nugong_name_dummy_01 + nSeq, (LPCTSTR)szMugongName);
				// 레벨
				g_pUIManager->SetString(WINDOW_SKILL, skill_window_nugong_skill_01 + nSeq, bLevel);
				// 아이콘
				g_pUIManager->SetData(WINDOW_SKILL, skill_window_mugong_dummy_01 + nSeq, TEXTURE, nResID);
				// 툴팁
				SetRebirthToolTip(dwMugongID, WINDOW_SKILL, skill_window_mugong_dummy_01 + nSeq);
			}
		}
	}
	else if(dwMugongID >= 191 && dwMugongID <= 198) //HT_0711 :진각성 무공(bType은 유파를 나타내는 것이나 흡성신공에서는 무공 아이콘의 위치 값으로 처리한다.)
	{
		sArrayData* pMugongData = XiahArrayIndex::g_MugongTemplate.GetData(dwMugongID);

		if( pMugongData)
		{
			// 이름
			sString szMugongName = pMugongData->GetString( 1);
			g_pUIManager->SetString(WINDOW_INSIDE, inside_window_negong_name_dummy_10 + bType - 3, (LPCTSTR)szMugongName);

			// 레벨
			g_pUIManager->SetString(WINDOW_INSIDE, inside_window_negong_skill_10 + bType - 3, 0);

			// 툴팁
			if( g_MainCharInfo.m_pMugong)
				g_MainCharInfo.m_pMugong->SetMugongToolTip( 199 + bType - 3 , WINDOW_INSIDE, inside_window_mugong_dummy_01 + bType - 3);
			
			// 아이콘 이미지
			int nResID = pMugongData->GetInt(1);
			g_pUIManager->SetData(WINDOW_INSIDE, inside_window_mugong_dummy_01 + bType - 3, TEXTURE, nResID);
		}
	}
	else 
	{
		sArrayData* pMugongData = XiahArrayIndex::g_MugongTemplate.GetData(dwMugongID);

		if( pMugongData)
		{
			int nResID	= pMugongData->GetInt(1);
			int nSeq	= pMugongData->GetInt(11);

			switch(bType)
			{
			case MUGONGTYPE_FIVEELEMENT:
				{
					g_pUIManager->SetData(WINDOW_FIVEELEMENTS, fiveelements_window_skill_dummy_01 + nSeq, TEXTURE, nResID+bLevel);

					SetFiveElementToolTip(dwMugongID, WINDOW_FIVEELEMENTS, fiveelements_window_skill_dummy_01 + nSeq);
				}
				break;
			case MUGONGTYPE_PASSIVE:
				{
					if( nSeq < 8)
						g_pUIManager->SetData(WINDOW_INSIDE, inside_window_negong_01 + nSeq - 1, CURRENT_INDEX, 0);
					else
						g_pUIManager->SetData(WINDOW_INSIDE, inside_window_negong_01 + nSeq - 1, TEXTURE, nResID);

					SetMugongToolTip( dwMugongID, WINDOW_INSIDE, inside_window_negong_01 + nSeq - 1);
				}
				break;

			case MUGONGTYPE_ACTIVE:
			default:
				{
					g_pUIManager->SetData(WINDOW_OUTSIDE, outside_window_mugong_dummy_01 + nSeq - 1, TEXTURE, nResID);
					SetMugongToolTip( dwMugongID, WINDOW_OUTSIDE, outside_window_mugong_dummy_01 + nSeq - 1);
				}
				break;
			}
		}
	}
}

void CMugong::CheckMugongSelected()
{
	if( g_MainCharInfo.m_pHoldItem->IsHoldingItemMugong())
		return;

	//if(!(g_pUIManager->IsShow(WINDOW_OUTSIDE) || g_pUIManager->IsShow(WINDOW_SKILL)))////HO_0709_07 : 진각성 내공 프레임 추가전코드
	//	return;
	if(!(g_pUIManager->IsShow(WINDOW_OUTSIDE) || g_pUIManager->IsShow(WINDOW_SKILL)|| g_pUIManager->IsShow(WINDOW_INSIDE)))//HO_0709_07 : 진각성 내공 프레임 : 무공 이미지 체크
		return;

	if( XiahInput::g_bLButtonDown)
	{
		if(g_pUIManager->IsShow(WINDOW_OUTSIDE))
		{
			for(int i=1; i < 13; ++i)
			{
				if( g_pUIManager->IsMouseOn(WINDOW_OUTSIDE, outside_window_mugong_dummy_01 + i -1))
				{
					DWORD dwMugongID = FindMugongByIndex( 1, i);
					g_MainCharInfo.m_pHoldItem->SetHoldItemMugong( dwMugongID);

					if( !g_MainCharInfo.m_pHoldItem->IsHoldingItemMugong())
						return;
				}
			}
		}
		else if(g_pUIManager->IsShow(WINDOW_SKILL))
		{
			for(int i=0; i < 4; ++i)
			{
				if( g_pUIManager->IsMouseOn(WINDOW_SKILL, skill_window_mugong_dummy_01 + i))
				{
                    DWORD dwMugongID = FindRebirthMugongByIndex(i + 4);

					if(dwMugongID)
					{
						g_MainCharInfo.m_pHoldItem->SetHoldItemMugong(dwMugongID);

						if( !g_MainCharInfo.m_pHoldItem->IsHoldingItemMugong())
							return;
					}					
				} 
			}
		} 
		else if(g_pUIManager->IsShow(WINDOW_INSIDE))//HO_0709_07 : 진각성 내공 프레임 : 무공 이미지 체크
		{
			for(int i=0; i < 2; ++i)
			{
				if( g_pUIManager->IsMouseOn(WINDOW_INSIDE, inside_window_mugong_dummy_01 + i))
				{
					DWORD dwMugongID = Find2ThRebirthMugongByIndex(i + 3);
					g_MainCharInfo.m_pHoldItem->SetHoldItemMugong( dwMugongID);

					if( !g_MainCharInfo.m_pHoldItem->IsHoldingItemMugong())
						return;
				}
			}
		}
		
	} 
}


void CMugong::CheckMugongUnSelected()
{
	if( !g_MainCharInfo.m_pHoldItem->IsHoldingItemMugong())
		return;

	if( XiahInput::g_bLButtonDown)
	{
		for(int i=0; i < 5; ++i)
		{
			if( g_pUIManager->IsMouseOn(MAIN_FRAME, main_frame_socket_dummy_02 + i))
			{
				// 입만 아프지만... 슬롯쪽에서 아이템 검사하고 여기서는 무공하고.. -_-;

				// 퀵 슬롯 확장
				if(g_MainCharInfo.m_pSlot->GetCurrentSlotGroup() >= 1)
					i += 5;

				SendCS_IT_SETSLOT_REQ( g_MainCharInfo.m_pHoldItem->GetHoldItemMugong(), i + 1);

				return;
			}
		}

		g_MainCharInfo.m_pHoldItem->SetHoldItemMugong( 0);
	}
}


/**
 *
 * \param MugongType 
 * \param Index 
 * \return 
 */
int CMugong::FindMugongByIndex( BYTE MugongType, BYTE Index)
{
	sArrayData* pData = NULL;

	switch( MugongType)
	{
	case MUGONGTYPE_PASSIVE:
		{
			switch( g_MainCharInfo.m_bCharType)
			{
			case 1:
				pData = XiahArrayIndex::g_MugongIndex_InGum.GetData( Index);
				break;
			case 2:
				pData = XiahArrayIndex::g_MugongIndex_InYun.GetData( Index);
				break;
			case 3:
				pData = XiahArrayIndex::g_MugongIndex_InMutu.GetData( Index);
				break;
			case 4:
				pData = XiahArrayIndex::g_MugongIndex_InYaCha.GetData( Index);
				break;
			}
		}
		break;
	case MUGONGTYPE_ACTIVE:
	default:
		{
			switch( g_MainCharInfo.m_bCharType)
			{
			case 1:
				pData = XiahArrayIndex::g_MugongIndex_OutGum.GetData( Index);
				break;
			case 2:
				pData = XiahArrayIndex::g_MugongIndex_OutYun.GetData( Index);
				break;
			case 3:
				pData = XiahArrayIndex::g_MugongIndex_OutMutu.GetData( Index);
				break;
			case 4:
				pData = XiahArrayIndex::g_MugongIndex_OutYaCha.GetData( Index);
				break;
			}
		}
		break;
	}

	if( pData)
	{
		DWORD nMugongID = pData->GetInt(1);

		if( ( nMugongID))
			return nMugongID;
	}

	return 0;
}


void CMugong::RefreshMugongContent()
{
	//RefreshMugongOutside();
	//RefreshMugongInside();

	MugongMap::iterator it = m_mapMugong.begin();

	for( ; it != m_mapMugong.end(); ++it)
	{
		sMugongInfo* pMugong = it->second;

		if( pMugong)
		{
			sArrayData* pData = XiahArrayIndex::g_MugongTemplate.GetData( pMugong->m_dwMugongID);

			if( pData)
			{
				switch( pMugong->m_byMugongType)
				{
				case MUGONGTYPE_FIVEELEMENT:
					{
						SetFiveElementToolTip(pMugong->m_dwMugongID, WINDOW_FIVEELEMENTS, fiveelements_window_skill_dummy_01 + pData->GetInt(11));
					}
					break;
				case MUGONGTYPE_PASSIVE:	// 내공
					{
						//레벨
						int nLevelControlSeq = inside_window_negong_skill_01 + pData->GetInt( 11) - 1;
						g_pUIManager->SetString(WINDOW_INSIDE, nLevelControlSeq, pMugong->m_byMugongLevel);
						//툴팁
						int nToolTipControlSeq = inside_window_negong_01 + pData->GetInt( 11) - 1;
						SetMugongToolTip( pMugong->m_dwMugongID, WINDOW_INSIDE, nToolTipControlSeq);
					}
					break;

				case MUGONGTYPE_ACTIVE:		// 외공
				default:
					{
						//레벨
						int nLevelControlSeq = outside_window_mugong_skill_01 + pData->GetInt( 11) - 1;
						g_pUIManager->SetString(WINDOW_OUTSIDE, nLevelControlSeq, pMugong->m_byMugongLevel);
						//툴팁
						int nToolTipControlSeq = outside_window_mugong_dummy_01 + pData->GetInt( 11) - 1;
						SetMugongToolTip( pMugong->m_dwMugongID, WINDOW_OUTSIDE, nToolTipControlSeq);
					}
					break;
				}
			} // if( pData)
		} // if( pMugong)
	} // for( ; it != m_mapMugong.end(); ++it)

	//HT_0711 : 진각성 무공
	it = m_map2ThRebirthMugong.begin();

	for( ; it != m_map2ThRebirthMugong.end(); ++it)
	{
		sMugongInfo* pMugong = it->second;

		if( pMugong)
		{
			sArrayData* pData = XiahArrayIndex::g_MugongTemplate.GetData( pMugong->m_dwMugongID);

			if( pData)
			{
				switch( pMugong->m_byMugongType)
				{
				case MUGONGTYPE_2TH_REBIRTH1:		//HT_0711 :진각성 무공
					{
						//레벨
						int nLevelControlSeq = inside_window_negong_skill_10;
						g_pUIManager->SetString(WINDOW_INSIDE, nLevelControlSeq, pMugong->m_byMugongLevel);
						//툴팁
						int nToolTipControlSeq = inside_window_mugong_dummy_01;
						SetMugongToolTip( pMugong->m_dwMugongID, WINDOW_INSIDE, nToolTipControlSeq);
					}
					break;
				case MUGONGTYPE_2TH_REBIRTH2:		
					{
						//레벨
						int nLevelControlSeq = inside_window_negong_skill_11;
						g_pUIManager->SetString(WINDOW_INSIDE, nLevelControlSeq, pMugong->m_byMugongLevel);
						//툴팁
						int nToolTipControlSeq = inside_window_mugong_dummy_02;
						SetMugongToolTip( pMugong->m_dwMugongID, WINDOW_INSIDE, nToolTipControlSeq);
					}
					break;
				}
			} // if( pData)
		} // if( pMugong)
	} // for( ; it != m_mapMugong.end(); ++it)
}

////////////////////////////////////
void CMugong::RefreshMugongOutside()
////////////////////////////////////
{
	g_pUIManager->SetString(WINDOW_OUTSIDE, outside_window_training_point_name_02, g_MainCharInfo.m_wRemainTp);

	for(int i=1; i < 13; ++i)
	{
		sArrayData* pData = NULL;

		switch( g_MainCharInfo.m_bCharType)
		{
		case 1:
			pData = XiahArrayIndex::g_MugongIndex_OutGum.GetData( i);
			break;
		case 2:
			pData = XiahArrayIndex::g_MugongIndex_OutYun.GetData( i);
			break;
		case 3:
			pData = XiahArrayIndex::g_MugongIndex_OutMutu.GetData( i);
			break;
		case 4:
			pData = XiahArrayIndex::g_MugongIndex_OutYaCha.GetData( i);
			break;
		}

		if( pData)
		{
			int nMugongID = pData->GetInt( 1);

			sArrayData* pMugongData = XiahArrayIndex::g_MugongTemplate.GetData( nMugongID);

			if( pMugongData)
			{
				// 이름
				sString szMugongName = pMugongData->GetString( 1);
				g_pUIManager->SetString(WINDOW_OUTSIDE, outside_window_negong_name_dummy_01 + i -1, (LPCTSTR)szMugongName);
				// 레벨
				g_pUIManager->SetString(WINDOW_OUTSIDE, outside_window_mugong_skill_01 + i -1, 0);
				// 아이콘 이미지
				int nResID = pMugongData->GetInt(1);				
				g_pUIManager->SetData(WINDOW_OUTSIDE, outside_window_mugong_dummy_01 + i - 1, TEXTURE, nResID + 2);
				// 툴팁
				SetMugongToolTip( nMugongID, WINDOW_OUTSIDE, outside_window_mugong_dummy_01 + i - 1);
			}
		}
		else
		{
			DBG_LogFile( _T("RefreshMugongOutside() fail"));
		}
	}
}

///////////////////////////////////
void CMugong::RefreshMugongInside()
///////////////////////////////////
{
	g_pUIManager->SetString(WINDOW_INSIDE, inside_window_training_point_dummy_02, g_MainCharInfo.m_wRemainTp);

	for( int i=1; i < 10; ++i)
	{
		sArrayData* pData = NULL;

		switch( g_MainCharInfo.m_bCharType)
		{
		case 1:
			pData = XiahArrayIndex::g_MugongIndex_InGum.GetData( i);
			break;
		case 2:
			pData = XiahArrayIndex::g_MugongIndex_InYun.GetData( i);
			break;
		case 3:
			pData = XiahArrayIndex::g_MugongIndex_InMutu.GetData( i);
			break;
		case 4:
			pData = XiahArrayIndex::g_MugongIndex_InYaCha.GetData( i);
			break;
		}

		if( pData)
		{
			int nMugongID = pData->GetInt( 1);

			sArrayData* pMugongData = XiahArrayIndex::g_MugongTemplate.GetData( nMugongID);

			if( pMugongData)
			{
				// 이름
				sString szMugongName = pMugongData->GetString( 1);
				g_pUIManager->SetString(WINDOW_INSIDE, inside_window_negong_name_dummy_01 + i -1, (LPCTSTR)szMugongName);
				// 레벨
				g_pUIManager->SetString(WINDOW_INSIDE, inside_window_negong_skill_01 + i -1, 0);
				// 툴팁
				SetMugongToolTip( nMugongID, WINDOW_INSIDE, inside_window_negong_01 + i - 1);
				// 아이콘 이미지
				if( i > 7)
				{
					int nResID = pMugongData->GetInt(1);

					g_pUIManager->SetData(WINDOW_INSIDE, inside_window_negong_01 + i - 1, TEXTURE, nResID);
				}
			}
		}
		else
		{
			DBG_LogFile( _T("RefreshMugongInside() fail"));
		}
	}

	//HT_0711 : 진각성 내공 프레임 : 흡성 신공 표시
	for(i=0; i<2; i++)
	{
		DWORD dwMugongID = Find2ThRebirthMugongByIndex(i + 3);

		sArrayData* pMugongData = XiahArrayIndex::g_MugongTemplate.GetData( dwMugongID  + i );

		if( pMugongData)
		{
			// 이름
			sString szMugongName = pMugongData->GetString( 1);

			g_pUIManager->SetString(WINDOW_INSIDE, inside_window_negong_name_dummy_10 + i, (LPCTSTR)szMugongName);

			// 레벨
			g_pUIManager->SetString(WINDOW_INSIDE, inside_window_negong_skill_10 + i, 0);

			// 툴팁
			if( g_MainCharInfo.m_pMugong)
				g_MainCharInfo.m_pMugong->SetMugongToolTip( dwMugongID + i, WINDOW_INSIDE, inside_window_mugong_dummy_01 + i);
			
			// 아이콘 이미지
			int nResID = pMugongData->GetInt(1);
			g_pUIManager->SetData(WINDOW_INSIDE, inside_window_mugong_dummy_01 + i, TEXTURE, nResID);
		}
		else
		{
			DBG_LogFile( _T("XiahGame_Intro::Init_WindowInSide 실패"));
		}
	}
}


/**
 *
 * \param dwMugongID 
 * \return 
 */
BOOL CMugong::IsLearnedMugong(DWORD dwMugongID)
{
	if(m_mapMugong.find( dwMugongID) != m_mapMugong.end())
		return TRUE;
	else if(m_map2ThRebirthMugong.find( dwMugongID) != m_map2ThRebirthMugong.end())	//HT_0711 : 
		return TRUE;
	else 
		return FALSE;
}


/**
 *
 * \param dwMugongID 
 * \return 
 */
BYTE CMugong::GetMugongLevelOfLearnedMugong( DWORD dwMugongID)
{
	//HT_0711 : 진각성 무공
	if(dwMugongID >= 191 && dwMugongID <= 198)
	{
		MugongMap::iterator it = m_map2ThRebirthMugong.find( dwMugongID);

		if( it != m_map2ThRebirthMugong.end())
		{
			sMugongInfo* pInfo = it->second;

			return pInfo->m_byMugongLevel;
		}
	}
	else
	{
		MugongMap::iterator it = m_mapMugong.find( dwMugongID);

		if( it != m_mapMugong.end())
		{
			sMugongInfo* pInfo = it->second;

			return pInfo->m_byMugongLevel;
		}
	}

	return 0;    
}

/////////////////////////////////////////////////////////////////////////////////////////////////////

#define SETMUGONGTOOLTIP(idx,str,color) \
	data = pMugongList->GetInt(idx); \
	if( data) {	\
	TCHAR temp[100];	\
	if( idx == 9) _stprintf( temp, str, data/1000);	\
	else _stprintf( temp, str, data);	\
	g_pUIManager->AddToolTip(nFrameID, nControlID, (LPCTSTR)temp, 0);	}

#define SETMUGONGTOOLTIP_COLOR(idx,cond,str) \
	data = pMugongData->GetInt( idx); \
	if( data) { 	\
	if( cond < data) byColorFlag = 2; \
	else 	byColorFlag = 0; \
	TCHAR temp[100]; \
	_stprintf( temp, str, data); \
	g_pUIManager->AddToolTip(nFrameID, nControlID, (LPCTSTR)temp, byColorFlag); }

#define SETMUGONGTOOLTIP_COLOR2(idx,cond,str) \
	data = pMugongList->GetInt( idx); \
	if( data) { 	\
	if( cond < data) byColorFlag = 2; \
	else 	byColorFlag = 0; \
	TCHAR temp[100]; \
	_stprintf( temp, str, data); \
	g_pUIManager->AddToolTip(nFrameID, nControlID, (LPCTSTR)temp, byColorFlag); }

#define SETMUGONGTOOLTIP_2(idx,str,color) \
	data = pMugongList->GetInt(idx); \
	if( data) {	\
	TCHAR temp[100];	\
	_stprintf( temp, str, (float)data/(float)1000);	\
	g_pUIManager->AddToolTip(nFrameID, nControlID, (LPCTSTR)temp, 0);	}

/////////////////////////////////////////////////////////////////////////////////////////////////////

/**
 *
 * \param nMugongID 
 * \param nFrameID 
 * \param nControlID 
 */
void CMugong::SetMugongToolTip( int nMugongID, int nFrameID, int nControlID)
{	
	sArrayData* pMugongData = XiahArrayIndex::g_MugongTemplate.GetData( nMugongID);
	sArrayData* pMugongList = XiahArrayIndex::g_MugongList.GetData( nMugongID, 1);
	BYTE byToolTipType = 0;

	if( !pMugongData || !pMugongList)
	{
		DBG_LogFile( _T("SetMugongToolTip fail"));
		//return;
	}

	// 툴팁 타입
	if( pMugongData->GetInt(3) == 0 && 
		(pMugongData->GetInt(11) == 8 || pMugongData->GetInt(11) == 9))
	{
		if( pMugongData->GetInt( 11) % 2 == 0)
			byToolTipType = 7;
		else
			byToolTipType = 6;
	}
	else
	{
		if( pMugongData->GetInt( 11) % 2 == 0)
			byToolTipType = 6;
		else
			byToolTipType = 7;
	}

	// 무공 이름
	sString szMugongName = pMugongData->GetString( 1);
	g_pUIManager->SetToolTip(nFrameID, nControlID, byToolTipType, (LPCTSTR)szMugongName, 1);

	ArrayText(pMugongData, 0, nFrameID, nControlID);

	int data=0;
	BYTE byColorFlag = 0;
	BYTE byMugongLevel = GetMugongLevelOfLearnedMugong( nMugongID);

	/////////////////////////////////////////////////////////////////////////////////
	// 익힌 무공이다
	/////////////////////////////////////////////////////////////////////////////////
	if( byMugongLevel > 0)
	{
		pMugongList = XiahArrayIndex::g_MugongList.GetData( nMugongID, byMugongLevel);

		if( !pMugongList)
			return;

		/////////////////////////////////////////////////////////////////////////////////////////////////////
		if( nMugongID == 31 || nMugongID == 40)		// 기세배강, 투척무강
		{
			SETMUGONGTOOLTIP(13, IDS_D_ATTK_INC_KISE2, 0);
		}
		// 혈선풍, 도화천, 대수인, 박투술, 흑사장, 오독침, 독무, 광마독공
		else if( nMugongID == 33 || nMugongID == 67 || nMugongID == 97 || nMugongID == 91 || nMugongID == 121 || nMugongID == 122 || nMugongID == 127 ||  nMugongID == 131)
		{
			SETMUGONGTOOLTIP(13, IDS_D_ATTK_INC2, 0);
		}
		else if( nMugongID == 41)	// 분신격
		{
			SETMUGONGTOOLTIP(13, IDS_MUGONG_NEW_04, 0);
		}
		else if( nMugongID == 192)	//HT_0711 : 진각성 무공 (진분신격)
		{
			SETMUGONGTOOLTIP(13, IDS_MUGONG_NEW_04, 0);
		}
		else if( nMugongID == 69)	// 원기신강
		{
			SETMUGONGTOOLTIP(12, MUGONG_TIP2, 0);
		}
		else if( nMugongID == 70)	// 이타생
		{
			SETMUGONGTOOLTIP(20, MUGONG_TIP3, 0);
		}
		else if( nMugongID == 65)	// 연우영
		{
			SETMUGONGTOOLTIP(13, IDS_MUGONG_NEW_05, 0);
		}
		else if( nMugongID == 71)	// 환수유
		{
			SETMUGONGTOOLTIP(13, MUGONG_TIP4, 0);
		}
		// 조양기 / 필강기 / 검제기 / 도황기 / 조양기 부장기
		else if(nMugongID == 12 || nMugongID == 13 || nMugongID == 8 || nMugongID == 9 || nMugongID == 10 || nMugongID == 11 || nMugongID == 14 || nMugongID == 15 )
		{
			SETMUGONGTOOLTIP(13, IDS_D_ATTK_INC2, 0);
		}
		else if( nMugongID == 197 )	//HT_0711 : 진각성 무공(광마신공)
		{
			SETMUGONGTOOLTIP(13, IDS_D_ATTK_INC2, 0);
		}
		else
		{
			SETMUGONGTOOLTIP(12, IDS_D_ATTK_INC, 0);		// 공격력 증가
		}

		/////////////////////////////////////////////////////////////////////////////////////////////////////
		// 선학기, 필강기, 검제기, 도황기, 조양기, 부장기
		if(nMugongID == 10 || nMugongID == 11 || nMugongID == 8 || nMugongID == 9 || nMugongID == 12 || nMugongID == 13 || nMugongID == 14 || nMugongID == 15 )
		{
			SETMUGONGTOOLTIP(15, IDS_D_DEF_INC2, 0);
		}
		else if(nMugongID == 99 || nMugongID == 195) //HT_0711 : 진각성 무공(적운강기, 적운신공)
		{
			SETMUGONGTOOLTIP(15, IDS_MUGONG_NEW_11, 0);
		}
		else
		{
			SETMUGONGTOOLTIP(14, IDS_D_DEF_INC, 0);			// 방어력 증가
		}


		/////////////////////////////////////////////////////////////////////////////////////////////////////
		if( nMugongID == 65)		// 연우영
		{
			SETMUGONGTOOLTIP(17, IDS_MUGONG_NEW_06, 0);
		}
		else if(nMugongID == 95 || nMugongID == 196)	//HT_0711 : 진각성 무공(마령각, 마령신공)
		{
			SETMUGONGTOOLTIP(17, IDS_MUGONG_NEW_10, 0);
		}
		else if( nMugongID == 69)	// 원기신강
		{
			SETMUGONGTOOLTIP(16, MUGONG_TIP5, 0);
		}
		// 조양기 / 필강기 / 검제기 / 도황기 / 조양기 부장기
		else if(nMugongID == 10 || nMugongID == 11 || nMugongID == 8 || nMugongID == 9 || nMugongID == 12 || nMugongID == 13 || nMugongID == 14 || nMugongID == 15 )
		{
			SETMUGONGTOOLTIP(17, IDS_D_AGI_INC2, 0);
		}
		else
		{
			SETMUGONGTOOLTIP(16, IDS_D_AGI_INC, 0);			// 정확도 증가
		}

		SETMUGONGTOOLTIP(18, IDS_D_MAX_LIFE_INC, 0);		// 최대 생명력 증가
		SETMUGONGTOOLTIP(23, IDS_D_MAX_INLIFE_INC, 0);		// 최대 내력 증가

		SETMUGONGTOOLTIP(21, IDS_MUGONG_NEW_13, 0);			// 생명력 회복량 증가
		SETMUGONGTOOLTIP(26, IDS_MUGONG_NEW_14, 0);			// 내력 회복량 증가

		if(nMugongID == 63)			// 전유음
		{
			SETMUGONGTOOLTIP(19, MUGONG_TIP6, 0);			// 대상 생명력 회복량
		}
		else if( nMugongID == 66 /*|| nMugongID == 92*/)	// 이광음
		{
			SETMUGONGTOOLTIP(19, MUGONG_TIP7, 0);			// 단원 생명력 회복량
		}
		else
		{
			SETMUGONGTOOLTIP(19, IDS_D_LIFE_RECO, 0);		// 생명력 회복량
		}

		if(nMugongID == 71)			// 환수유
		{
			SETMUGONGTOOLTIP(20, IDS_MUGONG_NEW_23, 0);		// 소환수 생명력 증가율
		}
		else if(nMugongID == 41)	// 분신격
		{
			SETMUGONGTOOLTIP(20, IDS_MUGONG_NEW_27, 0);		// 분신 "
		}
		else if( nMugongID == 192)	//HT_0711 : 진각성 무공 (진분신격)
		{
			SETMUGONGTOOLTIP(20, IDS_MUGONG_NEW_27, 0);		// 분신 "
		}
		else if(nMugongID != 70)	// 이타생 제외
		{
			SETMUGONGTOOLTIP(20, IDS_D_LIFE_DEC, 0);		// 생명력 감소율
		}

		SETMUGONGTOOLTIP(28, IDS_MUGONG_NEW_22, 0);			// 일격술 증가
		SETMUGONGTOOLTIP(31, IDS_MUGONG_NEW_01, 0);			// 유효시간(초)

		if(nMugongID == 92)			// 파천소
		{
			SETMUGONGTOOLTIP(32, IDS_MUGONG_NEW_03, 0);		// 유효범위
		}
		// [4/18/2005] 무공 툴팁 수정
		else if(nMugongID != 38 && /*nMugongID != 127 &&*/ nMugongID != 129)		// 이형참,독무 제외
		{
			SETMUGONGTOOLTIP(32, IDS_MUGONG_NEW_02, 0);		// 유효거리
		}

		SETMUGONGTOOLTIP(33, IDS_MUGONG_NEW_08, 0);			// 시전성공율

		// etc 1 /////////////////////////////////////////////////////////////////////////////////////////////////////
		if(nMugongID == 68 || nMugongID == 194)			//HT_0711 : 진각성 무공(교감수, 교감신공)
		{
			SETMUGONGTOOLTIP(34, IDS_MUGONG_NEW_07, 0);		// 추가 경험치 흡수율
		}
		else if(nMugongID == 101)	// 반탄강기
		{
			SETMUGONGTOOLTIP(34, IDS_MUGONG_NEW_12, 0);		// 반탄비율
		}
		else if(nMugongID == 127)	// 독무
		{
			SETMUGONGTOOLTIP(34, IDS_MUGONG_NEW_15, 0);		// 타격범위
		}
		else if(nMugongID == 128)	// 쌍도수
		{
			SETMUGONGTOOLTIP(34, IDS_MUGONG_NEW_16, 0);		// 드랍2배 성공율
		}
		else if(nMugongID == 129)	// 만독불진
		{
			SETMUGONGTOOLTIP(34, IDS_MUGONG_NEW_17, 0);		// 방어막내구도
		}
		else if(nMugongID == 131 ||  nMugongID == 197 )	// 광마독공
		{
			SETMUGONGTOOLTIP(34, IDS_MUGONG_NEW_18, 0);		// 받는 데미지 추가율
		}
		else if(nMugongID == 6 || nMugongID == 7)	// 천수공, 호력공
		{
			SETMUGONGTOOLTIP(34, IDS_MUGONG_NEW_09, 0);		// 회복주기
		}
		else if(nMugongID == 31)	// 기세배강
		{
			SETMUGONGTOOLTIP_2(34, IDS_MUGONG_NEW_20, 0);	// 무기 내구도 감소율
		}
		else if(nMugongID == 37)// || nMugongID == 99 || nMugongID == 195)		// 동귀어진, 적운강기 //HT_0711 : 진각성 무공(적운강기, 적운신공)
		{
			SETMUGONGTOOLTIP(34, IDS_MUGONG_NEW_21, 0);		// 공격력 전환 비율
		}
		else if(nMugongID == 125)	// 독내공
		{
			SETMUGONGTOOLTIP(34, IDS_MUGONG_NEW_24, 0);		// 초당 내력 감소량
		}
		else if(nMugongID == 126 || nMugongID == 198 )	//HT_0711 : 진각성 무공(독혈공, 독혈신공)
		{
			SETMUGONGTOOLTIP(34, IDS_MUGONG_NEW_25, 0);		// 초당 생명력 감소량
		}
		else if(nMugongID == 127)	// 독무
		{
			SETMUGONGTOOLTIP(34, IDS_MUGONG_NEW_03, 0);		// 유효범위
		}
		else if(nMugongID == 40)	// 투척무강
		{
			SETMUGONGTOOLTIP(34, IDS_MUGONG_NEW_28, 0);		// 내구력 감소율
		}
		else if(nMugongID != 91 && nMugongID != 71 && nMugongID != 99 && nMugongID != 195)			// 박투술, 환수유 //HT_0711 : 진각성 무공(적운강기, 적운신공)
		{
			SETMUGONGTOOLTIP(34, IDS_MUGONG_NEW_03, 0);		// 유효범위
		}
		/////////////////////////////////////////////////////////////////////////////////////////////////////

		// etc 2
		if(nMugongID == 127)	// 독무
		{
			SETMUGONGTOOLTIP(35, IDS_MUGONG_NEW_19, 0);		// 타격 주기
		}
		
		SETMUGONGTOOLTIP_COLOR2(7, g_MainCharInfo.m_wIpMax, IDS_D_DEMAND_INLIFE);		// 내력 소모량

		/////////////////////////////////////////////////////////////////////////////////////////////////////
		//////////////////////////////////////////////////////////////////////////////////
		// 극성 완료 체크
		//////////////////////////////////////////////////////////////////////////////////
		pMugongList = XiahArrayIndex::g_MugongList.GetData( nMugongID, byMugongLevel + 1);

		if( !pMugongList)
		{
			g_pUIManager->AddToolTip(nFrameID, nControlID, IDS_MUGONG_END, 1);
		}
		else
		{
			g_pUIManager->AddToolTip(nFrameID, nControlID, _T(" "));

			//다음레벨 요구 갑자및 능력치
			if( pMugongList->GetInt(11))
				g_pUIManager->AddToolTip(nFrameID, nControlID, MUGONG_TIP8);

			SETMUGONGTOOLTIP_COLOR2(2, g_MainCharInfo.m_wLevel, MUGONG_TIP9);	// [6/22/2004] e
			SETMUGONGTOOLTIP_COLOR2(3, g_MainCharInfo.m_wStr, MUGONG_TIP10);	// [6/22/2004] e
			SETMUGONGTOOLTIP_COLOR2(4, g_MainCharInfo.m_wSus, MUGONG_TIP12);	// [6/22/2004] e
			SETMUGONGTOOLTIP_COLOR2(5, g_MainCharInfo.m_wDex, MUGONG_TIP11);	// [6/22/2004] e			
			SETMUGONGTOOLTIP_COLOR2(6, g_MainCharInfo.m_wVit, MUGONG_TIP13);	// [6/22/2004] e

			SETMUGONGTOOLTIP_COLOR2(10, g_MainCharInfo.m_wRemainTp, IDS_D_NEXT_LEVEL_TP);	// [6/22/2004] e

			if( nMugongID == 31 || nMugongID == 40)			// 기세배강, 투척무강
			{
				SETMUGONGTOOLTIP(13, MUGONG_TIP14, 2);
			}
			// 혈선풍, 도화천, 대수인, 박투술, 흑사장, 오독침, 독무, 광마독공
			else if( nMugongID == 33 || nMugongID == 67 || nMugongID == 97 || nMugongID == 91 || nMugongID == 121 || nMugongID == 122 || nMugongID == 127 ||  nMugongID == 131)
			{
				SETMUGONGTOOLTIP(13, IDS_D_NEXT_ATTK_INC2, 2);
			}
			else if( nMugongID == 41)		// 분신격
			{
				SETMUGONGTOOLTIP(13, MUGONG_TIP16, 0);
			}
			else if( nMugongID == 192)	//HT_0711 : 진각성 무공 (진분신격)
			{
				SETMUGONGTOOLTIP(13, MUGONG_TIP16, 0);
			}
			else if( nMugongID == 69)		// 원기신강
			{
				SETMUGONGTOOLTIP(12, MUGONG_TIP17, 0);
			}
			else if( nMugongID == 70)		// 이타생
			{
				SETMUGONGTOOLTIP(20, MUGONG_TIP18, 2);
			}
			else if( nMugongID == 65)		// 연우영
			{
				SETMUGONGTOOLTIP(13, MUGONG_TIP19, 2);
			}
			else if( nMugongID == 71)		// 환수유
			{
				SETMUGONGTOOLTIP(13, MUGONG_TIP20, 0);
			}
			// 조양기 / 필강기 / 검제기 / 도황기 / 조양기 부장기
			else if(nMugongID == 12 || nMugongID == 13 || nMugongID == 8 || nMugongID == 9 || nMugongID == 10 || nMugongID == 11 || nMugongID == 14 || nMugongID == 15 )
			{
				SETMUGONGTOOLTIP(13, IDS_D_ATTK_INC2, 0);
			}
			else if( nMugongID == 197 )	//HT_0711 : 진각성 무공(광마신공)
			{
				SETMUGONGTOOLTIP(13, IDS_D_NEXT_ATTK_INC2, 0);
			}
			else
			{
				SETMUGONGTOOLTIP(12, IDS_D_NEXT_ATTK_INC, 2);		// 공격력 증가
			}

			// 선학기, 필강기, 검제기, 도황기, 조양기, 부장기
			if(nMugongID == 10 || nMugongID == 11 || nMugongID == 8 || nMugongID == 9 || nMugongID == 12 || nMugongID == 13 || nMugongID == 14 || nMugongID == 15 )
			{
				SETMUGONGTOOLTIP(15, IDS_D_NEXT_DEF_INC2, 2);
			}
			else if(nMugongID == 99 || nMugongID == 195) //HT_0711 : 진각성 무공(적운강기, 적운신공)
			{
				SETMUGONGTOOLTIP(15, IDS_MUGONG_NEW_11_NEXT, 2);
			}
			else
			{
				SETMUGONGTOOLTIP(14, IDS_D_NEXT_DEF_INC, 2);			// 방어력 증가
			}


			if( nMugongID == 65)		// 연우영
			{
				SETMUGONGTOOLTIP(17, MUGONG_TIP21, 2);
			}
			else if(nMugongID == 95 || nMugongID == 196)	//HT_0711 : 진각성 무공(마령각, 마령신공)
			{
				SETMUGONGTOOLTIP(17, IDS_MUGONG_NEW_10_NEXT, 2);
			}
			else if( nMugongID == 69)	// 원기신강
			{
				SETMUGONGTOOLTIP(16, MUGONG_TIP22, 2);
			}
			// 조양기 / 필강기 / 검제기 / 도황기 / 조양기 부장기
			else if(nMugongID == 10 || nMugongID == 11 || nMugongID == 8 || nMugongID == 9 || nMugongID == 12 || nMugongID == 13 || nMugongID == 14 || nMugongID == 15 )
			{
				SETMUGONGTOOLTIP(17, IDS_D_NEXT_AGI_INC2, 2);
			}
			else
			{
				SETMUGONGTOOLTIP(16, IDS_D_NEXT_AGI_INC, 2);		// 정확도 증가
			}

			SETMUGONGTOOLTIP(18, IDS_D_MAX_LIFE_INC_NEXT, 2);		// 최대 생명력 증가
			SETMUGONGTOOLTIP(23, IDS_D_MAX_INLIFE_INC_NEXT, 2);		// 최대 내력 증가

			SETMUGONGTOOLTIP(21, IDS_MUGONG_NEW_13_NEXT, 2);		// 생명력 회복량 증가
			SETMUGONGTOOLTIP(26, IDS_MUGONG_NEW_14_NEXT, 2);		// 내력 회복량 증가

			if( nMugongID == 63)		// 전유음
			{
				SETMUGONGTOOLTIP(19, MUGONG_TIP23, 2);				// 대상 생명력 회복량
			}
			else if( nMugongID == 66 /*|| nMugongID == 92*/)		// 이광음
			{
				SETMUGONGTOOLTIP(19, MUGONG_TIP24, 2);				// 단원 생명력 회복량
			}
			else
			{
				SETMUGONGTOOLTIP(19, IDS_D_NEXT_LIFE_RECO, 2);		// 생명력 회복량
			}

			if(nMugongID == 71)			// 환수유
			{
				SETMUGONGTOOLTIP(20, IDS_MUGONG_NEW_23_NEXT, 0);	// 소환수 생명력 증가율
			}
			else if(nMugongID == 41)	// 분신격
			{
				SETMUGONGTOOLTIP(20, IDS_MUGONG_NEW_27_NEXT, 0);	// 분신 "
			}
			else if( nMugongID == 192)	//HT_0711 : 진각성 무공 (진분신격)
			{
				SETMUGONGTOOLTIP(20, IDS_MUGONG_NEW_27_NEXT, 0);	// 분신 "
			}
			else if(nMugongID != 70)	// 이타생 제외
			{
				SETMUGONGTOOLTIP(20, IDS_D_NEXT_LIFE_DEC, 2);		// 생명력 감소율
			}
			
			SETMUGONGTOOLTIP(28, IDS_MUGONG_NEW_22_NEXT, 0);		// 일격술 증가
			SETMUGONGTOOLTIP(31, IDS_MUGONG_NEW_01_NEXT, 2);		// 유효시간(초)


			if(nMugongID == 92)			// 파천소
			{
				SETMUGONGTOOLTIP(32, IDS_MUGONG_NEW_03_NEXT, 2);	// 유효범위
			}
			else if(nMugongID != 38 && nMugongID != 127 && nMugongID != 129)			// 이형참,독무 제외
			{
				SETMUGONGTOOLTIP(32, IDS_MUGONG_NEW_02_NEXT, 2);	// 유효거리
			}

			SETMUGONGTOOLTIP(33,IDS_MUGONG_NEW_08_NEXT, 2);			// 시전성공율

			// etc1 /////////////////////////////////////////////////////////////////////////////////////////////////////
			if(nMugongID == 68 || nMugongID == 194)			//HT_0711 : 진각성 무공(교감수, 교감신공)
			{
				SETMUGONGTOOLTIP(34, IDS_MUGONG_NEW_07_NEXT, 2);	// 추가 경험치 흡수율
			}
			else if(nMugongID == 101)	// 반탄강기
			{
				SETMUGONGTOOLTIP(34, IDS_MUGONG_NEW_12_NEXT, 2);	// 반탄비율
			}
			else if(nMugongID == 127)	// 독무
			{
				SETMUGONGTOOLTIP(34, IDS_MUGONG_NEW_15_NEXT, 0);	// 타격범위
			}
			else if(nMugongID == 128)	// 쌍도수
			{
				SETMUGONGTOOLTIP(34, IDS_MUGONG_NEW_16_NEXT, 0);	// 드랍2배 성공율
			}
			else if(nMugongID == 129)	// 만독불진
			{
				SETMUGONGTOOLTIP(34, IDS_MUGONG_NEW_17_NEXT, 0);	// 방어막내구도
			}
			else if(nMugongID == 131)	// 광마독공
			{
				SETMUGONGTOOLTIP(34, IDS_MUGONG_NEW_18_NEXT, 0);	// 받는 데미지 추가율
			}
			else if(nMugongID == 197)	//HT_0711 : 진각성 무공(광마신공)
			{
				SETMUGONGTOOLTIP(34, IDS_MUGONG_NEW_18_NEXT, 0);	// 받는 데미지 추가율
			}
			else if(nMugongID == 6 || nMugongID == 7)	// 천수공, 호력공
			{
				SETMUGONGTOOLTIP(34, IDS_MUGONG_NEW_09_NEXT, 0);	// 회복주기
			}
			else if(nMugongID == 31)	// 기세배강
			{
				SETMUGONGTOOLTIP_2(34, IDS_MUGONG_NEW_20_NEXT, 0);	// 무기 내구도 감소율
			}
			else if(nMugongID == 37)// || nMugongID == 99 || nMugongID == 195)			// 동귀어진 //HT_0711 : 진각성 무공(적운강기, 적운신공)
			{
				SETMUGONGTOOLTIP(34, IDS_MUGONG_NEW_21_NEXT, 0);	// 공격력 전환 비율
			}
			else if(nMugongID == 125)	// 독내공
			{
				SETMUGONGTOOLTIP(34, IDS_MUGONG_NEW_24_NEXT, 0);	// 초당 내력 감소량
			}
			else if(nMugongID == 126 || nMugongID == 198 )	//HT_0711 : 진각성 무공(독혈공, 독혈신공)
			{
				SETMUGONGTOOLTIP(34, IDS_MUGONG_NEW_25_NEXT, 0);	// 초당 생명력 감소량
			}
			else if(nMugongID == 127)	// 독무
			{
				SETMUGONGTOOLTIP(34, IDS_MUGONG_NEW_03_NEXT, 0);	// 유효범위
			}
			else if(nMugongID == 40)	// 투척무강
			{
				SETMUGONGTOOLTIP(34, IDS_MUGONG_NEW_28_NEXT, 0);	// 내구력 감소율
			}
			else if(nMugongID != 91 && nMugongID != 71 && nMugongID != 99 && nMugongID != 195)				// 박투술, 환수유 //HT_0711 : 진각성 무공(적운강기, 적운신공)
			{
				SETMUGONGTOOLTIP(34,IDS_MUGONG_NEW_03_NEXT, 2);		// 유효범위
			}

			// etc 2
			if(nMugongID == 127)	// 독무
			{
				SETMUGONGTOOLTIP(35, IDS_MUGONG_NEW_19_NEXT, 0);	// 타격 주기
			}

			SETMUGONGTOOLTIP_COLOR2(7, g_MainCharInfo.m_wIpMax, IDS_D_NEXT_LEVEL_INLIFE);		// 내력 소모량
		}

	}
	//////////////////////////////////////////////////////////////////////////////////
	// 안익힌 무공이다
	//////////////////////////////////////////////////////////////////////////////////////
	else
	{
		SETMUGONGTOOLTIP_COLOR(6,	g_MainCharInfo.m_wLevel,	IDS_D_GABJA_DEMAND);		// 요구갑자
		SETMUGONGTOOLTIP_COLOR(7,	g_MainCharInfo.m_wStr,		IDS_D_GUNRYUK);				// 요구근력
		SETMUGONGTOOLTIP_COLOR(8,	g_MainCharInfo.m_wSus,		IDS_D_JIGU);				// 요구지구력
		SETMUGONGTOOLTIP_COLOR(9,	g_MainCharInfo.m_wDex,		IDS_D_MINCHUP);				// 요구민첩력
		SETMUGONGTOOLTIP_COLOR(10,	g_MainCharInfo.m_wVit,		IDS_D_JINKI);				// 요구진기

		SETMUGONGTOOLTIP_COLOR2(10, g_MainCharInfo.m_wRemainTp,	IDS_D_TP_DEMAND);			// 요구 수련치
	}
}

/**
 *
 * \param nMugongID 
 * \param nFrameID 
 * \param nControlID 
 */
void CMugong::SetFiveElementToolTip(int nMugongID, int nFrameID, int nControlID)
{
	sArrayData* pMugongData = XiahArrayIndex::g_MugongTemplate.GetData(nMugongID);
	sArrayData* pMugongList = XiahArrayIndex::g_MugongList.GetData(nMugongID, 1);	

	if(!pMugongData || !pMugongList)
	{
		DBG_LogFile( _T("SetMugongToolTip fail"));
		return;
	}	

	// 무공 이름	
	g_pUIManager->SetToolTip(nFrameID, nControlID, 6, (LPCTSTR)pMugongData->GetString(1), 1);

	ArrayText(pMugongData, 0, nFrameID, nControlID);

	int data=0;
	BYTE byColorFlag = 0;
	BYTE byMugongLevel = GetMugongLevelOfLearnedMugong( nMugongID);


	/////////////////////////////////////////////////////////////////////////////////
	// 익힌 무공이다
	/////////////////////////////////////////////////////////////////////////////////
	if(byMugongLevel > 0)
	{
		pMugongList = XiahArrayIndex::g_MugongList.GetData( nMugongID, byMugongLevel);

		if(!pMugongList)
			return;

		SETMUGONGTOOLTIP(13, IDS_D_ATTK_INC2, 0);			// 공격력 증가율
		SETMUGONGTOOLTIP(15, IDS_D_DEF_INC2, 0);			// 방어력 증가율
		SETMUGONGTOOLTIP(31, IDS_MUGONG_FIVE, 0);			// 속성 변경 가능 시간(초) //HT_1010 : 오행무공툴팁수정

		//////////////////////////////////////////////////////////////////////////////////
		// 극성 완료 체크
		//////////////////////////////////////////////////////////////////////////////////
		pMugongList = XiahArrayIndex::g_MugongList.GetData(nMugongID, byMugongLevel + 1);

		if(!pMugongList)
		{
			g_pUIManager->AddToolTip(nFrameID, nControlID, IDS_MUGONG_END, 1);
		}
		else
		{
			g_pUIManager->AddToolTip(nFrameID, nControlID, _T(" "));

			//다음레벨 요구 갑자및 능력치
			if(pMugongList->GetInt(11))
				g_pUIManager->AddToolTip(nFrameID, nControlID, MUGONG_TIP8);
			
			SETMUGONGTOOLTIP_COLOR2(37, g_MainCharInfo.m_wFiveElmPoint, IDS_FE_POINT_NEXT);

			SETMUGONGTOOLTIP(13, IDS_D_NEXT_ATTK_INC2, 0);			// 공격력 증가율
			SETMUGONGTOOLTIP(15, IDS_D_NEXT_DEF_INC2, 0);			// 방어력 증가율
			SETMUGONGTOOLTIP(31, IDS_MUGONG_FIVE_NEXT, 0);			// 다음 레벨 변경 가능 시간(초) //HT_1010 : 오행무공툴팁수정
		}

	}
	else
	{		
		SETMUGONGTOOLTIP_COLOR(6, g_MainCharInfo.m_wFiveElmPoint, IDS_FE_POINT); // 요구 숙련도
	}
}

void CMugong::ArrayText(sArrayData* pData, int nIndex, int nFrameID, int nControlID)
{
	if(pData)
	{
		// 무공 Desc
		sString szMugongTip = pData->GetString(nIndex);

		// 다중 라인을 위해서
		int nLen = szMugongTip.length();

		TCHAR Desc[128] = {0,};
		memcpy(Desc, szMugongTip, nLen);

		TCHAR buf[64] = {0,};
		int c = 0;
		int nCount = 0;

		for(int i=0; i < nLen; ++i)
		{
			if( Desc[i] == '|')
			{
				if( c > 0)
				{
					c = 0;
					g_pUIManager->AddToolTip(nFrameID, nControlID, nCount, (LPCTSTR)buf, 1);

					++nCount;
					memset(buf, 0, sizeof(buf));
				}
			}
			else
			{
				buf[c++] = (char)Desc[i];
			}
		}

		if( c > 0)
		{
			g_pUIManager->AddToolTip(nFrameID, nControlID, nCount, (LPCTSTR)buf, 1);
		}
	}	
}

/**
 * @brief 환생 무공 툴팁
 * \param nMugongID 
 * \param nFrameID 
 * \param nControlID 
 */
void CMugong::SetRebirthToolTip(int nMugongID, int nFrameID, int nControlID)
{	
	BYTE byMugongLevel = GetMugongLevelOfLearnedMugong( nMugongID);
	sArrayData* pMugongData = XiahArrayIndex::g_RebirthMugong_List.GetData(nMugongID, byMugongLevel);

	if(!pMugongData)
	{
		DBG_LogFile( _T("SetRebirthToolTip fail"));
		return;
	}	

	g_pUIManager->SetToolTip(nFrameID, nControlID, 6, (LPCTSTR)pMugongData->GetString(1), 1);

	ArrayText(pMugongData, 0, nFrameID, nControlID);

	TCHAR szTemp[128] = {0,};

	if(pMugongData)
	{
        if(pMugongData->GetInt(4))	// 요구조건:?차각성
		{			
			_stprintf(szTemp, IDS_REBIRTH_NEED_CONDITION, pMugongData->GetInt(4));
			g_pUIManager->AddToolTip(nFrameID, nControlID, szTemp, 0);
		}
		if(pMugongData->GetInt(5))	// 요구갑자:?
		{
			_stprintf(szTemp, IDS_D_GABJA_DEMAND, pMugongData->GetInt(5));
			g_pUIManager->AddToolTip(nFrameID, nControlID, szTemp, 0);
		}
		if(pMugongData->GetInt(6))	// 요구 수련치:?
		{
			_stprintf(szTemp, IDS_D_TP_DEMAND, pMugongData->GetInt(6));
			g_pUIManager->AddToolTip(nFrameID, nControlID, szTemp, 0);
		}
		if(pMugongData->GetInt(7))	// 무공서 필요
		{
			_stprintf(szTemp, IDS_REBIRTH_NEED_CONDITION_01, pMugongData->GetInt(7));
			g_pUIManager->AddToolTip(nFrameID, nControlID, szTemp, 0);
		}
		if(pMugongData->GetInt(8))	// 내력 소모량:?
		{
			_stprintf(szTemp, IDS_D_DEMAND_INLIFE, pMugongData->GetInt(8));
			g_pUIManager->AddToolTip(nFrameID, nControlID, szTemp, 0);
		}
		if(pMugongData->GetInt(9))	// 공격력 증가율(%):?
		{
			_stprintf(szTemp, IDS_D_ATTK_INC2, pMugongData->GetInt(9));
			g_pUIManager->AddToolTip(nFrameID, nControlID, szTemp, 0);
		}
		if(pMugongData->GetInt(10)) // 방어력 증가율(%):?
		{
			_stprintf(szTemp, IDS_D_DEF_INC2, pMugongData->GetInt(10));
			g_pUIManager->AddToolTip(nFrameID, nControlID, szTemp, 0);
		}
		if(pMugongData->GetInt(11)) // 무기 공격 증가율(%):?
		{
			_stprintf(szTemp, IDS_REBIRTH_NEED_CONDITION_02, pMugongData->GetInt(11));
			g_pUIManager->AddToolTip(nFrameID, nControlID, szTemp, 0);
		}
		if(pMugongData->GetInt(12)) // 무기 방어 증가율(%):?
		{
			_stprintf(szTemp, IDS_REBIRTH_NEED_CONDITION_03, pMugongData->GetInt(12));
			g_pUIManager->AddToolTip(nFrameID, nControlID, szTemp, 0);
		}
		if(pMugongData->GetInt(13)) // 무기 정확 증가율(%):?
		{
			_stprintf(szTemp, IDS_REBIRTH_NEED_CONDITION_04, pMugongData->GetInt(13));
			g_pUIManager->AddToolTip(nFrameID, nControlID, szTemp, 0);
		}
		if(pMugongData->GetInt(14)) // 펫 공격력 증가율(%):?
		{
			_stprintf(szTemp, IDS_MUGONG_NEW_05, pMugongData->GetInt(14));
			g_pUIManager->AddToolTip(nFrameID, nControlID, szTemp, 0);
		}
		if(pMugongData->GetInt(15)) // 펫 방어력 증가율(%):?
		{
			_stprintf(szTemp, IDS_REBIRTH_NEED_CONDITION_05, pMugongData->GetInt(15));
			g_pUIManager->AddToolTip(nFrameID, nControlID, szTemp, 0);
		}
		if(pMugongData->GetInt(16)) // 펫 정확도 증가율(%):?
		{
			_stprintf(szTemp, IDS_MUGONG_NEW_06, pMugongData->GetInt(16));
			g_pUIManager->AddToolTip(nFrameID, nControlID, szTemp, 0);
		}
		if(pMugongData->GetInt(17)) // 분신 생명력 증가율(%):?
		{
			_stprintf(szTemp, IDS_MUGONG_NEW_27, pMugongData->GetInt(17));
			g_pUIManager->AddToolTip(nFrameID, nControlID, szTemp, 0);
		}
		if(pMugongData->GetInt(18)) // 펫 생명력 증가율(%):?
		{
			_stprintf(szTemp, IDS_REBIRTH_NEED_CONDITION_06, pMugongData->GetInt(18));
			g_pUIManager->AddToolTip(nFrameID, nControlID, szTemp, 0);
		}
		if(pMugongData->GetInt(19)) // 수련한 내공 효율 증가(%):?
		{
			_stprintf(szTemp, IDS_INPWR_INC, pMugongData->GetInt(19));
			g_pUIManager->AddToolTip(nFrameID, nControlID, szTemp, 0);
		}
		if(pMugongData->GetInt(20)) // 추가 데미지율(%):?
		{
			_stprintf(szTemp, IDS_REBIRTH_NEED_CONDITION_07, pMugongData->GetInt(20));
			g_pUIManager->AddToolTip(nFrameID, nControlID, szTemp, 0);
		}
		if(pMugongData->GetInt(21)) // 생명력 감소량:?
		{
			_stprintf(szTemp, IDS_REBIRTH_NEED_CONDITION_08, pMugongData->GetInt(21));
			g_pUIManager->AddToolTip(nFrameID, nControlID, szTemp, 0);
		}
		if(pMugongData->GetInt(22)) // 공격, 방어, 정확 감소율(%):?
		{
			_stprintf(szTemp, IDS_REBIRTH_NEED_CONDITION_09, pMugongData->GetInt(22));
			g_pUIManager->AddToolTip(nFrameID, nControlID, szTemp, 0);
		}
		if(pMugongData->GetInt(23)) // 유효거리 증가:?
		{
			_stprintf(szTemp, IDS_REBIRTH_NEED_CONDITION_10, pMugongData->GetInt(23));
			g_pUIManager->AddToolTip(nFrameID, nControlID, szTemp, 0);
		}
		if(pMugongData->GetInt(24)) // 무공 흡수율(%):?
		{
			_stprintf(szTemp, IDS_REBIRTH_NEED_CONDITION_11, pMugongData->GetInt(24));
			g_pUIManager->AddToolTip(nFrameID, nControlID, szTemp, 0);
		}
		if(pMugongData->GetInt(25)) // 시전 유효 범위:?
		{
			_stprintf(szTemp, IDS_REBIRTH_NEED_CONDITION_12, pMugongData->GetInt(25));
			g_pUIManager->AddToolTip(nFrameID, nControlID, szTemp, 0);
		}
		if(pMugongData->GetInt(26)) // 1초당 내력 감소량:?
		{
			_stprintf(szTemp, IDS_REBIRTH_NEED_CONDITION_13, pMugongData->GetInt(26));
			g_pUIManager->AddToolTip(nFrameID, nControlID, szTemp, 0);
		}
		if(pMugongData->GetInt(27)) // 지속 시간(초):?
		{
			_stprintf(szTemp, IDS_D_LASTTIME, pMugongData->GetInt(27));
			g_pUIManager->AddToolTip(nFrameID, nControlID, szTemp, 0);
		}
		if(pMugongData->GetInt(28)) // 유효 거리:?
		{
			_stprintf(szTemp, IDS_MUGONG_NEW_02, pMugongData->GetInt(28));
			g_pUIManager->AddToolTip(nFrameID, nControlID, szTemp, 0);
		}
		if(pMugongData->GetInt(29)) // 속성 시전 성공률(%):?
		{
			_stprintf(szTemp, IDS_MUGONG_NEW_08, pMugongData->GetInt(29));
			g_pUIManager->AddToolTip(nFrameID, nControlID, szTemp, 0);
		}
		if(pMugongData->GetInt(30)) // 극성 완료
		{
			_stprintf(szTemp, IDS_MUGONG_END, pMugongData->GetInt(30));
			g_pUIManager->AddToolTip(nFrameID, nControlID, szTemp, 1);
		}
		if(pMugongData->GetInt(31)) // 다음 레벨 요구 수련치:?
		{
			g_pUIManager->AddToolTip(nFrameID, nControlID, _T(" "));
			_stprintf(szTemp, IDS_D_NEXT_LEVEL_TP, pMugongData->GetInt(31));
			g_pUIManager->AddToolTip(nFrameID, nControlID, szTemp, 0);
		}
		if(pMugongData->GetInt(32)) // 다음 레벨 요구 조건 :?차 각성
		{
			_stprintf(szTemp, IDS_REBIRTH_NEED_CONDITION_14, pMugongData->GetInt(32));
			g_pUIManager->AddToolTip(nFrameID, nControlID, szTemp, 0);
		}
		if(pMugongData->GetInt(33)) // 다음 레벨 내력 소모량:?
		{
			_stprintf(szTemp, IDS_D_NEXT_LEVEL_INLIFE, pMugongData->GetInt(33));
			g_pUIManager->AddToolTip(nFrameID, nControlID, szTemp, 0);
		}
		if(pMugongData->GetInt(34)) // 다음 레벨 공격력 증가율(%):?
		{
			_stprintf(szTemp, IDS_D_NEXT_ATTK_INC2, pMugongData->GetInt(34));
			g_pUIManager->AddToolTip(nFrameID, nControlID, szTemp, 0);
		}
		if(pMugongData->GetInt(35)) // 다음 레벨 방어력 증가율(%):?
		{
			_stprintf(szTemp, IDS_D_NEXT_DEF_INC2, pMugongData->GetInt(35));
			g_pUIManager->AddToolTip(nFrameID, nControlID, szTemp, 0);
		}
		if(pMugongData->GetInt(36)) // 다음 레벨 무기 공격 증가율(%):?
		{
			_stprintf(szTemp, IDS_REBIRTH_NEED_CONDITION_27, pMugongData->GetInt(36));
			g_pUIManager->AddToolTip(nFrameID, nControlID, szTemp, 0);
		}
		if(pMugongData->GetInt(37)) // 다음 레벨 무기 방어 증가율(%):?
		{
			_stprintf(szTemp, IDS_REBIRTH_NEED_CONDITION_15, pMugongData->GetInt(37));
			g_pUIManager->AddToolTip(nFrameID, nControlID, szTemp,0);
		}
		if(pMugongData->GetInt(38)) // 다음 레벨 무기 정확 증가율(%):?
		{
			_stprintf(szTemp, IDS_REBIRTH_NEED_CONDITION_16, pMugongData->GetInt(38));
			g_pUIManager->AddToolTip(nFrameID, nControlID, szTemp, 0);
		}
		if(pMugongData->GetInt(39)) // 다음 레벨 펫 공격력 증가율(%):?
		{
			_stprintf(szTemp, MUGONG_TIP19, pMugongData->GetInt(39));
			g_pUIManager->AddToolTip(nFrameID, nControlID, szTemp, 0);
		}
		if(pMugongData->GetInt(40)) // 다음 레벨 펫 방어력 증가율(%):?
		{
			_stprintf(szTemp, IDS_REBIRTH_NEED_CONDITION_17, pMugongData->GetInt(40));
			g_pUIManager->AddToolTip(nFrameID, nControlID, szTemp, 0);
		}
		if(pMugongData->GetInt(41)) // 다음 레벨 펫 정확도 증가율(%):?
		{
			_stprintf(szTemp, MUGONG_TIP21, pMugongData->GetInt(41));
			g_pUIManager->AddToolTip(nFrameID, nControlID, szTemp, 0);
		}
		if(pMugongData->GetInt(42)) // 다음 레벨 분신 생명력 증가율(%):?
		{
			_stprintf(szTemp, IDS_MUGONG_NEW_27_NEXT, pMugongData->GetInt(42));
			g_pUIManager->AddToolTip(nFrameID, nControlID, szTemp, 0);
		}
		if(pMugongData->GetInt(43)) // 다음 레벨 펫 생명력 증가율(%):?
		{
			_stprintf(szTemp, IDS_REBIRTH_NEED_CONDITION_18, pMugongData->GetInt(43));
			g_pUIManager->AddToolTip(nFrameID, nControlID, szTemp, 0);
		}
		if(pMugongData->GetInt(44)) // 다음 레벨 내공 효율 증가:?
		{
			_stprintf(szTemp, IDS_REBIRTH_NEED_CONDITION_19, pMugongData->GetInt(44));
			g_pUIManager->AddToolTip(nFrameID, nControlID, szTemp, 0);
		}
		if(pMugongData->GetInt(45)) // 다음 레벨 추가 데미지율(%):?
		{
			_stprintf(szTemp, IDS_REBIRTH_NEED_CONDITION_20, pMugongData->GetInt(45));
			g_pUIManager->AddToolTip(nFrameID, nControlID, szTemp, 0);
		}
		if(pMugongData->GetInt(46)) // 다음 레벨 생명력 감소량:?
		{
			_stprintf(szTemp, IDS_REBIRTH_NEED_CONDITION_21, pMugongData->GetInt(46));
			g_pUIManager->AddToolTip(nFrameID, nControlID, szTemp, 0);
		}
		if(pMugongData->GetInt(47)) // 다음 레벨 공격, 방어, 정확 감소율(%):?
		{
			_stprintf(szTemp, IDS_REBIRTH_NEED_CONDITION_22, pMugongData->GetInt(47));
			g_pUIManager->AddToolTip(nFrameID, nControlID, szTemp, 0);
		}
		if(pMugongData->GetInt(48)) // 다음 레벨 유효거리 증가:?
		{
			_stprintf(szTemp, IDS_REBIRTH_NEED_CONDITION_23, pMugongData->GetInt(48));
			g_pUIManager->AddToolTip(nFrameID, nControlID, szTemp, 0);
		}
		if(pMugongData->GetInt(49)) // 다음 레벨 무공 흡수율(%):?
		{
			_stprintf(szTemp, IDS_REBIRTH_NEED_CONDITION_24, pMugongData->GetInt(49));
			g_pUIManager->AddToolTip(nFrameID, nControlID, szTemp, 0);
		}
		if(pMugongData->GetInt(50)) // 다음 레벨 시전 유효 범위:?
		{
			_stprintf(szTemp, IDS_REBIRTH_NEED_CONDITION_25, pMugongData->GetInt(50));
			g_pUIManager->AddToolTip(nFrameID, nControlID, szTemp, 0);
		}
		if(pMugongData->GetInt(51)) // 다음 레벨 1초당 내력 감소량:?
		{
			_stprintf(szTemp, IDS_REBIRTH_NEED_CONDITION_26, pMugongData->GetInt(51));
			g_pUIManager->AddToolTip(nFrameID, nControlID, szTemp, 0);
		}
		if(pMugongData->GetInt(52)) // 다음 레벨 지속 시간(초):?
		{
			_stprintf(szTemp, IDS_D_NEXT_LASTTIME, pMugongData->GetInt(52));
			g_pUIManager->AddToolTip(nFrameID, nControlID, szTemp, 0);
		}
		if(pMugongData->GetInt(53)) // 다음 레벨 유효 거리:?
		{
			_stprintf(szTemp, IDS_MUGONG_NEW_02_NEXT, pMugongData->GetInt(53));
			g_pUIManager->AddToolTip(nFrameID, nControlID, szTemp, 0);
		}
		if(pMugongData->GetInt(54)) // 다음 레벨 시전 성공율(%):?
		{
			_stprintf(szTemp, IDS_REBIRTH_NEED_CONDITION_28, pMugongData->GetInt(54));
			g_pUIManager->AddToolTip(nFrameID, nControlID, szTemp, 0);
		}
	}
}

DWORD CMugong::FindRebirthMugongByIndex(BYTE bySeq)
{
	MugongMap::iterator it = m_mapMugong.begin();

	for( ; it != m_mapMugong.end(); ++it)
	{
		sMugongInfo* pMugong = it->second;

		if( pMugong)
		{
			sArrayData* pData = XiahArrayIndex::g_RebirthMugong_List.GetData(pMugong->m_dwMugongID);

			if( pData)
			{
				if(pData->GetInt(55) == bySeq)
				{
					return pMugong->m_dwMugongID;
				}
			}
		}
	}

	return 0;
}

DWORD CMugong::Find2ThRebirthMugongByIndex(BYTE bySeq) //HT_0711 : 진각성 무공 
{
	MugongMap::iterator it = m_map2ThRebirthMugong.begin();

	for( ; it != m_map2ThRebirthMugong.end(); ++it)
	{
		sMugongInfo* pMugong = it->second;

		if( pMugong)
		{
			sArrayData* pData = XiahArrayIndex::g_MugongList.GetData(pMugong->m_dwMugongID);

			if( pData)
			{
				if(pMugong->m_byMugongType == bySeq)
				{
					return pMugong->m_dwMugongID;
				}
			}
		}
	}

	return 0;
}

void CMugong::CreateKeepUpIcon() //HO_0703_07 게임내 심의 등급 표기
{
	sRect rtRect;
	
	if(!m_pKeepUpIconVB)
	g_pDirect3DDevice->CreateVertexBuffer( 4*sizeof(VT_TLVertex), 0, D3DFVF_TLVERTEX,
													D3DPOOL_MANAGED, &m_pKeepUpIconVB, NULL);

	
	//rtRect.left = 362;
	//rtRect.top = 0;
	//rtRect.right = rtRect.left + 280;
	//rtRect.bottom = rtRect.top + 115;
	rtRect.left = WINDOW_FIRST_XPOS;
	rtRect.top = 0;
	rtRect.right = rtRect.left + 280;
	rtRect.bottom = rtRect.top + 108;

	VT_TLVertex Vertex[4];

	Vertex[ 0].pos = Vector4( rtRect.left, rtRect.top, 0, 1);
	Vertex[ 1].pos = Vector4( rtRect.right, rtRect.top, 0, 1);
	Vertex[ 2].pos = Vector4( rtRect.left, rtRect.bottom, 0, 1);
	Vertex[ 3].pos = Vector4( rtRect.right, rtRect.bottom, 0, 1);
	
	Vertex[ 0].diffuse = Vertex[ 1].diffuse = Vertex[ 2].diffuse = Vertex[ 3].diffuse = 0xffffffff;

	Vertex[ 0].tex = Vector2( 0, 0);
	Vertex[ 1].tex = Vector2( 1, 0);
	Vertex[ 2].tex = Vector2( 0, 1);
	Vertex[ 3].tex = Vector2( 1, 1);

	VOID* pVertices;
	if( !FAILED( m_pKeepUpIconVB->Lock( 0, sizeof(Vertex), (void**)&pVertices, 0 )))
	{
		memcpy( pVertices, Vertex, sizeof(Vertex) );
		m_pKeepUpIconVB->Unlock();
	}

}

void CMugong::CreateKeepUpMugongIcon()
{
	sRect rtRect;
	
	for( int i=0; i < 15; ++i)
	{
		if(!m_pKeepUpVB[i])
			g_pDirect3DDevice->CreateVertexBuffer( 4*sizeof(VT_TLVertex), 0, D3DFVF_TLVERTEX,
													D3DPOOL_MANAGED, &m_pKeepUpVB[i], NULL);

		if(i < 13) //일반 무공 지속 아이콘 VB
		{
			rtRect.left = (30*i) + 215;
			rtRect.top = 0;
			rtRect.right = rtRect.left + 30;
			rtRect.bottom = rtRect.top + 30;
		}
		else //펫 무공 지속 아이콘 VB
		{
			rtRect.left = 710 - (30*(i-13));
			rtRect.top = 0;
			rtRect.right = rtRect.left + 30;
			rtRect.bottom = rtRect.top + 30;
		}

		VT_TLVertex Vertex[4];

		Vertex[ 0].pos = Vector4( rtRect.left, rtRect.top, 0, 1);
		Vertex[ 1].pos = Vector4( rtRect.right, rtRect.top, 0, 1);
		Vertex[ 2].pos = Vector4( rtRect.left, rtRect.bottom, 0, 1);
		Vertex[ 3].pos = Vector4( rtRect.right, rtRect.bottom, 0, 1);
		
		Vertex[ 0].diffuse = Vertex[ 1].diffuse = Vertex[ 2].diffuse = Vertex[ 3].diffuse = 0xffffffff;

		Vertex[ 0].tex = Vector2( 0, 0);
		Vertex[ 1].tex = Vector2( 1, 0);
		Vertex[ 2].tex = Vector2( 0, 1);
		Vertex[ 3].tex = Vector2( 1, 1);

		VOID* pVertices;
		if( !FAILED( m_pKeepUpVB[i]->Lock( 0, sizeof(Vertex), (void**)&pVertices, 0 )))
		{
			memcpy( pVertices, Vertex, sizeof(Vertex) );
			m_pKeepUpVB[i]->Unlock();
		}
	}
}

void CMugong::DrawKeepUpIcon() //HO_0703_07 게임내 심의 등급 표기 
{
	g_pDirect3DDevice->SetRenderState( D3DRS_ZENABLE, FALSE);
	g_pDirect3DDevice->SetRenderState( D3DRS_FOGENABLE, FALSE);

	g_pDirect3DDevice->SetRenderState( D3DRS_ALPHABLENDENABLE, TRUE);
	g_pDirect3DDevice->SetRenderState( D3DRS_DIFFUSEMATERIALSOURCE, D3DMCS_COLOR1);
	g_pDirect3DDevice->SetRenderState( D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	g_pDirect3DDevice->SetRenderState( D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
	
	g_pDirect3DDevice->SetSamplerState( 0,  D3DSAMP_ADDRESSU , D3DTADDRESS_CLAMP);
	g_pDirect3DDevice->SetSamplerState( 0,  D3DSAMP_ADDRESSV , D3DTADDRESS_CLAMP);
	
	if(KeepUpIconTimer + 3000 >= g_dwCurTime)
	{
		g_Device.SetTexture(0, XiahPak::GetTexture(1608));
		g_Device.SetStreamSource( m_pKeepUpIconVB, sizeof(VT_TLVertex));
		g_Device.SetFVF(D3DFVF_TLVERTEX);
		g_pDirect3DDevice->DrawPrimitive( D3DPT_TRIANGLESTRIP, 0, 2);				
	}
	else if(KeepUpIconTimer + 3600000 < g_dwCurTime)
		KeepUpIconTimer = g_dwCurTime;	
}

void CMugong::DrawKeepUpMugongIcon()
{
	int nSize = g_MainCharInfo.m_vkeepUpMugongIconList.size();
	int nResID, i;

	std::vector<sKEEPUPMUGONGICONLIST*>::iterator iter = g_MainCharInfo.m_vkeepUpMugongIconList.begin();
	std::vector<sKEEPUPMUGONGICONLIST*>::iterator iter2 = g_MainCharInfo.m_vkeepUpPetMugongIconList.begin();

	for(i=0; iter != g_MainCharInfo.m_vkeepUpMugongIconList.end(); ++iter)
	{
		if(i > 12) //무공 지속 아이콘은 12개를 넘길 수 없다.
			break;

		g_pDirect3DDevice->SetRenderState( D3DRS_ZENABLE, FALSE);
		g_pDirect3DDevice->SetRenderState( D3DRS_FOGENABLE, FALSE);

		g_pDirect3DDevice->SetRenderState( D3DRS_ALPHABLENDENABLE, TRUE);
		g_pDirect3DDevice->SetRenderState( D3DRS_DIFFUSEMATERIALSOURCE, D3DMCS_COLOR1);
		g_pDirect3DDevice->SetRenderState( D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
		g_pDirect3DDevice->SetRenderState( D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
		
		g_pDirect3DDevice->SetSamplerState( 0,  D3DSAMP_ADDRESSU , D3DTADDRESS_CLAMP);
		g_pDirect3DDevice->SetSamplerState( 0,  D3DSAMP_ADDRESSV , D3DTADDRESS_CLAMP);

		sKEEPUPMUGONGICONLIST *psKeepUpMugong = (*iter);

		//일반 무공과 각성 무공은 다른 리스트를 쓰므로... 
		//각성 무공
		if(psKeepUpMugong->m_MugongID >= SUNSINGONG && psKeepUpMugong->m_MugongID <= OUTGONGID_DRAGONSUNGCHEON)
		{
			sArrayData* pMugongList = XiahArrayIndex::g_RebirthMugong_List.GetData(psKeepUpMugong->m_MugongID, psKeepUpMugong->m_MugongLevel);
			
			if(!pMugongList)
				continue;
			
			int TotalKeepUpTime = pMugongList->GetInt(27);
			nResID = pMugongList->GetInt(2);
			DWORD TempTime = g_dwCurTime;

			
			//if(pMugongList) //HO_0529_07 무공사용이미지 동작 변경 : 적용전코드
			//{
			//	if( (5000 <= ((TotalKeepUpTime * 1000) - (TempTime - psKeepUpMugong->m_CurTime))) && psKeepUpMugong->m_DrawIcon )
			//	{
			//		g_Device.SetTexture(0, XiahPak::GetTexture(nResID));
			//		g_Device.SetStreamSource( m_pKeepUpVB[i], sizeof(VT_TLVertex));
			//		g_Device.SetFVF(D3DFVF_TLVERTEX);
			//		g_pDirect3DDevice->DrawPrimitive( D3DPT_TRIANGLESTRIP, 0, 2);
			//	}
			//	else
			//	{
			//		psKeepUpMugong->m_DrawIcon = false;
			//		if(psKeepUpMugong->m_CurTime + 500 > g_dwCurTime)
			//		{
			//			g_Device.SetTexture(0, XiahPak::GetTexture(nResID));
			//			g_Device.SetStreamSource( m_pKeepUpVB[i], sizeof(VT_TLVertex));
			//			g_Device.SetFVF(D3DFVF_TLVERTEX);
			//			g_pDirect3DDevice->DrawPrimitive( D3DPT_TRIANGLESTRIP, 0, 2);
			//		}
			//		else
			//		{
			//			psKeepUpMugong->m_CurTime = g_dwCurTime;
			//		}
			//	}
			//	++i;
			//}
			if(pMugongList) //HO_0529_07 무공사용이미지 동작 변경 : 0.3초동안 보여지고 안보여짐
			{
				if( (10000 <= ((TotalKeepUpTime * 1000) - (TempTime - psKeepUpMugong->m_CurTime))) && psKeepUpMugong->m_DrawIcon )
				{
					g_Device.SetTexture(0, XiahPak::GetTexture(nResID));
					g_Device.SetStreamSource( m_pKeepUpVB[i], sizeof(VT_TLVertex));
					g_Device.SetFVF(D3DFVF_TLVERTEX);
					g_pDirect3DDevice->DrawPrimitive( D3DPT_TRIANGLESTRIP, 0, 2);
				}
				else
				{
					if (((TotalKeepUpTime * 1000) - (TempTime - psKeepUpMugong->m_CurTime)) <= 0)
					{
						psKeepUpMugong->m_DrawIcon = false;
					}
					else
					{
						psKeepUpMugong->m_DrawIcon = false;
						if( (((TotalKeepUpTime * 1000)-300) <= ((TotalKeepUpTime * 1000) - (TempTime - psKeepUpMugong->m_CurTime))) )
						{
							g_Device.SetTexture(0, XiahPak::GetTexture(nResID));
							g_Device.SetStreamSource( m_pKeepUpVB[i], sizeof(VT_TLVertex));
							g_Device.SetFVF(D3DFVF_TLVERTEX);
							g_pDirect3DDevice->DrawPrimitive( D3DPT_TRIANGLESTRIP, 0, 2);
						}
						else
						{							
							if(psKeepUpMugong->m_CurTime + 600 < g_dwCurTime)
							{
								psKeepUpMugong->m_CurTime = g_dwCurTime;
							}							
						}
					}
				}
				++i;
			}
		}
		//HO_0404_07 환배 시스템추가(아이템)
		else if(psKeepUpMugong->m_MugongID >= 200 && psKeepUpMugong->m_MugongID <= 202)
		{
			switch(psKeepUpMugong->m_MugongID)
			{
			case 200:
				nResID=50002586;
				break;
			case 201:
				nResID=50002585;
				break;
			case 202:
				nResID=50002584;
				break;
			}
			
			int TotalKeepUpTime = psKeepUpMugong->m_MugongLevel * 60;
			DWORD TempTime = g_dwCurTime;

			//if( (5000 <= ((TotalKeepUpTime * 1000) - (TempTime - psKeepUpMugong->m_CurTime))) && psKeepUpMugong->m_DrawIcon ) //HO_0529_07 환배사용이미지 동작 변경 : 아직환배의 구현은 안되었다.. 나중에 빼먹을지 모르니 같은 방식으로 가자
			//	{
			//		g_Device.SetTexture(0, XiahPak::GetTexture(nResID));
			//		g_Device.SetStreamSource( m_pKeepUpVB[i], sizeof(VT_TLVertex));
			//		g_Device.SetFVF(D3DFVF_TLVERTEX);
			//		g_pDirect3DDevice->DrawPrimitive( D3DPT_TRIANGLESTRIP, 0, 2);	
			//	}
			//else
			//{
			//	psKeepUpMugong->m_DrawIcon = false;
			//	if(psKeepUpMugong->m_CurTime + 500 > g_dwCurTime)
			//	{
			//		g_Device.SetTexture(0, XiahPak::GetTexture(nResID));
			//		g_Device.SetStreamSource( m_pKeepUpVB[i], sizeof(VT_TLVertex));
			//		g_Device.SetFVF(D3DFVF_TLVERTEX);
			//		g_pDirect3DDevice->DrawPrimitive( D3DPT_TRIANGLESTRIP, 0, 2);
			//	}
			//	else
			//	{
			//		psKeepUpMugong->m_CurTime = g_dwCurTime;
			//	}
			//}
			//	++i;
			if( (10000 <= ((TotalKeepUpTime * 1000) - (TempTime - psKeepUpMugong->m_CurTime))) && psKeepUpMugong->m_DrawIcon ) //HO_0529_07 환배사용이미지 동작 변경 : 0.3초동안 보여지고 안보여짐
			{
				g_Device.SetTexture(0, XiahPak::GetTexture(nResID));
				g_Device.SetStreamSource( m_pKeepUpVB[i], sizeof(VT_TLVertex));
				g_Device.SetFVF(D3DFVF_TLVERTEX);
				g_pDirect3DDevice->DrawPrimitive( D3DPT_TRIANGLESTRIP, 0, 2);
			}
			else
			{
				psKeepUpMugong->m_DrawIcon = false;
				if( (((TotalKeepUpTime * 1000)-300) <= ((TotalKeepUpTime * 1000) - (TempTime - psKeepUpMugong->m_CurTime))) )
				{
					g_Device.SetTexture(0, XiahPak::GetTexture(nResID));
					g_Device.SetStreamSource( m_pKeepUpVB[i], sizeof(VT_TLVertex));
					g_Device.SetFVF(D3DFVF_TLVERTEX);
					g_pDirect3DDevice->DrawPrimitive( D3DPT_TRIANGLESTRIP, 0, 2);
				}
				else
				{							
					if(psKeepUpMugong->m_CurTime + 600 < g_dwCurTime)
					{
						psKeepUpMugong->m_CurTime = g_dwCurTime;
					}							
				}					
			}
			++i;
		}
		//일반
		else 
		{
			sArrayData* pMugongData = XiahArrayIndex::g_MugongTemplate.GetData(psKeepUpMugong->m_MugongID);
			if(!pMugongData)
			{
				continue;
			}
			
			nResID = pMugongData->GetInt(1);

			int TotalKeepUpTime = 7200; // default 2 hours
			sArrayData* pMugongList = XiahArrayIndex::g_MugongList.GetData(psKeepUpMugong->m_MugongID, psKeepUpMugong->m_MugongLevel);
			if(pMugongList)
			{
				TotalKeepUpTime = pMugongList->GetInt(31);
			}
			else
			{
				// 150-154 allowed without level details
				if (psKeepUpMugong->m_MugongID < 150 || psKeepUpMugong->m_MugongID > 154)
				{
					continue;
				}
			}
			
			DWORD TempTime = g_dwCurTime;

			//if(pMugongList) //HO_0529_07 무공사용이미지 동작 변경 : 적용전코드
			//{
			//	if( (5000 <= ((TotalKeepUpTime * 1000) - (TempTime - psKeepUpMugong->m_CurTime))) && psKeepUpMugong->m_DrawIcon )
			//	{
			//		g_Device.SetTexture(0, XiahPak::GetTexture(nResID));
			//		g_Device.SetStreamSource( m_pKeepUpVB[i], sizeof(VT_TLVertex));
			//		g_Device.SetFVF(D3DFVF_TLVERTEX);
			//		g_pDirect3DDevice->DrawPrimitive( D3DPT_TRIANGLESTRIP, 0, 2);
			//	}
			//	else
			//	{
			//		psKeepUpMugong->m_DrawIcon = false;
			//		if(psKeepUpMugong->m_CurTime + 500 > g_dwCurTime)
			//		{
			//			g_Device.SetTexture(0, XiahPak::GetTexture(nResID));
			//			g_Device.SetStreamSource( m_pKeepUpVB[i], sizeof(VT_TLVertex));
			//			g_Device.SetFVF(D3DFVF_TLVERTEX);
			//			g_pDirect3DDevice->DrawPrimitive( D3DPT_TRIANGLESTRIP, 0, 2);
			//		}
			//		else
			//			psKeepUpMugong->m_CurTime = g_dwCurTime;
			//	}
			//	++i;
			//}
			if(pMugongList || (psKeepUpMugong->m_MugongID >= 150 && psKeepUpMugong->m_MugongID <= 154)) //HO_0529_07 무공사용이미지 동작 변경 : 0.3초동안 보여지고 안보여짐
			{
				if( (10000 <= ((TotalKeepUpTime * 1000) - (TempTime - psKeepUpMugong->m_CurTime))) && psKeepUpMugong->m_DrawIcon )
				{
					g_Device.SetTexture(0, XiahPak::GetTexture(nResID));
					g_Device.SetStreamSource( m_pKeepUpVB[i], sizeof(VT_TLVertex));
					g_Device.SetFVF(D3DFVF_TLVERTEX);
					g_pDirect3DDevice->DrawPrimitive( D3DPT_TRIANGLESTRIP, 0, 2);
				}
				else
				{
					if (((TotalKeepUpTime * 1000) - (TempTime - psKeepUpMugong->m_CurTime)) <= 0)
					{
						psKeepUpMugong->m_DrawIcon = false;
					}
					else
					{
						psKeepUpMugong->m_DrawIcon = false;
						if( (((TotalKeepUpTime * 1000)-300) <= ((TotalKeepUpTime * 1000) - (TempTime - psKeepUpMugong->m_CurTime))) )
						{
							g_Device.SetTexture(0, XiahPak::GetTexture(nResID));
							g_Device.SetStreamSource( m_pKeepUpVB[i], sizeof(VT_TLVertex));
							g_Device.SetFVF(D3DFVF_TLVERTEX);
							g_pDirect3DDevice->DrawPrimitive( D3DPT_TRIANGLESTRIP, 0, 2);
						}
						else
						{							
							if(psKeepUpMugong->m_CurTime + 600 < g_dwCurTime)
							{
								psKeepUpMugong->m_CurTime = g_dwCurTime;
							}							
						}
					}
				}
				++i;
			}
		}		
	}


	for(i=0; iter2 != g_MainCharInfo.m_vkeepUpPetMugongIconList.end(); ++iter2)
	{
		if(i > 1) //펫무공 지속 아이콘은 2개를 넘길 수 없다.
			break;

		g_pDirect3DDevice->SetRenderState( D3DRS_ZENABLE, FALSE);
		g_pDirect3DDevice->SetRenderState( D3DRS_FOGENABLE, FALSE);

		g_pDirect3DDevice->SetRenderState( D3DRS_ALPHABLENDENABLE, TRUE);
		g_pDirect3DDevice->SetRenderState( D3DRS_DIFFUSEMATERIALSOURCE, D3DMCS_COLOR1);
		g_pDirect3DDevice->SetRenderState( D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
		g_pDirect3DDevice->SetRenderState( D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
		
		g_pDirect3DDevice->SetSamplerState( 0,  D3DSAMP_ADDRESSU , D3DTADDRESS_CLAMP);
		g_pDirect3DDevice->SetSamplerState( 0,  D3DSAMP_ADDRESSV , D3DTADDRESS_CLAMP);

		sKEEPUPMUGONGICONLIST *psKeepUpMugong = (*iter2);

		//일반 무공과 각성 무공은 다른 리스트를 쓰므로... 
		//각성 무공
		if(psKeepUpMugong->m_MugongID == YUN_SUSINKIKANG)
		{
			sArrayData* pMugongList = XiahArrayIndex::g_RebirthMugong_List.GetData(psKeepUpMugong->m_MugongID, psKeepUpMugong->m_MugongLevel);
			
			if(!pMugongList)
				continue;
			
			int TotalKeepUpTime = pMugongList->GetInt(27);
			nResID = pMugongList->GetInt(2);
			DWORD TempTime = g_dwCurTime;

			//if(pMugongList && psKeepUpMugong->m_MugongID == YUN_SUSINKIKANG) //수신기강은 펫전용무공 아이콘이므로.
			//{
			//	if( (5000 <= ((TotalKeepUpTime * 1000) - (TempTime - psKeepUpMugong->m_CurTime))) && psKeepUpMugong->m_DrawIcon )
			//	{
			//		g_Device.SetTexture(0, XiahPak::GetTexture(nResID));
			//		g_Device.SetStreamSource( m_pKeepUpVB[13 + i], sizeof(VT_TLVertex));
			//		g_Device.SetFVF(D3DFVF_TLVERTEX);
			//		g_pDirect3DDevice->DrawPrimitive( D3DPT_TRIANGLESTRIP, 0, 2);	
			//	}
			//	else
			//	{
			//		psKeepUpMugong->m_DrawIcon = false;
			//		if(psKeepUpMugong->m_CurTime + 500 > g_dwCurTime)
			//		{
			//			g_Device.SetTexture(0, XiahPak::GetTexture(nResID));
			//			g_Device.SetStreamSource( m_pKeepUpVB[13 + i], sizeof(VT_TLVertex));
			//			g_Device.SetFVF(D3DFVF_TLVERTEX);
			//			g_pDirect3DDevice->DrawPrimitive( D3DPT_TRIANGLESTRIP, 0, 2);
			//		}
			//		else
			//		{
			//			psKeepUpMugong->m_CurTime = g_dwCurTime;
			//		}
			//	}
			//	++i;
			//}
			if(pMugongList && psKeepUpMugong->m_MugongID == YUN_SUSINKIKANG) //HO_0529_07 무공사용이미지 동작 변경 : 0.3초동안 보여지고 안보여짐
			{
				if( (10000 <= ((TotalKeepUpTime * 1000) - (TempTime - psKeepUpMugong->m_CurTime))) && psKeepUpMugong->m_DrawIcon )
				{
					g_Device.SetTexture(0, XiahPak::GetTexture(nResID));
					g_Device.SetStreamSource( m_pKeepUpVB[13 + i], sizeof(VT_TLVertex));
					g_Device.SetFVF(D3DFVF_TLVERTEX);
					g_pDirect3DDevice->DrawPrimitive( D3DPT_TRIANGLESTRIP, 0, 2);
				}
				else
				{
					psKeepUpMugong->m_DrawIcon = false;
						if( (((TotalKeepUpTime * 1000)-300) <= ((TotalKeepUpTime * 1000) - (TempTime - psKeepUpMugong->m_CurTime))) )
						{
							g_Device.SetTexture(0, XiahPak::GetTexture(nResID));
							g_Device.SetStreamSource( m_pKeepUpVB[13 + i], sizeof(VT_TLVertex));
							g_Device.SetFVF(D3DFVF_TLVERTEX);
							g_pDirect3DDevice->DrawPrimitive( D3DPT_TRIANGLESTRIP, 0, 2);
						}
						else
						{							
							if(psKeepUpMugong->m_CurTime + 600 < g_dwCurTime)
							{
								psKeepUpMugong->m_CurTime = g_dwCurTime;
							}							
						}					
				}
				++i;
			}
		}
		//일반
		else 
		{
			sArrayData* pMugongList = XiahArrayIndex::g_MugongList.GetData(psKeepUpMugong->m_MugongID, psKeepUpMugong->m_MugongLevel);
			
			if(!pMugongList)
				continue;
			
			int TotalKeepUpTime = pMugongList->GetInt(31);


			sArrayData* pMugongData = XiahArrayIndex::g_MugongTemplate.GetData(psKeepUpMugong->m_MugongID);
			
			if(!pMugongData)
				continue;
			
			nResID = pMugongData->GetInt(1);
			
			DWORD TempTime = g_dwCurTime;

			//if(pMugongList) //HO_0529_07 무공사용 이미지 동작 변경 : 적용전 코드
			//{
			//	if( (5000 <= ((TotalKeepUpTime * 1000) - (TempTime - psKeepUpMugong->m_CurTime))) && psKeepUpMugong->m_DrawIcon )
			//	{
			//		g_Device.SetTexture(0, XiahPak::GetTexture(nResID));
			//		g_Device.SetStreamSource( m_pKeepUpVB[13 + i], sizeof(VT_TLVertex));
			//		g_Device.SetFVF(D3DFVF_TLVERTEX);
			//		g_pDirect3DDevice->DrawPrimitive( D3DPT_TRIANGLESTRIP, 0, 2);
			//	}
			//	else
			//	{
			//		psKeepUpMugong->m_DrawIcon = false;
			//		if(psKeepUpMugong->m_CurTime + 500 > g_dwCurTime)
			//		{
			//			g_Device.SetTexture(0, XiahPak::GetTexture(nResID));
			//			g_Device.SetStreamSource( m_pKeepUpVB[13 + i], sizeof(VT_TLVertex));
			//			g_Device.SetFVF(D3DFVF_TLVERTEX);
			//			g_pDirect3DDevice->DrawPrimitive( D3DPT_TRIANGLESTRIP, 0, 2);
			//		}
			//		else
			//		{
			//			psKeepUpMugong->m_CurTime = g_dwCurTime;
			//		}
			//	}
			//	++i;
			//}
			if(pMugongList) //HO_0529_07 무공사용이미지 동작 변경 : 0.3초동안 보여지고 안보여짐
			{
				if( (10000 <= ((TotalKeepUpTime * 1000) - (TempTime - psKeepUpMugong->m_CurTime))) && psKeepUpMugong->m_DrawIcon )
				{
					g_Device.SetTexture(0, XiahPak::GetTexture(nResID));
					g_Device.SetStreamSource( m_pKeepUpVB[13 + i], sizeof(VT_TLVertex));
					g_Device.SetFVF(D3DFVF_TLVERTEX);
					g_pDirect3DDevice->DrawPrimitive( D3DPT_TRIANGLESTRIP, 0, 2);
				}
				else
				{
					psKeepUpMugong->m_DrawIcon = false;
						if( (((TotalKeepUpTime * 1000)-300) <= ((TotalKeepUpTime * 1000) - (TempTime - psKeepUpMugong->m_CurTime))) )
						{
							g_Device.SetTexture(0, XiahPak::GetTexture(nResID));
							g_Device.SetStreamSource( m_pKeepUpVB[13 + i], sizeof(VT_TLVertex));
							g_Device.SetFVF(D3DFVF_TLVERTEX);
							g_pDirect3DDevice->DrawPrimitive( D3DPT_TRIANGLESTRIP, 0, 2);
						}
						else
						{							
							if(psKeepUpMugong->m_CurTime + 600 < g_dwCurTime)
							{
								psKeepUpMugong->m_CurTime = g_dwCurTime;
							}							
						}					
				}
				++i;
			}
		}		
	}
}
