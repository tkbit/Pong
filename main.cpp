#include<iostream>
#include <SDL.h>
#include "window.h"
#include "main.h"

// Global varibles 

bool programRunning = true; //false if trying to exit

int ballDirX = -1, ballDirY = 1;

SDL_Rect pixelRect, paddle1, paddle2, ball;

int PL1 = 0, PL2 = 0;

// sets up sdl

// Sets up Varibles 

void gameInit() {

    pixelRect.x = 0;
    pixelRect.y = 0;
    pixelRect.w = pixelSizeX;
    pixelRect.h = pixelSizeY;

    //left paddle

    paddle1.y = 2;
    paddle1.w = 1;
    paddle1.h = 20;
    paddle1.x = paddle1.w;

    //right paddle
    paddle2.y = 0;
    paddle2.w = 1;
    paddle2.h = 20;
    paddle2.x = gameArrayX - 2 * paddle2.w;

    //ball
    ball.x = gameArrayX / 2;
    ball.y = gameArrayY / 2;
    ball.w = 2;
    ball.h = 2;

}

// CODE

void gameLogic() {

    paddle1.y = mouse.y / pixelSizeY - paddle1.h / 2;
    paddle2.y = mouse.y / pixelSizeY - paddle2.h / 2;

    if (paddle1.y > gameArrayY - paddle1.h) paddle1.y = gameArrayY - paddle1.h;
    else if (paddle1.y < 0) paddle1.y = 0;

    if (paddle2.y > gameArrayY - paddle2.h) paddle2.y = gameArrayY - paddle2.h;
    else if (paddle2.y < 0) paddle2.y = 0;

    // ball hit PDL1 and PDL2 bounce
    if (
        //ball.x + ball.w > paddle2.x
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
        ballDirY = (std::rand() % 4) - 2;
    }

    // hit top oe bottom bounce
    if (
        ball.y > gameArrayY - ball.h
        && ballDirY > 0
        || ball.y < 0
        && ballDirY < 0
        ) ballDirY = ballDirY * -1;


    ball.x = ball.x + ballDirX;
    ball.y = ball.y + ballDirY;


    // reset 
    if (ball.x < 0) {
        gameInit();
        paddle1.y = mouse.y / pixelSizeY - paddle1.h / 2;
        paddle2.y = mouse.y / pixelSizeY - paddle2.h / 2;
        PL2++;
        std::cout << PL1 << PL2;
    }
    if (ball.x > gameArrayX) {
        gameInit();
        paddle1.y = mouse.y / pixelSizeY - paddle1.h / 2;
        paddle2.y = mouse.y / pixelSizeY - paddle2.h / 2;
        PL1++;
        std::cout << PL1 << PL2;

    }

}

// main function game start

int main(int argc, char* argv[]) {

    int targetFPSDelay = (int)((1 / targetFPS) * 1000);

    initSDL();
    gameInit();

    while (programRunning) {

        gameLogic();

        gameDraw();

        SDL_Delay(targetFPSDelay);

        checkInputs();


    }


    cleanUp();

    if (statusCode != 0) {
        std::cin.get();
    }

    return statusCode;
}