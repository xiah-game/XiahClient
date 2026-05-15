#include "precompile.h"
#include "holditem.h"
#include "CharacterInfo.h"
#include "XiahArrayIndex.h"
#include "InterfaceDefine.h"
#include "XiahCamera.h"
#include "XiahGame_Handler_Sender.h"
#include "XiahGame_Pet.h"
#include "XiahMap.h"

CHoldItem::CHoldItem(void)
{
	m_fDraw = TRUE;

	m_pHoldItemItem = NULL;
	m_dwHoldItemMugong = m_bBackPosition = 0;
	m_pHoldItemMoney = NULL;
	m_pHoldItemItemFromNpcSack = NULL;

	m_pHoldItemItemVB = NULL;
	CreateHoldItemVB();

	m_pHoldItemMoney = new XiahItem::sHoldMoney();
}

CHoldItem::~CHoldItem(void)
{
	DeleteHoldItemItem();

	if( m_pHoldItemItemVB)
	{
		m_pHoldItemItemVB->Release();
		m_pHoldItemItemVB = NULL;
	}

	if( m_pHoldItemMoney)
	{
		delete m_pHoldItemMoney;
		m_pHoldItemMoney = NULL;
	}
}

void CHoldItem::DrawHoldItem()
{
	if( m_fDraw)
	{
		DrawHoldItemItem();
		DrawHoldItemMugong();
		DrawHoldItemMoney();
	}
}

void CHoldItem::DrawHoldItemItem()
{
	if( m_pHoldItemItem)
	{
		g_pDirect3DDevice->SetRenderState( D3DRS_ZENABLE, FALSE);
		g_pDirect3DDevice->SetRenderState( D3DRS_FOGENABLE, FALSE);

		g_pDirect3DDevice->SetRenderState( D3DRS_ALPHABLENDENABLE, TRUE);
		g_pDirect3DDevice->SetRenderState( D3DRS_DIFFUSEMATERIALSOURCE, D3DMCS_COLOR1);
		g_pDirect3DDevice->SetRenderState( D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
		g_pDirect3DDevice->SetRenderState( D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
		
		g_pDirect3DDevice->SetSamplerState( 0,  D3DSAMP_ADDRESSU , D3DTADDRESS_CLAMP);
		g_pDirect3DDevice->SetSamplerState( 0,  D3DSAMP_ADDRESSV , D3DTADDRESS_CLAMP);

		MakeHoldItemVB( 1);

		g_Device.SetTexture(0, XiahPak::GetTexture( m_pHoldItemItem->m_nResID));
		//g_pDirect3DDevice->SetTexture( 0, XiahPak::GetTexture( m_pHoldItemItem->m_nResID));
		g_Device.SetStreamSource( m_pHoldItemItemVB, sizeof(VT_TLVertex));
		g_Device.SetFVF(D3DFVF_TLVERTEX);
		//g_pDirect3DDevice->SetFVF( D3DFVF_TLVERTEX);
		g_pDirect3DDevice->DrawPrimitive( D3DPT_TRIANGLESTRIP, 0, 2);
	}
}

void CHoldItem::DrawHoldItemMugong()
{
	if( m_dwHoldItemMugong)
	{
		g_pDirect3DDevice->SetRenderState( D3DRS_ZENABLE, FALSE);
		g_pDirect3DDevice->SetRenderState( D3DRS_FOGENABLE, FALSE);

		g_pDirect3DDevice->SetRenderState( D3DRS_ALPHABLENDENABLE, TRUE);
		g_pDirect3DDevice->SetRenderState( D3DRS_DIFFUSEMATERIALSOURCE, D3DMCS_COLOR1);
		g_pDirect3DDevice->SetRenderState( D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
		g_pDirect3DDevice->SetRenderState( D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
		
		g_pDirect3DDevice->SetSamplerState( 0,  D3DSAMP_ADDRESSU , D3DTADDRESS_CLAMP);
		g_pDirect3DDevice->SetSamplerState( 0,  D3DSAMP_ADDRESSV , D3DTADDRESS_CLAMP);

		MakeHoldItemVB( 0);

		if(m_dwHoldItemMugong >=161 && m_dwHoldItemMugong <=179)
		{
			sArrayData *pData = XiahArrayIndex::g_RebirthMugong_List.GetData(m_dwHoldItemMugong,1);

			if(pData == NULL)
			{
				DBG_LogFile( _T("CHoldItem::DrawHoldItemMugong fail"));
			}

			
			int nResID = pData->GetInt(2);

			g_Device.SetTexture(0, XiahPak::GetTexture( nResID));
			//g_pDirect3DDevice->SetTexture( 0, XiahPak::GetTexture( nResID));
			g_Device.SetStreamSource( m_pHoldItemItemVB, sizeof(VT_TLVertex));
			g_Device.SetFVF(D3DFVF_TLVERTEX);
			//g_pDirect3DDevice->SetFVF( D3DFVF_TLVERTEX);
			g_pDirect3DDevice->DrawPrimitive( D3DPT_TRIANGLESTRIP, 0, 2);
		}
		
		else
		{
			sArrayData *pData = XiahArrayIndex::g_MugongTemplate.GetData( m_dwHoldItemMugong);

			if(pData == NULL)
			{
				DBG_LogFile( _T("CHoldItem::DrawHoldItemMugong fail"));
			}


			int nResID = pData->GetInt( 1);

			g_Device.SetTexture(0, XiahPak::GetTexture( nResID));
			//g_pDirect3DDevice->SetTexture( 0, XiahPak::GetTexture( nResID));
			g_Device.SetStreamSource( m_pHoldItemItemVB, sizeof(VT_TLVertex));
			g_Device.SetFVF(D3DFVF_TLVERTEX);
			//g_pDirect3DDevice->SetFVF( D3DFVF_TLVERTEX);
			g_pDirect3DDevice->DrawPrimitive( D3DPT_TRIANGLESTRIP, 0, 2);
		}
	}
}

void CHoldItem::DrawHoldItemMoney()
{
	if( IsHoldingItemMoney())
	{
		g_pDirect3DDevice->SetRenderState( D3DRS_ZENABLE, FALSE);
		g_pDirect3DDevice->SetRenderState( D3DRS_FOGENABLE, FALSE);

		g_pDirect3DDevice->SetRenderState( D3DRS_ALPHABLENDENABLE, TRUE);
		g_pDirect3DDevice->SetRenderState( D3DRS_DIFFUSEMATERIALSOURCE, D3DMCS_COLOR1);
		g_pDirect3DDevice->SetRenderState( D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
		g_pDirect3DDevice->SetRenderState( D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
		
		g_pDirect3DDevice->SetSamplerState( 0,  D3DSAMP_ADDRESSU , D3DTADDRESS_CLAMP);
		g_pDirect3DDevice->SetSamplerState( 0,  D3DSAMP_ADDRESSV , D3DTADDRESS_CLAMP);

		MakeHoldItemVB( 2);

		g_Device.SetTexture(0, XiahPak::GetTexture(50000319));
		//g_pDirect3DDevice->SetTexture( 0, XiahPak::GetTexture( 50000319));
		g_Device.SetStreamSource( m_pHoldItemItemVB, sizeof(VT_TLVertex));
		g_Device.SetFVF(D3DFVF_TLVERTEX);
		//g_pDirect3DDevice->SetFVF( D3DFVF_TLVERTEX);
		g_pDirect3DDevice->DrawPrimitive( D3DPT_TRIANGLESTRIP, 0, 2);
	}
}

void CHoldItem::CreateHoldItemVB()
{
	// HOLD ITEM VB
//	g_pDirect3DDevice->CreateVertexBuffer( 4*sizeof(VT_TLVertex),
//										D3DUSAGE_WRITEONLY | D3DUSAGE_DYNAMIC, D3DFVF_TLVERTEX,
//										D3DPOOL_DEFAULT, &m_pHoldItemItemVB, NULL);

	g_pDirect3DDevice->CreateVertexBuffer( 4*sizeof(VT_TLVertex),
										0, D3DFVF_TLVERTEX,
										D3DPOOL_MANAGED, &m_pHoldItemItemVB, NULL);
}

void CHoldItem::MakeHoldItemVB( BYTE byType)
{
	sRect rtRect;

	rtRect.left = XiahInput::g_ptMouse.x;
	rtRect.top = XiahInput::g_ptMouse.y;

	switch( byType)
	{
		case 0:	// mugong
			{
				rtRect.right = rtRect.left + DEFAULT_CELL_XSIZE;
				rtRect.bottom = rtRect.top + DEFAULT_CELL_XSIZE;
			}		
			break;
		case 1:	// item
			{
				rtRect.right = rtRect.left + m_pHoldItemItem->m_bSackSizeX * DEFAULT_CELL_XSIZE;
				rtRect.bottom = rtRect.top + m_pHoldItemItem->m_bSackSizeY * DEFAULT_CELL_XSIZE;
			}		
			break;
		case 2:	// money
			{
				rtRect.right = rtRect.left + DEFAULT_CELL_XSIZE;
				rtRect.bottom = rtRect.top + DEFAULT_CELL_XSIZE;
			}
			break;
	}

	//HT_CHEAT : WINDOWSIZE	
	RECT* prtWindow = XiahGameEngine::GetWindowRect();
	if ( prtWindow && prtWindow->right && prtWindow->bottom )
	{
		float fXRatio = float(G_WIDTH)  / float(prtWindow->right);
		float fYRatio = float(G_HEIGHT) / float(prtWindow->bottom);

		if ( fXRatio && fXRatio != 1.0f )
		{
			rtRect.left		= rtRect.left  * fXRatio;
			rtRect.right	= rtRect.right * fXRatio;
		}

		if ( fYRatio && fYRatio != 1.0f )
		{
			rtRect.top		= rtRect.top	 * fYRatio;
			rtRect.bottom	= rtRect.bottom  * fYRatio;
		}
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
	if( !FAILED( m_pHoldItemItemVB->Lock( 0, sizeof(Vertex), (void**)&pVertices, 0 )))
	{
		memcpy( pVertices, Vertex, sizeof(Vertex) );
		m_pHoldItemItemVB->Unlock();
	}
	else
	{
		DBG_LogFile( _T("CHoldItem::MakeHoldItemVB fail"));
	}
}

BOOL CHoldItem::IsHoldingItem()
{
	if( m_pHoldItemItem)
		return TRUE;

	if( m_dwHoldItemMugong)
		return TRUE;

	if( IsHoldingItemMoney())
		return TRUE;

//	if( m_pHoldItemItemFromNpcSack)
//		return TRUE;


	return FALSE;
}

BOOL CHoldItem::IsHoldingItemItem()
{
	if( m_pHoldItemItem)
		return TRUE;

	return FALSE;
}

BOOL CHoldItem::IsHoldingItemMugong()
{
	if( m_dwHoldItemMugong)
		return TRUE;

	return FALSE;
}

BOOL CHoldItem::IsHoldingItemMoney()
{
	if( m_pHoldItemMoney && m_pHoldItemMoney->m_dwAmount)
		return TRUE;

	return FALSE;
}

void CHoldItem::SetHoldItemItem( XiahItem::sItemInfo* pItem, BOOL bFlag) 
{ 
	g_MainCharInfo.PlayInterfaceSound( ISOUND_SELECT_BUTTON);
	m_pHoldItemItem = pItem;
	m_fDraw = bFlag;
}

void CHoldItem::SetHoldItemMoneySack( BYTE bySrcSackID)
{
	if( m_pHoldItemMoney)
		m_pHoldItemMoney->m_bySrcSackID = bySrcSackID;
}

void CHoldItem::SetHoldItemMoney( DWORD dwAmount) 
{ 
	g_MainCharInfo.PlayInterfaceSound( ISOUND_SELECT_BUTTON);
	
	if( m_pHoldItemMoney)
		m_pHoldItemMoney->m_dwAmount = dwAmount;
}

void CHoldItem::ReleaseHoldItemMoney()
{
	if( m_pHoldItemMoney)
	{
		m_pHoldItemMoney->m_dwAmount = 0;
	}
}

void CHoldItem::SetHoldItemMugong( DWORD dwMugong) 
{ 
	g_MainCharInfo.PlayInterfaceSound( ISOUND_SELECT_BUTTON);
	m_dwHoldItemMugong = dwMugong;
}

void CHoldItem::SetHoldItemItemFromNpcSack( XiahItem::sItemInfo* pItem) 
{ 
	g_MainCharInfo.PlayInterfaceSound( ISOUND_SELECT_BUTTON);
	m_pHoldItemItemFromNpcSack = pItem;
}

void CHoldItem::SetItemBackToSack()
{
	if( IsHoldingItemItem())
	{
		switch( GetHoldItemItem()->m_bSackID)
		{
			case SACKTYPE__DEFAULT:
				g_MainCharInfo.m_pMySack[ GetHoldItemItem()->m_bSackCount]->InsertItem( GetHoldItemItem()->m_bSackPos, GetHoldItemItem());
				break;
			case SACKTYPE__EQUIPMENT:
				g_MainCharInfo.m_pEquipSack->InsertItem( GetHoldItemItem()->m_bSackPos, GetHoldItemItem());
				break;
			case SACKTYPE__PC_TRADE_MINE:
				// 내 색으로 가야지..
				g_MainCharInfo.m_pMySack[g_MainCharInfo.m_byMySackCurrIdx]->InsertItem( GetHoldItemItem()->m_bSackPos, GetHoldItemItem());
				break;
			case SACKTYPE__PERSONAL_TRADE_SET:
				if(g_MainCharInfo.m_pPersonalTradeSet)
					g_MainCharInfo.m_pPersonalTradeSet->InsertItem(GetHoldItemItem()->m_bSackPos, GetHoldItemItem());
				break;
			case SACKTYPE__PERSONAL_TRADE_SELL:
				if(g_MainCharInfo.m_pPersonalTradeSell)
					g_MainCharInfo.m_pPersonalTradeSell->InsertItem(GetHoldItemItem()->m_bSackPos, GetHoldItemItem());
				break;
			case SACKTYPE__SMELT:	// 조합
				{
					if(g_MainCharInfo.m_pSmeltSack)
						g_MainCharInfo.m_pSmeltSack->InsertItem(GetHoldItemItem()->m_bSackPos, GetHoldItemItem());
				}
				break;
			case SACKTYPE__ITEMMALL:
				{
					if(g_MainCharInfo.m_pItemMallSack)
						g_MainCharInfo.m_pItemMallSack->InsertItem( GetHoldItemItem()->m_bSackPos, GetHoldItemItem());
				}			
				break;

			case SACKTYPE__DEPOSIT:
				if(g_MainCharInfo.m_pDepositSack)
					g_MainCharInfo.m_pDepositSack->InsertItem( GetHoldItemItem()->m_bSackPos, GetHoldItemItem());
				break;
			case SACKTYPE__PET:
				{
					sPetInfo* pPetInfo = g_PetList.GetCurrentPet();
					if( pPetInfo)
					{
						pPetInfo->m_pSack[ pPetInfo->m_byMySackCurrIdx]->InsertItem( GetHoldItemItem()->m_bSackPos, GetHoldItemItem());
					}
				}
				break;
			case SACKTYPE__PET_EQUIP:
				{
					sPetInfo* pPetInfo = g_PetList.GetCurrentPet();
					if( pPetInfo)
					{
						pPetInfo->m_pEquipSack->InsertItem( GetHoldItemItem()->m_bSackPos, GetHoldItemItem());
					}
				}
				break;
			case SACKTYPE__COLLECTION:	// 아이템 수집
				{
					g_MainCharInfo.m_pCollection->InsertItem(GetHoldItemItem()->m_bSackPos, GetHoldItemItem());
				}
				break;
		}

		g_MainCharInfo.PlayInterfaceSound( ISOUND_WARNING);
		EmptyHoldItemItem();
	}

	if( IsHoldingItemMugong())
	{
		SetHoldItemMugong( 0);
	}
}

void CHoldItem::DeleteHoldItemItem()
{
	if( m_pHoldItemItem)
	{
		delete m_pHoldItemItem;
		m_pHoldItemItem = NULL;
	}
}

void CHoldItem::EmptyHoldItemItem()
{
	m_pHoldItemItem = NULL;
}

void CHoldItem::ThrowItem( BYTE byType)
{
	if(g_MainCharInfo.m_bPersonalTradeSell)
		return;

	if( byType == 0)
	{
		//float height = Map::g_MapRes.GetHeight( g_XiahCamera.m_vAt.x, -g_XiahCamera.m_vAt.z);
		CXiahCharObject* pMainObj = (CXiahCharObject*)g_pMainChar->m_pObject;

		if(pMainObj)
		{
			float fix_x = (float)((rand()%10)-5);
			float fix_z = (float)((rand()%10)-5);

			if(fix_x > -3.0f && fix_x < 3.0f)
			{
				if(fix_x >= 0.0f) fix_x = ((rand() % 3)+1);
				else
					if(fix_x < 0.0f) fix_x = -((rand() % 3)+1);
			}

			if(fix_z > -3.0f && fix_z < 3.0f)
			{
				if(fix_z >= 0.0f) fix_z = ((rand() % 3)+1);
				else
					if(fix_z < 0.0f) fix_z = -((rand() % 3)+1);
			}

			float posx = pMainObj->m_Position.x + fix_x;
			float posz = -pMainObj->m_Position.z + fix_z;

			if(XiahMap::g_Map_Attri.Get_Attr(posx,posz) == 0x01)	// 들어갈수 없는곳이면 다시!
			{
				DBG_Put( _T("들어갈수 없는곳에 아이템 드롭!"));
			}

			SendCS_IM_THROW_REQ( GetHoldItemItem()->m_bSackCount+1,
								GetHoldItemItem()->m_bSackPos, 
								GetHoldItemItem()->m_dwItemID, 
								posx,
								posz,
								0,
								GetHoldItemItem()->m_dwAmount);
		}		
	}
	else
	{
		float height = Map::g_MapRes.GetHeight( g_XiahCamera.m_vAt.x, -g_XiahCamera.m_vAt.z);

		sPetInfo* pPet = g_PetList.GetCurrentPet();
		if( pPet)
		{
			SendCS_NC_PETTHROWITEM_REQ( pPet->dwID,
										GetHoldItemItem()->m_dwItemID,
										GetHoldItemItem()->m_bSackCount,
										GetHoldItemItem()->m_bSackPos, 
										g_XiahCamera.m_vAt.x, 
										-g_XiahCamera.m_vAt.z, 
										height,
										GetHoldItemItem()->m_dwAmount);
		}
	}

	//g_MainCharInfo.PlayInterfaceSound( ISOUND_ITEM_PICKUP);
}

void CHoldItem::ThrowMoney()
{
	if(g_MainCharInfo.m_bPersonalTradeSell)
		return;

	DWORD dwAmount = m_pHoldItemMoney->m_dwAmount;

	float height = Map::g_MapRes.GetHeight( g_XiahCamera.m_vAt.x, -g_XiahCamera.m_vAt.z);
	SendCS_IM_THROWMONEY_REQ( 20042, 
							g_XiahCamera.m_vAt.x,
							-g_XiahCamera.m_vAt.z,
							height,
							dwAmount);

	ReleaseHoldItemMoney();
	//g_MainCharInfo.PlayInterfaceSound( ISOUND_ITEM_PICKUP);	
}