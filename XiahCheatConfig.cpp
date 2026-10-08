#include "precompile.h"
#include "XiahCheatConfig.h"
#include <tchar.h>
#include <stdio.h>
#include <commctrl.h>
#include "XiahArrayIndex.h"
#include "Mugong.h"
#include "CharacterInfo.h"
#include "XiahGameObject.h"
#include "XiahObject.h"
#include "XiahGame_Handler_Sender.h"

#pragma comment(lib, "comctl32.lib")

// Link common controls v6 for modern visual styles
#pragma comment(linker,"\"/manifestdependency:type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")

#ifndef GB2132_CHARSET
#define GB2132_CHARSET 134
#endif
#ifndef CLEARTYPE_QUALITY
#define CLEARTYPE_QUALITY 5
#endif

// Global variables with default values
BOOL g_bAutoAttack = TRUE;
BOOL g_bAutoLoot = TRUE;
int  g_nAttackMode = 0;  // 0=Physical, 1=Skill

BOOL g_bAutoHP = TRUE;
int  g_nHPPercent = 50;
TCHAR g_szHPPotionName[32] = _T("\xc8\xab\xb4\xb4\xd2\xa9\x28\xd0\xa1\x29");
BOOL g_bAutoBuyHP = FALSE;

BOOL g_bAutoMP = TRUE;
int  g_nMPPercent = 10;
TCHAR g_szMPPotionName[32] = _T("\xc4\xfd\xc6\xf8\xb5\xa4\x28\xb4\xf3\x29");
BOOL g_bAutoBuyMP = FALSE;

BOOL g_bAutoPet = TRUE;
int  g_nPetWildRate = 50;
DWORD g_dwPetFoodID = 9102;
DWORD g_dwPetSealID = 9210;
int  g_nBuffInterval = 10;

TCHAR g_szFilterList[4096] = _T("");

// Skill Settings
DWORD g_dwAttackSkillID = 0;
int g_nAttackSkillInterval = 5;

DWORD g_dwBuffSkillID1 = 0;
int g_nBuffSkillInterval1 = 30;

DWORD g_dwBuffSkillID2 = 0;
int g_nBuffSkillInterval2 = 30;

DWORD g_dwTeammateSkillID1 = 0;
int g_nTeammateSkillInterval1 = 10;

DWORD g_dwTeammateSkillID2 = 0;
int g_nTeammateSkillInterval2 = 10;

// Last cast times
DWORD g_dwLastAttackSkillTime = 0;
DWORD g_dwLastBuffSkillTime1 = 0;
DWORD g_dwLastBuffSkillTime2 = 0;
DWORD g_dwLastTeammateSkillTime1 = 0;
DWORD g_dwLastTeammateSkillTime2 = 0;
BOOL g_bIsAutoCasting = FALSE;

// Bank settings
BOOL g_bAutoStoreBank = TRUE;
TCHAR g_szStoreBankList[4096] = _T("");

// Sell settings
BOOL g_bAutoSellFull = TRUE;
BOOL g_bAutoSellAll = FALSE;
BOOL g_bBanSellFilter = TRUE;
TCHAR g_szBanSellList[4096] = _T("\xbd\xf0\xb4\xb4\xd2\xa9\r\n\xd0\xa1\xbb\xb9\xb5\xa4\r\n\xb4\xf3\xbb\xb9\xb5\xa4\r\n\xc4\xfd\xc6\xf8\xb5\xa4\r\n\xd4\xb6\xc9\xed\xb7\xfb\r\n\xbb\xd8\xb3\xc7\xb7\xfb");

// 挂机中心点 & 空闲回归 & 定点攻击 & 活动半径
BOOL  g_bUseHomePoint   = TRUE;
WORD  g_wHomeX = 0, g_wHomeY = 0;
int   g_nIdleReturnSec  = 15;
int   g_nHomeRadius     = 25;
BOOL  g_bFixedPointAttack = FALSE;
DWORD g_dwLastAttackTime = 0;
BOOL  g_bReturningHome  = FALSE;

// Control tracking arrays
static HWND g_hTab0Controls[70];
static int g_nTab0Count = 0;

static HWND g_hTab1Controls[60];
static int g_nTab1Count = 0;

static HWND g_hTab2Controls[40];
static int g_nTab2Count = 0;

static void ShowTabControls(int iTab) {
    for (int i = 0; i < g_nTab0Count; ++i) ShowWindow(g_hTab0Controls[i], iTab == 0 ? SW_SHOW : SW_HIDE);
    for (int i = 0; i < g_nTab1Count; ++i) ShowWindow(g_hTab1Controls[i], iTab == 1 ? SW_SHOW : SW_HIDE);
    for (int i = 0; i < g_nTab2Count; ++i) ShowWindow(g_hTab2Controls[i], iTab == 2 ? SW_SHOW : SW_HIDE);
}

void PopulateSkillComboBox(HWND hCombo, DWORD dwSelectedID) {
    SendMessage(hCombo, CB_RESETCONTENT, 0, 0);
    int idx = SendMessage(hCombo, CB_ADDSTRING, 0, (LPARAM)_T("\xce\xde"));
    SendMessage(hCombo, CB_SETITEMDATA, idx, 0);
    
    int selectedIdx = 0;
    
    if (g_MainCharInfo.m_pMugong) {
        for (MugongMap::iterator it = g_MainCharInfo.m_pMugong->m_mapMugong.begin(); 
             it != g_MainCharInfo.m_pMugong->m_mapMugong.end(); ++it) {
            DWORD dwID = it->first;
            sArrayData* pMugongData = XiahArrayIndex::g_MugongTemplate.GetData(dwID);
            if (pMugongData) {
                sString szName = pMugongData->GetString(1);
                TCHAR szLabel[64];
                _sntprintf(szLabel, 64, _T("%s (ID: %d)"), (LPCTSTR)szName, dwID);
                int curIdx = SendMessage(hCombo, CB_ADDSTRING, 0, (LPARAM)szLabel);
                SendMessage(hCombo, CB_SETITEMDATA, curIdx, dwID);
                if (dwID == dwSelectedID) {
                    selectedIdx = curIdx;
                }
            }
        }
        for (MugongMap::iterator it = g_MainCharInfo.m_pMugong->m_map2ThRebirthMugong.begin(); 
             it != g_MainCharInfo.m_pMugong->m_map2ThRebirthMugong.end(); ++it) {
            DWORD dwID = it->first;
            sArrayData* pMugongData = XiahArrayIndex::g_MugongTemplate.GetData(dwID);
            if (pMugongData) {
                sString szName = pMugongData->GetString(1);
                TCHAR szLabel[64];
                _sntprintf(szLabel, 64, _T("%s (ID: %d)"), (LPCTSTR)szName, dwID);
                int curIdx = SendMessage(hCombo, CB_ADDSTRING, 0, (LPARAM)szLabel);
                SendMessage(hCombo, CB_SETITEMDATA, curIdx, dwID);
                if (dwID == dwSelectedID) {
                    selectedIdx = curIdx;
                }
            }
        }
    }
    SendMessage(hCombo, CB_SETCURSEL, selectedIdx, 0);
}

// Handle to config window thread
static HANDLE g_hConfigThread = NULL;
static HWND g_hwndConfig = NULL;

// Control IDs
enum {
    ID_TABCONTROL = 1000,
    ID_CHECK_AUTOATTACK = 1001,
    ID_CHECK_AUTOLOOT,
    ID_RADIO_PHYSICAL,
    ID_RADIO_SKILL,
    ID_CHECK_AUTOHP,
    ID_EDIT_HPPERCENT,
    ID_EDIT_HPPOTIONNAME,
    ID_CHECK_AUTOBUYHP,
    ID_CHECK_AUTOMP,
    ID_EDIT_MPPERCENT,
    ID_EDIT_MPPOTIONNAME,
    ID_CHECK_AUTOBUYMP,
    ID_CHECK_AUTOPET,
    ID_EDIT_PETWILDRATE,
    ID_EDIT_PETFOODID,
    ID_EDIT_PETSEALID,
    ID_EDIT_FILTERLIST,
    
    // New Skill tab control IDs
    ID_COMBO_ATTACKSKILL,
    ID_EDIT_ATTACKINTERVAL,
    ID_COMBO_BUFFSKILL1,
    ID_EDIT_BUFFINTERVAL1,
    ID_COMBO_BUFFSKILL2,
    ID_EDIT_BUFFINTERVAL2,
    ID_COMBO_TEAMMATESKILL1,
    ID_EDIT_TEAMMATEINTERVAL1,
    ID_COMBO_TEAMMATESKILL2,
    ID_EDIT_TEAMMATEINTERVAL2,
    
    // Bank & Sell tab control IDs
    ID_CHECK_AUTOSTORE_BANK,
    ID_EDIT_STOREBANK_LIST,
    ID_CHECK_AUTOSELL_FULL,
    ID_CHECK_AUTOSELL_ALL,
    ID_CHECK_BANSELL_FILTER,
    ID_EDIT_BANSELL_LIST,
    
    ID_BUTTON_TOGGLE,
    ID_BUTTON_SAVE,
    ID_BUTTON_CANCEL,
    
    // Home point control IDs
    ID_CHECK_USEHOME,
    ID_EDIT_HOMEX,
    ID_EDIT_HOMEY,
    ID_BUTTON_GETCURPOS,
    ID_CHECK_FIXEDPOINT,
    ID_EDIT_IDLERETURN,
    ID_EDIT_HOMERADIUS
};

// 获取配置文件路径：支持多号独立配置（cheat_<角色名>.ini），未进入游戏或无角色名时回退为 cheat.ini
static void GetIniPath(TCHAR* szPath, DWORD dwSize) {
    TCHAR szExePath[MAX_PATH];
    GetModuleFileName(NULL, szExePath, MAX_PATH);
    TCHAR* pLastSlash = _tcsrchr(szExePath, _T('\\'));
    if (pLastSlash) {
        *(pLastSlash + 1) = _T('\0');
    }
    
    LPCTSTR pszCharName = (LPCTSTR)g_MainCharInfo.m_szNickName;
    if (pszCharName && _tcslen(pszCharName) > 0) {
        TCHAR szSafeName[64] = {0};
        int dst = 0;
        for (int i = 0; pszCharName[i] != _T('\0') && dst < 60; ++i) {
            TCHAR c = pszCharName[i];
            if (c != _T('\\') && c != _T('/') && c != _T(':') && c != _T('*') && 
                c != _T('?') && c != _T('\"') && c != _T('<') && c != _T('>') && c != _T('|')) {
                szSafeName[dst++] = c;
            }
        }
        szSafeName[dst] = _T('\0');
        if (dst > 0) {
            _sntprintf(szPath, dwSize, _T("%scheat_%s.ini"), szExePath, szSafeName);
            return;
        }
    }
    _sntprintf(szPath, dwSize, _T("%scheat.ini"), szExePath);
}

void LoadCheatConfig() {
    TCHAR szIniFile[MAX_PATH];
    GetIniPath(szIniFile, MAX_PATH);
    
    // 如果专属角色配置尚不存在，但默认 cheat.ini 存在，优先以 cheat.ini 作为模板读取
    if (GetFileAttributes(szIniFile) == INVALID_FILE_ATTRIBUTES) {
        TCHAR szDefaultIni[MAX_PATH];
        TCHAR szExePath[MAX_PATH];
        GetModuleFileName(NULL, szExePath, MAX_PATH);
        TCHAR* pLastSlash = _tcsrchr(szExePath, _T('\\'));
        if (pLastSlash) {
            *(pLastSlash + 1) = _T('\0');
        }
        _sntprintf(szDefaultIni, MAX_PATH, _T("%scheat.ini"), szExePath);
        if (GetFileAttributes(szDefaultIni) != INVALID_FILE_ATTRIBUTES) {
            _tcscpy_s(szIniFile, MAX_PATH, szDefaultIni);
        }
    }
    
    g_bAutoAttack = GetPrivateProfileInt(_T("CHEAT"), _T("AutoAttack"), 1, szIniFile);
    g_bAutoLoot = GetPrivateProfileInt(_T("CHEAT"), _T("AutoLoot"), 1, szIniFile);
    g_nAttackMode = GetPrivateProfileInt(_T("CHEAT"), _T("AttackMode"), 0, szIniFile);
    
    g_bAutoHP = GetPrivateProfileInt(_T("CHEAT"), _T("AutoHP"), 1, szIniFile);
    g_nHPPercent = GetPrivateProfileInt(_T("CHEAT"), _T("HPPercent"), 50, szIniFile);
    GetPrivateProfileString(_T("CHEAT"), _T("HPPotionName"), _T("\xc8\xab\xb4\xb4\xd2\xa9\x28\xd0\xa1\x29"), g_szHPPotionName, 32, szIniFile);
    g_bAutoBuyHP = GetPrivateProfileInt(_T("CHEAT"), _T("AutoBuyHP"), 0, szIniFile);
    
    g_bAutoMP = GetPrivateProfileInt(_T("CHEAT"), _T("AutoMP"), 1, szIniFile);
    g_nMPPercent = GetPrivateProfileInt(_T("CHEAT"), _T("MPPercent"), 10, szIniFile);
    GetPrivateProfileString(_T("CHEAT"), _T("MPPotionName"), _T("\xc4\xfd\xc6\xf8\xb5\xa4\x28\xb4\xf3\x29"), g_szMPPotionName, 32, szIniFile);
    g_bAutoBuyMP = GetPrivateProfileInt(_T("CHEAT"), _T("AutoBuyMP"), 0, szIniFile);
    
    g_bAutoPet = GetPrivateProfileInt(_T("CHEAT"), _T("AutoPet"), 1, szIniFile);
    g_nPetWildRate = GetPrivateProfileInt(_T("CHEAT"), _T("PetWildRate"), 50, szIniFile);
    g_dwPetFoodID = GetPrivateProfileInt(_T("CHEAT"), _T("PetFoodID"), 9102, szIniFile);
    g_dwPetSealID = GetPrivateProfileInt(_T("CHEAT"), _T("PetSealID"), 9210, szIniFile);
    g_nBuffInterval = GetPrivateProfileInt(_T("CHEAT"), _T("BuffInterval"), 10, szIniFile);
    
    // Load filter list
    TCHAR szRawFilter[4096] = {0};
    GetPrivateProfileString(_T("FILTER"), _T("List"), _T(""), szRawFilter, 4096, szIniFile);
    int dst = 0;
    for (int src = 0; szRawFilter[src] != _T('\0') && dst < 4094; ++src) {
        if (szRawFilter[src] == _T('|')) {
            g_szFilterList[dst++] = _T('\r');
            g_szFilterList[dst++] = _T('\n');
        } else {
            g_szFilterList[dst++] = szRawFilter[src];
        }
    }
    g_szFilterList[dst] = _T('\0');

    // Load Skill Configs
    g_dwAttackSkillID = GetPrivateProfileInt(_T("SKILL"), _T("AttackSkillID"), 0, szIniFile);
    g_nAttackSkillInterval = GetPrivateProfileInt(_T("SKILL"), _T("AttackSkillInterval"), 5, szIniFile);
    if (g_nAttackSkillInterval < 1) g_nAttackSkillInterval = 1;

    g_dwBuffSkillID1 = GetPrivateProfileInt(_T("SKILL"), _T("BuffSkillID1"), 0, szIniFile);
    g_nBuffSkillInterval1 = GetPrivateProfileInt(_T("SKILL"), _T("BuffSkillInterval1"), 30, szIniFile);
    if (g_nBuffSkillInterval1 < 1) g_nBuffSkillInterval1 = 1;

    g_dwBuffSkillID2 = GetPrivateProfileInt(_T("SKILL"), _T("BuffSkillID2"), 0, szIniFile);
    g_nBuffSkillInterval2 = GetPrivateProfileInt(_T("SKILL"), _T("BuffSkillInterval2"), 30, szIniFile);
    if (g_nBuffSkillInterval2 < 1) g_nBuffSkillInterval2 = 1;

    g_dwTeammateSkillID1 = GetPrivateProfileInt(_T("SKILL"), _T("TeammateSkillID1"), 0, szIniFile);
    g_nTeammateSkillInterval1 = GetPrivateProfileInt(_T("SKILL"), _T("TeammateSkillInterval1"), 10, szIniFile);
    if (g_nTeammateSkillInterval1 < 1) g_nTeammateSkillInterval1 = 1;

    g_dwTeammateSkillID2 = GetPrivateProfileInt(_T("SKILL"), _T("TeammateSkillID2"), 0, szIniFile);
    g_nTeammateSkillInterval2 = GetPrivateProfileInt(_T("SKILL"), _T("TeammateSkillInterval2"), 10, szIniFile);
    if (g_nTeammateSkillInterval2 < 1) g_nTeammateSkillInterval2 = 1;

    // Load Bank Configs
    g_bAutoStoreBank = GetPrivateProfileInt(_T("BANK"), _T("AutoStoreBank"), 1, szIniFile);
    TCHAR szRawStoreBank[4096] = {0};
    GetPrivateProfileString(_T("FILTER"), _T("StoreBankList"), _T(""), szRawStoreBank, 4096, szIniFile);
    int dstS = 0;
    for (int src = 0; szRawStoreBank[src] != _T('\0') && dstS < 4094; ++src) {
        if (szRawStoreBank[src] == _T('|')) {
            g_szStoreBankList[dstS++] = _T('\r');
            g_szStoreBankList[dstS++] = _T('\n');
        } else {
            g_szStoreBankList[dstS++] = szRawStoreBank[src];
        }
    }
    g_szStoreBankList[dstS] = _T('\0');

    // Load Sell Configs
    g_bAutoSellFull = GetPrivateProfileInt(_T("SELL"), _T("AutoSellFull"), 1, szIniFile);
    g_bAutoSellAll = GetPrivateProfileInt(_T("SELL"), _T("AutoSellAll"), 0, szIniFile);
    g_bBanSellFilter = GetPrivateProfileInt(_T("SELL"), _T("BanSellFilter"), 1, szIniFile);
    
    // Load bansell list
    TCHAR szRawBanSell[4096] = {0};
    GetPrivateProfileString(_T("FILTER"), _T("BanSellList"), _T(""), szRawBanSell, 4096, szIniFile);
    if (_tcslen(szRawBanSell) > 0) {
        int dstB = 0;
        for (int src = 0; szRawBanSell[src] != _T('\0') && dstB < 4094; ++src) {
            if (szRawBanSell[src] == _T('|')) {
                g_szBanSellList[dstB++] = _T('\r');
                g_szBanSellList[dstB++] = _T('\n');
            } else {
                g_szBanSellList[dstB++] = szRawBanSell[src];
            }
        }
        g_szBanSellList[dstB] = _T('\0');
    }

    // 挂机中心点 & 空闲回归 & 定点攻击 & 活动半径
    g_bUseHomePoint     = GetPrivateProfileInt(_T("HOME"), _T("UseHomePoint"), 1, szIniFile);
    g_nIdleReturnSec    = GetPrivateProfileInt(_T("HOME"), _T("IdleReturnSec"), 15, szIniFile);
    g_nHomeRadius       = GetPrivateProfileInt(_T("HOME"), _T("HomeRadius"), 25, szIniFile);
    g_wHomeX            = (WORD)GetPrivateProfileInt(_T("HOME"), _T("HomeX"), 0, szIniFile);
    g_wHomeY            = (WORD)GetPrivateProfileInt(_T("HOME"), _T("HomeY"), 0, szIniFile);
    g_bFixedPointAttack = GetPrivateProfileInt(_T("HOME"), _T("FixedPointAttack"), 0, szIniFile);
    if (g_nIdleReturnSec < 0) g_nIdleReturnSec = 0;
    if (g_nHomeRadius < 5) g_nHomeRadius = 5;
}

void SaveCheatConfig() {
    TCHAR szIniFile[MAX_PATH];
    GetIniPath(szIniFile, MAX_PATH);
    
    TCHAR szVal[32];
    
    _sntprintf(szVal, 32, _T("%d"), g_bAutoAttack);
    WritePrivateProfileString(_T("CHEAT"), _T("AutoAttack"), szVal, szIniFile);
    _sntprintf(szVal, 32, _T("%d"), g_bAutoLoot);
    WritePrivateProfileString(_T("CHEAT"), _T("AutoLoot"), szVal, szIniFile);
    _sntprintf(szVal, 32, _T("%d"), g_nAttackMode);
    WritePrivateProfileString(_T("CHEAT"), _T("AttackMode"), szVal, szIniFile);
    
    _sntprintf(szVal, 32, _T("%d"), g_bAutoHP);
    WritePrivateProfileString(_T("CHEAT"), _T("AutoHP"), szVal, szIniFile);
    _sntprintf(szVal, 32, _T("%d"), g_nHPPercent);
    WritePrivateProfileString(_T("CHEAT"), _T("HPPercent"), szVal, szIniFile);
    WritePrivateProfileString(_T("CHEAT"), _T("HPPotionName"), g_szHPPotionName, szIniFile);
    _sntprintf(szVal, 32, _T("%d"), g_bAutoBuyHP);
    WritePrivateProfileString(_T("CHEAT"), _T("AutoBuyHP"), szVal, szIniFile);
    
    _sntprintf(szVal, 32, _T("%d"), g_bAutoMP);
    WritePrivateProfileString(_T("CHEAT"), _T("AutoMP"), szVal, szIniFile);
    _sntprintf(szVal, 32, _T("%d"), g_nMPPercent);
    WritePrivateProfileString(_T("CHEAT"), _T("MPPercent"), szVal, szIniFile);
    WritePrivateProfileString(_T("CHEAT"), _T("MPPotionName"), g_szMPPotionName, szIniFile);
    _sntprintf(szVal, 32, _T("%d"), g_bAutoBuyMP);
    WritePrivateProfileString(_T("CHEAT"), _T("AutoBuyMP"), szVal, szIniFile);
    
    _sntprintf(szVal, 32, _T("%d"), g_bAutoPet);
    WritePrivateProfileString(_T("CHEAT"), _T("AutoPet"), szVal, szIniFile);
    _sntprintf(szVal, 32, _T("%d"), g_nPetWildRate);
    WritePrivateProfileString(_T("CHEAT"), _T("PetWildRate"), szVal, szIniFile);
    _sntprintf(szVal, 32, _T("%u"), g_dwPetFoodID);
    WritePrivateProfileString(_T("CHEAT"), _T("PetFoodID"), szVal, szIniFile);
    _sntprintf(szVal, 32, _T("%u"), g_dwPetSealID);
    WritePrivateProfileString(_T("CHEAT"), _T("PetSealID"), szVal, szIniFile);
    _sntprintf(szVal, 32, _T("%d"), g_nBuffInterval);
    WritePrivateProfileString(_T("CHEAT"), _T("BuffInterval"), szVal, szIniFile);
    
    // Save filter list
    TCHAR szRawFilter[4096] = {0};
    int dst = 0;
    for (int src = 0; g_szFilterList[src] != _T('\0') && dst < 4094; ++src) {
        if (g_szFilterList[src] == _T('\r')) {
            // skip
        } else if (g_szFilterList[src] == _T('\n')) {
            szRawFilter[dst++] = _T('|');
        } else {
            szRawFilter[dst++] = g_szFilterList[src];
        }
    }
    szRawFilter[dst] = _T('\0');
    WritePrivateProfileString(_T("FILTER"), _T("List"), szRawFilter, szIniFile);

    // Save Skill Configs
    _sntprintf(szVal, 32, _T("%u"), g_dwAttackSkillID);
    WritePrivateProfileString(_T("SKILL"), _T("AttackSkillID"), szVal, szIniFile);
    _sntprintf(szVal, 32, _T("%d"), g_nAttackSkillInterval);
    WritePrivateProfileString(_T("SKILL"), _T("AttackSkillInterval"), szVal, szIniFile);

    _sntprintf(szVal, 32, _T("%u"), g_dwBuffSkillID1);
    WritePrivateProfileString(_T("SKILL"), _T("BuffSkillID1"), szVal, szIniFile);
    _sntprintf(szVal, 32, _T("%d"), g_nBuffSkillInterval1);
    WritePrivateProfileString(_T("SKILL"), _T("BuffSkillInterval1"), szVal, szIniFile);

    _sntprintf(szVal, 32, _T("%u"), g_dwBuffSkillID2);
    WritePrivateProfileString(_T("SKILL"), _T("BuffSkillID2"), szVal, szIniFile);
    _sntprintf(szVal, 32, _T("%d"), g_nBuffSkillInterval2);
    WritePrivateProfileString(_T("SKILL"), _T("BuffSkillInterval2"), szVal, szIniFile);

    _sntprintf(szVal, 32, _T("%u"), g_dwTeammateSkillID1);
    WritePrivateProfileString(_T("SKILL"), _T("TeammateSkillID1"), szVal, szIniFile);
    _sntprintf(szVal, 32, _T("%d"), g_nTeammateSkillInterval1);
    WritePrivateProfileString(_T("SKILL"), _T("TeammateSkillInterval1"), szVal, szIniFile);

    _sntprintf(szVal, 32, _T("%u"), g_dwTeammateSkillID2);
    WritePrivateProfileString(_T("SKILL"), _T("TeammateSkillID2"), szVal, szIniFile);
    _sntprintf(szVal, 32, _T("%d"), g_nTeammateSkillInterval2);
    WritePrivateProfileString(_T("SKILL"), _T("TeammateSkillInterval2"), szVal, szIniFile);

    // Save Bank Configs
    _sntprintf(szVal, 32, _T("%d"), g_bAutoStoreBank);
    WritePrivateProfileString(_T("BANK"), _T("AutoStoreBank"), szVal, szIniFile);
    TCHAR szRawStoreBank[4096] = {0};
    int dstS = 0;
    for (int src = 0; g_szStoreBankList[src] != _T('\0') && dstS < 4094; ++src) {
        if (g_szStoreBankList[src] == _T('\r')) {
            // skip
        } else if (g_szStoreBankList[src] == _T('\n')) {
            szRawStoreBank[dstS++] = _T('|');
        } else {
            szRawStoreBank[dstS++] = g_szStoreBankList[src];
        }
    }
    szRawStoreBank[dstS] = _T('\0');
    WritePrivateProfileString(_T("FILTER"), _T("StoreBankList"), szRawStoreBank, szIniFile);

    // Save Sell Configs
    _sntprintf(szVal, 32, _T("%d"), g_bAutoSellFull);
    WritePrivateProfileString(_T("SELL"), _T("AutoSellFull"), szVal, szIniFile);
    _sntprintf(szVal, 32, _T("%d"), g_bAutoSellAll);
    WritePrivateProfileString(_T("SELL"), _T("AutoSellAll"), szVal, szIniFile);
    _sntprintf(szVal, 32, _T("%d"), g_bBanSellFilter);
    WritePrivateProfileString(_T("SELL"), _T("BanSellFilter"), szVal, szIniFile);
    
    // Save bansell list
    TCHAR szRawBanSell[4096] = {0};
    int dstB = 0;
    for (int src = 0; g_szBanSellList[src] != _T('\0') && dstB < 4094; ++src) {
        if (g_szBanSellList[src] == _T('\r')) {
            // skip
        } else if (g_szBanSellList[src] == _T('\n')) {
            szRawBanSell[dstB++] = _T('|');
        } else {
            szRawBanSell[dstB++] = g_szBanSellList[src];
        }
    }
    szRawBanSell[dstB] = _T('\0');
    WritePrivateProfileString(_T("FILTER"), _T("BanSellList"), szRawBanSell, szIniFile);

    // 挂机中心点 & 空闲回归 & 定点攻击 & 活动半径
    _sntprintf(szVal, 32, _T("%d"), g_bUseHomePoint);
    WritePrivateProfileString(_T("HOME"), _T("UseHomePoint"), szVal, szIniFile);
    _sntprintf(szVal, 32, _T("%d"), g_nIdleReturnSec);
    WritePrivateProfileString(_T("HOME"), _T("IdleReturnSec"), szVal, szIniFile);
    _sntprintf(szVal, 32, _T("%d"), g_nHomeRadius);
    WritePrivateProfileString(_T("HOME"), _T("HomeRadius"), szVal, szIniFile);
    _sntprintf(szVal, 32, _T("%d"), g_wHomeX);
    WritePrivateProfileString(_T("HOME"), _T("HomeX"), szVal, szIniFile);
    _sntprintf(szVal, 32, _T("%d"), g_wHomeY);
    WritePrivateProfileString(_T("HOME"), _T("HomeY"), szVal, szIniFile);
    _sntprintf(szVal, 32, _T("%d"), g_bFixedPointAttack);
    WritePrivateProfileString(_T("HOME"), _T("FixedPointAttack"), szVal, szIniFile);
}

BOOL IsItemFiltered(const TCHAR* szItemName) {
    if (!szItemName || _tcslen(szItemName) == 0) return FALSE;
    TCHAR szListCopy[4096];
    _tcscpy_s(szListCopy, 4096, g_szFilterList);
    TCHAR* context = NULL;
    TCHAR* token = _tcstok_s(szListCopy, _T("\r\n|,"), &context);
    while (token != NULL) {
        TCHAR szTrimmed[128] = {0};
        int len = _tcslen(token);
        int start = 0;
        while (start < len && (token[start] == _T(' ') || token[start] == _T('\t'))) {
            start++;
        }
        int end = len - 1;
        while (end >= start && (token[end] == _T(' ') || token[end] == _T('\t'))) {
            end--;
        }
        if (end >= start) {
            _tcsncpy(szTrimmed, token + start, end - start + 1);
            szTrimmed[end - start + 1] = _T('\0');
            if (_tcscmp(szTrimmed, szItemName) == 0) {
                return TRUE;
            }
        }
        token = _tcstok_s(NULL, _T("\r\n|,"), &context);
    }
    return FALSE;
}

void ApplyCheatConfigToMainChar() {
}

LRESULT CALLBACK ConfigWndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
    static HFONT hFont = NULL;
    
    switch (msg) {
    case WM_CREATE: {
        LoadCheatConfig();
        // 12号字 (12 pt，即小四号字)。利用屏幕 DPI 进行标准点数换算：96 DPI 下为 -16px 字符高度
        HDC hdc = GetDC(hwnd);
        int nFontSize = 12; // 12 pt
        int nFontHeight = -MulDiv(nFontSize, GetDeviceCaps(hdc, LOGPIXELSY), 72);
        ReleaseDC(hwnd, hdc);

        hFont = CreateFont(nFontHeight, 0, 0, 0, FW_NORMAL, FALSE, FALSE, FALSE,
                           GB2132_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
                           CLEARTYPE_QUALITY, DEFAULT_PITCH | FF_DONTCARE, _T("Microsoft YaHei"));
        if (!hFont) {
            hFont = CreateFont(nFontHeight, 0, 0, 0, FW_NORMAL, FALSE, FALSE, FALSE,
                               GB2132_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
                               CLEARTYPE_QUALITY, DEFAULT_PITCH | FF_DONTCARE, _T("SimSun"));
        }
        if (!hFont) {
            hFont = (HFONT)GetStockObject(DEFAULT_GUI_FONT);
        }
        
        g_nTab0Count = 0;
        g_hTab0Controls[g_nTab0Count++] = CreateWindow(_T("STATIC"), _T(""), WS_CHILD, 0, 0, 0, 0, hwnd, NULL, NULL, NULL);
        
        g_nTab1Count = 0;
        g_nTab2Count = 0;
        
        TCHAR szTmp[64];
        HWND hCtrl;
        
        HWND hTab = CreateWindow(WC_TABCONTROL, _T(""), WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS, 
                                 12, 10, 620, 475, hwnd, (HMENU)ID_TABCONTROL, NULL, NULL);
        SendMessage(hTab, WM_SETFONT, (WPARAM)hFont, TRUE);
        
        TCITEM tie;
        tie.mask = TCIF_TEXT;
        tie.pszText = _T("\xbb\xf9\xb4\xa1\xc9\xe8\xd6\xc3");
        TabCtrl_InsertItem(hTab, 0, &tie);
        tie.pszText = _T("\xbc\xbc\xc4\xdc\xc9\xe8\xd6\xc3");
        TabCtrl_InsertItem(hTab, 1, &tie);
        tie.pszText = _T("\xb4\xe6\xb2\xd6\xd3\xeb\xca\xdb\xc2\xf4");
        TabCtrl_InsertItem(hTab, 2, &tie);
        
        hCtrl = CreateWindow(_T("BUTTON"), _T("\xd7\xd4\xb6\xaf\xd1\xb0\xd5\xd2\xb9\xd6\xce\xef\xb4\xf2\xb9\xd6"), WS_VISIBLE | WS_CHILD | BS_AUTOCHECKBOX, 25, 45, 185, 24, hwnd, (HMENU)ID_CHECK_AUTOATTACK, NULL, NULL);
        SendMessage(hCtrl, BM_SETCHECK, g_bAutoAttack ? BST_CHECKED : BST_UNCHECKED, 0);
        SendMessage(hCtrl, WM_SETFONT, (WPARAM)hFont, TRUE);
        g_hTab0Controls[g_nTab0Count++] = hCtrl;
        
        hCtrl = CreateWindow(_T("BUTTON"), _T("\xce\xef\xc0\xed\xb9\xa5\xbb\xf7"), WS_VISIBLE | WS_CHILD | BS_AUTORADIOBUTTON | WS_GROUP, 220, 45, 90, 24, hwnd, (HMENU)ID_RADIO_PHYSICAL, NULL, NULL);
        SendMessage(hCtrl, BM_SETCHECK, (g_nAttackMode == 0) ? BST_CHECKED : BST_UNCHECKED, 0);
        SendMessage(hCtrl, WM_SETFONT, (WPARAM)hFont, TRUE);
        g_hTab0Controls[g_nTab0Count++] = hCtrl;
        
        hCtrl = CreateWindow(_T("BUTTON"), _T("\xbc\xbc\xc4\xdc\xb9\xa5\xbb\xf7"), WS_VISIBLE | WS_CHILD | BS_AUTORADIOBUTTON, 315, 45, 90, 24, hwnd, (HMENU)ID_RADIO_SKILL, NULL, NULL);
        SendMessage(hCtrl, BM_SETCHECK, (g_nAttackMode == 1) ? BST_CHECKED : BST_UNCHECKED, 0);
        SendMessage(hCtrl, WM_SETFONT, (WPARAM)hFont, TRUE);
        g_hTab0Controls[g_nTab0Count++] = hCtrl;
        
        hCtrl = CreateWindow(_T("BUTTON"), _T("\xd7\xd4\xb6\xaf\xca\xb0\xc8\xa1\xb7\xb6\xce\xa7\xce\xef\xc6\xb7"), WS_VISIBLE | WS_CHILD | BS_AUTOCHECKBOX, 25, 73, 300, 24, hwnd, (HMENU)ID_CHECK_AUTOLOOT, NULL, NULL);
        SendMessage(hCtrl, BM_SETCHECK, g_bAutoLoot ? BST_CHECKED : BST_UNCHECKED, 0);
        SendMessage(hCtrl, WM_SETFONT, (WPARAM)hFont, TRUE);
        g_hTab0Controls[g_nTab0Count++] = hCtrl;
        
        hCtrl = CreateWindow(_T("BUTTON"), _T("\xd7\xd4\xb6\xaf\xb3\xd4\xd2\xa9\xd3\xeb\xd4\xb6\xb3\xcc\xb2\xb9\xb8\xf8"), WS_VISIBLE | WS_CHILD | BS_GROUPBOX, 22, 102, 355, 135, hwnd, NULL, NULL, NULL);
        SendMessage(hCtrl, WM_SETFONT, (WPARAM)hFont, TRUE);
        g_hTab0Controls[g_nTab0Count++] = hCtrl;
        
        hCtrl = CreateWindow(_T("BUTTON"), _T("\xba\xec\xd2\xa9"), WS_VISIBLE | WS_CHILD | BS_AUTOCHECKBOX, 32, 128, 55, 24, hwnd, (HMENU)ID_CHECK_AUTOHP, NULL, NULL);
        SendMessage(hCtrl, BM_SETCHECK, g_bAutoHP ? BST_CHECKED : BST_UNCHECKED, 0);
        SendMessage(hCtrl, WM_SETFONT, (WPARAM)hFont, TRUE);
        g_hTab0Controls[g_nTab0Count++] = hCtrl;
        
        HWND hHPPotionName = CreateWindow(_T("EDIT"), _T(""), WS_VISIBLE | WS_CHILD | WS_BORDER | ES_AUTOHSCROLL, 90, 128, 95, 24, hwnd, (HMENU)ID_EDIT_HPPOTIONNAME, NULL, NULL);
        SendMessage(hHPPotionName, WM_SETFONT, (WPARAM)hFont, TRUE);
        SetWindowText(hHPPotionName, g_szHPPotionName);
        g_hTab0Controls[g_nTab0Count++] = hHPPotionName;
        
        hCtrl = CreateWindow(_T("STATIC"), _T("\xe3\xd0\xd6\xb5\x3a"), WS_VISIBLE | WS_CHILD, 190, 130, 42, 20, hwnd, NULL, NULL, NULL);
        SendMessage(hCtrl, WM_SETFONT, (WPARAM)hFont, TRUE);
        g_hTab0Controls[g_nTab0Count++] = hCtrl;
        
        HWND hHPPercent = CreateWindow(_T("EDIT"), _T(""), WS_VISIBLE | WS_CHILD | WS_BORDER | ES_NUMBER, 235, 128, 35, 24, hwnd, (HMENU)ID_EDIT_HPPERCENT, NULL, NULL);
        SendMessage(hHPPercent, WM_SETFONT, (WPARAM)hFont, TRUE);
        _sntprintf(szTmp, 16, _T("%d"), g_nHPPercent);
        SetWindowText(hHPPercent, szTmp);
        g_hTab0Controls[g_nTab0Count++] = hHPPercent;
        
        hCtrl = CreateWindow(_T("STATIC"), _T("%"), WS_VISIBLE | WS_CHILD, 273, 130, 15, 20, hwnd, NULL, NULL, NULL);
        SendMessage(hCtrl, WM_SETFONT, (WPARAM)hFont, TRUE);
        g_hTab0Controls[g_nTab0Count++] = hCtrl;
        
        hCtrl = CreateWindow(_T("BUTTON"), _T("\xb2\xb9\xbb\xf5"), WS_VISIBLE | WS_CHILD | BS_AUTOCHECKBOX, 292, 128, 65, 24, hwnd, (HMENU)ID_CHECK_AUTOBUYHP, NULL, NULL);
        SendMessage(hCtrl, BM_SETCHECK, g_bAutoBuyHP ? BST_CHECKED : BST_UNCHECKED, 0);
        SendMessage(hCtrl, WM_SETFONT, (WPARAM)hFont, TRUE);
        g_hTab0Controls[g_nTab0Count++] = hCtrl;
        
        hCtrl = CreateWindow(_T("BUTTON"), _T("\xc0\xb6\xd2\xa9"), WS_VISIBLE | WS_CHILD | BS_AUTOCHECKBOX, 32, 180, 55, 24, hwnd, (HMENU)ID_CHECK_AUTOMP, NULL, NULL);
        SendMessage(hCtrl, BM_SETCHECK, g_bAutoMP ? BST_CHECKED : BST_UNCHECKED, 0);
        SendMessage(hCtrl, WM_SETFONT, (WPARAM)hFont, TRUE);
        g_hTab0Controls[g_nTab0Count++] = hCtrl;
        
        HWND hMPPotionName = CreateWindow(_T("EDIT"), _T(""), WS_VISIBLE | WS_CHILD | WS_BORDER | ES_AUTOHSCROLL, 90, 180, 95, 24, hwnd, (HMENU)ID_EDIT_MPPOTIONNAME, NULL, NULL);
        SendMessage(hMPPotionName, WM_SETFONT, (WPARAM)hFont, TRUE);
        SetWindowText(hMPPotionName, g_szMPPotionName);
        g_hTab0Controls[g_nTab0Count++] = hMPPotionName;
        
        hCtrl = CreateWindow(_T("STATIC"), _T("\xe3\xd0\xd6\xb5\x3a"), WS_VISIBLE | WS_CHILD, 190, 182, 42, 20, hwnd, NULL, NULL, NULL);
        SendMessage(hCtrl, WM_SETFONT, (WPARAM)hFont, TRUE);
        g_hTab0Controls[g_nTab0Count++] = hCtrl;
        
        HWND hMPPercent = CreateWindow(_T("EDIT"), _T(""), WS_VISIBLE | WS_CHILD | WS_BORDER | ES_NUMBER, 235, 180, 35, 24, hwnd, (HMENU)ID_EDIT_MPPERCENT, NULL, NULL);
        SendMessage(hMPPercent, WM_SETFONT, (WPARAM)hFont, TRUE);
        _sntprintf(szTmp, 16, _T("%d"), g_nMPPercent);
        SetWindowText(hMPPercent, szTmp);
        g_hTab0Controls[g_nTab0Count++] = hMPPercent;
        
        hCtrl = CreateWindow(_T("STATIC"), _T("%"), WS_VISIBLE | WS_CHILD, 273, 182, 15, 20, hwnd, NULL, NULL, NULL);
        SendMessage(hCtrl, WM_SETFONT, (WPARAM)hFont, TRUE);
        g_hTab0Controls[g_nTab0Count++] = hCtrl;
        
        hCtrl = CreateWindow(_T("BUTTON"), _T("\xb2\xb9\xbb\xf5"), WS_VISIBLE | WS_CHILD | BS_AUTOCHECKBOX, 292, 180, 65, 24, hwnd, (HMENU)ID_CHECK_AUTOBUYMP, NULL, NULL);
        SendMessage(hCtrl, BM_SETCHECK, g_bAutoBuyMP ? BST_CHECKED : BST_UNCHECKED, 0);
        SendMessage(hCtrl, WM_SETFONT, (WPARAM)hFont, TRUE);
        g_hTab0Controls[g_nTab0Count++] = hCtrl;
        
        hCtrl = CreateWindow(_T("BUTTON"), _T("\xb3\xe8\xce\xef\xd6\xfa\xca\xd6\xc9\xe8\xd6\xc3"), WS_VISIBLE | WS_CHILD | BS_GROUPBOX, 22, 248, 355, 135, hwnd, NULL, NULL, NULL);
        SendMessage(hCtrl, WM_SETFONT, (WPARAM)hFont, TRUE);
        g_hTab0Controls[g_nTab0Count++] = hCtrl;
        
        hCtrl = CreateWindow(_T("BUTTON"), _T("\xc6\xf4\xd3\xc3\xd7\xd4\xb6\xaf\xb3\xe8\xce\xef\xd6\xfa\xca\xd6\x20\x28\xce\xb9\xca\xb3\x2f\xb2\xb9\xd1\xaa\x2f\x42\x75\x66\x66\x2f\xd7\xd4\xb6\xaf\xd5\xd9\xbb\xd8\x29"), WS_VISIBLE | WS_CHILD | BS_AUTOCHECKBOX, 32, 273, 335, 24, hwnd, (HMENU)ID_CHECK_AUTOPET, NULL, NULL);
        SendMessage(hCtrl, BM_SETCHECK, g_bAutoPet ? BST_CHECKED : BST_UNCHECKED, 0);
        SendMessage(hCtrl, WM_SETFONT, (WPARAM)hFont, TRUE);
        g_hTab0Controls[g_nTab0Count++] = hCtrl;
        
        hCtrl = CreateWindow(_T("STATIC"), _T("\xd2\xb0\xd0\xd4\xe3\xd0\xd6\xb5\x3a"), WS_VISIBLE | WS_CHILD, 32, 325, 68, 20, hwnd, NULL, NULL, NULL);
        SendMessage(hCtrl, WM_SETFONT, (WPARAM)hFont, TRUE);
        g_hTab0Controls[g_nTab0Count++] = hCtrl;
        
        HWND hPetWildRate = CreateWindow(_T("EDIT"), _T(""), WS_VISIBLE | WS_CHILD | WS_BORDER | ES_NUMBER, 102, 323, 32, 24, hwnd, (HMENU)ID_EDIT_PETWILDRATE, NULL, NULL);
        SendMessage(hPetWildRate, WM_SETFONT, (WPARAM)hFont, TRUE);
        _sntprintf(szTmp, 16, _T("%d"), g_nPetWildRate);
        SetWindowText(hPetWildRate, szTmp);
        g_hTab0Controls[g_nTab0Count++] = hPetWildRate;
        
        hCtrl = CreateWindow(_T("STATIC"), _T("\xcb\xc7\xc1\xcf\x49\x44\x3a"), WS_VISIBLE | WS_CHILD, 138, 325, 56, 20, hwnd, NULL, NULL, NULL);
        SendMessage(hCtrl, WM_SETFONT, (WPARAM)hFont, TRUE);
        g_hTab0Controls[g_nTab0Count++] = hCtrl;
        
        HWND hPetFoodID = CreateWindow(_T("EDIT"), _T(""), WS_VISIBLE | WS_CHILD | WS_BORDER | ES_NUMBER, 198, 323, 44, 24, hwnd, (HMENU)ID_EDIT_PETFOODID, NULL, NULL);
        SendMessage(hPetFoodID, WM_SETFONT, (WPARAM)hFont, TRUE);
        _sntprintf(szTmp, 16, _T("%u"), g_dwPetFoodID);
        SetWindowText(hPetFoodID, szTmp);
        g_hTab0Controls[g_nTab0Count++] = hPetFoodID;
        
        hCtrl = CreateWindow(_T("STATIC"), _T("\xb7\xe2\xd3\xa1\x49\x44\x3a"), WS_VISIBLE | WS_CHILD, 248, 325, 56, 20, hwnd, NULL, NULL, NULL);
        SendMessage(hCtrl, WM_SETFONT, (WPARAM)hFont, TRUE);
        g_hTab0Controls[g_nTab0Count++] = hCtrl;
        
        HWND hPetSealID = CreateWindow(_T("EDIT"), _T(""), WS_VISIBLE | WS_CHILD | WS_BORDER | ES_NUMBER, 306, 323, 44, 24, hwnd, (HMENU)ID_EDIT_PETSEALID, NULL, NULL);
        SendMessage(hPetSealID, WM_SETFONT, (WPARAM)hFont, TRUE);
        _sntprintf(szTmp, 16, _T("%u"), g_dwPetSealID);
        SetWindowText(hPetSealID, szTmp);
        g_hTab0Controls[g_nTab0Count++] = hPetSealID;
        
        hCtrl = CreateWindow(_T("BUTTON"), _T("\xca\xb0\xc8\xa1\xb9\xfd\xc2\xcb\xc1\xd0\xb1\xed\x20\x28\xc3\xbf\xd0\xd0\xd2\xbb\xb8\xf6\x29"), WS_VISIBLE | WS_CHILD | BS_GROUPBOX, 390, 42, 230, 341, hwnd, NULL, NULL, NULL);
        SendMessage(hCtrl, WM_SETFONT, (WPARAM)hFont, TRUE);
        g_hTab0Controls[g_nTab0Count++] = hCtrl;
        
        HWND hFilterEdit = CreateWindow(_T("EDIT"), _T(""), WS_VISIBLE | WS_CHILD | WS_BORDER | ES_MULTILINE | ES_AUTOVSCROLL | ES_WANTRETURN | WS_VSCROLL, 400, 68, 210, 305, hwnd, (HMENU)ID_EDIT_FILTERLIST, NULL, NULL);
        SendMessage(hFilterEdit, WM_SETFONT, (WPARAM)hFont, TRUE);
        SetWindowText(hFilterEdit, g_szFilterList);
        g_hTab0Controls[g_nTab0Count++] = hFilterEdit;
        
        // === 挂机中心点 & 空闲回归设置 ===
        hCtrl = CreateWindow(_T("BUTTON"), _T("\xb9\xd2\xbb\xfa\xd6\xd0\xd0\xc4\xb5\xe3\xc9\xe8\xd6\xc3"), WS_VISIBLE | WS_CHILD | BS_GROUPBOX, 22, 403, 600, 82, hwnd, NULL, NULL, NULL);
        SendMessage(hCtrl, WM_SETFONT, (WPARAM)hFont, TRUE);
        g_hTab0Controls[g_nTab0Count++] = hCtrl;
        
        // 启用回中心点
        hCtrl = CreateWindow(_T("BUTTON"), _T("\xc6\xf4\xd3\xc3\xbb\xd8\xd6\xd0\xd0\xc4\xb5\xe3"), WS_VISIBLE | WS_CHILD | BS_AUTOCHECKBOX, 32, 424, 120, 24, hwnd, (HMENU)ID_CHECK_USEHOME, NULL, NULL);
        SendMessage(hCtrl, BM_SETCHECK, g_bUseHomePoint ? BST_CHECKED : BST_UNCHECKED, 0);
        SendMessage(hCtrl, WM_SETFONT, (WPARAM)hFont, TRUE);
        g_hTab0Controls[g_nTab0Count++] = hCtrl;

        // X坐标输入框
        HWND hHomeX = CreateWindow(_T("EDIT"), _T(""), WS_VISIBLE | WS_CHILD | WS_BORDER | ES_NUMBER, 156, 424, 48, 24, hwnd, (HMENU)ID_EDIT_HOMEX, NULL, NULL);
        SendMessage(hHomeX, WM_SETFONT, (WPARAM)hFont, TRUE);
        _sntprintf(szTmp, 16, _T("%u"), g_wHomeX);
        SetWindowText(hHomeX, szTmp);
        g_hTab0Controls[g_nTab0Count++] = hHomeX;

        // Y坐标输入框
        HWND hHomeY = CreateWindow(_T("EDIT"), _T(""), WS_VISIBLE | WS_CHILD | WS_BORDER | ES_NUMBER, 208, 424, 48, 24, hwnd, (HMENU)ID_EDIT_HOMEY, NULL, NULL);
        SendMessage(hHomeY, WM_SETFONT, (WPARAM)hFont, TRUE);
        _sntprintf(szTmp, 16, _T("%u"), g_wHomeY);
        SetWindowText(hHomeY, szTmp);
        g_hTab0Controls[g_nTab0Count++] = hHomeY;

        // [ 取 ] 按钮
        HWND hBtnGetPos = CreateWindow(_T("BUTTON"), _T("\xc8\xa1"), WS_VISIBLE | WS_CHILD, 260, 424, 38, 24, hwnd, (HMENU)ID_BUTTON_GETCURPOS, NULL, NULL);
        SendMessage(hBtnGetPos, WM_SETFONT, (WPARAM)hFont, TRUE);
        g_hTab0Controls[g_nTab0Count++] = hBtnGetPos;

        // 定点攻击复选框
        hCtrl = CreateWindow(_T("BUTTON"), _T("\xb6\xa8\xb5\xe3\xb9\xa5\xbb\xf7"), WS_VISIBLE | WS_CHILD | BS_AUTOCHECKBOX, 310, 424, 90, 24, hwnd, (HMENU)ID_CHECK_FIXEDPOINT, NULL, NULL);
        SendMessage(hCtrl, BM_SETCHECK, g_bFixedPointAttack ? BST_CHECKED : BST_UNCHECKED, 0);
        SendMessage(hCtrl, WM_SETFONT, (WPARAM)hFont, TRUE);
        g_hTab0Controls[g_nTab0Count++] = hCtrl;

        // 提示：(站原地不动攻击/群攻)
        hCtrl = CreateWindow(_T("STATIC"), _T("\x28\xd5\xbe\xd4\xad\xb5\xd8\xb2\xbb\xb6\xaf\xb9\xa5\xbb\xf7\x2f\xc8\xba\xb9\xa5\x29"), WS_VISIBLE | WS_CHILD, 404, 426, 210, 20, hwnd, NULL, NULL, NULL);
        SendMessage(hCtrl, WM_SETFONT, (WPARAM)hFont, TRUE);
        g_hTab0Controls[g_nTab0Count++] = hCtrl;
        
        // 第2行：活动半径 [ 25 ] 码(超范围即回)  空闲超过: [ 15 ] 秒未攻击回中心点(未设置时自动记录)
        hCtrl = CreateWindow(_T("STATIC"), _T("\xbb\xee\xb6\xaf\xb1\xeb\xbe\xb6\x3a"), WS_VISIBLE | WS_CHILD, 32, 453, 65, 20, hwnd, NULL, NULL, NULL);
        SendMessage(hCtrl, WM_SETFONT, (WPARAM)hFont, TRUE);
        g_hTab0Controls[g_nTab0Count++] = hCtrl;
        
        HWND hHomeRadius = CreateWindow(_T("EDIT"), _T(""), WS_VISIBLE | WS_CHILD | WS_BORDER | ES_NUMBER, 98, 451, 35, 24, hwnd, (HMENU)ID_EDIT_HOMERADIUS, NULL, NULL);
        SendMessage(hHomeRadius, WM_SETFONT, (WPARAM)hFont, TRUE);
        _sntprintf(szTmp, 16, _T("%d"), g_nHomeRadius);
        SetWindowText(hHomeRadius, szTmp);
        g_hTab0Controls[g_nTab0Count++] = hHomeRadius;

        hCtrl = CreateWindow(_T("STATIC"), _T("\xc2\xeb\x28\xb3\xac\xb7\xb6\xce\xa7\xbc\xb4\xbb\xd8\x29"), WS_VISIBLE | WS_CHILD, 136, 453, 105, 20, hwnd, NULL, NULL, NULL);
        SendMessage(hCtrl, WM_SETFONT, (WPARAM)hFont, TRUE);
        g_hTab0Controls[g_nTab0Count++] = hCtrl;

        hCtrl = CreateWindow(_T("STATIC"), _T("\xbf\xd5\xc8\xd5\xb3\xac\xb9\xfd\x3a"), WS_VISIBLE | WS_CHILD, 245, 453, 65, 20, hwnd, NULL, NULL, NULL);
        SendMessage(hCtrl, WM_SETFONT, (WPARAM)hFont, TRUE);
        g_hTab0Controls[g_nTab0Count++] = hCtrl;
        
        HWND hIdleSec = CreateWindow(_T("EDIT"), _T(""), WS_VISIBLE | WS_CHILD | WS_BORDER | ES_NUMBER, 312, 451, 35, 24, hwnd, (HMENU)ID_EDIT_IDLERETURN, NULL, NULL);
        SendMessage(hIdleSec, WM_SETFONT, (WPARAM)hFont, TRUE);
        _sntprintf(szTmp, 16, _T("%d"), g_nIdleReturnSec);
        SetWindowText(hIdleSec, szTmp);
        g_hTab0Controls[g_nTab0Count++] = hIdleSec;
        
        hCtrl = CreateWindow(_T("STATIC"), _T("\xc3\xeb\xce\xb4\xb9\xa5\xbb\xf7\xbb\xd8\xd6\xd0\xd0\xc4\xb5\xe3\x28\xce\xb4\xc9\xe8\xd6\xc3\xd7\xd4\xb6\xaf\xbc\xc7\xc2\xbc\x29"), WS_VISIBLE | WS_CHILD, 350, 453, 265, 20, hwnd, NULL, NULL, NULL);
        SendMessage(hCtrl, WM_SETFONT, (WPARAM)hFont, TRUE);
        g_hTab0Controls[g_nTab0Count++] = hCtrl;
        
        
        hCtrl = CreateWindow(_T("BUTTON"), _T("\xd7\xd4\xb6\xaf\xca\xa9\xb7\xa8\xbc\xbc\xc4\xdc\xc9\xe8\xd6\xc3"), WS_VISIBLE | WS_CHILD | BS_GROUPBOX, 22, 42, 600, 341, hwnd, NULL, NULL, NULL);
        SendMessage(hCtrl, WM_SETFONT, (WPARAM)hFont, TRUE);
        g_hTab1Controls[g_nTab1Count++] = hCtrl;
        
        hCtrl = CreateWindow(_T("STATIC"), _T("\xb9\xa5\xbb\xf7\xbc\xbc\xc4\xdc\x3a"), WS_VISIBLE | WS_CHILD, 38, 70, 75, 20, hwnd, NULL, NULL, NULL);
        SendMessage(hCtrl, WM_SETFONT, (WPARAM)hFont, TRUE);
        g_hTab1Controls[g_nTab1Count++] = hCtrl;
        
        HWND hComboAttack = CreateWindow(_T("COMBOBOX"), _T(""), WS_VISIBLE | WS_CHILD | CBS_DROPDOWNLIST | WS_VSCROLL, 115, 68, 230, 200, hwnd, (HMENU)ID_COMBO_ATTACKSKILL, NULL, NULL);
        SendMessage(hComboAttack, WM_SETFONT, (WPARAM)hFont, TRUE);
        PopulateSkillComboBox(hComboAttack, g_dwAttackSkillID);
        g_hTab1Controls[g_nTab1Count++] = hComboAttack;
        
        hCtrl = CreateWindow(_T("STATIC"), _T("\xca\xa9\xb7\xc5\xbc\xe4\xb8\xf4\x3a"), WS_VISIBLE | WS_CHILD, 360, 70, 75, 20, hwnd, NULL, NULL, NULL);
        SendMessage(hCtrl, WM_SETFONT, (WPARAM)hFont, TRUE);
        g_hTab1Controls[g_nTab1Count++] = hCtrl;
        
        HWND hEditAttackInt = CreateWindow(_T("EDIT"), _T(""), WS_VISIBLE | WS_CHILD | WS_BORDER | ES_NUMBER, 438, 68, 50, 24, hwnd, (HMENU)ID_EDIT_ATTACKINTERVAL, NULL, NULL);
        SendMessage(hEditAttackInt, WM_SETFONT, (WPARAM)hFont, TRUE);
        _sntprintf(szTmp, 16, _T("%d"), g_nAttackSkillInterval);
        SetWindowText(hEditAttackInt, szTmp);
        g_hTab1Controls[g_nTab1Count++] = hEditAttackInt;
        
        hCtrl = CreateWindow(_T("STATIC"), _T("\xc3\xeb\x20\x28\xd7\xee\xb5\xcd\x31\xc3\xeb\x29"), WS_VISIBLE | WS_CHILD, 495, 70, 115, 20, hwnd, NULL, NULL, NULL);
        SendMessage(hCtrl, WM_SETFONT, (WPARAM)hFont, TRUE);
        g_hTab1Controls[g_nTab1Count++] = hCtrl;
        
        hCtrl = CreateWindow(_T("STATIC"), _T("\xd7\xb4\xcc\xac\xbc\xbc\xc4\xdc\x20\x31\x3a"), WS_VISIBLE | WS_CHILD, 38, 120, 75, 20, hwnd, NULL, NULL, NULL);
        SendMessage(hCtrl, WM_SETFONT, (WPARAM)hFont, TRUE);
        g_hTab1Controls[g_nTab1Count++] = hCtrl;
        
        HWND hComboBuff1 = CreateWindow(_T("COMBOBOX"), _T(""), WS_VISIBLE | WS_CHILD | CBS_DROPDOWNLIST | WS_VSCROLL, 115, 118, 230, 200, hwnd, (HMENU)ID_COMBO_BUFFSKILL1, NULL, NULL);
        SendMessage(hComboBuff1, WM_SETFONT, (WPARAM)hFont, TRUE);
        PopulateSkillComboBox(hComboBuff1, g_dwBuffSkillID1);
        g_hTab1Controls[g_nTab1Count++] = hComboBuff1;
        
        hCtrl = CreateWindow(_T("STATIC"), _T("\xca\xa9\xb7\xc5\xbc\xe4\xb8\xf4\x3a"), WS_VISIBLE | WS_CHILD, 360, 120, 75, 20, hwnd, NULL, NULL, NULL);
        SendMessage(hCtrl, WM_SETFONT, (WPARAM)hFont, TRUE);
        g_hTab1Controls[g_nTab1Count++] = hCtrl;
        
        HWND hEditBuffInt1 = CreateWindow(_T("EDIT"), _T(""), WS_VISIBLE | WS_CHILD | WS_BORDER | ES_NUMBER, 438, 118, 50, 24, hwnd, (HMENU)ID_EDIT_BUFFINTERVAL1, NULL, NULL);
        SendMessage(hEditBuffInt1, WM_SETFONT, (WPARAM)hFont, TRUE);
        _sntprintf(szTmp, 16, _T("%d"), g_nBuffSkillInterval1);
        SetWindowText(hEditBuffInt1, szTmp);
        g_hTab1Controls[g_nTab1Count++] = hEditBuffInt1;
        
        hCtrl = CreateWindow(_T("STATIC"), _T("\xc3\xeb\x20\x28\xd7\xee\xb5\xcd\x31\xc3\xeb\x29"), WS_VISIBLE | WS_CHILD, 495, 120, 115, 20, hwnd, NULL, NULL, NULL);
        SendMessage(hCtrl, WM_SETFONT, (WPARAM)hFont, TRUE);
        g_hTab1Controls[g_nTab1Count++] = hCtrl;
        
        hCtrl = CreateWindow(_T("STATIC"), _T("\xd7\xb4\xcc\xac\xbc\xbc\xc4\xdc\x20\x32\x3a"), WS_VISIBLE | WS_CHILD, 38, 170, 75, 20, hwnd, NULL, NULL, NULL);
        SendMessage(hCtrl, WM_SETFONT, (WPARAM)hFont, TRUE);
        g_hTab1Controls[g_nTab1Count++] = hCtrl;
        
        HWND hComboBuff2 = CreateWindow(_T("COMBOBOX"), _T(""), WS_VISIBLE | WS_CHILD | CBS_DROPDOWNLIST | WS_VSCROLL, 115, 168, 230, 200, hwnd, (HMENU)ID_COMBO_BUFFSKILL2, NULL, NULL);
        SendMessage(hComboBuff2, WM_SETFONT, (WPARAM)hFont, TRUE);
        PopulateSkillComboBox(hComboBuff2, g_dwBuffSkillID2);
        g_hTab1Controls[g_nTab1Count++] = hComboBuff2;
        
        hCtrl = CreateWindow(_T("STATIC"), _T("\xca\xa9\xb7\xc5\xbc\xe4\xb8\xf4\x3a"), WS_VISIBLE | WS_CHILD, 360, 170, 75, 20, hwnd, NULL, NULL, NULL);
        SendMessage(hCtrl, WM_SETFONT, (WPARAM)hFont, TRUE);
        g_hTab1Controls[g_nTab1Count++] = hCtrl;
        
        HWND hEditBuffInt2 = CreateWindow(_T("EDIT"), _T(""), WS_VISIBLE | WS_CHILD | WS_BORDER | ES_NUMBER, 438, 168, 50, 24, hwnd, (HMENU)ID_EDIT_BUFFINTERVAL2, NULL, NULL);
        SendMessage(hEditBuffInt2, WM_SETFONT, (WPARAM)hFont, TRUE);
        _sntprintf(szTmp, 16, _T("%d"), g_nBuffSkillInterval2);
        SetWindowText(hEditBuffInt2, szTmp);
        g_hTab1Controls[g_nTab1Count++] = hEditBuffInt2;
        
        hCtrl = CreateWindow(_T("STATIC"), _T("\xc3\xeb\x20\x28\xd7\xee\xb5\xcd\x31\xc3\xeb\x29"), WS_VISIBLE | WS_CHILD, 495, 170, 115, 20, hwnd, NULL, NULL, NULL);
        SendMessage(hCtrl, WM_SETFONT, (WPARAM)hFont, TRUE);
        g_hTab1Controls[g_nTab1Count++] = hCtrl;
        
        hCtrl = CreateWindow(_T("STATIC"), _T("\xb8\xa8\xd6\xfa\xb6\xd3\xd3\xd1\x20\x31\x3a"), WS_VISIBLE | WS_CHILD, 38, 220, 75, 20, hwnd, NULL, NULL, NULL);
        SendMessage(hCtrl, WM_SETFONT, (WPARAM)hFont, TRUE);
        g_hTab1Controls[g_nTab1Count++] = hCtrl;
        
        HWND hComboTeam1 = CreateWindow(_T("COMBOBOX"), _T(""), WS_VISIBLE | WS_CHILD | CBS_DROPDOWNLIST | WS_VSCROLL, 115, 218, 230, 200, hwnd, (HMENU)ID_COMBO_TEAMMATESKILL1, NULL, NULL);
        SendMessage(hComboTeam1, WM_SETFONT, (WPARAM)hFont, TRUE);
        PopulateSkillComboBox(hComboTeam1, g_dwTeammateSkillID1);
        g_hTab1Controls[g_nTab1Count++] = hComboTeam1;
        
        hCtrl = CreateWindow(_T("STATIC"), _T("\xca\xa9\xb7\xc5\xbc\xe4\xb8\xf4\x3a"), WS_VISIBLE | WS_CHILD, 360, 220, 75, 20, hwnd, NULL, NULL, NULL);
        SendMessage(hCtrl, WM_SETFONT, (WPARAM)hFont, TRUE);
        g_hTab1Controls[g_nTab1Count++] = hCtrl;
        
        HWND hEditTeamInt1 = CreateWindow(_T("EDIT"), _T(""), WS_VISIBLE | WS_CHILD | WS_BORDER | ES_NUMBER, 438, 218, 50, 24, hwnd, (HMENU)ID_EDIT_TEAMMATEINTERVAL1, NULL, NULL);
        SendMessage(hEditTeamInt1, WM_SETFONT, (WPARAM)hFont, TRUE);
        _sntprintf(szTmp, 16, _T("%d"), g_nTeammateSkillInterval1);
        SetWindowText(hEditTeamInt1, szTmp);
        g_hTab1Controls[g_nTab1Count++] = hEditTeamInt1;
        
        hCtrl = CreateWindow(_T("STATIC"), _T("\xc3\xeb\x20\x28\xd7\xee\xb5\xcd\x31\xc3\xeb\x29"), WS_VISIBLE | WS_CHILD, 495, 220, 115, 20, hwnd, NULL, NULL, NULL);
        SendMessage(hCtrl, WM_SETFONT, (WPARAM)hFont, TRUE);
        g_hTab1Controls[g_nTab1Count++] = hCtrl;
        
        hCtrl = CreateWindow(_T("STATIC"), _T("\xb8\xa8\xd6\xfa\xb6\xd3\xd3\xd1\x20\x32\x3a"), WS_VISIBLE | WS_CHILD, 38, 270, 75, 20, hwnd, NULL, NULL, NULL);
        SendMessage(hCtrl, WM_SETFONT, (WPARAM)hFont, TRUE);
        g_hTab1Controls[g_nTab1Count++] = hCtrl;
        
        HWND hComboTeam2 = CreateWindow(_T("COMBOBOX"), _T(""), WS_VISIBLE | WS_CHILD | CBS_DROPDOWNLIST | WS_VSCROLL, 115, 268, 230, 200, hwnd, (HMENU)ID_COMBO_TEAMMATESKILL2, NULL, NULL);
        SendMessage(hComboTeam2, WM_SETFONT, (WPARAM)hFont, TRUE);
        PopulateSkillComboBox(hComboTeam2, g_dwTeammateSkillID2);
        g_hTab1Controls[g_nTab1Count++] = hComboTeam2;
        
        hCtrl = CreateWindow(_T("STATIC"), _T("\xca\xa9\xb7\xc5\xbc\xe4\xb8\xf4\x3a"), WS_VISIBLE | WS_CHILD, 360, 270, 75, 20, hwnd, NULL, NULL, NULL);
        SendMessage(hCtrl, WM_SETFONT, (WPARAM)hFont, TRUE);
        g_hTab1Controls[g_nTab1Count++] = hCtrl;
        
        HWND hEditTeamInt2 = CreateWindow(_T("EDIT"), _T(""), WS_VISIBLE | WS_CHILD | WS_BORDER | ES_NUMBER, 438, 268, 50, 24, hwnd, (HMENU)ID_EDIT_TEAMMATEINTERVAL2, NULL, NULL);
        SendMessage(hEditTeamInt2, WM_SETFONT, (WPARAM)hFont, TRUE);
        _sntprintf(szTmp, 16, _T("%d"), g_nTeammateSkillInterval2);
        SetWindowText(hEditTeamInt2, szTmp);
        g_hTab1Controls[g_nTab1Count++] = hEditTeamInt2;
        
        hCtrl = CreateWindow(_T("STATIC"), _T("\xc3\xeb\x20\x28\xd7\xee\xb5\xcd\x31\xc3\xeb\x29"), WS_VISIBLE | WS_CHILD, 495, 270, 115, 20, hwnd, NULL, NULL, NULL);
        SendMessage(hCtrl, WM_SETFONT, (WPARAM)hFont, TRUE);
        g_hTab1Controls[g_nTab1Count++] = hCtrl;
        
        hCtrl = CreateWindow(_T("STATIC"), _T("\xcc\xe1\xca\xbe\x3a\x20\xb8\xa8\xd6\xfa\xb6\xd3\xd3\xd1\xbc\xbc\xc4\xdc\xbb\xe1\xd4\xda\xb9\xd2\xbb\xfa\xc6\xda\xbc\xe4\xd7\xd4\xb6\xaf\xce\xaa\xb4\xa6\xd3\xda\xb4\xe6\xbb\xee\xd7\xb4\xcc\xac\xb5\xc4\xb6\xd3\xd3\xd1\xca\xa9\xb7\xc5\xa1\xa3"), WS_VISIBLE | WS_CHILD, 38, 330, 560, 24, hwnd, NULL, NULL, NULL);
        SendMessage(hCtrl, WM_SETFONT, (WPARAM)hFont, TRUE);
        g_hTab1Controls[g_nTab1Count++] = hCtrl;
        
        
        // ------------------ Tab 2: 存仓与售卖 ------------------
        // 第1行开关：启用自动存仓、启用自动售卖、启用禁卖保护、仅满包时售卖
        hCtrl = CreateWindow(_T("BUTTON"), _T("\xc6\xf4\xd3\xc3\xd7\xd4\xb6\xaf\xb4\xe6\xb2\xd6"), WS_VISIBLE | WS_CHILD | BS_AUTOCHECKBOX, 25, 45, 130, 24, hwnd, (HMENU)ID_CHECK_AUTOSTORE_BANK, NULL, NULL);
        SendMessage(hCtrl, BM_SETCHECK, g_bAutoStoreBank ? BST_CHECKED : BST_UNCHECKED, 0);
        SendMessage(hCtrl, WM_SETFONT, (WPARAM)hFont, TRUE);
        g_hTab2Controls[g_nTab2Count++] = hCtrl;

        hCtrl = CreateWindow(_T("BUTTON"), _T("\xc6\xf4\xd3\xc3\xd7\xd4\xb6\xaf\xca\xdb\xc2\xf4"), WS_VISIBLE | WS_CHILD | BS_AUTOCHECKBOX, 165, 45, 130, 24, hwnd, (HMENU)ID_CHECK_AUTOSELL_FULL, NULL, NULL);
        SendMessage(hCtrl, BM_SETCHECK, g_bAutoSellFull ? BST_CHECKED : BST_UNCHECKED, 0);
        SendMessage(hCtrl, WM_SETFONT, (WPARAM)hFont, TRUE);
        g_hTab2Controls[g_nTab2Count++] = hCtrl;

        hCtrl = CreateWindow(_T("BUTTON"), _T("\xc6\xf4\xd3\xc3\xbd\xfb\xca\xdb\xb1\xa3\xbb\xa4"), WS_VISIBLE | WS_CHILD | BS_AUTOCHECKBOX, 305, 45, 140, 24, hwnd, (HMENU)ID_CHECK_BANSELL_FILTER, NULL, NULL);
        SendMessage(hCtrl, BM_SETCHECK, g_bBanSellFilter ? BST_CHECKED : BST_UNCHECKED, 0);
        SendMessage(hCtrl, WM_SETFONT, (WPARAM)hFont, TRUE);
        g_hTab2Controls[g_nTab2Count++] = hCtrl;

        hCtrl = CreateWindow(_T("BUTTON"), _T("\xbd\xf6\xd4\xda\xc2\xfa\xb0\xfc\xca\xb1\xca\xdb\xc2\xf4"), WS_VISIBLE | WS_CHILD | BS_AUTOCHECKBOX, 455, 45, 160, 24, hwnd, (HMENU)ID_CHECK_AUTOSELL_ALL, NULL, NULL);
        SendMessage(hCtrl, BM_SETCHECK, g_bAutoSellAll ? BST_CHECKED : BST_UNCHECKED, 0);
        SendMessage(hCtrl, WM_SETFONT, (WPARAM)hFont, TRUE);
        g_hTab2Controls[g_nTab2Count++] = hCtrl;

        // 第2行：规则说明提示
        hCtrl = CreateWindow(_T("STATIC"), _T("\xcb\xb5\xc3\xf7\xa3\xba\xd3\xc5\xcf\xc8\xb4\xe6\xb2\xd6\x20\x2d\x3e\x20\xca\xdb\xc2\xf4\xce\xb4\xb4\xe6\xb2\xd6\xbf\xe2\xc7\xd2\xce\xb4\xbd\xfb\xc2\xf4\xce\xef\xc6\xb7\xa3\xac\xc3\xbf\xb8\xf4\x33\x35\x30\x6d\x73\xd6\xf0\xd2\xbb\xb4\xa6\xc0\xed\xa1\xa3"), WS_VISIBLE | WS_CHILD, 25, 75, 590, 20, hwnd, NULL, NULL, NULL);
        SendMessage(hCtrl, WM_SETFONT, (WPARAM)hFont, TRUE);
        g_hTab2Controls[g_nTab2Count++] = hCtrl;

        // 左侧：自动存仓物品列表 (每行一个)
        hCtrl = CreateWindow(_T("BUTTON"), _T("\xd7\xd4\xb6\xaf\xb4\xe6\xb2\xd6\xce\xef\xc6\xb7\xc1\xd0\xb1\xed\x20\x28\xc3\xbf\xd0\xd0\xd2\xbb\xb8\xf6\x29"), WS_VISIBLE | WS_CHILD | BS_GROUPBOX, 22, 100, 290, 365, hwnd, NULL, NULL, NULL);
        SendMessage(hCtrl, WM_SETFONT, (WPARAM)hFont, TRUE);
        g_hTab2Controls[g_nTab2Count++] = hCtrl;

        HWND hStoreBankEdit = CreateWindow(_T("EDIT"), _T(""), WS_VISIBLE | WS_CHILD | WS_BORDER | ES_MULTILINE | ES_AUTOVSCROLL | ES_WANTRETURN | WS_VSCROLL, 32, 126, 270, 328, hwnd, (HMENU)ID_EDIT_STOREBANK_LIST, NULL, NULL);
        SendMessage(hStoreBankEdit, WM_SETFONT, (WPARAM)hFont, TRUE);
        SetWindowText(hStoreBankEdit, g_szStoreBankList);
        g_hTab2Controls[g_nTab2Count++] = hStoreBankEdit;

        // 右侧：禁卖保护物品列表 (每行一个)
        hCtrl = CreateWindow(_T("BUTTON"), _T("\xbd\xfb\xc2\xf4\xb1\xa3\xbb\xa4\xce\xef\xc6\xb7\xc1\xd0\xb1\xed\x20\x28\xc3\xbf\xd0\xd0\xd2\xbb\xb8\xf6\x29"), WS_VISIBLE | WS_CHILD | BS_GROUPBOX, 325, 100, 295, 365, hwnd, NULL, NULL, NULL);
        SendMessage(hCtrl, WM_SETFONT, (WPARAM)hFont, TRUE);
        g_hTab2Controls[g_nTab2Count++] = hCtrl;

        HWND hBanSellEdit = CreateWindow(_T("EDIT"), _T(""), WS_VISIBLE | WS_CHILD | WS_BORDER | ES_MULTILINE | ES_AUTOVSCROLL | ES_WANTRETURN | WS_VSCROLL, 335, 126, 275, 328, hwnd, (HMENU)ID_EDIT_BANSELL_LIST, NULL, NULL);
        SendMessage(hBanSellEdit, WM_SETFONT, (WPARAM)hFont, TRUE);
        SetWindowText(hBanSellEdit, g_szBanSellList);
        g_hTab2Controls[g_nTab2Count++] = hBanSellEdit;
        
        ShowTabControls(0);
        
        // Toggle button text based on g_bCheat (master switch)
        extern BOOL g_bCheat;
        LPCTSTR pszToggleText = g_bCheat ? _T("\xbd\xe1\xca\xf8\xb9\xd2\xbb\xfa") : _T("\xbf\xaa\xca\xbc\xb9\xd2\xbb\xfa");
        HWND hBtnToggle = CreateWindow(_T("BUTTON"), pszToggleText, WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON, 185, 498, 125, 34, hwnd, (HMENU)ID_BUTTON_TOGGLE, NULL, NULL);
        SendMessage(hBtnToggle, WM_SETFONT, (WPARAM)hFont, TRUE);
        
        HWND hBtnSave = CreateWindow(_T("BUTTON"), _T("\xb1\xa3\xb4\xe6\xc9\xe8\xd6\xc3"), WS_VISIBLE | WS_CHILD, 335, 498, 125, 34, hwnd, (HMENU)ID_BUTTON_SAVE, NULL, NULL);
        SendMessage(hBtnSave, WM_SETFONT, (WPARAM)hFont, TRUE);
        
        // 启动定时器：每500ms同步按钮文字与 g_bCheat 实际状态（死亡复活后后台恢复挂机时刷新UI）
        SetTimer(hwnd, 1, 500, NULL);
        break;
    }
    
    case WM_NOTIFY: {
        LPNMHDR pnmhdr = (LPNMHDR)lParam;
        if (pnmhdr->idFrom == ID_TABCONTROL && pnmhdr->code == TCN_SELCHANGE) {
            int iSel = TabCtrl_GetCurSel(pnmhdr->hwndFrom);
            ShowTabControls(iSel);
        }
        break;
    }
    
    case WM_COMMAND: {
        int id = LOWORD(wParam);
        if (id == ID_BUTTON_GETCURPOS) {
            if (g_pMainChar && g_pMainChar->m_pObject) {
                CXiahCharObject* pCharObj = (CXiahCharObject*)g_pMainChar->m_pObject;
                WORD curX = 0, curY = 0;
                pCharObj->GetPosition(curX, curY);
                g_wHomeX = curX;
                g_wHomeY = curY;
                TCHAR szTmpPos[32];
                _sntprintf(szTmpPos, 32, _T("%u"), g_wHomeX);
                SetWindowText(GetDlgItem(hwnd, ID_EDIT_HOMEX), szTmpPos);
                _sntprintf(szTmpPos, 32, _T("%u"), g_wHomeY);
                SetWindowText(GetDlgItem(hwnd, ID_EDIT_HOMEY), szTmpPos);
            }
            break;
        } else if (id == ID_BUTTON_TOGGLE || id == ID_BUTTON_SAVE) {
            TCHAR szTmp[32];
            
            g_bAutoAttack = (SendMessage(GetDlgItem(hwnd, ID_CHECK_AUTOATTACK), BM_GETCHECK, 0, 0) == BST_CHECKED);
            g_bAutoLoot = (SendMessage(GetDlgItem(hwnd, ID_CHECK_AUTOLOOT), BM_GETCHECK, 0, 0) == BST_CHECKED);
            g_nAttackMode = (SendMessage(GetDlgItem(hwnd, ID_RADIO_SKILL), BM_GETCHECK, 0, 0) == BST_CHECKED) ? 1 : 0;
            g_bAutoHP = (SendMessage(GetDlgItem(hwnd, ID_CHECK_AUTOHP), BM_GETCHECK, 0, 0) == BST_CHECKED);
            g_bAutoBuyHP = (SendMessage(GetDlgItem(hwnd, ID_CHECK_AUTOBUYHP), BM_GETCHECK, 0, 0) == BST_CHECKED);
            g_bAutoMP = (SendMessage(GetDlgItem(hwnd, ID_CHECK_AUTOMP), BM_GETCHECK, 0, 0) == BST_CHECKED);
            g_bAutoBuyMP = (SendMessage(GetDlgItem(hwnd, ID_CHECK_AUTOBUYMP), BM_GETCHECK, 0, 0) == BST_CHECKED);
            g_bAutoPet = (SendMessage(GetDlgItem(hwnd, ID_CHECK_AUTOPET), BM_GETCHECK, 0, 0) == BST_CHECKED);
            
            GetWindowText(GetDlgItem(hwnd, ID_EDIT_HPPOTIONNAME), g_szHPPotionName, 32);
            GetWindowText(GetDlgItem(hwnd, ID_EDIT_MPPOTIONNAME), g_szMPPotionName, 32);
            
            GetWindowText(GetDlgItem(hwnd, ID_EDIT_HPPERCENT), szTmp, 32);
            g_nHPPercent = _ttoi(szTmp);
            if (g_nHPPercent < 0) g_nHPPercent = 0;
            if (g_nHPPercent > 100) g_nHPPercent = 100;
            
            GetWindowText(GetDlgItem(hwnd, ID_EDIT_MPPERCENT), szTmp, 32);
            g_nMPPercent = _ttoi(szTmp);
            if (g_nMPPercent < 0) g_nMPPercent = 0;
            if (g_nMPPercent > 100) g_nMPPercent = 100;
            
            GetWindowText(GetDlgItem(hwnd, ID_EDIT_PETWILDRATE), szTmp, 32);
            g_nPetWildRate = _ttoi(szTmp);
            
            GetWindowText(GetDlgItem(hwnd, ID_EDIT_PETFOODID), szTmp, 32);
            g_dwPetFoodID = (DWORD)_ttoi64(szTmp);
            
            GetWindowText(GetDlgItem(hwnd, ID_EDIT_PETSEALID), szTmp, 32);
            g_dwPetSealID = (DWORD)_ttoi64(szTmp);
            
            GetWindowText(GetDlgItem(hwnd, ID_EDIT_FILTERLIST), g_szFilterList, 4096);
            
            HWND hCombo;
            int curSel;
            
            hCombo = GetDlgItem(hwnd, ID_COMBO_ATTACKSKILL);
            curSel = SendMessage(hCombo, CB_GETCURSEL, 0, 0);
            if (curSel != CB_ERR) {
                g_dwAttackSkillID = (DWORD)SendMessage(hCombo, CB_GETITEMDATA, curSel, 0);
            }
            GetWindowText(GetDlgItem(hwnd, ID_EDIT_ATTACKINTERVAL), szTmp, 32);
            g_nAttackSkillInterval = _ttoi(szTmp);
            if (g_nAttackSkillInterval < 1) g_nAttackSkillInterval = 1;
            
            hCombo = GetDlgItem(hwnd, ID_COMBO_BUFFSKILL1);
            curSel = SendMessage(hCombo, CB_GETCURSEL, 0, 0);
            if (curSel != CB_ERR) {
                g_dwBuffSkillID1 = (DWORD)SendMessage(hCombo, CB_GETITEMDATA, curSel, 0);
            }
            GetWindowText(GetDlgItem(hwnd, ID_EDIT_BUFFINTERVAL1), szTmp, 32);
            g_nBuffSkillInterval1 = _ttoi(szTmp);
            if (g_nBuffSkillInterval1 < 1) g_nBuffSkillInterval1 = 1;
            
            hCombo = GetDlgItem(hwnd, ID_COMBO_BUFFSKILL2);
            curSel = SendMessage(hCombo, CB_GETCURSEL, 0, 0);
            if (curSel != CB_ERR) {
                g_dwBuffSkillID2 = (DWORD)SendMessage(hCombo, CB_GETITEMDATA, curSel, 0);
            }
            GetWindowText(GetDlgItem(hwnd, ID_EDIT_BUFFINTERVAL2), szTmp, 32);
            g_nBuffSkillInterval2 = _ttoi(szTmp);
            if (g_nBuffSkillInterval2 < 1) g_nBuffSkillInterval2 = 1;
            
            hCombo = GetDlgItem(hwnd, ID_COMBO_TEAMMATESKILL1);
            curSel = SendMessage(hCombo, CB_GETCURSEL, 0, 0);
            if (curSel != CB_ERR) {
                g_dwTeammateSkillID1 = (DWORD)SendMessage(hCombo, CB_GETITEMDATA, curSel, 0);
            }
            GetWindowText(GetDlgItem(hwnd, ID_EDIT_TEAMMATEINTERVAL1), szTmp, 32);
            g_nTeammateSkillInterval1 = _ttoi(szTmp);
            if (g_nTeammateSkillInterval1 < 1) g_nTeammateSkillInterval1 = 1;
            
            hCombo = GetDlgItem(hwnd, ID_COMBO_TEAMMATESKILL2);
            curSel = SendMessage(hCombo, CB_GETCURSEL, 0, 0);
            if (curSel != CB_ERR) {
                g_dwTeammateSkillID2 = (DWORD)SendMessage(hCombo, CB_GETITEMDATA, curSel, 0);
            }
            GetWindowText(GetDlgItem(hwnd, ID_EDIT_TEAMMATEINTERVAL2), szTmp, 32);
            g_nTeammateSkillInterval2 = _ttoi(szTmp);
            if (g_nTeammateSkillInterval2 < 1) g_nTeammateSkillInterval2 = 1;
            
            g_bAutoStoreBank = (SendMessage(GetDlgItem(hwnd, ID_CHECK_AUTOSTORE_BANK), BM_GETCHECK, 0, 0) == BST_CHECKED);
            GetWindowText(GetDlgItem(hwnd, ID_EDIT_STOREBANK_LIST), g_szStoreBankList, 4096);

            g_bAutoSellFull = (SendMessage(GetDlgItem(hwnd, ID_CHECK_AUTOSELL_FULL), BM_GETCHECK, 0, 0) == BST_CHECKED);
            g_bAutoSellAll = (SendMessage(GetDlgItem(hwnd, ID_CHECK_AUTOSELL_ALL), BM_GETCHECK, 0, 0) == BST_CHECKED);
            g_bBanSellFilter = (SendMessage(GetDlgItem(hwnd, ID_CHECK_BANSELL_FILTER), BM_GETCHECK, 0, 0) == BST_CHECKED);
            GetWindowText(GetDlgItem(hwnd, ID_EDIT_BANSELL_LIST), g_szBanSellList, 4096);
            
            // 读取中心点配置 & 定点攻击 & 活动半径
            g_bUseHomePoint = (SendMessage(GetDlgItem(hwnd, ID_CHECK_USEHOME), BM_GETCHECK, 0, 0) == BST_CHECKED);
            GetWindowText(GetDlgItem(hwnd, ID_EDIT_IDLERETURN), szTmp, 32);
            g_nIdleReturnSec = _ttoi(szTmp);
            if (g_nIdleReturnSec < 0) g_nIdleReturnSec = 0;

            GetWindowText(GetDlgItem(hwnd, ID_EDIT_HOMERADIUS), szTmp, 32);
            g_nHomeRadius = _ttoi(szTmp);
            if (g_nHomeRadius < 5) g_nHomeRadius = 5;
            
            GetWindowText(GetDlgItem(hwnd, ID_EDIT_HOMEX), szTmp, 32);
            g_wHomeX = (WORD)_ttoi(szTmp);
            GetWindowText(GetDlgItem(hwnd, ID_EDIT_HOMEY), szTmp, 32);
            g_wHomeY = (WORD)_ttoi(szTmp);
            g_bFixedPointAttack = (SendMessage(GetDlgItem(hwnd, ID_CHECK_FIXEDPOINT), BM_GETCHECK, 0, 0) == BST_CHECKED);
            
            ApplyCheatConfigToMainChar();
            SaveCheatConfig();
            
            if (id == ID_BUTTON_TOGGLE) {
                // Only Toggle button controls auto-hunt on/off
                extern BOOL g_bCheat;
                extern BOOL g_bCheatEtc;
                g_bCheat = !g_bCheat;
                // 物理攻击模式(g_nAttackMode==0): g_bCheatEtc=FALSE → 走 else if(g_bCheat) 分支执行 ProcessAutoAttack
                // 技能攻击模式(g_nAttackMode==1): g_bCheatEtc=TRUE  → 走 if(g_bCheatEtc) 分支执行原地施法
                extern int g_nAttackMode;
                g_bCheatEtc = (g_nAttackMode == 1) ? g_bCheat : FALSE;
                
                // Update button text to reflect current state
                HWND hBtnToggle = GetDlgItem(hwnd, ID_BUTTON_TOGGLE);
                if (hBtnToggle) {
                    SetWindowText(hBtnToggle, g_bCheat 
                        ? _T("\xbd\xe1\xca\xf8\xb9\xd2\xbb\xfa")
                        : _T("\xbf\xaa\xca\xbc\xb9\xd2\xbb\xfa"));
                }
                
                // 开始挂机时设置标记，让主循环第一帧记录中心点坐标
                if (g_bCheat) {
                    g_dwLastAttackTime = 0;
                    g_bReturningHome = FALSE;
                    // 若玩家未手动配置/抓取中心点(均为0)，才置0让主循环自动记录当前坐标；若已设置则保留
                    if (g_wHomeX == 0 && g_wHomeY == 0) {
                        g_wHomeX = 0;
                        g_wHomeY = 0;
                    }
                    TriggerImmediateBagScan();
                }
            } else {
                // Save button: save config, sync mode if hunting (Do NOT close window)
                extern BOOL g_bCheat;
                extern BOOL g_bCheatEtc;
                extern int g_nAttackMode;
                if (g_bCheat) {
                    // 挂机中切换模式：实时同步 g_bCheatEtc
                    g_bCheatEtc = (g_nAttackMode == 1) ? TRUE : FALSE;
                    TriggerImmediateBagScan();
                }
            }
        }
        break;
    }
    
    case WM_TIMER: {
        // 定时同步按钮文字：当 g_bCheat 被外部逻辑（如死亡复活恢复）改变时，按钮文字自动刷新
        extern BOOL g_bCheat;
        HWND hBtnToggle = GetDlgItem(hwnd, ID_BUTTON_TOGGLE);
        if (hBtnToggle) {
            TCHAR szCur[32] = {0};
            GetWindowText(hBtnToggle, szCur, 32);
            LPCTSTR pszExpected = g_bCheat
                ? _T("\xbd\xe1\xca\xf8\xb9\xd2\xbb\xfa")
                : _T("\xbf\xaa\xca\xbc\xb9\xd2\xbb\xfa");
            if (_tcscmp(szCur, pszExpected) != 0) {
                SetWindowText(hBtnToggle, pszExpected);
            }
        }
        break;
    }
    
    case WM_CLOSE:
        DestroyWindow(hwnd);
        break;
        
    case WM_DESTROY:
        KillTimer(hwnd, 1);
        if (hFont) DeleteObject(hFont);
        g_hwndConfig = NULL;
        PostQuitMessage(0);
        break;
        
    default:
        return DefWindowProc(hwnd, msg, wParam, lParam);
    }
    return 0;
}

DWORD WINAPI ConfigThreadProc(LPVOID lpParam) {
    INITCOMMONCONTROLSEX icex;
    icex.dwSize = sizeof(INITCOMMONCONTROLSEX);
    icex.dwICC = ICC_TAB_CLASSES;
    InitCommonControlsEx(&icex);

    HWND hwndParent = (HWND)lpParam;
    HINSTANCE hInst = GetModuleHandle(NULL);
    
    WNDCLASSEX wc = {0};
    wc.cbSize = sizeof(WNDCLASSEX);
    wc.style = CS_HREDRAW | CS_VREDRAW;
    wc.lpfnWndProc = ConfigWndProc;
    wc.hInstance = hInst;
    wc.hCursor = LoadCursor(NULL, IDC_ARROW);
    wc.hbrBackground = (HBRUSH)(COLOR_BTNFACE + 1);
    wc.lpszClassName = _T("XiahConfigClass");
    
    RegisterClassEx(&wc);
    
    int w = 660;
    int h = 585;
    int x = CW_USEDEFAULT;
    int y = CW_USEDEFAULT;
    if (hwndParent) {
        RECT rect;
        GetWindowRect(hwndParent, &rect);
        x = rect.left + ((rect.right - rect.left) - w) / 2;
        y = rect.top + ((rect.bottom - rect.top) - h) / 2;
    }
    
    TCHAR szTitle[128] = {0};
    LPCTSTR pszCharName = (LPCTSTR)g_MainCharInfo.m_szNickName;
    if (pszCharName && pszCharName[0] != _T('\0')) {
        _sntprintf(szTitle, 128, _T("\xb9\xd2\xbb\xfa\xb8\xa8\xd6\xfa - [%s]"), pszCharName);
    } else {
        _tcscpy_s(szTitle, 128, _T("\xb9\xd2\xbb\xfa\xb8\xa8\xd6\xfa"));
    }
    
    g_hwndConfig = CreateWindowEx(WS_EX_DLGMODALFRAME, _T("XiahConfigClass"), szTitle,
                                  WS_POPUP | WS_CAPTION | WS_SYSMENU | WS_VISIBLE,
                                  x, y, w, h, hwndParent, NULL, hInst, NULL);
    
    if (!g_hwndConfig) {
        UnregisterClass(_T("XiahConfigClass"), hInst);
        return 0;
    }
    
    ShowWindow(g_hwndConfig, SW_SHOW);
    UpdateWindow(g_hwndConfig);
    
    MSG msg;
    while (GetMessage(&msg, NULL, 0, 0)) {
        if (!IsDialogMessage(g_hwndConfig, &msg)) {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }
    
    UnregisterClass(_T("XiahConfigClass"), hInst);
    return 0;
}

void ShowCheatConfigWindow(HWND hwndParent) {
    if (g_hwndConfig && IsWindow(g_hwndConfig)) {
        SendMessage(g_hwndConfig, WM_CLOSE, 0, 0);
        return;
    }
    
    if (g_hConfigThread) {
        CloseHandle(g_hConfigThread);
        g_hConfigThread = NULL;
    }
    
    g_hConfigThread = CreateThread(NULL, 0, ConfigThreadProc, (LPVOID)hwndParent, 0, NULL);
}

BOOL IsStoreBankFiltered(const TCHAR* szItemName) {
    if (!szItemName || _tcslen(szItemName) == 0) return FALSE;
    TCHAR szListCopy[4096];
    _tcscpy_s(szListCopy, 4096, g_szStoreBankList);
    TCHAR* context = NULL;
    TCHAR* token = _tcstok_s(szListCopy, _T("\r\n|,"), &context);
    while (token != NULL) {
        TCHAR szTrimmed[128] = {0};
        int len = _tcslen(token);
        int start = 0;
        while (start < len && (token[start] == _T(' ') || token[start] == _T('\t'))) {
            start++;
        }
        int end = len - 1;
        while (end >= start && (token[end] == _T(' ') || token[end] == _T('\t'))) {
            end--;
        }
        if (end >= start) {
            _tcsncpy(szTrimmed, token + start, end - start + 1);
            szTrimmed[end - start + 1] = _T('\0');
            if (_tcscmp(szTrimmed, szItemName) == 0 || (_tcslen(szTrimmed) >= 2 && _tcsstr(szItemName, szTrimmed) != NULL)) {
                return TRUE;
            }
        }
        token = _tcstok_s(NULL, _T("\r\n|,"), &context);
    }
    return FALSE;
}

BOOL IsBanSellFiltered(const TCHAR* szItemName) {
    if (!szItemName || _tcslen(szItemName) == 0) return FALSE;
    TCHAR szListCopy[4096];
    _tcscpy_s(szListCopy, 4096, g_szBanSellList);
    TCHAR* context = NULL;
    TCHAR* token = _tcstok_s(szListCopy, _T("\r\n|,"), &context);
    while (token != NULL) {
        TCHAR szTrimmed[128] = {0};
        int len = _tcslen(token);
        int start = 0;
        while (start < len && (token[start] == _T(' ') || token[start] == _T('\t'))) {
            start++;
        }
        int end = len - 1;
        while (end >= start && (token[end] == _T(' ') || token[end] == _T('\t'))) {
            end--;
        }
        if (end >= start) {
            _tcsncpy(szTrimmed, token + start, end - start + 1);
            szTrimmed[end - start + 1] = _T('\0');
            if (_tcscmp(szTrimmed, szItemName) == 0 || (_tcslen(szTrimmed) >= 2 && _tcsstr(szItemName, szTrimmed) != NULL)) {
                return TRUE;
            }
        }
        token = _tcstok_s(NULL, _T("\r\n|,"), &context);
    }
    return FALSE;
}

// 统计主背包空闲格数（0~35格）
static int GetMainBagFreeSlots() {
    int nFree = 0;
    CSack* pSack = g_MainCharInfo.m_pMySack[0];
    if (pSack) {
        for (int i = 0; i < 36; ++i) {
            if (pSack->FindSackItemByPos(i) == NULL) {
                nFree++;
            }
        }
    }
    return nFree;
}

static DWORD s_dwLastActionTime = 0;
static DWORD s_dwLastScanTime = 0;
static DWORD s_dwLastActionItemID = 0;
static BOOL s_bSellingProcessActive = FALSE;
static BOOL s_bForceScanNow = TRUE;
static BOOL s_bLastCheatState = FALSE;

void TriggerImmediateBagScan() {
    s_bForceScanNow = TRUE;
    s_bSellingProcessActive = TRUE;
    s_dwLastScanTime = 0;
    s_dwLastActionItemID = 0;
}

void ProcessAutoSell() {
    if (!g_bCheat) {
        s_bSellingProcessActive = FALSE;
        s_bLastCheatState = FALSE;
        return;
    }

    // 辅助开启瞬间边缘检测：刚开启辅助时立即检查背包并清理
    if (!s_bLastCheatState && g_bCheat) {
        s_bForceScanNow = TRUE;
        s_bSellingProcessActive = TRUE;
        s_dwLastScanTime = 0;
        s_dwLastActionItemID = 0;
        g_MainCharInfo.ShowHelpMessage(_T("\xbf\xaa\xc6\xf4\xb8\xa8\xd6\xfa\xa3\xac\xd5\xfd\xd4\xda\xbc\xec\xb2\xe9\xb1\xb3\xb0\xfc\x2e\x2e\x2e"), TEXTEFFECT_COLOR_GAIN);
    }
    s_bLastCheatState = g_bCheat;

    if (!g_bAutoStoreBank && !g_bAutoSellFull) {
        s_bSellingProcessActive = FALSE;
        return;
    }

    DWORD dwNow = GetTickCount();

    // 动作间隔：每次发送封包后等待 300ms，保证网络与服务端稳定
    if (dwNow - s_dwLastActionTime < 300) {
        return;
    }

    if (!s_bSellingProcessActive) {
        // 非活动处理期，默认每 2 秒巡检一次
        if (!s_bForceScanNow && dwNow - s_dwLastScanTime < 2000) {
            return;
        }

        BOOL bShouldTrigger = FALSE;
        if (s_bForceScanNow) {
            bShouldTrigger = TRUE;
            s_bForceScanNow = FALSE;
        } else {
            if (g_bAutoStoreBank) {
                bShouldTrigger = TRUE;
            }
            if (g_bAutoSellFull) {
                if (!g_bAutoSellAll) {
                    // 未勾选"仅满包时售卖" -> 随时自动清理未禁卖物品
                    bShouldTrigger = TRUE;
                } else if (GetMainBagFreeSlots() <= 4) {
                    // 勾选了"仅满包时售卖" -> 空格<=4(一件装备放不下)即触发清理
                    bShouldTrigger = TRUE;
                }
            }
        }

        if (bShouldTrigger) {
            s_bSellingProcessActive = TRUE;
            s_dwLastScanTime = dwNow;
        } else {
            return;
        }
    }

    BOOL bFoundActionable = FALSE;
    CSack* pSacks[3] = { g_MainCharInfo.m_pMySack[0], g_MainCharInfo.m_pMySack[1], g_MainCharInfo.m_pMySack[2] };

    for (int sackIdx = 0; sackIdx < 3; ++sackIdx) {
        CSack* pSack = pSacks[sackIdx];
        if (!pSack) continue;

        // 背包全部 36 个格子(0~35)均为包裹格子，从 0 开始完整扫描
        for (int i = 0; i < 36; ++i) {
            XiahItem::sItemInfo* pItem = pSack->FindSackItemByPos(i);
            if (!pItem) continue;

            // 避免对同一个物品连续瞬间重复发送
            if (pItem->m_dwItemID == s_dwLastActionItemID && dwNow - s_dwLastActionTime < 1000) {
                continue;
            }

            BYTE bSackID = pItem->m_bSackCount + 1; // 必须是 1, 2, 3
            BYTE bSackPos = pItem->m_bSackPos;
            LPCTSTR szItemName = (LPCTSTR)pItem->m_szName;

            // 1. 优先自动存仓
            if (g_bAutoStoreBank && IsStoreBankFiltered(szItemName)) {
                SendCS_EC_DRAWINBANK_REQ(
                    g_MainCharInfo.m_dwObjectID,
                    pItem->m_dwItemID,
                    bSackID,
                    bSackPos,
                    255, // 255 表示服务端自动在 0~35 寻找空位
                    pItem->m_dwAmount ? pItem->m_dwAmount : 1
                );

                TCHAR szTip[128];
                _sntprintf(szTip, 128, _T("\xd7\xd4\xb6\xaf\xb4\xe6\xb2\xd6\x3a\x20\x25\x73"), szItemName);
                g_MainCharInfo.ShowHelpMessage(szTip, TEXTEFFECT_COLOR_GAIN);

                s_dwLastActionItemID = pItem->m_dwItemID;
                s_dwLastActionTime = dwNow;
                bFoundActionable = TRUE;
                break;
            }

            // 2. 售卖未存仓库、未禁卖的物品
            // 命中了存仓名单的物品即使暂时未存入（如仓库已满），也坚决不能被当做垃圾卖掉！
            BOOL bIsInStoreBankList = g_bAutoStoreBank && IsStoreBankFiltered(szItemName);
            if (bIsInStoreBankList) {
                continue;
            }

            if (g_bAutoSellFull) {
                // 禁卖保护检查
                BOOL bIsBanSell = g_bBanSellFilter && IsBanSellFiltered(szItemName);
                if (bIsBanSell) {
                    continue; // 禁卖保护，保留不卖
                }

                // 不可售卖的基础类型过滤（任务、活动道具等，商店不收）
                BYTE type = pItem->m_bItemType;
                if (type == ITEMTYPE_QUEST || type == ITEMTYPE_PORTAL || 
                    type == ITEMTYPE_EVENT || type == ITEMTYPE_LOTTO || type == ITEMTYPE_SOCKET) {
                    continue;
                }

                // 强化过的装备防误卖保护
                if (type >= ITEMTYPE_WEAPON && type <= ITEMTYPE_NECKLACE) {
                    if (pItem->m_bModifyCnt >= 1) {
                        continue;
                    }
                }

                // 未存仓库、未禁卖 -> 发送远程售卖封包
                SendCS_EC_SELLITEM_REQ(984, pItem->m_dwItemID, bSackID, bSackPos);

                TCHAR szTip[128];
                _sntprintf(szTip, 128, _T("\xd7\xd4\xb6\xaf\xca\xdb\xc2\xf4\x3a\x20\x25\x73"), szItemName);
                g_MainCharInfo.ShowHelpMessage(szTip, TEXTEFFECT_COLOR_GAIN);

                s_dwLastActionItemID = pItem->m_dwItemID;
                s_dwLastActionTime = dwNow;
                bFoundActionable = TRUE;
                break;
            }
        }

        if (bFoundActionable) {
            break;
        }
    }

    if (!bFoundActionable) {
        s_bSellingProcessActive = FALSE;
        s_dwLastScanTime = dwNow;
        s_dwLastActionItemID = 0;
    }
}

DWORD GetSkillCD(DWORD dwMugongID) {
    if (dwMugongID == g_dwAttackSkillID && g_dwAttackSkillID > 0) {
        return (DWORD)g_nAttackSkillInterval * 1000;
    }
    if (dwMugongID == g_dwBuffSkillID1 && g_dwBuffSkillID1 > 0) {
        return (DWORD)g_nBuffSkillInterval1 * 1000;
    }
    if (dwMugongID == g_dwBuffSkillID2 && g_dwBuffSkillID2 > 0) {
        return (DWORD)g_nBuffSkillInterval2 * 1000;
    }
    if (dwMugongID == g_dwTeammateSkillID1 && g_dwTeammateSkillID1 > 0) {
        return (DWORD)g_nTeammateSkillInterval1 * 1000;
    }
    if (dwMugongID == g_dwTeammateSkillID2 && g_dwTeammateSkillID2 > 0) {
        return (DWORD)g_nTeammateSkillInterval2 * 1000;
    }
    
    // Default fallback values for manual casting of specific skills:
    if (dwMugongID == 32 || dwMugongID == 36 || dwMugongID == 34 || dwMugongID == 180 || dwMugongID == 181) {
        return 1500; // Attack skills
    }
    if (dwMugongID == 39 || dwMugongID == 30 || dwMugongID == 41 || dwMugongID == 69 || dwMugongID == 68 || dwMugongID == 124 || dwMugongID == 129) {
        return 3000; // Buff/utility skills
    }
    
    return 1500; // Default fallback
}

// -------------------------------------------------------------
// 防卡墙与寻怪寻路模块实现
// -------------------------------------------------------------
#include <vector>
#include <queue>
#include <cmath>
#include <algorithm>
#include "XiahMap.h"

#define MAX_STUCK_MONSTERS 64
struct SStuckMonsterInfo {
    DWORD dwObjectID;
    DWORD dwExpireTime;
};
static SStuckMonsterInfo g_StuckMonsters[MAX_STUCK_MONSTERS];
static int g_nStuckCount = 0;

void AddStuckMonster(DWORD dwObjectID, DWORD dwDurationMs) {
    if (dwObjectID == 0) return;
    DWORD now = GetTickCount();
    for (int i = 0; i < g_nStuckCount; ++i) {
        if (g_StuckMonsters[i].dwObjectID == dwObjectID) {
            g_StuckMonsters[i].dwExpireTime = now + dwDurationMs;
            return;
        }
    }
    if (g_nStuckCount < MAX_STUCK_MONSTERS) {
        g_StuckMonsters[g_nStuckCount].dwObjectID = dwObjectID;
        g_StuckMonsters[g_nStuckCount].dwExpireTime = now + dwDurationMs;
        g_nStuckCount++;
    } else {
        // 循环覆盖最老记录
        g_StuckMonsters[0].dwObjectID = dwObjectID;
        g_StuckMonsters[0].dwExpireTime = now + dwDurationMs;
    }
}

bool IsMonsterStuck(DWORD dwObjectID) {
    if (dwObjectID == 0) return false;
    DWORD now = GetTickCount();
    for (int i = 0; i < g_nStuckCount; ++i) {
        if (g_StuckMonsters[i].dwObjectID == dwObjectID) {
            if (now < g_StuckMonsters[i].dwExpireTime) {
                return true;
            } else {
                g_StuckMonsters[i] = g_StuckMonsters[g_nStuckCount - 1];
                g_nStuckCount--;
                i--;
            }
        }
    }
    return false;
}

// 检测逻辑坐标 (x, y) 是否被地图 3D 模型（大树、枯树桩、石头、建筑物等）的物理碰撞盒阻挡
bool IsMapObjectBlocked(int x, int y) {
    if (!XiahMap::g_XiahMap.m_pMapRender) return false;
    if (x < 5 || y < 5 || x >= 2043 || y >= 2043) return true;

    XiahGameEngine::Map::MAPRENDER_MAPOBJECTLIST *pObjectList = NULL;
    if (!XiahMap::g_XiahMap.m_pMapRender->QueryMeshblockObjectList((WORD)x, (WORD)y, &pObjectList))
        return false;
    if (!pObjectList || pObjectList->empty())
        return false;

    float worldX = (float)x;
    float worldZ = -(float)y;
    const float fRadius = 0.8f; // 角色碰撞半宽容差

    // 获取角色当前高度，避免与天上或地下物体误相交
    float charY = (g_pMainChar && g_pMainChar->m_pObject) 
        ? ((CXiahCharObject*)g_pMainChar->m_pObject)->m_Position.y 
        : 0.0f;

    for (XiahGameEngine::Map::MAPRENDER_MAPOBJECTLIST::iterator it = pObjectList->begin(); it != pObjectList->end(); ++it) {
        XiahGameEngine::Map::CMapObjectRender *pObj = *it;
        if (!pObj) continue;
        int boxCount = pObj->GetCollideBoxCount();
        if (boxCount <= 0) continue;
        BBoxOBB3* boxList = pObj->GetCollideBoxList();
        if (!boxList) continue;

        for (int i = 0; i < boxCount; ++i) {
            BBoxOBB3* pBox = boxList + i;
            // 高度容差检查（高度差在合理站立范围）
            if (charY < pBox->m_BBoxAABB.m_vMin.y - 6.0f || charY > pBox->m_BBoxAABB.m_vMax.y + 6.0f)
                continue;

            // 2D 投影包围盒重叠检测（大树、枯树桩、石头等实体）
            if ((worldX + fRadius) >= pBox->m_BBoxAABB.m_vMin.x &&
                (worldX - fRadius) <= pBox->m_BBoxAABB.m_vMax.x &&
                (worldZ + fRadius) >= pBox->m_BBoxAABB.m_vMin.z &&
                (worldZ - fRadius) <= pBox->m_BBoxAABB.m_vMax.z) {
                return true; // 被大树、树桩或模型障碍物阻挡
            }
        }
    }
    return false;
}

// 检查地图网格是否可行走（包含地表 0x01 阻挡属性，以及大树、枯树桩、建筑等 3D 碰撞盒）
bool IsMapCellWalkable(int x, int y) {
    if (x < 5 || y < 5 || x >= 2043 || y >= 2043) return false;
    if (XiahMap::g_Map_Attri.Get_Attr(x, y) == 0x01) return false;
    if (IsMapObjectBlocked(x, y)) return false;
    return true;
}

// 寻找绕开障碍物的侧切或后撤脱困点 (Side Detour / Backstep Waypoint)
// nStepMode: 0=斜后退步拉开树干粘滞; 1=垂直大角度侧切绕出大树
bool FindDetourWaypoint(int curX, int curY, int targetX, int targetY, int& outSideX, int& outSideY, int nStepMode) {
    float dx = (float)(targetX - curX);
    float dy = (float)(targetY - curY);
    float len = sqrtf(dx * dx + dy * dy);
    if (len < 0.001f) return false;

    // 前进单位向量
    float ux = dx / len;
    float uy = dy / len;

    // 垂直法向量（左侧与右侧）
    float lx = -uy, ly = ux;
    float rx = uy, ry = -ux;

    if (nStepMode == 0) {
        // 模式0：斜后退步（向反方向后退 2.5 码，并尝试向左或右轻微偏斜 1.5 码，脱离大树树干碰撞吸附）
        const float backOffsets[][2] = {
            { -ux * 2.5f + lx * 1.5f, -uy * 2.5f + ly * 1.5f }, // 左后撤
            { -ux * 2.5f + rx * 1.5f, -uy * 2.5f + ry * 1.5f }, // 右后撤
            { -ux * 3.0f,            -uy * 3.0f }              // 正后撤
        };
        for (int i = 0; i < 3; ++i) {
            int bx = curX + (int)floorf(backOffsets[i][0] + 0.5f);
            int by = curY + (int)floorf(backOffsets[i][1] + 0.5f);
            if (IsMapCellWalkable(bx, by)) {
                outSideX = bx;
                outSideY = by;
                return true;
            }
        }
    }

    // 模式1：大角度垂直侧切（向左或向右大步跨出 4~6 码，绕开大树和树桩）
    const float sideDists[] = { 4.5f, 6.0f, 3.5f, 7.5f };
    for (int i = 0; i < 4; ++i) {
        float dist = sideDists[i];
        // 优先左侧切
        int leftX = curX + (int)floorf(lx * dist + 0.5f);
        int leftY = curY + (int)floorf(ly * dist + 0.5f);
        if (IsMapCellWalkable(leftX, leftY)) {
            outSideX = leftX;
            outSideY = leftY;
            return true;
        }
        // 其次右侧切
        int rightX = curX + (int)floorf(rx * dist + 0.5f);
        int rightY = curY + (int)floorf(ry * dist + 0.5f);
        if (IsMapCellWalkable(rightX, rightY)) {
            outSideX = rightX;
            outSideY = rightY;
            return true;
        }
    }
    return false;
}

// Bresenham 视线遮挡判定
bool CheckMapLineOfSight(int x0, int y0, int x1, int y1) {
    if (x0 < 0 || y0 < 0 || x0 >= MAX_MAPSIZE || y0 >= MAX_MAPSIZE) return false;
    if (x1 < 0 || y1 < 0 || x1 >= MAX_MAPSIZE || y1 >= MAX_MAPSIZE) return false;

    int dx = abs(x1 - x0);
    int dy = abs(y1 - y0);
    if (dx == 0 && dy == 0) return true;

    int sx = (x0 < x1) ? 1 : -1;
    int sy = (y0 < y1) ? 1 : -1;
    int err = dx - dy;

    int x = x0;
    int y = y0;
    while (x != x1 || y != y1) {
        // 忽略起点自身与目标终点网格（防止贴墙站立或怪物碰撞体积边缘造成整条视线误判）
        if ((x != x0 || y != y0) && (x != x1 || y != y1) && !IsMapCellWalkable(x, y)) {
            return false;
        }
        int e2 = 2 * err;
        if (e2 > -dy) {
            err -= dy;
            x += sx;
        }
        if (e2 < dx) {
            err += dx;
            y += sy;
        }
    }
    return true;
}

// 局部 A* 寻路获取最远直通拐角点 (Waypoint)
bool FindNextWaypoint(int startX, int startY, int goalX, int goalY, int& outWpX, int& outWpY, int maxSearchRadius) {
    // 1. 直线视线无墙体遮挡，直接直通
    if (CheckMapLineOfSight(startX, startY, goalX, goalY)) {
        outWpX = goalX;
        outWpY = goalY;
        return true;
    }

    // 2. 目标若在障碍网格内部（如怪物紧贴墙体被算入墙内），在 2 格邻域寻找合法替代格
    if (!IsMapCellWalkable(goalX, goalY)) {
        int bestGx = -1, bestGy = -1;
        int bestDistSq = 9999;
        for (int dy = -2; dy <= 2; ++dy) {
            for (int dx = -2; dx <= 2; ++dx) {
                int cx = goalX + dx;
                int cy = goalY + dy;
                if (IsMapCellWalkable(cx, cy)) {
                    int d = dx * dx + dy * dy;
                    if (d < bestDistSq) {
                        bestDistSq = d;
                        bestGx = cx;
                        bestGy = cy;
                    }
                }
            }
        }
        if (bestGx < 0) return false;
        goalX = bestGx;
        goalY = bestGy;
        if (CheckMapLineOfSight(startX, startY, goalX, goalY)) {
            outWpX = goalX;
            outWpY = goalY;
            return true;
        }
    }

    // 3. 限制局部搜索包围盒，避免全图遍历
    int minX = (std::max)(0, (std::min)(startX, goalX) - 12);
    int maxX = (std::min)(MAX_MAPSIZE - 1, (std::max)(startX, goalX) + 12);
    int minY = (std::max)(0, (std::min)(startY, goalY) - 12);
    int maxY = (std::min)(MAX_MAPSIZE - 1, (std::max)(startY, goalY) + 12);

    if ((maxX - minX) > maxSearchRadius * 2 || (maxY - minY) > maxSearchRadius * 2) {
        return false;
    }

    int boxW = maxX - minX + 1;
    int boxH = maxY - minY + 1;
    int totalCells = boxW * boxH;

    std::vector<int> gScore(totalCells, 100000000);
    std::vector<int> cameFrom(totalCells, -1);

    auto ToLocal = [=](int x, int y) {
        return (y - minY) * boxW + (x - minX);
    };

    struct QNode {
        int x, y;
        int fCost;
        bool operator>(const QNode& o) const { return fCost > o.fCost; }
    };
    std::priority_queue<QNode, std::vector<QNode>, std::greater<QNode> > pq;

    int startLocal = ToLocal(startX, startY);
    gScore[startLocal] = 0;
    int h0 = (abs(goalX - startX) + abs(goalY - startY)) * 10;
    QNode startNode; startNode.x = startX; startNode.y = startY; startNode.fCost = h0;
    pq.push(startNode);

    static const int dirs[8][2] = {
        {0, 1}, {1, 0}, {0, -1}, {-1, 0},
        {1, 1}, {1, -1}, {-1, 1}, {-1, -1}
    };

    bool found = false;
    int expansions = 0;
    const int maxExpansions = 600;

    while (!pq.empty() && expansions < maxExpansions) {
        QNode cur = pq.top();
        pq.pop();
        expansions++;

        if (cur.x == goalX && cur.y == goalY) {
            found = true;
            break;
        }

        int curLocal = ToLocal(cur.x, cur.y);
        int curG = gScore[curLocal];

        for (int i = 0; i < 8; ++i) {
            int nx = cur.x + dirs[i][0];
            int ny = cur.y + dirs[i][1];

            if (nx < minX || nx > maxX || ny < minY || ny > maxY) continue;
            if (!IsMapCellWalkable(nx, ny)) continue;

            // 斜向移动防切死角
            if (dirs[i][0] != 0 && dirs[i][1] != 0) {
                if (!IsMapCellWalkable(cur.x + dirs[i][0], cur.y) || !IsMapCellWalkable(cur.x, cur.y + dirs[i][1])) {
                    continue;
                }
            }

            int stepCost = (dirs[i][0] != 0 && dirs[i][1] != 0) ? 14 : 10;
            int nextG = curG + stepCost;
            int nextLocal = ToLocal(nx, ny);

            if (nextG < gScore[nextLocal]) {
                gScore[nextLocal] = nextG;
                cameFrom[nextLocal] = curLocal;
                int h = (abs(goalX - nx) + abs(goalY - ny)) * 10;
                QNode nxt; nxt.x = nx; nxt.y = ny; nxt.fCost = nextG + h;
                pq.push(nxt);
            }
        }
    }

    if (!found) return false;

    // 回溯生成整条路径
    std::vector<std::pair<int, int> > path;
    int curr = ToLocal(goalX, goalY);
    while (curr != -1) {
        int ly = curr / boxW;
        int lx = curr % boxW;
        path.push_back(std::make_pair(minX + lx, minY + ly));
        if (curr == startLocal) break;
        curr = cameFrom[curr];
    }
    std::reverse(path.begin(), path.end());

    if (path.empty()) return false;

    // String-pulling: 从后往前寻找离当前起点最远且视线直通的拐角点
    for (int i = (int)path.size() - 1; i >= 0; --i) {
        if (CheckMapLineOfSight(startX, startY, path[i].first, path[i].second)) {
            outWpX = path[i].first;
            outWpY = path[i].second;
            return true;
        }
    }

    if (path.size() > 1) {
        outWpX = path[1].first;
        outWpY = path[1].second;
        return true;
    }

    outWpX = goalX;
    outWpY = goalY;
    return true;
}
