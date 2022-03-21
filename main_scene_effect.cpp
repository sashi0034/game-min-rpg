#include "stdafx.h"
#include "main_scene_effect.h"
#include "start.h"


namespace ingame::main::effect
{
	void StartEffect()
	{
		new CloudController();
		new SpiritController();
	}

	CloudController::CloudController() : Actor()
	{
		for (int i = 0; i < 64; ++i)
		{
			new Cloud(this);
		}
	}

	void CloudController::update()
	{
		double rateY = (ScrollManager::Sole->GetY() - ScrollManager::Sole->GetMaxXY().Y) / (ScrollManager::Sole->GetMinXY().Y - ScrollManager::Sole->GetMaxXY().Y);

		mSpr->SetXY(ScrollManager::Sole->GetX(), THICKNESS * (-1 + 2 * rateY));
	}

	Cloud::Cloud(CloudController* parent) : Actor()
	{
		mSpr->SetLinkActive(parent->GetSpr());
		mSpr->SetLinkXY(parent->GetSpr());

		int r = Rand->Get(3);
		if (r == 0)
		{
			mSpr->SetImage(Images->Cloud[0], 0, 0, 32, 16);
			mSprOriginX = -16; mSprOriginY = -8;
		}
		else if (r == 1)
		{
			mSpr->SetImage(Images->Cloud[1], 0, 0, 64, 32);
			mSprOriginX = -32; mSprOriginY = -16;
		}
		else if (r == 2)
		{
			mSpr->SetImage(Images->Cloud[1], 0, 0, 128, 64);
			mSprOriginX = -64; mSprOriginY = -32;
		}

		mOriginY = -Rand->Get(CloudController::THICKNESS * 2);
		if (Rand->Get(2) == 0) mOriginY = GRID_HEIGHT - mOriginY;

		mX = Rand->Get(GRID_WIDTH);
		mY = mOriginY;

		mVelX = 5 + Rand->Get(45);
		mVelX *= Rand->Get(2) == 0 ? 1 : -1;
		mAmplitude = 2 + Rand->Get(16);

		mSpr->SetFlip(Rand->Get(2) == 0 ? false : true);
		mSpr->SetBlendPal(200);

		mSpr->SetZ(double(ZIndex::CLOUD));

		mUpdateTimer = EventTimer([&]() {updateCloud(); return true; }, 20);
	}


	void Cloud::update()
	{
		mUpdateTimer.Update();

		Actor::update();
	}

	void Cloud::updateCloud()
	{
		double deltaSec = 20.0 / 1000;
		int x, y;
		mSpr->GetScreenXY(&x, &y);
		if (mVelX < 0 && x < mSprOriginX * 2)
		{
			mX += -x - mSprOriginX * 2 + GRID_WIDTH + (mSprOriginX * -1);
		}

		if (mVelX > 0 && x > GRID_WIDTH)
		{
			mX += -x - (mSprOriginX * 2 * -1);
		}

		mY = mOriginY + mAmplitude * std::sin((mTime / mAmplitude) / 180.0 * M_PI);

		mX += mVelX * deltaSec;
		mSpr->SetXY(mX + mSprOriginX, mY + mSprOriginY);
	}
}
namespace ingame::main::effect
{
	SpiritController::SpiritController() : Actor()
	{
		for (int i = 0; i < 64; ++i)
		{
			new Spirit(this);
		}
	}
	void SpiritController::update()
	{
		Actor::update();
	}

	const useful::Vec2<int> Spirit::imageSize = useful::Vec2<int>{ 64, 64 };

	Spirit::Spirit(SpiritController* parent) : Actor()
	{
		mSprOriginPt = useful::Vec2<int>{ imageSize.X/2, imageSize.Y/2 } / PX_PER_GRID * -1;
		mPt = useful::Vec2<double>{ (double)Rand->Get(GRID_WIDTH), (double)Rand->Get(GRID_HEIGHT) };

		int deg = Rand->Get(360);
		double rad = deg * M_PI / 180;
		mVel = useful::Vec2<double>{ std::cos(rad), std::sin(rad)} * 20;

		mSpr->SetImage(Images->EffectSpirit, 0, 0, imageSize.X, imageSize.Y);
		mSpr->SetDrawingMethod(Sprite::DrawingKind::DotByDot);
		mSpr->SetZ(double(ZIndex::CLOUD) + 1);
		mSpr->SetLinkXY(ScrollManager::Sole->GetSpr());
		mSpr->SetLinkActive(parent->GetSpr());
	}

	void Spirit::update()
	{
		Actor::update();
		updateSpirit(Time::DeltaMilli());
	}

	void Spirit::updateSpirit(int deltaMilli)
	{
		mPt = mPt + mVel * (deltaMilli/1000.0);

		int x, y;
		mSpr->GetScreenXY(&x, &y);
		x -= mSprOriginPt.X;
		y -= mSprOriginPt.Y;
		if (
			x < mSprOriginPt.X ||
			GRID_WIDTH-mSprOriginPt.X< x || 
			y < mSprOriginPt.Y ||
			GRID_HEIGHT - mSprOriginPt.Y < y)
		{
			mAnimTIme = 0;
			mPt = useful::Vec2<double>{ (double)Rand->Get(GRID_WIDTH), (double)Rand->Get(GRID_HEIGHT) };
			mPt.X -= ScrollManager::Sole->GetX();
			mPt.Y -= ScrollManager::Sole->GetY();
		}


		const double blendSpeed = 0.2;
		double blendRad = mAnimTIme * blendSpeed * M_PI / 180;
		mSpr->SetBlendPal(100-std::cos(blendRad)*100);
		mSpr->SetRotationRad(blendRad);
		mSpr->SetXY(mPt.X+mSprOriginPt.X, mPt.Y+mSprOriginPt.Y);
		mAnimTIme += deltaMilli;
	}

}

