#pragma once

#include "XiahArrayIndex.h"

//////////////////
struct sMugongInfo
//////////////////
{
	DWORD	m_dwMugongID;
	BYTE	m_byMugongLevel;
	BYTE	m_byMugongType;		
};



typedef map<DWORD, sMugongInfo*> MugongMap;

/**
 * \ingroup XiahClient
 *
 * \date 2004-07-15
 */
class CMugong
{
public:

	CMugong(void);
	~CMugong(void);

	void InsertMugong( DWORD dwMugongID, BYTE bType, BYTE bLevel);

	void CheckMugongSelected();
	void CheckMugongUnSelected();	
	void RefreshMugongContent();							// 자잘한 무공 업데이트는 여기서 다 하자
	void RefreshMugongOutside();
	void RefreshMugongInside();
	int	FindMugongByIndex( BYTE MugongType, BYTE Index);	
	BOOL IsLearnedMugong( DWORD dwMugongID);
	BYTE GetMugongLevelOfLearnedMugong( DWORD dwMugongID);
	void SetMugongToolTip( int nMugongID, int nFrameID, int nControlID);	

	void SetFiveElementToolTip(int nMugongID, int nFrameID, int nControlID);

	void SetRebirthToolTip(int nMugongID, int nFrameID, int nControlID);

	DWORD FindRebirthMugongByIndex(BYTE bySeq);
	DWORD Find2ThRebirthMugongByIndex(BYTE bySeq);
	DWORD KeepUpIconTimer;	// HO_0702_07 등급 아이콘 추가 : 게임내 심의등급 표기


	//HT_0403 : 지속형 무공 시전 아이콘 
	void CreateKeepUpMugongIcon();
	void DrawKeepUpMugongIcon();
	void CreateKeepUpIcon();
	void DrawKeepUpIcon();

	//HT_0711 : 진각성 무공
	MugongMap	m_map2ThRebirthMugong;	// level이 1이상인 흡성신공을 담는다

private:

	void ArrayText(sArrayData* pData, int nIndex, int nFrameID, int nControlID);
	void SetMugongGUI(DWORD dwMugongID, BYTE bType, BYTE bLevel);

private:
	MugongMap	m_mapMugong;			// level이 1이상인 무공을 담는다	
	
	//HT_0403 : 지속형 무공 시전 아이콘
	LPDIRECT3DVERTEXBUFFER9	m_pKeepUpVB[15];
	LPDIRECT3DVERTEXBUFFER9	m_pKeepUpIconVB;
};
