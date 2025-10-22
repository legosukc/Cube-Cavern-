#ifndef _OPENGLOBJECTS_PARENTEDCOLOREDMODELBASE
#define _OPENGLOBJECTS_PARENTEDCOLOREDMODELBASE

#include "./LuauClasses/Vector3.hpp"

#include "ColoredModelBase.hpp"
#include "ModelBase.hpp"

namespace OpenGLObjects {

    template<class GLObjectParent = OpenGLObjects::ModelBase>
    class ParentedColoredModelBase : public OpenGLObjects::ColoredModelBase {
    public:

        typedef GLObjectParent ParentClass;

        GLObjectParent* Parent;

        LuauClasses::Vector3* Position = nullptr;

        LuauClasses::Vector3 OffsetPosition;
        LuauClasses::Vector3 OffsetDirection;

        virtual void Update() override;

    protected:

        void ParentedColoredModelBaseConstructor(const char* ModelFile, GLObjectParent* ParentPtr, const char* VertexShader, const char* FragmentShader);
        void ParentedColoredModelBaseConstructor(const char* ModelFile, GLObjectParent* ParentPtr);
    };
}

#endif