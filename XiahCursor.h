#pragma once

enum eXiahCursorType
{
	eCT_General = 0,	// 일반 커서
	eCT_Attack_Over = 1,
	eCT_Attack_Clic = 2,
	eCT_PickUp = 3,
	eCT_Menu = 4,
	eCT_DoorOpen = 5, 
	eCT_RightTurn = 6,
	eCT_LeftTurn = 7,
	eCT_Repair = 8,
	eCT_RepairNo = 9,

	eCT_Count
};

extern eXiahCursorType g_CursorType;
extern BOOL InitXiahCursor();
extern BOOL ReleaseXiahCursor();
extern BOOL ChangeXiahCursor(eXiahCursorType cursor_type);
extern BOOL SetXiahCursor();

