#pragma once

extern void ProcessXiahBGM(BOOL bForce = FALSE);
extern int XiahBGMTrigger_PlayEnd(unsigned param);
extern void InitXiahBGM();
extern void ReleaseXiahBGM();

#define MAX_FXTYPE		250
#define FX_NONE			0		// 지정안됨
#define FX_SOFT			1
#define FX_NORMAL		2
#define FX_HARD			3


extern int g_FXType[MAX_FXTYPE];
