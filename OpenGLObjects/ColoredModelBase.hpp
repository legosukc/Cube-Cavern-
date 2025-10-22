#ifndef _OPENGLOBJECTS_COLOREDMODELBASE
#define _OPENGLOBJECTS_COLOREDMODELBASE

#include "ModelBase.hpp"

namespace OpenGLObjects {

    class ColoredModelBase : public OpenGLObjects::ModelBase {
    protected:

        void ColoredModelBaseConstructor(const char* ModelName, const char* VertexShader = "ShadedColorModel.vert", const char* FragmentShader = "ShadedColor.frag");
    };
}

#endif