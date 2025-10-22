#ifndef LUAGLOBALTABLE_HEADER
#define LUAGLOBALTABLE_HEADER

#include "LuaTableBase.hpp"

extern "C" {
#include <lua-5.4.2/lua.h>
}

#include "LuaContext.hpp"

namespace LuaClasses {

    class LuaGlobalTable : public LuaTableBase {

        const char* TableName;

    public:

        LuaGlobalTable(LuaClasses::LuaContext* Context, const char* GlobalName, int PreallocateIndexes = 0, int PreallocateKeyElements = 0);

        inline void PushOntoStack();
    };



    // IMPLEMENTATION

    LuaGlobalTable::LuaGlobalTable(LuaClasses::LuaContext* Context, const char* GlobalName, int PreallocateIndexes, int PreallocateKeyElements) : LuaTableBase(Context, PreallocateIndexes, PreallocateKeyElements) {

        lua_setglobal(Context->ContextObject, GlobalName);
        this->TableName = GlobalName;
    }

    void LuaGlobalTable::PushOntoStack() {
        this->AttachedContext->GetGlobal(this->TableName);
    }
};

#endif