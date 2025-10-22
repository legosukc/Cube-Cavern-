#ifndef _GLCLASSES_HEADER
#define _GLCLASSES_HEADER

#include <glad/glad.h>

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_rwops.h>
#include <SDL2/SDL_stdinc.h>
#include <SDL2/SDL_surface.h>

#include "./FunctionHeaders/AppendCstring.hpp"
#include "./OtherClasses/SmartPointer.hpp"

#include "./LuauClasses/Vector3.hpp"
#include "./LuauClasses/Vector2.hpp"

#include <iostream>

#include <glm/glm.hpp>
#include <glm/fwd.hpp>

#include <map>
#include <memory>

namespace GLClasses {

    constexpr size_t sizeofGLType(GLenum GLType) {
        switch (GLType) {

            case GL_BYTE:
                return sizeof(GLbyte);
            case GL_SHORT:
                return sizeof(GLshort);
            case GL_INT:
                return sizeof(GLint);

            case GL_UNSIGNED_BYTE:
                return sizeof(GLubyte);
            case GL_UNSIGNED_SHORT:
                return sizeof(GLushort);
            case GL_UNSIGNED_INT:
                return sizeof(GLuint);

            case GL_HALF_FLOAT:                
                return sizeof(GLhalf);
            case GL_FLOAT:
                return sizeof(GLfloat);
            case GL_DOUBLE:
                return sizeof(GLdouble);
        }
        return 0;
    }

    class Texture {
    public:
        SDL_FORCE_INLINE Texture() {}

        // Load image as texture
        Texture(const char* textureName, GLenum WrapX, GLenum WrapY, bool Mipmapping = false, GLenum NearFilterType = GL_NEAREST, GLenum FarFilterType = GL_NEAREST_MIPMAP_LINEAR) {

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
        Texture(GLint Width, GLint Height, GLenum InternalFormat, GLenum Format, GLenum FormatType, GLenum WrapX, GLenum WrapY, GLenum NearFilterType = GL_NEAREST, GLenum FarFilterType = GL_NEAREST_MIPMAP_LINEAR) {

            this->Width = Width;
            this->Height = Height;

            this->InternalFormat = InternalFormat;
            this->Format = Format;
            this->FormatType = FormatType;

            this->Constructor(WrapX, WrapY, NearFilterType, FarFilterType);

            this->CreateTextureUsingProperties(NULL);

            glBindTexture(GL_TEXTURE_2D, 0);
        }

        SDL_FORCE_INLINE ~Texture() {
            glDeleteTextures(1, &this->TextureObject);
        }

        GLuint TextureObject;
        GLenum BoundTo = 0;

        GLint Width, Height;
        GLenum InternalFormat, Format, FormatType, WrapX, WrapY, NearFilterType, FarFilterType;

        void CreateTextureUsingProperties(void* Pixels) const {
            glBindTexture(GL_TEXTURE_2D, this->TextureObject);
            std::cout << this->InternalFormat << '\n'
                << this->Width << '\n'
                << this->Height << '\n'
                << this->Format << '\n'
                << this->FormatType << '\n';

            glTexImage2D(GL_TEXTURE_2D, 0, this->InternalFormat, this->Width, this->Height, 0, this->Format, this->FormatType, Pixels);
            glBindTexture(GL_TEXTURE_2D, 0);
        }

        void Bind(GLenum ToSlot) {
            this->BoundTo = ToSlot;

            glActiveTexture(ToSlot);
            glBindTexture(GL_TEXTURE_2D, this->TextureObject);
            glActiveTexture(0);
        }

        void Unbind() {

            if (this->BoundTo != 0) {
                glActiveTexture(this->BoundTo);
                glBindTexture(GL_TEXTURE_2D, 0);
                glActiveTexture(0);

                this->BoundTo = 0;
            }
        }

        char* ReadPixels(size_t* size = nullptr, GLint MipmapLevel = 0) const {

            const size_t SizeOfImage = (this->Width * this->Height) * 3;
            if (size != nullptr) {
                *size = SizeOfImage;
            }

            char* TexturePixels = new char[SizeOfImage];
            glGetTexImage(GL_TEXTURE_2D, MipmapLevel, this->Format, this->FormatType, TexturePixels);

            return TexturePixels;
        }

    private:

        void Constructor(GLenum WrapX, GLenum WrapY, GLenum NearFilterType, GLenum FarFilterType) {

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

    // Buffer Classes

    template<GLenum BufferType>
    class BufferBase {
    public:
        SDL_FORCE_INLINE BufferBase() {
            glGenBuffers(1, &this->Buffer);
        }
        SDL_FORCE_INLINE BufferBase(const void* Data, GLsizeiptr Size, GLenum Usage = GL_STATIC_DRAW) {
            this->Constructor(Data, Size, Usage);
        }
        SDL_FORCE_INLINE BufferBase(GLsizeiptr AllocateBytes, GLenum Usage = GL_STATIC_DRAW) {
            this->Constructor(NULL, AllocateBytes, Usage);
        }

        SDL_FORCE_INLINE ~BufferBase() {
            glDeleteBuffers(1, &this->Buffer);
        }

        GLuint Buffer;

        // Binding Data

        template<typename T>
        static SDL_FORCE_INLINE void BindDataPointer(const T* Data, GLenum Usage = GL_STATIC_DRAW) {
            glBufferData(BufferType, sizeof(T), Data, Usage);
        }

        template<typename T>
        static SDL_FORCE_INLINE void BindDataVariable(const T Data, GLenum Usage = GL_STATIC_DRAW) {
            glBufferData(BufferType, sizeof(T), &Data, Usage);
        }

        static SDL_FORCE_INLINE void BindDataPointerWithSize(const void* Data, GLsizeiptr Size, GLenum Usage = GL_STATIC_DRAW) {
            glBufferData(BufferType, Size, Data, Usage);
        }

        // Subdata

        template<typename T>
        static SDL_FORCE_INLINE void BindSubDataPointer(const T* Data, GLintptr Offset) {
            glBufferSubData(BufferType, Offset, sizeof(T), Data);
        }

        template<typename T>
        static SDL_FORCE_INLINE void BindSubDataVariable(const T Data, GLintptr Offset) {
            glBufferSubData(BufferType, Offset, sizeof(T), &Data);
        }

        static SDL_FORCE_INLINE void BindSubDataPointerWithSize(const void* Data, GLsizeiptr Size, GLintptr Offset) {
            glBufferSubData(BufferType, Offset, Size, Data);
        }

        // Binding Data Helpers

        static SDL_FORCE_INLINE void AllocateBytes(GLsizeiptr Size, GLenum Usage = GL_STATIC_DRAW) {
            glBufferData(BufferType, Size, NULL, Usage);
        }

        // Binding Buffer Helpers

        SDL_FORCE_INLINE void Bind() const {
            glBindBuffer(BufferType, this->Buffer);
        }
        static SDL_FORCE_INLINE void Unbind() {
            glBindBuffer(BufferType, 0);
        }
    protected:

        void Constructor(const void* Data, GLsizeiptr Size, GLenum Usage) {
            glGenBuffers(1, &this->Buffer);

            this->Bind();
            this->BindDataPointerWithSize(Data, Size, Usage);
            this->Unbind();
        }
    };

    class VertexArray {

        size_t AttributeOffset = 0;

    public:
        VertexArray(GLsizei StrideSize = 0) {
            glGenVertexArrays(1, &this->Buffer);
            this->StrideSize = StrideSize;
        }

        SDL_FORCE_INLINE ~VertexArray() {
            glDeleteVertexArrays(1, &this->Buffer);
        }

        GLuint Buffer;
        GLsizei StrideSize;
        GLuint AttributeCount = 0;

        SDL_FORCE_INLINE void Bind() const {
            glBindVertexArray(this->Buffer);
        }
        static SDL_FORCE_INLINE void Unbind() {
            glBindVertexArray(0);
        }

        template<GLenum AttributeType>
        constexpr void AddAttribute(GLint AttributeLength, bool Normalized = false) {

            glVertexAttribPointer(this->AttributeCount, AttributeLength, AttributeType, Normalized, this->StrideSize, (void*)this->AttributeOffset);
            glEnableVertexAttribArray(this->AttributeCount);

            this->AttributeOffset += AttributeLength * GLClasses::sizeofGLType(AttributeType);
            ++this->AttributeCount;
        }

        template<GLenum AttributeType>
        constexpr void AddAttribute(GLint AttributeLength, GLsizei AttributeOffset, bool Normalized = false) {
            glVertexAttribPointer(this->AttributeCount, AttributeLength, AttributeType, Normalized, AttributeLength * GLClasses::sizeofGLType(AttributeType), (void*)AttributeOffset);
            glEnableVertexAttribArray(this->AttributeCount);

            ++this->AttributeCount;
        }

        template<GLenum AttributeType>
        constexpr void AppendSubAttribute(GLint AttributeLength, GLsizei AttributeBlockSize, bool Normalized = false) {
            glVertexAttribPointer(this->AttributeCount, AttributeLength, AttributeType, Normalized, AttributeLength * GLClasses::sizeofGLType(AttributeType), (void*)(this->AttributeOffset));
            glEnableVertexAttribArray(this->AttributeCount);

            this->AttributeOffset += AttributeBlockSize;
            ++this->AttributeCount;
        }
    };

    typedef BufferBase<GL_ARRAY_BUFFER> VertexBuffer;
    typedef BufferBase<GL_ELEMENT_ARRAY_BUFFER> ElementBuffer;

    class UniformBuffer : public BufferBase<GL_UNIFORM_BUFFER> {
    public:

        SDL_FORCE_INLINE UniformBuffer() {
        }

        UniformBuffer(const void* Data, GLsizeiptr Size, GLuint BindingIndex, GLenum Usage = GL_STATIC_DRAW) {
            this->Constructor(Data, Size, Usage);
            this->BindBufferBase(BindingIndex);
        }

        UniformBuffer(GLsizeiptr AllocateBytes, GLuint BindingIndex, GLenum Usage = GL_STATIC_DRAW) {
            this->Constructor(NULL, AllocateBytes, Usage);
            this->BindBufferBase(BindingIndex);
        }

        SDL_FORCE_INLINE ~UniformBuffer() {
            glDeleteBuffers(1, &this->Buffer);
        }

        GLuint BufferIndex;

        SDL_FORCE_INLINE void BindBufferBase() const {
            glBindBufferBase(GL_UNIFORM_BUFFER, this->BufferIndex, this->Buffer);
        }
        void BindBufferBase(GLuint BindingIndex) {
            glBindBufferBase(GL_UNIFORM_BUFFER, BindingIndex, this->Buffer);
            this->BufferIndex = BindingIndex;
        }
    };

    class Framebuffer {
    public:
        SDL_FORCE_INLINE Framebuffer() {
            glGenFramebuffers(1, &buffer);
        }
        SDL_FORCE_INLINE ~Framebuffer() {
            glDeleteFramebuffers(1, &buffer);
        }

        GLuint buffer;

        SDL_FORCE_INLINE void BindBuffer() const {
            glBindFramebuffer(GL_FRAMEBUFFER, buffer);
        }
        static SDL_FORCE_INLINE void BindDefaultFramebuffer() {
            glBindFramebuffer(GL_FRAMEBUFFER, 0);
        }

        static void AttachTexture(GLuint renderTexture, GLenum attachmentType) {
            glFramebufferTexture2D(GL_FRAMEBUFFER, attachmentType, GL_TEXTURE_2D, renderTexture, 0);
        }

        static SDL_FORCE_INLINE void AttachRenderTexture(GLuint renderTexture) {
            AttachTexture(renderTexture, GL_COLOR_ATTACHMENT0);
        }
        static SDL_FORCE_INLINE void AttachDepthStencilTexture(GLuint renderTexture) {
            AttachTexture(renderTexture, GL_DEPTH_STENCIL_ATTACHMENT);
        }

        static SDL_FORCE_INLINE void AttachDepthTexture(GLuint renderTexture) {
            AttachTexture(renderTexture, GL_DEPTH_ATTACHMENT);
        }

        static SDL_FORCE_INLINE void AttachStencilTexture(GLuint renderTexture) {
            AttachTexture(renderTexture, GL_STENCIL_ATTACHMENT);
        }

        static SDL_FORCE_INLINE bool CheckComplete() {
            return glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE;
        }
    };


    class Shader {
    public:
        SDL_FORCE_INLINE Shader(GLenum ShaderType) {
            this->ShaderObject = glCreateShader(ShaderType);
        }

        Shader(GLenum ShaderType, const char* ShaderName) {

            this->ShaderObject = glCreateShader(ShaderType);

            {
                char* ShaderPath = AppendCstring::AppendCstring("shaders\\", ShaderName);
                char* ShaderSource = (char*)SDL_LoadFile(ShaderPath, NULL);
                delete[] ShaderPath;

                glShaderSource(this->ShaderObject, 1, &ShaderSource, NULL);
                SDL_free(ShaderSource);
            }
            glCompileShader(this->ShaderObject);

            GLint isCompiled;
            glGetShaderiv(this->ShaderObject, GL_COMPILE_STATUS, &isCompiled);
            if (isCompiled == GL_FALSE) {
                GLint maxLength;
                glGetShaderiv(this->ShaderObject, GL_INFO_LOG_LENGTH, &maxLength);

                GLchar* errorLog = new GLchar[maxLength];
                glGetShaderInfoLog(this->ShaderObject, maxLength, &maxLength, &errorLog[0]);

                std::cerr << errorLog << '\n' << std::endl;
                delete[] errorLog;

                this->~Shader();
            }
        }

        SDL_FORCE_INLINE ~Shader() {
            glDeleteShader(this->ShaderObject);
        }

        GLuint ShaderObject;
    };

    class Program {

        //Uint8 AttachedShadersSize;
        //GLuint AttachedShaders[3];

    public:
        SDL_FORCE_INLINE Program() {
            this->glProgram = glCreateProgram();
            //this->AttachedShadersSize = 0;
        }
        SDL_FORCE_INLINE ~Program() {
            glDeleteProgram(this->glProgram);
        }

        GLuint glProgram;

        SDL_FORCE_INLINE GLuint GetUniformBlockIndex(const char* UniformBlockName) const {
            return glGetUniformBlockIndex(this->glProgram, UniformBlockName);
        }


        SDL_FORCE_INLINE void UniformBlockBinding(GLuint BlockIndex, GLuint Binding) const {
            glUniformBlockBinding(this->glProgram, BlockIndex, Binding);
        }
        SDL_FORCE_INLINE void UniformBlockBinding(GLuint BlockIndex, const GLClasses::UniformBuffer& UniformBuffer) const {
            this->UniformBlockBinding(BlockIndex, UniformBuffer.BufferIndex);
        }

        SDL_FORCE_INLINE void UniformBlockBinding(const char* BlockName, GLuint Binding) const {
            this->UniformBlockBinding(this->GetUniformBlockIndex(BlockName), Binding);
        }
        SDL_FORCE_INLINE void UniformBlockBinding(const char* BlockName, const GLClasses::UniformBuffer& UniformBuffer) const {
            this->UniformBlockBinding(BlockName, UniformBuffer.BufferIndex);
        }


        SDL_FORCE_INLINE GLint GetUniformLocation(const char* UniformName) const {
            return glGetUniformLocation(this->glProgram, UniformName);
        }


        static SDL_FORCE_INLINE void IntSetUniform(GLint UniformID, GLint SetTo) {
            glUniform1i(UniformID, SetTo);
        }
        SDL_FORCE_INLINE void IntSetUniform(const char* UniformName, GLint SetTo) const {
            glUniform1i(this->GetUniformLocation(UniformName), SetTo);
        }


        static SDL_FORCE_INLINE void UintSetUniform(GLint UniformID, GLuint SetTo) {
            glUniform1ui(UniformID, SetTo);
        }
        SDL_FORCE_INLINE void UintSetUniform(const char* UniformName, GLuint SetTo) const {
            glUniform1ui(this->GetUniformLocation(UniformName), SetTo);
        }


        static SDL_FORCE_INLINE void FloatSetUniform(GLint UniformID, GLfloat SetTo) {
            glUniform1f(UniformID, SetTo);
        }
        SDL_FORCE_INLINE void FloatSetUniform(const char* UniformName, GLfloat SetTo) const {
            glUniform1f(this->GetUniformLocation(UniformName), SetTo);
        }


        static SDL_FORCE_INLINE void DoubleSetUniform(GLint UniformID, GLdouble SetTo) {
            glUniform1d(UniformID, SetTo);
        }
        SDL_FORCE_INLINE void DoubleSetUniform(const char* UniformName, GLdouble SetTo) const {
            glUniform1d(this->GetUniformLocation(UniformName), SetTo);
        }


        static SDL_FORCE_INLINE void Vec3SetUniform(GLint UniformID, const LuauClasses::Vector3& SetTo) {
            glUniform3fv(UniformID, 1, &SetTo[0]);
        }
        SDL_FORCE_INLINE void Vec3SetUniform(const char* UniformName, const LuauClasses::Vector3& SetTo) {
            this->Vec3SetUniform(this->GetUniformLocation(UniformName), SetTo);
            //glUniform3fv(this->GetUniformLocation(UniformName), 1, &SetTo[0]);
        }


        static SDL_FORCE_INLINE void Mat4SetUniform(GLint UniformID, const glm::mat4& SetTo) {
            glUniformMatrix4fv(UniformID, 1, GL_FALSE, &SetTo[0][0]);
        }
        SDL_FORCE_INLINE void Mat4SetUniform(const char* UniformName, const glm::mat4& SetTo) const {
            glUniformMatrix4fv(this->GetUniformLocation(UniformName), 1, GL_FALSE, &SetTo[0][0]);
        }

        // Misc Uniform Functions

        static SDL_FORCE_INLINE void SetTextureBinding(GLint TextureUniformID, GLint SetBindingTo) {
            glUniform1i(TextureUniformID, SetBindingTo);
        }
        SDL_FORCE_INLINE void SetTextureBinding(const char* TextureUniformName, GLint SetBindingTo) const {
            this->IntSetUniform(TextureUniformName, SetBindingTo);
        }

        void LinkProgram() const {

            glLinkProgram(this->glProgram);

            GLint isLinked;
            glGetProgramiv(this->glProgram, GL_LINK_STATUS, &isLinked);
            if (isLinked == GL_FALSE) {

                std::cout << "linking failed\n";

                GLint maxLength;
                glGetProgramiv(this->glProgram, GL_INFO_LOG_LENGTH, &maxLength);

                char* InfoLog = new char[maxLength];
                glGetProgramInfoLog(this->glProgram, maxLength, &maxLength, &InfoLog[0]);

                std::cout << InfoLog << '\n' << '\n';
                delete[] InfoLog;

                this->~Program();
                return;
            }

            GLsizei AttachedShadersCount;
            GLuint AttachedShaders[2];
            glGetAttachedShaders(this->glProgram, 2, &AttachedShadersCount, AttachedShaders);

            for (GLsizei i = 0; i < AttachedShadersCount; ++i) {
                std::cout << "detatch\n";
                glDetachShader(this->glProgram, AttachedShaders[i]);
            }
            std::cout << "detatch done\n";
        }

        SDL_FORCE_INLINE void Use() const {
            glUseProgram(this->glProgram);
        }

        void AttachShader(GLuint glShader) {
            glAttachShader(this->glProgram, glShader);

            //this->AttachedShaders[this->AttachedShadersSize] = glShader;
            //++this->AttachedShadersSize;
        }
        SDL_FORCE_INLINE void AttachShader(const Shader& ShaderObject) {
            this->AttachShader(ShaderObject.ShaderObject);
        }

        void LoadShader(GLenum ShaderType, const char* ShaderName) {
            Shader ShaderObject(ShaderType, ShaderName);
            this->AttachShader(ShaderObject.ShaderObject);
        }
    };

    typedef OtherClasses::SharedPointer<Program, 1> SharedProgramPointer;

    std::map<const char*, SharedProgramPointer> SharedPrograms;
    auto GetSharedProgram(const char* ProgramName) {

        struct {
            SharedProgramPointer Program;
            bool IsNewProgram;
        } ReturnStruct;

        ReturnStruct.IsNewProgram = !SharedPrograms.contains(ProgramName);

        if (ReturnStruct.IsNewProgram) {
            SharedPrograms.insert({ ProgramName, SharedProgramPointer() });
        }

        ReturnStruct.Program = SharedPrograms[ProgramName];

        return ReturnStruct;
    }
}

#endif