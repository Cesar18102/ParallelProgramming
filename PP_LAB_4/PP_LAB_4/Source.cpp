#include <omp.h>
#include <math.h>
#include <thread>
#include <stdio.h>
#include <iostream>
#include <Windows.h>

using namespace std;

#define M_PI 3.141592653589793

#define CALC_PI_PREC 6
#define CALC_PI_PREC_TASK_4 14

#define TIME_ASSESS_FUNC omp_get_wtime
#define TIME_ASSESS_TYPE double

#define PROCESS_WITH_ASSESSMENT(func, name) \
TIME_ASSESS_TYPE start = TIME_ASSESS_FUNC();\
func;\
cout << "Processing " << name << " took " << TIME_ASSESS_FUNC() - start << " s.\n";

void execTask(int num, void(*t)(void))
{
	cout << "\n*****<TASK_" << num << ">*****\n\n";
	t();
	cout << "\n*****<\TASK_" << num << ">*****\n\n";
}

void task1()
{
#ifdef _OPENMP
	printf("_OPENMP Defined\n");
#else
	printf("_OPENMP UnDefined\n");
#endif
}

int getProcCountWin()
{
	SYSTEM_INFO si;
	GetSystemInfo(&si);
	return si.dwNumberOfProcessors;
}

int getProcCountHDC()
{
	return std::thread::hardware_concurrency();
}

int getProcCountOMP()
{
	return omp_get_max_threads();
}

int getProcCountOMP2()
{
	return omp_get_num_procs();
}

void task2()
{
	cout << "Count of processors retrieved by GetSystemInfo = " << getProcCountWin() << "\n";
	cout << "Count of processors retrieved by omp_get_max_threads = " << getProcCountOMP() << "\n";
	cout << "Count of processors retrieved by omp_get_num_procs = " << getProcCountOMP2() << "\n";
	cout << "Count of processors retrieved by hardware_concurrency = " << getProcCountHDC() << "\n";
}

double getPiLeibniz(double precision, double ethalon, int* iter)
{
	int sign = 1;
	double pi = 0;
	long long i = 1;

	for ( ; abs(pi - ethalon) > precision; i += 2)
	{
		pi += sign * 4.0 / i;
		sign = -sign;
	}

	*iter = (i - 1) / 2;
	return pi;
}

double getPiNilakantha(double precision, double ethalon, int* iter) //best practices
{
	int sign = 1;
	double pi = 3;
	long long i = 2;

	for ( ; abs(pi - ethalon) > precision; i += 2)
	{
		pi += sign * 4.0 / (i * (i + 1) * (i + 2));
		sign = -sign;
	}

	*iter = (i - 2) / 2;
	return pi;
}

double getPiUnityCircle(double precision, double ethalon, int* iter)
{
	int segments = 100;
	int segment_step = 2;

	double scope_calculated = 0.0;

	do
	{
		double x = 0;
		double dx = 1.0 / segments;

		scope_calculated = 0.0;
		for (int i = 0; i < segments; i++)
		{
			scope_calculated += 4.0 * sqrt(1.0 - x * x);
			x += dx;
		}

		(*iter)++;
		scope_calculated *= dx;
		segments *= segment_step;

	} while (abs(ethalon - scope_calculated) > precision);

	return scope_calculated;
}

void task3()
{
	double piCalc;
	int* iter = new int(0);

	*iter = 0;
	{ PROCESS_WITH_ASSESSMENT(piCalc = getPiLeibniz(pow(10.0, -CALC_PI_PREC - 1), M_PI, iter), "CACL PI BY LEIBNIZ"); }
	cout << "PI BY LEIBNIZ = " << piCalc << "; ITERATIONS = " << *iter << "\n\n";

	*iter = 0;
	{ PROCESS_WITH_ASSESSMENT(piCalc = getPiNilakantha(pow(10.0, -CALC_PI_PREC - 1), M_PI, iter), "CACL PI BY NILAKANTHA"); }
	cout << "PI BY NILAKANTHA = " << piCalc << "; ITERATIONS = " << *iter << "\n\n";

	*iter = 0;
	{ PROCESS_WITH_ASSESSMENT(piCalc = getPiUnityCircle(pow(10.0, -CALC_PI_PREC - 1), M_PI, iter), "CACL PI BY UNITY CIRCLE"); }
	cout << "PI BY UNITY CIRCLE = " << piCalc << "; ITERATIONS = " << *iter << "\n\n";
}

struct CALC_INFO {
	int starti;
	int counti;
	double result;
};

DWORD WINAPI getPiNilakanthaThread(void *info)
{
	CALC_INFO *inf = (CALC_INFO*)info;

	double pi = 0;
	int sign = inf->starti % 2 == 0 ? 1 : -1;
	int endi = inf->starti + inf->counti;

	for (int i = inf->starti; i < endi; i++)
	{
		double di = 2.0 * (i + 1);
		pi += sign * 4.0 / (di * (di + 1) * (di + 2));
		sign = -sign;
	}

	inf->result = pi;
	return 0;
}

double getPiNilakanthaParallel(int iter)
{
	int sign = 1;
	double pi = 3;
	int endi = iter * 2 + 2;

#pragma omp parallel for reduction(+ : pi) reduction(* : sign)
	for (long long i = 2; i < endi; i += 2)
	{
		pi += sign * 4.0 / (i * (i + 1) * (i + 2));
		sign *= -1;
	}

	return pi;
}

void task4()
{
	double piCalc;
	int* iter = new int(0);

	*iter = 0;
	{ PROCESS_WITH_ASSESSMENT(piCalc = getPiNilakantha(pow(10.0, -CALC_PI_PREC_TASK_4 - 1), M_PI, iter), "CACL PI BY NILAKANTHA"); }
	cout << "PI BY NILAKANTHA = " << piCalc << "; ITERATIONS = " << *iter << "\n\n";

	TIME_ASSESS_TYPE start = TIME_ASSESS_FUNC();

	int tcount = getProcCountWin() - 1;
	int segmlen = *iter / tcount;
	HANDLE *threads = new HANDLE[tcount];
	CALC_INFO* infos = new CALC_INFO[tcount];

	for (int i = 0; i < tcount - 1; i++)
	{
		infos[i].starti = i * segmlen;
		infos[i].counti = segmlen;

		threads[i] = CreateThread(NULL, NULL, getPiNilakanthaThread, infos + i, NULL, NULL);
	}

	infos[tcount - 1].starti = (tcount - 1) * segmlen;
	infos[tcount - 1].counti = segmlen + (*iter % segmlen);
	threads[tcount - 1] = CreateThread(NULL, NULL, getPiNilakanthaThread, infos + tcount - 1, NULL, NULL);

	WaitForMultipleObjects(tcount, threads, true, INFINITE);

	double pi = 3.0;
	for (int i = 0; i < tcount; i++)
		pi += infos[i].result;

	TIME_ASSESS_TYPE end = TIME_ASSESS_FUNC();
	cout << "PI BY NILAKANTHA IN WIN THREADS = " << pi << "; CALCULATION TOOK " << end - start << " s.\n\n";


	{ PROCESS_WITH_ASSESSMENT(piCalc = getPiNilakanthaParallel(*iter), "CACL PI BY NILAKANTHA IN OMP THREADS"); }
	cout << "PI BY NILAKANTHA IN OMP THREADS = " << piCalc << "\n\n";
}

DWORD WINAPI getPiLeiblizThread(void* info)
{
	CALC_INFO* inf = (CALC_INFO*)info;

	double pi = 0;
	int sign = inf->starti % 2 == 0 ? 1 : -1;
	int endi = inf->starti + inf->counti;

	for (int i = inf->starti; i < endi; i++)
	{
		pi += sign * 4.0 / (2.0 * i + 1);
		sign = -sign;
	}

	inf->result = pi;
	return 0;
}

double getPiLeiblizParallel(int iter)
{
	int sign = 1;
	double pi = 0;

#pragma omp parallel for reduction(+ : pi) reduction(* : sign)
	for (long long i = 0; i < iter; i++)
	{
		pi += sign * 4.0 / (2 * i + 1);
		sign *= -1;
	}

	return pi;
}

void task5()
{
	double piCalc;
	int* iter = new int(0);

	*iter = 0;
	{ PROCESS_WITH_ASSESSMENT(piCalc = getPiLeibniz(pow(10.0, -CALC_PI_PREC - 1), M_PI, iter), "CACL PI BY LEIBNIZ"); }
	cout << "PI BY LEIBNIZ = " << piCalc << "; ITERATIONS = " << *iter << "\n\n";

	TIME_ASSESS_TYPE start = TIME_ASSESS_FUNC();

	int tcount = getProcCountWin() - 1;
	int segmlen = *iter / tcount;
	HANDLE* threads = new HANDLE[tcount];
	CALC_INFO* infos = new CALC_INFO[tcount];

	for (int i = 0; i < tcount - 1; i++)
	{
		infos[i].starti = i * segmlen;
		infos[i].counti = segmlen;

		threads[i] = CreateThread(NULL, NULL, getPiLeiblizThread, infos + i, NULL, NULL);
	}

	infos[tcount - 1].starti = (tcount - 1) * segmlen;
	infos[tcount - 1].counti = segmlen + (*iter % segmlen);
	threads[tcount - 1] = CreateThread(NULL, NULL, getPiLeiblizThread, infos + tcount - 1, NULL, NULL);

	WaitForMultipleObjects(tcount, threads, true, INFINITE);

	double pi = 0;
	for (int i = 0; i < tcount; i++)
		pi += infos[i].result;

	TIME_ASSESS_TYPE end = TIME_ASSESS_FUNC();
	cout << "PI BY LEIBNIZ IN WIN THREADS = " << pi << "; CALCULATION TOOK " << end - start << " s.\n\n";


	{ PROCESS_WITH_ASSESSMENT(piCalc = getPiLeiblizParallel(*iter), "CACL PI BY LEIBNIZ IN OMP THREADS"); }
	cout << "PI BY LEIBNIZ IN OMP THREADS = " << piCalc << "\n\n";
}

int main() 
{
	cout.precision(CALC_PI_PREC + 1);

	execTask(1, task1);
	execTask(2, task2);
	execTask(3, task3);

	cout.precision(CALC_PI_PREC_TASK_4 + 1);
	execTask(4, task4);

	cout.precision(CALC_PI_PREC + 1);
	execTask(5, task5);
}
