#pragma once
#include <istream>
#include "utils.h"

class ObjModelLoader {
	int vertexCount;
	int textureCoordCount;
	int normalCount;
	int triangleCount;

	typedef struct {
		int vertexIndex[3];
		int texCoordIndex[3];
		int normalIndex[3];
	} TriangleType;

	// data loaded from .obj file
	Vector3f* vertices;
	Vector2f* textureCoords;
	Vector3f* normals;
	TriangleType* triangles;

	ObjModelLoader();
	~ObjModelLoader();
	void freeMemory();
	// .obj loading
	void loadCounts(std::istream& source);
	bool loadData(std::istream& source);
	bool loadObj(std::istream& source);
	// converter
	TexturedModel* convertToTexturedModel() const;
public:
	static TexturedModel* loadTexturedModel(std::istream& source);
};

