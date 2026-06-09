#pragma once

#include "CharacterInfo.h"

//#define LIGHTSET

/*
	물론 MainWindow에 다 집어 넣어도 되는거지만, MainWindow프로그램이 너무 복잡해지는 걸 방지 하기 위해


*/
#define COLOR_PICKCURSOR D3DCOLOR_XRGB( 255, 255, 128)

extern BOOL InitXiahGame();		// 게임 초기화
extern BOOL CloseXiahGame();	// 게임 종료 (서버 연결 끊기거나, Map에서 MainInterface로 나올때는 아님!!!)
extern BOOL LoopXiahGame_BeforeRender();
extern BOOL LoopXiahGame();		// 게임 Main Loop

extern BOOL LoopXiahGamePreShadow();	// Pre Shadow Making
extern BOOL ManageExtraEffect();
extern BOOL LoopXiahGameFX();	// GAME SOUND FX PLAY

extern void OnConnectedToServer();
extern sString MoneyCommaStr(INT64 nMoney);
extern LPCTSTR GetMapName(DWORD dwMapID);
extern LPCTSTR GetMapSmallName(DWORD dwMapID);

extern BOOL	g_XiahGameStarted;	// 본 게임이 시작 되었나?
extern float g_fix;				// 경사오르기 파라메터
extern long g_quickslot;		// PAD때문에 넣은 quick slot 번호

extern XiahGameEngine::Map::CMapDecal	g_PickCursor;
extern DWORD   dwSelObjectID;
extern DWORD   dwSelObjectType;

extern CharacterInfo g_MainCharInfo;
extern CUIManager* g_pUIManager;

extern int light_mode;
extern unsigned char lightR;
extern unsigned char lightG;
extern unsigned char lightB;

extern unsigned char flightR;
extern unsigned char flightG;
extern unsigned char flightB;

extern unsigned char skyR1;
extern unsigned char skyG1;
extern unsigned char skyB1;

extern unsigned char skyR2;
extern unsigned char skyG2;
extern unsigned char skyB2;

extern unsigned char skyR3;
extern unsigned char skyG3;
extern unsigned char skyB3;

extern sString	g_ServerName;
extern bool g_IsFocus;

