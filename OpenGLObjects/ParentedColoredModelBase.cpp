
#include "ParentedColoredModelBase.hpp"

#include "./LuauClasses/Vector3.hpp"

#include "./FunctionHeaders/RenderingFunctions.hpp"
#include <glm/fwd.hpp>

namespace OpenGLObjects {

    template<class GLObjectParent>
    void ParentedColoredModelBase<GLObjectParent>::Update() {

        if (this->Parent == nullptr) {
            this->ModelMatrix = RenderingFunctions::translate(glm::mat4(1.f), *this->Position);
        } else {
            this->ModelMatrix = RenderingFunctions::translate(this->Parent->ModelMatrix, this->OffsetPosition);
        }

        this->ModelMatrix = RenderingFunctions::rotate(this->ModelMatrix, this->OffsetDirection.X, LuauClasses::Vector3(1.f, 0.f, 0.f));
        this->ModelMatrix = RenderingFunctions::rotate(this->ModelMatrix, this->OffsetDirection.Y, LuauClasses::Vector3(0.f, 1.f, 0.f));
        this->ModelMatrix = RenderingFunctions::rotate(this->ModelMatrix, this->OffsetDirection.Z, LuauClasses::Vector3(0.f, 0.f, 1.f));
    }

    template<class GLObjectParent>
    void ParentedColoredModelBase<GLObjectParent>::ParentedColoredModelBaseConstructor(const char* ModelFile, GLObjectParent* ParentPtr, const char* VertexShader, const char* FragmentShader) {
        this->ColoredModelBaseConstructor(ModelFile, VertexShader, FragmentShader);

        this->Parent = ParentPtr;
    }

    template<class GLObjectParent>
    void ParentedColoredModelBase<GLObjectParent>::ParentedColoredModelBaseConstructor(const char* ModelFile, GLObjectParent* ParentPtr) {
        this->ColoredModelBaseConstructor(ModelFile);

        this->Parent = ParentPtr;
    }
};