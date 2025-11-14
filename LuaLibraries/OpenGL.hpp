#ifndef LUALIB_OPENGL
#define LUALIB_OPENGL

#include "LibraryBase.hpp"

#include "OpenGL/VertexArray.hpp"
#include "OpenGL/VertexBuffer.hpp"
#include "OpenGL/ElementBuffer.hpp"

#include "OpenGL/Program.hpp"
#include "OpenGL/Shader.hpp"

#include "OpenGL/Wrappers.hpp"

extern "C" {
#include <lua-5.4.2/lua.h>
}

namespace LuaLibraries::OpenGL {

	int luaopen_OpenGL(lua_State* Context) {

		lua_createtable(Context, 0, 23);
		
		// CLASSES

		VertexArray::Open(Context);
		VertexBuffer::Open(Context);
		ElementBuffer::Open(Context);

		Program::Open(Context);
		Shader::Open(Context);

		Wrappers::Open(Context);
		//Wrappers::Open(Context);

		lua_setglobal(Context, "OpenGL");

		return 1;
	}
}

#endif