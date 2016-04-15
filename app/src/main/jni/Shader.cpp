//
// Created by Simon on 15.04.2016.
//
#include "Shader.h"
#include "Helper.h"

#define POS_ATTRIB 0
#define COLOR_ATTRIB 1

const char* VERTEX_SHADER =
"#version 300 es\n"
//
"layout(location=" STRV(POS_ATTRIB) ") in vec3 pos;\n"
"layout(location=" STRV(COLOR_ATTRIB) ") in vec4 color;\n"
//
"uniform mat4 projection;\n"
"uniform mat4 view;\n"
"uniform mat4 model;\n"
//
"out vec4 vColor;\n"
//
"void main() {\n"
"   mat4 MVP = model * view * projection;\n"
"   gl_Position = MVP * vec4(pos, 1.0f);\n"
"   vColor = color;\n"
"}\n";



const char* FRAGMENT_SHADER =
"#version 300 es\n"
//
"precision mediump float;\n"
"in vec4 vColor;\n"
"out vec4 outColor;\n"
//
"void main() {\n"
"    outColor = vColor;\n"
"}\n";


