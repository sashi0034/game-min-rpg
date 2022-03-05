#include "ingame_manager.h"

/*
**	íçà”
**	LuaÇÕ1-indexÇ≈Ç∑
*/

namespace game
{
	MapManager::MapManager(int stageIndex)
	{
		sol::table fieldTable = luaManager::Lua["LoadScripte"](R"(.\asset\tilemap\field_01.lua)");

		std::string tilesetFileName = fieldTable["tilesets"][1]["exportfilename"];
		
		std::string tilesetFilePath = R"(.\asset\tilemap\)" + tilesetFileName;
		
		sol::table tilesetTable = luaManager::Lua["LoadScripte"](tilesetFilePath);

		sol::table tiles = tilesetTable["tiles"];

		std::string name = tiles[2]["properties"]["name"].get_or(std::string("NONE"));
		std::string name2 = tiles[2]["properties"]["name2"].get_or(std::string("NONE"));
		std::cout <<  name << " " <<  name2 << "\n";
		if (magic_enum::enum_cast<ETileName>(name) == ETileName::water_place)
		{
			std::cout << "magic_enum Ç‡OK";
		}

		std::cout << tilesetTable.size();
	}
}
