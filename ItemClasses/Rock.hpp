#ifndef _ITEMCLASSES_DAGGER
#define _ITEMCLASSES_DAGGER

#include "WeaponBase.hpp"
#include "./OpenGLObjects/Rock.hpp"

#include "./SDLClasses/Sound.hpp"

namespace ItemClasses {

	class Rock : public ItemClasses::WeaponBase<OpenGLObjects::Rock> {

		SDLClasses::Sound::ALAttached3DSoundObject* Swing;

	public:
		Rock() {
			this->Name = "Rock";
			this->Description = "He who throws stones is pretty cool 2 throwing damage. Crafting material.";

			this->BookEntry = "Rocks are obtained by finding the or purchasing them in shops. They are a throwing weapon and do 2 damage on contact, also serves as a basic crafting material.";

			this->Cooldown = 0.4f;
			this->SwingDuration = 0.14f;
			this->Damage = 2;

			this->KnockbackAmount = 0.5f;

			this->Swing = new SDLClasses::Sound::ALAttached3DSoundObject("sfx\\Swing.ogg", &this->Position);
		};

		virtual ~Rock() {
			delete this->Swing;
		}

		void LMB() override {
			//self.root.Sound:PlaySound("Swing", nil, nil, 1 + math.random())
			std::cout << "threw rockr\n";
			
			//MeleeWeaponBase::LMB();

			this->Swing->Play();
		}
	};
}

#endif