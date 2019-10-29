#include "Task5.h"

#include<random>
#include<Windows.h>
#include<iostream>

#define MATRIX_SIZE 1000

void Task5::doTask()
{
	int** M1 = new int* [MATRIX_SIZE];
	for (int i = 0; i < MATRIX_SIZE; i++)
		M1[i] = new int[MATRIX_SIZE];
	genSquareMatrix(M1, MATRIX_SIZE);

	int** M2 = new int* [MATRIX_SIZE];
	for (int i = 0; i < MATRIX_SIZE; i++)
		M2[i] = new int[MATRIX_SIZE];
	genSquareMatrix(M2, MATRIX_SIZE);

	int** M3 = new int* [MATRIX_SIZE];
	for (int i = 0; i < MATRIX_SIZE; i++)
		M3[i] = new int[MATRIX_SIZE];

	DWORD startObj = GetTickCount();
	multiplyObj(M1, M2, M3, MATRIX_SIZE);
	DWORD durationObj = GetTickCount() - startObj;

	DWORD startPtr = GetTickCount();
	multiplyPtr(M1, M2, M3, MATRIX_SIZE);
	DWORD durationPtr = GetTickCount() - startPtr;

	std::cout << "Object multiplication took " << durationObj << "ms" << std::endl;
	std::cout << "Pointer multiplication took " << durationPtr << "ms" << std::endl;
	std::cout << std::endl;
}

void Task5::multiplyObj(int** M1, int** M2, int** R, int n)
{
	for (int i = 0; i < n; i++)
		for (int j = 0; j < n; j++)
		{
			int val = 0;
			for (int k = 0; k < n; k++)
				val += M1[i][k] * M2[k][j];
			R[i][j] = val;
		}
}

void Task5::multiplyPtr(int** M1, int** M2, int** R, int n)
{
	for (int i = 0; i < n; i++)
		for (int j = 0; j < n; j++)
		{
			int val = 0;
			for (int k = 0; k < n; k++)
				val += *(*(M1 + i) + k) * *(*(M2 + k) + j);
			*(*(R + i)) = val;
		}
}

void Task5::genSquareMatrix(int** M, int n)
{
	for (int i = 0; i < n; i++)
		for (int j = 0; j < n; j++)
			M[i][j] = rand();
}
