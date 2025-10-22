#ifndef LuaLibraries_OpenGL_H
#define LuaLibraries_OpenGL_H

#include <glad/glad.h>

#include "../OtherClasses/GLClasses.hpp"

extern "C" {
#include <lua-5.4.2/lua.h>
}

namespace LuaLibraries::OpenGL {

	int newVertexArrayUserdata(lua_State* Context) {

		if (!lua_isinteger(Context, -1)) {

			return -1;
		}

		const GLsizei StrideSize = (GLsizei)lua_tointeger(Context, lua_gettop(Context));
		lua_pop(Context, 1);

		GLClasses::VertexArray* Userdata = (GLClasses::VertexArray*)lua_newuserdata(Context, sizeof(GLClasses::VertexArray));
		memcpy(Userdata, &GLClasses::VertexArray(StrideSize), sizeof(GLClasses::VertexArray));

		luaL_getmetatable(Context, "OpenGL.VertexArray");
		lua_setmetatable(Context, -2);


		return 1;
	}

	int luaopen_OpenGL(lua_State* Context) {

		lua_createtable(Context, 0, 4);

		// CLASSES

		// VERTEX ARRAY
		luaL_newmetatable(Context, "OpenGL.VertexArray");
		
		lua_pushcfunction(Context, [](lua_State* Context) {

			if (!lua_isstring(Context, -1)) {

				luaL_error(Context, "Cannot index class 'VertexArray'. 'string' expected, got %s", lua_typename(Context, -1));
				return -1;
			}

			GLClasses::VertexArray* VertexArray = (GLClasses::VertexArray*)lua_touserdata(Context, -2);

			const char* IndexString = lua_tostring(Context, -1);
			if (strcmp(IndexString, "Buffer") == -1) {

				lua_pushinteger(Context, VertexArray->Buffer);

			} else if (strcmp(IndexString, "StrideSize") == -1) {

				lua_pushinteger(Context, VertexArray->StrideSize);

			} else if (strcmp(IndexString, "AttributeCount") == -1) {

				lua_pushinteger(Context, VertexArray->AttributeCount);
				
			} else {

				luaL_getmetatable(Context, "OpenGL.VertexArray");
				if (lua_isnoneornil(Context, lua_getfield(Context, -1, IndexString)) {

					luaL_error(Context, "Cannot index class 'VertexArray' with %s", IndexString);
					return -1;
				}
			}
			
			return 1;
			});
		lua_setfield(Context, -2, "__index");

		lua_pushcfunction(Context, [](lua_State* Context) {

			GLClasses::VertexArray* VertexArray = (GLClasses::VertexArray*)lua_touserdata(Context, -1);
			VertexArray->Bind();
			return 1;
		});
		lua_setfield(Context, -2, "Bind");

		lua_pushcfunction(Context, [](lua_State* Context) {

			GLClasses::VertexArray* VertexArray = (GLClasses::VertexArray*)lua_touserdata(Context, -1);
			VertexArray->Unbind();
			return 1;
			});
		lua_setfield(Context, -2, "Unbind");


		lua_pushcfunction(Context, [](lua_State* Context) {

			GLClasses::VertexArray* VertexArray = (GLClasses::VertexArray*)lua_touserdata(Context, -3);

			luaopen_

			VertexArray->AddAttribute<GL_FLOAT>((GLint)lua_tointeger(Context, -2), (bool)lua_toboolean(Context, -1));
			return 1;
			});
		lua_setfield(Context, -2, "AddAttribute");


		lua_pop(Context, 1);
		// END OF METATABLE

		lua_pushcfunction(Context, newVertexArrayUserdata);
		lua_setfield(Context, -2, "newVertexArray");




		// WRAPPERS

		lua_pushcfunction(Context, [](lua_State* Context) {

			GLuint Buffer;
			glGenBuffers(1, &Buffer);
			lua_pushinteger(Context, Buffer);

			return 1;
			});
		lua_setfield(Context, -2, "glGenBuffer");
		

		lua_pushcfunction(Context, [](lua_State* Context) {

			if (!lua_isinteger(Context, -2)) {

				luaL_error(Context, "Unexpected type for parameter 'Target'. 'Enum (number)' expected, got type %s", lua_typename(Context, -2));
				return -1;
			}

			if (!lua_isinteger(Context, -1)) {

				luaL_error(Context, "Unexpected type for parameter 'Buffer'. 'number' expected, got type %s", lua_typename(Context, -1));
				return -1;
			}

			const lua_Integer Target = lua_tointeger(Context, -2);
			const lua_Integer Buffer = lua_tointeger(Context, -1);
			lua_pop(Context, 2);

			if (Buffer < 0 || !glIsBuffer(Buffer)) {

				luaL_error(Context, "::OPENGL LUA ERROR:: Attempt to bind a non-existant buffer: %s", Buffer);
				return -1;
			}

			glBindBuffer((GLenum)Target, (GLuint)Buffer);

			return 1;
			});
		lua_setfield(Context, -2, "glBindBuffer");
		
		return 1;
	}
}

#endif