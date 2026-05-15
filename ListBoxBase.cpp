/*===================================================================================================
										ListBoxBase.cpp
-----------------------------------------------------------------------------------------------------
	date :	2004/05/19  16:54
  Author :	
	
 Purpose :	리스트 베이스
			현재 인터페이스 엔진상 리스트 박스를 넣지 못해서 클라이언트에 붙였음.
			(인터페이스 툴을 예전것을 사용하기에...)
	
===================================================================================================*/
#include "precompile.h"
#include ".\listboxbase.h"

CListBoxBase::CListBoxBase(void) : m_nCount(0), m_nType(0), m_pSelectVB(NULL), m_bSelect(false), m_dwSelect(0)
{
	m_mList.clear();

	g_pDirect3DDevice->CreateVertexBuffer(4*sizeof(VT_TLVertex), 0, D3DFVF_TLVERTEX, D3DPOOL_MANAGED, &m_pSelectVB, NULL);
}

CListBoxBase::~CListBoxBase(void)
{
	std::map<DWORD, ListData*>::iterator iter = m_mList.begin();

	for(; iter != m_mList.end(); ++iter)
	{
		ListData* pListData = iter->second;

		if(pListData)
			delete pListData, pListData = NULL;
	}

	m_mList.clear();

	if(m_pSelectVB)
		m_pSelectVB->Release();
}


/**
 *
 */
void CListBoxBase::Render()
{
	register std::map<DWORD, ListData*>::iterator iter = m_mList.begin();

	for(; iter != m_mList.end(); ++iter)
	{
		ListData *pList = (*iter).second;

		pList->m_Text.Render();
	} // for(; iter != m_mList.end(); ++iter)

	if(m_nCount)
	{
		g_pDirect3DDevice->SetRenderState(D3DRS_ZENABLE, FALSE);
		g_pDirect3DDevice->SetRenderState(D3DRS_FOGENABLE, FALSE);

		g_pDirect3DDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
		g_pDirect3DDevice->SetRenderState(D3DRS_DIFFUSEMATERIALSOURCE, D3DMCS_COLOR1);
		g_pDirect3DDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
		g_pDirect3DDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

		g_pDirect3DDevice->SetSamplerState(0,  D3DSAMP_ADDRESSU , D3DTADDRESS_CLAMP);
		g_pDirect3DDevice->SetSamplerState(0,  D3DSAMP_ADDRESSV , D3DTADDRESS_CLAMP);

		g_Device.SetTexture(0, NULL);
		//g_pDirect3DDevice->SetTexture(0, NULL);
		g_Device.SetStreamSource( m_pSelectVB, sizeof(VT_TLVertex));
		g_Device.SetFVF(D3DFVF_TLVERTEX);
		//g_pDirect3DDevice->SetFVF(D3DFVF_TLVERTEX);
		g_pDirect3DDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);	
	}
}

/**
 *
 */
void CListBoxBase::Refresh()
{
	std::map<DWORD, ListData*>::iterator iter = m_mList.begin();

	for(register int i=0; iter != m_mList.end(); ++iter, ++i)
	{
		ListData *pList = (*iter).second;

		pList->m_rtZone.left	= m_ptPos.x;
		pList->m_rtZone.right	= m_ptPos.x + m_Size.cx;
		pList->m_rtZone.top		= m_ptPos.y + m_Size.cy * i;
		pList->m_rtZone.bottom	= pList->m_rtZone.top + m_Size.cy;

		pList->m_Text.SetParentRect(&pList->m_rtZone);
	}
}

/**
 *
 * \return 
 */
const bool CListBoxBase::CheckList()
{
	if(XiahInput::g_bLButtonDown)
	{
		register std::map<DWORD, ListData*>::iterator iter = m_mList.begin();

		for(; iter != m_mList.end(); ++iter)
		{
			ListData *pList = (*iter).second;

			if(pList->m_rtZone.PtInRect(XiahInput::g_ptMouse))
			{
				m_dwSelect = (*iter).first;

				MakeSelectVB(pList->m_rtZone);

				m_bSelect = true;

				return true;
			}
		}
	}

	return false;
}


/**
 *
 * \param rtRect 
 */
void CListBoxBase::MakeSelectVB(sRect rtRect)
{
	D3DCOLOR d3dcolor =  0x55999999; //D3DCOLOR_ARGB(120, 215, 121, 179);

	VT_TLVertex Vertex[4];

	Vertex[ 0].pos = Vector4(rtRect.left-5,	rtRect.top-3,		0, 1);
	Vertex[ 1].pos = Vector4(rtRect.right,	rtRect.top-3,		0, 1);
	Vertex[ 2].pos = Vector4(rtRect.left-5, rtRect.bottom-5,	0, 1);
	Vertex[ 3].pos = Vector4(rtRect.right,	rtRect.bottom-5,	0, 1);

	Vertex[ 0].diffuse = Vertex[ 1].diffuse = Vertex[ 2].diffuse = Vertex[ 3].diffuse = d3dcolor;

	Vertex[ 0].tex = Vector2(0, 0);
	Vertex[ 1].tex = Vector2(1, 0);
	Vertex[ 2].tex = Vector2(0, 1);
	Vertex[ 3].tex = Vector2(1, 1);

	VOID* pVertices;
	if(!FAILED(m_pSelectVB->Lock( 0, sizeof(Vertex), (void**)&pVertices, 0)))
	{
		memcpy(pVertices, Vertex, sizeof(Vertex) );
		m_pSelectVB->Unlock();
	}
}


/**
 *
 * \param nPosX 
 * \param nPosY 
 * \param nWidth 
 * \param nHeight 
 */
void CListBoxBase::Set(const long nPosX, const long nPosY, const long nWidth, const long nHeight, const int nType)
{
	m_ptPos.x	= nPosX;
	m_ptPos.y	= nPosY;

	m_Size.cx	= nWidth;
	m_Size.cy	= nHeight;

	m_nType		= nType;
}

/**
 *
 * \param dwID 
 * \param strTemp 
 */
void CListBoxBase::AddString(const DWORD dwID, sString strTemp)
{
	ListData *pInfo = new ListData;

	pInfo->m_Text.SetText(0,0, (LPCTSTR)strTemp, GetFont(IDS_DUDUM, 12), D3DCOLOR_XRGB(255,255,255));

	m_mList.insert(std::map<DWORD, ListData*>::value_type(dwID, pInfo));

	++m_nCount;

	Refresh();
}


DWORD CListBoxBase::DelString(const DWORD dwID, const int nType)
{
	DWORD dwTemp = m_dwSelect;
	DWORD dwFind = 0;

	if(nType)
	{
		dwFind = m_dwSelect;

		m_dwSelect = 0;
	}
	else
	{
		dwFind = dwID;

		if(m_dwSelect == dwID)
			m_dwSelect = 0;

		dwTemp = dwID;
	}

	std::map<DWORD, ListData*>::iterator iter = m_mList.find(dwFind);

	if(iter != m_mList.end())
	{
		ListData *pInfo = iter->second;

		if(pInfo)
			delete pInfo, pInfo = NULL;

		m_mList.erase(iter);
	}


	--m_nCount;

	Refresh();

	return dwTemp;
}