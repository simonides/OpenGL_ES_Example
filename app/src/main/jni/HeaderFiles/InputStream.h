////
//// Created by Jakob on 24.04.2016.
////
//
//#ifndef OPENGL_ES_EXAMPLE_INPUTSTREAM_H
//#define OPENGL_ES_EXAMPLE_INPUTSTREAM_H
//
//
//class InputStream {
//    const char* buffer;
//    int size;
//    int idx;
//public:
//    InputStream(const char* buffer, int size)
//            : buffer(buffer)
//            , size(size)
//            , idx(0) {}
//
//    char getChar() {
//        if(eof()) {
//            return 0;
//        }
//        return buffer[idx++];
//    }
//
//    void ignoreLine() {
//        char c;
//        do {
//            c = getChar();
//        } while(!eof() &&  c != '\n');
//    }
//    bool eof() {
//        return idx >= size;
//    }
//};
//
//#endif //OPENGL_ES_EXAMPLE_INPUTSTREAM_H
