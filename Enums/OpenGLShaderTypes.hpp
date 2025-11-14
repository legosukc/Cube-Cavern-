#ifndef _ENUMS_OPENGLSHADERTYPES
#define _ENUMS_OPENGLSHADERTYPES

#include "EnumBase.hpp"
#include "../LuaClasses/LuaContext.hpp"

extern "C" {
#include <lua-5.4.2/lua.h>
}

#include <glad/glad.h>

namespace Enums {

	struct OpenGLShaderTypes : public Enums::EnumBase {

		static inline const char* EnumCatagoryName = "OpenGLShaderTypes";
		static inline const size_t ElementCount = 6;

		template<typename T>
		constexpr static inline bool IsValidEnum(T Enum);

		static inline void AddToLuaTable(LuaClasses::LuaContext& Context);
	};

	template<typename T>
	constexpr bool OpenGLShaderTypes::IsValidEnum(T Enum) {

		return (Enum == GL_VERTEX_SHADER) || (Enum == GL_GEOMETRY_SHADER)
			|| (Enum == GL_TESS_CONTROL_SHADER) || (Enum == GL_TESS_EVALUATION_SHADER)
			|| (Enum == GL_FRAGMENT_SHADER)
			|| (Enum == GL_COMPUTE_SHADER);
	}

	void OpenGLShaderTypes::AddToLuaTable(LuaClasses::LuaContext& Context) {

		Context.SetTableField<lua_Integer>(-2, "GL_VERTEX_SHADER", GL_VERTEX_SHADER);

		Context.SetTableField<lua_Integer>(-2, "GL_TESS_CONTROL_SHADER", GL_TESS_CONTROL_SHADER);
		Context.SetTableField<lua_Integer>(-2, "GL_TESS_EVALUATION_SHADER", GL_TESS_EVALUATION_SHADER);

		Context.SetTableField<lua_Integer>(-2, "GL_GEOMETRY_SHADER", GL_GEOMETRY_SHADER);

		Context.SetTableField<lua_Integer>(-2, "GL_FRAGMENT_SHADER", GL_FRAGMENT_SHADER);

		Context.SetTableField<lua_Integer>(-2, "GL_COMPUTE_SHADER", GL_COMPUTE_SHADER);
	}
}

#endif