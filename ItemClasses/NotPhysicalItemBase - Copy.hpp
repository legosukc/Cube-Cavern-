#ifndef _ITEMCLASSES_NOTPHYSICALITEMBASE
#define _ITEMCLASSES_NOTPHYSICALITEMBASE

// i know, i know, "NotPhysicalItemBase" is a shit name
// and the fucking inheritance fucking annoying
// and pretty much everything else
// fuck me bro!!!!!!!!!!!!!!!

// this is (sadly) the TRUE item base
// yeah im gonna, kill myself
// couldn't think of a better name

// it's item base without any open gl object
// so literally only the NoWeapon item uses this directly loll
// not to save like 60 bytes of memory but for the ability to have polymorphismimsimssm
// is that how you use that word or am i retarted
// i feel like all this inheritance is gonna bite me back in the future
// like really hard
// womp womp

#include "./Enums/ItemTypeEnums.hpp"
#include "./GameClasses/Player.hpp"

namespace ItemClasses {
	
	template <class OwnerType = GameClasses::Player, typename GLObjectType = void>
	class NotPhysicalItemBase {
	public:
		
		typedef OwnerType OwnerType;
		typedef GLObjectType GLObjectType;

		NotPhysicalItemBase() {

			this->Type = Enums::ItemTypes::Item;

			this->AutoUse = false;
			this->Cooldown = 0.1f;

			this->GLObject = nullptr;
		};

		virtual ~NotPhysicalItemBase() = default;

		OwnerType* Owner;

		Enums::ItemTypes::ItemTypesEnum Type;

		bool AutoUse;
		float Cooldown;

		virtual bool CanPickUp() {
			return false;
		}

		virtual void Update() {}

		virtual void LMB() {}
		virtual void RMB() {}


		virtual void OnPickup() {}
		virtual void OnDrop() {
			this->Equipped = false;
		}


		virtual void OnEquip() {
			this->Equipped = true;
		}
		virtual void OnUnequip() {
			this->Equipped = false;
		}

		virtual void Draw() {}

		virtual char* GetHoverText() {
			return nullptr;
		}

		GLObjectType* GLObject;

	protected:

		bool Equipped = false;
	};
}

#endif