#pragma once

struct sXiahEnvInfo : public XiahGameEngine::Map::sMapRenderInfo
{
	D3DCOLOR m_SkyColorUp;
	D3DCOLOR m_SkyColorMiddle;
	D3DCOLOR m_SkyColorBottom;
};

extern sXiahEnvInfo g_XiahEnvInfo;

struct sXiahChangeEnvInfo
{
	bool	bChangeStart;								// 변화되는지

/*
	int		nFogR,			nFogG,			nFogB;			// 목표 값
	float	fFogRGap,		fFogGGap,		fFogBGap;		// 증감 값
	float	fFogRGapSum,	fFogGGapSum,	fFogBGapSum;	// 증감 값 누적치

	int		nDiffuseR,		nDiffuseG,		nDiffuseB;			// 목표 값
	float	fDifRGap,		fDifGGap,		fDifBGap;			// 증감 값
	float	fDifRGapSum,	fDifGGapSum,	fDifBGapSum;		// 증감 값 누적치
*/

	int		nR[5],			nG[5],			nB[5];				// 목표 값
	float	fRGap[5],		fGGap[5],		fBGap[5];			// 증감 값
	float	fRGapSum[5],	fGGapSum[5],	fBGapSum[5];		// 증감 값 누적치

	float fFogDensity;			// 목표 값.
	float fFogDensityGap;		// 증감 값.
};

extern sXiahChangeEnvInfo g_XiahChangeEnvInfo;
