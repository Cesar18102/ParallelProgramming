#pragma once

#define llong long long
#define ulong unsigned long

class Task4
{
	public:
		void doTask();

	private:
		llong sum(int* arr, int n);
		void genArr(int* arr, int n);
		double sum_abs(int* arr, int n);
		int sum_ratio(int* arr, int n);
};

