#pragma once

#include "ItemInfo.h"
#include "XiahObjectType.h"
#include "XiahArrayIndex.h"

typedef vector< XiahItem::sItemInfo* > ItemList;
typedef vector< LPDIRECT3DVERTEXBUFFER9 > ItemVB;
typedef vector< LPDIRECT3DTEXTURE9 > ItemTex;
typedef vector< sRect* > ItemRt;

#define DEFAULT_CELL_XSIZE			38		// cell 의 X size
#define DEFAULT_CELL_YSIZE			38		// cell 의 Y size

#define OPEN_NORMAL			1		// NPC를 통하여 열었다
#define OPEN_ITEM			2		// ITEM을 통하여 열었다

/*
Sack
 |-----------------|
 |                 |
 EquipSack         CharSack
                   |
				   |----------| --------------|-----------------|
				   MySack     NpcTradeSack    PcTradeSackMine   PcTradeSackOther
*/


/**
 * \ingroup XiahClient
 *
 * \date 2004-07-15
 */
class CSack
{
public:

	CSack();
	virtual ~CSack();

	void HideSack(BOOL bFlagForModifySack = TRUE);
	void ShowSack();
	void SetAction(BYTE	bAction) { m_bAction = bAction; }

	virtual void DrawSack();

	BOOL IsShow() { return m_bShow;};
	BOOL CheckItemSelected();						// m_HoldItem이 없을때 마우스가 놓일때 자기 sack의 어떤 item에서 놓여졌는지
	BOOL CheckItemSelectedByLButton();
	BOOL CheckItemSelectedByRButton();
	virtual BOOL CheckItemUnSelected();				// m_HoldItem이 있을때 마우스가 놓일때 자기 sack의 어떤 pos에서 놓여졌는지

	virtual BOOL InsertItem(BYTE bSackPos, XiahItem::sItemInfo* pItem);
	// 누수 수정
	virtual void DeleteItem(BYTE bSackPos, bool bDelete = false);

	virtual XiahItem::sItemInfo* FindSackItemByPos( int nPosition);
	virtual XiahItem::sItemInfo* FindSackItemByPosPrev( int nPosition);
	XiahItem::sItemInfo* FindSackItemByID( int nID);
	XiahItem::sItemInfo* FindSackItemByVisualID( int nVisualID);

	virtual void RefreshSackPos();

	void SetRefreshToolTip(BOOL bFlag) { m_bRefreshToolTip = bFlag;};

	int HowManyItem( int nVisualID);
	D3DCOLOR MoneyUnitColor(DWORD dwAmount);

protected:

	virtual void SetVB( BYTE nPosition, XiahItem::sItemInfo* pItem);
	virtual void SetSackRegion();

	IDirect3DTexture9* Gettex( int nResID);
	BOOL ProcessItemUnSelected( BYTE byPosition);

	void CreateSocketVB(BYTE bSackPos, XiahItem::sItemInfo* pItem);

private:

	BOOL ProcessSackUnSelected( BYTE byPosiont);
	BOOL ProcessMoneyUnSelected( BYTE byPosition);
	BOOL ProcessItemUnSelected_Default_Default( BYTE byPosition, XiahItem::sItemInfo* pHoldItem);
	BOOL ProcessItemUnSelected_Default_Shop( BYTE byPosition, XiahItem::sItemInfo* pHoldItem);
	BOOL ProcessItemUnSelected_Default_Equip( BYTE byPosition, XiahItem::sItemInfo* pHoldItem);
	BOOL ProcessItemUnSelected_Default_TradeMine( BYTE byPosition, XiahItem::sItemInfo* pHoldItem);	
	BOOL ProcessItemUnSelected_Default_Deposit( BYTE byPosition, XiahItem::sItemInfo* pHoldItem);
	BOOL ProcessItemUnSelected_Default_Modify( BYTE byPosition, XiahItem::sItemInfo* pHoldItem);
	BOOL ProcessItemUnSelected_Default_Pet( BYTE byPosition, XiahItem::sItemInfo* pHoldItem);
	BOOL ProcessItemUnSelected_Default_PetEquip( BYTE byPosition, XiahItem::sItemInfo* pHoldItem);
	bool ProcessItemUnSelected_Default_Personal_Trade_Set(BYTE byPosition, XiahItem::sItemInfo* pHoldItem);
	bool ProcessItemUnSelected_Default_Smelt(BYTE byPosition, XiahItem::sItemInfo* pHoldItem);		// 조합
	bool ProcessItemUnSelected_Default_FE_Convert(BYTE byPosition, XiahItem::sItemInfo* pHoldItem);	// 오행 제련
	bool ProcessItemUnSelected_Default_QuickMart(BYTE byPosition, XiahItem::sItemInfo* pHoldItem);	// 매품패
	bool ProcessItemUnSelected_Default_Collection(BYTE byPosition, XiahItem::sItemInfo* pHoldItem);	// 아이템 수집

	BOOL ProcessItemUnSelected_Default_To_Itemmall(BYTE byPosition, XiahItem::sItemInfo* pHoldItem);
	BOOL ProcessItemUnSelected_Equip_Default( BYTE byPosition, XiahItem::sItemInfo* pHoldItem);
	BOOL ProcessItemUnSelected_Equip_Equip( BYTE byPosition, XiahItem::sItemInfo* pHoldItem);
	BOOL ProcessItemUnSelected_Shop_Default( BYTE byPosition, XiahItem::sItemInfo* pHoldItem);
	BOOL ProcessItemUnSelected_Shop_Shop( BYTE byPosition, XiahItem::sItemInfo* pHoldItem);
	BOOL ProcessItemUnSelected_TradeMine_Default( BYTE byPosition, XiahItem::sItemInfo* pHoldItem);
	BOOL ProcessItemUnSelected_Pet_Default( BYTE byPosition, XiahItem::sItemInfo* pHoldItem);
	BOOL ProcessItemUnSelected_Pet_Pet( BYTE byPosition, XiahItem::sItemInfo* pHoldItem);
	BOOL ProcessItemUnSelected_PetEquip_default( BYTE byPosition, XiahItem::sItemInfo* pHoldItem);
	BOOL ProcessItemUnSelected_Deposit_Default( BYTE byPosition, XiahItem::sItemInfo* pHoldItem);
	BOOL ProcessItemUnSelected_Deposit_Deposit( BYTE byPosition, XiahItem::sItemInfo* pHoldItem);
	bool ProcessItemUnSelected_Personal_TradeSell_Default(BYTE byPosition, XiahItem::sItemInfo* pHoldItem);
	bool ProcessItemUnSelected_Personal_Trade_Set_Default(BYTE byPosition, XiahItem::sItemInfo* pHoldItem);
	bool ProcessItemUnSelected_Personal_Trade_Set_Trade_Set(BYTE byPosition, XiahItem::sItemInfo* pHoldItem);	
	bool ProcessItemUnSelected_Collection_Default(BYTE byPosition, XiahItem::sItemInfo* pHoldItem);	// 아이템 수집

	// ITEMMALL -> DEFAULT SACK
	BOOL ProcessItemUnSelected_Itemmall_To_Default(BYTE byPosition, XiahItem::sItemInfo* pHoldItem);
	// ITEMMALL -> ITEMMALL
	BOOL ProcessItemUnSelected_Itemmall_To_Itemmall( BYTE byPosition, XiahItem::sItemInfo* pHoldItem);

	void DrawItem( int nPosition);
	void DrawItemToolTip( int nPosition);
	void SetItemToolTip( int nPosition);

	void DemandClass(XiahItem::sItemInfo* pItem);
	void ArrayText(sArrayData* pData, int nIndex, D3DCOLOR dwColor = D3DCOLOR_XRGB(255, 255, 255));
	void ToolTipArrayText(sArrayData* pData);

protected:

	BOOL					m_bShow;
	BOOL					m_bRefreshToolTip;
	BYTE					m_bAction;	// 1: Normal open	2: itemopen

	int						m_nCurToolTipItemPos;
	int						m_nPrevToolTipItemPos;

	BYTE					m_bySackTotalSize;		// sack에 들어가는 cell의 개수
	BYTE					m_bySackType;			// sack type
	sRect					m_SackRt;				// sack 영역

	ItemList				m_vecItem;				// 각 item info
	ItemVB					m_vecItemVB;			// 각 item vb
	ItemTex					m_vecItemTex;			// 각 item texture
	ItemRt					m_vecItemRt;			// 각 item 영역

	ItemVB					m_vecItemSocketVB;		// 각 아이템 소켓
	ItemVB					m_vecSocketItem1VB;		// 소켓 아이템
	ItemVB					m_vecSocketItem2VB;		// 소켓 아이템
	ItemVB					m_vecSocketItem3VB;		// 소켓 아이템
	ItemVB					m_vecRBSocketItemVB;	//HT_1116 : 각성자 아이템 추가 (소켓)
	ItemVB					m_vecRBItemStoneVB;		//HT_1116 : 각성자 아이템 추가 (연환석)

};

