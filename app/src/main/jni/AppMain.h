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

    GLuint vertexbuffer;
    GLuint m_programID;

    glm::mat4 transform;
    float degrees;
    GLuint vao;
    GLuint vbo;
    GLuint vio;

};


#endif //OPENGL_ES_EXAMPLE_APPMAIN_H
