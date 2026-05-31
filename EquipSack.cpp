#include "precompile.h"
#include "equipsack.h"
#include "CharacterInfo.h"
#include "XiahGame_Main.h"
#include "InterfaceDefine.h"
#include "XiahGameObject.h"
#include <algorithm>
#include <assert.h>

//HT_CHEAT : 자동 수리를 위해 헤더 추가
#include "XiahGame_Handler_Sender.h"

typedef vector< BYTE> VSHORTENDU;
VSHORTENDU g_ShortEndu;

extern BOOL SetupPC_VisualEquipement(CXiahCharObject* pObject, WORD* pVisualList, BYTE* pRarityList=NULL, BYTE* pStxTypeList=NULL);

CEquipSack::CEquipSack( BYTE byType, BYTE byTotalSize) : m_dwTime(0), m_bRepairShow(false)
{
	m_bySackType = byType;
	m_bySackTotalSize = byTotalSize;

	for( int i=0; i < 9; ++i)
		m_pEquipVB[i] = NULL;

	for( int i=0; i < 9; ++i)
		m_pEquipTex[i] = NULL;

	if( byType == SACKTYPE__EQUIPMENT)
	{
		CreateEquipVB();
	}

	for( int i=0; i < m_bySackTotalSize; ++i)
	{
		m_vecItem.push_back( NULL);
		m_vecItemVB.push_back( NULL);
		m_vecItemTex.push_back( NULL);
		m_vecItemRt.push_back( NULL);

		// 제련
		m_vecItemSocketVB.push_back(NULL);

		m_vecSocketItem1VB.push_back(NULL);
		m_vecSocketItem2VB.push_back(NULL);
		m_vecSocketItem3VB.push_back(NULL);
		m_vecRBSocketItemVB.push_back(NULL);//HT_1116 : 각성자 아이템 추가
		m_vecRBItemStoneVB.push_back(NULL);
	}

	SetSackRegion();
}

CEquipSack::~CEquipSack(void)
{
	for(int i=0; i < 9; ++i)
	{
		// VERTEX BUFFER
		if( m_pEquipVB[i] )
		{
			m_pEquipVB[i]->Release();
			m_pEquipVB[i] = NULL;
		}

		// TEXTURE는 XIAH PAK에 맞긴다.
	}
}


/**
 *
 */
void CEquipSack::SetSackRegion()
{
	switch(m_bySackType)
	{
	case SACKTYPE__EQUIPMENT:
		{
			if(m_vecItemRt[ EQUIPPOS_NECLACE])
				return;

			RECT rtTemp;

			g_pUIManager->GetRegionData(DRG_ITEM_WINDOW, drg_item_window_wear_1_1_dummy, rtTemp);
			m_vecItemRt[ EQUIPPOS_NECLACE] = new sRect( rtTemp);
			g_pUIManager->GetRegionData(DRG_ITEM_WINDOW, drg_item_window_wear_1_1_dummy_01, rtTemp);
			m_vecItemRt[ EQUIPPOS_HAT] = new sRect( rtTemp);	
			g_pUIManager->GetRegionData(DRG_ITEM_WINDOW, drg_item_window_wear_1_1_dummy_02, rtTemp);
			m_vecItemRt[ EQUIPPOS_RING] = new sRect( rtTemp);

			//--------------------------------------------------------------------

			g_pUIManager->GetRegionData(DRG_ITEM_WINDOW, drg_item_window_wear_2_2_dummy, rtTemp);
			m_vecItemRt[ EQUIPPOS_WEAPON] = new sRect( rtTemp);
			g_pUIManager->GetRegionData(DRG_ITEM_WINDOW, drg_item_window_wear_2_2_dummy_01, rtTemp);
			m_vecItemRt[ EQUIPPOS_CLOTH] = new sRect( rtTemp);
			g_pUIManager->GetRegionData(DRG_ITEM_WINDOW, drg_item_window_wear_2_2_dummy_02, rtTemp);
			m_vecItemRt[ EQUIPPOS_CLOAK] = new sRect( rtTemp);

			//--------------------------------------------------------------------

			g_pUIManager->GetRegionData(DRG_ITEM_WINDOW, drg_item_window_wear_1_1_dummy_03, rtTemp);
			m_vecItemRt[ EQUIPPOS_PROTECTOR] = new sRect( rtTemp);
			g_pUIManager->GetRegionData(DRG_ITEM_WINDOW, drg_item_window_wear_1_1_dummy_04, rtTemp);
			m_vecItemRt[ EQUIPPOS_SHOE] = new sRect( rtTemp);
			g_pUIManager->GetRegionData(DRG_ITEM_WINDOW, drg_item_window_wear_1_1_dummy_05, rtTemp);
			m_vecItemRt[ EQUIPPOS_BONGIN] = new sRect( rtTemp);

			m_SackRt.left	= m_vecItemRt[ EQUIPPOS_WEAPON]->left;
			m_SackRt.top	= m_vecItemRt[ EQUIPPOS_HAT]->top;
			m_SackRt.right	= m_vecItemRt[ EQUIPPOS_CLOAK]->right;
			m_SackRt.bottom = m_vecItemRt[ EQUIPPOS_SHOE]->bottom;
		}
		break;
	case SACKTYPE__MODIFY:
		{
			RECT rtTemp;

			g_pUIManager->GetRegionData(WINDOW_CONVERT, convert_window_socket_dummy_01, rtTemp);
			m_vecItemRt[ 0] = new sRect( rtTemp);
			g_pUIManager->GetRegionData(WINDOW_CONVERT, convert_window_socket_dummy_02, rtTemp);
			m_vecItemRt[ 1] = new sRect( rtTemp);
			g_pUIManager->GetRegionData(WINDOW_CONVERT, convert_window_socket_dummy_03, rtTemp);
			m_vecItemRt[ 2] = new sRect( rtTemp);
			g_pUIManager->GetRegionData(WINDOW_CONVERT, convert_window_socket_dummy_04, rtTemp);
			m_vecItemRt[ 3] = new sRect( rtTemp);

			m_SackRt.left	= m_vecItemRt[ 0]->left;
			m_SackRt.top	= m_vecItemRt[ 1]->top;
			m_SackRt.right	= m_vecItemRt[ 1]->right;
			m_SackRt.bottom = m_vecItemRt[ 3]->bottom;
		}
		break;
	case SACKTYPE__PET_EQUIP:	// 펫 장착
		{
			RECT rtTemp;

			g_pUIManager->GetRegionData(WINDOW_NEW_TAMING, taming_window_arms_dummy, rtTemp);
			m_vecItemRt[ PETEQUIP_WEAPON] = new sRect( rtTemp);

			g_pUIManager->GetRegionData(WINDOW_NEW_TAMING, taming_window_arms_dummy_01, rtTemp);
			m_vecItemRt[ PETEQUIP_ARMOR] = new sRect( rtTemp);		

			g_pUIManager->GetRegionData(WINDOW_NEW_TAMING, taming_window_accessory_dummy_03, rtTemp);
			m_vecItemRt[ PETEQUIP_RIDING] = new sRect( rtTemp);

			g_pUIManager->GetRegionData(WINDOW_NEW_TAMING, taming_window_accessory_dummy_04, rtTemp);
			m_vecItemRt[ PETEQUIP_BAG] = new sRect( rtTemp);

			g_pUIManager->GetRegionData(WINDOW_NEW_TAMING, taming_window_accessory_dummy_01, rtTemp);
			m_vecItemRt[ PETEQUIP_ACCESSORY1] = new sRect( rtTemp);

			g_pUIManager->GetRegionData(WINDOW_NEW_TAMING, taming_window_accessory_dummy_02, rtTemp);
			m_vecItemRt[ PETEQUIP_ACCESSORY2] = new sRect( rtTemp);

			m_SackRt.left	= m_vecItemRt[ PETEQUIP_WEAPON]->left;
			m_SackRt.top	= m_vecItemRt[ PETEQUIP_ACCESSORY2]->top;
			m_SackRt.right	= m_vecItemRt[ PETEQUIP_ACCESSORY2]->right;
			m_SackRt.bottom = m_vecItemRt[ PETEQUIP_BAG]->bottom;
		}
		break;

	case SACKTYPE__FIVEELEMENT_CONVERT:	// 오행 아이템 제련
		{
			RECT rtTemp;

			g_pUIManager->GetRegionData(WINDOW_FIVEELEMENTS_CONVERT, fiveelements_convert_window_dummy_01, rtTemp);
			m_vecItemRt[ 0] = new sRect(rtTemp);
			g_pUIManager->GetRegionData(WINDOW_FIVEELEMENTS_CONVERT, fiveelements_convert_window_dummy_02, rtTemp);
			m_vecItemRt[ 1] = new sRect(rtTemp);
			g_pUIManager->GetRegionData(WINDOW_FIVEELEMENTS_CONVERT, fiveelements_convert_window_dummy_03, rtTemp);
			m_vecItemRt[ 2] = new sRect(rtTemp);
			g_pUIManager->GetRegionData(WINDOW_FIVEELEMENTS_CONVERT, fiveelements_convert_window_dummy_04, rtTemp);
			m_vecItemRt[ 3] = new sRect(rtTemp);

			m_SackRt.left	= m_vecItemRt[ 0]->left;
			m_SackRt.top	= m_vecItemRt[ 1]->top;
			m_SackRt.right	= m_vecItemRt[ 1]->right;
			m_SackRt.bottom = m_vecItemRt[ 3]->bottom;
		}
		break;
	case SACKTYPE__COLLECTION:		// 아이템 수집
		{
			RECT rtTemp;

			for(int i=0; i < 10; ++i)
			{
				g_pUIManager->GetRegionData(WINDOW_COLLECTION, collection_window_dummy_01 + i, rtTemp);
				m_vecItemRt[i] = new sRect(rtTemp);
			}

			m_SackRt.left	= m_vecItemRt[0]->left;
			m_SackRt.top	= m_vecItemRt[0]->top;
			m_SackRt.right	= m_vecItemRt[2]->right;
			m_SackRt.bottom = m_vecItemRt[9]->bottom;
		}
		break;
	default:
		break;
	} // switch(m_bySackType)
}

/**
 *
 */
void CEquipSack::CreateEquipVB()
{
	sRect rtRect;
	
	for( int i=0; i < 9; ++i)
	{
		if(!m_pEquipVB[i])
			g_pDirect3DDevice->CreateVertexBuffer( 4*sizeof(VT_TLVertex), 0, D3DFVF_TLVERTEX,
													D3DPOOL_MANAGED, &m_pEquipVB[i], NULL);

		if( i < 6)
		{
			rtRect.left = 30*i;
			rtRect.top = 730;
			rtRect.right = rtRect.left + 30;
			rtRect.bottom = rtRect.top + 30;
		}
		else
		{
			rtRect.left = 30*i;
			rtRect.top = 730 - 30;
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
		if( !FAILED( m_pEquipVB[i]->Lock( 0, sizeof(Vertex), (void**)&pVertices, 0 )))
		{
			memcpy( pVertices, Vertex, sizeof(Vertex) );
			m_pEquipVB[i]->Unlock();
		}
	}


	m_pEquipTex[EQUIPPOS_WEAPON]	= XiahPak::GetTexture(887);//무기887
	m_pEquipTex[EQUIPPOS_HAT]		= XiahPak::GetTexture(885);//모자885
	m_pEquipTex[EQUIPPOS_CLOTH]		= XiahPak::GetTexture(890);//의복890
	m_pEquipTex[EQUIPPOS_SHOE]		= XiahPak::GetTexture(889);//신발889
	m_pEquipTex[4]					= NULL;
	m_pEquipTex[EQUIPPOS_RING]		= XiahPak::GetTexture(888);//반지888
	m_pEquipTex[EQUIPPOS_NECLACE]	= XiahPak::GetTexture(886);//목걸이886
	m_pEquipTex[7]					= NULL;	
	m_pEquipTex[EQUIPPOS_BONGIN]	= XiahPak::GetTexture(1071);//항아리 1071
}

/**
 *
 * \return 
 */
BOOL CEquipSack::CheckItemUnSelected()
{
	if( !m_bShow)
		return FALSE;

	if( CSack::CheckItemUnSelected())
	{
		for( int i=0; i < m_bySackTotalSize; ++i)
		{
			if( m_vecItemRt[i] && m_vecItemRt[i]->PtInRect( XiahInput::g_ptMouse))
			{
				return ProcessItemUnSelected( i);
			}				
		}
	}

	return FALSE;
}

/**
 *
 * \param bSackPos 
 * \param pItem 
 * \return 
 */
BOOL CEquipSack::InsertItem( BYTE bSackPos, XiahItem::sItemInfo* pItem)
{
	if( bSackPos >= m_bySackTotalSize) return FALSE;
	// 장착
	if( g_pMainChar && m_bySackType == SACKTYPE__EQUIPMENT)
	{
		CXiahCharObject* pCharObject = (CXiahCharObject*)g_pMainChar->m_pObject;
		if(pCharObject == NULL)
			return FALSE;

		g_MainCharInfo.wEquipVisualID[ bSackPos] = pItem->m_wVisualID;
		g_MainCharInfo.m_bRarity[ bSackPos ]  = pItem->m_bRarity;
		g_MainCharInfo.m_bStxType[ bSackPos ] = pItem->m_bLimitCnt; // 发光特效由强化等级驱动

		SetupPC_VisualEquipement( pCharObject, g_MainCharInfo.wEquipVisualID, g_MainCharInfo.m_bRarity, g_MainCharInfo.m_bStxType );
	}

	return CSack::InsertItem( bSackPos, pItem);
}

/**
 *
 * \param bSackPos 
 */
void CEquipSack::DeleteItem( BYTE bSackPos, bool bDelete)
{
	if( bSackPos >= m_bySackTotalSize) return;
	// 탈착
	if( g_pMainChar && m_bySackType == SACKTYPE__EQUIPMENT)
	{	
		CXiahCharObject* pCharObject = (CXiahCharObject*)g_pMainChar->m_pObject;
		if(!pCharObject)
			return;

		g_MainCharInfo.wEquipVisualID[ bSackPos] = 0;
		g_MainCharInfo.m_bRarity[ bSackPos ] = 0;
		g_MainCharInfo.m_bStxType[ bSackPos ] = 0;

		SetupPC_VisualEquipement( pCharObject, g_MainCharInfo.wEquipVisualID, g_MainCharInfo.m_bRarity, g_MainCharInfo.m_bStxType );
	}

	m_vecItem[ bSackPos] = NULL;

	if( m_vecItemVB[ bSackPos])
	{
		m_vecItemVB[ bSackPos]->Release();
		m_vecItemVB[ bSackPos] = NULL;
	}

	// 제련
	if(m_vecItemSocketVB[bSackPos])
	{
		m_vecItemSocketVB[bSackPos]->Release();
		m_vecItemSocketVB[bSackPos] = NULL;
	}

	if(m_vecSocketItem1VB[bSackPos])
	{
		m_vecSocketItem1VB[bSackPos]->Release();
		m_vecSocketItem1VB[bSackPos] = NULL;
	}

	if(m_vecSocketItem2VB[bSackPos])
	{
		m_vecSocketItem2VB[bSackPos]->Release();
		m_vecSocketItem2VB[bSackPos] = NULL;
	}

	if(m_vecSocketItem3VB[bSackPos])
	{
		m_vecSocketItem3VB[bSackPos]->Release();
		m_vecSocketItem3VB[bSackPos] = NULL;
	}
	//HT_1116 : 각성자 아이템 추가
	if(m_vecRBSocketItemVB[bSackPos])
	{
		m_vecRBSocketItemVB[bSackPos]->Release();
		m_vecRBSocketItemVB[bSackPos] = NULL;
	}
	if(m_vecRBItemStoneVB[bSackPos])
	{
		m_vecRBItemStoneVB[bSackPos]->Release();
		m_vecRBItemStoneVB[bSackPos] = NULL;
	}
}

/**
 *
 * \param nPosition 
 * \param pItem 
 */
/*
void CEquipSack::SetVB( BYTE nPosition, XiahItem::sItemInfo* pItem)
{
	CSack::SetVB( nPosition, pItem);
}
*/


/**
 *
 */
void CEquipSack::DrawEquipShortEndu()
{
	g_pDirect3DDevice->SetRenderState( D3DRS_ZENABLE, FALSE);
	g_pDirect3DDevice->SetRenderState( D3DRS_FOGENABLE, FALSE);

	g_pDirect3DDevice->SetRenderState( D3DRS_ALPHABLENDENABLE, TRUE);
	g_pDirect3DDevice->SetRenderState( D3DRS_DIFFUSEMATERIALSOURCE, D3DMCS_COLOR1);
	g_pDirect3DDevice->SetRenderState( D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	g_pDirect3DDevice->SetRenderState( D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
	
	g_pDirect3DDevice->SetSamplerState( 0,  D3DSAMP_ADDRESSU , D3DTADDRESS_CLAMP);
	g_pDirect3DDevice->SetSamplerState( 0,  D3DSAMP_ADDRESSV , D3DTADDRESS_CLAMP);

	if(m_bRepairShow)
	{
		int nSize = g_ShortEndu.size();

		for( int i = 0; i < nSize; ++i)
		{
			g_Device.SetTexture(0, m_pEquipTex[g_ShortEndu[i]]);
			//g_pDirect3DDevice->SetTexture( 0, m_pEquipTex[ g_ShortEndu[i]]);
			g_Device.SetStreamSource( m_pEquipVB[i], sizeof(VT_TLVertex));
			g_Device.SetFVF(D3DFVF_TLVERTEX);
			//g_pDirect3DDevice->SetFVF( D3DFVF_TLVERTEX);
			g_pDirect3DDevice->DrawPrimitive( D3DPT_TRIANGLESTRIP, 0, 2);
		}
	}
}

/**
 *
 */
void CEquipSack::CheckEquipShortEndu()
{
	bool bRepairShow = false;		

	if(m_bySackType == SACKTYPE__EQUIPMENT)
	{
		WORD wTemp = 0;		

		for(register int i=0; i < m_bySackTotalSize; ++i)
		{
			if( m_vecItem[i])
			{
				// 이벤트 아이템은 통과
				if(m_vecItem[i]->m_bItemType == ITEMTYPE_SOCKET)
					continue;

				WORD wCurDur = m_vecItem[i]->m_wCurDur;
				WORD wMaxDur = m_vecItem[i]->m_wMaxDur;

				assert(wMaxDur);

				if(wMaxDur)
					wTemp = wCurDur * 100 / wMaxDur;

				if( wTemp <= 20)
				{
					if(m_vecItem[i]->m_wPrev > wTemp)
						m_vecItem[i]->m_bShowMessage = true;

					switch(wTemp)
					{
					case 20:
					case 10:
					case 5:
						{
							if(m_vecItem[i]->m_bShowMessage)
							{
								// 망치는 문구 제외
								if(!(ITEMTYPE_WEAPON == m_vecItem[i]->m_bItemType && 8 == m_vecItem[i]->m_bItemKind))
									g_MainCharInfo.ShowHelpMessage(INTER_WARNNIG3 ,TEXTEFFECT_COLOR_WARNING);

								
								//HT_CHEAT : 자동 수리
								XiahItem::sItemInfo* pItem = m_vecItem[i];
								WORD wDur = pItem->m_wCurDur * 100 / pItem->m_wMaxDur;
								SendCS_IM_REPAIRITEM_REQ( pItem->m_dwItemID, pItem->m_bSackID, pItem->m_bSackPos);

								m_vecItem[i]->m_wPrev = wTemp;
								m_vecItem[i]->m_bShowMessage = false;
							}
						}						
						break;
					}
					
					VSHORTENDU::iterator it = find( g_ShortEndu.begin(), g_ShortEndu.end(), i);

					if( it == g_ShortEndu.end())
						g_ShortEndu.push_back( i);

					bRepairShow = true;
				}
				else
				{
					VSHORTENDU::iterator it = find( g_ShortEndu.begin(), g_ShortEndu.end(), i);

					if( it != g_ShortEndu.end())
						g_ShortEndu.erase( it);
				}
			}
			else
			{
				VSHORTENDU::iterator it = find( g_ShortEndu.begin(), g_ShortEndu.end(), i);

				if( it != g_ShortEndu.end())
					g_ShortEndu.erase( it);
			}
		}
	}

	// 0.5초 간격
	if(m_dwTime+500 <= g_dwCurTime)
	{
		if(bRepairShow)
			m_bRepairShow = !m_bRepairShow;
		else
			m_bRepairShow = false;

		m_dwTime = g_dwCurTime;
	}
}


/**
 *
 * \param nPosition 
 * \return 
 */
XiahItem::sItemInfo* CEquipSack::FindSackItemByPos( int nPosition)
{
	if( nPosition < 0 || nPosition >= m_bySackTotalSize)
		return NULL;

	if( m_vecItem[ nPosition])
	{
		return m_vecItem[ nPosition];
	}	
	
	return NULL;
}

/**
 *
 * \param nPosition 
 * \return 
 */
XiahItem::sItemInfo* CEquipSack::FindSackItemByPosPrev( int nPosition)
{
	if( m_bySackType == SACKTYPE__MODIFY)
	{
		for( int i=0; i < m_bySackTotalSize; ++i)
		{
			if( m_vecItem[ i] && m_vecItem[ i]->m_bSackPosPrev == nPosition)
				return m_vecItem[ i];
		}
	}

	return NULL;
}
