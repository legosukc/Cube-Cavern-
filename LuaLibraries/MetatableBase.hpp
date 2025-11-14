#ifndef LUALIBS_METATABLEBASE
#define LUALIBS_METATABLEBASE

extern "C" {
#include <lua-5.4.2/lua.h>
#include <lua-5.4.2/lauxlib.h>
}

namespace LuaLibraries::Metatables {
	
	struct LibraryEntry {
		const char* Name;
		lua_CFunction Function;
	};

	class MetatableBase {
	protected:

		static void AddLibraryToTable(lua_State* Context) {

			for (const LibraryEntry& LibEntry : Entries) {

				lua_pushcfunction(LibEntry.Function);
				lua_setfield(Context, -2, LibEntry.Name);
			}
		}

		static void CreateTableFromLibrary(lua_State* Context) {

			lua_createtable(Context, 0, sizeof(Entries) / sizeof(LibraryEntry));
			AddLibraryToTable(Context);
		}

		static void OpenLibrary(lua_State* Context) {

			CreateTableFromLibrary(Context);
			lua_setglobal(Context, Name);
		}

	public:

		constexpr static inline const char* Name = nullptr;

		constexpr static inline LibraryEntry Entries[] = {
			{"Member", (lua_CFunction)nullptr}
		};

		static void Open(lua_State* Context) {

		}
	};
}

#endif