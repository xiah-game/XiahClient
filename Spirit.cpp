/*===================================================================================================
										Spirit.cpp
-----------------------------------------------------------------------------------------------------
	date :	2005/08/31  14:28
  Author :	
	
 Purpose :	
	
===================================================================================================*/
#include "precompile.h"
#include ".\spirit.h"
#include "CharacterInfo.h"

Spirit g_Spirit;

Spirit::Spirit(void) : m_bUpdate(false), m_nGaugeCount(0)
{

}

Spirit::~Spirit(void)
{

}

/**
 *
 */
void Spirit::Update()
{
	if(m_bUpdate)
	{
		DWORD dwTime = timeGetTime();

		if(dwTime > m_dwEndTime)
		{
			m_bUpdate = false;

			for(int i=0; i < m_nGaugeCount; ++i)
			{
				g_pUIManager->Hide(MAIN_FRAME, main_frame_1_gauge_01 + i);
			}
			return;
		}
		else
		{
			dwTime = m_dwEndTime - dwTime;

			int nCount = (dwTime / m_dwIntervalTime) + 1;
			g_MainCharInfo.m_bStaminaCnt = static_cast<BYTE>(nCount);

			for(int i=0; i < m_nGaugeCount; ++i)
			{
				if(nCount > i)
				{
					g_pUIManager->Show(MAIN_FRAME, main_frame_1_gauge_01 + i);
				}
				else
				{					
					g_pUIManager->Hide(MAIN_FRAME, main_frame_1_gauge_01 + i);
				}
			}
		}
	}
}

/**
 *
 * \param dwIntervalTime 
 * \param nGaugeCount 
 */
void Spirit::Start(DWORD dwIntervalTime, int nGaugeCount)
{
	m_dwIntervalTime	= dwIntervalTime;
	m_nGaugeCount		= nGaugeCount;

	m_dwStartTime = timeGetTime();

	m_dwEndTime	  = m_dwStartTime + (m_dwIntervalTime * m_nGaugeCount);

	m_bUpdate = true;
}

void Spirit::Clear()
{
	m_bUpdate = false;
}