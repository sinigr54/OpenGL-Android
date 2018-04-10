//
// Created by sinigr on 4/10/18.
//

#ifndef OPENGLCOURSE_SHADER_H
#define OPENGLCOURSE_SHADER_H

#include <GLES3/gl3.h>
#include <string>
#include <glm.hpp>
#include <gtc/type_ptr.hpp>
#include <android/asset_manager.h>
#include <utils/RenderUtils.h>

class Shader {
    GLuint program;

    void checkGlError(const char *operation) {
        for (GLint error = glGetError(); error; error = glGetError()) {
            LOGE("after %s() glError (0x%x)\n", operation, error);
            abort();
        }
    }

    GLuint loadShader(GLenum sharedType, const char *shaderSource) {
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
            LOGE("RenderUtils::Could not compile shader %d:\n%s\n",
                 sharedType, buffer);

            delete[] buffer;

            glDeleteShader(shader);
            shader = 0;
        }

        return shader;
    }

    std::string loadShaderFromFile(AAssetManager *assetManager,
                                   const std::string &fileName) {
        AAsset *asset = AAssetManager_open(assetManager,
                                           fileName.c_str(),
                                           AASSET_MODE_STREAMING);
        if (asset == nullptr) {
            LOGE("Error opening asset %s", fileName.c_str());
            return 0;
        }

        off_t fileSize = AAsset_getLength(asset);
        std::string fileBuffer;
        fileBuffer.resize(fileSize);
        int result = AAsset_read(asset, &fileBuffer.front(), fileSize);
        if (result < 0 || result == EOF) {
            LOGE("Failed to open file: %s", fileName.c_str());
            AAsset_close(asset);
            return 0;
        }

        AAsset_close(asset);
        return fileBuffer;
    }

    GLuint createProgram(AAssetManager *assetManager,
                         const std::string &vertexShaderFile,
                         const std::string &fragmentShaderFile) {
        std::string vertexShader = loadShaderFromFile(assetManager, vertexShaderFile);
        std::string fragmentShader = loadShaderFromFile(assetManager, fragmentShaderFile);

        return createProgram(vertexShader.c_str(), fragmentShader.c_str());
    }

    GLuint createProgram(const char *vertexSource, const char *fragmentSource) {
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
                    LOGE("RenderUtils::Could not link program:\n%s\n", buffer);
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

public:
    void init(AAssetManager *assetManager,
              const std::string &vertexShaderFile,
              const std::string &fragmentShaderFile) {
        program = createProgram(assetManager, vertexShaderFile, fragmentShaderFile);
    }

    void init(const char *vertexSource, const char *fragmentSource) {
        program = createProgram(vertexSource, fragmentSource);
    }

    void use() {
        glUseProgram(program);
    }

    void setUniform(const std::string &parameter, GLfloat value) {
        glUniform1f(glGetUniformLocation(program, parameter.c_str()), value);
    }

    void setUniform(const std::string &parameter, GLint value) {
        glUniform1i(glGetUniformLocation(program, parameter.c_str()), value);
    }

    void setUniform(const std::string &parameter, const glm::vec3 &value) {
        glUniform3f(glGetUniformLocation(program, parameter.c_str()), value.x, value.y, value.z);
    }

    void setUniform(const std::string &parameter, const glm::mat4 &value) {
        glUniformMatrix4fv(glGetUniformLocation(program, parameter.c_str()),
                           1, GL_FALSE, glm::value_ptr(value));
    }
};

#endif //OPENGLCOURSE_SHADER_H
