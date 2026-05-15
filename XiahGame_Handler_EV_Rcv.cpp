#include "XiahEnvInfo.h"

#define	DXRGB(r,g,b)	D3DCOLOR_XRGB(r,g,b)

#define		RAINFOG_VALUE		40
#define		RAINFOG_DENSITY		0.005f

// MAP갯수,시간별,컬러
// Diffuse, FogColor, Sky Bottom Color ,Sky Middle Color, Sky UP Color
D3DCOLOR g_DayFogColor[5 * 24][5] = 
{
	//////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// Diffuse				FogColor				Sky Bottom Color		Sky Middle Color		 Sky UP Color
	//////////////////////////////////////////////////////////////////////////////////////////////////////////////

	// MAP ID : 1 (기암)
	{DXRGB( 193, 224, 255), DXRGB(   4,  31,  17), DXRGB(  64,  88, 102), DXRGB(  64,  88, 102) , DXRGB(  16,  37,  88)},//0
	{DXRGB( 193, 224, 255), DXRGB(   4,  31,  17), DXRGB(  64,  88, 102), DXRGB(  64,  88, 102) , DXRGB(  16,  37,  88)},//1
	{DXRGB( 193, 224, 255), DXRGB(   4,  31,  17), DXRGB(  64,  88, 102), DXRGB(  64,  88, 102) , DXRGB(  16,  37,  88)},//2
	{DXRGB( 255, 240, 255), DXRGB(   4,  41,  35), DXRGB( 62,  62,   62), DXRGB( 0,   145, 251) , DXRGB( 255, 255, 255)},//3
	{DXRGB( 255, 240, 255), DXRGB(   4,  41,  35), DXRGB( 62,  62,   62), DXRGB( 0,   145, 251) , DXRGB( 255, 255, 255)},//4
	{DXRGB( 255, 240, 255), DXRGB(   4,  41,  35), DXRGB( 62,  62,   62), DXRGB( 0,   145, 251) , DXRGB( 255, 255, 255)},//5
	{DXRGB( 248, 250, 255), DXRGB(  55,  60,  20), DXRGB( 170, 170, 170), DXRGB( 170, 170, 170) , DXRGB( 255, 255, 255)},//6
	{DXRGB( 248, 250, 255), DXRGB(  55,  60,  20), DXRGB( 170, 170, 170), DXRGB( 170, 170, 170) , DXRGB( 255, 255, 255)},//7
	{DXRGB( 248, 250, 255), DXRGB(  55,  60,  20), DXRGB( 170, 170, 170), DXRGB( 170, 170, 170) , DXRGB( 255, 255, 255)},//8
	{DXRGB( 248, 250, 255), DXRGB(  55,  60,  20), DXRGB( 170, 170, 170), DXRGB( 170, 170, 170) , DXRGB( 255, 255, 255)},//9
	{DXRGB( 248, 250, 255), DXRGB(  55,  60,  20), DXRGB( 170, 170, 170), DXRGB( 170, 170, 170) , DXRGB( 255, 255, 255)},//10
	{DXRGB( 255, 255, 255), DXRGB(  81,  85,  42), DXRGB( 255, 255, 255), DXRGB( 255, 255, 255) , DXRGB( 255, 255, 255)},//11
	{DXRGB( 255, 255, 255), DXRGB(  81,  85,  42), DXRGB( 255, 255, 255), DXRGB( 255, 255, 255) , DXRGB( 255, 255, 255)},//12
	{DXRGB( 255, 255, 255), DXRGB(  81,  85,  42), DXRGB( 255, 255, 255), DXRGB( 255, 255, 255) , DXRGB( 255, 255, 255)},//13
	{DXRGB( 255, 255, 255), DXRGB(  81,  85,  42), DXRGB( 255, 255, 255), DXRGB( 255, 255, 255) , DXRGB( 255, 255, 255)},//14
	{DXRGB( 255, 255, 255), DXRGB(  81,  85,  42), DXRGB( 255, 255, 255), DXRGB( 255, 255, 255) , DXRGB( 255, 255, 255)},//15
	{DXRGB( 255, 255, 255), DXRGB(  81,  85,  42), DXRGB( 255, 255, 255), DXRGB( 255, 255, 255) , DXRGB( 255, 255, 255)},//16
	{DXRGB( 250, 220, 193), DXRGB(  79,  53,  13), DXRGB( 252, 157,  80), DXRGB( 252, 157,  80) , DXRGB( 155, 218, 255)},//17
	{DXRGB( 250, 220, 193), DXRGB(  79,  53,  13), DXRGB( 252, 157,  80), DXRGB( 252, 157,  80) , DXRGB( 155, 218, 255)},//18
	{DXRGB( 250, 220, 193), DXRGB(  79,  53,  13), DXRGB( 252, 157,  80), DXRGB( 252, 157,  80) , DXRGB( 155, 218, 255)},//19
	{DXRGB( 250, 220, 193), DXRGB(  79,  53,  13), DXRGB( 252, 157,  80), DXRGB( 252, 157,  80) , DXRGB( 155, 218, 255)},//20
	{DXRGB( 255, 208, 235), DXRGB(   8,  33,  18), DXRGB(  57,  94, 108), DXRGB(  57,  94, 108) , DXRGB(  62,  78,  91)},//21
	{DXRGB( 255, 208, 235), DXRGB(   8,  33,  18), DXRGB(  57,  94, 108), DXRGB(  57,  94, 108) , DXRGB(  62,  78,  91)},//22
	{DXRGB( 255, 208, 235), DXRGB(   8,  33,  18), DXRGB(  57,  94, 108), DXRGB(  57,  94, 108) , DXRGB(  62,  78,  91)},//23

	// MAP ID : 2 (화산)
	{DXRGB( 220, 235, 255), DXRGB( 100,  30,  30), DXRGB( 104,  69,  90), DXRGB( 104,  69,  90) , DXRGB(  70,  51,  40)},//0
	{DXRGB( 220, 235, 255), DXRGB( 100,  30,  30), DXRGB( 104,  69,  90), DXRGB( 104,  69,  90) , DXRGB(  70,  51,  40)},//1
	{DXRGB( 220, 235, 255), DXRGB( 100,  30,  30), DXRGB( 104,  69,  90), DXRGB( 104,  69,  90) , DXRGB(  70,  51,  40)},//2
	{DXRGB( 238, 248, 255), DXRGB(  69,  11,   6), DXRGB(  87,   0,  39), DXRGB(  87,   0,  39) , DXRGB( 255, 227, 176)},//3
	{DXRGB( 238, 248, 255), DXRGB(  69,  11,   6), DXRGB(  87,   0,  39), DXRGB(  87,   0,  39) , DXRGB( 255, 227, 176)},//4
	{DXRGB( 238, 248, 255), DXRGB(  69,  11,   6), DXRGB(  87,   0,  39), DXRGB(  87,   0,  39) , DXRGB( 255, 227, 176)},//5
	{DXRGB( 238, 248, 255), DXRGB(  69,  11,   6), DXRGB(  87,   0,  39), DXRGB(  87,   0,  39) , DXRGB( 255, 227, 176)},//6
	{DXRGB( 238, 248, 255), DXRGB(  69,  11,   6), DXRGB(  87,   0,  39), DXRGB(  87,   0,  39) , DXRGB( 255, 227, 176)},//7
	{DXRGB( 255, 255, 255), DXRGB( 190,  97,  62), DXRGB( 255, 255, 255), DXRGB( 255, 255, 255) , DXRGB( 219, 199, 185)},//8
	{DXRGB( 255, 255, 255), DXRGB( 190,  97,  62), DXRGB( 255, 255, 255), DXRGB( 255, 255, 255) , DXRGB( 219, 199, 185)},//9
	{DXRGB( 255, 255, 255), DXRGB( 190,  97,  62), DXRGB( 255, 255, 255), DXRGB( 255, 255, 255) , DXRGB( 219, 199, 185)},//10
	{DXRGB( 255, 255, 255), DXRGB( 190,  97,  62), DXRGB( 255, 255, 255), DXRGB( 255, 255, 255) , DXRGB( 219, 199, 185)},//11
	{DXRGB( 255, 255, 255), DXRGB( 190,  97,  62), DXRGB( 255, 255, 255), DXRGB( 255, 255, 255) , DXRGB( 219, 199, 185)},//12
	{DXRGB( 255, 255, 255), DXRGB( 190,  97,  62), DXRGB( 255, 255, 255), DXRGB( 255, 255, 255) , DXRGB( 219, 199, 185)},//13
	{DXRGB( 255, 255, 255), DXRGB( 190,  97,  62), DXRGB( 255, 255, 255), DXRGB( 255, 255, 255) , DXRGB( 219, 199, 185)},//14
	{DXRGB( 255, 255, 255), DXRGB( 190,  97,  62), DXRGB( 255, 255, 255), DXRGB( 255, 255, 255) , DXRGB( 219, 199, 185)},//15
	{DXRGB( 255, 255, 255), DXRGB( 190,  97,  62), DXRGB( 255, 255, 255), DXRGB( 255, 255, 255) , DXRGB( 219, 199, 185)},//16
	{DXRGB( 255, 255, 255), DXRGB( 190,  97,  62), DXRGB( 255, 255, 255), DXRGB( 255, 255, 255) , DXRGB( 219, 199, 185)},//17
	{DXRGB( 255, 255, 255), DXRGB( 190,  97,  62), DXRGB( 255, 255, 255), DXRGB( 255, 255, 255) , DXRGB( 219, 199, 185)},//18
	{DXRGB( 255, 255, 255), DXRGB( 190,  97,  62), DXRGB( 255, 255, 255), DXRGB( 255, 255, 255) , DXRGB( 219, 199, 185)},//19
	{DXRGB( 220, 222, 255), DXRGB(  89,  28,  18), DXRGB( 104,  69,  90), DXRGB( 104,  69,  90) , DXRGB(  70,  51,  40)},//20
	{DXRGB( 220, 222, 255), DXRGB(  89,  28,  18), DXRGB( 104,  69,  90), DXRGB( 104,  69,  90) , DXRGB(  70,  51,  40)},//21
	{DXRGB( 220, 222, 255), DXRGB(  89,  28,  18), DXRGB( 104,  69,  90), DXRGB( 104,  69,  90) , DXRGB(  70,  51,  40)},//22
	{DXRGB( 220, 222, 255), DXRGB(  89,  28,  18), DXRGB( 104,  69,  90), DXRGB( 104,  69,  90) , DXRGB(  70,  51,  40)},//23

	// MAP ID : 3 (빙하)
	{DXRGB( 214, 224, 241), DXRGB(  39,  63, 101), DXRGB(  44,  69, 101), DXRGB(  44,  69, 101) , DXRGB( 255, 255, 255)},//0
	{DXRGB( 214, 224, 241), DXRGB(  39,  63, 101), DXRGB(  44,  69, 101), DXRGB(  44,  69, 101) , DXRGB( 255, 255, 255)},//1
	{DXRGB( 214, 224, 241), DXRGB(  39,  63, 101), DXRGB(  44,  69, 101), DXRGB(  44,  69, 101) , DXRGB( 255, 255, 255)},//2
	{DXRGB( 236, 241, 249), DXRGB(  95, 125, 188), DXRGB(  96, 136, 185), DXRGB(  96, 136, 185) , DXRGB( 255, 255, 255)},//3
	{DXRGB( 236, 241, 249), DXRGB(  95, 125, 188), DXRGB(  96, 136, 185), DXRGB(  96, 136, 185) , DXRGB( 255, 255, 255)},//4
	{DXRGB( 236, 241, 249), DXRGB(  95, 125, 188), DXRGB(  96, 136, 185), DXRGB(  96, 136, 185) , DXRGB( 255, 255, 255)},//5
	{DXRGB( 236, 241, 249), DXRGB(  95, 125, 188), DXRGB(  96, 136, 185), DXRGB(  96, 136, 185) , DXRGB( 255, 255, 255)},//6
	{DXRGB( 236, 241, 249), DXRGB(  95, 125, 188), DXRGB(  96, 136, 185), DXRGB(  96, 136, 185) , DXRGB( 255, 255, 255)},//7
	{DXRGB( 255, 255, 255), DXRGB( 236, 239, 255), DXRGB( 255, 255, 255), DXRGB( 255, 255, 255) , DXRGB( 255, 255, 255)},//8
	{DXRGB( 255, 255, 255), DXRGB( 236, 239, 255), DXRGB( 255, 255, 255), DXRGB( 255, 255, 255) , DXRGB( 255, 255, 255)},//9
	{DXRGB( 255, 255, 255), DXRGB( 236, 239, 255), DXRGB( 255, 255, 255), DXRGB( 255, 255, 255) , DXRGB( 255, 255, 255)},//10
	{DXRGB( 255, 255, 255), DXRGB( 236, 239, 255), DXRGB( 255, 255, 255), DXRGB( 255, 255, 255) , DXRGB( 255, 255, 255)},//11
	{DXRGB( 255, 255, 255), DXRGB( 236, 239, 255), DXRGB( 255, 255, 255), DXRGB( 255, 255, 255) , DXRGB( 255, 255, 255)},//12
	{DXRGB( 255, 255, 255), DXRGB( 236, 239, 255), DXRGB( 255, 255, 255), DXRGB( 255, 255, 255) , DXRGB( 255, 255, 255)},//13
	{DXRGB( 255, 255, 255), DXRGB( 236, 239, 255), DXRGB( 255, 255, 255), DXRGB( 255, 255, 255) , DXRGB( 255, 255, 255)},//14
	{DXRGB( 255, 255, 255), DXRGB( 236, 239, 255), DXRGB( 255, 255, 255), DXRGB( 255, 255, 255) , DXRGB( 255, 255, 255)},//15
	{DXRGB( 255, 255, 255), DXRGB( 236, 239, 255), DXRGB( 255, 255, 255), DXRGB( 255, 255, 255) , DXRGB( 255, 255, 255)},//16
	{DXRGB( 255, 255, 255), DXRGB( 236, 239, 255), DXRGB( 255, 255, 255), DXRGB( 255, 255, 255) , DXRGB( 255, 255, 255)},//17
	{DXRGB( 255, 255, 255), DXRGB( 236, 239, 255), DXRGB( 255, 255, 255), DXRGB( 255, 255, 255) , DXRGB( 255, 255, 255)},//18
	{DXRGB( 255, 255, 255), DXRGB( 236, 239, 255), DXRGB( 255, 255, 255), DXRGB( 255, 255, 255) , DXRGB( 255, 255, 255)},//19
	{DXRGB( 255, 255, 255), DXRGB( 236, 239, 255), DXRGB( 255, 255, 255), DXRGB( 255, 255, 255) , DXRGB( 255, 255, 255)},//20
	{DXRGB( 214, 224, 241), DXRGB(  39,  63, 101), DXRGB(  44,  69, 101), DXRGB(  44,  69, 101) , DXRGB( 255, 255, 255)},//21
	{DXRGB( 214, 224, 241), DXRGB(  39,  63, 101), DXRGB(  44,  69, 101), DXRGB(  44,  69, 101) , DXRGB( 255, 255, 255)},//22
	{DXRGB( 214, 224, 241), DXRGB(  39,  63, 101), DXRGB(  44,  69, 101), DXRGB(  44,  69, 101) , DXRGB( 255, 255, 255)},//23

	// MAP ID : 4 (사막)
	{DXRGB( 252, 190, 135), DXRGB(  45,  41,  37), DXRGB(  64,  88, 102), DXRGB(  64,  88, 102) , DXRGB(  43,  51,  62)},//0
	{DXRGB( 252, 190, 135), DXRGB(  45,  41,  37), DXRGB(  64,  88, 102), DXRGB(  64,  88, 102) , DXRGB(  43,  51,  62)},//1
	{DXRGB( 252, 190, 135), DXRGB(  45,  41,  37), DXRGB(  64,  88, 102), DXRGB(  64,  88, 102) , DXRGB(  43,  51,  62)},//2
	{DXRGB( 157, 201, 255), DXRGB(  45,  82,  90), DXRGB(  20,  71, 148), DXRGB(  20,  71, 148) , DXRGB(  26, 140, 255)},//3
	{DXRGB( 157, 201, 255), DXRGB(  45,  82,  90), DXRGB(  20,  71, 148), DXRGB(  20,  71, 148) , DXRGB(  26, 140, 255)},//4
	{DXRGB( 157, 201, 255), DXRGB(  45,  82,  90), DXRGB(  20,  71, 148), DXRGB(  20,  71, 148) , DXRGB(  26, 140, 255)},//5
	{DXRGB( 200, 220, 240), DXRGB(  62, 100, 100), DXRGB(  90, 197, 254), DXRGB(  90, 197, 254) , DXRGB( 255, 255, 255)},//6
	{DXRGB( 200, 220, 240), DXRGB(  62, 100, 100), DXRGB(  90, 197, 254), DXRGB(  90, 197, 254) , DXRGB( 255, 255, 255)},//7
	{DXRGB( 200, 220, 240), DXRGB(  62, 100, 100), DXRGB(  90, 197, 254), DXRGB(  90, 197, 254) , DXRGB( 255, 255, 255)},//8
	{DXRGB( 200, 220, 240), DXRGB(  62, 100, 100), DXRGB(  90, 197, 254), DXRGB(  90, 197, 254) , DXRGB( 255, 255, 255)},//9
	{DXRGB( 200, 220, 240), DXRGB(  62, 100, 100), DXRGB(  90, 197, 254), DXRGB(  90, 197, 254) , DXRGB( 255, 255, 255)},//10
	{DXRGB( 255, 255, 255), DXRGB( 175, 120, 100), DXRGB( 255, 255, 255), DXRGB( 255, 255, 255) , DXRGB( 255, 255, 255)},//11
	{DXRGB( 255, 255, 255), DXRGB( 175, 120, 100), DXRGB( 255, 255, 255), DXRGB( 255, 255, 255) , DXRGB( 255, 255, 255)},//12
	{DXRGB( 255, 255, 255), DXRGB( 175, 120, 100), DXRGB( 255, 255, 255), DXRGB( 255, 255, 255) , DXRGB( 255, 255, 255)},//13
	{DXRGB( 255, 255, 255), DXRGB( 175, 120, 100), DXRGB( 255, 255, 255), DXRGB( 255, 255, 255) , DXRGB( 255, 255, 255)},//14
	{DXRGB( 255, 255, 255), DXRGB( 175, 120, 100), DXRGB( 255, 255, 255), DXRGB( 255, 255, 255) , DXRGB( 255, 255, 255)},//15
	{DXRGB( 255, 255, 255), DXRGB( 175, 120, 100), DXRGB( 255, 255, 255), DXRGB( 255, 255, 255) , DXRGB( 255, 255, 255)},//16
	{DXRGB( 255, 255, 255), DXRGB(  86,  55,  65), DXRGB( 255, 255, 255), DXRGB( 255, 255, 255) , DXRGB( 255, 255, 255)},//17
	{DXRGB( 255, 255, 255), DXRGB(  86,  55,  65), DXRGB( 255, 255, 255), DXRGB( 255, 255, 255) , DXRGB( 255, 255, 255)},//18
	{DXRGB( 255, 255, 255), DXRGB(  86,  55,  65), DXRGB( 255, 255, 255), DXRGB( 255, 255, 255) , DXRGB( 255, 255, 255)},//19
	{DXRGB( 255, 255, 255), DXRGB(  86,  55,  65), DXRGB( 128, 128, 128), DXRGB( 128, 128, 128) , DXRGB( 255, 204, 170)},//20
	{DXRGB( 254, 234, 216), DXRGB(  50,  20,  30), DXRGB( 128, 128, 128), DXRGB( 128, 128, 128) , DXRGB( 255, 204, 170)},//21
	{DXRGB( 254, 234, 216), DXRGB(  50,  20,  30), DXRGB( 128, 128, 128), DXRGB( 128, 128, 128) , DXRGB( 255, 204, 170)},//22
	{DXRGB( 254, 234, 216), DXRGB(  50,  20,  30), DXRGB(  64,  88, 102), DXRGB(  64,  88, 102) , DXRGB(  43,  51,  62)},//23

	// MAP ID : 5 (늪)
	{DXRGB( 255, 255, 255), DXRGB( 127, 135,  92), DXRGB(  75, 119,  40), DXRGB(  75, 119,  40) , DXRGB( 255, 255, 255)},//0
	{DXRGB( 255, 255, 255), DXRGB( 127, 135,  92), DXRGB(  75, 119,  40), DXRGB(  75, 119,  40) , DXRGB( 255, 255, 255)},//1
	{DXRGB( 255, 255, 255), DXRGB( 127, 135,  92), DXRGB(  75, 119,  40), DXRGB(  75, 119,  40) , DXRGB( 255, 255, 255)},//2
	{DXRGB( 255, 255, 255), DXRGB( 127, 135,  92), DXRGB(  75, 119,  40), DXRGB(  75, 119,  40) , DXRGB( 255, 255, 255)},//3
	{DXRGB( 255, 255, 255), DXRGB( 127, 135,  92), DXRGB( 255, 255, 255), DXRGB( 255, 255, 255) , DXRGB( 255, 255, 255)},//4
	{DXRGB( 255, 255, 255), DXRGB( 127, 135,  92), DXRGB( 255, 255, 255), DXRGB( 255, 255, 255) , DXRGB( 255, 255, 255)},//5
	{DXRGB( 255, 255, 255), DXRGB( 127, 135,  92), DXRGB( 255, 255, 255), DXRGB( 255, 255, 255) , DXRGB( 255, 255, 255)},//6
	{DXRGB( 255, 255, 255), DXRGB( 127, 135,  92), DXRGB( 255, 255, 255), DXRGB( 255, 255, 255) , DXRGB( 255, 255, 255)},//7
	{DXRGB( 255, 255, 255), DXRGB( 127, 135,  92), DXRGB( 255, 255, 255), DXRGB( 255, 255, 255) , DXRGB( 255, 255, 255)},//8
	{DXRGB( 255, 255, 255), DXRGB( 127, 135,  92), DXRGB( 255, 255, 255), DXRGB( 255, 255, 255) , DXRGB( 255, 255, 255)},//9
	{DXRGB( 255, 255, 255), DXRGB( 127, 135,  92), DXRGB( 255, 255, 255), DXRGB( 255, 255, 255) , DXRGB( 255, 255, 255)},//10
	{DXRGB( 255, 255, 255), DXRGB( 127, 135,  92), DXRGB( 255, 255, 255), DXRGB( 255, 255, 255) , DXRGB( 255, 255, 255)},//11
	{DXRGB( 255, 255, 255), DXRGB( 127, 135,  92), DXRGB( 255, 255, 255), DXRGB( 255, 255, 255) , DXRGB( 255, 255, 255)},//12
	{DXRGB( 255, 255, 255), DXRGB( 127, 135,  92), DXRGB( 255, 255, 255), DXRGB( 255, 255, 255) , DXRGB( 255, 255, 255)},//13
	{DXRGB( 255, 255, 255), DXRGB( 127, 135,  92), DXRGB( 255, 255, 255), DXRGB( 255, 255, 255) , DXRGB( 255, 255, 255)},//14
	{DXRGB( 255, 255, 255), DXRGB( 127, 135,  92), DXRGB( 255, 255, 255), DXRGB( 255, 255, 255) , DXRGB( 255, 255, 255)},//15
	{DXRGB( 255, 255, 255), DXRGB( 127, 135,  92), DXRGB( 255, 255, 255), DXRGB( 255, 255, 255) , DXRGB( 255, 255, 255)},//16
	{DXRGB( 255, 255, 255), DXRGB( 127, 135,  92), DXRGB( 255, 255, 255), DXRGB( 255, 255, 255) , DXRGB( 255, 255, 255)},//17
	{DXRGB( 255, 255, 255), DXRGB( 127, 135,  92), DXRGB( 255, 255, 255), DXRGB( 255, 255, 255) , DXRGB( 255, 255, 255)},//18
	{DXRGB( 255, 255, 255), DXRGB( 127, 135,  92), DXRGB( 255, 255, 255), DXRGB( 255, 255, 255) , DXRGB( 255, 255, 255)},//19
	{DXRGB( 255, 255, 255), DXRGB( 127, 135,  92), DXRGB( 255, 255, 255), DXRGB( 255, 255, 255) , DXRGB( 255, 255, 255)},//20
	{DXRGB( 255, 255, 255), DXRGB( 127, 135,  92), DXRGB( 255, 255, 255), DXRGB( 255, 255, 255) , DXRGB( 255, 255, 255)},//21
	{DXRGB( 255, 255, 255), DXRGB( 127, 135,  92), DXRGB( 255, 255, 255), DXRGB( 255, 255, 255) , DXRGB( 255, 255, 255)},//22
	{DXRGB( 255, 255, 255), DXRGB( 127, 135,  92), DXRGB( 255, 255, 255), DXRGB( 255, 255, 255) , DXRGB( 255, 255, 255)} //23

	/*
	//////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// Diffuse				FogColor				Sky Bottom Color		Sky Middle Color		 Sky UP Color
	//////////////////////////////////////////////////////////////////////////////////////////////////////////////

	// MAP ID : 1 (기암)
	{DXRGB( 193, 224, 255), DXRGB(   4,  31,  17), DXRGB(  64,  88, 102), DXRGB(  64,  88, 102) , DXRGB(  16,  37,  88)},//0
	{DXRGB( 193, 224, 255), DXRGB(   4,  31,  17), DXRGB(  64,  88, 102), DXRGB(  64,  88, 102) , DXRGB(  16,  37,  88)},//1
	{DXRGB( 193, 224, 255), DXRGB(   4,  31,  17), DXRGB(  64,  88, 102), DXRGB(  64,  88, 102) , DXRGB(  16,  37,  88)},//2
	{DXRGB( 255, 240, 255), DXRGB(   4,  41,  35), DXRGB( 62,  62,   62), DXRGB( 0,   145, 251) , DXRGB( 255, 255, 255)},//3
	{DXRGB( 255, 240, 255), DXRGB(   4,  41,  35), DXRGB( 62,  62,   62), DXRGB( 0,   145, 251) , DXRGB( 255, 255, 255)},//4
	{DXRGB( 255, 240, 255), DXRGB(   4,  41,  35), DXRGB( 62,  62,   62), DXRGB( 0,   145, 251) , DXRGB( 255, 255, 255)},//5
	{DXRGB( 247, 251, 255), DXRGB(  55,  57,  26), DXRGB( 170, 170, 170), DXRGB( 170, 170, 170) , DXRGB( 255, 255, 255)},//6
	{DXRGB( 247, 251, 255), DXRGB(  55,  57,  26), DXRGB( 170, 170, 170), DXRGB( 170, 170, 170) , DXRGB( 255, 255, 255)},//7
	{DXRGB( 247, 251, 255), DXRGB(  55,  57,  26), DXRGB( 170, 170, 170), DXRGB( 170, 170, 170) , DXRGB( 255, 255, 255)},//8
	{DXRGB( 247, 251, 255), DXRGB(  55,  57,  26), DXRGB( 170, 170, 170), DXRGB( 170, 170, 170) , DXRGB( 255, 255, 255)},//9
	{DXRGB( 247, 251, 255), DXRGB(  55,  57,  26), DXRGB( 170, 170, 170), DXRGB( 170, 170, 170) , DXRGB( 255, 255, 255)},//10
	{DXRGB( 255, 255, 255), DXRGB(  81,  85,  42), DXRGB( 255, 255, 255), DXRGB( 255, 255, 255) , DXRGB( 255, 255, 255)},//11
	{DXRGB( 255, 255, 255), DXRGB(  81,  85,  42), DXRGB( 255, 255, 255), DXRGB( 255, 255, 255) , DXRGB( 255, 255, 255)},//12
	{DXRGB( 255, 255, 255), DXRGB(  81,  85,  42), DXRGB( 255, 255, 255), DXRGB( 255, 255, 255) , DXRGB( 255, 255, 255)},//13
	{DXRGB( 255, 255, 255), DXRGB(  81,  85,  42), DXRGB( 255, 255, 255), DXRGB( 255, 255, 255) , DXRGB( 255, 255, 255)},//14
	{DXRGB( 255, 255, 255), DXRGB(  81,  85,  42), DXRGB( 255, 255, 255), DXRGB( 255, 255, 255) , DXRGB( 255, 255, 255)},//15
	{DXRGB( 255, 255, 255), DXRGB(  81,  85,  42), DXRGB( 255, 255, 255), DXRGB( 255, 255, 255) , DXRGB( 255, 255, 255)},//16
	{DXRGB( 255, 220, 193), DXRGB(  79,  53,  13), DXRGB( 252, 157,  80), DXRGB( 252, 157,  80) , DXRGB( 155, 218, 255)},//17
	{DXRGB( 255, 220, 193), DXRGB(  79,  53,  13), DXRGB( 252, 157,  80), DXRGB( 252, 157,  80) , DXRGB( 155, 218, 255)},//18
	{DXRGB( 255, 220, 193), DXRGB(  79,  53,  13), DXRGB( 252, 157,  80), DXRGB( 252, 157,  80) , DXRGB( 155, 218, 255)},//19
	{DXRGB( 255, 220, 193), DXRGB(  79,  53,  13), DXRGB( 252, 157,  80), DXRGB( 252, 157,  80) , DXRGB( 155, 218, 255)},//20
	{DXRGB( 255, 208, 235), DXRGB(   8,  33,  18), DXRGB(  57,  94, 108), DXRGB(  57,  94, 108) , DXRGB(  62,  78,  91)},//21
	{DXRGB( 255, 208, 235), DXRGB(   8,  33,  18), DXRGB(  57,  94, 108), DXRGB(  57,  94, 108) , DXRGB(  62,  78,  91)},//22
	{DXRGB( 255, 208, 235), DXRGB(   8,  33,  18), DXRGB(  57,  94, 108), DXRGB(  57,  94, 108) , DXRGB(  62,  78,  91)},//23

	// MAP ID : 2 (화산)
	{DXRGB( 220, 235, 255), DXRGB(  89,  28,  18), DXRGB( 104,  69,  90), DXRGB( 104,  69,  90) , DXRGB(  70,  51,  40)},//0
	{DXRGB( 220, 235, 255), DXRGB(  89,  28,  18), DXRGB( 104,  69,  90), DXRGB( 104,  69,  90) , DXRGB(  70,  51,  40)},//1
	{DXRGB( 220, 235, 255), DXRGB(  89,  28,  18), DXRGB( 104,  69,  90), DXRGB( 104,  69,  90) , DXRGB(  70,  51,  40)},//2
	{DXRGB( 238, 248, 255), DXRGB(  74,   3,   6), DXRGB(  87,   0,  39), DXRGB(  87,   0,  39) , DXRGB( 255, 227, 176)},//3
	{DXRGB( 238, 248, 255), DXRGB(  74,   3,   6), DXRGB(  87,   0,  39), DXRGB(  87,   0,  39) , DXRGB( 255, 227, 176)},//4
	{DXRGB( 238, 248, 255), DXRGB(  74,   3,   6), DXRGB(  87,   0,  39), DXRGB(  87,   0,  39) , DXRGB( 255, 227, 176)},//5
	{DXRGB( 238, 248, 255), DXRGB(  74,   3,   6), DXRGB(  87,   0,  39), DXRGB(  87,   0,  39) , DXRGB( 255, 227, 176)},//6
	{DXRGB( 238, 248, 255), DXRGB(  74,   3,   6), DXRGB(  87,   0,  39), DXRGB(  87,   0,  39) , DXRGB( 255, 227, 176)},//7
	{DXRGB( 255, 255, 255), DXRGB( 218,  97,  62), DXRGB( 255, 255, 255), DXRGB( 255, 255, 255) , DXRGB( 219, 199, 185)},//8
	{DXRGB( 255, 255, 255), DXRGB( 218,  97,  62), DXRGB( 255, 255, 255), DXRGB( 255, 255, 255) , DXRGB( 219, 199, 185)},//9
	{DXRGB( 255, 255, 255), DXRGB( 218,  97,  62), DXRGB( 255, 255, 255), DXRGB( 255, 255, 255) , DXRGB( 219, 199, 185)},//10
	{DXRGB( 255, 255, 255), DXRGB( 218,  97,  62), DXRGB( 255, 255, 255), DXRGB( 255, 255, 255) , DXRGB( 219, 199, 185)},//11
	{DXRGB( 255, 255, 255), DXRGB( 218,  97,  62), DXRGB( 255, 255, 255), DXRGB( 255, 255, 255) , DXRGB( 219, 199, 185)},//12
	{DXRGB( 255, 255, 255), DXRGB( 218,  97,  62), DXRGB( 255, 255, 255), DXRGB( 255, 255, 255) , DXRGB( 219, 199, 185)},//13
	{DXRGB( 255, 255, 255), DXRGB( 218,  97,  62), DXRGB( 255, 255, 255), DXRGB( 255, 255, 255) , DXRGB( 219, 199, 185)},//14
	{DXRGB( 255, 255, 255), DXRGB( 218,  97,  62), DXRGB( 255, 255, 255), DXRGB( 255, 255, 255) , DXRGB( 219, 199, 185)},//15
	{DXRGB( 255, 255, 255), DXRGB( 218,  97,  62), DXRGB( 255, 255, 255), DXRGB( 255, 255, 255) , DXRGB( 219, 199, 185)},//16
	{DXRGB( 255, 255, 255), DXRGB( 218,  97,  62), DXRGB( 255, 255, 255), DXRGB( 255, 255, 255) , DXRGB( 219, 199, 185)},//17
	{DXRGB( 255, 255, 255), DXRGB( 218,  97,  62), DXRGB( 255, 255, 255), DXRGB( 255, 255, 255) , DXRGB( 219, 199, 185)},//18
	{DXRGB( 255, 255, 255), DXRGB( 218,  97,  62), DXRGB( 255, 255, 255), DXRGB( 255, 255, 255) , DXRGB( 219, 199, 185)},//19
	{DXRGB( 220, 235, 255), DXRGB(  89,  28,  18), DXRGB( 104,  69,  90), DXRGB( 104,  69,  90) , DXRGB(  70,  51,  40)},//20
	{DXRGB( 220, 235, 255), DXRGB(  89,  28,  18), DXRGB( 104,  69,  90), DXRGB( 104,  69,  90) , DXRGB(  70,  51,  40)},//21
	{DXRGB( 220, 235, 255), DXRGB(  89,  28,  18), DXRGB( 104,  69,  90), DXRGB( 104,  69,  90) , DXRGB(  70,  51,  40)},//22
	{DXRGB( 220, 235, 255), DXRGB(  89,  28,  18), DXRGB( 104,  69,  90), DXRGB( 104,  69,  90) , DXRGB(  70,  51,  40)},//23

	// MAP ID : 3 (빙하)
	{DXRGB( 214, 224, 241), DXRGB(  39,  63, 101), DXRGB(  44,  69, 101), DXRGB(  44,  69, 101) , DXRGB( 255, 255, 255)},//0
	{DXRGB( 214, 224, 241), DXRGB(  39,  63, 101), DXRGB(  44,  69, 101), DXRGB(  44,  69, 101) , DXRGB( 255, 255, 255)},//1
	{DXRGB( 214, 224, 241), DXRGB(  39,  63, 101), DXRGB(  44,  69, 101), DXRGB(  44,  69, 101) , DXRGB( 255, 255, 255)},//2
	{DXRGB( 236, 241, 249), DXRGB(  95, 125, 188), DXRGB(  96, 136, 185), DXRGB(  96, 136, 185) , DXRGB( 255, 255, 255)},//3
	{DXRGB( 236, 241, 249), DXRGB(  95, 125, 188), DXRGB(  96, 136, 185), DXRGB(  96, 136, 185) , DXRGB( 255, 255, 255)},//4
	{DXRGB( 236, 241, 249), DXRGB(  95, 125, 188), DXRGB(  96, 136, 185), DXRGB(  96, 136, 185) , DXRGB( 255, 255, 255)},//5
	{DXRGB( 236, 241, 249), DXRGB(  95, 125, 188), DXRGB(  96, 136, 185), DXRGB(  96, 136, 185) , DXRGB( 255, 255, 255)},//6
	{DXRGB( 236, 241, 249), DXRGB(  95, 125, 188), DXRGB(  96, 136, 185), DXRGB(  96, 136, 185) , DXRGB( 255, 255, 255)},//7
	{DXRGB( 255, 255, 255), DXRGB( 236, 239, 255), DXRGB( 255, 255, 255), DXRGB( 255, 255, 255) , DXRGB( 255, 255, 255)},//8
	{DXRGB( 255, 255, 255), DXRGB( 236, 239, 255), DXRGB( 255, 255, 255), DXRGB( 255, 255, 255) , DXRGB( 255, 255, 255)},//9
	{DXRGB( 255, 255, 255), DXRGB( 236, 239, 255), DXRGB( 255, 255, 255), DXRGB( 255, 255, 255) , DXRGB( 255, 255, 255)},//10
	{DXRGB( 255, 255, 255), DXRGB( 236, 239, 255), DXRGB( 255, 255, 255), DXRGB( 255, 255, 255) , DXRGB( 255, 255, 255)},//11
	{DXRGB( 255, 255, 255), DXRGB( 236, 239, 255), DXRGB( 255, 255, 255), DXRGB( 255, 255, 255) , DXRGB( 255, 255, 255)},//12
	{DXRGB( 255, 255, 255), DXRGB( 236, 239, 255), DXRGB( 255, 255, 255), DXRGB( 255, 255, 255) , DXRGB( 255, 255, 255)},//13
	{DXRGB( 255, 255, 255), DXRGB( 236, 239, 255), DXRGB( 255, 255, 255), DXRGB( 255, 255, 255) , DXRGB( 255, 255, 255)},//14
	{DXRGB( 255, 255, 255), DXRGB( 236, 239, 255), DXRGB( 255, 255, 255), DXRGB( 255, 255, 255) , DXRGB( 255, 255, 255)},//15
	{DXRGB( 255, 255, 255), DXRGB( 236, 239, 255), DXRGB( 255, 255, 255), DXRGB( 255, 255, 255) , DXRGB( 255, 255, 255)},//16
	{DXRGB( 255, 255, 255), DXRGB( 236, 239, 255), DXRGB( 255, 255, 255), DXRGB( 255, 255, 255) , DXRGB( 255, 255, 255)},//17
	{DXRGB( 255, 255, 255), DXRGB( 236, 239, 255), DXRGB( 255, 255, 255), DXRGB( 255, 255, 255) , DXRGB( 255, 255, 255)},//18
	{DXRGB( 255, 255, 255), DXRGB( 236, 239, 255), DXRGB( 255, 255, 255), DXRGB( 255, 255, 255) , DXRGB( 255, 255, 255)},//19
	{DXRGB( 255, 255, 255), DXRGB( 236, 239, 255), DXRGB( 255, 255, 255), DXRGB( 255, 255, 255) , DXRGB( 255, 255, 255)},//20
	{DXRGB( 214, 224, 241), DXRGB(  39,  63, 101), DXRGB(  44,  69, 101), DXRGB(  44,  69, 101) , DXRGB( 255, 255, 255)},//21
	{DXRGB( 214, 224, 241), DXRGB(  39,  63, 101), DXRGB(  44,  69, 101), DXRGB(  44,  69, 101) , DXRGB( 255, 255, 255)},//22
	{DXRGB( 214, 224, 241), DXRGB(  39,  63, 101), DXRGB(  44,  69, 101), DXRGB(  44,  69, 101) , DXRGB( 255, 255, 255)},//23

	// MAP ID : 4 (사막)
	{DXRGB( 252, 190, 135), DXRGB(  45,  41,  37), DXRGB(  64,  88, 102), DXRGB(  64,  88, 102) , DXRGB(  43,  51,  62)},//0
	{DXRGB( 252, 190, 135), DXRGB(  45,  41,  37), DXRGB(  64,  88, 102), DXRGB(  64,  88, 102) , DXRGB(  43,  51,  62)},//1
	{DXRGB( 252, 190, 135), DXRGB(  45,  41,  37), DXRGB(  64,  88, 102), DXRGB(  64,  88, 102) , DXRGB(  43,  51,  62)},//2
	{DXRGB( 157, 201, 253), DXRGB(  31,  82,  90), DXRGB(  20,  71, 148), DXRGB(  20,  71, 148) , DXRGB(  26, 140, 255)},//3
	{DXRGB( 157, 201, 253), DXRGB(  31,  82,  90), DXRGB(  20,  71, 148), DXRGB(  20,  71, 148) , DXRGB(  26, 140, 255)},//4
	{DXRGB( 157, 201, 253), DXRGB(  31,  82,  90), DXRGB(  20,  71, 148), DXRGB(  20,  71, 148) , DXRGB(  26, 140, 255)},//5
	{DXRGB( 196, 218, 240), DXRGB(  62,  92,  93), DXRGB(  90, 197, 254), DXRGB(  90, 197, 254) , DXRGB( 255, 255, 255)},//6
	{DXRGB( 196, 218, 240), DXRGB(  62,  92,  93), DXRGB(  90, 197, 254), DXRGB(  90, 197, 254) , DXRGB( 255, 255, 255)},//7
	{DXRGB( 196, 218, 240), DXRGB(  62,  92,  93), DXRGB(  90, 197, 254), DXRGB(  90, 197, 254) , DXRGB( 255, 255, 255)},//8
	{DXRGB( 196, 218, 240), DXRGB(  62,  92,  93), DXRGB(  90, 197, 254), DXRGB(  90, 197, 254) , DXRGB( 255, 255, 255)},//9
	{DXRGB( 196, 218, 240), DXRGB(  62,  92,  93), DXRGB(  90, 197, 254), DXRGB(  90, 197, 254) , DXRGB( 255, 255, 255)},//10
	{DXRGB( 255, 255, 255), DXRGB( 174, 118,  93), DXRGB( 255, 255, 255), DXRGB( 255, 255, 255) , DXRGB( 255, 255, 255)},//11
	{DXRGB( 255, 255, 255), DXRGB( 174, 118,  93), DXRGB( 255, 255, 255), DXRGB( 255, 255, 255) , DXRGB( 255, 255, 255)},//12
	{DXRGB( 255, 255, 255), DXRGB( 174, 118,  93), DXRGB( 255, 255, 255), DXRGB( 255, 255, 255) , DXRGB( 255, 255, 255)},//13
	{DXRGB( 255, 255, 255), DXRGB( 174, 118,  93), DXRGB( 255, 255, 255), DXRGB( 255, 255, 255) , DXRGB( 255, 255, 255)},//14
	{DXRGB( 255, 255, 255), DXRGB( 174, 118,  93), DXRGB( 255, 255, 255), DXRGB( 255, 255, 255) , DXRGB( 255, 255, 255)},//15
	{DXRGB( 255, 255, 255), DXRGB( 174, 118,  93), DXRGB( 255, 255, 255), DXRGB( 255, 255, 255) , DXRGB( 255, 255, 255)},//16
	{DXRGB( 255, 255, 255), DXRGB(  90,  59,  48), DXRGB( 255, 255, 255), DXRGB( 255, 255, 255) , DXRGB( 255, 255, 255)},//17
	{DXRGB( 255, 255, 255), DXRGB(  90,  59,  48), DXRGB( 255, 255, 255), DXRGB( 255, 255, 255) , DXRGB( 255, 255, 255)},//18
	{DXRGB( 255, 255, 255), DXRGB(  90,  59,  48), DXRGB( 255, 255, 255), DXRGB( 255, 255, 255) , DXRGB( 255, 255, 255)},//19
	{DXRGB( 254, 234, 216), DXRGB(  49,  23,  15), DXRGB( 128, 128, 128), DXRGB( 128, 128, 128) , DXRGB( 255, 204, 170)},//20
	{DXRGB( 254, 234, 216), DXRGB(  49,  23,  15), DXRGB( 128, 128, 128), DXRGB( 128, 128, 128) , DXRGB( 255, 204, 170)},//21
	{DXRGB( 254, 234, 216), DXRGB(  49,  23,  15), DXRGB( 128, 128, 128), DXRGB( 128, 128, 128) , DXRGB( 255, 204, 170)},//22
	{DXRGB( 252, 190, 135), DXRGB(  45,  41,  37), DXRGB(  64,  88, 102), DXRGB(  64,  88, 102) , DXRGB(  43,  51,  62)},//23

	// MAP ID : 5 (늪)
	{DXRGB( 255, 255, 255), DXRGB(   0,   0,   0), DXRGB(  75, 119,  40), DXRGB(  75, 119,  40) , DXRGB( 255, 255, 255)},//0
	{DXRGB( 255, 255, 255), DXRGB(   0,   0,   0), DXRGB(  75, 119,  40), DXRGB(  75, 119,  40) , DXRGB( 255, 255, 255)},//1
	{DXRGB( 255, 255, 255), DXRGB(   0,   0,   0), DXRGB(  75, 119,  40), DXRGB(  75, 119,  40) , DXRGB( 255, 255, 255)},//2
	{DXRGB( 255, 255, 255), DXRGB(   0,   0,   0), DXRGB(  75, 119,  40), DXRGB(  75, 119,  40) , DXRGB( 255, 255, 255)},//3
	{DXRGB( 255, 255, 255), DXRGB(   0,   0,   0), DXRGB( 255, 255, 255), DXRGB( 255, 255, 255) , DXRGB( 255, 255, 255)},//4
	{DXRGB( 255, 255, 255), DXRGB(   0,   0,   0), DXRGB( 255, 255, 255), DXRGB( 255, 255, 255) , DXRGB( 255, 255, 255)},//5
	{DXRGB( 255, 255, 255), DXRGB(   0,   0,   0), DXRGB( 255, 255, 255), DXRGB( 255, 255, 255) , DXRGB( 255, 255, 255)},//6
	{DXRGB( 255, 255, 255), DXRGB(   0,   0,   0), DXRGB( 255, 255, 255), DXRGB( 255, 255, 255) , DXRGB( 255, 255, 255)},//7
	{DXRGB( 255, 255, 255), DXRGB(   0,   0,   0), DXRGB( 255, 255, 255), DXRGB( 255, 255, 255) , DXRGB( 255, 255, 255)},//8
	{DXRGB( 255, 255, 255), DXRGB(   0,   0,   0), DXRGB( 255, 255, 255), DXRGB( 255, 255, 255) , DXRGB( 255, 255, 255)},//9
	{DXRGB( 255, 255, 255), DXRGB(   0,   0,   0), DXRGB( 255, 255, 255), DXRGB( 255, 255, 255) , DXRGB( 255, 255, 255)},//10
	{DXRGB( 255, 255, 255), DXRGB(   0,   0,   0), DXRGB( 255, 255, 255), DXRGB( 255, 255, 255) , DXRGB( 255, 255, 255)},//11
	{DXRGB( 255, 255, 255), DXRGB(   0,   0,   0), DXRGB( 255, 255, 255), DXRGB( 255, 255, 255) , DXRGB( 255, 255, 255)},//12
	{DXRGB( 255, 255, 255), DXRGB(   0,   0,   0), DXRGB( 255, 255, 255), DXRGB( 255, 255, 255) , DXRGB( 255, 255, 255)},//13
	{DXRGB( 255, 255, 255), DXRGB(   0,   0,   0), DXRGB( 255, 255, 255), DXRGB( 255, 255, 255) , DXRGB( 255, 255, 255)},//14
	{DXRGB( 255, 255, 255), DXRGB(   0,   0,   0), DXRGB( 255, 255, 255), DXRGB( 255, 255, 255) , DXRGB( 255, 255, 255)},//15
	{DXRGB( 255, 255, 255), DXRGB(   0,   0,   0), DXRGB( 255, 255, 255), DXRGB( 255, 255, 255) , DXRGB( 255, 255, 255)},//16
	{DXRGB( 255, 255, 255), DXRGB(   0,   0,   0), DXRGB( 255, 255, 255), DXRGB( 255, 255, 255) , DXRGB( 255, 255, 255)},//17
	{DXRGB( 255, 255, 255), DXRGB(   0,   0,   0), DXRGB( 255, 255, 255), DXRGB( 255, 255, 255) , DXRGB( 255, 255, 255)},//18
	{DXRGB( 255, 255, 255), DXRGB(   0,   0,   0), DXRGB( 255, 255, 255), DXRGB( 255, 255, 255) , DXRGB( 255, 255, 255)},//19
	{DXRGB( 255, 255, 255), DXRGB(   0,   0,   0), DXRGB( 255, 255, 255), DXRGB( 255, 255, 255) , DXRGB( 255, 255, 255)},//20
	{DXRGB( 255, 255, 255), DXRGB(   0,   0,   0), DXRGB( 255, 255, 255), DXRGB( 255, 255, 255) , DXRGB( 255, 255, 255)},//21
	{DXRGB( 255, 255, 255), DXRGB(   0,   0,   0), DXRGB( 255, 255, 255), DXRGB( 255, 255, 255) , DXRGB( 255, 255, 255)},//22
	{DXRGB( 255, 255, 255), DXRGB(   0,   0,   0), DXRGB( 255, 255, 255), DXRGB( 255, 255, 255) , DXRGB( 255, 255, 255)} //23
	*/
};

float	g_DayFogDensity[5][24] =
{
	// MAP ID : 1
	0.007f,//0
	0.007f,//1
	0.007f,//2

	0.005f,//3
	0.005f,//4
	0.005f,//5

	0.005f,//6
	0.005f,//7
	0.005f,//8
	0.005f,//9
	0.005f,//10

	0.003f,//11
	0.003f,//12
	0.003f,//13
	0.003f,//14
	0.003f,//15
	0.003f,//16

	0.004f,//17
	0.004f,//18
	0.004f,//19
	0.004f,//20

	0.006f,//21
	0.006f,//22
	0.006f,//23

	// MAP ID : 2
	0.006f,//0
	0.006f,//1
	0.006f,//2

	0.006f,//3
	0.006f,//4
	0.006f,//5
	0.006f,//6
	0.006f,//7

	0.005f,//8
	0.005f,//9
	0.005f,//10
	0.005f,//11
	0.005f,//12
	0.005f,//13
	0.005f,//14
	0.005f,//15
	0.005f,//16
	0.005f,//17
	0.005f,//18
	0.005f,//19

	0.006f,//20
	0.006f,//21
	0.006f,//22
	0.006f,//23

	// MAP ID : 3
	0.006f,//0
	0.006f,//1
	0.006f,//2

	0.006f,//3
	0.006f,//4
	0.006f,//5
	0.006f,//6
	0.006f,//7

	0.005f,//8
	0.005f,//9
	0.005f,//10
	0.005f,//11
	0.005f,//12
	0.005f,//13
	0.005f,//14
	0.005f,//15
	0.005f,//16
	0.005f,//17
	0.005f,//18
	0.005f,//19
	0.005f,//20

	0.006f,//21
	0.006f,//22
	0.006f,//23

	// MAP ID : 4
	0.005f,//0
	0.005f,//1
	0.005f,//2

	0.005f,//3
	0.005f,//4
	0.005f,//5

	0.004f,//6
	0.004f,//7
	0.004f,//8
	0.004f,//9
	0.004f,//10

	0.003f,//11
	0.003f,//12
	0.003f,//13
	0.003f,//14
	0.003f,//15
	0.003f,//16

	0.004f,//17
	0.004f,//18
	0.004f,//19
	0.004f,//20

	0.005f,//21
	0.005f,//22
	0.005f,//23

	// MAP ID : 5
	0.003f,//0
	0.003f,//1
	0.003f,//2
	0.003f,//3
	0.003f,//4
	0.003f,//5
	0.003f,//6
	0.003f,//7
	0.003f,//8
	0.003f,//9
	0.003f,//10
	0.003f,//11
	0.003f,//12
	0.003f,//13
	0.003f,//14
	0.003f,//15
	0.003f,//16
	0.003f,//17
	0.003f,//18
	0.003f,//19
	0.003f,//20
	0.003f,//21
	0.003f,//22
	0.003f //23


	/*
	// MAP ID : 1
	0.009f,//0
	0.009f,//1
	0.009f,//2
	0.007f,//3
	0.007f,//4
	0.007f,//5
	0.005f,//6
	0.005f,//7
	0.005f,//8
	0.005f,//9
	0.005f,//10
	0.003f,//11
	0.003f,//12
	0.003f,//13
	0.003f,//14
	0.003f,//15
	0.003f,//16
	0.005f,//17
	0.005f,//18
	0.005f,//19
	0.005f,//20
	0.007f,//21
	0.007f,//22
	0.007f,//23

	// MAP ID : 2
	0.007f,//0
	0.007f,//1
	0.007f,//2
	0.009f,//3
	0.009f,//4
	0.009f,//5
	0.009f,//6
	0.009f,//7
	0.004f,//8
	0.004f,//9
	0.004f,//10
	0.004f,//11
	0.004f,//12
	0.004f,//13
	0.004f,//14
	0.004f,//15
	0.004f,//16
	0.004f,//17
	0.004f,//18
	0.004f,//19
	0.007f,//20
	0.007f,//21
	0.007f,//22
	0.007f,//23

	// MAP ID : 3
	0.007f,//0
	0.007f,//1
	0.007f,//2
	0.007f,//3
	0.007f,//4
	0.007f,//5
	0.007f,//6
	0.007f,//7
	0.006f,//8
	0.006f,//9
	0.006f,//10
	0.006f,//11
	0.006f,//12
	0.006f,//13
	0.006f,//14
	0.006f,//15
	0.006f,//16
	0.006f,//17
	0.006f,//18
	0.006f,//19
	0.006f,//20
	0.007f,//21
	0.007f,//22
	0.007f,//23

	// MAP ID : 4
	0.005f,//0
	0.005f,//1
	0.005f,//2
	0.006f,//3
	0.006f,//4
	0.006f,//5
	0.004f,//6
	0.004f,//7
	0.004f,//8
	0.004f,//9
	0.004f,//10
	0.003f,//11
	0.003f,//12
	0.003f,//13
	0.003f,//14
	0.003f,//15
	0.003f,//16
	0.004f,//17
	0.004f,//18
	0.004f,//19
	0.004f,//20
	0.004f,//21
	0.004f,//22
	0.005f,//23

	// MAP ID : 5
	0.004f,//0
	0.004f,//1
	0.004f,//2
	0.004f,//3
	0.004f,//4
	0.004f,//5
	0.004f,//6
	0.004f,//7
	0.004f,//8
	0.004f,//9
	0.004f,//10
	0.004f,//11
	0.004f,//12
	0.004f,//13
	0.004f,//14
	0.004f,//15
	0.004f,//16
	0.004f,//17
	0.004f,//18
	0.004f,//19
	0.004f,//20
	0.004f,//21
	0.004f,//22
	0.004f //23
	*/
};

/*
	MAP ID
	기암괴석	Sec2 -> 1
	화산		Sec3 -> 2
	사막		Sec1 -> 4
	빙하		Sec4 -> 3
	늪			Sec5 -> 5
	문파		Battle -> 10
 */

int g_nHour = -1;

/**
 * 시간에 따른 셋팅
 * \param &msg 
 * \return 
 */
int OnCS_EV_TIME_ACK( CMsg &msg)
{
	WORD wYear	=0;
	BYTE bMonth	=0;
	BYTE bDay	=0;
	BYTE bHour	=0;

	msg 
		>> wYear
		>> bMonth
		>> bDay
		>> bHour;
	
	g_MainCharInfo.RefreshTime1( wYear, bMonth, bDay, bHour);
	g_nHour = bHour;

	// 포그, 라이트가 서서히 변화하도록 하기 위해. R과 B값이 바뀌었다
	int nR[5], nG[5], nB[5];

	// MAP ID를 얻고!
	DWORD dwMapID = XiahMap::g_XiahMap.m_MapInfo.m_dwMapID;

	switch(dwMapID)
	{
		// 문파전맵 일때에는 일단 임시 처리로..
	case 10:
	case 6:
	case 7:
	case 15: //HO_0727_07 화염곡추가
		{
			dwMapID = 1;
		}
	    break;
	case 12:	// 마혈성은 화산 지대와 같은 포그 사용하자는구나
	case 13:	// 마혈성은 화산 지대와 같은 포그 사용하자는구나
	case 14:	// 마혈성은 화산 지대와 같은 포그 사용하자는구나
		{
			dwMapID = 2;
		}
		break;
	case 9:
		{
			dwMapID = 3;
		}
		break;
	case 11:	// 던전용
		{
			if( g_nHour == -1 )
			{
				g_XiahEnvInfo.m_DiffuseColor= GetBValue( DXRGB( 115, 113, 164));
				g_XiahEnvInfo.m_FogColor	= GetBValue( DXRGB( 89, 114, 117));
				g_XiahEnvInfo.m_fFogDensity	= 0.004f;
			}
			else
			{
				nR[0] = GetBValue( DXRGB( 115, 113, 164) );	// Diffuse
				nG[0] = GetGValue( DXRGB( 115, 113, 164) );
				nB[0] = GetRValue( DXRGB( 115, 113, 164) );

				nR[1] = GetBValue( DXRGB( 89, 114, 117) );	// FogColor
				nG[1] = GetGValue( DXRGB( 89, 114, 117) );
				nB[1] = GetRValue( DXRGB( 89, 114, 117) );

				nR[2] = GetBValue( DXRGB( 0, 0, 0) );	// Sky Color Bottom
				nG[2] = GetGValue( DXRGB( 0, 0, 0) );
				nB[2] = GetRValue( DXRGB( 0, 0, 0) );

				nR[3] = GetBValue( DXRGB( 0, 0, 0) );	// Sky Color Middle
				nG[3] = GetGValue( DXRGB( 0, 0, 0) );
				nB[3] = GetRValue( DXRGB( 0, 0, 0) );

				nR[4] = GetBValue( DXRGB( 0, 0, 0) );	// Sky Color Top
				nG[4] = GetGValue( DXRGB( 0, 0, 0) );
				nB[4] = GetRValue( DXRGB( 0, 0, 0) );

				for(int i=0; i < 5; ++i)
				{
					g_XiahChangeEnvInfo.nR[i] = nR[i];
					g_XiahChangeEnvInfo.nG[i] = nG[i];
					g_XiahChangeEnvInfo.nB[i] = nB[i];

					g_XiahChangeEnvInfo.fRGap[i] = -300;
					g_XiahChangeEnvInfo.fGGap[i] = -300;
					g_XiahChangeEnvInfo.fBGap[i] = -300;
				}

				g_XiahChangeEnvInfo.fFogDensity		= 0.004f;
				g_XiahChangeEnvInfo.fFogDensityGap	= -300;
				g_XiahChangeEnvInfo.bChangeStart	= true;
			}		

			return 0;
		}
		break;
	default:
		break;
	}

	if( g_nHour == -1 )
	{
		g_XiahEnvInfo.m_DiffuseColor= g_DayFogColor [(dwMapID-1) * 24][0];
		g_XiahEnvInfo.m_FogColor	= g_DayFogColor [(dwMapID-1) * 24][1];
		g_XiahEnvInfo.m_fFogDensity	= g_DayFogDensity [(dwMapID-1)][0];
	}
	else
	{
		nR[0] = GetBValue( g_DayFogColor[(dwMapID-1) * 24 + bHour][0] );	// Diffuse
		nG[0] = GetGValue( g_DayFogColor[(dwMapID-1) * 24 + bHour][0] );
		nB[0] = GetRValue( g_DayFogColor[(dwMapID-1) * 24 + bHour][0] );

		nR[1] = GetBValue( g_DayFogColor[(dwMapID-1) * 24 + bHour][1] );	// FogColor
		nG[1] = GetGValue( g_DayFogColor[(dwMapID-1) * 24 + bHour][1] );
		nB[1] = GetRValue( g_DayFogColor[(dwMapID-1) * 24 + bHour][1] );

		nR[2] = GetBValue( g_DayFogColor[(dwMapID-1) * 24 + bHour][2] );	// Sky Color Bottom
		nG[2] = GetGValue( g_DayFogColor[(dwMapID-1) * 24 + bHour][2] );
		nB[2] = GetRValue( g_DayFogColor[(dwMapID-1) * 24 + bHour][2] );

		nR[3] = GetBValue( g_DayFogColor[(dwMapID-1) * 24 + bHour][3] );	// Sky Color Middle
		nG[3] = GetGValue( g_DayFogColor[(dwMapID-1) * 24 + bHour][3] );
		nB[3] = GetRValue( g_DayFogColor[(dwMapID-1) * 24 + bHour][3] );

		nR[4] = GetBValue( g_DayFogColor[(dwMapID-1) * 24 + bHour][4] );	// Sky Color Top
		nG[4] = GetGValue( g_DayFogColor[(dwMapID-1) * 24 + bHour][4] );
		nB[4] = GetRValue( g_DayFogColor[(dwMapID-1) * 24 + bHour][4] );

		for(int i=0; i < 5; ++i)
		{
			g_XiahChangeEnvInfo.nR[i] = nR[i];
			g_XiahChangeEnvInfo.nG[i] = nG[i];
			g_XiahChangeEnvInfo.nB[i] = nB[i];

			g_XiahChangeEnvInfo.fRGap[i] = -300;
			g_XiahChangeEnvInfo.fGGap[i] = -300;
			g_XiahChangeEnvInfo.fBGap[i] = -300;
		}

		g_XiahChangeEnvInfo.fFogDensity = g_DayFogDensity[(dwMapID-1)][bHour];
		g_XiahChangeEnvInfo.fFogDensityGap = -300;
		g_XiahChangeEnvInfo.bChangeStart = true;


		// 현재 비가 오고 있다면
		if( g_RainSnow.GetType() == eRain )
		{
			// 비가 오면 좀더 안개낀듯.
			nR[1] += RAINFOG_VALUE;
			if( nR[1] > 255 ) nR[1] = 255;
			nG[1] += RAINFOG_VALUE;
			if( nG[1] > 255 ) nG[1] = 255;
			nB[1] += RAINFOG_VALUE;
			if( nB[1] > 255 ) nB[1] = 255;

			g_XiahChangeEnvInfo.nR[1] = nR[1];
			g_XiahChangeEnvInfo.nG[1] = nG[1];
			g_XiahChangeEnvInfo.nB[1] = nB[1];
			g_XiahChangeEnvInfo.fFogDensity += RAINFOG_DENSITY;
		}// if
	}

	/*
	if( bHour >= 7 && bHour <= 19 ) // 해
	{
		g_SkyStar.SetType( eSun );
		g_SkyStar.CreateStarPosition();
	}
	else if( bHour >= 20 || bHour <= 4 )	// 별과 달
	{
		g_SkyStar.SetType( eStars );
		g_SkyStar.CreateCloudPosition();
	}
	else if( bHour >= 5 && bHour <= 6 )	// 달
	{
		g_SkyStar.SetType( eMoon );
	}
	*/

	return 0;
}

/**
 *
 * \param &msg 
 * \return 
 */
int OnCS_EV_WEATHER_ACK( CMsg &msg)
{
	BYTE bWeatherType =0;

	msg	
		>> bWeatherType;
/*
	0:맑음
	1:비
	2:눈
	3:기타
	*/

	DWORD dwMapID = XiahMap::g_XiahMap.m_MapInfo.m_dwMapID;

	switch(dwMapID)
	{
		// 문파전맵 일때에는 일단 임시 처리로.. 
	case 10:
	case 6:
	case 7:
	case 15: //HO_0727_07 화염곡추가
		{
			dwMapID = 1;
		}
	    break;
	case 12:	// 마혈성은 화산 지대와 같은 포그 사용하자는구나
	case 13:	// 마혈성은 화산 지대와 같은 포그 사용하자는구나
	case 14:	// 마혈성은 화산 지대와 같은 포그 사용하자는구나
		{
			dwMapID = 2;
		}
		break;
	case 9:
		{
			dwMapID = 3;
		}
		break;
	case 11:
		{
			return 0;
		}
		break;
	default:
		break;
	}

	// 포그, 라이트가 서서히 변화하도록 하기 위해.
	int nR[4], nG[4], nB[4];

	nR[0] = GetBValue( g_DayFogColor[(dwMapID-1) * 24 + g_nHour][0] );	// Diffuse
	nG[0] = GetGValue( g_DayFogColor[(dwMapID-1) * 24 + g_nHour][0] );
	nB[0] = GetRValue( g_DayFogColor[(dwMapID-1) * 24 + g_nHour][0] );

	nR[1] = GetBValue( g_DayFogColor[(dwMapID-1) * 24 + g_nHour][1] );	// FogColor
	nG[1] = GetGValue( g_DayFogColor[(dwMapID-1) * 24 + g_nHour][1] );
	nB[1] = GetRValue( g_DayFogColor[(dwMapID-1) * 24 + g_nHour][1] );

	nR[2] = GetBValue( g_DayFogColor[(dwMapID-1) * 24 + g_nHour][2] );	// Sky Color Middle
	nG[2] = GetGValue( g_DayFogColor[(dwMapID-1) * 24 + g_nHour][2] );
	nB[2] = GetRValue( g_DayFogColor[(dwMapID-1) * 24 + g_nHour][2] );

	nR[3] = GetBValue( g_DayFogColor[(dwMapID-1) * 24 + g_nHour][3] );	// Sky Color Bottom
	nG[3] = GetGValue( g_DayFogColor[(dwMapID-1) * 24 + g_nHour][3] );
	nB[3] = GetRValue( g_DayFogColor[(dwMapID-1) * 24 + g_nHour][3] );

	for(int i=0; i < 4; ++i)
	{
		g_XiahChangeEnvInfo.nR[i] = nR[i];
		g_XiahChangeEnvInfo.nG[i] = nG[i];
		g_XiahChangeEnvInfo.nB[i] = nB[i];

		g_XiahChangeEnvInfo.fRGap[i] = -300;
		g_XiahChangeEnvInfo.fGGap[i] = -300;
		g_XiahChangeEnvInfo.fBGap[i] = -300;
	}

	g_XiahChangeEnvInfo.fFogDensity		= g_DayFogDensity[(dwMapID-1)][g_nHour];
	g_XiahChangeEnvInfo.fFogDensityGap	= -300;
	g_XiahChangeEnvInfo.bChangeStart	= true;

	//
	int nRainSnowType = g_RainSnow.GetType();

	switch( bWeatherType )
	{
	case 0:
		{
			if( nRainSnowType == eRain || nRainSnowType == eSnow )
				g_RainSnow.End();
		}		
		break;
	case 1:
		{
			if( nRainSnowType != eRain )
				g_RainSnow.Start( eRain );

			if( nRainSnowType == eRain && g_RainSnow.GetStatus() != 1 )
				g_RainSnow.Restart();

			// 비가 오면 좀더 안개낀듯.
			nR[1] += RAINFOG_VALUE;
			if( nR[1] > 255 ) nR[1] = 255;
			nG[1] += RAINFOG_VALUE;
			if( nG[1] > 255 ) nG[1] = 255;
			nB[1] += RAINFOG_VALUE;
			if( nB[1] > 255 ) nB[1] = 255;

			g_XiahChangeEnvInfo.nR[1] = nR[1];
			g_XiahChangeEnvInfo.nG[1] = nG[1];
			g_XiahChangeEnvInfo.nB[1] = nB[1];
			g_XiahChangeEnvInfo.fFogDensity += RAINFOG_DENSITY;			
		}
		break;
	case 2:
		{
			if( nRainSnowType != eSnow )
				g_RainSnow.Start( eSnow );

			if( nRainSnowType == eSnow && g_RainSnow.GetStatus() != 1 )
				g_RainSnow.Restart();
		}		
		break;
	};// switch

	return 0;
}

