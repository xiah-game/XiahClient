#include "precompile.h"
#include "XiahObject.h"
#include "XiahGameObject.h"
#include "XiahMap.h"
#include "XiahGame_BGM.h"
#include "XiahGame_BGM_Data.cpp"
#include "characterinfo.h"
#include "AppData.h"

#include "XiahBGMcore.h"

#define	PLAYING_BGM			0
#define	PLAYING_AMBIENT		1


BOOL	g_bBGMForce;
long	g_changeBGMtime;

void InitXiahBGM()
{
	g_bBGMForce = FALSE;
	g_changeBGMtime = 0;
}

void ReleaseXiahBGM()
{

}

/*
	BGMStatus:
				0 -> BGM출력
				1 -> Ambience출력
*/

extern	int g_nHour;
static	int pre_pos = -1;
static	int g_BGMStatus = PLAYING_AMBIENT;


void ProcessXiahBGM(BOOL bForce)
{
	TCHAR *filename  = NULL;
	// 현재 위치를 가지고 BGM를 선택해서 연주한다
	if( g_pMainChar == NULL) return;
	int index = 0;

	CXiahCharObject* pCharObject = (CXiahCharObject*)g_pMainChar->m_pObject;

	if(g_bBGMForce == TRUE)
	{
		// 음악이 끝나서 다음 음악으로 바꾸어 준다
		// 33% 확률로 BGM
		int t = rand() % 3;

		// 무조건 처음에 BGM으로 해달라는 요청으로...
		if(bForce)
			t = 0;

		if(t == 0)	
		{
			g_BGMStatus = PLAYING_BGM;
			index = 0;
		}
		else
		{
			g_BGMStatus = PLAYING_AMBIENT;
			index = 2;	// Ambience
		}

		GetBGM_index(pCharObject->m_Position, index, XiahMap::g_XiahMap.m_MapInfo.m_dwMapID);

		pre_pos = GetBGM_index(pCharObject->m_Position, 2, XiahMap::g_XiahMap.m_MapInfo.m_dwMapID);
		filename = GetBGM(pCharObject->m_Position, index, XiahMap::g_XiahMap.m_MapInfo.m_dwMapID);

		long c_time = timeGetTime();
		if(g_changeBGMtime + 5000 < c_time)
		{
/*
#ifdef WLOG
			if(g_pLog)
			g_pLog->Log("음악이 끝나서 BGM 바뀜 %s",filename);
#endif
*/
			Change_BGM(filename);
			g_changeBGMtime = c_time;
			g_bBGMForce = FALSE;
		}
	}
	else
	{
		// 무조건 처음에 BGM으로 해달라는 요청으로...

		// 음악이 끝나지 않았는데 다른곳으로 이동하여 바꿔주는 경우는 무조건 Ambience로 시작
		if(bForce)
			index = 0;	// BGM
		else
			index = 2;	// Ambience

		int new_pos = GetBGM_index(pCharObject->m_Position, index, XiahMap::g_XiahMap.m_MapInfo.m_dwMapID);

		if(pre_pos != new_pos && new_pos != -1)
		{
			// 새로운 음악!
			filename = GetBGM(pCharObject->m_Position, index, XiahMap::g_XiahMap.m_MapInfo.m_dwMapID);
			long c_time = timeGetTime();
			if(g_changeBGMtime + 5000 < c_time)
			{
/*
#ifdef WLOG
				if(g_pLog)
				g_pLog->Log("BGM 바뀜 %s (%d -> %d)",filename,pre_pos,new_pos);
#endif
*/
				Change_BGM(filename);
				g_changeBGMtime = c_time;
				pre_pos = new_pos;
				Change_BGM(filename);
				g_BGMStatus = PLAYING_AMBIENT;
			}
		}
	}
}
