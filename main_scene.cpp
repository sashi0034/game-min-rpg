
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


    void Character::InstallCharacter(double x, double y, std::string character, std::string name)
    {
        ECharacterKind kind = magic_enum::enum_cast<ECharacterKind>(character).value_or(ECharacterKind::none);
        switch (kind)
        {
        case ECharacterKind::player:
            new Player(x, y);
            break;
        case ECharacterKind::punicat:
            new Punicat(x, y, kind, name);
            break;
        case ECharacterKind::slime:
            new Slime(x, y, kind, name);
            break;
        case ECharacterKind::sheep:
            new Sheep(x, y, kind, name);
            break;
        case ECharacterKind::chick:
            new Chick(x, y, kind, name);
            break;
        case ECharacterKind::chicken:
            new Chicken(x, y, kind, name);
            break;
        case ECharacterKind::skull:
            new Skull(x, y, kind, name);
            break;
        default:
            std::cerr ERR_LOG "Invalid character name `" << character << "` exit.\n";
            break;
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
    bool Character::DoMoveInStep(double* curX, double* curY, double toX, double toY, double vel)
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
        return ZIndex::CHARACTER - gridY / maxY * 1000;
    }

}


namespace ingame::main{
    const std::string FlagManager::LUA_CLASS = "FlagManager";


    FlagManager::FlagManager() : Actor()
    {
        Sole = this;
        luaManager::Lua[LUA_CLASS] = luaManager::Lua.create_table();
        luaManager::Lua[LUA_CLASS]["setFlag"] = [&](std::string flagName, bool flag)->void {SetFlag(flagName, flag); };
        luaManager::Lua[LUA_CLASS]["getFlag"] = [&](std::string flagName)->bool {return GetFlag(flagName); };
    }

    FlagManager::~FlagManager()
    {
        Sole = nullptr;
    }

    void FlagManager::update()
    {
        Actor::update();
    }

    void FlagManager::SetFlag(std::string flagName, bool flag)
    {
        mFlag[flagName] = flag;
    }

    bool FlagManager::GetFlag(std::string flagName)
    {
        if (mFlag.count(flagName) == 0)
        {
            std::cout OUT_LOG "Caution: flag `" << flagName << "` does not exit now.\n";
        }
        return mFlag[flagName];
    }

}


namespace ingame::main
{
    const std::string MapEventManager::LUA_CLASS = "MapEventManager";

    MapEventManager::MapEventManager() : LuaActor(LUA_CLASS, true)
    {
        Sole = this;

        luaManager::Lua[LUA_CLASS]["getUnique"] = [&](std::string key)-> sol::table{
            UniqueEventValue* uniqu = GetUniquEvent(key);
            if (uniqu== nullptr)
            {
                std::cerr ERR_LOG "Unique event `" << key << "` does not exit."; return luaManager::Lua.create_table();
            }
            sol::table ret = luaManager::Lua.create_table_with(
                "x", uniqu->X,
                "y", uniqu->Y
            );
            return ret;
        };

        luaManager::Lua[LUA_CLASS]["installCharacter"] = 
            [&](double x, double y, std::string character, std::string name) {InstallCharacter(x, y, character, name); };
    }

    void MapEventManager::update()
    {
        LuaActor::update();
    }
    MapEventManager::~MapEventManager()
    {
        Sole = nullptr;
    }

    UniqueEventValue* MapEventManager::GetUniquEvent(std::string key)
    {
        return MapManager::Sole->GetUniqueEvent(key);
    }

    void MapEventManager::InstallCharacter(double x, double y, std::string character, std::string name)
    {
        Character::InstallCharacter(x, y, character, name);
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
        NPCBase(startX, startY, characterKind, uniqueName, sprOriginX, sprOriginY)
    {
        mSpr->SetImage(Images->Punicat, 0, 0, 24, 24);
        mFrameInterval = mLuaData["frameInterval"].get_or(0);

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
}


namespace ingame::main
{
    Slime::Slime(double startX, double startY, ECharacterKind characterKind, std::string uniqueName) :
        NPCBase(startX, startY, characterKind, uniqueName, sprOriginX, sprOriginY)
    {
        mSpr->SetImage(Images->Slime, 0, 0, 24, 24);
        mFrameInterval = mLuaData["frameInterval"].get_or(0);

    }

    void Slime::animation()
    {
        int frame = (mAnimTime / mFrameInterval);
        
        mSpr->SetImage((frame % 4) * 24, 24 * 1);

        switch (mAngle)
        {
        case EAngle::DOWN:
            mSpr->SetFlip(true);
            break;
        case EAngle::RIGHT:
            mSpr->SetFlip(false);
            break;
        case EAngle::UP:
            mSpr->SetFlip(false);
            break;
        case EAngle::LEFT:
            mSpr->SetFlip(true);
            break;
        }

        mAnimTime += Time::DeltaMilli();
    }
}

namespace ingame::main
{
    Sheep::Sheep(double startX, double startY, ECharacterKind characterKind, std::string uniqueName) :
        NPCBase(startX, startY, characterKind, uniqueName, sprOriginX, sprOriginY)
    {
        mSpr->SetImage(Images->Sheep, 0, 0, 32, 32);
        mFrameInterval = mLuaData["frameInterval"].get_or(0);

    }

    void Sheep::animation()
    {
        int frame = (mAnimTime / mFrameInterval);
        
        mSpr->SetImage((frame % 3) * 32, int(mStopMovingTime>200 ? 0 : 1)*32);

        switch (mAngle)
        {
        case EAngle::DOWN:
            mSpr->SetFlip(false);
            break;
        case EAngle::RIGHT:
            mSpr->SetFlip(true);
            break;
        case EAngle::UP:
            mSpr->SetFlip(true);
            break;
        case EAngle::LEFT:
            mSpr->SetFlip(false);
            break;
        }

        mAnimTime += Time::DeltaMilli();
    }


    Chick::Chick(double startX, double startY, ECharacterKind characterKind, std::string uniqueName)
        : NPCBase(startX, startY, characterKind, uniqueName, sprOriginX, sprOriginY)
    {
    }

    void Chick::animation()
    {
    }

    Chicken::Chicken(double startX, double startY, ECharacterKind characterKind, std::string uniqueName)
        : NPCBase(startX, startY, characterKind, uniqueName, sprOriginX, sprOriginY)
    {
    }

    void Chicken::animation()
    {
    }

    Skull::Skull(double startX, double startY, ECharacterKind characterKind, std::string uniqueName)
        : NPCBase(startX, startY, characterKind, uniqueName, sprOriginX, sprOriginY)
    {
    }

    void Skull::animation()
    {
    }

}

namespace ingame::main
{



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





