/*===================================================================================================
										MunpaBBSList.cpp
-----------------------------------------------------------------------------------------------------
	date :	2004/05/19  17:15
  Author :	
	
 Purpose :	
	
===================================================================================================*/
#include "precompile.h"
#include ".\munpabbslist.h"
#include "XiahGameObject.h"
#include "XiahGame_Handler_Sender.h"

CMunpaBBSList::CMunpaBBSList(void)
{
}

CMunpaBBSList::~CMunpaBBSList(void)
{
}

/**
 *
 */
void CMunpaBBSList::UpDate()
{
	DWORD dwTemp = m_dwSelect;
	if(CheckList())
	{
		if(dwTemp == m_dwSelect)
		{
			CXiahCharObject* pMainChar = reinterpret_cast<CXiahCharObject*>(g_pMainChar->m_pObject);

			SendCS_RL_MUNPABBSREAD_REQ(pMainChar->m_dwMunpaID, m_dwSelect);
		} // if(dwTemp == m_dwSelect)
	} // if(CheckList())
}


/////////////////////////////////////////////////////////////////////////////////////////////////////
CMunpaBBSListFactory::CMunpaBBSListFactory()
{
}

CMunpaBBSListFactory::~CMunpaBBSListFactory()
{
}

/**
 *
 * \return 
 */
CListBoxBase* CMunpaBBSListFactory::CreateProductList() const
{
	return new CMunpaBBSList();

	CListAbstractFactory::CreateProductList();
}