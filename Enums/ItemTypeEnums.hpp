#ifndef _ENUMS_ITEMTYPES
#define _ENUMS_ITEMTYPES

#include "EnumBase.hpp"
#include "../LuaClasses/LuaContext.hpp"

#include "../LuaHelper.hpp"

extern "C" {
#include <lua-5.4.2/lua.h>
}

namespace Enums {

	struct ItemTypes : public Enums::EnumBase {
				
		static inline const char* Name = "ItemTypes";
		//static inline const size_t ElementCount = 6;

		enum ItemTypesEnum {
			Item,
			Pickup,

			Weapon,
			Melee,
			Ranged,
			Magic,
		};

		static inline ElementType Entries[] = {
			{"Item", Item},
			{"Pickup", Pickup},

			{"Weapon", Weapon},
			{"Melee", Melee},
			{"Ranged", Ranged},
			{"Magic", Magic},
		};

		template<typename T>
		constexpr static inline bool IsValidEnum(T Enum);

		//static inline void AddToLuaTable(LuaClasses::LuaContext& Context);
	};

	template<typename T>
	constexpr bool ItemTypes::IsValidEnum(T Enum) {
		return Enum <= ItemTypes::Magic;
	}
	/*
	void ItemTypes::AddToLuaTable(LuaClasses::LuaContext& Context) {

		Context.SetTableField<lua_Integer>(-2, "Item",	 ItemTypes::Item);
		Context.SetTableField<lua_Integer>(-2, "Pickup", ItemTypes::Pickup);

		Context.SetTableField<lua_Integer>(-2, "Weapon", ItemTypes::Weapon);

		Context.SetTableField<lua_Integer>(-2, "Melee",  ItemTypes::Melee);
		Context.SetTableField<lua_Integer>(-2, "Ranged", ItemTypes::Ranged);
		Context.SetTableField<lua_Integer>(-2, "Magic",  ItemTypes::Magic);
	}*/
}

#endif