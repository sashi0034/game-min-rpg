#include "start.h"


#define LOOP    (DxLib::ProcessMessage() != -1 && (!luaManager::CanRestartProgram))





int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
    {//�f�o�b�O�p�R���\�[���E�C���h�E�̕\��
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
    useful::Random* Rand = nullptr;
    int GameState = 0;
    resorce::Image* Images = nullptr;


    int doProcess()
    {
        // �E�C���h�E���[�h�ŋN��
        DxLib::ChangeWindowMode(TRUE);

        // �t���X�N���[�����[�h���̃��j�^�[�̉𑜓x���ő�ɂ��郂�[�h�ɐݒ�
        DxLib::SetFullScreenResolutionMode(DX_FSRESOLUTIONMODE_NATIVE);

        // �t���X�N���[�����[�h���̉�ʂ̊g�僂�[�h���ŋߓ_���[�h�ɐݒ�
        DxLib::SetFullScreenScalingMode(DX_FSSCALINGMODE_NEAREST);

        DxLib::SetGraphMode(FULL_WIDTH, FULL_HEIGHT, 16);

        DxLib::SetAlwaysRunFlag(1); //��A�N�e�B�u��Ԃł�������

        if (DxLib::DxLib_Init() == -1)        // DX���C�u��������������
        {
            return -1;        // �G���[���N�����璼���ɏI��
            
        }
        Sprite::Init();

        new Time();
        Rand = new useful::Random();
        Images = new resorce::Image(); //�摜�ǂݍ���


    restart:
        
        if (luaManager::SolStart()==-1) return -1;

        std::cout OUT_LOG "Game is started.\n";

        //�V�[��
        SceneTransition();

        if (luaManager::CanRestartProgram)
        {// �ċN��
            luaManager::CanRestartProgram = false;
            Sprite::DisposeAll();
            printfDx("Hot Reload ����\n");
            goto restart;
        }

        std::cout OUT_LOG "Game is finished.\n";

        Sprite::End();
        DxLib::DxLib_End();        // �c�w���C�u�����g�p�̏I������


        return 0;
        // �\�t�g�̏I��
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
        Time::Sole->Update();
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






    //���C���V�[��
    namespace main
    {
        Scene::Scene()
        {
            
            new MapManager(1);
            new TestNPC(250, 200);
            //new Test();
            new BackGroundManager();
            new luaManager::LuaDebugManager();
            
            Loop();

            delete this;
        }
        void Scene::Loop()
        {
            Time::Sole->Restart();
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


        // �e�X�g
        Test::Test() : Actor()
        {
            Test::Sole = this;
            mSpr->SetImage(Images->Chicken, 0, 0, 32, 32);
            mSpr->SetXY(20, 20);
            mSpr->SetZ(-20);

            OtherSp = new Sprite(Images->Test, 0, 0, 128, 64);
            OtherSp->SetXY(100, 50);
            OtherSp->SetZ(-200);

            SolState = luaManager::Lua.create_table();
            SolState = luaManager::Lua["Test"]["new"](static_cast<Actor*>(this));
            //std::cout << res;

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

            //std::cout OUT_LOG Time::DeltaMilli() << " " << Time::DeltaSec() << "\n";

            Actor::update();
        }
    }











}


