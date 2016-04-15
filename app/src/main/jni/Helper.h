//
// Created by Simon on 15.04.2016.
//

#ifndef OPENGL_ES_EXAMPLE_HELPER_H
#define OPENGL_ES_EXAMPLE_HELPER_H

#include <string>
#include <sstream>
#include <android/log.h>



#define DEBUG 1

#define LOG_TAG "GLES3JNI"
#define ALOGE(...) __android_log_print(ANDROID_LOG_ERROR, LOG_TAG, __VA_ARGS__)
#if DEBUG
#define ALOGV(...) __android_log_print(ANDROID_LOG_VERBOSE, LOG_TAG, __VA_ARGS__)

//TODO check this has to be tested
#define ALOGD(...) __android_log_print(ANDROID_LOG_DEBUG, LOG_TAG, __VA_ARGS__)
#else
#define ALOGV(...)
#define ALOGD(...)
#endif

///assert functions with printf syntax
#define clean_errno() (errno == 0 ? "None" : strerror(errno))
#define log_error(M, ...) fprintf(stderr, "[ERROR] %s:%d: errno: %s \n\t" M "\n", __FILE__, __LINE__, clean_errno(), ##__VA_ARGS__)
#define assertf(A, M, ...) if(!(A)) {log_error(M, ##__VA_ARGS__); assert(A); }




// android is missing all cpp11 features
// this snippet should only be used in debug code
template <typename T>
std::string ToString(T value) {
    std::ostringstream os ;
    os << value ;
    return os.str() ;
}



// String replacement for defines
#define STR(s) #s
#define STRV(s) STR(s)

















#endif //OPENGL_ES_EXAMPLE_HELPER_H
