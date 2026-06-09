#pragma once

struct sFunctionalNpcInfo
{
	DWORD	m_dwObjectID;
	BYTE	m_bType;
	BYTE	m_bKind;
	BYTE	m_bCanShare;
	sString m_szName;
	BYTE	m_bOwnType;
	DWORD	m_dwOwnID;
	BYTE	m_bSizeX;
	BYTE	m_bSizeY;
	WORD	m_wNumItem;

	BYTE	m_bMainStone;
	BYTE	m_bWar;
	DWORD	m_dwEnemyMunpaID;

	sFunctionalNpcInfo() : m_bType(0), m_bMainStone(0), m_bKind(0), m_bWar(0), m_dwEnemyMunpaID(0)
	{

	}
	
};

BOOL ReleaseFunctionalNpcInfo(DWORD pInfo);