
#include "ModelBase.hpp"

#include <glad/glad.h>

#include "./LuauClasses/Vector3.hpp"
#include "./LuauClasses/Vector2.hpp"

#include <vector>

#include "./file.hpp"

#include "./OtherClasses/GLClasses.hpp"

namespace OpenGLObjects {

	void ModelBase::Draw() {
		this->VAO.Bind();
		this->Program.Use();

		this->Program.Mat4SetUniform(this->ModelUniformID, this->ModelMatrix);

		glDrawElements(GL_TRIANGLES, this->IndicesLen, this->IndicesType, 0);

		this->VAO.Unbind();
	}

	void ModelBase::ModelBaseConstructor(const char* ModelFile) {

		this->VAO.Bind();
		this->VBO.Bind();
		this->EBO.Bind();

		std::vector<LuauClasses::Vector3> Positions;
		std::vector<LuauClasses::Vector3> Normals;
		std::vector<LuauClasses::Vector2> Texcoords;

		size_t IndicesSize;
		void* Indices = file::loadOBJ(ModelFile, Positions, Normals, Texcoords, &this->IndicesType, &IndicesSize, &this->IndicesLen);


		this->VAO.StrideSize = 0;

		this->VBO.AllocateBytes((Positions.size() * sizeof(LuauClasses::Vector3)) + (Normals.size() * sizeof(LuauClasses::Vector3)) + (Texcoords.size() * sizeof(LuauClasses::Vector2)), GL_STATIC_DRAW);

		GLintptr VBOOffset = 0;
		if (Positions.size() > 0) {
			this->VBO.BindSubDataPointerWithSize(Positions.data(), Positions.size() * sizeof(LuauClasses::Vector3), VBOOffset);

			this->VAO.StrideSize += sizeof(float) * 3;
			VBOOffset += Positions.size() * sizeof(LuauClasses::Vector3);

			this->VAO.AppendSubAttribute<GL_FLOAT>(3, Positions.size() * sizeof(LuauClasses::Vector3));
		}

		if (Normals.size() > 0) {
			this->VBO.BindSubDataPointerWithSize(Normals.data(), Normals.size() * sizeof(LuauClasses::Vector3), VBOOffset);

			this->VAO.StrideSize += sizeof(float) * 3;
			VBOOffset += Normals.size() * sizeof(LuauClasses::Vector3);

			this->VAO.AppendSubAttribute<GL_FLOAT>(3, Normals.size() * sizeof(LuauClasses::Vector3));
		}

		if (Texcoords.size() > 0) {
			this->VBO.BindSubDataPointerWithSize(Texcoords.data(), Texcoords.size() * sizeof(LuauClasses::Vector2), VBOOffset);

			this->VAO.StrideSize += sizeof(float) * 2;

			this->VAO.AppendSubAttribute<GL_FLOAT>(2, Texcoords.size() * sizeof(LuauClasses::Vector2));
		}


		this->EBO.BindDataPointerWithSize(Indices, IndicesSize, GL_STATIC_DRAW);

		//this->VAO.AppendSubAttribute<GL_FLOAT>(3, Positions.size() * sizeof(LuauClasses::Vector3));	// Position
		//this->VAO.AppendSubAttribute<GL_FLOAT>(3, Normals.size() * sizeof(LuauClasses::Vector3));		// Normal
		//this->VAO.AppendSubAttribute<GL_FLOAT>(2, Texcoords.size() * sizeof(LuauClasses::Vector2));	// Texcoord

		this->VAO.Unbind();
	}
};