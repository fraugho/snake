#ifndef SCREEN_H
#define SCREEN_H

#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "init.h"
#include "buffer.h"

typedef struct Point{
    int16_t x;
    int16_t y;
} Point;

typedef struct Screen{
    Buffer* frames;
    uint16_t width, height;
} Screen;

/* Global state */
Screen screen;

#define RENDER 1 << 1
#define IO 1 << 2

static inline void draw_blank(){
    char* buf = (char*)malloc(screen.width * screen.height);
    memset(buf, ' ', screen.width * screen.height);
    write(STDOUT_FILENO, buf, screen.width * screen.height);
}

void screen_init() {
    enable_raw_mode();
    if (get_window_size(&screen.height, &screen.width) == -1){
        die("get_window_size");
    }
    screen.height -= debug;

    draw_blank();

    // Hide cursor
    write(STDOUT_FILENO, "\x1b[?25l", 6);

    // Initialize both buffers
    screen.frames = (Buffer*)malloc(sizeof(Buffer) * num_frames);

    for (int i = 0; i < num_frames; i++){
        screen.frames[i].c = (char*)malloc(BUF_SIZE);
        screen.frames[i].len = BUF_SIZE;
        screen.frames[i].used = 0;
        screen.frames[i].state = RENDER;
    }
}

void clean_square(Buffer* buf, Point TopLeft, Point BottomRight, char c) {
    size_t total_len = 0;  // This will store the total length of the output string
    static const char* format = "\x1b[%hd;%hdH%c";  // Escape sequence format

    // Calculate how much space we will need in the buffer
    for (uint16_t y = TopLeft.y; y <= BottomRight.y; ++y) {
        for (uint16_t x = TopLeft.x; x <= BottomRight.x; ++x) {
            total_len += snprintf(NULL, 0, format, y, x, c);  // Add the length for each formatted string
        }
    }

    // Make sure the buffer has enough space for the total string
    if (total_len > buf->len - buf->used) {
        return;
    }

    char* cursor = &buf->c[buf->used];

    for (uint16_t y = TopLeft.y; y <= BottomRight.y; ++y) {
        for (uint16_t x = TopLeft.x; x <= BottomRight.x; ++x) {
            cursor += snprintf(cursor, buf->len - (cursor - buf->c), format, y, x, c);
        }
    }

    // Update the buffer used size
    buf->used += total_len;
}

static inline void write_to_terminal(Buffer* buf) {
    write(STDOUT_FILENO, buf->c, buf->used);
}

static inline void draw_pixel(Buffer* buf, Point point, char c){
    uint16_t len = snprintf(&buf->c[buf->used], buf->len - buf-> used, "\x1b[%hd;%hdH%c", point.y, point.x, c);
    buf->used += len;
}

static inline void free_screen(){
    disable_raw_mode();
    for(int i = 0; i < num_frames; ++i){
        buf_free(&screen.frames[i]);
    }
}

#endif
