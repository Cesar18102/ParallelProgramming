#include "ArrayGenerator.h"
#include "ArrayUtil.h"

#include "PipelineArrayGenerator.h"
#include "PipelineUtil.h"

#include <Windows.h>
#include <iostream>
#include <intrin.h>
#include <omp.h>
#include <map>
#include <list>

#define TIME_MESURE_FUNC omp_get_wtime 
#define TIME_UNIT " seconds."
#define TIME_DATA_TYPE double

#define T1_ARR_LEN 800000
#define T1_ARR_TP int
#define T1_ARR_TP_PP __m256i

#define T2_ARR_LEN 30000
#define T2_ARR_TP int

#define T3_ARR1_LEN 20000
#define T3_ARR2_LEN 20000
#define T3_ARR_TP long long

#define T4_ARR_LEN 9999999
#define T4_ARR1_TYPE float
#define T4_ARR2_TYPE double

using namespace std;

template<typename T>
void printArr(T* arr, int n)
{
	for (int i = 0; i < n; i++)
		cout << arr[i] << " ";
	cout << endl;
}

void printArrPipeline(__m256i* arr, int n)
{
	for (int i = 0; i < n; i++)
		cout << *(int*)(arr + i) << " ";
	cout << endl;
}

ArrayGenerator AG = ArrayGenerator();
ArrayUtil AU = ArrayUtil();

PipelineArrayGenerator PAG = PipelineArrayGenerator();
PipelineUtil PU = PipelineUtil();

void task1()
{
	T1_ARR_TP* arrPos = new _declspec(align(sizeof(__m256i))) T1_ARR_TP[T1_ARR_LEN];
	AG.UnsortedRandomArray<T1_ARR_TP>(0, 10, arrPos, T1_ARR_LEN);

	T1_ARR_TP_PP* arrPosPP = new T1_ARR_TP_PP[T1_ARR_LEN + (T1_ARR_LEN % 8 == 0 ? 0 : 8 - T1_ARR_LEN % 8)];
	PAG.Cast<T1_ARR_TP, T1_ARR_TP_PP>(arrPos, arrPosPP, T1_ARR_LEN);


	T1_ARR_TP* arrNeg = new T1_ARR_TP[T1_ARR_LEN];
	AG.UnsortedRandomArray<T1_ARR_TP>(-100, -1, arrNeg, T1_ARR_LEN);

	T1_ARR_TP_PP* arrNegPP = new T1_ARR_TP_PP[T1_ARR_LEN + (T1_ARR_LEN % 8 == 0 ? 0 : 8 - T1_ARR_LEN % 8)];
	PAG.Cast<T1_ARR_TP, T1_ARR_TP_PP>(arrNeg, arrNegPP, T1_ARR_LEN);


	T1_ARR_TP* arrRand = new T1_ARR_TP[T1_ARR_LEN];
	AG.UnsortedRandomArray<T1_ARR_TP>(-50, 50, arrRand, T1_ARR_LEN);

	T1_ARR_TP_PP* arrRandPP = new T1_ARR_TP_PP[T1_ARR_LEN + (T1_ARR_LEN % 8 == 0 ? 0 : 8 - T1_ARR_LEN % 8)];
	PAG.Cast<T1_ARR_TP, T1_ARR_TP_PP>(arrRand, arrRandPP, T1_ARR_LEN);


	cout << "\n*****SIMPLE*****\n\n";

	TIME_DATA_TYPE startPos = TIME_MESURE_FUNC(); 
	int arrPosPosCount = AU.PositiveCount(arrPos, T1_ARR_LEN);
	TIME_DATA_TYPE durPos = TIME_MESURE_FUNC() - startPos;

	cout << "There are " << arrPosPosCount << " positive items of " << T1_ARR_LEN << "; operation took " << durPos << TIME_UNIT << endl;

	TIME_DATA_TYPE startNeg = TIME_MESURE_FUNC();
	int arrNegPosCount = AU.PositiveCount(arrNeg, T1_ARR_LEN);
	TIME_DATA_TYPE durNeg = TIME_MESURE_FUNC() - startNeg;

	cout << "There are " << arrNegPosCount << " of positive items " << T1_ARR_LEN << "; operation took " << durNeg << TIME_UNIT << endl;

	TIME_DATA_TYPE startRand = TIME_MESURE_FUNC();
	int arrRandPosCount = AU.PositiveCount(arrRand, T1_ARR_LEN);
	TIME_DATA_TYPE durRand = TIME_MESURE_FUNC() - startRand;

	cout << "There are " << arrRandPosCount << " of positive items " << T1_ARR_LEN << "; operation took " << durRand << TIME_UNIT << endl;

	cout << "\n*****FAST*****\n\n";

	TIME_DATA_TYPE startPosFast = TIME_MESURE_FUNC();
	int arrPosPosCountFast = AU.PositiveCountFast(arrPos, T1_ARR_LEN);
	TIME_DATA_TYPE durPosFast = TIME_MESURE_FUNC() - startPosFast;

	cout << "There are " << arrPosPosCountFast << " positive items of " << T1_ARR_LEN << "; Fast operation took " << durPosFast << TIME_UNIT << endl;

	TIME_DATA_TYPE startNegFast = TIME_MESURE_FUNC();
	int arrNegPosCountFast = AU.PositiveCountFast(arrNeg, T1_ARR_LEN);
	TIME_DATA_TYPE durNegFast = TIME_MESURE_FUNC() - startNegFast;

	cout << "There are " << arrNegPosCountFast << " of positive items " << T1_ARR_LEN << "; operation took " << durNegFast << TIME_UNIT << endl;

	TIME_DATA_TYPE startRandFast = TIME_MESURE_FUNC();
	int arrRandPosCountFast = AU.PositiveCountFast(arrRand, T1_ARR_LEN);
	TIME_DATA_TYPE durRandFast = TIME_MESURE_FUNC() - startRandFast;

	cout << "There are " << arrRandPosCountFast << " of positive items " << T1_ARR_LEN << "; operation took " << durRandFast << TIME_UNIT << endl;

	cout << "\n*****SIMD*****\n\n";

	TIME_DATA_TYPE startPosPP = TIME_MESURE_FUNC();
	int arrPosPosCountPP = PU.PositiveCount<T1_ARR_TP_PP>(arrPosPP, T1_ARR_LEN);
	TIME_DATA_TYPE durPosPP = TIME_MESURE_FUNC() - startPosPP;

	cout << "There are " << arrPosPosCountPP << " of positive items " << T1_ARR_LEN << "; operation took " << durPosPP << TIME_UNIT << endl;

	TIME_DATA_TYPE startNegPP = TIME_MESURE_FUNC();
	int arrNegPosCountPP = PU.PositiveCount(arrNegPP, T1_ARR_LEN);
	TIME_DATA_TYPE durNegPP = TIME_MESURE_FUNC() - startNegPP;

	cout << "There are " << arrNegPosCountPP << " of positive items " << T1_ARR_LEN << "; operation took " << durNegPP << TIME_UNIT << endl;

	TIME_DATA_TYPE startRandPP = TIME_MESURE_FUNC();
	int arrRandPosCountPP = PU.PositiveCount(arrRandPP, T1_ARR_LEN);
	TIME_DATA_TYPE durRandPP = TIME_MESURE_FUNC() - startRandPP;

	cout << "There are " << arrRandPosCountPP << " of positive items " << T1_ARR_LEN << "; operation took " << durRandPP << TIME_UNIT << endl;

	cout << endl;
}

void task2()
{
	T2_ARR_TP* arrUnsorted = new T2_ARR_TP[T2_ARR_LEN];
	AG.UnsortedRandomArray(-50, 50, arrUnsorted, T2_ARR_LEN);


	T2_ARR_TP* arrUnsorted2 = new T2_ARR_TP[T2_ARR_LEN];
	memcpy(arrUnsorted2, arrUnsorted, T2_ARR_LEN * sizeof(T2_ARR_LEN));


	T2_ARR_TP* arrPresorted = new T2_ARR_TP[T2_ARR_LEN];
	AG.SortedRandomArray(-50, 50, arrPresorted, T2_ARR_LEN);


	T2_ARR_TP* arrPresorted2 = new T2_ARR_TP[T2_ARR_LEN];
	memcpy(arrPresorted2, arrPresorted, T2_ARR_LEN * sizeof(T2_ARR_LEN));


	TIME_DATA_TYPE unsortedSimpleSortStart = TIME_MESURE_FUNC();
	AU.BubbleSort(arrUnsorted, T2_ARR_LEN);
	TIME_DATA_TYPE unsortedSimpleSortDur = TIME_MESURE_FUNC() - unsortedSimpleSortStart;

	cout << "NOT presorted array of " << T2_ARR_LEN << " item was sorted by simple sort in " << unsortedSimpleSortDur << TIME_UNIT << endl;


	TIME_DATA_TYPE unsortedFastSortStart = TIME_MESURE_FUNC();
	AU.BubbleSortFast(arrUnsorted2, T2_ARR_LEN);
	TIME_DATA_TYPE unsortedFastSortDur = TIME_MESURE_FUNC() - unsortedFastSortStart;

	cout << "NOT presorted array of " << T2_ARR_LEN << " item was sorted by fast sort in " << unsortedFastSortDur << TIME_UNIT << endl;

	if(!AU.Equals(arrUnsorted, T2_ARR_LEN, arrUnsorted2, T2_ARR_LEN))
		cout << "Wrong sort\n";
	cout << endl;

	TIME_DATA_TYPE sortedSimpleSortStart = TIME_MESURE_FUNC();
	AU.BubbleSort(arrPresorted, T2_ARR_LEN);
	TIME_DATA_TYPE sortedSimpleSortDur = TIME_MESURE_FUNC() - sortedSimpleSortStart;

	cout << "Presorted array of " << T2_ARR_LEN << " item was sorted by simple sort in " << sortedSimpleSortDur << TIME_UNIT << endl;


	TIME_DATA_TYPE sortedFastSortStart = TIME_MESURE_FUNC();
	AU.BubbleSortFast(arrPresorted2, T2_ARR_LEN);
	TIME_DATA_TYPE sortedFastSortDur = TIME_MESURE_FUNC() - sortedFastSortStart;

	cout << "Presorted array of " << T2_ARR_LEN << " item was sorted by fast sort in " << sortedFastSortDur << TIME_UNIT << endl;

	if (!AU.Equals(arrPresorted, T2_ARR_LEN, arrPresorted2, T2_ARR_LEN))
		cout << "Wrong sort\n";

	cout << endl;
}

void task3()
{
	T3_ARR_TP* arr1 = new T3_ARR_TP[T3_ARR1_LEN];
	AG.UnsortedRandomArray(-50LL, 50LL, arr1, T3_ARR1_LEN);

	T3_ARR_TP* arr2 = new T3_ARR_TP[T3_ARR2_LEN];
	AG.UnsortedRandomArray(-1LL, 1LL, arr2, T3_ARR2_LEN);

	int res_len = 2 * max(T3_ARR1_LEN, T3_ARR2_LEN);
	T3_ARR_TP* arr3 = new T3_ARR_TP[res_len];
	T3_ARR_TP* arr4 = new T3_ARR_TP[res_len];

	TIME_DATA_TYPE start = TIME_MESURE_FUNC();
	AU.MultiplyX(arr1, T3_ARR1_LEN, arr2, T3_ARR2_LEN, arr3, res_len);
	TIME_DATA_TYPE dur = TIME_MESURE_FUNC() - start;

	cout << "Multiplying polynoms of size " << T3_ARR1_LEN << " and " << T3_ARR2_LEN << " took " << dur << TIME_UNIT << endl;

	TIME_DATA_TYPE start1 = TIME_MESURE_FUNC();
	AU.MultiplyXFast(arr1, T3_ARR1_LEN, arr2, T3_ARR2_LEN, arr4, res_len);
	TIME_DATA_TYPE dur1 = TIME_MESURE_FUNC() - start1;

	cout << "Fast multiplying polynoms of size " << T3_ARR1_LEN << " and " << T3_ARR2_LEN << " took " << dur1 << TIME_UNIT << endl;

	if (!AU.Equals(arr3, res_len, arr4, res_len))
		cout << "Wrong multiply!" << endl;
	cout << endl;
}

void task4()
{
	T4_ARR1_TYPE* arr11 = new T4_ARR1_TYPE[T4_ARR_LEN];
	AG.UnsortedRandomArray(-50.0f, 50.0f, arr11, T4_ARR_LEN);

	T4_ARR1_TYPE* arr12 = new T4_ARR1_TYPE[T4_ARR_LEN];
	memcpy(arr12, arr11, T4_ARR_LEN * sizeof(T4_ARR1_TYPE));

	T4_ARR1_TYPE* arr13 = new T4_ARR1_TYPE[T4_ARR_LEN];
	memcpy(arr13, arr11, T4_ARR_LEN * sizeof(T4_ARR1_TYPE));

	T4_ARR1_TYPE* arr14 = new T4_ARR1_TYPE[T4_ARR_LEN];
	memcpy(arr14, arr11, T4_ARR_LEN * sizeof(T4_ARR1_TYPE));

	T4_ARR1_TYPE* arr15 = new T4_ARR1_TYPE[T4_ARR_LEN];
	memcpy(arr15, arr11, T4_ARR_LEN * sizeof(T4_ARR1_TYPE));


	T4_ARR2_TYPE* arr21 = new T4_ARR2_TYPE[T4_ARR_LEN];
	AG.UnsortedRandomArray(-50.0, 50.0, arr21, T4_ARR_LEN);

	T4_ARR2_TYPE* arr22 = new T4_ARR2_TYPE[T4_ARR_LEN];
	memcpy(arr22, arr21, T4_ARR_LEN * sizeof(T4_ARR2_TYPE));

	T4_ARR2_TYPE* arr23 = new T4_ARR2_TYPE[T4_ARR_LEN];
	memcpy(arr23, arr21, T4_ARR_LEN * sizeof(T4_ARR2_TYPE));

	T4_ARR2_TYPE* arr24 = new T4_ARR2_TYPE[T4_ARR_LEN];
	memcpy(arr24, arr21, T4_ARR_LEN * sizeof(T4_ARR2_TYPE));

	T4_ARR2_TYPE* arr25 = new T4_ARR2_TYPE[T4_ARR_LEN];
	memcpy(arr25, arr21, T4_ARR_LEN * sizeof(T4_ARR2_TYPE));


	TIME_DATA_TYPE start11 = TIME_MESURE_FUNC();
	AU.RoundCeil(arr11, T4_ARR_LEN);
	TIME_DATA_TYPE dur11 = TIME_MESURE_FUNC() - start11;

	cout << "Ceiling " << typeid(T4_ARR1_TYPE).name() << " array of size " << T4_ARR_LEN << " took " << dur11 << TIME_UNIT << endl;

	TIME_DATA_TYPE start21 = TIME_MESURE_FUNC();
	AU.RoundCeil(arr21, T4_ARR_LEN);
	TIME_DATA_TYPE dur21 = TIME_MESURE_FUNC() - start21;

	cout << "Ceiling " << typeid(T4_ARR2_TYPE).name() << " array of size " << T4_ARR_LEN << " took " << dur21 << TIME_UNIT << endl;

	cout << endl;

	TIME_DATA_TYPE start12 = TIME_MESURE_FUNC();
	AU.RoundTrunc(arr12, T4_ARR_LEN);
	TIME_DATA_TYPE dur12 = TIME_MESURE_FUNC() - start12;

	cout << "Truncating " << typeid(T4_ARR1_TYPE).name() << " array of size " << T4_ARR_LEN << " took " << dur12 << TIME_UNIT << endl;

	TIME_DATA_TYPE start22 = TIME_MESURE_FUNC();
	AU.RoundTrunc(arr22, T4_ARR_LEN);
	TIME_DATA_TYPE dur22 = TIME_MESURE_FUNC() - start22;

	cout << "Truncating " << typeid(T4_ARR2_TYPE).name() << " array of size " << T4_ARR_LEN << " took " << dur22 << TIME_UNIT << endl;

	cout << endl;

	TIME_DATA_TYPE start13 = TIME_MESURE_FUNC();
	AU.RoundRound(arr13, T4_ARR_LEN);
	TIME_DATA_TYPE dur13 = TIME_MESURE_FUNC() - start13;

	cout << "Rounding " << typeid(T4_ARR1_TYPE).name() << " array of size " << T4_ARR_LEN << " took " << dur13 << TIME_UNIT << endl;

	TIME_DATA_TYPE start23 = TIME_MESURE_FUNC();
	AU.RoundRound(arr23, T4_ARR_LEN);
	TIME_DATA_TYPE dur23 = TIME_MESURE_FUNC() - start23;

	cout << "Rounding " << typeid(T4_ARR2_TYPE).name() << " array of size " << T4_ARR_LEN << " took " << dur23 << TIME_UNIT << endl;

	cout << endl;

	TIME_DATA_TYPE start14 = TIME_MESURE_FUNC();
	AU.RoundConvert(arr14, T4_ARR_LEN);
	TIME_DATA_TYPE dur14 = TIME_MESURE_FUNC() - start14;

	cout << "Converting " << typeid(T4_ARR1_TYPE).name() << " array of size " << T4_ARR_LEN << " took " << dur14 << TIME_UNIT << endl;

	TIME_DATA_TYPE start24 = TIME_MESURE_FUNC();
	AU.RoundConvert(arr24, T4_ARR_LEN);
	TIME_DATA_TYPE dur24 = TIME_MESURE_FUNC() - start24;

	cout << "Converting " << typeid(T4_ARR2_TYPE).name() << " array of size " << T4_ARR_LEN << " took " << dur24 << TIME_UNIT << endl;

	cout << endl;

	TIME_DATA_TYPE start15 = TIME_MESURE_FUNC();
	AU.RoundIf(arr15, T4_ARR_LEN);
	TIME_DATA_TYPE dur15 = TIME_MESURE_FUNC() - start15;

	cout << "RoundIf " << typeid(T4_ARR1_TYPE).name() << " array of size " << T4_ARR_LEN << " took " << dur15 << TIME_UNIT << endl;

	TIME_DATA_TYPE start25 = TIME_MESURE_FUNC();
	AU.RoundIf(arr25, T4_ARR_LEN);
	TIME_DATA_TYPE dur25 = TIME_MESURE_FUNC() - start25;

	cout << "RoundIf " << typeid(T4_ARR2_TYPE).name() << " array of size " << T4_ARR_LEN << " took " << dur25 << TIME_UNIT << endl;

	if (!AU.Equals(arr13, T4_ARR_LEN, arr15, T4_ARR_LEN))
		cout << "Wrong" << endl;

	if (!AU.Equals(arr23, T4_ARR_LEN, arr25, T4_ARR_LEN))
		cout << "Wrong" << endl;

	cout << endl;
}

int main()
{
	cout << "*****TASK1*****\n\n";
	//task1();

	cout << "*****TASK2*****\n\n";
	//task2();

	cout << "*****TASK3*****\n\n";
	task3();

	cout << "*****TASK4*****\n\n";
	task4();

	cout << "\n\n";
}