#pragma GCC diagnostic ignored "-Wunknown-pragmas"

#include <stdlib.h>
#include <SDL2/SDL.h>

SDL_Window *Window;
SDL_Surface *screen;


int main(int argc, char *argv[]) {

    (void)argc;
    (void)argv;

    // automaticly initilize event subsysem
    SDL_Init(SDL_INIT_VIDEO);

    Window = SDL_CreateWindow("", SDL_WINDOWPOS_CENTERED,
                              SDL_WINDOWPOS_CENTERED, 200, 200,
                              SDL_WINDOW_HIDDEN);
    screen = SDL_GetWindowSurface(Window);

    // SDL window flag
    SDL_SetWindowSize(Window, 200, 200);
    SDL_SetWindowTitle(Window, "WIN");
    SDL_ShowWindow(Window);
    SDL_SetWindowPosition(Window, SDL_WINDOWPOS_CENTERED,
                          SDL_WINDOWPOS_CENTERED);


    Uint32 black = SDL_MapRGB(screen->format, 133, 133, 133);

    // Gives every pixel the same color
    SDL_FillRect(screen, NULL, black);


    SDL_Event event;
    int running=1;

    SDL_UpdateWindowSurface(Window);

    // Loop to catch the quit event
    while (running){
        // Consuming events
        while (SDL_PollEvent(&event))
            if (event.type == SDL_QUIT)
                running=0;

        // Wait a number of milliseconds before returning
        SDL_Delay(16);
    }

    // Clean up before exit
    SDL_FreeSurface(screen);
    SDL_DestroyWindow(Window);
    SDL_Quit();
    return EXIT_SUCCESS;
}
