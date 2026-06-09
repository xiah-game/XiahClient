#pragma once

#include "XiahSocket.h"

namespace XiahItem
{
	struct sHoldMoney
	{
		DWORD	m_dwAmount;
		BYTE	m_bySrcSackID;

		sHoldMoney()
		{
			m_dwAmount = 0;
			m_bySrcSackID = -1;
		}
	};


	// [12/9/2004] 초기에 아이템 구조를 통으로 잡았져 있었는데... 클래스로 나눠야 할것 같다.
	// 초기에 누가 이렇게 잡았지 -_-;
	struct sItemInfo
	{
		// 2004-02-18, 아이템 패킷 변경에 따른 데이타 추가.
		WORD	m_wRefID;
		WORD	m_wLevel;		// Item Level
		BYTE	m_bLimitCnt;
		WORD	m_wSoakHPRatio;
		WORD	m_wSoakAtkRatio;
		WORD	m_wSoakDefRatio;
		WORD	m_wSoakHitRatio;
		WORD	m_wTamingLevel;
		BYTE	m_bNeedMugongLevel;
		WORD	m_wHuljungModityCount;

		// Instance정보 
		DWORD	m_dwMapObjectID;
		DWORD	m_dwMapID;
		DWORD	m_dwItemID;
		DWORD	m_dwAmount;

		// Item Template
		WORD	m_wVisualID;
		BYTE	m_bItemType;
		BYTE	m_bItemKind;
		sString m_szName;

		BYTE	m_bEquipPos;
		BYTE	m_bConstraintCharType;

		// redmoon. 4.24
		BYTE	m_bNeedCharType;
		//BYTE	m_bProdType;
		DWORD	m_dwPrice;
		//sString	m_szCreator;
		WORD	m_wNeedDex;
		WORD	m_wNeedLevel;
		WORD	m_wNeedStr;
		WORD	m_wNeedSus;
		WORD	m_wNeedVit;
		// redmoon. 4.25
		BYTE 	m_bDecrDurRate;
		WORD 	m_wCurDur;
		WORD 	m_wMaxDur;
		DWORD 	m_wAtkPwr;
		DWORD 	m_wDefPwr;
		DWORD 	m_wAtkRating;
		WORD 	m_wStkSpeed;
		WORD 	m_wDecrSpeed;
		BYTE 	m_bPlusAtkType;
		WORD 	m_wPlusAtkPwr;
		WORD 	m_wAtkRange;

		// 안쓰는데
		//WORD 	m_wMeleeBlock;
		//WORD 	m_wShotBlock;
		//WORD 	m_wBoltBlock;
		//WORD	m_wFireBlock;
		//WORD 	m_wIceBlock;
		//WORD 	m_wPoisonBlock;

		DWORD 	m_wIncrHp;
		DWORD 	m_wIncrIp;
		WORD 	m_wRestoreHp;
		WORD 	m_wRestoreIp;
		BYTE 	m_bModifyCnt;
		
		// CG_2005/01/28 : 변종아이템기능추가
		// 변종아이템의 수리횟수 - 12번 수리하면 원래대로....
		BYTE	m_bRepairCnt;

		WORD 	m_wIncrCritical; //일격술 수정
		BYTE 	m_bStxType;
		BYTE	m_bRarity;
	
		//DWORD 	m_dwLockObject;
		//BYTE 	m_bLockObjectType;
		DWORD 	m_dwOwnerID;
		//BYTE 	m_bNumKey;
		DWORD 	m_dwMunpaID;
		//BYTE 	m_bPotionType;
		DWORD 	m_dwMugongID;
		BYTE 	m_bMugongType;
		BYTE 	m_bMugongKind;
		//BYTE 	m_bJewelryID;
		//DWORD 	m_dwAppearRatio;
		//BYTE 	m_bBunchAmount;
		// redmoon. 4.26
		BYTE	m_bSackID;
		BYTE	m_bSackCount;	// mysack에서 두번째 색 구현시 사용
		BYTE	m_bSackPos;
		BYTE	m_bSackIDPrev;
		BYTE	m_bSackPosPrev;	// mysack 과 modifysack 처리할때 사용		

		WORD	m_wSuccessRatio;
		WORD	m_wFactorValue;
		
		DWORD	m_dwPotalMapID;
		BYTE	m_bPortalType;

		DWORD	m_dwNpcID;
		BYTE	m_bNpcItemType;

		WORD	m_wTamingRate;
		BYTE	m_bWildRate;

		BYTE	m_bNpcRace;
		BYTE	m_bNpcBagSize;

		// Client용 VisualData
		int		m_nResID;
		BYTE	m_bSackSizeX;
		BYTE	m_bSackSizeY;

		int		m_nMapCharID;
		int		m_nMapMeshType;
		int		m_nMapTextureType;

		int		m_nEquipCharID;
		int		m_nEquipMeshType;
		int		m_nEquipTextureType;

		WORD	m_wPrev;
		bool	m_bShowMessage;
		
		DWORD	m_dwBuyCount;   // 구입 개수
		WORD	m_wFunctionItem; // 기능성 번호

		WORD	m_wPosX, m_wPosY;	// 동신적 좌표

		// [4/11/2004]
		BYTE m_bDanIncExp;
		BYTE m_bMopDecAtk;
		BYTE m_bMopDecDef;
		BYTE m_bMopDecAtkRatio;
		BYTE m_bMopDecHP;
		BYTE m_bShopDecTax;
		BYTE m_bGambleShopDec;
		BYTE m_bEffectType;

		// 2004_05_12 Changth : 전낭에 사용될 변수, 입금, 출금할때 액수.
		DWORD	m_dwValue;

		BYTE m_bPrizeRank;		// 등위
		BYTE m_bLottoNum[4];	// 선택번호
		DWORD m_dwRound;		// 회차		
		DWORD m_dwPrizeMoney;	// 당첨금액(천단위)

		BYTE m_bIsDividedRes;	// 개조가능 여부 (0-가능, 1-불가능)
		BYTE m_bPuzzleType;		// 조합된 아이템 타입

		// 제련
		BYTE m_bSocketCount;	// 소켓 수
		BYTE m_bSocketItem[3];	// 소켓 아이템 ()
		WORD m_wRBSocketItem;	// HT_1116 : 각성자 아이템 추가

		BYTE m_bFuncID;

		WORD m_wOriginRefID;
		WORD m_wUnionRefID;

		// 수리비 할인(백화수정)
		BYTE m_bRepairDiscount;

		//각성석
		BYTE m_bStepID;
		WORD m_wRebirthFuncID;
		WORD m_wRebirthNeedLevel;

		//HT_0829 : 프리미엄 퀘스트
		DWORD m_dwPremiumQuestID;

		//HT_1126 : 각성자 아이템 추가
		WORD  m_wRebuithValue;

		//HT_0406 : 환배 시스템 추가
		DWORD	m_dwKeepUpTime;
		BYTE	m_bMaxLevel;
		BYTE	m_bMinLevel;

		//HO_0828_07 황금열쇠 추가
		WORD	m_wKeyRefID;
		WORD	m_wKeyAmount;

		sItemInfo()
		{
			// 수리 아이콘
			m_wPrev = 0;
			m_bShowMessage = true;

			// Instance정보 
			m_dwMapObjectID = 0;
			m_dwMapID = 0;
			m_dwItemID = 0;
			m_dwAmount = 0;

			// Item Template
			m_wVisualID = 0;
			m_bItemType = 0;
			m_bItemKind = 0;
			m_szName = _T("");

			m_bEquipPos = 0;
			m_bConstraintCharType = 0;

			// redmoon. 4.24
			m_bNeedCharType = 0;
			//m_bProdType = 0;
			m_dwPrice = 0;
			//m_szCreator = _T("");
			m_wNeedDex = 0;
			m_wNeedLevel = 0;
			m_wNeedStr = 0;
			m_wNeedSus = 0;
			m_wNeedVit = 0;
			// redmoon. 4.25
			m_bDecrDurRate = 0;
			m_wCurDur = 0;
			m_wMaxDur = 0;
			m_wAtkPwr = 0;
			m_wDefPwr = 0;
			m_wAtkRating = 0;
			m_wStkSpeed = 0;
			m_wDecrSpeed = 0;
			m_bPlusAtkType = 0;
			m_wPlusAtkPwr = 0;
			m_wAtkRange = 0;
			//m_wMeleeBlock = 0;
			//m_wShotBlock = 0;
			//m_wBoltBlock = 0;
			//m_wFireBlock = 0;
			//m_wIceBlock = 0;
			//m_wPoisonBlock = 0;
			m_wIncrHp = 0;
			m_wIncrIp = 0;
			m_wRestoreHp = 0;
			m_wRestoreIp = 0;
			m_bModifyCnt = 0;
			// CG_2005/01/28 : 변종아이템기능추가
			m_bRepairCnt = 0;

			m_wIncrCritical = 0;
			m_bStxType = 0;
			m_bRarity = 0;
			//m_dwLockObject = 0;
			//m_bLockObjectType = 0;
			m_dwOwnerID = 0;
			//m_bNumKey = 0;
			m_dwMunpaID = 0;
			//m_bPotionType = 0;
			m_dwMugongID = 0;
			m_bMugongType = 0;
			m_bMugongKind = 0;
			//m_bJewelryID = 0;
			//m_dwAppearRatio = 0;
			//m_bBunchAmount = 0;

			// redmoon. 4.26
			m_bSackID = 0;
			m_bSackCount = 0;
			m_bSackPos = 0;
			m_bSackIDPrev = 0;
			m_bSackPosPrev = 0;

			m_wSuccessRatio=0;
			m_wFactorValue=0;

			m_dwPotalMapID = 0;
			m_bPortalType = 0;

			m_dwNpcID = 0;
			m_bNpcItemType = 0;

			m_bNpcRace = 0;
			m_bNpcBagSize = 0;

			// Client용 VisualData
			m_nResID = 0;
			m_bSackSizeX = 0;
			m_bSackSizeY = 0;

			m_nMapCharID = 0;
			m_nMapMeshType = 0;
			m_nMapTextureType = 0;

			m_nEquipCharID = 0;
			m_nEquipMeshType = 0;
			m_nEquipTextureType = 0;

			// [3/2/2004] 펫 흡수율 초기화
			m_wSoakHPRatio = m_wSoakAtkRatio = m_wSoakDefRatio = m_wSoakHitRatio = m_bWildRate = 0;
			m_wFunctionItem = -1;

			m_dwBuyCount = 0;

			m_wPosX = m_wPosY = 0;

			m_bDanIncExp = m_bMopDecAtk =m_bMopDecDef = m_bMopDecAtkRatio = m_bMopDecHP = m_bShopDecTax = m_bGambleShopDec = m_bEffectType = 0;

			m_dwValue = 0;

			m_bPrizeRank	=0;
			m_dwRound		=0;
			m_bLottoNum[4]	=0;
			m_dwPrizeMoney	=0;

			m_bIsDividedRes = m_bPuzzleType = 0;

			m_bSocketCount =0;
			ZeroMemory(m_bSocketItem, sizeof(BYTE)*3);

			m_bFuncID = 0;

			m_wOriginRefID = 0;
			m_wUnionRefID = 0;

			m_bRepairDiscount = 0;

			m_bStepID = 0;
			m_wRebirthFuncID = 0;
			m_wRebirthNeedLevel = 0;
			
			m_dwPremiumQuestID = 0;
			m_wRBSocketItem = 0;
			m_wRebuithValue =0;

			//HT_0406 : 환배 시스템 추가
			m_dwKeepUpTime = 0;
			m_bMaxLevel = 0;
			m_bMinLevel = 0;

			//HO_0828_07 황금열쇠 추가
			m_wKeyRefID = 0;
			m_wKeyAmount = 0;
		};
	};

	// info에 미리 세팅되어 있어야 할 값들
	// m_wVisualID	반다시!!!

	BOOL SetItemVisualData(sItemInfo* pInfo);
	BOOL ReleaseItemInfo(DWORD pInfo);
	void GetItemData( sItemInfo* pItem, CMsg& msg);	
};