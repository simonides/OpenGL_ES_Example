//
// Created by Simon on 15.04.2016.
//

#ifndef OPENGL_ES_EXAMPLE_HELPER_H
#define OPENGL_ES_EXAMPLE_HELPER_H





#define clean_errno() (errno == 0 ? "None" : strerror(errno))

#define log_error(M, ...) fprintf(stderr, "[ERROR] %s:%d: errno: %s \n\t" M "\n", __FILE__, __LINE__, clean_errno(), ##__VA_ARGS__)

#define assertf(A, M, ...) if(!(A)) {log_error(M, ##__VA_ARGS__); assert(A); }



#endif //OPENGL_ES_EXAMPLE_HELPER_H
