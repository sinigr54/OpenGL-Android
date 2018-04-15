//
// Created by sinigr on 3/21/18.
//

#include <android/asset_manager.h>
#include "BaseRenderer.h"

BaseRenderer::BaseRenderer(AAssetManager *assetManager) :
        assetManager(assetManager) {

}

BaseRenderer::~BaseRenderer() {

}
