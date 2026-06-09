#include "precompile.h"
#include "XiahCursor.h"
#include "CharacterInfo.h"

eXiahCursorType g_CursorType;

LPCTSTR g_strCursorFileList[ eCT_Count] =
{
	_T("cursor\\general.ani"),
	_T("cursor\\attack_over.ani"),
	_T("cursor\\attack_click.ani"),
	_T("cursor\\pickup.ani"),
	_T("cursor\\menu.ani"),
	_T("cursor\\dooropen.ani"),
	_T("cursor\\rightturn.ani"),
	_T("cursor\\leftturn.ani"),
	_T("cursor\\repair.ani"),
	_T("cursor\\not_repair.ani"),
};

HCURSOR g_hCursorList[ eCT_Count];

BOOL InitXiahCursor()
{
	for(int i = 0; i < eCT_Count; i++)
	{
		g_hCursorList[ i] = LoadCursorFromFile( g_strCursorFileList[ i]);
		
		if( g_hCursorList[ i] == NULL)
			return FALSE;
	}

	return TRUE;
}

BOOL ReleaseXiahCursor()
{
	SetCursor( NULL);

	for(int i = 0; i < eCT_Count; i++)
	{
		DestroyCursor( g_hCursorList[ i]);
	}

	return TRUE;
}

BOOL ChangeXiahCursor(eXiahCursorType cursor_type)
{
	// [3/19/2004]
	if( g_CursorType == eCT_Repair || cursor_type == eCT_Repair)
		g_MainCharInfo.RefreshItemFrame();

	if(cursor_type != eCT_Repair)
	{
		g_MainCharInfo.m_bReairItemUse = false;
		g_MainCharInfo.m_bReairItemUse2 = false;
	}

	g_CursorType = cursor_type;

	return SetXiahCursor();
}

BOOL SetXiahCursor()
{
	if( g_hCursorList[ g_CursorType] != NULL)
		::SetCursor( g_hCursorList[ g_CursorType]);

	return TRUE;
}

