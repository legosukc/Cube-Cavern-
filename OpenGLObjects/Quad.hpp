#ifndef _OPENGLOBJECTS_QUAD
#define _OPENGLOBJECTS_QUAD

#include "OtherClasses/GLClasses.hpp"

#include "OpenGLObjects/ObjectBase.hpp"

const float quadVert[] = {
    // positions   // texCoords
    -1.f,  1.f,  0.f, 1.f,
    -1.f, -1.f,  0.f, 0.f,
     1.f, -1.f,  1.f, 0.f,
     1.f,  1.f,  1.f, 1.f
};

const Uint8 quadIndices[] = {
    0, 1, 2,
    0, 2, 3
};

namespace OpenGLObjects {

    class Quad : public OpenGLObjects::ObjectBase<Quad> {
        
        static void _Draw(Quad* self) {
            self->VAO.Bind();
            self->Program.Use();

            glDrawElements(GL_TRIANGLES, sizeof(quadIndices), GL_UNSIGNED_BYTE, 0);

            self->VAO.Unbind();
        }

    public:
        Quad() {

            this->Draw = this->_Draw;

            this->VAO.Bind();
            this->VBO.Bind();
            this->EBO.Bind();

            this->VAO.StrideSize = sizeof(float) * 4;

            this->VBO.BindDataPointerWithSize(&quadVert, sizeof(quadVert), GL_STATIC_DRAW);
            this->EBO.BindDataPointerWithSize(&quadIndices, sizeof(quadIndices), GL_STATIC_DRAW);

            this->VAO.AddAttribute<GL_FLOAT>(2);    // Position
            this->VAO.AddAttribute<GL_FLOAT>(2);    // Texcoord

            this->VAO.Unbind();


            // Shader Program

            this->Program.LoadShader(GL_VERTEX_SHADER, "quad.vert");
            this->Program.LoadShader(GL_FRAGMENT_SHADER, "quad.frag");

            this->Program.LinkProgram();
            this->Program.Use();

            // Texture Bindings

            this->Program.SetTextureBinding("RenderTexture", 0);
        };
    };
}

#endif