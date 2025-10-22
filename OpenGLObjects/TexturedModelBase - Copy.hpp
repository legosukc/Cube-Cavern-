#ifndef _OPENGLOBJECTS_TEXTUREDMODELBASE
#define _OPENGLOBJECTS_TEXTUREDMODELBASE

#include <glad/glad.h>

#include "./OtherClasses/GLClasses.hpp"
#include "./GameClasses/TextureManager.hpp"

#include "ColoredModelBase.hpp"

namespace OpenGLObjects {

    class TexturedModelBase : public OpenGLObjects::ColoredModelBase {
    public:

        virtual void Draw() override {

            this->Texture->Bind(GL_TEXTURE0);

            ColoredModelBase::Draw();

            this->Texture->Unbind();
            /*
            this->VAO.Bind();
            this->Program.Use();

            this->Program.Mat4SetUniform(this->ModelUniformID, this->ModelMatrix);

            this->Texture->Bind(GL_TEXTURE0);

            glStencilMask(0xFF);
            glStencilFunc(GL_ALWAYS, 1, 0xFF);
            glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);

            glDrawElements(GL_TRIANGLES, this->IndicesLen, this->IndicesType, 0);

            glStencilOp(GL_KEEP, GL_KEEP, GL_KEEP);
            glStencilFunc(GL_EQUAL, 0, 0xFF);

            this->Texture->Unbind();
            this->VAO.Unbind();*/
        }

        //virtual void Deconstructor() override {
            //GameClasses::TextureManager::RemoveTexture(Palette);
            //delete this->Palette;
        //}

    protected:

        GLClasses::Texture* Texture;

        void TexturedModelBaseConstructor(const char* ModelName, const char* TextureName, const char* VertexShader = "ShadedTextureModel.vert", const char* FragmentShader = "ShadedTexture.frag") {
            this->ColoredModelBaseConstructor(ModelName, VertexShader, FragmentShader);

            this->Texture = GameClasses::TextureManager::LoadTexture(TextureName, GL_REPEAT, GL_REPEAT, false, GL_NEAREST, GL_NEAREST);

            // Shaders
            if (VertexShader == nullptr or FragmentShader == nullptr) {
                return;
            }

            // Texture Bindings

            this->Program.SetTextureBinding("Texture", 0);
        };
    };
}

#endif