#ifndef BUFFER_H
#define BUFFER_H
#include <stdlib.h>

enum FrameState{
    RENDER = 1 << 1,
    IO = 1 << 2,
    CLEAN = 1 << 3,
};

/* Data structures */
typedef struct Buffer {
    char *c;
    int len;
    enum FrameState state;
} Buffer;

void buf_free(Buffer *buf) {
    free(buf->c);
}
#endif
