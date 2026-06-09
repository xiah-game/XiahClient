#pragma once

///////////////
struct sDanInfo
///////////////
{
	DWORD	m_dwCharID;
	BYTE	m_byPriority;	
	BYTE	m_byLevel;
	WORD	m_wPosX;
	WORD	m_wPosY;
	DWORD	m_dwCurHp;
	DWORD	m_dwMaxHp;
	DWORD	m_dwMapID;
	sString	m_szNickName;

	sDanInfo()
	{
		m_dwCharID		=0;
		m_byPriority	=0;		
		m_byLevel		=0;
		m_wPosX			=0;
		m_wPosY			=0;
		m_dwCurHp		=0;
		m_dwMaxHp		=0;
		m_dwMapID		=0;
		m_szNickName	=_T("");
	}
};


////////////////
struct sShipInfo
////////////////
{
	BYTE	m_bShipType;	// 0:friend, 1:sabu, 2:jeja, 3:child, 4:parent, 6:spouse
	BYTE	m_bWorldID;
	DWORD	m_dwCharID;	
	DWORD	m_dwMapID;
	WORD	m_wPosX;
	WORD	m_wPosY;	
	WORD	m_wPriority;
	BOOL	m_bIsConnect;
	sString m_szNickName;

	sShipInfo()
	{
		m_bShipType =0;	
		m_dwCharID =0;
		m_szNickName =_T("");
		m_bWorldID =0;
		m_dwMapID =0;
		m_wPosX =0;
		m_wPosY =0;
		m_bIsConnect =FALSE;
		m_wPriority =0;
	}
};

////////////////
struct sClanWonInfo
////////////////
{
	DWORD	m_dwCharID;		
	DWORD	m_dwOrderID;
	WORD	m_wCharLev;	
	BYTE	m_bState;			// 접속상태
	BYTE	m_bCharType;		// 유파
	sString m_szCharName;
	sString m_szOrderName;
	sString m_szMunpaNickName;	

	sClanWonInfo()
	{
		m_dwCharID		=0;
		m_szCharName	=_T("");
		m_wCharLev		=0;
		m_dwOrderID		=0;
		m_szOrderName	=_T("");
		m_szMunpaNickName =_T("");
		m_bState		=0;
		m_bCharType		=0;
	}
};

///////////////////
struct sWhisperInfo
///////////////////
{
	DWORD	m_dwCharID;
	sString m_szNickName;
};




typedef vector< sDanInfo*> VDAN;
typedef vector< sShipInfo*> VSHIP;
typedef vector< sClanWonInfo*> VCLAN;
typedef vector< sWhisperInfo*> VWHISPER;

enum eRELATION_TYPE { eDAN, eShip, eClan};

#define MAX_NUM_IN_FRAME		5//8  // 친구
#define MAX_NUM_IN_FRAME_CLAN	8	  // 문파


struct sMunpaWarDay
{
	DWORD dwGameTime;
	DWORD dwRealTime;

	sMunpaWarDay()
	{
		dwGameTime = dwRealTime = 0;
	}
};


class CRelation
{
public:
	// temp
	std::map<int, sMunpaWarDay*> m_mMunpaWarDayList;
	int			m_nDanType;			// 단 종류 (일반단/관계단)
	// 단 분배
	BYTE		m_byExpDivision;	// 경험치 분배 0-개인 분배, 1-공동 분배
	BYTE		m_byFEDivision;		// 오행 값 분배 0-개인 분배, 1-공동 분배

	DWORD		m_dwDonateMoney;
	DWORD		m_dwMunpaWarEnemy;
	DWORD		m_dwMunpaWarTime;
	BYTE		m_bStealStone;

private:
	VDAN		m_vDan;
	VSHIP		m_vShip;
	VCLAN		m_vClan;
	VWHISPER	m_vWhisper;

	// common
	CText2D			m_text2D;
	eRELATION_TYPE	m_eCurrType;
	BYTE			m_byTotalPage;
	BYTE			m_byCurrPage;
	BYTE			m_byCurrIndex;
	DWORD			m_dwCurrRelation;
	LPDIRECT3DVERTEXBUFFER9	m_pVB;	

public:
	// dan
	DWORD		m_dwDanID;
	DWORD		m_dwDanLeader;
	// ship
    // clan
	DWORD		m_dwClanID;
	sString		m_szClanName;
	DWORD		m_dwClanLeader;
	// clan war
	DWORD		m_dwMunpaBattleID;
	BYTE		m_bBattleStatus;

public:
	// common
	CRelation(void);
	~CRelation(void);
	sString FindRelationNameByID( DWORD dwCharID);
	DWORD	FindRelationIDByName( LPCTSTR szName);
	DWORD	FindRelationIDByIndex( BYTE byPage, BYTE byIndex);
	void RefreshRelationContent();

	void DrawCurrSelectedRelation();
	void MakeVB();
	void CheckRelationIndexSelected();

	void SetCurrType( eRELATION_TYPE eType, BYTE byCurrPage = 1);
	void SetTotalPage();	
	void SetCurrPage( BYTE byPage);
	void SetCurrRelation( BYTE byIndex, BOOL bFlag = FALSE);

	eRELATION_TYPE GetCurrType() { return m_eCurrType;};
	BYTE GetTotalPage();
	BYTE GetCurrPage();
	BYTE GetCurrIndex();
	DWORD GetCurrRelation();

	// Dan
	sDanInfo* FindDanInfoByID( DWORD dwCharID);
	sDanInfo* FindDanInfoByName( LPCTSTR szName);
	sDanInfo* FindDanInfoByIndex( BYTE byIndex);
	void RefreshDanContent();
	void InsertDan( DWORD dwCharID, BYTE byPriority, sString szNickName, BYTE byLevel, WORD wPosX, WORD wPosY, DWORD dwCurHp, DWORD dwMaxHp, DWORD dwMapID);
	void DeleteDan( DWORD dwCharID);
	void ClearDan();

	void SetDanID( DWORD dwDanID) { m_dwDanID = dwDanID;};
	void SetDanLeader( DWORD dwDanLeader);
	void RefreshDanInfo( DWORD dwCharID, WORD wLevel, DWORD dwHpCur, DWORD dwHpMax, DWORD dwMapID, WORD wPosX, WORD wPosY);
	void DrawDanInfo();
	DWORD GetDanID() { return m_dwDanID;};
	BOOL Am_I_InDan();
	BOOL Am_I_LeaderInDan();

	// Ship
	sShipInfo* FindShipInfoByID( DWORD dwCharID);
	sShipInfo* FindShipInfoByName( LPCTSTR szName);
	sShipInfo* FindShipInfoByIndex( BYTE byIndex);
	void RefreshShipContent( BYTE byPage = 1);
	void InsertShip( BYTE bShipType, DWORD dwCharID, sString szNickName, BYTE bWordlID, DWORD dwMapID, WORD wPosX, WORD wPosY, BOOL bIsConnect);
	void DeleteShip( DWORD dwCharID);
	void ClearShip();

	void ChangeBuddyConnect( DWORD dwCharID, BYTE bWorldID, DWORD dwMapID, BOOL bIsConnected);

	DWORD FindSabuID();
	DWORD FindJejaID();
	DWORD FindSweetheart();

	// ClanWon
	sClanWonInfo* FindClanInfoByID( DWORD dwCharID);
	sClanWonInfo* FindClanInfoByName( LPCTSTR szName);
	sClanWonInfo* FindClanInfoByIndex( BYTE byIndex);
	int	GetClanSize() { return m_vClan.size();};
	void RefreshClanContent();
	void InsertClan( DWORD dwOrderID, sString szOrderName, DWORD dwCharID, sString szCharName, sString szMunpaNickName, WORD wService, BYTE bState, BYTE bType);
	void DeleteClan( DWORD dwCharID);
	void ClearClan();

	void SetClanInfo( DWORD dwClanID, sString szClanName) { m_dwClanID = dwClanID; m_szClanName = szClanName;};
	void SetClanID( DWORD dwClanID) { m_dwClanID = dwClanID;};
	void SetClanLeader( DWORD dwClanLeader) { m_dwClanLeader = dwClanLeader;};
	DWORD GetClanID() { return m_dwClanID;};
	sString GetClanName() { return m_szClanName;};
	BOOL Am_I_InClan();
	BOOL Am_I_LeaderInClan();
	bool Am_I_2stLeaderInClan();	// 본인이 부문주인지

	// Whisper
	void InsertWhisperInfo( DWORD dwCharID, sString szNickName);
	void ClearWhisper();
};
