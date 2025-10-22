#ifndef _TEXTUREMANAGER_HEADER
#define _TEXTUREMANAGER_HEADER

#include <glad/glad.h>
#include "./OtherClasses/GLClasses.hpp"

#include <map>

static std::map<const char*, GLClasses::Texture*> Textures;

namespace GameClasses::TextureManager {

	GLClasses::Texture* RegisterTexture(const char* RegisterName, GLClasses::Texture* Texture) {
		return Textures.insert({ RegisterName, Texture }).first->second;
	}

	GLClasses::Texture* LoadTexture(const char* TextureName, const char* RegisterName, GLenum WrapX, GLenum WrapY, bool Mipmapping = false, GLenum NearFilterType = GL_NEAREST, GLenum FarFilterType = GL_NEAREST_MIPMAP_LINEAR) {
		return RegisterTexture(RegisterName, new GLClasses::Texture(TextureName, WrapX, WrapY, Mipmapping, NearFilterType, FarFilterType));
	}
	GLClasses::Texture* LoadTexture(const char* TextureName, GLenum WrapX, GLenum WrapY, bool Mipmapping = false, GLenum NearFilterType = GL_NEAREST, GLenum FarFilterType = GL_NEAREST_MIPMAP_LINEAR) {
		return LoadTexture(TextureName, TextureName, WrapX, WrapY, Mipmapping, NearFilterType, FarFilterType);
	}

	GLClasses::Texture* GetTexture(const char* TextureName) {

		const auto Texture = Textures.find(TextureName);

		// Check if found
		if (Texture != Textures.end()) {
			return Textures[TextureName];
		}

		return nullptr;
	}

	bool RemoveTexture(const GLClasses::Texture* Texture) {

		for (auto& Element : Textures) {

			if (Element.second->TextureObject == Texture->TextureObject) {
				RemoveTexture(Element.second);
				return true;
			}
		}

		return false;
	}

	void RemoveTexture(const char* TextureName) {
		delete Textures[TextureName];
		Textures.erase(TextureName);
	}
}

#endif