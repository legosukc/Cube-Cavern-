#ifndef LuaLibraries_buffer_H
#define LuaLibraries_buffer_H

#include "../IntTypes.h"
#include <SDL2/SDL_stdinc.h>

#include "../FunctionHeaders/Maths.hpp"
#include "../LuaClasses/LuaGlobalTable.hpp"

#include <cstring>

extern "C" {
#include <lua-5.4.2/lua.h>
#include <lua-5.4.2/lauxlib.h>
}

namespace LuaLibraries::buffer {

	bool Initialized = false;

	struct lua_Buffer {

		size_t Size;
		char BufferContents[];
	};

	lua_Buffer* newBufferUserdata(lua_State* Context, size_t BufferByteSize) {

		lua_Buffer* Buffer = (lua_Buffer*)lua_newuserdata(Context, sizeof(lua_Buffer) + BufferByteSize);
		Buffer->Size = BufferByteSize;

		luaL_getmetatable(Context, "buffer");
		lua_setmetatable(Context, -2);

		return Buffer;
	}

	static int LuanewBufferUserdata(lua_State* Context) {

		if (!lua_isinteger(Context, -1)) {

			return -1;
		}

		const size_t BufferByteSize = (size_t)lua_tointeger(Context, -1);
		lua_pop(Context, 1);

		std::memset(&newBufferUserdata(Context, BufferByteSize)->BufferContents, 0, BufferByteSize);
		/*
		lua_Buffer* Buffer = (lua_Buffer*)lua_newuserdata(Context, sizeof(lua_Buffer) + BufferByteSize);
		Buffer->Size = BufferByteSize;
		std::memset(&Buffer->BufferContents, 0, BufferByteSize);

		luaL_getmetatable(Context, "buffer");
		lua_setmetatable(Context, -2);*/

		return 1;
	}

	void BufferFromRawData(lua_State* Context, size_t BufferByteSize, const void* Data) {

		lua_Buffer* Buffer = (lua_Buffer*)lua_newuserdata(Context, sizeof(lua_Buffer) + BufferByteSize);
		Buffer->Size = BufferByteSize;
		std::memcpy(Buffer->BufferContents, Data, BufferByteSize);

		luaL_getmetatable(Context, "buffer");
		lua_setmetatable(Context, -2);
	}

	int BufferFromString(lua_State* Context) {

		if (!lua_isstring(Context, -1)) {

			return -1;
		}

		size_t BufferByteSize;
		const char* BufferString = lua_tolstring(Context, -1, &BufferByteSize);
		lua_pop(Context, 1);

		BufferFromRawData(Context, BufferByteSize, BufferString);
		/*
		lua_Buffer* Buffer = (lua_Buffer*)lua_newuserdata(Context, sizeof(lua_Buffer) + BufferByteSize);
		Buffer->Size = BufferByteSize;
		std::memcpy(Buffer->BufferContents, BufferString, BufferByteSize);

		luaL_getmetatable(Context, "buffer");
		lua_setmetatable(Context, -2);*/

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

		//*(WriteValueType*)&Buffer->BufferContents[Offset] = WriteValue;
		reinterpret_cast<WriteValueType&>(Buffer->BufferContents[Offset]) = WriteValue;

		return 1;
	}

	template<typename ReadValueType>
	static int ReadBufferValue(lua_State* Context) {

		if (!lua_isinteger(Context, -1)) {
			return -1;
		}

		const size_t Offset = static_cast<size_t>(Maths::Maximum(lua_tointeger(Context, -1), static_cast<lua_Integer>(0)));

		const lua_Buffer* Buffer = static_cast<lua_Buffer*>(lua_touserdata(Context, -2));
		lua_pop(Context, 2);

		if (Offset + sizeof(ReadValueType) > Buffer->Size) {
			return -1;
		}

		lua_pushnumber(Context, static_cast<lua_Number>(reinterpret_cast<ReadValueType[]>(Buffer->BufferContents)[Offset]));

		return 1;
	}

	int luaopen_buffer(lua_State* Context) {

		if (Initialized) {
			return 1;
		}

		// CLASSES

		// VERTEX ARRAY
		luaL_newmetatable(Context, "buffer");
		
		lua_pushvalue(Context, -1);
		lua_setfield(Context, -2, "__index");

		
		LuaHelper::SetTableIndex(Context, "size", [](lua_State* Context) {

			const lua_Buffer* Buffer = static_cast<lua_Buffer*>(lua_touserdata(Context, -1));
			lua_pushinteger(Context, static_cast<lua_Integer>(Buffer->Size));

			return 1;
		});

		LuaHelper::SetTableIndex(Context, "tostring", [](lua_State* Context) {

			const lua_Buffer* Buffer = static_cast<lua_Buffer*>(lua_touserdata(Context, -1));
			lua_pushlstring(Context, Buffer->BufferContents, Buffer->Size);

			return 1;
		});

		LuaHelper::SetTableIndex(Context, "writei8", WriteBufferValue<Int8>);
		LuaHelper::SetTableIndex(Context, "writeu8", WriteBufferValue<Uint8>);

		LuaHelper::SetTableIndex(Context, "writei16", WriteBufferValue<Int16>);
		LuaHelper::SetTableIndex(Context, "writeu16", WriteBufferValue<Uint16>);

		LuaHelper::SetTableIndex(Context, "writei32", WriteBufferValue<Int32>);
		LuaHelper::SetTableIndex(Context, "writeu32", WriteBufferValue<Uint32>);

		LuaHelper::SetTableIndex(Context, "writei64", WriteBufferValue<Int64>);
		LuaHelper::SetTableIndex(Context, "writeu64", WriteBufferValue<Uint64>);

		LuaHelper::SetTableIndex(Context, "writef32", WriteBufferValue<float>);
		LuaHelper::SetTableIndex(Context, "writef64", WriteBufferValue<double>);



		LuaHelper::SetTableIndex(Context, "readi8", ReadBufferValue<Int8>);
		LuaHelper::SetTableIndex(Context, "readu8", ReadBufferValue<Uint8>);

		LuaHelper::SetTableIndex(Context, "readi16", ReadBufferValue<Int16>);
		LuaHelper::SetTableIndex(Context, "readu16", ReadBufferValue<Uint16>);

		LuaHelper::SetTableIndex(Context, "readi32", ReadBufferValue<Int32>);
		LuaHelper::SetTableIndex(Context, "readu32", ReadBufferValue<Uint32>);

		LuaHelper::SetTableIndex(Context, "readi64", ReadBufferValue<Int64>);
		LuaHelper::SetTableIndex(Context, "readu64", ReadBufferValue<Uint64>);

		LuaHelper::SetTableIndex(Context, "readf32", ReadBufferValue<float>);
		LuaHelper::SetTableIndex(Context, "readf64", ReadBufferValue<double>);

		lua_pop(Context, 1);
		// END OF METATABLE

		lua_createtable(Context, 0, 2);

		lua_pushcfunction(Context, LuanewBufferUserdata);
		lua_pushliteral(Context, "create");
		lua_settable(Context, -3);

		lua_pushcfunction(Context, BufferFromString);
		lua_pushliteral(Context, "fromstring");
		lua_settable(Context, -3);

		lua_setglobal(Context, "buffer");

		Initialized = true;
		
		return 1;
	}
}

#endif