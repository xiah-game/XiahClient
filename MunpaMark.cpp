#include "precompile.h"
#include ".\munpamark.h"
#include "XiahGame_Handler_Sender.h"
#include "AppData.h"
#include <io.h>
#include <assert.h>


CMunpaMark g_MunpaMark;

CMunpaMark::CMunpaMark() : m_pVB(NULL)
{

}

CMunpaMark::~CMunpaMark()
{
	Release();
}

/**
 * 초기화
 */
void CMunpaMark::Init()
{
	g_pDirect3DDevice->CreateVertexBuffer(4 * sizeof(VT_TLVertex), 0, D3DFVF_TLVERTEX, D3DPOOL_MANAGED, &m_pVB, NULL);
}

/**
 * 해제
 */
void CMunpaMark::Release()
{
	if(m_pVB)
		m_pVB->Release(), m_pVB = NULL;

	// Mark list clear
	MarkMap::iterator iter = m_mMark.begin();

	for(; iter != m_mMark.end(); ++iter)
	{
		sMark *pMark = iter->second;
		assert(pMark);

		if(pMark)
		{
			if(pMark->pTexture)
				pMark->pTexture->Release(), pMark->pTexture = NULL;

			delete pMark, pMark = NULL;
		}		
	} // for(; iter != m_mMark.end(); ++iter)

	m_mMark.clear();
}

/**
* 렌더링
* \param dwMarkID 마크ID
* \param nX 위치X
* \param nY 위치Y
*/
void CMunpaMark::Render(DWORD dwMarkID, int nX, int nY)
{
	MarkMap::iterator iter = m_mMark.find(dwMarkID);

	if(iter != m_mMark.end())
	{
		sMark *pMark = iter->second;
		assert(pMark);

		if(!pMark)
			return;

		float fX = nX - 0.5f;
		float fY = nY - 0.5f;

		VT_TLVertex	Vertex[4];
		Vertex[ 0].pos = Vector4( fX,		fY,			0, 1);
		Vertex[ 1].pos = Vector4( fX + 15,	fY,			0, 1);
		Vertex[ 2].pos = Vector4( fX,		fY + 15,	0, 1);
		Vertex[ 3].pos = Vector4( fX + 15,	fY + 15,	0, 1);

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

		g_Device.SetTexture(0, pMark->pTexture);		

		g_Device.SetStreamSource(m_pVB, sizeof(VT_TLVertex));
		g_Device.SetFVF(D3DFVF_TLVERTEX);
		
		g_pDirect3DDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);
	}
}

/**
* 마크렌더링 
* \param dwMarkID 
* \param nX 
* \param nY 
*/
void CMunpaMark::RenderMark(DWORD dwMarkID, int nX, int nY)
{
	assert(dwMarkID);

	MarkMap::iterator iter = m_mMark.find(dwMarkID);

	if(iter != m_mMark.end())
	{
		// 마크 있을시
		sMark *pMark = iter->second;

		assert(pMark);

		if(pMark->pTexture)
		{
			Render(dwMarkID, nX, nY);
		}
		else
		{
			// 텍스쳐 미로딩시

			if(!pMark->bDemand) // 요청중인지
			{
				if(IsMarkFile(dwMarkID))	// 파일존재여부
				{
					if(LoadMarkFile(dwMarkID))
					{
						Render(dwMarkID, nX, nY);
					}
				} // if(IsMarkFile(dwMarkID))
				else
				{
					// 다시 요청
					pMark->bDemand = true;
					SendCS_RL_GAINMARKIMAGE_REQ(dwMarkID);
				}
			}
		}
	} // if(iter != m_mMark.end())
	else
	{
		sMark *pMark = new sMark;
		m_mMark.insert(MarkMap::value_type(dwMarkID, pMark));

		if(IsMarkFile(dwMarkID))	// 파일존재여부
		{
			if(LoadMarkFile(dwMarkID))
			{
				Render(dwMarkID, nX, nY);
			}
		}
		else
		{
			pMark->bDemand = true;
			// 서버에 요청
			SendCS_RL_GAINMARKIMAGE_REQ(dwMarkID);
		}
	}
}

/**
 * 파일 존재 여부
 * \param dwMarkID 마크ID
 * \return 
*/
bool CMunpaMark::IsMarkFile(DWORD dwMarkID)
{
	bool bResult = true;

	TCHAR strFile[128] = {0,};
	// 월드별로 폴더에 관리
	// [2/18/2005] 폴더명 변경
	_stprintf(strFile, "mark\\n%d\\%d.bmp", g_AppData.m_byWorldID, dwMarkID);

	WIN32_FIND_DATA FindFileData;
	HANDLE hFind = FindFirstFile(strFile, &FindFileData);
	
	if(hFind == INVALID_HANDLE_VALUE) 
		bResult = false;
	
	FindClose(hFind);

	return bResult;
}

/**
 * 마크 파일 로딩
 * \param dwMarkID 마크ID
 * \return 
*/
int CMunpaMark::LoadMarkFile(DWORD dwMarkID)
{
	TCHAR strFile[128] = {0,};
	_stprintf(strFile, "mark\\n%d\\%d.bmp", g_AppData.m_byWorldID, dwMarkID);

	D3DXIMAGE_INFO imageinfo;
	if(FAILED(D3DXGetImageInfoFromFile(strFile, &imageinfo)))
	{
		return 0;
	}
#ifdef _DEBUG
	else
	{
		if(imageinfo.ImageFileFormat != D3DXIFF_BMP)
		{
			assert(0);
		}

		if(imageinfo.Width != 16 || imageinfo.Height != 16)
		{
			assert(0);
		}
	}
#endif // _DEBUG

	MarkMap::iterator iter = m_mMark.find(dwMarkID);

	if(iter == m_mMark.end())
		return false;

	sMark *pMark = iter->second;

	assert(pMark);

	if(!pMark)
		return 0;

	if(FAILED(D3DXCreateTextureFromFileEx(g_pDirect3DDevice, strFile, 
										D3DX_DEFAULT, D3DX_DEFAULT, D3DX_DEFAULT, 0, D3DFMT_UNKNOWN, 
										D3DPOOL_MANAGED, D3DX_FILTER_POINT , D3DX_FILTER_LINEAR,
										D3DCOLOR_ARGB(0xFF, 255, 1, 255), NULL, NULL, 
										&pMark->pTexture)))
	{
		return 0;
	}

	return 1;
}

/**
 * 마크 파일 저장 
 * \param dwMarkID 마크ID 
 * \param strData 이미지
 * \return 
*/
bool CMunpaMark::SaveMarkFile(DWORD dwMarkID, LPCTSTR strData)
{
	MarkMap::iterator iter = m_mMark.find(dwMarkID);

	if(iter != m_mMark.end())
	{
		sMark *pMark = iter->second;
		assert(pMark);

		pMark->bDemand = false;
	} // if(iter != m_mMark.end())

	TCHAR strDir[128] = {0,};
	_stprintf(strDir, ".\\mark\\n%d\\", g_AppData.m_byWorldID);

	// 월드별로 폴더에 관리
	CreateDirectory(".\\mark\\", NULL);	// mark 생성
	CreateDirectory(strDir, NULL);		// 월드번호폴더 생성

	TCHAR strFile[128] = {0,};
	_stprintf(strFile, "mark\\n%d\\%d.bmp", g_AppData.m_byWorldID, dwMarkID);

	FILE* fp = NULL;

	try
	{
		if((fp = _tfopen(strFile, _T("wb"))) != NULL)
		{
			// 서버에는 헤드를 제거한 순수 이미지만 들어가 있다.
			// 클라이언트는 bmp저장시 헤드를 붙여주어야 된다.
			// 이유는 이미지를 문자열로 보내는데 0이면 문자열에서 null이기에 문제가 클/서버 다 생긴다.
			// 이미지 또한 0은 1로 변경되어져 있다.

			BITMAPFILEHEADER BMPfileHeader;
			BITMAPINFOHEADER BMPinfoHeader;

			BMPfileHeader.bfType = 19778;
			BMPfileHeader.bfSize = 822;
			BMPfileHeader.bfReserved1 = 0;
			BMPfileHeader.bfReserved2 = 0;
			BMPfileHeader.bfOffBits = 54;

			// 파일 해더
			fwrite(&BMPfileHeader, sizeof(BITMAPFILEHEADER), 1, fp);

			BMPinfoHeader.biSize		= 40;
			BMPinfoHeader.biWidth		= 16;
			BMPinfoHeader.biHeight		= 16;
			BMPinfoHeader.biPlanes		= 1;
			BMPinfoHeader.biBitCount	= 24;
			BMPinfoHeader.biCompression = 0;
			BMPinfoHeader.biSizeImage	= 768;
			BMPinfoHeader.biXPelsPerMeter = 3780;
			BMPinfoHeader.biYPelsPerMeter = 3780;
			BMPinfoHeader.biClrUsed		= 0;
			BMPinfoHeader.biClrImportant = 0;

			// 파일 정보 해더
			fwrite(&BMPinfoHeader, sizeof(BITMAPINFOHEADER), 1, fp);

			// 실제 이미지
			fwrite(strData, 768, 1, fp);

			fclose(fp);
		}
	}
	catch(...)
	{
		
	}

	return true;
}