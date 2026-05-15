#pragma once


// 화면 번쩍효과를 위한 클래스

class	cEFFECT_SPOT
{
public :
	cEFFECT_SPOT();
	~cEFFECT_SPOT();

	// 번쩍 시간, 색깔
	BOOL Start(D3DCOLOR color,DWORD time = 1000);
	
	BOOL Update();
	void Render();

private :
	DWORD	AxisTime;			// 기준시간
	DWORD	FadeTimeLength;
	
	D3DCOLOR FadeColor;
	LPDIRECT3DVERTEXBUFFER9	m_VB;
};

extern cEFFECT_SPOT	*g_effect_spot;

/*

// Loading LOGO

class cTITLE
{
public:
	cTITLE();
	~cTITLE();
	
	BOOL	Create();
	BOOL 	Render();
	BOOL	Destroy();
	BOOL	ChangeTexture(LPDIRECT3DTEXTURE9 pTexture);
	
private:
	LPDIRECT3DTEXTURE9	m_pTexture;
	LPDIRECT3DVERTEXBUFFER9	m_VB;
};

*/


//
class cTRANS_BOX
{
public :
	cTRANS_BOX();
	~cTRANS_BOX();

	BOOL	Create();
	BOOL	Destroy();

private :
	float	top,bottom,left,right;

	D3DCOLOR FadeColor;
	LPDIRECT3DVERTEXBUFFER9	m_VB;
};


//////////////////////////////////////////////////////////////////////////

extern BOOL	Update_Special_Effect();
extern BOOL	Make_Special_Effect(D3DCOLOR col);