//
// Created by Simon on 15.04.2016.
//

#include "../HeaderFiles/ObjModelLoader.h"
#include <time.h>
#include <fstream>
#include <android/asset_manager.h>
#include "../HeaderFiles/AppMain.h"
#include "../HeaderFiles/Camera.h"
#include "../HeaderFiles/Program.h"
#include "../HeaderFiles/Shader.h"
#include "../HeaderFiles/Vertices.h"


using namespace program;
using namespace ogl;




AppMain::AppMain(AAssetManager *assetManager)
        : m_eglContext(eglGetCurrentContext())
        , m_assetManager(assetManager)
        , model(nullptr)
        , modelAsset(nullptr)
        , degrees(0.f)
        , degreesY(0.f)
        , m_LastFrameNs(0)
        , m_camera(new Camera())
        , bird1Pos(glm::vec3(-1.5, 0, 0))
        , bird2Pos(glm::vec3(0, 0, 0))
        , bird3Pos(glm::vec3(1.5, 0, 0))
        , bird1Mat(glm::mat4())
        , bird2Mat(glm::mat4())
        , bird3Mat(glm::mat4()) {
    birdMovementProgress[0] = 0;
    birdMovementProgress[1] = 0;
    birdMovementProgress[2] = 0;

    Init();

}


void AppMain::Init() {
    ALOGV("Initializing App...");

    glEnable(GL_DEPTH_TEST);
    glClearDepthf(1.0);
    glDepthFunc(GL_LESS);
    glClearColor(0.5f, 0.5f, 0.9f, 1.0f);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_CULL_FACE);
    glFrontFace(GL_CCW);

    m_programID = program::createProgram(VERTEX_SHADER_TEX, FRAGMENT_SHADER_TEX);


    uint64_t diff = 0;
    for(int i=0; i<100; ++i)
    {
        struct timespec time;
        uint64_t start, end;
        clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &time);
        start = time.tv_sec * 1000000000ull + time.tv_nsec;

        model = loadModel("LowPolyFighter.obj", false);

        clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &time);
        end = time.tv_sec * 1000000000ull + time.tv_nsec;
        diff += end - start;
    }
    ALOGE("Obj model format:  %fms", (float)(diff/1000)/1000 / 100);


    diff = 0;
    for(int i=0; i<100; ++i)
    {
        struct timespec time;
        uint64_t start, end;
        clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &time);
        start = time.tv_sec * 1000000000ull + time.tv_nsec;

        loadModel("LowPolyFighter.bin", true);

        clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &time);
        end = time.tv_sec * 1000000000ull + time.tv_nsec;
        diff += end - start;
    }
    ALOGE("Custom model format:  %fms", (float)(diff/1000/1000) / 100);



    // model = loadModel("girl.obj");
//    model = loadModel("LowPolyFighter.obj", false);
    model = loadModel("LowPolyFighter.bin", true);
    ALOGV("Model loaded");

    ALOGE("Model:\n"
                   "\tVertices: %4d\n"
                   "\tIndices:  %4d (%d triangles)\n", model->vertexCount, model->indexCount, model->indexCount / 3);


    /*for(int t = 0; t < model->indexCount/3; ++t) {
        for(int v = 0; v < 3; ++v) {
            int vIdx = model->indices[t * 3 + v];
            glm::vec3 pos = model->vertices[vIdx].pos;
            glm::vec2 tex = model->vertices[vIdx].tex;
            ALOGE(" (%3d) [%5.2f / %5.2f / %5.2f]  [%5.2f/%5.2f]\n", vIdx, pos.x, pos.y, pos.z, tex.x, tex.y);
        }
        ALOGE("--------------------------------------\n");
    }*/


    modelAsset = new ModelAsset(m_programID, *model);
    ALOGD("Model Asset ready");

    glGenVertexArrays(1, &vao);
    glGenBuffers(1, &vbo);
    glGenBuffers(1, &vio);

    glBindVertexArray(vao);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, vertexCount * sizeOfVertex, cubeColoredVertex, GL_STATIC_DRAW);

    auto posID = program::Attrib(m_programID, "pos");
    glEnableVertexAttribArray(posID);
    glVertexAttribPointer(posID, 3, GL_FLOAT, GL_FALSE, sizeOfVertex, (void *) 0);

    auto uvID = program::Attrib(m_programID, "vertexUV");
    glEnableVertexAttribArray(uvID);
    glVertexAttribPointer(uvID, 2, GL_FLOAT, GL_FALSE, sizeOfVertex, reinterpret_cast<const GLvoid*>(3 * sizeof(GLfloat)));


//    auto colorID = program::Attrib(m_programID, "color");
//    glEnableVertexAttribArray(colorID);
//    glVertexAttribPointer(colorID, 3, GL_FLOAT, GL_FALSE, sizeOfVertex,
//                          reinterpret_cast<const GLvoid *>(5 * sizeof(GLfloat)));

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vio);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, triangleCount * sizeOfIndexData, cubeIndices,
                 GL_STATIC_DRAW);

    glBindVertexArray(0);

    glUseProgram(m_programID);

    //***************LOAD ASSETS************************************************************
    assert(m_assetManager != nullptr);
    m_cubeTex = texture::LoadTextureFromAssetManager(m_assetManager, "Mountain.ktx");
    m_backgroundTex = texture::LoadTextureFromAssetManager(m_assetManager, "background.ktx");
    m_birdTex = texture::LoadTextureFromAssetManager(m_assetManager, "birds.ktx");
    m_fighterTex = texture::LoadTextureFromAssetManager(m_assetManager, "Fighter.ktx");

    //***********************************************************************************************
    ALOGV("Initializing App finished.");
}


void AppMain::Resize(int w, int h) {
    ALOGD("Resize called: %d, %d", w, h);
    m_camera->setViewportAspectRatio((float) w / (float) h);
    glViewport(0, 0, w, h);
}

void AppMain::Step() {
    timespec now;
    clock_gettime(CLOCK_MONOTONIC, &now);
    uint64_t nowNs = now.tv_sec * 1000000000ull + now.tv_nsec;

    if (m_LastFrameNs > 0) {
        float dt = float(nowNs - m_LastFrameNs) * 0.000000001f;
        dt = clamp(dt, 0.0001, 0.5);
//        ALOGD("Update & Render @ %f fps", 1.f/dt);
        Update(dt);
        Render();
    }

    m_LastFrameNs = nowNs;
}


void AppMain::Update(float deltaTimeSec) {
    const GLfloat degreesPerSecond = 60.0f;
    const GLfloat degreesPerSecondY = 20.0f;
    degrees += deltaTimeSec * degreesPerSecond;
    degreesY += deltaTimeSec * degreesPerSecondY;
//    ALOGD("degrees: (delta time = %f)  %f / %f" , deltaTimeSec, degrees, degreesY);
    if (degrees >= 360.0f) {
        degrees -= ((int) degrees / 360) * 360.f;
    }
    if (degreesY >= 360.0f) {
        degreesY -= ((int) degreesY / 360) * 360.f;
    }

    transform = glm::rotate(glm::mat4(), glm::radians(degrees), glm::vec3(0, 1, 0));
    transform = glm::rotate(transform, glm::radians(degreesY), glm::vec3(1, 0, 0));

    for(int i=0; i<3; ++i) {
        birdMovementProgress[i] += deltaTimeSec * (1.5 + i * 0.9);
        if (birdMovementProgress[i] >= M_PI) {
            birdMovementProgress[i] -= ((int) birdMovementProgress[i] / M_PI) * M_PI;
        }
    }

    bird1Pos.y = sinf(birdMovementProgress[0]) * 3 - 3.8;
    bird2Pos.y = sinf(birdMovementProgress[1]) * 3 - 3.8;
    bird3Pos.y = sinf(birdMovementProgress[2]) * 3 - 3.8;

    bird1Mat = glm::translate(glm::mat4(), bird1Pos) * glm::scale(glm::mat4(), glm::vec3(0.4f,0.4f,0.4f));
    bird2Mat = glm::translate(glm::mat4(), bird2Pos) * glm::scale(glm::mat4(), glm::vec3(0.4f,0.4f,0.4f));
    bird3Mat = glm::translate(glm::mat4(), bird3Pos) * glm::scale(glm::mat4(), glm::vec3(0.4f,0.4f,0.4f));
}

void AppMain::Render() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glm::mat4 viewMatrix = glm::lookAt(m_camera->position(),
                                       m_camera->position() + m_camera->forward(),
                                       m_camera->up());


    glBindVertexArray(vao);
    GLuint TextureID  = glGetUniformLocation(m_programID, "textureSampler");
    glUniform1i(TextureID, 0);  // set to texture unit 0

    // draw background
    SetUniform(m_programID, "model", glm::mat4(1.0f), false);
    SetUniform(m_programID, "view", glm::mat4(1.0f), false);
    SetUniform(m_programID, "projection", glm::mat4(1.0f), false);

    m_backgroundTex.BindTexture();
    glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, reinterpret_cast<const GLvoid *>(36 * sizeof(GLuint)));
    glClear(GL_DEPTH_BUFFER_BIT);   // the background must not occlude any other objects



    SetUniform(m_programID, "model", transform, false);
    SetUniform(m_programID, "view", viewMatrix, false);
    SetUniform(m_programID, "projection", m_camera->projection(), false);

    //draw cube
//    m_cubeTex.BindTexture();
//    glDrawElements(GL_TRIANGLES, cubeTriangleCount * 3, GL_UNSIGNED_INT, (void *) 0);

    // draw Birds
    m_birdTex.BindTexture();

    SetUniform(m_programID, "model", bird1Mat, false);
    glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, reinterpret_cast<const GLvoid *>(39 * sizeof(GLuint)));


    SetUniform(m_programID, "model", bird2Mat, false);
    glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, reinterpret_cast<const GLvoid *>(42 * sizeof(GLuint)));

    SetUniform(m_programID, "model", bird3Mat, false);
    glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, reinterpret_cast<const GLvoid *>(45 * sizeof(GLuint)));
    glClear(GL_DEPTH_BUFFER_BIT);   // the birds must not occlude any other objects

    //draw model
    modelAsset->bind();
    SetUniform(m_programID, "model", transform, false);
    SetUniform(m_programID, "view", viewMatrix, false);
    SetUniform(m_programID, "projection", m_camera->projection(), false);

    m_fighterTex.BindTexture();
    glDrawElements(GL_TRIANGLES, model->indexCount, GL_UNSIGNED_INT, (void *) 0);

    glBindVertexArray(0);

    checkGlError("Renderer::render");
}


AppMain::~AppMain() {
    ALOGE("Killing AppMain....");
    glDeleteBuffers(1, &vao);
    glDeleteBuffers(1, &vbo);
    glDeleteBuffers(1, &vio);

    glDeleteProgram(m_programID);

    delete modelAsset;
    TexturedModel ::deleteModel(model);
    delete m_camera;
}

TexturedModel* AppMain::loadModel(const char* path, bool customFormat) {
    AAsset *modelAsset = nullptr;
    modelAsset = AAssetManager_open(m_assetManager, path, AASSET_MODE_BUFFER);
    if(modelAsset == nullptr) {
        ALOGE("Failed to open model asset");
        assert(false);
    }

    const char* modelData = static_cast<const char*>(AAsset_getBuffer(modelAsset));
    int length = AAsset_getLength(modelAsset);


    TexturedModel* model;
    if(customFormat) {
        model = TexturedModel::load(modelData, length);
    } else {
        std::istringstream source(modelData);
        model = ObjModelLoader::loadTexturedModel(source);
    }

    return model;
}
