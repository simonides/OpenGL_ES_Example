//
// Created by Simon on 15.04.2016.
//

#ifndef OPENGL_ES_EXAMPLE_APPMAIN_H
#define OPENGL_ES_EXAMPLE_APPMAIN_H


#include <jni.h>
#include <stdint.h>

class Camera;


class AppMain {
public:
    AppMain();
    void Step();
    void Update(float deltaTimeSec);
    void Render();
    void Resize(int w, int h);

private:
    Camera* m_camera;
    uint64_t m_LastFrameNs;

};


#endif //OPENGL_ES_EXAMPLE_APPMAIN_H
