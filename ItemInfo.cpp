#include "precompile.h"
#include "XiahArrayIndex.h"
#include "ItemInfo.h"
#include "XiahObjectType.h"

#include <assert.h>

namespace XiahItem
{
	BOOL SetItemVisualData(sItemInfo* pInfo)
	{
		sArrayData* pData = XiahArrayIndex::g_ItemType.GetData( pInfo->m_wVisualID);

//		DBG_Assert( pData != NULL);

		if( pData == NULL)
			return FALSE;

		pInfo->m_nResID				= pData->GetInt( 1);
		pInfo->m_bSackSizeX			= pData->GetInt( 2);
		pInfo->m_bSackSizeY			= pData->GetInt( 3);
		pInfo->m_bItemType			= pData->GetInt( 4);
		pInfo->m_bItemKind			= pData->GetInt( 5);
		pInfo->m_bEquipPos			= pData->GetInt( 6);
		pInfo->m_nMapCharID			= pData->GetInt( 7);
		pInfo->m_nMapMeshType		= pData->GetInt( 8);
		pInfo->m_nMapTextureType	= pData->GetInt( 9);
		pInfo->m_nEquipCharID		= pData->GetInt(10);
		pInfo->m_nEquipMeshType		= pData->GetInt(11);
		pInfo->m_nEquipTextureType	= pData->GetInt(12);
		pInfo->m_bConstraintCharType= pData->GetInt(13);
		//pInfo->m_szName				= pData->GetString( 0);

		// 왜 이렇게 했지??
		if( !pInfo->m_szName || pInfo->m_szName == _T(""))
			pInfo->m_szName				= pData->GetString( 0);

		return TRUE;
	}

	BOOL ReleaseItemInfo(DWORD pInfo)
	{
		delete (sItemInfo*)pInfo;	// 안녕~~
		pInfo = NULL;
		return TRUE;
	}

	void GetItemData( sItemInfo* pItem, CMsg& msg)
	{
		assert(pItem);

		if(NULL == pItem)
			return;

		WORD wAmount	=0;

		msg
			>> pItem->m_dwItemID
			>> pItem->m_wRefID
			>> pItem->m_bItemType
			>> pItem->m_bItemKind
			>> pItem->m_wVisualID
			>> pItem->m_szName
			>> pItem->m_dwPrice
			>> pItem->m_wLevel
			>> pItem->m_bNeedCharType
			>> wAmount;

        pItem->m_dwAmount = wAmount;

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
					>> pItem->m_bDecrDurRate
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
					>> pItem->m_wIncrCritical
					>> pItem->m_bRarity
					>> pItem->m_bStxType
					>> pItem->m_bLimitCnt
					>> pItem->m_bModifyCnt
					// CG_2005/01/28 : 변종아이템기능추가
					>> pItem->m_bRepairCnt
					>> pItem->m_bRepairDiscount;
	
				if( pItem->m_bItemType == ITEMTYPE_BONGIN )
				{
					msg
                        >> pItem->m_dwNpcID
						>> pItem->m_wSoakHPRatio
						>> pItem->m_wSoakAtkRatio
						>> pItem->m_wSoakDefRatio
						>> pItem->m_wSoakHitRatio;
				}
				else if(pItem->m_bItemType == ITEMTYPE_SOCKET)
				{
					msg
						>> pItem->m_bDanIncExp		
						>> pItem->m_bMopDecAtk		
						>> pItem->m_bMopDecDef		
						>> pItem->m_bMopDecAtkRatio	
						>> pItem->m_bMopDecHP			
						>> pItem->m_bShopDecTax		
						>> pItem->m_bGambleShopDec	
						>> pItem->m_bEffectType;

					pItem->m_bRepairDiscount = 0;
				}
				else 
				{
					msg							
						>> pItem->m_bPuzzleType;	// 조합된 아이템 타입
				}
			
				// 제련
				switch(pItem->m_bItemType)
				{
					case ITEMTYPE_WEAPON:
					case ITEMTYPE_CLOTH:
					case ITEMTYPE_HAT:
					case ITEMTYPE_SHOE:
						{
							BYTE bSocketItem[3] = {0,};
							WORD wRBSocketItem = 0;

							msg
								>> bSocketItem[0]
								>> bSocketItem[1]
								>> bSocketItem[2]
								>> wRBSocketItem;//HT_1116 : 각성자 아이템 추가
								
							BYTE bSocketCount = 0;
							for(int i=0; i < 3; ++i)
							{
								if(bSocketItem[i])
									++bSocketCount;

								if(1 != bSocketItem[i])
								{
									pItem->m_bSocketItem[i] = bSocketItem[i];
								}
								else
								{
									pItem->m_bSocketItem[i] = 0;
								}
							}
	
							pItem->m_wRBSocketItem = wRBSocketItem;
					
							pItem->m_bSocketCount = bSocketCount;
							assert(bSocketCount <= 3);
						}
						break;
				} // switch(pItem->m_bItemType)
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

		case ITEMTYPE_NPCBAG:
			{
				msg
					>> pItem->m_bDecrDurRate
					>> pItem->m_wCurDur
					>> pItem->m_wMaxDur
					>> pItem->m_bNpcRace
					>> pItem->m_bNpcBagSize;
			}
			break;
		case ITEMTYPE_NPCITEM:
			{
				msg 
					>> pItem->m_wTamingLevel
					>> pItem->m_bNpcItemType
					>> pItem->m_wTamingRate
					>> pItem->m_bWildRate
					>> pItem->m_wIncrHp;
			}
			break;

		case ITEMTYPE_SUNANG:
			{
				msg
					>> pItem->m_bFuncID
					>> pItem->m_dwValue
					>> pItem->m_wCurDur
					>> pItem->m_wMaxDur
					>> pItem->m_bModifyCnt;
			}
			break;

		case ITEMTYPE_EVENT:
			{//HO_0828_07 황금열쇠 추가 : 기존에는 이벤트 일경우 값이 없었으나 황금열쇠 추가되며 값이 들어 가게 되었다.	
			msg	
				>> pItem->m_wKeyRefID 
				>> pItem->m_wKeyAmount; 
			}
			break;

		case ITEMTYPE_SURESOURCE:
			{
				msg
					>> pItem->m_bFuncID
					>> pItem->m_dwValue;
			}
			break;

		case ITEMTYPE_BOOK:
			{
				msg
					>> pItem->m_wNeedLevel
					>> pItem->m_dwMugongID
					>> pItem->m_bMugongType
					>> pItem->m_bMugongKind
					>> pItem->m_bNeedMugongLevel;
			}
			break;
		case ITEMTYPE_PORTAL:
			{	
				msg
					>> pItem->m_dwPotalMapID
					>> pItem->m_bPortalType
					>> pItem->m_wPosX
					>> pItem->m_wPosY;
			}
			break;
		case ITEMTYPE_POTION:
			{
				msg
					>> pItem->m_dwKeepUpTime
					>> pItem->m_wIncrHp
					>> pItem->m_wIncrIp
					>> pItem->m_bMinLevel
					>> pItem->m_bMaxLevel;
			}
			break;

		case ITEMTYPE_REBUILDRES:
			{
				// 개조 가능여부
				// 0개조가능 1불가
				msg
					>> pItem->m_bIsDividedRes
					>> pItem->m_wSuccessRatio
					>> pItem->m_wFactorValue;
				
			}
			break;
		case ITEMTYPE_QUEST:
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
		case ITEMTYPE_MANUAL:
			{
				msg
					>> pItem->m_wOriginRefID
					>> pItem->m_wUnionRefID;
			}
			break;


		case ITEMTYPE_GISDURABLITY:
			{
				msg
					>> pItem->m_wFunctionItem // 기능번호
					>> pItem->m_wCurDur
					>> pItem->m_wMaxDur		 // 최대내구력
					>> pItem->m_dwValue;	 // 전낭용
			}
			break;
		case ITEMTYPE_REBIRTH:
			{
				msg
                    >> pItem->m_bStepID
					>> pItem->m_wRebirthFuncID
                    >> pItem->m_wRebirthNeedLevel;
			}
			break;
		case ITEMTYPE_PREMIUMQUEST:
			{
				msg
					>> pItem->m_dwPremiumQuestID
					>> pItem->m_bLimitCnt;
			}

		default:
			break;
		}
	}
};
