#ifndef _OPENGLOBJECTS_VIEWMODELITEMBASE
#define _OPENGLOBJECTS_VIEWMODELITEMBASE

#include "./OtherClasses/GLClasses.hpp"

#include "ViewModelObjectBase.hpp"

#include "ViewArmRight.hpp"

namespace OpenGLObjects {

    class ViewModelItemBase : public OpenGLObjects::ViewModelObjectBase {
    public:

        LuauClasses::Vector3 GripPosition;
        LuauClasses::Vector3 GripDirection;

        virtual void Update() override {

            this->Program.Use();
            
            this->ModelMatrix = RenderingFunctions::translate(this->ViewArmRight->ModelMatrix, this->GripPosition);

            this->ModelMatrix = RenderingFunctions::rotate(this->ModelMatrix, this->GripDirection.X, LuauClasses::Vector3(1.f, 0.f, 0.f));
            this->ModelMatrix = RenderingFunctions::rotate(this->ModelMatrix, this->GripDirection.Y, LuauClasses::Vector3(0.f, 1.f, 0.f));
            this->ModelMatrix = RenderingFunctions::rotate(this->ModelMatrix, this->GripDirection.Z, LuauClasses::Vector3(0.f, 0.f, 1.f));

            this->Program.Mat4SetUniform(this->ModelUniformID, this->ModelMatrix);
        }

    protected:

        void ViewModelItemBaseConstructor(const char* ModelFile, const char* PaletteFile, OpenGLObjects::ViewArmRight* ViewArmRight) {
            this->ViewModelObjectBaseConstructor(ModelFile, PaletteFile);

            this->ViewArmRight = ViewArmRight;
        }

        OpenGLObjects::ViewArmRight* ViewArmRight;
    };
}

#endif