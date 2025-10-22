#ifndef _OPENGLOBJECTS_PARENTEDTEXTUREDMODELBASE
#define _OPENGLOBJECTS_PARENTEDTEXTUREDMODELBASE

#include <glad/glad.h>

#include "./GameClasses/TextureManager.hpp"

//#include "ViewArmRight.hpp"
#include "ModelBase.hpp"
#include "ParentedColoredModelBase.hpp"

#include "./OtherClasses/GLClasses.hpp"

namespace OpenGLObjects {

    template<class GLObjectParent = OpenGLObjects::ModelBase>
    class ParentedTexturedModelBase : public OpenGLObjects::ParentedColoredModelBase<GLObjectParent> {
    public:

        GLClasses::Texture* Texture;

        virtual void Draw() override {

            this->Texture->Bind(GL_TEXTURE0);

            ParentedColoredModelBase<GLObjectParent>::Draw();

            this->Texture->Unbind();
        }

    protected:

        void ParentedTexturedModelBaseConstructor(const char* ModelFile, const char* TextureName, GLObjectParent* ParentPtr, const char* VertexShader = "ShadedTextureModel.vert", const char* FragmentShader = "ShadedTexture.frag") {
            this->ParentedColoredModelBaseConstructor(ModelFile, ParentPtr, VertexShader, FragmentShader);

            this->Texture = GameClasses::TextureManager::LoadTexture(TextureName, GL_REPEAT, GL_REPEAT, false, GL_NEAREST, GL_NEAREST);

            // Shaders
            if (VertexShader == nullptr || FragmentShader == nullptr) {
                return;
            }

            // Texture Bindings

            this->Program.SetTextureBinding("Texture", 0);
        }
    };
}

#endif