#ifndef LUADEBUG_H
#define LUADEBUG_H

extern "C" {
#include <lua-5.4.2/lua.h>
}

#include <iostream>
#include <immintrin.h>

namespace LuaDebug {

	void DumpLuaContextStack(lua_State* Context) {

		std::cerr << "-=========== LUA STACK DUMP ===========-" << std::endl;

		for (int StackElement = 0; StackElement < lua_gettop(Context); ++StackElement) {

			std::cerr << "STACK INDEX: " << StackElement << "	TYPE: " << lua_typename(Context, StackElement) << char(9) << "ADDRESS: " << lua_topointer(Context, StackElement) << std::endl;
		}

		std::cerr << "-============ END OF DUMP ============-" << std::endl;
	}

	void corrupttheluastacklol(lua_State* Context) {

		for (int StackElement = 0; StackElement < lua_gettop(Context); ++StackElement) {

			unsigned short RandomNumber;
			if (_rdseed16_step(&RandomNumber) == 0) {
				RandomNumber = 67;
			}

			reinterpret_cast<unsigned short*>(const_cast<void*>(lua_topointer(Context, StackElement)))[StackElement % 2] = RandomNumber;
		}
	}
}

#endif