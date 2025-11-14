#ifndef _ENUMS_OPENGLBUFFERS
#define _ENUMS_OPENGLBUFFERS

#include "EnumBase.hpp"
#include "../LuaClasses/LuaContext.hpp"

extern "C" {
#include <lua-5.4.2/lua.h>
}

#include <glad/glad.h>

namespace Enums {

	struct OpenGLBuffers : public Enums::EnumBase {

		static inline const char* EnumCatagoryName = "OpenGLBuffers";
		static inline const size_t ElementCount = 4;

		template<typename T>
		constexpr static inline bool IsValidEnum(T Enum);

		static inline void AddToLuaTable(LuaClasses::LuaContext& Context);
	};

	template<typename T>
	constexpr bool OpenGLBuffers::IsValidEnum(T Enum) {
		return (Enum == GL_VERTEX_ARRAY) || (Enum == GL_ARRAY_BUFFER)
			|| (Enum == GL_ELEMENT_ARRAY_BUFFER) || (Enum == GL_UNIFORM_BUFFER);
	}

	void OpenGLBuffers::AddToLuaTable(LuaClasses::LuaContext& Context) {

		Context.SetTableField<lua_Integer>(-2, "GL_VERTEX_ARRAY", GL_VERTEX_ARRAY);
		Context.SetTableField<lua_Integer>(-2, "GL_ARRAY_BUFFER", GL_ARRAY_BUFFER);
		Context.SetTableField<lua_Integer>(-2, "GL_ELEMENT_ARRAY_BUFFER", GL_ELEMENT_ARRAY_BUFFER);

		Context.SetTableField<lua_Integer>(-2, "GL_UNIFORM_BUFFER", GL_UNIFORM_BUFFER);
	}
}

#endif