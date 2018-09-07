//
// Created by Ayman on 8/20/2018.
//

#include "Emulator.h"
#include <vector>
#include <fstream>
#include <iostream>
#include <thread>

#define MAX_MEM 4096

Emulator::Emulator() {
    machine = new C8();
    display = new Display();
    input = new Input();
}

Emulator::~Emulator() {
    delete input;
    input = NULL;
    display->close();
    delete display;
    display = NULL;
    delete machine;
    machine = NULL;
}

void Emulator::init() {
    machine->init();
    display->init();
    input->init();
    // start run
    isRunning = true;
}

void Emulator::setDebug() {
    debug = machine->debug = true;
}

int Emulator::load(string file) {
    // read file
    ifstream f(file, ios::in|ios::binary|ios::ate);
    if (!f.good()) {
        cout << "Couldn't read '" << file << "'!" << endl;
        return 1;
    }
    vector<char> buffer;
    // file size
    int fsize = f.tellg();
    f.seekg(ios::beg);
    buffer.resize(fsize);
    // 512(0x200)
    if (fsize > MAX_MEM-512) {
        cout << "Out of memory! File is too big." << endl;
        return 1;
    }
    f.read(buffer.data(), fsize);
    f.close();
    // load program
    machine->load(buffer);
    return 0;
}

int Emulator::run(string file) {
    // initialize
    init();
    begin:
    if (file.empty()) {
        cout << "Error: no file path specified!" << endl;
        return 1;
    }
    if (load(file)) {
        cout << "Error: no rom loaded!" << endl;
        return 1;
    }
    // main loop
    while (isRunning) {
        // execute
        machine->executeOP();
        // graphics
        if (machine->drawFlag) {
            display->drawDisplay(machine->display);
            machine->drawFlag = false;
        }
        // sound
        if (machine->playSound) {
            // Beep!
            cout << "\a" << flush;
            machine->playSound = false;
        }
        // input
        input->setKeys(machine->keys);
        isRunning = !input->quit;
        // reset
        if (input->reset) {
            input->reset = false;
            this_thread::sleep_for(std::chrono::milliseconds(500));
            goto begin;
        }
        // delay
        this_thread::sleep_for(std::chrono::microseconds(input->speed));
    }
    return 0;
}