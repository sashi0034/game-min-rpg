#include "main.h"


#define LOOP    (DxLib::ProcessMessage() != -1 && (!luaManager::CanRestartProgram))





int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
    {//デバッグ用コンソールウインドウの表示
        AllocConsole();
        FILE* stream;
        freopen_s(&stream, "CONIN$", "r", stdin);
        freopen_s(&stream, "CONOUT$", "w", stdout);
        freopen_s(&stream, "CONOUT$", "w", stderr);
    }

    return ingame::doProcess();
    
}









namespace ingame
{
    int doProcess()
    {
        // ウインドウモードで起動
        DxLib::ChangeWindowMode(TRUE);

        // フルスクリーンモード時のモニターの解像度を最大にするモードに設定
        DxLib::SetFullScreenResolutionMode(DX_FSRESOLUTIONMODE_NATIVE);

        // フルスクリーンモード時の画面の拡大モードを最近点モードに設定
        DxLib::SetFullScreenScalingMode(DX_FSSCALINGMODE_NEAREST);

        DxLib::SetGraphMode(FULL_WIDTH, FULL_HEIGHT, 16);

        DxLib::SetAlwaysRunFlag(1); //非アクティブ状態でも動かす

        if (DxLib::DxLib_Init() == -1)        // DXライブラリ初期化処理
        {
            return -1;        // エラーが起きたら直ちに終了
            
        }
        Sprite::Init();

        Images = new resorce::Image(); //画像読み込み


    restart:
        
        if (luaManager::SolStart()==-1) return -1;

        std::cout OUT_LOG "Game is started.\n";

        //シーン
        SceneTransition();

        if (luaManager::CanRestartProgram)
        {// 再起動
            luaManager::CanRestartProgram = false;
            Sprite::DisposeAll();
            printfDx("再起動完了\n");
            goto restart;
        }

        printf("game is end\n");

        Sprite::End();
        DxLib::DxLib_End();        // ＤＸライブラリ使用の終了処理


        return 0;
        // ソフトの終了
    }


    int SceneTransition()
    {
        //new test::Scene();
        while (LOOP)
        {
            //new top::Scene();
            new main::Scene();
        }

        return 0;
    }


    void LoopBasicUpdate()
    {
        Sprite::UpdateAll();
        DxLib::SetDrawScreen(DX_SCREEN_BACK);
        DxLib::ClearDrawScreen();
        Sprite::DrawingAll();
        DxLib::ScreenFlip();
    }
}



namespace ingame::resorce 
{
    Graph* Image::loadPng(std::string fileName)
    {
        std::string filePath = R"(.\asset\image\)" + fileName + ".png";
        Graph* ret = Graph::LoadGraph(filePath.c_str());
        if (ret->GetHandler() == -1)
        {
            std::cerr ERR_LOG fileName << "cannot be loaded.";
        }
        return ret;
    }
}






namespace ingame
{






    //メインシーン
    namespace main
    {
        Scene::Scene()
        {
            new MapManager(1);
            new Test();
            new BackGroundManager();
            
            Loop();

            delete this;
        }
        void Scene::Loop()
        {

            while (LOOP)
            {
                LoopBasicUpdate();
            }
        }




    }


    enum class ETest
    {
        RED = 0,
        GREEN = 1,
        BLUE = 2,
    };





    // Test
    namespace main
    {
        Test* Test::Sole = nullptr;


        // テスト
        Test::Test() : Actor()
        {
            Test::Sole = this;
            mSpr->SetImage(Images->Chicken, 0, 0, 32, 32);
            mSpr->SetXY(20, 20);
            mSpr->SetZ(-20);

            OtherSp = new Sprite(Images->Test, 0, 0, 128, 64);
            OtherSp->SetXY(100, 50);
            OtherSp->SetZ(-200);

            SolState = luaManager::Lua["Test"];
            std::string res = luaManager::Lua["Test"]["new"](SolState, mSpr);
            std::cout << res;

            //std::string testLoad = luaManager::Lua["Field"]["layers"][1]["type"];
            //std::cout << testLoad << "\n";

            //sol::table testLoad = luaManager::Lua["Field"]["layers"];
            //std::cout << testLoad.size() << "\n";

            //sol::table testLoadVec = luaManager::Lua["Field"]["layers"][1]["data"];
            //std::cout << testLoadVec.size() << " " << (int)(testLoadVec[15]) << "\n";

            
        }

        void Test::update()
        {
            std::string res = luaManager::Lua["Test"]["update"](SolState);
            //std::cout << res;

            ETest color = ETest::RED;
            auto name = magic_enum::enum_name(color);
            //std::cout << name;

            std::string str{ "BLUE" };
            auto casted_str = magic_enum::enum_cast<ETest>(str);
            if (casted_str.has_value())
            {
                if (casted_str.value() == ETest::BLUE)
                {
                    //std::cout << "ok\n";
                }
            }
            //std::cout << "\n" << DxLib::GetFPS() << "\n";
            Actor::update();
        }
    }





    // BackGround
    namespace main
    {
        BackGroundTest* BackGroundTest::Sole = nullptr;


        // 背景テスト
        BackGroundTest::BackGroundTest() : Actor()
        {
            Image = DxLib::MakeScreen(ROUGH_WIDTH, ROUGH_HEIGHT, TRUE);
            mSpr->SetImage(new Graph(Image), 0, 0, ROUGH_WIDTH, ROUGH_HEIGHT);
            DxLib::SetDrawScreen(Image);
            
            for (int x = 0; x < ROUGH_WIDTH; x+=32)
            {
                for (int y = 0; y < ROUGH_HEIGHT; y+=32)
                {
                    DxLib::DrawGraph(x, y, Images->Tile32->GetHandler(), TRUE);
                }
            }

            mSpr->SetZ(4000);
            BackGroundTest::Sole = this;

        }
        void BackGroundTest::update()
        {
            Actor::update();
        }

    }

    namespace main
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
    }
    namespace main
    {
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

            int left = static_cast<int>(canConnect(matX-1, matY));
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
                MapManager::Sole->GetMatAt(x, y)->HasChip[static_cast<int>(tile)]!=0
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
    }

    namespace main
    {
        FloorLayer::FloorLayer() : FieldLayerBase(ZIndex::FLOOR)
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
                drawingAutoTile(matX, matY, dpX, dpY, Images->NaturalTile, 0, 0,
                    [&](int x, int y) {return canConnect(x, y, ETileName::sand_pit); });
                break;
            case ETileName::meadows:
                drawingAutoTile(matX, matY, dpX, dpY, Images->NaturalTile, 0, 0,
                    [&](int x, int y) {return canConnect(x, y, ETileName::meadows); });
                break;
            default:
                MapManager::Sole->GetTilesetGraph()->DrawGraph(dpX, dpY, chip->srcX, chip->srcY, 16, 16, ROUGH_SCALE);
                break;
            }
           
        }
    }






}


