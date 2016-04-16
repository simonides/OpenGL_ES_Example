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
#include "Config.h"

class Camera;


class AppMain {
public:
    AppMain();
    ~AppMain();
    void Init();
    void Step();
    void Update(float deltaTimeSec);
    void Render();
    void Resize(int w, int h);

private:
    Camera* m_camera;
    uint64_t m_LastFrameNs;

    //********************************
//    GLuint vertexbuffer;
//    GLuint vertexindexbuffer;

    int vertexCount = 24;
    int triangleCount = 12;
    int sizeOfVertex = 6* sizeof(GLfloat);
    int sizeOfIndexData = 3* sizeof(GLuint);


    glm::mat4 transform;

    //********************************

    float degrees;
    GLuint m_programID;
    GLuint vao;
    GLuint vbo;
    GLuint vio;

};


#endif //OPENGL_ES_EXAMPLE_APPMAIN_H
