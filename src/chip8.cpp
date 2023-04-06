#include <iostream>
#include <SDL2\SDL.h>
#include <fstream>
#include <exception>
#include <vector>
#include <memory>

#include "system.h"
#include "romHandler.h"
#include "graphicsHandler.h"

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
    // chip8.initialise(rb);
    delete(rb.buffer); // we're done with the buffer clean up;

    GFX::init(&chip8);
    GFX::renderLoop();
    GFX::close();

    if(chip8.drawFlag){
        // Draw graphics
    }
    std::cout << argv << std::endl;
    std::cout << args[0] << std::endl;


    return 0;
}