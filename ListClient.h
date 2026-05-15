/*===================================================================================================
										ListClient.h
-----------------------------------------------------------------------------------------------------
	date :	2004/05/19  17:30
  Author :	
	
 Purpose :	리스트 클라이언트
	
===================================================================================================*/
#pragma once

#include ".\listabstractfactory.h"
#include ".\listboxbase.h"
#include ".\munpabbslist.h"

/**
 * \ingroup XiahClient
 *
 * \date 2004-05-19
 */
class CListClient
{
public:
	enum Type 
	{
		MUNPA_BBS_LIST,
		MUNPA_BBS
	};

public:
	CListClient(void);
	CListClient(Type type);
	~CListClient(void);

	void Create(Type type);
	void UpDate();
	void Render();

	void Set(const long nPosX, const long nPosY, const long nWidth, const long nHeight, const int nType = -1);
	void AddString(const DWORD dwID, sString strTemp);
	DWORD DelString(const DWORD dwID, const int nType = 0);

private:
	CListBoxBase* m_pList;
	CListAbstractFactory* m_pListAbstractFactory;

private:

	void Destroy();

};
