#ifndef _GLCLASSES_HEADER
#define _GLCLASSES_HEADER

#include <glad/glad.h>

#include "./LuauClasses/Vector3.hpp"
#include "SmartPointer.hpp"

#include <glm/fwd.hpp>

#include <map>

namespace GLClasses {

    constexpr size_t sizeofGLType(GLenum GLType);

    class Texture {

        void Constructor(GLenum WrapX, GLenum WrapY, GLenum NearFilterType, GLenum FarFilterType);

    public:
        inline Texture();

        // Load image as texture
        Texture(const char* textureName, GLenum WrapX, GLenum WrapY, bool Mipmapping = false, GLenum NearFilterType = GL_NEAREST, GLenum FarFilterType = GL_NEAREST_MIPMAP_LINEAR);

        // Create Texture
        Texture(GLint Width, GLint Height, GLenum InternalFormat, GLenum Format, GLenum FormatType, GLenum WrapX, GLenum WrapY, GLenum NearFilterType = GL_NEAREST, GLenum FarFilterType = GL_NEAREST_MIPMAP_LINEAR);

        inline ~Texture();

        GLuint TextureObject;
        GLenum BoundTo = 0;

        GLint Width, Height;
        GLenum InternalFormat, Format, FormatType, WrapX, WrapY, NearFilterType, FarFilterType;

        void CreateTextureUsingProperties(void* Pixels) const;

        void Bind(GLenum ToSlot);
        void Unbind();

        char* ReadPixels(size_t* size = nullptr, GLint MipmapLevel = 0) const;
    };

    // Buffer Classes

    template<GLenum BufferType>
    class BufferBase {
    public:
        inline BufferBase();
        inline BufferBase(const void* Data, GLsizeiptr Size, GLenum Usage = GL_STATIC_DRAW);
        inline BufferBase(GLsizeiptr AllocateBytes, GLenum Usage = GL_STATIC_DRAW);

        inline ~BufferBase();

        GLuint Buffer;

        // Binding Data

        template<typename T>
        static inline void BindDataPointer(const T* Data, GLenum Usage = GL_STATIC_DRAW);

        template<typename T>
        static inline void BindDataVariable(const T Data, GLenum Usage = GL_STATIC_DRAW);

        static inline void BindDataPointerWithSize(const void* Data, GLsizeiptr Size, GLenum Usage = GL_STATIC_DRAW);

        // Subdata

        template<typename T>
        static inline void BindSubDataPointer(const T* Data, GLintptr Offset);

        template<typename T>
        static inline void BindSubDataVariable(const T Data, GLintptr Offset);

        static inline void BindSubDataPointerWithSize(const void* Data, GLsizeiptr Size, GLintptr Offset);

        // Binding Data Helpers

        static inline void AllocateBytes(GLsizeiptr Size, GLenum Usage = GL_STATIC_DRAW);

        // Binding Buffer Helpers

        inline void Bind() const;
        static inline void Unbind();

    protected:
        void Constructor(const void* Data, GLsizeiptr Size, GLenum Usage);
    };

    class VertexArray {

        size_t AttributeOffset = 0;

    public:
        VertexArray(GLsizei StrideSize = 0);
        inline ~VertexArray();

        GLuint Buffer;
        GLsizei StrideSize;
        GLuint AttributeCount = 0;

        inline void Bind() const;
        static inline void Unbind();

        template<GLenum AttributeType>
        constexpr void AddAttribute(GLint AttributeLength, bool Normalized = false);

        template<GLenum AttributeType>
        constexpr void AddAttribute(GLint AttributeLength, GLsizei AttributeOffset, bool Normalized = false);

        template<GLenum AttributeType>
        constexpr void AppendSubAttribute(GLint AttributeLength, GLsizei AttributeBlockSize, bool Normalized = false);
    };

    typedef BufferBase<GL_ARRAY_BUFFER> VertexBuffer;
    typedef BufferBase<GL_ELEMENT_ARRAY_BUFFER> ElementBuffer;

    class UniformBuffer : public BufferBase<GL_UNIFORM_BUFFER> {
    public:

        inline UniformBuffer();

        UniformBuffer(const void* Data, GLsizeiptr Size, GLuint BindingIndex, GLenum Usage = GL_STATIC_DRAW);
        UniformBuffer(GLsizeiptr AllocateBytes, GLuint BindingIndex, GLenum Usage = GL_STATIC_DRAW);

        inline ~UniformBuffer();

        GLuint BufferIndex;

        inline void BindBufferBase() const;
        void BindBufferBase(GLuint BindingIndex);
    };

    class Framebuffer {
    public:
        inline Framebuffer();
        inline ~Framebuffer();

        GLuint buffer;

        inline void BindBuffer() const;
        static inline void BindDefaultFramebuffer();

        static void AttachTexture(GLuint renderTexture, GLenum attachmentType);

        static inline void AttachRenderTexture(GLuint renderTexture);
        static inline void AttachDepthStencilTexture(GLuint renderTexture);

        static inline void AttachDepthTexture(GLuint renderTexture);

        static inline void AttachStencilTexture(GLuint renderTexture);

        static inline bool CheckComplete();
    };


    class Shader {
    public:
        inline Shader(GLenum ShaderType);

        Shader(GLenum ShaderType, const char* ShaderName);

        inline ~Shader();

        GLuint ShaderObject;
    };

    class Program {

        //Uint8 AttachedShadersSize;
        //GLuint AttachedShaders[3];

    public:
        inline Program();
        inline ~Program();

        GLuint glProgram;

        inline GLuint GetUniformBlockIndex(const char* UniformBlockName) const;


        inline void UniformBlockBinding(GLuint BlockIndex, GLuint Binding) const;
        inline void UniformBlockBinding(GLuint BlockIndex, const GLClasses::UniformBuffer& UniformBuffer) const;

        inline void UniformBlockBinding(const char* BlockName, GLuint Binding) const;
        inline void UniformBlockBinding(const char* BlockName, const GLClasses::UniformBuffer& UniformBuffer) const;


        inline GLint GetUniformLocation(const char* UniformName) const;


        static inline void IntSetUniform(GLint UniformID, GLint SetTo);
        inline void IntSetUniform(const char* UniformName, GLint SetTo) const;


        static inline void UintSetUniform(GLint UniformID, GLuint SetTo);
        inline void UintSetUniform(const char* UniformName, GLuint SetTo) const;


        static inline void FloatSetUniform(GLint UniformID, GLfloat SetTo);
        inline void FloatSetUniform(const char* UniformName, GLfloat SetTo) const;


        static inline void DoubleSetUniform(GLint UniformID, GLdouble SetTo);
        inline void DoubleSetUniform(const char* UniformName, GLdouble SetTo) const;


        static inline void Vec3SetUniform(GLint UniformID, const LuauClasses::Vector3& SetTo);
        inline void Vec3SetUniform(const char* UniformName, const LuauClasses::Vector3& SetTo);


        static inline void Mat4SetUniform(GLint UniformID, const glm::mat4& SetTo);
        inline void Mat4SetUniform(const char* UniformName, const glm::mat4& SetTo) const;

        // Misc Uniform Functions

        static inline void SetTextureBinding(GLint TextureUniformID, GLint SetBindingTo);
        inline void SetTextureBinding(const char* TextureUniformName, GLint SetBindingTo) const;

        void LinkProgram() const;

        inline void Use() const;

        void AttachShader(GLuint glShader);
        inline void AttachShader(const Shader& ShaderObject);

        void LoadShader(GLenum ShaderType, const char* ShaderName);
    };

    typedef OtherClasses::SharedPointer<Program, 1> SharedProgramPointer;

    std::map<const char*, SharedProgramPointer> SharedPrograms;
    auto GetSharedProgram(const char* ProgramName);
};

#endif