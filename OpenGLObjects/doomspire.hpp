#ifndef _OPENGLOBJECT_DOOMSPIRE
#define _OPENGLOBJECT_DOOMSPIRE

#include <glad/glad.h>
#include <glm/glm.hpp>

#include <vector>

#include "file.hpp"

/*  OTHER CLASSES   */
#include "./OtherClasses/GLClasses.hpp"

#include "./OpenGLObjects/ObjectBase.hpp"


/*  FUNCTION HEADERS   */
#include "./FunctionHeaders/RenderingFunctions.hpp"
#include "./FunctionHeaders/Maths.hpp"


/*   LUAU CLASSES   */
#include "./LuauClasses/Vector3.hpp"


/*  OTHER   */
#include "./UniformBufferBindingIndexes.hpp"

namespace OpenGLObjects {

    class Doomspire : public OpenGLObjects::ModelBase<Doomspire> {

        GLint ModelUniformID;   // mat4 Uniform
        glm::mat4 ModelMatrix;

        static void _Draw(Doomspire* self) {
            self->VAO.Bind();
            self->Program.Use();

            self->Program.Mat4SetUniform(self->ModelUniformID, self->ModelMatrix);

            glDrawElements(GL_TRIANGLES, self->IndicesLen, GL_UNSIGNED_INT, 0);

            self->VAO.Unbind();
        }

    public:
        Doomspire() {

            this->Draw = this->_Draw;

            file::loadOBJIntoOpenGLObject("doomspire.obj", this);

            // Shader Program

            this->Program.LoadShader(GL_VERTEX_SHADER, "doomspire.vert");
            this->Program.LoadShader(GL_FRAGMENT_SHADER, "doomspire.frag");

            this->Program.LinkProgram();

            this->Program.UniformBlockBinding("RenderingMatrices", UniformBufferBindings::RenderingMatrices);

            this->Program.Use();

            // Uniforms

            this->ModelMatrix = RenderingFunctions::rotate(glm::mat4(1.f), 0.f, LuauClasses::Vector3(0.5f, 1.f, 0.f));

            this->ModelUniformID = this->Program.GetUniformLocation("model");

            this->Program.Mat4SetUniform(this->ModelUniformID, this->ModelMatrix);

            // Texture Bindings

            this->Program.SetTextureBinding("Texture", 0);
        };
    };
}

#endif