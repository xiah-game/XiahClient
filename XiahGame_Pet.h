#pragma once

/*
	¾Ö¿Ïµ¿¹°
*/

#include "XiahGameObject.h"
#include "XiahObjectType.h"
#include "CharSack.h"
#include "EquipSack.h"

#include <assert.h>


// ¾Ö¿Ïµ¿¹° AI

#define	PETAI_NONE				0	// ¾Æ¹«°Íµµ ¾ø½¿ (PC¸¦ µû¶ó ´Ù´Ô)
#define	PETAI_AUTOATTACK		1	// ÀÚµ¿°ø°Ý
#define	PETAI_TARGETATTACK		2	// ´ë»ó°ø°Ý
#define	PETAI_TAKEITEM			3	// ¾ÆÀÌÅÛ ¼öÁý
#define	PETAI_SPECIALATTACK		4	// ¹«°ø°ø°Ý
#define PETAI_CALLTOME			5	// È£Ãâ

#define PET_SACK_COUNT			3	// ÆêÀÌ °¡Áú ¼ö ÀÖ´Â »öÀÇ °³¼ö

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
	DWORD	dwMoveTime;	// ¿òÁ÷ÀÌ±â ½ÃÀÛÇÑ½Ã°£ syncmove¿ë
	DWORD	dwLastAITime;
	DWORD	dwAIFrameTime;	// AI¸¦ ½ÃÇàÇÏ´Â ½Ã°¢ (1ÃÊ ÀÌ»óÀ¸·Î ÁÙ°Í)
	float	fFollowRange;	// ÁÖÀÎ°ú À¯ÁöÇÏ°í ½ÍÀº °Å¸® (ÀÌ °Å¸® ÀÌÇÏ¸é ÁÖÀÎ±ÙÃ³·Î ´Þ·Á°£´Ù)
	DWORD	dwStartIdleTime;	// ÇÒÀÏ ¾ø¾îÁö±â ½ÃÀÛÇÑ ½Ã°¢
	DWORD	dwIdleStepTime;
	BOOL	bIdle;
	int		nIdleStep;
	DWORD	dwIdleStepDelay;	// idle´ÙÀ½ µ¿ÀÛ±îÁöÀÇ ½Ã°£

	// 2004.07.07 Changth
	float	fAttackRange;	// ºÐ½Å°ÝÀÌ³ª È¯¼öÀ¯°¡ °ø°ÝÇÒ ¼ö ÀÖ´Â ¹üÀ§ fFollowRangeº¸´Ù Å©°Ô ÇÏ±â À§ÇØ
	BOOL	bHwanAttack;	// ºÐ½Å°ÝÀÌ³ª È¯¼öÀ¯°¡ °ø°ÝÀ» ÇÏ°íÀÖ³ª?

	
	// AI °ü·Ã
	BOOL	bAI;				// AI°¡ ÀÖ³ª?
	int		AI_Type;			// ÇöÀçÀÇ AI TypeÀº?
	BOOL	bSelected;			// ÇöÀç ¼±ÅÃÁßÀÎ°¡?
	BOOL	bFight;				// ÇöÀç AI¹ßµ¿ÇØ¼­ ½Î¿òÁßÀÎ°¡??
	DWORD	dwDestID;			// PETÀÌ °ø°ÝÇØ¾ßÇÒ ID
	DWORD	dwDestType;			// PETÀÌ °ø°ÝÇØ¾ßÇÒ OBJECT Type
	DWORD	dwGuardID;			// PETÀÌ GUARD ÇÒ ID
	DWORD	dwGuardType;		// PETÀÌ GUARD ÇÒ OBJECT Type
	BOOL	bFollowPC;			// PC¸¦ ¦i¾Æ°¡¾ßÇÔ

	DWORD	dwLastAttackTime;
	DWORD	dwAttackDelayTime;

	CSack*	m_pEquipSack;				// Á¹¶ó ¸¹ÀÌµµ °¡Áö°í ÀÖ³× ®X
	CSack*	m_pSack[PET_SACK_COUNT];	// ÆêÀÌ °¡Áö´Â »ö(3°³)
	BYTE	m_byMySackCurrIdx;			// ÇöÀç º¸¿©Áö´Â »ö

	// °ø°Ý °ü·Ã Á¤º¸ 
	BYTE	bAttackType;
	DWORD	dwAttackID;
	BYTE	bDefType;
	DWORD	dwDefID;
	WORD	wAttackPosX;
	WORD	wAttackPosY;
	BYTE	bAttackHeight;
	BYTE	bAttackMode;
	int		nRemainAttackCount;	// Attack_Ack¸¦ º¸³»¾ßÇÒ ¼ýÀÚ

	// 0:NONE, 1:È¯À¯¼ö, 2:ºÐ½Å°Ý
	DWORD	m_dwIsHwan;

	CClassTrigger<sPetInfo>* pEndTargetMove;
	CClassTrigger<sPetInfo>* pTimerTrigger;

	sPetInfo()
	{
		pEndTargetMove = new CClassTrigger<sPetInfo>(this, &sPetInfo::OnEndTargetMove, 0);
		pTimerTrigger  = new CClassTrigger<sPetInfo>(this, &sPetInfo::OnTimerTrigger, 0);

		if(pEndTargetMove == NULL || pTimerTrigger == NULL)
		{
			DBG_LogFile( _T("sPetInfo ½ÇÆÐ"));
			//return;
		}

		for( int i=0; i < PET_SACK_COUNT; ++i)
			m_pSack[i] = new CCharSack( SACKTYPE__PET, 6, 6);

		m_pEquipSack = new CEquipSack( SACKTYPE__PET_EQUIP, 6);

		assert(m_pEquipSack);

		m_byMySackCurrIdx = 0;
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
	BOOL	UpdatePet();	// Pet¿¡ AI·çÆ¾À» Å¸°Ô ¸¸µé¾î ÁØ´Ù
	BOOL	JumpToPlayer();

	BOOL	SelectPet(DWORD id);	// ÇØ´ç PetÀ» ¼±ÅÃ
	BOOL	DeSelectPet(DWORD id);	// ÇØ´ç PetÀ» ¼±ÅÃ Ç®±â

	BOOL	SelectAllPet();			// °¡Áö°í ÀÖ´Â ¸ðµç PETÀ» ¼±ÅÃ
	BOOL	DeSelectAllPet();		// °¡Áö°í ÀÖ´Â ¸ðµç PETÀ» ¼±ÅÃ Ç®±â

	// ¼±ÅÃµÇ¾îÁø PET¿¡°Ô AI/ÆÄ¶ó¸ÞÅÍ¸¦ Á¤ÇÏ¿© ÁØ´Ù
	BOOL	Change_PET_AI(int Type, DWORD di=NULL,DWORD gi=NULL,DWORD dt=NULL,DWORD gt=NULL);

	// ÇØ´ç PET¿¡°Ô AI/ÆÄ¶ó¸ÞÅÍ¸¦ Á¤ÇÏ¿© ÁØ´Ù
	BOOL	Change_PET_AI_Specify(DWORD PetID, int Type, DWORD di=NULL,DWORD gi=NULL,DWORD dt=NULL,DWORD gt=NULL);

	sPetInfo* GetPetInfo(DWORD id);
	sPetInfo* GetPetInfoByIndex( BYTE bIndex);
	sPetInfo* GetCurrentPet();
	sPetInfo* GetBunsinPet();	// ºÐ½Å°Ý PETÀ» ¾ò´Â´Ù

	void InitWild();
	void RenderWild(int nX, int nY, int nMark);
	void ReleaseWildMark();

	//HT_CHEAT : ÆêÀÇ Á¾·ù¸¦ ¾ò´Â´Ù.
	DWORD GetPetByHwan();

private:
	LPDIRECT3DVERTEXBUFFER9	m_pVB;
	LPDIRECT3DTEXTURE9		m_pTexture[3];
};

extern CPetList g_PetList;
extern BOOL		g_bCommandAI;
extern DWORD	g_dwCommandType;