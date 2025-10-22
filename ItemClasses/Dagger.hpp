#ifndef _ITEMCLASSES_DAGGER
#define _ITEMCLASSES_DAGGER

#include "MeleeWeaponBase.hpp"
#include "./OpenGLObjects/Dagger.hpp"

#include <AL/al.h>
#include "./SDLClasses/Sound.hpp"

#include "./FunctionHeaders/Random.hpp"

namespace ItemClasses {

	class Dagger : public ItemClasses::MeleeWeaponBase<OpenGLObjects::Dagger> {

		SDLClasses::Sound::ALAttached3DSoundObject* Swing;

	public:
		Dagger() {
			this->Name = "Dagger";
			this->Description = "The sword's little brother, half as long, just as deadly! Deals 3 damage."; //"3 damage, pretty fuckin weak.";

			this->BookEntry = "Daggers are obtained by crafting a rock and a stick together. They do 3 damage.";

			this->Cooldown = 0.4f;
			this->SwingDuration = 0.14f;
			this->Damage = 3;

			this->KnockbackAmount = 0.5f;

			this->Swing = new SDLClasses::Sound::ALAttached3DSoundObject("sfx\\Swing.ogg", &this->Position);
		};

		void LMB() override {
			
			std::cout << "used dagger\n";
			MeleeWeaponBase::LMB();

			this->Swing->Source.FloatSetProperty(AL_PITCH, 1.f + Random::xorshiftFloat());
			this->Swing->Play();
		}
	};
}

#endif