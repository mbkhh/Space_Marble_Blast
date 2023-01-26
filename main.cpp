#include <iostream>
#include <cmath>
#include <vector>
#include <time.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_mixer.h>
using namespace std;
#define screenWidth 1280
#define screenHeight 800

const int FPS = 60;
const int frameDelay = 1000 / FPS;
enum ball_modes {RED , GREEN , BLUE , YELLOW};

#include "functions.hpp"
#include "timer.hpp"
#include "player.hpp"
#include "map.hpp"
#include "ball.hpp"


int main(int argv, char **args)
{
    srand(time(NULL));
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
    //SDL_Texture* stone_background = IMG_LoadTexture(renderer , "assest/stone_background.jpg");
    SDL_Texture* Red_marble = IMG_LoadTexture(renderer , "assest/red_marble.png");
    SDL_Texture* Green_marble = IMG_LoadTexture(renderer , "assest/green_marble.png");
    SDL_Texture* Blue_marble = IMG_LoadTexture(renderer , "assest/blue_marble.png");
    SDL_Texture* Yellow_marble = IMG_LoadTexture(renderer , "assest/yellow_marble.png");
    //SDL_Texture* ADD = IMG_LoadTexture(renderer , "assest/Add.png");
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
    SDL_SetRenderTarget(renderer , NULL);

    int balls_width = 50;
    Ball in_air_balls[20];
    int in_air_count = 0;
    int bullet_speed = 12;
    
    int balls_v = 2;
    int count_ball = 30;
    Ball balls[200];
    creat_start_balls(count_ball , balls , ma.total_lenght , balls_width , Red_marble , Green_marble , Blue_marble , Yellow_marble);
    balls[29].current_loc = 800;
    balls[29].leftConnnected = false;
    balls[28].rightConnnected = false;
    balls[28].current_loc = 560;
    balls[27].current_loc = 510;
    balls[27].leftConnnected = false;
    balls[26].rightConnnected = false;

    Ball bullet;
    make_cannon_ball(count_ball , balls , &bullet ,  balls_width , bullet_speed , &player , Red_marble , Green_marble , Blue_marble , Yellow_marble);
    
    Timer bullet_shoot;
    bool is_gameRunning = true;
	SDL_Event event;
	Uint32 frameStart;
	int frameTime;
    SDL_Point mouth;
    bool mouthL = false;

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

        handle_map_balls(count_ball , balls , balls_v , &ma);

        for(int i = 0 ; i < count_ball ; i++)
        {
            balls[i].Draw(renderer);            
        }



        if((mouthL &&in_air_count==0 )|| (mouthL && bullet.is_in_cannon && in_air_count < 20 && bullet_shoot.get_current_time() > 600))
        {
            bullet.shoot(&mouth);
            in_air_balls[in_air_count] = bullet;
            bullet_shoot.creat();
            in_air_count++;
            make_cannon_ball(count_ball , balls , &bullet ,  balls_width , bullet_speed , &player , Red_marble , Green_marble , Blue_marble , Yellow_marble);
        }

        bullet.update();
        bullet.Draw(renderer);
        int deleted_index = -1;

        for(int i = 0 ; i < in_air_count;i++)
        {
            in_air_balls[i].update();
            in_air_balls[i].Draw(renderer);
            if(in_air_balls[i].is_out())
                deleted_index = i;
            for(int j = 0 ; j < count_ball ; j++)
            {
                if(check_ball_collision(&in_air_balls[i] , &balls[j]))
                {
                    collision(balls ,&count_ball ,j , &in_air_balls[i] , &ma , balls_v , balls_width);
                    deleted_index = i;
                    break;
                }
            }
        }
        //cout<<count_ball<<endl;
        if(deleted_index != -1)
        {
            delete_ball(in_air_balls , in_air_count , deleted_index);
            in_air_count--;
        }

        SDL_RenderPresent(renderer);
        frameTime = SDL_GetTicks() - frameStart;
		if (frameTime < frameDelay){
            //cout<<frameDelay - frameTime<<endl;
            SDL_Delay(frameDelay - frameTime);
        }
    }
    
    return 0;
}