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
degrees(0.f),
        m_camera(new Camera())
{
    Init();

}

static const GLfloat g_vertex_buffer_data[] = {
        -1.0f, -1.0f, 0.0f,
        1.0f, -1.0f, 0.0f,
        0.0f,  1.0f, 0.0f,
};

void AppMain::Init() {
    ALOGV("Initializing App...");

    glDisable(GL_CULL_FACE);
    glFrontFace(GL_CCW);

    m_programID = program::createProgram(VERTEX_SHADER, FRAGMENT_SHADER);


    // This will identify our vertex buffer

// Generate 1 buffer, put the resulting identifier in vertexbuffer
    glGenBuffers(1, &vertexbuffer);
// The following commands will talk about our 'vertexbuffer' buffer
    glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
// Give our vertices to OpenGL.
    glBufferData(GL_ARRAY_BUFFER, sizeof(g_vertex_buffer_data), g_vertex_buffer_data, GL_STATIC_DRAW);
    glVertexAttribPointer(
            0,                  // attribute 0. No particular reason for 0, but must match the layout in the shader.
            3,                  // size
            GL_FLOAT,           // type
            GL_FALSE,           // normalized?
            0,                  // stride
            (void*)0            // array buffer offset
    );
// Draw the triangle !
//    glGenVertexArrays(1,&vao);
//    glGenBuffers(1, &vbo);
//    glGenBuffers(1, &vio);
//
//    glBindVertexArray(vao);
//
//    glBindBuffer(GL_ARRAY_BUFFER,vbo);
//    glBufferData(GL_ARRAY_BUFFER,  24 * 6 *sizeof(GLfloat), &cubeColoredVertex[0], GL_STATIC_DRAW);
//
//
//    auto posID = program::Attrib(m_programID, "pos");
//    ALOGD("simon %d",posID );
//    glEnableVertexAttribArray(posID);
//    glVertexAttribPointer(posID, 3, GL_FLOAT, GL_FALSE, 3*sizeof(GLfloat), nullptr);
//
//
//    auto colorID = program::Attrib(m_programID, "color");
//    glEnableVertexAttribArray(colorID);
//    glVertexAttribPointer(colorID, 3, GL_FLOAT, GL_FALSE, 3*sizeof(GLfloat)
//            , reinterpret_cast<const GLvoid*>(3 * sizeof(GLfloat)));
//
//    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vio);
//    glBufferData(GL_ELEMENT_ARRAY_BUFFER, 12 * 3 * sizeof(GLfloat), &cubeIndices[0], GL_STATIC_DRAW);
//
//    glBindVertexArray(0);
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
    uint64_t nowNs = now.tv_sec * 1000 * 1000 * 1000 + now.tv_nsec;

    if (m_LastFrameNs > 0) {
        float dt = float(nowNs - m_LastFrameNs) / 1000.f / 1000.f / 1000.f;
        dt = clamp(dt,0.001f,0.2f);
        Update(dt);
        Render();
        //ALOGD(ToString(dt).c_str());
    }
    m_LastFrameNs = nowNs;
}

void AppMain::Update(float deltaTimeSec) {
//float test = 0.1f*deltaTimeSec;
//ALOGD("frametime: %f", test);
// m_camera->offsetOrientation(0.f,test);

    const GLfloat degreesPerSecond = 180.0f;
    degrees += deltaTimeSec * degreesPerSecond;
   // ALOGD("degrees: %f" , degrees);
//    while(degrees > 360.0f) degrees -= 360.0f;
    if(degrees > 360.f){
        degrees -= 360.f;
    }
    transform = glm::rotate(glm::mat4(), glm::radians(degrees), glm::vec3(0,1,0));

}

void AppMain::Render() {
    glClearColor(0.2f, 0.2f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glm::mat4 viewMatrix = glm::lookAt(m_camera->position(),
                                       m_camera->position() + m_camera->forward(),
                                       m_camera->up());

    viewMatrix =  m_camera->orientation() * glm::translate(glm::mat4(), -m_camera->position());



//    glm::mat4 transform;
//  transform =
//          glm::translate(glm::mat4(), glm::vec3(0.f,0.f,0.f))*
//          glm::scale(glm::mat4(1.f), glm::vec3(0.5f,0.5f,0.5f));




    glUseProgram(m_programID);

    SetUniform(m_programID, "model",        transform ,              false );
    SetUniform(m_programID, "view",         viewMatrix ,             false );
    SetUniform(m_programID, "projection",   m_camera->projection() , false  );

    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
// Draw the triangle !
    glDrawArrays(GL_TRIANGLES, 0, 3); // Starting from vertex 0; 3 vertices total -> 1 triangle
    glDisableVertexAttribArray(0);


//    transform =
//            glm::translate(glm::mat4(), glm::vec3(2.f,2.f,0.f))*
//            glm::scale(glm::mat4(1.f), glm::vec3(0.5f,0.5f,0.5f));
//    SetUniform(m_programID, "model",        transform ,true  );
//
//    glEnableVertexAttribArray(0);
//    glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
//// Draw the triangle !
//    glDrawArrays(GL_TRIANGLES, 0, 3); // Starting from vertex 0; 3 vertices total -> 1 triangle
//    glDisableVertexAttribArray(0);

//    glBindVertexArray(vao);
//    glDrawElements(GL_TRIANGLES, 8 * 3, GL_UNSIGNED_INT, 0);
//    glBindVertexArray(0);   //Unbind
////    draw calls

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
