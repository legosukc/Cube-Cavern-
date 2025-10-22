#ifndef VECTOR3_H
#define VECTOR3_H

#include <xmmintrin.h>
#include <smmintrin.h>
#include <cmath>

#include "../IntTypes.h"

#include "../LuaClasses/LuaContext.hpp"
#include "../LuaClasses/LuaGlobalTable.hpp"

extern "C" {
#include <lua-5.4.2/lua.h>
#include <lua-5.4.2/lualib.h>
#include <lua-5.4.2/lauxlib.h>
}

namespace LuauClasses {

	class Vector3 {
	public:

		Vector3(float X, float Y, float Z);
		Vector3(float One = 0.f);

		float X, Y, Z;

		inline float& operator[](int Index) {
			return reinterpret_cast<float*>(this)[Index];
		}

		inline const float& operator[](int Index) const {
			return reinterpret_cast<const float*>(this)[Index];
		}
		

		/*	ADDITION   */

		//inline Vector3 operator+(const Vector3& b) const;
		//inline void operator+=(const Vector3& b);



		/*	SUBTRACTION	 */
		/*
		inline Vector3 operator-(const Vector3& b) const;
		inline Vector3 operator-() const;

		inline void operator-=(const Vector3& b);*/



		/*	MULTIPLICATION	*/
		/*
		inline Vector3 operator*(const Vector3& b) const;
		inline Vector3 operator*(float Scalar) const;

		inline void operator*=(const Vector3& b);
		inline void operator*=(float Scalar);*/



		/*	 DIVISION	*/
		/*
		inline Vector3 operator/(const Vector3& b) const;
		inline Vector3 operator/(float b) const;

		inline void operator/=(const Vector3& b);
		inline void operator/=(float Scalar);*/



		/*	COMPARRISON	 */
		/*
		inline bool operator==(const Vector3& b) const;

		inline bool operator>(const Vector3& b) const;
		inline bool operator<(const Vector3& b) const;

		inline bool operator>=(const Vector3& b) const;
		inline bool operator<=(const Vector3& b) const;*/

		/* Class Functions */

		inline __m128 m128FromVector3() const;

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


	/*	 ADDITION	*/

	inline Vector3 operator+(const LuauClasses::Vector3& a, const LuauClasses::Vector3& b);

	inline void operator+=(LuauClasses::Vector3& a, const LuauClasses::Vector3& b);



	/*	SUBTRACTION	 */

	inline Vector3 operator-(const LuauClasses::Vector3& a, const LuauClasses::Vector3& b);
	inline Vector3 operator-(const LuauClasses::Vector3& a);

	inline void operator-=(LuauClasses::Vector3& a, const LuauClasses::Vector3& b);



	/*	MULTIPLICATION	*/

	inline LuauClasses::Vector3 operator*(const LuauClasses::Vector3& a, const LuauClasses::Vector3& b);
	inline LuauClasses::Vector3 operator*(const LuauClasses::Vector3& a, float Scalar);
	inline LuauClasses::Vector3 operator*(float Scalar, const LuauClasses::Vector3& b);

	inline void operator*=(LuauClasses::Vector3& a, const LuauClasses::Vector3& b);
	inline void operator*=(LuauClasses::Vector3& a, float Scalar);



	/*	 DIVISION	*/

	inline LuauClasses::Vector3 operator/(float a, const LuauClasses::Vector3& b);
	inline LuauClasses::Vector3 operator/(const LuauClasses::Vector3& a, const LuauClasses::Vector3& b);
	inline LuauClasses::Vector3 operator/(const LuauClasses::Vector3& a, float b);

	inline void operator/=(LuauClasses::Vector3& a, const LuauClasses::Vector3& b);
	inline void operator/=(LuauClasses::Vector3& a, float Scalar);



	/*	COMPARRISON	 */

	inline bool operator==(const LuauClasses::Vector3& a, const LuauClasses::Vector3& b);

	inline bool operator>(const LuauClasses::Vector3& a, const LuauClasses::Vector3& b);
	inline bool operator<(const LuauClasses::Vector3& a, const LuauClasses::Vector3& b);

	inline bool operator>=(const LuauClasses::Vector3& a, const LuauClasses::Vector3& b);
	inline bool operator<=(const LuauClasses::Vector3& a, const LuauClasses::Vector3& b);




	// IMPLEMENTATION

	Vector3::Vector3(float X, float Y, float Z) {
		this->X = X;
		this->Y = Y;
		this->Z = Z;
	}

	Vector3::Vector3(float One) {
		this->X = One;
		this->Y = One;
		this->Z = One;
	}

	/* Class Functions */

	__m128 Vector3::m128FromVector3() const {
		return _mm_setr_ps(this->X, this->Y, this->Z, 0.f);
	}

	/*	 ADDITION	*/

	LuauClasses::Vector3 operator+(const LuauClasses::Vector3& a, const LuauClasses::Vector3& b) {
		return LuauClasses::Vector3(
			a.X + b.X,
			a.Y + b.Y,
			a.Z + b.Z
		);
	}

	void operator+=(LuauClasses::Vector3& a, const LuauClasses::Vector3& b) {
		a = (a + b);
	}



	/*	SUBTRACTION	 */

	LuauClasses::Vector3 operator-(const LuauClasses::Vector3& a, const LuauClasses::Vector3& b) {
		return LuauClasses::Vector3(
			a.X - b.X,
			a.Y - b.Y,
			a.Z - b.Z
		);
	}

	LuauClasses::Vector3 operator-(const LuauClasses::Vector3& a) {
		return LuauClasses::Vector3(0.f) - a;
	}

	void operator-=(LuauClasses::Vector3& a, const LuauClasses::Vector3& b) {
		a = (a - b);
	}



	/*	MULTIPLICATION	*/

	LuauClasses::Vector3 operator*(const LuauClasses::Vector3& a, const LuauClasses::Vector3& b) {
		return LuauClasses::Vector3(
			a.X * b.X,
			a.Y * b.Y,
			a.Z * b.Z
		);
	}

	LuauClasses::Vector3 operator*(const LuauClasses::Vector3& a, float Scalar) {
		return LuauClasses::Vector3(
			a.X * Scalar,
			a.Y * Scalar,
			a.Z * Scalar
		);
	}

	LuauClasses::Vector3 operator*(float Scalar, const LuauClasses::Vector3& b) {
		return b * Scalar;
	}

	void operator*=(LuauClasses::Vector3& a, const LuauClasses::Vector3& b) {
		a = (a * b);
	}

	void operator*=(LuauClasses::Vector3& a, float Scalar) {
		a = (a * Scalar);
	}



	/*	 DIVISION	*/

	LuauClasses::Vector3 operator/(float a, const LuauClasses::Vector3& b) {
		return LuauClasses::Vector3(
			a / b.X,
			a / b.Y,
			a / b.Z
		);
	}

	LuauClasses::Vector3 operator/(const LuauClasses::Vector3& a, const LuauClasses::Vector3& b) {
		return LuauClasses::Vector3(
			a.X / b.X,
			a.Y / b.Y,
			a.Z / b.Z
		);
	}

	LuauClasses::Vector3 operator/(const LuauClasses::Vector3& a, float b) {
		return LuauClasses::Vector3(
			a.X / b,
			a.Y / b,
			a.Z / b
		);
	}

	void operator/=(LuauClasses::Vector3& a, const LuauClasses::Vector3& b) {
		a = (a / b);
	}

	void operator/=(LuauClasses::Vector3& a, float Scalar) {
		a = (a / Scalar);
	}



	/*	COMPARRISON	 */

	bool operator==(const LuauClasses::Vector3& a, const LuauClasses::Vector3& b) {
		return a.X == b.X and a.Y == b.Y and a.Z == b.Z;
	}

	bool operator>(const LuauClasses::Vector3& a, const LuauClasses::Vector3& b) {
		return a.X > b.X and a.Y > b.Y and a.Z > b.Z;
	}
	bool operator<(const LuauClasses::Vector3& a, const LuauClasses::Vector3& b) {
		return a.X < b.X and a.Y < b.Y and a.Z < b.Z;
	}

	bool operator>=(const LuauClasses::Vector3& a, const LuauClasses::Vector3& b) {
		return a.X >= b.X and a.Y >= b.Y and a.Z >= b.Z;
	}
	bool operator<=(const LuauClasses::Vector3& a, const LuauClasses::Vector3& b) {
		return a.X <= b.X and a.Y <= b.Y and a.Z <= b.Z;
	}




	/*	CLASS FUNCTIONS	 */

	Vector3 Vector3::Sqrt() const {
		float SquareRootVector[4];
		_mm_store_ps(SquareRootVector, _mm_sqrt_ps(this->m128FromVector3()));
		return *reinterpret_cast<Vector3*>(SquareRootVector);
	}

	float Vector3::Dot(const Vector3& b) const {
		//const Vector3 tmp = *this * b;
		//return tmp.X + tmp.Y + tmp.Z;
		return (this->X * b.X) + (this->Y * b.Y) + (this->Z * b.Z);
	}


	float Vector3::Magnitude() const {
		float Magnitude;
		_mm_store_ss(&Magnitude, _mm_sqrt_ss(_mm_set_ss(this->Dot(*this))));
		return Magnitude;
	}

	Vector3 Vector3::Unit() const {

		float NormalizedVector[4];
		_mm_store_ps(NormalizedVector,
			_mm_mul_ps(
				this->m128FromVector3(),
				_mm_rsqrt_ps(_mm_set1_ps(this->Dot(*this)))
			)
		);

		//const Vector3 compare = *this / std::sqrt(this->Dot(*this));
		//std::cout << '\n' << NormalizedVector[0] << ", " << NormalizedVector[1] << ", " << NormalizedVector[2] << '\n';
		//std::cout << compare.X << ", " << compare.Y << ", " << compare.Z << '\n';

		//return *reinterpret_cast<Vector3*>(NormalizedVector);
		return *this / std::sqrt(this->Dot(*this));
	}


	/*	ROUNDING FUNCTIONS 	*/

	Vector3 Vector3::Round() const {
		float RoundedVector[4];
		_mm_store_ps(RoundedVector, _mm_round_ps(this->m128FromVector3(), _MM_ROUND_TOWARD_ZERO));
		return *reinterpret_cast<Vector3*>(RoundedVector);
	}

	Vector3 Vector3::Ceil() const {
		float CeiledVector[4];
		_mm_store_ps(CeiledVector, _mm_ceil_ps(this->m128FromVector3()));
		return *reinterpret_cast<Vector3*>(CeiledVector);
	}

	Vector3 Vector3::Floor() const {
		float FlooredVector[4];
		_mm_store_ps(FlooredVector, _mm_floor_ps(this->m128FromVector3()));
		return *reinterpret_cast<Vector3*>(FlooredVector);
	}



	/*	CLAMPING FUNCTIONS 	*/

	Vector3 Vector3::Min(const Vector3& Min) const {
		float MinVector[4];
		_mm_store_ps(MinVector, _mm_min_ps(this->m128FromVector3(), Min.m128FromVector3()));
		return *reinterpret_cast<Vector3*>(MinVector);
		/*
		return Vector3(
			this->X > Min.X ? Min.X : this->X,
			this->Y > Min.Y ? Min.Y : this->Y,
			this->Z > Min.Z ? Min.Z : this->Z
		);*/
	}

	Vector3 Vector3::Max(const Vector3& Max) const {
		float MaxVector[4];
		_mm_store_ps(MaxVector, _mm_max_ps(this->m128FromVector3(), Max.m128FromVector3()));
		return *reinterpret_cast<Vector3*>(MaxVector);
		/*return Vector3(
			this->X < Max.X ? Max.X : this->X,
			this->Y < Max.Y ? Max.Y : this->Y,
			this->Z < Max.Z ? Max.Z : this->Z
		);*/
	}

	Vector3 Vector3::Clamp(const Vector3& Smallest, const Vector3& Biggest) const {
		return this->Min(Biggest).Max(Smallest);
	}


	Vector3 Vector3::Lerp(const Vector3& Goal, float Alpha) const {
		return *this + (Goal - *this) * Alpha;
	}

	Vector3 Vector3::Cross(const Vector3& b) const {

		return Vector3(
			this->Y * b.Z - b.Y * this->Z,
			this->Z * b.X - b.Z * this->X,
			this->X * b.Y - b.X * this->Y
		);
	}

	float Vector3::AngleBetween(const Vector3& b) const {

		//float angle = this->Dot(b) / (this->Magnitude() * b.Magnitude());
		//return angle = std::acosf(angle);
		return std::acosf(this->Dot(b) / (this->Magnitude() * b.Magnitude()));
	}

	Vector3 Vector3::ProjectOnto(const Vector3& b) const {
		const Vector3 bn = b / b.Magnitude();
		return bn * this->Dot(bn);
	}

	bool Vector3::SameSide(const Vector3& p1, const Vector3& p2, const Vector3& a, const Vector3& b) {
		return ((b - a).Cross(p1 - a)).Dot((b - a).Cross(p2 - a)) >= 0;
	}

	Vector3 Vector3::GenerateTriNormal(const Vector3& Point1, const Vector3& Point2, const Vector3& Point3) {
		return (Point2 - Point1).Cross(Point3 - Point1);
	}

	bool Vector3::InTriangle(const Vector3& Point1, const Vector3& Point2, const Vector3& Point3) const {

		// Test to see if it is within an infinite prism that the triangle outlines
		const bool WithinTrianglePrism =
			SameSide(*this, Point1, Point2, Point3) &&
			SameSide(*this, Point2, Point1, Point3) &&
			SameSide(*this, Point3, Point1, Point2);

		if (!WithinTrianglePrism) {
			return false;
		}

		return this->ProjectOnto(Vector3::GenerateTriNormal(Point1, Point2, Point3)).Magnitude() == 0;
	}



	// LUA IMPLEMENTATION

	int newVector3Userdata(lua_State* Context) {

		LuauClasses::Vector3* Vector3Userdata = (LuauClasses::Vector3*)lua_newuserdata(Context, sizeof(LuauClasses::Vector3));
		luaL_getmetatable(Context, "LuauClasses.Vector3");
		lua_setmetatable(Context, -2);

		for (int i = 0; i > -3; --i) {

			if (!lua_isnumber(Context, i)) {

				luaL_error(Context, "Attempted to create class 'Vector3' with component type: %s", lua_typename(Context, i));
				return -1;
			}

			Vector3Userdata[i] = (float)lua_tonumber(Context, i);
		}
		lua_pop(Context, 3);

		return 1;
	}

	static int Vector3Userdata__index(lua_State* Context) {

		if (!lua_isstring(Context, -1)) {

		}

		float* Vector3 = (float*)lua_touserdata(Context, -2);

		const char* IndexString = lua_tostring(Context, -1);
		if (IndexString[1] == '\0' && IndexString[0] >= 'X' && IndexString[0] <= 'Z') {
			lua_pushnumber(Context, (lua_Number)Vector3[IndexString[0] - 'X']);
		}

		return 1;
	}

	static int Vector3Userdata__newindex(lua_State* Context) {

		/*
		if (!lua_isstring(Context, -2)) {
			lua_pushfstring(Context, "Cannot index class 'Vector3' with %s. Expected type 'string', got %s", lua_tostring(Context, -2), lua_typename(Context, -2));
			lua_error(Context);
			return -1;
		}*/
		luaL_checktype(Context, -2, LUA_TSTRING);

		/*
		if (!lua_isnumber(Context, -1)) {

			lua_pushfstring(Context, "Cannot set element of class 'Vector3' with %s. Expected type 'number', got %s", lua_tostring(Context, -1), lua_typename(Context, -1));
			lua_error(Context);
			return -1;
		}*/
		luaL_checktype(Context, -1, LUA_TNUMBER);

		const char* IndexString = lua_tostring(Context, -2);
		if (IndexString[1] == '\0' && IndexString[0] >= 'X' && IndexString[0] <= 'Z') {

			float* Vector3 = (float*)lua_touserdata(Context, -3);
			Vector3[IndexString[0] - 'X'] = (float)lua_tonumber(Context, -1);
		} else {
			luaL_error(Context, "Cannot set element '%s' of class 'Vector3'. You can only set the X, Y, and Z components.", IndexString);
			return -1;
		}

		return 1;
	}

	static int Vector3Userdata__tostring(lua_State* Context) {

		const LuauClasses::Vector3* Vector3 = (LuauClasses::Vector3*)lua_touserdata(Context, -1);
		lua_pushfstring(Context, "%s, %s, %s", Vector3->X, Vector3->Y, Vector3->Z);

		return 1;
	}

	struct _arithmiticBaseVector3 {

		const LuauClasses::Vector3* P1;
		const LuauClasses::Vector3* P2;

		LuauClasses::Vector3* Result;
	};

	static inline _arithmiticBaseVector3 Vector3Userdata_arithmiticBase(lua_State* Context) {

		_arithmiticBaseVector3 Vectors;

		Vectors.P1 = (LuauClasses::Vector3*)lua_touserdata(Context, -2);

		Vectors.P2 = (LuauClasses::Vector3*)luaL_checkudata(Context, -1, "LuauClasses.Vector3");
		luaL_argcheck(Context, Vectors.P2 != NULL, -1, "Class 'Vector3' expected.");

		Vectors.Result = (LuauClasses::Vector3*)lua_newuserdata(Context, sizeof(LuauClasses::Vector3));
		luaL_getmetatable(Context, "LuauClasses.Vector3");
		lua_setmetatable(Context, -2);

		return Vectors;
	}

	static int Vector3Userdata__add(lua_State* Context) {

		_arithmiticBaseVector3 Vectors = Vector3Userdata_arithmiticBase(Context);
		*Vectors.Result = *Vectors.P1 + *Vectors.P2;
		return 1;
	}
	static int Vector3Userdata__sub(lua_State* Context) {

		_arithmiticBaseVector3 Vectors = Vector3Userdata_arithmiticBase(Context);
		*Vectors.Result = *Vectors.P1 - *Vectors.P2;
		return 1;
	}


	static int Vector3Userdata__mul(lua_State* Context) {

		_arithmiticBaseVector3 Vectors = Vector3Userdata_arithmiticBase(Context);
		*Vectors.Result = *Vectors.P1 * *Vectors.P2;
		return 1;
	}
	static int Vector3Userdata__div(lua_State* Context) {

		_arithmiticBaseVector3 Vectors = Vector3Userdata_arithmiticBase(Context);
		*Vectors.Result = *Vectors.P1 / *Vectors.P2;
		return 1;
	}


	int inline luaopen_Vector3(lua_State* Context) {

		luaL_newmetatable(Context, "LuauClasses.Vector3");

		lua_pushcfunction(Context, Vector3Userdata__index);
		lua_setfield(Context, -2, "__index");

		lua_pushcfunction(Context, Vector3Userdata__newindex);
		lua_setfield(Context, -2, "__newindex");


		lua_pushcfunction(Context, Vector3Userdata__tostring);
		lua_setfield(Context, -2, "__tostring");


		lua_pushcfunction(Context, Vector3Userdata__add);
		lua_setfield(Context, -2, "__add");

		lua_pushcfunction(Context, Vector3Userdata__sub);
		lua_setfield(Context, -2, "__sub");


		lua_pushcfunction(Context, Vector3Userdata__mul);
		lua_setfield(Context, -2, "__mul");

		lua_pushcfunction(Context, Vector3Userdata__div);
		lua_setfield(Context, -2, "__div");


		lua_pushstring(Context, "Vector3");
		lua_setfield(Context, -2, "__type");


		// Set Library Table

		lua_createtable(Context, 0, 1);


		lua_pushcfunction(Context, newVector3Userdata);
		lua_setfield(Context, -2, "new");


		lua_setglobal(Context, "Vector3");

		return 1;
	}
}

#endif