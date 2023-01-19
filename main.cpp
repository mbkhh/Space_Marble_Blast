#include <iostream>
#include <cmath>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_mixer.h>
using namespace std;
#include "player.hpp"
#include "map.hpp"
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
    SDL_Texture* PathTex = IMG_LoadTexture(renderer , "assest/path2.png");

    SDL_Texture* BackGround = SDL_CreateTexture(renderer , SDL_PIXELFORMAT_UNKNOWN , SDL_TEXTUREACCESS_TARGET , screenWidth , screenHeight);
    //SDL_SetRenderDrawColor( renderer, 0, 0,255, 0 );
    //SDL_RenderClear( renderer );
    

    Player player;
    player.creat(Cannon , screenWidth/2 , 300 , 200 , 80 , 40 , 40 );
    string mode="login_menu";
    SDL_Rect fullScreen = {0 , 0 , screenWidth , screenHeight};
    map ma;
    ma.tex = PathTex;
    ma.p1 = {50,screenHeight-100};
    ma.p2 = {screenWidth/2-200,screenHeight/2};
    ma.p3 = {0,0};
    ma.p4 = {screenWidth/4 ,100};
    ma.p6 = {screenWidth,0};
    ma.p5 = {screenWidth/2,200};
    ma.p7 = {screenWidth-100 , screenHeight-100};

    SDL_SetRenderTarget(renderer , BackGround);
    SDL_SetRenderDrawColor(renderer , 0 , 0 , 0 ,255);
    SDL_RenderClear(renderer);
    ma.draw_path(renderer);
    ma.find_distance_samples();
    SDL_SetRenderTarget(renderer , NULL);

    Ball ba;
    ba.tex = Red_marble;
    ba.color = "Red";
    ba.v = 2;
    ba.total_path = ma.total_lenght;
    ba.rect = {0,0,50,50};
    ba.current_loc = 0 ;

    Ball ba2;
    ba2.current_loc = 50 ;
    ba2.color = "Green";
    ba2.v = 2;
    ba2.total_path = ma.total_lenght;
    ba2.tex = Green_marble;
    ba2.rect = {0,0,50,50};

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
        SDL_SetRenderDrawColor(renderer , 0 , 0 , 0 ,255);
        SDL_RenderClear(renderer);
        SDL_RenderCopy(renderer , BackGround , NULL , NULL);
        if(mode == "login_menu")
        {
            player.Draw(renderer,&mouth);
        }
        ba.Draw(renderer , &ma);
        ba2.Draw(renderer , &ma);
        SDL_RenderPresent(renderer);

        frameTime = SDL_GetTicks() - frameStart;
		if (frameTime < frameDelay){
        	SDL_Delay(frameDelay - frameTime);
        }
    }
    
    return 0;
}
