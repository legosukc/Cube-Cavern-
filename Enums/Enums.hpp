#ifndef _ENUMS_MAIN
#define _ENUMS_MAIN

#include "../LuaClasses/LuaContext.hpp"
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

} _private;

namespace Enums {

	inline void luaopen_Enums(LuaClasses::LuaContext& Context, LuaClasses::LuaGlobalTable& EnumsTable) {

		Enums::OpenGL::

		Enums::ItemTypes::AddToLuaTable(Context, EnumsTable);

		Enums::OpenGLTypes::AddToLuaTable(Context, EnumsTable);
		Enums::OpenGLShaderTypes::AddToLuaTable(Context, EnumsTable);

		Enums::OpenGLComparison::AddToLuaTable(Context, EnumsTable);
		Enums::OpenGLOperations::AddToLuaTable(Context, EnumsTable);

		Enums::OpenGLDrawModes::AddToLuaTable(Context, EnumsTable);
		Enums::OpenGLTextureFormats::AddToLuaTable(Context, EnumsTable);
		Enums::OpenGLBuffers::AddToLuaTable(Context, EnumsTable);


		
		lua_createtable(Context.ContextObject, 0, 2);
		const LuaTableObject FreezeMetatableStackIndex = Context.GetStackTop();
		
		lua_newtable(Context.ContextObject);
		lua_setfield(Context.ContextObject, -2, "__metatable");

		LuaHelper::SetTableIndex(Context.ContextObject, "__newindex", _private.NewIndexError);



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