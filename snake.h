#ifndef SNAKE_H
#define SNAKE_H

#include <vec.h>
#include <stdlib.h>
#include <stdio.h>
#include <timing.h>
#include <screen.h>

typedef struct Snake{
    Vec x;
    Vec y;
    Vec vx;
    Vec vy;
} Snake;

//global snake
Snake snake;

void
snake_init(Snake* snake, int size){

    snake->x.data = (char*)malloc(sizeof(int) * size);
    snake->x.used = size;
    snake->x.capacity = size;

    for(int i = 0; i < snake->x.used; ++i){
        ((int*)snake->x.data)[i] = i;
    }

    snake->y.data = (char*)malloc(sizeof(int) * size);
    snake->y.used = size;
    snake->y.capacity = size;

    snake->vx.data = (char*)malloc(sizeof(int) * size);
    snake->vx.used = size;
    snake->vx.capacity = size;

    snake->vy.data = (char*)malloc(sizeof(int) * size);
    snake->vy.used = size;
    snake->vy.capacity = size;
}

void
snake_move_(char* frame, Snake* snake){
    int center_y = screen.height / 2;
    long current_time = get_ms();
    float elapsed = (current_time - screen.start_time) / 1000.0f;  // In seconds

    // Pixels per second - adjust this number to control speed
    float pixels_per_second = screen.width / 100.0f;  // Cross screen in 2 seconds

    for(int i = 0; i < snake->x.used; ++i){
        frame[snake->x.data[i] % screen.width + center_y * (screen.width + 5) + 3] = '#';
        snake->x.data[i] = (int)(snake->x.data[i] + (elapsed * pixels_per_second)) % screen.width;
    }

    //placeholder
    switch(0){
        case 'w':
            break;
        case 'a':
            break;
        case 's':
            break;
        case 'd':
            break;
        default:
            break;
    }
}

void snake_move_f(char* frame, Snake* snake) {
    int center_y = screen.height / 2;
    static long last_move_time = 0;
    long current_time = get_ms();
    
    // Calculate how many frames we want per snake movement
    // For example, if rendering at 120fps and we want snake to move 10 times per second:
    const int FRAMES_PER_MOVE = 12;  // 120fps / 10 movements = 12 frames per move
    static int frame_counter = 0;
    
    frame_counter++;
    if (frame_counter >= FRAMES_PER_MOVE) {
        // Store old head position
        int prev_x = snake->x.data[0];
        
        // Move head
        snake->x.data[0] = (snake->x.data[0] + 1) % screen.width;
        
        // Move body segments
        for(int i = 1; i < snake->x.used; ++i) {
            int temp = snake->x.data[i];
            snake->x.data[i] = prev_x;
            prev_x = temp;
        }
        
        frame_counter = 0;
    }
    
    // Draw snake (happens every frame regardless of movement)
    for(int i = 0; i < snake->x.used; ++i) {
        frame[snake->x.data[i] % screen.width + center_y * (screen.width + 5) + 3] = '#';
    }
}

void snake_move(char* frame, Snake* snake) {
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
        
        // Move head first
        ++snake->x.data[0];
        
        // Move rest of body - each segment takes previous segment's position
        for(int i = 1; i < snake->x.used; ++i) {
            int temp = ((int*)snake->x.data)[i];
            ((int*)snake->x.data)[i] = prev_x;
            prev_x = temp;
        }
        
        last_move_time = current_time;
    }
    
    // Draw snake
    for(int i = 0; i < snake->x.used; ++i) {
        frame[((int*)snake->x.data)[i] % screen.width + center_y * (screen.width + 5) + 3] = '#';
    }
}

#endif
