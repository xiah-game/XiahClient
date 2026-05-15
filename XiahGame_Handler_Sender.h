#pragma once

extern void SendCS_IT_LOGIN_AUTH_REQ(sString szAccountID, sString szPasswd);	// 인증서버로의 로그인
extern void SendCS_IT_LOGIN_REQ();										// 클라이언트 띄울때 (유닛서버로의 로그인)
extern void SendCS_IT_CHARACTERLIST_REQ();								// LOGIN_ACK() 받고
extern void SendCS_IT_NEWCHARACTER_REQ( BYTE byType, sString strUserName);	// 새 캐릭터 생성할때
extern void SendCS_IT_DELCHARACTER_REQ( DWORD dwCharID);
extern void SendCS_IT_CHARSTATUSINFO_REQ();								// STARTGAME_REQ() 받고
extern void SendCS_IT_MAPINFO_REQ(DWORD dwMapID);						// STARTGAME_REQ() 받고
extern void SendCS_IT_IMREADY_REQ( DWORD dwObjectID, DWORD dwMapID);	// Client가 캐릭터 다 만들고 맵 다 로딩했을때
extern void SendCS_IT_MUGONGLIST_REQ( BYTE bMugongType);
extern void SendCS_IT_ITEMLIST_REQ( BYTE bSackType);
extern void SendCS_IT_CHARINFO_REQ( DWORD dwObjectID);
extern void SendCS_IT_CHARSLOT_REQ();
extern void SendCS_IT_SETSLOT_REQ(DWORD dwValue, BYTE bSlot);
extern void SendCS_IT_SPEEDPING_REQ( DWORD dwClientTick);
extern void SendCS_IT_CHANGEPW_REQ(sString szAccountID, sString szPasswd, sString strChangePasswd);

extern void SendCS_NV_STARTGAME_REQ();									// GAME MAIN으로 들어갈때
extern void SendCS_NV_MAPENTER_REQ(DWORD dwObjectID,DWORD dwMapID);		// MAPINFO_REQ() 받고
extern void SendCS_NV_ENDGAME_REQ(BYTE  bChChange = 0);					// GAME을 종료할때
extern void SendCS_NV_MAPMOVE_REQ(DWORD dwMapID);
extern void SendCS_NV_MAPMOVE_REQ(DWORD dwMapID,WORD wPosX,WORD wPosY);	// 죽을때 보내는 MapMove

extern void SendCS_NV_SYNCMOVE_REQ( DWORD dwObjectID,WORD wPosX,WORD wPosY,BYTE bHeight,WORD wDesPosX,WORD wDesPosY,BYTE bDesHeight,WORD wDirection,BYTE bState,BYTE bWalkSpeed);
extern void SendCS_NV_ENDMOVE_REQ( DWORD dwObjectID,WORD wPosX,WORD wPosY,BYTE bHeight,BYTE bState);
extern void SendCS_NV_STARTMOVE_REQ( DWORD dwObjectID, WORD	wPosX, WORD	wPosY, BYTE	bHeight, WORD	wDesPosX, WORD	wDesPosY, BYTE	bDesHeight, WORD	wDirection, BYTE	bStatus, BYTE	bSpeed );
extern void SendCS_NV_PORTALMOVE_REQ( DWORD dwObjectID,WORD wPosX,WORD wPosY,BYTE bHeight,BYTE bState);
extern void SendCS_NV_QUICKMOVE_REQ(DWORD dwMoveMapID);					// NPC 포탈 이동
extern void SendCS_NV_PRIVATEPORTAL_REQ(DWORD dwMoveMapID);				// 문파대전 NPC 포탈 이동

extern void SendCS_BT_ATTACK_REQ( BYTE bAttackType,DWORD dwAttackID,WORD wAttackPosX,WORD wAttackPosY,BYTE bAttackHeight,BYTE bDefType,DWORD dwDefID,BYTE bAttackMode);
extern void SendCS_BT_PREATTACK_REQ( BYTE bAttackType,DWORD dwAttackID,BYTE bDefType,DWORD dwDefID,WORD wAttackPosX,WORD wAttackPosY,BYTE bAttackHeight,BYTE bAttackMode);
extern void SendCS_BT_EXECSP_REQ( BYTE bSpType, BYTE bSpValue);
extern void SendCS_BT_LEARNMUGONG_REQ( DWORD dwMugongID);
extern void SendCS_BT_SELMUGONG_REQ( BYTE bType , DWORD dwMugongID, BYTE bIndex);

extern void SendCS_BT_PRESHOT_REQ(BYTE bAtkType,DWORD dwAtkID,WORD wPosX,WORD wPosY,BYTE bHeight,WORD wDesPosX,WORD wDesPosY,BYTE bDesHeight,WORD wLifeTime,BYTE bAttackMode);
extern void SendCS_BT_SHOT_REQ(BYTE bAtkType,DWORD dwAtkID,WORD wPosX,WORD wPosY,BYTE bHeight,WORD wDesPosX,WORD wDesPosY,BYTE bDesHeight,WORD wLifeTime,BYTE bAttackMode);

extern void SendCS_BT_MUGONGPREATTACK_REQ(DWORD dwMugongID,BYTE bAttackType,DWORD dwAttackID,WORD wAttackPosX,WORD wAttackPosY,BYTE bAttackHeight,BYTE bDefenseType,DWORD dwDefenseID,WORD wTargetPosX,WORD wTargetPosY,BYTE bTargetHeight);
extern void SendCS_BT_MUGONGATTACK_REQ(DWORD dwMugongID,BYTE bAtkType,DWORD dwAtkID,WORD wAtkPosX,WORD wAtkPosY,BYTE bAtkHeight,BYTE bDefObjType,DWORD dwDefObjID,WORD wTargetPosX,WORD wTargetPosY,BYTE bTargetHeight);

extern void SendCS_BT_ASKPARTYBATTLE_REQ(BYTE bAction, DWORD dwAskPartyID, DWORD dwAskCharID, DWORD dwTargetPartylID, DWORD dwTargetCharID, DWORD dwBetMoney);

extern void SendCS_NC_NPCINFO_REQ( DWORD dwObjectID);
extern void SendCS_NC_PETINFO_REQ( DWORD dwObjectID);
extern void SendCS_NC_FUNCTIONALNPCITEMLIST_REQ( DWORD dwMapID, DWORD dwObjectID, BYTE bSackCnt);
extern void SendCS_NC_STARTMOVE_REQ( DWORD dwObjectID, WORD wPosX, WORD wPosY,BYTE bHeight,WORD wDesPosX,WORD wDesPosY,BYTE bDesHeight,WORD wDirection,BYTE bState,BYTE bSpeed);
extern void SendCS_NC_SYNCMOVE_REQ( DWORD dwObjectID, WORD wPosX, WORD wPosY, BYTE bHeight, WORD wDesPosX, WORD wDesPosY,BYTE bDesHeight,WORD wDirection,BYTE bState,BYTE bSpeed);
extern void SendCS_NC_ENDMOVE_REQ(DWORD dwObjectID, WORD wPosX,WORD wPosY,BYTE bHeight,WORD wDirection,BYTE bState,BYTE bSpeed);
extern void SendCS_NC_PETDETAILINFO_REQ(DWORD dwObjectID);
extern void SendCS_NC_TAMING_REQ(DWORD dwTargetID,BYTE bTamingType);
extern void SendCS_NC_MAPENTER_REQ(DWORD dwObjectID,DWORD dwMapID);
extern void SendCS_NC_STATUSCHANGE_REQ(BYTE bObjectType,DWORD dwObjectID,BYTE bStatus,WORD wDirection);
extern void SendCS_NC_PETRENAME_REQ( DWORD dwMapID, DWORD dwObjectID, sString szPetName);
extern void SendCS_NC_PETBONGIN_REQ( DWORD dwObjectID, BYTE bSackID, BYTE bSackPos);
extern void SendCS_NC_PETBONGOUT_REQ( BYTE bSackID, BYTE bSackPos);
extern void SendCS_NC_PETSACKLIST_REQ( DWORD dwOwnerID, DWORD dwPetID, BYTE bSackID);
extern void SendCS_NC_PETITEMPUT_REQ( DWORD dwPetID, DWORD dwItemID, BYTE bCharSackID, BYTE bCharSackPos, BYTE bPetSackID, BYTE bPetSackPos);
extern void SendCS_NC_PETITEMOUT_REQ( DWORD dwPetID, DWORD dwItemID, BYTE bPetSackID, BYTE bPetSackPos, BYTE bCharSackID, BYTE bCharSackPos);
extern void SendCS_NC_PETITEMMOVE_REQ( DWORD dwPetID, DWORD dwItemID, BYTE bSrcID, BYTE bSrcPos, BYTE bDesID, BYTE bDesPos);
extern void SendCS_NC_PETPICKITEM_REQ( DWORD dwMapID, DWORD dwPetID, DWORD dwItemID, WORD wPosX, WORD wPosY, BYTE bSackID, BYTE bSackPos, DWORD dwMapItemID, DWORD dwMapAmount);
extern void SendCS_NC_PETTHROWITEM_REQ( DWORD dwPetID, DWORD dwItemID, BYTE bSackID, BYTE bSackPos, WORD wPosX, WORD wPosY, BYTE bHeight, DWORD dwAmount);
extern void SendCS_NC_PETRESTORE_REQ(DWORD dwPetID, BYTE bSackID, BYTE bSackPos);
extern void SendCS_NC_PREPETTRADE_REQ(DWORD dwAskedID, DWORD dwPetID);
extern void SendCS_NC_PETTRADE_REQ(BYTE bResult, DWORD dwAskID, DWORD dwAskedID, DWORD dwPetID, DWORD dwPrice);

extern void SendCS_IM_MAPITEMINFO_REQ( DWORD dwObjectID);
extern void SendCS_IM_PICK_REQ( DWORD dwMapID, DWORD dwItemID, WORD wPosX, WORD wPosY, BYTE bSackPos, DWORD dwObjectID, DWORD dwAmount);
extern void SendCS_IM_MOVE_REQ( BYTE bSrcSackID, BYTE bSrcSackPos, DWORD dwSrcObjID, BYTE bDesSackID, BYTE bDesSackPos, DWORD dwDesObjID);
extern void SendCS_IM_THROW_REQ( BYTE bSackID, BYTE bSackPos, DWORD dwItemID, WORD wPosX, WORD wPosY, BYTE bHeight, DWORD dwAmount);
extern void	SendCS_IM_USEITEM_REQ( BYTE bSackID, BYTE bSackPos, DWORD dwItemID);
extern void SendCS_IM_THROWMONEY_REQ( DWORD dwObjectID, WORD wPosX, WORD wPosY, BYTE bHeight, DWORD dwAmount);
extern void SendCS_IM_MERGERES_REQ( BYTE bSrcSackID, BYTE bSrcPos, DWORD dwSrcObjectID, BYTE bDesSackID, BYTE bDesPos, DWORD dwDestObjectID);
//extern void SendCS_IM_CHECKITEMPRICE_REQ( BYTE bType, DWORD dwOwnerID, DWORD dwItemID, BYTE bSackID, BYTE bSackPos);
extern void SendCS_IM_REPAIRITEM_REQ( DWORD dwItemID, BYTE bSackID, BYTE bSackPos);
extern void SendCS_IM_REBUILDITEMTERM_REQ( DWORD dwItemID, BYTE bSackID, BYTE bSackPos);
extern void SendCS_IM_REBUILDITEM_REQ( DWORD dwShopID, DWORD dwItemID, BYTE bSackID, BYTE bSackPos, DWORD dwResourceID1, BYTE bResourceSackID1, BYTE bResourcePos1, DWORD dwResourceID2, BYTE bResourceSackID2, BYTE bResourcePos2, DWORD dwResourceID3, BYTE bResourceSackID3, BYTE bResourcePos3);
extern void SendCS_IM_SPLITRES_REQ( BYTE bSrcSackID, BYTE bSrcPos, DWORD dwSrcObjID, BYTE bDesSackID, BYTE bDesPos, DWORD dwAmount);
extern void SendCS_IM_GIVEITEM_REQ( BYTE bSackID, BYTE bSackPos, DWORD dwItemID, BYTE bObjectType, DWORD dwObjectID);
// 
extern void SendCS_IM_REPAIRWITHITEM_REQ(DWORD dwResItemID, BYTE bResSackID, BYTE bResSackPos, DWORD dwTarItemID, BYTE bTarSackID, BYTE bTarSackPos);
extern void SendCS_IM_REMARKITEM_REQ(DWORD dwItemID, BYTE bSackID, BYTE bSackPos);
extern void SendCS_IM_MONEYBAG_REQ(BYTE bAction, DWORD dwItemID, BYTE bSackID, BYTE bSackPos, DWORD dwMoney);

// MEMO or MAIL
extern void SendCS_IM_SENDMEMO_REQ(sString szCharName,sString szTitle,sString szContents,BYTE bSackID, BYTE bSackPos);
extern void SendCS_IM_READMEMO_REQ(DWORD dwMemoID);
extern void SendCS_IM_DELETEMEMO_REQ(DWORD dwMemoID);

extern void SendCS_IM_PUZZLEITEM_REQ(DWORD dwShopID);
extern void SendCS_IM_REJOINITEM_REQ(DWORD dwShopID);

extern void SendCS_IM_VARIENTITEM_REQ(BYTE bVarientType);						// 개조 (떡국, 변종 자원)
extern void SendCS_IM_REWARDGUARANTEE_REQ(BYTE bRewardType, DWORD dwItemID);	// 보험 아이템 복구,소멸
extern void SendCS_IM_EVENTPUZZLE_REQ(DWORD dwFNpcID);							// 이미지 조합
extern void SendCS_IM_MIXITEM_REQ(DWORD dwShopID);
extern void SendCS_IM_MAKEREPAIRHAMMER_REQ(DWORD dwShopID);						// 망치 조합

//HT_CHEAT : 변종 패킷 추가
extern void SendCS_IM_VARIENTITEM_HT_REQ(DWORD dwitemID, BYTE bSackIDPrev, BYTE bSackPosPrev);			// 행낭에서 바로 개조하기 개조 

// 아이템 수집
extern void SendCS_IM_MOVEINCOLLECTITEM_REQ(BYTE bSackID, BYTE bSackPos, DWORD dwItemID, BYTE bCollectSackPos);
extern void SendCS_IM_MOVEOUTCOLLECTITEM_REQ(BYTE bCollectSackPos, DWORD dwItemID, BYTE bSackID, BYTE bSackPos);

extern void SendCS_IM_MAKEUNIONITEM_REQ(DWORD dwShopID);						//사신셋 
extern void SendCS_IM_REBIRTH_REQ(BYTE bSackID, BYTE bSackPos, DWORD dwItemID);	//각성제 사용
//HT_1116 : 각성제 아이템 추가
extern void SendCS_IM_MAKEREBIRTHITEM_REQ();

extern void SendCS_EC_BUYITEM_REQ( DWORD dwShopID, DWORD dwItemID, DWORD dwAmount, BYTE bShopSackCnt, BYTE bShopSackPos, BYTE bCharSackCnt, BYTE bCharSackPos);
extern void SendCS_EC_SELLITEM_REQ( DWORD dwShopID, DWORD dwItemID, BYTE bSackID, BYTE bSackPos);
extern void	SendCS_EC_ASKTRADE_REQ(BYTE bResult, DWORD dwAskID, DWORD dwAskedID);
extern void	SendCS_EC_TRADESACKONITEM_REQ(BYTE bSrcSackID, BYTE bSrcPos, BYTE bDesSackID, BYTE bDesPos, DWORD dwItemID, DWORD dwAmount,DWORD dwTraderid);
extern void SendCS_EC_TRADESACKOFFITEM_REQ(BYTE bSrcSackID, BYTE bSrcPos, BYTE bDesSackID, BYTE bDesPos, DWORD dwItemID, DWORD dwAmount);
extern void	SendCS_EC_TRADEITEM_REQ(BYTE bResult, DWORD dwTraderID);
extern void SendCS_EC_TRADESACKONMONEY_REQ( DWORD dwTraderID, DWORD dwAmount);
extern void SendCS_EC_TRADESACKOFFMONEY_REQ( DWORD dwTraderID, DWORD dwAmount);

extern void SendCS_EC_ITEMLISTINBANK_REQ( DWORD dwCharID);

extern void SendCS_EC_DRAWINBANK_REQ( DWORD dwCharID, DWORD dwItemID, BYTE bSackID, BYTE bSackPos, BYTE bBankPos, DWORD dwAmount);
extern void SendCS_EC_DRAWOUTBANK_REQ( DWORD dwCharID, DWORD dwItemID, BYTE bBankPos, BYTE bSackID, BYTE bSackPos, DWORD dwAmount);
extern void SendCS_EC_DRAWMOVEBANK_REQ( BYTE bSrcPos, DWORD dwSrcItemID, BYTE bDesPos, DWORD dwDesItemID);

// ITEM MALL
extern void SendCS_EC_ITEMLISTINMALL_REQ(DWORD dwCharID);
extern void SendCS_EC_DRAWOUTMALL_REQ(DWORD dwCharID, DWORD dwItemID, BYTE bBankPos, BYTE bSackID, BYTE bSackPos, DWORD dwAmount);
extern void SendCS_EC_DRAWMOVEMALL_REQ( BYTE bSrcPos, DWORD dwSrcItemID, BYTE bDesPos, DWORD dwDesItemID);

// 복권
extern void SendCS_EC_BUYLOTTO_REQ(BYTE bNum1, BYTE bNum2, BYTE bNum3, BYTE bNum4);
extern void SendCS_EC_LOTTOSALEINFO_REQ();
extern void SendCS_EC_PRIZELOTTOINFO_REQ(BYTE bNowLotto);
extern void SendCS_EC_GETLOTTOMONEY_REQ(DWORD dwItemID, BYTE bSackID, BYTE bSackPos, DWORD dwMoney);
extern void SendCS_EC_CHECKLOTTO_REQ(DWORD dwItemID, BYTE bSackID, BYTE bSackPos);

extern void SendCS_EC_GUARANTEELIST_REQ();									// 보험 아이템
extern void SendCS_EC_QUICKMART_REQ();										// 매품패


// 파티
extern void	SendCS_IF_ASKPARTY_REQ(DWORD dwAskID, DWORD dwAskedID, BYTE bResult, BYTE bPartyType = 0);
extern void	SendCS_IF_INVITEPARTY_REQ(DWORD dwAskID, DWORD dwAskedID, BYTE bResult, BYTE bPartyType = 0);
extern void	SendCS_IF_LEAVEPARTY_REQ(DWORD dwPartyID);
extern void	SendCS_IF_PARTYPOSITION_REQ(DWORD dwPartyID, DWORD dwCharID);
extern void	SendCS_IF_BANISHPARTY_REQ(DWORD dwPartyID, DWORD dwBanishCharID);
extern void SendCS_IF_PARTYSHARE_REQ(BYTE byExpDivision, BYTE byFEDivision);	// 단 경험치 분배 선택
// 친구
extern void SendCS_IF_ASKADDBUDDY_REQ(DWORD dwAskID, DWORD dwAskedID, BYTE bResult);
extern void SendCS_IF_DELBUDDY_REQ( DWORD dwBuddyID);
extern void SendCS_IF_BUDDYLIST_REQ();
extern void SendCS_IF_BUDDYPOSITION_REQ( DWORD dwBuddyID);
// 애완동물
extern void SendCS_IF_PETLIST_REQ(DWORD dwCharID);
// 오행
extern void SendCS_IF_EXECFIVEELM_REQ(BYTE byFiveElme);							// 오행수치 올리기
extern void SendCS_IF_CHANGEFIVEELM_REQ(BYTE byFiveElme);						// 오행 선택
//HT_0720 : 오행 개선 사항
extern void SendCS_IF_ENDFIVEELM_REQ();											// 오행 종료 

extern void SendCS_IF_EXECSTAMINA_REQ();										// 기 발동


extern void SendCS_RL_CREATEMUNPA_REQ( sString szMunpaName);
extern void SendCS_RL_DELETEMUNPA_REQ();
extern void SendCS_RL_ASKMUNWON_REQ( BYTE bResult, DWORD dwAskID, DWORD dwAskedID);
extern void SendCS_RL_DELMUNWON_REQ( DWORD dwCharID, DWORD dwOrderID);
extern void SendCS_RL_MUNWONINFO_REQ( DWORD dwMunwonID);
extern void SendCS_RL_MUNWONLIST_REQ();
extern void SendCS_RL_CHANGEMUNWONORDER_REQ( DWORD dwMunwonID, DWORD dwOldOrderID, DWORD dwNewOrderID);
extern void SendCS_RL_MUNPAINFO_REQ(BYTE bType, DWORD dwMunpaID);
extern void SendCS_RL_MUNPACHAT_REQ( BYTE bType, sString szMsg);
extern void SendCS_RL_MUNPANICK_REQ( DWORD dwMunpaID, DWORD dwCharID, sString szNickName);
extern void SendCS_RL_ASKRELATION_REQ(BYTE bRelType, BYTE bRelStep, DWORD dwAskCharID, DWORD dwAnsCharID);
extern void SendCS_RL_BREAKRELATION_REQ(BYTE bRelKind, BYTE bRelStep, DWORD dwAskCharID, DWORD dwAnsCharID);

extern void SendCS_RL_DONATE_REQ(DWORD dwMunpaID, DWORD dwReqFame, DWORD dwDonateMoney);
extern void SendCS_RL_PREDONATE_REQ(DWORD dwMunpaID, DWORD dwReqFame);
extern void SendCS_RL_GAINSTONE_REQ(DWORD dwMunpaID, DWORD dwStoneID);
extern void SendCS_RL_MUNPABBSLIST_REQ(DWORD dwMunpaID);
extern void SendCS_RL_MUNPABBSREAD_REQ(DWORD dwMunpaID, DWORD dwBBSID);
extern void SendCS_RL_MUNPABBSWRITE_REQ(DWORD dwMunpaID, LPCTSTR strTitle, LPCTSTR strContents);
extern void SendCS_RL_MUNPABBSDEL_REQ(DWORD dwMunpaID, DWORD dwBBSID);

extern void SendCS_RL_MUNPANOTICE_REQ(sString strNotice);
extern void SendCS_RL_MUNPAMARKREG_REQ(BYTE bType, DWORD dwMunpaID, LPCTSTR lpstrImage, DWORD dwStoneID);
extern void SendCS_RL_GAINMARKIMAGE_REQ(DWORD dwMarkID);
extern void SendCS_RL_GETMUNPAMONEY_REQ(DWORD dwMunpaID, DWORD dwTaxMunpaMoney, DWORD dwStoneID);

// 2004_05_31 새로워진 관계
extern void SendCS_RL_RELATIONLIST_REQ();
extern void SendCS_RL_ADDRELATION_REQ(BYTE bType, DWORD dwCharID, LPCSTR strNick, BYTE bConnect);
extern void SendCS_RL_DELRELATION_REQ(BYTE bType, DWORD dwCharID);
extern void SendCS_RL_CHGRELATION_REQ(DWORD dwCharID, BYTE bWorldID, DWORD dwMapID, BYTE bConnect);

// 절연부
extern void SendCS_RL_BREAKRELATIONITEM_REQ(BYTE bSackID, BYTE bSackPos, DWORD dwItemID, BYTE bRelType, DWORD dwTargetID);

extern void SendCS_QS_LIST_REQ();
extern void SendCS_QS_START_REQ( DWORD dwQuestID);
extern void SendCS_QS_STOP_REQ( DWORD dwQuestID);
extern void SendCS_QS_DELETE_REQ(DWORD dwQuestID);

extern void SendCS_WR_PRECHALLENGEWAR_REQ(DWORD dwMunpaID);
extern void SendCS_WR_CHALLENGEWAR_REQ(DWORD dwMunpaID, DWORD dwGameTime, BYTE bStealStone);
extern void SendCS_WR_STONEDELETE_REQ(DWORD dwMunpaID);
extern void SendCS_WR_APPLYWAR_REQ(DWORD dwFNpcID);			// 문파대전 참가 신청
extern void SendCS_WR_REWARD_REQ(BYTE bType);				// 문파대전 보상

extern void SendCS_CH_CHAT_REQ( BYTE type, DWORD listener, sString content, sString szNickName);

// OPTION
extern void SendCS_OP_OPTIONLIST_REQ();
// 옵션 패킷 변경
extern void SendCS_OP_CHANGE_REQ(DWORD dwCharID, BYTE v1, BYTE v2, BYTE v3,BYTE bSafe, DWORD dwBuyLimit, BYTE bRarityLimit, BYTE bStxTypeLimit);

// CASUAL
extern void SendCS_ACTION_REQ(BYTE bAnitype, BYTE bAniKind, WORD wDirection, DWORD dwObjectID = 0);

// 상점
extern void SendCS_SH_SETSHOP_REQ(LPCTSTR strName, LPCTSTR strDescription);
extern void SendCS_SH_MOVESHOP_REQ(BYTE bSackPos, DWORD dwSrcItemID, BYTE bDesSackPos, DWORD dwDesItemID);
extern void SendCS_SH_REGSHOP_REQ(BYTE bSackID, BYTE bSackPos, DWORD dwItemID, BYTE bShopSackPos, DWORD dwPrice);
extern void SendCS_SH_DELSHOP_REQ(BYTE bShopSackPos, DWORD dwItemID, BYTE bSackID, BYTE bSackPos);
extern void SendCS_SH_GETMONEY_REQ(DWORD dwMoney);
extern void SendCS_SH_GETSHOPINFO_REQ(DWORD dwCharID);
extern void SendCS_SH_BUYPCSHOP_REQ(DWORD dwCharID, BYTE bSrcSackPos, DWORD dwItemID, BYTE bSackID, BYTE bSackPos, DWORD dwPrice);
extern void SendCS_SH_STATUSCHANGE_REQ(BYTE bStatus);

//HT_0313 : 광명전 & 천황전
extern void SendCS_WR_APPLYSECRET_REQ();
extern void SendCS_WR_APPLYDEVIL_REQ();
extern void SendCS_WR_APPLYDEVILREADY_REQ();
extern void SendCS_WR_APPLYSECRETREADY_REQ();
extern void SendCS_NV_SECRETADVENTURE_REQ();
extern void SendCS_NV_DEVILADVENTURE_REQ();

//HT_0423 : 단주 위임
extern void SendCS_IF_CHANGEPARTYLEADER_REQ(DWORD dwCurLeaderID, DWORD dwPostLeaderID);
