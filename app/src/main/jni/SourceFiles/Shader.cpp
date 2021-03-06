//
// Created by Simon on 15.04.2016.
//
#include "../HeaderFiles/Shader.h"
#include "../HeaderFiles/Helper.h"

#define POS_ATTRIB 0
#define COLOR_ATTRIB 1

const char* VERTEX_SHADER =
"#version 300 es\n"
"layout(location = 0) in vec3 pos;\n"
"layout(location = 1) in vec3 color;\n"
//
"out vec4 vColor;\n"
//
"uniform mat4 projection;\n"
"uniform mat4 view;\n"
"uniform mat4 model;\n"
//
"void main(){\n"
"   vColor = vec4(color, 1.0f);\n"
"   gl_Position = projection * view * model * vec4(pos, 1.0f);\n"
"}\n";



const char* FRAGMENT_SHADER =
"#version 300 es\n"
//
"precision mediump float;\n"
//
"in vec4 vColor;\n"
//
"out vec4 outColor;\n"
//
"void main() {\n"
"    outColor = vColor;\n"
"}\n";



const char* VERTEX_SHADER_TEX =
                "#version 300 es\n"
                        //
                "layout(location = 0) in vec3 pos;\n"
                "layout(location = 1) in vec2 vertexUV;\n"
                //
                "uniform mat4 projection;\n"
                "uniform mat4 view;\n"
                "uniform mat4 model;\n"
                //
                "out vec2 uv;\n"
                //
                "void main(){\n"
                "   uv = vertexUV;\n"
                "   gl_Position = projection * view * model * vec4(pos, 1.0f);\n"
                "}\n";



const char* FRAGMENT_SHADER_TEX =
        "#version 300 es\n"
//
                "precision mediump float;\n"
//
                "in vec2 uv;\n"
                "uniform sampler2D textureSampler;\n"
//
                "out vec4 outColor;\n"
//
                "void main() {\n"
                "    outColor = texture( textureSampler, uv );\n"
                "}\n";


