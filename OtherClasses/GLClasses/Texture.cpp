
#include "./OtherClasses/GLClasses.hpp"

#include <glad/glad.h>

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include "./FunctionHeaders/AppendCstring.hpp"

#include <iostream>

namespace GLClasses {

    Texture::Texture() {}

    // Load image as texture
    Texture::Texture(const char* textureName, GLenum WrapX, GLenum WrapY, bool Mipmapping, GLenum NearFilterType, GLenum FarFilterType) {

        this->InternalFormat = GL_RGB;
        this->Format = GL_RGB;
        this->FormatType = GL_UNSIGNED_BYTE;

        SDL_Surface* LoadedSurface;
        {
            OtherClasses::SmartPointer TexturePath = AppendCstring::AppendCstring("textures\\", textureName);
            LoadedSurface = IMG_Load(TexturePath);
        }

        if (LoadedSurface == NULL) {
            std::cout << "Failed to load texture: " << textureName << '\n';

            LoadedSurface = IMG_Load("textures\\missing.bmp");
            this->Constructor(GL_REPEAT, GL_REPEAT, GL_NEAREST, GL_NEAREST);
        } else {
            this->Constructor(WrapX, WrapY, NearFilterType, FarFilterType);
        }

        {
            SDL_Surface* ConvertedSurface = SDL_ConvertSurfaceFormat(LoadedSurface, SDL_PIXELFORMAT_RGB24, 0);
            SDL_FreeSurface(LoadedSurface);
            LoadedSurface = ConvertedSurface;
        }

        this->Width = LoadedSurface->w;
        this->Height = LoadedSurface->h;

        //this->CreateTextureUsingProperties(LoadedSurface->pixels);
        glTexImage2D(GL_TEXTURE_2D, 0, this->InternalFormat, this->Width, this->Height, 0, this->Format, this->FormatType, LoadedSurface->pixels);

        if (Mipmapping) {
            glBindTexture(GL_TEXTURE_2D, this->TextureObject);
            glGenerateMipmap(GL_TEXTURE_2D);
        }

        SDL_FreeSurface(LoadedSurface);

        glBindTexture(GL_TEXTURE_2D, 0);
    }
    /*
    // Load image as texture
    Texture(const char* textureName, GLenum InternalFormat, GLenum Format, GLenum FormatType, GLenum WrapX, GLenum WrapY, bool Mipmapping = true, GLenum NearFilterType = GL_NEAREST, GLenum FarFilterType = GL_NEAREST_MIPMAP_LINEAR) {

        this->InternalFormat = InternalFormat;
        this->Format = Format;
        this->FormatType = FormatType;

        this->Constructor(WrapX, WrapY, NearFilterType, FarFilterType);

        SDL_Surface* LoadedSurface;
        {
            OtherClasses::SmartPointer TexturePath = AppendCstring::AppendCstring("textures\\", textureName);
            LoadedSurface = IMG_Load(TexturePath);
        }

        if (LoadedSurface == NULL) {
            std::cout << "Failed to load texture: " << textureName << '\n';

            LoadedSurface = IMG_Load("textures\\missing.bmp");
            this->Constructor(GL_REPEAT, GL_REPEAT, GL_NEAREST, GL_NEAREST);
        }

        {
            SDL_Surface* ConvertedSurface = SDL_ConvertSurfaceFormat(LoadedSurface, SDL_PIXELFORMAT_RGB24, 0);
            SDL_FreeSurface(LoadedSurface);
            LoadedSurface = ConvertedSurface;
        }

        this->Width = LoadedSurface->w;
        this->Height = LoadedSurface->h;

        this->CreateTextureUsingProperties(LoadedSurface->pixels);

        if (Mipmapping) {
            glGenerateMipmap(TextureTarget);
        }

        SDL_FreeSurface(LoadedSurface);

        glBindTexture(TextureTarget, 0);
    }*/

    // Create Texture
    Texture::Texture(GLint Width, GLint Height, GLenum InternalFormat, GLenum Format, GLenum FormatType, GLenum WrapX, GLenum WrapY, GLenum NearFilterType, GLenum FarFilterType) {

        this->Width = Width;
        this->Height = Height;

        this->InternalFormat = InternalFormat;
        this->Format = Format;
        this->FormatType = FormatType;

        this->Constructor(WrapX, WrapY, NearFilterType, FarFilterType);

        this->CreateTextureUsingProperties(NULL);

        glBindTexture(GL_TEXTURE_2D, 0);
    }

    Texture::~Texture() {
        glDeleteTextures(1, &this->TextureObject);
    }

    void Texture::CreateTextureUsingProperties(void* Pixels) const {
        glBindTexture(GL_TEXTURE_2D, this->TextureObject);
        std::cout << this->InternalFormat << '\n'
            << this->Width << '\n'
            << this->Height << '\n'
            << this->Format << '\n'
            << this->FormatType << '\n';

        glTexImage2D(GL_TEXTURE_2D, 0, this->InternalFormat, this->Width, this->Height, 0, this->Format, this->FormatType, Pixels);
        glBindTexture(GL_TEXTURE_2D, 0);
    }

    void Texture::Bind(GLenum ToSlot) {
        this->BoundTo = ToSlot;

        glActiveTexture(ToSlot);
        glBindTexture(GL_TEXTURE_2D, this->TextureObject);
        glActiveTexture(0);
    }

    void Texture::Unbind() {

        if (this->BoundTo != 0) {
            glActiveTexture(this->BoundTo);
            glBindTexture(GL_TEXTURE_2D, 0);
            glActiveTexture(0);

            this->BoundTo = 0;
        }
    }

    char* Texture::ReadPixels(size_t* size, GLint MipmapLevel) const {

        const size_t SizeOfImage = (this->Width * this->Height) * 3;
        if (size != nullptr) {
            *size = SizeOfImage;
        }

        char* TexturePixels = new char[SizeOfImage];
        glGetTexImage(GL_TEXTURE_2D, MipmapLevel, this->Format, this->FormatType, TexturePixels);

        return TexturePixels;
    }

    void Texture::Constructor(GLenum WrapX, GLenum WrapY, GLenum NearFilterType, GLenum FarFilterType) {

        this->WrapX = WrapX;
        this->WrapY = WrapY;

        this->NearFilterType = NearFilterType;
        this->FarFilterType = FarFilterType;

        glGenTextures(1, &this->TextureObject);
        glBindTexture(GL_TEXTURE_2D, this->TextureObject);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, WrapX);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, WrapY);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, NearFilterType);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, FarFilterType);
    }
};