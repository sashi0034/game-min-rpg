#include "main.h"


#define LOOP    (DxLib::ProcessMessage() != -1 && (!luaManager::CanRestartProgram))





int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
    {//�f�o�b�O�p�R���\�[���E�C���h�E�̕\��
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

        Img = new resorce::Image(); //�摜�ǂݍ���


    restart:
        
        if (luaManager::SolStart()==-1) return -1;

        printf("game is start\n");

        //�V�[��
        SceneTransition();

        if (luaManager::CanRestartProgram)
        {// �ċN��
            luaManager::CanRestartProgram = false;
            Sprite::DisposeAll();
            printfDx("�ċN������\n");
            goto restart;
        }

        printf("game is end\n");

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
    }
}











namespace game
{






    //���C���V�[��
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


        // �e�X�g
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


        // �w�i
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


















































    