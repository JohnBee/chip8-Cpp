#include <SDL2\SDL.h>
#include <iostream>

#include "graphicsHandler.h"
#include "system.h"

//The window we'll be rendering to
SDL_Window * gWindow = NULL;
    
//The surface contained by the window
SDL_Surface* gScreenSurface = NULL;

SDL_Surface* gHelloWorld = NULL;

System * chip8 = NULL;

//Screen dimension constants
const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;
namespace GFX {
    bool init(System * sys){
        //Initialization flag
        bool success = true;
        chip8 = sys;
        //Initialize SDL
        if( SDL_Init( SDL_INIT_VIDEO ) < 0 )
        {
            std::cout << "SDL could not initialize! SDL_Error: " << SDL_GetError() << std::endl;
            success = false;
        }
        else
        {
            //Create window
            gWindow = SDL_CreateWindow( "SDL Tutorial", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN );
            if( gWindow == NULL )
            {
                std::cout << "Window could not be created! SDL_Error: " << SDL_GetError() << std::endl;
                success = false;
            }
            else
            {
                //Get window surface
                gScreenSurface = SDL_GetWindowSurface( gWindow );
            }
        }

        return success;
    }
    void renderLoop(){
        //Main loop flag
        bool quit = false;

        //Event handler
        SDL_Event e;
        //While application is running
                while( !quit )
                {
                    //Handle events on queue
                    while( SDL_PollEvent( &e ) != 0 )
                    {
                        //User requests quit
                        if( e.type == SDL_QUIT )
                        {
                            quit = true;
                        }
                    }
                    //Apply the image
                    SDL_Surface *screenSurface = SDL_GetWindowSurface(gWindow);

                    //Fill the surface white
                    SDL_FillRect(screenSurface, NULL, SDL_MapRGB(screenSurface->format, 0xFF, 0xFF, 0xFF));

                    //create a square
                    SDL_Rect rect;
                    rect.x = 0; rect.y = 0; rect.h = 100; rect.w = 100;
                    SDL_FillRect(screenSurface, &rect, SDL_MapRGB(screenSurface->format, 0x00, 0x00, 0x00));
                    SDL_BlitSurface( screenSurface, NULL, gScreenSurface, NULL );
                
                    //Update the surface
                    SDL_UpdateWindowSurface( gWindow );
                }
    }

    void close(){
        //Deallocate surface
        SDL_FreeSurface( gHelloWorld );
        gHelloWorld = NULL;

        //Destroy window
        SDL_DestroyWindow( gWindow );
        gWindow = NULL;

        //Quit SDL subsystems
        SDL_Quit();
    }
}
