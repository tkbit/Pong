#pragma once
#include<iostream>
#include <SDL.h>

extern bool programRunning; //false if trying to exit
extern SDL_Rect pixelRect, paddle1, paddle2, ball;
extern SDL_Surface* spriteSheet;