#pragma once

namespace Fade
{
	typedef BOOL (*FADE_TRIGGER)(DWORD TriggerIndex);
	extern BOOL		g_bFadeStart;

	extern BOOL StartFade(D3DCOLOR color,BOOL bFadeIn,FADE_TRIGGER pTrigger,DWORD time = 300,BOOL bNotRender=FALSE);
	extern BOOL UpdateFade();
	extern BOOL RenderFade();
	extern BOOL SetFade(D3DCOLOR color);

	extern BOOL CreateFade();
	extern BOOL DestroyFade();
};
