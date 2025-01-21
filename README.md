# Terminal Snake Game

A classic Snake game implementation using the Termeric terminal graphics engine. The game features smooth animations, performance monitoring, and responsive controls in a terminal environment.

## Overview

This Snake game demonstrates the capabilities of the Termeric engine with:
- Smooth snake movement with variable speed based on direction
- Apple collection mechanics for snake growth
- Collision detection
- Screen wrapping
- Performance monitoring

## Game Features

### Controls
- `w` - Move Up
- `s` - Move Down
- `a` - Move Left
- `d` - Move Right
- `Ctrl+q` - Quit game

### Gameplay Elements
- Snake (`#`): Controlled by the player
- Apple (`*`): Collect to grow longer
- Screen wrapping: Snake can move through edges
- Collision detection: Game ends if snake hits itself

## Technical Features

### Movement System
- Different render intervals for horizontal/vertical movement to account for terminal character spacing
- Smooth movement using timer-based updates
- Configurable movement speeds:
  ```c
  #define X_RENDER_INTERVAL 50000  // Horizontal movement timing
  #define Y_RENDER_INTERVAL 100000 // Vertical movement timing
  #define MOVE_INTERVAL 100000     // Base movement speed
  ```

### Snake Implementation
- Dynamic body growth using vector data structure
- Efficient collision detection
- Separate rendering and movement logic
- Random apple placement

## Requirements

- Termeric engine (included)
- POSIX-compliant operating system
- C compiler with C11 support
- Terminal with ANSI escape sequence support

## Building

```bash
gcc main.c -o snake
```

### Key Features

#### Snake Structure
```c
typedef struct Snake {
    Vec* x;         // X coordinates of snake segments
    Vec* y;         // Y coordinates of snake segments
    int8_t vx;      // X velocity
    int8_t vy;      // Y velocity
    uint8_t size;   // Current snake length
    int16_t apple_x; // Apple X position
    int16_t apple_y; // Apple Y position
} Snake;
```
#### Performance Monitoring
- Frame timing statistics
- Configurable update intervals

## Implementation Details

### Initialization
The snake starts with:
- Initial size of 5 segments
- Moving right
- Centered position
- Random apple placement

### Game Loop
1. Input processing
2. Movement updates
3. Collision detection
4. Apple collection
5. Screen rendering

### Collision System
- Self-collision detection
- Apple collection detection
- Screen boundary handling with wrapping

## Performance

The game includes built-in performance monitoring that tracks:
- Frame times
- FPS calculations
- Min/max frame times
- Average performance metrics

## Configuration

Several parameters can be adjusted in the source:
- Initial snake size (`INIT_SIZE`)
- Movement intervals
- Buffer sizes
- Screen update rates
