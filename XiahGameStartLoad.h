#pragma once

#include "XiahGame_StepObject.h"

enum eGameStartLoadEnum
{
	eStartLoad_Start,

	eStartLoad_TaewoolLogoFadeOut,	// 이름을 잘못 붙임.
	eStartLoad_TaewoolLogoFadeIn,

	eStartLoad_XiahLogoFadeOut,
	eStartLoad_XiahLogoFadeIn,

	eStartLoad_China_WarnningIn,	// 중국용 경고 메세지 
	eStartLoad_China_WarnningOut,

	eStartLoad_Load1,
	eStartLoad_LoadLogoFadeIn,
	eStartLoad_Load2,

	eStartLoad_End
};


//////////////////////////////////////////////////////////////////////////
// 이녀석이 로고 출력 및 첨 로딩을 담당한다. 
class CXiahGameStartLoad : public CXiahGame_StepObject
{
	LPDIRECT3DVERTEXBUFFER9		m_pVB1;		// Taewool Logo
	LPDIRECT3DVERTEXBUFFER9		m_pVB2;		// Xiah Logo

	LPDIRECT3DTEXTURE9			m_pTexture1;
	LPDIRECT3DTEXTURE9			m_pTexture2;
	LPDIRECT3DTEXTURE9			m_pTexture3;

public:
	CXiahGameStartLoad();
	virtual ~CXiahGameStartLoad();

	BOOL Release();
	BOOL Init();

	BOOL Update();
	BOOL Render();

	BOOL ConnectAuthServer();

	// variables
	BYTE	m_byCurStep;
	DWORD	m_dwElapsedTime;
	DWORD	m_dwPrevTime;
	DWORD	m_dwBackTime;

};

extern CXiahGameStartLoad* g_StartLoad;