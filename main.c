/* Terminator - A simple terminal graphics system */
/* Standard includes */
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <pthread.h>
#include <stdint.h>
//mine
#include "timing.h"
#include "screen.h"
#include "init.h"
#include "input.h"
#include "snake.h"
#include "engine.h"

//global snake
Snake snake;

int64_t rps = 0;
int64_t main_start = 0;

/* Rendering */
void* thread_render() {
    int64_t start = get_us();
    static int64_t last_move = 0;
    static int64_t last_render = 0;
    const int64_t X_RENDER_INTERVAL = 50000;
    const int64_t Y_RENDER_INTERVAL = 100000;
    const int64_t MOVE_INTERVAL = 100000;
    while(RUNNING) {
        if (screen.frames[render_index].state == RENDER) {
            int64_t current_time = get_us();

            if (y_move){
                if (current_time - last_render > Y_RENDER_INTERVAL) {
                    snake_clean(&screen.frames[render_index], &snake);
                    move_snake(&snake);
                    last_render = get_us();
                    if (current_time - last_move > MOVE_INTERVAL) {
                        snake_move(&snake, last_key);
                        last_move = get_us();
                    }
                    snake_render(&screen.frames[render_index], &snake);
                    apple_render(&screen.frames[render_index], &snake);
                }
            } else {
                if (current_time - last_render > X_RENDER_INTERVAL) {
                    snake_clean(&screen.frames[render_index], &snake);
                    move_snake(&snake);
                    last_render = get_us();
                    if (current_time - last_move > MOVE_INTERVAL) {
                        snake_move(&snake, last_key);
                        last_move = get_us();
                    }
                    snake_render(&screen.frames[render_index], &snake);
                    apple_render(&screen.frames[render_index], &snake);
                }
            }

            screen.frames[render_index].state = IO;
            render_index = (render_index + 1) % num_frames;

            int64_t end = get_us();
            rps = time_diff_us(start, end);

            start = get_us();
        }
    }
    return NULL;
}

/* Main entry point */
int main() {
    snake_init(&snake);
    engine_init();
}
