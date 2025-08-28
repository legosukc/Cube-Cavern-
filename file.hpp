#pragma once

#include <iostream>

#include <SDL2/SDL_image.h>

#include <SDL2/SDL_rwops.h>
#include <filesystem>

#include <Windows.h>
#include <libloaderapi.h>

#include <OBJ_Loader.h>

#include <stdlib.h>

#include <zlib-1.3.1/zconf.h>
#include <zlib-1.3.1/zlib.h>
#include <cstring>

#include "LuauClasses/Vector3.hpp"
#include "LuauClasses/Vector2.hpp"

#include "./OtherClasses/SmartPointer.hpp"

#include "./OpenGLObjects/ObjectBase.hpp"
#include "FunctionHeaders/AppendCstring.hpp"

namespace file {
	
	static SDL_FORCE_INLINE void Init() {

		// get the parent directory of the executable
		char ExecutablePath[MAX_PATH];

		// GetModuleFileNameA returns length of string
		size_t i = GetModuleFileNameA(NULL, ExecutablePath, sizeof(ExecutablePath));
		for (; ExecutablePath[i] != '\\'; --i);

		ExecutablePath[i] = '\0';
		std::filesystem::current_path(ExecutablePath);
	}

	static Bytef* CompressData(const void* data, uLong dataLen, uLongf* compressedLen, int Compressionlevel = Z_BEST_SPEED) {
		
		Bytef* compressedData = new Bytef[(dataLen * 2) + sizeof(uLong)];
		memcpy(compressedData, &dataLen, sizeof(uLong));
		
		switch (compress2(compressedData + sizeof(uLong), compressedLen, (const Bytef*)data, dataLen, Compressionlevel)) {
			case Z_MEM_ERROR:
				std::cerr << ":OUT OF MEMORY: Unable to compress data." << std::endl;
				delete[] compressedData;
				return nullptr;
			case Z_BUF_ERROR:
				std::cerr << ":OUT OF BUFFER: Output buffer not big enough for compression." << std::endl;
				delete[] compressedData;
				return nullptr;
			case Z_STREAM_ERROR:
				std::cerr << ":STREAM ERROR: Invalid compression level: " << Compressionlevel << '.' << std::endl;
				delete[] compressedData;
				return nullptr;
		}

		*compressedLen += sizeof(uLong);

		Bytef* ShrunkCompressedData = new Bytef[*compressedLen];
		memcpy(ShrunkCompressedData, compressedData, *compressedLen);
		delete[] compressedData;
		/*
		void* reallocPtr = realloc(compressedData, *compressedLen);

		if (reallocPtr == nullptr) {
			std::cerr << "OUT OF MEMORY: Couldn't, shrink? the compressedData buffer? what the FUCK? IM FREEING MEMORY?\n"
				<< "leaving disgusting oversized buffer as is :(" << std::endl;
		} else {
			compressedData = (Bytef*)reallocPtr;
		}*/

		return ShrunkCompressedData;
	}

	static Bytef* DecompressData(const void* CompressedData, uLong CompressedDataSize) {

		uLong UncompressedSize = reinterpret_cast<const uLong*>(CompressedData)[0];

		Bytef* UncompressedData = new Bytef[UncompressedSize];
		uncompress(UncompressedData, &UncompressedSize, reinterpret_cast<const Bytef*>(CompressedData) + sizeof(uLong), CompressedDataSize - sizeof(uLong));

		return UncompressedData;
	}

	void CompressToFile(const char* FileName, const void* Data, size_t DataSize, int Compressionlevel = Z_BEST_SPEED) {
		SDL_RWops* File = SDL_RWFromFile(FileName, "w");

		uLongf CompressedSaveSize;
		void* CompressedSave = CompressData(Data, DataSize, &CompressedSaveSize, Compressionlevel);

		SDL_RWwrite(File, CompressedSave, CompressedSaveSize, 1);
		SDL_RWclose(File);

		delete[] CompressedSave;
	}


	struct Vertex {
		LuauClasses::Vector3 Position;
		LuauClasses::Vector3 Normal;
		LuauClasses::Vector2 Texcoord;
	};
	
	// Generate vertices from a list of positions, 
		//	tcoords, normals and a face line
	static void GenVerticesFromRawOBJ(std::vector<Vertex>& oVerts,
		const std::vector<LuauClasses::Vector3>& iPositions,
		const std::vector<LuauClasses::Vector2>& iTCoords,
		const std::vector<LuauClasses::Vector3>& iNormals,
		std::string icurline) {
		std::vector<std::string> sface, svert;

		Vertex vVert;
		objl::algorithm::split(objl::algorithm::tail(icurline), sface, " ");

		bool noNormal = false;

		// For every given vertex do this
		for (size_t i = 0; i < sface.size(); i++) {

			objl::algorithm::split(sface[i], svert, "/");

			// Calculate and store the vertex
			vVert.Position = objl::algorithm::getElement(iPositions, svert[0]);

			switch (svert.size()) {
				case 1: // P
					memset(&vVert.Texcoord, 0, sizeof(objl::Vector2));
					//vVert.TextureCoordinate = objl::Vector2(0, 0);
					noNormal = true;
					break;
				case 2: // P/T
					vVert.Texcoord = objl::algorithm::getElement(iTCoords, svert[1]);
					noNormal = true;
					break;
				default: // P//N
					if (!svert[1].empty()) {
						vVert.Texcoord = objl::algorithm::getElement(iTCoords, svert[1]);
					} else {
						memset(&vVert.Texcoord, 0, sizeof(LuauClasses::Vector2));
						//vVert.TextureCoordinate = objl::Vector2(0, 0);
					}
					vVert.Normal = objl::algorithm::getElement(iNormals, svert[2]);
					break;
			}
			oVerts.push_back(vVert);
		}

		// take care of missing normals
		// these may not be truly acurate but it is the 
		// best they get for not compiling a mesh with normals	
		if (noNormal) {

			const LuauClasses::Vector3 Normal = (oVerts[0].Position - oVerts[1].Position).Cross(oVerts[2].Position - oVerts[1].Position);

			for (size_t i = 0; i < oVerts.size(); i++) {
				oVerts[i].Normal = Normal;
			}
		}
	}

	// Triangulate a list of vertices into a face by printing
	//	inducies corresponding with triangles within it
	static void VertexTriangluation(std::vector<unsigned int>& oIndices,
		const std::vector<Vertex>& iVerts) {
		// If there are 2 or less verts,
		// no triangle can be created,
		// so exit
		if (iVerts.size() < 3) {
			return;
		}

		if (iVerts.size() == 3) { // If it is a triangle no need to calculate it
			oIndices.push_back(0);
			oIndices.push_back(1);
			oIndices.push_back(2);
			return;
		}

		// Create a list of vertices
		std::vector<Vertex> tVerts = iVerts;

		while (true) {
			// For every vertex
			for (int i = 0; i < int(tVerts.size()); i++) {
				// pPrev = the previous vertex in the list
				Vertex pPrev;
				if (i == 0) {
					pPrev = tVerts[tVerts.size() - 1];
				} else {
					pPrev = tVerts[i - 1];
				}

				// pCur = the current vertex;
				Vertex pCur = tVerts[i];

				// pNext = the next vertex in the list
				Vertex pNext;
				if (i == tVerts.size() - 1) {
					pNext = tVerts[0];
				} else {
					pNext = tVerts[i + 1];
				}

				// Check to see if there are only 3 verts left
				// if so this is the last triangle
				if (tVerts.size() == 3) {
					// Create a triangle from pCur, pPrev, pNext
					for (size_t j = 0; j < tVerts.size(); j++) {
						if (iVerts[j].Position == pCur.Position or iVerts[j].Position == pPrev.Position or iVerts[j].Position == pNext.Position)
							oIndices.push_back(j);
					}

					tVerts.clear();
					break;
				} else if (tVerts.size() == 4) {
					// Create a triangle from pCur, pPrev, pNext
					for (size_t j = 0; j < iVerts.size(); ++j) {
						if (iVerts[j].Position == pCur.Position or iVerts[j].Position == pPrev.Position or iVerts[j].Position == pNext.Position)
							oIndices.push_back(j);
					}

					LuauClasses::Vector3 tempVec;
					for (size_t j = 0; j < tVerts.size(); ++j) {
						if (tVerts[j].Position != pCur.Position
							&& tVerts[j].Position != pPrev.Position
							&& tVerts[j].Position != pNext.Position) {
							tempVec = tVerts[j].Position;
							break;
						}
					}

					// Create a triangle from pCur, pPrev, pNext
					for (size_t j = 0; j < iVerts.size(); ++j) {
						if (iVerts[j].Position == pPrev.Position or iVerts[j].Position == pNext.Position or iVerts[j].Position == tempVec)
							oIndices.push_back(j);
					}

					tVerts.clear();
					break;
				}

				// If Vertex is not an interior vertex
				const float angle = (pPrev.Position - pCur.Position).AngleBetween(pNext.Position - pCur.Position) * (180.f / 3.14159265359f); //objl::math::AngleBetweenV3(pPrev.Position - pCur.Position, pNext.Position - pCur.Position) * (180.f / 3.14159265359f);
				if (angle <= 0 && angle >= 180)
					continue;

				// If any vertices are within this triangle
				bool inTri = false;
				for (size_t j = 0; j < iVerts.size(); j++) {
					if (iVerts[j].Position.InTriangle(pPrev.Position, pCur.Position, pNext.Position)
						and iVerts[j].Position != pPrev.Position
						and iVerts[j].Position != pCur.Position
						and iVerts[j].Position != pNext.Position) {
						inTri = true;
						break;
					}
				}

				if (inTri)
					continue;

				// Create a triangle from pCur, pPrev, pNext
				for (size_t j = 0; j < iVerts.size(); j++) {
					if (iVerts[j].Position == pCur.Position or iVerts[j].Position == pPrev.Position or iVerts[j].Position == pNext.Position)
						oIndices.push_back(j);
				}

				// Delete pCur from the list
				for (size_t j = 0; j < tVerts.size(); j++) {
					if (tVerts[j].Position == pCur.Position) {
						tVerts.erase(tVerts.begin() + j);
						break;
					}
				}

				// reset i to the start
				// -1 since loop will add 1 to it
				i = -1;
			}

			// if no triangles were created or no more vertices
			if (oIndices.empty() or tVerts.empty()) //(oIndices.size() == 0 or tVerts.size() == 0)
				break;
		}
	}

	static void loadOBJBase(const char* filePath, std::vector<Vertex>& LoadedVertices, std::vector<unsigned int>& LoadedIndices) {

		std::ifstream file;
		{
			const OtherClasses::SmartPointer ModelPath = AppendCstring::AppendCstring("models\\", filePath);
			file.open(ModelPath);

			if (!file.is_open()) {
				std::cout << ModelPath << " no load :(\n";
				return;
			}
		}

		std::vector<LuauClasses::Vector3> Positions;
		std::vector<LuauClasses::Vector3> Normals;
		std::vector<LuauClasses::Vector2> TCoords;

		std::vector<file::Vertex> Vertices;
		std::vector<unsigned int> Indices;

		bool listening = false;

		constexpr unsigned int outputEveryNth = 1000;
		unsigned int outputIndicator = outputEveryNth;

		std::string curline;

		while (std::getline(file, curline)) {
		#ifdef OBJL_CONSOLE_OUTPUT
			if ((outputIndicator = ((outputIndicator + 1) % outputEveryNth)) == 0) {
				std::cout
					<< '\r'
					<< "vertices > " << Positions.size()
					<< " | texcoords > " << TCoords.size()
					<< " | normals > " << Normals.size()
					<< " | triangles > " << (Vertices.size() / 3);
			}
		#endif

			const std::string curToken = objl::algorithm::firstToken(curline);

			std::vector<std::string> svec;
			switch (curToken.front()) {
				// Generate a Mesh Object or Prepare for an object to be created
				case 'o' or 'g' or (curline.front() == 'g') * curToken.front():
					if (!listening) {
						listening = true;
					} else {
						if (!Indices.empty() && !Vertices.empty()) {
							// Cleanup
							Vertices.clear();
							Indices.clear();
						}
					}
				#ifdef OBJL_CONSOLE_OUTPUT
					std::cout << std::endl;
					outputIndicator = 0;
				#endif
					break;
				case 'v':
					switch (curToken.back()) {
						case 't':
							svec.reserve(2);
							objl::algorithm::split(objl::algorithm::tail(curline), svec, " ");

							TCoords.push_back(
								LuauClasses::Vector2(
									std::stof(svec[0]),
									std::stof(svec[1])
								)
							);
							break;
						default:
							svec.reserve(3);
							objl::algorithm::split(objl::algorithm::tail(curline), svec, " ");

							const LuauClasses::Vector3 vvec(
								std::stof(svec[0]),
								std::stof(svec[1]),
								std::stof(svec[2])
							);

							if (curToken.back() == 'n') {
								Normals.push_back(vvec);
							} else {
								Positions.push_back(vvec);
							}
					}
					break;
				case 'f':
					// Generate the vertices
					std::vector<Vertex> vVerts;
					GenVerticesFromRawOBJ(vVerts, Positions, TCoords, Normals, curline);

					// Add Vertices
					Vertices.reserve(vVerts.size());
					LoadedVertices.reserve(vVerts.size());
					for (std::vector<Vertex>::iterator vertex = vVerts.begin(); vertex != vVerts.end(); ++vertex) {
						Vertices.push_back(*vertex);
						LoadedVertices.push_back(*vertex);
					}

					std::vector<unsigned int> iIndices;

					VertexTriangluation(iIndices, vVerts);

					// Add Indices
					Indices.reserve(iIndices.size());
					LoadedIndices.reserve(iIndices.size());
					for (std::vector<unsigned int>::iterator indice = iIndices.begin(); indice != iIndices.end(); ++indice) {
						Indices.push_back((Vertices.size() - vVerts.size()) + *indice);
						LoadedIndices.push_back((LoadedVertices.size() - vVerts.size()) + *indice);
					}
					break;
			}
		}

		file.close();

	#ifdef OBJL_CONSOLE_OUTPUT
		std::cout << '\n';
	#endif

		if (LoadedVertices.empty() && LoadedIndices.empty()) {
			std::cout << "fuck " << filePath << " no load :(\n";
		}
	}


	void loadOBJ(const char* filePath, std::vector<LuauClasses::Vector3>& Positions, std::vector<LuauClasses::Vector3>& Normals, std::vector<LuauClasses::Vector2>& Texcoords, std::vector<unsigned int>& Indices) {

		std::vector<Vertex> Vertices;
		file::loadOBJBase(filePath, Vertices, Indices);

		for (size_t i = 0; i < Vertices.size(); ++i) {

			Positions.push_back(Vertices[i].Position);
			Normals.push_back(Vertices[i].Normal);
			Texcoords.push_back(Vertices[i].Texcoord);
		}
	}

	template <class ObjectType = OpenGLObjects::ModelBase<>>
	void loadOBJIntoOpenGLObject(const char* OBJName, ObjectType* Object) {

		Object->VAO.Bind();
		Object->VBO.Bind();
		Object->EBO.Bind();

		std::vector<LuauClasses::Vector3> Positions;
		std::vector<LuauClasses::Vector3> Normals;
		std::vector<LuauClasses::Vector2> Texcoords;

		std::vector<unsigned int> Indices;
		file::loadOBJ(OBJName, Positions, Normals, Texcoords, Indices);

		Object->VAO.StrideSize = sizeof(float) * 8;

		Object->IndicesLen = Indices.size();

		Object->VBO.AllocateBytes((Positions.size() * sizeof(LuauClasses::Vector3)) + (Normals.size() * sizeof(LuauClasses::Vector3)) + (Texcoords.size() * sizeof(LuauClasses::Vector2)), GL_STATIC_DRAW);

		Object->VBO.BindSubDataPointerWithSize(Positions.data(),	Positions.size() * sizeof(LuauClasses::Vector3),	0);
		Object->VBO.BindSubDataPointerWithSize(Normals.data(),		Normals.size() * sizeof(LuauClasses::Vector3),		Positions.size() * sizeof(LuauClasses::Vector3));
		Object->VBO.BindSubDataPointerWithSize(Texcoords.data(),	Texcoords.size() * sizeof(LuauClasses::Vector2),	(Positions.size() * sizeof(LuauClasses::Vector3)) + (Normals.size() * sizeof(LuauClasses::Vector3)));

		Object->EBO.BindDataPointerWithSize(Indices.data(), Object->IndicesLen * sizeof(unsigned int), GL_STATIC_DRAW);

		Object->VAO.AppendSubAttribute<GL_FLOAT>(3, Positions.size() * sizeof(LuauClasses::Vector3));	// Position
		Object->VAO.AppendSubAttribute<GL_FLOAT>(3, Normals.size() * sizeof(LuauClasses::Vector3));		// Normal
		Object->VAO.AppendSubAttribute<GL_FLOAT>(2, Texcoords.size() * sizeof(LuauClasses::Vector2));	// Texcoord

		Object->VAO.Unbind();
	}
}