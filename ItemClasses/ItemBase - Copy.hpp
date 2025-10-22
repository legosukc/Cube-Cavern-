#ifndef _ITEMCLASSES_ITEMBASE
#define _ITEMCLASSES_ITEMBASE

#include "./Enums/ItemTypeEnums.hpp"

#include "./LuauClasses/Vector3.hpp"

#include "./FunctionHeaders/AppendCstring.hpp"

//#include "./OpenGLObjects/ViewArmRight.hpp"

#include "NotPhysicalItemBase.hpp"

#include "./GameClasses/Player.hpp"

#include "./IntTypes.h"
#include <cstring>

namespace ItemClasses {
	
	template <class GLObjectType, class OwnerType = GameClasses::Player>
	class ItemBase : public NotPhysicalItemBase<OwnerType, GLObjectType> {
	public:

		ItemBase() {
			
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

		const char* Name;
		const char* Description;
		const char* HoverText;

		const char* BookEntry;

		//Uint32 Thumbnail = 544316911; // TODO: change this

		Uint16 Price;
		Uint8 Rarity;
		Uint8 PickupTier;

		bool Purchasable;
		bool ShowOnDetector;

		bool MustBePurchased = false;
		
		const char* FloorHitSFX;
		float FloorHitSFXSpeed;

		bool Physics = false;

		LuauClasses::Vector3 Position;
		LuauClasses::Vector3 Velocity;

		LuauClasses::Vector3 Rotation;

		//bool ShouldDraw = true;

		virtual bool CanPickUp() override {
			return this->Owner == nullptr && !this->Physics;
		}

		virtual void Draw() override {

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

		virtual char* GetHoverText() override {
			
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
			std::strcpy(HoverText + strlen(InteractKeyText), InteractionText);

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
}

#endif