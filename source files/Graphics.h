#pragma once

#include <SDL2/SDL.h>
#include <stdio.h>
#include <string>


class Graphics
{
    
public:
    Graphics();
    ~Graphics();

    SDL_Window* GetWindow();
    SDL_Renderer* GetRenderer();

    bool Init();
    void Close();

private:
    const int SCREEN_WIDTH = 640;
    const int SCREEN_HEIGHT = 480;

    SDL_Window* Window;
    SDL_Renderer* Renderer;

};
