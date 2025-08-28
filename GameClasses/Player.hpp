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

#include <vector>

namespace GameClasses {

	class Player {

		LuauClasses::Vector3 WorldUp = LuauClasses::Vector3(0.f, 1.f, 0.f);

		const double* Delta;

	public:

		Player(const double* DeltaTimePtr) {
			using namespace SDLClasses;

			this->Items.resize(this->InventorySlots);

			this->Delta = DeltaTimePtr;

			this->Position = LuauClasses::Vector3(0.f, 0.f, -2.f);
			this->Velocity = LuauClasses::Vector3(0.f);

			this->LookVector = LuauClasses::Vector3(0.f, 0.f, -1.f);
			this->RightVector = LuauClasses::Vector3(0.f);
			this->UpVector = LuauClasses::Vector3(0.f, 1.f, 0.f);

			this->UpdateDirectionalVectors();

			const auto InputDown = [](void* Connector, SDL_Scancode Scancode) {

				Player* self = reinterpret_cast<Player*>(Connector);

				switch (Scancode) {
					
				}
				};

			InputHandler::AddInputDownConnection(InputDown, this);

			const auto MouseButtonDown = [](void* Connector, SDL_MouseButtonEvent* MouseButtonEvent) {
				using namespace SDLClasses;

				Player* self = reinterpret_cast<Player*>(Connector);

				switch (MouseButtonEvent->button) {
					case InputHandler::MouseButtonEnums::LeftMouse:

						self->Items[self->HeldItemSlot]->LMB(self->Items[self->HeldItemSlot], self);
						self->ArmYGoal = 70.f;

						const auto ResetArmY = [](Player* self) {
							self->ArmYGoal = 0.f;
							};

						LuauClasses::task::CreateDelayedTask<Player*>(ResetArmY, self, 0.38f);
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
		float ArmY = 0.f;
		float ArmYGoal = 0.f;


		size_t InventorySlots = 3;

		size_t HeldItemSlot = 0;
		std::vector<ItemClasses::ItemBase<>*> Items;

		void UpdateDirectionalVectors() {
			
			float YawSine, YawCosine, PitchSine, PitchCosine;
			{
				__m128 CosineVec;
				const __m128 SineVec = _mm_sincos_ps(&CosineVec, _mm_setr_ps(Maths::ToRadians(this->Yaw), Maths::ToRadians(this->Pitch), 0.f, 0.f));

				YawSine =	SineVec.m128_f32[0];
				YawCosine = CosineVec.m128_f32[0];

				PitchSine =		SineVec.m128_f32[1];
				PitchCosine = CosineVec.m128_f32[1];
			}

			this->LookVector.X = YawCosine * PitchCosine;
			this->LookVector.Y = PitchSine;
			this->LookVector.Z = YawSine * PitchCosine;

			//this->LookVector.X = glm::cos(glm::radians(this->Yaw)) * glm::cos(glm::radians(this->Pitch));
			//this->LookVector.Y = glm::sin(glm::radians(this->Pitch));
			//this->LookVector.Z = glm::sin(glm::radians(this->Yaw)) * glm::cos(glm::radians(this->Pitch));

			this->LookVector = this->LookVector.Unit();
			this->RightVector = this->LookVector.Cross(this->WorldUp).Unit();  // normalize the vectors, because their length gets closer to 0 the more you look up or down which results in slower movement.
			this->UpVector = this->RightVector.Cross(this->LookVector).Unit();

			const glm::mat3 roll_mat = (glm::mat3)RenderingFunctions::rotate(glm::mat4(1.f), Maths::ToRadians(this->Roll), this->LookVector);

			const glm::vec3 a = roll_mat * (*(glm::vec3*)(&this->UpVector));
			this->UpVector = *(LuauClasses::Vector3*)&a;
		}

		inline void Update() {
			using namespace SDLClasses;

			// Mouse Input

			const InputHandler::MouseInput MouseInput = InputHandler::PollMouse();
			InputHandler::CenterMouse();

			this->Yaw += (static_cast<float>(MouseInput.x) * this->LookSensitivity) * *this->Delta;
			
			this->Yaw += (static_cast<float>(static_cast<Int8>(InputHandler::IsKeyPressed(SDL_SCANCODE_RIGHT)) - static_cast<Int8>(InputHandler::IsKeyPressed(SDL_SCANCODE_LEFT))) * this->LookSensitivity) * *this->Delta;

			if (this->Yaw < -180.f) {
				this->Yaw = 180.f - (-180.f - this->Yaw);
			} else if (Yaw > 180.f) {
				this->Yaw = -180.f - (180.f - this->Yaw);
			}

			this->Pitch -= (static_cast<float>(MouseInput.y) * this->LookSensitivity) * *this->Delta;
			this->Pitch += (static_cast<float>(static_cast<Int8>(InputHandler::IsKeyPressed(SDL_SCANCODE_UP)) - static_cast<Int8>(InputHandler::IsKeyPressed(SDL_SCANCODE_DOWN))) * this->LookSensitivity) * *this->Delta;

			this->Pitch = SDL_clamp(this->Pitch, -89.9f, 89.9f);

			this->UpdateDirectionalVectors();

			// Keyboard Input
			
			const Int8 MoveX = static_cast<Int8>(InputHandler::IsKeyPressed(SDL_SCANCODE_D))	-	static_cast<Int8>(InputHandler::IsKeyPressed(SDL_SCANCODE_A));
			const Int8 MoveZ = static_cast<Int8>(InputHandler::IsKeyPressed(SDL_SCANCODE_W))	-	static_cast<Int8>(InputHandler::IsKeyPressed(SDL_SCANCODE_S));

			this->Velocity += this->RightVector * static_cast<float>(MoveX) * 0.1f;
			this->Velocity += this->LookVector * static_cast<float>(MoveZ) * 0.1f;

			this->Velocity *= 0.5f;
			//this->Velocity.Z *= 0.5f;

			// Move

			this->Position += Velocity * *this->Delta;


			// Other
			
			this->ArmY = std::lerp(this->ArmY, this->ArmYGoal, 0.5f);
		}

		inline glm::mat4 GetViewMatrix() {
			this->UpdateDirectionalVectors();
			return RenderingFunctions::lookAt(this->Position, this->Position + this->LookVector, this->UpVector);
		}
	};
}

#endif