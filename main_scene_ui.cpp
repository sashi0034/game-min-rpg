#include "stdafx.h"
#include "main_scene_ui.h"
#include "start.h"
#include "main_scene_player.h"
#include "main_scene_controller.h"


namespace ingame::main
{

    /// <summary>
    /// 9 pach imageのスプライトを生成する基底クラス
    /// </summary>
    /// <param name="drawCenterX">配置する中央原点X</param>
    /// <param name="drawCenterY">配置する中央原点Y</param>
    /// <param name="gridUnitWidth">幅</param>
    /// <param name="gridUnitHeight">高さ</param>
    /// <param name="sideRatioX">端の比率</param>
    /// <param name="sideRatioY"><端の比率/param>
    /// <param name="srcGraph"></param>
    NinePatchImage::NinePatchImage(double drawCenterX, double drawCenterY, double gridUnitWidth, double gridUnitHeight, double sideRatioX, double sideRatioY, Graph* srcGraph)
    {
        mSrcGraph = srcGraph;
        DxLib::GetGraphSize(mSrcGraph->GetHandler(), &mSrcSize.X, &mSrcSize.Y);

        mSideRatio = useful::Vec2<double>{ sideRatioX, sideRatioY };
        mDivLineSrc[0] = useful::Vec2<int>{ 0,0 };
        mDivLineSrc[1] = useful::Vec2<int>{ int(sideRatioX * mSrcSize.X),int(sideRatioY * mSrcSize.Y) };
        mDivLineSrc[2] = useful::Vec2<int>{ int((1 - sideRatioX) * mSrcSize.X),int((1 - sideRatioY) * mSrcSize.Y) };
        mDivLineSrc[3] = useful::Vec2<int>{ mSrcSize.X, mSrcSize.Y };

        mSpriteCenterPos = useful::Vec2<double>{ drawCenterX, drawCenterY };

        SetSize(useful::Vec2<double>{gridUnitWidth, gridUnitHeight});


        mSpr->SetZ(double(ZIndex::UI));
        mSpr->SetDrawingMethod(Sprite::DrawingKind::DotByDot);
    }

    NinePatchImage::~NinePatchImage()
    {
        delete mRenderGraph;
    }

    void NinePatchImage::SetSize(useful::Vec2<double> size)
    {

        mSpriteSize = (size * PX_PER_GRID).EachTo<int>();
        mSpritePos = mSpriteCenterPos - (size / 2.0).EachTo<double>();
        int cornerLength = (std::min)(mSpriteSize.X * mSideRatio.X, mSpriteSize.Y * mSideRatio.Y);
        mRenderLine[0] = useful::Vec2<int>{ 0,0 };
        mRenderLine[1] = useful::Vec2<int>{ cornerLength, cornerLength };
        mRenderLine[2] = useful::Vec2<int>{ mSpriteSize.X - cornerLength, mSpriteSize.Y - cornerLength };
        mRenderLine[3] = useful::Vec2<int>{ mSpriteSize.X, mSpriteSize.Y };

        DxLib::SetCreateDrawValidGraphMultiSample(8, 4);
        {// アンチエイリアスをかけて描画できるように
            if (mRenderGraph != nullptr) delete mRenderGraph;
            mRenderGraph = new Graph(DxLib::MakeScreen(mSpriteSize.X, mSpriteSize.Y, TRUE));
        }
        DxLib::SetCreateDrawValidGraphMultiSample(0, 0);

        renderWindow();

        mSpr->SetXY(mSpritePos.X, mSpritePos.Y);
        mSpr->SetImage(mRenderGraph, 0, 0, mSpriteSize.X, mSpriteSize.Y);
    }

    void NinePatchImage::renderWindow()
    {
        DxLib::SetDrawScreen(mRenderGraph->GetHandler());
        for (int x = 0; x < 3; ++x)
        {
            for (int y = 0; y < 3; ++y)
            {
                int w = mDivLineSrc[x + 1].X - mDivLineSrc[x].X;
                int h = mDivLineSrc[y + 1].Y - mDivLineSrc[y].Y;

                int handler = DxLib::DerivationGraph(mDivLineSrc[x].X, mDivLineSrc[y].Y, w, h, mSrcGraph->GetHandler());

                DxLib::DrawExtendGraph(mRenderLine[x].X, mRenderLine[y].Y, mRenderLine[x + 1].X, mRenderLine[y + 1].Y,
                    handler, true);

                DxLib::DeleteGraph(handler);
            }
        }
        DxLib::SetDrawScreen(DX_SCREEN_BACK);
    }



    UiBlackWindow::UiBlackWindow(double drawCenterX, double drawCenterY, int gridUnitWidth, int gridUnitHeight, double sideRatioX, double sideRatioY)
        : NinePatchImage(drawCenterX, drawCenterY, 8, gridUnitHeight, sideRatioX, sideRatioY, Images->UiWindowBlack)
    {
        mCurWidth = 8;
        mToWidth = gridUnitWidth;
        mSprHeight = gridUnitHeight;
    }


    void UiBlackWindow::update()
    {
        if (mCurWidth < mToWidth)
        {
            mCurWidth += Time::DeltaMilli();
            mCurWidth = (std::min)(mCurWidth, mToWidth);
            NinePatchImage::SetSize(useful::Vec2<double>{mCurWidth, mSprHeight});
        }

        NinePatchImage::update();
    }



    TextField::TextField(Graph* fontData, int width, int height) : Actor()
    {
        mFontGraph = fontData;
        mSprWidth = width;
        mSprHeight = height;
        mRenderWidth = mSprWidth * PX_PER_GRID / 2;
        mRenderHeight = mSprHeight * PX_PER_GRID / 2;
        mForegroundColor = DxLib::GetColor(255, 255, 255);
        mEdgegroundColor = DxLib::GetColor(32, 32, 32);

        mGraph = std::unique_ptr<Graph>(new Graph(DxLib::MakeScreen(mRenderWidth, mRenderHeight, TRUE)));
        mSpr->SetImage(mGraph.get(), 0, 0, mRenderWidth, mRenderHeight);
        mSpr->SetDrawingMethod(Sprite::DrawingKind::TwoDots);
    }

    void TextField::SetColor(UINT foreground, UINT background)
    {
        mForegroundColor = foreground;
        mEdgegroundColor = background;
    }

    void TextField::SetText(std::string str)
    {
        mText = str;
        RenderDirect([&]() {
            DxLib::DrawStringToHandle(mPaddingLeft, mPaddingTop, mText.c_str(), mForegroundColor, mFontGraph->GetHandler(), mEdgegroundColor);
            });
    }

    void TextField::SetStartXY(double x, double y)
    {
        mSpr->SetXY(x, y);
    }

    void TextField::SetCenterXY(double x, double y)
    {
        mSpr->SetXY(x - mSprWidth/2.0, y - mSprHeight/2.0);
    }

    void TextField::SetPaddingStart(int left, int top)
    {
        mPaddingLeft = left;
        mPaddingTop = top;
    }

    void TextField::RenderDirect(std::function<void()> callback)
    {
        DxLib::SetDrawScreen(mGraph->GetHandler());
        DxLib::ClearDrawScreen();
        callback();
        DxLib::SetDrawScreen(DX_SCREEN_BACK);
    }

}


namespace ingame::main
{

    void StartUi()
    {
        MessageWindow::Init();
        SelectionWindow::Init();
        new LimitTimeWindow();
    }


};



namespace ingame::main
{
    const std::string MessageWindow::CLASS_NAME = "MessageWindow";

    MessageWindow::MessageWindow() : LuaActor("MessageWindowLuaData", true)
    {
        mSprWidth = mLuaData["width"].get_or(0) * PX_PER_GRID / 2;
        mSprHeight = mLuaData["height"].get_or(0) * PX_PER_GRID / 2;
        //new UiWindow(GRID_WIDTH / 2, mLuaData["centerY"].get_or(0), mLuaData["width"].get_or(0), mLuaData["height"].get_or(0), 0.2, 0.2);
        mTextWindow = new UiBlackWindow(GRID_WIDTH / 2, mLuaData["centerY"].get_or(0), mLuaData["width"].get_or(0), mLuaData["height"].get_or(0), 0.2, 0.2);
        mTextWindow->GetSpr()->SetLinkAlive(this->mSpr);
        mTextWindow->GetSpr()->SetLinkXY(this->mSpr);

        mTextReadIndex = 0;
        mNextLetterX = mLuaData["paddingX"]; mNextLetterY = mLuaData["paddingY"];

        constructTextArea();


        if (Player::Sole != nullptr)Player::Sole->IncreaseFixed();
    }

    void MessageWindow::constructTextArea()
    {
        mTextSpr = new Sprite();
        mTextFieldGraph = new Graph(DxLib::MakeScreen(mSprWidth, mSprHeight, TRUE));
        mTextSpr->SetImage(mTextFieldGraph, 0, 0, mSprWidth, mSprHeight);
        mTextSpr->SetDrawingMethod(Sprite::DrawingKind::TwoDots);
        mTextSpr->SetXY(GRID_WIDTH / 2 - mLuaData["width"].get_or(0) / 2, mLuaData["centerY"].get_or(0) - mLuaData["height"].get_or(0) / 2);
        mTextSpr->SetZ(double(ZIndex::UI) - 1);
        mTextSpr->SetLinkAlive(this->mSpr);
        mTextSpr->SetLinkXY(this->mSpr);
    }

    MessageWindow::~MessageWindow()
    {
        // @: このままではスプライトの2重解放のバグが発生します
        // 近日中にisProtectメンバを削除し、linkActive, linkedChiildActivesを追加してください
        // :done
        delete mTextFieldGraph;
        if (Player::Sole != nullptr)Player::Sole->DecreaseFixed();
    }

    bool MessageWindow::GetIsRunning()
    {
        return mRunningCount!=0;
    }
    void MessageWindow::StreamText(std::string text)
    {
        mTextReadIndex = 0;
        mTextBuffer = std::wstring{};
        useful::NarrowStrToWideStr(text, mTextBuffer);
        mRunningCount++;
        mIsPusedSkipButtonOnStart = false;

        this->mWriteLetterTimer = EventTimer([&]()->bool { return writeLetter(); }, mLuaData["letterReadMinInterval"]);
    }
    void MessageWindow::AnimShake(double intensity)
    {
        mRunningCount++;

        auto t = std::shared_ptr<int>(new int{0});
        const double intensityNormal = mLuaData["shakeIntensity"];
        auto timer = new EventTimerAsActor([this, t, intensityNormal]()->bool {
            const int finish = 60 * 1;

            (*t)++;
            double rate = 1 - (double(*t) / finish);
            mSpr->SetXY(std::sin(*t * intensityNormal * M_PI / 180) * intensityNormal * rate, 0);

            if (*t > finish)
            {
                mSpr->SetXY(0, 0);
                mRunningCount--;
                return false;
            }
            return true;
            }, int(FPS60_MILLI));

        timer->GetSpr()->SetLinkAlive(mSpr);
    }

    void MessageWindow::AnimFlash()
    {
        mRunningCount++;

        auto t = std::shared_ptr<int>(new int{ 0 });
        const double flashInterval = mLuaData["flashInterval"];
        const int frame = mLuaData["flashFrame"];

        auto sprWin = mTextWindow->GetSpr();
        auto sprText = mTextSpr;

        auto timer = new EventTimerAsActor([this, t, flashInterval, frame, sprWin, sprText]()->bool {
            const int finish = 60 * 1/2;

            (*t)++;
            if (*t % 2 == 0)
            {
                sprWin->SetBlendPal(16);
                sprText->SetBlendPal(224);
            }
            else
            {
                sprWin->SetBlendPal(224);
                sprText->SetBlendPal(16);
            }
            if ((*t * frame) > finish)
            {
                sprWin->SetBlendPal(255);
                sprText->SetBlendPal(255);
            }
            if ((*t * frame) > finish && Input::Sole->GetKeyDown(KEY_INPUT_SPACE))
            {
                mRunningCount--;
                return false;
            }
            return true;
            }, int(frame*FPS60_MILLI));

        timer->GetSpr()->SetLinkAlive(mSpr);
    }

    bool MessageWindow::hasUnreadText()
    {
        return mTextReadIndex != mTextBuffer.size();
    }
    /// <summary>
    /// 文字を1文字ずつ打っていく
    /// </summary>
    /// <returns></returns>
    bool MessageWindow::writeLetter()
    {
        if (mScrollTimer.IsAlive()) return true;

        mReadIntervalTimeBuffer += mLuaData["letterReadMinInterval"].get_or(1);

        if (Input::Sole->GetKeyDown(KEY_INPUT_SPACE) == 0)
        {// スキップボタンを押していないときの処理
            if (mIsPusedSkipButtonOnStart)
            {
                if (mReadIntervalTimeBuffer < mLuaData["letterReadInterval"].get_or(1))
                {// 普通のスピードで読むように間隔をあける
                    return true;
                }
                else
                {
                    mReadIntervalTimeBuffer -= mLuaData["letterReadInterval"].get_or(1);
                }
            }
            mIsPusedSkipButtonOnStart = true;
        }

        DxLib::SetDrawScreen(mTextFieldGraph->GetHandler());
        {
            std::string str{};
            std::wstring wstr = mTextBuffer.substr(mTextReadIndex, 1);

            auto returnLine = [&]() {
                mNextLetterX = mLuaData["paddingX"].get_or(0);
                int lineHeight = fontSize + mLuaData["lineMargin"].get_or(0);

                if (mNextLetterY + lineHeight < mSprHeight - fontSize - mLuaData["paddingY"].get_or(0))
                {// 改行
                    mNextLetterY += lineHeight;
                }
                else
                {// スクロールする
                    mScrollRemainAmount = (lineHeight) * 2;
                    mNextLetterY -= lineHeight;
                    mScrollTimer = EventTimer([&]() {return scrollLine(); }, 8);
                }
            };

            if (wstr == L"\n")
            {// 改行
                returnLine();
            }
            else
            {
                useful::WideStrToNarrowStr(wstr, str);
                char* c = const_cast<char*>(str.c_str());

                DxLib::DrawStringToHandle(mNextLetterX, mNextLetterY, c, DxLib::GetColor(255, 255, 255), Fonts->Font18Edged->GetHandler(), DxLib::GetColor(32, 32, 32));
                mNextLetterX += (int)(DxLib::GetDrawStringWidthToHandle(c, (int)(DxLib::strlenDx(c)), Fonts->Font18Edged->GetHandler()) * 0.9);

                if (mNextLetterX > mSprWidth - mLuaData["paddingX"].get_or(0) - fontSize)
                {// 文が端まで行った
                    returnLine();
                }
            }
            mTextReadIndex++;
        }
        DxLib::SetDrawScreen(DX_SCREEN_BACK);
        if (hasUnreadText())
        {
            return true;
        }
        else
        {// ボタン待ちしてプロセス終了

            this->mWriteLetterTimer = EventTimer([&]()->bool {
                if (!Input::Sole->GetKeyDown(KEY_INPUT_SPACE))
                {// 一旦ボタンを離してもらってから
                    this->mWriteLetterTimer = EventTimer([&]()->bool {
                        if (Input::Sole->GetKeyDown(KEY_INPUT_SPACE))
                        {
                            //mIsRunning = false;
                            new EventTimerAsActor([&]()->bool {mRunningCount--; return false; }, 100);
                            return false;
                        }
                        return true;
                        }, 1000 / 60);
                }
                return true;
                }, 1000 / 60);

            return true;
        }
    }
    /// <summary>
    /// 行のスクロール
    /// </summary>
    /// <returns></returns>
    bool MessageWindow::scrollLine()
    {
        DxLib::SetDrawScreen(mTextFieldGraph->GetHandler());
        {
            int top = mLuaData["paddingY"].get_or(0);
            int handler = DxLib::MakeScreen(mSprWidth, mSprHeight - top - 1, TRUE);
            {
                int a = GetDrawScreenGraph(0, top + 1, mSprWidth, mSprHeight, handler);
                DxLib::ClearDrawScreen();
                DxLib::DrawGraph(0, top, handler, TRUE);
                //const RECT rect = RECT{ 0, 0, mWidth, mLuaData["paddingY"].get_or(0) };
                //DxLib::ClearDrawScreen(&rect);
            }
            DxLib::DeleteGraph(handler);
        }
        DxLib::SetDrawScreen(DX_SCREEN_BACK);

        mScrollRemainAmount--;
        if (mScrollRemainAmount == 0) return false;
        return true;
    }
    void MessageWindow::update()
    {
        LuaActor::update();
        if (mTime > 500)
        {// ウインドウの展開のためちょっと待つ
            mWriteLetterTimer.Update();
            mScrollTimer.Update();
        }
    }

    void MessageWindow::Init()
    {
        luaManager::Lua.new_usertype<MessageWindow>(
            "MessageWindow",
            sol::constructors<MessageWindow()>(),
            "open", []()->MessageWindow* {return new MessageWindow(); },
            "streamText", &MessageWindow::StreamText,
            "animShake", &MessageWindow::AnimShake,
            "animFlash", & MessageWindow::AnimFlash,
            "isRunning", &MessageWindow::GetIsRunning,
            "close", [](MessageWindow* self) {Sprite::Destroy(self->GetSpr()); });
    }
}






namespace ingame::main
{
    SelectionWindow::SelectionWindow(sol::table table) : LuaActor("SelectionWindowLuaData", true)
    {
        for (int i = 1; i <= table.size(); ++i)
        {
            mOptions.push_back(table[i].get_or(std::string("(null)")));
        }
        mOptionNum = mOptions.size();

        mGridUnitWidth = mLuaData["width"].get_or(0);
        mGridUnitHeight = 2 * mLuaData["paddingY"].get_or(0) * 2 / PX_PER_GRID
            + mOptionNum * mLuaData["lineHeight"].get_or(0) * 2 / PX_PER_GRID;

        // ウィンドウ
        mWindow = new UiBlackWindow(
            mLuaData["centerX"].get_or(0), 
            mLuaData["bottomY"].get_or(0)- mGridUnitHeight /2,
            mGridUnitWidth,
            mGridUnitHeight,
            0.2, 0.2);
        mWindow->GetSpr()->SetLinkAlive(this->mSpr);
        mWindow->GetSpr()->SetZ(double(ZIndex::UI) - 10);

        // テキスト
        mTextFieldGraph = new Graph(DxLib::MakeScreen(mGridUnitWidth * 2, mGridUnitHeight * 2, TRUE));
        mSpr->SetImage(mTextFieldGraph, 0, 0, mGridUnitWidth * 2, mGridUnitHeight * 2);
        mSpr->SetDrawingMethod(Sprite::DrawingKind::TwoDots);
        mSpr->SetXY(
            mLuaData["centerX"].get_or(0) - mGridUnitWidth / 2,
            mLuaData["bottomY"].get_or(0) - mGridUnitHeight);
        mSpr->SetZ(double(ZIndex::UI) - 11);

        // カーソル
        mCursorSpr = new Sprite();
        mCursorSpr->SetLinkXY(this->mSpr);
        mCursorSpr->SetLinkAlive(this->mSpr);
        mCursorSpr->SetZ(double(ZIndex::UI) - 12);
        //mCursorSpr->SetDrawingMethod(Sprite::DrawingKind::TwoDots);
        resetCursor();

        if (Player::Sole != nullptr)Player::Sole->IncreaseFixed();

        renderText();
        this->mInputTimer = EventTimer([&]()->bool {
            renderText();
            mInputTimer = EventTimer([&]()->bool {
                return inputOptions(); 
                }, 1000 / 60);
            return true;
            }, 500);
        mIsRunning = true;
    }

    SelectionWindow::~SelectionWindow()
    {
        delete mTextFieldGraph;
        if (Player::Sole != nullptr)Player::Sole->DecreaseFixed();
    }

    bool SelectionWindow::GetIsRunning()
    {
        return mIsRunning;
    }

    int SelectionWindow::GetSelectedIndex()
    {
        return mSelectedIndex;
    }

    std::string SelectionWindow::GetSelectedWord()
    {
        return mOptions[mSelectedIndex];
    }

    void SelectionWindow::renderText()
    {
        DxLib::SetDrawScreen(mTextFieldGraph->GetHandler());
        for (int i = 0; i < mOptionNum; ++i)
        {
            int x = mLuaData["paddingX"].get_or(0);
            int y = mLuaData["paddingY"].get_or(0) + i * mLuaData["lineHeight"].get_or(0);
            
            DxLib::DrawStringToHandle(x, y, mOptions[i].c_str(), DxLib::GetColor(255, 255, 255), Fonts->Font18Edged->GetHandler(), DxLib::GetColor(32, 32, 32));
        }
        DxLib::SetDrawScreen(DX_SCREEN_BACK);
    }

    bool SelectionWindow::inputOptions()
    {
        mButton.Update();
        if (mButton.ChackIntervalPress(KEY_INPUT_W, mLuaData["buttonIntervalFirst"].get_or(1), mLuaData["buttonIntervalSecond"].get_or(1)))
        {
            mSelectedIndex--;
        }
        else if (mButton.ChackIntervalPress(KEY_INPUT_S, mLuaData["buttonIntervalFirst"].get_or(1), mLuaData["buttonIntervalSecond"].get_or(1)))
        {
            mSelectedIndex++;
        }
        mSelectedIndex = (mSelectedIndex + mOptionNum) % mOptionNum;

        if (mButton.CheckJustAfterPress(KEY_INPUT_SPACE))
        {
            mIsRunning = false;
            return false;
        }
        
        resetCursor();

        return true;
    }
    void SelectionWindow::resetCursor()
    {
        mCursorSpr->SetImage(Images->UiArrow, 0, 0, 16, 16);
        mCursorSpr->SetXY(
            mLuaData["cursolLeftX"].get_or(0),
            (mLuaData["paddingY"].get_or(0) + mSelectedIndex * mLuaData["lineHeight"].get_or(0)) * 2 / PX_PER_GRID
        );
    }
    void SelectionWindow::update()
    {
        LuaActor::update();
        //if (mTime > 500)
        {// ウインドウの展開のためちょっと待つ
            mInputTimer.Update();
        }
    }
    void SelectionWindow::Init()
    {
        luaManager::Lua.new_usertype<SelectionWindow>(
            "SelectionWindow",
            sol::constructors<SelectionWindow(sol::table table)>(),
            "open", [](sol::table table)->SelectionWindow* {return new SelectionWindow(table); },
            "isRunning", &SelectionWindow::GetIsRunning,
            "selectedIndex", &SelectionWindow::GetSelectedIndex,
            "selectedWord", & SelectionWindow::GetSelectedWord,
            "close", [](SelectionWindow* self) {Sprite::Destroy(self->GetSpr()); });
    }
}



namespace ingame::main
{
    LimitTimeWindow::LimitTimeWindow() : LuaActor("LimitTimeWindowLuaData", true)
    {
        // ウィンドウ
        mWindow = new NinePatchImage(
            mLuaData["centerX"].get_or(0),
            mLuaData["centerY"].get_or(0),
            mLuaData["width"].get_or(0),
            mLuaData["height"].get_or(0),
            0.2, 0.2,
            Images->UiWhiteRoundRect);
        mWindow->GetSpr()->SetBlendPal(mLuaData["opacity"].get_or(0));
        mWindow->GetSpr()->SetLinkAlive(this->mSpr);
        mWindow->GetSpr()->SetZ(double(ZIndex::UI) - 10);

        mTextField = new TextField(Fonts->Font18EdgedThick, mLuaData["width"].get_or(0), mLuaData["height"].get_or(0));
        mTextField->SetPaddingStart(mLuaData["paddingLeft"].get_or(0), mLuaData["paddingTop"].get_or(0));
        sol::table fore = mLuaData["textForegroundColor"];
        sol::table back = mLuaData["textEdgegroundColor"];
        mTextField->SetColor(
            DxLib::GetColor(fore[1].get_or(0), fore[2].get_or(0), fore[3].get_or(0)), 
            DxLib::GetColor(back[1].get_or(0), back[2].get_or(0), back[3].get_or(0)));
        mTextField->GetSpr()->SetLinkAlive(this->mSpr);
        mTextField->GetSpr()->SetZ(double(ZIndex::UI) - 11);
        mTextField->SetCenterXY(mLuaData["centerX"].get_or(0), mLuaData["centerY"].get_or(0));
        
        GameController::Sole->OnGameTimeChanged.push_back([&](int minu, int sec) {renderText(minu, sec); });
        renderText(GameController::GAME_TIME_START / 60, GameController::GAME_TIME_START % 60);
    }

    void LimitTimeWindow::update()
    {
        LuaActor::update();
    }

    void LimitTimeWindow::renderText(int minutue, int sec)
    {
        std::ostringstream oss;
        oss << minutue << ":" << std::setfill('0') << std::setw(2) << sec;
        mTextField->SetText(oss.str());
    }

}












