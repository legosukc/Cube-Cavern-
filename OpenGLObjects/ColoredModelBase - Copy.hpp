#ifndef _OPENGLOBJECTS_COLOREDMODELBASE
#define _OPENGLOBJECTS_COLOREDMODELBASE

#include <glad/glad.h>

#include "./OtherClasses/GLClasses.hpp"

#include "./Enums/UniformBufferBindingIndexes.hpp"

#include "ModelBase.hpp"

namespace OpenGLObjects {

    class ColoredModelBase : public OpenGLObjects::ModelBase {
    protected:

        void ColoredModelBaseConstructor(const char* ModelName, const char* VertexShader = "ShadedColorModel.vert", const char* FragmentShader = "ShadedColor.frag") {
            this->ModelBaseConstructor(ModelName);

            // Shaders
            if (VertexShader == nullptr or FragmentShader == nullptr) {
                return;
            }

            this->Program.LoadShader(GL_VERTEX_SHADER, VertexShader);
            this->Program.LoadShader(GL_FRAGMENT_SHADER, FragmentShader);

            this->Program.LinkProgram();

            this->Program.UniformBlockBinding("RenderingMatrices", Enums::UniformBufferBindings::RenderingMatrices);

            this->Program.Use();

            // Uniforms

            this->ModelUniformID = this->Program.GetUniformLocation("model");
        };
    };
}

#endif