#include "system.h"
#include "romHandler.h"


void System::initialise(romBuffer buffer){
    // init registers
    pc = 0x200;
    opcode = 0;
    I = 0;
    sp = 0;
    for(size_t i = 0; i < buffer.size; i++){
        memory[i + 0x200] = buffer.buffer[i];
    }


}
void System::cycle(){
    // get opcode
    opcode = memory[pc] << 8 | memory[pc+1];

    // decode opcode
    // execute opcode

    // update timers

}
void System::setKeys(){

}