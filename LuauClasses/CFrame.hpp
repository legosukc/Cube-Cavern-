#ifndef LUAUCLASSES_CFRAME_H
#define LUAUCLASSES_CFRAME_H

#include "./LuauClasses/Vector3.hpp"

#include <glm/glm.hpp>

namespace LuauClasses {

	class CFrame {
	public:
		CFrame() {
			this->Position = LuauClasses::Vector3(0.f);

			//glm::mat4(1.f) * glm::mat4(1.f);


			this->LookVector = LuauClasses::Vector3(0.f);
			this->RightVector = LuauClasses::Vector3(0.f);
			this->UpVector = LuauClasses::Vector3(0.f);
		}

		CFrame(LuauClasses::Vector3 Position) {
			this->Position = Position;

			this->LookVector = LuauClasses::Vector3(0.f);
			this->RightVector = LuauClasses::Vector3(0.f);
			this->UpVector = LuauClasses::Vector3(0.f);
		}

		CFrame(float X, float Y, float Z) {
			this->Position = LuauClasses::Vector3(X, Y, Z);

			this->LookVector = LuauClasses::Vector3(0.f);
			this->RightVector = LuauClasses::Vector3(0.f);
			this->UpVector = LuauClasses::Vector3(0.f);
		}

		CFrame(float X, float Y, float Z, float R00, float R01, float R02, float R10, float R11, float R12, float R20, float R21, float R22) {
			this->Position = LuauClasses::Vector3(X, Y, Z);

			this->LookVector = LuauClasses::Vector3(R00, R01, R02);
			this->RightVector = LuauClasses::Vector3(R10, R11, R12);
			this->UpVector = LuauClasses::Vector3(R20, R21, R22);
		}

		LuauClasses::Vector3 Position;

		LuauClasses::Vector3 LookVector;
		LuauClasses::Vector3 RightVector;
		LuauClasses::Vector3 UpVector;

		template<typename T>
		inline LuauClasses::Vector3& operator[](T Index) {
			return reinterpret_cast<LuauClasses::Vector3*>(this)[Index];
		}

		template<typename T>
		inline const LuauClasses::Vector3& operator[](T Index) const {
			return reinterpret_cast<const LuauClasses::Vector3*>(this)[Index];
		}

		/*
		CFrame Inverse() {

			const CFrame& m = *this;

			const float Coef00 = m[2][2] * m[3][3] - m[3][2] * m[2][3];
			const float Coef02 = m[1][2] * m[3][3] - m[3][2] * m[1][3];
			const float Coef03 = m[1][2] * m[2][3] - m[2][2] * m[1][3];

			const float Coef04 = m[2][1] * m[3][3] - m[3][1] * m[2][3];
			const float Coef06 = m[1][1] * m[3][3] - m[3][1] * m[1][3];
			const float Coef07 = m[1][1] * m[2][3] - m[2][1] * m[1][3];

			const float Coef08 = m[2][1] * m[3][2] - m[3][1] * m[2][2];
			const float Coef10 = m[1][1] * m[3][2] - m[3][1] * m[1][2];
			const float Coef11 = m[1][1] * m[2][2] - m[2][1] * m[1][2];

			const float Coef12 = m[2][0] * m[3][3] - m[3][0] * m[2][3];
			const float Coef14 = m[1][0] * m[3][3] - m[3][0] * m[1][3];
			const float Coef15 = m[1][0] * m[2][3] - m[2][0] * m[1][3];

			const float Coef16 = m[2][0] * m[3][2] - m[3][0] * m[2][2];
			const float Coef18 = m[1][0] * m[3][2] - m[3][0] * m[1][2];
			const float Coef19 = m[1][0] * m[2][2] - m[2][0] * m[1][2];

			const float Coef20 = m[2][0] * m[3][1] - m[3][0] * m[2][1];
			const float Coef22 = m[1][0] * m[3][1] - m[3][0] * m[1][1];
			const float Coef23 = m[1][0] * m[2][1] - m[2][0] * m[1][1];

			glm::vec4 Fac0(Coef00, Coef00, Coef02, Coef03);
			glm::vec4 Fac1(Coef04, Coef04, Coef06, Coef07);
			glm::vec4 Fac2(Coef08, Coef08, Coef10, Coef11);
			glm::vec4 Fac3(Coef12, Coef12, Coef14, Coef15);
			glm::vec4 Fac4(Coef16, Coef16, Coef18, Coef19);
			glm::vec4 Fac5(Coef20, Coef20, Coef22, Coef23);

			glm::vec4 Vec0(m[1][0], m[0][0], m[0][0], m[0][0]);
			glm::vec4 Vec1(m[1][1], m[0][1], m[0][1], m[0][1]);
			glm::vec4 Vec2(m[1][2], m[0][2], m[0][2], m[0][2]);
			glm::vec4 Vec3(m[1][3], m[0][3], m[0][3], m[0][3]);

			glm::vec4 Inv0(Vec1 * Fac0 - Vec2 * Fac1 + Vec3 * Fac2);
			glm::vec4 Inv1(Vec0 * Fac0 - Vec2 * Fac3 + Vec3 * Fac4);
			glm::vec4 Inv2(Vec0 * Fac1 - Vec1 * Fac3 + Vec3 * Fac5);
			glm::vec4 Inv3(Vec0 * Fac2 - Vec1 * Fac4 + Vec2 * Fac5);

			glm::vec4 SignA(+1, -1, +1, -1);
			glm::vec4 SignB(-1, +1, -1, +1);
			glm::mat4 Inverse(Inv0 * SignA, Inv1 * SignB, Inv2 * SignA, Inv3 * SignB);

			glm::vec4 Row0(Inverse[0][0], Inverse[1][0], Inverse[2][0], Inverse[3][0]);

			glm::vec4 Dot0(m[0] * Row0);

			return Inverse * (1.f / ((Dot0.x + Dot0.y) + (Dot0.z + Dot0.w)));
		}*/
	};

	LuauClasses::CFrame operator*(const LuauClasses::CFrame& m1, const LuauClasses::CFrame& m2) {
		const LuauClasses::Vector3& SrcA0 = m1.Position;
		const LuauClasses::Vector3& SrcA1 = m1.LookVector;
		const LuauClasses::Vector3& SrcA2 = m1.RightVector;
		const LuauClasses::Vector3& SrcA3 = m1.UpVector;

		const LuauClasses::Vector3& SrcB0 = m2.Position;
		const LuauClasses::Vector3& SrcB1 = m2.LookVector;
		const LuauClasses::Vector3& SrcB2 = m2.RightVector;
		const LuauClasses::Vector3& SrcB3 = m2.UpVector;

		LuauClasses::CFrame Result;
		Result.Position =		SrcA0 * SrcB0.X + SrcA1 * SrcB0.Y + SrcA2 * SrcB0.Z + SrcA3;
		Result.LookVector =		SrcA0 * SrcB1.X + SrcA1 * SrcB1.Y + SrcA2 * SrcB1.Z + SrcA3;
		Result.RightVector =	SrcA0 * SrcB2.X + SrcA1 * SrcB2.Y + SrcA2 * SrcB2.Z + SrcA3;
		Result.UpVector	=		SrcA0 * SrcB3.X + SrcA1 * SrcB3.Y + SrcA2 * SrcB3.Z + SrcA3;
		
		return Result;
	}
}

#endif