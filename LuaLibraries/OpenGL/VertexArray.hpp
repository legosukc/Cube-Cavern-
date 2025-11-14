#ifndef LUALIB_OPENGL_VERTEXARRAY
#define LUALIB_OPENGL_VERTEXARRAY

#include "../LibraryBase.hpp"

#include "BufferBase.hpp"

#include <glad/glad.h>
#include "../../OtherClasses/GLClasses.hpp"

#include "../../LuaHelper.hpp"
#include "../../Enums/OpenGLTypes.hpp"

extern "C" {
#include <lua-5.4.2/lua.h>
#include <lua-5.4.2/lauxlib.h>
}

#include <cstring>

namespace LuaLibraries::OpenGL {

	class VertexArray : public LuaLibraries::LibraryBase {

		static int newUserdata(lua_State* Context) {

			if (!LuaHelper::AssertArguments(Context, 0, LUA_TNUMBER)) {
				return -1;
			}

			const GLsizei StrideSize = static_cast<GLsizei>(lua_tointeger(Context, 1));
			lua_settop(Context, 0);

			_private::LuaLibraries::OpenGL::_BufferBase::newUserdata(Context, GLClasses::VertexArray(StrideSize), "OpenGL.VertexArray");

			return 1;
		}

		static int __index(lua_State* Context) {

			auto* VertexArray = static_cast<GLClasses::VertexArray*>(lua_touserdata(Context, 1));

			const char* IndexString = lua_tostring(Context, 2);
			if (std::strcmp(IndexString, "Buffer") == -1) {

				lua_pushinteger(Context, static_cast<lua_Integer>(VertexArray->Buffer));

			} else if (std::strcmp(IndexString, "StrideSize") == -1) {

				lua_pushinteger(Context, static_cast<lua_Integer>(VertexArray->StrideSize));

			} else if (std::strcmp(IndexString, "AttributeCount") == -1) {

				lua_pushinteger(Context, static_cast<lua_Integer>(VertexArray->AttributeCount));

			} else {

				lua_getmetatable(Context, 1);
				lua_replace(Context, 1);
				//luaL_getmetatable(Context, "OpenGL.VertexArray");
				//lua_pushvalue(Context, 2);
				if (lua_isnoneornil(Context, lua_rawget(Context, 1))) {

					luaL_error(Context, "Cannot index class 'VertexArray' with %s", IndexString);
					return -1;
				}
			}

			lua_replace(Context, 1);
			lua_settop(Context, 1);
			return 1;
		}

		static int AddAttribute(lua_State* Context) {

			const GLenum AttributeType = static_cast<GLenum>(lua_tointeger(Context, 3));
			if (!Enums::OpenGLTypes::IsValidEnum(AttributeType)) {

				luaL_error(Context, "Parameter 'AttributeType' is not a valid 'OpenGLTypes' enum");
				return -1;
			}

			auto* VertexArray = static_cast<GLClasses::VertexArray*>(lua_touserdata(Context, 1));

			VertexArray->AddAttribute(
				static_cast<GLint>(lua_tointeger(Context, 2)),	// Atrribute Length
				AttributeType,
				static_cast<bool>(lua_toboolean(Context, 4))	// Normalize
			);

			lua_settop(Context, 0);
			return 1;
		}

	public:

		constexpr static inline const char* Name = "VertexArray";

		constexpr static inline LibraryEntry Entries[] = {
			{"new", newUserdata},
		};

		static inline void Open(lua_State* Context) {

			// VERTEX ARRAY METATABLE

			luaL_newmetatable(Context, "OpenGL.VertexArray");

			_private::LuaLibraries::OpenGL::_BufferBase::AddBasicMemberFunctions<GLClasses::VertexArray>(Context);

			LuaHelper::SetTableIndex(Context, "__index", __index);
			LuaHelper::SetTableIndex(Context, "AddAttribute", AddAttribute);

			lua_pop(Context, 1);

			LuaLibraries::AddLibraryToTable<VertexArray>(Context);
		}
	};
}

#endif