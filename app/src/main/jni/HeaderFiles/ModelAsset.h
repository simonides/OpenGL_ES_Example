//
// Created by Jakob on 24.04.2016.
//

#ifndef OPENGL_ES_EXAMPLE_MODELASSET_H
#define OPENGL_ES_EXAMPLE_MODELASSET_H


#include <GLES3/gl3.h>
#include "utils.h"

class ModelAsset {
    GLuint vao;
    GLuint vbo;
    GLuint vio;

public:
    ModelAsset(GLuint programId, TexturedModel& model);
    ~ModelAsset();

    void bind();
};


#endif //OPENGL_ES_EXAMPLE_MODELASSET_H
