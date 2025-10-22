#ifndef VECTOR2_HEADER
#define VECTOR2_HEADER

#include <xmmintrin.h>
#include <smmintrin.h>

namespace LuauClasses {

	class Vector2 {
	public:
		Vector2(float X, float Y) {
			this->X = X;
			this->Y = Y;
		}
		Vector2(float One = 0.f) {
			this->X = One;
			this->Y = One;
		}

		float X, Y;

		/* Class Functions */

		inline __m128 m128FromVector2() const {
			return _mm_setr_ps(this->X, this->Y, 0.f, 0.f);
		}

		// Returns the magnitude of the Vector2.
		float Magnitude() const;

		// Returns a normalized version of the Vector2.
		Vector2 Unit() const;

		// Returns the square root of the Vector2.
		Vector2 Sqrt() const;

		// Returns the dot product between another Vector2.
		float Dot(const Vector2& b) const;


		// Returns a rounded version of the Vector2.
		Vector2 Round() const;

		// Returns a rounded-up version of the Vector2.
		Vector2 Ceil() const;

		// Returns a rounded-down version of the Vector2.
		Vector2 Floor() const;


		// Returns the smallest element from bith Vector2's X and Y coordinates.
		Vector2 Min(const Vector2& a) const;

		// Returns the largest element from bith Vector2's X and Y coordinates.
		Vector2 Max(const Vector2& a) const;

		// Returns a Vector2 that's within the range of "Min" and "Max".
		Vector2 Clamp(const Vector2& Min, const Vector2& Max) const;


		/* Returns a Vector2 interpolated to "Goal".
		Example: If "Alpha" is 0.5, then a Vector inbetween the two Vectors is returned.
		@param Goal The end point.
		@param Alpha Range: [0-1]
		*/
		Vector2 Lerp(const Vector2& Goal, float Alpha) const;
	};
}

// MATH OPERATORS


/*	 ADDITION	*/

LuauClasses::Vector2 operator+(const LuauClasses::Vector2& a, const LuauClasses::Vector2& b) {
	return LuauClasses::Vector2(
		a.X + b.X,
		a.Y + b.Y
	);
}



/*	SUBTRACTION	 */

LuauClasses::Vector2 operator-(const LuauClasses::Vector2& a, const LuauClasses::Vector2& b) {
	return LuauClasses::Vector2(
		a.X - b.X,
		a.Y - b.Y
	);
}

LuauClasses::Vector2 operator-(const LuauClasses::Vector2& a) {
	return LuauClasses::Vector2(0.f, 0.f) - a;
}



/*	MULTIPLICATION	*/

LuauClasses::Vector2 operator*(const LuauClasses::Vector2& a, const LuauClasses::Vector2& b) {
	return LuauClasses::Vector2(
		a.X * b.X,
		a.Y * b.Y
	);
}

LuauClasses::Vector2 operator*(const LuauClasses::Vector2& a, float Scalar) {
	return LuauClasses::Vector2(
		a.X * Scalar,
		a.Y * Scalar
	);
}

LuauClasses::Vector2 operator*(float Scalar, const LuauClasses::Vector2& b) {
	return b * Scalar;
}



/*	 DIVISION	*/

LuauClasses::Vector2 operator/(float a, const LuauClasses::Vector2& b) {
	return LuauClasses::Vector2(
		a / b.X,
		a / b.Y
	);
}

LuauClasses::Vector2 operator/(const LuauClasses::Vector2& a, const LuauClasses::Vector2& b) {
	return LuauClasses::Vector2(
		a.X / b.X,
		a.Y / b.Y
	);
}

LuauClasses::Vector2 operator/(const LuauClasses::Vector2& a, float b) {
	return LuauClasses::Vector2(
		a.X / b,
		a.Y / b
	);
}


/*	CLASS FUNCTIONS	 */

LuauClasses::Vector2 LuauClasses::Vector2::Sqrt() const {
	float SquareRootVector[4];
	_mm_store_ps(SquareRootVector, _mm_sqrt_ps(this->m128FromVector2()));
	return *reinterpret_cast<LuauClasses::Vector2*>(SquareRootVector);
}

float LuauClasses::Vector2::Dot(const LuauClasses::Vector2& b) const {
	return (this->X * b.X) + (this->Y * b.Y);
}


float LuauClasses::Vector2::Magnitude() const {
	float Magnitude;
	_mm_store_ss(&Magnitude, _mm_sqrt_ss(_mm_set_ss(this->Dot(*this))));
	return Magnitude;
}

LuauClasses::Vector2 LuauClasses::Vector2::Unit() const {
	float NormalizedVector[4];
	_mm_store_ps(NormalizedVector,
		_mm_mul_ps(
			this->m128FromVector2(),
			_mm_rsqrt_ps(_mm_set1_ps(this->Dot(*this)))
		)
	);
	return *reinterpret_cast<LuauClasses::Vector2*>(NormalizedVector); //*this * (1.f / this->Dot(*this).Sqrt());
}


/*	ROUNDING FUNCTIONS 	*/

LuauClasses::Vector2 LuauClasses::Vector2::Round() const {
	float RoundedVector[4];
	_mm_store_ps(RoundedVector, _mm_round_ps(this->m128FromVector2(), _MM_ROUND_NEAREST));
	return *reinterpret_cast<LuauClasses::Vector2*>(RoundedVector);
}

LuauClasses::Vector2 LuauClasses::Vector2::Ceil() const {
	float CeiledVector[4];
	_mm_store_ps(CeiledVector, _mm_ceil_ps(this->m128FromVector2()));
	return *reinterpret_cast<LuauClasses::Vector2*>(CeiledVector);
}

LuauClasses::Vector2 LuauClasses::Vector2::Floor() const {
	float FlooredVector[4];
	_mm_store_ps(FlooredVector, _mm_floor_ps(this->m128FromVector2()));
	return *reinterpret_cast<LuauClasses::Vector2*>(FlooredVector);
}



/*	CLAMPING FUNCTIONS 	*/

LuauClasses::Vector2 LuauClasses::Vector2::Min(const LuauClasses::Vector2& Min) const {
	return LuauClasses::Vector2(
		this->X > Min.X ? Min.X : this->X,
		this->Y > Min.Y ? Min.Y : this->Y
	);
}

LuauClasses::Vector2 LuauClasses::Vector2::Max(const LuauClasses::Vector2& Max) const {
	return LuauClasses::Vector2(
		this->X < Max.X ? Max.X : this->X,
		this->Y < Max.Y ? Max.Y : this->Y
	);
}

LuauClasses::Vector2 LuauClasses::Vector2::Clamp(const LuauClasses::Vector2& Min, const LuauClasses::Vector2& Max) const {
	return this->Min(Min).Max(Max);
}



LuauClasses::Vector2 LuauClasses::Vector2::Lerp(const LuauClasses::Vector2& Goal, float Alpha) const {
	return LuauClasses::Vector2(0.f); //*this + (Goal - *this) * Alpha;
}

#endif