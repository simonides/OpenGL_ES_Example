//
// Created by Simon on 15.04.2016.
//

#include <time.h>
#include "AppMain.h"
#include "Camera.h"
#include "Program.h"
#include "Helper.h"
#include "Shader.h"
#include "Vertices.h"

#include <GLES3/gl3.h>
#include <glm/gtc/matrix_transform.hpp>
#include <EGL/egl.h>
#include <ktx20/ktx.h>
#include "android/asset_manager.h"
#include "android/asset_manager_jni.h"


using namespace program;
using namespace ogl;

AppMain::AppMain(AAssetManager *assetManager) :
        m_eglContext(eglGetCurrentContext()),
        m_assetManager(assetManager),
        degrees(0.f),
        degreesY(0.f),
        m_LastFrameNs(0),
        m_camera(new Camera()) {

    Init();

}


void AppMain::Init() {
    ALOGV("Initializing App...");

    glEnable(GL_CULL_FACE);
    glFrontFace(GL_CCW);

    m_programID = program::createProgram(VERTEX_SHADER_TEX, FRAGMENT_SHADER_TEX);


    glGenVertexArrays(1, &vao);
    glGenBuffers(1, &vbo);
    glGenBuffers(1, &vio);

    glBindVertexArray(vao);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, vertexCount * sizeOfVertex, cubeColoredVertex, GL_STATIC_DRAW);
//    glBufferData(GL_ARRAY_BUFFER,   vertexCount * sizeOfVertex, &cubeColoredVertex[0], GL_STATIC_DRAW);
//
//
    auto posID = program::Attrib(m_programID, "pos");
    glEnableVertexAttribArray(posID);
    glVertexAttribPointer(posID, 3, GL_FLOAT, GL_FALSE, sizeOfVertex, (void *) 0);

    auto uvID = program::Attrib(m_programID, "vertexUV");
    glEnableVertexAttribArray(uvID);
    glVertexAttribPointer(uvID, 2, GL_FLOAT, GL_FALSE, sizeOfVertex
            , reinterpret_cast<const GLvoid*>(3 * sizeof(GLfloat)));


//    auto colorID = program::Attrib(m_programID, "color");
//    glEnableVertexAttribArray(colorID);
//    glVertexAttribPointer(colorID, 3, GL_FLOAT, GL_FALSE, sizeOfVertex,
//                          reinterpret_cast<const GLvoid *>(5 * sizeof(GLfloat)));

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vio);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, triangleCount * sizeOfIndexData, cubeIndices,
                 GL_STATIC_DRAW);
//
//    glBindBuffer(GL_ARRAY_BUFFER, 0);
//    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    glBindVertexArray(0);





    //***********************************************************************************************
    assert(m_assetManager != nullptr);
    AAsset *textureAsset = nullptr;
    const char *textureFileName = "Mountain.ktx";
    textureAsset = AAssetManager_open(m_assetManager, textureFileName, AASSET_MODE_BUFFER);

    assert(textureAsset != nullptr);

    const void *textureData;
    textureData = AAsset_getBuffer(textureAsset);
    GLsizei textureDataSize = (GLsizei) AAsset_getLength(textureAsset);


    GLboolean isMipmapped;
    GLenum glResult;
    KTX_error_code ktxResult;

// This call is really needed twice, or otherwise the creation of the texture won't work when the surface is recreated.
    ktxResult = ktxLoadTextureM(textureData, textureDataSize, &textureHandle, &textureTarget, NULL,
                                &isMipmapped, &glResult, 0, NULL);
    ktxResult = ktxLoadTextureM(textureData, textureDataSize, &textureHandle, &textureTarget, NULL,
                                &isMipmapped, &glResult, 0, NULL);

    ALOGV("Texture handle id: %d enum: %d", textureHandle, textureTarget);
    if (ktxResult != KTX_SUCCESS) {
        ALOGE("KTXLib couldn't load texture %s. Error: %d", textureFileName, ktxResult);
        assert(false);
    }

    AAsset_close(textureAsset);

    //***********************************************************************************************
    ALOGV("Initializing App finished.");
}


void AppMain::Resize(int w, int h) {
    ALOGD("Resize called: %d, %d", w, h);
    m_camera->setViewportAspectRatio((float) w / (float) h);
    glViewport(0, 0, w, h);
}

void AppMain::Step() {
    timespec now;
    clock_gettime(CLOCK_MONOTONIC, &now);
    uint64_t nowNs = now.tv_sec * 1000000000ull + now.tv_nsec;

//    ALOGD("Step!");
    if (m_LastFrameNs > 0) {
        float dt = float(nowNs - m_LastFrameNs) * 0.000000001f;
        dt = clamp(dt, 0.0001, 0.5);
//        ALOGD("Update & Render @ %f fps", 1.f/dt);
        Update(dt);
//        ALOGD("Render...");
        Render();
//        ALOGD("DONE");
    }

    m_LastFrameNs = nowNs;
}

void AppMain::Update(float deltaTimeSec) {
    const GLfloat degreesPerSecond = 60.0f;
    const GLfloat degreesPerSecondY = 20.0f;
    degrees += deltaTimeSec * degreesPerSecond;
    degreesY += deltaTimeSec * degreesPerSecondY;
//    ALOGD("degrees: (delta time = %f)  %f / %f" , deltaTimeSec, degrees, degreesY);
    if (degrees >= 360.0f) {
        degrees -= ((int) degrees / 360) * 360.f;
    }
    if (degreesY >= 360.0f) {
        degreesY -= ((int) degreesY / 360) * 360.f;
    }

    transform = glm::rotate(glm::mat4(), glm::radians(degrees), glm::vec3(0, 1, 0));
    transform = glm::rotate(transform, glm::radians(degreesY), glm::vec3(1, 0, 0));

}

void AppMain::Render() {
    glClearColor(0.5f, 0.5f, 0.55f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glm::mat4 viewMatrix = glm::lookAt(m_camera->position(),
                                       m_camera->position() + m_camera->forward(),
                                       m_camera->up());


    glUseProgram(m_programID);
    glBindVertexArray(vao);

    //todo extract
    glBindTexture(textureTarget, textureHandle);
    GLuint TextureID  = glGetUniformLocation(m_programID, "textureSampler");
    // Set our "myTextureSampler" sampler to user Texture Unit 0
    glUniform1i(TextureID, 0);


    SetUniform(m_programID, "model", glm::mat4(1.0f), false);
    SetUniform(m_programID, "view", glm::mat4(1.0f), false);
    SetUniform(m_programID, "projection", glm::mat4(1.0f), false);
    glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT,
                   reinterpret_cast<const GLvoid *>(36 * sizeof(GLuint)));


    //draw cube
    SetUniform(m_programID, "model", transform, false);
    SetUniform(m_programID, "view", viewMatrix, false);
    SetUniform(m_programID, "projection", m_camera->projection(), false);

    glDrawElements(GL_TRIANGLES, triangleCount * 3 - 3, GL_UNSIGNED_INT, (void *) 0);


    glEnableVertexAttribArray(0);
    glBindVertexArray(0);
    checkGlError("Renderer::render");
}


AppMain::~AppMain() {
    ALOGE("Killing AppMain....");

    glDeleteBuffers(1, &vao);
    glDeleteBuffers(1, &vbo);
    glDeleteBuffers(1, &vio);

    glDeleteProgram(m_programID);
}


