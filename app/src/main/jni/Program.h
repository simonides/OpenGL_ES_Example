//
// Created by Simon on 15.04.2016.
//

#ifndef OPENGL_ES_EXAMPLE_PROGRAM_H
#define OPENGL_ES_EXAMPLE_PROGRAM_H

#include <GLES3/gl3.h>
#include <glm/detail/type_mat.hpp>

namespace program {
    GLint GetUniformLocation(GLuint programID,
                             const GLchar* uniformName);

    void SetUniform(GLuint programID,
                    const GLchar* name,
                    const glm::mat4& m,
                    GLboolean transpose);

    bool isInUse(GLuint programID);

    GLuint createShader(GLenum shaderType, const char* src);

    GLuint createProgram(const char* vtxSrc, const char* fragSrc);

    void printGlString(const char* name, GLenum s);

    GLint Attrib(GLuint programID, const GLchar *attribName);
}

namespace ogl{
    bool checkGlError(const char* funcName);
}


#endif //OPENGL_ES_EXAMPLE_PROGRAM_H
