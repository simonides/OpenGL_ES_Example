#pragma once
#include <ostream>
#include <iostream>
#include "RelativePointer.h"
#include "utils.h"
#include "Helper.h"


template<typename VertexType, typename IndexType = int>
struct Model {
    uint32_t vertexCount;
    uint32_t indexCount;

    RelativePointer<VertexType> vertices;
    RelativePointer<IndexType> indices;

    size_t size() const {
        return getTotalSize(vertexCount, indexCount);
    }

private:
    Model(uint32_t vertexCount, uint32_t indexCount)
            : vertexCount(vertexCount)
            , indexCount(indexCount)
            , vertices(sizeof(vertices) + sizeof(indices))
            , indices(sizeof(indices) + vertexCount * sizeof(VertexType)) {
    }

    static uint32_t getTotalSize(uint32_t vertexCount, uint32_t indexCount) {
        return sizeof(Model) + getTailingSize(vertexCount, indexCount);
    }
    static uint32_t getTailingSize(uint32_t vertexCount, uint32_t indexCount) {
        return vertexCount * sizeof(VertexType) + indexCount * sizeof(IndexType);
    }
public:
    static Model* newModel(uint32_t vertexCount, uint32_t indexCount) {
        uint32_t size = getTotalSize(vertexCount, indexCount);
        void* memory = new char[size];

        Model* model = new(memory) Model(vertexCount, indexCount);
        return model;
    }

    static void deleteModel(Model* model) {
        char* memory = reinterpret_cast<char*>(model);
        delete[] memory;
    }

    void store(std::ostream& destination) const {
        destination.write(reinterpret_cast<const char*>(this), size());
    }

	/*static Model* load(std::istream& source) {
        ALOGE("%d : %d : %d # %d : %d\n", sizeof(int), sizeof(Model<TexturedVertex>), sizeof(RelativePointer<TexturedVertex>), sizeof(glm::vec3), sizeof(TexturedVertex));
		size_t size;
        source >> size;
        ALOGD("Size: %d", size);
        char* memory = new char[size];
        source.read(memory, size);


        for(int i=0; i<100; ++i) {
            ALOGD("%d) %x ", i, memory[i]);
        }


        Model* model = reinterpret_cast<Model*>(memory);
//        ALOGE("%d / %d\n", sizeof(vertices) + sizeof(indices), sizeof(indices) + model->vertexCount * sizeof(VertexType));
        return model;
	}*/

    static Model* load(const char* buffer, int size) {
        char* memory = new char[size];
        memcpy(memory, buffer, size);
        Model* model = reinterpret_cast<Model*>(memory);
        assert(model->vertexCount > 0 && model->indexCount > 0);
        assert(model->size() == size);
        return model;
    }


//    static Model* load(const char* buffer, int length) {
//        size_t size = length;
//
//        char* memory = new char[size];
//        memcpy(memory, buffer, size);
//
//        for(int i=0; i<100; ++i) {
//            ALOGD("%d) %x ", i, memory[i]);
//        }
//
//        Model* model = reinterpret_cast<Model*>(memory);
////        ALOGE("%d / %d\n", sizeof(vertices) + sizeof(indices), sizeof(indices) + model->vertexCount * sizeof(VertexType));
//        return model;
//    }
};


using TexturedModel = Model<TexturedVertex, int>;

