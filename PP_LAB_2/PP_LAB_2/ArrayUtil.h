#pragma once

#include <math.h>

class ArrayUtil
{
	public:
		template<typename T>
		int PositiveCount(T* arr, int n);

		template<typename T>
		int PositiveCountFast(T* arr, int n);

		template<typename T>
		void BubbleSort(T* arr, int n);

		template<typename T>
		void BubbleSortFast(T* arr, int n);

		template<typename T>
		bool Equals(T* arr1, int n1, T* arr2, int n2);

		template<typename T>
		void MultiplyX(T* arr1, int n1, T* arr2, int n2, T* arr3, int n3);

		template<typename T>
		void MultiplyXFast(T* arr1, int n1, T* arr2, int n2, T* arr3, int n3);

		template<typename T>
		void RoundTrunc(T* arr, int n);

		template<typename T>
		void RoundCeil(T* arr, int n);

		template<typename T>
		void RoundIf(T* arr, int n);

		template<typename T>
		void RoundRound(T* arr, int n);

		template<typename T>
		void RoundConvert(T* arr, int n);
};

template<typename T>
int ArrayUtil::PositiveCount(T* arr, int n)
{
	int posCount = 0;
	for (int i = 0; i < n; ++i)
		if (arr[i] >= 0)
			++posCount;
	return posCount;
}

template<typename T>
int ArrayUtil::PositiveCountFast(T* arr, int n)
{
	int posCount = 0;
	for (int i = 0; i < n; ++i)
		posCount += arr[i] >= 0;
	return posCount;
}

template<typename T>
void ArrayUtil::BubbleSort(T* arr, int n)
{
	for (int i = 0; i < n - 1; ++i)
		for (int j = n - 2; j >= i; --j)
			if (arr[j] > arr[j + 1])
			{
				T temp = arr[j];
				arr[j] = arr[j + 1];
				arr[j + 1] = temp;
			}
}

template<typename T>
void ArrayUtil::BubbleSortFast(T* arr, int n)
{
	for (int i = 0; i < n; ++i)
		for (int j = i + 1; j < n; ++j)
			if (arr[j] < arr[i])
			{
				T temp = arr[i];
				arr[i] = arr[j];
				arr[j] = temp;
			}
}

template<typename T>
bool ArrayUtil::Equals(T* arr1, int n1, T* arr2, int n2)
{
	if (n1 != n2)
		return false;

	for (int i = 0; i < n1; ++i)
		if (arr1[i] != arr2[i])
			return false;

	return true;
}

template<typename T>
void ArrayUtil::MultiplyX(T* arr1, int n1, T* arr2, int n2, T* arr3, int n3)
{
	memset(arr3, 0, n3 * sizeof(T));
	for (int i = 0; i < n1; ++i)
		for (int j = 0; j < n2; ++j)
			arr3[i + j] += arr1[i] * arr2[j];
}

template<typename T>
void ArrayUtil::MultiplyXFast(T* arr1, int n1, T* arr2, int n2, T* arr3, int n3)
{
	memset(arr3, 0, n3 * sizeof(T));
	for (int j = 0; j < n2; ++j)
		if (arr2[j] != 0)
			for (int i = 0; i < n1; ++i)
				arr3[i + j] += arr2[j] == 1 ? arr1[i] : -arr1[i];
}

template<typename T>
void ArrayUtil::RoundTrunc(T* arr, int n)
{
	for (int i = 0; i < n; ++i)
		arr[i] = trunc(arr[i]);
}

template<typename T>
void ArrayUtil::RoundCeil(T* arr, int n)
{
	for (int i = 0; i < n; ++i)
		arr[i] = ceil(arr[i]);
}

template<typename T>
void ArrayUtil::RoundRound(T* arr, int n)
{
	for (int i = 0; i < n; ++i)
		arr[i] = round(arr[i]);
}

template<typename T>
void ArrayUtil::RoundIf(T* arr, int n)
{
	for (int i = 0; i < n; ++i)
		arr[i] = (int)(arr[i] + (arr[i] > 0 ? 0.5 : -0.5));
}

template<typename T>
void ArrayUtil::RoundConvert(T* arr, int n)
{
	for (int i = 0; i < n; ++i)
		arr[i] = (int)arr[i];
}

