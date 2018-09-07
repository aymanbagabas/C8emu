//
// Created by ayman on 8/29/18.
//

#include "Display.h"
#include <iostream>

struct Color {
    Uint8 R;
    Uint8 G;
    Uint8 B;
    Uint8 A;
};

Color colors[5] = {
        {0xFF, 0xFF, 0xFF, 0xFF},   // white
        {0xFF, 0x00, 0x00, 0xFF},   // red
        {0x00, 0xFF, 0x00, 0xFF},   // green
        {0x00, 0x00, 0xFF, 0xFF},   // blue
        {0x00, 0x00, 0x00, 0xFF}    // black
};

int WIDTH = 1280;
int HEIGHT = 720;

Display::Display() {
}

Display::~Display() {
    close();
}

void Display::init() {
    if (SDL_Init(SDL_INIT_VIDEO) != 0){
        std::cout << "SDL_Init Error: " << SDL_GetError() << std::endl;
        return ;
    }
    window = SDL_CreateWindow("C8emu", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, WIDTH, HEIGHT, SDL_WINDOW_SHOWN);
    if (window == NULL){
        std::cout << "SDL_CreateWindow Error: " << SDL_GetError() << std::endl;
        return ;
    }
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (renderer == NULL) {
        printf( "Renderer could not be created! SDL Error: %s\n", SDL_GetError() );
        return ;
    } else {
        SDL_SetRenderDrawColor( renderer, 0x00, 0x00, 0x00, 0xFF );
    }
}

void Display::close() {
//    SDL_FreeSurface(surface);
//    surface = NULL;
    SDL_DestroyTexture(texture);
    texture = NULL;
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    renderer = NULL;
    window = NULL;

    SDL_Quit();
}

void Display::drawDisplay(array<array<bool, 64>, 32>& display) {

    SDL_SetRenderDrawColor( renderer, 0x00, 0x00, 0x00, 0xFF );
    SDL_RenderClear(renderer);

    int w = WIDTH /64;
    int h = HEIGHT /32;
    for(int y = 0; y < display.size(); y++) {
        for(int x = 0; x < display[y].size(); x++) {
            if (display[y][x]) {
                SDL_Rect fillRect = {x*w, y*h, w, h};
                SDL_SetRenderDrawColor(renderer, colors[0].R, colors[0].G, colors[0].B, colors[0].A);
                SDL_RenderFillRect(renderer, &fillRect);
            }
        }
    }
    SDL_RenderPresent(renderer);

}