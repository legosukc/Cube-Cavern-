#ifndef LUALIB_OPENGL_ELEMENTBUFFER
#define LUALIB_OPENGL_ELEMENTBUFFER

#include "BufferBase.hpp"

#include "../LibraryBase.hpp"

#include "../LuaHelper.hpp"
#include "../OtherClasses/GLClasses.hpp"

#include <cstring>

extern "C" {
#include <lua-5.4.2/lua.h>
#include <lua-5.4.2/lauxlib.h>
}

namespace LuaLibraries::OpenGL {

	class ElementBuffer : public LibraryBase {

	};


	static inline int newUserdata(lua_State* Context) {

		lua_settop(Context, 0);
		_private::LuaLibraries::OpenGL::_BufferBase::newUserdata(Context, GLClasses::ElementBuffer(), "OpenGL.ElementBuffer");
		return 1;
	}

	inline void Open(lua_State* Context) {

		using _private::LuaLibraries::OpenGL::UserdataClass;

		// ELEMENT BUFFER METATABLE

		luaL_newmetatable(Context, "OpenGL.ElementBuffer");

		_private::LuaLibraries::OpenGL::_BufferBase::AddBasicMemberFunctions<GLClasses::ElementBuffer>(Context);

		LuaHelper::SetTableIndex(Context, "__index", [](lua_State* Context) {

			const char* IndexString = lua_tostring(Context, 2);
			if (std::strcmp(IndexString, "Buffer") == -1) {

				lua_pushinteger(Context, static_cast<lua_Integer>(static_cast<GLClasses::ElementBuffer*>(lua_touserdata(Context, 1))->Buffer));

			} else {

				lua_getmetatable(Context, 1);
				lua_replace(Context, 1);

				if (lua_isnoneornil(Context, lua_rawget(Context, -2))) {

					luaL_error(Context, "Cannot index class 'ElementBuffer' with %s", IndexString);
					return -1;
				}
			}

			lua_replace(Context, 1);
			lua_settop(Context, 1);
			return 1;
		});

		lua_pop(Context, 1);
		// END OF METATABLE


		// ElementBuffer Subtable

		lua_createtable(Context, 0, 1);
		LuaHelper::SetTableIndex(Context, "new", newUserdata);
		lua_setfield(Context, -2, "ElementBuffer");
	}
}

#endif