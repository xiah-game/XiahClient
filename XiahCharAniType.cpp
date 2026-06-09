#include "precompile.h"
#include "XiahCharAniType.h"

#include "XiahArrayIndex.h"
#include "XiahObjectType.h"

namespace XiahAniType
{
	//---------------------------------------------------------------------------------------
	CXiahChar_LogicalAnimationType::CXiahChar_LogicalAnimationType()
	{
		for(int i = 0; i < eLAT_Count; i++)
		{
			ANITYPE_LIST *pList = new ANITYPE_LIST;

#ifdef TRACE_LOG
			if(!pList)
			{
				DBG_LogFile( _T("CXiahChar_LogicalAnimationType 실패"));
			}
#endif
			push_back( pList);
		}
	}

	//---------------------------------------------------------------------------------------
	BOOL CXiahChar_LogicalAnimationType::Create(CArrayIndexData *pData)
	{
		CArrayIndexData::iterator it;

		for(it = pData->begin(); it != pData->end(); it++)
		{
			sArrayData *pData = *it;

#ifdef TRACE_LOG
			if(pData == NULL)
			{
				DBG_LogFile( _T("CXiahChar_LogicalAnimationType::Create 실패"));
			}
#endif

			int nMotionType = pData->GetInt( 0);

			DBG_Assert( nMotionType < size());
			ANITYPE_LIST *pList = operator [] (nMotionType);

#ifdef TRACE_LOG
			if(pList == NULL)
			{
				DBG_LogFile( _T("CXiahChar_LogicalAnimationType::Create 실패"));
			}
#endif

			pList->push_back( pData->GetInt( 1));
		}

		return TRUE;
	}

	//---------------------------------------------------------------------------------------
	CXiahChar_LogicalAnimationType::~CXiahChar_LogicalAnimationType()
	{
		Release();
	}

	//---------------------------------------------------------------------------------------
	BOOL CXiahChar_LogicalAnimationType::Release()
	{
		iterator it;

		for(it = begin(); it != end(); it++)
		{
			ANITYPE_LIST *pList = *it;

#ifdef TRACE_LOG
			if(pList == NULL)
			{
				DBG_LogFile( _T("CXiahChar_LogicalAnimationType::Release 실패"));
			}
#endif
			delete pList;
			pList = NULL;
		}

		clear();
		
		return TRUE;
	}

	//---------------------------------------------------------------------------------------
	int  CXiahChar_LogicalAnimationType::GetAniType(int nType,int index)
	{
		DBG_Assert( nType >= 0 && nType < size());

		ANITYPE_LIST* pList = operator [] (nType);

		if(pList == NULL)
		{
			DBG_LogFile( _T("CXiahChar_LogicalAnimationType::GetAniType 실패"));
			return -1;
		}

		if( pList->size() == 0)
			return -1;

		if( index == -1)
			index = rand() % pList->size();

		return pList->operator [] (index);
	}

	/*************************************************************************************************************
	..............................................................................................................
	......................SSSS...EEEEEE..PPPPP.....AA....RRRRR.....AA....TTTTTT...OOOO...RRRRR....................
	.....................SS..SS..EE......PP..PP...AAAA...RR..RR...AAAA.....TT....OO..OO..RR..RR...................
	.....................SS......EE......PP..PP..AA..AA..RR..RR..AA..AA....TT....OO..OO..RR..RR...................
	......................SSSS...EEEEEE..PPPPP...AAAAAA..RRRR....AAAAAA....TT....OO..OO..RRRR.....................
	.........................SS..EE......PP......AA..AA..RR.RR...AA..AA....TT....OO..OO..RR.RR....................
	.....................SS..SS..EE......PP......AA..AA..RR..RR..AA..AA....TT....OO..OO..RR..RR...................
	......................SSSS...EEEEEE..PP......AA..AA..RR..RR..AA..AA....TT.....OOOO...RR..RR...................
	..............................................................................................................
	*************************************************************************************************************/
	/*
		일단 캐릭터만 하겠다
	*/
	CXiahChar_LogicalAnimationType g_AniType[ XIAH_ANITYPE_COUNT];

	BOOL InitializeLogicalAnimationType()
	{
		for(int i = 0; i < XIAH_ANITYPE_COUNT; i++)
			g_AniType[ i].Create( &XiahArrayIndex::g_AniType[ i]);
	
		return TRUE;
	}
	
	BOOL ReleaseLogicalAnimationType()
	{
		// ?

		return TRUE;
	}
	
	// Object가 NPC인 경우 NpcType도 넣어 주어야 함
	CXiahChar_LogicalAnimationType* GetAniType(BYTE nObjectType,BYTE nNpcType)
	{
		if( nObjectType == OBJTYPE_PC)
			return &g_AniType[ 0];
		else if( nObjectType == OBJTYPE_NPC)
			return &g_AniType[ 1];
		else if( nObjectType == OBJTYPE_FUNCTIONALNPC)
			return &g_AniType[ 2];

		return NULL;
	}


};