#pragma once
#include "stdafx.h"
#include <any>
#include <string>
#include <map>
#include <vector>
#include <iostream>
#include <tchar.h>



using namespace gameEngine;

namespace luaManager
{
    extern sol::state Lua;
    extern bool CanRestartProgram;

    int SolStart();
    void DefineTable();

    class LuaDebugManager : public Singleton<LuaDebugManager>
    {
        const std::string luaFolderPath = R"(E:\dev\VisualStudioRepos\min-rpg\asset\script)";
        std::vector<std::string> luaFilePaths{};

        std::map<std::string, SYSTEMTIME> mLastWriteTimeTable{};
        EventTimer mCheckEvent{};

        void findInDirectory(std::string oFolderPath, bool isShowLog);
        bool chaeckAllLastWriteTimes();
        void update();
    public:
        LuaDebugManager();

        Sprite* Spr;

        static void CallUpdate(Sprite* hSpr);
        static void Calldestructor(Sprite* hSpr);
    };

}
