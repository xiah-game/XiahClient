ï»#include "csprotocol.h"
#include "mail.h"

#include ".\spirit.h"
#include "RebirthMark.h"

//HT_CHEAT : ‘¹é£ ™
extern BOOL g_bCheat;

//HT_CHEAT : ç£Šæ‚¼ é­„æ¦‚
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
			case 7: //é¦’ä¾© …’è¢ç”¸ ‰¢• œ‰†œ çªæ‰ 
				//if(pItemInfo->m_bRarity < 3 && pItemInfo->m_bStxType < 3 && pItemInfo->m_bModifyCnt < 1)
				if(pItemInfo->m_bModifyCnt < 1)
				{
					g_MainCharInfo.m_bAutoSell = true;
					g_MainCharInfo.m_bySellPos = i;
					return;
				}
				break;
			case 9: //è±ªç‰¢ çªœç˜¤ç» è¿«ç£Š 
			//	if(pItemInfo->m_bItemKind != 5)
			//	{
					g_MainCharInfo.m_bAutoSell = true;
					g_MainCharInfo.m_bySellPos = i;
					return;
			//	}
			case 15: //ç¡…æ‰¯
				g_MainCharInfo.m_bAutoSell = true;
				g_MainCharInfo.m_bySellPos = i;
				return;
			case 16://„ †ˆ 
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
							else if(pItemInfo->m_wVisualID != 9102) //„˜‘ ™å¿«ç»Š ä¹ä¿ƒ...„ …·ç§å· ä¸´æç» ä»·é¢ç» è¿«ç˜¤ å¯Œç£Š. 
							{
								g_MainCharInfo.m_bAutoSell = true;
								g_MainCharInfo.m_bySellPos = i;
								return;
							}
						}
					}
				}
				break;	
			case 18: // ‰¯ …’è¢
				//if(	pItemInfo->m_wRefID != 20963 &&
				//	pItemInfo->m_wRefID != 20965 &&
				//	pItemInfo->m_wRefID != 20966)
				//{
					g_MainCharInfo.m_bAutoSell = true;
					g_MainCharInfo.m_bySellPos = i;
					return;
			//	}
			case 19://æ£±ä¿ƒŒ„ å·´ç”¸(’¦œ‰ …’è¢ç”¸ ....)
				if(pItemInfo->m_wVisualID == 31000) //¥æ‹± ƒ‘ç£Šç»° †ˆ  å¿«åŠª…
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
			case 20: //ç¬ …’è¢ç”¸
			//	if(pItemInfo->m_wVisualID != 29200 && pItemInfo->m_wVisualID != 29201 )
			//	{
					g_MainCharInfo.m_bAutoSell = true;
					g_MainCharInfo.m_bySellPos = i;
					return;
			//	}	break;
			case 21: //æ°
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
			case 23: //è·
				//if( pItemInfo->m_wVisualID == 20000 ||
				//	pItemInfo->m_wVisualID == 20100 ||
				//	pItemInfo->m_wVisualID == 21000 ||
				//	pItemInfo->m_wVisualID == 21100)
				//{
					//if(pItemInfo->m_wRefID != 20073 && pItemInfo->m_wRefID  != 20070 ) //ªæ¼ç ç² …’æ»¨å. ›
					//if(pItemInfo->m_wRefID == 20073 || pItemInfo->m_wRefID  == 20070 ) //ªæ¼ç ç² …’æ»¨å. ›
					//{
						g_MainCharInfo.m_bAutoSell = true;
						g_MainCharInfo.m_bySellPos = i;
						return;
					//}
				//}
			case 25: //’ç«¿
				if( pItemInfo->m_wRefID == 20189 || pItemInfo->m_wRefID == 20267 )
				{
					g_MainCharInfo.m_bAutoSell = true;
					g_MainCharInfo.m_bySellPos = i;
					return;
				}
				break;
			case 33: //ä¿ºç—¢©
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
			case 7: //é¦’ä¾© …’è¢ç”¸ ‰¢• œ‰†œ çªæ‰ 
				//if(pItemInfo->m_bRarity < 3 && pItemInfo->m_bStxType < 3 && pItemInfo->m_bModifyCnt < 1)
				if(pItemInfo->m_bModifyCnt < 1)
				{
					g_MainCharInfo.m_bAutoSell = true;
					g_MainCharInfo.m_bySellPos = i;
					return;
				}
				break;
			case 9: //è±ªç‰¢ çªœç˜¤ç» è¿«ç£Š 
			//	if(pItemInfo->m_bItemKind == 0)
			//	{
					g_MainCharInfo.m_bAutoSell = true;
					g_MainCharInfo.m_bySellPos = i;
					return;
			//	}
			case 15: //ç¡…æ‰¯
				g_MainCharInfo.m_bAutoSell = true;
				g_MainCharInfo.m_bySellPos = i;
				return;
			case 16://„ †ˆ 
				if(pItemInfo->m_bItemKind >= 0 && pItemInfo->m_bItemKind < 4)// && pItemInfo->m_wVisualID != 9102)
				{
					if(g_PetList.size() == 0)// && pItemInfo->m_wVisualID != 9102) //„ ™å¿«ç»°  …’ªä¿.
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
							else if(pItemInfo->m_wVisualID != 9102) //„˜‘ ™å¿«ç»Š ä¹ä¿ƒ...„ …·ç§å· ä¸´æç» Ÿƒç¯ è¿«ç˜¤ å¯Œç£Š. 
							{
								g_MainCharInfo.m_bAutoSell = true;
								g_MainCharInfo.m_bySellPos = i;
								return;
							}
						}
					}
				}
				break;
			case 18: // ‰¯ …’è¢
				//if(	pItemInfo->m_wRefID != 20963 &&
				//	pItemInfo->m_wRefID != 20965 &&
				//	pItemInfo->m_wRefID != 20966)
				//{
					g_MainCharInfo.m_bAutoSell = true;
					g_MainCharInfo.m_bySellPos = i;
					return;
			//	}
			case 19://æ£±ä¿ƒŒ„ å·´ç”¸(’¦œ‰ …’è¢ç”¸ ....)
				if(pItemInfo->m_wVisualID == 31000) //¥æ‹± ƒ‘ç£Šç»° †ˆ  å¿«åŠª…
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
			case 20: //ç¬ …’è¢ç”¸
			//	if(pItemInfo->m_wVisualID != 29200 && pItemInfo->m_wVisualID != 29201 )
			//	{
					g_MainCharInfo.m_bAutoSell = true;
					g_MainCharInfo.m_bySellPos = i;
					return;
			//	}	break;
			case 21: //æ°
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
			case 23: //è·
				//if( pItemInfo->m_wVisualID == 20000 ||
				//	pItemInfo->m_wVisualID == 20100 ||
				//	pItemInfo->m_wVisualID == 21000 ||
				//	pItemInfo->m_wVisualID == 21100)
				//{
					//if(pItemInfo->m_wRefID != 20073 && pItemInfo->m_wRefID  != 20070 ) //ªæ¼ç ç² …’æ»¨å. ›
					//if(pItemInfo->m_wRefID == 20073 || pItemInfo->m_wRefID  == 20070 ) //ªæ¼ç ç² …’æ»¨å. ›
					//{
						g_MainCharInfo.m_bAutoSell = true;
						g_MainCharInfo.m_bySellPos = i;
						return;
					//}
				//}
			case 25: //’ç«¿
				if( pItemInfo->m_wRefID == 20189 || pItemInfo->m_wRefID == 20267 )
				{
					g_MainCharInfo.m_bAutoSell = true;
					g_MainCharInfo.m_bySellPos = i;
					return;
				}
				break;
			case 33: //ä¿ºç—¢©
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
	sString szNickName;//HO_0313_07  ¡–™‚ˆ è¯•é£˜è‚º ‰¢Œ„ œ •Š
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

			//HT_CHEAT : ç£Šæ‚¼ é­„æ¦‚
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

	// PARTY BATTLEä¿ŠçŸ¾Ÿ« £ ˆ¸­ä¿ Œ…Œ„ š‹˜¤
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
	case 5:	// ˜é¦
		{
			g_MainCharInfo.ShowHelpMessage(IDS_PET_ITEM_1);			
		}
		break;
	case 6:	// 
		{
			g_MainCharInfo.ShowHelpMessage(IDS_PET_ITEM_2);
		}
		break;
	
	case HM_BLOODDEVIL://HO_0313_07  ¡–™‚ˆ è¯•é£˜è‚º ‰¢Œ„ œ •Š
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
	// CG_2005/01/28 : ‡½è¾†é…’è¢æ‰“·œ •Š
	// ‡½è¾†é…’è¢èé”­ œ‰†œ
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
		>> g_MainCharInfo.m_dwPremiumTP //HT_0911 : æ©‡åºœ›º†³ æ¶…èƒ¶é£ è®¿æ‘¹
		>> g_MainCharInfo.m_dwPremiumSP
		>> g_MainCharInfo.m_dwGameMasterMark; //HT_1023 : æ¬¾åº·ç£ ä»˜å†œ œ •Š
	
	g_MainCharInfo.RefreshChracterInfo();
	g_MainCharInfo.RefreshMainFrame();
	g_MainCharInfo.RefreshMugongFrame();

	// ›’„š‹œ å®˜å±‚œ–ä¿.
	sPetInfo *pPet = g_PetList.GetBunsinPet();
	if(pPet != NULL)
	{
		pPet->bSpeed = g_MainCharInfo.m_bWalkSpeed;
	}

	// useitem_ack•Š ç»ç»° Œ…‹Œ‚º..
	for( int i=0; i < 5; ++i)
		g_MainCharInfo.m_pSlot->SetSlotToolTip(i);

	// CG_2005/01/28 : ‡½è¾†é…’è¢æ‰“·œ •Š
	CXiahCharObject* pObject = (CXiahCharObject*)g_pMainChar->m_pObject;
	if( pObject )
	{
		pObject->m_bChangeItemSet = bChangeItemSet;
		pObject->RefreshFameColor(g_MainCharInfo.m_dwFame);
		pObject->m_bRebirth = g_MainCharInfo.m_bRebirth;
		pObject->m_bGameMasterMark = g_MainCharInfo.m_dwGameMasterMark; //HT_1023 : æ¬¾åº·ç£ ä»˜å†œ œ •Š
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
	case 0:		// ç£Šæ‚¼ æ±
		break;
		// ‹±è· ¤ä¾
	case 1:		// HP
		{
			_EFFECTPACKAGE* pEffectPackage = g_EffectManager.EnqAppearEffectImmediately( eMulYak_HP );

			if( pEffectPackage && pEffectPackage->pEffectRender && pEffectPackage->pEffectRender->pPackagePair )
			{
				pEffectPackage->pEffectRender->pPackagePair->pWorldMatrix = (MATRIX*)pCharObject->m_CharRender.GetCharTM();
//				pEffectPackage->pEffectRender->pPackagePair->WorldMatrix = (MATRIX)pCharObject->m_ObjectTM;

				// çº³ç§¦…· Ÿ…ç£å•Š ç»ç»¢é¾™é”­ æ£‹é£˜æ¡  ç»ç»¢Ÿ³ä¿.
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
		// …¬‚
	case 11:	// ‚ˆœ¡æ¾ - é¸¥ç‰¢ ‘¹ä¸.
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
	case 12:	//  ¡æ¾ - çª ‘¹ä¸.
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
 * ‰ˆæ°°æ‘¹
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

		//100™Œç£ æª¬è‹Ÿ« Š›Œ„
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

		//HO_0413_07 ‹ •Š› è¯•é£ : ‚ª œ‰†œé¥ æ»šç“¢ ˜¤æ¡
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

		//100™Œç£ æª¬è‹Ÿ« Š›Œ„
		if(g_MainCharInfo.m_wLevel<100)
		{
            g_MainCharInfo.m_i64LevelExp = g_MainCharInfo.m_i64NextLevelUpExp;
            g_MainCharInfo.m_i64NextLevelUpExp = i64NextLevelUpExp;
		}
		else
		{
			g_MainCharInfo.m_wLevel=100;
		}

		// æµšæŸ³ä¿ ä¹ç»° ™¨‰¢ç£å•Š Šª‰¼”«é£˜ç‹¼ •é¸¥ç”« •Šåºœè™ Š›ª‚º ç´‚¼çªç˜¤ ‡¼æ¾.
		// å¼Šè´°è¾ æµšæŸ³‹¼ ™¨‰¢ç£å•Š æµšæŸ³‹¼ •é¸¥ç”« •Šåºœè™æ¡£åºŸ çªæ ‡³.
		// è¡¬çª æµšæŸ³ç¯ DLLª˜ª.
		// effect
		CXiahCharObject* pCharObject = (CXiahCharObject*)g_pMainChar->m_pObject;
		_EFFECTPACKAGE* pPackage = g_EffectManager.EnqLevelUpEffectImmediately( LEVELUP_GAPJA );
		if( pPackage && pPackage->pEffectRender && pPackage->pEffectRender->pPackagePair )
		{
			pPackage->pEffectRender->pPackagePair->pWorldMatrix = (MATRIX*)pCharObject->m_CharRender.GetCharTM();
			//pPackage->pEffectRender->pPackagePair->WorldMatrix = (MATRIX)pCharObject->m_ObjectTM;

			// çº³ç§¦…· Ÿ…ç£å•Š ç»ç»¢é¾™é”­ æ£‹é£˜æ¡  ç»ç»¢Ÿ³ä¿.
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

		// TODO: æ£‹é£˜ œ •Š / ¤æ¬¾é› œ •Š
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
 * çª „š ©“¦
 * \param &msg 
 * \return 
 */
int OnCS_IF_ASKPARTY_ACK( CMsg &msg)
{
	// bResult	0 : é¢‡è “°‰¢‡³
	//			4 : ??
	//			5 : ç§å¯ Ÿ…ç£ç”« Œ«˜¤ ç»™çªƒ
	//			6 : é¢‡è ç§å· è§’è© ( ç§‹æ©‡š‹Ÿ«˜¤‚º •Š˜¤ ‡¼˜‘é³..)
	//			9 : é¢‡è Š­ä¾‹å‡³
	DWORD dwAskID		=0;
	DWORD dwAskedID		=0;
	BYTE bResult		=0;
	BYTE bPartyType		=0;	// 0-é¦†çª 1-Œ…‹Œçª

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
				// é¦† çª
				_stprintf( szText, IDS_D_MAKE_DAN, (LPCTSTR)g_MainCharInfo.FindNameByID( dwAskID));

				if( !g_pUIManager->ShowNotice( szText, NOTICE_FRAME_OKCANCEL, NOTICE_FRAME_ASKPARTY))
				{
					// Š­ä¾
					SendCS_IF_ASKPARTY_REQ(g_MainCharInfo.m_dwAskID, g_MainCharInfo.m_dwObjectID, 9);
				}
			}
			else if(bPartyType == 1)
			{
				// Œ…‹Œ çª
				_stprintf(szText, IDS_D_MAKE_REL_DAN, (LPCTSTR)g_MainCharInfo.FindNameByID(dwAskID));

				if(!g_pUIManager->ShowNotice(szText, NOTICE_FRAME_OKCANCEL, NOTICE_FRAME_RELATION_ASKPARTY))
				{
					// Š­ä¾
					SendCS_IF_ASKPARTY_REQ(g_MainCharInfo.m_dwAskID, g_MainCharInfo.m_dwObjectID, 9, 1);
				}
			}
		}
		break;

	case 9:	// çª „šæ² Š­ä¾‹å‡³
		{
			g_MainCharInfo.m_dwAskID = 0;
			g_MainCharInfo.ShowHelpMessage( IDS_REJECT_DAN, TEXTEFFECT_COLOR_WARNING);
		}
		break;

	case 4:	// ™Œç£ ’‚º çª ç§’å¶å‡³
		if(dwAskID == g_MainCharInfo.m_dwObjectID)	
		{
			g_MainCharInfo.ShowHelpMessage( IDS_AUTO_CANCEL, TEXTEFFECT_COLOR_WARNING);
		}
		break;

	case 5:	// ç£Šè„šç¯ çªœæ—•Š …’ä¸
		{
			TCHAR strText[100] = {0,};

			_stprintf(strText, IDS_DAN_NOLEADER, (LPCTSTR)g_MainCharInfo.m_szNickName);
			g_MainCharInfo.ShowHelpMessage(strText, TEXTEFFECT_COLOR_WARNING);
		}
		break;

	case 7: // çª ‰¢›” •Š«è°©æ¾œ
		{
			g_MainCharInfo.ShowHelpMessage(IDS_DAN_EXCEED, TEXTEFFECT_COLOR_WARNING);
		}
		break;

	case 8:	// ƒ‘ªè§„æ ›º ä¿ƒå¼— çªœä¿Š å®¶åŠ  ™»ç» ä¹æ¾œ
		{
			g_MainCharInfo.m_dwAskID = 0;
			g_MainCharInfo.ShowHelpMessage(IDS_DAN_OTHER_DAN, TEXTEFFECT_COLOR_WARNING);
		}
		break;

	case 11:// ¯è® Œ…‹Œ Š­ä½ ƒ‘
		{
			g_MainCharInfo.m_dwAskID = 0;
			g_MainCharInfo.ShowHelpMessage(IDS_REJECT_DAN_2, TEXTEFFECT_COLOR_WARNING);
		}
		break;

	case 12: // Œ…‹Œ çª ‚˜‘ ç»é˜‘”­
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
 * çª ç§å·
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
		>> bPartyType;	// 0 é¦†çª, 1 Œ…‹Œçª

	g_MainCharInfo.m_pRelation->m_nDanType = static_cast<int>(bPartyType);

	// è§„å˜ ª”­ ‘›”µ è´¸åºœ”« ç§…·çª
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

		// çª ‰ˆæ°°æ‘¹ ›’ç¡
		g_pUIManager->Hide(WINDOW_DAN_NEW, window_dan_new_1button);
		g_pUIManager->Show(WINDOW_DAN_NEW, window_dan_new_2button_01);
		g_pUIManager->Show(WINDOW_DAN_NEW, window_dan_new_2button_02);

		g_MainCharInfo.m_pRelation->SetDanID( dwPartyID);
		g_MainCharInfo.m_pRelation->SetDanLeader( dwLeaderID);
		// çª ”˜
		g_MainCharInfo.m_pRelation->InsertDan( g_MainCharInfo.m_dwObjectID, byPriority, g_MainCharInfo.m_szNickName,
										byLevel, wPosX, wPosY, g_MainCharInfo.m_dwHpCur, g_MainCharInfo.m_dwHpMax, 0);
	}
	
	g_MainCharInfo.ShowHelpMessage( IDS_MAKE_DAN);

	return 0;
}

/**
 * çªœç›” œ •Š
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

	// çª ”˜
	g_MainCharInfo.m_pRelation->InsertDan( dwCharID, bPartybPriority, szName, wLevel, wPosX, wPosY, dwCurHp, dwMaxHp, 0);

	TCHAR szText[100] = {0,};
	_stprintf( szText, IDS_D_JOIN_DAN, (LPCTSTR)szName);
	
	g_MainCharInfo.ShowHelpMessage( szText);

	return 0;
}

/**
 * çª åºœèƒ¶é£
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
		>> bPartyType;	// 0 é¦†çª, 1 Œ…‹Œçª

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
	// bResult	0 : é¢‡è “°‰¢‡³
	//			4 : ??
	//			5 : ç§å¯ Ÿ…ç£ç”« Œ«˜¤ ç»™çªƒ
	//			6 : é¢‡è ç§å· è§’è© ( ç§‹æ©‡š‹Ÿ«˜¤‚º •Š˜¤ ‡¼˜‘é³..)
	//			9 : é¢‡è Š­ä¾‹å‡³
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
				// Š­ä¾
				SendCS_IF_INVITEPARTY_REQ( g_MainCharInfo.m_dwAskID, g_MainCharInfo.m_dwObjectID, 9);
			}
		}
		break;
	case 9:
		if( dwAskID == g_MainCharInfo.m_dwObjectID)
			g_MainCharInfo.ShowHelpMessage( IDS_REJECT_DAN, TEXTEFFECT_COLOR_WARNING);
		break;

	case 12: // Œ…‹Œ çª ‚˜‘ ç»é˜‘”­
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
	//HO_0419_07 ©™œ •Š : ©“¦ œ •Š‚ˆ †…›
	//DWORD dwPartyID;
	//DWORD dwCharID;

	//msg
	//	>> dwPartyID
	//	>> dwCharID;

	//if( g_MainCharInfo.m_pRelation->GetDanID() == dwPartyID)
	//	g_MainCharInfo.m_pRelation->SetDanLeader( dwCharID);

	DWORD dwPartyID;
	DWORD dwCharID;
	BYTE  bResult;	////HO_0419_07 ©“¦œ •Š bResultœ •Š bResult ”¼ä¿ ¶‰¼ dwPartyID, dwCharID”« ç½ç»Š •‘ç½ç»Šçª

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
			//ProcessClickRelationButton( eDAN); //HO_0424_07 çªœæ©‡é¥ƒ™Š’ ä¿ƒçŸ« “†ç»°å·´˜‘ ˜œæ¾
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

	// ”±‹¼ çª æ²¥ç„Šç» ƒ´•Š
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

	// bResult	0 : é¢‡è “°‰¢‡³
	//			4 : ??
	//			5 : ç§å¯ Ÿ…ç£ç”« Œ«˜¤ ç»™çªƒ
	//			6 : é¢‡è ç§å· è§’è© ( ç§‹æ©‡š‹Ÿ«˜¤‚º •Š˜¤ ‡¼˜‘é³..)
	//          8 : ¯è® Š­ä½ ƒ‘
	//			9 : é¢‡è Š­ä¾‹å‡³

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
				// Š­ä¾
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
	// •‘•œæ³¢ä¿ƒ.

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

		// ‚ˆè¾‘å¤‡ åºœèƒ¶é£˜ä¿Š œ •Š
		// bTypeç¯ csProtocol ›¼‚¼
		if(bType == RELATION_TYPE_BUDDY)
			g_Mail.Add_SendList(dwCharID,szNickName,MAIL_BUDDY,bType);		// æ¨¡å¤‡”¸
		else	
			g_Mail.Add_SendList(dwCharID,szNickName,MAIL_RELATION,bType);	// æ¨¡å¤‡”« Š›å¯‡èŒ„ ‰¢æ¥
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

		// ‚ˆè¾‘å¤‡ åºœèƒ¶é£˜ä¿Šè¾ Š›Š­
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
	// •‘•¬ä¿.

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

		// ‚ˆè¾‘å¤‡
		// bTypeç¯ csProtocol ›¼‚¼
		if(bType == RELATION_TYPE_BUDDY)
			g_Mail.Add_SendList(dwCharID,szNickName,MAIL_BUDDY,bType);		// æ¨¡å¤‡”¸
		else	
			g_Mail.Add_SendList(dwCharID,szNickName,MAIL_RELATION,bType);	// æ¨¡å¤‡”« Š›å¯‡èŒ„ ‰¢æ¥
	}

	return 0;
}

int OnCS_IF_CHANGEBUDDYCONNECT_ACK( CMsg &msg)
{
	// •‘•¬ä¿.

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
//------------- å±‚¯‚¼‹±
/**
 * „åºœèƒ¶é£
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

		// ˆ¶”¸ç» —‰
		XiahObject::CXiahObject *pPetObject = NULL;
		if((pPetObject = XiahObject::g_XiahObjectManager.FindXiahObject( MAKEOBJECTID( 0, dwID, OBJTYPE_PET))) != NULL)
		{	
			sArrayData* pData = XiahArrayIndex::g_NpcType.GetData( bNpcType);

			if( pData == NULL)
				continue;

			int nCharID			= pData->GetInt( 1);
			int nMeshType		= pData->GetInt( 2);
			int nTextureType	= pData->GetInt( 3);

			if(bRevolutionStep == 4 && bNpcType == 0) //HT_0621 : åº·è•Œ™Œ„šçª ˆ©ä¾
			{
				// [10/31/2005] Ÿ…ç£ ID‡½‰ˆ
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

			if(pObject && bRevolutionStep == 4 && bNpcType == 0) //HT_0621 : åº·è•Œ™Œ„šçª ˆ©ä¾
				SetupPET_VisualEquipement(pObject, wVisualID);

			continue;
		}

		sArrayData* pData = XiahArrayIndex::g_NpcType.GetData( bNpcType);
		
		if( pData == NULL)
			continue;

		int nCharID			= pData->GetInt( 1);
		int nMeshType		= pData->GetInt( 2);
		int nTextureType	= pData->GetInt( 3);

		if(bRevolutionStep == 4 && bNpcType == 0) //HT_0621 : åº·è•Œ™Œ„šçª ˆ©ä¾
		{
			// [10/31/2005] Ÿ…ç£ ID‡½‰ˆ
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

		if(bRevolutionStep == 4 && bNpcType == 0) //HT_0621 : åº·è•Œ™Œ„šçª ˆ©ä¾
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
 * ·’ ‘¹ æ£µåºœ‰
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
	case ERR_EXECFIVEELM_NEEDPOINT:	// ·’ è®¿æ‘¹ ä½•ç»ƒ
		{			
			g_MainCharInfo.ShowHelpMessage(IDS_EXECFIVEELM_NEEDPOINT, TEXTEFFECT_COLOR_WARNING);
		}
		break;
	case ERR_EXECFIVEELM_NOTFINDCHAR:
		{

		}
		break;
	case 3:	// ‘›æ» Š³èºæ¾
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
 * ·’ ¥ç¶
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
	case 0:				//HT_0720 : ·’ ä¿ºæ ¤äº
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
	case 1:	// ·’ ¤ä¾  ä¿ƒå¼— ·’ ¥ç¶ Ÿ« ‡½‰ˆ ˜‚•Š
		{
			g_MainCharInfo.ShowHelpMessage(IDS_CHANGEFIVEELM_ONAIR, TEXTEFFECT_COLOR_WARNING);		
		}
		break;
	case 2:	// “·ä» ä½•ç»ƒ ‚º ¤ä¾©ä¸” ç»æ¾œ
		{
			g_MainCharInfo.ShowHelpMessage(IDS_CHANGEFIVEELM_NOTYET, TEXTEFFECT_COLOR_WARNING);	
		}
		break;
	
	case 3: //HT_0720 : ·’ ä¿ºæ ¤äº (¤ä¾ ‰¢ ·’ ç»æ¾œ)
		{
			g_MainCharInfo.ShowHelpMessage(IDS_CHANGEFIVEELM_NONE, TEXTEFFECT_COLOR_WARNING);
		}
		break;
	
	case 4:	//HT_0720 : ·’ ä¿ºæ ¤äº (30æª¬å•Š •‘ ˜¤”±è¾ ·’˜‘ å®˜æ›¹  ç»æ¾œ)
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
 * çª ‰ˆæ°°æ‘¹ ›’ç¡ ƒ‘ ”¼
 * \param &msg 
 * \return 
 */
int OnCS_IF_PARTYSHARE_ACK(CMsg &msg)
{
	//BYTE byExpDivision	=0;		// ‰ˆæ°°æ‘¹ ›’ç¡ 0-ä¿ºç‰¢ ›’ç¡, 1-‚‚¼ ›’ç¡
	//BYTE byFEDivision	=0;		// ·’ ”¼ ›’ç¡ 0-ä¿ºç‰¢ ›’ç¡, 1-‚‚¼ ›’ç¡

	if(g_MainCharInfo.m_pRelation)
	{
		msg
			>> g_MainCharInfo.m_pRelation->m_byExpDivision
			>> g_MainCharInfo.m_pRelation->m_byFEDivision;

		// TODO: ‡½‰ˆŸ« å·©å¤‡
		// Š’ ‡¯å¦ ä¹é˜‘Ÿ« ›’ç¡ ƒ‘ ‡½‰ˆ
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
* ‰
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

		//HT_CHEAT : ‰ ç£Šæ‚¼
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
* ‰ ƒ¯‚¼
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
			// TODO: è§’è© å·©å¤‡
		}
		break;
	default:
		break;
	}

	return 0;
}
