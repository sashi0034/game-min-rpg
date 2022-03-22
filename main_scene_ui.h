#pragma once

#include "stdafx.h"


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


    class UiBlackWindow : public NinePatchImage
    {
    private:
        double mCurWidth = 0;
        double mToWidth;
        double mSprHeight;
    public:
        UiBlackWindow(double drawCenterX, double drawCenterY, int gridUnitWidth, int gridUnitHeight, double sideRatioX, double sideRatioY);
    protected:
        void update() override;
    };

    class TextField : public Actor
    {
        int mSprWidth, mSprHeight;
        int mRenderWidth, mRenderHeight;
        UINT mForegroundColor;
        UINT mEdgegroundColor;
        int mPaddingTop = 0;
        int mPaddingLeft = 0;
        Graph* mFontGraph = nullptr;
        std::unique_ptr<Graph> mGraph;
        std::string mText;
    public:
        TextField(Graph* fontData, int width, int height);
        void SetColor(UINT foreground, UINT background);
        void SetText(std::string str);
        void SetStartXY(double x, double y);
        void SetCenterXY(double x, double y);
        void SetPaddingStart(int left, int top);
        void RenderDirect(std::function<void()> callback);
    };

    class MessageWindow : public LuaActor
    {
        const int fontSize = 18;
        EventTimer mWriteLetterTimer;
        EventTimer mScrollTimer;
        int mSprWidth, mSprHeight;
        int mNextLetterX{}, mNextLetterY{};
        Sprite* mTextSpr;
        Graph* mTextFieldGraph;
        UiBlackWindow* mTextWindow;
        std::wstring mTextBuffer{};
        int mTextReadIndex = 0;
        int mReadIntervalTimeBuffer = 0;
        bool mIsPusedSkipButtonOnStart = false;

        int mScrollRemainAmount = 0;

        int mRunningCount = 0;

        void constructTextArea();
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
        void AnimShake(double intensity);
        void AnimFlash();
        static const std::string CLASS_NAME;
        static void Init();
    };

    class SelectionWindow : public LuaActor
    {
        const int fontSize = 18;
        EventTimer mInputTimer;
        ButtonInTimer mButton{};

        int mGridUnitWidth, mGridUnitHeight;
        int mWindowHeight;
        Graph* mTextFieldGraph;
        UiBlackWindow* mWindow;
        std::vector<std::string> mOptions{};
        int mOptionNum = 0;

        Sprite* mCursorSpr = nullptr;
 
        bool mIsRunning = false;
        int mSelectedIndex = 0;

        void renderText();
        bool inputOptions();
        void resetCursor();
    protected:
        void update() override;
    public:
        SelectionWindow(sol::table table);
        ~SelectionWindow();
        bool GetIsRunning();
        int GetSelectedIndex();
        std::string GetSelectedWord();
        static const std::string CLASS_NAME;
        static void Init();
    };



    class LimitTimeWindow : public LuaActor
    {
        NinePatchImage* mWindow;
        TextField* mTextField;
    public:
        LimitTimeWindow();
    protected:
        void update() override;
        void renderText(int minutue, int sec);
    };

    class FlagInfoWindow : public LuaActor
    {
        NinePatchImage* mWindow;
        TextField* mTextField;
        int mIndex = 0;

        void appeaAndFadeAnim();
    public:
        FlagInfoWindow(std::string str);
        ~FlagInfoWindow();
        static int ActiveCount;
    protected:
        void update() override;
    };

    void StartUi();

}



