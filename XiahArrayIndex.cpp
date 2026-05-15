#include "precompile.h"
#include "XiahArrayIndex.h"

namespace XiahArrayIndex
{
	CArrayIndexData g_MapFileName;
	CArrayIndexData g_MainCharType;
	CArrayIndexData g_NpcType;	
	CArrayIndexData g_NpcType2;				// 성인서버용 NPC
	CArrayIndexData g_MugongTemplate;
	CArrayIndexData g_MugongList;
	CArrayIndexData g_MugongColumnName;
	CArrayIndexData g_MugongIndex_InGum;
	CArrayIndexData g_MugongIndex_InYun;
	CArrayIndexData g_MugongIndex_InMutu;
	CArrayIndexData g_MugongIndex_InYaCha;
	CArrayIndexData g_MugongIndex_OutGum;
	CArrayIndexData g_MugongIndex_OutYun;
	CArrayIndexData g_MugongIndex_OutMutu;
	CArrayIndexData g_MugongIndex_OutYaCha;
	CArrayIndexData g_QuestDesc;
	CArrayIndexData g_AniType[ XIAH_ANITYPE_COUNT];
	CArrayIndexData g_ItemType;
	CArrayIndexData g_FunctionalNpcType;
	CArrayIndexData g_ItemLength;			// 무기 길이, visualid and length
	CArrayIndexData g_ItemTip;
	CArrayIndexData g_MugongDesc;
	CArrayIndexData g_QuestItem;			// 퀘스트 아이템 툴팁
	CArrayIndexData g_QuestScript;
	CArrayIndexData g_HelperScript;			// 도우미 대화
	// 각성
	CArrayIndexData g_RebirthMugong_List;
	CArrayIndexData g_RebirthMugong_Desc;

	//CArrayIndexData g_SkillTiemIndex;
	CArrayIndexData g_QuickIndex;			//HO_0413_07 퀵 가이드 업데이트

	//HT_0824 : 퀘스트 도우미 추가
	CArrayIndexData g_QuestHelpList;
	CArrayIndexData g_QuestMonList;
	CArrayIndexData g_QuestNPCList;


#define LOAD_ARRAYINDEX( a, b) \
	if( !a.Create( b)) return FALSE

#define UNLOAD_ARRAYINDEX( a ) \
	if( !a.Release() ) return FALSE

	BOOL LoadXiahArrayIndex()
	{
		LOAD_ARRAYINDEX( g_MapFileName,			_T("index\\mapfilename.idx"));
		LOAD_ARRAYINDEX( g_MainCharType,		_T("index\\mainchartype.idx"));
		LOAD_ARRAYINDEX( g_NpcType,				_T("index\\npctype.idx"));		
		LOAD_ARRAYINDEX( g_NpcType2,			_T("index\\npctype2.idx"));			// 성인서버용 NPC
		LOAD_ARRAYINDEX( g_MugongTemplate,		_T("index\\pc_mugong_template.idx"));
		LOAD_ARRAYINDEX( g_MugongList,			_T("index\\pc_mugong_list.idx"));
		LOAD_ARRAYINDEX( g_MugongColumnName,	_T("index\\pc_mugong_column_name.idx"));
		LOAD_ARRAYINDEX( g_MugongIndex_InGum,	_T("index\\inmugongindex_gum.idx"));
		LOAD_ARRAYINDEX( g_MugongIndex_InYun,	_T("index\\inmugongindex_yun.idx"));
		LOAD_ARRAYINDEX( g_MugongIndex_InMutu,	_T("index\\inmugongindex_mu.idx"));
		LOAD_ARRAYINDEX( g_MugongIndex_InYaCha, _T("index\\inmugongindex_ya.idx"));
		LOAD_ARRAYINDEX( g_MugongIndex_OutGum,	_T("index\\outmugongindex_gum.idx"));
		LOAD_ARRAYINDEX( g_MugongIndex_OutYun,	_T("index\\outmugongindex_yun.idx"));
		LOAD_ARRAYINDEX( g_MugongIndex_OutMutu, _T("index\\outmugongindex_mu.idx"));
		LOAD_ARRAYINDEX( g_MugongIndex_OutYaCha,_T("index\\outmugongindex_ya.idx"));

		LOAD_ARRAYINDEX( g_QuestDesc,			_T("index\\quest.idx"));
		LOAD_ARRAYINDEX( g_ItemType,			_T("index\\itemtype.idx"));
		LOAD_ARRAYINDEX( g_FunctionalNpcType,	_T("index\\functional_npc_type.idx"));

		LOAD_ARRAYINDEX( g_ItemLength,			_T("index\\itemlength.idx"));
		LOAD_ARRAYINDEX( g_ItemTip,				_T("index\\itemtip.idx"));

		LOAD_ARRAYINDEX( g_MugongDesc,			_T("index\\MugongDesc.idx"));
		LOAD_ARRAYINDEX( g_QuestItem,			_T("index\\QuestItem.idx"));
		LOAD_ARRAYINDEX( g_QuestScript,			_T("index\\QuestScript.idx"));
		
		LOAD_ARRAYINDEX( g_AniType[ 0],			_T("index\\anitype_mainchar_1.idx"));
		LOAD_ARRAYINDEX( g_AniType[ 1],			_T("index\\anitype_npc_1.idx"));
		LOAD_ARRAYINDEX( g_AniType[ 2],			_T("index\\functional_npc_anitype.idx"));

		// 도우미 대화
		LOAD_ARRAYINDEX(g_HelperScript,			_T("index\\HelpScript.idx"));

		// 각성
		LOAD_ARRAYINDEX(g_RebirthMugong_List,	_T("index\\rebirth_mugong.idx"));
		LOAD_ARRAYINDEX(g_RebirthMugong_Desc,	_T("index\\Rebirth_MugongDesc.idx"));

		//LOAD_ARRAYINDEX(g_SkillTiemIndex,		_T("index\\skilltime.idx"));
		
		//HO_0413_07 퀵 가이드 업데이트
		LOAD_ARRAYINDEX(g_QuickIndex,			_T("index\\Quick.idx"));

		//HT_0824 : 퀘스트 도우미 추가
		LOAD_ARRAYINDEX(g_QuestHelpList,		_T("index\\QuestHelpList.idx"));
		LOAD_ARRAYINDEX(g_QuestMonList,			_T("index\\QuestMonList.idx"));
		LOAD_ARRAYINDEX(g_QuestNPCList,			_T("index\\QuestNPCList.idx"));
		return TRUE;
	}

	BOOL UnLoadXiahArrayIndex()
	{
		UNLOAD_ARRAYINDEX( g_MapFileName);
		UNLOAD_ARRAYINDEX( g_MainCharType);
		UNLOAD_ARRAYINDEX( g_NpcType);		
		UNLOAD_ARRAYINDEX(g_NpcType2);			// 성인서버용 NPC
		UNLOAD_ARRAYINDEX( g_MugongTemplate);
		UNLOAD_ARRAYINDEX( g_MugongList);
		UNLOAD_ARRAYINDEX( g_MugongColumnName);
		UNLOAD_ARRAYINDEX( g_MugongIndex_InGum);
		UNLOAD_ARRAYINDEX( g_MugongIndex_InYun);
		UNLOAD_ARRAYINDEX( g_MugongIndex_InMutu);
		UNLOAD_ARRAYINDEX( g_MugongIndex_InYaCha);
		UNLOAD_ARRAYINDEX( g_MugongIndex_OutGum);
		UNLOAD_ARRAYINDEX( g_MugongIndex_OutYun);
		UNLOAD_ARRAYINDEX( g_MugongIndex_OutMutu);
		UNLOAD_ARRAYINDEX( g_MugongIndex_OutYaCha);
		UNLOAD_ARRAYINDEX( g_QuestDesc);
		UNLOAD_ARRAYINDEX( g_ItemType);
		UNLOAD_ARRAYINDEX( g_FunctionalNpcType);

		UNLOAD_ARRAYINDEX( g_ItemLength);
		UNLOAD_ARRAYINDEX( g_ItemTip);

		UNLOAD_ARRAYINDEX( g_MugongDesc);
		UNLOAD_ARRAYINDEX( g_QuestItem);
		UNLOAD_ARRAYINDEX( g_QuestScript);

		UNLOAD_ARRAYINDEX( g_AniType[ 0]);
		UNLOAD_ARRAYINDEX( g_AniType[ 1]);
		UNLOAD_ARRAYINDEX( g_AniType[ 2]);

		// 도우미 대화
		UNLOAD_ARRAYINDEX(g_HelperScript);

		// 각성
		UNLOAD_ARRAYINDEX(g_RebirthMugong_List);
		UNLOAD_ARRAYINDEX(g_RebirthMugong_Desc);

		//UNLOAD_ARRAYINDEX(g_SkillTiemIndex);

		UNLOAD_ARRAYINDEX(g_QuickIndex);				//HO_0413_07 퀵 가이드 업데이트

		//HT_0824 : 퀘스트 도우미 추가
		UNLOAD_ARRAYINDEX(g_QuestHelpList);
		UNLOAD_ARRAYINDEX(g_QuestMonList);
		UNLOAD_ARRAYINDEX(g_QuestNPCList);

		return TRUE;
	}

};