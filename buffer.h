#ifndef BUFFER_H
#define BUFFER_H
#include <stdlib.h>
#include <string.h>

#define RENDER 1 << 1
#define IO 1 << 2

/* Data structures */
typedef struct Buffer {
    char *c;
    size_t len;
    size_t used;
    char state;
} Buffer;

void buf_free(Buffer *buf) {
    free(buf->c);
}

void buf_append(Buffer *buf, char* s, size_t len) {
    if (buf->used + len < buf->len){
        memcpy(&buf->c[buf->used], s, len);
    }
}
#endif
