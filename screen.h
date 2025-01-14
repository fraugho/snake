#ifndef SCREEN_H
#define SCREEN_H

#include <unistd.h>
#include <termios.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <stdbool.h>
#include <init.h>
#include <timing.h>
#include <buffer.h>

typedef struct Screen {
    int width, height;
    struct termios og_termios;
    Buffer* frames;
    long start_time;
} Screen;

/* Global state */
struct Screen screen;
int num_frames = 10;

void clear_frame_buf(struct Buffer* frame){
    // For each line we need:
    // - screen.width characters
    // - \x1b[K\r\n (5 bytes) for clear line and newline
    int line_size = screen.width + 5 + 4;
    int total_size = 3 + (line_size * screen.height);  // 3 for initial \x1b[H
    
    memset(frame->c, ' ', total_size);
    memcpy(frame->c, "\x1b[H", 3);

    // Copy the formatted line screen.height times
    for (int i = 0; i < screen.height; i++) {
        //memcpy(&buf->c[3 + screen.width + i * (line_size)], " ", 1);
        memcpy(&frame->c[3 + i * line_size ], "\x1b[2K\r", 5);
        memcpy(&frame->c[3 + ((i + 1) * line_size) - 4],"\x1b[1B", 4);
    }
}

void draw_pixel(int x, int y, char c, char* frame){
    frame[x + y * (screen.width + 5 + 4) + 3] = c;
}

void screen_init() {
    if (get_window_size(&screen.height, &screen.width) == -1) 
        die("get_window_size");

    screen.height -= debug;

    // Hide cursor
    write(STDOUT_FILENO, "\x1b[?25l", 6); 
    
    // Initialize both buffers
    int line_size = screen.width + 5 + 4;
    int total_size = 3 + (line_size * screen.height);

    screen.frames = (Buffer*)malloc(sizeof(Buffer) * num_frames);

    for (int i = 0; i < num_frames; i++){
        screen.frames[i].c = (char*)malloc(total_size);
        screen.frames[i].len = total_size;
        clear_frame_buf(&screen.frames[i]);
        screen.frames[i].used = total_size;
        screen.frames[i].state = RENDER;
    }

    screen.start_time = get_ms();
}

void
free_screen(){
    for(int i = 0; i < num_frames; ++i){
        buf_free(&screen.frames[i]);
    }
}

#endif
