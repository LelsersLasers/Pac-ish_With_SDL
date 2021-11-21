#include <stdio.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_timer.h>
#include <SDL2/SDL_image.h>
#include <time.h>

#define WINDOW_WIDTH (640)
#define WINDOW_HEIGHT (480)

// speed in pixels/second
#define SPEED (200)
#define FPS (60)


int main(void)
{
    // attempt to initialize graphics and timer system
    if (SDL_Init(SDL_INIT_VIDEO|SDL_INIT_TIMER) != 0) {
        printf("error initializing SDL: %s\n", SDL_GetError());
        return 1;
    }

    SDL_Window* win = SDL_CreateWindow("Pac-ish", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WINDOW_WIDTH, WINDOW_HEIGHT, 0);
    if (!win) {
        printf("error creating window: %s\n", SDL_GetError());
        SDL_Quit();
	    return 1;
    }

    // create a renderer, which sets up the graphics hardware
    Uint32 render_flags = SDL_RENDERER_ACCELERATED;
    SDL_Renderer* rend = SDL_CreateRenderer(win, -1, render_flags);
    if (!rend) {
        printf("error creating renderer: %s\n", SDL_GetError());
        SDL_DestroyWindow(win);
        SDL_Quit();
        return 1;
    }


    // PLAYER
    // load the image into memory using SDL_image library function
    SDL_Surface* surfPlayer = IMG_Load("resources/pacman.png");
    if (!surfPlayer) {
        printf("error creating surface\n");
        SDL_DestroyRenderer(rend);
        SDL_DestroyWindow(win);
        SDL_Quit();
        return 1;
    }
    // load the image data into the graphics hardware's memory
    SDL_Texture* texPlayer = SDL_CreateTextureFromSurface(rend, surfPlayer);
    SDL_FreeSurface(surfPlayer);
    if (!texPlayer) {
        printf("error creating texture: %s\n", SDL_GetError());
        SDL_DestroyRenderer(rend);
        SDL_DestroyWindow(win);
        SDL_Quit();
        return 1;
    }
    // struct to hold the position and size of the sprite
    SDL_Rect rectPlayer;
    // get and scale the dimensions of texture
    SDL_QueryTexture(texPlayer, NULL, NULL, &rectPlayer.w, &rectPlayer.h);
    rectPlayer.w = 30;
    rectPlayer.h = 30;
    float posPlayer[] = {(WINDOW_WIDTH - rectPlayer.w) / 2, (WINDOW_HEIGHT - rectPlayer.h) / 2}; // {x, y}


    // GHOSTS
    // load the image into memory using SDL_image library function
    SDL_Surface* surfGhost = IMG_Load("resources/ghost.png");
    if (!surfGhost) {
        printf("error creating surface\n");
        SDL_DestroyRenderer(rend);
        SDL_DestroyWindow(win);
        SDL_Quit();
        return 1;
    }
    // load the image data into the graphics hardware's memory
    SDL_Texture* texGhost = SDL_CreateTextureFromSurface(rend, surfGhost);
    SDL_FreeSurface(surfGhost);
    if (!texGhost) {
        printf("error creating texture: %s\n", SDL_GetError());
        SDL_DestroyRenderer(rend);
        SDL_DestroyWindow(win);
        SDL_Quit();
        return 1;
    }

    float posGhostsArr[3][2]; // why
    SDL_Rect rectGhosts[3];
    for (int i = 0; i < 3; i++) {    
        // struct to hold the position and size of the sprite
        SDL_Rect rectGhost;
        // get and scale the dimensions of texture
        SDL_QueryTexture(texGhost, NULL, NULL, &rectGhost.w, &rectGhost.h);
        rectGhost.w = 30;
        rectGhost.h = 30;
        posGhostsArr[i][0] = 50 * i; // x
        posGhostsArr[i][1] = 50; // y
        rectGhosts[i] = rectGhost;
    }


    // player direction
    int dir = 0; // 1, 2, 3, 4 :: up, down, left, right

    // set to 1 when window close button is pressed
    int closeRequested = 0;
    
    // animation loop
    while (!closeRequested) {
        // process events
        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            switch (event.type) {
                case SDL_QUIT:
                    closeRequested = 1;
                    break;
                case SDL_KEYDOWN:
                    switch (event.key.keysym.scancode) {
                        case SDL_SCANCODE_W:
                            dir = 1;
                            break;
                        case SDL_SCANCODE_S:
                            dir = 2;
                            break;
                        case SDL_SCANCODE_A:
                            dir = 3;
                            break;
                        case SDL_SCANCODE_D:
                            dir = 4;
                            break;
                    }
                    break;
            }
        }

        // move the player
        switch (dir) {
            case 1:
                posPlayer[1] -= SPEED / FPS;
                break;
            case 2:
                posPlayer[1] += SPEED / FPS;
                break;
            case 3:
                posPlayer[0] -= SPEED / FPS;
                break;
            case 4:
                posPlayer[0] += SPEED / FPS;
                break;
        }

        // collision detection with bounds
        if (posPlayer[0] <= 0) posPlayer[0] = 0;
        if (posPlayer[1] <= 0) posPlayer[1] = 0;
        if (posPlayer[0] >= WINDOW_WIDTH - rectPlayer.w) posPlayer[0] = WINDOW_WIDTH - rectPlayer.w;
        if (posPlayer[1] >= WINDOW_HEIGHT - rectPlayer.h) posPlayer[1] = WINDOW_HEIGHT - rectPlayer.h;

        // move ghost
        // int difX = posPlayer[0] - posGhostsArr[0][0];
        // int difY = posPlayer[1] - posGhostsArr[0][1];
        // printf("difX: %d difY: %d\n", difX, difY);
        // if (abs(difX) > abs(difY)) {
        //     posGhostsArr[0][0] += difX / FPS;
        // }
        // else {
        //     posGhostsArr[0][1] += difY / FPS;
        // }

        // // set the positions in the struct
        // rectPlayer.x = (int) posPlayer[0];
        // rectPlayer.y = (int) posPlayer[1];

        // rectGhost.x = (int) posGhostsArr[0][0];
        // rectGhost.y = (int) posGhostsArr[0][1];
        // printf("X: %d Y: %d\n", rectGhost.x, rectGhost.y);

        // DRAW THE FRAME
        // clear the window
        SDL_RenderClear(rend);

        // draw the image to the window
        SDL_RenderCopy(rend, texPlayer, NULL, &rectPlayer);
        // for (int i = 0; i < 3; i++) {
        //     SDL_RenderCopy(rend, texGhost, NULL, &rectGhosts[i]);
        // }
        // SDL_RenderCopy(rend, texGhost, NULL, &rectGhosts[0]);
        
        SDL_RenderPresent(rend);

        // wait 1/FPSth of a second
        SDL_Delay(1000/FPS);
    }
    
    // clean up resources before exiting
    SDL_DestroyTexture(texPlayer);
    SDL_DestroyTexture(texGhost);
    SDL_DestroyRenderer(rend);
    SDL_DestroyWindow(win);
    SDL_Quit();
}
