#ifndef BUFFER_H
#define BUFFER_H
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

enum FrameState{
    RENDER = 1 << 1,
    IO = 1 << 2,
    CLEAN = 1 << 3,
};

/* Data structures */
typedef struct Buffer {
    char *c;
    int len;
    int used;
    enum FrameState state;
} Buffer;

void buffer_append(struct Buffer *buf, char *str, int len) {
    memcpy(&buf->c[buf->used], str, len);
    buf->used += len;
}

void buf_free(struct Buffer *abuf) {
    free(abuf->c);
}
#endif
