#ifndef _ITEMCLASSES_STICK
#define _ITEMCLASSES_STICK

#include <SDL2/SDL_timer.h>

#include "MeleeWeaponBase.hpp"
#include "./OpenGLObjects/Stick.hpp"

#include <AL/al.h>
#include "./SDLClasses/Sound.hpp"

#include "./FunctionHeaders/Random.hpp"

namespace ItemClasses {

	class Stick : public ItemClasses::MeleeWeaponBase<OpenGLObjects::Stick> {

		SDLClasses::Sound::ALAttached3DSoundObject* Swing;

	public:
		Stick() {
			this->Name = "Stick";
			this->Description = "How did this get down here? Deals 2 damage. Crafting material.";

			this->BookEntry = "Sticks are obtained by finding them or from shops. They do 2 damage and serves as a basic crafting material.";

			this->Cooldown = 0.4f;
			this->SwingDuration = 0.14f;
			this->Damage = 2;

			this->KnockbackAmount = 0.5f;

			this->Swing = new SDLClasses::Sound::ALAttached3DSoundObject("sfx\\Swing.ogg", &this->Position);
		};

		void LMB() override {
			//std::cout << "used stick\n";
			
			MeleeWeaponBase::LMB();

			this->Swing->Source.FloatSetProperty(AL_PITCH, 1.f + Random::xorshiftFloat(SDL_GetTicks() * 16));
			this->Swing->Play();
		}
		/*
		void DrawFirstPerson() override {
			this->GLObject->ViewArmRight = this->Owner->ViewArmRight;

			this->GLObject->Draw();
		}*/
	};
}

#endif