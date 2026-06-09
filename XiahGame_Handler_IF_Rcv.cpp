#include "csprotocol.h"
#include "mail.h"

#include ".\spirit.h"
#include "RebirthMark.h"

//HT_CHEAT : 치트 키
extern BOOL g_bCheat;

//HT_CHEAT : 자동 판매
void AutoSell()
{
 	XiahItem::sItemInfo* pItemInfo = NULL;

	int i = 12;
	while(1)
	{
		pItemInfo = g_MainCharInfo.m_pMySack[0]->FindSackItemByPos(i);

		if(pItemInfo && i < 36)
		{
			g_MainCharInfo.m_bySellSackPos = 0;
			switch(pItemInfo->m_bItemType)
			{
			case 0:
			case 1:
			case 2:
			case 3:
			case 4:
			case 5:
			case 6:
			case 7: //착용 아이템들 인첸 체크 하기 
				//if(pItemInfo->m_bRarity < 3 && pItemInfo->m_bStxType < 3 && pItemInfo->m_bModifyCnt < 1)
				if(pItemInfo->m_bModifyCnt < 1)
				{
					g_MainCharInfo.m_bAutoSell = true;
					g_MainCharInfo.m_bySellPos = i;
					return;
				}
				break;
			case 9: //봉인 단지는 팔자 
			//	if(pItemInfo->m_bItemKind != 5)
			//	{
					g_MainCharInfo.m_bAutoSell = true;
					g_MainCharInfo.m_bySellPos = i;
					return;
			//	}
			case 15: //배낭
				g_MainCharInfo.m_bAutoSell = true;
				g_MainCharInfo.m_bySellPos = i;
				return;
			case 16://펫 먹이 
				if(pItemInfo->m_bItemKind >= 0 && pItemInfo->m_bItemKind < 4)// && pItemInfo->m_wVisualID != 9102)
				{
					if(g_PetList.size() == 0)
					{
						g_MainCharInfo.m_bAutoSell = true;
						g_MainCharInfo.m_bySellPos = i;
						return;
					}
					else
					{
						sPetInfo* pPetInfo = g_PetList.GetCurrentPet();

						if(pPetInfo)
						{
							if(pPetInfo->m_dwIsHwan != 0)
							{
								g_MainCharInfo.m_bAutoSell = true;
								g_MainCharInfo.m_bySellPos = i;
								return;
							}
							else if(pItemInfo->m_wVisualID != 9102) //펫을 키우고 있다면...펫 야생성 줄이는 송충이는 팔지 말자. 
							{
								g_MainCharInfo.m_bAutoSell = true;
								g_MainCharInfo.m_bySellPos = i;
								return;
							}
						}
					}
				}
				break;	
			case 18: // 낭 아이템
				//if(	pItemInfo->m_wRefID != 20963 &&
				//	pItemInfo->m_wRefID != 20965 &&
				//	pItemInfo->m_wRefID != 20966)
				//{
					g_MainCharInfo.m_bAutoSell = true;
					g_MainCharInfo.m_bySellPos = i;
					return;
			//	}
			case 19://잡다한 것들(합체 아템들 ....)
				if(pItemInfo->m_wVisualID == 31000) //선물 상자는 먹으면 우클릭
				{
					SendCS_IM_USEITEM_REQ( pItemInfo->m_bSackCount+1, pItemInfo->m_bSackPos, pItemInfo->m_dwItemID);
				}

				if(	pItemInfo->m_wRefID == 21040 || pItemInfo->m_wRefID == 22118)
				{
					g_MainCharInfo.m_bAutoSell = true;
					g_MainCharInfo.m_bySellPos = i;
					return;
				}
				break;
			case 20: //수집 아이템들
			//	if(pItemInfo->m_wVisualID != 29200 && pItemInfo->m_wVisualID != 29201 )
			//	{
					g_MainCharInfo.m_bAutoSell = true;
					g_MainCharInfo.m_bySellPos = i;
					return;
			//	}	break;
			case 21: //책
				if( !(pItemInfo->m_wRefID >= 22017 && pItemInfo->m_wRefID <= 22103) )
				{
					if( pItemInfo->m_wRefID != 10128 &&
					    pItemInfo->m_wRefID != 10129 &&
					    pItemInfo->m_wRefID != 10134 &&
					    pItemInfo->m_wRefID != 20633 &&
					    pItemInfo->m_wRefID != 20636 &&
					    pItemInfo->m_wRefID != 20639 &&
					    pItemInfo->m_wRefID != 20642 &&
					    pItemInfo->m_wRefID != 20645 )
					{
						g_MainCharInfo.m_bAutoSell = true;
						g_MainCharInfo.m_bySellPos = i;
						return;
					}
				}
				break;
			case 23: //약
				//if( pItemInfo->m_wVisualID == 20000 ||
				//	pItemInfo->m_wVisualID == 20100 ||
				//	pItemInfo->m_wVisualID == 21000 ||
				//	pItemInfo->m_wVisualID == 21100)
				//{
					//if(pItemInfo->m_wRefID != 20073 && pItemInfo->m_wRefID  != 20070 ) //대짤은 좀 아깝당. ㅋ
					//if(pItemInfo->m_wRefID == 20073 || pItemInfo->m_wRefID  == 20070 ) //대짤은 좀 아깝당. ㅋ
					//{
						g_MainCharInfo.m_bAutoSell = true;
						g_MainCharInfo.m_bySellPos = i;
						return;
					//}
				//}
			case 25: //돌맹이
				if( pItemInfo->m_wRefID == 20189 || pItemInfo->m_wRefID == 20267 )
				{
					g_MainCharInfo.m_bAutoSell = true;
					g_MainCharInfo.m_bySellPos = i;
					return;
				}
				break;
			case 33: //개점패
				if(pItemInfo->m_wVisualID == 10200)
				{
					g_MainCharInfo.m_bAutoSell = true;
					g_MainCharInfo.m_bySellPos = i;
					return;
				}
				break;
			}
			i++;
			if(i > 35) 	break;
		}
		else
		{
			i++;
			if(i > 35) 	break;
		}
	}
	i=0;
	while(1)
	{
		pItemInfo = g_MainCharInfo.m_pMySack[1]->FindSackItemByPos(i);

		if(pItemInfo && i < 36)
		{
			g_MainCharInfo.m_bySellSackPos = 1;

			switch(pItemInfo->m_bItemType)
			{
			case 0:
			case 1:
			case 2:
			case 3:
			case 4:
			case 5:
			case 6:
			case 7: //착용 아이템들 인첸 체크 하기 
				//if(pItemInfo->m_bRarity < 3 && pItemInfo->m_bStxType < 3 && pItemInfo->m_bModifyCnt < 1)
				if(pItemInfo->m_bModifyCnt < 1)
				{
					g_MainCharInfo.m_bAutoSell = true;
					g_MainCharInfo.m_bySellPos = i;
					return;
				}
				break;
			case 9: //봉인 단지는 팔자 
			//	if(pItemInfo->m_bItemKind == 0)
			//	{
					g_MainCharInfo.m_bAutoSell = true;
					g_MainCharInfo.m_bySellPos = i;
					return;
			//	}
			case 15: //배낭
				g_MainCharInfo.m_bAutoSell = true;
				g_MainCharInfo.m_bySellPos = i;
				return;
			case 16://펫 먹이 
				if(pItemInfo->m_bItemKind >= 0 && pItemInfo->m_bItemKind < 4)// && pItemInfo->m_wVisualID != 9102)
				{
					if(g_PetList.size() == 0)// && pItemInfo->m_wVisualID != 9102) //펫 키우는 중이 아니다.
					{
						g_MainCharInfo.m_bAutoSell = true;
						g_MainCharInfo.m_bySellPos = i;
						return;
					}
					else
					{
						sPetInfo* pPetInfo = g_PetList.GetCurrentPet();

						if(pPetInfo)
						{
							if(pPetInfo->m_dwIsHwan != 0)
							{
								g_MainCharInfo.m_bAutoSell = true;
								g_MainCharInfo.m_bySellPos = i;
								return;
							}
							else if(pItemInfo->m_wVisualID != 9102) //펫을 키우고 있다면...펫 야생성 줄이는 간은 팔지 말자. 
							{
								g_MainCharInfo.m_bAutoSell = true;
								g_MainCharInfo.m_bySellPos = i;
								return;
							}
						}
					}
				}
				break;
			case 18: // 낭 아이템
				//if(	pItemInfo->m_wRefID != 20963 &&
				//	pItemInfo->m_wRefID != 20965 &&
				//	pItemInfo->m_wRefID != 20966)
				//{
					g_MainCharInfo.m_bAutoSell = true;
					g_MainCharInfo.m_bySellPos = i;
					return;
			//	}
			case 19://잡다한 것들(합체 아템들 ....)
				if(pItemInfo->m_wVisualID == 31000) //선물 상자는 먹으면 우클릭
				{
					SendCS_IM_USEITEM_REQ( pItemInfo->m_bSackCount+1, pItemInfo->m_bSackPos, pItemInfo->m_dwItemID);
				}

				if(	pItemInfo->m_wRefID == 21040 || pItemInfo->m_wRefID == 22118)
				{
					g_MainCharInfo.m_bAutoSell = true;
					g_MainCharInfo.m_bySellPos = i;
					return;
				}
				break;
			case 20: //수집 아이템들
			//	if(pItemInfo->m_wVisualID != 29200 && pItemInfo->m_wVisualID != 29201 )
			//	{
					g_MainCharInfo.m_bAutoSell = true;
					g_MainCharInfo.m_bySellPos = i;
					return;
			//	}	break;
			case 21: //책
				if( !(pItemInfo->m_wRefID >= 22017 && pItemInfo->m_wRefID <= 22103) )
				{
					if( pItemInfo->m_wRefID != 10128 &&
					    pItemInfo->m_wRefID != 10129 &&
					    pItemInfo->m_wRefID != 10134 &&
					    pItemInfo->m_wRefID != 20633 &&
					    pItemInfo->m_wRefID != 20636 &&
					    pItemInfo->m_wRefID != 20639 &&
					    pItemInfo->m_wRefID != 20642 &&
					    pItemInfo->m_wRefID != 20645 )
					{
						g_MainCharInfo.m_bAutoSell = true;
						g_MainCharInfo.m_bySellPos = i;
						return;
					}
				}
				break;
			case 23: //약
				//if( pItemInfo->m_wVisualID == 20000 ||
				//	pItemInfo->m_wVisualID == 20100 ||
				//	pItemInfo->m_wVisualID == 21000 ||
				//	pItemInfo->m_wVisualID == 21100)
				//{
					//if(pItemInfo->m_wRefID != 20073 && pItemInfo->m_wRefID  != 20070 ) //대짤은 좀 아깝당. ㅋ
					//if(pItemInfo->m_wRefID == 20073 || pItemInfo->m_wRefID  == 20070 ) //대짤은 좀 아깝당. ㅋ
					//{
						g_MainCharInfo.m_bAutoSell = true;
						g_MainCharInfo.m_bySellPos = i;
						return;
					//}
				//}
			case 25: //돌맹이
				if( pItemInfo->m_wRefID == 20189 || pItemInfo->m_wRefID == 20267 )
				{
					g_MainCharInfo.m_bAutoSell = true;
					g_MainCharInfo.m_bySellPos = i;
					return;
				}
				break;
			case 33: //개점패
				if(pItemInfo->m_wVisualID == 10200)
				{
					g_MainCharInfo.m_bAutoSell = true;
					g_MainCharInfo.m_bySellPos = i;
					return;
				}
				break;
			}
			i++;
			if(i > 35) 	break;
		}
		else
		{
			i++;
			if(i > 35) 	break;
		}
	}
}

///////////////////////////////////////
int OnCS_IF_HELPMESSAGE_ACK( CMsg &msg)
///////////////////////////////////////
{
	WORD	wMsgIndex;	
	DWORD	dwAmount;
	sString szName;
	sString szNickName;//HO_0313_07 광명전 업데이트로 인한 추가
	TCHAR szContent[256] = {0,};

	msg
		>> wMsgIndex;

	switch( wMsgIndex)
	{
	case HM_PICKITEM:
		{
			msg
				>> szName
				>> dwAmount;
			
			if( _tcscmp(szName,JUN_MONEY) == NULL)
				_stprintf( szContent, IDS_D_OBTAIN_MONEY, dwAmount);
			else
				_stprintf( szContent, IDS_D_OBTAIN_ITEM, (LPCTSTR)szName);

			//HT_CHEAT : 자동 판매
			//if(g_bCheat)
				//AutoSell();

			if(g_MainCharInfo.m_bAutoSell)
			{
				XiahItem::sItemInfo* pItemInfo = NULL;
				pItemInfo = g_MainCharInfo.m_pMySack[g_MainCharInfo.m_bySellSackPos]->FindSackItemByPos(g_MainCharInfo.m_bySellPos);
				if(pItemInfo)
				{
					g_MainCharInfo.ShowHelpMessage(pItemInfo->m_szName);
					SendCS_EC_SELLITEM_REQ( 984, 
												pItemInfo->m_dwItemID,
												pItemInfo->m_bSackCount+1,
												pItemInfo->m_bSackPos);

				}
				else
				{
					g_MainCharInfo.m_bAutoSell = false;
					g_MainCharInfo.m_bySellPos = 0;
				}
			}

			g_MainCharInfo.ShowHelpMessage( szContent,TEXTEFFECT_COLOR_GAIN);
			g_MainCharInfo.PlayInterfaceSound( ISOUND_ITEM_PICKUP);
		}
		break;
	case HM_BOSSKILL:
		{
			msg
				>> dwAmount;

			_stprintf( szContent, IDS_VICELEVEL_DEC, dwAmount);

			g_MainCharInfo.ShowHelpMessage( szContent,TEXTEFFECT_COLOR_GAIN);			
		}
		break;

	case HM_PKDROP:
		{
			msg
				>> szName
				>> dwAmount;

			_stprintf(szContent, IDS_ITEM_LOSE, szName.data() );

			g_MainCharInfo.ShowHelpMessage( szContent,TEXTEFFECT_COLOR_GAIN);			
		}
		break;

	// PARTY BATTLE에러시 돈 환급에 관한 메세지
	case HM_PARTYBATTLEMONEY:
		{
			msg
				>> dwAmount;

			_stprintf(szContent, IDS_PARTYBATTLEMONEY_BACK);
			g_MainCharInfo.ShowHelpMessage( szContent,TEXTEFFECT_COLOR_GAIN);
			_stprintf(szContent, IDS_PARTYBATTLEMONEY_BACK_V,dwAmount );
			g_MainCharInfo.ShowHelpMessage( szContent,TEXTEFFECT_COLOR_GAIN);
		}
		break;
	case 5:	// 장착
		{
			g_MainCharInfo.ShowHelpMessage(IDS_PET_ITEM_1);			
		}
		break;
	case 6:	// 
		{
			g_MainCharInfo.ShowHelpMessage(IDS_PET_ITEM_2);
		}
		break;
	
	case HM_BLOODDEVIL://HO_0313_07 광명전 업데이트로 인한 추가
		{
			msg
				>> szName
				>> szNickName;

			_stprintf(szContent, IDS_EVENTITEM_BLOODDEVIL, (LPCTSTR)szNickName, (LPCTSTR)szName);
			
			g_MainCharInfo.SpecialChatMessage(szContent,TEXTEFFECT_COLOR_GAIN);
		}
		break;
	}
	
	return 0;
}

//////////////////////////////////////
int OnCS_IF_SCHOOLLIST_ACK( CMsg &msg)
//////////////////////////////////////
{
	return TRUE;
}



int OnCS_IF_CHARMONEY_ACK(CMsg &msg)
{
	INT64 dwMoney;
	BYTE bFreeUser;
	
	msg
		>> dwMoney
		>> bFreeUser;

	g_MainCharInfo.m_dwMoney = dwMoney;
//	g_MainCharInfo.m_i64Money = dwMoney;
	g_MainCharInfo.RefreshItemFrame();

	return TRUE;
}




int OnCS_IF_CHARINFO_ACK( CMsg &msg)
{
	// CG_2005/01/28 : 변종아이템기능추가
	// 변종아이템일때 체크
	BYTE bChangeItemSet = 0;

	msg
		>> g_MainCharInfo.m_wLevel
		>> g_MainCharInfo.m_wStr
		>> g_MainCharInfo.m_wSus
		>> g_MainCharInfo.m_wDex
		>> g_MainCharInfo.m_wVit
		>> g_MainCharInfo.m_wRemainSp
		>> g_MainCharInfo.m_dwTotalSp
		>> g_MainCharInfo.m_dwTotalAtkPower
		>> g_MainCharInfo.m_dwTotalDefPower
		>> g_MainCharInfo.m_dwTotalAttackRating
		>> g_MainCharInfo.m_bState
		>> g_MainCharInfo.m_bWalkSpeed
		>> g_MainCharInfo.m_dwHpCur
		>> g_MainCharInfo.m_dwHpMax
		>> g_MainCharInfo.m_wIpCur
		>> g_MainCharInfo.m_wIpMax
		>> g_MainCharInfo.m_wCritical
		>> g_MainCharInfo.m_wBaseAtkPwr
		>> g_MainCharInfo.m_wBaseDefPwr
		>> g_MainCharInfo.m_wBaseAttackRating
		>> g_MainCharInfo.m_bAttackSpeed
		>> g_MainCharInfo.m_wAttackRange
		>> g_MainCharInfo.m_bPlusSpeed
		>> g_MainCharInfo.m_wRemainTp
		>> g_MainCharInfo.m_dwTotalTp
		>> g_MainCharInfo.m_dwFame
		>> bChangeItemSet
		>> g_MainCharInfo.m_bRebirth
		>> g_MainCharInfo.m_dwPremiumTP //HT_0911 : 프리미엄 퀘스트 수련치
		>> g_MainCharInfo.m_dwPremiumSP
		>> g_MainCharInfo.m_dwGameMasterMark; //HT_1023 : 운영자 마크 추가
	
	g_MainCharInfo.RefreshChracterInfo();
	g_MainCharInfo.RefreshMainFrame();
	g_MainCharInfo.RefreshMugongFrame();

	// 분신격이면 바꿔준다.
	sPetInfo *pPet = g_PetList.GetBunsinPet();
	if(pPet != NULL)
	{
		pPet->bSpeed = g_MainCharInfo.m_bWalkSpeed;
	}

	// useitem_ack가 없는 관계로..
	for( int i=0; i < 5; ++i)
		g_MainCharInfo.m_pSlot->SetSlotToolTip(i);

	// CG_2005/01/28 : 변종아이템기능추가
	CXiahCharObject* pObject = (CXiahCharObject*)g_pMainChar->m_pObject;
	if( pObject )
	{
		pObject->m_bChangeItemSet = bChangeItemSet;
		pObject->RefreshFameColor(g_MainCharInfo.m_dwFame);
		pObject->m_bRebirth = g_MainCharInfo.m_bRebirth;
		pObject->m_bGameMasterMark = g_MainCharInfo.m_dwGameMasterMark; //HT_1023 : 운영자 마크 추가
	}

	return TRUE;
}


#define USE_HP		50001217
#define USE_MP		50001218
int OnCS_IF_CHARHP_ACK( CMsg &msg)
{
	BYTE bType = 0;

	msg
		>> g_MainCharInfo.m_dwHpMax
		>> g_MainCharInfo.m_dwHpCur
		>> g_MainCharInfo.m_wIpMax
		>> g_MainCharInfo.m_wIpCur
		>> bType;

	g_MainCharInfo.RefreshMainFrame();
	g_MainCharInfo.RefreshChracterInfo();

	if( g_pMainChar == NULL || g_pMainChar->m_pObject == NULL ) return 0;

	CXiahCharObject* pCharObject = (CXiahCharObject*)g_pMainChar->m_pObject;
	if( pCharObject == NULL ) return 0;

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
//				pEffectPackage->pEffectRender->pPackagePair->WorldMatrix = (MATRIX)pCharObject->m_ObjectTM;

				// 이케해야 캐릭터가 없어질때 이펙트도 같이 없어진다.
				pCharObject->m_EffectPPList.push_back( pEffectPackage->pEffectRender->pPackagePair );
			}

			g_MainCharInfo.PlayInterfaceSound(USE_HP);
		}
		break;
	case 2:		// IP
		{
			_EFFECTPACKAGE* pEffectPackage = g_EffectManager.EnqAppearEffectImmediately( eMulYak_IP );

			if( pEffectPackage && pEffectPackage->pEffectRender && pEffectPackage->pEffectRender->pPackagePair )
			{
				pEffectPackage->pEffectRender->pPackagePair->pWorldMatrix = (MATRIX*)pCharObject->m_CharRender.GetCharTM();
//				pEffectPackage->pEffectRender->pPackagePair->WorldMatrix = (MATRIX)pCharObject->m_ObjectTM;

				pCharObject->m_EffectPPList.push_back( pEffectPackage->pEffectRender->pPackagePair );
			}
			g_MainCharInfo.PlayInterfaceSound(USE_MP);
		}
		break;
	case 3:		// HP, IP
		{
			_EFFECTPACKAGE* pEffectPackage = g_EffectManager.EnqAppearEffectImmediately( eMulYak_HPIP );

			if( pEffectPackage && pEffectPackage->pEffectRender && pEffectPackage->pEffectRender->pPackagePair )
			{
				pEffectPackage->pEffectRender->pPackagePair->pWorldMatrix = (MATRIX*)pCharObject->m_CharRender.GetCharTM();
//				pEffectPackage->pEffectRender->pPackagePair->WorldMatrix = (MATRIX)pCharObject->m_ObjectTM;

				pCharObject->m_EffectPPList.push_back( pEffectPackage->pEffectRender->pPackagePair );
			}
			g_MainCharInfo.PlayInterfaceSound(USE_HP);
			g_MainCharInfo.PlayInterfaceSound(USE_MP);
		}
		break;
		// 무공
	case 11:	// 전유음 - 타인 치료.
		{
			_EFFECTPACKAGE* pEffectPackage = g_EffectManager.EnqOutGongPersistEffectImmediately( eJunuoum_recv );

			if( pEffectPackage && pEffectPackage->pEffectRender && pEffectPackage->pEffectRender->pPackagePair )
			{
				pEffectPackage->pEffectRender->pPackagePair->pWorldMatrix = (MATRIX*)pCharObject->m_CharRender.GetCharTM();
//				pEffectPackage->pEffectRender->pPackagePair->WorldMatrix = (MATRIX)pCharObject->m_ObjectTM;

				pCharObject->m_EffectPPList.push_back( pEffectPackage->pEffectRender->pPackagePair );
			}
		}
		break;
	case 12:	// 이광음 - 단 치료.
		{
			_EFFECTPACKAGE* pEffectPackage = g_EffectManager.EnqOutGongPersistEffectImmediately( eLeekwangum_heal_recv );

			if( pEffectPackage && pEffectPackage->pEffectRender && pEffectPackage->pEffectRender->pPackagePair )
			{
				pEffectPackage->pEffectRender->pPackagePair->pWorldMatrix = (MATRIX*)pCharObject->m_CharRender.GetCharTM();
//				pEffectPackage->pEffectRender->pPackagePair->WorldMatrix = (MATRIX)pCharObject->m_ObjectTM;

				pCharObject->m_EffectPPList.push_back( pEffectPackage->pEffectRender->pPackagePair );
			}
		}
		break;
	};

	return TRUE;
}

/**
 * 경험치
 * \param &msg 
 * \return 
 */
int OnCS_IF_CHAREXP_ACK( CMsg &msg)
{	
	INT64 n64Exp				=0;
	INT64 i64NextLevelUpExp		=0;
	INT64 i64NextTpUpExp		=0;	
	BYTE bLevelUp				=0;
	BYTE bTpUp					=0;
	BYTE bFiveElmLevelUp		=0;
	WORD wFiveElmPoint			=0;
	WORD wFiveElmExp			=0;
	DWORD dwIncrExp				=0;
	DWORD dwEventExp			=0;
	DWORD dwFiveElmPower		=0;
	DWORD dwFiveElmPowerMax		=0;
	DWORD dwFiveElmGauge		=0;	

	msg
		>> dwIncrExp
		>> n64Exp
		>> bLevelUp
		>> bTpUp
		>> bFiveElmLevelUp
		>> i64NextLevelUpExp
		>> i64NextTpUpExp
		>> wFiveElmPoint
		>> dwFiveElmPower
		>> dwFiveElmPowerMax
		>> dwFiveElmGauge
		>> dwEventExp
		>> wFiveElmExp;

	if(dwIncrExp)
	{
		TCHAR szContent[64] = {0,};

		//100갑자 초과시 제한
		if(g_MainCharInfo.m_wLevel < 150)
		{
            if(dwEventExp)
                _stprintf( szContent, IDS_D_OBTAIN_EVENT_EXP, dwIncrExp, dwEventExp);
            else
                _stprintf( szContent, IDS_D_OBTAIN_EXP, dwIncrExp);
            
			g_MainCharInfo.ShowHelpMessage(szContent, TEXTEFFECT_COLOR_GAIN);
		}
	}

	if(wFiveElmExp)
	{
		TCHAR szExp[64] = {0,};
		_stprintf(szExp, IDS_FE_EXP, wFiveElmExp);
		
		g_MainCharInfo.ShowHelpMessage(szExp, TEXTEFFECT_COLOR_GAIN);
	}

	g_MainCharInfo.m_i64Exp = n64Exp;

	if( bLevelUp)
	{
		++g_MainCharInfo.m_wLevel;

		//HO_0413_07 퀵 가이드 업데이트 : 렙 체크후 버튼 지움
		if(g_MainCharInfo.m_wLevel < 11)
		{
			g_MainCharInfo.m_bQuickIndex = 0;
			CloseAllWindow();
			if(g_MainCharInfo.m_bShowMiniMap = TRUE)  
				g_MainCharInfo.ShowMiniMap( TRUE);

			g_MainCharInfo.OpenFrame( WINDOW_HELPER_LIST0);
			
			g_pUIManager->Hide(HELP_BUTTON);  

//			g_pUIManager->Show(WINDOW_HELPER_LIST0, helper_window0_button_01);
//			g_pUIManager->Show(WINDOW_HELPER_LIST0, helper_window0_button_02);

			sArrayData* pQuickScript = XiahArrayIndex::g_QuickIndex.GetData(g_MainCharInfo.m_wLevel);
			
			if(pQuickScript)
			{
				sString strTemp = pQuickScript->GetString(g_MainCharInfo.m_bQuickIndex);
				TCHAR szLevel[50]={0,};
				_stprintf( szLevel, IDS_D_GAPJA, g_MainCharInfo.m_wLevel);
			
				g_pUIManager->SetString(WINDOW_HELPER_LIST0, helper_window0_small_title_dummy, szLevel);
				g_pUIManager->SetString(WINDOW_HELPER_LIST0, helper_window0_list_dummy, strTemp.data(), 5);
			}
		}
		else 
		{
			if(g_pUIManager->IsShow(WINDOW_HELPER_LIST0))
				g_MainCharInfo.CloseFrame(WINDOW_HELPER_LIST0);

			g_pUIManager->Hide(HELP_BUTTON); 
		}

		//100갑자 초과시 제한
		if(g_MainCharInfo.m_wLevel<150)
		{
            g_MainCharInfo.m_i64LevelExp = g_MainCharInfo.m_i64NextLevelUpExp;
            g_MainCharInfo.m_i64NextLevelUpExp = i64NextLevelUpExp;
		}
		else
		{
			g_MainCharInfo.m_wLevel=150;
		}

		// 엔진에 있는 포인터가 클라이언트의 데이타를 가리키면 제대로 작동하지 않음.
		// 그래서 엔진의 포인터가 엔진의 데이타를 가리키도록 하면 됨.
		// 왜냐하면 엔진은 DLL이니깐.
		// effect
		CXiahCharObject* pCharObject = (CXiahCharObject*)g_pMainChar->m_pObject;
		_EFFECTPACKAGE* pPackage = g_EffectManager.EnqLevelUpEffectImmediately( LEVELUP_GAPJA );
		if( pPackage && pPackage->pEffectRender && pPackage->pEffectRender->pPackagePair )
		{
			pPackage->pEffectRender->pPackagePair->pWorldMatrix = (MATRIX*)pCharObject->m_CharRender.GetCharTM();
			//pPackage->pEffectRender->pPackagePair->WorldMatrix = (MATRIX)pCharObject->m_ObjectTM;

			// 이케해야 캐릭터가 없어질때 이펙트도 같이 없어진다.
			pCharObject->m_EffectPPList.push_back( pPackage->pEffectRender->pPackagePair );
		}

		// sound
		g_MainCharInfo.PlayInterfaceSound( ISOUND_LEVEL_UP );
	}

	if( bTpUp)
	{
		++g_MainCharInfo.m_wRemainTp;
		g_MainCharInfo.m_i64TpExp = g_MainCharInfo.m_i64NextTpUpExp;
		g_MainCharInfo.m_i64NextTpUpExp = i64NextTpUpExp;

		// effect
		CXiahCharObject* pCharObject = (CXiahCharObject*)g_pMainChar->m_pObject;
		_EFFECTPACKAGE* pPackage = g_EffectManager.EnqLevelUpEffectImmediately( LEVELUP_TP );
		if( pPackage && pPackage->pEffectRender && pPackage->pEffectRender->pPackagePair )
		{
			pPackage->pEffectRender->pPackagePair->pWorldMatrix = (MATRIX*)pCharObject->m_CharRender.GetCharTM();
			//pPackage->pEffectRender->pPackagePair->WorldMatrix = (MATRIX)pCharObject->m_ObjectTM;

			pCharObject->m_EffectPPList.push_back( pPackage->pEffectRender->pPackagePair );
		}

		// sound
		g_MainCharInfo.PlayInterfaceSound( ISOUND_TP_UP );
	}

	//if(bFiveElmLevelUp)
	{
		g_MainCharInfo.m_wFiveElmPoint		= wFiveElmPoint;
		g_MainCharInfo.m_dwFiveElmPower		= dwFiveElmPower;
		g_MainCharInfo.m_dwFiveElmPowerMax  = dwFiveElmPowerMax;
		g_MainCharInfo.m_dwFiveElmGauge		= dwFiveElmGauge;

		// TODO: 이펙트 추가 / 사운드 추가
	}

	g_MainCharInfo.RefreshChracterInfo();
	g_MainCharInfo.RefreshMainFrame();
	g_MainCharInfo.RefreshMugongFrame();
	g_MainCharInfo.RefreshFiveElement();	

	return TRUE;
}

int OnCS_IF_FAMEINFO_ACK(CMsg &msg)
{
	BYTE	bResult	=0;
	DWORD	dwFame	=0;

	msg
		>> bResult
		>> dwFame;

	g_MainCharInfo.m_dwFame = dwFame;

	g_MainCharInfo.RefreshChracterInfo();
	g_MainCharInfo.RefreshQuest();

	CXiahCharObject *pObject = (CXiahCharObject*)g_pMainChar->m_pObject;

	if(pObject)
		pObject->RefreshFameColor(g_MainCharInfo.m_dwFame);

	return TRUE;
}



///////////////////////////////// PARTY


/**
 * 단 수신 패킷
 * \param &msg 
 * \return 
 */
int OnCS_IF_ASKPARTY_ACK( CMsg &msg)
{
	// bResult	0 : 파티 승인됨
	//			4 : ??
	//			5 : 해당 캐릭터를 찾지 못함
	//			6 : 파티 생성 실패 ( 헬프메시지로 가지 않을까..)
	//			9 : 파티 거절됨
	DWORD dwAskID		=0;
	DWORD dwAskedID		=0;
	BYTE bResult		=0;
	BYTE bPartyType		=0;	// 0-일반단 1-관계단

	msg
		>> dwAskID
		>> dwAskedID
		>> bResult
		>> bPartyType;

	switch(bResult) 
	{
	case 0:
		if( dwAskID != g_MainCharInfo.m_dwObjectID)	
		{
			g_MainCharInfo.m_dwAskID = dwAskID;
			TCHAR szText[100] = {0,};

			if(bPartyType == 0)
			{
				// 일반 단
				_stprintf( szText, IDS_D_MAKE_DAN, (LPCTSTR)g_MainCharInfo.FindNameByID( dwAskID));

				if( !g_pUIManager->ShowNotice( szText, NOTICE_FRAME_OKCANCEL, NOTICE_FRAME_ASKPARTY))
				{
					// 거절
					SendCS_IF_ASKPARTY_REQ(g_MainCharInfo.m_dwAskID, g_MainCharInfo.m_dwObjectID, 9);
				}
			}
			else if(bPartyType == 1)
			{
				// 관계 단
				_stprintf(szText, IDS_D_MAKE_REL_DAN, (LPCTSTR)g_MainCharInfo.FindNameByID(dwAskID));

				if(!g_pUIManager->ShowNotice(szText, NOTICE_FRAME_OKCANCEL, NOTICE_FRAME_RELATION_ASKPARTY))
				{
					// 거절
					SendCS_IF_ASKPARTY_REQ(g_MainCharInfo.m_dwAskID, g_MainCharInfo.m_dwObjectID, 9, 1);
				}
			}
		}
		break;

	case 9:	// 단 신청 거절됨
		{
			g_MainCharInfo.m_dwAskID = 0;
			g_MainCharInfo.ShowHelpMessage( IDS_REJECT_DAN, TEXTEFFECT_COLOR_WARNING);
		}
		break;

	case 4:	// 갑자 차이로 단 취소됨
		if(dwAskID == g_MainCharInfo.m_dwObjectID)	
		{
			g_MainCharInfo.ShowHelpMessage( IDS_AUTO_CANCEL, TEXTEFFECT_COLOR_WARNING);
		}
		break;

	case 5:	// 자신은 단주가 아님
		{
			TCHAR strText[100] = {0,};

			_stprintf(strText, IDS_DAN_NOLEADER, (LPCTSTR)g_MainCharInfo.m_szNickName);
			g_MainCharInfo.ShowHelpMessage(strText, TEXTEFFECT_COLOR_WARNING);
		}
		break;

	case 7: // 단 인원 가득찼음
		{
			g_MainCharInfo.ShowHelpMessage(IDS_DAN_EXCEED, TEXTEFFECT_COLOR_WARNING);
		}
		break;

	case 8:	// 상대방이 이미 다른 단에 소속 되어 있음
		{
			g_MainCharInfo.m_dwAskID = 0;
			g_MainCharInfo.ShowHelpMessage(IDS_DAN_OTHER_DAN, TEXTEFFECT_COLOR_WARNING);
		}
		break;

	case 11:// 옵션 관계 거부 상태
		{
			g_MainCharInfo.m_dwAskID = 0;
			g_MainCharInfo.ShowHelpMessage(IDS_REJECT_DAN_2, TEXTEFFECT_COLOR_WARNING);
		}
		break;

	case 12: // 관계 단 맺을수 없을때
		{
			g_MainCharInfo.m_dwAskID = 0;
			g_MainCharInfo.ShowHelpMessage(IDS_DAN_RELATION_FAIL, TEXTEFFECT_COLOR_WARNING);
		}
		break;

	// ?
	//case 5:
	case 6:
		break;
	}

	return 0;
}

/**
 * 단 생성
 * \param &msg 
 * \return 
 */
int OnCS_IF_CREATEPARTY_ACK( CMsg &msg)
{
	DWORD dwPartyID		=0;
	DWORD dwLeaderID	=0;
	BYTE bPartyType		=0;

	msg
		>> dwPartyID
		>> dwLeaderID
		>> bPartyType;	// 0 일반단, 1 관계단

	g_MainCharInfo.m_pRelation->m_nDanType = static_cast<int>(bPartyType);

	// 방장이 될때 모든 처리를 해야함
	if( dwLeaderID == g_MainCharInfo.m_dwObjectID)
	{
		BYTE	byPriority = 1;
		BYTE	byLevel = (BYTE)g_MainCharInfo.m_wLevel;
		WORD	wPosX = 0;
		WORD	wPosY = 0;		

		if( g_pMainChar)
		{
			CXiahCharObject* pMainChar = (CXiahCharObject*)g_pMainChar->m_pObject;
			if(pMainChar)
				pMainChar->GetPosition( wPosX, wPosY);
		}

		// 단 경험치 분배
		g_pUIManager->Hide(WINDOW_DAN_NEW, window_dan_new_1button);
		g_pUIManager->Show(WINDOW_DAN_NEW, window_dan_new_2button_01);
		g_pUIManager->Show(WINDOW_DAN_NEW, window_dan_new_2button_02);

		g_MainCharInfo.m_pRelation->SetDanID( dwPartyID);
		g_MainCharInfo.m_pRelation->SetDanLeader( dwLeaderID);
		// 단 맵
		g_MainCharInfo.m_pRelation->InsertDan( g_MainCharInfo.m_dwObjectID, byPriority, g_MainCharInfo.m_szNickName,
										byLevel, wPosX, wPosY, g_MainCharInfo.m_dwHpCur, g_MainCharInfo.m_dwHpMax, 0);
	}
	
	g_MainCharInfo.ShowHelpMessage( IDS_MAKE_DAN);

	return 0;
}

/**
 * 단원 추가
 * \param &msg 
 * \return 
 */
int OnCS_IF_ENTERPARTY_ACK( CMsg &msg)
{
	DWORD	dwPartyID	=0;
	DWORD	dwCharID	=0;	
	WORD	wLevel		=0;
	WORD	wPosX		=0;
	WORD	wPosY		=0;
	DWORD	dwCurHp		=0;
	DWORD	dwMaxHp		=0;
	BYTE	bPartybPriority=0;
	sString szName;

	msg
		>> dwPartyID
		>> dwCharID
		>> bPartybPriority
		>> szName
		>> wLevel
		>> wPosX
		>> wPosY
		>> dwCurHp
		>> dwMaxHp;

	// 단 맵
	g_MainCharInfo.m_pRelation->InsertDan( dwCharID, bPartybPriority, szName, wLevel, wPosX, wPosY, dwCurHp, dwMaxHp, 0);

	TCHAR szText[100] = {0,};
	_stprintf( szText, IDS_D_JOIN_DAN, (LPCTSTR)szName);
	
	g_MainCharInfo.ShowHelpMessage( szText);

	return 0;
}

/**
 * 단 리스트
 * \param &msg 
 * \return 
 */
int OnCS_IF_PARTYLIST_ACK( CMsg &msg)
{
	DWORD dwPartyID	=0;
	BYTE bNumParty	=0;
	BYTE bPartyType	=0;

	msg
		>> dwPartyID
		>> bNumParty
		>> bPartyType;	// 0 일반단, 1 관계단

	if( !g_MainCharInfo.m_pRelation)
		return 0;

	g_MainCharInfo.m_pRelation->m_nDanType = static_cast<int>(bPartyType);

	g_MainCharInfo.m_dwAskID = 0;
	g_MainCharInfo.m_pRelation->SetDanID( dwPartyID);

	for(int i=0; i < bNumParty; ++i)
	{
		DWORD	dwCharID	=0;
		DWORD	dwMapID		=0;		
		WORD	wLevel		=0;
		WORD	wPosX		=0;
		WORD	wPosY		=0;
		DWORD	dwCurHp		=0;
		DWORD	dwMaxHp		=0;
		BYTE	bPartyPriority=0;
		sString szNickName;

		msg
			>> dwCharID
			>> bPartyPriority
			>> szNickName
			>> wLevel
			>> wPosX
			>> wPosY
			>> dwCurHp
			>> dwMaxHp
			>> dwMapID;

		if( bPartyPriority == 0)
			g_MainCharInfo.m_pRelation->SetDanLeader( dwCharID);

		g_MainCharInfo.m_pRelation->InsertDan( dwCharID, bPartyPriority, szNickName, wLevel, wPosX, wPosY, dwCurHp, dwMaxHp, dwMapID);
	}

	return 0;
}

int OnCS_IF_INVITEPARTY_ACK( CMsg &msg)
{
	// bResult	0 : 파티 승인됨
	//			4 : ??
	//			5 : 해당 캐릭터를 찾지 못함
	//			6 : 파티 생성 실패 ( 헬프메시지로 가지 않을까..)
	//			9 : 파티 거절됨
	DWORD dwAskID;
	DWORD dwAskedID;
	BYTE  bResult;

	msg
		>> dwAskID
		>> dwAskedID
		>> bResult;

	switch(bResult) 
	{
	case 0:
		if( dwAskedID == g_MainCharInfo.m_dwObjectID) 
		{
			g_MainCharInfo.m_dwAskID = dwAskID;

			TCHAR szText[100];

			_stprintf( szText, IDS_D_ASK_DAN, (LPCTSTR)g_MainCharInfo.FindNameByID( dwAskID));
			if( !g_pUIManager->ShowNotice( szText, NOTICE_FRAME_OKCANCEL, NOTICE_FRAME_INVITEPARTY))
			{
				// 거절
				SendCS_IF_INVITEPARTY_REQ( g_MainCharInfo.m_dwAskID, g_MainCharInfo.m_dwObjectID, 9);
			}
		}
		break;
	case 9:
		if( dwAskID == g_MainCharInfo.m_dwObjectID)
			g_MainCharInfo.ShowHelpMessage( IDS_REJECT_DAN, TEXTEFFECT_COLOR_WARNING);
		break;

	case 12: // 관계 단 맺을수 없을때
		{
			g_MainCharInfo.m_dwAskID = 0;
			g_MainCharInfo.ShowHelpMessage(IDS_DAN_RELATION_FAIL, TEXTEFFECT_COLOR_WARNING);
		}
		break;

	case 4:
	case 5:
	case 6:
		break;
	}


	return 0;
}

int OnCS_IF_LEAVEPARTY_ACK( CMsg &msg)
{
	DWORD dwPartyID	=0;
	DWORD dwCharID	=0;
	BYTE bProity	=0;

	msg
		>> dwPartyID
		>> dwCharID
		>> bProity;

	if( g_MainCharInfo.m_pRelation)
	{
		g_MainCharInfo.m_pRelation->DeleteDan( dwCharID);

		BOOL bValue = g_MainCharInfo.m_pRelation->Am_I_InDan();
		if( !bValue && g_pMainChar )
		{
			CXiahCharObject* pMainChar = (CXiahCharObject*)g_pMainChar->m_pObject;

			if(pMainChar)
			{
				pMainChar->m_dwPartyID = 0;
				pMainChar->m_dwPartyLeaderID = 0;
				pMainChar->m_dwEnemyPartyID = 0;
			}			
		}
	}

	return 0;
}

int OnCS_IF_BANISHPARTY_ACK( CMsg &msg)
{
	DWORD dwPartyID;
	DWORD dwCharID;
	BYTE bProity;

	msg
		>> dwPartyID
		>> dwCharID
		>> bProity;

	if( g_MainCharInfo.m_pRelation)
		g_MainCharInfo.m_pRelation->DeleteDan( dwCharID);

	if( g_MainCharInfo.m_dwObjectID == dwCharID)
		g_MainCharInfo.ShowHelpMessage( IDS_OUT_DAN, TEXTEFFECT_COLOR_WARNING);
	
	return 0;
}

int OnCS_IF_PARTYPOSITION_ACK( CMsg &msg)
{
	DWORD dwPartyID;
	DWORD dwCharID;
	WORD wLevel;
	DWORD dwHpCur;
	DWORD dwHpMax;
	DWORD dwMapID;
	WORD wPosX;
	WORD wPosY;

	msg
		>> dwPartyID
		>> dwCharID
		>> wLevel
		>> dwHpCur
		>> dwHpMax
		>> dwMapID
		>> wPosX
		>> wPosY;

	if( g_MainCharInfo.m_pRelation)
		g_MainCharInfo.m_pRelation->RefreshDanInfo( dwCharID, wLevel, dwHpCur, dwHpMax, dwMapID, wPosX, wPosY);

	return 0;
}

int OnCS_IF_CHANGEPARTYLEADER_ACK( CMsg &msg)
{
	//HO_0419_07 패키추가 : 패킷 추가전 코드
	//DWORD dwPartyID;
	//DWORD dwCharID;

	//msg
	//	>> dwPartyID
	//	>> dwCharID;

	//if( g_MainCharInfo.m_pRelation->GetDanID() == dwPartyID)
	//	g_MainCharInfo.m_pRelation->SetDanLeader( dwCharID);

	DWORD dwPartyID;
	DWORD dwCharID;
	BYTE  bResult;	////HO_0419_07 패킷추가 bResult추가 bResult 값에 따라 dwPartyID, dwCharID를 받고 안받고함

	msg		
		>> dwPartyID
		>> dwCharID
		>> bResult;
		
	switch(bResult) 
	{
	case ERR_CHANGEPARTYLEADER_SUCCESS:
		if( g_MainCharInfo.m_pRelation->GetDanID() == dwPartyID)
		{
			CloseAllWindow();
			//ProcessClickRelationButton( eDAN); //HO_0424_07 단프레임창이 다시 뜨는것을 막음
			g_MainCharInfo.m_pRelation->SetDanLeader( dwCharID);
		}
		break;
	case ERR_CHANGEPARTYLEADER_FAIL:
		break;
	default:
		break;
	}		

	return 0;
}

int OnCS_IF_DESTROYPARTY_ACK( CMsg &msg)
{
	DWORD dwPartyID;

	msg
		>> dwPartyID;

	g_MainCharInfo.m_pRelation->ClearDan();

	// 나의 단 정보는 내가
	CXiahCharObject* pMainChar = (CXiahCharObject*)g_pMainChar->m_pObject;

	pMainChar->m_dwPartyID = 0;
	pMainChar->m_dwPartyLeaderID = 0;
	pMainChar->m_dwEnemyPartyID = 0;
	
	return 0;
}















/////////////////////////////////////// Buddy

int OnCS_IF_ASKADDBUDDY_ACK( CMsg &msg)
{
	DWORD dwAskID;
	DWORD dwAskedID;
	BYTE bResult;

	// bResult	0 : 파티 승인됨
	//			4 : ??
	//			5 : 해당 캐릭터를 찾지 못함
	//			6 : 파티 생성 실패 ( 헬프메시지로 가지 않을까..)
	//          8 : 옵션 거부 상태
	//			9 : 파티 거절됨

	msg
		>> bResult
		>> dwAskID
		>> dwAskedID;		

	switch(bResult) 
	{
	case 0:
		if( dwAskID != g_MainCharInfo.m_dwObjectID)	
		{
			g_MainCharInfo.m_dwAskID = dwAskID;

			TCHAR szText[100];

			_stprintf( szText, IDS_D_ASK_FRIEND, (LPCTSTR)g_MainCharInfo.FindNameByID( dwAskID));
			if( !g_pUIManager->ShowNotice( szText, NOTICE_FRAME_OKCANCEL, NOTICE_FRAME_ASKBUDDY))
			{
				// 거절
				SendCS_IF_ASKADDBUDDY_REQ( g_MainCharInfo.m_dwAskID, g_MainCharInfo.m_dwObjectID, 9);
			}
		}
		break;
	case 8:
	case 9:
		g_MainCharInfo.m_dwAskID = 0;
		g_MainCharInfo.ShowHelpMessage( IDS_REJECT_FRIEND, TEXTEFFECT_COLOR_WARNING);
		break;
	case 4:
	case 5:
	case 6:
		break;
	}

	return 0;
}

int OnCS_IF_ADDBUDDY_ACK( CMsg &msg)
{
	// 안쓸꺼다.

	BYTE bResult;
	DWORD dwCharID;
	BYTE bType;
	sString szNickName;
	BYTE bWorldID;
	DWORD dwMapID;
	WORD wPosX;
	WORD wPosY;

	msg
		>> bResult
		>> dwCharID
		>> bType
		>> szNickName
		>> bWorldID
		>> dwMapID
		>> wPosX
		>> wPosY;

	if( g_MainCharInfo.m_pRelation && bResult == 0)
	{
		if(!g_MainCharInfo.m_pRelation->FindShipInfoByID(dwCharID))
		{
			g_MainCharInfo.m_pRelation->InsertShip( 0, dwCharID, szNickName, bWorldID, dwMapID, wPosX, wPosY, TRUE);

			TCHAR szText[100];
			_stprintf( szText, IDS_D_REGIST_FRIEND, (LPCTSTR)g_MainCharInfo.FindNameByID( dwCharID));
			g_MainCharInfo.ShowHelpMessage( szText);

			g_MainCharInfo.m_pRelation->RefreshShipContent();
		}

		// 전서구 리스트에 추가
		// bType은 csProtocol 참조
		if(bType == RELATION_TYPE_BUDDY)
			g_Mail.Add_SendList(dwCharID,szNickName,MAIL_BUDDY,bType);		// 친구들
		else	
			g_Mail.Add_SendList(dwCharID,szNickName,MAIL_RELATION,bType);	// 친구를 제외한 인연
	}

	return 0;
}

int OnCS_IF_DELBUDDY_ACK( CMsg &msg)
{
	BYTE bResult;
	DWORD dwBuddyID;

	msg
		>> bResult
		>> dwBuddyID;

	if( !bResult)
	{
		g_MainCharInfo.ShowHelpMessage(IDS_CUT_SHIP);
		g_MainCharInfo.m_pRelation->DeleteShip( dwBuddyID);

		// 전서구 리스트에서 제거
		g_Mail.Delete_SendMailList(dwBuddyID);
	}
	else
	{
		TCHAR str[50];
		_stprintf( str, IDS_ERROR, bResult);
		g_MainCharInfo.ShowHelpMessage(str);
	}

	return 0;
}

int OnCS_IF_BUDDYLIST_ACK( CMsg &msg)
{
	// 안쓴다.

	BYTE bNumBuddy;
	DWORD dwCharID;
	sString szNickName;
	BYTE bWorldID;
	DWORD dwMapID;
	BYTE bIsConnected;
	BYTE bType;

	msg 
		>> bNumBuddy;

	for( int i=0; i<bNumBuddy; i++)
	{
		msg
			>> bType
			>> dwCharID
			>> szNickName
			>> bWorldID
			>> dwMapID
			>> bIsConnected;

		if( g_MainCharInfo.m_pRelation)
		{
			if(!g_MainCharInfo.m_pRelation->FindShipInfoByID(dwCharID))
                g_MainCharInfo.m_pRelation->InsertShip( bType, dwCharID, szNickName, bWorldID, dwMapID, 0, 0, (BOOL)bIsConnected);
		}

		// 전서구
		// bType은 csProtocol 참조
		if(bType == RELATION_TYPE_BUDDY)
			g_Mail.Add_SendList(dwCharID,szNickName,MAIL_BUDDY,bType);		// 친구들
		else	
			g_Mail.Add_SendList(dwCharID,szNickName,MAIL_RELATION,bType);	// 친구를 제외한 인연
	}

	return 0;
}

int OnCS_IF_CHANGEBUDDYCONNECT_ACK( CMsg &msg)
{
	// 안쓴다.

	DWORD dwCharID;
	BYTE bWorldID;
	DWORD dwMapID;
	BYTE bIsConnected;

	msg
		>> dwCharID
		>> bWorldID
		>> dwMapID
		>> bIsConnected;

	if( g_MainCharInfo.m_pRelation)
		g_MainCharInfo.m_pRelation->ChangeBuddyConnect( dwCharID, bWorldID, dwMapID, (BOOL)bIsConnected);

	return 0;
}

int OnCS_IF_BUDDYPOSITION_ACK( CMsg &msg)
{
	sString szNickName;
	WORD wPosX;
	WORD wPosY;

	msg
		>> szNickName
		>> wPosX
		>> wPosY;

	return 0;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////
//------------- 애완동물
/**
 * 펫리스트
 * \param &msg 
 * \return 
 */
int OnCS_IF_PETLIST_ACK(CMsg &msg)
{
	DWORD dwCharID	=0;
	BYTE bNpcCnt	=0;

	msg
		>> dwCharID
		>> bNpcCnt;

	g_PetList.Release();

	for(int i=0; i < bNpcCnt; ++i) 
	{
		DWORD dwID				= 0;
		DWORD dwOwnID			= 0;
		DWORD dwMapID			= 0;
		BYTE  bNpcType			= 0;
		sString szName;
		WORD  wLevel			= 0;
		WORD  wPosX				= 0;
		WORD  wPosY				= 0;
		BYTE  bHeight			= 0;
		WORD  wDesPosX			= 0;
		WORD  wDesPosY			= 0;
		BYTE  bDesHeight		= 0;
		WORD  wDirection		= 0;
		DWORD  dwHpMax			= 0;
		DWORD  dwHpCur			= 0;
		WORD  wAtkPwr			= 0;
		WORD  wDefPwr			= 0;
		WORD  wAtkRating		= 0;
		WORD  wAvoidRatio		= 0;
		BYTE  bSpeed			= 0;
		WORD  wMeleeAtkRange	= 0;
		WORD  wShotAtkRange		= 0;
		BYTE  bAtkType			= 0;
		DWORD dwRefNpcID		= 0;
		BYTE  bCurJob			= 0;
		INT64 i64Exp			= 0;
		INT64 i64LevelExp		= 0;
		INT64 i64NextLevelUpExp	= 0;
		BYTE  bRevolutionStep	= 0;
		BYTE  bWildRate			= 0;
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

		// 만들어 주기
		XiahObject::CXiahObject *pPetObject = NULL;
		if((pPetObject = XiahObject::g_XiahObjectManager.FindXiahObject( MAKEOBJECTID( 0, dwID, OBJTYPE_PET))) != NULL)
		{	
			sArrayData* pData = XiahArrayIndex::g_NpcType.GetData( bNpcType);

			if( pData == NULL)
				continue;

			int nCharID			= pData->GetInt( 1);
			int nMeshType		= pData->GetInt( 2);
			int nTextureType	= pData->GetInt( 3);

			if(bRevolutionStep == 4 && bNpcType == 0) //HT_0621 : 영수둔갑신단 적용
			{
				// [10/31/2005] 캐릭터 ID변경
				nCharID = 1133;

				if(wVisualID[5])
				{
					nMeshType = 1;
					nTextureType = wVisualID[5] - 27700;
				}
			} // if(bRevolutionStep == 4)

			if( XiahGameEngine::GetCharacter( nCharID) == NULL)
				continue;
			
			CXiahCharObject *pObject = reinterpret_cast<CXiahCharObject*>(pPetObject->m_pObject);

			if(pObject && bRevolutionStep == 4 && bNpcType == 0) //HT_0621 : 영수둔갑신단 적용
				SetupPET_VisualEquipement(pObject, wVisualID);

			continue;
		}

		sArrayData* pData = XiahArrayIndex::g_NpcType.GetData( bNpcType);
		
		if( pData == NULL)
			continue;

		int nCharID			= pData->GetInt( 1);
		int nMeshType		= pData->GetInt( 2);
		int nTextureType	= pData->GetInt( 3);

		if(bRevolutionStep == 4 && bNpcType == 0) //HT_0621 : 영수둔갑신단 적용
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
			continue;

		//YS_0805 : BUG
		CXiahCharObject* pCharObject = (CXiahCharObject *)XiahObject::g_XiahPetPool.GetChar(); //new CXiahCharObject;

		if(NULL == pCharObject)
		{
			pCharObject = new CXiahCharObject;
		}

		pPetObject = XiahObject::g_XiahObjectManager.CreateXiahObject( dwID, OBJTYPE_PET, pCharObject);

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
		pPetInfo->m_dwIsHwan		= 0;


		pPetInfo->bAI = TRUE;
		pPetInfo->AI_Type = PETAI_AUTOATTACK;

		pCharObject->m_pPrivateData = (DWORD)pPetInfo;
		pCharObject->m_PrivateDataDestoryer = ReleasePetInfo;
		
		//YS_0812 : BUGFIX
		if ( pPetObject->m_pObject && pPetObject->m_pObject->m_bPoolClass )
		{
			pPetObject->m_pObject->m_bPetPool = true;
		}
		
		g_PetList.AddPet( pPetObject);

		for( int i=0; i < 4; ++i)
			SendCS_NC_PETSACKLIST_REQ( dwOwnID, dwID, i);

//		SendCS_NC_MAPENTER_REQ( pPetObject->m_dwServerID, XiahMap::g_XiahMap.m_MapInfo.m_dwMapID);
	}	

	g_PetList.JumpToPlayer();
	return 0;	
}

int OnCS_IF_CHARTPSP_ACK(CMsg &msg)
{
	BYTE bTp	=0;
	BYTE bSp	=0;

	msg
		>> bTp
		>> bSp;

	if( bTp)
	{
		g_MainCharInfo.m_wRemainTp += bTp;
		g_MainCharInfo.RefreshMugongFrame();
	}

	if( bSp)
	{
		g_MainCharInfo.m_wRemainSp += bSp;
		g_MainCharInfo.RefreshChracterInfo();		
	}

	return 0;
}

/**
 * 오행 수치 올리기
 * \param &msg 
 * \return 
 */
int OnCS_IF_EXECFIVEELM_ACK(CMsg &msg)
{
	BYTE bResult	=0;

	msg
		>> bResult;		

	switch(bResult)
	{
	case ERR_EXECFIVEELM_SUCCESS:
		{
			BYTE bFiveElm		=0;
			WORD wFiveElmExp	=0;
			WORD wFiveElmPoint  =0;

			msg
				>> bFiveElm
				>> wFiveElmExp
				>> wFiveElmPoint;

			g_MainCharInfo.m_wFiveElmExp[bFiveElm - 1]  = wFiveElmExp;
			g_MainCharInfo.m_wFiveElmPoint				= wFiveElmPoint;

			g_pUIManager->SetString(WINDOW_FIVEELEMENTS, fiveelements_window_exp_dummy_01+bFiveElm-1, wFiveElmExp);
			g_pUIManager->SetString(WINDOW_FIVEELEMENTS, fiveelements_window_exp_dummy, wFiveElmPoint);
		}
		break;
	case ERR_EXECFIVEELM_NEEDPOINT:	// 오행 수련치 부족
		{			
			g_MainCharInfo.ShowHelpMessage(IDS_EXECFIVEELM_NEEDPOINT, TEXTEFFECT_COLOR_WARNING);
		}
		break;
	case ERR_EXECFIVEELM_NOTFINDCHAR:
		{

		}
		break;
	case 3:	// 모두 익혔음
		{
			g_MainCharInfo.ShowHelpMessage(IDS_ALL_TRAINED_EF, TEXTEFFECT_COLOR_WARNING);
		}
		break;
	case 255:
		{

		}
		break;
	default:
		break;
	}

	return 0;
}

/**
 * 오행 선택
 * \param &msg 
 * \return 
 */
int OnCS_IF_CHANGEFIVEELM_ACK(CMsg &msg)
{
	BYTE bResult	=0;

	msg
		>> bResult;

	switch(bResult)
	{
	case 0:				//HT_0720 : 오행 개선 사항
		{
			BYTE bCurFiveElm = 0;
			BYTE bChangeFiveElm = 0;

			msg
				>> bCurFiveElm
				>> bChangeFiveElm;

			for(int i=0; i < 5; ++i)
				g_pUIManager->SetData(WINDOW_FIVEELEMENTS, fiveelements_window_button_fire+i, CURRENT_INDEX, -1);

			if(bCurFiveElm)
			{
				g_pUIManager->SetData(WINDOW_FIVEELEMENTS, fiveelements_window_button_fire+bCurFiveElm-1, CURRENT_INDEX, 2);
			}

			if(bChangeFiveElm)
			{
				g_pUIManager->SetData(MAIN_FRAME, main_frame_ok, TEXTURE, 1536);
			}
			else
			{
				g_pUIManager->SetData(MAIN_FRAME, main_frame_ok, TEXTURE, 1535);
			}
		}
		break;
	case 1:	// 오행 사용 중 다른 오행 선택 시 변경 불가
		{
			g_MainCharInfo.ShowHelpMessage(IDS_CHANGEFIVEELM_ONAIR, TEXTEFFECT_COLOR_WARNING);		
		}
		break;
	case 2:	// 능력 부족으로 사용할수 없음
		{
			g_MainCharInfo.ShowHelpMessage(IDS_CHANGEFIVEELM_NOTYET, TEXTEFFECT_COLOR_WARNING);	
		}
		break;
	
	case 3: //HT_0720 : 오행 개선 사항 (사용 중인 오행이 없음)
		{
			g_MainCharInfo.ShowHelpMessage(IDS_CHANGEFIVEELM_NONE, TEXTEFFECT_COLOR_WARNING);
		}
		break;
	
	case 4:	//HT_0720 : 오행 개선 사항 (30초가 안 지나서 오행을 바꿀 수 없음)
		{
			g_MainCharInfo.ShowHelpMessage(IDS_CHANGEFIVEELM_SHORTTIME, TEXTEFFECT_COLOR_WARNING);
		}
		break;

	default:
		break;
	}

	return 0;
}

/**
 * 단 경험치 분배 상태 값
 * \param &msg 
 * \return 
 */
int OnCS_IF_PARTYSHARE_ACK(CMsg &msg)
{
	//BYTE byExpDivision	=0;		// 경험치 분배 0-개인 분배, 1-공동 분배
	//BYTE byFEDivision	=0;		// 오행 값 분배 0-개인 분배, 1-공동 분배

	if(g_MainCharInfo.m_pRelation)
	{
		msg
			>> g_MainCharInfo.m_pRelation->m_byExpDivision
			>> g_MainCharInfo.m_pRelation->m_byFEDivision;

		// TODO: 변경시 문구
		// 창 열려 있을시 분배 상태 변경
		if(g_pUIManager->IsShow(WINDOW_DAN_NEW))
		{
			if(g_MainCharInfo.m_pRelation->m_byExpDivision == 1)
			{
				g_pUIManager->SetData(WINDOW_DAN_NEW, window_dan_new_select_button1, STATICLIST_SHOW, 0);
				g_pUIManager->SetData(WINDOW_DAN_NEW, window_dan_new_select_button2, STATICLIST_SHOW, 1);
			}
			else
			{
				g_pUIManager->SetData(WINDOW_DAN_NEW, window_dan_new_select_button1, STATICLIST_SHOW, 1);
				g_pUIManager->SetData(WINDOW_DAN_NEW, window_dan_new_select_button2, STATICLIST_SHOW, 0);
			}

			if(g_MainCharInfo.m_pRelation->m_byFEDivision == 1)
			{
				g_pUIManager->SetData(WINDOW_DAN_NEW, window_dan_new_select_button3, STATICLIST_SHOW, 0);
				g_pUIManager->SetData(WINDOW_DAN_NEW, window_dan_new_select_button4, STATICLIST_SHOW, 1);
			}
			else
			{
				g_pUIManager->SetData(WINDOW_DAN_NEW, window_dan_new_select_button3, STATICLIST_SHOW, 1);
				g_pUIManager->SetData(WINDOW_DAN_NEW, window_dan_new_select_button4, STATICLIST_SHOW, 0);
			}
		}
	}

	return 0;
}


/**
* 기
* \param &msg 
* \return 
*/
int OnCS_IF_STAMINA_ACK(CMsg &msg)
{
	BYTE bStaminaCnt =0;

	msg
		>> bStaminaCnt;

	g_MainCharInfo.m_bStaminaCnt = bStaminaCnt;

	if(bStaminaCnt >= 5)
	{
		g_pUIManager->Show(SPIRIT);

		g_MainCharInfo.m_bSpirit = true;

		//HT_CHEAT : 기 자동
		if(g_bCheat)
			SendCS_IF_EXECSTAMINA_REQ();
			
	}
	else
	{
		g_pUIManager->Hide(SPIRIT);
	}

	for(int i=0; i < 5; ++i)
	{
		if(bStaminaCnt > i)
		{
			g_pUIManager->Show(MAIN_FRAME, main_frame_1_gauge_01 + i);
		}
		else
		{
			g_pUIManager->Hide(MAIN_FRAME, main_frame_1_gauge_01 + i);
		}
	}

	return 0;
}

/**
* 기 발동
* \param &msg 
* \return 
*/
int OnCS_IF_EXECSTAMINA_ACK(CMsg &msg)
{
	BYTE bResult = 0;

	msg
		>> bResult;

	switch(bResult)
	{
	case 0:
		{
			g_MainCharInfo.m_bSpirit = false;

			g_Spirit.Start(12000, 5);

			g_pUIManager->Hide(SPIRIT);
		}
		break;
	case 1:
		{
			// TODO: 실패 문구
		}
		break;
	default:
		break;
	}

	return 0;
}