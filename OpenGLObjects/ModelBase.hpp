#ifndef _OPENGLOBJECTS_MODELBASE
#define _OPENGLOBJECTS_MODELBASE

#include <glad/glad.h>

#include "./LuauClasses/Vector3.hpp"

#include <vector>

#include "./file.hpp"

#include "ObjectBase.hpp"
#include "./OtherClasses/GLClasses.hpp"

namespace OpenGLObjects {

	class ModelBase : public ObjectBase {
	public:

        virtual void Draw() override {
            this->VAO.Bind();
            this->Program.Use();

            glDrawElements(GL_TRIANGLES, this->IndicesLen, GL_UNSIGNED_INT, 0);

            this->VAO.Unbind();
        }

	protected:

		size_t IndicesLen;

		void ModelBaseConstructor(const char* ModelFile) {
			this->VAO.Bind();
			this->VBO.Bind();
			this->EBO.Bind();

			std::vector<LuauClasses::Vector3> Positions;
			std::vector<LuauClasses::Vector3> Normals;
			std::vector<LuauClasses::Vector2> Texcoords;

			std::vector<unsigned int> Indices;
			file::loadOBJ(ModelFile, Positions, Normals, Texcoords, Indices);

			this->VAO.StrideSize = sizeof(float) * 8;

			this->IndicesLen = Indices.size();

			this->VBO.AllocateBytes((Positions.size() * sizeof(LuauClasses::Vector3)) + (Normals.size() * sizeof(LuauClasses::Vector3)) + (Texcoords.size() * sizeof(LuauClasses::Vector2)), GL_STATIC_DRAW);

			this->VBO.BindSubDataPointerWithSize(Positions.data(), Positions.size() * sizeof(LuauClasses::Vector3), 0);
			this->VBO.BindSubDataPointerWithSize(Normals.data(), Normals.size() * sizeof(LuauClasses::Vector3), Positions.size() * sizeof(LuauClasses::Vector3));
			this->VBO.BindSubDataPointerWithSize(Texcoords.data(), Texcoords.size() * sizeof(LuauClasses::Vector2), (Positions.size() * sizeof(LuauClasses::Vector3)) + (Normals.size() * sizeof(LuauClasses::Vector3)));

			this->EBO.BindDataPointerWithSize(Indices.data(), this->IndicesLen * sizeof(unsigned int), GL_STATIC_DRAW);

			this->VAO.AppendSubAttribute<GL_FLOAT>(3, Positions.size() * sizeof(LuauClasses::Vector3));	// Position
			this->VAO.AppendSubAttribute<GL_FLOAT>(3, Normals.size() * sizeof(LuauClasses::Vector3));		// Normal
			this->VAO.AppendSubAttribute<GL_FLOAT>(2, Texcoords.size() * sizeof(LuauClasses::Vector2));	// Texcoord

			this->VAO.Unbind();
		}
	};
}

#endif