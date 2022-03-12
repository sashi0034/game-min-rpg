#include "main_scene_ui.h"
#include "start.h"

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


        mSpr->SetZ(ZIndex::UI);
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



    UiWindow::UiWindow(double drawCenterX, double drawCenterY, int gridUnitWidth, int gridUnitHeight, double sideRatioX, double sideRatioY)
        : NinePatchImage(drawCenterX, drawCenterY, 8, gridUnitHeight, sideRatioX, sideRatioY, Images->UiWindowBlack)
    {
        mCurWidth = 8;
        mToWidth = gridUnitWidth;
        mHeight = gridUnitHeight;
    }


    void UiWindow::update()
    {
        if (mCurWidth < mToWidth)
        {
            mCurWidth += Time::DeltaMilli();
            mCurWidth = (std::min)(mCurWidth, mToWidth);
            NinePatchImage::SetSize(useful::Vec2<double>{mCurWidth, mHeight});
        }

        NinePatchImage::update();
    }

};



namespace ingame::main
{
    const std::string MessageWindow::CLASS_NAME = "MessageWindow";

    MessageWindow::MessageWindow() : LuaActor("MessageWindowLuaData", true)
    {
        mWidth = mLuaData["width"].get_or(0) * PX_PER_GRID / 2;
        mHeight = mLuaData["height"].get_or(0) * PX_PER_GRID / 2;
        //new UiWindow(GRID_WIDTH / 2, mLuaData["centerY"].get_or(0), mLuaData["width"].get_or(0), mLuaData["height"].get_or(0), 0.2, 0.2);
        mTextWindow = new UiWindow(GRID_WIDTH / 2, mLuaData["centerY"].get_or(0), mLuaData["width"].get_or(0), mLuaData["height"].get_or(0), 0.2, 0.2);
        mTextWindow->GetSpr()->SetLinkActive(this->mSpr);

        mTextReadIndex = 0;
        mNextLetterX = mLuaData["paddingX"]; mNextLetterY = mLuaData["paddingY"];

        mTextFieldGraph = new Graph(DxLib::MakeScreen(mWidth, mHeight, TRUE));
        mSpr->SetImage(mTextFieldGraph, 0, 0, mWidth, mHeight);
        mSpr->SetDrawingMethod(Sprite::DrawingKind::TwoDots);
        mSpr->SetXY(GRID_WIDTH / 2 - mLuaData["width"].get_or(0) / 2, mLuaData["centerY"].get_or(0) - mLuaData["height"].get_or(0) / 2);
        mSpr->SetZ(ZIndex::UI - 1);

        if (Player::Sole != nullptr)Player::Sole->IncreaseFixed();
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
        return mIsRunning;
    }
    void MessageWindow::StreamText(std::string text)
    {
        mTextReadIndex = 0;
        mTextBuffer = std::wstring{};
        useful::NarrowStrToWideStr(text, mTextBuffer);
        mIsRunning = true;
        mIsPusedSkipButtonOnStart = false;

        this->mWriteLetterTimer = EventTimer([&]()->bool { return writeLetter(); }, mLuaData["letterReadMinInterval"]);
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

                if (mNextLetterY + lineHeight < mHeight - fontSize - mLuaData["paddingY"].get_or(0))
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

                DxLib::DrawStringToHandle(mNextLetterX, mNextLetterY, c, DxLib::GetColor(255, 255, 255), Images->Font18Edged->GetHandler(), DxLib::GetColor(32, 32, 32));
                mNextLetterX += (int)(DxLib::GetDrawStringWidthToHandle(c, (int)(DxLib::strlenDx(c)), Images->Font18Edged->GetHandler()) * 0.9);

                if (mNextLetterX > mWidth - mLuaData["paddingX"].get_or(0) - fontSize)
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
                            mIsRunning = false;
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
            int handler = DxLib::MakeScreen(mWidth, mHeight - top - 1, TRUE);
            {
                int a = GetDrawScreenGraph(0, top + 1, mWidth, mHeight, handler);
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
            "isRunning", &MessageWindow::GetIsRunning,
            "close", [](MessageWindow* self) {Sprite::Dispose(self->GetSpr()); });
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
        mWindow = new UiWindow(
            mLuaData["centerX"].get_or(0), 
            mLuaData["bottomY"].get_or(0)- mGridUnitHeight /2,
            mGridUnitWidth,
            mGridUnitHeight,
            0.2, 0.2);
        mWindow->GetSpr()->SetLinkActive(this->mSpr);
        mWindow->GetSpr()->SetZ(ZIndex::UI - 10);

        // テキスト
        mTextFieldGraph = new Graph(DxLib::MakeScreen(mGridUnitWidth * 2, mGridUnitHeight * 2, TRUE));
        mSpr->SetImage(mTextFieldGraph, 0, 0, mGridUnitWidth * 2, mGridUnitHeight * 2);
        mSpr->SetDrawingMethod(Sprite::DrawingKind::TwoDots);
        mSpr->SetXY(
            mLuaData["centerX"].get_or(0) - mGridUnitWidth / 2,
            mLuaData["bottomY"].get_or(0) - mGridUnitHeight);
        mSpr->SetZ(ZIndex::UI - 11);

        // カーソル
        mCursorSpr = new Sprite();
        mCursorSpr->SetLinkXY(this->mSpr);
        mCursorSpr->SetLinkActive(this->mSpr);
        mCursorSpr->SetZ(ZIndex::UI - 12);
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

    void SelectionWindow::renderText()
    {
        DxLib::SetDrawScreen(mTextFieldGraph->GetHandler());
        for (int i = 0; i < mOptionNum; ++i)
        {
            int x = mLuaData["paddingX"].get_or(0);
            int y = mLuaData["paddingY"].get_or(0) + i * mLuaData["lineHeight"].get_or(0);
            
            DxLib::DrawStringToHandle(x, y, mOptions[i].c_str(), DxLib::GetColor(255, 255, 255), Images->Font18Edged->GetHandler(), DxLib::GetColor(32, 32, 32));
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
            "close", [](SelectionWindow* self) {Sprite::Dispose(self->GetSpr()); });
    }
}

