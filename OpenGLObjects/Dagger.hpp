#ifndef _OPENGLOBJECT_DAGGER
#define _OPENGLOBJECT_DAGGER

#include "ParentedTexturedModelBase.hpp"
#include "ViewArmRight.hpp"
//#include "ModelBase.hpp"

#include "./FunctionHeaders/Maths.hpp"

#include "./LuauClasses/Vector3.hpp"

#include "./OtherClasses/GLClasses.hpp"

namespace OpenGLObjects {

    class Dagger : public OpenGLObjects::ParentedTexturedModelBase<OpenGLObjects::ViewArmRight> {
    public:
        Dagger(OpenGLObjects::ViewArmRight* ViewArmRight = nullptr) {
            
            this->ParentedTexturedModelBaseConstructor("Dagger.obj", "dagger_palette.bmp", ViewArmRight, nullptr, nullptr);
            
            this->OffsetPosition = LuauClasses::Vector3(0.101002f, 0.000879f, -0.071655f);
            this->OffsetDirection = LuauClasses::Vector3(0.f, Maths::ToRadians(270.f), 0.f);

            this->Program = ViewArmRight->Program;
        };
    };
}

#endif