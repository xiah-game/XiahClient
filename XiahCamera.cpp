#include "precompile.h"
#include "XiahCamera.h"
#include "XiahGameObject.h"
#include "XiahEnvInfo.h"
#include "AppData.h"
#include "XiahCursor.h"
#include "XiahMap.h"
#include "XiahGameMain.h"
#include "cjoystic.h"

#define CAMERA_MOVE_SPEED	(64.0f / 30.0f)

#define CAMERA_Y_ROTATE_SPEED	( _PI / 50.0f)
#define CAMERA_X_ROTATE_SPEED	( _PI / 4.0f / 30.0f)

#define CAMERA_DISTANCE_SPEED (0.75f)

#define CAMERA_MAX_XANGLE -0.087266465028127030f //( -5 * _PI / 180.0f)
#define CAMERA_MIN_XANGLE -0.61086525519688928f //(-35 * _PI / 180.0f)

#define CAMERA_MIN_DISTANCE 15.0f
#define CAMERA_MAX_DISTANCE 55.0f

#define DUN_CAMERA_MAX_XANGLE -0.48869220415751141f //(-28 * _PI / 180.0f)
#define DUN_CAMERA_MIN_XANGLE -0.69813172022501624f //(-40 * _PI / 180.0f)

extern CUIManager* g_pUIManager;

CXiahCamera g_XiahCamera;

CXiahCamera::CXiahCamera()
{
	g_pCurrentCamera = this;
	m_bFollowMainChar = FALSE;

	m_fXAngle = -_PI / 6;
	m_fYAngle = _PI;
	m_fDistance = 15;

	m_fDestXAngle	 = m_fXAngle;
	m_fDestDistance	 = m_fDistance;

	m_bNeedUpdate = TRUE;

	m_bIntro = false;
	//m_bOnlyMoveCameraForTest = false;
}

CXiahCamera::~CXiahCamera()
{
}

BOOL CXiahCamera::ProcessKeyboard()
{
	return TRUE;

	//////////////////////////////////////

	BOOL bUpdate = FALSE;
	if( GetAsyncKeyState( VK_CONTROL) < 0)
	{
		if( GetAsyncKeyState( VK_UP) < 0)
		{
			m_fXAngle -= CAMERA_X_ROTATE_SPEED * g_fFrameScale;
			bUpdate = TRUE;
		}

		if( GetAsyncKeyState( VK_DOWN) < 0)
		{
			m_fXAngle += CAMERA_X_ROTATE_SPEED * g_fFrameScale;
			bUpdate = TRUE;
		}
	}
	else if( GetAsyncKeyState( VK_MENU) < 0)
	{
		if( GetAsyncKeyState( VK_UP) < 0)
		{
			m_fDistance -= CAMERA_DISTANCE_SPEED * g_fFrameScale;
			bUpdate = TRUE;
		}

		if( GetAsyncKeyState( VK_DOWN) < 0)
		{
			m_fDistance += CAMERA_DISTANCE_SPEED * g_fFrameScale;
			bUpdate = TRUE;
		}

		if( m_fDistance < 10.0f)
			m_fDistance = 10.0f;

		if( m_fDistance > 1000.0f)
			m_fDistance = 1000.0f;
	}
	else//; if( GetAsyncKeyState( VK_SHIFT) < 0)
	{
		if( GetAsyncKeyState( VK_LEFT) < 0)
		{
			m_fYAngle -= CAMERA_Y_ROTATE_SPEED * g_fFrameScale;
			bUpdate = TRUE;
		}

		if( GetAsyncKeyState( VK_RIGHT) < 0)
		{
			m_fYAngle += CAMERA_Y_ROTATE_SPEED * g_fFrameScale;
			bUpdate = TRUE;
		}
	}

	return bUpdate;
}

float CXiahCamera::GetCursorDirection(Vector3 &vTarget)
{
	Vector3 start;
	Vector3 end;

	start = GetCursorWorld( 0.0001f);
	end   = GetCursorWorld( 0.9999f);
	
	Plane3 plane( Vector3(0, 1, 0), -m_vAt.y);

	Vector3 cpos;

	if( !plane.Intersect( start, end, cpos))
	{
		int xDelta = XiahInput::g_ptMouse.x - 1024 / 2;
		int yDelta = XiahInput::g_ptMouse.y - 768 / 2;

		Vector3 xAxis;
		Vector3 yAxis;

		Vector3 vDirection = m_vAt - m_vFrom;
		vDirection.y = 0;

		vDirection.Normalize();

		Vector3 up( 0, 1, 0);

		xAxis = up.Cross( vDirection);
		yAxis = vDirection;

		cpos = m_vAt + xAxis * xDelta * 128 / (1024 / 2 )
						 - yAxis * yDelta * 128 / (768 / 2);

		cpos.y = m_vAt.y;
	}

	vTarget = cpos;

	return atan2( cpos.x - m_vAt.x, -(cpos.z - m_vAt.z));
//	return ;
}

BOOL CXiahCamera::Update()
{
	// 如果客户端窗口失去焦点，则不进行任何视角旋转和缩放的更新，防范后台全局按键响应
	if (GetForegroundWindow() != g_AppData.m_hWnd)
	{
		return TRUE;
	}

	// 패드의 상하 확대
	if(g_GameWork.m_nNavigationMode == 0)
	{
		if(XiahInput::g_Zoomin_Down)
			g_XiahCamera.Zoom(TRUE);
		else if(XiahInput::g_Zoomout_Down)
			g_XiahCamera.Zoom(FALSE);
	}

	if( g_GameWork.m_nNavigationMode == 0)
	{
		if( ( !g_pUIManager->IsMouseOnFrame() && XiahInput::g_ptMouse.x < 10) || 
			( !g_pUIManager->IsMouseOnFrame() && XiahInput::g_Pan_Left_Down) ||
			( GetAsyncKeyState(VK_MENU) < 0 && GetAsyncKeyState( 'A') < 0) ||
			( !g_MainCharInfo.m_bChatModeAction && GetAsyncKeyState( 'A') < 0 && !g_pUIManager->IsOnEditing()))
		{
			if( g_CursorType != eCT_LeftTurn)
				ChangeXiahCursor( eCT_LeftTurn);

			RotateY( FALSE);
		}
		else if( ( !g_pUIManager->IsMouseOnFrame() && XiahInput::g_ptMouse.x > 1014) || 
			( !g_pUIManager->IsMouseOnFrame() && XiahInput::g_Pan_Right_Down) ||
			( GetAsyncKeyState(VK_MENU) < 0 && GetAsyncKeyState( 'D') < 0) ||
			( !g_MainCharInfo.m_bChatModeAction && GetAsyncKeyState('D') < 0 && !g_pUIManager->IsOnEditing() ))
		{
			if( g_CursorType != eCT_RightTurn)
				ChangeXiahCursor( eCT_RightTurn);

			RotateY( TRUE);
		}
		else
		{
			if( g_CursorType == eCT_LeftTurn || g_CursorType == eCT_RightTurn)
				ChangeXiahCursor( eCT_General);
		}
	}

	if( m_fXAngle != m_fDestXAngle)
	{
		if( m_fXAngle < m_fDestXAngle)
		{
			if( m_fXAngle + CAMERA_X_ROTATE_SPEED * g_fFrameScale < m_fDestXAngle)
			{
				m_fXAngle += CAMERA_X_ROTATE_SPEED * g_fFrameScale;
			}
			else
			{
				m_fXAngle = m_fDestXAngle;
			}
		}
		else
		{
			if( m_fXAngle - CAMERA_X_ROTATE_SPEED * g_fFrameScale > m_fDestXAngle)
			{
				m_fXAngle -= CAMERA_X_ROTATE_SPEED * g_fFrameScale;
			}
			else
			{
				m_fXAngle = m_fDestXAngle;
			}
		}
	
		m_bNeedUpdate = TRUE;
	}

	if( m_fDistance != m_fDestDistance)
	{
		float fNextDistance = m_fDistance;
		if( fNextDistance < m_fDestDistance)
		{
			if( fNextDistance + CAMERA_DISTANCE_SPEED * g_fFrameScale < m_fDestDistance)
			{
				fNextDistance += CAMERA_DISTANCE_SPEED * g_fFrameScale;
			}
			else
			{
				fNextDistance = m_fDestDistance;
			}
		}
		else
		{
			if( fNextDistance - CAMERA_DISTANCE_SPEED * g_fFrameScale > m_fDestDistance)
			{
				fNextDistance -= CAMERA_DISTANCE_SPEED * g_fFrameScale;
			}
			else
			{
				fNextDistance = m_fDestDistance;
			}
		}

		// 좀 번거 롭지만 from을 구해본다
		if( g_pMainChar)
		{
			CXiah3DObject *pCharObject = (CXiah3DObject *)g_pMainChar->m_pObject;
			Vector3 vAt;
			Vector3 vFrom;

			Matrix4x4 rotX,rotY;
			rotY.SetRotationEuler( Vector3( 0, m_fYAngle, 0));

			vAt   = pCharObject->m_Position + Vector3( 0, 4, 0);
			vFrom = Vector3( 0, 0, -1) * rotY;

			Vector3 vRight;

			vRight= vFrom.Cross( Vector3( 0, -1, 0));

			rotX.SetRotationAxisAngle( vRight, m_fXAngle);

			vFrom *= rotX;
			vFrom.Normalize();
			vFrom *= fNextDistance;

			vFrom = vAt + vFrom;
			
			float height = Map::g_MapRes.GetHeight( vFrom.x, vFrom.z);

			if( height <= vFrom.y)
				m_fDistance = fNextDistance;
		}

		m_bNeedUpdate = TRUE;
	}

	if( m_bNeedUpdate && g_pMainChar )
	{
		CXiah3DObject *pCharObject = (CXiah3DObject *)g_pMainChar->m_pObject;
	
		Vector3 vAt;
		Vector3 vFrom;

		Matrix4x4 rotX,rotY;
		rotY.SetRotationEuler( Vector3( 0, m_fYAngle, 0));

		vAt   = pCharObject->m_Position + Vector3( 0, 4, 0);
		vFrom = Vector3( 0, 0, -1) * rotY;

		Vector3 vRight;

		vRight= vFrom.Cross( Vector3( 0, -1, 0));

		rotX.SetRotationAxisAngle( vRight, m_fXAngle);

		vFrom *= rotX;
		vFrom.Normalize();
		vFrom *= m_fDistance;

		vFrom = vAt + vFrom;
	
		// 지형과의 카메라 충돌 처리
		float height = Map::g_MapRes.GetHeight( vFrom.x, vFrom.z);
		Vector3 vDir = vFrom - vAt;
		vDir.Normalize();

		while( m_fDistance > g_fix && height + g_fix > vFrom.y)
		{
			m_fDistance -= 0.1f;
			//m_fDestDistance = m_fDistance;

			vFrom = vAt + vDir * m_fDistance;			
			
			height = Map::g_MapRes.GetHeight( vFrom.x, vFrom.z);
		}

		SetView( vFrom, vAt, Vector3( 0, 1, 0));
#ifdef MINI
		SetProjection( _PI / 4, (float)G_HEIGHT / (float)G_WIDTH, 1.0f, (float)g_XiahEnvInfo.m_CameraBoundSize * 1.5f);
#else
		SetProjection( _PI / 4, 768.0f / 1024.0f, 1.0f, (float)g_XiahEnvInfo.m_CameraBoundSize * 1.5f);
#endif
	
		m_bNeedUpdate = FALSE;
	}

    //
	if( m_bIntro )
	{
		Vector3 vAt;
		Vector3 vFrom;

		Matrix4x4 rotX,rotY;
		rotY.SetRotationEuler( Vector3( 0, m_fYAngle, 0));

		vAt = m_vAt;
		//	vFrom = g_XiahCamera.m_vFrom;
//		float height = Map::g_MapRes.GetHeight( vAt.x,  vAt.z );

		vFrom = Vector3( 0, 0, -1) * rotY;

		Vector3 vRight;
		vRight= vFrom.Cross( Vector3( 0, -1, 0));

		rotX.SetRotationAxisAngle( vRight, m_fXAngle);

		vFrom *= rotX;
		vFrom.Normalize();
		vFrom *= m_fDistance;

		vFrom = vAt + vFrom;

/*
		// 지형과의 카메라 충돌 처리
		height = Map::g_MapRes.GetHeight( vFrom.x, vFrom.z);
		Vector3 vDir = vFrom - vAt;
		vDir.Normalize();

		while( m_fDistance > 1 && height > vFrom.y)
		{
			m_fDistance -= 0.1f;
			m_fDestDistance = m_fDistance;

			vFrom = vAt + vDir * m_fDistance;			

			height = Map::g_MapRes.GetHeight( vFrom.x, vFrom.z);
		}
*/

		SetView( vFrom, vAt, Vector3( 0, 1, 0));
#ifdef MINI
		SetProjection( _PI / 4, (float)G_HEIGHT / (float)G_WIDTH, 1.0f, (float)g_XiahEnvInfo.m_CameraBoundSize * 1.5f);
#else
		SetProjection( _PI / 4, 768.0f / 1024.0f, 1.0f, (float)g_XiahEnvInfo.m_CameraBoundSize * 1.5f);
#endif
		
	}

	/*
	if( m_bOnlyMoveCameraForTest )
	{
		Vector3 vAt;
		Vector3 vFrom;

		Matrix4x4 rotX,rotY;
		rotY.SetRotationEuler( Vector3( 0, m_fYAngle, 0));

		vAt = m_vNewAt;
		//	vFrom = g_XiahCamera.m_vFrom;
//		float height = Map::g_MapRes.GetHeight( vAt.x,  vAt.z );

		vFrom = Vector3( 0, 0, -1) * rotY;

		Vector3 vRight;
		vRight= vFrom.Cross( Vector3( 0, -1, 0));

		rotX.SetRotationAxisAngle( vRight, m_fXAngle);

		vFrom *= rotX;
		vFrom.Normalize();
		vFrom *= m_fDistance;

		vFrom = vAt + vFrom;

		SetView( vFrom, vAt, Vector3( 0, 1, 0));
#ifdef MINI
		SetProjection( _PI / 4, (float)G_HEIGHT / (float)G_WIDTH, 1.0f, (float)g_XiahEnvInfo.m_CameraBoundSize * 1.5f);
#else
		SetProjection( _PI / 4,768.0f / 1024.0f, 1.0f, (float)g_XiahEnvInfo.m_CameraBoundSize * 1.5f);
#endif
	}
	*/

	XiahGameEngine::g_pDirect3DDevice->SetTransform( D3DTS_VIEW, (D3DMATRIX *)&m_matView);
	XiahGameEngine::g_pDirect3DDevice->SetTransform( D3DTS_PROJECTION, (D3DMATRIX *)&m_matProjection);

	return TRUE;
}

BOOL CXiahCamera::RotateY(BOOL bRight, float fSpeed)
{
	if( bRight == FALSE)
	{
		m_fYAngle -= CAMERA_Y_ROTATE_SPEED * g_fFrameScale * fSpeed;
	}
	else if( bRight)
	{
		m_fYAngle += CAMERA_Y_ROTATE_SPEED * g_fFrameScale * fSpeed;
	}	

	m_bNeedUpdate = TRUE;

	return TRUE;
}

/**
 *
 * \param bIn 
 * \return 
 */
BOOL CXiahCamera::Zoom(BOOL bIn)
{	
	// 던전용 카메라 설정
	if(11 == XiahMap::g_XiahMap.m_MapInfo.m_dwMapID)
	{
		//m_fDestXAngle = -0.710f;

        if(bIn)
		{
			m_fDestDistance -= 5;
			m_fDestXAngle += (DUN_CAMERA_MAX_XANGLE - DUN_CAMERA_MIN_XANGLE) / 9.0f;
		}
		else
		{
			m_fDestDistance += 5;
			m_fDestXAngle -= (DUN_CAMERA_MAX_XANGLE - DUN_CAMERA_MIN_XANGLE) / 9.0f;
		}

		if( m_fDestDistance < CAMERA_MIN_DISTANCE)
			m_fDestDistance = CAMERA_MIN_DISTANCE;

		if( m_fDestDistance > CAMERA_MAX_DISTANCE+10.0f)
			m_fDestDistance = CAMERA_MAX_DISTANCE+10.0f;

		if( m_fDestXAngle < DUN_CAMERA_MIN_XANGLE)
			m_fDestXAngle = DUN_CAMERA_MIN_XANGLE;

		if( m_fDestXAngle > DUN_CAMERA_MAX_XANGLE)
			m_fDestXAngle = DUN_CAMERA_MAX_XANGLE;
	}
	else
	{
		if( bIn)
		{
			m_fDestDistance -= 5;
			/*
			if( m_bOnlyMoveCameraForTest )
				m_fDestXAngle += (( 30 * _PI / 180.0f) - (-60 * _PI / 180.0f)) / 18.0f;
			else
			*/
			m_fDestXAngle += (CAMERA_MAX_XANGLE - CAMERA_MIN_XANGLE) / 6.0f;
		}
		else 
		{
			m_fDestDistance += 5;
			/*
			if( m_bOnlyMoveCameraForTest )
				m_fDestXAngle -= (( 30 * _PI / 180.0f) - (-60 * _PI / 180.0f)) / 18.0f;
			else
			*/
			m_fDestXAngle -= (CAMERA_MAX_XANGLE - CAMERA_MIN_XANGLE) / 6.0f;
		}

		if( m_fDestDistance < CAMERA_MIN_DISTANCE)
			m_fDestDistance = CAMERA_MIN_DISTANCE;

		if( m_fDestDistance > CAMERA_MAX_DISTANCE)
			m_fDestDistance = CAMERA_MAX_DISTANCE;

		/*
		if( m_bOnlyMoveCameraForTest )
		{
			if( m_fDestXAngle < (-60 * _PI / 180.0f) )
				m_fDestXAngle = (-60 * _PI / 180.0f);

			if( m_fDestXAngle > ( 30 * _PI / 180.0f) )
				m_fDestXAngle = ( 30 * _PI / 180.0f);
		}
		else
		*/
		if( m_fDestXAngle < CAMERA_MIN_XANGLE)
			m_fDestXAngle = CAMERA_MIN_XANGLE;

		if( m_fDestXAngle > CAMERA_MAX_XANGLE)
			m_fDestXAngle = CAMERA_MAX_XANGLE;
	}	

	return TRUE;
}
