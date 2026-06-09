/*===================================================================================================
										ListBoxBase.h
-----------------------------------------------------------------------------------------------------
	date :	2004/05/19  16:54
  Author :	
	
 Purpose :	
	
===================================================================================================*/
#pragma once

#include "InterfaceDefine.h"

/**
 * \ingroup XiahClient
 *
 * \date 2004-05-19
 */
class CListBoxBase
{
public:

protected:
	struct ListData
	{
		sRect m_rtZone;
		CText2D	m_Text;

		DWORD m_dwTempValue;	// 예비 변수

		ListData() : m_dwTempValue(0)
		{
		}

	};

public:
	CListBoxBase(void);
	virtual ~CListBoxBase(void);

	virtual void UpDate() = 0;
	virtual void Render();
	virtual void Refresh();

	void Set(const long nPosX, const long nPosY, const long nWidth, const long nHeight, const int nType = -1);
	virtual void AddString(const DWORD dwID, sString strTemp);
	virtual DWORD DelString(const DWORD dwID, const int nType);

protected:

	virtual const bool CheckList();
	virtual void MakeSelectVB(sRect rtRect);

protected:

	std::map<DWORD, ListData*> m_mList;

	int m_nCount;
	int m_nType;			// 공용 타입
	DWORD m_dwSelect;
	bool m_bSelect;

	sPoint m_ptPos;
	sSize  m_Size;

	LPDIRECT3DVERTEXBUFFER9	m_pSelectVB;

};
