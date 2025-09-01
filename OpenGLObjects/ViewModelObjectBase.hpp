#ifndef _OPENGLOBJECTS_VIEWMODELOBJECTBASE
#define _OPENGLOBJECTS_VIEWMODELOBJECTBASE

#include "./OtherClasses/GLClasses.hpp"

#include "./OpenGLObjects/ModelBase.hpp"
#include "./GameClasses/TextureManager.hpp"

#include "./file.hpp"

namespace OpenGLObjects {

    class ViewModelObjectBase : public OpenGLObjects::ModelBase {
    public:

        glm::mat4 ModelMatrix;

        virtual void Draw() override {

            this->VAO.Bind();
            this->Program.Use();

            this->Palette->Bind(GL_TEXTURE0);

            glStencilMask(0xFF);
            glStencilFunc(GL_ALWAYS, 1, 0xFF);
            glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);

            glDrawElements(GL_TRIANGLES, this->IndicesLen, GL_UNSIGNED_INT, 0);

            glStencilOp(GL_KEEP, GL_KEEP, GL_KEEP);
            glStencilFunc(GL_EQUAL, 0, 0xFF);

            this->Palette->Unbind();
            this->VAO.Unbind();
        }

    protected:

        GLint ModelUniformID;   // mat4 Uniform

        GLClasses::Texture* Palette;

        void ViewModelObjectBaseConstructor(const char* ModelFile, const char* PaletteFile, const char* VertexShader = "ViewArm.vert", const char* FragmentShader = "ViewArm.frag") {
            this->ModelBaseConstructor(ModelFile);

            this->Palette = GameClasses::TextureManager::LoadTexture(PaletteFile, GL_CLAMP_TO_BORDER, GL_CLAMP_TO_BORDER, false, GL_NEAREST, GL_NEAREST);

            // Shader Program

            this->Program.LoadShader(GL_VERTEX_SHADER, VertexShader);
            this->Program.LoadShader(GL_FRAGMENT_SHADER, FragmentShader);

            this->Program.LinkProgram();

            this->Program.UniformBlockBinding("RenderingMatrices", 0);

            this->Program.Use();

            // Uniforms

            this->ModelUniformID = this->Program.GetUniformLocation("model");

            // Texture Bindings

            this->Program.SetTextureBinding("Palette", 0);
        };
    };
}

#endif