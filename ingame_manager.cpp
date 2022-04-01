#include "stdafx.h"
#include "ingame_manager.h"
#include "start.h"
#include "main_scene.h"
#include "main_scene_player.h"






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
	void MapMatElement::update()
	{
		HasChip = std::vector<byte>(magic_enum::enum_count<ETileName>(), 0);
		for (auto &chip : Chips)
		{
			HasChip[static_cast<int>(chip->Name)] = 1;
		}
	}

	MapMatElement::~MapMatElement()
	{
        // MapManagerでdelteしている
        // chipはMapManagerが持っているものを参照しているので
		//for (auto &chip : Chips)
		//{
		//	delete chip;
		//}
	}


	MapManager::MapManager(int stageIndex) : Singleton<MapManager>()
	{
		sol::table fieldTable = luaManager::Lua["LoadScript"](R"(.\asset\tilemap\field_01.lua)");

		std::cout OUT_LOG fieldTable["tilesets"][1]["exportfilename"].get<std::string>() << "\n";

		std::string tilesetFilePath = R"(.\asset\tilemap\)" + fieldTable["tilesets"][1]["exportfilename"].get<std::string>();
		
		sol::table tilesetTable = luaManager::Lua["LoadScript"](tilesetFilePath);

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

                /*
                // プロパティ追加コード 1 / 2
                */
				bool isWall = prop["wall"].get_or(false);
                bool isBridge = prop["bridge"].get_or(false);
                bool isStepUp = prop["step_up"].get_or(false);
                bool isStepRight = prop["step_right"].get_or(false);
                bool isStepLeft = prop["step_left"].get_or(false);

				int srcX = (id % tileCollums) * tileWidth;
				int srcY = (id / tileCollums) * tileHeight;

				mTilechips[id] = new TileMapChip{
                    eName, 
                    isWall, 
                    isBridge, 
                    isStepUp,
                    isStepLeft,
                    isStepRight,
                    srcX, srcY};
			}
			else
			{
				std::cerr ERR_LOG "Invalid tile is exist.\n";
			}
		}

		
		mSprWidth = fieldTable["width"].get_or(0);
		mSprHeight = fieldTable["height"].get_or(0);


		// タイルマップの読み込み
		if (mSprWidth != 0 && mSprHeight != 0)
		{
			mMat.resize(mSprWidth, std::vector<MapMatElement*>(mSprHeight));
			for (auto& row : mMat)
				for (auto& ele : row)
					ele = new MapMatElement();

			sol::table layers = fieldTable["layers"];
			for (int i = 1; i <= layers.size(); ++i)
			{
                std::string layerType = layers[i]["type"];
                if (layerType =="tilelayer")
                {// タイルレイヤー
                    loadTileLayer(layers[i]);
                }
                else if (layerType == "objectgroup")
                {
                    loadObjectGroup(layers[i]);
                }
	
			}

		}

		std::string imagePath = R"(.\asset\tilemap\)" + tilesetTable["image"].get_or(std::string{});
		mTilesetGraph = Graph::LoadGraph(imagePath.c_str());
		std::cout OUT_LOG "Get tilemap sorce image handler: " << mTilesetGraph->GetHandler() << "\n";


		for (auto& row : mMat)
			for (auto& ele : row)
				ele->update();

        ScrollManager::Sole->SetRange(-(mSprWidth*16-GRID_WIDTH), -(mSprHeight*16-GRID_HEIGHT), 0, 0);
	}


    /// <summary>
    /// タイルレイヤー1枚の読み込み
    /// </summary>
    /// <param name="layer">Luaテーブル</param>
    void MapManager::loadTileLayer(sol::table layer)
    {
        for (int y = 0; y < mSprHeight; ++y)
        {
            for (int x = 0; x < mSprWidth; ++x)
            {
                // Tiledのレイヤー要素のマップタイルのIDは+1加算されているので
                // -1する必要がある
                int chipNo = layer["data"][x + y * mSprWidth + 1].get_or(0) - 1;
                if (mTilechips.count(chipNo) != 0)
                {
                    TileMapChip* chip = mTilechips[chipNo];

                    if (!doStartChip(x, y, chip->Name)) mMat[x][y]->Chips.push_back(chip);

                    /*
                    // プロパティ追加コード 2 / 2
                    */
                    mMat[x][y]->IsWall = chip->IsWall;
                    if (chip->IsBridge) mMat[x][y]->IsBridge = true;

                    if (chip->IsStepLeft && x - 1 >= 0) {
                        mMat[x][y]->IsStep[(int)EAngle::LEFT] = true;
                        mMat[x - 1][y]->IsStep[(int)EAngle::RIGHT] = true;
                    }
                    if (chip->IsStepRight && x + 1 <= mSprWidth - 1) {
                        mMat[x][y]->IsStep[(int)EAngle::RIGHT] = true;
                        mMat[x + 1][y]->IsStep[(int)EAngle::LEFT] = true;
                    }
                    if (chip->IsStepUp && y - 1 >= 0) {
                        mMat[x][y]->IsStep[(int)EAngle::DOWN] = true;
                        mMat[x][y - 1]->IsStep[(int)EAngle::UP] = true;
                    }
                }
                else if (chipNo != -1)
                {
                    std::cerr ERR_LOG "Invalid tile " << chipNo << " exits in " << x << ", " << y << "\n";
                }
            }
        }
    }

    void MapManager::loadObjectGroup(sol::table layer)
    {
        sol::table objects = layer["objects"];

        for (int i = 1; i <= objects.size(); ++i)
        {
            std::string name = objects[i]["name"].get_or(std::string(""));
            std::string typeName = objects[i]["type"].get_or(std::string(""));
            int x = static_cast<int>(objects[i]["x"].get_or(0));
            int y = static_cast<int>(objects[i]["y"].get_or(0));
            int matX = x / 16;
            int matY = y / 16;

            if (typeName == "character")
            {// キャラクター
                std::string characterName = objects[i]["properties"]["kind"].get_or(std::string(""));

                Character::InstallCharacter(x, y, characterName, name);
            }
            else if (typeName == "event")
            {// イベント
                std::string kind = objects[i]["properties"]["kind"].get_or(std::string(""));
                if (kind == "reach")
                {
                    mMat[matX][matY]->Events.ReachEvents.push_back(name);
                }
                else if (kind == "touch")
                {
                    mMat[matX][matY]->Events.TouchEvents.push_back(name);
                }
                else if (kind == "unique")
                {
                    mUniqueEvents[name] = new UniqueEventValue{x, y};
                }
                else
                {
                    std::cerr ERR_LOG "Invalid event kind exist in " << x << ", " << y << "\n";
                }
            }

        }

    }

 



	MapManager::~MapManager()
	{
		delete mTilesetGraph;

        for (auto& tile : mTilechips)
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
        for (auto iter = mUniqueEvents.begin(); iter != mUniqueEvents.end(); ++iter)
        {
            delete iter->second;
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
        case ETileName::castle:
            new main::Castle(x, y);
            return true;
		default:
			return false;
		}
		return false;
	}

    UniqueEventValue* MapManager::GetUniqueEvent(std::string key)
    {
        if (mUniqueEvents.count(key) != 0)
            return mUniqueEvents[key];
        else
            std::cerr ERR_LOG "Event vent key `" << key << "` is not exit.\n";
            return nullptr;
    }


	Graph* MapManager::GetTilesetGraph()
	{
		return mTilesetGraph;
	}

	int MapManager::GetWidth()
	{
		return mSprWidth;
	}

	int MapManager::GetHeight()
	{
		return mSprHeight;
	}

	bool MapManager::IsInRange(int x, int y)
	{
		return 
			0<=x && x<=mSprWidth-1 &&
			0<=y && y<=mSprHeight-1;
	}


    INonPlayerCharacter::INonPlayerCharacter(ECharacterKind characterKind, std::string uniqueName)
    {
        mCharacterKind = characterKind;
        mUniqueName = uniqueName;
    }

}

namespace ingame::main
{

    ScrollManager::ScrollManager() : Actor()
    {
        Sole = this;

    }
    void ScrollManager::update()
    {   
        mX = -(Player::Sole->GetX() + 8) + GRID_WIDTH / 2;
        mY = -(Player::Sole->GetY() + 8) + GRID_HEIGHT / 2;

        mX = (std::min)((std::max)(mX, double(mMinX)), double(mMaxX));
        mY = (std::min)((std::max)(mY, double(mMinY)), double(mMaxY));

        mSpr->SetXY(mX, mY);
    }

    double ScrollManager::GetX()
    {
        return mX;
    }

    double ScrollManager::GetY()
    {
        return mY;
    }

    useful::Vec2<int> ScrollManager::GetMaxXY()
    {
        return useful::Vec2<int>{mMaxX, mMaxY};
    }

    useful::Vec2<int> ScrollManager::GetMinXY()
    {
        return useful::Vec2<int>{mMinX, mMinY};
    }

    void ScrollManager::SetRange(int minX, int minY, int maxX, int maxY)
    {
        mMinX = minX;
        mMinY = minY;
        mMaxX = maxX;
        mMaxY = maxY;
    }

    ScrollManager::~ScrollManager()
    {
        Sole = nullptr;
    }




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
        mSpr->SetLinkXY(ScrollManager::Sole->GetSpr());
    }
    void FieldLayerBase::drawing(int hX, int hY)
    {
        double hX1 = hX / PX_PER_GRID;
        double hY1 = hY / PX_PER_GRID;

        int x0 = int((-hX1 / mGridUnit) - (-hX1 < 0 ? 1 : 0));
        int y0 = int((-hY1 / mGridUnit) - (-hY1 < 0 ? 1 : 0));

        for (int y = std::max(y0, 0); y <= std::min(y0 + (GRID_HEIGHT / mGridUnit), MapManager::Sole->GetHeight() - 1); ++y)
        {
            for (int x = std::max(x0, 0); x <= std::min(x0 + (GRID_WIDTH / mGridUnit), MapManager::Sole->GetWidth() - 1); ++x)
            {
                int displayX = hX + x * mGridUnit * PX_PER_GRID;
                int displayY = hY + y * mGridUnit * PX_PER_GRID;

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
        useful::Vec2<int> clipXY[16] = {
            useful::Vec2<int>{0 * unit, 0 * unit}, //        
            useful::Vec2<int>{0 * unit, 1 * unit}, //       D
            useful::Vec2<int>{0 * unit, 3 * unit}, //     U  
            useful::Vec2<int>{0 * unit, 2 * unit}, //     U D

            useful::Vec2<int>{1 * unit, 0 * unit}, //   R    
            useful::Vec2<int>{1 * unit, 1 * unit}, //   R   D
            useful::Vec2<int>{1 * unit, 3 * unit}, //   R U  
            useful::Vec2<int>{1 * unit, 2 * unit}, //   R U D

            useful::Vec2<int>{3 * unit, 0 * unit}, // L      
            useful::Vec2<int>{3 * unit, 1 * unit}, // L     D
            useful::Vec2<int>{3 * unit, 3 * unit}, // L   U  
            useful::Vec2<int>{3 * unit, 2 * unit}, // L   U D

            useful::Vec2<int>{2 * unit, 0 * unit}, // L R    
            useful::Vec2<int>{2 * unit, 1 * unit}, // L R   D
            useful::Vec2<int>{2 * unit, 3 * unit}, // L R U  
            useful::Vec2<int>{2 * unit, 2 * unit}, // L R U D
        };

        int left = static_cast<int>(canConnect(matX - 1, matY));
        int right = static_cast<int>(canConnect(matX + 1, matY));
        int up = static_cast<int>(canConnect(matX, matY - 1));
        int down = static_cast<int>(canConnect(matX, matY + 1));
        int index = (left << 3) + (right << 2) + (up << 1) + (down << 0);


        auto drawQuarter = [&](int x1, int y1) {
            srcImage->DrawGraph(
                dpX + x1 * unit / 2 * PX_PER_GRID, 
                dpY + y1 * unit / 2 * PX_PER_GRID, 
                srcX + clipXY[index].X + x1 * unit / 2, 
                srcY + y1 * unit / 2 + clipXY[index].Y, 
                unit / 2, unit / 2, PX_PER_GRID);
        };

        int drawedFlag = 0;

        if (left != 0 && up != 0 && !canConnect(matX - 1, matY - 1))
        {// 角あり
            srcImage->DrawGraph(dpX, dpY, srcX, srcY + unit * 4, unit, unit, PX_PER_GRID);
            drawedFlag += 1 << 0;
        }
        if (right != 0 && up != 0 && !canConnect(matX + 1, matY - 1))
        {
            srcImage->DrawGraph(dpX, dpY, srcX + unit * 1, srcY + unit * 4, unit, unit, PX_PER_GRID);
            drawedFlag += 1 << 1;
        }
        if (left != 0 && down != 0 && !canConnect(matX - 1, matY + 1))
        {
            srcImage->DrawGraph(dpX, dpY, srcX + unit * 0, srcY + unit * 5, unit, unit, PX_PER_GRID);
            drawedFlag += 1<<2;
        }
        if (right != 0 && down != 0 && !canConnect(matX + 1, matY + 1))
        {
            srcImage->DrawGraph(dpX, dpY, srcX + unit * 1, srcY + unit * 5, unit, unit, PX_PER_GRID);
            drawedFlag += 1 << 3;
        }
        if (drawedFlag==0)
        {// 通常
            srcImage->DrawGraph(dpX, dpY, srcX + clipXY[index].X, srcY + clipXY[index].Y, unit, unit, PX_PER_GRID);
        }
        else
        {
            for (int i = 0; i < 4; ++i)
            {
                if (!(drawedFlag & (1 << i)))  drawQuarter(i % 2, i / 2);
            }
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



    FloorLayer::FloorLayer() : FieldLayerBase(double(ZIndex::FLOOR))
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
        case ETileName::rock_zone:
            drawingAutoTile(matX, matY, dpX, dpY, Images->RockZone, 16 * 4 * 0, 0,
                [&](int x, int y) {return canConnect(x, y, ETileName::rock_zone); });
            break;
        default:
            MapManager::Sole->GetTilesetGraph()->DrawGraph(dpX, dpY, chip->srcX, chip->srcY, 16, 16, PX_PER_GRID);
            break;
        }

    }

}








