#include "Task4.h"

#include<iostream>
#include<Windows.h>
#include<random>
#include<omp.h>

#define ARR_LENGTH_1 100000
#define ARR_LENGTH_2 200000
#define ARR_LENGTH_3 300000

#define TIME_SPAN_BORDER_MS 2000

void Task4::doTask()
{
	int lens[3]{ ARR_LENGTH_1, ARR_LENGTH_2, ARR_LENGTH_3 };
	double timeAbs[3] = { 0 };
	double cyclesCount[3] = { 0 };


	for (int i = 0; i < 3; i++)
	{
		int* arr = new int[lens[i]];
		genArr(arr, lens[i]);

		timeAbs[i] = sum_abs(arr, lens[i]);
		cyclesCount[i] = sum_ratio(arr, lens[i]);

		std::cout << "arr of length of " << lens[i] << " was summed in " << timeAbs[i] << " seconds" << std::endl;
		std::cout << "array of length of " << lens[i] << " was summed for " << cyclesCount[i] << " times in period of " << TIME_SPAN_BORDER_MS << "ms" << std::endl;
		std::cout << std::endl;
	}

	std::cout << "T_ABS(" << ARR_LENGTH_2 << ") / T_ABS(" << ARR_LENGTH_1 << ") = " << timeAbs[1] / timeAbs[0] << std::endl;
	std::cout << "T_ABS(" << ARR_LENGTH_3 << ") / T_ABS(" << ARR_LENGTH_1 << ") = " << timeAbs[2] / timeAbs[0] << std::endl;

	std::cout << std::endl;

	std::cout << "T_CYCLE(" << ARR_LENGTH_2 << ") / T_CYCLE(" << ARR_LENGTH_1 << ") = " << cyclesCount[1] / cyclesCount[0] << std::endl;
	std::cout << "T_CYCLE(" << ARR_LENGTH_3 << ") / T_CYCLE(" << ARR_LENGTH_1 << ") = " << cyclesCount[2] / cyclesCount[0] << std::endl;

	std::cout << std::endl;
}

llong Task4::sum(int* arr, int n)
{
	int res = 0;
	for (int i = 0; i < n; i++)
		res += arr[i];
	return res;
}

void Task4::genArr(int* arr, int n)
{
	for (int i = 0; i < n; i++)
		arr[i] = rand();
}

double Task4::sum_abs(int* arr, int n)
{
	double start = omp_get_wtime(); 
	sum(arr, n);
	return omp_get_wtime() - start; //seconds
}

int Task4::sum_ratio(int* arr, int n)
{
	DWORD start = GetTickCount();

	int count = 0;
	while (GetTickCount() - start < TIME_SPAN_BORDER_MS)
	{
		sum(arr, n);
		++count;
	}

	return count;
}