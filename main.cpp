#include <iostream>
#include <cmath>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_mixer.h>
using namespace std;
#include "player.hpp"
#include "ball.hpp"
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
    
    //Load resources
    SDL_Texture* stone_background = IMG_LoadTexture(renderer , "assest/stone_background.jpg");
    SDL_Texture* Red_marble = IMG_LoadTexture(renderer , "assest/red_marble.png");
    SDL_Texture* Green_marble = IMG_LoadTexture(renderer , "assest/green_marble.png");
    SDL_Texture* Blue_marble = IMG_LoadTexture(renderer , "assest/blue_marble.png");
    SDL_Texture* Yellow_marble = IMG_LoadTexture(renderer , "assest/yellow_marble.png");
    SDL_Texture* ADD = IMG_LoadTexture(renderer , "assest/Add.png");
    SDL_Texture* Cannon = IMG_LoadTexture(renderer , "assest/cannon.png");



    SDL_SetRenderDrawColor( renderer, 0, 0,255, 0 );
    SDL_RenderClear( renderer );
    

    Player player;
    player.creat(Cannon , 500 , 300 , 200 , 80 , 40 , 40 );
    string mode="login_menu";
    SDL_Rect fullScreen = {0 , 0 , screenWidth , screenHeight};

    bool is_gameRunning = true;
	SDL_Event event;
	Uint32 frameStart;
	int frameTime;
    SDL_Point mouth;
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
                SDL_GetMouseState(&mouth.x , &mouth.y);
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
        SDL_RenderClear(renderer);
        if(mode == "login_menu")
        {
            //SDL_RenderCopy(renderer , stone_background , NULL , &fullScreen);
            player.Draw(renderer,&mouth);
            //SDL_RenderCopyEx(renderer , Cannon , NULL , &test ,(atan2( mouth.y-(center.y+test.y) , mouth.y - (center.x+test.x))*180)/M_PI,&center , SDL_FLIP_NONE);
            SDL_SetRenderDrawColor(renderer , 255 , 0 , 0 ,255);
            SDL_RenderDrawPoint(renderer , 175,203);
            SDL_SetRenderDrawColor( renderer, 0, 0,255, 0 );
            //SDL_RenderCopy(renderer , Red_marble , NULL , &test);
        }
        //cout<<mouthX<<" "<<mouthY<<endl;

        SDL_RenderPresent(renderer);

        frameTime = SDL_GetTicks() - frameStart;
		if (frameTime < frameDelay)
			SDL_Delay(frameDelay - frameTime);
    }
    
    return 0;
}
