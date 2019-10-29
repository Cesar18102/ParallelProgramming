#pragma once

#define llong long long
#define ulong unsigned long

class Task3
{
	public:
		void doTask();

	private:
		void printArr(int* arr, int n);
		llong sum(int* arr, int n);
		void genArr(int* arr, int n);
		ulong sum__rdtsc(int* arr, int n);
		llong sum_QRC(int* arr, int n);
};

