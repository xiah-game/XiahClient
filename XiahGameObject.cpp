#include "precompile.h"
#include "XiahGameObject.h"
#include "XiahObjectType.h"
#include "XiahMap.h"
#include "XiahGame_Handler_Sender.h"
#include "AppData.h"

#include "XiahGameMain.h"
#include "CharacterInfo.h"
#include "InterfaceDefine.h"
#include "XiahEnvInfo.h"
#include "functionalnpcinfo.h"

#include <assert.h>

#define GRAVITY_PER_FRAME	 1.0f

// test code
//#include "XiahGame_Main.h"

CXiah3DObject::CXiah3DObject()
{
	m_Angle = 0;
	m_LocalAngle = 0;
	m_nBasicType = XiahObject::eXOT_3DObject;
	m_LastNavigationTime = 0;
	m_CollisionEnable = FALSE;
	m_bTargetMove = FALSE;
	m_bCollide = FALSE;
	m_bRide = FALSE;
	m_nTargetMoveType	= 0;
	m_fTargetMoveSpeed = 0;
}

CXiah3DObject::~CXiah3DObject()
{
}

//YS_0728 : BUGFIX
void CXiah3DObject::InitClass()
{
	CXiahObject_Basic::InitClass();

	m_Angle					= 0;
	m_LocalAngle			= 0;
	m_nBasicType			= XiahObject::eXOT_3DObject;
	m_LastNavigationTime	= 0;
	m_CollisionEnable		= FALSE;
	m_bTargetMove			= FALSE;
	m_bCollide				= FALSE;
	m_bRide					= FALSE;
	m_nTargetMoveType		= 0;
	m_fTargetMoveSpeed		= 0;

	//YS_TEMP
	m_TargetObjectID		= 0;
	m_TargetObjectType		= 0;
	m_TargetStartTime		= 0;
	m_TargetLifeTime		= 0;
	m_bShowObjectName		= FALSE;
	m_fColHeight			= 0;
	m_bGravityEnable		= FALSE;

	m_Position				= Vector3(0,0,0);	
	m_SyncPosition			= Vector3(0,0,0);	
	m_TargetPosition		= Vector3(0,0,0);
	m_TargetStartPosition	= Vector3(0,0,0);
	m_TargetDir				= Vector3(0,0,0);

	m_rcObjectScreenPos		= sRect( 0, 0, 0, 0);
	m_rcObjectScreenPos2	= sRect( 0, 0, 0, 0);

	m_LocalBound			= BBoxAABB3(Vector3(0,0,0),Vector3(0,0,0));
	m_WorldBound			= BBoxOBB3();
	
	m_ObjectTM.Zero();
	m_tObjectName.Clear();	
}

void CXiah3DObject::DeleteClass()
{
	CXiahObject_Basic::DeleteClass();
}
//..BUGFIX

BOOL CXiah3DObject::SetAngle(WORD angle)
{
	angle %= 360;
	
	float old_angle = m_Angle;
	m_Angle = (90.0f - (float)angle) * 0.01745329f; // _PI / 180.0f;

	m_LocalAngle = ABS( m_Angle - old_angle);

	if( m_LocalAngle > _PI)
		m_LocalAngle = _2_PI - m_LocalAngle;

	return TRUE;
}

BOOL CXiah3DObject::SetPosition(WORD wPosX,WORD wPosY)
{
	m_Position.x = wPosX;
	m_Position.z = -wPosY;
	m_Position.y = GetHeight( m_Position.x, m_Position.z);
	
	return TRUE;
}

//#define XIAH3DOBJECT_ROTATE_SPEED (4 * _PI / 30)
#define XIAH3DOBJECT_ROTATE_SPEED		0.41887902f

BOOL CXiah3DObject::UpdateTM()
{
	// �?화살일때�?ObjectTM�?내가 직접 만져 주겠�?

	if( m_bTargetMove && m_nTargetMoveType == eLBP_Arrow)
	{
		Matrix4x4 InvTM;
		
		InvTM.SetViewMatrix( m_TargetPosition, m_Position, Vector3( 0, 1, 0));

		m_ObjectTM = InvTM.GetInverse();
		m_ObjectTM.t = m_Position;
	}
	else
	{
		m_ObjectTM.SetRotationY( m_Angle);
		m_ObjectTM.t = m_Position;
	}

	// 초당 90도씩

	if( m_LocalAngle != 0)
	{
		if( m_LocalAngle < 0)
		{
			m_LocalAngle += XIAH3DOBJECT_ROTATE_SPEED * g_fFrameScale;
		
			if( m_LocalAngle > 0)
				m_LocalAngle = 0;
		}
		else
		{
			m_LocalAngle -= XIAH3DOBJECT_ROTATE_SPEED * g_fFrameScale;

			if( m_LocalAngle < 0)
				m_LocalAngle = 0;
		}

	}

	return TRUE;
}

BOOL CXiah3DObject::SetTargetPosition(WORD wPosX,WORD wPosY)
{
	m_TargetPosition.y = GetHeight( wPosX, -wPosY);	
	m_TargetPosition.x = wPosX;
	m_TargetPosition.z = -wPosY;

	//WORD CurrentAngle = GetServerAngle( m_Angle);
	WORD TargetAngle  = GetServerAngle( m_TargetPosition.GetAngle( m_Position));

	SetAngle( TargetAngle);

	return TRUE;
}

BOOL CXiah3DObject::SetTargetMove( WORD wPosX, WORD wPosY,int Type,int lifetime)
{
//	SetTargetPosition( wPosX, wPosY);
	m_TargetPosition.x = wPosX;
	m_TargetPosition.y = m_Position.y;
	m_TargetPosition.z = -wPosY;

	m_TargetStartPosition = m_Position;

	m_nTargetMoveType = Type;
	m_bTargetMove = TRUE;

	m_TargetStartTime	= g_dwCurTime;
	m_TargetLifeTime	= lifetime;

	Vector3 dir = m_TargetPosition - m_Position;

	float speed = dir.GetLength() * 33.0f / (float)lifetime;

	m_fTargetMoveSpeed = speed;

	dir.Normalize();
	m_TargetDir = dir;

	return TRUE;
}

float CXiah3DObject::GetHeight(float x,float z)
{
	return XiahGameEngine::Map::g_MapRes.GetHeight( x, z);
}

BOOL CXiah3DObject::GetPosition(WORD &wPosX,WORD &wPosY)
{
	wPosX = m_Position.x;
	wPosY = -m_Position.z;
	return TRUE;
}

BOOL CXiah3DObject::GetAngle(WORD &angle)
{
//	angle = (90.0f - m_Angle) * 180.0f / _PI;
	angle = GetServerAngle( m_Angle);
	return TRUE;
}

float CXiah3DObject::GetDistance(WORD wPosX,WORD wPosY)
{
	Vector3 vDistance( wPosX - m_Position.x, 0, wPosY - (-m_Position.z));

	return vDistance.GetLength();
}

float CXiah3DObject::GetDistance(Vector3 pos)
{
	Vector3 vDis = pos - m_Position;
	vDis.y = 0;
	return vDis.GetLength();
}

Vector3 CXiah3DObject::GetAngle_Vector()
{
	Matrix4x4 rotM;
	rotM.SetRotationY( m_Angle);

	return Vector3( 0, 0, -1) * rotM;
};

float CXiah3DObject::GetDistance_Normal(WORD wPosX,WORD wPosY)
{
	Vector3 vDistance( wPosX - m_Position.x, 0, wPosY - (-m_Position.z));
	Vector3 vDir;

	float fDistance = vDistance.GetLength();

	vDistance.Normalize();

	vDir = GetAngle_Vector();

	return vDir.Dot( vDistance) < 0 ? -fDistance : fDistance;
}

float CXiah3DObject::GetSyncMoveScale(WORD wPosX,WORD wPosY)
{
	Vector3 vDis1( m_Position.x - m_SyncPosition.x, 0,m_Position.z - m_SyncPosition.z);
	Vector3 vDis2( wPosX - m_SyncPosition.x, 0,wPosY - (-m_SyncPosition.z));

	float fLength = vDis1.GetLength();
	
	m_SyncPosition.x = wPosX;
	m_SyncPosition.z = -wPosY;

	if( fLength == 0)
		return 1;

	return vDis2.GetLength() / vDis1.GetLength();
}

BOOL CXiah3DObject::SetAngleTarget(Vector3 position)
{
	Vector3 vStart	= m_Position;
	Vector3 vEnd	= position;

	vStart.y = 0;
	vEnd.y = 0;

	vEnd -= vStart;
	vEnd.Normalize();

	float angle = atan2( vEnd.x, -vEnd.z);
	short server_angle = GetServerAngle( angle);

	return SetAngle( server_angle);
}

WORD CXiah3DObject::GetTargetAngle(Vector3 pos)
{
	Vector3 vStart	= m_Position;
	Vector3 vEnd	= pos;

	vStart.y = 0;
	vEnd.y = 0;

	vEnd -= vStart;
	vEnd.Normalize();

	float angle = atan2( vEnd.x, -vEnd.z);
	short server_angle = GetServerAngle( angle);

	return server_angle;
}

BOOL CXiah3DObject::SetAngleTarget(WORD wPosX,WORD wPosY)
{
	return SetAngleTarget( Vector3( wPosX, 0, -wPosY));
}

BOOL CXiah3DObject::SetAngleTarget(CXiah3DObject* pObject)
{
	return SetAngleTarget( pObject->m_Position);
}


/*************************************************************************************************************
..............................................................................................................
......................SSSS...EEEEEE..PPPPP.....AA....RRRRR.....AA....TTTTTT...OOOO...RRRRR....................
.....................SS..SS..EE......PP..PP...AAAA...RR..RR...AAAA.....TT....OO..OO..RR..RR...................
.....................SS......EE......PP..PP..AA..AA..RR..RR..AA..AA....TT....OO..OO..RR..RR...................
......................SSSS...EEEEEE..PPPPP...AAAAAA..RRRR....AAAAAA....TT....OO..OO..RRRR.....................
.........................SS..EE......PP......AA..AA..RR.RR...AA..AA....TT....OO..OO..RR.RR....................
.....................SS..SS..EE......PP......AA..AA..RR..RR..AA..AA....TT....OO..OO..RR..RR...................
......................SSSS...EEEEEE..PP......AA..AA..RR..RR..AA..AA....TT.....OOOO...RR..RR...................
..............................................................................................................
*************************************************************************************************************/

#ifndef MASTER
static	long xiahobj_count = 0;
#endif

CXiahCharObject::CXiahCharObject() : m_bTradeSell(false), m_bFECur(0), m_bFELevel(0), m_bOrderID(255)
{
#ifndef MASTER
	++xiahobj_count;
	//DBG_Put("XiahOBJ count = %d",xiahobj_count);
#endif

	m_nBasicType = XiahObject::eXOT_CharObject;
	m_bShowObjectName = TRUE;

	m_nCurMotionType = 0;
	m_nNextMotionType = 0;
	m_nCurAniType = 0;
	m_nNextAniType = 0;
	m_nCurAniIndex = 0;
	m_nNextAniIndex = 0;

	m_bRotatable = TRUE;
	m_bMoveable = FALSE;
	m_bAttack = FALSE;
	m_bAttackType = 0;
	m_nChildChar = 0;

	CClassTrigger<CXiahCharObject> *pTrigger;
	ADD_CHARRENDER_TRIGGER( eTE_CharRender_EndAnimation, OnEndAnimation);
	ADD_CHARRENDER_TRIGGER( eTE_CharRender_EndAlphaEffect, OnEndAlphaEffect);
	ADD_CHARRENDER_TRIGGER( eTE_CharRender_TMUpdate, OnTMUpdate);
	ADD_CHARRENDER_TRIGGER( eTE_CharRender_Timer, OnTimer);

	m_pAniType = NULL;

	m_bGravityEnable = TRUE;

	m_pLineParticle = NULL;
	m_bRenderOK = true;

	m_KeepUpMugongList.clear();

	m_bCallRelease = false;

	m_bGlowEnable = FALSE;
	m_GlowColor = 0;
	m_bShowGage = FALSE;
	m_bShowManaGage = FALSE;

	m_TitleEffect.m_bLoaded = FALSE;
	m_TitleEffect.m_nCurrentFrame = 0;
	m_TitleEffect.m_dwLastTime = 0;
	m_nActiveTitleID = -1;

	m_pUpdateTargetDecal = NULL;
//	m_SwordTrace.Init();
	m_fWeaponLength = 0.5f;
	m_fWeaponBackLength = 0.0f;

	m_bGray = false;

	m_pMusuhonEffectPP		= NULL;
	m_pPoksahonEffectPP		= NULL;
	m_pKuymgangrukEffectPP	= NULL;
	m_pYuenoyuengEffectPP	= NULL;
	m_pKyugamsuEffectPP		= NULL;
	m_pWonkisingangEffectPP	= NULL;
	m_pPachunsoEffectPP		= NULL;
	m_pMarulkakEffectPP		= NULL;
	m_pAmhukmuEffectPP		= NULL;
	m_pTalbacinEffectPP		= NULL;
	m_pJukwonkangkiEffectPP	= NULL;
	m_pKumnasuEffectPP		= NULL;
	m_pBantankangkiEffectPP	= NULL;
	m_pOdokchimEffectPP		= NULL;
	m_pDokhyulgongEffectPP	= NULL;
	m_pDoknaegongEffectPP	= NULL;
	m_pSsangdosuEffectPP	= NULL;
	m_pMandokbuljinEffectPP	= NULL;
	m_pDokmuEffectPP		= NULL;

    m_pGyungGongEffectPP	= NULL;

	m_pFEEffectPP			= NULL;

	m_pEventItemEffectPP	= NULL;
	m_pSpiritEffectPP		= NULL;	// �?

	m_pWha_DragonPP			= NULL;
	m_pBing_DragonPP		= NULL;
	m_pDok_DragonPP			= NULL;
	m_pNoi_DragonPP			= NULL;

	//HT_0711 : 진각�?무공
	m_pKuymgangsingongEffectPP	= NULL;		// 흡성 신공 금강신공
	//m_pBunsinsingongEffectPP	= NULL;		// 흡성 신공 분신신공
	m_pWonkisingongEffectPP		= NULL;		// 흡성 신공 원기신공
	m_pKyugamsingongEffectPP	= NULL;		// 흡성 신공 교감신공
	m_pKumnasingongEffectPP		= NULL;		// 흡성 신공 금나신공
	m_pMarulsingongEffectPP		= NULL;		// 흡성 신공 마령신공
	//m_pGwangmasingongEffectPP	= NULL;		// 흡성 신공 광마신공
	m_pDokhyulsingongEffectPP	= NULL;		// 흡성 신공 독혈신공

	m_EffectPPList.clear();

	m_pItemGroundEffectPP = NULL;
	m_bCreateItemGroundEffect = false;

	m_tObjectName.SetParentRect( &m_rcObjectScreenPos);
	m_text2DForChatBox.SetParentRect( &m_rcObjectScreenPos2);

	m_ChatMsg = _T("");
	m_ChatColor = D3DCOLOR_XRGB( 255, 255, 255);
	m_cGageColor = D3DCOLOR_XRGB( 255, 0, 0);
	m_cNameColor = D3DCOLOR_XRGB( 255, 255, 200);

	m_dwMunpaID = 0;
	m_dwMunpaOrder = 0;
	m_szMunpaName = _T("");
	m_szMunpaNickName = _T("");

	// 공격시의 Sound Effect 관�?
	m_dwEnemyType = 0; 
	m_dwOwnerID = 0;

	m_dwPartyID = m_dwPartyLeaderID = m_dwEnemyPartyID = m_dwEnemyMunpaID =	m_dwEnemyStoneID = 0;
	m_bWarStatus = 0;

	m_bNowGwangmadokgong = false;
	m_fLocalScaleForGwangmadokgong = 1.0f;
	m_bNowGyuisikdaebub = false;

	m_dwMunpaMarkID	=0;

	// 설승단약
	m_bPotionEndKeepup = 0;

	// �?
	m_bSpirit = 0;
	m_wLevel  = 0;
	m_bRebirth = 0;

	m_pRebirthItemEffectPP = NULL;

	m_pSajangsingongEffectPP = NULL;
	m_pKihubkangkiEffectPP = NULL;
	m_pEunsinsulEffectPP = NULL;
	m_pHojungkangkiEffectPP = NULL;
	m_pSusinkikangEffectPP = NULL;

	m_bGameMasterMark = 0;//HT_1023 : 운영�?마크 추가
}

CXiahCharObject::~CXiahCharObject()
{
	if( !m_bCallRelease )
		Release();

	TRIGGER_LIST::iterator it;
	for(it = m_TriggerList.begin(); it != m_TriggerList.end(); ++it)
	{
		CClassTrigger<CXiahCharObject>* pTrigger = *it;

		if(pTrigger)
			delete pTrigger;
	}
	m_TriggerList.clear();


#ifndef MASTER
	--xiahobj_count;
	//DBG_Put("XiahOBJ count = %d",xiahobj_count);
#endif
}

//YS_0728 : BUGFIX
void CXiahCharObject::DeleteClass()
{	
	if ( !m_bCallRelease )
		Release();

	CXiah3DObject::DeleteClass ();	
}

void CXiahCharObject::InitClass()
{
	CXiah3DObject::InitClass();

	m_nBasicType		= XiahObject::eXOT_CharObject;

	m_bShowObjectName	= TRUE;

	m_nCurMotionType	= 0;
	m_nNextMotionType	= 0;
	m_nCurAniType		= 0;
	m_nNextAniType		= 0;
	m_nCurAniIndex		= 0;
	m_nNextAniIndex		= 0;

	m_bRotatable		= TRUE;
	m_bMoveable			= FALSE;
	m_bAttack			= FALSE;
	m_bAttackType		= 0;
	m_nChildChar		= 0;

	m_pAniType			= NULL;
	m_bGravityEnable	= TRUE;
	m_pLineParticle		= NULL;
	m_bRenderOK			= true;
	m_bCallRelease		= false;
	m_bGlowEnable		= FALSE;
	m_GlowColor			= 0;
	m_bShowGage			= FALSE;
	m_bShowManaGage		= FALSE;

	m_pUpdateTargetDecal = NULL;
	m_fWeaponLength		= 0.5f;
	m_bGray				= false;

	// 무공 지�?이펙�?
	// 검�?
	m_pMusuhonEffectPP		= NULL;
	m_pPoksahonEffectPP		= NULL;
	m_pKuymgangrukEffectPP	= NULL;
	// 연랑
	m_pYuenoyuengEffectPP	= NULL;
	m_pKyugamsuEffectPP		= NULL;
	m_pWonkisingangEffectPP	= NULL;
	// 무투.
	m_pPachunsoEffectPP		= NULL;
	m_pMarulkakEffectPP		= NULL;
	m_pAmhukmuEffectPP		= NULL;
	m_pTalbacinEffectPP		= NULL;
	m_pJukwonkangkiEffectPP	= NULL;
	m_pKumnasuEffectPP		= NULL;
	m_pBantankangkiEffectPP	= NULL;

	//HT_0530 각성 무공 
	m_pWha_DragonPP			= NULL;
	m_pBing_DragonPP		= NULL;
	m_pDok_DragonPP			= NULL;
	m_pNoi_DragonPP			= NULL;

	//HT_0711 : 진각�?무공
	m_pKuymgangsingongEffectPP	= NULL;		// 흡성 신공 금강신공
//	m_pBunsinsingongEffectPP	= NULL;		// 흡성 신공 분신신공
	m_pWonkisingongEffectPP		= NULL;		// 흡성 신공 원기신공
	m_pKyugamsingongEffectPP	= NULL;		// 흡성 신공 교감신공
	m_pKumnasingongEffectPP		= NULL;		// 흡성 신공 금나신공
	m_pMarulsingongEffectPP		= NULL;		// 흡성 신공 마령신공
//	m_pGwangmasingongEffectPP	= NULL;		// 흡성 신공 광마신공
	m_pDokhyulsingongEffectPP	= NULL;		// 흡성 신공 독혈신공

	m_rcObjectScreenPos		= sRect(0,0,0,0);
	m_rcObjectScreenPos2	= sRect(0,0,0,0);

	m_EffectPPList.clear();

	m_pItemGroundEffectPP		= NULL;
	m_bCreateItemGroundEffect	= false;

	m_tObjectName.SetParentRect( &m_rcObjectScreenPos);
	m_text2DForChatBox.SetParentRect( &m_rcObjectScreenPos2);

	m_ChatMsg		= _T("");
	m_ChatColor		= D3DCOLOR_XRGB( 255, 255, 255);
	m_cGageColor	= D3DCOLOR_XRGB( 255, 0, 0);
	m_cNameColor	= D3DCOLOR_XRGB( 255, 255, 200);

	m_dwMunpaID			= 0;
	m_dwMunpaOrder		= 0;
	m_szMunpaName		= _T("");
	m_szMunpaNickName	= _T("");

	m_dwEnemyType		= 0;
	m_dwOwnerID			= 0;
	m_dwPartyID			= m_dwPartyLeaderID = m_dwEnemyPartyID = 0;

	m_KeepUpMugongList.clear();

	//YS_TEMP			
	m_pLineParticle			= NULL;
	m_fWeaponLength			= 0;
	m_bGlowEnable			= 0;
	m_bShowGage				= 0;	// 에너지 게이지
	m_bShowManaGage			= 0;
	m_bGray					= false;
	m_dwOwnerID				= 0;
	m_bSemiPKStatus			= 0;
	// CG_2005/01/28 : 변종아이템기능추가
	m_bChangeItemSet		= 0;

	m_dwTimeInterval		= 0;
	m_fWeaponLength			= 0;
	
	m_ShotAttackInfo.bDesHeight	=	0;
	m_ShotAttackInfo.dwObjType	=	0;
	m_ShotAttackInfo.dwTargetID	=	0;
	m_ShotAttackInfo.wDesPosX	=	0;
	m_ShotAttackInfo.wDesPosY	=	0;
	m_ShotAttackInfo.wLifeTime	=	0;	

	m_SwordTrace.Release();
	// 2D Text 릴리�?하면 미출�?
	//m_text2DForChatBox.Release();
	m_Shadow.Release();

	ZeroMemory( m_pParentTrigger, sizeof(CTrigger*) * eXCT_Count);

	for(int i = 0; i < LOGICAL_BONE_POS_COUNT; i++)
	{
		m_ChildChar[i].Clear();
		m_ChildChar[i].Init();
	}	

	m_CharRender.Init();

	m_bTradeSell			= false;
	m_strShopName			= _T("");
	m_strShopDescription	= _T("");

	m_dwMunpaMarkID			= 0;

	// 오행 이펙�?
	m_bFECur = m_bFELevel = 0;
	m_bOrderID = 255;

	// 설승단약
	m_bPotionEndKeepup = 0;

	// �?
	m_bSpirit = 0;
	m_wLevel  = 0;
	m_bRebirth = 0;

	m_pRebirthItemEffectPP = NULL;

	m_pSajangsingongEffectPP = NULL;
	m_pKihubkangkiEffectPP = NULL;
	m_pEunsinsulEffectPP = NULL;
	m_pHojungkangkiEffectPP = NULL;
	m_pSusinkikangEffectPP = NULL;
}
//..BUGFIX

BOOL CXiahCharObject::Release()
{
	ClearTitle();

	for(int i = 0; i < LOGICAL_BONE_POS_COUNT; i++)
	{
		if( m_ChildChar[ i].IsValid())
		{
			m_ChildChar[ i].Clear();
		}
	}
	m_nChildChar = 0;

	m_CharRender.Clear();

	m_bGlowEnable = FALSE;
	/////////////////////////////////////////////////////////////////////////////////////////////////////
	/* 위로 이동
	TRIGGER_LIST::iterator it;

	for(it = m_TriggerList.begin(); it != m_TriggerList.end(); it++)
	{
		CClassTrigger<CXiahCharObject>* pTrigger = *it;

		if(pTrigger)
			delete pTrigger;
	}

	m_TriggerList.clear();
	*/
	/////////////////////////////////////////////////////////////////////////////////////////////////////

	if( m_pLineParticle ) delete m_pLineParticle;
	m_pLineParticle = NULL;

	m_SwordTrace.Release();
	m_SwordTrace2.Release();

	XiahMap::g_XiahMap.m_pMapRender->DeleteVisibleMapDecal(&m_Shadow);

	ClearMugongEffect();

	if( m_pEventItemEffectPP )
	{
		g_EffectManager.DeqEffectPackagePair( m_pEventItemEffectPP );

		m_pEventItemEffectPP = NULL;
	}

	if(m_pSpiritEffectPP)
	{
		g_EffectManager.DeqEffectPackagePair( m_pSpiritEffectPP );

		m_pSpiritEffectPP = NULL;
	}

	if( m_pItemGroundEffectPP )	// �?이펙트는 반복 이펙�?이므�?바닥 아이템이 없어지�?이펙트도 같이 삭제.
	{
		g_EffectManager.DeqEffectPackagePair( m_pItemGroundEffectPP );
		m_pItemGroundEffectPP = NULL;
	}

	if(m_pRebirthItemEffectPP)
	{
		g_EffectManager.DeqEffectPackagePair(m_pRebirthItemEffectPP);
		m_pRebirthItemEffectPP = NULL;
	}

	m_KeepUpMugongList.clear();

	m_bCallRelease = true;

	return TRUE;
}

BOOL CXiahCharObject::ClearMugongEffect()
{
	if( m_pMusuhonEffectPP )
	{
		g_EffectManager.DeqEffectPackagePair( m_pMusuhonEffectPP );
		m_pMusuhonEffectPP = NULL;
	}
	if( m_pPoksahonEffectPP )
	{
		g_EffectManager.DeqEffectPackagePair( m_pPoksahonEffectPP );
		m_pPoksahonEffectPP = NULL;
	}
	if( m_pKuymgangrukEffectPP )
	{
		g_EffectManager.DeqEffectPackagePair( m_pKuymgangrukEffectPP );
		m_pKuymgangrukEffectPP = NULL;
	}

	if( m_pYuenoyuengEffectPP )
	{
		g_EffectManager.DeqEffectPackagePair( m_pYuenoyuengEffectPP );
		m_pYuenoyuengEffectPP = NULL;
	}
	if( m_pKyugamsuEffectPP )
	{
		g_EffectManager.DeqEffectPackagePair( m_pKyugamsuEffectPP );
		m_pKyugamsuEffectPP = NULL;
	}
	if( m_pWonkisingangEffectPP )
	{
		g_EffectManager.DeqEffectPackagePair( m_pWonkisingangEffectPP );
		m_pWonkisingangEffectPP = NULL;
	}

	if( m_pPachunsoEffectPP )
	{
		g_EffectManager.DeqEffectPackagePair( m_pPachunsoEffectPP );
		m_pPachunsoEffectPP = NULL;
	}
	if( m_pMarulkakEffectPP )
	{
		g_EffectManager.DeqEffectPackagePair( m_pMarulkakEffectPP );
		m_pMarulkakEffectPP = NULL;
	}
	if( m_pAmhukmuEffectPP )
	{
		g_EffectManager.DeqEffectPackagePair( m_pAmhukmuEffectPP );
		m_pAmhukmuEffectPP = NULL;
	}
	if( m_pTalbacinEffectPP )
	{
		g_EffectManager.DeqEffectPackagePair( m_pTalbacinEffectPP );
		m_pTalbacinEffectPP = NULL;
	}
	if( m_pJukwonkangkiEffectPP )
	{
		g_EffectManager.DeqEffectPackagePair( m_pJukwonkangkiEffectPP );
		m_pJukwonkangkiEffectPP = NULL;
	}
	if( m_pKumnasuEffectPP )
	{
		g_EffectManager.DeqEffectPackagePair( m_pKumnasuEffectPP );
		m_pKumnasuEffectPP = NULL;
	}
	if( m_pBantankangkiEffectPP )
	{
		g_EffectManager.DeqEffectPackagePair( m_pBantankangkiEffectPP );
		m_pBantankangkiEffectPP = NULL;
	}
	if( m_pGyungGongEffectPP )
	{
		g_EffectManager.DeqEffectPackagePair( m_pGyungGongEffectPP );
		m_pGyungGongEffectPP = NULL;
	}
	if( m_pOdokchimEffectPP )
	{
		g_EffectManager.DeqEffectPackagePair( m_pOdokchimEffectPP );
		m_pOdokchimEffectPP = NULL;
	}
	if( m_pDokhyulgongEffectPP )
	{
		g_EffectManager.DeqEffectPackagePair( m_pDokhyulgongEffectPP );
		m_pDokhyulgongEffectPP = NULL;
	}
	if( m_pDoknaegongEffectPP )
	{
		g_EffectManager.DeqEffectPackagePair( m_pDoknaegongEffectPP );
		m_pDoknaegongEffectPP = NULL;
	}
	if( m_pSsangdosuEffectPP )
	{
		g_EffectManager.DeqEffectPackagePair( m_pSsangdosuEffectPP );
		m_pSsangdosuEffectPP = NULL;
	}
	if( m_pMandokbuljinEffectPP )
	{
		g_EffectManager.DeqEffectPackagePair( m_pMandokbuljinEffectPP );
		m_pMandokbuljinEffectPP = NULL;
	}
	if( m_pDokmuEffectPP )
	{
		g_EffectManager.DeqEffectPackagePair( m_pDokmuEffectPP );
		m_pDokmuEffectPP = NULL;
	}

	if(m_pFEEffectPP)
	{
		g_EffectManager.DeqEffectPackagePair( m_pFEEffectPP );
		m_pFEEffectPP = NULL;
	}

	if(m_pWha_DragonPP)
	{
		g_EffectManager.DeqEffectPackagePair( m_pWha_DragonPP );
		m_pWha_DragonPP = NULL;
	}

	if(m_pBing_DragonPP)
	{
		g_EffectManager.DeqEffectPackagePair( m_pBing_DragonPP );
		m_pBing_DragonPP = NULL;
	}

	if(m_pDok_DragonPP)
	{
		g_EffectManager.DeqEffectPackagePair( m_pDok_DragonPP );
		m_pDok_DragonPP = NULL;
	}

	if(m_pNoi_DragonPP)
	{
		g_EffectManager.DeqEffectPackagePair( m_pNoi_DragonPP );
		m_pNoi_DragonPP = NULL;
	}

	if(m_pSajangsingongEffectPP)
	{
		g_EffectManager.DeqEffectPackagePair( m_pSajangsingongEffectPP );
		m_pSajangsingongEffectPP = NULL;
	}
	if(m_pKihubkangkiEffectPP)
	{
		g_EffectManager.DeqEffectPackagePair( m_pKihubkangkiEffectPP );
		m_pKihubkangkiEffectPP = NULL;
	}
	if(m_pEunsinsulEffectPP)
	{
		g_EffectManager.DeqEffectPackagePair( m_pEunsinsulEffectPP );
		m_pEunsinsulEffectPP = NULL;
	}
	if(m_pHojungkangkiEffectPP)
	{
		g_EffectManager.DeqEffectPackagePair( m_pHojungkangkiEffectPP );
		m_pHojungkangkiEffectPP = NULL;
	}

	if(m_pSusinkikangEffectPP)
	{
		g_EffectManager.DeqEffectPackagePair(m_pSusinkikangEffectPP);
		m_pSusinkikangEffectPP = NULL;
	}

	//HT_0711 : 진각�?무공
	if(m_pKuymgangsingongEffectPP)
	{
		g_EffectManager.DeqEffectPackagePair(m_pKuymgangsingongEffectPP);
		m_pKuymgangsingongEffectPP = NULL;
	}

	//if(m_pBunsinsingongEffectPP)
	//{
	//	g_EffectManager.DeqEffectPackagePair(m_pBunsinsingongEffectPP);
	//	m_pBunsinsingongEffectPP = NULL;
	//}

	if(m_pWonkisingongEffectPP)
	{
		g_EffectManager.DeqEffectPackagePair(m_pWonkisingongEffectPP);
		m_pWonkisingongEffectPP = NULL;
	}

	if(m_pKyugamsingongEffectPP)
	{
		g_EffectManager.DeqEffectPackagePair(m_pKyugamsingongEffectPP);
		m_pKyugamsingongEffectPP = NULL;
	}

	if(m_pKumnasingongEffectPP)
	{
		g_EffectManager.DeqEffectPackagePair(m_pKumnasingongEffectPP);
		m_pKumnasingongEffectPP = NULL;
	}

	if(m_pMarulsingongEffectPP)
	{
		g_EffectManager.DeqEffectPackagePair(m_pMarulsingongEffectPP);
		m_pMarulsingongEffectPP = NULL;
	}

	//if(m_pGwangmasingongEffectPP)
	//{
	//	g_EffectManager.DeqEffectPackagePair(m_pGwangmasingongEffectPP);
	//	m_pGwangmasingongEffectPP = NULL;
	//}

	if(m_pDokhyulsingongEffectPP)
	{
		g_EffectManager.DeqEffectPackagePair(m_pDokhyulsingongEffectPP);
		m_pDokhyulsingongEffectPP = NULL;
	}

	// 연결�?이펙트도 같이 지워준�?
	EFFECTPACKAGEPAIRLIST::iterator eppit;
	for(eppit=m_EffectPPList.begin(); eppit!=m_EffectPPList.end(); eppit++)
	{
		_EFFECTPACKAGEPAIR* pEPP = *eppit;

		if( pEPP->bNowUsing )
			g_EffectManager.DeqEffectPackagePair( pEPP );
	}// for
	m_EffectPPList.clear();

	return TRUE;
}

BOOL CXiahCharObject::UpdateTargetMove()
{
	float fLocalFrameScale = 33.0f * g_fFrameScale;

	if( !m_bTargetMove)
	{
		// 화살 궤적
		if( m_pLineParticle )
		{
			m_pLineParticle->Update( fLocalFrameScale, m_Position );
			if( m_pLineParticle->IsFinished() )
			{
				m_bDeleteME = TRUE;
			}
		}

		return TRUE;
	}

	// TargetPosition�?실시간으�?변한다, 오브젝트 가 있다�?
	if( m_TargetObjectID != 0)
	{
		XiahObject::CXiahObject *pTargetObject = XiahObject::g_XiahObjectManager.FindXiahObject( MAKEOBJECTID( 0, m_TargetObjectID, m_TargetObjectType));

		if(pTargetObject == NULL)
		{
			m_TargetObjectID = 0;
		}

		if( pTargetObject && pTargetObject->m_pObject )
		{
			CXiahCharObject* pTargetCharObject = (CXiahCharObject*) pTargetObject->m_pObject;
			m_TargetPosition = pTargetCharObject->m_Position + Vector3( 0, pTargetCharObject->m_LocalBound.Size().y / 2, 0);
		}
	}

	Vector3 vDir = m_TargetPosition - m_Position;
//	Vector3 vDir2 = m_TargetPosition - m_TargetStartPosition;

	switch( m_nTargetMoveType)
	{
	case eLBP_Arrow:

		if( g_dwCurTime - m_TargetStartTime < m_TargetLifeTime)
		{
			vDir.Normalize();
			m_Position += vDir * m_fTargetMoveSpeed * g_fFrameScale;

//			SetAngleTarget( m_TargetPosition);

			// 화살 궤적
			if( m_pLineParticle )
			{
				m_pLineParticle->Update( fLocalFrameScale, m_Position );
			}
		}
		else
		{
			m_Position = m_TargetPosition;
			m_bTargetMove = FALSE;
			m_bRenderOK = false;
//			m_bDeleteME = TRUE;

			// 화살 궤적
			if( m_pLineParticle )
			{
				m_pLineParticle->End();
				m_pLineParticle->Update( fLocalFrameScale, m_Position );
			}
		}

		break;
	case eLBP_CharNavigation:	// 이따쉭은 비등�?운동인데 �?,�?
		// 여기�?아무것도 할수 없겠�?

		break;
	}

	return TRUE;
}

void CXiahCharObject::PersistEffect(_EFFECTPACKAGEPAIR** ppEffect, DWORD dwMugongID, int nType,
									float fLocalFrameScale, DWORD dwTotalTime)
{
	if( m_KeepUpMugongList.IsExist(dwMugongID) )
	{
		if(!*ppEffect)
		{
			g_EffectManager.MakeSharedPackagePair( 0, 0, 0 );

			_EFFECTPACKAGE* pEffectPackage = g_EffectManager.EnqOutGongPersistEffectImmediately( nType );
			// matrix
			if( pEffectPackage )
			{
				*ppEffect = g_EffectManager.GetCurEffectPackagePair();

				_EFFECTPACKAGEPAIR* pEffect = *ppEffect;

				pEffect->pWorldMatrix = (MATRIX*)m_CharRender.GetCharTM();
				if(dwTotalTime)
					pEffect->dwTotalTime = dwTotalTime;
				else
					pEffect->dwTotalTime = m_KeepUpMugongList.GetTime(dwMugongID);

				pEffect->dwElapsedTime = 0;
			}
			else
			{
				g_EffectManager.DeqEffectPackagePair( g_EffectManager.GetCurEffectPackagePair() );
				*ppEffect = (_EFFECTPACKAGEPAIR*)1;
			}

			g_EffectManager.OffSharedPackagePair();
		}
		else	// 
		{
			_EFFECTPACKAGEPAIR* pEffect = *ppEffect;
			if( pEffect != (_EFFECTPACKAGEPAIR*)1 )
			{
				pEffect->dwElapsedTime += fLocalFrameScale;
				pEffect->bIsVisible = true;
			}
		}
	}
	else	// 
	{
		if( *ppEffect )
		{
			if( *ppEffect != (_EFFECTPACKAGEPAIR*)1 )
				g_EffectManager.DeqEffectPackagePair( *ppEffect );
			*ppEffect = NULL;
		}
	}
}

BOOL CXiahCharObject::Update(BOOL bVisible)
{
	if( !m_CharRender.IsValid())
		return TRUE;

	float fLocalFrameScale = 33.0f * g_fFrameScale;

	// 아이�?회전
	if(m_bObjType == OBJTYPE_ITEM)
	{
		long _f = g_dwCurTime;
		SetAngle( _f / 5 );
	}

//  화살 이펙트를 위해�?이렇�?했음.
//	if( m_bTargetMove)
//	{
		UpdateTargetMove();
//	}

	// 야차�?광마독공, 사이즈가 커진�?
	if( m_bObjType == OBJTYPE_PC && m_bSubObjType == 4 )
	{
		if( m_bNowGwangmadokgong )
		{
			// 목표 값보�?작으�?점점 키워준�?
			if( m_fLocalScaleForGwangmadokgong < GWANGMADOKGONG_CHARSCALE )
			{
				m_fLocalScaleForGwangmadokgong += (GWANGMADOKGONG_CHARSCALE-1.0f) * (fLocalFrameScale) / 1500.0f;

				m_CharRender.SetLocalScale( Vector3(m_fLocalScaleForGwangmadokgong, m_fLocalScaleForGwangmadokgong, m_fLocalScaleForGwangmadokgong) );
			}
			else	// 그리�?고정
			if( m_fLocalScaleForGwangmadokgong != GWANGMADOKGONG_CHARSCALE )
			{
				m_fLocalScaleForGwangmadokgong = GWANGMADOKGONG_CHARSCALE;

				m_CharRender.SetLocalScale( Vector3(m_fLocalScaleForGwangmadokgong, m_fLocalScaleForGwangmadokgong, m_fLocalScaleForGwangmadokgong) );
			}
		}
		else
		{
			// 이제 다시 줄여준�?
			if( m_fLocalScaleForGwangmadokgong > 1.0f )
			{
				m_fLocalScaleForGwangmadokgong -= (GWANGMADOKGONG_CHARSCALE-1.0f) * (fLocalFrameScale) / 1500.0f;

				m_CharRender.SetLocalScale( Vector3(m_fLocalScaleForGwangmadokgong, m_fLocalScaleForGwangmadokgong, m_fLocalScaleForGwangmadokgong) );
			}
			else
			if( m_fLocalScaleForGwangmadokgong != 1.0f )
			{
				m_fLocalScaleForGwangmadokgong = 1.0f;

				m_CharRender.SetLocalScale( Vector3(m_fLocalScaleForGwangmadokgong, m_fLocalScaleForGwangmadokgong, m_fLocalScaleForGwangmadokgong) );
			}
		}
	}
	//HT_0711 : 진각�?무공(광마신공)
	else if( m_bObjType == OBJTYPE_PC && m_bRebirth > 6 )
	{
		if( m_bNowGwangmadokgong )
		{
			// 목표 값보�?작으�?점점 키워준�?
			if( m_fLocalScaleForGwangmadokgong < GWANGMADOKGONG_CHARSCALE )
			{
				m_fLocalScaleForGwangmadokgong += (GWANGMADOKGONG_CHARSCALE-1.0f) * (fLocalFrameScale) / 1500.0f;

				m_CharRender.SetLocalScale( Vector3(m_fLocalScaleForGwangmadokgong, m_fLocalScaleForGwangmadokgong, m_fLocalScaleForGwangmadokgong) );
			}
			else	// 그리�?고정
			if( m_fLocalScaleForGwangmadokgong != GWANGMADOKGONG_CHARSCALE )
			{
				m_fLocalScaleForGwangmadokgong = GWANGMADOKGONG_CHARSCALE;

				m_CharRender.SetLocalScale( Vector3(m_fLocalScaleForGwangmadokgong, m_fLocalScaleForGwangmadokgong, m_fLocalScaleForGwangmadokgong) );
			}
		}
		else
		{
			// 이제 다시 줄여준�?
			if( m_fLocalScaleForGwangmadokgong > 1.0f )
			{
				m_fLocalScaleForGwangmadokgong -= (GWANGMADOKGONG_CHARSCALE-1.0f) * (fLocalFrameScale) / 1500.0f;

				m_CharRender.SetLocalScale( Vector3(m_fLocalScaleForGwangmadokgong, m_fLocalScaleForGwangmadokgong, m_fLocalScaleForGwangmadokgong) );
			}
			else
			if( m_fLocalScaleForGwangmadokgong != 1.0f )
			{
				m_fLocalScaleForGwangmadokgong = 1.0f;

				m_CharRender.SetLocalScale( Vector3(m_fLocalScaleForGwangmadokgong, m_fLocalScaleForGwangmadokgong, m_fLocalScaleForGwangmadokgong) );
			}
		}
	}

	//
	UpdateTM();

	// 캐릭터가 화면�?안보이면 Update만하�?Render�?안한�? 이때 연결�?이펙트도 같이 해줘야함.
	// 여기서는 메인 오브젝트와 부착된 오브젝트�?Mesh 이펙트만 해주�?되고 
	// 메인 오브젝트�?애니메이�?이펙트는 CCharRender::PrepareRender 에서 한다. 
	// Mesh 이펙�? 보이�?오브젝트가 안보이면 CXiahCharObject에서 Update할때 Visible = FALSE�?하는�?
	// 이때 얘가 가지�?있는 Mesh 이펙트도 같이 안보이도�?해줘�?한다.캐릭터와 이펙트가 따로 렌더링되�?때문.
	if( bVisible )
	{
		if( m_CharRender.m_pMeshEffectPackagePair )
			m_CharRender.m_pMeshEffectPackagePair->bIsVisible = true;

		for(int h=0; h<LOGICAL_BONE_POS_COUNT; h++)
		{
			if( m_ChildChar[h].m_pMeshEffectPackagePair )
				m_ChildChar[h].m_pMeshEffectPackagePair->bIsVisible = true;
		}

		if( m_pItemGroundEffectPP )
            m_pItemGroundEffectPP->bIsVisible = true;

		if(m_pRebirthItemEffectPP)
			m_pRebirthItemEffectPP->bIsVisible = true;
	}
	else
	{
		if( m_CharRender.m_pMeshEffectPackagePair )
			m_CharRender.m_pMeshEffectPackagePair->bIsVisible = false;

		for(int h=0; h<LOGICAL_BONE_POS_COUNT; h++)
		{
			if( m_ChildChar[h].m_pMeshEffectPackagePair )
				m_ChildChar[h].m_pMeshEffectPackagePair->bIsVisible = false;
		}

		if( m_pItemGroundEffectPP )
            m_pItemGroundEffectPP->bIsVisible = false;

		if(m_pRebirthItemEffectPP)
			m_pRebirthItemEffectPP->bIsVisible = false;
	}// if

	// 숨어 있는 애가 Mesh Effect가 있으�?이것�?통과
	if( m_bObjStatus == NPCSTATUS_HIDE )
	{
		if( m_CharRender.m_pMeshEffectPackagePair )
			m_CharRender.m_pMeshEffectPackagePair->bIsVisible = false;
	}

	//
	m_CharRender.SetPosition( &m_ObjectTM);
	m_CharRender.SetLocalAngle( m_LocalAngle);

	// 바닥 아이템일 경우, 첨에 한번 실행. 이펙�?생성.
	if( m_bCreateItemGroundEffect && !m_pItemGroundEffectPP )
	{
		_EFFECTPACKAGE* pPackage = g_EffectManager.EnqAppearEffectImmediately( eItemGround );

		if( pPackage )
		{
			pPackage->pEffectRender->pPackagePair->WorldMatrix = (MATRIX)m_ObjectTM;
			pPackage->pEffectRender->pPackagePair->WorldMatrix._42 += 0.5f;
			m_pItemGroundEffectPP = pPackage->pEffectRender->pPackagePair;
		}
	}// if


	float reduse = m_CharRender.Get_AlphaEffect();
	//
	if( m_bGravityEnable)
	{
		float terrain_height = GetHeight( m_Position.x, m_Position.z);

		// 비등속이 일어난다
		if(m_bObjType == OBJTYPE_PC)
		{
			unsigned char ground_type = 0;
			ground_type = XiahMap::g_Map_Attri.Get_Attr(m_Position.x,-m_Position.z);
			m_CharRender.PrepareRender( bVisible == FALSE,m_dwEnemyType,ground_type);
			m_Shadow.SetSize(reduse);
		}
		else
		{
			m_CharRender.PrepareRender( bVisible == FALSE,m_dwEnemyType,0);
			m_Shadow.SetSize(reduse);
		}

		/*
		// 의문�?코드! PC�?m_bCollide가 TRUE인데 그러�?Y좌표보정은?? 이렇�?하니�?빠지�?올라가�?현상나타�?
		if( m_ObjectTM.t.y > terrain_height && m_bCollide == FALSE)
			m_ObjectTM.t.y -= GRAVITY_PER_FRAME * g_fFrameScale;
		if( m_ObjectTM.t.y < terrain_height && m_bCollide == FALSE)
			m_ObjectTM.t.y = terrain_height;
		*/
		// 그리하여, 바운딩박�?위에 올라�?때를 제외하고서는 보정해준�?
		if( m_ObjectTM.t.y > terrain_height && m_bRide == FALSE)
			m_ObjectTM.t.y -= GRAVITY_PER_FRAME * g_fFrameScale;

		if( m_ObjectTM.t.y < terrain_height && m_bRide == FALSE)
			m_ObjectTM.t.y = terrain_height;

		// 올라가 있으�?여기�?보정!
		if(m_bRide == TRUE) m_ObjectTM.t.y = m_fColHeight;	// 임시저�?바운�?높이

	}
	else
	{
		m_CharRender.PrepareRender( bVisible == FALSE,m_dwEnemyType,0);
		m_Shadow.SetSize(reduse);
	}

//	m_bCollide = FALSE;
	
	m_Position = m_ObjectTM.t;
	m_WorldBound = BBoxOBB3( m_LocalBound, m_ObjectTM);
	//
	if( m_nChildChar > 0 && bVisible)
	{
		for(int i = 0; i < LOGICAL_BONE_POS_COUNT; i++)
		{
			if( m_ChildChar[ i].IsValid())
			{
				m_ChildChar[ i].PrepareRender( bVisible == FALSE);	// 이넘들은 특별�?할게 없는�?
			}
		}
	}

	// 숨어있는 애들은 그림�?통과
	if( bVisible && m_bObjStatus != NPCSTATUS_HIDE)
	{
		Vector3 size = m_LocalBound.Size();
		size.y = 0;
		float fSize = 1.1f;

		// 2004_04_26 Changth : �?비무에서�?그림자가 바뀐다.

		// 일반 그림�? �?비무에서 나오�?특별�?그림자를 구분
		// 0 : 일반 그림�?
		// 1 : �?비무에서 단주
		// 2 : �?비무에서 단원
		// 3 : 문주
		BYTE byRenderType = 0;

		DWORD dwResIDAry[4];
		dwResIDAry[0] = 50000786;	// 일반 그림�?
		dwResIDAry[1] = 50001584;	// 단주 - 육각�?
		dwResIDAry[2] = 50001583;	// 단원 - 원형
		dwResIDAry[3] = 50002012;	// 문주

		DWORD dwResID = dwResIDAry[0];

		D3DCOLOR cColorAry[5];
		cColorAry[0] = D3DCOLOR_XRGB( 255, 255, 128 );	// Pick Cursor Color
		cColorAry[1] = D3DCOLOR_XRGB(  16, 128, 255 );	// Blue
		cColorAry[2] = D3DCOLOR_XRGB( 255,  67,  16 );	// Red
		cColorAry[3] = D3DCOLOR_XRGB( 255, 211, 117 );	// Yellow
		cColorAry[4] = D3DCOLOR_XRGB(   0, 255, 128 );	// Black�?초록색으�?바�?

		D3DCOLOR cColor = cColorAry[0];

		if(m_bObjType == OBJTYPE_PC)
		{
			if(m_dwPartyID)	// �?캐릭�?
			{
				CXiahCharObject* pMainCharObject = (CXiahCharObject*)g_pMainChar->m_pObject;

				// server id�?클라이언트가 가지�?있는 id�?바꾼�?
				DWORD dwPartyLeaderID = MAKEOBJECTID( 0, m_dwPartyLeaderID, OBJTYPE_PC );

				if( m_dwEnemyPartyID )	// �?비무 중인 캐릭터다.
				{
					if( pMainCharObject == this ) // 이게 나야?
					{
						// �?비무일때에는 포탈 불가�?
						g_MainCharInfo.m_bPortalMove = false;

						cColor = cColorAry[1];

						// 내가 단주인가?
						if( g_MainCharInfo.m_dwObjectID == m_dwPartyLeaderID )
						{
							dwResID = dwResIDAry[1];
							byRenderType = 1;
						}
						else
						{
							dwResID = dwResIDAry[2];
							byRenderType = 2;
						}
					}
					else	// 내가 아닌 다른 캐릭터일�?
					{
						if( m_dwPartyID == pMainCharObject->m_dwPartyID )	// 아군이네
						{
							cColor = cColorAry[1];

							// 이녀석이 단주인가?
							if( dwPartyLeaderID == m_dwXiahObjectID )
							{
								dwResID = dwResIDAry[1];
								byRenderType = 1;
							}
							else
							{
								dwResID = dwResIDAry[2];
								byRenderType = 2;
							}
						}
						else if( m_dwPartyID == pMainCharObject->m_dwEnemyPartyID )	// 적군이네
						{
							cColor = cColorAry[2];

							// 이녀석이 단주인가?
							if( dwPartyLeaderID == m_dwXiahObjectID )
							{
								dwResID = dwResIDAry[1];
								byRenderType = 1;
							}
							else
							{
								dwResID = dwResIDAry[2];
								byRenderType = 2;
							}
						}
						else	// �?�?단이로구�?
						{
							cColor = cColorAry[3];

							// 이녀석이 단주인가?
							if( dwPartyLeaderID == m_dwXiahObjectID )
							{
								dwResID = dwResIDAry[1];
								byRenderType = 1;
							}
							else
							{
								dwResID = dwResIDAry[2];
								byRenderType = 2;
							}
						}
					}
				}	// if( m_dwEnemyPartyID )
				else	// 단은 있고 �?비무�?안하�?있다.
				{
					cColor = cColorAry[4];
					byRenderType = 0;

					// 이녀석이 단주인가?
					if( dwPartyLeaderID == m_dwXiahObjectID )
					{
						dwResID = dwResIDAry[1];
						byRenderType = 1;
					}
				}
			}	// if( m_dwPartyID )
			else if(m_dwMunpaOrder == 1)
			{
				dwResID = dwResIDAry[3];
				byRenderType = 3;

				fSize = 1.5f;
				// 문주 그림�?교체
			} // if(m_dwMunpaOrder && !m_dwPartyID)

			// 이번�?문파전이�?
			// 문파전일�? 아군은 파란�? 적은 빨간색이�?
			// 주인공인 메인 캐릭터가 문파전을 할때에만 적용된다.
			if( g_pMainChar )
			{			
				CXiahCharObject* pMainCharObject = (CXiahCharObject*)g_pMainChar->m_pObject;
				if( pMainCharObject->m_bWarStatus == 2 )
				{
					if( m_bWarStatus == 2 )	// in war
					{
						fSize = 1.1f;

						if( pMainCharObject == this || m_dwMunpaID == pMainCharObject->m_dwMunpaID )
						{
							byRenderType = 2;
							cColor = cColorAry[1];
							dwResID = dwResIDAry[2];
						}
						else if( m_dwMunpaID == pMainCharObject->m_dwEnemyMunpaID )
						{
							byRenderType = 2;
							cColor = cColorAry[2];
							dwResID = dwResIDAry[2];
						}
					}
				}
			}// if( g_pMainChar )
		}
		else if(m_bObjType == OBJTYPE_FUNCTIONALNPC)
		{
			if(m_CharRender.GetCharID() == 1136)
			{
				fSize = 0.2f;
			}
		}
		

		bool bShadowCreate = false;

		// 메인 캐릭터가 암흑무에 걸리�?메인 캐릭�?그림자만 그린�?
		if( !g_XiahEnvInfo.m_bAmhukmuFog )
		{
            m_Shadow.Create( XiahPak::GetTexture(dwResID, true), m_Position.x,m_Position.z , size.GetLength() * fSize, cColor, TRUE, byRenderType );
			bShadowCreate = true;
		}
		else
		{
			if( g_pMainChar && g_pMainChar->m_pObject )
			{
				CXiahCharObject* pMainCharObject = (CXiahCharObject*)g_pMainChar->m_pObject;
				if( pMainCharObject == this )
				{
					m_Shadow.Create( XiahPak::GetTexture(dwResID, true), m_Position.x,m_Position.z , size.GetLength() * fSize, cColor, TRUE, byRenderType );
					bShadowCreate = true;
				}
			}
		}

		if( bShadowCreate )
		{
			XiahMap::g_XiahMap.m_pMapRender->AddVisibalMapDecal(&m_Shadow);
			if( m_pUpdateTargetDecal)
				m_pUpdateTargetDecal( (unsigned long)this);
		}
	}

	// �?궤적.
	if( m_SwordTrace.IsStart() )
	{
		Matrix4x4 *pBoneMatrix = m_CharRender.GetChildBoneMatrix( eLBP_RightHand);

		if( pBoneMatrix)
		{
			Vector3 vStart,vEnd;

			// 무투꺼는 두개가 변한다.
			Vector3 vV;
			int nCharID = m_CharRender.GetCharID();
			if( nCharID != 891 )
				vV = Vector3( 0, 0, -m_fWeaponLength );		// 검�? 연랑.
			else
				vV = Vector3( 0, 0.7f, -m_fWeaponLength );	// 무투

			vEnd = vStart + vV;

			vStart = vStart * (*pBoneMatrix);
			vEnd   = vEnd   * (*pBoneMatrix);

			m_SwordTrace.SetVisible( bVisible );
			m_SwordTrace.Update(fLocalFrameScale, vStart, vEnd);
		}
	}// if

	// 야차�?왼손 �?궤적
	if( m_bSubObjType == 4 && m_SwordTrace2.IsStart() )
	{
		Matrix4x4 *pBoneMatrix = m_CharRender.GetChildBoneMatrix( eLBP_LeftHand );

		if( pBoneMatrix )
		{
			Vector3 vStart,vEnd;

			Vector3 vV = Vector3( 0, 0, -m_fWeaponLength );

			vEnd = vStart + vV;
			vStart.z = m_fWeaponBackLength;

			vStart = vStart * (*pBoneMatrix);
			vEnd   = vEnd   * (*pBoneMatrix);

			m_SwordTrace2.SetVisible( bVisible );
			m_SwordTrace2.Update(fLocalFrameScale, vStart, vEnd);
		}
	}// if

	// 무공 지�?이펙�? 포탈�?날라�?캐릭�?문제�?여기�?이펙트를 만들�?업데이트�?해준�?
	if( bVisible )
	{
		if( m_bObjType == OBJTYPE_PC )		// PC에만 붙는 무공 지�?이펙�?
		{
			// 무수�?
			PersistEffect(&m_pMusuhonEffectPP, OUTGONGID_MUSUHON, eMusuhon, fLocalFrameScale);

			// 폭사�?
			PersistEffect(&m_pPoksahonEffectPP, OUTGONGID_POKSAHON, ePoksahon, fLocalFrameScale);

			// 금강�?
			PersistEffect(&m_pKuymgangrukEffectPP, OUTGONGID_KUMKANGLUK, eKuymgangruk, fLocalFrameScale);

			// 원기신강.
			PersistEffect(&m_pWonkisingangEffectPP, OUTGONGID_W0NKISINKANG, eWonkisingang, fLocalFrameScale);

			// 반탄강기.
			PersistEffect(&m_pBantankangkiEffectPP, OUTGONGID_BANTANKANGKI, eBantankangki, fLocalFrameScale);

			// 적운강기.
			PersistEffect(&m_pJukwonkangkiEffectPP, OUTGONGID_JUKUNKANGKI, eJukwonkangki, fLocalFrameScale);

			// 만독불진.
			PersistEffect(&m_pMandokbuljinEffectPP, OUTGONGID_MANDOKBULJIN, eMandokbuljin, fLocalFrameScale);

			// 연랑 사장신공지�?
			PersistEffect(&m_pSajangsingongEffectPP, YUN_SAJANGSINGONG, eYunSajangsingong, fLocalFrameScale);

			// 무투 기흡강기
			PersistEffect(&m_pKihubkangkiEffectPP, MU_KIHUBKANGKI, eMuKihubkangki, fLocalFrameScale);

			// 야차 호정강기
			PersistEffect(&m_pHojungkangkiEffectPP, YA_HOJUNGKANGKI, eYaHojungkangki, fLocalFrameScale);

			// 야차 은신술
			PersistEffect(&m_pEunsinsulEffectPP, YA_EUNSINSUL, eYaEunsinsul, fLocalFrameScale);

			// 연랑 수신기강
			PersistEffect(&m_pSusinkikangEffectPP, YUN_SUSINKIKANG, eYunrangSpecial, fLocalFrameScale);

			////HT_0711 : 진각�?무공
			// 흡성 신공 금강신공
			PersistEffect(&m_pKuymgangsingongEffectPP, REBRITH_KUMKANGSINGONG, eKuymgangruk, fLocalFrameScale);

			// 흡성 신공 분신신공
			//PersistEffect(&m_pBunsinsingongEffectPP, REBRITH_BUSNSINGONG, eKuymgangruk, fLocalFrameScale);
			
			// 흡성 신공 원기신공
			PersistEffect(&m_pWonkisingongEffectPP, REBRITH_W0NKISINGONG, eWonkisingang, fLocalFrameScale);
			
			// 흡성 신공 광마신공
			//PersistEffect(&m_pGwangmasingongEffectPP, REBRITH_GWANGMASINGONG, eKuymgangruk, fLocalFrameScale);
			
			
			// 이제 경공이다.
						bool hasGyungGong = ( m_KeepUpMugongList.IsExist(OUTGONGID_ILYUIDOGANG ) ||
								  m_KeepUpMugongList.IsExist(OUTGONGID_YUESUSINYUNG) ||
								  m_KeepUpMugongList.IsExist(OUTGONGID_JILPUNGBO)    ||
								  m_KeepUpMugongList.IsExist(OUTGONGID_CHOSANGBI) );
			if( hasGyungGong )
			{
				if( !m_pGyungGongEffectPP )
				{
					int nEffectType = eIlyuidogang;

					if( m_KeepUpMugongList.IsExist(OUTGONGID_ILYUIDOGANG) )
						nEffectType = eIlyuidogang;
					else if( m_KeepUpMugongList.IsExist(OUTGONGID_YUESUSINYUNG) )
						nEffectType = eYuesusinyung;
					else if( m_KeepUpMugongList.IsExist(OUTGONGID_JILPUNGBO) )
						nEffectType = eJilpungbo;
					else if( m_KeepUpMugongList.IsExist(OUTGONGID_CHOSANGBI) )
						nEffectType = eChosangbi;

					g_EffectManager.MakeSharedPackagePair( 0, 0, 0 );

					_EFFECTPACKAGE* pEffectPackage = g_EffectManager.EnqOutGongPersistEffectImmediately( nEffectType );
					if( pEffectPackage )
					{
						m_pGyungGongEffectPP = g_EffectManager.GetCurEffectPackagePair();
						m_pGyungGongEffectPP->pWorldMatrix = (MATRIX*)m_CharRender.GetCharTM();
					}
					else
					{
						g_EffectManager.DeqEffectPackagePair( g_EffectManager.GetCurEffectPackagePair() );
						m_pGyungGongEffectPP = (_EFFECTPACKAGEPAIR*)1;
					}

					g_EffectManager.OffSharedPackagePair();
				}
			}
			else
			{
				if( m_pGyungGongEffectPP )
				{
					if( m_pGyungGongEffectPP != (_EFFECTPACKAGEPAIR*)1 )
						g_EffectManager.DeqEffectPackagePair( m_pGyungGongEffectPP );
					m_pGyungGongEffectPP = NULL;
				}
			}
			
			bool hasFE = (m_KeepUpMugongList.IsExist(FIVEELEMENT_FIRE) || m_KeepUpMugongList.IsExist(FIVEELEMENT_WATER)
						|| m_KeepUpMugongList.IsExist(FIVEELEMENT_TREE) || m_KeepUpMugongList.IsExist(FIVEELEMENT_METAL)
						|| m_KeepUpMugongList.IsExist(FIVEELEMENT_EARTH));
			if( hasFE )
			{
				if(!m_pFEEffectPP)
				{
					g_EffectManager.MakeSharedPackagePair(0, 0, 0 );					
					
					int nEffectType =0;
					switch(m_bFECur)
					{
						case 1:	nEffectType = eFEFire;		break;
						case 2:	nEffectType = eFEWater;		break;
						case 3:	nEffectType = eFETree;		break;
						case 4:	nEffectType = eFEMetal;		break;
						case 5:	nEffectType = eFEEarth;		break;
					}

					nEffectType += (m_bFELevel * 5);					

					_EFFECTPACKAGE* pEffectPackage = g_EffectManager.EnqOutGongPersistEffectImmediately(nEffectType);
					if( pEffectPackage )
					{
						m_pFEEffectPP = g_EffectManager.GetCurEffectPackagePair();

						m_pFEEffectPP->pWorldMatrix = (MATRIX*)m_CharRender.GetCharTM();
						m_pFEEffectPP->dwTotalTime	= m_KeepUpMugongList.GetTime(FIVEELEMENT_FIRE);
						m_pFEEffectPP->dwElapsedTime = 0;
					}
					else
					{
						g_EffectManager.DeqEffectPackagePair( g_EffectManager.GetCurEffectPackagePair() );
						m_pFEEffectPP = (_EFFECTPACKAGEPAIR*)1;
					}

					g_EffectManager.OffSharedPackagePair();
				}
				else if( m_pFEEffectPP != (_EFFECTPACKAGEPAIR*)1 )
				{
					m_pFEEffectPP->dwElapsedTime += fLocalFrameScale;
					m_pFEEffectPP->bIsVisible = true;
				}
			}
			else
			{
				if( m_pFEEffectPP )
				{
					if( m_pFEEffectPP != (_EFFECTPACKAGEPAIR*)1 )
						g_EffectManager.DeqEffectPackagePair( m_pFEEffectPP );
					m_pFEEffectPP = NULL;
				}
			}

			if(m_bPotionEndKeepup == 1)
			{
				if(!m_pEventItemEffectPP)
				{
					g_EffectManager.MakeSharedPackagePair(0, 0, 0 );					

					_EFFECTPACKAGE* pEffectPackage = g_EffectManager.EnqOutGongPersistEffectImmediately(ePotion);
					if( pEffectPackage )
					{
						m_pEventItemEffectPP = g_EffectManager.GetCurEffectPackagePair();

						m_pEventItemEffectPP->pWorldMatrix  = (MATRIX*)m_CharRender.GetCharTM();
						m_pEventItemEffectPP->dwTotalTime	= 1000;
						m_pEventItemEffectPP->dwElapsedTime = 0;
					}
					else
					{
						g_EffectManager.DeqEffectPackagePair( g_EffectManager.GetCurEffectPackagePair() );
						m_pEventItemEffectPP = (_EFFECTPACKAGEPAIR*)1;
					}

					g_EffectManager.OffSharedPackagePair();
				}
				else if( m_pEventItemEffectPP != (_EFFECTPACKAGEPAIR*)1 )
				{
					m_pEventItemEffectPP->dwElapsedTime += fLocalFrameScale;
					m_pEventItemEffectPP->bIsVisible	= true;
				}
			}
			else
			{
				if( m_pEventItemEffectPP )
				{
					if( m_pEventItemEffectPP != (_EFFECTPACKAGEPAIR*)1 )
						g_EffectManager.DeqEffectPackagePair( m_pEventItemEffectPP );
					m_pEventItemEffectPP = NULL;
				}
			}

			if(m_bSpirit == 1)
			{
				if(!m_pSpiritEffectPP)
				{
					g_EffectManager.MakeSharedPackagePair(0, 0, 0 );					

					_EFFECTPACKAGE* pEffectPackage = g_EffectManager.EnqOutGongPersistEffectImmediately(eSpirit);
					if( pEffectPackage )
					{
						m_pSpiritEffectPP = g_EffectManager.GetCurEffectPackagePair();

						m_pSpiritEffectPP->pWorldMatrix  = (MATRIX*)m_CharRender.GetCharTM();
						m_pSpiritEffectPP->dwTotalTime	 = 1000;
						m_pSpiritEffectPP->dwElapsedTime = 0;
					}
					else
					{
						g_EffectManager.DeqEffectPackagePair( g_EffectManager.GetCurEffectPackagePair() );
						m_pSpiritEffectPP = (_EFFECTPACKAGEPAIR*)1;
					}

					g_EffectManager.OffSharedPackagePair();
				}
				else if( m_pSpiritEffectPP != (_EFFECTPACKAGEPAIR*)1 )
				{
					m_pSpiritEffectPP->dwElapsedTime += (33.0f * g_fFrameScale);
					m_pSpiritEffectPP->bIsVisible	= true;
				}
			}
			else
			{
				if(m_pSpiritEffectPP)
				{
					if( m_pSpiritEffectPP != (_EFFECTPACKAGEPAIR*)1 )
						g_EffectManager.DeqEffectPackagePair(m_pSpiritEffectPP);
					m_pSpiritEffectPP = NULL;
				}
			}

			if( m_KeepUpMugongList.IsExist(WHA_DRAGONSINJANG) || m_KeepUpMugongList.IsExist(WHA_DRAGONSUNGCHEON) )
			{
				if( !m_pWha_DragonPP )
				{
					g_EffectManager.MakeSharedPackagePair( 0, 0, 0 );

					_EFFECTPACKAGE* pEffectPackage = g_EffectManager.EnqOutGongPersistEffectImmediately( eWha_Dragon );
					if( pEffectPackage )
					{
						m_pWha_DragonPP = g_EffectManager.GetCurEffectPackagePair();

						m_pWha_DragonPP->pWorldMatrix = (MATRIX*)m_CharRender.GetCharTM();
						m_pWha_DragonPP->dwTotalTime = 3000;
						m_pWha_DragonPP->dwElapsedTime = 0;
					}
					else
					{
						g_EffectManager.DeqEffectPackagePair( g_EffectManager.GetCurEffectPackagePair() );
						m_pWha_DragonPP = (_EFFECTPACKAGEPAIR*)1;
					}

					g_EffectManager.OffSharedPackagePair();
				}
				else if( m_pWha_DragonPP != (_EFFECTPACKAGEPAIR*)1 )
				{
					if(m_pWha_DragonPP->dwElapsedTime >= m_pWha_DragonPP->dwTotalTime)
					{
						if( m_KeepUpMugongList.IsExist(WHA_DRAGONSINJANG))
							m_KeepUpMugongList.Delete( WHA_DRAGONSINJANG);
						
						if( m_KeepUpMugongList.IsExist(WHA_DRAGONSUNGCHEON))
							m_KeepUpMugongList.Delete( WHA_DRAGONSUNGCHEON);

						g_EffectManager.DeqEffectPackagePair( m_pWha_DragonPP );
						m_pWha_DragonPP = NULL;
					}
					else
					{
						m_pWha_DragonPP->dwElapsedTime += fLocalFrameScale;
						m_pWha_DragonPP->bIsVisible = true;
					}
				}
			}
			else
			{
				if( m_pWha_DragonPP )
				{
					if( m_pWha_DragonPP != (_EFFECTPACKAGEPAIR*)1 )
						g_EffectManager.DeqEffectPackagePair( m_pWha_DragonPP );
					m_pWha_DragonPP = NULL;
				}
			}

			if( m_KeepUpMugongList.IsExist(BING_DRAGONSINJANG ) || m_KeepUpMugongList.IsExist(BING_DRAGONSUNGCHEON ) )
			{
				if( !m_pBing_DragonPP )
				{
					g_EffectManager.MakeSharedPackagePair( 0, 0, 0 );

					_EFFECTPACKAGE* pEffectPackage = g_EffectManager.EnqOutGongPersistEffectImmediately( eBing_Dragon );
					if( pEffectPackage )
					{
						m_pBing_DragonPP = g_EffectManager.GetCurEffectPackagePair();

						m_pBing_DragonPP->pWorldMatrix = (MATRIX*)m_CharRender.GetCharTM();
						m_pBing_DragonPP->dwTotalTime = 3000;
						m_pBing_DragonPP->dwElapsedTime = 0;
					}
					else
					{
						g_EffectManager.DeqEffectPackagePair( g_EffectManager.GetCurEffectPackagePair() );
						m_pBing_DragonPP = (_EFFECTPACKAGEPAIR*)1;
					}

					g_EffectManager.OffSharedPackagePair();
				}
				else if( m_pBing_DragonPP != (_EFFECTPACKAGEPAIR*)1 )
				{
					if(m_pBing_DragonPP->dwElapsedTime >= m_pBing_DragonPP->dwTotalTime)
					{
						if( m_KeepUpMugongList.IsExist(BING_DRAGONSINJANG))
							m_KeepUpMugongList.Delete( BING_DRAGONSINJANG);
						
						if( m_KeepUpMugongList.IsExist(BING_DRAGONSUNGCHEON))
							m_KeepUpMugongList.Delete( BING_DRAGONSUNGCHEON);

						g_EffectManager.DeqEffectPackagePair( m_pBing_DragonPP );
						m_pBing_DragonPP = NULL;
					}
					else
					{
						m_pBing_DragonPP->dwElapsedTime += fLocalFrameScale;
						m_pBing_DragonPP->bIsVisible = true;
					}
				}
			}
			else
			{
				if( m_pBing_DragonPP )
				{
					if( m_pBing_DragonPP != (_EFFECTPACKAGEPAIR*)1 )
						g_EffectManager.DeqEffectPackagePair( m_pBing_DragonPP );
					m_pBing_DragonPP = NULL;
				}
			}

			if( m_KeepUpMugongList.IsExist(DOK_DRAGONSINJANG ) || m_KeepUpMugongList.IsExist(DOK_DRAGONSUNGCHEON ) )
			{
				if( !m_pDok_DragonPP )
				{
					g_EffectManager.MakeSharedPackagePair( 0, 0, 0 );

					_EFFECTPACKAGE* pEffectPackage = g_EffectManager.EnqOutGongPersistEffectImmediately( eDok_Dragon );
					if( pEffectPackage )
					{
						m_pDok_DragonPP = g_EffectManager.GetCurEffectPackagePair();

						m_pDok_DragonPP->pWorldMatrix = (MATRIX*)m_CharRender.GetCharTM();
						m_pDok_DragonPP->dwTotalTime = 3000;
						m_pDok_DragonPP->dwElapsedTime = 0;
					}
					else
					{
						g_EffectManager.DeqEffectPackagePair( g_EffectManager.GetCurEffectPackagePair() );
						m_pDok_DragonPP = (_EFFECTPACKAGEPAIR*)1;
					}

					g_EffectManager.OffSharedPackagePair();
				}
				else if( m_pDok_DragonPP != (_EFFECTPACKAGEPAIR*)1 )
				{
					if(m_pDok_DragonPP->dwElapsedTime >= m_pDok_DragonPP->dwTotalTime)
					{
						if( m_KeepUpMugongList.IsExist(DOK_DRAGONSINJANG))
							m_KeepUpMugongList.Delete( DOK_DRAGONSINJANG);
						
						if( m_KeepUpMugongList.IsExist(DOK_DRAGONSUNGCHEON))
							m_KeepUpMugongList.Delete( DOK_DRAGONSUNGCHEON);

						g_EffectManager.DeqEffectPackagePair( m_pDok_DragonPP );
						m_pDok_DragonPP = NULL;
					}
					else
					{
						m_pDok_DragonPP->dwElapsedTime += fLocalFrameScale;
						m_pDok_DragonPP->bIsVisible = true;
					}
				}
			}
			else
			{
				if( m_pDok_DragonPP )
				{
					if( m_pDok_DragonPP != (_EFFECTPACKAGEPAIR*)1 )
						g_EffectManager.DeqEffectPackagePair( m_pDok_DragonPP );
					m_pDok_DragonPP = NULL;
				}
			}

			if( m_KeepUpMugongList.IsExist(NOI_DRAGONSINJANG) || m_KeepUpMugongList.IsExist(NOI_DRAGONSUNGCHEON ) )
			{
				if( !m_pNoi_DragonPP )
				{
					g_EffectManager.MakeSharedPackagePair( 0, 0, 0 );

					_EFFECTPACKAGE* pEffectPackage = g_EffectManager.EnqOutGongPersistEffectImmediately( eNoi_Dragon );
					if( pEffectPackage )
					{
						m_pNoi_DragonPP = g_EffectManager.GetCurEffectPackagePair();

						m_pNoi_DragonPP->pWorldMatrix = (MATRIX*)m_CharRender.GetCharTM();
						m_pNoi_DragonPP->dwTotalTime = 3000;
						m_pNoi_DragonPP->dwElapsedTime = 0;
					}
					else
					{
						g_EffectManager.DeqEffectPackagePair( g_EffectManager.GetCurEffectPackagePair() );
						m_pNoi_DragonPP = (_EFFECTPACKAGEPAIR*)1;
					}

					g_EffectManager.OffSharedPackagePair();
				}
				else if( m_pNoi_DragonPP != (_EFFECTPACKAGEPAIR*)1 )
				{
					if(m_pNoi_DragonPP->dwElapsedTime >= m_pNoi_DragonPP->dwTotalTime)
					{
						if( m_KeepUpMugongList.IsExist(NOI_DRAGONSINJANG))
							m_KeepUpMugongList.Delete( NOI_DRAGONSINJANG);
						
						if( m_KeepUpMugongList.IsExist(NOI_DRAGONSUNGCHEON))
							m_KeepUpMugongList.Delete( NOI_DRAGONSUNGCHEON);

						g_EffectManager.DeqEffectPackagePair( m_pNoi_DragonPP );
						m_pNoi_DragonPP = NULL;
					}
					else
					{
						m_pNoi_DragonPP->dwElapsedTime += fLocalFrameScale;
						m_pNoi_DragonPP->bIsVisible = true;
					}
				}
			}
			else
			{
				if( m_pNoi_DragonPP )
				{
					if( m_pNoi_DragonPP != (_EFFECTPACKAGEPAIR*)1 )
						g_EffectManager.DeqEffectPackagePair( m_pNoi_DragonPP );
					m_pNoi_DragonPP = NULL;
				}
			}//각성 뇌룡 무공

		}// if( OBJTYPE_PC || OBJTYPE_NPC || OBJTYPE_PET )
	}// if( bVisible )
	else
	{	// 시간 계산은 계속하고 보이지 않도�?한다.
		EffectTimeUpdate(m_pMusuhonEffectPP, fLocalFrameScale);

		EffectTimeUpdate(m_pPoksahonEffectPP, fLocalFrameScale);

		EffectTimeUpdate(m_pKuymgangrukEffectPP, fLocalFrameScale);
	
		EffectTimeUpdate(m_pYuenoyuengEffectPP, fLocalFrameScale);

		EffectTimeUpdate(m_pKyugamsuEffectPP, fLocalFrameScale);

		EffectTimeUpdate(m_pWonkisingangEffectPP, fLocalFrameScale);

		EffectTimeUpdate(m_pPachunsoEffectPP, fLocalFrameScale);

		EffectTimeUpdate(m_pMarulkakEffectPP, fLocalFrameScale);

		EffectTimeUpdate(m_pAmhukmuEffectPP, fLocalFrameScale);

		EffectTimeUpdate(m_pTalbacinEffectPP, fLocalFrameScale);

		EffectTimeUpdate(m_pJukwonkangkiEffectPP, fLocalFrameScale);

		EffectTimeUpdate(m_pKumnasuEffectPP, fLocalFrameScale);

		EffectTimeUpdate(m_pBantankangkiEffectPP, fLocalFrameScale);

		EffectTimeUpdate(m_pFEEffectPP, fLocalFrameScale);

		EffectTimeUpdate(m_pWha_DragonPP, fLocalFrameScale);

		EffectTimeUpdate(m_pBing_DragonPP, fLocalFrameScale);

		EffectTimeUpdate(m_pDok_DragonPP, fLocalFrameScale);

		EffectTimeUpdate(m_pNoi_DragonPP, fLocalFrameScale);

		EffectTimeUpdate(m_pSajangsingongEffectPP, fLocalFrameScale);

		EffectTimeUpdate(m_pKihubkangkiEffectPP, fLocalFrameScale);

		EffectTimeUpdate(m_pEunsinsulEffectPP, fLocalFrameScale);

		EffectTimeUpdate(m_pHojungkangkiEffectPP, fLocalFrameScale);

		EffectTimeUpdate(m_pOdokchimEffectPP, fLocalFrameScale);

		EffectTimeUpdate(m_pDokhyulgongEffectPP, fLocalFrameScale);

		EffectTimeUpdate(m_pDoknaegongEffectPP, fLocalFrameScale);

		EffectTimeUpdate(m_pSsangdosuEffectPP, fLocalFrameScale);

		EffectTimeUpdate(m_pMandokbuljinEffectPP, fLocalFrameScale);

		EffectTimeUpdate(m_pDokmuEffectPP, fLocalFrameScale);

		EffectTimeUpdate(m_pGyungGongEffectPP, fLocalFrameScale);

		EffectTimeUpdate(m_pSusinkikangEffectPP, fLocalFrameScale);

		EffectTimeUpdate(m_pKuymgangsingongEffectPP, fLocalFrameScale); //HT_0711 : 진각�?무공

		//EffectTimeUpdate(m_pBunsinsingongEffectPP, fLocalFrameScale);

		EffectTimeUpdate(m_pWonkisingongEffectPP, fLocalFrameScale);		

		EffectTimeUpdate(m_pKyugamsingongEffectPP, fLocalFrameScale);

		EffectTimeUpdate(m_pKumnasingongEffectPP, fLocalFrameScale);

		EffectTimeUpdate(m_pMarulsingongEffectPP, fLocalFrameScale);

		//EffectTimeUpdate(m_pGwangmasingongEffectPP, fLocalFrameScale);

		EffectTimeUpdate(m_pDokhyulsingongEffectPP, fLocalFrameScale);

	}// if( bVisible )

	// 캐릭터에 연결�?이펙�? 여기�?리스�?관리만 하면 �? 사용하고 있는 것만 가지�?있는�?
	EFFECTPACKAGEPAIRLIST NowUseingList;

	EFFECTPACKAGEPAIRLIST::iterator eppit;
	for(eppit=m_EffectPPList.begin(); eppit!=m_EffectPPList.end(); ++eppit)
	{
		_EFFECTPACKAGEPAIR* pEPP = *eppit;
        
		if(pEPP && pEPP->bNowUsing )
		{
			bool bVis = false;
			if( bVisible )
				bVis = true;

			pEPP->bIsVisible = bVis;

			NowUseingList.push_back( pEPP );
		}
	}// for

	m_EffectPPList.clear();

	for(eppit=NowUseingList.begin(); eppit!=NowUseingList.end(); ++eppit)
	{
		m_EffectPPList.push_back( *eppit );
	}

	return TRUE;
}

/*
BOOL CXiahCharObject::ShadowRender()
{
	// 일단 PC, NPC, FUNCNPC �?Real Shadow
	if(m_bObjType != OBJTYPE_PC || m_bObjType != OBJTYPE_NPC || m_bObjType != OBJTYPE_FUNCTIONALNPC) return FALSE;

	// CCharRender Class
	m_CharRender.ShadowRender();
	return TRUE;
}
*/


// XIAH CHARACTER OBJECT�?렌더�?
BOOL CXiahCharObject::Render()
{
	if( !m_bRenderOK ) 
	{
		// 화살 궤적.
		if( m_pLineParticle )
			m_pLineParticle->Render();

		return TRUE;
	}

	if( !m_CharRender.IsValid()) return TRUE;

	BOOL bYaChaGwangmadokgong = FALSE;
	// 야차 광마독공은 헐크가 되어�?한다.
	if( m_bObjType == OBJTYPE_PC && m_bSubObjType == 4 )
	{
		if( m_bNowGwangmadokgong )
		{
			bYaChaGwangmadokgong = TRUE;
			m_CharRender.SetMaterialDiffuseColor( TRUE, GWANGMADOKGONG_MATERIAL_R, GWANGMADOKGONG_MATERIAL_G, GWANGMADOKGONG_MATERIAL_B );
		}
		else
			m_CharRender.SetMaterialDiffuseColor( FALSE, 1, 1, 1 );
	}
	//HT_0711 : 진각�?무공(광마신공)
	else if( m_bObjType == OBJTYPE_PC && m_bRebirth > 6 )
	{
		if( m_bNowGwangmadokgong )
		{
			bYaChaGwangmadokgong = TRUE;
			m_CharRender.SetMaterialDiffuseColor( TRUE, GWANGMADOKGONG_MATERIAL_R, GWANGMADOKGONG_MATERIAL_G, GWANGMADOKGONG_MATERIAL_B );
		}
		else
			m_CharRender.SetMaterialDiffuseColor( FALSE, 1, 1, 1 );
	}

	m_CharRender.Render(m_bGray);

	if( m_nChildChar > 0)
	{
		for(int i = 0; i < LOGICAL_BONE_POS_COUNT; i++)
		{
			if( m_ChildChar[ i].IsValid())
			{
				// 야차 광마독공은 모자까지 헐크가 되어�?한다.
				if( i == eLBP_Head )
				if( bYaChaGwangmadokgong )
					m_ChildChar[ i].SetMaterialDiffuseColor( TRUE, GWANGMADOKGONG_MATERIAL_R, GWANGMADOKGONG_MATERIAL_G, GWANGMADOKGONG_MATERIAL_B );
				else
					m_ChildChar[ i].SetMaterialDiffuseColor( FALSE, 1, 1, 1 );

				m_ChildChar[ i].Render(m_bGray);
			}
		}
	}

	g_pDirect3DDevice->SetRenderState( D3DRS_CULLMODE, D3DCULL_CCW);

	// 에너지 게이지
	if( m_bShowGage)
	{
		if( g_pCurrentCamera)
		{
			Vector3 scPos = g_pCurrentCamera->WorldToScreen( m_Position + Vector3( 0, m_LocalBound.m_vMax.y + 1, 0));

			m_rcObjectScreenPos.left = scPos.x;
			m_rcObjectScreenPos.top = scPos.y;
		}
		
		if( m_bShowManaGage)
		{
			// �?
			RenderEnergyGauge( m_rcObjectScreenPos.left - 40, m_rcObjectScreenPos.top - 13, 80, g_MainCharInfo.m_bStaminaCnt, 5, D3DCOLOR_XRGB(255, 204, 153), D3DCOLOR_XRGB(0, 0, 0), 5);

			RenderEnergyGauge( m_rcObjectScreenPos.left - 40, m_rcObjectScreenPos.top - 8, 80, g_MainCharInfo.m_dwHpCur, g_MainCharInfo.m_dwHpMax, m_cGageColor, D3DCOLOR_XRGB(0, 0, 0), 5);
			RenderEnergyGauge( m_rcObjectScreenPos.left - 40, m_rcObjectScreenPos.top - 3, 80, g_MainCharInfo.m_wIpCur, g_MainCharInfo.m_wIpMax, D3DCOLOR_XRGB( 0, 0, 255), D3DCOLOR_XRGB(0, 0, 0), 5);
		}
		else
		{
			RenderEnergyGauge( m_rcObjectScreenPos.left - 40, m_rcObjectScreenPos.top - 5, 80, m_dwCurHP, m_dwMaxHP, m_cGageColor, D3DCOLOR_XRGB(0, 0, 0), 5);
		}

		m_bShowObjectName = TRUE;	// �?. 이건 XiahGame_Main.cpp�?다른 사람�?쓰고 있어�?그냥 넣었�? 나중�?고쳐야됨
	}

	if(m_bShowObjectName || m_bSemiPKStatus == 2)
	{
		if( g_pCurrentCamera)
		{
			Vector3 scPos = g_pCurrentCamera->WorldToScreen( m_Position + Vector3( 0, m_LocalBound.m_vMax.y + 1, 0));

			m_rcObjectScreenPos.left = scPos.x;
			m_rcObjectScreenPos.top = scPos.y;
		}		

		switch(m_bObjType)
		{
		case OBJTYPE_PC:
			{
				D3DCOLOR cTradeName, cTradeDea;

				if(m_bSemiPKStatus == 1 || m_bSemiPKStatus == 2)
				{
					m_cNameColor = D3DCOLOR_XRGB(183, 46, 247);
					cTradeName	 = D3DCOLOR_XRGB(183, 46, 247);
					cTradeDea	 = D3DCOLOR_XRGB(183, 46, 247);
				}
				// CG_2005/01/28 : 변종아이템기능추가
				// 변종아이템 셋트�?입었�?		
				else if(m_bChangeItemSet == 1)
				{
					m_cNameColor = D3DCOLOR_XRGB( 0, 255, 0 );
					cTradeName	 = D3DCOLOR_XRGB( 0, 255, 0 );
					cTradeDea	 = D3DCOLOR_XRGB( 0, 255, 0 );
				}
				
				else
				{
					cTradeName = D3DCOLOR_XRGB(0, 204, 0);
					cTradeDea = D3DCOLOR_XRGB(255, 255, 166);

					RefreshFameColor();
				}

				TCHAR strTemp[128]= {0,};

				if(m_bTradeSell)
				{
					_stprintf(strTemp, _T("%s (%s)|%s"), (LPCTSTR)m_strShopName, (LPCTSTR)m_szObjectName, (LPCTSTR)m_strShopDescription);
					m_tObjectName.SetText( 0, 0, strTemp, GetFont(IDS_GULIM, 12), cTradeName, 8, 1, cTradeDea);

					m_rcObjectScreenPos.left -= m_tObjectName.GetSize().cx / 2;
				}
				else
				{
					if(m_dwMunpaID)
					{
						_stprintf(strTemp, _T("[%s]%s|%s"), (LPCTSTR)m_szMunpaName, (LPCTSTR)m_szMunpaNickName, (LPCTSTR)m_szObjectName);

						if((m_dwMunpaID != 0) && (g_MainCharInfo.m_dwLordMunpaID == m_dwMunpaID))
						{
							// 우승 문파명을 하늘색으�?바꿔�?							
							m_tObjectName.SetText( 0, 0, strTemp, GetFont(IDS_GULIM, 12), D3DCOLOR_XRGB( 102, 204, 255 ), 8, 1, m_cNameColor);
						}
						else
						{							
							m_tObjectName.SetText( 0, 0, strTemp, GetFont(IDS_GULIM, 12), m_cNameColor, 8);
						}
					}
					else
					{
						m_tObjectName.SetText(0, 0, m_szObjectName, GetFont(IDS_GULIM, 12), m_cNameColor, 8);
					}

					if(m_ChatMsg != _T("") && g_info.m_bHideChat)
					{
						if(m_dwMunpaID)
							m_rcObjectScreenPos.left -= m_tObjectName.GetSize(1).cx + m_ChatMsg.size()*2;
						else
							m_rcObjectScreenPos.left -= m_tObjectName.GetSize().cx + m_ChatMsg.size()*2;
					}
					else
					{
						m_rcObjectScreenPos.left -= m_tObjectName.GetSize().cx / 2;
					}
				}
			}
			break;
		case OBJTYPE_NPC:
			{
				if(m_wLevel)
				{
					TCHAR strTemp[128]= {0,};
					D3DCOLOR dwColor = D3DCOLOR_XRGB(255, 255, 255);
					LPCTSTR lpStr = NULL;

					int nLevelGap = static_cast<int>(m_wLevel) - static_cast<int>(g_MainCharInfo.m_wLevel);

					if(nLevelGap > 1)
					{
						if(nLevelGap >= 2 && nLevelGap <= 4)
						{
							lpStr = IDS_NPC_POWER_H_01;
							dwColor = D3DCOLOR_XRGB(255, 150, 150);
						}
						else if(nLevelGap >= 5 && nLevelGap <= 9)
						{
							lpStr = IDS_NPC_POWER_H_02;
							dwColor = D3DCOLOR_XRGB(255, 75, 75);
						}
						else
						{
							lpStr = IDS_NPC_POWER_H_03;
							dwColor = D3DCOLOR_XRGB(255, 0, 0);
						}
					}
					else
					{
						if(nLevelGap <= 1 && nLevelGap >= -1)
						{
							lpStr = IDS_NPC_POWER_N;
							dwColor = D3DCOLOR_XRGB(255, 255, 255);
						}
						else if(nLevelGap <= -2 && nLevelGap >= -4)
						{
							lpStr = IDS_NPC_POWER_L_01;
							dwColor = D3DCOLOR_XRGB(255, 255, 200);
						}
						else if(nLevelGap <= -5 && nLevelGap >= -9)
						{
							lpStr = IDS_NPC_POWER_L_02;
							dwColor = D3DCOLOR_XRGB(255, 255, 120);
						}
						else
						{
							lpStr = IDS_NPC_POWER_L_03;
							dwColor = D3DCOLOR_XRGB(255, 255, 0);
						}
					}

					_stprintf(strTemp, _T("%s(%d %s)|%s"), (LPCTSTR)m_szObjectName, m_wLevel, IDS_LEVEL, lpStr);

					m_tObjectName.SetText(0, 0, strTemp, GetFont(IDS_GULIM, 12), m_cNameColor, 8, 1, dwColor);
				}
				else
				{
					m_tObjectName.SetText(0, 0, (LPCTSTR)m_szObjectName, GetFont(IDS_GULIM, 12), m_cNameColor, 8, 1);
				}

				m_rcObjectScreenPos.left -= m_tObjectName.GetSize().cx / 2;
			}
			break;
		case OBJTYPE_ITEM:
			{
				if(m_bChangeItemSet)
				{
					m_cNameColor = D3DCOLOR_XRGB( 0, 255, 0 );
				}
				else if(!m_bChangeItemSet && g_bScreenShot)
				{
					m_cNameColor = D3DCOLOR_XRGB( 255, 255, 200);//HO_0509_07 오토대처방�?: 스샷으로 색상�?알아내지 못하도록 색상�?변�?
				}
				else
				{
					m_cNameColor = D3DCOLOR_XRGB( 240, 240, 190 );//HO_0509_07 오토대처방�?: 아이템을 아이�?명의 색상으로 검색하기에 색상�?변�?.... 기존색상( 255, 255, 200)					
				}

				m_tObjectName.SetText(0, 0, (LPCTSTR)m_szObjectName, GetFont(IDS_GULIM, 12), m_cNameColor, 8);

				m_rcObjectScreenPos.left -= m_tObjectName.GetSize().cx / 2;
			}
		    break;		
		//case OBJTYPE_ITEM: //HO_0525_07 오토대처방�?: 기존코드
		//	{
		//		if(m_bChangeItemSet)
		//		{
		//			m_cNameColor = D3DCOLOR_XRGB( 0, 255, 0 );
		//		}
		//		else
		//		{
		//			m_cNameColor = D3DCOLOR_XRGB( 255, 255, 200 );
		//		}

		//		m_tObjectName.SetText(0, 0, (LPCTSTR)m_szObjectName, GetFont(IDS_GULIM, 12), m_cNameColor, 8);

		//		m_rcObjectScreenPos.left -= m_tObjectName.GetSize().cx / 2;
		//	}
		//    break;
		case OBJTYPE_FUNCTIONALNPC:
			{
				sFunctionalNpcInfo* pInfo = (sFunctionalNpcInfo*)m_pPrivateData;

				if(pInfo)
				{
					if((pInfo->m_dwOwnID != 0) && (g_MainCharInfo.m_dwLordMunpaID == pInfo->m_dwOwnID))
					{
						// 우승 문파명을 하늘색으�?바꿔�?							
						m_tObjectName.SetText(0, 0, (LPCTSTR)m_szObjectName, GetFont(IDS_GULIM, 12), D3DCOLOR_XRGB( 102, 204, 255 ), 8);
					}
					else
					{							
						m_tObjectName.SetText(0, 0, (LPCTSTR)m_szObjectName, GetFont(IDS_GULIM, 12), m_cNameColor, 8);
					}
				}				

				if(m_ChatMsg.size() && g_info.m_bHideChat)
				{
					m_rcObjectScreenPos.left -= m_tObjectName.GetSize().cx + m_ChatMsg.size();
				}
				else
				{
					m_rcObjectScreenPos.left -= m_tObjectName.GetSize().cx / 2;
				}
			}
			break;
		default:
			{
				m_tObjectName.SetText(0, 0, (LPCTSTR)m_szObjectName, GetFont(IDS_GULIM, 12), m_cNameColor, 8);

				if(m_ChatMsg.size() && g_info.m_bHideChat)
				{
					m_rcObjectScreenPos.left -= m_tObjectName.GetSize().cx + m_ChatMsg.size();
				}
				else
				{
					m_rcObjectScreenPos.left -= m_tObjectName.GetSize().cx / 2;
				}
			}
			break;
		}

		// 여기�?안그린다.
//		m_tObjectName.Render();
	} // if( m_bShowObjectName)

	// 화살 궤적.
	if( m_pLineParticle )
        m_pLineParticle->Render();

	// �?궤적.
	if( m_SwordTrace.IsStart() )
		m_SwordTrace.Render();

	// 야차 왼손 �?궤적
	if( m_bSubObjType == 4 && m_SwordTrace2.IsStart() )
		m_SwordTrace2.Render();

	// 채팅 박스
	//ShowChatBox();	

	return TRUE;
}

void CXiahCharObject::SetChatBox( DWORD dwTimeInterval, sString ChatMsg, BYTE byChatType)
{
	m_dwTimeInterval = dwTimeInterval;
	m_ChatMsg = ChatMsg;

	switch( byChatType)
	{
	case CT_WHISPER:
		m_ChatColor = D3DCOLOR_XRGB( 0, 255, 0);
		break;
	case CT_DAN:
		m_ChatColor = D3DCOLOR_XRGB( 255, 255, 0);
		break;
	case CT_BATTLE:
		break;
	default:
		m_ChatColor = D3DCOLOR_XRGB( 255, 255, 255);
		break;
	}

	m_text2DForChatBox.SetText(0, 0, (LPCTSTR)m_ChatMsg, GetFont( IDS_DUDUM, 12), m_ChatColor);
}

void CXiahCharObject::ShowChatBox()
{
	if( !m_ChatMsg.empty())
	{		
		if( (g_dwCurTime - m_dwTimeInterval) > 30000)
		{
			m_ChatMsg.clear();
		}
		
		if( g_pCurrentCamera)
		{
			Vector3 scPos = g_pCurrentCamera->WorldToScreen( m_Position + Vector3( 0, m_LocalBound.m_vMax.y + 1, 0));

			if( m_bShowObjectName || m_bSemiPKStatus == 2)	// SEMI PK�?깜빡상태�?경우
			{
				if(m_bTradeSell)
				{
					// 개인 상점 개설�?
					m_rcObjectScreenPos2.left = scPos.x - (m_ChatMsg.size()*2 + 6);
					m_rcObjectScreenPos2.top = m_rcObjectScreenPos.top + 32;
				}
				else
				{
					// 일반 사용�?
					if(m_dwMunpaID)
					{
						m_rcObjectScreenPos2.left = m_rcObjectScreenPos.left + m_tObjectName.GetSize(1).cx + 6;
						m_rcObjectScreenPos2.top = m_rcObjectScreenPos.top + 16;
					}
					else
					{
						m_rcObjectScreenPos2.left = m_rcObjectScreenPos.left + m_tObjectName.GetSize().cx + 6;
						m_rcObjectScreenPos2.top = scPos.y;
					}
				}
			} // if( m_bShowObjectName)
			else
			{
				// 이것�?저것도 아닌사람
				m_rcObjectScreenPos2.left = scPos.x - m_ChatMsg.size()*2;
				m_rcObjectScreenPos2.top = scPos.y;
			}
		}

		m_text2DForChatBox.Render();
	}
}

BOOL CXiahCharObject::Create(int nCharID,int nMeshType,int nTextureType,int nAniType)
{
	m_CharRender.SetChar( nCharID);
	m_CharRender.SetMesh( nMeshType, nTextureType);
	
	if( !m_CharRender.IsValid())
		return TRUE;

	if( nAniType != -1)
	{
		if(m_CharRender.SetAnimation( nAniType) == FALSE)
		{
			// 여기�?RETURN 하면 BOUNDBOX 설정 실패�?
			//return FALSE;	
		}
	}
	
	m_LocalBound = m_CharRender.GetLocalBound();

	Vector3 c = m_LocalBound.Center();
	//c.y = -c.y;
	c.y = 0;
	m_LocalBound.m_vMin -= c;
	m_LocalBound.m_vMax -= c;
 
	if( nAniType != -1)
	{
		m_nCurMotionType	= XiahAniType::eLAT_Stand;
		m_nNextMotionType	= XiahAniType::eLAT_Stand;
		
		m_nCurAniType		= nAniType;
		m_nNextAniType		= nAniType;

		SetAnimation( XiahAniType::eLAT_Stand, XiahAniType::eLAT_Stand, 0, 0);
	}

	ZeroMemory( m_pParentTrigger, sizeof(CTrigger*) * eXCT_Count);

	// Load fame title dynamically based on player's fame
	if (m_bObjType == OBJTYPE_PC)
	{
		RefreshFameColor();
	}

	return TRUE;
}

int CXiahCharObject::GetAnimation()		// Get current animation
{
	return m_nCurMotionType;
}

BOOL CXiahCharObject::SetAnimation(int nCurMotionType,int nNextMotionType,int nCurIndex,int nNextIndex,float fAnimationSpeed)
{
	if( m_pAniType == NULL)
		return FALSE;

	m_nCurMotionType	= nCurMotionType;
	m_nNextMotionType	= nNextMotionType;

	m_nCurAniIndex	= nCurIndex;
	m_nNextAniIndex	= nNextIndex;
	
	m_nCurAniType = m_pAniType->GetAniType( nCurMotionType, nCurIndex);
	m_nNextAniType = m_pAniType->GetAniType( nNextMotionType, nNextIndex);

	if( m_nCurMotionType == XiahAniType::eLAT_Stand)
	{
		m_bTargetMove = FALSE;
	}

	m_CharRender.SetAnimation( m_nCurAniType, fAnimationSpeed);

	BOOL bSwordTrace = FALSE;
	// �?궤적
	if( m_bObjType == OBJTYPE_PC && (m_nCurMotionType == XiahAniType::eLAT_NormalAttack || m_nCurMotionType == XiahAniType::eLAT_Mugong))
	{
		bSwordTrace = TRUE;

		// Effect Enable
		Matrix4x4 *pBoneMatrix = m_CharRender.GetChildBoneMatrix( eLBP_RightHand);

		if( pBoneMatrix)
		{
			Vector3 vStart,vEnd;

			// 무투꺼는 두개가 변한다.
			Vector3 vV;
			int nCharID = m_CharRender.GetCharID();
			if( nCharID != 891 )
				vV = Vector3( 0, 0, -m_fWeaponLength );		// 검�? 연랑.
			else
				vV = Vector3( 0, 0.7f, -m_fWeaponLength );	// 무투

			vEnd = vStart + vV;

			vStart = vStart * (*pBoneMatrix);
			vEnd   = vEnd   * (*pBoneMatrix);

			m_SwordTrace.Start( vStart, vEnd, 200, 200, 200 );
		}
		else
		{
			DBG_LogFile( _T("CXiahCharObject::SetAnimation 실패"));
//			return false;
		}
	}
	else
	{
		// Effect Disable
		m_SwordTrace.End();
	}

	// 야차 왼손 �?궤적
	if( bSwordTrace && m_bSubObjType == 4 )
	{
		Matrix4x4 *pBoneMatrix = m_CharRender.GetChildBoneMatrix( eLBP_LeftHand);

		if( pBoneMatrix)
		{
			Vector3 vStart,vEnd;

			Vector3 vV = Vector3( 0, 0, -m_fWeaponLength );

			vEnd = vStart + vV;
			vStart.z = m_fWeaponBackLength;

			vStart = vStart * (*pBoneMatrix);
			vEnd   = vEnd   * (*pBoneMatrix);

			m_SwordTrace2.Start( vStart, vEnd, 200, 200, 200 );
		}
		else
		{
			DBG_LogFile( _T("CXiahCharObject::SetAnimation 실패"));
		}
	}

	if( !bSwordTrace && m_bSubObjType == 4 )
	{
		// Effect Disable
		m_SwordTrace2.End();
	}

	return TRUE;
}

BOOL CXiahCharObject::SetAnimation(int nMotionType,int nIndex,float fAnimationSpeed)
{
	if( m_pAniType == NULL)
		return FALSE;

	m_nCurMotionType = m_nNextMotionType = nMotionType;
	m_nCurAniType = m_nNextAniType = m_pAniType->GetAniType( nMotionType, nIndex);

	m_nCurAniIndex = m_nNextAniIndex = nIndex;

	if( m_nCurMotionType == XiahAniType::eLAT_Stand)
	{
		m_bTargetMove = FALSE;
	}

	if(m_CharRender.SetAnimation( m_nCurAniType, fAnimationSpeed) == FALSE)
	{
		return FALSE;	
	}

	BOOL bSwordTrace = FALSE;
	// �?궤적
	if( m_bObjType == OBJTYPE_PC && (m_nCurMotionType == XiahAniType::eLAT_NormalAttack || m_nCurMotionType == XiahAniType::eLAT_Mugong))
	{
		bSwordTrace = TRUE;

		// Effect Enable
		Matrix4x4 *pBoneMatrix = m_CharRender.GetChildBoneMatrix( eLBP_RightHand);

		if( pBoneMatrix)
		{
			Vector3 vStart,vEnd;

			// 무투꺼는 두개가 변한다.
			Vector3 vV;
			int nCharID = m_CharRender.GetCharID();
			if( nCharID != 891 )
				vV = Vector3( 0, 0, -m_fWeaponLength );		// 검�? 연랑.
			else
				vV = Vector3( 0, 0.7f, -m_fWeaponLength );	// 무투

			vEnd = vStart + vV;

			vStart = vStart * (*pBoneMatrix);
			vEnd   = vEnd   * (*pBoneMatrix);

			m_SwordTrace.Start( vStart, vEnd, 200, 200, 200 );
		}
	}
	else
	{
		// Effect Disable
		m_SwordTrace.End();
	}

	// 야차 왼손 �?궤적
	if( bSwordTrace && m_bSubObjType == 4 )
	{
		Matrix4x4 *pBoneMatrix = m_CharRender.GetChildBoneMatrix( eLBP_LeftHand);

		if( pBoneMatrix)
		{
			Vector3 vStart,vEnd;

			Vector3 vV = Vector3( 0, 0, -m_fWeaponLength );

			vEnd = vStart + vV;
			vStart.z = m_fWeaponBackLength;

			vStart = vStart * (*pBoneMatrix);
			vEnd   = vEnd   * (*pBoneMatrix);

			m_SwordTrace2.Start( vStart, vEnd, 200, 200, 200 );
		}
		else
		{
			DBG_LogFile( _T("CXiahCharObject::SetAnimation 실패"));
		}
	}
	if( !bSwordTrace && m_bSubObjType == 4 )
	{
		// Effect Disable
		m_SwordTrace2.End();
	}

	return TRUE;
}

/**
 * 애니메이션이 끝날�?
 * \param param 
 * \return 
 */
int CXiahCharObject::OnEndAnimation(unsigned long param)
{
	m_bMoveable = TRUE;
	m_bAttack = FALSE;

	// �?궤적 Effect Disable
	if( m_nCurAniType != m_nNextAniType)
	{
		m_SwordTrace.End();
		m_SwordTrace2.End();
		m_CharRender.SetAnimation( m_nNextAniType);

		m_nCurAniType = m_nNextAniType;
		m_nCurMotionType = m_nNextMotionType;
		m_nCurAniIndex = m_nNextAniIndex;

/*		if(XiahAniType::eLAT_Rebirth == m_nCurMotionType)
		{
			if(m_dwXiahObjectID == g_MainCharInfo.m_dwXiahObjectID)
			{
				g_pUIManager->ShowNotice(IDS_REBIRTH_INFORM, NOTICE_FRAME_OK, NOTICE_FRAME_REBIRTH_SUCCESS);
			}
		}*/

		return 1;
	}
	else
	{
		if( m_nCurMotionType == XiahAniType::eLAT_Die && m_bObjType != OBJTYPE_PC)
		{
			if( m_CharRender.IsAlphaEffect() == FALSE)
			{
				float fAlphaEffectSpeed = 1.0f;

				// 어떤 NPC�?�?빠르�?사라진다.
				if( m_bObjType == OBJTYPE_NPC )
				{
					switch( m_bSubObjType )
					{
					case 184:	// 신조
						fAlphaEffectSpeed = 4.0f;
						break;
					};
				}

				m_CharRender.StartAlphaEffect( fAlphaEffectSpeed );
			}

			m_SwordTrace.End();
			m_SwordTrace2.End();
		}
		else if( m_nCurAniIndex == -1)	// 랜덤 반복이라�?�?�?,�?
		{
			m_SwordTrace.End();
			m_SwordTrace2.End();
			m_nCurAniType = m_pAniType->GetAniType( m_nCurMotionType, m_nCurAniIndex);
			m_CharRender.SetAnimation( m_nCurAniType);

			return 1;
		}
		// 채집애니 끝날�?�?이펙�?붙여주기
		else if(XiahAniType::eLAT_Collect == m_nCurMotionType)
		{
			m_CharRender.StopEffect();
			m_CharRender.SpawnEffect();
		}
	}

	return 0;
}

int CXiahCharObject::OnEndAlphaEffect(unsigned long param)
{
	m_bDeleteME = TRUE;

	return 0;
}

BOOL CXiahCharObject::AttachChildCharRender(int nLogicalPos,int nCharID,int nMeshType,int nTextureType, int nEffectIndex)
{
	// 신발은 예외 처리
	if( nLogicalPos == eLBP_Shoe)
	{
		CRes_Character* pChar = GetCharacter( nCharID);

//		DBG_Assert( pChar != NULL);
		if( pChar == NULL)		return TRUE;

		Res_Mesh* pMesh = pChar->GetMesh( nMeshType);

//		DBG_Assert( pMesh != NULL);
		if( pMesh == NULL)		return TRUE;

		Res_CharTexture* pTexture = pMesh->GetTexture( nTextureType);

//		DBG_Assert( pTexture != NULL);
		if( pTexture == NULL)	return TRUE;

//		DBG_Assert( pTexture->texture_sub_count >= 3);
		if( pTexture->texture_sub_count < 3)	return TRUE;

		m_CharRender.ChangeTexture( 1, pTexture->texture_sub_ptr[ 1].texture_id);

		return TRUE;
	}


	if( nLogicalPos == eLBP_Protector) // �?
	{
		CRes_Character* pChar = GetCharacter( nCharID);

		if( pChar == NULL)		return TRUE;

		Res_Mesh* pMesh = pChar->GetMesh( nMeshType);

		if( pMesh == NULL)		return TRUE;

		Res_CharTexture* pTexture = pMesh->GetTexture( nTextureType);

		if( pTexture == NULL)	return TRUE;

		//if( pTexture->texture_sub_count < 4)	return TRUE;
				
		// 같은 캐릭터면 MeshType�?바꿔 준�?
		// 코드 멋지�?
		if( m_CharRender.GetCharID() == nCharID && m_CharRender.GetMeshType() == nMeshType)
		{
			m_CharRender.ChangeTexture( 0, pTexture->texture_sub_ptr[ 0].texture_id);
			m_CharRender.ChangeTexture( 3, pTexture->texture_sub_ptr[ 0].texture_id);

			// 무투�?4단계�?날개�?텍스쳐가 몸과 다른�?쓴다. 날개�?MeshBlock 4번째�?
			// 변�?넣어가지�?버그�?생기�?하고..
			if( nCharID == 891/*m_bSubObjType == 3*/ && nMeshType >= 3 )
			{
				 m_CharRender.ChangeTexture( 3, pTexture->texture_sub_ptr[ 3].texture_id);
			}
			// 야차�?2단계 부터는 망토가 몸과 다른 텍스쳐를 쓴다. 망토�?MeshBlock 4번째�?
			// 변�?넣어가지�?버그�?생기�?하고..
			else if( nCharID == 906 /*m_bSubObjType == 4*/ && nMeshType > 0 )
			{
				 m_CharRender.ChangeTexture( 3, pTexture->texture_sub_ptr[ 3].texture_id);
			}
			else if(nCharID == 867 && (nMeshType == 4 || nMeshType == 5 || nMeshType == 6)) //HT_1116 : 각성�?아이�?추가
			{
				m_CharRender.ChangeTexture(3, pTexture->texture_sub_ptr[ 3].texture_id);
			}
		}
		else// 캐릭터가 다르�?다시 만들�?준�?꾸웩!!
		{
			// 일단 캐릭터를 만들�?
			m_CharRender.Clear();
			m_CharRender.SetChar( nCharID);
			m_CharRender.SetMesh( nMeshType, nTextureType);
			
			// 이부분에�?Intro�?Aninmation 결정!

			if( m_pAniType == NULL)
			{
				switch(m_bSubObjType)
				{
					case 1 : // 검�?
						m_CharRender.SetAnimation( 201);
					break;

					case 2 :	// 연랑
					case 3 :	// 무투
					case 4 :	// 야차
						m_CharRender.SetAnimation( 103);
					break;
				}
			}
			else
				SetAnimation( XiahAniType::eLAT_Stand, 0);


			// 장착 관�?Bone정보�?다시 세팅해준�?
			for(int i = 0; i < LOGICAL_BONE_POS_COUNT; i++)
			{
				if( m_ChildChar[ i].IsValid())
				{
					Matrix4x4 *pBoneMatrix = m_CharRender.GetChildBoneMatrix( i);

					m_ChildChar[ i].SetPosition( pBoneMatrix);
				}
			}
 
		}
		return TRUE; 
	}

	RemoveChildCharRender( nLogicalPos);

	CRes_Character* pChar = GetCharacter( nCharID);

	if( pChar == NULL)		return TRUE;

	Res_Mesh* pMesh = pChar->GetMesh( nMeshType);

	if( pMesh == NULL)		return TRUE;

	Res_CharTexture* pTexture = pMesh->GetTexture( nTextureType);

	if( pTexture == NULL)	return TRUE;


	m_ChildChar[ nLogicalPos].SetChar( nCharID);
	m_ChildChar[ nLogicalPos].SetMesh( nMeshType, nTextureType);
	// 
	//m_ChildChar[ nLogicalPos].SetAnimation( 0);
	m_ChildChar[ nLogicalPos].EnableBoneAnimation( FALSE);
	Matrix4x4 DummyTM;
	m_ChildChar[ nLogicalPos].SetLocalCenter(DummyTM);
	//m_ChildChar[ nLogicalPos].SetLocalCenter(Matrix4x4());
	DBG_Assert( m_ChildChar[ nLogicalPos].IsValid());

	// 무기일때 이펙트가 붙는�?
	if( nEffectIndex >= 0 )
	{
		m_ChildChar[ nLogicalPos].MakeMeshEffect( nEffectIndex );
	}

	// Matrix
	Matrix4x4 *pBoneMatrix = m_CharRender.GetChildBoneMatrix( nLogicalPos);

	m_ChildChar[ nLogicalPos].SetPosition( pBoneMatrix);
	++m_nChildChar;	// 아구가 맞을려나?

	return TRUE;
}

// test code
bool CXiahCharObject::AttachPetChildChar(int nLogicalPos, int nCharID, int nMeshType, int nTextureType)
{
	// 신발은 예외 처리
	if(nLogicalPos == eLBP_Shoe)
	{
		CRes_Character* pChar = GetCharacter( nCharID);

		if(pChar == NULL)		return TRUE;

		Res_Mesh* pMesh = pChar->GetMesh( nMeshType);

		if( pMesh == NULL)		return TRUE;

		Res_CharTexture* pTexture = pMesh->GetTexture( nTextureType);

		if(pTexture == NULL)	return TRUE;
		if(pTexture->texture_sub_count < 3)	return TRUE;

		m_CharRender.ChangeTexture(2, pTexture->texture_sub_ptr[2].texture_id);

		return true;
	}

	/////////////////////////////////////////////////////////////////////////////////////////////////////
	if(nLogicalPos == eLBP_Protector) // �?
	{
		CRes_Character* pChar = GetCharacter( nCharID);

		if( pChar == NULL)		return TRUE;

		Res_Mesh* pMesh = pChar->GetMesh( nMeshType);

		if( pMesh == NULL)		return TRUE;

		Res_CharTexture* pTexture = pMesh->GetTexture( nTextureType);

		if( pTexture == NULL)	return TRUE;

		//if( pTexture->texture_sub_count < 4)	return TRUE;

		// 같은 캐릭터면 MeshType�?바꿔 준�?
		if( m_CharRender.GetCharID() == nCharID && m_CharRender.GetMeshType() == nMeshType)
		{
			m_CharRender.ChangeTexture( 0, pTexture->texture_sub_ptr[ 0].texture_id);
			m_CharRender.ChangeTexture( 3, pTexture->texture_sub_ptr[ 0].texture_id);
		}
		else// 캐릭터가 다르�?다시 만들�?준�?꾸웩!!
		{
			// 일단 캐릭터를 만들�?
			m_CharRender.Clear();
			m_CharRender.SetChar( nCharID);
			m_CharRender.SetMesh( nMeshType, nTextureType);

			SetAnimation( XiahAniType::eLAT_Stand, 0);


			// 장착 관�?Bone정보�?다시 세팅해준�?
			for(int i = 0; i < LOGICAL_BONE_POS_COUNT; ++i)
			{
				if( m_ChildChar[ i].IsValid())
				{
					Matrix4x4 *pBoneMatrix = m_CharRender.GetChildBoneMatrix( i);

					assert(pBoneMatrix);

					m_ChildChar[ i].SetPosition( pBoneMatrix);
				}
			}
		}

		return true; 
	} // if(nLogicalPos == eLBP_Protector) // �?

	/////////////////////////////////////////////////////////////////////////////////////////////////////
	RemoveChildCharRender(nLogicalPos);

	CRes_Character* pChar = GetCharacter( nCharID);

	if( pChar == NULL)		return TRUE;

	Res_Mesh* pMesh = pChar->GetMesh( nMeshType);

	if( pMesh == NULL)		return TRUE;

	Res_CharTexture* pTexture = pMesh->GetTexture( nTextureType);

	if( pTexture == NULL)	return TRUE;


	m_ChildChar[ nLogicalPos].SetChar( nCharID);
	m_ChildChar[ nLogicalPos].SetMesh( nMeshType, nTextureType);
	m_ChildChar[ nLogicalPos].EnableBoneAnimation( FALSE);

	Matrix4x4 DummyTM;
	m_ChildChar[ nLogicalPos].SetLocalCenter(DummyTM);
	
	DBG_Assert( m_ChildChar[ nLogicalPos].IsValid());

	// Matrix
	Matrix4x4 *pBoneMatrix = m_CharRender.GetChildBoneMatrix( nLogicalPos);

	assert(pBoneMatrix);

	m_ChildChar[ nLogicalPos].SetPosition( pBoneMatrix);
	++m_nChildChar;	// 아구가 맞을려나?


	return true;
}

BOOL CXiahCharObject::RemoveChildCharRender(int nLogicalPos)
{
	if( m_ChildChar[ nLogicalPos].IsValid())
	{
		m_ChildChar[ nLogicalPos].Clear();
		--m_nChildChar;	// 아구가 맞을려나?
	}

	return TRUE;
}

int CXiahCharObject::OnTMUpdate(unsigned long frame_delta_tm)
{
	Matrix4x4* pFrameDelta = (Matrix4x4*)frame_delta_tm;

	if( pFrameDelta->t.GetLength() == 0)
		return 0;
	

	if( m_nCurMotionType != XiahAniType::eLAT_Walk &&
		m_nCurMotionType != XiahAniType::eLAT_Run &&
		m_nCurMotionType != XiahAniType::eLAT_Mugong && m_bObjType == OBJTYPE_PC)
	{
		pFrameDelta->t.x = 0;
		pFrameDelta->t.z = 0;
		return 0;
	}

	if( m_nCurMotionType == XiahAniType::eLAT_NormalAttack &&
		m_nCurMotionType == XiahAniType::eLAT_Hit &&
		m_nCurMotionType == XiahAniType::eLAT_Defend && m_bObjType == OBJTYPE_NPC)
	{
		pFrameDelta->t.x = 0;
		pFrameDelta->t.z = 0;
		return 0;
	}
	// 화살�?아닌 넘들�?한해�?TargetMove�?여기다가 살짝 넣어준�?
	
	Matrix4x4 future_tm = *pFrameDelta * m_ObjectTM;
	future_tm.t.y -= 0.5f;
	BBoxOBB3 future_bound = BBoxOBB3( m_LocalBound, future_tm);

	if( m_bTargetMove && m_nTargetMoveType == eLBP_CharNavigation)
	{
		Vector3 vDir = m_TargetPosition - m_TargetStartPosition;
		Vector3 vDir2 = m_TargetPosition - future_tm.t;
		vDir.y = 0;
		vDir2.y = 0;

		vDir.Normalize();
		vDir2.Normalize();

		float bt = sqrt((m_TargetPosition.x - m_Position.x)*(m_TargetPosition.x - m_Position.x) + (m_TargetPosition.z - m_Position.z)*(m_TargetPosition.z - m_Position.z));
		// bt�?이동중에 현재 PC�?발바닥을 찍을경우 m_TargetPosition�?m_TargetStartPosition�?갱신되지�?
		// 계산상의 댄轎�?pFrameDelta가 비정상적으로 나오�?경우 무한 달리기를 한다. 이를 막기위하�?추가
		// 1.5f�?경험상의 수치�?클릭�?목적지와 현재�?위치가 1.5f정도만큼 이하�?나오�?멈추�?한다. 1.5 정도�?잘작동한�?

		if( vDir.Dot( vDir2) < 0 || bt < 1.5f)
		{
			m_bTargetMove = FALSE; // 다왔�?

			// 임시 하드 코딩 캐릭터는 -1주면 않됨
			SetAnimation( XiahAniType::eLAT_Stand, m_bObjType != OBJTYPE_PC ? -1 : 0);

			if( m_pParentTrigger[ eXCT_OnEndTargetMove] != NULL)
				m_pParentTrigger[ eXCT_OnEndTargetMove]->Invoke();

			return 1;
		}
	}

	if(m_CollisionEnable == FALSE)
		return 0;

	//////////////////////////////////////////////////////////////////////////
	// MAP 속성처리

	int att_x,att_y;
	unsigned char	att_res;
	att_x = (int)future_tm.t.x;
	att_y = (int)(-future_tm.t.z);
	att_res = XiahMap::g_Map_Attri.Get_Attr(att_x,att_y);

	if(att_res == 0x1)
	{
		sString str;
		str.printf(XIAH_BLOCK);
		g_MainCharInfo.ShowHelpMessage(str);
		g_MainCharInfo.PlayInterfaceSound( ISOUND_WARNING);

		pFrameDelta->t = Vector3();

		if( m_pParentTrigger[ eXCT_OnCollision] != NULL)
			return m_pParentTrigger[ eXCT_OnCollision]->Invoke();
	}


	//////////////////////////////////////////////////////////////////////////
	// 세상�?�?
	if( future_tm.t.x < 5 || -future_tm.t.z < 5 || future_tm.t.x > 2043 || -future_tm.t.z > 2043)
	{
		pFrameDelta->t = Vector3();

		if( m_pParentTrigger[ eXCT_OnCollision] != NULL)
			return m_pParentTrigger[ eXCT_OnCollision]->Invoke();
		return 0;
	}
	
	//////////////////////////////////////////////////////////////////////////
	// 경사�?처리 못올라가�?
	float t_h = Map::g_MapRes.GetHeight(future_tm.t.x,future_tm.t.z);
/*
	sString str;
	str.printf("%f (now:%f) (future:%f)", g_fix,m_Position.y,t_h);
	g_MainCharInfo.ShowHelpMessage( str);
*/
	if(m_Position.y + g_fix < t_h)
	{
		pFrameDelta->t = Vector3();
		if( m_pParentTrigger[ eXCT_OnCollision] != NULL)
			return m_pParentTrigger[ eXCT_OnCollision]->Invoke();
		return 0;
	}
/*
	// 바운�?박스 무시
	if( GetAsyncKeyState( VK_SPACE) < 0)
		return 0;
*/


	// 배경으로 부�?object list�?얻어 온다
	// 2004_04_12 changth : 캐릭터가 움직일�?속해 있는 맵셀�?오브젝트�?충돌 검사를 하는�?
	// 이때 문제가 발생�? 맵셀 사이�?끼어 있는 오브젝트�?경우, �?에디터에�?저장될�?
	// 어떤 맵셀�?저장되는지 알수가 없다. 그래�?캐릭터가 속해있는 맵셀�?경계 부분에 있으�?
	// 인접�?맵셀�?오브젝트까지 충돌 검사를 한다. 그래�?정확하다.

	// 일단 캐릭터가 맵셀�?가운데 부분에 있는지, 경계 부분에 있는지 검사하�?
	// 검사할 �?셀�?개수�?알아 낸다. 참고�? 맵셀�?크기�?256x256이다.
	int nMapCellCount = 1;
	WORD wXAry[4];	// 최대�?4개다. 현재 있는 곳을 포함하여
	WORD wZAry[4];

	// 현재 속해 있는 �?
	wXAry[0] = (WORD)future_tm.t.x;
	wZAry[0] = (WORD)(-future_tm.t.z);

	int nRemnantX = wXAry[0] % 256;
	int nRemnantZ = wZAry[0] % 256;

	int nSearchGap = 50;
	if( nRemnantX < nSearchGap )	// 왼쪽�?있는 맵셀�?추가된다.
	{
		nMapCellCount++;

		wXAry[1] = (WORD)(future_tm.t.x - nRemnantX - 2);
		wZAry[1] = (WORD)(-future_tm.t.z);

		if( nRemnantZ >= nSearchGap && nRemnantZ <= 256-nSearchGap )	// �? 아래�?검사가 필요 없다.
		{
		}
		else
		{
			if( nRemnantZ < nSearchGap )	// 위의 맵셀�?추가된다.
			{
				nMapCellCount++;

				wXAry[2] = (WORD)future_tm.t.x;
				wZAry[2] = (WORD)(-future_tm.t.z - nRemnantZ - 2);
			}
			else
			if( nRemnantZ > 256-nSearchGap )	// 아래�?맵셀�?추가된다.
			{
				nMapCellCount++;

				int nZ = 256 - nRemnantZ;

				wXAry[2] = (WORD)future_tm.t.x;
				wZAry[2] = (WORD)(-future_tm.t.z + nZ + 2);
			}
		}

		if( nMapCellCount == 3 )	// 대각선쪽으로도 추가한다.
		{
			nMapCellCount++;

			wXAry[3] = wXAry[1];
			wZAry[3] = wZAry[2];
		}
	}// if
	else
	if( nRemnantX > 256-nSearchGap )	// 오른쪽에 있는 맵셀�?추가된다.
	{
		nMapCellCount++;

		int nX = 256 - nRemnantX;

		wXAry[1] = (WORD)(future_tm.t.x + nX + 2);
		wZAry[1] = (WORD)(-future_tm.t.z);

		if( nRemnantZ >= nSearchGap && nRemnantZ <= 256-nSearchGap )	// �? 아래�?검사가 필요 없다.
		{
		}
		else
		{
			if( nRemnantZ < nSearchGap )	// 위의 맵셀�?추가된다.
			{
				nMapCellCount++;

				wXAry[2] = (WORD)future_tm.t.x;
				wZAry[2] = (WORD)(-future_tm.t.z - nRemnantZ - 2);
			}
			else
			if( nRemnantZ > 256-nSearchGap )	// 아래�?맵셀�?추가된다.
			{
				nMapCellCount++;

				int nZ = 256 - nRemnantZ;

				wXAry[2] = (WORD)future_tm.t.x;
				wZAry[2] = (WORD)(-future_tm.t.z + nZ + 2);
			}
		}

		if( nMapCellCount == 3 )	// 대각선쪽으로도 추가한다.
		{
			nMapCellCount++;

			wXAry[3] = wXAry[1];
			wZAry[3] = wZAry[2];
		}
	}// if

	// Z 축도 검�? �?녀석은 �? 아래�?검사하�?된다.
	if( nMapCellCount == 1 && nRemnantZ < nSearchGap )	// 위의 맵셀�?추가된다.
	{
		nMapCellCount++;

		wXAry[1] = (WORD)future_tm.t.x;
		wZAry[1] = (WORD)(-future_tm.t.z - nRemnantZ - 2);
	}
	else
	if( nMapCellCount == 1 && nRemnantZ > 256-nSearchGap )	// 아래�?맵셀�?추가된다.
	{
		nMapCellCount++;

		int nZ = 256 - nRemnantZ;

		wXAry[1] = (WORD)future_tm.t.x;
		wZAry[1] = (WORD)(-future_tm.t.z + nZ + 2);
	}

	// 이제 리스트에 들어 있는 맵셀�?오브젝트와 충돌 검사를 한다.
	for(int i=0; i<nMapCellCount; i++)
	{
		XiahGameEngine::Map::MAPRENDER_MAPOBJECTLIST *pObjectList;
		XiahGameEngine::Map::MAPRENDER_MAPOBJECTLIST::iterator it;

		if( wXAry[i] < 0 || wZAry[i] < 0 || 
			wXAry[i] > XiahMap::g_XiahMap.m_MapInfo.m_wWidth || 
			wZAry[i] > XiahMap::g_XiahMap.m_MapInfo.m_wHeight ) continue;

		if( !XiahMap::g_XiahMap.m_pMapRender->QueryMeshblockObjectList( wXAry[i], wZAry[i], &pObjectList ) )
			continue;

		//
		float collide_height = 0;
		BOOL bCollide = FALSE;
		BOOL bFirstCollide = TRUE;
		for(it = pObjectList->begin(); it != pObjectList->end() && !bCollide; it++)
		{
			XiahGameEngine::Map::CMapObjectRender *pObject = *it;

			int box_count;
			BBoxOBB3* box_list;

			box_count = pObject->GetCollideBoxCount();
			box_list = pObject->GetCollideBoxList();

			for(int i = 0; i < box_count; i++)
			{
				BBoxOBB3* map_object_bound = box_list + i;

				// 원래 코드가 이랬는데..
				if( !map_object_bound->m_BBoxAABB.Intersect( future_bound.m_BBoxAABB))
					continue;

				// 원래 코드가 이것�?체크�?하는�? 현재�?이게 없어�?�?
//				if( !map_object_bound->IsIntersect( &future_bound))
//					continue;

				// 다리 주위�?있을때에�?두개�?오브젝트�?검사해�?하므�?한번 �?검사한�?
				if( !bFirstCollide )
					bCollide = TRUE;

				if( collide_height < map_object_bound->m_HeightMax.y)
				{
					collide_height = map_object_bound->m_HeightMax.y;
					bFirstCollide = FALSE;
				}
			}// for
		}

		if( collide_height != 0.0f ) // 한번 검�?했을때도 역시 충돌 �?상태�?
			bCollide = TRUE;

		m_bCollide = bCollide;

		// 충돌!
		if( bCollide)
		{
			// 충돌되었는데 충돌된얘보다 �?위에 서있는경�?-_-;;
			if( future_tm.t.y > collide_height)
			{
				// �?경우�?내려가�?한다.
				m_ObjectTM.y = collide_height;
				m_fColHeight = collide_height;
				m_bRide = TRUE;
			}
			else 
			if( future_tm.t.y < collide_height)		// 바운�?박스 height보다 현재 위치가 작은가?
			{
				if( abs( future_tm.t.y - collide_height) < 4)
				{
					// 올라가 버림
					m_ObjectTM.y = collide_height;
					m_fColHeight = collide_height;	// 임시저�?바운�?높이
					m_bRide = TRUE;
				}
				else
				{
					//				m_bRide = FALSE;
					pFrameDelta->t = Vector3();
					// 더이�?�?�?없음�?알려 준�?
					if( m_pParentTrigger[ eXCT_OnCollision] != NULL)
						return m_pParentTrigger[ eXCT_OnCollision]->Invoke();
				}
			}
			else
				m_bRide = FALSE;

			break;
		}
		else
			m_bRide = FALSE;

	}// for


	// 2004_06_01 Changth
	// 문파 비석�?충돌 처리�?넣어준�?
	BBoxOBB3 StoneBound;

	DWORDLIST::iterator dit;
	for(dit=g_MainCharInfo.m_MunpaStonIDList.begin(); dit!=g_MainCharInfo.m_MunpaStonIDList.end(); dit++)
	{
		DWORD dwStoneID = *dit;

        XiahObject::CXiahObject* pFindObject = XiahObject::g_XiahObjectManager.FindXiahObject( MAKEOBJECTID(0, dwStoneID, OBJTYPE_FUNCTIONALNPC) );
		if( pFindObject == NULL ) continue;

		CXiahCharObject* pStoneChar = (CXiahCharObject*)pFindObject->m_pObject;

		StoneBound = BBoxOBB3( pStoneChar->m_LocalBound, pStoneChar->m_ObjectTM );

		if( StoneBound.m_BBoxAABB.Intersect( future_bound.m_BBoxAABB ) )
		{
			pFrameDelta->t = Vector3();
			if( m_pParentTrigger[ eXCT_OnCollision] != NULL)
				return m_pParentTrigger[ eXCT_OnCollision]->Invoke();
		}
	}

	return 0;
}

int CXiahCharObject::OnTimer(unsigned long type)
{
	if( m_pParentTrigger[ eXCT_OnTimer] != NULL)
	{
		m_pParentTrigger[ eXCT_OnTimer]->m_nParam = type;
		return m_pParentTrigger[ eXCT_OnTimer]->Invoke();
	}

	// 아~~ 결국 하드코딩이구�?
	if( m_bObjType == OBJTYPE_NPC)
	{
		if( type == 0)
		{
			AttachChildCharRender( eLBP_RightHand,	837, 0, 0);

			m_ChildChar[ eLBP_RightHand].SetLocalAngle( 0);
		}
		else if( type == 1)
		{
			if( m_ChildChar[ eLBP_RightHand].IsValid())
			{
			
				Matrix4x4 *pTM = m_CharRender.GetChildBoneMatrix( eLBP_RightHand);
				
				CXiahCharObject* pObject = new CXiahCharObject;

#ifdef TRACE_LOG
				if(pTM == NULL || pObject == NULL)
				{
					DBG_LogFile( _T("CXiahCharObject::OnTimer 실패"));
				}
#endif
				pObject->Create( 837, 0, 0, 0);
				pObject->m_bGravityEnable = FALSE;
				pObject->m_Position = pTM->t;
				pObject->SetTargetMove( m_ShotAttackInfo.wDesPosX, m_ShotAttackInfo.wDesPosY, eLBP_Arrow, m_ShotAttackInfo.wLifeTime / 2);
				pObject->m_TargetObjectID = m_ShotAttackInfo.dwTargetID;
				pObject->m_TargetObjectType = m_ShotAttackInfo.dwObjType;
				//pObject->m_fTargetMoveSpeed = 85.0f * 33.0f / 1000.0f;
				pObject->m_bObjType = OBJTYPE_ARROW;
				pObject->Update(TRUE);
				
				pObject->m_CharRender.EnableBoneAnimation( FALSE);
//				rotM.SetRotationY( _PI);
				Matrix4x4 DummyTM;
				pObject->m_CharRender.SetLocalCenter(DummyTM);
				//pObject->m_CharRender.SetLocalCenter(Matrix4x4());
				RemoveChildCharRender( eLBP_RightHand);
			
				// 화살�?쏜다
				XiahObject::g_XiahObjectManager.CreateXiahObject( 0, 0, pObject);

				// 화살 궤적
				if( pObject->m_pLineParticle == NULL )
				{
					pObject->m_pLineParticle = new CLineParticle;

#ifdef TRACE_LOG
					if(pObject->m_pLineParticle == NULL)
					{
						DBG_LogFile( _T("CXiahCharObject::OnTimer 실패"));
					}
#endif
					pObject->m_pLineParticle->Init();
					pObject->m_pLineParticle->Start( pObject->m_Position );
				}
			}
		}
	}


	return 0;
}

BOOL CXiahCharObject::EnableGlowEffect(BOOL bTrue,D3DCOLOR color)
{
	if( m_bGlowEnable == bTrue && color == m_GlowColor)
		return TRUE;

	m_bGlowEnable = bTrue;
	m_GlowColor = color;

	m_CharRender.EnableGlowEffect( bTrue,color);

	for(int i = 0; i < LOGICAL_BONE_POS_COUNT; i++)
	{
		m_ChildChar[ i].EnableGlowEffect( bTrue,color);
		m_ChildChar[ i].StartAlphaEffect();
	}

	return TRUE;
}

float CXiahCharObject::GetInteractionDistance(Vector3 pos)
{
	float fDistance = GetDistance( pos);

	if( m_CharRender.IsValid())
	{
		Vector3 vSize = m_LocalBound.Size();

		vSize.y = 0;

		fDistance -= vSize.GetLength() / 2;
	}

	return fDistance;
}

void CXiahCharObject::RefreshFameColor(DWORD dwFame)
{
	if(dwFame != -99)
		m_dwFame = dwFame;

	if(m_dwFame >= 127)
	{   // 노락�?계열
		if(m_dwFame >= 133 && m_dwFame <= 226) // 선인 2단계
		{
			m_cNameColor = D3DCOLOR_XRGB( 255, 255, 120);
		}
		else if(m_dwFame >= 227) // 선인 3단계
		{
			m_cNameColor = D3DCOLOR_XRGB( 255, 255, 0);
		}
		else  // 선인 1단계
		{
			m_cNameColor = D3DCOLOR_XRGB( 255, 255, 200);
		}
	} // if(dwFame >= 127)
	else
	{
		// 빨간�?계열
		if(m_dwFame <= 121 && m_dwFame >= 28) // 악인 2단계
		{
			m_cNameColor = D3DCOLOR_XRGB( 255, 75, 75);
		}
		else if(m_dwFame <= 27) // 악인 3단계
		{
			m_cNameColor = D3DCOLOR_XRGB( 255, 0, 0);
		}
		else  // 악인 1단계
		{
			m_cNameColor = D3DCOLOR_XRGB( 255, 150, 150);
		}
	}

	// Update active dynamic title when fame changes
	int nNewTitleID = GetTitleIDByFame(m_dwFame);
	if (nNewTitleID != m_nActiveTitleID)
	{
		m_nActiveTitleID = nNewTitleID;
		LoadLegendTitle(nNewTitleID);
	}
	
}

BOOL CheckServerInteractionDistance(WORD wPosX,WORD wPosY)
{
	if( g_pMainChar == NULL)
		return TRUE;

	CXiahCharObject* pCharObject = (CXiahCharObject*)g_pMainChar->m_pObject;

	//YS_0811 : BUGFIX
	if ( pCharObject == NULL )
	{		
		return false;
	}

	float fDistance = pCharObject->GetDistance( wPosX, wPosY);

	if( fDistance > SERVER_INTERACTION_DISTANCE)
		return FALSE;
	
	return TRUE;
}

BOOL ValidateObject(BYTE bObjType,DWORD ObjID,WORD wPosX,WORD wPosY)
{
	if( !CheckServerInteractionDistance( wPosX, wPosY))
		return FALSE;

	switch( bObjType)
	{
	case OBJTYPE_PC:
		SendCS_IT_CHARINFO_REQ( ObjID);
		break;
	case OBJTYPE_NPC:	// NPC가 NPC�?떄려?
		SendCS_NC_NPCINFO_REQ( ObjID);
		break;
	case OBJTYPE_PET:
		SendCS_NC_PETINFO_REQ( ObjID);
		break;
	default:
		//DBG_Assert( FALSE);
		break;
	}

	return TRUE;
}


void CXiahCharObject::ClearTitle()
{
	for (size_t i = 0; i < m_TitleEffect.m_vecFrames.size(); i++)
	{
		if (m_TitleEffect.m_vecFrames[i].pTexture)
		{
			m_TitleEffect.m_vecFrames[i].pTexture->Release();
			m_TitleEffect.m_vecFrames[i].pTexture = NULL;
		}
	}
	m_TitleEffect.m_vecFrames.clear();
	m_TitleEffect.m_bLoaded = FALSE;
}

BOOL CXiahCharObject::LoadLegendTitle(int nTitleID)
{
	ClearTitle();

	m_TitleEffect.m_nCurrentFrame = 0;
	m_TitleEffect.m_dwLastTime = GetTickCount();

	int nFrameIdx = 0;
	while (TRUE)
	{
		char szImgPath[MAX_PATH];
		char szTxtPath[MAX_PATH];
		
		sprintf(szImgPath, "fame\\%d\\%06d.png", nTitleID, nFrameIdx);
		sprintf(szTxtPath, "fame\\%d\\Placements\\%06d.txt", nTitleID, nFrameIdx);

		// Try loading texture
		LPDIRECT3DTEXTURE9 pTexture = NULL;
		HRESULT hr = D3DXCreateTextureFromFileExA(
			g_pDirect3DDevice, 
			szImgPath, 
			D3DX_DEFAULT, D3DX_DEFAULT, 1, 0, 
			D3DFMT_A8R8G8B8, D3DPOOL_MANAGED, 
			D3DX_FILTER_NONE, D3DX_FILTER_NONE, 
			0, NULL, NULL, &pTexture
		);

		if (FAILED(hr) || pTexture == NULL)
		{
			break;
		}

		// Read placements
		int nOffX = 0;
		int nOffY = 0;
		FILE* fp = fopen(szTxtPath, "r");
		if (fp)
		{
			if (fscanf(fp, "%d\n%d", &nOffX, &nOffY) != 2)
			{
				nOffX = 0;
				nOffY = 0;
			}
			fclose(fp);
		}

		sTitleFrame newFrame;
		newFrame.pTexture = pTexture;
		newFrame.nOffsetX = nOffX;
		newFrame.nOffsetY = nOffY;

		m_TitleEffect.m_vecFrames.push_back(newFrame);
		nFrameIdx++;
	}

	if (m_TitleEffect.m_vecFrames.size() > 0)
	{
		m_TitleEffect.m_bLoaded = TRUE;
		return TRUE;
	}
	
	return FALSE;
}

void CXiahCharObject::RenderLegendTitle(int nNameX, int nNameY)
{
	if (!m_TitleEffect.m_bLoaded || m_TitleEffect.m_vecFrames.empty())
		return;

	DWORD dwCurTime = GetTickCount();
	if (dwCurTime - m_TitleEffect.m_dwLastTime >= 100)
	{
		m_TitleEffect.m_nCurrentFrame = (m_TitleEffect.m_nCurrentFrame + 1) % m_TitleEffect.m_vecFrames.size();
		m_TitleEffect.m_dwLastTime = dwCurTime;
	}

	sTitleFrame& curFrame = m_TitleEffect.m_vecFrames[m_TitleEffect.m_nCurrentFrame];
	LPDIRECT3DTEXTURE9 pTexture = curFrame.pTexture;
	if (!pTexture) return;

	D3DSURFACE_DESC desc;
	pTexture->GetLevelDesc(0, &desc);
	float fWidth  = (float)desc.Width;
	float fHeight = (float)desc.Height;

	float fX = (float)nNameX + (float)curFrame.nOffsetX - 0.5f;
	float fY = (float)nNameY + (float)curFrame.nOffsetY - 0.5f;

	VT_TLVertex Vertex[4];
	
	Vertex[0].pos = Vector4(fX,           fY,           0.0f, 1.0f); Vertex[0].tex = Vector2(0.0f, 0.0f);
	Vertex[1].pos = Vector4(fX + fWidth,  fY,           0.0f, 1.0f); Vertex[1].tex = Vector2(1.0f, 0.0f);
	Vertex[2].pos = Vector4(fX,           fY + fHeight, 0.0f, 1.0f); Vertex[2].tex = Vector2(0.0f, 1.0f);
	Vertex[3].pos = Vector4(fX + fWidth,  fY + fHeight, 0.0f, 1.0f); Vertex[3].tex = Vector2(1.0f, 1.0f);

	D3DCOLOR d3dColor = D3DCOLOR_ARGB(255, 255, 255, 255);
	Vertex[0].diffuse = Vertex[1].diffuse = Vertex[2].diffuse = Vertex[3].diffuse = d3dColor;

	g_pDirect3DDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	g_pDirect3DDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_NOTEQUAL);
	g_pDirect3DDevice->SetRenderState(D3DRS_ALPHAREF, 0);

	g_Device.SetTexture(0, pTexture);
	g_Device.SetFVF(D3DFVF_TLVERTEX);
	
	g_pDirect3DDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, 2, Vertex, sizeof(VT_TLVertex));
}


int CXiahCharObject::GetTitleIDByFame(DWORD dwFame)
{
	if (dwFame == 127)
	{
		return 19; // Neutral
	}
	else if (dwFame > 127)
	{
		// Good (Hero) grades: 20 to 28
		if (dwFame >= 128 && dwFame <= 132) return 20;
		if (dwFame >= 133 && dwFame <= 150) return 21;
		if (dwFame >= 151 && dwFame <= 170) return 22;
		if (dwFame >= 171 && dwFame <= 190) return 23;
		if (dwFame >= 191 && dwFame <= 210) return 24;
		if (dwFame >= 211 && dwFame <= 226) return 25;
		if (dwFame >= 227 && dwFame <= 500) return 26;
		if (dwFame >= 501 && dwFame <= 1000) return 27;
		return 28; // >= 1001
	}
	else
	{
		// Evil (Villain) grades: 10 to 18
		if (dwFame >= 122 && dwFame <= 126) return 18;
		if (dwFame >= 100 && dwFame <= 121) return 17;
		if (dwFame >= 80 && dwFame <= 99) return 16;
		if (dwFame >= 60 && dwFame <= 79) return 15;
		if (dwFame >= 45 && dwFame <= 59) return 14;
		if (dwFame >= 28 && dwFame <= 44) return 13;
		if (dwFame >= 15 && dwFame <= 27) return 12;
		if (dwFame >= 5 && dwFame <= 14) return 11;
		return 10; // <= 4
	}
}
