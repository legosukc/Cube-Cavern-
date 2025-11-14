#ifndef _ENUMS_OPENGLTYPES
#define _ENUMS_OPENGLTYPES

#include "EnumBase.hpp"
#include "../LuaClasses/LuaContext.hpp"

extern "C" {
#include <lua-5.4.2/lua.h>
}

#include <glad/glad.h>

namespace Enums {

	struct OpenGLTypes : public Enums::EnumBase {

		static inline const char* EnumCatagoryName = "OpenGLTypes";
		static inline const size_t ElementCount = 7;

		template<typename T>
		constexpr static inline bool IsValidEnum(T Enum);

		static inline void AddToLuaTable(LuaClasses::LuaContext& Context);
	};

	template<typename T>
	constexpr bool OpenGLTypes::IsValidEnum(T Enum) {
		return (Enum >= GL_BYTE) && (Enum <= GL_FLOAT);
	}

	void OpenGLTypes::AddToLuaTable(LuaClasses::LuaContext& Context) {

		Context.SetTableField<lua_Integer>(-2, "GL_BYTE", GL_BYTE);
		Context.SetTableField<lua_Integer>(-2, "GL_UNSIGNED_BYTE", GL_UNSIGNED_BYTE);

		Context.SetTableField<lua_Integer>(-2, "GL_SHORT", GL_SHORT);
		Context.SetTableField<lua_Integer>(-2, "GL_UNSIGNED_SHORT", GL_UNSIGNED_SHORT);

		Context.SetTableField<lua_Integer>(-2, "GL_INT", GL_INT);
		Context.SetTableField<lua_Integer>(-2, "GL_UNSIGNED_INT", GL_UNSIGNED_INT);

		Context.SetTableField<lua_Integer>(-2, "GL_FLOAT", GL_FLOAT);
	}
}

#endif