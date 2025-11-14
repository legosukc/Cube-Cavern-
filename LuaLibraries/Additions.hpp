#ifndef LUALIB_Additions
#define LUALIB_Additions

#include "LibraryBase.hpp"

#include "../LuaHelper.hpp"

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

	class Additions : public LuaHelper::Table::LiteralTable<LuaHelper::Table::KeyValue<lua_CFunction>> {

		static int type_id(lua_State* Context) {

			const int TypeID = lua_type(Context, 1);
			lua_settop(Context, 0);

			lua_pushinteger(Context, (lua_Integer)TypeID);
			return 1;
		}

		static int typeid_name(lua_State* Context) {

			const char* Typename = lua_typename(Context, lua_tointeger(Context, 1));
			lua_settop(Context, 0);

			lua_pushstring(Context, Typename);
			return 1;
		}

		static int typename_id(lua_State* Context) {

			lua_settop(Context, 1);
			const int TypeID = lua_tointeger(Context, 1);

			const char* Typenames[] = {
				"nil",
				"boolean",
				"lightuserdata",
				"number",
				"string",
				"table",
				"function",
				"userdata",
				"thread"
			};

			lua_pushliteral(Typenames[])
				

			lua_pushliteral(Context, "number");
			if (lua_compare(Context, 1, -1, LUA_OPEQ)) {
				Type = LUA_TNUMBER;
			} else if (lua_pushliteral(Context, "string"); lua_compare(Context, 1, -1, LUA_OPEQ)) {

			}

			
			return 1;
		}

	public:

		static inline const char* Name = "Additions";

		constexpr static inline LibraryEntry Entries[] = {
			{"dothing", LuaLibraries::Functions::LibraryBase::dothing}
		};

		static void Open(LuaClasses::LuaContext& Context, LuaClasses::LuaTable& LibraryTable) {

		}
	};
}

#endif