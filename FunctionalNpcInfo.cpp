#include "precompile.h"
#include "functionalnpcinfo.h"

BOOL ReleaseFunctionalNpcInfo(DWORD pInfo)
{
	sFunctionalNpcInfo *pFunctionalNpcInfo = (sFunctionalNpcInfo *)pInfo;

	if(pFunctionalNpcInfo)
	{
		delete pFunctionalNpcInfo;
		pFunctionalNpcInfo = NULL;
	}

	return TRUE;
}