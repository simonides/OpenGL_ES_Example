#include "../HeaderFiles/ObjModelLoader.h"
#include <cassert>
#include <vector>
#include <map>
#include <limits>
#include <HeaderFiles/Helper.h>


ObjModelLoader::ObjModelLoader()
	: vertexCount(0)
	, textureCoordCount(0)
	, normalCount(0)
	, triangleCount(0)
	, vertices(nullptr)
	, textureCoords(nullptr)
	, normals(nullptr)
	, triangles(nullptr) {
}

ObjModelLoader::~ObjModelLoader() {
	freeMemory();
}

void ObjModelLoader::freeMemory() {
	delete vertices;
	delete textureCoords;
	delete normals;
	delete triangles;
}

void ObjModelLoader::loadCounts(std::istream& source) {
	vertexCount = 0;
	textureCoordCount = 0;
	normalCount = 0;
	triangleCount = 0;

	char input;
	while(!source.eof()) {
		source.get(input);
		if(input == 'v') {
			source.get(input);
			switch(input) {
				case ' ': vertexCount++; break;
				case 't': textureCoordCount++; break;
				case 'n': normalCount++; break;
			}
		} else if(input == 'f') {
			source.get(input);
			if(input == ' ') {
				triangleCount++;
			}
		}
		source.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
	}
}

bool ObjModelLoader::loadData(std::istream& source) {
	assert(vertices == nullptr && textureCoords == nullptr && normals == nullptr && triangles == nullptr && "Internal storage structures not empty");

	vertices = new glm::vec3[vertexCount];
	textureCoords = new glm::vec2[textureCoordCount];
	normals = new glm::vec3[normalCount];
	triangles = new TriangleType[triangleCount];
	assert(vertices != nullptr);
	assert(textureCoords != nullptr);
	assert(normals != nullptr);
	assert(triangles != nullptr);

	int vertexIndex = 0;
	int texcoordIndex = 0;
	int normalIndex = 0;
	int triangleIndex = 0;

	char input, trash;
	while(!source.eof()) {
		source.get(input);
		if(input == 'v') {
			source.get(input);
			if(input == ' ') {			// vertex
				if(vertexIndex >= vertexCount) {
					goto fail;
				}
				source >> vertices[vertexIndex].x >> vertices[vertexIndex].y >> vertices[vertexIndex].z;
				++vertexIndex;
			} else if(input == 't') {	// texture coords
				if(texcoordIndex >= textureCoordCount) {
					goto fail;
				}
				source >> textureCoords[texcoordIndex].x >> textureCoords[texcoordIndex].y;
				++texcoordIndex;
			} else if(input == 'n') {	// normals 
				if(normalIndex >= normalCount) {
					goto fail;
				}
				source >> normals[normalIndex].x >> normals[normalIndex].y >> normals[normalIndex].z;
				++normalIndex;
			}
		} else if(input == 'f') {		// face
			source.get(input);
			if(input == ' ') {
				if(triangleIndex >= triangleCount) {
					goto fail;
				}
				for(int i = 0; i < 3; ++i) {
					source >> triangles[triangleIndex].vertexIndex[i] >> trash >> triangles[triangleIndex].texCoordIndex[i] >> trash >> triangles[triangleIndex].normalIndex[i];
					// array indices are zero-based, .obj files are one-based:
					--triangles[triangleIndex].vertexIndex[i];
					--triangles[triangleIndex].texCoordIndex[i];
					--triangles[triangleIndex].normalIndex[i];
				}
				++triangleIndex;
			}
		}
		source.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
	}

	assert(vertexIndex == vertexCount);
	assert(texcoordIndex == textureCoordCount);
	assert(normalIndex == normalCount);
	assert(triangleIndex == triangleCount);

#ifdef _DEBUG
	// Validate all indices
	for(int t = 0; t < triangleCount; ++t) {
		for(int v = 0; v < 3; ++v) {
			int vIdx = triangles[t].vertexIndex[v];
			int tIdx = triangles[t].texCoordIndex[v];
			int nIdx = triangles[t].normalIndex[v];
			assert(vIdx >= 0 && vIdx < vertexCount);
			assert(tIdx >= 0 && tIdx < textureCoordCount);
			assert(nIdx >= 0 && nIdx < normalCount);
		}
	}
#endif
	return true;

fail:
	freeMemory();
	vertices = nullptr;
	textureCoords = nullptr;
	normals = nullptr;
	triangles = nullptr;
	return false;
}

bool ObjModelLoader::loadObj(std::istream& source) {
	int streamPos = source.tellg();
	loadCounts(source);
//#ifdef _DEBUG
	ALOGV("Object file content:\n"
		   "\tVertices            %4d\n"
		   "\tTexture coordinates %4d\n"
		   "\tNormals             %4d\n"
		   "\tTriangles           %4d\n", vertexCount, textureCoordCount, normalCount, triangleCount);
//#endif
	source.clear();
	source.seekg(streamPos);
	return loadData(source);
}

TexturedModel* ObjModelLoader::convertToTexturedModel() const {
	std::vector<TexturedVertex> vertexList;		// List of all needed (unique) vertices
	int* indexList;								// List of indices pointing to the vertexList
	std::multimap<int, int> mapping;			// mapping[vertexIndex] = [vertexListIndex1, vertexListIndex2, ...]		(one vertex can have multiple (different) texture coordinates and therefore needs to be duplicated)
	std::multimap<int, int>::iterator it;
	vertexList.reserve(vertexCount);
	indexList = new int[triangleCount * 3];
	assert(indexList != nullptr);

	for(int t = 0; t < triangleCount; ++t) {
		for(int v = 0; v < 3; ++v) {
			int vIdx = triangles[t].vertexIndex[v];
			int texIdx = triangles[t].texCoordIndex[v];

			// check if the required position <-> texture combination has already been stored
			int vertexListIndex = -1;
			auto vRange = mapping.equal_range(vIdx);
			for(it = vRange.first; it != vRange.second; ++it) {	// for each stored vertex: does it have the same texture coordinates?
				int vListIdx = it->second;
				assert(vListIdx >= 0 && vListIdx < vertexList.size());
				if(vertexList[vListIdx].tex.x == textureCoords[texIdx].x
				   && vertexList[vListIdx].tex.y == textureCoords[texIdx].y) {
					vertexListIndex = vListIdx;
					break;
				}
			}
			if(vertexListIndex == -1) {	// Add new vertex
                glm::vec3 pos = vertices[vIdx];
                glm::vec2 tex = textureCoords[texIdx];
				vertexList.push_back(TexturedVertex(pos, tex));
				vertexListIndex = vertexList.size() - 1;
				mapping.insert(std::make_pair(vIdx, vertexListIndex));
			}


//            glm::vec3 pos = vertices[vIdx];
//            glm::vec2 tex = textureCoords[texIdx];
//            vertexList.push_back(TexturedVertex(pos, tex));
//            vertexListIndex = vertexList.size() - 1;
//            if(vertexListIndex != vIdx) {
//                ALOGE("FAIL!");
//                return nullptr;
//            }

			assert(vertexListIndex >= 0 && vertexListIndex < vertexList.size());
			indexList[t * 3 + v] = vertexListIndex;
		}
	}

	// Convert into target format:
	TexturedModel* model = new TexturedModel(vertexList.size(), triangleCount);
	int idx = 0;
	for(TexturedVertex& v : vertexList) {
		model->vertices[idx] = v;
		++idx;
	}

	memcpy(model->indices, indexList, (triangleCount * 3) * sizeof(*indexList));
	assert(model->indices[triangleCount * 3 - 1] == indexList[triangleCount * 3 - 1]);
	delete[] indexList;
	return model;
}

TexturedModel* ObjModelLoader::loadTexturedModel(std::istream& source) {
	ObjModelLoader modelLoader;

	bool success = modelLoader.loadObj(source);
	assert(success && "Failed to load model");
	if(!success) {
		return nullptr;
	}

	ALOGV("Conveting model data...");
	TexturedModel* model = modelLoader.convertToTexturedModel();
//#ifdef _DEBUG
	if(model != nullptr) {
		ALOGV("Model:\n"
			   "\tVertices: %4d\n"
			   "\tIndices:  %4d (%d triangles)\n", model->vertexCount, model->triangleCount * 3, model->triangleCount);
	}
//#endif
	return model;
}
