#include "ingame_manager.h"
#include "start.h"
#include "main_scene.h"







/*
** 注意
** Luaは1-indexです
**
** 注意
** Luaは1-indexです
**
** 大事なことなので2回言いました
*/
namespace ingame::main
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
		for (auto &chip : Chips)
		{
			delete &chip;
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
			sol::table prop = tiles[i]["properties"].get_or(luaManager::Lua.create_table());
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
							
							if (!doStartChip(x, y, chip->Name)) mMat[x][y]->Chips.push_back(chip);

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
				delete &element;
			}
		}
	}
	MapMatElement* MapManager::GetMatAt(int x, int y)
	{
		return mMat[x][y];
	}

	bool MapManager::doStartChip(int x, int y, ETileName tile)
	{
		switch (tile)
		{
		case ETileName::tree:
			new main::Tree(x, y);
			return true;
		case ETileName::weed:
			new main::Weed(x, y);
			return true;
		default:
			return false;
		}
		return false;
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

namespace ingame::main
{


    BackGroundManager::BackGroundManager() : SelfDrawingActor()
    {
        new FloorLayer();
    }
    void BackGroundManager::update()
    {
        SelfDrawingActor::update();
    }
    void BackGroundManager::drawing(int hX, int hY)
    {

    }


    FieldLayerBase::FieldLayerBase(double z) : SelfDrawingActor()
    {
        mZ = z;
        mSpr->SetZ(z);
    }
    void FieldLayerBase::drawing(int hX, int hY)
    {
        double hX1 = hX / ROUGH_SCALE;
        double hY1 = hY / ROUGH_SCALE;

        int x0 = (-hX / mGridUnit) - (-hX1 < 0 ? 1 : 0);
        int y0 = (-hX / mGridUnit) - (-hY1 < 0 ? 1 : 0);

        for (int y = y0; y <= y0 + (ROUGH_HEIGHT / mGridUnit); ++y)
        {
            for (int x = x0; x <= x0 + (ROUGH_WIDTH / mGridUnit); ++x)
            {
                int displayX = hX + x * mGridUnit * ROUGH_SCALE;
                int displayY = hY + y * mGridUnit * ROUGH_SCALE;

                MapMatElement* element = MapManager::Sole->GetMatAt(x, y);
                std::vector<TileMapChip*> chips = element->Chips;
                for (auto chip : chips)
                {
                    drawingChip(x, y, displayX, displayY, chip);
                }
            }
        }
    }


    void FieldLayerBase::drawingAutoTile(int matX, int matY, int dpX, int dpY,
        Graph* srcImage, int srcX, int srcY, std::function<bool(int x, int y)> canConnect)
    {
        const int unit = 16;
        useful::XY<int> clipXY[16] = {
            useful::XY<int>{0 * unit, 0 * unit}, //        
            useful::XY<int>{0 * unit, 3 * unit}, //       D
            useful::XY<int>{0 * unit, 1 * unit}, //     U  
            useful::XY<int>{1 * unit, 2 * unit}, //     U D

            useful::XY<int>{1 * unit, 0 * unit}, //   R    
            useful::XY<int>{1 * unit, 1 * unit}, //   R   D
            useful::XY<int>{1 * unit, 3 * unit}, //   R U  
            useful::XY<int>{1 * unit, 2 * unit}, //   R U D

            useful::XY<int>{3 * unit, 0 * unit}, // L      
            useful::XY<int>{3 * unit, 1 * unit}, // L     D
            useful::XY<int>{3 * unit, 3 * unit}, // L   U  
            useful::XY<int>{3 * unit, 2 * unit}, // L   U D

            useful::XY<int>{2 * unit, 0 * unit}, // L R    
            useful::XY<int>{2 * unit, 1 * unit}, // L R   D
            useful::XY<int>{2 * unit, 3 * unit}, // L R U  
            useful::XY<int>{2 * unit, 2 * unit}, // L R U D
        };

        int left = static_cast<int>(canConnect(matX - 1, matY));
        int right = static_cast<int>(canConnect(matX + 1, matY));
        int up = static_cast<int>(canConnect(matX, matY - 1));
        int down = static_cast<int>(canConnect(matX, matY + 1));
        int index = (left << 3) + (right << 2) + (up << 1) + (down << 0);

        srcImage->DrawGraph(dpX, dpY, srcX + clipXY[index].X, srcY + clipXY[index].Y, unit, unit, ROUGH_SCALE);

        if (left != 0 && up != 0 && !canConnect(matX - 1, matY - 1))
        {
            srcImage->DrawGraph(dpX, dpY, srcX, srcY + unit * 4, unit, unit, ROUGH_SCALE);
        }
        else if (right != 0 && up != 0 && !canConnect(matX + 1, matY - 1))
        {
            srcImage->DrawGraph(dpX, dpY, srcX + unit * 1, srcY + unit * 4, unit, unit, ROUGH_SCALE);
        }
        else if (left != 0 && down != 0 && !canConnect(matX - 1, matY + 1))
        {
            srcImage->DrawGraph(dpX, dpY, srcX + unit * 0, srcY + unit * 5, unit, unit, ROUGH_SCALE);
        }
        else if (right != 0 && down != 0 && !canConnect(matX + 1, matY + 1))
        {
            srcImage->DrawGraph(dpX, dpY, srcX + unit * 1, srcY + unit * 5, unit, unit, ROUGH_SCALE);
        }

    }

    bool FieldLayerBase::canConnect(int x, int y, ETileName tile)
    {
        return MapManager::Sole->IsInRange(x, y)
            ?
            MapManager::Sole->GetMatAt(x, y)->HasChip[static_cast<int>(tile)] != 0
            : true;
    }
    bool FieldLayerBase::canConnect(int x, int y, ETileName tile1, ETileName tile2)
    {
        return MapManager::Sole->IsInRange(x, y)
            ?
            MapManager::Sole->GetMatAt(x, y)->HasChip[static_cast<int>(tile1)] != 0 ||
            MapManager::Sole->GetMatAt(x, y)->HasChip[static_cast<int>(tile2)] != 0
            : true;
    }
    bool FieldLayerBase::canConnect(int x, int y, ETileName tile1, ETileName tile2, ETileName tile3)
    {
        return MapManager::Sole->IsInRange(x, y)
            ?
            MapManager::Sole->GetMatAt(x, y)->HasChip[static_cast<int>(tile1)] != 0 ||
            MapManager::Sole->GetMatAt(x, y)->HasChip[static_cast<int>(tile2)] != 0 ||
            MapManager::Sole->GetMatAt(x, y)->HasChip[static_cast<int>(tile3)] != 0
            : true;
    }
    bool FieldLayerBase::canConnect(int x, int y, ETileName tile1, ETileName tile2, ETileName tile3, ETileName tile4)
    {
        return MapManager::Sole->IsInRange(x, y)
            ?
            MapManager::Sole->GetMatAt(x, y)->HasChip[static_cast<int>(tile1)] != 0 ||
            MapManager::Sole->GetMatAt(x, y)->HasChip[static_cast<int>(tile2)] != 0 ||
            MapManager::Sole->GetMatAt(x, y)->HasChip[static_cast<int>(tile3)] != 0 ||
            MapManager::Sole->GetMatAt(x, y)->HasChip[static_cast<int>(tile4)] != 0
            : true;
    }



    FloorLayer::FloorLayer() : FieldLayerBase(main::ZIndex::FLOOR)
    {
    }

    void FloorLayer::drawingChip(int matX, int matY, int dpX, int dpY, TileMapChip* chip)
    {


        switch (chip->Name)
        {
        case ETileName::water_place:
            drawingAutoTile(matX, matY, dpX, dpY, Images->NaturalTile, 0, 0,
                [&](int x, int y) {return canConnect(x, y, ETileName::water_place); });
            break;
        case ETileName::sand_pit:
            drawingAutoTile(matX, matY, dpX, dpY, Images->NaturalTile, 16 * 4, 0,
                [&](int x, int y) {return canConnect(x, y, ETileName::sand_pit); });
            break;
        case ETileName::meadows:
            drawingAutoTile(matX, matY, dpX, dpY, Images->NaturalTile, 16 * 4 * 2, 0,
                [&](int x, int y) {return canConnect(x, y, ETileName::meadows); });
            break;
        default:
            MapManager::Sole->GetTilesetGraph()->DrawGraph(dpX, dpY, chip->srcX, chip->srcY, 16, 16, ROUGH_SCALE);
            break;
        }

    }

}








