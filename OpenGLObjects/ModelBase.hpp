#ifndef _OPENGLOBJECTS_MODELBASE
#define _OPENGLOBJECTS_MODELBASE

#include "ObjectBase.hpp"

#include <glad/glad.h>

#include <glm/glm.hpp>
#include <glm/fwd.hpp>

namespace OpenGLObjects {

	// not intended for use!
	class ModelBase : public ObjectBase {
	public:

		GLenum IndicesType;
		glm::mat4 ModelMatrix;

		virtual void Draw() override;

	protected:

		size_t IndicesLen;
		GLint ModelUniformID;   // mat4 Uniform

		void ModelBaseConstructor(const char* ModelFile);
	};
}

#endif