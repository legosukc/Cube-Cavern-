#ifndef _ITEMCLASSES_ITEMBASE
#define _ITEMCLASSES_ITEMBASE

#include "NotPhysicalItemBase.hpp"

#include <SDL2/SDL_stdinc.h>
#include "./LuauClasses/Vector3.hpp"

namespace ItemClasses {
	
	template <class GLObjectClass = void, class OwnerClass = void>
	class ItemBase : public ItemClasses::NotPhysicalItemBase<GLObjectClass, OwnerClass> {
	public:

		ItemBase();

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

		virtual bool CanPickUp() override;

		virtual void Draw() override;

		virtual char* GetHoverText() override;
	};
}

#endif