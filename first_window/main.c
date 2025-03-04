#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#include "SDL.h"
#include "SDL_events.h"

const int SCREEN_WIDTH = 600;
const int SCREEN_HEIGHT = 400;

typedef struct State
{
    SDL_Window *window;
    SDL_Renderer *renderer;
    bool is_running;
} state_t;

state_t state;

bool init(void)
{
    if (SDL_Init(SDL_INIT_VIDEO) < 0)
    {
        printf("SDL Initialization failed %s\n", SDL_GetError());
        return false;
    }

    state.window = SDL_CreateWindow(
        "SDL2 Window",
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        SCREEN_WIDTH,
        SCREEN_HEIGHT,
        SDL_WINDOW_SHOWN);
    if (state.window == NULL)
    {
        printf("SDL Window Creation Failed %s\n", SDL_GetError());
        return false;
    }

    state.renderer = SDL_CreateRenderer(state.window, -1, SDL_RENDERER_ACCELERATED);
    if (state.renderer == NULL)
    {
        printf("SDL Renderer Creation Failed %s\n", SDL_GetError());
        return false;
    }

    printf("game is running...\n");
    return true;
}

void render(void)
{
    SDL_SetRenderDrawColor(state.renderer, 190, 190, 190, 0xFF);
    SDL_RenderClear(state.renderer);
}

void present(void)
{
    // UPDATE/PRESENT THE SCREEN.
    SDL_RenderPresent(state.renderer);
}

void cleanup()
{
    SDL_DestroyRenderer(state.renderer);
    SDL_DestroyWindow(state.window);
    state.renderer = NULL;
    state.window = NULL;
    SDL_Quit();
}

int main(int argv, char *argc[])
{
    memset(&state, 0, sizeof(state_t));

    if (!init())
    {
        SDL_Log("Game initialization failed.\n");
        exit(1);
    }

    atexit(cleanup);

    state.is_running = true;
    SDL_Event event;

    // GAME LOOP.
    while (state.is_running)
    {
        // INPUT HANDLER.
        while (SDL_PollEvent(&event) != 0)
        {
            switch (event.type)
            {
                case SDL_QUIT:
                    state.is_running = false;
                    break;
                default:
                    break;
            }
        }
        render();
        // BLIT.
        present();
    }

    return EXIT_SUCCESS;
}
