#ifndef _ENUMS_OPENGLTEXTUREFORMATS
#define _ENUMS_OPENGLTEXTUREFORMATS

#include <glad/glad.h>

#include "../LuaClasses/LuaContext.hpp"
#include "../LuaClasses/LuaTable.hpp"
#include "../LuaClasses/LuaGlobalTable.hpp"

namespace Enums::OpenGLTextureFormats {

	constexpr inline bool IsValidEnum(GLenum Enum) {
		return (Enum == GL_RGBA) || (Enum == GL_RGB) || (Enum == GL_RG) || (Enum == GL_R);
	}

	inline void AddToLuaTable(LuaClasses::LuaContext& Context, LuaClasses::LuaGlobalTable& EnumsTable) {

		LuaClasses::LuaTable EnumCatagoryTable(&Context, 0, 4);

		EnumCatagoryTable.SetElementWithLiteral("GL_RGBA", GL_RGBA);
		EnumCatagoryTable.SetElementWithLiteral("GL_RGB", GL_RGB);

		EnumCatagoryTable.SetElementWithLiteral("GL_RG", GL_RG);
		EnumCatagoryTable.SetElementWithLiteral("GL_R", GL_R);

		EnumsTable.SetSubtable("OpenGLTextureFormats", EnumCatagoryTable);
	}
}

#endif