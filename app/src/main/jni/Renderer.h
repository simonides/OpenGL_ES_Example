//
// Created by Simon on 15.04.2016.
//

#ifndef OPENGL_ES_EXAMPLE_RENDERER_H
#define OPENGL_ES_EXAMPLE_RENDERER_H

class AppMain;

class Renderer {
public:
    virtual ~Renderer();
    void resize(int w, int h);
    void render();

protected:
    Renderer();

    // return a pointer to a buffer of MAX_INSTANCES * sizeof(vec2).
    // the buffer is filled with per-instance offsets, then unmapped.
    virtual float* mapOffsetBuf() = 0;
    virtual void unmapOffsetBuf() = 0;
    // return a pointer to a buffer of MAX_INSTANCES * sizeof(vec4).
    // the buffer is filled with per-instance scale and rotation transforms.
    virtual float* mapTransformBuf() = 0;
    virtual void unmapTransformBuf() = 0;

    virtual void draw(unsigned int numInstances) = 0;

private:
    void calcSceneParams(unsigned int w, unsigned int h, float* offsets);
    void step();

    unsigned int mNumInstances;
//    float mScale[2];
//    float mAngularVelocity[MAX_INSTANCES];
//    uint64_t mLastFrameNs;
//    float mAngles[MAX_INSTANCES];


    AppMain* m_appMain;


};


#endif //OPENGL_ES_EXAMPLE_RENDERER_H
