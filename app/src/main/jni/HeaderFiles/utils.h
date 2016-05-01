#pragma once

#include <glm/detail/type_vec.hpp>
#include <glm/detail/type_vec3.hpp>
#include <glm/detail/type_vec2.hpp>
#include <GLES3/gl3.h>


struct TexturedVertex {
	glm::vec3 pos;			// object space position
	glm::vec2 tex;		// texure space texture coordinates

	TexturedVertex() : pos(), tex() {};
	TexturedVertex(glm::vec3 pos, glm::vec2 tex) : pos(pos), tex(tex) {}
};
/*
struct TexturedModel {
	int vertexCount;
	int triangleCount;

	//	GLenum drawType;
	TexturedVertex* vertices;
	GLuint* indices;

	TexturedModel(int vertexCount, int triangleCount)
		: vertexCount(vertexCount)
		, triangleCount(triangleCount)
		//, drawType(GL_TRIANGLES)
		, vertices(new TexturedVertex[vertexCount])
		, indices(new GLuint[triangleCount * 3]) {
	}
	~TexturedModel() {
		delete vertices;
		delete indices;
	}
};
*/
