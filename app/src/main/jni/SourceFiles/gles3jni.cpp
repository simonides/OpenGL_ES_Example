#include <jni.h>
#include <stdlib.h>
#include <android/asset_manager.h>
#include <android/asset_manager_jni.h>
//#include <time.h>
//#include <glm/gtc/type_ptr.hpp>

#include "../HeaderFiles/Helper.h"
#include "../HeaderFiles/gles3jni.h"
#include "../HeaderFiles/Program.h"
#include "../HeaderFiles/AppMain.h"

using namespace program;

static AppMain* g_appMain = nullptr;
static AAssetManager* g_assetManager = nullptr;

extern "C" {
    JNIEXPORT void JNICALL Java_com_android_gles3jni_GLES3JNILib_init(JNIEnv *env, jclass type, jobject assetManager);
    JNIEXPORT void JNICALL Java_com_android_gles3jni_GLES3JNILib_resize(JNIEnv* env, jobject obj, jint width, jint height);
    JNIEXPORT void JNICALL Java_com_android_gles3jni_GLES3JNILib_step(JNIEnv* env, jobject obj);
};



// init function that creates the appmain instance
JNIEXPORT void JNICALL
Java_com_android_gles3jni_GLES3JNILib_init(JNIEnv *env, jclass type, jobject assetManager) {

    g_assetManager = AAssetManager_fromJava(env, assetManager);
    assert(g_assetManager != nullptr);

    if (g_appMain) {
        delete g_appMain;
        g_appMain = nullptr;
    }

    printGlString("Version", GL_VERSION);
    printGlString("Vendor", GL_VENDOR);
    printGlString("Renderer", GL_RENDERER);
    printGlString("Extensions", GL_EXTENSIONS);

    const char* versionStr = (const char*)glGetString(GL_VERSION);
    if (strstr(versionStr, "OpenGL ES 3.")) {
        g_appMain = new AppMain(g_assetManager);
    } else {
        ALOGE("Unsupported OpenGL ES version");
    }


}

JNIEXPORT void JNICALL
Java_com_android_gles3jni_GLES3JNILib_resize(
        JNIEnv* /*env*/,
        jobject /*obj*/,
        jint width,
        jint height) {
    if (g_appMain) {
        g_appMain->Resize(width, height);
    }
}

JNIEXPORT void JNICALL
Java_com_android_gles3jni_GLES3JNILib_step(JNIEnv* env, jobject obj) {
    if (g_appMain) {
        g_appMain->Step();
    }
}

