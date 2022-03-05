#pragma once

#include <iostream>
#include <string.h>
#include <map>
#include <vector>

#include "magic_enum.h"

#include "game_utils.h"
#include "lua_manager.h"


using namespace gameUtils;

namespace ingame
{



	enum class ETileName
	{
		none,
		water_place,
		sand_pit,
		meadows,
		grass,
		tree,
		house,
		weed
	};

	struct TilePallete
	{
		ETileName Name;
		bool IsRaw;
		bool IsWall;
		int srcX;
		int srcY;
	};

	struct MapMatElement
	{
		std::vector<TilePallete*> elements{};
		bool IsWall = false;
	};

	class MapManager
	{
		Graph* mTileSetGraph;
		std::map<int, TilePallete*> mTilechips{};
		
		int mWidth;
		int mHeight;
		std::vector<std::vector<MapMatElement*>> mMat{};
		
	public:
		MapManager(int stageIndex);
	};
}

