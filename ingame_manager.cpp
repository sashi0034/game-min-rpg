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
	void MapMatElement::Update()
	{
		HasChip = std::vector<byte>(magic_enum::enum_count<ETileName>(), 0);
		for (auto &chip : Chips)
		{
			HasChip[static_cast<int>(chip->Name)] = 1;
		}
	}

	MapMatElement::~MapMatElement()
	{
		for (auto chip : Chips)
		{
			delete chip;
		}
	}


	MapManager::MapManager(int stageIndex) : Singleton<MapManager>()
	{
		sol::table fieldTable = luaManager::Lua["LoadScripte"](R"(.\asset\tilemap\field_01.lua)");

		std::cout OUT_LOG fieldTable["tilesets"][1]["exportfilename"].get<std::string>() << "\n";

		std::string tilesetFilePath = R"(.\asset\tilemap\)" + fieldTable["tilesets"][1]["exportfilename"].get<std::string>();
		
		sol::table tilesetTable = luaManager::Lua["LoadScripte"](tilesetFilePath);

		sol::table tiles = tilesetTable["tiles"];

		int tileCollums = tilesetTable["columns"].get_or(0);
		int tileWidth = tilesetTable["tilewidth"].get_or(0);
		int tileHeight = tilesetTable["tileheight"].get_or(0);

		// タイルパレットの読み込み
		for (int i = 1; i <= tiles.size(); i++)
		{
			int id = tiles[i]["id"].get_or(-1);
			sol::table prop = tiles[i]["properties"].get_or(sol::table{});
			if (id != -1)
			{
				std::string name = prop["name"].get_or(std::string("none"));
				ETileName eName = magic_enum::enum_cast<ETileName>(name).value_or(ETileName::none);

				bool isWall = prop["wall"].get_or(false);

				int srcX = (id % tileCollums) * tileWidth;
				int srcY = (id / tileCollums) * tileHeight;

				mTilechips[id] = new TileMapChip{eName, isWall, srcX, srcY};
			}
			else
			{
				std::cerr ERR_LOG "Invalid tile is exist.\n";
			}
		}

		
		mWidth = fieldTable["width"].get_or(0);
		mHeight = fieldTable["height"].get_or(0);


		// タイルマップの読み込み
		if (mWidth != 0 && mHeight != 0)
		{
			mMat.resize(mWidth, std::vector<MapMatElement*>(mHeight));
			for (auto& row : mMat)
				for (auto& ele : row)
					ele = new MapMatElement();

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
							TileMapChip* chip = mTilechips[chipNo];
							mMat[x][y]->Chips.push_back(chip);
							if (chip->IsWall) mMat[x][y]->IsWall = true;
						}
						else if (chipNo != -1)
						{
							std::cerr ERR_LOG "Invalid tile "<< chipNo <<" exits in "<< x << ", " << y << "\n";
						}
					}
				}
			}

		}

		std::string imagePath = R"(.\asset\tilemap\)" + tilesetTable["image"].get_or(std::string{});
		mTilesetGraph = Graph::LoadGraph(imagePath.c_str());
		std::cout OUT_LOG "Get tilemap sorce image handler: " << mTilesetGraph->GetHandler() << "\n";


		for (auto& row : mMat)
			for (auto& ele : row)
				ele->Update();
	}

	MapManager::~MapManager()
	{
		delete mTilesetGraph;
		for (auto &tile : mTilechips)
		{
			delete (&tile)->second;
		}
		for (auto &raw : mMat)
		{
			for (auto& element : raw)
			{
				delete element;
			}
		}
	}
	MapMatElement* MapManager::GetMatAt(int x, int y)
	{
		return mMat[x][y];
	}

	Graph* MapManager::GetTilesetGraph()
	{
		return mTilesetGraph;
	}

	int MapManager::GetWidth()
	{
		return mWidth;
	}

	int MapManager::GetHeight()
	{
		return mHeight;
	}

	bool MapManager::IsInRange(int x, int y)
	{
		return 
			0<=x && x<=mWidth-1 &&
			0<=y && y<=mHeight-1;
	}

}
