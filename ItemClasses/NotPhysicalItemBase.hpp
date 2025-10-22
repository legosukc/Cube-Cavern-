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

namespace ItemClasses {
	
	template <class GLObjectClass = void, class OwnerClass = void>
	class NotPhysicalItemBase {
	public:
		
		typedef GLObjectClass GLObjectType;
		typedef OwnerClass OwnerType;

		NotPhysicalItemBase();
		virtual ~NotPhysicalItemBase();

		OwnerClass* Owner;

		Enums::ItemTypes::ItemTypesEnum Type;

		bool AutoUse;
		float Cooldown;

		virtual bool CanPickUp();

		virtual void Update();

		virtual void LMB();
		virtual void RMB();


		virtual void OnPickup();
		virtual void OnDrop();


		virtual void OnEquip();
		virtual void OnUnequip();

		virtual void Draw();

		virtual char* GetHoverText();

		GLObjectClass* GLObject;

	protected:

		bool Equipped = false;
	};
}

#endif