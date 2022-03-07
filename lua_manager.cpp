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
        // 絶対パスを指定
        std::string debugFilePath = Lua["DEBUG_FILE_PATH"];
        HANDLE hFile = FindFirstFile(debugFilePath.c_str(), &findData);

        if (hFile == INVALID_HANDLE_VALUE)
        {
            std::cerr ERR_LOG "指定されたLuaファイルが見つかりませんでした\n";
        }
        else
        {
            FindClose(hFile);

            // FindFirstFileにてUTCタイムスタンプを取得(100ナノ秒単位)
            // FileTimeToLocalFileTimeにてUTCからローカル時間に変換
            // FileTimeToSystemTimeにて100ナノ秒単位を年月日時分秒ミリ秒に変換
            // 更新日時
            SYSTEMTIME lastWriteTime;
            FileTimeToLocalFileTime(&findData.ftLastWriteTime, &fileTime);
            FileTimeToSystemTime(&fileTime, &lastWriteTime);

            if (!mHasLastWriteTime)
            {// 初期化
                mLastWriteTime = lastWriteTime;
                mHasLastWriteTime = true;
            }

            if ((mLastWriteTime.wMilliseconds != lastWriteTime.wMilliseconds)
                || (mLastWriteTime.wSecond != lastWriteTime.wSecond))
            {//　更新してたなら
                std::cout OUT_LOG "Luaファイルが変更されたので再起動します";
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
