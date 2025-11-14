#ifndef LUACONTEXT_HEADER
#define LUACONTEXT_HEADER

extern "C" {
#include <lua-5.4.2/lua.h>
#include <lua-5.4.2/lualib.h>
#include <lua-5.4.2/lauxlib.h>
}

#include "../LuaHelper.hpp"
#include "../LuaTypes.h"

#include <iostream>

namespace LuaClasses {

    class LuaContext {
    public:

        inline LuaContext();
        inline LuaContext(lua_State* Context);

        inline ~LuaContext();

        lua_State* ContextObject;

        inline operator lua_State*();

        // Lua Stack Helpers

        inline int GetStackSize() const;
        inline LuaObject GetStackTop() const;

        inline void PrintStackSize() const;


        inline void PushOntoStack(const char* Value);
        inline void PushOntoStack(lua_Integer Value);
        inline void PushOntoStack(lua_Number Value);
        inline void PushOntoStack(lua_CFunction Function);
        inline void PushOntoStack(void* Userdata);

        inline void PushStringLiteral(const char* String);

        inline void PushValueOntoStack(LuaObject Value);

        inline void PushNilOntoStack();

        inline void ShrinkStackTo(int ShrinkToIndex);
        constexpr inline void PopStack(int PopOffAmount = 1);



        // Global Lua Helpers

        inline LuaObject GetGlobal(const char* GlobalName) const;

        inline void SetGlobal(const char* GlobalName);

        template<typename SetType>
        inline void SetGlobal(SetType Value, const char* GlobalName);


        inline void PushDefinedMetatable(const char* MetatableName);


        // Get Lua Object Helpers

        inline const char* GetString(LuaStringObject Object) const;

        inline int GetBoolean(LuaBoolObject Object) const;

        inline lua_Integer GetInteger(LuaIntObject Object) const;

        inline lua_Number GetNumber(LuaNumberObject Object) const;

        inline lua_CFunction GetCFunction(LuaCFunctionObject Object) const;

        inline LuaContext GetThread(LuaThreadObject Object);
        inline const LuaContext GetThread(LuaThreadObject Object) const;

        inline void* GetUserdata(LuaUserdataObject Object);
        inline const void* GetUserdata(LuaUserdataObject Object) const;

        inline const void* GetPointer(LuaPointerObject Object) const;


        // Types

        inline int GetTypeEnum(LuaObject StackIndex) const;
        inline const char* GetNameFromTypeEnum(int TypeEnum) const;

        inline const char* GetTypename(LuaObject StackIndex) const;

        // Type Checking

        inline bool IsBool(LuaObject Object) const;
        inline bool IsInteger(LuaObject Object) const;
        inline bool IsNumber(LuaObject Object) const;

        inline bool IsString(LuaObject Object) const;

        inline bool IsFunction(LuaObject Object) const;

        inline bool IsTable(LuaObject Object) const;

        inline bool IsUserdata(LuaObject Object) const;

        void* CheckUserdataMetatable(const char* MetatableName, const char* Error) const;
        inline void* CheckUserdataMetatable(LuaUserdataObject UserdataObject, const char* MetatableName, const char* Error);


        // Creation

        inline void PushNewTable(int AllocateIndices = 0, int AllocateKeys = 0);

        inline void* PushNewUserdata(size_t UserdataSize);


        // Table Functions

        // The value at the top of the stack is what gets assigned to the field.
        inline void SetTableField(LuaTableObject TableStackIndex, const char* Field);

        template<typename T>
        inline void SetTableField(LuaTableObject TableStackIndex, const char* Field, T SetTo);


        // The value at the top of the stack is what gets assigned to the index.
        inline void SetTableIndex(LuaTableObject TableStackIndex, lua_Integer Index);

        template<typename T>
        inline void SetTableIndex(LuaTableObject TableStackIndex, lua_Integer Index, T SetTo);

        /*
        * Uses the lua stack for the parameters.
        * The 2 values after the table in the lua stack are used as the following:
        * 1st: (TableStackIndex) - must be a table or a userdata with a metatable containing a "__newindex" metamethod.
        * 2nd: Index
        * 3rd: Assignment
        * Pops every value after the table off the lua stack.
        */
        inline void SetTableElement(LuaTableObject TableStackIndex);

        // Puts the value from the index at the top of the lua stack.
        inline LuaObject GetTableField(LuaTableObject TableStackIndex, const char* Field) const;

        // Puts the value from the index at the top of the lua stack.
        inline LuaObject GetTableIndex(LuaTableObject TableStackIndex, lua_Integer Index) const;


        /*
        * Uses the lua stack for the parameters.
        * The value after the table in the lua stack is used as the index.
        * Pops every value after the table off the lua stack.
        */
        inline LuaObject GetTableElement(LuaTableObject TableStackIndex) const;


        // Other

        inline void PrintLuaError(const char* ErrorCatagory) const;

        inline void LuaCFunctionError(const char* Error);
    };



    // IMPLEMENTATION

    LuaContext::LuaContext() {
        this->ContextObject = luaL_newstate();
        luaL_openlibs(this->ContextObject);

        luaopen_base(this->ContextObject);
        luaopen_table(this->ContextObject);
        //luaopen_io(this->ContextObject);
        luaopen_string(this->ContextObject);
        luaopen_math(this->ContextObject);
    };

    LuaContext::LuaContext(lua_State* Context) {
        this->ContextObject = Context;
    }

    LuaContext::~LuaContext() {
        lua_close(this->ContextObject);
    };


    LuaContext::operator lua_State*() {
        return this->ContextObject;
    }


    // Lua Stack Helpers

    int LuaContext::GetStackSize() const {
        return lua_gettop(this->ContextObject);
    }
    LuaObject LuaContext::GetStackTop() const {
        return lua_gettop(this->ContextObject);
    }

    void LuaContext::PrintStackSize() const {
        std::cout << "Lua Stacksize: " << this->GetStackSize() << '\n';
    }


    void LuaContext::PushOntoStack(const char* String) {
        lua_pushstring(this->ContextObject, String);
    }

    void LuaContext::PushOntoStack(lua_Integer Integer) {
        lua_pushinteger(this->ContextObject, Integer);
    }

    void LuaContext::PushOntoStack(lua_Number Float) {
        lua_pushnumber(this->ContextObject, Float);
    }

    void LuaContext::PushOntoStack(lua_CFunction Function) {
        lua_pushcfunction(this->ContextObject, Function);
    }

    void LuaContext::PushOntoStack(void* Userdata) {
        lua_pushlightuserdata(this->ContextObject, Userdata);
    }

    void LuaContext::PushStringLiteral(const char* StringLiteral) {
        //lua_pushliteral(this->ContextObject, String);
        this->PushOntoStack(StringLiteral);
    }

    void LuaContext::PushNilOntoStack() {
        lua_pushnil(this->ContextObject);
    }

    void LuaContext::PushValueOntoStack(LuaObject Value) {
        lua_pushvalue(this->ContextObject, Value);
    }


    void LuaContext::ShrinkStackTo(int ShrinkToIndex) {
        lua_settop(this->ContextObject, ShrinkToIndex);
    }

    constexpr void LuaContext::PopStack(int PopOffAmount) {
        this->ShrinkStackTo(-PopOffAmount - 1);
    }



    // Global Lua Helpers

    LuaObject LuaContext::GetGlobal(const char* GlobalName) const {
        return lua_getglobal(this->ContextObject, GlobalName);
    }

    void LuaContext::SetGlobal(const char* GlobalName) {
        lua_setglobal(this->ContextObject, GlobalName);
    }

    template<typename SetType>
    void LuaContext::SetGlobal(SetType Value, const char* GlobalName) {
        this->PushOntoStack(Value);
        this->SetGlobal(GlobalName);
    }

    void LuaContext::PushDefinedMetatable(const char* MetatableName) {
        luaL_getmetatable(this->ContextObject, MetatableName);
    }


    // Get Lua Object Helpers

    const char* LuaContext::GetString(LuaStringObject Object) const {
        return lua_tostring(this->ContextObject, Object);
    }

    int LuaContext::GetBoolean(LuaBoolObject Object) const {
        return lua_toboolean(this->ContextObject, Object);
    }

    lua_Integer LuaContext::GetInteger(LuaIntObject Object) const {
        return lua_tointeger(this->ContextObject, Object);
    }

    lua_Number LuaContext::GetNumber(LuaNumberObject Object) const {
        return lua_tonumber(this->ContextObject, Object);
    }

    lua_CFunction LuaContext::GetCFunction(LuaCFunctionObject Object) const {
        return lua_tocfunction(this->ContextObject, Object);
    }

    LuaContext LuaContext::GetThread(LuaThreadObject Object) {
        return LuaContext(lua_tothread(this->ContextObject, Object));
    }
    const LuaContext LuaContext::GetThread(LuaThreadObject Object) const {
        return const LuaContext(lua_tothread(this->ContextObject, Object));
    }

    void* LuaContext::GetUserdata(LuaUserdataObject Object) {
        return lua_touserdata(this->ContextObject, Object);
    }
    const void* LuaContext::GetUserdata(LuaUserdataObject Object) const {
        return const_cast<const void*>(lua_touserdata(this->ContextObject, Object));
    }

    const void* LuaContext::GetPointer(LuaPointerObject Object) const {
        return lua_topointer(this->ContextObject, Object);
    }



    // Types

    int LuaContext::GetTypeEnum(LuaObject StackIndex) const {
        return lua_type(this->ContextObject, StackIndex);
    }
    const char* LuaContext::GetNameFromTypeEnum(int TypeEnum) const {
        return lua_typename(this->ContextObject, TypeEnum);
    }

    const char* LuaContext::GetTypename(LuaObject StackIndex) const {
        return this->GetNameFromTypeEnum(this->GetTypeEnum(StackIndex));
    }


    // Type Checking

    bool LuaContext::IsBool(LuaObject Object) const {
        return lua_isboolean(this->ContextObject, Object);
    }

    bool LuaContext::IsInteger(LuaObject Object) const {
        return lua_isinteger(this->ContextObject, Object);
    }

    bool LuaContext::IsNumber(LuaObject Object) const {
        return lua_isnumber(this->ContextObject, Object);
    }

    bool LuaContext::IsString(LuaObject Object) const {
        return lua_isstring(this->ContextObject, Object);
    }

    bool LuaContext::IsFunction(LuaObject Object) const {
        return lua_isfunction(this->ContextObject, Object);
    }

    bool LuaContext::IsTable(LuaObject Object) const {
        return lua_istable(this->ContextObject, Object);
    }

    bool LuaContext::IsUserdata(LuaObject Object) const {
        return lua_isuserdata(this->ContextObject, Object);
    }

    void* LuaContext::CheckUserdataMetatable(const char* MetatableName, const char* Error) const {

        void* Userdata = luaL_checkudata(this->ContextObject, 1, MetatableName);
        luaL_argcheck(this->ContextObject, Userdata != NULL, 1, Error);
        return Userdata;
    }

    void* LuaContext::CheckUserdataMetatable(LuaUserdataObject UserdataObject, const char* MetatableName, const char* Error) {
        this->PushValueOntoStack(UserdataObject);
        return this->CheckUserdataMetatable(MetatableName, Error);
    }


    // Creation

    void LuaContext::PushNewTable(int AllocateIndices, int AllocateKeys) {
        lua_createtable(this->ContextObject, AllocateIndices, AllocateKeys);
    }

    void* LuaContext::PushNewUserdata(size_t UserdataSize) {
        return lua_newuserdata(this->ContextObject, UserdataSize);
    }



    // Table Functions

    // The value at the top of the stack is what gets assigned to the field.
    void LuaContext::SetTableField(LuaTableObject TableStackIndex, const char* Field) {
        lua_setfield(this->ContextObject, TableStackIndex, Field);
    }

    template<typename T>
    void LuaContext::SetTableField(LuaTableObject TableStackIndex, const char* Field, T SetTo) {
        this->PushOntoStack(SetTo);
        this->SetTableField(TableStackIndex, Field);
    }


    // The value at the top of the stack is what gets assigned to the index.
    void LuaContext::SetTableIndex(LuaTableObject TableStackIndex, lua_Integer Index) {
        lua_seti(this->ContextObject, TableStackIndex, Index);
    }

    template<typename T>
    void LuaContext::SetTableIndex(LuaTableObject TableStackIndex, lua_Integer Index, T SetTo) {
        this->PushOntoStack(SetTo);
        this->SetTableIndex(TableStackIndex, Index);
    }

    /*
    * Uses the lua stack for the parameters.
    * The 2 values after the table in the lua stack are used as the following:
    * 1st: (TableStackIndex) - must be a table or a userdata with a metatable containing a "__newindex" metamethod.
    * 2nd: Index
    * 3rd: Assignment
    * Pops every value after the table off the lua stack.
    */
    void LuaContext::SetTableElement(LuaTableObject TableStackIndex) {
        lua_settable(this->ContextObject, TableStackIndex);
    }

    // Puts the value from the index at the top of the lua stack.
    LuaObject LuaContext::GetTableField(LuaTableObject TableStackIndex, const char* Field) const {
        return lua_getfield(this->ContextObject, TableStackIndex, Field);
    }

    // Puts the value from the index at the top of the lua stack.
    LuaObject LuaContext::GetTableIndex(LuaTableObject TableStackIndex, lua_Integer Index) const {
        return lua_geti(this->ContextObject, TableStackIndex, Index);
    }


    /*
    * Uses the lua stack for the parameters.
    * The value after the table in the lua stack is used as the index.
    * Pops every value after the table off the lua stack.
    */
    LuaObject LuaContext::GetTableElement(LuaTableObject TableStackIndex) const {
        return lua_gettable(this->ContextObject, TableStackIndex);
    }


    // Other

    void LuaContext::PrintLuaError(const char* ErrorCatagory) const {
        std::cerr << "::" << ErrorCatagory << ":: " << this->GetString(-1) << std::endl;
    }

    void LuaContext::LuaCFunctionError(const char* Error) {

        this->PushOntoStack(Error);
        lua_error(this->ContextObject);
    }
};

#endif