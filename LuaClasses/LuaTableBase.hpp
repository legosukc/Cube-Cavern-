#ifndef LUATABLEBASE_HEADER
#define LUATABLEBASE_HEADER

extern "C" {
#include <lua-5.4.2/lua.h>
}

#include "../LuaHelper.hpp"

#include "LuaContext.hpp"
#include "../LuaTypes.h"

namespace LuaClasses {

    class LuaTableBase {

        class Iterator {

            const LuaTableBase* Table;

        public:
            Iterator(const LuaTableBase* TablePtr);

            int Key;
            int Value;
            bool Running;

            // Prefix increment
            const Iterator& operator++();
        };
        friend struct Iterator;

    protected:

        LuaClasses::LuaContext* AttachedContext;

    public:

        LuaTableBase(LuaClasses::LuaContext* Context, int PreallocateIndexes, int PreallocateKeyElements);
        inline LuaTableBase();

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

        inline Iterator GetIterator() const;
    };



    // IMPLEMENTATION

    LuaTableBase::Iterator::Iterator(const LuaTableBase* TablePtr) {

        this->Table = TablePtr;

        TablePtr->AttachedContext->PushNilOntoStack();
        this->Key = TablePtr->AttachedContext->GetStackTop();
        this->Value = this->Key;

        this->Running = true;
    }

    // Prefix increment
    const LuaTableBase::Iterator& LuaTableBase::Iterator::operator++() {

        this->Running = lua_next(this->Table->AttachedContext->ContextObject, this->Key) != 0;

        this->Key = lua_absindex(this->Table->AttachedContext->ContextObject, -2);
        this->Value = this->Table->AttachedContext->GetStackTop();

        return *this;
    }



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

    LuaTableBase::Iterator LuaTableBase::GetIterator() const {
        return LuaTableBase::Iterator(this);
    }
}

#endif