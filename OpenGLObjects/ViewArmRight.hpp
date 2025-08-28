#ifndef _OPENGLOBJECT_VIEWARMRIGHT
#define _OPENGLOBJECT_VIEWARMRIGHT

#include <glad/glad.h>

#include <glm/glm.hpp>

#include "./OtherClasses/GLClasses.hpp"

#include "./OpenGLObjects/ObjectBase.hpp"

#include "./FunctionHeaders/RenderingFunctions.hpp"

#include "./LuauClasses/Vector3.hpp"

#include "./GameClasses/Player.hpp"
#include "./GameClasses/TextureManager.hpp"

#include "./file.hpp"

namespace OpenGLObjects {

    class ViewArmRight : public OpenGLObjects::ModelBase<ViewArmRight> {

        GLint ModelUniformID;   // mat4 Uniform

        GameClasses::Player* Player;

        GLClasses::Texture* Palette;

        static void _Update(ViewArmRight* self) {

            self->Program.Use();

            self->ModelMatrix = RenderingFunctions::rotate(glm::mat4(1.f), self->Player->ArmY / 360.f, LuauClasses::Vector3(0.f, 1.f, 0.f));
            self->ModelMatrix = RenderingFunctions::translate(self->ModelMatrix, LuauClasses::Vector3(0.175f, -0.2f, -0.35f));

            self->Program.Mat4SetUniform(self->ModelUniformID, self->ModelMatrix);
        }

        static void _Draw(ViewArmRight* self) {

            self->VAO.Bind();
            self->Program.Use();

            self->Palette->Bind(GL_TEXTURE0);

            glStencilMask(0xFF);
            glStencilFunc(GL_ALWAYS, 1, 0xFF);
            glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);

            glDrawElements(GL_TRIANGLES, self->IndicesLen, GL_UNSIGNED_INT, 0);

            glStencilOp(GL_KEEP, GL_KEEP, GL_KEEP);
            glStencilFunc(GL_EQUAL, 0, 0xFF);

            self->Palette->Unbind();
            self->VAO.Unbind();
        }

        static inline void _Deconstructor(ViewArmRight* self) {
            delete self->Palette;
        }

    public:
        ViewArmRight(GameClasses::Player* Player) {

            file::loadOBJIntoOpenGLObject("ViewArm.obj", this);

            this->Player = Player;

            this->Draw = this->_Draw;
            this->Update = this->_Update;
            this->Deconstructor = this->_Deconstructor;
            
            this->Palette = GameClasses::TextureManager::LoadTexture("viewarm_palette.bmp", "ViewArmRightPalette", GL_CLAMP_TO_BORDER, GL_CLAMP_TO_BORDER, false, GL_NEAREST, GL_NEAREST);

            // Shader Program

            this->Program.LoadShader(GL_VERTEX_SHADER, "ViewArm.vert");
            this->Program.LoadShader(GL_FRAGMENT_SHADER, "ViewArm.frag");

            this->Program.LinkProgram();

            this->Program.UniformBlockBinding("RenderingMatrices", 0);

            this->Program.Use();

            // Uniforms

            this->ModelUniformID = this->Program.GetUniformLocation("model");

            // Texture Bindings

            this->Program.SetTextureBinding("Palette", 0);
        };

        glm::mat4 ModelMatrix;
    };
}

#endif