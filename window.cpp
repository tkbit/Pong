#include<iostream>
#include <SDL.h>
#include "main.h"
#include "window.h"

int statusCode = 0; //0 = good, 1 = sdl_init faild
int windowSizeIn = 8, pixelSizeX = 3, pixelSizeY = 3;
float targetFPS = 60;

const int gameArrayX = 256, gameArrayY = 240;

int windowWidth = gameArrayX * pixelSizeX, windowHeight = gameArrayY * pixelSizeY;

SDL_Window* windowMain;
Uint32 windowMainID = NULL;
SDL_Renderer* rendererMain;
SDL_Rect pixelRectArray[gameArrayX * gameArrayY];

pos mouse;
Uint32 mouseButtons;
const Uint8* keyboardState;


void initSDL() {

    //initilize needed SDL functions
    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        SDL_Log("Unable to initialize SDL: %s", SDL_GetError());
        statusCode = 1;
        programRunning = false;
    }

    /*
    * 
    * float dpiH, dpiV, dpiD;
    * 
    if (SDL_GetDisplayDPI(0, &dpiD, &dpiH, &dpiV) != 0) {
        SDL_Log("Unable to get display dpi: %s", SDL_GetError());
    }
    else {
        pixelSizeX = (int)((dpiH * windowSizeIn) / gameArrayX);
        pixelSizeY = (int)((dpiH * windowSizeIn) / gameArrayY);
        windowWidth = gameArrayX * pixelSizeX;
        windowHeight = gameArrayY * pixelSizeY;
        SDL_Log("Display DPI: %f", dpiD);
    }

    */

    windowMain = SDL_CreateWindow(
        "Pong v0.3.1-alpha",                       // window title
        SDL_WINDOWPOS_UNDEFINED,           // initial x position
        SDL_WINDOWPOS_UNDEFINED,           // initial y position
        windowWidth,                               // width, in pixels
        windowHeight,                               // height, in pixels
        NULL                  // flags
    );
    if (windowMain == NULL) { // Check that the window was successfully created
        SDL_Log("Could not create window: %s\n", SDL_GetError());
        statusCode = 1;
        programRunning = false;
    }
    else {
        windowMainID = SDL_GetWindowID(windowMain);
        SDL_Log("Window Resoultion: %i , %i", windowWidth, windowHeight);
    }

    rendererMain = SDL_CreateRenderer(windowMain, -1, SDL_RENDERER_ACCELERATED);
    if (rendererMain == NULL) { //Check that thr renderer was successfully created
        SDL_Log("Could not create renderer: %s\n", SDL_GetError());
        statusCode = 1;
        programRunning = false;
    }

}

void checkInputs() {

    mouseButtons = SDL_GetMouseState(&mouse.x, &mouse.y);

    keyboardState = SDL_GetKeyboardState(NULL);

    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        switch (event.type) {

        case SDL_WINDOWEVENT: {
            if (event.window.windowID == windowMainID) {
                switch (event.window.event) {

                case SDL_WINDOWEVENT_SIZE_CHANGED: {
                    windowWidth = event.window.data1;
                    windowHeight = event.window.data2;
                    break;
                }

                case SDL_WINDOWEVENT_CLOSE: {
                    event.type = SDL_QUIT;
                    SDL_PushEvent(&event);
                    break;
                }

                }
            }
            break;
        }

        case SDL_QUIT: {
            programRunning = false;
            break;
        }
        }
    }
}

void gameDraw() {

    int pixelRectArrayIndex = 0;
    for (int y = 0; y < gameArrayY; y++) {
        for (int x = 0; x < gameArrayX; x++) {

            if (x >= ball.x && x < ball.x + ball.w && y >= ball.y && y < ball.y + ball.h
                || x >= paddle1.x && x < paddle1.x + paddle1.w && y >= paddle1.y && y < paddle1.y + paddle1.h
                || x >= paddle2.x && x < paddle2.x + paddle2.w && y >= paddle2.y && y < paddle2.y + paddle2.h
                ) {
                pixelRect.x = x * pixelSizeX;
                pixelRect.y = y * pixelSizeY;
                pixelRectArray[pixelRectArrayIndex] = pixelRect;
                pixelRectArrayIndex++;
            }
        }
    }

    SDL_SetRenderDrawColor(rendererMain, 31, 31, 33, NULL);
    SDL_RenderClear(rendererMain);

    SDL_SetRenderDrawColor(rendererMain, 89, 89, 94, NULL);
    SDL_RenderFillRects(rendererMain, pixelRectArray, pixelRectArrayIndex);

    SDL_RenderPresent(rendererMain);



}

void cleanUp() {

    programRunning = false;
    if(!spriteSheetSurface) SDL_FreeSurface(spriteSheetSurface);
    if(!spriteSheetTexture) SDL_DestroyTexture(spriteSheetTexture);
    if(!rendererMain) SDL_DestroyRenderer(rendererMain);
    if(!windowMain)SDL_DestroyWindow(windowMain);
    SDL_Quit();

}
