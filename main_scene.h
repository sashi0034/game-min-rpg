#pragma once



#include "stdafx.h"


#include "lua_manager.h"

#include "ingame_manager.h"
#include "ingame_base.h"

#include "main_scene_ui.h"
#include "main_scene_effect.h"
#include "main_scene_controller.h"

namespace ingame::main
{
    enum class EColbit
    {
        NONE = 0,
        CHARACTER = 1 << 0,
        BIRD = 1 << 1,
    };

    enum class ZIndex
    {
        FLOOR = 1000,
        CHARACTER = 0,
        CLOUD = -1000,
        UI = -2000,
    };


    class Angle
    {
    public:
        static useful::Vec2<int> ToXY(EAngle ang);
        static EAngle ToAng(double x, double y);
    };


    struct PlayerEventProps
    {
        bool HasValue = false;
        int X;
        int Y;
    };


    class Character
    {
    public:
        static void InstallCharacter(double x, double y, std::string character, std::string name);
        static bool DoMoveInStep(double *curX, double *curY, double toX, double toY, double vel);
        static void AttachToGridXY(double *x, double *y, int unit);
        static void GetMatXY(int *x, int *y);
        static bool CanMappinglyMoveTo(double x, double y, EAngle toAng);    // å¸Ç´ÇÕäRëŒçÙ
        static bool CanCharacterPutIn(double x, double y);
        static bool DriveTalkEvent(double x, double y, sol::table luaData);
        static EAngle TurnTowardPlayer(double x, double y);
        //static void IncCharacterCountOnMap(double gridX, double gridY);
        //static void DecCharacterCountOnMap(double gridX, double gridY);
        //static void IncCharacterCountOnMapByMatXY(int matX, int matY);
        //static void DecCharacterCountOnMapByMatXY(int matX, int matY);
        static double GetZFromY(double gridY);
    };


    class MapEventManager : public LuaActor, public ISingleton<MapEventManager>
    {
        void trigger(std::string eventName, sol::table e);
        
    protected:
        void update() override;
    public:
        MapEventManager();
        ~MapEventManager();
        UniqueEventValue* GetUniquEvent(std::string key);
        void InstallCharacter(double x, double y, std::string character, std::string name);
        void DrivePlayerReachEvent(int x, int y);
        void DrivePlayerTouchEvent(int x, int y);
        static const std::string LUA_CLASS;
    };

    class FlagManager : public Actor, public ISingleton<FlagManager>
    {
        std::map<std::string, bool> mFlag{};
    protected:
        void update() override;

    public:
        FlagManager();
        ~FlagManager();
        void SetFlag(std::string flagName, bool flag);
        bool GetFlag(std::string flagName);
        std::string GetDescription(std::string flagName);
        static const std::string LUA_CLASS;
    };


    class Punicat : public NPCBase
    {
        static const int sprOriginX = -4;
        static const int sprOriginY = -4 - 8;
        int mFrameInterval;
    public:
        Punicat(double startX, double startY, ECharacterKind characterKind, std::string uniqueName);
    protected:
        void update() override;
        void animation() override;
    };

    class Slime : public NPCBase
    {
        static const int sprOriginX = -4;
        static const int sprOriginY = -4 - 8;
        int mFrameInterval;
    public:
        Slime(double startX, double startY, ECharacterKind characterKind, std::string uniqueName);
    protected:
        void animation() override;
        void update() override;
    };

    class Sheep : public NPCBase
    {
        static const int sprOriginX = -8;
        static const int sprOriginY = -16 - 4;
        int mFrameInterval;
    public:
        Sheep(double startX, double startY, ECharacterKind characterKind, std::string uniqueName);
    protected:
        void animation() override;
    };

    class Mouse : public NPCBase
    {
        static const int sprOriginX = -4;
        static const int sprOriginY = -4 - 8;
        int mFrameInterval;
    public:
        Mouse(double startX, double startY, ECharacterKind characterKind, std::string uniqueName);
    protected:
        void update() override;
        void animation() override;
    };


    class Chicken : public NPCBase
    {
        static const int sprOriginX = -8;
        static const int sprOriginY = -16 - 4;
        int mFrameInterval;
    public:
        Chicken(double startX, double startY, ECharacterKind characterKind, std::string uniqueName);
        ~Chicken();
    protected:
        void animation() override;
    };


    class Chick : public NPCBase
    {
        static const int sprOriginX = 0;
        static const int sprOriginY = -4;
        int mFrameInterval;
    public:
        Chick(double startX, double startY, ECharacterKind characterKind, std::string uniqueName);
    protected:
        void animation() override;
    private:
        int mChickId = 0;
        std::shared_ptr<useful::Vec2<int>> deltaDifference = std::shared_ptr<useful::Vec2<int>>(nullptr);
        useful::Vec2<double> mFollowVel = useful::Vec2<double>{ 0, 0 };
        bool doFollowPlayer();
        void followMove();
        void followResistWithChracter();
    public:
        static Chicken* ChickenInstance;
    };

    class Skull : public NPCBase
    {
        static const int sprOriginX = -4;
        static const int sprOriginY = -4 - 8;
        static const int skullNum = 3;
        int mFrameInterval;

        Sprite* mSkullCohortSpr[3];
    public:
        Skull(double startX, double startY, ECharacterKind characterKind, std::string uniqueName);
    protected:
        void animation() override;
    private:
        void skullCohortSetup();
        void skullCohortAnim();
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

    class Castle : public FieldDecorationBase
    {
    public:
        Castle(int x, int y);
    protected:
        void update() override;
    };


}




