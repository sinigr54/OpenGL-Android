#include <jni.h>
#include <string>

#define JNI_METHOD(return_type, method_name) \
  extern "C" JNIEXPORT return_type JNICALL              \
      Java_com_example_sinigr_nativetest_MainActivity_##method_name

JNI_METHOD(jstring, stringFromJNI)
(JNIEnv *env, jobject /* this */) {
    std::string hello = "Hello from C++";
    return env->NewStringUTF(hello.c_str());
}
