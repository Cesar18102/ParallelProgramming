#include "Task1.h"

#include<time.h>
#include<iostream>

#define TIME_TO_CONVERT 0x7FFFFFFF

void Task1::doTask()
{
	SYSTEMTIME result = SYSTEMTIME();
	time_t tm = time_t(TIME_TO_CONVERT);

	time_t_to_systime(tm, &result);

	std::cout << "32-bit time is going to overflow by ";
	printSystemTime(result);
	std::cout << std::endl;
}

void Task1::time_t_to_systime(time_t t, LPSYSTEMTIME st)
{
	tm time = tm();
	localtime_s(&time, &t);

	st->wSecond = time.tm_sec;
	st->wMinute = time.tm_min;
	st->wHour = time.tm_hour;
	st->wDay = time.tm_mday;
	st->wMonth = time.tm_mon + 1;
	st->wYear = time.tm_year + 1900;
}

void Task1::printSystemTime(SYSTEMTIME t)
{
	std::cout << t.wDay << "." << t.wMonth << "." << t.wYear << " " << t.wHour << ":" << t.wMinute << ":" << t.wSecond << std::endl;
}