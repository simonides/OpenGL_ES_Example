//
// Created by Jakob on 24.04.2016.
//

#include <HeaderFiles/Program.h>
#include "../HeaderFiles/ModelAsset.h"

ModelAsset::ModelAsset(GLuint programId, TexturedModel& model){
    glGenVertexArrays(1, &vao);
    glGenBuffers(1, &vbo);
    glGenBuffers(1, &vio);

    glBindVertexArray(vao);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, model.vertexCount * sizeof(TexturedVertex), &(model.vertices[0]), GL_STATIC_DRAW);

    GLint posID = program::Attrib(programId, "pos");
    glEnableVertexAttribArray(posID);
    glVertexAttribPointer(posID, 3, GL_FLOAT, GL_FALSE, sizeof(TexturedVertex), (void *) 0);

    GLint uvID = program::Attrib(programId, "vertexUV");
    glEnableVertexAttribArray(uvID);
    glVertexAttribPointer(uvID, 2, GL_FLOAT, GL_FALSE, sizeof(TexturedVertex), reinterpret_cast<const GLvoid*>(3 * sizeof(GLfloat)));

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vio);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, model.indexCount * sizeof(GLuint), &(model.indices[0]), GL_STATIC_DRAW);

    glBindVertexArray(0);
}

ModelAsset::~ModelAsset() {
    glDeleteBuffers(1, &vao);
    glDeleteBuffers(1, &vbo);
    glDeleteBuffers(1, &vio);
}

void ModelAsset::bind() {
    glBindVertexArray(vao);
}
