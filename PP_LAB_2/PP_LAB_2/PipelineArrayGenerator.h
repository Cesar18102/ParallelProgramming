#pragma once

#pragma once

#include <intrin.h>
#include <time.h>
#include <random>

class PipelineArrayGenerator
{
	public:
		template<typename T, typename P>
		void Cast(T* src, P* dest, int n);
};

template<typename T, typename P>
void PipelineArrayGenerator::Cast(T* src, P* dest, int n)
{
	int size = sizeof(P) / sizeof(T);
	int cnt = n + (n % size == 0 ? 0 : size - n % size);

	for (int i = 0; i < cnt; i++)
		dest[i] = *(P*)(src + i);
}