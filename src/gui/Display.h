//
// Created by ayman on 8/29/18.
//

#ifndef PROJECT_DISPLAY_H
#define PROJECT_DISPLAY_H
#include "SDL.h"
#include <array>

using namespace std;

class Display {
public:
    Display();
    ~Display();
    SDL_Window* window = NULL;
//    SDL_Surface* surface;
    SDL_Texture* texture = NULL;
    SDL_Renderer* renderer = NULL;
    void drawDisplay(array<array<bool, 64>, 32>& display);
    void init();
    void close();
};


#endif //PROJECT_DISPLAY_H
