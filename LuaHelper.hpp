#ifndef LUAHELPER_H
#define LUAHELPER_H

#include <cstdarg>
#include "define.h"

extern "C" {
#include <lua-5.4.2/lua.h>
#include <lua-5.4.2/lauxlib.h>
}

namespace LuaHelper {

	template<typename T>
	inline void PushOntoStack(lua_State* Context, T Value) {}

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

	namespace MetatableIndexers {

	}

	/*
	inline void TypeError(lua_State* Context, int ArgIndex, const char* ExpectedType) {
		luaL_error(Context, "::LUA ARG ERROR:: Unexpected type of argument at stack index %s. '%s' expected, got %s.", lua_absindex(Context, ArgIndex), ExpectedType, luaL_typename(Context, ArgIndex));
	}

	inline void TypeError(lua_State* Context, int ArgIndex, int ExpectedType) {
		TypeError(Context, ArgIndex, lua_typename(Context, ExpectedType));
	}*/


	bool AssertArgumentTypes(lua_State* Context, int StartAtArg, int ArgCount...) {

		va_list args;
		va_start(args, ArgCount);

		for (int ArgIndex = 1 + StartAtArg; ArgIndex < ArgCount; ++ArgIndex) {

			unlikely_branch
			if (const int ArgType = lua_type(Context, ArgIndex),
						  ExpectedType = va_arg(args, int);

				ArgType != ExpectedType) {

				luaL_error(Context, "::LUA ARG ERROR:: Unexpected type of argument at index %s. '%s' expected, got %s", ArgIndex, lua_typename(Context, ExpectedType), lua_typename(Context, ArgType));
				return false;
			}
			
		}
		va_end(args);

		return true;
	}

	inline bool AssertArgumentCount(lua_State* Context, int ExpectedArgumentCount, int StartAtArg = 0) {

		unlikely_branch
		if (const int StackSize = lua_gettop(Context); StackSize < (ExpectedArgumentCount + StartAtArg)) {

			luaL_error(Context, "::LUA ARG ERROR:: Expected %s arguments, got %s", ExpectedArgumentCount, StackSize);
			return false;
		}

		return true;
	}

	inline bool AssertArguments(lua_State* Context, int StartAtArg, int ExpectedArgumentCount...) {
		return AssertArgumentCount(Context, ExpectedArgumentCount, StartAtArg) && AssertArgumentTypes(Context, StartAtArg, ExpectedArgumentCount);
	}
}

namespace LuaHelper::Table {

	template<typename T_TableElement>
	struct LiteralTable {

		using ElementType = T_TableElement;

		static inline T_TableElement Entries[];

		static void AddToLuaTable(lua_State* Context) {

			for (const auto& Entry : Entries) {
				LuaHelper::SetTableIndex(Context, Entry.Index, Entry.Value);
			}
		}

		static inline void CreateLuaTable(lua_State* Context) {

			lua_createtable(Context, 0, sizeof(Entries) / sizeof(T_TableElement));
			AddToLuaTable(Context);
		}
	};

	template<typename T_Index, typename T_Value>
	struct Element {
		T_Index Index;
		T_Value Value;
	};

	template<typename T_Value>
	using KeyValue = Element<const char*, T_Value>;
}

#endif