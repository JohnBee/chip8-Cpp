#ifndef SYSTEM_H
#define SYSTEM_H

#include <vector>
#include <iostream>
#include "romHandler.h"

class System {
    private:
        unsigned short opcode;
        unsigned char memory[4096] = {0};
        // 16 Registers
        unsigned char V[16];
        // Index Register
        unsigned short I;
        // Program Counter
        unsigned short pc;

        // counter 60hz. Count Down To Zero
        unsigned char delay_timer;
        unsigned char sound_timer;
        // return stack
        // unsigned short stack[16];
        std::vector<unsigned short>stack;
        // stack pointer
        unsigned short sp;

        unsigned char key[16];

        

    public:
        void opcodeHandler(unsigned short opcode);
        void unknownOpcode(unsigned short opcode);
        void initialise(romBuffer buffer);
        void cycle();
        void setKeys();
        bool drawFlag;
        // 64 x 32  : 2048 pixel display 
        unsigned char gfx[64 * 32];
        void keyUp(char key);
        void keyDown(char key);
};

#endif