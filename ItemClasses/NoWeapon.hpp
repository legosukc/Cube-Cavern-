#ifndef _ITEMCLASSES_NOWEAPON
#define _ITEMCLASSES_NOWEAPON

//#include "MeleeWeaponBase.hpp"
#include "NotPhysicalItemBase.hpp"
#include "./GameClasses/Player.hpp"

namespace ItemClasses {

	class NoWeapon : public ItemClasses::NotPhysicalItemBase<void, GameClasses::Player> {

		// damage is hard coded because... fuck you.
		// Int16 Damage = 1;

	public:

		NoWeapon() {
		
			this->Type = Enums::ItemTypes::Melee;

			this->Cooldown = 0.4f;
		};

		void LMB() override {

			constexpr float SwingDuration = 0.14f;

			std::cout << "used fist\n";

			this->Owner->RightArmGoalRotation.Y = 30.f;

			const auto ResetArmY = [](GameClasses::Player* Owner) {
				Owner->RightArmGoalRotation.Y = 0.f;
				};
			LuauClasses::task::CreateDelayedTask<GameClasses::Player*>(ResetArmY, this->Owner, SwingDuration);
		}
	};
}

#endif