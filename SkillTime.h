#pragma once

#include <map>

class SkillTime
{
public:

	struct sSkill
	{		
		DWORD dwStartTime;
		DWORD dwTotalTime;
		float fRatio;
		bool bEnd;

		sSkill() : dwStartTime(0), dwTotalTime(0), fRatio(0.0f), bEnd(false)
		{
		}
	};

	typedef std::map<DWORD, sSkill> SkillList;

public:

	SkillTime();
	~SkillTime();

	void UpDate(DWORD dwTime);

	void AddSkill(DWORD dwID, DWORD dwStartTime, DWORD dwTotalTime);
	void DelSkill(DWORD dwID);

	void Clear();

	inline SkillTime::SkillList& GetSkillList();

protected:

	SkillList m_mSkillList;

};

inline SkillTime::SkillList& SkillTime::GetSkillList()
{
	return m_mSkillList;
}


extern SkillTime g_SkillTime;