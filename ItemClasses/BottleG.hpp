#ifndef _ITEMCLASSES_BOTTLEG
#define _ITEMCLASSES_BOTTLEG

#include "MeleeWeaponBase.hpp"
#include "./OpenGLObjects/Dagger.hpp"

#include "./SDLClasses/Sound.hpp"

namespace ItemClasses {

	class Dagger : public ItemClasses::MeleeWeaponBase<OpenGLObjects::Dagger> {

		std::unique_ptr<SDLClasses::Sound::SoundObject>* Swing;

	public:
		Dagger() {
			this->Name = "Dagger";
			this->Description = "The sword's little brother, half as long, just as deadly! Deals 3 damage."; //"3 damage, pretty fuckin weak.";

			this->BookEntry = "Daggers are obtained by crafting a rock and a stick together. They do 3 damage.";

			this->Cooldown = 0.4f;
			this->SwingDuration = 0.14f;
			this->Damage = 3;

			this->KnockbackAmount = 0.5f;

			this->Swing = &SDLClasses::Sound::LoadSoundObject("Swing");
		};

		void LMB() override {
			//self.root.Sound:PlaySound("Swing", nil, nil, 1 + math.random())
			std::cout << "used dagger\n";
			
			MeleeWeaponBase::LMB();

			this->Swing->get()->Play();
		}
	};
}

#endif