#include "precompile.h"
#include "ArrayIndexData.h"

#define MAX_ARRAYINDEXFILE_SIZE	500 * 1024
BYTE g_ArrayIndexFileTempBuffer[ MAX_ARRAYINDEXFILE_SIZE];

CArrayIndexData::CArrayIndexData()
{
	m_nIntCount = m_nStringCount = 0;
}

CArrayIndexData::~CArrayIndexData()
{
	Release();
}

#define GET_INT *((int *)pData);pData+=4;

BOOL CArrayIndexData::Create(LPCTSTR filename)
{
	FILE *fp = NULL; // 초기화좀 해줍시다.

	fp = _tfopen( filename, _T("rb"));
	
	if( fp == NULL)
		return FALSE;

	long start = ftell( fp);
	fseek( fp, 0, SEEK_END);
	long end = ftell( fp);
	fseek( fp, 0, SEEK_SET);
	
	long file_size = end - start;

	fread( g_ArrayIndexFileTempBuffer, file_size, 1, fp);
	
	fclose( fp);

	LPBYTE pData = g_ArrayIndexFileTempBuffer;

	int nCount = GET_INT;
	m_nIntCount = GET_INT;
	m_nStringCount = GET_INT;

	for(int i = 0; i < nCount; ++i)
	{
		sArrayData *pArrayData = new sArrayData;

		if(pArrayData)
		{
			for(int j = 0; j < m_nIntCount; j++)
			{
				int data = GET_INT;
				pArrayData->m_IntList.push_back( data);
			}

			for(int j = 0; j < m_nStringCount; j++)
			{
				unsigned short size = *(unsigned short *)pData;
				pData+= 2;

				sString str;

				str = (LPCTSTR)pData;

				pData += size;

				pArrayData->m_StringList.push_back( str);
			}

			push_back( pArrayData);
		}
		else
		{
			DBG_LogFile( _T("BOOL CArrayIndexData::Create에서 실패"));
		}
	}

	return TRUE;
}

BOOL CArrayIndexData::Release()
{
	for(iterator it = begin(); it != end(); ++it)
	{
		sArrayData *pData = *it;

		delete pData;
		pData = NULL;
	}

	clear();
	
	return TRUE;
}

sArrayData *CArrayIndexData::GetSkipData(int nSkip,int index)
{
	DBG_Assert( m_nIntCount >= 1);

	int nCount = 0;

	for(iterator it = begin(); it != end(); ++it)
	{
		sArrayData *pData = *it;

		if(pData)
		{
			if( pData->m_IntList[ 0] == index)
			{
				if(nCount == nSkip)
					return pData;

				++nCount;
			}
		}
	}

	return NULL;
}

sArrayData *CArrayIndexData::GetData(int index)
{
	DBG_Assert( m_nIntCount >= 1);

	for(iterator it = begin(); it != end(); ++it)
	{
		sArrayData *pData = *it;
		
		if(pData)
		{
			if( pData->m_IntList[ 0] == index)
				return pData;
		}
	}

	return NULL;
}

sArrayData *CArrayIndexData::GetData(int index_1,int index_2)
{
	DBG_Assert( m_nIntCount >= 2);

	for(iterator it = begin(); it != end(); ++it)
	{
		sArrayData *pData = *it;

		if(pData)
		{
			if( pData->m_IntList[ 0] == index_1 &&
				pData->m_IntList[ 1] == index_2)
				return pData;
		}		
	}

	return NULL;
}

sArrayData *CArrayIndexData::GetData(int index_1,int index_2,int index_3)
{
	DBG_Assert( m_nIntCount >= 3);

	for(iterator it = begin(); it != end(); ++it)
	{
		sArrayData *pData = *it;

		if(pData)
		{
			if( pData->m_IntList[ 0] == index_1 &&
				pData->m_IntList[ 1] == index_2 &&
				pData->m_IntList[ 2] == index_3	)
				return pData;
		}		
	}

	return NULL;
}

sArrayData *CArrayIndexData::GetData(int index_1,int index_2,int index_3,int index_4)
{
	DBG_Assert( m_nIntCount >= 4);

	for(iterator it = begin(); it != end(); ++it)
	{
		sArrayData *pData = *it;

		if(pData)
		{
			if( pData->m_IntList[ 0] == index_1 &&
				pData->m_IntList[ 1] == index_2 &&
				pData->m_IntList[ 2] == index_3 &&
				pData->m_IntList[ 3] == index_4	)
				return pData;
		}
	}

	return NULL;
}



