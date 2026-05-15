
#define MAIN_CHAROBJECT	((CXiahCharObject*)(g_pMainChar->m_pObject))

#define STOP_MAINCHAR	\
							MAIN_CHAROBJECT->SetAnimation( XiahAniType::eLAT_Stand, 0);\
							SendCS_NV_ENDMOVE_REQ( g_pMainChar->m_dwServerID, MAIN_CHAROBJECT->m_Position.x, -MAIN_CHAROBJECT->m_Position.z, MAIN_CHAROBJECT->m_Position.y, CHARSTATE_NORMAL);

#define STARTMOVE_MAINCHAR	\
								bMove = TRUE;\
								MoveTime = g_dwCurTime;\
								MAIN_CHAROBJECT->SetAnimation( XiahAniType::eLAT_Run, 1);\
								MAIN_CHAROBJECT->SetAngleTarget( pSelCharObject->m_Position);\
								MAIN_CHAROBJECT->GetAngle( angle);\
								MAIN_CHAROBJECT->SetTargetMove( pSelCharObject->m_Position.x, -pSelCharObject->m_Position.z, eLBP_CharNavigation, 0);\
								fMoveSpeed = (float)(g_MainCharInfo.m_bWalkSpeed + g_MainCharInfo.m_bPlusSpeed) / 9.0f;\
								MAIN_CHAROBJECT->m_CharRender.SetAnimationSpeed( fMoveSpeed);\
								SendCS_NV_STARTMOVE_REQ( g_pMainChar->m_dwServerID, MAIN_CHAROBJECT->m_Position.x, -MAIN_CHAROBJECT->m_Position.z, MAIN_CHAROBJECT->m_Position.y, \
									pSelCharObject->m_Position.x, -pSelCharObject->m_Position.z, pSelCharObject->m_Position.y, (WORD)angle, CHARSTATE_NORMAL, 0);\
								g_PickCursor.Create( XiahPak::GetTexture( 50000396), pSelCharObject->m_Position.x, pSelCharObject->m_Position.z, 6, COLOR_PICKCURSOR);\
								g_PickCursor.SetRotate( _PI / 90.0f);

#define KEEPMOVE_MAINCHAR \
								SendCS_NV_ENDMOVE_REQ( g_pMainChar->m_dwServerID, MAIN_CHAROBJECT->m_Position.x, -MAIN_CHAROBJECT->m_Position.z, MAIN_CHAROBJECT->m_Position.y, CHARSTATE_NORMAL);\
								bMove = TRUE;\
								MoveTime = g_dwCurTime;\
								MAIN_CHAROBJECT->SetAngleTarget( pSelCharObject->m_Position);\
								MAIN_CHAROBJECT->GetAngle( angle);\
								MAIN_CHAROBJECT->SetTargetMove( pSelCharObject->m_Position.x, -pSelCharObject->m_Position.z, eLBP_CharNavigation, 0);\
								fMoveSpeed = (float)(g_MainCharInfo.m_bWalkSpeed + g_MainCharInfo.m_bPlusSpeed) / 9.0f;\
								MAIN_CHAROBJECT->m_CharRender.SetAnimationSpeed( fMoveSpeed);\
								SendCS_NV_STARTMOVE_REQ( g_pMainChar->m_dwServerID, MAIN_CHAROBJECT->m_Position.x, -MAIN_CHAROBJECT->m_Position.z, MAIN_CHAROBJECT->m_Position.y, \
								pSelCharObject->m_Position.x, -pSelCharObject->m_Position.z, pSelCharObject->m_Position.y, (WORD)angle, CHARSTATE_NORMAL, 0);\
								g_PickCursor.Create( XiahPak::GetTexture( 50000396), pSelCharObject->m_Position.x, pSelCharObject->m_Position.z, 6, COLOR_PICKCURSOR);\
								g_PickCursor.SetRotate( _PI / 90.0f);

				
//---------------------------------------------------------------------------------------
BOOL ProcessAutoNavigation(int mode)
{
	static DWORD dwAITime = 0;
	WORD angle;
	float fMoveSpeed;
	
	// 선택된 넘이 없어지면 자동 종료
	if( dwSelObjectID == 0)
	{
		bAutoNavigation = FALSE;
		bAutoAttack = FALSE;
		STOP_MAINCHAR;
//		g_MainCharInfo.ShowHelpMessage(IDS_END_AUTOMODE);
		return TRUE;
	}
	
	XiahObject::CXiahObject* pSelObject	= NULL;
	CXiahCharObject* pSelCharObject = NULL;
	
	pSelObject = XiahObject::g_XiahObjectManager.FindXiahObject( MAKEOBJECTID( 0, dwSelObjectID, dwSelObjectType));

	if( pSelObject == NULL)
	{
		bAutoNavigation = FALSE;
		bAutoAttack = FALSE;
		STOP_MAINCHAR;
		dwSelObjectID = 0;
		dwSelObjectType = 0;
		//	g_MainCharInfo.ShowHelpMessage(IDS_END_AUTOMODE);
		return TRUE;
	}

	if( pSelObject->m_pObject->IsA( XiahObject::eXOT_CharObject))
	{
		pSelCharObject = (CXiahCharObject*)pSelObject->m_pObject;
	}

	if( dwSelObjectType == OBJTYPE_NPC && pSelCharObject->m_nCurMotionType == XiahAniType::eLAT_Die)
	{
		bAutoNavigation = FALSE;
		bAutoAttack = FALSE;
		STOP_MAINCHAR;
		dwSelObjectID = 0;
		dwSelObjectType = 0;
		return TRUE;
	}

	// 거리 체크
	float fRange = pSelCharObject->GetInteractionDistance( MAIN_CHAROBJECT->m_Position);

	// 움직일때는 약간 더 안쪽으로 들어갈 수 있게 해준다, 그래야 떨리는걸 막을 수 있겠다.
	if( bMove)
		fRange += 3;

	// 인터렉션 거리가 범위안에 들어오면 동작을 시작한다.
	if( fRange < fInteractionRange)
	{
		// 움직이는중이면 멈추어주고
		if( bMove)
		{
			g_MainChar_PreAttackInfo.nRemainAttackCount	 = 0;
			g_MainChar_PreAttackInfo.dwLastPreAttackTime = 0;
			STOP_MAINCHAR;
		}

		// 인터렉션 처리를 한다
		//HT_CHEAT : 펫 자동 중에는 때리지 말아야지
		if(g_bCheat)
		{
			if(g_PetList.size() == 0 || g_MainCharInfo.m_bCheat)
				InteractObject( dwSelObjectID, dwSelObjectType, 0);
			//else if(  )
			//	InteractObject( dwSelObjectID, dwSelObjectType, 0);
		}
		else
			InteractObject( dwSelObjectID, dwSelObjectType, 0);

		if( !bAutoAttack && dwSelObjectType != OBJTYPE_PC)
		{
			bAutoNavigation = FALSE;
		}
	}
	else if( g_dwCurTime - dwAITime > 1000 || mode == 0)
	{
		dwAITime = g_dwCurTime;

		if( mode == 0)
		{
			if( bMove)
			{
				//STOP_MAINCHAR;
				//STARTMOVE_MAINCHAR;
				KEEPMOVE_MAINCHAR ;	// 움직임을 유지해야한다. 아니면 끈기는 동작이 나온다.
				return TRUE;
			}
		}

		if( bMove == FALSE)
		{
			STARTMOVE_MAINCHAR;
			g_MainChar_PreAttackInfo.nRemainAttackCount	 = 0;
			g_MainChar_PreAttackInfo.dwLastPreAttackTime = 0;
		}
		else
		{
			WORD desAngle = MAIN_CHAROBJECT->GetTargetAngle( pSelCharObject->m_Position);
			WORD curAngle;
			MAIN_CHAROBJECT->GetAngle( curAngle);

			if( ABS( curAngle - desAngle) > 30)	// 30도 이상 차이가 나면
			{
				//STOP_MAINCHAR;
				//STARTMOVE_MAINCHAR;
				KEEPMOVE_MAINCHAR ;	// 움직임을 유지해야한다. 아니면 끈기는 동작이 나온다.
				g_MainChar_PreAttackInfo.nRemainAttackCount	 = 0;
				g_MainChar_PreAttackInfo.dwLastPreAttackTime = 0;
			}
		}
	}

	return TRUE;
}
