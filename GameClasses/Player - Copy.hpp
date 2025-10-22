#ifndef _PLAYER_HEADER
#define _PLAYER_HEADER

#include <immintrin.h>

#include "./IntTypes.h"

#include "./LuauClasses/Vector3.hpp"
#include "./LuauClasses/task.hpp"

#include "./SDLClasses/InputHandler.hpp"

#include "./FunctionHeaders/RenderingFunctions.hpp"
#include "./FunctionHeaders/Maths.hpp"

#include "./ItemClasses/ItemBase.hpp"

#include "./OpenGLObjects/ViewArmRight.hpp"
#include "./OpenGLObjects/ModelBase.hpp"

#include "./OtherClasses/ExpandableArray.hpp"

#include "./GameClasses/Items.hpp"

namespace GameClasses {

	class Player {
	public:

		typedef ItemClasses::ItemBase<OpenGLObjects::ModelBase, Player> InventoryItemType;

		OpenGLObjects::ViewArmRight* ViewArmRight;

		Player(const double* DeltaTimePtr, InventoryItemType* NoWeaponRef) {
			using namespace SDLClasses;

			this->NoWeaponRef = NoWeaponRef;

			//this->Items.resize(this->InventorySlots);
			this->Items.SetSizeTo(this->InventorySlots); //reserve(this->InventorySlots);
			this->Items.Fill(NoWeaponRef);
			NoWeaponRef->Owner = this;

			this->ViewArmRight = new OpenGLObjects::ViewArmRight(&this->RightArmRotation);

			this->Delta = DeltaTimePtr;

			this->Position = LuauClasses::Vector3(0.f, 0.f, -2.f);
			this->Velocity = LuauClasses::Vector3(0.f);

			this->LookVector = LuauClasses::Vector3(0.f, 0.f, -1.f);
			this->RightVector = LuauClasses::Vector3(0.f);
			this->UpVector = LuauClasses::Vector3(0.f, 1.f, 0.f);

			this->UpdateDirectionalVectors();

			const auto InputDown = [](void* Connector, SDL_Scancode Scancode) {

				Player* self = reinterpret_cast<Player*>(Connector);

				if (Uint8(Scancode - SDL_SCANCODE_1) < self->InventorySlots && !self->OnItemCooldown) {

					InventoryItemType* OldEquipedItem = self->GetHeldItem();
					self->HeldItemSlot = Uint8(Scancode - SDL_SCANCODE_1);

					self->GetHeldItem()->OnEquip();
					OldEquipedItem->OnUnequip();

					return;
				}

				switch (Scancode) {
					
					case SDL_SCANCODE_SPACE:
						std::cout << "jump\n";
						break;
					case SDL_SCANCODE_LSHIFT:
						std::cout << "crawl\n";
						break;
				}
				};

			InputHandler::AddInputDownConnection(InputDown, this);

			const auto MouseButtonDown = [](void* Connector, SDL_MouseButtonEvent* MouseButtonEvent) {
				using namespace SDLClasses;

				Player* self = reinterpret_cast<Player*>(Connector);

				switch (MouseButtonEvent->button) {
					case InputHandler::MouseButtonEnums::LeftMouse:

						if (self->OnItemCooldown) {
							break;
						}

						InventoryItemType* HeldItem = self->GetHeldItem();

						self->OnItemCooldown = true;
						HeldItem->LMB();

						const auto ResetItemCooldown = [](GameClasses::Player* self) {
							self->OnItemCooldown = false;
							std::cout << "cooldown over\n";
							};
						LuauClasses::task::CreateDelayedTask<GameClasses::Player*>(ResetItemCooldown, self, HeldItem->Cooldown);
						break;
				}
				};

			InputHandler::AddMouseButtonDownConnection(MouseButtonDown, this);
		}

		LuauClasses::Vector3 Position;
		LuauClasses::Vector3 Velocity;

		float Yaw = -90.f;
		float Pitch = 0.f;
		float Roll = 0.f;

		float LookSensitivity = 0.5f;

		LuauClasses::Vector3 LookVector;
		LuauClasses::Vector3 RightVector;
		LuauClasses::Vector3 UpVector;

		float FOV = 70.f;

		// Left Arm Rotation
		LuauClasses::Vector3 LeftArmRotation;
		LuauClasses::Vector3 LeftArmGoalRotation;

		// Right Arm Rotation
		LuauClasses::Vector3 RightArmRotation;
		LuauClasses::Vector3 RightArmGoalRotation;


		Uint8 InventorySlots = 3;

		Uint8 HeldItemSlot = 0;
		bool OnItemCooldown = false;
		
		OtherClasses::ExpandableArray<InventoryItemType*, Uint8> Items;

		template<typename ItemClass = InventoryItemType>
		bool OverwriteInventoryItem(Uint8 Slot) {
			
			if (Slot < this->InventorySlots) {

				ItemClass* NewItem = GameClasses::Items::CreateItem<ItemClass, Player>(this, this->ViewArmRight);

				if (this->Items[Slot] != this->NoWeaponRef) {
					delete this->Items[Slot];
				}

				this->Items[Slot] = (InventoryItemType*)NewItem;
				return true;
			}
			
			return false;
		}

		template<class ItemClass = InventoryItemType>
		inline void AddItemToInventory(ItemClass* Item) {

			for (Uint8 i = 0; i < this->InventorySlots; ++i) {
				if (this->Items[i] == this->NoWeaponRef) {

					Item->Owner = this;

					if (Item->GLObject != nullptr) {
						Item->GLObject->Position = &Item->Position;
					}
					
					this->Items[i] = (InventoryItemType*)Item;
					break;
				}
			}
		}

		template<class ItemClass>
		inline void AddItemToInventory() {
			this->AddItemToInventory<ItemClass>(GameClasses::Items::CreateItem<ItemClass, Player>(this, this->ViewArmRight));
		}

		InventoryItemType*& GetHeldItem() {

			if (const Uint8 LastSize = this->Items.ReservedSize(); this->HeldItemSlot > LastSize) {
				this->Items.SetSizeTo(this->HeldItemSlot);
				this->Items.Fill(LastSize, this->HeldItemSlot, this->NoWeaponRef);
			}

			return this->Items[this->HeldItemSlot];
		}


		void UpdateDirectionalVectors() {

			const __m128 RadiansVec = _mm_mul_ps(
				_mm_setr_ps(this->Yaw, this->Pitch, this->Roll, 0.f),
				_mm_set1_ps(static_cast<float>(Maths::Pi / 180.0))
			);

			const float RollRadians = RadiansVec.m128_f32[2];

			__m128 CosineVec;
			const __m128 SineVec = _mm_sincos_ps(&CosineVec, RadiansVec);

			const float YawSine = SineVec.m128_f32[0];
			const float YawCosine = CosineVec.m128_f32[0];

			const float PitchSine = SineVec.m128_f32[1];
			const float PitchCosine = CosineVec.m128_f32[1];

			this->LookVector.X = YawCosine * PitchCosine;
			this->LookVector.Y = PitchSine;
			this->LookVector.Z = YawSine * PitchCosine;

			this->LookVector = this->LookVector.Unit();
			this->RightVector = this->LookVector.Cross(this->WorldUp).Unit();  // normalize the vectors, because their length gets closer to 0 the more you look up or down which results in slower movement.
			this->UpVector = this->RightVector.Cross(this->LookVector).Unit();

			const glm::mat3 roll_mat = (glm::mat3)RenderingFunctions::rotate(glm::mat4(1.f), RollRadians, this->LookVector);

			const glm::vec3 a = roll_mat * (*(glm::vec3*)(&this->UpVector));
			this->UpVector = *(LuauClasses::Vector3*)&a;
		}

		inline void Update() {
			using namespace SDLClasses;

			// Mouse Input

			const InputHandler::MouseInput MouseInput = InputHandler::PollMouse();
			InputHandler::CenterMouse();

			this->Yaw += (float)MouseInput.x * this->LookSensitivity * (float)*this->Delta;
			
			this->Yaw += float((Int8)InputHandler::IsKeyPressed(SDL_SCANCODE_RIGHT) - (Int8)InputHandler::IsKeyPressed(SDL_SCANCODE_LEFT)) * this->LookSensitivity * (float)*this->Delta;

			if (this->Yaw < -180.f) {
				this->Yaw = 180.f - (-180.f - this->Yaw);
			} else if (Yaw > 180.f) {
				this->Yaw = -180.f - (180.f - this->Yaw);
			}

			this->Pitch -= ((float)MouseInput.y * this->LookSensitivity) * (float)*this->Delta;
			this->Pitch += (float((Int8)InputHandler::IsKeyPressed(SDL_SCANCODE_UP) - (Int8)InputHandler::IsKeyPressed(SDL_SCANCODE_DOWN)) * this->LookSensitivity) * (float)*this->Delta;

			this->Pitch = SDL_clamp(this->Pitch, -89.9f, 89.9f);

			this->UpdateDirectionalVectors();

			// Keyboard Input
			
			const Int8 MoveX = (Int8)InputHandler::IsKeyPressed(SDL_SCANCODE_D)	 -	(Int8)InputHandler::IsKeyPressed(SDL_SCANCODE_A);
			const Int8 MoveZ = (Int8)InputHandler::IsKeyPressed(SDL_SCANCODE_W)	 -	(Int8)InputHandler::IsKeyPressed(SDL_SCANCODE_S);

			this->Velocity += this->RightVector * (float)MoveX * 0.1f;
			this->Velocity += this->LookVector	* (float)MoveZ * 0.1f;

			this->Velocity *= 0.5f;
			//this->Velocity.Z *= 0.5f;

			// Move

			this->Position += Velocity * (float)*this->Delta;

			// Other
			
			this->LeftArmRotation	+= (this->LeftArmGoalRotation	-	this->LeftArmRotation)	* 0.5f;
			this->RightArmRotation	+= (this->RightArmGoalRotation	-	this->RightArmRotation) * 0.5f;

			this->ViewArmRight->Update();

			InventoryItemType* HeldItem = this->GetHeldItem();
			
			if (HeldItem->GLObject != nullptr) {
				//HeldItem->GLObject->Update();
			}
		}

		inline glm::mat4 GetViewMatrix() {
			this->UpdateDirectionalVectors();
			return RenderingFunctions::lookAt(this->Position, this->Position + this->LookVector, this->UpVector);
		}

		inline void Draw() {

			glStencilMask(0xFF);
			glStencilFunc(GL_ALWAYS, 1, 0xFF);
			glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);

			//this->GetHeldItem()->DrawFirstPerson();

			glStencilOp(GL_KEEP, GL_KEEP, GL_KEEP);
			glStencilFunc(GL_EQUAL, 0, 0xFF);
		}

	private:

		LuauClasses::Vector3 WorldUp = LuauClasses::Vector3(0.f, 1.f, 0.f);

		const double* Delta;

		InventoryItemType* NoWeaponRef;
	};
}

#endif