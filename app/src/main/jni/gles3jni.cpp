#include <jni.h>
#include <stdlib.h>
//#include <time.h>
//#include <glm/gtc/type_ptr.hpp>

#include "Helper.h"
#include "gles3jni.h"
#include "Program.h"
#include "AppMain.h"

using namespace program;

static AppMain* g_appMain = nullptr;


extern "C" {
    JNIEXPORT void JNICALL Java_com_android_gles3jni_GLES3JNILib_init(JNIEnv* env, jobject obj);
    JNIEXPORT void JNICALL Java_com_android_gles3jni_GLES3JNILib_resize(JNIEnv* env, jobject obj, jint width, jint height);
    JNIEXPORT void JNICALL Java_com_android_gles3jni_GLES3JNILib_step(JNIEnv* env, jobject obj);
};



// init function that creates the appmain instance
JNIEXPORT void JNICALL
Java_com_android_gles3jni_GLES3JNILib_init(JNIEnv* env, jobject obj) {
    if (g_appMain) {
        delete g_appMain;
        g_appMain = nullptr;
    }

    program::printGlString("Version", GL_VERSION);
    printGlString("Vendor", GL_VENDOR);
    printGlString("Renderer", GL_RENDERER);
    printGlString("Extensions", GL_EXTENSIONS);

    const char* versionStr = (const char*)glGetString(GL_VERSION);
    if (strstr(versionStr, "OpenGL ES 3.")) {
        g_appMain = new AppMain();
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


