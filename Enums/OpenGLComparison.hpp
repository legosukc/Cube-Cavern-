#ifndef _ENUMS_OPENGLCOMPARISON
#define _ENUMS_OPENGLCOMPARISON

#include <glad/glad.h>

#include "../LuaClasses/LuaContext.hpp"
#include "../LuaClasses/LuaTable.hpp"
#include "../LuaClasses/LuaGlobalTable.hpp"

namespace Enums::OpenGLComparison {

	constexpr inline bool IsValidEnum(GLenum Enum) {
		return (Enum >= GL_NEVER) && (Enum <= GL_ALWAYS);
	}

	inline void AddToLuaTable(LuaClasses::LuaContext& Context, LuaClasses::LuaGlobalTable& EnumsTable) {

		LuaClasses::LuaTable EnumCatagoryTable(&Context, 0, 8);

		EnumCatagoryTable.SetElementWithLiteral("GL_NEVER", GL_NEVER);
		EnumCatagoryTable.SetElementWithLiteral("GL_LESS", GL_LESS);

		EnumCatagoryTable.SetElementWithLiteral("GL_EQUAL", GL_EQUAL);
		EnumCatagoryTable.SetElementWithLiteral("GL_LEQUAL", GL_LEQUAL);

		EnumCatagoryTable.SetElementWithLiteral("GL_GREATER", GL_GREATER);
		EnumCatagoryTable.SetElementWithLiteral("GL_NOTEQUAL", GL_NOTEQUAL);

		EnumCatagoryTable.SetElementWithLiteral("GL_GEQUAL", GL_GEQUAL);
		EnumCatagoryTable.SetElementWithLiteral("GL_ALWAYS", GL_ALWAYS);

		EnumsTable.SetSubtable("OpenGLComparison", EnumCatagoryTable);
	}
}

#endif