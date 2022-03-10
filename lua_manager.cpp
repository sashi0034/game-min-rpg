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

        //luaManager::Lua.new_usertype<Sprite>(
        //    "Sprite",
        //    sol::constructors<Sprite()>(),
        //    "setXY", &Sprite::SetXY,
        //    "setImage", sol::resolve<void(Graph*)>(&Sprite::SetImage));

        //luaManager::Lua.new_usertype<Actor>(
        //    "Actor",
        //    sol::constructors<Actor()>(),
        //    "getTime", &Actor::GetTime,
        //    "getSpr", &Actor::GetSpr);

        //luaManager::Lua.new_usertype<CollideActor>(
        //    "CollideActor",
        //    "getTime", &CollideActor::GetTime,
        //    "getSpr", &CollideActor::GetSpr,
        //    "getHit", &CollideActor::GetHit,
        //    "getHitWith", &CollideActor::GetHitWith);
        //Lua["Images"] = Lua.create_table();
        //Lua["Images"]["Kisaragi"] = ingame::Images->Kisaragi;

        Lua.set_function("Cout", [](std::string str) -> void {std::cout << str; });
        Lua.set_function("Cerr", [](std::string str) -> void {std::cerr << str; });

        Lua["Time"] = Lua.create_table();
        Lua["Time"]["deltaSec"] = &Time::DeltaSec;
        Lua["Time"]["deltaMilli"] = &Time::DeltaMilli;

        Lua.script_file(R"(.\asset\scripte\start.lua)");
    }
}

namespace luaManager
{
    LuaDebugManager::LuaDebugManager() : Singleton<LuaDebugManager>()
    {
        Spr = new Sprite();
        Spr->SetBelong(this);
        Spr->SetUpdateMethod(LuaDebugManager::CallUpdate);
        Spr->SetDestructorMethod(LuaDebugManager::Calldestructor);

        std::cout OUT_LOG "Serch files in "<< luaFolderPath <<":\n";
        findInDirectory(luaFolderPath, true);
        chaeckAllLastWriteTimes();

        mCheckEvent = EventTimer([&]()->bool {
            luaFilePaths = std::vector<std::string>{};
            findInDirectory(luaFolderPath, false);
            if (chaeckAllLastWriteTimes()) CanRestartProgram = true;
            return true;
            }, 500);
    }

    void LuaDebugManager::update()
    {
        mCheckEvent.Update();
    }

    /*
    // 指定したフォルダ内のファイルをサブフォルダを含めて巡回する
    // 参考: https://www.wabiapp.com/WabiSampleSource/windows/find_directory.html
    */
    void LuaDebugManager::findInDirectory
    (
        std::string oFolderPath,    // フォルダパス
        bool isShowLog
    )
    {
        WIN32_FIND_DATA tFindFileData;

        // 全てのファイル
        oFolderPath += "\\*.*";

        // 最初に一致するファイルを取得
        HANDLE hFile = ::FindFirstFile(oFolderPath.c_str(), &tFindFileData);
        if (INVALID_HANDLE_VALUE == hFile) {
            return;
        }

        // L"¥¥*.*"を削除
        oFolderPath = oFolderPath.substr(0, oFolderPath.size() - 4);

        do {

            TCHAR* wpFileName = tFindFileData.cFileName;

            // フォルダかどうかを判定
            if (tFindFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) {

                /*
                    L"."とL".."はスキップ
                */
                if (L'.' == wpFileName[0]) {
                    if ((L'\0' == wpFileName[1])
                        || (L'.' == wpFileName[1] && L'\0' == wpFileName[2])
                        ) {
                        continue;
                    }
                }

                // フルパスの生成
                std::string oFullPath = oFolderPath + "\\" + wpFileName;
                if (isShowLog) std::cout << "(dir )" << oFullPath << std::endl;

                // 再起してサブフォルダを巡回する
                findInDirectory(oFullPath, isShowLog);
            }
            else {

                // フルパスの生成
                std::string oFullPath = oFolderPath + "\\" + wpFileName;
                if (isShowLog) std::cout << "(file)" << oFullPath << std::endl;
                luaFilePaths.push_back(oFullPath);
            }

            // 次に一致するファイルの検索
        } while (::FindNextFile(hFile, &tFindFileData));

        // 検索ハンドルを閉じる
        ::FindClose(hFile);
    }

    /// <summary>
    /// それぞれファイルに書き込みがあるか調べてあるなら更新処理を走らせる
    /// </summary>
    /// <returns>trueなら変更あり</returns>
    bool LuaDebugManager::chaeckAllLastWriteTimes()
    {
        bool isChanged = false;
        for (auto debugFilePath : luaFilePaths)
        {

            WIN32_FIND_DATA findData;
            FILETIME fileTime;

            // 絶対パスを指定
            HANDLE hFile = FindFirstFile(debugFilePath.c_str(), &findData);

            if (hFile == INVALID_HANDLE_VALUE)
            {
                std::cerr ERR_LOG debugFilePath << "が見つかりませんでした\n";
                isChanged = true;
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

                if (mLastWriteTimeTable.count(debugFilePath)==0)
                {// 初期化
                    mLastWriteTimeTable[debugFilePath] = lastWriteTime;
                    isChanged = true;
                }

                if ((mLastWriteTimeTable[debugFilePath].wMilliseconds != lastWriteTime.wMilliseconds)
                    || (mLastWriteTimeTable[debugFilePath].wSecond != lastWriteTime.wSecond))
                {//　更新してたなら
                    mLastWriteTimeTable[debugFilePath] = lastWriteTime;
                    std::cout OUT_LOG "Luaファイルが変更されました\n";
                    isChanged = true;
                }
            }
        }
        return isChanged;
    }


    void LuaDebugManager::CallUpdate(Sprite* hSpr)
    {
        std::any_cast<LuaDebugManager*>(hSpr->GetBelong())->update();
    }
    void LuaDebugManager::Calldestructor(Sprite* hSpr)
    {
        delete std::any_cast<LuaDebugManager*>(hSpr->GetBelong());
    }

}
