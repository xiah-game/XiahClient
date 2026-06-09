#include "precompile.h"
#include "AppData.h"
#include "resource.h"
#include "InterfaceDefine.h"
#include "resource.h"
#include "XiahNetworkHandler.h"
#include "XiahSocket.h"
#include "XiahGame_Intro.h"
#include "XiahCamera.h"
#include "XiahMap.h"
#include "XiahArrayIndex.h"
#include "XiahGameMain.h"




void XiahGame_Intro::Init_Frames()
{
	Init_IntroAccount();
	Init_IntroButtonSet();
	Init_IntroCharaterSelect();
	Init_IntroWindow();

	Init_MiniMapWindow();
	Init_DateWindow();

	// 晞灅 欤检劃摛 OnCS_IT_CHARSTATUSINFO_ACK氚涥碃 觳橂Μ
	//Init_MainFrame();
	Init_WindowButton();
	//Init_SystemButton();
	Init_ChatFrame();
	//Init_MessengerChannel();
	Init_PetControlFrame();
	
	//Init_WindowCharacter();
	Init_WindowItem();
	//Init_WindowOutSide();
	//Init_WindowInSide();
	Init_WindowDan();
	Init_WindowClan();
	Init_WindowClanFound();
	Init_WindowTaming();
	Init_WindowTamingItem();

	Init_WindowPcTrade();
	Init_WindowNpcTrade();
	Init_WindowNpcTradeTab();
	Init_WindowConvert();
	Init_WindowVolume();
	Init_WindowConnectionInfo();
	Init_WindowNameConfer();
	Init_NameChannel();
	Init_WindowQuest();

	Init_WindowOption();
	Init_WindowClose();

	Init_WindowPcStore();
	Init_WindowMoney();

	Init_WindowGakMessage();
	Init_WindowDongsin();

	Init_WindowDanWar();
	Init_WindowPurse();					// 爠偔

	Init_WindowMunpaBBSTop();
	Init_WindowMunpaBBSList();
	Init_WindowMunpaBBSRead();
	Init_WindowMunpaBBSWrite();
	Init_WindowMunpaDonate();
	Init_WindowMunpaWarPetition();

	Init_WindowMark();

	Init_WindowMail();
	Init_WindowMailSelect();
	Init_WindowMailResult();

	Init_WindowCommon();
	Init_WindowPetTrade();

	Init_WindowBokNumber();				// 氤店秾劆儩
	Init_WindowBokPrize();				// 氤店秾嫻觳氩垬
	Init_WindowSmelt();					// 臁绊暕彀
	Init_WindowFiveElementConvert();	// 槫枆 牅牗

	Init_WindowHelperList();			// 檾 毽鞀姼
	Init_WindowHelperScript();			// 檾彀

	Init_WindowRecovery();				// 晞澊厹 氤店惮

	Init_WindowDanNew();				// 嫧 瓴巾棙旃 攵勲鞍

	Init_WindowPortal();				// NPC 彫儓 澊彊

	Init_WindowCollection();			// 晞澊厹 垬歆

	Init_WindowSecretMove();			//HT_0313 : 甏戨獏爠 & 觳滍櫓爠	(澊彊)
	Init_WindowSecretApplication();		//HT_0313 : 甏戨獏爠 & 觳滍櫓爠	(彀胳棳)
	Init_WindowSecretBasis();	
}








//////////////////////////////////////////
// GAME INTRO
//////////////////////////////////////////
void XiahGame_Intro::Init_IntroAccount()
{
	g_pUIManager->SetPosition(INTRO_ACCOUNT, INTRO_ACCOUNT_XPOS, INTRO_ACCOUNT_YPOS);

	g_pUIManager->SetData(INTRO_ACCOUNT, account_name_edit, MAXSTRING, INTRO_ACCOUNT_EDIT_MAX_STRING);

	g_pUIManager->SetString(INTRO_ACCOUNT, account_name_dumy, IDS_NAME);
	g_pUIManager->SetString(INTRO_ACCOUNT, account_button, IDS_INTRO_ACCOUNT_BUTTON);

	// q[4/21/2004]
	//pFrame->GetControl( intro_account_mixture_china)->SetPositionS(55,-13);

	g_pUIManager->Hide(INTRO_ACCOUNT, intro_account_mixture_china);

//	g_pUIManager->GetFrame( INTRO_ACCOUNT)->GetControl( account_name_image)->Hide();
}

void XiahGame_Intro::Init_IntroButtonSet()
// 澑姼搿 齑堦赴 檾氅 氅旊壌 劆儩 氩勴娂
{
	g_pUIManager->SetPosition(INTRO_BUTTONSET, INTRO_BUTTONSET_XPOS, INTRO_BUTTONSET_YPOS);

	g_pUIManager->SetString(INTRO_BUTTONSET, INTRO_BUTTON_01, IDS_INTRO_BUTTONSET_BUTTON11);
	g_pUIManager->SetString(INTRO_BUTTONSET, INTRO_BUTTON_04, IDS_INTRO_BUTTONSET_BUTTON14);
	g_pUIManager->SetString(INTRO_BUTTONSET, INTRO_BUTTON_03, IDS_INTRO_MULTICHANNEL);
	g_pUIManager->SetString(INTRO_BUTTONSET, INTRO_BUTTON_02, IDS_TERMINATE);	
}

void XiahGame_Intro::Init_IntroCharaterSelect()
// 澑姼搿 齑堦赴 檾氅 旌愲Ν劙 劆儩 氩勴娂
{
	g_pUIManager->SetPosition(INTRO_CHARACTER_SELECT, INTRO_CHARACTER_SELECT_XPOS, INTRO_CHARACTER_SELECT_YPOS); 
}

void XiahGame_Intro::Init_IntroWindow()
// 澑姼搿 齑堦赴 檾氅 旌愲Ν劙 劋氇 彀
{
	g_pUIManager->SetPosition(INTRO_WINDOW01, INTRO_WINDOW01_XPOS, INTRO_WINDOW01_YPOS);

	g_pUIManager->SetString(INTRO_WINDOW01, intro_window_title_back, IDS_INTRO_WINDOW01_TITLE);
	g_pUIManager->SetString(INTRO_WINDOW01, intro_window_dummy_01, IDS_INTRO_WINDOW01_DUMMY1);
	g_pUIManager->SetString(INTRO_WINDOW01, intro_window_dummy_02, IDS_INTRO_WINDOW01_DUMMY2);
	g_pUIManager->SetString(INTRO_WINDOW01, intro_window_dummy_03, IDS_INTRO_WINDOW01_DUMMY3);
}









/////////////////////////////////////////////////////////////
// GAME MAIN
////////////////////////////////////////////////////////////
void XiahGame_Intro::Init_MiniMapWindow()
{
	g_pUIManager->SetPosition(MINIMAP_WINDOW, MINIMAP_WINDOW_XPOS, MINIMAP_WINDOW_YPOS);
	g_pUIManager->Hide(MINIMAP_WINDOW, minimap_window_direction);
}

void XiahGame_Intro::Init_DateWindow()
{
	g_pUIManager->SetPosition(DATA_WINDOW, WINDOW_FIRST_XPOS, 0);


	g_pUIManager->SetData(MAIN_FRAME, main_frame_inside_gauge, TYPE, CUIProgressCtrl::RIGHT_TO_LEFT);
}

// 氅旍澑 攧爤瀯
void  XiahGame_Intro::Init_MainFrame()
{
	if(g_MainCharInfo.m_wLevel < 11)//HO_0413_07  臧澊摐 梾嵃澊姼 : 旌愲Ν劙臧 嫓瀾悹晫 臧戩瀽 觳错伂泟 店澊摐 嫧於曤矂娂潉 氤挫棳欷
		g_pUIManager->Show(HELP_BUTTON);

	g_pUIManager->SetPosition(HELP_BUTTON, 44, 690);
	g_pUIManager->SetToolTip(HELP_BUTTON, help_button_resource, 1, IDS_QUICK_GUIDE, 2); 

	g_pUIManager->SetPosition(MAIN_FRAME, MAIN_FRAME_XPOS, MAIN_FRAME_YPOS);

	BYTE bHpPer = 0;
	BYTE bIpPer = 0;
	int bLevelExpPer = 0;
	int bTPExpPer = 0;

	if( g_MainCharInfo.m_dwHpMax == 0 || g_MainCharInfo.m_dwHpCur > g_MainCharInfo.m_dwHpMax)
		bHpPer = 100;
	else
		bHpPer	= 100 * g_MainCharInfo.m_dwHpCur / g_MainCharInfo.m_dwHpMax;

	if( g_MainCharInfo.m_wIpMax == 0 || g_MainCharInfo.m_wIpCur > g_MainCharInfo.m_wIpMax)
	{
		bIpPer = 100;
	}
	else
	{
		bIpPer	= 100 * g_MainCharInfo.m_wIpCur / g_MainCharInfo.m_wIpMax;
	}

	//int dwMAXLevelExp = g_MainCharInfo.m_dwNextLevelUpExp - g_MainCharInfo.m_dwLevelExp;
	//int dwSUBLevelExp = g_MainCharInfo.m_dwExp - g_MainCharInfo.m_dwLevelExp;
	INT64 i64MAXLevelExp = g_MainCharInfo.m_i64NextLevelUpExp - g_MainCharInfo.m_i64LevelExp;
	INT64 i64SUBLevelExp = g_MainCharInfo.m_i64Exp - g_MainCharInfo.m_i64LevelExp;

	//int dwMAXTPExp = g_MainCharInfo.m_dwNextTpUpExp - g_MainCharInfo.m_dwTpExp;
	//int dwSUBTPExp = g_MainCharInfo.m_dwExp - g_MainCharInfo.m_dwTpExp;
	INT64 i64MAXTPExp = g_MainCharInfo.m_i64NextTpUpExp - g_MainCharInfo.m_i64TpExp;
	INT64 i64SUBTPExp = g_MainCharInfo.m_i64Exp - g_MainCharInfo.m_i64TpExp;

	if( i64SUBLevelExp < 1)
		i64SUBLevelExp = 0;
	if( i64SUBTPExp < 1)
		i64SUBTPExp = 0;

	if( i64MAXLevelExp < 1)
		bLevelExpPer = 100;
	else
		bLevelExpPer = 100 * i64SUBLevelExp / i64MAXLevelExp;	

	if( i64MAXTPExp < 1)
		bTPExpPer = 100;
	else
		bTPExpPer = 100 * i64SUBTPExp / i64MAXTPExp;	

	if( bHpPer > 100)
		bHpPer = 100;
	if( bIpPer > 100)
		bIpPer = 100;
	if( bLevelExpPer >= 100)
		bLevelExpPer = 100;
	else if( bLevelExpPer < 1)
		bLevelExpPer = 0;
		
	if( bTPExpPer >= 100)
		bTPExpPer = 100;
	else if( bTPExpPer < 1)
		bTPExpPer = 0;

	g_pUIManager->SetData(MAIN_FRAME, main_frame_outside_gauge, VALUE1, bHpPer);
	g_pUIManager->SetData(MAIN_FRAME, main_frame_inside_gauge,	VALUE1, bIpPer);
	g_pUIManager->SetData(MAIN_FRAME, main_frame_level_gauge,	VALUE1, bLevelExpPer);
	g_pUIManager->SetData(MAIN_FRAME, main_frame_skill_gauge,	VALUE1, bTPExpPer);

	// 埓寔
	g_pUIManager->SetToolTip(MAIN_FRAME, main_frame_window_button, 1, IDS_GAME_MENU, 2);
	g_pUIManager->SetToolTip(MAIN_FRAME, main_frame_system_button, 1, IDS_SYSTEM_MENU, 2);

	//HT_0720 : 槫枆 臧滌劆 偓暛
	g_pUIManager->SetData(MAIN_FRAME, main_frame_ok, TEXTURE, 1536);
}

void XiahGame_Intro::Init_WindowButton()
{	
	g_pUIManager->SetPosition(WINDOW_BUTTON_GROUP_01, WINDOW_BUTTON_GROUP_XPOS, WINDOW_BUTTON_GROUP_YPOS);

	// 埓寔
	g_pUIManager->SetToolTip(WINDOW_BUTTON_GROUP_01, new_window_button_01, 1, IDS_SACK1, 2);
	g_pUIManager->SetToolTip(WINDOW_BUTTON_GROUP_01, new_window_button_02, 1, IDS_STATUS1, 2);
	g_pUIManager->SetToolTip(WINDOW_BUTTON_GROUP_01, new_window_button_03, 1, IDS_MUGONG1, 2);
	g_pUIManager->SetToolTip(WINDOW_BUTTON_GROUP_01, new_window_button_04, 1, IDS_RELATION1, 2);
	g_pUIManager->SetToolTip(WINDOW_BUTTON_GROUP_01, new_window_button_05, 1, IDS_FIGHT, 2);
	g_pUIManager->SetToolTip(WINDOW_BUTTON_GROUP_01, new_window_button_06, 1, IDS_COLLECTION2, 2);


	// 旮
	g_pUIManager->SetPosition(SPIRIT, 4, 690);
	g_pUIManager->Hide(SPIRIT);
}

void XiahGame_Intro::Init_SystemButton()
{
	RECT rtTemp;
	
	//g_pUIManager->GetRegionData(MAIN_FRAME, main_frame_system_button, rtTemp);
	// 垬歆 晞澊旖 晫氍胳棎 垬爼
	//HO_0413_07  臧澊摐 梾嵃澊姼 : 氩勴娂 於旉搿 澑暅 渼旃 垬爼
	g_pUIManager->GetRegionData(WINDOW_BUTTON_GROUP_01, new_window_button_02, rtTemp);// 臧澊摐 氩勴娂 於旉搿 澑暅 new_window_button_06潉 02搿 灛劋爼
	g_pUIManager->SetPosition(SYSTEM_BUTTON_GROUP_01, rtTemp.right-2, SYSTEM_BUTTON_GROUP_YPOS); //, FALSE // 臧澊摐 氩勴娂 於旉搿 澑暅 rtTemp.right棎 -2毳 彫暔暣欷
	
	//g_pUIManager->GetRegionData(WINDOW_BUTTON_GROUP_01, new_window_button_06, rtTemp); // 臧澊摐 氩勴娂 於旉爠 渼旃
	//g_pUIManager->SetPosition(SYSTEM_BUTTON_GROUP_01, rtTemp.right, WINDOW_BUTTON_GROUP_YPOS); //, FALSE // 臧澊摐 氩勴娂 於旉爠 渼旃

	// 埓寔
	g_pUIManager->SetToolTip(SYSTEM_BUTTON_GROUP_01, new_system_button_01, 1, IDS_MINIMAP, 2);
	g_pUIManager->SetToolTip(SYSTEM_BUTTON_GROUP_01, new_system_button_02, 1, IDS_HELP, 2);
	g_pUIManager->SetToolTip(SYSTEM_BUTTON_GROUP_01, new_system_button_03, 1, IDS_OPTION, 2);
	g_pUIManager->SetToolTip(SYSTEM_BUTTON_GROUP_01, new_system_button_04, 1, IDS_EXIT, 2);
}

void  XiahGame_Intro::Init_ChatFrame()
// 毂勴寘 彀
{
	g_pUIManager->SetPosition(MAIN_CHAT, MAIN_CHAT_XPOS, MAIN_CHAT_YPOS);

	g_pUIManager->SetPosition(SMALL_MESSENGER, SMALL_MESSENGER_XPOS, SMALL_MESSENGER_YPOS);
	g_pUIManager->SetPosition(LARGE_MESSENGER, LARGE_MESSENGER_XPOS, LARGE_MESSENGER_YPOS);

	g_pUIManager->Hide(SMALL_MESSENGER, small_messenger_mixture);
	g_pUIManager->Hide(LARGE_MESSENGER, large_messenger_mixture);

	/*
	g_pUIManager->GetFrame( SMALL_MESSENGER)->GetControl(small_messenger_mixture_china)->Hide();
	g_pUIManager->GetFrame( LARGE_MESSENGER)->GetControl(large_messenger_mixture_china)->Hide();
	*/
	/*
	g_pUIManager->GetFrame( SMALL_MESSENGER)->GetControl(small_messenger_mixture)->SetPosition(SMALL_MESSENGER_XPOS+115,SMALL_MESSENGER_YPOS+140,TRUE);
	g_pUIManager->GetFrame( LARGE_MESSENGER)->GetControl(large_messenger_mixture)->SetPosition(LARGE_MESSENGER_XPOS+115,LARGE_MESSENGER_YPOS+689,TRUE);
	*/

	g_pUIManager->SetData(MAIN_CHAT, chat_name_edit, MAXSTRING, 14);
	g_pUIManager->SetData(MAIN_CHAT, main_chat_edit, MAXSTRING, 50);

	g_pUIManager->SetData(MAIN_CHAT, main_chat_select_01, CURRENT_INDEX, 0);
	g_pUIManager->SetData(MAIN_CHAT, main_chat_select_02, CURRENT_INDEX, 0);
	g_pUIManager->SetData(MAIN_CHAT, main_chat_select_03, CURRENT_INDEX, 0);
	g_pUIManager->SetData(MAIN_CHAT, main_chat_select_04, CURRENT_INDEX, 0);

	g_pUIManager->SetToolTip(MAIN_CHAT, main_chat_select_01, 1, IDS_GENERAL_CHAT_FILTER, 2);
	g_pUIManager->SetToolTip(MAIN_CHAT, main_chat_select_02, 1, IDS_WHISPER_CHAT_FILTER, 2);
	g_pUIManager->SetToolTip(MAIN_CHAT, main_chat_select_03, 1, IDS_CLAN_CHAT_FILTER, 2);
	g_pUIManager->SetToolTip(MAIN_CHAT, main_chat_select_04, 1, IDS_DAN_CHAT_FILTER, 2);

	g_pUIManager->SetData(MAIN_CHAT, main_chat_channel_select_01, CURRENT_INDEX, 0);
	g_pUIManager->SetData(MAIN_CHAT, main_chat_channel_select_02, CURRENT_INDEX, 1);
	g_pUIManager->SetData(MAIN_CHAT, main_chat_channel_select_03, CURRENT_INDEX, 1);
	g_pUIManager->SetData(MAIN_CHAT, main_chat_channel_select_04, CURRENT_INDEX, 1);

	g_pUIManager->SetData(MAIN_CHAT, main_chat_mode_select_01, CURRENT_INDEX, 1);
	g_pUIManager->SetToolTip(MAIN_CHAT, main_chat_mode_select_01, 1, IDS_CHAT_MODE, 2);
}

/*
void XiahGame_Intro::Init_MessengerChannel()
{
	g_pUIManager->SetPosition(MESSENGER_CHANNEL, MESSENGER_CHANNEL_XPOS, MESSENGER_CHANNEL_YPOS);

	g_pUIManager->SetString(MESSENGER_CHANNEL, messenger_channel_button_01, IDS_GENERAL_CHAT);
	g_pUIManager->SetString(MESSENGER_CHANNEL, messenger_channel_button_02, IDS_DAN_CHAT);
	g_pUIManager->SetString(MESSENGER_CHANNEL, messenger_channel_button_03, IDS_WHISPER);
	g_pUIManager->SetString(MESSENGER_CHANNEL, channel_button_04, IDS_CLAN_CHAT);
}
*/

void XiahGame_Intro::Init_PetControlFrame()
{
	g_pUIManager->SetPosition(PET_BUTTON_GROUP, PET_BUTTON_GROUP_XPOS, WINDOW_BUTTON_GROUP_YPOS);

	g_pUIManager->SetToolTip(PET_BUTTON_GROUP, pet_button_01, 1, IDS_CALL_PET, 2);
	g_pUIManager->SetToolTip(PET_BUTTON_GROUP, pet_button_02, 1, IDS_COMPANY_ATTK, 2);
	g_pUIManager->SetToolTip(PET_BUTTON_GROUP, pet_button_03, 1, IDS_OBJECT_ATTK, 2);
	g_pUIManager->SetToolTip(PET_BUTTON_GROUP, pet_button_04, 1, IDS_MUGONG_ATTK, 2);
	g_pUIManager->SetToolTip(PET_BUTTON_GROUP, pet_button_05, 1, IDS_ITEM_COLLECT, 2);
}


















////////////////////////////////////////////
//	Window
////////////////////////////////////////////
void XiahGame_Intro::Init_WindowCharacter()
// 旌愲Ν劙 爼氤 彀
{
	g_pUIManager->SetPosition(WINDOW_CHARACTER, WINDOW_FIRST_XPOS, 0);

	////////////////
	//  旮半掣 爼氤
	////////////////
	TCHAR strName[128]={0,};
	TCHAR strClass[64]={0,};
	TCHAR strLevel[64]={0,};
	TCHAR strExp[64]={0,};
	TCHAR strFame[64]={0,};
	BYTE byType;
	
	switch( g_MainCharInfo.m_bCharType)
	{
	case 1:
		_tcscpy( strClass, IDS_GUMYONG);
		break;
	case 2:
		_tcscpy( strClass, IDS_YUNRANG);
		break;
	case 3:
		_tcscpy( strClass, IDS_MUTU);
		break;
	case 4:
		_tcscpy( strClass, IDS_YACHA);
		break;
	}	
	//_tcscpy( strName, g_MainCharInfo.m_szNickName);
	_stprintf( strLevel, IDS_D_GABJA1, g_MainCharInfo.m_wLevel);
	//_stprintf( strExp, "%d / %d", g_MainCharInfo.m_dwExp, g_MainCharInfo.m_dwNextLevelUpExp);
	//int dwMAXLevelExp = g_MainCharInfo.m_dwNextLevelUpExp - g_MainCharInfo.m_dwLevelExp;
	//int dwSUBLevelExp = g_MainCharInfo.m_dwExp - g_MainCharInfo.m_dwLevelExp;

	//HT_0621 : 瓴巾棙旃 垬旃 垬爼
	INT64 i64MAXLevelExp = g_MainCharInfo.m_i64NextLevelUpExp - g_MainCharInfo.m_i64LevelExp;
	INT64 i64SUBLevelExp = g_MainCharInfo.m_i64Exp - g_MainCharInfo.m_i64LevelExp;
	_stprintf( strExp, _T("%I64d / %I64d"), i64SUBLevelExp, i64MAXLevelExp);

	if( g_MainCharInfo.m_dwFame > 126)
	{
		g_pUIManager->SetString(WINDOW_CHARACTER, character_window_contents_dummy_12, IDS_FAME, 0);

		_stprintf( strFame, IDS_RATE, g_MainCharInfo.m_dwFame - 127);
		byType = 0;
	}
	else
	{
		g_pUIManager->SetString(WINDOW_CHARACTER, character_window_contents_dummy_12, VICE_LEVEL, 1);

		_stprintf( strFame, IDS_RATE, 127 - g_MainCharInfo.m_dwFame);
		byType = 1;
	}

	g_pUIManager->SetString(WINDOW_CHARACTER, character_window_title_back, IDS_STATUS);
	g_pUIManager->SetString(WINDOW_CHARACTER, character_window_contents_dummy_01, IDS_NAME);
	g_pUIManager->SetString(WINDOW_CHARACTER, character_window_contents_dummy_02, IDS_CLASS);
	g_pUIManager->SetString(WINDOW_CHARACTER, character_window_contents_dummy_03, IDS_LEVEL);
	g_pUIManager->SetString(WINDOW_CHARACTER, character_window_contents_dummy_04, IDS_EXP);
	
	if(g_MainCharInfo.m_bRebirth)
	{		
		if(g_MainCharInfo.m_bRebirth < 7)	//HT_0702 : 臧侅劚瀽
			_stprintf( strName, IDS_REBIRTH_COUNT_01, (LPCTSTR)g_MainCharInfo.m_szNickName, g_MainCharInfo.m_bRebirth);
		else				//歆勱皝劚瀽
			_stprintf( strName, IDS_2TH_REBIRTH_COUNT_01,(LPCTSTR)g_MainCharInfo.m_szNickName, (g_MainCharInfo.m_bRebirth - 6));

		g_pUIManager->SetString(WINDOW_CHARACTER, character_window_contents_dummy_001, strName);
	}
	else
	{
        _tcscpy( strName, g_MainCharInfo.m_szNickName);
        g_pUIManager->SetString(WINDOW_CHARACTER, character_window_contents_dummy_001, strName);
	}
	g_pUIManager->SetString(WINDOW_CHARACTER, character_window_contents_dummy_002, strClass);
	g_pUIManager->SetString(WINDOW_CHARACTER, character_window_contents_dummy_003, strLevel);
	g_pUIManager->SetString(WINDOW_CHARACTER, character_window_contents_dummy_004, strExp);
	g_pUIManager->SetString(WINDOW_CHARACTER, character_window_contents_dummy_012, strFame, byType);

	////////////////
	//  旮半掣旃
	////////////////
	g_pUIManager->SetString(WINDOW_CHARACTER, character_window_dummy_01, IDS_STR);
	g_pUIManager->SetString(WINDOW_CHARACTER, character_window_dummy_02, IDS_DEF);
	g_pUIManager->SetString(WINDOW_CHARACTER, character_window_dummy_03, IDS_AGI);
	g_pUIManager->SetString(WINDOW_CHARACTER, character_window_dummy_04, IDS_LIFE);
	g_pUIManager->SetString(WINDOW_CHARACTER, haracter_window_dummy_09, IDS_PWR);

	g_pUIManager->SetString(WINDOW_CHARACTER, character_window_dummy_05, g_MainCharInfo.m_wStr);
	g_pUIManager->SetString(WINDOW_CHARACTER, character_window_dummy_06, g_MainCharInfo.m_wSus);
	g_pUIManager->SetString(WINDOW_CHARACTER, character_window_dummy_07, g_MainCharInfo.m_wDex);
	g_pUIManager->SetString(WINDOW_CHARACTER, character_window_dummy_08, g_MainCharInfo.m_wVit);
	g_pUIManager->SetString(WINDOW_CHARACTER, haracter_window_dummy_10, g_MainCharInfo.m_wRemainSp);

	////////////////
	//  儊劯旃
	////////////////	
	TCHAR strHp[20]={0,};
	TCHAR strIp[20]={0,};
	TCHAR atk[20]={0,};
	TCHAR def[20]={0,};
	TCHAR atkrat[20]={0,};
	TCHAR plusspeed[20]={0,};
	TCHAR critical[20]={0,};
	
	int plusAtk = g_MainCharInfo.m_dwTotalAtkPower - g_MainCharInfo.m_wBaseAtkPwr;
	int plusDef = g_MainCharInfo.m_dwTotalDefPower - g_MainCharInfo.m_wBaseDefPwr;
	int plusAtkrat = g_MainCharInfo.m_dwTotalAttackRating - g_MainCharInfo.m_wBaseAttackRating;

	_stprintf( strHp,_T( "%d / %d"),	g_MainCharInfo.m_dwHpCur, g_MainCharInfo.m_dwHpMax);
	_stprintf( strIp,	_T("%d / %d"),	g_MainCharInfo.m_wIpCur, g_MainCharInfo.m_wIpMax);
	if( plusAtk)
		_stprintf( atk,   _T("%d + %d"),	g_MainCharInfo.m_wBaseAtkPwr, plusAtk);
	else
		_stprintf( atk,   _T("%d"),	g_MainCharInfo.m_wBaseAtkPwr);
	if( plusDef)
		_stprintf( def,   _T("%d + %d"),	g_MainCharInfo.m_wBaseDefPwr, plusDef);
	else
		_stprintf( def,	_T("%d"),	g_MainCharInfo.m_wBaseDefPwr);
	if( plusAtkrat)
		_stprintf( atkrat,_T("%d + %d"),	g_MainCharInfo.m_wBaseAttackRating, plusAtkrat);
	else
		_stprintf( atkrat,_T("%d"),	g_MainCharInfo.m_wBaseAttackRating);

	_stprintf( plusspeed, _T("+ %d"), g_MainCharInfo.m_bPlusSpeed);
	_stprintf( critical, _T("+ %d"), g_MainCharInfo.m_wCritical);

	g_pUIManager->SetString(WINDOW_CHARACTER, character_window_contents_dummy_05, IDS_STR_PWR);
	g_pUIManager->SetString(WINDOW_CHARACTER, character_window_contents_dummy_06, IDS_DEF_PWR);
	g_pUIManager->SetString(WINDOW_CHARACTER, character_window_contents_dummy_07, IDS_AGI_PWR);
	g_pUIManager->SetString(WINDOW_CHARACTER, character_window_contents_dummy_08, IDS_NAVIGATION_SPEED);
	g_pUIManager->SetString(WINDOW_CHARACTER, character_window_contents_dummy_09, IDS_CRITICAL);
	g_pUIManager->SetString(WINDOW_CHARACTER, character_window_contents_dummy_10, IDS_LIFE_PWR);
	g_pUIManager->SetString(WINDOW_CHARACTER, character_window_contents_dummy_11, IDS_INLIFE_PWR);

	g_pUIManager->SetString(WINDOW_CHARACTER, character_window_contents_dummy_005, atk);
	g_pUIManager->SetString(WINDOW_CHARACTER, character_window_contents_dummy_006, def);
	g_pUIManager->SetString(WINDOW_CHARACTER, character_window_contents_dummy_007, atkrat);
	g_pUIManager->SetString(WINDOW_CHARACTER, character_window_contents_dummy_008, plusspeed);
	g_pUIManager->SetString(WINDOW_CHARACTER, character_window_contents_dummy_009, critical);
	g_pUIManager->SetString(WINDOW_CHARACTER, character_window_contents_dummy_010, strHp);
	g_pUIManager->SetString(WINDOW_CHARACTER, character_window_contents_dummy_011, strIp);

	g_pUIManager->SetToolTip(WINDOW_CHARACTER, character_window_button_up_01, 1, IDS_STR_PWR_INC);
	g_pUIManager->SetToolTip(WINDOW_CHARACTER, character_window_button_up_02, 1, IDS_DEF_PWR_INC);
	g_pUIManager->SetToolTip(WINDOW_CHARACTER, character_window_button_up_03, 1, IDS_AGI_PWR_INC);
	g_pUIManager->SetToolTip(WINDOW_CHARACTER, character_window_button_up_04, 1, IDS_LIFE_PWR_INC);	
}

void  XiahGame_Intro::Init_WindowItem()
{
	g_pUIManager->SetPosition(DRG_ITEM_WINDOW, WINDOW_FIRST_XPOS, 0);

	g_pUIManager->SetData(DRG_ITEM_WINDOW, drg_item_window_sack_1_button, CURRENT_INDEX, 2);
	g_pUIManager->SetData(DRG_ITEM_WINDOW, drg_item_window_sack_2_button, CURRENT_INDEX, -1);
	g_pUIManager->SetData(DRG_ITEM_WINDOW, drg_item_window_sack_3_button, CURRENT_INDEX, -1);

	g_pUIManager->SetString(DRG_ITEM_WINDOW, drg_item_window_title_back, IDS_SACK);
	g_pUIManager->SetString(DRG_ITEM_WINDOW, drg_item_window_money_dummy_01, MoneyCommaStr(g_MainCharInfo.m_dwMoney));
}

// 櫢瓿 彀
void  XiahGame_Intro::Init_WindowOutSide()
{
	g_pUIManager->SetPosition(WINDOW_OUTSIDE, WINDOW_FIRST_XPOS, 0);
	
	g_pUIManager->SetString(WINDOW_OUTSIDE, outside_window_title_back, IDS_MUGONG);
	g_pUIManager->SetString(WINDOW_OUTSIDE, outside_window_top_button_01, IDS_OUT_PWR);
	g_pUIManager->SetData(WINDOW_OUTSIDE, outside_window_top_button_01, CURRENT_INDEX, 2);
	g_pUIManager->SetString(WINDOW_OUTSIDE, outside_window_top_button_02, IDS_IN_PWR);
	// 槫枆
	g_pUIManager->SetString(WINDOW_OUTSIDE, outside_window_top_button_03, IDS_FIVEELEMENT);
	// 臧侅劚
	g_pUIManager->SetString(WINDOW_OUTSIDE, outside_window_top_button_04, IDS_SKILL);

	g_pUIManager->SetString(WINDOW_OUTSIDE, outside_window_training_point_name_01, IDS_TP);
	g_pUIManager->SetString(WINDOW_OUTSIDE, outside_window_training_point_name_02, g_MainCharInfo.m_wRemainTp);

	g_pUIManager->SetString(WINDOW_OUTSIDE, outside_attack_mode_dummy, IDS_ATTACK_MODE);

	g_pUIManager->SetString(WINDOW_OUTSIDE, outside_attack_mode_button_01, IDS_ATTACK_MODE_01);
	g_pUIManager->SetString(WINDOW_OUTSIDE, outside_attack_mode_button_02, IDS_ATTACK_MODE_02);
	g_pUIManager->SetString(WINDOW_OUTSIDE, outside_attack_mode_button_03, IDS_ATTACK_MODE_03);
	g_pUIManager->SetData(WINDOW_OUTSIDE, outside_attack_mode_button_03, CURRENT_INDEX, 2);
	
	for(int i=1; i < 13; ++i)
	{
		sArrayData* pData = NULL;

		switch( g_MainCharInfo.m_bCharType)
		{
		case 1:		// 瓴榿
			 pData = XiahArrayIndex::g_MugongIndex_OutGum.GetData(i);
			break;
		case 2:		// 棸瀾
			pData = XiahArrayIndex::g_MugongIndex_OutYun.GetData(i);
			break;
		case 3:		// 氍错埇
			pData = XiahArrayIndex::g_MugongIndex_OutMutu.GetData(i);
			break;
		case 4:		// 暭彀
			pData = XiahArrayIndex::g_MugongIndex_OutYaCha.GetData(i);
			break;
		}

		if( pData)
		{
			int nMugongID = pData->GetInt( 1);

			sArrayData* pMugongData = XiahArrayIndex::g_MugongTemplate.GetData( nMugongID);

			if( pMugongData)
			{
				// 澊毽
				sString szMugongName = pMugongData->GetString( 1);

				g_pUIManager->SetString(WINDOW_OUTSIDE, outside_window_negong_name_dummy_01 + i -1, (LPCTSTR)szMugongName);

				// 爤氩
				g_pUIManager->SetString(WINDOW_OUTSIDE, outside_window_mugong_skill_01 + i -1, 0);

				// 晞澊旖 澊氙胳
				int nResID = pMugongData->GetInt(1);
				g_pUIManager->SetData(WINDOW_OUTSIDE, outside_window_mugong_dummy_01 + i - 1, TEXTURE, nResID + 2);

				// 埓寔
				if( g_MainCharInfo.m_pMugong)
					g_MainCharInfo.m_pMugong->SetMugongToolTip( nMugongID, WINDOW_OUTSIDE, outside_window_mugong_dummy_01 + i - 1);
			}
		}
		else
		{
			DBG_LogFile( _T("XiahGame_Intro::Init_WindowOutSide 嫟尐"));
		}
	}
}

// 偞瓿 彀
void  XiahGame_Intro::Init_WindowInSide()
{
	g_pUIManager->SetPosition(WINDOW_INSIDE, WINDOW_FIRST_XPOS, 0);

	g_pUIManager->SetString(WINDOW_INSIDE, inside_window_title_back, IDS_MUGONG);
	g_pUIManager->SetString(WINDOW_INSIDE, inside_window_top_button_01, IDS_OUT_PWR);
	g_pUIManager->SetString(WINDOW_INSIDE, inside_window_top_button_02, IDS_IN_PWR);
	// 槫枆
	g_pUIManager->SetString(WINDOW_INSIDE, inside_window_top_button_03, IDS_FIVEELEMENT);
	// 臧侅劚
	g_pUIManager->SetString(WINDOW_INSIDE, inside_window_top_button_04, IDS_SKILL);	

	g_pUIManager->SetData(WINDOW_INSIDE, inside_window_top_button_02, CURRENT_INDEX, 2);
	g_pUIManager->SetString(WINDOW_INSIDE, inside_window_training_point_dummy_01, IDS_TP);
	g_pUIManager->SetString(WINDOW_INSIDE, inside_window_training_point_dummy_02, g_MainCharInfo.m_wRemainTp);

	g_pUIManager->SetString(WINDOW_INSIDE, inside_window_box_title_dummy, IDS_2TH_REBIRTH_MUGONG);//HO_0709_07 : 歆勱皝劚 偞瓿 攧爤瀯 : 潯劚 嫚瓿 憸嫓

	for( int i=1; i < 10; ++i)
	{
		sArrayData* pData = NULL;

		switch( g_MainCharInfo.m_bCharType)
		{
		case 1:
			pData = XiahArrayIndex::g_MugongIndex_InGum.GetData( i);
			break;
		case 2:
			pData = XiahArrayIndex::g_MugongIndex_InYun.GetData( i);
			break;
		case 3:
			pData = XiahArrayIndex::g_MugongIndex_InMutu.GetData( i);
			break;
		case 4:
			pData = XiahArrayIndex::g_MugongIndex_InYaCha.GetData( i);
			break;
		}

		if( pData)
		{
			int nMugongID = pData->GetInt( 1);

			sArrayData* pMugongData = XiahArrayIndex::g_MugongTemplate.GetData( nMugongID);

			if( pMugongData)
			{
				// 澊毽
				sString szMugongName = pMugongData->GetString( 1);

				g_pUIManager->SetString(WINDOW_INSIDE, inside_window_negong_name_dummy_01 + i -1, (LPCTSTR)szMugongName);

				// 爤氩
				g_pUIManager->SetString(WINDOW_INSIDE, inside_window_negong_skill_01 + i -1, 0);

				// 埓寔
				if( g_MainCharInfo.m_pMugong)
					g_MainCharInfo.m_pMugong->SetMugongToolTip( nMugongID, WINDOW_INSIDE, inside_window_negong_01 + i - 1);
				// 晞澊旖 澊氙胳
				if( i < 8)
				{
					g_pUIManager->SetData(WINDOW_INSIDE, inside_window_negong_01 + i - 1, CURRENT_INDEX, 1);
				}
				else
				{
					int nResID = pMugongData->GetInt(1);
					g_pUIManager->SetData(WINDOW_INSIDE, inside_window_negong_01 + i - 1, TEXTURE, nResID+1);
				}
			}
			else
			{
				DBG_LogFile( _T("XiahGame_Intro::Init_WindowInSide 嫟尐"));
			}
		}
	}

	//HT_0711 : 歆勱皝劚 偞瓿 攧爤瀯 : 潯劚 嫚瓿 憸嫓
	for(i=0; i<2; i++)
	{
		sArrayData* pMugongData = XiahArrayIndex::g_MugongTemplate.GetData( 199 + i );

		if( pMugongData)
		{
			// 澊毽
			sString szMugongName = pMugongData->GetString( 1);

			g_pUIManager->SetString(WINDOW_INSIDE, inside_window_negong_name_dummy_10 + i, (LPCTSTR)szMugongName);

			// 爤氩
			g_pUIManager->SetString(WINDOW_INSIDE, inside_window_negong_skill_10 + i, 0);

			// 埓寔
			if( g_MainCharInfo.m_pMugong)
				g_MainCharInfo.m_pMugong->SetMugongToolTip( 199 + i, WINDOW_INSIDE, inside_window_mugong_dummy_01 + i);
			
			// 晞澊旖 澊氙胳
			int nResID = pMugongData->GetInt(1);
			g_pUIManager->SetData(WINDOW_INSIDE, inside_window_mugong_dummy_01 + i, TEXTURE, nResID);
		}
		else
		{
			DBG_LogFile( _T("XiahGame_Intro::Init_WindowInSide 嫟尐"));
		}
	}
}

void XiahGame_Intro::Init_WindowDan()
{
	g_pUIManager->SetPosition(WINDOW_DAN, WINDOW_FIRST_XPOS, 0);

	g_pUIManager->SetString(WINDOW_DAN, dan_window_title_back, IDS_RELATION);
	g_pUIManager->SetString(WINDOW_DAN, dan_window_3button_01, IDS_DAN);
	g_pUIManager->SetData(WINDOW_DAN, dan_window_3button_01, CURRENT_INDEX, 2);
	g_pUIManager->SetString(WINDOW_DAN, dan_window_3button_02, IDS_SHIP);
	g_pUIManager->SetString(WINDOW_DAN, dan_window_3button_03, IDS_CLAN);

	g_pUIManager->SetString(WINDOW_DAN, dan_window_list_dummy_01, IDS_NAME);
	g_pUIManager->SetString(WINDOW_DAN, dan_window_list_dummy_02, IDS_LEVEL);
	g_pUIManager->SetString(WINDOW_DAN, dan_window_list_dummy_03, IDS_LOCATION);

	g_pUIManager->SetString(WINDOW_DAN, dan_window_2button_01, IDS_JEMYUNG);
	g_pUIManager->SetString(WINDOW_DAN, dan_window_2button_02, IDS_TALTE);
	g_pUIManager->SetString(WINDOW_DAN, dan_window_1button, IDS_TALTE);
}

void XiahGame_Intro::Init_WindowClanFound()
{
	g_pUIManager->SetPosition(WINDOW_MUNPA_FOUND, WINDOW_FIRST_XPOS, 0);

	g_pUIManager->SetData(WINDOW_MUNPA_FOUND, found_window_name_edit, MAXSTRING, 10);

	g_pUIManager->SetString(WINDOW_MUNPA_FOUND, found_window_title_back, IDS_RELATION);
	g_pUIManager->SetString(WINDOW_MUNPA_FOUND, found_window_3button_01, IDS_DAN);
	g_pUIManager->SetString(WINDOW_MUNPA_FOUND, found_window_3button_02, IDS_SHIP);
	g_pUIManager->SetString(WINDOW_MUNPA_FOUND, found_window_3button_03, IDS_CLAN);
	g_pUIManager->SetData(WINDOW_MUNPA_FOUND, found_window_3button_03, CURRENT_INDEX, 2);

	g_pUIManager->SetString(WINDOW_MUNPA_FOUND, found_window_contents_dummy_01, IDS_CLAN_NAME);
	g_pUIManager->SetString(WINDOW_MUNPA_FOUND, found_window_contents_dummy_00, IDS_NO_CLAN);

	g_pUIManager->SetString(WINDOW_MUNPA_FOUND, found_window_center_title_dummy, IDS_CLAN_FOUND);

#ifdef _CHINA_
	pFrame->GetControl( found_window_bottom_dummy_01)->SetString( "锟届爲斐旐儼帢榻§劕 锛30劕铳锟|铯項轨赴妗儛渼 锛10劕铳锟|澃煖搿ょ棸锛100.000.膦冿锟|澃煖對臁颁护 锛10");
#else
	g_pUIManager->SetString(WINDOW_MUNPA_FOUND, found_window_bottom_dummy_01, "氍疙寣 劋毽 牅暅 臧戩瀽 : 30 臧戩瀽 澊儊|殧甑 氇呾劚摫旮 : 10 摫旮 澊儊|唽氇 牍勳毄 : 100,000爠|唽氇 垬牗旃 : 10");
#endif

	g_pUIManager->SetString(WINDOW_MUNPA_FOUND, found_window_bottom_dummy_03, IDS_CLAN_EXPLAIN4, 1);

	g_pUIManager->SetString(WINDOW_MUNPA_FOUND, found_window_name_dummy, IDS_CLAN_NAME);
	g_pUIManager->SetString(WINDOW_MUNPA_FOUND, found_window_found_button, IDS_FOUND);
}

void XiahGame_Intro::Init_WindowClan()
{
	g_pUIManager->SetPosition(WINDOW_MUNPA, WINDOW_FIRST_XPOS, 0);

	g_pUIManager->SetString(WINDOW_MUNPA, munpa_window_title_back, IDS_RELATION);
	g_pUIManager->SetString(WINDOW_MUNPA, munpa_window_3button_01, IDS_DAN);
	g_pUIManager->SetString(WINDOW_MUNPA, munpa_window_3button_02, IDS_SHIP);
	g_pUIManager->SetString(WINDOW_MUNPA, munpa_window_3button_03, IDS_CLAN);
	g_pUIManager->SetData(WINDOW_MUNPA, munpa_window_3button_03, CURRENT_INDEX, 2);

	g_pUIManager->SetString(WINDOW_MUNPA, found_window_contents_dummy_01, IDS_CLAN_NAME);
	g_pUIManager->SetString(WINDOW_MUNPA, found_window_contents_dummy_02, IDS_NICK);
	g_pUIManager->SetString(WINDOW_MUNPA, found_window_contents_dummy_03, IDS_JOB);
	g_pUIManager->SetString(WINDOW_MUNPA, found_window_contents_dummy_04, IDS_M_BBS_FAME);
	g_pUIManager->SetString(WINDOW_MUNPA, found_window_contents_dummy_05, IDS_TOTAL);

	g_pUIManager->SetString(WINDOW_MUNPA, munpa_window_center_title_dummy, IDS_CLANLIST);
	g_pUIManager->SetString(WINDOW_MUNPA, munpa_window_bottom_dummy_1, IDS_NAME);
	g_pUIManager->SetString(WINDOW_MUNPA, munpa_window_bottom_dummy_2, IDS_NICK);
	g_pUIManager->SetString(WINDOW_MUNPA, munpa_window_bottom_dummy_3, IDS_JOB);
	g_pUIManager->SetString(WINDOW_MUNPA, munpa_window_bottom_dummy_4, IDS_STATUS);

	g_pUIManager->SetString(WINDOW_MUNPA, munpa_window_button_01, IDS_M_NOTICE_RECORD);
	g_pUIManager->SetString(WINDOW_MUNPA, munpa_window_button_02, IDS_GIVE_NICK);
	g_pUIManager->SetString(WINDOW_MUNPA, munpa_window_button_03, IDS_TALTE);
	g_pUIManager->SetString(WINDOW_MUNPA, munpa_window_button_04, IDS_CLOSE_CLAN);
}








void  XiahGame_Intro::Init_WindowPcTrade()
// PC臧 瓯半灅 彀
{
	g_pUIManager->SetPosition(WINDOW_PC_TRADE, WINDOW_SECOND_XPOS, 0);

	g_pUIManager->SetString(WINDOW_PC_TRADE, pc_trade_window_title_back, IDS_DEAL);
	g_pUIManager->SetString(WINDOW_PC_TRADE, pc_trade_window_sub_dummy_01, IDS_OTHER_SACK);
	g_pUIManager->SetString(WINDOW_PC_TRADE, pc_trade_window_sub_dummy_02, IDS_MY_SACK);

	g_pUIManager->SetString(WINDOW_PC_TRADE, pc_trade_window_button_01, IDS_OK);
	g_pUIManager->SetString(WINDOW_PC_TRADE, pc_trade_window_button_02, IDS_CANCEL);
}

void  XiahGame_Intro::Init_WindowNpcTrade()
// NPC 瓯半灅 彀
{
	g_pUIManager->SetPosition(WINDOW_NPC_TRADE, WINDOW_SECOND_XPOS, 0);

	g_pUIManager->SetString( WINDOW_NPC_TRADE, npc_trade_window_title_back, IDS_NPC_DEAL);
	g_pUIManager->SetString( WINDOW_NPC_TRADE, npc_trade_window_button, IDS_CANCEL);
}

void XiahGame_Intro::Init_WindowNpcTradeTab()
{
	g_pUIManager->SetPosition(TAP_NPC_TRADE_2, WINDOW_SECOND_XPOS-40, 30);
	g_pUIManager->SetData(TAP_NPC_TRADE_2, npc_trade_2_tap_button_01, CURRENT_INDEX, 2);


	g_pUIManager->SetPosition(TAP_NPC_TRADE_3, WINDOW_SECOND_XPOS-40, 30);
	g_pUIManager->SetData(TAP_NPC_TRADE_3, npc_trade_3_tap_button_01, CURRENT_INDEX, 2);

	g_pUIManager->SetPosition(TAP_NPC_TRADE_4, WINDOW_SECOND_XPOS-40, 30);
	g_pUIManager->SetData(TAP_NPC_TRADE_4, npc_trade_4_tap_button_01, CURRENT_INDEX, 2);
}

/**
 * 臧滌“
 */
void XiahGame_Intro::Init_WindowConvert()
{
	g_pUIManager->SetPosition(WINDOW_CONVERT, WINDOW_SECOND_XPOS, 0);

	g_pUIManager->SetString(WINDOW_CONVERT, convert_window_title_dummy, IDS_ITEM_CONVERT);
	g_pUIManager->SetString(WINDOW_CONVERT, convert_window_button_01, IDS_CONVERT);
	g_pUIManager->SetString(WINDOW_CONVERT, convert_window_button_02, IDS_CANCEL);
}

void XiahGame_Intro::Init_WindowTaming()
{
	g_pUIManager->SetPosition(WINDOW_NEW_TAMING, WINDOW_SECOND_XPOS, 0);

	g_pUIManager->SetString(WINDOW_NEW_TAMING, taming_window_title_dummy, IDS_PET);
	g_pUIManager->SetString(WINDOW_NEW_TAMING, taming_window_name_dummy, IDS_NAME);
	g_pUIManager->SetString(WINDOW_NEW_TAMING, taming_window_button, IDS_CHANGE);
	g_pUIManager->SetString(WINDOW_NEW_TAMING, taming_window_contents_1_dummy_01, IDS_LEVEL);
	g_pUIManager->SetString(WINDOW_NEW_TAMING, taming_window_contents_1_dummy_02, IDS_LIFE_PWR);
	g_pUIManager->SetString(WINDOW_NEW_TAMING, taming_window_contents_1_dummy_03, IDS_EXP);
	g_pUIManager->SetString(WINDOW_NEW_TAMING, taming_window_contents_1_dummy_04, IDS_FIELD_TEND);
	g_pUIManager->SetString(WINDOW_NEW_TAMING, taming_window_contents_1_dummy_05, IDS_STR_PWR);
	g_pUIManager->SetString(WINDOW_NEW_TAMING, taming_window_contents_1_dummy_06, IDS_DEF_PWR);
	g_pUIManager->SetString(WINDOW_NEW_TAMING, taming_window_contents_1_dummy_07, IDS_AGI_PWR);
	g_pUIManager->SetString(WINDOW_NEW_TAMING, taming_window_contents_1_dummy_08, IDS_RECOVER_SPEED);

	g_pUIManager->SetData(WINDOW_NEW_TAMING, taming_window_name_edit, MAXSTRING, 10);
}

void XiahGame_Intro::Init_WindowVolume()
{
	g_pUIManager->SetPosition(WINDOW_VOLUME, WINDOW_VOLUME_XPOS, WINDOW_VOLUME_YPOS);

	g_pUIManager->SetData(WINDOW_VOLUME, volume_window_edit, MAXSTRING, 12);
	g_pUIManager->SetData(WINDOW_VOLUME, volume_window_edit, EDIT_INPUT_MODE, NUMERAL);

	g_pUIManager->SetString(WINDOW_VOLUME, volume_window_edit, 1);

	g_pUIManager->SetString(WINDOW_VOLUME, volume_window_title_dummy, IDS_AMOUNT);
	g_pUIManager->SetString(WINDOW_VOLUME, volume_window_title_button_01, IDS_OK);
	g_pUIManager->SetString(WINDOW_VOLUME, volume_window_title_button_02, IDS_CANCEL);
}

void XiahGame_Intro::Init_WindowConnectionInfo()
{
	g_pUIManager->SetString(WINDOW_CONNECTION_INFO, info_window_button, IDS_OK);
	g_pUIManager->SetString(WINDOW_CONNECTION_INFO, info_window_button_01, IDS_GIVE_RANK);
	g_pUIManager->SetString(WINDOW_CONNECTION_INFO, info_window_button_02, IDS_CALL);
}

void XiahGame_Intro::Init_WindowNameConfer()
{
	g_pUIManager->SetPosition(WINDOW_NAME_CONFER, 545, 400);

	g_pUIManager->SetString(WINDOW_NAME_CONFER, name_wiandow_title_dummy, IDS_GIVE_NICK);
	g_pUIManager->SetString(WINDOW_NAME_CONFER, name_wiandow_contents_dummy_01, IDS_NAME_OF_NICK);
	g_pUIManager->SetString(WINDOW_NAME_CONFER, name_window_button_01, IDS_OK);
	g_pUIManager->SetString(WINDOW_NAME_CONFER, name_window_button_02, IDS_CANCEL);

	g_pUIManager->SetData(WINDOW_NAME_CONFER, name_window_edit, MAXSTRING, 10);
}

void XiahGame_Intro::Init_NameChannel()
{
	g_pUIManager->SetString(NAME_CHANNEL, name_channel_button_01,IDS_MUNJU);
	g_pUIManager->SetString(NAME_CHANNEL, name_channel_button_02,IDS_BUMUNJU);
	g_pUIManager->SetString(NAME_CHANNEL, name_channel_button_03,IDS_JANGRO);
	g_pUIManager->SetString(NAME_CHANNEL, name_channel_button_04,IDS_HOBUB);
	g_pUIManager->SetString(NAME_CHANNEL, name_channel_button_05,IDS_DANGJU);
}

void XiahGame_Intro::Init_WindowQuest()
{
	g_pUIManager->SetPosition(WINDOW_QUEST_01, WINDOW_FIRST_XPOS, 0);

	g_pUIManager->SetString(WINDOW_QUEST_01, quest_window_01_title_back, IDS_GIYUN);

	// [3/25/2004]
	g_pUIManager->SetString(WINDOW_QUEST_01, quest_window_01_start_button, IDS_START);
	g_pUIManager->SetString(WINDOW_QUEST_01, quest_window_01_stop_button, IDS_QUEST_P);
	g_pUIManager->SetString(WINDOW_QUEST_01, quest_window_01_delete_button, IDS_DELETE);

	g_pUIManager->SetString(WINDOW_QUEST_01, quest_window_01_button_05, IDS_CONTENT);
	g_pUIManager->SetData(WINDOW_QUEST_01, quest_window_01_button_05, CURRENT_INDEX, 2);
	g_pUIManager->SetString(WINDOW_QUEST_01, quest_window_01_button_06, IDS_COMPLETE_CONDITION);
	g_pUIManager->SetString(WINDOW_QUEST_01, quest_window_01_button_07, IDS_REWARD);

	g_pUIManager->Hide(WINDOW_QUEST_01, quest_window_01_listbox);

	g_pUIManager->SetData(WINDOW_QUEST_01, quest_window_01_scrollbar1, TYPE, SCROLL_VERTICAL_2);
	g_pUIManager->SetData(WINDOW_QUEST_01, quest_window_01_scrollbar1, SCROLL_TOTAL, 0);
	g_pUIManager->SetData(WINDOW_QUEST_01, quest_window_01_scrollbar1, SCROLL_MAX, 9);
	g_pUIManager->SetData(WINDOW_QUEST_01, quest_window_01_scrollbar1, SCROLL_MOVE, 0);

	g_pUIManager->SetData(WINDOW_QUEST_01, quest_window_01_scrollbar2, TYPE, SCROLL_VERTICAL_2);
	g_pUIManager->SetData(WINDOW_QUEST_01, quest_window_01_scrollbar2, SCROLL_TOTAL, 0);
	g_pUIManager->SetData(WINDOW_QUEST_01, quest_window_01_scrollbar2, SCROLL_MAX, 9);
	g_pUIManager->SetData(WINDOW_QUEST_01, quest_window_01_scrollbar2, SCROLL_MOVE, 0);

	g_pUIManager->SetData(WINDOW_QUEST_01, quest_window_01_scrollbar1, VALUE1, quest_window_01_scrollbar1);	// Send ID
	g_pUIManager->SetData(WINDOW_QUEST_01, quest_window_01_scrollbar2, VALUE1, quest_window_01_scrollbar2);	// Send ID
}

void XiahGame_Intro::Init_WindowTamingItem()
{
	g_pUIManager->SetPosition(WINDOW_TAMING_ITEM, WINDOW_SECOND_XPOS, 0);
}

/**
 * 樀厴
 */
void XiahGame_Intro::Init_WindowOption()
{
	g_pUIManager->SetPosition(WINDOW_OPTION_01, WINDOW_FIRST_XPOS, 0);

	g_pUIManager->SetString(WINDOW_OPTION_01, option_window_1_title_dummy, IDS_OPTION1);
	g_pUIManager->SetString(WINDOW_OPTION_01, option_window_1_top_button_01, IDS_GAME);
	g_pUIManager->SetString(WINDOW_OPTION_01, option_window_1_top_button_02, IDS_ENVIRONMENT);
	g_pUIManager->SetString(WINDOW_OPTION_01, option_window_1_top_button_03, IDS_DEAL);
	g_pUIManager->SetString(WINDOW_OPTION_01, option_window_1_bottom_button_01, IDS_OK);
	g_pUIManager->SetString(WINDOW_OPTION_01, option_window_1_bottom_button_02, IDS_CANCEL);

	g_pUIManager->SetString(WINDOW_OPTION_01, option_window_1_select_dummy_01, IDS_WHISPER_OK);
	g_pUIManager->SetString(WINDOW_OPTION_01, option_window_1_select_dummy_02, IDS_RELATION_OK);
	g_pUIManager->SetString(WINDOW_OPTION_01, option_window_1_select_dummy_03, IDS_TRADE_OK);
	g_pUIManager->SetString(WINDOW_OPTION_01, option_window_1_select_dummy_04, IDS_CHAT_OK);
	g_pUIManager->SetString(WINDOW_OPTION_01, option_window_1_select_dummy_05, IDS_CHARNAME_OK);
	g_pUIManager->SetString(WINDOW_OPTION_01, option_window_1_select_dummy_06, IDS_MOBNAME_OK);
	g_pUIManager->SetString(WINDOW_OPTION_01, option_window_1_select_dummy_07, IDS_ITEMDROP); //HO_0816_07 晞澊厹 摐瀺嫓 殔晬

	g_pUIManager->SetData(WINDOW_OPTION_01, option_window_1_select_01, CURRENT_INDEX, 1);
	g_pUIManager->SetData(WINDOW_OPTION_01, option_window_1_select_02, CURRENT_INDEX, 1);
	g_pUIManager->SetData(WINDOW_OPTION_01, option_window_1_select_03, CURRENT_INDEX, 1);
	g_pUIManager->SetData(WINDOW_OPTION_01, option_window_1_select_04, CURRENT_INDEX, 1);
	g_pUIManager->SetData(WINDOW_OPTION_01, option_window_1_select_05, CURRENT_INDEX, 1);
	g_pUIManager->SetData(WINDOW_OPTION_01, option_window_1_select_06, CURRENT_INDEX, 1);
	g_pUIManager->SetData(WINDOW_OPTION_01, option_window_1_select_07, CURRENT_INDEX, 1); //HO_0816_07 晞澊厹 摐瀺嫓 殔晬

	/////////////////////////////////////////////////////////////////////////////////////////////////////
	g_pUIManager->SetPosition(WINDOW_OPTION_02, WINDOW_FIRST_XPOS, 0);

	g_pUIManager->SetString(WINDOW_OPTION_02, option_window_2_title_back, IDS_OPTION1);
	g_pUIManager->SetString(WINDOW_OPTION_02, option_window_2_top_button_01, IDS_GAME);
	g_pUIManager->SetString(WINDOW_OPTION_02, option_window_2_top_button_02, IDS_ENVIRONMENT);
	g_pUIManager->SetString(WINDOW_OPTION_02, option_window_2_top_button_03, IDS_DEAL);
	g_pUIManager->SetString(WINDOW_OPTION_02, option_window_2_bottom_button_01, IDS_OK);
	g_pUIManager->SetString(WINDOW_OPTION_02, option_window_2_bottom_button_02, IDS_CANCEL);

	g_pUIManager->SetString(WINDOW_OPTION_02, option_window_2_scroll_dummy_01, IDS_VISUAL_DISTANCE);
	g_pUIManager->SetString(WINDOW_OPTION_02, option_window_2_scroll_dummy_02, IDS_POL_DETAIL);
	g_pUIManager->SetString(WINDOW_OPTION_02, option_window_2_scroll_dummy_05, IDS_BACK_MUSIC);
	g_pUIManager->SetString(WINDOW_OPTION_02, option_window_2_scroll_dummy_06, IDS_EFFE_MUSIC);

	g_pUIManager->SetData(WINDOW_OPTION_02, option_window_2_scroll_01, TYPE, SCROLL_HORIZONTAL_2);
	g_pUIManager->SetData(WINDOW_OPTION_02, option_window_2_scroll_01, SCROLL_TOTAL, 10);
	g_pUIManager->SetData(WINDOW_OPTION_02, option_window_2_scroll_01, SCROLL_MAX, 10);
	g_pUIManager->SetData(WINDOW_OPTION_02, option_window_2_scroll_01, SCROLL_MOVE, 0);
	g_pUIManager->SetData(WINDOW_OPTION_02, option_window_2_scroll_01, VALUE1, option_window_2_scroll_01);	// Send ID

	g_pUIManager->SetData(WINDOW_OPTION_02, option_window_2_scroll_02, TYPE, SCROLL_HORIZONTAL_2);
	g_pUIManager->SetData(WINDOW_OPTION_02, option_window_2_scroll_02, SCROLL_TOTAL, 10);
	g_pUIManager->SetData(WINDOW_OPTION_02, option_window_2_scroll_02, SCROLL_MAX, 10);
	g_pUIManager->SetData(WINDOW_OPTION_02, option_window_2_scroll_02, SCROLL_MOVE, 0);	
	g_pUIManager->SetData(WINDOW_OPTION_02, option_window_2_scroll_02, VALUE1, option_window_2_scroll_02);	// Send ID
	
	g_pUIManager->SetData(WINDOW_OPTION_02, option_window_2_scroll_03, TYPE, SCROLL_HORIZONTAL_2);
	g_pUIManager->SetData(WINDOW_OPTION_02, option_window_2_scroll_03, SCROLL_TOTAL, 10);
	g_pUIManager->SetData(WINDOW_OPTION_02, option_window_2_scroll_03, SCROLL_MAX, 10);
	g_pUIManager->SetData(WINDOW_OPTION_02, option_window_2_scroll_03, SCROLL_MOVE, 0);	
	g_pUIManager->SetData(WINDOW_OPTION_02, option_window_2_scroll_03, VALUE1, option_window_2_scroll_03);	// Send ID

	g_pUIManager->SetData(WINDOW_OPTION_02, option_window_2_scroll_04, TYPE, SCROLL_HORIZONTAL_2);
	g_pUIManager->SetData(WINDOW_OPTION_02, option_window_2_scroll_04, SCROLL_TOTAL, 10);
	g_pUIManager->SetData(WINDOW_OPTION_02, option_window_2_scroll_04, SCROLL_MAX, 10);
	g_pUIManager->SetData(WINDOW_OPTION_02, option_window_2_scroll_04, SCROLL_MOVE, 0);	
	g_pUIManager->SetData(WINDOW_OPTION_02, option_window_2_scroll_04, VALUE1, option_window_2_scroll_04);	// Send ID

	/////////////////////////////////////////////////////////////////////////////////////////////////////
	g_pUIManager->SetPosition(WINDOW_OPTION_03, WINDOW_FIRST_XPOS, 0);

	g_pUIManager->SetString(WINDOW_OPTION_03, option_window_3_title_dummy, IDS_OPTION1);
	g_pUIManager->SetString(WINDOW_OPTION_03, option_window_3_top_button_01, IDS_GAME);
	g_pUIManager->SetString(WINDOW_OPTION_03, option_window_3_top_button_02, IDS_ENVIRONMENT);
	g_pUIManager->SetString(WINDOW_OPTION_03, option_window_3_top_button_03, IDS_DEAL);
	g_pUIManager->SetString(WINDOW_OPTION_03, option_window_3_bottom_button_01, IDS_OK);
	g_pUIManager->SetString(WINDOW_OPTION_03, option_window_3_bottom_button_02, IDS_CANCEL);

	g_pUIManager->SetString(WINDOW_OPTION_03, option_window_3_money_dummy3, IDS_LIMITBUY);
	g_pUIManager->SetString(WINDOW_OPTION_03, window_option_3_money_dummy_02, IDS_LIMITSELL);
	g_pUIManager->SetString(WINDOW_OPTION_03, window_option_3_sell_dummy_03, IDS_LIMIT_RL);
	g_pUIManager->SetString(WINDOW_OPTION_03, window_option_3_sell_dummy_04, IDS_LIMIT_SL);
	
	g_pUIManager->SetData(WINDOW_OPTION_03, option_window_3_money_dummy2, TYPE, STATIC);
	g_pUIManager->SetData(WINDOW_OPTION_03, option_window_3_money_dummy2, TEXTURE, 50000319);
}

void XiahGame_Intro::Init_WindowClose()
{
	TCHAR strServerState[64] = {0,};

	sprintf(strServerState,"%s : %s",IDS_SERVERNAME,g_ServerName.data());

	g_pUIManager->SetPosition(WINDOW_CLOSE, 420, 250);

	g_pUIManager->SetString(WINDOW_CLOSE, close_window_title_dummy, IDS_GAME_OVER);
	g_pUIManager->SetString(WINDOW_CLOSE, close_window_state_dummy, strServerState);
	g_pUIManager->SetString(WINDOW_CLOSE, close_window_button_01, IDS_RECONNECT);
	g_pUIManager->SetString(WINDOW_CLOSE, close_window_button_02, IDS_CANCEL);
	g_pUIManager->SetString(WINDOW_CLOSE, close_window_button_03, IDS_GAME_OVER);
}

/**
 *
 */
void XiahGame_Intro::Init_WindowPcStore()
{
	g_pUIManager->SetPosition(WINDOW_PC_STORE, WINDOW_SECOND_XPOS, 0);
	
	g_pUIManager->SetData(WINDOW_PC_STORE, pc_store_passage_edit_01, MAXSTRING, 19);	// 吀爯氇
	g_pUIManager->SetData(WINDOW_PC_STORE, pc_store_passage_edit_02, MAXSTRING, 39);	// 樃臧濍戈

	g_pUIManager->SetString(WINDOW_PC_STORE, pc_store_title_dummy, IDS_PT_SET_TITLE);
	g_pUIManager->SetString(WINDOW_PC_STORE, pc_store_passage_dummy_01, IDS_PT_SET_NAME);
	g_pUIManager->SetString(WINDOW_PC_STORE, pc_store_passage_dummy_02, IDS_PT_SET_DES);
	g_pUIManager->SetString(WINDOW_PC_STORE, pc_store_earnings_dummy_01, IDS_PT_SET_TOV);
	g_pUIManager->SetString(WINDOW_PC_STORE, pc_store_earnings_dummy_02, IDS_PT_SET_TOTAL);

	g_pUIManager->SetString(WINDOW_PC_STORE, pc_store_button_01, IDS_CHANGE);
	g_pUIManager->SetString(WINDOW_PC_STORE, pc_store_button_02, IDS_PT_SET_START);
	g_pUIManager->SetString(WINDOW_PC_STORE, pc_store_button_03, IDS_PT_SET_COLLECT);
}

// 臧瓴 瀰牓彀
void XiahGame_Intro::Init_WindowMoney()
{
	g_pUIManager->SetPosition(WINDOW_MONEY, WINDOW_VOLUME_XPOS, WINDOW_VOLUME_YPOS);

	g_pUIManager->SetData(WINDOW_MONEY, money_window_edit, MAXSTRING, 9);
	g_pUIManager->SetData(WINDOW_MONEY, money_window_edit, EDIT_INPUT_MODE, NUMERAL);
	
	g_pUIManager->SetString(WINDOW_MONEY, money_window_edit, 0);

	g_pUIManager->SetString(WINDOW_MONEY, money_window_dummy_01, IDS_WINDOW_MONEY);
	g_pUIManager->SetString(WINDOW_MONEY, money_window_dummy_02, IDS_PT_SET_SELL_MONEY);
	g_pUIManager->SetString(WINDOW_MONEY, money_window_button_01, IDS_OK);
	g_pUIManager->SetString(WINDOW_MONEY, money_window_button_02, IDS_CANCEL);
}

// [3/19/2004]
void XiahGame_Intro::Init_WindowGakMessage()
{
	g_pUIManager->SetPosition(GAK_MESSAGE_WINDOW, WINDOW_VOLUME_XPOS-100, WINDOW_VOLUME_YPOS);

	g_pUIManager->SetData(GAK_MESSAGE_WINDOW, gak_message_edit, MAXSTRING, 50);
	g_pUIManager->SetString(GAK_MESSAGE_WINDOW, gak_title_dummy, IDS_GAK_TITLE_S);
	g_pUIManager->SetString(GAK_MESSAGE_WINDOW, gak_button_01, IDS_OK);
	g_pUIManager->SetString(GAK_MESSAGE_WINDOW, gak_button_02, IDS_CANCEL);
}

void XiahGame_Intro::Init_WindowDongsin()
{
	g_pUIManager->SetPosition(WINDOW_DONGSIN, 420, 250);

	g_pUIManager->SetString(WINDOW_DONGSIN, dongsin_window_title_dummy, IDS_DONGSIN_TITLE);
	g_pUIManager->SetString(WINDOW_DONGSIN, dongsin_window_button_01, IDS_DONGSIN_D_1);
	g_pUIManager->SetString(WINDOW_DONGSIN, dongsin_window_button_02, IDS_DONGSIN_D_2);
}

/**
 * 嫧 牍勲
 */
void XiahGame_Intro::Init_WindowDanWar()
{
	g_pUIManager->SetPosition(WINDOW_DAN_WAR, 400, 300);

	g_pUIManager->SetData(WINDOW_DAN_WAR, window_dan_war_edit, MAXSTRING, 9);
	g_pUIManager->SetData(WINDOW_DAN_WAR, window_dan_war_edit, EDIT_INPUT_MODE, NUMERAL);

	g_pUIManager->SetString(WINDOW_DAN_WAR, window_dan_war_dummy_01, IDS_DANWAR);
	g_pUIManager->SetString(WINDOW_DAN_WAR, window_dan_war_dummy_02, IDS_BET_MONEY);

	g_pUIManager->SetString(WINDOW_DAN_WAR, window_dan_war_button_01, IDS_OK);
	g_pUIManager->SetString(WINDOW_DAN_WAR, window_dan_war_button_02, IDS_CANCEL);
}

/**
 * 爠偔 澑劙帢澊姢
 */
void XiahGame_Intro::Init_WindowPurse()
{
	g_pUIManager->SetPosition(WINDOW_PURSE, 400, 300);

	g_pUIManager->SetData(WINDOW_PURSE, window_purse_edit, MAXSTRING, 9);
	g_pUIManager->SetData(WINDOW_PURSE, window_purse_edit, EDIT_INPUT_MODE, NUMERAL);

	g_pUIManager->SetString(WINDOW_PURSE, window_purse_button_01, IDS_OK);
	g_pUIManager->SetString(WINDOW_PURSE, window_purse_button_02, IDS_CANCEL);
}


/**
 * 氍疙寣 瓴岇嫓寪 槃櫓
 */
void XiahGame_Intro::Init_WindowMunpaBBSTop()
{
	g_pUIManager->SetPosition(WINDOW_MUNPA_BBS_TOP, WINDOW_FIRST_XPOS-2, 0);

	g_pUIManager->SetString(WINDOW_MUNPA_BBS_TOP, munpa_bbs_top_title_dummy, IDS_M_BBS_1);

	g_pUIManager->SetString(WINDOW_MUNPA_BBS_TOP, munpa_bbs_top_dummy_01, IDS_CLANNAME, 5);
	g_pUIManager->SetString(WINDOW_MUNPA_BBS_TOP, munpa_bbs_top_dummy_03, IDS_M_BBS_MUNJU_NAME, 5);
	g_pUIManager->SetString(WINDOW_MUNPA_BBS_TOP, munpa_bbs_top_dummy_05, IDS_CLAN_NUMBER, 5);
	g_pUIManager->SetString(WINDOW_MUNPA_BBS_TOP, munpa_bbs_top_dummy_07, IDS_M_BBS_GRADE, 5);
	g_pUIManager->SetString(WINDOW_MUNPA_BBS_TOP, munpa_bbs_top_dummy_09, IDS_M_BBS_FAME, 5);
	g_pUIManager->SetString(WINDOW_MUNPA_BBS_TOP, munpa_bbs_top_dummy_11, IDS_CLAN_RANK, 5);
	g_pUIManager->SetString(WINDOW_MUNPA_BBS_TOP, munpa_bbs_top_dummy_13, IDS_CLAN_RECORD, 5);
	g_pUIManager->SetString(WINDOW_MUNPA_BBS_TOP, munpa_bbs_top_dummy_15, IDS_M_BBS_STONE, 5);
	g_pUIManager->SetString(WINDOW_MUNPA_BBS_TOP, munpa_bbs_top_dummy_17, IDS_M_BBS_WAR, 5);
	g_pUIManager->SetString(WINDOW_MUNPA_BBS_TOP, munpa_bbs_top_dummy_19, IDS_OPPOSITE_CLAN, 5);

	g_pUIManager->SetString(WINDOW_MUNPA_BBS_TOP, munpa_bbs_top_button_01, IDS_M_BBS_STATUS);
	g_pUIManager->SetString(WINDOW_MUNPA_BBS_TOP, munpa_bbs_top_button_02, IDS_M_BBS_NOTICE);
	g_pUIManager->SetString(WINDOW_MUNPA_BBS_TOP, munpa_bbs_top_button_03, IDS_M_BBS_NOTICE_RECORD);
	g_pUIManager->SetString(WINDOW_MUNPA_BBS_TOP, munpa_bbs_top_button_04, IDS_M_BBS_NOTICE_DEL);

	g_pUIManager->SetString(WINDOW_MUNPA_BBS_TOP, munpa_bbs_top_dummy_21,		IDS_M_BBS_MONEY);
	g_pUIManager->SetString(WINDOW_MUNPA_BBS_TOP, munpa_bbs_top_money_button,	IDS_M_BBS_MONEY_COLLECT);

	g_pUIManager->SetData(WINDOW_MUNPA_BBS_TOP, munpa_bbs_top_button_02, CURRENT_INDEX, 2);
	g_pUIManager->SetData(WINDOW_MUNPA_BBS_TOP, munpa_bbs_top_button_03, CURRENT_INDEX, 2);
	g_pUIManager->SetData(WINDOW_MUNPA_BBS_TOP, munpa_bbs_top_button_04, CURRENT_INDEX, 2);
}

/**
 * 氍疙寣 瓴岇嫓寪 毽鞀姼
 */
void XiahGame_Intro::Init_WindowMunpaBBSList()
{
	g_pUIManager->SetPosition(WINDOW_MUNPA_BBS_LIST, WINDOW_FIRST_XPOS-2, 0);

	g_pUIManager->SetString(WINDOW_MUNPA_BBS_LIST, munpa_bbs_list_title_dummy, IDS_M_BBS_1);

	g_pUIManager->SetString(WINDOW_MUNPA_BBS_LIST, munpa_bbs_list_button_01, IDS_M_BBS_STATUS);
	g_pUIManager->SetString(WINDOW_MUNPA_BBS_LIST, munpa_bbs_list_button_02, IDS_M_BBS_NOTICE);
	g_pUIManager->SetString(WINDOW_MUNPA_BBS_LIST, munpa_bbs_list_button_03, IDS_M_BBS_NOTICE_RECORD);
	g_pUIManager->SetString(WINDOW_MUNPA_BBS_LIST, munpa_bbs_list_button_04, IDS_M_BBS_NOTICE_DEL);

	g_pUIManager->SetData(WINDOW_MUNPA_BBS_LIST, munpa_bbs_list_button_03, CURRENT_INDEX, 2);
	g_pUIManager->SetData(WINDOW_MUNPA_BBS_LIST, munpa_bbs_list_button_04, CURRENT_INDEX, 2);
}

/**
 * 瓿奠 澖旮
 */
void XiahGame_Intro::Init_WindowMunpaBBSRead()
{
	g_pUIManager->SetPosition(WINDOW_MUNPA_BBS_READ, WINDOW_FIRST_XPOS-2, 0);

	g_pUIManager->SetString(WINDOW_MUNPA_BBS_READ, munpa_bbs_read_title_dummy, IDS_M_BBS_2);

	g_pUIManager->SetString(WINDOW_MUNPA_BBS_READ, munpa_bbs_read_button_01, IDS_M_BBS_STATUS);
	g_pUIManager->SetString(WINDOW_MUNPA_BBS_READ, munpa_bbs_read_button_02, IDS_M_BBS_NOTICE);
	g_pUIManager->SetString(WINDOW_MUNPA_BBS_READ, munpa_bbs_read_button_03, IDS_M_BBS_NOTICE_RECORD);
	g_pUIManager->SetString(WINDOW_MUNPA_BBS_READ, munpa_bbs_read_button_04, IDS_M_BBS_NOTICE_DEL);

	//g_pUIManager->SetData(WINDOW_MUNPA_BBS_READ, munpa_bbs_read_button_02, CURRENT_INDEX, 2);
	g_pUIManager->SetData(WINDOW_MUNPA_BBS_READ, munpa_bbs_read_button_03, CURRENT_INDEX, 2);
	g_pUIManager->SetData(WINDOW_MUNPA_BBS_READ, munpa_bbs_read_button_04, CURRENT_INDEX, 2);
}

/**
 * 瓿奠 摪旮
 */
void XiahGame_Intro::Init_WindowMunpaBBSWrite()
{
	g_pUIManager->SetPosition(WINDOW_MUNPA_BBS_WRITE, WINDOW_FIRST_XPOS-2, 0);

	g_pUIManager->SetString(WINDOW_MUNPA_BBS_WRITE, munpa_bbs_write_title_dummy, IDS_M_BBS_3);

	g_pUIManager->SetString(WINDOW_MUNPA_BBS_WRITE, munpa_bbs_write_dummy_01, IDS_M_TITLE);
	g_pUIManager->SetString(WINDOW_MUNPA_BBS_WRITE, munpa_bbs_write_dummy_02, IDS_CONTENT);

	g_pUIManager->SetString(WINDOW_MUNPA_BBS_WRITE, munpa_bbs_write_button_01, IDS_OK);
	g_pUIManager->SetString(WINDOW_MUNPA_BBS_WRITE, munpa_bbs_write_button_02, IDS_CANCEL);

	g_pUIManager->SetData(WINDOW_MUNPA_BBS_WRITE, munpa_bbs_write_edit, MAXSTRING, 22);

	g_pUIManager->SetData(WINDOW_MUNPA_BBS_WRITE, munpa_bbs_write_edit_01, MAXSTRING, 26);
	g_pUIManager->SetData(WINDOW_MUNPA_BBS_WRITE, munpa_bbs_write_edit_02, MAXSTRING, 26);
	g_pUIManager->SetData(WINDOW_MUNPA_BBS_WRITE, munpa_bbs_write_edit_03, MAXSTRING, 26);
	g_pUIManager->SetData(WINDOW_MUNPA_BBS_WRITE, munpa_bbs_write_edit_04, MAXSTRING, 26);
	g_pUIManager->SetData(WINDOW_MUNPA_BBS_WRITE, munpa_bbs_write_edit_05, MAXSTRING, 26);
	g_pUIManager->SetData(WINDOW_MUNPA_BBS_WRITE, munpa_bbs_write_edit_06, MAXSTRING, 26);
	g_pUIManager->SetData(WINDOW_MUNPA_BBS_WRITE, munpa_bbs_write_edit_07, MAXSTRING, 26);
	g_pUIManager->SetData(WINDOW_MUNPA_BBS_WRITE, munpa_bbs_write_edit_08, MAXSTRING, 26);
	//g_pUIManager->SetData(WINDOW_MUNPA_BBS_WRITE, munpa_bbs_write_edit_09, MAXSTRING, 26);
	//g_pUIManager->SetData(WINDOW_MUNPA_BBS_WRITE, munpa_bbs_write_edit_10, MAXSTRING, 26);

	g_pUIManager->SetData(WINDOW_MUNPA_BBS_WRITE, munpa_bbs_write_edit_09, EDITMODE, NOEDIT);
	g_pUIManager->SetData(WINDOW_MUNPA_BBS_WRITE, munpa_bbs_write_edit_10, EDITMODE, NOEDIT);
}

/**
 * 旮半
 */
void XiahGame_Intro::Init_WindowMunpaDonate()
{
	g_pUIManager->SetPosition(WINDOW_MUNPA_DONATE, 350, 250);

	g_pUIManager->SetData(WINDOW_MUNPA_DONATE, munpa_donate_edit, MAXSTRING, 3);
	g_pUIManager->SetData(WINDOW_MUNPA_DONATE, munpa_donate_edit, EDIT_INPUT_MODE, NUMERAL);

	g_pUIManager->SetString(WINDOW_MUNPA_DONATE, munpa_donate_button_01, IDS_CONTRIBUTE_OK);
	g_pUIManager->SetString(WINDOW_MUNPA_DONATE, munpa_donate_button_02, IDS_CANCEL);

	g_pUIManager->SetString(WINDOW_MUNPA_DONATE, munpa_donate_dummy_01, IDS_CONTRIBUTE_DES);
	g_pUIManager->SetString(WINDOW_MUNPA_DONATE, munpa_donate_dummy_02, IDS_CONTRIBUTE_FAME, 4);
	g_pUIManager->SetString(WINDOW_MUNPA_DONATE, munpa_donate_dummy_03, IDS_CONTRIBUTE_FAME_ADD, 4);
	g_pUIManager->SetString(WINDOW_MUNPA_DONATE, munpa_donate_dummy_04, IDS_CONTRIBUTE_MONEY, 4);
	g_pUIManager->SetString(WINDOW_MUNPA_DONATE, munpa_donate_check_button, IDS_CONTRIBUTE_EXAMINE);
}

/**
 * 氍疙寣爠 嫚觳
 */
void XiahGame_Intro::Init_WindowMunpaWarPetition()
{
	g_pUIManager->SetPosition(WINDOW_MUNPA_WAR_PETITION, 350, 250);

	g_pUIManager->SetString(WINDOW_MUNPA_WAR_PETITION, munpa_war_petition_dummy_01, IDS_MUNPA_WAR_INFO, 5);
	g_pUIManager->SetString(WINDOW_MUNPA_WAR_PETITION, munpa_war_petition_dummy_02, IDS_WAR_TIME, 4);
	g_pUIManager->SetString(WINDOW_MUNPA_WAR_PETITION, munpa_war_petition_dummy_03, IDS_MUNPA_WAR_STONE, 4);
	g_pUIManager->SetString(WINDOW_MUNPA_WAR_PETITION, munpa_war_petition_dummy_04, IDS_MUNPA_REPARATION, 4);

	g_pUIManager->SetString(WINDOW_MUNPA_WAR_PETITION, munpa_war_petition_button_01, IDS_OK);
	g_pUIManager->SetString(WINDOW_MUNPA_WAR_PETITION, munpa_war_petition_button_02, IDS_CANCEL);
}

/**
 * 爠劀甑
 */
void XiahGame_Intro::Init_WindowMail()
{
	g_pUIManager->SetPosition(WINDOW_MAIL, WINDOW_FIRST_XPOS, 0);

	g_pUIManager->SetString(WINDOW_MAIL, window_mail_title_dummy, IDS_MAIL_TITLE_01);

	g_pUIManager->SetString(WINDOW_MAIL, window_mail_top_dummy_01, IDS_MAIL_01);
	g_pUIManager->SetString(WINDOW_MAIL, window_mail_top_dummy_02, IDS_MAIL_02);
	g_pUIManager->SetString(WINDOW_MAIL, window_mail_top_dummy_03, IDS_M_TITLE);

	g_pUIManager->SetString(WINDOW_MAIL, window_mail_button_01, IDS_MAIL_READ);
	g_pUIManager->SetString(WINDOW_MAIL, window_mail_button_02, IDS_MAIL_WIRTE);
	g_pUIManager->SetString(WINDOW_MAIL, window_mail_button_03, IDS_DELETE);
	g_pUIManager->SetString(WINDOW_MAIL, window_mail_button_04, IDS_SEND);

	g_pUIManager->SetString(WINDOW_MAIL, window_mail_input_dummy_01, IDS_MAIL_01);
	g_pUIManager->SetString(WINDOW_MAIL, window_mail_input_dummy_02, IDS_M_TITLE);

	g_pUIManager->SetData(WINDOW_MAIL, window_mail_top_edit_01, MAXSTRING, 20);	// ID
	g_pUIManager->SetData(WINDOW_MAIL, window_mail_top_edit_02, MAXSTRING, 24);

	g_pUIManager->SetData(WINDOW_MAIL, window_mail_edit_01, MAXSTRING, 30);
	g_pUIManager->SetData(WINDOW_MAIL, window_mail_edit_02, MAXSTRING, 30);
	g_pUIManager->SetData(WINDOW_MAIL, window_mail_edit_03, MAXSTRING, 30);
	g_pUIManager->SetData(WINDOW_MAIL, window_mail_edit_04, MAXSTRING, 30);
	g_pUIManager->SetData(WINDOW_MAIL, window_mail_edit_05, MAXSTRING, 30);
	g_pUIManager->SetData(WINDOW_MAIL, window_mail_edit_06, MAXSTRING, 30);
	g_pUIManager->SetData(WINDOW_MAIL, window_mail_edit_07, MAXSTRING, 30);
	g_pUIManager->SetData(WINDOW_MAIL, window_mail_edit_08, MAXSTRING, 30);

	// 瀰牓 牍勴櫆劚檾
	g_pUIManager->SetData(WINDOW_MAIL, window_mail_top_edit_01, EDITMODE, NOEDIT);
//	g_pUIManager->SetData(WINDOW_MAIL, window_mail_top_edit_02, EDITMODE, NOEDIT);
//
//	g_pUIManager->SetData(WINDOW_MAIL, window_mail_edit_01, EDITMODE, NOEDIT);
//	g_pUIManager->SetData(WINDOW_MAIL, window_mail_edit_02, EDITMODE, NOEDIT);
//	g_pUIManager->SetData(WINDOW_MAIL, window_mail_edit_03, EDITMODE, NOEDIT);
//	g_pUIManager->SetData(WINDOW_MAIL, window_mail_edit_04, EDITMODE, NOEDIT);
//	g_pUIManager->SetData(WINDOW_MAIL, window_mail_edit_05, EDITMODE, NOEDIT);
//	g_pUIManager->SetData(WINDOW_MAIL, window_mail_edit_06, EDITMODE, NOEDIT);
//	g_pUIManager->SetData(WINDOW_MAIL, window_mail_edit_07, EDITMODE, NOEDIT);
//	g_pUIManager->SetData(WINDOW_MAIL, window_mail_edit_08, EDITMODE, NOEDIT);
}

/**
 * 爠劀甑 劆儩
 */
void XiahGame_Intro::Init_WindowMailSelect()
{
	g_pUIManager->SetPosition(WINDOW_MAIL_SELECT, WINDOW_SECOND_XPOS, 0);

	g_pUIManager->SetString(WINDOW_MAIL_SELECT, window_mail_select_title_dummy, IDS_MAIL_TITLE_02);

	g_pUIManager->SetString(WINDOW_MAIL_SELECT, window_mail_select_top_dummy_01, IDS_RELATION);
	g_pUIManager->SetString(WINDOW_MAIL_SELECT, window_mail_select_top_dummy_02, IDS_NAME);
	g_pUIManager->SetString(WINDOW_MAIL_SELECT, window_mail_select_top_dummy_03, IDS_MAIL_ISSEND);

	g_pUIManager->SetString(WINDOW_MAIL_SELECT, window_mail_select_button_01, IDS_MAIL_MUNPA_SELECT);
	g_pUIManager->SetString(WINDOW_MAIL_SELECT, window_mail_select_button_02, IDS_MAIL_ALL_SELECT);
	g_pUIManager->SetString(WINDOW_MAIL_SELECT, window_mail_select_button_03, IDS_MAIL_ALL_UNSELECT);

}

/**
 * 爠劀甑 爠啞 瓴瓣臣
 */
void XiahGame_Intro::Init_WindowMailResult()
{
	g_pUIManager->SetPosition(WINDOW_MAIL_RESULT, WINDOW_SECOND_XPOS, 0);

	g_pUIManager->SetString(WINDOW_MAIL_RESULT, window_mail_result_title_dummy, IDS_MAIL_TITLE_03);

	g_pUIManager->SetString(WINDOW_MAIL_RESULT, window_mail_result_top_dummy_01, IDS_RELATION);
	g_pUIManager->SetString(WINDOW_MAIL_RESULT, window_mail_result_top_dummy_02, IDS_NAME);
	g_pUIManager->SetString(WINDOW_MAIL_RESULT, window_mail_result_top_dummy_03, IDS_MAIL_ISSEND);

	g_pUIManager->SetString(WINDOW_MAIL_RESULT, window_mail_result_button_01, IDS_OK);
}


/**
 * 
 */
void XiahGame_Intro::Init_WindowCommon()
{
	g_pUIManager->SetPosition(WINDOW_COMMON, 400, 300);

	g_pUIManager->SetString(WINDOW_COMMON, winodw_common_dummy_01, IDS_PET_SELL);
	g_pUIManager->SetString(WINDOW_COMMON, winodw_common_dummy_02, IDS_PT_SET_SELL_MONEY);

	g_pUIManager->SetString(WINDOW_COMMON, winodw_common_button_01, IDS_OK);
	g_pUIManager->SetString(WINDOW_COMMON, winodw_common_button_02, IDS_CANCEL);
}

/**
 * 帿 瓯半灅 爼氤
 */
void XiahGame_Intro::Init_WindowPetTrade()
{
	g_pUIManager->SetPosition(WINDOW_PET_TRADE, 350, 250);

	g_pUIManager->SetString(WINDOW_PET_TRADE, window_pet_trade_dummy_title, IDS_PET_TRADE_INFO);

	g_pUIManager->SetString(WINDOW_PET_TRADE, window_pet_trade_dummy_01, IDS_TYPE);
	g_pUIManager->SetString(WINDOW_PET_TRADE, window_pet_trade_dummy_02, IDS_NAME_2);
	g_pUIManager->SetString(WINDOW_PET_TRADE, window_pet_trade_dummy_03, IDS_STR_PWR_2);
	g_pUIManager->SetString(WINDOW_PET_TRADE, window_pet_trade_dummy_04, IDS_AGI_PWR_2);
	g_pUIManager->SetString(WINDOW_PET_TRADE, window_pet_trade_dummy_05, IDS_DEF_PWR_2);
	g_pUIManager->SetString(WINDOW_PET_TRADE, window_pet_trade_dummy_06, IDS_LIFE_PWR_2);
	g_pUIManager->SetString(WINDOW_PET_TRADE, window_pet_trade_dummy_07, IDS_SELLPRICE);

	g_pUIManager->SetString(WINDOW_PET_TRADE, window_pet_trade_dummy, IDS_BUY);

	g_pUIManager->SetString(WINDOW_PET_TRADE, window_pet_trade_button_01, IDS_OK);
	g_pUIManager->SetString(WINDOW_PET_TRADE, window_pet_trade_button_02, IDS_CANCEL);
}

/**
 * 氤店秾 劆儩
 */
void XiahGame_Intro::Init_WindowBokNumber()
{
	g_pUIManager->SetPosition(WINDOW_BOK_NUMBER, 350, 200);

	g_pUIManager->SetString(WINDOW_BOK_NUMBER, bok_number_title_dummy, IDS_LOTTO_TITLE);
	g_pUIManager->SetString(WINDOW_BOK_NUMBER, bok_number_dummy_01, IDS_LOTTO_SELECT);

	for(int i=0; i < 25; ++i)
	{
		g_pUIManager->SetString(WINDOW_BOK_NUMBER, bok_number_number_button_01 + i, i+1);
	}

	g_pUIManager->SetString(WINDOW_BOK_NUMBER, bok_number_button_01, IDS_LOTTO_BUY_2);
	g_pUIManager->SetString(WINDOW_BOK_NUMBER, bok_number_button_02, IDS_CANCEL);
	g_pUIManager->SetString(WINDOW_BOK_NUMBER, bok_number_button_03, IDS_LOTTO_EXPECT);


}

/**
 * 氤店秾 嫻觳氩垬 殔晬
 */
void XiahGame_Intro::Init_WindowBokPrize()
{
	g_pUIManager->SetPosition(WINDOW_BOK_PRIZE, 350, 200);

	g_pUIManager->SetString(WINDOW_BOK_PRIZE, bok_prize_title_dummy, IDS_LOTTO_TITLE);

	g_pUIManager->SetString(WINDOW_BOK_PRIZE, bok_prize_button_01, IDS_LOTTO_DEFINITE_LAST);
	g_pUIManager->SetString(WINDOW_BOK_PRIZE, bok_prize_button_02, IDS_LOTTO_DEFINITE_NOW);
}

/**
 * 氍疙寣毵堩伂 - 嫟毳胳毄弰臧姤
 */
void XiahGame_Intro::Init_WindowMark()
{
	g_pUIManager->SetPosition(MESSAGE_WINDOW_MARK, 350, 250);

	g_pUIManager->SetString(MESSAGE_WINDOW_MARK, mark_window_dummy, IDS_MUNPA_MARK_INFO);

	g_pUIManager->SetString(MESSAGE_WINDOW_MARK, mark_window_button_01, IDS_MUNPA_MARK_RECORD);
	g_pUIManager->SetString(MESSAGE_WINDOW_MARK, mark_window_button_02, IDS_CANCEL);
}

/**
 * 臁绊暕彀
 */
void XiahGame_Intro::Init_WindowSmelt()
{
	g_pUIManager->SetPosition(WINDOW_SMELT, WINDOW_SECOND_XPOS, 0);

	g_pUIManager->SetString(WINDOW_SMELT, smelt_window_title_dummy, IDS_MIXTURE);

	g_pUIManager->SetString(WINDOW_SMELT, smelt_window_button_01, IDS_OK);
	g_pUIManager->SetString(WINDOW_SMELT, smelt_window_button_02, IDS_CANCEL);
}

/**
 * 槫枆
 */
void XiahGame_Intro::Init_WindowFiveElement()
{
	g_pUIManager->SetPosition(WINDOW_FIVEELEMENTS, WINDOW_FIRST_XPOS, 0);

	g_pUIManager->SetString(WINDOW_FIVEELEMENTS, fiveelements_window_title_dummy, IDS_FIVEELEMENT);
	g_pUIManager->SetString(WINDOW_FIVEELEMENTS, fiveelements_window_top_button_01, IDS_OUT_PWR);	
	g_pUIManager->SetString(WINDOW_FIVEELEMENTS, fiveelements_window_top_button_02, IDS_IN_PWR);
	g_pUIManager->SetString(WINDOW_FIVEELEMENTS, fiveelements_window_top_button_03, IDS_FIVEELEMENT);
	g_pUIManager->SetString(WINDOW_FIVEELEMENTS, fiveelements_window_top_button_04, IDS_SKILL);

	g_pUIManager->SetData(WINDOW_FIVEELEMENTS, fiveelements_window_top_button_03, CURRENT_INDEX, 2);

	g_pUIManager->SetString(WINDOW_FIVEELEMENTS, fiveelements_window_name_dummy_01, IDS_FIVEELEMENT_EXP);
	g_pUIManager->SetString(WINDOW_FIVEELEMENTS, fiveelements_window_name_dummy_02, IDS_FIVEELEMENT_SPECIALATTACK);

	g_pUIManager->SetString(WINDOW_FIVEELEMENTS, fiveelements_window_exp_dummy_06, IDS_FIVEELEMENT_EXP1);
	g_pUIManager->SetString(WINDOW_FIVEELEMENTS, fiveelements_window_exp_dummy_07, IDS_FIVEELEMENT_EXP2);
	g_pUIManager->SetString(WINDOW_FIVEELEMENTS, fiveelements_window_exp_dummy_08, IDS_FIVEELEMENT_EXP3);
	g_pUIManager->SetString(WINDOW_FIVEELEMENTS, fiveelements_window_exp_dummy_09, IDS_FIVEELEMENT_EXP4);
	g_pUIManager->SetString(WINDOW_FIVEELEMENTS, fiveelements_window_exp_dummy_10, IDS_FIVEELEMENT_EXP5);

	g_pUIManager->SetData(WINDOW_FIVEELEMENTS, fiveelements_window_exp_bar, VALUE1, 0);
	g_pUIManager->SetData(WINDOW_FIVEELEMENTS, fiveelements_window_skill_bar, VALUE1, 0);

	g_pUIManager->SetString(WINDOW_FIVEELEMENTS, fiveelements_window_skill_percent_dummy, _T("0 %"));
		
	g_pUIManager->SetData(WINDOW_FIVEELEMENTS, fiveelements_window_skill_dummy_01, TEXTURE, 1336);
	g_pUIManager->SetData(WINDOW_FIVEELEMENTS, fiveelements_window_skill_dummy_02, TEXTURE, 1340);
	g_pUIManager->SetData(WINDOW_FIVEELEMENTS, fiveelements_window_skill_dummy_03, TEXTURE, 1344);
	g_pUIManager->SetData(WINDOW_FIVEELEMENTS, fiveelements_window_skill_dummy_04, TEXTURE, 0);
	g_pUIManager->SetData(WINDOW_FIVEELEMENTS, fiveelements_window_skill_dummy_05, TEXTURE, 0);
	g_pUIManager->SetData(WINDOW_FIVEELEMENTS, fiveelements_window_skill_dummy_06, TEXTURE, 0);
	g_pUIManager->SetData(WINDOW_FIVEELEMENTS, fiveelements_window_skill_dummy_07, TEXTURE, 1352);
	g_pUIManager->SetData(WINDOW_FIVEELEMENTS, fiveelements_window_skill_dummy_08, TEXTURE, 1348);

	
	g_pUIManager->Hide(WINDOW_FIVEELEMENTS, fiveelements_window_skill_dummy_04);
	g_pUIManager->Hide(WINDOW_FIVEELEMENTS, fiveelements_window_skill_dummy_05);
	g_pUIManager->Hide(WINDOW_FIVEELEMENTS, fiveelements_window_skill_dummy_06);

	
	g_MainCharInfo.m_pMugong->SetFiveElementToolTip(150, WINDOW_FIVEELEMENTS, fiveelements_window_skill_dummy_01);
	g_MainCharInfo.m_pMugong->SetFiveElementToolTip(151, WINDOW_FIVEELEMENTS, fiveelements_window_skill_dummy_02);
	g_MainCharInfo.m_pMugong->SetFiveElementToolTip(152, WINDOW_FIVEELEMENTS, fiveelements_window_skill_dummy_03);
	g_MainCharInfo.m_pMugong->SetFiveElementToolTip(153, WINDOW_FIVEELEMENTS, fiveelements_window_skill_dummy_07);
	g_MainCharInfo.m_pMugong->SetFiveElementToolTip(154, WINDOW_FIVEELEMENTS, fiveelements_window_skill_dummy_08);

	//HT_0720 : 槫枆 臧滌劆 偓暛
	g_pUIManager->SetData(WINDOW_FIVEELEMENTS, fiveelements_window_button_end, CURRENT_INDEX, -1);
}

/**
 * 槫枆 牅牗
 */
void XiahGame_Intro::Init_WindowFiveElementConvert()
{
	g_pUIManager->SetPosition(WINDOW_FIVEELEMENTS_CONVERT, WINDOW_SECOND_XPOS, 0);

	g_pUIManager->SetString(WINDOW_FIVEELEMENTS_CONVERT, fiveelements_convert_window_dummy, IDS_FIVEELEMENTCONVERT);
	g_pUIManager->SetString(WINDOW_FIVEELEMENTS_CONVERT, fiveelements_convert_window_dummy_06, IDS_FE_CONVERT_MONEY);
	g_pUIManager->SetString(WINDOW_FIVEELEMENTS_CONVERT, fiveelements_convert_window_button_01, IDS_FIVEELEMENTCONVERT_OK);
	g_pUIManager->SetString(WINDOW_FIVEELEMENTS_CONVERT, fiveelements_convert_window_button_02, IDS_CANCEL);

	TCHAR strMoney[32] = {0,};
	_stprintf(strMoney, IDS_MONEY, MoneyCommaStr(0).data());
	g_pUIManager->SetString(WINDOW_FIVEELEMENTS_CONVERT, fiveelements_convert_window_dumy_05, strMoney);
}

/**
 * 檾 毽鞀姼
 */
void XiahGame_Intro::Init_WindowHelperList()
{
	//HO_0410_07 儊劀牴 臧澊摐 梾嵃澊姼
	g_pUIManager->SetPosition(WINDOW_HELPER_LIST2, WINDOW_FIRST_XPOS-100, 270); //WINDOW_FIRST_XPOS, 0); //儊劀牴 臧澊摐 梾嵃澊姼 : 儊劀牴 臧澊摐 梾嵃澊姼 爠 唽姢毳 儛瀾毄溂搿 氤櫂
	
	g_pUIManager->SetPosition(WINDOW_HELPER_LIST1, WINDOW_FIRST_XPOS, 0);//儊劀牴 臧澊摐 梾嵃澊姼 : 攧爤瀯彀 伂旮 氤瓴届溂搿 儊劀牴 檾彀届潣 毎旄 儊嫧溂搿 澊彊

	g_pUIManager->SetString(WINDOW_HELPER_LIST1, helper_window1_title_bar, IDS_GAME_GUIDE);
	g_pUIManager->SetString(WINDOW_HELPER_LIST1, helper_window1_button_01, IDS_TALK_1);	
	g_pUIManager->SetString(WINDOW_HELPER_LIST1, helper_window1_button_02, IDS_TALK_2);
	g_pUIManager->Show(WINDOW_HELPER_LIST1, helper_window1_button_01);
	g_pUIManager->Show(WINDOW_HELPER_LIST1, helper_window1_button_02);

	g_pUIManager->SetPosition(WINDOW_HELPER_LIST, WINDOW_FIRST_XPOS, 0);

	g_pUIManager->SetString(WINDOW_HELPER_LIST, helper_window_title_bar, IDS_GAME_GUIDE);
	g_pUIManager->SetString(WINDOW_HELPER_LIST, helper_window_button_01, IDS_TALK_1);
	g_pUIManager->SetString(WINDOW_HELPER_LIST, helper_window_button_02, IDS_TALK_2);
	g_pUIManager->Show(WINDOW_HELPER_LIST, helper_window_button_01);
	g_pUIManager->Show(WINDOW_HELPER_LIST, helper_window_button_02);

	//g_pUIManager->SetString(WINDOW_HELPER_LIST2, helper_window_list_title_dummy, IDS_CONVERSATION);	
}

/**
 * 檾 彀
 */
void XiahGame_Intro::Init_WindowHelperScript()
{
	g_pUIManager->SetPosition(WINDOW_HELPER_SCRIPT, WINDOW_FIRST_XPOS-100, 270);//儛瀾

	g_pUIManager->SetString(WINDOW_HELPER_SCRIPT, helper_window_script_button1, IDS_TALK_1);	
	g_pUIManager->SetString(WINDOW_HELPER_SCRIPT, helper_window_script_button2, IDS_TALK_2);	
	
	//HO_0413_07 儊劀牴 臧澊摐 梾嵃澊姼
	g_pUIManager->SetPosition(WINDOW_HELPER_LIST0, 0, 0);//儊劀牴 臧澊摐 梾嵃澊姼搿 澑暅 攧爤瀯彀 伂旮 氤瓴届溂搿 儊劀牴 檾彀届潣 毎旄 儊嫧溂搿 澊彊 //HO_0215_07 儓搿滌毚 毽靻寠 爜毄 旮办〈 毽靻寠姅 WINDOW_HELPER_SCRIPT 瀯

	g_pUIManager->Show(WINDOW_HELPER_LIST0, helper_window0_button_01);
	g_pUIManager->Show(WINDOW_HELPER_LIST0, helper_window0_button_02);

	g_pUIManager->SetString(WINDOW_HELPER_LIST0, helper_window0_title_bar, IDS_QUICK_GUIDE);
	g_pUIManager->SetString(WINDOW_HELPER_LIST0, helper_window0_button_01, IDS_QUICK_BUTTON1);
	g_pUIManager->SetString(WINDOW_HELPER_LIST0, helper_window0_button_02, IDS_QUICK_BUTTON2);

}

/**
 * 晞澊厹 氤店惮
 */
void XiahGame_Intro::Init_WindowRecovery()
{
	g_pUIManager->SetPosition(WINDOW_RECOVERY, WINDOW_FIRST_XPOS, -2);

	g_pUIManager->SetString(WINDOW_RECOVERY, recovery_title, IDS_RECOVERY_TITLE);

	g_pUIManager->SetString(WINDOW_RECOVERY, recovery_window_button01, IDS_RECOVERY);
	g_pUIManager->SetString(WINDOW_RECOVERY, recovery_window_button02, IDS_SWEEP);
	g_pUIManager->SetString(WINDOW_RECOVERY, recovery_window_button03, IDS_CANCEL);

	//g_pUIManager->SetData(MAIN_FRAME, main_frame_socket_dummy_02 + i, TYPE, STATICDUMMY);
}

/**
* 嫧 瓴巾棙旃 攵勲鞍
*/
void XiahGame_Intro::Init_WindowDanNew()
{
	g_pUIManager->SetPosition(WINDOW_DAN_NEW, WINDOW_FIRST_XPOS, 0);

	g_pUIManager->SetString(WINDOW_DAN_NEW, window_dan_new_title_dummy, IDS_RELATION);
	g_pUIManager->SetString(WINDOW_DAN_NEW, window_dan_new_button1, IDS_DAN);
	g_pUIManager->SetData(WINDOW_DAN_NEW, window_dan_new_button1, CURRENT_INDEX, 2);
	g_pUIManager->SetString(WINDOW_DAN_NEW, window_dan_new_button2, IDS_SHIP);
	g_pUIManager->SetString(WINDOW_DAN_NEW, window_dan_new_button3, IDS_CLAN);

	g_pUIManager->SetString(WINDOW_DAN_NEW, window_dan_new_list_dummy1, IDS_NAME);
	g_pUIManager->SetString(WINDOW_DAN_NEW, window_dan_new_list_dummy2, IDS_LEVEL);
	g_pUIManager->SetString(WINDOW_DAN_NEW, window_dan_new_list_dummy3, IDS_LOCATION);

	g_pUIManager->SetString(WINDOW_DAN_NEW, window_dan_new_2button_01, IDS_JEMYUNG);
	g_pUIManager->SetString(WINDOW_DAN_NEW, window_dan_new_2button_02, IDS_TALTE);
	g_pUIManager->SetString(WINDOW_DAN_NEW, window_dan_new_1button, IDS_TALTE);

	g_pUIManager->SetString(WINDOW_DAN_NEW, window_dan_new_button_dummy2, IDS_EXP);
	g_pUIManager->SetString(WINDOW_DAN_NEW, window_dan_new_button_dummy3, IDS_FE_EXP_VALUE);

	g_pUIManager->SetString(WINDOW_DAN_NEW, window_dan_new_exp_dummy1, IDS_UNION_DIVISION);
	g_pUIManager->SetString(WINDOW_DAN_NEW, window_dan_new_exp_dummy2, IDS_PERSONAL_DIVISION);
	g_pUIManager->SetString(WINDOW_DAN_NEW, window_dan_new_exp_dummy3, IDS_UNION_DIVISION);
	g_pUIManager->SetString(WINDOW_DAN_NEW, window_dan_new_exp_dummy4, IDS_PERSONAL_DIVISION);
}

/**
 * NPC 彫儓 澊彊
 */
void XiahGame_Intro::Init_WindowPortal()
{
	g_pUIManager->SetPosition(WINDOW_PORTAL, WINDOW_FIRST_XPOS-100, 240);

	g_pUIManager->SetString(WINDOW_PORTAL, window_portal_back_dummy, IDS_NPC_PORTAL);

	g_pUIManager->SetString(WINDOW_PORTAL, window_portal_button1, IDS_NPC_PORTAL_1);
	g_pUIManager->SetString(WINDOW_PORTAL, window_portal_button2, IDS_NPC_PORTAL_2);

	g_pUIManager->SetString(WINDOW_PORTAL, window_portal_exit_button, IDS_PORTAL_CANCEL);

	g_pUIManager->Hide(WINDOW_PORTAL, window_portal_button3);
	g_pUIManager->Hide(WINDOW_PORTAL, window_portal_button4);
}

/**
* 晞澊厹 垬歆
*/
void XiahGame_Intro::Init_WindowCollection()
{
	g_pUIManager->SetPosition(WINDOW_COLLECTION, WINDOW_SECOND_XPOS+92, -2);

	g_pUIManager->SetString(WINDOW_COLLECTION, collection_window_titledummy, IDS_COLLECTION);
}

/**
* 臧侅劚 彀
*/
void XiahGame_Intro::Init_WindowSkill()
{
	g_pUIManager->SetPosition(WINDOW_SKILL, WINDOW_FIRST_XPOS, 0);
	
	g_pUIManager->SetString(WINDOW_SKILL, skill_window_title_back, IDS_SKILL);

	g_pUIManager->SetString(WINDOW_SKILL, skill_window_top_button_01, IDS_OUT_PWR);	
	g_pUIManager->SetString(WINDOW_SKILL, skill_window_top_button_02, IDS_IN_PWR);
	g_pUIManager->SetString(WINDOW_SKILL, skill_window_top_button_03, IDS_FIVEELEMENT);
	g_pUIManager->SetString(WINDOW_SKILL, skill_window_top_button_04, IDS_SKILL);

	g_pUIManager->SetData(WINDOW_SKILL, skill_window_top_button_04, CURRENT_INDEX, 2);

	g_pUIManager->SetString(WINDOW_SKILL, skill_window_training_point_name_01, IDS_TP);
	g_pUIManager->SetString(WINDOW_SKILL, skill_window_training_point_name_02, g_MainCharInfo.m_wRemainTp);
	
	g_pUIManager->SetString(WINDOW_SKILL, skill_window_center_title_dummy_01, IDS_REBIRTH_IN_PWR);
	g_pUIManager->SetString(WINDOW_SKILL, skill_window_center_title_dummy_02, IDS_REBIRTH_OUT_PWR);
	g_pUIManager->SetString(WINDOW_SKILL, skill_window_center_title_dummy_03, IDS_REBIRTH_SPECIAL_PWR);


	// 偞瓿
	for( int i=161; i < 164; ++i)
	{
		sArrayData* pRebirthData = XiahArrayIndex::g_RebirthMugong_List.GetData( i);
		if( pRebirthData)
		{
			int nMugongID = pRebirthData->GetInt(0);
			int nResID	  = pRebirthData->GetInt(2);
			int nSeq	  = pRebirthData->GetInt(55);

			// 澊毽
			sString szMugongName = pRebirthData->GetString( 1);

			g_pUIManager->SetString(WINDOW_SKILL, skill_window_negong_name_dummy_01 + nSeq - 1, (LPCTSTR)szMugongName);
			// 爤氩
			g_pUIManager->SetString(WINDOW_SKILL, skill_window_negong_skill_01 + nSeq - 1, 0);

			// 埓寔
			if( g_MainCharInfo.m_pMugong)
				g_MainCharInfo.m_pMugong->SetRebirthToolTip( nMugongID, WINDOW_SKILL, skill_window_negong_01 + nSeq - 1);

			g_pUIManager->SetData(WINDOW_SKILL, skill_window_negong_01 + nSeq - 1, TEXTURE, nResID);
		}
	}

	// 瓿淀喌 澊氙胳
	for( int i=180; i < 182; ++i)
	{
		sArrayData* pRebirthData = XiahArrayIndex::g_RebirthMugong_List.GetData( i);
		if( pRebirthData)
		{
			int nMugongID = pRebirthData->GetInt(0);
			int nResID	  = pRebirthData->GetInt(2);
			int nSeq	  = pRebirthData->GetInt(55);

			// 澊毽
			sString szMugongName = pRebirthData->GetString( 1);

			g_pUIManager->SetString(WINDOW_SKILL, skill_window_nugong_name_dummy_01 + nSeq - 4, (LPCTSTR)szMugongName);
			// 爤氩
			g_pUIManager->SetString(WINDOW_SKILL, skill_window_nugong_skill_01 + nSeq - 4, 0);

			// 埓寔
			if( g_MainCharInfo.m_pMugong)
				g_MainCharInfo.m_pMugong->SetRebirthToolTip( nMugongID, WINDOW_SKILL, skill_window_mugong_dummy_01 + nSeq - 4);

			// 晞澊旖 澊氙胳
			g_pUIManager->SetData(WINDOW_SKILL, skill_window_mugong_dummy_01 + nSeq - 4, TEXTURE, nResID);
		}
	}

	// 湢寣 櫢瓿
	for( int i=0; i < 2; ++i)
	{		
		int nMugongID = (g_MainCharInfo.m_bCharType * 2) + 170 + i;
		
		sArrayData* pMugongData = XiahArrayIndex::g_RebirthMugong_List.GetData( nMugongID);

		if( pMugongData)
		{
			int nResID	  = pMugongData->GetInt(2);
			int nSeq	  = pMugongData->GetInt(55);

			// 澊毽
			sString szMugongName = pMugongData->GetString( 1);

			g_pUIManager->SetString(WINDOW_SKILL, skill_window_nugong_name_dummy_01 + nSeq - 4, (LPCTSTR)szMugongName);

			// 爤氩
			g_pUIManager->SetString(WINDOW_SKILL, skill_window_nugong_skill_01 + nSeq - 4, 0);

			// 埓寔
			if( g_MainCharInfo.m_pMugong)
				g_MainCharInfo.m_pMugong->SetRebirthToolTip( nMugongID, WINDOW_SKILL, skill_window_mugong_dummy_01 + nSeq - 4);

			// 晞澊旖 澊氙胳
			g_pUIManager->SetData(WINDOW_SKILL, skill_window_mugong_dummy_01 + nSeq - 4, TEXTURE, nResID);			
		}
	}
}

/**
 * 甏戨獏爠 & 觳滍櫓爠 彀胳棳彀
 */
void XiahGame_Intro::Init_WindowSecretApplication()
{
	g_pUIManager->SetPosition(WINDOW_SECRET_INFORMATION, WINDOW_FIRST_XPOS-100, 270); 

	g_pUIManager->SetString(WINDOW_SECRET_INFORMATION, secret_information_window_button01, IDS_APPLICATION);	
	g_pUIManager->SetString(WINDOW_SECRET_INFORMATION, secret_information_window_button02, IDS_CANCEL);	
	
}

/**
 * 甏戨獏爠 & 觳滍櫓爠 澊彊彀
 */
void XiahGame_Intro::Init_WindowSecretMove()
{
	g_pUIManager->SetPosition(WINDOW_SECRET_CHECK, WINDOW_FIRST_XPOS-100, 270);

	g_pUIManager->SetString(WINDOW_SECRET_CHECK, secret_check_window_button01, IDS_MAP_MOVE);	
	g_pUIManager->SetString(WINDOW_SECRET_CHECK, secret_check_window_button02, IDS_CANCEL);	
}

/**
 * 甏戨獏爠 & 觳滍櫓爠 旮半掣彀
 */
void XiahGame_Intro::Init_WindowSecretBasis()
{
	g_pUIManager->SetPosition(WINDOW_SECRET_BASIS, WINDOW_FIRST_XPOS-100, 270); 

}