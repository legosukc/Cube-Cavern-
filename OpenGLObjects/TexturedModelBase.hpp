#ifndef _OPENGLOBJECTS_TEXTUREDMODELBASE
#define _OPENGLOBJECTS_TEXTUREDMODELBASE

#include "./OtherClasses/GLClasses.hpp"

#include "ColoredModelBase.hpp"

namespace OpenGLObjects {

    class TexturedModelBase : public OpenGLObjects::ColoredModelBase {
    public:

        virtual void Draw() override;

    protected:

        GLClasses::Texture* Texture;

        void TexturedModelBaseConstructor(const char* ModelName, const char* TextureName, const char* VertexShader = "ShadedTextureModel.vert", const char* FragmentShader = "ShadedTexture.frag");
    };
}

#endif