#ifndef __XIAH_CHEAT_CONFIG_H__
#define __XIAH_CHEAT_CONFIG_H__

#include <windows.h>

extern BOOL g_bAutoAttack;
extern BOOL g_bAutoLoot;
extern int  g_nAttackMode;  // 0=Physical, 1=Skill
extern BOOL g_bAutoHP;
extern int  g_nHPPercent;
extern TCHAR g_szHPPotionName[32];
extern BOOL g_bAutoBuyHP;

extern BOOL g_bAutoMP;
extern int  g_nMPPercent;
extern TCHAR g_szMPPotionName[32];
extern BOOL g_bAutoBuyMP;

extern BOOL g_bAutoPet;
extern int  g_nPetWildRate;
extern DWORD g_dwPetFoodID;
extern DWORD g_dwPetSealID;
extern int  g_nBuffInterval;

// Item filter text
extern TCHAR g_szFilterList[4096];

// Skill Settings
extern DWORD g_dwAttackSkillID;
extern int g_nAttackSkillInterval;

extern DWORD g_dwBuffSkillID1;
extern int g_nBuffSkillInterval1;

extern DWORD g_dwBuffSkillID2;
extern int g_nBuffSkillInterval2;

extern DWORD g_dwTeammateSkillID1;
extern int g_nTeammateSkillInterval1;

extern DWORD g_dwTeammateSkillID2;
extern int g_nTeammateSkillInterval2;

// Last cast times
extern DWORD g_dwLastAttackSkillTime;
extern DWORD g_dwLastBuffSkillTime1;
extern DWORD g_dwLastBuffSkillTime2;
extern DWORD g_dwLastTeammateSkillTime1;
extern DWORD g_dwLastTeammateSkillTime2;
extern BOOL g_bIsAutoCasting;

// Bank Settings
extern BOOL g_bAutoStoreBank;
extern TCHAR g_szStoreBankList[4096];
BOOL IsStoreBankFiltered(const TCHAR* szItemName);

// Sell Settings
extern BOOL g_bAutoSellFull;
extern BOOL g_bAutoSellAll;
extern BOOL g_bBanSellFilter;
extern TCHAR g_szBanSellList[4096];
BOOL IsBanSellFiltered(const TCHAR* szItemName);
void TriggerImmediateBagScan();

// 挂机中心点 & 空闲回归 & 定点攻击 & 活动半径
extern BOOL  g_bUseHomePoint;      // 是否启用中心点回归
extern WORD  g_wHomeX, g_wHomeY;   // 中心点坐标（开始挂机时自动记录，或手动设定）
extern int   g_nIdleReturnSec;     // 空闲N秒后回中心点（0=禁用）
extern int   g_nHomeRadius;        // 挂机活动半径（码，默认25），超范围打死怪后立即返回中心点
extern BOOL  g_bFixedPointAttack;  // 定点攻击（站原地不动攻击，方便群攻）
extern DWORD g_dwLastAttackTime;   // 运行时：上次攻击/拾取的时间戳
extern BOOL  g_bReturningHome;     // 运行时：正在回中心点的途中

// Forward declaration of XiahItem::sItemInfo
namespace XiahItem {
    struct sItemInfo;
}

XiahItem::sItemInfo* FindSackItemByName(const TCHAR* szName);
int CountSackItemByName(const TCHAR* szName);

void LoadCheatConfig();
void SaveCheatConfig();
void ShowCheatConfigWindow(HWND hwndParent);
BOOL IsItemFiltered(const TCHAR* szItemName);
void ApplyCheatConfigToMainChar();
DWORD GetSkillCD(DWORD dwMugongID);

// 视线阻挡检测与防卡墙寻路接口
bool IsMapCellWalkable(int x, int y);
bool CheckMapLineOfSight(int x0, int y0, int x1, int y1);
bool FindNextWaypoint(int startX, int startY, int goalX, int goalY, int& outWpX, int& outWpY, int maxSearchRadius = 35);
bool FindDetourWaypoint(int curX, int curY, int targetX, int targetY, int& outSideX, int& outSideY, int nStepMode = 1);
void AddStuckMonster(DWORD dwObjectID, DWORD dwDurationMs = 25000);
bool IsMonsterStuck(DWORD dwObjectID);

#endif // __XIAH_CHEAT_CONFIG_H__
