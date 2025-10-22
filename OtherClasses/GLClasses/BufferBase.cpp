
#include "./OtherClasses/GLClasses.hpp"

#include <glad/glad.h>

namespace GLClasses {

    template<GLenum BufferType>
    BufferBase<BufferType>::BufferBase() {
        glGenBuffers(1, &this->Buffer);
    }

    template<GLenum BufferType>
    BufferBase<BufferType>::BufferBase(const void* Data, GLsizeiptr Size, GLenum Usage) {
        this->Constructor(Data, Size, Usage);
    }

    template<GLenum BufferType>
    BufferBase<BufferType>::BufferBase(GLsizeiptr AllocateBytes, GLenum Usage) {
        this->Constructor(NULL, AllocateBytes, Usage);
    }

    template<GLenum BufferType>
    BufferBase<BufferType>::~BufferBase() {
        glDeleteBuffers(1, &this->Buffer);
    }

    // Binding Data

    template<GLenum BufferType> template<typename T>
    void BufferBase<BufferType>::BindDataPointer(const T* Data, GLenum Usage) {
        glBufferData(BufferType, sizeof(T), Data, Usage);
    }

    template<GLenum BufferType> template<typename T>
    void BufferBase<BufferType>::BindDataVariable(const T Data, GLenum Usage) {
        glBufferData(BufferType, sizeof(T), &Data, Usage);
    }

    template<GLenum BufferType>
    void BufferBase<BufferType>::BindDataPointerWithSize(const void* Data, GLsizeiptr Size, GLenum Usage) {
        glBufferData(BufferType, Size, Data, Usage);
    }

    // Subdata

    template<GLenum BufferType> template<typename T>
    void BufferBase<BufferType>::BindSubDataPointer(const T* Data, GLintptr Offset) {
        glBufferSubData(BufferType, Offset, sizeof(T), Data);
    }

    template<GLenum BufferType> template<typename T>
    void BufferBase<BufferType>::BindSubDataVariable(const T Data, GLintptr Offset) {
        glBufferSubData(BufferType, Offset, sizeof(T), &Data);
    }

    template<GLenum BufferType>
    void BufferBase<BufferType>::BindSubDataPointerWithSize(const void* Data, GLsizeiptr Size, GLintptr Offset) {
        glBufferSubData(BufferType, Offset, Size, Data);
    }

    // Binding Data Helpers

    template<GLenum BufferType>
    void BufferBase<BufferType>::AllocateBytes(GLsizeiptr Size, GLenum Usage) {
        glBufferData(BufferType, Size, NULL, Usage);
    }

    // Binding Buffer Helpers

    template<GLenum BufferType>
    void BufferBase<BufferType>::Bind() const {
        glBindBuffer(BufferType, this->Buffer);
    }

    template<GLenum BufferType>
    void BufferBase<BufferType>::Unbind() {
        glBindBuffer(BufferType, 0);
    }

    template<GLenum BufferType>
    void BufferBase<BufferType>::Constructor(const void* Data, GLsizeiptr Size, GLenum Usage) {
        glGenBuffers(1, &this->Buffer);

        this->Bind();
        this->BindDataPointerWithSize(Data, Size, Usage);
        this->Unbind();
    }
};