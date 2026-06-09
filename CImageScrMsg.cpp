#include "precompile.h"
#include ".\cimagescrmsg.h"
#include "XiahObject.h"
#include "InterfaceDefine.h"
#include "XiahGame_Handler_Sender.h"
#include "CharacterInfo.h"

CImageScrMsg::CImageScrMsg(void) : m_nPrevToolTipPos(-1), m_nImageWidth(35), m_nImageHeight(35), m_nPosX(9), m_nPosY(239), m_nCount(0),	m_eType(LEFT)
{
	for(register int i=0; i < MAX_IMGMSG_SIZE; ++i)
	{
		m_pScrImageVB[i] = NULL;
	}

	SetVB();

	m_mTexList.clear();
}

CImageScrMsg::~CImageScrMsg(void)
{
	for(register int i=0; i < MAX_IMGMSG_SIZE; ++i)
	{
		m_pScrImageVB[i]->Release();
		m_pScrImageVB[i] = NULL;
	}

	map<int, LPDIRECT3DTEXTURE9>::iterator iter = m_mTexList.begin();

	/*
	for(; iter != m_mTexList.end(); ++iter)
	{
		if(iter == m_mTexList.end())
			continue;

		LPDIRECT3DTEXTURE9 pTexture = (*iter).second;

		//pTexture->Release();
	}*/

	m_mTexList.clear();
	m_ScrMsgList.AllDeleteMsg();
}

void CImageScrMsg::SetVB()
{
	for(register int i=0; i < MAX_IMGMSG_SIZE; ++i)
	{
		if(!m_pScrImageVB[i])
			g_pDirect3DDevice->CreateVertexBuffer( 4*sizeof(VT_TLVertex), 0, D3DFVF_TLVERTEX,
			D3DPOOL_MANAGED, &m_pScrImageVB[i], NULL);
		
		m_rtImageRegion[i].left = m_nPosX;
		m_rtImageRegion[i].right = m_nPosX + m_nImageWidth;
		m_rtImageRegion[i].top = m_nPosY + 10 +i * m_nImageHeight;
		m_rtImageRegion[i].bottom = m_rtImageRegion[i].top + m_nImageHeight;


		D3DCOLOR d3dcolor = 0xffffffff;

		VT_TLVertex Vertex[4];

		Vertex[ 0].pos = Vector4( (float)m_rtImageRegion[i].left-0.5f, (float)m_rtImageRegion[i].top-0.5f, 0, 1);
		Vertex[ 1].pos = Vector4( (float)m_rtImageRegion[i].right-0.5f, (float)m_rtImageRegion[i].top-0.5f, 0, 1);
		Vertex[ 2].pos = Vector4( (float)m_rtImageRegion[i].left-0.5f, (float)m_rtImageRegion[i].bottom-0.5f, 0, 1);
		Vertex[ 3].pos = Vector4( (float)m_rtImageRegion[i].right-0.5f, (float)m_rtImageRegion[i].bottom-0.5f, 0, 1);

		Vertex[ 0].diffuse = Vertex[ 1].diffuse = Vertex[ 2].diffuse = Vertex[ 3].diffuse = d3dcolor;

		Vertex[ 0].tex = Vector2( 0, 0);
		Vertex[ 1].tex = Vector2( 1, 0);
		Vertex[ 2].tex = Vector2( 0, 1);
		Vertex[ 3].tex = Vector2( 1, 1);

		VOID* pVertices;
		if( !FAILED( m_pScrImageVB[i]->Lock( 0, sizeof(Vertex), (void**)&pVertices, 0 )))
		{
			memcpy( pVertices, Vertex, sizeof(Vertex) );
			m_pScrImageVB[i]->Unlock();
		}
	}
}

void CImageScrMsg::AddTexture(const int nResID)
{
	m_mTexList.insert(map<int, LPDIRECT3DTEXTURE9>::value_type(nResID, XiahPak::GetTexture(nResID, true)));
}

void CImageScrMsg::SetScrMsg(const int nID, sString sContent, BYTE byColorType)
{
	//SetVB();

	if(m_ScrMsgList.size() >= 20)
		m_ScrMsgList.DeleteFront();

	m_ScrMsgList.AddScrMsg(nID, sContent, byColorType);
	UpdateTex();
}

void CImageScrMsg::SetScrImg(const int nID, const int nResID, const DWORD dwEventCharID)
{
	if(m_ScrMsgList.size() >= 20)
		m_ScrMsgList.DeleteFront();

	m_ScrMsgList.AddScrImg(nID, nResID, dwEventCharID);
}

void CImageScrMsg::DelScrMsg(const int nID)
{
	m_ScrMsgList.DelScrMsg(nID);
}

void CImageScrMsg::UpdateScrMsg()
{

}

void CImageScrMsg::UpdateTex()
{
	RECT rtRect;

	if(m_eType == LEFT)
	{
		rtRect.left		= m_nPosX + m_nImageWidth + 10;
		rtRect.right	= 300;
		rtRect.top		= m_nPosY + 10;
		rtRect.bottom	= rtRect.top + m_nImageHeight;
	}
	else
	{
		rtRect.left		= m_nPosX - 150;
		rtRect.right	= 300;
		rtRect.top		= m_nPosY + 10;
		rtRect.bottom	= rtRect.top + m_nImageHeight;
	}	

	CImageScrMsgList::iterator iter;
	register int i=0;

	for(iter = m_ScrMsgList.begin(); iter != m_ScrMsgList.end(); ++iter)
	{
		sImageScrMsg* pMsg = (*iter).second;

		if(!pMsg->bShow)
			continue;

		m_rtRegion[i].left = rtRect.left;
		m_rtRegion[i].right = rtRect.right;
		m_rtRegion[i].top = rtRect.top + m_nImageHeight * i;
		m_rtRegion[i].bottom = rtRect.bottom + m_nImageHeight * i;

		m_text2D[i].SetParentRect( &m_rtRegion[i]);

		m_text2D[i].SetText(0, 0, (LPCTSTR)pMsg->str, GetFont( IDS_DUDUM, 12), D3DCOLOR_XRGB( 255, 255, 255));

		++i;
	}

	ScrMsgShow();
}

void CImageScrMsg::ScrMsgShow()
{
	//int nCount = m_ScrMsgList.size();

	CImageScrMsgList::iterator iterMsg;
	register int i=0;

	for(iterMsg = m_ScrMsgList.begin(); iterMsg != m_ScrMsgList.end(); ++iterMsg)
	{
		sImageScrMsg* pMsg = (*iterMsg).second;

		if(!pMsg->bShow)
			continue;

		if(pMsg->nResID == 0)
			continue;

		map<int, LPDIRECT3DTEXTURE9>::iterator iter = m_mTexList.find(pMsg->nResID);
		
		if(iter == m_mTexList.end())
			continue;

		LPDIRECT3DTEXTURE9 pTexture = (*iter).second;

		g_pDirect3DDevice->SetRenderState( D3DRS_ZENABLE, FALSE);
		g_pDirect3DDevice->SetRenderState( D3DRS_FOGENABLE, FALSE);

		g_pDirect3DDevice->SetRenderState( D3DRS_ALPHABLENDENABLE, FALSE);

		g_Device.SetTexture(0, pTexture);
		//g_pDirect3DDevice->SetTexture( 0, pTexture);

		g_Device.SetStreamSource( m_pScrImageVB[i], sizeof(VT_TLVertex));
		g_Device.SetFVF(D3DFVF_TLVERTEX);
		//g_pDirect3DDevice->SetFVF( D3DFVF_TLVERTEX);
		g_pDirect3DDevice->DrawPrimitive( D3DPT_TRIANGLESTRIP, 0, 2);

		m_text2D[i].Render();

		++i;
	}	

	i = 0;
	for(iterMsg = m_ScrMsgList.begin(); iterMsg != m_ScrMsgList.end(); ++iterMsg)
	{
		sImageScrMsg* pMsg = (*iterMsg).second;

		if(!pMsg->bShow)
			continue;

		DrawToolTip(i);

		++i;
	}
}

void CImageScrMsg::DrawToolTip(const int nPos)
{
	if(m_rtImageRegion[nPos].PtInRect(XiahInput::g_ptMouse))
	{
		if(nPos != m_nPrevToolTipPos)
		{
			m_nPrevToolTipPos = nPos;

			if(m_eType == LEFT)
				SetToolTip(nPos);
		}

		if(m_eType == LEFT)
			g_MainCharInfo.m_pToolTip->Draw();
	}
	else
	{
		if(m_nPrevToolTipPos == nPos)
			m_nPrevToolTipPos = -1;
	}
}

void CImageScrMsg::SetToolTip(const int nPos)
{
	int i = 0;
	CImageScrMsgList::iterator iterMsg;
	for(iterMsg = m_ScrMsgList.begin(); iterMsg != m_ScrMsgList.end(); ++iterMsg)
	{
		sImageScrMsg* pMsg = (*iterMsg).second;

		if(!pMsg->bShow)
			continue;

		if(i == nPos)
		{
			sRect rtImageRegion = m_rtImageRegion[nPos];
			rtImageRegion.left += 35;
			rtImageRegion.right += 160;

			rtImageRegion.top += 15;
			rtImageRegion.bottom += 15;

			map<int, sString>::iterator iter = pMsg->mStrToolTip.begin();

			sString strTemp = (*iter).second;
			g_MainCharInfo.m_pToolTip->SetToolTip(8, &rtImageRegion, 1, strTemp.data(), D3DCOLOR_XRGB(255, 255, 255), 2);

			for(++iter; iter != pMsg->mStrToolTip.end(); ++iter)
			{
				strTemp = (*iter).second;
				g_MainCharInfo.m_pToolTip->AddToolTip(strTemp.data(), 12, D3DCOLOR_XRGB(255, 255, 255));
			}
			return;
		}

		++i;
	}

}

void CImageScrMsg::DeleteScrMsgByTime()
{
	BYTE bySize = m_ScrMsgList.size();
	if(bySize > 0)	
		m_ScrMsgList.DeleteFront();

	UpdateTex();
}

void CImageScrMsg::AllDeleteScrMsg()
{
	BYTE bySize = m_ScrMsgList.size();

	if( bySize > 0)	
		m_ScrMsgList.AllDeleteMsg();

	UpdateTex();
}

void CImageScrMsg::AddToolTip(const int nID, sString strData)
{
	m_ScrMsgList.AddToolTip(nID, strData);
}

void CImageScrMsg::SetToolTip(const int nID, const int nLine, sString strData)
{
	m_ScrMsgList.SetToolTip(nID, nLine, strData);
}

void CImageScrMsg::Show(const int nID)
{
	CImageScrMsgList::iterator iterMsg = m_ScrMsgList.find(nID);

	if(iterMsg == m_ScrMsgList.end())
		return;

	sImageScrMsg* pMsg = (*iterMsg).second;
	pMsg->bShow = true;

	UpdateTex();

	// temp
	++m_nCount;

	g_MainCharInfo.m_bEvSocketItemUse = true;
}

void CImageScrMsg::Hide(const int nID)
{
	CImageScrMsgList::iterator iterMsg = m_ScrMsgList.find(nID);

	if(iterMsg == m_ScrMsgList.end())
		return;

	sImageScrMsg* pMsg = (*iterMsg).second;
	pMsg->bShow = false;
	pMsg->dwEventCharID = 0;

	UpdateTex();

	// temp
	--m_nCount;

	if(m_nCount == 0)
		g_MainCharInfo.m_bEvSocketItemUse = false;
}

void CImageScrMsg::AllHide()
{
	CImageScrMsgList::iterator iterMsg;
	for(iterMsg = m_ScrMsgList.begin(); iterMsg != m_ScrMsgList.end(); ++iterMsg)
	{
		if(iterMsg == m_ScrMsgList.end())
			continue;

		sImageScrMsg* pMsg = (*iterMsg).second;

		pMsg->bShow = false;
	}
	UpdateTex();
}

void CImageScrMsg::DelToolTip(const int nID)
{
	m_ScrMsgList.DelToolTip(nID);
}