#ifndef _ITEMCLASSES_BOTTLE
#define _ITEMCLASSES_BOTTLE

#include "MeleeWeaponBase.hpp"
#include "./OpenGLObjects/Bottle.hpp"

#include "./SDLClasses/Sound.hpp"

#include "./GameClasses/Player.hpp"

namespace ItemClasses {

	class Bottle : public ItemClasses::ItemBase<OpenGLObjects::Bottle, GameClasses::Player> {

		SDLClasses::Sound::ALAttached3DSoundObject* Swing;

	public:
		Bottle() {
			this->Name = "Bottle";
			this->Description = "Allows you to pick up souls.";

			this->BookEntry = "Bottles are obtained by finding them or purchasing them in shops. They allow you to pick up souls from enemies.";

			this->Cooldown = 0.4f;

			this->Swing = new SDLClasses::Sound::ALAttached3DSoundObject("sfx\\Swing.ogg", &this->Position);
		};

		~Bottle() override {
			delete this->Swing;
		}

		void LMB() override {
			//self.root.Sound:PlaySound("Swing", nil, nil, 1 + math.random());
			std::cout << "used bottle\n";
			
			this->Owner->RightArmGoalRotation.Y = 30.f;

			const auto ResetArmY = [](GameClasses::Player* Owner) {
				Owner->RightArmGoalRotation.Y = 0.f;
				};
			LuauClasses::task::CreateDelayedTask<GameClasses::Player*>(ResetArmY, this->Owner, 0.14f);

			this->Swing->Play();
		}
	};
}

#endif