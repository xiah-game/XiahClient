#include "precompile.h"
#include "XiahGame_Pet.h"
#include "XiahArrayIndex.h"
#include "XiahGame_Handler_Sender.h"
#include "XiahMap.h"
#include "CharacterInfo.h"
#include "InterfaceDefine.h"

//HT_CHEAT : 치트 키
extern  BOOL g_bCheat;

// single tone
CPetList g_PetList;
BOOL	g_bCommandAI = FALSE;			// PET에게 COMMAND를 내리기 위하여 커서의 모양 변화가 있나?
DWORD	g_dwCommandType = PETAI_NONE;	// 어떤 AI의 Command를 내리나?

BOOL ReleasePetInfo(DWORD pInfo)
{
	sPetInfo* pPetInfo = (sPetInfo*)pInfo;

	if(pPetInfo)
	{
		delete pPetInfo;
		pPetInfo = NULL;
	}

	return TRUE;
}


CPetList::CPetList()
{

}

CPetList::~CPetList()
{
	ReleaseWildMark();
	Release();
}

XiahObject::CXiahObject* CPetList::Find(DWORD id)
{
	iterator it = find( id);

	if( it == end())
		return NULL;

	return it->second;
}

CXiahCharObject* CPetList::FindChar(DWORD id)
{
	XiahObject::CXiahObject* pObject = Find( id);

	if( pObject == NULL)
		return NULL;

	return reinterpret_cast<CXiahCharObject*>( pObject->m_pObject);
}

void CPetList::Release()
{
	clear();
}

BOOL CPetList::AddPet(XiahObject::CXiahObject* pPet)
{
	if( Find( pPet->m_dwServerID) != NULL)
		return FALSE;

	sPetInfo* pPetInfo = (sPetInfo*)pPet->m_pObject->m_pPrivateData;
	
	//YS_0811 : BUGFIX
	if ( !pPetInfo ) 
		return FALSE;

	CXiahCharObject* pCharObject = (CXiahCharObject*)pPet->m_pObject;

	//YS_0811 : BUGFIX
	if ( !pCharObject ) 
		return FALSE;

	// 간단한 초기화
    pPetInfo->dwLastAITime = 0;
	pPetInfo->dwAIFrameTime = 600 + rand() % 600;
	pPetInfo->fFollowRange = pCharObject->m_LocalBound.Size().GetLength();
	if (pPetInfo->m_dwIsHwan == 2 || pPetInfo->m_dwIsHwan == 1)
	{
		pPetInfo->fFollowRange = 2.0f;
	}
	else if( pPetInfo->fFollowRange > 20)
	{
		pPetInfo->fFollowRange = 20;
	}

	pPetInfo->fAttackRange = pPetInfo->fFollowRange * 3.9f;

	// 환수유
	if( pPetInfo->m_dwIsHwan == 1 )
		pPetInfo->fAttackRange = pPetInfo->fFollowRange * 4.5f; //HT_CHEAT : 거리를 조금 길게 잡아 준다..

	pPetInfo->bHwanAttack = FALSE;

	// 조금 편법
	pCharObject->SetAnimation( XiahAniType::eLAT_NormalAttack, 0);
	pPetInfo->dwAttackDelayTime =(DWORD) (pCharObject->m_CharRender.GetAnimationLength() / 1.5f); // 1.5배로 빨리 해본다
	pPetInfo->dwLastAttackTime = 0;	

	pCharObject->SetAnimation( XiahAniType::eLAT_Stand, 0);
	
	pCharObject->m_pParentTrigger[ eXCT_OnTimer] = pPetInfo->pTimerTrigger;
	pCharObject->m_pParentTrigger[ eXCT_OnEndTargetMove] = pPetInfo->pEndTargetMove;
	
	pPetInfo->bIdle = FALSE;
	pPetInfo->bFollowPC = FALSE;

	insert( value_type( pPet->m_dwServerID, pPet));
	return TRUE;
}

BOOL CPetList::DeletePet(XiahObject::CXiahObject* pPet)
{
	sPetInfo* pCurrPet = GetCurrentPet();

	if( pCurrPet)
	{
		sPetInfo* pTempPet = (sPetInfo*)pPet->m_pObject->m_pPrivateData;

		//YS_0811 : BUGFIX			
		if ( pTempPet == NULL )
			return FALSE;
		
		if( pCurrPet->dwID == pTempPet->dwID)
		{
			if(g_pUIManager->IsShow(WINDOW_NEW_TAMING))
				g_MainCharInfo.HideSack( SACKTYPE__PET_EQUIP);

			if(g_pUIManager->IsShow(WINDOW_TAMING_ITEM))
				g_MainCharInfo.HideSack( SACKTYPE__PET);
		}
	}
	
	return DeletePet( pPet->m_dwServerID);
}

BOOL CPetList::DeletePet(DWORD id)
{
	iterator it = find( id);

	if( it == end())
		return FALSE;

	erase( it);

	// 해당되는 펫의 상태창이 떠 있을경우만 닫아줘야 하겠지만 그냥 닫아도 무방하겠다
	g_MainCharInfo.CloseFrame( WINDOW_NEW_TAMING);

	return TRUE;
}

extern BOOL PetAI(DWORD dwObjectID,CXiahCharObject* pObject);
BOOL CPetList::UpdatePet()
{
	//HT_CHEAT : 선택한 몬스터가 있는데 멀어서 공격을 못할 경우..
	BOOL PetSeleteMon = true;
	iterator it;
	for(it = begin(); it != end(); it++)
	{
		// 리스트 안의 Pet을 구한후에 각각 AI를 적용한다
		XiahObject::CXiahObject* pObject = it->second;

		if(pObject == NULL)
		{
			DBG_LogFile( _T("CPetList::UpdatePet 실패"));
//			return false;
		}

		CXiahCharObject* pCharObject = reinterpret_cast<CXiahCharObject*>(pObject->m_pObject);

		if(pCharObject == NULL)
		{
			DBG_LogFile( _T("CPetList::UpdatePet 실패"));
//			return false;
		}

		//HT_CHEAT : 선택한 몬스터가 있는데 멀어서 공격을 못할 경우..
		PetSeleteMon = PetAI( it->first, pCharObject);
	}
	//HT_CHEAT : 선택한 몬스터가 있는데 멀어서 공격을 못할 경우..
	return PetSeleteMon;
}

sPetInfo* CPetList::GetPetInfo(DWORD id)
{
	iterator it = find( id);

	if( it == end())
		return NULL;

	XiahObject::CXiahObject* pObject = it->second;

	//YS_0811 : BUGFIX
	if ( pObject == NULL || pObject->m_pObject == NULL )
	{		
		return NULL;
	}

	return (sPetInfo*)pObject->m_pObject->m_pPrivateData;
}

sPetInfo* CPetList::GetPetInfoByIndex(BYTE nIndex)
{
	BYTE index=0;

	iterator it;
	for(it = begin(); it != end(); it++)
	{
		XiahObject::CXiahObject* pObject = it->second;
		if( index == nIndex)
			return (sPetInfo*)pObject->m_pObject->m_pPrivateData;
		else
			index++;
	}

	return NULL;
}

DWORD CPetList::GetPetByHwan()
{
	sPetInfo* pCurrPet = GetCurrentPet();

	if(pCurrPet)
		return pCurrPet->m_dwIsHwan;
	else
		return false;
}


#define STOP_PET	\
			pCharObject->SetAnimation( XiahAniType::eLAT_Stand, 0);	\
			pCharObject->GetAngle( wDirection);\
			SendCS_NC_ENDMOVE_REQ( pObject->m_dwServerID, pCharObject->m_Position.x, -pCharObject->m_Position.z, pCharObject->m_Position.y, wDirection, 0, 18);


BOOL CPetList::JumpToPlayer()
{
	if( g_pMainChar == NULL)
		return TRUE;

	CXiahCharObject* pMainCharObject = (CXiahCharObject*)g_pMainChar->m_pObject;

	if(pMainCharObject == NULL)
	{
		DBG_LogFile( _T("CPetList::JumpToPlayer 실패"));
//		return false;
	}

	iterator it;

	for(it = begin(); it != end(); it++)
	{
		XiahObject::CXiahObject* pObject = it->second;

		if( pObject)
		{
			CXiahCharObject* pCharObject = reinterpret_cast<CXiahCharObject*>(pObject->m_pObject);

			if( pCharObject)
			{
				int x = 6 - (rand() % 12);
				int y = 6 - (rand() % 12);

				pCharObject->SetPosition( pMainCharObject->m_Position.x + x , -pMainCharObject->m_Position.z + y);

				WORD wDirection;
				STOP_PET;
				SendCS_NC_MAPENTER_REQ( pObject->m_dwServerID, XiahMap::g_XiahMap.m_MapInfo.m_dwMapID);
				STOP_PET;
			}
			else
			{
				DBG_LogFile( _T("CPetList::JumpToPlayer 실패"));
//				return false;
			}
		}
		else
		{
			DBG_LogFile( _T("CPetList::JumpToPlayer 실패"));
//			return false;
		}
	}
	return TRUE;
}

// 해당 Pet을 선택
BOOL CPetList::SelectPet(DWORD id)
{

	DeSelectAllPet();

	sPetInfo* pinfo = GetPetInfo(id);

	if(pinfo)
	{
		if(pinfo->m_dwIsHwan == 0)
			pinfo->bSelected = TRUE;
	}
	else
		return FALSE;

	return TRUE;
}

// 해당 Pet을 선택하지 않는다
BOOL CPetList::DeSelectPet(DWORD id)
{
	sPetInfo* pinfo = GetPetInfo(id);

	if(pinfo)
	{
		pinfo->bSelected = FALSE;
	}
	else
		return FALSE;

	return TRUE;
}

// 가지고 있는 모든 PET을 선택
BOOL CPetList::SelectAllPet()
{
	iterator it;
	sPetInfo* tinfo = NULL;

	for(it = begin(); it != end(); it++)
	{
		XiahObject::CXiahObject* pObject = it->second;

		if( pObject)
		{
			tinfo = (sPetInfo*)pObject->m_pObject->m_pPrivateData;
			
			//YS_0811 : BUGFIX
			if ( tinfo )
				tinfo->bSelected = TRUE;
		}		
	}

	return TRUE;
}

// 가지고 있는 모든 PET을 선택을 푼다
BOOL CPetList::DeSelectAllPet()
{
	if(g_pUIManager->IsShow(WINDOW_NEW_TAMING))
	{
		g_MainCharInfo.HideSack( SACKTYPE__PET_EQUIP);
	}

	if(g_pUIManager->IsShow(WINDOW_TAMING_ITEM))
	{
		g_MainCharInfo.HideSack( SACKTYPE__PET);
	}
	
	iterator it;
	sPetInfo* tinfo = NULL;

	for(it = begin(); it != end(); it++)
	{
		XiahObject::CXiahObject* pObject = it->second;

		if( pObject)
		{
			tinfo = (sPetInfo*)pObject->m_pObject->m_pPrivateData;

			//YS_0811 : BUGFIX
			if ( tinfo )
			{			
				tinfo->bSelected = FALSE;
				if( tinfo->m_pEquipSack && tinfo->m_pEquipSack->IsShow())
					tinfo->m_pEquipSack->HideSack();
				if( tinfo->m_pSack[tinfo->m_byMySackCurrIdx] && tinfo->m_pSack[tinfo->m_byMySackCurrIdx]->IsShow())
					tinfo->m_pSack[tinfo->m_byMySackCurrIdx]->HideSack();
			}
		}	
	}
	return TRUE;
}

// 선택되어진 PET에게 AI를 정하여 준다
BOOL CPetList::Change_PET_AI(int Type, DWORD di,DWORD gi,DWORD dt,DWORD gt)
{
	iterator it;
	sPetInfo* tinfo = NULL;

	for(it = begin(); it != end(); it++)
	{
		XiahObject::CXiahObject* pObject = it->second;

		if( pObject)
		{
			tinfo = (sPetInfo*)pObject->m_pObject->m_pPrivateData;

			//YS_0811 : BUGFIX
			if(tinfo == NULL)
				continue;

			if(tinfo->bAI == TRUE)
			{
				tinfo->AI_Type = Type;
				tinfo->dwDestID = di;
				tinfo->dwGuardID = gi;
				tinfo->dwDestType = dt;
				tinfo->dwGuardType = gt;

				if(di == NULL && gi == NULL && dt == NULL && gt == NULL && !g_bCheat)
				{
					TCHAR temp[64] = {0,};
					switch( Type)
					{
					case PETAI_AUTOATTACK:
						_stprintf( temp, IDS_AUTO_ATTK_MODE, (LPCTSTR)tinfo->szName);
						break;
					case PETAI_TARGETATTACK:
						_stprintf( temp, IDS_OBJECT_ATTK_MODE, (LPCTSTR)tinfo->szName);
						break;
					case PETAI_TAKEITEM:
						_stprintf( temp, IDS_COLLECT_ITEM_MODE, (LPCTSTR)tinfo->szName);
						break;
					case PETAI_SPECIALATTACK:
						_stprintf( temp, IDS_MUGONG_ATTK_MODE, (LPCTSTR)tinfo->szName);
						break;
					case PETAI_CALLTOME:
						_stprintf( temp, IDS_CALL_MONSTER, (LPCTSTR)tinfo->szName);
						break;

					default:
						_stprintf( temp, IDS_CHANGEMODE_, (LPCTSTR)tinfo->szName);
						break;
					}
					g_MainCharInfo.ShowHelpMessage( temp);
				}
			}
		}
		else
		{
			DBG_LogFile( _T("CPetList::Change_PET_AI 실패"));
//			return false;
		}
	}
	return TRUE;
}

// 해당 PET에게 AI를 정하여 준다
BOOL CPetList::Change_PET_AI_Specify(DWORD PetID, int Type, DWORD di,DWORD gi,DWORD dt,DWORD gt)
{
	iterator it;
	sPetInfo* tinfo = NULL;

	for(it = begin(); it != end(); it++)
	{
		XiahObject::CXiahObject* pObject = it->second;

		if( pObject)
		{
			//YS_0811 : BUGFIX
			tinfo = (sPetInfo*)pObject->m_pObject->m_pPrivateData;
			if( tinfo && tinfo->dwID == PetID)
			{
				tinfo->AI_Type = Type;
				tinfo->dwDestID = di;
				tinfo->dwGuardID = gi;
				tinfo->dwDestType = dt;
				tinfo->dwGuardType = gt;
			}
		}
	}

	return TRUE;
}

sPetInfo* CPetList::GetCurrentPet()
{
	iterator it;
	sPetInfo* tinfo = NULL;
	for(it = begin(); it != end(); it++)
	{
		XiahObject::CXiahObject* pObject = it->second;

		//YS_0812 : BUGFIX
		if( pObject && pObject->m_pObject )
		{
			tinfo = (sPetInfo*)pObject->m_pObject->m_pPrivateData;
			if ( tinfo ) 
				return tinfo;
		}
		else
		{
			DBG_LogFile( _T("CPetList::GetCurrentPet 실패"));
//			return false;
		}
	}
	return NULL;
}

// 분신격 PET을 얻는다
sPetInfo* CPetList::GetBunsinPet()
{
	iterator it;
	sPetInfo* tinfo = NULL;

	for(it = begin(); it != end(); it++)
	{
		XiahObject::CXiahObject* pObject = it->second;

		if( pObject)
		{			
			tinfo = (sPetInfo*)pObject->m_pObject->m_pPrivateData;

			//YS_0811 : BUGFIX
			if( tinfo && tinfo->m_dwIsHwan == 2 ) 
				return tinfo;
		}
		else
		{
			DBG_LogFile( _T("CPetList::GetBunsinPet 실패"));
//			return false;
		}
	}
	return NULL;
}

//----------------------------------------------------------------------------------------------------
// Name: InitWild
//----------------------------------------------------------------------------------------------------
void CPetList::InitWild()
{
	g_pDirect3DDevice->CreateVertexBuffer( 4*sizeof(VT_TLVertex), 0, D3DFVF_TLVERTEX, D3DPOOL_MANAGED, &m_pVB, NULL);

	m_pTexture[0] = XiahPak::GetTexture( 50000527, TRUE);
	m_pTexture[1] = XiahPak::GetTexture( 50000528, TRUE);
	m_pTexture[2] = XiahPak::GetTexture( 50000526, TRUE);
}

//----------------------------------------------------------------------------------------------------
// Name: RenderWild
// Parameters: 
// Desc: 
// [return] 
//----------------------------------------------------------------------------------------------------
void CPetList::RenderWild(int nX, int nY, int nMark)
{
	VT_TLVertex	Vertex[4];
	
	D3DCOLOR d3dcolor = D3DCOLOR_ARGB( 100, 255, 255, 255);

	float fX = nX - 0.5;
	float fY = nY - 0.5;

	Vertex[ 0].pos = Vector4( fX, fY, 0, 1);
	Vertex[ 1].pos = Vector4( fX + 15, fY, 0, 1);
	Vertex[ 2].pos = Vector4( fX, fY+15, 0, 1);
	Vertex[ 3].pos = Vector4( fX + 15, fY + 15, 0, 1);

	Vertex[ 0].diffuse = d3dcolor;
	Vertex[ 1].diffuse = d3dcolor;
	Vertex[ 2].diffuse = d3dcolor;
	Vertex[ 3].diffuse = d3dcolor;

	Vertex[ 0].tex = Vector2( 0, 0);
	Vertex[ 1].tex = Vector2( 1, 0);
	Vertex[ 2].tex = Vector2( 0, 1);
	Vertex[ 3].tex = Vector2( 1, 1);

	VOID* pVertices = NULL;
	if( !FAILED( m_pVB->Lock( 0, sizeof(Vertex), (void**)&pVertices, 0 )))
	{
		memcpy( pVertices, Vertex, sizeof(Vertex) );
		m_pVB->Unlock();
	}

	if(nMark >= 0 && nMark <= 50)
		g_Device.SetTexture(0, m_pTexture[0]);
		//g_pDirect3DDevice->SetTexture( 0, m_pTexture[0]);
	else if(nMark >= 51 && nMark <= 80)
		g_Device.SetTexture(0, m_pTexture[1]);
		//g_pDirect3DDevice->SetTexture( 0, m_pTexture[1]);
	else if(nMark >= 81 && nMark <= 999)
		g_Device.SetTexture(0, m_pTexture[2]);
		//g_pDirect3DDevice->SetTexture( 0, m_pTexture[2]);

	g_Device.SetStreamSource( m_pVB, sizeof(VT_TLVertex));
	g_Device.SetFVF(D3DFVF_TLVERTEX);
	//g_pDirect3DDevice->SetFVF( D3DFVF_TLVERTEX);
	g_pDirect3DDevice->DrawPrimitive( D3DPT_TRIANGLESTRIP, 0, 2);
}

//----------------------------------------------------------------------------------------------------
// Name: ReleaseWildMark
//----------------------------------------------------------------------------------------------------
void CPetList::ReleaseWildMark()
{
	if(m_pVB)
	m_pVB->Release();

	XiahPak::ReleaseRes( 50000527);
	XiahPak::ReleaseRes( 50000528);
	XiahPak::ReleaseRes( 50000526);

	m_pTexture[0] = NULL;
	m_pTexture[1] = NULL;
	m_pTexture[2] = NULL;
}
