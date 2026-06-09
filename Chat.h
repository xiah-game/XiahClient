
#pragma once

#include <deque>

using namespace std;

#define NOCHAT			0
#define SMALLCHAT		1
#define MEDIUMCHAT		2
#define LARGECHAT		3

#define MAXCHATSIZE		50
#define SMALLCHATSIZE	6 //7
#define MEDIUMCHATSIZE	10
#define LARGECHATSIZE	41 //44

#define CHAT_VERTICAL_DISTANCE 15



struct sChat 
{
	DWORD	dwSender;
	sString szContent;
	BYTE	byType;
};

typedef deque<sChat*> ChatList;

/**
 * \ingroup XiahClient
 *
 * \date 2004-07-15
 */
class CChat
{
private:
	ChatList	m_listChat;
	BYTE		m_byType;					// 몇 개까지의 ChatMsg를 보여줄건지
	sRect		m_rtRegion[ LARGECHATSIZE];
	CText2D		m_text2D[ LARGECHATSIZE];

	BOOL		m_bChatFlag;			// 현재 에디팅중이다:TRUE
	DWORD		m_dwTimeInterval;
public:
	// get
	BYTE GetChatType() { return m_byType;};
	BOOL GetChatFlag() { return m_bChatFlag;};

	// set
	void UpdateChat();
	void SetChatType( BYTE byType);
	void SetChatMsg( DWORD dwSender, BYTE byType, sString sContent, sString SenderName, DWORD listner, sString listenrName);
	void UpdateTex();
	void ChatShow();
	void SetChatFlag( BOOL bFlag) { m_bChatFlag = bFlag;};
	void DeleteChatByTime();

	void Clear();

	CChat(void);
	~CChat(void);
};