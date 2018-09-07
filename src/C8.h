#ifndef C8_H
#define C8_H
#include <vector>
#include <array>

using namespace std;

class C8 {
private:
    // CPU
    uint16_t opcode;      // Opcodes are two bytes
    array<uint8_t, 16> V;       // 16 registers
    uint16_t I;           // I register 16bits/2bytes
    uint16_t pc;          // Program counter 2bytes
    array<uint16_t, 16> stack;   // Stack is 16 levels
    uint16_t sp;          // Stack pointer
    uint8_t delay_timer;  // Delay timer
    uint8_t sound_timer;  // Sound timer 
    // Memory
    array<uint8_t, 4096> mem; // 4KB memory
    // Fontset
    array<uint8_t, 80> fontset;
    // Game data
    array<uint8_t, 4096> rom;

public:
    bool debug = false;
    // Graphics
    array<array<bool, 64>, 32> display; //[32][64];
    // Keys
    array<bool, 16> keys;
    C8();
    ~C8();
    void init();
    bool drawFlag = false;
    bool playSound = false;
    void executeOP();
    void clearDisplay();
    int load(const vector<char>& buf);
};

#endif
