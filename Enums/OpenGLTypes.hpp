#ifndef _ENUMS_OPENGLTYPES
#define _ENUMS_OPENGLTYPES

#include <glad/glad.h>

#include "../LuaClasses/LuaContext.hpp"
#include "../LuaClasses/LuaTable.hpp"
#include "../LuaClasses/LuaGlobalTable.hpp"

namespace Enums::OpenGLTypes {

	constexpr inline bool IsValidEnum(GLenum Enum) {
		return (Enum >= GL_BYTE) && (Enum <= GL_FLOAT);
	}

	inline void AddToLuaTable(LuaClasses::LuaContext& Context, LuaClasses::LuaGlobalTable& EnumsTable) {

		LuaClasses::LuaTable EnumCatagoryTable(&Context, 0, 7);

		EnumCatagoryTable.SetElementWithLiteral("GL_BYTE", GL_BYTE);
		EnumCatagoryTable.SetElementWithLiteral("GL_UNSIGNED_BYTE", GL_UNSIGNED_BYTE);

		EnumCatagoryTable.SetElementWithLiteral("GL_SHORT", GL_SHORT);
		EnumCatagoryTable.SetElementWithLiteral("GL_UNSIGNED_SHORT", GL_UNSIGNED_SHORT);

		EnumCatagoryTable.SetElementWithLiteral("GL_INT", GL_INT);
		EnumCatagoryTable.SetElementWithLiteral("GL_UNSIGNED_INT", GL_UNSIGNED_INT);

		EnumCatagoryTable.SetElementWithLiteral("GL_FLOAT", GL_FLOAT);

		EnumsTable.SetSubtable("OpenGLTypes", EnumCatagoryTable);
	}
}

#endif