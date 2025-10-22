
#include "ItemBase.hpp"

#include <glad/glad.h>

#include "./Enums/ItemTypeEnums.hpp"

#include "./LuauClasses/Vector3.hpp"

#include "./FunctionHeaders/AppendCstring.hpp"
#include <cstring>

namespace ItemClasses {

	template <class GLObjectClass, class OwnerClass>
	ItemBase<GLObjectClass, OwnerClass>::ItemBase() {

		this->Name = "";
		this->Description = "";
		this->HoverText = "";

		this->BookEntry = "";

		this->Price = 0;
		this->Rarity = 0;
		this->PickupTier = 0;

		this->Purchasable = false;
		this->ShowOnDetector = true;

		this->FloorHitSFX = "FloorHit";
		this->FloorHitSFXSpeed = 1.f;

		this->Position = LuauClasses::Vector3(0.f);
		this->Velocity = LuauClasses::Vector3(0.f);

		this->Rotation = LuauClasses::Vector3(0.f);
	};

	template <class GLObjectClass, class OwnerClass>
	bool ItemBase<GLObjectClass, OwnerClass>::CanPickUp() {
		return this->Owner == nullptr && !this->Physics;
	}

	template <class GLObjectClass, class OwnerClass>
	void ItemBase<GLObjectClass, OwnerClass>::Draw() {

		if (this->Owner != nullptr) {

			if (!this->Equipped) {
				return;
			}

			glStencilMask(0xFF);
			glStencilFunc(GL_ALWAYS, 1, 0xFF);
			glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);
		}

		this->GLObject->Draw();

		glStencilOp(GL_KEEP, GL_KEEP, GL_KEEP);
		glStencilFunc(GL_EQUAL, 0, 0xFF);
	}

	template <class GLObjectClass, class OwnerClass>
	char* ItemBase<GLObjectClass, OwnerClass>::GetHoverText() {

		const char* InteractionText;
		if (this->MustBePurchased) {
			InteractionText = "inspect";
		} else {
			InteractionText = (Enums::ItemTypes::Item && this->Type != Enums::ItemTypes::Pickup) ? "wear" : "take";
		}

		const char* InteractKeyText = "E to ";

		const size_t HoverTextAllocSize = std::strlen(InteractKeyText) + std::strlen(InteractionText) + std::strlen(this->Name) + 1;
		char* HoverText = new char[HoverTextAllocSize];

		std::strcpy(HoverText, InteractKeyText);
		std::strcpy(HoverText + std::strlen(InteractKeyText), InteractionText);

		HoverText[HoverTextAllocSize] = '\0';

		return AppendCstring::AppendCstringFree(
			AppendCstring::AppendCharacterFree(
				AppendCstring::AppendCstring("E to ", InteractionText),
				' '
			),
			this->Name
		);
	}
};