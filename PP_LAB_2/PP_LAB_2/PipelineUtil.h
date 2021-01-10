#pragma once

#include <intrin.h>

class PipelineUtil
{
	public:
		template<typename T>
		int PositiveCount(T* arr, int n);
};

template<typename T>
int PipelineUtil::PositiveCount(T* arr, int n)
{
	__m256i count = _mm256_setzero_si256();
	for (int i = 0; i < n; i += 8)
		count = _mm256_add_epi32(count, _mm256_set_epi32(
			*(int*)(arr + i) >= 0, *(int*)(arr + i + 1) >= 0, *(int*)(arr + i + 2) >= 0, *(int*)(arr + i + 3) >= 0,
			*(int*)(arr + i + 4) >= 0, *(int*)(arr + i + 5) >= 0, *(int*)(arr + i + 6) >= 0, *(int*)(arr + i + 7) >= 0)
		);

	count = _mm256_hadd_epi32(count, count);
	count = _mm256_hadd_epi32(count, count);

	return count.m256i_i32[0] + count.m256i_i32[4];
}