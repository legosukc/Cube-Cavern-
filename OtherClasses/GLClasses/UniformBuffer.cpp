
#include "./OtherClasses/GLClasses.hpp"

#include <glad/glad.h>
#include <SDL2/SDL.h>

namespace GLClasses {

    SDL_FORCE_INLINE UniformBuffer::UniformBuffer() {
        this->BufferIndex = 0;
    }

    UniformBuffer::UniformBuffer(const void* Data, GLsizeiptr Size, GLuint BindingIndex, GLenum Usage) {
        this->Constructor(Data, Size, Usage);
        this->BindBufferBase(BindingIndex);
    }

    UniformBuffer::UniformBuffer(GLsizeiptr AllocateBytes, GLuint BindingIndex, GLenum Usage) {
        this->Constructor(NULL, AllocateBytes, Usage);
        this->BindBufferBase(BindingIndex);
    }

    SDL_FORCE_INLINE UniformBuffer::~UniformBuffer() {
        glDeleteBuffers(1, &this->Buffer);
    }

    SDL_FORCE_INLINE void UniformBuffer::BindBufferBase() const {
        glBindBufferBase(GL_UNIFORM_BUFFER, this->BufferIndex, this->Buffer);
    }
    void UniformBuffer::BindBufferBase(GLuint BindingIndex) {
        glBindBufferBase(GL_UNIFORM_BUFFER, BindingIndex, this->Buffer);
        this->BufferIndex = BindingIndex;
    }
};