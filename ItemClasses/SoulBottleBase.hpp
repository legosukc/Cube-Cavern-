#ifndef _ITEMCLASSES_SOULBOTTLEBASE
#define _ITEMCLASSES_SOULBOTTLEBASE

#include "Bottle.hpp"
#include "./OpenGLObjects/Bottle.hpp"

#include "./GameClasses/Items.hpp"

namespace ItemClasses {

	class BottleB : public ItemClasses::Bottle {
	public:
		BottleB() {
			this->Name = "BottleB";
			this->Description = "The soul of speed. RMB to release it. Crafting material.";

			this->BookEntry = "BottleB is obtained by defeating a blue-empowered enemy and capturing their soul with a Bottle. It serves as a crafting material and can upgrade some equipment. It is the soul of speed.";
		};

		void LMB() override {}
		void RMB() override {
			this->Owner->GetHeldItem() = (GameClasses::Player::InventoryItemType*)GameClasses::Items::CreateItem<ItemClasses::Bottle>(this->Owner, this->Owner->ViewArmRight);
		}
	};
}

#endif