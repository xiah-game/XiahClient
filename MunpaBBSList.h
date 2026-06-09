/*===================================================================================================
										MunpaBBSList.h
-----------------------------------------------------------------------------------------------------
	date :	2004/05/19  17:15
  Author :	
	
 Purpose :	
	
===================================================================================================*/
#pragma once
#include "listboxbase.h"
#include ".\listabstractfactory.h"

/**
 * \ingroup XiahClient
 *
 * \date 2004-05-19
 */
class CMunpaBBSList :
	public CListBoxBase
{
public:
	CMunpaBBSList(void);
	virtual ~CMunpaBBSList(void);

	virtual void UpDate();

protected:

private:

};


/**
 * \ingroup XiahClient
 *
 * \date 2004-05-19
 */
class CMunpaBBSListFactory : public CListAbstractFactory
{
public:
	CMunpaBBSListFactory();
	virtual ~CMunpaBBSListFactory();

	virtual CListBoxBase* CreateProductList() const;

protected:

private:

};