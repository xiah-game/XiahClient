#include "precompile.h"
#include "XiahNetworkHandler.h"
#include "XiahSocket.h"

#include "resource.h"
#include "AppData.h"
#include "XiahArrayIndex.h"

#include "XiahObject.h"
#include "XiahGameObject.h"
#include "XiahObjectType.h"

#include "ItemInfo.h"
#include "XiahMap.h"
#include "FunctionalNpcInfo.h"
#include "PCVisualInfo.h"
#include "CharSack.h"

#include "XiahGame_Main.h"
#include "XiahGame_Intro.h"
#include "XiahGame_Handler_Sender.h"

#include "InterfaceDefine.h"
#include "InterfaceHandler.h"
#include "XiahCamera.h"
#include "Fade.h"
#include "XiahGame_Minimap.h"
#include "XiahGame_Bgm.h"

#include "XiahGame_Pet.h"

extern BOOL SetupPC_VisualEquipement(CXiahCharObject* pObject, WORD* pVisualList, BYTE* pRarityList=NULL, BYTE* pStxTypeList=NULL);
extern bool SetupPET_VisualEquipement(CXiahCharObject* pObject, WORD* pVisualList);








/////////////////////////////////////////////////////////////
// Regist Receiver
/////////////////////////////////////////////////////////////

#include "CXiahGame_Login.h"

#include "XiahGame_Handler_IT_Rcv.cpp"
#include "XiahGame_Handler_NV_Rcv.cpp"
#include "XiahGame_Handler_BT_Rcv.cpp"
#include "XiahGame_Handler_NC_Rcv.cpp"
#include "XiahGame_Handler_IF_Rcv.cpp"
#include "XiahGame_Handler_IM_Rcv.cpp"
#include "XiahGame_Handler_CD_Rcv.cpp"
#include "XiahGame_Handler_EC_Rcv.cpp"
#include "XiahGame_Handler_CH_Rcv.cpp"
#include "XiahGame_Handler_EV_Rcv.cpp"
#include "XiahGame_Handler_RL_Rcv.cpp"
#include "XiahGame_Handler_OP_Rcv.cpp"
#include "XiahGame_Handler_QS_Rcv.cpp"
#include "XiahGame_Handler_WR_Rcv.cpp"

#include "XiahGame_Handler_SH_Rcv.cpp"

void RegisterNetworkHandler_Main()
{
	XiahNetwork::RegisterHandler( CS_IT_LOGIN_ACK,					OnCS_IT_LOGIN_ACK);
	XiahNetwork::RegisterHandler( CS_IT_LOGINCHECK_ACK,					OnCS_IT_LOGINCHECK_ACK);

	XiahNetwork::RegisterHandler( CS_IT_WORLDLIST_ACK,				OnCS_IT_WORLDLIST_ACK);
	XiahNetwork::RegisterHandler( CS_IT_WORLDSTATE_ACK,				OnCS_IT_WORLDSTATE_ACK);
	XiahNetwork::RegisterHandler( CS_IT_CHARACTERLIST_ACK,			OnCS_IT_CHARACTERLIST_ACK);
	XiahNetwork::RegisterHandler( CS_IT_NOTICE_ACK,					OnCS_IT_NOTICE_ACK);
	XiahNetwork::RegisterHandler( CS_IT_NEWCHARACTER_ACK,			OnCS_IT_NEWCHARACTER_ACK);
	XiahNetwork::RegisterHandler( CS_IT_DELCHARACTER_ACK,			OnCS_IT_DELCHARACTER_ACK);
	XiahNetwork::RegisterHandler( CS_IT_CHARSTATUSINFO_ACK, 		OnCS_IT_CHARSTATUSINFO_ACK);
	XiahNetwork::RegisterHandler( CS_IT_MAPINFO_ACK,				OnCS_IT_MAPINFO_ACK);
	XiahNetwork::RegisterHandler( CS_IT_GENERALMUGONGLIST_ACK,		OnCS_IT_GENERALMUGONGLIST_ACK);
	XiahNetwork::RegisterHandler( CS_IT_PASSIVEMUGONGLIST_ACK,		OnCS_IT_PASSIVEMUGONGLIST_ACK);
	XiahNetwork::RegisterHandler( CS_IT_ACTIVEMUGONGLIST_ACK,		OnCS_IT_ACTIVEMUGONGLIST_ACK);
	XiahNetwork::RegisterHandler( CS_IT_ITEMLIST_ACK,				OnCS_IT_ITEMLIST_ACK);
	XiahNetwork::RegisterHandler( CS_IT_IMREADY_ACK,				OnCS_IT_IMREADY_ACK);
	XiahNetwork::RegisterHandler( CS_IT_CHARINFO_ACK,				OnCS_IT_CHARINFO_ACK);
	XiahNetwork::RegisterHandler( CS_IT_CHARINFOLIST_ACK,			OnCS_IT_CHARINFOLIST_ACK);
	XiahNetwork::RegisterHandler( CS_IT_CHARSLOT_ACK,				OnCS_IT_CHARSLOT_ACK);
	XiahNetwork::RegisterHandler( CS_IT_SETSLOT_ACK,				OnCS_IT_SETSLOT_ACK);
	XiahNetwork::RegisterHandler(CS_IT_CHANGEPW_ACK,				OnCS_IT_CHANGEPW_ACK);

	XiahNetwork::RegisterHandler( CS_NV_STARTGAME_ACK,				OnCS_NV_STARTGAME_ACK);
	XiahNetwork::RegisterHandler( CS_NV_ENDGAME_ACK,				OnCS_NV_ENDGAME_ACK);
	XiahNetwork::RegisterHandler( CS_NV_MAPOBJECTLIST_ACK,			OnCS_NV_MAPOBJECTLIST_ACK);
	XiahNetwork::RegisterHandler( CS_NV_MAPENTER_ACK,				OnCS_NV_MAPENTER_ACK);
	XiahNetwork::RegisterHandler( CS_NV_MAPMOVE_ACK,				OnCS_NV_MAPMOVE_ACK);
	XiahNetwork::RegisterHandler( CS_NV_MAPLEAVE_ACK,				OnCS_NV_MAPLEAVE_ACK);
	XiahNetwork::RegisterHandler( CS_NV_STARTMOVE_ACK,				OnCS_NV_STARTMOVE_ACK);
	XiahNetwork::RegisterHandler( CS_NV_SYNCMOVE_ACK,				OnCS_NV_SYNCMOVE_ACK);
	XiahNetwork::RegisterHandler( CS_NV_ENDMOVE_ACK,				OnCS_NV_ENDMOVE_ACK);
	XiahNetwork::RegisterHandler( CS_NV_SETPOSITION_ACK,			OnCS_NV_SETPOSITION_ACK);
	XiahNetwork::RegisterHandler(CS_NV_QUICKMOVE_ACK,				OnCS_NV_QUICKMOVE_ACK);		// NPC 포탈 이동
	XiahNetwork::RegisterHandler(CS_NV_PRIVATEPORTAL_ACK,			OnCS_NV_PRIVATEPORTAL_ACK);	// 문파대전 NPC 포탈 이동	

	XiahNetwork::RegisterHandler( CS_BT_CHANGEMODE_ACK,				OnCS_BT_CHANGEMODE_ACK);
	XiahNetwork::RegisterHandler( CS_BT_PREATTACK_ACK,				OnCS_BT_PREATTACK_ACK);	
	XiahNetwork::RegisterHandler( CS_BT_ATTACK_ACK,					OnCS_BT_ATTACK_ACK);	
	XiahNetwork::RegisterHandler( CS_BT_NPCPREATTACK_ACK,			OnCS_BT_NPCPREATTACK_ACK);
	XiahNetwork::RegisterHandler( CS_BT_NPCATTACK_ACK	,			OnCS_BT_NPCATTACK_ACK);	
	XiahNetwork::RegisterHandler( CS_BT_NPCPRESHOT_ACK	,			OnCS_BT_NPCPRESHOT_ACK);	
	XiahNetwork::RegisterHandler( CS_BT_NPCSHOT_ACK		,			OnCS_BT_NPCSHOT_ACK);	
	XiahNetwork::RegisterHandler( CS_BT_EXECSP_ACK,					OnCS_BT_EXECSP_ACK);
	XiahNetwork::RegisterHandler( CS_BT_LEARNMUGONG_ACK,			OnCS_BT_LEARNMUGONG_ACK);
	XiahNetwork::RegisterHandler( CS_BT_SELMUGONG_ACK,				OnCS_BT_SELMUGONG_ACK);

	XiahNetwork::RegisterHandler( CS_BT_PRESHOT_ACK,				OnCS_BT_PRESHOT_ACK);
	XiahNetwork::RegisterHandler( CS_BT_SHOT_ACK,					OnCS_BT_SHOT_ACK);
	XiahNetwork::RegisterHandler( CS_BT_MUGONGPREATTACK_ACK,		OnCS_BT_MUGONGPREATTACK_ACK);
	XiahNetwork::RegisterHandler( CS_BT_MUGONGATTACK_ACK,			OnCS_BT_MUGONGATTACK_ACK);
	XiahNetwork::RegisterHandler( CS_BT_KEEPUPMUGONGSTART_ACK,		My_OnCS_BT_KEEPUPMUGONGSTART_ACK);
	XiahNetwork::RegisterHandler( CS_BT_KEEPUPMUGONGEND_ACK,		My_OnCS_BT_KEEPUPMUGONGEND_ACK);
	XiahNetwork::RegisterHandler( CS_BT_KEEPUPMUGONGSTATUS_ACK,		OnCS_BT_KEEPUPMUGONGSTATUS_ACK);
	XiahNetwork::RegisterHandler( CS_BT_KEEPUPMUGONGSTATUSLIST_ACK,	OnCS_BT_KEEPUPMUGONGSTATUSLIST_ACK);

	XiahNetwork::RegisterHandler( CS_BT_KILLSUCCESS_ACK,			OnCS_BT_KILLSUCCESS_ACK);
	XiahNetwork::RegisterHandler( CS_BT_LEVELUP_ACK,				OnCS_BT_LEVELUP_ACK);

	// [4/23/2004]
	XiahNetwork::RegisterHandler( CS_BT_ASKPARTYBATTLE_ACK,		OnCS_BT_ASKPARTYBATTLE_ACK);
	XiahNetwork::RegisterHandler( CS_BT_STARTPARTYBATTLE_ACK,		OnCS_BT_STARTPARTYBATTLE_ACK);
	XiahNetwork::RegisterHandler( CS_BT_ENDPARTYBATTLE_ACK,			OnCS_BT_ENDPARTYBATTLE_ACK);

	XiahNetwork::RegisterHandler( CS_NC_STARTMOVE_ACK,				OnCS_NC_STARTMOVE_ACK);
	XiahNetwork::RegisterHandler( CS_NC_SYNCMOVE_ACK,				OnCS_NC_SYNCMOVE_ACK);
	XiahNetwork::RegisterHandler( CS_NC_ENDMOVE_ACK,				OnCS_NC_ENDMOVE_ACK);
	XiahNetwork::RegisterHandler( CS_NC_MAPENTER_ACK,				OnCS_NC_MAPENTER_ACK);
	XiahNetwork::RegisterHandler( CS_NC_MAPLEAVE_ACK,				OnCS_NC_MAPLEAVE_ACK);
	XiahNetwork::RegisterHandler( CS_NC_NPCINFO_ACK,				OnCS_NC_NPCINFO_ACK);
	XiahNetwork::RegisterHandler( CS_NC_STATUSCHANGE_ACK,			OnCS_NC_STATUSCHANGE_ACK);
	XiahNetwork::RegisterHandler( CS_NC_NPCINFOLIST_ACK,			OnCS_NC_NPCINFOLIST_ACK);
	XiahNetwork::RegisterHandler( CS_NC_FUNCTIONALNPCINFOLIST_ACK,	OnCS_NC_FUNCTIONALNPCINFOLIST_ACK);
	XiahNetwork::RegisterHandler( CS_NC_FUNCTIONALNPCINFO_ACK,		OnCS_NC_FUNCTIONALNPCINFO_ACK);
	XiahNetwork::RegisterHandler( CS_NC_FUNCTIONALNPCITEMLIST_ACK,	OnCS_NC_FUNCTIONALNPCITEMLIST_ACK);
	XiahNetwork::RegisterHandler( CS_NC_PETINFO_ACK,				OnCS_NC_PETINFO_ACK);
	XiahNetwork::RegisterHandler( CS_NC_PETINFOLIST_ACK,			OnCS_NC_PETINFOLIST_ACK);
	XiahNetwork::RegisterHandler( CS_NC_PETDETAILINFO_ACK,			OnCS_NC_PETDETAILINFO_ACK);
	XiahNetwork::RegisterHandler( CS_NC_CHGOWNER_ACK,				OnCS_NC_CHGOWNER_ACK);
	XiahNetwork::RegisterHandler( CS_NC_TAMING_ACK,					OnCS_NC_TAMING_ACK);
	XiahNetwork::RegisterHandler( CS_NC_NPCHP_ACK,					OnCS_NC_NPCHP_ACK);
	XiahNetwork::RegisterHandler( CS_NC_PETRENAME_ACK,				OnCS_NC_PETRENAME_ACK);
	XiahNetwork::RegisterHandler( CS_NC_PETLEVELUP_ACK,				OnCS_NC_PETLEVELUP_ACK);
	XiahNetwork::RegisterHandler( CS_NC_PETEXP_ACK,					OnCS_NC_PETEXP_ACK);
	XiahNetwork::RegisterHandler( CS_NC_PETHP_ACK,					OnCS_NC_PETHP_ACK);
	XiahNetwork::RegisterHandler( CS_NC_PETBONGIN_ACK,				OnCS_NC_PETBONGIN_ACK);
	XiahNetwork::RegisterHandler( CS_NC_PETBONGOUT_ACK,				OnCS_NC_PETBONGOUT_ACK);
	XiahNetwork::RegisterHandler( CS_NC_PET_CONTROL_ACK,			OnCS_NC_PET_CONTROL_ACK);
	XiahNetwork::RegisterHandler( CS_NC_PETWILDRATE_ACK,			OnCS_NC_PETWILDRATE_ACK);
	XiahNetwork::RegisterHandler( CS_NC_PETREVOLUTION_ACK,			OnCS_NC_PETREVOLUTION_ACK);
	XiahNetwork::RegisterHandler( CS_NC_PETSACKLIST_ACK,			OnCS_NC_PETSACKLIST_ACK);
	XiahNetwork::RegisterHandler( CS_NC_PETITEMPUT_ACK,				OnCS_NC_PETITEMPUT_ACK);
	XiahNetwork::RegisterHandler( CS_NC_PETITEMOUT_ACK,				OnCS_NC_PETITEMOUT_ACK);
	XiahNetwork::RegisterHandler( CS_NC_PETITEMMOVE_ACK,			OnCS_NC_PETITEMMOVE_ACK);
	XiahNetwork::RegisterHandler( CS_NC_PETPICKITEM_ACK,			OnCS_NC_PETPICKITEM_ACK);
	XiahNetwork::RegisterHandler( CS_NC_PETTHROWITEM_ACK,			OnCS_NC_PETTHROWITEM_ACK);
	XiahNetwork::RegisterHandler( CS_NC_PETITEMADD_ACK,				OnCS_NC_PETITEMADD_ACK);
	XiahNetwork::RegisterHandler( CS_NC_PETITEMDEL_ACK,				OnCS_NC_PETITEMDEL_ACK);
	XiahNetwork::RegisterHandler( CS_NC_PETNEWMAKE_ACK,				OnCS_NC_PETNEWMAKE_ACK);

	XiahNetwork::RegisterHandler( CS_NC_PETRESTORELIST_ACK,			OnCS_NC_PETRESTORELIST_ACK);
	XiahNetwork::RegisterHandler( CS_NC_PETRESTORE_ACK,				OnCS_NC_PETRESTORE_ACK);
	XiahNetwork::RegisterHandler( CS_NC_PREPETTRADE_ACK,			OnCS_NC_PREPETTRADE_ACK);
	XiahNetwork::RegisterHandler( CS_NC_PETTRADEINFO_ACK,			OnCS_NC_PETTRADEINFO_ACK);
	XiahNetwork::RegisterHandler( CS_NC_PETTRADE_ACK,				OnCS_NC_PETTRADE_ACK);

	XiahNetwork::RegisterHandler( CS_IF_HELPMESSAGE_ACK,			OnCS_IF_HELPMESSAGE_ACK);
	XiahNetwork::RegisterHandler( CS_IF_SCHOOLLIST_ACK,				OnCS_IF_SCHOOLLIST_ACK);
	XiahNetwork::RegisterHandler( CS_IF_CHARMONEY_ACK,				OnCS_IF_CHARMONEY_ACK);
	XiahNetwork::RegisterHandler( CS_IF_CHARINFO_ACK,				OnCS_IF_CHARINFO_ACK);
	XiahNetwork::RegisterHandler( CS_IF_CHARHP_ACK,					OnCS_IF_CHARHP_ACK);
	XiahNetwork::RegisterHandler( CS_IF_CHAREXP_ACK,				OnCS_IF_CHAREXP_ACK);
	XiahNetwork::RegisterHandler( CS_IF_FAMEINFO_ACK,				OnCS_IF_FAMEINFO_ACK);
	// 파티
	XiahNetwork::RegisterHandler( CS_IF_ASKPARTY_ACK,				OnCS_IF_ASKPARTY_ACK);
	XiahNetwork::RegisterHandler( CS_IF_CREATEPARTY_ACK,			OnCS_IF_CREATEPARTY_ACK);
	XiahNetwork::RegisterHandler( CS_IF_ENTERPARTY_ACK,				OnCS_IF_ENTERPARTY_ACK);
	XiahNetwork::RegisterHandler( CS_IF_PARTYLIST_ACK,				OnCS_IF_PARTYLIST_ACK);
	XiahNetwork::RegisterHandler( CS_IF_INVITEPARTY_ACK,			OnCS_IF_INVITEPARTY_ACK);
	XiahNetwork::RegisterHandler( CS_IF_LEAVEPARTY_ACK,				OnCS_IF_LEAVEPARTY_ACK);
	XiahNetwork::RegisterHandler( CS_IF_PARTYPOSITION_ACK,			OnCS_IF_PARTYPOSITION_ACK);
	XiahNetwork::RegisterHandler( CS_IF_CHANGEPARTYLEADER_ACK,		OnCS_IF_CHANGEPARTYLEADER_ACK);
	XiahNetwork::RegisterHandler( CS_IF_DESTROYPARTY_ACK,			OnCS_IF_DESTROYPARTY_ACK);
	XiahNetwork::RegisterHandler( CS_IF_BANISHPARTY_ACK,			OnCS_IF_BANISHPARTY_ACK);
	// 친구
	XiahNetwork::RegisterHandler( CS_IF_ASKADDBUDDY_ACK,			OnCS_IF_ASKADDBUDDY_ACK);
	XiahNetwork::RegisterHandler( CS_IF_ADDBUDDY_ACK,				OnCS_IF_ADDBUDDY_ACK);
	XiahNetwork::RegisterHandler( CS_IF_DELBUDDY_ACK,				OnCS_IF_DELBUDDY_ACK);
	XiahNetwork::RegisterHandler( CS_IF_BUDDYLIST_ACK,				OnCS_IF_BUDDYLIST_ACK);
	XiahNetwork::RegisterHandler( CS_IF_CHANGEBUDDYCONNECT_ACK,		OnCS_IF_CHANGEBUDDYCONNECT_ACK);
	XiahNetwork::RegisterHandler( CS_IF_BUDDYPOSITION_ACK,			OnCS_IF_BUDDYPOSITION_ACK);
	//애완동물
	XiahNetwork::RegisterHandler( CS_IF_PETLIST_ACK,				OnCS_IF_PETLIST_ACK);
	//보상
	XiahNetwork::RegisterHandler( CS_IF_CHARTPSP_ACK,				OnCS_IF_CHARTPSP_ACK);
	// 오행
	XiahNetwork::RegisterHandler(CS_IF_EXECFIVEELM_ACK,				OnCS_IF_EXECFIVEELM_ACK);
	XiahNetwork::RegisterHandler(CS_IF_CHANGEFIVEELM_ACK,			OnCS_IF_CHANGEFIVEELM_ACK);
	// 단 경험치 분배
	XiahNetwork::RegisterHandler(CS_IF_PARTYSHARE_ACK,				OnCS_IF_PARTYSHARE_ACK);

	XiahNetwork::RegisterHandler(CS_IF_STAMINA_ACK,					OnCS_IF_STAMINA_ACK);
	XiahNetwork::RegisterHandler(CS_IF_EXECSTAMINA_ACK,				OnCS_IF_EXECSTAMINA_ACK);	

	XiahNetwork::RegisterHandler( CS_IM_ADDONMAP_ACK,				OnCS_IM_ADDONMAP_ACK);
	XiahNetwork::RegisterHandler( CS_IM_REMOVEFROMMAP_ACK,			OnCS_IM_REMOVEFROMMAP_ACK);
	XiahNetwork::RegisterHandler( CS_IM_MAPITEMINFO_ACK,			OnCS_IM_MAPITEMINFO_ACK);
	XiahNetwork::RegisterHandler( CS_IM_MAPITEMINFOLIST_ACK,		OnCS_IM_MAPITEMINFOLIST_ACK);
	XiahNetwork::RegisterHandler( CS_IM_MOVE_ACK,					OnCS_IM_MOVE_ACK);
	XiahNetwork::RegisterHandler( CS_IM_REMOVEFROMSACK_ACK,			OnCS_IM_REMOVEFROMSACK_ACK);
	XiahNetwork::RegisterHandler( CS_IM_REMOVELISTFROMMAP_ACK,		OnCS_IM_REMOVELISTFROMMAP_ACK);
	XiahNetwork::RegisterHandler( CS_IM_ADDONSACK_ACK,				OnCS_IM_ADDONSACK_ACK);
	XiahNetwork::RegisterHandler( CS_IM_READRESULT_ACK,				OnCS_IM_READRESULT_ACK);
	XiahNetwork::RegisterHandler( CS_IM_CHANGERES_ACK,				OnCS_IM_CHANGERES_ACK);
	//XiahNetwork::RegisterHandler( CS_IM_CHECKITEMPRICE_ACK,			OnCS_IM_CHECKITEMPRICE_ACK);
	XiahNetwork::RegisterHandler( CS_IM_DURABILITY_ACK,				OnCS_IM_DURABILITY_ACK);
	XiahNetwork::RegisterHandler( CS_IM_REPAIRITEM_ACK,				OnCS_IM_REPAIRITEM_ACK);
	XiahNetwork::RegisterHandler( CS_IM_REBUILDITEMTERM_ACK,		OnCS_IM_REBUILDITEMTERM_ACK);
	XiahNetwork::RegisterHandler( CS_IM_REBUILDITEM_ACK,			OnCS_IM_REBUILDITEM_ACK);
	XiahNetwork::RegisterHandler( CS_IM_ITEMINFO_ACK,				OnCS_IM_ITEMINFO_ACK);
	XiahNetwork::RegisterHandler( CS_IM_PICK_ACK,					OnCS_IM_PICK_ACK);
	XiahNetwork::RegisterHandler( CS_IM_GIVEITEM_ACK,				OnCS_IM_GIVEITEM_ACK);

	XiahNetwork::RegisterHandler( CS_IM_REPAIRWITHITEM_ACK,			OnCS_IM_REPAIRWITHITEM_ACK);
	XiahNetwork::RegisterHandler( CS_IM_REMARKITEM_ACK,				OnCS_IM_REMARKITEM_ACK);
	XiahNetwork::RegisterHandler( CS_IM_MONEYBAG_ACK,				OnCS_IM_MONEYBAG_ACK);

	// 전서구
	XiahNetwork::RegisterHandler( CS_IM_MEMOLIST_ACK,				OnCS_IM_MEMOLIST_ACK);
	XiahNetwork::RegisterHandler( CS_IM_SENDMEMO_ACK,				OnCS_IM_SENDMEMO_ACK);
	XiahNetwork::RegisterHandler( CS_IM_NEWMEMO_ACK,				OnCS_IM_NEWMEMO_ACK);
	XiahNetwork::RegisterHandler( CS_IM_READMEMO_ACK,				OnCS_IM_READMEMO_ACK);
	XiahNetwork::RegisterHandler( CS_IM_DELETEMEMO_ACK,				OnCS_IM_DELETEMEMO_ACK);

	XiahNetwork::RegisterHandler( CS_IM_DAMAGE_ACK,					OnCS_IM_DAMAGE_ACK);

	XiahNetwork::RegisterHandler( CS_IM_PETITEMINFO_ACK,			OnCS_IM_PETITEMINFO_ACK);
	
	// 조합
	XiahNetwork::RegisterHandler(CS_IM_PUZZLEITEM_ACK,				OnCS_IM_PUZZLEITEM_ACK);
	XiahNetwork::RegisterHandler(CS_IM_REJOINITEM_ACK,				OnCS_IM_REJOINITEM_ACK);
	XiahNetwork::RegisterHandler(CS_IM_GATHERITEM_ACK,				OnCS_IM_GATHERITEM_ACK);	
	XiahNetwork::RegisterHandler(CS_IM_VARIENTITEM_ACK,				OnCS_IM_VARIENTITEM_ACK);		// 가래떡
	XiahNetwork::RegisterHandler(CS_IM_REWARDGUARANTEE_ACK,			OnCS_IM_REWARDGUARANTEE_ACK);	// 보험 아이템 복구,소멸
	XiahNetwork::RegisterHandler(CS_IM_EVENTPUZZLE_ACK,				OnCS_IM_EVENTPUZZLE_ACK);		// 이미지 조합
	XiahNetwork::RegisterHandler(CS_IM_MIXITEM_ACK,					OnCS_IM_MIXITEM_ACK);			
	XiahNetwork::RegisterHandler(CS_IM_MAKEREPAIRHAMMER_ACK,		OnCS_IM_MAKEREPAIRHAMMER_ACK);	// 망치 조합	

	// 아이템 수집
	XiahNetwork::RegisterHandler(CS_IM_MOVEINCOLLECTITEM_ACK,		OnCS_IM_MOVEINCOLLECTITEM_ACK);
	XiahNetwork::RegisterHandler(CS_IM_MOVEOUTCOLLECTITEM_ACK,		OnCS_IM_MOVEOUTCOLLECTITEM_ACK);

	XiahNetwork::RegisterHandler(CS_IM_QUESTROLL_ACK,				OnCS_IM_QUESTROLL_ACK);			//HT_0829 : 프리미엄 퀘스트 

	// 이벤트 아이템 장착
	XiahNetwork::RegisterHandler(CS_NV_EVENTTIME_ACK,				OnCS_NV_EVENTTIME_ACK);
	XiahNetwork::RegisterHandler(CS_NV_RUNEVENTINFO_ACK,			OnCS_NV_RUNEVENTINFO_ACK);
	XiahNetwork::RegisterHandler(CS_NV_CHANGEEVENTINFO_ACK,			OnCS_NV_CHANGEEVENTINFO_ACK);
	XiahNetwork::RegisterHandler(CS_NV_CHARPREMIUM_ACK,				OnCS_NV_CHARPREMIUM_ACK);		//HT_0122 : 기간제 프리미엄 아이템 추가

	XiahNetwork::RegisterHandler( CS_CD_ADDEQUIPMENT_ACK,			OnCS_CD_ADDEQUIPMENT_ACK);
	XiahNetwork::RegisterHandler( CS_CD_CHGEQUIPMENT_ACK,			OnCS_CD_CHGEQUIPMENT_ACK);
	XiahNetwork::RegisterHandler( CS_CD_DELEQUIPMENT_ACK,			OnCS_CD_DELEQUIPMENT_ACK);
	XiahNetwork::RegisterHandler( CS_CD_ACTION_ACK,					OnCS_CD_ACTION_ACK);
	XiahNetwork::RegisterHandler( CS_CD_CHARUPDATE_ACK,				OnCS_CD_CHARUPDATE_ACK);	

	XiahNetwork::RegisterHandler( CS_EC_BUYITEM_ACK,				OnCS_EC_BUYITEM_ACK);
	XiahNetwork::RegisterHandler( CS_EC_SELLITEM_ACK,				OnCS_EC_SELLITEM_ACK);
	XiahNetwork::RegisterHandler( CS_EC_ASKTRADE_ACK,				OnCS_EC_ASKTRADE_ACK);
	XiahNetwork::RegisterHandler( CS_EC_TRADEOPENSACK_ACK,			OnCS_EC_TRADEOPENSACK_ACK);
	XiahNetwork::RegisterHandler( CS_EC_TRADESACKONITEM_ACK,		OnCS_EC_TRADESACKONITEM_ACK);
	XiahNetwork::RegisterHandler( CS_EC_TRADESACKOFFITEM_ACK,		OnCS_EC_TRADESACKOFFITEM_ACK);
	XiahNetwork::RegisterHandler( CS_EC_TRADEITEM_ACK,				OnCS_EC_TRADEITEM_ACK);
	XiahNetwork::RegisterHandler( CS_EC_TRADECOMPLETE_ACK,			OnCS_EC_TRADECOMPLETE_ACK);
	XiahNetwork::RegisterHandler( CS_EC_TRADESACKONMONEY_ACK,		OnCS_EC_TRADESACKONMONEY_ACK);
	XiahNetwork::RegisterHandler( CS_EC_TRADESACKOFFMONEY_ACK,		OnCS_EC_TRADESACKOFFMONEY_ACK);

	XiahNetwork::RegisterHandler( CS_EC_ITEMLISTINBANK_ACK,			OnCS_EC_ITEMLISTINBANK_ACK);
	XiahNetwork::RegisterHandler( CS_EC_ADDONBANK_ACK,				OnCS_EC_ADDONBANK_ACK);
	XiahNetwork::RegisterHandler( CS_EC_REMOVEFROMBANK_ACK,			OnCS_EC_REMOVEFROMBANK_ACK);
	XiahNetwork::RegisterHandler( CS_EC_DRAWINBANK_ACK,				OnCS_EC_DRAWINBANK_ACK);
	XiahNetwork::RegisterHandler( CS_EC_DRAWOUTBANK_ACK,			OnCS_EC_DRAWOUTBANK_ACK);
	XiahNetwork::RegisterHandler( CS_EC_DRAWMOVEBANK_ACK,			OnCS_EC_DRAWMOVEBANK_ACK);

	XiahNetwork::RegisterHandler( CS_EC_ITEMLISTINMALL_ACK,			OnCS_EC_ITEMLISTINMALL_ACK);
	XiahNetwork::RegisterHandler( CS_EC_DRAWOUTMALL_ACK,			OnCS_EC_DRAWOUTMALL_ACK);
	XiahNetwork::RegisterHandler( CS_EC_DRAWMOVEMALL_ACK,			OnCS_EC_DRAWMOVEMALL_ACK);
	XiahNetwork::RegisterHandler( CS_EC_ADDONMALL_ACK,				OnCS_EC_ADDONMALL_ACK);
	XiahNetwork::RegisterHandler( CS_EC_REMOVEFROMMALL_ACK,			OnCS_EC_REMOVEFROMMALL_ACK);

	XiahNetwork::RegisterHandler( CS_EC_BUYLOTTO_ACK,				OnCS_EC_BUYLOTTO_ACK);
	XiahNetwork::RegisterHandler( CS_EC_LOTTOSALEINFO_ACK,			OnCS_EC_LOTTOSALEINFO_ACK);
	XiahNetwork::RegisterHandler( CS_EC_PRIZELOTTOINFO_ACK,			OnCS_EC_PRIZELOTTOINFO_ACK);
	XiahNetwork::RegisterHandler( CS_EC_GETLOTTOMONEY_ACK,			OnCS_EC_GETLOTTOMONEY_ACK);
	XiahNetwork::RegisterHandler( CS_EC_CHECKLOTTO_ACK,				OnCS_EC_CHECKLOTTO_ACK);
	XiahNetwork::RegisterHandler( CS_EC_LOTTONOTICE_ACK,			OnCS_EC_LOTTONOTICE_ACK);	
	
	XiahNetwork::RegisterHandler( CS_EC_GUARANTEELIST_ACK,			OnCS_EC_GUARANTEELIST_ACK);	// 보험 아이템
	XiahNetwork::RegisterHandler( CS_EC_QUICKMART_ACK,				OnCS_EC_QUICKMART_ACK);		// 매품패	
	
	XiahNetwork::RegisterHandler( CS_EV_TIME_ACK,					OnCS_EV_TIME_ACK);
	XiahNetwork::RegisterHandler( CS_EV_WEATHER_ACK,				OnCS_EV_WEATHER_ACK);

	XiahNetwork::RegisterHandler( CS_RL_CREATEMUNPA_ACK,			OnCS_RL_CREATEMUNPA_ACK);
	XiahNetwork::RegisterHandler( CS_RL_DELETEMUNPA_ACK,			OnCS_RL_DELETEMUNPA_ACK);
	XiahNetwork::RegisterHandler( CS_RL_ASKMUNWON_ACK,				OnCS_RL_ASKMUNWON_ACK);
	XiahNetwork::RegisterHandler( CS_RL_ADDMUNWON_ACK,				OnCS_RL_ADDMUNWON_ACK);
	XiahNetwork::RegisterHandler( CS_RL_DELMUNWON_ACK,				OnCS_RL_DELMUNWON_ACK);
	XiahNetwork::RegisterHandler( CS_RL_MUNWONINFO_ACK,				OnCS_RL_MUNWONINFO_ACK);
	XiahNetwork::RegisterHandler( CS_RL_MUNWONINFO2_ACK,			OnCS_RL_MUNWONINFO2_ACK);
	XiahNetwork::RegisterHandler( CS_RL_MUNWONLIST_ACK,				OnCS_RL_MUNWONLIST_ACK);
	XiahNetwork::RegisterHandler( CS_RL_CHANGEMUNWONORDER_ACK,		OnCS_RL_CHANGEMUNWONORDER_ACK);
	XiahNetwork::RegisterHandler( CS_RL_MUNPAINFO_ACK,				OnCS_RL_MUNPAINFO_ACK);
	XiahNetwork::RegisterHandler( CS_RL_MUNPACHAT_ACK,				OnCS_RL_MUNPACHAT_ACK);
	XiahNetwork::RegisterHandler( CS_RL_MUNPANICK_ACK,				OnCS_RL_MUNPANICK_ACK);

	XiahNetwork::RegisterHandler( CS_RL_DONATE_ACK,					OnCS_RL_DONATE_ACK);
	XiahNetwork::RegisterHandler( CS_RL_PREDONATE_ACK,				OnCS_RL_PREDONATE_ACK);
	XiahNetwork::RegisterHandler( CS_RL_CHANGEMUNPAFAME_ACK,		OnCS_RL_CHANGEMUNPAFAME_ACK);
	XiahNetwork::RegisterHandler( CS_RL_GAINSTONE_ACK,				OnCS_RL_GAINSTONE_ACK);
	XiahNetwork::RegisterHandler( CS_RL_MUNPABBSLIST_ACK,			OnCS_RL_MUNPABBSLIST_ACK);
	XiahNetwork::RegisterHandler( CS_RL_MUNPABBSREAD_ACK,			OnCS_RL_MUNPABBSREAD_ACK);
	XiahNetwork::RegisterHandler( CS_RL_MUNPABBSWRITE_ACK,			OnCS_RL_MUNPABBSWRITE_ACK);
	XiahNetwork::RegisterHandler( CS_RL_MUNPABBSDEL_ACK,			OnCS_RL_MUNPABBSDEL_ACK);
	XiahNetwork::RegisterHandler( CS_RL_STONEDELETE_ACK,			OnCS_RL_STONEDELETE_ACK);

	XiahNetwork::RegisterHandler(CS_RL_MUNPANOTICE_ACK,				OnCS_RL_MUNPANOTICE_ACK);
	XiahNetwork::RegisterHandler(CS_RL_MUNPAMARKREG_ACK,			OnCS_RL_MUNPAMARKREG_ACK);
	XiahNetwork::RegisterHandler(CS_RL_MUNPAMARK_ACK,				OnCS_RL_MUNPAMARK_ACK);
	XiahNetwork::RegisterHandler(CS_RL_GAINMARKIMAGE_ACK,			OnCS_RL_GAINMARKIMAGE_ACK);
	XiahNetwork::RegisterHandler(CS_RL_GETMUNPAMONEY_ACK,			OnCS_RL_GETMUNPAMONEY_ACK);

	XiahNetwork::RegisterHandler( CS_RL_ASKRELATION_ACK,			OnCS_RL_ASKRELATION_ACK);
	XiahNetwork::RegisterHandler( CS_RL_BREAKRELATION_ACK,			OnCS_RL_BREAKRELATION_ACK);

	XiahNetwork::RegisterHandler( CS_RL_RELATIONLIST_ACK,			OnCS_RL_RELATIONLIST_ACK);
	XiahNetwork::RegisterHandler( CS_RL_ADDRELATION_ACK,			OnCS_RL_ADDRELATION_ACK);
	XiahNetwork::RegisterHandler( CS_RL_DELRELATION_ACK,			OnCS_RL_DELRELATION_ACK);
	XiahNetwork::RegisterHandler( CS_RL_CHGRELATION_ACK,			OnCS_RL_CHGRELATION_ACK);
	// 절연부
	XiahNetwork::RegisterHandler(CS_RL_BREAKRELATIONITEM_ACK,		OnCS_RL_BREAKRELATIONITEM_ACK);


	XiahNetwork::RegisterHandler( CS_QS_CHANGE_ACK,					OnCS_QS_CHANGE_ACK);
	XiahNetwork::RegisterHandler( CS_QS_LIST_ACK,					OnCS_QS_LIST_ACK);
	XiahNetwork::RegisterHandler( CS_QS_START_ACK,					OnCS_QS_START_ACK);
	XiahNetwork::RegisterHandler( CS_QS_STOP_ACK,					OnCS_QS_STOP_ACK);
	XiahNetwork::RegisterHandler( CS_QS_DELETE_ACK,					OnCS_QS_DELETE_ACK);

	// WR
	// 문파전
	XiahNetwork::RegisterHandler( CS_WR_MUNPASTONELIST_ACK,			OnCS_WR_MUNPASTONELIST_ACK);
	XiahNetwork::RegisterHandler( CS_WR_STONESTATUSCHANGE_ACK,		OnCS_WR_STONESTATUSCHANGE_ACK);
	XiahNetwork::RegisterHandler( CS_WR_PRECHALLENGEWAR_ACK,		OnCS_WR_PRECHALLENGEWAR_ACK);
	XiahNetwork::RegisterHandler( CS_WR_CHALLENGEWAR_ACK,			OnCS_WR_CHALLENGEWAR_ACK);
	XiahNetwork::RegisterHandler( CS_WR_WARSTATUS_ACK,				OnCS_WR_WARSTATUS_ACK);	
	XiahNetwork::RegisterHandler( CS_WR_APPLYWAR_ACK,				OnCS_WR_APPLYWAR_ACK);			// 문파대전 참가 신청
	XiahNetwork::RegisterHandler( CS_WR_WORLDWARMESSAGE_ACK,		OnCS_WR_WORLDWARMESSAGE_ACK);	// 문파대전 발생시 상황과 메시지
	XiahNetwork::RegisterHandler( CS_WR_WORLDWARPOINT_ACK,		    OnCS_WR_WORLDWARPOINT_ACK);		// 현재1위문파와 자신의문파 포인트

	XiahNetwork::RegisterHandler( CS_OP_SETOPTION_ACK,				OnCS_OP_SETOPTION_ACK);
	XiahNetwork::RegisterHandler( CS_OP_OPTIONLIST_ACK,				OnCS_OP_OPTIONLIST_ACK);
	XiahNetwork::RegisterHandler( CS_CH_CHAT_ACK,					OnCS_CH_CHAT_ACK);
	XiahNetwork::RegisterHandler( CS_CH_SYSTEMMESSAGE_ACK,			OnCS_CH_SYSTEMMESSAGE_ACK);

//	XiahNetwork::RegisterHandler( CS_WR_LORDMUNPA_REQ,			OnCS_CH_SYSTEMMESSAGE_ACK);
	XiahNetwork::RegisterHandler( CS_WR_LORDMUNPA_ACK,				OnCS_WR_LORDMUNPA_ACK);			// 문파대전 우승 문파 ID
	XiahNetwork::RegisterHandler( CS_WR_REWARD_ACK,					OnCS_WR_REWARD_ACK);			// 문파대전 상금수령

	// 상점
	XiahNetwork::RegisterHandler( CS_SH_SHOPINFO_ACK,			OnCS_SH_SHOPINFO_ACK);
	XiahNetwork::RegisterHandler( CS_SH_SETSHOP_ACK,			OnCS_SH_SETSHOP_ACK);
	XiahNetwork::RegisterHandler( CS_SH_MOVESHOP_ACK,			OnCS_SH_MOVESHOP_ACK);
	XiahNetwork::RegisterHandler( CS_SH_REGSHOP_ACK,			OnCS_SH_REGSHOP_ACK);
	XiahNetwork::RegisterHandler( CS_SH_DELSHOP_ACK,			OnCS_SH_DELSHOP_ACK);
	XiahNetwork::RegisterHandler( CS_SH_STATUSCHANGE_ACK,		OnCS_SH_STATUSCHANGE_ACK);
	XiahNetwork::RegisterHandler( CS_SH_GETMONEY_ACK,			OnCS_SH_GETMONEY_ACK);
	XiahNetwork::RegisterHandler( CS_SH_GETSHOPINFO_ACK,		OnCS_SH_GETSHOPINFO_ACK); 
	XiahNetwork::RegisterHandler( CS_SH_BUYPCSHOP_ACK,			OnCS_SH_BUYPCSHOP_ACK);

	XiahNetwork::RegisterHandler( CS_SH_ADDONSHOP_ACK,			OnCS_SH_ADDONSHOP_ACK);
	XiahNetwork::RegisterHandler( CS_SH_REMOVEFROMSHOP_ACK,		OnCS_SH_REMOVEFROMSHOP_ACK);
	XiahNetwork::RegisterHandler( CS_SH_SHOPINFOCHANGE_ACK,		OnCS_SH_SHOPINFOCHANGE_ACK);

//	XiahNetwork::RegisterHandler( CS_IM_MAKEUNIONITEM_REQ,		OnCS_IM_MAKEUNIONITEM_REQ);
	XiahNetwork::RegisterHandler( CS_IM_MAKEUNIONITEM_ACK,		OnCS_IM_MAKEUNIONITEM_ACK);

	//HO_0404_07 환배 시스템추가
	XiahNetwork::RegisterHandler( CS_IT_INSTANCE_ACK,			OnCS_IT_INSTANCE_ACK);	

	//각성제 사용
	XiahNetwork::RegisterHandler( CS_IM_REBIRTH_ACK,			OnCS_IM_REBIRTH_ACK);
	XiahNetwork::RegisterHandler( CS_IM_MAKEREBIRTHITEM_ACK,	OnCS_IM_MAKEREBIRTHITEM_ACK);
	
	XiahNetwork::RegisterHandler( CS_IM_GIVEPOWERITEM_ACK,		OnCS_IM_GIVEPOWERITEM_ACK); //HO_0427_07 영수환골신단
	
	XiahNetwork::RegisterHandler( CS_IM_GOLDBOX_ACK,			OnCS_IM_GOLDBOX_ACK); //HO_0828_07 황금열쇠 추가

	//HO_0227_07 광명전,천황전 이벤트 추가
	XiahNetwork::RegisterHandler( CS_WR_CHAMBEROFSECRETMESSAGE_ACK,		OnCS_WR_CHAMBEROFSECRETMESSAGE_ACK);//광명전(비밀의 방) 시스템메세지
	XiahNetwork::RegisterHandler( CS_WR_APPLYSECRETREADY_ACK,			OnCS_WR_APPLYSECRETREADY_ACK);		//광명전(비밀의 방) 참여 신청 버튼 클릭시
	XiahNetwork::RegisterHandler( CS_WR_APPLYSECRET_ACK,				OnCS_WR_APPLYSECRET_ACK);			//광명전(비밀의 방) 신청
	XiahNetwork::RegisterHandler( CS_NV_SECRETADVENTURE_ACK,			OnCS_NV_SECRETADVENTURE_ACK);		//광명전(비밀의 방) 입장
	XiahNetwork::RegisterHandler( CS_WR_BLOODDEVILMESSAGE_ACK,			OnCS_WR_BLOODDEVILMESSAGE_ACK);		//천황전(마혈천황의 방) 시스템메세지
	XiahNetwork::RegisterHandler( CS_WR_APPLYDEVILREADY_ACK,			OnCS_WR_APPLYDEVILREADY_ACK);		//천황전(마혈천황의 방) 참여 신청 버튼 클릭시
	XiahNetwork::RegisterHandler( CS_WR_APPLYDEVIL_ACK,					OnCS_WR_APPLYDEVIL_ACK);			//천황전(마혈천황의 방) 신청	
	XiahNetwork::RegisterHandler( CS_NV_DEVILADVENTURE_ACK,				OnCS_NV_DEVILADVENTURE_ACK);		//천황전(마혈천황의 방) 입장
}