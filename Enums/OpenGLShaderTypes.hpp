#ifndef _ENUMS_OPENGLSHADERTYPES
#define _ENUMS_OPENGLSHADERTYPES

#include <glad/glad.h>

#include "../LuaClasses/LuaContext.hpp"
#include "../LuaClasses/LuaTable.hpp"
#include "../LuaClasses/LuaGlobalTable.hpp"


namespace Enums::OpenGLShaderTypes {

	constexpr inline bool IsValidEnum(GLenum Enum) {

		return (Enum == GL_VERTEX_SHADER) || (Enum == GL_GEOMETRY_SHADER)
			|| (Enum == GL_TESS_CONTROL_SHADER) || (Enum == GL_TESS_EVALUATION_SHADER)
			|| (Enum == GL_FRAGMENT_SHADER)
			|| (Enum == GL_COMPUTE_SHADER);
	}

	inline void AddToLuaTable(LuaClasses::LuaContext& Context, LuaClasses::LuaGlobalTable& EnumsTable) {

		LuaClasses::LuaTable EnumCatagoryTable(&Context, 0, 6);

		EnumCatagoryTable.SetElementWithLiteral("GL_VERTEX_SHADER", GL_VERTEX_SHADER);

		EnumCatagoryTable.SetElementWithLiteral("GL_TESS_CONTROL_SHADER", GL_TESS_CONTROL_SHADER);
		EnumCatagoryTable.SetElementWithLiteral("GL_TESS_EVALUATION_SHADER", GL_TESS_EVALUATION_SHADER);

		EnumCatagoryTable.SetElementWithLiteral("GL_GEOMETRY_SHADER", GL_GEOMETRY_SHADER);
		
		EnumCatagoryTable.SetElementWithLiteral("GL_FRAGMENT_SHADER", GL_FRAGMENT_SHADER);

		EnumCatagoryTable.SetElementWithLiteral("GL_COMPUTE_SHADER", GL_COMPUTE_SHADER);

		EnumsTable.SetSubtable("OpenGLShaderTypes", EnumCatagoryTable);
	}
}

#endif