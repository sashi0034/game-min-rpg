#pragma once

#include <string>
#include <vector>
#include <random>
#include "game_utils.h"

#define OUT_LOG	<< "\n [OUT] " << __FILE__  << " ** " << __FUNCTION__ << " ** " << __LINE__ << "\n" <<
#define ERR_LOG	<< "\n [ERR] " << __FILE__  << " ** " << __FUNCTION__ << " ** " << __LINE__ << "\n" <<

namespace gameUtils
{

	/*
	** template functiones and classes
	*/
	template <typename T>
	class ISingleton
	{
	public:
		static T* Sole;
	};
	template <typename T> T* ISingleton<T>::Sole = nullptr;


	template <typename T>
	class Singleton
	{
	public:
		Singleton()
		{
			if (Sole != nullptr)
			{
				std::cerr ERR_LOG "Singleton object is already exit\n";
				return;
			}
			Sole = static_cast<T*>(this);
		}
		virtual ~Singleton()
		{
			Sole = nullptr;
		}
		static T* Sole;
	};
	template <typename T> T* Singleton<T>::Sole = nullptr;


	namespace useful {
		template<typename T>
		int FindIndex(std::vector<T> *arr, T target)
		{
			auto size = (*arr).size();
			if (size == 0) return -1;
			auto iter = std::find((*arr).begin(), (*arr).end(), target);
			size_t index = std::distance((*arr).begin(), iter);
			if (index == size)
			{
				return -1;
			}
			return index;
		}

		template<typename T>
		int Remove(std::vector<T> *arr, T target)
		{
			int index = useful::FindIndex<T>(arr, target);
			if (index == -1) return -1;
			(*arr).erase((*arr).begin() + index);
			return 0;
		}


		template<typename T>
		struct Vec2
		{
			T X;
			T Y;

			Vec2<T> operator + (Vec2<T> other)
			{
				return Vec2<T>{this->X + other.X, this->Y + other.Y};
			}
			Vec2<T> operator - (Vec2<T> other)
			{
				return Vec2<T>{this->X - other.X, this->Y - other.Y};
			}
			Vec2<T> operator * (int time)
			{
				return Vec2<T>{X* time, Y* time};
			}
			Vec2<T> operator * (double time)
			{
				return Vec2<T>{X* time, Y* time};
			}
			Vec2<T> operator / (int div)
			{
				return Vec2<T>{static_cast<T>(X / div), static_cast<T>(Y / div)};
			}
			Vec2<T> operator / (double div)
			{
				return Vec2<T>{static_cast<T>(X / div), static_cast<T>(Y / div)};
			}
			template<typename U> Vec2<U> EachTo()
			{
				return Vec2<U>{static_cast<U>(this->X), static_cast<U>(this->Y)};
			}
		};

		template<typename T>
		struct Vec3 { T X; T Y; T Z; };
	}

}


