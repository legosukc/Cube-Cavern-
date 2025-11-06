#ifndef _ENUMS_OPENGLOPERATIONS
#define _ENUMS_OPENGLOPERATIONS

#include <glad/glad.h>

#include "../LuaClasses/LuaContext.hpp"
#include "../LuaClasses/LuaTable.hpp"
#include "../LuaClasses/LuaGlobalTable.hpp"

namespace Enums::OpenGLOperations {

	constexpr inline bool IsValidEnum(GLenum Enum) {
		return (Enum == GL_ZERO) || ((Enum >= GL_KEEP) && (Enum <= GL_DECR)) || (Enum == GL_INCR_WRAP) || (Enum == GL_DECR_WRAP) || (Enum == GL_INVERT);
	}

	inline void AddToLuaTable(LuaClasses::LuaContext& Context, LuaClasses::LuaGlobalTable& EnumsTable) {

		LuaClasses::LuaTable EnumCatagoryTable(&Context, 0, 8);

		EnumCatagoryTable.SetElementWithLiteral("GL_ZERO", GL_ZERO);

		EnumCatagoryTable.SetElementWithLiteral("GL_KEEP", GL_KEEP);
		EnumCatagoryTable.SetElementWithLiteral("GL_REPLACE", GL_REPLACE);

		EnumCatagoryTable.SetElementWithLiteral("GL_INCR", GL_INCR);
		EnumCatagoryTable.SetElementWithLiteral("GL_DECR", GL_DECR);

		EnumCatagoryTable.SetElementWithLiteral("GL_INCR_WRAP", GL_INCR_WRAP);
		EnumCatagoryTable.SetElementWithLiteral("GL_DECR_WRAP", GL_DECR_WRAP);

		EnumCatagoryTable.SetElementWithLiteral("GL_INVERT", GL_INVERT);

		EnumsTable.SetSubtable("OpenGLOperations", EnumCatagoryTable);
	}
}

#endif