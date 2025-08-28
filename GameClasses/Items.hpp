#ifndef _GAMECLASSES_ITEMS
#define _GAMECLASSES_ITEMS

#include <vector>

#include "./ItemClasses/ItemBase.hpp"

namespace GameClasses::Items {

	std::vector<ItemClasses::ItemBase<>*> Items;

	void Update() {

		for (auto& Item : Items) {

			if (Item == nullptr) {
				continue;
			}

			if (Item->Update == nullptr) {
				continue;
			}

			Item->Update(Item);
		}
	}
}

#endif