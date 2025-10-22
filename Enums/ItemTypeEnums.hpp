#ifndef _ENUMS_ITEMTYPES
#define _ENUMS_ITEMTYPES

extern "C" {
#include <lua-5.4.2/lua.h>
}

#include "../LuaClasses/LuaContext.hpp"
#include "../LuaClasses/LuaTable.hpp"
#include "../LuaClasses/LuaGlobalTable.hpp"

namespace Enums::ItemTypes {

	enum ItemTypesEnum {
		Item,
		Pickup,

		Weapon,
		Melee,
		Ranged,
		Magic,
	};

	inline void AddToLuaTable(LuaClasses::LuaContext& Context, LuaClasses::LuaGlobalTable& EnumsTable) {

		LuaClasses::LuaTable EnumsItemTypesTable(&Context, 0, 6);

		EnumsItemTypesTable.SetElementWithLiteral("Item", Enums::ItemTypes::Item);
		EnumsItemTypesTable.SetElementWithLiteral("Pickup", Enums::ItemTypes::Pickup);

		EnumsItemTypesTable.SetElementWithLiteral("Weapon", Enums::ItemTypes::Weapon);
		EnumsItemTypesTable.SetElementWithLiteral("Melee", Enums::ItemTypes::Melee);
		EnumsItemTypesTable.SetElementWithLiteral("Ranged", Enums::ItemTypes::Ranged);
		EnumsItemTypesTable.SetElementWithLiteral("Magic", Enums::ItemTypes::Magic);

		EnumsTable.SetSubtable("ItemTypes", EnumsItemTypesTable);
	}
}

#endif