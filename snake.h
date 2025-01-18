#ifndef SNAKE_H
#define SNAKE_H

#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>

#include <timing.h>
#include <screen.h>
#include <init.h>
#include <vec.h>

const uint8_t INIT_SIZE = 5;

typedef struct Snake{
    Vec* x;
    Vec* y;
    int vx;
    int vy;
    int size;
    int apple_x;
    int apple_y;
} Snake;

void snake_init(Snake* snake){
    snake->size = INIT_SIZE;

    snake->vx = 1;
    snake->vy = 0;

    snake->x = vec_create(INIT_SIZE, sizeof(int));
    for(int i = 0; i < INIT_SIZE; ++i){
        vec_append(snake->x, &i);
    }

    snake->y = vec_create(INIT_SIZE, sizeof(int));
    for(int i = 0; i < INIT_SIZE; ++i){
        int num = 50;
        vec_append(snake->y, &num);
    }
    snake->apple_x = 50;
    snake->apple_y = 50;
}

static uint8_t y_move = 0;
void snake_move(Snake* snake, int key) {
    switch (key) {
        //up
        case 'w':
            snake->vx = 0;
            snake->vy = -1;
            y_move = 1;
            break;
        //left
        case 'a':
            snake->vx = -1;
            snake->vy = 0;
            y_move = 0;
            break;
        //down
        case 's':
            snake->vx = 0;
            snake->vy = 1;
            y_move = 1;
            break;
        //right
        case 'd':
            snake->vx = 1;
            snake->vy = 0;
            y_move = 0;
            break;
        default:
            break;
    }
}

void move_head(Snake* snake){

    ((int*)snake->x->data)[0] += snake->vx;
    ((int*)snake->y->data)[0] += snake->vy;

    // Wrap around the screen
    if (((int*)snake->x->data)[0] < 0){
        ((int*)snake->x->data)[0] = screen.width - 1;
    }
    if (((int*)snake->x->data)[0] > screen.width){
        ((int*)snake->x->data)[0] = 0;
    }
    if (((int*)snake->y->data)[0] < 0){
        ((int*)snake->y->data)[0] = screen.height;
    }
    if (((int*)snake->y->data)[0] > screen.height){
        ((int*)snake->y->data)[0] = 0;
    }
}

static inline void apple_logic(Snake* snake){
    if (snake->apple_x == ((int*)snake->x->data)[0]){
        if (snake->apple_y == ((int*)snake->y->data)[0]){
            int num = 0;
            //adds body cell
            vec_append(snake->x, &num);
            vec_append(snake->y, &num);
            ++snake->size;

            //changes apple position
            snake->apple_x = rand() % screen.width;
            snake->apple_y = rand() % screen.height;
        }
    }
}

void move_snake(Snake* snake) {
    apple_logic(snake);

    // Store old head position
    int prev_x = ((int*)snake->x->data)[0];
    int prev_y = ((int*)snake->y->data)[0];

    // Move head first
    move_head(snake);

    // Move rest of body - each segment takes previous segment's position
    for(int i = 1; i < snake->size; ++i) {
        int temp = ((int*)snake->x->data)[i];
        ((int*)snake->x->data)[i] = prev_x;
        prev_x = temp;

        temp = ((int*)snake->y->data)[i];
        ((int*)snake->y->data)[i] = prev_y;
        prev_y = temp;
    }
}

// Draw snake
void snake_render(Buffer* buf, const Snake* snake) {
    for(int i = 0; i < snake->size; ++i) {
        draw_pixel(buf, ((int*)snake->x->data)[i], ((int*)snake->y->data)[i], '#');
    }
}

void snake_clean(Buffer* buf, const Snake* snake) {
    for(int i = 0; i < snake->size; ++i) {
        draw_pixel(buf, ((int*)snake->x->data)[i], ((int*)snake->y->data)[i], ' ');
    }
}

void apple_render(Buffer* buf, const Snake* snake) {
    int center_y = screen.height / 2;
    draw_pixel(buf, snake->apple_x, snake->apple_y, '*');
}

void free_snake(Snake* snake){
    free_vec(snake->x);
    free_vec(snake->y);
}

#endif
