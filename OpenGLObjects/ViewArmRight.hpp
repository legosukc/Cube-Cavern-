#ifndef _OPENGLOBJECT_VIEWARMRIGHT
#define _OPENGLOBJECT_VIEWARMRIGHT

#include <glad/glad.h>

#include <glm/glm.hpp>

#include "./OtherClasses/GLClasses.hpp"

#include "ViewModelObjectBase.hpp"

#include "./FunctionHeaders/RenderingFunctions.hpp"
#include "./FunctionHeaders/Maths.hpp"

#include "./LuauClasses/Vector3.hpp"

#include "./GameClasses/Player.hpp"
#include "./GameClasses/TextureManager.hpp"

#include "./file.hpp"

namespace OpenGLObjects {

    class ViewArmRight : public OpenGLObjects::ViewModelObjectBase {

        //GLint ModelUniformID;   // mat4 Uniform

        GameClasses::Player* Player;

        //GLuint Palette;

    public:
        ViewArmRight(GameClasses::Player* Player) {

            this->ViewModelObjectBaseConstructor("ViewArm.obj", "viewarm_palette.bmp");

            this->Player = Player;
            /*
            this->Palette = GLClasses::Texture("viewarm_palette.bmp", GL_CLAMP_TO_BORDER, GL_CLAMP_TO_BORDER, false, GL_NEAREST, GL_NEAREST).TextureObject;//GameClasses::TextureManager::LoadTexture("viewarm_palette.bmp", "ViewArmRightPalette", GL_CLAMP_TO_BORDER, GL_CLAMP_TO_BORDER, false, GL_NEAREST, GL_NEAREST);

            // Shader Program

            this->Program.LoadShader(GL_VERTEX_SHADER, "ViewArm.vert");
            this->Program.LoadShader(GL_FRAGMENT_SHADER, "ViewArm.frag");

            this->Program.LinkProgram();

            this->Program.UniformBlockBinding("RenderingMatrices", 0);

            this->Program.Use();

            // Uniforms

            this->ModelUniformID = this->Program.GetUniformLocation("model");

            // Texture Bindings

            this->Program.SetTextureBinding("Palette", 0);*/
        };

        //glm::mat4 ModelMatrix;

        void Update() override {

            this->Program.Use();

            this->ModelMatrix = RenderingFunctions::rotate(glm::mat4(1.f), Maths::ToRadians(this->Player->ArmY), LuauClasses::Vector3(0.f, 1.f, 0.f));
            this->ModelMatrix = RenderingFunctions::translate(this->ModelMatrix, LuauClasses::Vector3(0.175f, -0.2f, -0.35f));

            this->Program.Mat4SetUniform(this->ModelUniformID, this->ModelMatrix);
        }

        void Draw() override {

            this->VAO.Bind();
            this->Program.Use();

            this->Palette->Bind(GL_TEXTURE0);

            glStencilMask(0xFF);
            glStencilFunc(GL_ALWAYS, 1, 0xFF);
            glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);

            glDrawElements(GL_TRIANGLES, this->IndicesLen, GL_UNSIGNED_INT, 0);

            glStencilOp(GL_KEEP, GL_KEEP, GL_KEEP);
            glStencilFunc(GL_EQUAL, 0, 0xFF);

            this->VAO.Unbind();
        }
    };
}

#endif