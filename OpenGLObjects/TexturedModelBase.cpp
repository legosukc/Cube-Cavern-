
#include "TexturedModelBase.hpp"
#include "ColoredModelBase.hpp"

#include <glad/glad.h>

#include "./OtherClasses/GLClasses.hpp"
#include "./GameClasses/TextureManager.hpp"

namespace OpenGLObjects {

    void TexturedModelBase::Draw() {

        this->Texture->Bind(GL_TEXTURE0);

        ColoredModelBase::Draw();

        this->Texture->Unbind();
    }

    void TexturedModelBase::TexturedModelBaseConstructor(const char* ModelName, const char* TextureName, const char* VertexShader, const char* FragmentShader) {
        this->ColoredModelBaseConstructor(ModelName, VertexShader, FragmentShader);

        this->Texture = GameClasses::TextureManager::LoadTexture(TextureName, GL_REPEAT, GL_REPEAT, false, GL_NEAREST, GL_NEAREST);

        // Shaders
        if (VertexShader == nullptr || FragmentShader == nullptr) {
            return;
        }

        // Texture Bindings

        this->Program.SetTextureBinding("Texture", 0);
    }
};