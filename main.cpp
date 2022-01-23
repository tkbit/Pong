#include<iostream>
#include<string>

#include <SDL.h>

#include "window.h"
#include "main.h"

// Global varibles 



SDL_Surface* spriteSheetSurface;
SDL_Texture* spriteSheetTexture;

const int version = 2022012121; // yyyymmddhh 24 hour time format

bool programRunning = true; //false if trying to exit
int currentMenu = Screen::MainMenu;
float ballDirX = 1, ballDirY = 0;
int ballSpeed = pixelSizeX;

SDL_Rect pixelRect, paddle1, paddle2, ball, spriteSheetSprite, sprite;

int player1Score = 0, player2Score = 0;



void updatePaddles() {

    //move paddle1 y to the mouse cursor
    //paddle1.y = mouse.y / pixelSizeY - paddle1.h / 2;
    //paddle2.y = ball.y + ball.h / 2 - paddle2.h / 2;

    if (paddle1.y > mouse.y / pixelSizeY - paddle1.h / 2) {
        paddle1.y -= pixelSizeY;
    }
    if (paddle1.y < mouse.y / pixelSizeY - paddle1.h / 2) {
        paddle1.y += pixelSizeY;
    }

    if (paddle2.y > ball.y + ball.h / 2 - paddle2.h / 2) {
        paddle2.y -= pixelSizeY;
    }
    if(paddle2.y < ball.y + ball.h / 2 - paddle2.h / 2) {
        paddle2.y += pixelSizeY;
    }

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
    //paddle1.y = 0;
    paddle1.w = 8;
    paddle1.h = 64;
    paddle1.x = paddle1.w;

    //right paddle
    //paddle2.y = 0;
    paddle2.w = 8;
    paddle2.h = 64;
    paddle2.x = gameArrayX - 2 * paddle2.w;

    //ball
    ball.x = gameArrayX / 2;
    ball.y = gameArrayY / 2;
    ball.w = 8;
    ball.h = 8;

    ballDirX = 1;
    ballDirY = 0;

    updatePaddles();

}

// Game code
void gameLogic() {

    updatePaddles();

    // if ball hits a paddle then bounce
    if (SDL_HasIntersection(&ball, &paddle1) || SDL_HasIntersection(&ball, &paddle2)) {
        ballDirX = ballDirX * -1;
        ballDirY = (float)std::sin(std::rand() + mouse.y) * 2;
    }

    // if ball hits top or bottom of screen then bounce
    int zero = 0, WW = gameArrayX, WH = gameArrayY;
    if (SDL_IntersectRectAndLine(&ball, &zero, &zero, &WW, &zero)
        || SDL_IntersectRectAndLine(&ball, &zero, &WH, &WW, &WH)
        ) ballDirY = ballDirY * -1;

    //ballSpeed = (float)(player1Score - player2Score) + 1.0f; // changing ball speed seems to have broken hit dection

    // move ball
    ball.x += (int)(ballDirX * ballSpeed);
    ball.y += (int)(ballDirY * ballSpeed);


    // reset if ball hits left or right edge of screen
    if (ball.x < 0) {
        gameReset();
        player2Score++;
    }else if (ball.x > gameArrayX) {
        gameReset();
        player1Score++;
    }

}

void textToScreen(std::string text, int textPosX, int textPosY, int size, int spacing) {

    sprite.x = textPosX;
    sprite.y = textPosY;
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

    SDL_Rect startButton, settingsButton, exitButton;

    startButton.w = 130;
    startButton.h = 40;
    startButton.x = (windowWidth / 2) - startButton.w / 2;
    startButton.y = windowHeight / 2;

    settingsButton.w = 178;
    settingsButton.h = 34;
    settingsButton.x = startButton.x;
    settingsButton.y = startButton.y + startButton.h * 2;

    exitButton.w = 110;
    exitButton.h = 34;
    exitButton.x = startButton.x;
    exitButton.y = settingsButton.y + settingsButton.h * 2;

    SDL_SetRenderDrawColor(rendererMain, 21, 21, 23, NULL);
    SDL_RenderClear(rendererMain);

    //start button
    if (SDL_PointInRect(&mouse, &startButton)) {
        SDL_SetRenderDrawColor(rendererMain, 89, 89, 94, NULL);
        if (mouseButtons & SDL_BUTTON_LEFT) {
            currentMenu = Screen::Game;
        }
    }
    else {
        SDL_SetRenderDrawColor(rendererMain, 49, 125, 44, NULL);
    }
    SDL_RenderFillRect(rendererMain, &startButton);

    //setting button
    if (SDL_PointInRect(&mouse, &settingsButton)) {
        SDL_SetRenderDrawColor(rendererMain, 89, 89, 94, NULL);
        if (mouseButtons & SDL_BUTTON_LEFT) {
            currentMenu = Screen::SettingsMenu;
        }
    }
    else {
        SDL_SetRenderDrawColor(rendererMain, 49, 49, 125, NULL);
    }
    SDL_RenderFillRect(rendererMain, &settingsButton);

    // exit button
    if (SDL_PointInRect(&mouse, &exitButton)) {
        SDL_SetRenderDrawColor(rendererMain, 89, 89, 94, NULL);
        if (mouseButtons & SDL_BUTTON_LEFT) {
            cleanUp();
        }
    }
    else {
        SDL_SetRenderDrawColor(rendererMain, 125, 49, 44, NULL);
    }
    SDL_RenderFillRect(rendererMain, &exitButton);

    textToScreen(std::to_string(version), 0, windowHeight - 8 * pixelSizeY);
    textToScreen("PONG", 200, 200, 32, 0);
    textToScreen("Start", startButton.x + 10, startButton.y + 6);
    textToScreen("Settings", settingsButton.x + 5, settingsButton.y + 5);
    textToScreen("Exit", exitButton.x + 5, exitButton.y + 5);
    SDL_RenderPresent(rendererMain);

}



void gameTick() {

    gameLogic();

    gameDraw();

}

void settingsMenuTick() {
    SDL_SetRenderDrawColor(rendererMain, 69, 69,69 , NULL);
    SDL_RenderClear(rendererMain);

    textToScreen("SETTINGS",150, 200, 20 );
    textToScreen("ESC FOR MAINMENU", 260, 265, 6);

    SDL_RenderPresent(rendererMain);
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