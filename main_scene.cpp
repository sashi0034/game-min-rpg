
#include "main_scene.h"
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



    LuaCollideActor::LuaCollideActor(std::string luaClass, bool canLuaConstruct, collider::Shape* col, UINT mask) : CollideActor(col, mask)
    {
        mLuaClassName = luaClass;
        mLuaData = luaManager::Lua.create_table();
        if (canLuaConstruct) luaConstructor();
    }
    void LuaCollideActor::luaConstructor()
    {
        mLuaData = luaManager::Lua[mLuaClassName]["new"](static_cast<CollideActor*>(this));

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

    /// <summary>
    /// キャラクターの標準的な移動
    /// </summary>
    /// <param name="curX">現在Xポインタ</param>
    /// <param name="curY">現在Yポインタ</param>
    /// <param name="toX"></param>
    /// <param name="toY"></param>
    /// <param name="vel">速度</param>
    /// <returns>完了したらtrue</returns>
    bool Character::DoMove(double *curX, double *curY, double toX, double toY, double vel)
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
    }
    void Character::GetMatXY(int *x, int *y)
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
    bool Character::CanMoveTo(double x, double y, EAngle toAng)
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

            return  !MapManager::Sole->GetMatAt(matX-moveXY.X, matY-moveXY.Y)->IsStep[static_cast<int>(toAng)] &&
                (MapManager::Sole->GetMatAt(matX, matY)->IsBridge || !MapManager::Sole->GetMatAt(matX, matY)->IsWall);
        }
    }




    UiWindow::UiWindow(double drawCenterX, double drawCenterY, int roughWidth, int roughHeight, double sideRatioX, double sideRatioY) : Actor()
    {
        mSrcGraph = Images->UiWindows;
        DxLib::GetGraphSize(mSrcGraph->GetHandler(), &mSrcSize.X, &mSrcSize.Y);

        mDrawSize = useful::Vec2<int>{ roughWidth * ROUGH_SCALE, roughHeight * ROUGH_SCALE };
        mSideRatio = useful::Vec2<double>{ sideRatioX, sideRatioY };

        mDivLineSrc[0] = useful::Vec2<int>{ 0,0 };
        mDivLineSrc[1] = useful::Vec2<int>{ int(sideRatioX * mSrcSize.X),int(sideRatioY * mSrcSize.Y) };
        mDivLineSrc[2] = useful::Vec2<int>{ int((1 - sideRatioX) * mSrcSize.X),int((1 - sideRatioY) * mSrcSize.Y) };
        mDivLineSrc[3] = useful::Vec2<int>{ mSrcSize.X, mSrcSize.Y };

        mDrawScreenLeftTop = useful::Vec2<double>{drawCenterX-roughWidth/2.0, drawCenterY-roughHeight/2.0};

        int cornerLength = (std::min)(mDrawSize.X*mSideRatio.X, mDrawSize.Y * mSideRatio.Y);
        mRenderLine[0] = useful::Vec2<int>{ 0,0 };
        mRenderLine[1] = useful::Vec2<int>{ cornerLength, cornerLength };
        mRenderLine[2] = useful::Vec2<int>{ mDrawSize.X - cornerLength, mDrawSize.Y - cornerLength };
        mRenderLine[3] = useful::Vec2<int>{ mDrawSize.X, mDrawSize.Y };


        DxLib::SetCreateDrawValidGraphMultiSample(4, 2);
        mRenderGraph = new Graph(DxLib::MakeScreen(mDrawSize.X, mDrawSize.Y, TRUE));
        DxLib::SetCreateDrawValidGraphMultiSample(0, 0);

        renderWindow();

        mSpr->SetZ(ZIndex::UI);
        mSpr->SetXY(mDrawScreenLeftTop.X, mDrawScreenLeftTop.Y);
        mSpr->SetImage(mRenderGraph, 0, 0, mDrawSize.X, mDrawSize.Y);
        mSpr->SetDrawingMethod(Sprite::DrawingKind::DotByDot);
    }

    UiWindow::~UiWindow()
    {
        delete mRenderGraph;
    }


    void UiWindow::update()
    {
        Actor::update();
    }

    void UiWindow::renderWindow()
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

}

namespace ingame::main
{
    /// <summary>
    /// プレイヤー
    /// 中心に16*16pxの板があるイメージでそれの左上が原点
    /// 描画時に座標をずらす
    /// </summary>
    /// <param name="startX"></param>
    /// <param name="startY"></param>
    Player::Player(double startX, double startY) : LuaCollideActor("Player", false, new collider::Rectangle(8, 16, 16, 16), 1)
    {
        Sole = this;

        mSpr->SetLinkXY(ScrollManager::Sole->GetSpr());
        mSpr->SetImage(Images->Kisaragi, 0, 0, 32, 32);
        mSpr->SetZ(ZIndex::CHARACTER);

        mX = startX;
        mY = startY;

        mLuaData = luaManager::Lua[mLuaClassName]["new"]();
        mLuaData["doWaitForMove"] = [&]()->bool {return this->doWaitForMove(); };
        mLuaData["doMove"] = [&]()->bool {return this->doMove(); };

        mVel = mLuaData["vel"];

        new UiWindow(ROUGH_WIDTH/2, ROUGH_HEIGHT-120, 200,120,0.2, 0.2);
    }
    Player::~Player()
    {
        Sole = nullptr;
    }
    double Player::GetX()
    {
        return mX;
    }
    double Player::GetY()
    {
        return mY;
    }
    void Player::SetFixed(bool isFixed)
    {
        mLuaData["isFixed"] = isFixed;
    }
    void Player::update()
    {
        LuaCollideActor::update();

    }
    void Player::luaUpdate()
    {
        luaManager::Lua[mLuaClassName]["update"](mLuaData);

        mSpr->SetXY(mX - 8, mY - 16 - 4);
        animation();
    }

    bool Player::doMove()
    {
        if (Character::DoMove(&mX, &mY, mGotoX, mGotoY, mVel))
        {
            return true;
        }
        else
        {
            mX += moveUnit/2; mY += moveUnit/2;
            Character::AttachToGridXY(&mX, &mY, moveUnit);
            return false;
        }
    }

    void Player::animation()
    {
        int frame = (mAnimTime / 200);
        mSpr->SetFlip(false);
        if (mWaitTime > 50)
        {// 待機
            switch (mAngle)
            {
            case EAngle::DOWN:
                mSpr->SetImage((frame % 4) * 32, 0);
                break;
            case EAngle::RIGHT:
                mSpr->SetImage((frame % 3) * 32, 32 * 1);
                break;
            case EAngle::UP:
                mSpr->SetImage((frame % 4) * 32, 32 * 2);
                break;
            case EAngle::LEFT:
                mSpr->SetImage((frame % 3) * 32, 32 * 1);
                mSpr->SetFlip(true);
                break;
            }
        }
        else
        {// 移動
            switch (mAngle)
            {
            case EAngle::DOWN:
                mSpr->SetImage((frame % 4) * 32, 32 * 3);
                break;
            case EAngle::RIGHT:
                mSpr->SetImage((frame % 6) * 32, 32 * 4);
                break;
            case EAngle::UP:
                mSpr->SetImage((frame % 4) * 32, 32 * 5);
                break;
            case EAngle::LEFT:
                mSpr->SetImage((frame % 6) * 32, 32 * 4);
                mSpr->SetFlip(true);
                break;
            }
        }


        mAnimTime += Time::DeltaMilli();
    }

    bool Player::doWaitForMove()
    {
        EAngle ang = EAngle::NONE;
        if (DxLib::CheckHitKey(KEY_INPUT_W)) ang = EAngle::UP;
        if (DxLib::CheckHitKey(KEY_INPUT_A)) ang = EAngle::LEFT;
        if (DxLib::CheckHitKey(KEY_INPUT_S)) ang = EAngle::DOWN;
        if (DxLib::CheckHitKey(KEY_INPUT_D)) ang = EAngle::RIGHT;

        if (ang != EAngle::NONE)
        {
            mAngle = ang;
            useful::Vec2 xy = Angle::ToXY(ang);

            bool canMove = false;

            switch (ang)
            {
            case EAngle::RIGHT:
                canMove =
                    Character::CanMoveTo(mX + 16 * 3 / 4 + xy.X * moveUnit, mY + 16 * 1 / 4 + xy.Y * moveUnit, ang) &&
                    Character::CanMoveTo(mX + 16 * 3 / 4 + xy.X * moveUnit, mY + 16 * 3 / 4 + xy.Y * moveUnit, ang);
                break;
            case EAngle::DOWN:
                canMove =
                    Character::CanMoveTo(mX + 16 * 1 / 4 + xy.X * moveUnit, mY + 16 * 3 / 4 + xy.Y * moveUnit, ang) &&
                    Character::CanMoveTo(mX + 16 * 3 / 4 + xy.X * moveUnit, mY + 16 * 3 / 4 + xy.Y * moveUnit, ang);
                break;
            case EAngle::LEFT:
                canMove =
                    Character::CanMoveTo(mX + 16 * 1 / 4 + xy.X * moveUnit, mY + 16 * 1 / 4 + xy.Y * moveUnit, ang) &&
                    Character::CanMoveTo(mX + 16 * 1 / 4 + xy.X * moveUnit, mY + 16 * 3 / 4 + xy.Y * moveUnit, ang);
                break;
            case EAngle::UP:
                canMove =
                    Character::CanMoveTo(mX + 16 * 1 / 4 + xy.X * moveUnit, mY + 16 * 1 / 4 + xy.Y * moveUnit, ang) &&
                    Character::CanMoveTo(mX + 16 * 3 / 4 + xy.X * moveUnit, mY + 16 * 1 / 4 + xy.Y * moveUnit, ang);
                break;
            }

            if (canMove)
            {
                mGotoX = mX + xy.X * moveUnit;
                mGotoY = mY + xy.Y * moveUnit;

                if (mWaitTime > 1000) mAnimTime = 0;
                mWaitTime = 0;
                return true;

            }
            else
            {
                return false;
            }
        }
        else
        {
            mWaitTime += Time::DeltaMilli();
            return false;
        }
    }
}


namespace ingame::main
{
    Punicat::Punicat(double startX, double startY, ECharacterKind characterKind, std::string uniqueName) : 
        LuaCollideActor(uniqueName, true, new collider::Rectangle(4, 8, 16, 16), 1),
        INonPlayerCharacter(characterKind, uniqueName)
    {
        mSpr->SetLinkXY(ScrollManager::Sole->GetSpr());
        mSpr->SetImage(Images->Punicat, 0, 0, 24, 24);
        mSpr->SetZ(ZIndex::CHARACTER);

        mX = startX;
        mY = startY;

        mLuaData["doMove"] = [&](double x, double y)->bool {return this->doMove(x, y); };
        mLuaData["getX"] = [&]()->double {return this->mX; };
        mLuaData["getY"] = [&]()->double {return this->mY; };

        mVel = mLuaData["vel"];
        mFrameInterval = mLuaData["frameInterval"].get_or(0);
    }

    void Punicat::update()
    {
        luaManager::Lua[mLuaClassName]["update"](mLuaData);

        mSpr->SetXY(mX - 4, mY - 8 - 4);

        animation();
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
    bool Punicat::doMove(double gotoX, double gotoY)
    {
       
        if (Character::DoMove(&mX, &mY, gotoX, gotoY, mVel))
        {
            if (std::abs(mX - gotoX) > moveUnit / 2 || std::abs(mY - gotoY) > moveUnit/2)
            {
                mAngle = Angle::ToAng(gotoX - mX, gotoY - mY);
            }
            
            return true;
        }
        else
        {
            mX += moveUnit / 2; mY += moveUnit / 2;
            Character::AttachToGridXY(&mX, &mY, moveUnit);
            return false;
        }
    }

}



namespace ingame::main
{
    TestNPC::TestNPC(int startX, int startY) : LuaCollideActor("TestNPC", false, new collider::Rectangle(8, 16, 16, 16), 1)
    {
        mX = startX;
        mY = startY;

        mSpr->SetImage(Images->Kisaragi, 0, 0, 32, 32);
        mSpr->SetZ(ZIndex::CHARACTER);
        mLuaData = luaManager::Lua[mLuaClassName]["new"](static_cast<CollideActor*>(this));
        mLuaData["doMove"] = [&](double x, double y)->bool {return this->doMove(x, y); };
        mLuaData["getX"] = [&]()->double {return this->mX; };
        mLuaData["getY"] = [&]()->double {return this->mY; };
        //mLuaData["doWaitForMove"] = [&]() {this->doWaitForMove(); };
    }
    void TestNPC::update()
    {
        LuaCollideActor::update();
    }
    void TestNPC::luaUpdate()
    {
        luaManager::Lua[mLuaClassName]["update"](mLuaData);
        mSpr->SetXY(mX-8, mY-8);
    }

    bool TestNPC::doMove(double x, double y)
    {
        double vx=0, vy=0;
        if (mX < x - 1) vx = 1;
        if (mX > x + 1) vx = -1;
        if (mY < y - 1) vy = 1;
        if (mY > y + 1) vy = -1;

        if (vx == 0 && vy == 0) 
        {
            return false; 
        }
     

        double vel = 20;
        mX += vx * vel * Time::DeltaSec();
        mY += vy * vel * Time::DeltaSec();
        
        return true;
    }

    bool TestNPC::doWaitForMove()
    {
        return false;
    }


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





}





