
//	전 서 구

#ifndef _MAIL_
#define _MAIL_

#define	MAIL_RELATION		0
#define	MAIL_BUDDY			1
#define	MAIL_MUNPA			2
#define	MAIL_ETC			3


#define	MAX_MAIL_LIST		17	// 17라인이 최대 (보내기)
#define	MAX_RECV_MAIL		5	// 받은거 표시는 한화면에 5줄

//////////////////////////////////////////////////////////////////////////

struct sSENDMAIL_RESULT
{
	sString			szName;		// 받는 사람
	BYTE			bType;		// 종류
	BYTE			b_SubType;	// 관계 종류

	BYTE			bResult;	// 결과
};


//////////////////////////////////////////////////////////////////////////

struct sRECVMAIL_LIST
{
	DWORD			dwMailID;	// 전서의 ID
	sString			szName;		// 보낸사람
	DWORD			dwDate;		// 시간	(MACRO로 추출하기 바람)

	sString			szTitle;	// 제목
	sString			szMail;		// 내용
	bool			bRead;		// 읽었나?

};

//////////////////////////////////////////////////////////////////////////
struct sSENDMAIL_LIST
{
	DWORD		ID;			// char ID
	sString		szName;		// 이름(별호)
	BYTE		bType;		// 종류
	BYTE		b_SubType;	// 관계 종류

	bool		bChecked;	// 보낼거인가??
};


//////////////////////////////////////////////////////////////////////////

typedef std::vector<sSENDMAIL_RESULT*> SENDMAIL_RESULT;	// 보낸 결과

typedef std::vector<sRECVMAIL_LIST*> RECVMAIL_LIST;
typedef std::vector<sSENDMAIL_LIST*> SENDMAIL_LIST;
typedef std::vector<DWORD> TEMPSENDMAIL_LIST;			// 보낼때의 중복 검사를 위함

class cMAIL
{
public:
	cMAIL();
	~cMAIL();

	//////////////////////////////////////////////////////////////////////////	RESULT

	void DeleteAll_SendResult(void);
	void Add_SendResult(sString szName,BYTE bType,BYTE b_SubType);
	void Assign_Result(sString str,BYTE result);
	void Reflash_Result(void);

	void Back_Result_Page(void);
	void Next_Result_Page(void);

	//////////////////////////////////////////////////////////////////////////	ETC

	sString	Make_Short_Msg(sString str, int limit);

	//////////////////////////////////////////////////////////////////////////	RECV

	void Display_ReadMail(int pos);	// 전서의 내용을 디스플레이

	void Add_RecvMailList(DWORD dwMailID,sString Sendername,DWORD dwDate, sString Title,bool bRead);	// 전서구 리스트 추가
	void Add_RecvMailContents(DWORD dwMailID, sString szContents);	// 해당 전서 읽기

	void DeleteAll_RecvMail();			// 모두 지우기

	bool Delete_RecvMail();
	bool Delete_RecvMail(DWORD MailID);	// 해당 전서 지우기

	int	Get_RecvMailCount(void);		// 받은 메일의 갯수
	int Get_NonRead_RecvMailCount(void);// 받은 메일의 갯수 (읽지 않은거)

	void Reflash_MAIL(void);
	void Send_ReadMail(void);

	void Back_Recv_Page(void);
	void Next_Recv_Page(void);

	void CheckIndexSelected(void);
	void CreateVB(void);
	void MakeVB(void);
	void DrawCurrSelected();

	//////////////////////////////////////////////////////////////////////////	SEND

	void Make_SenderList();				// 보낼사람 리스트 만들기
	void Add_SendList(DWORD Id, sString szName,BYTE type,BYTE b_SubType);
	bool Search_SendList(DWORD ID,BYTE type,BYTE b_SubType);

	bool Delete_SendMailList(DWORD	charID);
	void DeleteAll_SendMailList();

	bool Unchecked_SendList(DWORD ID);
	void UncheckedAll_SendList(void);
	void CheckedAll_SendList(void);
	void CheckedMunpaAll_SendList(void);
	int Get_Checked_SendList(void);			// 선택된 애들의 갯수를 얻어온다.

	bool SendMailToOne(sString szName);

	bool SendMail();					// 선택된 사람들에게 전서 보내기
	bool Check_SentMail(DWORD ID);

	void Assign_Content(sString Title,sString Content);	// 메일의 내용 지정

	////////////////////////////////////////////////////////////////////////// 인터페이스

	void Reflash_MAIL_Select(void);
	void Back_Page(void);
	void Next_Page(void);
	void Check_SendList(void);

	BYTE Get_SackID(void) { return m_SackID; }
	BYTE Get_SackPos(void) { return m_SackPos; }
	BYTE Get_Amonut(void);

	void Set_SackID(BYTE ID) { m_SackID = ID; }
	void Set_SackPos(BYTE Pos) { m_SackPos = Pos; }

private:

	//////////////////////////////////////////////////////////////////////////// RECV
	
	RECVMAIL_LIST		m_Recv_Mail;

	int					m_Recv_Page;
	int					m_Recv_Total_Page;
	BYTE				m_byCurrIndex;

	LPDIRECT3DVERTEXBUFFER9	m_pVB;

	//////////////////////////////////////////////////////////////////////////// SEND

	SENDMAIL_LIST		m_Send_Mail;
	TEMPSENDMAIL_LIST	m_checksendmail;

	sString				m_Mail_Content;
	sString				m_Mail_Title;

	int					m_Page;
	int					m_Total_Page;

	// 행낭의 아이템 위치
	BYTE				m_SackID;
	BYTE				m_SackPos;
	
	//////////////////////////////////////////////////////////////////////////// RESULT

	SENDMAIL_RESULT		m_Send_Result;
	int					m_Result_Page;
	int					m_Result_Total_Page;

};


extern	cMAIL	g_Mail;

#endif