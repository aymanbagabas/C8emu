//
// Created by Ayman on 8/20/2018.
//

#ifndef C8EMU_EMULATOR_H
#define C8EMU_EMULATOR_H

#include "C8.h"
#include "Input.h"
#include "Display.h"
#include <string.h>

class Emulator {
public:
    Emulator();
    ~Emulator();
    int load(string file);
    int run(string file);
    void setGfx(int x, int y);
    bool getGfx(int x, int y);
    const array<array<bool, 64>, 32> getDisplay();
    void setKey(int x);
    bool getKey(int x);
    void setDebug();
    void stop();
private:
    C8* machine;
    Input* input;
    Display* display;
    void init();
    bool isRunning = false;
    bool isLoaded;
    bool debug = false;
};

#endif //C8EMU_EMULATOR_H
