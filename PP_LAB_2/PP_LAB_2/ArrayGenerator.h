#pragma once

#include <time.h>
#include <random>

class ArrayGenerator
{
	public:
		template<typename T>
		void UnsortedRandomArray(T min, T max, T* arr, int n);

		template<typename T>
		void SortedRandomArray(T min, T max, T* arr, int n);

	private:
		template<typename T>
		T GetRandom(T min, T max);
};

template<typename T>
void ArrayGenerator::UnsortedRandomArray(T min, T max, T* arr, int n)
{
	srand(time(NULL));

	for (int i = 0; i < n; i++)
		arr[i] = GetRandom(min, max);
}

template<typename T>
void ArrayGenerator::SortedRandomArray(T min, T max, T* arr, int n)
{
	srand(time(NULL));

	for (int i = 0; i < n; i++)
	{
		T item = GetRandom(min, max);

		int j = i;
		for (; j > 0 && item < arr[j - 1]; j--)
			arr[j] = arr[j - 1];

		arr[j] = item;
	}
}

template<typename T>
T ArrayGenerator::GetRandom(T min, T max)
{
	return rand() * (max - min) / RAND_MAX  + min;
}

