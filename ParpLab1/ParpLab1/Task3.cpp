#include "Task3.h"

#include<random>
#include<intrin.h>
#include<Windows.h>
#include<profileapi.h>
#include<iostream>

#define ARR_LENGTH 1000

void Task3::doTask()
{
	int* arr = new int[ARR_LENGTH];
	genArr(arr, ARR_LENGTH);

	ulong time__rdtsc = sum__rdtsc(arr, ARR_LENGTH);
	llong time_QueryPerformanceCounter = sum_QRC(arr, ARR_LENGTH);

	std::cout << "__rdtsc time = " << time__rdtsc << std::endl;
	std::cout << "QueryPerformaceCounter = " << time_QueryPerformanceCounter << std::endl;
	std::cout << std::endl;
}

void Task3::printArr(int* arr, int n)
{
	for (int i = 0; i < n; i++)
		std::cout << arr[i] << " ";
	std::cout << std::endl;
}

llong Task3::sum(int* arr, int n)
{
	int res = 0;
	for (int i = 0; i < n; i++)
		res += arr[i];
	return res;
}

void Task3::genArr(int* arr, int n)
{
	for (int i = 0; i < n; i++)
		arr[i] = rand();
}

ulong Task3::sum__rdtsc(int* arr, int n)
{
	ulong start = __rdtsc();
	sum(arr, n);
	return __rdtsc() - start;
}

llong Task3::sum_QRC(int* arr, int n)
{
	PLARGE_INTEGER start = new LARGE_INTEGER();
	QueryPerformanceCounter(start);

	sum(arr, n);
	
	PLARGE_INTEGER end = new LARGE_INTEGER();
	QueryPerformanceCounter(end);

	return end->QuadPart - start->QuadPart;
}	