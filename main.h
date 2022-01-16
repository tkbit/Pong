#pragma once
#include<iostream>
#include <SDL.h>

extern int currentMenu, player1Score, player2Score;
extern bool programRunning; //false if trying to exit
extern SDL_Rect pixelRect, paddle1, paddle2, ball;
extern SDL_Surface* spriteSheetSurface;
extern SDL_Texture* spriteSheetTexture;

enum Screen { Game = 0, MainMenu, SettingsMenu };

void textToScreen(std::string text, int textX, int textY, int size = 8, int spacing = -1);
void scoreReset();
void gameReset();