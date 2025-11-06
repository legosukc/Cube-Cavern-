#ifndef LUAHELPER_H
#define LUAHELPER_H

extern "C" {
#include <lua-5.4.2/lua.h>
}

namespace LuaHelper {

	template<typename T>
	void PushOntoStack(lua_State* Context, T Value);

	template<>
	inline void PushOntoStack<const char*>(lua_State* Context, const char* Value) {
		lua_pushstring(Context, Value);
	}

	template<>
	inline void PushOntoStack<lua_Integer>(lua_State* Context, lua_Integer Value) {
		lua_pushinteger(Context, Value);
	}

	template<>
	inline void PushOntoStack<lua_Number>(lua_State* Context, lua_Number Value) {
		lua_pushnumber(Context, Value);
	}

	template<>
	inline void PushOntoStack<bool>(lua_State* Context, bool Value) {
		lua_pushboolean(Context, Value);
	}

	template<>
	inline void PushOntoStack<lua_CFunction>(lua_State* Context, lua_CFunction Function) {
		lua_pushcfunction(Context, Function);
	}

	template<>
	inline void PushOntoStack<void*>(lua_State* Context, void* Userdata) {
		lua_pushlightuserdata(Context, Userdata);
	}



	template<typename IndexType, typename SetType>
	void SetTableIndex(lua_State* Context, IndexType Index, SetType Value) {

		PushOntoStack<IndexType>(Context, Index);
		PushOntoStack<SetType>(Context, Value);
		lua_settable(Context, -3);
	}

	template<typename SetType>
	void SetTableIndex<const char*, SetType>(lua_State* Context, const char* Index, SetType Value) {

		PushOntoStack<SetType>(Context, Value);
		lua_setfield(Context, -2, Index);
	}

	template<typename SetType>
	void SetTableIndex<lua_Integer, SetType>(lua_State* Context, lua_Integer Index, SetType Value) {

		PushOntoStack<SetType>(Context, Value);
		lua_seti(Context, -2, Index);
	}

	template<typename SetType>
	void SetTableIndexWithLiteral(lua_State* Context, const char* Index, SetType Value) {

		lua_pushstring(Context, Index);
		PushOntoStack<SetType>(Context, Value);
		lua_settable(Context, -3);
	}


	template<typename IndexType>
	void GetTableIndex(lua_State* Context, IndexType Index) {

		PushOntoStack<IndexType>(Context, Index);
		lua_gettable(Context, -2);
	}
}

#endif