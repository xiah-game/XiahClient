#pragma once

class CXiahCamera : public XiahGameEngine::CCamera
{
public: 
	CXiahCamera();
	virtual ~CXiahCamera();

	BOOL Update();

	BOOL ProcessKeyboard();

	float GetCursorDirection(Vector3 &vTarget);

	BOOL Zoom(BOOL bIn);
	BOOL RotateY(BOOL bRight, float fSpeed = 1.0f);

public:
	float	m_fYAngle;
	float	m_fXAngle;
	float	m_fDistance;

	float	m_fDestXAngle;
	float	m_fDestDistance;

	BOOL m_bNeedUpdate;
	BOOL m_bFollowMainChar;

	// intro interface
	bool		m_bIntro;

	// Only Move Camera for Test
	//bool		m_bOnlyMoveCameraForTest;
	Vector3		m_vNewAt;
};

extern CXiahCamera g_XiahCamera;