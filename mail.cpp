#include "precompile.h"
#include "xiahobjecttype.h"
#include "mail.h"
#include "XiahGame_Handler_Sender.h"
#include "XiahGameMain.h"

cMAIL	g_Mail;

//////////////////////////////////////////////////////////////////////////

cMAIL::cMAIL()
{
	m_Mail_Title = _T("");
	m_Mail_Content = _T("");

	m_Page = 0;
	m_Total_Page = 0;

	m_Recv_Page = 0;
	m_Recv_Total_Page = 0;

	m_Result_Page = 0;
	m_Result_Total_Page = 0;

	m_SackID = 0;
	m_SackPos = 0;

}

cMAIL::~cMAIL()
{
	DeleteAll_RecvMail();		// 받은거 모두 지우고
	DeleteAll_SendMailList();	// 보낼사람들 모두 지운다.
	DeleteAll_SendResult();		// 결과를 모두 지우자

	if(m_pVB)
		m_pVB->Release();
}

//////////////////////////////////////////////////////////////////////////	ETC

sString	cMAIL::Make_Short_Msg(sString str,int limit)
{	
	sString ret;
	char buffer[1024] = {0,};
	char tempbuf[1024] = {0,};
	int len = 0;

	strcpy(buffer,str.data());
	for(int i =0; i < str.length(); i++)
	{
		if(buffer[i] & 0x80)
		{
			tempbuf[len++] = buffer[i++];
			tempbuf[len++] = buffer[i];
		}
		else
		{
			tempbuf[len++] = buffer[i];
		}

		if(len > limit)
		{
			tempbuf[len++] = '.';
			tempbuf[len] = '.';
			break;
		}
	}

	ret.printf("%s",tempbuf);
	return ret;
}

//////////////////////////////////////////////////////////////////////////	RECV

// 메일 리스트 추가
void cMAIL::Add_RecvMailList(DWORD dwMailID,sString Sendername,DWORD dwDate, sString Title,bool bRead)
{
	sRECVMAIL_LIST	*m_mail;

	m_mail = new sRECVMAIL_LIST;
	m_mail->dwMailID = dwMailID;
	m_mail->szName = Sendername;
	m_mail->dwDate = dwDate;
	m_mail->szTitle = Title;
	m_mail->bRead = bRead;

	m_Recv_Mail.push_back(m_mail);
}

// 읽은 전서의 내용을 뿌리자!
void cMAIL::Display_ReadMail(int pos)
{
	int ptr = 0, p = 0;
	int line = 0;
	char c;
	char buf[1024] = {0,};
	char temp[1024] = {0,};

	m_Recv_Mail[pos]->bRead = true;
	strcpy(buf,m_Recv_Mail[pos]->szMail.data());

	// 내용 Clear
	for(int j = 0; j < 8; j++)
		g_pUIManager->SetString(WINDOW_MAIL, window_mail_edit_01 - j, _T(""));

	while(1)
	{
		c = buf[p++];
		if(c == 0)
		{
			if(strlen(temp) > 0)
				g_pUIManager->SetString(WINDOW_MAIL, window_mail_edit_01 - line, temp);
			break;
		}
		else if(c == '|')
		{
			g_pUIManager->SetString(WINDOW_MAIL, window_mail_edit_01 - line, temp);
			memset(temp,0,sizeof(temp));
			line++;
			ptr = 0;
		}
		else
		{
			temp[ptr++] = c;
		}
	}

	g_pUIManager->SetString(WINDOW_MAIL,window_mail_top_edit_01, m_Recv_Mail[pos]->szName.data());
	g_pUIManager->SetString(WINDOW_MAIL,window_mail_top_edit_02, m_Recv_Mail[pos]->szTitle.data());

	// 화면 갱신
	Reflash_MAIL();
}

// 해당 전서를 읽기
void cMAIL::Add_RecvMailContents(DWORD dwMailID, sString szContents)
{
	int num = m_Recv_Mail.size();

	for(int i = 0; i < num; i++)
	{
		if(m_Recv_Mail[i]->dwMailID == dwMailID)
		{
			// 내용을 전달
			m_Recv_Mail[i]->szMail = szContents;
			Display_ReadMail(i);	// 화면에 출력
			return;
		}
	}
}

// 모두 지우기
void cMAIL::DeleteAll_RecvMail()
{
	std::vector<sRECVMAIL_LIST*>::iterator iteratorRecvmail;
	for(iteratorRecvmail = m_Recv_Mail.begin(); iteratorRecvmail != m_Recv_Mail.end(); ++iteratorRecvmail)
	{
		delete (*iteratorRecvmail);
	}
	m_Recv_Mail.clear();
}

bool cMAIL::Delete_RecvMail()
{
	DWORD pos;
	pos = m_Recv_Page * MAX_RECV_MAIL + m_byCurrIndex;
	if(Get_RecvMailCount() <= pos) return false;

	SendCS_IM_DELETEMEMO_REQ(m_Recv_Mail[pos]->dwMailID);
	return true;
}

// 해당 전서구 지우기
bool cMAIL::Delete_RecvMail(DWORD MailID)
{
	int num = m_Recv_Mail.size();
	RECVMAIL_LIST::iterator where = m_Recv_Mail.begin();

	for(int i = 0; i < num; i++)
	{
		if(m_Recv_Mail[i]->dwMailID == MailID)
		{
			delete m_Recv_Mail[i];
			m_Recv_Mail.erase(where);

			Reflash_MAIL();
			return true;
		}
		where++;
	}

	return false;	// 해당 전서가 없슴
}

// 받은 메일의 갯수
int	cMAIL::Get_RecvMailCount(void)
{
	return m_Recv_Mail.size();
}

// 받은 메일의 갯수 (읽지 않은거)
int cMAIL::Get_NonRead_RecvMailCount(void)
{
	int ret = 0;
	for(int i = 0; i < m_Recv_Mail.size(); i++)
		if(m_Recv_Mail[i]->bRead == false) ret++;

	return ret;
}

// 서버에 읽을 전서 내용을 요청
void cMAIL::Send_ReadMail(void)
{
	DWORD pos;
	pos = m_Recv_Page * MAX_RECV_MAIL + m_byCurrIndex;
	if(Get_RecvMailCount() <= pos) return;

	SendCS_IM_READMEMO_REQ(m_Recv_Mail[pos]->dwMailID);
}

// 읽은 편지 표시
void cMAIL::Reflash_MAIL(void)
{
	int i;
	int num = m_Recv_Mail.size();
	sString	Total_page_str;
	sString	szDate;
	sString	szRemain;

	m_Recv_Total_Page = (num / MAX_RECV_MAIL);
	if(num % MAX_RECV_MAIL > 0) m_Recv_Total_Page++;

	// Clear
	for(i = 0; i < MAX_RECV_MAIL; i++)
	{
		g_pUIManager->SetString(WINDOW_MAIL, window_mail_list_dummy_01 - i ,"");
		g_pUIManager->SetString(WINDOW_MAIL, window_mail_list_dummy_06 - i ,"");
		g_pUIManager->SetString(WINDOW_MAIL, window_mail_list_dummy_11 - i ,"");
	}

	// Page
	Total_page_str.printf("%d/%d",m_Recv_Page+1,m_Recv_Total_Page);
	g_pUIManager->SetString(WINDOW_MAIL, window_mail_button_dummy ,Total_page_str.data());

	// 남은 전송횟수표시
	szRemain.printf(IDS_REMAIN_SEND,g_Mail.Get_Amonut());
	g_pUIManager->SetString(WINDOW_MAIL, window_mail_info_dummy_01 ,szRemain.data());
	
	for(i = 0; i < MAX_RECV_MAIL; i++)
	{
		if(num <= m_Recv_Page * MAX_RECV_MAIL+i) break;

		// 읽었나?? 안읽었나??		
		if(m_Recv_Mail[m_Recv_Page * MAX_RECV_MAIL+i]->bRead == true)
		{
			// 보낸사람
			g_pUIManager->SetString(WINDOW_MAIL, window_mail_list_dummy_01-i ,m_Recv_Mail[m_Recv_Page * MAX_RECV_MAIL+i]->szName,6);
			// 보낸날짜
			szDate.printf("%2d/%2d/%2d",GETYEAR(m_Recv_Mail[m_Recv_Page * MAX_RECV_MAIL+i]->dwDate),GETMONTH(m_Recv_Mail[m_Recv_Page * MAX_RECV_MAIL+i]->dwDate),GETDAY(m_Recv_Mail[m_Recv_Page * MAX_RECV_MAIL+i]->dwDate));
			g_pUIManager->SetString(WINDOW_MAIL, window_mail_list_dummy_06-i ,szDate.data(),6);
			// 제목
			g_pUIManager->SetString(WINDOW_MAIL, window_mail_list_dummy_11-i ,Make_Short_Msg(m_Recv_Mail[m_Recv_Page * MAX_RECV_MAIL+i]->szTitle,9),6);
		}
		else
		{
			g_pUIManager->SetString(WINDOW_MAIL, window_mail_list_dummy_01-i ,m_Recv_Mail[m_Recv_Page * MAX_RECV_MAIL+i]->szName);
			szDate.printf("%2d/%2d/%2d",GETYEAR(m_Recv_Mail[m_Recv_Page * MAX_RECV_MAIL+i]->dwDate),GETMONTH(m_Recv_Mail[m_Recv_Page * MAX_RECV_MAIL+i]->dwDate),GETDAY(m_Recv_Mail[m_Recv_Page * MAX_RECV_MAIL+i]->dwDate));
			g_pUIManager->SetString(WINDOW_MAIL, window_mail_list_dummy_06-i ,szDate.data());
			g_pUIManager->SetString(WINDOW_MAIL, window_mail_list_dummy_11-i ,Make_Short_Msg(m_Recv_Mail[m_Recv_Page * MAX_RECV_MAIL+i]->szTitle,9));
		}
	}

}

// 뒤로 버튼
void cMAIL::Back_Recv_Page(void)
{
	if(m_Recv_Page == 0) return;
	--m_Recv_Page;
	m_byCurrIndex = 0;
	MakeVB();
	Reflash_MAIL();
}

// 앞으로 버튼
void cMAIL::Next_Recv_Page(void)
{
	if(m_Recv_Page+1 >= m_Recv_Total_Page) return;
	++m_Recv_Page;
	m_byCurrIndex = 0;
	MakeVB();
	Reflash_MAIL();
}

// 받은 메일 선택
void cMAIL::CheckIndexSelected(void)
{
	CreateVB();

	if(g_pUIManager->IsShow(WINDOW_MAIL))
	{
		for( int i=0; i < 5; ++i)
		{
			if( g_pUIManager->IsMouseOn(WINDOW_MAIL, window_mail_list_dummy_01 - i)
				|| g_pUIManager->IsMouseOn(WINDOW_MAIL, window_mail_list_dummy_06 - i)
				|| g_pUIManager->IsMouseOn(WINDOW_MAIL, window_mail_list_dummy_11 - i))
			{
				m_byCurrIndex = i;	// 어디를 선택했지??
				MakeVB();
				return;
			}
		}
	}
}

void cMAIL::CreateVB()
{
	// Create Recv Select bar Vertex Buffer
	if(NULL == m_pVB)
		g_pDirect3DDevice->CreateVertexBuffer( 4*sizeof(VT_TLVertex),0, D3DFVF_TLVERTEX,D3DPOOL_MANAGED, &m_pVB, NULL);
}


void cMAIL::DrawCurrSelected()
{
	if(g_pUIManager->IsShow(WINDOW_MAIL))
	{
		g_pDirect3DDevice->SetRenderState( D3DRS_ZENABLE, FALSE);
		g_pDirect3DDevice->SetRenderState( D3DRS_FOGENABLE, FALSE);

		g_pDirect3DDevice->SetRenderState( D3DRS_ALPHABLENDENABLE, TRUE);
		g_pDirect3DDevice->SetRenderState( D3DRS_DIFFUSEMATERIALSOURCE, D3DMCS_COLOR1);
		g_pDirect3DDevice->SetRenderState( D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
		g_pDirect3DDevice->SetRenderState( D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

		g_pDirect3DDevice->SetSamplerState( 0,  D3DSAMP_ADDRESSU , D3DTADDRESS_CLAMP);
		g_pDirect3DDevice->SetSamplerState( 0,  D3DSAMP_ADDRESSV , D3DTADDRESS_CLAMP);

		g_Device.SetTexture(0, NULL);
		//g_pDirect3DDevice->SetTexture( 0, NULL);
		g_Device.SetStreamSource( m_pVB, sizeof(VT_TLVertex));
		g_Device.SetFVF(D3DFVF_TLVERTEX);
		//g_pDirect3DDevice->SetFVF( D3DFVF_TLVERTEX);
		g_pDirect3DDevice->DrawPrimitive( D3DPT_TRIANGLESTRIP, 0, 2);	
	}
}


void cMAIL::MakeVB()
{
	sRect rtRect;
	RECT rtTemp;

	if(g_pUIManager->IsShow(WINDOW_MAIL))
	{
		g_pUIManager->GetRegionData(WINDOW_MAIL, window_mail_list_dummy_01 - m_byCurrIndex, rtRect);
		g_pUIManager->GetRegionData(WINDOW_MAIL, window_mail_list_dummy_11 - m_byCurrIndex, rtTemp);

		rtRect.right = rtTemp.right;
		rtRect.bottom = rtTemp.bottom;

		D3DCOLOR d3dcolor = 0x55999999;

		VT_TLVertex Vertex[4];

		Vertex[ 0].pos = Vector4( rtRect.left, rtRect.top, 0, 1);
		Vertex[ 1].pos = Vector4( rtRect.right, rtRect.top, 0, 1);
		Vertex[ 2].pos = Vector4( rtRect.left, rtRect.bottom, 0, 1);
		Vertex[ 3].pos = Vector4( rtRect.right, rtRect.bottom, 0, 1);

		Vertex[ 0].diffuse = d3dcolor;
		Vertex[ 1].diffuse = d3dcolor;
		Vertex[ 2].diffuse = d3dcolor;
		Vertex[ 3].diffuse = d3dcolor;

		Vertex[ 0].tex = Vector2( 0, 0);
		Vertex[ 1].tex = Vector2( 1, 0);
		Vertex[ 2].tex = Vector2( 0, 1);
		Vertex[ 3].tex = Vector2( 1, 1);

		VOID* pVertices;
		if( !FAILED( m_pVB->Lock( 0, sizeof(Vertex), (void**)&pVertices, 0 )))
		{
			memcpy( pVertices, Vertex, sizeof(Vertex) );
			m_pVB->Unlock();
		}

	}
}


//////////////////////////////////////////////////////////////////////////	SEND

// 받는사람 리스트를 여기저기서 모아서 나온다.
void cMAIL::Make_SenderList()
{

}

// 선택된 애들의 갯수를 얻어온다.
int cMAIL::Get_Checked_SendList(void)
{
	TCHAR strTo[64] = {0,};
	int ret = 0;
	int num = m_Send_Mail.size();
	int i;

	for(i = 0; i < num; i++)
	{
		// 친구이자 문원은 보여야 한다. (중복가능)
		if(m_Send_Mail[i]->bChecked == true)
			ret++;
	}

	// 그냥 친 이름을 가지고 있는애또한 계산한다
	g_pUIManager->GetString(WINDOW_MAIL, window_mail_top_edit_01, strTo, GET_STRING);

	for(i = 0; i < num; i++)
	{
		if(!strcmp(m_Send_Mail[i]->szName.data(),strTo) && m_Send_Mail[i]->bChecked == true) 
			break;
	}

	if(i == num) ret++;
	return ret;
}

bool cMAIL::Search_SendList(DWORD ID,BYTE type,BYTE b_SubType)
{
	int num = m_Send_Mail.size();

	for(int i = 0; i < num; i++)
	{
		// 친구이자 문원은 보여야 한다. (중복가능)
		if(m_Send_Mail[i]->ID == ID && m_Send_Mail[i]->bType == type && m_Send_Mail[i]->b_SubType == b_SubType)
		{
			// 이미 있슴
			return false;
		}
	}

	return true;
}

// 추가
void cMAIL::Add_SendList(DWORD	Id, sString	szName, BYTE type,BYTE b_SubType)
{
	sSENDMAIL_LIST	*temp;
	// 이미 있나를 검사
	if(Search_SendList(Id,type,b_SubType) == false) return;

	temp = new sSENDMAIL_LIST;
	temp->bChecked = false;
	temp->bType = type;
	temp->b_SubType = b_SubType;
	temp->ID = Id;
	temp->szName = szName;

	m_Send_Mail.push_back(temp);
}

void cMAIL::DeleteAll_SendMailList()
{
	std::vector<sSENDMAIL_LIST*>::iterator iteratorSendmail;
	for(iteratorSendmail = m_Send_Mail.begin(); iteratorSendmail != m_Send_Mail.end(); ++iteratorSendmail)
	{
		delete (*iteratorSendmail);
	}
	m_Send_Mail.clear();

}

bool cMAIL::Delete_SendMailList(DWORD	charID)
{
	int num = m_Send_Mail.size();
	SENDMAIL_LIST::iterator where = m_Send_Mail.begin();

	for(int i = 0; i < num; i++)
	{
		if(m_Send_Mail[i]->ID == charID)
		{
			delete m_Send_Mail[i];
			m_Send_Mail.erase(where);
			return true;
		}
		where++;
	}

	return false;	// 해당 전서가 없슴
}

// 해당 ID의 send checked를 unchecked로 한다. 
bool cMAIL::Unchecked_SendList(DWORD ID)
{
	int num = m_Send_Mail.size();
	SENDMAIL_LIST::iterator where = m_Send_Mail.begin();

	for(int i = 0; i < num; i++)
	{
		if(m_Send_Mail[i]->ID == ID)
		{
			m_Send_Mail[i]->bChecked = false;
			return true;
		}
		where++;
	}

	return false;
}

// 전부다 unchecked로 mark
void cMAIL::UncheckedAll_SendList(void)
{
	int num = m_Send_Mail.size();

	for(int i = 0; i < num; i++)
	{
		m_Send_Mail[i]->bChecked = false;
	}
}

// 전체 선택
void cMAIL::CheckedAll_SendList(void)
{
	int num = m_Send_Mail.size();

	for(int i = 0; i < num; i++)
	{
		m_Send_Mail[i]->bChecked = true;
	}
}

// 문파 선택
void cMAIL::CheckedMunpaAll_SendList(void)
{
	int num = m_Send_Mail.size();

	for(int i = 0; i < num; i++)
	{
		if(m_Send_Mail[i]->bType == MAIL_MUNPA)
			m_Send_Mail[i]->bChecked = true;
		else
			m_Send_Mail[i]->bChecked = false;
	}
}

// 해당 ID만
bool cMAIL::Check_SentMail(DWORD ID)
{
	int num = m_checksendmail.size();

	for(int i = 0; i < num; i++)
	{
		if(m_checksendmail[i] == ID) return false;
	}

	return true;
}

// 선택된 사람들에게 전서 보내기
bool cMAIL::SendMail()
{
	m_checksendmail.clear();
	int num = m_Send_Mail.size();

	DeleteAll_SendResult();

	for(int i = 0; i < num; i++)
	{
		// 같은 ID로 두번이상 보내지 않도록 조치!
		if(m_Send_Mail[i]->bChecked == true && Check_SentMail(m_Send_Mail[i]->ID) == true)
		{
			SendCS_IM_SENDMEMO_REQ(m_Send_Mail[i]->szName,m_Mail_Title ,m_Mail_Content,this->Get_SackID(),this->Get_SackPos());
			m_checksendmail.push_back(m_Send_Mail[i]->ID);

			// 결과를 위하여 저장
			Add_SendResult(m_Send_Mail[i]->szName,m_Send_Mail[i]->bType,m_Send_Mail[i]->b_SubType);
		}
	}


	g_MainCharInfo.CloseFrame(WINDOW_MAIL_SELECT);
	g_MainCharInfo.OpenFrame(WINDOW_MAIL_RESULT);
	return true;
}

// 제목과 전서 내용을 지정
void cMAIL::Assign_Content(sString Title,sString Content)
{
	m_Mail_Title = Title;
	m_Mail_Content = Content;
}

bool cMAIL::SendMailToOne(sString szName)
{
	BYTE Kind;
	BYTE SubKind;
	int num = m_Send_Mail.size();
	if(szName.length() < 1) return false;

	for(int i = 0; i < num; i++)
	{
		if(!strcmp(m_Send_Mail[i]->szName.data(),szName))
		{
			if(m_Send_Mail[i]->bChecked == true) return false;
			Kind = m_Send_Mail[i]->bType;
			SubKind = m_Send_Mail[i]->b_SubType;
			break;
		}
		else
		{
			Kind = MAIL_ETC;
			SubKind = 0;
		}

	}

	// 이사람은 날린적이 없으므로, 이 사람에게 날린다.
	SendCS_IM_SENDMEMO_REQ(szName,m_Mail_Title,m_Mail_Content,this->Get_SackID(),this->Get_SackPos());
	Add_SendResult(szName.data(),Kind,SubKind);
	return true;
}

//////////////////////////////////////////////////////////////////////////	인터페이스

void cMAIL::Reflash_MAIL_Select(void)
{
	sString	relation_type;
	sString	Total_page_str;
	int i;

	int num = m_Send_Mail.size();
	m_Total_Page = (num / MAX_MAIL_LIST );
	if(num % MAX_MAIL_LIST > 0) m_Total_Page++;

	for(i = 0; i < MAX_MAIL_LIST; i++)
	{
		g_pUIManager->SetString(WINDOW_MAIL_SELECT, window_mail_select_list_dummy_01-i ,"");
		g_pUIManager->SetString(WINDOW_MAIL_SELECT, window_mail_select_list_dummy_02_01-i ,"");
		// Check 버튼 Disable
		g_pUIManager->Hide(WINDOW_MAIL_SELECT,mail_list_select_01-i);
	}

	// Page
	Total_page_str.printf("%d/%d",m_Page+1,m_Total_Page);
	g_pUIManager->SetString(WINDOW_MAIL_SELECT, window_mail_select_button_dummy ,Total_page_str.data());

	for(i = 0; i < MAX_MAIL_LIST; i++)
	{
		if(num <= m_Page*MAX_MAIL_LIST+i) break;

		// 문원 / 인연 / 친구 이름
		g_pUIManager->SetString(WINDOW_MAIL_SELECT, window_mail_select_list_dummy_02_01-i ,m_Send_Mail[m_Page*MAX_MAIL_LIST + i]->szName);

		// Check 버튼 Enable
		g_pUIManager->Show(WINDOW_MAIL_SELECT,mail_list_select_01-i);
		if(m_Send_Mail[m_Page*MAX_MAIL_LIST + i]->bChecked == true)
			g_pUIManager->SetData(WINDOW_MAIL_SELECT,mail_list_select_01-i,CURRENT_INDEX,0);
		else
			g_pUIManager->SetData(WINDOW_MAIL_SELECT,mail_list_select_01-i,CURRENT_INDEX,1);

		// 종류
		switch(m_Send_Mail[m_Page*MAX_MAIL_LIST + i]->bType)
		{
			case MAIL_ETC:
					relation_type = IDS_RECV_MAIL_ETC;
				break;

			// 친구
			case MAIL_BUDDY :
					relation_type = IDS_FRIEND;
				break;

			// 관계
			case MAIL_RELATION:
				{
					switch(m_Send_Mail[m_Page*MAX_MAIL_LIST + i]->b_SubType)
					{
						// 연인
						case RELATION_TYPE_LOVER:
							relation_type = IDS_SWEETHEART;
							break;

						// 스승
						case RELATION_TYPE_TEACHER:
							relation_type = IDS_TEACHER;
							break;

						// 제자
						case RELATION_TYPE_STUDENT:
							relation_type = IDS_DISCIPLE;
							break;
					}
				}
				break;
			
			// 문파
			case MAIL_MUNPA :
				{
					switch(m_Send_Mail[m_Page*MAX_MAIL_LIST + i]->b_SubType)
					{
						case MUNPA_ORDER_MUNJU:
								relation_type = IDS_MUNJU;
							break;
						case MUNPA_ORDER_BUMUNJU:
								relation_type = IDS_BUMUNJU;
							break;
						case MUNPA_ORDER_JANGRO:
							relation_type = IDS_JANGRO;
							break;
						case MUNPA_ORDER_HOBUB:
							relation_type = IDS_HOBUB;
							break;

						case MUNPA_ORDER_DANGJU:
							relation_type = IDS_DANGJU;
							break;

						case MUNPA_ORDER_MUNWON:
							relation_type = IDS_MUNWON;
							break;
					}
				}
				break;

		}
		g_pUIManager->SetString(WINDOW_MAIL_SELECT, window_mail_select_list_dummy_01-i ,relation_type);
	}
}

void cMAIL::Back_Page(void)
{
	if(m_Page == 0) return;
	--m_Page;
	Reflash_MAIL_Select();
}

void cMAIL::Next_Page(void)
{
	if(m_Page+1 >= m_Total_Page) return;
	++m_Page;
	Reflash_MAIL_Select();
}

void cMAIL::Check_SendList(void)
{
	int num = m_Send_Mail.size();

	for(int i = 0; i < MAX_MAIL_LIST; i++)
	{
		if(num <= m_Page*MAX_MAIL_LIST+i) break;

		if(g_pUIManager->IsMouseOn(WINDOW_MAIL_SELECT,mail_list_select_01-i))
		{
			if( g_pUIManager->GetData(WINDOW_MAIL_SELECT,mail_list_select_01-i, GET_CURRENT_INDEX) == 1)
			{ 
				g_pUIManager->SetData(WINDOW_MAIL_SELECT,mail_list_select_01-i,CURRENT_INDEX, 0);
				m_Send_Mail[m_Page*MAX_MAIL_LIST + i]->bChecked = true;
			}
			else
			{
				g_pUIManager->SetData(WINDOW_MAIL_SELECT,mail_list_select_01-i,CURRENT_INDEX, 1);
				m_Send_Mail[m_Page*MAX_MAIL_LIST + i]->bChecked = false;
			}
		}
	}
}

BYTE cMAIL::Get_Amonut(void) 
{
	WORD	Amount = 0;
	XiahItem::sItemInfo* pItem = NULL;

	if(m_SackID == 0)
	pItem = g_MainCharInfo.m_pEquipSack->FindSackItemByPos( m_SackPos);
		else
	pItem = g_MainCharInfo.m_pMySack[ m_SackID-1]->FindSackItemByPos( m_SackPos);

	if(pItem)
	{
		Amount = pItem->m_wCurDur;
	}
	
	return Amount;
}

//////////////////////////////////////////////////////////////////////////

// Clear
void cMAIL::DeleteAll_SendResult(void)
{
	std::vector<sSENDMAIL_RESULT*>::iterator iteratorResultmail;
	for(iteratorResultmail = m_Send_Result.begin(); iteratorResultmail != m_Send_Result.end(); ++iteratorResultmail)
	{
		delete (*iteratorResultmail);
	}
	m_Send_Result.clear();

}

// 전송한 목록을 넣는다.
void cMAIL::Add_SendResult(sString szName,BYTE bType,BYTE b_SubType)
{
	sSENDMAIL_RESULT	*m_result;

	m_result = new sSENDMAIL_RESULT;
	m_result->szName = Make_Short_Msg(szName,12);
	m_result->bType = bType;
	m_result->b_SubType = b_SubType;

	m_Send_Result.push_back(m_result);
}

// 결과를 넣고 보이자
void cMAIL::Assign_Result(sString str,BYTE result)
{
	int num = m_Send_Result.size();
	for(int i = 0; i < num; i++)
	{
		if(!strcmp(m_Send_Result[i]->szName.data(),str))
		{
			m_Send_Result[i]->bResult = result;
			break;
		}
	}

	Reflash_Result();
	Reflash_MAIL();
}

void cMAIL::Reflash_Result(void)
{
	sString	relation_type;
	sString	Total_page_str;
	int num = m_Send_Result.size();
	int i;

	m_Result_Total_Page = (num / MAX_MAIL_LIST );
	if(num % MAX_MAIL_LIST > 0) m_Result_Total_Page++;

	// Page
	Total_page_str.printf("%d/%d",m_Result_Page+1,m_Result_Total_Page);
	g_pUIManager->SetString(WINDOW_MAIL_RESULT, window_mail_result_button_dummy ,Total_page_str.data());


	for(i = 0; i < MAX_MAIL_LIST; i++)
	{
		g_pUIManager->SetString(WINDOW_MAIL_RESULT, window_mail_result_list_dummy_01-i ,"");
		g_pUIManager->SetString(WINDOW_MAIL_RESULT, window_mail_result_list_dummy_02_01-i ,"");
		g_pUIManager->SetString(WINDOW_MAIL_RESULT, window_mail_result_list_dummy_03_01-i ,"");
	}

	// 화면에 출력
	for(i = 0; i < MAX_MAIL_LIST; i++)
	{
		if(num <= m_Result_Page*MAX_MAIL_LIST+i) break;
		// 종류
		switch(m_Send_Result[m_Result_Page*MAX_MAIL_LIST + i]->bType)
		{
		case MAIL_ETC:
			relation_type = IDS_RECV_MAIL_ETC;
			break;

			// 친구
		case MAIL_BUDDY :
			relation_type = IDS_FRIEND;
			break;

			// 관계
		case MAIL_RELATION:
			{
				switch(m_Send_Result[m_Result_Page*MAX_MAIL_LIST + i]->b_SubType)
				{
					// 연인
				case RELATION_TYPE_LOVER:
					relation_type = IDS_SWEETHEART;
					break;

					// 스승
				case RELATION_TYPE_TEACHER:
					relation_type = IDS_TEACHER;
					break;

					// 제자
				case RELATION_TYPE_STUDENT:
					relation_type = IDS_DISCIPLE;
					break;
				}
			}
			break;

			// 문파
		case MAIL_MUNPA :
			{
				switch(m_Send_Result[m_Result_Page*MAX_MAIL_LIST + i]->b_SubType)
				{
				case MUNPA_ORDER_MUNJU:
					relation_type = IDS_MUNJU;
					break;
				case MUNPA_ORDER_BUMUNJU:
					relation_type = IDS_BUMUNJU;
					break;
				case MUNPA_ORDER_JANGRO:
					relation_type = IDS_JANGRO;
					break;
				case MUNPA_ORDER_HOBUB:
					relation_type = IDS_HOBUB;
					break;

				case MUNPA_ORDER_DANGJU:
					relation_type = IDS_DANGJU;
					break;

				case MUNPA_ORDER_MUNWON:
					relation_type = IDS_MUNWON;
					break;
				}
			}
			break;

		}
		// 관계 종류
		g_pUIManager->SetString(WINDOW_MAIL_RESULT, window_mail_result_list_dummy_01-i ,relation_type.data());

		// 이름
		g_pUIManager->SetString(WINDOW_MAIL_RESULT, window_mail_result_list_dummy_02_01-i ,m_Send_Result[m_Result_Page*MAX_MAIL_LIST + i]->szName.data());

		// 결과
		switch(m_Send_Result[i]->bResult)
		{
		case ERR_SENDMEMO_SUCCESS:
			g_pUIManager->SetString(WINDOW_MAIL_RESULT, window_mail_result_list_dummy_03_01-i ,IDS_SEND_ERROR1);
			break;

			// 그런이름 없슴
		case ERR_SENDMEMO_FAULTNAME:
			g_pUIManager->SetString(WINDOW_MAIL_RESULT, window_mail_result_list_dummy_03_01-i ,IDS_SEND_ERROR2);
			break;

			// 받는사람 메모함 꽉참
		case ERR_SENDMEMO_MEMOFULL:
			g_pUIManager->SetString(WINDOW_MAIL_RESULT, window_mail_result_list_dummy_03_01-i ,IDS_SEND_ERROR3);
			break;

			// 아이템 부족
		case ERR_SENDMEMO_ITEMERROR:
			g_pUIManager->SetString(WINDOW_MAIL_RESULT, window_mail_result_list_dummy_03_01-i ,IDS_SEND_ERROR4);
			break;

			// 아이템 부족
		case ERR_SENDMEMO_STRINGERROR:
			g_pUIManager->SetString(WINDOW_MAIL_RESULT, window_mail_result_list_dummy_03_01-i ,IDS_SEND_ERROR5);
			break;

		}
	}

}

void cMAIL::Back_Result_Page(void)
{
	if(m_Result_Page == 0) return;
	--m_Result_Page;
	Reflash_Result();
}

void cMAIL::Next_Result_Page(void)
{
	if(m_Result_Page+1 >= m_Result_Total_Page) return;
	++m_Result_Page;
	Reflash_Result();
}

