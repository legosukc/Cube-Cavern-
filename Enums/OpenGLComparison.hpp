#ifndef _ENUMS_OPENGLCOMPARISON
#define _ENUMS_OPENGLCOMPARISON

#include "EnumBase.hpp"
#include "../LuaClasses/LuaContext.hpp"

extern "C" {
#include <lua-5.4.2/lua.h>
}

#include <glad/glad.h>

namespace Enums {

	struct OpenGLComparison : public Enums::EnumBase {

		static inline const char* EnumCatagoryName = "OpenGLComparison";
		static inline const size_t ElementCount = 8;

		template<typename T>
		constexpr static inline bool IsValidEnum(T Enum);

		static inline void AddToLuaTable(LuaClasses::LuaContext& Context);
	};

	template<typename T>
	constexpr bool OpenGLComparison::IsValidEnum(T Enum) {
		return (Enum >= GL_NEVER) && (Enum <= GL_ALWAYS);
	}

	void OpenGLComparison::AddToLuaTable(LuaClasses::LuaContext& Context) {

		Context.SetTableField<lua_Integer>(-2, "GL_NEVER", GL_NEVER);
		Context.SetTableField<lua_Integer>(-2, "GL_LESS", GL_LESS);

		Context.SetTableField<lua_Integer>(-2, "GL_EQUAL", GL_EQUAL);
		Context.SetTableField<lua_Integer>(-2, "GL_LEQUAL", GL_LEQUAL);

		Context.SetTableField<lua_Integer>(-2, "GL_GREATER", GL_GREATER);
		Context.SetTableField<lua_Integer>(-2, "GL_NOTEQUAL", GL_NOTEQUAL);

		Context.SetTableField<lua_Integer>(-2, "GL_GEQUAL", GL_GEQUAL);
		Context.SetTableField<lua_Integer>(-2, "GL_ALWAYS", GL_ALWAYS);
	}
}

#endif