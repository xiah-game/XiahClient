/*===================================================================================================
										ListAbstractFactory.h
-----------------------------------------------------------------------------------------------------
	date :	2004/05/19  16:46
  Author :	
	
 Purpose :	리스트 추상 팩토리
			현재 인터페이스 엔진상 리스트 박스를 넣지 못해서 클라이언트에 붙였음.
			(인터페이스 툴을 예전것을 사용하기에...)
	
===================================================================================================*/
#pragma once

#include ".\listboxbase.h"

/**
 * \ingroup XiahClient
 * 리스트 추상 팩토리
 * \date 2004-05-19
 */
class CListAbstractFactory
{
public:
	CListAbstractFactory(void);
	virtual ~CListAbstractFactory(void);

	virtual CListBoxBase* CreateProductList() const;
};
