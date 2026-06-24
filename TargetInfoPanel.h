#pragma once
// Target Info Panel
// Shows target info (name, level, HP) at a fixed screen position

#include "XiahObject.h"
#include "XiahGameObject.h"

namespace TargetPanel
{
	const int PANEL_X		= 412;
	const int PANEL_Y		= 10;
	const int PANEL_WIDTH	= 200;
	const int PANEL_HEIGHT	= 56;
	const int PADDING		= 6;

	const int HP_BAR_WIDTH	= PANEL_WIDTH - PADDING * 2;
	const int HP_BAR_HEIGHT	= 8;
	const int HP_BAR_Y_OFFSET = 38;

	// Debuff icon row below HP bar
	const int ICON_SIZE		= 24;
	const int ICON_Y_OFFSET	= 52;
	const int ICON_SPACING	= 2;
	const int PANEL_HEIGHT_WITH_ICONS = 80;

	const D3DCOLOR BG_COLOR			= D3DCOLOR_ARGB(160, 10, 10, 10);
	const D3DCOLOR BORDER_COLOR		= D3DCOLOR_ARGB(200, 80, 80, 80);

	const D3DCOLOR HP_COLOR_NPC		= D3DCOLOR_XRGB(220, 50, 50);
	const D3DCOLOR HP_COLOR_PC		= D3DCOLOR_XRGB(100, 200, 100);
	const D3DCOLOR HP_COLOR_PET		= D3DCOLOR_XRGB(100, 150, 255);
	const D3DCOLOR HP_COLOR_BG		= D3DCOLOR_XRGB(30, 30, 30);

	const D3DCOLOR NAME_COLOR_NPC	= D3DCOLOR_XRGB(255, 180, 180);
	const D3DCOLOR NAME_COLOR_PC	= D3DCOLOR_XRGB(200, 255, 200);
	const D3DCOLOR NAME_COLOR_FNPC	= D3DCOLOR_XRGB(200, 200, 255);
	const D3DCOLOR LEVEL_COLOR		= D3DCOLOR_XRGB(255, 255, 200);
	const D3DCOLOR TYPE_COLOR		= D3DCOLOR_XRGB(180, 180, 180);
}

class CTargetInfoPanel
{
public:
	CTargetInfoPanel();
	~CTargetInfoPanel();

	void SetTarget(DWORD dwObjectID, BYTE bObjType);
	void Clear();
	void Update();
	void Render();
	BOOL IsActive() const { return m_bActive; }

private:
	void UpdateTargetUI192(CXiahCharObject* pTarget);
	void UpdateTargetHP(CXiahCharObject* pTarget);
	void UpdateTargetMP(CXiahCharObject* pTarget);
	void UpdateTargetDebuffs(CXiahCharObject* pTarget);
	void CheckAndAddNewDebuffs(CXiahCharObject* pTarget);
	void RenderActiveDebuffs(CXiahCharObject* pTarget);
	void RenderBackground();
	void RenderText();
	void RenderDebuffIcons();
	LPCTSTR GetTypeName(BYTE bObjType) const;
	D3DCOLOR GetHPColor(BYTE bObjType) const;
	D3DCOLOR GetNameColor(BYTE bObjType) const;

private:
	BOOL	m_bActive;
	DWORD	m_dwTargetID;
	BYTE	m_bTargetObjType;

	sString	m_szName;
	WORD	m_wLevel;
	DWORD	m_dwCurHP;
	DWORD	m_dwMaxHP;
	BYTE	m_bObjType;
	BYTE	m_bRebirth;

	CText2D	m_textName;
	CText2D	m_textLevel;
	CText2D m_textType;
};

extern CTargetInfoPanel* g_pTargetInfoPanel;
