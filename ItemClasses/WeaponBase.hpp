#ifndef _ITEMCLASSES_WEAPONBASE
#define _ITEMCLASSES_WEAPONBASE

#include "ItemBase.hpp"
#include "./OpenGLObjects/ModelBase.hpp"

#include "./GameClasses/Player.hpp"

#include "./IntTypes.h"

namespace ItemClasses {

	template<class GLObjectType = OpenGLObjects::ModelBase>
	class WeaponBase : public ItemClasses::ItemBase<GLObjectType, GameClasses::Player> {
	public:
		WeaponBase() {
			this->SwingDuration = 0.05f;
			this->Damage = 0;
			this->Range = 4.25f;

			this->KnockbackAmount = 0.5f;
			this->SlowsEnemies = false;
		};

		float SwingDuration;
		Int16 Damage;
		float Range;

		float KnockbackAmount;
		bool SlowsEnemies;
	};
}

#endif