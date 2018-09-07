//
// Created by ayman on 8/29/18.
//

#include "Input.h"
#include <iostream>

uint8_t KEYMAP[16] = {
        SDLK_x,
        SDLK_1,
        SDLK_2,
        SDLK_3,
        SDLK_q,
        SDLK_w,
        SDLK_e,
        SDLK_a,
        SDLK_s,
        SDLK_d,
        SDLK_z,
        SDLK_c,
        SDLK_4,
        SDLK_r,
        SDLK_f,
        SDLK_v,
};

void Input::init() {

    if (SDL_Init(SDL_INIT_EVENTS) != 0){
        std::cout << "SDL_Init Error: " << SDL_GetError() << std::endl;
        return;
    }
}

void Input::setKeys(array<bool, 16>& keys) {
    SDL_Event e;
    while (SDL_PollEvent(&e) != 0) {
        if (e.type == SDL_QUIT) {
            quit = true;
        }
        if (e.type == SDL_KEYDOWN) {
            if (e.key.keysym.sym == SDLK_ESCAPE)
                quit = true;
            if (e.key.keysym.sym == SDLK_DOWN)
                speed += 200;
            if (e.key.keysym.sym == SDLK_UP)
                (speed > 200) ? speed -= 200: speed = 0;
            if (e.key.keysym.sym == SDLK_RCTRL)
                reset = true;
            for(int i = 0; i < keys.size(); i++) {
                if (e.key.keysym.sym == KEYMAP[i])
                    keys[i] = true;
            }
        }
        if (e.type == SDL_KEYUP) {
            for(int i = 0; i < keys.size(); i++) {
                if (e.key.keysym.sym == KEYMAP[i])
                    keys[i] = false;
            }
        }
    }
}