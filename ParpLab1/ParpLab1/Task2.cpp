#include "Task2.h"

#include <iostream>

#define TEST_COUNT 10

void Task2::doTask()
{
	time_t minTime = time_t(0x7FFFFFFF), maxTime = 0;
	double minClock = INT_MAX, maxClock = 0;
	double minSysTimeAsFileTime = INT_MAX, maxSysTimeAsFileTime = 0;
	double minSysTimePreciseAsFileTime = INT_MAX, maxSysTimePreciseAsFileTime = 0;
	DWORD minTickCount = INT_MAX, maxTickCount = 0;
	int min__rdtsc = INT_MAX, max__rdtsc = 0;
	double minQPC = INT_MAX, maxQPC = 0;
	double minChrono = INT_MAX, maxChrono = 0;
	double minOmp = INT_MAX, maxOmp = 0;

	for (int i = 0; i < TEST_COUNT; i++)
	{
		time_t time = getTimePrecision();
		if (time < minTime) minTime = time;
		if (time > maxTime) maxTime = time;

		double clock = getClockPrecision();
		if (clock < minClock) minClock = clock;
		if (clock > maxClock) maxClock = clock;

		double sysTimeAsFileTime = getSystemTimeAsFileTimePrecision() / 10;
		if (sysTimeAsFileTime < minSysTimeAsFileTime) minSysTimeAsFileTime = sysTimeAsFileTime;
		if (sysTimeAsFileTime > maxSysTimeAsFileTime) maxSysTimeAsFileTime = sysTimeAsFileTime;

		double sysTimePreciseAsFileTime = getSystemTimePreciseAsFileTimePrecision() / 10;
		if (sysTimePreciseAsFileTime < minSysTimePreciseAsFileTime) minSysTimePreciseAsFileTime = sysTimePreciseAsFileTime;
		if (sysTimePreciseAsFileTime > maxSysTimePreciseAsFileTime) maxSysTimePreciseAsFileTime = sysTimePreciseAsFileTime;

		DWORD tickCount = getTickCountPrecision();
		if (tickCount < minTickCount) minTickCount = tickCount;
		if (tickCount > maxTickCount) maxTickCount = tickCount;

		int __rdtsc = get__rdtscPrecision();
		if (__rdtsc < min__rdtsc) min__rdtsc = __rdtsc;
		if (__rdtsc > max__rdtsc) max__rdtsc = __rdtsc;

		double QPC = getQueryPerformanceCounterPrecision();
		if (QPC < minQPC) minQPC = QPC;
		if (QPC > maxQPC) maxQPC = QPC;

		double chrono = getChronoPrecision();
		if (chrono < minChrono) minChrono = chrono;
		if (chrono > maxChrono) maxChrono = chrono;

		double omp = getOmpPrecision();
		if (omp < minOmp) minOmp = omp;
		if (omp > maxOmp) maxOmp = omp;

		std::cout << "test " << i + 1 << std::endl;
	}

	std::cout << "time func precision = [" << minTime << ", " << maxTime << "] sec" << std::endl;
	std::cout << "clock func precision = [" << minClock << ", " << maxClock << "] sec" << std::endl;
	std::cout << "getSystemTimeAsFileTime func precision = [" << minSysTimeAsFileTime << ", " << maxSysTimeAsFileTime << "] micro sec" << std::endl;
	std::cout << "getSystemTimePreciseAsFileTime func precision = [" << minSysTimePreciseAsFileTime << ", " << maxSysTimePreciseAsFileTime << "] micro sec" << std::endl;
	std::cout << "getTickCount func precision = [" << minTickCount << ", " << maxTickCount << "] ms" << std::endl;
	std::cout << "__rdtsc func precision = [" << min__rdtsc << ", " << max__rdtsc << "] ticks" << std::endl;
	std::cout << "QueryPerformanceCounter func precision = [" << minQPC << ", " << maxQPC << "] sec" << std::endl;
	std::cout << "chrono func precision = [" << minChrono << ", " << maxChrono << "] nano sec" << std::endl;
	std::cout << "omp_get_wtime func precision = [" << minOmp << ", " << maxOmp << "] sec" << std::endl;

	std::cout << std::endl;
}

time_t Task2::getTimePrecision()
{
	time_t start = time(NULL);
	time_t finish = time(NULL);

	while (start == finish)
		finish = time(NULL);

	return finish - start;
}

double Task2::getClockPrecision()
{
	clock_t start = clock();
	clock_t finish = clock();

	while (start == finish)
		finish = clock();

	return ((double)finish - start) / CLOCKS_PER_SEC;
}

double Task2::getSystemTimeAsFileTimePrecision()
{
	LPFILETIME start = new FILETIME();
	GetSystemTimeAsFileTime(start);

	LPFILETIME finish = new FILETIME();
	GetSystemTimeAsFileTime(finish);

	while(!CompareFileTime(start, finish))
		GetSystemTimeAsFileTime(finish);

	LARGE_INTEGER liStart, liFinish;

	liStart.LowPart = start->dwLowDateTime;
	liFinish.LowPart = finish->dwLowDateTime;

	liStart.HighPart = start->dwHighDateTime;
	liFinish.HighPart = finish->dwHighDateTime;

	return liFinish.QuadPart - liStart.QuadPart;
}

double Task2::getSystemTimePreciseAsFileTimePrecision()
{
	LPFILETIME start = new FILETIME();
	GetSystemTimePreciseAsFileTime(start);

	LPFILETIME finish = new FILETIME();
	GetSystemTimePreciseAsFileTime(finish);

	while (!CompareFileTime(start, finish))
		GetSystemTimePreciseAsFileTime(finish);

	LARGE_INTEGER liStart, liFinish;

	liStart.LowPart = start->dwLowDateTime;
	liFinish.LowPart = finish->dwLowDateTime;

	liStart.HighPart = start->dwHighDateTime;
	liFinish.HighPart = finish->dwHighDateTime;

	return liFinish.QuadPart - liStart.QuadPart;
}

DWORD Task2::getTickCountPrecision()
{
	DWORD start = GetTickCount();
	DWORD finish = GetTickCount();

	while (start == finish)
		finish = GetTickCount();

	return finish - start;
}

int Task2::get__rdtscPrecision()
{
	DWORD start = __rdtsc();
	DWORD finish = __rdtsc();

	while (start == finish)
		finish = __rdtsc();

	return finish - start;
}

double Task2::getQueryPerformanceCounterPrecision()
{
	PLARGE_INTEGER start = new LARGE_INTEGER();
	QueryPerformanceCounter(start);

	PLARGE_INTEGER finish = new LARGE_INTEGER();
	QueryPerformanceCounter(finish);

	while (finish == start)
		QueryPerformanceCounter(finish);

	PLARGE_INTEGER freq = new LARGE_INTEGER();
	QueryPerformanceFrequency(freq);

	return ((double)finish->QuadPart - start->QuadPart) / freq->QuadPart;
}

double Task2::getChronoPrecision()
{
	std::chrono::high_resolution_clock::time_point start = std::chrono::high_resolution_clock::now();
	std::chrono::high_resolution_clock::time_point finish = std::chrono::high_resolution_clock::now();

	while (start == finish)
		finish = std::chrono::high_resolution_clock::now();

	return (finish - start).count();
}

double Task2::getOmpPrecision()
{
	double start = omp_get_wtime();
	double finish = omp_get_wtime();

	while (start == finish)
		finish = omp_get_wtime();

	return finish - start;
}