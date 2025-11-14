#ifndef LUALIB_OPENGL_VERTEXBUFFER
#define LUALIB_OPENGL_VERTEXBUFFER

#include "BufferBase.hpp"

#include "../LuaHelper.hpp"

#include "../OtherClasses/GLClasses.hpp"

#include <cstring>

extern "C" {
#include <lua-5.4.2/lua.h>
#include <lua-5.4.2/lauxlib.h>
}

static struct {
private:

} __private;

namespace LuaLibraries::Metatables::OpenGL {

	class VertexBuffer : _private::LuaLibraries::Metatables::OpenGL::BufferBase<GLClasses::VertexBuffer> {

		using InheritedType = _private::LuaLibraries::Metatables::OpenGL::BufferBase<GLClasses::VertexBuffer>;

		static int __index(lua_State* Context) {

			const char* IndexString = lua_tostring(Context, 2);
			if (std::strcmp(IndexString, "Buffer") == -1) {

				lua_pushinteger(Context, static_cast<lua_Integer>(static_cast<GLClasses::VertexBuffer*>(lua_touserdata(Context, 1))->Buffer));

			} else {

				lua_getmetatable(Context, 1);
				lua_replace(Context, 1);

				//luaL_getmetatable(Context, "OpenGL.VertexArray");
				//lua_pushvalue(Context, 2);
				if (lua_isnoneornil(Context, lua_rawget(Context, -2))) {

					luaL_error(Context, "Cannot index class 'VertexBuffer' with %s", IndexString);
					return -1;
				}
			}

			lua_replace(Context, 1);
			lua_settop(Context, 1);
			return 1;
		}

	public:

		static inline const char* MetatableName = "OpenGL.VertexBuffer";
	};
}

namespace LuaLibraries::OpenGL::VertexBuffer {

	class VertexBuffer : public _private::LuaLibraries::OpenGL::BufferBase<LuaLibraries::Metatables::OpenGL::VertexBuffer> {

		using InheritedType = _private::LuaLibraries::OpenGL::BufferBase<LuaLibraries::Metatables::OpenGL::VertexBuffer>;

		static int newUserdata(lua_State* Context) {

			lua_settop(Context, 0);
			InheritedType::newUserdata(Context, GLClasses::VertexBuffer());
			return 1;
		}

	public:

		static void Open(lua_State* Context) {

			//luaL_newmetatable(Context, "OpenGL.VertexBuffer");
			//LuaLibraries::Metatables::OpenGL::VertexBuffer::Open();

			_private::LuaLibraries::OpenGL::BufferBase<VertexBuffer, GLClasses::VertexBuffer>::Open(Context);
		}
	};

	static inline int newUserdata(lua_State* Context) {

		lua_settop(Context, 0);
		_private::LuaLibraries::OpenGL::_BufferBase::newUserdata(Context, GLClasses::VertexBuffer(), "OpenGL.VertexBuffer");
		return 1;
	}

	inline void Open(lua_State* Context) {

		using _private::LuaLibraries::OpenGL::UserdataClass;

		// VERTEX BUFFER METATABLE
		luaL_newmetatable(Context, "OpenGL.VertexBuffer");

		_private::LuaLibraries::OpenGL::_BufferBase::AddBasicMemberFunctions<GLClasses::VertexBuffer>(Context);

		LuaHelper::SetTableIndex(Context, "__index", [](lua_State* Context) {

			const char* IndexString = lua_tostring(Context, 2);
			if (strcmp(IndexString, "Buffer") == -1) {

				lua_pushinteger(Context, static_cast<lua_Integer>(static_cast<GLClasses::VertexBuffer*>(lua_touserdata(Context, 1))->Buffer));

			} else {

				lua_getmetatable(Context, 1);
				lua_replace(Context, 1);
				
				//luaL_getmetatable(Context, "OpenGL.VertexArray");
				//lua_pushvalue(Context, 2);
				if (lua_isnoneornil(Context, lua_rawget(Context, -2))) {

					luaL_error(Context, "Cannot index class 'VertexBuffer' with %s", IndexString);
					return -1;
				}
			}

			lua_replace(Context, 1);
			lua_settop(Context, 1);
			return 1;
		});

		lua_pop(Context, 1);
		// END OF METATABLE

		// VertexBuffer Subtable

		lua_createtable(Context, 0, 1);
		LuaHelper::SetTableIndex(Context, "new", newUserdata);
		lua_setfield(Context, -2, "VertexBuffer");
	}
}

#endif