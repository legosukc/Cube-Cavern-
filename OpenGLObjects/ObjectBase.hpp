#ifndef _OPENGLOBJECTS_OBJECTBASE
#define _OPENGLOBJECTS_OBJECTBASE

#include "./OtherClasses/GLClasses.hpp"

namespace OpenGLObjects {

	class ObjectBase {
	public:
		virtual ~ObjectBase();

		inline virtual void Draw();
		inline virtual void Update();

		GLClasses::VertexArray VAO;
		GLClasses::VertexBuffer VBO;
		GLClasses::ElementBuffer EBO;

		GLClasses::Program Program;
	};
}

#endif