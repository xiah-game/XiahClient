#include "precompile.h"
#include "PCVisualInfo.h"

BOOL ReleasePCVisualnfo(DWORD pInfo)
{
	sPCVisualInfo* pVisualInfo = (sPCVisualInfo*)pInfo;

	if(pVisualInfo)
	{
		delete pVisualInfo;
		pVisualInfo = NULL;
	}

	return TRUE;
}
