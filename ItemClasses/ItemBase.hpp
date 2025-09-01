#ifndef _ITEMCLASSES_ITEMBASE
#define _ITEMCLASSES_ITEMBASE

#include "./Enums/ItemTypeEnums.hpp"
#include "./OpenGLObjects/ModelBase.hpp"

#include "./LuauClasses/Vector3.hpp"

#include <SDL2/SDL_stdinc.h>

#include <format>
#include <string>

namespace ItemClasses {
	
	template <typename GLObjectType = OpenGLObjects::ModelBase, typename OwnerType = void>
	class ItemBase {
	public:

		virtual void Update() {}

		virtual void LMB() {}
		virtual void RMB() {}

		virtual const char* GetHoverText() {

			const char* InteractionText;
			if (this->MustBePurchased) {
				InteractionText = "inspect";
			} else {
				InteractionText = (Enums::ItemTypes::Item and this->Type != Enums::ItemTypes::Pickup) ? "wear" : "take";
			}

			return std::format("E to %s %s.", InteractionText, this->Name).c_str();
		}

		OwnerType* Owner = nullptr;

		const char* Name = "";
		const char* Description = "";
		const char* HoverText = "";

		const char* BookEntry = "";

		//Uint32 Thumbnail = 544316911; // TODO: change this

		Enums::ItemTypes::ItemTypesEnum Type = Enums::ItemTypes::Item;

		Uint16 Price = 0;
		Uint16 Rarity = 0;
		Uint16 PickupTier = 0;

		bool AutoUse = false;
		bool Purchasable = false;
		bool ShowOnDetector = true;

		bool MustBePurchased = false;

		float Cooldown = 0.f;
		
		const char* FloorHitSFX = "FloorHit";
		float FloorHitSFXSpeed = 1;

		bool Physics = false;

		LuauClasses::Vector3 Position = LuauClasses::Vector3(0.f);
		LuauClasses::Vector3 Velocity = LuauClasses::Vector3(0.f);

		LuauClasses::Vector3 Rotation = LuauClasses::Vector3(0.f);

		GLObjectType* GLObject = nullptr;
	};
}

#endif