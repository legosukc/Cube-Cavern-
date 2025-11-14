#ifndef _ENUMS_OPENGLDRAWMODES
#define _ENUMS_OPENGLDRAWMODES

#include "EnumBase.hpp"
#include "../LuaClasses/LuaContext.hpp"

extern "C" {
#include <lua-5.4.2/lua.h>
}

#include <glad/glad.h>

namespace Enums {

	struct OpenGLDrawModes : public Enums::EnumBase {

		static inline const char* EnumCatagoryName = "OpenGLDrawModes";
		static inline const size_t ElementCount = 3;

		template<typename T>
		constexpr static inline bool IsValidEnum(T Enum);

		static inline void AddToLuaTable(LuaClasses::LuaContext& Context);
	};

	template<typename T>
	constexpr bool OpenGLDrawModes::IsValidEnum(T Enum) {
		return (Enum == GL_POINTS) || (Enum == GL_LINES) || (Enum == GL_TRIANGLES);
	}

	void OpenGLDrawModes::AddToLuaTable(LuaClasses::LuaContext& Context) {

		Context.SetTableField<lua_Integer>(-2, "GL_POINTS", GL_POINTS);
		Context.SetTableField<lua_Integer>(-2, "GL_LINES", GL_LINES);
		Context.SetTableField<lua_Integer>(-2, "GL_TRIANGLES", GL_TRIANGLES);
	}
}

#endif