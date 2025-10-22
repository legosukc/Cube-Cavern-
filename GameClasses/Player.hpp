#ifndef _PLAYER_HEADER
#define _PLAYER_HEADER

#include <SDL2/SDL_stdinc.h>

#include "./LuauClasses/Vector3.hpp"

//#include "./ItemClasses/ItemBase.hpp"

//#include "./OpenGLObjects/ViewArmRight.hpp"
//#include "./OpenGLObjects/ModelBase.hpp"

#include "./OtherClasses/ExpandableArray.hpp"
#include <glm/fwd.hpp>

#include "../LuaClasses/LuaContext.hpp"
#include "../LuaClasses/LuaTable.hpp"

namespace GameClasses {

	class Player {

		static void MouseButtonDown(void* Connector, SDL_MouseButtonEvent* MouseButtonEvent);

		static void InputDown(void* Connector, SDL_Scancode Scancode);

	public:

		//OpenGLObjects::ViewArmRight* ViewArmRight;

		Player(const double* DeltaTimePtr);

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

		OtherClasses::ExpandableArray<LuaTableObject, Uint8> Items;

		//template<typename ItemClass = InventoryItemType>
		bool OverwriteInventoryItem(Uint8 Slot, LuaTableObject Item);

		inline bool OverwriteInventoryItem(Uint8 Slot, LuaClasses::LuaTable& Item);
		inline bool OverwriteInventoryItem(Uint8 Slot, const char* ItemName);

		//template<class ItemClass = InventoryItemType>
		inline void AddItemToInventory(LuaTableObject Item);

		inline void AddItemToInventory(LuaClasses::LuaTable& Item);
		inline void AddItemToInventory(const char* ItemName);

		LuaTableObject GetHeldItem();


		void UpdateDirectionalVectors();

		inline void Update();

		inline glm::mat4 GetViewMatrix();

		inline void Draw();

	private:

		LuauClasses::Vector3 WorldUp = LuauClasses::Vector3(0.f, 1.f, 0.f);

		const double* Delta;

		LuaClasses::LuaContext* AttachedContext;
		
		LuaObject LuaClassObject;
	};


	void Player::MouseButtonDown(void* Connector, SDL_MouseButtonEvent* MouseButtonEvent) {
		using namespace SDLClasses;

		Player* self = (GameClasses::Player*)Connector;

		switch (MouseButtonEvent->button) {

			case InputHandler::MouseButtonEnums::LeftMouse:

				if (self->OnItemCooldown) {
					break;
				}

				LuaTableObject HeldItem = self->GetHeldItem();

				self->OnItemCooldown = true;
				//HeldItem->LMB();

				self->AttachedContext->PushValueOntoStack(HeldItem);
				self->AttachedContext->PushStringLiteral("LMB");
				lua_gettable(self->AttachedContext->ContextObject, -2);

				lua_callk(self->AttachedContext->ContextObject, 0, -1, 0, 0);

				const auto ResetItemCooldown = [](Player* self) {
					self->OnItemCooldown = false;
					std::cout << "cooldown over\n";
					};

				//lua_getfield(self->AttachedContext->ContextObject, -1, "Cooldown");
				self->AttachedContext->PushStringLiteral("Cooldown");
				LuauClasses::task::CreateDelayedTask<Player*>(ResetItemCooldown,
					self,
					(float)self->AttachedContext->GetNumber(lua_gettable(self->AttachedContext->ContextObject, -2))
				);
				break;

			case InputHandler::MouseButtonEnums::RightMouse:

				if (self->OnItemCooldown) {
					break;
				}

				LuaTableObject HeldItem = self->GetHeldItem();

				//self->OnItemCooldown = true;
				//HeldItem->LMB();

				self->AttachedContext->PushValueOntoStack(HeldItem);
				self->AttachedContext->PushStringLiteral("RMB");
				lua_gettable(self->AttachedContext->ContextObject, -2);

				lua_callk(self->AttachedContext->ContextObject, 0, -1, 0, 0);
				/*
				const auto ResetItemCooldown = [](Player* self) {
					self->OnItemCooldown = false;
					std::cout << "cooldown over\n";
					};

				//lua_getfield(self->AttachedContext->ContextObject, -1, "Cooldown");
				self->AttachedContext->PushStringLiteral()
				LuauClasses::task::CreateDelayedTask<Player*>(ResetItemCooldown,
					self,
					(float)self->AttachedContext->GetNumber(lua_getfield(self->AttachedContext->ContextObject, -1, "Cooldown"))
				);*/

				break;
		}
	}

	void Player::InputDown(void* Connector, SDL_Scancode Scancode) {

		Player* self = (Player*)Connector;

		if (Scancode - SDL_SCANCODE_1 < self->InventorySlots && !self->OnItemCooldown) {

			LuaTableObject OldEquipedItem = self->GetHeldItem();
			self->HeldItemSlot = Uint8(Scancode - SDL_SCANCODE_1);

			self->AttachedContext->PushValueOntoStack(self->GetHeldItem());
			self->AttachedContext->PushStringLiteral("OnEquip");
			lua_gettable(self->AttachedContext->ContextObject, -2);

			lua_callk(self->AttachedContext->ContextObject, 0, -1, 0, 0);


			self->AttachedContext->PushValueOntoStack(OldEquipedItem);
			self->AttachedContext->PushStringLiteral("OnUnequip");
			lua_gettable(self->AttachedContext->ContextObject, -2);

			lua_callk(self->AttachedContext->ContextObject, 0, -1, 0, 0);

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
	}


	bool Player::OverwriteInventoryItem(Uint8 Slot, LuaTableObject Item) {

		if (Slot < this->InventorySlots) {

			if (this->Items[Slot] != INT_MIN) {
				Game::Items::RemoveItem(this->Items[Slot]);
				//this->Items[Slot] = INT_MIN;
			}

			this->AttachedContext->PushValueOntoStack(Item);
			this->AttachedContext->PushStringLiteral("Owner");
			this->AttachedContext->PushValueOntoStack(this->LuaClassObject);
			lua_settable(this->AttachedContext->ContextObject, -3);

			this->Items[Slot] = Item;
			return true;
		}

		return false;
	}

	bool Player::OverwriteInventoryItem(Uint8 Slot, LuaClasses::LuaTable& Item) {
		return this->OverwriteInventoryItem(Slot, Item.TableObject);
	}

	bool Player::OverwriteInventoryItem(Uint8 Slot, const char* ItemName) {
		return this->OverwriteInventoryItem(Slot, Game::Items::CreateItem(ItemName).TableObject);
	}


	void Player::AddItemToInventory(LuaTableObject Item) {

		for (Uint8 i = 0; i < this->InventorySlots; ++i) {

			if (this->Items[i] == INT_MIN) {

				this->AttachedContext->PushValueOntoStack(Item);
				this->AttachedContext->PushStringLiteral("Owner");
				this->AttachedContext->PushValueOntoStack(this->LuaClassObject);
				lua_settable(this->AttachedContext->ContextObject, -3);
				//Item->Owner = this;
				/*
				if (Item->GLObject != nullptr) {
					Item->GLObject->Position = &Item->Position;
				}*/

				this->Items[i] = Item;
				break;
			}
		}
	}

	void Player::AddItemToInventory(LuaClasses::LuaTable& Item) {
		this->AddItemToInventory(Item.TableObject);
	}

	void Player::AddItemToInventory(const char* ItemName) {
		this->AddItemToInventory(Game::Items::CreateItem(ItemName).TableObject);
	}


	LuaTableObject Player::GetHeldItem() {

		if (const Uint8 LastSize = this->Items.ReservedSize(); this->HeldItemSlot > LastSize) {
			this->Items.SetSizeTo(this->HeldItemSlot);
			this->Items.Fill(LastSize, this->HeldItemSlot, INT_MIN);
		}

		return this->Items[this->HeldItemSlot];
	}



	void Player::UpdateDirectionalVectors() {

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


	void Player::Update() {
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

		const Int8 MoveX = (Int8)InputHandler::IsKeyPressed(SDL_SCANCODE_D) - (Int8)InputHandler::IsKeyPressed(SDL_SCANCODE_A);
		const Int8 MoveZ = (Int8)InputHandler::IsKeyPressed(SDL_SCANCODE_W) - (Int8)InputHandler::IsKeyPressed(SDL_SCANCODE_S);

		this->Velocity += this->RightVector * (float)MoveX * 0.1f;
		this->Velocity += this->LookVector * (float)MoveZ * 0.1f;

		this->Velocity *= 0.5f;
		//this->Velocity.Z *= 0.5f;

		// Move

		this->Position += Velocity * (float)*this->Delta;

		// Other

		this->LeftArmRotation += (this->LeftArmGoalRotation - this->LeftArmRotation) * 0.5f;
		this->RightArmRotation += (this->RightArmGoalRotation - this->RightArmRotation) * 0.5f;

		//this->ViewArmRight->Update();

		//LuaTableObject HeldItem = this->GetHeldItem();

		//if (HeldItem->GLObject != nullptr) {
			//HeldItem->GLObject->Update();
		//}
	}

	glm::mat4 Player::GetViewMatrix() {
		this->UpdateDirectionalVectors();
		return RenderingFunctions::lookAt(this->Position, this->Position + this->LookVector, this->UpVector);
	}

	void Player::Draw() {

		glStencilMask(0xFF);
		glStencilFunc(GL_ALWAYS, 1, 0xFF);
		glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);

		//this->GetHeldItem()->DrawFirstPerson();

		glStencilOp(GL_KEEP, GL_KEEP, GL_KEEP);
		glStencilFunc(GL_EQUAL, 0, 0xFF);
	}


	Player::Player(const double* DeltaTimePtr) {
		using namespace SDLClasses;

		//this->Items.resize(this->InventorySlots);
		this->Items.SetSizeTo(this->InventorySlots); //reserve(this->InventorySlots);
		this->Items.Fill(INT_MIN);
		//NoWeaponRef->Owner = this;

		//this->ViewArmRight = new OpenGLObjects::ViewArmRight(&this->RightArmRotation);

		this->Delta = DeltaTimePtr;

		this->Position = LuauClasses::Vector3(0.f, 0.f, -2.f);
		this->Velocity = LuauClasses::Vector3(0.f);

		this->LookVector = LuauClasses::Vector3(0.f, 0.f, -1.f);
		this->RightVector = LuauClasses::Vector3(0.f);
		this->UpVector = LuauClasses::Vector3(0.f, 1.f, 0.f);

		this->UpdateDirectionalVectors();
		/*
		const auto InputDown = [](void* Connector, SDL_Scancode Scancode) {

			Player* self = (Player*)Connector;

			if (Uint8(Scancode - SDL_SCANCODE_1) < self->InventorySlots && !self->OnItemCooldown) {

				LuaTableObject OldEquipedItem = self->GetHeldItem();
				self->HeldItemSlot = Uint8(Scancode - SDL_SCANCODE_1);

				self->AttachedContext->PushValueOntoStack(self->GetHeldItem());
				self->AttachedContext->PushStringLiteral("OnEquip");
				lua_gettable(self->AttachedContext->ContextObject, -2);

				lua_callk(self->AttachedContext->ContextObject, 0, -1, 0, 0);


				self->AttachedContext->PushValueOntoStack(OldEquipedItem);
				self->AttachedContext->PushStringLiteral("OnUnequip");
				lua_gettable(self->AttachedContext->ContextObject, -2);

				lua_callk(self->AttachedContext->ContextObject, 0, -1, 0, 0);

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
			};*/

		InputHandler::AddInputDownConnection(InputDown, this);

		InputHandler::AddMouseButtonDownConnection(MouseButtonDown, this);
	}

	int PlayerLuaIndexFunction(lua_State* L) {

		LuaClasses::LuaContext* Context = (LuaClasses::LuaContext*)L;

		if (!Context->IsString(-1)) {

			luaL_error(Context->ContextObject, "::LUA CFUNCTION ERROR:: Can't index C++ class 'Player' with type: %s", lua_typename(Context->ContextObject, -1));
			return -1;
		}

		const char* IndexString = Context->GetString(-2);
		if (strcmp(IndexString, "Position") == -1) {

			//LuauClasses::newVector3Userdata()
		}

		return 1;
	}
};

#endif