#ifndef _OPENGLOBJECT_DOOMSPIRE
#define _OPENGLOBJECT_DOOMSPIRE

#include <glad/glad.h>

#include "./file.hpp"

/*  OTHER CLASSES   */
#include "./OtherClasses/GLClasses.hpp"

#include "ModelBase.hpp"


/*  FUNCTION HEADERS   */
#include "./FunctionHeaders/RenderingFunctions.hpp"
#include "./FunctionHeaders/Maths.hpp"


/*   LUAU CLASSES   */
#include "./LuauClasses/Vector3.hpp"


/*  OTHER   */
#include "./UniformBufferBindingIndexes.hpp"

namespace OpenGLObjects {

    class Doomspire : public OpenGLObjects::ModelBase {
    public:
        Doomspire() {
            this->ModelBaseConstructor("doomspire.obj");
            
            // Shader Program

            this->Program.LoadShader(GL_VERTEX_SHADER, "doomspire.vert");
            this->Program.LoadShader(GL_FRAGMENT_SHADER, "doomspire.frag");

            this->Program.LinkProgram();

            this->Program.UniformBlockBinding("RenderingMatrices", UniformBufferBindings::RenderingMatrices);

            this->Program.Use();

            // Texture Bindings

            this->Program.SetTextureBinding("Texture", 0);
        };
        /*
        void Draw() {
            this->VAO.Bind();
            this->Program.Use();

            glDrawElements(GL_TRIANGLES, this->IndicesLen, GL_UNSIGNED_INT, 0);

            this->VAO.Unbind();
        }*/
    };
}

#endif