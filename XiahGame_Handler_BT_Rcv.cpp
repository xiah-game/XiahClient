#include "cjoystic.h"
#include "XiahEnvInfo.h"
//#include "SkillTime.h"

//---------------------------------------------------------------------------------------
extern WORD	g_wDiePosX, g_wDiePosY;	// 呀 系
extern BOOL FadeTrigger_MainCharDie(DWORD nIndex);
extern sString MoneyCommaStr (INT64 nMoney);

//HT_CHEAT : 摹飘 虐
extern BOOL g_bCheat;
extern  BOOL g_bCheatEtc;

bool GetHitEffectType(BYTE byDefObjType, BYTE byDefSubType, int &nPosX, int &nPosY, int &nPosZ, int &nStartTime, BYTE byAtkObjType, BYTE byAtkSubType, int &nEffectType)
{
    nPosX = 0;
	nPosY = 0;
	nPosZ = 0;
	nStartTime = 0;
	nEffectType = 0;

	// 麓 嗉?
	if( byDefObjType == OBJTYPE_PC )
	{
		switch( byDefSubType )
		{
		case 1:	// 丝
			nPosX = 0;		nPosY = 7;		nPosZ = -1;		nStartTime = 0;
			break;
		case 2: // 
			nPosX = 0;		nPosY = 6;		nPosZ = -1;		nStartTime = 0;
			break;
		case 3:	// 
			nPosX = 0;		nPosY = 7;		nPosZ = -1;		nStartTime = 0;
			break;
		case 4:	// 
			nPosX = 0;		nPosY = 5;		nPosZ = -1;		nStartTime = 0;
			break;
		};// switch
	}
	else if( byDefObjType == OBJTYPE_NPC )
	{
		switch( byDefSubType )
		{
		case 1:	//  eYager
			nPosX = 0;		nPosY = 2;		nPosZ = -3;		nStartTime = 0;
			break;
		case 2:	//  eTuo
			nPosX = 0;		nPosY = 4;		nPosZ = -5;		nStartTime = 0;
			break;
		case 7 : case 8 : case 9:	//  eGyu
			nPosX = 0;		nPosY = 5;		nPosZ = -7;		nStartTime = 0;
			break;

		case 21:	//  1 
			nPosX = 0;		nPosY = 8;		nPosZ = 0;		nStartTime = 0;
			break;
		case 23:	//  1 
			nPosX = 0;		nPosY = 6;		nPosZ = -4;		nStartTime = 0;
			break;
		case 24:	//  1 飧?
			nPosX = 0;		nPosY = 4;		nPosZ = -2;		nStartTime = 0;
			break;
		case 25:	//  1 玫偶
			nPosX = 0;		nPosY = 6;		nPosZ = -1;		nStartTime = 0;
			break;

		case 26:	//  2 
			nPosX = 0;		nPosY = 7;		nPosZ = -1;		nStartTime = 0;
			break;
		case 27:	//  2 甙
			nPosX = 0;		nPosY = 4;		nPosZ = -1;		nStartTime = 0;
			break;
		case 28:	//  2 
			nPosX = 0;		nPosY = 7;		nPosZ = -2;		nStartTime = 0;
			break;
		case 29:	//  2 钎
			nPosX = 0;		nPosY = 6;		nPosZ = -1;		nStartTime = 0;
			break;
		case 30:	//  2 
			nPosX = 0;		nPosY = 7;		nPosZ = -2;		nStartTime = 0;
			break;
		case 81:	//  eWestGwyin
			nPosX = 0;		nPosY = 8;		nPosZ = -1;		nStartTime = 0;
			break;
		case 82:	// 匕 eHagolgwuy
			nPosX = 0;		nPosY = 6;		nPosZ = -1;		nStartTime = 0;
			break;
		case 161:	// 绨?eSagal
			nPosX = 0;		nPosY = 5;		nPosZ = -7;		nStartTime = 0;
			break;
		case 80:	// 涓?eYuoma
			nPosX = 0;		nPosY = 5;		nPosZ = -1;		nStartTime = 0;
			break;
		case 160:	//  eToChung
			nPosX = 0;		nPosY = 9;		nPosZ = -2;		nStartTime = 0;
			break;
		case 4 : case 5 : case 6:	//  eAlrue
			nPosX = 1;		nPosY = 7;		nPosZ = -6;		nStartTime = 100;
			break;
		case 91 : case 92 : case 93:	// , 没, ,  eBakranggyun
			nPosX = 0;		nPosY = 5;		nPosZ = -3;		nStartTime = 0;
			break;
		case 0:		//  eGwainggyun
			nPosX = 0;		nPosY = 3;		nPosZ = -2;		nStartTime = 50;
			break;
		case 168 : case 169 : case 170:	// 涂 eGumwawa
			nPosX = 0;		nPosY = 3;		nPosZ = -5;		nStartTime = 50;
			break;
		case 83:	//  eGumgunsujang
			nPosX = -1;		nPosY = 7;		nPosZ = -1;		nStartTime = 100;
			break;
		case 84:	//  eMadoninja
			nPosX = -1;		nPosY = 8;		nPosZ = -1;		nStartTime = 0;
			break;
		case 3: case 10: case 11:	// 龋 eMangho, 龋 eBakho, 龋
			nPosX = 0;		nPosY = 3;		nPosZ = -6;		nStartTime = 0;
			break;
		case 94: case 95: case 96: case 104: case 114:	// , , 驴, 拳, 
			nPosX = 0;		nPosY = 6;		nPosZ = -1;		nStartTime = 0;
			break;
		case 97: case 106: case 117:	// 拳, , 亟
			nPosX = 0;		nPosY = 8;		nPosZ = -1;		nStartTime = 0;
			break;
		case 102: case 105:	// 强, 前
			nPosX = 0;		nPosY = 6;		nPosZ = -2;		nStartTime = 0;
			break;
		case 103:	// 
			nPosX = 0;		nPosY = 8;		nPosZ = 0;		nStartTime = 0;
			break;
		case 162: case 163: case 164:	// , , 榘?
			nPosX = 1;		nPosY = 6;		nPosZ = -9;		nStartTime = 0;
			break;
		case 174: case 175: case 176:	// , 姆, 姆
			nPosX = 1;		nPosY = 3;		nPosZ = -11;	nStartTime = 0;
			break;
		case 101:	// 胃
			nPosX = 0;		nPosY = 7;		nPosZ = -1;		nStartTime = 0;
			break;
		case 100:	// 
			nPosX = 0;		nPosY = 7;		nPosZ = -1;		nStartTime = 0;
			break;
		case 88: case 89: case 90:	//  酶
			nPosX = 0;		nPosY = 7;		nPosZ = -3;		nStartTime = 0;
			break;
		case 86:	// 匕
			nPosX = 0;		nPosY = 6;		nPosZ = -1;		nStartTime = 0;
			break;
		case 87:	// 匕
			nPosX = 0;		nPosY = 6;		nPosZ = -1;		nStartTime = 0;
			break;
		case 85:	// 匕
			nPosX = 0;		nPosY = 6;		nPosZ = 0;		nStartTime = 0;
			break;
		case 107:	// 甙
			nPosX = 0;		nPosY = 4;		nPosZ = -1;		nStartTime = 0;
			break;
		case 109:	// 玫偶
			nPosX = 0;		nPosY = 6;		nPosZ = -1;		nStartTime = 0;
			break;
		case 184:	// 
			nPosX = 0;		nPosY = 10;		nPosZ = -2;		nStartTime = 0;
			break;
		case 108:	// 
			nPosX = 0;		nPosY = 7;		nPosZ = -2;		nStartTime = 0;
			break;
		case 110:	// 钎
			nPosX = 0;		nPosY = 6;		nPosZ = -1;		nStartTime = 0;
			break;
		case 185:	// 
			nPosX = 0;		nPosY = 6;		nPosZ = -4;		nStartTime = 0;
			break;
		case 187:	// 一
			nPosX = 0;		nPosY = 10;		nPosZ = -2;		nStartTime = 0;
			break;
		case 188:	// 
			nPosX = 0;		nPosY = 10;		nPosZ = -2;		nStartTime = 0;
			break;
		case 113:	//  玫
			nPosX = 0;		nPosY = 7;		nPosZ = -1;		nStartTime = 0;
			break;
		case 112:	//  玫
			nPosX = 0;		nPosY = 7;		nPosZ = -1;		nStartTime = 0;
			break;
		case 115:	// 
			nPosX = 0;		nPosY = 7;		nPosZ = -1;		nStartTime = 0;
			break;
		case 116:	// 
			nPosX = 0;		nPosY = 5;		nPosZ = -2;		nStartTime = 0;
			break;
		case 118:	// 胃
			nPosX = 0;		nPosY = 7;		nPosZ = -1;		nStartTime = 0;
			break;
		case 119:	// 莅
			nPosX = 0;		nPosY = 7;		nPosZ = -2;		nStartTime = 0;
			break;
		case 186:	// 
			nPosX = 0;		nPosY = 7;		nPosZ = -1;		nStartTime = 0;
			break;
		case 190:	// 飧?
			nPosX = 0;		nPosY = 4;		nPosZ = -2;		nStartTime = 0;
			break;
			//   甙 - 鸥 摹
		case 122:	// 
			nPosX = 0;		nPosY = 6;		nPosZ = -1;		nStartTime = 0;			
			break;
		case 124:	// 绲?
			nPosX = 0;		nPosY = 5;		nPosZ = -1;		nStartTime = 0;			
			break;
		case 193:	// 
			nPosX = 0;		nPosY = 5;		nPosZ = -6;		nStartTime = 0;			
			break;
		case 121:	// 偶
			nPosX = 0;		nPosY = 5;		nPosZ = -2;		nStartTime = 0;			
			break;
		case 194:	// 
			nPosX = 0;		nPosY = 4;		nPosZ = -3;		nStartTime = 0;			
			break;
		case 123:	// 
			nPosX = 0;		nPosY = 4;		nPosZ = -2;		nStartTime = 0;			
			break;

			// 
		case 195:
		case 196:
		case 197:
		case 198:
		case 199:
		case 200:
		case 201:
		case 202:
			nPosX = 0;		nPosY = 6;		nPosZ = 0;		nStartTime = 0;
			break;

		case 241:
			nPosX = 0;		nPosY = 7;		nPosZ = -3;		nStartTime = 0;
			break;
		};//switch
	}// if

	// 
	if( byAtkObjType == OBJTYPE_PC )
	{
		if(g_AppData.m_bAdult)
		{
			//    鸥
			switch(rand() % 3)
			{
				case 0:
					nEffectType = eAdultAttack1;
					break;
				case 1:
					nEffectType = eAdultAttack2;
					break;
				case 2:
					nEffectType = eAdultAttack3;
					break;
				default:
					nEffectType = eAdultAttack1;
					break;
			}				
		}
		else
		{
			switch(byAtkSubType)
			{
			case 1:	// 丝 
				nEffectType = eGumYung;
				break;
			case 2: // 
				nEffectType = eYunrang;
				break;
			case 3:	// 
				nEffectType = eMooToo;
				break;		
			case 4:	// 
				nEffectType = eYacha;
				break;		
			};// switch
		}		
	}
	else if( byAtkObjType == OBJTYPE_NPC )
	{
		switch( byAtkSubType )
		{
		case 1:	//  eYager
			nEffectType = eYager;
			break;
		case 2:	//  eTuo
			nEffectType = eTuo;
			break;
		case 7 : case 8 : case 9:	//  eGyu
			nEffectType = eGyu;
			break;

		case 21:	//  1 
			nEffectType = eHksabong;
			break;
		case 23:	//  1 
			nEffectType = eGonlyeongja;
			break;
		case 24:	//  1 飧?
			nEffectType = eFireballTiger;
			break;
		case 25:	//  1 玫偶
			nEffectType = eChunshinsulsa;
			break;

		case 26:	//  2 
			nEffectType = eGolem;
			break;
		case 27:	//  2 
			nEffectType = eYagon;
			break;
		case 28:	//  2 
			nEffectType = eArmorGiant;
			break;
		case 29:	//  2 钎
			nEffectType = ePyo;
			break;
		case 30:	//  2 
			nEffectType = eJinmoin;
			break;

		case 81:	//  eWestGwyin
			nEffectType = eWestGwyin;
			break;
		case 82:	//  eHagolgwuy
			nEffectType = eHagolgwuy;
			break;
		case 161:	// 绨?eSagal
			nEffectType = eSagal;
			break;
		case 80:	// 涓?eYuoma
			nEffectType = eYuoma;
			break;
		case 160:	//  eToChung
			nEffectType = eToChung;
			break;
		case 4 : case 5 : case 6:	//  eAlrue
			nEffectType = eAlrue;
			break;
		case 91 : case 92 : case 93:	//  eBakranggyun
			nEffectType = eBakranggyun;
			break;
		case 0:	//  eGwainggyun
			nEffectType = eGwainggyun;
			break;
		case 168 : case 169 : case 170:	// 菘涂 eGumwawa
			nEffectType = eGumwawa;
			break;
		case 83:	// 荼 eGumgunsujang
			nEffectType = eGumgunsujang;
			break;
		case 84:	//  eMadoninja
			nEffectType = eMadoninja;
			break;
		case 3:	// 龋 eMangho
			nEffectType = eMangho;
			break;
		case 94: case 95: case 96: case 114:	// , , 驴, 
			nEffectType = eYoihee;
			break;
		case 97:	// 拳
			nEffectType = eNwyhwa;
			break;
		case 102:	// 强
			nEffectType = eGunyeja;
			break;
		case 103:	// 
			nEffectType = eHksabong;
			break;
		case 162: case 163: case 164:	// , , 榘?
			nEffectType = eBackangjamsi;
			break;
		case 174: case 175: case 176: case 189:	// , 姆, 姆, 
			nEffectType = eJuparyuong;
			break;
		case 10: case 11:	// 龋, 龋
			nEffectType = eBakho;
			break;
		case 104:	// 拳
			nEffectType = eHwanyu;
			break;
		case 106: case 117:	// , 亟
			nEffectType = eNwysin;
			break;
		case 105:	// 前
			nEffectType = eGungon;
			break;
		case 101:	// 胃
			nEffectType = eBumado;
			break;
		case 100:	// 
			nEffectType = eJaso;
			break;
//		case 88:	// 鸥 
//			nEffectType = eSantaGwanHung;
//			break;
		case 88: case 89: case 90:	//  酶
			nEffectType = eGwanhungin;
			break;
		case 86:	// 匕
			nEffectType = eHaegolSerize;
			break;
		case 87:	// 匕
			nEffectType = eHaegolSerize;
			break;
		case 85:	// 匕
			nEffectType = eHaegolSerize;
			break;
		case 184:	// 
			nEffectType = eShinjo;
			break;
		case 107:	// 甙
			nEffectType = eYagon;
			break;
		case 109:	// 玫偶
			nEffectType = eChunshinsulsa;
			break;
		case 108:	// 
			nEffectType = eJinmoin;
			break;
		case 110:	// 钎
			nEffectType = ePyo;
			break;
		case 185:	// 
			nEffectType = eGonlyeongja;
			break;
		case 187:	// 一
			nEffectType = eShinjo;
			break;
		case 188:	// 
			nEffectType = eShinjo;
			break;
		case 113:	// 诩 玫
			nEffectType = eJaso;
			break;
		case 112:	// 诩 玫
			nEffectType = eJaso;
			break;
		case 115:	// 莅
			nEffectType = eMetalMonster;
			break;
		case 116:	// 
			nEffectType = eMouseMonster;
			break;
		case 118:	// 胃
			nEffectType = eTreeMonster;
			break;
		case 119:	// 莅
			nEffectType = eArmorGiant;
			break;
		case 186:	// 
			nEffectType = eGolem;
			break;
		case 190:	// 飧?
			nEffectType = eFireballTiger;
			break;
			// [3/25/2005]   甙 - 鸥
		case 122:	// 
			nEffectType = eFireMonster1;
			break;
		case 193:	// 
			nEffectType = eWaterMonster1;
			break;
		case 121:	// 偶
			nEffectType = eTreeMonster1;
			break;
		case 194:	// 
			nEffectType = eMetalMonster1;
			break;
		case 123:	// 
			nEffectType = eEarthMonster1;
			break;

			// 
		case 195:
		case 196:
		case 197:
		case 198:
		case 199:
		case 200:
		case 201:
		case 202:
			nEffectType = eHaegolSerize;
			break;

		case 241:
			nEffectType = eGwanhungin;
			break;
		};// switch
	}

	return true;
}

// 死亡时暂存挂机状态，复活后自动恢复（需跨编译单元访问，不能是 static）
BOOL s_bCheatBeforeDeath = FALSE;
BOOL s_bCheatEtcBeforeDeath = FALSE;

int OnCS_BT_CHANGEMODE_ACK(CMsg &msg)
{
	DWORD dwObjectID	=0;
	BYTE bState			=0;
	WORD wDirection		=0;

	WORD wPosX		=0;
	WORD wPosY		=0;

	msg
		>> dwObjectID
		>> bState
		>> wDirection
		>> wPosX	// 拙 摹 麓
		>> wPosY;


	if(wPosX < 0 || wPosX > 2047 || wPosY < 0 || wPosY > 2047) 
	{
		DBG_LogFile("Invalid Position in OnCS_BT_CHANGEMODE_ACK");
		return TRUE;
	}

	XiahObject::CXiahObject* pObject = XiahObject::g_XiahObjectManager.FindXiahObject( MAKEOBJECTID( 0, dwObjectID, OBJTYPE_PC));

	if( pObject == NULL)
	{
		ValidateObject( OBJTYPE_PC, dwObjectID, wPosX, wPosY);
		return TRUE;
	}
	
	CXiahCharObject* pCharObject = (CXiahCharObject*)pObject->m_pObject;
	if(pCharObject == NULL)
	{
		DBG_LogFile("Invalid Position in OnCS_BT_CHANGEMODE_ACK");
		return TRUE;
	}
	
	switch( bState)
	{
		// 饪?拙 瞥.
		case CHARSTATE_NORMAL:
			{
				DBG_Put(_T(" PC 暇畛?!"));

				pCharObject->SetAnimation( XiahAniType::eLAT_Spawn, XiahAniType::eLAT_Stand, 0, 1);
				pCharObject->m_bRide = FALSE;	//  枚蟀〖 装懦 壹 媳.

				//  瞥
				if( pObject == g_pMainChar)
				{
					// 俳 探 卮
					g_MainCharInfo.m_IsStarted = TRUE;

					// 复活时恢复死亡前的挂机状态
					if (s_bCheatBeforeDeath || s_bCheatEtcBeforeDeath) {
						g_bCheat = s_bCheatBeforeDeath;
						g_bCheatEtc = s_bCheatEtcBeforeDeath;
						s_bCheatBeforeDeath = FALSE;
						s_bCheatEtcBeforeDeath = FALSE;
					}
				}
			}			
			break;

		// PC  拙.
		case CHARSTATE_DIE:
			{
				if( pCharObject->m_nCurMotionType == XiahAniType::eLAT_Die)
					break;

				// ANIMATION DIE 汛 & LOOP 
				pCharObject->SetAnimation( XiahAniType::eLAT_Die, XiahAniType::eLAT_Died, 0, 0);
				pCharObject->m_CharRender.SetLoopAnimation(FALSE);

				// 2004.07.02 Changth
				//   俜 MapMove_req      MapLeave   职 汛.
				// 赘 MapMove_ack  MapInfo 冒 .
				if( pObject == g_pMainChar) 
				{
					g_wDiePosX	= wPosX; 
					g_wDiePosY	= wPosY;

					g_MainCharInfo.m_bMainCharDie = TRUE;
					// 探 .
					g_MainCharInfo.m_IsStarted = FALSE;

					//  榭≡?  .     执麓.
					Fade::StartFade( 0, TRUE, FadeTrigger_MainCharDie, 2000, TRUE );

					// 死亡时保存挂机状态，复活后在 CHARSTATE_NORMAL 分支恢复
					s_bCheatBeforeDeath = g_bCheat;
					s_bCheatEtcBeforeDeath = g_bCheatEtc;
					g_bCheat = FALSE;
					g_bCheatEtc = FALSE;
				}
			}			
			break;
	}

	return TRUE;
}

//---------------------------------------------------------------------------------------
int OnCS_BT_PREATTACK_ACK(CMsg &msg)
{
	BYTE	bAttackType		=0;
	DWORD	dwAttackID		=0;
	BYTE	bDefType		=0;
	DWORD	dwDefID			=0;
	WORD	wAttackPosX		=0;
	WORD	wAttackPosY		=0;
	BYTE	bAttackHeight	=0;
	BYTE	bAttackMode		=0;
	BYTE	bAttackSpeed	= 9;

	msg >> bAttackType
		>> dwAttackID
		>> wAttackPosX
		>> wAttackPosY
		>> bAttackHeight
		>> bDefType
		>> dwDefID
		>> bAttackMode;

	if(wAttackPosX < 0 || wAttackPosX > 2047 || wAttackPosY < 0 || wAttackPosY > 2047) 
	{
		DBG_LogFile("Invalid Position in OnCS_BT_PREATTACK_ACK");
		return TRUE;
	}

	if( bAttackType == OBJTYPE_PC)
	{
		msg >> bAttackSpeed;
	}

	XiahObject::CXiahObject* pAttacker = XiahObject::g_XiahObjectManager.FindXiahObject( MAKEOBJECTID( 0, dwAttackID, bAttackType));
	XiahObject::CXiahObject* pDefender = XiahObject::g_XiahObjectManager.FindXiahObject( MAKEOBJECTID( 0, dwDefID, bDefType));
	
	if( pAttacker == NULL)
	{
		ValidateObject( bAttackType, dwAttackID, wAttackPosX, wAttackPosY);
		return TRUE;
	}

	if( pDefender == NULL)
	{
		ValidateObject( bDefType, dwDefID, wAttackPosX, wAttackPosY);
		return TRUE;
	}

	CXiahCharObject* pAttackerCharObject = (CXiahCharObject*) pAttacker->m_pObject;
	if(pAttackerCharObject == NULL)
	{
		DBG_LogFile("Invalid Position in OnCS_BT_PREATTACK_ACK");
		return TRUE;
	}

	pAttackerCharObject->SetAnimation( XiahAniType::eLAT_NormalAttack, XiahAniType::eLAT_Stand, -1, 0);
	
	float fAttackSpeed = (float)bAttackSpeed / 9.0f;
	if( fAttackSpeed > 3.0f)
		fAttackSpeed = 3.0f;

	//HT_CHEAT : Restored to use server-driven attack speed
	pAttackerCharObject->m_CharRender.SetAnimationSpeed( fAttackSpeed);

	pAttackerCharObject->m_bMoveable = FALSE;
	pAttackerCharObject->m_bAttack = TRUE;
	pAttackerCharObject->m_bTargetMove = FALSE;

	if( pDefender)
	{
		CXiahCharObject* pDefenderCharObject = (CXiahCharObject*) pDefender->m_pObject;
		if(pDefenderCharObject == NULL)
		{
			DBG_LogFile("Invalid Position in OnCS_BT_PREATTACK_ACK2");
			return TRUE;
		}
		
		pAttackerCharObject->SetAngleTarget( pDefenderCharObject->m_Position);
	}

	if( g_pMainChar == pAttacker)
	{
		g_MainChar_PreAttackInfo.bAttackType = bAttackType;
		g_MainChar_PreAttackInfo.dwAttackID = dwAttackID;
		g_MainChar_PreAttackInfo.wAttackPosX = wAttackPosX;
		g_MainChar_PreAttackInfo.wAttackPosY = wAttackPosY;
		g_MainChar_PreAttackInfo.bAttackHeight = bAttackHeight;
		g_MainChar_PreAttackInfo.bDefType = bDefType;
		g_MainChar_PreAttackInfo.dwDefID = dwDefID;
		g_MainChar_PreAttackInfo.bAttackMode = bAttackMode;
		g_MainChar_PreAttackInfo.nRemainAttackCount = pAttackerCharObject->m_CharRender.GetTimerTriggerCount();;

		pAttackerCharObject->m_bAttackType = 0;

	//	g_MainChar_PreAttackInfo.bAttackReq = true;
		//HT_1026 :  
	//	if(g_MainChar_PreAttackInfo.bPreAttackReq)// && (dwAttackID == g_pMainChar->m_dwServerID))
	//		g_MainChar_PreAttackInfo.bPreAttackReq = false;
		//if(!g_MainChar_PreAttackInfo.bPreAttackReq)
		//	g_MainChar_PreAttackInfo.bPreAttackReq = true;

	}
	else if( g_PetList.Find( dwAttackID) != NULL)
	{
		sPetInfo* pPetInfo = g_PetList.GetPetInfo( dwAttackID);

		if( pPetInfo)
		{
			pPetInfo->dwLastAttackTime = g_dwCurTime;

			pPetInfo->bAttackType = bAttackType;
			pPetInfo->dwAttackID = dwAttackID;
			pPetInfo->wAttackPosX = wAttackPosX;
			pPetInfo->wAttackPosY = wAttackPosY;
			pPetInfo->bAttackHeight = bAttackHeight;
			pPetInfo->bDefType = bDefType;
			pPetInfo->dwDefID = dwDefID;
			pPetInfo->bAttackMode = bAttackMode;
			pPetInfo->nRemainAttackCount = pAttackerCharObject->m_CharRender.GetTimerTriggerCount();;
		
			if( pPetInfo->nRemainAttackCount == 0)
			{
				SendCS_BT_ATTACK_REQ(pPetInfo->bAttackType, 
					pPetInfo->dwAttackID, 
					pPetInfo->wAttackPosX, 
					pPetInfo->wAttackPosY, 
					pPetInfo->bAttackHeight, 
					pPetInfo->bDefType, 
					pPetInfo->dwDefID, 
					pPetInfo->bAttackMode);
			}
		}
	}
	
	return TRUE;
}

//---------------------------------------------------------------------------------------
int OnCS_BT_ATTACK_ACK(CMsg &msg)
{
	//sString szText;
	BYTE	bResult			=0;
	BYTE	bAttackMode		=0;
	BYTE	bAtkType		=0;
	DWORD	dwAtkID			=0;
	WORD	wAtkPosX		=0;
	WORD	wAtkPosY		=0;
	BYTE	bAtkHeight		=0;
	BYTE	bDefType		=0;
	DWORD	dwDefID			=0;
	DWORD	dwDefHpMax		=0;
	DWORD	dwDefHpCur		=0;
	DWORD	wDamage	=0; //HT_0907 : 莘 平 ( 65000 汛俅 绚; )
	DWORD	dwExp			=0;
	BYTE	bHitFlag		=0;

	msg
		>> bResult
		>> bAtkType
		>> dwAtkID
		>> wAtkPosX
		>> wAtkPosY
		>> bAtkHeight
		>> bDefType
		>> dwDefID
		>> dwDefHpMax
		>> dwDefHpCur
		>> wDamage
		>> dwExp
		>> bAttackMode
		>> bHitFlag; // bHitFlag 1谈 农萍 Hit

	if(wAtkPosX < 0 || wAtkPosX > 2047 || wAtkPosY < 0 || wAtkPosY > 2047) 
	{
		DBG_LogFile("Invalid Position in OnCS_BT_ATTACK_ACK");
		return TRUE;
	}

	XiahObject::CXiahObject* pAttacker = XiahObject::g_XiahObjectManager.FindXiahObject( MAKEOBJECTID( 0, dwAtkID,bAtkType));
	XiahObject::CXiahObject* pDefender = XiahObject::g_XiahObjectManager.FindXiahObject( MAKEOBJECTID( 0, dwDefID,bDefType));
	
	if(pAttacker == NULL || pDefender == NULL)
	{
		//DBG_LogFile( _T("OnCS_BT_ATTACK_ACK1 "));
		return 0;
	}

	if( pAttacker == NULL)
	{
		ValidateObject( bAtkType, dwAtkID, wAtkPosX, wAtkPosY);
		return TRUE;
	}

	if( pDefender == NULL)
	{
		ValidateObject( bDefType, dwDefID, wAtkPosX, wAtkPosY);
		return TRUE;
	}

	CXiahCharObject* pAttackerCharObject = NULL;
	CXiahCharObject* pDefenderCharObject = (CXiahCharObject*) pDefender->m_pObject;

	if(pDefenderCharObject == NULL)
	{
		DBG_LogFile( _T("OnCS_BT_ATTACK_ACK2 "));
		return TRUE;
	}

	// 2004.07.12 Changth
	//  贪  .
	if( pDefenderCharObject->m_nCurMotionType == XiahAniType::eLAT_Die ||
        pDefenderCharObject->m_nCurMotionType == XiahAniType::eLAT_Died  )
	{
		return TRUE;
	}

	if( pAttacker)
	{
		pAttackerCharObject = (CXiahCharObject*) pAttacker->m_pObject;

		if(pAttackerCharObject == NULL)
		{
			DBG_LogFile( _T("OnCS_BT_ATTACK_ACK3 "));
			return TRUE;
		}

		if( pDefenderCharObject->m_bTargetMove == FALSE && pDefenderCharObject->m_bRotatable == TRUE)
			pDefenderCharObject->SetAngleTarget( pAttackerCharObject->m_Position);

		if( pAttackerCharObject->m_bTargetMove == FALSE && pAttackerCharObject->m_bRotatable == TRUE)
			pAttackerCharObject->SetAngleTarget( pDefenderCharObject->m_Position);
	}

	//   HP  飘 冒 俨  鼐 .
	if( pDefenderCharObject->m_bObjType == OBJTYPE_FUNCTIONALNPC )
	{
		sFunctionalNpcInfo* pInfo = (sFunctionalNpcInfo*)pDefenderCharObject->m_pPrivateData;
	
		//YS_0811 : BUGFIX
		if ( !pInfo )
			return TRUE;

		BYTE bInfoKind = pInfo->m_bKind;
		BYTE bInfoType = pInfo->m_bType;

		//  
		if( bInfoKind == 100 )
		{
			// HP  隙, 赘 MeshType 0隙.
			if( dwDefHpMax/2 >= dwDefHpCur && pDefenderCharObject->m_CharRender.GetMeshType() == 0 )
			{
				sArrayData *pData = XiahArrayIndex::g_FunctionalNpcType.GetData(bInfoType);

				if(pData == NULL)
					return TRUE;

				int nCharID = pData->GetInt(1);
				if(XiahGameEngine::GetCharacter(nCharID) == NULL)
					return TRUE;

				CRes_Character* pResChar = XiahGameEngine::GetCharacter( nCharID);

				if( pResChar == NULL) return TRUE;
				if( !pResChar->GetMesh( 1 )) return TRUE;
				if( pResChar->GetMesh( 1 )->GetTexture( 0 ) == NULL) return TRUE;

				// 喂掳 梅 俨卮.   士 鸥 .
				float fAngle = pDefenderCharObject->m_Angle;
				float fX =  pDefenderCharObject->m_Position.x;
				float fZ = -pDefenderCharObject->m_Position.z;
				sString szName = pDefenderCharObject->m_szObjectName;
				BYTE bSubObjType = pDefenderCharObject->m_bSubObjType;

				sFunctionalNpcInfo pTempInfo;
				memcpy( &pTempInfo, pInfo, sizeof(sFunctionalNpcInfo) );

				// 喂掳 梅 俨卮.
				pDefenderCharObject->Create( nCharID, 1, 0, -1);
				pDefenderCharObject->m_pAniType = NULL;

				pDefenderCharObject->SetAngle(fAngle);
				pDefenderCharObject->SetPosition(fX, fZ);
				pDefenderCharObject->m_bRotatable = FALSE;

				pDefenderCharObject->m_szObjectName = szName;
				pDefenderCharObject->m_bObjType = OBJTYPE_FUNCTIONALNPC;
				pDefenderCharObject->m_bSubObjType = bSubObjType;

				pDefenderCharObject->m_dwCurHP = dwDefHpCur;
				pDefenderCharObject->m_dwMaxHP = dwDefHpMax;

				sFunctionalNpcInfo* pNewInfo = new sFunctionalNpcInfo;
				memcpy( pNewInfo, &pTempInfo, sizeof(sFunctionalNpcInfo) );

				pDefenderCharObject->m_pPrivateData = (DWORD)pNewInfo;
				pDefenderCharObject->m_PrivateDataDestoryer = ReleaseFunctionalNpcInfo;

				// Effect
				int nAry[2];
				nAry[0] = 0;
				nAry[1] = 1;	// 2掳  飘.
				pDefenderCharObject->m_CharRender.MakeMeshEffect( 2, nAry );

			}// HP  隙
		}//if( bInfoKind == 1 )
	}

	//
	CXiahCharObject* pMainChar = (CXiahCharObject*)g_pMainChar->m_pObject;

	//  OBJ 伟谈 SOUND FX 峡  Type 执麓.
	if(pAttackerCharObject == pMainChar)
	{
		pAttackerCharObject->m_dwEnemyType = g_FXType[pDefenderCharObject->m_bSubObjType];
	}


#define PLAYER1_CRITICAL_FX		50001201	// 丝 CRITICAL 
#define PLAYER2_CRITICAL_FX		50001202	//  CRITICAL 
#define PLAYER3_CRITICAL_FX		50001203	//  CRITICAL 
#define PLAYER4_CRITICAL_FX		50001887	//  CRITICAL 

#define PLAYER1_DAMAGE1_FX1		50001253	// 丝 1 瞎 卤
#define PLAYER1_DAMAGE1_FX2		50001256	// 丝 2 农萍 卤
#define PLAYER1_DAMAGE2_FX1		50001254	//  1
#define PLAYER1_DAMAGE2_FX2		50001257	//  2
#define PLAYER1_DAMAGE3_FX1		50001255	//  1
#define PLAYER1_DAMAGE3_FX2		50001258	//  2
#define PLAYER1_DAMAGE4_FX1		50001888	//  1 瞎 卤
#define PLAYER1_DAMAGE4_FX2		50001889	//  2 农萍 卤

	if(bHitFlag && pAttackerCharObject == pMainChar)
	{
		switch(pAttackerCharObject->m_bSubObjType)
		{
		// 丝
		case 1:
			g_MainCharInfo.PlayInterfaceSound(PLAYER1_CRITICAL_FX);
			break;
		// 
		case 2:
			g_MainCharInfo.PlayInterfaceSound(PLAYER2_CRITICAL_FX);
			break;
		// 
		case 3:
			g_MainCharInfo.PlayInterfaceSound(PLAYER3_CRITICAL_FX);
			break;
		// 
		case 4:
			g_MainCharInfo.PlayInterfaceSound(PLAYER4_CRITICAL_FX);
			break;
		}
	}

	// 农萍 hit  
	if(bHitFlag && g_cj && pAttackerCharObject == pMainChar)
	{
		HRESULT hr;
		hr = g_cj->Change_Para(1500,BIG_STRONG);
		hr = g_cj->Rumble_Start();
	}

	// NORMAL PC DAMAGE (   )
	if(pDefenderCharObject == pMainChar)
	{
		HRESULT hr;
		// 
		if(wDamage && g_cj)//HT_0907 :  平 ( 65000  绚; )
		{
			hr = g_cj->Change_Para(1000,SMALL_STRONG);
			hr = g_cj->Rumble_Start();
		}

		// DAMAGE SOUND
		switch(pDefenderCharObject->m_bSubObjType)
		{
			// 丝
			case 1 :
				{
					if(bHitFlag)	// 农萍
						g_MainCharInfo.PlayInterfaceSound(PLAYER1_DAMAGE1_FX2);
					else
						g_MainCharInfo.PlayInterfaceSound(PLAYER1_DAMAGE1_FX1);
				}
				break;
			// 
			case 2 :
				{
					if(bHitFlag)	// 农萍
						g_MainCharInfo.PlayInterfaceSound(PLAYER1_DAMAGE2_FX2);
					else
						g_MainCharInfo.PlayInterfaceSound(PLAYER1_DAMAGE2_FX1);
				}
				break;
			// 
			case 3 :
				{
					if(bHitFlag)	// 农萍
						g_MainCharInfo.PlayInterfaceSound(PLAYER1_DAMAGE3_FX2);
					else
						g_MainCharInfo.PlayInterfaceSound(PLAYER1_DAMAGE3_FX1);
				}
				break;
			case 4:
				{
					if(bHitFlag)	// 农萍
						g_MainCharInfo.PlayInterfaceSound(PLAYER1_DAMAGE4_FX1);
					else
						g_MainCharInfo.PlayInterfaceSound(PLAYER1_DAMAGE4_FX2);
				}
				break;
		}
	}

	Vector3 vHitEffectPos = pDefenderCharObject->m_Position + Vector3( 0, pDefenderCharObject->m_LocalBound.m_vMax.y, 0);
	Vector3 vHitEffectPos2 = pDefenderCharObject->m_Position + Vector3( 0, pDefenderCharObject->m_LocalBound.m_vMax.y - 3.0f, 0);

	int nHitEffectType1;

	if( pDefenderCharObject->m_bObjType == OBJTYPE_PC )
	{
		nHitEffectType1 = 0;
	}
	else
	{
		nHitEffectType1 = 1;
	}

	BOOL bSetHitAnimation = TRUE;

	if( pDefenderCharObject->m_bAttack || pDefenderCharObject->m_bTargetMove)
		bSetHitAnimation = FALSE;

	if( pDefender == g_pMainChar)
	{
		g_MainChar_PreAttackInfo.nRemainAttackCount	 = 0;
		g_MainChar_PreAttackInfo.dwLastPreAttackTime = 0;
	}

	int nHitEffectType2;

	//if(pAttackerCharObject == pMainChar)
	//{
	//	//HT_1026 :  
	//	if(!g_MainChar_PreAttackInfo.bAttackReq && !g_MainChar_PreAttackInfo.bPreAttackReq)
	//	{
	//		g_MainChar_PreAttackInfo.bAttackReq = true;
	//		g_MainChar_PreAttackInfo.bPreAttackReq = true;
	//	}
	//	else if(g_MainChar_PreAttackInfo.bPreAttackReq)
	//		g_MainCharInfo.ShowHelpMessage(CH_WARNNIG4);
	//}

	switch( bResult)
	{
	case 0:
		//g_MainCharInfo.ShowHelpMessage( "Attack_Result 0", TEXTEFFECT_COLOR_GENERAL);
		//if(pAttackerCharObject == pMainChar)
		//{
		//	//HT_1026 :  
		//	/*if(!g_MainChar_PreAttackInfo.bAttackReq && !g_MainChar_PreAttackInfo.bPreAttackReq)
		//	{
		//		g_MainChar_PreAttackInfo.bAttackReq = true;
		//		g_MainChar_PreAttackInfo.bPreAttackReq = true;
		//	}*/
		//	//else if(g_MainChar_PreAttackInfo.bPreAttackReq)
		//	//	g_MainCharInfo.ShowHelpMessage(CH_WARNNIG4);
		//}

		break;

	case 1: //   
		//if(pAttackerCharObject == pMainChar)
		//{
		//	//HT_1026 :  
		//	/*if(!g_MainChar_PreAttackInfo.bAttackReq && !g_MainChar_PreAttackInfo.bPreAttackReq)
		//	{
		//		g_MainChar_PreAttackInfo.bAttackReq = true;
		//		g_MainChar_PreAttackInfo.bPreAttackReq = true;
		//	}*/
		////	else if(g_MainChar_PreAttackInfo.bPreAttackReq)
		////	
		////		g_MainCharInfo.ShowHelpMessage(CH_WARNNIG4);
		////	
		//}
		if( bSetHitAnimation)
			pDefenderCharObject->SetAnimation( XiahAniType::eLAT_Defend, XiahAniType::eLAT_Stand, 0, 0);
		pDefenderCharObject->m_bMoveable = FALSE;

		if( nHitEffectType1 == 0 ) // Red
			nHitEffectType2 = 2;	// miss
		else
			nHitEffectType2 = 3;
		g_HitEffect.AddHitEffect( nHitEffectType2, 0, vHitEffectPos );
		break;

	case 2:	
		//if(pAttackerCharObject == pMainChar)
		//{
		//	//HT_1026 :  
		//	if(!g_MainChar_PreAttackInfo.bAttackReq && !g_MainChar_PreAttackInfo.bPreAttackReq)
		//	{
		//		g_MainChar_PreAttackInfo.bAttackReq = true;
		//		g_MainChar_PreAttackInfo.bPreAttackReq = true;
		//	}
		////	else if(g_MainChar_PreAttackInfo.bPreAttackReq)
		////		g_MainCharInfo.ShowHelpMessage(CH_WARNNIG4);
		//}
		if( wDamage == 0)//HT_0907 : 莘 平 ( 65000 汛俅 绚; )
		{
			if( bSetHitAnimation)
				pDefenderCharObject->SetAnimation( XiahAniType::eLAT_Defend, XiahAniType::eLAT_Stand, 0, 0);
			pDefenderCharObject->m_bMoveable = FALSE;


			if( nHitEffectType1 == 0 ) // Red
				nHitEffectType2 = 2;
			else
				nHitEffectType2 = 3;

			g_HitEffect.AddHitEffect( nHitEffectType2, 0, vHitEffectPos );
		}
		else //   
		{
			if( bSetHitAnimation)
				pDefenderCharObject->SetAnimation( XiahAniType::eLAT_Hit, XiahAniType::eLAT_Stand, 0, 0);
			pDefenderCharObject->m_bMoveable = FALSE;
			pDefenderCharObject->m_dwCurHP = dwDefHpCur;
			pDefenderCharObject->m_dwMaxHP = dwDefHpMax;

			if( pDefenderCharObject->m_bObjType == OBJTYPE_PET && g_PetList.Find( pDefender->m_dwServerID) != NULL)
			{
				sPetInfo* pPetInfo = (sPetInfo*)pDefenderCharObject->m_pPrivateData;

				//YS_0811 : BUGFIX
				if(pPetInfo == NULL)
				{					
					return 0;
				}

				pPetInfo->dwHpCur = dwDefHpCur;
				pPetInfo->dwHpMax = dwDefHpMax;
			}


			if( nHitEffectType1 == 0 ) // Red
				nHitEffectType2 = 0;
			else
				nHitEffectType2 = 1;

			g_HitEffect.AddHitEffect( nHitEffectType2, wDamage, vHitEffectPos );//HT_0907 :  平 ( 65000  绚; )

			// Critical hit
			if( bHitFlag )
				g_HitEffect.AddHitEffect( 4, wDamage, vHitEffectPos2 );//HT_0907 :  平 ( 65000  绚; )

			// 鸥 飘 摹 
			int nPosX = 0, nPosY = 0, nPosZ = 0, nStartTime = 0, nEffectType = 0;

			GetHitEffectType( pDefenderCharObject->m_bObjType, pDefenderCharObject->m_bSubObjType, 
							  nPosX, nPosY, nPosZ, nStartTime,
							  pAttackerCharObject->m_bObjType, pAttackerCharObject->m_bSubObjType,
							  nEffectType );

			if (nEffectType != 0)
			{
				_EFFECTPACKAGE* pEffectPackage = g_EffectManager.EnqHitEffectImmediately( nEffectType, nStartTime, nPosX, nPosY, nPosZ );
				// matrix
				if( pEffectPackage && pEffectPackage->pEffectRender && pEffectPackage->pEffectRender->pPackagePair )
				{
//					pEffectPackage->pEffectRender->pPackagePair->WorldMatrix = (MATRIX)pDefenderCharObject->m_ObjectTM;
					pEffectPackage->pEffectRender->pPackagePair->pWorldMatrix = (MATRIX*)pDefenderCharObject->m_CharRender.GetCharTM();

					pDefenderCharObject->m_EffectPPList.push_back( pEffectPackage->pEffectRender->pPackagePair );
				}// if
			}
		}
		break;
	}

	return TRUE;
}

//---------------------------------------------------------------------------------------
int OnCS_BT_NPCPREATTACK_ACK(CMsg &msg)
{
	BYTE bAtkType;
	WORD dwAtkID;
	BYTE bDefType;
	DWORD dwDefID;
	BYTE bAttackMode;
	WORD wAtkPosX;
	WORD wAtkPosY;
	BYTE bAtkHeight;

	msg
		>> bAtkType
		>> dwAtkID
		>> bDefType
		>> dwDefID
		>> wAtkPosX
		>> wAtkPosY
		>> bAtkHeight
		>> bAttackMode;	
	return TRUE;
}

//---------------------------------------------------------------------------------------
int OnCS_BT_NPCATTACK_ACK(CMsg &msg)
{
	BYTE bResult;
	BYTE bAttackMode;
	BYTE bAtkType;
	DWORD dwAtkID;
	WORD wAtkPosX;
	WORD wAtkPosY;
	BYTE bAtkHeight;
	BYTE bDefType;
	DWORD dwDefID;
	DWORD dwDefHpMax;
	DWORD dwDefHpCur;
	DWORD wDamage;
	DWORD dwExp;

	msg
		>> bResult
		>> bAtkType
		>> dwAtkID
		>> wAtkPosX
		>> wAtkPosY
		>> bAtkHeight
		>> bDefType
		>> dwDefID
		>> dwDefHpMax
		>> dwDefHpCur
		>> wDamage
		>> dwExp
		>> bAttackMode;

	return TRUE;
}

//---------------------------------------------------------------------------------------
int OnCS_BT_NPCPRESHOT_ACK(CMsg &msg)
{
	BYTE bAtkType;
	DWORD dwAtkID;
	WORD wPosX;
	WORD wPosY;
	BYTE bHeight;
	WORD wDesPosX;
	WORD wDesPosY;
	BYTE bDesHeight;
	WORD wLifeTime;
	BYTE bAttackMode;

	msg
		>> bAtkType
		>> dwAtkID
		>> wPosX
		>> wPosY
		>> bHeight
		>> wDesPosX
		>> wDesPosY
		>> bDesHeight
		>> wLifeTime
		>> bAttackMode;

	return TRUE;
}

//---------------------------------------------------------------------------------------
int OnCS_BT_NPCSHOT_ACK	(CMsg &msg)
{
	BYTE bAttackMode;
	BYTE bAtkType;
	DWORD dwAtkID;
	WORD wAtkPosX;
	WORD wAtkPosY;
	BYTE bAtkHeight;
	BYTE bDefType;
	DWORD dwDefID;
	DWORD dwDefHpMax;
	DWORD dwDefHpCur;
	DWORD wDamage;
	DWORD dwExp;

	msg
		>> bAtkType
		>> dwAtkID
		>> wAtkPosX
		>> wAtkPosY
		>> bAtkHeight
		>> bDefType
		>> dwDefID
		>> dwDefHpMax
		>> dwDefHpCur
		>> wDamage
		>> dwExp
		>> bAttackMode;

	return TRUE;
}

//---------------------------------------------------------------------------------------
int OnCS_BT_EXECSP_ACK( CMsg &msg)
{
	BYTE bResult;
	BYTE bSpType;
	BYTE bSpValue;
	WORD wRemainSp;
	WORD wResultSpTypeValue;

	msg
		>> bResult
		>> bSpType
		>> bSpValue
		>> wRemainSp
		>> wResultSpTypeValue;

	if(bResult != 0) 
	{
		g_MainCharInfo.ShowHelpMessage(IDS_SHORT_ABILITY, TEXTEFFECT_COLOR_WARNING);
	}	
	else
	{
		g_MainCharInfo.m_wRemainSp = wRemainSp;
		switch(bSpType)
		{
		case SP_STR:
			g_MainCharInfo.m_wStr = wResultSpTypeValue;
			break;
		case SP_SUS:
			g_MainCharInfo.m_wSus = wResultSpTypeValue;
			break;
		case SP_DEX:
			g_MainCharInfo.m_wDex = wResultSpTypeValue;
			break;
		case SP_VIT:
			g_MainCharInfo.m_wVit = wResultSpTypeValue;
			break;
		}
		g_MainCharInfo.RefreshChracterInfo();
	}

	return TRUE;
}


//---------------------------------------------------------------------------------------
int OnCS_BT_LEARNMUGONG_ACK( CMsg &msg)
{
	BYTE	bResult;
	DWORD	dwMugongID;
	BYTE	bLevel;
	WORD	wUsedTP;
	WORD	wRemainedTP;
	
	msg >> bResult
		>> dwMugongID
		>> bLevel 
		>> wUsedTP			// ...
		>> wRemainedTP ;

	if(bResult == 0)
	{
		if(dwMugongID >= 161 && dwMugongID <= 179)
		{
			sArrayData* pRebirthData = XiahArrayIndex::g_RebirthMugong_List.GetData( dwMugongID);

			if(pRebirthData)
			{
				g_MainCharInfo.m_pMugong->InsertMugong(dwMugongID, 0, bLevel);				
			}			
		}
		else if(dwMugongID >= 191 && dwMugongID <= 198) //HT_0711 : 
		{
			wUsedTP = 3;

			MugongMap::iterator it = g_MainCharInfo.m_pMugong->m_map2ThRebirthMugong.begin();

			sMugongInfo* pMugong = new sMugongInfo(); 
			
			pMugong= it->second;
			
			if(pMugong && it != g_MainCharInfo.m_pMugong->m_map2ThRebirthMugong.end())
			{
				if(pMugong->m_dwMugongID == dwMugongID)
					wUsedTP = pMugong->m_byMugongType;
				else
					wUsedTP = 4;
			}

			sArrayData* pData = XiahArrayIndex::g_MugongTemplate.GetData( dwMugongID);

			if(pData)
			{
				g_MainCharInfo.m_pMugong->InsertMugong( dwMugongID, wUsedTP, bLevel);
			}		
		}
		else
		{
			sArrayData* pData = XiahArrayIndex::g_MugongTemplate.GetData( dwMugongID);

			if(pData)
			{
				BYTE bType = pData->GetInt( 3);
				if (dwMugongID >= 150 && dwMugongID <= 154)
				{
					bType = MUGONGTYPE_FIVEELEMENT;
				}
				g_MainCharInfo.m_pMugong->InsertMugong( dwMugongID, bType, bLevel);
			}
		}

		if(bLevel == 1)
			g_MainCharInfo.ShowHelpMessage( IDS_LEARN_MUGONG);

		g_MainCharInfo.m_wRemainTp = wRemainedTP;

		g_MainCharInfo.RefreshMugongFrame( TRUE);
		g_MainCharInfo.PlayInterfaceSound( READ_BOOK_SOUND );
		
		return 0;
	}

	switch( bResult)
	{
	case 1:
		g_MainCharInfo.ShowHelpMessage( IDS_SHORT_DEX1, TEXTEFFECT_COLOR_WARNING);
		break;
	case 2:
		g_MainCharInfo.ShowHelpMessage( IDS_SHORT_PWR1, TEXTEFFECT_COLOR_WARNING);
		break;
	case 3:
		g_MainCharInfo.ShowHelpMessage( IDS_SHORT_AGI1, TEXTEFFECT_COLOR_WARNING);
		break;
	case 4:
		g_MainCharInfo.ShowHelpMessage( IDS_SHORT_LIFE1, TEXTEFFECT_COLOR_WARNING);
		break;
	case 5:
		g_MainCharInfo.ShowHelpMessage( IDS_SHORT_LEVEL, TEXTEFFECT_COLOR_WARNING);
		break;
	case 6:
		g_MainCharInfo.ShowHelpMessage( IDS_NEED_MUGONG_BOOK, TEXTEFFECT_COLOR_WARNING);
		break;
	case 7:
		g_MainCharInfo.ShowHelpMessage( IDS_NO_MUGONG, TEXTEFFECT_COLOR_WARNING);
		break;
	case 8:
		g_MainCharInfo.ShowHelpMessage( IDS_SHORT_TP1, TEXTEFFECT_COLOR_WARNING);
		break;
	case 9:
		g_MainCharInfo.ShowHelpMessage( IDS_SHORT_TP1, TEXTEFFECT_COLOR_WARNING);
		break;
	case 10:
		g_MainCharInfo.ShowHelpMessage( IDS_SHORT_MUGONGLEVEL, TEXTEFFECT_COLOR_WARNING);
		break;
	case 11:
		g_MainCharInfo.ShowHelpMessage( _T("Error"), TEXTEFFECT_COLOR_WARNING);
		break;
	case 14:
        TCHAR szTemp[128] = {0,};
		_stprintf(szTemp, IDS_REBIRTH_MUGONG_BOOK_01, bLevel);
		g_MainCharInfo.ShowHelpMessage( szTemp, TEXTEFFECT_COLOR_WARNING);
		break;
	}

	g_MainCharInfo.PlayInterfaceSound( ISOUND_WARNING);

	return 0;
}

//---------------------------------------------------------------------------------------
int OnCS_BT_SELMUGONG_ACK( CMsg &msg)
{
	BYTE	bResult		=0;
	BYTE	bType		=0;	
	BYTE	bIndex		=0;
	DWORD	dwMugongID	=0;
	
	msg
		>> bResult
		>> bType
		>> dwMugongID 
		>> bIndex;
	
	if( bResult)
	{
		g_MainCharInfo.m_pSlot->SetActiveSlot( bIndex, 0);
	}
	else
	{
		g_MainCharInfo.m_pSlot->SetActiveSlot( bIndex, dwMugongID);
	}

	return 0;
}

//---------------------------------------------------------------------------------------
int OnCS_BT_PRESHOT_ACK( CMsg &msg)
{
	BYTE	bAtkType;
	DWORD	dwAtkID; 
	WORD	wPosX; 
	WORD	wPosY; 
	BYTE	bHeight; 
	WORD	wDesPosX; 
	WORD	wDesPosY; 
	BYTE	bDesHeight; 
	WORD	wLifeTime; 
	BYTE	bAttackMode;
	DWORD	dwDefID;
	BYTE	bDefType;

	msg
		>> bAtkType
		>> dwAtkID
		>> wPosX
		>> wPosY
		>> bHeight
		>> wDesPosX
		>> wDesPosY
		>> bDesHeight
		>> wLifeTime
		>> bAttackMode
		>> dwDefID
		>> bDefType;

	if(wPosX < 0 || wPosX > 2047 || wPosY < 0 || wPosY > 2047) 
	{
		DBG_LogFile("Invalid Position in OnCS_BT_ATTACK_ACK");
		return TRUE;
	}

	XiahObject::CXiahObject* pAttacker = XiahObject::g_XiahObjectManager.FindXiahObject( MAKEOBJECTID( 0, dwAtkID,bAtkType));
	XiahObject::CXiahObject* pDefender = XiahObject::g_XiahObjectManager.FindXiahObject( MAKEOBJECTID( 0, dwDefID,bDefType));
	
	if( pAttacker == NULL)
	{
		ValidateObject( bAtkType, dwAtkID, wPosX, wPosY);
		return TRUE;
	}

	// 嗫?鸥 飘 执俑 TargetObject  
	if( pDefender)
	{
		CXiahCharObject* pDefenderCharObject = (CXiahCharObject*) pDefender->m_pObject;

		if(pDefenderCharObject == NULL)
		{
			DBG_LogFile( _T("OnCS_BT_PRESHOT_ACK "));
			return TRUE;
		}

		pDefenderCharObject->GetPosition( wDesPosX, wDesPosY);
	}

	CXiahCharObject* pAttackerCharObject = (CXiahCharObject*) pAttacker->m_pObject;

	if(pAttackerCharObject == NULL)
	{
		DBG_LogFile( _T("OnCS_BT_PRESHOT_ACK "));
		return TRUE;
	}

	pAttackerCharObject->SetAnimation( XiahAniType::eLAT_NormalAttack, XiahAniType::eLAT_Stand, -1, 0);
	pAttackerCharObject->m_bMoveable = FALSE;
	pAttackerCharObject->m_bAttack = TRUE;

	pAttackerCharObject->SetAngleTarget( wDesPosX, wDesPosY);

	pAttackerCharObject->m_ShotAttackInfo.wDesPosX		= wDesPosX;
	pAttackerCharObject->m_ShotAttackInfo.wDesPosY		= wDesPosY;
	pAttackerCharObject->m_ShotAttackInfo.bDesHeight	= bDesHeight;
	pAttackerCharObject->m_ShotAttackInfo.wLifeTime		= wLifeTime;
	pAttackerCharObject->m_ShotAttackInfo.dwTargetID	= dwDefID;
	pAttackerCharObject->m_ShotAttackInfo.dwObjType		= bDefType;

	return TRUE;
}


//---------------------------------------------------------------------------------------
int OnCS_BT_SHOT_ACK( CMsg &msg)
{
	//sString szText;
	BYTE	bAttackMode	=0;
	BYTE	bAtkType	=0;
	BYTE	bAtkHeight	=0;
	BYTE	bDefType	=0;		
	DWORD	dwAtkID		=0;
	DWORD	dwDefID		=0;
	DWORD	dwExp		=0;
	WORD	wAtkPosX	=0;
	WORD	wAtkPosY	=0;
	DWORD	dwDefHpMax	=0;
	DWORD	dwDefHpCur	=0;
	DWORD	wDamage		=0;

	msg
		>> bAtkType
		>> dwAtkID
		>> wAtkPosX
		>> wAtkPosY
		>> bAtkHeight
		>> bDefType
		>> dwDefID
		>> dwDefHpMax
		>> dwDefHpCur
		>> wDamage //HT_0907 :  平 ( 65000  绚; )
		>> dwExp
		>> bAttackMode;

	if(wAtkPosX < 0 || wAtkPosX > 2047 || wAtkPosY < 0 || wAtkPosY > 2047) 
	{
		DBG_LogFile("Invalid Position in OnCS_BT_ATTACK_ACK");
		return TRUE;
	}

	XiahObject::CXiahObject* pAttacker = XiahObject::g_XiahObjectManager.FindXiahObject( MAKEOBJECTID( 0, dwAtkID,bAtkType));
	XiahObject::CXiahObject* pDefender = XiahObject::g_XiahObjectManager.FindXiahObject( MAKEOBJECTID( 0, dwDefID,bDefType));

	if( pAttacker == NULL)
	{
		ValidateObject( bAtkType, dwAtkID, wAtkPosX, wAtkPosY);
		return TRUE;
	}

	if( pDefender == NULL)
	{
		ValidateObject( bDefType, dwDefID, wAtkPosX, wAtkPosY);
		return TRUE;
	}

	CXiahCharObject* pDefenderCharObject = (CXiahCharObject*) pDefender->m_pObject;	
	CXiahCharObject* pAttackerCharObject = NULL;

	if(pDefenderCharObject == NULL)
	{
		DBG_LogFile( _T("OnCS_BT_SHOT_ACK "));
		return TRUE;
	}

	if( pAttacker)
	{
		pAttackerCharObject = (CXiahCharObject*) pAttacker->m_pObject;

		if(pAttackerCharObject == NULL)
		{
			DBG_LogFile( _T("OnCS_BT_SHOT_ACK "));
			return TRUE;
		}

		if( pDefenderCharObject->m_bTargetMove == FALSE)
			pDefenderCharObject->SetAngleTarget( pAttackerCharObject->m_Position);
	}

	BOOL bSetHitAnimation = TRUE;

	if( pDefenderCharObject->m_bAttack || pDefenderCharObject->m_bTargetMove)
		bSetHitAnimation = FALSE;

	//   飘 摹 .
/*
	Vector3 scPos;
	sRect rcRect;
	D3DCOLOR TextEffectColor;
	if( g_pCurrentCamera )
	{
		scPos = g_pCurrentCamera->WorldToScreen( pDefenderCharObject->m_Position + Vector3( 0, pDefenderCharObject->m_LocalBound.m_vMax.y, 0));
		rcRect.left = scPos.x;
		rcRect.top = scPos.y;
	}
	else
	{
		rcRect.left = pDefenderCharObject->m_rcObjectScreenPos.left;
		rcRect.top = pDefenderCharObject->m_rcObjectScreenPos.top;
	}
*/
	Vector3 vHitEffectPos = pDefenderCharObject->m_Position + Vector3( 0, pDefenderCharObject->m_LocalBound.m_vMax.y, 0);

	int nHitEffectType1;

	if( pDefenderCharObject->m_bObjType == OBJTYPE_PC )
	{
		nHitEffectType1 = 0;
	}
	else
	{
		nHitEffectType1 = 1;
	}

	//
	int nHitEffectType2;
	if( wDamage == 0)//HT_0907 : 莘 平 ( 65000 汛俅 绚; )
	{
		pDefenderCharObject->SetAnimation( XiahAniType::eLAT_Defend, XiahAniType::eLAT_Stand, 0, 0);
		pDefenderCharObject->m_bMoveable = FALSE;

		//
		if( nHitEffectType1 == 0 ) // Red
			nHitEffectType2 = 2;	// miss
		else
			nHitEffectType2 = 3;

		g_HitEffect.AddHitEffect( nHitEffectType2, 0, vHitEffectPos );
	}
	else //   
	{
		if( bSetHitAnimation)
			pDefenderCharObject->SetAnimation( XiahAniType::eLAT_Hit, XiahAniType::eLAT_Stand, 0, 0);

		pDefenderCharObject->m_bMoveable = FALSE;
		pDefenderCharObject->m_dwCurHP = dwDefHpCur;
		pDefenderCharObject->m_dwMaxHP = dwDefHpMax;

		if( pDefenderCharObject->m_bObjType == OBJTYPE_PET && g_PetList.Find( pDefender->m_dwServerID) != NULL)
		{
			sPetInfo* pPetInfo = (sPetInfo*)pDefenderCharObject->m_pPrivateData;

			//YS_0811 : BUGFIX
			if(pPetInfo == NULL)
			{					
				return 0;
			}

			pPetInfo->dwHpCur = dwDefHpCur;
			pPetInfo->dwHpMax = dwDefHpMax;
		}

		//
		if( nHitEffectType1 == 0 ) // Red
			nHitEffectType2 = 0;
		else
			nHitEffectType2 = 1;

		g_HitEffect.AddHitEffect( nHitEffectType2, wDamage, vHitEffectPos );//HT_0907 : 莘 平 ( 65000 汛俅 绚; )

		int nPosX = 0, nPosY = 0, nPosZ = 0, nStartTime = 0, nEffectType = 0;

		GetHitEffectType( pDefenderCharObject->m_bObjType, pDefenderCharObject->m_bSubObjType, 
							nPosX, nPosY, nPosZ, nStartTime,
							pAttackerCharObject->m_bObjType, pAttackerCharObject->m_bSubObjType,
							nEffectType );

		if (nEffectType != 0)
		{
			_EFFECTPACKAGE* pEffectPackage = g_EffectManager.EnqHitEffectImmediately( nEffectType, nStartTime, nPosX, nPosY, nPosZ );
			// matrix
			if( pEffectPackage && pEffectPackage->pEffectRender && pEffectPackage->pEffectRender->pPackagePair )
			{
				//				pEffectPackage->pEffectRender->pPackagePair->WorldMatrix = (MATRIX)pDefenderCharObject->m_ObjectTM;
				pEffectPackage->pEffectRender->pPackagePair->pWorldMatrix = (MATRIX*)pDefenderCharObject->m_CharRender.GetCharTM();

				pDefenderCharObject->m_EffectPPList.push_back( pEffectPackage->pEffectRender->pPackagePair );
			}// if
		}
	}

	return TRUE;
}

//---------------------------------------------------------------------------------------
int OnCS_BT_MUGONGPREATTACK_ACK( CMsg &msg)
{
	BYTE bResult		=0;	
	BYTE bMugongLevel	=0;
	BYTE bAttackType	=0;
	BYTE bAttackHeight	=0;
	BYTE bDefenseType	=0;
	BYTE bTargetHeight	=0;
	BYTE bAttackMode	=0;
	DWORD dwMugongID	=0;
	DWORD dwAttackID	=0;
	DWORD dwDefenseID	=0;
	WORD wAttackPosX	=0;
	WORD wAttackPosY	=0;
	WORD wTargetPosX	=0;
	WORD wTargetPosY	=0;
	WORD wLifeTime		=0;

	//sString szText;

	msg
		>> bResult
		>> dwMugongID
		>> bMugongLevel
		>> bAttackType
		>> dwAttackID
		>> wAttackPosX
		>> wAttackPosY
		>> bAttackHeight
		>> bDefenseType
		>> dwDefenseID
		>> wTargetPosX
		>> wTargetPosY
		>> bTargetHeight
		>> wLifeTime
		>> bAttackMode;

	if(wAttackPosX < 0 || wAttackPosX > 2047 || wAttackPosY < 0 || wAttackPosY > 2047) 
	{
		DBG_LogFile("Invalid Position in OnCS_BT_ATTACK_ACK");
		return TRUE;
	}

	switch( bResult)
	{
	case 1:
		g_MainCharInfo.ShowHelpMessage( IDS_SHORT_INLIFE, TEXTEFFECT_COLOR_WARNING);
		break;
	case 2:
		g_MainCharInfo.ShowHelpMessage( IDS_NO_MUGONG, TEXTEFFECT_COLOR_WARNING);
		break;
	}
	

	if(bResult != 0)
		return TRUE;	//   寻 汛

	XiahObject::CXiahObject* pAttacker = XiahObject::g_XiahObjectManager.FindXiahObject( MAKEOBJECTID( 0, dwAttackID,bAttackType));
	XiahObject::CXiahObject* pDefender = XiahObject::g_XiahObjectManager.FindXiahObject( MAKEOBJECTID( 0, dwDefenseID,bDefenseType));

	if( pAttacker == NULL)
	{
		ValidateObject( bAttackType, dwAttackID, wAttackPosX, wAttackPosY);
		return TRUE;
	}


	// pDefender Null眉农 . 
	
	//   直
	CXiahCharObject* pAttackerCharObject = (CXiahCharObject*) pAttacker->m_pObject;

	if(pAttackerCharObject == NULL)
	{
		DBG_LogFile( _T("OnCS_BT_MUGONGPREATTACK_ACK "));
		return TRUE;
	}

	// [ModernControl] 寸草不生地面 AoE 毒雾持续粒子特效创建逻辑
	// 当服务端预施法成功应答到达时，如果是寸草不生（OUTGONGID_DOKMU 127），利用预施法包中携带的精确地表目标网格坐标 wTargetPosX/Y，
	// 构造一个完全静止的 3D 平移矩阵，在此坐标处为 pAttackerCharObject 创建持续 4 秒播放的 eDokmu 地面粒子特效，
	// 完美攻克地板粒子不持续播放和位置偏移的全部问题。
	if (dwMugongID == OUTGONGID_DOKMU)
	{
		if (pAttackerCharObject->m_pDokmuEffectPP)
		{
			g_EffectManager.DeqEffectPackagePair(pAttackerCharObject->m_pDokmuEffectPP);
			pAttackerCharObject->m_pDokmuEffectPP = NULL;
		}

		pAttackerCharObject->m_matDokmuWorld._11 = 1.0f; pAttackerCharObject->m_matDokmuWorld._12 = 0.0f; pAttackerCharObject->m_matDokmuWorld._13 = 0.0f; pAttackerCharObject->m_matDokmuWorld._14 = 0.0f;
		pAttackerCharObject->m_matDokmuWorld._21 = 0.0f; pAttackerCharObject->m_matDokmuWorld._22 = 1.0f; pAttackerCharObject->m_matDokmuWorld._23 = 0.0f; pAttackerCharObject->m_matDokmuWorld._24 = 0.0f;
		pAttackerCharObject->m_matDokmuWorld._31 = 0.0f; pAttackerCharObject->m_matDokmuWorld._32 = 0.0f; pAttackerCharObject->m_matDokmuWorld._33 = 1.0f; pAttackerCharObject->m_matDokmuWorld._34 = 0.0f;
		pAttackerCharObject->m_matDokmuWorld._41 = (float)wTargetPosX;
		pAttackerCharObject->m_matDokmuWorld._42 = pAttackerCharObject->m_Position.y; // 保持高度一致
		pAttackerCharObject->m_matDokmuWorld._43 = -(float)wTargetPosY;
		pAttackerCharObject->m_matDokmuWorld._44 = 1.0f;

		_EFFECTPACKAGE* pEffectPackage = g_EffectManager.EnqOutGongPersistEffectImmediately(eDokmu);
		if (pEffectPackage && pEffectPackage->pEffectRender && pEffectPackage->pEffectRender->pPackagePair)
		{
			pEffectPackage->pEffectRender->pPackagePair->pWorldMatrix = &pAttackerCharObject->m_matDokmuWorld;
			pAttackerCharObject->m_pDokmuEffectPP = pEffectPackage->pEffectRender->pPackagePair;

			DBG_LogFile(_T("[ClientMugongLog] Created ground AoE effect eDokmu for OUTGONGID_DOKMU at grid(%u,%u) -> world(%.2f, %.2f, %.2f)\n"),
				wTargetPosX, wTargetPosY, pAttackerCharObject->m_matDokmuWorld._41, pAttackerCharObject->m_matDokmuWorld._42, pAttackerCharObject->m_matDokmuWorld._43);
		}
	}

	// [Client-side Buff Animation & Attack Lock Bypass Patch]
	{
		bool isBuffSkill = dwMugongID == OUTGONGID_UNKIHAENG || dwMugongID == OUTGONGID_MUSUHON || 
						   dwMugongID == OUTGONGID_ILYUIDOGANG || dwMugongID == OUTGONGID_POKSAHON || 
						   dwMugongID == OUTGONGID_KUMKANGLUK || dwMugongID == OUTGONGID_BUSIN || 
						   dwMugongID == OUTGONGID_JOSIKSUL || dwMugongID == OUTGONGID_JUNYUUM || 
						   dwMugongID == OUTGONGID_YUESUSINYUNG || dwMugongID == OUTGONGID_KYOKANSU || 
						   dwMugongID == OUTGONGID_W0NKISINKANG || dwMugongID == OUTGONGID_WHANSUYUO || 
						   dwMugongID == OUTGONGID_KIYOESUL || dwMugongID == OUTGONGID_JILPUNGBO || 
						   dwMugongID == OUTGONGID_AMHUKMU || dwMugongID == OUTGONGID_TALBAKIN || 
						   dwMugongID == OUTGONGID_JUKUNKANGKI || dwMugongID == OUTGONGID_KUMNASU || 
						   dwMugongID == OUTGONGID_BANTANKANGKI || dwMugongID == OUTGONGID_ODOKCHIM || 
						   dwMugongID == OUTGONGID_CHOSANGBI || dwMugongID == OUTGONGID_DOKNAEGONG || 
						   dwMugongID == OUTGONGID_DOKHYULGONG || dwMugongID == OUTGONGID_DOKMU || 
						   dwMugongID == OUTGONGID_MANDOKBULJIN || dwMugongID == OUTGONGID_GYUISIKDAEBUB || 
						   dwMugongID == OUTGONGID_GWANGMADOKGONG || 
						   (dwMugongID >= 150 && dwMugongID <= 154);
		if (isBuffSkill)
		{
			DBG_LogFile(_T("[AngleDebug] OnCS_BT_MUGONGPREATTACK_ACK isBuffSkill: dwMugongID=%u, target=(%u,%u)\n"), dwMugongID, wTargetPosX, wTargetPosY);
			pAttackerCharObject->SetAngleTarget( wTargetPosX, wTargetPosY);
			
			// Play the correct casting animation for the buff skill
			if (dwMugongID == OUTGONGID_ILYUIDOGANG || dwMugongID == OUTGONGID_YUESUSINYUNG || 
				dwMugongID == OUTGONGID_JILPUNGBO || dwMugongID == OUTGONGID_CHOSANGBI)
			{
				sArrayData* pData = XiahArrayIndex::g_MugongTemplate.GetData(dwMugongID);
				if (pData != NULL)
				{
					int ani_index = pData->GetInt(2);
					g_MainCharInfo.m_nFastIndex = ani_index;
					if (!pAttackerCharObject->m_KeepUpMugongList.IsExist(dwMugongID))
					{
						pAttackerCharObject->SetAnimation(XiahAniType::eLAT_Mugong, XiahAniType::eLAT_Stand, 12, -1, 1.0f);
					}
				}
			}
			else
			{
				sArrayData* pData = XiahArrayIndex::g_MugongTemplate.GetData(dwMugongID);
				if (pData == NULL)
				{
					pData = XiahArrayIndex::g_RebirthMugong_List.GetData(dwMugongID);
				}
				if (pData != NULL)
				{
					int ani_index = (dwMugongID >= 161 && dwMugongID <= 179) ? pData->GetInt(3) : pData->GetInt(2);
					pAttackerCharObject->SetAnimation(XiahAniType::eLAT_Mugong, XiahAniType::eLAT_Stand, ani_index, -1, 1.0f);
				}
			}

			// Send MugongAttackReq to complete the client-server action cycle
			if (g_pMainChar == pAttacker)
			{
				SendCS_BT_MUGONGATTACK_REQ(dwMugongID, 
											bAttackType, 
											dwAttackID, 
											wAttackPosX, 
											wAttackPosY, 
											bAttackHeight, 
											bDefenseType, 
											dwDefenseID, 
											wTargetPosX, 
											wTargetPosY, 
											bTargetHeight);
			}
			return TRUE; // Return immediately to completely skip attack lock (m_bAttack = TRUE) sequence!
		}
	}

	switch( bAttackType)
	{
	case OBJTYPE_PC:
			{
				if(dwMugongID >= 161 && dwMugongID <= 179)
				{
					sArrayData* pData = XiahArrayIndex::g_RebirthMugong_List.GetData( dwMugongID);
				
					if( pData == NULL)
						return TRUE;

					int ani_index = pData->GetInt(3);

					pAttackerCharObject->SetAngleTarget( wTargetPosX, wTargetPosY);
					// 媳魔
					if(dwMugongID == 172)
					{
						//  鸥 3卤
						int nNum = rand() % 3;
						
						//  诘 眉   饪?洗..
						pAttackerCharObject->SetAnimation( XiahAniType::eLAT_MugongException, XiahAniType::eLAT_Stand, nNum, -1, 1.0f);
					}
					else                        
					{
						pAttackerCharObject->SetAnimation( XiahAniType::eLAT_Mugong, XiahAniType::eLAT_Stand, ani_index, -1, 1.0f);
					}
				}
				else
				{
					sArrayData* pData = XiahArrayIndex::g_MugongTemplate.GetData( dwMugongID);

					if( pData == NULL)
						return TRUE;

					int ani_index = pData->GetInt(2);

					DBG_LogFile(_T("[AngleDebug] OnCS_BT_MUGONGPREATTACK_ACK non-buff: dwMugongID=%u, target=(%u,%u)\n"), dwMugongID, wTargetPosX, wTargetPosY);
					pAttackerCharObject->SetAngleTarget( wTargetPosX, wTargetPosY);

					// 
					if( dwMugongID == OUTGONGID_ILYUIDOGANG	 || 
						dwMugongID == OUTGONGID_YUESUSINYUNG || 
						dwMugongID == OUTGONGID_JILPUNGBO	 ||
						dwMugongID == OUTGONGID_CHOSANGBI )
					{
						// 2004_06_22 Changth   执碳 甙蔷.
						if( !pAttackerCharObject->m_KeepUpMugongList.IsExist(dwMugongID) )
						{
							// 饧?12  12掳 赘   执碳. AniType = 315
							pAttackerCharObject->SetAnimation( XiahAniType::eLAT_Mugong, XiahAniType::eLAT_Stand, 12, -1, 1.0f);
							g_MainCharInfo.m_nFastIndex = ani_index;
						}
					}
					else	//  徒    志 却.
					if( dwMugongID == OUTGONGID_GYUISIKDAEBUB )
					{
						pAttackerCharObject->SetAnimation( XiahAniType::eLAT_Mugong, ani_index );
						pAttackerCharObject->m_CharRender.SetLoopAnimation(FALSE);
						pAttackerCharObject->m_bNowGyuisikdaebub = true;
					}
					else
					{
						pAttackerCharObject->SetAnimation( XiahAniType::eLAT_Mugong, XiahAniType::eLAT_Stand, ani_index, -1, 1.0f);
					}

					//  
					if( dwMugongID == OUTGONGID_GWANGMADOKGONG )
					{
						pAttackerCharObject->m_bNowGwangmadokgong = true;
					}

					//HT_0711 :   (虐)
					if( dwMugongID == REBRITH_GWANGMASINGONG )
					{
						pAttackerCharObject->m_bNowGwangmadokgong = true;
					}
				}
			}
			break;
	case OBJTYPE_NPC:// 
			pAttackerCharObject->SetAnimation( XiahAniType::eLAT_Mugong, XiahAniType::eLAT_Stand, 0, -1);
			break;
	}

	if( pDefender && pDefender != pAttacker)	// [ModernControl] 修复：当防御者是自己本身时（如寸草不生等无目标/地表技能），不执行无意义的“自己面向自己”转向逻辑，以维持正确的鼠标地表施法朝向
	{
		CXiahCharObject* pDefenderCharObject = (CXiahCharObject*) pDefender->m_pObject;

		if(pDefenderCharObject == NULL)
		{
			DBG_LogFile( _T("OnCS_BT_MUGONGPREATTACK_ACK "));
			return TRUE;
		}
		
		pAttackerCharObject->SetAngleTarget( pDefenderCharObject->m_Position);
	}

	pAttackerCharObject->m_bAttack = TRUE;

	if( g_pMainChar == pAttacker) //  没 
	{
		//HT_CHEAT
		// 鸥谈影    蚀麓
		//if( pAttackerCharObject->m_CharRender.GetTimerTriggerCount() > 0)
		//{
		//	g_MainChar_MugongPreAttackInfo.dwMugongID	=   dwMugongID; 
		//	g_MainChar_MugongPreAttackInfo.bAttackType	=	bAttackType; 
		//	g_MainChar_MugongPreAttackInfo.dwAttackID	=	dwAttackID; 
		//	g_MainChar_MugongPreAttackInfo.wAttackPosX	=	wAttackPosX; 
		//	g_MainChar_MugongPreAttackInfo.wAttackPosY	=	wAttackPosY; 
		//	g_MainChar_MugongPreAttackInfo.bAttackHeight= 	bAttackHeight;
		//	g_MainChar_MugongPreAttackInfo.bDefendType	=	bDefenseType;
		//	g_MainChar_MugongPreAttackInfo.dwDefendID	=	dwDefenseID; 
		//	g_MainChar_MugongPreAttackInfo.wTargetPosX	=	wTargetPosX; 
		//	g_MainChar_MugongPreAttackInfo.wTargetPosY	=	wTargetPosY; 
		//	g_MainChar_MugongPreAttackInfo.bTargetHeight=	bTargetHeight;

		//	pAttackerCharObject->m_bAttackType = 1;
		//}
		//else
		//{
		//	SendCS_BT_MUGONGATTACK_REQ(dwMugongID, 
		//								bAttackType, 
		//								dwAttackID, 
		//								wAttackPosX, 
		//								wAttackPosY, 
		//								bAttackHeight, 
		//								bDefenseType, 
		//								dwDefenseID, 
		//								wTargetPosX, 
		//								wTargetPosY, 
		//								bTargetHeight);
		//}
		 SendCS_BT_MUGONGATTACK_REQ(dwMugongID, 
										bAttackType, 
										dwAttackID, 
										wAttackPosX, 
										wAttackPosY, 
										bAttackHeight, 
										bDefenseType, 
										dwDefenseID, 
										wTargetPosX, 
										wTargetPosY, 
										bTargetHeight);


//		sArrayData* pData = XiahArrayIndex::g_SkillTiemIndex.GetData(dwMugongID, bMugongLevel);
//
//		if(pData)
//		{
//			g_SkillTime.AddSkill(dwMugongID, timeGetTime(), pData->GetInt(2));
//		}
	}

	return TRUE;
}

//---------------------------------------------------------------------------------------
int OnCS_BT_MUGONGATTACK_ACK( CMsg &msg)
{
	BYTE bResult		=0;	
	BYTE bMugongLevel	=0;
	BYTE bAtkType		=0;
	BYTE bAtkHeight		=0;
	BYTE bDefType		=0;
	BYTE bHitFlag		=0;
	DWORD dwMugongID	=0;
	DWORD dwAtkID		=0;
	DWORD dwDefID		=0;
	DWORD dwExp			=0;
	WORD wAtkPosX		=0;
	WORD wAtkPosY		=0;	
	DWORD dwDefHpMax		=0;
	DWORD dwDefHpCur		=0;
	DWORD	wDamage	=0; //HT_0907 :  平 ( 65000  绚; )

	msg
		>> bResult
		>> dwMugongID
		>> bMugongLevel
		>> bAtkType
		>> dwAtkID
		>> wAtkPosX
		>> wAtkPosY
		>> bAtkHeight
		>> bDefType
		>> dwDefID
		>> dwDefHpMax
		>> dwDefHpCur
		>> wDamage//HT_0907 : 莘 平 ( 65000 汛俅 绚; )
		>> dwExp
		>> bHitFlag; // bHitFlag 1谈 农萍 Hit

	if(wAtkPosX < 0 || wAtkPosX > 2047 || wAtkPosY < 0 || wAtkPosY > 2047) 
	{
		DBG_LogFile("Invalid Position in OnCS_BT_ATTACK_ACK");
		return TRUE;
	}

	XiahObject::CXiahObject* pAttacker = XiahObject::g_XiahObjectManager.FindXiahObject( MAKEOBJECTID( 0, dwAtkID,bAtkType));
	XiahObject::CXiahObject* pDefender = XiahObject::g_XiahObjectManager.FindXiahObject( MAKEOBJECTID( 0, dwDefID,bDefType));

	if( pAttacker == NULL)
	{
		ValidateObject( bAtkType, dwAtkID, wAtkPosX, wAtkPosY);
		return TRUE;
	}

	if( pDefender == NULL)
	{
		ValidateObject( bDefType, dwDefID, wAtkPosX, wAtkPosY);
		return TRUE;
	}	 

	// [6/18/2004]
	if( dwMugongID == OUTGONGID_UNKIHAENG || 
		dwMugongID == OUTGONGID_JOSIKSUL  || 
		dwMugongID == OUTGONGID_KIYOESUL )
		return TRUE;	//  

	CXiahCharObject* pDefenderCharObject = (CXiahCharObject*) pDefender->m_pObject;	
	CXiahCharObject* pAttackerCharObject = NULL;

	if(pDefenderCharObject == NULL)
	{
		DBG_LogFile( _T("OnCS_BT_MUGONGATTACK_ACK "));
		return TRUE;
	}

	// 2004.07.12 Changth
	//  贪  .
	if( pDefenderCharObject->m_nCurMotionType == XiahAniType::eLAT_Die ||
        pDefenderCharObject->m_nCurMotionType == XiahAniType::eLAT_Died  )
	{
		return TRUE;
	}

	if( pAttacker)
	{
		pAttackerCharObject = (CXiahCharObject*) pAttacker->m_pObject;

		if(pAttackerCharObject == NULL)
		{
			DBG_LogFile( _T("OnCS_BT_MUGONGATTACK_ACK "));
			return TRUE;
		}

		// QUEST OBJ 雀 蚀麓.
		if( pDefenderCharObject->m_bTargetMove == FALSE && pDefenderCharObject->m_bRotatable == TRUE)
			pDefenderCharObject->SetAngleTarget( pAttackerCharObject->m_Position);
	}

	// MUGONGSTART_ACK 贸 洗 炔 饪?贸. 飘
	switch( dwMugongID )
	{
	case OUTGONGID_JUNYUUM:	// , 2,3 飘 .
		{
			if( pDefenderCharObject )
			{
				CXiahCharObject* pMainChar = (CXiahCharObject*)g_pMainChar->m_pObject;

				if( pDefenderCharObject == pMainChar )
					return TRUE;

				_EFFECTPACKAGE* pEffectPackage = g_EffectManager.EnqOutGongPersistEffectImmediately( eJunuoum_recv );

				if( pEffectPackage && pEffectPackage->pEffectRender && pEffectPackage->pEffectRender->pPackagePair )
				{
					pEffectPackage->pEffectRender->pPackagePair->pWorldMatrix = (MATRIX*)pDefenderCharObject->m_CharRender.GetCharTM();

					pDefenderCharObject->m_EffectPPList.push_back( pEffectPackage->pEffectRender->pPackagePair );
				}
			}// if

			return TRUE;	// 袒 士  .
		}		
	case OUTGONGID_BANTANKANGKI:	// 藕, 藕  鸥 飘
		if( pDefenderCharObject && pAttackerCharObject )
		{
			// 鸥 飘 摹 
			int nPosX, nPosY, nPosZ, nStartTime, nEffectType;

			GetHitEffectType( pDefenderCharObject->m_bObjType, pDefenderCharObject->m_bSubObjType, 
							  nPosX, nPosY, nPosZ, nStartTime,
							  pAttackerCharObject->m_bObjType, pAttackerCharObject->m_bSubObjType,
							  nEffectType );

			_EFFECTPACKAGE* pEffectPackage = g_EffectManager.EnqHitEffectImmediately( eBantankangki_Hit, nStartTime, nPosX, nPosY, nPosZ );
			// matrix
			if( pEffectPackage && pEffectPackage->pEffectRender && pEffectPackage->pEffectRender->pPackagePair )
			{
				pEffectPackage->pEffectRender->pPackagePair->WorldMatrix = (MATRIX)pDefenderCharObject->m_ObjectTM;
//				pEffectPackage->pEffectRender->pPackagePair->pWorldMatrix = (MATRIX*)pDefenderCharObject->m_CharRender.GetCharTM();

//				pDefenderCharObject->m_EffectPPList.push_back( pEffectPackage->pEffectRender->pPackagePair );
			}// if
		}// if
		break;

	case GUM_ILKICHAM:	// 媳
		{
			if( pDefenderCharObject && pAttackerCharObject )
			{
				// 鸥 飘 摹 
				int nPosX, nPosY, nPosZ, nStartTime, nEffectType;

				GetHitEffectType( pDefenderCharObject->m_bObjType, pDefenderCharObject->m_bSubObjType, 
						nPosX, nPosY, nPosZ, nStartTime,
						pAttackerCharObject->m_bObjType, pAttackerCharObject->m_bSubObjType, nEffectType );

				_EFFECTPACKAGE* pEffectPackage = g_EffectManager.EnqHitEffectImmediately(eGumyongSpecial, nStartTime, nPosX, nPosY, nPosZ );
				// matrix
				if( pEffectPackage && pEffectPackage->pEffectRender && pEffectPackage->pEffectRender->pPackagePair )
				{
					pEffectPackage->pEffectRender->pPackagePair->WorldMatrix = (MATRIX)pDefenderCharObject->m_ObjectTM;
				}// if
			}
		}
		break;
	case MU_KANGKIPOKWON:	// 
		{
			if( pDefenderCharObject && pAttackerCharObject )
			{
				// 鸥 飘 摹 
				int nPosX, nPosY, nPosZ, nStartTime, nEffectType;

				GetHitEffectType( pDefenderCharObject->m_bObjType, pDefenderCharObject->m_bSubObjType, 
						nPosX, nPosY, nPosZ, nStartTime,
						pAttackerCharObject->m_bObjType, pAttackerCharObject->m_bSubObjType, nEffectType );

				_EFFECTPACKAGE* pEffectPackage = g_EffectManager.EnqHitEffectImmediately(eMutuSpecial, nStartTime, nPosX, nPosY, nPosZ );
				// matrix
				if( pEffectPackage && pEffectPackage->pEffectRender && pEffectPackage->pEffectRender->pPackagePair )
				{
					pEffectPackage->pEffectRender->pPackagePair->WorldMatrix = (MATRIX)pDefenderCharObject->m_ObjectTM;
				}// if
			}
		}
		break;
	}; // switch


	//   飘 摹 .
/*
	Vector3 scPos;
	sRect rcRect;
	D3DCOLOR TextEffectColor;
	if( g_pCurrentCamera )
	{
		scPos = g_pCurrentCamera->WorldToScreen( pDefenderCharObject->m_Position + Vector3( 0, pDefenderCharObject->m_LocalBound.m_vMax.y, 0));
		rcRect.left = scPos.x;
		rcRect.top = scPos.y;
	}
	else
	{
		rcRect.left = pDefenderCharObject->m_rcObjectScreenPos.left;
		rcRect.top = pDefenderCharObject->m_rcObjectScreenPos.top;
	}
*/

	//
	CXiahCharObject* pMainChar = (CXiahCharObject*)g_pMainChar->m_pObject;

	//  OBJ 伟谈 SOUND FX 峡  Type 执.
	if(pAttackerCharObject == pMainChar)
	{
		pAttackerCharObject->m_dwEnemyType = g_FXType[pDefenderCharObject->m_bSubObjType];
	}

	if(bHitFlag && pAttackerCharObject == pMainChar)
	{
		switch(pAttackerCharObject->m_bSubObjType)
		{
		// 丝
		case 1:
			g_MainCharInfo.PlayInterfaceSound(PLAYER1_CRITICAL_FX);
			break;
		// 
		case 2:
			g_MainCharInfo.PlayInterfaceSound(PLAYER2_CRITICAL_FX);
			break;
		// 
		case 3:
			g_MainCharInfo.PlayInterfaceSound(PLAYER3_CRITICAL_FX);
			break;
		// 
		case 4:
			g_MainCharInfo.PlayInterfaceSound(PLAYER4_CRITICAL_FX);
			break;
		}
	}

	// 农萍 hit  
	if(bHitFlag && g_cj && pAttackerCharObject == pMainChar)
	{
		HRESULT hr;
		hr = g_cj->Change_Para(1500,BIG_STRONG);
		hr = g_cj->Rumble_Start();
	}

	// NORMAL PC DAMAGE (   )
	if(pDefenderCharObject == pMainChar)
	{
		HRESULT hr;
		// 
		if(wDamage && g_cj)//HT_0907 : 莘 平 ( 65000 汛俅 绚; )
		{
			hr = g_cj->Change_Para(1000,SMALL_STRONG);
			hr = g_cj->Rumble_Start();
		}

		// DAMAGE SOUND
		switch(pDefenderCharObject->m_bSubObjType)
		{
			// 丝
		case 1 :
			{
				if(bHitFlag)	// 农萍
					g_MainCharInfo.PlayInterfaceSound(PLAYER1_DAMAGE1_FX2);
				else
					g_MainCharInfo.PlayInterfaceSound(PLAYER1_DAMAGE1_FX1);
			}
			break;
			// 
		case 2 :
			{
				if(bHitFlag)	// 农萍
					g_MainCharInfo.PlayInterfaceSound(PLAYER1_DAMAGE2_FX2);
				else
					g_MainCharInfo.PlayInterfaceSound(PLAYER1_DAMAGE2_FX1);
			}
			break;
			// 
		case 3 :
			{
				if(bHitFlag)	// 农萍
					g_MainCharInfo.PlayInterfaceSound(PLAYER1_DAMAGE3_FX2);
				else
					g_MainCharInfo.PlayInterfaceSound(PLAYER1_DAMAGE3_FX1);
			}
			break;
		case 4:
			{
				if(bHitFlag)	// 农萍
					g_MainCharInfo.PlayInterfaceSound(PLAYER1_DAMAGE4_FX1);
				else
					g_MainCharInfo.PlayInterfaceSound(PLAYER1_DAMAGE4_FX2);
			}
			break;
		}
	}

	//
	Vector3 vHitEffectPos = pDefenderCharObject->m_Position + Vector3( 0, pDefenderCharObject->m_LocalBound.m_vMax.y, 0);
	Vector3 vHitEffectPos2 = pDefenderCharObject->m_Position + Vector3( 0, pDefenderCharObject->m_LocalBound.m_vMax.y - 3.0f, 0);

	int nHitEffectType1;
	if( pDefenderCharObject->m_bObjType == OBJTYPE_PC )
	{
		nHitEffectType1 = 0;
	}
	else
	{
		nHitEffectType1 = 1;
	}

	BOOL bSetHitAnimation = TRUE;

	if( pDefenderCharObject->m_bAttack || pDefenderCharObject->m_bTargetMove)
		bSetHitAnimation =	FALSE;

	int nHitEffectType2;

	//HT_CHEAT :  
	XiahItem::sItemInfo* pItem;

	switch( bResult)
	{
	case 1: //     (MISS)
		{
			if( bSetHitAnimation)
				pDefenderCharObject->SetAnimation( XiahAniType::eLAT_Defend, XiahAniType::eLAT_Stand, 0, 0);

			pDefenderCharObject->m_bMoveable = FALSE;

			if( nHitEffectType1 == 0 ) // Red
				nHitEffectType2 = 2;	// miss
			else
				nHitEffectType2 = 3;

			g_HitEffect.AddHitEffect( nHitEffectType2, 0, vHitEffectPos );
		}
		break;

	case 2:	
		{
			if( wDamage == 0)//HT_0907 :  平 ( 65000  绚; )
			{
				if( bSetHitAnimation)
					pDefenderCharObject->SetAnimation( XiahAniType::eLAT_Defend, XiahAniType::eLAT_Stand, 0, 0);

				pDefenderCharObject->m_bMoveable = FALSE;

				//
				if( nHitEffectType1 == 0 ) // Red
					nHitEffectType2 = 2;
				else
					nHitEffectType2 = 3;

				g_HitEffect.AddHitEffect( nHitEffectType2, 0, vHitEffectPos );
			}
			else //   
			{
				if( bSetHitAnimation)
					pDefenderCharObject->SetAnimation( XiahAniType::eLAT_Hit, XiahAniType::eLAT_Stand, 0, 0);

				pDefenderCharObject->m_bMoveable = FALSE;
				pDefenderCharObject->m_dwCurHP = dwDefHpCur;
				pDefenderCharObject->m_dwMaxHP = dwDefHpMax;

				if( pDefenderCharObject->m_bObjType == OBJTYPE_PET && g_PetList.Find( pDefender->m_dwServerID) != NULL)
				{
					sPetInfo* pPetInfo = (sPetInfo*)pDefenderCharObject->m_pPrivateData;

					//YS_0811 : BUGFIX
					if ( pPetInfo == NULL )
					{					
						return 0;
					}

					pPetInfo->dwHpCur = dwDefHpCur;
					pPetInfo->dwHpMax = dwDefHpMax;
				}

				//
				if( nHitEffectType1 == 0 ) // Red
					nHitEffectType2 = 0;
				else
					nHitEffectType2 = 1;

				g_HitEffect.AddHitEffect( nHitEffectType2, wDamage, vHitEffectPos );//HT_0907 : 莘 平 ( 65000 汛俅 绚; )

				// Critical hit
				if( bHitFlag )//HT_0907 :  平 ( 65000  绚; )
					g_HitEffect.AddHitEffect( 4, wDamage, vHitEffectPos2 );

				// 鸥, NPC 飘
				if( dwMugongID == 24)
				{
					_EFFECTPACKAGE* pEffectPackage = g_EffectManager.EnqOutGongPersistEffectImmediately( eLeetasaeng_damage );

					if( pEffectPackage && pEffectPackage->pEffectRender && pEffectPackage->pEffectRender->pPackagePair )
					{
						if( pDefenderCharObject )
							pEffectPackage->pEffectRender->pPackagePair->WorldMatrix = (MATRIX)pDefenderCharObject->m_ObjectTM;
					}// if
				}// if
			}
		}
		break;
	case 3:
		g_MainCharInfo.ShowHelpMessage( IDS_SHORT_INLIFE, TEXTEFFECT_COLOR_WARNING);
		//HT_CHEAT :  
		pItem = g_MainCharInfo.m_pMySack[0]->FindSackItemByVisualID(21100 );
		if( pItem)
		{
			SendCS_IM_USEITEM_REQ( pItem->m_bSackCount+1, pItem->m_bSackPos, pItem->m_dwItemID);
		}
		else
		{
			pItem = g_MainCharInfo.m_pMySack[1]->FindSackItemByVisualID( 21100);
			if( pItem)
				SendCS_IM_USEITEM_REQ( pItem->m_bSackCount+1, pItem->m_bSackPos, pItem->m_dwItemID);
		}
		break;
	case 4:
		g_MainCharInfo.ShowHelpMessage( IDS_NO_MUGONG, TEXTEFFECT_COLOR_WARNING);
		break;
	case 5:
		g_MainCharInfo.ShowHelpMessage( IDS_SHORT_LIFE, TEXTEFFECT_COLOR_WARNING);
		break;
	}
	// Client-side debuff icon: when bType=4 skill hits NPC, add icon
	if (bResult == 2 && bDefType == OBJTYPE_NPC && pDefenderCharObject)
	{
		sArrayData* pDbgTemplate = XiahArrayIndex::g_MugongTemplate.GetData(dwMugongID);
		if (pDbgTemplate && pDbgTemplate->GetInt(3) == 4)
		{
			DWORD dwDurMs = 10000;
			sArrayData* pDbgMList = XiahArrayIndex::g_MugongList.GetData(dwMugongID, bMugongLevel);
			if (pDbgMList && pDbgMList->GetInt(31) > 0)
				dwDurMs = pDbgMList->GetInt(31) * 1000;
			pDefenderCharObject->AddDebuff((WORD)dwMugongID, dwDurMs);
		}
	}

	return TRUE;
}

//---------------------------------------------------------------------------------------
int OnCS_BT_KEEPUPMUGONGSTART_ACK( CMsg &msg)
{
	BYTE	bResult		=0;	
	BYTE	bObjectType	=0;
	BYTE	bMugongLevel=0;
	DWORD	dwObjectID	=0;
	DWORD	dwMugongID	=0;	
		
	msg
		>> bResult
		>> dwObjectID
		>> bObjectType
		>> dwMugongID
		>> bMugongLevel;

	// 屏蔽五行技能 (150~154) 的 Buff 图标挂载和本地 Buff 列表记录
	if (dwMugongID >= 150 && dwMugongID <= 154)
	{
		return TRUE;
	}

	XiahObject::CXiahObject* pObject = XiahObject::g_XiahObjectManager.FindXiahObject( MAKEOBJECTID( 0, dwObjectID,bObjectType));

	if( pObject == NULL)
	{
		DBG_Put("OnCS_BT_KEEPUPMUGONGSTART_ACK 飘 茫 %d", dwObjectID);
		return TRUE;
	}

	//HT_0403 :    
	if(g_pMainChar && g_pMainChar->m_dwServerID == dwObjectID)
	{ 
		bool bFound = false;
		for(auto iter = g_MainCharInfo.m_vkeepUpMugongIconList.begin(); iter != g_MainCharInfo.m_vkeepUpMugongIconList.end(); ++iter)
		{
			if((*iter)->m_MugongID == dwMugongID)
			{
				(*iter)->m_CurTime = g_dwCurTime;
				(*iter)->m_DrawIcon = true;
				(*iter)->m_MugongLevel = bMugongLevel;
				bFound = true;
				break;
			}
		}
		
		if(!bFound)
		{
			sKEEPUPMUGONGICONLIST *TempList = new sKEEPUPMUGONGICONLIST;
			TempList->m_MugongID = dwMugongID;
			TempList->m_CurTime =  g_dwCurTime;
			TempList->m_DrawIcon = true;
			TempList->m_MugongLevel = bMugongLevel;
			g_MainCharInfo.m_vkeepUpMugongIconList.push_back(TempList);
		}
	}
	else if( g_PetList.Find( dwObjectID) != NULL)
	{
		bool bFound = false;
		for(auto iter = g_MainCharInfo.m_vkeepUpPetMugongIconList.begin(); iter != g_MainCharInfo.m_vkeepUpPetMugongIconList.end(); ++iter)
		{
			if((*iter)->m_MugongID == dwMugongID)
			{
				(*iter)->m_CurTime = g_dwCurTime;
				(*iter)->m_DrawIcon = true;
				(*iter)->m_MugongLevel = bMugongLevel;
				bFound = true;
				break;
			}
		}
		
		if(!bFound)
		{
			sKEEPUPMUGONGICONLIST *TempList = new sKEEPUPMUGONGICONLIST;
			TempList->m_MugongID = dwMugongID;
			TempList->m_CurTime =  g_dwCurTime;
			TempList->m_DrawIcon = true;
			TempList->m_MugongLevel = bMugongLevel;
			g_MainCharInfo.m_vkeepUpPetMugongIconList.push_back(TempList);
		}
	}

	CXiahCharObject* pCharObject = reinterpret_cast<CXiahCharObject*>(pObject->m_pObject);

	if(pCharObject == NULL)
	{
		DBG_LogFile( _T("OnCS_BT_KEEPUPMUGONGSTART_ACK "));
		return TRUE;
	}

	DWORD dwTime =0;

	//HT_0601   

	//   毯 9掳 冒  
	sArrayData* pMugongList = NULL;
	if((dwMugongID >= WHA_DRAGONSINJANG) && (dwMugongID <= YA_HOJUNGKANGKI))
		pMugongList = XiahArrayIndex::g_RebirthMugong_List.GetData( dwMugongID, bMugongLevel);
	else
	{
		pMugongList = XiahArrayIndex::g_MugongList.GetData( dwMugongID, bMugongLevel);

		if( pMugongList)
			dwTime = pMugongList->GetInt(9);
	}	

	if(dwMugongID == YA_EUNSINSUL && pObject != g_pMainChar)
		pCharObject->m_bRenderOK = false;
	else	
		pCharObject->m_KeepUpMugongList.Add(dwMugongID, bMugongLevel, dwTime); 

//	CKeepupMugongList::iterator iter = pCharObject->m_KeepUpMugongList.find(dwMugongID);
//	if(iter != pCharObject->m_KeepUpMugongList.end())
//	{
//		sKeepUpMugong& keep = iter->second;
//		keep.dwTime2 = timeGetTime();
//	}

	// 婀?
	if( dwMugongID == OUTGONGID_AMHUKMU )
	{
		CXiahCharObject* pMainChar = (CXiahCharObject*)g_pMainChar->m_pObject;
		if( pCharObject == pMainChar )
			g_XiahEnvInfo.m_bAmhukmuFog = TRUE;

		// 婀? 赘  群未. 拳  赘诟 .
		XiahMap::g_XiahMap.ClearAllDecal();
	}

	// 虐隙. 虐 薇 飘
	if( dwMugongID == OUTGONGID_W0NKISINKANG )
	{
		_EFFECTPACKAGE* pEffectPackage = g_EffectManager.EnqOutGongPersistEffectImmediately( eWonkisingang_recv );

		if( pEffectPackage && pEffectPackage->pEffectRender && pEffectPackage->pEffectRender->pPackagePair )
		{
			pEffectPackage->pEffectRender->pPackagePair->pWorldMatrix = (MATRIX*)pCharObject->m_CharRender.GetCharTM();
			//			pEffectPackage->pEffectRender->pPackagePair->WorldMatrix = (MATRIX)pCharObject->m_ObjectTM;

			// 鼐 某桶  飘  .
			pCharObject->m_EffectPPList.push_back( pEffectPackage->pEffectRender->pPackagePair );
		}
	}

	//HT_0711 :  
	if( dwMugongID == REBRITH_W0NKISINGONG )
	{
		_EFFECTPACKAGE* pEffectPackage = g_EffectManager.EnqOutGongPersistEffectImmediately( eWonkisingang_recv );

		if( pEffectPackage && pEffectPackage->pEffectRender && pEffectPackage->pEffectRender->pPackagePair )
		{
			pEffectPackage->pEffectRender->pPackagePair->pWorldMatrix = (MATRIX*)pCharObject->m_CharRender.GetCharTM();
			//			pEffectPackage->pEffectRender->pPackagePair->WorldMatrix = (MATRIX)pCharObject->m_ObjectTM;

			// 鼐 某桶  飘  .
			pCharObject->m_EffectPPList.push_back( pEffectPackage->pEffectRender->pPackagePair );
		}
	}

	if(dwMugongID == YUN_SUSINKIKANG && OBJTYPE_PET == bObjectType)
	{
		pCharObject->m_CharRender.SetLocalScale(Vector3(2.0f, 2.0f, 2.0f));

		sPetInfo* pPetInfo = (sPetInfo*)pCharObject->m_pPrivateData;

		if(pPetInfo)
		{
			XiahObject::CXiahObject* pMainObject = XiahObject::g_XiahObjectManager.FindXiahObject(MAKEOBJECTID(0, pPetInfo->dwOwnID, OBJTYPE_PC));

			if(pMainObject == NULL)
				return FALSE;

			CXiahCharObject* pMainCharObject = reinterpret_cast<CXiahCharObject*>(pMainObject->m_pObject);

			if(pMainCharObject == NULL)
				return FALSE;

			pMainCharObject->m_KeepUpMugongList.Add(dwMugongID, bMugongLevel, dwTime);
		}	
	}
	
	switch(dwMugongID)
	{
	case OUTGONGID_ILYUIDOGANG:
	case OUTGONGID_YUESUSINYUNG:
	case OUTGONGID_JILPUNGBO:
	case OUTGONGID_CHOSANGBI:
		{
			// 
			if(g_pMainChar->m_dwServerID == dwObjectID)
			{
				g_MainCharInfo.m_bFastMove = true;
			}
			else
			{
				// 2004_06_22 Changth   执碳 甙蔷.
				//  某桶   执细碳 .
				// 饧?12  12掳 赘   执细碳檀. AniType = 315
				pCharObject->SetAnimation( XiahAniType::eLAT_Mugong, XiahAniType::eLAT_Stand, 12, -1, 1.0f);
			}
		}
		break;
	}

	return TRUE;
}

int OnCS_BT_KEEPUPMUGONGEND_ACK( CMsg &msg)
{
	BYTE bResult		=0;	
	BYTE bObjectType	=0;
	BYTE bMugongLevel	=0;
	DWORD dwObjectID	=0;
	DWORD dwMugongID	=0;
	

	msg
		>> bResult
		>> dwObjectID
		>> bObjectType
		>> dwMugongID
		>> bMugongLevel;

	// 屏蔽五行技能 (150~154) 的 Buff 清除包处理
	if (dwMugongID >= 150 && dwMugongID <= 154)
	{
		return TRUE;
	}

	XiahObject::CXiahObject* pObject = XiahObject::g_XiahObjectManager.FindXiahObject( MAKEOBJECTID( 0, dwObjectID,bObjectType));

	if( pObject == NULL)
	{
		DBG_Put(_T("呀!!"));
		return TRUE;
	}

	//HT_0403 :    
	if(g_pMainChar && g_pMainChar->m_dwServerID == dwObjectID)
	{
		std::vector<sKEEPUPMUGONGICONLIST*>::iterator iter;

		for(iter = g_MainCharInfo.m_vkeepUpMugongIconList.begin(); iter != g_MainCharInfo.m_vkeepUpMugongIconList.end(); ++iter)
		{
			sKEEPUPMUGONGICONLIST *psKeepUpMugong = (*iter);
			if(psKeepUpMugong->m_MugongID == dwMugongID)
			{
				g_MainCharInfo.m_vkeepUpMugongIconList.erase(iter);
				break;
			}
		}	
		for(iter = g_MainCharInfo.m_vkeepUpPetMugongIconList.begin(); iter != g_MainCharInfo.m_vkeepUpPetMugongIconList.end(); ++iter)
		{
			sKEEPUPMUGONGICONLIST *psKeepUpMugong = (*iter);
			if(psKeepUpMugong->m_MugongID == dwMugongID)
			{
				g_MainCharInfo.m_vkeepUpPetMugongIconList.erase(iter);
				break;
			}
		}	
	}
	else if( g_PetList.Find( dwObjectID) != NULL)
	{
		std::vector<sKEEPUPMUGONGICONLIST*>::iterator iter = g_MainCharInfo.m_vkeepUpPetMugongIconList.begin();

		for(; iter != g_MainCharInfo.m_vkeepUpPetMugongIconList.end(); ++iter)
		{
			sKEEPUPMUGONGICONLIST *psKeepUpMugong = (*iter);
			if(psKeepUpMugong->m_MugongID == dwMugongID)
			{
				g_MainCharInfo.m_vkeepUpPetMugongIconList.erase(iter);
				break;
			}
		}	
	}

	CXiahCharObject* pCharObject = reinterpret_cast<CXiahCharObject*>(pObject->m_pObject);

	if(pCharObject == NULL)
	{
		DBG_LogFile( _T("OnCS_BT_KEEPUPMUGONGEND_ACK "));
		return TRUE;
	}

	// 飘 俜   确  .
	bool bExistGyuisikdaebub = false;
	if( pCharObject->m_KeepUpMugongList.IsExist(OUTGONGID_GYUISIKDAEBUB) )
		bExistGyuisikdaebub = true;

//	CKeepupMugongList::iterator iter = pCharObject->m_KeepUpMugongList.find(dwMugongID);
//	if(iter != pCharObject->m_KeepUpMugongList.end())
//	{
//		sKeepUpMugong& keep = iter->second;
//		DWORD dwTime = timeGetTime();
//		DWORD dwAAAAATime = dwTime - keep.dwTime2;
//
//		char strTemp[128] = {0,};
//		sprintf(strTemp, "%d ID %d level %d ms", dwMugongID, bMugongLevel, dwAAAAATime);
//
//		g_pUIManager->ShowNotice(strTemp);
//	}

	pCharObject->m_KeepUpMugongList.Delete( dwMugongID);

	// 潭 飘 . 约    饧?.
	switch( dwMugongID )
	{
	case OUTGONGID_MUSUHON:		// 去.
		if( pCharObject->m_pMusuhonEffectPP )
		{
			g_EffectManager.DeqEffectPackagePair( pCharObject->m_pMusuhonEffectPP );
			pCharObject->m_pMusuhonEffectPP = NULL;
		}
		break;
	case OUTGONGID_POKSAHON:	// 去.
		if( pCharObject->m_pPoksahonEffectPP )
		{
			g_EffectManager.DeqEffectPackagePair( pCharObject->m_pPoksahonEffectPP );
			pCharObject->m_pPoksahonEffectPP = NULL;
			//HT_CHEAT : 去 
			if(g_bCheat && pObject == g_pMainChar)
			{
				SendCS_BT_MUGONGPREATTACK_REQ(OUTGONGID_POKSAHON,1,g_pMainChar->m_dwServerID,
											0,0,0,0,0,0,0,0);
			}
		}
		break;
	case OUTGONGID_KUMKANGLUK:	// .
		if( pCharObject->m_pKuymgangrukEffectPP )
		{
			g_EffectManager.DeqEffectPackagePair( pCharObject->m_pKuymgangrukEffectPP );
			pCharObject->m_pKuymgangrukEffectPP = NULL;
		}
		break;
	case REBRITH_KUMKANGSINGONG:	//HT_0711 :  ( 虐 )
		if( pCharObject->m_pKuymgangrukEffectPP )
		{
			g_EffectManager.DeqEffectPackagePair( pCharObject->m_pKuymgangrukEffectPP );
			pCharObject->m_pKuymgangrukEffectPP = NULL;
		}
		break;
	case OUTGONGID_YUNOYUNG:	// 炜?
		if( pCharObject->m_pYuenoyuengEffectPP )
		{
			g_EffectManager.DeqEffectPackagePair( pCharObject->m_pYuenoyuengEffectPP );
			pCharObject->m_pYuenoyuengEffectPP = NULL;
			//HT_CHEAT : 炜?
			sPetInfo* pPetInfo = g_PetList.GetCurrentPet();
			if(pPetInfo)
			{
				XiahObject::CXiahObject* pMyObject= g_PetList.Find( pPetInfo->dwID );
				if(g_bCheat && pObject == pMyObject)
				{
					WORD wPosX, wPosY;
					pCharObject->GetPosition( wPosX, wPosY);

					SendCS_BT_MUGONGPREATTACK_REQ(OUTGONGID_YUNOYUNG,1,g_pMainChar->m_dwServerID,
												1,1,1, OBJTYPE_PET, pPetInfo->dwID, wPosX, wPosY, 1);
				}
			}
		}
		break;
	case OUTGONGID_W0NKISINKANG:	// 虐.
		if( pCharObject->m_pWonkisingangEffectPP )
		{
			g_EffectManager.DeqEffectPackagePair( pCharObject->m_pWonkisingangEffectPP );
			pCharObject->m_pWonkisingangEffectPP = NULL;
			//HT_CHEAT :  虐 
			if(g_bCheat && pObject == g_pMainChar)
			{
				SendCS_BT_MUGONGPREATTACK_REQ(OUTGONGID_W0NKISINKANG,1,g_pMainChar->m_dwServerID,
											0,0,0,0,0,0,0,0);
			}
		}
		break;
	case REBRITH_W0NKISINGONG:	//HT_0711 :  ( 虐 )
		if( pCharObject->m_pWonkisingongEffectPP )
		{
			g_EffectManager.DeqEffectPackagePair( pCharObject->m_pWonkisingongEffectPP );
			pCharObject->m_pWonkisingongEffectPP = NULL;
		}
		break;
	case OUTGONGID_KYOKANSU:	// .
		if( pCharObject->m_pKyugamsuEffectPP )
		{
			g_EffectManager.DeqEffectPackagePair( pCharObject->m_pKyugamsuEffectPP );
			pCharObject->m_pKyugamsuEffectPP = NULL;
			//HT_CHEAT :  
			sPetInfo* pPetInfo = g_PetList.GetCurrentPet();
			if(pPetInfo)
			{
				XiahObject::CXiahObject* pMyObject= g_PetList.Find( pPetInfo->dwID );
				if(g_bCheat && pObject == pMyObject)
				{
					WORD wPosX, wPosY;
					pCharObject->GetPosition( wPosX, wPosY);

					SendCS_BT_MUGONGPREATTACK_REQ(OUTGONGID_KYOKANSU,1,g_pMainChar->m_dwServerID,
												1,1,1, OBJTYPE_PET, pPetInfo->dwID, wPosX, wPosY, 1);
				}
			}
		}
		break;
	case REBRITH_KYOKANSINGONG:	//HT_0711 :  ( 虐 )
		if( pCharObject->m_pKyugamsuEffectPP )
		{
			g_EffectManager.DeqEffectPackagePair( pCharObject->m_pKyugamsuEffectPP );
			pCharObject->m_pKyugamsuEffectPP = NULL;
		}
		break;
	case OUTGONGID_PACHUNSO:	// 玫.
		if( pCharObject->m_pPachunsoEffectPP )
		{
			g_EffectManager.DeqEffectPackagePair( pCharObject->m_pPachunsoEffectPP );
			pCharObject->m_pPachunsoEffectPP = NULL;
		}
		break;
	case OUTGONGID_MARYULKAK:	// 砂.
		if( pCharObject->m_pMarulkakEffectPP )
		{
			g_EffectManager.DeqEffectPackagePair( pCharObject->m_pMarulkakEffectPP );
			pCharObject->m_pMarulkakEffectPP = NULL;
		}
		break;
	case REBRITH_MARYUNGSINGONG:	//HT_0711 :  ( 山虐 )
		if( pCharObject->m_pMarulkakEffectPP )
		{
			g_EffectManager.DeqEffectPackagePair( pCharObject->m_pMarulkakEffectPP );
			pCharObject->m_pMarulkakEffectPP = NULL;
		}
		break;
	case OUTGONGID_AMHUKMU:	// 婀?
		{
			if( pCharObject->m_pAmhukmuEffectPP )
			{
				g_EffectManager.DeqEffectPackagePair( pCharObject->m_pAmhukmuEffectPP );
				pCharObject->m_pAmhukmuEffectPP = NULL;
			}

			CXiahCharObject* pMainChar = (CXiahCharObject*)g_pMainChar->m_pObject;
			if( pCharObject == pMainChar )
				g_XiahEnvInfo.m_bAmhukmuFog = FALSE;
		}
		break;
	case OUTGONGID_TALBAKIN:	// 呕.
		if( pCharObject->m_pTalbacinEffectPP )
		{
			g_EffectManager.DeqEffectPackagePair( pCharObject->m_pTalbacinEffectPP );
			pCharObject->m_pTalbacinEffectPP = NULL;
		}
		break;
	case OUTGONGID_JUKUNKANGKI:	// 畎?
		if( pCharObject->m_pJukwonkangkiEffectPP )
		{
			g_EffectManager.DeqEffectPackagePair( pCharObject->m_pJukwonkangkiEffectPP );
			pCharObject->m_pJukwonkangkiEffectPP = NULL;
		}
		break;
	case OUTGONGID_KUMNASU:	// .
		if( pCharObject->m_pKumnasuEffectPP )
		{
			g_EffectManager.DeqEffectPackagePair( pCharObject->m_pKumnasuEffectPP );
			pCharObject->m_pKumnasuEffectPP = NULL;
		}
		break;
	case REBRITH_KUMNASINGONG:	//HT_0711 :  ( 虐 )
		if( pCharObject->m_pKumnasuEffectPP )
		{
			g_EffectManager.DeqEffectPackagePair( pCharObject->m_pKumnasuEffectPP );
			pCharObject->m_pKumnasuEffectPP = NULL;
		}
		break;
	case OUTGONGID_BANTANKANGKI:	// 藕.
		if( pCharObject->m_pBantankangkiEffectPP )
		{
			g_EffectManager.DeqEffectPackagePair( pCharObject->m_pBantankangkiEffectPP );
			pCharObject->m_pBantankangkiEffectPP = NULL;
		}
		break;
	case OUTGONGID_ILYUIDOGANG:
	case OUTGONGID_YUESUSINYUNG:
	case OUTGONGID_JILPUNGBO:
		if( pCharObject->m_pGyungGongEffectPP )
		{
			g_EffectManager.DeqEffectPackagePair( pCharObject->m_pGyungGongEffectPP );
			pCharObject->m_pGyungGongEffectPP = NULL;
		}
		break;
	case OUTGONGID_SSANGDOSU:
		if( pCharObject->m_pSsangdosuEffectPP )
		{
			g_EffectManager.DeqEffectPackagePair( pCharObject->m_pSsangdosuEffectPP );
			pCharObject->m_pSsangdosuEffectPP = NULL;
		}
		break;
	case OUTGONGID_ODOKCHIM:
		if( pCharObject->m_pOdokchimEffectPP )
		{
			g_EffectManager.DeqEffectPackagePair( pCharObject->m_pOdokchimEffectPP );
			pCharObject->m_pOdokchimEffectPP = NULL;
		}
		break;
	case OUTGONGID_DOKHYULGONG:
		if( pCharObject->m_pDokhyulgongEffectPP )
		{
			g_EffectManager.DeqEffectPackagePair( pCharObject->m_pDokhyulgongEffectPP );
			pCharObject->m_pDokhyulgongEffectPP = NULL;
		}
		break;
	case REBRITH_DOKHYULSINGONG:			//HT_0711 :  ( 虐 )
		if( pCharObject->m_pDokhyulgongEffectPP )
		{
			g_EffectManager.DeqEffectPackagePair( pCharObject->m_pDokhyulgongEffectPP );
			pCharObject->m_pDokhyulgongEffectPP = NULL;
		}
		break;
	case OUTGONGID_DOKNAEGONG:
		if( pCharObject->m_pDoknaegongEffectPP )
		{
			g_EffectManager.DeqEffectPackagePair( pCharObject->m_pDoknaegongEffectPP );
			pCharObject->m_pDoknaegongEffectPP = NULL;
		}
		break;
	case OUTGONGID_MANDOKBULJIN:
		if( pCharObject->m_pMandokbuljinEffectPP )
		{
			g_EffectManager.DeqEffectPackagePair( pCharObject->m_pMandokbuljinEffectPP );
			pCharObject->m_pMandokbuljinEffectPP = NULL;

			//HT_CHEAT :  
			if(g_bCheat && pObject == g_pMainChar)
			{
				SendCS_BT_MUGONGPREATTACK_REQ(OUTGONGID_MANDOKBULJIN,1,g_pMainChar->m_dwServerID,
											0,0,0,0,0,0,0,0);
			}
		}
		break;
	case OUTGONGID_GYUISIKDAEBUB:	//  执细碳  汛.
		{
			if( bExistGyuisikdaebub )
			{
				sArrayData* pData = XiahArrayIndex::g_MugongTemplate.GetData(OUTGONGID_GYUISIKDAEBUB);

				if( pData == NULL)
				{
					DBG_Put(_T("装  坛?"));
					break;
				}

				int ani_index = pData->GetInt(2);

				pCharObject->SetAnimation( XiahAniType::eLAT_Mugong, XiahAniType::eLAT_Stand, ani_index, -1, 1.0f);
				pCharObject->m_CharRender.StopEffect();
				pCharObject->m_CharRender.SetReverseAnimation();
				pCharObject->m_CharRender.SetLoopAnimation( FALSE);
				pCharObject->m_bNowGyuisikdaebub = false;
			}
		}
		break;
	case OUTGONGID_GWANGMADOKGONG:	// .
        pCharObject->m_bNowGwangmadokgong = false;
		break;
	case REBRITH_GWANGMASINGONG:	//HT_0711 :  (虐)
        pCharObject->m_bNowGwangmadokgong = false;
		break;
	case OUTGONGID_DOKMU:
		if( pCharObject->m_pDokmuEffectPP )
		{
			g_EffectManager.DeqEffectPackagePair( pCharObject->m_pDokmuEffectPP );
			pCharObject->m_pDokmuEffectPP;
		}
		break;
	case YUN_SUSINKIKANG:
		{
			if(OBJTYPE_PET == bObjectType)
			{
				pCharObject->m_CharRender.SetLocalScale(Vector3(1.0f, 1.0f, 1.0f));

				sPetInfo* pPetInfo = (sPetInfo*)pCharObject->m_pPrivateData;

				if(pPetInfo)
				{
					XiahObject::CXiahObject* pMainObject = XiahObject::g_XiahObjectManager.FindXiahObject(MAKEOBJECTID(0, pPetInfo->dwOwnID, OBJTYPE_PC));

					if(pMainObject == NULL)
						return FALSE;

					CXiahCharObject* pMainCharObject = reinterpret_cast<CXiahCharObject*>(pMainObject->m_pObject);

					if(pMainCharObject == NULL)
						return FALSE;

					pMainCharObject->m_KeepUpMugongList.Delete(dwMugongID);
				}
				//HT_CHEAT : 疟獍?
				if(g_bCheat)
				{
					WORD wPosX, wPosY;
					pCharObject->GetPosition( wPosX, wPosY);

					SendCS_BT_MUGONGPREATTACK_REQ(YUN_SUSINKIKANG,1,g_pMainChar->m_dwServerID,
												1,1,1, OBJTYPE_PET, pPetInfo->dwID, wPosX, wPosY, 1);
				}
			}			
		}		
		break;
	};// switch

	//HT_0530  馨 

	//拳   冒目 俜 努叹飘 卮.
	//if( dwMugongID == WHA_DRAGONSINJANG || dwMugongID == WHA_DRAGONSUNGCHEON)
	//{
	//	if( pCharObject->m_pWha_DragonPP )
	//	{
	//		g_EffectManager.DeqEffectPackagePair( pCharObject->m_pWha_DragonPP );
	//		pCharObject->m_pWha_DragonPP = NULL;
	//	}
	//}

	if( dwMugongID == BING_DRAGONSINJANG || dwMugongID == BING_DRAGONSUNGCHEON)
	{
		if( pCharObject->m_pBing_DragonPP )
		{
			g_EffectManager.DeqEffectPackagePair( pCharObject->m_pBing_DragonPP );
			pCharObject->m_pBing_DragonPP = NULL;
		}
	}

	if( dwMugongID == DOK_DRAGONSINJANG || dwMugongID == DOK_DRAGONSUNGCHEON)
	{
		if( pCharObject->m_pDok_DragonPP )
		{
			g_EffectManager.DeqEffectPackagePair( pCharObject->m_pDok_DragonPP );
			pCharObject->m_pDok_DragonPP = NULL;
		}
	}

	if( dwMugongID == NOI_DRAGONSINJANG || dwMugongID == NOI_DRAGONSUNGCHEON)
	{
		if( pCharObject->m_pNoi_DragonPP )
		{
			g_EffectManager.DeqEffectPackagePair( pCharObject->m_pNoi_DragonPP );
			pCharObject->m_pNoi_DragonPP = NULL;
		}
	}

	// 
	if(g_pMainChar->m_dwServerID == dwObjectID)
	{
		switch(dwMugongID)
		{
		case OUTGONGID_ILYUIDOGANG:
		case OUTGONGID_YUESUSINYUNG:
		case OUTGONGID_JILPUNGBO:
		case OUTGONGID_CHOSANGBI:
			g_MainCharInfo.m_bFastMove = false;
			break;
		}
	}

	if(dwMugongID == YA_EUNSINSUL)
	{
		pCharObject->m_bRenderOK = true;
	}

	return TRUE;
}

//---------------------------------------------------------------------------------------
int OnCS_BT_KEEPUPMUGONGSTATUS_ACK( CMsg &msg)
{
	BYTE	bResult			=0;	
	BYTE	bObjectType		=0;
	BYTE	bMugongNum		=0;	
	DWORD	dwObjectID		=0;
		
	msg
		>> bResult
		>> dwObjectID
		>> bObjectType
		>> bMugongNum;

	XiahObject::CXiahObject* pObject = XiahObject::g_XiahObjectManager.FindXiahObject( MAKEOBJECTID( 0, dwObjectID,bObjectType));

	if( pObject == NULL)
	{
		DBG_Put(_T("眉~~~"));
		return TRUE;
	}

	CXiahCharObject* pCharObject = reinterpret_cast<CXiahCharObject*>(pObject->m_pObject);

	if(pCharObject == NULL)
	{
		DBG_LogFile( _T("OnCS_BT_KEEPUPMUGONGSTATUS_ACK "));
		return TRUE;
	}

	DWORD	dwMugongID		=0;
	BYTE	bMugongLevel	=0;

	for(int i=0; i < bMugongNum; i++) 
	{
		msg
			>> dwMugongID
			>> bMugongLevel;

		DWORD dwTime = 0;
		//HT_0601   		
		sArrayData* pMugongList = NULL;
		//   毯 9掳 冒  
		if((dwMugongID >= WHA_DRAGONSINJANG) && (dwMugongID <= YA_HOJUNGKANGKI))
			pMugongList = XiahArrayIndex::g_RebirthMugong_List.GetData( dwMugongID, bMugongLevel);
		else
		{
			pMugongList = XiahArrayIndex::g_MugongList.GetData( dwMugongID, bMugongLevel);

			if(pMugongList)
				dwTime = pMugongList->GetInt(9);
		}

		if( !pMugongList )
			continue;

		pCharObject->m_KeepUpMugongList.Add( dwMugongID, bMugongLevel, dwTime);

		// 趣 ?
		if( dwMugongID == OUTGONGID_GWANGMADOKGONG )
		{
			pCharObject->m_bNowGwangmadokgong = true;
			pCharObject->m_fLocalScaleForGwangmadokgong = GWANGMADOKGONG_CHARSCALE - 0.1f;
		}
		//HT_0711 :   (虐)
		if( dwMugongID == REBRITH_GWANGMASINGONG )
		{
			pCharObject->m_bNowGwangmadokgong = true;
			pCharObject->m_fLocalScaleForGwangmadokgong = GWANGMADOKGONG_CHARSCALE - 0.1f;
		}
	}
	return TRUE;
}

//---------------------------------------------------------------------------------------
int OnCS_BT_KEEPUPMUGONGSTATUSLIST_ACK( CMsg &msg)
{
	BYTE	bResult		=0;
	BYTE	bCharNum	=0;	
	BYTE	bObjectType	=0;
	BYTE	bMugongNum	=0;
	BYTE	bMugongLevel=0;
	DWORD	dwMugongID	=0;
	DWORD	dwObjectID	=0;
	

	msg
		>> bResult
		>> bCharNum;
	
	for(int i=0; i < bCharNum; i++) 
	{
		msg
			>> dwObjectID
			>> bObjectType
			>> bMugongNum;
		
		for(int j=0; j < bMugongNum; j++) 
		{
			msg
				>> dwMugongID
				>> bMugongLevel;
		
			XiahObject::CXiahObject* pObject = XiahObject::g_XiahObjectManager.FindXiahObject( MAKEOBJECTID( 0, dwObjectID, bObjectType));

			if( pObject == NULL)
			{
				DBG_Put(_T("!!!~~~"));
				continue;
			}

			DWORD dwTime =0;
			//HT_0601   
			sArrayData* pMugongList = NULL;
			//   毯 9掳 冒  
			// 确 习  劬 野
			if((dwMugongID >= WHA_DRAGONSINJANG) && (dwMugongID <= YA_HOJUNGKANGKI))
				pMugongList = XiahArrayIndex::g_RebirthMugong_List.GetData( dwMugongID, bMugongLevel);
			else
			{
				pMugongList = XiahArrayIndex::g_MugongList.GetData( dwMugongID, bMugongLevel);
				if(pMugongList)
					dwTime = pMugongList->GetInt(9);
			}

			if( pMugongList)
			{
				CXiahCharObject* pCharObject = reinterpret_cast<CXiahCharObject*>(pObject->m_pObject);

				if(pCharObject == NULL)
				{
					DBG_LogFile( _T("OnCS_BT_KEEPUPMUGONGSTATUSLIST_ACK "));
					continue;
				}

				if(dwMugongID == YA_EUNSINSUL && pObject != g_pMainChar)
					pCharObject->m_bRenderOK = false;
				else	
					pCharObject->m_KeepUpMugongList.Add(dwMugongID, bMugongLevel, dwTime);

				if(dwMugongID == YUN_SUSINKIKANG && OBJTYPE_PET == bObjectType)
				{
					pCharObject->m_CharRender.SetLocalScale(Vector3(2.0f, 2.0f, 2.0f));

					sPetInfo* pPetInfo = (sPetInfo*)pCharObject->m_pPrivateData;

					if(pPetInfo)
					{
						XiahObject::CXiahObject* pMainObject = XiahObject::g_XiahObjectManager.FindXiahObject(MAKEOBJECTID(0, pPetInfo->dwOwnID, OBJTYPE_PC));

						if(pMainObject == NULL)
							return FALSE;

						CXiahCharObject* pMainCharObject = reinterpret_cast<CXiahCharObject*>(pMainObject->m_pObject);

						if(pMainCharObject == NULL)
							return FALSE;

						pMainCharObject->m_KeepUpMugongList.Add(dwMugongID, bMugongLevel, dwTime);
					}
				}

				// 趣 ?
				if( dwMugongID == OUTGONGID_GWANGMADOKGONG )
				{
					pCharObject->m_bNowGwangmadokgong = true;
					pCharObject->m_fLocalScaleForGwangmadokgong = GWANGMADOKGONG_CHARSCALE - 0.1f;
				}
				//HT_0711 :   (虐)
				if( dwMugongID == REBRITH_GWANGMASINGONG )
				{
					pCharObject->m_bNowGwangmadokgong = true;
					pCharObject->m_fLocalScaleForGwangmadokgong = GWANGMADOKGONG_CHARSCALE - 0.1f;
				}
			}
		}
	}

	return TRUE;
}

int OnCS_BT_KILLSUCCESS_ACK( CMsg &msg)
{
	BYTE bObjectType;
	DWORD dwObjectID;
	DWORD dwExp;

	msg
		>> bObjectType
		>> dwObjectID
		>> dwExp;

	//  NPC  摹 裙 飘 .
	if( bObjectType == OBJTYPE_NPC )
	{
		XiahObject::CXiahObject* pNpc;
		pNpc = XiahObject::g_XiahObjectManager.FindXiahObject( MAKEOBJECTID(0, dwObjectID, bObjectType) );
		if( pNpc == NULL ) return 1;

		CXiahCharObject* pNpcCharObject = (CXiahCharObject*)pNpc->m_pObject;

		if(pNpcCharObject == NULL)
		{
			DBG_LogFile( _T("OnCS_BT_KILLSUCCESS_ACK "));
			return TRUE;
		}

		//  筛  麓.
		CXiahCharObject* pMainChar = (CXiahCharObject*)g_pMainChar->m_pObject;

		if(pMainChar == NULL)
		{
			DBG_LogFile( _T("OnCS_BT_KILLSUCCESS_ACK "));
			return TRUE;
		}

		Vector3 vMainCharSize = pMainChar->m_LocalBound.Size();
		Vector3 vMainCharPos = pMainChar->m_Position + Vector3( 0, vMainCharSize.y*2.0f/3.0f, 0 );

		_EFFECTPACKAGE* pEffectPackage = g_EffectManager.EnqExpAcquireEffectImmediately( (VECTOR)vMainCharPos );
		// 摹 裙 飘 , NPC 飘 飘 谴碌, 
		// 状 NPC   努叹飘 欧 飘 NPC 某 桶  
		// 飘 飘 桶 寻 挪.  贪 飘  .
		// matrix
		if( pEffectPackage )
		{
			pEffectPackage->pEffectRender->pPackagePair->WorldMatrix = (MATRIX)pNpcCharObject->m_ObjectTM;
			//pEffectPackage->pEffectRender->pPackagePair->pWorldMatrix = (MATRIX*)pNpcCharObject->m_CharRender.GetCharTM();
		}
	}

	return 0;
}

/**
 * 
 * \param &msg 
 * \return 
 */
int OnCS_BT_LEVELUP_ACK(CMsg &msg)
{
	DWORD dwCharID		 =0;
	BYTE bLevelUp		 =0;
	BYTE bTpLevelUp		 =0;
	BYTE bFiveElmLevelUp =0;

	msg
		>> dwCharID
		>> bLevelUp
		>> bTpLevelUp
		>> bFiveElmLevelUp;

	// 拳榭?檀 俑 某桶  枚.
	XiahObject::CXiahObject* pPc = XiahObject::g_XiahObjectManager.FindXiahObject( MAKEOBJECTID(0, dwCharID, OBJTYPE_PC) );
	if( pPc == NULL )
		return 1;

	CXiahCharObject* pPcCharObject = (CXiahCharObject*)pPc->m_pObject;

	if(pPcCharObject == NULL)
	{
		DBG_LogFile( _T("OnCS_BT_LEVELUP_ACK "));
		return TRUE;
	}

	if( bLevelUp )
	{
		//   飘
		_EFFECTPACKAGE* pPackage = g_EffectManager.EnqLevelUpEffectImmediately( LEVELUP_GAPJA );
		if( pPackage && pPackage->pEffectRender && pPackage->pEffectRender->pPackagePair )
		{
			pPackage->pEffectRender->pPackagePair->pWorldMatrix = (MATRIX*)pPcCharObject->m_CharRender.GetCharTM();
			//pPackage->pEffectRender->pPackagePair->WorldMatrix = (MATRIX)pPcCharObject->m_ObjectTM;

			// 鼐 某桶  飘  .
			pPcCharObject->m_EffectPPList.push_back( pPackage->pEffectRender->pPackagePair );
		}
	}

	if( bTpLevelUp )
	{
		//   飘
		_EFFECTPACKAGE* pPackage = g_EffectManager.EnqLevelUpEffectImmediately( LEVELUP_TP );
		if( pPackage && pPackage->pEffectRender && pPackage->pEffectRender->pPackagePair )
		{
			pPackage->pEffectRender->pPackagePair->pWorldMatrix = (MATRIX*)pPcCharObject->m_CharRender.GetCharTM();
			//pPackage->pEffectRender->pPackagePair->WorldMatrix = (MATRIX)pPcCharObject->m_ObjectTM;

            pPcCharObject->m_EffectPPList.push_back( pPackage->pEffectRender->pPackagePair );
		}
	}

	return 0;
}

int OnCS_BT_ASKPARTYBATTLE_ACK(CMsg &msg)
{
	BYTE bAction			=0;
	DWORD dwAskPartyID		=0;
	DWORD dwAskCharID		=0;
	DWORD dwTargetPartyID	=0;
	DWORD dwTargetCharID	=0;
	DWORD dwBetMoney		=0;

	msg
		>> bAction
		>> dwAskPartyID
		>> dwAskCharID
		>> dwTargetPartyID
		>> dwTargetCharID
		>> dwBetMoney;

	switch( bAction )
	{
	case 0:	// 没
		{
			g_MainCharInfo.m_dwAskID		= dwAskCharID;
			g_MainCharInfo.m_dwAskPartyID	= dwAskPartyID;
			g_MainCharInfo.m_dwBetMoney		= dwBetMoney;

			TCHAR strTemp[256] = {0,};
			_stprintf(strTemp, IDS_DANWAR_ASK, MoneyCommaStr(dwBetMoney).data());
			g_pUIManager->ShowNotice( strTemp, NOTICE_FRAME_OKCANCEL, NOTICE_FRAME_DAN_WAR_ASK);
		}
		break;
	case 1:	// 
		{
			g_MainCharInfo.ShowHelpMessage(IDS_BATTLE_AGREE, TEXTEFFECT_COLOR_WARNING);
		}
		break;
	case 9:	// 
		{
			g_MainCharInfo.ShowHelpMessage(IDS_BATTLE_DISAGREE, TEXTEFFECT_COLOR_WARNING);
		}
		break;
		// Error
	case 10:	// ACT_ASKPARTYBATTLE_MYMONEYSHORT
		g_MainCharInfo.ShowHelpMessage(IDS_DANBATTLE_MYMONEYSHORT, TEXTEFFECT_COLOR_WARNING);
		break;
	case 11:	// ACT_ASKPARTYBATTLE_YOUMONEYSHORT
		g_MainCharInfo.ShowHelpMessage(IDS_DANBATTLE_YOUMONEYSHORT, TEXTEFFECT_COLOR_WARNING);
		break;
	case 12:	//ACT_ASKPARTYBATTLE_MONEYOVER
		g_MainCharInfo.ShowHelpMessage(IDS_DANBATTLE_MONEYOVER, TEXTEFFECT_COLOR_WARNING);
		break;
	case 13:	// ACT_ASKPARTYBATTLE_NOTFINDTARGET
		g_MainCharInfo.ShowHelpMessage(IDS_DANBATTLE_NOTFINDTARGET, TEXTEFFECT_COLOR_WARNING);
		break;
	case 14:	// ACT_ASKPARTYBATTLE_OPTIONCLOSE
		g_MainCharInfo.ShowHelpMessage(IDS_DANBATTLE_OPTIONCLOSE, TEXTEFFECT_COLOR_WARNING);
		break;
	};

	return 0;
}

/**
 *   
 * \param &msg 
 * \return 
 */
int OnCS_BT_STARTPARTYBATTLE_ACK(CMsg &msg)
{
	sString strNickName1;
	sString strNickName2;

	msg
		>> strNickName1
		>> strNickName2;

	TCHAR strTemp[256] = {0,};
	_stprintf(strTemp, IDS_STARTPARTYBATTLE, (LPCTSTR)strNickName1, (LPCTSTR)strNickName2);

	g_MainCharInfo.SpecialChatMessage(strTemp, 0);

	return 0;
}

/**
 *   
 * \param &msg 
 * \return 
 */
int OnCS_BT_ENDPARTYBATTLE_ACK(CMsg &msg)
{
	sString strWinName;
	DWORD dwWinMoney	=0;

	msg
		>> strWinName
		>> dwWinMoney;

	TCHAR strTemp[128] = {0,};
	_stprintf(strTemp, IDS_ENDPARTYBATTLE_1, (LPCTSTR)strWinName);
	g_MainCharInfo.SpecialChatMessage(strTemp, 0);

	memset(strTemp, 0, 128);
	_stprintf(strTemp, IDS_ENDPARTYBATTLE_2, (LPCTSTR)strWinName, MoneyCommaStr(dwWinMoney).data());
	g_MainCharInfo.SpecialChatMessage(strTemp, 0);

	g_MainCharInfo.m_bPortalMove = true;

	return 0;
}


// 拦截 CS_BT_KEEPUPMUGONGSTART_ACK (0x402C)
// 彻底解决原版引擎在怪物身上不写入 KeepUpMugongList 的 Bug，强制让怪物也同步 Buff 状态以驱动目标信息面板渲染 Debuff 图标
int My_OnCS_BT_KEEPUPMUGONGSTART_ACK(CMsg &msg)
{
	// 1. 复制一份 CMsg 以防打乱原本解包流程
	CMsg msgCopy(msg);

	BYTE bResult = 0;
	DWORD dwObjectID = 0;
	BYTE bObjectType = 0;
	DWORD dwMugongID = 0;
	BYTE bMugongLevel = 0;

	msgCopy >> bResult
			>> dwObjectID
			>> bObjectType
			>> dwMugongID
			>> bMugongLevel;

	// 用于保存查找到的对象指针，在自建维护中做类型与主角判断
	XiahObject::CXiahObject* pObject = NULL;

	// 2. 只有原生封包成功时才做内存状态同步与自建渲染维护
	if (bResult == 0)
	{
		pObject = XiahObject::g_XiahObjectManager.FindXiahObject(MAKEOBJECTID(0, dwObjectID, bObjectType));
		if (pObject && pObject->m_pObject)
		{
			CXiahCharObject* pChar = (CXiahCharObject*)pObject->m_pObject;
			
			// 从 pc_mugong_list.idx 模板中获取 Buff 持续时间 (31号字段，单位为秒)，默认8秒
			DWORD dwDurationMs = 8000;
			sArrayData* pMugongList = XiahArrayIndex::g_MugongList.GetData(dwMugongID, bMugongLevel);
			if (pMugongList != NULL)
			{
				int nSec = pMugongList->GetInt(31);
				if (nSec > 0)
				{
					dwDurationMs = (DWORD)nSec * 1000;
				}
			}

			// A. 强制写入对象的状态列表中，保障驱动目标状态面板（WINDOW_TARGET_STATUS 192 窗口）图标渲染
			pChar->m_KeepUpMugongList.Add(dwMugongID, bMugongLevel, dwDurationMs);
			pChar->AddDebuff((WORD)dwMugongID, dwDurationMs);

			// B. 如果受击者是主角自己，手动在客户端右上角图标列表中添加/更新，实现 100% 自主安全托管
			if (pObject == g_pMainChar)
			{
				bool bIconExists = false;
				for (size_t i = 0; i < g_MainCharInfo.m_vkeepUpMugongIconList.size(); i++)
				{
					if (g_MainCharInfo.m_vkeepUpMugongIconList[i]->m_MugongID == (WORD)dwMugongID)
					{
						// 如果已存在，刷新开始时间戳与等级
						g_MainCharInfo.m_vkeepUpMugongIconList[i]->m_CurTime = g_dwCurTime;
						g_MainCharInfo.m_vkeepUpMugongIconList[i]->m_MugongLevel = bMugongLevel;
						g_MainCharInfo.m_vkeepUpMugongIconList[i]->m_DrawIcon = true;
						bIconExists = true;
						break;
					}
				}

				if (!bIconExists)
				{
					sKEEPUPMUGONGICONLIST* pNewIcon = new sKEEPUPMUGONGICONLIST;
					pNewIcon->m_MugongID = (WORD)dwMugongID;
					pNewIcon->m_CurTime = g_dwCurTime;
					pNewIcon->m_DrawIcon = true;
					pNewIcon->m_MugongLevel = bMugongLevel;
					g_MainCharInfo.m_vkeepUpMugongIconList.push_back(pNewIcon);
				}
			}

			DBG_LogFile(_T("[DebuffSync] Intercepted MUGONGSTART: Object=%s ID=%u Type=%d, MugongID=%u, Level=%d, Duration=%u\n"),
				(LPCTSTR)pChar->m_szObjectName, dwObjectID, (int)bObjectType, dwMugongID, (int)bMugongLevel, dwDurationMs);

			DBG_LogFile(_T("[DebuffSync] Compare ID (START): MainCharServerID=%u, dwObjectID=%u, Equal=%d\n"),
				g_pMainChar ? g_pMainChar->m_dwServerID : 0, dwObjectID, 
				(g_pMainChar && g_pMainChar->m_dwServerID == dwObjectID) ? 1 : 0);
		}
	}

	// 3. 彻底拦截！绝对不回调原版引擎自带的 OnCS_BT_KEEPUPMUGONGSTART_ACK。
	// 从底层彻底规避并隔离原版引擎在解析 0x402C 时因特效缺失或骨骼指针访问引发的全部空指针/Heap Corruption 崩溃。
	return 0;
}

// 拦截 CS_BT_KEEPUPMUGONGEND_ACK (0x402E)
// 同样强制清除怪物的内存 Buff 状态，同步清除目标面板上的图标
int My_OnCS_BT_KEEPUPMUGONGEND_ACK(CMsg &msg)
{
	CMsg msgCopy(msg);

	BYTE bResult = 0;
	DWORD dwObjectID = 0;
	BYTE bObjectType = 0;
	DWORD dwMugongID = 0;
	BYTE bMugongLevel = 0;

	msgCopy >> bResult
			>> dwObjectID
			>> bObjectType
			>> dwMugongID
			>> bMugongLevel;

	if (bResult == 0)
	{
		XiahObject::CXiahObject* pObject = XiahObject::g_XiahObjectManager.FindXiahObject(MAKEOBJECTID(0, dwObjectID, bObjectType));
		if (pObject && pObject->m_pObject)
		{
			CXiahCharObject* pChar = (CXiahCharObject*)pObject->m_pObject;
			
			// 如果受击者是主角自己，直接手动在客户端右上角图标列表中擦除并释放内存
			if (pObject == g_pMainChar)
			{
				for (auto it = g_MainCharInfo.m_vkeepUpMugongIconList.begin(); it != g_MainCharInfo.m_vkeepUpMugongIconList.end(); )
				{
					if ((*it)->m_MugongID == dwMugongID)
					{
						delete (*it); // 释放分配的内存，杜绝泄露
						it = g_MainCharInfo.m_vkeepUpMugongIconList.erase(it);
						break; // 一次清除包只清除一个对应的 Buff 图标实例
					}
					else
					{
						++it;
					}
				}
			}

			// 从怪物的状态列表中清除
			pChar->m_KeepUpMugongList.Delete(dwMugongID);
			
			for (int i = (int)pChar->m_vDebuffList.size() - 1; i >= 0; i--)
			{
				if (pChar->m_vDebuffList[i].wMugongID == dwMugongID)
				{
					pChar->m_vDebuffList.erase(pChar->m_vDebuffList.begin() + i);
				}
			}

			DBG_LogFile(_T("[DebuffSync] Intercepted MUGONGEND: Object=%s ID=%u Type=%d, MugongID=%u\n"),
				(LPCTSTR)pChar->m_szObjectName, dwObjectID, (int)bObjectType, dwMugongID);

			DBG_LogFile(_T("[DebuffSync] Compare ID (END): MainCharServerID=%u, dwObjectID=%u, Equal=%d\n"),
				g_pMainChar ? g_pMainChar->m_dwServerID : 0, dwObjectID, 
				(g_pMainChar && g_pMainChar->m_dwServerID == dwObjectID) ? 1 : 0);
		}
	}

	extern int OnCS_BT_KEEPUPMUGONGEND_ACK(CMsg &msg);
	return OnCS_BT_KEEPUPMUGONGEND_ACK(msg);
}
