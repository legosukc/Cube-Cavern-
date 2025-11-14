#ifndef _ENUMS_OPENGLOPERATIONS
#define _ENUMS_OPENGLOPERATIONS

#include "EnumBase.hpp"
#include "../LuaClasses/LuaContext.hpp"

extern "C" {
#include <lua-5.4.2/lua.h>
}

#include <glad/glad.h>

namespace Enums {

	struct OpenGLOperations : public Enums::EnumBase {

		static inline const char* EnumCatagoryName = "OpenGLOperations";
		static inline const size_t ElementCount = 8;

		template<typename T>
		constexpr static inline bool IsValidEnum(T Enum);

		static inline void AddToLuaTable(LuaClasses::LuaContext& Context);
	};

	template<typename T>
	constexpr bool OpenGLOperations::IsValidEnum(T Enum) {
		return (Enum == GL_ZERO) || ((Enum >= GL_KEEP) && (Enum <= GL_DECR))
			|| (Enum == GL_INCR_WRAP) || (Enum == GL_DECR_WRAP) || (Enum == GL_INVERT);
	}

	void OpenGLOperations::AddToLuaTable(LuaClasses::LuaContext& Context) {

		Context.SetTableField<lua_Integer>(-2, "GL_ZERO", GL_ZERO);

		Context.SetTableField<lua_Integer>(-2, "GL_KEEP", GL_KEEP);
		Context.SetTableField<lua_Integer>(-2, "GL_REPLACE", GL_REPLACE);

		Context.SetTableField<lua_Integer>(-2, "GL_INCR", GL_INCR);
		Context.SetTableField<lua_Integer>(-2, "GL_DECR", GL_DECR);

		Context.SetTableField<lua_Integer>(-2, "GL_INCR_WRAP", GL_INCR_WRAP);
		Context.SetTableField<lua_Integer>(-2, "GL_DECR_WRAP", GL_DECR_WRAP);

		Context.SetTableField<lua_Integer>(-2, "GL_INVERT", GL_INVERT);
	}
}

#endif