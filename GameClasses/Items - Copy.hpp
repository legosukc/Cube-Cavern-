#ifndef _GAMECLASSES_ITEMS
#define _GAMECLASSES_ITEMS

#include <vector>

#include "./ItemClasses/ItemBase.hpp"
#include "./OpenGLObjects/ObjectBase.hpp"

namespace GameClasses::Items {

	std::vector<ItemClasses::ItemBase<OpenGLObjects::ObjectBase>*> Items;

	inline void Update() {

		for (auto& Item : Items) {

			if (Item == nullptr) {
				continue;
			}

			Item->Update();
		}
	}

	inline void Draw() {

		for (auto& Item : Items) {

			if (Item == nullptr) {
				continue;
			}

			Item->Draw();
		}
	}

	template<class ItemClass, class OwnerType = ItemClass::OwnerType, typename _inferred>
	ItemClass* CreateItem(OwnerType* Owner, _inferred* GLObjectParent) {

		ItemClass* NewItem = new ItemClass;
		NewItem->Owner = Owner;
		NewItem->GLObject = new ItemClass::GLObjectType(GLObjectParent);

		Items.push_back((ItemClasses::ItemBase<OpenGLObjects::ObjectBase>*)NewItem);

		return NewItem;
	}

	template<class ItemClass, class OwnerType = GameClasses::Player>
	ItemClass* CreateItem(OwnerType* Owner) {

		ItemClass* NewItem = new ItemClass;
		NewItem->Owner = Owner;
		NewItem->GLObject = new ItemClass::GLObjectType;

		Items.push_back((ItemClasses::ItemBase<OpenGLObjects::ObjectBase>*)NewItem);

		return NewItem;
	}
};

#endif