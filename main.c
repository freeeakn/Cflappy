#include "SDL.h"
#include <stdio.h>
#include <stdbool.h>

#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 600
#define BIRD_WIDTH 40
#define BIRD_HEIGHT 40
#define PIPE_WIDTH 70
#define PIPE_GAP 200
#define GRAVITY 0.7f
#define JUMP_FORCE -10.0f

typedef struct {
    float x;
    float y;
    float velocity;
} Bird;

typedef struct {
    int x;
    int top_height;
    bool passed;
} Pipe;

SDL_Window* window = NULL;
SDL_Renderer* renderer = NULL;
Bird bird;
Pipe pipes[3];
bool game_running = true;
int score = 0;

void init_game() {
    // Initialize bird
    bird.x = WINDOW_WIDTH / 4;
    bird.y = WINDOW_HEIGHT / 2;
    bird.velocity = 0;

    // Initialize pipes
    for (int i = 0; i < 3; i++) {
        pipes[i].x = WINDOW_WIDTH + (i * WINDOW_WIDTH/2);
        pipes[i].top_height = (rand() % (WINDOW_HEIGHT - PIPE_GAP - 100)) + 50;
        pipes[i].passed = false;
    }
}

void handle_input() {
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        if (event.type == SDL_QUIT) {
            game_running = false;
        } else if (event.type == SDL_KEYDOWN) {
            if (event.key.keysym.sym == SDLK_SPACE) {
                bird.velocity = JUMP_FORCE;
            }
        }
    }
}

void update_game() {
    // Update bird
    bird.velocity += GRAVITY;
    bird.y += bird.velocity;

    // Check collisions with window boundaries
    if (bird.y < 0) bird.y = 0;
    if (bird.y > WINDOW_HEIGHT - BIRD_HEIGHT) {
        bird.y = WINDOW_HEIGHT - BIRD_HEIGHT;
        game_running = false;
    }

    // Update pipes and check collisions
    for (int i = 0; i < 3; i++) {
        pipes[i].x -= 3;

        // Reset pipe when it goes off screen
        if (pipes[i].x < -PIPE_WIDTH) {
            pipes[i].x = WINDOW_WIDTH;
            pipes[i].top_height = (rand() % (WINDOW_HEIGHT - PIPE_GAP - 100)) + 50;
            pipes[i].passed = false;
        }

        // Check for collision with pipes
        if (bird.x + BIRD_WIDTH > pipes[i].x && bird.x < pipes[i].x + PIPE_WIDTH) {
            if (bird.y < pipes[i].top_height || bird.y + BIRD_HEIGHT > pipes[i].top_height + PIPE_GAP) {
                game_running = false;
            }
        }

        // Update score
        if (!pipes[i].passed && bird.x > pipes[i].x + PIPE_WIDTH) {
            score++;
            pipes[i].passed = true;
        }
    }
}

void render_game() {
    SDL_SetRenderDrawColor(renderer, 135, 206, 235, 255); // Sky blue background
    SDL_RenderClear(renderer);

    // Draw bird
    SDL_Rect bird_rect = {(int)bird.x, (int)bird.y, BIRD_WIDTH, BIRD_HEIGHT};
    SDL_SetRenderDrawColor(renderer, 255, 255, 0, 255); // Yellow bird
    SDL_RenderFillRect(renderer, &bird_rect);

    // Draw pipes
    SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255); // Green pipes
    for (int i = 0; i < 3; i++) {
        // Top pipe
        SDL_Rect top_pipe = {pipes[i].x, 0, PIPE_WIDTH, pipes[i].top_height};
        SDL_RenderFillRect(renderer, &top_pipe);

        // Bottom pipe
        SDL_Rect bottom_pipe = {
            pipes[i].x,
            pipes[i].top_height + PIPE_GAP,
            PIPE_WIDTH,
            WINDOW_HEIGHT - (pipes[i].top_height + PIPE_GAP)
        };
        SDL_RenderFillRect(renderer, &bottom_pipe);
    }

    SDL_RenderPresent(renderer);
}

int main(int argc, char* argv[]) {
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        printf("SDL initialization failed: %s\n", SDL_GetError());
        return 1;
    }

    window = SDL_CreateWindow("Flappy Bird Clone",
        SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
        WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_SHOWN);
    if (!window) {
        printf("Window creation failed: %s\n", SDL_GetError());
        return 1;
    }

    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (!renderer) {
        printf("Renderer creation failed: %s\n", SDL_GetError());
        return 1;
    }

    init_game();

    // Game loop
    while (game_running) {
        handle_input();
        update_game();
        render_game();
        SDL_Delay(16); // Cap at roughly 60 FPS
    }

    // Cleanup
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    printf("Game Over! Final Score: %d\n", score);
    return 0;
} 