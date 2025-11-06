#ifndef LUAUPVALUE_H
#define LUAUPVALUE_H

extern "C" {
#include <lua-5.4.2/lua.h>
}

#include "LuaContext.hpp"
#include "../LuaTypes.h"

#include <map>

namespace LuaClasses {

    class LuaUpvalue {

        LuaClasses::LuaContext* AttachedContext;

        static std::map<LuaFunctionObject, int> UpvalueCount;

        LuaFunctionObject UpvalueLuaFunction;
        int UpvalueIndex;

    public:

        LuaUpvalue(LuaClasses::LuaContext* Context, LuaFunctionObject LuaFunctionIndex);
        
        inline void SetUpvalue();

        template<typename T>
        void SetUpvalue(T Value);

        inline void PushUpvalueOntoStack();
    };
    


    // IMPLEMENTATION

    LuaUpvalue::LuaUpvalue(LuaClasses::LuaContext* Context, LuaFunctionObject LuaFunctionIndex) {
        
        this->AttachedContext = Context;
        this->UpvalueLuaFunction = lua_absindex(Context->ContextObject, (int)LuaFunctionIndex);

        if (!this->UpvalueCount.contains(this->UpvalueLuaFunction)) {

            this->UpvalueCount.insert({ this->UpvalueLuaFunction, 0 });
            this->UpvalueIndex = 0;
        } else {
            this->UpvalueIndex = ++this->UpvalueCount[this->UpvalueLuaFunction];
        }
    }

    void LuaUpvalue::SetUpvalue() {
        lua_setupvalue(this->AttachedContext->ContextObject, this->UpvalueLuaFunction, this->UpvalueIndex);
    }

    template<typename T>
    void LuaUpvalue::SetUpvalue(T Value) {

        this->AttachedContext->PushOntoStack(Value);
        this->SetUpvalue();
    }

    void LuaUpvalue::PushUpvalueOntoStack() {
        lua_getupvalue(this->AttachedContext->ContextObject, this->UpvalueLuaFunction, this->UpvalueIndex);
    }
};

#endif