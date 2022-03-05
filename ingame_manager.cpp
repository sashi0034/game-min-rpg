#include "ingame_manager.h"












/*
** 注意
** Luaは1-indexです
**
** 注意
** Luaは1-indexです
**
** 大事なことなので2回言いました
*/
namespace ingame
{
	MapManager::MapManager(int stageIndex)
	{
		sol::table fieldTable = luaManager::Lua["LoadScripte"](R"(.\asset\tilemap\field_01.lua)");

		std::cout OUT_LOG fieldTable["tilesets"][1]["exportfilename"].get<std::string>() << "\n";

		std::string tilesetFilePath = R"(.\asset\tilemap\)" + fieldTable["tilesets"][1]["exportfilename"].get<std::string>();
		
		sol::table tilesetTable = luaManager::Lua["LoadScripte"](tilesetFilePath);

		sol::table tiles = tilesetTable["tiles"];

		int tileCollums = tilesetTable["columns"].get_or(0);
		int tileWidth = tilesetTable["tilewidth"].get_or(0);
		int tileHeight = tilesetTable["tileheight"].get_or(0);

		for (int i = 1; i <= tiles.size(); i++)
		{
			int id = tiles[i]["id"].get_or(-1);
			sol::table prop = tiles[i]["properties"].get_or(sol::table{});
			if (id != -1)
			{
				std::string name = prop["name"].get_or(std::string("none"));
				ETileName eName = magic_enum::enum_cast<ETileName>(name).value_or(ETileName::none);

				bool isRaw = prop["raw"].get_or(false);

				bool isWall = prop["wall"].get_or(false);

				int srcX = (id % tileCollums) * tileWidth;
				int srcY = (id / tileCollums) * tileHeight;

				mTilechips[id] = new TilePallete{eName, isRaw, isWall, srcX, srcY};
			}
			else
			{
				std::cerr ERR_LOG "Invalid tile is exist.\n";
			}
		}

		
		mWidth = fieldTable["width"].get_or(0);
		mHeight = fieldTable["height"].get_or(0);



		if (mWidth != 0 && mHeight != 0)
		{
			mMat.resize(mWidth, std::vector<MapMatElement*>(mHeight, new MapMatElement()));

			sol::table layers = fieldTable["layers"];
			for (int i = 1; i <= layers.size(); ++i)
			{
				for (int y = 0; y < mHeight; ++y)
				{
					for (int x = 0; x < mWidth; ++x)
					{
						// Tiledのレイヤー要素のマップタイルのIDは+1加算されているので
						// -1する必要がある
						int chipNo = layers[i]["data"][x + y * mWidth + 1].get_or(0) - 1;
						if (mTilechips.count(chipNo) != 0)
						{
							TilePallete* pal = mTilechips[chipNo];
							mMat[x][y]->elements.push_back(pal);
							if (pal->IsWall) mMat[x][y]->IsWall = true;
						}
						else if (chipNo != -1)
						{
							std::cerr ERR_LOG "Invalid tile "<< chipNo <<" exits in "<< x << ", " << y << "\n";
						}
					}
				}
			}
		}





	}
}
