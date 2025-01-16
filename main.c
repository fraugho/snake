/* Terminator - A simple terminal graphics system */

/* Standard includes */
#include <unistd.h>
#include <termios.h>
#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <sys/ioctl.h>
#include <string.h>
#include <time.h>
#include <stdbool.h>
#include <pthread.h>
//mine
#include <init.h>
#include <screen.h>
#include <timing.h>
#include <snake.h>
#include <input.h>

//global snake
Snake snake;

int times = 0;
long total = 0;
long min_time = 1000000;  // 1 second in microseconds
long max_time = 0;


/* Constants and macros */
#define true 1
#define false 0
#define CTRL_KEY(k) ((k) & 0x1f)

bool RUNNING = true;

int io_index = 0;
int render_index = 0;
int clean_index = 0;

/* Buffer operations */
#define BUFFER_INIT {NULL, 0}

void disable_raw_mode() {
    if (tcsetattr(STDIN_FILENO, TCSAFLUSH, &screen.og_termios) == -1)
        die("tcsetattr");
}

void enable_raw_mode() {
    if (tcgetattr(STDIN_FILENO, &screen.og_termios) == -1)
        die("tcgetattr");
    atexit(disable_raw_mode);

    struct termios raw = screen.og_termios;
    raw.c_iflag &= ~(IXON | ICRNL | BRKINT | INPCK | ISTRIP);
    raw.c_oflag &= ~(OPOST);
    raw.c_cflag |= (CS8);
    raw.c_lflag &= ~(ECHO | ICANON | ISIG | IEXTEN);
    raw.c_cc[VMIN] = 0;
    raw.c_cc[VTIME] = 1;  // Read timeout in deciseconds

    if (tcsetattr(STDIN_FILENO, TCSAFLUSH, &raw) == -1)
        die("tcsetattr");
}

/* Cleaning */
void*
thread_clean(){
    while(RUNNING){
        if (screen.frames[clean_index].state == CLEAN){
            clear_frame_buf(&screen.frames[clean_index]);
            screen.frames[clean_index].state = RENDER;
            clean_index = (clean_index + 1) % num_frames;
        }
    }
    return NULL;
}

long rps = 0;
/* Writing */
void*
thread_write() {
    long start = get_us();
    while(RUNNING){
        if (screen.frames[io_index].state == IO){
            write(STDOUT_FILENO, screen.frames[io_index].c, screen.frames[io_index].len);
            screen.frames[io_index].state = CLEAN;
            io_index = (io_index + 1) % num_frames;

            // Calculate and display timing
            long end = get_us();
            long elapsed_us = end - start;

            if(debug){
                char t_buf[100];
                int len = snprintf(t_buf, sizeof(t_buf), "\x1b[K\r frame time taken: %ld us | render time taken: %ld us i: 0 x: %d y: %d size: %zu\x1b[K\r",
                                   elapsed_us, rps, ((int*)snake.x->data)[1], ((int*)snake.y->data)[1], snake.x->capacity);
                write(STDOUT_FILENO, t_buf, len);
            }

            start = get_us();

            total += elapsed_us;

            if (elapsed_us < min_time) min_time = elapsed_us;
            if (elapsed_us > max_time) max_time = elapsed_us;
        }
    }
    return NULL;
}

/* Rendering */
void* thread_snake_render() {
    long start = get_us();
    static long last_move = 0;
    static long last_render = 0;
    while(RUNNING) {
        if (screen.frames[render_index].state == RENDER) {

            const long RENDER_INTERVAL = 50000;
            const long MOVE_INTERVAL = 100000;
            long current_time = get_us();

            if (current_time - last_render > RENDER_INTERVAL){
                move_snake(screen.frames[render_index].c, &snake);
                last_render = get_us();
                if (current_time - last_move > MOVE_INTERVAL){
                    snake_move(&snake, last_key);
                    last_move = get_us();
                }
            }

            snake_render(screen.frames[render_index].c, snake);
            apple_render(screen.frames[render_index].c, snake);

            screen.frames[render_index].state = IO;
            render_index = (render_index + 1) % num_frames;
            times++;

            long end = get_us();
            rps = end - start;

            start = get_us();
        }
    }
    return NULL;
}

/* Main entry point */
int main() {
    screen_init();
    snake_init(&snake);
    enable_raw_mode();

    long cpu_num = sysconf(_SC_NPROCESSORS_ONLN);
    pthread_t threads[cpu_num];

    pthread_create(&threads[0], NULL, thread_snake_render, NULL);
    pthread_create(&threads[1], NULL, thread_write, NULL);
    pthread_create(&threads[2], NULL, thread_clean, NULL);

    while (last_key != CTRL_KEY('q')) {
        last_key = editor_read_key();
    }

    RUNNING = false;

    for(int i = 0; i < 3; i++) {
        pthread_join(threads[i], NULL);
    }

    printf("\nPerformance Stats:\n\r");
    printf("Average frame time: %ld us (%.2f FPS)\n\r",
           total / times, 1000000.0 / (total / (float)times));
    printf("Best frame time: %ld us (%.2f FPS)\n\r", 
           min_time, 1000000.0 / min_time);
    printf("Worst frame time: %ld us (%.2f FPS)", 
           max_time, 1000000.0 / max_time);
    
    disable_raw_mode();

    free_screen();
    free_snake(&snake);
    return 0;
}
