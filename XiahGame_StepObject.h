#pragma once

class CXiahGame_StepObject
{
public:
	CXiahGame_StepObject();
	virtual ~CXiahGame_StepObject();

	virtual BOOL Update() = 0;
	virtual BOOL Render() = 0;
};

typedef std::vector<CXiahGame_StepObject*> GAMESTEP_LIST;

extern BOOL InitGameStepObject();
extern BOOL ReleaseGameStepObject();
extern GAMESTEP_LIST g_GameStep;