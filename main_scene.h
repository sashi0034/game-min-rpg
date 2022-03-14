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
#include "ingame_base.h"

#include "main_scene_ui.h"
#include "main_scene_effect.h"

namespace ingame::main
{
    enum ZIndex
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
        void DrivePlayerReachEvent(int x, int y);
        void DrivePlayerTouchEvent(int x, int y);
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




