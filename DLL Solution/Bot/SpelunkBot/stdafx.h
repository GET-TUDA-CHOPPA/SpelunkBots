// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently, but
// are changed infrequently
//

#pragma once

#include "targetver.h"

#define WIN32_LEAN_AND_MEAN
#define _ATL_CSTRING_EXPLICIT_CONSTRUCTORS 

#include <windows.h>
#include <atlbase.h>
#include <atlstr.h>
#include <iostream>

enum TerrainType
{
	spEmptyNode = 0,
	spStandardTerrain,
	spLadder,
	spExit,
	spEntrance,
	spSacAltar,
	spArrowTrapRight,
	spArrowTrapLeft,
	spIsInShop,
	spIce,
	spSpike,
	spSpearTrap,
	spGameEntrance
};

enum Collectable
{
	spGoldBar = 1,
	spGoldBars,
	spEmerald,
	spEmeraldBig,
	spSapphire,
	spSapphireBig,
	spRuby,
	spRubyBig,
	spDiamond,
	spGoldNugget,
	spGoldChunk,
};