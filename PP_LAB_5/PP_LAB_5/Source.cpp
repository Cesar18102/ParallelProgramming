#include <iostream>
#include <bitset>
#include <omp.h>

using namespace std;

#ifdef _OPENMP
#define ompdef
#else
#define ompundef
#endif

#define TIME_ASSESS_FUNC omp_get_wtime
#define TIME_ASSESS_TYPE double

#define PROCESS_WITH_ASSESSMENT(func, name) \
TIME_ASSESS_TYPE start = TIME_ASSESS_FUNC();\
func;\
cout << "Processing " << name << " took " << TIME_ASSESS_FUNC() - start << " s.\n";

#define M_MIN_RAND 0
#define M_MAX_RAND 5

#define M_WIDTH_SUM 1024 * 16
#define M_HEIGHT_SUM 1024 * 16

#define M_WIDTH_SUM_BIT 1024 * 32
#define M_HEIGHT_SUM_BIT 1024 * 32

#define M_LEFT_WIDTH_MULT 1024 * 2
#define M_LEFT_HEIGHT_MULT 1024 * 4//

#define M_RIGHT_WIDTH_MULT 1024 * 3//
#define M_RIGTH_HEIGTH_MULT 1024 * 2

#define M_LEFT_WIDTH_MULT_BIT 1024 * 8
#define M_LEFT_HEIGHT_MULT_BIT 1024 * 9//

#define M_RIGHT_WIDTH_MULT_BIT 1024 * 10//
#define M_RIGTH_HEIGTH_MULT_BIT 1024 * 8

template<typename T>
T rand_val(T min, T max)
{
	return rand() * (max - min) / RAND_MAX + min;
}

template<typename T>
T** rand_matrix(T min, T max, int width, int height)
{
	T** m = new T* [height];
	for (int i = 0; i < height; ++i)
	{
		m[i] = new T[width];
		for (int j = 0; j < width; ++j)
			m[i][j] = rand_val<T>(min, max);
	}
	return m;
}

template<typename T>
void print_matrix(T** m, int width, int height)
{
	for (int i = 0; i < height; i++)
	{
		for (int j = 0; j < width; j++)
			cout << m[i][j] << " ";
		cout << "\n";
	}
}

template<typename T>
bool comp_matrix(T** a, T** b, int width, int height)
{
	for (int i = 0; i < height; i++)
		for (int j = 0; j < width; j++)
			if (a[i][j] != b[i][j])
				return false;
	return true;
}

template<size_t width>
bool comp_matrix_bit(bitset<width>* a, bitset<width>* b, int height)
{
	for (int i = 0; i < height; i++)
		if (!(~a[i] & b[i]).none())
			return false;
	return true;
}

template<size_t width>
bitset<width>* rand_matrix_bit(int height)
{
	bitset<width>* res = new bitset<width>[height];
	for (int i = 0; i < height; i++)
	{
		res[i] = bitset<width>();
		for (int j = 0; j < width; j++)
			res[i].set(j, rand_val<int>(0, 10) >= 5);
	}
	return res;
}

template<size_t width>
void print_matrix_bit(bitset<width>* m, int height)
{
	for (int i = 0; i < height; i++)
	{
		for (int j = 0; j < width; j++)
			cout << m[i][j] << " ";
		cout << "\n";
	}
}

template<typename T>
T** matrix_add(T** a, T** b, int width, int height)
{
	T** res = new T* [height];
	for (int i = 0; i < height; i++)
	{
		res[i] = new T[width];
		for (int j = 0; j < width; j++)
			res[i][j] = a[i][j] + b[i][j];
	}
	return res;
}

template<typename T>
T** matrix_add_parallel(T** a, T** b, int width, int height)
{
	T** res = new T * [height];
	for(int i = 0; i < height; i++)
		res[i] = new T[width];

#pragma omp parallel for
	for (int i = 0; i < height; i++)
#pragma omp parallel for
		for (int j = 0; j < width; j++)
			res[i][j] = a[i][j] + b[i][j];

	return res;
}

template<size_t width>
bitset<width>* matrix_add_bit(bitset<width>* a, bitset<width>* b, int height)
{
	bitset<width>* res = new bitset<width>[height];
	for (int i = 0; i < height; i++)
		res[i] = a[i] | b[i];
	return res;
}

template<size_t width>
bitset<width>* matrix_add_bit_parallel(bitset<width>* a, bitset<width>* b, int height)
{
	bitset<width>* res = new bitset<width>[height];

#pragma omp parallel for
	for (int i = 0; i < height; i++)
		res[i] = a[i] | b[i];

	return res;
}

template<typename T>
T** matrix_mult(T** a, T** b, int height_a, int width_b, int common)
{
	T** res = new T * [height_a];
	for (int i = 0; i < height_a; i++)
	{
		res[i] = new T[width_b];
		memset(res[i], 0, sizeof(T) * width_b);

		for (int k = 0; k < common; k++)
			for (int j = 0; j < width_b; j++)
				res[i][j] += a[i][k] * b[k][j];
	}
	return res;
}

template<typename T>
T** matrix_mult_parallel(T** a, T** b, int height_a, int width_b, int common)
{
	T** res = new T * [height_a];

#pragma omp parallel for
	for (int i = 0; i < height_a; i++)
	{
		res[i] = new T[width_b];
		memset(res[i], 0, sizeof(T) * width_b);

		for (int k = 0; k < common; k++)
#pragma omp parallel for
			for (int j = 0; j < width_b; j++)
				res[i][j] += a[i][k] * b[k][j];
	}
	return res;
}

template<size_t width, size_t height>
bitset<height>* matrix_transp_bit(bitset<width>* a)
{
	bitset<height>* res = new bitset<height>[width];
	for (int i = 0; i < height; i++)
		for (int j = 0; j < width; j++)
			res[j].set(i, a[i][j]);
	return res;
}

template<size_t width_a, size_t width_b>
bitset<width_b>* matrix_mult_bit(bitset<width_a>* a, bitset<width_b>* b, int height_a)
{
	bitset<width_b>* res = new bitset<width_b>[height_a];
	bitset<width_a>* btrans = matrix_transp_bit<width_b, width_a>(b);

	for (int i = 0; i < height_a; i++)
		for (int j = 0; j < width_b; j++)
			res[i][j] = (a[i] & btrans[j]).any();
	return res;
}

template<size_t width, size_t height>
bitset<height>* matrix_transp_bit_parallel(bitset<width>* a)
{
	bitset<height>* res = new bitset<height>[width];

#pragma omp parallel for
	for (int i = 0; i < height; i++)
#pragma omp parallel for
		for (int j = 0; j < width; j++)
			res[j].set(i, a[i][j]);

	return res;
}

template<size_t width_a, size_t width_b>
bitset<width_b>* matrix_mult_bit_parallel(bitset<width_a>* a, bitset<width_b>* b, int height_a)
{
	bitset<width_b>* res = new bitset<width_b>[height_a];
	bitset<width_a>* btrans = matrix_transp_bit_parallel<width_b, width_a>(b);

#pragma omp parallel for
	for (int i = 0; i < height_a; i++)
#pragma omp parallel for
		for (int j = 0; j < width_b; j++)
			res[i][j] = (a[i] & btrans[j]).any();

	return res;
}

int main()
{
	int** a = rand_matrix<int>(M_MIN_RAND, M_MAX_RAND, M_WIDTH_SUM, M_HEIGHT_SUM);
	int** b = rand_matrix<int>(M_MIN_RAND, M_MAX_RAND, M_WIDTH_SUM, M_HEIGHT_SUM);
	int** c, ** cp;

	cout << "TESTING SUM OF " << M_WIDTH_SUM << "x" << M_HEIGHT_SUM << " MATRIXES\n";

	{ PROCESS_WITH_ASSESSMENT(c = matrix_add(a, b, M_WIDTH_SUM, M_HEIGHT_SUM), "MATRIX SUM"); }
	{ PROCESS_WITH_ASSESSMENT(cp = matrix_add_parallel(a, b, M_WIDTH_SUM, M_HEIGHT_SUM), "MATRIX SUM PARALLEL"); }

	cout << (comp_matrix(c, cp, M_WIDTH_SUM, M_HEIGHT_SUM) ? "NO " : "") << "ERROR\n\n";
	cout << "\n********************\n\n";

	cout << "TESTING SUM OF " << M_WIDTH_SUM_BIT << "x" << M_HEIGHT_SUM_BIT << " BIT MATRIXES\n";

	bitset<M_WIDTH_SUM_BIT>* abit = rand_matrix_bit<M_WIDTH_SUM_BIT>(M_HEIGHT_SUM_BIT);
	bitset<M_WIDTH_SUM_BIT>* bbit = rand_matrix_bit<M_WIDTH_SUM_BIT>(M_HEIGHT_SUM_BIT);
	bitset<M_WIDTH_SUM_BIT>* cbit, *cbitp;

	{ PROCESS_WITH_ASSESSMENT(cbit = matrix_add_bit<M_WIDTH_SUM_BIT>(abit, bbit, M_HEIGHT_SUM_BIT), "BIT MATRIX SUM"); }
	{ PROCESS_WITH_ASSESSMENT(cbitp = matrix_add_bit_parallel<M_WIDTH_SUM_BIT>(abit, bbit, M_HEIGHT_SUM_BIT), "BIT MATRIX SUM PARALLEL"); }

	cout << (comp_matrix_bit<M_WIDTH_SUM_BIT>(cbit, cbitp, M_HEIGHT_SUM_BIT) ? "NO " : "") << "ERROR\n\n";
	cout << "\n********************\n\n";

	cout << "TESTING MULT OF " << M_LEFT_WIDTH_MULT << "x" << M_LEFT_HEIGHT_MULT << " AND " << M_RIGHT_WIDTH_MULT << "x" << M_RIGTH_HEIGTH_MULT << " MATRIXES\n";

	int** am = rand_matrix<int>(M_MIN_RAND, M_MAX_RAND, M_LEFT_WIDTH_MULT, M_LEFT_HEIGHT_MULT);
	int** bm = rand_matrix<int>(M_MIN_RAND, M_MAX_RAND, M_RIGHT_WIDTH_MULT, M_RIGTH_HEIGTH_MULT);
	int** cm, ** cmp;

	{ PROCESS_WITH_ASSESSMENT(cm = (matrix_mult<int>(am, bm, M_LEFT_HEIGHT_MULT, M_RIGHT_WIDTH_MULT, M_LEFT_WIDTH_MULT)), "MATRIX MULT"); }
	{ PROCESS_WITH_ASSESSMENT(cmp = (matrix_mult_parallel<int>(am, bm, M_LEFT_HEIGHT_MULT, M_RIGHT_WIDTH_MULT, M_LEFT_WIDTH_MULT)), "MATRIX MULT PARALLEL"); }

	cout << (comp_matrix(cm, cmp, M_RIGHT_WIDTH_MULT, M_LEFT_HEIGHT_MULT) ? "NO " : "") << "ERROR\n\n";
	cout << "\n********************\n\n";

	cout << "TESTING MULT OF " << M_LEFT_WIDTH_MULT_BIT << "x" << M_LEFT_HEIGHT_MULT_BIT << " AND " << M_RIGHT_WIDTH_MULT_BIT << "x" << M_RIGTH_HEIGTH_MULT_BIT << " BIT MATRIXES\n";

	bitset<M_LEFT_WIDTH_MULT_BIT>* abitm = rand_matrix_bit<M_LEFT_WIDTH_MULT_BIT>(M_LEFT_HEIGHT_MULT_BIT);
	bitset<M_RIGHT_WIDTH_MULT_BIT>* bbitm = rand_matrix_bit<M_RIGHT_WIDTH_MULT_BIT>(M_RIGTH_HEIGTH_MULT_BIT);

	bitset<M_RIGHT_WIDTH_MULT_BIT>* cbitm, *cbitmp;

	{ PROCESS_WITH_ASSESSMENT(cbitm = (matrix_mult_bit<M_LEFT_WIDTH_MULT_BIT, M_RIGHT_WIDTH_MULT_BIT>(abitm, bbitm, M_LEFT_HEIGHT_MULT_BIT)), "BIT MATRIX MULT"); }
	{ PROCESS_WITH_ASSESSMENT(cbitmp = (matrix_mult_bit_parallel<M_LEFT_WIDTH_MULT_BIT, M_RIGHT_WIDTH_MULT_BIT>(abitm, bbitm, M_LEFT_HEIGHT_MULT_BIT)), "BIT MATRIX MULT PARALLEL"); }

	cout << (comp_matrix_bit<M_RIGHT_WIDTH_MULT_BIT>(cbitm, cbitmp, M_LEFT_HEIGHT_MULT_BIT) ? "NO " : "") << "ERROR\n\n";
}