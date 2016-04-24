#pragma once

/*

struct Vector3f {
	float x, y, z;
	Vector3f() : x(0), y(0), z(0) {}
};
struct Vector2f {
	float x, y;
	Vector2f() : x(0), y(0) {}
};

typedef Vector3f vec3;
typedef Vector2f vec2;
*/

#include <glm/detail/type_vec.hpp>
#include <glm/detail/type_vec3.hpp>
#include <glm/detail/type_vec2.hpp>
#include <GLES3/gl3.h>

// Data for exacly one vertex in an VBO
struct TexturedVertex {
	glm::vec3 pos;			// object space position
	glm::vec2 tex;		// texure space texture coordinates

	TexturedVertex() : pos(), tex() {};
	TexturedVertex(glm::vec3 pos, glm::vec2 tex) : pos(pos), tex(tex) {}
};
/*
// Data for exacly one vertex in an VBO
struct TexturedVertex {
    GLfloat posx, posy, posz;
    GLfloat texx, texy;

    TexturedVertex() : posx(0), posy(0), posz(0), texx(0), texy(0) {};
    TexturedVertex(glm::vec3 pos, glm::vec2 tex) : posx(pos.x), posy(pos.y), posz(pos.z), texx(tex.x), texy(tex.y) {}
};*/


struct TexturedModel {
	int vertexCount;
	int triangleCount;

	//	GLenum drawType;
	TexturedVertex* vertices;
	int* indices;

	TexturedModel(int vertexCount, int triangleCount)
		: vertexCount(vertexCount)
		, triangleCount(triangleCount)
		//, drawType(GL_TRIANGLES)
		, vertices(new TexturedVertex[vertexCount])
		, indices(new int[triangleCount * 3]) {
	}
	~TexturedModel() {
		delete vertices;
		delete indices;
	}
};

