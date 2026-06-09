#include "CurseFilter.h"

extern LPCTSTR GetMapName(DWORD dwMapID);

int OnCS_CH_CHAT_ACK( CMsg &msg)
{
	DWORD sender	= 0;
	DWORD listner	= 0;
	BYTE type		= 0;
	sString content			= _T("");
	sString senderName		= _T("");	
	sString listnerName		= _T("");

	msg
		>> sender
		>> type;

	// CG_1217 : 문파전시 한시간뒤에 비석 때릴수있다는 메세지출력
	if( type != CT_BATTLE && type != CT_TIMEMESSAGE )
	{
		msg
			>> content
			>> senderName;
	}

	switch(type)
	{
	case CT_WHISPER:
		{
			msg
				>> listner
				>> listnerName;
		}
		break;

		// CG_2005/01/19 : time message
	case CT_TIMEMESSAGE:
		{
			DWORD dwMsgType = 0;

			msg
				>> dwMsgType;

			TCHAR strTemp[ 256 ] = { 0, };

			// [3/14/2005] 대련장 메시지추가 3,4
			switch( dwMsgType )
			{
				case 1:	_stprintf( strTemp, IDS_DUNJEON_START );		break;
				case 2:	_stprintf( strTemp, IDS_DUNJEON_END );			break;
				case 3:	_stprintf( strTemp, IDS_MUNPAMAP_START );		break;
				case 4:	_stprintf( strTemp, IDS_MUNPAMAP_END );			break;
			}

			g_MainCharInfo.SpecialChatMessage(strTemp, 0);

			return 0;
		}
		break;

	case CT_SAYITEM_CELL:
	case CT_SAYITEM_MAP:
	case CT_SAYITEM_CHANNEL:
	case 15:	// 황금각적
		{
			DWORD dwMapID = -1;
			WORD wPosX = 0;
			WORD wPosY = 0;

			msg
				>> dwMapID
				>> wPosX
				>> wPosY;

			LPCTSTR lpStrTemp = GetMapName(dwMapID);

			TCHAR szContent[128] = {0,};

			if(type == 15)
			{
				_stprintf( szContent, IDS_GAK_INFO_GOLD, (LPCTSTR)senderName, (LPCTSTR)content, sender, lpStrTemp, wPosX/4, wPosY/4);
				g_MainCharInfo.SpecialChatMessage(szContent, 5);
			}
			else
			{
				_stprintf( szContent, _T("%s : %s [ %s %d, %d ]"), (LPCTSTR)senderName, (LPCTSTR)content, lpStrTemp, wPosX/4, wPosY/4);
				g_MainCharInfo.SpecialChatMessage(szContent, type-7);
			}			

			return 0;
		}
		break;
	case CT_EVENTMSG: // 이벤트 메시지
		{
			BYTE bEventKind = 0;
			BYTE bCanUser	= 0;
			DWORD dwMapID	= 0;			
			DWORD dwUserType = 0;
			sString strMunpaName;

			msg
                >> dwMapID
				>> bEventKind
				>> bCanUser
				>> dwUserType
				>> strMunpaName;

			TCHAR szItemName[64] = {0,};
			TCHAR szSpMsg[128] = {0,};

			LPCTSTR lpStrTemp = GetMapName(dwMapID);

			switch(bEventKind)
			{
			case 0:
				_stprintf( szSpMsg, IDS_E_ATTK_INC_S, lpStrTemp, 20);
				_stprintf(szItemName, IDS_E_ITEM_1);
				break;
			case 1:
				_stprintf( szSpMsg, IDS_E_DEF_INC_S,lpStrTemp,  20);
				_stprintf(szItemName, IDS_E_ITEM_2);
				break;
			case 2:
				_stprintf( szSpMsg, IDS_E_AGI_INC_S,lpStrTemp,  20);
				_stprintf(szItemName, IDS_E_ITEM_3);
				break;
			case 3:
				_stprintf( szSpMsg, IDS_E_LIFE_INC_S,lpStrTemp,  20);
				_stprintf(szItemName, IDS_E_ITEM_4);
				break;
			case 4:
				_stprintf( szSpMsg, IDS_E_CRITICAL_INC_S,lpStrTemp,  20);
				_stprintf(szItemName, IDS_E_ITEM_5);
				break;
			case 5:
				_stprintf( szSpMsg, IDS_E_DAN_EXP_INC_S,lpStrTemp,  20);
				_stprintf(szItemName, IDS_E_ITEM_6);
				break;
			case 6:
				_stprintf( szSpMsg, IDS_E_MON_ATTK_DIM_S,lpStrTemp,  20);
				_stprintf(szItemName, IDS_E_ITEM_7);
				break;
			case 7:
				_stprintf( szSpMsg, IDS_E_MON_DEF_DIM_S, lpStrTemp, 20);
				_stprintf(szItemName, IDS_E_ITEM_8);
				break;
			case 8:
				_stprintf( szSpMsg, IDS_E_MON_AGI_DIM_S, lpStrTemp, 20);
				_stprintf(szItemName, IDS_E_ITEM_9);
				break;
			case 9:
				_stprintf( szSpMsg, IDS_E_MON_LIFE_DIM_S, lpStrTemp, 20);
				_stprintf(szItemName, IDS_E_ITEM_10);
				break;
			case 10:
				_stprintf(szSpMsg, IDS_E_PT_FEE_S, lpStrTemp);
				_stprintf(szItemName, IDS_E_ITEM_11);
				break;
			case 11:
				_stprintf( szSpMsg, IDS_E_NT_DC_S, lpStrTemp, 50);
				_stprintf(szItemName, IDS_E_ITEM_12);
				break;
			default:
				break;
			}

			switch(bCanUser)
			{
			case 1:// 1 : 검영, 2 : 연랑, 3 : 무투, 4 : 야차
				{
					LPCTSTR lpStrTempChar;

					switch(dwUserType)
					{
						case 1:	lpStrTempChar = IDS_GUMYONG;		break;
						case 2:	lpStrTempChar = IDS_YUNRANG;		break;
						case 3:	lpStrTempChar = IDS_MUTU;			break;
						case 4:	lpStrTempChar = IDS_YACHA;			break;
					}

					switch(bEventKind)
					{
					case 0:
						_stprintf( szSpMsg, IDS_E_ATTK_INC_S_CALSS, lpStrTemp, lpStrTempChar, 20);
						break;
					case 1:
						_stprintf( szSpMsg, IDS_E_DEF_INC_S_CALSS, lpStrTemp, lpStrTempChar, 20);
						break;
					case 2:
						_stprintf( szSpMsg, IDS_E_AGI_INC_S_CALSS, lpStrTemp, lpStrTempChar, 20);
						break;
					case 3:
						_stprintf( szSpMsg, IDS_E_LIFE_INC_S_CALSS, lpStrTemp, lpStrTempChar, 20);
						break;
					case 4:
						_stprintf( szSpMsg, IDS_E_CRITICAL_INC_S_CALSS, lpStrTemp, lpStrTempChar, 20);
						break;
					}
				}
				break;
			case 2:
				{
					switch(bEventKind)
					{
					case 0:
						_stprintf( szSpMsg, IDS_E_ATTK_INC_S_CLAN, lpStrTemp, strMunpaName.data(), 20);
						break;
					case 1:
						_stprintf( szSpMsg, IDS_E_DEF_INC_S_CLAN, lpStrTemp, strMunpaName.data(), 20);
						break;
					case 2:
						_stprintf( szSpMsg, IDS_E_AGI_INC_S_CLAN, lpStrTemp, strMunpaName.data(), 20);
						break;
					case 3:
						_stprintf( szSpMsg, IDS_E_LIFE_INC_S_CLAN, lpStrTemp, strMunpaName.data(), 20);
						break;
					case 4:
						_stprintf( szSpMsg, IDS_E_CRITICAL_INC_S_CLAN, lpStrTemp, strMunpaName.data(), 20);
						break;
					}
				}
				break;
			}

			TCHAR szContent[100];
			_stprintf( szContent, IDS_ITEM_SOCKET_USER, (LPCTSTR)senderName, lpStrTemp, szItemName);

			g_MainCharInfo.SpecialChatMessage(szContent, 6);
			g_MainCharInfo.SpecialChatMessage(szSpMsg, 6);

			return 0;
		}
		break;

	// 일반 챗
	/*
	case CT_NORMAL:
		{
			
		}
		break;
	*/

	// 이벤트용 아이템을 획득했을때. .. 마우스 등 경품.

	//HO_0706_07 얼음 이벤트 : 이벤트관련 아이템 획득시 전체 공지 뜨는게 랙발생을 유도 한다고 해서 주석 처리함 개인 메세지로 바꾸자..
	//이벤트 아이템 획득시 공지 출력은 이번 이벤트 횅땍후 앞으로의 이벤트 상황에 따라.... 변화한다는...
	case CT_GETEVENTITEM:
		{
			TCHAR szContent[150] = {0,};
		//	_stprintf( szContent, IDS_EVENTITEM_OBTAIN, (LPCTSTR)senderName, (LPCTSTR)content ); //HO_0706_07 기존에는 스페셜 메세지 사용
		//	g_MainCharInfo.SpecialChatMessage(szContent, 6);

			_stprintf( szContent, IDS_D_OBTAIN_ITEM,(LPCTSTR)content );
			g_MainCharInfo.ShowHelpMessage(szContent, TEXTEFFECT_COLOR_GAIN);

			// 이때 사운드도 있다.
			g_MainCharInfo.PlayInterfaceSound( EVENT_PREMIUMITEM_SOUND_CONGRATULATION );

			return 0;
		}
	
		break;
	}

	// [4/19/2005] 공지사항 제외
	if(CT_BROADCAST != type)
	{
		// [3/14/2005] 전 메시지 필터링
		// 욕설 방지
		TCHAR strTemp[100] = {0,};		
		memcpy(strTemp, content.data(), strlen(content.data()));
		content.printf("%s", ConvertString(strTemp, 100));
	}	

	g_MainCharInfo.RefreshChatFrame( sender, type, content, senderName, listner, listnerName);

	return 0;
}

int OnCS_CH_SYSTEMMESSAGE_ACK( CMsg &msg)
{
	BYTE bResult;

	msg
		>> bResult;

	switch( bResult)
	{
	case 1://스헥
	case 2://패킷조작
	case 3://잘못된패킷
		g_pUIManager->ShowNotice(CH_WARNNIG1, NOTICE_FRAME_OK, NOTICE_FRAME_UNEXPECTED_TERMINATE);
		break;

	case 4://점검
		g_pUIManager->ShowNotice(CH_WARNNIG2, NOTICE_FRAME_OKCANCEL);
		break;
	case 9:// TOO BUSY
		g_pUIManager->ShowNotice(CH_WARNNIG3, NOTICE_FRAME_OKCANCEL);
		break;
	}

	return 0;
}