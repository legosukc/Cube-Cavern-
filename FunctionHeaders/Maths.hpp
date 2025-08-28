#ifndef _MATHS_HEADER
#define _MATHS_HEADER

//#include "LuauClasses/Vector3.hpp"

namespace Maths {
	
	constexpr double Pi = 3.141592653589793;

	template<typename T>
	constexpr static inline T ToRadians(T x) {

		constexpr double Radians = Pi / 180.0;
		return x * T(Radians);
	}

	template<typename T>
	constexpr static inline T ToDegrees(T x) {

		constexpr double Degrees = 180.0 / Pi;
		return x * T(Degrees);
	}
}

#endif