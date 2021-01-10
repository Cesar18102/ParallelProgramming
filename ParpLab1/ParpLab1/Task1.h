#pragma once
#include<Windows.h>

class Task1
{
	public:
		void doTask();
	private:
		void time_t_to_systime(time_t t, LPSYSTEMTIME st);
		void printSystemTime(SYSTEMTIME t);
};

