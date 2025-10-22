
#include "./OtherClasses/GLClasses.hpp"

#include <glad/glad.h>

#include <SDL2/SDL.h>

#include "./LuauClasses/Vector3.hpp"
#include "./LuauClasses/Vector2.hpp"

#include <iostream>

#include <glm/glm.hpp>
#include <glm/fwd.hpp>

namespace GLClasses {

    SDL_FORCE_INLINE Program::Program() {
        this->glProgram = glCreateProgram();
        //this->AttachedShadersSize = 0;
    }
    SDL_FORCE_INLINE Program::~Program() {
        glDeleteProgram(this->glProgram);
    }

    SDL_FORCE_INLINE GLuint Program::GetUniformBlockIndex(const char* UniformBlockName) const {
        return glGetUniformBlockIndex(this->glProgram, UniformBlockName);
    }


    SDL_FORCE_INLINE void Program::UniformBlockBinding(GLuint BlockIndex, GLuint Binding) const {
        glUniformBlockBinding(this->glProgram, BlockIndex, Binding);
    }
    SDL_FORCE_INLINE void Program::UniformBlockBinding(GLuint BlockIndex, const GLClasses::UniformBuffer& UniformBuffer) const {
        this->UniformBlockBinding(BlockIndex, UniformBuffer.BufferIndex);
    }

    SDL_FORCE_INLINE void Program::UniformBlockBinding(const char* BlockName, GLuint Binding) const {
        this->UniformBlockBinding(this->GetUniformBlockIndex(BlockName), Binding);
    }
    SDL_FORCE_INLINE void Program::UniformBlockBinding(const char* BlockName, const GLClasses::UniformBuffer& UniformBuffer) const {
        this->UniformBlockBinding(BlockName, UniformBuffer.BufferIndex);
    }


    SDL_FORCE_INLINE GLint Program::GetUniformLocation(const char* UniformName) const {
        return glGetUniformLocation(this->glProgram, UniformName);
    }


    SDL_FORCE_INLINE void Program::IntSetUniform(GLint UniformID, GLint SetTo) {
        glUniform1i(UniformID, SetTo);
    }
    SDL_FORCE_INLINE void Program::IntSetUniform(const char* UniformName, GLint SetTo) const {
        glUniform1i(this->GetUniformLocation(UniformName), SetTo);
    }


    SDL_FORCE_INLINE void Program::UintSetUniform(GLint UniformID, GLuint SetTo) {
        glUniform1ui(UniformID, SetTo);
    }
    SDL_FORCE_INLINE void Program::UintSetUniform(const char* UniformName, GLuint SetTo) const {
        glUniform1ui(this->GetUniformLocation(UniformName), SetTo);
    }


    SDL_FORCE_INLINE void Program::FloatSetUniform(GLint UniformID, GLfloat SetTo) {
        glUniform1f(UniformID, SetTo);
    }
    SDL_FORCE_INLINE void Program::FloatSetUniform(const char* UniformName, GLfloat SetTo) const {
        glUniform1f(this->GetUniformLocation(UniformName), SetTo);
    }


    SDL_FORCE_INLINE void Program::DoubleSetUniform(GLint UniformID, GLdouble SetTo) {
        glUniform1d(UniformID, SetTo);
    }
    SDL_FORCE_INLINE void Program::DoubleSetUniform(const char* UniformName, GLdouble SetTo) const {
        glUniform1d(this->GetUniformLocation(UniformName), SetTo);
    }


    SDL_FORCE_INLINE void Program::Vec3SetUniform(GLint UniformID, const LuauClasses::Vector3& SetTo) {
        glUniform3fv(UniformID, 1, &SetTo[0]);
    }
    SDL_FORCE_INLINE void Program::Vec3SetUniform(const char* UniformName, const LuauClasses::Vector3& SetTo) {
        this->Vec3SetUniform(this->GetUniformLocation(UniformName), SetTo);
        //glUniform3fv(this->GetUniformLocation(UniformName), 1, &SetTo[0]);
    }


    SDL_FORCE_INLINE void Program::Mat4SetUniform(GLint UniformID, const glm::mat4& SetTo) {
        glUniformMatrix4fv(UniformID, 1, GL_FALSE, &SetTo[0][0]);
    }
    SDL_FORCE_INLINE void Program::Mat4SetUniform(const char* UniformName, const glm::mat4& SetTo) const {
        glUniformMatrix4fv(this->GetUniformLocation(UniformName), 1, GL_FALSE, &SetTo[0][0]);
    }

    // Misc Uniform Functions

    SDL_FORCE_INLINE void Program::SetTextureBinding(GLint TextureUniformID, GLint SetBindingTo) {
        glUniform1i(TextureUniformID, SetBindingTo);
    }
    SDL_FORCE_INLINE void Program::SetTextureBinding(const char* TextureUniformName, GLint SetBindingTo) const {
        this->IntSetUniform(TextureUniformName, SetBindingTo);
    }

    void Program::LinkProgram() const {

        glLinkProgram(this->glProgram);

        GLint isLinked;
        glGetProgramiv(this->glProgram, GL_LINK_STATUS, &isLinked);
        if (isLinked == GL_FALSE) {

            std::cerr << "linking failed" << std::endl;

            GLint maxLength;
            glGetProgramiv(this->glProgram, GL_INFO_LOG_LENGTH, &maxLength);

            char* InfoLog = new char[maxLength];
            glGetProgramInfoLog(this->glProgram, maxLength, &maxLength, &InfoLog[0]);

            std::cerr << InfoLog << '\n' << std::endl;
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

    SDL_FORCE_INLINE void Program::Use() const {
        glUseProgram(this->glProgram);
    }

    void Program::AttachShader(GLuint glShader) {
        glAttachShader(this->glProgram, glShader);

        //this->AttachedShaders[this->AttachedShadersSize] = glShader;
        //++this->AttachedShadersSize;
    }
    SDL_FORCE_INLINE void Program::AttachShader(const Shader& ShaderObject) {
        this->AttachShader(ShaderObject.ShaderObject);
    }

    void Program::LoadShader(GLenum ShaderType, const char* ShaderName) {
        Shader ShaderObject(ShaderType, ShaderName);
        this->AttachShader(ShaderObject.ShaderObject);
    }
};