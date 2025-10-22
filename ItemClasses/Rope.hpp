#ifndef _ITEMCLASSES_ROPE
#define _ITEMCLASSES_ROPE

#include "ItemBase.hpp"
#include "./OpenGLObjects/Rope.hpp"

namespace ItemClasses {

	class Rope : public ItemClasses::ItemBase<OpenGLObjects::Rope> {
	public:
		Rope() {
			this->Name = "Rope";
			this->Description = "A loop de loop and a pull and now your shoes are lookin' real cool. Crafting material.";

			this->BookEntry = "Rope is obtained by finding them or purchasing them in shops. They serve as a basic crafting material.";
		};
	};
}

#endif