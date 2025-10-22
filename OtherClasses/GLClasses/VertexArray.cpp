
#include "./OtherClasses/GLClasses.hpp"

#include <glad/glad.h>

namespace GLClasses {

    VertexArray::VertexArray(GLsizei StrideSize) {
        glGenVertexArrays(1, &this->Buffer);
        this->StrideSize = StrideSize;
    }

    VertexArray::~VertexArray() {
        glDeleteVertexArrays(1, &this->Buffer);
    }

    void VertexArray::Bind() const {
        glBindVertexArray(this->Buffer);
    }
    void VertexArray::Unbind() {
        glBindVertexArray(0);
    }

    template<GLenum AttributeType>
    constexpr void VertexArray::AddAttribute(GLint AttributeLength, bool Normalized) {

        glVertexAttribPointer(this->AttributeCount, AttributeLength, AttributeType, Normalized, this->StrideSize, (void*)this->AttributeOffset);
        glEnableVertexAttribArray(this->AttributeCount);

        this->AttributeOffset += AttributeLength * GLClasses::sizeofGLType(AttributeType);
        ++this->AttributeCount;
    }

    template<GLenum AttributeType>
    constexpr void VertexArray::AddAttribute(GLint AttributeLength, GLsizei AttributeOffset, bool Normalized) {
        glVertexAttribPointer(this->AttributeCount, AttributeLength, AttributeType, Normalized, AttributeLength * GLClasses::sizeofGLType(AttributeType), (void*)AttributeOffset);
        glEnableVertexAttribArray(this->AttributeCount);

        ++this->AttributeCount;
    }

    template<GLenum AttributeType>
    constexpr void VertexArray::AppendSubAttribute(GLint AttributeLength, GLsizei AttributeBlockSize, bool Normalized) {
        glVertexAttribPointer(this->AttributeCount, AttributeLength, AttributeType, Normalized, AttributeLength * GLClasses::sizeofGLType(AttributeType), (void*)(this->AttributeOffset));
        glEnableVertexAttribArray(this->AttributeCount);

        this->AttributeOffset += AttributeBlockSize;
        ++this->AttributeCount;
    }
};