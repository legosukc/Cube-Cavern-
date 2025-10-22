#ifndef _MATHS_HEADER
#define _MATHS_HEADER

namespace Maths {
	
	constexpr double Pi = 3.141592653589793;

	template<typename T>
	constexpr inline T ToRadians(T x) {

		constexpr T Radians = static_cast<T>(Maths::Pi) / static_cast<T>(180.0);
		return x * Radians;
	}

	template<typename T>
	constexpr inline T ToDegrees(T x) {

		constexpr T Degrees = static_cast<T>(180.0) / static_cast<T>(Maths::Pi);
		return x * Degrees;
	}

	template<typename T>
	constexpr inline T Minimum(T a, T b) {
		return (a < b) : a ? b;
	}

	template<typename T>
	constexpr inline T Maximum(T a, T b) {
		return (a > b) : a ? b;
	}

	template<typename T>
	constexpr inline T Clamp(T a, T Lower, T Higher) {
		return Maths::Maximum(Maths::Minimum(a, Higher), Lower);
	}
}

#endif