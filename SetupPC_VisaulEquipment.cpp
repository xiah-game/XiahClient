// 装备外观加载调试日志（Release 关闭）
static void LogEquipVisual(const char* fmt, ...) {}

BOOL SetupPC_VisualEquipement(CXiahCharObject* pObject, WORD* pVisualList, BYTE* pRarityList, BYTE* pStxTypeList)
{
	XiahItem::sItemInfo info;

	// CG_2005/01/27 : 변종아이템기능추가
	int nCharID = pObject->m_CharRender.GetCharID();

	// ===== 调试日志：输出全部穿戴位 VisualID =====
	LogEquipVisual("======== SetupPC_VisualEquipement ========\n");
	LogEquipVisual("  CharRenderID=%d  SubObjType=%d\n", nCharID, pObject->m_bSubObjType);
	const char* slotNames[] = {"WEAPON","HAT","CLOTH","SHOE","PROTECTOR","RING","NECLACE","CLOAK","BONGIN","(9)","KEY"};
	for (int _i = 0; _i < 11; _i++)
		LogEquipVisual("  Slot[%d]=%s  VID=%d\n", _i, slotNames[_i], (int)pVisualList[_i]);

	if( pVisualList[ EQUIPPOS_CLOTH] != 0)
	{
		info.m_wVisualID = pVisualList[ EQUIPPOS_CLOTH];

		if( XiahItem::SetItemVisualData( &info))
		{
			LogEquipVisual("  [CLOTH] VID=%d -> CharID=%d Mesh=%d Tex=%d (eLBP_Protector)\n",
				info.m_wVisualID, info.m_nEquipCharID, info.m_nEquipMeshType, info.m_nEquipTextureType);
			pObject->AttachChildCharRender( eLBP_Protector, info.m_nEquipCharID, info.m_nEquipMeshType, info.m_nEquipTextureType);
		}
	}
	else
	{
		switch( pObject->m_bSubObjType)
		{
		case 1://검영
			pVisualList[ EQUIPPOS_CLOTH] = 2000;
			break;
		case 2://연랑
			pVisualList[ EQUIPPOS_CLOTH] = 2100;
			break;
		case 3://무투
			pVisualList[ EQUIPPOS_CLOTH] = 2200;
			break;
		case 4:// 야차
			pVisualList[ EQUIPPOS_CLOTH] = 2300;
			break;
		}
		info.m_wVisualID = pVisualList[ EQUIPPOS_CLOTH];

		if( XiahItem::SetItemVisualData( &info))
		{
			LogEquipVisual("  [CLOTH-Default] VID=%d -> CharID=%d Mesh=%d Tex=%d (eLBP_Protector)\n",
				info.m_wVisualID, info.m_nEquipCharID, info.m_nEquipMeshType, info.m_nEquipTextureType);
			pObject->AttachChildCharRender( eLBP_Protector, info.m_nEquipCharID, info.m_nEquipMeshType, info.m_nEquipTextureType);
		}
	}

	// 披风渲染：根据披风模型是否和主角色模型相同，采用不同的渲染策略
	// - 同模型（云朗867/夜叉906 等）：只换贴图层3，衣服和披风共存
	// - 不同模型（剑侠790→1253 等）：走 eLBP_Protector 重建模型
	if( pVisualList[ EQUIPPOS_CLOAK] != 0)
	{
		info.m_wVisualID = pVisualList[ EQUIPPOS_CLOAK];

		if( XiahItem::SetItemVisualData( &info))
		{
			int nCurrentCharID = pObject->m_CharRender.GetCharID();

			if( info.m_nEquipCharID == nCurrentCharID)
			{
				// 同模型：只换贴图层3（披风/斗篷专属层），保留层0（衣服层）
				LogEquipVisual("  [CLOAK] VID=%d -> CharID=%d Mesh=%d Tex=%d (同模型, ChangeTexture layer 3)\n",
					info.m_wVisualID, info.m_nEquipCharID, info.m_nEquipMeshType, info.m_nEquipTextureType);

				CRes_Character* pCloakChar = XiahGameEngine::GetCharacter( info.m_nEquipCharID);
				if( pCloakChar)
				{
					Res_Mesh* pCloakMesh = pCloakChar->GetMesh( info.m_nEquipMeshType);
					if( pCloakMesh)
					{
						Res_CharTexture* pCloakTex = pCloakMesh->GetTexture( info.m_nEquipTextureType);
						if( pCloakTex && pCloakTex->texture_sub_count >= 4)
						{
							pObject->m_CharRender.ChangeTexture( 3, pCloakTex->texture_sub_ptr[ 3].texture_id);
						}
						else if( pCloakTex && pCloakTex->texture_sub_count >= 1)
						{
							pObject->m_CharRender.ChangeTexture( 3, pCloakTex->texture_sub_ptr[ 0].texture_id);
						}
					}
				}
			}
			else
			{
				// 独立3D模型（如剑侠盾牌 CharID=1253）：挂载到左手骨骼点
				// 不同职业的 CLOAK 栏位可以是不同形态的物品（披风/盾牌等），
				// 独立模型挂载到 eLBP_LeftHand，和武器（eLBP_RightHand）搭配
				LogEquipVisual("  [CLOAK] VID=%d -> CharID=%d Mesh=%d Tex=%d (独立模型, eLBP_LeftHand)\n",
					info.m_wVisualID, info.m_nEquipCharID, info.m_nEquipMeshType, info.m_nEquipTextureType);
				pObject->AttachChildCharRender( eLBP_LeftHand, info.m_nEquipCharID, info.m_nEquipMeshType, info.m_nEquipTextureType);
			}
		}
	}

	if( pVisualList[ EQUIPPOS_WEAPON] != 0)
	{
		info.m_wVisualID = pVisualList[ EQUIPPOS_WEAPON]; // 무기
		if( XiahItem::SetItemVisualData( &info))
		{	// 여기서 무기에 붙는 이펙트의 인덱스를 결정한다.
			// +
			int nREffectIndex = -1;
			if( pRarityList != NULL )
			{
				if( pRarityList[ EQUIPPOS_WEAPON ] >=1 && pRarityList[ EQUIPPOS_WEAPON ] <=2 )
					nREffectIndex = 0;
				else
				if( pRarityList[ EQUIPPOS_WEAPON ] >=3 && pRarityList[ EQUIPPOS_WEAPON ] <=4 )
					nREffectIndex = 1;
				else
				if( pRarityList[ EQUIPPOS_WEAPON ] >=5 && pRarityList[ EQUIPPOS_WEAPON ] <=6 )
					nREffectIndex = 2;
				else
				if( pRarityList[ EQUIPPOS_WEAPON ] >=7 )
					nREffectIndex = 3;
			}

			// 성.
			int nSEffectIndex = -1;
			if( pStxTypeList != NULL )
			{
				// 发光特效由强化等级驱动：+1~3基础, +4~10中级, +11~20高级, +21+极品
				if( pStxTypeList[ EQUIPPOS_WEAPON ] >=1 && pStxTypeList[ EQUIPPOS_WEAPON ] <=3 )
					nSEffectIndex = 0;
				else
				if( pStxTypeList[ EQUIPPOS_WEAPON ] >=4 && pStxTypeList[ EQUIPPOS_WEAPON ] <=10 )
					nSEffectIndex = 1;
				else
				if( pStxTypeList[ EQUIPPOS_WEAPON ] >=11 && pStxTypeList[ EQUIPPOS_WEAPON ] <=20 )
					nSEffectIndex = 2;
				else
				if( pStxTypeList[ EQUIPPOS_WEAPON ] >=21 )
					nSEffectIndex = 3;
			}

			// 큰 인덱스를 선택.
			int nEffectIndex = nREffectIndex;
			if( nEffectIndex < nSEffectIndex )
				nEffectIndex = nSEffectIndex;

			// 망치가 캐릭터 마다 다르다.
			if(info.m_wVisualID >= 101 && info.m_wVisualID <= 107)
			{
				switch(pObject->m_bSubObjType)
				{
					case 1:	// 검영
						{
							info.m_nEquipCharID = 749;
						}					
						break;
					case 2:	// 연랑
						{
							info.m_nEquipCharID = 750;
						}
						break;
					case 3:	// 무투
						{
							info.m_nEquipCharID = 889;
						}
						break;
					case 4:	// 야차
						{
							info.m_nEquipCharID = 966;
						}
						break;
					default:
						break;
				}
			}


			switch( pObject->m_bSubObjType )
			{
				case 1:	// 검영
				case 2:	// 연랑
				case 3:	// 무투
					LogEquipVisual("  [WEAPON] VID=%d -> CharID=%d Mesh=%d Tex=%d Effect=%d (eLBP_RightHand)\n",
						info.m_wVisualID, info.m_nEquipCharID, info.m_nEquipMeshType, info.m_nEquipTextureType, nEffectIndex);
					pObject->AttachChildCharRender( eLBP_RightHand, info.m_nEquipCharID, info.m_nEquipMeshType, info.m_nEquipTextureType, nEffectIndex );
					break;
				case 4:// 야차는 양손 무기이다.
					{
						// 야차는 두손 사용하는데 한쪽만. 메시타입은 201 부터
						if(info.m_wVisualID >= 101 && info.m_wVisualID <= 107)
						{							
							info.m_nEquipMeshType = info.m_wVisualID + 100;

							pObject->AttachChildCharRender( eLBP_RightHand, info.m_nEquipCharID, info.m_nEquipMeshType, info.m_nEquipTextureType, nEffectIndex );
						}
						else	// 일반 무기
						{
							// CG_2005/01/27 : 변종아이템기능추가
							if( info.m_nEquipCharID == 966 )
							{
								pObject->AttachChildCharRender( eLBP_LeftHand,  info.m_nEquipCharID, info.m_nEquipMeshType,   info.m_nEquipTextureType, nEffectIndex );
								pObject->AttachChildCharRender( eLBP_RightHand, info.m_nEquipCharID, info.m_nEquipMeshType+1, info.m_nEquipTextureType, nEffectIndex );
							}
							else
							{
								pObject->AttachChildCharRender( eLBP_RightHand, info.m_nEquipCharID, info.m_nEquipMeshType,	  info.m_nEquipTextureType, nEffectIndex );
							}
						}
					}				
					break;
			}

			// 2004_04_19 Changth : 이젠 무기에 스페큘라가 없다. ㅠㅠ, 이젠 뽀대가 없다.
//			pObject->m_ChildChar[ eLBP_RightHand].EnableSpecularEffect();

			// 무기 길이 세팅
			sArrayData* pData = XiahArrayIndex::g_ItemLength.GetData( info.m_wVisualID );
			if( pData )
			{
				int nLength = pData->GetInt(1);
				pObject->m_fWeaponLength = (float)nLength / 10.0f;

				// ㅋㅋ, 야차 무기 '비' 는 손잡이 뒤로 길게 나와 있다.
				if( info.m_wVisualID >= 1700 && info.m_wVisualID < 1800 )
				{
					int nLengthBack = pData->GetInt(2);
					pObject->m_fWeaponBackLength = (float)nLengthBack / 10.0f;
				}
			}
			else
			{
				pObject->m_fWeaponLength = 0.5f;
				pObject->m_fWeaponBackLength = 0.0f; 
			}
		}
	}
	else
	{
		pObject->RemoveChildCharRender( eLBP_RightHand );
		pObject->m_fWeaponLength = 0.5f;
		pObject->m_fWeaponBackLength = 0.0f;

		if( pObject->m_bSubObjType == 4 )
			pObject->RemoveChildCharRender( eLBP_LeftHand );
	}
	
	// CG_2005/01/27 : 변종아이템기능추가
	if( nCharID == pObject->m_CharRender.GetCharID() )
	{
		if( pVisualList[ EQUIPPOS_HAT] != 0)	// 모자
		{
			info.m_wVisualID = pVisualList[ EQUIPPOS_HAT];
			if( XiahItem::SetItemVisualData( &info))
			{
				LogEquipVisual("  [HAT] VID=%d -> CharID=%d Mesh=%d Tex=%d (eLBP_Head)\n",
					info.m_wVisualID, info.m_nEquipCharID, info.m_nEquipMeshType, info.m_nEquipTextureType);
				pObject->AttachChildCharRender( eLBP_Head, info.m_nEquipCharID, info.m_nEquipMeshType, info.m_nEquipTextureType);
			}
		}
		else
		{
			pObject->RemoveChildCharRender( eLBP_Head );

			// 민머리
			switch( nCharID/*pObject->m_bSubObjType*/ )
			{
			case 790:// 검영
				pObject->AttachChildCharRender( eLBP_Head, 755, 8, 0 );
				break;
			case 867:// 연랑
				pObject->AttachChildCharRender( eLBP_Head, 756, 8, 0 );
				break;
			case 891:// 무투
				pObject->AttachChildCharRender( eLBP_Head, 893, 8, 0 );
				break;
			case 906:// 야차
				pObject->AttachChildCharRender( eLBP_Head, 969, 8, 0 );
				break;
			};
		}
	}
	else
	{
		if( pVisualList[ EQUIPPOS_HAT] != 0)	// 모자
		{
			info.m_wVisualID = pVisualList[ EQUIPPOS_HAT]; 
			if( XiahItem::SetItemVisualData( &info ))
			{
				LogEquipVisual("  [HAT-Other] VID=%d -> CharID=%d Mesh=%d Tex=%d (eLBP_Head)\n",
					info.m_wVisualID, info.m_nEquipCharID, info.m_nEquipMeshType, info.m_nEquipTextureType);
				pObject->AttachChildCharRender( eLBP_Head, info.m_nEquipCharID, info.m_nEquipMeshType, info.m_nEquipTextureType);
			}
		}
		else
		{
			pObject->RemoveChildCharRender( eLBP_Head );

			// 민머리
			switch( pObject->m_CharRender.GetCharID() )
			{
			case 790:// 검영
				pObject->AttachChildCharRender( eLBP_Head, 755, 8, 0 );
				break;
			case 867:// 연랑
				pObject->AttachChildCharRender( eLBP_Head, 756, 8, 0 );
				break;
			case 891:// 무투
				pObject->AttachChildCharRender( eLBP_Head, 893, 8, 0 );
				break;
			case 906:// 야차
				pObject->AttachChildCharRender( eLBP_Head, 969, 8, 0 );
				break;
			};
		}
	}

	if( pVisualList[ EQUIPPOS_SHOE] != 0)
	{
		info.m_wVisualID = pVisualList[ EQUIPPOS_SHOE]; // 신발

		if( XiahItem::SetItemVisualData( &info))
		{
			LogEquipVisual("  [SHOE] VID=%d -> CharID=%d Mesh=%d Tex=%d (eLBP_Shoe, 换贴图)\n",
				info.m_wVisualID, info.m_nEquipCharID, info.m_nEquipMeshType, info.m_nEquipTextureType);
			pObject->AttachChildCharRender( eLBP_Shoe, info.m_nEquipCharID, info.m_nEquipMeshType, info.m_nEquipTextureType);
		}
	}
	else
	{
        // 기본 신발.
		switch( pObject->m_bSubObjType )
		{
		case 1:
			pObject->AttachChildCharRender( eLBP_Shoe, 790, 0, 9);
			break;
		case 2:
			pObject->AttachChildCharRender( eLBP_Shoe, 867, 0, 4);
			break;
		case 3:
			pObject->AttachChildCharRender( eLBP_Shoe, 891, 0, 2);
			break;
		case 4:
			pObject->AttachChildCharRender( eLBP_Shoe, 906, 0, 3);
			break;
		};
	}

	pObject->EnableGlowEffect( pObject->m_bGlowEnable);
	LogEquipVisual("  [CLOAK] VID=%d (已实现渲染, EQUIPPOS_CLOAK=7)\n", (int)pVisualList[EQUIPPOS_CLOAK]);
	LogEquipVisual("  [PROTECTOR] VID=%d (未实现渲染, EQUIPPOS_PROTECTOR=4)\n", (int)pVisualList[EQUIPPOS_PROTECTOR]);
	LogEquipVisual("  [RING] VID=%d (无外观, EQUIPPOS_RING=5)\n", (int)pVisualList[EQUIPPOS_RING]);
	LogEquipVisual("  [NECLACE] VID=%d (无外观, EQUIPPOS_NECLACE=6)\n", (int)pVisualList[EQUIPPOS_NECLACE]);
	LogEquipVisual("========================================\n\n");

	int nNewTitleID = (int)pVisualList[8];
	if (g_pMainChar && pObject == (CXiahCharObject*)g_pMainChar->m_pObject)
	{
		g_MainCharInfo.wEquipVisualID[8] = (WORD)nNewTitleID;
	}
	if (nNewTitleID != pObject->m_nActiveTitleID)
	{
		pObject->m_nActiveTitleID = nNewTitleID;
		pObject->LoadLegendTitle(nNewTitleID);

		if (g_pMainChar && pObject == (CXiahCharObject*)g_pMainChar->m_pObject)
		{
			g_MainCharInfo.RefreshChracterInfo();
		}
	}

/*
	// 2004_04_19 Changth
	// 이벤트 아이템의 이펙트를 호출하기 위해서는 두군데에 코딩을 한다.
	// 하나는 장비를 장착할때 횅땍하는 것이고, 또 하나는 ChangeEventInfo_ack가 올때 이다.
	// 이유는, 이펙트의 시작은 ChangeEventInfo_ack에서 알수 있는데, 장비를 장착하고 나서
	// 이 패킷이 오기때문에 정작 장착할때 이벤트가 시작인지 알수가 없다. 다만 다른 캐릭터가
	// 나의 위치로 왔을때 현재 진행중인 이벤트를 횅땍하기 위해서 이렇게 한다.

	// 이벤트 아이템을 현재 사용중이면 이펙트가 나와야 한다.
	if( g_pMainChar && NULL == pObject->m_pEventItemEffectPP )
	{
		std::map<int, sImageScrMsg*>::iterator iit;
		for(iit=g_MainCharInfo.m_pImageScrMsg->m_ScrMsgList.begin(); iit!=g_MainCharInfo.m_pImageScrMsg->m_ScrMsgList.end(); iit++)
		{
			int nEventKind = iit->first;
			sImageScrMsg *pItem = iit->second;

			if( !pItem->bShow ) continue; // 현재 진행중인것만.

			// 현재 장비를 교체하려는 캐릭터가 이벤트 아이템을 사용중인 캐릭터인지 검사한다.
			XiahObject::CXiahObject* pXiahObject = XiahObject::g_XiahObjectManager.FindXiahObject( MAKEOBJECTID(0, pItem->dwEventCharID, OBJTYPE_PC) );
			if( pXiahObject )
			{
				CXiahCharObject* pCharObject = (CXiahCharObject*)pXiahObject->m_pObject;
				if( pCharObject && pCharObject == pObject )
				{
					// 0: 공격계, 1: 성장계, 2: 몬스터계, 3: 경제계
					int nEffectType = 0;

					if( nEventKind >= 0 && nEventKind <= 4 )
						nEffectType = 0;
					else
					if( nEventKind == 5 )
						nEffectType = 1;
					else
					if( nEventKind >= 6 && nEventKind <= 9 )
						nEffectType = 2;
					else
					if( nEventKind >= 10 && nEventKind <= 11 )
						nEffectType = 3;

					// 지속 이펙트
					// 같은 종류의 아이템을 벌써 쓰고 있으면 이펙트가 있는 상태다.
					if( NULL == pCharObject->m_pEventItemEffectPP )
					{
						_EFFECTPACKAGE* pEffectPackage2 = g_EffectManager.EnqOutGongPersistEffectImmediately( eAttackKindItem_keepup + nEffectType );

						if( pEffectPackage2 && pEffectPackage2->pEffectRender && pEffectPackage2->pEffectRender->pPackagePair )
						{
							pEffectPackage2->pEffectRender->pPackagePair->pWorldMatrix = (MATRIX*)pCharObject->m_CharRender.GetCharTM();

							pCharObject->m_pEventItemEffectPP = pEffectPackage2->pEffectRender->pPackagePair;
						}// if
					}

				}// if( pCharObject && pCharObject == pObject )
			}// if( pXiahObject )

		}// for
	}// if
*/
	return TRUE;
}


// 현재는 펫코드형태만
bool SetupPET_VisualEquipement(CXiahCharObject* pObject, WORD* pVisualList)
{
	XiahItem::sItemInfo info;
	// 2-4-1-3 ( 몸통-발-머리-꼬리 )

	if(pVisualList[5] != 0)
	{
		info.m_wVisualID = pVisualList[5];

		if(XiahItem::SetItemVisualData(&info))
			pObject->AttachPetChildChar(eLBP_Protector, info.m_nEquipCharID, info.m_nEquipMeshType, info.m_nEquipTextureType);
	}
	else
	{
		pObject->AttachPetChildChar(eLBP_Protector, 1024, 0, 0);
	}

	/////////////////////////////////////////////////////////////////////////////////////////////////////
	if(pVisualList[4] != 0)
	{
		info.m_wVisualID = pVisualList[4]; // 신발

		if(XiahItem::SetItemVisualData(&info))
			pObject->AttachPetChildChar(eLBP_Shoe, info.m_nEquipCharID, info.m_nEquipMeshType, info.m_nEquipTextureType);

	} // if( pVisualList[ EQUIPPOS_WEAPON] != 0)
	else
	{
		pObject->AttachPetChildChar(eLBP_Shoe, 1024, 0, 0);
	}

	/////////////////////////////////////////////////////////////////////////////////////////////////////
	if(pVisualList[3] != 0)	// 머리
	{
		info.m_wVisualID = pVisualList[3];

		if(XiahItem::SetItemVisualData(&info))
			pObject->AttachPetChildChar(eLBP_Head, info.m_nEquipCharID, info.m_nEquipMeshType, info.m_nEquipTextureType);
	}
	else
	{
		pObject->RemoveChildCharRender(eLBP_Head);
	}

	/////////////////////////////////////////////////////////////////////////////////////////////////////
	//꼬리
	if(pVisualList[1] != 0)
	{
		info.m_wVisualID = pVisualList[1];

		if(XiahItem::SetItemVisualData(&info))
			pObject->AttachPetChildChar(eLBP_LeftHand, info.m_nEquipCharID, info.m_nEquipMeshType, info.m_nEquipTextureType);
	}
	else
	{
		pObject->RemoveChildCharRender(eLBP_LeftHand);
	}

	pObject->EnableGlowEffect(pObject->m_bGlowEnable);

	return true;
}