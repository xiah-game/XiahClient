/*===================================================================================================
										ListClient.cpp
-----------------------------------------------------------------------------------------------------
	date :	2004/05/19  17:30
  Author :	
	
 Purpose :	
	
===================================================================================================*/
#include "precompile.h"
#include ".\listclient.h"

CListClient::CListClient(void) : m_pList(NULL), m_pListAbstractFactory(NULL)
{
}

CListClient::CListClient(Type type)
{
	Create(type);
}

CListClient::~CListClient(void)
{
	Destroy();
}


/**
 *
 * \param type 
 */
void CListClient::Create(Type type)
{
	switch(type)
	{
	//case MUNPA_BBS:
	//	break;
	case MUNPA_BBS_LIST:
		m_pListAbstractFactory = new CMunpaBBSListFactory;
		break;
	default:
		{
			return;
		}
		break;
	} // switch(type)

	m_pList = m_pListAbstractFactory->CreateProductList();
}

/**
 *
 */
void CListClient::Destroy()
{
	delete m_pList, m_pList = NULL;
	delete m_pListAbstractFactory, m_pListAbstractFactory = NULL;
}


/**
 *
 */
void CListClient::UpDate()
{
	m_pList->UpDate();
}

/**
 *
 */
void CListClient::Render()
{
	 m_pList->Render();
}



void CListClient::Set(const long nPosX, const long nPosY, const long nWidth, const long nHeight, const int nType)
{
	m_pList->Set(nPosX, nPosY, nWidth, nHeight, nType);
}

void CListClient::AddString(const DWORD dwID, sString strTemp)
{
	m_pList->AddString(dwID, strTemp);
}

DWORD CListClient::DelString(const DWORD dwID, const int nType)
{
	return m_pList->DelString(dwID, nType);
}