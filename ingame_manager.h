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
	template <typename T>
	class Singleton
	{
	public:
		Singleton()
		{
			if (Sole != nullptr)
			{
				std::cerr ERR_LOG "Singleton object is already exit\n";
				return;
			}
			Sole = static_cast<T*>(this);
		}
		~Singleton()
		{
			Sole = nullptr;
		}
		static T* Sole;
	};
	template <typename T> T* Singleton<T>::Sole = nullptr;


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

	struct TileMapChip
	{
		ETileName Name;
		bool IsRaw;
		bool IsWall;
		int srcX;
		int srcY;
	};

	struct MapMatElement
	{
		std::vector<TileMapChip*> Chips{};
		bool IsWall = false;
		~MapMatElement();
	};

	class MapManager : public Singleton<MapManager>
	{
		Graph* mTilesetGraph;
		std::map<int, TileMapChip*> mTilechips{};
		
		int mWidth;
		int mHeight;
		std::vector<std::vector<MapMatElement*>> mMat{};
		
	public:
		MapManager(int stageIndex);
		~MapManager();
		MapMatElement* GetMatAt(int x, int y);
		Graph* GetTilesetGraph();
	};
}

