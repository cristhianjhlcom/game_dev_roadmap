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
    struct
    {
        int w, h;
        SDL_Texture *texture;
        struct
        {
            int x, y;
        } pos;
    } player;
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

    if (!(IMG_Init(IMG_INIT_PNG) & IMG_INIT_PNG))
    {
        printf("SDL Image Initialization Failed %s\n", IMG_GetError());
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

// Resources.
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
    state.player.texture = load_texture("assets/graphics/player.png");
    if (state.player.texture == NULL)
    {
        SDL_Log("Load player texture failed %s\n", IMG_GetError());
        return false;
    }

    return true;
}

// Draw.
/*
void square_blit(void)
{
    SDL_SetRenderDrawColor(state.renderer, 0xFF, 0x00, 0xFF, 0xFF);
    // SDL_RenderCopy(state.renderer, SDL_Texture *texture, const SDL_Rect *srcrect, const SDL_Rect *dstrect)
    SDL_RenderFillRect(state.renderer, &state.square);
}
*/

void texture_blit(state_t state)
{
    SDL_Rect dest;
    dest.x = state.player.pos.x;
    dest.y = state.player.pos.y;
    dest.w = state.player.w;
    dest.h = state.player.h;
    SDL_RenderCopy(state.renderer, state.player.texture, NULL, &dest);
}

// Player inputs.
void mouse_handler(SDL_Event *event)
{
    printf("Mouse click x %d - y %d\n", event->button.x, event->button.y);
    state.player.pos.x = event->button.x;
    state.player.pos.y = event->button.y;
    state.player.w = 100;
    state.player.h = 100;
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
                case SDL_MOUSEBUTTONDOWN:
                    mouse_handler(&event);
                    break;
                default:
                    break;
            }
        }
        render();
        // square_blit();
        texture_blit(state);
        present();
    }

    return EXIT_SUCCESS;
}
