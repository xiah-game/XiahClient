#include "precompile.h"
#include "charsack.h"
#include "CharacterInfo.h"
#include "XiahGame_Main.h"
#include "InterfaceDefine.h"



CCharSack::CCharSack( BYTE bySackType, BYTE bySackXSize, BYTE bySackYSize)
{
	m_bySackType  = bySackType;
	m_byCellXSize = DEFAULT_CELL_XSIZE;
	m_byCellYSize = DEFAULT_CELL_YSIZE;
	m_byCellDis	  = DEFAULT_CELL_DISTANCE;
	m_bySackXSize = bySackXSize;
	m_bySackYSize = bySackYSize;
	m_bySackTotalSize = bySackXSize * bySackYSize;

	m_pMoneyVB = NULL;
	m_pMoneyTex = NULL;

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

	// character sack cell 영역
	SetSackRegion();
}

CCharSack::~CCharSack(void)
{
	if( m_pMoneyVB )
	{
		m_pMoneyVB->Release();
		m_pMoneyVB = NULL;
	}
}

void CCharSack::SetSackRegion()
{
	RECT rtRegion;
	switch( m_bySackType)
	{
		case SACKTYPE__DEFAULT:
			{
				g_pUIManager->GetRegionData(DRG_ITEM_WINDOW, drg_item_window_sack_dummy, rtRegion);

				sRect Rect;
				g_pUIManager->GetRegionData(DRG_ITEM_WINDOW, drg_item_window_money_dummy, Rect);

				if(!m_pMoneyVB)
					g_pDirect3DDevice->CreateVertexBuffer( 4*sizeof(VT_TLVertex),
														0, D3DFVF_TLVERTEX,
														D3DPOOL_MANAGED, &m_pMoneyVB, NULL);

				VT_TLVertex Vertex[4];

				Vertex[ 0].pos = Vector4( Rect.left,  Rect.top, 0, 1);
				Vertex[ 1].pos = Vector4( Rect.right, Rect.top, 0, 1);
				Vertex[ 2].pos = Vector4( Rect.left,  Rect.bottom, 0, 1);
				Vertex[ 3].pos = Vector4( Rect.right, Rect.bottom, 0, 1);

				Vertex[ 0].diffuse = Vertex[ 1].diffuse = Vertex[ 2].diffuse = Vertex[ 3].diffuse = 0xffffffff;

				Vertex[ 0].tex = Vector2( 0, 0);
				Vertex[ 1].tex = Vector2( 1, 0);
				Vertex[ 2].tex = Vector2( 0, 1);
				Vertex[ 3].tex = Vector2( 1, 1);

				VOID* pVertices;
				if( !FAILED( m_pMoneyVB->Lock( 0, sizeof(Vertex), (void**)&pVertices, 0 )))
				{
					memcpy( pVertices, Vertex, sizeof(Vertex) );
					m_pMoneyVB->Unlock();
				}
				else
				{
					DBG_LogFile( _T("Charsack Lock fail"));
				}

				// textrue 세팅
				m_pMoneyTex = Gettex( 50000319);
			}			
			break;
		case SACKTYPE__NPC_TRADE:
		case SACKTYPE__ITEMMALL:	// 아이템몰
		case SACKTYPE__DEPOSIT:
		case SACKTYPE__PERSONAL_TRADE_SELL:
		case SACKTYPE__QUICKMART:	// 매품패
			g_pUIManager->GetRegionData(WINDOW_NPC_TRADE, npc_trade_window_dummy_01, rtRegion);
			break;
		case SACKTYPE__PC_TRADE_MINE:
			g_pUIManager->GetRegionData(WINDOW_PC_TRADE, pc_trade_window_dummy_25, rtRegion);
			break;
		case SACKTYPE__PC_TRADE_OTHER:
			g_pUIManager->GetRegionData(WINDOW_PC_TRADE, pc_trade_window_dummy_01, rtRegion);
			break;
		case SACKTYPE__PET:
			g_pUIManager->GetRegionData(WINDOW_TAMING_ITEM, taming_item_window_sack_dummy_01, rtRegion);
			break;
		case SACKTYPE__PERSONAL_TRADE_SET:
			g_pUIManager->GetRegionData(WINDOW_PC_STORE, pc_store_box_dummy_01, rtRegion);
			break;
		case SACKTYPE__SMELT:			// 조합
			g_pUIManager->GetRegionData(WINDOW_SMELT, smelt_window_bummy_01, rtRegion);		
			break;
	} // switch( m_bySackType)

	m_nFirstCellXPos = rtRegion.left;
	m_nFirstCellYPos = rtRegion.top;

	// character sack 전체 영역
	m_SackRt.left	= rtRegion.left;
	m_SackRt.top	= rtRegion.top;
	m_SackRt.right	= m_SackRt.left + m_bySackXSize * ( DEFAULT_CELL_XSIZE + DEFAULT_CELL_DISTANCE);
	m_SackRt.bottom = m_SackRt.top	+ m_bySackYSize * ( DEFAULT_CELL_YSIZE + DEFAULT_CELL_DISTANCE);

	// money
	/*
	if( m_bySackType == SACKTYPE__DEFAULT)
	{		
		sRect Rect;
		g_pUIManager->GetRegionData(DRG_ITEM_WINDOW, drg_item_window_money_dummy, Rect);

		if(!m_pMoneyVB)
			g_pDirect3DDevice->CreateVertexBuffer( 4*sizeof(VT_TLVertex),
											0, D3DFVF_TLVERTEX,
											D3DPOOL_MANAGED, &m_pMoneyVB, NULL);

		VT_TLVertex Vertex[4];

		Vertex[ 0].pos = Vector4( Rect.left,  Rect.top, 0, 1);
		Vertex[ 1].pos = Vector4( Rect.right, Rect.top, 0, 1);
		Vertex[ 2].pos = Vector4( Rect.left,  Rect.bottom, 0, 1);
		Vertex[ 3].pos = Vector4( Rect.right, Rect.bottom, 0, 1);

		Vertex[ 0].diffuse = Vertex[ 1].diffuse = Vertex[ 2].diffuse = Vertex[ 3].diffuse = 0xffffffff;

		Vertex[ 0].tex = Vector2( 0, 0);
		Vertex[ 1].tex = Vector2( 1, 0);
		Vertex[ 2].tex = Vector2( 0, 1);
		Vertex[ 3].tex = Vector2( 1, 1);

		VOID* pVertices;
		if( !FAILED( m_pMoneyVB->Lock( 0, sizeof(Vertex), (void**)&pVertices, 0 )))
		{
			memcpy( pVertices, Vertex, sizeof(Vertex) );
			m_pMoneyVB->Unlock();
		}
		else
		{
			DBG_LogFile( _T("Charsack Lock fail"));
		}

		// textrue 세팅
		m_pMoneyTex = Gettex( 50000319);
	}
	*/
}

void CCharSack::DrawSack()
{
	if( !m_bShow)
		return;

	if( m_bySackType == SACKTYPE__DEFAULT)
		DrawMoney();

	CSack::DrawSack();
}

void CCharSack::DrawMoney()
{
	g_pDirect3DDevice->SetRenderState( D3DRS_ZENABLE, FALSE);
	g_pDirect3DDevice->SetRenderState( D3DRS_FOGENABLE, FALSE);

	g_pDirect3DDevice->SetRenderState( D3DRS_ALPHABLENDENABLE, TRUE);
	g_pDirect3DDevice->SetRenderState( D3DRS_DIFFUSEMATERIALSOURCE, D3DMCS_COLOR1);
	g_pDirect3DDevice->SetRenderState( D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	g_pDirect3DDevice->SetRenderState( D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
	
	g_pDirect3DDevice->SetSamplerState( 0,  D3DSAMP_ADDRESSU , D3DTADDRESS_CLAMP);
	g_pDirect3DDevice->SetSamplerState( 0,  D3DSAMP_ADDRESSV , D3DTADDRESS_CLAMP);

	g_Device.SetTexture(0, m_pMoneyTex);
	//g_pDirect3DDevice->SetTexture( 0, m_pMoneyTex);
	g_Device.SetStreamSource( m_pMoneyVB, sizeof(VT_TLVertex));
	g_Device.SetFVF(D3DFVF_TLVERTEX);
	//g_pDirect3DDevice->SetFVF( D3DFVF_TLVERTEX);

	if(FAILED(g_pDirect3DDevice->DrawPrimitive( D3DPT_TRIANGLESTRIP, 0, 2)))
	{
		DBG_LogFile( _T("Draw Money fail"));
	}
}

/**
 *
 * \return 
 */
BOOL CCharSack::CheckItemUnSelected()
{
	if( !m_bShow)
		return FALSE;
	
	if( CSack::CheckItemUnSelected())
	{
		for( int i=0; i < m_bySackTotalSize; ++i)
		{
			sRect temp;
			switch( m_bySackType)
			{
				case SACKTYPE__DEFAULT:
					g_pUIManager->GetRegionData(DRG_ITEM_WINDOW, drg_item_window_sack_dummy + i, temp);
					break;
				case SACKTYPE__NPC_TRADE:
				case SACKTYPE__DEPOSIT:
				case SACKTYPE__ITEMMALL:
				case SACKTYPE__QUICKMART:			// 매품패
					g_pUIManager->GetRegionData(WINDOW_NPC_TRADE, npc_trade_window_dummy_01 + i, temp);
					break;

				case SACKTYPE__PERSONAL_TRADE_SET: // 개인 상점 설정
					g_pUIManager->GetRegionData(WINDOW_PC_STORE, pc_store_box_dummy_01 + i, temp);
					break;
				case SACKTYPE__PC_TRADE_MINE:
					g_pUIManager->GetRegionData(WINDOW_PC_TRADE, pc_trade_window_dummy_25 + i, temp);
					break;
				case SACKTYPE__PC_TRADE_OTHER:
					g_pUIManager->GetRegionData(WINDOW_PC_TRADE, pc_trade_window_dummy_01 + i, temp);
					break;
				case SACKTYPE__PET:
					g_pUIManager->GetRegionData(WINDOW_TAMING_ITEM, taming_item_window_sack_dummy_01 + i, temp);
					break;
				case SACKTYPE__PERSONAL_TRADE_SELL:
					{
						if( g_MainCharInfo.m_pHoldItem->IsHoldingItemItem())
							g_MainCharInfo.m_pHoldItem->SetItemBackToSack();

						return true;
					}				
					break;
				case SACKTYPE__SMELT:	// 조합
					g_pUIManager->GetRegionData(WINDOW_SMELT, smelt_window_bummy_01 + i, temp);
					break;
			}

			if( temp.PtInRect( XiahInput::g_ptMouse))
			{
				return ProcessItemUnSelected( i);
			}
		}

		return ProcessItemUnSelected( 255);
	}

	return FALSE;
}

/**
 *
 * \param bSackPos 
 */
void CCharSack::DeleteItem( BYTE bSackPos, bool bDelete)
{
	// 누수 수정
//	if( m_bySackType == SACKTYPE__NPC_TRADE)
//		return;

	CSack::DeleteItem(bSackPos, bDelete);
}

/**
 *
 * \param nPosition 
 * \param pItem 
 */
void CCharSack::SetVB( BYTE nPosition, XiahItem::sItemInfo* pItem)
{
	sRect* rtRect = new sRect;

	BYTE xDiff = (nPosition % m_bySackXSize);
	BYTE yDiff = (nPosition / m_bySackXSize);

	rtRect->left   = m_nFirstCellXPos + ( xDiff * m_byCellXSize) + ( xDiff * DEFAULT_CELL_DISTANCE);
	rtRect->right  = rtRect->left	  + ( pItem->m_bSackSizeX * DEFAULT_CELL_XSIZE);
	rtRect->top    = m_nFirstCellYPos + ( yDiff * m_byCellYSize) + ( yDiff * DEFAULT_CELL_DISTANCE);
	rtRect->bottom = rtRect->top	  + ( pItem->m_bSackSizeY * DEFAULT_CELL_YSIZE);

	if(m_vecItemRt[ nPosition])
		delete m_vecItemRt[ nPosition], m_vecItemRt[ nPosition] = NULL;

	m_vecItemRt[ nPosition] = rtRect;

	CSack::SetVB( nPosition, pItem);
}

/**
 *
 * \param nPosition 
 * \return 
 */
XiahItem::sItemInfo* CCharSack::FindSackItemByPos( int nPosition)
{
	// 위치 초과시 검사
	if(nPosition == 0xff || nPosition >= m_bySackTotalSize)
		return NULL;

	// 아이템의 크기를 계산한 포지션 값을 계산
	// default행낭의 크기 2*2인 아이템만 계산
	//int temp[ m_bySackTotalSize];
	int* temp = (int*)malloc( sizeof(int) * m_bySackTotalSize);

#ifdef TRACE_LOG
	if(temp == NULL) DBG_LogFile( _T("FindSackItemByPos fail"));
#endif

	for(int i=0; i < m_bySackTotalSize; ++i)
	{
		temp[ i] = -1;
	}
	
	for( int i=0; i < m_bySackTotalSize; ++i)
	{
		if( m_vecItem[ i])
		{
			if( m_vecItem[i]->m_bSackSizeX == 1)
			{
				temp[i] = i;
			}

			if( m_vecItem[i]->m_bSackSizeX == 2)
			{
				temp[i] = i;
				if(i+1 < m_bySackTotalSize) temp[i+1]=i;
				if(i + m_bySackXSize < m_bySackTotalSize) temp[i + m_bySackXSize] = i;
				if(i + m_bySackXSize +1 < m_bySackTotalSize) temp[i + m_bySackXSize +1] = i;
			}
		}
	}

	int nAbsPosition = temp[ nPosition];

	if( nAbsPosition != -1)
	{
		if( m_vecItem[ nAbsPosition])
		{
			free( temp);
			return m_vecItem[ nAbsPosition];
		}
	}
	
	free( temp);

	return NULL;
}

void CCharSack::RefreshSackPos()
{
	if(m_bySackType == SACKTYPE__PERSONAL_TRADE_SET) // 이런 몰라 이상한 구조라서 몰라.
	{
		SetSackRegion();
	}

	for( int i=0; i < m_bySackTotalSize; ++i)
	{
		if( m_vecItem[ i])
		{
			SetSackRegion();
			SetVB( i, m_vecItem[ i]);
		}
	}
}
