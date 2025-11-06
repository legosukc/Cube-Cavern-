#ifndef _OBJECTMANAGER_HEADER
#define _OBJECTMANAGER_HEADER

#include <vector>
#include <map>

#include "./OpenGLObjects/ObjectBase.hpp"

namespace GameClasses {

	class ObjectManager {

		std::vector<OpenGLObjects::ObjectBase*> Objects;

	public:
		~ObjectManager() {

			for (auto& Object : this->Objects) {

				if (Object == nullptr)
					continue;

				delete Object;
				Object = nullptr;
			}
		}

		template <class ObjectClass = OpenGLObjects::ObjectBase>
		inline ObjectClass* AddObject(ObjectClass* Object) {
			this->Objects.push_back(static_cast<OpenGLObjects::ObjectBase*>(Object));
			return Object;
		}

		template <class ObjectClass>
		inline ObjectClass* AddObject() {
			return this->AddObject<ObjectClass>(new ObjectClass());
		}

		inline void* AddObject(void* Object) {
			return this->AddObject(Object);
		}


		template <class ObjectClass>
		bool RemoveObject(ObjectClass* RemovingObject) {

			for (auto& Object : this->Objects) {

				if (Object == RemovingObject) {
					delete Object;
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

				Object->Draw();
			}
		}

		void UpdateObjects() {

			for (auto& Object : this->Objects) {

				if (Object == nullptr)
					continue;

				Object->Update();
			}
		}
	};
}

#endif