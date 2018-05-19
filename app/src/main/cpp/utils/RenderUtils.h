//
// Created by sinigr on 3/19/18.
//

#ifndef OPENGLCOURSE_SHADERUTILS_H
#define OPENGLCOURSE_SHADERUTILS_H

#include <GLES3/gl3.h>
#include <android/log.h>
#include <stdlib.h>
#include <string>
#include <jni.h>
#include <android/asset_manager.h>

#ifndef LOGI
#define LOGI(...) \
  __android_log_print(ANDROID_LOG_WARN, "OpenGLCourse", __VA_ARGS__)
#endif  // LOGI

#ifndef LOGE
#define LOGE(...) \
  __android_log_print(ANDROID_LOG_ERROR, "OpenGLCourse", __VA_ARGS__)
#endif  // LOGE

#ifndef CHECK
#define CHECK(condition)                                                   \
  if (!(condition)) {                                                      \
    LOGE("*** CHECK FAILED at %s:%d: %s", __FILE__, __LINE__, #condition); \
    abort();                                                               \
  }
#endif

#include "arcore_c_api.h"
#include "glm.hpp"

// Provides a scoped allocated instance of Anchor.
// Can be treated as an ArAnchor*.
class ScopedArPose {
public:
    explicit ScopedArPose(const ArSession *session) {
        ArPose_create(session, nullptr, &pose_);
    }

    ~ScopedArPose() { ArPose_destroy(pose_); }

    ArPose *getArPose() { return pose_; }

    // Delete copy constructors.
    ScopedArPose(const ScopedArPose &) = delete;

    void operator=(const ScopedArPose &) = delete;

private:
    ArPose *pose_;
};

// Get the plane's normal from center pose.
glm::vec3 getPlaneNormal(const ArSession* arSession, const ArPose& arPlanePos);

float calculateDistanceToPlane(const ArSession *ar_session,
                               const ArPose &plane_pose,
                               const ArPose &camera_pose);

void getTransformMatrixFromAnchor(ArSession *arSession,
                                  const ArAnchor *arAnchor,
                                  glm::mat4 *outModelMat);

class RenderUtils {
    static void checkGlError(const char *operation);

    static GLuint loadShader(GLenum sharedType, const char *shaderSource);

    static std::string loadShaderFromFile(AAssetManager *assetManager,
                                          const std::string &fileName);

    static JNIEnv *jniEnvironment();

    static jclass findClass(const char *classname);

public:
    static bool bindTextureSource(int target, const std::string &path);

    static GLuint createProgram(AAssetManager *assetManager,
                                const std::string &vertexShaderFile,
                                const std::string &fragmentShaderFile);

    static GLuint createProgram(const char *vertexSource, const char *fragment_source);
};


#endif //OPENGLCOURSE_SHADERUTILS_H
