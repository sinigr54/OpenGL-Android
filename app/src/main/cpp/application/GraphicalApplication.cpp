//
// Created by sinigr on 3/21/18.
//

#include <application/renderer/ObjectRenderer.h>
#include <AndroidJNIIOSystem.h>

#include "GraphicalApplication.h"

GraphicalApplication::GraphicalApplication(AAssetManager *assetManager,
                                     const std::string &internalDirectoryPath) :
        assetManager(assetManager),
        internalDirectory(internalDirectoryPath) {
}

GraphicalApplication::~GraphicalApplication() {

}

