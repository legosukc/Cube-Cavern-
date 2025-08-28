#ifndef _OPENGLOBJECTS_OBJECTBASE
#define _OPENGLOBJECTS_OBJECTBASE

#include "OtherClasses/GLClasses.hpp"

namespace OpenGLObjects {

	template<class SelfType = void>
	class ObjectBase {

		static void _Draw(SelfType* self);
		static void _Update(SelfType* self);

		static void _Deconstructor(SelfType* self);

	public:

		inline ~ObjectBase() {

			if (this->Deconstructor == nullptr) {
				return;
			}

			this->Deconstructor((SelfType*)this);
		}

		// Function pointer to draw the object.
		void(*Draw)(SelfType* self) = nullptr;
		void(*Update)(SelfType* self) = nullptr;

		void(*Deconstructor)(SelfType* self) = nullptr;

		GLClasses::VertexArray VAO;
		GLClasses::VertexBuffer VBO;
		GLClasses::ElementBuffer EBO;

		GLClasses::Program Program;
	};

	template<class SelfType = void>
	class ModelBase : public ObjectBase<SelfType> {
	public:

		size_t IndicesLen;
	};
}

#endif