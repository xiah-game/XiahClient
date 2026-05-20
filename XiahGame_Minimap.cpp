#include "precompile.h"

#include "CharacterInfo.h"
#include "XiahGameObject.h"
#include "XiahGame_Minimap.h"
#include "XiahMap.h"
#include "XiahCamera.h"
#include "InterfaceDefine.h"
#include "XiahGame_Pet.h"
#include "XiahEnvInfo.h"

#ifdef _DEBUG_CHEAT	
#include "XiahGame_Main.h"
extern BOOL g_bCheat;
#endif

namespace Minimap
{

#define APPEND_ICON( xsize, ysize, index, color)	\
	icon_pos[ 0] = Vector3(  - (xsize) / 2, + (ysize) / 2, 0);\
	icon_pos[ 1] = Vector3(  - (xsize) / 2, - (ysize) / 2, 0);\
	icon_pos[ 2] = Vector3(  + (xsize) / 2, + (ysize) / 2, 0);\
	icon_pos[ 3] = Vector3(  + (xsize) / 2, - (ysize) / 2, 0);\
	icon_pos[ 0] *= char_rotM;\
	icon_pos[ 1] *= char_rotM;\
	icon_pos[ 2] *= char_rotM;\
	icon_pos[ 3] *= char_rotM;\
	icon_pos[ 0] += Vector3( x, y, 0);\
	icon_pos[ 1] += Vector3( x, y, 0);\
	icon_pos[ 2] += Vector3( x, y, 0);\
	icon_pos[ 3] += Vector3( x, y, 0);\
	icon_pos[ 0] *= scaleM * rotM;\
	icon_pos[ 1] *= scaleM * rotM;\
	icon_pos[ 2] *= scaleM * rotM;\
	icon_pos[ 3] *= scaleM * rotM;\
	icon_pos[ 0] += Vector3( g_rcWindow.Center().x, g_rcWindow.Center().y, 0);\
	icon_pos[ 1] += Vector3( g_rcWindow.Center().x, g_rcWindow.Center().y, 0);\
	icon_pos[ 2] += Vector3( g_rcWindow.Center().x, g_rcWindow.Center().y, 0);\
	icon_pos[ 3] += Vector3( g_rcWindow.Center().x, g_rcWindow.Center().y, 0);\
	pVertex[ 0].pos = Vector4( (int)icon_pos[ 0].x - 0.5f, (int)icon_pos[ 0].y - 0.5f, 0, 1);\
	pVertex[ 1].pos = Vector4( (int)icon_pos[ 1].x - 0.5f, (int)icon_pos[ 1].y - 0.5f, 0, 1);\
	pVertex[ 2].pos = Vector4( (int)icon_pos[ 2].x - 0.5f, (int)icon_pos[ 2].y - 0.5f, 0, 1);\
	pVertex[ 3].pos = Vector4( (int)icon_pos[ 3].x - 0.5f, (int)icon_pos[ 3].y - 0.5f, 0, 1);\
	pVertex[ 0].diffuse = color;\
	pVertex[ 1].diffuse = color;\
	pVertex[ 2].diffuse = color;\
	pVertex[ 3].diffuse = color;\
	pVertex[ 0].tex = Vector2( 0, index * 0.25f + 0.25f);\
	pVertex[ 1].tex = Vector2( 0, index * 0.25f);\
	pVertex[ 2].tex = Vector2( 1, index * 0.25f + 0.25f);\
	pVertex[ 3].tex = Vector2( 1, index * 0.25f);\
	pIndex[ 0] = g_nIconCount * 4 + 0;\
	pIndex[ 1] = g_nIconCount * 4 + 1;\
	pIndex[ 2] = g_nIconCount * 4 + 2;\
	pIndex[ 3] = g_nIconCount * 4 + 1;\
	pIndex[ 4] = g_nIconCount * 4 + 2;\
	pIndex[ 5] = g_nIconCount * 4 + 3;\
	++g_nIconCount;\
	pVertex += 4;\
	pIndex += 6;

//HT_0824 : 퀘스트 도우미 추가
#define APPEND_ICON2( xsize, ysize, index, color)	\
	icon_pos[ 0] = Vector3(  - (xsize) / 2, + (ysize) / 2, 0);\
	icon_pos[ 1] = Vector3(  - (xsize) / 2, - (ysize) / 2, 0);\
	icon_pos[ 2] = Vector3(  + (xsize) / 2, + (ysize) / 2, 0);\
	icon_pos[ 3] = Vector3(  + (xsize) / 2, - (ysize) / 2, 0);\
	icon_pos[ 0] *= char_rotM;\
	icon_pos[ 1] *= char_rotM;\
	icon_pos[ 2] *= char_rotM;\
	icon_pos[ 3] *= char_rotM;\
	icon_pos[ 0] += Vector3( x, y, 0);\
	icon_pos[ 1] += Vector3( x, y, 0);\
	icon_pos[ 2] += Vector3( x, y, 0);\
	icon_pos[ 3] += Vector3( x, y, 0);\
	icon_pos[ 0] *= scaleM;\
	icon_pos[ 1] *= scaleM;\
	icon_pos[ 2] *= scaleM;\
	icon_pos[ 3] *= scaleM;\
	icon_pos[ 0] += Vector3( rcMini.left, rcMini.top, 0);\
	icon_pos[ 1] += Vector3( rcMini.left, rcMini.top, 0);\
	icon_pos[ 2] += Vector3( rcMini.left, rcMini.top, 0);\
	icon_pos[ 3] += Vector3( rcMini.left, rcMini.top, 0);\
	pVertex[ 0].pos = Vector4( (int)icon_pos[ 0].x - 0.5f, (int)icon_pos[ 0].y - 0.5f, 0, 1);\
	pVertex[ 1].pos = Vector4( (int)icon_pos[ 1].x - 0.5f, (int)icon_pos[ 1].y - 0.5f, 0, 1);\
	pVertex[ 2].pos = Vector4( (int)icon_pos[ 2].x - 0.5f, (int)icon_pos[ 2].y - 0.5f, 0, 1);\
	pVertex[ 3].pos = Vector4( (int)icon_pos[ 3].x - 0.5f, (int)icon_pos[ 3].y - 0.5f, 0, 1);\
	pVertex[ 0].diffuse = color;\
	pVertex[ 1].diffuse = color;\
	pVertex[ 2].diffuse = color;\
	pVertex[ 3].diffuse = color;\
	pVertex[ 0].tex = Vector2( 0, index * 0.25f + 0.25f);\
	pVertex[ 1].tex = Vector2( 0, index * 0.25f);\
	pVertex[ 2].tex = Vector2( 1, index * 0.25f + 0.25f);\
	pVertex[ 3].tex = Vector2( 1, index * 0.25f);\
	pIndex[ 0] = g_nIconCount * 4 + 0;\
	pIndex[ 1] = g_nIconCount * 4 + 1;\
	pIndex[ 2] = g_nIconCount * 4 + 2;\
	pIndex[ 3] = g_nIconCount * 4 + 1;\
	pIndex[ 4] = g_nIconCount * 4 + 2;\
	pIndex[ 5] = g_nIconCount * 4 + 3;\
	++g_nIconCount;\
	pVertex += 4;\
	pIndex += 6;

	// RESPACKER에서 ID를 횅땍하기 바람.
	long g_MiniMapTextureID[15] = 
	{
		50003258,	// 기암
		50003260,	// 화산
		50003262,	// 빙하
		50003256,	// 사막
		50003264,	// 늪
		50003254,	// 초원
		50003266,	// 고산
		0,
		50003256,	// [10/4/2005] 이벤트 맵 - 사막
		50003268,	// Battle1
		50003271,	// 던전1
		50002866,	// 마혈성
		50003270,	// 광명전
		50003273,	// 천황전
		50003283,	// 화염곡 // HO_0727_07 화염곡추가
	};

	long g_MiniMapTextureID_text[15] = 
	{
		50003259,	// 기암
		50003261,	// 화산
		50003263,	// 빙하
		50003257,	// 사막
		50003265,	// 늪
		50003255,	// 초원
		50003267,	// 고산
		0,
		50003256,	// [10/4/2005] 이벤트 맵 - 사막
		50003268,	// Battle1
		50003271,	// 던전1
		50002866,	// 마혈성
		50003270,	// 광명전
		50003273,	// 천황전
		50003284,	// 화염곡 // HO_0727_07 화염곡추가
	};

	IDirect3DTexture9* g_pMainTexture = NULL;		// 빽판
	IDirect3DTexture9* g_pMainTexture_text = NULL;	// 빽판2

	IDirect3DTexture9* g_pIconTexture = NULL;
	IDirect3DTexture9* g_CompassTexture = NULL;// 나침반

	DWORD			   g_dwMiniMapID;	// Mimap갱신용
	
	LPDIRECT3DVERTEXBUFFER9	m_VB1;
	LPDIRECT3DVERTEXBUFFER9	m_VB2;

	sRect			   g_rcWindow;	// 미니맵이 보여질 영역

	#define MAX_MINIMAP_ICON	300		// 음
	#define COMPASS_SIZE		64		// 나침반 사이즈

	IDirect3DVertexBuffer9* g_pIconVertexBuffer = NULL;
	IDirect3DVertexBuffer9* g_CompassVertexBuffer = NULL;
	IDirect3DIndexBuffer9*  g_pIconIndexBuffer = NULL;
	
	int		g_nIconCount;
	float	g_ZoomScale;
	float	g_Realmap_width;
	float	g_Realmap_height;


	/**
	 *
	 * \return 
	 */
	BOOL CreateMiniMap()
	{
		g_pDirect3DDevice->CreateVertexBuffer( 4*sizeof(VT_TLVertex), 0 ,D3DFVF_TLVERTEX, D3DPOOL_MANAGED, &m_VB1, NULL );
		g_pDirect3DDevice->CreateVertexBuffer( 4*sizeof(VT_TLVertex), 0 ,D3DFVF_TLVERTEX, D3DPOOL_MANAGED, &m_VB2, NULL );

		if( g_pIconVertexBuffer == NULL)
		{
			g_pDirect3DDevice->CreateVertexBuffer( MAX_MINIMAP_ICON * 4 * sizeof( VT_TLVertex),
											0, D3DFVF_TLVERTEX,
											D3DPOOL_MANAGED, &g_pIconVertexBuffer, NULL);
		}

		if(g_CompassVertexBuffer == NULL)
		{
			g_pDirect3DDevice->CreateVertexBuffer( 4 * sizeof( VT_TLVertex),
													0, D3DFVF_TLVERTEX,
													D3DPOOL_MANAGED, &g_CompassVertexBuffer, NULL);
		}

		if( g_pIconIndexBuffer == NULL )
		{
			g_pDirect3DDevice->CreateIndexBuffer( 6 * sizeof(WORD) * MAX_MINIMAP_ICON, 0, D3DFMT_INDEX16, D3DPOOL_MANAGED, &g_pIconIndexBuffer, NULL);
		}

		g_nIconCount = 0;
		g_ZoomScale = 0.392f;	// 경험적수치 -_-;

		return TRUE;
	}

	/**
	 *
	 * \return 
	 */
	BOOL ReleaseMiniMap()
	{
		if(m_VB1) m_VB1->Release();
		m_VB1 = NULL;
		if(m_VB2) m_VB2->Release();
		m_VB2 = NULL;

		if( g_pIconVertexBuffer)
		{
			g_pIconVertexBuffer->Release();
			g_pIconVertexBuffer = NULL;
		}


		if(g_CompassVertexBuffer)
		{
			g_CompassVertexBuffer->Release();
			g_CompassVertexBuffer = NULL;
		}

		if( g_pIconIndexBuffer)
		{
			g_pIconIndexBuffer->Release();
			g_pIconIndexBuffer = NULL;
		}

		return TRUE;
	}
	
	float g_fMinimapAlpha = 0;

	/**
	 *
	 * \return 
	 */
	BOOL UpdateMinimap()
	{
		// Real MAP의 크기
		g_Realmap_width = (float)XiahMap::g_XiahMap.m_MapInfo.m_wWidth;
		g_Realmap_height = (float)XiahMap::g_XiahMap.m_MapInfo.m_wHeight;

		if( g_pMainTexture == NULL || g_pMainTexture_text == NULL || g_dwMiniMapID != XiahMap::g_XiahMap.m_MapInfo.m_dwMapID)
		{
			if( g_pMainTexture != NULL) // 기존꺼는 지워준다
			{
				XiahPak::ReleaseRes( g_MiniMapTextureID[ g_dwMiniMapID - 1]);
				XiahPak::ReleaseRes( g_MiniMapTextureID_text[ g_dwMiniMapID - 1]);
			}

			g_dwMiniMapID = XiahMap::g_XiahMap.m_MapInfo.m_dwMapID;
			g_pMainTexture = XiahPak::GetTexture( g_MiniMapTextureID[ g_dwMiniMapID - 1], TRUE);
			g_pMainTexture_text = XiahPak::GetTexture( g_MiniMapTextureID_text[ g_dwMiniMapID - 1], TRUE);
		}

		// 나침반 texture
		if(!g_CompassTexture)
		{
			g_CompassTexture = XiahPak::GetTexture( g_pUIManager->GetData(MINIMAP_WINDOW, minimap_window_direction, GET_TEXTURE), TRUE);
		}

		if( g_pIconTexture == NULL)
		{
			g_pIconTexture = XiahPak::GetTexture( 50003274);
		}

		if( g_pMainChar == NULL)	// 캐릭터가 만들어 질때 까지 기다림
			return TRUE;


		static bool s_curMapSize = FALSE;

		if( g_MainCharInfo.m_bChangMinimap)
		{
			if( s_curMapSize)  //전체 미니맵	
			{
				//g_fMinimapAlpha += 0.3f * g_fFrameScale;
				g_fMinimapAlpha = 1.0f;
				if( g_fMinimapAlpha >= 1.0f)
				{
					g_fMinimapAlpha = 1.0f;
					s_curMapSize = FALSE;
					g_MainCharInfo.m_bChangMinimap = FALSE;
				}
			}
			else			//작은 미니맵
			{
				//g_fMinimapAlpha -= 0.3f * g_fFrameScale;
				g_fMinimapAlpha = 0;
				if( g_fMinimapAlpha <= 0.0f)
				{
					g_fMinimapAlpha = 0;
					s_curMapSize = TRUE;
					g_MainCharInfo.m_bChangMinimap = FALSE;
				}
			}
		}
#ifdef _DEBUG_CHEAT
		CXiahGame_Main *pGameMainStep = (CXiahGame_Main*)g_GameStep[ 3];			
		g_bCheat = FALSE;
		pGameMainStep->g_tHelp[6].SetText( 200,80 , "중지", GetFont("若뗤퐪", 14), D3DCOLOR_XRGB( 0, 255, 255), 15);
#endif

#ifdef TRACE_LOG
		if(pControl == NULL)
		{
			DBG_LogFile( _T("UpdateMinimap 실패"));
		}
#endif
		sRect rcMini;
		g_pUIManager->GetRegionData(MINIMAP_WINDOW, minimap_window_dummy, rcMini);

		if(g_fMinimapAlpha)
		{
			g_rcWindow.left = 0;
			g_rcWindow.top = 0;
			g_rcWindow.bottom = g_rcWindow.right = 512;
		}
		else
		{
			g_rcWindow = rcMini;
		}
		
		VT_TLVertex* pV1;
		VT_TLVertex* pV2;
		m_VB1->Lock( 0, 0, (void**)&pV1, 0 );
		m_VB2->Lock( 0, 0, (void**)&pV2, 0 );

		pV1[ 0].tex = Vector2( 0, 1);
		pV1[ 1].tex = Vector2( 0, 0);
		pV1[ 2].tex = Vector2( 1, 1);
		pV1[ 3].tex = Vector2( 1, 0);

		pV2[ 0].tex = Vector2( 0, 1);
		pV2[ 1].tex = Vector2( 0, 0);
		pV2[ 2].tex = Vector2( 1, 1);
		pV2[ 3].tex = Vector2( 1, 0);

		if( g_fMinimapAlpha)
		{
			pV1[ 0].diffuse = D3DCOLOR_ARGB( 200, 255, 255, 255);
			pV1[ 1].diffuse = D3DCOLOR_ARGB( 200, 255, 255, 255);
			pV1[ 2].diffuse = D3DCOLOR_ARGB( 200, 255, 255, 255);
			pV1[ 3].diffuse = D3DCOLOR_ARGB( 200, 255, 255, 255);
		}
		else
		{
			pV1[ 0].diffuse = D3DCOLOR_ARGB( 200, 255, 255, 255);
			pV1[ 1].diffuse = D3DCOLOR_ARGB( 200, 255, 255, 255);
			pV1[ 2].diffuse = D3DCOLOR_ARGB( 200, 255, 255, 255);
			pV1[ 3].diffuse = D3DCOLOR_ARGB( 200, 255, 255, 255);
		}

		// compass
		pV2[ 0].diffuse = D3DCOLOR_ARGB( 255, 255, 255, 255);
		pV2[ 1].diffuse = D3DCOLOR_ARGB( 255, 255, 255, 255);
		pV2[ 2].diffuse = D3DCOLOR_ARGB( 255, 255, 255, 255);
		pV2[ 3].diffuse = D3DCOLOR_ARGB( 255, 255, 255, 255);

		pV1[ 0].pos = pV1[ 1].pos = pV1[ 2].pos = pV1[ 3].pos = Vector4( 0, 0, 0, 1);
		pV2[ 0].pos = pV2[ 1].pos = pV2[ 2].pos = pV2[ 3].pos = Vector4( 0, 0, 0, 1);

		Vector3 pos[4],pos2[4];
		// MAP
		pos[ 0] = Vector3( 0, 512, 0);
		pos[ 1] = Vector3( 0, 0, 0);
		pos[ 2] = Vector3( 512, 512, 0);
		pos[ 3] = Vector3( 512, 0, 0);

		// COMPASS 중앙중심 Rotate를 위하여 좌표이동.
		pos2[0] = Vector3( -(COMPASS_SIZE/2), (COMPASS_SIZE/2), 0);
		pos2[1] = Vector3( -(COMPASS_SIZE/2), -(COMPASS_SIZE/2), 0);
		pos2[2] = Vector3( (COMPASS_SIZE/2), (COMPASS_SIZE/2), 0);
		pos2[3] = Vector3( (COMPASS_SIZE/2), -(COMPASS_SIZE/2), 0);

		CXiahCharObject* pObject = (CXiahCharObject*)g_pMainChar->m_pObject;
#ifdef TRACE_LOG
		if(pObject == NULL)
		{
			DBG_LogFile( _T("UpdateMinimap 실패"));
		}
#endif

		float fPosX = (float)pObject->m_Position.x * 512.0f / g_Realmap_width;
		float fPosY = (float)-pObject->m_Position.z * 512.0f / g_Realmap_height;

		float angle = -g_XiahCamera.m_fYAngle;

		Matrix4x4 rotM;
		Matrix4x4 scaleM;

		if( g_MainCharInfo.m_bRotateMinimap)
		{
			rotM.SetRotationZ( angle);
		}

		if( g_fMinimapAlpha)
		{
			scaleM.SetScale(g_ZoomScale);
		}
		else
		{
			scaleM.SetScale( 2.5f);
		}
	
		int i;
		for(i = 0; i < 4; ++i)
		{
			if(!g_fMinimapAlpha) 
			{
				pos[ i] -= Vector3( fPosX, fPosY, 0);
				pos[ i] *= scaleM * rotM;
			}
			else
			{
				pos[ i] *= scaleM;
			}
		}

		for(i = 0; i < 4; ++i)
		{
			if(!g_fMinimapAlpha)
			{
				// MAP 출력
				pos[i].x = (int)pos[ i].x + g_rcWindow.Center().x - 0.5f;
				pos[i].y = (int)pos[ i].y + g_rcWindow.Center().y - 0.5f;

				// COMPASS 출력
				// 중앙중심으로 이동한 COMPASS를 회전한다
				pos2[i] *= rotM;
				// 출력을위하여 좌표를 보정한다.
				pos2[i].x += rcMini.left + (COMPASS_SIZE/2);
				pos2[i].y += rcMini.top + (COMPASS_SIZE/2);

			}
			else
			{
				// FULL SIZE  MAP 출력
				pos[i].x += rcMini.left;
				pos[i].y += rcMini.top;
			}
			// map
			//memcpy( &g_Vertex[i].pos, &pos[i], sizeof( float) * 2);
			// compass
			//memcpy( &g_Vertex2[i].pos, &pos2[i], sizeof( float) * 2);
			memcpy( &pV1[i].pos, &pos[i], sizeof( float) * 2);
			memcpy( &pV2[i].pos, &pos2[i], sizeof( float) * 2);
		}

		m_VB1->Unlock();
		m_VB2->Unlock();

		g_nIconCount = 0;
		if( g_pIconVertexBuffer == NULL || g_pIconIndexBuffer == NULL || g_CompassVertexBuffer == NULL)
			return TRUE;

		// 모든 오브젝트에 대해서 돌아볼까? 꾸웩~

		VT_TLVertex *pVertex = NULL;
		VT_TLVertex *pVertex2 = NULL;
		unsigned short *pIndex = NULL;

		float x,y;
		Matrix4x4 char_rotM;
		Vector3 icon_pos[ 4];

		// 나를 찍자!
		x = 0; y = 0;

		g_CompassVertexBuffer->Lock( 0, 0, (void **)&pVertex2, 0);
		g_pIconVertexBuffer->Lock( 0, 0, (void **)&pVertex, 0);
		g_pIconIndexBuffer->Lock( 0, 0, (void **)&pIndex, 0);

		if(g_fMinimapAlpha)
		{
			char_rotM.SetRotationZ( _PI - pObject->m_LocalAngle - pObject->m_Angle);

			x = (float)pObject->m_Position.x * 512.0f / g_Realmap_width;
			y = (float)-pObject->m_Position.z * 512.0f / g_Realmap_height;

			scaleM.SetScale(g_ZoomScale);

			float xsize, ysize;
			int index = 0;
			xsize = ysize = 40;

			icon_pos[ 0] = Vector3(  - (xsize) / 2, + (ysize) / 2, 0);
			icon_pos[ 1] = Vector3(  - (xsize) / 2, - (ysize) / 2, 0);
			icon_pos[ 2] = Vector3(  + (xsize) / 2, + (ysize) / 2, 0);
			icon_pos[ 3] = Vector3(  + (xsize) / 2, - (ysize) / 2, 0);
			
			icon_pos[ 0] *= char_rotM;
			icon_pos[ 1] *= char_rotM;
			icon_pos[ 2] *= char_rotM;
			icon_pos[ 3] *= char_rotM;

			icon_pos[ 0] += Vector3( x, y, 0);
			icon_pos[ 1] += Vector3( x, y, 0);
			icon_pos[ 2] += Vector3( x, y, 0);
			icon_pos[ 3] += Vector3( x, y, 0);

			icon_pos[ 0] *= scaleM;
			icon_pos[ 1] *= scaleM;
			icon_pos[ 2] *= scaleM;
			icon_pos[ 3] *= scaleM;

			icon_pos[ 0] += Vector3( rcMini.left, rcMini.top, 0);
			icon_pos[ 1] += Vector3( rcMini.left, rcMini.top, 0);
			icon_pos[ 2] += Vector3( rcMini.left, rcMini.top, 0);
			icon_pos[ 3] += Vector3( rcMini.left, rcMini.top, 0);
			 

			pVertex[ 0].pos = Vector4( (int)icon_pos[ 0].x - 0.5f, (int)icon_pos[ 0].y - 0.5f, 0, 1);
			pVertex[ 1].pos = Vector4( (int)icon_pos[ 1].x - 0.5f, (int)icon_pos[ 1].y - 0.5f, 0, 1);
			pVertex[ 2].pos = Vector4( (int)icon_pos[ 2].x - 0.5f, (int)icon_pos[ 2].y - 0.5f, 0, 1);
			pVertex[ 3].pos = Vector4( (int)icon_pos[ 3].x - 0.5f, (int)icon_pos[ 3].y - 0.5f, 0, 1);
			
			pVertex[ 0].diffuse = D3DCOLOR_XRGB( 0, 255,0);
			pVertex[ 1].diffuse = D3DCOLOR_XRGB( 0, 255,0);
			pVertex[ 2].diffuse = D3DCOLOR_XRGB( 0, 255,0);
			pVertex[ 3].diffuse = D3DCOLOR_XRGB( 0, 255,0);

			pVertex[ 0].tex = Vector2( 0, index * 0.25f + 0.25f);
			pVertex[ 1].tex = Vector2( 0, index * 0.25f);
			pVertex[ 2].tex = Vector2( 1, index * 0.25f + 0.25f);
			pVertex[ 3].tex = Vector2( 1, index * 0.25f);
			pIndex[ 0] = g_nIconCount * 4 + 0;
			pIndex[ 1] = g_nIconCount * 4 + 1;
			pIndex[ 2] = g_nIconCount * 4 + 2;
			pIndex[ 3] = g_nIconCount * 4 + 1;
			pIndex[ 4] = g_nIconCount * 4 + 2;
			pIndex[ 5] = g_nIconCount * 4 + 3;
			
			++g_nIconCount;
			pVertex += 4;
			pIndex += 6;
		}
		else
		{
			char_rotM.SetRotationZ( _PI -pObject->m_LocalAngle - pObject->m_Angle);
			scaleM.SetScale( 2.5f);
			APPEND_ICON( 6, 6, 0, D3DCOLOR_XRGB( 255, 255, 255));
		}

		// 다른애들
		if( !g_fMinimapAlpha)
		{
			// 메인 캐릭터가 암흑무에 걸리면 아예 안그린다.
			if( !g_XiahEnvInfo.m_bAmhukmuFog )
			{
#ifdef _DEBUG_CHEAT	
			int nCount = 0;
#endif
				XiahObject::CXiahObjectManager::iterator it = XiahObject::g_XiahObjectManager.begin();

				for( ; it != XiahObject::g_XiahObjectManager.end(); ++it)
				{
					XiahObject::CXiahObject *pXiahObject = it->second;

					if( pXiahObject == g_pMainChar || pXiahObject->m_pObject == NULL)
						continue;

					if( g_nIconCount >= MAX_MINIMAP_ICON)
						continue;

					if( pXiahObject->m_pObject->IsA( XiahObject::eXOT_CharObject))
					{
						CXiahCharObject* pXiahCharObject = reinterpret_cast<CXiahCharObject*>( pXiahObject->m_pObject);

						if(!pXiahCharObject->m_bRenderOK)
							continue;

						x = pXiahCharObject->m_Position.x;
						y = -pXiahCharObject->m_Position.z;
						x = x * 512.0f / g_Realmap_width - fPosX;
						y = y * 512.0f / g_Realmap_height - fPosY;

						char_rotM.SetRotationZ( _PI - pXiahCharObject->m_LocalAngle - pXiahCharObject->m_Angle);

						switch( pXiahCharObject->m_bObjType)
						{
						case OBJTYPE_PC:
							{
								// 단인가를 검사한다.
								if(g_MainCharInfo.m_pRelation->FindDanInfoByID(pXiahObject->m_dwServerID) == NULL)
								{
									// 단이 아니면 정상 출력
									APPEND_ICON( 4, 4, 1, D3DCOLOR_XRGB( 0, 0, 255));
								}
								else
								{
									// 단 일경우
									APPEND_ICON( 4, 4, 1, D3DCOLOR_XRGB( 0, 255, 255));
								}
#ifdef _DEBUG_CHEAT	
							++nCount;
#endif
							}

							break;

							case OBJTYPE_NPC:
							{
								// HIDE 상태의 놈은 그리지 않는다
								if(pXiahCharObject->m_bObjStatus == NPCSTATUS_HIDE)
									continue;

								// 광물은 색 교체
								if(pXiahCharObject->m_bExSubObjType == 3)
								{
									APPEND_ICON( 4, 4, 1, D3DCOLOR_XRGB(255, 120, 0));
								}
								else
								{
									if( pXiahCharObject->m_nCurMotionType == XiahAniType::eLAT_Die)
									{
										APPEND_ICON( 4, 4, 1, D3DCOLOR_ARGB( 200, 128, 128, 128));
									}
									else if(pXiahCharObject->m_nCurMotionType != XiahAniType::eLAT_Die && g_bScreenShot)
									{
										APPEND_ICON( 4, 4, 1, D3DCOLOR_XRGB( 255, 0, 0)); //HO_0509_07 오토대처방안 : 스샷으로 색상을 알아내지 못하도록 색상을 변경
									}
									else
									{									
										APPEND_ICON( 4, 4, 1, D3DCOLOR_XRGB( 240, 0, 5)); //HO_0509_07 오토대처방안 : 몬스터를 미니맵 상의 빨간점으로 인식하여 색상을 바꾸어줌....  기존색상( 255, 0, 0)
									}

								}							
							}
							break;
						//case OBJTYPE_NPC: //HO_0525_07 오토대처방안 : 기존코드
						//	{
						//		// HIDE 상태의 놈은 그리지 않는다
						//		if(pXiahCharObject->m_bObjStatus == NPCSTATUS_HIDE)
						//			continue;

						//		// 광물은 색 교체
						//		if(pXiahCharObject->m_bExSubObjType == 3)
						//		{
						//			APPEND_ICON( 4, 4, 1, D3DCOLOR_XRGB(255, 120, 0));
						//		}
						//		else
						//		{
						//			if( pXiahCharObject->m_nCurMotionType == XiahAniType::eLAT_Die)
						//			{
						//				APPEND_ICON( 4, 4, 1, D3DCOLOR_ARGB( 200, 128, 128, 128));
						//			}
						//			else
						//			{
						//				APPEND_ICON( 4, 4, 1, D3DCOLOR_XRGB( 255, 0, 0)); 
						//			}
						//		}							
						//	}
						//	break;
						case OBJTYPE_PET:
							{
								if( g_PetList.Find( pXiahObject->m_dwServerID) != NULL)
								{
									APPEND_ICON( 4, 4, 1, D3DCOLOR_XRGB( 88, 153, 45));
								}
								else
								{
									APPEND_ICON( 4, 4, 1, D3DCOLOR_XRGB( 154, 136, 44));
								}
							}
							break;

						case OBJTYPE_ITEM:				// 아이템
							APPEND_ICON( 2, 2, 1, D3DCOLOR_XRGB( 255, 255, 0));
							break;

						case OBJTYPE_FUNCTIONALNPC:		// 기능 NPC
							APPEND_ICON( 4, 4, 3, D3DCOLOR_XRGB( 255, 255, 255));
							break;

						case OBJTYPE_ARROW:
							APPEND_ICON( 2, 2, 1, D3DCOLOR_XRGB( 255, 200, 200));
							break;
						}
					}
				}

#ifdef _DEBUG_CHEAT
				CXiahGame_Main *pGameMainStep = (CXiahGame_Main*)g_GameStep[ 3];			

				if(nCount == 0)
				{
					g_bCheat = TRUE;
					pGameMainStep->g_tHelp[6].SetText( 200,80 , "사냥", GetFont("若뗤퐪", 14), D3DCOLOR_XRGB( 0, 255, 255), 15);
				}
				else
				{
					g_bCheat = FALSE;
					pGameMainStep->g_tHelp[6].SetText( 200,80 , "사람있다", GetFont("若뗤퐪", 14), D3DCOLOR_XRGB( 0, 255, 255), 15);
				}
#endif
			}
		}
		else if(g_MainCharInfo.m_pQuest->m_sQuestHelp.m_bStart)//HT_0824 : 퀘스트 도우미 추가
		{
			BYTE ShowType = g_MainCharInfo.m_pQuest->m_sQuestHelp.m_byShowType;

			std::vector<sQuestcoordinate*>::iterator it;

			for(it = g_MainCharInfo.m_pQuest->m_sQuestHelp.m_vQuestCoordinate.begin(); 
				it != g_MainCharInfo.m_pQuest->m_sQuestHelp.m_vQuestCoordinate.end(); ++it)
			{
				sQuestcoordinate *QuestCoordinate = *it;
				
				if(QuestCoordinate)
				{
					if(	g_dwMiniMapID == QuestCoordinate->MapID )
					{
						x = QuestCoordinate->xPos* 512.0f / g_Realmap_width;
						y = QuestCoordinate->yPos* 512.0f / g_Realmap_width;
	
						switch(g_MainCharInfo.m_pQuest->m_sQuestHelp.m_byShowType)
						{
						case 1:
							APPEND_ICON2( 10, 10, 1, D3DCOLOR_XRGB( 255, 0, 0));
							break;
						case 2:
							APPEND_ICON2( 20, 20, 1, D3DCOLOR_XRGB( 255, 0, 0));
							break;
						case 3:
							APPEND_ICON2( 20, 20, 1, D3DCOLOR_XRGB( 255, 255, 255));
							break;
						}	
					}
				}
			}
		}
	
		g_CompassVertexBuffer->Unlock();
		g_pIconVertexBuffer->Unlock();
		g_pIconIndexBuffer->Unlock();

		return TRUE;
	}

	BOOL RenderMinimap()
	{
		if( g_pMainChar == NULL)	// 캐릭터가 만들어 질때 까지 기다림
			return TRUE;
		
		D3DVIEWPORT9 pre_view, view;

		g_pDirect3DDevice->GetViewport( &pre_view);
		view = pre_view;

		view.X = g_rcWindow.left;
		view.Y = g_rcWindow.top;
		view.Width = g_rcWindow.Width();
		view.Height = g_rcWindow.Height();

		g_pDirect3DDevice->SetViewport( &view);

		g_pDirect3DDevice->SetRenderState( D3DRS_CULLMODE, D3DCULL_NONE);
		g_pDirect3DDevice->SetRenderState( D3DRS_ALPHABLENDENABLE, TRUE);
		g_pDirect3DDevice->SetRenderState( D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
		g_pDirect3DDevice->SetRenderState( D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

		if(!g_fMinimapAlpha)
		{
			g_Device.SetTexture(0, g_pMainTexture);
			//g_pDirect3DDevice->SetTexture( 0, g_pMainTexture);
		}
		else
		{
			g_Device.SetTexture(0, g_pMainTexture_text);
			//g_pDirect3DDevice->SetTexture( 0, g_pMainTexture_text);
		}

		g_pDirect3DDevice->SetRenderState( D3DRS_ZENABLE, FALSE);
		g_pDirect3DDevice->SetRenderState( D3DRS_FOGENABLE, FALSE);
		g_pDirect3DDevice->SetSamplerState( 0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
		g_pDirect3DDevice->SetSamplerState( 0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);

		g_Device.SetStreamSource( m_VB1, sizeof(VT_TLVertex) );
		g_Device.SetFVF(D3DFVF_TLVERTEX);
		//g_pDirect3DDevice->SetFVF( D3DFVF_TLVERTEX);		
		g_pDirect3DDevice->DrawPrimitive( D3DPT_TRIANGLESTRIP, 0, 2);


		// 오브젝트
		if( g_pIconVertexBuffer == NULL || g_pIconIndexBuffer == NULL)
		{
			g_pDirect3DDevice->SetViewport( &pre_view);
			return TRUE;
		}

		g_Device.SetTexture(0, g_pIconTexture);
		//g_pDirect3DDevice->SetTexture( 0, g_pIconTexture);
		g_pDirect3DDevice->SetRenderState( D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
		g_pDirect3DDevice->SetRenderState( D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

		g_pDirect3DDevice->SetSamplerState( 0, D3DSAMP_ADDRESSU, D3DTADDRESS_CLAMP);
		g_pDirect3DDevice->SetSamplerState( 0, D3DSAMP_ADDRESSV, D3DTADDRESS_CLAMP);

		g_Device.SetStreamSource( g_pIconVertexBuffer, sizeof( VT_TLVertex));
		g_Device.SetIndices( g_pIconIndexBuffer);
		g_pDirect3DDevice->DrawIndexedPrimitive( D3DPT_TRIANGLELIST, 0, 0, g_nIconCount * 4, 0, g_nIconCount * 2);

		int a  = g_nIconCount;


		// Compass
		if(!g_fMinimapAlpha)
		{
			g_pDirect3DDevice->SetRenderState( D3DRS_CULLMODE, D3DCULL_NONE);
			g_pDirect3DDevice->SetRenderState( D3DRS_ALPHABLENDENABLE, TRUE);
			g_pDirect3DDevice->SetRenderState( D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
			g_pDirect3DDevice->SetRenderState( D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

			g_Device.SetTexture(0, g_CompassTexture);
			//g_pDirect3DDevice->SetTexture( 0, g_CompassTexture);
			g_pDirect3DDevice->SetRenderState( D3DRS_ZENABLE, FALSE);
			g_pDirect3DDevice->SetRenderState( D3DRS_FOGENABLE, FALSE);
			g_pDirect3DDevice->SetSamplerState( 0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
			g_pDirect3DDevice->SetSamplerState( 0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);

			g_Device.SetStreamSource( m_VB2, sizeof(VT_TLVertex) );
			g_Device.SetFVF(D3DFVF_TLVERTEX);
			//g_pDirect3DDevice->SetFVF( D3DFVF_TLVERTEX);
			
			g_pDirect3DDevice->DrawPrimitive( D3DPT_TRIANGLESTRIP, 0, 2);
		}


		g_pDirect3DDevice->SetRenderState( D3DRS_CULLMODE, D3DCULL_CCW);
		g_pDirect3DDevice->SetRenderState( D3DRS_ZENABLE, TRUE);
		
		g_pDirect3DDevice->SetRenderState( D3DRS_ALPHABLENDENABLE, FALSE);
//		g_pDirect3DDevice->SetSamplerState( 0, D3DSAMP_MAGFILTER, D3DTEXF_POINT);
//		g_pDirect3DDevice->SetSamplerState( 0, D3DSAMP_MINFILTER, D3DTEXF_POINT);

		g_pDirect3DDevice->SetSamplerState( 0, D3DSAMP_ADDRESSU, D3DTADDRESS_WRAP);
		g_pDirect3DDevice->SetSamplerState( 0, D3DSAMP_ADDRESSV, D3DTADDRESS_WRAP);

		g_pDirect3DDevice->SetViewport( &pre_view);

		return TRUE;
	}
};
