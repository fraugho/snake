#ifndef SCREEN_H
#define SCREEN_H

#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <unistd.h>

#include "init.h"
#include "timing.h"
#include "buffer.h"

typedef struct Screen {
    Buffer* frames;
    uint16_t width, height;
} Screen;

/* Global state */
struct Screen screen;
int num_frames = 10;
const size_t BUF_SIZE = 8000;

void screen_init() {
    if (get_window_size(&screen.height, &screen.width) == -1){
        die("get_window_size");
    }

    screen.height -= debug;

    // Hide cursor
    write(STDOUT_FILENO, "\x1b[?25l", 6);

    // Initialize buffers
    screen.frames = (Buffer*)malloc(sizeof(Buffer) * num_frames);

    for (int i = 0; i < num_frames; i++){
        screen.frames[i].c = (char*)malloc(BUF_SIZE);
        screen.frames[i].len = BUF_SIZE;
        screen.frames[i].used = 0;
        screen.frames[i].state = RENDER;
    }
}

void draw_blank(){
    char* buf = (char*)malloc(screen.width * screen.height);
    memset(buf, ' ', screen.width * screen.height);
    write(STDOUT_FILENO, buf, screen.width * screen.height);
}

void clean_square(Buffer* buf, uint16_t tlc_x, uint16_t tlc_y, uint16_t brc_x, uint16_t brc_y, char c) {
    size_t total_len = 0;  // This will store the total length of the output string
    static const char* format = "\x1b[%hu;%huH%c";  // Escape sequence format

    // Calculate how much space we will need in the buffer
    for (uint16_t y = tlc_y; y <= brc_y; ++y) {
        for (uint16_t x = tlc_x; x <= brc_x; ++x) {
            total_len += snprintf(NULL, 0, format, y, x, c);  // Add the length for each formatted string
        }
    }

    // Make sure the buffer has enough space for the total string
    if (total_len > buf->len - buf->used) {
        return;
    }

    char* cursor = &buf->c[buf->used];

    for (uint16_t y = tlc_y; y <= brc_y; ++y) {
        for (uint16_t x = tlc_x; x <= brc_x; ++x) {
            cursor += snprintf(cursor, buf->len - (cursor - buf->c), format, y, x, c);
        }
    }

    // Update the buffer used size
    buf->used += total_len;
}


static inline void write_to_terminal(Buffer* buf) {
    write(STDOUT_FILENO, buf->c, buf->used);
}

static inline void draw_pixel(Buffer* buf, uint16_t x, uint16_t y, char c){
    uint16_t len = snprintf(&buf->c[buf->used], buf->len - buf-> used, "\x1b[%hu;%huH%c", y, x, c);
    buf->used += len;
}

static inline void free_screen(){
    for(int i = 0; i < num_frames; ++i){
        buf_free(&screen.frames[i]);
    }
}

#endif
