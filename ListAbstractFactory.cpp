/*===================================================================================================
										ListAbstractFactory.cpp
-----------------------------------------------------------------------------------------------------
	date :	2004/05/19  16:46
  Author :	
	
 Purpose :	
	
===================================================================================================*/
#include "precompile.h"
#include ".\listabstractfactory.h"

CListAbstractFactory::CListAbstractFactory(void)
{
}

CListAbstractFactory::~CListAbstractFactory(void)
{
}

/**
 *
 * \return 
 */
CListBoxBase* CListAbstractFactory::CreateProductList() const
{
	return (CListBoxBase*)0;
}