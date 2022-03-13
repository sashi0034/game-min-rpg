#pragma once
#include "main_scene.h"


namespace ingame::main
{

    class Player : public LuaCollideActor, public ISingleton<Player>
    {
        const int moveUnit = 8;
        double mX, mY;
        int mMatX, mMatY;
        double mGotoX = 0, mGotoY = 0;
        EAngle mAngle = EAngle::DOWN;
        double mVel = 0;
        double mVelStandard = 0;
        int mAnimTime = 0;
        int mWaitTime = 0;
        int mFixedCount = 0;
        bool mIsMoveingNow = false;
        bool isFixed();
        EventTimer debugTimer;
        EventTimer mRegularTimer;
        ButtonInTimer mButton;
        PlayerEventProps mSenddingReachEvent{};
        PlayerEventProps mSenddingTouchEvent{};
    public:
        Player(double startX, double startY);
        ~Player();
        double GetX();
        double GetY();
        void IncreaseFixed();
        void DecreaseFixed();

        bool CanPopReachEvent(int x, int y);
        bool HasPopReachEvent();
        PlayerEventProps PopReachEvent();

        bool CanPopTouchEvent(int x, int y);
        bool HasPopTouchEvent();
        PlayerEventProps PopTouchEvent();
    protected:
        void update() override;
        void luaUpdate() override;
    private:
        void touchSomething();
        void animation();
        bool doWaitForMove();
        bool doMove();
    };
};
