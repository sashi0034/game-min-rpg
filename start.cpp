#include "stdafx.h"
#include "start.h"


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

    return ingame::DoProcess();
    
}









namespace ingame
{
    const char* GAME_TITLE_NAME = "Min RPG";
    useful::Random* Rand = nullptr;
    int GameState = 0;
    resorce::ImageRes* Images = nullptr;
    resorce::FontRes* Fonts = nullptr;
    resorce::SoundRes* Sounds = nullptr;

    int DoProcess()
    {
        setlocale(LC_CTYPE, "");

        DxLib::SetMainWindowText(GAME_TITLE_NAME);

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

        new Time();
        new Input();
        Rand = new useful::Random();
        Images = new resorce::ImageRes(); //画像読み込み
        Fonts = new resorce::FontRes();
        Sounds = new resorce::SoundRes();


    restart:
        
        if (luaManager::SolStart()==-1) return -1;

        std::cout OUT_LOG "Game is started.\n";

        //シーン
        SceneTransition();

        if (luaManager::CanRestartProgram)
        {// 再起動
            luaManager::CanRestartProgram = false;
            Sprite::DestroyAll();
            printfDx("Hot reloaded  at %s.\n", useful::GetDateTimeStr().c_str());
            goto restart;
        }

        std::cout OUT_LOG "Game is finished.\n";

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
            ExecuteScene((SceneBase*)(new title::TitleScene()));
            ExecuteScene((SceneBase*)(new main::MainScene()));
        }

        return 0;
    }


    void ExecuteScene(SceneBase* scene)
    {
        scene->Loop();
        delete scene;
    }


    SceneBase::SceneBase() : Singleton<SceneBase>()
    {}

    SceneBase::~SceneBase()
    {}

    void SceneBase::EnableExit()
    {
        canExit = true;
    }

    void SceneBase::Loop()
    {
        Time::Sole->Restart();
        while (LOOP)
        {
            if (canExit) break;
            Sprite::UpdateAll();
            DxLib::SetDrawScreen(DX_SCREEN_BACK);
            DxLib::ClearDrawScreen();
            Sprite::DrawingAll();
            DxLib::ScreenFlip();
            Time::Sole->update();
        }
    }



}



namespace ingame::resorce 
{
    Graph* ImageRes::loadPng(std::string fileName)
    {
        std::string filePath = R"(.\asset\image\)" + fileName + ".png";
        Graph* ret = Graph::LoadGraph(filePath.c_str());
        if (ret->GetHandler() == -1)
        {
            std::cerr ERR_LOG fileName << " cannot be loaded.";
        }
        return ret;
    }

    Sound* SoundRes::loadMp3(std::string fileName)
    {
        std::string filePath = R"(.\asset\sound\)" + fileName + ".mp3";
        Sound* ret = Sound::LoadSound(filePath.c_str());
        if (ret->GetHandler() == -1)
        {
            std::cerr ERR_LOG fileName << " cannot be loaded.";
        }
        return ret;
    }
}






namespace ingame
{
    namespace title
    {
        TitleScene::TitleScene()
        {
            new TitleManager();
            IntermissionCurtain::CreateOpen([]() {});
        }
        TitleScene::~TitleScene()
        {
            Sprite::DestroyAll();
        }
    }


    //メインシーン
    namespace main
    {
        MainScene::MainScene()
        {
            new SoundManager();
            new ScrollManager();
            new MapManager(1);
            new BackGroundManager();
            new GameController();
            new MapEventManager();
            new FlagManager();
            effect::StartEffect();
            StartUi();
            IntermissionCurtain::CreateOpen([](){});
        }
        MainScene::~MainScene()
        {
            delete MapManager::Sole;
            Sprite::DestroyAll();
        }
    }


    enum class ETest
    {
        RED = 0,
        GREEN = 1,
        BLUE = 2,
    };












}


