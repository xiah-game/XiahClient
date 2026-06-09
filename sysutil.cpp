#include "precompile.h"
#include "sysutil.h"

float cTimer::s_invSecondsFrequency = 0.0f;
float cTimer::s_invMillisecondsFrequency = 0.0f;

void cTimer::setupTimerFrequency()
{
	if (s_invSecondsFrequency == 0.0f)
	{
		LARGE_INTEGER frequency;
		BOOL success= QueryPerformanceFrequency(&frequency);

		// make sure frequency is non-zero
		frequency.LowPart = maximum((DWORD)1, (DWORD)frequency.LowPart);

		s_invSecondsFrequency = 1.0f/(float)frequency.LowPart;
		s_invMillisecondsFrequency = s_invSecondsFrequency*1000.0f;
	}
}

unsigned long cTimer::samplePerformanceCounter()
{
	LARGE_INTEGER sample;
	QueryPerformanceCounter(&sample);
	return sample.LowPart;
}

////////////////////////////////////////////////////////////////////////////////////////

void cSysutil::readCPUCounter(__int64 *pCounter)
{
	_asm
	{
		RDTSC
			mov edi, pCounter
			mov DWORD PTR [edi], eax
			mov DWORD PTR [edi+4], edx
	};
}


void cSysutil::computeProcessorSpeed()
{
	__int64 startTime, endTime;
	cTimer localTimer;

	// start the timer
	localTimer.start();
	
	// sample the cpu counter
	readCPUCounter(&startTime);
	
	// waste some time
	Sleep(100);
	
	// resample the cpu counter
	readCPUCounter(&endTime);
	
	// stop the clock
	localTimer.stop();

	// compute the CPU speed 
	// as ticks per millisecond
	__int64 sampleDelta = 
		endTime - startTime;
	unsigned long elapsedMilliseconds = 
		(unsigned long)localTimer.elapsedMilliseconds();
	
	// make sure time is non-zero
	elapsedMilliseconds = maximum((unsigned long)1, elapsedMilliseconds);
	
	m_processorSpeed = 
		(unsigned long)sampleDelta/elapsedMilliseconds;
}


void cSysutil::querySystemInformation()
{
	SYSTEM_INFO		SysInfo;
	MEMORYSTATUS	MemStatus;
	OSVERSIONINFO 	OSVersion;

	// read the memory status
	MemStatus.dwLength = sizeof (MemStatus);
	GlobalMemoryStatus(&MemStatus);

	// read the OS Version data
	OSVersion.dwOSVersionInfoSize =sizeof(OSVersion);
	GetVersionEx(&OSVersion);

	// read the System Info
	GetSystemInfo(&SysInfo);

	// fill in our data members
	m_physicalMemory		=	MemStatus.dwTotalPhys;
	m_totalMemory				=	MemStatus.dwAvailPhys+ MemStatus.dwAvailPageFile;
	m_osVersion.MajorVersion	=OSVersion.dwMajorVersion;
	m_osVersion.MinorVersion	=OSVersion.dwMinorVersion;


	//
	// Figure out which OS this is
	//
	if (OSVersion.dwPlatformId==VER_PLATFORM_WIN32_WINDOWS)
	{
		m_osVersion.Build=LOWORD(OSVersion.dwBuildNumber);

		m_platform=WINDOWS_95;

		if (m_osVersion.MinorVersion==0 && m_osVersion.Build>950)
		{
			m_platform=WINDOWS_95_SR2;
		}
		else if (m_osVersion.MinorVersion==10)
		{
			m_platform=WINDOWS_98;
		}
		else if (m_osVersion.MinorVersion>10)
		{
			m_platform=WINDOWS_ME;
		}
	}
	else if (OSVersion.dwPlatformId==VER_PLATFORM_WIN32_NT)
	{
		m_osVersion.Build =OSVersion.dwBuildNumber;

		if (m_osVersion.MajorVersion<4)
		{
			m_platform=WINDOWS_NT;
		}
		else if (m_osVersion.MajorVersion == 4)
		{
			m_platform=WINDOWS_2K;
		}
		else if (m_osVersion.MajorVersion == 5)
		{
			m_platform=WINDOWS_XP;
		}
		else
		{
			m_platform=WINDOWS_FUTURE;
		}
	}
	else
	{
		m_platform			=UNKNOWN;
		m_osVersion.Build	=OSVersion.dwBuildNumber;
	}

	computeProcessorSpeed();
}

unsigned long	cSysutil::availableMemory()
{
	MEMORYSTATUS	MemStatus;

	// read the memory status
	MemStatus.dwLength = sizeof (MemStatus);
	GlobalMemoryStatus(&MemStatus);

	// return the amount of available, physical memory
	return(MemStatus.dwAvailPhys);
}

// OS 정보를 얻는다
unsigned long cSysutil::GetFlatform()
{
	return	m_platform;
}

// CPU의 CLOCK을 얻는다
unsigned long cSysutil::GetCpuClock()
{
	return (unsigned long)(m_processorSpeed / 1000);
}

// TOTAL MEMORY를 얻는다
unsigned long cSysutil::GetTotalMem()
{
	return m_totalMemory;
}

