

//////////////////////////////////////////
// Notice
//////////////////////////////////////////
#define NOTICE_FRAME_OK						56
#define NOTICE_FRAME_OKCANCEL				55

// 1button
#define NOTICE_FRAME_UNEXPECTED_TERMINATE	1
// 2button
#define NOTICE_FRAME_TRADE					1	// 거래
#define NOTICE_FRAME_ASKPARTY				2	// 일반단
#define NOTICE_FRAME_INVITEPARTY			3
#define NOTICE_FRAME_ASKBUDDY				4
#define NOTICE_FRAME_CLAN					5
#define NOTICE_FRAME_BUYITEM				6
#define NOTICE_FRAME_DELETE_CHARACTER		7
//#define NOTICE_FRAME_CHECKPRICE			8
#define NOTICE_FRAME_PAMUN					8
#define NOTICE_FRAME_JEMYUNG				9
#define NOTICE_FRAME_CLOSE_CLAN				10
#define NOTICE_FRAME_FOUNT					11
#define NOTICE_FRAME_QUEST_DEL				12
#define NOTICE_FRAME_DAN_WAR 				13
#define NOTICE_FRAME_DAN_WAR_ASK			14

#define NOTICE_FRAME_STONE_OWN				15	// 문파 비석 소유
#define NOTICE_FRAME_STONE_RESIGN			16	// 문파 비석 포기
#define NOTICE_FRAME_AFFINITY				17
#define NOTICE_FRAME_ASKRELATION			18	// 관계
#define NOTICE_FRAME_PT_BUY					19	// 개인 노점 구입 횅땍
#define NOTICE_FRAME_MAIL_DELETE			20	// 전서 삭제 여부 횅땍
#define NOTICE_FRAME_MAIL_SEND				21	// 전서 전송 여부 횅땍
#define NOTICE_FRAME_PET_REVIVAL			22	// 펫 부활 여부
#define NOTICE_FRAME_PET_BONGIN 			23	// 빙정에 펫 봉인 여부

#define NOTICE_FRAME_RELATION_ASKPARTY		24	// 관계단
#define NOTICE_FRAME_RELINQUISH				25	// 문주 이양
#define NOTICE_FRAME_LEAVE					26	// 문파 탈퇴
#define NOTICE_FRAME_MARK_DEL				27	// 문파문장 삭제 횅땍

// 거래 횅땍
#define NOTICE_FRAME_LIMIT_BUY				28	// 일반 구매 제한시
#define NOTICE_FRAME_LIMIT_PT_BUY			29	// 개인 노점 구매 제한시
#define NOTICE_FRAME_LIMIT_SELL				30	// 판매 제한시

// 아이템 복구
#define NOTICE_FRAME_RECOVERY1				31	// 아이템 복구
#define NOTICE_FRAME_RECOVERY2				32	// 아이템 복구 - 소멸시

// 절연부
#define NOTICE_FRAME_SEVER_RELATION1		33	// 절연부 선택
#define NOTICE_FRAME_SEVER_RELATION2		34	// 절연부 횅땍

// 문파대전
#define NOTICE_FRAME_CLAN_WAR_APPLY			35	// 문파대전 참여 신청
#define NOTICE_FRAME_CLAN_WAR_REWARD		36	// 문파대전 우승 상금

#define NOTICE_FRAME_MUGONG					37	//HO_0329_07 무공 습득 여부 추가

//#define NOTICE_FRAME_REBIRTH				37  // 각성제 사용
//#define NOTICE_FRAME_REBIRTH_SUCCESS		38	// 각성제 사용 성공

#define NOTiCE_FRAME_DANCOMMIT				39	//HT_0423 : 단주 위임

#define NOTICE_FRAME_ENDGAME				40  //HO_0816_07 종료 기능 추가
#define NOTICE_FRAME_ITEMDROP				41  //HO_0816_07 아이템 드랍시 횅땍





//////////////////////////////////////////////
// PopMenu
//////////////////////////////////////////////

// 링메뉴 프레임 아이디
#define FRAMEID_PC			10001	// 대화, 거래, 관계
#define FRAMEID_NPC			10002	// 수리, 거래, 개조
#define FRAMEID_PET			10003	// 상태, 특수, AI
#define FRAMEID_STONE		10004	// 문파전 비석
#define FRAMEID_OFFICIAL	10005	// 정사관 NPC
#define FRAMEID_ALCHEMIST	10006	// 연금술사 NPC ( 대화, 조합, 제련 )
#define FRAMEID_HELP		10007	// 도우미 NPC (상서령)
#define FRAMEID_NPC_PORTAL	10008	// NPC 포탈 이동 상서령
#define FRAMEID_HELP_2		10009	// 성녀
#define FRAMEID_CLAN_WAR	10010	// 문파대전 관리인
#define FRAMEID_REBIRTHITEM	10011	//HT_1116 : 각성자 아이템 추가

// 링서브메뉴 프레임 아이디
#define FRAMEID_PC_RELATION	20004	// from FRAMEID_NPC
#define FRAMEID_PET_AI		20005	// from FRAMEID_PET
#define FRAMEID_PET_SPECIAL	20006
#define FRAMEID_STONE_SUB_1	20007	// 문파전 비석 서브 관리
#define FRAMEID_STONE_SUB_2	20008	// 문파전 비석 서브 경제
#define FRAMEID_STONE_SUB_3	20009	// 문파전 비석 서브 신청
#define FRAMEID_PC_TRADE 	20010	// 거래 선택 서브	FRAMEID_PC_RELATION => FRAMEID_PC_TRADE
#define FRAMEID_LOTTO		20011	// 복권 서브
#define FRAMEID_SECRET		20012	//HT_0313 : 광명전 & 천황전 

// 단독적인 링서브메뉴 프레임 아이디(콤보메뉴)
#define FRAMEID_BONBINITEM	30004	// 봉인 아이템 눌렀을때
#define FRAMEID_WAR_DAY		30005
#define FRAMEID_STONE_MOVE	30006
#define FRAMEID_PURSE		30014	// 아이템몰 전낭
#define FRAMEID_REVIVAL		30015	// 펫 복구 리스트
#define FRAMEID_CRYOLITE	30016	// 빙정 봉인
#define FRAMEID_LOTTOCHECK	30017	// 복권아이템 당첨횅땍/당첨금수령



/////////////////////////////////////////////////////////////////////////////////////////////////////
// 리소스 ID
// FRAMEID_PC의 리소스 아이디
#define RESID_COMMUNICATION	455
#define RESID_TRADE			459
#define RESID_RELATION		463
// FRAMEID_NPC의 리소스 아이디
#define RESID_REPAIR		467
#define RESID_TEACHER		483		// 문파전 신청
#define RESID_MODIFY		857

#define RESID_ITEMMALL		1086

#define RESID_QUEST_COMM	1097
#define RESID_QUEST_FIGHT	1101

#define RESID_ACQUIT		1186	// 면죄
#define RESID_DONATE		1190	// 기부
#define RESID_ADMINISTER	1194	// 관리
#define RESID_LOTTO			1278	// 복권
#define RESID_MIXTURE		1292	// 조합
#define RESID_REFINE		1296	// 제련
#define RESID_RECORD		483		// 등록	


// FRAMEID_PET의 리소스 아이디
#define RESID_STATUS		837
#define RESID_SPECIAL		841
#define RESID_AI			861

// FRAMEID_POPUP_SUBMENU의 리소스
#define FRAMEID_POPUP_SUBMENU 865
/*//아래는 안쓰임
// FRAMEID_PC_RELATION의 리소스
#define RESID_DAN			471
#define RESID_ADOPT			475
#define RESID_FRIEND		479
#define RESID_WEDDING		487
#define RESID_CLAN			491

*/
// COMBO_POPUP 리소스
#define RESID_COMBO			964
#define RESID_COMBO_2		1183



/////////// 아이콘 리소스 아이디
#define RESID_ICON_HAT		885
#define RESID_ICON_NECK		886
#define RESID_ICON_WEAPON	887
#define RESID_ICON_RING		888
#define RESID_ICON_SHOE		889
#define RESID_ICON_CLOTH	890


/////////////////////////////////////////////////////////////////////////////////////////////////////
// Event Type
// 각적 인터페이스 - 문파공지
#define GAK_MSG_WINDOW_MSG		0
#define GAK_MSG_WINDOW_MUNPA	1

#define WINDOW_MONEY_PCTRADE	1	// 개인노점 가격설정
#define WINDOW_MONEY_LOTTO		2	// 복권 당첨금액 수령

#define WINDOW_NPC_PORTAL		1	// NPC 포탈 이동 상서령
#define WINDOW_NPC_PORTAL__WAR	2	// NPC 포탈 이동(문파대전시-문파대전 관리인)