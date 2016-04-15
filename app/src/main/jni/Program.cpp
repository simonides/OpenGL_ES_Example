//
// Created by Simon on 15.04.2016.
//

#include "Program.h"
#include <assert.h>
#include <glm/gtc/type_ptr.hpp>



GLint program::GetUniformLocation(GLuint
                                  programID,
                                  const GLchar *uniformName)
{
    assert(!uniformName);
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
