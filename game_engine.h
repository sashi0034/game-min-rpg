#pragma once
#include "sprite.h"
#include "game_utils.h"
#include <chrono>

namespace gameEngine
{

	/// <summary>
	/// 時間を管理するシングルトンクラス
	/// ループ前にRestartを呼び、ループ回にUpdateを呼ぶこと
	/// </summary>
	class Time : public gameUtils::Singleton<Time>
	{
		std::chrono::system_clock::time_point mOldTime;
		int mDeletaMili{};
		double mDeletaSec{};
	public:
		Time();
		int GetDeltaMilli();
		double GetDeletaSec();
		void Restart();
		void Update();
		static int DeltaMilli();
		static double DeltaSec();
	};

	/// <summary>
	/// ゲームオブジェクト基底
	/// </summary>
	class Actor
	{
	protected:
		Sprite* mSpr;
		long long mTime = 0;
	public:
		Sprite* GetSpr();
		double GetTime();
	protected:
		virtual void update();
		virtual void destructor();
	private:
		static void callUpdate(Sprite* hSpr);
		static void callDestructor(Sprite* hSpr);
	public:
		Actor();
	};

	
	/// <summary>
	/// 手動描画するゲームオブジェクトの基底
	/// </summary>
	class SelfDrawingActor : public Actor
	{
	protected:
		virtual void drawing(int hX, int hY) = 0;
	private:
		static void callDrawing(Sprite* hSpr, int hX, int hY);
	public:
		SelfDrawingActor();
	};



	namespace collider
	{
		enum class EType
		{
			RECTANGLE = 0,
		};

		class Shape
		{
		public:
			virtual inline EType Type() = 0;
		};

		class Rectangle : public Shape
		{
		public:
			EType Type() override;
			int ColX;
			int ColY;
			int ColWidth;
			int ColHeight;
			Rectangle(int colX, int colY, int colWidth, int colHeight);
		};
	}

	/// <summary>
	/// 当たり判定付きゲームオブジェクトの基底
	/// </summary>
	class CollideActor : public Actor
	{
	private:
		static std::vector<CollideActor*> colliders;
	public:
		static std::vector<CollideActor*> GetColliders();
	protected:
		UINT mColbit;
	private:
		collider::Shape* mShape;
	public:
		auto GetColbit();
		auto GetShape();
		CollideActor(collider::Shape* shape, UINT colbit);

		CollideActor* GetHit();
		bool GetHitWith(CollideActor* actor);
	protected:
		void destructor() override;
	};

	

	namespace hit
	{
		bool CheckRectRect(collider::Rectangle* rect1, collider::Rectangle* rect2);
		bool CheckRectRect(double x1, double y1, int width1, int height1, double x2, double y2, int width2, int height2);

		CollideActor* GetHitRect(double x, double y, int width, int height, UINT colbit);
		std::pair<CollideActor*, int> GetHitRectFromIndex(double x, double y, int width, int height, UINT colbit,int firstIndex);
		bool GetHitRectWith(double x, double y, int width, int height, UINT colbit, CollideActor* col);
		std::vector<CollideActor*> GetHitRectAll(double x, double y, int width, int height, UINT colbit);
		CollideActor* GetHitCollideActor(CollideActor* actor);

	}
};


