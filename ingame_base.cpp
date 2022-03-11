#pragma once

#include "ingame_base.h"
#include "main_scene.h"


namespace ingame
{

    LuaActor::LuaActor(std::string luaClass, bool canLuaConstruct)
    {
        mLuaClassName = luaClass;
        mLuaData = luaManager::Lua.create_table();
        if (canLuaConstruct) luaConstructor();
    }
    void LuaActor::update()
    {
        luaUpdate();
        Actor::update();
    }
    void LuaActor::luaConstructor()
    {
        mLuaData = luaManager::Lua[mLuaClassName]["new"]();
    }
    void LuaActor::luaUpdate()
    {
        luaManager::Lua[mLuaClassName]["update"](mLuaData);
    }


    LuaCollideActor::LuaCollideActor(std::string luaClass, bool canLuaConstruct, collider::Shape* col, UINT mask) : CollideActor(col, mask)
    {
        mLuaClassName = luaClass;
        mLuaData = luaManager::Lua.create_table();
        if (canLuaConstruct) luaConstructor();
    }
    void LuaCollideActor::luaConstructor()
    {
        mLuaData = luaManager::Lua[mLuaClassName]["new"]();

    }
    void LuaCollideActor::luaUpdate()
    {
        luaManager::Lua[mLuaClassName]["update"](mLuaData);
    }
    void LuaCollideActor::update()
    {
        luaUpdate();
        CollideActor::update();
    }


    namespace main 
    {
        /// <summary>
        /// フィールド上の飾りの基底
        /// </summary>
        /// <param name="x">行列X</param>
        /// <param name="y">行列Y</param>
        FieldDecorationBase::FieldDecorationBase(int x, int y) : Actor()
        {
            mSpr->SetZ(double(ZIndex::FLOOR) - 1);
            mSpr->SetXY(x * 16, y * 16);
            mSpr->SetLinkXY(ScrollManager::Sole->GetSpr());
        }
        void FieldDecorationBase::update()
        {
            mAnimTime += Time::DeltaMilli();
            Actor::update();
        }
    }
}



