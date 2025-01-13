#ifndef SNAKE_H
#define SNAKE_H

#include <vec.h>
#include <stdlib.h>
#include <stdio.h>
#include <timing.h>
#include <screen.h>
#include <init.h>

typedef struct Snake{
    Vec x;
    Vec y;
    int vx;
    int vy;
    int size;
} Snake;


int last_key = 0;

void
snake_init(Snake* snake, int size){

    snake->x.data = (char*)malloc(sizeof(int) * size);
    snake->x.used = size;
    snake->x.capacity = size;
    for(int i = 0; i < size; ++i){
        ((int*)snake->x.data)[i] = i;
    }

    snake->y.data = (char*)malloc(sizeof(int) * size);
    snake->y.used = size;
    snake->y.capacity = size;
    for(int i = 0; i < size; ++i){
        ((int*)snake->y.data)[i] = 50;
    }

    snake->vx = 2;
    snake->vy = 0;

    snake->size = size;
}

void snake_move(Snake* snake, int key) {
    switch (key) {
        //up
        case 'w':
            snake->vx = 0;
            snake->vy = -1;
            break;
        //left
        case 'a':
            snake->vx = -2;
            snake->vy = 0;
            break;
        //down
        case 's':
            snake->vx = 0;
            snake->vy = 1;
            break;
        //right
        case 'd':
            snake->vx = 2;
            snake->vy = 0;
            break;
        default:
            break;
    }
}

void move_snake(char* frame, Snake* snake) {

    //snake_move(snake, last_key);
    // Store old head position
    int prev_x = ((int*)snake->x.data)[0];
    int prev_y = ((int*)snake->y.data)[0];

    // Move head first
    ((int*)snake->x.data)[0] += snake->vx;
    ((int*)snake->y.data)[0] += snake->vy;

    // Move rest of body - each segment takes previous segment's position
    for(int i = 1; i < snake->x.used; ++i) {
        int temp = ((int*)snake->x.data)[i];
        ((int*)snake->x.data)[i] = prev_x;
        prev_x = temp;
        temp = ((int*)snake->y.data)[i];
        ((int*)snake->y.data)[i] = prev_y;
        prev_y = temp;
    }
}

void snake_render(char* frame, Snake* snake) {
    int center_y = screen.height / 2;
    // Draw snake
    for(int i = 0; i < snake->x.used; ++i) {
        frame[((int*)snake->x.data)[i] % screen.width + ((int*)snake->y.data)[i] * (screen.width + 5) + 3] = '#';
    }
}

void snake_render_(char* frame, Snake* snake) {
    int center_y = screen.height / 2;
    static long last_move_time = 0;  // Track last movement time
    long current_time = get_ms();
    
    // Move every 100ms (0.1 seconds)
    // This means it'll take 1 second to move 10 spaces
    const long MOVE_INTERVAL = 30;
    
    // Only move if enough time has passed
    if (current_time - last_move_time >= MOVE_INTERVAL) {
        // Store old head position
        int prev_x = ((int*)snake->x.data)[0];
        int prev_y = ((int*)snake->y.data)[0];
        
        // Move head first
        ++((int*)snake->x.data)[0];
        
        // Move rest of body - each segment takes previous segment's position
        for(int i = 1; i < snake->x.used; ++i) {
            int temp = ((int*)snake->x.data)[i];
            ((int*)snake->x.data)[i] = prev_x;
            prev_x = temp;
            temp = ((int*)snake->y.data)[i];
            ((int*)snake->y.data)[i] = prev_y;
            prev_y = temp;
        }
        
        last_move_time = current_time;
    }

    // Draw snake
    for(int i = 0; i < snake->x.used; ++i) {
        frame[((int*)snake->x.data)[i] % screen.width + ((int*)snake->y.data)[i] * (screen.width + 5) + 3] = '#';
    }
}

#endif
