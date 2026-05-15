#pragma once
#include "sack.h"

/**
 * \ingroup XiahClient
 *
 * \date 2004-07-15
 */
class CEquipSack : public CSack
{
private:
	virtual BOOL CheckItemUnSelected();	
	//virtual void SetVB( BYTE nPosition, XiahItem::sItemInfo* pItem);
	virtual void SetSackRegion();

	void CreateEquipVB();
	
	//EquipSack durability에 따른 화면 출력
private:
	DWORD m_dwTime;
	bool m_bRepairShow;

	LPDIRECT3DVERTEXBUFFER9	m_pEquipVB[9];
	LPDIRECT3DTEXTURE9		m_pEquipTex[9];	

public:
	CEquipSack( BYTE byType, BYTE byTotalSize);
	virtual ~CEquipSack(void);

	virtual BOOL InsertItem( BYTE bSackPos, XiahItem::sItemInfo* pItem);
	// 누수 수정
	virtual void DeleteItem( BYTE bSackPos, bool bDelete = false);
	virtual XiahItem::sItemInfo* FindSackItemByPos( int nPosition);	
	virtual XiahItem::sItemInfo* FindSackItemByPosPrev( int nPosition);

	void CheckEquipShortEndu();
	void DrawEquipShortEndu();
};
