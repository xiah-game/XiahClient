#pragma once

#include "ItemInfo.h"
#include "XiahObjectType.h"

/**
 * \ingroup XiahClient
 *
 * \date 2004-07-15
 */
class CHoldItem
{
public:
	// [3/5/2004] 임시로 열어둠. -__- 시간=_=
	BYTE m_bBackPosition;

	CHoldItem(void);
	~CHoldItem(void);

	// Draw
	void DrawHoldItem();

	// Check
	BOOL IsHoldingItem();
	BOOL IsHoldingItemItem();
	BOOL IsHoldingItemMugong();
	BOOL IsHoldingItemMoney();

	// Get
	XiahItem::sItemInfo*	GetHoldItemItem() { return m_pHoldItemItem;};
	DWORD					GetHoldItemMugong() { return m_dwHoldItemMugong;};
	XiahItem::sHoldMoney*	GetHoldItemMoney() { return m_pHoldItemMoney;};
	XiahItem::sItemInfo*	GetHoldItemItemFromNpcSack() { return m_pHoldItemItemFromNpcSack;};

	// Set
	void SetHoldItemItem( XiahItem::sItemInfo* pItem, BOOL bFlag=TRUE);
	void SetHoldItemMoney( DWORD dwAmount);
	void SetHoldItemMoneySack( BYTE bySrcSackID);
	void ReleaseHoldItemMoney();
	void SetHoldItemMugong( DWORD dwMugong);
	void SetHoldItemItemFromNpcSack( XiahItem::sItemInfo* pItem);
	void SetDrawFlag( BOOL bFlag) { m_fDraw = bFlag;};

	// HoldItemItem : additional function
	void SetItemBackToSack();
	void DeleteHoldItemItem();
	void EmptyHoldItemItem();
	void ThrowItem( BYTE byType=0);
	void ThrowMoney();

private:
	void CreateHoldItemVB();
	void MakeHoldItemVB( BYTE byType=1);
	
	void DrawHoldItemMugong();
	void DrawHoldItemItem();
	void DrawHoldItemMoney();

	LPDIRECT3DVERTEXBUFFER9	m_pHoldItemItemVB;		// hold item vertex buffer

	BOOL					m_fDraw;	// 그릴까?

	// VB가 필요한 HoldItem
	XiahItem::sItemInfo*	m_pHoldItemItem;		// hold item 정보
	DWORD					m_dwHoldItemMugong;		// hold mugong 정보
	XiahItem::sHoldMoney*	m_pHoldItemMoney;

	// VB가 필요없는 HoldItem
	XiahItem::sItemInfo*	m_pHoldItemItemFromNpcSack;
};
