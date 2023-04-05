#include "system.h"
#include "romHandler.h"
#include <iomanip>
#include <stdlib.h>
#include <time.h>
#include <cstring>

unsigned char chip8_fontset[80] =
{ 
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

unsigned char lastRandom = 0;

// opcode handler
void System::unknownOpcode(unsigned short opcode){
    std::cout << "Unkown Opcode: " << std::setfill('0') 
                        << std::setw(2) 
                        << std::uppercase 
                        << std::hex << (0xFFFF & opcode) << std::endl;
}

void System::opcodeHandler(unsigned short opcode){
    // Fixed opcode
    // unknownOpcode(opcode);
    if(opcode == 0x00E0){
        // clear the screen;
        pc +=2;
        return;
    }
    if(opcode == 0x00EE){
        // return from subroutine
        pc = stack.back(); // stack is normall array[16] of short with counter? Want to try vector
        stack.pop_back();
        pc +=2;
        return;
    }
    if((opcode & 0xF000) == 0x0000){// 0NNN call subroutine?
        stack.push_back(pc);
        pc = opcode & 0x0FFF;
        return;
    } 
    // match opcode patterns
    if((opcode & 0xF000) == 0x1000){ // jump to address
        pc = opcode & 0x0FFF;
        return;
    }
    if((opcode & 0xF000) == 0x2000){ // call subroutine
        stack.push_back(pc);
        pc = opcode & 0x0FFF;
        return;
    }
    if((opcode & 0xF000) == 0x3000){ // skip next insutrction if VX == NN
        unsigned char vx = V[(opcode & 0x0F00) >> 8];
        unsigned char nn = (opcode & 0x00FF);
        if(vx == nn){
            pc+=4;
        }
        else{
            pc +=2;
        }
        return;
    }
    if((opcode & 0xF000) == 0x4000){ // skip next instruction if VX != NN
        unsigned char vx = V[(opcode & 0x0F00) >> 8];
        unsigned char nn = (opcode & 0x00FF);
        if(vx != nn){
            pc += 4;
        }
        else{
            pc +=2;
        }
        return;
    }
    if((opcode & 0xF00F) == 0x5000){ // skip next insutrction if VX == VY
        unsigned char vx = V[(opcode & 0x0F00) >> 8];
        unsigned char vy = V[(opcode & 0x00F0) >> 4];
        if(vx == vy){
            pc+=4;
        }
        else{
            pc +=2;
        }
        return;
    }
    if((opcode & 0xF000) == 0x6000){ // 6XNN set VX to NN
        unsigned char vx_address = (opcode & 0x0F00) >> 8;
        unsigned char nn = (opcode & 0x00FF);
        V[vx_address] = nn;
        pc+=2;
        return;
    }
    if((opcode & 0xF000) == 0x7000){ // 7XNN add NN to VX (carry flag unchanged)
        unsigned char vx_address = (opcode & 0x0F00) >> 8;
        unsigned char nn = (opcode & 0x00FF);
        V[vx_address] += nn;
        pc+=2;
        return;
    }
    if((opcode & 0xF000) == 0x8000){ //0x8XY(0 - E)
        switch (opcode & 0x000F){
            case 0x0000: { //0x8XY0 Sets VX to the value of VY.
                unsigned char vx = (opcode & 0x0F00) >> 8;
                unsigned char vy = (opcode & 0x00F0) >> 4;
                V[vx] = V[vy];
                pc+=2;
                return;
            } 
            break;
            case 0x0001:{ // 0x8XY1 sets VX to VX | VY
                unsigned char vx = (opcode & 0x0F00) >> 8;
                unsigned char vy = (opcode & 0x00F0) >> 4;
                V[vx] = V[vx] | V[vy];
                pc+=2;
                return;
            }
            break;
            case 0x0002:{ //0x8XY2 set VX to VX & VY
                unsigned char vx = (opcode & 0x0F00) >> 8;
                unsigned char vy = (opcode & 0x00F0) >> 4;
                V[vx] = V[vx] & V[vy];
                pc+=2;
                return;
            }
            break;
            case 0x0003:{ //0x8XY3 set VX to VX xor VY
                unsigned char vx = (opcode & 0x0F00) >> 8;
                unsigned char vy = (opcode & 0x00F0) >> 4;
                V[vx] = V[vx] ^ V[vy];
                pc+=2;
                return;
            }
            break;
            case 0x0004:{ //0x8XY4 set VX += VY w/ carry flag V[F]
                unsigned char vx = (opcode & 0x0F00) >> 8;
                unsigned char vy = (opcode & 0x00F0) >> 4;
                if(V[vy] > (0xFF - V[vx])){ //if VY is bigger than the different from current VX to 255, then we will overflow
                    V[0xF] = 1; // Set carry flag
                }
                else{
                    V[0xF] = 0;
                }
                V[vx] += V[vy];
                pc+=2;
                return;
            }
            break;
            case 0x0005:{ //0x8XY5 set VX -= VY w/ carry flag V[F] set 0 if borrow else 1
                unsigned char vx = (opcode & 0x0F00) >> 8;
                unsigned char vy = (opcode & 0x00F0) >> 4;
                if(V[vy] > V[vx]){ //if VY is bigger than VX then it will underflow and need to borrow
                    V[0xF] = 0; // borrow set carry flow to 0
                }
                else{
                    V[0xF] = 1;
                }
                V[vx] -= V[vy];
                pc+=2;
                return;
            }
            break;
            case 0x0006:{ //0x8XY6 store LSB of VX in VF, then shift VX to right by 1
                unsigned char vx = (opcode & 0x0F00) >> 8;
                // unsigned char vy = (opcode & 0x00F0) >> 4;
                V[0xF] = (V[vx] & 0x01); // store lsb of VX in VF
                V[vx] >>= 1; // right shift x by 1
                pc+=2;
                return;
            }
            break;
            case 0x0007:{ //0x8XY7 VX = VY - VX 
                unsigned char vx = (opcode & 0x0F00) >> 8;
                unsigned char vy = (opcode & 0x00F0) >> 4;
                if(V[vx] > V[vy]){
                    V[0xF] = 0; //borrow
                }
                else{
                    V[0xF] = 1; // no borrow
                }
                V[vx] = V[vy] - V[vx];
                pc+=2;
                return;
            }
            break;
            case 0x000E:{ //0x8XYE store msb of Vx to VF. left shift VX by 1.
                unsigned char vx = (opcode & 0x0F00) >> 8;
                // unsigned char vy = (opcode & 0x00F0) >> 4;
                V[0xF] = (V[vx] & 0x80) != 0;
                V[vx] <<= 1;
                pc+=2;
                return;
            }
            break;
            default:{
                unknownOpcode(opcode);
                pc+=2;
                return;
            }
        }
    }
    if((opcode & 0xF000) == 0x9000){ //0x9XY0 skip next instruction if VX != VY
        unsigned char vx = (opcode & 0x0F00) >> 8;
        unsigned char vy = (opcode & 0x00F0) >> 4;
        if(V[vx] != V[vy]){
            pc +=2;
        }
        pc +=2;
        return;
    }
    if((opcode & 0xF000) == 0xA000){ // 0xANNN Sets I to the address NNN
        unsigned short nnn = (opcode & 0x0FFF);
        I = nnn;
        pc +=2;
        return;
    }
    if((opcode & 0xF000) == 0xB000){ // 0xBNNN Jumps to the address NNN plus V0. 
        unsigned short nnn = (opcode & 0x0FFF);
        pc = nnn + V[0]; 
        return;
    }
    if((opcode & 0xF000) == 0xC000){ // 0xCXNN Sets VX to the result of a bitwise and operation on a random number (Typically: 0 to 255) and NN. 
        unsigned char vx = (opcode & 0x0F00) >> 8;
        unsigned char nn = (opcode & 0x00FF);
        std::srand (std::time(NULL) + lastRandom);
        unsigned char r = rand() % 255;
        lastRandom = r;
        V[vx] = nn & r;
        pc +=2;
        return;
    }
    if((opcode & 0xF000) == 0xD000){ // 0xDXYN draw sprite at coord (VX, VY) width of 8 pixels height of N, read from memory location I
        unsigned char vx = (opcode & 0x0F00) >> 8;
        unsigned char vy = (opcode & 0x00F0) >> 4;
        unsigned char n = (opcode & 0x000F);

        
        unsigned char x = V[vx];
        unsigned char y = V[vy];
        unsigned short pixelLine;
        unsigned char pixelStatus;
        V[0xF] = 0;
        for(char yline = 0; yline < n; yline++){
            pixelLine = memory[I + yline];
            for(char xline = 0; xline < 8 && (x + xline < 64); xline++){
                pixelStatus = pixelLine & (0x80 >> xline); // check pixels left to right
                if(pixelStatus != 0){
                    if((gfx[x + xline + (y + yline) * 64]) == 1){
                    // pixel changed from set to unset, therefore flag
                        gfx[x + xline + (y + yline) * 64] = 0;
                        V[0xF] = 1;
                    }
                    else{
                        gfx[x + xline + (y + yline) * 64] = 1;
                    }
                }
                
                
            }
        }
        drawFlag = true;
        pc +=2;
        return;
    }
    if((opcode & 0xF000) == 0xE000){    // 0xE contains key press checks
        if((opcode & 0x00FF) == 0x009E){ // 0xEX9E skip next instruction is key stored in VX is pressed
            unsigned char vx = (opcode & 0x0F00) >> 8;
            if(V[vx] != 0){
                pc +=2;
            }
            pc +=2;
            return;
        }
        if((opcode & 0x00FF) == 0x009E){ // 0xEXA1 skip next instruction is key stored in VX is not pressed
            unsigned char vx = (opcode & 0x0F00) >> 8;
            if(V[vx] == 0){
                pc +=2;
            }
            pc +=2;
            return;
        }
    }
    if((opcode & 0xF000) == 0xF000){ //0xF instruction blocks
        switch(opcode & 0x00FF){
            case (0x0007):{ //0xFX07 set VX to delay timer
                unsigned char vx = (opcode & 0x0F00) >> 8;
                V[vx] = delay_timer;
                pc +=2;
                return;
            }
            break;
            case (0x000A):{ //0xFX0A get key press, blocking operation, then store in vx
                unsigned char vx = (opcode & 0x0F00) >> 8;
                // V[vx] = keypress <- blocking keypress
                pc +=2;
                return;
            }
            break;
            case (0x0015):{ //0xFX15 Sets the delay timer to VX.
                unsigned char vx = (opcode & 0x0F00) >> 8;
                delay_timer = V[vx];
                pc +=2;
                return;
            }
            break;
            case (0x0018):{ //0xFX18 Sets the sound timer to VX.
                unsigned char vx = (opcode & 0x0F00) >> 8;
                sound_timer = V[vx];
                pc +=2;
                return;
            }
            break;
            case (0x001E):{ //0xFX1E add VXC to I. VF is not affected
                unsigned char vx = (opcode & 0x0F00) >> 8;
                I += V[vx];
                pc +=2;
                return;
            }
            break;
            case (0x0029):{ //0xFX29 Take the value in VX, convert it to the font
                unsigned char vx = (opcode & 0x0F00) >> 8;
                I = V[vx] * 5;
                pc +=2;
                return;
            }
            break;
            case (0x0033):{ //0xFX33 store BCD of VX in I, I+1, I+2
                unsigned char vx = (opcode & 0x0F00) >> 8;
                unsigned char value = V[vx];
                unsigned char hundreds = value / 100;
                value -= (hundreds*100);
                unsigned char tens = (value) / 10;
                value -= (tens*10);
                unsigned char ones = (value);
                memory[I] = hundreds;
                memory[I + 1] = tens;
                memory[I + 2] = ones; 
                pc +=2;
                return;
            }
            break;
            case (0x0055):{ // 0xFX55 dump registers from V0 to including Vx into memory
                unsigned char vx = (opcode & 0x0F00) >> 8;
                for(unsigned char i = 0; i <= vx; i++){
                    memory[I+i] = V[i];
                }
                pc += 2;
                return;
            }
            break;
            case (0x0065):{ // 0xFX65 reg_load from V0 to including Vx from memory
                unsigned char vx = (opcode & 0x0F00) >> 8;
                for(unsigned char i = 0; i <= vx; i++){
                    V[i] = memory[I+i];
                }
                pc += 2;
                return;
            }
            break;
            
            
            default: {
                unknownOpcode(opcode);
                pc+=2;
                return;
            }
        }
    }
}

void System::initialise(romBuffer buffer){
    // init registers
    pc = 0x200;
    opcode = 0;
    I = 0;
    sp = 0;

    memset(memory, 0, 4096);
    memset(gfx, 0, 64*32);
    // load fontset
    for(int i = 0; i < 80; ++i)
        memory[i] = chip8_fontset[i];	

    // load program into memory
    for(size_t i = 0; i < buffer.size; i++){
        memory[i + 0x200] = buffer.buffer[i];
    }


}
void System::cycle(){
    // get opcode
    opcode = memory[pc] << 8 | memory[pc+1];

    // decode opcode
    opcodeHandler(opcode);

    // update timers
    if(delay_timer > 0)
        delay_timer--;
    
    if(sound_timer > 0){
        if(sound_timer == 1){
            // play sound
        }
        sound_timer--;
    }

}
void System::setKeys(){

}