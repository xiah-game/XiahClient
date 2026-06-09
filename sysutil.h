#pragma	   once
//
//	SYSTEM UTILITY CORE
//

template <class T> 
inline const T& minimum(const T& a, const T& b)
{
	return a < b ? a : b;
}

//:	max
template <class T> 
inline const T& maximum(const T& a, const T& b)
{
	return a > b ? a : b;
}

////////////////////////////////////////////////////////////////////////////////////////
//	cTimer
class cTimer
{
public:

	// Data Types & Constants...
	enum TIMER_STATE
	{
		OFF = 0,
		ON,
		HOLD,
	};

    cTimer();
    ~cTimer();

	void start();	// start the timer
	void stop();	// stop the timer

	void suspend();	// pause the timer
	void resume();	// resume the timer from when is was suspended

	// Accessors...
	float elapsedSeconds();
	float elapsedMilliseconds();
	unsigned long elapsedCount();

private:

	static float s_invSecondsFrequency;
	static float s_invMillisecondsFrequency;

	unsigned long			m_startTime;
	unsigned long			m_stopTime;
	unsigned long			m_timeDelta;
	unsigned long			m_elapsedCount;
	TIMER_STATE		m_state;

	// Private Functions...
	static void setupTimerFrequency();
	unsigned long samplePerformanceCounter();

	// non-existant functions
    cTimer(const cTimer& Src);
    cTimer& operator=(const cTimer& Src);

};

inline cTimer::cTimer()
:m_state(OFF)
,m_elapsedCount(0)
{
	memset(&m_startTime, 0, sizeof(m_startTime));
	memset(&m_stopTime, 0, sizeof(m_stopTime));
	memset(&m_timeDelta, 0, sizeof(m_timeDelta));
	setupTimerFrequency();
}

inline cTimer::~cTimer()
{
}

inline void cTimer::start()	// start the timer
{
	m_startTime = samplePerformanceCounter();
	m_elapsedCount = 0;
	m_state = ON;
}

inline void cTimer::stop()	// stop the timer
{
	m_elapsedCount = elapsedCount();
	m_state = OFF;
}


inline void cTimer::suspend()	// resume the timer from when is was stopped
{
	if (m_state == ON)
	{
		m_elapsedCount = elapsedCount();
		m_state = HOLD;
	}
}


inline void cTimer::resume()	// resume the timer from when is was stopped
{
	if (m_state == HOLD)
	{
		// get the current time
		m_startTime = samplePerformanceCounter();

		// roll the start time back by our previous delta
		m_startTime -= m_timeDelta;

		m_elapsedCount = 0;
		m_state = ON;
	}
}

inline unsigned long cTimer::elapsedCount()
{
	if (m_state != ON)
	{
		return m_elapsedCount;
	}
	else
	{
		m_stopTime = samplePerformanceCounter();
		m_timeDelta = m_stopTime - m_startTime;

		return(m_timeDelta);
	}
}

inline float cTimer::elapsedSeconds()
{
	float count = (float)elapsedCount();
	return count*s_invSecondsFrequency;
}

inline float cTimer::elapsedMilliseconds()
{
	float count = (float)elapsedCount();
	return count*s_invMillisecondsFrequency;
}

////////////////////////////////////////////////////////////////////////////////////////

typedef	struct
{
	int	MajorVersion;
	int	MinorVersion;
	int	Build;
} sOS;

#define	WINDOWS_95				0
#define	WINDOWS_95_SR2			1
#define	WINDOWS_98				3
#define	WINDOWS_ME				4
#define	WINDOWS_NT				5
#define	WINDOWS_2K				6
#define	WINDOWS_XP				7
#define	WINDOWS_FUTURE			8
#define	UNKNOWN					10

class cSysutil
{
public :
	void readCPUCounter(__int64 *pCounter);
	void computeProcessorSpeed();
	void querySystemInformation();
	unsigned	long	availableMemory();

	unsigned long GetFlatform();
	unsigned long GetCpuClock();
	unsigned long GetTotalMem();


	unsigned long	m_processorSpeed;	// CPU SPEED!
	unsigned	long	m_physicalMemory	;	// 피지컬
	unsigned	long	m_totalMemory;				// total mem
	unsigned	long	m_platform;

	sOS	m_osVersion;		// OS infomation
};

