#ifndef _ENUMS_ENUMBASE
#define _ENUMS_ENUMBASE

#include "../LuaClasses/LuaContext.hpp"

#include "../LuaHelper.hpp"

extern "C" {
#include <lua-5.4.2/lua.h>
}

namespace Enums {

	struct EnumBase : public LuaHelper::Table::LiteralTable<LuaHelper::Table::KeyValue<lua_Integer>> {

		static inline const char* Name;
//		static inline const size_t ElementCount = 0;
		
		template<typename T>
		constexpr static inline bool IsValidEnum(T Enum);
	};
}

#endif