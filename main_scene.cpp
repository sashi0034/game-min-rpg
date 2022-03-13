
#include "main_scene.h"
#include "start.h"

namespace ingame::main
{




    useful::Vec2<int> Angle::ToXY(EAngle ang)
    {
        switch (ang)
        {
        case(EAngle::UP):
            return useful::Vec2<int>{0, -1};
        case(EAngle::RIGHT):
            return useful::Vec2<int>{1, 0};
        case(EAngle::DOWN):
            return useful::Vec2<int>{0, 1};
        case(EAngle::LEFT):
            return useful::Vec2<int>{-1, 0};
        default:
            std::cerr ERR_LOG "Invalid angle exit.\n";
            return useful::Vec2<int>{};
        }
    }

    EAngle Angle::ToAng(double x, double y)
    {
        // atan2の定義域は -pi ~ pi
        double theta = std::atan2(y, x);
        if (useful::Between(theta, -M_PI / 4, M_PI / 4))
        {
            return EAngle::RIGHT;
        }
        else if (useful::Between(theta, -M_PI * 3 / 4, -M_PI / 4))
        {
            return EAngle::UP;
        }
        else if (useful::Between(theta, M_PI / 4, M_PI * 3 / 4))
        {
            return EAngle::DOWN;
        }
        else if (useful::Between(theta, M_PI / 4, M_PI * 3 / 4))
        {
            return EAngle::DOWN;
        }
        else
        {
            return EAngle::LEFT;
        }
    }


    /// <summary>
    /// キャラクターの標準的な移動
    /// </summary>
    /// <param name="curX">現在Xポインタ</param>
    /// <param name="curY">現在Yポインタ</param>
    /// <param name="toX"></param>
    /// <param name="toY"></param>
    /// <param name="vel">速度</param>
    /// <returns>完了したらtrue</returns>
    bool Character::DoMove(double* curX, double* curY, double toX, double toY, double vel)
    {
        double vx = 0, vy = 0;
        double delta = 0.1;
        if (*curX < toX - delta) vx = 1;
        if (*curX > toX + delta) vx = -1;
        if (*curY < toY - delta) vy = 1;
        if (*curY > toY + delta) vy = -1;

        if (vx == 0 && vy == 0)
        {
            return false;
        }
        // 斜めでも同じ速度にする
        /*if (vx != 0 && vy != 0)
        {
            vx /= std::sqrt(2);
            vy /= std::sqrt(2);
        }*/

        *curX += vx * vel * Time::DeltaSec();
        *curY += vy * vel * Time::DeltaSec();

        if (vx < 0 && *curX < toX) *curX = toX;
        if (vx > 0 && *curX > toX) *curX = toX;
        if (vy < 0 && *curY < toY) *curY = toY;
        if (vy > 0 && *curY > toY) *curY = toY;
        return true;
    }
    void Character::AttachToGridXY(double* x, double* y, int unit)
    {
        *x = int(*x / unit) * unit;
        *y = int(*y / unit) * unit;
        *x = (std::min)((std::max)(0.0, *x), (MapManager::Sole->GetWidth()-1) * 16.0);
        *y = (std::min)((std::max)(0.0, *y), (MapManager::Sole->GetHeight()-1) * 16.0);
    }
    void Character::GetMatXY(int* x, int* y)
    {
        *x = *x / 16;
        *y = *y / 16;
    }
    /// <summary>
    /// キャラクターがすすめる場所か
    /// </summary>
    /// <param name="x">進みたい地点X</param>
    /// <param name="y"></param>
    /// <param name="toAng">進む方向</param>
    /// <returns></returns>
    bool Character::CanMoveTo(double x, double y, EAngle toAng)
    {
        int matX = x, matY = y;
        GetMatXY(&matX, &matY);


        if (!MapManager::Sole->IsInRange(matX, matY))
        {
            return true;
        }
        else
        {
            bool isStep = false;    // 崖のチェック

            useful::Vec2 moveXY = Angle::ToXY(toAng);

            return  !MapManager::Sole->GetMatAt(matX - moveXY.X, matY - moveXY.Y)->IsStep[static_cast<int>(toAng)] &&
                (MapManager::Sole->GetMatAt(matX, matY)->IsBridge || !MapManager::Sole->GetMatAt(matX, matY)->IsWall);
        }
    }

    void Character::DriveTalkEvent(int checkX, int checkY, sol::table luaData)
    {
        if (Player::Sole->CanPopTouchEvent(checkX, checkY))
        {
            auto e = Player::Sole->PopTouchEvent();
            int x = e.X;
            int y = e.Y;
            sol::table eve = luaManager::Lua.create_table_with("x", x, "y", y);
            luaData["trigger"](luaData, "talk", e);
        }
    }


}


namespace ingame::main
{

    MapEventManager::MapEventManager() : LuaActor("MapEventManager", true)
    {
        Sole = this;
    }

    void MapEventManager::update()
    {
        LuaActor::update();
    }
    MapEventManager::~MapEventManager()
    {
        Sole = nullptr;
    }

    void MapEventManager::DrivePlayerReachEvent(int x, int y)
    {
        if (!MapManager::Sole->IsInRange(x, y)) return;
        for (auto &eventName : MapManager::Sole->GetMatAt(x, y)->Events.ReachEvents)
        {
            sol::table e = luaManager::Lua.create_table_with("x", x, "y", y);
            trigger(eventName, e);
        }
    }

    void MapEventManager::DrivePlayerTouchEvent(int x, int y)
    {
        if (!MapManager::Sole->IsInRange(x, y)) return;
        for (auto& eventName : MapManager::Sole->GetMatAt(x, y)->Events.TouchEvents)
        {
            sol::table e = luaManager::Lua.create_table_with("x", x, "y", y);
            trigger(eventName, e);
        }
    }
    void MapEventManager::trigger(std::string eventName, sol::table e)
    {
        mLuaData["trigger"](mLuaData, eventName, e);
    }

    
}

namespace ingame::main
{
    /// <summary>
    /// プレイヤー
    /// 中心に16*16pxの板があるイメージでそれの左上が原点
    /// 描画時に座標をずらす
    /// </summary>
    /// <param name="startX"></param>
    /// <param name="startY"></param>
    Player::Player(double startX, double startY) : LuaCollideActor("Player", false, new collider::Rectangle(8, 16, 16, 16), 1)
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

        mVel = mLuaData["vel"];

        debugTimer = EventTimer([&]() {
            std::cout OUT_LOG 
                "プレイヤー座標: " << mX << ", "<< mY << "\n" << 
                "プレイヤー経過時間: " << mTime/1000.0 << "\n";
            return true; }
        , 1000 * 15);

        mRegularTimer = EventTimer([&]() {
            mButton.Update();
            return true;
            }, 1000/60);

        //new UiWindow(GRID_WIDTH/2, GRID_HEIGHT-120, 200,120,0.2, 0.2);
    }
    Player::~Player()
    {
        Sole = nullptr;
    }
    double Player::GetX()
    {
        return mX;
    }
    double Player::GetY()
    {
        return mY;
    }
    void Player::IncreaseFixed()
    {
        mFixedCount++;
    }
    void Player::DecreaseFixed()
    {
        mFixedCount--;
    }
    bool Player::CanPopTouchEvent(int x, int y)
    {
        if (mSenddingTouchEvent.HasValue)
        {
            if (std::abs(mSenddingTouchEvent.X - x) + std::abs(mSenddingTouchEvent.Y - y) <= 16 + 0.1)
            {
                return true;
            }
        }
        return false;
    }
    TouchEventProps Player::PopTouchEvent()
    {
        TouchEventProps ret = mSenddingTouchEvent;
        mSenddingTouchEvent.HasValue = false;
        return ret;
    }
    bool Player::isFixed()
    {
        return mFixedCount>0;
    }

  
    void Player::update()
    {
        mSenddingTouchEvent.HasValue = false;
        LuaCollideActor::update();

        mSpr->SetXY(mX - 8, mY - 16 - 4);
        animation();

        mRegularTimer.Update();
        debugTimer.Update();
    }
    void Player::luaUpdate()
    {
        luaManager::Lua[mLuaClassName]["update"](mLuaData);
    }

    bool Player::doMove()
    {
        if (Character::DoMove(&mX, &mY, mGotoX, mGotoY, mVel))
        {
            return true;
        }
        else
        {
            mX += moveUnit/2; mY += moveUnit/2;
            Character::AttachToGridXY(&mX, &mY, moveUnit);

            if ((int(mX)) % 16 == 0 && (int(mY)) % 16 == 0)
            {
                MapEventManager::Sole->DrivePlayerReachEvent(int(mX)/16, int(mY)/16);
            }

            return false;
        }
    }

    void Player::touchSomething()
    {
        if (mButton.CheckJustAfterPress(KEY_INPUT_SPACE))
        {
            if ((int(mX)) % 16 == 0 && (int(mY)) % 16 == 0)
            {
                auto way = Angle::ToXY(mAngle);
                MapEventManager::Sole->DrivePlayerTouchEvent(int(mX) / 16 + way.X, int(mY) / 16 + way.Y);
            }
            mSenddingTouchEvent = TouchEventProps{true, int(mX), int(mY) };
        }
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
                    Character::CanMoveTo(mX + 16 * 3 / 4 + xy.X * moveUnit, mY + 16 * 1 / 4 + xy.Y * moveUnit, ang) &&
                    Character::CanMoveTo(mX + 16 * 3 / 4 + xy.X * moveUnit, mY + 16 * 3 / 4 + xy.Y * moveUnit, ang);
                break;
            case EAngle::DOWN:
                canMove =
                    Character::CanMoveTo(mX + 16 * 1 / 4 + xy.X * moveUnit, mY + 16 * 3 / 4 + xy.Y * moveUnit, ang) &&
                    Character::CanMoveTo(mX + 16 * 3 / 4 + xy.X * moveUnit, mY + 16 * 3 / 4 + xy.Y * moveUnit, ang);
                break;
            case EAngle::LEFT:
                canMove =
                    Character::CanMoveTo(mX + 16 * 1 / 4 + xy.X * moveUnit, mY + 16 * 1 / 4 + xy.Y * moveUnit, ang) &&
                    Character::CanMoveTo(mX + 16 * 1 / 4 + xy.X * moveUnit, mY + 16 * 3 / 4 + xy.Y * moveUnit, ang);
                break;
            case EAngle::UP:
                canMove =
                    Character::CanMoveTo(mX + 16 * 1 / 4 + xy.X * moveUnit, mY + 16 * 1 / 4 + xy.Y * moveUnit, ang) &&
                    Character::CanMoveTo(mX + 16 * 3 / 4 + xy.X * moveUnit, mY + 16 * 1 / 4 + xy.Y * moveUnit, ang);
                break;
            }

            if (canMove)
            {
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


namespace ingame::main
{
    Punicat::Punicat(double startX, double startY, ECharacterKind characterKind, std::string uniqueName) : 
        LuaCollideActor(uniqueName, true, new collider::Rectangle(4, 8, 16, 16), 1),
        INonPlayerCharacter(characterKind, uniqueName)
    {
        mSpr->SetLinkXY(ScrollManager::Sole->GetSpr());
        mSpr->SetImage(Images->Punicat, 0, 0, 24, 24);
        mSpr->SetZ(ZIndex::CHARACTER);

        mX = startX;
        mY = startY;

        mLuaData["doMove"] = [&](double x, double y)->bool {return this->doMove(x, y); };
        mLuaData["getX"] = [&]()->double {return this->mX; };
        mLuaData["getY"] = [&]()->double {return this->mY; };

        mVel = mLuaData["vel"];
        mFrameInterval = mLuaData["frameInterval"].get_or(0);
    }

    void Punicat::update()
    {
        luaManager::Lua[mLuaClassName]["update"](mLuaData);

        mSpr->SetXY(mX - 4, mY - 8 - 4);

        animation();
    }
    void Punicat::animation()
    {
        int frame = (mAnimTime / mFrameInterval);
        mSpr->SetFlip(false);

        switch (mAngle)
        {
        case EAngle::DOWN:
            mSpr->SetImage((frame % 4) * 24, 0);
            break;
        case EAngle::RIGHT:
            mSpr->SetImage((frame % 4) * 24, 24 * 3);
            break;
        case EAngle::UP:
            mSpr->SetImage((frame % 4) * 24, 24 * 2);
            break;
        case EAngle::LEFT:
            mSpr->SetImage((frame % 4) * 24, 24 * 1);
            break;
        }

        mAnimTime += Time::DeltaMilli();
    }
    bool Punicat::doMove(double gotoX, double gotoY)
    {
       
        if (Character::DoMove(&mX, &mY, gotoX, gotoY, mVel))
        {
            if (std::abs(mX - gotoX) > moveUnit / 2 || std::abs(mY - gotoY) > moveUnit/2)
            {
                mAngle = Angle::ToAng(gotoX - mX, gotoY - mY);
            }
            
            return true;
        }
        else
        {
            mX += moveUnit / 2; mY += moveUnit / 2;
            Character::AttachToGridXY(&mX, &mY, moveUnit);
            return false;
        }
    }

}



namespace ingame::main
{
    TestNPC::TestNPC(int startX, int startY) : LuaCollideActor("TestNPC", false, new collider::Rectangle(8, 16, 16, 16), 1)
    {
        mX = startX;
        mY = startY;

        mSpr->SetImage(Images->Kisaragi, 0, 0, 32, 32);
        mSpr->SetZ(ZIndex::CHARACTER);
        mLuaData = luaManager::Lua[mLuaClassName]["new"](static_cast<CollideActor*>(this));
        mLuaData["doMove"] = [&](double x, double y)->bool {return this->doMove(x, y); };
        mLuaData["getX"] = [&]()->double {return this->mX; };
        mLuaData["getY"] = [&]()->double {return this->mY; };
        //mLuaData["doWaitForMove"] = [&]() {this->doWaitForMove(); };
    }
    void TestNPC::update()
    {
        LuaCollideActor::update();
    }
    void TestNPC::luaUpdate()
    {
        luaManager::Lua[mLuaClassName]["update"](mLuaData);
        mSpr->SetXY(mX-8, mY-8);
    }

    bool TestNPC::doMove(double x, double y)
    {
        double vx=0, vy=0;
        if (mX < x - 1) vx = 1;
        if (mX > x + 1) vx = -1;
        if (mY < y - 1) vy = 1;
        if (mY > y + 1) vy = -1;

        if (vx == 0 && vy == 0) 
        {
            return false; 
        }
     

        double vel = 20;
        mX += vx * vel * Time::DeltaSec();
        mY += vy * vel * Time::DeltaSec();
        
        return true;
    }

    bool TestNPC::doWaitForMove()
    {
        return false;
    }




    Weed::Weed(int x, int y) : FieldDecorationBase(x, y) { mAnimTime = (x + y) * 100; }
    void Weed::update()
    {
        mSpr->SetImage(Images->Weed, ((int)(mAnimTime / 300) % 2) * 16, 0, 16, 16);
        FieldDecorationBase::update();
    }

    Tree::Tree(int x, int y) : FieldDecorationBase(x, y) 
    {
        mSpr->SetXY(x * 16, y * 16-4);
        mAnimTime = (x + y) * 100; 
    }
    void Tree::update()
    {
        mSpr->SetImage(Images->Tree, ((int)(mAnimTime/500) % 4) * 16, 0, 16, 16);
        FieldDecorationBase::update();
    }

    Castle::Castle(int x, int y) : FieldDecorationBase(x, y) { mAnimTime = (x + y) * 100; }
    void Castle::update()
    {
        mSpr->SetImage(Images->Castle, ((int)(mAnimTime / 300) % 4) * 32, 0, 32, 32);
        FieldDecorationBase::update();
    }





}





