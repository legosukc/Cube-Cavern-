#ifndef _OPENGL_UNIFORMBUFFERS_RENDERINGMATRICES
#define _OPENGL_UNIFORMBUFFERS_RENDERINGMATRICES

#include "./OtherClasses/GLClasses.hpp"

#include "./FunctionHeaders/RenderingFunctions.hpp"
#include "./FunctionHeaders/Maths.hpp"

#include "./SDLClasses/Window.hpp"

#include "./GameClasses/Player.hpp"

#include "./UniformBufferBindingIndexes.hpp"

namespace UniformBuffers {

    class RenderingMatrices : public GLClasses::UniformBuffer {

        GameClasses::Player* Player;

    public:
        RenderingMatrices(GameClasses::Player* Player) {
            this->Constructor(NULL, sizeof(glm::mat4) * 2, GL_DYNAMIC_DRAW);
            this->BindBufferBase(UniformBufferBindings::RenderingMatrices);

            this->Player = Player;
        }

        static void UpdateProjection(void* Connector, SDLClasses::Window* Window) {

            const UniformBuffers::RenderingMatrices* self = (UniformBuffers::RenderingMatrices*)Connector;

            self->Bind();

            const glm::mat4 Projection = RenderingFunctions::perspective(
                Maths::ToRadians(self->Player->FOV),
                float(Window->Width) / float(Window->Height),
                Window->zNear, Window->zFar
            )
                ;
            self->BindSubDataPointer(&Projection, 0);

            self->Unbind();
        }

        void UpdateView() {
           
            this->Bind();

            const glm::mat4 View = this->Player->GetViewMatrix();
            this->BindSubDataPointer(&View, sizeof(glm::mat4));

            this->Unbind();
        }
    };
}

#endif