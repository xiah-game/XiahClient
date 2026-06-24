#include "precompile.h"
#include "Xiahgamemain.h"
#include "XiahGameObject.h"
#include "CharacterInfo.h"
#include "XiahGame_Pet.h"
#include "XiahGame_Handler_Sender.h"
#include "XiahMap.h"
#include "XiahGame_Main.h"

//HT_CHEAT : 치트 키
extern  BOOL g_bCheat;

#define WARP_PET	\
			pObject->SetAnimation( XiahAniType::eLAT_Run, 0);	\
			pObject->GetAngle( wDirection);\
			SendCS_NC_SYNCMOVE_REQ( dwObjectID, pObject->m_Position.x, -pObject->m_Position.z, pObject->m_Position.y, pObject->m_TargetPosition.x, -pObject->m_TargetPosition.z, pObject->m_TargetPosition.y, wDirection, pObject->m_bObjStatus,bMoveSpeed);

#define STOP_PET	\
			pObject->SetAnimation( XiahAniType::eLAT_Stand, 0);	\
			pObject->GetAngle( wDirection);\
			SendCS_NC_ENDMOVE_REQ( dwObjectID, pObject->m_Position.x, -pObject->m_Position.z, pObject->m_Position.y, wDirection, 0,bMoveSpeed);

#define SYNCMOVE_PET	\
			pObject->GetAngle( wDirection);\
			SendCS_NC_SYNCMOVE_REQ( dwObjectID, pObject->m_Position.x, -pObject->m_Position.z, pObject->m_Position.y, pObject->m_TargetPosition.x, -pObject->m_TargetPosition.z, pObject->m_TargetPosition.y, wDirection, pObject->m_bObjStatus,bMoveSpeed);

#define STARTMOVE_PET_WALK	\
			pPetInfo->dwMoveTime = g_dwCurTime;\
			pObject->SetAnimation( XiahAniType::eLAT_Walk, 0);\
			pObject->m_CharRender.SetAnimationSpeed( fMoveSpeed);\
			pObject->SetAngleTarget( vTarget);\
			pObject->SetTargetMove( vTarget.x, -vTarget.z, eLBP_CharNavigation, 0);\
			pObject->GetAngle( wDirection);\
			pObject->m_bObjStatus = NPCSTATUS_WALK;\
			SendCS_NC_STARTMOVE_REQ( dwObjectID, pObject->m_Position.x, -pObject->m_Position.z, pObject->m_Position.y, pObject->m_TargetPosition.x, -pObject->m_TargetPosition.z, pObject->m_TargetPosition.y, wDirection, NPCSTATUS_WALK,bMoveSpeed);

#define STARTMOVE_PET_RUN	\
			pPetInfo->dwMoveTime = g_dwCurTime;\
			pObject->SetAnimation( XiahAniType::eLAT_Run, 0);\
			pObject->m_CharRender.SetAnimationSpeed( fMoveSpeed);\
			pObject->SetAngleTarget( vTarget);\
			pObject->SetTargetMove( vTarget.x, -vTarget.z, eLBP_CharNavigation, 0);\
			pObject->GetAngle( wDirection);\
			pObject->m_bObjStatus = NPCSTATUS_RUN;\
			SendCS_NC_STARTMOVE_REQ( dwObjectID, pObject->m_Position.x, -pObject->m_Position.z, pObject->m_Position.y,pObject->m_TargetPosition.x, -pObject->m_TargetPosition.z, pObject->m_TargetPosition.y, wDirection, NPCSTATUS_RUN,bMoveSpeed);

// 분신격 움직임
#define STARTMOVE_PET_RUN2	\
			pPetInfo->dwMoveTime = g_dwCurTime;\
			pObject->SetAnimation( XiahAniType::eLAT_Run, 1);\
			pObject->m_CharRender.SetAnimationSpeed( fMoveSpeed);\
			pObject->SetAngleTarget( vTarget);\
			pObject->SetTargetMove( vTarget.x, -vTarget.z, eLBP_CharNavigation, 0);\
			pObject->GetAngle( wDirection);\
			pObject->m_bObjStatus = NPCSTATUS_RUN;\
			SendCS_NC_STARTMOVE_REQ( dwObjectID, pObject->m_Position.x, -pObject->m_Position.z, pObject->m_Position.y, pObject->m_TargetPosition.x,	-pObject->m_TargetPosition.z, pObject->m_TargetPosition.y,  wDirection, CHARSTATE_NORMAL,bMoveSpeed);
	

#define STATUSCHANGE_PET \
			pObject->GetAngle( wDirection);\
			SendCS_NC_STATUSCHANGE_REQ( OBJTYPE_PET, dwObjectID, pObject->m_bObjStatus, wDirection);

#define PREATTACK_PET	\
			SendCS_BT_PREATTACK_REQ( OBJTYPE_PET, dwObjectID, dwPetSelObjectType, dwPetSelObjectID, pObject->m_Position.x, -pObject->m_Position.z, pObject->m_Position.y, 0);

BOOL InteractObject(DWORD dwObjectID,BYTE bObjType,int mode);



// 业务设计意图：战斗状态下宠物/幻兽的最大活动半径与闪回极限值定义，用以控制跟随和防拉扯边界。
// 为适应大范围战斗与拉怪需求，将活动半径放宽至 60 格（上限贴合九宫格视野同步极限）。
const float MAX_WARP_DIST_IN_COMBAT = 60.0f;
const float MAX_FOLLOW_DIST_IN_COMBAT = 60.0f;

// 실제의 AI
BOOL ProcessPETAI(DWORD dwObjectID,CXiahCharObject* pObject,sPetInfo* pPetInfo,CXiahCharObject* pTargetChar)
{
	BOOL ret = FALSE;
	WORD wDirection;
	Vector3 vTarget;

	
    float fMoveSpeed = 2.0f;
	if(pPetInfo->m_dwIsHwan == 0)
		fMoveSpeed = 1.9f;
	else 
		fMoveSpeed = 1.2f;
	// [12/20/2004] 아바타 펫(광견이 느려서 빠르게)
//	if(pPetInfo->bRevolutionStep == 4 && pPetInfo->bNpcType == 0) //HT_0621 : 영수둔갑신단 적용
//		fMoveSpeed = 1.5f;

	BYTE bMoveSpeed = fMoveSpeed * 9;

	switch(pPetInfo->AI_Type)
	{
		// 더이상 오지 않는다.
		case PETAI_CALLTOME:
			//pPetInfo->AI_Type = PETAI_AUTOATTACK;
		break;

		// 자동 공격 PC가 공격하는 ID를 공격
		case PETAI_AUTOATTACK :
		{
			if(pPetInfo->dwDestType == OBJTYPE_NPC && pPetInfo->dwDestID != NULL)
			{
				pPetInfo->bFight = TRUE;

				vTarget = pTargetChar->m_Position;
				int FR = pPetInfo->fFollowRange;
				vTarget.x += (FR / 2) + -(rand() % FR);
				vTarget.z += (FR / 2) + -(rand() % FR);

				WORD desAngle = pObject->GetTargetAngle( vTarget);
				WORD curAngle;
				WORD diffAngle;

				pObject->GetAngle( curAngle);
				diffAngle = ABS( curAngle - desAngle);

				// 대상과 Pet과의 거리
				float fDistance = pTargetChar->GetInteractionDistance( pObject->m_Position);

				if( (fDistance*2) > pPetInfo->fFollowRange) // 멀리 있다
				{
					if(pObject->m_nCurMotionType != XiahAniType::eLAT_Run && pObject->m_nCurMotionType != XiahAniType::eLAT_Walk)
					{
						if( diffAngle > 30)	STOP_PET;
						STARTMOVE_PET_RUN;
					}
				}
				else
				{
					SendCS_BT_PREATTACK_REQ( OBJTYPE_PET, dwObjectID, pPetInfo->dwDestType, pPetInfo->dwDestID, pObject->m_Position.x, -pObject->m_Position.z, pObject->m_Position.y, 0);
					pPetInfo->dwLastAttackTime = g_dwCurTime;	// PreAttack_Ack올때까지 기다려주려면.
				}
				ret = TRUE;
			}
		}
		break;

		// 대상공격
		case PETAI_TARGETATTACK :
		{
			if(pPetInfo->dwDestID == NULL)
				break;

			if( pTargetChar == NULL )
			{
				pPetInfo->bFight = FALSE;
				pPetInfo->dwDestID = 0;
				pPetInfo->dwDestType = 0;
				pPetInfo->dwGuardID = 0;
				pPetInfo->dwGuardType = 0;
				//pPetInfo->AI_Type = PETAI_AUTOATTACK;
				break;
			}

			pPetInfo->bFight = TRUE;
			vTarget = pTargetChar->m_Position;
			int FR = pPetInfo->fFollowRange;
			vTarget.x += (FR / 2) + -(rand() % FR);
			vTarget.z += (FR / 2) + -(rand() % FR);

			WORD desAngle = pObject->GetTargetAngle( vTarget);
			WORD curAngle;
			WORD diffAngle;

			pObject->GetAngle( curAngle);
			diffAngle = ABS( curAngle - desAngle);

			// 대상과 Pet과의 거리
			float fDistance = pTargetChar->GetInteractionDistance( pObject->m_Position);

			// 业务设计意图：指定攻击判定时，必须使用攻击距离 pPetInfo->fAttackRange (如5.0格)，
			// 绝对不能误用跟随距离 pPetInfo->fFollowRange (仅2.0格)，否则因怪物碰撞体积阻挡无法贴近到2.0格内，宠物将永远在怪物前跑步而不发起普通攻击。
			float fTargetRange = (pPetInfo->fAttackRange > 0.0f) ? pPetInfo->fAttackRange : pPetInfo->fFollowRange;
			if( fDistance > fTargetRange) // 멀리 있다
			{
				if(pObject->m_nCurMotionType != XiahAniType::eLAT_Run && pObject->m_nCurMotionType != XiahAniType::eLAT_Walk)
				{
					if( diffAngle > 30)	STOP_PET;
					STARTMOVE_PET_RUN;
				}
			}
			else if(pPetInfo->m_dwIsHwan == 2)
			{
				//HT_CHEAT : 분신격은 몬스터 한대만 때리자 ㅡㅡ;;
				if(pObject->m_dwMaxHP * 0.5 < pObject->m_dwCurHP)
				{
					// 대상을 공격해준다
					SendCS_BT_PREATTACK_REQ( OBJTYPE_PET, dwObjectID, pPetInfo->dwDestType, pPetInfo->dwDestID, pObject->m_Position.x, -pObject->m_Position.z, pObject->m_Position.y, 0);
					pPetInfo->dwLastAttackTime = g_dwCurTime;	// PreAttack_Ack올때까지 기다려주려면.
				}
				//// 대상을 공격해준다 (자기 방어)
				//SendCS_BT_PREATTACK_REQ( OBJTYPE_PET, dwObjectID, pPetInfo->dwDestType, pPetInfo->dwDestID, pObject->m_Position.x, -pObject->m_Position.z, pObject->m_Position.y, 0);
				//pPetInfo->dwLastAttackTime = g_dwCurTime;	// PreAttack_Ack올때까지 기다려주려면.
			}
			else
			{
				// 대상을 공격해준다 (자기 방어)
				SendCS_BT_PREATTACK_REQ( OBJTYPE_PET, dwObjectID, pPetInfo->dwDestType, pPetInfo->dwDestID, pObject->m_Position.x, -pObject->m_Position.z, pObject->m_Position.y, 0);
				pPetInfo->dwLastAttackTime = g_dwCurTime;	// PreAttack_Ack올때까지 기다려주려면.
			}
			ret = TRUE;
		}		
		break;

		// 아이템 수집.
		case PETAI_TAKEITEM	:
		{
			float min_len = 1000.0f;
			CXiahCharObject* ItemObj = NULL;

			// 가장 가까운것을 찾고
			XiahObject::CXiahObjectManager::iterator it;
			for(it = XiahObject::g_XiahObjectManager.begin(); it != XiahObject::g_XiahObjectManager.end(); it++)
			{
				XiahObject::CXiahObject* t_Object = it->second;
				CXiahCharObject* pCharObject = (CXiahCharObject*) t_Object->m_pObject;

				if(t_Object == NULL || pCharObject == NULL)
				{
					DBG_LogFile( _T("ProcessPETAI 실패"));
//					return false;
				}

				// 아이템이면 집는다.
				if( pCharObject->m_bObjType == OBJTYPE_ITEM)
				{
					float Len = pCharObject->GetInteractionDistance(pObject->m_Position);
					if(min_len > Len)
					{
						min_len = Len;
						ItemObj = pCharObject;
					}
				}
			}

			// 수집할것이 있다!
			if(ItemObj)
			{
				pPetInfo->bFight = TRUE;
				vTarget = ItemObj->m_Position;
				int FR = pPetInfo->fFollowRange;
				vTarget.x += (FR / 2) + -(rand() % FR);
				vTarget.z += (FR / 2) + -(rand() % FR);

				WORD desAngle = pObject->GetTargetAngle( vTarget);
				WORD curAngle;
				WORD diffAngle;

				pObject->GetAngle( curAngle);
				diffAngle = ABS( curAngle - desAngle);

				// 수집할 ITEM 과의 거리
				float fDistance = ItemObj->GetInteractionDistance( pObject->m_Position);

				if( fDistance > pPetInfo->fFollowRange) // 멀리 있다
				{
					if(pObject->m_nCurMotionType != XiahAniType::eLAT_Run && pObject->m_nCurMotionType != XiahAniType::eLAT_Walk)
					{
						if( diffAngle > 30)	STOP_PET;

						// 2004.08.05 Changth						
						if( pPetInfo->m_dwIsHwan == 1 || pPetInfo->m_dwIsHwan == 2 )
						{
							fMoveSpeed = 3.3f;
							bMoveSpeed = fMoveSpeed * 9;
						}
		
						//YS_0810 : PETAI
						if ( pPetInfo->m_dwIsHwan == 2 )	
						{
							STARTMOVE_PET_RUN2;
						}
						else								
						{
							STARTMOVE_PET_RUN;
						}
					}
				}
				else
				{
					WORD wPosX,wPosY;
					ItemObj->GetPosition( wPosX, wPosY);
					XiahItem::sItemInfo* pInfo = (XiahItem::sItemInfo*)ItemObj->m_pPrivateData;

					//YS_0811 : BUGFIX
					if ( pInfo )
					{						
						SendCS_NC_PETPICKITEM_REQ(  pInfo->m_dwMapID, dwObjectID, pInfo->m_dwItemID,
													wPosX, wPosY,
													255, 255,
													pInfo->m_dwMapObjectID, pInfo->m_dwAmount );
					
					}

					pPetInfo->dwLastAttackTime = g_dwCurTime;	// PreAttack_Ack올때까지 기다려주려면.
				}
			}
			else
			{
				pPetInfo->bFight = FALSE;
			}
			ret = TRUE;
		}
		break;

		// 무공공격
		case PETAI_SPECIALATTACK :
		break;
	}
	return ret;
}


// AI LOOP
// dwObjectID : 해당 PET ID
// pObject : 해당 PET 오브젝트
BOOL PetAI(DWORD dwObjectID,CXiahCharObject* pObject)
{
	BOOL PetSelect = true;

	if( g_pMainChar == NULL || pObject == NULL) return TRUE;
	if( pObject->m_pPrivateData == 0) return TRUE; // 아직 완벽한 Pet가 아니다

	sPetInfo* pPetInfo = (sPetInfo*)pObject->m_pPrivateData;
	CXiahCharObject* pMainChar = (CXiahCharObject*)g_pMainChar->m_pObject;
	CXiahCharObject* pSelChar = NULL;

	if(pPetInfo == NULL || pMainChar == NULL)
	{
		DBG_LogFile( _T("PetAI 실패"));
//		return false;
	}

	// pTargetChar는 PC이다.
	CXiahCharObject* pTargetChar = pMainChar;

	WORD wDirection;
	Vector3 vTarget;

	// PET에게 AI가 있는가?
	if( pPetInfo->bAI == FALSE) return TRUE;

	// PET이 선택하고 있는 Obejct ID
	if( pPetInfo->dwDestID != NULL)
	{
		// PET이 선택중인 OBJECT를 구한다
		XiahObject::CXiahObject* pSelObject = XiahObject::g_XiahObjectManager.FindXiahObject( MAKEOBJECTID( 0, pPetInfo->dwDestID, pPetInfo->dwDestType));

		// 2004.08.04 Changth
		// NPC가 죽었는데 PET이 몇번 더 공격을 하는군. 이것을 수정한다. 상태를 파악하자.
		bool bExistDestObject = true;

		if( !pSelObject )
			bExistDestObject = false;

		CXiahCharObject* pSelCharObject = NULL;
		if( bExistDestObject )
		{
			pSelCharObject = (CXiahCharObject*)pSelObject->m_pObject;
			if( pSelCharObject )
			{
				if( pSelCharObject->m_bObjStatus == NPCSTATUS_DIE || !pSelCharObject->m_bRenderOK)
					bExistDestObject = false;
			}
			else
			{
				bExistDestObject = false;
			}
		}

		if( bExistDestObject )
			pSelChar = pSelCharObject;
		else
		{
			// 해당 OBJECT이 죽거나해서 사라진경우 초기화
			pPetInfo->dwDestID = 0;
			pPetInfo->dwDestType = 0;

			// 2004.08.04 Changth
			// 캐릭터와 동반 공격할때 사용하는 것으로, 펫을 지정 공격을 한 후 공격이 끝나면
			// 자동으로 자동공격으로 바뀌는데, 이때 마우스로 선택한 NPC의 ObjectID가 
			// dwSelObjectID 에 아직까지 저장되어 있다. 그래서 또다시 한번 더 공격하는 애니메이션이
			// 작동한다. 이것을 방지하기 위해서 이렇게 초기화를 해준다.
			dwSelObjectID = 0;
			dwSelObjectType = 0;
		}
	}
	else
	{
		// 자동공격은 타겟을 지정해줘야한다
		if(pPetInfo->AI_Type == PETAI_AUTOATTACK)
		{
			// PET의 DEST가 PET 자신일수 없다
			if(dwObjectID != dwSelObjectID)
			{
				// 2004.08.05 Changth
				// 분신격을 쓴 후, 일반 캐릭터를 클릭하면 분신이 바로 공격한다 이것을 막는다.
				// 단 비무, 문파전 일땐 클릭이 공격이므로 이때를 제외하곤 분신이 공격을 못하게 한다.
				// nRemainAttackCount == 1 이면 메인이 공격을 하는 상태다.
				if (pPetInfo->m_dwIsHwan == 2 || pPetInfo->m_dwIsHwan == 1)
				{
					if (dwSelObjectID != 0 && (dwSelObjectType == OBJTYPE_NPC || dwSelObjectType == OBJTYPE_PC))
					{
						pPetInfo->dwDestID = dwSelObjectID;
						pPetInfo->dwDestType = dwSelObjectType;
					}
					else
					{
						pPetInfo->dwDestID = 0;
						pPetInfo->dwDestType = 0;
					}
				}
				else
				{
					if( g_MainChar_PreAttackInfo.nRemainAttackCount == 0 )
					{
						pPetInfo->dwDestID = 0;
						pPetInfo->dwDestType = 0;
					}
					else
					{
						pPetInfo->dwDestID = dwSelObjectID;
						pPetInfo->dwDestType = dwSelObjectType;
					}
				}
			}
		}
	}

	// PET이 선택하고있는 OBJECT 없으면 MAIN PC를 선택한다
	if( pSelChar != NULL) pTargetChar = pSelChar;
	
	BOOL bMove = pObject->m_nCurMotionType == XiahAniType::eLAT_Run || pObject->m_nCurMotionType == XiahAniType::eLAT_Walk;

	//HT_CHEAT : 펫 속도 증가 시키자
	float fMoveSpeed = 1.5f;
	// [12/20/2004] 아바타 펫(광견이 느려서 빠르게)
	//if(pPetInfo->bRevolutionStep == 4 && pPetInfo->bNpcType == 0) //HT_0621 : 영수둔갑신단 적용
	//	fMoveSpeed = 1.5f;
	
	BYTE bMoveSpeed = fMoveSpeed * 9;

	float fFollowRange;

	if( g_dwCurTime - pPetInfo->dwLastAITime > pPetInfo->dwAIFrameTime)
	{
		// MAIN PC와 Pet과의 거리
		float fDistance = pMainChar->GetInteractionDistance( pObject->m_Position);

//////////////////////////////////
		switch(pPetInfo->m_dwIsHwan)
		{
			// 일반 PET
			case 0 :
				fMoveSpeed = 1.9f;
			{
				float dis = 9.0f;
			//	if(pPetInfo->m_dwIsHwan == 0) dis = 9.0f; //HT_CHEAT : 펫 사정거리 증가 시키자
			//	else
			//	if(pPetInfo->m_dwIsHwan == 1) dis = 1.5f; //환수유용... 

				// 멀리 있거나 호출
				if( fDistance > pPetInfo->fFollowRange * dis || pPetInfo->bFollowPC == TRUE || pPetInfo->AI_Type == PETAI_CALLTOME)
				{
					if(pPetInfo->AI_Type == PETAI_CALLTOME)
					{
						// 부를때는 가까이 까지 부른다.
						dis = 1.5f;

						if(fDistance < pPetInfo->fFollowRange * dis)
							pPetInfo->AI_Type = PETAI_AUTOATTACK;
					}

					pPetInfo->bFight = FALSE;
					pPetInfo->dwDestID = 0;
					pPetInfo->dwDestType = 0;
					pPetInfo->dwGuardID = 0;
					pPetInfo->dwGuardType = 0;

					// 무조건 1.5f 이하로 따라와야한다.
					if(fDistance > pPetInfo->fFollowRange * dis - 1.0f) 
						pPetInfo->bFollowPC = TRUE;
					else
						pPetInfo->bFollowPC = FALSE;

					vTarget = pMainChar->m_Position;
					// 메인 케렉터를 따라다닐때의 UPDATE
					if( pMainChar->m_bTargetMove) vTarget = pMainChar->m_TargetPosition;

					int FR = pPetInfo->fFollowRange;
					vTarget.x += (FR / 2) + -(rand() % FR);
					vTarget.z += (FR / 2) + -(rand() % FR);

					WORD desAngle = pObject->GetTargetAngle( vTarget);
					WORD curAngle;
					WORD diffAngle;

					pObject->GetAngle( curAngle);
					diffAngle = ABS( curAngle - desAngle);

					// 10배 떨어져 있으면 JUMP 한다
					if(fDistance > pPetInfo->fFollowRange * 10.0f)
					{
						int x = 16 - (rand() % 32);
						int y = 16 - (rand() % 32);

						pObject->SetPosition( pMainChar->m_Position.x + x , -pMainChar->m_Position.z + y);
						SendCS_NC_MAPENTER_REQ( dwObjectID, XiahMap::g_XiahMap.m_MapInfo.m_dwMapID);
						//SYNCMOVE_PET;
						WARP_PET;
					}
					else
					{
						// 요구 근접 거리보다 1.5배 차이 나면 뛴다
						BOOL bRun = fDistance > pPetInfo->fFollowRange * 1.5f; 
						#define STARTMOVE_PET	\
						if( bRun)\
						{\
							STARTMOVE_PET_RUN;\
						}\
						else\
						{\
							STARTMOVE_PET_WALK;\
						}

						//
						if( bMove == FALSE)
						{
							STARTMOVE_PET;
						}
						else
						{
							// 각도가 30도 이상 차이나면 멈췄다가 다시 움직임
							if( diffAngle > 30)
							{
								STOP_PET;
								STARTMOVE_PET;
							}
						}
						pPetInfo->bIdle = FALSE;
					}
				}
				else // 근처에 있다
				{
					// PET AI 실제 작동
					if(ProcessPETAI(dwObjectID,pObject,pPetInfo,pTargetChar) == FALSE)
					{
						// 움직이고 있었다면 멈춘다
						if( bMove)
						{
							STOP_PET;
						}

						// IDLE MODE 시작을 알림
						if( pPetInfo->bFight == FALSE && pPetInfo->bIdle == FALSE && pTargetChar->m_bObjType == OBJTYPE_PC)
						{
							pPetInfo->bIdle = TRUE;
							pPetInfo->dwStartIdleTime = g_dwCurTime;
							pPetInfo->nIdleStep = 0;
							pPetInfo->dwIdleStepDelay = 1500 + rand() % 3000;
							pPetInfo->dwIdleStepTime = g_dwCurTime;
						}

						// PET의 IDLE 상태
						if( pPetInfo->bIdle && pPetInfo->bFight == FALSE)
						{
							switch( pPetInfo->nIdleStep)
							{

							case 0:	// 인제 idle되기 시작했다
								if( g_dwCurTime - pPetInfo->dwIdleStepTime > pPetInfo->dwIdleStepDelay)
								{
									// NPCSTATUS_IDLE
									// 주저 앉기 시작 (광견기준)
									pObject->m_bObjStatus = NPCSTATUS_IDLE;
									pObject->SetAnimation( XiahAniType::eLAT_Idle, 0);
									pObject->m_CharRender.SetLoopAnimation( FALSE);

									pPetInfo->nIdleStep = 1;

									pPetInfo->dwIdleStepDelay = pObject->m_CharRender.GetAnimationLength();
									pPetInfo->dwIdleStepTime = g_dwCurTime;
									// 나중에 NpcType별로 잡아 준다
									STATUSCHANGE_PET;
									DBG_Put("0");
								}
								break;
							case 1: // idle1
								if( g_dwCurTime - pPetInfo->dwIdleStepTime > pPetInfo->dwIdleStepDelay)
								{
									// 앉은 상태 두리번 (광견기준)
									pObject->m_bObjStatus = NPCSTATUS_IDLE1;
									pObject->SetAnimation( XiahAniType::eLAT_Idle, 1);
									pObject->m_CharRender.SetLoopAnimation( FALSE);

									pPetInfo->dwIdleStepDelay = pObject->m_CharRender.GetAnimationLength();
									pPetInfo->dwIdleStepTime = g_dwCurTime;

									pPetInfo->nIdleStep = 1 + rand() % 4;
									STATUSCHANGE_PET;
									DBG_Put("1");
								}
								break;

							case 2: // idle1
							case 3: // 일어나기
								if( g_dwCurTime - pPetInfo->dwIdleStepTime > pPetInfo->dwIdleStepDelay)
								{
									// 일어나! (광견기준)
									pObject->m_bObjStatus = NPCSTATUS_WAKEUP;

									// 앉는거를 꺼꾸로 돌리고, 다음 에니메이션은 Stand
									pObject->SetAnimation( XiahAniType::eLAT_Idle, XiahAniType::eLAT_Stand,0,  0);
									pObject->m_CharRender.SetReverseAnimation();
									pObject->m_CharRender.SetLoopAnimation( FALSE);

									if( rand() % 5)
										pPetInfo->nIdleStep = 4;
									else
										pPetInfo->nIdleStep = 0;

									pPetInfo->dwIdleStepDelay = pObject->m_CharRender.GetAnimationLength()+ 500 + rand() % 3000;
									pPetInfo->dwIdleStepTime = g_dwCurTime;
									STATUSCHANGE_PET;
									DBG_Put("2,3");
								}
								break;

								// PC 주위 방황하기.
							case 4 :
								if( g_dwCurTime - pPetInfo->dwIdleStepTime > pPetInfo->dwIdleStepDelay)
								{
									// 주위 방황
									pObject->m_bObjStatus = NPCSTATUS_WALK;		// 주인공 주위 방황하기 (IDLE)
									vTarget = pMainChar->m_Position;
									int FR = pPetInfo->fFollowRange;
									vTarget.x += (FR / 2) + -(rand() % FR);
									vTarget.z += (FR / 2) + -(rand() % FR);

									WORD desAngle = pObject->GetTargetAngle( vTarget);
									WORD curAngle;
									WORD diffAngle;

									pObject->GetAngle( curAngle);
									diffAngle = ABS( curAngle - desAngle);

									STARTMOVE_PET_WALK;

									if( rand() % 2)
										pPetInfo->nIdleStep = 2;
									else
										pPetInfo->nIdleStep = 0;

									pPetInfo->dwIdleStepDelay = 3000;
									pPetInfo->dwIdleStepTime = g_dwCurTime;
									//STATUSCHANGE_PET;					
									DBG_Put("4");
								}
								break;
							}// IDLE 종류
						}// IDLE 상태?
					}// AI 탓나?
				}
			}
			break;

			// 분신격 
			case 2 :
			{
				float dis = 3.0f; //HT_CHEAT : 환수유 사정 거리 증가..

				bMoveSpeed = pPetInfo->bSpeed;

			/*	if( pPetInfo->bHwanAttack )
					fFollowRange = pPetInfo->fAttackRange;
				else*/
					fFollowRange = pPetInfo->fFollowRange;

				float fWarpDistance = MAX_WARP_DIST_IN_COMBAT;
				float fMaxFollowDist = fFollowRange * dis;
				if (pPetInfo->dwDestID != 0)
				{
					fMaxFollowDist = MAX_FOLLOW_DIST_IN_COMBAT;
					pPetInfo->bFollowPC = FALSE;
				}

				if( fDistance > fMaxFollowDist || pPetInfo->bFollowPC == TRUE ) // 멀리 있다
				{
					// 业务设计意图：仅在没有攻击目标，或者已远超闪回距离导致拉回时，才允许在非战斗状态清空攻击目标。
					// 这样能有效阻断分身在中途跑向怪物的过程中（bFight尚为FALSE）因为距离主人的跟随判定而丢失目标。
					if(!pPetInfo->bFight && (pPetInfo->dwDestID == 0 || fDistance > fWarpDistance))
					{
						pPetInfo->bFight		= FALSE;
						pPetInfo->dwDestID		= 0;
						pPetInfo->dwDestType	= 0;
						pPetInfo->dwGuardID		= 0;
						pPetInfo->dwGuardType	= 0;
					}

					// 무조건 1.5f 이하로 따라와야한다. 작은 범위 안에서
					if( fDistance > pPetInfo->fFollowRange * dis) //HT_CHEAT : 범위를 넘히자
					{
						pPetInfo->bFollowPC = TRUE;
					}
					else
					{
						pPetInfo->bFollowPC = FALSE;
					}

					vTarget = pMainChar->m_Position;
					// 메인 케렉터를 따라다닐때의 UPDATE
					if( pMainChar->m_bTargetMove) vTarget = pMainChar->m_TargetPosition;
					int FR = pPetInfo->fFollowRange;
					vTarget.x += (FR / 2) + -(rand() % FR);
					vTarget.z += (FR / 2) + -(rand() % FR);

					WORD desAngle = pObject->GetTargetAngle( vTarget);
					WORD curAngle;
					WORD diffAngle;

					pObject->GetAngle( curAngle);
					diffAngle = ABS( curAngle - desAngle);

					// 5배 떨어져 있으면 JUMP 한다
					if(fDistance > fWarpDistance)
					{
						int x = 16 - (rand() % 32);
						int y = 16 - (rand() % 32);

						pObject->SetPosition( pMainChar->m_Position.x + x , -pMainChar->m_Position.z + y);
						SendCS_NC_MAPENTER_REQ( dwObjectID, XiahMap::g_XiahMap.m_MapInfo.m_dwMapID);
						//SYNCMOVE_PET;
						WARP_PET;
					}
					else
					{
						// 이놈들은 무조껀 뛴다.
						if( bMove == FALSE)
						{
							//YS_0810 : PETAI
							fMoveSpeed = 3.3f;
							bMoveSpeed = fMoveSpeed * 9;

						//	if ( pPetInfo->m_dwIsHwan == 2 )
						//	{
								STARTMOVE_PET_RUN2;
						//	}
						//	else
						//	{
						//		STARTMOVE_PET_RUN;
						//	}
						}
						else
						{
							// 각도가 30도 이상 차이나면 멈췄다가 다시 움직임
							if( diffAngle > 30)
							{
								STOP_PET;

								//YS_0810 : PETAI
							//	if ( pPetInfo->m_dwIsHwan == 2 ) 
							//	{
									STARTMOVE_PET_RUN2;
							//	}
							//	else							 
							//	{
							//		STARTMOVE_PET_RUN;
							//	}
							}
						}

						pPetInfo->bIdle = FALSE;
					}
				}
				else // 근처에 있다
				{
					// 움직이고 있었다면 멈춘다
					if( bMove)
					{
						STOP_PET;
					}

//					ProcessPETAI(dwObjectID,pObject,pPetInfo,pTargetChar);
					if(ProcessPETAI(dwObjectID,pObject,pPetInfo,pTargetChar) == FALSE)
					{

					// 대상공격
						if(pPetInfo->dwDestID != NULL && pTargetChar != NULL)
						{
							pPetInfo->bFight = TRUE;
							vTarget = pTargetChar->m_Position;
							int FR = pPetInfo->fFollowRange;
							vTarget.x += (FR / 2) + -(rand() % FR);
							vTarget.z += (FR / 2) + -(rand() % FR);

							pPetInfo->bHwanAttack = TRUE;

							WORD desAngle = pObject->GetTargetAngle( vTarget);
							WORD curAngle;
							WORD diffAngle;

							pObject->GetAngle( curAngle);
							diffAngle = ABS( curAngle - desAngle);

							// 대상과 Pet과의 거리
							float fDistance = pTargetChar->GetInteractionDistance( pObject->m_Position);

							// 业务设计意图：战斗贴身攻击判定时，必须使用已根据战斗状态正确切换为攻击距离的本地变量 fFollowRange，
							// 绝对不能误用跟随距离 pPetInfo->fFollowRange (仅2.0格)，否则会导致分身在怪物身旁只跑步不打怪。
							if( fDistance > fFollowRange) // 멀리 있다
							{
								//YS_0810 : PETAI
								fMoveSpeed = 3.3f;
								bMoveSpeed = fMoveSpeed * 9;
								
							//	if ( pPetInfo->m_dwIsHwan == 2 ) 
							//	{
									STARTMOVE_PET_RUN2;
							//	}
							//	else							 
							//	{
							//		STARTMOVE_PET_RUN;
							//	}
								
							}
							else if( g_dwCurTime - pPetInfo->dwLastAttackTime > pPetInfo->dwAttackDelayTime)
							{
								// 대상을 공격해준다
								SendCS_BT_PREATTACK_REQ( OBJTYPE_PET, dwObjectID, pPetInfo->dwDestType, pPetInfo->dwDestID, pObject->m_Position.x, -pObject->m_Position.z, pObject->m_Position.y, 0);
								pPetInfo->dwLastAttackTime = g_dwCurTime;	// PreAttack_Ack올때까지 기다려주려면.
							}
						}
						else
						{
							pPetInfo->bFight = FALSE;
							pPetInfo->dwDestID = 0;
							pPetInfo->dwDestType = 0;
							pPetInfo->dwGuardID = 0;
							pPetInfo->dwGuardType = 0;
							pPetInfo->AI_Type = PETAI_AUTOATTACK;

							pPetInfo->bHwanAttack = FALSE;
						}
					}
				}
			//	break;
			}
			break; //HT_CHEAT : 분신은 때리지마.. 

			case 1 :
			{
				float dis = 3.0f; //HT_CHEAT : 환수유 사정 거리 증가..
				fMoveSpeed = 1.2f;

				bMoveSpeed = pPetInfo->bSpeed;

				if( pPetInfo->bHwanAttack )
					fFollowRange = pPetInfo->fAttackRange;
				else
					fFollowRange = pPetInfo->fFollowRange;

				float fWarpDistance = pPetInfo->fFollowRange * 5.0f;
				if (pPetInfo->dwDestID != 0) fWarpDistance = MAX_WARP_DIST_IN_COMBAT;

				float fMaxFollowDist = fFollowRange * dis;
				if (pPetInfo->dwDestID != 0)
				{
					fMaxFollowDist = MAX_FOLLOW_DIST_IN_COMBAT;
					pPetInfo->bFollowPC = FALSE;
				}

				TCHAR szLog[256];
				_stprintf(szLog, _T("[PetAI-Dragon] DestID: %d, Distance: %f, MaxFollow: %f, FollowPC: %d"), pPetInfo->dwDestID, fDistance, fMaxFollowDist, pPetInfo->bFollowPC);
				DBG_LogFile(szLog);

				if( fDistance > fMaxFollowDist || pPetInfo->bFollowPC == TRUE ) // 멀리 있다
				{
					// 业务设计意图：仅在没有攻击目标，或者已远超闪回距离导致拉回时，才允许在非战斗状态清空攻击目标。
					// 这样能有效阻断幻兽龙在中途跑向怪物的过程中（bFight尚为FALSE）因为距离主人的跟随判定而丢失目标。
					if(!pPetInfo->bFight && (pPetInfo->dwDestID == 0 || fDistance > fWarpDistance))
					{
						pPetInfo->bFight		= FALSE;
						pPetInfo->dwDestID		= 0;
						pPetInfo->dwDestType	= 0;
						pPetInfo->dwGuardID		= 0;
						pPetInfo->dwGuardType	= 0;
					}

					// 무조건 1.5f 이하로 따라와야한다. 작은 범위 안에서
					if( fDistance > pPetInfo->fFollowRange * dis) //HT_CHEAT : 범위를 넘히자
					{
						pPetInfo->bFollowPC = TRUE;
					}
					else
					{
						pPetInfo->bFollowPC = FALSE;
					}

					vTarget = pMainChar->m_Position;
					// 메인 케렉터를 따라다닐때의 UPDATE
					if( pMainChar->m_bTargetMove) vTarget = pMainChar->m_TargetPosition;
					int FR = pPetInfo->fFollowRange;
					vTarget.x += (FR / 2) + -(rand() % FR);
					vTarget.z += (FR / 2) + -(rand() % FR);

					WORD desAngle = pObject->GetTargetAngle( vTarget);
					WORD curAngle;
					WORD diffAngle;

					pObject->GetAngle( curAngle);
					diffAngle = ABS( curAngle - desAngle);

					// 5배 떨어져 있으면 JUMP 한다
					TCHAR szWarpLog[256];
					_stprintf(szWarpLog, _T("[PetAI-Dragon] Checking Warp. Dist: %f, WarpDist: %f"), fDistance, fWarpDistance);
					DBG_LogFile(szWarpLog);

					if(fDistance > fWarpDistance)
					{
						TCHAR szWarpLog2[256];
						_stprintf(szWarpLog2, _T("[PetAI-Dragon] WARP JUMP EXECUTE! OwnerPos: (%f, %f)"), pMainChar->m_Position.x, pMainChar->m_Position.z);
						DBG_LogFile(szWarpLog2);

						int x = 16 - (rand() % 32);
						int y = 16 - (rand() % 32);

						pObject->SetPosition( pMainChar->m_Position.x + x , -pMainChar->m_Position.z + y);
						SendCS_NC_MAPENTER_REQ( dwObjectID, XiahMap::g_XiahMap.m_MapInfo.m_dwMapID);
						//SYNCMOVE_PET;
						WARP_PET;
					}
					else
					{
						// 이놈들은 무조껀 뛴다.
						if( bMove == FALSE)
						{
							//YS_0810 : PETAI
							fMoveSpeed = 3.3f;
							bMoveSpeed = fMoveSpeed * 9;

						/*	if ( pPetInfo->m_dwIsHwan == 2 )
							{
								STARTMOVE_PET_RUN2;
							}
							else
							{*/
								STARTMOVE_PET_RUN;
							//}
						}
						else
						{
							// 각도가 30도 이상 차이나면 멈췄다가 다시 움직임
							if( diffAngle > 30)
							{
								STOP_PET;

								//YS_0810 : PETAI
							/*	if ( pPetInfo->m_dwIsHwan == 2 ) 
								{
									STARTMOVE_PET_RUN2;
								}
								else							 
								{*/
									STARTMOVE_PET_RUN;
								//}
							}
						}

						pPetInfo->bIdle = FALSE;
					}
				}
				else // 근처에 있다
				{
					// 움직이고 있었다면 멈춘다
					if( bMove)
					{
						STOP_PET;
					}

					ProcessPETAI(dwObjectID,pObject,pPetInfo,pTargetChar);

					// 대상공격
					if(pPetInfo->dwDestID != NULL && pTargetChar != NULL)
					{
						pPetInfo->bFight = TRUE;
						vTarget = pTargetChar->m_Position;
						int FR = pPetInfo->fFollowRange;
						vTarget.x += (FR / 2) + -(rand() % FR);
						vTarget.z += (FR / 2) + -(rand() % FR);

						pPetInfo->bHwanAttack = TRUE;

						WORD desAngle = pObject->GetTargetAngle( vTarget);
						WORD curAngle;
						WORD diffAngle;

						pObject->GetAngle( curAngle);
						diffAngle = ABS( curAngle - desAngle);

						// 대상과 Pet과의 거리
						float fDistance = pTargetChar->GetInteractionDistance( pObject->m_Position);

						// 业务设计意图：战斗贴身攻击判定时，必须使用已根据战斗状态正确切换为攻击距离的本地变量 fFollowRange，
						// 绝对不能误用跟随距离 pPetInfo->fFollowRange (仅2.0格)，否则会导致幻兽龙在怪物身旁只跑步不打怪。
						if( fDistance > fFollowRange) // 멀리 있다
						{
							//YS_0810 : PETAI
							fMoveSpeed = 3.3f;
							bMoveSpeed = fMoveSpeed * 9;
						/*	
							if ( pPetInfo->m_dwIsHwan == 2 ) 
							{
								STARTMOVE_PET_RUN2;
							}
							else							 
							{*/
								STARTMOVE_PET_RUN;
						//	}
							
						}
						else if( g_dwCurTime - pPetInfo->dwLastAttackTime > pPetInfo->dwAttackDelayTime)
						{
							TCHAR szAtkLog[256];
							_stprintf(szAtkLog, _T("[PetAI-Dragon] PreAttack sent. TargetID: %d, TargetType: %d, CurPos: (%f, %f), DestPos: (%f, %f)"), pPetInfo->dwDestID, pPetInfo->dwDestType, pObject->m_Position.x, pObject->m_Position.z, pTargetChar->m_Position.x, pTargetChar->m_Position.z);
							DBG_LogFile(szAtkLog);
							// 대상을 공격해준다
							SendCS_BT_PREATTACK_REQ( OBJTYPE_PET, dwObjectID, pPetInfo->dwDestType, pPetInfo->dwDestID, pObject->m_Position.x, -pObject->m_Position.z, pObject->m_Position.y, 0);
							pPetInfo->dwLastAttackTime = g_dwCurTime;	// PreAttack_Ack올때까지 기다려주려면.
						}
					}
					else
					{
						pPetInfo->bFight = FALSE;
						pPetInfo->dwDestID = 0;
						pPetInfo->dwDestType = 0;
						pPetInfo->dwGuardID = 0;
						pPetInfo->dwGuardType = 0;
						pPetInfo->AI_Type = PETAI_AUTOATTACK;

						pPetInfo->bHwanAttack = FALSE;
					}
				}
	//			break;
			}
			break;
		}
//////////////////////////////////////////////
		pPetInfo->dwLastAITime = g_dwCurTime;
	}

	// 움직이고 있다면 Sync
	if( g_dwCurTime - pPetInfo->dwMoveTime > 700 && bMove)
	{
		SYNCMOVE_PET;
		pPetInfo->dwMoveTime = g_dwCurTime;
	}

	//HT_CHEAT : 펫 사냥할게 있나 없나?
	if(g_bCheat)
		return pPetInfo->bFight;
	else
		return true;
}

int sPetInfo::OnEndTargetMove(unsigned long param)
{
	XiahObject::CXiahObject* pPetObject = g_PetList.Find( dwID);

	if( pPetObject == NULL)
		return 0;

	CXiahCharObject* pObject = (CXiahCharObject*)pPetObject->m_pObject;
	WORD wDirection;

	pObject->GetAngle( wDirection);

	SendCS_NC_ENDMOVE_REQ( dwID, pObject->m_Position.x, -pObject->m_Position.z, pObject->m_Position.y, wDirection, 0, 18);
	return 0;
}

int sPetInfo::OnTimerTrigger(unsigned long param)
{
	SendCS_BT_ATTACK_REQ(bAttackType, 
						dwAttackID, 
						wAttackPosX, 
						wAttackPosY, 
						bAttackHeight, 
						bDefType, 
						dwDefID, 
						bAttackMode);

	return 0;
}


