#include <iostream>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_mixer.h>
using namespace std;
#include "player.hpp"
#define screenWidth 1280
#define screenHeight 800

const int FPS = 60;
const int frameDelay = 1000 / FPS;

int main(int argv, char **args)
{
    if (SDL_Init(SDL_INIT_EVERYTHING) > 0)
		std::cout << "SDL Init fail .error : " << SDL_GetError() << std::endl;
  	if (IMG_Init(IMG_INIT_PNG) == 0)
	  	std::cout << "SDL img fail to init .error : " << SDL_GetError() << std::endl;
    if (TTF_Init() != 0)
        std::cout << "SDL TTF fail to init .error : " << SDL_GetError() << std::endl;    
    SDL_Window *window = SDL_CreateWindow("test game", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, screenWidth, screenHeight, 0);
    if (window == NULL)
		std::cout << "SDL fail to create window .error : " << SDL_GetError() << std::endl;
    SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (renderer == NULL)
		std::cout << "SDL fail to create renderer . error : " << SDL_GetError() << std::endl;
    
    SDL_SetRenderDrawColor( renderer, 0, 0,255, 0 );
    SDL_RenderClear( renderer );
    
    Player player;
    player.location = 100;
    player.printloc();
    
    bool is_gameRunning = true;
	SDL_Event event;
	Uint32 frameStart;
	int frameTime;
    int mouthX,mouthY;
    bool mouthL;

    while (is_gameRunning)
    {
        frameStart = SDL_GetTicks();

        while (SDL_PollEvent(&event))
        {
            switch (event.type)
            {
            case SDL_QUIT:
                is_gameRunning = false;
                break;
            case SDL_MOUSEMOTION:
                SDL_GetMouseState(&mouthX , &mouthY);
                break;
            case SDL_MOUSEBUTTONDOWN:
                if(event.button.button == SDL_BUTTON_LEFT)
                    mouthL = true;
                break;
            case SDL_MOUSEBUTTONUP:
                if(event.button.button == SDL_BUTTON_LEFT)
                    mouthL = false;
                break;
            case SDL_KEYDOWN:
                if(event.key.keysym.sym == SDLK_q)
                    is_gameRunning = false;
                break;
            }
        }
        if(mouthL)
            cout<<"hello"<<endl;

        SDL_RenderPresent(renderer);

        frameTime = SDL_GetTicks() - frameStart;
		if (frameTime < frameDelay)
			SDL_Delay(frameDelay - frameTime);
    }
    
    return 0;
}

