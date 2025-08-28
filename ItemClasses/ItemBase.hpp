#ifndef _ITEMCLASSES_ITEMBASE
#define _ITEMCLASSES_ITEMBASE

#include "./Enums/ItemTypeEnums.hpp"
#include "./OpenGLObjects/ObjectBase.hpp"

#include "./LuauClasses/Vector3.hpp"

#include "./GameClasses/Player.hpp"

#include <SDL2/SDL_stdinc.h>

#include <format>
#include <string>

namespace ItemClasses {

	void a() {
		int a = true ? 0 : 1;
	}
	
	template <class SelfType = void, typename GLObjectType = OpenGLObjects::ModelBase<>>
	class ItemBase {

		static void _Update(SelfType* self);

		static void _LMB(SelfType* self, GameClasses::Player* Player);
		static void _RMB(SelfType* self, GameClasses::Player* Player);

		static const char* _GetHoverText(SelfType* self) {
			
			const char* InteractionText;
			if (self->MustBePurchased) {
				InteractionText = "inspect";
			} else {
				InteractionText = (Enums::ItemTypes::Item and self->Type != Enums::ItemTypes::Pickup) ? "wear" : "take";
			}
			
			return std::format("E to %s %s.", InteractionText, self->Name);
		}

	public:
		ItemBase() {
			this->CreationTime = SDL_GetTicks64();
		}
		~ItemBase() {

		}

		void(*Update)(SelfType* self) = nullptr;

		void(*LMB)(SelfType* self) = nullptr;
		void(*RMB)(SelfType* self) = nullptr;

		void(*GetHoverText)(SelfType* self) = nullptr;

		const char* Name = "Item Base";
		const char* Description = nullptr;
		const char* HoverText = nullptr;

		const char* BookEntry = nullptr;

		Uint32 Thumbnail = 544316911; // TODO: change this

		const Enums::ItemTypes::ItemTypesEnum Type = Enums::ItemTypes::Item;

		Uint16 Price = 0;
		Uint16 Rarity = 0;
		Uint16 PickupTier = 0;

		bool AutoUse = false;
		bool Purchasable = false;
		bool ShowOnDetector = true;

		float Cooldown = 0.f;
		
		const char* FloorHitSFX = "FloorHit";
		float FloorHitSFXSpeed = 1;

		bool Physics = false;
		Uint64 CreationTime;

		LuauClasses::Vector3 Position;
		LuauClasses::Vector3 Velocity;

		LuauClasses::Vector3 Rotation;

		GLObjectType* GLObject;

	private:

	};
}

#endif