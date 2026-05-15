#pragma once

// Windows Default Header
//#define _WIN32_WINDOWS 0x0410

// 이렇게 해야 SetWindowsHookEx을 사용할 수 있다.
#define _WIN32_WINNT 0x0500

#include <windows.h>
#include <tchar.h>

#include <d3d9.h>
#include <d3dx9.h>
#include <mmsystem.h>
#include <dsound.h>

//#include "../ThothCore/Memory.h"
//
//#pragma comment(lib, "ThothCore.lib")


#ifdef _DEBUG
	#pragma comment(lib, "XiahGameEngined.lib")
#else
	#pragma comment(lib, "XiahGameEngine.lib")
#endif

#include "XiahGameEngine.h"
#include "csprotocol.h"	// 이건 너무 떡대쟁이라
// �
#define SAFE_DELETE_ARRAY( array) \
						if( array != NULL)\
						{\
							delete [] array;\
							array = NULL;\
						}

#define SAFE_DELETE( value) \
						if( value != NULL)\
						{\
							delete value;\
							value = NULL;\
						}

using namespace XiahGameEngine;

#define TEST_PERFORMANCE	0

#ifdef MINI
#define G_WIDTH		640
#define G_HEIGHT	480
#endif

// GAME GROBAL INFOMATION
extern sXiahGameEngine_CreateInfo g_info;
extern sXiahGameEngine_CreateInfo g_info_Temp;
extern void Save_Option(bool bSend);	// REGISTRY에 OPTION 값 저장


