#pragma once
#include <iostream>
#include <string.h>
#include <map>
#include <vector>

#include "magic_enum.h"

#include "game_utils.h"
#include "game_engine.h"
#include "lua_manager.h"

namespace ingame
{

    class ButtonInTimer
    {
        std::map<int, int> mButtonTime{};
        bool notKeyOrAppend(int keyCode);
    public:
        ButtonInTimer();
        bool CheckJustAfterPress(int keyCode);
        bool ChackIntervalPress(int keyCode, int firstTime, int intervalTime);
        void Update();
    };

    //class ButtonEmbeddedTimer
    //{
    //    EventTimer eventTimer{};
    //    ButtonInTimer buttonInTimer{};
    //public:
    //    ButtonEmbeddedTimer();
    //    ButtonEmbeddedTimer(std::function<bool()> doEvent, int intervalMilliSec);
    //    ButtonInTimer* GetButton();
    //    void Update();
    //};


    class ILuaUser
    {
    protected:
        std::string mLuaClassName = "";
        sol::table mLuaData{};
        virtual void luaConstructor() = 0;
        virtual void luaUpdate() = 0;
    };

    class LuaActor : public Actor, public ILuaUser
    {
    protected:
        LuaActor(std::string luaClass, bool canLuaConstruct);
        void update() override;
        void luaConstructor() override;
        virtual void luaUpdate() override;
    };

    class LuaCollideActor : public CollideActor, public ILuaUser
    {
    protected:
        LuaCollideActor(std::string luaClass, bool canLuaConstruct, collider::Shape* col, UINT mask);
        void update() override;
        void luaConstructor() override;
        virtual void luaUpdate() override;
    };

    namespace main 
    {
        class FieldDecorationBase : public Actor
        {
        protected:
            int mAnimTime{};
        public:
            FieldDecorationBase(int x, int y);
        protected:
            void update() override;
        };
    }
};

