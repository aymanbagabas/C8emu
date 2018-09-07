#include <iostream>
#include <vector>
#include <string>
#include "C8.h"

#define WIDTH 64
#define HEIGHT 32

using namespace std;

C8::C8() {
    fontset = {
      0xF0, 0x90, 0x90, 0x90, 0xF0, // 0
      0x20, 0x60, 0x20, 0x20, 0x70, // 1
      0xF0, 0x10, 0xF0, 0x80, 0xF0, // 2
      0xF0, 0x10, 0xF0, 0x10, 0xF0, // 3
      0x90, 0x90, 0xF0, 0x10, 0x10, // 4
      0xF0, 0x80, 0xF0, 0x10, 0xF0, // 5
      0xF0, 0x80, 0xF0, 0x90, 0xF0, // 6
      0xF0, 0x10, 0x20, 0x40, 0x40, // 7
      0xF0, 0x90, 0xF0, 0x90, 0xF0, // 8
      0xF0, 0x90, 0xF0, 0x10, 0xF0, // 9
      0xF0, 0x90, 0xF0, 0x90, 0x90, // A
      0xE0, 0x90, 0xE0, 0x90, 0xE0, // B
      0xF0, 0x80, 0x80, 0x80, 0xF0, // C
      0xE0, 0x90, 0x90, 0x90, 0xE0, // D
      0xF0, 0x80, 0xF0, 0x80, 0xF0, // E
      0xF0, 0x80, 0xF0, 0x80, 0x80  // F
    };
    rom.fill(0);
}

C8::~C8() {
}

void C8::init() {
    opcode = I = sp = delay_timer = sound_timer = 0;
    // reset memory
    mem.fill(0);
    // reset registers
    V.fill(0);
    // reset stack levels
    stack.fill(0);
    // reset display matrix
    clearDisplay();
    // reset keys
    keys.fill(false);
    // copy fontset to mem
    for (int i = 0; i < 80; i++) {
        mem[i] = fontset[i];
    }
    // init pc counter to 0x200
    pc = 0x200;
    // random seed
    srand(time(NULL));
}

int C8::load(const vector<char>& buf) {
    init();
    for(int i = 0; i < buf.size(); i++)
        rom[i] = (mem[i+0x200] = buf[i]);
    // load rom to memory starting from 0x200
    return 0;
}

void C8::clearDisplay() {
    for (auto &i : display)
        i.fill(false);
}

void C8::executeOP() {
    // opcode is a 2 bytes
    opcode = (mem[pc] << 8) + mem[pc+1];
    if (debug) {
        printf("OPCODE: 0x%02X%02X\tPC: 0x%04X\tI: 0x%04X\nV:\t", mem[pc], mem[pc + 1], pc, I);
        for(int i = 0; i < V.size(); i++) printf("[%X]: %X ", i, V[i]);
        printf("\nKeys:\t");
        for(int i = 0; i < keys.size(); i++) printf("[%d]: %d ", i, keys[i]);
        printf("\nDelay: %X\t Sound: %X\n", delay_timer, sound_timer);
    }
    switch (opcode >> 12) {
        case 0x0:
            switch (opcode) {
                // Clears the screen.
                case 0x00E0:
                    clearDisplay();
                    pc += 2;
                    drawFlag = true;
                    break;
                // Returns from a subroutine.
                case 0x00EE:
                    pc = stack[--sp];
                    pc += 2;
                    break;
                default:
                    // RCA 1802 instructions are not implemented
                    if (debug)
                        printf("Error: unknown instruction 0x%04X\n", opcode);
            }
            break;
        // Jumps to address NNN.
        case 0x1:
            pc = opcode & 0x0FFF;
            break;
        // Calls subroutine at NNN.
        case 0x2:
            stack[sp++] = pc;
            pc = opcode & 0x0FFF;
            break;
        // Skips the next instruction if VX equals NN. (Usually the next instruction is a jump to skip a code block)
        case 0x3:
            if (V[(opcode & 0x0F00) >> 8] == (opcode & 0x00FF))
                pc += 4;
            else
                pc += 2;
            break;
        // Skips the next instruction if VX doesn't equal NN. (Usually the next instruction is a jump to skip a code block)
        case 0x4:
            if (V[(opcode & 0x0F00) >> 8] != (opcode & 0x00FF))
                pc += 4;
            else
                pc += 2;
            break;
        // Skips the next instruction if VX equals VY. (Usually the next instruction is a jump to skip a code block)
        case 0x5:
            if (V[(opcode & 0x0F00) >> 8] == V[(opcode & 0x00F0) >> 4])
                pc += 4;
            else
                pc += 2;
            break;
        // Sets VX to NN.
        case 0x6:
            V[(opcode & 0x0F00) >> 8] = opcode & 0x00FF;
            pc += 2;
            break;
        // Adds NN to VX. (Carry flag is not changed)
        case 0x7:
            V[(opcode & 0x0F00) >> 8] += (opcode & 0x00FF);
            pc += 2;
            break;
        case 0x8:
            switch (opcode & 0x000F) {
                // Sets VX to the value of VY.
                case 0x0:
                    V[(opcode & 0x0F00) >> 8] = V[(opcode & 0x00F0) >> 4];
                    pc += 2;
                    break;
                // Sets VX to VX or VY. (Bitwise OR operation)
                case 0x1:
                    V[(opcode & 0x0F00) >> 8] |= V[(opcode & 0x00F0) >> 4];
                    pc += 2;
                    break;
                // Sets VX to VX and VY. (Bitwise AND operation)
                case 0x2:
                    V[(opcode & 0x0F00) >> 8] &= V[(opcode & 0x00F0) >> 4];
                    pc += 2;
                    break;
                // Sets VX to VX xor VY.
                case 0x3:
                    V[(opcode & 0x0F00) >> 8] ^= V[(opcode & 0x00F0) >> 4];
                    pc += 2;
                    break;
                // Adds VY to VX. VX is set to 1 when there's a carry, and to 0 when there isn't.
                case 0x4:
                    V[(opcode & 0x0F00) >> 8] += V[(opcode & 0x00F0) >> 4];
                    if(V[(opcode & 0x00F0) >> 4] > (0xFF - V[(opcode & 0x0F00) >> 8]))
                        V[0xF] = 1; //carry
                    else
                        V[0xF] = 0;
                    pc += 2;
                    break;
                // VY is subtracted from VX. if VX > VY, VX is set to 1 when there isn't a borrow, and 0 when there is.
                case 0x5:
                    if(V[(opcode & 0x0F00) >> 8] > (V[(opcode & 0x00F0) >> 4]))
                        V[0xF] = 1;
                    else
                        V[0xF] = 0; //borrow
                    V[(opcode & 0x0F00) >> 8] -= V[(opcode & 0x00F0) >> 4];
                    pc += 2;
                    break;
                // Stores the least significant bit of VX input VX and then shifts VX to the right by 1.
                case 0x6:
                    V[0xF] = V[(opcode & 0x0F00) >> 8] & 1;
                    V[(opcode & 0x0F00) >> 8] >>= 1;
                    pc += 2;
                    break;
                // Sets VX to VY minus VX. VX is set to 0 when there's a borrow, and 1 when there isn't.
                // If Vy > VX, then VX is set to 1, otherwise 0. Then VX is subtracted from Vy, and the results stored input VX.
                case 0x7:
                    if(V[(opcode & 0x00F0) >> 4] > (V[(opcode & 0x0F00) >> 8]))
                        V[0xF] = 1;
                    else
                        V[0xF] = 0; //borrow
                    V[(opcode & 0x0F00) >> 8] = V[(opcode & 0x00F0) >> 4] - V[(opcode & 0x0F00) >> 8];
                    pc += 2;
                    break;
                // Stores the most significant bit of VX input VX and then shifts VX to the left by 1.
                case 0xE:
                    V[0xF] = V[(opcode & 0x0F00) >> 8] >> 7;
                    V[(opcode & 0x0F00) >> 8] <<= 1;
                    pc += 2;
                    break;
                default:
                    if (debug)
                        printf("Error: unknown instruction 0x%04X\n", opcode);
            }
            break;
        // Skip next instruction if VX != Vy.
        case 0x9:
                if(V[(opcode & 0x0F00) >> 8] != (V[(opcode & 0x00F0) >> 4]))
                    pc += 4;
                else
                    pc += 2;
                break;
        // Set I = nnn.
        case 0xA:
            I = opcode & 0x0FFF;
            pc += 2;
            break;
        // Jump to location NNN + VX.
        case 0xB:
            pc = (opcode & 0x0FFF) + V[0x0];
            break;
        // Sets VX to the result of a bitwise and operation on a random number (Typically: 0 to 255) and NN.
        // VX = rand() & NN
        case 0xC:
            V[(opcode & 0x0F00) >> 8] = (rand() % 256) & (opcode & 0x00FF);
            pc += 2;
            break;
        // Display n-byte sprite starting at memory location I at (VX, Vy), set VX = collision.
        case 0xD:
            V[0xF] = 0;
            for (int j = 0; j < (opcode & 0x000F); j++) {
                uint8_t sprite = mem[I + j];
                for (int i = 0; i < 8; i++) {
                    int x = (V[(opcode & 0x0F00) >> 8] + i) % WIDTH;
                    int y = (V[(opcode & 0x00F0) >> 4] + j) % HEIGHT;
                    if  ((sprite & (0x80 >> i)) != 0) {
//                        V[0xF] = draw(x, y);
                        // if collision set VX to 1, otherwise to 0.
                        if (display[y][x])
                            V[0xF] = 1;
                        display[y][x] = !display[y][x];
                    }
                }
            }
            drawFlag = true;
            pc += 2;
            break;
        case 0xE:
            switch (opcode & 0x00FF) {
                // Skip next instruction if key with the value of VX is pressed.
                case 0x9E:
                    if (keys[V[(opcode & 0x0F00) >> 8]])
                        pc += 4;
                    else
                        pc += 2;
                    break;
                // Skip next instruction if key with the value of VX is not pressed.
                case 0xA1:
                    if (!keys[V[(opcode & 0x0F00) >> 8]])
                        pc += 4;
                    else
                        pc += 2;
                    break;
                default:
                    if (debug)
                        printf("Error: unknown instruction 0x%04X\n", opcode);
            }
            break;
        case 0xF:
            switch (opcode & 0x00FF) {
                // Set VX = delay timer value.
                case 0x07:
                    V[(opcode & 0x0F00) >> 8] = delay_timer;
                    pc += 2;
                    break;
                // Wait for a key press, store the value of the key input VX.
                case 0x0A: {
                    // wait for a keypress then store the value in VX
                    bool key_pressed = false;
                    for (int i = 0; i < keys.size() && !key_pressed; i++) {
                        if (keys[i]) {
                            V[(opcode & 0x0F00) >> 8] = i;
                            key_pressed = true;
                        }
                    }
                    if (!key_pressed) {
                        return;
                    }
                    pc += 2;
                    break;
                }
                // Set delay timer = VX.
                case 0x15:
                    delay_timer = V[(opcode & 0x0F00) >> 8];
                    pc += 2;
                    break;
                // Set sound timer = VX.
                case 0x18:
                    sound_timer = V[(opcode & 0x0F00) >> 8];
                    pc += 2;
                    break;
                // Set I = I + VX.
                case 0x1E:
                    // VF = 1 when overflow, 0 otherwise
                    if(I + V[(opcode & 0x0F00) >> 8] > 0xFFF)
                        V[0xF] = 1;
                    else
                        V[0xF] = 0;

                    I += V[(opcode & 0x0F00) >> 8];
                    pc += 2;
                    break;
                // Set I = location of sprite for digit VX.
                case 0x29:
                    I = V[(opcode & 0x0F00) >> 8] * 0x5;
                    pc += 2;
                    break;
                // Store BCD representation of VX input memory locations I, I+1, and I+2.
                case 0x33: {
                    uint8_t x = V[(opcode & 0x0F00) >> 8];
                    mem[I] = x / 100; // hundreds
                    mem[I + 1] = (x / 10) % 10; // tens
                    mem[I + 2] = (x % 100) % 10; // ones
                    pc += 2;
                    break;
                }
                // Store registers V0 through VX in memory starting at location I.
                case 0x55:
                    for(int i = 0; i <= ((opcode & 0x0F00) >> 8); i++)
                        mem[I + i] = V[i];

                    // On the original interpreter, when the
                    // operation is done, I = I + X + 1.
//                    I += ((opcode & 0x0F00) >> 8) + 1;
                    pc += 2;
                    break;
                // Read registers V0 through VX from memory starting at location I.
                case 0x65:
                    for(int i = 0; i <= ((opcode & 0x0F00) >> 8); i++)
                        V[i] = mem[I + i];

                    // On the original interpreter, when the
                    // operation is done, I = I + X + 1.
//                    I += ((opcode & 0x0F00) >> 8) + 1;
                    pc += 2;
                    break;
                default:
                    if (debug)
                        printf("Error: unknown instruction 0x%04X\n", opcode);
            }
            break;
        default:
            if (debug)
                printf("Error: unknown instruction 0x%04X\n", opcode);
    }
    // Update timers
    if (delay_timer > 0) --delay_timer;
    if (sound_timer > 0) {
        if (sound_timer == 1) {
            playSound = true;
        }
        --sound_timer;
    }
}

