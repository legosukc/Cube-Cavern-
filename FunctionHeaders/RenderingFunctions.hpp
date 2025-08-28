#ifndef _RENDERING_FUNCTIONS
#define _RENDERING_FUNCTIONS

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/fwd.hpp>

#include <cmath>

#include "LuauClasses/Vector3.hpp"

namespace RenderingFunctions {

	static glm::mat4 perspective(float fovy, float aspect, float zNear, float zFar) {

		const float tanHalfFovy = std::tan(fovy / 2.f);

		glm::mat4 Result(0.f);

		Result[0][0] = 1.f / (aspect * tanHalfFovy);
		Result[1][1] = 1.f / (tanHalfFovy);
		Result[2][2] = -(zFar + zNear) / (zFar - zNear);
		Result[2][3] = -1.f;
		Result[3][2] = -(2.f * zFar * zNear) / (zFar - zNear);
		return Result;
	}

	static glm::mat4 rotate(const glm::mat4& m, float angle, const LuauClasses::Vector3& v) {
		
		const float AngleCosine = std::cos(angle);
		const float AngleSine = std::sin(angle);

		const LuauClasses::Vector3 axis = v.Unit();
		const LuauClasses::Vector3 temp = ((1.f - AngleCosine) * axis);

		glm::mat4 Rotate;
		Rotate[0][0] = AngleCosine + temp.X * axis.X;
		Rotate[0][1] = temp.X * axis.Y + AngleSine * axis.Z;
		Rotate[0][2] = temp.X * axis.Z - AngleSine * axis.Y;

		Rotate[1][0] = temp.Y * axis.X - AngleSine * axis.Z;
		Rotate[1][1] = AngleCosine + temp.Y * axis.Y;
		Rotate[1][2] = temp.Y * axis.Z + AngleSine * axis.X;

		Rotate[2][0] = temp.Z * axis.X + AngleSine * axis.Y;
		Rotate[2][1] = temp.Z * axis.Y - AngleSine * axis.X;
		Rotate[2][2] = AngleCosine + temp.Z * axis.Z;

		glm::mat4 Result;
		Result[0] = m[0] * Rotate[0][0] + m[1] * Rotate[0][1] + m[2] * Rotate[0][2];
		Result[1] = m[0] * Rotate[1][0] + m[1] * Rotate[1][1] + m[2] * Rotate[1][2];
		Result[2] = m[0] * Rotate[2][0] + m[1] * Rotate[2][1] + m[2] * Rotate[2][2];
		Result[3] = m[3];
		return Result;
	}

	static glm::mat4 translate(const glm::mat4& m, const LuauClasses::Vector3& v) {
		glm::mat4 Result(m);
		Result[3] = m[0] * v.X + m[1] * v.Y + m[2] * v.Z + m[3];
		return Result;
	}

	static glm::mat4 lookAt(const LuauClasses::Vector3& eye, const LuauClasses::Vector3& center, const LuauClasses::Vector3& up) {

		const LuauClasses::Vector3 f = (center - eye).Unit();
		const LuauClasses::Vector3 s = f.Cross(up).Unit();
		const LuauClasses::Vector3 u = s.Cross(f);

		glm::mat4 Result(1.f);
		Result[0][0] = s.X;
		Result[1][0] = s.Y;
		Result[2][0] = s.Z;

		Result[0][1] = u.X;
		Result[1][1] = u.Y;
		Result[2][1] = u.Z;

		Result[0][2] = -f.X;
		Result[1][2] = -f.Y;
		Result[2][2] = -f.Z;

		Result[3][0] = -s.Dot(eye);
		Result[3][1] = -u.Dot(eye);
		Result[3][2] = f.Dot(eye);
		return Result;
	}
}

#endif