
#include "./OtherClasses/GLClasses.hpp"

#include <glad/glad.h>

#include <SDL2/SDL.h>
#include <SDL2/SDL_rwops.h>

#include "./FunctionHeaders/AppendCstring.hpp"

#include <iostream>

namespace GLClasses {

    SDL_FORCE_INLINE Shader::Shader(GLenum ShaderType) {
        this->ShaderObject = glCreateShader(ShaderType);
    }

    Shader::Shader(GLenum ShaderType, const char* ShaderName) {

        this->ShaderObject = glCreateShader(ShaderType);

        {
            char* ShaderPath = AppendCstring::AppendCstring("shaders\\", ShaderName);
            char* ShaderSource = reinterpret_cast<char*>(SDL_LoadFile(ShaderPath, NULL));
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

            std::cout << errorLog << '\n' << '\n';
            delete[] errorLog;

            this->~Shader();
        }
    }

    SDL_FORCE_INLINE Shader::~Shader() {
        glDeleteShader(this->ShaderObject);
    }
};