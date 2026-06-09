#pragma once

#include "XiahGame_StepObject.h"
#include "CharacterInfo.h"
#include "XiahObject.h"

typedef std::list<XiahObject::CXiahObject *> VISIBLE_XIAHOBJECT_LIST;

typedef std::list<CText2D*> TEXT2DLIST;

#define ADJUST_SYNCMOVE_THRESOLD 5	// 5 Grid이상 차이 나면 보정

#define XIAH_STATIC_TRIGGER_COUNT 10

#define	XIAH_PORTAL_NPC_MAX		20	// 한 맵에 최대 포탈이 10개로 한정한다.

typedef std::list<XiahObject::CXiahObject*> sVisibleCharLIST;

class CXiahGame_Main : public CXiahGame_StepObject
{
public:
	// test code
	//int m_nTestCount;

	CXiahGame_Main();
	~CXiahGame_Main();

	BOOL Update();
	BOOL Render();

	BOOL UpdateObject();
	BOOL RenderCharObject(bool bIsAlphaTest);
	BOOL RenderCharObjectFromList(VISIBLE_XIAHOBJECT_LIST& VisibleXishObjectList);

	BOOL RenderCharName();

	BOOL MoveOnlyCameraForTest();

	BOOL ReleasePortalNPC();

public:
	// 디버깅용
#ifndef MASTER
	//CText2D g_tHelp[22];
	CText2D g_tHelp[8];
#endif

public:
	// 화면에 보이는 Object List
//	VISIBLE_XIAHOBJECT_LIST	m_VisibleXiahObjectList;
	VISIBLE_XIAHOBJECT_LIST	m_VisibleXiahObjectListNoAlpha;
	VISIBLE_XIAHOBJECT_LIST	m_VisibleXiahObjectListAlphaTest;

public:
	// Client Main Char 처리용


	// 포탈 Functional NPC. 맵 어디에서도 보여야 하므로, Client Character
	XiahObject::CXiahObject*	m_pPortalNPC[ XIAH_PORTAL_NPC_MAX ];
	int							m_nPortalNPCCount;

	// 화면에 보이는 캐릭터들의 이름
	TEXT2DLIST		m_VisibleXiahCharObjectNameList;
	sVisibleCharLIST m_VisibleXiahCharObjectList;
	sVisibleCharLIST m_PetObjectList;

public:
	// 이건 카메라를 직접 움직이는 부분으로 스크린샷 전용 기능.
	BOOL	m_bOnlyCameraMoveForTest;
	float	m_fOnlyCameraMoveY;

	// 매프레임마다 업데이트가 필요 없는 것들을 위한 타이머
	DWORD	m_Timer_UpdateMinimap;	// MINI MAP
	DWORD	m_Timer_UpdateSkyStar;	// Sky Star
};


extern BOOL CreateMainChar();

// statical한 트리거들
extern CStaticTrigger* g_StaticTriggerList[ XIAH_STATIC_TRIGGER_COUNT];
extern int OnCollided_MainChar(unsigned long);
extern int OnTimer_MainChar(unsigned long);
extern int OnTimer_Pet(unsigned long);
extern int OnEndTargetMove_MainChar(unsigned long);
extern int OnEndTargetMove_Pet(unsigned long);
extern int OnUpdateTargetDecal(unsigned long);

struct sMainChar_PreAttackInfo
{
	BYTE	bAttackType;
	DWORD	dwAttackID;
	BYTE	bDefType;
	DWORD	dwDefID;
	WORD	wAttackPosX;
	WORD	wAttackPosY;
	BYTE	bAttackHeight;
	BYTE	bAttackMode;
	DWORD	dwLastPreAttackTime;
	int		nRemainAttackCount;	// Attack_Ack를 보내야할 숫자
	BOOL	bPreAttackReq;		//HT_1026 : 스핵 방지
	BOOL	bAttackReq;
};

struct sMainChar_MugongPreAttackInfo
{
	DWORD dwMugongID;
	BYTE  bAttackType;
	DWORD dwAttackID;
	WORD  wAttackPosX;
	WORD  wAttackPosY;
	BYTE  bAttackHeight;
	BYTE  bDefendType;
	DWORD dwDefendID;
	WORD  wTargetPosX;
	WORD  wTargetPosY;
	BYTE  bTargetHeight;
	int		nRemainAttackCount;	// Attack_Ack를 보내야할 숫자
};

extern sMainChar_PreAttackInfo g_MainChar_PreAttackInfo;
extern sMainChar_MugongPreAttackInfo g_MainChar_MugongPreAttackInfo;
