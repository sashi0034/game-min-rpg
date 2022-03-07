#include "lua_manager.h"
#include "start.h"

namespace luaManager
{
    sol::state Lua;
    bool CanRestartProgram = false;

    int SolStart()
    {

        Lua.open_libraries(sol::lib::base, sol::lib::package, sol::lib::coroutine, sol::lib::string, sol::lib::os, sol::lib::math,
            sol::lib::table, sol::lib::debug, sol::lib::bit32, sol::lib::io, sol::lib::ffi, sol::lib::utf8);
        DefineTable();

        new luaManager::LuaDebugManager();
        return 0;
    }
    void DefineTable()
    {

        luaManager::Lua.new_usertype<Sprite>(
            "Sprite",
            sol::constructors<Sprite()>(),
            "setXY", &Sprite::SetXY,
            "setImage", sol::resolve<void(Graph*)>(&Sprite::SetImage));

        luaManager::Lua.new_usertype<Actor>(
            "Actor",
            sol::constructors<Actor()>(),
            "getTime", &Actor::GetTime,
            "getSpr", &Actor::GetSpr);

        luaManager::Lua.new_usertype<CollideActor>(
            "CollideActor",
            "getTime", &CollideActor::GetTime,
            "getSpr", &CollideActor::GetSpr,
            "getHit", &CollideActor::GetHit,
            "getHitWith", &CollideActor::GetHitWith);

        Lua.set_function("Cout", [](std::string str) -> void {std::cout << str; });
        Lua.set_function("Cerr", [](std::string str) -> void {std::cerr << str; });
        
        Lua["Images"] = Lua.create_table();
        Lua["Images"]["Kisaragi"] = ingame::Images->Kisaragi;

        Lua["Time"] = Lua.create_table();
        Lua["Time"]["deltaSec"] = &Time::DeltaSec;
        Lua["Time"]["deltaMilli"] = &Time::DeltaMilli;

        Lua.script_file(R"(.\asset\scripte\start.lua)");
    }
}

namespace luaManager
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
        // ��΃p�X���w��
        std::string debugFilePath = Lua["DEBUG_FILE_PATH"];
        HANDLE hFile = FindFirstFile(debugFilePath.c_str(), &findData);

        if (hFile == INVALID_HANDLE_VALUE)
        {
            std::cerr ERR_LOG "�w�肳�ꂽLua�t�@�C����������܂���ł���\n";
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
                std::cout OUT_LOG "Lua�t�@�C�����ύX���ꂽ�̂ōċN�����܂�";
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
