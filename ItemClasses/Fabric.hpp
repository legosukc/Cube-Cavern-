#ifndef _ITEMCLASSES_FABRIC
#define _ITEMCLASSES_FABRIC

#include "ItemBase.hpp"
#include "./OpenGLObjects/Fabric.hpp"

namespace ItemClasses {

	class Fabric : public ItemClasses::ItemBase<OpenGLObjects::Fabric> {
	public:
		Fabric() {
			this->Name = "Fabric";
			this->Description = "Good for most things, except most things. Crafting material.";

			this->BookEntry = "Fabric is obtained by finding them or purchasing them in shops. It serves as a basic crafting material.";
		};
	};
}

#endif