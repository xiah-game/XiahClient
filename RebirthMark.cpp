
#include "precompile.h"
#include "RebirthMark.h"
#include <assert.h>


RebirthMark g_RebirthMark;

RebirthMark::RebirthMark() : m_pVB(NULL)
{
	// Construct
}

RebirthMark::~RebirthMark()
{
	// Destruct
	Release();
}

void RebirthMark::Init()
{
	g_pDirect3DDevice->CreateVertexBuffer(4 * sizeof(VT_TLVertex), 0, D3DFVF_TLVERTEX, D3DPOOL_MANAGED, &m_pVB, NULL);

	for(int i=0; i < 6; ++i)
	{
		m_pTexture[i] = XiahPak::GetTexture(1510 + i, TRUE);	    
	}
	for(int i=0; i < 6; ++i)
	{
		m_pTexture[i+6] = XiahPak::GetTexture(1610 + (i*2), TRUE);	    
	}
	//HT_1023 : 운영자 마크 추가
	m_pTexture[12] = XiahPak::GetTexture(50003115, TRUE);	    
}

void RebirthMark::Release()
{
	if(m_pVB)
		m_pVB->Release(), m_pVB = NULL;

	for(int i=0; i < 6; ++i)
	{
		XiahPak::ReleaseRes(1510 + i);		
	}

	for(int i=0; i < 6; ++i)
	{
		XiahPak::ReleaseRes(1610 + (i*2));		
	}
	//HT_1023 : 운영자 마크 추가
	XiahPak::ReleaseRes(50003115);
}


void RebirthMark::RenderMark(int nMarkID, int nX, int nY)
{
	if(nMarkID > 13)
		return;
	
	float fX = nX - 0.5f;
	float fY = nY - 0.5f;

	VT_TLVertex	Vertex[4];
	//HT_1023 : 운영자 마크 추가
	if(nMarkID != 13)
	{
		Vertex[ 0].pos = Vector4( fX,		fY,			0, 1);
		Vertex[ 1].pos = Vector4( fX + 16,	fY,			0, 1);
		Vertex[ 2].pos = Vector4( fX,		fY + 16,	0, 1);
		Vertex[ 3].pos = Vector4( fX + 16,	fY + 16,	0, 1);
	}
	else
	{
		Vertex[ 0].pos = Vector4( fX,		fY,			0, 1);
		Vertex[ 1].pos = Vector4( fX + 14,	fY,			0, 1);
		Vertex[ 2].pos = Vector4( fX,		fY + 14,	0, 1);
		Vertex[ 3].pos = Vector4( fX + 14,	fY + 14,	0, 1);
	}

	D3DCOLOR d3dcolor = D3DCOLOR_ARGB(100, 255, 255, 255);
	Vertex[0].diffuse = Vertex[1].diffuse = Vertex[2].diffuse = Vertex[3].diffuse = d3dcolor;

	Vertex[ 0].tex = Vector2(0, 0);
	Vertex[ 1].tex = Vector2(1, 0);
	Vertex[ 2].tex = Vector2(0, 1);
	Vertex[ 3].tex = Vector2(1, 1);

	VOID* pVertices = NULL;
	if(!FAILED(m_pVB->Lock(0, sizeof(Vertex), (void**)&pVertices, 0 )))
	{
		memcpy( pVertices, Vertex, sizeof(Vertex) );
		m_pVB->Unlock();
	} // if(!FAILED(m_pVB->Lock(0, sizeof(Vertex), (void**)&pVertices, 0 )))

	g_pDirect3DDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	g_pDirect3DDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_NOTEQUAL);
	g_pDirect3DDevice->SetRenderState(D3DRS_ALPHAREF, 0);

	g_Device.SetTexture(0, m_pTexture[nMarkID - 1]);

	g_Device.SetStreamSource(m_pVB, sizeof(VT_TLVertex));
	g_Device.SetFVF(D3DFVF_TLVERTEX);

	g_pDirect3DDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);
}
