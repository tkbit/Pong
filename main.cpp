#include<iostream>
#include <SDL.h>
#include "window.h"
#include "main.h"

// Global varibles 

enum Screen { Game = 0, MainMenu, SettingsMenu };

SDL_Surface* spriteSheet = SDL_LoadBMP("assects/spriteSheet.bmp");

const int version = 2022011519; // yyyymmddhh time format

bool programRunning = true; //false if trying to exit
int currentMenu = Screen::MainMenu;
int ballDirX = 1, ballDirY = 0;
int ballSpeed = 4;

SDL_Rect pixelRect, paddle1, paddle2, ball;

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

void gameReset() {

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

    if (keyboardState[SDL_SCANCODE_ESCAPE]) {
        currentMenu = Screen::MainMenu;
        gameReset();
    }

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
    }

}

void mainMenuTick() {

    SDL_Rect startButton;       //does a thing

    startButton.w = windowWidth / 4;
    startButton.h = windowHeight / 8;
    startButton.x = (windowWidth / 2) - startButton.w / 2;
    startButton.y = windowHeight / 2;

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

    SDL_RenderPresent(rendererMain);

}

void gameTick() {

    gameLogic();

    gameDraw();

}

// main function game start

int main(int argc, char* argv[]) {

    int targetFPSDelay = (int)((1 / targetFPS) * 1000);

    initSDL();


    if (!spriteSheet) {
        SDL_Log("Faild to load spriteSheet.bmp: %s", SDL_GetError());
        statusCode = 2;
    }


    gameReset();

    while (programRunning) {

        checkInputs();

        if (currentMenu == 1) mainMenuTick(); else gameTick();

        SDL_Delay(targetFPSDelay);

    }


    cleanUp();

    if (statusCode != 0) {
        std::cin.get();
    }

    return statusCode;
}