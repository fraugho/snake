/* Terminator - A simple terminal graphics system */
/* Standard includes */
#include <stdbool.h>
//mine
#include "timing.h"
#include "snake.h"
#include "engine.h"

//global snake
Snake snake;

/* Rendering */
void render() {
    static int64_t last_move = 0;
    static int64_t last_render = 0;
    static const int64_t X_RENDER_INTERVAL = 50000;
    static const int64_t Y_RENDER_INTERVAL = 100000;
    static const int64_t MOVE_INTERVAL = 100000;
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
}

/* Main entry point */
int main() {
    snake_init(&snake);
    engine_init();
}
