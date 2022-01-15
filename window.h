#pragma once

struct pos {
    int x, y;
};

extern int windowSizeIn, pixelSizeX, pixelSizeY, statusCode, windowWidth, windowHeight;
extern float targetFPS;

extern const int gameArrayX, gameArrayY;

extern SDL_Renderer* rendererMain;
extern SDL_Window* windowMain;
extern Uint32 windowMainID, mouseButtons;
extern SDL_Rect pixelRectArray[];

extern pos mouse;
extern const Uint8* keyboardState;

void cleanUp();
void checkInputs();
void initSDL();
void gameDraw();