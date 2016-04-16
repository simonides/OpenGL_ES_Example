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

using namespace program;
using namespace ogl;


AppMain::AppMain() :
        mEglContext(eglGetCurrentContext()),
        degrees(0.f),
        degreesY(0.f),
    m_camera(new Camera())
{
    Init();

}


void AppMain::Init() {
    ALOGV("Initializing App...");

    glEnable(GL_CULL_FACE);
    glFrontFace(GL_CCW);

    m_programID = program::createProgram(VERTEX_SHADER, FRAGMENT_SHADER);


    glGenVertexArrays(1,&vao);
    glGenBuffers(1, &vbo);
    glGenBuffers(1, &vio);

    glBindVertexArray(vao);
    glBindBuffer(GL_ARRAY_BUFFER,vbo);
    glBufferData(GL_ARRAY_BUFFER,   vertexCount * sizeOfVertex, cubeColoredVertex, GL_STATIC_DRAW);
//    glBufferData(GL_ARRAY_BUFFER,   vertexCount * sizeOfVertex, &cubeColoredVertex[0], GL_STATIC_DRAW);
//
//
    auto posID = program::Attrib(m_programID, "pos");
    ALOGD("simon %d",posID );
    glEnableVertexAttribArray(posID);
    glVertexAttribPointer(posID, 3, GL_FLOAT, GL_FALSE, 6*sizeof(GLfloat),(void*)0);


    auto colorID = program::Attrib(m_programID, "color");
    glEnableVertexAttribArray(colorID);
    glVertexAttribPointer(colorID, 3, GL_FLOAT, GL_TRUE, 6*sizeof(GLfloat)
            , reinterpret_cast<const GLvoid*>(3 * sizeof(GLfloat)));

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vio);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, triangleCount *  sizeOfIndexData, cubeIndices, GL_STATIC_DRAW);
//
//    glBindBuffer(GL_ARRAY_BUFFER, 0);
//    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
    ALOGV("Initializing App finished.");
}


void AppMain::Resize(int w, int h) {
    //ALOGD("Resize called: %d, %d", w,h);
    m_camera->setViewportAspectRatio((float) w / (float) h);
    glViewport(0, 0, w, h);
}

void AppMain::Step() {
    timespec now;
    clock_gettime(CLOCK_MONOTONIC, &now);
    uint64_t nowNs = now.tv_sec*1000000000ull + now.tv_nsec;

    if (m_LastFrameNs > 0) {
        float dt = float(nowNs - m_LastFrameNs) * 0.000000001f;
        Update(dt);
        Render();
    }
    m_LastFrameNs = nowNs;
}

void AppMain::Update(float deltaTimeSec) {
    const GLfloat degreesPerSecond = 60.0f;
    const GLfloat degreesPerSecondY = 30.0f;
    degrees += deltaTimeSec * degreesPerSecond;
    degreesY += deltaTimeSec * degreesPerSecondY;
    //ALOGD("degrees: %f" , degrees);
    while(degrees > 360.0f) {degrees -= 360.0f;}
    while(degreesY > 360.0f) {degreesY -= 360.0f;}

    transform = glm::rotate(glm::mat4(), glm::radians(degrees),  glm::vec3(0,1,0));
    transform = glm::rotate(transform, glm::radians(degreesY), glm::vec3(1,0,0));

}

void AppMain::Render() {
    glClearColor(0.5f, 0.5f, 0.55f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glm::mat4 viewMatrix = glm::lookAt(m_camera->position(),
                                       m_camera->position() + m_camera->forward(),
                                       m_camera->up());


    glUseProgram(m_programID);

    SetUniform(m_programID, "model",        transform ,              false );
    SetUniform(m_programID, "view",         viewMatrix ,             false );
    SetUniform(m_programID, "projection",   m_camera->projection() , false  );

    glEnableVertexAttribArray(0);

    glBindVertexArray(vao);
    glDrawElements(GL_TRIANGLES,  triangleCount*3, GL_UNSIGNED_INT, (void*)0 );
    glBindVertexArray(0);


    checkGlError("Renderer::render");

}


AppMain::~AppMain() {
    ALOGE("Killing AppMain....");
//    if (vbo) {
//        glDeleteBuffers(1, &vbo);
//        vbo = 0;
//    }

//    if (ubo_) {
//        glDeleteBuffers(1, &ubo_);
//        ubo_ = 0;
//    }
//    if (ibo_) {
//        glDeleteBuffers(1, &ibo_);
//        ibo_ = 0;
//    }
//    if (m_programID) {
//        glDeleteProgram(m_programID);
//        m_programID = 0;
//    }

}
