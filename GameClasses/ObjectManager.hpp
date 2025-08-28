#ifndef _OBJECTMANAGER_HEADER
#define _OBJECTMANAGER_HEADER

#include <vector>

#include "OpenGLObjects/ObjectBase.hpp"

namespace GameClasses {

	class ObjectManager {

		std::vector<OpenGLObjects::ObjectBase<void>*> Objects;

	public:
		~ObjectManager() {

			for (auto& Object : this->Objects) {

				if (Object == nullptr)
					continue;

				if (Object->Deconstructor == nullptr)
					continue;

				Object->Deconstructor(Object);
				Object = nullptr;
			}
		}

		template <class ObjectClass = OpenGLObjects::ObjectBase<void>>
		inline ObjectClass* AddObject(ObjectClass* Object) {
			this->Objects.push_back(reinterpret_cast<OpenGLObjects::ObjectBase<void>*>(Object));
			return Object;
		}

		template <class ObjectClass>
		inline ObjectClass* AddObject() {
			return this->AddObject(new ObjectClass());
		}

		inline void* AddObject(void* Object) {
			return this->AddObject(Object);
		}


		template <class ObjectClass>
		bool RemoveObject(ObjectClass* RemovingObject) {

			for (auto& Object : this->Objects) {

				if (Object == RemovingObject) {
					Object = nullptr;
					return true;
				}
			}

			return false;
		}

		void DrawObjects() {

			for (auto& Object : this->Objects) {

				if (Object == nullptr)
					continue;

				if (Object->Draw == nullptr)
					continue;

				Object->Draw(Object);
			}
		}

		void UpdateObjects() {

			for (auto& Object : this->Objects) {

				if (Object == nullptr)
					continue;

				if (Object->Update == nullptr)
					continue;

				Object->Update(Object);
			}
		}
	};
}

#endif