#ifndef _OPENGLOBJECT_DAGGER
#define _OPENGLOBJECT_DAGGER

#include "ViewModelItemBase.hpp"
#include "ViewArmRight.hpp"

#include "./FunctionHeaders/Maths.hpp"

#include "./LuauClasses/Vector3.hpp"

namespace OpenGLObjects {

    class Dagger : public OpenGLObjects::ViewModelItemBase {
    public:
        Dagger(OpenGLObjects::ViewArmRight* ViewArmRight) {
            this->ViewModelItemBaseConstructor("Dagger.obj", "dagger_palette.bmp", ViewArmRight);
            
            this->GripPosition = LuauClasses::Vector3(0.101002f, 0.000879f, -0.071655f);
            this->GripDirection = LuauClasses::Vector3(0.f, Maths::ToRadians(270.f), 0.f);
        };
    };
}

#endif