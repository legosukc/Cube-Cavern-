#pragma once

#include <xmmintrin.h>
#include <smmintrin.h>
#include <cmath>

namespace LuauClasses {

	class Vector3 {
	public:

		Vector3(float X, float Y, float Z) {
			this->X = X;
			this->Y = Y;
			this->Z = Z;
		}
		Vector3(float One = 0.f) {
			this->X = One;
			this->Y = One;
			this->Z = One;
		}

		float X, Y, Z;

		inline float& operator[](int Index) {
			return reinterpret_cast<float*>(this)[Index];
		}

		inline const float& operator[](int Index) const {
			return reinterpret_cast<const float*>(this)[Index];
		}

		/* Class Functions */

		inline __m128 m128FromVector3() const {
			return _mm_setr_ps(this->X, this->Y, this->Z, 0.f);
		}

		// Returns the magnitude of the Vector3.
		inline float Magnitude() const;

		// Returns a normalized version of the Vector3.
		inline Vector3 Unit() const;

		// Returns the square root of the Vector3.
		inline Vector3 Sqrt() const;

		// Returns the dot product between another Vector3.
		inline float Dot(const Vector3& b) const;


		// Returns a rounded version of the Vector3.
		inline Vector3 Round() const;

		// Returns a rounded-up version of the Vector3.
		inline Vector3 Ceil() const;

		// Returns a rounded-down version of the Vector3.
		inline Vector3 Floor() const;


		// Returns the smallest element from bith Vector3's X, Y and Z coordinates.
		inline Vector3 Min(const Vector3& a) const;

		// Returns the largest element from bith Vector3's X, Y and Z coordinates.
		inline Vector3 Max(const Vector3& a) const;

		// Returns a Vector3 that's within the range of "Min" and "Max".
		inline Vector3 Clamp(const Vector3& Min, const Vector3& Max) const;


		/* Returns a Vector3 interpolated to "Goal".
		Example: If "Alpha" is 0.5, then a Vector inbetween the two Vectors is returned.
		@param Goal The end point.
		@param Alpha Range: [0-1]
		*/
		inline Vector3 Lerp(const Vector3& Goal, float Alpha) const;

		inline Vector3 Cross(const Vector3& b) const;

		inline float AngleBetween(const Vector3& b) const;

		// Returns a projection "a" onto "b".
		Vector3 ProjectOnto(const Vector3& b) const;

		bool InTriangle(const Vector3& Point1, const Vector3& Point2, const Vector3& Point3) const;

	private:

		// A test to see if P1 is on the same side as P2 of a line segment ab
		static inline bool SameSide(const Vector3& p1, const Vector3& p2, const Vector3& a, const Vector3& b);

		static inline Vector3 GenerateTriNormal(const Vector3& Point1, const Vector3& Point2, const Vector3& Point3);
	};


}

// MATH OPERATORS


/*	 ADDITION	*/

inline LuauClasses::Vector3 operator+(const LuauClasses::Vector3& a, const LuauClasses::Vector3& b) {
	return LuauClasses::Vector3(
		a.X + b.X,
		a.Y + b.Y,
		a.Z + b.Z
	);
}

inline void operator+=(LuauClasses::Vector3& a, const LuauClasses::Vector3& b) {
	a = (a + b);
}



/*	SUBTRACTION	 */

inline LuauClasses::Vector3 operator-(const LuauClasses::Vector3& a, const LuauClasses::Vector3& b) {
	return LuauClasses::Vector3(
		a.X - b.X,
		a.Y - b.Y,
		a.Z - b.Z
	);
}

inline LuauClasses::Vector3 operator-(const LuauClasses::Vector3& a) {
	return LuauClasses::Vector3(0.f) - a;
}

inline void operator-=(LuauClasses::Vector3& a, const LuauClasses::Vector3& b) {
	a = (a - b);
}



/*	MULTIPLICATION	*/

inline LuauClasses::Vector3 operator*(const LuauClasses::Vector3& a, const LuauClasses::Vector3& b) {
	return LuauClasses::Vector3(
		a.X * b.X,
		a.Y * b.Y,
		a.Z * b.Z
	);
}

inline LuauClasses::Vector3 operator*(const LuauClasses::Vector3& a, float Scalar) {
	return LuauClasses::Vector3(
		a.X * Scalar,
		a.Y * Scalar,
		a.Z * Scalar
	);
}

inline LuauClasses::Vector3 operator*(float Scalar, const LuauClasses::Vector3& b) {
	return b * Scalar;
}

inline void operator*=(LuauClasses::Vector3& a, const LuauClasses::Vector3& b) {
	a = (a * b);
}

inline void operator*=(LuauClasses::Vector3& a, float Scalar) {
	a = (a * Scalar);
}



/*	 DIVISION	*/

inline LuauClasses::Vector3 operator/(float a, const LuauClasses::Vector3& b) {
	return LuauClasses::Vector3(
		a / b.X,
		a / b.Y,
		a / b.Z
	);
}

inline LuauClasses::Vector3 operator/(const LuauClasses::Vector3& a, const LuauClasses::Vector3& b) {
	return LuauClasses::Vector3(
		a.X / b.X,
		a.Y / b.Y,
		a.Z / b.Z
	);
}

inline LuauClasses::Vector3 operator/(const LuauClasses::Vector3& a, float b) {
	return LuauClasses::Vector3(
		a.X / b,
		a.Y / b,
		a.Z / b
	);
}

inline void operator/=(LuauClasses::Vector3& a, const LuauClasses::Vector3& b) {
	a = (a / b);
}

inline void operator/=(LuauClasses::Vector3& a, float Scalar) {
	a = (a / Scalar);
}



/*	COMPARRISON	 */

inline bool operator==(const LuauClasses::Vector3& a, const LuauClasses::Vector3& b) {
	return a.X == b.X and a.Y == b.Y and a.Z == b.Z;
}

inline bool operator>(const LuauClasses::Vector3& a, const LuauClasses::Vector3& b) {
	return a.X > b.X and a.Y > b.Y and a.Z > b.Z;
}
inline bool operator<(const LuauClasses::Vector3& a, const LuauClasses::Vector3& b) {
	return a.X < b.X and a.Y < b.Y and a.Z < b.Z;
}

inline bool operator>=(const LuauClasses::Vector3& a, const LuauClasses::Vector3& b) {
	return a.X >= b.X and a.Y >= b.Y and a.Z >= b.Z;
}
inline bool operator<=(const LuauClasses::Vector3& a, const LuauClasses::Vector3& b) {
	return a.X <= b.X and a.Y <= b.Y and a.Z <= b.Z;
}



/*	CLASS FUNCTIONS	 */

inline LuauClasses::Vector3 LuauClasses::Vector3::Sqrt() const {
	float SquareRootVector[4];
	_mm_store_ps(SquareRootVector, _mm_sqrt_ps(this->m128FromVector3()));
	return *reinterpret_cast<LuauClasses::Vector3*>(SquareRootVector);
}

inline float LuauClasses::Vector3::Dot(const LuauClasses::Vector3& b) const {
	//const LuauClasses::Vector3 tmp = *this * b;
	//return tmp.X + tmp.Y + tmp.Z;
	return (this->X * b.X) + (this->Y * b.Y) + (this->Z * b.Z);
}


inline float LuauClasses::Vector3::Magnitude() const {
	float Magnitude;
	_mm_store_ss(&Magnitude, _mm_sqrt_ss(_mm_set_ss(this->Dot(*this))));
	return Magnitude;
}

inline LuauClasses::Vector3 LuauClasses::Vector3::Unit() const {
	
	float NormalizedVector[4];
	_mm_store_ps(NormalizedVector,
		_mm_mul_ps(
			this->m128FromVector3(),
			_mm_rsqrt_ps(_mm_set1_ps(this->Dot(*this)))
		)
	);

	//const LuauClasses::Vector3 compare = *this / std::sqrt(this->Dot(*this));
	//std::cout << '\n' << NormalizedVector[0] << ", " << NormalizedVector[1] << ", " << NormalizedVector[2] << '\n';
	//std::cout << compare.X << ", " << compare.Y << ", " << compare.Z << '\n';
	
	//return *reinterpret_cast<LuauClasses::Vector3*>(NormalizedVector);
	return *this / std::sqrt(this->Dot(*this));
}


/*	ROUNDING FUNCTIONS 	*/

inline LuauClasses::Vector3 LuauClasses::Vector3::Round() const {
	float RoundedVector[4];
	_mm_store_ps(RoundedVector, _mm_round_ps(this->m128FromVector3(), _MM_ROUND_TOWARD_ZERO));
	return *reinterpret_cast<LuauClasses::Vector3*>(RoundedVector);
}

inline LuauClasses::Vector3 LuauClasses::Vector3::Ceil() const {
	float CeiledVector[4];
	_mm_store_ps(CeiledVector, _mm_ceil_ps(this->m128FromVector3()));
	return *reinterpret_cast<LuauClasses::Vector3*>(CeiledVector);
}

inline LuauClasses::Vector3 LuauClasses::Vector3::Floor() const {
	float FlooredVector[4];
	_mm_store_ps(FlooredVector, _mm_floor_ps(this->m128FromVector3()));
	return *reinterpret_cast<LuauClasses::Vector3*>(FlooredVector);
}



/*	CLAMPING FUNCTIONS 	*/

inline LuauClasses::Vector3 LuauClasses::Vector3::Min(const LuauClasses::Vector3& Min) const {
	float MinVector[4];
	_mm_store_ps(MinVector, _mm_min_ps(this->m128FromVector3(), Min.m128FromVector3()));
	return *reinterpret_cast<LuauClasses::Vector3*>(MinVector);
	/*
	return LuauClasses::Vector3(
		this->X > Min.X ? Min.X : this->X,
		this->Y > Min.Y ? Min.Y : this->Y,
		this->Z > Min.Z ? Min.Z : this->Z
	);*/
}

inline LuauClasses::Vector3 LuauClasses::Vector3::Max(const LuauClasses::Vector3& Max) const {
	float MaxVector[4];
	_mm_store_ps(MaxVector, _mm_max_ps(this->m128FromVector3(), Max.m128FromVector3()));
	return *reinterpret_cast<LuauClasses::Vector3*>(MaxVector);
	/*return LuauClasses::Vector3(
		this->X < Max.X ? Max.X : this->X,
		this->Y < Max.Y ? Max.Y : this->Y,
		this->Z < Max.Z ? Max.Z : this->Z
	);*/
}

inline LuauClasses::Vector3 LuauClasses::Vector3::Clamp(const LuauClasses::Vector3& Smallest, const LuauClasses::Vector3& Biggest) const {
	return this->Min(Biggest).Max(Smallest);
}


inline LuauClasses::Vector3 LuauClasses::Vector3::Lerp(const LuauClasses::Vector3& Goal, float Alpha) const {
	return *this + (Goal - *this) * Alpha;
}

inline LuauClasses::Vector3 LuauClasses::Vector3::Cross(const LuauClasses::Vector3& b) const {
	
	return LuauClasses::Vector3(
		this->Y * b.Z - b.Y * this->Z,
		this->Z * b.X - b.Z * this->X,
		this->X * b.Y - b.X * this->Y
	);
}

inline float LuauClasses::Vector3::AngleBetween(const LuauClasses::Vector3& b) const {

	//float angle = this->Dot(b) / (this->Magnitude() * b.Magnitude());
	//return angle = std::acosf(angle);
	return std::acosf(this->Dot(b) / (this->Magnitude() * b.Magnitude()));
}

inline LuauClasses::Vector3 LuauClasses::Vector3::ProjectOnto(const LuauClasses::Vector3& b) const {
	const LuauClasses::Vector3 bn = b / b.Magnitude();
	return bn * this->Dot(bn);
}

inline bool LuauClasses::Vector3::SameSide(const Vector3& p1, const Vector3& p2, const Vector3& a, const Vector3& b) {
	return ((b - a).Cross(p1 - a)).Dot((b - a).Cross(p2 - a)) >= 0;
}

inline LuauClasses::Vector3 LuauClasses::Vector3::GenerateTriNormal(const Vector3& Point1, const Vector3& Point2, const Vector3& Point3) {
	return (Point2 - Point1).Cross(Point3 - Point1);
}

bool LuauClasses::Vector3::InTriangle(const LuauClasses::Vector3& Point1, const LuauClasses::Vector3& Point2, const LuauClasses::Vector3& Point3) const {

	// Test to see if it is within an infinite prism that the triangle outlines
	const bool WithinTrianglePrism =
		SameSide(*this, Point1, Point2, Point3) and
		SameSide(*this, Point2, Point1, Point3) and
		SameSide(*this, Point3, Point1, Point2);

	if (not WithinTrianglePrism) {
		return false;
	}

	return this->ProjectOnto(LuauClasses::Vector3::GenerateTriNormal(Point1, Point2, Point3)).Magnitude() == 0;
}