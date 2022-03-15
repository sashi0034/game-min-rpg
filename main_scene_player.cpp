#include "start.h"
#include "main_scene_player.h"


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
        mSpr->SetZ(ZIndex::CHARACTER);

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
                "プレイヤー座標: " << mX << ", " << mY << "\n" <<
                "プレイヤー経過時間: " << mTime / 1000.0 << "\n";
            return true; }
        , 1000 * 15);

        mRegularTimer = EventTimer([&]() {
            mButton.Update();
            return true;
            }, 1000 / 60);

        //new UiWindow(GRID_WIDTH/2, GRID_HEIGHT-120, 200,120,0.2, 0.2);
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

        mRegularTimer.Update();
        debugTimer.Update();
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
    PlayerEventProps Player::PopTouchEvent()
    {
        PlayerEventProps ret = mSenddingTouchEvent;
        mSenddingTouchEvent.HasValue = false;
        return ret;
    }

    void Player::animation()
    {
        int frame = (mAnimTime / 200);
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

    bool Player::doWaitForMove()
    {
        EAngle ang = EAngle::NONE;
        if (DxLib::CheckHitKey(KEY_INPUT_W)) ang = EAngle::UP;
        if (DxLib::CheckHitKey(KEY_INPUT_A)) ang = EAngle::LEFT;
        if (DxLib::CheckHitKey(KEY_INPUT_S)) ang = EAngle::DOWN;
        if (DxLib::CheckHitKey(KEY_INPUT_D)) ang = EAngle::RIGHT;

        if (ang != EAngle::NONE)
        {
            mAngle = ang;
            useful::Vec2 xy = Angle::ToXY(ang);

            bool canMove = false;

            switch (ang)
            {
            case EAngle::RIGHT:
                canMove =
                    Character::CanMappinglyMoveTo(mX + 16 * 3 / 4 + xy.X * moveUnit, mY + 16 * 1 / 4 + xy.Y * moveUnit, ang) &&
                    Character::CanMappinglyMoveTo(mX + 16 * 3 / 4 + xy.X * moveUnit, mY + 16 * 3 / 4 + xy.Y * moveUnit, ang);
                break;
            case EAngle::DOWN:
                canMove =
                    Character::CanMappinglyMoveTo(mX + 16 * 1 / 4 + xy.X * moveUnit, mY + 16 * 3 / 4 + xy.Y * moveUnit, ang) &&
                    Character::CanMappinglyMoveTo(mX + 16 * 3 / 4 + xy.X * moveUnit, mY + 16 * 3 / 4 + xy.Y * moveUnit, ang);
                break;
            case EAngle::LEFT:
                canMove =
                    Character::CanMappinglyMoveTo(mX + 16 * 1 / 4 + xy.X * moveUnit, mY + 16 * 1 / 4 + xy.Y * moveUnit, ang) &&
                    Character::CanMappinglyMoveTo(mX + 16 * 1 / 4 + xy.X * moveUnit, mY + 16 * 3 / 4 + xy.Y * moveUnit, ang);
                break;
            case EAngle::UP:
                canMove =
                    Character::CanMappinglyMoveTo(mX + 16 * 1 / 4 + xy.X * moveUnit, mY + 16 * 1 / 4 + xy.Y * moveUnit, ang) &&
                    Character::CanMappinglyMoveTo(mX + 16 * 3 / 4 + xy.X * moveUnit, mY + 16 * 1 / 4 + xy.Y * moveUnit, ang);
                break;
            }

            canMove = canMove && Character::CanCharacterPutIn(mX + xy.X*16, mY + xy.Y*16);

            if (canMove)
            {
                mVel = mVelStandard * 
                    (Input::Sole->GetKeyDown(KEY_INPUT_RSHIFT) || Input::Sole->GetKeyDown(KEY_INPUT_LSHIFT) ? 2.0 : 1.0);   // ダッシュをするか

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





