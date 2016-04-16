#include <glm/glm.hpp>
#include <string>
#include <sstream>

#include "gles3jni.h"
#include "Helper.h"
#include <EGL/egl.h>
#include <glm/gtc/matrix_transform.hpp>

#include "Renderer.h"


class RendererES3: public Renderer {
public:
    RendererES3();
    virtual ~RendererES3();
    bool init();

private:
    enum {VB_INSTANCE, VB_SCALEROT, VB_OFFSET, VB_COUNT};

    virtual float* mapOffsetBuf();
    virtual void unmapOffsetBuf();
    virtual float* mapTransformBuf();
    virtual void unmapTransformBuf();
    virtual void draw(unsigned int numInstances);

//    const EGLContext mEglContext;
//    GLuint mProgram;
//    GLuint mVB[VB_COUNT];
//    GLuint mVBState;
};

Renderer* createES3Renderer() {
    RendererES3* renderer = new RendererES3;
    if (!renderer->init()) {
        delete renderer;
        return NULL;
    }
    return renderer;
}

RendererES3::RendererES3()
//:   mEglContext(eglGetCurrentContext()),
//    mProgram(0),
//    mVBState(0)
{
//    for (int i = 0; i < VB_COUNT; i++)
//        mVB[i] = 0;
}

bool RendererES3::init() {
    return  true;
//    glm::vec2 vec2(2.f,2.f);
//    std::ostringstream os ;
//    os << "vector: " << ToString(vec2.x) << " ..........." ;
//    std::string test = os.str();
//    ALOGV(test.c_str());
//
//
//    mProgram = createProgram(VERTEX_SHADER, FRAGMENT_SHADER);
//    if (!mProgram)
//        return false;
//
//    glGenBuffers(VB_COUNT, mVB);
//    glBindBuffer(GL_ARRAY_BUFFER, mVB[VB_INSTANCE]);
//    glBufferData(GL_ARRAY_BUFFER, sizeof(QUAD), &QUAD[0], GL_STATIC_DRAW);
//    glBindBuffer(GL_ARRAY_BUFFER, mVB[VB_SCALEROT]);
//    glBufferData(GL_ARRAY_BUFFER, MAX_INSTANCES * 4 * sizeof(float), NULL, GL_DYNAMIC_DRAW);
//    glBindBuffer(GL_ARRAY_BUFFER, mVB[VB_OFFSET]);
//    glBufferData(GL_ARRAY_BUFFER, MAX_INSTANCES * 2 * sizeof(float), NULL, GL_STATIC_DRAW);
//
//    glGenVertexArrays(1, &mVBState);
//    glBindVertexArray(mVBState);
//
//    glBindBuffer(GL_ARRAY_BUFFER, mVB[VB_INSTANCE]);
//    glVertexAttribPointer(POS_ATTRIB, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const GLvoid*)offsetof(Vertex, pos));
//    glVertexAttribPointer(COLOR_ATTRIB, 4, GL_UNSIGNED_BYTE, GL_TRUE, sizeof(Vertex), (const GLvoid*)offsetof(Vertex, rgba));
//    glEnableVertexAttribArray(POS_ATTRIB);
//    glEnableVertexAttribArray(COLOR_ATTRIB);
//
//    glBindBuffer(GL_ARRAY_BUFFER, mVB[VB_SCALEROT]);
//    glVertexAttribPointer(SCALEROT_ATTRIB, 4, GL_FLOAT, GL_FALSE, 4*sizeof(float), 0);
//    glEnableVertexAttribArray(SCALEROT_ATTRIB);
//    glVertexAttribDivisor(SCALEROT_ATTRIB, 1);
//
//    glBindBuffer(GL_ARRAY_BUFFER, mVB[VB_OFFSET]);
//    glVertexAttribPointer(OFFSET_ATTRIB, 2, GL_FLOAT, GL_FALSE, 2*sizeof(float), 0);
//    glEnableVertexAttribArray(OFFSET_ATTRIB);
//    glVertexAttribDivisor(OFFSET_ATTRIB, 1);
//
//    ALOGV("Using OpenGL ES 3.0 renderer");
//    return true;
}

RendererES3::~RendererES3() {
    /* The destructor may be called after the context has already been
     * destroyed, in which case our objects have already been destroyed.
     *
     * If the context exists, it must be current. This only happens when we're
     * cleaning up after a failed init().
     */
//    if (eglGetCurrentContext() != mEglContext)
//        return;
//    glDeleteVertexArrays(1, &mVBState);
//    glDeleteBuffers(VB_COUNT, mVB);
//    glDeleteProgram(mProgram);
}

float* RendererES3::mapOffsetBuf() {
    return nullptr;
//    glBindBuffer(GL_ARRAY_BUFFER, mVB[VB_OFFSET]);
//    return (float*)glMapBufferRange(GL_ARRAY_BUFFER,
//            0, MAX_INSTANCES * 2*sizeof(float),
//            GL_MAP_WRITE_BIT | GL_MAP_INVALIDATE_BUFFER_BIT);
}

void RendererES3::unmapOffsetBuf() {
//    glUnmapBuffer(GL_ARRAY_BUFFER);
}

float* RendererES3::mapTransformBuf() {
//    glBindBuffer(GL_ARRAY_BUFFER, mVB[VB_SCALEROT]);
//    return (float*)glMapBufferRange(GL_ARRAY_BUFFER,
//            0, MAX_INSTANCES * 4*sizeof(float),
//            GL_MAP_WRITE_BIT | GL_MAP_INVALIDATE_BUFFER_BIT);
}

void RendererES3::unmapTransformBuf() {
//    glUnmapBuffer(GL_ARRAY_BUFFER);
}

void RendererES3::draw(unsigned int numInstances) {
//   float horizontalAngle= 0.0f;
//   float verticalAngle=(0.0f);
//   glm::vec3 position(0.0f, 0.0f, 1.0f);
//   float fieldOfView=(50.0f);
//   float nearPlane=(1.51f);
//   float farPlane=(500.0f);
//   float viewportAspectRatio=(4.0f / 3.0f);
//    glm::mat4 orientation;
//    orientation = glm::rotate(orientation, glm::radians(verticalAngle), glm::vec3(1, 0, 0));
//    orientation = glm::rotate(orientation, glm::radians(horizontalAngle), glm::vec3(0, 1, 0));
//
//    glm::vec4 forward_4 = glm::inverse(orientation) * glm::vec4(0, 0, -1, 1);
//    glm::vec3 forward(forward_4);
//
//    glm::mat4 viewMatrix = glm::lookAt(position,
//                             position + forward,
//                             glm::vec3(0, 1, 0));
//
//    glm::mat4 projection =  glm::perspective(glm::radians(fieldOfView), viewportAspectRatio,
//                                             nearPlane, farPlane);
//    shaders->setUniform("projection",projection);
//    shaders->setUniform("view", viewMatrix);
//    shaders->setUniform("model", inst.transform);

//    glUseProgram(mProgram);
//    glBindVertexArray(mVBState);
//    glDrawArraysInstanced(GL_TRIANGLE_STRIP, 0, 4, numInstances);
}
