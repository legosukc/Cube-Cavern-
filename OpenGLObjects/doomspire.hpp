#ifndef _OPENGLOBJECT_DOOMSPIRE
#define _OPENGLOBJECT_DOOMSPIRE

#include <glad/glad.h>

/*  OTHER CLASSES   */
#include "./OtherClasses/GLClasses.hpp"


#include "ModelBase.hpp"


/*  ENUMS   */
#include "./Enums/UniformBufferBindingIndexes.hpp"

namespace OpenGLObjects {

    class Doomspire : public OpenGLObjects::ModelBase {
    public:
        Doomspire() {
            this->ModelBaseConstructor("doomspire.obj");
            
            // Shader Program

            this->Program.LoadShader(GL_VERTEX_SHADER, "doomspire.vert");
            this->Program.LoadShader(GL_FRAGMENT_SHADER, "doomspire.frag");

            this->Program.LinkProgram();

            this->Program.UniformBlockBinding("RenderingMatrices", Enums::UniformBufferBindings::RenderingMatrices);

            this->Program.Use();

            // Texture Bindings

            this->Program.SetTextureBinding("Texture", 0);
        };
    };
}

#endif