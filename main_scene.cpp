
#include "main_scene.h"
#include "main_scene_player.h"
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
    bool Character::CanMappinglyMoveTo(double x, double y, EAngle toAng)
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

            auto gotoMat = MapManager::Sole->GetMatAt(matX, matY);

            return  !MapManager::Sole->GetMatAt(matX - moveXY.X, matY - moveXY.Y)->IsStep[static_cast<int>(toAng)] &&
                (gotoMat->IsBridge || !gotoMat->IsWall);
        }
    }

    bool Character::CanCharacterPutIn(double x, double y)
    {
        double delta = 0.1;
        return (hit::GetHitRect(x + ScrollManager::Sole->GetX() + delta, y + ScrollManager::Sole->GetY() + delta,
            16 - delta * 2, 16 - delta * 2, 1) == nullptr);
    }

    bool Character::DriveTalkEvent(double checkX, double checkY, sol::table luaData)
    {
        if (Player::Sole->CanPopTouchEvent(int(checkX), int(checkY)))
        {
            auto e = Player::Sole->PopTouchEvent();
            int x = e.X;
            int y = e.Y;
            sol::table eve = luaManager::Lua.create_table_with("x", x, "y", y);
            luaData["eventDrive"](luaData, "talk", e);
            return true;
        }
        return false;
    }

    EAngle Character::TurnTowardPlayer(double x, double y)
    {
        return Angle::ToAng(Player::Sole->GetX()-x, Player::Sole->GetY()-y);
    }

    //void Character::IncCharacterCountOnMap(double gridX, double gridY)
    //{
    //    int matX = gridX, matY = gridY;
    //    GetMatXY(&matX, &matY);
    //    MapManager::Sole->GetMatAt(matX, matY)->CharacterCount++;
    //}

    //void Character::DecCharacterCountOnMap(double gridX, double gridY)
    //{
    //    int matX = gridX, matY = gridY;
    //    GetMatXY(&matX, &matY);
    //    MapManager::Sole->GetMatAt(matX, matY)->CharacterCount--;
    //}

    //void Character::IncCharacterCountOnMapByMatXY(int matX, int matY)
    //{
    //    MapManager::Sole->GetMatAt(matX, matY)->CharacterCount++;
    //}

    //void Character::DecCharacterCountOnMapByMatXY(int matX, int matY)
    //{
    //    MapManager::Sole->GetMatAt(matX, matY)->CharacterCount--;
    //}

    double Character::GetZFromY(double gridY)
    {
        static const int maxY = 16 * 256;
        return ZIndex::CHARACTER - gridY/maxY * 1000 ;
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
    Punicat::Punicat(double startX, double startY, ECharacterKind characterKind, std::string uniqueName) : 
        LuaCollideActor(uniqueName, true, new collider::Rectangle(-sprOriginX, -sprOriginY, 16, 16), 1),
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

        driveTalkEvent();
        animation();


        mSpr->SetXY(mX + sprOriginX, mY + sprOriginY);
        mSpr->SetZ(Character::GetZFromY(mY));

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
    void Punicat::driveTalkEvent()
    {
        if (Character::DriveTalkEvent(mX, mY, mLuaData))
        {
            mAngle = Character::TurnTowardPlayer(mX, mY);
        }
    }
    bool Punicat::doMove(double gotoX, double gotoY)
    {
        if (mHasTempGoto)
        {
            mHasTempGoto = doMoveTemporary(mTempGotoX, mTempGotoY);
        }

        if (!mHasTempGoto)
        {
            if ((std::abs)(mX - gotoX) >= moveUnit / 2)
            {
                mTempGotoX = mX + (gotoX - mX < 0 ? -1 : 1) * moveUnit;
                mTempGotoY = mY;
                mHasTempGoto = true;
            }
            else if ((std::abs)(mTempGotoY - gotoY) >= moveUnit / 2)
            {
                mTempGotoY = mY + (gotoY - mY < 0 ? -1 : 1) * moveUnit;
                mTempGotoX = mX;
                mHasTempGoto = true;
            }
        }
        return mHasTempGoto;
    }

    bool Punicat::doMoveTemporary(double gotoX, double gotoY)
    {
        auto onMoved = [&]() {
            mX += moveUnit / 2; mY += moveUnit / 2;
            Character::AttachToGridXY(&mX, &mY, moveUnit);
            IsMovingNow = false;
        };


        if (!IsMovingNow)
        {// 動き始めの処理
            mAngle = Angle::ToAng(gotoX - mX, gotoY - mY);
            if (!Character::CanMappinglyMoveTo(gotoX + moveUnit / 2, gotoY + moveUnit / 2, mAngle) ||
                !Character::CanCharacterPutIn(gotoX, gotoY))
            {// 進めない
                onMoved();
                return false;
            }
            IsMovingNow = true;
        }

        if (Character::DoMove(&mX, &mY, gotoX, gotoY, mVel))
        {
            return true;
        }
        else
        {
            onMoved();
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





