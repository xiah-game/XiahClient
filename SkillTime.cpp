
#include "precompile.h"
#include "skilltime.h"

SkillTime g_SkillTime;

SkillTime::SkillTime()
{

}

SkillTime::~SkillTime()
{

}

void SkillTime::UpDate(DWORD dwTime)
{
	for(SkillList::iterator iter = m_mSkillList.begin(); iter != m_mSkillList.end(); ++iter)
	{
		sSkill& skill = iter->second;

		if(skill.bEnd)
		{
			DelSkill(iter->first);
			break;	    
		}

		DWORD dwRunTime = dwTime - skill.dwStartTime;

		skill.fRatio = static_cast<float>(dwRunTime) / static_cast<float>(skill.dwTotalTime);

        if(skill.dwTotalTime < dwRunTime)
        {
			//skill.fRatio = 0.0f;
			skill.bEnd = true;            
        }
	}
}

void SkillTime::AddSkill(DWORD dwID, DWORD dwStartTime, DWORD dwTotalTime)
{
	DelSkill(dwID);

	sSkill skill;
	skill.dwStartTime = dwStartTime;
	skill.dwTotalTime = dwTotalTime;

	m_mSkillList.insert(SkillList::value_type(dwID, skill));
}

void SkillTime::DelSkill(DWORD dwID)
{
	SkillList::iterator iter = m_mSkillList.find(dwID);

	if(iter != m_mSkillList.end())
	{
		m_mSkillList.erase(iter);
	}
}

void SkillTime::Clear()
{
	m_mSkillList.clear();
}
