
#include "NotPhysicalItemBase.hpp"

#include "./Enums/ItemTypeEnums.hpp"

namespace ItemClasses {

	template <class GLObjectClass, class OwnerClass>
	NotPhysicalItemBase<GLObjectClass, OwnerClass>::NotPhysicalItemBase() {

		this->Type = Enums::ItemTypes::Item;

		this->AutoUse = false;
		this->Cooldown = 0.1f;

		this->GLObject = nullptr;
	};

	template <class GLObjectClass, class OwnerClass>
	NotPhysicalItemBase<GLObjectClass, OwnerClass>::~NotPhysicalItemBase() = default;


	template <class GLObjectClass, class OwnerClass>
	bool NotPhysicalItemBase<GLObjectClass, OwnerClass>::CanPickUp() {
		return false;
	}

	template <class GLObjectClass, class OwnerClass>
	void NotPhysicalItemBase<GLObjectClass, OwnerClass>::Update() {
	}


	template <class GLObjectClass, class OwnerClass>
	void NotPhysicalItemBase<GLObjectClass, OwnerClass>::LMB() {
	}

	template <class GLObjectClass, class OwnerClass>
	void NotPhysicalItemBase<GLObjectClass, OwnerClass>::RMB() {
	}


	template <class GLObjectClass, class OwnerClass>
	void NotPhysicalItemBase<GLObjectClass, OwnerClass>::OnPickup() {
	}

	template <class GLObjectClass, class OwnerClass>
	void NotPhysicalItemBase<GLObjectClass, OwnerClass>::OnDrop() {
		this->Equipped = false;
	}


	template <class GLObjectClass, class OwnerClass>
	void NotPhysicalItemBase<GLObjectClass, OwnerClass>::OnEquip() {
		this->Equipped = true;
	}

	template <class GLObjectClass, class OwnerClass>
	void NotPhysicalItemBase<GLObjectClass, OwnerClass>::OnUnequip() {
		this->Equipped = false;
	}


	template <class GLObjectClass, class OwnerClass>
	void NotPhysicalItemBase<GLObjectClass, OwnerClass>::Draw() {}


	template <class GLObjectClass, class OwnerClass>
	char* NotPhysicalItemBase<GLObjectClass, OwnerClass>::GetHoverText() {
		return nullptr;
	}
};