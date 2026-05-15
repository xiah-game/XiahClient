#pragma once

#include "ArrayIndexData.h"

namespace XiahAniType
{

	//------------------------------------------------------------------------------
	//------------------------------------------------------------------------------
	// 논리적인 Animation Controller를 만들어 주겠다

	enum eXiahChar_MotionType
	{
		eLAT_Stand			= 0,	// 서 있는 동작
		eLAT_Idle			= 1,	// 정지해서 그냥 서 있는 동작
		eLAT_Walk			= 2,	// 걷기
		eLAT_Run			= 3,	// 뛰기
		eLAT_NormalAttack	= 4,	// 일반 공격동작
		eLAT_Die			= 5,	// 죽기 동작
		eLAT_Hit			= 6,	// 맞을때
		eLAT_Defend			= 7,	// 막을때
		eLAT_Special		= 8,	// 기타 특수 동작 (캐주얼은 아님)
		eLAT_Spawn			= 9,	// 등장할때
		eLAT_Casual			= 10,	// 케주얼
		eLAT_Mugong			= 11,	// 무공
		eLAT_Died			= 12,	// 죽었을때를 유지
		eLAT_Collect		= 13,	// 채집
		eLAT_Rebirth		= 14,	// 각성
		eLAT_MugongException= 15,	// 예외 무공들..
		eLAT_Count
	};

	typedef std::vector<int> ANITYPE_LIST;

	/**
	 * \ingroup XiahClient
	 *
	 * \date 2004-07-15
	 */
	class CXiahChar_LogicalAnimationType : public std::vector<ANITYPE_LIST *>
	{
	public:
		CXiahChar_LogicalAnimationType();
		virtual ~CXiahChar_LogicalAnimationType();

		virtual BOOL Create(CArrayIndexData *pData);
		virtual BOOL Release();
		// index가 -1이면 random하게 준다
		virtual int GetAniType(int nType,int index = -1);
	};

	extern BOOL InitializeLogicalAnimationType();
	extern BOOL ReleaseLogicalAnimationType();
	
	// Object가 NPC인 경우 NpcType도 넣어 주어야 함
	extern CXiahChar_LogicalAnimationType* GetAniType(BYTE nObjectType,BYTE nNpcType);

};