#ifndef _OPENGLOBJECT_BOTTLE
#define _OPENGLOBJECT_BOTTLE

#include "ParentedColoredModelBase.hpp"
#include "ViewArmRight.hpp"

#include "./FunctionHeaders/Maths.hpp"

#include "./LuauClasses/Vector3.hpp"

#include "./OtherClasses/GLClasses.hpp"

namespace OpenGLObjects {

    class Bottle : public OpenGLObjects::ParentedColoredModelBase<OpenGLObjects::ViewArmRight> {
    public:
        Bottle(OpenGLObjects::ViewArmRight* ViewArmRight) {

            this->ParentedColoredModelBaseConstructor("EmptyBottleNoTexcoord.obj", ViewArmRight);
            //this->ColoredViewModelItemBaseConstructor("EmptyBottleNoTexcoord.obj", "dagger_palette.bmp", ViewArmRight, nullptr, nullptr);

            /*
            // Shaders
            this->Program.LoadShader(GL_VERTEX_SHADER, "Bottle.vert");
            this->Program.LoadShader(GL_FRAGMENT_SHADER, "Bottle.frag");

            this->Program.LinkProgram();

            this->Program.UniformBlockBinding("RenderingMatrices", Enums::UniformBufferBindings::RenderingMatrices);

            this->Program.Use();


            this->GripPosition = LuauClasses::Vector3(0.101002f, 0.000879f, -0.071655f);
            this->GripDirection = LuauClasses::Vector3(0.f, Maths::ToRadians(270.f), 0.f);

            this->Program.~Program();
            this->Program = ViewArmRight->Program;*/
        };
    };
}

#endif