#include <jni.h>
#include <android/asset_manager.h>
#include <android/asset_manager_jni.h>
#include <memory>
#include <application/NativeApplication.h>

#define JNI_METHOD(return_type, method_name) \
  extern "C" JNIEXPORT return_type JNICALL              \
      Java_com_example_sinigr_openglcourse_application_NativeApplication_##method_name

NativeApplication *nativeApplication;

JNI_METHOD(void, createNativeApplication)(JNIEnv *env, jobject, jobject assetManager) {
    nativeApplication = new NativeApplication(AAssetManager_fromJava(env, assetManager));
}

JNI_METHOD(void, destroyNativeApplication)(JNIEnv *env, jobject) {
    delete nativeApplication;
}

JNI_METHOD(void, onPause)(JNIEnv *env, jobject) {
    nativeApplication->onPause();
}

JNI_METHOD(void, onResume)(JNIEnv *env, jobject, jobject context, jobject activity) {
    nativeApplication->onResume();
}

JNI_METHOD(void, onSurfaceCreated)(JNIEnv *env, jobject) {
    nativeApplication->onSurfaceCreated();
}

JNI_METHOD(void, onSurfaceChanged)(JNIEnv *env, jobject, jint width, jint height) {
    nativeApplication->onSurfaceChanged(width, height);
}

JNI_METHOD(void, onDrawFrame)(JNIEnv *env, jobject) {
    nativeApplication->onDrawFrame();
}







