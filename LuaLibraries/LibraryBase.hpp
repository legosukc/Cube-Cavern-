#ifndef LUALIBS_LIBRARYBASE
#define LUALIBS_LIBRARYBASE

extern "C" {
#include <lua-5.4.2/lua.h>
#include <lua-5.4.2/lauxlib.h>
}

namespace LuaLibraries {
	
	struct LibraryEntry {
		const char* Name;
		lua_CFunction Function;
	};

	struct LibraryBase {

		static inline const char* Name;

		static inline LibraryEntry Entries[] = {
			{"Member", (lua_CFunction)nullptr}
		};

		static void Open(lua_State* Context);
	};

	template<class Library>
	void AddLibraryToTable(lua_State* Context) {

		for (const LibraryEntry& LibEntry : Library::Entries) {

			lua_pushcfunction(LibEntry.Function);
			lua_setfield(Context, -2, LibEntry.Name);
		}
	}

	template<class Library>
	inline void CreateTableFromLibrary(lua_State* Context) {

		lua_createtable(Context, 0, sizeof(Library::Entries) / sizeof(LibraryEntry));
		AddLibraryToTable<Library>(Context);
	}

	template<class Library>
	inline void OpenLibrary(lua_State* Context) {

		CreateTableFromLibrary<Library>(Context);
		lua_setglobal(Context, Library::Name);
	}
}

#endif