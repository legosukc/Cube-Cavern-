/*
#ifndef _GAMECLASSES_ITEMS
#define _GAMECLASSES_ITEMS

#include <vector>

#include "./ItemClasses/ItemBase.hpp"
#include "./OpenGLObjects/ObjectBase.hpp"

namespace GameClasses::Items {

	typedef ItemClasses::ItemBase<OpenGLObjects::ObjectBase> AbstractItemType;

	std::vector<AbstractItemType*> Items;

	inline void Update();
	inline void Draw();

	template<class ItemClass, class OwnerType = ItemClass::OwnerType, typename _inferred>
	ItemClass* CreateItem(OwnerType* Owner, _inferred* GLObjectParent);

	template<class ItemClass, class OwnerType = ItemClass::OwnerType>
	ItemClass* CreateItem(OwnerType* Owner);
};

#endif*/

#include <SDL2/SDL_timer.h>

#include <cstdlib>

#include <iostream>
#include <ostream>

#include <thread>
#include <queue>

#include <filesystem>

#include "LuaClasses/LuaContext.hpp"
#include "LuaClasses/LuaGlobalTable.hpp"
#include "LuaClasses/LuaTable.hpp"

#include "../LuaTypes.h"

extern "C" {
#include <lua-5.4.2/lua.h>
#include <lua-5.4.2/lauxlib.h>
}

static LuaClasses::LuaContext* CurrentContext;


namespace _Game_Items_CFunctions {

    static inline int _cdecl CreateItem(lua_State* Context) {

        if (!lua_isstring(Context, -1)) {
            return -1;
        }

        lua_getglobal(Context, "Game");

        lua_pushliteral(Context, "Items");
        lua_gettable(Context, -2);

        lua_pushliteral(Context, "CreateItem");
        lua_gettable(Context, -2);

        lua_pushvalue(Context, -4);
        lua_callk(Context, -1, -1, 0, 0);

        return 1;
    }

    static int _cdecl RemoveItem(lua_State* Context) {

        if (!lua_istable(Context, -1)) {
            return -1;
        }

        lua_pushliteral(Context, "_ItemsTableIndex");
        LuaObject ItemClassIndex = lua_gettable(Context, -2);
        if (!lua_isinteger(Context, ItemClassIndex)) {
            return -1;
        }

        lua_getglobal(Context, "Game");

        lua_pushliteral(Context, "Items");
        lua_gettable(Context, -2);

        lua_pushliteral(Context, "Items");
        lua_gettable(Context, -2);

        lua_pushvalue(Context, ItemClassIndex);
        lua_pushnil(Context);
        lua_settable(Context, -3);

        lua_pop(Context, 3);

        lua_pushliteral(Context, "_ItemsTableIndex");
        lua_pushnil(Context);
        lua_settable(Context, -3);

        lua_pushliteral(Context, "Owner");
        lua_pushnil(Context);
        lua_settable(Context, -3);

        lua_pop(Context, 2);

        return 1;
    }
}

namespace Game::Items {

    struct ItemClassEntry {

        char* Name;
        char* Source;
    };

    void ItemClassLoader(std::queue<ItemClassEntry>& ItemClassScripts, volatile bool* ThreadRunning) {

        for (const auto& File : std::filesystem::directory_iterator("ItemClasses\\")) {

            const char* ItemClassPath = File.path().string().c_str();

            /*
            if (Context.IsTable(AssetsItemClasses.GetElement(ItemClassName))) {
                continue;
            }*/

            std::cout << ItemClassPath << '\n';

            if (File.is_directory()) {
                // TODO: implement searching sub-directories (modpacks)
                continue;
            }

            ItemClassEntry ItemClass;

            ItemClass.Name = new char[File.path().filename().stem().string().size() + 1];
            std::strcpy(ItemClass.Name, File.path().filename().stem().string().c_str());

            std::ifstream ItemClassFile(ItemClassPath);

            ItemClassFile.seekg(0, std::ios::end);
            const std::streamsize ItemClassLen = ItemClassFile.tellg();
            ItemClassFile.seekg(0, std::ios::beg);

            ItemClass.Source = new char[ItemClassLen];
            ItemClassFile.read(ItemClass.Source, ItemClassLen);

            ItemClassFile.close();

            ItemClassScripts.push(std::move(ItemClass));
        }

        *ThreadRunning = false;
    }

    inline void Init(LuaClasses::LuaContext& Context, LuaClasses::LuaGlobalTable& GameTable, LuaClasses::LuaGlobalTable& AssetsTable) {

        LuaClasses::LuaTable AssetsItemClasses(&Context);

        if (auto ItemClassesDir = std::filesystem::directory_entry("ItemClasses\\"); !ItemClassesDir.is_directory()) {

            std::cerr << "::" << "FATAL" << ":: " << "::" << "ITEM CLASSES LOADING ERROR" << ":: ";
            if (ItemClassesDir.is_regular_file()) {
                std::cerr << "expected 'ItemClasses' to be a directory";
            } else if (!ItemClassesDir.exists()) {
                std::cerr << "directory 'ItemClasses' doesn't exist";
            }
            std::cerr << std::endl;

            for (char i = '6'; i > '1';) {
                std::cerr << "\rAborting in " << --i;
                SDL_Delay(1000);
            }
            exit(EXIT_FAILURE);
        }

        AssetsItemClasses.SetElement("__index", [](lua_State* Context) {

            if (!lua_isstring(Context, -1)) {

                lua_pop(Context, 2);
                lua_pushnil(Context);
                return 1;
            }

            const char* RequiringString = lua_tostring(Context, -1);

            if (lua_istable(Context, lua_gettable(Context, -2))) {
                return 1;
            }
             
            const size_t ItemClassFileNameSize = std::strlen("ItemClasses\\") + std::strlen(RequiringString) + std::strlen(".lua") + 1;
            auto ItemClassFileName = std::make_unique<char>(ItemClassFileNameSize);
            
            std::memcpy(ItemClassFileName.get(), "ItemClasses\\", std::strlen("ItemClasses\\"));
            std::memcpy(ItemClassFileName.get() + std::strlen("ItemClasses\\"), RequiringString, std::strlen(RequiringString));
            std::strcpy(ItemClassFileName.get() + ItemClassFileNameSize - sizeof(".lua"), ".lua");

            if (!std::filesystem::exists(ItemClassFileName.get())) {
                luaL_error(Context, "::ITEM CLASS REQUIRE ERROR:: ItemClass '%s' doesn't exist", RequiringString);
                return -1;
            }

            if (luaL_loadfilex(Context, ItemClassFileName.get(), 0)) {
                luaL_error(Context, "::ITEM CLASS REQUIRE ERROR:: ItemClass '%s' had an error during loading", RequiringString);
                return -1;
            }

            if (lua_pcallk(Context, 0, -1, 0, 0, 0)) {
                luaL_error(Context, "::ITEM CLASS REQUIRE ERROR:: ItemClass '%s' had an error during running", RequiringString);
                return -1;
            }

            lua_setfield(Context, -2, RequiringString);

            return 1;
            });

        /*
        {
            std::queue<ItemClassEntry> ItemClassScripts;
            volatile bool ThreadRunning = true;

            std::thread ItemClassLoader(ItemClassLoader, ItemClassScripts, &ThreadRunning);

            while (ThreadRunning) {
                
                if (ItemClassScripts.size() == 0) {
                    std::this_thread::sleep_for(std::chrono::milliseconds(500));
                    continue;
                }

                ItemClassEntry& ItemClass = ItemClassScripts.front();

                if (luaL_loadstring(Context.ContextObject, ItemClass.Source)) {
                    std::cerr << "::" << "ITEM CLASS REQUIRE ERROR" << ":: " << "ItemClass '" << ItemClass.Name << "' had an error during loading" << std::endl;
                    goto FreeItemClass;
                }

                if (lua_pcallk(Context.ContextObject, 0, -1, 0, 0, 0)) {
                    std::cerr << "::" << "ITEM CLASS REQUIRE ERROR" << ":: " << "ItemClass '" << ItemClass.Name << "' had an error during running" << std::endl;
                    goto FreeItemClass;
                }

                AssetsItemClasses.SetSubtable(ItemClass.Name, Context.GetStackTop());

            FreeItemClass:
                delete[] ItemClass.Name;
                delete[] ItemClass.Source;

                ItemClassScripts.pop();
            }

            ItemClassLoader.join();
        }*/
        
        
        for (const auto& File : std::filesystem::directory_iterator("ItemClasses\\")) {
            
            const char* ItemClassPath = File.path().string().c_str();
            const char* ItemClassName = File.path().filename().stem().string().c_str();

            if (Context.IsTable(AssetsItemClasses.GetElement(ItemClassName))) {
                continue;
            }

            std::cout << ItemClassPath << '\n';

            if (File.is_directory()) {
                // TODO: implement searching sub-directories (modpacks)
                continue;
            }

            if (luaL_loadfilex(Context.ContextObject, ItemClassPath, 0)) {
                std::cerr << "::" << "ITEM CLASS REQUIRE ERROR" << ":: " << "ItemClass '" << ItemClassName << "' had an error during loading" << std::endl;
                continue;
            }

            if (lua_pcallk(Context.ContextObject, 0, -1, 0, 0, 0)) {
                std::cerr << "::" << "ITEM CLASS REQUIRE ERROR" << ":: " << "ItemClass '" << ItemClassName << "' had an error during running" << std::endl;
                continue;
            }

            AssetsItemClasses.SetSubtable(ItemClassName, Context.GetStackTop());
        }

        //AssetsItemClasses.PushOntoStack();
        //Context.PushNilOntoStack();
        //lua_setfield(Context.ContextObject, -2, "__index");

        AssetsTable.SetSubtable("ItemClasses", AssetsItemClasses);

        luaL_dofile(Context.ContextObject, "Items.lua");

        if (!Context.IsTable(-1)) {
            std::cerr << "::" << "FATAL" << ":: " << "Failed to load " << "Items.lua" << '\n'
                << "Returned value from script isn't a table.\n"
                << "Aborting" << std::endl;
            exit(EXIT_FAILURE);
        }

        LuaClasses::LuaTable Items(Context.GetStackTop(), &Context);
        
        GameTable.SetSubtable("Items", Items);

        Items.SetElementWithLiteral("CreateItem", _Game_Items_CFunctions::CreateItem);
        Items.SetElementWithLiteral("RemoveItem", _Game_Items_CFunctions::RemoveItem);

        CurrentContext = &Context;
    }

    LuaClasses::LuaTable&& CreateItem(const char* Name) {

        CurrentContext->PushOntoStack(Name);
        _Game_Items_CFunctions::CreateItem(CurrentContext->ContextObject);

        LuaClasses::LuaTable NewItem(CurrentContext, CurrentContext->GetStackTop());

        return std::move(NewItem);
    }

    inline void RemoveItem(LuaClasses::LuaTable& ItemClass) {
        
        ItemClass.PushOntoStack();
        _Game_Items_CFunctions::RemoveItem(CurrentContext->ContextObject);
    }

    inline void RemoveItem(LuaTableObject ItemClass) {

        CurrentContext->PushValueOntoStack(ItemClass);
        _Game_Items_CFunctions::RemoveItem(CurrentContext->ContextObject);
    }

    /*
    inline void Update() {

        CurrentContext->GetGlobal("Game");

        CurrentContext->PushStringLiteral("Items");
        lua_gettable(CurrentContext->ContextObject, -2);

        CurrentContext->PushStringLiteral("Update");
        lua_gettable(CurrentContext->ContextObject, -2);

        lua_callk(CurrentContext->ContextObject, 0, -1, 0, 0);
        CurrentContext->PopStack(3);
    }*/
}