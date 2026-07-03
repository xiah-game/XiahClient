#pragma once

/*
	애완동물
*/

#include "XiahGameObject.h"
#include "XiahObjectType.h"
#include "CharSack.h"
#include "EquipSack.h"

#include <assert.h>


// 애완동물 AI

#define	PETAI_NONE				0	// 아무것도 없슴 (PC를 따라 다님)
#define	PETAI_AUTOATTACK		1	// 자동공격
#define	PETAI_TARGETATTACK		2	// 대상공격
#define	PETAI_TAKEITEM			3	// 아이템 수집
#define	PETAI_SPECIALATTACK		4	// 무공공격
#define PETAI_CALLTOME			5	// 호출

#define PET_SACK_COUNT			3	// 펫이 가질 수 있는 색의 개수

struct sPetRevival
{
	DWORD dwID;
	sString strName;
};

struct sPetInfo
{
	DWORD dwID;
	DWORD dwOwnID;
	DWORD dwMapID;
	BYTE  bNpcType;
	sString szName;
	WORD  wLevel;
	WORD  wPosX;
	WORD  wPosY;
	BYTE  bHeight;
	WORD  wDesPosX;
	WORD  wDesPosY;
	BYTE  bDesHeight;
	WORD  wDirection;
	DWORD  dwHpMax;
	DWORD  dwHpCur;
	WORD  wAtkPwr;
	WORD  wDefPwr;
	WORD  wAtkRating;
	WORD  wAvoidRatio;
	BYTE  bSpeed;
	WORD  wMeleeAtkRange;
	WORD  wShotAtkRange;
	BYTE  bAtkType;
	DWORD dwRefNpcID;
	BYTE  bCurJob;
	INT64 i64Exp;
	INT64 i64LevelExp;
	INT64 i64NextLevelUpExp;
	BYTE  bRevolutionStep;
	BYTE  bWildRate;

	//
	DWORD	dwMoveTime;	// 움직이기 시작한시간 syncmove용
	DWORD	dwLastAITime;
	DWORD	dwAIFrameTime;	// AI를 시행하는 시각 (1초 이상으로 줄것)
	float	fFollowRange;	// 주인과 유지하고 싶은 거리 (이 거리 이하면 주인근처로 달려간다)
	DWORD	dwStartIdleTime;	// 할일 없어지기 시작한 시각
	DWORD	dwIdleStepTime;
	BOOL	bIdle;
	int		nIdleStep;
	DWORD	dwIdleStepDelay;	// idle다음 동작까지의 시간

	// 2004.07.07 Changth
	float	fAttackRange;	// 분신격이나 환수유가 공격할 수 있는 범위 fFollowRange보다 크게 하기 위해
	BOOL	bHwanAttack;	// 분신격이나 환수유가 공격을 하고있나?

	
	// AI 관련
	BOOL	bAI;				// AI가 있나?
	int		AI_Type;			// 현재의 AI Type은?
	BOOL	bSelected;			// 현재 선택중인가?
	BOOL	bFight;				// 현재 AI발동해서 싸움중인가??
	DWORD	dwDestID;			// PET이 공격해야할 ID
	DWORD	dwDestType;			// PET이 공격해야할 OBJECT Type
	DWORD	dwGuardID;			// PET이 GUARD 할 ID
	DWORD	dwGuardType;		// PET이 GUARD 할 OBJECT Type
	BOOL	bFollowPC;			// PC를 쫒아가야함

	DWORD	dwLastAttackTime;
	DWORD	dwAttackDelayTime;

	CSack*	m_pEquipSack;				// 졸라 많이도 가지고 있네 췟
	CSack*	m_pSack[PET_SACK_COUNT];	// 펫이 가지는 색(3개)
	BYTE	m_byMySackCurrIdx;			// 현재 보여지는 색

	// 공격 관련 정보 
	BYTE	bAttackType;
	DWORD	dwAttackID;
	BYTE	bDefType;
	DWORD	dwDefID;
	WORD	wAttackPosX;
	WORD	wAttackPosY;
	BYTE	bAttackHeight;
	BYTE	bAttackMode;
	int		nRemainAttackCount;	// Attack_Ack를 보내야할 숫자

	// 0:NONE, 1:환유수, 2:분신격
	DWORD	m_dwIsHwan;

	CClassTrigger<sPetInfo>* pEndTargetMove;
	CClassTrigger<sPetInfo>* pTimerTrigger;

	sPetInfo()
	{
		pEndTargetMove = new CClassTrigger<sPetInfo>(this, &sPetInfo::OnEndTargetMove, 0);
		pTimerTrigger  = new CClassTrigger<sPetInfo>(this, &sPetInfo::OnTimerTrigger, 0);

		if(pEndTargetMove == NULL || pTimerTrigger == NULL)
		{
			DBG_LogFile( _T("sPetInfo 실패"));
			//return;
		}

		for( int i=0; i < PET_SACK_COUNT; ++i)
			m_pSack[i] = new CCharSack( SACKTYPE__PET, 6, 6);

		m_pEquipSack = new CEquipSack( SACKTYPE__PET_EQUIP, 6);

		assert(m_pEquipSack);

		m_byMySackCurrIdx = 0;
	}

	sPetInfo(const sPetInfo& other)
	{
		dwID = other.dwID;
		dwOwnID = other.dwOwnID;
		dwMapID = other.dwMapID;
		bNpcType = other.bNpcType;
		szName = other.szName;
		wLevel = other.wLevel;
		wPosX = other.wPosX;
		wPosY = other.wPosY;
		bHeight = other.bHeight;
		wDesPosX = other.wDesPosX;
		wDesPosY = other.wDesPosY;
		bDesHeight = other.bDesHeight;
		wDirection = other.wDirection;
		dwHpMax = other.dwHpMax;
		dwHpCur = other.dwHpCur;
		wAtkPwr = other.wAtkPwr;
		wDefPwr = other.wDefPwr;
		wAtkRating = other.wAtkRating;
		wAvoidRatio = other.wAvoidRatio;
		bSpeed = other.bSpeed;
		wMeleeAtkRange = other.wMeleeAtkRange;
		wShotAtkRange = other.wShotAtkRange;
		bAtkType = other.bAtkType;
		dwRefNpcID = other.dwRefNpcID;
		bCurJob = other.bCurJob;
		i64Exp = other.i64Exp;
		i64LevelExp = other.i64LevelExp;
		i64NextLevelUpExp = other.i64NextLevelUpExp;
		bRevolutionStep = other.bRevolutionStep;
		bWildRate = other.bWildRate;

		dwMoveTime = other.dwMoveTime;
		dwLastAITime = other.dwLastAITime;
		dwAIFrameTime = other.dwAIFrameTime;
		fFollowRange = other.fFollowRange;
		dwStartIdleTime = other.dwStartIdleTime;
		dwIdleStepTime = other.dwIdleStepTime;
		bIdle = other.bIdle;
		nIdleStep = other.nIdleStep;
		dwIdleStepDelay = other.dwIdleStepDelay;
		fAttackRange = other.fAttackRange;
		bHwanAttack = other.bHwanAttack;
		bAI = other.bAI;
		AI_Type = other.AI_Type;
		bSelected = other.bSelected;
		bFight = other.bFight;
		dwDestID = other.dwDestID;
		dwDestType = other.dwDestType;
		dwGuardID = other.dwGuardID;
		dwGuardType = other.dwGuardType;
		bFollowPC = other.bFollowPC;
		dwLastAttackTime = other.dwLastAttackTime;
		dwAttackDelayTime = other.dwAttackDelayTime;
		m_byMySackCurrIdx = other.m_byMySackCurrIdx;
		bAttackType = other.bAttackType;
		dwAttackID = other.dwAttackID;
		bDefType = other.bDefType;
		dwDefID = other.dwDefID;
		wAttackPosX = other.wAttackPosX;
		wAttackPosY = other.wAttackPosY;
		bAttackHeight = other.bAttackHeight;
		bAttackMode = other.bAttackMode;
		nRemainAttackCount = other.nRemainAttackCount;
		m_dwIsHwan = other.m_dwIsHwan;

		pEndTargetMove = new CClassTrigger<sPetInfo>(this, &sPetInfo::OnEndTargetMove, 0);
		pTimerTrigger  = new CClassTrigger<sPetInfo>(this, &sPetInfo::OnTimerTrigger, 0);
		for( int i=0; i < PET_SACK_COUNT; ++i)
			m_pSack[i] = new CCharSack( SACKTYPE__PET, 6, 6);
		m_pEquipSack = new CEquipSack( SACKTYPE__PET_EQUIP, 6);
	}

	sPetInfo& operator=(const sPetInfo& other)
	{
		if (this == &other) return *this;

		dwID = other.dwID;
		dwOwnID = other.dwOwnID;
		dwMapID = other.dwMapID;
		bNpcType = other.bNpcType;
		szName = other.szName;
		wLevel = other.wLevel;
		wPosX = other.wPosX;
		wPosY = other.wPosY;
		bHeight = other.bHeight;
		wDesPosX = other.wDesPosX;
		wDesPosY = other.wDesPosY;
		bDesHeight = other.bDesHeight;
		wDirection = other.wDirection;
		dwHpMax = other.dwHpMax;
		dwHpCur = other.dwHpCur;
		wAtkPwr = other.wAtkPwr;
		wDefPwr = other.wDefPwr;
		wAtkRating = other.wAtkRating;
		wAvoidRatio = other.wAvoidRatio;
		bSpeed = other.bSpeed;
		wMeleeAtkRange = other.wMeleeAtkRange;
		wShotAtkRange = other.wShotAtkRange;
		bAtkType = other.bAtkType;
		dwRefNpcID = other.dwRefNpcID;
		bCurJob = other.bCurJob;
		i64Exp = other.i64Exp;
		i64LevelExp = other.i64LevelExp;
		i64NextLevelUpExp = other.i64NextLevelUpExp;
		bRevolutionStep = other.bRevolutionStep;
		bWildRate = other.bWildRate;

		dwMoveTime = other.dwMoveTime;
		dwLastAITime = other.dwLastAITime;
		dwAIFrameTime = other.dwAIFrameTime;
		fFollowRange = other.fFollowRange;
		dwStartIdleTime = other.dwStartIdleTime;
		dwIdleStepTime = other.dwIdleStepTime;
		bIdle = other.bIdle;
		nIdleStep = other.nIdleStep;
		dwIdleStepDelay = other.dwIdleStepDelay;
		fAttackRange = other.fAttackRange;
		bHwanAttack = other.bHwanAttack;
		bAI = other.bAI;
		AI_Type = other.AI_Type;
		bSelected = other.bSelected;
		bFight = other.bFight;
		dwDestID = other.dwDestID;
		dwDestType = other.dwDestType;
		dwGuardID = other.dwGuardID;
		dwGuardType = other.dwGuardType;
		bFollowPC = other.bFollowPC;
		dwLastAttackTime = other.dwLastAttackTime;
		dwAttackDelayTime = other.dwAttackDelayTime;
		m_byMySackCurrIdx = other.m_byMySackCurrIdx;
		bAttackType = other.bAttackType;
		dwAttackID = other.dwAttackID;
		bDefType = other.bDefType;
		dwDefID = other.dwDefID;
		wAttackPosX = other.wAttackPosX;
		wAttackPosY = other.wAttackPosY;
		bAttackHeight = other.bAttackHeight;
		bAttackMode = other.bAttackMode;
		nRemainAttackCount = other.nRemainAttackCount;
		m_dwIsHwan = other.m_dwIsHwan;

		return *this;
	}

	~sPetInfo()
	{
		if(pEndTargetMove)
			delete pEndTargetMove, pEndTargetMove = NULL;
		if(pTimerTrigger)
			delete pTimerTrigger, pTimerTrigger = NULL;

		for( int i=0; i<PET_SACK_COUNT; i++)
		{
			if( m_pSack[i])
			{
				delete m_pSack[i];
				m_pSack[i] = NULL;
			}
		}

		if( m_pEquipSack)
		{
			delete m_pEquipSack;
			m_pEquipSack = NULL;
		}
	}

	int OnEndTargetMove(unsigned long param);
	int OnTimerTrigger(unsigned long param);
};

extern BOOL ReleasePetInfo(DWORD pInfo);

class CPetList : public std::map<DWORD,XiahObject::CXiahObject*>
{
public:
	std::map<BYTE, sPetRevival*>	m_mPetRevivalList;

	CPetList();
	virtual ~CPetList();

	void Release();	
	
	XiahObject::CXiahObject* Find(DWORD id);
	CXiahCharObject*		 FindChar(DWORD id);

	BOOL	AddPet(XiahObject::CXiahObject* pPet);
	BOOL	DeletePet(XiahObject::CXiahObject* pPet);
	BOOL	DeletePet(DWORD id);
	BOOL	UpdatePet();	// Pet에 AI루틴을 타게 만들어 준다
	BOOL	JumpToPlayer();

	BOOL	SelectPet(DWORD id);	// 해당 Pet을 선택
	BOOL	DeSelectPet(DWORD id);	// 해당 Pet을 선택 풀기

	BOOL	SelectAllPet();			// 가지고 있는 모든 PET을 선택
	BOOL	DeSelectAllPet();		// 가지고 있는 모든 PET을 선택 풀기

	// 선택되어진 PET에게 AI/파라메터를 정하여 준다
	BOOL	Change_PET_AI(int Type, DWORD di=NULL,DWORD gi=NULL,DWORD dt=NULL,DWORD gt=NULL);

	// 해당 PET에게 AI/파라메터를 정하여 준다
	BOOL	Change_PET_AI_Specify(DWORD PetID, int Type, DWORD di=NULL,DWORD gi=NULL,DWORD dt=NULL,DWORD gt=NULL);

	sPetInfo* GetPetInfo(DWORD id);
	sPetInfo* GetPetInfoByIndex( BYTE bIndex);
	sPetInfo* GetCurrentPet();
	sPetInfo* GetBunsinPet();	// 분신격 PET을 얻는다

	void InitWild();
	void RenderWild(int nX, int nY, int nMark);
	void ReleaseWildMark();

	//HT_CHEAT : 펫의 종류를 얻는다.
	DWORD GetPetByHwan();

private:
	LPDIRECT3DVERTEXBUFFER9	m_pVB;
	LPDIRECT3DTEXTURE9		m_pTexture[3];
};

extern CPetList g_PetList;
extern BOOL		g_bCommandAI;
extern DWORD	g_dwCommandType;