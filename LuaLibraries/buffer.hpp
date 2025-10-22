#ifndef LuaLibraries_buffer_H
#define LuaLibraries_buffer_H

#include "../IntTypes.h"
#include <SDL2/SDL_stdinc.h>

#include "../FunctionHeaders/Maths.hpp"

#include <cstring>

extern "C" {
#include <lua-5.4.2/lua.h>
#include <lua-5.4.2/lauxlib.h>
}

namespace LuaLibraries::buffer {

	struct lua_Buffer {

		size_t Size;
		char BufferContents[];
	};

	int newBufferUserdata(lua_State* Context) {

		if (!lua_isinteger(Context, -1)) {

			return -1;
		}

		const size_t BufferByteSize = (size_t)lua_tointeger(Context, -1);
		lua_pop(Context, 1);

		lua_Buffer* Buffer = (lua_Buffer*)lua_newuserdata(Context, sizeof(lua_Buffer) + BufferByteSize);
		Buffer->Size = BufferByteSize;
		std::memset(Buffer->BufferContents, 0, BufferByteSize);

		luaL_getmetatable(Context, "buffer");
		lua_setmetatable(Context, -2);

		return 1;
	}

	template<typename WriteValueType>
	static int WriteBufferValue(lua_State* Context) {

		if (!lua_isnumber(Context, -1)) {
			return -1;
		}
		if (!lua_isinteger(Context, -2)) {
			return -1;
		}

		const WriteValueType WriteValue = (WriteValueType)lua_tonumber(Context, -1);
		const size_t Offset = (size_t)Maths::Maximum(lua_tointeger(Context, -2), (lua_Integer)0);

		lua_Buffer* Buffer = (lua_Buffer*)lua_touserdata(Context, -3);
		lua_pop(Context, 3);

		if (Offset + sizeof(WriteValueType) > Buffer->Size) {
			return -1;
		}

		*(WriteValueType*)&Buffer->BufferContents[Offset] = WriteValue;

		return 1;
	}

	template<typename ReadValueType>
	static int ReadBufferValue(lua_State* Context) {

		if (!lua_isinteger(Context, -1)) {
			return -1;
		}

		const size_t Offset = (size_t)Maths::Maximum(lua_tointeger(Context, -1), (lua_Integer)0);

		lua_Buffer* Buffer = (lua_Buffer*)lua_touserdata(Context, -2);
		lua_pop(Context, 2);

		if (Offset + sizeof(ReadValueType) > Buffer->Size) {
			return -1;
		}

		lua_pushnumber(Context, *(ReadValueType*)&Buffer->BufferContents[Offset])

		return 1;
	}

	int luaopen_buffer(lua_State* Context) {

		lua_createtable(Context, 0, 1);

		// CLASSES

		// VERTEX ARRAY
		luaL_newmetatable(Context, "buffer");
		
		luaL_getmetatable(Context, "buffer");
		lua_setfield(Context, -2, "__index");

		lua_pushcfunction(Context, [](lua_State* Context) {

			lua_Buffer* Buffer = (lua_Buffer*)lua_touserdata(Context, -1);
			lua_pushinteger(Context, (lua_Integer)Buffer->Size);
			
			return 1;
		});
		lua_setfield(Context, -2, "size");


		lua_pushcfunction(Context, [](lua_State* Context) {

			lua_Buffer* Buffer = (lua_Buffer*)lua_touserdata(Context, -1);

			char* BufferString = new char[Buffer->Size + 1];
			std::memcpy(BufferString, Buffer->BufferContents, Buffer->Size);
			BufferString[Buffer->Size] = '\0';

			lua_pushstring(Context, BufferString);
			delete[] BufferString;

			return 1;
			});
		lua_setfield(Context, -2, "tostring");


		lua_pushcfunction(Context, WriteBufferValue<Int8>);
		lua_setfield(Context, -2, "writei8");

		lua_pushcfunction(Context, WriteBufferValue<Uint8>);
		lua_setfield(Context, -2, "writeu8");


		lua_pushcfunction(Context, WriteBufferValue<Int16>);
		lua_setfield(Context, -2, "writei16");

		lua_pushcfunction(Context, WriteBufferValue<Uint16>);
		lua_setfield(Context, -2, "writeu16");


		lua_pushcfunction(Context, WriteBufferValue<Int32>);
		lua_setfield(Context, -2, "writei32");

		lua_pushcfunction(Context, WriteBufferValue<Uint32>);
		lua_setfield(Context, -2, "writeu32");


		lua_pushcfunction(Context, WriteBufferValue<Int64>);
		lua_setfield(Context, -2, "writei64");

		lua_pushcfunction(Context, WriteBufferValue<Uint64>);
		lua_setfield(Context, -2, "writeu64");


		lua_pushcfunction(Context, WriteBufferValue<float>);
		lua_setfield(Context, -2, "writef32");

		lua_pushcfunction(Context, WriteBufferValue<double>);
		lua_setfield(Context, -2, "writef64");




		lua_pushcfunction(Context, ReadBufferValue<Int8>);
		lua_setfield(Context, -2, "readi8");

		lua_pushcfunction(Context, ReadBufferValue<Uint8>);
		lua_setfield(Context, -2, "readu8");


		lua_pushcfunction(Context, ReadBufferValue<Int16>);
		lua_setfield(Context, -2, "readi16");

		lua_pushcfunction(Context, ReadBufferValue<Uint16>);
		lua_setfield(Context, -2, "readu16");


		lua_pushcfunction(Context, ReadBufferValue<Int32>);
		lua_setfield(Context, -2, "readi32");

		lua_pushcfunction(Context, ReadBufferValue<Uint32>);
		lua_setfield(Context, -2, "readu32");


		lua_pushcfunction(Context, ReadBufferValue<Int64>);
		lua_setfield(Context, -2, "readi64");

		lua_pushcfunction(Context, ReadBufferValue<Uint64>);
		lua_setfield(Context, -2, "readu64");


		lua_pushcfunction(Context, ReadBufferValue<float>);
		lua_setfield(Context, -2, "readf32");

		lua_pushcfunction(Context, ReadBufferValue<double>);
		lua_setfield(Context, -2, "readf64");

		lua_pop(Context, 1);

		// END OF METATABLE

		lua_pushcfunction(Context, newBufferUserdata);
		lua_setfield(Context, -2, "new");

		lua_pop(Context, 1);
		
		return 1;
	}
}

#endif