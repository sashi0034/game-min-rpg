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
#include "main_scene.h"


namespace ingame::main
{

    class NinePatchImage : public Actor
    {
    private:
        Graph* mSrcGraph;
        Graph* mRenderGraph = nullptr;

        useful::Vec2<int> mSrcSize{};
        useful::Vec2<int> mSpriteSize{};
        useful::Vec2<double> mSpriteCenterPos{};
        useful::Vec2<double> mSpritePos{};

        useful::Vec2<int> mDivLineSrc[4];
        useful::Vec2<int> mRenderLine[4];
        useful::Vec2<double> mSideRatio{};

        void renderWindow();
    public:
        NinePatchImage(double drawCenterX, double drawCenterY, double gridUnitWidth, double gridUnitHeight, double sideRatioX, double sideRatioY, Graph* srcGraph);
        ~NinePatchImage();
        void SetSize(useful::Vec2<double> size);
    };


    class UiWindow : public NinePatchImage
    {
    private:
        double mCurWidth = 0;
        double mToWidth;
        double mHeight;
    public:
        UiWindow(double drawCenterX, double drawCenterY, int gridUnitWidth, int gridUnitHeight, double sideRatioX, double sideRatioY);
    protected:
        void update() override;
    };


    class MessageWindow : public LuaActor
    {
        const int fontSize = 18;
        EventTimer mWriteLetterTimer;
        EventTimer mScrollTimer;
        int mWidth, mHeight;
        int mNextLetterX{}, mNextLetterY{};
        Graph* mTextField;
        UiWindow* mTextWindow;
        std::wstring mTextBuffer{};
        int mTextReadIndex = 0;
        int mReadIntervalTimeBuffer = 0;

        int mScrollRemainAmount = 0;

        bool mIsRunning = false;

        bool hasUnreadText();
        bool writeLetter();
        bool scrollLine();
    protected:
        void update() override;
    public:
        MessageWindow();
        ~MessageWindow();
        bool GetIsRunning();
        void StreamText(std::string text);
        //void InputChoices(std::vector<std::string> choices);
        static const std::string CLASS_NAME;
        static void Init();
    };

}



