#include "precompile.h"
#include "ceffect_spot.h"


// 화면 번쩍효과를 위한 클래스
cEFFECT_SPOT	*g_effect_spot;

//////////////////////////////////////////////////////////////////////////

cEFFECT_SPOT::cEFFECT_SPOT()
{
	AxisTime = 0;
	FadeTimeLength = 0;
	m_VB = NULL;

	g_pDirect3DDevice->CreateVertexBuffer( 4*sizeof(VT_TLVertex), 0 ,D3DFVF_TLVERTEX, D3DPOOL_MANAGED, &m_VB, NULL );
	VT_TLVertex* pVertex = NULL;
	m_VB->Lock( 0, 0, (void**)&pVertex, 0 );
	pVertex[0].pos = Vector4( 0 - 0.5f, 769 - 0.5f, 0, 1);
	pVertex[1].pos = Vector4( 0 - 0.5f, 0 - 0.5f, 0, 1);
	pVertex[2].pos = Vector4( 1025 - 0.5f, 769 - 0.5f, 0, 1);
	pVertex[3].pos = Vector4( 1025 - 0.5f, 0 - 0.5f, 0, 1);
	m_VB->Unlock();
}

cEFFECT_SPOT::~cEFFECT_SPOT()
{
	if( m_VB ) 
		m_VB->Release();
	m_VB = NULL;
}

//////////////////////////////////////////////////////////////////////////

BOOL cEFFECT_SPOT::Start(D3DCOLOR color,DWORD time)
{
	AxisTime = timeGetTime();
	FadeTimeLength = time;
	FadeColor = color;
	return TRUE;
}


// UPDATE & RENDER
// 한번에 하나 밖에 렌더가 안된다
BOOL cEFFECT_SPOT::Update()
{
	float detalTime = timeGetTime() - AxisTime;
	float FadeAlpha;
	DWORD dwAlpha;	

	// 시간이 넘었다. 끝내자.
	if(detalTime > FadeTimeLength) return FALSE;

	//FadeAlpha = (float)detalTime / (float)FadeTimeLength;	// fade in
	FadeAlpha = 1.0f - (float)detalTime / (float)FadeTimeLength;	// fade out

	if( FadeAlpha > 1.0f) FadeAlpha = 1.0f;
	if( FadeAlpha < 0.f) FadeAlpha = 0.f;
	
	dwAlpha = (BYTE)(255 * FadeAlpha) << 24;

	VT_TLVertex* pVertex = NULL;
	m_VB->Lock( 0, 0, (void**)&pVertex, 0 );
	pVertex[0].diffuse = (FadeColor & 0xFFFFFF) + dwAlpha;
	pVertex[1].diffuse = (FadeColor & 0xFFFFFF) + dwAlpha;
	pVertex[2].diffuse = (FadeColor & 0xFFFFFF) + dwAlpha;
	pVertex[3].diffuse = (FadeColor & 0xFFFFFF) + dwAlpha;
	m_VB->Unlock();
	
	// 렌더한다
	Render();
	
	return TRUE;
}

void cEFFECT_SPOT::Render()
{
	g_pDirect3DDevice->SetRenderState( D3DRS_FOGENABLE, FALSE);
	g_pDirect3DDevice->SetRenderState( D3DRS_LIGHTING, FALSE);
	g_pDirect3DDevice->SetRenderState( D3DRS_ALPHABLENDENABLE, TRUE);
	g_pDirect3DDevice->SetRenderState( D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	g_pDirect3DDevice->SetRenderState( D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
	g_pDirect3DDevice->SetRenderState( D3DRS_DIFFUSEMATERIALSOURCE, D3DMCS_COLOR1);

	g_Device.SetStreamSource( m_VB, sizeof(VT_TLVertex) );
	g_Device.SetTexture(0, NULL);
	//g_pDirect3DDevice->SetTexture( 0, NULL);
	g_Device.SetFVF(D3DFVF_TLVERTEX);
	//g_pDirect3DDevice->SetFVF( D3DFVF_TLVERTEX);
	g_pDirect3DDevice->DrawPrimitive( D3DPT_TRIANGLESTRIP, 0, 2);

	g_pDirect3DDevice->SetRenderState( D3DRS_LIGHTING, TRUE);
	g_pDirect3DDevice->SetRenderState( D3DRS_FOGENABLE, TRUE);
}

////////////////////////////////////////////////////////////////////////////////

/*

cTITLE::cTITLE()
{
	m_pTexture = NULL;
	m_VB = NULL;
}

cTITLE::~cTITLE()
{


}

BOOL cTITLE::Create()
{
	g_pDirect3DDevice->CreateVertexBuffer( 4*sizeof(VT_TLVertex), 0 ,D3DFVF_TLVERTEX, D3DPOOL_MANAGED, &m_VB, NULL );
	VT_TLVertex* pVertex = NULL;
	m_VB->Lock( 0, 0, (void**)&pVertex, 0 );
	pVertex[0].pos = Vector4( 0 - 0.5f, 769 - 0.5f, 0, 1);
	pVertex[1].pos = Vector4( 0 - 0.5f, 0 - 0.5f, 0, 1);
	pVertex[2].pos = Vector4( 1025 - 0.5f, 769 - 0.5f, 0, 1);
	pVertex[3].pos = Vector4( 1025 - 0.5f, 0 - 0.5f, 0, 1);
	m_VB->Unlock();
	return TRUE;
}

BOOL cTITLE::Render()
{


	return TRUE;
}


BOOL cTITLE::Destroy()
{
	if(m_pTexture)
	{
		m_pTexture->Release();
		m_pTexture = NULL;
	}

	if(m_VB)
	{
		m_VB->Release();
		m_VB = NULL;
	}
	return TRUE;
}


BOOL cTITLE::ChangeTexture(LPDIRECT3DTEXTURE9 pTexture)
{
	if(NULL == pTexture)
	{
		return FALSE;
	}
	else
		m_pTexture = pTexture;

	return TRUE;
}
*/

//////////////////////////////////////////////////////////////////////////

cTRANS_BOX::cTRANS_BOX()
{
	top = bottom = left = right = 0.0f;
	m_VB = NULL;
}

cTRANS_BOX::~cTRANS_BOX()
{

}


BOOL cTRANS_BOX::Create()
{

	return TRUE;
}

BOOL cTRANS_BOX::Destroy()
{

	return TRUE;
}

	


//////////////////////////////////////////////////////////////////////////

// 번쩍 효과를 만든다
BOOL	Make_Special_Effect(D3DCOLOR	col)
{
	if(NULL == g_effect_spot)
	{
		g_effect_spot = new cEFFECT_SPOT;
		g_effect_spot ->Start(col);
	}
	return TRUE;
}

// 화면 번쩍이라던가 그런것들을 Update 한다
BOOL	Update_Special_Effect()
{
	// 화면 번쩍!
	if(g_effect_spot)
	{
		if(g_effect_spot->Update() == FALSE) 
		{
			delete 	g_effect_spot;
			g_effect_spot = NULL;
		}
	}

	return TRUE;
}

