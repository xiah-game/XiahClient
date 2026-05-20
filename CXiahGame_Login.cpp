/*===================================================================================================
										CXiahGame_Login.cpp
-----------------------------------------------------------------------------------------------------
	date :	2004/04/22  14:01
  Author :	
	
 Purpose :	Xiah 내부 로그인 & 서버 선택 처리 클래스
	
===================================================================================================*/

#include "precompile.h"
#include ".\cxiahgame_login.h"

#include "InterfaceDefine.h"
#include "AppData.h"

#include "XiahSocket.h"
#include "XiahGame_Handler_Sender.h"

#include "CharacterInfo.h"
#include <assert.h>

extern sString	g_ServerName;


CXiahGame_Login::CXiahGame_Login(void) : m_nStep(LOGIN), m_bSelectServer(250), m_bSelectChannel(250), m_nServerCount(0), m_nChannelCount(0),
									m_pServerSelectVB(NULL), m_pChannelSelectVB(NULL), m_nPrevToolTipPos(-1)
{
#ifdef _DEBUG_2		// _DEBUG_2
	m_nUserCount = 0;
#endif

	m_mServerList.clear();

	RECT rtRect;
	rtRect.left		= 414;
	rtRect.right	= 467;
	rtRect.top		= 232;
	rtRect.bottom	= 270;

	for(register int i=0; i < 5; ++i)
	{
		m_rtServer1[i].left		= rtRect.left;
		m_rtServer1[i].right	= rtRect.right;
		m_rtServer1[i].top		= rtRect.top + 20 * i;
		m_rtServer1[i].bottom	= m_rtServer1[i].top + 20;

		m_TextServer1[i].SetParentRect(&m_rtServer1[i]);
	}

	rtRect.left		= 480;
	rtRect.right	= 600;
	rtRect.top		= 232;
	rtRect.bottom	= 270;

	for(register int i=0; i < 20; ++i)
	{
		m_rtChannel1[i].left	= rtRect.left;
		m_rtChannel1[i].right	= rtRect.right;
		m_rtChannel1[i].top		= rtRect.top + 20 * i;
		m_rtChannel1[i].bottom	= m_rtChannel1[i].top + 20;

		m_TextChannel1[i].SetParentRect(&m_rtChannel1[i]);

		m_rtChannel2[i].left	= rtRect.right + 10;
		m_rtChannel2[i].right	= rtRect.right + 70;
		m_rtChannel2[i].top		= rtRect.top + 20 * i;
		m_rtChannel2[i].bottom	= m_rtChannel2[i].top + 20;
		m_TextChannel2[i].SetParentRect(&m_rtChannel2[i]);
	}

	g_pDirect3DDevice->CreateVertexBuffer(4*sizeof(VT_TLVertex), 0, D3DFVF_TLVERTEX, D3DPOOL_MANAGED, &m_pServerSelectVB, NULL);
	g_pDirect3DDevice->CreateVertexBuffer(4*sizeof(VT_TLVertex), 0, D3DFVF_TLVERTEX, D3DPOOL_MANAGED, &m_pChannelSelectVB, NULL);

	// test code
	m_dwTime = 0;

	srand(timeGetTime());

	for(int i=0; i < 30; ++i)
	{
		g_pDirect3DDevice->CreateVertexBuffer(4*sizeof(VT_TLVertex), 0, D3DFVF_TLVERTEX, D3DPOOL_MANAGED, &m_sPetals[i].pPetalVB, NULL);

		m_sPetals[i].nAniType = rand() % 5;

		m_sPetals[i].nPattern = rand() % 15;
		m_sPetals[i].fX = (rand() % 200) + 530;
	}

#ifndef MASTER
	sString str;
	str.printf("개발 버전 - 빌드 %s %s", __DATE__, __TIME__);
	m_TextInfo.SetText(10, 15, str, GetFont("굴림", 14), D3DCOLOR_XRGB(255, 255, 0), 15);

	TCHAR szDir[512] = {0,};
	GetCurrentDirectory(512, szDir);
	str.printf(_T("실행경로 - %s"), szDir);
	m_DirInfo.SetText(10, 40, str, GetFont("굴림", 14), D3DCOLOR_XRGB(255, 255, 0), 15);
#endif
}

CXiahGame_Login::~CXiahGame_Login(void)
{
	ListDestroy();

	if(m_pServerSelectVB)
		m_pServerSelectVB->Release(), m_pServerSelectVB = NULL;

	if(m_pChannelSelectVB)
		m_pChannelSelectVB->Release(), m_pChannelSelectVB = NULL;

	for(register int i=0; i < 5; ++i)
		m_TextServer1[i].Release();

	for(register int i=0; i < 20; ++i)
	{
		m_TextChannel1[i].Release();
		m_TextChannel2[i].Release();
	}

	for(int i=0; i < 30; ++i)
	{
		m_sPetals[i].pPetalVB->Release();
		m_sPetals[i].pPetalVB = NULL;
	}

#ifndef MASTER
	m_TextInfo.Release();
	m_DirInfo.Release();
#endif
}

BOOL CXiahGame_Login::Update()
{
	static bool bFirst = false;
	if(!bFirst)
	{
		Init_Login();
		bFirst = true;
	}

	g_pUIManager->UpDate();

	CheckServerList();

	// test code
	DWORD dwCur = timeGetTime();

	if(m_dwTime+50 < dwCur)
	{
		m_dwTime = dwCur;

		VT_TLVertex Vertex[4];	

		Vertex[0].diffuse = Vertex[1].diffuse = Vertex[2].diffuse = Vertex[3].diffuse = D3DCOLOR_ARGB(255, 255, 255, 255);

		Vertex[0].tex = Vector2(0, 0);
		Vertex[1].tex = Vector2(1, 0);
		Vertex[2].tex = Vector2(0, 1);
		Vertex[3].tex = Vector2(1, 1);
		

		if((rand() % 10) == 0)
		{
			int n = rand() % 30;
			if(!m_sPetals[n].nState)
			{
				m_sPetals[n].nState = 1;

				if(m_nStep == SERVER_SELECT)
					m_sPetals[n].fX = (rand() % 200) + 190;
				else
					m_sPetals[n].fX = (rand() % 200) + 530;
			}			
		}		

		for(int i=0; i < 30; ++i)
		{
			if(!m_sPetals[i].nState)
				continue;

			if((m_sPetals[i].nPatternNum % 5) == 0)
				++m_sPetals[i].nAniNum;

			if(m_sPetals[i].nAniNum >= 5)
				m_sPetals[i].nAniNum = 0;

			// 꽃잎 떨어지는 패턴 (단순하지..히히)
			switch(m_sPetals[i].nPattern)
			{
			case 0:
				m_sPetals[i].fX -= sin((m_sPetals[i].nPatternNum++) * 3.14/180) * 4.5f;
				m_sPetals[i].fY += 4;
				break;
			case 1:
				m_sPetals[i].fX -= sin((m_sPetals[i].nPatternNum++) * 3.14/180) * 3.5f;
				m_sPetals[i].fY += 4.5f;
				break;
			case 2:
				m_sPetals[i].fX -= cos((m_sPetals[i].nPatternNum++) * 3.14/180) * 4.3f;
				m_sPetals[i].fY += 4.8f;
				break;
			case 3:
				m_sPetals[i].fX -= cos((m_sPetals[i].nPatternNum++) * 3.14/180) * 3.3f;
				m_sPetals[i].fY += 5.5f;
				break;
			case 4:
				m_sPetals[i].fX -= sin((m_sPetals[i].nPatternNum++) * 3.14/180) * 2.5f;
				m_sPetals[i].fY += 4;
				break;
			case 5:
				m_sPetals[i].fX -= sin((m_sPetals[i].nPatternNum++) * 3.14/180) * 3.9f;
				m_sPetals[i].fY += 4.1f;
				break;
			case 6:
				m_sPetals[i].fX -= sin((m_sPetals[i].nPatternNum++) * 3.14/180) * 2.4f;
				m_sPetals[i].fY += 3.2f;
				break;
			case 7:
				m_sPetals[i].fX -= cos((m_sPetals[i].nPatternNum++) * 3.14/180) * 1.3f;
				m_sPetals[i].fY += 5.5f;
				break;
			case 8:
				m_sPetals[i].fX -= cos((m_sPetals[i].nPatternNum++) * 3.14/180) * 3.8f;
				m_sPetals[i].fY += 4.5f;
				break;
			case 9:
				m_sPetals[i].fX -= cos((m_sPetals[i].nPatternNum++) * 3.14/180) * 3.68f;
				m_sPetals[i].fY += 2.5f;
				break;
			case 10:
				m_sPetals[i].fX -= cos((m_sPetals[i].nPatternNum++) * 3.14/180) * 3.5f + 0.2f;
				m_sPetals[i].fY += 2.5f;
				break;
			case 11:
				m_sPetals[i].fX -= cos((m_sPetals[i].nPatternNum++) * 3.14/180) * 2.8f;
				m_sPetals[i].fY += 4.2f;
				break;
			case 12:
				m_sPetals[i].fX -= sin((m_sPetals[i].nPatternNum++) * 3.14/180) * 3.1f;
				m_sPetals[i].fY += 3.95f;
				break;
			case 13:
				m_sPetals[i].fX -= sin((m_sPetals[i].nPatternNum++) * 3.14/180) * 2.9f;
				m_sPetals[i].fY += 3.11f;
				break;
			case 14:
				m_sPetals[i].fX -= sin((m_sPetals[i].nPatternNum++) * 3.14/180) * 1.9f;
				m_sPetals[i].fY += 4.1f;
				break;
			default:
				break;
			}	

			Vertex[0].pos = Vector4(m_sPetals[i].fX,		m_sPetals[i].fY,		0, 1);
			Vertex[1].pos = Vector4(m_sPetals[i].fX+50.f,	m_sPetals[i].fY,		0, 1);
			Vertex[2].pos = Vector4(m_sPetals[i].fX,		m_sPetals[i].fY+50.f,	0, 1);
			Vertex[3].pos = Vector4(m_sPetals[i].fX+50.f,	m_sPetals[i].fY+50.f,	0, 1);

			VOID* pVertices;
			if(!FAILED(m_sPetals[i].pPetalVB->Lock( 0, sizeof(Vertex), (void**)&pVertices, 0)))
			{
				memcpy(pVertices, Vertex, sizeof(Vertex) );
				m_sPetals[i].pPetalVB->Unlock();
			}

			// 좌,우 아래 끝나면 위치 초기화
			if(m_sPetals[i].fY > 800 || m_sPetals[i].fX > 1024 || m_sPetals[i].fX < 0)
			{
				m_sPetals[i].fY = -50.0f;

				if(m_nStep == SERVER_SELECT)
					m_sPetals[i].fX = (rand() % 200) + 190;
				else
					m_sPetals[i].fX = (rand() % 200) + 530;
			}
		}

	}

	

	return true;
}

BOOL CXiahGame_Login::Render()
{
	g_pUIManager->Draw();

	/////////////////////////////////////////////////////////////////////////////////////////////////////
	if(m_nStep == SERVER_SELECT)
	{
		for(register int i=0; i < 5; ++i)
		{
			m_TextServer1[i].Render();			
		}

		for(register int i=0; i < 20; ++i)
		{
			m_TextChannel1[i].Render();
			m_TextChannel2[i].Render();			
		}

		/////////////////////////////////////////////////////////////////////////////////////////////////////
		if(m_bSelectServer != 250)
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
			g_Device.SetStreamSource( m_pServerSelectVB, sizeof(VT_TLVertex));
			g_Device.SetFVF(D3DFVF_TLVERTEX);
			//g_pDirect3DDevice->SetFVF( D3DFVF_TLVERTEX);
			g_pDirect3DDevice->DrawPrimitive( D3DPT_TRIANGLESTRIP, 0, 2);	
		}

		if(m_bSelectChannel != 250)
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
			g_Device.SetStreamSource( m_pChannelSelectVB, sizeof(VT_TLVertex));
			g_Device.SetFVF(D3DFVF_TLVERTEX);
			//g_pDirect3DDevice->SetFVF( D3DFVF_TLVERTEX);
			g_pDirect3DDevice->DrawPrimitive( D3DPT_TRIANGLESTRIP, 0, 2);	
		}
	}
/////////////////////////////////////////////////////////////////////////////////////////////////////

	if(m_bSelectServer != 250)
	{
		std::map<BYTE, sServerList*>::iterator iter = m_mServerList.find(m_bSelectServer);

		if(iter != m_mServerList.end())
		{
			sServerList *pServerInfo = (*iter).second;

			assert(pServerInfo);

			int nCount = pServerInfo->mChannelList.size();

			for(register int i=0; i < nCount; ++i)
			{
				DrawToolTip(i);
			}
		}

	}
		
	g_pUIManager->SpecialDraw();

	// 꽃잎
	if(m_nStep == LOGIN || m_nStep == SERVER_SELECT)
	{
		g_pDirect3DDevice->SetRenderState(D3DRS_ZENABLE, FALSE);
		g_pDirect3DDevice->SetRenderState(D3DRS_FOGENABLE, FALSE);

		g_pDirect3DDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
		g_pDirect3DDevice->SetRenderState(D3DRS_DIFFUSEMATERIALSOURCE, D3DMCS_COLOR1);
		g_pDirect3DDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
		g_pDirect3DDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

		g_pDirect3DDevice->SetSamplerState(0,  D3DSAMP_ADDRESSU , D3DTADDRESS_CLAMP);
		g_pDirect3DDevice->SetSamplerState(0,  D3DSAMP_ADDRESSV , D3DTADDRESS_CLAMP);

		g_Device.SetFVF(D3DFVF_TLVERTEX);
		//g_pDirect3DDevice->SetFVF(D3DFVF_TLVERTEX);

		for(int i=0; i < 30; ++i)
		{
			int nResID =0;

			switch(m_sPetals[i].nAniType)
			{
			case 0:	nResID = 1368;	break;
			case 1:	nResID = 1373;	break;
			case 2:	nResID = 1378;	break;
			case 3:	nResID = 1383;	break;
			case 4:	nResID = 1388;	break;
			default:			
				break;
			}

			g_Device.SetTexture(0, XiahPak::GetTexture(nResID+m_sPetals[i].nAniNum, TRUE));
			//g_pDirect3DDevice->SetTexture(0, XiahPak::GetTexture(nResID+m_sPetals[i].nAniNum, TRUE));
			g_Device.SetStreamSource( m_sPetals[i].pPetalVB, sizeof(VT_TLVertex));		
			g_pDirect3DDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);	
		}
	}

#ifndef MASTER		// MASTER	
	m_TextInfo.Render();
	m_DirInfo.Render();
#endif

	return true;
}

/**
 * 리스트 제거
 */
void CXiahGame_Login::ListDestroy()
{
	register std::map<BYTE, sServerList*>::iterator iter = m_mServerList.begin();

	for(; iter != m_mServerList.end(); ++iter)
	{
		sServerList *pServerList = (*iter).second;

		delete pServerList, pServerList = NULL;
	}

	m_mServerList.clear();
}

/**
 * 서버(월드) 생성
 * \param bWorld 
 * \param strWorldName 
 * \param strWorldDes 
 */
void CXiahGame_Login::WorldCreate(const BYTE bWorld, const sString strWorldName, const sString strWorldDes)
{
	sServerList *pServerInfo = new sServerList;

	pServerInfo->strWorldName = strWorldName;
	pServerInfo->strWorldDes = strWorldDes;

	m_mServerList.insert(std::map<BYTE, sServerList*>::value_type(bWorld, pServerInfo));

	++m_nServerCount;

	Refresh();
}

/**
 * 채널 생성
 * \param bWorld 
 * \param bChannel 
 * \param strChannelName 
 * \param strChannelDes 
 * \param bAge 
 * \param wMaxUser 
 * \param dwUnitPort 
 * \param strUnitAddress 
 */
void CXiahGame_Login::ChannelCreate(const BYTE bWorld, const BYTE bChannel, const sString strChannelName, const sString strChannelDes, const BYTE bAge, const WORD wMaxUser, const DWORD dwUnitPort, const sString strUnitAddress)
{
	std::map<BYTE, sServerList*>::iterator iter = m_mServerList.find(bWorld);

	if(iter != m_mServerList.end())
	{
		sServerList *pServerInfo = (*iter).second;

		assert(pServerInfo);

		// Channel
		sChannelList *pChannelInfo = new sChannelList;

		pChannelInfo->strChannelName	= strChannelName;
		pChannelInfo->strChannelDes		= strChannelDes;
		pChannelInfo->bAge				= bAge;
		pChannelInfo->wMaxUser			= wMaxUser;
		pChannelInfo->dwUnitPort		= dwUnitPort;
		pChannelInfo->strUnitAddress	= strUnitAddress;
		pChannelInfo->bState			= 1; // Force online!

		// add
		pServerInfo->mChannelList.insert(std::map<BYTE, sChannelList*>::value_type(bChannel, pChannelInfo));

		//++m_bSelectChannel;

		// 리스트 갱신
		Refresh();
	}
	else
	{
		DBG_LogFile(_T("ChannelCreate fail"));
	}
}

/**
 * 채널 상태 정보
 * \param bWorld 
 * \param bChannel 
 * \param bState 
 * \param wUser 
 */
void CXiahGame_Login::ChannelState(const BYTE bWorld, const BYTE bChannel, const BYTE bState, const WORD wUser)
{
	std::map<BYTE, sServerList*>::iterator iter = m_mServerList.find(bWorld);	// 월드

	if(iter != m_mServerList.end())
	{
		sServerList *pServerInfo = (*iter).second;

		assert(pServerInfo);

		std::map<BYTE, sChannelList*>::iterator Channel_Iter = pServerInfo->mChannelList.find(bChannel);	// 채널

		if(Channel_Iter != pServerInfo->mChannelList.end())
		{
			sChannelList *pChannelInfo = (*Channel_Iter).second;

			assert(pChannelInfo);

			pChannelInfo->bState = bState;
			pChannelInfo->wUser = wUser;

			// 리스트 갱신
			Refresh();

#ifdef _DEBUG_2		// _DEBUG_2
			m_nUserCount += wUser;
			DBG_Put("%s 채널 유저수 %d  총 %d", pChannelInfo->strChannelName.data(), wUser, m_nUserCount);
#endif
		}
		else
		{
			DBG_LogFile(_T("ChannelState fail Channel %d"), bChannel);
		}
	}
	else
	{
		DBG_LogFile(_T("ChannelState fail World %d"), bWorld);
	}
}


/**
 * 서버 정보 갱신
 */
void CXiahGame_Login::Refresh()
{
	std::map<BYTE, sServerList*>::iterator ServerIter = m_mServerList.begin();

	// 서버 리스트
	for(int i=0; ServerIter != m_mServerList.end(); ++ServerIter, ++i)
	{
		sServerList *pServerInfo = (*ServerIter).second;

		assert(pServerInfo);

		m_TextServer1[i].SetText(0, 0, (LPCTSTR)pServerInfo->strWorldName, GetFont(IDS_DUDUM, 12), D3DCOLOR_XRGB(255, 255, 255));		
	}

	// 채널 리스트 제거
	for(register int j=0; j < 20; ++j)
	{
		m_TextChannel1[j].SetText(0, 0, _T(" "), GetFont(IDS_DUDUM, 12), D3DCOLOR_XRGB(255, 255, 255));
		m_TextChannel2[j].SetText(0, 0, _T(" "), GetFont(IDS_DUDUM, 12), D3DCOLOR_XRGB(255, 255, 255));
	}

	if(m_bSelectServer != 250)
	{
		m_nChannelCount = 0;

		// 서버
		std::map<BYTE, sServerList*>::iterator iter = m_mServerList.find(m_bSelectServer);

		if(iter != m_mServerList.end())
		{
			sServerList *pServerInfo = (*iter).second;

			assert(pServerInfo);

			// 채널
			std::map<BYTE, sChannelList*>::iterator ChannelIter = pServerInfo->mChannelList.begin();

			for(int i=0; ChannelIter != pServerInfo->mChannelList.end(); ++ChannelIter, ++i)
			{
				sChannelList *pChannelInfo = (*ChannelIter).second;

				assert(pChannelInfo);

				if(g_AppData.m_bAge >= pChannelInfo->bAge)			
					m_TextChannel1[i].SetText(0, 0, (LPCTSTR)pChannelInfo->strChannelName, GetFont(IDS_DUDUM, 12), D3DCOLOR_XRGB(255, 255, 255));
				else
					m_TextChannel1[i].SetText(0, 0, (LPCTSTR)pChannelInfo->strChannelName, GetFont(IDS_DUDUM, 12), D3DCOLOR_XRGB(255, 0, 0));

				LPCTSTR lpStrTemp;
				D3DCOLOR dwChannelColor = D3DCOLOR_XRGB(255, 255, 255);

				if(pChannelInfo->bState == 1)
				{
					int nUser = static_cast<int>((static_cast<float>(pChannelInfo->wUser) / static_cast<float>(pChannelInfo->wMaxUser)) * 100.0f);

					if(nUser <= 30)			// 원활
						lpStrTemp = IDS_LOGIN_SMOOTHLY;
					else if(nUser <= 60)	// 보통
						lpStrTemp = IDS_LOGIN_NORMAL;
					else if(nUser <= 90) 	// 혼잡
						lpStrTemp = IDS_LOGIN_CROWDED;
					else					// 정체
						lpStrTemp = IDS_LOGIN_CONGESTION;
				}
				else
				{
					lpStrTemp = IDS_LOGIN_EXAMINE;
					dwChannelColor = D3DCOLOR_XRGB(255, 0, 0);
				}

				m_TextChannel2[i].SetText(0, 0, lpStrTemp, GetFont(IDS_DUDUM, 12), dwChannelColor);

				++m_nChannelCount;
			}
		}
	}
}

/**
 * 서버 선택 검사
 */
void CXiahGame_Login::CheckServerList()
{
	if(XiahInput::g_bLButtonDown && !g_pUIManager->IsNotice())
	{
		for(register int i=0; i < m_nServerCount; ++i)	// 서버
		{
			if(m_rtServer1[i].PtInRect(XiahInput::g_ptMouse))
			{
				std::map<BYTE, sServerList*>::iterator iter = m_mServerList.begin();

				for(register int j=0; iter != m_mServerList.end(); ++iter, ++j)
				{
					if(i == j)
					{
						m_bSelectServer = iter->first;	// 선택 서버

						g_AppData.m_byWorldID = iter->first;
						break;
					}
				}

				m_bSelectChannel = 250;	// 타 서버 선택 때문에
				Refresh();

				MakeSelectVB(true, m_rtServer1[i]);

				m_nPrevToolTipPos = -1;
				return;
			}
		}

		for(register int i=0; i < m_nChannelCount; ++i)	// 채널
		{
			if(m_rtChannel1[i].PtInRect(XiahInput::g_ptMouse))
			{
				//	두번 선택시 접속
				if(m_bSelectChannel == i)
				{
					ConnectToServer();
				}
				else
				{
					std::map<BYTE, sServerList*>::iterator iter = m_mServerList.find(m_bSelectServer);

					if(iter != m_mServerList.end())
					{
						sServerList *pServerInfo = (*iter).second;

						assert(pServerInfo);

						std::map<BYTE, sChannelList*>::iterator ChannelIter = pServerInfo->mChannelList.begin();

						for(register int j=0; ChannelIter != pServerInfo->mChannelList.end(); ++ChannelIter, ++j)
						{
							if(i == j)
							{
								g_AppData.m_byChannelID = ChannelIter->first;
								break;
							}							
						}
					}

					m_bSelectChannel = i;

					MakeSelectVB(false, m_rtChannel1[i]);
					return;
				}
			} // if(m_rtChannel1[i].PtInRect(XiahInput::g_ptMouse))
		} // for(register int i=0; i < m_nChannelCount; ++i)	// 채널
	} // if(XiahInput::g_bLButtonDown)
}



/**
 * 선택
 * \param bType 
 * \param rtRect 
 */
void CXiahGame_Login::MakeSelectVB(const bool bType, sRect rtRect)
{
	VT_TLVertex Vertex[4];

	Vertex[0].pos = Vector4(rtRect.left-5, rtRect.top-3, 0, 1);
	Vertex[1].pos = Vector4(rtRect.right,  rtRect.top-3, 0, 1);
	Vertex[2].pos = Vector4(rtRect.left-5, rtRect.bottom-5, 0, 1);
	Vertex[3].pos = Vector4(rtRect.right,  rtRect.bottom-5, 0, 1);

	Vertex[ 0].diffuse = Vertex[ 1].diffuse = Vertex[ 2].diffuse = Vertex[ 3].diffuse = D3DCOLOR_ARGB(76, 168, 247, 179);

	Vertex[0].tex = Vector2( 0, 0);
	Vertex[1].tex = Vector2( 1, 0);
	Vertex[2].tex = Vector2( 0, 1);
	Vertex[3].tex = Vector2( 1, 1);

	LPDIRECT3DVERTEXBUFFER9 pVB = NULL;

	if(bType)
		pVB = m_pServerSelectVB;
	else
		pVB = m_pChannelSelectVB;

	VOID* pVertices;
	if(!FAILED(pVB->Lock( 0, sizeof(Vertex), (void**)&pVertices, 0)))
	{
		memcpy(pVertices, Vertex, sizeof(Vertex) );
		pVB->Unlock();
	}
}



/**
 * 로그인 창 설정
 */
void CXiahGame_Login::Init_Login()
{
	g_pUIManager->SetData(LOGIN_1, login1_edit_id, MAXSTRING, 17);
	g_pUIManager->SetData(LOGIN_1, login1_edit_password, MAXSTRING, 17);

	g_pUIManager->SetData(LOGIN_1, login1_edit_password, TYPE, 1);

	g_pUIManager->SetString(LOGIN_1, login1_ok, IDS_OK);
	g_pUIManager->SetString(LOGIN_1, login1_exit, IDS_CANCEL);

	g_pUIManager->SetString(LOGIN_1, login1_id_dummy, IDS_LOGIN_ID);
	g_pUIManager->SetString(LOGIN_1, login1_pw_dummy, IDS_LOGIN_PW);

	g_pUIManager->SetFocus(LOGIN_1);
	g_pUIManager->SetFocus(LOGIN_1, login1_edit_id);

	g_pUIManager->SetString(LOGIN_2, login2_ok, IDS_OK);
	g_pUIManager->SetString(LOGIN_2, login2_exit, IDS_CANCEL);

	g_pUIManager->SetString(LOGIN_2, login2_world_dummy, IDS_LOGIN_WORLD);
	g_pUIManager->SetString(LOGIN_2, login2_channel_dummy, IDS_LOGIN_CHANNEL);
	g_pUIManager->SetString(LOGIN_2, login2_status_dummy, IDS_STATUS);	

	// RS [11/29/2005] 비밀번호 변경
	g_pUIManager->SetData(LOGIN_1, login1_pw_change_edit_01, MAXSTRING, 17);
	g_pUIManager->SetData(LOGIN_1, login1_pw_change_edit_02, MAXSTRING, 9);
	g_pUIManager->SetData(LOGIN_1, login1_pw_change_edit_03, MAXSTRING, 9);
	g_pUIManager->SetData(LOGIN_1, login1_pw_change_edit_04, MAXSTRING, 9);

	g_pUIManager->SetData(LOGIN_1, login1_pw_change_edit_02, TYPE, 1);
	g_pUIManager->SetData(LOGIN_1, login1_pw_change_edit_03, TYPE, 1);
	g_pUIManager->SetData(LOGIN_1, login1_pw_change_edit_04, TYPE, 1);

	g_pUIManager->SetString(LOGIN_1, login1_pw_change_button_01, IDS_OK);
	g_pUIManager->SetString(LOGIN_1, login1_pw_change_button_02, IDS_CANCEL);

	g_pUIManager->SetString(LOGIN_1, login1_pw_change_button, IDS_PW_CHANGE_BUTTON);

	g_pUIManager->SetString(LOGIN_1, login1_pw_change_dummy_01, IDS_LOGIN_ID);
	g_pUIManager->SetString(LOGIN_1, login1_pw_change_dummy_02, IDS_LOGIN_PW);

	g_pUIManager->SetString(LOGIN_1, login1_pw_change_dummy_03, IDS_PW_CHANGE_NEW1);
	g_pUIManager->SetString(LOGIN_1, login1_pw_change_dummy_04, IDS_PW_CHANGE_NEW2);

	g_pUIManager->Hide(LOGIN_1, login1_pw_change_back);
	g_pUIManager->Hide(LOGIN_1, login1_pw_change_button_01);
	g_pUIManager->Hide(LOGIN_1, login1_pw_change_button_02);

	g_pUIManager->Hide(LOGIN_1, login1_pw_change_dummy_01);
	g_pUIManager->Hide(LOGIN_1, login1_pw_change_dummy_02);
	g_pUIManager->Hide(LOGIN_1, login1_pw_change_dummy_03);
	g_pUIManager->Hide(LOGIN_1, login1_pw_change_dummy_04);

	g_pUIManager->Hide(LOGIN_1, login1_pw_change_edit_01);
	g_pUIManager->Hide(LOGIN_1, login1_pw_change_edit_02);
	g_pUIManager->Hide(LOGIN_1, login1_pw_change_edit_03);
	g_pUIManager->Hide(LOGIN_1, login1_pw_change_edit_04);
}


/**
 * 서버 접속
 */
void CXiahGame_Login::ConnectToServer()
{
	if(m_bSelectServer != 250 && m_bSelectChannel != 250)
	{
		XiahNetwork::DisconnectFromServer();	// 인증서버 접속 끊기

		// 서버 (월드)
		std::map<BYTE, sServerList*>::iterator iter = m_mServerList.find(m_bSelectServer);

		if(iter != m_mServerList.end())
		{
			sServerList *pServerInfo = (*iter).second;

			assert(pServerInfo);

			// 채널
			std::map<BYTE, sChannelList*>::iterator ChannelIter = pServerInfo->mChannelList.find(g_AppData.m_byChannelID);

			if(ChannelIter != pServerInfo->mChannelList.end())
			{
				sChannelList *pChannelInfo = (*ChannelIter).second;

				assert(pChannelInfo);
				assert(pChannelInfo->wMaxUser);

				// 서버 접속 비율 계산
				int nUser = static_cast<int>((static_cast<float>(pChannelInfo->wUser) / static_cast<float>(pChannelInfo->wMaxUser)) * 100.0f);

				if(nUser > 90)
				{
					g_pUIManager->ShowNotice(IDS_LOGIN_SERVER_MANY);						
					return;
				}

				// 나이와 서버 상태 검사
				if(g_AppData.m_bAge >= pChannelInfo->bAge && pChannelInfo->bState)	
				{
					g_ServerName = pChannelInfo->strChannelName;	// 종료 창에 사용될 서버명

					// 접속 시도
                    DBG_LogFile(_T("Connecting to %s : %d"), pChannelInfo->strUnitAddress.c_str(), pChannelInfo->dwUnitPort);
					if(!XiahNetwork::ConnectToServer(pChannelInfo->strUnitAddress, pChannelInfo->dwUnitPort))
					{
                        DBG_LogFile(_T("ConnectToServer returned FALSE"));
						pChannelInfo->bState = 0;
						g_pUIManager->ShowNotice(IDS_LOGIN_SERVER_STOP);
						Refresh();
						return;
					}
                    DBG_LogFile(_T("ConnectToServer returned TRUE. SetStep = NONE."));

					// 꽃잎 떄문에
					SetStep(NONE);
				}
				else
				{
					// 나이 또는 서버 상태 알림
					if(!(g_AppData.m_bAge >= pChannelInfo->bAge))
					{
						TCHAR strTemp[128] = {0,};
						_stprintf(strTemp, IDS_LOGIN_AGE, pChannelInfo->bAge);
						g_pUIManager->ShowNotice(strTemp);
						return;
					}

					if(!pChannelInfo->bState)
					{
						g_pUIManager->ShowNotice(IDS_LOGIN_SERVER_STOP);
						return;
					}
				}
			}
			else
				return;
		}
		else
			return;
	}
}




/**
 * 툴팁 설정
 * \param nPos 위치
 */
void CXiahGame_Login::SetToolTip(const int nPos)
{
	std::map<BYTE, sServerList*>::iterator iter = m_mServerList.find(m_bSelectServer);

	if(iter != m_mServerList.end())
	{
		sServerList *pServerInfo = (*iter).second;

		std::map<BYTE, sChannelList*>::iterator ChannelIter = pServerInfo->mChannelList.begin();

		for(register int j=0; ChannelIter != pServerInfo->mChannelList.end(); ++ChannelIter, ++j)
		{
			if(nPos == j)
			{
				break;
			}							
		}

		if(ChannelIter != pServerInfo->mChannelList.end())
		{
			sChannelList *pChannelInfo = (*ChannelIter).second;

			sRect rtRegion = m_rtChannel1[nPos];

			rtRegion.left += 45;
			rtRegion.right += 100;
			rtRegion.top += 10;

			g_MainCharInfo.m_pToolTip->SetToolTip(8, &rtRegion, 1, pChannelInfo->strChannelName.data(), D3DCOLOR_XRGB(255, 255, 255), 2);
			g_MainCharInfo.m_pToolTip->AddToolTip(pChannelInfo->strChannelDes.data(), 12, D3DCOLOR_XRGB(255, 255, 255));

#ifdef _DEBUG_2		// _DEBUG_2
			TCHAR strTemp[128];
			_stprintf(strTemp, _T("%d , T = %d"), pChannelInfo->wUser, m_nUserCount);
			g_MainCharInfo.m_pToolTip->AddToolTip(strTemp, 12, D3DCOLOR_XRGB(255, 10, 10));
#endif
		}
		else
			return;
	}
	else
		return;

}


/**
 * 툴팁 출력
 * \param nPos 위치
 */
void CXiahGame_Login::DrawToolTip(const int nPos)
{
	if(m_rtChannel1[nPos].PtInRect(XiahInput::g_ptMouse))
	{
		if(nPos != m_nPrevToolTipPos)
		{
			m_nPrevToolTipPos = nPos;

			SetToolTip(nPos);
		}

		g_MainCharInfo.m_pToolTip->Draw();
	}
	else
	{
		if(m_nPrevToolTipPos == nPos)
			m_nPrevToolTipPos = -1;
	}
}