#ifndef _ENUMS_OPENGLDRAWMODES
#define _ENUMS_OPENGLDRAWMODES

#include <glad/glad.h>

#include "../LuaClasses/LuaContext.hpp"
#include "../LuaClasses/LuaTable.hpp"
#include "../LuaClasses/LuaGlobalTable.hpp"

namespace Enums::OpenGLDrawModes {

	constexpr inline bool IsValidEnum(GLenum Enum) {
		return (Enum <= GL_LINES) || (Enum == GL_TRIANGLES);
	}

	inline void AddToLuaTable(LuaClasses::LuaContext& Context, LuaClasses::LuaGlobalTable& EnumsTable) {

		LuaClasses::LuaTable EnumCatagoryTable(&Context, 0, 3);

		EnumCatagoryTable.SetElementWithLiteral("GL_POINTS", GL_POINTS);
		EnumCatagoryTable.SetElementWithLiteral("GL_LINES", GL_LINES);
		EnumCatagoryTable.SetElementWithLiteral("GL_TRIANGLES", GL_TRIANGLES);

		EnumsTable.SetSubtable("OpenGLDrawModes", EnumCatagoryTable);
	}
}

#endif