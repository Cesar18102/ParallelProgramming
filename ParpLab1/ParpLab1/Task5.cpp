#include "Task5.h"

#include<random>
#include<Windows.h>
#include<iostream>

#define MAX_MATRIX_VALUE 10
#define MATRIX_SIZE 1024
#define MTYPE double

void Task5::doTask()
{
	MTYPE** M1 = new MTYPE * [MATRIX_SIZE];
	for (int i = 0; i < MATRIX_SIZE; i++)
		M1[i] = new MTYPE[MATRIX_SIZE];
	genSquareMatrix<MTYPE>(M1, MATRIX_SIZE, MAX_MATRIX_VALUE);

	MTYPE** M2 = new MTYPE * [MATRIX_SIZE];
	for (int i = 0; i < MATRIX_SIZE; i++)
		M2[i] = new MTYPE[MATRIX_SIZE];
	genSquareMatrix<MTYPE>(M2, MATRIX_SIZE, MAX_MATRIX_VALUE);

	MTYPE** M3 = new MTYPE * [MATRIX_SIZE];
	for (int i = 0; i < MATRIX_SIZE; i++)
		M3[i] = new MTYPE[MATRIX_SIZE];

	Matrix<MTYPE> M1O, M2O, M3O;

	M1O.data = M1;
	M1O.size = MATRIX_SIZE;

	M2O.data = M2;
	M2O.size = MATRIX_SIZE;

	M3O.data = M3;
	M3O.size = MATRIX_SIZE;

	/*std::cout << "Left matrix:" << std::endl;
	printSquareMatrix(M1, MATRIX_SIZE);

	std::cout << std::endl << "Right matrix:" << std::endl;
	printSquareMatrix(M2, MATRIX_SIZE);
	std::cout << std::endl;*/

	DWORD start = GetTickCount();
	multiply(M1, M2, M3, MATRIX_SIZE);
	std::cout << "Pointer multiplication took " << GetTickCount() - start << "ms" << std::endl;

	/*std::cout << std::endl;
	printSquareMatrix(M3, MATRIX_SIZE);
	std::cout << std::endl;*/

	start = GetTickCount();
	multiplyFast(M1, M2, M3, MATRIX_SIZE);
	std::cout << "Pointer fast multiplication took " << GetTickCount() - start << "ms" << std::endl;

	/*std::cout << std::endl;
	printSquareMatrix(M3, MATRIX_SIZE);
	std::cout << std::endl;*/

	start = GetTickCount();
	multiply(M1O, M2O, M3O);
	std::cout << "Object multiplication took " << GetTickCount() - start << "ms" << std::endl;

	/*std::cout << std::endl;
	printSquareMatrix(M3O);
	std::cout << std::endl;*/

	start = GetTickCount();
	multiplyFast(M1O, M2O, M3O);
	std::cout << "Object fast multiplication took " << GetTickCount() - start << "ms" << std::endl;

	/*std::cout << std::endl;
	printSquareMatrix(M3O);
	std::cout << std::endl;*/
}

template<typename T>
void Task5::multiply(Matrix<T> M1, Matrix<T> M2, Matrix<T> R)
{
	for (int i = 0; i < R.size; i++)
		for (int j = 0; j < R.size; j++)
		{
			T val = 0;
			for (int k = 0; k < R.size; k++)
				val += M1.data[i][k] * M2.data[k][j];
			R.data[i][j] = val;
		}
}

template<typename T>
void Task5::multiplyFast(Matrix<T> M1, Matrix<T> M2, Matrix<T> R)
{
	for (int i = 0; i < R.size; i++)
	{
		memset(R.data[i], 0, R.size * sizeof(T));
		for (int k = 0; k < R.size; k++)
			for (int j = 0; j < R.size; j++)
				R.data[i][j] += M1.data[i][k] * M2.data[k][j];
	}
}

template<typename T>
void Task5::multiply(T** M1, T** M2, T** R, int n)
{
	for (int i = 0; i < n; i++)
		for (int j = 0; j < n; j++)
		{
			T val = 0;
			for (int k = 0; k < n; k++)
				val += M1[i][k] * M2[k][j];
			R[i][j] = val;
		}
}

template<typename T>
void Task5::multiplyFast(T** M1, T** M2, T** R, int n)
{
	for (int i = 0; i < n; i++)
	{
		memset(R[i], 0, n * sizeof(T));
		for (int k = 0; k < n; k++)
			for (int j = 0; j < n; j++)
				R[i][j] += M1[i][k] * M2[k][j];
	}
}

template<typename T>
void Task5::genSquareMatrix(T** M, int n, T maxVal)
{
	for (int i = 0; i < n; i++)
		for (int j = 0; j < n; j++)
			M[i][j] = rand();
}

template<typename T>
void Task5::printSquareMatrix(T** M, int n)
{
	for (int i = 0; i < n; i++)
	{
		for (int j = 0; j < n; j++)
			std::cout << M[i][j] << " ";
		std::cout << std::endl;
	}
}

template<typename T>
void Task5::printSquareMatrix(Matrix<T> M)
{
	for (int i = 0; i < M.size; i++)
	{
		for (int j = 0; j < M.size; j++)
			std::cout << M.data[i][j] << " ";
		std::cout << std::endl;
	}
}

