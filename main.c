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

int times = 0;
int64_t total = 0;
int64_t min_time = INT64_MAX;  // Initialize to maximum possible value
int64_t max_time = 0;

/* Constants and macros */
#define CTRL_KEY(k) ((k) & 0x1f)

bool RUNNING = true;

int io_index = 0;
int render_index = 0;

//global snake
Snake snake;

int64_t rps = 0;
int64_t main_start = 0;

/* Writing */
void* thread_write() {
    int64_t start = get_ns();
    main_start = start;
    while(RUNNING) {
        if (screen.frames[io_index].state == IO) {
            if (screen.frames[io_index].used != 0) {
                write_to_terminal(&screen.frames[io_index]);
            }
            screen.frames[io_index].used = 0;
            screen.frames[io_index].state = RENDER;

            io_index = (io_index + 1) % num_frames;

            int64_t current = get_ns();
            int64_t elapsed_ns = time_diff_ns(start, current);

            if (elapsed_ns > 0) {
                total += elapsed_ns;
                min_time = (elapsed_ns < min_time) ? elapsed_ns : min_time;
                max_time = (elapsed_ns > max_time) ? elapsed_ns : max_time;
                ++times;
            }

            start = get_ns();
        }
    }
    return NULL;
}

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

void print_timings(){
    printf("\n\r\x1b[K");
    printf("Performance Stats:\n\x1b[K\r");
    if (times > 0) {
        double avg_time = total / (double)times;
        printf("Average frame time: %.2f ns (%.2f FPS)\n\x1b[K\r",
               avg_time, ns_to_fps(avg_time));
        printf("Best frame time: %lld ns (%.2f FPS)\n\x1b[K\r", 
               min_time, ns_to_fps(min_time));
        printf("Worst frame time: %lld ns (%.2f FPS)\n\x1b[K\r", 
               max_time, ns_to_fps(max_time));
    }
}

/* Main entry point */
int main() {
    screen_init();
    snake_init(&snake);
    enable_raw_mode();

    draw_blank();

    long cpu_num = sysconf(_SC_NPROCESSORS_ONLN);
    pthread_t threads[cpu_num];

    pthread_create(&threads[0], NULL, thread_render, NULL);
    pthread_create(&threads[1], NULL, thread_write, NULL);

    while (last_key != CTRL_KEY('q')) {
        last_key = editor_read_key();
    }

    RUNNING = false;

    for(int i = 0; i < 2; i++) {
        pthread_join(threads[i], NULL);
    }

    print_timings();

    disable_raw_mode();
    free_screen();
    return 0;
}
