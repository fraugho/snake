#ifndef SNAKE_H
#define SNAKE_H

#include <vec.h>
#include <stdlib.h>
#include <stdio.h>
#include <timing.h>
#include <screen.h>
#include <init.h>
#include <stdbool.h>

typedef struct Snake{
    Vec* x;
    Vec* y;
    int vx;
    int vy;
    int size;
    int apple_x;
    int apple_y;
} Snake;

void
snake_init(Snake* snake, int size){
    snake->x = vec_create(size, sizeof(int));
    for(int i = 0; i < size; ++i){
        vec_append(snake->x, &i);
    }
    snake->y = vec_create(size, sizeof(int));
    for(int i = 0; i < size; ++i){
        int num = 50;
        vec_append(snake->y, &num);
    }
    snake->vx = 1;
    snake->vy = 0;
    snake->apple_x = 50;
    snake->apple_y = 50;
    snake->size = size;
}

static int y_move = 0;
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

static int y_toggle = 1;
void
move_head(Snake* snake){

    ((int*)snake->x->data)[0] += snake->vx;
    ((int*)snake->y->data)[0] += snake->vy * y_toggle;

    // Wrap around the screen
    if (((int*)snake->x->data)[0] < 0){
        ((int*)snake->x->data)[0] = screen.width;
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

void move_snake(char* frame, Snake* snake) {
    if (snake->apple_x == ((int*)snake->x->data)[0]){
        if (snake->apple_y == ((int*)snake->y->data)[0]){
            int num = 0;
            vec_append(snake->x, &num);
            vec_append(snake->y, &num);
            snake->apple_x = rand() % screen.width;
            snake->apple_y = rand() % screen.height;
            ++snake->size;
        }
    }

    //snake_move(snake, last_key);
    // Store old head position
    int prev_x = ((int*)snake->x->data)[0];
    int prev_y = ((int*)snake->y->data)[0];

    // Move head first
    move_head(snake);

    if (y_toggle){
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


    // Toggle y movement based on terminal cell proportions
    if (y_move){
        y_toggle = 1 - y_toggle;
    }
}

void snake_render(char* frame, Snake* snake) {
    int center_y = screen.height / 2;
    // Draw snake
    for(int i = 0; i < snake->size; ++i) {
        frame[((int*)snake->x->data)[i] + ((int*)snake->y->data)[i] * (screen.width + 5) + 3] = '#';
    }
}

void apple_render(char* frame, Snake* snake) {
    int center_y = screen.height / 2;
    frame[snake->apple_x + snake->apple_y * (screen.width + 5) + 3] = '*';
}

#endif
