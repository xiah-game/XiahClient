#pragma once
#include <Windows.h>
#include "fmod.h"
#include "fmod_errors.h"

// 상태
#define	BGM_PLAYING		0
#define	BGM_STOPED		1
#define	BGM_ENDPLAY		2
#define	BGM_FADEOUT		3
#define	BGM_FADEIN		4

// USE FMOD
class cBGM
{
public:

	cBGM();
	~cBGM();

	FSOUND_STREAM *GetStream() { return stream; }
	int		GetChannel() {return m_channel;	}
	int		GetState();
	void	SetState(int state);

	BOOL	GetState_Playing();
	BOOL	GetState_Stopped();

	BOOL	SetVolume(int vol);
	BOOL	Init(TCHAR *filename);
	BOOL	Play(BOOL flag);
	BOOL 	Stop();
	void	Close();

	BOOL 	FadeIn(long time);
	BOOL 	FadeOut(long time);
	DWORD 	Update();

protected :
	FSOUND_STREAM *stream;
	long	m_time;
	float	m_para;
	int		m_state;
	int		m_channel;
		
};

extern BOOL	g_bBGMForce;

extern BOOL IntializeXiahBGM(int vol);	
extern BOOL UninitializeXiahBGM();

extern int Get_BGM_Bank();
extern BOOL Play_BGM(TCHAR *name, int mode);
extern BOOL Change_BGM(TCHAR *name);
extern BOOL Stop_BGM();
extern BOOL Stop_BGM(int i);
extern BOOL SetBGMVolume(int vol);
