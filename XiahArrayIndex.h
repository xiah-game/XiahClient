#pragma once

#include "ArrayIndexData.h"

namespace XiahArrayIndex
{
	#define	XIAH_ANITYPE_COUNT	3


	extern CArrayIndexData g_MapFileName;
	extern CArrayIndexData g_MainCharType;
	extern CArrayIndexData g_NpcType;
	// 성인서버용 NPC
	extern CArrayIndexData g_NpcType2;
	extern CArrayIndexData g_MugongTemplate;
	extern CArrayIndexData g_MugongList;
	extern CArrayIndexData g_MugongColumnName;
	extern CArrayIndexData g_MugongIndex_InGum;
	extern CArrayIndexData g_MugongIndex_InYun;
	extern CArrayIndexData g_MugongIndex_InMutu;
	extern CArrayIndexData g_MugongIndex_InYaCha;
	extern CArrayIndexData g_MugongIndex_OutGum;
	extern CArrayIndexData g_MugongIndex_OutYun;
	extern CArrayIndexData g_MugongIndex_OutMutu;
	extern CArrayIndexData g_MugongIndex_OutYaCha;
	extern CArrayIndexData g_QuestDesc;
	extern CArrayIndexData g_ItemType;
	extern CArrayIndexData g_FunctionalNpcType;
	extern CArrayIndexData g_ItemLength;
	extern CArrayIndexData g_ItemTip;

	extern CArrayIndexData g_MugongDesc;
	extern CArrayIndexData g_QuestItem;
	extern CArrayIndexData g_QuestScript;

	// 도우미 대화
	extern CArrayIndexData g_HelperScript;

	// 각성
	extern CArrayIndexData g_RebirthMugong_List;
	extern CArrayIndexData g_RebirthMugong_Desc;

	//extern CArrayIndexData g_SkillTiemIndex;
	extern CArrayIndexData g_QuickIndex;				//HO_0413_07 퀵 가이드 업데이트

	//HT_0824 : 퀘스트 도우미 추가
	extern CArrayIndexData g_QuestHelpList;
	extern CArrayIndexData g_QuestMonList;
	extern CArrayIndexData g_QuestNPCList;

	// 요넘은 난중에 캐릭터나 NPC추가 될때 마다 늘어남
	extern CArrayIndexData g_AniType[ XIAH_ANITYPE_COUNT];

	extern	BOOL LoadXiahArrayIndex();
	extern	BOOL UnLoadXiahArrayIndex();
};