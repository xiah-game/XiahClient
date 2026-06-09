
#include "precompile.h"
#include "slot.h"
#include "XiahGame_Main.h"
#include "XiahArrayIndex.h"
#include "XiahGame_Handler_Sender.h"
#include "InterfaceDefine.h"
#include "XiahGame_Pet.h"

#include "SkillTime.h"


CSlot::CSlot() : m_pVB(NULL)
{
	Clear();

	g_pDirect3DDevice->CreateVertexBuffer( 4 * sizeof(VT_TLVertex),
											D3DUSAGE_WRITEONLY, D3DFVF_TLVERTEX,
											D3DPOOL_MANAGED, &m_pVB, NULL);
}

CSlot::~CSlot()
{
	if(m_pVB)
	{
		m_pVB->Release();
		m_pVB = NULL;
	}
}

void CSlot::Clear()
{	
	m_byCurrentSlotGroup = m_byCurrentSlotIndex = 0;
	
	ZeroMemory(&m_dwSlot, sizeof(DWORD) * 10);

	for(int i=0; i < 5; ++i)
	{
		g_pUIManager->SetData(MAIN_FRAME, main_frame_socket_dummy_02 + i, TYPE, STATICDUMMY);
		g_pUIManager->SetData(MAIN_FRAME, main_frame_socket_dummy_02 + i, TEXTURE, 0);
	}

	g_pUIManager->SetData(MAIN_FRAME, main_frame_socket_dummy_01, TYPE, STATICDUMMY);
	g_pUIManager->SetData(MAIN_FRAME, main_frame_socket_dummy_01, TEXTURE, 0);
}

void CSlot::UpDateRender()
{
	SkillTime::SkillList& list = g_SkillTime.GetSkillList();

	SkillTime::SkillList::iterator iter = list.begin();
	for(; iter != list.end(); ++iter)
	{
		DWORD dwID = iter->first;

		int nIndex = m_byCurrentSlotGroup * 5;
		for(int i=0; i < 5; ++i)
		{
			int n = nIndex + i;

			if(m_dwSlot[n] == dwID)
			{
				SkillTime::sSkill skill = iter->second;

				RECT rtTemp;
				g_pUIManager->GetRegionData(MAIN_FRAME, main_frame_socket_dummy_02 + i, rtTemp);

				int nGap = rtTemp.bottom - rtTemp.top;
				rtTemp.bottom = rtTemp.bottom - (nGap * skill.fRatio);

				SkillTimeRender(rtTemp);
			}
		}

		if(GetActiveSlot() == dwID)
		{
			SkillTime::sSkill skill = iter->second;

			RECT rtTemp;
			g_pUIManager->GetRegionData(MAIN_FRAME, main_frame_socket_dummy_01, rtTemp);

			int nGap = rtTemp.bottom - rtTemp.top;
			rtTemp.bottom = rtTemp.bottom - (nGap * skill.fRatio);

			SkillTimeRender(rtTemp);
		}		
	}	
}

void CSlot::SkillTimeRender(RECT& rtTemp)
{
	VT_TLVertex	Vertex[4];
	Vertex[ 0].pos = Vector4(rtTemp.left,  rtTemp.top,    0, 1);
	Vertex[ 1].pos = Vector4(rtTemp.right, rtTemp.top,    0, 1);
	Vertex[ 2].pos = Vector4(rtTemp.left,  rtTemp.bottom, 0, 1);
	Vertex[ 3].pos = Vector4(rtTemp.right, rtTemp.bottom, 0, 1);

	Vertex[ 0].diffuse = D3DCOLOR_ARGB( 210, 50, 50, 50);
	Vertex[ 1].diffuse = D3DCOLOR_ARGB( 210, 50, 50, 50);
	Vertex[ 2].diffuse = D3DCOLOR_ARGB( 210, 50, 50, 50);
	Vertex[ 3].diffuse = D3DCOLOR_ARGB( 210, 50, 50, 50);

	Vertex[ 0].tex = Vector2(0, 0);
	Vertex[ 1].tex = Vector2(1, 0);
	Vertex[ 2].tex = Vector2(0, 1);
	Vertex[ 3].tex = Vector2(1, 1);

	VOID* pVertices;
	if(!FAILED( m_pVB->Lock( 0, sizeof(Vertex), (void**)&pVertices, 0 )))
	{
		memcpy( pVertices, Vertex, sizeof(Vertex) );
		m_pVB->Unlock();
	}

	g_pDirect3DDevice->SetRenderState( D3DRS_ZENABLE, FALSE);

	g_pDirect3DDevice->SetRenderState( D3DRS_ALPHABLENDENABLE, TRUE);
	g_pDirect3DDevice->SetRenderState( D3DRS_DIFFUSEMATERIALSOURCE, D3DMCS_COLOR1);
	g_pDirect3DDevice->SetRenderState( D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	g_pDirect3DDevice->SetRenderState( D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

	g_pDirect3DDevice->SetSamplerState( 0,  D3DSAMP_ADDRESSU , D3DTADDRESS_CLAMP);
	g_pDirect3DDevice->SetSamplerState( 0,  D3DSAMP_ADDRESSV , D3DTADDRESS_CLAMP);

	g_Device.SetTexture(0, NULL);
	g_Device.SetStreamSource( m_pVB, sizeof(VT_TLVertex));
	g_Device.SetFVF(D3DFVF_TLVERTEX);
	g_pDirect3DDevice->DrawPrimitive( D3DPT_TRIANGLESTRIP, 0, 2);
}

void CSlot::ChangeSlot(BYTE bySlot)
{
	g_MainCharInfo.PlayInterfaceSound(ISOUND_SELECT_BUTTON);

	if(bySlot == 255)
	{
		if(m_byCurrentSlotGroup == 0)
			m_byCurrentSlotGroup = 1;
		else
			m_byCurrentSlotGroup = 0;
	}
	else
	{
		m_byCurrentSlotGroup = bySlot;
	}

	for(BYTE byIndex=0; byIndex < 10; ++byIndex)
	{
		SetSlot(byIndex, m_dwSlot[byIndex]);
	}
}

void CSlot::SetSlot( BYTE bySlotID, DWORD dwID)
{
	int nResID =0;

	if( dwID)
	{
		if(dwID >= 1 && dwID <= 131)	//if(dwID < POTION_VISUALID_1) // 무공
		{
			sArrayData *pData = XiahArrayIndex::g_MugongTemplate.GetData( dwID);

			if(!pData)
			{
				DBG_LogFile( "SetSlot fail %d %d %x",bySlotID,dwID,&pData);
				return;
			}
			
			nResID = pData->GetInt( 1);			
		}

		//HT_0711 : 진각성 무공
		if(dwID >= 191 && dwID <= 198)	
		{
			sArrayData *pData = XiahArrayIndex::g_MugongTemplate.GetData( dwID);

			if(!pData)
			{
				DBG_LogFile( "SetSlot fail %d %d %x",bySlotID,dwID,&pData);
				return;
			}
			
			nResID = pData->GetInt( 1);			
		}

		if(dwID >=161 && dwID <=179)
		{
			sArrayData *pData = XiahArrayIndex::g_RebirthMugong_List.GetData(dwID, 1);

			if(!pData)
			{
				DBG_LogFile( "SetSlot fail %d %d %x",bySlotID,dwID,&pData);
				return;
			}

			nResID = pData->GetInt(2);
		}

		for( int i=0; i < 2; ++i)
		{
			XiahItem::sItemInfo *pItemInfo = g_MainCharInfo.m_pMySack[ i]->FindSackItemByVisualID( dwID);

			if( pItemInfo && (pItemInfo->m_bItemType == ITEMTYPE_POTION || pItemInfo->m_bItemType == ITEMTYPE_NPCITEM))
			{
				nResID = pItemInfo->m_nResID;

				break;
			}				
		}
	}

	BYTE bySlotGroup = 0;
	BYTE bySlotIndex = bySlotID;
	if(bySlotID >= 5)
	{
		bySlotGroup = 1;
		bySlotIndex -= 5;
	}

	// 퀵 슬롯 확장
	if(m_byCurrentSlotGroup == bySlotGroup)
		g_pUIManager->SetData(MAIN_FRAME, main_frame_socket_dummy_02 + bySlotIndex, TYPE, STATIC);

	if( nResID)
	{
		if(m_byCurrentSlotGroup == bySlotGroup)
			g_pUIManager->SetData(MAIN_FRAME, main_frame_socket_dummy_02 + bySlotIndex, TEXTURE, nResID);

		m_dwSlot[ bySlotID] = dwID;
	}
	else	// 없을때 지우자
	{
		if(m_byCurrentSlotGroup == bySlotGroup)
			g_pUIManager->SetData(MAIN_FRAME, main_frame_socket_dummy_02 + bySlotIndex, TEXTURE, 0);

		m_dwSlot[ bySlotID] = 0;		
	}

	SetSlotToolTip( bySlotID);
}

void CSlot::SetSlotToolTip( BYTE bySlotIndex)
{
	// 퀵 슬롯 확장
	DWORD dwID = m_dwSlot[bySlotIndex];	
	
	BYTE bySlotGroup	= 0;
	BYTE bySlotIndex2	= bySlotIndex;

	if(bySlotIndex >= 5)
	{
		bySlotGroup = 1;
		bySlotIndex2 -= 5;
	}

	if( dwID == 0)
	{
		if(m_byCurrentSlotGroup == bySlotGroup)
		{
			g_pUIManager->SetToolTip(MAIN_FRAME, main_frame_socket_dummy_02 + bySlotIndex2, 1, _T(""));
			g_pUIManager->SetData(MAIN_FRAME, main_frame_socket_dummy_02 + bySlotIndex2, TYPE, STATICDUMMY);
		}		

		return;
	}

	TCHAR szToolTip[128] = {0,};
	bool bTemp = false;

	//무공	
	if(dwID >= 1 && dwID <= 131)
	{
		sArrayData *pData = XiahArrayIndex::g_MugongTemplate.GetData( dwID);

		if( !pData)
		{
			DBG_LogFile( _T("SetSlotToolTip fail"));
			return;
		}

		_tcscpy( szToolTip, pData->GetString( 1));

		bTemp = true;
	}

	// 각성 무공
	if(dwID >= 161 && dwID <= 179)
	{
		sArrayData *pData = XiahArrayIndex::g_RebirthMugong_List.GetData( dwID);

		if( !pData)
		{
			DBG_LogFile( _T("SetSlotToolTip fail"));
			return;
		}

		_tcscpy( szToolTip, pData->GetString( 1));

		bTemp = true;
	}

	//HT_0711 :진각성 무공
	if(dwID >= 191 && dwID <= 198)
	{
		sArrayData *pData = XiahArrayIndex::g_MugongTemplate.GetData( dwID);

		if( !pData)
		{
			DBG_LogFile( _T("SetSlotToolTip fail"));
			return;
		}

		_tcscpy( szToolTip, pData->GetString( 1));

		bTemp = true;
	}
	//아이템
	else if((dwID >= 20000 && dwID <= 22200) || (dwID == 9301) || (dwID == 9302) || (dwID == 31009))	
	{					
		XiahItem::sItemInfo* pItem = NULL;

		for( int i=0; i < 2; ++i)
		{
			pItem = g_MainCharInfo.m_pMySack[ i]->FindSackItemByVisualID( dwID);
			if( pItem)
			{
				_stprintf( szToolTip, _T("%s %d"), (LPCTSTR)pItem->m_szName, pItem->m_dwAmount);
				break;
			}
		}

		if( !pItem)
		{
			m_dwSlot[ bySlotIndex] = 0;

			if(m_byCurrentSlotGroup == bySlotGroup)
			{
				g_pUIManager->SetToolTip(MAIN_FRAME, main_frame_socket_dummy_02 + bySlotIndex2, 1, _T(""));
				g_pUIManager->SetData(MAIN_FRAME, main_frame_socket_dummy_02 + bySlotIndex2, TYPE, STATICDUMMY);
			}			

			return;
		}

		bTemp = true;
	}

	if(bTemp && m_byCurrentSlotGroup == bySlotGroup)
		g_pUIManager->SetToolTip(MAIN_FRAME, main_frame_socket_dummy_02 + bySlotIndex2, 1, szToolTip, 2);
}

void CSlot::SetActiveSlot( BYTE byIndex, DWORD dwID)
{
	int		nResID =0;

	// 무공 resID
	if(dwID < POTION_VISUALID_1 && (dwID != 9301 || dwID != 9302))
	{
		//HT_0711 : 진각성 무공
		if(dwID < 160 || dwID >190)
		{
			sArrayData *pData = XiahArrayIndex::g_MugongTemplate.GetData( dwID);

			if( !pData)
			{
				DBG_LogFile( _T("SetActiveSlot fail"));
				return;
			}

			// image
			nResID = pData->GetInt( 1);
		}
		else if(dwID >=161 && dwID <=179)
		{
			sArrayData *pData = XiahArrayIndex::g_RebirthMugong_List.GetData(dwID, 1);

			if( !pData)
			{
				DBG_LogFile( _T("SetActiveSlot fail"));
				return;
			}

			// image
			nResID = pData->GetInt( 2);
		}
	}
	else
	{
		if( !g_MainCharInfo.m_pMySack[g_MainCharInfo.m_byMySackCurrIdx]->FindSackItemByVisualID( POTION_VISUALID_1))
			if( !g_MainCharInfo.m_pMySack[g_MainCharInfo.m_byMySackCurrIdx]->FindSackItemByVisualID( POTION_VISUALID_2))
				if( !g_MainCharInfo.m_pMySack[g_MainCharInfo.m_byMySackCurrIdx]->FindSackItemByVisualID( 9301))
					if( !g_MainCharInfo.m_pMySack[g_MainCharInfo.m_byMySackCurrIdx]->FindSackItemByVisualID( 9302))
						return;

		if( !(dwID == POTION_VISUALID_1 || dwID == POTION_VISUALID_2 || dwID == 9301 || dwID == 9302))
			return;

		XiahItem::sItemInfo itemInfo;

		itemInfo.m_wVisualID = dwID;

		XiahItem::SetItemVisualData(&itemInfo);		
		
		nResID = itemInfo.m_nResID;
	}

	g_pUIManager->SetData(MAIN_FRAME, main_frame_socket_dummy_01, TYPE, STATIC);
	g_pUIManager->SetData(MAIN_FRAME, main_frame_socket_dummy_01, TEXTURE, nResID);

	SetCurrentSlotIndex( byIndex);

	g_MainCharInfo.PlayInterfaceSound( ISOUND_SKILL_ACTIVE);
}

/**
 *
 * \return 
 */
BOOL CSlot::CheckSetItemOnSlot()
{
	if( g_MainCharInfo.m_pHoldItem->GetHoldItemItem()->m_bSackID != SACKTYPE__DEFAULT)
		return FALSE;

	for(int i=0; i < 5; ++i)
	{
		if( g_pUIManager->IsMouseOn(MAIN_FRAME, main_frame_socket_dummy_02 + i))
		{
			// 퀵 슬롯 확장
			if(m_byCurrentSlotGroup >= 1)
				i += 5;

			SendCS_IT_SETSLOT_REQ( g_MainCharInfo.m_pHoldItem->GetHoldItemItem()->m_wRefID, i + 1);

			return TRUE;
		}
	}
	
	return FALSE;
}

/**
 *
 */
void CSlot::CheckSlotSelected()
{
	for( int i=0; i < 5; ++i)
	{
		if( g_pUIManager->IsMouseOn(MAIN_FRAME, main_frame_socket_dummy_02 + i))
		{
			// 퀵 슬롯 확장
			if(m_byCurrentSlotGroup >= 1)
				i += 5;

			if( SelectSlot( i))
				return;
		}
	}
}

// 슬롯에 무엇이 들어있는가를 검사
/**
 *
 * \param i 
 * \return 
 */
DWORD CSlot::CheckQuickSlot(int i)
{
	if( m_dwSlot[i] == 0)
		return 0;

	// ITEM
	if( m_dwSlot[i] > 10000)
		return 1;

	// 무공
	if( m_dwSlot[i] != 0 && m_dwSlot[i] < 10000)
		return 2;

	return -1;	// FAIL
}

/**
 *
 * \param bySlotIndex 
 * \return 
 */
BOOL CSlot::SelectSlot( BYTE bySlotIndex)
{
	if( bySlotIndex < 0 || bySlotIndex > 10)
		return FALSE;

	BYTE bType = 1;

	// ITEM 사용
	if( m_dwSlot[ bySlotIndex] > 10000 || ( m_dwSlot[ bySlotIndex] == 9301 ||  m_dwSlot[ bySlotIndex] == 9302))
	{
		//g_MainCharInfo.m_pSlot->SetActiveSlot( bySlotIndex, m_dwSlot[ bySlotIndex]);
		UseItem( bySlotIndex);
		return TRUE;
	}
	else if(m_dwSlot[ bySlotIndex] != 0)
	{
		// 무공사용
		SendCS_BT_SELMUGONG_REQ( bType, m_dwSlot[ bySlotIndex], bySlotIndex+1);
		return TRUE;
	}

	return FALSE;
}

/**
 *
 * \param bySlotIndex 
 */
void CSlot::UseItem( BYTE bySlotIndex)
{
	switch( m_dwSlot[ bySlotIndex])
	{
	case POTION_VISUALID_1:
	case 20100:
		{
			if(g_MainCharInfo.m_dwHpMax == g_MainCharInfo.m_dwHpCur)
				return;
		}		
		break;

	case POTION_VISUALID_2:
	case 21100:
		{
			if(g_MainCharInfo.m_wIpMax == g_MainCharInfo.m_wIpCur)
				return;
		}		
		break;

	case 9301:
	case 9302:
		{
			sPetInfo* pPetInfo = g_PetList.GetCurrentPet();

			if(pPetInfo)
			{
				if( pPetInfo->dwHpCur != pPetInfo->dwHpMax)
				{
					if(g_PetList.Find( pPetInfo->dwID))
					{
						XiahObject::CXiahObject* pObject = XiahObject::g_XiahObjectManager.FindXiahObject( MAKEOBJECTID( 0, pPetInfo->dwID, OBJTYPE_PET));
						CXiahCharObject* pCharObject = reinterpret_cast<CXiahCharObject*>( pObject->m_pObject);

						XiahItem::sItemInfo* pItem = g_MainCharInfo.m_pMySack[0]->FindSackItemByVisualID( m_dwSlot[ bySlotIndex]);

						if(pItem)
						{
							SendCS_IM_GIVEITEM_REQ( pItem->m_bSackCount+1, pItem->m_bSackPos, pItem->m_dwItemID, pCharObject->m_bObjType, pObject->m_dwServerID);
							g_MainCharInfo.ShowHelpMessage( IDS_FEED);
						}
						else
						{
							pItem = g_MainCharInfo.m_pMySack[1]->FindSackItemByVisualID( m_dwSlot[ bySlotIndex]);

							if(pItem)
							{
								SendCS_IM_GIVEITEM_REQ( pItem->m_bSackCount+1, pItem->m_bSackPos, pItem->m_dwItemID, pCharObject->m_bObjType, pObject->m_dwServerID);
								g_MainCharInfo.ShowHelpMessage( IDS_FEED);
							}
							else
							{
								SetSlot( bySlotIndex, 0);
							}
						}

					}
				}
				else
				{
					g_MainCharInfo.ShowHelpMessage( IDS_PET_HP_NOT);
				}
			}

			SetSlotToolTip( bySlotIndex);

			return;
		}
		break;
	}

	static int last_use_hp = 0;
	static int last_use_mp = 0;
	long cur_time = timeGetTime();

	if(last_use_hp + 500 < cur_time)
	{
		last_use_hp = cur_time;

		XiahItem::sItemInfo* pItem = g_MainCharInfo.m_pMySack[0]->FindSackItemByVisualID( m_dwSlot[ bySlotIndex]);
		if( pItem)
		{
			SendCS_IM_USEITEM_REQ( pItem->m_bSackCount+1, pItem->m_bSackPos, pItem->m_dwItemID);
		}
		else
		{
			XiahItem::sItemInfo* pItem = g_MainCharInfo.m_pMySack[1]->FindSackItemByVisualID( m_dwSlot[ bySlotIndex]);
			if( pItem)
				SendCS_IM_USEITEM_REQ( pItem->m_bSackCount+1, pItem->m_bSackPos, pItem->m_dwItemID);
			else
				SetSlot( bySlotIndex, 0);
		}
	}
	
	SetSlotToolTip( bySlotIndex);
}
