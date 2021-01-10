#include <omp.h>
#include <math.h>
#include <intrin.h>
#include <iostream>
#include <malloc.h>

#define PROC_TYPE_COUNT 9

#define TASK2_ARR_LENGTH 80//4096 * 4096

#define TASK2_MIN_RAND -10
#define TASK2_MAX_RAND 10

#define TASK3_ARR_LENGTH 4096 * 4096

#define TASK3_MIN_RAND 0
#define TASK3_MAX_RAND 10

#define TASK4_ARR_LENGTH 4096 * 1024

#define TASK4_MIN_RAND -10
#define TASK4_MAX_RAND 10

#define TIME_ASSESS_FUNC omp_get_wtime
#define TIME_ASSESS_TYPE double

#define PROCESS_WITH_ASSESSMENT(func, name) \
TIME_ASSESS_TYPE start = TIME_ASSESS_FUNC();\
func;\
cout << "Processing " << name << " took " << TIME_ASSESS_FUNC() - start << " s.\n";

using namespace std;

char** names = new char* [PROC_TYPE_COUNT] 
{
	(char*)"AVX 512",
	(char*)"AVX 2" ,
	(char*)"AVX",
	(char*)"SSE 4.2",
	(char*)"SSE 4.1",
	(char*)"SSSE 3",
	(char*)"SSE 3",
	(char*)"SSE 2",
	(char*)"SSE"
};

int* procreg = new int[PROC_TYPE_COUNT * 3] 
{
	7, 1, 16,
	7, 1, 5,
	1, 2, 28,
	1, 2, 20,
	1, 2, 19,
	1, 2, 9, 
	1, 2, 0,
	1, 3, 26,
	1, 3, 25
};

void getreg(int func, int* reg)
{
	__cpuidex(reg, func, 0);
}

bool check(int func, int reg, int bit)
{
	int* data = new int[4];
	getreg(func, data);
	return (data[reg] >> bit) & 1;
}

void task1()
{
	cout << "\n<TASK1>\n\n";

	int* procdata = new int[4];
	getreg(0, procdata);

	cout << "max func number is " << procdata[0] << "\n";
	cout << "proc type is " << (char)procdata[1] << (char)procdata[2] << (char)procdata[3] << "\n";

	cout << "\n********************\n\n";


	for (int i = 0; i < PROC_TYPE_COUNT; i++)
		if (!check(procreg[i * 3], procreg[i * 3 + 1], procreg[i * 3 + 2]))
			cout << names[i] << " is not supported\n";
		else
		{
			cout << names[i] << " is supported\n";
			//break;
		}

	cout << "\n********************\n\n";

	cout << "HYPER Threading is" << (check(1, 3, 28) ? "" : " not") << " supported\n";

	int* proctype = new int[12];

	getreg(0x80000002, proctype);
	getreg(0x80000003, proctype + 4);
	getreg(0x80000004, proctype + 8);

	cout << (char*)proctype << endl;

	cout << "\n</TASK1>\n\n";
}

__m256 l_mm256_sqrt_ps(__m256 a) { return _mm256_sqrt_ps(a); }
__m256d l_mm256_sqrt_pd(__m256d a) { return _mm256_sqrt_pd(a); }

__m256i l_mm256_add_epi8(__m256i a, __m256i b) { return _mm256_add_epi8(a, b); }
__m256i l_mm256_add_epi16(__m256i a, __m256i b) { return _mm256_add_epi16(a, b); }
__m256i l_mm256_add_epi32(__m256i a, __m256i b) { return _mm256_add_epi32(a, b); }
__m256 l_mm256_add_ps(__m256 a, __m256 b) { return _mm256_add_ps(a, b); }
__m256d l_mm256_add_pd(__m256d a, __m256d b) { return _mm256_add_pd(a, b); }

__m256i l_mm256_abs_epi8(__m256i a) { return _mm256_abs_epi8(a); }
__m256i l_mm256_abs_epi16(__m256i a) { return _mm256_abs_epi16(a); }
__m256i l_mm256_abs_epi32(__m256i a) { return _mm256_abs_epi32(a); }
__m256 l_mm256_abs_ps(__m256 a) 
{
	__m256i ai = _mm256_castps_si256(a);
	__m256i zero = _mm256_setzero_si256();
	__m256 uno = _mm256_set1_ps(1);

	__m256i lt = _mm256_cmpgt_epi32(zero, ai);
	__m256i r = _mm256_and_si256(lt, ai);
	__m256i r2 = _mm256_sub_epi32(zero, r);
	__m256i r3 = _mm256_andnot_si256(lt, ai);
	__m256i res = _mm256_add_epi32(r3, r2);
	__m256 rf = _mm256_castsi256_ps(res);

	return rf;
}
__m256d l_mm256_abs_pd(__m256d a) 
{
	__m256d zero = _mm256_setzero_pd();
	__m256d uno = _mm256_set1_pd(1);

	__m256d lt = _mm256_add_pd(_mm256_cmp_pd(a, zero, _CMP_LT_OS), uno);
	__m256d ge = _mm256_sub_pd(_mm256_cmp_pd(a, zero, _CMP_GE_OS), uno);

	return _mm256_mul_pd(a, _mm256_and_pd(lt, ge));
}

char* l_m256_i8(__m256i a) { return a.m256i_i8; }
short* l_m256_i16(__m256i a) { return a.m256i_i16; }
int* l_m256_i32(__m256i a) { return a.m256i_i32; }
float* l_m256_ps(__m256 a) { return a.m256_f32; }
double* l_m256_pd(__m256d a) { return a.m256d_f64; }

__declspec(align(sizeof(__m256i))) char ac[TASK2_ARR_LENGTH];
__declspec(align(sizeof(__m256i))) char bc[TASK2_ARR_LENGTH];
__declspec(align(sizeof(__m256i))) char cc[TASK2_ARR_LENGTH];

__declspec(align(sizeof(__m256i))) short as[TASK2_ARR_LENGTH];
__declspec(align(sizeof(__m256i))) short bs[TASK2_ARR_LENGTH];
__declspec(align(sizeof(__m256i))) short cs[TASK2_ARR_LENGTH];

__declspec(align(sizeof(__m256i))) int ai[TASK2_ARR_LENGTH];
__declspec(align(sizeof(__m256i))) int bi[TASK2_ARR_LENGTH];
__declspec(align(sizeof(__m256i))) int ci[TASK2_ARR_LENGTH];

__declspec(align(sizeof(__m256))) float af[TASK2_ARR_LENGTH];
__declspec(align(sizeof(__m256))) float bf[TASK2_ARR_LENGTH];
__declspec(align(sizeof(__m256))) float cf[TASK2_ARR_LENGTH];

__declspec(align(sizeof(__m256d))) double ad[TASK2_ARR_LENGTH];
__declspec(align(sizeof(__m256d))) double bd[TASK2_ARR_LENGTH];
__declspec(align(sizeof(__m256d))) double cd[TASK2_ARR_LENGTH];

template<typename T>
T randval(T min, T max)
{
	return rand() * (max - min) / RAND_MAX + min;
}

template<typename T>
void randarr(T* arr, int n, T min, T max)
{
	for (int i = 0; i < n; i++)
		arr[i] = randval(min, max);
}

template<typename T, typename R>
bool compare(T* arr, R* simd, int n, int block, T* (*get)(R))
{
	for (int i = 0; i <= n / block; i++)
	{
		T* probe = get(simd[i]);
		for (int j = 0; j < block && i * block + j < n; j++)
			if (probe[j] == NAN && arr[i * block + j] == NAN)
				continue;
			else if (probe[j] != arr[i * block + j])
				return false;
	}
	return true;
}

template<typename T>
void printarr(T* arr, int n)
{
	for (int i = 0; i < n; i++)
		cout << arr[i] << " ";
	cout << "\n";
}

template<typename T, typename R>
void printsimd(R* simd, int n, int block, T*(*get)(R))
{
	for (int i = 0; i <= n / block; i++)
	{
		T* probe = get(simd[i]);
		for (int j = 0; j < block; j++)
			cout << probe[j] << " ";
	}
	cout << "\n";
}

template<typename T>
void sumabs(T* a, T* b, T* c, int n)
{
	for (int i = 0; i < n; i++)
		c[i] = abs(a[i]) + abs(b[i]);
}

template<typename T>
void sumabssimd(T* a, T* b, T* c, int n, int block, T(*add)(T, T), T(*abs)(T))
{
	for (int i = 0; i < n / block; i++)
		c[i] = add(abs(a[i]), abs(b[i]));
}

template<typename T, typename R>
void test2Task(T* a, T* b, T* c, R(*add)(R, R), R(*abs)(R), T*(*get)(R), int block, int n)
{
	randarr<T>(a, n, TASK2_MIN_RAND, TASK2_MAX_RAND);
	randarr<T>(b, n, TASK2_MIN_RAND, TASK2_MAX_RAND);
	memset(c, 0, n * sizeof(T));

	cout << "Started to sum " << typeid(T).name() << " arrays of " << n << " items." << "\n";
	{ PROCESS_WITH_ASSESSMENT(sumabs(a, b, c, n), "SUM_ARR_ABS_SIMPLE"); }

	R* asimd = (R*)(a);
	R* bsimd = (R*)(b);

	T* cs = new T[n];
	memset(cs, 0, n * sizeof(T));
	R* csimd = (R*)cs;

	{ PROCESS_WITH_ASSESSMENT(sumabssimd(asimd, bsimd, csimd, n, block, add, abs), "SUM_ARR_ABS_SIMD"); }

	cout << (compare<T>(c, csimd, n, block, get) ? "NO " : "") << "ERROR\n";
	cout << "\n********************\n\n";
}

void task2()
{	
	cout << "\n<TASK2>\n\n";

	test2Task<char, __m256i>(ac, bc, cc, l_mm256_add_epi8, l_mm256_abs_epi8, l_m256_i8, 32, TASK2_ARR_LENGTH);
	test2Task<short, __m256i>(as, bs, cs, l_mm256_add_epi16, l_mm256_abs_epi16, l_m256_i16, 16, TASK2_ARR_LENGTH);
	test2Task<int, __m256i>(ai, bi, ci, l_mm256_add_epi32, l_mm256_abs_epi32, l_m256_i32, 8, TASK2_ARR_LENGTH);
	test2Task<float, __m256>(af, bf, cf, l_mm256_add_ps, l_mm256_abs_ps , l_m256_ps, 8, TASK2_ARR_LENGTH);
	test2Task<double, __m256d>(ad, bd, cd, l_mm256_add_pd, l_mm256_abs_pd, l_m256_pd, 4, TASK2_ARR_LENGTH);

	cout << "\n</TASK2>\n\n";
}

__declspec(align(sizeof(__m256))) float af2[TASK3_ARR_LENGTH];
__declspec(align(sizeof(__m256))) float bf2[TASK3_ARR_LENGTH];

__declspec(align(sizeof(__m256d))) double ad2[TASK3_ARR_LENGTH];
__declspec(align(sizeof(__m256d))) double bd2[TASK3_ARR_LENGTH];

template<typename T>
void sqrtarr(T* a, T* b, int n)
{
	for (int i = 0; i < n; i++)
		b[i] = sqrt(a[i]);
}

template<typename T>
void sqrtsimd(T* a, T* b, int n, int block, T(*sqrt)(T))
{
	for (int i = 0; i <= n / block; i++)
		b[i] = sqrt(a[i]);
}

template<typename T, typename R>
void test3Task(T* a, T* b, R(*sqrt)(R), T* (*get)(R), int block, int n)
{
	randarr<T>(a, n, TASK3_MIN_RAND, TASK3_MAX_RAND);
	memset(b, 0, n * sizeof(T));

	cout << "Started to sqrt " << typeid(T).name() << " arrays of " << n << " items." << "\n";
	{ PROCESS_WITH_ASSESSMENT(sqrtarr(a, b, n), "SQRT_ARR_SIMPLE"); }

	R* asimd = (R*)(a);

	T* bs = new T[n];
	memset(bs, 0, n * sizeof(T));
	R* bsimd = (R*)bs;

	{ PROCESS_WITH_ASSESSMENT(sqrtsimd(asimd, bsimd, n, block, sqrt), "SQRT_ARR_SIMD"); }

	cout << (compare<T>(b, bsimd, n, block, get) ? "NO " : "") << "ERROR\n";
	cout << "\n********************\n\n";
}

void task3()
{
	cout << "\n<TASK3>\n\n";

	test3Task<float, __m256>(af2, bf2, l_mm256_sqrt_ps, l_m256_ps, 8, TASK3_ARR_LENGTH);
	test3Task<double, __m256d>(ad2, bd2, l_mm256_sqrt_pd, l_m256_pd, 4, TASK3_ARR_LENGTH);

	cout << "\n</TASK3>\n\n";
}

__declspec(align(sizeof(__m256))) typedef struct
{
	float a;
	float b;
}COMPLEX, *PCOMPLEX;

__declspec(align(sizeof(__m256))) COMPLEX acomp[TASK4_ARR_LENGTH];
__declspec(align(sizeof(__m256))) COMPLEX bcomp[TASK4_ARR_LENGTH];
__declspec(align(sizeof(__m256))) COMPLEX ccomp[TASK4_ARR_LENGTH];

void printcomplex(PCOMPLEX a, int n)
{
	for (int i = 0; i < n; i++)
		cout << a[i].a << " + " << a[i].b << " * i\n";
	cout << endl;
}

void randcomplex(PCOMPLEX a, int n, float min, float max)
{
	for (int i = 0; i < n; i++)
	{
		a[i].a = randval<float>(min, max);
		a[i].b = randval<float>(min, max);
	}
}

bool comparecompl(PCOMPLEX a, __m256* b, int n, float* (*get)(__m256))
{
	for (int i = 0; i < n; i++)
		if (a[i].a != b[i].m256_f32[0] || a[i].b != b[i].m256_f32[1])
			return false;
	return true;
}

void multcomplarr(PCOMPLEX a, PCOMPLEX b, PCOMPLEX c, int n)
{
	for (int i = 0; i < n; i++)
	{
		c[i].a = a[i].a * b[i].a - a[i].b * b[i].b;
		c[i].b = a[i].b * b[i].a + a[i].a * b[i].b;
	}
}

void devcomplarr(PCOMPLEX a, PCOMPLEX b, PCOMPLEX c, int n)
{
	for (int i = 0; i < n; i++)
	{
		float div = b[i].a * b[i].a + b[i].b * b[i].b;
		c[i].a = (a[i].a * b[i].a + a[i].b * b[i].b) / div;
		c[i].b = (a[i].b * b[i].a - a[i].a * b[i].b) / div;
	}
}

void multiplycomplsimd(__m256* a, __m256* b, __m256* c, int n)
{
	for (int i = 0; i < n; i++)
	{
		__m256 ma = _mm256_mul_ps(a[i], b[i]);
		__m256 mb = _mm256_mul_ps(_mm256_shuffle_ps(a[i], a[i], _MM_SHUFFLE(2, 2, 0, 1)), b[i]);
		__m256 cr = _mm256_shuffle_ps(_mm256_hsub_ps(ma, ma), _mm256_hadd_ps(mb, mb), _MM_SHUFFLE(1, 0, 1, 0));
		c[i] = _mm256_shuffle_ps(cr, cr, _MM_SHUFFLE(3, 3, 2, 0));
	}
}

void devidecomplsimd(__m256* a, __m256* b, __m256* c, int n)
{
	for (int i = 0; i < n; i++)
	{
		__m256 div = _mm256_mul_ps(b[i], b[i]);
		div = _mm256_hadd_ps(div, div);
		div = _mm256_shuffle_ps(div, div, _MM_SHUFFLE(0, 0, 0, 0));

		__m256 ma = _mm256_mul_ps(a[i], b[i]);
		__m256 mb = _mm256_mul_ps(_mm256_shuffle_ps(a[i], a[i], _MM_SHUFFLE(2, 2, 0, 1)), b[i]);

		__m256 res = _mm256_div_ps(_mm256_shuffle_ps(_mm256_hadd_ps(ma, ma), _mm256_hsub_ps(mb, mb), _MM_SHUFFLE(1, 0, 1, 0)), div);
		c[i] = _mm256_shuffle_ps(res, res, _MM_SHUFFLE(1, 1, 2, 0));
	} 
}

void test4Task(PCOMPLEX a, PCOMPLEX b, PCOMPLEX c, int n, int block, 
			 void(*opsmpl)(PCOMPLEX, PCOMPLEX, PCOMPLEX, int), 
			 void(*opsimd)(__m256*, __m256*, __m256*, int), 
			 const char* name)
{
	randcomplex(a, n, TASK4_MIN_RAND, TASK4_MAX_RAND);
	randcomplex(b, n, TASK4_MIN_RAND, TASK4_MAX_RAND);
	memset(c, 0, n * sizeof(COMPLEX));

	{ PROCESS_WITH_ASSESSMENT(opsmpl(a, b, c, n), "COMPLEX_ARR_SIMPLE(" << name << ")"); }

	__m256* asimd = (__m256*)a;
	__m256* bsimd = (__m256*)b;

	PCOMPLEX cs = new COMPLEX[n];
	memset(cs, 0, n * sizeof(COMPLEX));
	__m256* csimd = (__m256*)cs;

	{ PROCESS_WITH_ASSESSMENT(opsimd(asimd, bsimd, csimd, n), "COMPLEX_ARR_SIMD(" << name << ")"); }

	cout << (comparecompl(c, csimd, n, l_m256_ps) ? "NO " : "") << "ERROR\n";
	cout << "\n********************\n\n";
}

void task4()
{
	cout << "\n<TASK4>\n\n";

	test4Task(acomp, bcomp, ccomp, TASK4_ARR_LENGTH, 8, multcomplarr, multiplycomplsimd, "mult");
	test4Task(acomp, bcomp, ccomp, TASK4_ARR_LENGTH, 8, devcomplarr, devidecomplsimd, "div");

	cout << "\n</TASK4>\n\n";
}

int main()
{
	task1();
	task2();
	task3();
	task4();
}