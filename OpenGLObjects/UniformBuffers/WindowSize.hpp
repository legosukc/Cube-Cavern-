#ifndef _OPENGL_UNIFORMBUFFERS_WINDOWSIZE
#define _OPENGL_UNIFORMBUFFERS_WINDOWSIZE

#include "OtherClasses/GLClasses.hpp"

#include "SDLClasses/Window.hpp"

namespace UniformBuffers {

    static void WindowResizedConnection(void* Connector, SDLClasses::Window* Window) {

        const GLClasses::UniformBuffer* self = (GLClasses::UniformBuffer*)Connector;

        self->Bind();
        self->BindSubDataPointerWithSize(&Window->Width, sizeof(int) * 2, 0);
        self->Unbind();
    }
}

#endif