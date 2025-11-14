#ifndef LUALIB_table
#define LUALIB_table

#include "LibraryBase.hpp"

#include "../IntTypes.h"
#include <SDL2/SDL_stdinc.h>

#include "../FunctionHeaders/Maths.hpp"
#include "../LuaClasses/LuaGlobalTable.hpp"

#include <cstring>

extern "C" {
#include <lua-5.4.2/lua.h>
#include <lua-5.4.2/lauxlib.h>
}

namespace LuaLibraries {

	struct table : public LibraryBase {

		static inline const char* Name;

		constexpr static inline LibraryEntry Entries[] = {
			{"dothing", LuaLibraries::Functions::LibraryBase::dothing}
		};

		static void Open(LuaClasses::LuaContext& Context, LuaClasses::LuaTable& LibraryTable) {

		}
	};
}

#endif