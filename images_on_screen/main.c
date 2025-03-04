#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#include "SDL.h"
#include "SDL_events.h"
#include "SDL_image.h"

const int SCREEN_WIDTH = 600;
const int SCREEN_HEIGHT = 400;

typedef struct State
{
    SDL_Window *window;
    SDL_Renderer *renderer;
    bool is_running;
    SDL_Rect square;
    SDL_Texture *texture;
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

SDL_Texture *load_texture(const char *path)
{
    SDL_Texture *texture = NULL;
    SDL_Surface *surface = IMG_Load(path);
    if (surface == NULL)
    {
        SDL_Log("Load image failed %s\n", IMG_GetError());
        return NULL;
    }
    texture = SDL_CreateTextureFromSurface(state.renderer, surface);
    SDL_FreeSurface(surface);
    return texture;
}

bool load_media(void)
{
    state.texture = load_texture("assets/graphics/player.png");
    if (state.texture == NULL)
    {
        SDL_Log("Load player texture failed %s\n", IMG_GetError());
        return false;
    }

    return true;
}

void cleanup()
{
    SDL_DestroyRenderer(state.renderer);
    SDL_DestroyWindow(state.window);
    state.renderer = NULL;
    state.window = NULL;
    SDL_Quit();
}

// Render resources.
void square_blit(void)
{
    SDL_SetRenderDrawColor(state.renderer, 0xFF, 0x00, 0xFF, 0xFF);
    // SDL_RenderCopy(state.renderer, SDL_Texture *texture, const SDL_Rect *srcrect, const SDL_Rect *dstrect)
    SDL_RenderFillRect(state.renderer, &state.square);
}

void texture_blit(SDL_Texture *texture)
{
    SDL_RenderCopy(state.renderer, texture, NULL, &state.square);
}

int main(int argv, char *argc[])
{
    memset(&state, 0, sizeof(state_t));

    state.is_running = init();

    if (!load_media())
    {
        SDL_Log("Load media failed.\n");
        exit(1);
    }

    atexit(cleanup);

    SDL_Event event;
    state.square = (SDL_Rect){
        .x = SCREEN_WIDTH / 2 - 50,
        .y = SCREEN_HEIGHT / 2 - 50,
        .w = 100,
        .h = 100,
    };

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
        // square_blit();
        texture_blit(state.texture);
        present();
    }

    return EXIT_SUCCESS;
}
