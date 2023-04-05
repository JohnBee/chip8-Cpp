#include <SDL2\SDL.h>
#include <iostream>
#include <chrono>

#include "graphicsHandler.h"
#include "system.h"

//The window we'll be rendering to
SDL_Window * gWindow = NULL;
    
//The surface contained by the window
SDL_Surface* gScreenSurface = NULL;

SDL_Renderer* gRenderer = NULL;

System * chip8 = NULL;

//Screen dimension constants
const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 320;
const int xPixScale = SCREEN_WIDTH / 64;
const int yPixScale = SCREEN_HEIGHT / 32;

auto last = std::chrono::high_resolution_clock::now();


namespace GFX {
    void createPixel(size_t x, size_t y, SDL_Renderer* renderSurface){
        //create a square
        SDL_Rect rect;
        rect.x = x*xPixScale; rect.y = y*yPixScale;  rect.w = xPixScale; rect.h = yPixScale;
        // rect.x = 0; rect.y = 0; rect.h = 50; rect.w = 50;
        // SDL_FillRect(screenSurface, &rect, SDL_MapRGB(screenSurface->format, 0x00, 0x00, 0x00));
        SDL_SetRenderDrawColor( gRenderer, 255, 255, 255, 255 );
        SDL_RenderFillRect( renderSurface, &rect) ;
    }
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
            gWindow = SDL_CreateWindow( "CHIP-8 EMULATOR", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN );
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

        // We must call SDL_CreateRenderer in order for draw calls to affect this window.
        gRenderer = SDL_CreateRenderer(gWindow, -1, 0);

        // Select the color for drawing. It is set to red here.
        SDL_SetRenderDrawColor(gRenderer, 255, 0, 0, 255);

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
                    // execute cycles
                    // 500hz
                    auto now = std::chrono::high_resolution_clock::now();
                    std::chrono::duration<double, std::milli> elapsed = now - last;
                    if(elapsed.count() > 2){
                        chip8->cycle();
                        last = now;
                    }
                
                    //Apply the image
                    if(chip8->drawFlag){
                        SDL_Surface * screenSurface = SDL_GetWindowSurface(gWindow);
                        
                        // Clear the entire screen to our selected color.
                        SDL_SetRenderDrawColor( gRenderer, 0, 0, 0, 0 );
                        SDL_RenderClear(gRenderer);

                        // Up until now everything was drawn behind the scenes.
                        // This will show the new, red contents of the window.


                        //Fill the surface white
                        // SDL_FillRect(screenSurface, NULL, SDL_MapRGB(screenSurface->format, 0xFF, 0xFF, 0xFF));
                        
                        for(size_t y = 0; y < 32; y++){
                            for(size_t x = 0; x < 64; x++){
                                if(chip8->gfx[y*64 + x] != 0){
                                    createPixel(x,y, gRenderer);
                                }
                            }
                        }
                        chip8->drawFlag = 0;
                    }
                    
                    createPixel(41,63, gRenderer);
                    SDL_RenderPresent(gRenderer);
                    
                    // SDL_BlitSurface( screenSurface, NULL, gScreenSurface, NULL );
                
                    //Update the surface
                    //SDL_UpdateWindowSurface( gWindow );
                }
    }

    void close(){
        //Deallocate surface
        SDL_FreeSurface( gScreenSurface);
        gScreenSurface = NULL;


        //Destroy window
        SDL_DestroyWindow( gWindow );
        gWindow = NULL;

        //Quit SDL subsystems
        SDL_Quit();
    }
}
