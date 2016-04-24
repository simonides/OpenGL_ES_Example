//
// Created by Simon on 17.04.2016.
//

#ifndef OPENGL_ES_EXAMPLE_TEXTURE_H
#define OPENGL_ES_EXAMPLE_TEXTURE_H

class AAssetManager;
class Texture;

namespace texture{
    Texture LoadTextureFromAssetManager(AAssetManager* assetManager, const char * fileName);
}

class Texture{
public:
    friend Texture texture::LoadTextureFromAssetManager(AAssetManager* assetManager, const char * fileName);

    Texture();
    void BindTexture();

private:
    GLuint m_textureHandle;
    GLenum m_textureTarget;
    GLboolean m_isMipmapped;
};
#endif //OPENGL_ES_EXAMPLE_TEXTURE_H
