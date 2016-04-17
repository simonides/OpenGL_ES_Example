//
// Created by Simon on 17.04.2016.
//

#include <android/asset_manager.h>
#include <assert.h>
#include <GLES3/gl3.h>
#include <ktx20/ktx.h>
#include "Texture.h"
#include "Helper.h"

Texture texture::LoadTextureFromAssetManager(AAssetManager *assetManager, const char *fileName) {

    AAsset *textureAsset = nullptr;
    textureAsset = AAssetManager_open(assetManager, fileName, AASSET_MODE_BUFFER);
    assert(textureAsset != nullptr);

    Texture tex;
    const void *textureData = AAsset_getBuffer(textureAsset);
    GLsizei textureDataSize = (GLsizei) AAsset_getLength(textureAsset);
    GLenum glResult;
    KTX_error_code ktxResult;

// This call is really needed twice, or otherwise the creation of the texture won't work when the surface is recreated.
    ktxResult = ktxLoadTextureM(textureData, textureDataSize, &tex.m_textureHandle, &tex.m_textureTarget,
                                nullptr, &tex.m_isMipmapped, &glResult, 0, nullptr);

    ALOGD("Ktx result first call: %d", ktxResult);
    ktxResult = ktxLoadTextureM(textureData, textureDataSize, &tex.m_textureHandle, &tex.m_textureTarget,
                                nullptr, &tex.m_isMipmapped, &glResult, 0, nullptr);

    ALOGD("Texture handle id: %d enum: %d", tex.m_textureHandle, tex.m_textureTarget);
    if (ktxResult != KTX_SUCCESS) {
        ALOGE("KTXLib couldn't load texture %s. Error: %d", fileName, ktxResult);
        assert(false);
    }

    AAsset_close(textureAsset);
    return tex;
}

Texture::Texture() :
        m_textureTarget(0),
        m_textureHandle(0),
        m_isMipmapped(false) {


}

void Texture::BindTexture() {
    glBindTexture(m_textureTarget,m_textureHandle);
}
