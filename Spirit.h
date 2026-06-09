/*===================================================================================================
										Spirit.h
-----------------------------------------------------------------------------------------------------
	date :	2005/08/31  14:28
  Author :	
	
 Purpose :	
	
===================================================================================================*/
#pragma once

class Spirit
{
public:
	Spirit(void);
	virtual ~Spirit(void);

	virtual void Update();
	virtual void Start(DWORD dwIntervalTime, int nGaugeCount);

	virtual void Clear();

protected:

	bool m_bUpdate;

	DWORD m_dwStartTime;
	DWORD m_dwEndTime;
	DWORD m_dwIntervalTime;
	int m_nGaugeCount;
};

extern Spirit g_Spirit;