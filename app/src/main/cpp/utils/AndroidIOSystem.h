//
// Created by sinigr on 4/20/18.
//

#if __ANDROID__ and __ANDROID_API__ > 9 and defined(AI_CONFIG_ANDROID_JNI_ASSIMP_MANAGER_SUPPORT)
#ifndef OPENGLCOURSE_ANDROIDIOSYSTEM_H
#define OPENGLCOURSE_ANDROIDIOSYSTEM_H

#include <assimp/DefaultIOSystem.h>
#include <android/asset_manager.h>
#include <android/asset_manager_jni.h>
#include <android/native_activity.h>

namespace Assimp {

// ---------------------------------------------------------------------------
/** Android extension to DefaultIOSystem using the standard C file functions */
class AndroidIOSystem : public DefaultIOSystem {
public:

    /** Initialize android activity data */
    std::string mApkWorkspacePath;
    AAssetManager *mApkAssetManager;

    /** Constructor. */
    AndroidIOSystem(ANativeActivity *activity);

    AndroidIOSystem(AAssetManager *assetManager,
                       const std::string &internalPathDirectory);

    /** Destructor. */
    ~AndroidIOSystem();

    // -------------------------------------------------------------------
    /** Tests for the existence of a file at the given path. */
    bool Exists(const char *pFile) const;

    // -------------------------------------------------------------------
    /** Opens a file at the given path, with given mode */
    IOStream *Open(const char *strFile, const char *strMode);

    // ------------------------------------------------------------------------------------------------
    // Inits Android extractor
    void AndroidActivityInit(ANativeActivity *activity);

    // ------------------------------------------------------------------------------------------------
    // Extracts android asset
    bool AndroidExtractAsset(std::string name);

};

} //!ns Assimp

#endif //OPENGLCOURSE_ANDROIDIOSYSTEM_H
#endif //__ANDROID__ and __ANDROID_API__ > 9 and defined(AI_CONFIG_ANDROID_JNI_ASSIMP_MANAGER_SUPPORT)