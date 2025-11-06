#ifndef _ENUMS_OPENGLBUFFERS
#define _ENUMS_OPENGLBUFFERS

#include <glad/glad.h>

#include "../LuaClasses/LuaContext.hpp"
#include "../LuaClasses/LuaTable.hpp"
#include "../LuaClasses/LuaGlobalTable.hpp"

namespace Enums::OpenGLBuffers {

	constexpr inline bool IsValidEnum(GLenum Enum) {

		switch (Enum) {

			case GL_VERTEX_ARRAY:
				return true;
			case GL_ARRAY_BUFFER:
				return true;
			case GL_ELEMENT_ARRAY_BUFFER:
				return true;
			case GL_UNIFORM_BUFFER:
				return true;
		}

		return false;
	}

	inline void AddToLuaTable(LuaClasses::LuaContext& Context, LuaClasses::LuaGlobalTable& EnumsTable) {

		LuaClasses::LuaTable EnumCatagoryTable(&Context, 0, 4);

		EnumCatagoryTable.SetElementWithLiteral("GL_VERTEX_ARRAY", GL_VERTEX_ARRAY);
		EnumCatagoryTable.SetElementWithLiteral("GL_ARRAY_BUFFER", GL_ARRAY_BUFFER);

		EnumCatagoryTable.SetElementWithLiteral("GL_ELEMENT_ARRAY_BUFFER", GL_ELEMENT_ARRAY_BUFFER);
		
		EnumCatagoryTable.SetElementWithLiteral("GL_UNIFORM_BUFFER", GL_UNIFORM_BUFFER);

		EnumsTable.SetSubtable("OpenGLBuffers", EnumCatagoryTable);
	}
}

#endif