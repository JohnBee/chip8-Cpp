#ifndef SYSTEM_H
#define SYSTEM_H

class System {
    unsigned short opcode;
    unsigned char memory[4096];
    // 16 Registers
    unsigned char V[16];
    // Index Register
    unsigned short I;
    // Program Counter
    unsigned short pc;
    // 64 x 32  : 2048 pixel display 
    unsigned char gfx[64 * 32];
    // counter 60hz. Count Down To Zero
    unsigned char delay_timer;
    unsigned char sound_timer;
    // return stack
    unsigned short stack[16];
    // stack pointer
    unsigned short sp;

    unsigned char key[16];
};

#endif