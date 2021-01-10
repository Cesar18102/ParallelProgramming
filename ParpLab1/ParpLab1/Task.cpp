#include<random>
#include<intrin.h>
#include<Windows.h>
#include<profileapi.h>
#include<iostream>

#define llong long long
#define ulong unsigned long
#define ARR_LENGTH 1000

class Task
{

public :

	void task()
	{
		int* arr1 = new int[ARR_LENGTH];
		genArr(arr1, ARR_LENGTH);

		int* arr2 = new int[ARR_LENGTH];
		genArr(arr2, ARR_LENGTH);

		int* res = new int[ARR_LENGTH];


		ulong time__rdtsc = add__rdtsc(arr1, arr2, res, ARR_LENGTH);
		llong time_QueryPerformanceCounter = add_QRC(arr1, arr2, res, ARR_LENGTH);

		std::cout << "__rdtsc time = " << time__rdtsc << std::endl;
		std::cout << "QueryPerformaceCounter" << time_QueryPerformanceCounter << std::endl;
	}

private:

	void add(int* arr1, int* arr2, int* result, int n)
	{
		for (int i = 0; i < n; i++)
			result[i] = arr1[i] + arr2[i];
	}

	void genArr(int* arr, int n)
	{
		for (int i = 0; i < n; i++)
			arr[i] = rand();
	}

	ulong add__rdtsc(int* arr1, int* arr2, int* result, int n)
	{
		ulong start = __rdtsc();
		add(arr1, arr2, result, ARR_LENGTH);
		return __rdtsc() - start;
	}

	long long add_QRC(int* arr1, int* arr2, int* result, int n)
	{
		PLARGE_INTEGER start = new LARGE_INTEGER();
		QueryPerformanceCounter(start);

		add(arr1, arr2, result, ARR_LENGTH);

		PLARGE_INTEGER end = new LARGE_INTEGER();
		QueryPerformanceCounter(end);

		return end->QuadPart - start->QuadPart;
	}
};