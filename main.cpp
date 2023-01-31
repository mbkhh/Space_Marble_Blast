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
enum ball_modes
{
    RED,
    GREEN,
    BLUE,
    YELLOW,
    Black,
    Question
};
enum time_modes
{
    NONE,
    SLOWMO,
    STOP,
    REVERSE
};

#include "functions.hpp"
#include "timer.hpp"
#include "player.hpp"
#include "map.hpp"
#include "ball.hpp"
#include "keyboard_handler.hpp"
#include "button.hpp"

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
    SDL_Texture *Red_marble = IMG_LoadTexture(renderer, "assest/red_marble.png");
    SDL_Texture *Green_marble = IMG_LoadTexture(renderer, "assest/green_marble.png");
    SDL_Texture *Blue_marble = IMG_LoadTexture(renderer, "assest/blue_marble.png");
    SDL_Texture *Yellow_marble = IMG_LoadTexture(renderer, "assest/yellow_marble.png");
    SDL_Texture *Red_marble_ice = IMG_LoadTexture(renderer, "assest/red_marble_ice.png");
    SDL_Texture *Green_marble_ice = IMG_LoadTexture(renderer, "assest/green_marble_ice.png");
    SDL_Texture *Blue_marble_ice = IMG_LoadTexture(renderer, "assest/blue_marble_ice.png");
    SDL_Texture *Yellow_marble_ice = IMG_LoadTexture(renderer, "assest/yellow_marble_ice.png");
    SDL_Texture *Black_marble = IMG_LoadTexture(renderer, "assest/black_marble.png");
    SDL_Texture *Question_marble = IMG_LoadTexture(renderer, "assest/question_marble.png");
    SDL_Texture *stone_background = IMG_LoadTexture(renderer, "assest/stone_background.jpg");
    SDL_Texture *Cannon = IMG_LoadTexture(renderer, "assest/cannon.png");
    SDL_Texture *PathTex = IMG_LoadTexture(renderer, "assest/path2.png");
    SDL_Texture *Bomb_power_button_norm = IMG_LoadTexture(renderer, "assest/change_norm.png");
    SDL_Texture *Bomb_power_button_selected = IMG_LoadTexture(renderer, "assest/change_selected.png");
    SDL_Texture *Rainbow_power_button_norm = IMG_LoadTexture(renderer, "assest/change_norm.png");
    SDL_Texture *Rainbow_power_button_selected = IMG_LoadTexture(renderer, "assest/change_selected.png");
    SDL_Texture *Fireball_power_button_norm = IMG_LoadTexture(renderer, "assest/change_norm.png");
    SDL_Texture *Fireball_power_button_selected = IMG_LoadTexture(renderer, "assest/change_selected.png");
    SDL_Texture *Bomb = IMG_LoadTexture(renderer, "assest/bomb.png");
    SDL_Texture *Rainbow = IMG_LoadTexture(renderer, "assest/bomb.png");
    SDL_Texture *Fireball = IMG_LoadTexture(renderer, "assest/bomb.png");
    SDL_Texture *Slowmo_power = IMG_LoadTexture(renderer, "assest/slowmo_power.png");
    SDL_Texture *Stop_power = IMG_LoadTexture(renderer, "assest/pause_power.png");
    SDL_Texture *Reverse_power = IMG_LoadTexture(renderer, "assest/reverse_power.png");

    TTF_Font* arial_font;
    arial_font = TTF_OpenFont("assest/arial.ttf", 24);

    SDL_Texture *BackGround = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_UNKNOWN, SDL_TEXTUREACCESS_TARGET, screenWidth, screenHeight);
    //SDL_SetRenderDrawColor( renderer, 0, 0,255, 0 );
    //SDL_RenderClear( renderer );

    string mode = "login_menu";
    string game_mode = "normal";
    SDL_Rect fullScreen = {0, 0, screenWidth, screenHeight};

    bool is_gameRunning = true;
    SDL_Event event;
    Uint32 frameStart;
    int frameTime;
    SDL_Point mouth;
    bool mouthL = false;
    Timer game_timer;
    int score;
    string username = "mohammad";
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
                SDL_GetMouseState(&mouth.x, &mouth.y);
                break;
            case SDL_MOUSEBUTTONDOWN:
                if (event.button.button == SDL_BUTTON_LEFT)
                    mouthL = true;
                break;
            case SDL_MOUSEBUTTONUP:
                if (event.button.button == SDL_BUTTON_LEFT)
                    mouthL = false;
                break;
            case SDL_KEYDOWN:
                if (event.key.keysym.sym == SDLK_q)
                    is_gameRunning = false;
                break;
            }
        }
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);

        if (mode == "login_menu")
        {
            SDL_RenderCopy(renderer, stone_background, NULL, NULL);
            if (mouthL)
                mode = "game";
        }
        else if(mode == "end_game")
        {
            SDL_Surface* text_surface;
            SDL_Texture* text_texture;
            SDL_Color text_color = {0, 0, 255 };
            SDL_Rect text_rect = {50 , 30 , 0 , 0 };

            text_surface = TTF_RenderText_Solid( arial_font, "username" , text_color );
            text_texture = SDL_CreateTextureFromSurface( renderer, text_surface );
            text_rect.w = text_surface->w;
            text_rect.h = text_surface->h;
            text_rect.x = 100;
            text_rect.y = 100;
            SDL_RenderCopy( renderer, text_texture , NULL , &text_rect );

            text_surface = TTF_RenderText_Solid( arial_font, "score" , text_color );
            text_texture = SDL_CreateTextureFromSurface( renderer, text_surface );
            text_rect.w = text_surface->w;
            text_rect.h = text_surface->h;
            text_rect.x = 400;
            text_rect.y = 100;
            SDL_RenderCopy( renderer, text_texture , NULL , &text_rect );

            text_surface = TTF_RenderText_Solid( arial_font, "time" , text_color );
            text_texture = SDL_CreateTextureFromSurface( renderer, text_surface );
            text_rect.w = text_surface->w;
            text_rect.h = text_surface->h;
            text_rect.x = 600;
            text_rect.y = 100;
            SDL_RenderCopy( renderer, text_texture , NULL , &text_rect );

            text_color = {200, 0, 0 };

            text_surface = TTF_RenderText_Solid( arial_font, username.c_str() , text_color );
            text_texture = SDL_CreateTextureFromSurface( renderer, text_surface );
            text_rect.w = text_surface->w;
            text_rect.h = text_surface->h;
            text_rect.x = 100;
            text_rect.y = 160;
            SDL_RenderCopy( renderer, text_texture , NULL , &text_rect );

            text_surface = TTF_RenderText_Solid( arial_font, to_string(score).c_str() , text_color );
            text_texture = SDL_CreateTextureFromSurface( renderer, text_surface );
            text_rect.w = text_surface->w;
            text_rect.h = text_surface->h;
            text_rect.x = 400;
            text_rect.y = 160;
            SDL_RenderCopy( renderer, text_texture , NULL , &text_rect );

            text_surface = TTF_RenderText_Solid( arial_font, game_timer.get_current_time_minute_last().c_str() , text_color );
            text_texture = SDL_CreateTextureFromSurface( renderer, text_surface );
            text_rect.w = text_surface->w;
            text_rect.h = text_surface->h;
            text_rect.x = 600;
            text_rect.y = 160;
            SDL_RenderCopy( renderer, text_texture , NULL , &text_rect );

        }
        else if (mode == "game")
        {
            bool is_ingame = true;
            Player player;
            player.creat(Cannon, screenWidth / 2, 300, 200, 80, 40, 40);
            map ma;
            ma.tex = PathTex;
            ma.p1 = {50, screenHeight - 100};
            ma.p2 = {screenWidth / 2 - 200, screenHeight / 2};
            ma.p3 = {0, 0};
            ma.p4 = {screenWidth / 4, 100};
            ma.p6 = {screenWidth, 0};
            ma.p5 = {screenWidth / 2, 200};
            ma.p7 = {screenWidth - 100, screenHeight - 100};

            SDL_SetRenderTarget(renderer, BackGround);
            SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
            SDL_RenderClear(renderer);
            ma.draw_path(renderer);
            SDL_SetRenderTarget(renderer, NULL);

            int balls_width = 50;
            Ball in_air_balls[20];
            int in_air_count = 0;
            int bullet_speed = 12;
            int current_time_mode = NONE;

            double normal_speed = 1;
            double balls_v = 1;
            int count_ball = 30;
            Ball balls[200];
            creat_start_balls(count_ball, balls, ma.total_lenght, balls_width, Red_marble, Green_marble, Blue_marble, Yellow_marble, Red_marble_ice, Green_marble_ice, Blue_marble_ice, Yellow_marble_ice, Black_marble, Question_marble);
            balls[29].current_loc = 800;
            balls[28].time_effect_mode = REVERSE;
            balls[28].time_effect_timer.creat();
            balls[29].leftConnnected = false;
            balls[28].rightConnnected = false;
            balls[28].current_loc = 560;
            balls[27].current_loc = 510;
            balls[27].leftConnnected = false;
            balls[26].rightConnnected = false;

            Ball bullet;
            make_cannon_ball(count_ball, balls, &bullet, balls_width, bullet_speed, &player, Red_marble, Green_marble, Blue_marble, Yellow_marble);
            Ball bullet2;
            make_cannon_ball(count_ball, balls, &bullet2, balls_width, bullet_speed, &player, Red_marble, Green_marble, Blue_marble, Yellow_marble);

            Button bomb_power;
            bomb_power.create(Bomb_power_button_norm, Bomb_power_button_selected, screenWidth - 100, screenHeight / 2 - 100, 70, 50, 300);

            Button rainbow_pawer;
            rainbow_pawer.create(Rainbow_power_button_norm, Rainbow_power_button_selected, screenWidth - 100, screenHeight / 2, 70, 50, 300);

            Button fireball_power;
            fireball_power.create(Fireball_power_button_norm, Fireball_power_button_selected, screenWidth - 100, screenHeight / 2 - 200, 70, 50, 300);

            Timer bullet_shoot;
            Timer time_effect_timer;

            SDL_Surface* timer_surface;
            SDL_Texture* timer_texture;
            SDL_Color timer_color = { 255, 0, 0 };
            SDL_Rect timer_rect = {screenWidth - 100 , 30 , 0 , 0 };
            string current_time;

            SDL_Surface* score_surface;
            SDL_Texture* score_texture;
            SDL_Color score_color = {0, 255, 0 };
            SDL_Rect score_rect = {50 , 30 , 0 , 0 };

            Keyboard_handler game_keyboard;
            game_keyboard.delay = 500;
            int time_effect_chance[4] = {20000 , 1 , 1 , 1};
            int minute,second;

            score = 0;
            game_timer.creat();
            while (is_ingame)
            {
                frameStart = SDL_GetTicks();
                while (SDL_PollEvent(&event))
                {
                    switch (event.type)
                    {
                    case SDL_QUIT:
                        is_gameRunning = false;
                        is_ingame = false;
                        break;
                    case SDL_MOUSEMOTION:
                        SDL_GetMouseState(&mouth.x, &mouth.y);
                        break;
                    case SDL_MOUSEBUTTONDOWN:
                        if (event.button.button == SDL_BUTTON_LEFT)
                            mouthL = true;
                        break;
                    case SDL_MOUSEBUTTONUP:
                        if (event.button.button == SDL_BUTTON_LEFT)
                            mouthL = false;
                        break;
                    case SDL_KEYDOWN:
                        if (event.key.keysym.sym == SDLK_q)
                        {
                            is_ingame = false;
                        }
                        game_keyboard.keydown(&event);
                    case SDL_KEYUP:
                        game_keyboard.keyup(&event);
                        break;
                    }
                }
                SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
                SDL_RenderClear(renderer);
                SDL_RenderCopy(renderer, BackGround, NULL, NULL);
                player.Draw(renderer, &mouth);

                if(current_time_mode != NONE)
                    if(time_effect_timer.get_current_time() > 5000)
                        current_time_mode = NONE;

                if (current_time_mode == NONE)
                    balls_v = normal_speed;
                else if (current_time_mode == SLOWMO)
                    balls_v = normal_speed / 2;
                else if (current_time_mode == STOP)
                    balls_v = 0;
                else if (current_time_mode == REVERSE)
                    balls_v = -1 * normal_speed;
                handle_map_balls(count_ball, balls, balls_v, &ma);
                for (int i = 0; i < count_ball; i++)
                {
                    if(balls[i].time_effect_mode == NONE)
                        balls[i].Draw(renderer);
                    else if(balls[i].time_effect_mode == STOP)
                        balls[i].Draw(renderer , true , Stop_power);
                    else if(balls[i].time_effect_mode == SLOWMO)
                        balls[i].Draw(renderer , true , Slowmo_power);
                    else if(balls[i].time_effect_mode == REVERSE)
                        balls[i].Draw(renderer , true , Reverse_power);
                    if(balls[i].color == "Red" || balls[i].color == "Yellow" || balls[i].color == "Blue" || balls[i].color == "Green")
                    {
                        int timeE = random(time_effect_chance , 4);
                        if(timeE != 0 )
                        {
                            balls[i].time_effect_mode = timeE;
                            balls[i].time_effect_timer.creat();
                        }
                    }
                    if(game_mode == "normal")
                    {
                        if(balls[i].current_loc > ma.total_lenght)
                        {
                            is_ingame = false;
                            mode = "end_game";
                            game_timer.end();
                        }
                    }
                    //cout<<balls[i].leftConnnected<<balls[i].rightConnnected<<" ";
                }
                //cout<<endl;
                if (!bomb_power.is_inside(&mouth) && !rainbow_pawer.is_inside(&mouth) && !fireball_power.is_inside(&mouth) && ((mouthL && in_air_count == 0) || (mouthL && bullet.is_in_cannon && in_air_count < 20 && bullet_shoot.get_current_time() > 600)))
                {
                    bullet.shoot(&mouth);
                    in_air_balls[in_air_count] = bullet;
                    bullet_shoot.creat();
                    in_air_count++;
                    bullet = bullet2;
                    make_cannon_ball(count_ball, balls, &bullet2, balls_width, bullet_speed, &player, Red_marble, Green_marble, Blue_marble, Yellow_marble);
                }

                bullet.update();
                bullet.Draw(renderer);
                int deleted_index = -1;
                for (int i = 0; i < in_air_count; i++)
                {
                    in_air_balls[i].update();
                    in_air_balls[i].Draw(renderer);
                    if (in_air_balls[i].is_out())
                        deleted_index = i;
                    for (int j = 0; j < count_ball; j++)
                    {
                        if (check_ball_collision(&in_air_balls[i], &balls[j]))
                        {
                            collision(balls, &count_ball, j, &in_air_balls[i], &ma, balls_v, balls_width , &current_time_mode , &time_effect_timer);
                            deleted_index = i;
                            break;
                        }
                    }
                }
                if (game_keyboard.get_current() != '!')
                {
                    if (game_keyboard.curruntK == ' ')
                    {
                        Ball temp;
                        temp = bullet2;
                        bullet2 = bullet;
                        bullet = temp;
                    }
                }
                if (deleted_index != -1)
                {
                    delete_ball(in_air_balls, in_air_count, deleted_index);
                    in_air_count--;
                }
                bomb_power.Draw(renderer, &mouth);
                rainbow_pawer.Draw(renderer, &mouth);
                fireball_power.Draw(renderer, &mouth);

                if (mouthL)
                {
                    if (bomb_power.is_clicked(&mouth))
                    {
                        bullet.creat_cannon_ball(Bomb, "Bomb", &player, balls_width, bullet_speed);
                    }
                    if (rainbow_pawer.is_clicked(&mouth))
                    {
                        bullet.creat_cannon_ball(Rainbow, "Rainbow", &player, balls_width, bullet_speed);
                    }
                    if (fireball_power.is_clicked(&mouth))
                    {
                        bullet.creat_cannon_ball(Fireball, "Fireball", &player, balls_width, bullet_speed);
                        bullet2.creat_cannon_ball(Fireball, "Fireball", &player, balls_width, bullet_speed);
                    }
                }

                current_time = game_timer.get_current_time_minute();
                timer_surface = TTF_RenderText_Solid( arial_font, current_time.c_str() , timer_color );
                timer_texture = SDL_CreateTextureFromSurface( renderer, timer_surface );
                timer_rect.w = timer_surface->w;
                timer_rect.h = timer_surface->h;
                SDL_RenderCopy( renderer, timer_texture , NULL , &timer_rect );

                score_surface = TTF_RenderText_Solid( arial_font, to_string(score).c_str() , score_color );
                score_texture = SDL_CreateTextureFromSurface( renderer, score_surface );
                score_rect.w = score_surface->w;
                score_rect.h = score_surface->h;
                SDL_RenderCopy( renderer, score_texture , NULL , &score_rect );

                SDL_RenderPresent(renderer);
                frameTime = SDL_GetTicks() - frameStart;
                if (frameTime < frameDelay)
                    SDL_Delay(frameDelay - frameTime);
            }
        }

        SDL_RenderPresent(renderer);
        frameTime = SDL_GetTicks() - frameStart;
        if (frameTime < frameDelay)
        {
            //cout<<frameDelay - frameTime<<endl;
            SDL_Delay(frameDelay - frameTime);
        }
    }

    return 0;
}