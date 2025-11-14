#ifndef _ENUMS_MAIN
#define _ENUMS_MAIN

#include "../LuaClasses/LuaContext.hpp"
#include "../LuaClasses/LuaTable.hpp"
#include "../LuaClasses/LuaGlobalTable.hpp"

#include "../LuaHelper.hpp"
#include "../LuaTypes.h"

extern "C" {
#include <lua-5.4.2/lua.h>
#include <lua-5.4.2/lualib.h>
#include <lua-5.4.2/lauxlib.h>
}

#include "ItemTypeEnums.hpp"

#include "OpenGLTypes.hpp"
#include "OpenGLShaderTypes.hpp"

#include "OpenGLComparison.hpp"
#include "OpenGLOperations.hpp"

#include "OpenGLDrawModes.hpp"
#include "OpenGLTextureFormats.hpp"
#include "OpenGLBuffers.hpp"

static struct {

	static int NewIndexError(lua_State* Context) {
		luaL_error(Context, "::ERROR:: Cannot modify an enum table, read-only.");
		return 1;
	}

	template<class Enum>
	static void AddEnumNamespace(LuaClasses::LuaContext& Context, LuaClasses::LuaGlobalTable& EnumsTable) {

		LuaClasses::LuaTable EnumSubtable(&Context, 0, Enum::ElementCount);

		Enum::AddToLuaTable(Context);

		//Context.SetTableField(EnumSubtable.TableObject, Enum::EnumCatagoryName);
		EnumsTable.SetSubtable(Enum::EnumCatagoryName, EnumSubtable);
	}

} _private;

namespace Enums {

	inline void luaopen_Enums(LuaClasses::LuaContext& Context, LuaClasses::LuaGlobalTable& EnumsTable) {

		typedef decltype(_private) Private;
		

		Private::AddEnumNamespace<Enums::ItemTypes>(Context, EnumsTable);

		Private::AddEnumNamespace<Enums::OpenGLTypes>(Context, EnumsTable);
		Private::AddEnumNamespace<Enums::OpenGLShaderTypes>(Context, EnumsTable);

		Private::AddEnumNamespace<Enums::OpenGLComparison>(Context, EnumsTable);
		Private::AddEnumNamespace<Enums::OpenGLOperations>(Context, EnumsTable);

		Private::AddEnumNamespace<Enums::OpenGLDrawModes>(Context, EnumsTable);
		Private::AddEnumNamespace<Enums::OpenGLTextureFormats>(Context, EnumsTable);
		Private::AddEnumNamespace<Enums::OpenGLBuffers>(Context, EnumsTable);


		Context.PushNewTable(0, 2);
		const LuaTableObject FreezeMetatableStackIndex = Context.GetStackTop();
		
		Context.PushNewTable();
		lua_setfield(Context.ContextObject, -2, "__metatable");

		LuaHelper::SetTableIndex(Context.ContextObject, "__newindex", Private::NewIndexError);



		EnumsTable.PushOntoStack();
		const LuaTableObject EnumsTableStackIndex = Context.GetStackTop();

		Context.PushValueOntoStack(FreezeMetatableStackIndex);
		lua_setmetatable(Context.ContextObject, -2);

		for (auto Element = EnumsTable.GetIterator(); Element.Running; ++Element) {
			
			if (!Context.IsTable(Element.Value)) {
				Context.PopStack(1);
				continue;
			}
			
			Context.PushValueOntoStack(EnumsTableStackIndex);
			Context.PushValueOntoStack(FreezeMetatableStackIndex);
			lua_setmetatable(Context.ContextObject, -2);

			Context.PopStack(2);
		}
		Context.PopStack(4);
	}
}

#endif