#include "FrameDefine.h"
#include "PopMenuDefine.h"
#include "PositionDefine.h"
#include "SoundDefine.h"

#ifdef _CHINA_
	#include "StringDefine_China.h"
#else
	#include "StringDefine.h"
#endif

extern CUIManager* g_pUIManager;

#define POTION_VISUALID_1	20000	//호혈단
#define POTION_VISUALID_2	21000	//금창약

#define TEXTEFFECT_COLOR_GENERAL	0
#define TEXTEFFECT_COLOR_GAIN		1
#define TEXTEFFECT_COLOR_WARNING	2