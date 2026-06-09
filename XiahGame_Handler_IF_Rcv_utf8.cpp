癤#include "csprotocol.h"
#include "mail.h"

#include ".\spirit.h"
#include "RebirthMark.h"

//HT_CHEAT : 뫗繇 솏
extern BOOL g_bCheat;

//HT_CHEAT : 髥딀궪 邀꾣쫩
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
			case 7: //腰믢쑴 뀙뜛熬띸뵺 돚븕 쐣넑 囹띷뎮 
				//if(pItemInfo->m_bRarity < 3 && pItemInfo->m_bStxType < 3 && pItemInfo->m_bModifyCnt < 1)
				if(pItemInfo->m_bModifyCnt < 1)
				{
					g_MainCharInfo.m_bAutoSell = true;
					g_MainCharInfo.m_bySellPos = i;
					return;
				}
				break;
			case 9: //穩ょ돚 囹쒐삤瀯 瓦ョ즸 
			//	if(pItemInfo->m_bItemKind != 5)
			//	{
					g_MainCharInfo.m_bAutoSell = true;
					g_MainCharInfo.m_bySellPos = i;
					return;
			//	}
			case 15: //簾끾돬
				g_MainCharInfo.m_bAutoSell = true;
				g_MainCharInfo.m_bySellPos = i;
				return;
			case 16://꼸 냸뜛 
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
							else if(pItemInfo->m_wVisualID != 9102) //꼸삊 솏恙ョ퍓 阿먧퓘릟...꼸 끁燁藥 訝닸뜛瀯 餓룬씊뜛瀯 瓦ョ삤 野뚨즸. 
							{
								g_MainCharInfo.m_bAutoSell = true;
								g_MainCharInfo.m_bySellPos = i;
								return;
							}
						}
					}
				}
				break;	
			case 18: // 돬 뀙뜛熬
				//if(	pItemInfo->m_wRefID != 20963 &&
				//	pItemInfo->m_wRefID != 20965 &&
				//	pItemInfo->m_wRefID != 20966)
				//{
					g_MainCharInfo.m_bAutoSell = true;
					g_MainCharInfo.m_bySellPos = i;
					return;
			//	}
			case 19://汝긴퓘똾 藥당뵺(뮚쐣 뀙熬띸뵺 ....)
				if(pItemInfo->m_wVisualID == 31000) //ζ떛 깙髥딁뺐 냸젎릟 恙ュ뒱뀗
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
			case 20: //뜍寧 뀙뜛熬띸뵺
			//	if(pItemInfo->m_wVisualID != 29200 && pItemInfo->m_wVisualID != 29201 )
			//	{
					g_MainCharInfo.m_bAutoSell = true;
					g_MainCharInfo.m_bySellPos = i;
					return;
			//	}	break;
			case 21: //麗
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
			case 23: //瓮
				//if( pItemInfo->m_wVisualID == 20000 ||
				//	pItemInfo->m_wVisualID == 20100 ||
				//	pItemInfo->m_wVisualID == 21000 ||
				//	pItemInfo->m_wVisualID == 21100)
				//{
					//if(pItemInfo->m_wRefID != 20073 && pItemInfo->m_wRefID  != 20070 ) //렕轢뤹 暎 뀙譯ⓨ. 걵
					//if(pItemInfo->m_wRefID == 20073 || pItemInfo->m_wRefID  == 20070 ) //렕轢뤹 暎 뀙譯ⓨ. 걵
					//{
						g_MainCharInfo.m_bAutoSell = true;
						g_MainCharInfo.m_bySellPos = i;
						return;
					//}
				//}
			case 25: //믥タ뜛
				if( pItemInfo->m_wRefID == 20189 || pItemInfo->m_wRefID == 20267 )
				{
					g_MainCharInfo.m_bAutoSell = true;
					g_MainCharInfo.m_bySellPos = i;
					return;
				}
				break;
			case 33: //岳븀뿢룴
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
			case 7: //腰믢쑴 뀙뜛熬띸뵺 돚븕 쐣넑 囹띷뎮 
				//if(pItemInfo->m_bRarity < 3 && pItemInfo->m_bStxType < 3 && pItemInfo->m_bModifyCnt < 1)
				if(pItemInfo->m_bModifyCnt < 1)
				{
					g_MainCharInfo.m_bAutoSell = true;
					g_MainCharInfo.m_bySellPos = i;
					return;
				}
				break;
			case 9: //穩ょ돚 囹쒐삤瀯 瓦ョ즸 
			//	if(pItemInfo->m_bItemKind == 0)
			//	{
					g_MainCharInfo.m_bAutoSell = true;
					g_MainCharInfo.m_bySellPos = i;
					return;
			//	}
			case 15: //簾끾돬
				g_MainCharInfo.m_bAutoSell = true;
				g_MainCharInfo.m_bySellPos = i;
				return;
			case 16://꼸 냸뜛 
				if(pItemInfo->m_bItemKind >= 0 && pItemInfo->m_bItemKind < 4)// && pItemInfo->m_wVisualID != 9102)
				{
					if(g_PetList.size() == 0)// && pItemInfo->m_wVisualID != 9102) //꼸 솏恙ョ뺐 맃뜛 뀙겒岳.
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
							else if(pItemInfo->m_wVisualID != 9102) //꼸삊 솏恙ョ퍓 阿먧퓘릟...꼸 끁燁藥 訝닸뜛瀯 웵影 瓦ョ삤 野뚨즸. 
							{
								g_MainCharInfo.m_bAutoSell = true;
								g_MainCharInfo.m_bySellPos = i;
								return;
							}
						}
					}
				}
				break;
			case 18: // 돬 뀙뜛熬
				//if(	pItemInfo->m_wRefID != 20963 &&
				//	pItemInfo->m_wRefID != 20965 &&
				//	pItemInfo->m_wRefID != 20966)
				//{
					g_MainCharInfo.m_bAutoSell = true;
					g_MainCharInfo.m_bySellPos = i;
					return;
			//	}
			case 19://汝긴퓘똾 藥당뵺(뮚쐣 뀙熬띸뵺 ....)
				if(pItemInfo->m_wVisualID == 31000) //ζ떛 깙髥딁뺐 냸젎릟 恙ュ뒱뀗
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
			case 20: //뜍寧 뀙뜛熬띸뵺
			//	if(pItemInfo->m_wVisualID != 29200 && pItemInfo->m_wVisualID != 29201 )
			//	{
					g_MainCharInfo.m_bAutoSell = true;
					g_MainCharInfo.m_bySellPos = i;
					return;
			//	}	break;
			case 21: //麗
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
			case 23: //瓮
				//if( pItemInfo->m_wVisualID == 20000 ||
				//	pItemInfo->m_wVisualID == 20100 ||
				//	pItemInfo->m_wVisualID == 21000 ||
				//	pItemInfo->m_wVisualID == 21100)
				//{
					//if(pItemInfo->m_wRefID != 20073 && pItemInfo->m_wRefID  != 20070 ) //렕轢뤹 暎 뀙譯ⓨ. 걵
					//if(pItemInfo->m_wRefID == 20073 || pItemInfo->m_wRefID  == 20070 ) //렕轢뤹 暎 뀙譯ⓨ. 걵
					//{
						g_MainCharInfo.m_bAutoSell = true;
						g_MainCharInfo.m_bySellPos = i;
						return;
					//}
				//}
			case 25: //믥タ뜛
				if( pItemInfo->m_wRefID == 20189 || pItemInfo->m_wRefID == 20267 )
				{
					g_MainCharInfo.m_bAutoSell = true;
					g_MainCharInfo.m_bySellPos = i;
					return;
				}
				break;
			case 33: //岳븀뿢룴
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
	sString szNickName;//HO_0313_07 젩뼑굠 瑥뜒뜛繇섋궨 돚똾 쑀븡
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

			//HT_CHEAT : 髥딀궪 邀꾣쫩
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

	// PARTY BATTLE岳딁읈윬 뜠 댏옲岳 똿똾 쉵삤
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
	case 5:	// 럹腰
		{
			g_MainCharInfo.ShowHelpMessage(IDS_PET_ITEM_1);			
		}
		break;
	case 6:	// 
		{
			g_MainCharInfo.ShowHelpMessage(IDS_PET_ITEM_2);
		}
		break;
	
	case HM_BLOODDEVIL://HO_0313_07 젩뼑굠 瑥뜒뜛繇섋궨 돚똾 쑀븡
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
	// CG_2005/01/28 : 눦渦녽뀙뜛熬띷뎮벜쑀븡
	// 눦渦녽뀙뜛熬띹곲뵯 쐣넑
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
		>> g_MainCharInfo.m_dwPremiumTP //HT_0911 : 艅뉐틵쎓넶 易낁꺐繇 뜍溫욘뫗
		>> g_MainCharInfo.m_dwPremiumSP
		>> g_MainCharInfo.m_dwGameMasterMark; //HT_1023 : 轝얍볜髥 餓섇넑 쑀븡
	
	g_MainCharInfo.RefreshChracterInfo();
	g_MainCharInfo.RefreshMainFrame();
	g_MainCharInfo.RefreshMugongFrame();

	// 썟꽊떆뜛릟 若섇콆쐳岳.
	sPetInfo *pPet = g_PetList.GetBunsinPet();
	if(pPet != NULL)
	{
		pPet->bSpeed = g_MainCharInfo.m_bWalkSpeed;
	}

	// useitem_ack븡 瀯앯뺐 똿땶궨..
	for( int i=0; i < 5; ++i)
		g_MainCharInfo.m_pSlot->SetSlotToolTip(i);

	// CG_2005/01/28 : 눦渦녽뀙뜛熬띷뎮벜쑀븡
	CXiahCharObject* pObject = (CXiahCharObject*)g_pMainChar->m_pObject;
	if( pObject )
	{
		pObject->m_bChangeItemSet = bChangeItemSet;
		pObject->RefreshFameColor(g_MainCharInfo.m_dwFame);
		pObject->m_bRebirth = g_MainCharInfo.m_bRebirth;
		pObject->m_bGameMasterMark = g_MainCharInfo.m_dwGameMasterMark; //HT_1023 : 轝얍볜髥 餓섇넑 쑀븡
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
	case 0:		// 髥딀궪 黎
		break;
		// 떛瓮 뜡堊
	case 1:		// HP
		{
			_EFFECTPACKAGE* pEffectPackage = g_EffectManager.EnqAppearEffectImmediately( eMulYak_HP );

			if( pEffectPackage && pEffectPackage->pEffectRender && pEffectPackage->pEffectRender->pPackagePair )
			{
				pEffectPackage->pEffectRender->pPackagePair->pWorldMatrix = (MATRIX*)pCharObject->m_CharRender.GetCharTM();
//				pEffectPackage->pEffectRender->pPackagePair->WorldMatrix = (MATRIX)pCharObject->m_ObjectTM;

				// 뜛瀛녕㏄끁 윇뀗髥먨븡 瀯앯빪榕숅뵯 뜛汝뗩짌旅 엻뜛 瀯앯빪윹岳.
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
		// 뀶굧
	case 11:	// 굠쑁驛 - 勇η돚 뫗訝.
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
	case 12:	// 뜛젩驛 - 囹 뫗訝.
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
 * 뎵麗경뫗
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

		//100솈髥 茹ц떈윬 뒟똾
		if(g_MainCharInfo.m_wLevel < 100)
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

		//HO_0413_07 떈 븡뜛씃 瑥뜒뜛繇 : 궕 쐣넑耀 譯싩뱼 삤旅
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

		//100솈髥 茹ц떈윬 뒟똾
		if(g_MainCharInfo.m_wLevel<100)
		{
            g_MainCharInfo.m_i64LevelExp = g_MainCharInfo.m_i64NextLevelUpExp;
            g_MainCharInfo.m_i64NextLevelUpExp = i64NextLevelUpExp;
		}
		else
		{
			g_MainCharInfo.m_wLevel=100;
		}

		// 役싨윹岳 阿먪뺐 솳돚髥먨븡 뒱돹뜛뵭繇섊떬 뜒뜛勇η뵭 븡佯쒑솏릟 뒟렕궨 榮궪囹띸삤 눥驛.
		// 凉딂눗渦 役싨윹떬 솳돚髥먨븡 役싨윹떬 뜒뜛勇η뵭 븡佯쒑솏旅ｅ틹 囹띷릟 눛.
		// 걧烏х첀릟 役싨윹影 DLL뜛겒삫.
		// effect
		CXiahCharObject* pCharObject = (CXiahCharObject*)g_pMainChar->m_pObject;
		_EFFECTPACKAGE* pPackage = g_EffectManager.EnqLevelUpEffectImmediately( LEVELUP_GAPJA );
		if( pPackage && pPackage->pEffectRender && pPackage->pEffectRender->pPackagePair )
		{
			pPackage->pEffectRender->pPackagePair->pWorldMatrix = (MATRIX*)pCharObject->m_CharRender.GetCharTM();
			//pPackage->pEffectRender->pPackagePair->WorldMatrix = (MATRIX)pCharObject->m_ObjectTM;

			// 뜛瀛녕㏄끁 윇뀗髥먨븡 瀯앯빪榕숅뵯 뜛汝뗩짌旅 엻뜛 瀯앯빪윹岳.
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

		// TODO: 뜛汝뗩짌 쑀븡 / 뜡轝얗씃 쑀븡
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
 * 囹 뜍꽊 룴벀
 * \param &msg 
 * \return 
 */
int OnCS_IF_ASKPARTY_ACK( CMsg &msg)
{
	// bResult	0 : 窯뉓릫 벐돚눛
	//			4 : ??
	//			5 : 燁野 윇뀗髥먪뵭 뙧삤 瀯숂챶
	//			6 : 窯뉓릫 燁藥 鰲믦룴 ( 燁뗦찃쉵윬삤궨 븡삤 눥삊褥..)
	//			9 : 窯뉓릫 뒴堊뗥눛
	DWORD dwAskID		=0;
	DWORD dwAskedID		=0;
	BYTE bResult		=0;
	BYTE bPartyType		=0;	// 0-곲쫮囹 1-똿땶囹

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
				// 곲쫮 囹
				_stprintf( szText, IDS_D_MAKE_DAN, (LPCTSTR)g_MainCharInfo.FindNameByID( dwAskID));

				if( !g_pUIManager->ShowNotice( szText, NOTICE_FRAME_OKCANCEL, NOTICE_FRAME_ASKPARTY))
				{
					// 뒴堊
					SendCS_IF_ASKPARTY_REQ(g_MainCharInfo.m_dwAskID, g_MainCharInfo.m_dwObjectID, 9);
				}
			}
			else if(bPartyType == 1)
			{
				// 똿땶 囹
				_stprintf(szText, IDS_D_MAKE_REL_DAN, (LPCTSTR)g_MainCharInfo.FindNameByID(dwAskID));

				if(!g_pUIManager->ShowNotice(szText, NOTICE_FRAME_OKCANCEL, NOTICE_FRAME_RELATION_ASKPARTY))
				{
					// 뒴堊
					SendCS_IF_ASKPARTY_REQ(g_MainCharInfo.m_dwAskID, g_MainCharInfo.m_dwObjectID, 9, 1);
				}
			}
		}
		break;

	case 9:	// 囹 꽊亦 뒴堊뗥눛
		{
			g_MainCharInfo.m_dwAskID = 0;
			g_MainCharInfo.ShowHelpMessage( IDS_REJECT_DAN, TEXTEFFECT_COLOR_WARNING);
		}
		break;

	case 4:	// 솈髥 옊뜛궨 囹 燁믣뜹눛
		if(dwAskID == g_MainCharInfo.m_dwObjectID)	
		{
			g_MainCharInfo.ShowHelpMessage( IDS_AUTO_CANCEL, TEXTEFFECT_COLOR_WARNING);
		}
		break;

	case 5:	// 髥딂꽊影 囹쒏옑븡 뀙訝
		{
			TCHAR strText[100] = {0,};

			_stprintf(strText, IDS_DAN_NOLEADER, (LPCTSTR)g_MainCharInfo.m_szNickName);
			g_MainCharInfo.ShowHelpMessage(strText, TEXTEFFECT_COLOR_WARNING);
		}
		break;

	case 7: // 囹 돚썡 븡옯瘟⒵풙
		{
			g_MainCharInfo.ShowHelpMessage(IDS_DAN_EXCEED, TEXTEFFECT_COLOR_WARNING);
		}
		break;

	case 8:	// 깙렕鰲꾣뜛 뜛쎓 岳껃폌 囹쒍퓡 若뜹뒥 쇉瀯 阿먩풙
		{
			g_MainCharInfo.m_dwAskID = 0;
			g_MainCharInfo.ShowHelpMessage(IDS_DAN_OTHER_DAN, TEXTEFFECT_COLOR_WARNING);
		}
		break;

	case 11:// 룾溫 똿땶 뒴鵝 깙
		{
			g_MainCharInfo.m_dwAskID = 0;
			g_MainCharInfo.ShowHelpMessage(IDS_REJECT_DAN_2, TEXTEFFECT_COLOR_WARNING);
		}
		break;

	case 12: // 똿땶 囹 궄삊뜍 瀯앶삊뵯
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
 * 囹 燁藥
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
		>> bPartyType;	// 0 곲쫮囹, 1 똿땶囹

	g_MainCharInfo.m_pRelation->m_nDanType = static_cast<int>(bPartyType);

	// 鰲꾢럹뜛 옫뵯 몳뵷 兀멨틵뵭 燁끁囹
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

		// 囹 뎵麗경뫗 썟簾
		g_pUIManager->Hide(WINDOW_DAN_NEW, window_dan_new_1button);
		g_pUIManager->Show(WINDOW_DAN_NEW, window_dan_new_2button_01);
		g_pUIManager->Show(WINDOW_DAN_NEW, window_dan_new_2button_02);

		g_MainCharInfo.m_pRelation->SetDanID( dwPartyID);
		g_MainCharInfo.m_pRelation->SetDanLeader( dwLeaderID);
		// 囹 뵖
		g_MainCharInfo.m_pRelation->InsertDan( g_MainCharInfo.m_dwObjectID, byPriority, g_MainCharInfo.m_szNickName,
										byLevel, wPosX, wPosY, g_MainCharInfo.m_dwHpCur, g_MainCharInfo.m_dwHpMax, 0);
	}
	
	g_MainCharInfo.ShowHelpMessage( IDS_MAKE_DAN);

	return 0;
}

/**
 * 囹쒐썡 쑀븡
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

	// 囹 뵖
	g_MainCharInfo.m_pRelation->InsertDan( dwCharID, bPartybPriority, szName, wLevel, wPosX, wPosY, dwCurHp, dwMaxHp, 0);

	TCHAR szText[100] = {0,};
	_stprintf( szText, IDS_D_JOIN_DAN, (LPCTSTR)szName);
	
	g_MainCharInfo.ShowHelpMessage( szText);

	return 0;
}

/**
 * 囹 佯쒑꺐繇
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
		>> bPartyType;	// 0 곲쫮囹, 1 똿땶囹

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
	// bResult	0 : 窯뉓릫 벐돚눛
	//			4 : ??
	//			5 : 燁野 윇뀗髥먪뵭 뙧삤 瀯숂챶
	//			6 : 窯뉓릫 燁藥 鰲믦룴 ( 燁뗦찃쉵윬삤궨 븡삤 눥삊褥..)
	//			9 : 窯뉓릫 뒴堊뗥눛
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
				// 뒴堊
				SendCS_IF_INVITEPARTY_REQ( g_MainCharInfo.m_dwAskID, g_MainCharInfo.m_dwObjectID, 9);
			}
		}
		break;
	case 9:
		if( dwAskID == g_MainCharInfo.m_dwObjectID)
			g_MainCharInfo.ShowHelpMessage( IDS_REJECT_DAN, TEXTEFFECT_COLOR_WARNING);
		break;

	case 12: // 똿땶 囹 궄삊뜍 瀯앶삊뵯
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
	//HO_0419_07 룴솏쑀븡 : 룴벀 쑀븡굠 냵씃
	//DWORD dwPartyID;
	//DWORD dwCharID;

	//msg
	//	>> dwPartyID
	//	>> dwCharID;

	//if( g_MainCharInfo.m_pRelation->GetDanID() == dwPartyID)
	//	g_MainCharInfo.m_pRelation->SetDanLeader( dwCharID);

	DWORD dwPartyID;
	DWORD dwCharID;
	BYTE  bResult;	////HO_0419_07 룴벀쑀븡 bResult쑀븡 bResult 뵾岳 씢돹 dwPartyID, dwCharID뵭 營먪퍓 븨營먪퍓囹

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
			//ProcessClickRelationButton( eDAN); //HO_0424_07 囹쒏찃耀깧뒕뜛 岳껆윬 뱠瀯겼럽삊 삙驛
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

	// 뵳떬 囹 亦η꼯瀯 꺎븡
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

	// bResult	0 : 窯뉓릫 벐돚눛
	//			4 : ??
	//			5 : 燁野 윇뀗髥먪뵭 뙧삤 瀯숂챶
	//			6 : 窯뉓릫 燁藥 鰲믦룴 ( 燁뗦찃쉵윬삤궨 븡삤 눥삊褥..)
	//          8 : 룾溫 뒴鵝 깙
	//			9 : 窯뉓릫 뒴堊뗥눛

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
				// 뒴堊
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
	// 븨븳力㏘퓘.

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

		// 굠渦묈쨭 佯쒑꺐繇섆퓡 쑀븡
		// bType影 csProtocol 쎕궪
		if(bType == RELATION_TYPE_BUDDY)
			g_Mail.Add_SendList(dwCharID,szNickName,MAIL_BUDDY,bType);		// 與▼쨭뵺
		else	
			g_Mail.Add_SendList(dwCharID,szNickName,MAIL_RELATION,bType);	// 與▼쨭뵭 뒟野뉓똾 돚璵
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

		// 굠渦묈쨭 佯쒑꺐繇섆퓡渦 뒟뒴
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
	// 븨빃岳.

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

		// 굠渦묈쨭
		// bType影 csProtocol 쎕궪
		if(bType == RELATION_TYPE_BUDDY)
			g_Mail.Add_SendList(dwCharID,szNickName,MAIL_BUDDY,bType);		// 與▼쨭뵺
		else	
			g_Mail.Add_SendList(dwCharID,szNickName,MAIL_RELATION,bType);	// 與▼쨭뵭 뒟野뉓똾 돚璵
	}

	return 0;
}

int OnCS_IF_CHANGEBUDDYCONNECT_ACK( CMsg &msg)
{
	// 븨빃岳.

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
//------------- 掠궚궪떛
/**
 * 꼸佯쒑꺐繇
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

		// 댍뵺瀯 옑뎮
		XiahObject::CXiahObject *pPetObject = NULL;
		if((pPetObject = XiahObject::g_XiahObjectManager.FindXiahObject( MAKEOBJECTID( 0, dwID, OBJTYPE_PET))) != NULL)
		{	
			sArrayData* pData = XiahArrayIndex::g_NpcType.GetData( bNpcType);

			if( pData == NULL)
				continue;

			int nCharID			= pData->GetInt( 1);
			int nMeshType		= pData->GetInt( 2);
			int nTextureType	= pData->GetInt( 3);

			if(bRevolutionStep == 4 && bNpcType == 0) //HT_0621 : 佯룩뜍븣솈꽊囹 닶堊
			{
				// [10/31/2005] 윇뀗髥 ID눦뎵
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

			if(pObject && bRevolutionStep == 4 && bNpcType == 0) //HT_0621 : 佯룩뜍븣솈꽊囹 닶堊
				SetupPET_VisualEquipement(pObject, wVisualID);

			continue;
		}

		sArrayData* pData = XiahArrayIndex::g_NpcType.GetData( bNpcType);
		
		if( pData == NULL)
			continue;

		int nCharID			= pData->GetInt( 1);
		int nMeshType		= pData->GetInt( 2);
		int nTextureType	= pData->GetInt( 3);

		if(bRevolutionStep == 4 && bNpcType == 0) //HT_0621 : 佯룩뜍븣솈꽊囹 닶堊
		{
			// [10/31/2005] 윇뀗髥 ID눦뎵
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

		if(bRevolutionStep == 4 && bNpcType == 0) //HT_0621 : 佯룩뜍븣솈꽊囹 닶堊
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
 * 씣쓵 뜍뫗 汝드틵뎮
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
	case ERR_EXECFIVEELM_NEEDPOINT:	// 씣쓵 뜍溫욘뫗 鵝뺟퍌
		{			
			g_MainCharInfo.ShowHelpMessage(IDS_EXECFIVEELM_NEEDPOINT, TEXTEFFECT_COLOR_WARNING);
		}
		break;
	case ERR_EXECFIVEELM_NOTFINDCHAR:
		{

		}
		break;
	case 3:	// 몳譯 뒽翁驛
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
 * 씣쓵 η맯
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
	case 0:				//HT_0720 : 씣쓵 岳뷸 뜡雅
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
	case 1:	// 씣쓵 뜡堊 맃 岳껃폌 씣쓵 η맯 윬 눦뎵 쁻븡
		{
			g_MainCharInfo.ShowHelpMessage(IDS_CHANGEFIVEELM_ONAIR, TEXTEFFECT_COLOR_WARNING);		
		}
		break;
	case 2:	// 벜餓 鵝뺟퍌젎궨 뜡堊⒳툝뜍 瀯앮풙
		{
			g_MainCharInfo.ShowHelpMessage(IDS_CHANGEFIVEELM_NOTYET, TEXTEFFECT_COLOR_WARNING);	
		}
		break;
	
	case 3: //HT_0720 : 씣쓵 岳뷸 뜡雅 (뜡堊 맃돚 씣쓵뜛 瀯앮풙)
		{
			g_MainCharInfo.ShowHelpMessage(IDS_CHANGEFIVEELM_NONE, TEXTEFFECT_COLOR_WARNING);
		}
		break;
	
	case 4:	//HT_0720 : 씣쓵 岳뷸 뜡雅 (30茹у븡 븨 삤뵳渦 씣쓵삊 若섉쎒 뜍 瀯앮풙)
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
 * 囹 뎵麗경뫗 썟簾 깙 뵾
 * \param &msg 
 * \return 
 */
int OnCS_IF_PARTYSHARE_ACK(CMsg &msg)
{
	//BYTE byExpDivision	=0;		// 뎵麗경뫗 썟簾 0-岳븀돚 썟簾, 1-굧궪 썟簾
	//BYTE byFEDivision	=0;		// 씣쓵 뵾 썟簾 0-岳븀돚 썟簾, 1-굧궪 썟簾

	if(g_MainCharInfo.m_pRelation)
	{
		msg
			>> g_MainCharInfo.m_pRelation->m_byExpDivision
			>> g_MainCharInfo.m_pRelation->m_byFEDivision;

		// TODO: 눦뎵윬 藥⒴쨭
		// 뒕 눓倻 阿먬삊윬 썟簾 깙 눦뎵
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
* 뎮
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

		//HT_CHEAT : 뎮 髥딀궪
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
* 뎮 꺈궪
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
			// TODO: 鰲믦룴 藥⒴쨭
		}
		break;
	default:
		break;
	}

	return 0;
}
