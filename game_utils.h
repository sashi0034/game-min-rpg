#pragma once
#include <DxLib.h>
#include <string>
#include <vector>
#include <random>

#define OUT_LOG	<< "\n [OUT] " << __FILE__  << " ** " << __FUNCTION__ << " ** " << __LINE__ << "\n" <<
#define ERR_LOG	<< "\n [ERR] " << __FILE__  << " ** " << __FUNCTION__ << " ** " << __LINE__ << "\n" <<

namespace gameUtils
{
	class Graph
	{
	private:
		int mHandler;
	public:
		Graph(int handler);
		~Graph();
		int GetHandler();
		void DrawGraph(int x, int y, int u, int v, int width, int height, double scale);
		void DrawGraph(int x, int y, int u, int v, int width, int height, double scale, double rotationRad);
		void DrawGraph(int x, int y, int u, int v, int width, int height, double scale, double rotationRad, bool isFlip);

		bool operator ==(Graph* r);

		static Graph* LoadGraph(const char* path);
		static const Graph* NONE;
	};

	
	namespace useful
	{

		class Random
		{
			std::random_device device;
			std::default_random_engine engine;
			std::uniform_int_distribution<int> distribution;

		public:
			Random(void);
			Random(int seed);
			int Get(int max);
			int Get(int min, int max);
		};


		/*
		** templates
		*/
		template<typename T>
		int FindIndex(std::vector<T> arr, T target)
		{
			auto size = arr.size();
			if (size == 0) return -1;
			auto iter = std::find(arr.begin(), arr.end(), target);
			size_t index = std::distance(arr.begin(), iter);
			if (index == size)
			{
				return -1;
			}
			return index;
		}

		template<typename T>
		int Remove(std::vector<T> arr, T target)
		{
			int index = useful::FindIndex<T>(arr, target);
			if (index == -1) return -1;
			arr.erase(arr.begin() + index);
			return 0;
		}

		template<typename T>
		struct XY { T X; T Y; };

		template<typename T>
		struct XYZ { T X; T Y; T Z; };
	}
}


