#pragma once

/*
	Xiah의 Game용 오브젝트 정의

	**다음과 같은 것들이 될 수 있다.

		1) 메인 캐릭터 몸통
		2) 캐릭터의 장착 부위
		3) NPC
		4) 무기
		5) 바닥에 떨어진 아이템
		6) 화살
		7) 기타 Client전용의 Effect오브젝트

	**Rendering관련한 것부터 Animation관련한 처리가 존재

	**각각의 Object특성에 맞는 형태의 PrivateData를 가질 수 있다

	**제발 부탁인데 신영웅문의 MoveObject가 되지 않도록 각별히 주의바람	
*/

#define	MUNPA_PORTAL_CHAR_ID	1006		// 포탈 CharID
#define MUNPA_PORTAL_CHAR_ID2	1007		// 포탈 in 문파전 맵 CharID
#define	MUNPA_BBS_CHAR_ID		881		// 문파 게시판 CharID


#define CLIENT_OBJECT_MAX	5000

#define OBJECTID unsigned __int64	// 젠장 typedef으로 했더니 댄轎나네

namespace XiahObject
{
	enum eXiahObjectBasicType
	{
		eXOT_Basic			=	0x1,
		eXOT_3DObject		=	0x3,
		eXOT_CharObject		=	0x7
	};

	class CXiahObject_Basic
	{
	public:
		CXiahObject_Basic() : m_dwFame(127), m_nBasicType(eXOT_Basic), m_PrivateDataDestoryer(NULL), m_bDeleteME(FALSE)
		{
			//YS_0812 : BUGFIX
			m_bUse	= m_bPoolClass	= m_bPetPool = false;

			m_bObjType = m_bSubObjType = m_pPrivateData = m_bObjStatus = 0;
			m_dwCurHP = m_dwMaxHP = 100;	// 일단 처음에는 이렇게 놓고 			
			m_dwCurIP = m_dwMaxIP = 0;      // 统一非主角实体的内功属性支持

			m_dwXiahObjectID = 0;
		}

		virtual ~CXiahObject_Basic()
		{
			m_bObjType = 0;	
			m_nBasicType = eXOT_Basic;

			if( m_PrivateDataDestoryer && m_pPrivateData != 0)
			{
				m_PrivateDataDestoryer( m_pPrivateData);
			}

			m_pPrivateData = 0;
		}

		// 이녀석땜시 고생할순 없다.
		// 캐릭터를 그리는 부분에서 캐릭터 아이디를 알아야 되는 부분이 있다.
		DWORD	m_dwXiahObjectID;

		BYTE					m_bObjType;
		BYTE					m_bSubObjType;
		BYTE					m_bExSubObjType;
		BYTE					m_bObjStatus;
		sString					m_szObjectName;

		eXiahObjectBasicType	m_nBasicType;

		DWORD					m_dwCurHP;
		DWORD					m_dwMaxHP;
		DWORD					m_dwCurIP; // 统一内功当前值属性
		DWORD					m_dwMaxIP; // 统一内功最大值属性

		// 명성치
		DWORD					m_dwFame;

		DWORD					m_pPrivateData;
		BOOL				   (*m_PrivateDataDestoryer)(DWORD pData);

		BOOL					m_bDeleteME;

		//YS_0728 : BUGFIX
		bool					m_bUse;
		bool					m_bPoolClass;

		//YS_0812 : BUGFIX
		bool					m_bPetPool;

		virtual void InitClass()
		{
			m_dwFame				= 127;
			m_nBasicType			= eXOT_Basic;
			m_PrivateDataDestoryer	= NULL;
			m_bDeleteME				= FALSE;
			m_bObjType			    = m_bSubObjType = m_pPrivateData = m_bObjStatus = 0;
			m_dwCurHP				= m_dwMaxHP		= 100;
			m_dwCurIP				= m_dwMaxIP		= 0;
			m_dwXiahObjectID		= 0;
			m_bExSubObjType			= 0;			
			m_szObjectName			= "";
			m_pPrivateData			= 0;

			//YS_0812 : BUGFIX
			m_bPetPool = false;
		}

		virtual void DeleteClass()
		{
			m_bObjType = 0;	
			m_nBasicType = eXOT_Basic;

			if( m_PrivateDataDestoryer && m_pPrivateData != 0)
			{
				m_PrivateDataDestoryer( m_pPrivateData);
			}

			m_pPrivateData = 0;
		}
		//..BUGFIX

		// 뽀루꾸 RTTI 
		virtual BOOL IsKindOf(eXiahObjectBasicType type) { return m_nBasicType & type;}
		virtual BOOL IsA(eXiahObjectBasicType type){ return m_nBasicType == type;}

		// 화면에 보이면 bVisible을 Update시켜준다
		virtual BOOL Update(BOOL bVisible = TRUE){return TRUE;};
		virtual BOOL Render(){return TRUE;};
	};	
	
	// 단일 ObjectClass
	class CXiahObject
	{
	public:
		CXiahObject();
		virtual ~CXiahObject();
		
		// 객체 생성
		BOOL Create(CXiahObject_Basic *pInfo);
		// 객체 해제
		BOOL Release();

	public:
		DWORD	m_dwClientID;	// Client자체 ID
		DWORD	m_dwServerID;	// Server자체 ID
		OBJECTID m_ddwObjectID;	// ObjectID (Object Key Value)
	public:
		CXiahObject_Basic*	m_pObject;	// 객체 정보
	};

	// Client용 Free ID List
	typedef std::list<CXiahObject *> FREEOBJECTLIST;

	#define MAKEOBJECTID( ClientID, ServerID, ObjectType) ((((OBJECTID)ClientID) << 40) + ((OBJECTID)ServerID << 8) + ObjectType)

	/*
		OBJECTID 에 대해서 한마디

		XiahObjectManager가 Key로 사용하는데 서버에서 특정 Object에 대한 메세지를 날릴때
		바로바로 Object를 검색하기 위해서 다음과 같이 Key값을 정해준다

		 -	Server에서 ID를 날려줄 경우에는 ClientID는 무조건 0이고
			Client에서 만든거는 ServerID는 0이다

	*/
	
	
	// Object객체 관리자
	class CXiahObjectManager : public std::hash_map<OBJECTID,CXiahObject *>
	{
	public:
		CXiahObjectManager();
		~CXiahObjectManager();

		// XiahObject를 생성한다
		CXiahObject *CreateXiahObject(DWORD ServerID,BYTE bObjType,CXiahObject_Basic *pInfo);
		// XiajObject를 지운다
		BOOL		 ReleaseXiahObject(CXiahObject *pObject);
		BOOL		 ReleaseXiahObject(unsigned __int64 objectID);

		CXiahObject *FindXiahObject(unsigned __int64 id);

		BOOL		 Release();
		BOOL		 ChangeToClientObject(CXiahObject *pObject);
		BOOL		 ChangeObjectID(DWORD dwSourceClientID,DWORD dwSourceServerID,BYTE bSourceType,
										DWORD dwTargetClientID,DWORD dwTargetServerID,BYTE bTargetType);

		BOOL		 ReleaseAllObjectExceptMainChar();
	protected:
		FREEOBJECTLIST	m_FreeObjectList;
		
		// Instance리스트에서 Free한 Object를 하나 빼온다
		CXiahObject *GetFreeObject();

	public:
		// FreeObjectList에 Object를 추가한다
		BOOL		 ReleaseObject(CXiahObject *pObject);
	};

	//YS_0728 : BUGFIX
	struct sTempPool
	{
		CXiahObject_Basic *pInfo;

		sTempPool() :  pInfo(NULL)
		{ }
	};

	class CXiahCharPool
	{
	public:
		
	public:
		CXiahCharPool();
		~CXiahCharPool();

		//YS_0812 : BUGFIX
		bool Create(int nCreate);
		CXiahObject_Basic *GetChar();

		void AllClear();
	protected:
		int m_nCreateCount;
		int m_nCurNum;

		sTempPool m_Data[201]; //MAX_CHARPOOLCLASS

	private:
	};

	extern CXiahCharPool g_XiahCharPool;
	extern CXiahCharPool g_XiahNpcPool;
	extern CXiahCharPool g_XiahPetPool;


	extern CXiahObjectManager g_XiahObjectManager;
	extern CXiahObject*	g_pMouseOnObject;
	extern CXiahObject*	g_pMouseOnObjectSave;
};

extern XiahObject::CXiahObject *g_pMainChar;	// MainCharacter의 Pointer이다