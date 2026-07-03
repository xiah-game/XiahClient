#include "precompile.h"
#include "SkillTime.h"

BOOL IsSkillOnCD(DWORD dwMugongID) {
    SkillTime::SkillList& list = g_SkillTime.GetSkillList();
    SkillTime::SkillList::iterator iter = list.find(dwMugongID);
    if (iter != list.end()) {
        if (!iter->second.bEnd) {
            return TRUE;
        }
    }
    return FALSE;
}

#include "XiahCheatConfig.h"
#include "AppData.h"
#include "XiahSocket.h"

#include "XiahMap.h"
#include "XiahGame_Handler_Sender.h"
#include "XiahGame_Intro.h"

#include "InterfaceDefine.h"
#include "CurseFilter.h"
#include <mmsystem.h>

#include "XiahGame_Main.h"
#include "StringDefine.h"
#include "CharacterInfo.h"
////////////////////////////////////////
// IT
////////////////////////////////////////

// 인증 서버용 로그인
void SendCS_IT_LOGIN_AUTH_REQ(sString szAccountID, sString szPasswd)
{
	CMsg msg;

	g_AppData.m_strUserName = szAccountID;

	msg.ID( CS_IT_LOGIN_REQ)
		<< szAccountID
		<< szPasswd
		<< g_info.m_version
		<< sString(_T("027E648874F70C349119DA56AEE1D2ED"));

	XiahNetwork::SendNetMsg( msg);
}


void SendCS_IT_LOGIN_REQ()
{
	// For Unitserver
	CMsg msg;

	sString strUserName = g_AppData.m_strUserName;
	DWORD	dwKey		= g_AppData.m_dwKey;
	BYTE	byChannelID	= g_AppData.m_byChannelID;	

	msg.ID( CS_IT_LOGINCHECK_REQ)
		<< strUserName
		<< dwKey
		<< byChannelID //byWorldID
		<< (WORD)PROTOCOL_VERSION;

	XiahNetwork::SendNetMsg( msg);
}

void SendCS_IT_CHARACTERLIST_REQ()
{
	CMsg msg;

	msg.ID( CS_IT_CHARACTERLIST_REQ);

	XiahNetwork::SendNetMsg( msg);
}

void SendCS_IT_NEWCHARACTER_REQ( BYTE byType, sString strUserName)
{
	CMsg msg;

	msg.ID( CS_IT_NEWCHARACTER_REQ)
		<< byType
		<< strUserName;

	XiahNetwork::SendNetMsg( msg);
}

void SendCS_IT_DELCHARACTER_REQ( DWORD dwCharID)
{
	CMsg msg;

	msg.ID( CS_IT_DELCHARACTER_REQ)
		<< dwCharID;

	XiahNetwork::SendNetMsg( msg);
}

void SendCS_IT_CHARSTATUSINFO_REQ()
{
	CMsg msg;

	DWORD dwObjectID	= g_pIntro->GetCurrentChar()->m_dwObjectID;
	DWORD dwMapID		= g_pIntro->GetCurrentChar()->m_dwMapID;

	msg.ID( CS_IT_CHARSTATUSINFO_REQ)
		<< dwObjectID
		<< dwMapID;

	XiahNetwork::SendNetMsg( msg);
}


void SendCS_IT_MAPINFO_REQ(DWORD dwMapID)
{
	CMsg msg;

	msg.ID ( CS_IT_MAPINFO_REQ)
		<< dwMapID;

	XiahNetwork::SendNetMsg( msg);
}

void SendCS_IT_MUGONGLIST_REQ( BYTE bMugongType)
{
	CMsg msg;

	msg.ID(CS_IT_MUGONGLIST_REQ)
		<< bMugongType;

	XiahNetwork::SendNetMsg( msg);
}

void SendCS_IT_ITEMLIST_REQ( BYTE bSackType)
{
	CMsg msg;

	msg.ID(CS_IT_ITEMLIST_REQ)
		<< bSackType;

	XiahNetwork::SendNetMsg( msg);
}

void SendCS_IT_IMREADY_REQ(DWORD dwObjectID, DWORD dwMapID)
{
	CMsg msg;

	msg.ID ( CS_IT_IMREADY_REQ)
		<< dwObjectID
		<< dwMapID;

	XiahNetwork::SendNetMsg( msg);
}

void SendCS_IT_CHARINFO_REQ(DWORD dwObjectID)
{
	CMsg msg;

	msg.ID( CS_IT_CHARINFO_REQ)
		<< dwObjectID
		<< XiahMap::g_XiahMap.m_MapInfo.m_dwMapID;

	XiahNetwork::SendNetMsg( msg);
}

void SendCS_IT_CHARSLOT_REQ()
{
	CMsg msg;

	msg.ID(CS_IT_CHARSLOT_REQ);
		
	XiahNetwork::SendNetMsg( msg);
}

void SendCS_IT_SETSLOT_REQ(DWORD dwValue, BYTE bSlot)
{
	CMsg msg;

	msg.ID(CS_IT_SETSLOT_REQ)
		<< dwValue
		<< bSlot;

	XiahNetwork::SendNetMsg( msg);
}

void SendCS_IT_SPEEDPING_REQ( DWORD dwClientTick)
{
	CMsg msg;

	msg.ID(CS_IT_SPEEDPING_REQ)
		<< dwClientTick
		<< g_MainCharInfo.m_bPlusSpeed
		<< (BYTE)g_MainCharInfo.m_bEvSocketItemUse;

	XiahNetwork::SendNetMsg( msg);
}

void SendCS_IT_CHANGEPW_REQ(sString szAccountID, sString szPasswd, sString strChangePasswd)
{
	CMsg msg;

	msg.ID(CS_IT_CHANGEPW_REQ)
		<< szAccountID
		<< szPasswd
		<< strChangePasswd;

	XiahNetwork::SendNetMsg(msg);
}








////////////////////////////////////////
// NV
////////////////////////////////////////
void SendCS_NV_STARTGAME_REQ()
// 게임을 시작해도 되는지 인증서버에서 받은 키값을 서버에 보낸다.
{
	CMsg msg;

	DWORD	dwObjectID = g_pIntro->GetCurrentChar()->m_dwObjectID;
	DWORD	dwKey = g_AppData.m_dwKey;

	msg.ID( CS_NV_STARTGAME_REQ)
		<< dwObjectID
		<< dwKey;

	XiahNetwork::SendNetMsg( msg);
}

void SendCS_NV_MAPENTER_REQ(DWORD dwObjectID,DWORD dwMapID)
{
	CMsg msg;

	// 맵간에 이동을 할 수도 있겠다

	msg.ID ( CS_NV_MAPENTER_REQ)
		<< dwObjectID
		<< dwMapID;

	XiahNetwork::SendNetMsg( msg);

	DBG_Put(_T("MapEnterReq보냄"));
}

void SendCS_NV_MAPMOVE_REQ(DWORD dwMapID,WORD wPosX,WORD wPosY)
{
	CMsg msg;

	if(wPosX < 0 || wPosX > 2047 || wPosY < 0 || wPosY > 2047) 
	{
		DBG_LogFile("Invalid Position in SendCS_NV_MAPMOVE_REQ");
		return;
	}

	msg.ID( CS_NV_MAPMOVE_REQ)
		<< dwMapID
		<< wPosX
		<< wPosY;

	XiahNetwork::SendNetMsg( msg);

	bool bShowLoadImage = true;

	if( g_MainCharInfo.m_bMainCharDie || g_MainCharInfo.m_bMainCharMapMoveItemUse )
		bShowLoadImage = false;

	if( bShowLoadImage )
	{
		// 2004.07.20 이벤트용 로딩화면
		/*
		if( rand() % 2 )
			g_MainCharInfo.OpenFrame( EVENT_LOADING_1 );
		else
			g_MainCharInfo.OpenFrame( EVENT_LOADING_2 );
		*/

		 g_MainCharInfo.OpenFrame(LOADING_IMAGE3); //HO_0702_07 등급표시 : 등급표시와 함게 스타트로딩과 게임로딩 부분이 동일 이미지로 처리된다.

		//등급표시 적용전 코드 나중에 지워 버리자 ..; 등급표시 전에는 나이 구분이 있엇다...
		// 성인서버용로딩
		//if(g_AppData.m_bAdult)
		//	g_MainCharInfo.OpenFrame(LOADING_IMAGE2);
		//else
		//	g_MainCharInfo.OpenFrame(LOADING_IMAGE);		
	}
}

void SendCS_NV_MAPMOVE_REQ(DWORD dwMapID)
{
	CMsg msg;

	msg.ID( CS_NV_MAPMOVE_REQ)
		<< dwMapID
		<< 0
		<< 0;
	
	XiahNetwork::SendNetMsg( msg);

	// 2004.07.20 이벤트용 로딩화면
	/*
	if( rand() % 2 )
		g_MainCharInfo.OpenFrame( EVENT_LOADING_1 );
	else
		g_MainCharInfo.OpenFrame( EVENT_LOADING_2 );
	*/

	g_MainCharInfo.OpenFrame(LOADING_IMAGE3); //HO_0702_07 등급표시 : 등급표시와 함게 스타트로딩과 게임로딩 부분이 동일 이미지로 처리된다.

	//등급표시 적용전 코드 나중에 지워 버리자 ..; 등급표시 전에는 나이 구분이 있엇다...
	// 성인서버용로딩
	//if(g_AppData.m_bAdult)
	//	g_MainCharInfo.OpenFrame(LOADING_IMAGE2);
	//else
	//	g_MainCharInfo.OpenFrame(LOADING_IMAGE);	
}

void SendCS_NV_ENDGAME_REQ(BYTE bChChange)
{
	BYTE bEnd = 1;

	CMsg msg;
	msg.ID ( CS_NV_ENDGAME_REQ)
		<< bEnd
		<< bChChange;

	XiahNetwork::SendNetMsg( msg);
}

void SendCS_NV_STARTMOVE_REQ( DWORD dwObjectID, WORD	wPosX, WORD	wPosY, BYTE	bHeight, WORD	wDesPosX, WORD	wDesPosY, BYTE	bDesHeight, WORD	wDirection, BYTE	bStatus, BYTE	bSpeed )
{
	CMsg msg;

	if(wPosX < 0 || wPosX > 2047 || wPosY < 0 || wPosY > 2047) 
	{
		DBG_LogFile("Invalid Position in SendCS_NV_STARTMOVE_REQ");
		return;
	}

	msg.ID( CS_NV_STARTMOVE_REQ)
		<< dwObjectID
		<< wPosX
		<< wPosY
		<< bHeight
		<< wDesPosX
		<< wDesPosY
		<< bDesHeight
		<< wDirection
		<< bStatus
		<< g_MainCharInfo.m_bPlusSpeed;

	XiahNetwork::SendNetMsg( msg);
}

void SendCS_NV_ENDMOVE_REQ(DWORD dwObjectID,WORD wPosX,WORD wPosY,BYTE bHeight,BYTE bState)
{
	CMsg msg;

	if(wPosX < 0 || wPosX > 2047 || wPosY < 0 || wPosY > 2047) 
	{
		DBG_LogFile("Invalid Position in SendCS_NV_ENDMOVE_REQ");
		return;
	}

	msg.ID( CS_NV_ENDMOVE_REQ)
		<< dwObjectID
		<< wPosX
		<< wPosY
		<< bHeight
		<< bState
		<< g_MainCharInfo.m_bPlusSpeed;

	XiahNetwork::SendNetMsg( msg);
}

void SendCS_NV_PORTALMOVE_REQ(DWORD dwObjectID,WORD wPosX,WORD wPosY,BYTE bHeight,BYTE bState)
{
	CMsg msg;

	if(wPosX < 0 || wPosX > 2047 || wPosY < 0 || wPosY > 2047) 
	{
		DBG_LogFile("Invalid Position in SendCS_NV_PORTALMOVE_REQ");
		return;
	}

	msg.ID( CS_NV_PORTALMOVE_REQ)
		<< dwObjectID
		<< wPosX
		<< wPosY
		<< bHeight
		<< bState;

	XiahNetwork::SendNetMsg( msg);

	//g_MainCharInfo.OpenFrame( LOADING_IMAGE);
}


void SendCS_NV_SYNCMOVE_REQ(DWORD dwObjectID,WORD wPosX,WORD wPosY,BYTE bHeight,WORD wDesPosX,WORD wDesPosY,BYTE bDesHeight,WORD wDirection,BYTE bState,BYTE bWalkSpeed)
{
	CMsg msg;

	if(wPosX < 0 || wPosX > 2047 || wPosY < 0 || wPosY > 2047) 
	{
		DBG_LogFile("Invalid Position in SendCS_NV_SYNCMOVE_REQ");
		return;
	}

	msg.ID( CS_NV_SYNCMOVE_REQ)
		<< dwObjectID
		<< wPosX
		<< wPosY
		<< bHeight
		<< wDesPosX
		<< wDesPosY
		<< bDesHeight
		<< wDirection
		<< bState
		<< g_MainCharInfo.m_bPlusSpeed;

	XiahNetwork::SendNetMsg( msg);
}


/**
 * NPC 포탈 이동
 * \param dwMoveMapID 맵 ID
 */
void SendCS_NV_QUICKMOVE_REQ(DWORD dwMoveMapID)
{
	CMsg msg;

	msg.ID(CS_NV_QUICKMOVE_REQ)
		<< dwMoveMapID;

	XiahNetwork::SendNetMsg(msg);
}


/**
 * 문파대전 NPC 포탈 이동
 * \param dwMoveMapID 지역 ID
 */
void SendCS_NV_PRIVATEPORTAL_REQ(DWORD dwMoveMapID)
{
	CMsg msg;

	msg.ID(CS_NV_PRIVATEPORTAL_REQ)
		<< dwMoveMapID;

	XiahNetwork::SendNetMsg(msg);
}













////////////////////////////////////////
// NC
////////////////////////////////////////
void SendCS_NC_NPCINFO_REQ(DWORD dwObjectID)
{
	CMsg msg;

	msg.ID( CS_NC_NPCINFO_REQ)
		<< dwObjectID
		<< XiahMap::g_XiahMap.m_MapInfo.m_dwMapID;

	XiahNetwork::SendNetMsg( msg);
	//Sleep( 50);
}

void SendCS_NC_PETINFO_REQ( DWORD dwObjectID)
{
	CMsg msg;

	msg.ID( CS_NC_PETINFO_REQ)
		<< dwObjectID
		<< XiahMap::g_XiahMap.m_MapInfo.m_dwMapID;

	XiahNetwork::SendNetMsg( msg);
}

void SendCS_NC_PETDETAILINFO_REQ(DWORD dwObjectID)
{
	CMsg msg;

	msg.ID( CS_NC_PETDETAILINFO_REQ)
		<< XiahMap::g_XiahMap.m_MapInfo.m_dwMapID
		<< dwObjectID;

	XiahNetwork::SendNetMsg( msg);
}

void SendCS_NC_TAMING_REQ(DWORD dwTargetID,BYTE bTamingType)
{
	CMsg msg;

	msg.ID( CS_NC_TAMING_REQ)
		<< dwTargetID
		<< bTamingType;

	XiahNetwork::SendNetMsg( msg);
}

void SendCS_NC_FUNCTIONALNPCITEMLIST_REQ(DWORD dwMapID, DWORD dwObjectID, BYTE bSackCnt)
{
	CMsg msg;

	msg.ID(CS_NC_FUNCTIONALNPCITEMLIST_REQ)
		<< dwMapID
		<< dwObjectID
		<< bSackCnt;
	
	XiahNetwork::SendNetMsg( msg);
}

void SendCS_NC_STARTMOVE_REQ( DWORD dwObjectID, WORD wPosX, WORD wPosY,BYTE bHeight,WORD wDesPosX,WORD wDesPosY,BYTE bDesHeight,WORD wDirection,BYTE bState,BYTE bSpeed)
{
	CMsg msg;

	if(wPosX < 0 || wPosX > 2047 || wPosY < 0 || wPosY > 2047) 
	{
		DBG_LogFile("Invalid Position in SendCS_NC_STARTMOVE_REQ");
		return;
	}

	msg.ID(CS_NC_STARTMOVE_REQ)
		<< dwObjectID
		<< wPosX
		<< wPosY
		<< bHeight
		<< wDesPosX
		<< wDesPosY
		<< bDesHeight
		<< wDirection
		<< bState
		<< bSpeed;

	XiahNetwork::SendNetMsg( msg);
}

void SendCS_NC_SYNCMOVE_REQ( DWORD dwObjectID, WORD wPosX, WORD wPosY, BYTE bHeight, WORD wDesPosX, WORD wDesPosY,BYTE bDesHeight,WORD wDirection,BYTE bState,BYTE bSpeed)
{
	CMsg msg;

	if(wPosX < 0 || wPosX > 2047 || wPosY < 0 || wPosY > 2047) 
	{
		DBG_LogFile("Invalid Position in SendCS_NC_SYNCMOVE_REQ");
		return;
	}

	msg.ID(CS_NC_SYNCMOVE_REQ)
		<< dwObjectID
		<< wPosX
		<< wPosY
		<< bHeight
		<< wDesPosX
		<< wDesPosY
		<< bDesHeight
		<< wDirection
		<< bState
		<< bSpeed;

	XiahNetwork::SendNetMsg( msg);
}

void SendCS_NC_ENDMOVE_REQ(DWORD dwObjectID, WORD wPosX,WORD wPosY,BYTE bHeight,WORD wDirection,BYTE bState,BYTE bSpeed)
{
	CMsg msg;

	if(wPosX < 0 || wPosX > 2047 || wPosY < 0 || wPosY > 2047) 
	{
		DBG_LogFile("Invalid Position in SendCS_NC_ENDMOVE_REQ");
		return;
	}

	msg.ID( CS_NC_ENDMOVE_REQ)
		<< dwObjectID
		<< wPosX
		<< wPosY
		<< bHeight
		<< wDirection
		<< bState;

	XiahNetwork::SendNetMsg( msg);
/*
	sString str;
	str.printf("NC_ENDMOVE_REQ : %d", dwObjectID);
	g_MainCharInfo.ShowHelpMessage( (LPCTSTR)str);
*/
}

void SendCS_NC_MAPENTER_REQ(DWORD dwObjectID,DWORD dwMapID)
{
	CMsg msg;

	msg.ID( CS_NC_MAPENTER_REQ)
		<< dwObjectID
		<< dwMapID;

	XiahNetwork::SendNetMsg( msg);
}

void SendCS_NC_STATUSCHANGE_REQ(BYTE bObjectType,DWORD dwObjectID,BYTE bStatus,WORD wDirection)
{
	CMsg msg;

	msg.ID( CS_NC_STATUSCHANGE_REQ)
		<< bObjectType
		<< dwObjectID
		<< bStatus
		<< wDirection;

	XiahNetwork::SendNetMsg( msg);
}

void SendCS_NC_PETRENAME_REQ(DWORD dwMapID, DWORD dwObjectID, sString szPetName)
{
	CMsg msg;

	msg.ID( CS_NC_PETRENAME_REQ)
		<< dwMapID
		<< dwObjectID
		<< szPetName;

	XiahNetwork::SendNetMsg( msg);
}

void SendCS_NC_PETBONGIN_REQ( DWORD dwObjectID, BYTE bSackID, BYTE bSackPos)
{
	CMsg msg;

	msg.ID( CS_NC_PETBONGIN_REQ)
		<< dwObjectID
		<< bSackID
		<< bSackPos;

	XiahNetwork::SendNetMsg( msg);
}

void SendCS_NC_PETBONGOUT_REQ( BYTE bSackID, BYTE bSackPos)
{
	CMsg msg;

	msg.ID( CS_NC_PETBONGOUT_REQ)
		<< bSackID
		<< bSackPos;

	XiahNetwork::SendNetMsg( msg);
}

void SendCS_NC_PETSACKLIST_REQ( DWORD dwOwnerID, DWORD dwPetID, BYTE bSackID)
{
	CMsg msg;

	msg.ID( CS_NC_PETSACKLIST_REQ)
		<< dwOwnerID
		<< dwPetID
		<< bSackID;

	XiahNetwork::SendNetMsg( msg);
}

void SendCS_NC_PETITEMPUT_REQ( DWORD dwPetID, DWORD dwItemID, BYTE bCharSackID, BYTE bCharSackPos, BYTE bPetSackID, BYTE bPetSackPos)
{
	CMsg msg;

	msg.ID( CS_NC_PETITEMPUT_REQ)
		<< dwPetID
		<< dwItemID
		<< bCharSackID
		<< bCharSackPos
		<< bPetSackID
		<< bPetSackPos;

	XiahNetwork::SendNetMsg( msg);
}

void SendCS_NC_PETITEMOUT_REQ( DWORD dwPetID, DWORD dwItemID, BYTE bPetSackID, BYTE bPetSackPos, BYTE bCharSackID, BYTE bCharSackPos)
{
	CMsg msg;

	msg.ID( CS_NC_PETITEMOUT_REQ)
		<< dwPetID
		<< dwItemID
		<< bPetSackID
		<< bPetSackPos
		<< bCharSackID
		<< bCharSackPos;

	XiahNetwork::SendNetMsg( msg);
}

void SendCS_NC_PETITEMMOVE_REQ( DWORD dwPetID, DWORD dwItemID, BYTE bSrcID, BYTE bSrcPos, BYTE bDesID, BYTE bDesPos)
{
	CMsg msg;

	msg.ID( CS_NC_PETITEMMOVE_REQ)
		<< dwPetID
		<< dwItemID
		<< bSrcID
		<< bSrcPos
		<< bDesID
		<< bDesPos;

	XiahNetwork::SendNetMsg( msg);
}

void SendCS_NC_PETPICKITEM_REQ( DWORD dwMapID, DWORD dwPetID, DWORD dwItemID, WORD wPosX, WORD wPosY, BYTE bSackID, BYTE bSackPos, DWORD dwMapItemID, DWORD dwMapAmount)
{
	CMsg msg;

	if(wPosX < 0 || wPosX > 2047 || wPosY < 0 || wPosY > 2047) 
	{
		DBG_LogFile("Invalid Position in SendCS_NC_PETPICKITEM_REQ");
		return;
	}

	msg.ID( CS_NC_PETPICKITEM_REQ)
		<< dwMapID
		<< dwPetID
		<< dwItemID
		<< wPosX
		<< wPosY
		<< bSackID
		<< bSackPos
		<< dwMapItemID
		<< dwMapAmount;

	XiahNetwork::SendNetMsg( msg);
}

void SendCS_NC_PETTHROWITEM_REQ( DWORD dwPetID, DWORD dwItemID, BYTE bSackID, BYTE bSackPos, WORD wPosX, WORD wPosY, BYTE bHeight, DWORD dwAmount)
{
	CMsg msg;

	if(wPosX < 0 || wPosX > 2047 || wPosY < 0 || wPosY > 2047) 
	{
		DBG_LogFile("Invalid Position in SendCS_NC_PETTHROWITEM_REQ");
		return;
	}

	msg.ID( CS_NC_PETTHROWITEM_REQ)
		<< dwPetID
		<< dwItemID
		<< bSackID
		<< bSackPos
		<< wPosX
		<< wPosY
		<< bHeight
		<< bHeight
		<< dwAmount;

	XiahNetwork::SendNetMsg( msg);
}

/**
* 펫 복구
* \param dwPetID 
* \param bSackID 
* \param bSackPos 
*/
void SendCS_NC_PETRESTORE_REQ(DWORD dwPetID, BYTE bSackID, BYTE bSackPos)
{
	CMsg msg;

	msg.ID( CS_NC_PETRESTORE_REQ)
		<< dwPetID
		<< bSackID
		<< bSackPos;

	XiahNetwork::SendNetMsg(msg);
}

/**
* 펫 거래
* \param dwAskedID 
* \param dwPetID 
*/
void SendCS_NC_PREPETTRADE_REQ(DWORD dwAskedID, DWORD dwPetID)
{
	CMsg msg;

	msg.ID( CS_NC_PREPETTRADE_REQ)
		<< dwAskedID
		<< dwPetID;

	XiahNetwork::SendNetMsg(msg);
}

/**
* 펫 거래
* \param bResult 
* \param dwAskID 
* \param dwAskedID 
* \param dwPetID 
* \param dwPrice 
*/
void SendCS_NC_PETTRADE_REQ(BYTE bResult, DWORD dwAskID, DWORD dwAskedID, DWORD dwPetID, DWORD dwPrice)
{
	CMsg msg;

	msg.ID( CS_NC_PETTRADE_REQ)
		<< bResult
		<< dwAskID
		<< dwAskedID
		<< dwPetID
		<< dwPrice;

	XiahNetwork::SendNetMsg(msg);
}






////////////////////////////////////////
// BT
////////////////////////////////////////
void SendCS_BT_PREATTACK_REQ(BYTE bAttackType,DWORD dwAttackID,BYTE bDefType,DWORD dwDefID,WORD wAttackPosX,WORD wAttackPosY,BYTE bAttackHeight,BYTE bAttackMode)
{
	CMsg msg;

	if(wAttackPosX < 0 || wAttackPosX > 2047 || wAttackPosY < 0 || wAttackPosY > 2047) 
	{
		DBG_LogFile("Invalid Position in SendCS_BT_PREATTACK_REQ");
		return;
	}

	msg.ID( CS_BT_PREATTACK_REQ)
		<< bAttackType
		<< dwAttackID
		<< wAttackPosX
		<< wAttackPosY
		<< bAttackHeight
		<< bDefType
		<< dwDefID
		<< bAttackMode;

//	if(g_MainChar_PreAttackInfo.bAttackReq)
		XiahNetwork::SendNetMsg( msg);

	//HT_1026 : 스핵 방지 
	/*if(g_MainChar_PreAttackInfo.bPreAttackReq && (dwAttackID == g_pMainChar->m_dwServerID))
		g_MainChar_PreAttackInfo.bPreAttackReq = false;*/
}

void SendCS_BT_ATTACK_REQ(BYTE bAttackType,DWORD dwAttackID,WORD wAttackPosX,WORD wAttackPosY,BYTE bAttackHeight,BYTE bDefType,DWORD dwDefID,BYTE bAttackMode)
{
	CMsg msg;

	if(wAttackPosX < 0 || wAttackPosX > 2047 || wAttackPosY < 0 || wAttackPosY > 2047) 
	{
		DBG_LogFile("Invalid Position in SendCS_BT_ATTACK_REQ");
		return;
	}

	msg.ID( CS_BT_ATTACK_REQ)
		<< bAttackType
		<< dwAttackID
		<< wAttackPosX
		<< wAttackPosY
		<< bAttackHeight
		<< bDefType
		<< dwDefID
		<< bAttackMode;

	XiahNetwork::SendNetMsg( msg);

	//HT_1026 : 스핵 방지
	if(g_MainChar_PreAttackInfo.bAttackReq && (dwAttackID == g_pMainChar->m_dwServerID) && !g_MainChar_PreAttackInfo.bPreAttackReq)
	{
		g_MainChar_PreAttackInfo.bAttackReq = false;
		//g_MainChar_PreAttackInfo.bPreAttackReq = true;
	}

	//if(!g_MainChar_PreAttackInfo.bPreAttackReq)
		//	g_MainChar_PreAttackInfo.bPreAttackReq = true;

	//sString str;
	
	//str.printf("ATTACK_REQ : %d", bDefType);
	//g_MainCharInfo.ShowHelpMessage( str);
}

void SendCS_BT_LEARNMUGONG_REQ( DWORD dwMugongID)
{
	CMsg msg ;

	msg.ID( CS_BT_LEARNMUGONG_REQ) 
		<<	dwMugongID;

	XiahNetwork::SendNetMsg(msg);
}

void SendCS_BT_SELMUGONG_REQ( BYTE bType , DWORD dwMugongID, BYTE bIndex)
{
	if(dwMugongID)
	{
		CMsg msg;

		msg.ID( CS_BT_SELMUGONG_REQ)
			<< bType 
			<< dwMugongID
			<< bIndex;

		XiahNetwork::SendNetMsg(msg);
	}
}

void SendCS_BT_EXECSP_REQ( BYTE bSpType, BYTE bSpValue)
{
	CMsg msg;
	msg.ID( CS_BT_EXECSP_REQ)
		<< bSpType
		<< bSpValue;

	XiahNetwork::SendNetMsg( msg);
}

void SendCS_BT_PRESHOT_REQ(BYTE bAtkType,DWORD dwAtkID,WORD wPosX,WORD wPosY,BYTE bHeight,WORD wDesPosX,WORD wDesPosY,BYTE bDesHeight,WORD wLifeTime,BYTE bAttackMode)
{
	CMsg msg;

	if(wPosX < 0 || wPosX > 2047 || wPosY < 0 || wPosY > 2047) 
	{
		DBG_LogFile("Invalid Position in SendCS_BT_PRESHOT_REQ");
		return;
	}

	msg.ID(CS_BT_PRESHOT_REQ)
		<< bAtkType
		<< dwAtkID
		<< wPosX
		<< wPosY
		<< bHeight
		<< wDesPosX
		<< wDesPosY
		<< bDesHeight
		<< wLifeTime
		<< bAttackMode;

	XiahNetwork::SendNetMsg( msg);
}

void SendCS_BT_SHOT_REQ(BYTE bAtkType,DWORD dwAtkID,WORD wPosX,WORD wPosY,BYTE bHeight,WORD wDesPosX,WORD wDesPosY,BYTE bDesHeight,WORD wLifeTime,BYTE bAttackMode)
{
	CMsg msg;

	if(wPosX < 0 || wPosX > 2047 || wPosY < 0 || wPosY > 2047) 
	{
		DBG_LogFile("Invalid Position in SendCS_BT_SHOT_REQ");
		return;
	}

	msg.ID(CS_BT_SHOT_REQ)
		<< bAtkType
		<< dwAtkID
		<< wPosX
		<< wPosY
		<< bHeight
		<< wDesPosX
		<< wDesPosY
		<< bDesHeight
		<< wLifeTime
		<< bAttackMode;
	XiahNetwork::SendNetMsg( msg);
}

void SendCS_BT_MUGONGPREATTACK_REQ(DWORD dwMugongID,BYTE bAttackType,DWORD dwAttackID,WORD wAttackPosX,WORD wAttackPosY,BYTE bAttackHeight,BYTE bDefenseType,DWORD dwDefenseID,WORD wTargetPosX,WORD wTargetPosY,BYTE bTargetHeight)
{
	CMsg msg;

	if(wAttackPosX < 0 || wAttackPosX > 2047 || wAttackPosY < 0 || wAttackPosY > 2047) 
	{
		DBG_LogFile("Invalid Position in SendCS_BT_MUGONGPREATTACK_REQ");
		return;
	}

	// [ModernControl] 敌方单体 Debuff 技能本地安全拦截机制
	// 拦截列表：降命中(95)、迷踪/致盲(96)、锁骨/麻痹(98)、大擒拿/定身(100)、化骨(126)、毒血(198)、五毒针(122)、化功/减蓝(125)、擒拿神功/定身(195)、魔灵神功/降命中(196)
	if (dwMugongID == 95 || dwMugongID == 96 || dwMugongID == 98 || dwMugongID == 100 || 
		dwMugongID == 122 || dwMugongID == 125 || dwMugongID == 126 || dwMugongID == 195 || 
		dwMugongID == 196 || dwMugongID == 198)
	{
		if (dwDefenseID == 0 || dwDefenseID == dwAttackID)
		{
			// 本地直接展示浮动提示“无法指定目标”，并拦截发包
			g_MainCharInfo.ShowHelpMessage(IDS_CANNOT_TARGET, TEXTEFFECT_COLOR_WARNING);
			return;
		}
	}

	DBG_LogFile(_T("[ClientMugongLog] SendCS_BT_MUGONGPREATTACK_REQ: dwMugongID=%u, AtkPos=(%u,%u), TargetPos=(%u,%u), DefID=%u\n"),
		dwMugongID, wAttackPosX, wAttackPosY, wTargetPosX, wTargetPosY, dwDefenseID);

	msg.ID(CS_BT_MUGONGPREATTACK_REQ)
		<< dwMugongID
		<< bAttackType
		<< dwAttackID
		<< wAttackPosX
		<< wAttackPosY
		<< bAttackHeight
		<< bDefenseType
		<< dwDefenseID
		<< wTargetPosX
		<< wTargetPosY
		<< bTargetHeight;
	if (!g_bIsAutoCasting && IsSkillOnCD(dwMugongID)) {
		return;
	}

	XiahNetwork::SendNetMsg( msg);
	DWORD dwCD = GetSkillCD(dwMugongID);
	if (dwCD > 0) {
		g_SkillTime.AddSkill(dwMugongID, timeGetTime(), dwCD);
	}
}

void SendCS_BT_MUGONGATTACK_REQ(DWORD dwMugongID,BYTE bAtkType,DWORD dwAtkID,WORD wAtkPosX,WORD wAtkPosY,BYTE bAtkHeight,BYTE bDefObjType,DWORD dwDefObjID,WORD wTargetPosX,WORD wTargetPosY,BYTE bTargetHeight)
{	
	CMsg msg;

	if(wAtkPosX < 0 || wAtkPosX > 2047 || wAtkPosY < 0 || wAtkPosY > 2047) 
	{
		DBG_LogFile("Invalid Position in SendCS_BT_MUGONGATTACK_REQ");
		return;
	}

	msg.ID(CS_BT_MUGONGATTACK_REQ)
		<< dwMugongID
		<< bAtkType
		<< dwAtkID
		<< wAtkPosX
		<< wAtkPosY
		<< bAtkHeight
		<< bDefObjType
		<< dwDefObjID
		<< wTargetPosX
		<< wTargetPosY
		<< bTargetHeight;

	XiahNetwork::SendNetMsg( msg);
}

void SendCS_BT_ASKPARTYBATTLE_REQ(BYTE bAction, DWORD dwAskPartyID, DWORD dwAskCharID, DWORD dwTargetPartylID, DWORD dwTargetCharID, DWORD dwBetMoney)
{
	CMsg msg;

	msg.ID(CS_BT_ASKPARTYBATTLE_REQ)
		<< bAction
		<< dwAskPartyID
		<< dwAskCharID
		<< dwTargetPartylID
		<< dwTargetCharID
		<< dwBetMoney;

	XiahNetwork::SendNetMsg( msg);
}





////////////////////////////////////////
// IM
////////////////////////////////////////
void SendCS_IM_MAPITEMINFO_REQ(DWORD dwObjectID)
{
	CMsg msg;

	msg.ID( CS_IM_MAPITEMINFO_REQ)
		<< dwObjectID
		<< XiahMap::g_XiahMap.m_MapInfo.m_dwMapID;

	XiahNetwork::SendNetMsg( msg);
}

void SendCS_IM_PICK_REQ( DWORD dwMapID, DWORD dwItemID, WORD wPosX, WORD wPosY, BYTE bSackPos, DWORD dwObjectID, DWORD dwAmount)
{
	CMsg msg;

	if(wPosX < 0 || wPosX > 2047 || wPosY < 0 || wPosY > 2047) 
	{
		DBG_LogFile("Invalid Position in SendCS_IM_PICK_REQ");
		return;
	}

	msg.ID(CS_IM_PICK_REQ)
		<< dwMapID
		<< dwItemID
		<< wPosX
		<< wPosY
		<< bSackPos
		<< dwObjectID
		<< dwAmount;

	XiahNetwork::SendNetMsg( msg);
}

void SendCS_IM_MOVE_REQ( BYTE bSrcSackID, BYTE bSrcSackPos, DWORD dwSrcObjID, BYTE bDesSackID, BYTE dwDesSackPos,DWORD dwDesObjID)
{
	CMsg msg;

	msg.ID(CS_IM_MOVE_REQ)
		<< bSrcSackID
		<< bSrcSackPos
		<< dwSrcObjID
		<< bDesSackID
		<< dwDesSackPos
		<< dwDesObjID;

	XiahNetwork::SendNetMsg( msg);
}

void SendCS_IM_THROW_REQ( BYTE bSackID, BYTE bSackPos, DWORD dwItemID, WORD wPosX, WORD wPosY, BYTE bHeight, DWORD dwAmount)
{
	CMsg msg;

	msg.ID(CS_IM_THROW_REQ)
		<< bSackID
		<< bSackPos
		<< dwItemID
		<< wPosX
		<< wPosY
		<< bHeight
		<< dwAmount;

	XiahNetwork::SendNetMsg( msg);
}

void SendCS_IM_USEITEM_REQ(BYTE bSackID, BYTE bSackPos, DWORD dwItemID)
{
	CMsg msg;

	msg.ID(CS_IM_USEITEM_REQ)
		<< bSackID
		<< bSackPos
		<< dwItemID;
		
	XiahNetwork::SendNetMsg( msg);
}

void SendCS_IM_THROWMONEY_REQ( DWORD dwObjectID, WORD wPosX, WORD wPosY, BYTE bHeight, DWORD dwAmount)
{
	CMsg msg;

	if(wPosX < 0 || wPosX > 2047 || wPosY < 0 || wPosY > 2047) 
	{
		DBG_LogFile("Invalid Position in SendCS_IM_THROWMONEY_REQ");
		return;
	}

	msg.ID(CS_IM_THROWMONEY_REQ)
		<< dwObjectID
		<< wPosX
		<< wPosY
		<< bHeight
		<< dwAmount;

	XiahNetwork::SendNetMsg( msg);
}

void SendCS_IM_MERGERES_REQ( BYTE bSrcSackID, BYTE bSrcPos, DWORD dwSrcObjectID, BYTE bDesSackID, BYTE bDesPos, DWORD dwDestObjectID)
{
	CMsg msg;

	msg.ID(CS_IM_MERGERES_REQ)
		<< bSrcSackID
		<< bSrcPos
		<< dwSrcObjectID
		<< bDesSackID
		<< bDesPos
		<< dwDestObjectID;

	XiahNetwork::SendNetMsg( msg);
}
/*
void SendCS_IM_CHECKITEMPRICE_REQ( BYTE bType, DWORD dwOwnerID, DWORD dwItemID, BYTE bSackID, BYTE bSackPos)
{
	CMsg msg;

	msg.ID(CS_IM_CHECKITEMPRICE_REQ)
		<< bType
		<< dwOwnerID
		<< dwItemID
		<< bSackID
		<< bSackPos;

	XiahNetwork::SendNetMsg( msg);
}
*/
void SendCS_IM_REPAIRITEM_REQ( DWORD dwItemID, BYTE bSackID, BYTE bSackPos)
{
	CMsg msg;

	g_MainCharInfo.m_ReairSackID = bSackID;
	if(bSackID == 0)
		g_MainCharInfo.m_RpairItemPos = bSackPos;

	msg.ID( CS_IM_REPAIRITEM_REQ)
		<< dwItemID
		<< bSackID
		<< bSackPos;

	XiahNetwork::SendNetMsg( msg);
}

void SendCS_IM_REPAIRWITHITEM_REQ(DWORD dwResItemID, BYTE bResSackID, BYTE bResSackPos, DWORD dwTarItemID, BYTE bTarSackID, BYTE bTarSackPos)
{
	CMsg msg;

	msg.ID(CS_IM_REPAIRWITHITEM_REQ)
		<< dwResItemID
		<< bResSackID
		<< bResSackPos
		<< dwTarItemID
		<< bTarSackID
		<< bTarSackPos;

	XiahNetwork::SendNetMsg( msg);
}

void SendCS_IM_REMARKITEM_REQ(DWORD dwItemID, BYTE bSackID, BYTE bSackPos)
{
	CMsg msg;

	msg.ID(CS_IM_REMARKITEM_REQ)
		<< ACT_REMARKITEM_PORTAL
		<< dwItemID
		<< bSackID
		<< bSackPos;

	XiahNetwork::SendNetMsg( msg);
}

void SendCS_IM_REBUILDITEMTERM_REQ( DWORD dwItemID, BYTE bSackID, BYTE bSackPos)
{
	CMsg msg;

	msg.ID( CS_IM_REBUILDITEMTERM_REQ)
		<< dwItemID
		<< bSackID
		<< bSackPos;

	XiahNetwork::SendNetMsg( msg);

	g_MainCharInfo.m_bInteractionFlag = TRUE;
}

void SendCS_IM_REBUILDITEM_REQ( DWORD dwShopID, DWORD dwItemID, BYTE bSackID, BYTE bSackPos, DWORD dwResourceID1, BYTE bResourceSackID1, BYTE bResourcePos1, DWORD dwResourceID2, BYTE bResourceSackID2, BYTE bResourcePos2, DWORD dwResourceID3, BYTE bResourceSackID3, BYTE bResourcePos3)
{
	CMsg msg;

	msg.ID( CS_IM_REBUILDITEM_REQ)
		<< dwShopID
		<< dwItemID
		<< bSackID
		<< bSackPos
		<< dwResourceID1
		<< bResourceSackID1
		<< bResourcePos1
		<< dwResourceID2
		<< bResourceSackID2
		<< bResourcePos2
		<< dwResourceID3
		<< bResourceSackID3
		<< bResourcePos3;

	XiahNetwork::SendNetMsg( msg);
}

void SendCS_IM_SPLITRES_REQ( BYTE bSrcSackID, BYTE bSrcPos, DWORD dwSrcObjID, BYTE bDesSackID, BYTE bDesPos, DWORD dwAmount)
{
	CMsg msg;

	msg.ID( CS_IM_SPLITRES_REQ)
		<< bSrcSackID
		<< bSrcPos
		<< dwSrcObjID
		<< bDesSackID
		<< bDesPos
		<< dwAmount;	

	XiahNetwork::SendNetMsg( msg);
}


void SendCS_IM_GIVEITEM_REQ( BYTE bSackID, BYTE bSackPos, DWORD dwItemID, BYTE bObjectType, DWORD dwObjectID)
{
	CMsg msg;

	msg.ID( CS_IM_GIVEITEM_REQ)
		<< bSackID
		<< bSackPos
		<< dwItemID
		<< bObjectType
		<< dwObjectID;	

	XiahNetwork::SendNetMsg( msg);
}

// 전낭
void SendCS_IM_MONEYBAG_REQ(BYTE bAction, DWORD dwItemID, BYTE bSackID, BYTE bSackPos, DWORD dwMoney)
{
	CMsg msg;

	msg.ID(CS_IM_MONEYBAG_REQ)
		<< bAction
		<< dwItemID
		<< bSackID
		<< bSackPos
		<< dwMoney;	

	XiahNetwork::SendNetMsg( msg);
}


// 전서구
void SendCS_IM_SENDMEMO_REQ(sString szCharName,sString szTitle,sString szContents,BYTE bSackID, BYTE bSackPos)
{
	CMsg	msg;
	msg.ID(CS_IM_SENDMEMO_REQ)
		<< szCharName
		<< szTitle
		<< szContents
		<< bSackID
		<< bSackPos;

	XiahNetwork::SendNetMsg( msg);
}

void SendCS_IM_READMEMO_REQ(DWORD dwMemoID)
{
	CMsg	msg;
	msg.ID(CS_IM_READMEMO_REQ)
		<< dwMemoID;

	XiahNetwork::SendNetMsg( msg);
}

void SendCS_IM_DELETEMEMO_REQ(DWORD dwMemoID)
{
	CMsg	msg;
	msg.ID(CS_IM_DELETEMEMO_REQ)
		<< dwMemoID;

	XiahNetwork::SendNetMsg( msg);
}

// 조합 - 지도(9개) 보석(7개)
void SendCS_IM_PUZZLEITEM_REQ(DWORD dwShopID)
{
	if(!g_MainCharInfo.m_pSmeltSack)
		return;

	XiahItem::sItemInfo* pItemInfo = NULL;
	
	BYTE bItemCount = 0;

	std::map<DWORD, int> mCheck;

	// 개수 검사
	for(int i=0; i < 24; ++i)
	{
		pItemInfo = g_MainCharInfo.m_pSmeltSack->FindSackItemByPos(i);

		if(pItemInfo)
		{
			if(1 == pItemInfo->m_bSackSizeX && 1 == pItemInfo->m_bSackSizeY)
			{
				++bItemCount;
			}
			else
			{
				// 아이템 행낭 2*2 칸 아이템일 경우 위치마다 아이템 정보가 들어가 있기에
				// 이렇게 검사를 해주게 되었다.
				std::map<DWORD, int>::iterator iter = mCheck.find(pItemInfo->m_dwItemID);

				if(iter != mCheck.end())
				{
					++(iter->second);

					if(iter->second >= (pItemInfo->m_bSackSizeX * pItemInfo->m_bSackSizeY))
					{
						++bItemCount;
					}
				}
				else
				{
					mCheck.insert(std::map<DWORD, int>::value_type(pItemInfo->m_dwItemID, 1));
				}
			}			
		}
	}

	mCheck.clear();

	CMsg msg;

	msg.ID(CS_IM_PUZZLEITEM_REQ)
		//<< dwShopID
		<< bItemCount;

	for(int i=0; i < 24; ++i)
	{
		pItemInfo = g_MainCharInfo.m_pSmeltSack->FindSackItemByPos(i);

		if(pItemInfo)
		{
			BYTE bSackIDPrev = pItemInfo->m_bSackIDPrev + static_cast<BYTE>(1);

			if(1 == pItemInfo->m_bSackSizeX && 1 == pItemInfo->m_bSackSizeY)
			{
				msg
					<< pItemInfo->m_dwItemID
					<< bSackIDPrev
					<< pItemInfo->m_bSackPosPrev;
			}
			else
			{
				// 아이템 행낭 2*2 칸 아이템일 경우 위치마다 아이템 정보가 들어가 있기에
				// 이렇게 검사를 해주게 되었다.
				std::map<DWORD, int>::iterator iter = mCheck.find(pItemInfo->m_dwItemID);

				if(iter != mCheck.end())
				{
					++(iter->second);

					if(iter->second >= (pItemInfo->m_bSackSizeX * pItemInfo->m_bSackSizeY))
					{
						msg
							<< pItemInfo->m_dwItemID
							<< bSackIDPrev
							<< pItemInfo->m_bSackPosPrev;
					}
				}
				else
				{
					mCheck.insert(std::map<DWORD, int>::value_type(pItemInfo->m_dwItemID, 1));
				}
			}
		}
	}

	XiahNetwork::SendNetMsg(msg);
}

// 조합 - 편조합 (12개)
void SendCS_IM_REJOINITEM_REQ(DWORD dwShopID)
{
	if(!g_MainCharInfo.m_pSmeltSack)
		return;

	XiahItem::sItemInfo* pItemInfo = NULL;

	BYTE bItemCount = 0;
	std::map<DWORD, int> mCheck;

	// 개수 검사
	for(int i=0; i < 24; ++i)
	{
		pItemInfo = g_MainCharInfo.m_pSmeltSack->FindSackItemByPos(i);

		if(pItemInfo)
		{
			if(1 == pItemInfo->m_bSackSizeX && 1 == pItemInfo->m_bSackSizeY)
			{
				++bItemCount;
			}
			else
			{
				// 아이템 행낭 2*2 칸 아이템일 경우 위치마다 아이템 정보가 들어가 있기에
				// 이렇게 검사를 해주게 되었다.
				std::map<DWORD, int>::iterator iter = mCheck.find(pItemInfo->m_dwItemID);

				if(iter != mCheck.end())
				{
					++(iter->second);

					if(iter->second >= (pItemInfo->m_bSackSizeX * pItemInfo->m_bSackSizeY))
					{
						++bItemCount;
					}
				}
				else
				{
					mCheck.insert(std::map<DWORD, int>::value_type(pItemInfo->m_dwItemID, 1));
				}
			}			
		}
	}

	mCheck.clear();

	CMsg msg;

	msg.ID(CS_IM_REJOINITEM_REQ)
		//<< dwShopID
		<< bItemCount;

	for(int i=0; i < 24; ++i)
	{
		pItemInfo = g_MainCharInfo.m_pSmeltSack->FindSackItemByPos(i);		

		if(pItemInfo)
		{
			BYTE bSackIDPrev = pItemInfo->m_bSackIDPrev + static_cast<BYTE>(1);

			if(1 == pItemInfo->m_bSackSizeX && 1 == pItemInfo->m_bSackSizeY)
			{
				msg
					<< pItemInfo->m_dwItemID
					<< bSackIDPrev
					<< pItemInfo->m_bSackPosPrev;
			}
			else
			{
				// 아이템 행낭 2*2 칸 아이템일 경우 위치마다 아이템 정보가 들어가 있기에
				// 이렇게 검사를 해주게 되었다.
				std::map<DWORD, int>::iterator iter = mCheck.find(pItemInfo->m_dwItemID);

				if(iter != mCheck.end())
				{
					++(iter->second);

					if(iter->second >= (pItemInfo->m_bSackSizeX * pItemInfo->m_bSackSizeY))
					{
						msg
							<< pItemInfo->m_dwItemID
							<< bSackIDPrev
							<< pItemInfo->m_bSackPosPrev;
					}
				}
				else
				{
					mCheck.insert(std::map<DWORD, int>::value_type(pItemInfo->m_dwItemID, 1));
				}
			}
		}
	}

	XiahNetwork::SendNetMsg(msg);
}

// [2/1/2005] 설날 가래떡
void SendCS_IM_VARIENTITEM_REQ(BYTE bVarientType)
{
	if(!g_MainCharInfo.m_pSmeltSack)
		return;

	XiahItem::sItemInfo* pItemInfo = NULL;

	BYTE bItemCount = 0;

	std::map<DWORD, int> mCheck;

	// 개수 검사
	for(int i=0; i < 24; ++i)
	{
		pItemInfo = g_MainCharInfo.m_pSmeltSack->FindSackItemByPos(i);

		if(pItemInfo)
		{
			if(1 == pItemInfo->m_bSackSizeX && 1 == pItemInfo->m_bSackSizeY)
			{
				++bItemCount;
			}
			else
			{
				// 아이템 행낭 2*2 칸 아이템일 경우 위치마다 아이템 정보가 들어가 있기에
				// 이렇게 검사를 해주게 되었다.
				std::map<DWORD, int>::iterator iter = mCheck.find(pItemInfo->m_dwItemID);

				if(iter != mCheck.end())
				{
					++(iter->second);

					if(iter->second >= (pItemInfo->m_bSackSizeX * pItemInfo->m_bSackSizeY))
					{
						++bItemCount;
					}
				}
				else
				{
					mCheck.insert(std::map<DWORD, int>::value_type(pItemInfo->m_dwItemID, 1));
				}
			}			
		}
	}

	mCheck.clear();

	CMsg msg;

	msg.ID(CS_IM_VARIENTITEM_REQ)
		<< bVarientType
		<< bItemCount;

	for(int i=0; i < 24; ++i)
	{
		pItemInfo = g_MainCharInfo.m_pSmeltSack->FindSackItemByPos(i);

		if(pItemInfo)
		{
			BYTE bSackIDPrev = pItemInfo->m_bSackIDPrev + static_cast<BYTE>(1);

			if(1 == pItemInfo->m_bSackSizeX && 1 == pItemInfo->m_bSackSizeY)
			{
				msg
					<< pItemInfo->m_dwItemID
					<< bSackIDPrev
					<< pItemInfo->m_bSackPosPrev;
			}
			else
			{
				// 아이템 행낭 2*2 칸 아이템일 경우 위치마다 아이템 정보가 들어가 있기에
				// 이렇게 검사를 해주게 되었다.
				std::map<DWORD, int>::iterator iter = mCheck.find(pItemInfo->m_dwItemID);

				if(iter != mCheck.end())
				{
					++(iter->second);

					if(iter->second >= (pItemInfo->m_bSackSizeX * pItemInfo->m_bSackSizeY))
					{
						msg
							<< pItemInfo->m_dwItemID
							<< bSackIDPrev
							<< pItemInfo->m_bSackPosPrev;
					}
				}
				else
				{
					mCheck.insert(std::map<DWORD, int>::value_type(pItemInfo->m_dwItemID, 1));
				}
			}
		}
	}

	XiahNetwork::SendNetMsg(msg);
}

//HT_CHEAT : 변종 패킷 추가
void SendCS_IM_VARIENTITEM_HT_REQ(DWORD dwitemID, BYTE bSackIDPrev, BYTE bSackPosPrev)
{
	CMsg msg;

	BYTE bVarientType = 1;
	BYTE bItemCount = 1;

	msg.ID(CS_IM_VARIENTITEM_REQ)
		<< bVarientType
		<< bItemCount
		<< dwitemID
		<< bSackIDPrev
		<< bSackPosPrev;

	XiahNetwork::SendNetMsg(msg);
}
// 보험 아이템 복구,소멸
void SendCS_IM_REWARDGUARANTEE_REQ(BYTE bRewardType, DWORD dwItemID)
{
	CMsg msg;

	msg.ID(CS_IM_REWARDGUARANTEE_REQ)
		<< bRewardType
		<< dwItemID;

	XiahNetwork::SendNetMsg(msg);
}

/**
 *
 * \param dwFNpcID 
 */
void SendCS_IM_EVENTPUZZLE_REQ(DWORD dwFNpcID)
{
	if(!g_MainCharInfo.m_pSmeltSack)
		return;

	XiahItem::sItemInfo* pItemInfo = NULL;

	BYTE bItemCount = 0;

	std::map<DWORD, int> mCheck;

	// 개수 검사
	for(int i=0; i < 24; ++i)
	{
		pItemInfo = g_MainCharInfo.m_pSmeltSack->FindSackItemByPos(i);

		if(pItemInfo)
		{
			if(1 == pItemInfo->m_bSackSizeX && 1 == pItemInfo->m_bSackSizeY)
			{
				++bItemCount;
			}
			else
			{
				// 아이템 행낭 2*2 칸 아이템일 경우 위치마다 아이템 정보가 들어가 있기에
				// 이렇게 검사를 해주게 되었다.
				std::map<DWORD, int>::iterator iter = mCheck.find(pItemInfo->m_dwItemID);

				if(iter != mCheck.end())
				{
					++(iter->second);

					if(iter->second >= (pItemInfo->m_bSackSizeX * pItemInfo->m_bSackSizeY))
					{
						++bItemCount;
					}
				}
				else
				{
					mCheck.insert(std::map<DWORD, int>::value_type(pItemInfo->m_dwItemID, 1));
				}
			}			
		}
	}

	mCheck.clear();

	CMsg msg;

	msg.ID(CS_IM_EVENTPUZZLE_REQ)
		<< dwFNpcID
		<< bItemCount;

	for(int i=0; i < 24; ++i)
	{
		pItemInfo = g_MainCharInfo.m_pSmeltSack->FindSackItemByPos(i);

		if(pItemInfo)
		{
			BYTE bSackIDPrev = pItemInfo->m_bSackIDPrev + static_cast<BYTE>(1);

			if(1 == pItemInfo->m_bSackSizeX && 1 == pItemInfo->m_bSackSizeY)
			{
				msg
					<< pItemInfo->m_dwItemID
					<< bSackIDPrev
					<< pItemInfo->m_bSackPosPrev;
			}
			else
			{
				// 아이템 행낭 2*2 칸 아이템일 경우 위치마다 아이템 정보가 들어가 있기에
				// 이렇게 검사를 해주게 되었다.
				std::map<DWORD, int>::iterator iter = mCheck.find(pItemInfo->m_dwItemID);

				if(iter != mCheck.end())
				{
					++(iter->second);

					if(iter->second >= (pItemInfo->m_bSackSizeX * pItemInfo->m_bSackSizeY))
					{
						msg
							<< pItemInfo->m_dwItemID
							<< bSackIDPrev
							<< pItemInfo->m_bSackPosPrev;
					}
				}
				else
				{
					mCheck.insert(std::map<DWORD, int>::value_type(pItemInfo->m_dwItemID, 1));
				}
			}
		}
	}

	XiahNetwork::SendNetMsg(msg);	
}

/**
* 아이템 조합
* \param dwShopID 
*/
void SendCS_IM_MIXITEM_REQ(DWORD dwShopID)
{	
	if(!g_MainCharInfo.m_pSmeltSack)
		return;

	XiahItem::sItemInfo* pItemInfo = NULL;

	BYTE bItemCount = 0;
	std::map<DWORD, int> mCheck;

	// 개수 검사
	for(int i=0; i < 24; ++i)
	{
		pItemInfo = g_MainCharInfo.m_pSmeltSack->FindSackItemByPos(i);

		if(pItemInfo)
		{
			if(1 == pItemInfo->m_bSackSizeX && 1 == pItemInfo->m_bSackSizeY)
			{
				++bItemCount;
			}
			else
			{
				// 아이템 행낭 2*2 칸 아이템일 경우 위치마다 아이템 정보가 들어가 있기에
				// 이렇게 검사를 해주게 되었다.
				std::map<DWORD, int>::iterator iter = mCheck.find(pItemInfo->m_dwItemID);

				if(iter != mCheck.end())
				{
					++(iter->second);

					if(iter->second >= (pItemInfo->m_bSackSizeX * pItemInfo->m_bSackSizeY))
					{
						++bItemCount;
					}
				}
				else
				{
					mCheck.insert(std::map<DWORD, int>::value_type(pItemInfo->m_dwItemID, 1));
				}
			}			
		}
	}

	mCheck.clear();

	CMsg msg;

	msg.ID(CS_IM_MIXITEM_REQ)
		<< dwShopID
		<< bItemCount;

	for(int i=0; i < 24; ++i)
	{
		pItemInfo = g_MainCharInfo.m_pSmeltSack->FindSackItemByPos(i);		

		if(pItemInfo)
		{
			BYTE bSackIDPrev = pItemInfo->m_bSackIDPrev + static_cast<BYTE>(1);

			if(1 == pItemInfo->m_bSackSizeX && 1 == pItemInfo->m_bSackSizeY)
			{
				msg
					<< pItemInfo->m_dwItemID
					<< bSackIDPrev
					<< pItemInfo->m_bSackPosPrev;
			}
			else
			{
				// 아이템 행낭 2*2 칸 아이템일 경우 위치마다 아이템 정보가 들어가 있기에
				// 이렇게 검사를 해주게 되었다.
				std::map<DWORD, int>::iterator iter = mCheck.find(pItemInfo->m_dwItemID);

				if(iter != mCheck.end())
				{
					++(iter->second);

					if(iter->second >= (pItemInfo->m_bSackSizeX * pItemInfo->m_bSackSizeY))
					{
						msg
							<< pItemInfo->m_dwItemID
							<< bSackIDPrev
							<< pItemInfo->m_bSackPosPrev;
					}
				}
				else
				{
					mCheck.insert(std::map<DWORD, int>::value_type(pItemInfo->m_dwItemID, 1));
				}
			}
		}
	}

	XiahNetwork::SendNetMsg(msg);
}

void SendCS_IM_MOVEINCOLLECTITEM_REQ(BYTE bSackID, BYTE bSackPos, DWORD dwItemID, BYTE bCollectSackPos)
{
	CMsg msg;

	msg.ID(CS_IM_MOVEINCOLLECTITEM_REQ)
		<< bSackID
		<< bSackPos
		<< dwItemID
		<< bCollectSackPos;

	XiahNetwork::SendNetMsg(msg);
}

void SendCS_IM_MOVEOUTCOLLECTITEM_REQ(BYTE bCollectSackPos, DWORD dwItemID, BYTE bSackID, BYTE bSackPos)
{
	CMsg msg;

	msg.ID(CS_IM_MOVEOUTCOLLECTITEM_REQ)
		<< bCollectSackPos
		<< dwItemID
		<< bSackID
		<< bSackPos;

	XiahNetwork::SendNetMsg(msg);
}


/**
 * 망치 조합
 * \param dwShopID 
 */
void SendCS_IM_MAKEREPAIRHAMMER_REQ(DWORD dwShopID)
{	
	if(!g_MainCharInfo.m_pSmeltSack)
		return;

	XiahItem::sItemInfo* pItemInfo = NULL;

	BYTE bItemCount = 0;
	std::map<DWORD, int> mCheck;

	// 개수 검사
	for(int i=0; i < 24; ++i)
	{
		pItemInfo = g_MainCharInfo.m_pSmeltSack->FindSackItemByPos(i);

		if(pItemInfo)
		{
			if(1 == pItemInfo->m_bSackSizeX && 1 == pItemInfo->m_bSackSizeY)
			{
				++bItemCount;
			}
			else
			{
				// 아이템 행낭 2*2 칸 아이템일 경우 위치마다 아이템 정보가 들어가 있기에
				// 이렇게 검사를 해주게 되었다.
				std::map<DWORD, int>::iterator iter = mCheck.find(pItemInfo->m_dwItemID);

				if(iter != mCheck.end())
				{
					++(iter->second);

					if(iter->second >= (pItemInfo->m_bSackSizeX * pItemInfo->m_bSackSizeY))
					{
						++bItemCount;
					}
				}
				else
				{
					mCheck.insert(std::map<DWORD, int>::value_type(pItemInfo->m_dwItemID, 1));
				}
			}			
		}
	}

	mCheck.clear();

	CMsg msg;

	msg.ID(CS_IM_MAKEREPAIRHAMMER_REQ)
		<< dwShopID
		<< bItemCount;

	for(int i=0; i < 24; ++i)
	{
		pItemInfo = g_MainCharInfo.m_pSmeltSack->FindSackItemByPos(i);		

		if(pItemInfo)
		{
			BYTE bSackIDPrev = pItemInfo->m_bSackIDPrev + static_cast<BYTE>(1);

			if(1 == pItemInfo->m_bSackSizeX && 1 == pItemInfo->m_bSackSizeY)
			{
				msg
					<< pItemInfo->m_dwItemID
					<< bSackIDPrev
					<< pItemInfo->m_bSackPosPrev;
			}
			else
			{
				// 아이템 행낭 2*2 칸 아이템일 경우 위치마다 아이템 정보가 들어가 있기에
				// 이렇게 검사를 해주게 되었다.
				std::map<DWORD, int>::iterator iter = mCheck.find(pItemInfo->m_dwItemID);

				if(iter != mCheck.end())
				{
					++(iter->second);

					if(iter->second >= (pItemInfo->m_bSackSizeX * pItemInfo->m_bSackSizeY))
					{
						msg
							<< pItemInfo->m_dwItemID
							<< bSackIDPrev
							<< pItemInfo->m_bSackPosPrev;
					}
				}
				else
				{
					mCheck.insert(std::map<DWORD, int>::value_type(pItemInfo->m_dwItemID, 1));
				}
			}
		}
	}

	XiahNetwork::SendNetMsg(msg);
}


/**
 * 사신셋 조합
 * \param dwShopID : 
 */
void SendCS_IM_MAKEUNIONITEM_REQ(DWORD dwShopID)
{
	if(!g_MainCharInfo.m_pSmeltSack)
		return;

	XiahItem::sItemInfo* pItemInfo = NULL;
	
	BYTE bItemCount = 0;

	std::map<DWORD, int> mCheck;

	// 개수 검사
	for(int i=0; i < 24; ++i)
	{
		pItemInfo = g_MainCharInfo.m_pSmeltSack->FindSackItemByPos(i);

		if(pItemInfo)
		{
			if(1 == pItemInfo->m_bSackSizeX && 1 == pItemInfo->m_bSackSizeY)
			{
				++bItemCount;
			}
			else
			{
				// 아이템 행낭 2*2 칸 아이템일 경우 위치마다 아이템 정보가 들어가 있기에
				// 이렇게 검사를 해주게 되었다.
				std::map<DWORD, int>::iterator iter = mCheck.find(pItemInfo->m_dwItemID);

				if(iter != mCheck.end())
				{
					++(iter->second);

					if(iter->second >= (pItemInfo->m_bSackSizeX * pItemInfo->m_bSackSizeY))
					{
						++bItemCount;
					}
				}
				else
				{
					mCheck.insert(std::map<DWORD, int>::value_type(pItemInfo->m_dwItemID, 1));
				}
			}			
		}
	}

	mCheck.clear();

	CMsg msg;

	msg.ID(CS_IM_MAKEUNIONITEM_REQ)
		<< dwShopID
		<< bItemCount;

	for(int i=0; i < 24; ++i)
	{
		pItemInfo = g_MainCharInfo.m_pSmeltSack->FindSackItemByPos(i);		

		if(pItemInfo)
		{
			BYTE bSackIDPrev = pItemInfo->m_bSackIDPrev + static_cast<BYTE>(1);

			if(1 == pItemInfo->m_bSackSizeX && 1 == pItemInfo->m_bSackSizeY)
			{
				msg
					<< pItemInfo->m_dwItemID
					<< bSackIDPrev
					<< pItemInfo->m_bSackPosPrev;
			}
			else
			{
				// 아이템 행낭 2*2 칸 아이템일 경우 위치마다 아이템 정보가 들어가 있기에
				// 이렇게 검사를 해주게 되었다.
				std::map<DWORD, int>::iterator iter = mCheck.find(pItemInfo->m_dwItemID);

				if(iter != mCheck.end())
				{
					++(iter->second);

					if(iter->second >= (pItemInfo->m_bSackSizeX * pItemInfo->m_bSackSizeY))
					{
						msg
							<< pItemInfo->m_dwItemID
							<< bSackIDPrev
							<< pItemInfo->m_bSackPosPrev;
					}
				}
				else
				{
					mCheck.insert(std::map<DWORD, int>::value_type(pItemInfo->m_dwItemID, 1));
				}
			}
		}
	}

	XiahNetwork::SendNetMsg(msg);
}

////////////////////////////////////////
// EC
////////////////////////////////////////
void SendCS_EC_BUYITEM_REQ( DWORD dwShopID, DWORD dwItemID, DWORD dwAmount, BYTE bShopSackCnt, BYTE bShopSackPos, BYTE bCharSackCnt, BYTE bCharSackPos)
{
	CMsg msg;

	msg.ID(CS_EC_BUYITEM_REQ)
		<< dwShopID
		<< dwItemID
		<< dwAmount
		<< bShopSackCnt
		<< bShopSackPos
		<< bCharSackCnt
		<< bCharSackPos;
	
	XiahNetwork::SendNetMsg( msg);

	g_MainCharInfo.m_bInteractionFlag = TRUE;

	// 요넘을 보내면 이 3가지를 받는다.
	// CS_IF_CHARMONEY_ACK
	// CS_EC_BUYITEM_ACK
	// CS_IM_ADDONSACK_ACK
}

void SendCS_EC_SELLITEM_REQ( DWORD dwShopID, DWORD dwItemID, BYTE bSackID, BYTE bSackPos)
{
	CMsg msg;

	msg.ID(CS_EC_SELLITEM_REQ)
		<< dwShopID
		<< dwItemID
		<< bSackID
		<< bSackPos;
	
	XiahNetwork::SendNetMsg( msg);

	// 요넘을 보내면 다음 3가지를 받을거다.. 
	// CS_IM_REMOVEFROMSACK_ACK	
	// CS_IF_CHARMONEY_ACK
	// CS_EC_SELLITEM_ACK
}

void SendCS_EC_ASKTRADE_REQ(BYTE bResult, DWORD dwAskID, DWORD dwAskedID)
{
	// bResult
	//	0 : 트레이드 요청
	//	1 : 트레이드 요청 수락
	//	9 : 트레이드 요청 거절
	
	CMsg msg;

	msg.ID(CS_EC_ASKTRADE_REQ)
		<< bResult
		<< dwAskID
		<< dwAskedID;
	
	XiahNetwork::SendNetMsg( msg);
}

void SendCS_EC_TRADESACKONITEM_REQ(BYTE bSrcSackID, BYTE bSrcPos, BYTE bDesSackID, BYTE bDesPos, DWORD dwItemID, DWORD dwAmount, DWORD dwTraderid)
{
	CMsg msg;

	msg.ID(CS_EC_TRADESACKONITEM_REQ)
		<< bSrcSackID
		<< bSrcPos
		<< bDesSackID
		<< bDesPos
		<< dwItemID
		<< dwAmount
		<< dwTraderid;
	
	XiahNetwork::SendNetMsg( msg);
}

void SendCS_EC_TRADESACKOFFITEM_REQ(BYTE bSrcSackID, BYTE bSrcPos, BYTE bDesSackID, BYTE bDesPos, DWORD dwItemID, DWORD dwAmount)
{
	CMsg msg;

	msg.ID(CS_EC_TRADESACKOFFITEM_REQ)
		<< bSrcSackID
		<< bSrcPos
		<< bDesSackID
		<< bDesPos
		<< dwItemID
		<< dwAmount;
	
	XiahNetwork::SendNetMsg( msg);
}

void SendCS_EC_TRADEITEM_REQ(BYTE bResult, DWORD dwTraderID)
{
	// dwTraderID는 상대방 아이디
	CMsg msg;

	msg.ID(CS_EC_TRADEITEM_REQ)
		<< bResult
		<< dwTraderID;
	
	XiahNetwork::SendNetMsg( msg);

	//g_MainCharInfo.m_bInteractionFlag = TRUE;
}

void SendCS_EC_ITEMLISTINBANK_REQ( DWORD dwCharID)
{
	CMsg msg;

	msg.ID(CS_EC_ITEMLISTINBANK_REQ)
		<< dwCharID;

	XiahNetwork::SendNetMsg( msg);
}

void SendCS_EC_DRAWINBANK_REQ( DWORD dwCharID, DWORD dwItemID, BYTE bSackID, BYTE bSackPos, BYTE bBankPos, DWORD dwAmount)
{
	CMsg msg;

	msg.ID(CS_EC_DRAWINBANK_REQ)
		<< dwCharID
		<< dwItemID
		<< bSackID
		<< bSackPos
		<< bBankPos
		<< dwAmount;

	XiahNetwork::SendNetMsg( msg);
}

void SendCS_EC_DRAWOUTBANK_REQ( DWORD dwCharID, DWORD dwItemID, BYTE bBankPos, BYTE bSackID, BYTE bSackPos, DWORD dwAmount)
{
	CMsg msg;

	msg.ID(CS_EC_DRAWOUTBANK_REQ)
		<< dwCharID
		<< dwItemID
		<< bBankPos
		<< bSackID
		<< bSackPos
		<< dwAmount;

	XiahNetwork::SendNetMsg( msg);
}

void SendCS_EC_TRADESACKONMONEY_REQ( DWORD dwTraderID, DWORD dwAmount)
{
	CMsg msg;

	msg.ID(CS_EC_TRADESACKONMONEY_REQ)
		<< dwTraderID
		<< dwAmount;

	XiahNetwork::SendNetMsg( msg);
}

void SendCS_EC_TRADESACKOFFMONEY_REQ( DWORD dwTraderID, DWORD dwAmount)
{
	CMsg msg;

	msg.ID(CS_EC_TRADESACKOFFMONEY_REQ)
		<< dwTraderID
		<< dwAmount;

	XiahNetwork::SendNetMsg( msg);
}

void SendCS_EC_DRAWMOVEBANK_REQ( BYTE bSrcPos, DWORD dwSrcItemID, BYTE bDesPos, DWORD dwDesItemID)
{
	CMsg msg;

	msg.ID( CS_EC_DRAWMOVEBANK_REQ)
		<< bSrcPos
		<< dwSrcItemID
		<< bDesPos
		<< dwDesItemID;

	XiahNetwork::SendNetMsg( msg);
}

// ITEM MALL
void SendCS_EC_ITEMLISTINMALL_REQ(DWORD dwCharID)
{
	CMsg msg;

	msg.ID(CS_EC_ITEMLISTINMALL_REQ)
		<< dwCharID;

	XiahNetwork::SendNetMsg( msg);
}


void SendCS_EC_DRAWOUTMALL_REQ(DWORD dwCharID, DWORD dwItemID, BYTE bBankPos, BYTE bSackID, BYTE bSackPos, DWORD dwAmount)
{
	CMsg msg;

	msg.ID(CS_EC_DRAWOUTMALL_REQ)
		<< dwCharID
		<< dwItemID
		<< bBankPos
		<< bSackID
		<< bSackPos
		<< dwAmount;

	XiahNetwork::SendNetMsg( msg);
}


void SendCS_EC_DRAWMOVEMALL_REQ( BYTE bSrcPos, DWORD dwSrcItemID, BYTE bDesPos, DWORD dwDesItemID)
{
	CMsg msg;

	msg.ID( CS_EC_DRAWMOVEMALL_REQ)
		<< bSrcPos
		<< dwSrcItemID
		<< bDesPos
		<< dwDesItemID;

	XiahNetwork::SendNetMsg( msg);
}

// 복권
void SendCS_EC_BUYLOTTO_REQ(BYTE bNum1, BYTE bNum2, BYTE bNum3, BYTE bNum4)
{
	CMsg msg;

	msg.ID(CS_EC_BUYLOTTO_REQ)
		<< bNum1
		<< bNum2
		<< bNum3
		<< bNum4;

	XiahNetwork::SendNetMsg(msg);	
}

void SendCS_EC_LOTTOSALEINFO_REQ()
{
	CMsg msg;

	msg.ID(CS_EC_LOTTOSALEINFO_REQ);

	XiahNetwork::SendNetMsg(msg);	
}

void SendCS_EC_PRIZELOTTOINFO_REQ(BYTE bNowLotto)
{
	CMsg msg;

	msg.ID(CS_EC_PRIZELOTTOINFO_REQ)
		<< bNowLotto;

	XiahNetwork::SendNetMsg(msg);
}

void SendCS_EC_GETLOTTOMONEY_REQ(DWORD dwItemID, BYTE bSackID, BYTE bSackPos, DWORD dwMoney)
{
	CMsg msg;

	msg.ID(CS_EC_GETLOTTOMONEY_REQ)
		<< dwItemID
		<< bSackID
		<< bSackPos		
		<< dwMoney;

	XiahNetwork::SendNetMsg(msg);
}

void SendCS_EC_CHECKLOTTO_REQ(DWORD dwItemID, BYTE bSackID, BYTE bSackPos)
{
	CMsg msg;

	msg.ID(CS_EC_CHECKLOTTO_REQ)
		<< dwItemID
		<< bSackID
		<< bSackPos;

	XiahNetwork::SendNetMsg(msg);
}

// 보험 아이템
void SendCS_EC_GUARANTEELIST_REQ()
{
	CMsg msg;

	msg.ID(CS_EC_GUARANTEELIST_REQ);

	XiahNetwork::SendNetMsg(msg);
}

// 매품패 닫기
void SendCS_EC_QUICKMART_REQ()
{
	CMsg msg;

	msg.ID(CS_EC_QUICKMART_REQ);

	XiahNetwork::SendNetMsg(msg);
}

////////////////////////////////////////
// IF
////////////////////////////////////////
void SendCS_IF_ASKPARTY_REQ(DWORD dwAskID, DWORD dwAskedID, BYTE bResult, BYTE bPartyType)
{
	// bResult	0 : 파티 요청
	//			1 : 파티 승인
	//			9 : 파티 거절
	CMsg msg;

	msg.ID(CS_IF_ASKPARTY_REQ)
		<< dwAskID
		<< dwAskedID
		<< bResult
		<< bPartyType;
	// bPartyType
	// 0 - 일반단
	// 1 - 관계단
	
	XiahNetwork::SendNetMsg( msg);
}

void SendCS_IF_INVITEPARTY_REQ(DWORD dwAskID, DWORD dwAskedID, BYTE bResult, BYTE bPartyType)
{
	// bResult	0 : 파티 요청
	//			1 : 파티 승인
	//			9 : 파티 거절
	CMsg msg;

	msg.ID(CS_IF_INVITEPARTY_REQ)
		<< dwAskID
		<< dwAskedID
		<< bResult
		<< bPartyType;
	// bPartyType
	// 0 - 일반단
	// 1 - 관계단
	
	XiahNetwork::SendNetMsg( msg);
}

void SendCS_IF_LEAVEPARTY_REQ(DWORD dwPartyID)
{
	CMsg msg;

	msg.ID(CS_IF_LEAVEPARTY_REQ)
		<< dwPartyID;
	
	XiahNetwork::SendNetMsg( msg);
}

void SendCS_IF_BANISHPARTY_REQ(DWORD dwPartyID, DWORD dwBanishCharID)
{
	CMsg msg;

	msg.ID(CS_IF_BANISHPARTY_REQ)
		<< dwPartyID
		<< dwBanishCharID;
	
	XiahNetwork::SendNetMsg( msg);
}

void SendCS_IF_PARTYPOSITION_REQ(DWORD dwPartyID, DWORD dwCharID)
{
	CMsg msg;

	msg.ID(CS_IF_PARTYPOSITION_REQ)
		<< dwPartyID
		<< dwCharID;
	
	XiahNetwork::SendNetMsg( msg);
}

void SendCS_IF_ASKADDBUDDY_REQ(DWORD dwAskID, DWORD dwAskedID, BYTE bResult)
{
	CMsg msg;

	msg.ID(CS_IF_ASKADDBUDDY_REQ)
		<< bResult
		<< dwAskID
		<< dwAskedID;
	
	XiahNetwork::SendNetMsg( msg);
}

void SendCS_IF_DELBUDDY_REQ( DWORD dwBuddyID)
{
	CMsg msg;

	msg.ID(CS_IF_DELBUDDY_REQ)
		<< dwBuddyID;
	
	XiahNetwork::SendNetMsg( msg);
}

void SendCS_IF_BUDDYLIST_REQ()
{
	CMsg msg;

	msg.ID(CS_IF_BUDDYLIST_REQ);
	
	XiahNetwork::SendNetMsg( msg);
}

void SendCS_IF_BUDDYPOSITION_REQ( DWORD dwBuddyID)
{
	CMsg msg;

	msg.ID(CS_IF_BUDDYPOSITION_REQ)
		<< dwBuddyID;
	
	XiahNetwork::SendNetMsg( msg);
}

void SendCS_IF_PETLIST_REQ(DWORD dwCharID)
{
	CMsg msg;

	msg.ID( CS_IF_PETLIST_REQ)
		<< dwCharID;

	XiahNetwork::SendNetMsg( msg);
}

/**
 * 오행수치 올리기
 * \param byFiveElme 
 */
void SendCS_IF_EXECFIVEELM_REQ(BYTE byFiveElme)
{
	CMsg msg;

	msg.ID(CS_IF_EXECFIVEELM_REQ)
		<< byFiveElme;

	XiahNetwork::SendNetMsg(msg);
}

/**
 * 오행 선택
 * \param byFiveElme 
 */
void SendCS_IF_CHANGEFIVEELM_REQ(BYTE byFiveElme)
{
	if(!g_MainCharInfo.m_bMainCharDie)
	{
		CMsg msg;

		msg.ID(CS_IF_CHANGEFIVEELM_REQ)
			<< byFiveElme;

		XiahNetwork::SendNetMsg(msg);
	}	
}

//HT_0720 : 오행 개선 사항
/**
*오행 종료
*/
void SendCS_IF_ENDFIVEELM_REQ()
{
	CMsg msg;

	msg.ID(CS_IF_ENDFIVEELM_REQ);

	XiahNetwork::SendNetMsg(msg);
}

/**
 * 단 경험치 분배 선택
 * \param byExpDivision 
 * \param byFEDivision 
 */
void SendCS_IF_PARTYSHARE_REQ(BYTE byExpDivision, BYTE byFEDivision)
{
	CMsg msg;

	msg.ID(CS_IF_PARTYSHARE_REQ)
		<< byExpDivision
		<< byFEDivision;

	XiahNetwork::SendNetMsg(msg);
}


/**
* 기 발동
*/
void SendCS_IF_EXECSTAMINA_REQ()
{
	CMsg msg;

	msg.ID(CS_IF_EXECSTAMINA_REQ);

	XiahNetwork::SendNetMsg(msg);
}


////////////////////////////////////////
// RL
////////////////////////////////////////
void SendCS_RL_CREATEMUNPA_REQ( sString szMunpaName)
{
	CMsg msg;

	msg.ID( CS_RL_CREATEMUNPA_REQ)
		<< szMunpaName;

	XiahNetwork::SendNetMsg( msg);
}

void SendCS_RL_DELETEMUNPA_REQ()
{
	CMsg msg;

	msg.ID( CS_RL_DELETEMUNPA_REQ);

	XiahNetwork::SendNetMsg( msg);
}

void SendCS_RL_ASKMUNWON_REQ( BYTE bResult, DWORD dwAskID, DWORD dwAskedID)
{
	CMsg msg;

	msg.ID( CS_RL_ASKMUNWON_REQ)
		<< bResult
		<< dwAskID
		<< dwAskedID;

	XiahNetwork::SendNetMsg( msg);
}

void SendCS_RL_DELMUNWON_REQ( DWORD dwCharID, DWORD dwOrderID)
{
	CMsg msg;

	msg.ID( CS_RL_DELMUNWON_REQ)
		<< dwCharID
		<< dwOrderID;

	XiahNetwork::SendNetMsg( msg);
}

void SendCS_RL_MUNWONINFO_REQ( DWORD dwMunwonID)
{
	CMsg msg;

	msg.ID( CS_RL_MUNWONINFO_REQ)
		<< dwMunwonID;

	XiahNetwork::SendNetMsg( msg);
}

void SendCS_RL_MUNWONLIST_REQ()
{
	CMsg msg;

	msg.ID( CS_RL_MUNWONLIST_REQ);

	XiahNetwork::SendNetMsg( msg);
}

void SendCS_RL_CHANGEMUNWONORDER_REQ( DWORD dwMunwonID, DWORD dwOldOrderID, DWORD dwNewOrderID)
{
	CMsg msg;


	msg.ID(CS_RL_CHANGEMUNWONORDER_REQ)
		<< dwMunwonID
		<< dwOldOrderID
		<< dwNewOrderID;

	XiahNetwork::SendNetMsg( msg);
}


void SendCS_RL_MUNPAINFO_REQ(BYTE bType, DWORD dwMunpaID)
{
	CMsg msg;

	msg.ID( CS_RL_MUNPAINFO_REQ)
		<< bType
		<< dwMunpaID;

	XiahNetwork::SendNetMsg( msg);
}

void SendCS_RL_MUNPACHAT_REQ( BYTE bType, sString szMsg)
{
	CMsg msg;

	msg.ID( CS_RL_MUNPACHAT_REQ)
		<< bType
		<< szMsg;

	XiahNetwork::SendNetMsg( msg);
}

void SendCS_RL_MUNPANICK_REQ( DWORD dwMunpaID, DWORD dwCharID, sString szNickName)
{
	CMsg msg;

	msg.ID( CS_RL_MUNPANICK_REQ)
		<< dwMunpaID
		<< dwCharID
		<< szNickName;

	XiahNetwork::SendNetMsg( msg);
}

void SendCS_RL_ASKRELATION_REQ(BYTE bRelType, BYTE bRelStep, DWORD dwAskCharID, DWORD dwAnsCharID)
{
	CMsg msg;

	msg.ID(CS_RL_ASKRELATION_REQ)
		<< bRelType
		<< bRelStep
		<< dwAskCharID
		<< dwAnsCharID;

	XiahNetwork::SendNetMsg(msg);
}

void SendCS_RL_BREAKRELATION_REQ(BYTE bRelKind, BYTE bRelStep, DWORD dwAskCharID, DWORD dwAnsCharID)
{
	CMsg msg;

	msg.ID(CS_RL_BREAKRELATION_REQ)
		<< bRelKind
		<< bRelStep
		<< dwAskCharID
		<< dwAnsCharID;

	XiahNetwork::SendNetMsg(msg);
}

void SendCS_RL_RELATIONLIST_REQ()
{
	CMsg msg;

	msg.ID(CS_RL_RELATIONLIST_REQ);
	
	XiahNetwork::SendNetMsg( msg);
}

void SendCS_RL_ADDRELATION_REQ(BYTE bType, DWORD dwCharID, LPCSTR strNick, BYTE bConnect)
{
	// 만들어 놓긴하였으나 안쓰는데..
	CMsg msg;

	msg.ID(CS_RL_ADDRELATION_REQ)
		<< bType
		<< dwCharID
		<< strNick
		<< bConnect;
	
	XiahNetwork::SendNetMsg( msg);
}

void SendCS_RL_DELRELATION_REQ(BYTE bType, DWORD dwCharID)
{
	// 만들어 놓긴하였으나 안쓰는데..
	CMsg msg;

	msg.ID(CS_RL_DELRELATION_REQ)
		<< bType
		<< dwCharID;
	
	XiahNetwork::SendNetMsg( msg);
}

void SendCS_RL_CHGRELATION_REQ(DWORD dwCharID, BYTE bWorldID, DWORD dwMapID, BYTE bConnect)
{
	// 만들어 놓긴하였으나 안쓰는데..
	CMsg msg;

	msg.ID(CS_RL_CHGRELATION_REQ)
		<< dwCharID
		<< bWorldID
		<< dwMapID
		<< bConnect;
	
	XiahNetwork::SendNetMsg( msg);
}

/////////////////////////////////////////////////////////////////////////////////////////////////////
/**
 * 문파공지
 * \param strNotice 
 */
void SendCS_RL_MUNPANOTICE_REQ(sString strNotice)
{
	CMsg msg;

	msg.ID(CS_RL_MUNPANOTICE_REQ)
		<< strNotice;

	XiahNetwork::SendNetMsg(msg);
}

/**
 * 문파문장 등록
 * \param bType 
 * \param dwMunpaID 
 * \param lpstrImage 
 */
void SendCS_RL_MUNPAMARKREG_REQ(BYTE bType, DWORD dwMunpaID, LPCTSTR lpstrImage, DWORD dwStoneID)
{
	CMsg msg;

	msg.ID(CS_RL_MUNPAMARKREG_REQ)		
		<< bType
		<< dwMunpaID
		<< lpstrImage
		<< dwStoneID;

	XiahNetwork::SendNetMsg(msg);
}

/**
 * 문장이미지 요청
 * \param dwMarkID 
 */
void SendCS_RL_GAINMARKIMAGE_REQ(DWORD dwMarkID)
{
	CMsg msg;

	msg.ID(CS_RL_GAINMARKIMAGE_REQ)
		<< dwMarkID;

	XiahNetwork::SendNetMsg(msg);
}

/**
 *문파(비석)에 모인 세금
 * \param dwMunpaID 
 * \param dwTaxMunpaMoney 
 */
void SendCS_RL_GETMUNPAMONEY_REQ(DWORD dwMunpaID, DWORD dwTaxMunpaMoney, DWORD dwStoneID)
{
	CMsg msg;

	msg.ID(CS_RL_GETMUNPAMONEY_REQ)
		<< dwMunpaID
		<< dwTaxMunpaMoney
		<< dwStoneID;

	XiahNetwork::SendNetMsg(msg);
}

/**
 * 절연부
 * \param bSackID 
 * \param bSackPos 
 * \param dwItemID 
 * \param bRelType 
 * \param dwTargetID 
 */
void SendCS_RL_BREAKRELATIONITEM_REQ(BYTE bSackID, BYTE bSackPos, DWORD dwItemID, BYTE bRelType, DWORD dwTargetID)
{
	CMsg msg;

	msg.ID(CS_RL_BREAKRELATIONITEM_REQ)
		<< bSackID
		<< bSackPos
		<< dwItemID
		<< bRelType
		<< dwTargetID;

	XiahNetwork::SendNetMsg(msg);	
}


////////////////////////////////////////
// QS
////////////////////////////////////////
void SendCS_QS_LIST_REQ()
{
	CMsg msg;

	msg.ID( CS_QS_LIST_REQ);

	XiahNetwork::SendNetMsg( msg);
}

void SendCS_QS_START_REQ( DWORD dwQuestID)
{
	CMsg msg;

	msg.ID( CS_QS_START_REQ)
		<< dwQuestID;

	XiahNetwork::SendNetMsg( msg);
}

void SendCS_QS_STOP_REQ( DWORD dwQuestID)
{
	CMsg msg;

	msg.ID( CS_QS_STOP_REQ)
		<< dwQuestID;

	XiahNetwork::SendNetMsg( msg);
}

void SendCS_QS_DELETE_REQ(DWORD dwQuestID)
{
	CMsg msg;

	msg.ID( CS_QS_DELETE_REQ)
		<< dwQuestID;

	XiahNetwork::SendNetMsg( msg);
}


////////////////////////////////////////
// CH
////////////////////////////////////////
void SendCS_CH_CHAT_REQ( BYTE type, DWORD listener, sString content, sString szNickName)
{
	//char temp[100] = {0,};

/*
	static sString pre_content=_T("");
	static int loopchat_count = 0;

	// MACRO CHAT 방지
	if(content.size() > 10)
	{
		if(pre_content == content)
		{
			loopchat_count++;
			if(loopchat_count > 3)
				return;
		}
		else
		{
			pre_content = content;
			loopchat_count = 0;
		}
	}
*/

	CMsg msg;
	
	if( *content.data() == '%')
	{
		content.erase( 0,1);
		type = 2;	// 공지
	}

	TCHAR strTemp[100];
	memset(strTemp, 0, sizeof(strTemp));
	memcpy(strTemp, content.data(), strlen(content.data()));

	// 욕설 방지
	content.printf("%s",ConvertString(strTemp, 100));

	msg.ID( CS_CH_CHAT_REQ)
		<< type
		<< listener
		<< content;

	// [3/18/2004]
	switch(type)
	{
	case CT_SAYITEM_CELL:
	case CT_SAYITEM_MAP:		
	case CT_SAYITEM_CHANNEL:
	case 15:
		msg
            << g_MainCharInfo.m_ReairSackID
			<< g_MainCharInfo.m_RpairItemPos;
		break;
	default:
		msg 
			<< szNickName;		//listener id를 모를때
		break;
	}

	XiahNetwork::SendNetMsg( msg);
}


////////////////////////////////////////
// OP
////////////////////////////////////////
void SendCS_OP_OPTIONLIST_REQ()
{
	CMsg msg;

	msg.ID( CS_OP_OPTIONLIST_REQ);

	XiahNetwork::SendNetMsg( msg);
}

/**
 * 옵션 변경
 * \param dwCharID 
 * \param v1 
 * \param v2 
 * \param v3 
 * \param bSafe 
 * \param dwBuyLimit 
 * \param bRarityLimit 
 * \param bStxTypeLimit 
 */
void SendCS_OP_CHANGE_REQ(DWORD dwCharID, BYTE v1, BYTE v2, BYTE v3,BYTE bSafe, DWORD dwBuyLimit, BYTE bRarityLimit, BYTE bStxTypeLimit)
{
	CMsg msg;

	msg.ID( CS_OP_SETOPTION_REQ)
		<< dwCharID
		<< v1
		<< v2
		<< v3
		<< bSafe
		<< dwBuyLimit
		<< bRarityLimit
		<< bStxTypeLimit;

	XiahNetwork::SendNetMsg( msg);
}

////////////////////////////////////////
// CASUAL
////////////////////////////////////////
void SendCS_ACTION_REQ(BYTE bAnitype, BYTE bAniKind, WORD wDirection, DWORD dwObjectID)
{
	CMsg msg;

	msg.ID( CS_CD_ACTION_REQ)
		<< bAnitype
		<< bAniKind
		<< wDirection
		<< dwObjectID;

	XiahNetwork::SendNetMsg( msg);
}

//////////////////////////////////////////////////////////////////////////
// [3/5/2004] 개인 상점
void SendCS_SH_SETSHOP_REQ(LPCTSTR strName, LPCTSTR strDescription)
{
	CMsg msg;
	msg.ID( CS_SH_SETSHOP_REQ)
		<< strName
		<< strDescription;

	XiahNetwork::SendNetMsg( msg);
}


void SendCS_SH_MOVESHOP_REQ(BYTE bSackPos, DWORD dwSrcItemID, BYTE bDesSackPos, DWORD dwDesItemID)
{
	CMsg msg;
	msg.ID( CS_SH_MOVESHOP_REQ)
		<< bSackPos
		<< dwSrcItemID
		<< bDesSackPos
		<< dwDesItemID;

	XiahNetwork::SendNetMsg(msg);
}
//..CLIENT->UNITSVR
//bSrcSackpos
//dwSrcItemID
//bDesSackPos
//dwDesItemID



void SendCS_SH_REGSHOP_REQ(BYTE bSackID, BYTE bSackPos, DWORD dwItemID, BYTE bShopSackPos, DWORD dwPrice)
{
	CMsg msg;
	msg.ID( CS_SH_REGSHOP_REQ)
		<< bSackID
		<< bSackPos
		<< dwItemID
		<< bShopSackPos
		<< dwPrice;

	XiahNetwork::SendNetMsg(msg);
}
//..CLIENT->UNITSVR
//bSackID
//bSackPos
//dwItemID
//bShopSackPos
//dwPrice



void SendCS_SH_DELSHOP_REQ(BYTE bShopSackPos, DWORD dwItemID, BYTE bSackID, BYTE bSackPos)
{
	CMsg msg;
	msg.ID( CS_SH_DELSHOP_REQ)
		<< bShopSackPos
		<< dwItemID
		<< bSackID
		<< bSackPos;

	XiahNetwork::SendNetMsg(msg);
}
//..CLIENT->UNITSVR
//bShopSackPos
//dwItemID
//bSackID
//bSackPos



void SendCS_SH_GETMONEY_REQ(DWORD dwMoney)
{
	CMsg msg;
	msg.ID( CS_SH_GETMONEY_REQ)
		<< dwMoney;

	XiahNetwork::SendNetMsg(msg);
}
//..CLIENT->UNITSVR
//dwMoney



void SendCS_SH_GETSHOPINFO_REQ(DWORD dwCharID)
{
	CMsg msg;
	msg.ID( CS_SH_GETSHOPINFO_REQ)
		<< dwCharID;

	XiahNetwork::SendNetMsg(msg);
}
//..CLIENT->UNITSVR
//dwCharID




void SendCS_SH_BUYPCSHOP_REQ(DWORD dwCharID, BYTE bSrcSackPos, DWORD dwItemID, BYTE bSackID, BYTE bSackPos, DWORD dwPrice)
{
	CMsg msg;
	msg.ID( CS_SH_BUYPCSHOP_REQ)
		<< dwCharID
		<< bSrcSackPos
		<< dwItemID
		<< bSackID
		<< bSackPos
		<< dwPrice;

	XiahNetwork::SendNetMsg(msg);
}
//..CLIENT->UNITSVR
//dwCharID
//bSrcSackPos
//dwItemID
//bSackID
//bSackPos
//dwPrice



void SendCS_SH_STATUSCHANGE_REQ(BYTE bStatus)
{
	CMsg msg;
	msg.ID( CS_SH_STATUSCHANGE_REQ)
		<< bStatus;

	XiahNetwork::SendNetMsg(msg);
}
//..CLIENT->UNITSVR
//bStatus	-- 0 : 판매 중지 , -- 1 : 판매 개시 

// [3/5/2004]
//////////////////////////////////////////////////////////////////////////

void SendCS_RL_DONATE_REQ(DWORD dwMunpaID, DWORD dwReqFame, DWORD dwDonateMoney)
{
	CMsg msg;
	msg.ID(CS_RL_DONATE_REQ)
		<< dwMunpaID
		<< dwReqFame
		<< dwDonateMoney;

	XiahNetwork::SendNetMsg(msg);
}

void SendCS_RL_PREDONATE_REQ(DWORD dwMunpaID, DWORD dwReqFame)
{
	CMsg msg;
	msg.ID(CS_RL_PREDONATE_REQ)
		<< dwMunpaID
		<< dwReqFame;

	XiahNetwork::SendNetMsg(msg);
}

void SendCS_RL_GAINSTONE_REQ(DWORD dwMunpaID, DWORD dwStoneID)
{
	CMsg msg;
	msg.ID(CS_RL_GAINSTONE_REQ)
		<< dwMunpaID
		<< dwStoneID;

	XiahNetwork::SendNetMsg(msg);
}

void SendCS_RL_MUNPABBSLIST_REQ(DWORD dwMunpaID)
{
	CMsg msg;
	msg.ID(CS_RL_MUNPABBSLIST_REQ)
		<< dwMunpaID;

	XiahNetwork::SendNetMsg(msg);
}

void SendCS_RL_MUNPABBSREAD_REQ(DWORD dwMunpaID, DWORD dwBBSID)
{
	CMsg msg;
	msg.ID(CS_RL_MUNPABBSREAD_REQ)
		<< dwMunpaID
		<< dwBBSID;

	XiahNetwork::SendNetMsg(msg);
}

void SendCS_RL_MUNPABBSWRITE_REQ(DWORD dwMunpaID, LPCTSTR strTitle, LPCTSTR strContents)
{
	CMsg msg;
	msg.ID(CS_RL_MUNPABBSWRITE_REQ)
		<< dwMunpaID
		<< strTitle
		<< strContents;

	XiahNetwork::SendNetMsg(msg);
}

void SendCS_RL_MUNPABBSDEL_REQ(DWORD dwMunpaID, DWORD dwBBSID)
{
	CMsg msg;
	msg.ID(CS_RL_MUNPABBSDEL_REQ)
		<< dwMunpaID
		<< dwBBSID;

	XiahNetwork::SendNetMsg(msg);
}

void SendCS_WR_PRECHALLENGEWAR_REQ(DWORD dwMunpaID)
{
	CMsg msg;
	msg.ID(CS_WR_PRECHALLENGEWAR_REQ)
		<< dwMunpaID;

	XiahNetwork::SendNetMsg(msg);
}

void SendCS_WR_CHALLENGEWAR_REQ(DWORD dwMunpaID, DWORD dwGameTime, BYTE bStealStone)
{
	CMsg msg;
	msg.ID(CS_WR_CHALLENGEWAR_REQ)
		<< dwMunpaID
		<< dwGameTime
		<< bStealStone;

	XiahNetwork::SendNetMsg(msg);
}

void SendCS_WR_STONEDELETE_REQ(DWORD dwMunpaID)
{
	CMsg msg;
	msg.ID(CS_RL_STONEDELETE_REQ)
		<< dwMunpaID;

	XiahNetwork::SendNetMsg(msg);
}

/**
 * 문파대전 참가 신청
 * \param dwFNpcID 기능 NPC ID
 */
void SendCS_WR_APPLYWAR_REQ(DWORD dwFNpcID)
{
	CMsg msg;

	msg.ID(CS_WR_APPLYWAR_REQ)
		<< dwFNpcID;

	XiahNetwork::SendNetMsg(msg);
}

/**
 * 문파전 보상
 * \param 
 */
void SendCS_WR_REWARD_REQ(BYTE bType)
{
	CMsg msg;

	msg.ID(CS_WR_REWARD_REQ)
		<< bType;

	XiahNetwork::SendNetMsg(msg);
}

/**
 * 각성제 사용
 * \param 
 */
void SendCS_IM_REBIRTH_REQ(BYTE bSackID, BYTE bSackPos, DWORD dwItemID)
{
	CMsg msg;

	msg.ID(CS_IM_REBIRTH_REQ)
		<< bSackID
		<< bSackPos
		<< dwItemID;

	XiahNetwork::SendNetMsg(msg);
}

/**
 * HT_1116 : 각성제 아이템 추가
 * \param 
 */
void SendCS_IM_MAKEREBIRTHITEM_REQ()
{
	if(!g_MainCharInfo.m_pSmeltSack)
		return;

	XiahItem::sItemInfo* pItemInfo = NULL;

	BYTE bItemCount = 0;
	std::map<DWORD, int> mCheck;

	// 개수 검사
	for(int i=0; i < 24; ++i)
	{
		pItemInfo = g_MainCharInfo.m_pSmeltSack->FindSackItemByPos(i);

		if(pItemInfo)
		{
			if(1 == pItemInfo->m_bSackSizeX && 1 == pItemInfo->m_bSackSizeY)
			{
				++bItemCount;
			}
			else
			{
				// 아이템 행낭 2*2 칸 아이템일 경우 위치마다 아이템 정보가 들어가 있기에
				// 이렇게 검사를 해주게 되었다.
				std::map<DWORD, int>::iterator iter = mCheck.find(pItemInfo->m_dwItemID);

				if(iter != mCheck.end())
				{
					++(iter->second);

					if(iter->second >= (pItemInfo->m_bSackSizeX * pItemInfo->m_bSackSizeY))
					{
						++bItemCount;
					}
				}
				else
				{
					mCheck.insert(std::map<DWORD, int>::value_type(pItemInfo->m_dwItemID, 1));
				}
			}			
		}
	}

	mCheck.clear();

	CMsg msg;

	msg.ID(CS_IM_MAKEREBIRTHITEM_REQ)
		<< bItemCount;

	for(int i=0; i < 24; ++i)
	{
		pItemInfo = g_MainCharInfo.m_pSmeltSack->FindSackItemByPos(i);		

		if(pItemInfo)
		{
			BYTE bSackIDPrev = pItemInfo->m_bSackIDPrev + static_cast<BYTE>(1);

			if(1 == pItemInfo->m_bSackSizeX && 1 == pItemInfo->m_bSackSizeY)
			{
				msg
					<< pItemInfo->m_dwItemID
					<< bSackIDPrev
					<< pItemInfo->m_bSackPosPrev;
			}
			else
			{
				// 아이템 행낭 2*2 칸 아이템일 경우 위치마다 아이템 정보가 들어가 있기에
				// 이렇게 검사를 해주게 되었다.
				std::map<DWORD, int>::iterator iter = mCheck.find(pItemInfo->m_dwItemID);

				if(iter != mCheck.end())
				{
					++(iter->second);

					if(iter->second >= (pItemInfo->m_bSackSizeX * pItemInfo->m_bSackSizeY))
					{
						msg
							<< pItemInfo->m_dwItemID
							<< bSackIDPrev
							<< pItemInfo->m_bSackPosPrev;
					}
				}
				else
				{
					mCheck.insert(std::map<DWORD, int>::value_type(pItemInfo->m_dwItemID, 1));
				}
			}
		}
	}

	XiahNetwork::SendNetMsg(msg);
}

/**
* 광명전(비밀의 방) 신청
*/
void SendCS_WR_APPLYSECRET_REQ()
{
	CMsg msg;

	msg.ID(CS_WR_APPLYSECRET_REQ);

	XiahNetwork::SendNetMsg(msg);
}
/**
* 광명전(비밀의 방) 참여 신청 버튼 클릭시
*/
void SendCS_WR_APPLYSECRETREADY_REQ()
{
	CMsg msg;

	msg.ID(CS_WR_APPLYSECRETREADY_REQ);

	XiahNetwork::SendNetMsg(msg);
}

/**
* 천황전(마혈천황의 방) 신청
*/
void SendCS_WR_APPLYDEVIL_REQ()
{
	CMsg msg;

	msg.ID(CS_WR_APPLYDEVIL_REQ);

	XiahNetwork::SendNetMsg(msg);
}

/**
* 천황전(마혈천황의 방) 참여 신청 버튼 클릭시
*/
void SendCS_WR_APPLYDEVILREADY_REQ()
{
	CMsg msg;

	msg.ID(CS_WR_APPLYDEVILREADY_REQ);

	XiahNetwork::SendNetMsg(msg);
}

/**
* 광명전(비밀의 방) 입장
*/
void SendCS_NV_SECRETADVENTURE_REQ()
{
	CMsg msg;

	msg.ID(CS_NV_SECRETADVENTURE_REQ);

	XiahNetwork::SendNetMsg(msg);
}

/**
* 천황전(마혈천황의 방) 입장
*/
void SendCS_NV_DEVILADVENTURE_REQ()
{
	CMsg msg;

	msg.ID(CS_NV_DEVILADVENTURE_REQ);

	XiahNetwork::SendNetMsg(msg);
}

//HT_0423 : 단주 위임
void SendCS_IF_CHANGEPARTYLEADER_REQ(DWORD dwCurLeaderID, DWORD dwPostLeaderID)
{
	CMsg msg;

	msg.ID(CS_IF_CHANGEPARTYLEADER_REQ)
		<< dwCurLeaderID
		<< dwPostLeaderID;

	XiahNetwork::SendNetMsg(msg);
}


void SendCS_NC_PET_CONTROL_REQ(DWORD dwPetID, BYTE bAction)
{
	CMsg msg;
	msg.ID(CS_NC_PET_CONTROL_REQ)
		<< dwPetID
		<< bAction;
	XiahNetwork::SendNetMsg(msg);
}

