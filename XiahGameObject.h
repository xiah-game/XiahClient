#pragma once

#include "XiahObject.h"

#include "XiahCharAniType.h"

#define GetServerAngle(angle) ((((short)(90 - ((angle) * 180.0f / _PI))) + 360) % 360)
#define GetClientAngle(angle) ((90.0f - (float)(angle)) * _PI / 180.0f)

enum eLogicalBonePos
{
	eLBP_Head		= 1,
	eLBP_LeftHand	= 2,
	eLBP_RightHand	= 3,
	eLBP_Shoe		= 4,
	eLBP_Protector	= 5
};

enum eTargetMoveType
{
	eLBP_CharNavigation,
	eLBP_Arrow,

};

#define LOGICAL_BONE_POS_COUNT 4

class CXiah3DObject : public XiahObject::CXiahObject_Basic
{
public:
	CXiah3DObject();
	virtual ~CXiah3DObject();

	//YS_0728 : BUGFIX
	virtual void InitClass();
	virtual void DeleteClass();

public:
	Vector3		m_Position;
	float		m_Angle;
	float		m_LocalAngle;

	Vector3		m_SyncPosition;
	
	BOOL		m_bTargetMove;
	Vector3		m_TargetPosition;
	Vector3		m_TargetStartPosition;
	DWORD		m_TargetObjectID;		// 유도 화살?
	DWORD		m_TargetObjectType;
	int			m_nTargetMoveType;
	float		m_fTargetMoveSpeed;
	DWORD		m_TargetStartTime;
	DWORD		m_TargetLifeTime;
	Vector3		m_TargetDir;

	BBoxAABB3	m_LocalBound;
	BBoxOBB3	m_WorldBound;

	Matrix4x4	m_ObjectTM;
	CText2D		m_tObjectName;

	//---------------
	BOOL		m_bShowObjectName;
	sRect		m_rcObjectScreenPos;	// 객체의 화면 위치
	sRect		m_rcObjectScreenPos2;	// chatbox의 화면 위치

	DWORD		m_LastNavigationTime;

	//--------------
	BOOL		m_CollisionEnable;	// 충돌처리를 해줘야 하는 넘인가?
	BOOL		m_bCollide;
	BOOL		m_bRide;			// 현재 바운딩 박스 위에 올라가 있나?
	float		m_fColHeight;		// 바운딩 박스의 높이

	//--------------
	BOOL		m_bGravityEnable;

public:
	// 서버 Interaction을 위한 함수들
	virtual BOOL SetAngle(WORD angle);
	virtual BOOL SetAngleTarget(Vector3 position);
	virtual BOOL SetAngleTarget(WORD wPosX,WORD wPosY);
	virtual BOOL SetAngleTarget(CXiah3DObject* pObject);
	virtual BOOL SetPosition(WORD wPosX,WORD wPosY);
	virtual BOOL SetTargetPosition(WORD wPosX,WORD wPosY);
	virtual BOOL SetTargetMove( WORD wPosX, WORD wPosY,int Type,int lifetime);
	
	virtual BOOL GetPosition(WORD &wPosX,WORD &wPosY);
	virtual BOOL GetAngle(WORD &angle);
	virtual Vector3 GetAngle_Vector();	// 현재 각도를 방향 벡터로 바꿔준다

	// 현재 위치와 특정 위치와의 거리 -> 항상 양수
	virtual float GetDistance(WORD wPosX,WORD wPosY);
	virtual float GetDistance(Vector3 pos); // 3d상의 거리 y값 생각않함
	// 진행 방향상에서 현재 위치보다 뒤에 있을경우엔는 음수 리턴
	virtual float GetDistance_Normal(WORD wPosX,WORD wPosY);
	// Sync Move 위치값을 넣어서, 현재 속도를 얼마나 더 빠르게 해야 되는지를 알아냄
	virtual float GetSyncMoveScale(WORD wPosX,WORD wPosY);
	//
	virtual WORD GetTargetAngle(Vector3 pos);

	virtual BOOL UpdateTM();
	virtual BOOL UpdateTargetMove(){ return TRUE;}
	virtual float GetHeight(float x,float z);

	// 특정 위치까지만 이동

};

//------------------------------------------------------------------------------
//

// 2004_06_17 Changth 외공 아이디를 클라이언트도 저장하고 있자.
#define OUTGONGID_MUSUHON			32
#define OUTGONGID_POKSAHON			36
#define OUTGONGID_KUMKANGLUK		39
#define	OUTGONGID_ILYUIDOGANG		34
#define	OUTGONGID_UNKIHAENG			30
#define	OUTGONGID_BUSIN				41

#define OUTGONGID_JUNYUUM			63
#define OUTGONGID_YUNOYUNG			65
#define OUTGONGID_W0NKISINKANG		69
#define OUTGONGID_KYOKANSU			68
#define	OUTGONGID_YUESUSINYUNG		64
#define OUTGONGID_JOSIKSUL			60
#define	OUTGONGID_WHANSUYUO			71

#define OUTGONGID_PACHUNSO			92
#define OUTGONGID_MARYULKAK			95
#define OUTGONGID_AMHUKMU			96
#define OUTGONGID_TALBAKIN			98
#define OUTGONGID_JUKUNKANGKI		99
#define OUTGONGID_KUMNASU			100
#define OUTGONGID_BANTANKANGKI		101
#define	OUTGONGID_JILPUNGBO			94
#define	OUTGONGID_KIYOESUL			90

#define	OUTGONGID_ODOKCHIM			122
#define OUTGONGID_CHOSANGBI			124
#define	OUTGONGID_DOKNAEGONG		125
#define	OUTGONGID_DOKHYULGONG		126
#define	OUTGONGID_DOKMU				127
#define	OUTGONGID_SSANGDOSU			128
#define	OUTGONGID_MANDOKBULJIN		129
#define	OUTGONGID_GYUISIKDAEBUB		130
#define	OUTGONGID_GWANGMADOKGONG	131

// 오행
#define FIVEELEMENT_FIRE			150
#define FIVEELEMENT_WATER			151
#define FIVEELEMENT_TREE			152
#define FIVEELEMENT_METAL			153
#define FIVEELEMENT_EARTH			154

// 각성 
#define SUNSINGONG					161
#define SUNMUSUL					162
#define SUNGAPSUL					163

// 각성 외공
#define WHA_DRAGONSINJANG			164
#define BING_DRAGONSINJANG			165
#define DOK_DRAGONSINJANG			166
#define NOI_DRAGONSINJANG			167

#define WHA_DRAGONSUNGCHEON			168
#define BING_DRAGONSUNGCHEON		169
#define DOK_DRAGONSUNGCHEON			170
#define NOI_DRAGONSUNGCHEON			171


// 각성 유파 특화 외공
#define GUM_ILKICHAM				172
#define GUM_JINBUNSIN				173
#define YUN_SAJANGSINGONG			174
#define YUN_SUSINKIKANG				175
#define MU_KANGKIPOKWON				176
#define MU_KIHUBKANGKI				177
#define YA_EUNSINSUL				178
#define YA_HOJUNGKANGKI				179


// HT_0525 각성 무공 외공 ID 사용안함. 횅땍후 지우자 
#define OUTGONGID_DRAGONSINJANG		180
#define OUTGONGID_DRAGONSUNGCHEON	181

//HT_0711 : 진각성 무공
#define REBRITH_KUMKANGSINGONG		191
#define REBRITH_BUSNSINGONG			192

#define REBRITH_W0NKISINGONG		193
#define REBRITH_KYOKANSINGONG		194

#define REBRITH_KUMNASINGONG		195
#define REBRITH_MARYUNGSINGONG		196

#define REBRITH_GWANGMASINGONG		197
#define REBRITH_DOKHYULSINGONG		198





#define	GWANGMADOKGONG_CHARSCALE		1.2f
#define	GWANGMADOKGONG_MATERIAL_R		0
#define	GWANGMADOKGONG_MATERIAL_G		0.5f
#define	GWANGMADOKGONG_MATERIAL_B		0.25f

// HT_0525 각성 무공 대상 이펙트 사용 안함. 나중에 확정되면 지운다.
#define WHA_DRAGON_MATERIAL_R		1.0f	//붉은색
#define WHA_DRAGON_MATERIAL_G		0.5f
#define WHA_DRAGON_MATERIAL_B		0.0f

#define BING_DRAGON_MATERIAL_R		0.5f	//파란색
#define BING_DRAGON_MATERIAL_G		0.75f
#define BING_DRAGON_MATERIAL_B		1.0f

#define DOK_DRAGON_MATERIAL_R		0.25f	//연두색
#define DOK_DRAGON_MATERIAL_G		0.75f
#define DOK_DRAGON_MATERIAL_B		0.25f

#define NOI_DRAGOND_MATERIAL_R	0.5f	//연보라색
#define NOI_DRAGOND_MATERIAL_G	0.5f	
#define NOI_DRAGOND_MATERIAL_B	1.0f	

enum eXiahChar_Trigger
{
	eXCT_OnCollision,
	eXCT_OnTimer,
	eXCT_OnEndTargetMove,
	eXCT_Count
};

struct sShotAttackInfo
{
	WORD wDesPosX;
	WORD wDesPosY;
	WORD bDesHeight;
	WORD wLifeTime;
	DWORD dwTargetID;
	BYTE  dwObjType;
};

struct sKeepUpMugong
{
	DWORD dwMugongID;
	BYTE  bLevel;
	DWORD dwTime;

/*	DWORD dwTime2;*/
};

class CKeepupMugongList : public hash_map<DWORD,sKeepUpMugong>
{
public:
	BOOL Add(DWORD dwMugongID,BYTE bLevel,DWORD dwTime)
	{
		if( IsExist( dwMugongID))
			return TRUE;

		sKeepUpMugong mugong;

		mugong.dwMugongID = dwMugongID;
		mugong.bLevel = bLevel;
		mugong.dwTime = dwTime;

		insert( value_type( dwMugongID, mugong));

		return TRUE;
	}
	
	BOOL Delete(DWORD dwMugongID)
	{
		iterator it = find( dwMugongID);

		if( it == end())
			return FALSE;

		erase( it);

		return TRUE;
	}
	
	BOOL IsExist(DWORD dwMugongID)
	{
		iterator it = find( dwMugongID);

		return it != end();
	}

	DWORD GetTime(DWORD dwMugongID)
	{
		iterator it = find( dwMugongID );

		if( it != end() )
		{
			sKeepUpMugong mugong = it->second;

			return mugong.dwTime;
		}

		return 0;
	}
};


//////////////////////////////////////////////////////////////////////////////////////////

#define ADD_CHARRENDER_TRIGGER(event, func) \
	pTrigger = new CClassTrigger<CXiahCharObject>(this, &CXiahCharObject::func, 0);\
	m_TriggerList.push_back( pTrigger);\
	m_CharRender.SetTrigger( event, pTrigger);

class CXiahCharObject : public CXiah3DObject
{
public:

	CXiahCharObject();
	virtual ~CXiahCharObject();

	//YS_0728 : BUGFIX
	virtual void InitClass();
	virtual void DeleteClass();

	virtual BOOL Create(int nCharID,int nMeshType,int nTextureType,int nAniType);
	virtual BOOL Release();

	virtual BOOL ClearMugongEffect();

	virtual BOOL Update(BOOL bVisible = FALSE);
	virtual BOOL Render();
	//virtual BOOL ShadowRender();

	virtual int GetAnimation();		// Get current animation
	virtual BOOL SetAnimation(int nCurMotionType,int nNextMotionType,int nCurIndex,int nNextIndex,float fAnimationSpeed = 1.0f);
	virtual BOOL SetAnimation(int nMotionType,int nIndex,float fAnimationSpeed = 1.0f);

	// 반드시 m_CharRender가 Create된 후에 사용할것
	virtual BOOL AttachChildCharRender(int nLogicalPos,int nCharID,int nMeshType,int nTextureType, int nEffectIndex=-1);
	virtual bool AttachPetChildChar(int nLogicalPos, int nCharID, int nMeshType, int nTextureType);
	virtual BOOL RemoveChildCharRender(int nLogicalPos);

	virtual BOOL UpdateTargetMove();

	virtual float GetInteractionDistance(Vector3 pos);	// 별것 없음 MeshSize더해줌
	BOOL	EnableGlowEffect(BOOL bTrue,D3DCOLOR color = D3DCOLOR_XRGB( 255, 255, 128));

	void RefreshFameColor(DWORD dwFame = -99);

	void SetChatBox( DWORD dwTimeInterval, sString ChatMsg, BYTE byChatType);
	void ShowChatBox();

public:

	// 트리거 하위 CCharRender에 대응하는 트리거
	virtual int OnEndAnimation(unsigned long);	// Animation이 끝났다
	virtual int OnEndAlphaEffect(unsigned long);
	virtual int OnTMUpdate(unsigned long);
	virtual int OnTimer(unsigned long);

public:

	CCharRender												m_CharRender;
	XiahAniType::CXiahChar_LogicalAnimationType*			m_pAniType;

	int	m_nCurMotionType;
	int m_nNextMotionType;

	int m_nCurAniType;
	int m_nNextAniType;

	int	m_nCurAniIndex;
	int m_nNextAniIndex;

	BOOL	m_bRotatable;	// 회전가능한 OBJECT인가?? (파괴 퀘스트용 NPC는 맞아도 회전하지 않는다.)
	BOOL	m_bMoveable;	// 움직임이 가능한가?
	BOOL	m_bAttack;		// 공격 동작중인가?
	BYTE	m_bAttackType;	// 어떤 공격인가? 무공? 일반

	bool	m_bRenderOK;
	bool	m_bCallRelease;

	D3DCOLOR	m_cGageColor;
	D3DCOLOR	m_cNameColor;

	int			 m_nChildChar;
	CCharRender  m_ChildChar[ LOGICAL_BONE_POS_COUNT];	// 젠장 포인터로 동적할당 할려고 햇는데 DLL때문인가?? 않되네 T_T

	XiahGameEngine::Map::CMapDecal	m_Shadow;
	sShotAttackInfo	m_ShotAttackInfo;	// 일단 NPC용이닷

	// 효과를 위해
	CKeepupMugongList	m_KeepUpMugongList;

	// 무공 지속 이펙트
	// 검영
	_EFFECTPACKAGEPAIR*		m_pMusuhonEffectPP;
	_EFFECTPACKAGEPAIR*		m_pPoksahonEffectPP;
	_EFFECTPACKAGEPAIR*		m_pKuymgangrukEffectPP;
	// 연랑
	_EFFECTPACKAGEPAIR*		m_pYuenoyuengEffectPP;
	_EFFECTPACKAGEPAIR*		m_pKyugamsuEffectPP;
	_EFFECTPACKAGEPAIR*		m_pWonkisingangEffectPP;
	_EFFECTPACKAGEPAIR*		m_pSajangsingongEffectPP;
	_EFFECTPACKAGEPAIR*		m_pSusinkikangEffectPP;

	// 무투.
	_EFFECTPACKAGEPAIR*		m_pPachunsoEffectPP;
	_EFFECTPACKAGEPAIR*		m_pMarulkakEffectPP;
	_EFFECTPACKAGEPAIR*		m_pAmhukmuEffectPP;
	_EFFECTPACKAGEPAIR*		m_pTalbacinEffectPP;
	_EFFECTPACKAGEPAIR*		m_pJukwonkangkiEffectPP;
	_EFFECTPACKAGEPAIR*		m_pKumnasuEffectPP;
	_EFFECTPACKAGEPAIR*		m_pBantankangkiEffectPP;
	_EFFECTPACKAGEPAIR*		m_pKihubkangkiEffectPP;
	// 야차
	_EFFECTPACKAGEPAIR*		m_pOdokchimEffectPP;
	_EFFECTPACKAGEPAIR*		m_pDokhyulgongEffectPP;
	_EFFECTPACKAGEPAIR*		m_pDoknaegongEffectPP;
	_EFFECTPACKAGEPAIR*		m_pSsangdosuEffectPP;
	_EFFECTPACKAGEPAIR*		m_pMandokbuljinEffectPP;
	_EFFECTPACKAGEPAIR*		m_pDokmuEffectPP;
	_EFFECTPACKAGEPAIR*		m_pEunsinsulEffectPP;
	_EFFECTPACKAGEPAIR*		m_pHojungkangkiEffectPP;

    _EFFECTPACKAGEPAIR*		m_pGyungGongEffectPP;

	// 각성 지속 이펙트
	_EFFECTPACKAGEPAIR*		m_pWha_DragonPP;
	_EFFECTPACKAGEPAIR*		m_pBing_DragonPP;
	_EFFECTPACKAGEPAIR*		m_pDok_DragonPP;
	_EFFECTPACKAGEPAIR*		m_pNoi_DragonPP;

	//HT_0711 : 진각성 무공
	_EFFECTPACKAGEPAIR*		m_pKuymgangsingongEffectPP;		// 흡성 신공 금강신공
//	_EFFECTPACKAGEPAIR*		m_pBunsinsingongEffectPP;		// 흡성 신공 분신신공
	_EFFECTPACKAGEPAIR*		m_pWonkisingongEffectPP;		// 흡성 신공 원기신공
	_EFFECTPACKAGEPAIR*		m_pKyugamsingongEffectPP;		// 흡성 신공 교감신공	
	_EFFECTPACKAGEPAIR*		m_pKumnasingongEffectPP;		// 흡성 신공 금나신공
	_EFFECTPACKAGEPAIR*		m_pMarulsingongEffectPP;		// 흡성 신공 마령신공
//	_EFFECTPACKAGEPAIR*		m_pGwangmasingongEffectPP;		// 흡성 신공 광마신공
	_EFFECTPACKAGEPAIR*		m_pDokhyulsingongEffectPP;		// 흡성 신공 독혈신공

	// 오행 이펙트
	_EFFECTPACKAGEPAIR*		m_pFEEffectPP;

	// 이벤트 아이템 이펙트
	_EFFECTPACKAGEPAIR*		m_pEventItemEffectPP;

	// 기
	_EFFECTPACKAGEPAIR*		m_pSpiritEffectPP;

	// 캐릭터에 고정되는 이펙트로, 캐릭터가 없어지면 같이 없애줘야하는 이펙트.
	EFFECTPACKAGEPAIRLIST		m_EffectPPList;

	// 자신이 바닥에 있는 아이템이면 이펙트 호출.
	_EFFECTPACKAGEPAIR*		m_pItemGroundEffectPP;
	bool	m_bCreateItemGroundEffect;

	// 자신이 바닥에 있는 아이템이면서 각성아이템인 경우 이펙트
	_EFFECTPACKAGEPAIR*		m_pRebirthItemEffectPP;
	
	// 화살 궤적 이펙트
	CLineParticle*	m_pLineParticle;

	// 칼 궤적.
	CSwordTrace		m_SwordTrace;	// 대부분의 캐릭터를 위한 오른손 칼 궤적
	CSwordTrace		m_SwordTrace2;	// 야차를 위한 왼손 칼 궤적
	float			m_fWeaponLength;
	float			m_fWeaponBackLength;	// 야차의 비는 손잡이 뒤로 길게 나와 있다.

	BOOL			m_bGlowEnable;
	BOOL			m_bShowGage;	// 에너지 게이지
	BOOL			m_bShowManaGage;
	D3DCOLOR		m_GlowColor;
	bool			m_bGray;

	// 야차의 광마독공을 위해서.
	bool			m_bNowGwangmadokgong;
	float			m_fLocalScaleForGwangmadokgong;

	// 2004.08.03 Changth
	// 야차 귀식대법
	bool			m_bNowGyuisikdaebub;

	// 공격시 SOUND FX를 어떤것을 낼것인가?
	DWORD			m_dwEnemyType;	// 때린놈의 TYPE은??

	// 문파에 대한 정보
	DWORD			m_dwMunpaID;
	sString			m_szMunpaName;
	sString			m_szMunpaNickName;
	DWORD			m_dwMunpaOrder;
	DWORD			m_dwMunpaMarkID;		// 문파 마크

	// 문파전
	BYTE			m_bWarStatus;
	DWORD			m_dwEnemyMunpaID;
	DWORD			m_dwEnemyStoneID;
	sString			m_strEnemyMunpaName;	

	// STILL 방지
	DWORD			m_dwOwnerID;
	// SEMI PK SYSTEM
	BYTE			m_bSemiPKStatus;

	// CG_2005/01/28 : 변종아이템기능추가
	BYTE			m_bChangeItemSet;

	DWORD			m_dwPartyID;
	DWORD			m_dwPartyLeaderID;
	DWORD			m_dwEnemyPartyID;

	// 개인상점
	sString			m_strShopName;        // 개인 상점명
	sString			m_strShopDescription; // 개인 상점문구
	bool			m_bTradeSell;         // 판매여부


	// 채팅박스
	sString			m_ChatMsg;
	D3DCOLOR		m_ChatColor;
	CText2D			m_text2DForChatBox;
	DWORD			m_dwTimeInterval;

	// 오행
	BYTE			m_bFECur;
	BYTE			m_bFELevel;

	// 성인서버 NPC때문에
	BYTE			m_bOrderID;

	// 설승단약
	BYTE			m_bPotionEndKeepup;

	// 기
	BYTE			m_bSpirit;

	WORD			m_wLevel;

	BYTE			m_bRebirth;

	//HT_1023 : 운영자 마크 추가
	BYTE			m_bGameMasterMark;

	int (*m_pUpdateTargetDecal)(unsigned long);

protected:

	void PersistEffect(_EFFECTPACKAGEPAIR** ppEffect, DWORD dwMugongID, int nType,
						float fLocalFrameScale, DWORD dwTotalTime = 0);
	inline void EffectTimeUpdate(_EFFECTPACKAGEPAIR* pEffect, float fLocalFrameScale);

protected:
	typedef std::list<CClassTrigger<CXiahCharObject>*> TRIGGER_LIST;

	TRIGGER_LIST m_TriggerList;

// 상위 레벨에 대한 트리거
public:
	CTrigger*	m_pParentTrigger[ eXCT_Count];
};

inline void CXiahCharObject::EffectTimeUpdate(_EFFECTPACKAGEPAIR* pEffect, float fLocalFrameScale)
{
	if(pEffect)
	{
		pEffect->dwElapsedTime += fLocalFrameScale;
		pEffect->bIsVisible = false;
	}
}

// 서버에서 보내준 특정 오브젝트의 액션상의 위치를 체크한다
// 리턴값이 TRUE일때만 Charinfo나 npcinfo기타 등등을 요청한다

#define SERVER_INTERACTION_DISTANCE 150

extern BOOL g_bScreenShot;//HO_0509_07 오토대처방안 : 스샷으로 색상을 알아내지 못하도록 색상을 변경
extern BOOL CheckServerInteractionDistance(WORD wPosX,WORD wPosY);
extern BOOL ValidateObject(BYTE bObjType,DWORD ObjID,WORD wPosX,WORD wPosY);