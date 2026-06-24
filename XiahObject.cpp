#include "precompile.h"
#include "XiahObject.h"
#include "XiahGame_Pet.h"

XiahObject::CXiahObject *g_pMainChar = NULL;

namespace XiahObject
{

CXiahObjectManager	g_XiahObjectManager;
CXiahObject*		g_pMouseOnObject = NULL;

CXiahObject*		g_pMouseOnObjectSave = NULL;

//YS_0728 : BUGFIX
CXiahCharPool		g_XiahCharPool;
CXiahCharPool		g_XiahNpcPool;
CXiahCharPool		g_XiahPetPool;

//---------------------------------------------------------------------------------------
// XiahObject 생성자
CXiahObject::CXiahObject()
{
	m_pObject = NULL;
}

//---------------------------------------------------------------------------------------
// XiahObject 소멸자
CXiahObject::~CXiahObject()
{
	Release();
}

//---------------------------------------------------------------------------------------
// XiahObject 객체 소멸
BOOL CXiahObject::Release()
{
	//YS_0728 : BUGFIX
	if( m_pObject)
	{
		if( m_pObject->m_bPoolClass )	
		{
			CXiahCharObject* pXiahCharObject = (CXiahCharObject*)m_pObject;
			pXiahCharObject->DeleteClass();
			pXiahCharObject->m_bUse = false;			

			m_pObject = NULL;
		}
		else
		{
			delete m_pObject;
			m_pObject = NULL;
		}
	}

	return TRUE;
}

//---------------------------------------------------------------------------------------
// XiahObject 객체 생성
BOOL CXiahObject::Create(CXiahObject_Basic *pObject)
{
	DBG_Assert( pObject != NULL);
#ifndef MASTER
	if(pObject == NULL)
	{
		FILE *fp = NULL;
		fp = fopen("OBJECTERR.TXT","wt");

		if(fp == NULL)
		{
			DBG_LogFile( _T("CXiahObject::Create/ fopen 실패"));

			//return false;
		}

		fprintf(fp,"CXiahObject::Create ERROR %x \n",this);
		fclose(fp);

		fp = NULL;
	}

#endif

	m_pObject = pObject;

	return TRUE;
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

CXiahObject g_XiahObjectInstanceList[ CLIENT_OBJECT_MAX];		

//---------------------------------------------------------------------------------------
//	오브젝트 메네저 OBJECT POOL 사용
//---------------------------------------------------------------------------------------
CXiahObjectManager::CXiahObjectManager()
{
	for(int i = 0; i < CLIENT_OBJECT_MAX; i++)
	{
		CXiahObject *pObject = &g_XiahObjectInstanceList[ i];

		if(pObject == NULL)
		{
			DBG_LogFile( _T("CXiahObjectManager 실패"));
		}
	
		pObject->m_dwClientID = i + 1;
		pObject->m_dwServerID = 0;
		
		m_FreeObjectList.push_back( pObject);
	}
}

//---------------------------------------------------------------------------------------
CXiahObjectManager::~CXiahObjectManager()
{
}

//---------------------------------------------------------------------------------------
//	비워진 OBJECT를 얻어낸다!
//---------------------------------------------------------------------------------------
CXiahObject *CXiahObjectManager::GetFreeObject()
{
	if( m_FreeObjectList.size() == 0)	// m_FreeObjectList는 비워져서 대기중인 OBJECT
		return NULL;

	CXiahObject *pObject = m_FreeObjectList.front();	// 앞에서 하나 얻어낸다

	if(pObject == NULL)
	{
		DBG_LogFile( _T("GetFreeObject 실패"));
	}

	m_FreeObjectList.pop_front();	// 얻어낸 오브젝을 리스트에서 뺀다
	return pObject;
}

//---------------------------------------------------------------------------------------
//	해제한 오브젝 반환
//---------------------------------------------------------------------------------------
BOOL CXiahObjectManager::ReleaseObject(CXiahObject *pObject)
{
	if(!pObject)
	{
		DBG_LogFile( _T("ReleaseObject 실패"));
	}

	if( g_pMouseOnObject == pObject)
		g_pMouseOnObject = NULL;
	if(g_pMouseOnObjectSave == pObject)
		g_pMouseOnObjectSave = NULL;

	pObject->Release();
	m_FreeObjectList.push_back( pObject);
	return TRUE;
}

//---------------------------------------------------------------------------------------
CXiahObject *CXiahObjectManager::CreateXiahObject(DWORD ServerID,BYTE bObjType,CXiahObject_Basic *pInfo)
{
	// 빈 오브젝을 얻어오고
	CXiahObject *pObject = GetFreeObject();

#ifdef TRACE_LOG
	if(pObject == NULL || pInfo == NULL)
	{
		DBG_LogFile( _T("CreateXiahObject 실패"));
	}
#endif

	pObject->m_dwServerID = ServerID;

	// CXiahObject에 CXiahObject_Basic 정보를 넣고
	if( !pObject->Create( pInfo))
	{
		ReleaseObject( pObject);	
		return NULL;
	}

	if( ServerID == 0)
		pObject->m_ddwObjectID = MAKEOBJECTID(pObject->m_dwClientID, pObject->m_dwServerID, bObjType);
	else
		pObject->m_ddwObjectID = MAKEOBJECTID( 0, pObject->m_dwServerID, bObjType);

	insert( value_type( pObject->m_ddwObjectID, pObject));

	// 아싸, 이걸로 고생을 덜었다.
	pInfo->m_dwXiahObjectID = pObject->m_ddwObjectID;

	return pObject;
}

//---------------------------------------------------------------------------------------
BOOL CXiahObjectManager::ReleaseXiahObject(CXiahObject *pObject)
{
	iterator it = find( pObject->m_ddwObjectID);

	if(it == end()) return FALSE;

	if (pObject->m_pObject && pObject->m_pObject->m_bObjType == OBJTYPE_PET)
	{
		g_PetList.DeletePet(pObject->m_dwServerID);
	}

	erase( it);
	ReleaseObject( pObject);
	return TRUE;
}

//---------------------------------------------------------------------------------------
CXiahObject *CXiahObjectManager::FindXiahObject(OBJECTID id)
{
	iterator it = find( id);

	if( it == end())
		return NULL;

	//YS_0811 : BUGFIX
	CXiahObject* pXiahObject	 = NULL;
	CXiahObject* pTempXiahObject = (CXiahObject*)it->second;
	
	if ( pTempXiahObject && pTempXiahObject->m_pObject )
	{
		if ( pTempXiahObject->m_pObject->m_bPoolClass == true )
		{
			if ( pTempXiahObject->m_pObject->m_bUse == true )
			{
				pXiahObject = pTempXiahObject;
			}
			else
			{
				ReleaseXiahObject(id);
			}
		}
		else
		{
			pXiahObject = pTempXiahObject;
		}		
	}

	return pXiahObject;
}
 
//---------------------------------------------------------------------------------------
BOOL CXiahObjectManager::ReleaseXiahObject(OBJECTID objectID)
{
	iterator it = find( objectID);

	if(it == end())
		return NULL;
	
	return ReleaseXiahObject( it->second);
}

BOOL CXiahObjectManager::Release()
{
	iterator it;

	for(it = begin(); it != end(); it++)
	{
		if(false)
		{
			DBG_LogFile( _T("CXiahObjectManager::Release 실패"));
		}

		ReleaseObject( it->second);
	}

	clear();

	return TRUE;
}

BOOL CXiahObjectManager::ChangeToClientObject(CXiahObject *pObject)
{
	iterator it = find( pObject->m_ddwObjectID);

	DBG_Assert( it != end());
	if(it != end()) erase( it);	// 일단 지워주고

	// ID를 다시 만들어 준다
	pObject->m_dwServerID = 0;
	pObject->m_ddwObjectID = MAKEOBJECTID( pObject->m_dwClientID, 0, 0);

	insert( value_type( pObject->m_ddwObjectID, pObject));

	return TRUE;
}

BOOL CXiahObjectManager::ReleaseAllObjectExceptMainChar()
{
	std::vector<CXiahObject*> ObjList;

	for(iterator it = begin(); it != end(); ++it)
	{
		CXiahObject*pObject = it->second;

		if(pObject == NULL)
		{
			DBG_LogFile( _T("CXiahObjectManager::ReleaseAllObjectExceptMainChar 실패"));

			continue;
		}

		if( pObject != g_pMainChar && g_PetList.Find( pObject->m_dwServerID) == NULL)
			ObjList.push_back( pObject);
	}

	std::vector<CXiahObject*>::iterator o_it;

	for(o_it = ObjList.begin(); o_it != ObjList.end(); ++o_it)
	{
		if(false)
		{
			DBG_LogFile( _T("CXiahObjectManager::ReleaseAllObjectExceptMainChar 실패"));

			continue;
		}

		ReleaseXiahObject( *o_it);
	}

	ObjList.clear();
	return TRUE;
}

BOOL CXiahObjectManager::ChangeObjectID(DWORD dwSourceClientID,DWORD dwSourceServerID,BYTE bSourceType,
								  DWORD dwTargetClientID,DWORD dwTargetServerID,BYTE bTargetType)
{
	iterator it;
	it = find( MAKEOBJECTID( dwSourceClientID, dwSourceServerID, bSourceType));

	if( it == end())
		return FALSE;

	if(it == end()) return FALSE;
	CXiahObject* pObject = it->second;

	if(pObject == NULL)
	{
		DBG_LogFile( _T("CXiahObjectManager::ChangeObjectID 실패"));
		return FALSE;
	}
	
	// 일단 지우고
	erase( it);

	pObject->m_dwClientID = dwTargetClientID;
	pObject->m_dwServerID = dwTargetServerID;
	pObject->m_pObject->m_bObjType = bTargetType;

	pObject->m_ddwObjectID = MAKEOBJECTID( dwTargetClientID, dwTargetServerID, bTargetType);
	// 다시 등록
	insert( value_type( pObject->m_ddwObjectID, pObject));

	return TRUE;
}

//YS_0728 : BUGFIX
CXiahCharPool::CXiahCharPool()
{
	m_nCreateCount = 0;
	m_nCurNum = 0;
}

CXiahCharPool::~CXiahCharPool()
{
	for( int i = 0; i < m_nCreateCount; i++ )
	{
		delete m_Data[i].pInfo;
	}
}

//YS_0812 : BUGFIX
bool CXiahCharPool::Create(int nCreate)
{
	m_nCreateCount = nCreate;

	for(int i = 0; i < m_nCreateCount;/*MAX_CHARPOOLCLASS;*/ i++ )
	{
		m_Data[i].pInfo							= new CXiahCharObject;
		m_Data[i].pInfo->m_bUse					= false;
		m_Data[i].pInfo->m_bPoolClass			= true;
	}

	return true;
}

CXiahObject_Basic* CXiahCharPool::GetChar()
{			
	CXiahCharObject* pCharObject = NULL;

	for ( int i = 0; i < m_nCreateCount; i++ )
	{
		if ( m_nCurNum >= m_nCreateCount - 5 )
		{
			m_nCurNum = 0;
		}

		pCharObject = (CXiahCharObject*)m_Data[m_nCurNum].pInfo;

		if ( pCharObject->m_bUse == false )
		{
			pCharObject->InitClass();
			pCharObject->m_bUse				= true;			
			m_nCurNum++;
			break;
		}

		m_nCurNum++;	

		pCharObject = NULL;
	}

	return (CXiahObject_Basic*)pCharObject;
}

void CXiahCharPool::AllClear (void)
{		
	for(int i = 0; i < m_nCreateCount; i++ )
	{
		//YS_0812 : BUGFIX
		if ( m_Data[i].pInfo && false == m_Data[i].pInfo->m_bPetPool )
		{
			m_Data[i].pInfo->m_bUse	= false;
		}						
	}
}
//..BUGFIX

}
