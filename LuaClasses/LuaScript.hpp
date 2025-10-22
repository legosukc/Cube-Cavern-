#ifndef LUASCRIPT_HEADER
#define LUASCRIPT_HEADER

extern "C" {
#include <lua-5.4.2/lua.h>
#include <lua-5.4.2/lauxlib.h>
}

#include "LuaContext.hpp"

#include <iostream>

namespace LuaClasses {

    class LuaScript {

        LuaClasses::LuaContext* AttachedContext;
        int StackIndex;

    public:

        LuaScript(LuaClasses::LuaContext* Context, const char* ScriptFile);

        // Returns a stack index to where the script started running.
        int RunScript();

        // Returns a stack index to where the script started running.
        int ProtectedRunScript();
    };



    // IMPLEMENTATION

    LuaScript::LuaScript(LuaClasses::LuaContext* Context, const char* ScriptFile) {

        luaL_loadfilex(Context->ContextObject, ScriptFile, 0);
        this->StackIndex = Context->GetStackTop();

        this->AttachedContext = Context;
    }

    // Returns a stack index to where the script started running.
    int LuaScript::RunScript() {

        this->AttachedContext->PushValueOntoStack(this->StackIndex);
        const int StackSize = this->AttachedContext->GetStackSize();

        lua_callk(this->AttachedContext->ContextObject, 0, -1, 0, 0);

        return StackSize;
    }

    // Returns a stack index to where the script started running.
    int LuaScript::ProtectedRunScript() {

        this->AttachedContext->PushValueOntoStack(this->StackIndex);
        const int StackSize = this->AttachedContext->GetStackSize();

        if (lua_pcallk(this->AttachedContext->ContextObject, 0, -1, 0, 0, 0)) {
            std::cerr << "::LUA SCRIPT PROTECTED RUN ERROR:: " << this->AttachedContext->GetString(-1) << std::endl;
        }

        return StackSize;
    }
};

#endif