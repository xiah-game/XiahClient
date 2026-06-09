//---------------------------------------------------------------------------------------
BOOL ProcessCursor()
{
	// PET AI 명령에 따른 커서의 변화
	if(g_bCommandAI)
	{
		switch(g_dwCommandType)
		{
			case PETAI_NONE	:
			case PETAI_AUTOATTACK :
			case PETAI_TAKEITEM :
			case PETAI_SPECIALATTACK :
			break;
			
			// 대상공격
			case PETAI_TARGETATTACK	:
			{
				ChangeXiahCursor( eCT_Attack_Over);
				return TRUE;
			}
			break;
		}
	}

	if( !g_pUIManager->IsPopMenu() && pMouseOnCharObject)
	{
		switch( bMouseOnObjectType)
		{
		case OBJTYPE_PC:
			{

			}
			break;
		case OBJTYPE_NPC:
			{
				XiahItem::sItemInfo* pInfo = g_MainCharInfo.m_pHoldItem->GetHoldItemItem();
				if( pInfo && pInfo->m_bItemType == ITEMTYPE_NPCITEM)
				{
					if( g_CursorType != eCT_Menu)
						ChangeXiahCursor( eCT_Menu);
				}
				else
				{
					// 야차가 귀식 대법을 썼을땐 안바뀐다.
					if( MAIN_CHAROBJECT->m_bSubObjType == 4 &&
						MAIN_CHAROBJECT->m_KeepUpMugongList.IsExist(OUTGONGID_GYUISIKDAEBUB) )
					{
					}
					else if( g_CursorType != eCT_Attack_Over)
					{
						ChangeXiahCursor( eCT_Attack_Over);
					}
				}
			}
			break;
		case OBJTYPE_PET:
			{
				XiahItem::sItemInfo* pInfo = g_MainCharInfo.m_pHoldItem->GetHoldItemItem();
				if( pInfo && pInfo->m_bItemType == ITEMTYPE_NPCITEM)
				{
					if( g_CursorType != eCT_Menu)
						ChangeXiahCursor( eCT_Menu);
				}
			}
			break;
		case OBJTYPE_ITEM:
			if( g_CursorType != eCT_PickUp)
				ChangeXiahCursor( eCT_PickUp);
			break;
		case OBJTYPE_FUNCTIONALNPC:
			//if( !g_pUIManager->IsPopMenu() && g_CursorType != eCT_Menu)
				//ChangeXiahCursor( eCT_Menu);
			break;
		}
	}
	else if( g_CursorType != eCT_General)
		ChangeXiahCursor( eCT_General);
	
	return TRUE;
}