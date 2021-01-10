#pragma once

#include <time.h>
#include <Windows.h>
#include <intrin.h>
#include <chrono>
#include <omp.h>

class Task2
{
	public :
		void doTask();
	private:
		time_t getTimePrecision();
		double getClockPrecision();
		double getSystemTimeAsFileTimePrecision();
		double getSystemTimePreciseAsFileTimePrecision();
		DWORD getTickCountPrecision();
		int get__rdtscPrecision();
		double getQueryPerformanceCounterPrecision();
		double getChronoPrecision();
		double getOmpPrecision();
};

