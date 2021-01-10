#pragma once
class Task5
{
	public:
		void doTask();
	private:
		template <typename T>
		struct Matrix
		{
			T** data;
			int size;
		};

		template <typename T>
		void multiply(Matrix<T> M1, Matrix<T> M2, Matrix<T> R);

		template <typename T>
		void multiply(T** M1, T** M2, T** R, int n);

		template <typename T>
		void multiplyFast(Matrix<T> M1, Matrix<T> M2, Matrix<T> R);

		template <typename T>
		void multiplyFast(T** M1, T** M2, T** R, int n);

		template<typename T>
		void genSquareMatrix(T** M, int n, T maxVal);

		template<typename T>
		void printSquareMatrix(T** M, int n);

		template<typename T>
		void printSquareMatrix(Matrix<T> M);
};

