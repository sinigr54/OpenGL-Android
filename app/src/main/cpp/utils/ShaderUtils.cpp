//
// Created by sinigr on 3/19/18.
//

#include "ShaderUtils.h"

void ShaderUtils::checkGlError(const char *operation) {
    for (GLint error = glGetError(); error; error = glGetError()) {
        LOGE("after %s() glError (0x%x)\n", operation, error);
        abort();
    }
}

GLuint ShaderUtils::loadShader(GLenum sharedType, const char *shaderSource) {
    GLuint shader = glCreateShader(sharedType);
    if (!shader) {
        return shader;
    }

    glShaderSource(shader, 1, &shaderSource, nullptr);
    glCompileShader(shader);

    GLint compiled = 0;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &compiled);

    if (!compiled) {
        GLint infoLen = 0;
        glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &infoLen);
        if (!infoLen) {
            return shader;
        }

        char *buffer = new char[infoLen];

        glGetShaderInfoLog(shader, infoLen, nullptr, buffer);
        LOGE("ShaderUtils::Could not compile shader %d:\n%s\n",
             sharedType, buffer);

        delete[] buffer;

        glDeleteShader(shader);
        shader = 0;
    }

    return shader;
}

GLuint ShaderUtils::createProgram(const char *vertexSource, const char *fragmentSource) {
    GLuint vertexShader = loadShader(GL_VERTEX_SHADER, vertexSource);
    if (!vertexShader) {
        return 0;
    }

    GLuint fragmentShader = loadShader(GL_FRAGMENT_SHADER, fragmentSource);
    if (!fragmentShader) {
        return 0;
    }

    GLuint program = glCreateProgram();
    if (program) {
        glAttachShader(program, vertexShader);
        checkGlError("ShaderUtil::glAttachShader");
        glAttachShader(program, fragmentShader);
        checkGlError("ShaderUtil::glAttachShader");
        glLinkProgram(program);

        GLint linkStatus = GL_FALSE;
        glGetProgramiv(program, GL_LINK_STATUS, &linkStatus);
        if (linkStatus != GL_TRUE) {
            GLint bufferLength = 0;
            glGetProgramiv(program, GL_INFO_LOG_LENGTH, &bufferLength);

            if (bufferLength) {
                char *buffer = new char[bufferLength];
                glGetProgramInfoLog(program, bufferLength, nullptr, buffer);
                LOGE("ShaderUtils::Could not link program:\n%s\n", buffer);
                delete[] buffer;
            }

            glDeleteProgram(program);
            program = 0;
        }
    }

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    return program;
}




