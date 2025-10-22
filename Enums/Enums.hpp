#ifndef _ENUMS_MAIN
#define _ENUMS_MAIN

#include "../LuaClasses/LuaContext.hpp"
#include "../LuaClasses/LuaGlobalTable.hpp"

#include "ItemTypeEnums.hpp"

namespace Enums {

	inline void luaopen_Enums(LuaClasses::LuaContext& Context, LuaClasses::LuaGlobalTable& EnumsTable) {
		Enums::ItemTypes::AddToLuaTable(Context, EnumsTable);
	}
}

#endif