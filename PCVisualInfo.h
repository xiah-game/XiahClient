#pragma once

struct sPCVisualInfo
{
	WORD	wVisualID[ 9];
	BYTE	bRarity[9];
	BYTE	bStxType[9];
};

extern BOOL ReleasePCVisualnfo(DWORD pInfo);
