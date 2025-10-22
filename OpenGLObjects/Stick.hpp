#ifndef _OPENGLOBJECT_STICK
#define _OPENGLOBJECT_STICK

#include "ViewArmRight.hpp"
#include "ParentedColoredModelBase.hpp"

#include "./OtherClasses/GLClasses.hpp"

#include "./LuauClasses/Vector3.hpp"

namespace OpenGLObjects {

	class Stick : public OpenGLObjects::ParentedColoredModelBase<OpenGLObjects::ViewArmRight> {
	public:
		Stick(OpenGLObjects::ViewArmRight* ViewArmRight = nullptr) {
			this->ParentedColoredModelBaseConstructor("Stick.obj", ViewArmRight);

			this->Program.Vec3SetUniform("Color", LuauClasses::Vector3(106.f, 57.f, 9.f) / 255.f);

			this->OffsetPosition = LuauClasses::Vector3(0.00876f, 0.13f, -0.103351f);
		};
	};
}

#endif