#include "precompile.h"
#include "XiahMap.h"
#include "XiahArrayIndex.h"
#include "XiahEnvInfo.h"

#include "XiahGameObject.h"

// TODO: 지울것 - 광원 설정 테스트용
//#include "XiahGameMain.h"

#define	FOGCHANGETIME	10000.0f

using namespace XiahMap;

CXiahMap XiahMap::g_XiahMap;

CXiahMap::CXiahMap()
{
	m_bCreated = FALSE;
	m_pMapRender = &Map::g_MapRender;
}

CXiahMap::~CXiahMap()
{
	ReleaseMap();
}

BOOL CXiahMap::CreateMap( DWORD dwMapID, LPCTSTR szMapName, BYTE bType, WORD wWidth, WORD wHeight)
{
	if( !ReleaseMap())
		return FALSE;

	m_MapInfo.m_dwMapID = dwMapID;
	m_MapInfo.m_szMapName = szMapName;
	m_MapInfo.m_bType = bType;
	m_MapInfo.m_wWidth = wWidth;
	m_MapInfo.m_wHeight = wHeight;

	sArrayData *pData = XiahArrayIndex::g_MapFileName.GetData( dwMapID);

	if(pData == NULL)
	{
		DBG_LogFile( _T("CXiahMap::CreateMap 실패"));

		return false;
	}

	// MAP DATA
	Map::g_MapRes.LoadMapFile( pData->GetString( 0));

	// MAP Attribute Data
	XiahMap::g_Map_Attri.Load_Map_Attr((TCHAR*)pData->GetString( 1).data());

	m_pMapRender->SetRenderInfo( &g_XiahEnvInfo);
	m_pMapRender->CreateGrassZoneArray();

	m_bCreated = TRUE;

	return TRUE;
}

BOOL CXiahMap::ReleaseMap()
{
	// 3D엔진을 사용하여 Map을 초기화한다
	// 엔진에서는 특별히 Unload할껀 없을듯
	m_bCreated = FALSE;

	if(m_pMapRender)
		m_pMapRender->Release();

	XiahGameEngine::Map::g_MapRes.Release();
	XiahGameEngine::Map::g_TileRes.Clear();

	return TRUE;
}

BOOL CXiahMap::Update()
{
	if( !m_bCreated)
		return TRUE;

	// 현재의 포그와 라이트가 서서히 변하도록 하기 위해
	ChangeXiahEnvInfo();

	Light::CLight *pLight = Light::g_LightManager.GetGlobalLight();

	if(pLight == NULL)
	{
		DBG_LogFile( _T("CXiahMap::Update 실패"));
	}

	pLight->m_Light.Diffuse.r = (float)GetBValue( g_XiahEnvInfo.m_DiffuseColor) / 255.0f;
	pLight->m_Light.Diffuse.g = (float)GetGValue( g_XiahEnvInfo.m_DiffuseColor) / 255.0f;
	pLight->m_Light.Diffuse.b = (float)GetRValue( g_XiahEnvInfo.m_DiffuseColor) / 255.0f;
	pLight->m_Light.Diffuse.a = 1;

	pLight->m_Light.Ambient.r = pLight->m_Light.Diffuse.r * 0.7f;
	pLight->m_Light.Ambient.g = pLight->m_Light.Diffuse.g * 0.7f;
	pLight->m_Light.Ambient.b = pLight->m_Light.Diffuse.b * 0.7f;
	pLight->m_Light.Ambient.a = 1;

	// SKY BOX의 색 적용한다.
	//g_SkyBox.SetColor( g_XiahEnvInfo.m_SkyColorUp, g_XiahEnvInfo.m_SkyColorMiddle, g_XiahEnvInfo.m_SkyColorBottom );
	//g_SkyBox.SetFogEnable( g_XiahEnvInfo.m_bAmhukmuFog );

	//g_SkyStar.SetColor( g_XiahEnvInfo.m_DiffuseColor );

	m_pMapRender->SetRenderInfo( &g_XiahEnvInfo);

	return m_pMapRender->Update();
}

BOOL CXiahMap::RenderTerrain()
{
	if( !m_bCreated)
		return TRUE;
	
	return m_pMapRender->RenderTerrain();
}

BOOL CXiahMap::RenderObject(BYTE byType)
{
	if( !m_bCreated)
		return TRUE;
	
	return m_pMapRender->RenderObject(byType);
}

BOOL CXiahMap::RenderWater()
{
	if( !m_bCreated)
		return TRUE;

	return m_pMapRender->RenderWater();
}

BOOL CXiahMap::UpdateGrassZone()
{
	if( !m_bCreated)
		return TRUE;

	return m_pMapRender->UpdateGrassZone();
}

BOOL CXiahMap::RenderGrassZone()
{
	if( !m_bCreated)
		return TRUE;

	return m_pMapRender->RenderGrassZone();
}

sPortalInfo* CXiahMap::IntersectPortal(WORD wPosX,WORD wPosY)
{
	PORTALINFOLIST::iterator it = m_PortalInfoList.begin();

	for(; it != m_PortalInfoList.end(); ++it)
	{
		sPortalInfo &portal = *it;

		if(false)
		{
			DBG_LogFile( _T("CXiahMap::IntersectPortal 실패"));
		}

		if( portal.m_bLinkType == 99)
			continue;

		sRect rect( sPoint( portal.m_wPortalPosX, portal.m_wPortalPosY), sSize( portal.m_wPortalWidth, portal.m_wPortalHeight));
	
		if( rect.PtInRect( sPoint( wPosX, wPosY)))
			return &portal;
	}

	return NULL;
}

BOOL CXiahMap::GetPickPosition(Vector3 &pos)	// 졸라 느림 주의 바람
{
	if( g_pCurrentCamera == NULL)
		return FALSE;

#define PICK_DETAIL	300

	Vector3 start;
	Vector3 end;

	start = g_pCurrentCamera->ScreenToWorld( Vector3( XiahInput::g_ptMouse.x, XiahInput::g_ptMouse.y, 0.000001f));
	end	  = g_pCurrentCamera->ScreenToWorld( Vector3( XiahInput::g_ptMouse.x, XiahInput::g_ptMouse.y, 0.999999f));

	Vector3 vStart = start;
	Vector3 vEnd = end;

	Vector3 delta = (end - start) / PICK_DETAIL;

	// 2004_04_20 Changth :
	// 오브젝트 충돌 검사를 좀더 정확하게 해주지. 일단 속도는 나중문제다.
	// 원래는, 최종 start 위치만으로 맵 오브젝트 리스트를 가져왔는데, 불명확해서
	// 검색되는 모든 맵 셀의 메시 블락을 검사한다.
	int nStartXAry[ PICK_DETAIL ];
	int nStartZAry[ PICK_DETAIL ];
	int nSearchCount;

	// 음!! Terrain Check
	BOOL bPositionFind = FALSE;
	for(int i = 0; i < PICK_DETAIL; i++)
	{
		float height = XiahGameEngine::Map::g_MapRes.GetHeight( start.x, start.z);

		// 마우스 시작, 끝 위치로 투영되는 모든 위치를 저장한다.
		if(  start.x > 0 &&  start.x < XiahMap::g_XiahMap.m_MapInfo.m_wWidth &&
			-start.z > 0 && -start.z < XiahMap::g_XiahMap.m_MapInfo.m_wHeight  )
		{
			nStartXAry[i] =  (int)start.x;
			nStartZAry[i] = -(int)start.z;
		}

		if( height > start.y)
		{
			pos.x = start.x;
			pos.y = height;
			pos.z = start.z;

			bPositionFind = TRUE;
			break;
		}

		start += delta;
	}// for
	nSearchCount = i + 1;

	if( !bPositionFind ) return FALSE;

	// 찾아진 start 위치에서 같은 맵셀과 같은 메시 블락으로 구분하자.
	int mapcell_x;
	int mapcell_y;
	int meshblock_x;
	int meshblock_y;
	int mapcellx_ary[ PICK_DETAIL ];	// 비교하기 위해 저장되는 변수들
	int mapcelly_ary[ PICK_DETAIL ];
	int meshblockx_ary[ PICK_DETAIL ];
	int meshblocky_ary[ PICK_DETAIL ];
	int nMapCellMeshBlockCount = 0;

	// 최종 위치 변수
	int nTargetXAry[ PICK_DETAIL ];
	int nTargetZAry[ PICK_DETAIL ];

	for(int k=0; k<nSearchCount; k++)
	{
		// 맵 셀 위치, 메시 블락 위치 계산
		mapcell_x = nStartXAry[k] / 256;
		mapcell_y = nStartZAry[k] / 256;

		meshblock_x = (nStartXAry[k] % 256) / 32;
		meshblock_y = (nStartZAry[k] % 256) / 32;

		if( nMapCellMeshBlockCount == 0 )
		{
			mapcellx_ary[ nMapCellMeshBlockCount ] = mapcell_x;
			mapcelly_ary[ nMapCellMeshBlockCount ] = mapcell_y;
			meshblockx_ary[ nMapCellMeshBlockCount ] = meshblock_x;
			meshblocky_ary[ nMapCellMeshBlockCount ] = meshblock_y;

			nTargetXAry[ nMapCellMeshBlockCount ] = nStartXAry[k];
			nTargetZAry[ nMapCellMeshBlockCount ] = nStartZAry[k];

			nMapCellMeshBlockCount++;
		}
		else // 중복된것이 있나 볼까?
		{
			bool bFind = false;
			for(int kk=0; kk<nMapCellMeshBlockCount; kk++)
			{
				if( mapcell_x == mapcellx_ary[kk] &&
					mapcell_y == mapcelly_ary[kk] &&
					meshblock_x == meshblockx_ary[kk] &&
					meshblock_y == meshblocky_ary[kk]   )
				{
					bFind = true;
					break;
				}
			}// for

			if( !bFind )
			{
				mapcellx_ary[ nMapCellMeshBlockCount ] = mapcell_x;
				mapcelly_ary[ nMapCellMeshBlockCount ] = mapcell_y;
				meshblockx_ary[ nMapCellMeshBlockCount ] = meshblock_x;
				meshblocky_ary[ nMapCellMeshBlockCount ] = meshblock_y;

				nTargetXAry[ nMapCellMeshBlockCount ] = nStartXAry[k];
				nTargetZAry[ nMapCellMeshBlockCount ] = nStartZAry[k];

				nMapCellMeshBlockCount++;
			}
		}
	}// for

	// 찾아진 서로 다른 맵셀과 메시 블락에서 오브젝트를 검사한다. 그래야 정확하지.
	// 2004_04_19 Changth :
	// 이제, 캐릭터가 다리 위에 있을때, 엉뚱한 곳으로 가지않고 제대로 이동하도록 오브젝트도 검사한다.
	// object check
	for(int kk=0; kk<nMapCellMeshBlockCount; kk++)
	{
		XiahGameEngine::Map::MAPRENDER_MAPOBJECTLIST *pObjectList;
		XiahGameEngine::Map::MAPRENDER_MAPOBJECTLIST::iterator it;

		if( XiahMap::g_XiahMap.m_pMapRender->QueryMeshblockObjectList(nTargetXAry[kk], nTargetZAry[kk], &pObjectList) )
		{
			for(it = pObjectList->begin(); it != pObjectList->end(); it++)
			{
				XiahGameEngine::Map::CMapObjectRender *pObject = *it;

				int box_count = pObject->GetCollideBoxCount();
				if( box_count != 1 )	// 시간 관계상 충돌 박스가 1개인것 만 검사한다.
					continue;

				BBoxOBB3* map_object_bound = pObject->GetCollideBoxList();

				Vector3 vV1 = map_object_bound->m_BBoxAABB.m_vMin;
				Vector3 vV2 = map_object_bound->m_BBoxAABB.m_vMax;

				CXiahCharObject *pCharObject = (CXiahCharObject*)g_pMainChar->m_pObject;
				// 캐릭터 보다 높이 있으면 굳이 할 필요가 없지.
				if( vV2.y > pCharObject->m_Position.y + 10 )
					continue;

				XiahGameEngine::Face3 face[2];

				// 충돌 박스의 윗면
				face[0] = Face3( Vector3(vV1.x, vV2.y, vV1.z), Vector3(vV1.x, vV2.y, vV2.z), Vector3(vV2.x, vV2.y, vV1.z) );
				face[1] = Face3( Vector3(vV1.x, vV2.y, vV2.z), Vector3(vV2.x, vV2.y, vV2.z), Vector3(vV2.x, vV2.y, vV1.z) );

				// 직선이 면을 통과하는가?
				Vector3 vDelta;
				Vector3 vCollide;
				Vector3 vStartCollidePoint;
				float fDistance = 1000.0f;
				bool bCollide = false;
				for(int h=0; h<2; h++)
				{
					if( face[h].IsIntersect( vStart, vEnd ) )
					{
						vCollide = face[h].GetIntersectPoint( vStart, vEnd );
						vDelta = vStart - vCollide;

						float fLength = vDelta.GetLength();

						if( fLength < fDistance)
						{
							fDistance = fLength;
							vStartCollidePoint = vCollide;
						}

						bCollide = true;
					}// if
				}// for

				if( bCollide )
				{
					pos = vStartCollidePoint;
					pos.y = map_object_bound->m_HeightMax.y;

					return TRUE;
				}

			}// for

		}// if( QueryMeshblockObjectList )

	}// for



/*
	// 2004_04_19 Changth :
	// 이제, 캐릭터가 다리 위에 있을때, 엉뚱한 곳으로 가지않고 제대로 이동하도록 오브젝트도 검사한다.
	// object check
	if( bPositionFind &&
		 start.x > 0 &&  start.x < XiahMap::g_XiahMap.m_MapInfo.m_wWidth &&
		-start.z > 0 && -start.z < XiahMap::g_XiahMap.m_MapInfo.m_wHeight  )
	{
		XiahGameEngine::Map::MAPRENDER_MAPOBJECTLIST *pObjectList;
		XiahGameEngine::Map::MAPRENDER_MAPOBJECTLIST::iterator it;

		if( XiahMap::g_XiahMap.m_pMapRender->QueryMeshblockObjectList( start.x, -start.z, &pObjectList ) )
		{
			for(it = pObjectList->begin(); it != pObjectList->end(); it++)
			{
				XiahGameEngine::Map::CMapObjectRender *pObject = *it;

				int box_count = pObject->GetCollideBoxCount();
				if( box_count != 1 )	// 시간 관계상 충돌 박스가 1개인것 만 검사한다.
					continue;

				BBoxOBB3* map_object_bound = pObject->GetCollideBoxList();

				Vector3 vV1 = map_object_bound->m_BBoxAABB.m_vMin;
				Vector3 vV2 = map_object_bound->m_BBoxAABB.m_vMax;

				XiahGameEngine::Face3 face[2];

				// 충돌 박스의 윗면
				face[0] = Face3( Vector3(vV1.x, vV2.y, vV1.z), Vector3(vV1.x, vV2.y, vV2.z), Vector3(vV2.x, vV2.y, vV1.z) );
				face[1] = Face3( Vector3(vV1.x, vV2.y, vV2.z), Vector3(vV2.x, vV2.y, vV2.z), Vector3(vV2.x, vV2.y, vV1.z) );

				// 직선이 면을 통과하는가?
				Vector3 vDelta;
				Vector3 vCollide;
				Vector3 vStartCollidePoint;
				float fDistance = 1000.0f;
				bool bCollide = false;
				for(int h=0; h<2; h++)
				{
					if( face[h].IsIntersect( vStart, vEnd ) )
					{
						vCollide = face[h].GetIntersectPoint( vStart, vEnd );
						vDelta = vStart - vCollide;

						float fLength = vDelta.GetLength();

						if( fLength < fDistance)
						{
							fDistance = fLength;
							vStartCollidePoint = vCollide;
						}

						bCollide = true;
					}// if
				}// for

				if( bCollide )
				{
					pos = vStartCollidePoint;
					pos.y = map_object_bound->m_HeightMax.y;

					return TRUE;
				}

			}// for

		}// if

	}// if
*/



	return bPositionFind;
}

BOOL CXiahMap::RenderPortal()
{
	// 포탈은 이펙트로
	return TRUE;

	//////////////////////////////////////////////////////////////////////

	if( g_pCurrentCamera == NULL)
		return TRUE;

//	g_pDirect3DDevice->SetRenderState( D3DRS_LIGHTING, FALSE);
//	g_pDirect3DDevice->SetRenderState( D3DRS_ALPHABLENDENABLE, TRUE);
//	g_pDirect3DDevice->SetRenderState( D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
//	g_pDirect3DDevice->SetRenderState( D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

	g_pDirect3DDevice->SetRenderState( D3DRS_CULLMODE, D3DCULL_NONE);
	g_pDirect3DDevice->SetRenderState( D3DRS_ZWRITEENABLE, FALSE);

	PORTALINFOLIST::iterator it;

	IDirect3DTexture9 *pTexture = XiahPak::GetTexture( 50000205);

	for(it = m_PortalInfoList.begin(); it != m_PortalInfoList.end(); it++)
	{
		sPortalInfo &portal = *it;

		// 문파맵 이동 포탈.
		if( portal.m_bLinkType == 9 ) continue;
	
		BBoxAABB3 box;

		Vector3 start( portal.m_wPortalPosX, 0, -portal.m_wPortalPosY);
		Vector3 end( portal.m_wPortalPosX + portal.m_wPortalWidth, 255, -portal.m_wPortalPosY -portal.m_wPortalHeight);

		box.m_vMin = start.Min( end);
		box.m_vMax = start.Max( end);
	
		if( portal.m_bLinkType == 99)
			DrawBound( box, D3DCOLOR_ARGB( 128, 255, 180, 0), FALSE, pTexture);
		else
			DrawBound( box, D3DCOLOR_ARGB( 128, 0, 180, 255), FALSE, pTexture);
		
	};

	g_pDirect3DDevice->SetRenderState( D3DRS_CULLMODE, D3DCULL_CCW);
	g_pDirect3DDevice->SetRenderState( D3DRS_ZWRITEENABLE, TRUE);

//	g_pDirect3DDevice->SetRenderState( D3DRS_LIGHTING, TRUE);

	return TRUE;
}

BOOL CXiahMap::ClearAllDecal()
{
	if( !m_bCreated)
		return TRUE;

	return m_pMapRender->ClearVisibleMapDecalList();
}

BOOL CXiahMap::ChangeXiahEnvInfo()
{
	// 현재의 포그와 라이트가 서서히 변하도록 하기 위해
	// 비가 올때 포그가 시간에 따라서 서서히 변하도록 하기 위해.

	// 이것은 라이트 셋팅용
#ifdef LIGHTSET
	g_XiahChangeEnvInfo.bChangeStart = false;
	return TRUE;
#endif

	if( g_XiahChangeEnvInfo.bChangeStart )
	{
		// Diffuse : 0, FogColor : 1, SkyColorBottom : 2 ,SkyColorMiddle : 3, SkyColorUp : 4
		BOOL b1 = ChangeXiahEnvInfoData( g_XiahEnvInfo.m_DiffuseColor, 0 );
		BOOL b2 = ChangeXiahEnvInfoData( g_XiahEnvInfo.m_FogColor, 1 );
		BOOL b4 = ChangeXiahEnvInfoData( g_XiahEnvInfo.m_SkyColorBottom, 2 );
		BOOL b3 = ChangeXiahEnvInfoData( g_XiahEnvInfo.m_SkyColorMiddle, 3 );
		BOOL b6 = ChangeXiahEnvInfoData( g_XiahEnvInfo.m_SkyColorUp, 4 );
		BOOL b5 = ChangeXiahEnvInfoDensity();

		if( !b1 && !b2 && !b3 && !b4 && !b5 && !b6)
			g_XiahChangeEnvInfo.bChangeStart = false;

	}// if

	return TRUE;
}

BOOL CXiahMap::ChangeXiahEnvInfoData(D3DCOLOR& color, int nType)
{
	// 현재 값
	int nR = GetBValue( color );
	int nG = GetGValue( color );
	int nB = GetRValue( color );

	// 변화될 값.
	int nRGap = g_XiahChangeEnvInfo.nR[nType] - nR;
	int nGGap = g_XiahChangeEnvInfo.nG[nType] - nG;
	int nBGap = g_XiahChangeEnvInfo.nB[nType] - nB;

	// 새로운 증감치
	if( g_XiahChangeEnvInfo.fRGap[nType] == -300 )
	{
		// 지정된 시간 동안 서서히 변하도록 한다.
		float fValue = (float)nRGap / FOGCHANGETIME * 33.0f * g_fFrameScale;

		g_XiahChangeEnvInfo.fRGap[nType] = fValue;
		g_XiahChangeEnvInfo.fRGapSum[nType] = nR;
	}

	if( g_XiahChangeEnvInfo.fGGap[nType] == -300 )
	{
		float fValue = (float)nGGap / FOGCHANGETIME * 33.0f * g_fFrameScale;

		g_XiahChangeEnvInfo.fGGap[nType] = fValue;
		g_XiahChangeEnvInfo.fGGapSum[nType] = nG;
	}

	if( g_XiahChangeEnvInfo.fBGap[nType] == -300 )
	{
		float fValue = (float)nBGap / FOGCHANGETIME * 33.0f * g_fFrameScale;

		g_XiahChangeEnvInfo.fBGap[nType] = fValue;
		g_XiahChangeEnvInfo.fBGapSum[nType] = nB;
	}

	// 값 변화. 전체 증감값과 현재 변화될 값을 비교한다.
	if( fabs(g_XiahChangeEnvInfo.fRGap[nType]) < abs(nRGap) )
		g_XiahChangeEnvInfo.fRGapSum[nType] += g_XiahChangeEnvInfo.fRGap[nType];

	if( fabs(g_XiahChangeEnvInfo.fGGap[nType]) < abs(nGGap) )
		g_XiahChangeEnvInfo.fGGapSum[nType] += g_XiahChangeEnvInfo.fGGap[nType];

	if( fabs(g_XiahChangeEnvInfo.fBGap[nType]) < abs(nBGap) )
		g_XiahChangeEnvInfo.fBGapSum[nType] += g_XiahChangeEnvInfo.fBGap[nType];

	nR = g_XiahChangeEnvInfo.fRGapSum[nType];
	nG = g_XiahChangeEnvInfo.fGGapSum[nType];
	nB = g_XiahChangeEnvInfo.fBGapSum[nType];

	// Setting
	color = D3DCOLOR_XRGB( nR, nG, nB );

	// 비교
	if( g_XiahChangeEnvInfo.nR[nType] == nR &&
		g_XiahChangeEnvInfo.nG[nType] == nG &&
		g_XiahChangeEnvInfo.nB[nType] == nB   ) 
		return FALSE;

	return TRUE;
}

BOOL CXiahMap::ChangeXiahEnvInfoDensity()
{
	// 새로운 증감치
	float fDensityGap = g_XiahChangeEnvInfo.fFogDensity - g_XiahEnvInfo.m_fFogDensity;

	if( g_XiahChangeEnvInfo.fFogDensityGap == -300 )
	{
		float fValue = fDensityGap / FOGCHANGETIME * 33.0f * g_fFrameScale;

		g_XiahChangeEnvInfo.fFogDensityGap = fValue;
	}

	// 값 변화. 전체 증감값과 현재 변화될 값을 비교한다.
	if( fabs(g_XiahChangeEnvInfo.fFogDensityGap) < fabs(fDensityGap) ) 
		g_XiahEnvInfo.m_fFogDensity += g_XiahChangeEnvInfo.fFogDensityGap;
	else
		g_XiahEnvInfo.m_fFogDensity = g_XiahChangeEnvInfo.fFogDensity;

	// 비교
	if( g_XiahChangeEnvInfo.fFogDensity == g_XiahEnvInfo.m_fFogDensity )
		return FALSE;

	return TRUE;
}


//////////////////////////////////////////////////////////////////////////
// MAP ATTIRBUTE

cMap_Attr	XiahMap::g_Map_Attri;

#define BPP 1
#pragma pack(2)
		struct {
			TCHAR    signature[2];
			long    size;
			short   reserved1, reserved2;
			long    offbits;
		}   header ;
#pragma pack()

		struct {
			long    size, width, height;
			short   planes, bitcount;
			long    compression, sizecimage;
			long    xpelspermeter, ypelspermeter, clrused, clrimportant;
		}   info ;


		void cMap_Attr::Clear_Attr(void)
		{
			m_Width = 0;
			m_Height = 0;
			m_bLoaded = FALSE;

			memset(m_attir,0,MAX_MAPSIZE*MAX_MAPSIZE);
			
		}

		BOOL cMap_Attr::Load_Map_Attr(TCHAR *name)
		{		
			Clear_Attr();

			FILE *fp = _tfopen(name,_T("rb"));
			if(fp == NULL)
			{				
				DBG_Put(_T("Map attribute not found : %s"),name);
				return FALSE;
			}

			fread(&header, 1, sizeof(header), fp);
			if (memcmp(header.signature, _T("BM"), 2) != 0)
				return 0;
			fread(&info, 1, sizeof(info), fp);

//			unsigned char    palette[4*256];
//			if(info.bitcount > 8)
//			{
//				for(int x=0; x < 256; ++x)
//				{
//					palette[x*4+2] = (x%4)*256/3;
//					palette[x*4+1] = ((x/4)%8)*256/8;
//					palette[x*4+0] = ((x/32)%8)*256/8;
//				}
//			}
//
//			if (info.bitcount <= 8)
//				fread(palette, 4, (info.clrused==0?(1<<info.bitcount):info.clrused), fp);
			fseek(fp, header.offbits, SEEK_SET);

			m_Width  = info.width;
			m_Height = info.height;			
						
			for(int i=m_Width-1; i >= 0; i--)
			{
				fread(&m_attir[m_Width * i], 1, sizeof(unsigned char) * m_Height, fp);

//				for(int x=0; x<m_Height; x++)
//				{
//					switch(info.bitcount)
//					{
//					case 8 :
//						code = (unsigned char)fgetc(fp);						
//#if (BPP==1)
//						m_attir[m_Width*i+x] = (unsigned char)code;
//#endif
//
//#if (BPP==2)
//						code = RGB16(palette[code*4+2], palette[code*4+1], palette[code*4]);
//						memcpy(m_attir+(m_Width*i+x)*BPP, &code, BPP);
//#endif
//
//#if (BPP==3)
//						memcpy(m_attir+(m_Width*i+x)*BPP, palette+code*4, 3);
//#endif
//						break;
//
//					case 24 :
//						fread(palette, 3, 1, fp);
//
//#if (BPP==1)
//						m_attir[m_Width*i+x] = (palette[2]*4/256)+ (palette[1]*8/256)*4 + (palette[1]*8/256)*32;
//#endif
//
//#if (BPP==2)
//						code = RGB16(palette[2], palette[1], palette[0]);
//						memcpy(m_attir+(m_Width*i+x)*BPP, &code, BPP);
//#endif
//
//#if (BPP==3)
//						memcpy(m_attir+(m_Width*i+x)*BPP, palette, 3);
//#endif
//						break;
//					}
//				}

				fseek(fp, (4-((info.width*info.bitcount/8)&3))&3, SEEK_CUR);
			}

			fclose(fp);
			fp = NULL;

			m_bLoaded = TRUE;

			return TRUE;
		}

		unsigned char cMap_Attr::Get_Attr(int x, int y)
		{			
			if(m_bLoaded == FALSE)
				return 0;
			
			if(x < 0 || y < 0 || x >= MAX_MAPSIZE || y >= MAX_MAPSIZE)
			{
				return 0;
			}
		
			return m_attir[y * m_Width + x];
		}
