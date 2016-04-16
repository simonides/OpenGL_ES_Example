//
// Created by Paradies on 15.04.2016.
//

#include "Renderer.h"


Renderer::Renderer()
//        :   mNumInstances(0),
//            mLastFrameNs(0)
{
//    memset(mScale, 0, sizeof(mScale));
//    memset(mAngularVelocity, 0, sizeof(mAngularVelocity));
//    memset(mAngles, 0, sizeof(mAngles));
}

Renderer::~Renderer() {
}

void Renderer::resize(int w, int h) {
//
//    float* offsets = mapOffsetBuf();
//    calcSceneParams(w, h, offsets);
//    unmapOffsetBuf();
//
//    for (unsigned int i = 0; i < mNumInstances; i++) {
//        mAngles[i] = drand48() * TWO_PI;
//        mAngularVelocity[i] = MAX_ROT_SPEED * (2.0*drand48() - 1.0);
//    }
//
//    mLastFrameNs = 0;
//
//    glViewport(0, 0, w, h);
}



void Renderer::step() {
//    timespec now;
//    clock_gettime(CLOCK_MONOTONIC, &now);
//    uint64_t nowNs = now.tv_sec*1000*1000*1000 + now.tv_nsec;
//
//    if (mLastFrameNs > 0)
//        float dt = float(nowNs - mLastFrameNs) / 1000.f / 1000.f / 1000.f;
//
//    }
//
//    mLastFrameNs = nowNs;
}

void Renderer::render() {
    step();


    draw(mNumInstances);

}