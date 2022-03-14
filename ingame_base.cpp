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



    /// <summary>
    /// ボタンはstd::mapで管理しているので
    /// 初期化を気にしなくて良い
    /// </summary>
    bool ButtonInTimer::notKeyOrAppend(int keyCode)
    {
        if (mButtonTime.count(keyCode) == 0)
        {// キーが存在していないなら初期化
            mButtonTime[keyCode] = Input::Sole->GetKeyDown(keyCode)==true ? 1 : 0;
            return true; // 直後はCheckJustAfterPressが発火しないように
        }
        return false;
    }

    ButtonInTimer::ButtonInTimer()
    {
    }

    bool ButtonInTimer::CheckJustAfterPress(int keyCode)
    {
        if (notKeyOrAppend(keyCode)) return false;
        if (mButtonTime[keyCode] == 1) return true;
        return false;
    }

    bool ButtonInTimer::ChackIntervalPress(int keyCode, int firstTime, int intervalTime)
    {
        notKeyOrAppend(keyCode);

        if (mButtonTime[keyCode] == 1) 
        { 
            return true; 
        }
        else if (mButtonTime[keyCode] >= firstTime)
        {
            int time = mButtonTime[keyCode] - firstTime;
            if (time % intervalTime == 0)
            {
                return true;
            }
        }
        return false;
    }

    /// <summary>
    /// 押しているボタンの経過時間を+1する
    /// EventTimerの内部で使われることを前提
    /// </summary>
    void ButtonInTimer::Update()
    {
        mButtonTime;
        for (auto itr = mButtonTime.begin(); itr != mButtonTime.end(); ++itr)
        {
            if (Input::Sole->GetKeyDown(itr->first) != false)
            {
                itr->second += 1;
            }
            else
            {
                itr->second = 0;
            }
        }
    }

 /*   ButtonEmbeddedTimer::ButtonEmbeddedTimer()
    {
    }

    ButtonEmbeddedTimer::ButtonEmbeddedTimer(std::function<bool()> doEvent, int intervalMilliSec)
    {
        eventTimer = EventTimer([&]()->bool{
            buttonInTimer.Update();
            return doEvent();
            }, intervalMilliSec);
    }


    ButtonInTimer* ButtonEmbeddedTimer::GetButton()
    {
        return &buttonInTimer;
    }

    void ButtonEmbeddedTimer::Update()
    {
        eventTimer.Update();
    }*/

    EventTimerAsActor::EventTimerAsActor(std::function<bool()> doEvent, int intervalTime) : Actor()
    {
        mDoEvent = doEvent;
        mEventTImer = EventTimer(mDoEvent, intervalTime);
    }
    void EventTimerAsActor::update()
    {
        Actor::update();
        mEventTImer.Update();
        if (mEventTImer.IsAlive() == false)
        {
            Sprite::Dispose(mSpr);
            return;
        }
    }
    EventTimer* EventTimerAsActor::GetEventTimer()
    {
        return &mEventTImer;
    }

}



