//
// Created by Simon on 15.04.2016.
//

#include <time.h>
#include "AppMain.h"
#include "Camera.h"
#include "Program.h"

//#include <GLES3/gl3.h>

using namespace program;
using namespace ogl;


AppMain::AppMain() :
        m_camera(new Camera())
{


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
    }
    m_LastFrameNs = nowNs;
}

void AppMain::Update(float deltaTimeSec) {

}

void AppMain::Render() {
    glClearColor(0.2f, 0.2f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    //draw calls
    checkGlError("Renderer::render");

}


