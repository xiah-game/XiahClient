/*===================================================================================================
										CXiahGame_Login.h
-----------------------------------------------------------------------------------------------------
	date :	2004/04/22  14:00
  Author :	
	
 Purpose :	Xiah 내부 로그인 & 서버 선택 처리 클래스
	
===================================================================================================*/

#if defined (_MSC_VER) && (_MSC_VER >= 1000)
#pragma once
#endif
#ifndef _INC_XIAHGAME_LOGIN_INCLUDED
#define _INC_XIAHGAME_LOGIN_INCLUDED

#include "XiahGameEngineBase.h"
#include "xiahgame_stepobject.h"

/**
 * \ingroup XiahClient
 *
 * \date 2004-04-22
 */
class CXiahGame_Login :
	public CXiahGame_StepObject
{
public:
	enum eLoginStep
	{
		LOGIN = 0,
		SERVER_SELECT,
		NONE
	};

private:
	struct sChannelList
	{
		sString strChannelName;		// 채널 명
		sString strChannelDes;		// 채널 상세 정보
		BYTE bState;				// 서버 상태 0-서버다운 1-서버온
		BYTE bAge;					// 나이
		WORD wUser;					// 현재 유저
		WORD wMaxUser;				// 최대 접속 가능수

		sString strUnitAddress;		// 유닛 서버 주소
		DWORD dwUnitPort;			// 포트

		inline sChannelList() : bAge(0), wUser(0), wMaxUser(0), dwUnitPort(0), bState(0)
		{
		};
	};

	struct sServerList
	{
		sString strWorldName;		// 월드 명
		sString strWorldDes;		// 월드 상세 정보

		std::map<BYTE, sChannelList*> mChannelList;		// 채널 리스트

		inline sServerList()
		{
			mChannelList.clear();
		};

		inline ~sServerList()
		{
			register std::map<BYTE, sChannelList*>::iterator iter = mChannelList.begin();

			for(; iter != mChannelList.end(); ++iter)
			{
				sChannelList *pList = (*iter).second;

				delete pList, pList = NULL;
			}

			mChannelList.clear();
		};
	};

	// 로그인 꽃잎
	struct sPetal
	{
		int nPattern;
		int nPatternNum;
		int nState;
		int nAniType;
		int nAniNum;
		float fX;
		float fY;		

		LPDIRECT3DVERTEXBUFFER9 pPetalVB;

		inline sPetal() : pPetalVB(NULL), fY(-50.0f), nAniType(0), nAniNum(0), nPattern(0), nState(0), nPatternNum(0)
		{			
		}

	};

public:
	CXiahGame_Login(void);
	virtual ~CXiahGame_Login(void);

	void Init_Login();

	virtual BOOL Update();
	virtual BOOL Render();

	void WorldCreate(const BYTE bWorld, const sString strWorldName, const sString strWorldDes);
	void ChannelCreate(const BYTE bWorld, const BYTE bChannel, const sString strChannelName, const sString strChannelDes, const BYTE bAge, const WORD wMaxUser, const DWORD dwUnitPort, const sString strUnitAddress);
	void ChannelState(const BYTE bWorld, const BYTE bChannel, const BYTE bState, const WORD wUser);

	void ConnectToServer();

	void SetStep(const int nStep)
	{
		m_nStep = nStep;
	};
	

private:

	// test code
	sPetal m_sPetals[30];
	DWORD m_dwTime;
	

	int m_nStep;

	BYTE m_bSelectServer;
	BYTE m_bSelectChannel;

	int m_nServerCount;
	int m_nChannelCount;

	LPDIRECT3DVERTEXBUFFER9	m_pServerSelectVB;
	LPDIRECT3DVERTEXBUFFER9	m_pChannelSelectVB;

	std::map<BYTE, sServerList*> m_mServerList;

	// temp 리스트 박스가 현재 엔진쪽에서 미 구현상태. -_-
	sRect m_rtServer1[5];
	CText2D m_TextServer1[5];

	sRect m_rtChannel1[20];
	sRect m_rtChannel2[20];

	CText2D	m_TextChannel1[20];
	CText2D	m_TextChannel2[20];

	/////////////////////////////////////////////////////////////////////////////////////////////////////
	int m_nPrevToolTipPos;

	void ListDestroy();
	void Refresh();

	void CheckServerList();

	void MakeSelectVB(const bool bType, sRect rtRect);

	void SetToolTip(const int nPos);
	void DrawToolTip(const int nPos);

protected:

#ifdef _DEBUG_2		// _DEBUG_2
	int m_nUserCount;
#endif

#ifndef MASTER		// MASTER
	CText2D	m_TextInfo;
	CText2D	m_DirInfo;
#endif
};

#endif	// _INC_XIAHGAME_LOGIN_INCLUDED
