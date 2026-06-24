// Target Info Panel implementation
// Renders target name, level, HP bar at fixed screen position
// Uses D3D direct rendering (RenderEnergyGauge + CText2D)

#include "precompile.h"
#include "TargetInfoPanel.h"
#include "XiahObjectType.h"
#include "CharacterInfo.h"
#include "StringDefine.h"
#include "frameDefine.h"
#include "NEWINTERFACE/CUIManager.h"
#include "AppData.h"

namespace XiahGameEngine
{
	extern XIAHGE_API DWORD g_dwCurTime;
}

CTargetInfoPanel* g_pTargetInfoPanel = NULL;

CTargetInfoPanel::CTargetInfoPanel()
	: m_bActive(FALSE)
	, m_dwTargetID(0)
	, m_bTargetObjType(0)
	, m_wLevel(0)
	, m_dwCurHP(0)
	, m_dwMaxHP(100)
	, m_bObjType(0)
	, m_bRebirth(0)
{
}

CTargetInfoPanel::~CTargetInfoPanel()
{
}

void CTargetInfoPanel::SetTarget(DWORD dwObjectID, BYTE bObjType)
{
	DBG_LogFile(_T("[TargetUI192] SetTarget dwObjectID=%u, bObjType=%d"), dwObjectID, (int)bObjType);

	if (bObjType == OBJTYPE_ITEM || bObjType == OBJTYPE_PET)
	{
		Clear();
		return;
	}

	m_dwTargetID = dwObjectID;
	m_bTargetObjType = bObjType;
	m_bActive = TRUE;
	Update();
}

void CTargetInfoPanel::Clear()
{
	m_bActive = FALSE;
	m_dwTargetID = 0;
	m_bTargetObjType = 0;
	m_szName = _T("");
	m_wLevel = 0;
	m_dwCurHP = 0;
	m_dwMaxHP = 100;
	m_bObjType = 0;
	m_bRebirth = 0;

	UpdateTargetUI192(NULL);
}

void CTargetInfoPanel::Update()
{
	if (!m_bActive || m_dwTargetID == 0)
		return;

	XiahObject::CXiahObject* pObject = XiahObject::g_XiahObjectManager.FindXiahObject(
		MAKEOBJECTID(0, m_dwTargetID, m_bTargetObjType));

	if (pObject == NULL || pObject->m_pObject == NULL)
	{
		DBG_LogFile(_T("[TargetUI192] FindXiahObject failed. dwTargetID=%u, bTargetObjType=%d, pObject=%p, m_pObject=%p"),
			m_dwTargetID, (int)m_bTargetObjType, pObject, pObject ? pObject->m_pObject : NULL);
		Clear();
		return;
	}

	XiahObject::CXiahObject_Basic* pBasic = pObject->m_pObject;

	m_szName	= pBasic->m_szObjectName;
	m_dwCurHP	= pBasic->m_dwCurHP;
	m_dwMaxHP	= pBasic->m_dwMaxHP;
	m_bObjType	= pBasic->m_bObjType;

	if (pBasic->IsA(XiahObject::eXOT_CharObject))
	{
		CXiahCharObject* pChar = reinterpret_cast<CXiahCharObject*>(pBasic);
		m_wLevel	= pChar->m_wLevel;
		m_bRebirth	= pChar->m_bRebirth;
	}
	else
	{
		m_wLevel	= 0;
		m_bRebirth	= 0;
	}
}

void CTargetInfoPanel::Render()
{
	if (!m_bActive)
	{
		UpdateTargetUI192(NULL);
		return;
	}

	Update();

	if (!m_bActive)
	{
		UpdateTargetUI192(NULL);
		return;
	}

	// 1. 获取目标对象并刷新 192 窗口数据
	XiahObject::CXiahObject* pObject = XiahObject::g_XiahObjectManager.FindXiahObject(
		MAKEOBJECTID(0, m_dwTargetID, m_bTargetObjType));
	if (pObject && pObject->m_pObject)
	{
		CXiahCharObject* pChar = reinterpret_cast<CXiahCharObject*>(pObject->m_pObject);
		UpdateTargetUI192(pChar);
	}
	else
	{
		UpdateTargetUI192(NULL);
	}
}

void CTargetInfoPanel::RenderBackground()
{
	using namespace TargetPanel;

	g_pDirect3DDevice->SetRenderState(D3DRS_FOGENABLE, FALSE);
	g_pDirect3DDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	g_pDirect3DDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	g_pDirect3DDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

	int nHeight = PANEL_HEIGHT;

	// Taller panel when target has active debuffs
	XiahObject::CXiahObject* pObj = NULL;
	if (m_bActive && m_dwTargetID != 0)
		pObj = XiahObject::g_XiahObjectManager.FindXiahObject(
			MAKEOBJECTID(0, m_dwTargetID, m_bTargetObjType));
	if (pObj && pObj->m_pObject)
	{
		CXiahCharObject* pChar = reinterpret_cast<CXiahCharObject*>(pObj->m_pObject);
		if (pChar->m_bObjType != OBJTYPE_PET)
		{
			pChar->RemoveExpiredDebuffs();
			if (!pChar->m_vDebuffList.empty())
				nHeight = PANEL_HEIGHT_WITH_ICONS;
		}
	}

	if (m_bObjType == OBJTYPE_PC || m_bObjType == OBJTYPE_FUNCTIONALNPC)
	{
		nHeight = 34;
	}

	VT_TLVertex vertices[4];

	float fLeft		= (float)PANEL_X;
	float fTop		= (float)PANEL_Y;
	float fRight	= (float)(PANEL_X + PANEL_WIDTH);
	float fBottom	= (float)(PANEL_Y + nHeight);

	vertices[0].pos.x = fLeft;		vertices[0].pos.y = fTop;
	vertices[1].pos.x = fRight;		vertices[1].pos.y = fTop;
	vertices[2].pos.x = fLeft;		vertices[2].pos.y = fBottom;
	vertices[3].pos.x = fRight;		vertices[3].pos.y = fBottom;

	for (int i = 0; i < 4; i++)
	{
		vertices[i].pos.z = 0.0f;
		vertices[i].pos.w = 1.0f;
		vertices[i].diffuse = BG_COLOR;
	}

	g_Device.SetTexture(0, NULL);
	g_Device.SetFVF(D3DFVF_TLVERTEX);
	g_pDirect3DDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, 2, vertices, sizeof(VT_TLVertex));

	VT_TLVertex border[5];
	border[0].pos.x = fLeft;		border[0].pos.y = fTop;
	border[1].pos.x = fRight;		border[1].pos.y = fTop;
	border[2].pos.x = fRight;		border[2].pos.y = fBottom;
	border[3].pos.x = fLeft;		border[3].pos.y = fBottom;
	border[4].pos.x = fLeft;		border[4].pos.y = fTop;

	for (int j = 0; j < 5; j++)
	{
		border[j].pos.z = 0.0f;
		border[j].pos.w = 1.0f;
		border[j].diffuse = BORDER_COLOR;
	}

	g_pDirect3DDevice->DrawPrimitiveUP(D3DPT_LINESTRIP, 4, border, sizeof(VT_TLVertex));

	g_pDirect3DDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
	g_pDirect3DDevice->SetRenderState(D3DRS_FOGENABLE, TRUE);
}

void CTargetInfoPanel::RenderText()
{
	using namespace TargetPanel;

	TCHAR strBuffer[256] = {0};

	int nTextX = PANEL_X + PADDING;
	int nTextY = PANEL_Y + PADDING;

	// Line 1: type label
	LPCTSTR lpType = GetTypeName(m_bObjType);
	m_textType.SetText(nTextX, nTextY, lpType, GetFont(IDS_GULIM, 11), TYPE_COLOR);
	m_textType.Render();

	// Line 2: name + level
	nTextY += 14;

	if (m_wLevel > 0)
	{
		if (m_bRebirth > 0)
		{
			_stprintf(strBuffer, _T("%s  Lv.%d (%dR)"), (LPCTSTR)m_szName, m_wLevel, m_bRebirth);
		}
		else
		{
			_stprintf(strBuffer, _T("%s  Lv.%d"), (LPCTSTR)m_szName, m_wLevel);
		}
	}
	else
	{
		_stprintf(strBuffer, _T("%s"), (LPCTSTR)m_szName);
	}

	m_textName.SetText(nTextX, nTextY, strBuffer, GetFont(IDS_GULIM, 12), GetNameColor(m_bObjType));
	m_textName.Render();

	// Line 3: HP percentage (only for targets with HP bar)
	if (m_bObjType == OBJTYPE_NPC || m_bObjType == OBJTYPE_PET)
	{
		nTextY = PANEL_Y + HP_BAR_Y_OFFSET + HP_BAR_HEIGHT + 2;

		int nPercent = 0;
		if (m_dwMaxHP > 0)
			nPercent = (int)((float)m_dwCurHP / (float)m_dwMaxHP * 100.0f);
		if (nPercent > 100) nPercent = 100;

		_stprintf(strBuffer, _T("HP %d%%"), nPercent);
		m_textLevel.SetText(nTextX, nTextY, strBuffer, GetFont(IDS_GULIM, 11), LEVEL_COLOR);
		m_textLevel.Render();
	}
}

LPCTSTR CTargetInfoPanel::GetTypeName(BYTE bObjType) const
{
	switch (bObjType)
	{
	case OBJTYPE_NPC:				return _T("[Monster]");
	case OBJTYPE_PC:				return _T("[Player]");
	case OBJTYPE_PET:				return _T("[Pet]");
	case OBJTYPE_FUNCTIONALNPC:		return _T("[NPC]");
	default:						return _T("[Target]");
	}
}

D3DCOLOR CTargetInfoPanel::GetHPColor(BYTE bObjType) const
{
	using namespace TargetPanel;

	switch (bObjType)
	{
	case OBJTYPE_NPC:	return HP_COLOR_NPC;
	case OBJTYPE_PC:	return HP_COLOR_PC;
	case OBJTYPE_PET:	return HP_COLOR_PET;
	default:			return HP_COLOR_NPC;
	}
}

D3DCOLOR CTargetInfoPanel::GetNameColor(BYTE bObjType) const
{
	using namespace TargetPanel;

	switch (bObjType)
	{
	case OBJTYPE_NPC:				return NAME_COLOR_NPC;
	case OBJTYPE_PC:				return NAME_COLOR_PC;
	case OBJTYPE_FUNCTIONALNPC:		return NAME_COLOR_FNPC;
	default:						return NAME_COLOR_NPC;
	}
}

void CTargetInfoPanel::RenderDebuffIcons()
{
	// 该功能已完全由窗口 192 数据驱动托管，退役手工渲染
}

void CTargetInfoPanel::UpdateTargetUI192(CXiahCharObject* pTarget)
{
	if (pTarget == NULL)
	{
		if (g_pUIManager->IsShow(WINDOW_TARGET_STATUS))
		{
			DBG_LogFile(_T("[TargetUI192] pTarget is NULL, Hide 192"));
			g_pUIManager->Hide(WINDOW_TARGET_STATUS);
		}
		return;
	}

	DBG_LogFile(_T("[TargetUI192] target=%s, level=%d, maxHP=%d, curHP=%d"), 
		(LPCTSTR)pTarget->m_szObjectName, pTarget->m_wLevel, pTarget->m_dwMaxHP, pTarget->m_dwCurHP);

	if (!g_pUIManager->IsShow(WINDOW_TARGET_STATUS))
	{
		RECT rcClient;
		GetClientRect(g_AppData.m_hWnd, &rcClient);

		int nWndWidth = 260;
		int nX = (rcClient.right - nWndWidth) / 2;
		int nY = 15;

		g_pUIManager->SetPosition(WINDOW_TARGET_STATUS, nX, nY);
		g_pUIManager->Show(WINDOW_TARGET_STATUS);
		DBG_LogFile(_T("[TargetUI192] Show WINDOW_TARGET_STATUS at x=%d, y=%d"), nX, nY);
	}

	g_pUIManager->SetString(WINDOW_TARGET_STATUS, target_status_name, pTarget->m_szObjectName);

	TCHAR szLevel[32];
	_stprintf(szLevel, _T("Lv.%d"), pTarget->m_wLevel);
	g_pUIManager->SetString(WINDOW_TARGET_STATUS, target_status_level, szLevel);

	UpdateTargetHP(pTarget);
	UpdateTargetMP(pTarget);
	UpdateTargetDebuffs(pTarget);
}

void CTargetInfoPanel::UpdateTargetDebuffs(CXiahCharObject* pTarget)
{
	if (pTarget == NULL || pTarget->m_bObjType == OBJTYPE_PET) return;

	// 1. 每帧主动调用目标的 Debuff 清理，让过期的 Debuff 消失并同步清除 m_KeepUpMugongList 里的残留
	pTarget->RemoveExpiredDebuffs();

	// 2. 检测 m_KeepUpMugongList 中的新 Debuff 并录入 m_vDebuffList
	CheckAndAddNewDebuffs(pTarget);

	// 3. 渲染当前生效的 Debuff 图标并处理倒计时闪烁
	RenderActiveDebuffs(pTarget);
}

void CTargetInfoPanel::CheckAndAddNewDebuffs(CXiahCharObject* pTarget)
{
	if (pTarget == NULL) return;

	for (auto iter = pTarget->m_KeepUpMugongList.begin(); iter != pTarget->m_KeepUpMugongList.end(); ++iter)
	{
		DWORD dwMugongID = iter->first;
		sKeepUpMugong& mugong = iter->second;

		// 检查 m_vDebuffList 中是否已存在此 Debuff
		bool bExists = false;
		for (size_t j = 0; j < pTarget->m_vDebuffList.size(); j++)
		{
			if (pTarget->m_vDebuffList[j].wMugongID == (WORD)dwMugongID)
			{
				bExists = true;
				break;
			}
		}

		if (!bExists)
		{
			// 从武功列表模板 pc_mugong_list.idx 中获取持续时间 (第 31 号字段，单位：秒)
			DWORD dwDurationMs = 8000; // 默认值 8 秒
			sArrayData* pMugongList = XiahArrayIndex::g_MugongList.GetData(dwMugongID, mugong.bLevel);
			if (pMugongList != NULL)
			{
				int nSec = pMugongList->GetInt(31);
				if (nSec > 0)
				{
					dwDurationMs = (DWORD)nSec * 1000;
				}
			}

			pTarget->AddDebuff((WORD)dwMugongID, dwDurationMs);
			DBG_LogFile(_T("[TargetUI192] AddDebuff to target: dwMugongID=%u, level=%d, duration=%u"), 
				dwMugongID, (int)mugong.bLevel, dwDurationMs);
		}
	}
}

void CTargetInfoPanel::RenderActiveDebuffs(CXiahCharObject* pTarget)
{
	if (pTarget == NULL) return;

	struct sRenderDebuff
	{
		int nResID;
		DWORD dwExpireTime;
	};
	std::vector<sRenderDebuff> vRenderDebuffs;

	// 收集有效的 Debuff 贴图以及对应的到期时间戳
	for (size_t i = 0; i < pTarget->m_vDebuffList.size(); i++)
	{
		WORD wMugongID = pTarget->m_vDebuffList[i].wMugongID;
		sArrayData* pTemplate = XiahArrayIndex::g_MugongTemplate.GetData(wMugongID);
		if (pTemplate != NULL)
		{
			int nResID = pTemplate->GetInt(1);
			if (nResID > 0)
			{
				sRenderDebuff rd;
				rd.nResID = nResID;
				rd.dwExpireTime = pTarget->m_vDebuffList[i].dwExpireTime;
				vRenderDebuffs.push_back(rd);
			}
		}
	}

	int nCount = (int)vRenderDebuffs.size();

	// 渲染前 5 个插槽，并处理 10 秒内闪烁
	for (int i = 0; i < 5; i++)
	{
		int nCtrlID = target_status_debuff_1 + i;
		if (i < nCount)
		{
			sRenderDebuff& rd = vRenderDebuffs[i];
			bool bShowIcon = true;

			if (rd.dwExpireTime > XiahGameEngine::g_dwCurTime)
			{
				DWORD dwRemainMs = rd.dwExpireTime - XiahGameEngine::g_dwCurTime;
				if (dwRemainMs <= 10000)
				{
					// 10秒内，周期 0.6 秒闪烁 (300ms 亮, 300ms 暗)
					bShowIcon = ((XiahGameEngine::g_dwCurTime % 600) < 300);
				}
			}
			else
			{
				bShowIcon = false; // 已过期
			}

			if (bShowIcon)
			{
				g_pUIManager->SetData(WINDOW_TARGET_STATUS, nCtrlID, TYPE, STATIC);
				g_pUIManager->SetData(WINDOW_TARGET_STATUS, nCtrlID, TEXTURE, rd.nResID);
				g_pUIManager->Show(WINDOW_TARGET_STATUS, nCtrlID);
			}
			else
			{
				g_pUIManager->Hide(WINDOW_TARGET_STATUS, nCtrlID);
			}
		}
		else
		{
			g_pUIManager->SetData(WINDOW_TARGET_STATUS, nCtrlID, TEXTURE, 0);
			g_pUIManager->Hide(WINDOW_TARGET_STATUS, nCtrlID);
		}
	}
}

void CTargetInfoPanel::UpdateTargetHP(CXiahCharObject* pTarget)
{
	if (pTarget == NULL) return;

	int nHpPer = 0;
	if (pTarget->m_dwMaxHP > 0)
	{
		nHpPer = (int)((float)pTarget->m_dwCurHP / (float)pTarget->m_dwMaxHP * 100.0f);
	}
	if (nHpPer > 100) nHpPer = 100;
	if (nHpPer < 0)   nHpPer = 0;
	g_pUIManager->SetData(WINDOW_TARGET_STATUS, target_status_hp_gauge, VALUE1, nHpPer);

	TCHAR szHpText[64];
	_stprintf(szHpText, _T("%d/%d"), pTarget->m_dwCurHP, pTarget->m_dwMaxHP);
	g_pUIManager->SetString(WINDOW_TARGET_STATUS, target_status_hp_text, szHpText);
	DBG_LogFile(_T("[TargetUI192] HP set: per=%d, text=%s"), nHpPer, szHpText);
}

void CTargetInfoPanel::UpdateTargetMP(CXiahCharObject* pTarget)
{
	if (pTarget == NULL || pTarget->m_bObjType == OBJTYPE_PET) return;

	int nIpCur = 0;
	int nIpMax = 0;

	extern CharacterInfo g_MainCharInfo;
	extern XiahObject::CXiahObject* g_pMainChar;
	if (g_pMainChar && pTarget == (CXiahCharObject*)g_pMainChar->m_pObject)
	{
		nIpCur = g_MainCharInfo.m_wIpCur;
		nIpMax = g_MainCharInfo.m_wIpMax;
	}

	int nMpPer = 0;
	if (nIpMax > 0)
	{
		nMpPer = (int)((float)nIpCur / (float)nIpMax * 100.0f);
	}
	if (nMpPer > 100) nMpPer = 100;
	if (nMpPer < 0)   nMpPer = 0;
	g_pUIManager->SetData(WINDOW_TARGET_STATUS, target_status_mp_gauge, VALUE1, nMpPer);

	TCHAR szMpText[64];
	_stprintf(szMpText, _T("%d/%d"), nIpCur, nIpMax);
	g_pUIManager->SetString(WINDOW_TARGET_STATUS, target_status_mp_text, szMpText);
	DBG_LogFile(_T("[TargetUI192] MP set: per=%d, text=%s"), nMpPer, szMpText);
}
