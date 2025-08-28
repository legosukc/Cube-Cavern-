#define SDL_MAIN_HANDLED

#include <glad/glad.h>
#include <glad/glad.c>

#include <SDL2/SDL.h>
#include <SDL2/SDL_opengl.h>
#include <SDL2/SDL_timer.h>
#include <SDL2/SDL_error.h>

#include <SDL2/SDL_image.h>

#include <iostream>


/*  OTHER CLASSES   */
#include "OtherClasses/GLClasses.hpp"
#include "OpenGLObjects.hpp"


/*  UNIFORM BUFFER CLASSES  */
#include "OpenGLObjects/UniformBuffers/WindowSize.hpp"

#include "OpenGLObjects/UniformBuffers/RenderingMatrices.hpp"


/*  SDL CLASSES   */
#include "SDLClasses/Window.hpp"
#include "SDLClasses/InputHandler.hpp"


/*   LUAU CLASSES   */
#include "LuauClasses/Vector3.hpp"
#include "LuauClasses/task.hpp"


/*  GAME CLASS HEADERS  */
#include "GameClasses/Player.hpp"
#include "GameClasses/ObjectManager.hpp"


/*  FUNCTION HEADERS   */
#include "FunctionHeaders/RenderingFunctions.hpp"
#include "FunctionHeaders/Maths.hpp"

#include "FunctionHeaders/Error.hpp"

#include "UniformBufferBindingIndexes.hpp"

#include "file.hpp"

int _cdecl main() {

    // Initialize SDL
    if (const int ErrorCode = SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO); ErrorCode != 0) {
        FunctionFailedError("SDL_Init", SDL_GetError());
        std::cerr << "Error Code: " << ErrorCode
            << "\nClosing in 5 seconds" << std::endl;

        SDL_Delay(5000u);
        return 0;
    }

    IMG_Init(IMG_INIT_JPG | IMG_INIT_PNG | IMG_INIT_TIF | IMG_INIT_JXL | IMG_INIT_AVIF | IMG_INIT_WEBP);

    file::Init();

    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);

    SDLClasses::Window Window("SDL Window", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 800, 600, SDL_WINDOW_RESIZABLE);
    SDLClasses::InputHandler::SetTargetWindow(&Window);

    std::cout << gladLoadGLLoader((GLADloadproc)SDL_GL_GetProcAddress) << '\n';

    // OBJECTS

    GameClasses::Player Player(&Window.Delta60);


    // RENDERING OBJECTS

    GameClasses::ObjectManager ObjectManager;

    OpenGLObjects::ViewArmRight* ViewArmRight = ObjectManager.AddObject(new OpenGLObjects::ViewArmRight(&Player));
    ObjectManager.AddObject<OpenGLObjects::Doomspire>();
    

    //GLClasses::UniformBuffer RenderingMatrices(sizeof(glm::mat4) * 2, UniformBufferBindings::RenderingMatrices, GL_DYNAMIC_DRAW);

    UniformBuffers::RenderingMatrices RenderingMatrices(&Player);
    RenderingMatrices.UpdateProjection(&RenderingMatrices, &Window);

    GLClasses::UniformBuffer WindowSize(sizeof(int) * 2, UniformBufferBindings::WindowSize, GL_STATIC_DRAW);

    Window.WindowResizedEvent.Connect(UniformBuffers::WindowResizedConnection, &WindowSize, false);
    Window.WindowResizedEvent.Connect(RenderingMatrices.UpdateProjection, &RenderingMatrices, false);

    glEnable(GL_STENCIL_TEST);
    glEnable(GL_DEPTH_TEST);

    std::cout << "running\n";

    while (Window.IsWindowAlive) {

        Window.PollEvents();

        // Updates
        LuauClasses::task::Update();

        Player.Update();

        RenderingMatrices.UpdateView();

        ObjectManager.UpdateObjects();

        // Rendering

        glClearColor(0, 0, 0, 0);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

        //QuadObject.Draw();
        ObjectManager.DrawObjects();

        Window.RenderUpdateWindow();
    }

    //SDL_SetWindowGrab(Window.SDLWindow, SDL_FALSE);

    // Clean up

    SDL_Quit();
    return 0;
}