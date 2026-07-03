#pragma once

//extern void RegisterAllNetworkHandler_Interface();

typedef void (*XIAH_INTERFACE_PROCESS_FUNCTION)( LPARAM lParam);

extern void RegistInterfaceHandler( int nFrameID, XIAH_INTERFACE_PROCESS_FUNCTION function);
extern void ProcessInterfaceHanler( int nFrameID, LPARAM lParam);
extern void RegisterInterfaceHandler();



/////////////////////////////////////////////////////////////////////////////////////////////////////
// window
/////////////////////////////////////////////////////////////////////////////////////////////////////
extern void ProcessIntroAccount( LPARAM lParam);
extern void ProcessIntroButtonSet( LPARAM lParam);
extern void ProcessIntroCharacterSelect( LPARAM lParam);
extern void ProcessIntroWindow( LPARAM lParam);

// login
extern void ProcessLogin1(LPARAM lParam);
extern void ProcessLogin2(LPARAM lParam);

extern void ProcessMainFrame( LPARAM lParam);
extern void ProcessWindowButtonGroup( LPARAM lParam);
extern void ProcessSystemButtonGroup( LPARAM lParam);
extern void ProcessPetButtonGroup( LPARAM lParam);
extern void ProcessMainChat( LPARAM lParam);
//extern void ProcessMessengerChannel( LPARAM lParam);
extern void ProcessSpirit(LPARAM lParam);				// 기
extern void ProcessHELPER(LPARAM lParam);				//HO_0413_07 퀵 가이드 업데이트


extern void ProcessWindowCharacter( LPARAM lParam);
extern void ProcessWindowItem( LPARAM lParam);
extern void ProcessWindowOutSide( LPARAM lParam);
extern void ProcessWindowInSide( LPARAM lParam);
extern void ProcessWindowDan( LPARAM lParam);
extern void ProcessWindowDanNew(LPARAM lParam);			// 단 경험치 분배
extern void ProcessWindowClan( LPARAM lParam);
extern void ProcessWindowClanFound( LPARAM lParam);
extern void ProcessWindowTaming( LPARAM lParam);
extern void ProcessWindowTamingItem( LPARAM lParam);

extern void ProcessWindowNpcTradeTab( LPARAM lParam);
extern void ProcessWindowConvert( LPARAM lParam);
extern void ProcessWindowPcTrade( LPARAM lParam);
extern void ProcessWidnowNpcTrade( LPARAM lParam);
extern void ProcessWindowVolume( LPARAM lParam);
extern void ProcessWindowConnectionInfo( LPARAM lParam);
extern void ProcessWindowNameConfer( LPARAM lParam);
extern void ProcessWindowQuest( LPARAM lParam);
extern void ProcessWindowOption1( LPARAM lParam);
extern void ProcessWindowOption2( LPARAM lParam);
extern void ProcessWindowOption3(LPARAM lParam);		// 거래 옵션
extern void ProcessWindowClose( LPARAM lParam);

extern void ProcessWindowPcStore( LPARAM lParam);
extern void ProcessWindowMoney( LPARAM lParam);

extern void ProcessWindowGakMessage(LPARAM lParam);
extern void ProcessWindowDongSin(LPARAM lParam);

extern void ProcessWindowDanWar(LPARAM lParam);
extern void ProcessWindowPurse(LPARAM lParam);

extern void ProcessWindowMunpaBBSTop(LPARAM lParam);
extern void ProcessWindowMunpaBBSList(LPARAM lParam);
extern void ProcessWindowMunpaBBSRead(LPARAM lParam);	
extern void ProcessWindowMunpaBBSWrite(LPARAM lParam);
extern void ProcessWindowMunpaDonate(LPARAM lParam);
extern void ProcessWindowMunpaWarPetition(LPARAM lParam);
extern void ProcessWindowMark(LPARAM lParam);

extern void ProcessWindowMail(LPARAM lParam);
extern void	ProcessWindowMailSelect(LPARAM lParam);
extern void ProcessWindowMailResult(LPARAM lParam);
extern void ProcessWindowCommon(LPARAM lParam);
extern void ProcessWindowPetTrade(LPARAM lParam);
extern void ProcessWindowBokNumber(LPARAM lParam);
extern void ProcessWindowBokPrize(LPARAM lParam);
extern void ProcessWindowSmelt(LPARAM lParam);
extern void ProcessWindowFiveElement(LPARAM lParam);	// 오행
extern void ProcessWindowFiveElementConvert(LPARAM lParam);
// [1/14/2005] 도우미
//HO_0410_07 상서령 가이드 업데이트
extern void ProcessWindowTamRangScript(LPARAM lParam); //상서령 가이드 업데이트 : 탐랑 분리
extern void ProcessWindowQuickScript(LPARAM lParam); //HO_0413_07 퀵 가이드 업데이트 : 스크립트
extern void ProcessWindowHelperList(LPARAM lParam); //상서령 대화창
extern void ProcessWindowHelperScript(LPARAM lParam);//탐랑 대화창

extern void ProcessWindowRecovery(LPARAM lParam);		// 아이템 복구

extern void UpdatePetManagerList();extern void ProcessWindowPortal(LPARAM lParam);			// NPC 포탈 이동
extern void ProcessWindowCollection(LPARAM lParam);		// 아이템 수집

extern void ProcessWindowSecretMove(LPARAM lParam);		//HT_0313 : 광명전 & 천황전 (이동)	
extern void ProcessWindowSecretApplication(LPARAM lParam);		//HT_0313 : 광명전 & 천황전 (참여)	

/////////////////////////////////////////////////////////////////////////////////////////////////////
// Notice
/////////////////////////////////////////////////////////////////////////////////////////////////////
extern void ProcessNoticeWindow1( LPARAM lParam);
extern void ProcessNoticeReBirthSuccess( int controlID);
extern void ProcessNoticeFrameTerminate( int controlID);


extern void ProcessNoticeWindow2( LPARAM lParam);
extern void ProcessNoticeFrameTrade( int controlID);
extern void ProcessNoticeFrameAskParty( int controlID);
extern void ProcessNoticeFrameInviteParty( int controlID);
extern void ProcessNoticeFrameAskBuddy( int controlID);
extern void ProcessNoticeFrameClan( int controlID);
extern void ProcessNoticeFrameBuyItem( int controlID);
extern void ProcessNoticeDeleteCharacter( int controlID);


/////////////////////////////////////////////////////////////////////////////////////////////////////
// PopMenu
/////////////////////////////////////////////////////////////////////////////////////////////////////
extern void ProcessPopMenuPc(LPARAM lParam);
extern void ProcessPopMenuNpc(LPARAM lParam);
extern void ProcessPopMenuPet(LPARAM lParam);
extern void ProcessPopMenuStone(LPARAM lParam);		// 문파 비석
extern void ProcessPopMenuOfficial(LPARAM lParam);	// 정사관 NPC
extern void ProcessPopMenuAlchemist(LPARAM lParam); // 연금술사 NPC
extern void ProcessPopMenuHelp(LPARAM lParam);		// 상서령 NPC
extern void ProcessPopMenuHelp2(LPARAM lParam);		// 성녀

// PopSubMenu
extern void ProcessPopMenuRelation( LPARAM lParam);
extern void ProcessPopMenuAI( LPARAM lParam);
extern void ProcessPopMenuSpecial( LPARAM lParam);
extern void ProcessPopMenuStoneSub1(LPARAM lParam);
extern void ProcessPopMenuStoneSub2(LPARAM lParam);
extern void ProcessPopMenuStoneSub3(LPARAM lParam);
extern void ProcessPopMenuPcTrade(LPARAM lParam);
extern void ProcessPopMenuLotto(LPARAM lParam);

extern LRESULT ProcessInterfaceMessage( WPARAM wParam,LPARAM lParam);


/////////////////////////////////////////////////////////////////////////////////////////////////////
// Manage Frame
/////////////////////////////////////////////////////////////////////////////////////////////////////
extern void ProcessEnterForChat();
extern void ProcessFocusOnChat();

extern void CloseAllWindow();
extern void CancelInterface();

extern void ProcessHideMainFrame();
extern void ProcessClickSackButton();
extern void ProcessClickCharInfoButton();
extern void ProcessClickMugongButton( BYTE byType);
extern void ProcessClickRelationButton( BYTE byType);

extern void ProcessClickHelperButton();//HO_0410_07 상서령 가이드 업데이트

extern void ProcessClickOptionButton();
extern void ProcessClickPetSackButton();
extern void ProcessClickPetInfoButton();
extern void ProcessClickQuestButton();
extern void ProcessClickItemConvert();
extern void ProcessClickCloseButton();

extern void ProcessClickCollection();

//각성
extern void ProcessWindowSkill( LPARAM lParam);