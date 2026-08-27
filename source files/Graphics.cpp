#include <SDL2/SDL.h>
#include <iostream>
#include <stdio.h>
#include "Graphics.h"


Graphics::Graphics()
{
    Window = nullptr;
    Renderer = nullptr;

}

/*Graphics::~Graphics()
{
    Close();
};*/

bool Graphics::Init()
{
    //Initialization flag
    bool success = true;
    
    //Initialize SDL
    if (SDL_Init(SDL_INIT_VIDEO) < 0)
    {
        printf("SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
        success = false;
    }
    else
    {
        //Create window
        Window = SDL_CreateWindow("Renderer Example", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
        if (Window == nullptr)
        {
            printf("SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
            success = false;
        }
    }
    return success;

};

void Graphics::Close()
{
    //Destroy window
    SDL_DestroyWindow(Window);
    Window = nullptr;

    //Quit SDL subsystems
    SDL_Quit();

};

SDL_Window* Graphics::GetWindow()
{
    return Window;
};

SDL_Renderer* Graphics::GetRenderer()
{
    return Renderer;
};