#include "stdafx.h"
#include "start.h"
#include "main_scene_player.h"
#include "ingame_intermission_curtain.h"

namespace ingame::main
{
    /// <summary>
    /// プレイヤー
    /// 中心に16*16pxの板があるイメージでそれの左上が原点
    /// 描画時に座標をずらす
    /// </summary>
    /// <param name="startX"></param>
    /// <param name="startY"></param>
    Player::Player(double startX, double startY) : LuaCollideActor("PlayerLuaData", false, new collider::Rectangle(-sprOriginX, -sprOriginY, 16, 16), 1)
    {
        Sole = this;

        mSpr->SetLinkXY(ScrollManager::Sole->GetSpr());
        mSpr->SetImage(Images->Kisaragi, 0, 0, 32, 32);
        mSpr->SetZ(double(ZIndex::CHARACTER));

        mX = startX;
        mY = startY;


        mLuaData = luaManager::Lua[mLuaClassName]["new"]();
        mLuaData["doWaitForMove"] = [&]()->bool {return this->doWaitForMove(); };
        mLuaData["doMove"] = [&]()->bool {return this->doMove(); };
        mLuaData["isFixed"] = [&]()->bool {return this->isFixed(); };

        mVelStandard = mLuaData["vel"].get_or(1);
        mVel = mVelStandard;

        debugTimer = EventTimer([&]() {
            std::cout OUT_LOG
                "Player location: " << mX << ", " << mY << "\n" <<
                "Player time: " << mTime / 1000.0 << "\n";
            return true; }
        , 1000 * 15);

        mRegularTimer = EventTimer([&]() {
            mButton.Loop();
            return true;
            }, 1000 / 60);


        GameController::Sole->OnGameTimeStopped.push_back([&](int minu, int sec) {
            if (minu == 0 && sec == 0)
            {
                EnableKilled("活動時間を超えてしまった..");
            }
            });
    }
    Player::~Player()
    {
        Sole = nullptr;
    }
    void Player::update()
    {
        mSenddingTouchEvent.HasValue = false;
        LuaCollideActor::update();

        mSpr->SetXY(mX +sprOriginX, mY +sprOriginY);
        mSpr->SetZ(Character::GetZFromY(mY));
        animation();

        mRegularTimer.Loop();
        debugTimer.Loop();
    }
    double Player::GetX()
    {
        return mX;
    }
    double Player::GetY()
    {
        return mY;
    }
    /// <summary>
    /// ウインドウ表示などでプレイヤーを固定させるときに呼ぶ
    /// </summary>
    void Player::IncreaseFixed()
    {
        mFixedCount++;
    }
    void Player::DecreaseFixed()
    {
        mFixedCount--;
    }

    void Player::EnableKilled(std::string message)
    {
        if (mIsKilled) return;
        mIsKilled = true;

        sol::table e = luaManager::Lua.create_table();

        e["message"] = message;
        e["blackFadeOut"] = [&]() {blackFadeOut(); };
        e["exitScene"] = [&]() {
            IntermissionCurtain::CreateClose([&]() {MainScene::Sole->EnableExit(); }); 
        };

        mLuaData["eventDrive"](mLuaData, "killed", e);
    }

    void Player::EnableWinning()
    {
        if (mIsWinning) return;
        mIsWinning = true;

        sol::table e = luaManager::Lua.create_table();

        e["blackFadeOut"] = [&]() {blackFadeOut(); };
        e["exitScene"] = [&]() {
            IntermissionCurtain::CreateClose([&]() {MainScene::Sole->EnableExit(); }); 
        };

        mLuaData["eventDrive"](mLuaData, "winning", e);
    }

    void Player::blackFadeOut()
    {
        Sprite* blackSpr = new Sprite(Images->UiBlackScreen);
        auto alpha = std::shared_ptr<int>(new int(0));
        blackSpr->SetBlendPal(*alpha);
        blackSpr->SetZ(double(ZIndex::UI) + 1);
        
        new EventTimerAsActor([=]() {
            *alpha += 2;
            blackSpr->SetBlendPal(*alpha);

            if (*alpha > 255)
            {
                return false;
            }

            return true;
        }, (int)FPS60_MILLI);
    }

    bool Player::CanPopReachEvent(int x, int y)
    {
        if (mSenddingReachEvent.HasValue)
        {
            if (std::abs(mSenddingReachEvent.X - x) + std::abs(mSenddingReachEvent.Y - y) <= 0.1)
            {
                return true;
            }
        }
        return false;
    }
    bool Player::HasPopReachEvent()
    {
        return mSenddingReachEvent.HasValue;
    }
    PlayerEventProps Player::PopReachEvent()
    {
        PlayerEventProps ret = mSenddingReachEvent;
        mSenddingReachEvent.HasValue = false;
        return ret;
    }

    bool Player::CanPopTouchEvent(int x, int y)
    {
        if (mSenddingTouchEvent.HasValue)
        {
            if ((std::abs(mSenddingTouchEvent.X - x) + std::abs(mSenddingTouchEvent.Y - y)) <= 8 + 0.1)
            {
                return true;
            }
        }
        return false;
    }

    bool Player::isFixed()
    {
        return mFixedCount > 0;
    }



    void Player::luaUpdate()
    {
        luaManager::Lua[mLuaClassName]["update"](mLuaData);
    }

    bool Player::doMove()
    {

        if (Character::DoMoveInStep(&mX, &mY, mGotoX, mGotoY, mVel))
        {
            return true;
        }
        else
        {
            mX += moveUnit / 2; mY += moveUnit / 2;
            Character::AttachToGridXY(&mX, &mY, moveUnit);

            if (mFixedCount == 0)
            {
                if ((int(mX)) % 16 == 0 && (int(mY)) % 16 == 0)
                {
                    MapEventManager::Sole->DrivePlayerReachEvent(int(mX) / 16, int(mY) / 16);
                }
            }

            return false;
        }
    }


    void Player::dashAfterimage()
    {
        Sprite* afterimage = Sprite::CopyVisuallyFrom(mSpr);
        afterimage->SetZ(afterimage->GetZ()+0.001);
        auto time = std::shared_ptr<int>(new int{ 0 });
        
        new EventTimerAsActor(
            [afterimage, time]() {
                (*time)++;
                afterimage->SetBlendPal((std::max)(0, 120 - (* time) * 10));
                if (*time > 10)
                {
                    Sprite::Destroy(afterimage);
                    return false;
                }

                return true;
            }
            , 50);

    }

    int Player::getDashLevel()
    {
        int dashLevel = 1;
        if (Input::Sole->GetKeyDown(KEY_INPUT_RSHIFT) || Input::Sole->GetKeyDown(KEY_INPUT_LSHIFT))
        {
            dashLevel = 2;
            if (FlagManager::Sole->GetFlag("obtain_super_dash"))
            {
                dashLevel = 3;
            }
        }
        return dashLevel;
    }


    void Player::touchSomething()
    {
        if (mFixedCount == 0)
        {
            if (mButton.CheckJustAfterPress(KEY_INPUT_SPACE))
            {
                auto way = Angle::ToXY(mAngle);
                if ((int(mX)) % 16 == 0 && (int(mY)) % 16 == 0)
                {
                    MapEventManager::Sole->DrivePlayerTouchEvent(int(mX) / 16 + way.X, int(mY) / 16 + way.Y);
                }
                mSenddingTouchEvent = PlayerEventProps{ true, int(mX) + way.X * 16, int(mY) + way.Y * 16 };
            }
        }
    }

    bool Player::HasPopTouchEvent()
    {
        return mSenddingTouchEvent.HasValue;
    }
    bool Player::GetIsKilled()
    {
        return mIsKilled;
    }
    bool Player::GetIsWinning()
    {
        return mIsWinning;
    }
    PlayerEventProps Player::PopTouchEvent()
    {
        PlayerEventProps ret = mSenddingTouchEvent;
        mSenddingTouchEvent.HasValue = false;
        return ret;
    }

    void Player::animation()
    {
        if (mIsKilled)
        {// 死んだ
            mSpr->SetImage(0 * 32, 6 * 32);
        }
        else
        {// 生きている
            int t = getDashLevel();

            int frame = (mAnimTime / (200 / t));
            mSpr->SetFlip(false);
            if (mWaitTime > 50)
            {// 待機
                switch (mAngle)
                {
                case EAngle::DOWN:
                    mSpr->SetImage((frame % 4) * 32, 0);
                    break;
                case EAngle::RIGHT:
                    mSpr->SetImage((frame % 3) * 32, 32 * 1);
                    break;
                case EAngle::UP:
                    mSpr->SetImage((frame % 4) * 32, 32 * 2);
                    break;
                case EAngle::LEFT:
                    mSpr->SetImage((frame % 3) * 32, 32 * 1);
                    mSpr->SetFlip(true);
                    break;
                }
            }
            else
            {// 移動

                switch (mAngle)
                {
                case EAngle::DOWN:
                    mSpr->SetImage((frame % 4) * 32, 32 * 3);
                    break;
                case EAngle::RIGHT:
                    mSpr->SetImage((frame % 6) * 32, 32 * 4);
                    break;
                case EAngle::UP:
                    mSpr->SetImage((frame % 4) * 32, 32 * 5);
                    break;
                case EAngle::LEFT:
                    mSpr->SetImage((frame % 6) * 32, 32 * 4);
                    mSpr->SetFlip(true);
                    break;
                }
            }


            mAnimTime += Time::DeltaMilli();
        }
    }

    bool Player::doWaitForMove()
    {
        EAngle ang = EAngle::NONE;
        if (DxLib::CheckHitKey(KEY_INPUT_W)) ang = EAngle::UP;
        if (DxLib::CheckHitKey(KEY_INPUT_A)) ang = EAngle::LEFT;
        if (DxLib::CheckHitKey(KEY_INPUT_S)) ang = EAngle::DOWN;
        if (DxLib::CheckHitKey(KEY_INPUT_D)) ang = EAngle::RIGHT;

        if (ang != EAngle::NONE && mFixedCount==0)
        {
            mAngle = ang;
            useful::Vec2 xy = Angle::ToXY(ang);

            bool canMove = false;


            mColbit = 0;
            switch (ang)
            {
            case EAngle::RIGHT:
                canMove =
                    Character::CanMappinglyMoveTo(mX + 16 * 3 / 4 + xy.X * moveUnit, mY + 16 * 1 / 4 + xy.Y * moveUnit, ang) &&
                    Character::CanMappinglyMoveTo(mX + 16 * 3 / 4 + xy.X * moveUnit, mY + 16 * 3 / 4 + xy.Y * moveUnit, ang) && 
                    Character::CanCharacterPutIn(mX + xy.X * 8, mY + xy.Y * 0);
                break;
            case EAngle::DOWN:
                canMove =
                    Character::CanMappinglyMoveTo(mX + 16 * 1 / 4 + xy.X * moveUnit, mY + 16 * 3 / 4 + xy.Y * moveUnit, ang) &&
                    Character::CanMappinglyMoveTo(mX + 16 * 3 / 4 + xy.X * moveUnit, mY + 16 * 3 / 4 + xy.Y * moveUnit, ang) && 
                    Character::CanCharacterPutIn(mX + xy.X * 0, mY + xy.Y * 8);
                break;
            case EAngle::LEFT:
                canMove =
                    Character::CanMappinglyMoveTo(mX + 16 * 1 / 4 + xy.X * moveUnit, mY + 16 * 1 / 4 + xy.Y * moveUnit, ang) &&
                    Character::CanMappinglyMoveTo(mX + 16 * 1 / 4 + xy.X * moveUnit, mY + 16 * 3 / 4 + xy.Y * moveUnit, ang) && 
                    Character::CanCharacterPutIn(mX + xy.X * 8, mY + xy.Y * 0);
                break;
            case EAngle::UP:
                canMove =
                    Character::CanMappinglyMoveTo(mX + 16 * 1 / 4 + xy.X * moveUnit, mY + 16 * 1 / 4 + xy.Y * moveUnit, ang) &&
                    Character::CanMappinglyMoveTo(mX + 16 * 3 / 4 + xy.X * moveUnit, mY + 16 * 1 / 4 + xy.Y * moveUnit, ang) && 
                    Character::CanCharacterPutIn(mX + xy.X * 0, mY + xy.Y * 8);
                break;
            }
            mColbit = UINT(EColbit::CHARACTER);

            if (canMove)
            {
                int dash = getDashLevel();
                mVel = mVelStandard * dash;   // ダッシュをするか

                if (dash==3)
                {
                    dashAfterimage();
                }

                mGotoX = mX + xy.X * moveUnit;
                mGotoY = mY + xy.Y * moveUnit;

                if (mWaitTime > 1000) mAnimTime = 0;
                mWaitTime = 0;
                return true;

            }
            else
            {
                return false;
            }
        }
        else
        {
            mWaitTime += Time::DeltaMilli();
            touchSomething();
            return false;
        }
    }
}





