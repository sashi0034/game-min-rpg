#include "stdafx.h"
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
    const char* GAME_TITLE_NAME = "Min RPG";
    useful::Random* Rand = nullptr;
    int GameState = 0;
    resorce::ImageRes* Images = nullptr;
    resorce::FontRes* Fonts = nullptr;
    resorce::SoundRes* Sounds = nullptr;

    int doProcess()
    {
        setlocale(LC_CTYPE, "");

        DxLib::SetMainWindowText(GAME_TITLE_NAME);

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
        new Input();
        Rand = new useful::Random();
        Images = new resorce::ImageRes(); //�摜�ǂݍ���
        Fonts = new resorce::FontRes();
        Sounds = new resorce::SoundRes();


    restart:
        
        if (luaManager::SolStart()==-1) return -1;

        std::cout OUT_LOG "Game is started.\n";

        //�V�[��
        SceneTransition();

        if (luaManager::CanRestartProgram)
        {// �ċN��
            luaManager::CanRestartProgram = false;
            Sprite::DestroyAll();
            printfDx("Hot reloaded  at %s.\n", useful::GetDateTimeStr().c_str());
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
            new main::MainScene();
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
        Time::Sole->update();
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

    SceneBase::SceneBase()
    {

    }

    void SceneBase::loop()
    {
        Time::Sole->Restart();
        while (LOOP)
        {
            LoopBasicUpdate();
        }
    }





    //���C���V�[��
    namespace main
    {
        MainScene::MainScene() : SceneBase()
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
            
            SceneBase::loop();

            delete MapManager::Sole;
            delete this;
        }
    }


    enum class ETest
    {
        RED = 0,
        GREEN = 1,
        BLUE = 2,
    };












}


