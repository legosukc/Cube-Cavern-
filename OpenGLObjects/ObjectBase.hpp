#ifndef _OPENGLOBJECTS_OBJECTBASE
#define _OPENGLOBJECTS_OBJECTBASE

#include "./OtherClasses/GLClasses.hpp"

namespace OpenGLObjects {

	class ObjectBase {
	public:
		~ObjectBase() {
			std::cout << "called ~ObjectBase\n";
			this->Deconstructor();
		}

		virtual void Draw() {}
		virtual void Update() {}

		virtual void Deconstructor() {}

		GLClasses::VertexArray VAO;
		GLClasses::VertexBuffer VBO;
		GLClasses::ElementBuffer EBO;

		GLClasses::Program Program;
	};
}

#endif