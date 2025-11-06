
#ifndef _GAMECLASSES_ENEMIES
#define _GAMECLASSES_ENEMIES

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


namespace _private::Game::Enemies::CFunctions {

    static inline int _cdecl CreateEnemy(lua_State* Context) {

        lua_getglobal(Context, "Game");

        lua_pushliteral(Context, "Enemies");
        lua_gettable(Context, -2);

        lua_pushliteral(Context, "CreateEnemy");
        lua_gettable(Context, -2);

        lua_pushvalue(Context, -4);
        lua_callk(Context, -1, -1, 0, 0);

        return 1;
    }

    static int _cdecl RemoveEnemy(lua_State* Context) {

        if (!lua_istable(Context, -1)) {
            return -1;
        }

        lua_pushliteral(Context, "_EnemiesTableIndex");
        LuaObject EnemyClassIndex = lua_gettable(Context, -2);
        if (!lua_isinteger(Context, EnemyClassIndex)) {
            return -1;
        }

        lua_getglobal(Context, "Game");

        lua_pushliteral(Context, "Enemies");
        lua_gettable(Context, -2);

        lua_pushliteral(Context, "Enemies");
        lua_gettable(Context, -2);

        lua_pushvalue(Context, EnemyClassIndex);
        lua_pushnil(Context);
        lua_settable(Context, -3);

        lua_pop(Context, 3);

        lua_pushliteral(Context, "_EnemiesTableIndex");
        lua_pushnil(Context);
        lua_settable(Context, -3);

        lua_pop(Context, 2);

        return 1;
    }
}

namespace Game::Enemies {

    /*
    struct ItemClassEntry {

        char* Name;
        char* Source;
    };

    void ItemClassLoader(std::queue<ItemClassEntry>& ItemClassScripts, volatile bool* ThreadRunning) {

        for (const auto& File : std::filesystem::directory_iterator("ItemClasses\\")) {

            const char* ItemClassPath = File.path().string().c_str();

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
    }*/

    inline void Init(LuaClasses::LuaContext& Context, LuaClasses::LuaGlobalTable& GameTable, LuaClasses::LuaGlobalTable& AssetsTable) {

        LuaClasses::LuaTable AssetsEnemyClasses(&Context);

        if (auto EnemyClassesDir = std::filesystem::directory_entry("EnemyClasses\\"); !EnemyClassesDir.is_directory()) {

            std::cerr << "::" << "FATAL" << ":: " << "::" << "ENEMY CLASSES LOADING ERROR" << ":: ";
            if (EnemyClassesDir.is_regular_file()) {
                std::cerr << "expected 'EnemyClasses' to be a directory";
            } else if (!EnemyClassesDir.exists()) {
                std::cerr << "directory 'EnemyClasses' doesn't exist";
            }
            std::cerr << std::endl;

            for (char i = '6'; i > '1';) {
                std::cerr << "\rAborting in " << --i;
                SDL_Delay(1000);
            }
            exit(EXIT_FAILURE);
        }

        AssetsEnemyClasses.SetElement("__index", [](lua_State* Context) {

            if (!lua_isstring(Context, -1)) {

                lua_pop(Context, 2);
                lua_pushnil(Context);
                return 1;
            }

            const char* RequiringString = lua_tostring(Context, -1);

            if (lua_istable(Context, lua_gettable(Context, -2))) {
                return 1;
            }
             
            const size_t ClassFileNameSize = std::strlen("EnemyClasses\\") + std::strlen(RequiringString) + std::strlen(".lua") + 1;
            auto ClassFileName = std::make_unique<char>(ClassFileNameSize);
            
            std::memcpy(ClassFileName.get(), "EnemyClasses\\", std::strlen("EnemyClasses\\"));
            std::memcpy(ClassFileName.get() + std::strlen("EnemyClasses\\"), RequiringString, std::strlen(RequiringString));
            std::strcpy(ClassFileName.get() + ClassFileNameSize - sizeof(".lua"), ".lua");

            if (!std::filesystem::exists(ClassFileName.get())) {
                luaL_error(Context, "::ENEMY CLASS REQUIRE ERROR:: EnemyClass '%s' doesn't exist", RequiringString);
                return -1;
            }

            if (luaL_loadfilex(Context, ClassFileName.get(), 0)) {
                luaL_error(Context, "::ENEMY CLASS REQUIRE ERROR:: EnemyClass '%s' had an error during loading", RequiringString);
                return -1;
            }

            if (lua_pcallk(Context, 0, -1, 0, 0, 0)) {
                luaL_error(Context, "::ENEMY CLASS REQUIRE ERROR:: EnemyClass '%s' had an error during running", RequiringString);
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
        
        
        for (const auto& File : std::filesystem::directory_iterator("EnemyClasses\\")) {
            
            const char* ClassPath = File.path().string().c_str();
            const char* ClassName = File.path().filename().stem().string().c_str();

            if (Context.IsTable(AssetsEnemyClasses.GetElement(ClassName))) {
                continue;
            }

            std::cout << ClassPath << '\n';

            if (File.is_directory()) {
                // TODO: implement searching sub-directories (modpacks)
                continue;
            }

            if (luaL_loadfilex(Context.ContextObject, ClassPath, 0)) {
                std::cerr << "::" << "ENEMY CLASS REQUIRE ERROR" << ":: " << "EnemyClass '" << ClassName << "' had an error during loading" << std::endl;
                continue;
            }

            if (lua_pcallk(Context.ContextObject, 0, -1, 0, 0, 0)) {
                std::cerr << "::" << "ENEMY CLASS REQUIRE ERROR" << ":: " << "EnemyClass '" << ClassName << "' had an error during running" << std::endl;
                continue;
            }

            AssetsEnemyClasses.SetSubtable(ClassName, Context.GetStackTop());
        }

        //AssetsItemClasses.PushOntoStack();
        //Context.PushNilOntoStack();
        //lua_setfield(Context.ContextObject, -2, "__index");

        AssetsTable.SetSubtable("EnemyClasses", AssetsEnemyClasses);

        luaL_dofile(Context.ContextObject, "Enemies.lua");

        if (!Context.IsTable(-1)) {
            std::cerr << "::" << "FATAL" << ":: " << "Failed to load " << "Enemies.lua" << '\n'
                << "Returned value from script isn't a table.\n"
                << "Aborting" << std::endl;
            exit(EXIT_FAILURE);
        }

        LuaClasses::LuaTable Items(Context.GetStackTop(), &Context);
        
        GameTable.SetSubtable("Enemies", Items);
        
        Items.SetElementWithLiteral("CreateEnemy", _private::Game::Enemies::CFunctions::CreateEnemy);
        Items.SetElementWithLiteral("RemoveEnemy", _private::Game::Enemies::CFunctions::RemoveEnemy);

        CurrentContext = &Context;
    }

    LuaClasses::LuaTable&& CreateEnemy(const char* Name) {

        CurrentContext->PushOntoStack(Name);
        _private::Game::Enemies::CFunctions::CreateEnemy(CurrentContext->ContextObject);

        LuaClasses::LuaTable NewEnemy(CurrentContext, CurrentContext->GetStackTop());

        return std::move(NewEnemy);
    }

    inline void RemoveEnemy(LuaTableObject ItemClass) {

        CurrentContext->PushValueOntoStack(ItemClass);
        _private::Game::Enemies::CFunctions::RemoveEnemy(CurrentContext->ContextObject);
    }

    inline void RemoveEnemy(LuaClasses::LuaTable& ItemClass) {
        RemoveEnemy(ItemClass.TableObject);
    }
}

#endif