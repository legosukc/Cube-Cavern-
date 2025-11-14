#ifndef _ENUMS_OPENGLTEXTUREFORMATS
#define _ENUMS_OPENGLTEXTUREFORMATS

#include "EnumBase.hpp"
#include "../LuaClasses/LuaContext.hpp"

extern "C" {
#include <lua-5.4.2/lua.h>
}

#include <glad/glad.h>

namespace Enums {

	struct OpenGLTextureFormats : public Enums::EnumBase {

		static inline const char* EnumCatagoryName = "OpenGLTextureFormats";
		static inline const size_t ElementCount = 4;

		template<typename T>
		constexpr static inline bool IsValidEnum(T Enum);

		static inline void AddToLuaTable(LuaClasses::LuaContext& Context);
	};

	template<typename T>
	constexpr bool OpenGLTextureFormats::IsValidEnum(T Enum) {
		return (Enum == GL_RGBA) || (Enum == GL_RGB) || (Enum == GL_RG) || (Enum == GL_R);
	}

	void OpenGLTextureFormats::AddToLuaTable(LuaClasses::LuaContext& Context) {

		Context.SetTableField<lua_Integer>(-2, "GL_RGBA", GL_RGBA);
		Context.SetTableField<lua_Integer>(-2, "GL_RGB", GL_RGB);

		Context.SetTableField<lua_Integer>(-2, "GL_RG", GL_RG);
		Context.SetTableField<lua_Integer>(-2, "GL_R", GL_R);
	}
}

#endif