
#include "game_utils.h"


namespace gameUtils
{
	Graph::Graph(int handler)
	{
		mHandler = handler;
	}

	Graph::~Graph()
	{
		if (mHandler!=-1) DxLib::DeleteGraph(mHandler);
	}

	bool Graph::operator==(Graph* r)
	{
		return this->mHandler == r->mHandler;
	}


	int Graph::GetHandler()
	{
		return mHandler;
	}

	void Graph::DrawGraph(int x, int y, int u, int v, int width, int height, double scale)
	{
		this->DrawGraph(x, y, u, v, width, height, scale, 0.0);
	}

	void Graph::DrawGraph(int x, int y, int u, int v, int width, int height, double scale, double rotationRad)
	{
		this->DrawGraph(x, y, u, v, width, height, scale, rotationRad, false);
	}

	void Graph::DrawGraph(int x, int y, int u, int v, int width, int height, double scale, double rotationRad, bool isFlip)
	{
		int image = DxLib::DerivationGraph(u, v, width, height, mHandler);

		if (rotationRad == 0)
		{
			DxLib::DrawRotaGraph3(x, y, 0, 0, scale, scale, 0, image, 1, isFlip);
		}
		else
		{
			int x1 = width / 2, y1 = height / 2;
			double cos = std::cos(rotationRad), sin = std::sin(rotationRad);

			double x2 = x1 * cos - y1 * sin, y2 = x1 * sin + y1 * cos;

			double dx = x1 - x2, dy = y1 - y2;

			DxLib::DrawRotaGraph3(x + int(dx * scale), y + int(dy * scale),
				0, 0, // âÒì]íÜêS
				scale, scale,
				rotationRad, image,
				1, isFlip);
		}
		DxLib::DeleteGraph(image);
	}

	Graph* Graph::LoadGraph(const char* path)
	{
		int handler = DxLib::LoadGraph(path);
		return new Graph(handler);
	}

	const Graph* Graph::NONE = new Graph(-1);

	
	namespace useful
	{

		Random::Random() : engine(device()), distribution(0, INT_MAX - 1)
		{
		}
		Random::Random(int seed) : engine(seed), distribution(0, INT_MAX - 1)
		{
		}
		int Random::Get(int max)
		{
			return distribution(engine) % max;
		}
		int Random::Get(int min, int max)
		{
			return min + (distribution(engine) % (max - min));
		}


	}


}

