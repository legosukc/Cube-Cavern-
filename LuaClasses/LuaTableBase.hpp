#ifndef LUATABLEBASE_HEADER
#define LUATABLEBASE_HEADER

extern "C" {
#include <lua-5.4.2/lua.h>
}

#include "LuaContext.hpp"
#include "../LuaTypes.h"

namespace LuaClasses {

    class LuaTableBase {
    protected:

        LuaClasses::LuaContext* AttachedContext;

    public:

        LuaTableBase(LuaClasses::LuaContext* Context, int PreallocateIndexes, int PreallocateKeyElements);
        LuaTableBase();

        inline void PushOntoStack();


        template<typename IndexType, typename SetType>
        void SetElement(IndexType Index, SetType SetTo);

        template<typename SetType>
        void SetElement<const char*, SetType>(const char* Index, SetType SetTo);

        template<typename SetType>
        void SetElementWithLiteral(const char* LiteralIndex, SetType SetTo);


        template<typename IndexType>
        void SetSubtable(IndexType Index, LuaTableBase& Table);

        template<typename IndexType>
        void SetSubtable(IndexType Index, LuaTableObject Table);


        template<typename IndexType>
        LuaObject GetElement(IndexType Index);
    };
    


    // IMPLEMENTATION

    LuaTableBase::LuaTableBase(LuaClasses::LuaContext* Context, int PreallocateIndexes, int PreallocateKeyElements) {

        lua_createtable(Context->ContextObject, PreallocateIndexes, PreallocateKeyElements);
        this->AttachedContext = Context;
    }

    LuaTableBase::LuaTableBase() {
        this->AttachedContext = nullptr;
    }


    template<typename IndexType, typename SetType>
    void LuaTableBase::SetElement(IndexType Index, SetType SetTo) {

        this->PushOntoStack();

        this->AttachedContext->PushOntoStack(Index);
        this->AttachedContext->PushOntoStack(SetTo);

        lua_settable(this->AttachedContext->ContextObject, -3);
        this->AttachedContext->PopStack();
    }

    template<typename SetType>
    void LuaTableBase::SetElement<const char*, SetType>(const char* Index, SetType SetTo) {

        this->PushOntoStack();
        this->AttachedContext->PushOntoStack(SetTo);

        lua_setfield(this->AttachedContext->ContextObject, -2, Index);
        this->AttachedContext->PopStack();
    }


    template<typename SetType>
    void LuaTableBase::SetElementWithLiteral(const char* LiteralIndex, SetType SetTo) {

        this->PushOntoStack();

        this->AttachedContext->PushLiteralString(LiteralIndex);
        this->AttachedContext->PushOntoStack(SetTo);

        lua_settable(this->AttachedContext->ContextObject, -3);
        this->AttachedContext->PopStack();
    }


    template<typename IndexType>
    void LuaTableBase::SetSubtable(IndexType Index, LuaTableBase& Table) {

        this->PushOntoStack();

        this->AttachedContext->PushOntoStack(Index);
        Table.PushOntoStack();

        lua_settable(this->AttachedContext->ContextObject, -3);
        this->AttachedContext->PopStack();
    }

    template<typename IndexType>
    void LuaTableBase::SetSubtable(IndexType Index, LuaTableObject Table) {

        this->PushOntoStack();

        this->AttachedContext->PushOntoStack(Index);
        this->AttachedContext->PushValueOntoStack(Table);

        lua_settable(this->AttachedContext->ContextObject, -3);
        this->AttachedContext->PopStack();
    }



    template<typename IndexType>
    LuaObject LuaTableBase::GetElement(IndexType Index) {

        this->PushOntoStack();
        this->AttachedContext->PushOntoStack(Index);

        return lua_gettable(this->AttachedContext->ContextObject, -2);
    }
};

#endif