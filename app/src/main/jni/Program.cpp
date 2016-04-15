//
// Created by Simon on 15.04.2016.
//

#include "Program.h"
#include "Helper.h"

#include <assert.h>
#include <glm/gtc/type_ptr.hpp>

using namespace ogl;
using namespace program;

GLint program::GetUniformLocation(GLuint
                                  programID,
                                  const GLchar *uniformName)
{
    assert(uniformName != nullptr);
    GLint uniform = glGetUniformLocation(programID, uniformName);
    assert(uniform != -1);
    return uniform;
}


void program::SetUniform(GLuint programID,
                           const GLchar *name,
                           const glm::mat4 &m,
                           GLboolean transpose)
{
    assert(isInUse(programID));
    glUniformMatrix4fv(GetUniformLocation(programID, name), 1, transpose, glm::value_ptr(m));
}

bool program::isInUse(GLuint programID)
{
    GLint currentProgram = 0;
    glGetIntegerv(GL_CURRENT_PROGRAM, &currentProgram);
    return (currentProgram == static_cast<GLint>(programID));
}

GLuint program::createShader(GLenum shaderType,
                                const char *src)
{
    GLuint shader = glCreateShader(shaderType);
    if (!shader) {
        checkGlError("glCreateShader");
        return 0;
    }
    glShaderSource(shader, 1, &src, NULL);

    GLint compiled = GL_FALSE;
    glCompileShader(shader);
    glGetShaderiv(shader, GL_COMPILE_STATUS, &compiled);
    if (!compiled) {
        GLint infoLogLen = 0;
        glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &infoLogLen);
        if (infoLogLen > 0) {
            GLchar* infoLog = (GLchar*)malloc(infoLogLen);
            if (infoLog) {
                glGetShaderInfoLog(shader, infoLogLen, nullptr, infoLog);
                ALOGE("Could not compile %s shader:\n%s\n",
                      shaderType == GL_VERTEX_SHADER ? "vertex" : "fragment",
                      infoLog);
                free(infoLog);
            }
        }
        glDeleteShader(shader);
        return 0;
    }

    return shader;
}

GLuint program::createProgram(const char *vtxSrc, const char *fragSrc) {
    GLuint vtxShader = 0;
    GLuint fragShader = 0;
    GLuint program = 0;
    GLint linked = GL_FALSE;

    vtxShader = createShader(GL_VERTEX_SHADER, vtxSrc);
    if (!vtxShader)
        goto exit;

    fragShader = createShader(GL_FRAGMENT_SHADER, fragSrc);
    if (!fragShader)
        goto exit;

    program = glCreateProgram();
    if (!program) {
        checkGlError("glCreateProgram");
        goto exit;
    }
    glAttachShader(program, vtxShader);
    glAttachShader(program, fragShader);

    glLinkProgram(program);
    glGetProgramiv(program, GL_LINK_STATUS, &linked);
    if (!linked) {
        ALOGE("Could not link program");
        GLint infoLogLen = 0;
        glGetProgramiv(program, GL_INFO_LOG_LENGTH, &infoLogLen);
        if (infoLogLen) {
            GLchar* infoLog = (GLchar*)malloc(infoLogLen);
            if (infoLog) {
                glGetProgramInfoLog(program, infoLogLen, NULL, infoLog);
                ALOGE("Could not link program:\n%s\n", infoLog);
                free(infoLog);
            }
        }
        glDeleteProgram(program);
        program = 0;
    }

    exit:
    glDeleteShader(vtxShader);
    glDeleteShader(fragShader);
    return program;
}

bool ::ogl::checkGlError(const char *funcName) {
    GLint err;
    err = glGetError();
    if (err != GL_NO_ERROR) {
        ALOGE("GL error after %s(): 0x%08x\n", funcName, err);
        return true;
    }
    return false;
}

void ::program::printGlString(const char *name, GLenum s) {
    const char* v = (const char*)glGetString(s);
    ALOGV("GL %s: %s\n", name, v);
}

GLint program::Attrib(GLuint programID, const GLchar *attribName) {
    assert(attribName != nullptr);
    GLint attrib = glGetAttribLocation(programID, attribName);
    assert(attrib != -1);
    return attrib;
}
