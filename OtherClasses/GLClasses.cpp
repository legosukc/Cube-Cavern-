
#include "./OtherClasses/GLClasses.hpp"

#include <glad/glad.h>

#include "SmartPointer.hpp"

#include <map>

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
};