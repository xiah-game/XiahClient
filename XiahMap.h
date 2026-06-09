#pragma once


/*
	실제 게임상에서는 각각의 LinkMap에 대한 Local좌표를 사용하지만, 
	
	게임 화면상에서는 WorldMap을 보여준다
*/
using namespace std;

namespace XiahMap
{
	//////////////////////////////////////////////////////////////////////////
	// MAP ATTRIBUTE

#define MAX_MAPSIZE	2048

	class cMap_Attr
	{
		private :
			int		m_Width,m_Height;
			BOOL	m_bLoaded;
			unsigned char	m_attir[MAX_MAPSIZE * MAX_MAPSIZE];
		public :

			BOOL	Load_Map_Attr(TCHAR *name);
			void	Clear_Attr(void);
			unsigned char Get_Attr(int x,int y);

			cMap_Attr() { Clear_Attr();	}
			~cMap_Attr(){};
	};


	extern cMap_Attr	g_Map_Attri;

	//////////////////////////////////////////////////////////////////////////

	struct sPortalInfo
	{
		DWORD	m_dwLinkMapID;
		WORD	m_wPortalPosX;
		WORD	m_wPortalPosY;
		WORD	m_wPortalWidth;
		WORD	m_wPortalHeight;
		BYTE	m_bLinkType;
	};

	typedef list<sPortalInfo>	PORTALINFOLIST;

	struct sMapInfo
	{
		DWORD	m_dwMapID;
		sString	m_szMapName;
		BYTE	m_bType;
		WORD	m_wWidth;
		WORD	m_wHeight;
	};

	class CXiahMap
	{
	public:
		CXiahMap();
		~CXiahMap();
	
		BOOL CreateMap( DWORD dwMapID, LPCTSTR szMapName, BYTE bType, WORD wWidth, WORD wHeight);
		BOOL ReleaseMap();

		BOOL Update();
		BOOL RenderTerrain();
		BOOL RenderObject(BYTE byType);
		BOOL RenderWater();

		BOOL UpdateGrassZone();
		BOOL RenderGrassZone();

		BOOL ChangeXiahEnvInfo();
		BOOL ChangeXiahEnvInfoData(D3DCOLOR& color, int nType);
		BOOL ChangeXiahEnvInfoDensity();

		sPortalInfo* IntersectPortal(WORD wPosX,WORD wPosY);

		BOOL GetPickPosition(Vector3 &pos);	// 졸라 느림 주의 바람
		BOOL RenderPortal();

		BOOL ClearAllDecal();

	public:
		BOOL	m_bCreated;

		sMapInfo m_MapInfo;

		PORTALINFOLIST m_PortalInfoList;
		Map::CMapRender* m_pMapRender;
	};

	extern CXiahMap g_XiahMap;
};