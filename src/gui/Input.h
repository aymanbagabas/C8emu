//
// Created by ayman on 8/29/18.
//

#ifndef C8EMU_INPUT_H
#define C8EMU_INPUT_H
#include "SDL.h"
#include <array>

using namespace std;

class Input {
public:
    bool quit = false;
    bool reset = false;
    int speed = 1200;
    void init();
    void setKeys(array<bool, 16>& keys);
};


#endif //C8EMU_INPUT_H
