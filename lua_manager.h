#pragma once
#include <any>
#include <string>
#include <iostream>
#include "sol.hpp"
#include "DxLib.h"
#include "sprite.h"


namespace luaManager
{
    extern sol::state Lua;
    extern bool CanRestartProgram;

    int SolStart();
    void DefineTable();

    class LuaDebugManager
    {
        bool mHasLastWriteTime = false;
        SYSTEMTIME mLastWriteTime{};

    public:
        LuaDebugManager();

        Sprite* Spr;
        int Count = 0;

        void Update();

        static void CallUpdate(Sprite* hSpr);
        static void Calldestructor(Sprite* hSpr);
    };

}
