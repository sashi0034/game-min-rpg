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
        static useful::Vec2<int> ToXY(EAngle ang);
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

    class LuaActor : public Actor, public ILuaUser
    {
    protected:
        LuaActor(std::string luaClass, bool canLuaConstruct);
        void update() override;
        void luaConstructor() override;
        virtual void luaUpdate() override;
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
        static void AttachToGridXY(double *x, double *y, int unit);
        static void GetMatXY(int *x, int *y);
        static bool CanMoveTo(double x, double y, EAngle toAng);
    };


    class NinePatchImage : public Actor
    {
    private:
        Graph* mSrcGraph;
        Graph* mRenderGraph = nullptr;

        useful::Vec2<int> mSrcSize{};
        useful::Vec2<int> mSpriteSize{};
        useful::Vec2<double> mSpriteCenterPos{};
        useful::Vec2<double> mSpritePos{};

        useful::Vec2<int> mDivLineSrc[4];
        useful::Vec2<int> mRenderLine[4];
        useful::Vec2<double> mSideRatio{};

        void renderWindow();
    public:
        NinePatchImage(double drawCenterX, double drawCenterY, double roughWidth, double roughHeight, double sideRatioX, double sideRatioY, Graph* srcGraph);
        ~NinePatchImage();
        void SetSize(useful::Vec2<double> size);
    };


    class UiWindow : public NinePatchImage
    {
    private:
        double mCurWidth = 0;
        double mToWidth;
        double mHeight;
    public:
        UiWindow(double drawCenterX, double drawCenterY, int roughWidth, int roughHeight, double sideRatioX, double sideRatioY);
    protected:
        void update() override;
    };


    class MessageWindow : public LuaActor
    {
        const int fontSize = 18;
        EventTimer mWriteLetterTimer;
        EventTimer mScrollTimer;
        int mWidth, mHeight;
        int mNextLetterX{}, mNextLetterY{};
        Graph* mTextField;
        UiWindow* mTextWindow;
        std::wstring mTextBuffer{};
        int mTextReadIndex = 0;

        int mScrollRemainAmount = 0;

        bool mIsRunning = false;

        bool hasUnreadText();
        bool writeLetter();
        bool scrollLine();
    protected:
        void update() override;
    public:
        MessageWindow();
        ~MessageWindow();
        bool GetIsRunning();
        void StreamText(std::string text);
        //void InputChoices(std::vector<std::string> choices);
        static const std::string CLASS_NAME;
        static void Init();
    };

    class MapEventManager : public LuaActor, public ISingleton<MapEventManager>
    {
        void trigger(std::string eventName, sol::table e);
    protected:
        void update() override;
    public:
        MapEventManager();
        ~MapEventManager();
        void DriveReachEvent(int x, int y);
        void DriveTouchEvent(int x, int y);
    };


    class Player : public LuaCollideActor, public ISingleton<Player>
    {
        const int moveUnit = 8;
        double mX, mY;
        double mGotoX=0, mGotoY=0;
        EAngle mAngle = EAngle::DOWN;
        double mVel = 0;
        int mAnimTime = 0;
        int mWaitTime = 0;
        int mFixedCount = 0;
        bool isFixed();
        EventTimer debugTimer;
    public:
        Player(double startX, double startY);
        ~Player();
        double GetX();
        double GetY();
        void IncreaseFixed();
        void DecreaseFixed();
    protected:
        void update() override;
        void luaUpdate() override;
    private:
        void animation();
        bool doWaitForMove();
        bool doMove();
    };

    class Punicat : public LuaCollideActor, public INonPlayerCharacter
    {
        const int moveUnit = 16;
        double mX, mY;
        double mVel;
        double mAnimTime = 0;
        int mFrameInterval;
        EAngle mAngle = EAngle::DOWN;
    public:
        Punicat(double startX, double startY, ECharacterKind characterKind, std::string uniqueName);
    protected:
        void update() override;
        void animation();
        bool doMove(double x, double y);
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




