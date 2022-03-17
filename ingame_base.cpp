#include "stdafx.h"
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
        sol::safe_function_result res = luaManager::Lua[mLuaClassName]["new"]();
        if (res.valid())
        {
            mLuaData = res;
        }
        else
        {
            std::cerr ERR_LOG "In Lua constructer of " << mLuaClassName << " does not exit.";
        }

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
        sol::safe_function_result res = luaManager::Lua[mLuaClassName]["new"]();
        if (res.valid())
        {
            mLuaData = res;
        }
        else
        {
            std::cerr ERR_LOG "In Lua constructer of " << mLuaClassName << " does not exit.";
        }
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
        /// �t�B�[���h��̏���̊��
        /// </summary>
        /// <param name="x">�s��X</param>
        /// <param name="y">�s��Y</param>
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
    /// �{�^����std::map�ŊǗ����Ă���̂�
    /// ���������C�ɂ��Ȃ��ėǂ�
    /// </summary>
    bool ButtonInTimer::notKeyOrAppend(int keyCode)
    {
        if (mButtonTime.count(keyCode) == 0)
        {// �L�[�����݂��Ă��Ȃ��Ȃ珉����
            mButtonTime[keyCode] = Input::Sole->GetKeyDown(keyCode)==true ? 1 : 0;
            return true; // �����CheckJustAfterPress�����΂��Ȃ��悤��
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
    /// �����Ă���{�^���̌o�ߎ��Ԃ�+1����
    /// EventTimer�̓����Ŏg���邱�Ƃ�O��
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




namespace ingame::main
{
    /// <summary>
    /// NPC�̊��
    /// �f�t�H���g��Lua����"doMove", "getX", "getY"���o�^����A
    /// "vel"��Lua����ǂݍ��܂��
    /// </summary>
    /// <param name="startX"></param>
    /// <param name="startY"></param>
    /// <param name="characterKind"></param>
    /// <param name="uniqueName"></param>
    /// <param name="sprOriginX"></param>
    /// <param name="sprOriginY"></param>
    NPCBase::NPCBase(double startX, double startY, ECharacterKind characterKind, std::string uniqueName, int sprOriginX, int sprOriginY) :
        LuaCollideActor(uniqueName, true, new collider::Rectangle(-sprOriginX, -sprOriginY, 16, 16), 1),
        INonPlayerCharacter(characterKind, uniqueName)
    {
        mSpr->SetLinkXY(ScrollManager::Sole->GetSpr());
        mSpr->SetZ(ZIndex::CHARACTER);

        mX = startX;
        mY = startY;

        mLuaData["doMove"] = [&](double x, double y)->bool {return this->doMove(x, y); };
        mLuaData["getX"] = [&]()->double {return this->mX; };
        mLuaData["getY"] = [&]()->double {return this->mY; };
        mLuaData["setDeath"] = [&](bool isDeath)->void {this->mIsDeath = isDeath; };
        mLuaData["fadeAndDie"] = [&]()->void {NPCBase::fadeAndDie(); };

        mVel = mLuaData["vel"];

        this->sprOriginX = sprOriginX;
        this->sprOriginY = sprOriginY;
    }

    void NPCBase::update()
    {
        LuaCollideActor::update();

        driveTalkEvent();
        animation();

        if (mIsMovingNow)
        {
            mMovingTime+=Time::DeltaMilli();
            mStopMovingTime = 0;
        }
        else
        {
            mMovingTime = 0;
            mStopMovingTime += Time::DeltaMilli();
        }

        mSpr->SetXY(mX + sprOriginX, mY + sprOriginY);
        mSpr->SetZ(Character::GetZFromY(mY));

    }
    void NPCBase::driveTalkEvent()
    {
        if (Character::DriveTalkEvent(mX, mY, mLuaData))
        {
            mAngle = Character::TurnTowardPlayer(mX, mY);
        }
    }
    void NPCBase::fadeAndDie()
    {
        Sprite* copy = Sprite::CopyVisuallyFrom(this->mSpr);
        auto count = std::shared_ptr<int>(new int{0});

        new EventTimerAsActor([copy, count]()->bool {
            (*count)++;
            copy->SetBlendPal(*count % 2 == 0 ? 16 : 224);
            if (*count > 20)
            {
                Sprite::Dispose(copy);
                return false;
            }
            return true; 
            }, 100);

        mIsDeath = true;
    }


    bool NPCBase::doMove(double gotoX, double gotoY)
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

    bool NPCBase::doMoveTemporary(double gotoX, double gotoY)
    {
        auto onMoved = [&]() {
            mX += moveUnit / 2; mY += moveUnit / 2;
            Character::AttachToGridXY(&mX, &mY, moveUnit);
            mIsMovingNow = false;
        };


        if (!mIsMovingNow)
        {// �����n�߂̏���
            mAngle = Angle::ToAng(gotoX - mX, gotoY - mY);
            if (!Character::CanMappinglyMoveTo(gotoX + moveUnit / 2, gotoY + moveUnit / 2, mAngle) ||
                !Character::CanCharacterPutIn(gotoX, gotoY))
            {// �i�߂Ȃ�
                onMoved();
                return false;
            }
            mIsMovingNow = true;
        }

        if (Character::DoMoveInStep(&mX, &mY, gotoX, gotoY, mVel))
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


