#ifndef _RANDOM_HEADER
#define _RANDOM_HEADER

#include "./IntTypes.h"
#include <Windows.h>

namespace Random {
	
	// Algorithm "xor" from p. 4 of Marsaglia, "Xorshift RNGs"
	inline Uint32 xorshift32(Uint32 x) {
		x ^= x << 13;
		x ^= x >> 17;
		x ^= x << 5;
		return x;
	}
	inline Uint32 xorshift32() {
		return Random::xorshift32(static_cast<Uint32>(__rdtsc()));
	}


	// Returns a float between [0-1].
	inline float xorshiftFloat(Uint32 Seed) {
		return static_cast<float>(Random::xorshift32(Seed)) / static_cast<float>(UINT32_MAX);
	}

	// Returns a float between [0-1].
	inline float xorshiftFloat() {
		return static_cast<float>(Random::xorshift32()) / static_cast<float>(UINT32_MAX);
	}
}

#endif