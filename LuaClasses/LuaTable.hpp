#ifndef LUATABLE_HEADER
#define LUATABLE_HEADER

#include "LuaTableBase.hpp"

#include "LuaContext.hpp"
#include "../LuaTypes.h"

namespace LuaClasses {

    class LuaTable : public LuaTableBase {
    public:

        inline LuaTable(LuaClasses::LuaContext* Context, int PreallocateIndexes = 0, int PreallocateKeyElements = 0);
        inline LuaTable(LuaTableObject Table, LuaClasses::LuaContext* Context);

        inline void PushOntoStack();

        LuaTableObject TableObject;
    };



    // IMPLEMENTATION

    LuaTable::LuaTable(LuaClasses::LuaContext* Context, int PreallocateIndexes, int PreallocateKeyElements) : LuaTableBase(Context, PreallocateIndexes, PreallocateKeyElements) {
        this->TableObject = Context->GetStackTop();
    }

    LuaTable::LuaTable(LuaTableObject Table, LuaClasses::LuaContext* Context) {

        this->AttachedContext = Context;

        if (!Context->IsTable(Table)) {
            std::cerr << "not a fucking table" << std::endl
                << "line: " << __LINE__ << std::endl
                << "file: " << __FILE__ << std::endl;
            exit(EXIT_FAILURE);
        }

        this->TableObject = Table;
    }

    void LuaTable::PushOntoStack() {
        this->AttachedContext->PushValueOntoStack(this->TableObject);
    }
};

#endif