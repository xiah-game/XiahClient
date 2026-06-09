//
//	BGM CORE
//
#include <Windows.h>
#include <process.h>
#include "XiahDebug.h"
#include "xiahbgmcore.h"

#define	FADE_OUT_BGM_RATE		3
#define	FADE_IN_RATE			3

cBGM	*g_BGM;

static int now_playing = 0;
static int Master_BGM_Vol;

CRITICAL_SECTION	cr_bgm;
static BOOL		Now_Change;

BOOL IntializeXiahBGM(int vol);	
BOOL UninitializeXiahBGM();

int Get_BGM_Bank();
BOOL Play_BGM(TCHAR *name, int mode);
BOOL Change_BGM(TCHAR *name);
BOOL Stop_BGM();
BOOL Stop_BGM(int i);
BOOL SetBGMVolume(int vol);

signed char F_CALLBACKAPI BGMCallback(FSOUND_STREAM *stream, void *buff, int len, int param)
{
	g_bBGMForce = TRUE;
	return TRUE;
}


void Fade_Out_Thread(void *parameter)
{
	int Old_Music_Vol;
	int handle;
	int channel;
	unsigned long tick = 0;

	handle = (int)parameter;
	g_BGM[handle].SetState(BGM_FADEOUT);
	channel = g_BGM[handle].GetChannel();
	Old_Music_Vol = FSOUND_GetVolume(channel);
	// callback 금지
	FSOUND_Stream_SetEndCallback(g_BGM[handle].GetStream(), 0,0);

	while(1)
	{
		Sleep(1);
		if( tick + 10 < timeGetTime() )
		{
			tick = timeGetTime();
			if(Old_Music_Vol > 0 )
			{
				if(Old_Music_Vol-FADE_OUT_BGM_RATE > 0)			
					FSOUND_SetVolume(channel,Old_Music_Vol-=FADE_OUT_BGM_RATE);
				else
				{
					Old_Music_Vol = 0;
					FSOUND_SetVolume(channel,Old_Music_Vol);
				}			
			}
			else
				break;
		}
	}
	// Music Stop
	FSOUND_Stream_Stop(g_BGM[handle].GetStream());
	g_BGM[handle].SetState(BGM_STOPED);
	_endthread();
}

void Fade_In_Thread(void *parameter)
{
	int channel;
	int Music_Vol = 0;
	int handle;
	handle = (int)parameter;
	unsigned long tick = 0;

	g_BGM[handle].SetState(BGM_FADEIN);
	channel = g_BGM[handle].GetChannel();
	FSOUND_SetVolume(channel,0);
	FSOUND_SetPaused(channel,FALSE);

	while(1)
	{
		Sleep(1);
		if( tick + 10 < timeGetTime() )
		{
			tick = timeGetTime();

			if( Music_Vol < Master_BGM_Vol - FADE_IN_RATE )
			{
				FSOUND_SetVolume(channel,Music_Vol+=FADE_IN_RATE);
			}
			else
				break;
		}

	}
	g_BGM[handle].SetState(BGM_PLAYING);
	now_playing = 1-now_playing;
	_endthread();
}


// 플레이 한다. (0:No loop, 1:Loop)
BOOL Play_BGM(TCHAR *name, int mode)
{
	FSOUND_STREAM *stream;

	int select =0;

	if( g_BGM[0].GetState() == BGM_STOPED ) 
		select = 0;
	else
		if( g_BGM[1].GetState() == BGM_STOPED ) 
			select = 1;


	g_BGM[select].Init(name);
	stream = g_BGM[select].GetStream();

	// LOOP 설정
	if(mode == 1) 
		FSOUND_Stream_SetMode(stream,FSOUND_LOOP_NORMAL);

	//FSOUND_Stream_SetEndCallback(stream, BGMCallback,0);
	g_BGM[select].Play(FALSE);
	now_playing = select;
	SetBGMVolume(Master_BGM_Vol);

	return TRUE;
}

// 기존에 플레이 하던것이 있으면 overlap 한다.
BOOL Change_BGM(TCHAR *name)
{
	FSOUND_STREAM *stream;

	// 현재 플레이 중일때만 아웃
	if(g_BGM[now_playing].GetState() == BGM_PLAYING) 
		_beginthread(Fade_Out_Thread,0,(void*)now_playing);

	// 새로운 음악이 없을때만 로드
	if(g_BGM[1-now_playing].GetState() == BGM_STOPED)
	{
		// 새로운 음악의 시작
		g_BGM[1-now_playing].Init(name);
		stream = g_BGM[1-now_playing].GetStream();
		FSOUND_Stream_SetEndCallback(stream, BGMCallback,0);

		g_BGM[1-now_playing].Play(TRUE);
		_beginthread(Fade_In_Thread,0,(void*)(1-now_playing));
	}

	return TRUE;
}

// 현재 플레이 중인 BGM을 STOP 한다.
BOOL Stop_BGM()
{
	FSOUND_STREAM *stream = g_BGM[now_playing].GetStream();
	FSOUND_Stream_SetEndCallback(stream, 0,0);
	g_BGM[now_playing].Stop();
	return TRUE;
}

// 현재 플레이 중인 BGM을 STOP 한다.
BOOL Stop_BGM(int i)
{
	FSOUND_STREAM *stream = g_BGM[i].GetStream();
	FSOUND_Stream_SetEndCallback(stream, 0,0);
	g_BGM[i].Stop();

	//DBG_Put(_T("BGM %d 종료"),i);
	return TRUE;
}

// 현재 플레이 중인것 볼륨 조정
BOOL SetBGMVolume(int vol)
{
	Master_BGM_Vol = vol;
	g_BGM[now_playing].SetVolume(vol);
	return TRUE;
}

BOOL IntializeXiahBGM(int vol)
{
	InitializeCriticalSection(&cr_bgm);
	
	g_BGM = new cBGM[2];

	Master_BGM_Vol = vol;			// MASTER 볼륨 세팅
	Now_Change = FALSE;
	return TRUE;
}

BOOL UninitializeXiahBGM()
{
	delete [] g_BGM;

	DeleteCriticalSection(&cr_bgm);
	return TRUE;
}


//---------------------------------------------------------------------------------------

cBGM::cBGM()
{
	stream = NULL;
	m_state = BGM_STOPED;
	m_time = 0;
	m_para = 0;
	m_channel = 0;
}

cBGM::~cBGM()
{
	Close();
}

BOOL cBGM::Init(TCHAR *filename)
{
	// [4/28/2005] FMOD 사운드 누가 작업했어! 난 알지! 암튼 이것 때문에 지금까지 계속 쌓이는
	// 메모리 누수가 일어 났다. 누가 사운드 파일을 닫지도 않고 계속 열어!
	// 이 메모리 누수가 사고를 지금까지 쳤다. 제발 잡기 힘든 누수 좀 발생시키지 말자.
	Close();

	stream = FSOUND_Stream_Open(filename, FSOUND_STEREO | FSOUND_16BITS | FSOUND_LOOP_OFF | FSOUND_MPEGACCURATE , 0, 0);
	if(stream == NULL)
	{
		DBG_LogFile("cBGM::Init %s\n",filename);
		return FALSE;
	}

	return TRUE;
}

// [4/28/2005] 함수 추가
void cBGM::Close()
{
	if(stream)
	{
		FSOUND_Stream_Stop(stream);		
		FSOUND_Stream_Close(stream);

		stream = NULL;
	}
}

BOOL cBGM::SetVolume(int vol)
{
	FSOUND_SetVolume(m_channel , vol);
	return TRUE;
}

BOOL cBGM::Play(BOOL flag)
{
	m_channel = FSOUND_Stream_PlayEx(FSOUND_FREE,stream,NULL,flag);
	m_state = BGM_PLAYING;
	return TRUE;
}

BOOL cBGM::Stop()
{
	FSOUND_Stream_Stop(stream);
	m_state = BGM_STOPED;
	return TRUE;
}

BOOL cBGM::FadeIn(long time)
{
	return TRUE;
}

BOOL cBGM::FadeOut(long time)
{
	return TRUE;
}

void cBGM::SetState(int state)
{
	EnterCriticalSection(&cr_bgm);
	m_state = state;
	LeaveCriticalSection(&cr_bgm);
}

int	cBGM::GetState() 
{ 
	int state;
	EnterCriticalSection(&cr_bgm);
	state = m_state;
	LeaveCriticalSection(&cr_bgm);
	return state; 
}
