#pragma once


#include <iostream>
#include <string.h>
#include <map>
#include <vector>

#include "magic_enum.h"

#include "game_utils.h"
#include "game_engine.h"
#include "lua_manager.h"

#include "ingame_manager.h"


namespace ingame::main
{
    enum class EAngle
    {
        NONE = -1,
        RIGHT = 0,
        DOWN = 1,
        LEFT = 2,
        UP = 3
    };
    class Angle
    {
    public:
        static useful::XY<int> ToXY(EAngle ang);
        static EAngle ToAng(double x, double y);
    };



    class ILuaUser
    {
    protected:
        std::string mLuaClassName = "";
        sol::table mLuaData{};
        virtual void luaConstructor() = 0;
        virtual void luaUpdate() = 0;
    };

    class LuaCollideActor: public CollideActor, public ILuaUser
    {
    protected:
        LuaCollideActor(std::string luaClass, bool canLuaConstruct, collider::Shape* col, UINT mask);
        void update() override;
        void luaConstructor() override;
        virtual void luaUpdate() override;
    };

    class Character
    {
    public:
        static bool DoMove(double *curX, double *curY, double toX, double toY, double vel);
        static void AttachToGridXY(double *x, double *y);
    };



    class Player : public LuaCollideActor
    {
        double mX, mY;
        double mGotoX=0, mGotoY=0;
        EAngle mAngle = EAngle::DOWN;
        double mVel = 0;
        int mAnimTime = 0;
        int mWaitTime = 0;
    public:
        Player(int startX, int startY);
    protected:
        void update() override;
        void luaUpdate() override;
    private:
        void animation();
        bool doWaitForMove();
        bool doMove();
    };

    class TestNPC : public LuaCollideActor
    {
        double mX=0;
        double mY=0;
    public:
        TestNPC(int startX, int startY);
    protected:
        void update() override;
        void luaUpdate() override;
    private:
        bool doWaitForMove();
        bool doMove(double x, double y);
    };




    class FieldDecorationBase : public Actor
    {
    protected:
        int mAnimTime{};
    public:
        FieldDecorationBase(int x, int y);
    protected :
        void update() override;
    };

    class Weed : public FieldDecorationBase
    {
    public:
        Weed(int x, int y);
    protected:
        void update() override;
    };

    class Tree : public FieldDecorationBase
    {
    public:
        Tree(int x, int y);
    protected:
        void update() override;
    };


}




