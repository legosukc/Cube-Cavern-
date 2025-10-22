#ifndef _OPENGLOBJECT_TUTORIALBUBBLE
#define _OPENGLOBJECT_TUTORIALBUBBLE

#include <glad/glad.h>

/*  OTHER CLASSES   */
#include "./OtherClasses/GLClasses.hpp"

#include "ModelBase.hpp"
#include "Quad.hpp"

/*  ENUMS   */
#include "./Enums/UniformBufferBindingIndexes.hpp"


namespace OpenGLObjects {

    class TutorialBubble : public OpenGLObjects::ModelBase {

        GLClasses::Texture* TutorialBubbleTexture;

    public:
        TutorialBubble() {  

            this->IndicesType = GL_UNSIGNED_BYTE;

            this->VAO.Bind();
            this->VBO.Bind();
            this->EBO.Bind();

            this->VAO.StrideSize = sizeof(float) * 4;
            this->IndicesLen = sizeof(quadIndices);

            const float TutorialBubbleVert[] = {
                // positions   // texCoords
                -1.f,  0.64f,  0.f, -1.f,
                -1.f, -0.64f,  0.f,  0.f,
                 1.f, -0.64f,  2.f,  0.f,
                 1.f,  0.64f,  2.f, -1.f
            };

            this->VBO.BindDataPointerWithSize(&TutorialBubbleVert, sizeof(TutorialBubbleVert), GL_STATIC_DRAW);
            this->EBO.BindDataPointerWithSize(&quadIndices, sizeof(quadIndices), GL_STATIC_DRAW);

            this->VAO.AddAttribute<GL_FLOAT>(2);    // Position
            this->VAO.AddAttribute<GL_FLOAT>(2);    // Texcoord

            this->VAO.Unbind();


            this->TutorialBubbleTexture = new GLClasses::Texture("TutorialBubble.png", GL_MIRRORED_REPEAT, GL_REPEAT, false, GL_NEAREST, GL_NEAREST);
            
            // Shader Program

            this->Program.LoadShader(GL_VERTEX_SHADER, "TutorialBubble.vert");
            this->Program.LoadShader(GL_FRAGMENT_SHADER, "TutorialBubble.frag");

            this->Program.LinkProgram();

            this->Program.UniformBlockBinding("RenderingMatrices", Enums::UniformBufferBindings::RenderingMatrices);

            this->Program.Use();

            // Texture Bindings

            this->Program.SetTextureBinding("Texture", 0);
        };

        ~TutorialBubble() override {
            delete this->TutorialBubbleTexture;
        }

        void Draw() override {
            this->TutorialBubbleTexture->Bind(GL_TEXTURE0);

            glDisable(GL_DEPTH_TEST);
            glDisable(GL_CULL_FACE);

            ModelBase::Draw();

            glEnable(GL_DEPTH_TEST);
            glEnable(GL_CULL_FACE);

            this->TutorialBubbleTexture->Unbind();
        }
    };
}

#endif