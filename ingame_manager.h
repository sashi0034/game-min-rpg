#pragma once

#include <iostream>
#include <string.h>
#include <map>

#include "magic_enum.h"

#include "game_utils.h"
#include "lua_manager.h"


using namespace gameUtils;

namespace game
{

	enum class ETileName
	{
		NONE,
		water_place,
		sand_pit,
		meadows,
		grass,
		tree,
		house,
		weed
	};

	struct TileProps
	{
		ETileName name = ETileName::NONE;
		bool isRaw = false;
	};

	class MapManager
	{
		Graph* mTileSetGraph;
		std::vector<std::map<int, ETileName>> mTilechips{};
	public:
		MapManager(int stageIndex);
	};
}

