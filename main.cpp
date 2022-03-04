#include "main.h"


#define LOOP    (DxLib::ProcessMessage() != -1 && (!luaManager::CanRestartProgram))





int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
    {//デバッグ用コンソールウインドウの表示
        AllocConsole();
        FILE* stream;
        freopen_s(&stream, "CONIN$", "r", stdin);
        freopen_s(&stream, "CONOUT$", "w", stdout);
    }

    return game::Process();
    
}









namespace game
{
    int Process()
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

        Img = new resorce::Image(); //画像読み込み


    restart:
        
        if (luaManager::SolStart()==-1) return -1;

        printf("game is start\n");

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











namespace game
{






    //メインシーン
    namespace main
    {
        Scene::Scene()
        {
            new Test();
            new BackGround();
            
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
            mSpr->SetImage(Img->Chicken, 0, 0, 32, 32);
            mSpr->SetXY(20, 20);
            mSpr->SetZ(-20);

            OtherSp = new Sprite(Img->Test, 0, 0, 128, 64);
            OtherSp->SetXY(100, 50);
            OtherSp->SetZ(-200);

            SolState = luaManager::Lua["Test"];
            std::string res = luaManager::Lua["Test"]["new"](SolState, mSpr);
            std::cout << res;

            //std::string testLoad = luaManager::Lua["Field"]["layers"][1]["type"];
            //std::cout << testLoad << "\n";

            //sol::table testLoad = luaManager::Lua["Field"]["layers"];
            //std::cout << testLoad.size() << "\n";

            sol::table testLoadVec = luaManager::Lua["Field"]["layers"][1]["data"];
            std::cout << testLoadVec.size() << " " << (int)(testLoadVec[15]) << "\n";

            
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
        BackGround* BackGround::Sole = nullptr;


        // 背景
        BackGround::BackGround() : Actor()
        {
            Image = DxLib::MakeScreen(ROUGH_WIDTH, ROUGH_HEIGHT, TRUE);
            mSpr->SetImage(Graph(Image), 0, 0, ROUGH_WIDTH, ROUGH_HEIGHT);
            DxLib::SetDrawScreen(Image);
            
            for (int x = 0; x < ROUGH_WIDTH; x+=32)
            {
                for (int y = 0; y < ROUGH_HEIGHT; y+=32)
                {
                    DxLib::DrawGraph(x, y, Img->Tile32.getHandler(), TRUE);
                }
            }

            mSpr->SetZ(4000);
            BackGround::Sole = this;

        }
        void BackGround::update()
        {
            Actor::update();
        }

    }







}


















































    