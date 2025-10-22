#ifndef _OPENGLOBJECT_VIEWARMRIGHT
#define _OPENGLOBJECT_VIEWARMRIGHT

#include "TexturedModelBase.hpp"

#include "./FunctionHeaders/RenderingFunctions.hpp"
#include "./FunctionHeaders/Maths.hpp"

#include <glm/glm.hpp>
#include <glm/fwd.hpp>

#include "./LuauClasses/Vector3.hpp"

namespace OpenGLObjects {

    class ViewArmRight : public OpenGLObjects::TexturedModelBase {

        LuauClasses::Vector3* RightArmRotation;

    public:
        ViewArmRight(LuauClasses::Vector3* RightArmRotation) {

            this->TexturedModelBaseConstructor("ViewArm.obj", "viewarm_palette.bmp");

            this->RightArmRotation = RightArmRotation;
        };
        
        void Update() override {
            
            this->ModelMatrix = RenderingFunctions::rotate(glm::mat4(1.f), Maths::ToRadians(this->RightArmRotation->X), LuauClasses::Vector3(1.f, 0.f, 0.f));
            this->ModelMatrix = RenderingFunctions::rotate(this->ModelMatrix, Maths::ToRadians(this->RightArmRotation->Y), LuauClasses::Vector3(0.f, 1.f, 0.f));
            this->ModelMatrix = RenderingFunctions::rotate(this->ModelMatrix, Maths::ToRadians(this->RightArmRotation->Z), LuauClasses::Vector3(0.f, 0.f, 1.f));
            
            //this->ModelMatrix = RenderingFunctions::rotate(glm::mat4(1.f), 1.f, (*this->RightArmRotation) / 360.f);
            //constexpr float Radians = Maths::Pi / 180.f;

            this->ModelMatrix = RenderingFunctions::translate(this->ModelMatrix, LuauClasses::Vector3(0.175f, -0.2f, -0.35f));
        }
    };
}

#endif