#include "luas.h"

namespace luas
{
    sol::state Sol;
    bool CanRestartProgram = false;

    int SolStart()
    {

        Sol.open_libraries(sol::lib::base, sol::lib::package);
        DefineSpriteFunc();

        new luas::LuaDebugManager();
        return 0;
    }
    void DefineSpriteFunc()
    {

        luas::Sol.new_usertype<Sprite>(
            "Sprite",
            sol::constructors<Sprite()>(),
            "setXY", &Sprite::SetXY);

        Sol.script_file("test.lua");
    }
}

namespace luas
{
    LuaDebugManager::LuaDebugManager()
    {
        Spr = new Sprite();
        Spr->SetBelong(this);
        Spr->SetUpdateMethod(LuaDebugManager::CallUpdate);
        Spr->SetDestructorMethod(LuaDebugManager::Calldestructor);

    }

    void LuaDebugManager::Update()
    {
        WIN32_FIND_DATA findData;
        FILETIME fileTime;
        HANDLE hFile = FindFirstFile(R"(C:\Users\satos\source\repos\lua_test\resorce.lua)", &findData); // ��΃p�X���w��

        if (hFile == INVALID_HANDLE_VALUE)
        {
            printf("�ǂݍ��ݎ��s\n");
        }
        else
        {
            FindClose(hFile);

            // FindFirstFile�ɂ�UTC�^�C���X�^���v���擾(100�i�m�b�P��)
            // FileTimeToLocalFileTime�ɂ�UTC���烍�[�J�����Ԃɕϊ�
            // FileTimeToSystemTime�ɂ�100�i�m�b�P�ʂ�N���������b�~���b�ɕϊ�
            // �X�V����
            SYSTEMTIME lastWriteTime;
            FileTimeToLocalFileTime(&findData.ftLastWriteTime, &fileTime);
            FileTimeToSystemTime(&fileTime, &lastWriteTime);

            if (!mHasLastWriteTime)
            {// ������
                mLastWriteTime = lastWriteTime;
                mHasLastWriteTime = true;
            }

            if ((mLastWriteTime.wMilliseconds != lastWriteTime.wMilliseconds)
                || (mLastWriteTime.wSecond != lastWriteTime.wSecond))
            {//�@�X�V���Ă��Ȃ�
                printf("�v���O�����̕ύX���m�F���܂����B�ċN�����܂�\n");
                CanRestartProgram = true;
                mLastWriteTime = lastWriteTime;
            }


        }


    }


    void LuaDebugManager::CallUpdate(Sprite* hSpr)
    {
        std::any_cast<LuaDebugManager*>(hSpr->GetBelong())->Update();
    }
    void LuaDebugManager::Calldestructor(Sprite* hSpr)
    {
        delete std::any_cast<LuaDebugManager*>(hSpr->GetBelong());
    }

}
