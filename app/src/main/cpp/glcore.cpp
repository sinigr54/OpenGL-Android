#include <jni.h>
#include <android/asset_manager.h>
#include <android/asset_manager_jni.h>
#include <string>
#include "application/GraphicalApplication.h"
#include <AndroidJNIIOSystem.h>
#include <application/SceneApplication.h>
#include <application/ArApplication.h>

#define JNI_METHOD(return_type, method_name) \
  extern "C" JNIEXPORT return_type JNICALL              \
      Java_com_example_sinigr_openglcourse_application_NativeInterface_##method_name

/*
 * Representation of native application in SDK
 * */
inline jlong applicationPointer(GraphicalApplication *nativeApp) {
    return reinterpret_cast<intptr_t>(nativeApp);
}

inline GraphicalApplication *native(jlong nativeApp) {
    return reinterpret_cast<GraphicalApplication *>(nativeApp);
}

JNI_METHOD(jlong, createNativeApplication)(JNIEnv *env,
                                           jobject,
                                           jobject assetManager,
                                           jstring pathToInternalDir) {

    LOGI("%s", "createNativeApplication");

    const char *cPathToInternalDir = env->GetStringUTFChars(pathToInternalDir, nullptr);
    auto internalPath = std::string(cPathToInternalDir);

    AAssetManager *aAssetManager = AAssetManager_fromJava(env, assetManager);

    GraphicalApplication *nativeApplication =
            new ArApplication(aAssetManager, internalPath);

    env->ReleaseStringUTFChars(pathToInternalDir, cPathToInternalDir);

    return applicationPointer(nativeApplication);
}

JNI_METHOD(void, destroyNativeApplication)(JNIEnv *env, jobject, jlong nativeApp) {
    delete native(nativeApp);
}

JNI_METHOD(void, onPause)(JNIEnv *env, jobject, jlong nativeApp) {
    native(nativeApp)->onPause();
}

JNI_METHOD(void, onResume)(JNIEnv *env, jobject,
                           jlong nativeApp, jobject context, jobject activity) {
    native(nativeApp)->onResume(env, context, activity);
}

JNI_METHOD(void, onSurfaceCreated)(JNIEnv *env, jobject, jlong nativeApp) {
    native(nativeApp)->onSurfaceCreated();
}

JNI_METHOD(void, onDisplayGeometryChanged)(JNIEnv *env, jobject,
                                           jlong nativeApp,
                                           int displayRotation,
                                           jint width,
                                           jint height) {

    native(nativeApp)->onDisplayGeometryChanged(displayRotation, width, height);
}

JNI_METHOD(void, onDrawFrame)(JNIEnv *env, jobject, jlong nativeApp) {
    native(nativeApp)->onDrawFrame();
}






