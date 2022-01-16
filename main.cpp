#include<iostream>
#include <SDL.h>
#include "window.h"
#include "main.h"

// Global varibles 



SDL_Surface* spriteSheetSurface;
SDL_Texture* spriteSheetTexture;

const int version = 2022011603; // yyyymmddhh 24 hour time format

bool programRunning = true; //false if trying to exit
int currentMenu = Screen::MainMenu;
int ballDirX = 1, ballDirY = 0;
int ballSpeed = 4;

SDL_Rect pixelRect, paddle1, paddle2, ball, spriteSheetSprite, sprite;

int player1Score = 0, player2Score = 0;



void updatePaddles() {

    //move paddles y to the mouse cursor
    paddle1.y = mouse.y / pixelSizeY - paddle1.h / 2;
    //paddle2.y = mouse.y / pixelSizeY - paddle2.h / 2;
    paddle2.y = ball.y + ball.h / 2 - paddle2.h / 2;

    //keep paddles within the window
    if (paddle1.y > gameArrayY - paddle1.h) paddle1.y = gameArrayY - paddle1.h;
    else if (paddle1.y < 0) paddle1.y = 0;

    if (paddle2.y > gameArrayY - paddle2.h) paddle2.y = gameArrayY - paddle2.h;
    else if (paddle2.y < 0) paddle2.y = 0;

}

void scoreReset() {
    player1Score = 0;
    player2Score = 0;
}

void gameReset() {

    // sprite in the sprite sheet
    sprite.x = 100;
    sprite.y = 100;
    sprite.w = 8 * pixelSizeX;
    sprite.h = 8 * pixelSizeY;

    spriteSheetSprite.x = 0;
    spriteSheetSprite.y = 0;
    spriteSheetSprite.w = 8;
    spriteSheetSprite.h = 8;

    //game pixels
    pixelRect.x = 0;
    pixelRect.y = 0;
    pixelRect.w = pixelSizeX;
    pixelRect.h = pixelSizeY;

    //left paddle
    paddle1.y = 0;
    paddle1.w = 8;
    paddle1.h = 64;
    paddle1.x = paddle1.w;

    //right paddle
    paddle2.y = 0;
    paddle2.w = 8;
    paddle2.h = 64;
    paddle2.x = gameArrayX - 2 * paddle2.w;

    //ball
    ball.x = gameArrayX / 2;
    ball.y = gameArrayY / 2;
    ball.w = 8;
    ball.h = 8;

    updatePaddles();

}

// Game code
void gameLogic() {

    updatePaddles();

    // if ball hits a paddle then bounce
    if (
        ball.x + ball.w >= paddle2.x
        && ball.y  < paddle2.y + paddle2.h
        && ball.y  > paddle2.y
        && ball.x + ball.w < gameArrayX
        && ballDirX > 0
        || ball.x <= paddle1.w + paddle1.x
        && ball.y  < paddle1.y + paddle1.h
        && ball.y  > paddle1.y
        && ball.x  > 0
        && ballDirX < 0
        ) {
        ballDirX = ballDirX * -1;
        ballDirY = (int)(std::sin(mouse.y) * 6);
    }

    // if ball hits top or bottom of screen then bounce
    if (
        ball.y > gameArrayY - ball.h
        && ballDirY > 0
        || ball.y < 0
        && ballDirY < 0
        ) ballDirY = ballDirY * -1;


    // move ball
    ball.x = ball.x + ballDirX * ballSpeed;
    ball.y = ball.y + ballDirY;


    // reset if ball hits left or right edge of screen
    if (ball.x < 0) {
        gameReset();
        player2Score++;
    }else if (ball.x > gameArrayX) {
        gameReset();
        player1Score++;
        std::cout << player1Score << '\n';
    }

}

void textToScreen(std::string text, int textX, int textY, int size, int spacing) {

    sprite.x = textX;
    sprite.y = textY;
    sprite.w = size * pixelSizeX;
    sprite.h = size * pixelSizeY;

    for (int i = 0; i <= text.length(); i++) {

        spriteSheetSprite.x = 8 * (text[i] - 32);
        spriteSheetSprite.y = 0;

        while (spriteSheetSprite.x > 256) {
            spriteSheetSprite.x = spriteSheetSprite.x - 256;
            spriteSheetSprite.y = spriteSheetSprite.y + 8;
        }

        SDL_RenderCopy(rendererMain, spriteSheetTexture, &spriteSheetSprite, &sprite);
        sprite.x = sprite.x + sprite.w + spacing * pixelSizeX;
    }

}

void mainMenuTick() {

    SDL_Rect startButton, settingsButton;

    startButton.w = 130;
    startButton.h = 40;
    startButton.x = (windowWidth / 2) - startButton.w / 2;
    startButton.y = windowHeight / 2;

    settingsButton.w = 178;
    settingsButton.h = 34;
    settingsButton.x = startButton.x;
    settingsButton.y = startButton.y + startButton.h * 2;

    SDL_SetRenderDrawColor(rendererMain, 21, 21, 23, NULL);
    SDL_RenderClear(rendererMain);

    if (mouse.x >= startButton.x
        && mouse.x <= startButton.x + startButton.w
        && mouse.y >= startButton.y
        && mouse.y < startButton.y + startButton.h
        ) {

        SDL_SetRenderDrawColor(rendererMain, 89, 89, 94, NULL);
        
        if (mouseButtons & SDL_BUTTON_LEFT) {
            currentMenu = Screen::Game;
        }
        
    }
    else {
        SDL_SetRenderDrawColor(rendererMain, 49, 49, 44, NULL);
    }

    SDL_RenderFillRect(rendererMain, &startButton);

    /*
    if (mouse.x >= settingsButton.x
        && mouse.x <= settingsButton.x + settingsButton.w
        && mouse.y >= settingsButton.y
        && mouse.y < settingsButton.y + settingsButton.h
        ) {

        SDL_SetRenderDrawColor(rendererMain, 89, 89, 94, NULL);

        if (mouseButtons & SDL_BUTTON_LEFT) {
            currentMenu = Screen::SettingsMenu;
        }

    }
    else {
        SDL_SetRenderDrawColor(rendererMain, 49, 49, 44, NULL);
    }

    SDL_RenderFillRect(rendererMain, &settingsButton);
    */

    //textToScreen("THE QUICK BROWN FOX JUMPED OVER THE",0,0);
    //textToScreen("LAZY DOG", 0, 8 * pixelSizeY);
    textToScreen("PONG", 200, 200, 32, 0);
    textToScreen("START", startButton.x + 10, startButton.y + 6);
    //textToScreen("SETINGS", settingsButton.x + 5, settingsButton.y + 5);

    SDL_RenderPresent(rendererMain);

}

void gameTick() {

    gameLogic();

    gameDraw();

}

void settingsMenuTick() {



}

// main function game start

int main(int argc, char* argv[]) {

    int targetFPSDelay = (int)((1 / targetFPS) * 1000);

    initSDL();

    spriteSheetSurface = SDL_LoadBMP("assects/spriteSheet.bmp"); // load sprite sheet to ram
    if (!spriteSheetSurface) {
        SDL_Log("Faild to load spriteSheet.bmp: %s", SDL_GetError());
        statusCode = 2;
    }

    spriteSheetTexture = SDL_CreateTextureFromSurface(rendererMain, spriteSheetSurface); // move sprite sheet from ram to vram
    if (!spriteSheetTexture) {
        SDL_Log("Faild to create texture: %s", SDL_GetError());
        statusCode = 2;
    }

    SDL_FreeSurface(spriteSheetSurface); // free ram of sprite sheet

    gameReset();

    if (statusCode != 0) programRunning = false;
    while (programRunning) {

        checkInputs();

        //if (currentMenu == 1) mainMenuTick(); else gameTick();

        switch (currentMenu){
        case Game:
            gameTick();
            break;

        case MainMenu:
            mainMenuTick();
            break;

        case SettingsMenu:
            settingsMenuTick();
            break;
        }

        SDL_Delay(targetFPSDelay);

    }


    cleanUp();

    if (statusCode != 0) {
        std::cin.get();
    }

    return statusCode;
}