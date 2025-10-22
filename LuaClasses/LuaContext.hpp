#ifndef LUACONTEXT_HEADER
#define LUACONTEXT_HEADER

extern "C" {
#include <lua-5.4.2/lua.h>
#include <lua-5.4.2/lualib.h>
#include <lua-5.4.2/lauxlib.h>
}

#include "../LuaTypes.h"

#include <iostream>

namespace LuaClasses {

    class LuaContext {
    public:

        inline LuaContext();
        inline ~LuaContext();

        lua_State* ContextObject;


        // Lua Stack Helpers

        inline int GetStackSize();
        inline LuaObject GetStackTop();

        inline void PrintStackSize();


        inline void PushOntoStack(const char* Value);
        inline void PushOntoStack(lua_Integer Value);
        inline void PushOntoStack(lua_Number Value);
        inline void PushOntoStack(lua_CFunction Function);
        inline void PushOntoStack(void* Userdata);

        inline void PushStringLiteral(const char* String);

        inline void PushValueOntoStack(LuaObject Value);

        inline void PushNilOntoStack();


        inline void PopStack(int PopOffAmount = 1);



        // Global Lua Helpers

        inline LuaObject GetGlobal(const char* GlobalName) const;

        template<typename SetType>
        inline void SetGlobal(SetType Value, const char* GlobalName);


        inline void PushDefinedMetatable(const char* MetatableName);


        // Get Lua Object Helpers

        inline const char* GetString(LuaStringObject Object) const;

        inline int GetBoolean(LuaBoolObject Object) const;

        inline lua_Integer GetInteger(LuaIntObject Object) const;

        inline lua_Number GetNumber(LuaNumberObject Object) const;

        inline lua_CFunction GetCFunction(LuaCFunctionObject Object) const;

        inline lua_State* GetThread(LuaThreadObject Object) const;

        inline void* GetUserdata(LuaUserdataObject Object) const;

        inline const void* GetPointer(LuaPointerObject Object) const;



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

        inline void PushNewTable();

        inline void* PushNewUserdata(size_t UserdataSize);


        // Other

        inline void PrintLuaError(const char* ErrorCatagory) const;

        inline void LuaCFunctionError(const char* Error);
    };



    // IMPLEMENTATION

    LuaContext::LuaContext() {
        this->ContextObject = luaL_newstate();
        luaL_openlibs(this->ContextObject);

        luaopen_base(this->ContextObject);             /* opens the basic library */
        luaopen_table(this->ContextObject);            /* opens the table library */
        luaopen_io(this->ContextObject);               /* opens the I/O library */
        luaopen_string(this->ContextObject);           /* opens the string lib. */
        luaopen_math(this->ContextObject);             /* opens the math lib. */
    };

    LuaContext::~LuaContext() {
        lua_close(this->ContextObject);
    };


    //inline lua_State*(LuaContext Context) {
        //return Context->ContextObject;
    //}



    // Lua Stack Helpers

    int LuaContext::GetStackSize() {
        return lua_gettop(this->ContextObject);
    }
    LuaObject LuaContext::GetStackTop() {
        return lua_gettop(this->ContextObject);
    }

    void LuaContext::PrintStackSize() {
        std::cout << "Lua Stacksize: " << this->GetStackSize() << '\n';
    }


    void LuaContext::PushOntoStack(const char* Value) {
        lua_pushstring(this->ContextObject, Value);
    }

    void LuaContext::PushOntoStack(lua_Integer Value) {
        lua_pushinteger(this->ContextObject, Value);
    }

    void LuaContext::PushOntoStack(lua_Number Value) {
        lua_pushnumber(this->ContextObject, Value);
    }

    void LuaContext::PushOntoStack(lua_CFunction Function) {
        lua_pushcfunction(this->ContextObject, Function);
    }

    void LuaContext::PushOntoStack(void* Userdata) {
        lua_pushlightuserdata(this->ContextObject, Userdata);
    }

    void LuaContext::PushStringLiteral(const char* String) {
        lua_pushliteral(this->ContextObject, String);
    }

    void LuaContext::PushNilOntoStack() {
        lua_pushnil(this->ContextObject);
    }

    void LuaContext::PushValueOntoStack(LuaObject Value) {
        lua_pushvalue(this->ContextObject, Value);
    }


    void LuaContext::PopStack(int PopOffAmount) {
        lua_pop(this->ContextObject, PopOffAmount);
    }



    // Global Lua Helpers

    LuaObject LuaContext::GetGlobal(const char* GlobalName) const {
        return lua_getglobal(this->ContextObject, GlobalName);
    }

    template<typename SetType>
    void LuaContext::SetGlobal(SetType Value, const char* GlobalName) {
        this->PushOntoStack(Value);
        lua_setglobal(this->ContextObject, GlobalName);
        this->PopStack();
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

    lua_State* LuaContext::GetThread(LuaThreadObject Object) const {
        return lua_tothread(this->ContextObject, Object);
    }

    void* LuaContext::GetUserdata(LuaUserdataObject Object) const {
        return lua_touserdata(this->ContextObject, Object);
    }

    const void* LuaContext::GetPointer(LuaPointerObject Object) const {
        return lua_topointer(this->ContextObject, Object);
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

    void LuaContext::PushNewTable() {
        lua_createtable(this->ContextObject, 0, 0);
    }

    void* LuaContext::PushNewUserdata(size_t UserdataSize) {
        return lua_newuserdata(this->ContextObject, UserdataSize);
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