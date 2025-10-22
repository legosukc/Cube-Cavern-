#ifndef _ITEMCLASSES_MELEEWEAPONBASE
#define _ITEMCLASSES_MELEEWEAPONBASE

#include "WeaponBase.hpp"
#include "./OpenGLObjects/ModelBase.hpp"

#include "./GameClasses/Player.hpp"

#include "./Enums/ItemTypeEnums.hpp"

namespace ItemClasses {

	template<class GLObjectClass = OpenGLObjects::ModelBase>
	class MeleeWeaponBase : public ItemClasses::WeaponBase<GLObjectClass> {
	public:
		inline MeleeWeaponBase() {
			this->Type = Enums::ItemTypes::Melee;
			this->SwingDuration = 0.f;
		};

		float SwingDuration;

		virtual void LMB() override {

			this->Owner->RightArmGoalRotation.Y = 30.f;

			const auto ResetArmY = [](GameClasses::Player* Owner) {
				Owner->RightArmGoalRotation.Y = 0.f;
				};
			LuauClasses::task::CreateDelayedTask<GameClasses::Player*>(ResetArmY, this->Owner, this->SwingDuration);
		}
	};
}

#endif