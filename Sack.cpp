#include "precompile.h"
#include "resource.h"
#include "AppData.h"
#include "sack.h"
#include "XiahGameMain.h"
#include "XiahGame_Intro.h"
#include "InterfaceDefine.h"
#include "XiahGame_Handler_Sender.h"
#include "CharSack.h"
#include "EquipSack.h"
#include "XiahCursor.h"
#include "XiahGame_Pet.h"
#include "FunctionalNpcInfo.h"
#include "XiahArrayIndex.h"

#include "ProcessUnSelect.cpp"
#include "InterfaceHandler.h"
#include "mail.h"

#include <assert.h>

//HT_CHEAT : 치트 키 
extern BOOL g_bCheat;

CSack::CSack() : m_bShow(FALSE), m_bRefreshToolTip(FALSE), m_nCurToolTipItemPos(-1), m_nPrevToolTipItemPos(-1),
				m_bAction(0)
{
	m_vecItem.clear();
	m_vecItemVB.clear();
	m_vecItemTex.clear();
	m_vecItemRt.clear();
	// 제련
	m_vecItemSocketVB.clear();
	m_vecSocketItem1VB.clear();
	m_vecSocketItem2VB.clear();
	m_vecSocketItem3VB.clear();
	m_vecRBSocketItemVB.clear();//HT_1116 : 각성자 아이템 추가
	m_vecRBItemStoneVB.clear();
}

CSack::~CSack()
{
	for(int i=0; i < m_bySackTotalSize; ++i)
	{
		if( m_vecItem[i])
		{
			delete m_vecItem[i];
			m_vecItem[i] = NULL;
		}

		if( m_vecItemVB[i])
		{
			m_vecItemVB[i]->Release();
			m_vecItemVB[i] = NULL;
		}

		if( m_vecItemRt[i])
		{
			delete m_vecItemRt[i];
			m_vecItemRt[i] = NULL;
		}

		// 제련
		if(m_vecItemSocketVB[i])
		{
			m_vecItemSocketVB[i]->Release();
			m_vecItemSocketVB[i] = NULL;
		}

		if(m_vecSocketItem1VB[i])
		{
			m_vecSocketItem1VB[i]->Release();
			m_vecSocketItem1VB[i] = NULL;
		}

		if(m_vecSocketItem2VB[i])
		{
			m_vecSocketItem2VB[i]->Release();
			m_vecSocketItem2VB[i] = NULL;
		}

		if(m_vecSocketItem3VB[i])
		{
			m_vecSocketItem3VB[i]->Release();
			m_vecSocketItem3VB[i] = NULL;
		}
		//HT_1116 : 각성자 아이템 추가
		if(m_vecRBSocketItemVB[i])
		{
			m_vecRBSocketItemVB[i]->Release();
			m_vecRBSocketItemVB[i] = NULL;
		}
		if(m_vecRBItemStoneVB[i])
		{
			m_vecRBItemStoneVB[i]->Release();
			m_vecRBItemStoneVB[i] = NULL;
		}
	}

	m_vecItem.clear();
	m_vecItemVB.clear();
	m_vecItemTex.clear();
	m_vecItemRt.clear();

	// 제련
	m_vecItemSocketVB.clear();
	m_vecSocketItem1VB.clear();
	m_vecSocketItem2VB.clear();
	m_vecSocketItem3VB.clear();
	m_vecRBSocketItemVB.clear();//HT_1116 : 각성자 아이템 추가
	m_vecRBItemStoneVB.clear();
}


/**
 *
 * \param bFlagForModifySack 
 */
void CSack::HideSack( BOOL bFlagForModifySack)
{
	m_bShow = FALSE;

	if( m_bySackType == SACKTYPE__DEFAULT || m_bySackType == SACKTYPE__EQUIPMENT || m_bySackType == SACKTYPE__PET ||
		m_bySackType == SACKTYPE__PET2 || m_bySackType == SACKTYPE__PET3 || m_bySackType == SACKTYPE__PET_EQUIP ||
		m_bySackType == SACKTYPE__COLLECTION)
		return;

	for( int i=0; i < m_bySackTotalSize; ++i)
	{
		if(m_vecItem[i] != NULL)
		{
			// 오행 제련 추가
			if(m_bySackType == SACKTYPE__MODIFY || SACKTYPE__SMELT == m_bySackType || SACKTYPE__FIVEELEMENT_CONVERT == m_bySackType )
			{
				g_MainCharInfo.m_pMySack[m_vecItem[i]->m_bSackIDPrev]->InsertItem(m_vecItem[i]->m_bSackPosPrev, m_vecItem[i]);
				m_vecItem[i] = NULL;
			}

			this->DeleteItem( i, true);
		}
	}		
}

/**
 *
 */
void CSack::ShowSack()
{
	m_bRefreshToolTip = true;
	m_bShow = TRUE;

	RefreshSackPos();
}

/**
 *
 */
void CSack::DrawSack()
{
	if( !m_bShow)
		return;

	g_pDirect3DDevice->SetSamplerState( 0, D3DSAMP_MAGFILTER, D3DTEXF_POINT);
	g_pDirect3DDevice->SetSamplerState( 0, D3DSAMP_MINFILTER, D3DTEXF_POINT);

	// item 그리기
	for( int i=0; i < m_bySackTotalSize; ++i)
	{
		if( m_vecItem[i] != NULL)
		{
			DrawItem(i);
		}
	}

	// item tool tip 그리기
	for( int i=0; i < m_bySackTotalSize; ++i)
	{
		if( m_vecItem[i] != NULL)
		{
			DrawItemToolTip(i);
		}
	}
}

/**
 *
 * \param nPosition 
 */
void CSack::DrawItem( int nPosition)
{
	g_pDirect3DDevice->SetRenderState( D3DRS_ZENABLE, FALSE);
	g_pDirect3DDevice->SetRenderState( D3DRS_FOGENABLE, FALSE);

	g_pDirect3DDevice->SetRenderState( D3DRS_ALPHABLENDENABLE, TRUE);
	g_pDirect3DDevice->SetRenderState( D3DRS_DIFFUSEMATERIALSOURCE, D3DMCS_COLOR1);
	g_pDirect3DDevice->SetRenderState( D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	g_pDirect3DDevice->SetRenderState( D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
	
	g_pDirect3DDevice->SetSamplerState( 0,  D3DSAMP_ADDRESSU , D3DTADDRESS_CLAMP);
	g_pDirect3DDevice->SetSamplerState( 0,  D3DSAMP_ADDRESSV , D3DTADDRESS_CLAMP);

	g_Device.SetTexture(0, m_vecItemTex[nPosition]);
	//g_pDirect3DDevice->SetTexture( 0, m_vecItemTex[ nPosition]);
	g_Device.SetStreamSource( m_vecItemVB[ nPosition], sizeof(VT_TLVertex));
	g_Device.SetFVF(D3DFVF_TLVERTEX);
	//g_pDirect3DDevice->SetFVF( D3DFVF_TLVERTEX);
	g_pDirect3DDevice->DrawPrimitive( D3DPT_TRIANGLESTRIP, 0, 2);

	// 소켓
	XiahItem :: sItemInfo* pItem = m_vecItem[nPosition];

	if(pItem && m_vecItemSocketVB[nPosition] && pItem->m_bSocketCount)
	{
		switch(pItem->m_bSocketCount)
		{
			case 1:
				g_Device.SetTexture(0, Gettex(1364));
				//g_pDirect3DDevice->SetTexture(0, Gettex(1364));
				break;
			case 2:
				g_Device.SetTexture(0, Gettex(1365));
				//g_pDirect3DDevice->SetTexture(0, Gettex(1365));
				break;
			case 3:
				g_Device.SetTexture(0, Gettex(1366));
				//g_pDirect3DDevice->SetTexture(0, Gettex(1366));
				break;
		}
		
		g_Device.SetStreamSource( m_vecItemSocketVB[nPosition], sizeof(VT_TLVertex));
		g_pDirect3DDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);

		for(int i=0; i < 3; ++i)
		{
			if(pItem->m_bSocketItem[i])
			{
				int nResID = 0;

				// 소켓아이템의 리소스
				
				switch(pItem->m_bSocketItem[i])
				{
					case 10: nResID = 1314;	break;
					case 11: nResID = 1313; break;

					case 20: nResID = 1306;	break;
					case 21: nResID = 1305; break;

					case 30: nResID = 1312;	break;
					case 31: nResID = 1311; break;

					case 40: nResID = 1310;	break;
					case 41: nResID = 1309; break;

					case 50: nResID = 1308;	break;
					case 51: nResID = 1307; break;

					default: nResID = 0;	break;
				}

				g_Device.SetTexture(0, Gettex(nResID));
				//g_pDirect3DDevice->SetTexture(0, Gettex(nResID));

				switch(i)
				{
				case 0:
					g_Device.SetStreamSource( m_vecSocketItem1VB[nPosition], sizeof(VT_TLVertex));
					break;
				case 1:
					g_Device.SetStreamSource( m_vecSocketItem2VB[nPosition], sizeof(VT_TLVertex));
					break;
				case 2:	
					g_Device.SetStreamSource( m_vecSocketItem3VB[nPosition], sizeof(VT_TLVertex));
					break;
				}				

				g_pDirect3DDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);
			}
		} // for(int i=0; i < 3; ++i)
	}	

	//HT_1116 : 각성자 아이템 추가
	if(pItem && m_vecRBSocketItemVB[nPosition] && (pItem->m_wRBSocketItem != 0))
	{
		g_Device.SetTexture(0, Gettex(1569));
		g_Device.SetStreamSource( m_vecRBSocketItemVB[nPosition], sizeof(VT_TLVertex));
		g_pDirect3DDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);
	
		int nResID = 0;

		if(pItem->m_wRBSocketItem >= 100 && pItem->m_wRBSocketItem < 500)
			nResID = 1570;
		else if(pItem->m_wRBSocketItem >= 500 && pItem->m_wRBSocketItem < 900)
			nResID = 1571;
		else if(pItem->m_wRBSocketItem >= 900 && pItem->m_wRBSocketItem < 1100)
			nResID = 1572;
		else if(pItem->m_wRBSocketItem >= 1100 && pItem->m_wRBSocketItem < 1200)
			nResID = 1573;
		else if(pItem->m_wRBSocketItem >= 1200 && pItem->m_wRBSocketItem < 1300)
			nResID = 1574;

		if(pItem->m_wRBSocketItem > 1)
		{
			g_Device.SetTexture(0, Gettex(nResID));
			g_Device.SetStreamSource( m_vecRBItemStoneVB[nPosition], sizeof(VT_TLVertex));
			g_pDirect3DDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);
		}
	}
}

/**
 *
 * \param nPosition 
 */
void CSack::DrawItemToolTip( int nPosition)
{
	static int	nCurToolTipItemSack = -1;
	static int	nPrevToolTipItemSack = -1;

	if( m_vecItemRt[ nPosition]->PtInRect(XiahInput::g_ptMouse))
	{
		if( g_MainCharInfo.m_pHoldItem->IsHoldingItem())
		{
			m_nCurToolTipItemPos = -1;
			nCurToolTipItemSack = -1;

			return;
		}

		XiahItem::sItemInfo* pItem = m_vecItem[ nPosition];

		if(pItem)
			nCurToolTipItemSack = pItem->m_bSackID;

		m_nCurToolTipItemPos = nPosition;		

		// 얘한테는 조건 걸어서 한번만 세팅 되게 하자
		if( m_nCurToolTipItemPos != m_nPrevToolTipItemPos ||
			nCurToolTipItemSack != nPrevToolTipItemSack ||
			m_bRefreshToolTip)
		{
			m_nPrevToolTipItemPos = m_nCurToolTipItemPos;
			nPrevToolTipItemSack = nCurToolTipItemSack;
			m_bRefreshToolTip = FALSE;

			SetItemToolTip( nPosition);
		}

		g_MainCharInfo.m_pToolTip->Draw();
	}
	else
	{
		if(m_nPrevToolTipItemPos == nPosition)
			m_nPrevToolTipItemPos = -1;
	}
}


void CSack::DemandClass(XiahItem::sItemInfo* pItem)
{
	if( pItem->m_bNeedCharType)
	{
		TCHAR szTip[1024] = {0,};

		switch( pItem->m_bNeedCharType)
		{
		case 1:	// 요구유파 검영
			{
				_tcscpy( szTip, IDS_DEMAND_CLASS_GUM);
			}			
			break;
		case 2:	// 연랑
			{
				_tcscpy( szTip, IDS_DEMAND_CLASS_YUN);
			}			
			break;
		case 3:	// 무투
			{
				_tcscpy( szTip, IDS_DEMAND_CLASS_MUTU);
			}			
			break;
		case 4:	// 야차
			{
				_tcscpy( szTip, IDS_DEMAND_CLASS_YACHA);
			}			
			break;
			
		default:
			{
				return;
			}
			break;
		}

		if(g_MainCharInfo.m_bCharType == pItem->m_bNeedCharType)
		{
			g_MainCharInfo.m_pToolTip->AddToolTip( szTip);
		}
		else
		{
			g_MainCharInfo.m_pToolTip->AddToolTip( szTip, D3DCOLOR_XRGB( 255, 0, 0));
		}
	} // if( pItem->m_bNeedCharType)
}

/**
 *
 * \param pData 
 * \param nIndex 
 * \param dwColor 
 */
void CSack::ArrayText(sArrayData* pData, int nIndex, D3DCOLOR dwColor)
{
	if(pData)
	{
		sString strTemp = pData->GetString(nIndex);
		int nLen = strTemp.length();

		if (nLen >= 1024) nLen = 1023;

		TCHAR strDesc[1024] = {0,};
		memcpy(strDesc, strTemp, nLen);

		TCHAR strBuffer[1024] = {0,};
		int nCount = 0;

		for(int i=0; i < nLen; ++i)
		{
			if(strDesc[i] == '|' || strDesc[i] == '\n' || strDesc[i] == '\r')
			{
				if(nCount > 0)
				{
					nCount = 0;
					g_MainCharInfo.m_pToolTip->AddToolTip(strBuffer, 11, dwColor);
					memset(strBuffer, 0, sizeof(strBuffer));
				}
			}
			else
			{
			    if (nCount < 1023)
				    strBuffer[nCount++] = (char)strDesc[i];
			}
		}

		if(nCount > 0)
		{
			g_MainCharInfo.m_pToolTip->AddToolTip(strBuffer, 11, dwColor);
		}
	}
}

/**
 *
 * \param pData 
 */
void CSack::ToolTipArrayText(sArrayData* pData)
{
	ArrayText(pData, 0, D3DCOLOR_XRGB(204, 255, 255));

	if(pData)
	{
		sString strTemp = pData->GetString(1);

		if(stricmp(strTemp, "~") != 0)
		{
			ArrayText(pData, 1, D3DCOLOR_XRGB(0, 255, 0));
		}
	}
}

D3DCOLOR CSack::MoneyUnitColor(DWORD dwAmount)
{
	D3DCOLOR dwColor = D3DCOLOR_XRGB( 255, 255, 255);
	
    if(dwAmount >= 100000 && dwAmount < 1000000)
	{
		dwColor = D3DCOLOR_XRGB( 255, 255, 0);
	}
	else if(dwAmount >= 1000000 && dwAmount < 10000000)
	{
		dwColor = D3DCOLOR_XRGB( 0, 255, 0);
	}
	else if(dwAmount >= 10000000 && dwAmount < 100000000)
	{
		dwColor = D3DCOLOR_XRGB( 0, 204, 255);
	}
	else if(dwAmount >= 100000000 && dwAmount < 1000000000)
	{
		dwColor = D3DCOLOR_XRGB( 255, 0, 255);
	}
	else if(dwAmount >= 1000000000 && dwAmount < 10000000000)
	{
		dwColor = D3DCOLOR_XRGB( 255, 153, 0);
	}
	else
	{
		dwColor = D3DCOLOR_XRGB( 255, 255, 255);
	}
	return dwColor;
}


/**
 *
 * \param nPosition 
 */
void CSack::SetItemToolTip(int nPosition)
{
	XiahItem::sItemInfo* pItem = m_vecItem[nPosition];

	assert(pItem);
	if(!pItem)
		return;

	bool bMugongBook = false;
	bool bBongin = false;
	D3DCOLOR dwColor = D3DCOLOR_XRGB(255, 255, 255);				// m_bRarity == 0 : 보통 아이템, 흰색

	if(pItem->m_bSackID != SACKTYPE__NPC_TRADE)
	{
		// CG_2005/01/28 : 변종아이템기능추가
		// 변종아이템일경우
		// 251 : 검영변종 
		// 252 : 연랑변종
		// 253 : 무투변종
		// 254 : 야차변종
		if( pItem->m_bNeedCharType > 250 )
		{
			dwColor = D3DCOLOR_XRGB( 0, 255, 0 );
		}
		else
		{
			if( pItem->m_bRarity > 0 && pItem->m_bRarity < 128)			// rare 아이템, 금색
			{
				dwColor = D3DCOLOR_XRGB( 232, 136, 0);
			}
			else if( pItem->m_bRarity > 127 && pItem->m_bRarity < 255)	// 개조 아이템, 녹색
			{
				dwColor = D3DCOLOR_XRGB( 0, 255, 0);
			}
			else if( pItem->m_bRarity == 255)							// unique 아이템, 보라색
			{
				dwColor = D3DCOLOR_XRGB( 165, 0, 255);
			}
		}

		// TODO: 변종아이템과 겹친다
		if(pItem->m_bSocketCount)
			dwColor = D3DCOLOR_XRGB(0, 200, 255);

		if(pItem->m_bItemType == ITEMTYPE_REBUILDRES && pItem->m_bIsDividedRes >= 2)
			dwColor = D3DCOLOR_XRGB(0, 255, 0);

		// 아이템 수집
		if(pItem->m_bItemType == 18 && pItem->m_bFuncID)
		{
			if(pItem->m_bItemKind == 0)
				dwColor = D3DCOLOR_XRGB(0, 255, 0);
			else if(pItem->m_bItemKind == 1)
				dwColor = D3DCOLOR_XRGB(255, 0, 255);
		}
	}

	BYTE byToolFrameTipType = 0;
	if( pItem->m_bSackID == SACKTYPE__DEFAULT)
	{
		if( pItem->m_bSackPos % 6 == 4 ||
			pItem->m_bSackPos % 6 == 5)
			byToolFrameTipType = 5;
	}

	if( pItem->m_bSackID == SACKTYPE__EQUIPMENT)
	{
		if( pItem->m_bSackPos == EQUIPPOS_RING ||
			pItem->m_bSackPos == EQUIPPOS_CLOAK ||
			pItem->m_bSackPos == EQUIPPOS_BONGIN)
			byToolFrameTipType = 5;
	}

	// 툴팁 생성.
	g_MainCharInfo.m_pToolTip->SetToolTip( byToolFrameTipType, m_vecItemRt[nPosition], 
											1, (LPCTSTR)pItem->m_szName, dwColor, 1);


	// 약간 간격을 띄울 라인을 지정한다. 
	if( pItem->m_wLevel == 1 )	// 기연 아이템이면
		g_MainCharInfo.m_pToolTip->SetGapLine( 3 );
	else
		g_MainCharInfo.m_pToolTip->SetGapLine( 2 );

	TCHAR szTip[1024] = {0,};

#define SETITEMTOOLTIP(a,str) { if(a) { _stprintf(szTip,str,a); g_MainCharInfo.m_pToolTip->AddToolTip(szTip);}}
#define SETITEMTOOLTIP_COLOR(a,str,cond) { if(a) { _stprintf(szTip,str,a); if(cond<a) g_MainCharInfo.m_pToolTip->AddToolTip(szTip, D3DCOLOR_XRGB( 255, 0, 0)); else g_MainCharInfo.m_pToolTip->AddToolTip(szTip);}}

	// 추가적인 설명 넣기.

	// 기연 Item
	if(pItem->m_wLevel == 1 )
	{
		_stprintf( szTip, IDS_GIYEON_ITEM );
		g_MainCharInfo.m_pToolTip->AddToolTip( szTip, 11, D3DCOLOR_XRGB( 255, 255, 0) );
	}

	// PC방 아이템
	if(pItem->m_wLevel == 3)
	{
		g_MainCharInfo.m_pToolTip->AddToolTip(IDS_PCROOM_ITEM, 11, D3DCOLOR_XRGB(255, 255, 0));
	}

	// 이벤트 장착 아이템
	if(pItem->m_bItemType == ITEMTYPE_SOCKET)
	{
		g_MainCharInfo.m_pToolTip->AddToolTip(IDS_ITEM_SOCKET, 11, D3DCOLOR_XRGB( 255, 0, 0) );
	}

	// 이벤트용 아이템 - 플스, 마우스, 무림기서 
	if( pItem->m_bItemType == ITEMTYPE_EVENT && pItem->m_bItemKind != 7 && pItem->m_bItemKind != 11 )
	{
		if( pItem->m_bItemKind != 12 )
		{
			// 툴팁 만드는 코드가 잡다하다. 이벤트용은 따로 가자.
			g_MainCharInfo.m_pToolTip->AddToolTip( IDS_EVENTITEM, 11, D3DCOLOR_XRGB( 255, 0, 0) );
			g_MainCharInfo.m_pToolTip->SetGapLine( 2 );
		}
		else 
		{
			//HT_0829 : 프리미엄 퀘스트 
			g_MainCharInfo.m_pToolTip->AddToolTip( IDS_EVENTITEM_QUEST, 11, D3DCOLOR_XRGB( 255, 0, 0) );
			g_MainCharInfo.m_pToolTip->SetGapLine( 2 );
		}

		// 고대무림기서 미개봉
		if( pItem->m_wRefID == 20513)
		{
			g_MainCharInfo.m_pToolTip->AddToolTip( _T(""), 9, D3DCOLOR_XRGB(128, 128, 128) );
			g_MainCharInfo.m_pToolTip->AddToolTip( IDS_EVENTITEM_NOTOPEN, 12, D3DCOLOR_XRGB(128, 128, 128) );
			g_MainCharInfo.m_pToolTip->AddToolTip( _T(""), 9, D3DCOLOR_XRGB(128, 128, 128) );
		}
		// 개봉완료
		else if( pItem->m_wRefID >= 20508 && pItem->m_wRefID <= 20512 || (pItem->m_wRefID >= 20842 && pItem->m_wRefID <= 20845))
		{
			g_MainCharInfo.m_pToolTip->AddToolTip( _T(""), 9, D3DCOLOR_XRGB(128, 128, 128) );
			g_MainCharInfo.m_pToolTip->AddToolTip( IDS_EVENTITEM_OPEN, 12, D3DCOLOR_XRGB(255, 255, 0) );
			g_MainCharInfo.m_pToolTip->AddToolTip( _T(""), 9, D3DCOLOR_XRGB(128, 128, 128) );
		}
	} // if( pItem->m_bItemType == ITEMTYPE_EVENT )



	// 29일 패치 (수리망치,자소뿔)	
	if(pItem->m_wRefID == 20926 || pItem->m_wRefID == 20927 || pItem->m_wRefID == 20936)
	{
		g_MainCharInfo.m_pToolTip->AddToolTip( IDS_EVENTITEM, 11, D3DCOLOR_XRGB( 255, 0, 0) );
		g_MainCharInfo.m_pToolTip->SetGapLine( 2 );
	}	

	// 칠검
	switch(pItem->m_wRefID)
	{
		case 20861:
		case 20862:
		case 20863:
		case 20864:
		case 20865:
		case 20866:
		case 20867:
			{
				g_MainCharInfo.m_pToolTip->AddToolTip( IDS_7S_EVENTITEM, 11, D3DCOLOR_XRGB( 255, 0, 0) );
				g_MainCharInfo.m_pToolTip->SetGapLine( 3 );
			}
			break;
	}

	if(pItem->m_bItemType == ITEMTYPE_SUNANG)	// 낭
	{
		if(pItem->m_bItemKind == 0)
		{
			g_MainCharInfo.m_pToolTip->AddToolTip(IDS_COLLECTION_ITEM_01, 11, D3DCOLOR_XRGB(0, 0, 255));
		}
		else if(pItem->m_bItemKind == 1)
		{
			g_MainCharInfo.m_pToolTip->AddToolTip(IDS_COLLECTION_ITEM_02, 11, D3DCOLOR_XRGB(0, 0, 255));
		}
	}

	if(pItem->m_bItemType == ITEMTYPE_SURESOURCE)	// 재료
	{
		if(pItem->m_bItemKind == 0)
		{
			g_MainCharInfo.m_pToolTip->AddToolTip(IDS_COLLECTION_ITEM_03, 11, D3DCOLOR_XRGB(0, 0, 255));
		}
		else if(pItem->m_bItemKind == 1)
		{
			g_MainCharInfo.m_pToolTip->AddToolTip(IDS_COLLECTION_ITEM_04, 11, D3DCOLOR_XRGB(0, 0, 255));
		}
	}

	sArrayData* pItemTip = XiahArrayIndex::g_ItemTip.GetData(pItem->m_wRefID);

	// 아이템 설명
	ToolTipArrayText(pItemTip);
	//ArrayText(pItemTip, 0, D3DCOLOR_XRGB(204, 255, 255));

	//HO_0427_07 영수환골단
	if( pItem->m_wRefID == 21921 )
	{
		_stprintf(szTip, IDS_MONSTER_STATUS_TIP, pItem->m_wTamingLevel);
		g_MainCharInfo.m_pToolTip->AddToolTip(szTip);

	}
	//HT_0523 쥬크온 이벤트
	if( pItem->m_wRefID == 21119 || pItem->m_wRefID == 21123 ) 
	{
		g_MainCharInfo.m_pToolTip->AddToolTip( _T("1인 1매이상 사용 불가능"), 11, D3DCOLOR_XRGB(255, 0, 0) );
		g_MainCharInfo.m_pToolTip->SetGapLine( 2 );
	}

	// 아래 수치
	
	switch(pItem->m_bItemType)
	{
	case ITEMTYPE_LOTTO:
		{
			_stprintf(szTip, IDS_LOTTO_NUM, pItem->m_dwRound);
			g_MainCharInfo.m_pToolTip->AddToolTip(szTip);

			_stprintf(szTip, IDS_LOTTO_SELECT_NUM, pItem->m_bLottoNum[0], pItem->m_bLottoNum[1], pItem->m_bLottoNum[2], pItem->m_bLottoNum[3]);
			g_MainCharInfo.m_pToolTip->AddToolTip(szTip);

			switch(pItem->m_bPrizeRank)
			{
			case 0:	// 미횅땍
				{
					g_MainCharInfo.m_pToolTip->AddToolTip(IDS_LOTTO_RANK0);
				}
				break;
			case 1:	// 1~3등
			case 2:
			case 3:
				{
					_stprintf(szTip, IDS_LOTTO_RANK, pItem->m_bPrizeRank);
					g_MainCharInfo.m_pToolTip->AddToolTip(szTip);

					__int64 nTempMoney = pItem->m_dwPrizeMoney * 1000;
					_stprintf(szTip, IDS_LOTTO_PRIZEWIN_MONEY, MoneyCommaStr(nTempMoney).data());
					g_MainCharInfo.m_pToolTip->AddToolTip(szTip);
				}
				break;
			case 4:	// 꽝
				{
					g_MainCharInfo.m_pToolTip->AddToolTip(IDS_LOTTO_RANK4);
				}
				break;
			}

			return;
		}
		break;
	case ITEMTYPE_PORTAL:
		{
			if(pItem->m_bItemKind == 1)
			{
				LPCTSTR lpStrTemp = NULL;

				switch(pItem->m_dwPotalMapID)
				{
				case 0:
					{
						g_MainCharInfo.m_pToolTip->AddToolTip( IDS_NO_REMARK);

						if(pItem->m_bSackID == SACKTYPE__PERSONAL_TRADE_SET || pItem->m_bSackID == SACKTYPE__PERSONAL_TRADE_SELL)
						{
							if( g_MainCharInfo.m_pPersonalTradeSet || g_MainCharInfo.m_pPersonalTradeSell)
							{
								_stprintf( szTip, IDS_D_SELLPRICE, MoneyCommaStr(pItem->m_dwPrice).data());
								g_MainCharInfo.m_pToolTip->AddToolTip( szTip);
							}
						}
						goto TIP_Volume;
					}					
					break;
				default:
					{
						lpStrTemp = GetMapName(pItem->m_dwPotalMapID);
						break;
					}
				}

				_stprintf( szTip, _T("%s"), lpStrTemp);
				g_MainCharInfo.m_pToolTip->AddToolTip( szTip);
				_stprintf( szTip, _T("(%d,%d)"), pItem->m_wPosX/4, pItem->m_wPosY/4);
				g_MainCharInfo.m_pToolTip->AddToolTip( szTip);

				if(pItem->m_bSackID == SACKTYPE__PERSONAL_TRADE_SET || pItem->m_bSackID == SACKTYPE__PERSONAL_TRADE_SELL)
				{
					if( g_MainCharInfo.m_pPersonalTradeSet || g_MainCharInfo.m_pPersonalTradeSell)
					{
						_stprintf( szTip, IDS_D_SELLPRICE, MoneyCommaStr(pItem->m_dwPrice).data());
						g_MainCharInfo.m_pToolTip->AddToolTip( szTip);
					}
				}

				goto TIP_Volume;
			}
		}
		break;
	case ITEMTYPE_REBUILDRES:
		{
			// 상점에서는 구입할수 없기에..
			if(pItem->m_wSuccessRatio && pItem->m_bItemKind != 17)
			{
				_stprintf( szTip, IDS_SUCCESS_RATIO, pItem->m_wSuccessRatio);
				g_MainCharInfo.m_pToolTip->AddToolTip(szTip);
				
				//HT_0707 흑보 변종개조
				if(pItem->m_nResID == 50001354 && pItem->m_wSuccessRatio == 25 && pItem->m_wFactorValue > 5)
				{
					LPCTSTR lpStrVarient = NULL;
					LPCTSTR	lpStrAbilityCnt = NULL;

					lpStrVarient = IDS_VARIENT_0_0;
					g_MainCharInfo.m_pToolTip->AddToolTip(lpStrVarient, D3DCOLOR_XRGB(0, 255, 0));
					lpStrVarient = IDS_VARIENT_0_1;
					g_MainCharInfo.m_pToolTip->AddToolTip(lpStrVarient, D3DCOLOR_XRGB(0, 255, 0));

					switch(pItem->m_wFactorValue)
					{
						case 20:
						case 25:
							lpStrAbilityCnt = IDS_VARIENT_0_2;	break;	

						case 35:
						case 40:
							lpStrAbilityCnt = IDS_VARIENT_0_3;	break;	

						case 50:
						case 55:
							lpStrAbilityCnt = IDS_VARIENT_0_4;	break;
					}

					if(lpStrAbilityCnt)
						g_MainCharInfo.m_pToolTip->AddToolTip(lpStrAbilityCnt, D3DCOLOR_XRGB(0, 255, 0));
				}
	
				else if(pItem->m_bIsDividedRes >= 2)
				{
					LPCTSTR lpStrVarient = NULL;

					switch(pItem->m_bItemKind)
					{
					case 1:
						{
							switch(pItem->m_bIsDividedRes)
							{
								case 2:	lpStrVarient = IDS_VARIENT_0_0;	break;
								case 3:	lpStrVarient = IDS_VARIENT_0_1;	break;
							}

							if(lpStrVarient)
								g_MainCharInfo.m_pToolTip->AddToolTip(lpStrVarient, D3DCOLOR_XRGB(0, 255, 0));

							//HT_0707 흑보 변종개조
							if(pItem->m_nResID == 50001354 && pItem->m_wFactorValue >= 20) 
							{
								LPCTSTR	lpStrAbilityCnt = NULL;
								switch(pItem->m_wFactorValue) 
								{
									case 20:	
									case 25:	
										lpStrAbilityCnt = IDS_VARIENT_0_2;	break;	

									case 35:	
									case 40:	
										lpStrAbilityCnt = IDS_VARIENT_0_3;	break;	

									case 50:	
									case 55:	
										lpStrAbilityCnt = IDS_VARIENT_0_4;	break;
								}

								if(lpStrAbilityCnt)
									g_MainCharInfo.m_pToolTip->AddToolTip(lpStrAbilityCnt, D3DCOLOR_XRGB(0, 255, 0));
							}
						}
						break;
					case 2:
						{
							switch(pItem->m_bIsDividedRes)
							{
								case 2:	lpStrVarient = IDS_VARIENT_1_0;	break;
								case 3:	lpStrVarient = IDS_VARIENT_1_1;	break;
								case 4:	lpStrVarient = IDS_VARIENT_1_2;	break;
								case 5:	lpStrVarient = IDS_VARIENT_1_3;	break;
								case 6:	lpStrVarient = IDS_VARIENT_1_4;	break;
								case 7:	lpStrVarient = IDS_VARIENT_1_5;	break;
								case 8:	lpStrVarient = IDS_VARIENT_1_6;	break;
								case 9:	lpStrVarient = IDS_VARIENT_1_7;	break;
							}

							if(lpStrVarient)
								g_MainCharInfo.m_pToolTip->AddToolTip(lpStrVarient, D3DCOLOR_XRGB(0, 255, 0));
						}
						break;
					case 3:
						{
							switch(pItem->m_bIsDividedRes)
							{
								case 2:	lpStrVarient = IDS_VARIENT_2_0;	break;
								case 3:	lpStrVarient = IDS_VARIENT_2_1;	break;
								case 4:	lpStrVarient = IDS_VARIENT_2_2;	break;
								case 5:	lpStrVarient = IDS_VARIENT_2_3;	break;
								case 6:	lpStrVarient = IDS_VARIENT_2_4;	break;
							}

							if(lpStrVarient)
								g_MainCharInfo.m_pToolTip->AddToolTip(lpStrVarient, D3DCOLOR_XRGB(0, 255, 0));
						}
						break;
					}					
			//		g_MainCharInfo.m_pToolTip->AddToolTip(lpStrVarient, D3DCOLOR_XRGB(0, 255, 0));
				}
			}
		}
		break;
	case ITEMTYPE_BOOK:	// 무공서
		{
			if(pItem->m_wRefID >=21099 && pItem->m_wRefID <=21117)
			{
				sArrayData* pRebirthMugongDesc = XiahArrayIndex::g_RebirthMugong_Desc.GetData( pItem->m_wRefID);

				if(pRebirthMugongDesc)
				{	
					ArrayText(pRebirthMugongDesc, 0, D3DCOLOR_XRGB(204, 255, 255));

                    sString strTemp = pRebirthMugongDesc->GetString(1);
                    _stprintf( szTip, IDS_TR_ABLE_MUGONG, strTemp.data() );
                    g_MainCharInfo.m_pToolTip->AddToolTip( szTip);

					_stprintf( szTip, IDS_CON_TRAINING, pRebirthMugongDesc->GetInt(1));
					g_MainCharInfo.m_pToolTip->AddToolTip( szTip);

					if(pRebirthMugongDesc->GetInt(0) >= 21110 && pRebirthMugongDesc->GetInt(0) <= 21117)
					{
						DemandClass(pItem);
					}

					g_MainCharInfo.m_pToolTip->AddToolTip(IDS_REBIRTH_D_STEP);
					
					bMugongBook = true;
				}
			}
			//HT_0711 : 진각성 무공서
			//if(pItem->m_wRefID >=22065 && pItem->m_wRefID <=22079) || pItem->m_wRefID >=22017 && pItem->m_wRefID <=22111)
			if(pItem->m_wRefID >=22017 && pItem->m_wRefID <=22103)
			{
				sArrayData* pRebirthMugongDesc = XiahArrayIndex::g_RebirthMugong_Desc.GetData( pItem->m_wRefID);

				if(pRebirthMugongDesc)
				{	
					ArrayText(pRebirthMugongDesc, 0, D3DCOLOR_XRGB(204, 255, 255));

                    sString strTemp = pRebirthMugongDesc->GetString(1);
                    _stprintf( szTip, IDS_TR_ABLE_MUGONG, strTemp.data() );
                    g_MainCharInfo.m_pToolTip->AddToolTip( szTip);

					_stprintf( szTip, IDS_CON_TRAINING, pRebirthMugongDesc->GetInt(1));
					g_MainCharInfo.m_pToolTip->AddToolTip( szTip);

					if(pRebirthMugongDesc->GetInt(0) >= 22017 && pRebirthMugongDesc->GetInt(0) <= 22079)
					{
						DemandClass(pItem);
					}

					//타입값을 빼오고 그 값에 따른 툴팁과 요구치를 표시하면 되겟당 
					switch(pRebirthMugongDesc->GetInt(2)) //HO_0820_07 진서 요구치표시
					{
					case 0: //HT_1212 : 내공서 툴팁 수정 (요구 갑자가 2개 나온다)
						_stprintf(szTip, _T(""));
						break;						
					case 1:
						_stprintf(szTip, IDS_D_JINKI, pRebirthMugongDesc->GetInt(3));			break;
					case 2:
						_stprintf(szTip, IDS_D_MINCHUP, pRebirthMugongDesc->GetInt(3));			break;
					case 3:
						_stprintf(szTip, IDS_D_JIGU, pRebirthMugongDesc->GetInt(3));			break;
					case 4:
						_stprintf(szTip, IDS_D_GUNRYUK, pRebirthMugongDesc->GetInt(3));			break;
					} // switch(nDemandType)

					if(strlen(szTip))
						g_MainCharInfo.m_pToolTip->AddToolTip(szTip);
					
					//g_MainCharInfo.m_pToolTip->AddToolTip(IDS_2TH_REBIRTH_D_STEP);//각성차수 표시전 코드

					sString strTemp1 = pRebirthMugongDesc->GetString(2);//HO_0726_07 각성차수표시 : 사용가능 여부를 각성차수로 판단하여 툴팁 색변경 및 표시
					_stprintf( szTip, IDS_2TH_REBIRTH_D_STEP, strTemp1.data());
					if(g_MainCharInfo.m_bRebirth < pItem->m_wNeedLevel) 
						g_MainCharInfo.m_pToolTip->AddToolTip(szTip, D3DCOLOR_XRGB( 255, 0, 0)); 
					else 
						g_MainCharInfo.m_pToolTip->AddToolTip(szTip);
				
					
					
					bMugongBook = true;
				}
			}
			else
			{
				DWORD dwMugong = pItem->m_dwMugongID;
				sArrayData* pMugongDesc = XiahArrayIndex::g_MugongDesc.GetData( pItem->m_wRefID);

				if(pMugongDesc)
				{
					ArrayText(pMugongDesc, 0, D3DCOLOR_XRGB(204, 255, 255));

					sString strTemp = pMugongDesc->GetString(1);
					_stprintf( szTip, IDS_TR_ABLE_MUGONG, strTemp.data() );
					g_MainCharInfo.m_pToolTip->AddToolTip( szTip);

					if(pMugongDesc->GetInt(2) != 5)
					{
						_stprintf( szTip, IDS_CON_TRAINING, pMugongDesc->GetInt(1));
						g_MainCharInfo.m_pToolTip->AddToolTip( szTip);

						DemandClass(pItem);
					}
		
					switch(pMugongDesc->GetInt(2))
					{
					case 0: //HT_1212 : 내공서 툴팁 수정 (요구 갑자가 2개 나온다)
						{
                            if(pItem->m_bSackID == 5)
								_stprintf(szTip, IDS_D_GABJA_DEMAND, pMugongDesc->GetInt(3));
							else
								_stprintf(szTip, _T(""));
						}
						break;						
					case 1:
						_stprintf(szTip, IDS_D_JINKI, pMugongDesc->GetInt(3));				break;
					case 2:
						_stprintf(szTip, IDS_D_MINCHUP, pMugongDesc->GetInt(3));			break;
					case 3:
						_stprintf(szTip, IDS_D_JIGU, pMugongDesc->GetInt(3));				break;
					case 4:
						_stprintf(szTip, IDS_D_GUNRYUK, pMugongDesc->GetInt(3));			break;
					case 5:	// 오행 숙련도
						_stprintf(szTip, IDS_FE_POINT, pMugongDesc->GetInt(3));				break;
					} // switch(nDemandType)

					if(strlen(szTip))
						g_MainCharInfo.m_pToolTip->AddToolTip(szTip);

					bMugongBook = true;
				}
			}
		}
		break;
	case ITEMTYPE_QUEST:	// 퀘스트 아이템 툴팁
		{
			sArrayData* pQuestItem = XiahArrayIndex::g_QuestItem.GetData( pItem->m_wVisualID);

			if(pQuestItem)
			{
				g_MainCharInfo.m_pToolTip->SetGapLine( 3 );
				ArrayText(pQuestItem, 0);
				ArrayText(pQuestItem, 1, D3DCOLOR_XRGB(204, 255, 255));
			} // if(pQuestItem)

			g_MainCharInfo.m_pToolTip->AddToolTip(IDS_QUEST_ITEM_1);
			g_MainCharInfo.m_pToolTip->AddToolTip(IDS_QUEST_ITEM_2, D3DCOLOR_XRGB( 255, 0, 0));
		}
		break;
	case ITEMTYPE_BONGIN:
		{
			if(pItem->m_wLevel == 2)	// 빙정 아이템 장착 갑자
			{
				SETITEMTOOLTIP_COLOR(pItem->m_wNeedLevel, IDS_WEARING_GABJA, g_MainCharInfo.m_wLevel);
			}

			bBongin = true;

			if(pItem->m_wSoakAtkRatio)
			{
				_stprintf( szTip, IDS_SOAK_ATK_RATIO, pItem->m_wSoakAtkRatio);
				g_MainCharInfo.m_pToolTip->AddToolTip( szTip);
			} // if(pItem->m_wSoakAtkRatio)

			if(pItem->m_wSoakDefRatio)
			{
				_stprintf( szTip, IDS_SOAK_DEF_RATIO, pItem->m_wSoakDefRatio);
				g_MainCharInfo.m_pToolTip->AddToolTip( szTip);
			} // if(pItem->m_wSoakDefRatio)

			if(pItem->m_wSoakHitRatio)
			{
				_stprintf( szTip, IDS_SOAK_HIT_RATIO, pItem->m_wSoakHitRatio);
				g_MainCharInfo.m_pToolTip->AddToolTip( szTip);
			} // if(pItem->m_wSoakHitRatio)

			if(pItem->m_wSoakHPRatio)
			{
				_stprintf( szTip, IDS_SOAK_HP_RATIO, pItem->m_wSoakHPRatio);
				g_MainCharInfo.m_pToolTip->AddToolTip( szTip);
			} // if(pItem->m_wSoakHPRatio)

			SETITEMTOOLTIP(pItem->m_wAtkPwr,	IDS_SOAK_ATK);
			SETITEMTOOLTIP(pItem->m_wDefPwr,	IDS_SOAK_DEF);
			SETITEMTOOLTIP(pItem->m_wAtkRating,	IDS_SOAK_HIT);
			SETITEMTOOLTIP(pItem->m_wIncrHp,	IDS_SOAK_HP);
		}
		break;

		// [5/13/2005] 한국 이벤트 (중국 3월 31일 작업 - 중국 롤백 보상 이벤트, 경험치)
	case ITEMTYPE_EVENT:
		{
			// 경험치 물약만
			if(pItem->m_bItemKind == 6)
			{
				g_MainCharInfo.m_pToolTip->AddToolTip(IDS_EVENT_EXP);

				DWORD dwExp = 0;
				// [6/28/2005] 활령약 REF ID 교체
				switch(pItem->m_wRefID)
				{					
				case 15005:	dwExp = 5000;		break;
				case 15006:	dwExp = 10000;		break;
				case 15007:	dwExp = 50000;		break;
				case 15008:	dwExp = 100000;		break;
				case 15009:	dwExp = 1000000;	break;				
				default:
					dwExp = 0;
					break;
				}

				_stprintf(szTip, _T("%s"), MoneyCommaStr(dwExp).data());
				g_MainCharInfo.m_pToolTip->AddToolTip( szTip);
			}
			if( pItem->m_bItemKind == 12 )
			{
				g_MainCharInfo.m_pToolTip->AddToolTip(IDS_MOUSE_LEFT, D3DCOLOR_XRGB( 255, 0, 0));
			}
		}
		break;
	case ITEMTYPE_REBIRTH:
		{	
			//HT_0711 : 진각성 무공 (진각성 신단은 인덱스에 표기했음)
			if(!(pItem->m_wRefID >= 22112 && pItem->m_wRefID <= 22117))
			{
				if(pItem->m_bStepID == 0)
				{
					_stprintf( szTip, IDS_REBIRTH_ITEM_1, pItem->m_wRebirthNeedLevel);
					g_MainCharInfo.m_pToolTip->AddToolTip( szTip,D3DCOLOR_XRGB( 255, 255, 255));
					g_MainCharInfo.m_pToolTip->AddToolTip( _T(""));				
				}
				else
				{
					_stprintf( szTip, IDS_REBIRTH_ITEM_2, pItem->m_wRebirthNeedLevel, pItem->m_bStepID);
					g_MainCharInfo.m_pToolTip->AddToolTip( szTip,D3DCOLOR_XRGB( 255, 255, 255));
					g_MainCharInfo.m_pToolTip->AddToolTip( _T(""));			
				}	
			}
			g_MainCharInfo.m_pToolTip->AddToolTip(IDS_MOUSE_LEFT, D3DCOLOR_XRGB( 255, 0, 0));	
		}
		break;
	case ITEMTYPE_PREMIUMQUEST:
		{
			_stprintf( szTip, IDS_D_GABJA_DEMAND, pItem->m_bLimitCnt);
			g_MainCharInfo.m_pToolTip->AddToolTip( szTip,D3DCOLOR_XRGB( 255, 255, 255));

			g_MainCharInfo.m_pToolTip->AddToolTip(IDS_MOUSE_LEFT, D3DCOLOR_XRGB( 255, 0, 0));
		}
		break;
	
	//HT_0406 : 환배 시스템 추가
	case ITEMTYPE_POTION:
		{
			if(pItem->m_bItemKind == ITEMKIND_POTION_HWANBEA)
			{
				if(pItem->m_bMaxLevel)
				{
					_stprintf( szTip, IDS_HWANBEA_LIMITLEVEL1, pItem->m_bMinLevel, pItem->m_bMaxLevel);
					g_MainCharInfo.m_pToolTip->AddToolTip( szTip);
				}
				else
				{
					_stprintf( szTip, IDS_HWANBEA_LIMITLEVEL2, pItem->m_bMinLevel);
					g_MainCharInfo.m_pToolTip->AddToolTip( szTip);
				}

				_stprintf( szTip, IDS_HWANBEA_INC, pItem->m_wIncrIp);
				g_MainCharInfo.m_pToolTip->AddToolTip( szTip);

				BYTE TempTime = (pItem->m_dwKeepUpTime / 60000);
				_stprintf( szTip, IDS_HWANBEA_KEEPUPTIME, TempTime);
				g_MainCharInfo.m_pToolTip->AddToolTip( szTip);
			}
		}
		break;
	
	default:
		break;
	}

	SETITEMTOOLTIP(pItem->m_bWildRate, IDS_WILD_DEC);

	// 몰 아이템은 요구갑자 미출력을 왜 했을까.. 기억이
	if(pItem->m_bItemType != ITEMTYPE_BONGIN && pItem->m_wLevel != 2 && !(pItem->m_wRefID >=21099 && pItem->m_wRefID <=21117)
		&& !(pItem->m_wRefID >=22017 && pItem->m_wRefID <= 22103))	// 일반 요구 갑자
	{
		//SETITEMTOOLTIP_COLOR(pItem->m_wNeedLevel, IDS_D_GABJA_DEMAND, g_MainCharInfo.m_wLevel);
		if(pItem->m_wNeedLevel) 
		{ 
			_stprintf(szTip,IDS_D_GABJA_DEMAND,pItem->m_wNeedLevel); 
			if(g_MainCharInfo.m_wLevel < pItem->m_wNeedLevel) 
				g_MainCharInfo.m_pToolTip->AddToolTip(szTip, D3DCOLOR_XRGB( 255, 0, 0)); 
			else 
				g_MainCharInfo.m_pToolTip->AddToolTip(szTip);
		}
	}
	else if(pItem->m_wVisualID >= 105 && pItem->m_wVisualID <= 110)
	{
		SETITEMTOOLTIP_COLOR(pItem->m_wNeedLevel, IDS_D_GABJA_DEMAND, g_MainCharInfo.m_wLevel);
	}

	// CG_2005/01/28 변종 아이템
	if( pItem->m_bNeedCharType < 250 )
	{
		SETITEMTOOLTIP_COLOR(pItem->m_wNeedDex, IDS_D_MINCHUP,	g_MainCharInfo.m_wDex);
		SETITEMTOOLTIP_COLOR(pItem->m_wNeedStr, IDS_D_GUNRYUK,	g_MainCharInfo.m_wStr);
		SETITEMTOOLTIP_COLOR(pItem->m_wNeedSus, IDS_D_JIGU,		g_MainCharInfo.m_wSus);
		SETITEMTOOLTIP_COLOR(pItem->m_wNeedVit, IDS_D_JINKI,	g_MainCharInfo.m_wVit);
	}	

	if(pItem->m_bItemType != ITEMTYPE_BONGIN && pItem->m_bItemType != ITEMTYPE_SOCKET)
	{
		SETITEMTOOLTIP(pItem->m_wAtkPwr,	IDS_D_PWR);
		SETITEMTOOLTIP(pItem->m_wDefPwr,	IDS_D_DEF);
		SETITEMTOOLTIP(pItem->m_wAtkRating, IDS_D_AGI);
	}

	// 수리비 문구
	// [6/27/2005] 아이템몰 연장 (수리비 없음)
	if( g_CursorType == eCT_Repair && pItem->m_wMaxDur && !g_MainCharInfo.m_bReairItemUse2)
	{
		//DWORD Amount = pItem->m_dwPrice / 3 * (float)( (float)(pItem->m_wMaxDur - pItem->m_wCurDur) / (float)pItem->m_wMaxDur);
		DWORD Amount = pItem->m_dwPrice * 0.1 * ( (float)(pItem->m_wMaxDur - pItem->m_wCurDur) / (float)pItem->m_wMaxDur );

		// 황금연장은 수리비 50%로 변경
		if(pItem->m_wRefID == 20601)
		{
			Amount /= 2;
		}

		// 성인서버일때
		if(g_AppData.m_bAdult)
		{
			float fBadExtPrice = 0.0f;

			if(g_MainCharInfo.m_dwFame > 127)
			{
				if(g_MainCharInfo.m_dwFame >= 128 && g_MainCharInfo.m_dwFame <= 132)		// 선인 1단계
					fBadExtPrice = 5.0f;					
				else if(g_MainCharInfo.m_dwFame >= 133 && g_MainCharInfo.m_dwFame <= 226)	// 선인 2단계
					fBadExtPrice = 10.0f;
				else if(g_MainCharInfo.m_dwFame >= 227)										// 선인 3단계
					fBadExtPrice = 20.0f;

				Amount = Amount - (Amount * (fBadExtPrice / 100));								
			}		
			else if(g_MainCharInfo.m_dwFame < 127)
			{
				if(127 - g_MainCharInfo.m_dwFame < 6)
					fBadExtPrice = 10.0f;
				else if(127 - g_MainCharInfo.m_dwFame < 100)
					fBadExtPrice = 20.0f;
				else
					fBadExtPrice = 40.0f;

				Amount = Amount + (Amount * (fBadExtPrice / 100));							
			}			
		}
		
//		Amount = Amount + (Amount * ( * 0.01));				//백화수정 수리비 할인 개조
		
		_stprintf( szTip, IDS_D_SURI, MoneyCommaStr(Amount).data());

		// 수리가능시
		if( Amount > 0 && !g_MainCharInfo.m_bReairItemUse)
		{			
			XiahObject::CXiahObject* pObject = XiahObject::g_XiahObjectManager.FindXiahObject( MAKEOBJECTID( 0, g_MainCharInfo.m_dwPickedObject, OBJTYPE_FUNCTIONALNPC));

			if( pObject == NULL ) return;
			if( pObject->m_pObject == NULL ) return;

			CXiahCharObject* pCharObject = reinterpret_cast<CXiahCharObject*>( pObject->m_pObject);

			//YS_0811 : BUGFIX
			if ( !pCharObject ) 
				return ;

			sFunctionalNpcInfo* pInfo = (sFunctionalNpcInfo*)pCharObject->m_pPrivateData;

			//YS_0811 : BUGFIX
			if ( !pInfo ) 
				return ;

			bool bFlag = FALSE;
			switch( pInfo->m_bType)
			{
			case 1: //대장장이
				{
					if( pItem->m_bItemType != ITEMTYPE_WEAPON)
						bFlag = TRUE;
				}				
				break;
			case 2:	//의류상인
				{
					if( pItem->m_bItemType != ITEMTYPE_CLOTH)
						bFlag = TRUE;
				}				
				break;		
			case 3: //잡화상인
				{
					if( !(pItem->m_bItemType == ITEMTYPE_SHOE || pItem->m_bItemType == ITEMTYPE_HAT))
						bFlag = TRUE;
				}				
				break;
			case 4:	//보석상인
				{
					if( !(pItem->m_bItemType == ITEMTYPE_RING || pItem->m_bItemType == ITEMTYPE_NECKLACE))
						bFlag = TRUE;
				}				
				break;
			} // switch( pInfo->m_bType)

			if( g_MainCharInfo.m_dwMoney < Amount || bFlag)
			{
				g_MainCharInfo.m_pToolTip->AddToolTip( szTip, D3DCOLOR_XRGB( 255, 0, 0));
			}
			else
			{
				g_MainCharInfo.m_pToolTip->AddToolTip( szTip,MoneyUnitColor(Amount));
			}
		} // if( Amount > 0 && !g_MainCharInfo.m_bReairItemUse)
		else
		{
			if(g_MainCharInfo.m_bReairItemUse && Amount > 0)
			{
				g_MainCharInfo.m_pToolTip->AddToolTip( szTip,MoneyUnitColor(Amount));
			}
		}
	}

	// 이동/공격 속도
	if( pItem->m_wStkSpeed)
	{
		switch( pItem->m_bItemType)
		{
		case ITEMTYPE_SHOE:
			{
				_stprintf( szTip, IDS_D_MOVE_SPEED, pItem->m_wStkSpeed);

				if(pItem->m_bPuzzleType == 6)
				{
					g_MainCharInfo.m_pToolTip->AddToolTip(szTip, 12, D3DCOLOR_XRGB(0, 255, 0));
				}
				else
				{					
					g_MainCharInfo.m_pToolTip->AddToolTip( szTip);
				}				
			}			
			break;
		default:
			{
				_stprintf( szTip, IDS_D_ATTK_SPEED, pItem->m_wStkSpeed);
				g_MainCharInfo.m_pToolTip->AddToolTip( szTip);
			}			
			break;
		}
	}

	SETITEMTOOLTIP(pItem->m_wDecrSpeed,	IDS_D_MOVE_SPEED_DEC);
	SETITEMTOOLTIP(pItem->m_wAtkRange,	IDS_D_ATTK_DIST);


	// 생명력
	if( pItem->m_wIncrHp && !bBongin)// && (pItem->m_bItemKind != ITEMKIND_POTION_HWANBEA && pItem->m_bItemType == ITEMTYPE_POTION))
	{
		if( pItem->m_bItemType ==ITEMTYPE_POTION || pItem->m_bItemType==ITEMTYPE_NPCITEM)
		{
			if(pItem->m_bItemKind == ITEMKIND_POTION_PERCENT)
				_stprintf( szTip, IDS_D_LIFE_RECO_RATIO, pItem->m_wIncrHp);
			else
				_stprintf( szTip, IDS_D_LIFE_RECO, pItem->m_wIncrHp);
		}
		else if(pItem->m_bItemType != ITEMTYPE_SOCKET)
		{
			_stprintf( szTip, IDS_D_MAX_LIFE_INC, pItem->m_wIncrHp);
		}
		

		if(pItem->m_bItemType != ITEMTYPE_SOCKET)
			g_MainCharInfo.m_pToolTip->AddToolTip( szTip);
	} // if( pItem->m_wIncrHp)

	// 내력
	if( pItem->m_wIncrIp )//&& (pItem->m_bItemKind != ITEMKIND_POTION_HWANBEA && pItem->m_bItemType == ITEMTYPE_POTION))
	{
		if( pItem->m_bItemType ==ITEMTYPE_POTION)
		{
			if(pItem->m_bItemKind == ITEMKIND_POTION_PERCENT)
				_stprintf( szTip, IDS_D_INLIFE_REC_RATIO, pItem->m_wIncrIp);
			else
				_stprintf( szTip, IDS_D_INLIFE_REC, pItem->m_wIncrIp);
		}
		else
		{
			_stprintf( szTip,IDS_D_MAX_INLIFE_INC, pItem->m_wIncrIp);
		}

		g_MainCharInfo.m_pToolTip->AddToolTip( szTip);
	}

	SETITEMTOOLTIP(pItem->m_wRestoreHp, IDS_D_AUTO_LIFE_REC);
	SETITEMTOOLTIP(pItem->m_wRestoreIp, IDS_D_AUTO_INLIFE_REC);

	// 일격술 확률 증가
	if(pItem->m_bItemType != ITEMTYPE_SOCKET)
		SETITEMTOOLTIP(pItem->m_wIncrCritical,IDS_D_CRITICAL_INC);

	if(pItem->m_bNeedCharType && !bMugongBook)	// 요구유파
	{
		DemandClass(pItem);
	}

	// 수집 낭
	if(pItem->m_bItemType == ITEMTYPE_SUNANG || pItem->m_bItemType == ITEMTYPE_SURESOURCE)
	{
		switch(pItem->m_bFuncID)
		{
		case 1:	// 공격력
			_stprintf(szTip, IDS_D_PWR, pItem->m_dwValue);
			break;
		case 2:	// 방어력
			_stprintf(szTip, IDS_D_DEF, pItem->m_dwValue);
			break;
		case 3:	// 최대 생명력
			_stprintf(szTip, IDS_D_MAX_LIFE_INC, pItem->m_dwValue);
			break;
		case 4:	// 정확도
			_stprintf(szTip, IDS_D_AGI, pItem->m_dwValue);
			break;
		case 5:	// 최대 내력 증가
			_stprintf(szTip, IDS_D_MAX_INLIFE_INC, pItem->m_dwValue);
			break;
		case 6:	// 일격술 확률 증가
			_stprintf(szTip, IDS_D_CRITICAL_INC, pItem->m_dwValue);
			break;
		case 7:	// 자동 내력 회복
			_stprintf(szTip, IDS_D_AUTO_INLIFE_REC, pItem->m_dwValue);
			break;
		case 8:	// 자동 생명력 회복
			_stprintf(szTip, IDS_D_AUTO_LIFE_REC, pItem->m_dwValue);
			break;
		}

		g_MainCharInfo.m_pToolTip->AddToolTip(szTip);
	}

	if(pItem->m_wCurDur)	// 현재내구
	{
		switch(pItem->m_wFunctionItem)
		{
		case 0:	// 사용기간
			_stprintf( szTip, IDS_USABLE, pItem->m_wCurDur, pItem->m_wMaxDur);
			break;
		case 1:	// 사용횟수
		case 2:
		case 3:
		case 4:
		case 5:
		case 9:	// 사용횟수 매품패			
			_stprintf( szTip, IDS_USE_FREQUENCY, pItem->m_wCurDur, pItem->m_wMaxDur);
			break;
		case 6:	// 잔여 전낭(대)
		case 11:// 잔여 //HO_0918_07 초보자용 전낭 추가 : 전낭(소)
			_stprintf( szTip, IDS_PURSE_USE, pItem->m_wCurDur);
			break;
		case 7:	// 남은 전송 횟수
			_stprintf( szTip, IDS_REMAIN_SEND, pItem->m_wCurDur);
			break;
		default:
			{
				if(pItem->m_bItemType == ITEMTYPE_SOCKET)
					_stprintf(szTip, IDS_USABLE_TIME, pItem->m_wCurDur, pItem->m_wMaxDur);		// 사용시간
				else
				{
					_stprintf(szTip, IDS_D_DUR, pItem->m_wCurDur, pItem->m_wMaxDur);			// 내구력

					// [4/24/2006] 내구도 20% 이하 일때 빨간색 툴팁배경으로 교체
					if((pItem->m_bItemType >= 1 && pItem->m_bItemType <= 9) ||
						(pItem->m_bItemType >= 11 && pItem->m_bItemType <= 15) ||
						pItem->m_bItemType == 17 || pItem->m_bItemType == 18 ||
						pItem->m_bItemType == 32)
					{
						WORD wDur = pItem->m_wCurDur * 100 / pItem->m_wMaxDur;

						if(wDur <= 20)
						{
							g_MainCharInfo.m_pToolTip->SetFrameColor(D3DCOLOR_ARGB(120, 140, 0, 0));
						}
					}					
				}
			}
			break;
		}

		g_MainCharInfo.m_pToolTip->AddToolTip(szTip);
	}

	if(g_MainCharInfo.m_pDepositSack)
	{
		// 창고지기와 거래할때 인벤토리에 아이템 가격 나타내기
		DWORD Amount = (pItem->m_dwPrice / 100) * pItem->m_dwAmount;

		if( Amount < 1 )
			Amount = 1;

		_stprintf( szTip, IDS_D_BOGUAN, MoneyCommaStr(Amount).data());

		if( g_MainCharInfo.m_dwMoney < Amount)
			g_MainCharInfo.m_pToolTip->AddToolTip( szTip, D3DCOLOR_XRGB( 255, 0, 0));
		else
			g_MainCharInfo.m_pToolTip->AddToolTip( szTip);
	} // if( g_MainCharInfo.m_pDepositSack)

	// 설승단약
	if(pItem->m_wRefID == 20847)
	{
		SETITEMTOOLTIP(1, IDS_USABLE_TIME2);
	}
	else if(pItem->m_wRefID == 20848)
	{
		SETITEMTOOLTIP(3, IDS_USABLE_TIME2);
	}


TIP_Volume:

	switch(pItem->m_bSackID)
	{
	case SACKTYPE__NPC_TRADE:			// NPC 판매가
		{			
			if(pItem->m_dwPrice)
			{
				_stprintf(szTip, IDS_D_PRICE, MoneyCommaStr(pItem->m_dwPrice).data());

				if(g_MainCharInfo.m_dwMoney < pItem->m_dwPrice)
					g_MainCharInfo.m_pToolTip->AddToolTip(szTip, D3DCOLOR_XRGB(255, 0, 0));
				else
					g_MainCharInfo.m_pToolTip->AddToolTip(szTip, MoneyUnitColor(pItem->m_dwPrice));
			}
		}
		break;
	case SACKTYPE__PERSONAL_TRADE_SET:	// 개인상점시 판매가
	case SACKTYPE__PERSONAL_TRADE_SELL:
		{
			if( g_MainCharInfo.m_pPersonalTradeSet || g_MainCharInfo.m_pPersonalTradeSell)
			{
				_stprintf( szTip, IDS_D_SELLPRICE, MoneyCommaStr(pItem->m_dwPrice).data());
				g_MainCharInfo.m_pToolTip->AddToolTip( szTip, MoneyUnitColor(pItem->m_dwPrice));
			}
		}
		break;
	case SACKTYPE__DEFAULT:				// NPC와 거래시
		{
			// 매품패
			if(pItem->m_dwPrice && (g_MainCharInfo.m_pNpcSack || g_MainCharInfo.m_pQuickMart))
			{
				// 내구도를 적용하는 경우
				//Amount = (float)pItem->m_dwPrice * 2 / 5.0 * pItem->m_dwAmount * (float)((float)pItem->m_wCurDur / (float)pItem->m_wMaxDur);
				DWORD dwAmount = 0;

				// 금괴
				if(pItem->m_bItemType == ITEMTYPE_MONEY && pItem->m_bItemKind == 1)
					dwAmount = pItem->m_dwPrice;
				else
				{
					if((pItem->m_bItemType >= 1 && pItem->m_bItemType <= 9) ||
					   (pItem->m_bItemType >= 11 && pItem->m_bItemType <= 15) ||
					   pItem->m_bItemType == 17 || pItem->m_bItemType == 18 ||
					   pItem->m_bItemType == 32)
					{
						//HT_0719 : 판매가 안전장치
						if ( pItem->m_wCurDur > pItem->m_wMaxDur )
							pItem->m_wCurDur = pItem->m_wMaxDur;

						// [4/24/2006] NPC에게 판매하는 공식 변경
						float fPrice = (float)pItem->m_dwPrice * 0.4f * ((float)pItem->m_wCurDur / (float)pItem->m_wMaxDur);

						dwAmount = (DWORD)fPrice;
					}
					//else if(pItem->m_bItemType == 25) //HT_1026 : 개조석 판매 이벤트 
					//{
					//	switch (pItem->m_wRefID)
					//	{
					//	case 20268:		dwAmount = 1500000;		break;	//흑보
					//	case 20269:		dwAmount = 1000000;		break;	//소보
					//	case 20270:		dwAmount = 220000;		break;	//혈보
					//	case 20108:		dwAmount = 200000;		break;	//흑정
					//	case 20109:		dwAmount = 300000;		break;	//소정
					//	case 20189:		dwAmount = 150000;		break;	//혈정
					//	case 20265:		dwAmount = 100000;		break;	//흑편
					//	case 20266:		dwAmount = 150000;		break;	//소편
					//	case 20267:		dwAmount = 80000;		break;	//혈편
					//	default:
					//		{
					//			dwAmount = (float)pItem->m_dwPrice * 2 / 5.0 * pItem->m_dwAmount;
					//			break;
					//		}
					//	}
					//}
					else
					{
						dwAmount = (float)pItem->m_dwPrice * 2 / 5.0 * pItem->m_dwAmount;
					}
				}
				
				_stprintf(szTip, IDS_D_SELLPRICE, MoneyCommaStr(dwAmount).data());
				g_MainCharInfo.m_pToolTip->AddToolTip( szTip,MoneyUnitColor(dwAmount));
			}
		}
		break;
	}

	// 수량
	if( pItem->m_dwAmount > 1)
	{
		if(pItem->m_bItemType == ITEMTYPE_EVENT && 5 == pItem->m_bItemKind)
		{
			if(pItem->m_wRefID == 20672)
			{
				_stprintf(szTip, IDS_RICECAKE_LENGTH, pItem->m_dwAmount);
				g_MainCharInfo.m_pToolTip->AddToolTip(szTip);
			}
			else if(pItem->m_wRefID == 21922)//HO_0430_07 사랑의 하트
			{
				_stprintf(szTip, IDS_HEART_LENGTH, pItem->m_dwAmount);
				g_MainCharInfo.m_pToolTip->AddToolTip(szTip);
			}
			else if(pItem->m_wRefID == 22118)//HO_0706_07 얼음 이벤트 : 처음에는 가래떡만 있었다.. 그다음 사랑의 하트 그리고 얼음.. 더 추가되면 .. 스위치로 간다!!
			{
				_stprintf(szTip, IDS_ICE_LENGTH, pItem->m_dwAmount);
				g_MainCharInfo.m_pToolTip->AddToolTip(szTip);
			}
		}
		else
		{
			_stprintf(szTip, IDS_D_VOLUME, pItem->m_dwAmount);
			g_MainCharInfo.m_pToolTip->AddToolTip( szTip);
		}		
	} // if( pItem->m_dwAmount > 1)

	if( pItem->m_dwNpcID)
	{
		if( pItem->m_bItemType == ITEMTYPE_BONGIN)
		{
			g_MainCharInfo.m_pToolTip->AddToolTip( IDS_PET_BONGIN);
		}
	} // if( pItem->m_dwNpcID)

	// 아이템몰 전낭
	if(pItem->m_wFunctionItem == 6 || pItem->m_wFunctionItem == 11 )//HO_0918_07 초보자용 전낭 추가 : 전낭(소) pItem->m_wFunctionItem == 11
	{
		_stprintf(szTip, IDS_PURSE_IN_DES, MoneyCommaStr(pItem->m_dwValue).data());
		g_MainCharInfo.m_pToolTip->AddToolTip( szTip,MoneyUnitColor(pItem->m_dwValue));
	}


	if(pItem->m_bItemType == ITEMTYPE_BONGIN && (pItem->m_wLevel == 2 || pItem->m_wLevel == 3))	// 빙정일시 봉인된 수
	{
		SETITEMTOOLTIP(pItem->m_bModifyCnt, IDS_BONGIN_COUNT);
	}
	else if(pItem->m_bItemType == 18 && pItem->m_bModifyCnt)			// 수집 낭
	{
		SETITEMTOOLTIP(pItem->m_bModifyCnt, IDS_MIXITEM);
	}
	else if(pItem->m_bItemType != ITEMTYPE_BONGIN && pItem->m_bItemType != 18)	// 일반 개조 횟수
	{
		SETITEMTOOLTIP(pItem->m_bModifyCnt, IDS_D_CONVERT_TRY);
	}



	// 오행 소켓
	if(pItem->m_bSocketCount)
	{
		// 기공수
		_stprintf(szTip, IDS_FE_SOCKET, pItem->m_bSocketCount);		
		g_MainCharInfo.m_pToolTip->AddToolTip(szTip);

		BYTE bFEData[3];
		BYTE bCumulate[3];
		ZeroMemory(bFEData, sizeof(BYTE)*3);
		ZeroMemory(bCumulate, sizeof(BYTE)*3);

		bool bFE = false;
		// 제련된 정보
		
		// 기공에 박힌 종류를 분류
		for(BYTE i=0; i < pItem->m_bSocketCount; ++i)
		{
			if(pItem->m_bSocketItem[i])
			{
				bFE = true;

				// 기공에 박힌 종류
				BYTE bSocketItem = pItem->m_bSocketItem[i] / 10;				

				bool bExist = false;
				for(int j=0; j < 3; ++j)
				{
					// 분류된 데이터 종류
					BYTE bFESocket = bFEData[j] / 10;

					// 같은 종류만
					if(bFESocket == bSocketItem)
					{
						// 정/페 구분
						BYTE bRest = pItem->m_bSocketItem[i] % 10;

						// 누적 공식이 바뀌었다. 그냥 그대로 둘련다
						//bFEData[j] += ++bRest;
						++bRest;

						// 누적 적용 1단위 정, 10단위 패
						if(bRest == 1)
							bCumulate[j] += 1;
						else if(bRest == 2)
							bCumulate[j] += 10;

						bExist = true;

						break;
					}
				} // for(int j=0; j < 3; ++j)

				// 같은 종류 없을시
				if(!bExist)
				{
					for(int x=0; x < 3; ++x)
					{
						if(!bFEData[x])
						{
							bFEData[x] = pItem->m_bSocketItem[i];

							BYTE bRest = pItem->m_bSocketItem[i] % 10;
							++bRest;
							if(bRest == 1)
								bCumulate[x] += 1;
							else if(bRest == 2)
								bCumulate[x] += 10;

							break;
						}
					} // for(int x=0; x < 3; ++x)
				} // if(!bExist)
			} // if(pItem->m_bSocketItem[i])
		}

		ZeroMemory(szTip, strlen(szTip));
		for(int j=0; j < 3; ++j)
		{
			if(bFEData[j])
			{		
				LPCTSTR lpStrTemp = NULL;

				switch(bFEData[j] / 10)
				{
					case 1:	// 화
						lpStrTemp = IDS_FE_1;		break;
					case 2:	// 수
						lpStrTemp = IDS_FE_2;		break;
					case 3:	// 목
						lpStrTemp = IDS_FE_3;		break;
					case 4:	// 금
						lpStrTemp = IDS_FE_4;		break;
					case 5:	// 토
						lpStrTemp = IDS_FE_5;		break;
					default:
						continue;
						break;
				}

				// 누적 수치 계산
				BYTE bCumulateSum = 0;
				BYTE bTemp = (bCumulate[j] / 10);
				// [4/13/2005] 아니 12월에 이리 저리 공식을 하더만 횅땍을 하고는 이제 와서				
				// 패 (10단위)
				if(bTemp)
				{
					bCumulateSum += ((bTemp-1) * 2 + 10);

					// 정 (1단위)
					bTemp = (bCumulate[j] % 10);
					bCumulateSum += bTemp;
				}
				else
				{
					// 정 (1단위)
					bTemp = (bCumulate[j] % 10);
					if(bTemp)
						bCumulateSum += ((bTemp-1) * 1 + 5);
				}				

				TCHAR strFETemp[32] = {0,};

				_stprintf(strFETemp, lpStrTemp, bCumulateSum);
				_stprintf(szTip, _T("%s %s"), szTip, strFETemp);
			}
		}

		if(bFE)
			g_MainCharInfo.m_pToolTip->AddToolTip(szTip);
	}


	//HT_1116 : 각성자 아이템 추가
	if(pItem->m_wRBSocketItem)
	{
		// 각성자 아이템 
		_stprintf(szTip, IDS_REBIRTHITEM);	
		g_MainCharInfo.m_pToolTip->AddToolTip(szTip, 12, D3DCOLOR_XRGB(90, 255, 90));

		if(pItem->m_wRBSocketItem > 1)
		{
			LPCTSTR lpstr = NULL;
			LPCTSTR lpstr2 = NULL;

			if(pItem->m_wRBSocketItem >= 100 && pItem->m_wRBSocketItem < 200)
			{
				lpstr = _T("납매기"); 
				lpstr2 = _T("공격력 증가 : %d");
			}
			else if(pItem->m_wRBSocketItem >= 200 && pItem->m_wRBSocketItem < 300)
			{
				lpstr = _T("담향기"); 
				lpstr2 = _T("방어력 증가 : %d");
			}
			else if(pItem->m_wRBSocketItem >= 300 && pItem->m_wRBSocketItem < 400)
			{
				lpstr = _T("매화기"); 
				lpstr2 = _T("정확도 증가 : %d");
			}
			else if(pItem->m_wRBSocketItem >= 400 && pItem->m_wRBSocketItem < 500)
			{
				lpstr = _T("부용기"); 
				lpstr2 = _T("생명력 증가 : %d");
			}
			else if(pItem->m_wRBSocketItem >= 500 && pItem->m_wRBSocketItem < 600)
			{	
				lpstr = _T("월진루"); 
				lpstr2 = _T("내력 증가 : %d");
			}
			else if(pItem->m_wRBSocketItem >= 600 && pItem->m_wRBSocketItem < 700)
			{
				lpstr = _T("설유루"); 
				lpstr2 = _T("자동생명회복 증가 : %d");
			}
			else if(pItem->m_wRBSocketItem >= 700 && pItem->m_wRBSocketItem < 800)
			{
				lpstr = _T("타묘루"); 
				lpstr2 = _T("자동내력회복 증가 : %d");
			}
			else if(pItem->m_wRBSocketItem >= 800 && pItem->m_wRBSocketItem < 900)
			{
				lpstr = _T("결오루"); 
				lpstr2 = _T("일격술 증가 : %d");
			}
			else if(pItem->m_wRBSocketItem >= 900 && pItem->m_wRBSocketItem < 1000)
			{
				lpstr = _T("천잠금사"); 
				lpstr2 = _T("아이템내구도 증가 : %d(%%)");
			}
			else if(pItem->m_wRBSocketItem >= 1000 && pItem->m_wRBSocketItem < 1100)
			{
				lpstr = _T("천잠은사"); 
				lpstr2 = _T("착용제한 갑자하락 : %d");
			}
			else if(pItem->m_wRBSocketItem == 1100)
				lpstr = _T("청마패");
			else if(pItem->m_wRBSocketItem == 1101)
				lpstr = _T("적령패");
			else if(pItem->m_wRBSocketItem == 1102)
				lpstr = _T("백귀패");
			else if(pItem->m_wRBSocketItem == 1103)
				lpstr = _T("흑살패");
			else if(pItem->m_wRBSocketItem == 1200) 
			{
				lpstr = _T("희잠");
				lpstr2 = _T("흑개조성공시추가증가값:%d");
			}
			else if(pItem->m_wRBSocketItem == 1201)
			{
				lpstr = _T("수잠");
				lpstr2 = _T("흑개조성공시추가증가값:%d");
			}
			//else 
			//	lpstr = NULL; lpstr2 = NULL;
				
			TCHAR strMsg[1024] = {0,};
			TCHAR strMsg2[1024] = {0,};
			
			_stprintf(strMsg, lpstr);
			g_MainCharInfo.m_pToolTip->AddToolTip(strMsg, 12, D3DCOLOR_XRGB(90, 255, 90));
			if(lpstr2 != NULL)
			{
				_stprintf(strMsg2, lpstr2, pItem->m_wRebuithValue);
				g_MainCharInfo.m_pToolTip->AddToolTip(strMsg2, 12, D3DCOLOR_XRGB(90, 255, 90));
			}
		}
	}

	// 조합된 아이템 타입
	if(pItem->m_bPuzzleType)
	{
		LPCTSTR lpStrTemp = NULL;

		switch(pItem->m_bPuzzleType)
		{
			case 1:	// 요구 갑자 조합
				lpStrTemp = IDS_MIXTURE_ITEM1;
				break;
			case 2:	// 근력
				lpStrTemp = IDS_MIXTURE_ITEM2;
				break;
			case 3:	// 민첩
				lpStrTemp = IDS_MIXTURE_ITEM3;
				break;
			case 4:	// 지구력
				lpStrTemp = IDS_MIXTURE_ITEM4;
				break;
			case 5:	// 아이템 내구도
				lpStrTemp = IDS_MIXTURE_ITEM5;
				break;
			case 6:	// 이동속도 증가 조합
				lpStrTemp = IDS_MIXTURE_ITEM6;
				break;
			default:
				lpStrTemp = _T("?");
				break;
		}

		g_MainCharInfo.m_pToolTip->AddToolTip(lpStrTemp, 12, D3DCOLOR_XRGB(0, 255, 0));
	}

	// CG_2005/01/28 : 변종아이템기능추가
	// 변종아이템일경우 수리잔여회수 표시
	if( pItem->m_bNeedCharType > 250 )
	{
		_stprintf( szTip, IDS_D_REPAIRCOUNT, pItem->m_bRepairCnt );
		g_MainCharInfo.m_pToolTip->AddToolTip( szTip, D3DCOLOR_XRGB( 0, 255, 0 ));
	}

	// 보험 아이템
	if(pItem->m_wLevel == 4)
	{
		// 복구가능
		g_MainCharInfo.m_pToolTip->AddToolTip(IDS_RECOVERY_ABLE, D3DCOLOR_XRGB(250, 130, 210));
	}

	// 수리비 할인 개조를 한 아이템의 수리비 할인율 표시
	if(pItem->m_bRepairDiscount != 0)
	{
		_stprintf( szTip, IDS_REPAIR_DISCOUNT, pItem->m_bRepairDiscount );
		g_MainCharInfo.m_pToolTip->AddToolTip( szTip, D3DCOLOR_XRGB( 150, 150, 255 ));
		g_MainCharInfo.m_pToolTip->AddToolTip(IDS_REPAIR_DISCOUNT1, D3DCOLOR_XRGB(150, 150, 255));
	}
}

/**
 * 아이템 추가
 * \param bSackPos 위치
 * \param pItem 아이템 포인터
 * \return 성공 여부
 */
BOOL CSack::InsertItem(BYTE bSackPos, XiahItem::sItemInfo* pItem)
{	
	// [10/27/2004] 이상한 슬롯에 넣을려고 하는것 검사 - 이것 빠지면 잘못하면 죽는다
	if(m_bySackTotalSize < bSackPos)
		return false;

	XiahItem::SetItemVisualData( pItem);

	pItem->m_bSackID	= m_bySackType;
	pItem->m_bSackPos	= bSackPos;
	m_vecItem[bSackPos] = pItem;

	// m_vecItemVB 세팅하기
//	g_pDirect3DDevice->CreateVertexBuffer( 4*sizeof(VT_TLVertex),
//										D3DUSAGE_WRITEONLY | D3DUSAGE_DYNAMIC, D3DFVF_TLVERTEX,
//										D3DPOOL_DEFAULT, &m_vecItemVB[ bSackPos], NULL);
	if(!m_vecItemVB[bSackPos])
		g_pDirect3DDevice->CreateVertexBuffer( 4*sizeof(VT_TLVertex),
										0, D3DFVF_TLVERTEX,
										D3DPOOL_MANAGED, &m_vecItemVB[ bSackPos], NULL);

	SetVB(bSackPos, pItem);

	return TRUE;
}

/**
 * 소켓 - 보석 VB생성
 * \param bSackPos 
 * \param pItem 
 */
void CSack::CreateSocketVB(BYTE bSackPos, XiahItem::sItemInfo* pItem)
{
	// 제련
	if(pItem->m_bSocketCount)
	{
		if(!m_vecItemSocketVB[bSackPos])
			g_pDirect3DDevice->CreateVertexBuffer( 4*sizeof(VT_TLVertex),
											0, D3DFVF_TLVERTEX,
											D3DPOOL_MANAGED, &m_vecItemSocketVB[bSackPos], NULL);

		if(pItem->m_bSocketItem[0])
		{
			if(!m_vecSocketItem1VB[bSackPos])
				g_pDirect3DDevice->CreateVertexBuffer( 4*sizeof(VT_TLVertex),
											0, D3DFVF_TLVERTEX,
											D3DPOOL_MANAGED, &m_vecSocketItem1VB[bSackPos], NULL);
		}

		if(pItem->m_bSocketItem[1])
		{
			if(!m_vecSocketItem2VB[bSackPos])
				g_pDirect3DDevice->CreateVertexBuffer( 4*sizeof(VT_TLVertex),
											0, D3DFVF_TLVERTEX,
											D3DPOOL_MANAGED, &m_vecSocketItem2VB[bSackPos], NULL);
		}		

		if(pItem->m_bSocketItem[2])
		{
			if(!m_vecSocketItem3VB[bSackPos])
				g_pDirect3DDevice->CreateVertexBuffer( 4*sizeof(VT_TLVertex),
											0, D3DFVF_TLVERTEX,
											D3DPOOL_MANAGED, &m_vecSocketItem3VB[bSackPos], NULL);
		}
	}
	
	if(pItem->m_wRBSocketItem > 0)
	{
		//HT_1116 : 각성자 아이템 추가
		if(pItem->m_wRBSocketItem == 1)
		{
			if(!m_vecRBSocketItemVB[bSackPos])
				g_pDirect3DDevice->CreateVertexBuffer( 4*sizeof(VT_TLVertex),
											0, D3DFVF_TLVERTEX,
											D3DPOOL_MANAGED, &m_vecRBSocketItemVB[bSackPos], NULL);
		}	
		else 
		{
			if(!m_vecRBSocketItemVB[bSackPos])
				g_pDirect3DDevice->CreateVertexBuffer( 4*sizeof(VT_TLVertex),
											0, D3DFVF_TLVERTEX,
											D3DPOOL_MANAGED, &m_vecRBSocketItemVB[bSackPos], NULL);

			if(!m_vecRBItemStoneVB[bSackPos])
				g_pDirect3DDevice->CreateVertexBuffer( 4*sizeof(VT_TLVertex),
											0, D3DFVF_TLVERTEX,
											D3DPOOL_MANAGED, &m_vecRBItemStoneVB[bSackPos], NULL);
		}
	}
}

/**
 * 아이템 삭제
 * \param bSackPos 
 */
void CSack::DeleteItem( BYTE bSackPos, bool bDelete)
{
	// 이상한 슬롯 지울려고 하는지 검사
	if(m_bySackTotalSize < bSackPos)
		return;

	// 누수 수정
	if(bDelete)
	{
		if(m_vecItem[ bSackPos] != NULL)
			delete m_vecItem[ bSackPos];
	}

	m_vecItem[ bSackPos] = NULL;

	if( m_vecItemVB[ bSackPos])
	{
		m_vecItemVB[ bSackPos]->Release();
		m_vecItemVB[ bSackPos] = NULL;
	}

	if( m_vecItemRt[ bSackPos])
	{
		delete m_vecItemRt[ bSackPos];
		m_vecItemRt[ bSackPos] = NULL;
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

	return;
}

/**
 *
 * \param nResID 
 * \return 
 */
IDirect3DTexture9* CSack::Gettex( int nResID)
{
	return XiahPak::GetTexture( nResID, TRUE);
}

/**
 *
 * \return 
 */
BOOL CSack::CheckItemSelected()
{
	if(m_bShow)
	{
		if( CheckItemSelectedByLButton())
			return TRUE;
		else if( CheckItemSelectedByRButton())
			return TRUE;
	}

	return FALSE;
}

/**
 *
 * \return 
 */
BOOL CSack::CheckItemSelectedByLButton()
{
	if( !XiahInput::g_bLButtonDown)
		return FALSE;

	if( g_MainCharInfo.m_bMainCharDie || g_MainCharInfo.m_bMainCharMapMoveItemUse )
		return FALSE;

	if( g_CursorType == eCT_General)
	{
		// CG_2005/01/28 : 변종아이템 갑옷을 입었을때 아예 클릭안되게 막자.
		if( m_bySackType == SACKTYPE__EQUIPMENT )
		{
			if( g_MainCharInfo.m_pEquipSack->m_vecItem[ 1 ] != NULL &&
				g_MainCharInfo.m_pEquipSack->m_vecItem[ 2 ] != NULL &&
				g_MainCharInfo.m_pEquipSack->m_vecItem[ 2 ]->m_bNeedCharType > 250 &&                
				g_MainCharInfo.m_pEquipSack->m_vecItemRt[ 2 ]->PtInRect( XiahInput::g_ptMouse) )
			{
				g_MainCharInfo.ShowHelpMessage( IDS_NOTOUTSAMECHARTYPE, TEXTEFFECT_COLOR_WARNING);
				return TRUE;
			}
		}


		for( int i=0; i < m_bySackTotalSize; ++i)
		{
			if( m_vecItem[i] != NULL && 
				m_vecItemRt[i] != NULL &&
				m_vecItemRt[i]->PtInRect( XiahInput::g_ptMouse))
			{
				// 내가 처리하지 않아도 되는 종류의 행낭
				if( m_bySackType == SACKTYPE__PC_TRADE_OTHER || m_bySackType == SACKTYPE__MODIFY || m_bySackType == SACKTYPE__FIVEELEMENT_CONVERT)
				{
					return TRUE;
				}
	
				// 애초에 못움직이게 막기?
				if( m_bySackType == SACKTYPE__PC_TRADE_MINE)
				{
					g_MainCharInfo.ShowHelpMessage( (IDS_CANNOT_REMOVE_ITEM), TEXTEFFECT_COLOR_WARNING);

					return TRUE;
				}

				// 수량이 있는 종류의 아이템인가
				//if( m_vecItem[ i]->m_wVisualID == POTION_VISUALID_1 || m_vecItem[ i]->m_wVisualID == POTION_VISUALID_2)


				//XiahItem::sItemInfo* pTempT = m_vecItem[ i];
				bool bDraw = true;

				if( m_vecItem[ i]->m_bItemType == ITEMTYPE_POTION || m_vecItem[ i]->m_bItemType == ITEMTYPE_PORTAL || 
					m_vecItem[ i]->m_bItemType == ITEMTYPE_GOLDKEY || 
					(m_vecItem[ i]->m_bItemType == ITEMTYPE_EVENT && m_vecItem[ i]->m_bItemKind == 11)) //HO_0828_07 황금열쇠 추가
				{
					if( GetAsyncKeyState( VK_CONTROL) < 0 &&
					(( m_vecItem[ i]->m_dwAmount > 1 && (m_bySackType == SACKTYPE__DEFAULT || m_bySackType == SACKTYPE__DEFAULT2)) || (m_vecItem[ i]->m_dwAmount == 1 && m_bySackType == SACKTYPE__NPC_TRADE)))
					{
						g_MainCharInfo.OpenFrame( WINDOW_VOLUME, 2);
						g_MainCharInfo.m_pHoldItem->SetDrawFlag( FALSE);

						bDraw = false;
					}	
				}				

				g_MainCharInfo.m_pHoldItem->SetHoldItemItem( m_vecItem[ i], bDraw);
				g_MainCharInfo.m_pHoldItem->GetHoldItemItem()->m_bSackPos = i;


				// 상점아이템인가
				if( m_bySackType != SACKTYPE__NPC_TRADE)
					DeleteItem( i);

				g_MainCharInfo.PlayInterfaceSound( ISOUND_ITEM_HOLD);

				return TRUE;
			}
		}

		// Check Money
		if( m_bySackType == SACKTYPE__DEFAULT)
		{
			sRect Rect;

			g_pUIManager->GetRegionData(DRG_ITEM_WINDOW, drg_item_window_money_dummy, Rect);

			if( Rect.PtInRect( XiahInput::g_ptMouse))
			{
				//g_MainCharInfo.m_pHoldItem->SetHoldItemMoneySack( SACKTYPE__DEFAULT);
				g_pUIManager->SetString(WINDOW_VOLUME, volume_window_edit, 0);
				g_MainCharInfo.OpenFrame( WINDOW_VOLUME, 1);

				return TRUE;
			}
		}
	}
	else if( g_CursorType == eCT_Repair)
	{
		for( int i=0; i < m_bySackTotalSize; ++i)
		{
			if( m_vecItem[i] != NULL && 
				m_vecItemRt[i] != NULL &&
				m_vecItemRt[i]->PtInRect( XiahInput::g_ptMouse))
			{
				if( m_bySackType == SACKTYPE__DEFAULT || m_bySackType == SACKTYPE__EQUIPMENT)
				{
					if( m_vecItem[i]->m_wCurDur == m_vecItem[i]->m_wMaxDur)
					{
						;//g_MainCharInfo.ShowHelpMessage( (IDS_NO_FIX));
					}
					else
					{
						// [3/19/2004]
						if(g_MainCharInfo.m_bReairItemUse)
						{
							if( m_bySackType == SACKTYPE__DEFAULT)
							{
                                SendCS_IM_REPAIRWITHITEM_REQ(g_MainCharInfo.m_dwResItemID,  g_MainCharInfo.m_ReairSackID, g_MainCharInfo.m_RpairItemPos,
															m_vecItem[i]->m_dwItemID, m_vecItem[i]->m_bSackCount+1, m_vecItem[i]->m_bSackPos);
							}
							else
							{
								SendCS_IM_REPAIRWITHITEM_REQ(g_MainCharInfo.m_dwResItemID,  g_MainCharInfo.m_ReairSackID, g_MainCharInfo.m_RpairItemPos,
															m_vecItem[i]->m_dwItemID, m_vecItem[i]->m_bSackID, m_vecItem[i]->m_bSackPos);
							}

							return true;
						}

						XiahObject::CXiahObject* pObject = XiahObject::g_XiahObjectManager.FindXiahObject( MAKEOBJECTID( 0, g_MainCharInfo.m_dwPickedObject, OBJTYPE_FUNCTIONALNPC));

						if( pObject == NULL ) return FALSE;
						if( pObject->m_pObject == NULL ) return FALSE;

						CXiahCharObject* pCharObject = reinterpret_cast<CXiahCharObject*>( pObject->m_pObject);
						sFunctionalNpcInfo* pInfo = (sFunctionalNpcInfo*)pCharObject->m_pPrivateData;

						if( !pInfo)
							return TRUE;

						switch( pInfo->m_bType)
						{
						case 1: //대장장이
							if( m_vecItem[i]->m_bItemType != ITEMTYPE_WEAPON)
							{
								g_MainCharInfo.ShowHelpMessage( IDS_ONLY_WEAPON_REPAIR, TEXTEFFECT_COLOR_WARNING);
								return TRUE;
							}
							break;
						case 2: //의류상인
							if( m_vecItem[i]->m_bItemType != ITEMTYPE_CLOTH)
							{
								g_MainCharInfo.ShowHelpMessage( IDS_ONLY_CLOTH_REPAIR, TEXTEFFECT_COLOR_WARNING);
								return TRUE;
							}
							break;
						case 3: //잡화상인
							if( !(m_vecItem[i]->m_bItemType == ITEMTYPE_HAT ||
								m_vecItem[i]->m_bItemType == ITEMTYPE_SHOE))
							{
								g_MainCharInfo.ShowHelpMessage( IDS_ONLY_JABWHA_REPAIR, TEXTEFFECT_COLOR_WARNING);
								return TRUE;
							}
							break;
						case 4:	//보석상인
							if( !(m_vecItem[i]->m_bItemType == ITEMTYPE_RING ||
								m_vecItem[i]->m_bItemType == ITEMTYPE_NECKLACE))
							{
								g_MainCharInfo.ShowHelpMessage( IDS_ONLY_JEWERY_REPAIR, TEXTEFFECT_COLOR_WARNING);
								return TRUE;
							}
							break;
						}

						if( m_bySackType == SACKTYPE__DEFAULT)
							SendCS_IM_REPAIRITEM_REQ( m_vecItem[i]->m_dwItemID, m_vecItem[i]->m_bSackCount+1, m_vecItem[i]->m_bSackPos);
						else
							SendCS_IM_REPAIRITEM_REQ( m_vecItem[i]->m_dwItemID, m_vecItem[i]->m_bSackID, m_vecItem[i]->m_bSackPos);
					}
				}
				
				return TRUE;
			}
		}
	}

	return FALSE;
}

/**
 *
 * \return 
 */
BOOL CSack::CheckItemSelectedByRButton()
{
	if( !XiahInput::g_bRButtonDown)
		return FALSE;
	
	if ( m_bySackType != SACKTYPE__DEFAULT)
	{
		// 상점 빠른 구입
		if(m_bySackType == SACKTYPE__NPC_TRADE || m_bySackType == SACKTYPE__PERSONAL_TRADE_SELL)
		{
			for(BYTE i=0; i < m_bySackTotalSize; ++i)
			{
				if( m_vecItem[i] != NULL && 
					m_vecItemRt[i] != NULL &&
					m_vecItemRt[i]->PtInRect( XiahInput::g_ptMouse))
				{
					DWORD dwTime = timeGetTime();
					static bool bDelay = false;

					// 1.1초 간격의 더블 클릭시
					if(!bDelay && (dwTime - g_MainCharInfo.m_dwLastClickTime) <= 1100)
					{
						if(g_MainCharInfo.m_byLastSackPos == i)
						{
							g_MainCharInfo.m_byLastSackPos = 255;
							bDelay = true;											

							g_MainCharInfo.m_pHoldItem->SetHoldItemItem(m_vecItem[i], true);
							g_MainCharInfo.m_pHoldItem->GetHoldItemItem()->m_bSackPos = i;
							g_MainCharInfo.m_pHoldItem->GetHoldItemItem()->m_dwBuyCount = m_vecItem[i]->m_dwAmount;

							// NPC구입
							if(m_bySackType == SACKTYPE__NPC_TRADE)
							{
								// 거래 옵션
								if(m_vecItem[i]->m_dwPrice >= g_MainCharInfo.m_dwBuyLimit && g_MainCharInfo.m_dwBuyLimit)
								{	
									g_pUIManager->ShowNotice(IDS_LIMIT_PTBUY, NOTICE_FRAME_OKCANCEL, NOTICE_FRAME_LIMIT_BUY, XiahInput::g_ptMouse.x - 270, XiahInput::g_ptMouse.y-90);
								}
								else
								{
									SendCS_EC_BUYITEM_REQ(g_MainCharInfo.m_dwPickedObject, 
														m_vecItem[i]->m_wRefID, 
														m_vecItem[i]->m_dwAmount,
														g_MainCharInfo.m_bSackCnt,
														m_vecItem[i]->m_bSackPos, 
														g_MainCharInfo.m_byMySackCurrIdx+1,
														255);

									//g_MainCharInfo.ShowHelpMessage(IDS_PT_SELL_NOUSE);
								}
							}
							// 상점 구입
							else if(m_bySackType == SACKTYPE__PERSONAL_TRADE_SELL)
							{
								g_MainCharInfo.m_pHoldItem->m_bBackPosition = 255;								

								TCHAR strText[1024] = {0,};
								_stprintf(strText, IDS_PT_BUY, (LPCTSTR)m_vecItem[i]->m_szName, MoneyCommaStr(m_vecItem[i]->m_dwPrice).data());

								g_pUIManager->ShowNotice(strText, NOTICE_FRAME_OKCANCEL, NOTICE_FRAME_PT_BUY, XiahInput::g_ptMouse.x - 270, XiahInput::g_ptMouse.y-90);
							}						

							break;
						}
					}

					// 서버에서 0.8초 전에 재 구입시 막는다.
					// 서버에서도 검사 하지만 여기서도 막아주자.
					if((dwTime - g_MainCharInfo.m_dwLastClickTime) >= 810)
					{
						bDelay = false;
						g_MainCharInfo.m_byLastSackPos	 = i;
						g_MainCharInfo.m_dwLastClickTime = dwTime;
					}					

					break;
				}
			}		

			return TRUE;
		}

		return FALSE;
	}

	if(g_MainCharInfo.m_bPersonalTradeSell)
	{
		g_MainCharInfo.ShowHelpMessage(IDS_PT_SELL_NOUSE);
		return false;
	}

	if( g_CursorType != eCT_General)
		return FALSE;

	if( g_MainCharInfo.m_bMainCharDie || g_MainCharInfo.m_bMainCharMapMoveItemUse )
		return FALSE;

	for( int i=0; i < m_bySackTotalSize; ++i)
	{
		if( m_vecItem[i] != NULL && 
			m_vecItemRt[i] != NULL &&
			m_vecItemRt[i]->PtInRect( XiahInput::g_ptMouse))

		{
			bool bUseCommonItemUseSound = true;

			// 봉인단지 또는 봉인 아이템
			if( m_vecItem[i]->m_wVisualID == 9200 ||
				m_vecItem[i]->m_wVisualID == 9210 ||
				m_vecItem[i]->m_wVisualID == 9211 ||
				m_vecItem[i]->m_wVisualID == 9212 ||
				m_vecItem[i]->m_wVisualID == 9213 ||
				m_vecItem[i]->m_wVisualID == 9214 ||
				m_vecItem[i]->m_wVisualID == 9215 ||
				m_vecItem[i]->m_wVisualID == 9235 || //HT_0410 : 신규 빙정류 추가
				m_vecItem[i]->m_wVisualID == 9236 ||
				m_vecItem[i]->m_wVisualID == 9237)
			{
				if( m_vecItem[i]->m_dwNpcID)
					SendCS_NC_PETBONGOUT_REQ( m_vecItem[i]->m_bSackCount+1, m_vecItem[i]->m_bSackPos);
				else
				{
					//봉인해볼까..
					//HT_0711 : 진각성 무공 (애완용 펫만 이름을 출력해 준다. 분신 및 환수는 나오지마~)
					BYTE bSize = g_PetList.size();
					
					switch(bSize)
					{
					case 0:
						g_MainCharInfo.ShowHelpMessage( IDS_NO_PET);
						break;
					case 1:
						{	
							if((LPCTSTR)g_PetList.GetPetInfoByIndex(0)->m_dwIsHwan == 0)
							{
								g_pUIManager->MakePopComboMenu(	100, m_vecItemRt[i]->right, m_vecItemRt[i]->bottom,
													FRAMEID_BONBINITEM, 3, 
													TRUE, RESID_COMBO, (LPCTSTR)g_PetList.GetPetInfoByIndex(0)->szName,
													FALSE, RESID_COMBO, IDS_EMPTY,
													FALSE, RESID_COMBO, IDS_EMPTY);

								g_MainCharInfo.m_dwCurrentSelectedBongInItem = m_vecItem[i]->m_dwItemID;
							}
							else
								g_MainCharInfo.ShowHelpMessage( IDS_NO_PET);

						}
						break;
					case 2:
						{
							if((LPCTSTR)g_PetList.GetPetInfoByIndex(0)->m_dwIsHwan == 0)
							{
								g_pUIManager->MakePopComboMenu(	100, m_vecItemRt[i]->right, m_vecItemRt[i]->bottom,
													FRAMEID_BONBINITEM, 3, 
													TRUE, RESID_COMBO, (LPCTSTR)g_PetList.GetPetInfoByIndex(0)->szName,
													FALSE, RESID_COMBO, IDS_EMPTY,
													FALSE, RESID_COMBO, IDS_EMPTY);

								g_MainCharInfo.m_dwCurrentSelectedBongInItem = m_vecItem[i]->m_dwItemID;
							}
							else if((LPCTSTR)g_PetList.GetPetInfoByIndex(1)->m_dwIsHwan == 0)
							{
								g_pUIManager->MakePopComboMenu(	100, m_vecItemRt[i]->right, m_vecItemRt[i]->bottom,
													FRAMEID_BONBINITEM, 3, 
													TRUE, RESID_COMBO, (LPCTSTR)g_PetList.GetPetInfoByIndex(1)->szName,
													FALSE, RESID_COMBO, IDS_EMPTY,
													FALSE, RESID_COMBO, IDS_EMPTY);

								g_MainCharInfo.m_dwCurrentSelectedBongInItem = m_vecItem[i]->m_dwItemID;
							}
							else
								g_MainCharInfo.ShowHelpMessage( IDS_NO_PET);
						}
						break;
					case 3:
						{
							if((LPCTSTR)g_PetList.GetPetInfoByIndex(0)->m_dwIsHwan == 0)
							{
								g_pUIManager->MakePopComboMenu(	100, m_vecItemRt[i]->right, m_vecItemRt[i]->bottom,
													FRAMEID_BONBINITEM, 3, 
													TRUE, RESID_COMBO, (LPCTSTR)g_PetList.GetPetInfoByIndex(0)->szName,
													FALSE, RESID_COMBO, IDS_EMPTY,
													FALSE, RESID_COMBO, IDS_EMPTY);

								g_MainCharInfo.m_dwCurrentSelectedBongInItem = m_vecItem[i]->m_dwItemID;
							}
							else if((LPCTSTR)g_PetList.GetPetInfoByIndex(1)->m_dwIsHwan == 0)
							{
								g_pUIManager->MakePopComboMenu(	100, m_vecItemRt[i]->right, m_vecItemRt[i]->bottom,
													FRAMEID_BONBINITEM, 3, 
													TRUE, RESID_COMBO, (LPCTSTR)g_PetList.GetPetInfoByIndex(1)->szName,
													FALSE, RESID_COMBO, IDS_EMPTY,
													FALSE, RESID_COMBO, IDS_EMPTY);

								g_MainCharInfo.m_dwCurrentSelectedBongInItem = m_vecItem[i]->m_dwItemID;
							}
							else if((LPCTSTR)g_PetList.GetPetInfoByIndex(2)->m_dwIsHwan == 0)
							{
								g_pUIManager->MakePopComboMenu(	100, m_vecItemRt[i]->right, m_vecItemRt[i]->bottom,
													FRAMEID_BONBINITEM, 3, 
													TRUE, RESID_COMBO, (LPCTSTR)g_PetList.GetPetInfoByIndex(2)->szName,
													FALSE, RESID_COMBO, IDS_EMPTY,
													FALSE, RESID_COMBO, IDS_EMPTY);

								g_MainCharInfo.m_dwCurrentSelectedBongInItem = m_vecItem[i]->m_dwItemID;
							}
							else
								g_MainCharInfo.ShowHelpMessage( IDS_NO_PET);
						}
						break;
					//case 2:
					//	{
					//		g_pUIManager->MakePopComboMenu(	100, m_vecItemRt[i]->right, m_vecItemRt[i]->bottom,
					//							FRAMEID_BONBINITEM, 3, 
					//							TRUE, RESID_COMBO, (LPCTSTR)g_PetList.GetPetInfoByIndex(0)->szName,
					//							TRUE, RESID_COMBO, (LPCTSTR)g_PetList.GetPetInfoByIndex(1)->szName,
					//							FALSE, RESID_COMBO, IDS_EMPTY);

					//		g_MainCharInfo.m_dwCurrentSelectedBongInItem = m_vecItem[i]->m_dwItemID;
					//	}
					//	break;
					//case 3:
					//	{
					//		g_pUIManager->MakePopComboMenu(	100, m_vecItemRt[i]->right, m_vecItemRt[i]->bottom,
					//							FRAMEID_BONBINITEM, 3, 
					//							TRUE, RESID_COMBO, (LPCTSTR)g_PetList.GetPetInfoByIndex(0)->szName,
					//							TRUE, RESID_COMBO, (LPCTSTR)g_PetList.GetPetInfoByIndex(1)->szName,
					//							TRUE, RESID_COMBO, (LPCTSTR)g_PetList.GetPetInfoByIndex(2)->szName);

					//		g_MainCharInfo.m_dwCurrentSelectedBongInItem = m_vecItem[i]->m_dwItemID;
					//	}
					//	break;
					}
				}
			}
			else
			{
				switch(m_vecItem[i]->m_wRefID)
				{
				case 20503:	// 공빙정
				case 20504:	// 방빙정
				case 20505:	// 정빙정
				case 20506:	// 생빙정
				case 20603:	// 공빙정(pc방)
				case 20604:	// 방빙정(pc방)
				case 20605:	// 정빙정(pc방)
				case 20606:	// 생빙정(pc방)
	//			case 21863:	// 살혼정	//HO_0410_07 살,보,용혼정 추가
	//			case 21864:	// 보혼정
	//			case 21865:	// 용혼정
					{
						if(g_PetList.size())
						{							
							g_pUIManager->MakePopComboMenu(100, m_vecItemRt[i]->right, m_vecItemRt[i]->bottom,
															FRAMEID_CRYOLITE, 3, 
															TRUE, RESID_COMBO, (LPCTSTR)g_PetList.GetPetInfoByIndex(0)->szName,
															FALSE, RESID_COMBO, IDS_EMPTY,
															FALSE, RESID_COMBO, IDS_EMPTY);

							g_MainCharInfo.m_dwCurrentSelectedBongInItem = m_vecItem[i]->m_dwItemID;
						}
					}						
					break;
				case 20850:	// 절연부
					{
						g_MainCharInfo.m_ReairSackID = m_vecItem[i]->m_bSackCount+1;
						g_MainCharInfo.m_RpairItemPos = m_vecItem[i]->m_bSackPos; 
						g_MainCharInfo.m_dwResItemID = m_vecItem[i]->m_dwItemID;

						g_pUIManager->ShowNotice(IDS_SERVER_REL1, 60, NOTICE_FRAME_SEVER_RELATION1, XiahInput::g_ptMouse.x - 270, XiahInput::g_ptMouse.y-90);

						return true;
					}
					break;
				}

				// 전낭 콤보
				if( m_vecItem[i]->m_wFunctionItem == 6 || m_vecItem[i]->m_wFunctionItem == 11 && m_vecItem[i]->m_bItemType == ITEMTYPE_GISDURABLITY)//HO_0918_07 초보자용 전낭 추가 : 전낭(소) pItem->m_wFunctionItem == 11
				{
					// 전낭 패킷에 사용되는 항목 저장.
					g_MainCharInfo.m_byPurseSackID  = m_vecItem[i]->m_bSackCount+1;
					g_MainCharInfo.m_byPurseSackPos = m_vecItem[i]->m_bSackPos;
					g_MainCharInfo.m_dwPurseItemID  = m_vecItem[i]->m_dwItemID;

					g_pUIManager->MakePopComboMenu(	100, m_vecItemRt[i]->right, m_vecItemRt[i]->bottom,
													FRAMEID_PURSE, 2, 
													TRUE, RESID_COMBO, IDS_PURSE_IN_2,
													TRUE, RESID_COMBO, IDS_PURSE_OUT_2);

					XiahItem::sItemInfo* pItem = m_vecItem[i];

					if(pItem)
					{
						TCHAR strTemp[1024] = {0,};
						_stprintf(strTemp, IDS_PURSE_IN_DES, MoneyCommaStr(pItem->m_dwValue).data());
						g_pUIManager->SetString(WINDOW_PURSE, window_purse_dummy_01, strTemp);
						_stprintf(strTemp, IDS_PURSE_OUT_USE, pItem->m_wCurDur);
						g_pUIManager->SetString(WINDOW_PURSE, window_purse_dummy_03, strTemp);
					}					
				} // if( m_vecItem[i]->m_wFunctionItem == 6 && m_vecItem[i]->m_bItemType == ITEMTYPE_GISDURABLITY)

				/////////////////////////////////////////////////////////////////////////////////////////////////////
				// 복권
				if(m_vecItem[i]->m_bItemType == ITEMTYPE_LOTTO)
				{
					g_MainCharInfo.m_ReairSackID = m_vecItem[i]->m_bSackCount+1;
					g_MainCharInfo.m_RpairItemPos = m_vecItem[i]->m_bSackPos; 
					g_MainCharInfo.m_dwResItemID = m_vecItem[i]->m_dwItemID;

					bool bLottoCheck1 = true;
					bool bLottoCheck2 = false;

					// 복권횅땍자는 패스
					if(m_vecItem[i]->m_bPrizeRank >= 1 && m_vecItem[i]->m_bPrizeRank <= 4)
						bLottoCheck1 = false;

					// 당첨자만 & 당첨금액보유자
					if(m_vecItem[i]->m_bPrizeRank >=1 && m_vecItem[i]->m_bPrizeRank <= 3 && m_vecItem[i]->m_dwPrizeMoney)
						bLottoCheck2 = true;

					g_pUIManager->MakePopComboMenu(100, m_vecItemRt[i]->right, m_vecItemRt[i]->bottom,
													FRAMEID_LOTTOCHECK, 2, 
													bLottoCheck1, RESID_COMBO, IDS_LOTTO_DEFINITE_2,
													bLottoCheck2, RESID_COMBO, IDS_LOTTO_RECEIVE);

				}

				// 연장 - 수리
				// 수리망치
				if(m_vecItem[i]->m_wVisualID == 10300 || m_vecItem[i]->m_wVisualID == 10301 ||
					m_vecItem[i]->m_wVisualID == 31013 || m_vecItem[i]->m_wVisualID == 10302 ||
					m_vecItem[i]->m_wVisualID == 10303 || m_vecItem[i]->m_wVisualID == 10304)
				{
					ChangeXiahCursor(eCT_Repair);

					g_MainCharInfo.m_bReairItemUse	= true;
					g_MainCharInfo.m_ReairSackID	= m_vecItem[i]->m_bSackCount+1;
					g_MainCharInfo.m_RpairItemPos	= m_vecItem[i]->m_bSackPos; 
					g_MainCharInfo.m_dwResItemID	= m_vecItem[i]->m_dwItemID;

					if(m_vecItem[i]->m_wVisualID == 31013 || m_vecItem[i]->m_wVisualID == 10304 ||
						m_vecItem[i]->m_wVisualID == 10302 || m_vecItem[i]->m_wVisualID == 10303)
					{
						g_MainCharInfo.m_bReairItemUse2 = true;
					}

					return true;
				} // if(m_vecItem[i]->m_wVisualID == 10300)

				// 동신주
				if(((m_vecItem[i]->m_wFunctionItem >= 2 && m_vecItem[i]->m_wFunctionItem <= 4) || (m_vecItem[i]->m_wFunctionItem == 8))	&& !g_pUIManager->IsShow(WINDOW_DONGSIN))
				{
					g_MainCharInfo.m_nTempValue = m_vecItem[i]->m_wFunctionItem;

					switch(g_MainCharInfo.m_nTempValue)
					{
					case 2:
						g_pUIManager->SetString(GAK_MESSAGE_WINDOW, gak_title_dummy, IDS_GAK_TITLE_S);
						break;
					case 3:
						g_pUIManager->SetString(GAK_MESSAGE_WINDOW, gak_title_dummy, IDS_GAK_TITLE_M);
						break;
					case 4:
						g_pUIManager->SetString(GAK_MESSAGE_WINDOW, gak_title_dummy, IDS_GAK_TITLE_L);
						break;
					case 8:	// 황금각적
						g_pUIManager->SetString(GAK_MESSAGE_WINDOW, gak_title_dummy, IDS_GAK_TITLE_GOLD);						
						break;
					} // switch(m_vecItem[i]->m_wFacultyItem)

					g_MainCharInfo.m_ReairSackID = m_vecItem[i]->m_bSackCount+1;
					g_MainCharInfo.m_RpairItemPos = m_vecItem[i]->m_bSackPos; 

					g_MainCharInfo.OpenFrame(GAK_MESSAGE_WINDOW);
					g_pUIManager->SetPostMsg(GAK_MSG_WINDOW_MSG);

					g_pUIManager->SetString(GAK_MESSAGE_WINDOW, gak_message_edit, _T(""));
					g_pUIManager->SetFocus(GAK_MESSAGE_WINDOW);
					g_pUIManager->SetFocus(GAK_MESSAGE_WINDOW, gak_message_edit);

					return true;
				} // if(m_vecItem[i]->m_wFunctionItem >= 2 && m_vecItem[i]->m_wFunctionItem <= 4)

				// 동신주
				if(m_vecItem[i]->m_bItemKind == 1 && m_vecItem[i]->m_bItemType == ITEMTYPE_PORTAL && !g_pUIManager->IsShow(GAK_MESSAGE_WINDOW))
				{
					LPCTSTR lpStrTemp;

					switch(m_vecItem[i]->m_dwPotalMapID)
					{
					case 0:
						lpStrTemp = IDS_NO_REMARK;						
						break;
					default:
						lpStrTemp = GetMapName(m_vecItem[i]->m_dwPotalMapID);
						break;
					}

					TCHAR szContent[1024] = {0,};
					_stprintf( szContent, _T("%d, %d"), m_vecItem[i]->m_wPosX/4, m_vecItem[i]->m_wPosY/4);

					g_MainCharInfo.m_ReairSackID = m_vecItem[i]->m_bSackCount+1;
					g_MainCharInfo.m_RpairItemPos = m_vecItem[i]->m_bSackPos; 
					g_MainCharInfo.m_dwResItemID = m_vecItem[i]->m_dwItemID;

					// 미 지정 좌표일경우
					if(m_vecItem[i]->m_wPosX == 0 && m_vecItem[i]->m_wPosY == 0)
						g_MainCharInfo.m_bMark = false;
					else
						g_MainCharInfo.m_bMark = true;

					g_pUIManager->SetString(WINDOW_DONGSIN, dongsin_window_point_back, lpStrTemp);
					g_pUIManager->SetString(WINDOW_DONGSIN, dongsin_window_point_back_01, szContent);				

					CloseAllWindow();
					g_MainCharInfo.OpenFrame(WINDOW_DONGSIN);

					return true;
				} // if(m_vecItem[i]->m_bItemKind == 1 && m_vecItem[i]->m_bItemType == ITEMTYPE_PORTAL && !g_pUIManager->IsShow(GAK_MESSAGE_WINDOW))

				if(m_vecItem[i]->m_bItemKind == 1 && m_vecItem[i]->m_bItemType == ITEMTYPE_PORTAL)
					return true;

				// 포탈 불가능한 경우
				CXiahCharObject* pMainChar = (CXiahCharObject*)g_pMainChar->m_pObject;

				if(m_vecItem[i]->m_bItemType == ITEMTYPE_PORTAL && !g_MainCharInfo.m_bPortalMove)
				{
					// 단 비무일때에도 포탈 기능 불가능
					if( pMainChar->m_dwPartyID && pMainChar->m_dwEnemyPartyID )
						g_MainCharInfo.ShowHelpMessage(IDS_NOTPORTALMOVE_INDANBATTLE);
					else	// 이벤트 아이템의 경우 포탈 불가능
						g_MainCharInfo.ShowHelpMessage(IDS_NOTPORTALMOVE);

					return true;
				}

				// 동신주, 이형부
				if( m_vecItem[i]->m_wVisualID == 10000 || m_vecItem[i]->m_wVisualID == 10001)
				{
					if(g_MainCharInfo.m_dwHpCur == 0)	// 생명력 0 (죽을시) 이동하면 서버에서 무시 해버린다. 그래서 사용불가 처리
					{
						g_MainCharInfo.ShowHelpMessage(IDS_NOT_MOVE, 2);
						return true;
					}

					// 아이템을 사용하여 이동한다.
					g_MainCharInfo.m_bMainCharMapMoveItemUse = TRUE;

					g_MainCharInfo.ShowHelpMessage( IDS_MOVE_ITEMUSE );

					CloseAllWindow();

					// [5/20/2005] 매품패
					if(g_MainCharInfo.m_pQuickMart)
					{
						g_MainCharInfo.HideSack(SACKTYPE__QUICKMART, FALSE);						
					}

//					g_MainCharInfo.OpenFrame( LOADING_IMAGE);					
				}

				// 전서구
				if( m_vecItem[i]->m_wFunctionItem == GISDURABLITY_MEMO)
				{
					// 아이템의 위치
					g_Mail.Set_SackID(m_vecItem[i]->m_bSackCount+1);
					g_Mail.Set_SackPos(m_vecItem[i]->m_bSackPos);

					g_Mail.Reflash_MAIL_Select();
					g_Mail.Reflash_MAIL();

					CloseAllWindow();
					g_MainCharInfo.OpenFrame(WINDOW_MAIL);
					g_MainCharInfo.OpenFrame(WINDOW_MAIL_SELECT);

					if(m_vecItem[i]->m_wRefID == 20674)
					{
						g_pUIManager->SetString(WINDOW_MAIL, window_mail_title_dummy, IDS_CARD_TITLE_01);
					}
					else
					{
						g_pUIManager->SetString(WINDOW_MAIL, window_mail_title_dummy, IDS_MAIL_TITLE_01);
					}

					return true;
				}

				// 고대무림기서
				if( m_vecItem[i]->m_wRefID == 20513 )
				{
					// 이녀석은 사운드가 따로 있다.
					bUseCommonItemUseSound = false;

					g_MainCharInfo.PlayInterfaceSound( EVENT_PREMIUMITEM_SOUND_CLICK );
				}

				// TODO: 펫
				g_MainCharInfo.m_ReairSackID = m_vecItem[i]->m_bSackCount+1;
				g_MainCharInfo.m_RpairItemPos = m_vecItem[i]->m_bSackPos; 

				g_MainCharInfo.m_nLastUseItemXPos = m_vecItemRt[i]->right;
				g_MainCharInfo.m_nLastUseItemYPos = m_vecItemRt[i]->bottom;
				/////////////////////////////////////////////////////////////////////////////////////////////////////

				// 드뎌 서버에 아템을 사용했다고 보낸다.
				//SendCS_IM_USEITEM_REQ( m_vecItem[i]->m_bSackCount+1, m_vecItem[i]->m_bSackPos, m_vecItem[i]->m_dwItemID);

				if(m_vecItem[i]->m_bItemType != ITEMTYPE_BOOK)	//HO_0329_07 무공 습득 여부 추가
					SendCS_IM_USEITEM_REQ( m_vecItem[i]->m_bSackCount+1, m_vecItem[i]->m_bSackPos, m_vecItem[i]->m_dwItemID);
				else
				{
					switch(g_MainCharInfo.m_bCharType)//HO_0725_07 흡성신공 : 동일유파무공 습득불가
					{
					case 1:
						if( m_vecItem[i]->m_dwMugongID == 191 || m_vecItem[i]->m_dwMugongID == 192 )
						{
							g_MainCharInfo.ShowHelpMessage(IDS_2TH_NOT_LEARNMUGONG, TEXTEFFECT_COLOR_WARNING);//유파 고유의 무공은 흡성신공으로 배우지 못합니다
							return FALSE;
						}
						break;
					case 2:
						if( m_vecItem[i]->m_dwMugongID == 193 || m_vecItem[i]->m_dwMugongID == 194 )
						{
							g_MainCharInfo.ShowHelpMessage(IDS_2TH_NOT_LEARNMUGONG, TEXTEFFECT_COLOR_WARNING);
							return FALSE;
						}
						break;
					case 3:
						if( m_vecItem[i]->m_dwMugongID == 195 || m_vecItem[i]->m_dwMugongID == 196 )
						{
							g_MainCharInfo.ShowHelpMessage(IDS_2TH_NOT_LEARNMUGONG, TEXTEFFECT_COLOR_WARNING);
							return FALSE;
						}
						break;
					case 4:
						if( m_vecItem[i]->m_dwMugongID == 197 || m_vecItem[i]->m_dwMugongID == 198 )
						{
							g_MainCharInfo.ShowHelpMessage(IDS_2TH_NOT_LEARNMUGONG, TEXTEFFECT_COLOR_WARNING);
							return FALSE;
						}
						break;
					}

				//	g_MainCharInfo.m_ReairSackID = m_vecItem[i]->m_bSackCount+1;//HO_0419_07 위에서 적용하였기에 주석처리
				//	g_MainCharInfo.m_RpairItemPos = m_vecItem[i]->m_bSackPos;//HO_0419_07 위에서 적용하였기에 주석처리
					g_MainCharInfo.m_dwResItemID = m_vecItem[i]->m_dwItemID;

					TCHAR szContent[1024] = {0,};
					LPCTSTR lpStrTemp = m_vecItem[i]->m_szName;
					_stprintf( szContent, IDS_FRAME_MUGONG_APPLY, lpStrTemp);

					g_pUIManager->ShowNotice( szContent, NOTICE_FRAME_OKCANCEL, NOTICE_FRAME_MUGONG);
				}

				// 각성신단 사용
				if(m_vecItem[i]->m_bItemType == ITEMTYPE_REBIRTH)
				{	
					g_MainCharInfo.m_ReairSackID = m_vecItem[i]->m_bSackCount+1;
					g_MainCharInfo.m_RpairItemPos = m_vecItem[i]->m_bSackPos; 
					g_MainCharInfo.m_dwResItemID = m_vecItem[i]->m_dwItemID;

					SendCS_IM_REBIRTH_REQ(g_MainCharInfo.m_ReairSackID,
								  g_MainCharInfo.m_RpairItemPos,
                                  g_MainCharInfo.m_dwResItemID);
//					g_pUIManager->ShowNotice(IDS_REBIRTH_CARE, NOTICE_FRAME_OKCANCEL, NOTICE_FRAME_REBIRTH);
//					g_MainCharInfo.m_bRebirthItem_Use = true;
				}
			}

			if( bUseCommonItemUseSound )
                g_MainCharInfo.PlayInterfaceSound( ISOUND_ITEM_USE);

			return TRUE;
		}
	}

	return FALSE;
}

/**
 *
 * \return 
 */
BOOL CSack::CheckItemUnSelected()
{
	// 자기 sack의 영역을 검사해서 있으면
	if( m_SackRt.PtInRect( XiahInput::g_ptMouse))
		return TRUE;
	else
		return FALSE;
}

/**
 *
 * \param nPosition 
 * \param pItem 
 */
void CSack::SetVB( BYTE nPosition, XiahItem::sItemInfo* pItem)
{
	VT_TLVertex Vertex[4];

	Vertex[ 0].pos = Vector4( m_vecItemRt[ nPosition]->left,  m_vecItemRt[ nPosition]->top, 0, 1);
	Vertex[ 1].pos = Vector4( m_vecItemRt[ nPosition]->right, m_vecItemRt[ nPosition]->top, 0, 1);
	Vertex[ 2].pos = Vector4( m_vecItemRt[ nPosition]->left,  m_vecItemRt[ nPosition]->bottom, 0, 1);
	Vertex[ 3].pos = Vector4( m_vecItemRt[ nPosition]->right, m_vecItemRt[ nPosition]->bottom, 0, 1);

	Vertex[ 0].diffuse = Vertex[ 1].diffuse = Vertex[ 2].diffuse = Vertex[ 3].diffuse = 0xffffffff;

	Vertex[ 0].tex = Vector2( 0, 0);
	Vertex[ 1].tex = Vector2( 1, 0);
	Vertex[ 2].tex = Vector2( 0, 1);
	Vertex[ 3].tex = Vector2( 1, 1);

	VOID* pVertices;
	if( !FAILED( m_vecItemVB[ nPosition]->Lock( 0, sizeof(Vertex), (void**)&pVertices, 0 )))
	{
		memcpy( pVertices, Vertex, sizeof(Vertex) );
		m_vecItemVB[ nPosition]->Unlock();
	}
	else
	{
		DBG_LogFile( _T("CSack::SetVB fail"));
	}

	// texture 세팅
	m_vecItemTex[nPosition] = Gettex(pItem->m_nResID);

	// 제련	
	if(pItem->m_bSocketCount)
	{
		float fHeight = 0;
		// 이번 리소스 제작자 x!!! 같다. 수없이 수정요청해도... 다른것도 제대로 넘겨주는게 없군.
		// 왜 리소스 크기가 지멋대로야. 짜증나서 내가 하고말지
		switch(pItem->m_bSocketCount)
		{
		case 1:
			fHeight = 12;
			break;
		case 2:
			fHeight = 28;
			break;
		case 3:
			fHeight = 44;
			break;
		}

		CreateSocketVB(nPosition, pItem);

		switch(pItem->m_bItemType)
		{
		case ITEMTYPE_WEAPON:
		case ITEMTYPE_CLOTH:
			{
				Vertex[0].pos = Vector4(m_vecItemRt[nPosition]->left+65, m_vecItemRt[nPosition]->bottom-fHeight , 0, 1);
				Vertex[1].pos = Vector4(m_vecItemRt[nPosition]->right,	 m_vecItemRt[nPosition]->bottom-fHeight, 0, 1);
				Vertex[2].pos = Vector4(m_vecItemRt[nPosition]->left+65, m_vecItemRt[nPosition]->bottom, 0, 1);
				//Vertex[ 3].pos = Vector4(m_vecItemRt[nPosition]->right, m_vecItemRt[nPosition]->bottom, 0, 1);

				if(!FAILED(m_vecItemSocketVB[nPosition]->Lock(0, sizeof(Vertex), (void**)&pVertices, 0)))
				{
					memcpy(pVertices, Vertex, sizeof(Vertex) );
					m_vecItemSocketVB[nPosition]->Unlock();
				}
				else
				{
					DBG_LogFile(_T("CSack::SetVB fail"));
				}

				if(pItem->m_bSocketItem[2])
				{
					Vertex[0].pos = Vector4(m_vecItemRt[nPosition]->left+67, m_vecItemRt[nPosition]->bottom - 42, 0, 1);
					Vertex[1].pos = Vector4(m_vecItemRt[nPosition]->right,	 m_vecItemRt[nPosition]->bottom - 42, 0, 1);
					Vertex[2].pos = Vector4(m_vecItemRt[nPosition]->left+67, m_vecItemRt[nPosition]->bottom - 32, 0, 1);
					Vertex[3].pos = Vector4(m_vecItemRt[nPosition]->right,	 m_vecItemRt[nPosition]->bottom - 32, 0, 1);
					if(!FAILED(m_vecSocketItem3VB[nPosition]->Lock(0, sizeof(Vertex), (void**)&pVertices, 0)))
					{
						memcpy(pVertices, Vertex, sizeof(Vertex) );
						m_vecSocketItem3VB[nPosition]->Unlock();
					}
				}

				if(pItem->m_bSocketItem[1])
				{
					Vertex[0].pos = Vector4(m_vecItemRt[nPosition]->left+67, m_vecItemRt[nPosition]->bottom - 26, 0, 1);
					Vertex[1].pos = Vector4(m_vecItemRt[nPosition]->right,	 m_vecItemRt[nPosition]->bottom - 26, 0, 1);
					Vertex[2].pos = Vector4(m_vecItemRt[nPosition]->left+67, m_vecItemRt[nPosition]->bottom - 16, 0, 1);
					Vertex[3].pos = Vector4(m_vecItemRt[nPosition]->right,	 m_vecItemRt[nPosition]->bottom - 16, 0, 1);
					if(!FAILED(m_vecSocketItem2VB[nPosition]->Lock(0, sizeof(Vertex), (void**)&pVertices, 0)))
					{
						memcpy(pVertices, Vertex, sizeof(Vertex) );
						m_vecSocketItem2VB[nPosition]->Unlock();
					}
				}				

				if(pItem->m_bSocketItem[0])
				{
					Vertex[0].pos = Vector4(m_vecItemRt[nPosition]->left+67, m_vecItemRt[nPosition]->bottom - 12, 0, 1);
					Vertex[1].pos = Vector4(m_vecItemRt[nPosition]->right,	 m_vecItemRt[nPosition]->bottom - 12, 0, 1);
					Vertex[2].pos = Vector4(m_vecItemRt[nPosition]->left+67, m_vecItemRt[nPosition]->bottom, 0, 1);
					Vertex[3].pos = Vector4(m_vecItemRt[nPosition]->right,	 m_vecItemRt[nPosition]->bottom, 0, 1);
					if(!FAILED(m_vecSocketItem1VB[nPosition]->Lock(0, sizeof(Vertex), (void**)&pVertices, 0)))
					{
						memcpy(pVertices, Vertex, sizeof(Vertex) );
						m_vecSocketItem1VB[nPosition]->Unlock();
					}
				}				
			}
			break;
		case ITEMTYPE_HAT:
		case ITEMTYPE_SHOE:
			{
				Vertex[0].pos = Vector4(m_vecItemRt[nPosition]->right-17, m_vecItemRt[nPosition]->bottom-fHeight+2, 0, 1);
				Vertex[1].pos = Vector4(m_vecItemRt[nPosition]->right-5,  m_vecItemRt[nPosition]->bottom-fHeight+2, 0, 1);
				Vertex[2].pos = Vector4(m_vecItemRt[nPosition]->right-17, m_vecItemRt[nPosition]->bottom+2, 0, 1);
				Vertex[3].pos = Vector4(m_vecItemRt[nPosition]->right-5,  m_vecItemRt[nPosition]->bottom+2, 0, 1);

				if(!FAILED(m_vecItemSocketVB[nPosition]->Lock(0, sizeof(Vertex), (void**)&pVertices, 0)))
				{
					memcpy(pVertices, Vertex, sizeof(Vertex) );
					m_vecItemSocketVB[nPosition]->Unlock();
				}
				else
				{
					DBG_LogFile(_T("CSack::SetVB fail"));
				}

				/////////////////////////////////////////////////////////////////////////////////////////////////////

				if(pItem->m_bSocketItem[2])
				{
					Vertex[0].pos = Vector4(m_vecItemRt[nPosition]->right-17, m_vecItemRt[nPosition]->bottom - 42 + 2, 0, 1);
					Vertex[1].pos = Vector4(m_vecItemRt[nPosition]->right-5, m_vecItemRt[nPosition]->bottom - 42 + 2, 0, 1);
					Vertex[2].pos = Vector4(m_vecItemRt[nPosition]->right-17, m_vecItemRt[nPosition]->bottom - 32 + 2, 0, 1);
					Vertex[3].pos = Vector4(m_vecItemRt[nPosition]->right-5, m_vecItemRt[nPosition]->bottom - 32 + 2, 0, 1);
					if(!FAILED(m_vecSocketItem3VB[nPosition]->Lock(0, sizeof(Vertex), (void**)&pVertices, 0)))
					{
						memcpy(pVertices, Vertex, sizeof(Vertex) );
						m_vecSocketItem3VB[nPosition]->Unlock();
					}
				}

				if(pItem->m_bSocketItem[1])
				{
					Vertex[0].pos = Vector4(m_vecItemRt[nPosition]->right-17, m_vecItemRt[nPosition]->bottom - 26 + 2, 0, 1);
					Vertex[1].pos = Vector4(m_vecItemRt[nPosition]->right-5, m_vecItemRt[nPosition]->bottom - 26 + 2, 0, 1);
					Vertex[2].pos = Vector4(m_vecItemRt[nPosition]->right-17, m_vecItemRt[nPosition]->bottom - 16 + 2, 0, 1);
					Vertex[3].pos = Vector4(m_vecItemRt[nPosition]->right-5, m_vecItemRt[nPosition]->bottom - 16 + 2, 0, 1);
					if(!FAILED(m_vecSocketItem2VB[nPosition]->Lock(0, sizeof(Vertex), (void**)&pVertices, 0)))
					{
						memcpy(pVertices, Vertex, sizeof(Vertex) );
						m_vecSocketItem2VB[nPosition]->Unlock();
					}
				}				

				if(pItem->m_bSocketItem[0])
				{
					Vertex[0].pos = Vector4(m_vecItemRt[nPosition]->right-17, m_vecItemRt[nPosition]->bottom - 12 + 2, 0, 1);
					Vertex[1].pos = Vector4(m_vecItemRt[nPosition]->right-5, m_vecItemRt[nPosition]->bottom - 12 + 2, 0, 1);
					Vertex[2].pos = Vector4(m_vecItemRt[nPosition]->right-17, m_vecItemRt[nPosition]->bottom + 2, 0, 1);
					Vertex[3].pos = Vector4(m_vecItemRt[nPosition]->right-5, m_vecItemRt[nPosition]->bottom + 2, 0, 1);
					if(!FAILED(m_vecSocketItem1VB[nPosition]->Lock(0, sizeof(Vertex), (void**)&pVertices, 0)))
					{
						memcpy(pVertices, Vertex, sizeof(Vertex) );
						m_vecSocketItem1VB[nPosition]->Unlock();
					}
				}

			}
			break;
		default:
			break;
		}
	}
	//HT_1116 : 각성자 아이템 추가
	if(pItem->m_wRBSocketItem != 0)
	{
		float fHeight = 12;
		// 이번 리소스 제작자 x!!! 같다. 수없이 수정요청해도... 다른것도 제대로 넘겨주는게 없군.
		// 왜 리소스 크기가 지멋대로야. 짜증나서 내가 하고말지

		CreateSocketVB(nPosition, pItem);

		switch(pItem->m_bItemType)
		{
		case ITEMTYPE_WEAPON:
		case ITEMTYPE_CLOTH:
			{
				Vertex[0].pos = Vector4(m_vecItemRt[nPosition]->left,  m_vecItemRt[nPosition]->bottom-12 , 0, 1);
				Vertex[1].pos = Vector4(m_vecItemRt[nPosition]->left+13, m_vecItemRt[nPosition]->bottom-12, 0, 1);
				Vertex[2].pos = Vector4(m_vecItemRt[nPosition]->left,  m_vecItemRt[nPosition]->bottom, 0, 1);
				Vertex[3].pos = Vector4(m_vecItemRt[nPosition]->left+13, m_vecItemRt[nPosition]->bottom, 0, 1);

				if(!FAILED(m_vecRBSocketItemVB[nPosition]->Lock(0, sizeof(Vertex), (void**)&pVertices, 0)))
				{
					memcpy(pVertices, Vertex, sizeof(Vertex) );
					m_vecRBSocketItemVB[nPosition]->Unlock();
				}
				else
				{
					DBG_LogFile(_T("CSack::SetVB fail"));
				}
				
				if(pItem->m_wRBSocketItem > 1)
				{
					Vertex[0].pos = Vector4(m_vecItemRt[nPosition]->left+2,  m_vecItemRt[nPosition]->bottom-12 , 0, 1);
					Vertex[1].pos = Vector4(m_vecItemRt[nPosition]->left+12, m_vecItemRt[nPosition]->bottom-12, 0, 1);
					Vertex[2].pos = Vector4(m_vecItemRt[nPosition]->left+2,  m_vecItemRt[nPosition]->bottom, 0, 1);
					Vertex[3].pos = Vector4(m_vecItemRt[nPosition]->left+12, m_vecItemRt[nPosition]->bottom, 0, 1);

					if(!FAILED(m_vecRBItemStoneVB[nPosition]->Lock(0, sizeof(Vertex), (void**)&pVertices, 0)))
					{
						memcpy(pVertices, Vertex, sizeof(Vertex) );
						m_vecRBSocketItemVB[nPosition]->Unlock();
					}
					else
					{
						DBG_LogFile(_T("CSack::SetVB fail"));
					}
				}
			}
			break;
		case ITEMTYPE_HAT:
		case ITEMTYPE_SHOE:
			{
				Vertex[0].pos = Vector4(m_vecItemRt[nPosition]->left+2,  m_vecItemRt[nPosition]->bottom-12 , 0, 1);
				Vertex[1].pos = Vector4(m_vecItemRt[nPosition]->left+12, m_vecItemRt[nPosition]->bottom-12, 0, 1);
				Vertex[2].pos = Vector4(m_vecItemRt[nPosition]->left+2,  m_vecItemRt[nPosition]->bottom, 0, 1);
				Vertex[3].pos = Vector4(m_vecItemRt[nPosition]->left+12, m_vecItemRt[nPosition]->bottom, 0, 1);

				if(!FAILED(m_vecRBSocketItemVB[nPosition]->Lock(0, sizeof(Vertex), (void**)&pVertices, 0)))
				{
					memcpy(pVertices, Vertex, sizeof(Vertex) );
					m_vecRBSocketItemVB[nPosition]->Unlock();
				}
				else
				{
					DBG_LogFile(_T("CSack::SetVB fail"));
				}

				if(pItem->m_wRBSocketItem > 1)
				{
					Vertex[0].pos = Vector4(m_vecItemRt[nPosition]->left+2,  m_vecItemRt[nPosition]->bottom-10 , 0, 1);
					Vertex[1].pos = Vector4(m_vecItemRt[nPosition]->left+12, m_vecItemRt[nPosition]->bottom-10, 0, 1);
					Vertex[2].pos = Vector4(m_vecItemRt[nPosition]->left+2,  m_vecItemRt[nPosition]->bottom, 0, 1);
					Vertex[3].pos = Vector4(m_vecItemRt[nPosition]->left+12, m_vecItemRt[nPosition]->bottom, 0, 1);

					if(!FAILED(m_vecRBItemStoneVB[nPosition]->Lock(0, sizeof(Vertex), (void**)&pVertices, 0)))
					{
						memcpy(pVertices, Vertex, sizeof(Vertex) );
						m_vecRBSocketItemVB[nPosition]->Unlock();
					}
					else
					{
						DBG_LogFile(_T("CSack::SetVB fail"));
					}
				}

				/////////////////////////////////////////////////////////////////////////////////////////////////////
				break;
			default:
				break;
			}	
		}
	}
}

/**
 *
 */
void CSack::RefreshSackPos()
{
	if(m_bySackType == SACKTYPE__PET_EQUIP) // 이건 몰라 이상한 구조라서 몰라.
		SetSackRegion();

	for( int i=0; i < m_bySackTotalSize; ++i)
	{
		if( m_vecItem[ i])
		{
			SetSackRegion();
			SetVB( i, m_vecItem[ i]);
		}
	}
}

/**
 *
 */
void CSack::SetSackRegion()
{
	return;
}

/**
 *
 * \param nPosition 
 * \return 
 */
XiahItem::sItemInfo* CSack::FindSackItemByPos( int nPosition)
{	
	return NULL;
}

/**
 *
 * \param nPosition 
 * \return 
 */
XiahItem::sItemInfo* CSack::FindSackItemByPosPrev( int nPosition)
{	
	return NULL;
}

/**
 *
 * \param nID 
 * \return 
 */
XiahItem::sItemInfo* CSack::FindSackItemByID( int nID)
{
	for( int i=0; i < m_bySackTotalSize; ++i)
	{
		if( m_vecItem[ i])
		{
			if( m_vecItem[ i]->m_dwItemID == nID)
				return m_vecItem[ i];
		}
	}

	return NULL;
}

/**
 *
 * \param nVisualID 
 * \return 
 */
XiahItem::sItemInfo* CSack::FindSackItemByVisualID( int nVisualID)
{
	for( int i=0; i < m_bySackTotalSize; ++i)
	{
		if( m_vecItem[ i])
		{
			if( m_vecItem[ i]->m_wVisualID == nVisualID)
				return m_vecItem[ i];
		}
	}

	return NULL;
}

/**
 *
 * \param nVisualID 
 * \return 
 */
int CSack::HowManyItem( int nVisualID)
{
	int nAmount = 0;

	for( int i=0; i < m_bySackTotalSize; ++i)
	{
		if( m_vecItem[ i] && m_vecItem[ i]->m_wVisualID == nVisualID)
		{
			XiahItem::sItemInfo* pItem = m_vecItem[ i];

			if(pItem)
			{
				nAmount += pItem->m_dwAmount;
			}
			else
			{
				assert(0);
			}			
		}
	}

	return nAmount;
}
