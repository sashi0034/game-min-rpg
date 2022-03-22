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
		mSpr->SetLinkAlive(parent->GetSpr());
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
		//for (int i = 0; i < 64; ++i)
		//{
		//	new Spirit(this);
		//}
		mGenerateTImer = EventTimer([&]() {
			if (Spirit::AliveCount < 20)
			{
				new Spirit(this);
			}
			return true; 
			}, 50);
	}
	void SpiritController::update()
	{
		Actor::update();
		mGenerateTImer.Update();
	}

	const useful::Vec2<int> Spirit::imageSize = useful::Vec2<int>{ 64, 64 };

	int Spirit::AliveCount = 0;

	Spirit::Spirit(SpiritController* parent) : Actor()
	{
		AliveCount++;

		mSprOriginPt = useful::Vec2<int>{ imageSize.X / 2, imageSize.Y / 2 } / PX_PER_GRID * -1;
		mPt = useful::Vec2<double>{ (double)Rand->Get(GRID_WIDTH), (double)Rand->Get(GRID_HEIGHT) };
		mPt.X -= ScrollManager::Sole->GetX();
		mPt.Y -= ScrollManager::Sole->GetY();

		int deg = Rand->Get(360);
		double rad = deg * M_PI / 180;
		mVel = useful::Vec2<double>{ std::cos(rad), std::sin(rad) } *velMax;
		resetAccel();

		if (Rand->Get(100) < 20)
		{// •ª—ô‚µ‚Ä‚¢‚­
			createSplitAfterImage();
		}

		mSpr->SetImage(Images->EffectSpirit, 0, 0, imageSize.X, imageSize.Y);
		mSpr->SetDrawingMethod(Sprite::DrawingKind::DotByDot);
		mSpr->SetScale(100.0 / (100 + Rand->Get(100)));
		mSpr->SetZ(double(ZIndex::CLOUD) + 1);
		//if (Rand->Get(100) < 5) mSpr->SetBlendMode(DX_BLENDMODE_ADD);
		mSpr->SetBlendPal(0);
		mSpr->SetLinkXY(ScrollManager::Sole->GetSpr());
		mSpr->SetLinkAlive(parent->GetSpr());
	}

	Spirit::~Spirit()
	{
		AliveCount--;
	}

	void Spirit::createSplitAfterImage()
	{
		auto spliter = new EventTimerAsActor([&]() {
			Sprite* afterimage = Sprite::CopyVisuallyFrom(mSpr);
			afterimage->SetZ(afterimage->GetZ() + 0.001);
			afterimage->SetDrawingMethod(Sprite::DrawingKind::DotByDot);
			auto time = std::shared_ptr<int>(new int{ 0 });

			auto created = new EventTimerAsActor(
				[this, afterimage, time]() {
					(*time)++;
					int mode, pal;
					this->GetSpr()->GetBlend(&mode, &pal);
					afterimage->SetBlendPal((std::max)(0, pal * (10 - *time) / 10));
					if (*time > 10)
					{
						Sprite::Destroy(afterimage);
						return false;
					}

					return true;
				}
			, 100);
			afterimage->SetLinkAlive(mSpr);
			created->GetSpr()->SetLinkAlive(mSpr);
			return true;
			}, 200);
		spliter->GetSpr()->SetLinkAlive(mSpr);
	}

	void Spirit::resetAccel()
	{
		int accDeg = Rand->Get(360);
		double accRad = accDeg * M_PI / 180;
		mAccel = useful::Vec2<double>{ std::cos(accRad), std::sin(accRad) } * velMax;
	}

	void Spirit::update()
	{
		Actor::update();
		updateSpirit(Time::DeltaMilli());
	}

	void Spirit::updateSpirit(int deltaMilli)
	{
		const double blendSpeed = 0.2;
		double blendRad = mAnimTIme * blendSpeed * M_PI / 180;
		mSpr->SetBlendPal(100 - std::cos(blendRad) * 100);
		//mSpr->SetBlendPal(255);
		mSpr->SetRotationRad(blendRad);
		mSpr->SetXY(mPt.X + mSprOriginPt.X, mPt.Y + mSprOriginPt.Y);

		mVel = mVel +mAccel * (deltaMilli / 1000.0);
		if (useful::Distance(mVel.X, mVel.Y) > velMax)
		{
			mVel = mVel * 0.9;
			resetAccel();
		}

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
			mLifeCount--;
			if (mLifeCount <= 0)
			{
				Sprite::Destroy(mSpr);
				return;
			}
			mAnimTIme = 0;
			mPt = useful::Vec2<double>{ (double)Rand->Get(GRID_WIDTH), (double)Rand->Get(GRID_HEIGHT) };
			mPt.X -= ScrollManager::Sole->GetX();
			mPt.Y -= ScrollManager::Sole->GetY();
		}

		mAnimTIme += deltaMilli;
	}

}

