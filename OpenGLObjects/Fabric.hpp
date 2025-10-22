#ifndef _OPENGLOBJECT_FABRIC
#define _OPENGLOBJECT_FABRIC

#include "ParentedTexturedModelBase.hpp"
#include "ViewArmRight.hpp"

namespace OpenGLObjects {

	class Fabric : public OpenGLObjects::ParentedTexturedModelBase<OpenGLObjects::ViewArmRight> {
	public:
		Fabric(OpenGLObjects::ViewArmRight* Parent) {
			this->ParentedTexturedModelBaseConstructor("Fabric.obj", "fabric_palette.bmp", Parent, nullptr, nullptr);

			this->OffsetPosition = LuauClasses::Vector3(0.101002f, 0.000879f, -0.071655f);
			this->OffsetDirection = LuauClasses::Vector3(0.f, Maths::ToRadians(270.f), 0.f);

			this->Program = Parent->Program;
		};
	};
}

#endif