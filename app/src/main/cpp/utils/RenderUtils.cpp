//
// Created by sinigr on 3/19/18.
//

#include <android/asset_manager.h>
#include <jni.h>
#include "RenderUtils.h"

void RenderUtils::checkGlError(const char *operation) {
    for (GLint error = glGetError(); error; error = glGetError()) {
        LOGE("after %s() glError (0x%x)\n", operation, error);
        abort();
    }
}


std::string RenderUtils::loadShaderFromFile(AAssetManager *assetManager,
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

GLuint RenderUtils::loadShader(GLenum sharedType, const char *shaderSource) {
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

GLuint RenderUtils::createProgram(const char *vertexSource, const char *fragmentSource) {
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

GLuint RenderUtils::createProgram(AAssetManager *assetManager,
                                  const std::string &vertexShaderFile,
                                  const std::string &fragmentShaderFile) {

    std::string vertexShader = loadShaderFromFile(assetManager, vertexShaderFile);
    std::string fragmentShader = loadShaderFromFile(assetManager, fragmentShaderFile);

    return createProgram(vertexShader.c_str(), fragmentShader.c_str());
}

/* Загружаем текстуру с помощью Bitmap и GLUtils. Т.к. через asset manager
 * и Bitmap очень легко достать ресурс из assets, а напрямую это сделать невозможно. (?)
 * */
bool RenderUtils::bindTextureSource(int target, const std::string &path) {
    JNIEnv *env = jniEnvironment();

    /*
     * Помещаем все значения JNI в структуру,
     * которая статически инициализируется при первом вызове этого метода.
     * Это делает его потокобезопасным в маловероятном случае вызова этого метода несколькими потоками.
     * */

    static struct JNIData {
        jclass helper_class;
        jmethodID load_image_method;
        jmethodID load_texture_method;
    } jniIds = [env]() -> JNIData {
        constexpr char kHelperClassName[] =
                "com/example/sinigr/openglcourse/application/NativeInterface";
        constexpr char kLoadImageMethodName[] = "loadImage";
        constexpr char kLoadImageMethodSignature[] =
                "(Ljava/lang/String;)Landroid/graphics/Bitmap;";
        constexpr char kLoadTextureMethodName[] = "loadTexture";
        constexpr char kLoadTextureMethodSignature[] =
                "(ILandroid/graphics/Bitmap;)V";
        jclass helper_class = findClass(kHelperClassName);
        if (helper_class) {
            helper_class = static_cast<jclass>(env->NewGlobalRef(helper_class));
            jmethodID load_image_method = env->GetStaticMethodID(
                    helper_class, kLoadImageMethodName, kLoadImageMethodSignature);
            jmethodID load_texture_method = env->GetStaticMethodID(
                    helper_class, kLoadTextureMethodName, kLoadTextureMethodSignature);
            return {helper_class, load_image_method, load_texture_method};
        }

        LOGE("OpenGLCourse::RenderUtils::Could not find Java helper class %s", kHelperClassName);
        return {};
    }();

    if (!jniIds.helper_class) {
        return false;
    }

    jstring j_path = env->NewStringUTF(path.c_str());

    jobject image_obj = env->CallStaticObjectMethod(
            jniIds.helper_class, jniIds.load_image_method, j_path);

    if (j_path) {
        env->DeleteLocalRef(j_path);
    }

    env->CallStaticVoidMethod(jniIds.helper_class, jniIds.load_texture_method,
                              target, image_obj);
    return true;
}

static JavaVM *gVirtualMachine = nullptr;

/* Переопределяем коллбек жизненного цикла JVM */
jint JNI_OnLoad(JavaVM *virtualMachine, void *) {
    gVirtualMachine = virtualMachine;
    return JNI_VERSION_1_6;
}

JNIEnv *RenderUtils::jniEnvironment() {
    JNIEnv *env;
    jint result = gVirtualMachine->AttachCurrentThread(&env, nullptr);
    return result == JNI_OK ? env : nullptr;
}

jclass RenderUtils::findClass(const char *classname) {
    JNIEnv *env = jniEnvironment();
    return env->FindClass(classname);
}

