#include <jni.h>
#include <GLES3/gl3.h>

#define JNI_METHOD(return_type, method_name) \
  extern "C" JNIEXPORT return_type JNICALL              \
      Java_com_example_sinigr_openglcourse_glcore_renderer_NativeRenderer_##method_name

JNI_METHOD(void, nativeOnSurfaceCreated)(JNIEnv *env, jobject) {
    glClearColor(0.0f, 1.0f, 0.0f, 1.0f);
}

JNI_METHOD(void, nativeOnSurfaceChanged)(JNIEnv *env, jobject, jint width, jint height) {
    glViewport(0, 0, width, height);
}

JNI_METHOD(void, nativeOnDrawFrame)(JNIEnv *env, jobject) {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}
