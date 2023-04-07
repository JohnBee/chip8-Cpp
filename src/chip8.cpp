#include <iostream>
#include <SDL2\SDL.h>
#include <fstream>
#include <exception>
#include <vector>
#include <memory>

#include "system.hpp"
#include "romHandler.hpp"
#include "graphicsHandler.hpp"

using namespace std;



int main(int argv, char** args)
{
    //load game
    romBuffer rb;
    if(argv == 1){
        std::cout << "Missing ROM from Parameter." << std::endl;
        return 1;
    }
    else{
        rb = readROM(args[1]);
    }
    

    System chip8(rb);
    delete(rb.buffer); // we're done with the buffer clean up;

    GFX::init(&chip8);
    GFX::renderLoop();
    GFX::close();

    return 0;
}