#pragma once



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


// Data for exacly one vertex in an VBO
struct TexturedVertex {
	vec3 pos;			// object space position
	vec2 tex;		// texure space texture coordinates

	TexturedVertex() : pos(), tex() {};
	TexturedVertex(vec3 pos, vec2 tex) : pos(pos), tex(tex) {}
};


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

