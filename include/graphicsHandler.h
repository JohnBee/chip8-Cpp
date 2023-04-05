#ifndef SDLHANDLER_H
#define SDLHANDLER_H

#include <SDL2/SDL.h>
#include "system.h"

extern SDL_Window* gWindow;
extern SDL_Surface* gScreenSurface;
extern SDL_Surface* gHelloWorld;

namespace GFX {
    bool init(System * sys);
    void renderLoop();
    void close();
}


#endif