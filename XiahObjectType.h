// Type.h: Define common type constants.
//
//////////////////////////////////////////////////////////////////////

#ifndef _TYPE_H_
#define _TYPE_H_
#include <math.h>

//State Point TYPE
#define CHARMAXLEVEL		60

#define SP_DEX				(BYTE)0x01
#define SP_STR				(BYTE)0x02
#define SP_SUS				(BYTE)0x03
#define SP_VIT				(BYTE)0x04

//#define SQRT(X,Y)							sqrt(X^2+Y^2)

#define GETYEAR(t)					((WORD) ((DWORD)(t)/8640))
#define GETMONTH(t)					((BYTE) (((DWORD)(t)%8640)/720))
#define GETDAY(t)					((BYTE) ((((DWORD)(t)%8640)%720) / 24))
#define GETHOUR(t)					((BYTE) ((((DWORD)(t)%8640)%720) % 24))
#define MAKEREALTIME(y,m,d,h)		((DWORD)(y*12*30*24+m*30*24+d*24+h))


#define TILE_FROM_POS(x,y)		((DWORD)(((DWORD)(y)) << 16 | (DWORD)(x)))

#define MAKEGVALUE(m,c)			(((((DWORD) (m)) << 16)&0xFFFF0000)|((DWORD) (c))&0x0000FFFF)
#define GETMAXVALUE(v)			HIWORD(((DWORD) (v)))
#define GETCURVALUE(v)			LOWORD(((DWORD) (v)))

#define GETHPSTATUS(w,t)		((BOOL) ((((WORD) (w))&((WORD) (t))) == ((WORD) (t)))
#define GETCURPOWER(w)			((BYTE) (((WORD) (w))&0x000F))

//Client Dialog use Macro
#define TAB_ID(t,k)				((WORD)(((((WORD)(t)) << 8)&0xFF00)|(((WORD)(k))&0x00FF)))
#define GET_TABID(i)			((BYTE)(((((WORD)(i))&0xFF00) >> 8)&0x00FF))
#define GET_TREEID(i)			((BYTE)(((WORD)(i))&0x00FF))

// Object Types
#define NUM_OBJECT_TYPE						((BYTE)11)

#define CHAR_VIEW_X							((WORD)128)
#define CHAR_VIEW_Y							((WORD)128)

#define OBJTYPE_ALL							((BYTE)0x00)

#define OBJTYPE_PC							((BYTE)0x01)
#define OBJTYPE_ITEM						((BYTE)0x02)
#define OBJTYPE_NPC							((BYTE)0x03)
#define OBJTYPE_PET							((BYTE)0x04)
#define OBJTYPE_FUNCTIONALNPC				((BYTE)0x05)

#define OBJTYPE_BUILDING					((BYTE)0x06)
#define OBJTYPE_FURNITURE					((BYTE)0x07)
#define OBJTYPE_PORTAL						((BYTE)0x08)
#define OBJTYPE_ARROW						((BYTE)0x09)
#define OBJTYPE_LOT							((BYTE)0x10)

#define OBJTYPE_PC_TRADE_SELLING			((BYTE)0x11)  // 개인 상점 판매

// Character Type //////////////////////////////////////////////////////////////////////////
#define CHARTYPE_MALE						((BYTE)0x05)
#define CHARTYPE_FEMALE						((BYTE)0x06)

// Sack Type //////////////////////////////////////////////////////////////////////////
#define SACKTYPE_EQUIPMENT					((BYTE)0x00)
#define SACKTYPE_CHARSACK1					((BYTE)0x01)
#define SACKTYPE_CHARSACK2					((BYTE)0x02)
#define SACKTYPE_TRADE						((BYTE)0X03)

#define SACKTYPE_COLLECTION					((BYTE)20)

#define SACKTYPE__EQUIPMENT					((BYTE)0x00)
#define SACKTYPE__DEFAULT					((BYTE)0x01)
#define SACKTYPE__DEFAULT2					((BYTE)0x02)
#define SACKTYPE__PC_TRADE_MINE				((BYTE)0x03)
#define SACKTYPE__PC_TRADE_OTHER			((BYTE)0x04)
#define SACKTYPE__NPC_TRADE					((BYTE)0x05)
#define SACKTYPE__DEPOSIT					((BYTE)0x06)
#define SACKTYPE__MODIFY					((BYTE)0x07)
#define SACKTYPE__PET						((BYTE)0x08)
#define SACKTYPE__PET2						((BYTE)0x09)
#define SACKTYPE__PET3						((BYTE)0x10)
#define SACKTYPE__PET_EQUIP					((BYTE)0x11)

#define SACKTYPE__PERSONAL_TRADE_SET		((BYTE)0x12) // 개인 상점 설정
#define SACKTYPE__PERSONAL_TRADE_SELL		((BYTE)0x13) // 개인 상점 판매
#define SACKTYPE__ITEMMALL					((BYTE)0x14) // 아이템몰 창
#define SACKTYPE__SMELT						((BYTE)0x15) // 조합    - 이상하게도 16진수인데 위 0x10부터 저렇게 적혔넹...
#define SACKTYPE__FIVEELEMENT_CONVERT		((BYTE)0x16) // 오행 아이템 제련
#define SACKTYPE__QUICKMART					((BYTE)0x17) // 매품패
#define SACKTYPE__COLLECTION				((BYTE)0x18) // 아이템 수집
#define SACKTYPE__SECRETROOM				((BYTE)0x19) // 광명전 & 천황전
#define SACKTYPE__SECRETROOM1				((BYTE)0x20) // 

//naegong constants //////////////////////////////////////////////////////////////////////////
#define MAX_KEEPUPMUGONG					5

// Munpa Battle
#define TEAM_RED							1
#define TEAM_BLUE							2

#define BATTLE_NORMAL						0
#define BATTLE_OK							1
#define BATTLE_READY						2
#define BATTLE_COUNT						3
#define BATTLE_START						4
#define BATTLE_PLAY							5
#define BATTLE_END							6
#define BATTLE_DRAW							7
#define BATTLE_REFUSE						9

// 명성치의 초기값
#define FAME_BASE							128

// Character Sack /////////////////////////////////////////////
// Equipment
#define NUM_EQUIPPOS						((BYTE)11)
#define EQUIPPOS_WEAPON						((BYTE)0)
#define EQUIPPOS_HAT						((BYTE)1)
#define EQUIPPOS_CLOTH						((BYTE)2)
#define EQUIPPOS_SHOE						((BYTE)3)
#define EQUIPPOS_PROTECTOR					((BYTE)4)
#define EQUIPPOS_RING						((BYTE)5)
#define EQUIPPOS_NECLACE					((BYTE)6)
#define EQUIPPOS_CLOAK						((BYTE)7)
#define EQUIPPOS_BONGIN						((BYTE)8)
#define EQUIPPOS_KEY						((BYTE)10)

// CharSack
#define MAX_NUM_RESOURCE_BUNCH				((WORD)10)
#define MAX_ITEM_POS						((BYTE)47)
#define MAX_PK_ZONE							((BYTE)100)

#define MAX_SACK_ITEM						((BYTE)36)
#define MAX_SACK_CX							6
#define MAX_SACK_CY							6
#define SACKPOS_NULL						(0xFF)

// TradeSack 
#define MAX_TRADESACK_ITEM					((BYTE)30)
#define MAX_TRADESACK_CX					6
#define MAX_TRADESACK_CY					5

// BankSack
#define MAX_BANK_ITEM						((BYTE)72)
#define MAX_BANK_CX							6
#define MAX_BANK_CY							11

// GambleSack
#define GAMBLE_SIZE							((BYTE)3)
//#define GUNGON_MINUS						((BYTE)0)
//#define GAMBLE_EIGHT						((BYTE)1)
//#define GUNGON_PLUS							((BYTE)2)


// Pet ///////////////////////////////////////////////////////
#define MAX_CHARPET							((BYTE)3)

// 2004-02-21 Changth Change
#define PETSACKTYPE_EQUIPMENT				((BYTE)0)
#define PETSACKTYPE_PETSACK1				((BYTE)1)
#define PETSACKTYPE_PETSACK2				((BYTE)2)
#define PETSACKTYPE_PETSACK3				((BYTE)3)

// TODO: 펫
#define MAX_PETEQUIPMENT_ITEM				((BYTE)6)
#define PETEQUIP_WEAPON						((BYTE)0)
#define PETEQUIP_RIDING						((BYTE)1)
#define PETEQUIP_BAG						((BYTE)2)
#define PETEQUIP_ACCESSORY1					((BYTE)3)
#define PETEQUIP_ACCESSORY2					((BYTE)4)
#define PETEQUIP_ARMOR						((BYTE)5)


#define MAX_PETSACK_SIZE					3
#define MAX_PETSACK_ITEM					((BYTE)36)
#define MAX_PETSACK_CX						6
#define MAX_PETSACK_CY						6


#define DEFAULT_CHAR_SPEED					11
#define ATTACK_DELAY_TICK					300

// Item Type
#define MAX_ITEM_ATTRIBUTE					((WORD)25)
#define MAX_GREATBOOK_SKILL					8
#define	MAX_SKILL_ATTRIBUTE					((WORD)25)

#define NUM_ITEMTYPE						((BYTE)30)

//1 ~ 10 PC 행낭창에 장착 할 수 있는 아이템 
#define ITEMTYPE_WEAPON						((BYTE)1)
#define ITEMTYPE_CLOTH						((BYTE)2)
#define ITEMTYPE_HAT						((BYTE)3)
#define ITEMTYPE_SHOE						((BYTE)4)
#define ITEMTYPE_CLOAK						((BYTE)5)
#define ITEMTYPE_RING						((BYTE)6)
#define ITEMTYPE_NECKLACE					((BYTE)7)
#define ITEMTYPE_SOCKET						((BYTE)8)
#define ITEMTYPE_BONGIN						((BYTE)9)
#define ITEMTYPE_RESERVE1					((BYTE)10)

//11 ~ 20 NPC 행낭창과 NPC용 아이템 및 이벤트 아이템 
#define ITEMTYPE_NPCRING					((BYTE)11)
#define ITEMTYPE_NPCNECKLACE				((BYTE)12)
#define ITEMTYPE_NPCWEAPON					((BYTE)13)
#define ITEMTYPE_NPCRIDING					((BYTE)14)
#define ITEMTYPE_NPCBAG						((BYTE)15)
#define ITEMTYPE_NPCITEM					((BYTE)16)
#define ITEMTYPE_SADDLE						((BYTE)17)
#define ITEMTYPE_SUNANG						((BYTE)18)
#define ITEMTYPE_EVENT						((BYTE)19)
#define ITEMTYPE_SURESOURCE					((BYTE)20)

//21 ~ 30 소모성 아이템 
#define ITEMTYPE_BOOK						((BYTE)21)
#define ITEMTYPE_PORTAL						((BYTE)22)
#define ITEMTYPE_POTION						((BYTE)23)
#define ITEMTYPE_MONEY						((BYTE)24)
#define ITEMTYPE_REBUILDRES					((BYTE)25)
#define ITEMTYPE_QUEST						((BYTE)26)
#define ITEMTYPE_LOTTO						((BYTE)27)
#define ITEMTYPE_RESERVE7					((BYTE)28)
#define ITEMTYPE_MANUAL						((BYTE)29)
#define ITEMTYPE_RESERVE9					((BYTE)30)
#define ITEMTYPE_GOLDKEY					((BYTE)36) //HO_0828_07 황금열쇠 추가

//31 ~ 40 GIS용 아이템 
#define ITEMTYPE_REBIRTH					((BYTE)31)
#define ITEMTYPE_GISDURABLITY				((BYTE)32)
#define ITEMTYPE_GISTIMELIMIT				((BYTE)33)
#define ITEMTYPE_PREMIUMQUEST				((BYTE)34)

// Rebuild Item
#define REBUILDITEMTYPE_BASIC				((BYTE)1)
#define REBUILDITEMTYPE_OPTION				((BYTE)2)
#define REBUILDITEMTYPE_LIMIT				((BYTE)3)

#define REBUILDFLAG_ALL						((BYTE)0)
#define REBUILDFLAG_BASIC					((BYTE)1)
#define REBUILDFLAG_OPTION					((BYTE)2)
#define REBUILDFLAG_LIMIT					((BYTE)4)

#define REBUILDFACT_COUNT					8
#define REBUILDFACT_ATKPWR					((BYTE)0)
#define REBUILDFACT_DEFPWR					((BYTE)1)
#define REBUILDFACT_ATKRATING				((BYTE)2)
#define REBUILDFACT_INCRCRITICAL			((BYTE)3)
#define REBUILDFACT_INCRHP					((BYTE)4)
#define REBUILDFACT_INCRIP					((BYTE)5)
#define REBUILDFACT_RESTOREHP				((BYTE)6)
#define REBUILDFACT_RESTOREIP				((BYTE)7)

#define REBUILDLIMIT_COUNT					5
#define REBUILDLIMIT_LEVEL					((BYTE)0)
#define REBUILDLIMIT_STR					((BYTE)1)
#define REBUILDLIMIT_DEX					((BYTE)2)
#define REBUILDLIMIT_SUS					((BYTE)3)
#define REBUILDLIMIT_VIT					((BYTE)4)

// Item Kind
#define ITEMKIND_WEAPON_GUM					((BYTE)0)
#define ITEMKIND_WEAPON_DO					((BYTE)1)
#define ITEMKIND_WEAPON_SUN					((BYTE)2)
#define ITEMKIND_WEAPON_PIL					((BYTE)3)

#define ITEMKIND_DRESS_MAN					((BYTE)5)
#define ITEMKIND_DRESS_WOMAN				((BYTE)6)

#define ITEMKIND_KEY_PORTAL					((BYTE)1)
#define ITEMKIND_KEY_FURNITURE				((BYTE)2)

#define ITEMKIND_MUGONG_PASSIVE				((BYTE)0)
#define ITEMKIND_MUGONG_ACTIVE				((BYTE)1)
#define ITEMKIND_MUGONG_GENERAL				((BYTE)2)

#define ITEMKIND_NPCITEM_TAMING				((BYTE)0)
#define ITEMKIND_NPCITEM_WILDRATE			((BYTE)1)
#define ITEMKIND_NPCITEM_HEAL				((BYTE)2)

//NIGHT_0306 : MALLITEM
#define ITEMKIND_POTION_NORMAL				((BYTE)0)
#define ITEMKIND_POTION_PERCENT				((BYTE)1)
#define ITEMKIND_POTION_HWANBEA				((BYTE)6)

// Kind for weapon
#define ITEMKIND_WEAPON_GUM					((BYTE)0)
#define ITEMKIND_WEAPON_DO					((BYTE)1)
#define ITEMKIND_WEAPON_SUN					((BYTE)2)
#define ITEMKIND_WEAPON_PIL					((BYTE)3)

// Kind for dress
#define ITEMKIND_DRESS_MAN					((BYTE)5)
#define ITEMKIND_DRESS_WOMAN				((BYTE)6)

// Kind for Key
#define ITEMKIND_KEY_PORTAL					((BYTE)1)
#define ITEMKIND_KEY_FURNITURE				((BYTE)2)

// Kind for MugongBook
#define ITEMKIND_MUGONG_PASSIVE				((BYTE)0)
#define ITEMKIND_MUGONG_ACTIVE				((BYTE)1)
#define ITEMKIND_MUGONG_GENERAL				((BYTE)2)

//kind for NpcItem
#define ITEMKIND_NPCITEM_TAMING				((BYTE)0)
#define ITEMKIND_NPCITEM_WILDRATE			((BYTE)1)
#define ITEMKIND_NPCITEM_HEAL				((BYTE)2)

//Money 
#define MONEY_SILVERCOIN					((DWORD)10000)
#define MONEY_SILVERBO						((DWORD)100000)
#define MONEY_GOLDCOIN						((DWORD)1000000)
#define MONEY_GOLDBO						((DWORD)10000000)
#define MONEY_WHITEGOLDBO					((DWORD)100000000)

// Character Status
#define CHARSTATE_NORMAL					((BYTE)0)
#define CHARSTATE_BATTLE					((BYTE)1)
#define CHARSTATE_SIT						((BYTE)2)
#define CHARSTATE_SITCHAIR					((BYTE)3)
#define CHARSTATE_EXEC						((BYTE)4)
#define CHARSTATE_DIE						((BYTE)5)
#define CHARSTATE_LAY						((BYTE)6)
#define CHARSTATE_LAYBED_RIGHT				((BYTE)7)
#define CHARSTATE_LAYBED_LEFT				((BYTE)8)
#define CHARSTATE_KNEE						((BYTE)9)
#define CHARSTATE_PRODUCE					((BYTE)10)
#define CHARSTATE_REVIVAL					((BYTE)11)
#define CHARSTATE_NUMMODE					((BYTE)12)


//Relation type
#define RT_FAMILY							((BYTE)0)
#define RT_SCHOOL							((BYTE)1)

//가족관계의 유형
#define REL_PARENTHOOD						((BYTE)1)
#define REL_SPOUSE							((BYTE)2)

//FAMILY POSITION(가족내의 절대적 위치)
#define FAMILY_NONE							((BYTE)0)
#define FAMILY_FATHER						((BYTE)1)
#define FAMILY_MOTHER						((BYTE)2)
#define FAMILY_CHILD						((BYTE)3)

//FAMILY RELATION(본인을 중심으로 한 상대적 가족 관계)
#define FREL_NONE							((BYTE)0)
#define FREL_FATHER							((BYTE)1)
#define FREL_MOTHER							((BYTE)2)
#define FREL_SPOUSE							((BYTE)3)
#define FREL_CHILD							((BYTE)4)
#define FREL_SIBLING						((BYTE)5)

//SCHOLL RELATION(본인을 중심으로 한 상대적 사부사제관계)
#define SREL_NONE							((BYTE)0)
#define SREL_TEACHER						((BYTE)1)
#define SREL_STUDENT						((BYTE)2)
#define SREL_SAMEGROUP						((BYTE)3)

#define NUM_GAMEOPTION						4

//OPTION Type
#define GAMEOPTION_RELATIONMSG   			((BYTE)0)
#define GAMEOPTION_STARTRESERVATION			((BYTE)1)

//Delete Building Type
#define DELETED_BUILDING					((BYTE)1)
#define DELETED_BYTAX						((BYTE)2)
#define DELETED_ANNOUNCE					((BYTE)4)

#define MIN_BUILDING_TEMPLATE_SIZE			((BYTE)4)

//ApplicationSkill Type
#define APPLICATION_MUGONG					((BYTE)0)
#define APPLICATION_SKILL					((BYTE)1)


// MUGONG Type
#define MUGONGTYPE_GENERAL					((BYTE)2)
#define MUGONGTYPE_PASSIVE					((BYTE)0)
#define MUGONGTYPE_ACTIVE					((BYTE)1)
//HT_0711 :진각성 무공
#define MUGONGTYPE_2TH_REBIRTH1				((BYTE)3)
#define MUGONGTYPE_2TH_REBIRTH2				((BYTE)4)
// 오행
#define MUGONGTYPE_FIVEELEMENT				((BYTE)5)
// 각성 선무공
#define MUGONGTYPE_REBIRTH_NEGONG			((BYTE)6)
#define MUGONGTYPE_REBIRTH_OUTGONG			((BYTE)7)
#define	MUGONGTYPE_SPECIAL_OUTGONG			((BYTE)8)

// buddy
#define MAX_BUDDY							((BYTE)64)

// NPC Status -----------------------------------------------------------------
#define MAX_STATUS				64
#define NPCSTATUS_NONE			((BYTE)0)		// 개체 생성시
#define NPCSTATUS_INIT			((BYTE)1)		// 화면에 아직 안보이는 중
#define NPCSTATUS_STANDING		((BYTE)2)		// 서 있음
#define NPCSTATUS_DIE			((BYTE)3)		// 죽어있는 상태
#define NPCSTATUS_HIDE			((BYTE)6)		// 숨기
#define NPCSTATUS_UNHIDE		((BYTE)7)		// 나타나기
#define NPCSTATUS_WALK			((BYTE)20)		// 이동중 (방랑중과 숫자값은 같다)
#define NPCSTATUS_RUN			((BYTE)21)		// 달리기 이동중
#define NPCSTATUS_IDLE			((BYTE)24)		// 앉아서 휴식
#define NPCSTATUS_IDLE1			((BYTE)25)		// 앉아서 휴식1
#define NPCSTATUS_IDLE2			((BYTE)26)		// 앉아서 휴식2
#define NPCSTATUS_WAKEUP		((BYTE)27)		// 앉은 상태에서 일어나기
#define NPCSTATUS_MELEE			((BYTE)32)		// 근거리 공격
#define NPCSTATUS_SHOT			((BYTE)33)		// 장거리 공격
#define NPCSTATUS_MUGONG		((BYTE)34)		// 무공 공격
#define NPCSTATUS_PREMELEE		(NPCSTATUS_MELEE+10)	// PRE-근거리
#define NPCSTATUS_PRESHOT		(NPCSTATUS_SHOT+10)		// PRE-장거리
#define NPCSTATUS_PREMUGONG		(NPCSTATUS_MUGONG+10)	// PRE-무공

// 하위 호환성을 위해 만든 코드
#define NPCSTATUS_RUNAWAY		((BYTE)10)		// 도망중
#define NPCSTATUS_HEAL			((BYTE)11)		// 체력회복중 (도망 성공후)
#define NPCSTATUS_WANDER		((BYTE)20)		// 방랑중
#define NPCSTATUS_WANDERRUN		((BYTE)21)		// 방랑중 (뛰면서)
#define NPCSTATUS_WAIT			((BYTE)22)		// 대기 중 (부하 요청)
#define NPCSTATUS_COMEBACK		((BYTE)23)		// 귀환중 (방랑후)
#define NPCSTATUS_ATTENTION		((BYTE)30)		// 전투중
#define NPCSTATUS_ATTENTIONRUN	((BYTE)31)		// 전투중 (뛰면서 이동)
// Client에서 자동 처리하기 때문에 전송되지 않는 코드
#define NPCSTATUS_HIT			((BYTE)5)		// 맞는 동작
// ----------------------------------------------------------------------------

//NPC Attack Type
#define NPCATKTYPE_APPROCH					1
#define NPCATKTYPE_SHOT						2
#define NPCATKTYPE_MUGONG					3

//SFX Type
#define SFXTYPE_NPCSHOT						1

#define ATTACK_RANGE_NPC					64
#define MAX_APPEAR							255
#define MAX_CHARRES							1024

// LOT
#define	MAX_FARMS_PER_CHAR					10
#define	MAX_OTHERS_PER_CHAR					30

#define MAX_FARMS_PER_MUNPA					30
#define MAX_OTHERS_PER_MUNPA				50

#define LOTTYPE_FARM						((BYTE)0)		// 농지
#define LOTTYPE_OTHER						((BYTE)1)		// 비농지

#define LOTOWNER_NONE						((BYTE)0)		// 임자없음
#define LOTOWNER_MINE						((BYTE)1)		// 내땅
#define LOTOWNER_OTHERS						((BYTE)2)		// 남의 땅

#define LOTSTATE_NONE						((BYTE)0)
#define LOTSTATE_CLEAR						((BYTE)1)
#define LOTSTATE_YOUNG						((BYTE)2)
#define LOTSTATE_GROW_1						((BYTE)3)
#define LOTSTATE_GROW_2						((BYTE)4)
#define LOTSTATE_HARVEST					((BYTE)5)
// NONE->개간->CLEAR->모종->YOUNG->시간경과->GROW_1->김매기->GROW_2->시간경과->HARVEST->추수->CLEAR

// Resource Type
#define RESTYPE_FARM						((BYTE)1)
#define RESTYPE_HUNT						((BYTE)2)
#define RESTYPE_GATHER						((BYTE)3)
#define RESTYPE_MINE						((BYTE)4)
#define RESTYPE_WOOD						((BYTE)5)
#define RESTYPE_PROD						((BYTE)6)	
#define RESTYPE_JEWELRY						((BYTE)7)

// Resource Kind
#define RESKIND_FARM_RICE					((BYTE)0)
#define RESKIND_FARM_FARM					((BYTE)1)
#define RESKIND_FARM_SPECIAL				((BYTE)2)
#define RESKIND_GATHER_1					((BYTE)0)
#define RESKIND_GATHER_2					((BYTE)1)
#define RESKIND_GATHER_3					((BYTE)2)
#define RESKIND_GATHER_4					((BYTE)3)
#define RESKIND_MINE_1						((BYTE)0)
#define RESKIND_MINE_2						((BYTE)1)
#define RESKIND_PROD_SILK					((BYTE)0)
#define RESKIND_PROD_CLOTH					((BYTE)1)
#define RESKIND_PROD_CONSTRUCT				((BYTE)2)
#define RESKIND_JEWELRY_MINE				((BYTE)2)
#define RESKIND_JEWELRY_WOOD				((BYTE)1)
#define RESKIND_JEWELRY_HUNT				((BYTE)0)
#define RESKIND_JEWELRY_GATHER				((BYTE)3)

// Portal Type
#define PORTALTYPE_OUTDOOR					((BYTE)0)
#define PORTALTYPE_INDOOR					((BYTE)1)
#define PORTALTYPE_STAIR					((BYTE)2)
#define PORTALTYPE_CAVE						((BYTE)3)

// Layer Type
#define LAYERTYPE_WORLD						((BYTE)0)
#define LAYERTYPE_INDOOR					((BYTE)1)
#define LAYERTYPE_CAVE						((BYTE)2)
#define LAYERTYPE_NULL						((BYTE)255)

//Skill Constants
#define MAX_NUM_SKILLRES					6
#define MAX_KNOW_VALUE						100
#define MAX_SKILL_VALUE						100
#define MAX_BASICSKILL_VALUE				100000
#define MAX_MUGONG_VALUE					10000
#define MUGONGSKILL_BASE					100
#define MUGONG_EXP_VALUE					100
#define ACTIONSKILLVALUE_PER_KILL			5


#define BS_BASE								1000

#define LEGUPBYKILL_BASE					10
#define BSUPBYKILL_BASE						5

#define	DEFAULT_HVALUE						50

//기본기술
#define BS_BLACKSMITH						((BYTE)1)
#define BS_COOK								((BYTE)2)
#define BS_CLOTH							((BYTE)3)
#define BS_CONSTRUCT						((BYTE)4)
#define BS_DRUG								((BYTE)5)

//Furniture Type
#define FURTYPE_DECO						((BYTE)0)
#define FURTYPE_TABLE						((BYTE)1)
#define FURTYPE_CHAIR						((BYTE)2)
#define FURTYPE_BED							((BYTE)3)
#define FURTYPE_CABINET						((BYTE)4)
#define FURTYPE_DISPLAY						((BYTE)5)
#define FURTYPE_PROD						((BYTE)6)
#define FURTYPE_SUBPROD						((BYTE)7)
#define FURTYPE_LAMP						((BYTE)8)
#define FURTYPE_FLAG						((BYTE)9)
#define FURTYPE_INLAMP						((BYTE)10)
#define FURTYPE_ACCOUNTFUR					((BYTE)11)
#define FURTYPE_MILEPOST					((BYTE)12)
#define FURTYPE_MUNPA						((BYTE)13)
#define FURTYPE_TAX							((BYTE)14)//추가
#define FURTYPE_NPC							((BYTE)17)

#define FURNITURE_TYPE_SAFE					123

// Furniture Kind
#define FURKIND_CABINET_ALL					((BYTE)0)
#define FURKIND_CABINET_SMALL				((BYTE)1)
#define FURKIND_CABINET_DRESS				((BYTE)2)
#define FURKIND_CABINET_BOOK				((BYTE)3)
#define FURKIND_CABINET_ALLRESOURCE			((BYTE)4)

// Authority Level
#define AUTHORITY_LEVEL_USE					((BYTE)1)
#define AUTHORITY_LEVEL_MANAGE				((BYTE)2)
#define AUTHORITY_LEVEL_OWN					((BYTE)4)

// Shared Level
#define SHARE_LEVEL_PRIVATE					((BYTE)0)
#define SHARE_LEVEL_FAMILY					((BYTE)1)
#define SHARE_LEVEL_ALL						((BYTE)2)

#define DEFAULT_SHARE_LEVEL					SHARE_LEVEL_FAMILY

#define IS_MINE(a)							((BYTE) (((BYTE) (a))&AUTHORITY_LEVEL_OWN))
#define CAN_USE(a)							((BYTE) (((BYTE) (a))&(AUTHORITY_LEVEL_USE|AUTHORITY_LEVEL_MANAGE|AUTHORITY_LEVEL_OWN)))
#define CAN_MANAGE(a)						((BYTE) (((BYTE) (a))&(AUTHORITY_LEVEL_OWN|AUTHORITY_LEVEL_MANAGE)))

typedef enum
{
    dirN,
	dirNE,
	dirE,
	dirSE,
	dirS,
	dirSW,
	dirW,
	dirNW,
	dirNone
}typeDirection;

#define DEFAULT_DIRECTION					dirS


#define TRANSACTION_ROLLBACK				0
#define TRANSACTION_COMMIT					1

// Produce Type
#define PRODUCETYPE_BOOK					13

// Building Type

#define BUILDINGTYPE_NON							0
#define BUILDINGTYPE_KIWA							1
#define BUILDINGTYPE_SEOKJO_KIWA					2
#define BUILDINGTYPE_MOKJO_KIWA						3
#define BUILDINGTYPE_NANGAN_KIWA					4
#define BUILDINGTYPE_JANGSICK_KIWA					5
#define BUILDINGTYPE_CHOGA							6
#define BUILDINGTYPE_NEOWA							7

#define PPC_KIWA									26000
#define PPC_SEOKJO_KIWA								30000
#define PPC_MOKJO_KIWA								29000
#define PPC_NANGAN_KIWA								22000
#define PPC_JANGSICK_KIWA							22000
#define PPC_CHOGA									14000
#define PPC_NEOWA									11000


#define SCHOOL_NONE			((BYTE)0)
#define SCHOOL_SABU			((BYTE)1)
#define SCHOOL_JEJA			((BYTE)2)
#define SCHOOL_SAJO			((BYTE)3)
#define SCHOOL_SABAEK		((BYTE)4)
#define SCHOOL_SASUK		((BYTE)5)
#define SCHOOL_SAJE			((BYTE)6)
#define SCHOOL_SAMAE		((BYTE)7)
#define SCHOOL_SAHYUNG		((BYTE)8)
#define SCHOOL_SAJEO		((BYTE)9)
#define SCHOOL_SAJIL		((BYTE)10)
#define SCHOOL_SASON		((BYTE)11)
#define SCHOOL_DAEJEJA		((BYTE)12)

#define SCHOOLDEPTH_TAESABU	((BYTE)1)

#define MINE_TYPE1			147
#define MINE_TYPE2			181

#define GATHER_TYPE1		152
#define GATHER_TYPE2		153
#define GATHER_TYPE3		154
#define GATHER_TYPE4		155

#define FELLING_TYPE1		2996
#define FELLING_TYPE2		2988
#define FELLING_TYPE3		2990
#define FELLING_TYPE4		2992
#define FELLING_TYPE5		2994
#define FELLING_TYPE6		2986

//Chatting message type
#define CT_NORMAL			((BYTE)0)//챗
#define CT_WHISPER			((BYTE)1)//전음
#define CT_BROADCAST		((BYTE)2)//공지사항
#define CT_MUNJU    		((BYTE)3)//문주메시지
#define CT_BATTLE			((BYTE)4)//문파전 메세지
#define CT_DAN				((BYTE)5)//단 메세지
#define CT_MUNPA_BROADCAST	((BYTE)6)//문파원들에게 고함
#define CT_MUNPA_MUNJUSHOUT	((BYTE)7)//문주 사자후
// CG_2005/01/19 : time message
#define CT_TIMEMESSAGE		((BYTE)8)//앞으로 모든 시간제어 메세지 처리. 지금은 던전열림,닫힘 시간 공지

#define CT_SAYITEM_CELL		((BYTE)10)
#define CT_SAYITEM_MAP		((BYTE)11)
#define CT_SAYITEM_CHANNEL	((BYTE)12)
#define CT_EVENTMSG			((BYTE)13)

#define	CT_GETEVENTITEM		((BYTE)14)



//세율
#define TR_LOT_FARM			20	//5%
#define TR_LOT_OTHER		20	//5%			
#define TR_BUILDING			12  //8%
#define TR_PROGRESSIVE		2	//50%

//감세율

#define TR_CHILDREN1		10	//20%
#define TR_CHILDREN2		15  //40%
#define TR_CHILDREN3		20	//60%
#define TR_CHILDREN4		30	//80%

//Hard coded item visual id_s
#define KEY_VISUAL					181

#define KEYPACKAGE_VISUAL			182
#define YOUNGPAE_VISUAL				195

#define KEY_WEIGHT					1
#define YOUNGPAE_WEIGHT				1
#define MONEY_WEIGHT				1


#define MAX_KEYLIST						((BYTE)9)

#define DURABILITY_SAVE_COUNT			150

#define KEY_CREATED						1
#define KEY_NOT_CREATED					0

#define FURNITURE_STATUS_OPEN			1
#define FURNITURE_STATUS_CLOSE			0
#define PORTAL_STATUS_OPEN			    1
#define PORTAL_STATUS_CLOSE  			0


//MoveStatus
#define MOVE_STOP						0
#define MOVE_WALK						1
#define MOVE_RUN						2
#define MOVE_JUMP						4
#define MOVE_KYUNGGONG					8
#define MOVE_TOGETHER					16


//소유 형태
#define OWNTYPE_NONE					0
#define OWNTYPE_PRIVATE					1
#define OWNTYPE_MUNPA					2
#define OWNTYPE_GM						3		//for futher use

//BUILDING 소유	
#define OTHERBUILDING					0
#define MYBUILDING						1
#define MUNPABUILDING					2
#define MANAGERBUILDING					3

//MANAGER CharID
#define MANAGER1						5103
						
//문파
#define MUNPA_CREATELEVEL				30
#define MUNPA_LEVEL_DEFAULT				29

#define MUNPA_ORDER_NOTHING				((BYTE)0)
#define MUNPA_ORDER_MUNJU				((BYTE)1)
#define MUNPA_ORDER_BUMUNJU				((BYTE)2)
#define MUNPA_ORDER_JANGRO				((BYTE)3)
#define MUNPA_ORDER_HOBUB				((BYTE)4)
#define MUNPA_ORDER_DANGJU				((BYTE)5)
#define MUNPA_ORDER_MUNWON				((BYTE)6)

#define MIN_MUNPAPUPIL_NUM				15
#define	QUORUM_MUNPA_STONE				30

#define ORDER_NOTHING					((BYTE)0)
#define ORDER_MUNJU						((BYTE)1)

//문파전
#define MUNPABATTLE_TYPE_SIZE			((BYTE)3)
#define MUNPABATTLE_TYPE_SURVIVAL		((BYTE)0)
#define MUNPABATTLE_TYPE_SAVEMUNJU		((BYTE)1)
#define MUNPABATTLE_TYPE_FLAGCAPTURE	((BYTE)2)

#define MUNPABATTLE_TIME_SIZE			((BYTE)4)
#define MUNPABATTLE_TIME_10				((BYTE)0)
#define MUNPABATTLE_TIME_30				((BYTE)1)
#define MUNPABATTLE_TIME_60				((BYTE)2)
#define MUNPABATTLE_TIME_90				((BYTE)3)

#define MUNPABATTLE_CHAR_SIZE			((BYTE)4)
#define MUNPABATTLE_CHAR_10				((BYTE)0)
#define MUNPABATTLE_CHAR_20				((BYTE)1)
#define MUNPABATTLE_CHAR_30				((BYTE)2)
#define MUNPABATTLE_CHAR_50				((BYTE)3)

#define MUNPABATTLE_POTION_SIZE			((BYTE)5)
#define MUNPABATTLE_POTION_0			((BYTE)0)
#define MUNPABATTLE_POTION_1			((BYTE)1)
#define MUNPABATTLE_POTION_3			((BYTE)2)
#define MUNPABATTLE_POTION_10			((BYTE)3)
#define MUNPABATTLE_POTION_UNLIMIT		((BYTE)4)

#define MAX_BS_IDX_MUGONG				6

#define MAX_NICKNAME_LEN				20
#define MAX_ITEMNAME_LEN				20
#define MAX_KEYWORD_LEN					50
#define MAX_BUDDYMSG_LEN				100
#define MAX_MUNPANAME_LEN				20
#define MAX_MUNPAORDERNAME_LEN			20
#define MAX_BUILDING_LEN				20
#define MAX_FURNITURE_LEN				20
#define MAX_TEXTSEARCH_LEN				20
#define MAX_BULLETINTEXT_LEN			2000
#define MAX_BULLETINTITLE_LEN			50
#define MAX_PETNAME_LEN					20
#define MAX_NOTICETITLE_LEN				200
#define MAX_NOTICECONTENT_LEN			2000


#define RT_RESOURCE						0
#define RT_BOOK							1
#define RT_ITEM							2
#define RT_JEWELRY						3
#define RT_NONE							4
#define RT_MONEY						5
#define NUM_ROOT_RESULTS				6

#define	MAX_JEWELRYATTR					3
#define JEWELRY_REPARE					0
#define JEWELRY_REMODEL					1
#define JEWELRY_KIND_MULTIREMODEL		0
#define JEWELRY_KIND_SINGLEREMODEL		1

#define MAX_JEWELRY_APPLYTYPE			5

#define JEWELRY_FUNC_NORMAL				0
#define JEWELRY_FUNC_INCWEAPONPWR		1
#define JEWELRY_FUNC_INCDEFPWR			2
#define JEWELRY_FUNC_INCDAMAGE			3
#define JEWELRY_FUNC_DECDAMAGE			4
#define JEWELRY_FUNC_SPEEDUP			5
#define JEWELRY_FUNC_IGNOREDEFFIVEELM	6
#define JEWELRY_FUNC_INCHITRATE			7
#define JEWELRY_FUNC_IGNOREFIVEELM		8
#define JEWELRY_FUNC_INCMUGONGPOWER		9
#define JEWELRY_FUNC_DECNEEDINNER		10
#define JEWELRY_FUNC_INCINTENSITY		11
#define JEWELRY_FUNC_INCDUR				12
#define JEWELRY_FUNC_INCMONDAMAGE		13
#define JEWELRY_FUNC_INCINOUTMAX		14
#define JEWELRY_FUNC_INCINNERMAX		15
#define JEWELRY_FUNC_NOINJURE			16
#define JEWELRY_FUNC_INCSTAMINA			17
#define JEWELRY_FUNC_MAINBODYHEAT		18
#define	JEWELRY_FUNC_DECNEEDSTAMINA		19
#define JEWELRY_FUNC_DECNEEDIP			20

//attack result
#define HIT_RATEFAIL					((BYTE)0)
#define HIT_SUCCESS						((BYTE)1)
#define HIT_IPFAIL						((BYTE)2)
#define HIT_NODEADLY					((BYTE)3)

#define ATTACK_NORMAL					((BYTE)0)
#define ATTACK_SHOT						((BYTE)1)
#define ATTACK_MUGONG					((BYTE)2)

#define IF_TEMPORARY					((BYTE)0)
#define IF_PERMANENT					((BYTE)1)

#define MAKE_IP_INTEGER(b1, b2, b3, b4)	(((DWORD)b1)<<24 | ((DWORD)b2)<<16 | ((DWORD)b3)<<8 | ((DWORD)b4))

#define DEADLYTYPE_ONE					((BYTE) 0)
#define DEADLYTYPE_LINE					((BYTE) 1)
#define DEADLYTYPE_CIRCLE				((BYTE) 2)
#define DEADLYTYPE_ARC					((BYTE) 3)

#define IPUSAGE_DEADLYONE				((WORD) 6)
#define IPUSAGE_DEADLYLINE				((WORD) 4)
#define IPUSAGE_DEADLYCIRCLE			((WORD) 20)
#define IPUSAGE_DEADLYARC				((WORD) 5)

#define ATKPWR_DEADLYONE				((BYTE) 3)
#define ATKPWR_DEADLYLINE				((BYTE) 2)
#define ATKPWR_DEADLYCIRCLE				((BYTE) 1)
#define ATKPWR_DEADLYARC				((BYTE) 1)


#define ATTACKRESULT_FAIL				((BYTE) 0)
#define ATTACKRESULT_SUCCESS			((BYTE) 1)
#define ATTACKRESULT_DEFEND				((BYTE) 2)

#define BUDDYTYPE_FRIEND				((BYTE)0)
#define BUDDYTYPE_FAMILY				((BYTE)1)
#define BUDDYTYPE_SCHOOL				((BYTE)2)

#define WARTYPE_CHALLENGE				((BYTE)1)
#define WARTYPE_START					((BYTE)2)
#define WARTYPE_ALLIANCE				((BYTE)3)
#define WARTYPE_GIVEUP					((BYTE)4)
#define WARTYPE_ALLIANCEGIVEUP			((BYTE)5)
#define WARTYPE_DROP					((BYTE)6)
#define WARTYPE_ERROR					((BYTE)7)

#define BULLETIN_BOARDID_ANNOUNCEMENT	((BYTE)1)
#define BULLETIN_BOARDID_FREE			((BYTE)2)
#define BULLETIN_BOARDID_MARKET			((BYTE)3)
#define BULLETIN_BOARDID_MUNPA			((BYTE)4)
#define BULLETIN_BOARDID_COUNSEL		((BYTE)5)
#define BULLETIN_BOARDID_MSG			((BYTE)6)

#define BULLETIN_SEARCH_NAME			((BYTE)1)
#define BULLETIN_SEARCH_TITLE			((BYTE)2)

#define BULLETIN_MAXROWCOUNT			((BYTE)22)

#define MAX_PETRIDECNT					((BYTE) 2)

#define PETTYPE_NONE					((BYTE) 0)
#define PETTYPE_TAMING					((BYTE) 1)
#define PETTYPE_RIDE					((BYTE) 2)

#define PETACTIONTYPE_FEED				((BYTE)1)

#define SHABBY_POINT					100

#define	MAX_VALID_TIME_DIFF				8000

#define DECDUR_COMMON					((WORD)1)
#define DECDUR_SHOES					((WORD)1)
#define DECDUR_DRESS					((WORD)5)
#define DECDUR_HAT						((WORD)5)
#define DECDUR_ACCESSORY				((WORD)10)
#define DECDUR_PROTECTOR				((WORD)5)
#define	DECDUR_WEAPON					((WORD)3)

#define ER_BUDDY_TYPE_FRIEND			((BYTE)0X01)
#define ER_BUDDY_TYPE_RELATION			((BYTE)0X02)
#define ER_BUDDY_TYPE_ALL				((BYTE)0X03)

#define STARTOPTION_CURRENT				((BYTE)0)
#define STARTOPTION_DIE					((BYTE)1)
#define STARTOPTION_CASTLE				((BYTE)2)

#define PK_COUNT						((BYTE)5)

// 작물종류 
#define CROP_KIND_SSAL					1				
#define CROP_KIND_BORI					3
#define CROP_KIND_MIL					2
#define CROP_KIND_MOKHWA				4
#define CROP_KIND_BBONGIP				5
#define CROP_KIND_GOCHU					6
#define CROP_KIND_BAECHU				77
#define CROP_KIND_MU					78
#define CROP_KIND_HOBAK					79

///////////////////////////////////////////////////////////
// QUEST 관련
#define MAX_QUEST_DATA					((WORD)3)
#define MAX_QUEST_LIST_DATA				((WORD)10)

// Quest Types (Base)
#define QUESTTYPE_QUEST					((BYTE)0)
#define QUESTTYPE_NEW					((BYTE)1)
#define QUESTTYPE_START					((BYTE)2)
#define QUESTTYPE_NPC					((BYTE)3)
#define QUESTTYPE_CHAR					((BYTE)4)
#define QUESTTYPE_ITEM					((BYTE)5)

// Quest Kind
#define QUESTKIND_SINGLE				((BYTE)0)		// 한번
#define QUESTKIND_MULTIPLE				((BYTE)1)		// 여러번

	// 아래 4개는, <발생>인 경우 갯수를 지정할수 없다.
#define QUESTKIND_KILL_TYPE				((BYTE)10)		// 발생/완수:NPC 종류 사냥
#define QUESTKIND_KILL_ID				((BYTE)11)		// 발생/완수:NPC ID 사냥
#define QUESTKIND_PET_TYPE				((BYTE)12)		// 발생/완수:NPC 종류 테임
#define QUESTKIND_PET_ID				((BYTE)13)		// 발생/완수:NPC ID 테임
#define QUESTKIND_PET_LEVELUP			((BYTE)14)		// (발생)/완수:테임몹 레벨업
#define QUESTKIND_KILL_PC				((BYTE)15)		// (발생)/완수:PvP 승리

#define QUESTKIND_BATTLE_MISS			((BYTE)20)		// (발생)/(완수):빗나감 횟수 
#define QUESTKIND_BATTLE_HIT			((BYTE)21)		// (발생)/(완수):치명적 명중 횟수

#define QUESTKIND_GET_MUGONG			((BYTE)30)		// 발생/완수:무공 얻기
#define QUESTKIND_JOIN_DAN				((BYTE)31)		// 발생/완수:단 가입
#define QUESTKIND_JOIN_MUNPA			((BYTE)32)		// 발생/완수:문파 가입
#define QUESTKIND_DONE_QUEST			((BYTE)33)		// 발생:타 퀘스트 수행

#define QUESTKIND_GAPJA					((BYTE)40)		// 발생:갑자
#define QUESTKIND_YUPA					((BYTE)41)		// 발생/완수:유파
#define QUESTKIND_SERVICE				((BYTE)42)		// 발생/완수:명성치
#define QUESTKIND_ATK					((BYTE)43)		// 발생/완수:공격력
#define QUESTKIND_DEF					((BYTE)44)		// 발생/완수:방어력
#define QUESTKIND_ATKRATING				((BYTE)45)		// 발생/완수:명중률
#define QUESTKIND_STR					((BYTE)46)		// 발생/완수:힘
#define QUESTKIND_SUS					((BYTE)47)		// 발생/완수:지구력
#define QUESTKIND_DEX					((BYTE)48)		// 발생/완수:민첩
#define QUESTKIND_VIT					((BYTE)49)		// 발생/완수:진기

#define QUESTKIND_EXP					((BYTE)60)		// 보상:경험치
#define QUESTKIND_TP					((BYTE)61)		// 보상:수련치
#define QUESTKIND_SP					((BYTE)62)		// 보상:능력치

#define QUESTKIND_ITEM_NORMAL			((BYTE)70)		// 완수/보상:일반 아이템
#define QUESTKIND_ITEM_RARE				((BYTE)71)		// 완수:레어 아이템
#define QUESTKIND_ITEM_QUEST			((BYTE)72)		// 완수/보상:퀘스트 아이템
#define QUESTKIND_ITEM_REBUILD			((BYTE)73)		// (발생)/완수:아이템 개조
#define QUESTKIND_ITEM_MONEY			((BYTE)74)		// (발생)/완수/보상:돈
#define QUESTKIND_ITEM_REPAIR			((BYTE)75)		// (발생)/완수:아이템 수리
#define QUESTKIND_ITEM_EQUIP			((BYTE)76)		// (발생)/완수:아이템 착용
#define QUESTKIND_ITEM_GAMBLE			((BYTE)77)		// (발생)/완수:겜블 아이템 구입

// QUEST LIST 상태 : CSPROTOCOL.H의 QUEST_STATUS_XXXX와 같아야 한다.
/*
#define QUESTSTATUS_NEW					((BYTE)0)
#define QUESTSTATUS_START				((BYTE)1)
#define QUESTSTATUS_PAUSE				((BYTE)2)
#define QUESTSTATUS_EXPIRE				((BYTE)3)
#define QUESTSTATUS_SUCCESS				((BYTE)4)

// QUEST 결과 값
#define QUESTRESULT_OK					((BYTE)0)
#define QUESTRESULT_CANT_NEW			((BYTE)1)		// 발생 조건 만족 못함
#define QUESTRESULT_CANT_START			((BYTE)2)		// 수행 조건 만족 못함
#define QUESTRESULT_CANT_STOP			((BYTE)3)		// 중지 불가능
#define QUESTRESULT_CANT_DONE			((BYTE)4)		// 완수 조건 만족 못함
#define QUESTRESULT_CANT_REWARD			((BYTE)5)		// 보상 내역 지급 못함
#define QUESTRESULT_REWARD_DROP			((BYTE)6)		// 가방이 꽉 차서, 보상 아이템이 땅으로 떨어짐
#define QUESTRESULT_ERR_INTERNAL		((BYTE)9)		// 내부 에러
#define QUESTRESULT_WRN_QUESTLISTRESET	((BYTE)10)		// 퀘스트 완수 조건(QuestProcess)이 바뀌어서, 관련 진행 자료(QuestList)를 모두초기화한 경우
#define QUESTRESULT_CHANGED				((BYTE)11)		// 퀘스트 정보 변경됨
*/

//YS_0304 : SHOP
#define GISDURABLITY_SHOP				(BYTE)0		// 상점개설용 기능성 아이템 
#define GISDURABLITY_OPENBANK			(BYTE)1		// 창고문서
//NIGHT_0318 : MALLITEM
#define GISDURABLITY_SAYCELL			(BYTE)2		// 소각적
#define GISDURABLITY_SAYMAP				(BYTE)3		// 중각적
#define GISDURABLITY_SAYCHANNEL			(BYTE)4		// 대각적
#define GISDURABLITY_REPAIR				(BYTE)5		// 연장
#define GISDURABLITY_MONEYBAG			((BYTE)6)	// 전낭		//NIGHT_0513 : MONEYBAG
#define GISDURABLITY_MEMO				((BYTE)7)	// 전서구	//NIGHT_0525 : MEMO

#define MUNPA_ORDER_NOTHING				((BYTE)0)
#define MUNPA_ORDER_MUNJU				((BYTE)1)
#define MUNPA_ORDER_BUMUNJU				((BYTE)2)
#define MUNPA_ORDER_JANGRO				((BYTE)3)
#define MUNPA_ORDER_HOBUB				((BYTE)4)
#define MUNPA_ORDER_DANGJU				((BYTE)5)
#define MUNPA_ORDER_MUNWON				((BYTE)6)


#endif//_TYPE_H
