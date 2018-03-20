//
// Created by sinigr on 3/19/18.
//

#ifndef OPENGLCOURSE_SHADERUTILS_H
#define OPENGLCOURSE_SHADERUTILS_H

#include <GLES3/gl3.h>
#include <android/log.h>
#include <stdlib.h>
#include <string>
#include <android/asset_manager.h>

#ifndef LOGI
#define LOGI(...) \
  __android_log_print(ANDROID_LOG_INFO, "OpenGLCourse", __VA_ARGS__)
#endif  // LOGI

#ifndef LOGE
#define LOGE(...) \
  __android_log_print(ANDROID_LOG_ERROR, "OpenGLCourse", __VA_ARGS__)
#endif  // LOGE

class ShaderUtils {
    static void checkGlError(const char *operation);

    static GLuint loadShader(GLenum sharedType, const char *shaderSource);

    static std::string loadShaderFromFile(AAssetManager *assetManager,
                                    const std::string &fileName);

public:
    static GLuint createProgram(AAssetManager *assetManager,
                                const std::string &vertexShaderFile,
                                const std::string &fragmentShaderFile);

    static GLuint createProgram(const char *vertexSource, const char *fragment_source);
};


#endif //OPENGLCOURSE_SHADERUTILS_H
