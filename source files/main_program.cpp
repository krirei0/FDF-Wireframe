#include <SDL2/SDL.h>
#include <iostream>
#include <stdio.h>
#include "Graphics.h"
#include "FdfParser.h"
#include "Projection.h"


int main(int argc, char* args[])
{
    double windowWidth = 640;
    double windowHeight = 480;

    std::vector<std::vector<Point>> worldPoints = ParseFdf("C:\\Users\\krire\\OneDrive\\Desktop\\cpp\\42.fdf");
    std::vector<std::vector<RotatedPoint>> rotatedPoint = RotatePoint(worldPoints, 0, 0, 0);
    std::vector<std::vector<ScrCoordinates>> isometricProjectionPoints = IsometricProjection(rotatedPoint);

    Boundaries boundarycoordinates = FindBoundaries(isometricProjectionPoints);
    double scaleFactor = FindScaleFactor(boundarycoordinates, windowWidth, windowHeight);
    std::vector<std::vector<ScrCoordinates>> scaledPoints = ApplyScale(isometricProjectionPoints, scaleFactor);
    Boundaries scaledcoordinates = FindBoundaries(scaledPoints);
    Center projectionCenter = FindCenter(scaledcoordinates);
    Translation translationCoordinates = CalculateTranslation(projectionCenter, windowWidth, windowHeight);
    std::vector<std::vector<ScrCoordinates>> translatedPoints = ApplyTranslation(scaledPoints, translationCoordinates);
    

    SDL_Init(SDL_INIT_VIDEO);
    SDL_Window* window = SDL_CreateWindow
    (
        "Render Example", SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED, 640, 480,
        SDL_WINDOW_SHOWN
    );

    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
    SDL_RenderClear(renderer);
    SDL_SetRenderDrawColor(renderer, 255, 214, 10, 255);

    DrawConnection(translatedPoints, renderer, scaledcoordinates);
    DrawZ(translatedPoints, renderer, scaledcoordinates);
    
   
    SDL_RenderPresent(renderer);
    SDL_Delay(7000);

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;

}