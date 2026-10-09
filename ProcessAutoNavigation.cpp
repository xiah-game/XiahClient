#define MAIN_CHAROBJECT	((CXiahCharObject*)(g_pMainChar->m_pObject))

#define STOP_MAINCHAR	\
							MAIN_CHAROBJECT->SetAnimation( XiahAniType::eLAT_Stand, 0);\
							SendCS_NV_ENDMOVE_REQ( g_pMainChar->m_dwServerID, MAIN_CHAROBJECT->m_Position.x, -MAIN_CHAROBJECT->m_Position.z, MAIN_CHAROBJECT->m_Position.y, CHARSTATE_NORMAL);\
							bMove = FALSE;\
							MAIN_CHAROBJECT->m_bTargetMove = FALSE;

// 执行向目标（或其拐角航路点）移动
static inline bool DoMoveMainChar(CXiahCharObject* pSelCharObject, bool bKeepMove)
{
	if (!pSelCharObject) return false;

	int curX = (int)MAIN_CHAROBJECT->m_Position.x;
	int curY = (int)(-MAIN_CHAROBJECT->m_Position.z);
	int targetX = (int)pSelCharObject->m_Position.x;
	int targetY = (int)(-pSelCharObject->m_Position.z);

	int wpX = targetX, wpY = targetY;
	if (!FindNextWaypoint(curX, curY, targetX, targetY, wpX, wpY)) {
		// A*未找到拐角点时，平滑降级为直达目标坐标，由后续1.8秒位移看门狗兜底防卡
		wpX = targetX;
		wpY = targetY;
	}

	Vector3 vMoveTarget;
	if (wpX == targetX && wpY == targetY) {
		vMoveTarget = pSelCharObject->m_Position;
	} else {
		// 航路点：中间拐角点
		vMoveTarget = Vector3((float)wpX, pSelCharObject->m_Position.y, -(float)wpY);
	}

	if (bKeepMove) {
		SendCS_NV_ENDMOVE_REQ( g_pMainChar->m_dwServerID, MAIN_CHAROBJECT->m_Position.x, -MAIN_CHAROBJECT->m_Position.z, MAIN_CHAROBJECT->m_Position.y, CHARSTATE_NORMAL);
	}

	WORD angle = 0;
	bMove = TRUE;
	MoveTime = g_dwCurTime;

	// 关键修复：设置目标朝向与更新物理导航，获取真实服务器朝向角，彻底杜绝栈野值和反向乱跑！
	MAIN_CHAROBJECT->SetAngleTarget( vMoveTarget );
	MAIN_CHAROBJECT->GetAngle( angle );
	MAIN_CHAROBJECT->Update( 1 );
	MAIN_CHAROBJECT->SetTargetMove( (WORD)wpX, (WORD)wpY, eLBP_CharNavigation, 0 );

	if (g_MainCharInfo.m_bFastMove)
	{
		int fastIdx = (g_MainCharInfo.m_nFastIndex > 0) ? g_MainCharInfo.m_nFastIndex : 4;
		if (MAIN_CHAROBJECT->m_nCurMotionType != XiahAniType::eLAT_Mugong || MAIN_CHAROBJECT->m_nCurAniIndex != fastIdx)
		{
			MAIN_CHAROBJECT->SetAnimation( XiahAniType::eLAT_Mugong, fastIdx, 0.7f);
		}
	}
	else
	{
		if (MAIN_CHAROBJECT->m_nCurMotionType != XiahAniType::eLAT_Run)
		{
			MAIN_CHAROBJECT->SetAnimation( XiahAniType::eLAT_Run, 1);
		}
		float fMoveSpeed = (float)(g_MainCharInfo.m_bWalkSpeed + g_MainCharInfo.m_bPlusSpeed) / 9.0f;
		MAIN_CHAROBJECT->m_CharRender.SetAnimationSpeed( fMoveSpeed );
	}
	SendCS_NV_STARTMOVE_REQ( g_pMainChar->m_dwServerID, MAIN_CHAROBJECT->m_Position.x, -MAIN_CHAROBJECT->m_Position.z, MAIN_CHAROBJECT->m_Position.y, \
		(WORD)wpX, (WORD)wpY, (BYTE)vMoveTarget.y, (WORD)angle, CHARSTATE_NORMAL, 0 );

	Vector3 vMainCharSize = MAIN_CHAROBJECT->m_LocalBound.Size();
	g_PickCursor.Create( XiahPak::GetTexture( 50000396), pSelCharObject->m_Position.x, pSelCharObject->m_Position.z, 6, COLOR_PICKCURSOR, TRUE, 0, TRUE, pSelCharObject->m_Position.y, vMainCharSize.y, MAIN_CHAROBJECT->m_Position.y );
	g_PickCursor.SetRotate( 0.03490658f );

	return true;
}

#define STARTMOVE_MAINCHAR	DoMoveMainChar(pSelCharObject, false)
#define KEEPMOVE_MAINCHAR	DoMoveMainChar(pSelCharObject, true)

				
//---------------------------------------------------------------------------------------
BOOL ProcessAutoNavigation(int mode)
{
	static DWORD dwAITime = 0;
	float fMoveSpeed;
	
	// 선택된 넘이 없어지면 자동 종료
	if( dwSelObjectID == 0)
	{
		bAutoNavigation = FALSE;
		bAutoAttack = FALSE;
		STOP_MAINCHAR;
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

	// 脱困避障状态机与卡阻看门狗
	static DWORD   s_dwDetourUntil = 0;         // 脱困移动锁定截止时间（在此之前严格朝脱困航标移动，禁止纠偏重定向）
	static int     s_nDetourTargetX = 0;        // 脱困航标 X
	static int     s_nDetourTargetY = 0;        // 脱困航标 Y
	static int     s_nStuckCount = 0;           // 连续卡阻计数
	static Vector3 s_vLastNavPos = Vector3(0, 0, 0);
	static DWORD   s_dwLastNavCheckTime = 0;
	static DWORD   s_dwCurrentTrackingID = 0;
	static DWORD   s_dwTrackingStartTime = 0;
	static DWORD   s_dwAttackStuckStartTime = 0; // 停步攻击卡阻看门狗

	DWORD dwNow = GetTickCount();

	// 目标发生变更：重置所有看门狗与脱困状态
	if (dwSelObjectID != s_dwCurrentTrackingID) {
		s_dwCurrentTrackingID = dwSelObjectID;
		s_dwTrackingStartTime = dwNow;
		s_vLastNavPos = MAIN_CHAROBJECT->m_Position;
		s_dwLastNavCheckTime = dwNow;
		s_dwDetourUntil = 0;
		s_nStuckCount = 0;
		s_dwAttackStuckStartTime = 0;
	}

	// 1. 最高优先级：如果当前正处于脱困避障锁定中（持续约 1.0~1.2 秒）
	if (s_dwDetourUntil > dwNow) {
		float curX = MAIN_CHAROBJECT->m_Position.x;
		float curY = -MAIN_CHAROBJECT->m_Position.z;
		float dx = curX - (float)s_nDetourTargetX;
		float dy = curY - (float)s_nDetourTargetY;
		if ((dx * dx + dy * dy) < 1.44f) { // 距离脱困航标小于 1.2 码：脱困提前达成！
			s_dwDetourUntil = 0;
		} else {
			// 严格保持朝脱困航标奔跑，阻断主循环纠偏和攻击，确保角色完整绕出大树！
			return TRUE;
		}
	}

	// 2. 动态卡阻检测（移动中撞树原地踏步，或隔树停步打不到怪）
	bool bNeedDetour = false;
	if (bMove && dwSelObjectID != 0) {
		s_dwAttackStuckStartTime = 0;
		if (dwNow - s_dwLastNavCheckTime > 600) {
			float fMoved = (MAIN_CHAROBJECT->m_Position - s_vLastNavPos).GetLength();
			s_vLastNavPos = MAIN_CHAROBJECT->m_Position;
			s_dwLastNavCheckTime = dwNow;

			// 连续 600ms 位移小于 0.35 码：判定为被大树/树桩物理阻挡卡停！
			if (fMoved < 0.35f) {
				bNeedDetour = true;
			} else {
				s_nStuckCount = 0; // 正常位移中，重置卡阻计数
			}
		}

		// 追踪同一只怪超过 8 秒未命中：拉黑放弃（仅限怪物，绝不拉黑功能NPC）
		if (dwSelObjectType == OBJTYPE_NPC && dwNow - s_dwTrackingStartTime > 8000) {
			AddStuckMonster(dwSelObjectID, 25000);
			dwSelObjectID = 0;
			dwSelObjectType = 0;
			bAutoNavigation = FALSE;
			bAutoAttack = FALSE;
			STOP_MAINCHAR;
			s_nStuckCount = 0;
			return TRUE;
		}
	} else if (!bMove && dwSelObjectID != 0) {
		// 角色停步状态（例如停在大树后试图攻击）：
		if (s_dwAttackStuckStartTime == 0) {
			s_dwAttackStuckStartTime = dwNow;
		} else if (dwNow - s_dwAttackStuckStartTime > 2200) {
			// 停在原地超过 2.2 秒怪依然没死（被大树挡住打不到怪）：判定为卡阻！
			bNeedDetour = true;
			s_dwAttackStuckStartTime = dwNow;
		}
	}

	// 3. 执行脱困机制
	if (bNeedDetour) {
		s_nStuckCount++;
		if (s_nStuckCount <= 2) {
			int curX = (int)MAIN_CHAROBJECT->m_Position.x;
			int curY = (int)(-MAIN_CHAROBJECT->m_Position.z);
			int targetX = (int)pSelCharObject->m_Position.x;
			int targetY = (int)(-pSelCharObject->m_Position.z);
			int sideX = 0, sideY = 0;
			int stepMode = (s_nStuckCount == 1) ? 0 : 1; // 第一次斜后退脱离树干吸附，第二次大角度侧切绕出
			if (FindDetourWaypoint(curX, curY, targetX, targetY, sideX, sideY, stepMode)) {
				s_nDetourTargetX = sideX;
				s_nDetourTargetY = sideY;
				s_dwDetourUntil = dwNow + (stepMode == 0 ? 950 : 1200);

				Vector3 vSideTarget((float)sideX, pSelCharObject->m_Position.y, -(float)sideY);
				WORD angle = 0;
				bMove = TRUE;
				MoveTime = g_dwCurTime;

				MAIN_CHAROBJECT->SetAngleTarget( vSideTarget );
				MAIN_CHAROBJECT->GetAngle( angle );
				MAIN_CHAROBJECT->Update( 1 );
				MAIN_CHAROBJECT->SetTargetMove( (WORD)sideX, (WORD)sideY, eLBP_CharNavigation, 0 );

				if (g_MainCharInfo.m_bFastMove)
				{
					int fastIdx = (g_MainCharInfo.m_nFastIndex > 0) ? g_MainCharInfo.m_nFastIndex : 4;
					if (MAIN_CHAROBJECT->m_nCurMotionType != XiahAniType::eLAT_Mugong || MAIN_CHAROBJECT->m_nCurAniIndex != fastIdx)
					{
						MAIN_CHAROBJECT->SetAnimation(XiahAniType::eLAT_Mugong, fastIdx, 0.7f);
					}
				}
				else
				{
					if (MAIN_CHAROBJECT->m_nCurMotionType != XiahAniType::eLAT_Run)
					{
						MAIN_CHAROBJECT->SetAnimation(XiahAniType::eLAT_Run, 1);
					}
					float fMoveSpeed = (float)(g_MainCharInfo.m_bWalkSpeed + g_MainCharInfo.m_bPlusSpeed) / 9.0f;
					MAIN_CHAROBJECT->m_CharRender.SetAnimationSpeed(fMoveSpeed);
				}
				SendCS_NV_STARTMOVE_REQ(g_pMainChar->m_dwServerID,
					MAIN_CHAROBJECT->m_Position.x, -MAIN_CHAROBJECT->m_Position.z, MAIN_CHAROBJECT->m_Position.y,
					(WORD)sideX, (WORD)sideY, (BYTE)vSideTarget.y, (WORD)angle, CHARSTATE_NORMAL, 0);
				return TRUE;
			}
		} else {
			// 连续多次尝试脱困依然无法动弹，判定为大树死角，仅对怪物拉黑换怪！
			if (dwSelObjectType == OBJTYPE_NPC) {
				AddStuckMonster(dwSelObjectID, 30000);
			}
			dwSelObjectID = 0;
			dwSelObjectType = 0;
			bAutoNavigation = FALSE;
			bAutoAttack = FALSE;
			STOP_MAINCHAR;
			s_nStuckCount = 0;
			s_dwDetourUntil = 0;
			return TRUE;
		}
	}

	// 定点攻击模式：站原地不动攻击，方便群攻与定点刷怪，严禁发起移动位移
	extern BOOL g_bFixedPointAttack;
	if (g_bFixedPointAttack)
	{
		if (bMove)
		{
			g_MainChar_PreAttackInfo.nRemainAttackCount = 0;
			g_MainChar_PreAttackInfo.dwLastPreAttackTime = 0;
			STOP_MAINCHAR;
		}

		MAIN_CHAROBJECT->SetAngleTarget(pSelCharObject->m_Position);

		if (g_bCheat)
		{
			if (g_PetList.size() == 0 || g_MainCharInfo.m_bCheat)
				InteractObject(dwSelObjectID, dwSelObjectType, 0);
		}
		else
		{
			InteractObject(dwSelObjectID, dwSelObjectType, 0);
		}

		if (!bAutoAttack && dwSelObjectType != OBJTYPE_PC)
		{
			bAutoNavigation = FALSE;
		}
		return TRUE;
	}

	// 거리 체크
	float fRange = pSelCharObject->GetInteractionDistance( MAIN_CHAROBJECT->m_Position);

	// 追怪移动时预留3码防抖；但功能NPC为固定目标，不需要加3码，使角色能正常走到9码交互范围内
	if( bMove && dwSelObjectType != OBJTYPE_FUNCTIONALNPC)
		fRange += 3;

	// 인터렉션 거리가 범위안에 들어오면 동작을 시작한다.
	if( fRange < fInteractionRange)
	{
		int curX = (int)MAIN_CHAROBJECT->m_Position.x;
		int curY = (int)(-MAIN_CHAROBJECT->m_Position.z);
		int targetX = (int)pSelCharObject->m_Position.x;
		int targetY = (int)(-pSelCharObject->m_Position.z);
		// 严密视线检测：绝对不能仅凭 fRange <= 3.5f 盲目停步！隔着大树或墙壁必须继续绕行！
		bool bCanSeeTarget = CheckMapLineOfSight(curX, curY, targetX, targetY);

		// 必须在无障碍物遮挡时才判定进入攻击状态（防止隔着大树或墙壁对着障碍物空挥）
		if (bCanSeeTarget)
		{
			// 움직이는중이면 멈추어주고
			if( bMove)
			{
				g_MainChar_PreAttackInfo.nRemainAttackCount	 = 0;
				g_MainChar_PreAttackInfo.dwLastPreAttackTime = 0;
				STOP_MAINCHAR;
			}

			// 인터렉션 처리를 한다
			if(g_bCheat)
			{
				if(g_PetList.size() == 0 || g_MainCharInfo.m_bCheat)
					InteractObject( dwSelObjectID, dwSelObjectType, 0);
			}
			else
				InteractObject( dwSelObjectID, dwSelObjectType, 0);

			if( !bAutoAttack && dwSelObjectType != OBJTYPE_PC)
			{
				bAutoNavigation = FALSE;
			}
			return TRUE;
		}
		// 如果隔着大树或墙壁，绝不执行停步攻击，继续由下方的寻路逻辑绕过障碍物！
	}

	if( g_dwCurTime - dwAITime > 1000 || mode == 0)
	{
		dwAITime = g_dwCurTime;

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
				KEEPMOVE_MAINCHAR ;	// 움직임을 유지해야한다. 아니면 끈기는 동작이 나온다.
				g_MainChar_PreAttackInfo.nRemainAttackCount	 = 0;
				g_MainChar_PreAttackInfo.dwLastPreAttackTime = 0;
			}
		}
	}

	return TRUE;
}
