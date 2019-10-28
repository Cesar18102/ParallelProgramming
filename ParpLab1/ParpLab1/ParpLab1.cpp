// ParpLab1.cpp : Этот файл содержит функцию "main". Здесь начинается и заканчивается выполнение программы.
//

#include <iostream>
#include <time.h>
#include <ctime>
#include <cmath>
#include <windows.h>
#include <tchar.h>
#include <omp.h>
#include <strsafe.h>
#include <intrin.h>
#include <chrono>


using namespace std;
using namespace std::chrono;

//   Task 1

//Last date of using 32 bit time: 19.01.2038 03:14:07
void dateOfLastUsing_32_bitTime() {
	time_t* time_format = new time_t(0x7FFFFFFF); 
	tm t_result = tm();

	localtime_s(&t_result, time_format);

	SYSTEMTIME sys_time;

	sys_time.wDay = t_result.tm_mday;
	sys_time.wMonth = t_result.tm_mon + 1;
	sys_time.wYear = 1900 + t_result.tm_year;
	sys_time.wHour = t_result.tm_hour - 2;
	sys_time.wMinute = t_result.tm_min;
	sys_time.wSecond = t_result.tm_sec;

	cout << "Last date of using 32 bit time: "
		<< sys_time.wDay << "." << sys_time.wMonth << "." << sys_time.wYear << " "
		<< sys_time.wHour << ":" << sys_time.wMinute << ":" << sys_time.wSecond << endl;
}
//////////////////////////////////////////////////////////////////////////////////////////////////

// Task 2

//The time() function in C++ returns the current calendar time as an object of type time_t.
double time_h() {
	time_t start, finish;

	start = finish = time(0);

	while (start == finish)
	{
		finish = time(0);
	}

	return finish - start;
}


int main()
{
    std::cout << "Hello World!\n";
}

// Запуск программы: CTRL+F5 или меню "Отладка" > "Запуск без отладки"
// Отладка программы: F5 или меню "Отладка" > "Запустить отладку"

// Советы по началу работы 
//   1. В окне обозревателя решений можно добавлять файлы и управлять ими.
//   2. В окне Team Explorer можно подключиться к системе управления версиями.
//   3. В окне "Выходные данные" можно просматривать выходные данные сборки и другие сообщения.
//   4. В окне "Список ошибок" можно просматривать ошибки.
//   5. Последовательно выберите пункты меню "Проект" > "Добавить новый элемент", чтобы создать файлы кода, или "Проект" > "Добавить существующий элемент", чтобы добавить в проект существующие файлы кода.
//   6. Чтобы снова открыть этот проект позже, выберите пункты меню "Файл" > "Открыть" > "Проект" и выберите SLN-файл.
