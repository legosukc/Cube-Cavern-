#ifndef _ITEMCLASSES_DAGGER
#define _ITEMCLASSES_DAGGER

#include "./Enums/ItemTypeEnums.hpp"

#include "./ItemClasses/ItemBase.hpp"

#include "./OpenGLObjects/Dagger.hpp"

#include "./GameClasses/Player.hpp"

#include "./LuauClasses/task.hpp"

namespace ItemClasses {

	class Dagger : public ItemClasses::ItemBase<OpenGLObjects::Dagger, GameClasses::Player> {
	public:
		Dagger() {
			this->Name = "Dagger";
			this->Description = "3 damage, pretty fuckin weak.";

			this->Type = Enums::ItemTypes::Melee;

			this->SwingDuration = 0.25f;
			this->Cooldown = 0.3f;

			this->Damage = 3;
		}

		float SwingDuration;
		Uint32 Damage;

		void LMB() override {

			std::cout << "used dagger\n";

			this->Owner->ArmYGoal = 30.f;

			const auto ResetArmY = [](GameClasses::Player* Owner) {
				Owner->ArmYGoal = 0.f;
				};
			LuauClasses::task::CreateDelayedTask<GameClasses::Player*>(ResetArmY, this->Owner, this->SwingDuration);
		}
	};
}

#endif