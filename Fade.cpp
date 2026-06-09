#include "precompile.h"
#include "Fade.h"

namespace Fade
{
	LPDIRECT3DVERTEXBUFFER9	m_VB;

	D3DCOLOR	g_FadeColor;
	DWORD		g_FadeTimeLength;
	DWORD		g_FadeStartTime;
	BOOL		g_bFadeStart;
	float		g_FadeAlpha;
	BOOL		g_bFadeIn;
	FADE_TRIGGER g_pTrigger;
	BOOL		g_bNotRender;

	//////////////////////////////////////////////////////////////////////////

	BOOL CreateFade()
	{
		g_pDirect3DDevice->CreateVertexBuffer( 4*sizeof(VT_TLVertex), 0 ,D3DFVF_TLVERTEX, D3DPOOL_MANAGED, &m_VB, NULL );

		VT_TLVertex* pVertex = NULL;
		m_VB->Lock( 0, 0, (void**)&pVertex, 0 );

		pVertex[0].pos = Vector4( 0 - 0.5f, 768 - 0.5f, 0, 1);
		pVertex[1].pos = Vector4( 0 - 0.5f, 0 - 0.5f, 0, 1);
		pVertex[2].pos = Vector4( 1024 - 0.5f, 768 - 0.5f, 0, 1);
		pVertex[3].pos = Vector4( 1024 - 0.5f, 0 - 0.5f, 0, 1);

		m_VB->Unlock();

		return TRUE;
	}

	BOOL DestroyFade()
	{
		if( m_VB ) 
			m_VB->Release();

		m_VB = NULL;
		return TRUE;		
	}

	//////////////////////////////////////////////////////////////////////////

	BOOL StartFade(D3DCOLOR color,BOOL bFadeIn,FADE_TRIGGER pTrigger,DWORD time,BOOL bNotRender)
	{
		g_bFadeStart = TRUE;
		g_FadeStartTime = g_dwCurTime;
		g_FadeTimeLength = time;
		g_FadeColor = color;
		g_bFadeIn = bFadeIn;
		g_pTrigger = pTrigger;
		g_bNotRender = bNotRender;

		/*
		g_Vertex[0].pos = Vector4( 0, 767, 0, 1);
		g_Vertex[1].pos = Vector4( 0, 0, 0, 1);
		g_Vertex[2].pos = Vector4( 1023, 767, 0, 1);
		g_Vertex[3].pos = Vector4( 1023, 0, 0, 1);
		*/

		return TRUE;
	}

	BOOL UpdateFade()
	{
		if( g_bFadeStart == FALSE)
			return TRUE;

		float detalTime = g_dwCurTime - g_FadeStartTime;
		
		if( g_bFadeIn)
			g_FadeAlpha = (float)detalTime / (float)g_FadeTimeLength;
		else
			g_FadeAlpha = 1 - (float)detalTime / (float)g_FadeTimeLength;

		if( g_FadeAlpha > 1)
			g_FadeAlpha = 1;
		
		if( g_FadeAlpha < 0)
			g_FadeAlpha = 0;

		DWORD dwAlpha;
		dwAlpha = (BYTE)(255 * g_FadeAlpha) << 24;

		/*
		g_Vertex[ 0].diffuse = (g_FadeColor & 0xFFFFFF) + dwAlpha;
		g_Vertex[ 1].diffuse = (g_FadeColor & 0xFFFFFF) + dwAlpha;
		g_Vertex[ 2].diffuse = (g_FadeColor & 0xFFFFFF) + dwAlpha;
		g_Vertex[ 3].diffuse = (g_FadeColor & 0xFFFFFF) + dwAlpha;
		*/

		VT_TLVertex* pVertex = NULL;

		m_VB->Lock( 0, 0, (void**)&pVertex, 0 );

		pVertex[0].diffuse = (g_FadeColor & 0xFFFFFF) + dwAlpha;
		pVertex[1].diffuse = (g_FadeColor & 0xFFFFFF) + dwAlpha;
		pVertex[2].diffuse = (g_FadeColor & 0xFFFFFF) + dwAlpha;
		pVertex[3].diffuse = (g_FadeColor & 0xFFFFFF) + dwAlpha;

		m_VB->Unlock();

		if( g_dwCurTime - g_FadeStartTime > g_FadeTimeLength)
		{
			if( g_pTrigger)
				g_pTrigger(0); 

			DBG_Put(_T("Fade End"));
		}

		return TRUE;
	}

	BOOL RenderFade()
	{
		if( g_dwCurTime - g_FadeStartTime > g_FadeTimeLength)
		{
			g_bFadeStart = FALSE;
		}
		
		if( g_FadeAlpha == 0)
			return TRUE;

		if( g_bNotRender )
			return TRUE;

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

		return TRUE;
	}

	BOOL SetFade(D3DCOLOR color)
	{
		g_FadeStartTime = g_dwCurTime;
		g_FadeTimeLength = 0;
		g_bFadeStart = FALSE;
		g_FadeAlpha = 1;
		g_FadeColor = color;

		VT_TLVertex* pVertex = NULL;
		m_VB->Lock( 0, 0, (void**)&pVertex, 0 );
/*
		pVertex[0].pos = Vector4( 0 - 0.5f, 768 - 0.5f, 0, 1);
		pVertex[1].pos = Vector4( 0 - 0.5f, 0 - 0.5f, 0, 1);
		pVertex[2].pos = Vector4( 1024 - 0.5f, 768 - 0.5f, 0, 1);
		pVertex[3].pos = Vector4( 1024 - 0.5f, 0 - 0.5f, 0, 1);
*/
		DWORD dwAlpha;
		dwAlpha = (BYTE)(255 * g_FadeAlpha) << 24;

		pVertex[0].diffuse = (g_FadeColor & 0xFFFFFF) + dwAlpha;
		pVertex[1].diffuse = (g_FadeColor & 0xFFFFFF) + dwAlpha;
		pVertex[2].diffuse = (g_FadeColor & 0xFFFFFF) + dwAlpha;
		pVertex[3].diffuse = (g_FadeColor & 0xFFFFFF) + dwAlpha;

		m_VB->Unlock();

		return TRUE;
	}
};
