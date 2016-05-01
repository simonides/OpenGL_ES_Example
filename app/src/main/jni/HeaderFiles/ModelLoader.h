#pragma once
#include <ostream>
#include <iostream>
#include "RelativePointer.h"
#include "utils.h"
#include "Helper.h"


template<typename VertexType, typename IndexType = int>
struct Model {
	int vertexCount;
	int indexCount;

	RelativePointer<VertexType> vertices;
	RelativePointer<IndexType> indices;

	size_t size() const {
		return getTotalSize(vertexCount, indexCount);
	}

private:
	Model(int vertexCount, int indexCount)
		: vertexCount(vertexCount)
		, indexCount(indexCount)
		, vertices(sizeof(vertices) + sizeof(indices))
		, indices(sizeof(indices) + vertexCount * sizeof(VertexType)) {
	}

	static uint32_t getTotalSize(int vertexCount, int indexCount) {
		return sizeof(Model) + getTailingSize(vertexCount, indexCount);
	}
	static uint32_t getTailingSize(int vertexCount, int indexCount) {
		return vertexCount * sizeof(VertexType) + indexCount * sizeof(IndexType);
	}
public:
	static Model* newModel(int vertexCount, int indexCount) {
		uint32_t size = Model::getTotalSize(vertexCount, indexCount);
		void* memory = new char[size];

		Model* model = new(memory) Model(vertexCount, indexCount);
		return model;
	}

	static void deleteModel(Model* model) {
		char* memory = reinterpret_cast<char*>(model);
		delete[] memory;
	}

	void store(std::ostream& destination) const {
		destination << size();
		destination.write(reinterpret_cast<const char*>(this), size());
	}

	static Model* load(std::istream& source) {
        ALOGE("%d : %d : %d # %d : %d\n", sizeof(int), sizeof(Model<TexturedVertex>), sizeof(RelativePointer<TexturedVertex>), sizeof(glm::vec3), sizeof(TexturedVertex));
		size_t size;
        source >> size;
        ALOGD("Size: %d", sizeof(int));
        char* memory = new char[size];
        source.read(memory, size);

        Model* model = reinterpret_cast<Model*>(memory);
//        ALOGE("%d / %d\n", sizeof(vertices) + sizeof(indices), sizeof(indices) + model->vertexCount * sizeof(VertexType));
        return model;
	}
};


using TexturedModel = Model<TexturedVertex, int>;

