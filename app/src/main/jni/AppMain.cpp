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

using namespace program;
using namespace ogl;


AppMain::AppMain() :
        m_camera(new Camera())
{
    Init();

}
void AppMain::Init() {
    ALOGV("Initializing App...");

    glDisable(GL_CULL_FACE);

    m_programID = program::createProgram(VERTEX_SHADER, FRAGMENT_SHADER);

    glGenVertexArrays(1,&vao);
    glGenBuffers(1, &vbo);
    glGenBuffers(1, &vio);

    glBindVertexArray(vao);

    int vertexCount = 24;
    int sizePerVertex = 6 * sizeof(GLfloat);
    int triangleCount = 12;

    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER,  vertexCount * sizePerVertex, &cubeColoredVertex[0], GL_STATIC_DRAW);


    auto posID = program::Attrib(m_programID, "pos");
    if(posID == -1) {
        ALOGE("Couldn find attribute pos");
    }
    glEnableVertexAttribArray(posID);
    glVertexAttribPointer(posID, 3, GL_FLOAT, GL_FALSE, sizePerVertex, nullptr);


    auto colorID = program::Attrib(m_programID, "color");
    if(colorID == -1) {
        ALOGE("Couldn find attribute color");
    }
    glEnableVertexAttribArray(colorID);
    glVertexAttribPointer(colorID, 3, GL_FLOAT, GL_TRUE, sizePerVertex, reinterpret_cast<const GLvoid*>(3 * sizeof(GLfloat)));

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vio);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, triangleCount * 3 * sizeof(GLuint), &cubeIndices[0], GL_STATIC_DRAW);

    glBindVertexArray(0);
    ALOGV("Initializing App finished.");
}


void AppMain::Resize(int w, int h) {
    m_camera->setViewportAspectRatio((float) w / (float) h);
}

void AppMain::Step() {
    timespec now;
    clock_gettime(CLOCK_MONOTONIC, &now);
    uint64_t nowNs = now.tv_sec * 1000 * 1000 * 1000 + now.tv_nsec;

    if (m_LastFrameNs > 0) {
        float dt = float(nowNs - m_LastFrameNs) / 1000.f / 1000.f / 1000.f;
        Update(dt);
        Render();
        //ALOGD(ToString(dt).c_str());
    }
    m_LastFrameNs = nowNs;
}

void AppMain::Update(float deltaTimeSec) {

}

void AppMain::Render() {
    glClearColor(0.2f, 0.2f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glm::mat4 viewMatrix = glm::lookAt(m_camera->position(),
                                       m_camera->position() + m_camera->forward(),
                                       glm::vec3(0, 1, 0));

    glm::mat4 transform;
    transform = glm::translate(glm::mat4(), glm::vec3(0.f,0.f,-2.f)) *
                glm::scale(glm::mat4(1.f), glm::vec3(20.f,20.f,20.f));

    //transform = helper::Translate(glm::vec3(0.f,0.f,-2)) *
//                                helper::Scale(glm::vec3(20.f,20.f,20.f));

    glUseProgram(m_programID);

    SetUniform(m_programID, "model",        transform , false  );
    SetUniform(m_programID, "view",         viewMatrix, false  );
    SetUniform(m_programID, "projection",   m_camera->projection() ,false  );

    glBindVertexArray(vao);
    glDrawElements(GL_TRIANGLES, 12 * 2, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);   //Unbind
    //draw calls
    checkGlError("Renderer::render");

    eglSwapBuffer;
}



