#pragma once

#define CONDITION_NUM	20
#define REWARD_NUM		15  // [3/30/2004] 서버와 수치 횅땍 해야함

#define QUEST_SIZE		10
#define CONDITION_SIZE	15  // [3/30/2004]
#define QUEST_VERTICAL_DISTANCE	20

//enum QUESTSTATUS_TYPE { eNew, ePause, eStart, eDeleted,  eSuccess, eSuccessDel}; //정렬을 위해서 순서를 바꿔준다.
enum QUESTSTATUS_TYPE { eStart, ePause, eNew,  eSuccess, eDeleted,   eSuccessDel}; //HT_0914 : 기연창 및 낭 아이템 개선 사항

struct sQuestInfo
{
	DWORD	m_dwID;
	QUESTSTATUS_TYPE	m_eStatus;
	sString m_szName;
	sString m_szContent;
	BYTE	m_bConditionNum;
	BYTE	m_bRewardNum;
	BYTE	m_bRepeat;
	BYTE	m_bProcessType;

	sString m_szCondition[ CONDITION_NUM];
	DWORD	m_dwConditionTotalAmount[ CONDITION_NUM];
	DWORD	m_dwConditionCurrAmount[ CONDITION_NUM];

	sString m_szReward[ REWARD_NUM];
	DWORD	m_dwRewardAmount[ REWARD_NUM];

	sQuestInfo()
	{
		int i;
		m_dwID =0;
		m_eStatus = eStart;
		m_szName = _T("");
		m_szContent = _T("");
		m_bRepeat = 0;
		m_bProcessType = 0;

		for(i=0;i<CONDITION_NUM;i++)
		{
			m_szCondition[i] = _T("");
			m_dwConditionTotalAmount[i] = 0;
			m_dwConditionCurrAmount[i] = 0;
		}
		for(i=0;i<REWARD_NUM;i++)
		{
			m_szReward[i] = _T("");
			m_dwRewardAmount[i] = 0;
		}
	}
};

//HT_0824 : 퀘스트 도우미 추가
struct sQuestcoordinate
{
	WORD xPos;
	WORD yPos;
	BYTE MapID;
};

typedef vector< sQuestcoordinate*> VQUESTCOORDINATE;

struct sQuestHelp
{
	BOOL	m_bStart;
	BYTE	m_byShowType;
	VQUESTCOORDINATE m_vQuestCoordinate;

	sQuestHelp()
	{
		m_bStart = false;
		m_byShowType = 0;
	}
};

typedef vector< sQuestInfo*> VQUEST;

enum QUESTCONTENT_TYPE { eContent, eCondition, eReward};

class CQuest
{
private:
	VQUEST		m_vQuest;
	VQUEST		m_vTotalQuest;
	sRect		m_rtRegion[ QUEST_SIZE];
	sRect		m_rtRegion_2[ QUEST_SIZE];
	sRect		m_rtRegion_3[ QUEST_SIZE];
	CText2D		m_text2D[ QUEST_SIZE];
	CText2D		m_text2D_2[ QUEST_SIZE];
	CText2D		m_text2D_3[ QUEST_SIZE];

	sRect		m_rtConditonRegion[CONDITION_SIZE];
	sRect		m_rtConditonRegion_2[CONDITION_SIZE];
	CText2D		m_ConditionText2D[CONDITION_SIZE];
	CText2D		m_ConditionText2D_2[CONDITION_SIZE];

	LPDIRECT3DVERTEXBUFFER9	m_pVB;	

	//BYTE		m_byCurrQuestIndex;

	//HT_0824 : 퀘스트 도우미 추가
	QUESTCONTENT_TYPE		m_byCurrContent;

public:

	WORD		m_wCurrQuestIndex;
	sQuestHelp	m_sQuestHelp;

	CQuest( void);
	~CQuest( void);

	void MakeVB( sRect rtRect);
	void Show();
	void DrawCurrSelectedQuest();
	void CheckCurrQuestIndex();
	void SetCurrIndex( BYTE byIndex,  sRect* rtRect);
	void SetCurrContent( QUESTCONTENT_TYPE eType);
	DWORD GetCurrQuestID();

	void ChangeStatus( DWORD dwQuestID, QUESTSTATUS_TYPE eType, BYTE bRepeat);
	inline sQuestInfo* FindQuest( DWORD dwQuestID);
	void InsertQuest( DWORD dwQuestID, QUESTSTATUS_TYPE eType, sString szQuestName, sString szDescription, BYTE bRepeat, BYTE bProcessNum, BYTE bResultNum, BYTE bProcessType);
	void InsertQuestDesc( DWORD dwQuestID);
	void InsertQuestCondition( DWORD dwQuestID, BYTE bSeq, sString szProcessName, DWORD dwProcessCurrAmount, DWORD dwProcessTotalAmount);
	void InsertQuestReward( DWORD dwQuestID, BYTE bSeq, sString szResultName, DWORD dwResultAmount);
	void Refresh();
	void RefreshQuestIndex();
	void RefreshQuestContent();

	void DeleteQuest(DWORD dwQuestID);

	//HT_0914 : 기연창 및 낭 아이템 개선 사항
	void UpdateQuest();
	void ProgressUpdateQuest();
	void NewUpdateQuest();
	void CompleteUpdateQuest();

	//HT_0824 : 퀘스트 도우미 추가
	void SetQuestDestination(DWORD QuestID, BYTE ProcessCount);
	void SetQuestProcessKind(int nProcessKind, int nTergetID, VQUESTCOORDINATE *TempQuestcoordinate);
};