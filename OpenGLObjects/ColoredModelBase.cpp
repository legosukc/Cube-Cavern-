
#include "ColoredModelBase.hpp"

#include <glad/glad.h>

#include "./OtherClasses/GLClasses.hpp"

#include "./Enums/UniformBufferBindingIndexes.hpp"

namespace OpenGLObjects {

    void ColoredModelBase::ColoredModelBaseConstructor(const char* ModelName, const char* VertexShader, const char* FragmentShader) {
        this->ModelBaseConstructor(ModelName);

        // Shaders
        if (VertexShader == nullptr || FragmentShader == nullptr) {
            return;
        }

        this->Program.LoadShader(GL_VERTEX_SHADER, VertexShader);
        this->Program.LoadShader(GL_FRAGMENT_SHADER, FragmentShader);

        this->Program.LinkProgram();

        this->Program.UniformBlockBinding("RenderingMatrices", Enums::UniformBufferBindings::RenderingMatrices);

        this->Program.Use();

        // Uniforms

        this->ModelUniformID = this->Program.GetUniformLocation("model");
    }
};