#pragma once

#include "sack.h"

#define DEFAULT_CELL_DISTANCE		3		// cell 사이의 거리
#define DEFAULT_SACK_XSIZE			6		// 가로 cell의 개수
#define DEFAULT_SACK_YSIZE			6		// 세로 cell의 개수


/**
 * \ingroup XiahClient
 *
 * \date 2004-07-15
 */
class CCharSack :
	public CSack
{
public:	

	CCharSack( BYTE bySackType = SACKTYPE__DEFAULT, BYTE bySackXSize = DEFAULT_SACK_XSIZE, BYTE bySackYSize = DEFAULT_SACK_YSIZE);
	virtual ~CCharSack(void);	

	virtual void DrawSack();	
	virtual void RefreshSackPos();

private:
	int		m_nFirstCellXPos;
	int		m_nFirstCellYPos;
	BYTE	m_byCellXSize;
	BYTE	m_byCellYSize;
	BYTE	m_byCellDis;
	BYTE	m_bySackXSize;
	BYTE	m_bySackYSize;

	LPDIRECT3DVERTEXBUFFER9	m_pMoneyVB;
	LPDIRECT3DTEXTURE9		m_pMoneyTex;

	// 누수 수정
	virtual void DeleteItem( BYTE bSackPos, bool bDelete = false);
	virtual BOOL CheckItemUnSelected();
	virtual void SetVB( BYTE nPosition, XiahItem::sItemInfo* pItem);
	virtual void SetSackRegion();
	virtual XiahItem::sItemInfo* FindSackItemByPos( int nPosition);

	void DrawMoney();
};
