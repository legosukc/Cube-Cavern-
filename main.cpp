#define SDL_MAIN_HANDLED

#include <glad/glad.h>
#include <glad/glad.c>

#include <SDL2/SDL.h>
#include <SDL2/SDL_opengl.h>
#include <SDL2/SDL_timer.h>
#include <SDL2/SDL_error.h>

#include <SDL2/SDL_image.h>

#include <iostream>

#include <cstdlib>

/*  OTHER CLASSES   */
#include "OtherClasses/GLClasses.hpp"
#include "OpenGLObjects.hpp"


/*  UNIFORM BUFFER CLASSES  */
#include "OpenGLObjects/UniformBuffers/WindowSize.hpp"

#include "OpenGLObjects/UniformBuffers/RenderingMatrices.hpp"


/*  SDL CLASSES   */
#include "SDLClasses/Window.hpp"
#include "SDLClasses/InputHandler.hpp"
#include "SDLClasses/Sound.hpp"


/*   LUAU CLASSES   */
#include "LuauClasses/Vector3.hpp"
#include "LuauClasses/task.hpp"


/*   LUA CLASSES    */
#include "LuaClasses/LuaContext.hpp"
#include "LuaClasses/LuaGlobalTable.hpp"

#include "LuaClasses/LuaScript.hpp"


/*  GAME CLASS HEADERS  */
#include "GameClasses/Player.hpp"

#include "GameClasses/ObjectManager.hpp"
#include "GameClasses/Items.hpp"


/*  FUNCTION HEADERS   */
#include "FunctionHeaders/RenderingFunctions.hpp"
#include "FunctionHeaders/Maths.hpp"

#include "FunctionHeaders/Error.hpp"

#include "Enums/UniformBufferBindingIndexes.hpp"

#include "file.hpp"

#include "ItemClasses/ItemClasses.hpp"
#include "OpenGLObjects/ViewArmRight.hpp"
#include "LuaTypes.h"


// item classes and enemy classes and interact classes - everything like that will be in lua

// learn variadic templates

static SDL_FORCE_INLINE int Run() {

    SDLClasses::Window Window("SDL Window", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 800, 600, SDL_WINDOW_RESIZABLE);
    SDLClasses::InputHandler::SetTargetWindow(&Window);

    std::cout << gladLoadGLLoader((GLADloadproc)SDL_GL_GetProcAddress) << '\n';


    LuaClasses::LuaContext Context;

    LuaClasses::LuaGlobalTable GameTable(&Context, "Game");
    LuaClasses::LuaGlobalTable AssetsTable(&Context, "Assets");

    Game::Items::Init(Context, GameTable, AssetsTable);

    LuaFunctionObject LuaUpdateLoopFunction, LuaDrawLoopFunction;
    {
        LuaClasses::LuaScript GameLoop(&Context, "GameLoop.lua");
        GameLoop.RunScript();

        LuaDrawLoopFunction = Context.GetStackTop();
        Context.PopStack();

        LuaUpdateLoopFunction = Context.GetStackTop();
        Context.PopStack();
    }

    // OBJECTS

    GameClasses::Player Player(&Window.Delta60);
    SDLClasses::Sound::Listener::Position = &Player.Position;
    SDLClasses::Sound::Listener::Velocity = &Player.Velocity;

    //Player.AddItemToInventory<ItemClasses::Dagger>();
    //Player.AddItemToInventory<ItemClasses::Stick>();
    
    //auto StickItem = Game::Items::CreateItem<ItemClasses::Stick, GameClasses::Player, OpenGLObjects::ViewArmRight>(nullptr, nullptr);
    auto&& StickItem = Game::Items::CreateItem("Stick");

    //std::cout << Player.Items[0]->Name << '\n';


    // RENDERING OBJECTS


    GameClasses::ObjectManager ObjectManager;

    ObjectManager.AddObject<OpenGLObjects::Doomspire>();

    ObjectManager.AddObject(new OpenGLObjects::TutorialBubble);


    // UNIFORM BUFFERS

    UniformBuffers::RenderingMatrices RenderingMatrices(&Player);
    UniformBuffers::RenderingMatrices::RenderingMatrices::UpdateProjection(&RenderingMatrices, &Window);

    GLClasses::UniformBuffer WindowSize(sizeof(int) * 2, Enums::UniformBufferBindings::WindowSize, GL_STATIC_DRAW);

    Window.WindowResizedEvent.Connect(UniformBuffers::WindowResizedConnection, &WindowSize, false);
    Window.WindowResizedEvent.Connect(UniformBuffers::RenderingMatrices::UpdateProjection, &RenderingMatrices, false);

    glEnable(GL_STENCIL_TEST);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);

    std::cout << "running\n";

    while (Window.IsWindowAlive) {

        Window.PollEvents();

        // Updates
        LuauClasses::task::Update();

        Player.Update();

        Context.PushValueOntoStack(LuaUpdateLoopFunction);
        lua_callk(Context.ContextObject, 0, -1, 0, 0);
        Context.PopStack();

        //Game::Items::Update();

        SDLClasses::Sound::Listener::Update();

        RenderingMatrices.UpdateView();

        ObjectManager.UpdateObjects();

        // Rendering

        glClearColor(0.f, 0.f, 0.f, 0.f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

        Player.Draw();
        ObjectManager.DrawObjects();

        Context.PushValueOntoStack(LuaDrawLoopFunction);
        lua_callk(Context.ContextObject, 0, -1, 0, 0);
        Context.PopStack();

        Window.RenderUpdateWindow();
    }

    return EXIT_SUCCESS;
}

int _cdecl main() {
    
    // Initialize SDL
    if (const int ErrorCode = SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO); ErrorCode != 0) {
        FunctionFailedError("SDL_Init", SDL_GetError());
        std::cerr << "Error Code: " << ErrorCode
            << "\nClosing in 5 seconds" << std::endl;

        SDL_Delay(5000);
        return EXIT_FAILURE;
    }

    //IMG_Init(IMG_INIT_JPG | IMG_INIT_PNG | IMG_INIT_TIF | IMG_INIT_JXL | IMG_INIT_AVIF | IMG_INIT_WEBP);
    {
        const int InitializedFlags = IMG_Init(IMG_INIT_PNG);
        
        if (!(InitializedFlags & IMG_INIT_PNG)) {
            std::cerr << "::FATAL!:: " << ":IMG_Init ERROR:" << "Failed to initialize flag IMG_INIT_PNG."
                << "IMG Error: " << IMG_GetError() << '\n'
                << "::CRASH:: " << "Line " << __LINE__ << " in " << __FILE__
                << "Terminating" << std::endl;

            return EXIT_FAILURE;
        }
    }

    file::Init();
    SDLClasses::Sound::Init();

    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);

    const int ErrorCode = Run();

    IMG_Quit();
    SDLClasses::Sound::Quit();
    SDL_Quit();

    return ErrorCode;
}