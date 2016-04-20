//
// Created by Simon on 15.04.2016.
//

#ifndef OPENGL_ES_EXAMPLE_APPMAIN_H
#define OPENGL_ES_EXAMPLE_APPMAIN_H


#include <jni.h>
#include <stdint.h>
#include <GLES3/gl3.h>
#include <glm/detail/type_mat.hpp>
#include <glm/detail/type_mat4x4.hpp>
#include <EGL/egl.h>
#include <glm/detail/type_vec3.hpp>
#include "Config.h"
#include "Texture.h"

class Camera;
class AAssetManager;

class AppMain {
public:
    explicit AppMain(AAssetManager* assetManager);
    ~AppMain();
    void Init();
    void Step();
    void Update(float deltaTimeSec);
    void Render();
    void Resize(int w, int h);

private:
    const EGLContext m_eglContext;
    AAssetManager* m_assetManager;

    Camera* m_camera;
    uint64_t m_LastFrameNs;

    //********************************

    int vertexCount = 36;
    int triangleCount = 12 + 1 +( 1 + 1 +1); // cube + background + birds
    int cubeTriangleCount = 12;

    int sizeOfVertex = 8* sizeof(GLfloat);
    int sizeOfIndexData = 3* sizeof(GLuint);


    glm::mat4 transform;

    glm::vec3 bird1Pos;
    glm::mat4 bird1Mat;
    glm::vec3 bird2Pos;
    glm::mat4 bird2Mat;
    glm::vec3 bird3Pos;
    glm::mat4 bird3Mat;

    float birdMovementProgress[3];
    //********************************

    float degrees;
    float degreesY;
    GLuint m_programID;
    GLuint vao;
    GLuint vbo;
    GLuint vio;

    Texture m_backgroundTex;
    Texture m_birdTex;
};


#endif //OPENGL_ES_EXAMPLE_APPMAIN_H
