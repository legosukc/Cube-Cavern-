#ifndef _OPENGLOBJECTS_COLOREDITEMBASE
#define _OPENGLOBJECTS_COLOREDITEMBASE

#include "./OtherClasses/GLClasses.hpp"

#include "./LuauClasses/Vector3.hpp"

#include "./FunctionHeaders/RenderingFunctions.hpp"

//#include "ViewArmRight.hpp"
#include "ColoredModelBase.hpp"
#include "ModelBase.hpp"

namespace OpenGLObjects {

    template<class GLObjectParent = OpenGLObjects::ModelBase>
    class ParentedColoredModelBase : public OpenGLObjects::ColoredModelBase {
    public:

        typedef GLObjectParent ParentType;

        GLObjectParent* Parent;

        LuauClasses::Vector3* Position = nullptr;

        LuauClasses::Vector3 OffsetPosition;
        LuauClasses::Vector3 OffsetDirection;

        virtual void Update() {

            if (this->Parent == nullptr) {
                this->ModelMatrix = RenderingFunctions::translate(glm::mat4(1.f), *this->Position);
            } else {
                this->ModelMatrix = RenderingFunctions::translate(this->Parent->ModelMatrix, this->OffsetPosition);
            }
            
            this->ModelMatrix = RenderingFunctions::rotate(this->ModelMatrix, this->OffsetDirection.X, LuauClasses::Vector3(1.f, 0.f, 0.f));
            this->ModelMatrix = RenderingFunctions::rotate(this->ModelMatrix, this->OffsetDirection.Y, LuauClasses::Vector3(0.f, 1.f, 0.f));
            this->ModelMatrix = RenderingFunctions::rotate(this->ModelMatrix, this->OffsetDirection.Z, LuauClasses::Vector3(0.f, 0.f, 1.f));
        }

    protected:

        inline void ParentedColoredModelBaseConstructor(const char* ModelFile, GLObjectParent* ParentPtr, const char* VertexShader, const char* FragmentShader) {
            this->ColoredModelBaseConstructor(ModelFile, VertexShader, FragmentShader);

            this->Parent = ParentPtr;
        }

        inline void ParentedColoredModelBaseConstructor(const char* ModelFile, GLObjectParent* ParentPtr) {
            this->ColoredModelBaseConstructor(ModelFile);

            this->Parent = ParentPtr;
        }
    };
}

#endif