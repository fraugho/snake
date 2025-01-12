#ifndef VEC_H
#define VEC_H

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct Vec{
    int32_t capacity;
    int32_t used;
    char *data;
    char type_size;
} Vec;

void
vec_append(Vec *vec, void* thing){
    if (vec->used < vec->capacity){
        memcpy(&vec->data[vec->used * vec->type_size], thing, vec->type_size);
        ++vec->used;
    }
    else{
        vec->data = (char*)realloc(vec->data, vec->capacity * vec->type_size * 2);
        memcpy(&vec->data[vec->used * vec->type_size], thing, vec->type_size);
        ++vec->used;
        vec->capacity *= 2;
    }
}

void
vec_pop(Vec *vec, void* dest){
    if (vec->used | 0){
        memcpy(dest, &vec->data[--vec->used * vec->type_size], vec->type_size);
    }
}

void
vec_insert(Vec *vec, void* thing, int32_t index){
    if (vec->used < vec->capacity){
        memcpy(&vec->data[(index + 1) * vec->type_size], &vec->data[index * vec->type_size], vec->type_size *  (vec->used - index));
        memcpy(&vec->data[index * vec->type_size], thing, vec->type_size);
        ++vec->used;
    }
    else{
        vec->data = (char*)realloc(vec->data, vec->capacity * vec->type_size * 2);
        memcpy(&vec->data[(index + 1) * vec->type_size], &vec->data[index * vec->type_size], vec->type_size *  (vec->used - index));
        memcpy(&vec->data[index * vec->type_size], thing, vec->type_size);
        ++vec->used;
        vec->capacity *= 2;
    }
}

void
vec_delete(Vec *vec, int32_t index){
    if (vec->used | 0){
        memcpy(&vec->data[index * vec->type_size], &vec->data[(index + 1) * vec->type_size], vec->type_size *  (vec->used-- - index));
    }
}
#endif
