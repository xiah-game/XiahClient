#include "precompile.h"
#include "AppData.h"
#include "resource.h"	// 원래 precompile에 넣을까 생각했는데. 그냥..

#include "XiahGame_Intro.h"
#include "XiahObjectType.h"
#include "XiahGame_Handler_Sender.h"
#include "CharSack.h"
#include "FunctionalNpcInfo.h"
#include "XiahMap.h"
#include "InterfaceDefine.h"
#include "InterfaceHandler.h"
#include "XiahCursor.h"
#include "ItemInfo.h"
#include "XiahGame_Pet.h"


class CInterfaceMap : public std::hash_map<int,XIAH_INTERFACE_PROCESS_FUNCTION>
{
public:
	inline XIAH_INTERFACE_PROCESS_FUNCTION GetInterfaceHandler(WORD id)
	{
		iterator it = find( id);

		if( it == end())
			return NULL;

		return it->second;
	}
};

CInterfaceMap	g_InterfaceMap;



void RegistInterfaceHandler( int nFrameID, XIAH_INTERFACE_PROCESS_FUNCTION function)
{
	DBG_Assert( g_InterfaceMap.GetInterfaceHandler( nFrameID) == NULL);

	g_InterfaceMap.insert( CInterfaceMap::value_type( nFrameID, function));
}

void ProcessInterfaceHanler( int nFrameID, LPARAM lParam)
{
	CInterfaceMap::iterator it = g_InterfaceMap.find( nFrameID);

	if( it == g_InterfaceMap.end())
	{
		DBG_Put(_T("알수 없는 인터페이스 프레임 아이디 : 0x%X"), nFrameID);
		DBG_LogFile( _T("알수 없는 인터페이스 프레임 아이디 fail %d"), nFrameID);
		return;
	}	

	XIAH_INTERFACE_PROCESS_FUNCTION Handler = it->second;

	DBG_Assert( Handler);

	Handler( lParam);
}





#include "InterfaceHandle_Intro.cpp"
#include "InterfaceHandle_Manage.cpp"
#include "InterfaceHandle_Main.cpp"
#include "InterfaceHandle_Window.cpp"
#include "InterfaceHandle_Notice.cpp"
#include "InterfaceHandle_PopMenu.cpp"

void RegisterInterfaceHandler()
{
	// INTRO
	RegistInterfaceHandler( INTRO_ACCOUNT,			ProcessIntroAccount);
	RegistInterfaceHandler( INTRO_BUTTONSET,		ProcessIntroButtonSet);
	RegistInterfaceHandler( INTRO_CHARACTER_SELECT, ProcessIntroCharacterSelect);
	RegistInterfaceHandler( INTRO_WINDOW01,			ProcessIntroWindow);
	// LOGIN
	RegistInterfaceHandler( LOGIN_1,				ProcessLogin1);
	RegistInterfaceHandler( LOGIN_2,				ProcessLogin2);

	// MAIN
	RegistInterfaceHandler( SITUATION_BRA,			ProcessSituation);
	RegistInterfaceHandler( MAIN_FRAME,				ProcessMainFrame);
	RegistInterfaceHandler( WINDOW_BUTTON_GROUP_01,	ProcessWindowButtonGroup);
	RegistInterfaceHandler( SYSTEM_BUTTON_GROUP_01,	ProcessSystemButtonGroup);
	RegistInterfaceHandler( PET_BUTTON_GROUP,		ProcessPetButtonGroup);
	RegistInterfaceHandler( MAIN_CHAT,				ProcessMainChat);
	//RegistInterfaceHandler( MESSENGER_CHANNEL,		ProcessMessengerChannel);
	RegistInterfaceHandler(SPIRIT,					ProcessSpirit);				// 기
	RegistInterfaceHandler(HELP_BUTTON,				ProcessHELPER);				//HO_0413_07 퀵 가이드 업데이트 

	// WINDOW
	RegistInterfaceHandler( WINDOW_CHARACTER,		ProcessWindowCharacter);
	RegistInterfaceHandler( DRG_ITEM_WINDOW,		ProcessWindowItem);	
	RegistInterfaceHandler( WINDOW_OUTSIDE,			ProcessWindowOutSide);
	RegistInterfaceHandler( WINDOW_INSIDE,			ProcessWindowInSide);
	RegistInterfaceHandler( WINDOW_SKILL,			ProcessWindowSkill);	// 각성 
	RegistInterfaceHandler(WINDOW_FIVEELEMENTS,		ProcessWindowFiveElement);				// 오행
	RegistInterfaceHandler(WINDOW_FIVEELEMENTS_CONVERT,	ProcessWindowFiveElementConvert);	// 오행 제련

	RegistInterfaceHandler( WINDOW_DAN,				ProcessWindowDan);
	RegistInterfaceHandler(WINDOW_DAN_NEW,			ProcessWindowDanNew);					// 단 경험치 분배
	RegistInterfaceHandler( WINDOW_MUNPA,			ProcessWindowClan);
	RegistInterfaceHandler( WINDOW_MUNPA_FOUND,		ProcessWindowClanFound);
	RegistInterfaceHandler( WINDOW_NEW_TAMING,		ProcessWindowTaming);
	RegistInterfaceHandler( WINDOW_TAMING_ITEM,		ProcessWindowTamingItem);
	RegistInterfaceHandler( WINDOW_PC_TRADE,		ProcessWindowPcTrade);
	RegistInterfaceHandler( WINDOW_NPC_TRADE,		ProcessWidnowNpcTrade);
	RegistInterfaceHandler( WINDOW_CONVERT,			ProcessWindowConvert);
	RegistInterfaceHandler( WINDOW_VOLUME,			ProcessWindowVolume);
	RegistInterfaceHandler( WINDOW_CONNECTION_INFO,	ProcessWindowConnectionInfo);
	RegistInterfaceHandler( WINDOW_NAME_CONFER,		ProcessWindowNameConfer);
	RegistInterfaceHandler( NAME_CHANNEL,			ProcessNameChannel);
	RegistInterfaceHandler( WINDOW_QUEST_01,			ProcessWindowQuest);
	RegistInterfaceHandler(WINDOW_OPTION_01,		ProcessWindowOption1);	// 게임 옵션
	RegistInterfaceHandler(WINDOW_OPTION_02,		ProcessWindowOption2);	// 환경 옵션
	RegistInterfaceHandler(WINDOW_OPTION_03,		ProcessWindowOption3);	// 거래 옵션
	RegistInterfaceHandler(TAP_NPC_TRADE_4,			ProcessTabNpcTramde4);
	RegistInterfaceHandler(WINDOW_CLOSE,			ProcessWindowClose);

	RegistInterfaceHandler( WINDOW_PC_STORE,		ProcessWindowPcStore);	// 개인 상점
	RegistInterfaceHandler( WINDOW_MONEY,			ProcessWindowMoney);
	
	RegistInterfaceHandler( GAK_MESSAGE_WINDOW,		ProcessWindowGakMessage);
	RegistInterfaceHandler( WINDOW_DONGSIN,			ProcessWindowDongSin);

	RegistInterfaceHandler(WINDOW_DAN_WAR,			ProcessWindowDanWar);	// 단 비무
	RegistInterfaceHandler(WINDOW_PURSE,			ProcessWindowPurse);	// 전낭

	RegistInterfaceHandler(WINDOW_MUNPA_BBS_TOP,	ProcessWindowMunpaBBSTop);
	RegistInterfaceHandler(WINDOW_MUNPA_BBS_LIST,	ProcessWindowMunpaBBSList);
	RegistInterfaceHandler(WINDOW_MUNPA_BBS_READ,	ProcessWindowMunpaBBSRead);	
	RegistInterfaceHandler(WINDOW_MUNPA_BBS_WRITE,	ProcessWindowMunpaBBSWrite);
	RegistInterfaceHandler(WINDOW_MUNPA_DONATE,		ProcessWindowMunpaDonate);
	RegistInterfaceHandler(WINDOW_MUNPA_WAR_PETITION, ProcessWindowMunpaWarPetition);
	RegistInterfaceHandler(MESSAGE_WINDOW_MARK,		ProcessWindowMark);		// 문파문장

	RegistInterfaceHandler(WINDOW_MAIL,				ProcessWindowMail);
	RegistInterfaceHandler(WINDOW_MAIL_SELECT,		ProcessWindowMailSelect);
	RegistInterfaceHandler(WINDOW_MAIL_RESULT,		ProcessWindowMailResult);

	RegistInterfaceHandler(WINDOW_COMMON,			ProcessWindowCommon);
	RegistInterfaceHandler(WINDOW_PET_TRADE,		ProcessWindowPetTrade);

	RegistInterfaceHandler(WINDOW_BOK_NUMBER,		ProcessWindowBokNumber);		// 복권번호선택
	RegistInterfaceHandler(WINDOW_BOK_PRIZE,		ProcessWindowBokPrize);			// 당첨번호횅땍

	RegistInterfaceHandler(WINDOW_SMELT,			ProcessWindowSmelt);			// 조합창

	// 도우미 NPC
	RegistInterfaceHandler(WINDOW_HELPER_LIST2,		ProcessWindowHelperList);	// 대화 리스트
	//HO_0410_07 상서령 가이드 업데이트
	RegistInterfaceHandler(WINDOW_HELPER_SCRIPT,	ProcessWindowTamRangScript);	// 상서령 가이드 업데이트 : 탐랑 대화용으로 변환
	RegistInterfaceHandler(WINDOW_HELPER_LIST0,		ProcessWindowQuickScript);		// HO_0413_07 퀵 가이드 업데이트 : 퀵 가이드 내용
	RegistInterfaceHandler(WINDOW_HELPER_LIST1,		ProcessWindowHelperList);		// 대화 리스트
	RegistInterfaceHandler(WINDOW_HELPER_LIST,		ProcessWindowHelperScript);		// 대화 내용
	RegistInterfaceHandler(WINDOW_PORTAL,			ProcessWindowPortal);			// NPC 포탈 이동
	
	RegistInterfaceHandler(WINDOW_RECOVERY,			ProcessWindowRecovery);			// 아이템 복구

	RegistInterfaceHandler(WINDOW_COLLECTION,		ProcessWindowCollection);		// 아이템 수집

	RegistInterfaceHandler(WINDOW_SECRET_CHECK,		ProcessWindowSecretMove);		//HT_0313 : 광명전 & 천황전 (이동)	
	RegistInterfaceHandler(WINDOW_SECRET_INFORMATION,		ProcessWindowSecretApplication);		//HT_0313 : 광명전 & 천황전 (참여)	


	// NOTICE
	RegistInterfaceHandler(MESSAGE_WINDOW_1BUTTON,	ProcessNoticeWindow1);
	RegistInterfaceHandler(MESSAGE_WINDOW_2BUTTON,	ProcessNoticeWindow2);
	// POP MENU
	RegistInterfaceHandler(FRAMEID_PC,				ProcessPopMenuPc);			// PC
	RegistInterfaceHandler(FRAMEID_NPC,				ProcessPopMenuNpc);			// NPC
	RegistInterfaceHandler(FRAMEID_PET,				ProcessPopMenuPet);			// PET
	RegistInterfaceHandler(FRAMEID_STONE,			ProcessPopMenuStone);		// STONE
	RegistInterfaceHandler(FRAMEID_OFFICIAL,		ProcessPopMenuOfficial);	// 정사관
	RegistInterfaceHandler(FRAMEID_ALCHEMIST,		ProcessPopMenuAlchemist);	// 연금술사
	RegistInterfaceHandler(FRAMEID_HELP,			ProcessPopMenuHelp);		// 상서령
	RegistInterfaceHandler(FRAMEID_NPC_PORTAL,		ProcessPopMenuNpcPortal);	// NPC 포탈 이동 상서령	
	RegistInterfaceHandler(FRAMEID_HELP_2,			ProcessPopMenuHelp2);		// 성녀
	RegistInterfaceHandler(FRAMEID_CLAN_WAR,		ProcessPopMenuClanWar);		// 문파대전 관리인
	RegistInterfaceHandler(FRAMEID_REBIRTHITEM,		ProcessPopMenuRebirthItem);	//HT_1116 : 각성자 아이템 추가

	
	// POP SUBMENU
	RegistInterfaceHandler(FRAMEID_PC_RELATION,		ProcessPopMenuRelation);
	RegistInterfaceHandler(FRAMEID_PET_AI,			ProcessPopMenuAI);
	RegistInterfaceHandler(FRAMEID_PET_SPECIAL,		ProcessPopMenuSpecial);
	RegistInterfaceHandler(FRAMEID_STONE_SUB_1,		ProcessPopMenuStoneSub1);
	RegistInterfaceHandler(FRAMEID_STONE_SUB_2,		ProcessPopMenuStoneSub2);
	RegistInterfaceHandler(FRAMEID_STONE_SUB_3,		ProcessPopMenuStoneSub3);
	RegistInterfaceHandler(FRAMEID_PC_TRADE,		ProcessPopMenuPcTrade);	
	RegistInterfaceHandler(FRAMEID_LOTTO,			ProcessPopMenuLotto);		// 복권
	RegistInterfaceHandler(FRAMEID_SECRET,			ProcessPopMenuSecretRoom);	//HT_0313 : 광명전 & 천황전

	// POP SUBMENU(COMBO)
	RegistInterfaceHandler(FRAMEID_BONBINITEM,		ProcessPopMenuBongInItem);	// 봉인
	RegistInterfaceHandler(FRAMEID_CRYOLITE,		ProcessPopMenuCryolite);	// 빙정 봉인
	RegistInterfaceHandler(FRAMEID_PURSE,			ProcessPopMenuPurseItem);	// 전낭 콤보

	RegistInterfaceHandler(FRAMEID_WAR_DAY,			ProcessPopMenuWarDay);		// 대전 시간
	RegistInterfaceHandler(FRAMEID_STONE_MOVE,		ProcessPopMenuStoneMove);	// 문파 비석 이전

	RegistInterfaceHandler(FRAMEID_REVIVAL,			ProcessPopMenuRevival);		// 펫 복구 리스트

	RegistInterfaceHandler(FRAMEID_LOTTOCHECK,		ProcessPopMenuLottoCheck);	// 복권아이템 당첨횅땍/당첨금수령
}





// Interface모듈에서 보내온 메시지를 처리하는 함수
LRESULT ProcessInterfaceMessage( WPARAM wParam,LPARAM lParam)
{
	BeginScene();
	ProcessInterfaceHanler( wParam, lParam);
	EndScene();

	return 0;
}