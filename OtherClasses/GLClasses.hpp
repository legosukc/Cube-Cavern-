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
#include <exception>
#include <stdexcept>

#include <glm/glm.hpp>
#include <glm/fwd.hpp>

#include <map>
#include <memory>

namespace GLClasses {

    constexpr size_t sizeofGLType(GLenum Enum) {

        switch (Enum) {

            case GL_BYTE: [[fallthrough]]
            case GL_UNSIGNED_BYTE:
                return 1;

            case GL_SHORT: [[fallthrough]]
            case GL_UNSIGNED_SHORT: [[fallthrough]]
            case GL_HALF_FLOAT:
                return 2;

            case GL_INT: [[fallthrough]]
            case GL_UNSIGNED_INT: [[fallthrough]]
            case GL_FLOAT:
                return 4;

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

        inline ~Texture() {
            glDeleteTextures(1, &this->TextureObject);
        }

        GLuint TextureObject;
        GLenum BoundTo = 0;

        GLint Width, Height;
        GLenum InternalFormat, Format, FormatType, WrapX, WrapY, NearFilterType, FarFilterType;

        void CreateTextureUsingProperties(const void* Pixels) const {
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

        inline BufferBase() {
            glGenBuffers(1, &this->Buffer);
        }

        BufferBase(const void* Data, GLsizeiptr Size, GLenum Usage = GL_STATIC_DRAW) : BufferBase() {
            this->Bind();
            this->BindDataPointerWithSize(Data, Size, Usage);
            this->Unbind();
        }

        inline BufferBase(GLsizeiptr AllocateBytes, GLenum Usage = GL_STATIC_DRAW) : BufferBase(NULL, AllocateBytes, Usage) {}

        inline ~BufferBase() {
            glDeleteBuffers(1, &this->Buffer);
        }

        GLuint Buffer;

        // Binding Data

        template<typename T>
        static inline void BindDataPointer(const T* Data, GLenum Usage = GL_STATIC_DRAW) {
            glBufferData(BufferType, sizeof(T), Data, Usage);
        }

        template<typename T>
        static inline void BindDataVariable(const T Data, GLenum Usage = GL_STATIC_DRAW) {
            glBufferData(BufferType, sizeof(T), &Data, Usage);
        }

        static inline void BindDataPointerWithSize(const void* Data, GLsizeiptr Size, GLenum Usage = GL_STATIC_DRAW) {
            glBufferData(BufferType, Size, Data, Usage);
        }

        // Subdata

        template<typename T>
        static inline void BindSubDataPointer(const T* Data, GLintptr Offset) {
            glBufferSubData(BufferType, Offset, sizeof(T), Data);
        }

        template<typename T>
        static inline void BindSubDataVariable(const T Data, GLintptr Offset) {
            glBufferSubData(BufferType, Offset, sizeof(T), &Data);
        }

        static inline void BindSubDataPointerWithSize(const void* Data, GLsizeiptr Size, GLintptr Offset) {
            glBufferSubData(BufferType, Offset, Size, Data);
        }

        static inline void ReadFromBuffer(void* Data, GLsizeiptr ReadBytes, GLintptr Offset) {
            glGetBufferSubData(BufferType, Offset, ReadBytes, Data);
        }

        static char* ReadFromBuffer(GLsizeiptr ReadBytes, GLintptr Offset) {

            char* Data = new char[ReadBytes];
            ReadFromBuffer(Data, ReadBytes, Offset);
            return Data;
        }

        // Binding Data Helpers

        static inline void AllocateBytes(GLsizeiptr Size, GLenum Usage = GL_STATIC_DRAW) {
            glBufferData(BufferType, Size, NULL, Usage);
        }

        // Binding Buffer Helpers

        inline void Bind() const {
            glBindBuffer(BufferType, this->Buffer);
        }
        static inline void Unbind() {
            glBindBuffer(BufferType, 0);
        }
    };

    class VertexArray {

        size_t AttributeOffset = 0;

    public:
        VertexArray(GLsizei StrideSize = 0) {
            glGenVertexArrays(1, &this->Buffer);
            this->StrideSize = StrideSize;
        }

        inline ~VertexArray() {
            glDeleteVertexArrays(1, &this->Buffer);
        }

        GLuint Buffer;
        GLsizei StrideSize;
        GLuint AttributeCount = 0;

        inline void Bind() const {
            glBindVertexArray(this->Buffer);
        }
        static inline void Unbind() {
            glBindVertexArray(0);
        }

        void AddAttribute(GLint AttributeLength, GLenum AttributeType, bool Normalized = false) {

            glVertexAttribPointer(this->AttributeCount, AttributeLength, AttributeType, Normalized, this->StrideSize, (void*)this->AttributeOffset);
            glEnableVertexAttribArray(this->AttributeCount);

            this->AttributeOffset += AttributeLength * GLClasses::sizeofGLType(AttributeType);
            ++this->AttributeCount;
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

        inline UniformBuffer(const void* Data, GLsizeiptr Size, GLuint BindingIndex, GLenum Usage = GL_STATIC_DRAW) : BufferBase<GL_UNIFORM_BUFFER>(Data, Size, Usage) {
            this->BindBufferBase(BindingIndex);
        }

        inline UniformBuffer(GLsizeiptr AllocateBytes, GLuint BindingIndex, GLenum Usage = GL_STATIC_DRAW) : UniformBuffer(NULL, AllocateBytes, BindingIndex, Usage) {}

        inline ~UniformBuffer() {
            glDeleteBuffers(1, &this->Buffer);
        }

        GLuint BufferIndex;

        inline void BindBufferBase() const {
            glBindBufferBase(GL_UNIFORM_BUFFER, this->BufferIndex, this->Buffer);
        }
        void BindBufferBase(GLuint BindingIndex) {
            glBindBufferBase(GL_UNIFORM_BUFFER, BindingIndex, this->Buffer);
            this->BufferIndex = BindingIndex;
        }
    };

    class Framebuffer {
    public:
        inline Framebuffer() {
            glGenFramebuffers(1, &this->buffer);
        }
        inline ~Framebuffer() {
            glDeleteFramebuffers(1, &this->buffer);
        }

        GLuint buffer;

        inline void BindBuffer() const {
            glBindFramebuffer(GL_FRAMEBUFFER, this->buffer);
        }
        static inline void BindDefaultFramebuffer() {
            glBindFramebuffer(GL_FRAMEBUFFER, 0);
        }

        static inline void AttachTexture(GLuint RenderTexture, GLenum AttachmentType) {
            glFramebufferTexture2D(GL_FRAMEBUFFER, AttachmentType, GL_TEXTURE_2D, RenderTexture, 0);
        }

        static inline void AttachRenderTexture(GLuint RenderTexture) {
            AttachTexture(RenderTexture, GL_COLOR_ATTACHMENT0);
        }
        static inline void AttachDepthStencilTexture(GLuint RenderTexture) {
            AttachTexture(RenderTexture, GL_DEPTH_STENCIL_ATTACHMENT);
        }

        static inline void AttachDepthTexture(GLuint RenderTexture) {
            AttachTexture(RenderTexture, GL_DEPTH_ATTACHMENT);
        }

        static inline void AttachStencilTexture(GLuint RenderTexture) {
            AttachTexture(RenderTexture, GL_STENCIL_ATTACHMENT);
        }

        static inline bool CheckComplete() {
            return glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE;
        }
    };


    class Shader {
    public:
        inline Shader(GLenum ShaderType) {
            this->ShaderObject = glCreateShader(ShaderType);
        }

        Shader(GLenum ShaderType, const char* ShaderName) : Shader(ShaderType) {

            //this->ShaderObject = glCreateShader(ShaderType);

            this->LoadSource(ShaderName);
            this->Compile();
        }

        inline ~Shader() {
            glDeleteShader(this->ShaderObject);
        }

        inline void SetSource(const char* Source) {
            glShaderSource(this->ShaderObject, 1, &Source, NULL);
        }

        /*
        * Looks in the 'shaders' directory, and sets the contents as the shader's source.
        * Returns true on success, or false on error
        */
        void LoadSource(const char* ShaderName) {

            std::filesystem::path ShaderPath = "shaders\\";
            ShaderPath.append(ShaderName);

            if (!std::filesystem::exists(ShaderPath)) {

                std::string ErrorMessage = "Failed to load shader: ";
                ErrorMessage += ShaderPath.string();
                ErrorMessage += "\nFile doesn't exist.\n";
                ErrorMessage += "Line: ";
                ErrorMessage += __LINE__;
                ErrorMessage += " in header: ";
                ErrorMessage += __BASE_FILE__;

                throw std::runtime_error(ErrorMessage);
            }

            std::ifstream ShaderFile(ShaderPath);
            if (!ShaderFile.is_open()) {

                std::string ErrorMessage = "Failed to load shader: ";
                ErrorMessage += ShaderPath.string();
                ErrorMessage += "\nCouldn't open file for reading.\n";
                ErrorMessage += "Line: ";
                ErrorMessage += __LINE__;
                ErrorMessage += " in header: ";
                ErrorMessage += __BASE_FILE__;

                throw std::runtime_error(ErrorMessage);
            }

            ShaderFile.seekg(0, std::ios::end);
            const std::streamsize FileSize = ShaderFile.tellg();
            ShaderFile.seekg(0, std::ios::beg);

            auto ShaderSourceBuffer = std::make_unique<char>(FileSize + 1);

            ShaderFile.read(ShaderSourceBuffer.get(), FileSize);
            ShaderSourceBuffer.get()[FileSize] = '\0';

            this->SetSource(ShaderSourceBuffer.get());
        }

        /*
        * Compiles whatever was set with SetSource.
        */
        void Compile() {

            glCompileShader(this->ShaderObject);

            GLint isCompiled;
            glGetShaderiv(this->ShaderObject, GL_COMPILE_STATUS, &isCompiled);
            if (isCompiled == GL_FALSE) {

                GLint maxLength;
                glGetShaderiv(this->ShaderObject, GL_INFO_LOG_LENGTH, &maxLength);

                auto ErrorLogBuffer = std::make_unique<GLchar>(maxLength);
                glGetShaderInfoLog(this->ShaderObject, maxLength, &maxLength, ErrorLogBuffer.get());

                std::string ErrorMessage = "Failed to compile shader.\nCompilation error log:\n";
                ErrorMessage += ErrorLogBuffer.get();
                ErrorMessage.push_back('\n');
                ErrorMessage.push_back('\n');

                ErrorMessage += "Line: ";
                ErrorMessage += __LINE__;
                ErrorMessage += " in header: ";
                ErrorMessage += __BASE_FILE__;

                throw std::runtime_error(ErrorMessage);
            }
        }

        GLuint ShaderObject;
    };

    class Program {

        //Uint8 AttachedShadersSize;
        //GLuint AttachedShaders[3];

    public:
        inline Program() {
            this->glProgram = glCreateProgram();
            //this->AttachedShadersSize = 0;
        }
        inline ~Program() {
            glDeleteProgram(this->glProgram);
        }

        GLuint glProgram;

        inline GLuint GetUniformBlockIndex(const char* UniformBlockName) const {
            return glGetUniformBlockIndex(this->glProgram, UniformBlockName);
        }


        inline void UniformBlockBinding(GLuint BlockIndex, GLuint Binding) {
            glUniformBlockBinding(this->glProgram, BlockIndex, Binding);
        }
        inline void UniformBlockBinding(GLuint BlockIndex, const GLClasses::UniformBuffer& UniformBuffer) {
            this->UniformBlockBinding(BlockIndex, UniformBuffer.BufferIndex);
        }

        inline void UniformBlockBinding(const char* BlockName, GLuint Binding) {
            this->UniformBlockBinding(this->GetUniformBlockIndex(BlockName), Binding);
        }
        inline void UniformBlockBinding(const char* BlockName, const GLClasses::UniformBuffer& UniformBuffer) {
            this->UniformBlockBinding(BlockName, UniformBuffer.BufferIndex);
        }


        inline GLint GetUniformLocation(const char* UniformName) {
            return glGetUniformLocation(this->glProgram, UniformName);
        }


        static inline void IntSetUniform(GLint UniformID, GLint SetTo) {
            glUniform1i(UniformID, SetTo);
        }
        inline void IntSetUniform(const char* UniformName, GLint SetTo) {
            glUniform1i(this->GetUniformLocation(UniformName), SetTo);
        }


        static inline void UintSetUniform(GLint UniformID, GLuint SetTo) {
            glUniform1ui(UniformID, SetTo);
        }
        inline void UintSetUniform(const char* UniformName, GLuint SetTo) {
            glUniform1ui(this->GetUniformLocation(UniformName), SetTo);
        }


        static inline void FloatSetUniform(GLint UniformID, GLfloat SetTo) {
            glUniform1f(UniformID, SetTo);
        }
        inline void FloatSetUniform(const char* UniformName, GLfloat SetTo) {
            glUniform1f(this->GetUniformLocation(UniformName), SetTo);
        }


        static inline void DoubleSetUniform(GLint UniformID, GLdouble SetTo) {
            glUniform1d(UniformID, SetTo);
        }
        inline void DoubleSetUniform(const char* UniformName, GLdouble SetTo) {
            glUniform1d(this->GetUniformLocation(UniformName), SetTo);
        }


        static inline void Vec3SetUniform(GLint UniformID, const LuauClasses::Vector3& SetTo) {
            glUniform3fv(UniformID, 1, &SetTo[0]);
        }
        inline void Vec3SetUniform(const char* UniformName, const LuauClasses::Vector3& SetTo) {
            this->Vec3SetUniform(this->GetUniformLocation(UniformName), SetTo);
            //glUniform3fv(this->GetUniformLocation(UniformName), 1, &SetTo[0]);
        }


        static inline void Mat4SetUniform(GLint UniformID, const glm::mat4& SetTo) {
            glUniformMatrix4fv(UniformID, 1, GL_FALSE, &SetTo[0][0]);
        }
        inline void Mat4SetUniform(const char* UniformName, const glm::mat4& SetTo) {
            this->Mat4SetUniform(this->GetUniformLocation(UniformName), SetTo);
        }

        // Misc Uniform Functions

        static inline void SetTextureBinding(GLint TextureUniformID, GLint SetBindingTo) {
            glUniform1i(TextureUniformID, SetBindingTo);
        }
        inline void SetTextureBinding(const char* TextureUniformName, GLint SetBindingTo) {
            this->IntSetUniform(TextureUniformName, SetBindingTo);
        }

        /*
        * Links the program, completing the final step before the ShaderProgram is ready to be used.
        * Make sure you have attatched all the shaders you want to be linked inside the program.
        * Throws a runtime error when linkage fails.
        */
        void LinkProgram() {

            glLinkProgram(this->glProgram);

            GLint isLinked;
            glGetProgramiv(this->glProgram, GL_LINK_STATUS, &isLinked);
            if (isLinked == GL_FALSE) {

                //std::cerr << "Linking failed." << std::endl;

                std::string ErrorMessage = "::OPENGL ERROR:: Failed to link program\nLinkage error log:\n";

                {
                    GLint LogSize;
                    glGetProgramiv(this->glProgram, GL_INFO_LOG_LENGTH, &LogSize);

                    auto ErrorLogBuffer = std::make_unique<GLchar>(LogSize);
                    glGetProgramInfoLog(this->glProgram, LogSize, &LogSize, ErrorLogBuffer.get());

                    ErrorMessage += ErrorLogBuffer.get();
                }

                ErrorMessage.push_back('\n');

                ErrorMessage += "Line: ";
                ErrorMessage += __LINE__;
                ErrorMessage += " in header: ";
                ErrorMessage += __BASE_FILE__;

                throw std::runtime_error(ErrorMessage);
            }
            /*
            GLsizei AttachedShadersCount;
            GLuint AttachedShaders[2];
            glGetAttachedShaders(this->glProgram, 2, &AttachedShadersCount, AttachedShaders);

            for (GLsizei i = 0; i < AttachedShadersCount; ++i) {
                std::cout << "detatch\n";
                glDetachShader(this->glProgram, AttachedShaders[i]);
            }
            std::cout << "detatch done\n";*/
        }

        inline void Use() const {
            glUseProgram(this->glProgram);
        }

        static inline void Dispatch(GLuint GroupXSize, GLuint GroupYSize, GLuint GroupZSize) {
            glDispatchCompute(GroupXSize, GroupYSize, GroupZSize);
        }

        inline void AttachShader(GLuint glShader) {
            glAttachShader(this->glProgram, glShader);

            //this->AttachedShaders[this->AttachedShadersSize] = glShader;
            //++this->AttachedShadersSize;
        }
        inline void AttachShader(const Shader& ShaderObject) {
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