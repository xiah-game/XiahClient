#include "XiahCheatConfig.h"
#include "cEFFECT_SPOT.h"

#define BOOM_FXSOUND1	50001470
#define BOOM_FXSOUND2	50001471

//YS_0728 : BUGFIX
int OnCS_NC_NPCINFO_ACK(CMsg &msg)
{
	BYTE	bResult		=	0;	

	msg
		>> bResult;

	if(bResult != 0)
		return TRUE;

	DWORD	dwObjectID	=	0;	
	DWORD	dwMapID		=	0;
	WORD	wPosX		=	0;
	WORD	wPosY		=	0;
	WORD	wDirection	=	0;
	WORD	wDesPosX	=	0;
	WORD	wDesPosY	=	0;
	DWORD	dwHpMax		=	0;
	DWORD	dwHpCur		=	0;
	BYTE	bNpcType	=	0;
	BYTE	bHeight		=	0;	
	BYTE	bState		=	0;
	BYTE	bDesHeight	=	0;	
	BYTE	bWalkSpeed	=	0;
	BYTE	bOrderID	=	0;
	// NPC들의 TYPE을 결정 0,1,2 -> Quest용(이것은 Mesh Type과 같다),  NPC 255 -> 일반
	BYTE	bSubType	=	0;
	WORD	wLevel		=0;

	sString szName;

	msg
		>> dwObjectID
		>> bNpcType
		>> dwMapID
		>> wPosX
		>> wLevel
		>> wPosY
		>> bHeight
		>> wDirection
		>> szName
		>> bState
		>> wDesPosX
		>> wDesPosY
		>> bDesHeight
		>> dwHpMax
		>> dwHpCur
		>> bWalkSpeed
		>> bOrderID
		>> bSubType;

	if(dwMapID > 99 || wPosX < 0 || wPosX > 2047 || wPosY < 0 || wPosY > 2047) 
	{
		DBG_LogFile("Invalid Position in OnCS_NC_NPCINFO_ACK");
		return TRUE;
	}

	if( dwObjectID == 0)
		return TRUE;

	BYTE bHi_Subtype_Old = 0xf0;
	BYTE bLo_Subtype_Old = 0x0f;

	BYTE bHi_Subtype = (bSubType & 0xf0) >> 4;		// Mesh
	BYTE bLo_Subtype = (bSubType & 0x0f);			// Effect

	XiahObject::CXiahObject* pXiahObject = XiahObject::g_XiahObjectManager.FindXiahObject( MAKEOBJECTID( 0, dwObjectID,OBJTYPE_NPC) );

	// 새로운 캐릭터면 첨부터 생성하지만, 존재하는 거라면 데이타를 바꿔준다.
	bool bCreateChar = true;

	if( pXiahObject != NULL ) // 캐릭터 데이타가 있으면 new 하지 않고 데이타만 바꿔준다.
		bCreateChar = false;

	sArrayData *pData = XiahArrayIndex::g_NpcType.GetData( bNpcType);
	if( pData == NULL) return TRUE;

	int nCharID;
	int nMeshType;
	int nTextureType;		

	// Create Character
	//YS_0805 : BUGFIX
	CXiahCharObject* pObject = NULL;
	
	if( bCreateChar )
	{
		pObject = (CXiahCharObject*)XiahObject::g_XiahNpcPool.GetChar(); //new CXiahCharObject;
		if( NULL == pObject )
		{
            pObject = new CXiahCharObject;
		}
	}
	else
	{
		pObject = reinterpret_cast<CXiahCharObject*>(pXiahObject->m_pObject);

		//YS_0811 : BUGFIX
		if ( !pObject )
			return TRUE;
	}

	if( bCreateChar )
		XiahObject::g_XiahObjectManager.CreateXiahObject( dwObjectID, OBJTYPE_NPC, pObject);
	else
	{
		// 기존의 정보
		bHi_Subtype_Old = (pObject->m_bExSubObjType & 0xf0) >> 4;
		bLo_Subtype_Old = (pObject->m_bExSubObjType & 0x0f);
	}

	if(bSubType == 0xff)
	{
		// 일반 NPC일경우
		nCharID		 = pData->GetInt( 1);
		nMeshType	 = pData->GetInt( 2);
		nTextureType = pData->GetInt( 3);
	}
	else if(bHi_Subtype == 0 || bHi_Subtype == 1 || bHi_Subtype == 2)
	{
		// 파괴 퀘스트용 NPC -> Mesh를 바꾼다
		nCharID		 = pData->GetInt( 1);
		nMeshType	 = bHi_Subtype;
		nTextureType = pData->GetInt( 3);
	}

	if( XiahGameEngine::GetCharacter( nCharID) == NULL)
		return TRUE;

	CRes_Character* pResChar = XiahGameEngine::GetCharacter( nCharID);

	if( pResChar == NULL)
		return TRUE;
	if( !pResChar->GetMesh( nMeshType))
		return TRUE;
	if( pResChar->GetMesh( nMeshType)->GetTexture( nTextureType) == NULL)
		return TRUE;

	pObject->Create( nCharID, nMeshType, nTextureType, 1);
	
	pObject->m_bOrderID = bOrderID;

	if(11 == dwMapID)	// 던전에서만
	{
		switch(nCharID)
		{
			case 816:	// 빙요
				pObject->m_CharRender.SetLocalScale(Vector3(1.5f, 1.5f, 1.5f));
				break;
			case 902:	// 해신
				pObject->m_CharRender.SetLocalScale(Vector3(1.5f, 1.5f, 1.5f));
				break;
			case 942:	// 석귀
				pObject->m_CharRender.SetLocalScale(Vector3(1.5f, 1.5f, 1.5f));
				break;
			case 1041:	// 적호
				pObject->m_CharRender.SetLocalScale(Vector3(1.5f, 1.5f, 1.5f));
				break;
			case 1042:	// 금각동인
				pObject->m_CharRender.SetLocalScale(Vector3(1.5f, 1.5f, 1.5f));
				break;
			case 1040:	// 인면수
				pObject->m_CharRender.SetLocalScale(Vector3(1.5f, 1.5f, 1.5f));
				break;
			case 1043:	// 혈기린
				pObject->m_CharRender.SetLocalScale(Vector3(1.55f, 1.55f, 1.55f));
				break;
			case 1044:	// 금각거인
				pObject->m_CharRender.SetLocalScale(Vector3(1.15f, 1.15f, 1.15f));
				break;
			case 1045:	// 교룡
				pObject->m_CharRender.SetLocalScale(Vector3(1.5f, 1.5f, 1.5f));
				break;
			case 1039:	// 대조귀
				pObject->m_CharRender.SetLocalScale(Vector3(0.85f, 0.85f, 0.85f));
				break;
			case 1084:	// 후
				pObject->m_CharRender.SetLocalScale(Vector3(1.5f, 1.5f, 1.5f));
				break;
		}

		switch(bOrderID)
		{
			case 1:	// 보스
				pObject->m_CharRender.SetLocalScale( Vector3( 1.7f, 1.7f, 1.7f));
				break;
			case 2:	// 행동대장
				pObject->m_CharRender.SetLocalScale( Vector3( 1.6f, 1.6f, 1.6f));
				break;
			//case 3:	// 노련한
			//	pObject->m_CharRender.SetLocalScale( Vector3( 0.8f, 0.9f, 0.8f));
			//	break;
		}
	}
	else if(dwMapID == 12)	// 마혈성
	{
		//pObject->m_CharRender.SetLocalScale(Vector3(2.0f, 2.0f, 2.0f)); //HO_0614_07 자객추가 : 추가전 코드
		switch(nCharID) //HO_0614_07 자객추가
		{
			case 1145:	// 자객
				pObject->m_CharRender.SetLocalScale(Vector3(1.5f, 1.5f, 1.5f));
				break;
			default:
				pObject->m_CharRender.SetLocalScale(Vector3(2.0f, 2.0f, 2.0f));
				break;
		}
	}
	else if(dwMapID == 13)	// 광명전
	{
		switch(nCharID)
		{
			case 1084:	//광명좌사, 광명우사
				pObject->m_CharRender.SetLocalScale(Vector3(1.5f, 1.5f, 1.5f));
				break;
			case 942:	//석귀왕, 토의수호자
				pObject->m_CharRender.SetLocalScale(Vector3(1.2f, 1.2f, 1.2f));
				break;
			case 1131:	//금각왕, 금의수호자
				pObject->m_CharRender.SetLocalScale(Vector3(1.3f, 1.3f, 1.3f));
				break;
			case 935:	//야곤왕, 목의수호자				
			case 936:	//진모왕, 수의수호자				
			case 938:	//표인왕, 화의수호자
				pObject->m_CharRender.SetLocalScale(Vector3(1.5f, 1.5f, 1.5f));
				break;
		}
	}
	else if(dwMapID == 14)	// 천황전
	{
		switch(nCharID)
		{
			case 1144:	//4대천왕
				pObject->m_CharRender.SetLocalScale(Vector3(1.8f, 1.8f, 1.8f));
				break;
			case 1143:	// 마혈천황
				pObject->m_CharRender.SetLocalScale(Vector3(2.2f, 2.2f, 2.2f));
				break;
			case 942:	//석귀왕, 토의수호자
				pObject->m_CharRender.SetLocalScale(Vector3(1.2f, 1.2f, 1.2f));
				break;
			case 1131:	//금각왕, 금의수호자
				pObject->m_CharRender.SetLocalScale(Vector3(1.3f, 1.3f, 1.3f));
				break;
			case 935:	//야곤왕, 목의수호자				
			case 936:	//진모왕, 수의수호자				
			case 938:	//표인왕, 화의수호자 
				pObject->m_CharRender.SetLocalScale(Vector3(1.5f, 1.5f, 1.5f));
				break;
		}
	}
	else
	{
		// [3/25/2005] ㅎㅎ 또다 또 알아서 해라. 초기 스케일 테스트 없이 모델링하고.. 코드로 스케일 조절 해달라니
		switch(nCharID)
		{
		case 1084:	// [1/4/2005] 후 수정 - 패키징 다하고 올리고 나니 또 저런다. 췌,
			{
				switch(bOrderID)
				{
					//case 0:		// 보스
					//	pObject->m_CharRender.SetLocalScale(Vector3(1.7f, 1.7f, 1.7f));
					//	break;
					case 1:		// 행동대장
						pObject->m_CharRender.SetLocalScale(Vector3(1.7f, 1.7f, 1.7f));						
						break;
					case 2:		// 노련한
						pObject->m_CharRender.SetLocalScale(Vector3(1.6f, 1.6f, 1.6f));						
						break;
					default:	// 일반 타입
						pObject->m_CharRender.SetLocalScale(Vector3(1.5f, 1.5f, 1.5f));
						break;
				}
			}
			break;
		case 1097:	// 비강
			{
				switch(bOrderID)
				{
					//case 0:		// 보스
					//	pObject->m_CharRender.SetLocalScale(Vector3(0.9f, 0.9f, 0.9f));
					//	break;
					case 1:		// 행동대장
						pObject->m_CharRender.SetLocalScale(Vector3(0.9f, 0.9f, 0.9f));												
						break;
					case 2:		// 노련한
						pObject->m_CharRender.SetLocalScale(Vector3(0.8f, 0.8f, 0.8f));						
						break;
					default:	// 일반 타입
						pObject->m_CharRender.SetLocalScale(Vector3(0.7f, 0.7f, 0.7f));
						break;
				}
			}
			break;
		case 1098:	// 원신수
			{
				switch(bOrderID)
				{
					//case 0:		// 보스
					//	pObject->m_CharRender.SetLocalScale(Vector3(1.0f, 1.0f, 1.0f));
					//	break;
					case 1:		// 행동대장
						pObject->m_CharRender.SetLocalScale(Vector3(1.0f, 1.0f, 1.0f));						
						break;
					case 2:		// 노련한
						pObject->m_CharRender.SetLocalScale(Vector3(0.9f, 0.9f, 0.9f));						
						break;
					default:	// 일반 타입
						pObject->m_CharRender.SetLocalScale(Vector3(0.8f, 0.8f, 0.8f));
						break;
				}
			}
			break;
		case 1101:	// 사도는 그냥 통과
			break;
		default:
			{
				switch(bOrderID)
				{
					case 1:	// 보스
						pObject->m_CharRender.SetLocalScale( Vector3( 1.4f, 1.4f, 1.4f));
						break;
					case 2:	// 행동대장
						pObject->m_CharRender.SetLocalScale( Vector3( 1.2f, 1.2f, 1.2f));
						break;
					//case 3:	// 노련한
					//	pObject->m_CharRender.SetLocalScale( Vector3( 0.8f, 0.9f, 0.8f));
					//	break;
				}
			}
			break;
		}
	}

	////HT_0810 : 복돌이 크기 
	//if(bNpcType == 155 || bNpcType == 156)
	//{
	//	pObject->m_CharRender.SetLocalScale(Vector3(2.0f, 2.0f, 2.0f));
	//}

	////HT_1214 : 봉황 이벤트 
	//if(bNpcType == 12 || bNpcType == 13)
	//{
	//	pObject->m_CharRender.SetLocalScale(Vector3(0.5f, 0.5f, 0.5f));
	//}

	//HT_0112 : 복돼지 이벤트 
	//if(bNpcType == 14)// || bNpcType == 19)
	//{
	//	pObject->m_CharRender.SetLocalScale(Vector3(2.5f, 2.5f, 2.5f));
	//}

	pObject->m_pAniType = XiahAniType::GetAniType( OBJTYPE_NPC, 0);

	if(bCreateChar == true)
	{
		pObject->SetAngle( wDirection);
		pObject->SetPosition( wPosX, wPosY);
	}
	else
	{
		pObject->SetAnimation( XiahAniType::eLAT_Stand, -1);
	}

	pObject->m_szObjectName  = szName;
	pObject->m_bObjType		 = OBJTYPE_NPC;
	pObject->m_bSubObjType	 = bNpcType;
	pObject->m_bExSubObjType = bSubType;	// Extra subtype
	pObject->m_bObjStatus	 = bState;

	pObject->m_dwCurHP		 = dwHpCur;
	pObject->m_dwMaxHP		 = dwHpMax;

	pObject->m_wLevel		 = wLevel;

	// 퀘스트용 NPC는 ROTATE 불가
	if(bSubType != 0xff)
	{
		pObject->m_bRotatable = FALSE;
	}

	// 사도 소환 몹은 숨어 있음.
	_EFFECTPACKAGE* pPackage = NULL;
	if(NPCSTATUS_HIDE != bState)
	{
		// 캐릭터가 등장할때 이펙트
		Vector3 vSize = pObject->m_LocalBound.Size();
		vSize.y = 0;
		float fSize = vSize.GetLength();

		if( fSize >= 10.0f )
		{
			pPackage = g_EffectManager.EnqAppearEffectImmediately( eBig );
		}
		else if( fSize < 10.0f && fSize >= 5.0f )
		{
			pPackage = g_EffectManager.EnqAppearEffectImmediately( eMiddle );
		}
		else
		{
			pPackage = g_EffectManager.EnqAppearEffectImmediately( eSmall );
		}
	}	

	// 캐릭터가 Mesh 이펙트가 있으면
	if(bHi_Subtype == 0 || bHi_Subtype == 1 || bHi_Subtype == 2 && 3 != bSubType)	// Quest NPC
		pObject->m_CharRender.MakeMeshEffect( bLo_Subtype );
	else	// General
		pObject->m_CharRender.MakeMeshEffect();

	pObject->Update();
	//pObject->UpdateTM();

	if( pPackage && pPackage->pEffectRender && pPackage->pEffectRender->pPackagePair )
	{
		pPackage->pEffectRender->pPackagePair->WorldMatrix = (MATRIX)pObject->m_ObjectTM;

		pObject->m_EffectPPList.push_back( pPackage->pEffectRender->pPackagePair );
	}

	return TRUE;
}




//---------------------------------------------------------------------------------------
//YS_0728 : BUGFIX
int OnCS_NC_NPCINFOLIST_ACK(CMsg &msg)
{
	BYTE  bResult		= 0;

	msg
		>> bResult;

	if ( bResult != 0 )
		return TRUE;

	DWORD dwMapID		= 0;
	WORD  wObjectNum	= 0;

	msg
		>> dwMapID
		>> wObjectNum;

	if ( dwMapID > 99 || wObjectNum > 300 )
		return TRUE;

	for(int i = 0; i < wObjectNum; ++i)
	{
		DWORD	dwObjectID	=	0;
		BYTE	bNpcType	=	0;		
		BYTE	bHeight		=	0;
		BYTE	bState		=	0;
		BYTE	bDesHeight	=	0;
		BYTE	bWalkSpeed	=	0;	
		BYTE	bOrderID	=	0;
		WORD	wPosX		=	0;
		WORD	wPosY		=	0;
		WORD	wDirection	=	0;		
		WORD	wDesPosX	=	0;
		WORD	wDesPosY	=	0;		
		DWORD	dwHpMax		=	0;
		DWORD	dwHpCur		=	0;		
		
		// NPC들의 TYPE을 결정 0,1,2 -> Quest용(이것은 Mesh Type과 같다),  NPC 255 -> 일반
		BYTE	bSubType	=	0;
		WORD	wLevel		=0;

		sString szName;

		msg
			>> dwObjectID
			>> bNpcType
			>> wPosX
			>> wLevel
			>> wPosY
			>> bHeight
			>> wDirection
			>> szName
			>> bState
			>> wDesPosX
			>> wDesPosY
			>> bDesHeight
			>> dwHpMax
			>> dwHpCur
			>> bWalkSpeed
			>> bOrderID
			>> bSubType;

		if( dwObjectID == 0 || wPosX > 2047 || wPosY > 2047 )
			continue;

		BYTE	bHi_Subtype,bLo_Subtype			=	0;
		BYTE	bHi_Subtype_Old,bLo_Subtype_Old	=	0;

		// 새로운 캐릭터면 첨부터 생성하지만, 존재하는 거라면 데이타를 바꿔준다.
		bool bCreateChar = true;

		XiahObject::CXiahObject* pXiahObject = XiahObject::g_XiahObjectManager.FindXiahObject( MAKEOBJECTID( 0, dwObjectID,OBJTYPE_NPC) );

		if( pXiahObject != NULL ) // 캐릭터 데이타가 있으면 new 하지 않고 데이타만 바꿔준다.
			bCreateChar = false;

		sArrayData *pData = XiahArrayIndex::g_NpcType.GetData( bNpcType);
		if( pData == NULL)	continue;

		// Create Character
		CXiahCharObject* pObject = NULL;
		if( bCreateChar )
		{
			pObject = (CXiahCharObject *)XiahObject::g_XiahNpcPool.GetChar();//new CXiahCharObject;
			if ( NULL == pObject )
			{
				pObject = new CXiahCharObject;
			}
		}
		else
		{
			pObject = reinterpret_cast<CXiahCharObject*>(pXiahObject->m_pObject);
		}

		if(pObject == NULL)
		{
			continue;
		}

		if( bCreateChar )
		{
			XiahObject::g_XiahObjectManager.CreateXiahObject( dwObjectID, OBJTYPE_NPC, pObject);
		}
		else
		{
			// 기존의 정보
			bHi_Subtype_Old = (pObject->m_bExSubObjType & 0xf0) >> 4;
			bLo_Subtype_Old = (pObject->m_bExSubObjType & 0x0f);
		}

		bHi_Subtype = (bSubType & 0xf0) >> 4;
		bLo_Subtype = (bSubType & 0x0f);

		int nCharID		=0;
		int nMeshType	=0;
		int nTextureType=0;

		if(bSubType == 255)
		{
			// 일반 NPC일경우
			nCharID = pData->GetInt( 1);
			nMeshType = pData->GetInt( 2);
			nTextureType = pData->GetInt( 3);

		}
		else if(bHi_Subtype == 0 || bHi_Subtype == 1 || bHi_Subtype == 2)
		{
			// 파괴 퀘스트용 NPC -> Mesh를 바꾼다
			nCharID = pData->GetInt( 1);
			nMeshType = bHi_Subtype;
			nTextureType = pData->GetInt( 3);
		}

		//
		CRes_Character* pResChar = XiahGameEngine::GetCharacter( nCharID);

		if( pResChar == NULL)
			continue;
		if( !pResChar->GetMesh( nMeshType))
			continue;
		if( pResChar->GetMesh( nMeshType)->GetTexture( nTextureType) == NULL)
			continue;

		if(bSubType == 255)
		{
			// normal NPC CREATE
			pObject->Create( nCharID, nMeshType, nTextureType, 1);
		}
		else
		{
			// 파괴 BOX 오브젝트 (이건 ANI 없어서 -1 로 설정)
			pObject->Create( nCharID, nMeshType, nTextureType, -1);
		}

		pObject->m_bOrderID = bOrderID;

		if(11 == dwMapID)	// 던전에서만
		{
			switch(nCharID)
			{
				case 816:	// 빙요
					pObject->m_CharRender.SetLocalScale(Vector3(1.5f, 1.5f, 1.5f));
					break;
				case 902:	// 해신
					pObject->m_CharRender.SetLocalScale(Vector3(1.5f, 1.5f, 1.5f));
					break;
				case 942:	// 석귀
					pObject->m_CharRender.SetLocalScale(Vector3(1.5f, 1.5f, 1.5f));
					break;
				case 1041:	// 적호
					pObject->m_CharRender.SetLocalScale(Vector3(1.5f, 1.5f, 1.5f));
					break;
				case 1042:	// 금각동인
					pObject->m_CharRender.SetLocalScale(Vector3(1.5f, 1.5f, 1.5f));
					break;
				case 1040:	// 인면수
					pObject->m_CharRender.SetLocalScale(Vector3(1.5f, 1.5f, 1.5f));
					break;
				case 1043:	// 혈기린
					pObject->m_CharRender.SetLocalScale(Vector3(1.55f, 1.55f, 1.55f));
					break;
				case 1044:	// 금각거인
					pObject->m_CharRender.SetLocalScale(Vector3(1.15f, 1.15f, 1.15f));
					break;
				case 1045:	// 교룡
					pObject->m_CharRender.SetLocalScale(Vector3(1.5f, 1.5f, 1.5f));
					break;
				case 1039:	// 대조귀
					pObject->m_CharRender.SetLocalScale(Vector3(0.85f, 0.85f, 0.85f));
					break;
				case 1084:	// 후
					pObject->m_CharRender.SetLocalScale(Vector3(1.5f, 1.5f, 1.5f));
					break;
			}

			switch(bOrderID)
			{
				case 1:	// 보스
					pObject->m_CharRender.SetLocalScale( Vector3( 1.7f, 1.7f, 1.7f));
					break;
				case 2:	// 행동대장
					pObject->m_CharRender.SetLocalScale( Vector3( 1.6f, 1.6f, 1.6f));
					break;
				//case 3:	// 노련한
				//	pObject->m_CharRender.SetLocalScale( Vector3( 0.8f, 0.9f, 0.8f));
				//	break;
			}
		}
		else if(dwMapID == 12)	// 마혈성
		{
			//pObject->m_CharRender.SetLocalScale(Vector3(2.0f, 2.0f, 2.0f)); //HO_0614_07 자객추가 : 추가전 코드
			switch(nCharID)	//HO_0614_07 자객추가
			{
				case 1145:	// 자객
					pObject->m_CharRender.SetLocalScale(Vector3(1.5f, 1.5f, 1.5f));
					break;
				default:
					pObject->m_CharRender.SetLocalScale(Vector3(2.0f, 2.0f, 2.0f));
					break;
			}
		}
		else if(dwMapID == 13)	// 광명전
		{
			switch(nCharID)
			{
			case 1084:	//광명좌사, 광명우사
				pObject->m_CharRender.SetLocalScale(Vector3(1.5f, 1.5f, 1.5f));
				break;
			case 942:	//석귀왕, 토의수호자
				pObject->m_CharRender.SetLocalScale(Vector3(1.2f, 1.2f, 1.2f));
				break;
			case 1131:	//금각왕, 금의수호자
				pObject->m_CharRender.SetLocalScale(Vector3(1.3f, 1.3f, 1.3f));
				break;
			case 935:	//야곤왕, 목의수호자				
			case 936:	//진모왕, 수의수호자				
			case 938:	//표인왕, 화의수호자
				pObject->m_CharRender.SetLocalScale(Vector3(1.5f, 1.5f, 1.5f));
				break;
			}
		}
		else if(dwMapID == 14)	// 천황전
		{
			switch(nCharID)
			{
			case 1144:	//4대천왕
				pObject->m_CharRender.SetLocalScale(Vector3(1.8f, 1.8f, 1.8f));
				break;
			case 1143:	// 마혈천황
				pObject->m_CharRender.SetLocalScale(Vector3(2.2f, 2.2f, 2.2f));
				break;
			case 942:	//석귀왕, 토의수호자
				pObject->m_CharRender.SetLocalScale(Vector3(1.2f, 1.2f, 1.2f));
				break;
			case 1131:	//금각왕, 금의수호자
				pObject->m_CharRender.SetLocalScale(Vector3(1.3f, 1.3f, 1.3f));
				break;
			case 935:	//야곤왕, 목의수호자				
			case 936:	//진모왕, 수의수호자				
			case 938:	//표인왕, 화의수호자 
				pObject->m_CharRender.SetLocalScale(Vector3(1.5f, 1.5f, 1.5f));
				break;
			}
		}
		else
		{
			// [3/25/2005] ㅎㅎㅎ 또다 또 알아서 해라
			switch(nCharID)
			{
			case 1084:	// [1/4/2005] 후 수정 - 패키징 다하고 올리고 나니 또 저런다. 췌,
				{
					switch(bOrderID)
					{
						//case 0:		// 보스
						//	pObject->m_CharRender.SetLocalScale(Vector3(1.7f, 1.7f, 1.7f));
						//	break;
						case 1:		// 행동대장
							pObject->m_CharRender.SetLocalScale(Vector3(1.7f, 1.7f, 1.7f));
							break;
						case 2:		// 노련한
							pObject->m_CharRender.SetLocalScale(Vector3(1.6f, 1.6f, 1.6f));
							break;
						default:	// 일반 타입
							pObject->m_CharRender.SetLocalScale(Vector3(1.5f, 1.5f, 1.5f));
							break;
					}
				}
				break;
			case 1097:	// 비강
				{
					switch(bOrderID)
					{
						//case 0:		// 보스
						//	pObject->m_CharRender.SetLocalScale(Vector3(0.9f, 0.9f, 0.9f));
						//	break;
						case 1:		// 행동대장
							pObject->m_CharRender.SetLocalScale(Vector3(0.9f, 0.9f, 0.9f));
							break;
						case 2:		// 노련한
							pObject->m_CharRender.SetLocalScale(Vector3(0.8f, 0.8f, 0.8f));
							break;
						default:	// 일반 타입
							pObject->m_CharRender.SetLocalScale(Vector3(0.7f, 0.7f, 0.7f));
							break;
					}
				}
				break;
			case 1098:	// 원신수
				{
					switch(bOrderID)
					{
						//case 0:		// 보스
						//	pObject->m_CharRender.SetLocalScale(Vector3(1.0f, 1.0f, 1.0f));
						//	break;
						case 1:		// 행동대장
							pObject->m_CharRender.SetLocalScale(Vector3(1.0f, 1.0f, 1.0f));							
							break;
						case 2:		// 노련한
							pObject->m_CharRender.SetLocalScale(Vector3(0.9f, 0.9f, 0.9f));							
							break;
						default:	// 일반 타입
							pObject->m_CharRender.SetLocalScale(Vector3(0.8f, 0.8f, 0.8f));
							break;
					}
				}
				break;
			case 1101:	// 사도는 그냥 통과
				break;
			default:
				{
					switch(bOrderID)
					{
						case 1:	// 보스
							pObject->m_CharRender.SetLocalScale( Vector3( 1.4f, 1.4f, 1.4f));
							break;
						case 2:	// 행동대장
							pObject->m_CharRender.SetLocalScale( Vector3( 1.2f, 1.2f, 1.2f));
							break;
						//case 3:	// 노련한
						//	pObject->m_CharRender.SetLocalScale( Vector3( 0.8f, 0.9f, 0.8f));
						//	break;
					}
				}
				break;
			}
		}

		////HT_0810 : 복돌이 크기 
		//if(bNpcType == 155 || bNpcType == 156)
		//{
		//	pObject->m_CharRender.SetLocalScale(Vector3(2.0f, 2.0f, 2.0f));
		//}
	
		////HT_1214 : 봉황 이벤트 
		//if(bNpcType == 12 || bNpcType == 13)
		//{
		//	pObject->m_CharRender.SetLocalScale(Vector3(0.5f, 0.5f, 0.5f));
		//}

		//HT_0112 : 복돼지 이벤트 
		if(bNpcType == 14)// || bNpcType == 19)
		{
			pObject->m_CharRender.SetLocalScale(Vector3(2.5f, 2.5f, 2.5f));
		}

		pObject->m_pAniType = XiahAniType::GetAniType( OBJTYPE_NPC, 0);

		if(bCreateChar == true)
		{
			// 이미 있는거면 위치는 SKIP
			pObject->SetAngle( wDirection);
			pObject->SetPosition( wPosX, wPosY);
		}
		else
		{
			pObject->SetAnimation( XiahAniType::eLAT_Stand, -1);
		}

		pObject->m_szObjectName = szName;
		pObject->m_bObjType		= OBJTYPE_NPC;
		pObject->m_bSubObjType	= bNpcType;
		pObject->m_bExSubObjType= bSubType;	// Extra subtype

		pObject->m_dwCurHP	  = dwHpCur;
		pObject->m_dwMaxHP	  = dwHpMax;
		pObject->m_bObjStatus = bState;

		pObject->m_wLevel		= wLevel;

		// 새로만들어 지는 거만 이펙트
		if( bCreateChar && 3 != bSubType)
		{
			// [4/12/2005] 사도 소환 몹은 숨어 있음.
			_EFFECTPACKAGE* pPackage = NULL;
			if(NPCSTATUS_HIDE != bState)
			{
				// 캐릭터가 등장할때 이펙트
				Vector3 vSize = pObject->m_LocalBound.Size();
				vSize.y = 0;
				float fSize = vSize.GetLength();

				if( fSize >= 10.0f )
				{
					pPackage = g_EffectManager.EnqAppearEffectImmediately( eBig );
				}
				else if( fSize < 10.0f && fSize >= 5.0f )
				{
					pPackage = g_EffectManager.EnqAppearEffectImmediately( eMiddle );
				}
				else
				{
					pPackage = g_EffectManager.EnqAppearEffectImmediately( eSmall );
				}
			}			

			// 퀘스트용 NPC는 ROTATE 불가
			if(bSubType != 255)
			{
				pObject->m_bRotatable = FALSE;
			}

			// 캐릭터가 Mesh 이펙트가 있으면
			if(bHi_Subtype == 0 || bHi_Subtype == 1 || bHi_Subtype == 2)	// Quest NPC
				pObject->m_CharRender.MakeMeshEffect( bLo_Subtype );
			else	// General
				pObject->m_CharRender.MakeMeshEffect();

			pObject->Update();
			//pObject->UpdateTM();

			if( pPackage && pPackage->pEffectRender && pPackage->pEffectRender->pPackagePair )
			{
				pPackage->pEffectRender->pPackagePair->WorldMatrix = (MATRIX)pObject->m_ObjectTM;
				pObject->m_EffectPPList.push_back( pPackage->pEffectRender->pPackagePair );
			}
		}
		else
		{
			// 캐릭터가 Mesh 이펙트가 있으면
			if((bHi_Subtype == 0 || bHi_Subtype == 1 || bHi_Subtype == 2) && 3 != bSubType)	// Quest NPC
				pObject->m_CharRender.MakeMeshEffect( bLo_Subtype );
			else	// General
				pObject->m_CharRender.MakeMeshEffect();
		}
	}

	DBG_Put(_T("CS_NC_NPCINFOLIST_ACK"));

	return TRUE;
}




//---------------------------------------------------------------------------------------
//YS_0728 : BUGFIX
int OnCS_NC_FUNCTIONALNPCINFOLIST_ACK(CMsg &msg)
{
	BYTE	bResult		=	0;	
	
	msg
		>> bResult;

	if ( bResult != 0 )
		return TRUE;
	
	DWORD	dwMapID		=	0;
	WORD	wNumObject	=	0;

	msg
		>> dwMapID
		>> wNumObject;


	for(int i=0; i < wNumObject; ++i) 
	{
		WORD	wPosX		=	0;
		WORD	wPosY		=	0;		
		WORD	wDirection	=	0;
		WORD	wNumItem	=	0;
		DWORD	dwObjectID	=	0;
		DWORD	dwOwnID		=	0;
		BYTE	bHeight		=	0;
		BYTE	bType		=	0;
		BYTE	bKind		=	0;
		BYTE	bCanShare	=	0;		
		BYTE	bOwnType	=	0;		
		BYTE	bSizeX		=	0;
		BYTE	bSizeY		=	0;		

		sString szName;
		
		msg
			>> dwObjectID
			>> bType
			>> bKind
			>> bCanShare
			>> wPosX
			>> wPosY
			>> bHeight
			>> wDirection
			>> szName
			>> bOwnType
			>> dwOwnID
			>> bSizeX
			>> bSizeY
			>> wNumItem;
	
		if( XiahObject::g_XiahObjectManager.FindXiahObject( MAKEOBJECTID( 0, dwObjectID,OBJTYPE_FUNCTIONALNPC)) != NULL)
		{
			continue;
		}

		sArrayData *pData = XiahArrayIndex::g_FunctionalNpcType.GetData( bType);

		if( pData == NULL)
		{
			continue;
		}
		
		int nCharID		= pData->GetInt( 1);
		int nMeshType	= pData->GetInt( 2);
		int nTextureType= pData->GetInt( 3);

		if( XiahGameEngine::GetCharacter( nCharID) == NULL)
		{
			continue;
		}

		CXiahCharObject *pObject = new CXiahCharObject;
		if(pObject == NULL)
		{
			continue;
		}

		XiahObject::g_XiahObjectManager.CreateXiahObject( dwObjectID, OBJTYPE_FUNCTIONALNPC, pObject);

		//DBG_LogFile("FUNC NPC : %d %d %s",wPosX,wPosY,szName.data());		

		pObject->Create( nCharID, nMeshType, nTextureType, 1);

		// [1/24/2005] 상서령 키워달란다. 이렇게 하드코딩이면 끝도 없게 된다. 사전 스케일 테스트좀 해다오. 말해도 다음에도 똑같은...
		if(1091 == nCharID)
		{
			pObject->m_CharRender.SetLocalScale(Vector3(1.16f, 1.16f, 1.16f));
		}
		
		//HT_0313 : 수문장 수호금각귀
		if(1142 == nCharID)
		{
		//	pObject->m_CharRender.SetLocalScale(Vector3(0.8f, 0.8f, 0.8f));
		}

		// [10/29/2004] 이펙트 설정 (연금술사/도우미)
		int nIndex[2] = {0,1};
		pObject->m_CharRender.MakeMeshEffect(2, nIndex);

		pObject->m_pAniType = XiahAniType::GetAniType( OBJTYPE_FUNCTIONALNPC, 0);

		pObject->SetAngle( wDirection);
		pObject->SetPosition( wPosX, wPosY);

		
		pObject->SetAnimation( XiahAniType::eLAT_Stand, -1);

		pObject->m_szObjectName = szName;
		pObject->m_bObjType		= OBJTYPE_FUNCTIONALNPC;
		pObject->m_bSubObjType	= bType;
		
		sFunctionalNpcInfo* pInfo = new sFunctionalNpcInfo;
		pInfo->m_dwObjectID = dwObjectID;
		pInfo->m_bType		= bType;
		pInfo->m_bKind		= bKind;
		pInfo->m_szName		= szName;
		pInfo->m_bOwnType	= bOwnType;
		pInfo->m_dwOwnID	= dwOwnID;
		pInfo->m_bSizeX		= bSizeX;
		pInfo->m_bSizeY		= bSizeY;
		pInfo->m_wNumItem	= wNumItem;

		pObject->m_pPrivateData = (DWORD)pInfo;
		pObject->m_PrivateDataDestoryer = ReleaseFunctionalNpcInfo; // 앗싸
	}

	return TRUE;
}
//---------------------------------------------------------------------------------------
//YS_0728 : BUGFIX
int OnCS_NC_FUNCTIONALNPCINFO_ACK(CMsg &msg)
{
	BYTE bResult =0;	

	msg
		>> bResult;

	if ( bResult != 0 )
		return TRUE;

	if(bResult == 0) 
	{
		BYTE	bHeight		=	0;	
		BYTE	bType		=	0;
		BYTE	bKind		=	0;
		BYTE	bCanShare	=	0;
		BYTE	bSizeX		=	0;
		BYTE	bSizeY		=	0;
		BYTE	bOwnType	=	0;
		WORD	wNumItem	=	0;
		WORD	wPosX		=	0;
		WORD	wPosY		=	0;	
		WORD	wDirection	=	0;
		DWORD	dwMapID		=	0;
		DWORD	dwObjectID	=	0;	
		DWORD	dwOwnID		=	0;

		sString szName;


		msg
			>> dwObjectID
			>> bType
			>> bKind
			>> bCanShare
			>> dwMapID
			>> wPosX
			>> wPosY
			>> bHeight
			>> wDirection
			>> szName
			>> bOwnType
			>> dwOwnID
			>> bSizeX
			>> bSizeY
			>> wNumItem;
	
		if ( dwMapID > 99 || wPosX > 2047 || wPosY > 2047 )
			return TRUE;

		if( XiahObject::g_XiahObjectManager.FindXiahObject( MAKEOBJECTID( 0, dwObjectID,OBJTYPE_FUNCTIONALNPC)) != NULL)
			return TRUE;

		sArrayData *pData = XiahArrayIndex::g_FunctionalNpcType.GetData( bType);

		if( pData == NULL)
		{
			return TRUE;
		}
		
		int nCharID		= pData->GetInt( 1);
		int nMeshType	= pData->GetInt( 2);
		int nTextureType= pData->GetInt( 3);

		CRes_Character* pResChar = XiahGameEngine::GetCharacter( nCharID);

		if( pResChar == NULL)
		{
			DBG_LogFile("OnCS_NC_FUNCTIONALNPCINFO_ACK1");
			return TRUE;
		}
		
		if( !pResChar->GetMesh( nMeshType))
		{
			DBG_LogFile("OnCS_NC_FUNCTIONALNPCINFO_ACK2");
			return TRUE;
		}

		if( pResChar->GetMesh( nMeshType)->GetTexture( nTextureType) == NULL)
		{
			DBG_LogFile("OnCS_NC_FUNCTIONALNPCINFO_ACK1");
			return TRUE;
		}

		CXiahCharObject *pObject = new CXiahCharObject;
		XiahObject::g_XiahObjectManager.CreateXiahObject( dwObjectID, OBJTYPE_FUNCTIONALNPC, pObject);

		pObject->Create( nCharID, nMeshType, nTextureType, 1);
		// [1/24/2005] 상서령 키워달란다. 이렇게 하드코딩이면 끝도 없게 된다. 사전 스케일 테스트좀 해다오. 말해도 다음에도 똑같은...
		if(1091 == nCharID)
		{
			pObject->m_CharRender.SetLocalScale(Vector3(1.16f, 1.16f, 1.16f));
		}

		//HT_0313 : 수문장 수호금각귀
		if(1142 == nCharID)
		{
		//	pObject->m_CharRender.SetLocalScale(Vector3(0.8f, 0.8f, 0.8f));
		}

		// [10/29/2004] 이펙트 설정 (연금술사/도우미)
		int nIndex[2] = {0,1};
		pObject->m_CharRender.MakeMeshEffect(2, nIndex);

		pObject->m_pAniType = XiahAniType::GetAniType( OBJTYPE_FUNCTIONALNPC, 0);

		pObject->SetAngle( wDirection);
		pObject->SetPosition( wPosX, wPosY);

		pObject->SetAnimation( XiahAniType::eLAT_Stand, -1);

		pObject->m_szObjectName = szName;
		pObject->m_bObjType		= OBJTYPE_FUNCTIONALNPC;
		pObject->m_bSubObjType	= bType;
		
		sFunctionalNpcInfo* pInfo = new sFunctionalNpcInfo;
		pInfo->m_dwObjectID = dwObjectID;
		pInfo->m_bType		= bType;
		pInfo->m_bKind		= bKind;
		pInfo->m_szName		= szName;
		pInfo->m_bOwnType	= bOwnType;
		pInfo->m_dwOwnID	= dwOwnID;
		pInfo->m_bSizeX		= bSizeX;
		pInfo->m_bSizeY		= bSizeY;
		pInfo->m_wNumItem	= wNumItem;

		pObject->m_pPrivateData = (DWORD)pInfo;
		pObject->m_PrivateDataDestoryer = ReleaseFunctionalNpcInfo; // 앗싸
	}

	return TRUE;
}




//---------------------------------------------------------------------------------------
int OnCS_NC_STARTMOVE_ACK(CMsg &msg)
{
	try
	{
		BYTE	bResult		=0;	
		BYTE	bObjectType	=0;
		BYTE	bHeight		=0;
		BYTE	bStatus		=0;
		BYTE	bSpeed		=0;
		BYTE	bDesHeight	=0;
		WORD	wPosX		=0;
		WORD	wPosY		=0;	
		WORD	wDesPosX	=0;
		WORD	wDesPosY	=0;	
		WORD	wDirection	=0;
		DWORD	dwObjectID	=0;

		msg
			>> bResult
			>> dwObjectID
			>> bObjectType
			>> wPosX
			>> wPosY
			>> bHeight
			>> wDesPosX
			>> wDesPosY
			>> bDesHeight
			>> wDirection
			>> bStatus
			>> bSpeed;

		XiahObject::CXiahObject *pXiahObject = XiahObject::g_XiahObjectManager.FindXiahObject( MAKEOBJECTID( 0, dwObjectID,bObjectType));
		if( pXiahObject == NULL)
		{
			ValidateObject( bObjectType, dwObjectID, wPosX, wPosY);
			return TRUE;
		}

		if( g_PetList.Find( dwObjectID) != NULL)
		{
			// 내 Pet는 무시
			return TRUE;
		}

		CXiahCharObject *pObject = reinterpret_cast<CXiahCharObject*>(pXiahObject->m_pObject);
		if (!pObject)
			return TRUE;
		
		pObject->m_SyncPosition = Vector3( wPosX,0, -wPosY);

		if( bObjectType == OBJTYPE_NPC )
		{
			float fDistToServer = pObject->GetDistance( wPosX, wPosY );
			if( fDistToServer > 15.0f )
			{
				pObject->SetPosition( wPosX, wPosY );
			}

			Vector3 vTarget( (float)wDesPosX, pObject->m_Position.y, -(float)wDesPosY );
			pObject->SetAngleTarget( vTarget );
		}
		else
		{
			if( wDesPosX != wPosX || wDesPosY != wPosY )
			{
				float target_angle = atan2(((float)wDesPosX - (float)wPosX),((float)wDesPosY - (float)wPosY));
				short angle = GetServerAngle( target_angle);
				if( angle < 0)
					angle += 360;

				wDirection = angle;
			}

			pObject->SetAngle( wDirection);
		}

		// 2004.08.05 Changth
		// 음 이런일이 생기다니,, 나의 분신은 내 컴에서 달리는데, 다른 컴에서는 
		// 느리거 걷다가 점프를 하는군. 이걸 클라이언트가 간단한 방법으로 해결하자.
		// 펫의 캐릭터아이디가 검영이면 캐릭터가 달리는 것과 동일하게 해준다. ^^
		bool bBunsin = false;
		if( bObjectType == OBJTYPE_PET && bStatus == NPCSTATUS_RUN )
		{
			if(pObject->m_CharRender.GetCharID() == 790 )
				bBunsin = true;
		}

		if( bStatus == NPCSTATUS_WALK)
		{
			// 动画防重入：已在 Walk 状态时不重置动画帧，避免小碎步
			if( pObject->m_nCurMotionType != XiahAniType::eLAT_Walk)
				pObject->SetAnimation( XiahAniType::eLAT_Walk, 0);
		}
		else if( bStatus == NPCSTATUS_RUN)
		{
			if( bBunsin )
			{
				if( pObject->m_nCurMotionType != XiahAniType::eLAT_Run)
				{
					if (!pObject->SetAnimation( XiahAniType::eLAT_Run, 1 ))
						pObject->SetAnimation( XiahAniType::eLAT_Walk, 1 );
				}
			}
			else
			{
				if( pObject->m_nCurMotionType != XiahAniType::eLAT_Run)
				{
					if (!pObject->SetAnimation( XiahAniType::eLAT_Run, 0 ))
						pObject->SetAnimation( XiahAniType::eLAT_Walk, 0 );
				}
			}
		}
		
		if( bObjectType == OBJTYPE_PET)
		{
			float fMoveSpeed = (float)bSpeed / 9.0f;
			if( fMoveSpeed > 2.0f)
				fMoveSpeed = 2.0f;

			if( bBunsin )
				fMoveSpeed *= 3.0f;

			pObject->m_CharRender.SetAnimationSpeed( fMoveSpeed );
		}
		else
		{
			// 普通怪物/NPC：保持 1.0f 标准奔跑速率，彻底消灭 bSpeed/9 导致的慢动作追击与滞后闪跳
			pObject->m_CharRender.SetAnimationSpeed( 1.0f );
		}

		pObject->m_LastNavigationTime = g_dwCurTime;
		pObject->SetTargetMove( wDesPosX, wDesPosY, eLBP_CharNavigation, 100);

		WORD wCurPosX, wCurPosY;
		WORD wAngle;
		pObject->GetAngle( wAngle);
		pObject->GetPosition( wCurPosX, wCurPosY);

		Vector3 scPos;
		sRect rcRect;
		if( g_pCurrentCamera )
		{
			scPos = g_pCurrentCamera->WorldToScreen( pObject->m_Position + Vector3( 0, pObject->m_LocalBound.m_vMax.y, 0));
			rcRect.left = scPos.x;
			rcRect.top	= scPos.y;
		}
		else
		{
			rcRect.left = pObject->m_rcObjectScreenPos.left;
			rcRect.top	= pObject->m_rcObjectScreenPos.top;
		}

		return TRUE;
	}
	catch(...)
	{
		return TRUE;
	}
}

//---------------------------------------------------------------------------------------
int OnCS_NC_SYNCMOVE_ACK(CMsg &msg)
{
	BYTE	bResult		=0;	
	BYTE	bObjectType	=0;	
	BYTE	bHeight		=0;	
	BYTE	bDesHeight	=0;	
	BYTE	bStatus		=0;
	BYTE	bSpeed		=0;
	WORD	wPosX		=0;
	WORD	wPosY		=0;
	WORD	wDesPosX	=0;
	WORD	wDesPosY	=0;
	WORD	wDirection	=0;
	WORD	wDiffTime	=0;
	DWORD	dwObjectID	=0;

	msg
		>> bResult
		>> dwObjectID
		>> bObjectType
		>> wPosX
		>> wPosY
		>> bHeight
		>> wDesPosX
		>> wDesPosY
		>> bDesHeight
		>> wDirection
		>> bStatus
		>> bSpeed
		>> wDiffTime;

	XiahObject::CXiahObject *pXiahObject = XiahObject::g_XiahObjectManager.FindXiahObject( MAKEOBJECTID( 0, dwObjectID,bObjectType));
	if( pXiahObject == NULL)
	{
		ValidateObject( bObjectType, dwObjectID, wPosX, wPosY);
		return TRUE;
	}

	if( g_PetList.Find( dwObjectID) != NULL)
	{
		// 내 Pet는 무시
		return TRUE;
	}

	CXiahCharObject *pObject = reinterpret_cast<CXiahCharObject*>(pXiahObject->m_pObject);
	if(NULL == pObject)
		return FALSE;

	WORD wPreSyncPosX = pObject->m_SyncPosition.x;
	WORD wPreSyncPosY = -pObject->m_SyncPosition.z;

	// 2004.08.05 Changth
	// 음 이런일이 생기다니,, 나의 분신은 내 컴에서 달리는데, 다른 컴에서는 
	// 느리거 걷다가 점프를 하는군. 이걸 클라이언트가 간단한 방법으로 해결하자.
	// 펫의 캐릭터아이디가 검영이면 캐릭터가 달리는 것과 동일하게 해준다. ^^
	bool bBunsin = false;
	if( bObjectType == OBJTYPE_PET && bStatus == NPCSTATUS_RUN )
	{
		if( pObject->m_CharRender.GetCharID() == 790 )
			bBunsin = true;
	}

	float fScale = 1.0f;
	if( pObject->m_nCurMotionType != XiahAniType::eLAT_Walk && pObject->m_nCurMotionType != XiahAniType::eLAT_Run)
	{
		pObject->SetPosition( wPosX, wPosY);
		pObject->SetAngle( wDirection);

		if( bStatus == NPCSTATUS_WALK)
		{
			pObject->SetAnimation( XiahAniType::eLAT_Walk, 0);
		}
		else if( bStatus == NPCSTATUS_RUN)
		{
			if( bBunsin )
				pObject->SetAnimation( XiahAniType::eLAT_Run, 1 );
			else
				pObject->SetAnimation( XiahAniType::eLAT_Run, 0);
		}

		pObject->m_SyncPosition = Vector3( wPosX, 0, -wPosY);

		if( bObjectType == OBJTYPE_PET)
		{
			float fMoveSpeed = (float)bSpeed / 9.0f;
			if( fMoveSpeed > 2.0f)
				fMoveSpeed = 2.0f;

			if( bBunsin )
				fMoveSpeed *= 3.0f;

			pObject->m_CharRender.SetAnimationSpeed( fMoveSpeed );
		}
	}
	else
	{
		fScale = pObject->GetSyncMoveScale( wPosX, wPosY);
		
		WORD angle; 
		WORD wCurX;
		WORD wCurY;
		pObject->GetAngle(angle);
		pObject->GetPosition( wCurX, wCurY);

		if( bObjectType == OBJTYPE_PET)
		{
			float fMoveSpeed = (float)bSpeed / 9.0f;
			/*
			if( fMoveSpeed > 2.0f)
					fMoveSpeed = 2.0f;
			*/

			if( bBunsin )
				fMoveSpeed *= 3.0f;

			pObject->m_CharRender.SetAnimationSpeed( fMoveSpeed );
		}
		else
		{
			pObject->m_CharRender.SetAnimationSpeed( fScale);
		}

		pObject->m_LastNavigationTime = g_dwCurTime;
	}

	return TRUE;	
}

//---------------------------------------------------------------------------------------
int OnCS_NC_ENDMOVE_ACK(CMsg &msg)
{
	BYTE	bResult		=0;	
	BYTE	bObjectType	=0;	
	BYTE	bHeight		=0;
	BYTE	bStatus		=0;
	DWORD	dwObjectID	=0;
	WORD	wPosX		=0;
	WORD	wPosY		=0;

	msg
		>> bResult
		>> dwObjectID
		>> bObjectType
		>> wPosX
		>> wPosY
		>> bHeight
		>> bStatus;

	XiahObject::CXiahObject *pXiahObject = XiahObject::g_XiahObjectManager.FindXiahObject( MAKEOBJECTID( 0, dwObjectID, bObjectType));
	if( pXiahObject == NULL)
	{
		ValidateObject( bObjectType, dwObjectID, wPosX, wPosY);
		return TRUE;
	}

	if( g_PetList.Find( dwObjectID) != NULL)
	{
		// 내 Pet는 무시
		return TRUE;
	}

	CXiahCharObject *pObject = reinterpret_cast<CXiahCharObject*>(pXiahObject->m_pObject);
	if(NULL == pObject)
		return FALSE;

	WORD angle; 
	WORD wCurX;
	WORD wCurY;
	pObject->GetAngle(angle);
	pObject->GetPosition( wCurX, wCurY);

	if( bObjectType == OBJTYPE_PET)
	{
		float fDistToEnd = pObject->GetDistance( wPosX, wPosY );
		if( fDistToEnd <= 0.8f )
		{
			pObject->SetPosition( wPosX, wPosY );
			pObject->m_bTargetMove = FALSE;
			pObject->m_CharRender.SetAnimationSpeed( 1.0f );
			pObject->SetAnimation( XiahAniType::eLAT_Stand, 0 );
		}
		else if( fDistToEnd <= 8.0f )
		{
			pObject->SetAnimation( XiahAniType::eLAT_Run, 0 );
			pObject->SetTargetMove( wPosX, wPosY, eLBP_CharNavigation, 0 );
			float fSnapSpeed = fDistToEnd / 0.15f / 9.0f;
			if( fSnapSpeed < 2.0f ) fSnapSpeed = 2.0f;
			if( fSnapSpeed > 4.5f ) fSnapSpeed = 4.5f;
			pObject->m_CharRender.SetAnimationSpeed( fSnapSpeed );
		}
		else
		{
			pObject->SetPosition( wPosX, wPosY );
			pObject->m_bTargetMove = FALSE;
			pObject->m_CharRender.SetAnimationSpeed( 1.0f );
			pObject->SetAnimation( XiahAniType::eLAT_Stand, 0 );
		}
	}
	else
	{
		// 怪物与NPC停步：双阶平滑吸附停步
		float fDistToEnd = pObject->GetDistance( wPosX, wPosY );
		if( fDistToEnd <= 1.5f )
		{
			// 1.5 格以内近身射程误差：无感直接吸附对齐立定，动画恢复 1.0f，彻底消除未对齐与隔空出刀
			pObject->SetPosition( wPosX, wPosY );
			pObject->m_bTargetMove = FALSE;
			pObject->m_CharRender.SetAnimationSpeed( 1.0f );
			pObject->SetAnimation( XiahAniType::eLAT_Stand, -1 );
		}
		else if( fDistToEnd <= 8.0f )
		{
			// 1.5 ~ 8.0 格中等滞后：在 150ms 内极速急刹滑步到位
			if (!pObject->SetAnimation( XiahAniType::eLAT_Run, 0 ))
				pObject->SetAnimation( XiahAniType::eLAT_Walk, 0 );
			pObject->SetTargetMove( wPosX, wPosY, eLBP_CharNavigation, 0 );
			float fSnapSpeed = fDistToEnd / 0.15f / 9.0f;
			if( fSnapSpeed < 1.5f ) fSnapSpeed = 1.5f;
			if( fSnapSpeed > 3.5f ) fSnapSpeed = 3.5f;
			pObject->m_CharRender.SetAnimationSpeed( fSnapSpeed );
		}
		else
		{
			// 超大距离异常：安全瞬间校准并站定，动画速度恢复 1.0f
			pObject->SetPosition( wPosX, wPosY );
			pObject->m_bTargetMove = FALSE;
			pObject->m_CharRender.SetAnimationSpeed( 1.0f );
			pObject->SetAnimation( XiahAniType::eLAT_Stand, -1 );
		}
	}

	return TRUE;
}


//---------------------------------------------------------------------------------------
int OnCS_NC_MAPENTER_ACK(CMsg &msg)
{
	BYTE  bResult		=0;	
	BYTE  bObjectType	=0;	
	BYTE  bHeight		=0;
	BYTE  bStatus		=0;
	BYTE  bSpeed		=0;
	WORD  wPosX			=0;
	WORD  wPosY			=0;
	WORD  wDirection	=0;	
	DWORD dwMapID		=0;
	DWORD dwObjectID	=0;

	msg
		>> bResult
		>> dwMapID
		>> dwObjectID
		>> bObjectType
		>> wPosX
		>> wPosY
		>> bHeight
		>> wDirection
		>> bStatus
		>> bSpeed;

	if( bObjectType == OBJTYPE_NPC)
		SendCS_NC_NPCINFO_REQ( dwObjectID);
	else if( bObjectType == OBJTYPE_PET)
		SendCS_NC_PETINFO_REQ( dwObjectID);

	return TRUE;
}

//---------------------------------------------------------------------------------------
int OnCS_NC_MAPLEAVE_ACK(CMsg &msg)
{
	BYTE	bResult			=0;	
	BYTE	bObjectType		=0;
	DWORD	dwObjectID		=0;
	DWORD	dwMapID			=0;

	msg
		>> bResult
		>> dwObjectID
		>> bObjectType
		>> dwMapID;

	XiahObject::g_XiahObjectManager.ReleaseXiahObject( MAKEOBJECTID( 0, dwObjectID, bObjectType));	

	DBG_Put("MAPLEAVE ACK %d",dwObjectID);

	return TRUE;
}




//---------------------------------------------------------------------------------------
int OnCS_NC_STATUSCHANGE_ACK(CMsg &msg)
{
	DWORD dwObjectID	=0;
	BYTE bObjectType	=0;	
	BYTE bState			=0;	
	BYTE bSubType		=0;
	WORD wDirection		=0;
	
	msg
		>> bObjectType
		>> dwObjectID
		>> bState
		>> wDirection
		>> bSubType;


	BYTE	bHi_Subtype,bLo_Subtype;
	BYTE	bHi_Subtype_Old,bLo_Subtype_Old;

	bHi_Subtype = (bSubType & 0xf0) >> 4;
	bLo_Subtype = (bSubType & 0x0f);

	XiahObject::CXiahObject *pObject = XiahObject::g_XiahObjectManager.FindXiahObject( MAKEOBJECTID( 0, dwObjectID,bObjectType));

	if( pObject == NULL)
	{
		//	ValidateObject( bObjectType, dwObjectID, wPosX, wPosY);
		// 없는넘에 대한 Status Change는 받지 않겠다
		return TRUE;
	}

	// 퀘스트용 NPC
	if(bObjectType == OBJTYPE_NPC && bSubType != 255) 
	{
		if( bHi_Subtype == 0 && bLo_Subtype == 0 ) // 처음 상태를 두번 할 필요가 없다.
			return TRUE;

		CXiahCharObject *pObj = reinterpret_cast<CXiahCharObject*>(pObject->m_pObject);

		sArrayData *pData = XiahArrayIndex::g_NpcType.GetData(pObj->m_bSubObjType);
		if( pData == NULL) return TRUE;

		bHi_Subtype_Old = (pObj->m_bExSubObjType & 0xf0) >> 4;
		bLo_Subtype_Old = (pObj->m_bExSubObjType & 0x0f);

		// 화면 번쩍! 이건 메시 바뀔때!
		bool bExplore = false;	// 폭파 이펙트
		if( bHi_Subtype_Old != bHi_Subtype)
		{
			//Make_Special_Effect(0xffffffff);
			bExplore = true;	// 메시가 바꾸었으니 폭파이펙트가 나온다.

		}

		// UPDATE
		pObj->m_bExSubObjType = bSubType;

		// Subtype에 따라서 Mesh를 바꾸어 준다.
		// Subtype이 바로 Mesh Type 과 같다
		int nCharID = pData->GetInt( 1);
		int nMeshType = bHi_Subtype;
		int nTextureType = pData->GetInt( 3);

		if( XiahGameEngine::GetCharacter( nCharID) == NULL)
			return TRUE;

		CRes_Character* pResChar = XiahGameEngine::GetCharacter( nCharID);
		if( pResChar == NULL)return TRUE;
		if( !pResChar->GetMesh( nMeshType))	return TRUE;
		if( pResChar->GetMesh( nMeshType)->GetTexture( nTextureType) == NULL)return TRUE;

		int nCurCharID   = pObj->m_CharRender.GetCharID();
		int nCurMeshType = pObj->m_CharRender.GetMeshType();
		int nCurTexType  = pObj->m_CharRender.GetTextureType();

		// 데이타가 바뀔때만 하자.
		if( nCurCharID   != nCharID ||
			nCurMeshType != nMeshType ||
			nCurTexType  != nTextureType  )
            pObj->Create( nCharID, nMeshType, nTextureType, 1);

		// Mesh Effect를 만들때 Create 후에 Update 하기 전에 해야 한다.
		pObj->m_CharRender.ClearMeshEffect();

		// 1번째 폭파 이펙트는 2번째 메시에 붙어 있다.
		if( bExplore )
		{
			if( nMeshType == 2 )
			{
				pObj->m_CharRender.MakeMeshEffect( bLo_Subtype );
				g_MainCharInfo.PlayInterfaceSoundWithVol(BOOM_FXSOUND1,pObj->m_Position);
			}
			else
			{
				int nAry[2];
				nAry[0] = bLo_Subtype;
				nAry[1] = 3;	// 4번째가 폭파 이펙트이다.
				pObj->m_CharRender.MakeMeshEffect( 2, nAry );
				g_MainCharInfo.PlayInterfaceSoundWithVol(BOOM_FXSOUND2,pObj->m_Position);
			}
		}
		else
			pObj->m_CharRender.MakeMeshEffect( bLo_Subtype );

		pObj->Update();
	}

	if(bObjectType == OBJTYPE_NPC || bObjectType == OBJTYPE_PET )
	{
		CXiahCharObject* pCharObject = reinterpret_cast<CXiahCharObject*>(pObject->m_pObject);

		if( NULL == pCharObject )
			return FALSE;

		pCharObject->m_bTargetMove = FALSE;

		// Quest NPC일때는 건너뛴다.
		if( bSubType != 255 )
			return TRUE;

		pCharObject->m_bObjStatus = bState;

		switch( bState)
		{
		// 숨어있다가 나온다
		case NPCSTATUS_UNHIDE:
			{
				// 캐릭터가 등장할때 이펙트
				Vector3 vSize = pCharObject->m_LocalBound.Size();
				vSize.y = 0;
				float fSize = vSize.GetLength();

				_EFFECTPACKAGE* pPackage = NULL;
				if( fSize >= 10.0f )
				{
					pPackage = g_EffectManager.EnqAppearEffectImmediately( eBig );
				}
				else if( fSize < 10.0f && fSize >= 5.0f )
				{
					pPackage = g_EffectManager.EnqAppearEffectImmediately( eMiddle );
				}
				else
				{
					pPackage = g_EffectManager.EnqAppearEffectImmediately( eSmall );
				}

				pCharObject->UpdateTM();

				if( pPackage && pPackage->pEffectRender && pPackage->pEffectRender->pPackagePair )
				{
					pPackage->pEffectRender->pPackagePair->WorldMatrix = (MATRIX)pCharObject->m_ObjectTM;

					pCharObject->m_EffectPPList.push_back( pPackage->pEffectRender->pPackagePair );
				}
			}
			break;

		// 숨어있는 NPC 처리
		case NPCSTATUS_HIDE:
			{
				pCharObject->m_CharRender.SetLoopAnimation(FALSE);
			}
			break;
		case NPCSTATUS_IDLE:
			if( g_PetList.Find( dwObjectID) == NULL)
			{
				pCharObject->SetAnimation( XiahAniType::eLAT_Idle, 0);
				pCharObject->m_CharRender.SetLoopAnimation(FALSE);
			}
			break;
		case NPCSTATUS_IDLE1:
			if( g_PetList.Find( dwObjectID) == NULL)
			{
				pCharObject->SetAnimation( XiahAniType::eLAT_Idle, 1);
				pCharObject->m_CharRender.SetLoopAnimation(FALSE);
			}
			break;
		case NPCSTATUS_IDLE2:
			if( g_PetList.Find( dwObjectID) == NULL)
			{
				pCharObject->SetAnimation( XiahAniType::eLAT_Idle, 2);
				pCharObject->m_CharRender.SetLoopAnimation(FALSE);
			}
			break;
		case NPCSTATUS_WAKEUP:
			if( g_PetList.Find( dwObjectID) == NULL)
			{
				pCharObject->SetAnimation( XiahAniType::eLAT_Idle, 0);
				pCharObject->m_CharRender.SetReverseAnimation();
				pCharObject->m_CharRender.SetLoopAnimation( FALSE);
			}
			break;
		case NPCSTATUS_DIE:
			{
				// 业务设计意图：在被选中的怪物/对象死亡瞬间，客户端立即丢弃该目标，清空选取状态与光环，并关闭目标面板，
				// 从而根治倒地后长达 3 秒因等待离开视野包而导致的目标滞留不去的卡顿体感。
				extern DWORD dwSelObjectID;
				extern DWORD dwSelObjectType;
				extern BOOL bAutoAttack;
				extern BOOL bAutoNormalAttack;
				extern BOOL bAutoNavigation;
				if (dwSelObjectID == dwObjectID && dwSelObjectType == bObjectType)
				{
					dwSelObjectID = 0;
					dwSelObjectType = 0;
					bAutoAttack = FALSE;
					bAutoNormalAttack = FALSE;
					bAutoNavigation = FALSE;
					g_MainCharInfo.m_dwPickedObject = 0;
				}

				// PET 일경우에 Pet list에서 지워준다
				if(bObjectType == OBJTYPE_PET)
				{
					//HT_0403 : 지속형 무공 시전 아이콘
					CXiahCharObject *pCharObj = reinterpret_cast<CXiahCharObject*>(pObject->m_pObject);

					sPetInfo* pPetInfo = (sPetInfo*)pCharObj->m_pPrivateData;

					if(pPetInfo)
					{

						if(pPetInfo->dwOwnID == g_MainCharInfo.m_dwObjectID)
						{
						//	sPetInfo* pPetInfo = (sPetInfo*)pCharObj->m_pPrivateData;

							g_PetList.DeletePet(dwObjectID);

							//HT_CHEAT : 응룡 다시 소환 사냥
							if(pPetInfo->m_dwIsHwan == 1 && g_bCheat)
							{
								SendCS_BT_MUGONGPREATTACK_REQ(OUTGONGID_WHANSUYUO,1,g_pMainChar->m_dwServerID,1,1,1, 1, g_pMainChar->m_dwServerID, 0, 0, 0);
							}

							if(pPetInfo->m_dwIsHwan == 2 && g_bCheat) //분신 소환
							{
								SendCS_BT_MUGONGPREATTACK_REQ(41,1,g_pMainChar->m_dwServerID,1,1,1, 1, g_pMainChar->m_dwServerID, 0, 0, 0);
							}
						}
					}

					// 2004.08.04 Changth
					// 분신격일때, 오랫동안 시체(?)가 남아 있는 경우가 있는데,
					// 즉으면 조금 후에 바로 강제로 지워준다.
					pCharObject->m_LastNavigationTime = g_dwCurTime;
				}
				
				// 성인서버용 NPC				
				//if(g_AppData.m_bAdult && bObjectType != OBJTYPE_PET)
				else if(g_AppData.m_bAdult)
				{
					CXiahCharObject *pObj = reinterpret_cast<CXiahCharObject*>(pObject->m_pObject);
					if(pObj)
					{
						BYTE bSubType = pObj->m_bSubObjType;

						if(bSubType == 0 || (bSubType >=7 && bSubType <=9) || (bSubType >=88 && bSubType <= 93) ||
							bSubType == 81 || (bSubType >= 97 && bSubType <=99) || (bSubType >= 168 && bSubType <= 170) ||
							bSubType == 182 || bSubType == 183 || (bSubType >= 174 && bSubType <= 176))
						{
							sArrayData *pData1 = XiahArrayIndex::g_NpcType.GetData(pObj->m_bSubObjType);
							sArrayData *pData2 = XiahArrayIndex::g_NpcType2.GetData(pObj->m_bSubObjType);
							if(pData1 && pData2)
							{
								pCharObject->m_CharRender.ClearMeshEffect();
								pCharObject->ClearMugongEffect();
								pCharObject->m_KeepUpMugongList.clear();

								pCharObject->Create(pData2->GetInt(1), pData2->GetInt(2), pData2->GetInt(3), -1);

								/////////////////////////////////////////////////////////////////////////////////////////////////////
								if(11 == XiahMap::g_XiahMap.m_MapInfo.m_dwMapID)	// 던전에서만
								{
									switch(pData1->GetInt(1))
									{
									case 816:	// 빙요
										pCharObject->m_CharRender.SetLocalScale(Vector3(1.5f, 1.5f, 1.5f));
										break;
									case 902:	// 해신
										pCharObject->m_CharRender.SetLocalScale(Vector3(1.5f, 1.5f, 1.5f));
										break;
									case 942:	// 석귀
										pCharObject->m_CharRender.SetLocalScale(Vector3(1.5f, 1.5f, 1.5f));
										break;
									case 1041:	// 적호
										pCharObject->m_CharRender.SetLocalScale(Vector3(1.5f, 1.5f, 1.5f));
										break;
									case 1042:	// 금각동인
										pCharObject->m_CharRender.SetLocalScale(Vector3(1.5f, 1.5f, 1.5f));
										break;
									case 1040:	// 인면수
										pCharObject->m_CharRender.SetLocalScale(Vector3(1.5f, 1.5f, 1.5f));
										break;
									case 1043:	// 혈기린
										pCharObject->m_CharRender.SetLocalScale(Vector3(1.55f, 1.55f, 1.55f));
										break;
									case 1044:	// 금각거인
										pCharObject->m_CharRender.SetLocalScale(Vector3(1.15f, 1.15f, 1.15f));
										break;
									case 1045:	// 교룡
										pCharObject->m_CharRender.SetLocalScale(Vector3(1.5f, 1.5f, 1.5f));
										break;
									case 1039:	// 대조귀
										pCharObject->m_CharRender.SetLocalScale(Vector3(0.85f, 0.85f, 0.85f));
										break;
									}

									// [5/4/2005] 전체 ID 수정 - 아래도 전부 횅땍
									switch(pCharObject->m_bOrderID)
									{
									case 1:	// 보스
										pCharObject->m_CharRender.SetLocalScale( Vector3( 1.7f, 1.7f, 1.7f));
										break;
									case 2:	// 행동대장
										pCharObject->m_CharRender.SetLocalScale( Vector3( 1.6f, 1.6f, 1.6f));
										break;										
									}

									/*
									switch(pCharObject->m_bOrderID)
									{
									case 0:	// 보스
										pCharObject->m_CharRender.SetLocalScale(Vector3(1.7f, 1.7f, 1.7f));
										break;
									case 1:	// 행동대장
										pCharObject->m_CharRender.SetLocalScale(Vector3(1.6f, 1.6f, 1.6f));
										break;
									case 2:	// 노련한
										pCharObject->m_CharRender.SetLocalScale(Vector3(0.8f, 0.9f, 0.8f));
										break;
									}
									*/
								}
								else
								{
									switch(pCharObject->m_bOrderID)
									{
									case 1:	// 보스
										pCharObject->m_CharRender.SetLocalScale( Vector3( 1.4f, 1.4f, 1.4f));
										break;
									case 2:	// 행동대장
										pCharObject->m_CharRender.SetLocalScale( Vector3( 1.2f, 1.2f, 1.2f));
										break;										
									}

									/*
									switch(pCharObject->m_bOrderID)
									{
									case 0:	// 보스
										pCharObject->m_CharRender.SetLocalScale(Vector3(1.4f, 1.4f, 1.4f));
										break;
									case 1:	// 행동대장
										pCharObject->m_CharRender.SetLocalScale(Vector3(1.2f, 1.2f, 1.2f));
										break;
									case 2:	// 노련한
										pCharObject->m_CharRender.SetLocalScale(Vector3(0.8f, 0.9f, 0.8f));
										break;
									}
									*/
								}
								/////////////////////////////////////////////////////////////////////////////////////////////////////
							}
						}						
					}					
				}				

				pCharObject->SetAnimation(XiahAniType::eLAT_Die, XiahAniType::eLAT_Die, 0, 1);

				pCharObject->m_bTargetMove = FALSE;

				// 현재 캐릭터가 Mesh Effect가 있으면 없애준다.
				pCharObject->m_CharRender.ClearMeshEffect();

				pCharObject->ClearMugongEffect();

				pCharObject->m_KeepUpMugongList.clear();

				// 캐릭터가 죽을때 나오는 이펙트도 있다.
				switch( pCharObject->m_bSubObjType )
				{
				case 184:	// 신조
					{
						_EFFECTPACKAGE* pEffectPackage = g_EffectManager.EnqAppearEffectImmediately( eShinjo_Explode, 0, 0, 10, 0 );

						if( pEffectPackage && pEffectPackage->pEffectRender && pEffectPackage->pEffectRender->pPackagePair )
						{
							//pEffectPackage->pEffectRender->pPackagePair->pWorldMatrix = (MATRIX*)pCharObject->m_CharRender.GetCharTM();
							pEffectPackage->pEffectRender->pPackagePair->WorldMatrix = (MATRIX)pCharObject->m_ObjectTM;
						}

						// Sound
						g_MainCharInfo.PlayInterfaceSoundWithVol( NPC_SOUND_SHINJO_DIE, pCharObject->m_Position );
					}
					break;
				};

				/// 넌 인제 Client꺼다
				//XiahObject::g_XiahObjectManager.ChangeToClientObject( pObject);
			}
			break;
		}
	}

	return TRUE;
}

/**
 * 기능NPC 아이템 리스트
 * \param &msg 
 * \return 
 */
int OnCS_NC_FUNCTIONALNPCITEMLIST_ACK(CMsg &msg)
{
	BYTE	bResult		=0;	
	BYTE	bSackID		=0;
	WORD	wItemNum	=0;
	WORD	wAmount		=0;
	DWORD	dwObjectID	=0;
	
	msg
		>> bResult
		>> dwObjectID
		>> bSackID
		>> wItemNum;

	CloseAllWindow();

	g_MainCharInfo.ShowSack( SACKTYPE__NPC_TRADE);
	g_MainCharInfo.ShowSack( SACKTYPE__DEFAULT);
	g_MainCharInfo.ShowSack( SACKTYPE__EQUIPMENT);

	for(int i=0; i < wItemNum; ++i) 
	{
		XiahItem::sItemInfo* pItem = new XiahItem::sItemInfo;
	
		msg
			>> pItem->m_wRefID
			>> pItem->m_bItemType
			>> pItem->m_bItemKind
			>> pItem->m_wVisualID
			>> pItem->m_szName
			>> pItem->m_dwPrice 
			>> pItem->m_wLevel
			>> pItem->m_bNeedCharType
			>> wAmount
			>> pItem->m_bSackPos
			>> pItem->m_bRarity;

		pItem->m_dwAmount = wAmount;

		WORD wDecrDurRate			=0;
		WORD wIncrCritical			=0;
		WORD wHukjungModityCount	=0;
		WORD wSojungModityCount		=0;
		WORD wModifyCount			=0;

		switch( pItem->m_bItemType)
		{
		case ITEMTYPE_WEAPON:
		case ITEMTYPE_CLOTH:
		case ITEMTYPE_HAT:
		case ITEMTYPE_SHOE:
		case ITEMTYPE_CLOAK:
		case ITEMTYPE_RING:
		case ITEMTYPE_NECKLACE:
		case ITEMTYPE_SOCKET:
		case ITEMTYPE_BONGIN:
			{
				msg
					>> pItem->m_wNeedLevel
					>> pItem->m_wNeedDex
					>> pItem->m_wNeedStr
					>> pItem->m_wNeedSus
					>> pItem->m_wNeedVit
					>> wDecrDurRate
					>> pItem->m_wCurDur
					>> pItem->m_wMaxDur
					>> pItem->m_wAtkPwr
					>> pItem->m_wDefPwr
					>> pItem->m_wAtkRating
					>> pItem->m_wStkSpeed
					>> pItem->m_wAtkRange
					>> pItem->m_wIncrHp
					>> pItem->m_wIncrIp
					>> pItem->m_wRestoreHp
					>> pItem->m_wRestoreIp
					>> wIncrCritical
					>> wHukjungModityCount
					>> wSojungModityCount
					>> pItem->m_wHuljungModityCount
					>> wModifyCount;

				pItem->m_bDecrDurRate	= wDecrDurRate;
				pItem->m_wIncrCritical	= wIncrCritical;
				pItem->m_bRarity		= wHukjungModityCount;
				pItem->m_bStxType		= wSojungModityCount;
				pItem->m_bModifyCnt		= wModifyCount;

				if( pItem->m_bItemType == ITEMTYPE_BONGIN )
				{
					msg
						>> pItem->m_wSoakHPRatio
						>> pItem->m_wSoakAtkRatio
						>> pItem->m_wSoakDefRatio
						>> pItem->m_wSoakHitRatio;
				}
			}
			break;
		case ITEMTYPE_POTION:
			{
				msg
					>> pItem->m_wIncrHp
					>> pItem->m_wIncrIp;
			}			
			break;
		case ITEMTYPE_NPCITEM:
			{
				WORD wNpcItemType	=0;
				WORD wWildRate		=0;

				msg 
					>> pItem->m_wTamingLevel
					>> wNpcItemType
					>> pItem->m_wTamingRate
					>> wWildRate
					>> pItem->m_wIncrHp;

				pItem->m_bNpcItemType	= wNpcItemType;
				pItem->m_bWildRate		= wWildRate;
			}
			break;		
		case ITEMTYPE_NPCBAG:
			{
				msg
					>> pItem->m_bDecrDurRate
					>> pItem->m_wCurDur
					>> pItem->m_wMaxDur;

			}
			break;		
		case ITEMTYPE_BOOK:
			break;
		case ITEMTYPE_GISDURABLITY:
			{
				msg
					>> pItem->m_wFunctionItem // 기능번호
					>> pItem->m_wCurDur
					>> pItem->m_wMaxDur // 최대내구력
					>> pItem->m_dwValue;
			}
			break;
		case ITEMTYPE_LOTTO:
			{
				msg
					>> pItem->m_bPrizeRank
					>> pItem->m_dwRound
					>> pItem->m_bLottoNum[0]
					>> pItem->m_bLottoNum[1]
					>> pItem->m_bLottoNum[2]
					>> pItem->m_bLottoNum[3]
					>> pItem->m_dwPrizeMoney;
			}
			break;
		case ITEMTYPE_NPCRING:			
		case ITEMTYPE_NPCNECKLACE:			
		case ITEMTYPE_NPCWEAPON:			
		case ITEMTYPE_NPCRIDING:
		case ITEMTYPE_SADDLE:
			{
				BYTE bTemp = 0;

				msg
					>> bTemp	// 내구감소
					>> pItem->m_wCurDur
					>> pItem->m_wMaxDur;
			}
			break;
		case 18: //HT_0828 : 수집낭 판매 이벤트(낭 )
			{
				msg
					>> pItem->m_bModifyCnt;
			}
			break;
		case 20: //HT_0828 : 수집낭 판매 이벤트(재료)
			{
				msg
					>> pItem->m_bFuncID
					>> pItem->m_dwValue;
			}
			break;
		}

		//pItem->m_bSackID = bSackID;
		pItem->m_bSackID = SACKTYPE__NPC_TRADE;		

		if( g_MainCharInfo.m_pNpcSack)
			g_MainCharInfo.m_pNpcSack->InsertItem( pItem->m_bSackPos, pItem);
	}

	return 0;
}

int OnCS_NC_PETINFO_ACK(CMsg &msg)
{
	/*
		이 메세지는 NPCINFO_ACK과 같이 남이 등장했을때 (EnterAck) 상세 정보를 받아서 만들어 주는 기능이다
	*/
	BYTE	bResult		=0;
	DWORD	dwID		=0;
	BYTE	bNpcType	=0;
	DWORD	dwMapID		=0;
	WORD	wPosX		=0;
	WORD	wPosY		=0;
	BYTE	bHeight		=0;
	WORD	wDirection	=0;	
	BYTE	bStatus		=0;
	WORD	wDesPosX	=0;
	WORD	wDesPosY	=0;
	BYTE	bDesHeight	=0;
	DWORD	dwHpMax		=0;
	DWORD	dwHpCur		=0;
	BYTE	bSpeed		=0;
	DWORD	dwOwnerID	=0;
	BYTE	bRevolutionStep	=0;
	sString szName;
	WORD	wVisualID[6];

	msg 
		>> bResult
		>> dwID
		>> bNpcType
		>> dwMapID
		>> wPosX
		>> wPosY
		>> bHeight
		>> wDirection
		>> szName
		>> bStatus
		>> wDesPosX
		>> wDesPosY
		>> bDesHeight
		>> dwHpMax
		>> dwHpCur
		>> bSpeed
		>> dwOwnerID
		>> bRevolutionStep
		>> wVisualID[0]
		>> wVisualID[1]
		>> wVisualID[2]
		>> wVisualID[3]
		>> wVisualID[4]
		>> wVisualID[5];

	//if( g_PetList.Find( dwID) != NULL)
	//	return 0;	// 내꺼는 무시

	// 2004.08.06 Changth
	// 오너를 찾는다
	if( bNpcType >= 251 )
	{
		DBG_LogFile( _T("[DEBUG_BUNSIN] OnCS_NC_PETINFO_ACK: received bunsin! dwID=%d, bNpcType=%d, dwOwnerID=%d"), dwID, (int)bNpcType, dwOwnerID );
	}

	XiahObject::CXiahObject* pOwner = XiahObject::g_XiahObjectManager.FindXiahObject( MAKEOBJECTID( 0, dwOwnerID, OBJTYPE_PC));

	if ( !pOwner )
	{
		if( bNpcType >= 251 )
		{
			DBG_LogFile( _T("[DEBUG_BUNSIN] Owner %d not found in ObjectManager!"), dwOwnerID );
		}
		return 0;
	}

    // 새로운 캐릭터면 첨부터 생성하지만, 존재하는 거라면 데이타를 바꿔준다.
	bool bCreateChar = true;

	XiahObject::CXiahObject* pXiahObject = XiahObject::g_XiahObjectManager.FindXiahObject( MAKEOBJECTID( 0, dwID, OBJTYPE_PET) );

	if( pXiahObject != NULL ) // 캐릭터 데이타가 있으면 new 하지 않고 데이타만 바꿔준다.
		bCreateChar = false;

	int nCharID = 0;
	int nMeshType = 0;
	int nTextureType = 0;

	if( bNpcType >= 251 )
	{
		CXiahCharObject* pOwnerChar = (CXiahCharObject*)pOwner->m_pObject;
		if ( !pOwnerChar )
		{
			DBG_LogFile( _T("[DEBUG_BUNSIN] pOwnerChar is NULL!") );
			return 0;
		}
		nCharID = pOwnerChar->m_CharRender.GetCharID();
		nMeshType = pOwnerChar->m_CharRender.GetMeshType();
		nTextureType = pOwnerChar->m_CharRender.GetTextureType();
	}
	else
	{
		sArrayData* pData = XiahArrayIndex::g_NpcType.GetData( bNpcType);
		
		if( pData == NULL)
			return 0;

		nCharID = pData->GetInt( 1);
		nMeshType = pData->GetInt( 2);
		nTextureType = pData->GetInt( 3);
	}


	if(bRevolutionStep == 4 && bNpcType == 0) //HT_0621 : 영수둔갑신단 적용)
	{
		// [10/31/2005] 캐릭터 ID변경
		nCharID = 1133;

		if(wVisualID[5])
		{
			nMeshType = 1;

			nTextureType = wVisualID[5] - 27700;
		}
	}

	if( XiahGameEngine::GetCharacter( nCharID) == NULL)
		return 0;

	CRes_Character* pResChar = XiahGameEngine::GetCharacter( nCharID);

	if( pResChar == NULL) return 0;
	if( !pResChar->GetMesh( nMeshType)) return 0;
	if( pResChar->GetMesh( nMeshType)->GetTexture( nTextureType) == NULL) return 0;

	//YS_0805 : BUGFIX
	CXiahCharObject* pCharObject = NULL;
	if( bCreateChar )
	{
		pCharObject = (CXiahCharObject *)XiahObject::g_XiahPetPool.GetChar();// new CXiahCharObject;

		if ( NULL == pCharObject )
		{
			pCharObject = new CXiahCharObject;
		}
	}
	else
	{
		pCharObject = reinterpret_cast<CXiahCharObject*>(pXiahObject->m_pObject);
	}

	//YS_0811 : BUGFIX
	if ( !pCharObject )
		return 0;

	XiahObject::CXiahObject* pPetObject = NULL;
	if( bCreateChar )
		pPetObject = XiahObject::g_XiahObjectManager.CreateXiahObject( dwID, OBJTYPE_PET, pCharObject);
	
	pCharObject->Create( nCharID, nMeshType, nTextureType, 1);

	if( bNpcType >= 251 )
		pCharObject->m_pAniType = XiahAniType::GetAniType( OBJTYPE_PC, 0);
	else
		pCharObject->m_pAniType = XiahAniType::GetAniType( OBJTYPE_NPC, 0);
	pCharObject->SetAngle( wDirection);
	pCharObject->SetPosition( wPosX, wPosY);

	pCharObject->SetAnimation( XiahAniType::eLAT_Stand, -1);

	if(bRevolutionStep == 4 && bNpcType == 0) //HT_0621 : 영수둔갑신단 적용
		SetupPET_VisualEquipement(pCharObject, wVisualID);

	if( bNpcType >= 251 )
		SetupPC_VisualEquipement(pCharObject, wVisualID);


	// 만약 분신이면
	// 2004.08.06 Changth
	// 분신격이 뛰니깐 분신이 뛸때도 걷는 소리가 나는데 이때 에러가 난다. 
	// 그래서 분신이 뛸때는 소리가 없게한다.
	if( bNpcType == 251 )
		pCharObject->m_CharRender.EnableAnimationSound(false);
	if (bNpcType >= 251)
	{
		// [业务设计意图]
		// 规避 sString 局部变量拼接导致的生命周期和析构野指针 Bug。
		// 使用栈上的 C 风格缓冲区 szTemp 进行名字的 sprintf 格式化拼接，再赋给 sString 触发其内部深拷贝。
		// 彻底解决野指针被破坏成“孤”并导致 D3D 渲染多字节字符时吃掉 \0 陷入死循环挂起黑屏的问题。
		// [潜在风险]
		// 拼接缓冲区大小设为 128 字节，对于游戏角色名字绰绰有余，c_str() 获取 const char* 指针。
		char szTemp[128];
		sprintf(szTemp, "%s\xb5\xc4\xb7\xd6\xc9\xed", pOwner->m_pObject->m_szObjectName.c_str());
		pCharObject->m_szObjectName = szTemp;
		szName = szTemp;
	}
	else
	{
		pCharObject->m_szObjectName = szName;
	}
	pCharObject->m_bObjType = OBJTYPE_PET;
	pCharObject->m_dwCurHP = dwHpCur;
	pCharObject->m_dwMaxHP = dwHpMax;

	sPetInfo* pPetInfo = NULL;
	
	//YS_0811 : BUGFIX	
	if( bCreateChar )
	{
		pPetInfo = new sPetInfo;
	}
	else
	{
		pPetInfo = (sPetInfo*)pCharObject->m_pPrivateData;
		if (!pPetInfo )
		{
			pPetInfo = new sPetInfo;
		}
	}

	//YS_0811 : BUGFIX
	if ( !pPetInfo )
		return 0;

	pPetInfo->dwID				=	 dwID;			
	pPetInfo->bNpcType			=	 bNpcType;
	pPetInfo->dwMapID			=	 dwMapID;
	pPetInfo->wPosX				=	 wPosX;
	pPetInfo->wPosY				=	 wPosY;
	pPetInfo->bHeight			=	 bHeight;
	pPetInfo->wDirection		=	 wDirection;
	pPetInfo->szName			=	 szName;
	pCharObject->m_bObjStatus	=	 bStatus;	
//	pPetInfo->bStatus			=	 bStatus;
	pPetInfo->wDesPosX			=	 wDesPosX;
	pPetInfo->wDesPosY			=	 wDesPosY;
	pPetInfo->bDesHeight		=	 bDesHeight;
	pPetInfo->dwHpMax			=	 dwHpMax;
	pPetInfo->dwHpCur			=	 dwHpCur;
	pPetInfo->bSpeed			=	 bSpeed;
	pPetInfo->dwOwnID			=	 dwOwnerID;
	pPetInfo->bRevolutionStep	=	 bRevolutionStep;

	// 根据 bNpcType 设置分身/幻龙标记
	// 0=普通宠物, 1=幻龙(환수유), 2=分身(분신격)
	if( bNpcType == 251 )
		pPetInfo->m_dwIsHwan = 2;  // 分身
	else if( bNpcType == 250 )
		pPetInfo->m_dwIsHwan = 1;  // 幻兽龙
	else
		pPetInfo->m_dwIsHwan = 0;  // 普通宠物


	pCharObject->m_pPrivateData = (DWORD)pPetInfo;
	pCharObject->m_PrivateDataDestoryer = ReleasePetInfo;

	if(pCharObject->m_KeepUpMugongList.IsExist(YUN_SUSINKIKANG))
	{
		pCharObject->m_CharRender.SetLocalScale(Vector3(2.0f, 2.0f, 2.0f));
	}

	// 分身或幻兽龙自动加入 g_PetList：owner是自己时注入AI，使分身/龙跟随和攻击
	if( (bNpcType == 251 || bNpcType == 250) && bCreateChar && pPetObject != NULL )
	{
		if( g_pMainChar && g_pMainChar->m_dwServerID == dwOwnerID )
		{
			pPetInfo->bAI = TRUE;
			pPetInfo->AI_Type = PETAI_AUTOATTACK; // 默认自动攻击模式
			pPetInfo->bSelected = FALSE;
			pPetInfo->bFight = FALSE;
			pPetInfo->bFollowPC = FALSE;
			pPetInfo->bIdle = FALSE;
			pPetInfo->dwDestID = 0;
			pPetInfo->dwGuardID = 0;
			pPetInfo->dwDestType = 0;
			pPetInfo->dwGuardType = 0;
			g_PetList.AddPet( pPetObject );
			DBG_Put("OnCS_NC_PETINFO_ACK: Bunsin added to PetList, ID=%d", dwID);
		}
	}
	// 如果是自己拥有的真实宠物在地图上生成，也必须加入 g_PetList，使列表和控制逻辑生效
	else if( bNpcType < 250 && bCreateChar && pPetObject != NULL )
	{
		if( g_pMainChar && g_pMainChar->m_dwServerID == dwOwnerID )
		{
			pPetInfo->bAI = TRUE;
			pPetInfo->AI_Type = PETAI_AUTOATTACK;
			pPetInfo->bSelected = FALSE;
			pPetInfo->bFight = FALSE;
			pPetInfo->bFollowPC = FALSE;
			pPetInfo->bIdle = FALSE;
			pPetInfo->dwDestID = 0;
			pPetInfo->dwGuardID = 0;
			pPetInfo->dwDestType = 0;
			pPetInfo->dwGuardType = 0;
			g_PetList.AddPet( pPetObject );
			DBG_Put("OnCS_NC_PETINFO_ACK: Real pet added to PetList, ID=%d", dwID);

			extern void UpdatePetManagerList();
			UpdatePetManagerList();
		}
	}

	return 0;
}

int OnCS_NC_PETINFOLIST_ACK(CMsg &msg)
{
	BYTE bResult;
	DWORD dwMapID;
	WORD wNumID;

	msg 
		>> bResult
		>> dwMapID
		>> wNumID;

	for(int i = 0; i < wNumID; i++)
	{
		DWORD	dwID;
		BYTE	bNpcType;
		DWORD	dwMapID;
		WORD	wPosX;
		WORD	wPosY;
		BYTE	bHeight;
		WORD	wDirection;
		sString szName;
		BYTE	bStatus;
		WORD	wDesPosX;
		WORD	wDesPosY;
		BYTE	bDesHeight;
		DWORD	dwHpMax;
		DWORD	dwHpCur;
		BYTE	bSpeed;
		DWORD	dwOwnerID;
		BYTE	bRevolutionStep;
		WORD	wVisualID[6];

		msg 
			>> dwID
			>> bNpcType
			>> dwMapID
			>> wPosX
			>> wPosY
			>> bHeight
			>> wDirection
			>> szName
			>> bStatus
			>> wDesPosX
			>> wDesPosY
			>> bDesHeight
			>> dwHpMax
			>> dwHpCur
			>> bSpeed
			>> dwOwnerID
			>> bRevolutionStep
			>> wVisualID[0]
			>> wVisualID[1]
			>> wVisualID[2]
			>> wVisualID[3]
			>> wVisualID[4]
			>> wVisualID[5];
	
		if(dwOwnerID == g_MainCharInfo.m_dwObjectID)
			continue;

		if( g_PetList.Find( dwID) != NULL)
			continue;	// 내꺼는 무시

		// 새로운 캐릭터면 첨부터 생성하지만, 존재하는 거라면 데이타를 바꿔준다.
		bool bCreateChar = true;

		XiahObject::CXiahObject* pXiahObject = XiahObject::g_XiahObjectManager.FindXiahObject( MAKEOBJECTID( 0, dwID, OBJTYPE_PET) );

		if( pXiahObject != NULL ) // 캐릭터 데이타가 있으면 new 하지 않고 데이타만 바꿔준다.
			bCreateChar = false;

		// 2004.08.05 Changth
		// 분신이 이제 이곳으로 온다. 여기서 만들어야 맵 이동할때 상대의 분신이 보인다.
		if( bNpcType == 251 )
		{
			// 오너를 찾는다
			XiahObject::CXiahObject* pOwnerObject = XiahObject::g_XiahObjectManager.FindXiahObject( MAKEOBJECTID( 0, dwOwnerID, OBJTYPE_PC));
			if( !pOwnerObject ) continue;

			CXiahCharObject* pOwnerCharObject = (CXiahCharObject*)pOwnerObject->m_pObject;

			// 이미 만들어진 객체는 데이터를 업데이트 한다. 즉 다시 만들지 않는다.
			CXiahCharObject *pBunsin = NULL;
			if( bCreateChar )
                pBunsin = new CXiahCharObject;
			else
				pBunsin = (CXiahCharObject*)pXiahObject->m_pObject;

			//YS_0811 : BUGFIX
			if ( !pBunsin )
				continue;

			pBunsin->m_pAniType = XiahAniType::GetAniType( OBJTYPE_PC, 0);

			int nCharID		= pOwnerCharObject->m_CharRender.GetCharID();
			int nMeshType	= pOwnerCharObject->m_CharRender.GetMeshType();
			int nTextureType= pOwnerCharObject->m_CharRender.GetTextureType();

			CRes_Character* pResChar = XiahGameEngine::GetCharacter( nCharID);
			if( pResChar == NULL)
			{
				delete pBunsin;
				continue;
			}
			if( !pResChar->GetMesh( nMeshType))
			{
				delete pBunsin;
				continue;
			}
			if( pResChar->GetMesh( nMeshType)->GetTexture( nTextureType) == NULL)
			{
				delete pBunsin;
				continue;
			}

			pBunsin->Create( nCharID, nMeshType, nTextureType, 103);
			pBunsin->SetPosition( wPosX, wPosY );
			pBunsin->m_szObjectName = pOwnerCharObject->m_szObjectName;	
			pBunsin->m_bObjType = OBJTYPE_PET;
			pBunsin->m_bSubObjType = pOwnerCharObject->m_bSubObjType;

			// 2004.08.06 Changth
			// 분신격이 뛰니깐 분신이 뛸때도 걷는 소리가 나는데 이때 에러가 난다. 
			// 그래서 분신이 뛸때는 소리가 없게한다.
			pBunsin->m_CharRender.EnableAnimationSound(false);

			sPCVisualInfo *pVisualInfo = (sPCVisualInfo *)pOwnerCharObject->m_pPrivateData;

			//YS_0811 : BUGFIX
			if (!pVisualInfo )	
				continue;

			SetupPC_VisualEquipement( pBunsin,pVisualInfo->wVisualID,pVisualInfo->bRarity,pVisualInfo->bStxType);

			XiahObject::CXiahObject* pPetObject = NULL;
			if( bCreateChar )
                pPetObject = XiahObject::g_XiahObjectManager.CreateXiahObject( dwID, OBJTYPE_PET, pBunsin);
			else
				pPetObject = pXiahObject;

			//YS_0811 : BUGFIX
			if ( !pPetObject )
				continue;

			pBunsin->m_dwOwnerID = dwOwnerID;

			// 내꺼일경우에만 PETlist에 넣는다
			if(g_pMainChar->m_dwServerID == dwOwnerID)
			{
				// PET의 정보 설정
				sPetInfo* pPetInfo = NULL;
				
				//YS_0811 : BUGFIX				
				if( bCreateChar )
				{
                    pPetInfo = new sPetInfo;
				}
				else
				{
					pPetInfo = (sPetInfo*)pBunsin->m_pPrivateData;
					
					if  ( !pPetInfo )
						pPetInfo = new sPetInfo;
				}

				
				//YS_0811 : BUGFIX
				if ( !pPetInfo )
					continue;

				pPetInfo->m_dwIsHwan = 2;	// 2번은 분신격

				pPetInfo->dwID				= dwID;				
				pPetInfo->dwOwnID			= dwOwnerID;				
				pPetInfo->dwMapID			= dwMapID;				
				pPetInfo->bNpcType			= bNpcType;				
				pPetInfo->szName			= szName;				
//				pPetInfo->wLevel			= wLevel;				
				pPetInfo->wPosX				= wPosX;				
				pPetInfo->wPosY				= wPosY;				
				pPetInfo->bHeight			= bHeight;				
				pPetInfo->wDesPosX			= wDesPosX;				
				pPetInfo->wDesPosY			= wDesPosY;				
				pPetInfo->bDesHeight		= bDesHeight;				
				pPetInfo->wDirection		= wDirection;				
				pPetInfo->dwHpMax			= dwHpMax;				
				pPetInfo->dwHpCur			= dwHpCur;				
//				pPetInfo->wAtkPwr			= wAtkPwr;				
//				pPetInfo->wDefPwr			= wDefPwr;				
//				pPetInfo->wAtkRating		= wAtkRating;
//				pPetInfo->wAvoidRatio		= wAvoidRatio;				
				pPetInfo->bSpeed			= bSpeed;				
//				pPetInfo->wMeleeAtkRange	= wMeleeAtkRange;
//				pPetInfo->wShotAtkRange		= wShotAtkRange;				
//				pPetInfo->bAtkType			= bAtkType;				
//				pPetInfo->dwRefNpcID		= dwRefNpcID;				
//				pPetInfo->bCurJob			= bCurJob;				
//				pPetInfo->i64Exp			= i64Exp;
//				pPetInfo->i64LevelExp		= i64LevelExp;	
//				pPetInfo->i64NextLevelUpExp	= i64NextLevelUpExp;
				pPetInfo->bRevolutionStep	= bRevolutionStep;				
//				pPetInfo->bWildRate			= bWildRate;
				pPetInfo->bAI = TRUE;						// AI ON
				pPetInfo->AI_Type = PETAI_AUTOATTACK;		// 이것은 항상 Auto Attack

				pBunsin->m_pPrivateData = (DWORD)pPetInfo;
				pBunsin->m_PrivateDataDestoryer = ReleasePetInfo;	
				
				if( bCreateChar )
				{
					//YS_0812 : BUGFIX
					if ( pPetObject->m_pObject && pPetObject->m_pObject->m_bPoolClass )
					{
						pPetObject->m_pObject->m_bPetPool = true;
					}
                    
                    g_PetList.AddPet( pPetObject);
				}
			}

			continue;
		}
		else 
		if( bNpcType == 250 )	// 환수유
		{

		}

		// 일반 펫
		sArrayData* pData = XiahArrayIndex::g_NpcType.GetData( bNpcType);
		
		if( pData == NULL)
		{
		//	DBG_LogFile( "줸장 : %d", bNpcType);
			continue;
		}

		int nCharID = pData->GetInt( 1);
		int nMeshType = pData->GetInt( 2);
		int nTextureType = pData->GetInt( 3);

		if(bRevolutionStep == 4 && bNpcType == 0) //HT_0621 : 영수둔갑신단 적용)
		{
			// [10/31/2005] 캐릭터 ID변경
			nCharID = 1133;

			if(wVisualID[5])
			{
				nMeshType = 1;
				nTextureType = wVisualID[5] - 27700;
			}
		}

		if( XiahGameEngine::GetCharacter( nCharID) == NULL)
		{
		//	DBG_LogFile( "줸장 : %d", bNpcType);
			continue;
		}

		CRes_Character* pResChar = XiahGameEngine::GetCharacter( nCharID);

		if( pResChar == NULL)
		{
		//	DBG_LogFile( "줸장 : %d", bNpcType);
			continue;
		}
		
		if( !pResChar->GetMesh( nMeshType))
		{
		//	DBG_LogFile( "줸장 : %d", bNpcType);
			continue;
		}

		if( pResChar->GetMesh( nMeshType)->GetTexture( nTextureType) == NULL)
		{
		//	DBG_LogFile( "줸장 : %d", bNpcType);
			continue;
		}

		//YS_0805 : BUGFIX
		CXiahCharObject* pCharObject = NULL;
		if( bCreateChar )
		{
			pCharObject = (CXiahCharObject *)XiahObject::g_XiahPetPool.GetChar(); //new CXiahCharObject;

			if ( NULL == pCharObject )
			{
				pCharObject = new CXiahCharObject;
			}
		}
		else
		{
			pCharObject = reinterpret_cast<CXiahCharObject*>(pXiahObject->m_pObject);
		}

		XiahObject::CXiahObject* pPetObject = NULL;
		if( bCreateChar )
			pPetObject = XiahObject::g_XiahObjectManager.CreateXiahObject( dwID, OBJTYPE_PET, pCharObject);

		//YS_0811 : BUGFIX
		if ( !pPetObject )
			continue;

		pCharObject->Create( nCharID, nMeshType, nTextureType, 1);

		pCharObject->m_pAniType = XiahAniType::GetAniType( OBJTYPE_NPC, 0);
		pCharObject->SetAngle( wDirection);
		pCharObject->SetPosition( wPosX, wPosY);

		pCharObject->SetAnimation( XiahAniType::eLAT_Stand, -1);

		if(bRevolutionStep == 4 && bNpcType == 0) //HT_0621 : 영수둔갑신단 적용
			SetupPET_VisualEquipement(pCharObject, wVisualID);

		pCharObject->m_szObjectName = szName;
		pCharObject->m_bObjType = OBJTYPE_PET;
		pCharObject->m_dwCurHP = dwHpCur;
		pCharObject->m_dwMaxHP = dwHpMax;

		sPetInfo* pPetInfo = NULL;
		if( bCreateChar )
		{
			pPetInfo = new sPetInfo;
		}
		else
		{
			pPetInfo = (sPetInfo*)pCharObject->m_pPrivateData;
			
			//YS_0811 : BUGFIX
			if ( !pPetInfo )
				pPetInfo = new sPetInfo;
		}

		//YS_0811 : BUGFIX
		if ( !pPetInfo )
			continue;

		pPetInfo->dwID				=	 dwID;			
		pPetInfo->bNpcType			=	 bNpcType;
		pPetInfo->dwMapID			=	 dwMapID;
		pPetInfo->wPosX				=	 wPosX;
		pPetInfo->wPosY				=	 wPosY;
		pPetInfo->bHeight			=	 bHeight;
		pPetInfo->wDirection		=	 wDirection;
		pPetInfo->szName			=	 szName;
		pCharObject->m_bObjStatus	=	 bStatus;	
	//	pPetInfo->bStatus			=	 bStatus;
		pPetInfo->wDesPosX			=	 wDesPosX;
		pPetInfo->wDesPosY			=	 wDesPosY;
		pPetInfo->bDesHeight		=	 bDesHeight;
		pPetInfo->dwHpMax			=	 dwHpMax;
		pPetInfo->dwHpCur			=	 dwHpCur;
		pPetInfo->bSpeed			=	 bSpeed;
		pPetInfo->dwOwnID			=	 dwOwnerID;
		pPetInfo->bRevolutionStep	=	 bRevolutionStep;
		pPetInfo->m_dwIsHwan		=	 0;

		if( bNpcType == 250 )	// 환수유
			pPetInfo->m_dwIsHwan		= 1;

		pCharObject->m_pPrivateData = (DWORD)pPetInfo;
		pCharObject->m_PrivateDataDestoryer = ReleasePetInfo;

		if(pCharObject->m_KeepUpMugongList.IsExist(YUN_SUSINKIKANG))
		{
			pCharObject->m_CharRender.SetLocalScale(Vector3(2.0f, 2.0f, 2.0f));
		}
	}

	return 0;
}

int OnCS_NC_PETDETAILINFO_ACK(CMsg &msg)
{
	BYTE bResult		=0;
	DWORD dwID			=0;
	DWORD dwOwnID		=0;
	DWORD dwMapID		=0;
	BYTE  bNpcType		=0;
	sString szName;
	WORD  wLevel		=0;
	WORD  wPosX			=0;
	WORD  wPosY			=0;
	BYTE  bHeight		=0;
	WORD  wDesPosX		=0;
	WORD  wDesPosY		=0;
	BYTE  bDesHeight	=0;
	WORD  wDirection	=0;
	DWORD  dwHpMax		=0;
	DWORD  dwHpCur		=0;
	WORD  wAtkPwr		=0;
	WORD  wDefPwr		=0;
	WORD  wAtkRating	=0;
	WORD  wAvoidRatio	=0;
	BYTE  bSpeed		=0;
	WORD  wMeleeAtkRange=0;
	WORD  wShotAtkRange =0;
	BYTE  bAtkType		=0;
	DWORD dwRefNpcID	=0;
	BYTE  bCurJob		=0;
	INT64 i64Exp		=0;
	INT64 i64LevelExp	=0;
	INT64 i64NextLevelUpExp	=0;
	BYTE  bRevolutionStep	=0;
	BYTE bWildRate		=0;
	WORD  wVisualID[6];
	

	msg
		>> bResult
		>> dwID
		>> dwOwnID
		>> dwMapID
		>> bNpcType
		>> szName
		>> wLevel
		>> wPosX
		>> wPosY
		>> bHeight
		>> wDesPosX
		>> wDesPosY
		>> bDesHeight
		>> wDirection
		>> dwHpMax
		>> dwHpCur
		>> wAtkPwr
		>> wDefPwr
		>> wAtkRating
		>> wAvoidRatio
		>> bSpeed
		>> wMeleeAtkRange
		>> wShotAtkRange
		>> bAtkType
		>> dwRefNpcID
		>> bCurJob
		>> i64Exp
		>> i64LevelExp
		>> i64NextLevelUpExp
		>> bRevolutionStep
		>> bWildRate
		>> wVisualID[0]
		>> wVisualID[1]
		>> wVisualID[2]
		>> wVisualID[3]
		>> wVisualID[4]
		>> wVisualID[5];
		

	// detail정보를 받았으니 세팅해준다.
	// 이때 DetailInfo는 새로 꼬실경우에 해당되므로 sPetInfo를 새로 만들어 줘야 한다
	/*
		꼬시게 되면
	*/
	if( g_PetList.Find( dwID) == NULL)
		return 0;

	XiahObject::CXiahObject* pObject = g_PetList.Find( dwID);

	/*
		일단 CHGOWNER_ACK가 오면 PetList에 추가되면서
		sPetInfo가 들어간다 가라로.
	*/

	//YS_0811 : BUGFIX
	if ( !pObject->m_pObject )
		return 0;

	sPetInfo* pPetInfo = (sPetInfo*)pObject->m_pObject->m_pPrivateData;

	//YS_0811 : BUGFIX
	if ( !pPetInfo )
		return 0;		
	
	pPetInfo->dwID				= dwID;				
	pPetInfo->dwOwnID			= dwOwnID;				
	pPetInfo->dwMapID			= dwMapID;				
	pPetInfo->bNpcType			= bNpcType;				
	pPetInfo->szName			= szName;				
	pPetInfo->wLevel			= wLevel;				
	pPetInfo->wPosX				= wPosX;				
	pPetInfo->wPosY				= wPosY;				
	pPetInfo->bHeight			= bHeight;				
	pPetInfo->wDesPosX			= wDesPosX;				
	pPetInfo->wDesPosY			= wDesPosY;				
	pPetInfo->bDesHeight		= bDesHeight;				
	pPetInfo->wDirection		= wDirection;				
	pPetInfo->dwHpMax			= dwHpMax;				
	pPetInfo->dwHpCur			= dwHpCur;				
	pPetInfo->wAtkPwr			= wAtkPwr;				
	pPetInfo->wDefPwr			= wDefPwr;				
	pPetInfo->wAtkRating		= wAtkRating;
	pPetInfo->wAvoidRatio		= wAvoidRatio;				
	pPetInfo->bSpeed			= bSpeed;				
	pPetInfo->wMeleeAtkRange	= wMeleeAtkRange;				
	pPetInfo->wShotAtkRange		= wShotAtkRange;				
	pPetInfo->bAtkType			= bAtkType;				
	pPetInfo->dwRefNpcID		= dwRefNpcID;				
	pPetInfo->bCurJob			= bCurJob;				
	pPetInfo->i64Exp			= i64Exp;
	pPetInfo->i64LevelExp		= i64LevelExp;	
	pPetInfo->i64NextLevelUpExp	= i64NextLevelUpExp;
	pPetInfo->bRevolutionStep	= bRevolutionStep;				
	pPetInfo->bWildRate			= bWildRate;
	pPetInfo->bAI				= TRUE; // 이제서야 AI를 탄다
	pPetInfo->AI_Type			= PETAI_AUTOATTACK;
	pPetInfo->m_dwIsHwan		= 0;

	pObject->m_pObject->m_pPrivateData = (DWORD)pPetInfo;
	pObject->m_pObject->m_PrivateDataDestoryer = ReleasePetInfo;

	g_MainCharInfo.RefreshPetInfo();

	return 0;	
}

// 애완동물 테이밍
int OnCS_NC_TAMING_ACK(CMsg &msg)
{
	BYTE	bResult		=0;	
	BYTE	bType		=0;
	DWORD	dwObjectID	=0;

	msg >> bResult
		>> dwObjectID
		>> bType;

	if( bResult == 0)
	{
		if( g_PetList.Find( dwObjectID) != NULL)
		{
			DBG_LogFile(_T("이상한 애완동물"));
		}

		XiahObject::CXiahObject* pObject = XiahObject::g_XiahObjectManager.FindXiahObject( MAKEOBJECTID( 0, dwObjectID, OBJTYPE_PET));

		if( pObject->m_pObject->m_pPrivateData != 0)
		{
			pObject->m_pObject->m_PrivateDataDestoryer( pObject->m_pObject->m_pPrivateData);
			pObject->m_pObject->m_pPrivateData = 0;
		}

		// PET의 정보 설정
		sPetInfo* pPetInfo = new sPetInfo;
		
		pPetInfo->dwID = dwObjectID;
		// PET의 Info Structure
		pObject->m_pObject->m_pPrivateData = (DWORD)pPetInfo;
		pObject->m_pObject->m_PrivateDataDestoryer = ReleasePetInfo;

		//YS_0812 : BUGFIX
		if ( pObject->m_pObject && pObject->m_pObject->m_bPoolClass )
		{
			pObject->m_pObject->m_bPetPool = true;
		}

		g_PetList.AddPet( pObject);	// 일단 애완동물로 추가해 준다

		// 지금만든 PET의 세부정보 요청
		SendCS_NC_PETDETAILINFO_REQ( dwObjectID);
		g_MainCharInfo.ShowHelpMessage( IDS_SUCC_TAME);
		g_MainCharInfo.PlayInterfaceSound( SUCC_TAMMING_SOUND );
	}
	else
	{
		switch( bResult)
		{
		case 1:
			g_MainCharInfo.ShowHelpMessage( IDS_NO_KIND_MON, TEXTEFFECT_COLOR_WARNING);
			break;
		case 2:
			g_MainCharInfo.ShowHelpMessage( IDS_LONG_DISTANCE_FAIL, TEXTEFFECT_COLOR_WARNING);
			break;
		case 3:
			g_MainCharInfo.ShowHelpMessage( IDS_NOMORE_PET, TEXTEFFECT_COLOR_WARNING);
			break;
		case 4:
			g_MainCharInfo.ShowHelpMessage( IDS_FAILE_TAME, TEXTEFFECT_COLOR_WARNING);
			break;
		case 5:
			g_MainCharInfo.ShowHelpMessage( IDS_FAILE_TAME, TEXTEFFECT_COLOR_WARNING);
			break;
		case 6:
			g_MainCharInfo.ShowHelpMessage(IDS_TAMING_NOTTAMING, TEXTEFFECT_COLOR_WARNING);
			break;
		case 7:
			g_MainCharInfo.ShowHelpMessage(IDS_TAMING_LOWLEVEL, TEXTEFFECT_COLOR_WARNING);
			break;
		}
	}

	return 0;	
}

int OnCS_NC_CHGOWNER_ACK(CMsg &msg)
{
	DWORD dwSourceID;
	BYTE  bSourceType;
	DWORD dwSourceOwnerID;
	DWORD dwTargetID;
	BYTE  bTargetType;
	DWORD dwTargetOwnerID;
	sString szName;
	
	msg
		>> dwSourceID
		>> bSourceType
		>> dwSourceOwnerID
		>> dwTargetID
		>> bTargetType
		>> dwTargetOwnerID
		>> szName;

	// 
	if( g_pMainChar == NULL)
		return 0;

	if( dwTargetOwnerID != g_pMainChar->m_dwServerID)
		return 0;

	// 이렇게 전환만 해놓아도 좋다
	XiahObject::g_XiahObjectManager.ChangeObjectID( 0, dwSourceID, bSourceType, 0, dwTargetID, bTargetType);
	
	return 0;
}

int OnCS_NC_NPCHP_ACK(CMsg &msg)
{
	DWORD dwObjectID;
	BYTE  bObjectType;
	DWORD  dwCurHP;
	DWORD  dwMaxHP;
	BYTE  bType = 0;

	msg >> dwObjectID
		>> bObjectType
		>> dwCurHP
		>> dwMaxHP
		>> bType;
	
	XiahObject::CXiahObject* pObject = XiahObject::g_XiahObjectManager.FindXiahObject( MAKEOBJECTID( 0, dwObjectID, bObjectType));
	if( pObject == NULL)
		return 0;

	CXiahCharObject* pCharObject = (CXiahCharObject*)pObject->m_pObject;
	if( pCharObject == NULL )
		return 0;

	pCharObject->m_dwCurHP = dwCurHP;
	pCharObject->m_dwMaxHP = dwMaxHP;

	switch( bType )
	{
	case 0:		// 자동 회복
		break;
	case 11:	// 무공 회복.
		{
			_EFFECTPACKAGE* pEffectPackage = g_EffectManager.EnqOutGongPersistEffectImmediately( eJunuoum_recv );

			if( pEffectPackage && pEffectPackage->pEffectRender && pEffectPackage->pEffectRender->pPackagePair )
			{
				pEffectPackage->pEffectRender->pPackagePair->pWorldMatrix = (MATRIX*)pCharObject->m_CharRender.GetCharTM();
//				pEffectPackage->pEffectRender->pPackagePair->WorldMatrix = (MATRIX)pCharObject->m_ObjectTM;

				// 이케해야 캐릭터가 없어질때 이펙트도 같이 없어진다.
				pCharObject->m_EffectPPList.push_back( pEffectPackage->pEffectRender->pPackagePair );
			}// if
		}
		break;
	};// switch

/*
	sString str;
	str.printf("NPCHP_ACK %s %d %d", (LPCTSTR)pCharObject->m_szObjectName, wCurHP, wMaxHP);

	g_MainCharInfo.ShowHelpMessage( str);*/
	return 0;
}

/**
 * 펫이름변경
 * \param &msg 
 * \return 
 */
int OnCS_NC_PETRENAME_ACK(CMsg &msg)
{
	BYTE bResult		=0;
	DWORD dwObjectID	=0;
	sString szPetName;

	msg
		>> bResult
		>> dwObjectID
		>> szPetName;

	if(bResult)
	{
		g_MainCharInfo.ShowHelpMessage(IDS_PETNAME_FAIL);

		return 0;
	}

	XiahObject::CXiahObject* pObject = g_PetList.Find( dwObjectID);

	//YS_0812 : BUGFIX
	if( pObject && pObject->m_pObject )
	{
		sPetInfo* pPetInfo = (sPetInfo *)pObject->m_pObject->m_pPrivateData;

		if( pPetInfo)
		{
			TCHAR content[128] = {0,};
			_stprintf( content, IDS_D_CHANGE_PET_NAME, (LPCTSTR)szPetName);

			g_MainCharInfo.ShowHelpMessage( content);

			pPetInfo->szName = szPetName;
			pObject->m_pObject->m_szObjectName = szPetName;

			g_MainCharInfo.RefreshPetInfo();
		}
	}
	
	return 0;
}

int OnCS_NC_PETLEVELUP_ACK(CMsg &msg)
{
	DWORD dwOwnID;
	DWORD dwID;
	BYTE bLevel;
	WORD wAtkPwr;
	WORD wDefPwr;
	WORD wAtkRating;
	DWORD dwHpCur;
	DWORD dwHpMax;
	WORD wAvoidRatio;
	
	INT64 i64Exp;
	INT64 i64LevelExp;
	INT64 i64NextLevelUpExp;

	msg
		>> dwOwnID
		>> dwID
		>> bLevel
		>> wAtkPwr
		>> wDefPwr
		>> wAtkRating
		>> wAvoidRatio
		>> dwHpCur
		>> dwHpMax
		>> i64Exp
		>> i64LevelExp
		>> i64NextLevelUpExp;

	XiahObject::CXiahObject* pObject = g_PetList.Find( dwID);

	//YS_0812 : BUGFIX
	if( pObject && pObject->m_pObject )
	{
		sPetInfo* pPetInfo = (sPetInfo *)pObject->m_pObject->m_pPrivateData;

		if( pPetInfo)
		{
			pPetInfo->wLevel = bLevel;
			pPetInfo->wAtkPwr = wAtkPwr;
			pPetInfo->wDefPwr = wDefPwr;
			pPetInfo->wAtkRating = wAtkRating;
			pPetInfo->wAvoidRatio = wAvoidRatio;
			pPetInfo->dwHpCur = dwHpCur;
			pPetInfo->dwHpMax = dwHpMax;
			pPetInfo->i64Exp = i64Exp;
			pPetInfo->i64LevelExp = i64LevelExp;
			pPetInfo->i64NextLevelUpExp = i64NextLevelUpExp;

			g_MainCharInfo.RefreshPetInfo();
			TCHAR temp[50];
			_stprintf( temp, IDS_LEVEL_UP_MONSTER,  (LPCTSTR)pPetInfo->szName, bLevel);
			g_MainCharInfo.ShowHelpMessage( temp,TEXTEFFECT_COLOR_GAIN);
		}
	}

	return 0;
}

int OnCS_NC_PETEXP_ACK(CMsg &msg)
{
	DWORD dwPetID;
	DWORD dwIncrExp;

	msg
		>> dwPetID
		>> dwIncrExp;

	XiahObject::CXiahObject* pObject = g_PetList.Find( dwPetID);

	//YS_0812 : BUGFIX
	if( pObject && pObject->m_pObject )
	{
		sPetInfo* pPetInfo = (sPetInfo *)pObject->m_pObject->m_pPrivateData;

		if( pPetInfo)
		{
			//HT_0625 : 펫 경험치 150갑자 일 경우
			if(pPetInfo->wLevel != 150)
			{
				pPetInfo->i64Exp += dwIncrExp;

				TCHAR temp[50];
				
				_stprintf( temp, IDS_EXP_UP_MONSTER, (LPCTSTR)pPetInfo->szName, dwIncrExp);
				g_MainCharInfo.ShowHelpMessage( temp,TEXTEFFECT_COLOR_GAIN);
				g_MainCharInfo.RefreshPetInfo();
			}
		}
	}

	return 0;
}

int OnCS_NC_PETHP_ACK(CMsg &msg)
{
	DWORD dwPetID;
	DWORD dwHpCur;
	DWORD dwHpMax;
	BYTE bType = 0;

	msg
		>> dwPetID
		>> dwHpCur
		>> dwHpMax
		>> bType;

	XiahObject::CXiahObject* pObject = g_PetList.Find( dwPetID);

	if( pObject && pObject->m_pObject )
	{
		sPetInfo* pPetInfo = (sPetInfo *)pObject->m_pObject->m_pPrivateData;

		if( pPetInfo)
		{
			pPetInfo->dwHpCur = dwHpCur;
			pPetInfo->dwHpMax = dwHpMax;
			pObject->m_pObject->m_dwCurHP = dwHpCur;
			pObject->m_pObject->m_dwMaxHP = dwHpMax;
			g_MainCharInfo.RefreshPetInfo();

			CXiahCharObject* pCharObject = (CXiahCharObject*)pObject->m_pObject;
			if( pCharObject == NULL )
				return 0;

			// 얘두 이펙트다. OnCS_IF_CHARHP_ACK 하구 똑~ 같다.
			switch( bType )
			{
			case 0:		// 자동 회복
				break;
				// 물약 사용
			case 1:		// HP
				{
					_EFFECTPACKAGE* pEffectPackage = g_EffectManager.EnqAppearEffectImmediately( eMulYak_HP );

					if( pEffectPackage && pEffectPackage->pEffectRender && pEffectPackage->pEffectRender->pPackagePair )
					{
						pEffectPackage->pEffectRender->pPackagePair->pWorldMatrix = (MATRIX*)pCharObject->m_CharRender.GetCharTM();
//						pEffectPackage->pEffectRender->pPackagePair->WorldMatrix = (MATRIX)pCharObject->m_ObjectTM;

						// 이케해야 캐릭터가 없어질때 이펙트도 같이 없어진다.
						pCharObject->m_EffectPPList.push_back( pEffectPackage->pEffectRender->pPackagePair );
					}
				}
				break;
			case 2:		// IP
				{
					_EFFECTPACKAGE* pEffectPackage = g_EffectManager.EnqAppearEffectImmediately( eMulYak_IP );

					if( pEffectPackage && pEffectPackage->pEffectRender && pEffectPackage->pEffectRender->pPackagePair )
					{
						pEffectPackage->pEffectRender->pPackagePair->pWorldMatrix = (MATRIX*)pCharObject->m_CharRender.GetCharTM();
//						pEffectPackage->pEffectRender->pPackagePair->WorldMatrix = (MATRIX)pCharObject->m_ObjectTM;

						pCharObject->m_EffectPPList.push_back( pEffectPackage->pEffectRender->pPackagePair );
					}
				}
				break;
			case 3:		// HP, IP
				{
					_EFFECTPACKAGE* pEffectPackage = g_EffectManager.EnqAppearEffectImmediately( eMulYak_HPIP );

					if( pEffectPackage && pEffectPackage->pEffectRender && pEffectPackage->pEffectRender->pPackagePair )
					{
						pEffectPackage->pEffectRender->pPackagePair->pWorldMatrix = (MATRIX*)pCharObject->m_CharRender.GetCharTM();
//						pEffectPackage->pEffectRender->pPackagePair->WorldMatrix = (MATRIX)pCharObject->m_ObjectTM;

						pCharObject->m_EffectPPList.push_back( pEffectPackage->pEffectRender->pPackagePair );
					}
				}
				break;
				// 무공
			case 11:	// 전유음 - 타인 치료.
				{
					_EFFECTPACKAGE* pEffectPackage = g_EffectManager.EnqOutGongPersistEffectImmediately( eJunuoum_recv );

					if( pEffectPackage && pEffectPackage->pEffectRender && pEffectPackage->pEffectRender->pPackagePair )
					{
                        pEffectPackage->pEffectRender->pPackagePair->pWorldMatrix = (MATRIX*)pCharObject->m_CharRender.GetCharTM();
//						pEffectPackage->pEffectRender->pPackagePair->WorldMatrix = (MATRIX)pCharObject->m_ObjectTM;

						pCharObject->m_EffectPPList.push_back( pEffectPackage->pEffectRender->pPackagePair );
					}// if
				}
				break;
			case 12:	// 이광음 - 단 치료.
				{
					_EFFECTPACKAGE* pEffectPackage = g_EffectManager.EnqOutGongPersistEffectImmediately( eLeekwangum_heal_recv );

					if( pEffectPackage && pEffectPackage->pEffectRender && pEffectPackage->pEffectRender->pPackagePair )
					{
						pEffectPackage->pEffectRender->pPackagePair->pWorldMatrix = (MATRIX*)pCharObject->m_CharRender.GetCharTM();
//						pEffectPackage->pEffectRender->pPackagePair->WorldMatrix = (MATRIX)pCharObject->m_ObjectTM;

						pCharObject->m_EffectPPList.push_back( pEffectPackage->pEffectRender->pPackagePair );
					}// if
				}
				break;
			};

		}// if
	}// if

	return 0;
}


// 애완동물 봉인
int OnCS_NC_PETBONGIN_ACK(CMsg &msg)
{
	BYTE bResult	=0;
	DWORD dwPetID	=0;

	msg
		>> bResult
		>> dwPetID;

	if( !bResult)
	{
		g_PetList.DeletePet(dwPetID);
		g_MainCharInfo.ShowHelpMessage(IDS_PET_BONGIN_COMPLETE);
	}
	else
	{
		switch( bResult)
		{
		case 10:
			g_MainCharInfo.ShowHelpMessage(_T("\xb3\xf6\xd5\xbd\xd6\xd0\xb5\xc4\xb3\xe8\xce\xef\xce\xde\xb7\xa8\xb7\xe2\xd3\xa1\xa3\xac\xc7\xeb\xcf\xc8\xd5\xd9\xbb\xd8"), TEXTEFFECT_COLOR_WARNING);
			break;
		//봉인할수 없슴
		case ERR_PETBONGIN_NOTFINDPET:
			g_MainCharInfo.ShowHelpMessage(IDS_PETBONGIN_NOTFINDPET, TEXTEFFECT_COLOR_WARNING);
			break;
		// 봉인단지를 못찾음.
		case ERR_PETBONGIN_NOTFINDITEM:
			g_MainCharInfo.ShowHelpMessage(IDS_ITEM_NOTFIND, TEXTEFFECT_COLOR_WARNING);
			break;
		case ERR_PETBONGIN_HASITEM:
			g_MainCharInfo.ShowHelpMessage(IDS_CANNOT_BONGIN_ITEMMONSTER,TEXTEFFECT_COLOR_WARNING);
			break;
		case ERR_PETBONGIN_LEVELGAP:
			g_MainCharInfo.ShowHelpMessage(IDS_CANNOT_BONGIN_LEVEL, TEXTEFFECT_COLOR_WARNING);
			break;
		case ERR_PETBONGIN_INVALIDITEM:
			g_MainCharInfo.ShowHelpMessage(IDS_ITEM_NOTFIND, TEXTEFFECT_COLOR_WARNING);
			break;
		case ERR_PETBONGIN_LIMITLEVEL: //갑자 제한(착용,봉인,해제할때)
			break;
		case ERR_PETBONGIN_LIMITCOUNT: //봉인회수 제한(빙정의 경우 3마리까지 봉인가능)
			break;
		case 8:
			g_MainCharInfo.ShowHelpMessage(IDS_PETBONGIN_NOTBONGIN, TEXTEFFECT_COLOR_WARNING);
			break;
		}
	}

	return 0;
}

// 애완동물의 봉인을 풀어준다
int OnCS_NC_PETBONGOUT_ACK(CMsg &msg)
{
	BYTE bResult;

	msg
		>> bResult;

	if( !bResult)
	{
		DWORD dwID;
		DWORD dwOwnID;
		DWORD dwMapID;
		BYTE  bNpcType;
		sString szName;
		WORD  wLevel;
		WORD  wPosX;
		WORD  wPosY;
		BYTE  bHeight;
		WORD  wDesPosX;
		WORD  wDesPosY;
		BYTE  bDesHeight;
		WORD  wDirection;
		DWORD  dwHpMax;
		DWORD  dwHpCur;
		WORD  wAtkPwr;
		WORD  wDefPwr;
		WORD  wAtkRating;
		WORD  wAvoidRatio;
		BYTE  bSpeed;
		WORD  wMeleeAtkRange;
		WORD  wShotAtkRange;
		BYTE  bAtkType;
		DWORD dwRefNpcID;
		BYTE  bCurJob;
		INT64 i64Exp;
		INT64 i64LevelExp;
		INT64 i64NextLevelUpExp;
		BYTE  bRevolutionStep;
		BYTE  bWildRate;
		WORD  wVisualID[6];
		
		msg
			>> dwID
			>> dwOwnID
			>> dwMapID
			>> bNpcType
			>> szName
			>> wLevel
			>> wPosX
			>> wPosY
			>> bHeight
			>> wDesPosX
			>> wDesPosY
			>> bDesHeight
			>> wDirection
			>> dwHpMax
			>> dwHpCur
			>> wAtkPwr
			>> wDefPwr
			>> wAtkRating
			>> wAvoidRatio
			>> bSpeed
			>> wMeleeAtkRange
			>> wShotAtkRange
			>> bAtkType
			>> dwRefNpcID
			>> bCurJob
			>> i64Exp
			>> i64LevelExp
			>> i64NextLevelUpExp
			>> bRevolutionStep
			>> bWildRate
			>> wVisualID[0]
			>> wVisualID[1]
			>> wVisualID[2]
			>> wVisualID[3]
			>> wVisualID[4]
			>> wVisualID[5];

		if (wPosX == 0 && wPosY == 0)
		{
			g_MainCharInfo.ShowHelpMessage(IDS_PET_BONGOUT);
			return 0;
		}

		// 만들어 주기
		if( XiahObject::g_XiahObjectManager.FindXiahObject( MAKEOBJECTID( 0, dwID, OBJTYPE_PET)) != NULL)
		{	
			// 음.. 설마..
			return 0;
		}

		sArrayData* pData = XiahArrayIndex::g_NpcType.GetData( bNpcType);
		
		if( pData == NULL)
			//continue;
			return 0;

		int nCharID = pData->GetInt( 1);
		int nMeshType = pData->GetInt( 2);
		int nTextureType = pData->GetInt( 3);

		if(bRevolutionStep == 4 && bNpcType == 0) //HT_0621 : 영수둔갑신단 적용)
		{
			// [10/31/2005] 캐릭터 ID변경
			nCharID = 1133;

			if(wVisualID[5])
			{
				nMeshType = 1;
				nTextureType = wVisualID[5] - 27700;
			}
		}

		if( XiahGameEngine::GetCharacter( nCharID) == NULL)
			//continue;
			return 0;			

		//YS_0805 : BUGFIX
		CXiahCharObject* pCharObject = (CXiahCharObject *)XiahObject::g_XiahPetPool.GetChar(); //new CXiahCharObject;

		if ( NULL == pCharObject )
		{
			pCharObject = new CXiahCharObject;
		}

		XiahObject::CXiahObject* pPetObject = XiahObject::g_XiahObjectManager.CreateXiahObject( dwID, OBJTYPE_PET, pCharObject);

		
		pCharObject->Create( nCharID, nMeshType, nTextureType, 1);

		pCharObject->m_pAniType = XiahAniType::GetAniType( OBJTYPE_NPC, 0);
		pCharObject->SetAngle( wDirection);
		pCharObject->SetPosition( wPosX, wPosY);

		pCharObject->SetAnimation( XiahAniType::eLAT_Stand, -1);

		if(bRevolutionStep == 4 && bNpcType == 0) //HT_0621 : 영수둔갑신단 적용
			SetupPET_VisualEquipement(pCharObject, wVisualID);

		pCharObject->m_szObjectName = szName;
		pCharObject->m_bObjType = OBJTYPE_PET;
		pCharObject->m_dwCurHP = dwHpCur;
		pCharObject->m_dwMaxHP = dwHpMax;
		
		sPetInfo* pPetInfo = new sPetInfo;

		//ZeroMemory( pPetInfo, sizeof( sPetInfo));
		pPetInfo->dwID				= dwID;				
		pPetInfo->dwOwnID			= dwOwnID;				
		pPetInfo->dwMapID			= dwMapID;				
		pPetInfo->bNpcType			= bNpcType;				
		pPetInfo->szName			= szName;				
		pPetInfo->wLevel			= wLevel;				
		pPetInfo->wPosX				= wPosX;				
		pPetInfo->wPosY				= wPosY;				
		pPetInfo->bHeight			= bHeight;				
		pPetInfo->wDesPosX			= wDesPosX;				
		pPetInfo->wDesPosY			= wDesPosY;				
		pPetInfo->bDesHeight		= bDesHeight;				
		pPetInfo->wDirection		= wDirection;				
		pPetInfo->dwHpMax			= dwHpMax;				
		pPetInfo->dwHpCur			= dwHpCur;				
		pPetInfo->wAtkPwr			= wAtkPwr;				
		pPetInfo->wDefPwr			= wDefPwr;				
		pPetInfo->wAtkRating		= wAtkRating;
		pPetInfo->wAvoidRatio		= wAvoidRatio;				
		pPetInfo->bSpeed			= bSpeed;				
		pPetInfo->wMeleeAtkRange	= wMeleeAtkRange;				
		pPetInfo->wShotAtkRange		= wShotAtkRange;				
		pPetInfo->bAtkType			= bAtkType;				
		pPetInfo->dwRefNpcID		= dwRefNpcID;				
		pPetInfo->bCurJob			= bCurJob;				
		pPetInfo->i64Exp			= i64Exp;				
		pPetInfo->i64LevelExp		= i64LevelExp;
		pPetInfo->i64NextLevelUpExp	= i64NextLevelUpExp;
		pPetInfo->bRevolutionStep	= bRevolutionStep;				
		pPetInfo->bWildRate			= bWildRate;

		pPetInfo->bAI = TRUE;
		pPetInfo->AI_Type = PETAI_AUTOATTACK;
		pPetInfo->m_dwIsHwan		= 0;

		pCharObject->m_pPrivateData = (DWORD)pPetInfo;
		pCharObject->m_PrivateDataDestoryer = ReleasePetInfo;
		
		//YS_0812 : BUGFIX
		if ( pPetObject->m_pObject && pPetObject->m_pObject->m_bPoolClass )
		{
			pPetObject->m_pObject->m_bPetPool = true;
		}

		extern std::vector<sPetInfo> g_MyPetList;
		bool bExists = false;
		for (size_t i = 0; i < g_MyPetList.size(); ++i) {
			if (g_MyPetList[i].dwID == dwID) {
				bExists = true;
				break;
			}
		}
		if (!bExists) {
			sPetInfo myPet;
			myPet.dwID = dwID;
			myPet.szName = szName;
			myPet.wLevel = wLevel;
			myPet.dwHpCur = dwHpCur;
			g_MyPetList.push_back(myPet);
		}

		g_PetList.AddPet( pPetObject);
		g_MainCharInfo.ShowHelpMessage(IDS_PET_BONGOUT);
	}
	else
	{
		switch(bResult)
		{
		case 8:
			g_MainCharInfo.ShowHelpMessage(IDS_PETBONGOUT_IMPOSSIBLE, TEXTEFFECT_COLOR_WARNING);			
			break;
		}
	}
	
	return 0;
}

int OnCS_NC_PETWILDRATE_ACK(CMsg &msg)
{
	DWORD dwOwnerID;
	DWORD dwPetID;
	BYTE bWildRate;

	msg
		>> dwOwnerID
		>> dwPetID
		>> bWildRate;

	XiahObject::CXiahObject* pObject = g_PetList.Find( dwPetID);

    //YS_0812 : BUGFIX
	if( pObject && pObject->m_pObject )
	{
		sPetInfo* pPetInfo = (sPetInfo *)pObject->m_pObject->m_pPrivateData;

		if( pPetInfo)
		{
			pPetInfo->bWildRate = bWildRate;
			g_MainCharInfo.RefreshPetInfo();
		}

		if( bWildRate > g_nPetWildRate)
		{
			TCHAR temp[50];
			_stprintf( temp, IDS_WARN_FIELD, bWildRate);
			g_MainCharInfo.ShowHelpMessage( temp);
			g_MainCharInfo.ShowHelpMessage( IDS_BONGING_FEED,TEXTEFFECT_COLOR_WARNING);

			//HT_CHEAT : 간을 안 처먹을 때가 있지?? ㅡㅡ;;
			XiahItem::sItemInfo* pItem = g_MainCharInfo.m_pMySack[0]->FindSackItemByVisualID(g_dwPetFoodID); //독수리간만
			if( pItem) 
			{
				SendCS_IM_GIVEITEM_REQ( pItem->m_bSackCount+1, pItem->m_bSackPos, pItem->m_dwItemID, OBJTYPE_PET, pPetInfo->dwID);
			}
			else
			{
				pItem = g_MainCharInfo.m_pMySack[1]->FindSackItemByVisualID(g_dwPetFoodID);
				if( pItem)
				{
					SendCS_IM_GIVEITEM_REQ( pItem->m_bSackCount+1, pItem->m_bSackPos, pItem->m_dwItemID, OBJTYPE_PET, pPetInfo->dwID);
				}
				else //행낭에 독수리간이 없을때 봉인을 하자
				{
					XiahItem::sItemInfo* pBongInItem = g_MainCharInfo.m_pMySack[0]->FindSackItemByVisualID(g_dwPetSealID); //우선 공혼경
					if(pBongInItem)
					{
						g_MainCharInfo.ShowHelpMessage(_T("독수리 간이 없어서 봉인함.."), TEXTEFFECT_COLOR_GAIN);
						SendCS_NC_PETBONGIN_REQ( g_PetList.GetPetInfoByIndex(0)->dwID, pBongInItem->m_bSackCount+1, pBongInItem->m_bSackPos);
					}
					else
					{
						g_MainCharInfo.ShowHelpMessage(_T("공혼경이 없어서 봉인을 못해.. "), TEXTEFFECT_COLOR_WARNING);
					}
				}
			}
		}
	}

	return 0;
}

int OnCS_NC_PETREVOLUTION_ACK(CMsg &msg)
{
	DWORD dwOwnerID		=0;
	DWORD dwPetID		=0;
	BYTE bNpcType		=0;
	BYTE bRevolutionStep=0;
	BYTE bPetRebirth	=0; //HO_0913_07 영수 각성신단 추가 서버에서 우선 보낸다 받아 두기만 하자
	BYTE bUseRenolution =0;

	msg
		>> dwOwnerID
		>> dwPetID
		>> bNpcType
		>> bRevolutionStep
		>> bPetRebirth
		>> bUseRenolution;


	//HT_0622 : 영수둔갑신단 적용(리스트에서 못 찾아옴)
	//XiahObject::CXiahObject* pObject = g_PetList.Find( dwPetID);
	XiahObject::CXiahObject* pObject = XiahObject::g_XiahObjectManager.FindXiahObject( MAKEOBJECTID( 0, dwPetID, OBJTYPE_PET));

	//YS_0812 : BUGFIX
	if( pObject && pObject->m_pObject )
	{
		sPetInfo* pPetInfo = (sPetInfo *)pObject->m_pObject->m_pPrivateData;

		if( pPetInfo)
		{
			//HT_0714 : 주인에게만 메시지를 뿌린다. 
			if(dwOwnerID == g_MainCharInfo.m_dwObjectID)
			{
				if(bUseRenolution)	
				{
					g_MainCharInfo.ShowHelpMessage(IDS_MONSTER_USEREVOLUT, TEXTEFFECT_COLOR_GAIN);
				}
				
				else				
				{
					TCHAR temp[100];
					_stprintf( temp, IDS_MONSTER_REVOLUT, (LPCTSTR)pPetInfo->szName, bRevolutionStep);
					g_MainCharInfo.ShowHelpMessage(temp, TEXTEFFECT_COLOR_GAIN);
				}
			}

			pPetInfo->bRevolutionStep = bRevolutionStep;
			g_MainCharInfo.RefreshPetInfo();

			sArrayData *pData = XiahArrayIndex::g_NpcType.GetData( bNpcType);
			if( pData == NULL) return TRUE;

			int nCharID = pData->GetInt( 1);
			int nMeshType = pData->GetInt( 2);
			int nTextureType = pData->GetInt( 3);

			if(bRevolutionStep == 4 && bNpcType == 0) //HT_0621 : 영수둔갑신단 적용
			{
				// [10/31/2005] 캐릭터 ID변경
				nCharID = 1133;
			}

			if( XiahGameEngine::GetCharacter( nCharID) == NULL)
				return TRUE;

			CRes_Character* pResChar = XiahGameEngine::GetCharacter( nCharID);
			if( pResChar == NULL)
			{
				return TRUE;
			}

			if( !pResChar->GetMesh( nMeshType))
			{
				return TRUE;
			}

			if( pResChar->GetMesh( nMeshType)->GetTexture( nTextureType) == NULL)
			{
				return TRUE;
			}

			CXiahCharObject *pObj = reinterpret_cast<CXiahCharObject*>(pObject->m_pObject);

			if(bRevolutionStep == 4 && bNpcType != 0) //HT_0621 : 영수둔갑신단 적용
			{
				WORD  wVisualID[6];
				ZeroMemory(wVisualID, sizeof(WORD)*6);
				SetupPET_VisualEquipement(pObj, wVisualID);
			}
		
			pObj->Create( nCharID, nMeshType, nTextureType, 1);
			pObj->Update();

			if(bRevolutionStep == 4 && bNpcType == 0) //HT_0621 : 영수둔갑신단 적용
			{
				WORD  wVisualID[6];
				ZeroMemory(wVisualID, sizeof(WORD)*6);
				SetupPET_VisualEquipement(pObj, wVisualID);
			}
		}
	}

	return 0;
}

int OnCS_NC_PETSACKLIST_ACK(CMsg &msg)
{
	DWORD dwPetID;
	BYTE bSackID;
	BYTE bNumItem;
	BYTE bSackPos;

	msg
		>> dwPetID
		>> bSackID
		>> bNumItem;

	sPetInfo* pPetInfo = g_PetList.GetPetInfo( dwPetID);

	if(pPetInfo == NULL) return 0;

	for( int i=0; i< bNumItem; i++)
	{
		XiahItem::sItemInfo* pItem = new XiahItem::sItemInfo;

		msg
			>> bSackPos;
		XiahItem::GetItemData( pItem, msg);

		if( bSackID != PETSACKTYPE_EQUIPMENT )
		{
			pItem->m_bSackID = SACKTYPE__PET;
			pItem->m_bSackCount = bSackID - 1;

			if( pPetInfo)
			{
				if( bSackID >= 1 && bSackID <= 3 )
					pPetInfo->m_pSack[bSackID-1]->InsertItem( bSackPos, pItem);
			}
		}
		else
		{
			pItem->m_bSackID = SACKTYPE__PET_EQUIP;

			if( pPetInfo)
			{
				pPetInfo->m_pEquipSack->InsertItem( bSackPos, pItem);
			}
		}
	}

	return 0;
}

int OnCS_NC_PETITEMPUT_ACK(CMsg &msg)
{
	BYTE bResult;

	msg 
		>> bResult;

	switch( bResult)
	{
	case ERR_PETITEMPUT_SUCCESS:
		break;
	case ERR_PETITEMPUT_NOTFINDITEM:
		g_MainCharInfo.ShowHelpMessage( IDS_ITEM_NOTFIND, TEXTEFFECT_COLOR_WARNING);
		break;
	case ERR_PETITEMPUT_NOTFINDPET:
		g_MainCharInfo.ShowHelpMessage( IDS_PETBONGIN_NOTFINDPET, TEXTEFFECT_COLOR_WARNING);
		break;
	case ERR_PETITEMPUT_CANNOTEQUIPMENT:
		g_MainCharInfo.ShowHelpMessage( IDS_CANNOT_PUT, TEXTEFFECT_COLOR_WARNING);
		break;
	case ERR_PETITEMPUT_NOTACTIVE:
		g_MainCharInfo.ShowHelpMessage( IDS_CANNOT_PUT_ITEM_BAG, TEXTEFFECT_COLOR_WARNING);
		break;
	case ERR_PETITEMPUT_NOTEMPTY:
		g_MainCharInfo.ShowHelpMessage( IDS_PUT_FIT, TEXTEFFECT_COLOR_WARNING);
		break;
	case ERR_PETITEMPUT_DBERROR:
		g_MainCharInfo.ShowHelpMessage( IDS_INTERNAL_ERROR, TEXTEFFECT_COLOR_WARNING);
	case 7:
	case 8:
		g_MainCharInfo.ShowHelpMessage( IDS_CANNOT_PUT_REVOLUT, TEXTEFFECT_COLOR_WARNING);
		break;
	}

	g_MainCharInfo.m_pHoldItem->SetItemBackToSack();

	return 0;
}

int OnCS_NC_PETITEMOUT_ACK(CMsg &msg)
{
	BYTE bResult;

	msg 
		>> bResult;

	if( !bResult)
		return 0;

	TCHAR temp[50];
	_stprintf( temp, IDS_ERROR, bResult);

	switch( bResult)
	{
	case ERR_PETITEMOUT_NOTFINDPET:
		g_MainCharInfo.ShowHelpMessage( temp,TEXTEFFECT_COLOR_WARNING);
		break;
	case ERR_PETITEMOUT_NOTFINDITEM:
		g_MainCharInfo.ShowHelpMessage( temp,TEXTEFFECT_COLOR_WARNING);
		break;
	case ERR_PETITEMOUT_NOTEMPTY:
		g_MainCharInfo.ShowHelpMessage( IDS_PUT_SPACE,TEXTEFFECT_COLOR_WARNING);
		break;
	case ERR_PETITEMOUT_DBERROR:
		g_MainCharInfo.ShowHelpMessage( temp,TEXTEFFECT_COLOR_WARNING);
		break;
	case ERR_PETITEMOUT_HASITEM:
		g_MainCharInfo.ShowHelpMessage( IDS_CANNOT_REMOVE_ITEMBAG,TEXTEFFECT_COLOR_WARNING);
		break;
	}

	g_MainCharInfo.m_pHoldItem->SetItemBackToSack();
	
	return 0;
}

int OnCS_NC_PETITEMMOVE_ACK(CMsg &msg)
{
	BYTE bResult;

	msg 
		>> bResult;

	TCHAR temp[50];
	_stprintf( temp, IDS_ERROR, bResult);

	switch( bResult)
	{
	case  ERR_PETITEMMOVE_SUCCESS:
		break;
	case ERR_PETITEMMOVE_NOTFINDPET:
		g_MainCharInfo.ShowHelpMessage( temp);
		break;
	case ERR_PETITEMMOVE_NOTFINDITEM:
		g_MainCharInfo.ShowHelpMessage( temp);
		break;
	case ERR_PETITEMMOVE_CANNOTMOVE:
		g_MainCharInfo.ShowHelpMessage( temp);
		break;
	case ERR_PETITEMMOVE_CANNOTEQUIPMENT:
		g_MainCharInfo.ShowHelpMessage( temp);
		break;
	case ERR_PETITEMMOVE_NOTEMPTYSACK:
		g_MainCharInfo.ShowHelpMessage( temp);
		break;
	case ERR_PETITEMMOVE_NOTACTIVE:
		g_MainCharInfo.ShowHelpMessage( temp);
		break;
	case ERR_PETITEMMOVE_DBERROR:
		g_MainCharInfo.ShowHelpMessage( temp);
		break;
	}

	g_MainCharInfo.m_pHoldItem->SetItemBackToSack();
	
	return 0;
}

int OnCS_NC_PETPICKITEM_ACK(CMsg &msg)
{
	BYTE bResult;
	DWORD	dwPetID;

	msg 
		>> bResult
		>> dwPetID;

	if(bResult != 0)
	{
		// 아마도 Sack이 꽉차있을것이다!
		// 해당 PET의 AI를 자기 보호로 만든다.
		g_PetList.Change_PET_AI_Specify(dwPetID,PETAI_AUTOATTACK);

		if(bResult == 2)
		g_MainCharInfo.ShowHelpMessage( IDS_MONSTER_NOBAG);
	}

	return 0;
}

int OnCS_NC_PETTHROWITEM_ACK(CMsg &msg)
{
	BYTE bResult;

	msg 
		>> bResult;

	if( !bResult)
		return 0;

	TCHAR temp[50];
	_stprintf( temp, IDS_ERROR, bResult);
/*
	switch( bResult)
	{
	case ERR_PETTHROWITEM_NOTFINDPET:
		g_MainCharInfo.ShowHelpMessage( temp,TEXTEFFECT_COLOR_WARNING);
		break;
	case ERR_PETTHROWITEM_NOTFINDITEM:
		g_MainCharInfo.ShowHelpMessage( temp,TEXTEFFECT_COLOR_WARNING);
		break;
	case ERR_PETTHROWITEM_DBERROR:
		g_MainCharInfo.ShowHelpMessage( temp,TEXTEFFECT_COLOR_WARNING);
		break;
	}
*/
	g_MainCharInfo.ShowHelpMessage( temp,TEXTEFFECT_COLOR_WARNING);

	g_MainCharInfo.m_pHoldItem->SetItemBackToSack();

	return 0;
}

// SendCS_NC_PETPICKITEM_REQ의 결과
int OnCS_NC_PETITEMADD_ACK(CMsg &msg)
{
	DWORD dwPetID;
	BYTE bSackID;
	BYTE bSackPos;

	msg
		>> dwPetID
		>> bSackID
		>> bSackPos;

	if( XiahObject::g_XiahObjectManager.FindXiahObject( MAKEOBJECTID( 0, dwPetID, OBJTYPE_PET)) == NULL)
		return 0; 

	XiahItem::sItemInfo* pItem = new XiahItem::sItemInfo;
	XiahItem::GetItemData( pItem, msg);

	sPetInfo* pPetInfo = g_PetList.GetPetInfo( dwPetID);

	if( bSackID != PETSACKTYPE_EQUIPMENT )
	{
		pItem->m_bSackID = SACKTYPE__PET;
		pItem->m_bSackCount = bSackID - 1;

		if( pPetInfo)
		{
			//YS_0812 : BUGFIX
			if( bSackID >= 1 && bSackID <= 3 && pPetInfo->m_pSack[bSackID-1] )
                pPetInfo->m_pSack[bSackID-1]->InsertItem( bSackPos, pItem);
		}
	}
	else
	{
		pItem->m_bSackID = SACKTYPE__PET_EQUIP;
		
		//YS_0812 : BUGFIX
		if( pPetInfo && pPetInfo->m_pEquipSack )
		{
			pPetInfo->m_pEquipSack->InsertItem( bSackPos, pItem);
		}
	}

	g_MainCharInfo.m_pHoldItem->EmptyHoldItemItem();

	g_MainCharInfo.PlayInterfaceSound( ISOUND_ITEM_LAY_SACK);

	return 0;
}

int OnCS_NC_PETITEMDEL_ACK(CMsg &msg)
{
	DWORD dwPetID;
	BYTE bSackID;
	BYTE bSackPos;
	DWORD dwAmount;

	msg
		>> dwPetID
		>> bSackID
		>> bSackPos
		>> dwAmount;

	if( XiahObject::g_XiahObjectManager.FindXiahObject( MAKEOBJECTID( 0, dwPetID, OBJTYPE_PET)) == NULL)
		return 0; 

	sPetInfo* pPetInfo = g_PetList.GetPetInfo( dwPetID);
	if( bSackID != PETSACKTYPE_EQUIPMENT )
	{
		if( pPetInfo)
		{
			// 누수 수정
			if( bSackID >= 1 && bSackID <= 3 && pPetInfo->m_pSack[bSackID - 1] )
                pPetInfo->m_pSack[bSackID - 1]->DeleteItem( bSackPos, true);
		}
	}
	else
	{
		//YS_0812 : BUGFIX
		if( pPetInfo && pPetInfo->m_pEquipSack )
		{
			// 누수 수정
			pPetInfo->m_pEquipSack->DeleteItem( bSackPos, true);
		}
	}

	g_MainCharInfo.m_pHoldItem->EmptyHoldItemItem();

	return 0;
}

// 환수유 & 분신격
int OnCS_NC_PETNEWMAKE_ACK(CMsg &msg)
{
	BYTE bResult;
	DWORD dwID;
	DWORD dwOwnID;
	DWORD dwMapID;
	BYTE  bNpcType;
	sString szName;
	WORD  wLevel;
	WORD  wPosX;
	WORD  wPosY;
	BYTE  bHeight;
	WORD  wDesPosX;
	WORD  wDesPosY;
	BYTE  bDesHeight;
	WORD  wDirection;
	DWORD  dwHpMax;
	DWORD  dwHpCur;
	WORD  wAtkPwr;
	WORD  wDefPwr;
	WORD  wAtkRating;
	WORD  wAvoidRatio;
	BYTE  bSpeed;
	WORD  wMeleeAtkRange;
	WORD  wShotAtkRange;
	BYTE  bAtkType;
	DWORD dwRefNpcID;
	BYTE  bCurJob;
	INT64 i64Exp;
	INT64 i64LevelExp;
	INT64 i64NextLevelUpExp;
	BYTE  bRevolutionStep;
	BYTE bWildRate;

	msg
		>> bResult
		>> dwID
		>> dwOwnID
		>> dwMapID
		>> bNpcType
		>> szName
		>> wLevel
		>> wPosX
		>> wPosY
		>> bHeight
		>> wDesPosX
		>> wDesPosY
		>> bDesHeight
		>> wDirection
		>> dwHpMax
		>> dwHpCur
		>> wAtkPwr
		>> wDefPwr
		>> wAtkRating
		>> wAvoidRatio
		>> bSpeed
		>> wMeleeAtkRange
		>> wShotAtkRange
		>> bAtkType
		>> dwRefNpcID
		>> bCurJob
		>> i64Exp
		>> i64LevelExp
		>> i64NextLevelUpExp
		>> bRevolutionStep
		>> bWildRate;

	bWildRate = 0;

	//HT_0711 : 진각성 무공
	if(bNpcType == 250)
	{
		// 환수유
		if( g_PetList.Find( dwID) != NULL)
		{
			DBG_LogFile(_T("환수유는 한번에 한마리!!"));
		}

		sArrayData *pData = XiahArrayIndex::g_NpcType.GetData( bNpcType);
		if( pData == NULL) return TRUE;

		int nCharID = pData->GetInt( 1);
		int nMeshType = pData->GetInt( 2);
		int nTextureType = pData->GetInt( 3);
		if( XiahGameEngine::GetCharacter( nCharID) == NULL) return TRUE;
		CRes_Character* pResChar = XiahGameEngine::GetCharacter( nCharID);
		if( pResChar == NULL)
		{
			return TRUE;
		}

		if( !pResChar->GetMesh( nMeshType))
		{
			return TRUE;
		}

		if( pResChar->GetMesh( nMeshType)->GetTexture( nTextureType) == NULL)
		{
			return TRUE;
		}

		//YS_0805 : BUGFIX
		//CXiahCharObject *pObject = new CXiahCharObject;
		CXiahCharObject* pObject = NULL;
		pObject = (CXiahCharObject*)XiahObject::g_XiahPetPool.GetChar();// new CXiahCharObject;
		if ( NULL == pObject )
		{
			pObject = new CXiahCharObject;
		}

		pObject->Create( nCharID, nMeshType, nTextureType, 1);
		pObject->m_pAniType = XiahAniType::GetAniType( OBJTYPE_NPC, 0);

		pObject->SetAngle( wDirection);
		pObject->SetPosition( wPosX, wPosY);

		pObject->SetAnimation( XiahAniType::eLAT_Stand, -1);

		pObject->m_szObjectName = szName;
		pObject->m_bObjType = OBJTYPE_PET;
		pObject->m_bSubObjType = bNpcType;

		pObject->m_dwCurHP = dwHpCur;
		pObject->m_dwMaxHP = dwHpMax;

		XiahObject::CXiahObject* pPetObject = XiahObject::g_XiahObjectManager.CreateXiahObject( dwID,  OBJTYPE_PET, pObject);

		// 내꺼일경우에만 PETlist에 넣는다
		if(g_pMainChar->m_dwServerID == dwOwnID)
		{

			// PET의 정보 설정
			sPetInfo* pPetInfo = new sPetInfo;
			pPetInfo->m_dwIsHwan		= 1;	// 이건 환수유용 PET!

			pPetInfo->dwID				= dwID;				
			pPetInfo->dwOwnID			= dwOwnID;				
			pPetInfo->dwMapID			= dwMapID;				
			pPetInfo->bNpcType			= bNpcType;				
			pPetInfo->szName			= szName;				
			pPetInfo->wLevel			= wLevel;				
			pPetInfo->wPosX				= wPosX;				
			pPetInfo->wPosY				= wPosY;				
			pPetInfo->bHeight			= bHeight;				
			pPetInfo->wDesPosX			= wDesPosX;				
			pPetInfo->wDesPosY			= wDesPosY;				
			pPetInfo->bDesHeight		= bDesHeight;				
			pPetInfo->wDirection		= wDirection;				
			pPetInfo->dwHpMax			= dwHpMax;				
			pPetInfo->dwHpCur			= dwHpCur;				
			pPetInfo->wAtkPwr			= wAtkPwr;				
			pPetInfo->wDefPwr			= wDefPwr;				
			pPetInfo->wAtkRating		= wAtkRating;
			pPetInfo->wAvoidRatio		= wAvoidRatio;				
			pPetInfo->bSpeed			= bSpeed;				
			pPetInfo->wMeleeAtkRange	= wMeleeAtkRange;				
			pPetInfo->wShotAtkRange		= wShotAtkRange;				
			pPetInfo->bAtkType			= bAtkType;				
			pPetInfo->dwRefNpcID		= dwRefNpcID;				
			pPetInfo->bCurJob			= bCurJob;				
			pPetInfo->i64Exp			= i64Exp;			
			pPetInfo->i64LevelExp		= i64LevelExp;	
			pPetInfo->i64NextLevelUpExp	= i64NextLevelUpExp;
			pPetInfo->bRevolutionStep	= bRevolutionStep;				
			pPetInfo->bWildRate			= bWildRate;
			pPetInfo->bAI				= TRUE;						// AI ON
			pPetInfo->AI_Type			= PETAI_AUTOATTACK;		// 이것은 항상 Auto Attack

			pObject->m_pPrivateData		= (DWORD)pPetInfo;
			pObject->m_PrivateDataDestoryer = ReleasePetInfo;
			
			//YS_0812 : BUGFIX
			if ( pPetObject->m_pObject && pPetObject->m_pObject->m_bPoolClass )
			{
				pPetObject->m_pObject->m_bPetPool = true;
			}
			
			g_PetList.AddPet( pPetObject);
		}
	}


	// 분신격
	//if(bNpcType == 251 && bCurJob == 2)
	//if(bCurJob == 2)
	else
	{
		// 오너를 찾는다
		XiahObject::CXiahObject* pOwner = XiahObject::g_XiahObjectManager.FindXiahObject( MAKEOBJECTID( 0, dwOwnID, OBJTYPE_PC));

		//YS_0805 : BUGFIX
		if ( !pOwner )
			return TRUE;

		CXiahCharObject* pCharObject = (CXiahCharObject*)pOwner->m_pObject;

		CXiahCharObject* pBunsin = NULL;
		pBunsin = (CXiahCharObject *)XiahObject::g_XiahPetPool.GetChar();// new CXiahCharObject;
		if ( NULL == pBunsin )
		{
			pBunsin = new CXiahCharObject;
		}
		
		pBunsin->m_pAniType = XiahAniType::GetAniType( OBJTYPE_PC, 0);

		int nCharID		= pCharObject->m_CharRender.GetCharID();
		int nMeshType	= pCharObject->m_CharRender.GetMeshType();
		int nTextureType = pCharObject->m_CharRender.GetTextureType();

		if( XiahGameEngine::GetCharacter( nCharID) == NULL) return TRUE;
		CRes_Character* pResChar = XiahGameEngine::GetCharacter( nCharID);
		if( pResChar == NULL)
		{
			return TRUE;
		}

		if( !pResChar->GetMesh( nMeshType))
		{
			return TRUE;
		}

		if( pResChar->GetMesh( nMeshType)->GetTexture( nTextureType) == NULL)
		{
			return TRUE;
		}

		pBunsin->Create( nCharID, nMeshType, nTextureType, 103);
		pBunsin->SetPosition( wPosX, wPosY);
		pBunsin->m_szObjectName = pCharObject->m_szObjectName;	
		pBunsin->m_bObjType		= OBJTYPE_PET;
		pBunsin->m_bSubObjType  = pCharObject->m_bSubObjType;

		// 2004.08.06 Changth
		// 분신격이 뛰니깐 분신이 뛸때도 걷는 소리가 나는데 이때 에러가 난다. 
		// 그래서 분신이 뛸때는 소리가 없게한다.
		pBunsin->m_CharRender.EnableAnimationSound(false);

		sPCVisualInfo *pVisualInfo = (sPCVisualInfo *)pCharObject->m_pPrivateData;

		//YS_0811 : BUGFIX
		if ( !pVisualInfo )
			return TRUE;

		SetupPC_VisualEquipement( pBunsin,pVisualInfo->wVisualID,pVisualInfo->bRarity,pVisualInfo->bStxType);
		XiahObject::CXiahObject* pPetObject = XiahObject::g_XiahObjectManager.CreateXiahObject( dwID, OBJTYPE_PET, pBunsin);

		pBunsin->m_dwOwnerID = dwOwnID;

		// 내꺼일경우에만 PETlist에 넣는다
		if(g_pMainChar->m_dwServerID == dwOwnID)
		{
			// PET의 정보 설정
			sPetInfo* pPetInfo = new sPetInfo;
			pPetInfo->m_dwIsHwan		= 2;	// 2번은 분신격

			pPetInfo->dwID				= dwID;				
			pPetInfo->dwOwnID			= dwOwnID;				
			pPetInfo->dwMapID			= dwMapID;				
			pPetInfo->bNpcType			= bNpcType;				
			pPetInfo->szName			= szName;				
			pPetInfo->wLevel			= wLevel;				
			pPetInfo->wPosX				= wPosX;				
			pPetInfo->wPosY				= wPosY;				
			pPetInfo->bHeight			= bHeight;				
			pPetInfo->wDesPosX			= wDesPosX;				
			pPetInfo->wDesPosY			= wDesPosY;				
			pPetInfo->bDesHeight		= bDesHeight;				
			pPetInfo->wDirection		= wDirection;				
			pPetInfo->dwHpMax			= dwHpMax;				
			pPetInfo->dwHpCur			= dwHpCur;				
			pPetInfo->wAtkPwr			= wAtkPwr;				
			pPetInfo->wDefPwr			= wDefPwr;				
			pPetInfo->wAtkRating		= wAtkRating;
			pPetInfo->wAvoidRatio		= wAvoidRatio;				
			pPetInfo->bSpeed			= bSpeed;				
			pPetInfo->wMeleeAtkRange	= wMeleeAtkRange;
			pPetInfo->wShotAtkRange		= wShotAtkRange;				
			pPetInfo->bAtkType			= bAtkType;				
			pPetInfo->dwRefNpcID		= dwRefNpcID;				
			pPetInfo->bCurJob			= bCurJob;				
			pPetInfo->i64Exp			= i64Exp;
			pPetInfo->i64LevelExp		= i64LevelExp;	
			pPetInfo->i64NextLevelUpExp	= i64NextLevelUpExp;
			pPetInfo->bRevolutionStep	= bRevolutionStep;				
			pPetInfo->bWildRate			= bWildRate;
			pPetInfo->bAI				= TRUE;						// AI ON
			pPetInfo->AI_Type			= PETAI_AUTOATTACK;		// 이것은 항상 Auto Attack

			pBunsin->m_pPrivateData = (DWORD)pPetInfo;
			pBunsin->m_PrivateDataDestoryer = ReleasePetInfo;	

			//YS_0812 : BUGFIX
			if ( pPetObject->m_pObject && pPetObject->m_pObject->m_bPoolClass )
			{
				pPetObject->m_pObject->m_bPetPool = true;
			}

			g_PetList.AddPet( pPetObject);
		}
		
	}
	//else
	//{
	//	// 환수유
	//	if( g_PetList.Find( dwID) != NULL)
	//	{
	//		DBG_LogFile(_T("환수유는 한번에 한마리!!"));
	//	}

	//	sArrayData *pData = XiahArrayIndex::g_NpcType.GetData( bNpcType);
	//	if( pData == NULL) return TRUE;

	//	int nCharID = pData->GetInt( 1);
	//	int nMeshType = pData->GetInt( 2);
	//	int nTextureType = pData->GetInt( 3);
	//	if( XiahGameEngine::GetCharacter( nCharID) == NULL) return TRUE;
	//	CRes_Character* pResChar = XiahGameEngine::GetCharacter( nCharID);
	//	if( pResChar == NULL)
	//	{
	//		return TRUE;
	//	}

	//	if( !pResChar->GetMesh( nMeshType))
	//	{
	//		return TRUE;
	//	}

	//	if( pResChar->GetMesh( nMeshType)->GetTexture( nTextureType) == NULL)
	//	{
	//		return TRUE;
	//	}

	//	//YS_0805 : BUGFIX
	//	//CXiahCharObject *pObject = new CXiahCharObject;
	//	CXiahCharObject* pObject = NULL;
	//	pObject = (CXiahCharObject*)XiahObject::g_XiahPetPool.GetChar();// new CXiahCharObject;
	//	if ( NULL == pObject )
	//	{
	//		pObject = new CXiahCharObject;
	//	}

	//	pObject->Create( nCharID, nMeshType, nTextureType, 1);
	//	pObject->m_pAniType = XiahAniType::GetAniType( OBJTYPE_NPC, 0);

	//	pObject->SetAngle( wDirection);
	//	pObject->SetPosition( wPosX, wPosY);

	//	pObject->SetAnimation( XiahAniType::eLAT_Stand, -1);

	//	pObject->m_szObjectName = szName;
	//	pObject->m_bObjType = OBJTYPE_PET;
	//	pObject->m_bSubObjType = bNpcType;

	//	pObject->m_dwCurHP = dwHpCur;
	//	pObject->m_dwMaxHP = dwHpMax;

	//	XiahObject::CXiahObject* pPetObject = XiahObject::g_XiahObjectManager.CreateXiahObject( dwID,  OBJTYPE_PET, pObject);

	//	// 내꺼일경우에만 PETlist에 넣는다
	//	if(g_pMainChar->m_dwServerID == dwOwnID)
	//	{

	//		// PET의 정보 설정
	//		sPetInfo* pPetInfo = new sPetInfo;
	//		pPetInfo->m_dwIsHwan		= 1;	// 이건 환수유용 PET!

	//		pPetInfo->dwID				= dwID;				
	//		pPetInfo->dwOwnID			= dwOwnID;				
	//		pPetInfo->dwMapID			= dwMapID;				
	//		pPetInfo->bNpcType			= bNpcType;				
	//		pPetInfo->szName			= szName;				
	//		pPetInfo->wLevel			= wLevel;				
	//		pPetInfo->wPosX				= wPosX;				
	//		pPetInfo->wPosY				= wPosY;				
	//		pPetInfo->bHeight			= bHeight;				
	//		pPetInfo->wDesPosX			= wDesPosX;				
	//		pPetInfo->wDesPosY			= wDesPosY;				
	//		pPetInfo->bDesHeight		= bDesHeight;				
	//		pPetInfo->wDirection		= wDirection;				
	//		pPetInfo->dwHpMax			= dwHpMax;				
	//		pPetInfo->dwHpCur			= dwHpCur;				
	//		pPetInfo->wAtkPwr			= wAtkPwr;				
	//		pPetInfo->wDefPwr			= wDefPwr;				
	//		pPetInfo->wAtkRating		= wAtkRating;
	//		pPetInfo->wAvoidRatio		= wAvoidRatio;				
	//		pPetInfo->bSpeed			= bSpeed;				
	//		pPetInfo->wMeleeAtkRange	= wMeleeAtkRange;				
	//		pPetInfo->wShotAtkRange		= wShotAtkRange;				
	//		pPetInfo->bAtkType			= bAtkType;				
	//		pPetInfo->dwRefNpcID		= dwRefNpcID;				
	//		pPetInfo->bCurJob			= bCurJob;				
	//		pPetInfo->i64Exp			= i64Exp;			
	//		pPetInfo->i64LevelExp		= i64LevelExp;	
	//		pPetInfo->i64NextLevelUpExp	= i64NextLevelUpExp;
	//		pPetInfo->bRevolutionStep	= bRevolutionStep;				
	//		pPetInfo->bWildRate			= bWildRate;
	//		pPetInfo->bAI				= TRUE;						// AI ON
	//		pPetInfo->AI_Type			= PETAI_AUTOATTACK;		// 이것은 항상 Auto Attack

	//		pObject->m_pPrivateData		= (DWORD)pPetInfo;
	//		pObject->m_PrivateDataDestoryer = ReleasePetInfo;
	//		
	//		//YS_0812 : BUGFIX
	//		if ( pPetObject->m_pObject && pPetObject->m_pObject->m_bPoolClass )
	//		{
	//			pPetObject->m_pObject->m_bPetPool = true;
	//		}
	//		
	//		g_PetList.AddPet( pPetObject);
	//	}
	//}


	return 0;
}

/**
 *
 * \param &msg 
 * \return 
 */
int OnCS_NC_PETRESTORELIST_ACK(CMsg &msg)
{
	BYTE bResult =0;
	
	msg
		>> bResult;

	if(!bResult)
	{
		for(map<BYTE, sPetRevival*>::iterator iter = g_PetList.m_mPetRevivalList.begin(); iter != g_PetList.m_mPetRevivalList.end(); ++iter)
		{
			sPetRevival* pInfo = iter->second;

			if(pInfo)
				delete pInfo;
		}

		g_PetList.m_mPetRevivalList.clear();

		BYTE bPetCount = 0;

		msg
			>> bPetCount;

		if(bPetCount > 6)
		{
			DBG_LogFile(_T("펫 복구 리스트 수 이상"));
			return 1;
		}

		DWORD dwPetID[5];
		sString	strName[5];
		BYTE bNpcType	=0;		
		WORD wLevel		=0;
		DWORD dwHpMax		=0;
		WORD wAtkPwr	=0;
		WORD wDefPwr	=0;
		WORD wAtkRating	=0;
		WORD wAvoidRatio=0;

		bool bPet[5];

		for(int i=0; i < 5; ++i)
			bPet[i] = false;

        WORD wLevelAry[5];

		for(BYTE i=0; i < bPetCount; ++i)
		{
			msg
				>> dwPetID[i]
				>> bNpcType
				>> strName[i]
				>> wLevel
				>> dwHpMax
				>> wAtkPwr
				>> wDefPwr
				>> wAtkRating
				>> wAvoidRatio;

				bPet[i] = true;

				sPetRevival* pPetInfo = new sPetRevival;
				pPetInfo->dwID		= dwPetID[i];
				pPetInfo->strName	= strName[i];
				g_PetList.m_mPetRevivalList.insert(map<BYTE, sPetRevival*>::value_type(i, pPetInfo));
				//g_MainCharInfo.m_mPetRevivalList.insert(std::map<DWORD, sString>::value_type(dwPetID[i], strName[i]));

				wLevelAry[i] = wLevel;
		}

		// 2004.08.02 Changth
		// 펫 이름과 갑자를 같이 뿌려준다.  광견 (2)
		for(BYTE i=0; i < bPetCount; i++)
			strName[i].printf( "%s (%d)", (LPCTSTR)strName[i], wLevelAry[i] );

		g_pUIManager->MakePopComboMenu(100, g_MainCharInfo.m_nLastUseItemXPos, g_MainCharInfo.m_nLastUseItemYPos,
										FRAMEID_REVIVAL, 5,
										bPet[0], RESID_COMBO, (LPCTSTR)strName[0],
										bPet[1], RESID_COMBO, (LPCTSTR)strName[1],
										bPet[2], RESID_COMBO, (LPCTSTR)strName[2],
										bPet[3], RESID_COMBO, (LPCTSTR)strName[3],
										bPet[4], RESID_COMBO, (LPCTSTR)strName[4]);
	}
	else
	{
		switch(bResult)
		{
		case 1:		// 더 이상 펫을 가질 수 없다.
			g_MainCharInfo.ShowHelpMessage(IDS_PETRESTORE_HASMAXPET, TEXTEFFECT_COLOR_WARNING);
			break;
		case 2:		// 부활약이 아님
			g_MainCharInfo.ShowHelpMessage(IDS_PETRESTORE_NOTFITITEM, TEXTEFFECT_COLOR_WARNING);
			break;
		case 3:		// 내부댄轎
			g_MainCharInfo.ShowHelpMessage(IDS_REL_ERR_INTERNAL, TEXTEFFECT_COLOR_WARNING);
			break;
		case 4:		// 존재하지 않는 펫
			g_MainCharInfo.ShowHelpMessage(IDS_PETBONGIN_NOTFINDPET, TEXTEFFECT_COLOR_WARNING);	// 펫 찾을수 없음
		}
	}
	

	return 0;
}

/**
 *
 * \param &msg 
 * \return 
 */
int OnCS_NC_PETRESTORE_ACK(CMsg &msg)
{
	BYTE bResult =0;

	msg
		>> bResult;

	switch(bResult)
	{
	case 0:
		{
			DWORD dwID		=0;
			DWORD dwOwnID	=0;
			DWORD dwMapID	=0;
			DWORD dwRefNpcID=0;
			WORD  wLevel	=0;
			WORD  wPosX		=0;
			WORD  wPosY		=0;			
			WORD  wDesPosX	=0;
			WORD  wDesPosY	=0;			
			WORD  wDirection=0;
			DWORD  dwHpMax	=0;
			DWORD  dwHpCur	=0;
			WORD  wAtkPwr	=0;
			WORD  wDefPwr	=0;
			WORD  wAtkRating=0;
			WORD  wAvoidRatio=0;			
			WORD  wMeleeAtkRange=0;
			WORD  wShotAtkRange	=0;
			INT64 i64Exp		=0;
			INT64 i64LevelExp	=0;
			INT64 i64NextLevelUpExp	=0;
			BYTE  bNpcType	=0;
			BYTE  bHeight	=0;
			BYTE  bDesHeight=0;
			BYTE  bSpeed	=0;
			BYTE  bAtkType	=0;
			BYTE  bCurJob	=0;
			BYTE  bRevolutionStep	=0;
			BYTE bWildRate	=0;
			WORD  wVisualID[6];

			sString szName;

			msg
				>> dwID
				>> dwOwnID
				>> dwMapID
				>> bNpcType
				>> szName
				>> wLevel
				>> wPosX
				>> wPosY
				>> bHeight
				>> wDesPosX
				>> wDesPosY
				>> bDesHeight
				>> wDirection
				>> dwHpMax
				>> dwHpCur
				>> wAtkPwr
				>> wDefPwr
				>> wAtkRating
				>> wAvoidRatio
				>> bSpeed
				>> wMeleeAtkRange
				>> wShotAtkRange
				>> bAtkType
				>> dwRefNpcID
				>> bCurJob
				>> i64Exp
				>> i64LevelExp
				>> i64NextLevelUpExp
				>> bRevolutionStep
				>> bWildRate
				>> wVisualID[0]
				>> wVisualID[1]
				>> wVisualID[2]
				>> wVisualID[3]
				>> wVisualID[4]
				>> wVisualID[5];

			sArrayData *pData = XiahArrayIndex::g_NpcType.GetData( bNpcType);
			if( pData == NULL)
				return TRUE;

			int nCharID		= pData->GetInt( 1);
			int nMeshType	= pData->GetInt( 2);
			int nTextureType = pData->GetInt( 3);

			if(bRevolutionStep == 4 && bNpcType == 0) //HT_0621 : 영수둔갑신단 적용
			{
				// [10/31/2005] 캐릭터 ID변경
				nCharID		= 1133;
				nMeshType	= 0;
			}

			if( XiahGameEngine::GetCharacter( nCharID) == NULL)
				return TRUE;

			CRes_Character* pResChar = XiahGameEngine::GetCharacter( nCharID);
			if( pResChar == NULL)
			{
				return TRUE;
			}

			if( !pResChar->GetMesh( nMeshType))
			{
				return TRUE;
			}

			if( pResChar->GetMesh( nMeshType)->GetTexture( nTextureType) == NULL)
			{
				return TRUE;
			}
			
			//YS_0805 : BUGFIX
			CXiahCharObject* pObject = NULL;			
			pObject = (CXiahCharObject*)XiahObject::g_XiahPetPool.GetChar(); //new CXiahCharObject;

			if( NULL == pObject )
			{
				pObject = new CXiahCharObject;
			}
	
			pObject->Create( nCharID, nMeshType, nTextureType, 1);
			pObject->m_pAniType = XiahAniType::GetAniType( OBJTYPE_NPC, 0);

			pObject->SetAngle( wDirection);
			pObject->SetPosition( wPosX, wPosY);

			pObject->SetAnimation( XiahAniType::eLAT_Stand, -1);

			if(bRevolutionStep == 4 && bNpcType == 0) //HT_0621 : 영수둔갑신단 적용
				SetupPET_VisualEquipement(pObject, wVisualID);

			pObject->m_szObjectName = szName;
			pObject->m_bObjType		= OBJTYPE_PET;
			pObject->m_bSubObjType  = bNpcType;

			pObject->m_dwCurHP = dwHpCur;
			pObject->m_dwMaxHP = dwHpMax;

			XiahObject::CXiahObject* pPetObject = XiahObject::g_XiahObjectManager.CreateXiahObject( dwID,  OBJTYPE_PET, pObject);

			// 내꺼일경우에만 PETlist에 넣는다
			if(g_pMainChar->m_dwServerID == dwOwnID)
			{

				// PET의 정보 설정
				sPetInfo* pPetInfo = new sPetInfo;

				pPetInfo->m_dwIsHwan		= 0;

				pPetInfo->dwID				= dwID;				
				pPetInfo->dwOwnID			= dwOwnID;				
				pPetInfo->dwMapID			= dwMapID;				
				pPetInfo->bNpcType			= bNpcType;				
				pPetInfo->szName			= szName;				
				pPetInfo->wLevel			= wLevel;				
				pPetInfo->wPosX				= wPosX;				
				pPetInfo->wPosY				= wPosY;				
				pPetInfo->bHeight			= bHeight;				
				pPetInfo->wDesPosX			= wDesPosX;				
				pPetInfo->wDesPosY			= wDesPosY;				
				pPetInfo->bDesHeight		= bDesHeight;				
				pPetInfo->wDirection		= wDirection;				
				pPetInfo->dwHpMax			= dwHpMax;				
				pPetInfo->dwHpCur			= dwHpCur;				
				pPetInfo->wAtkPwr			= wAtkPwr;				
				pPetInfo->wDefPwr			= wDefPwr;				
				pPetInfo->wAtkRating		= wAtkRating;
				pPetInfo->wAvoidRatio		= wAvoidRatio;				
				pPetInfo->bSpeed			= bSpeed;				
				pPetInfo->wMeleeAtkRange	= wMeleeAtkRange;				
				pPetInfo->wShotAtkRange		= wShotAtkRange;				
				pPetInfo->bAtkType			= bAtkType;				
				pPetInfo->dwRefNpcID		= dwRefNpcID;				
				pPetInfo->bCurJob			= bCurJob;				
				pPetInfo->i64Exp			= i64Exp;			
				pPetInfo->i64LevelExp		= i64LevelExp;	
				pPetInfo->i64NextLevelUpExp	= i64NextLevelUpExp;
				pPetInfo->bRevolutionStep	= bRevolutionStep;				
				pPetInfo->bWildRate			= bWildRate;
				pPetInfo->bAI				= TRUE;					// AI ON
				pPetInfo->AI_Type			= PETAI_AUTOATTACK;		// 이것은 항상 Auto Attack

				pObject->m_pPrivateData		= (DWORD)pPetInfo;
				pObject->m_PrivateDataDestoryer = ReleasePetInfo;
			
				//YS_0812 : BUGFIX
				if ( pPetObject->m_pObject && pPetObject->m_pObject->m_bPoolClass )
				{
					pPetObject->m_pObject->m_bPetPool = true;
				}

				g_PetList.AddPet( pPetObject);
			}
		}
		break;
	case 1:		// 더 이상 펫을 가질 수 없다.
		g_MainCharInfo.ShowHelpMessage(IDS_PETRESTORE_HASMAXPET, TEXTEFFECT_COLOR_WARNING);
		break;
	case 2:		// 부활약이 아님
		g_MainCharInfo.ShowHelpMessage(IDS_PETRESTORE_NOTFITITEM, TEXTEFFECT_COLOR_WARNING);
		break;
	case 3:		// 내부댄轎
		g_MainCharInfo.ShowHelpMessage(IDS_REL_ERR_INTERNAL, TEXTEFFECT_COLOR_WARNING);
		break;
	case 4:		// 존재하지 않는 펫
		g_MainCharInfo.ShowHelpMessage(IDS_PETBONGIN_NOTFINDPET, TEXTEFFECT_COLOR_WARNING);	// 펫 찾을수 없음
		break;
	default:
		break;
	}

	return 0;
}

/**
 *
 * \param &msg 
 * \return 
 */
int OnCS_NC_PREPETTRADE_ACK(CMsg &msg)
{
	BYTE bResult = 0;

	msg
		>> bResult;

	switch(bResult)
	{
	case ERR_PETTRADE_SUCCESS:
		{
			g_MainCharInfo.OpenFrame(WINDOW_COMMON);

			g_pUIManager->SetString(WINDOW_COMMON, window_common_edit, _T(""));
			g_pUIManager->SetFocus(WINDOW_COMMON, window_common_edit);
		}
		break;
	case ERR_PETTRADE_NOTHASPET:		// 거래할 펫이 없다
		g_MainCharInfo.ShowHelpMessage(IDS_PETTRADE_NOTHASPET, TEXTEFFECT_COLOR_WARNING);
		break;
	case ERR_PETTRADE_HASPET:			// 상대가 이미 펫을 가지고 있다.
		g_MainCharInfo.ShowHelpMessage(IDS_PETTRADE_HASPET, TEXTEFFECT_COLOR_WARNING);
		break;
	case ERR_PETTRADE_TRADEOTHER:		// 다른 사람과 거래중
		g_MainCharInfo.ShowHelpMessage(IDS_PETTRADE_TRADEOTHER, TEXTEFFECT_COLOR_WARNING);
		break;
	case ERR_PETTRADE_BONGIN:			// 봉인된 펫은 거래 불가
		g_MainCharInfo.ShowHelpMessage(IDS_PETTRADE_BONGIN, TEXTEFFECT_COLOR_WARNING);
		break;
	case ERR_PETTRADE_NOTMONEYENOUGH:	// 상대가 돈이 부족하다.
		g_MainCharInfo.ShowHelpMessage(IDS_PETTRADE_NOTMONEYENOUGH, TEXTEFFECT_COLOR_WARNING);
		break;
	default:
	    break;
	}

	return 0;
}

/**
 * 펫 거래 정보
 * \param &msg 
 * \return 
 */
int OnCS_NC_PETTRADEINFO_ACK(CMsg &msg)
{
	BYTE bResult = 0;	

	msg
		>> bResult;

	if(!bResult)
	{
		DWORD dwAskedID = 0;
		sString sType;
		WORD wLevel		= 0;
		DWORD dwHpMax		= 0;
		WORD wAtkPwr	= 0;
		WORD wDefPwr	= 0;
		WORD wAtkRating = 0;
		WORD wAvoidRatio= 0;
		sString strName;

		msg
			>> g_MainCharInfo.m_dwAskID
			>> dwAskedID
			>> g_MainCharInfo.m_dwReserveID // dwPetID
			>> g_MainCharInfo.m_dwReserveMoney
			>> sType
			>> strName
			>> wLevel
			>> dwHpMax
			>> wAtkPwr
			>> wDefPwr
			>> wAtkRating
			>> wAvoidRatio;

		//HT_0622 : 영수둔갑신단 적용으로 패킷 수정
		//LPCTSTR lpstrTemp;

		//switch(bType)
		//{
		//case 0:
		//	lpstrTemp = IDS_PET_TYPE_0;		break;
		//case 1:
		//	lpstrTemp = IDS_PET_TYPE_1;		break;
		//case 2:
		//	lpstrTemp = IDS_PET_TYPE_2;		break;
		//case 3:
		//	lpstrTemp = IDS_PET_TYPE_3;		break;
		//case 4:
		//	lpstrTemp = IDS_PET_TYPE_4;		break;
		//case 7:
		//	lpstrTemp = IDS_PET_TYPE_7;		break;
		//case 10:
		//	lpstrTemp = IDS_PET_TYPE_10;	break;
		//case 87:
		//	lpstrTemp = IDS_PET_TYPE_87;	break;
		//case 91:
		//	lpstrTemp = IDS_PET_TYPE_91;	break;
		//case 166:
		//	lpstrTemp = IDS_PET_TYPE_166;	break;
		//case 168:
		//	lpstrTemp = IDS_PET_TYPE_168;	break;
		//default:
		//	break;
		//}

		CloseAllWindow();

		//g_pUIManager->SetString(WINDOW_PET_TRADE, window_pet_trade_dummy_11, lpstrTemp, 5);
		g_pUIManager->SetString(WINDOW_PET_TRADE, window_pet_trade_dummy_11, sType, 5);
		g_pUIManager->SetString(WINDOW_PET_TRADE, window_pet_trade_dummy_12, (LPCTSTR)strName, 5);

		g_pUIManager->SetString(WINDOW_PET_TRADE, window_pet_trade_dummy_13, wAtkPwr, 1);
		g_pUIManager->SetString(WINDOW_PET_TRADE, window_pet_trade_dummy_14, wAtkRating, 1);
		g_pUIManager->SetString(WINDOW_PET_TRADE, window_pet_trade_dummy_15, wDefPwr, 1);
		g_pUIManager->SetString(WINDOW_PET_TRADE, window_pet_trade_dummy_16, dwHpMax, 1);

		g_pUIManager->SetString(WINDOW_PET_TRADE, window_pet_trade_dummy_17, (LPCTSTR)MoneyCommaStr(g_MainCharInfo.m_dwReserveMoney), 5);

		g_MainCharInfo.OpenFrame(WINDOW_PET_TRADE);
	}

	return 0;
}

/**
 * 펫 거래
 * \param &msg 
 * \return 
 */
int OnCS_NC_PETTRADE_ACK(CMsg &msg)
{
	BYTE bResult = 0;

	msg
		>> bResult;

	switch(bResult)
	{
	case ERR_PETTRADE_SUCCESS:			// 펫 거래 신청(사용하지 않음)
		break;
	case ERR_PETTRADE_OKTRADE:			// 펫 거래 승락(사용하지 않음)
		break;
	case ERR_PETTRADE_NOTHASPET:		// 펫 없음
		g_MainCharInfo.ShowHelpMessage(IDS_PETTRADE_NOTHASPET, TEXTEFFECT_COLOR_WARNING);
		break;
	case ERR_PETTRADE_HASPET:			// 이미 보유
		g_MainCharInfo.ShowHelpMessage(IDS_PETTRADE_HASPET, TEXTEFFECT_COLOR_WARNING);		
		break;
	case ERR_PETTRADE_TRADEOTHER:		// 거래중
		g_MainCharInfo.ShowHelpMessage(IDS_PETTRADE_TRADEOTHER, TEXTEFFECT_COLOR_WARNING);
		break;
	case ERR_PETTRADE_BONGIN:			// 봉인상태
		g_MainCharInfo.ShowHelpMessage(IDS_CANNOT_DEAL_MONSTER, TEXTEFFECT_COLOR_WARNING);		
		break;
	case ERR_PETTRADE_NOTMONEYENOUGH:	// 돈 부족
		g_MainCharInfo.ShowHelpMessage(IDS_PETTRADE_NOTMONEYENOUGH, TEXTEFFECT_COLOR_WARNING);
		break;
	case ERR_PETTRADE_REFUSETRADE:		// 펫 거래 거부(사용하지 않음)
		break;
	case ERR_PETTRADE_INTERNAL:			// 내부 에러
		g_MainCharInfo.ShowHelpMessage(IDS_REL_ERR_INTERNAL, TEXTEFFECT_COLOR_WARNING);
		break;
	case ERR_PETTRADE_COMPLATE:			//거래 성공
		g_MainCharInfo.ShowHelpMessage(IDS_PETTRADE_COMPLATE);
		break;

	case ERR_PETTRADE_OVERMONEY:		// 소지금 초과
		{
			g_MainCharInfo.ShowHelpMessage(IDS_PETTRADE_OVERMONEY, TEXTEFFECT_COLOR_WARNING);			
		}
		break;

	default:
		break;
	}

	g_MainCharInfo.m_dwAskID = 0;

	return 0;
}



int OnCS_NC_PET_CONTROL_ACK(CMsg &msg)
{
	DWORD dwPetID = 0;
	BYTE bAction = 0;
	BYTE bResult = 0;
	msg >> dwPetID >> bAction >> bResult;

	if (bResult == 0)
	{
		if (bAction == 0)
		{
			g_PetList.DeletePet(dwPetID);
		}
		else if (bAction == 2)
		{
			g_PetList.DeletePet(dwPetID);
			g_MainCharInfo.m_dwResItemID = 0;

			extern std::vector<sPetInfo> g_MyPetList;
			DWORD dwTargetPetID = dwPetID;
			if (dwTargetPetID >= 800000000 && dwTargetPetID < 850000000) dwTargetPetID -= 800000000;
			for (auto it = g_MyPetList.begin(); it != g_MyPetList.end(); ++it) {
				if (it->dwID == dwTargetPetID) {
					g_MyPetList.erase(it);
					break;
				}
			}
		}

		extern void UpdatePetManagerList();
		UpdatePetManagerList();
	}
	else
	{
		TCHAR szErr[64];
		_stprintf(szErr, _T("\xd5\xbd\xb3\xe8\xb2\xd9\xd7\xf7\xca\xa7\xb0\xdc"));
		g_MainCharInfo.ShowHelpMessage(szErr, TEXTEFFECT_COLOR_WARNING);
	}
	return 0;
}
