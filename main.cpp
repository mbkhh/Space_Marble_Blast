#include <iostream>
#include <cmath>
#include <time.h>
#include <fstream>
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
#include "music_player.hpp"
Music_player music_player;
#include "player.hpp"
#include "map.hpp"
#include "ball.hpp"
#include "keyboard_handler.hpp"
#include "button.hpp"
#include "inputbox.hpp"
#include "user.hpp"
#include "missile.hpp"

void end_game(string game_mode, int score, Timer *game_timer, int *point, int *prize, User users[] , int count_user , int current_user)
{
    game_timer->end();
    int prize_chances[5] = {5, 4, 3, 2, 1};
    *prize = random(prize_chances, 5);
    if (*prize == 0)
        users[current_user].fireball_power++;
    else if (*prize == 1)
        users[current_user].bomb_power++;
    else if (*prize == 2)
        users[current_user].lightning_power++;
    else if (*prize == 3)
        users[current_user].rainbow_power++;
    else if (*prize == 4)
        users[current_user].missile_power++;
    if (game_mode == "normal" )
    {
        *point = 150 * score;
        *point += 50 * (3 * 60 - (game_timer->last / 1000));
        if(*point > users[current_user].max_normal)
            users[current_user].max_normal = *point;
        
    }
    else if (game_mode == "timer")
    {
        *point = 150 * score;
        *point += 50 * (3 * 60 - (game_timer->last / 1000));
        if(*point > users[current_user].max_timer)
            users[current_user].max_timer = *point;
    }
    else if (game_mode == "stone")
    {
        *point = 2000 * score;
        *point += 50 * (3 * 60 - (game_timer->last / 1000));
        if(*point > users[current_user].max_stone)
            users[current_user].max_stone = *point;
    }
    else if (game_mode == "fly")
    {
        *point = 2000 * score;
        *point += 50 * (3 * 60 - (game_timer->last / 1000));
        if(*point > users[current_user].max_fly)
            users[current_user].max_fly = *point;
    }
    write_users(users , count_user);
}
void make_leaderboard(User users[] , int count_user , User leaderboard_user[])
{
    for(int i = 0 ; i < count_user ; i++)
    {
        leaderboard_user[i] = users[i];
        leaderboard_user[i].total_point = leaderboard_user[i].max_fly + leaderboard_user[i].max_timer + leaderboard_user[i].max_stone + leaderboard_user[i].max_normal; 
    }
    for(int i = 0 ; i < count_user ; i++)
    {
        for(int j  = i ; j < count_user ; j++)
        {
            if(leaderboard_user[j].total_point < leaderboard_user[j+1].total_point)
                swap(leaderboard_user[j] , leaderboard_user[j+1]);
        }
    }
}
int main(int argv, char **args)
{
    //Inits
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
    Mix_Init(MIX_INIT_MP3);
    if( Mix_OpenAudio( 44100, MIX_DEFAULT_FORMAT, 2, 2048 ) < 0 )
        cout<<"SDL_mixer could not initialize! SDL_mixer Error: "<< Mix_GetError()<<endl;

    //Load resources
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
    SDL_Texture *Stone_marble = IMG_LoadTexture(renderer, "assest/stone_marble.png");
    SDL_Texture *Fly_marble = IMG_LoadTexture(renderer, "assest/fly_marble.png");
    SDL_Texture *Fly_gone_marble = IMG_LoadTexture(renderer, "assest/fly_marble.png");
    SDL_Texture *stone_background = IMG_LoadTexture(renderer, "assest/stone_background.jpg");
    SDL_Texture *space_background = IMG_LoadTexture(renderer, "assest/space_background.jpg");
    SDL_Texture *space_background2 = IMG_LoadTexture(renderer, "assest/space_background2.jpg");
    SDL_Texture *space_background3 = IMG_LoadTexture(renderer, "assest/space_background3.jpg");
    SDL_Texture *frist_page_background = IMG_LoadTexture(renderer, "assest/frist_page_background.jpg");
    SDL_Texture *Cannon = IMG_LoadTexture(renderer, "assest/cannon.png");
    SDL_Texture *PathTex = IMG_LoadTexture(renderer, "assest/path2.png");
    SDL_Texture *Quit_button_norm = IMG_LoadTexture(renderer, "assest/quit_btn-1.png");
    SDL_Texture *Quit_button_selected = IMG_LoadTexture(renderer, "assest/quit_selected_btn.png");
    SDL_Texture *Login_button_norm = IMG_LoadTexture(renderer, "assest/login_btn-1.png");
    SDL_Texture *Login_button_selected = IMG_LoadTexture(renderer, "assest/login_btn_selected.png");
    SDL_Texture *Register_button_norm = IMG_LoadTexture(renderer, "assest/register_btn-1.png");
    SDL_Texture *Register_button_selected = IMG_LoadTexture(renderer, "assest/register_btn_selected.png");
    SDL_Texture *Leaderboard_button_norm = IMG_LoadTexture(renderer, "assest/leaderboard_btn-1.png");
    SDL_Texture *Leaderboard_button_selected = IMG_LoadTexture(renderer, "assest/leaderboard_selected_btn.png");
    SDL_Texture *Tryagain_button_norm = IMG_LoadTexture(renderer, "assest/tryagain_btn-1.png");
    SDL_Texture *Tryagain_button_selected = IMG_LoadTexture(renderer, "assest/tryagain_btn_selected.png");
    SDL_Texture *Choose_mode_button_norm = IMG_LoadTexture(renderer, "assest/choose_mode_btn-1.png");
    SDL_Texture *Choose_mode_button_selected = IMG_LoadTexture(renderer, "assest/choose_mode_btn_selected.png");
    SDL_Texture *Done_button_norm = IMG_LoadTexture(renderer, "assest/done_btn-1.png");
    SDL_Texture *Done_button_selected = IMG_LoadTexture(renderer, "assest/done_btn_selected.png");
    SDL_Texture *Setting_button_norm = IMG_LoadTexture(renderer, "assest/setting_btn-1.png");
    SDL_Texture *Setting_button_selected = IMG_LoadTexture(renderer, "assest/setting_selected_btn.png");
    SDL_Texture *Start_button_norm = IMG_LoadTexture(renderer, "assest/start_game_btn-1.png");
    SDL_Texture *Start_button_selected = IMG_LoadTexture(renderer, "assest/start_game_selected_btn.png");
    SDL_Texture *Logout_button_norm = IMG_LoadTexture(renderer, "assest/logout_btn-1.png");
    SDL_Texture *Logout_button_selected = IMG_LoadTexture(renderer, "assest/logout_selected_btn.png");
    SDL_Texture *Normal_mode_button_norm = IMG_LoadTexture(renderer, "assest/normal.png");
    SDL_Texture *Normal_mode_button_selected = IMG_LoadTexture(renderer, "assest/normal.png");
    SDL_Texture *Timer_mode_button_norm = IMG_LoadTexture(renderer, "assest/timer.png");
    SDL_Texture *Timer_mode_button_selected = IMG_LoadTexture(renderer, "assest/timer.png");
    SDL_Texture *Stone_mode_button_norm = IMG_LoadTexture(renderer, "assest/stone_marble.png");
    SDL_Texture *Stone_mode_button_selected = IMG_LoadTexture(renderer, "assest/stone_marble.png");
    SDL_Texture *Fly_mode_button_norm = IMG_LoadTexture(renderer, "assest/fly_marble.png");
    SDL_Texture *Fly_mode_button_selected = IMG_LoadTexture(renderer, "assest/fly_marble.png");
    SDL_Texture *Inputbox_norm = IMG_LoadTexture(renderer, "assest/input_box_norm.png");
    SDL_Texture *Inputbox_selected = IMG_LoadTexture(renderer, "assest/input_box_selected.png");
    SDL_Texture *Music_change_button_norm = IMG_LoadTexture(renderer, "assest/change_music_btn-1.png");
    SDL_Texture *Music_change_button_selected = IMG_LoadTexture(renderer, "assest/change_music_btn_selected.png");
    SDL_Texture *Musicoff_button_norm = IMG_LoadTexture(renderer, "assest/music_btn-1.png");
    SDL_Texture *Musicoff_button_selected = IMG_LoadTexture(renderer, "assest/music_btn_selected.png");
    SDL_Texture *Soundoff_button_norm = IMG_LoadTexture(renderer, "assest/sound_btn-1.png");
    SDL_Texture *Soundoff_button_selected = IMG_LoadTexture(renderer, "assest/sound_btn_selected.png");
    SDL_Texture *Help_button_norm = IMG_LoadTexture(renderer, "assest/help_btn-1.png");
    SDL_Texture *Help_button_selected = IMG_LoadTexture(renderer, "assest/help_btn_selected.png");
    SDL_Texture *Resume_button_norm = IMG_LoadTexture(renderer, "assest/resume_btn-1.png");
    SDL_Texture *Resume_button_selected = IMG_LoadTexture(renderer, "assest/resume_btn_selected.png");
    SDL_Texture *Mainmenu_button_norm = IMG_LoadTexture(renderer, "assest/main_btn-1.png");
    SDL_Texture *Mainmenu_button_selected = IMG_LoadTexture(renderer, "assest/main_btn_selected.png");
    SDL_Texture *First_map_button = IMG_LoadTexture(renderer, "assest/First_map.png");
    SDL_Texture *Second_map_button = IMG_LoadTexture(renderer, "assest/Second_map.png");
    SDL_Texture *Third_map_button = IMG_LoadTexture(renderer, "assest/Third_map.png");
    SDL_Texture *Random_map_button = IMG_LoadTexture(renderer, "assest/Random_map.jpg");
    SDL_Texture *Bomb = IMG_LoadTexture(renderer, "assest/bomb.png");
    SDL_Texture *Rainbow = IMG_LoadTexture(renderer, "assest/rainbow_marble.png");
    SDL_Texture *Fireball = IMG_LoadTexture(renderer, "assest/fireball_marble.png");
    SDL_Texture *Slowmo_power = IMG_LoadTexture(renderer, "assest/slowmo_power.png");
    SDL_Texture *Stop_power = IMG_LoadTexture(renderer, "assest/pause_power.png");
    SDL_Texture *Reverse_power = IMG_LoadTexture(renderer, "assest/reverse_power.png");
    SDL_Texture *Missle_tex = IMG_LoadTexture(renderer, "assest/missile.png");
    SDL_Texture *Bomb_power_button_norm = IMG_LoadTexture(renderer, "assest/bomb.png");
    SDL_Texture *Bomb_power_button_selected = IMG_LoadTexture(renderer, "assest/bomb.png");
    SDL_Texture *Rainbow_power_button_norm = IMG_LoadTexture(renderer, "assest/change_norm.png");
    SDL_Texture *Rainbow_power_button_selected = IMG_LoadTexture(renderer, "assest/change_selected.png");
    SDL_Texture *Fireball_power_button_norm = IMG_LoadTexture(renderer, "assest/change_norm.png");
    SDL_Texture *Fireball_power_button_selected = IMG_LoadTexture(renderer, "assest/change_selected.png");
    SDL_Texture *Lightning_power_button_norm = IMG_LoadTexture(renderer, "assest/change_norm.png");
    SDL_Texture *Lightning_power_button_selected = IMG_LoadTexture(renderer, "assest/change_selected.png");
    SDL_Texture *Missile_power_button_norm = IMG_LoadTexture(renderer, "assest/change_norm.png");
    SDL_Texture *Missile_power_button_selected = IMG_LoadTexture(renderer, "assest/change_selected.png");

    TTF_Font *arial_font = TTF_OpenFont("assest/arial.ttf", 24);
    TTF_Font *arial_font2 = TTF_OpenFont("assest/arial.ttf", 32);

    Mix_Music *Music2 = Mix_LoadMUS("assest/The_Fathiers.mp3");
    Mix_Music *Music3 = Mix_LoadMUS("assest/Fun_With_Finn_and_Rose.mp3");
    Mix_Music *Music1 = Mix_LoadMUS("assest/Canto_Bight.mp3");
    music_player.Mclick = Mix_LoadWAV( "assest/click.wav");
    music_player.Mexplotion = Mix_LoadWAV( "assest/explotion.wav");
    music_player.Mfire = Mix_LoadWAV( "assest/fire.wav");
    music_player.Mhit = Mix_LoadWAV( "assest/hit.wav");
    if( Music1 == NULL )
    {
        printf( "Failed to load beat music! SDL_mixer Error: %s\n", Mix_GetError() );
    }
    
    //empty texture for map 
    SDL_Texture *BackGround = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_UNKNOWN, SDL_TEXTUREACCESS_TARGET, screenWidth, screenHeight);

    string mode = "start_menu";
    string game_mode = "stone";
    int current_map = 1;
    SDL_Rect fullScreen = {0, 0, screenWidth, screenHeight};

    int current_music = 0;
    bool is_gameRunning = true;
    SDL_Event event;
    Uint32 frameStart;
    int frameTime;
    SDL_Point mouse;
    bool mouseL = false;
    Timer game_timer;
    int score, point, prize;
    int max_timer_mode_lenght = 180;

    //button create
    Button Quit;
    Quit.create(Quit_button_norm, Quit_button_selected, screenWidth - 100, screenHeight / 2 - 100, 120, 80, 300);
    Button Tryagain;
    Tryagain.create(Tryagain_button_norm, Tryagain_button_selected, screenWidth - 100, screenHeight / 2 - 100, 120, 80, 300);
    Button Choose_mode;
    Choose_mode.create(Choose_mode_button_norm, Choose_mode_button_selected, screenWidth - 100, screenHeight / 2 - 100, 120, 80, 300);
    Button Leaderboard;
    Leaderboard.create(Leaderboard_button_norm, Leaderboard_button_selected, screenWidth - 100, screenHeight / 2 - 100, 120, 80, 300);
    Button Register;
    Register.create(Register_button_norm, Register_button_selected, screenWidth - 100, screenHeight / 2 - 100, 120, 80, 300);
    Button Login;
    Login.create(Login_button_norm, Login_button_selected, screenWidth - 100, screenHeight / 2 - 100, 120, 80, 300);
    Button Done;
    Done.create(Done_button_norm, Done_button_selected, screenWidth - 100, screenHeight / 2 - 100, 120, 80, 300);
    Button Start;
    Start.create(Start_button_norm, Start_button_selected, screenWidth - 100, screenHeight / 2 - 100, 120, 80, 300);
    Button Setting;
    Setting.create(Setting_button_norm, Setting_button_selected, screenWidth - 100, screenHeight / 2 - 100, 120, 80, 300);
    Button Logout;
    Logout.create(Logout_button_norm, Logout_button_selected, screenWidth - 100, screenHeight / 2 - 100, 120, 80, 300);
    Button Normal_mode;
    Normal_mode.create(Normal_mode_button_norm, Normal_mode_button_selected, 10, screenHeight / 2 - 150, 300, 300, 300);
    Button Timer_mode;
    Timer_mode.create(Timer_mode_button_norm, Timer_mode_button_selected, 330, screenHeight / 2 - 190, 300, 380, 300);
    Button Stone_mode;
    Stone_mode.create(Stone_mode_button_norm, Stone_mode_button_selected, 650, screenHeight / 2 - 150, 300, 300, 300);
    Button Fly_mode;
    Fly_mode.create(Fly_mode_button_norm, Fly_mode_button_selected, 970, screenHeight / 2 - 150, 300, 300, 300);
    Button Musicoff;
    Musicoff.create(Musicoff_button_norm, Musicoff_button_selected, screenWidth/2 - 100 , 500, 200, 100, 300);
    Button Soundoff;
    Soundoff.create(Soundoff_button_norm, Soundoff_button_selected, screenWidth/2 - 100 , 350, 200, 100, 300);
    Button Help;
    Help.create(Help_button_norm, Help_button_selected, screenWidth/2 - 100 , 350, 200, 100, 300);
    Button Mainmenu;
    Mainmenu.create(Mainmenu_button_norm, Mainmenu_button_selected, screenWidth/2 - 100 , 350, 200, 100, 300);

    Button First_map;
    First_map.create(First_map_button, First_map_button, 40 , 25 , 560, 370, 300);
    Button Second_map;
    Second_map.create(Second_map_button, Second_map_button, 650 , 25 , 560, 370, 300);
    Button Third_map;
    Third_map.create(Third_map_button, Third_map_button, 40 , screenHeight/2 + 25 , 560, 370, 300);
    Button Random_map;
    Random_map.create(Random_map_button, Random_map_button, 650 , screenHeight/2 + 25 , 560, 370, 300);

    Keyboard_handler input_keyboard;
    input_keyboard.delay = 3;

    Inputbox username_input;
    username_input.create(Inputbox_norm , Inputbox_selected , screenWidth/2 - 300 , 150 , 600 , 100 , 30 , 0 , 0 , 0 );

    Inputbox password_input;
    password_input.create(Inputbox_norm , Inputbox_selected , screenWidth/2 - 300 , 320 , 600 , 100 , 30 , 0 , 0 , 0 );

    string username , password;

    int current_user  = -1;
    int count_user = 0;
    User users[100];
    User leaderboard_user[100];
    count_user = load_users(users);
    
    bool music_on = true;
    Mix_PlayMusic( Music1, 1 );
    
    while (is_gameRunning)
    {
        frameStart = SDL_GetTicks();
        if(Mix_PlayingMusic() == 0 )
        {
            if(music_on)
            {
                current_music++;
                if(current_music > 2)
                    current_music = 0;

                if(current_music == 0)
                    Mix_PlayMusic( Music1, 1 );
                else if(current_music == 1)
                    Mix_PlayMusic( Music2, 1 );
                else if(current_music == 2)
                    Mix_PlayMusic( Music3, 1 );
            }
        }
        while (SDL_PollEvent(&event))
        {
            switch (event.type)
            {
            case SDL_QUIT:
                is_gameRunning = false;
                break;
            case SDL_MOUSEMOTION:
                SDL_GetMouseState(&mouse.x, &mouse.y);
                break;
            case SDL_MOUSEBUTTONDOWN:
                if (event.button.button == SDL_BUTTON_LEFT)
                    mouseL = true;
                break;
            case SDL_MOUSEBUTTONUP:
                if (event.button.button == SDL_BUTTON_LEFT)
                    mouseL = false;
                break;
            case SDL_KEYDOWN:
                if (event.key.keysym.sym == SDLK_q)
                    is_gameRunning = false;
                if (mode == "login" || mode == "register")
                {
                    input_keyboard.keydown(&event);
                    if(event.key.keysym.sym == SDLK_TAB)
                    {
                        if(username_input.is_selected)
                        {
                            password_input.is_selected = true;
                            username_input.is_selected = false;
                        }
                        else
                        {
                            username_input.is_selected = true;
                            password_input.is_selected = false;
                        }
                        input_keyboard.curruntK = '!';
                    }
                }
                break;
            case SDL_KEYUP:
                if (mode == "login" || mode == "register")
                {
                    input_keyboard.keyup(&event);
                }
                break;
            }
        }
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);

        if (mode == "start_menu")
        {
            SDL_RenderCopy(renderer, frist_page_background, NULL, NULL);

            Login.rect.w = 250;
            Login.rect.h = 58;
            Login.rect.x = 120;
            Login.rect.y = 35;

            Register.rect.w = 250;
            Register.rect.h = 58;
            Register.rect.x = 120;
            Register.rect.y = 113;

            Quit.rect.w = 250;
            Quit.rect.h = 58;
            Quit.rect.x = 120;
            Quit.rect.y = 191;

            Login.Draw(renderer, &mouse);
            Register.Draw(renderer, &mouse);
            Quit.Draw(renderer, &mouse);

            if (mouseL)
            {
                if (Login.is_clicked(&mouse))
                {
                    mode = "login";
                    username_input.text = "";
                    password_input.text = "";
                    username_input.is_selected = true;
                    password_input.is_selected = false;
                }
                if (Register.is_clicked(&mouse))
                {
                    mode = "register";
                    username_input.text = "";
                    password_input.text = "";
                    username_input.is_selected = true;
                    password_input.is_selected = false;
                }
                if (Quit.is_clicked(&mouse))
                {
                    is_gameRunning = false;
                }
            }
        }
        else if (mode == "login")
        {
            SDL_RenderCopy(renderer, space_background3, NULL, NULL);
            
            SDL_Color text_color = {255, 255, 255};

            Register.rect.w = 250;
            Register.rect.h = 58;
            Register.rect.x = 210;
            Register.rect.y = 500;

            Done.rect.w = 250;
            Done.rect.h = 58;
            Done.rect.x = 510;
            Done.rect.y = 500;

            Quit.rect.w = 250;
            Quit.rect.h = 58;
            Quit.rect.x = 810;
            Quit.rect.y = 500;

            draw_text(renderer , "Login" , arial_font2 , screenWidth/2 - 100 , 50, text_color.r , text_color.g , text_color.b , 2 , 2);

            draw_text(renderer , "Username :" , arial_font2 , 100 , username_input.text_rect.y, text_color.r , text_color.g , text_color.b);

            draw_text(renderer , "Password :", arial_font2 , 100 , password_input.text_rect.y, text_color.r , text_color.g , text_color.b);

            Register.Draw(renderer, &mouse);
            Done.Draw(renderer, &mouse);
            Quit.Draw(renderer, &mouse);

            if (mouseL)
            {
                if(username_input.select(&mouse))
                    password_input.is_selected = false;
                if(password_input.select(&mouse))
                    username_input.is_selected = false;
                if (Register.is_clicked(&mouse))
                {
                    mode = "register";
                    username_input.text = "";
                    password_input.text = "";
                    username_input.is_selected = true;
                    password_input.is_selected = false;
                    SDL_Delay(200);
                }
                if(Done.is_clicked(&mouse))
                {
                    int c = is_user_exist(users , count_user , username_input.text);
                    if(c != -1)
                    {
                        if(users[c].password == password_input.text)
                        {
                            current_user = c;
                            mode = "main_menu";
                            SDL_Delay(200);
                        }
                        else
                            cout<<"Wrong Password"<<endl;
                    }
                    else
                        cout<<"User Dosent Exist"<<endl;
                }
                if (Quit.is_clicked(&mouse))
                {
                    is_gameRunning = false;
                }
            }
            username_input.input(&input_keyboard);
            username_input.Draw(renderer , arial_font2);
            password_input.input(&input_keyboard);
            password_input.Draw(renderer , arial_font2);
        }
        else if (mode == "register")
        {
            SDL_RenderCopy(renderer, space_background3, NULL, NULL);

            SDL_Color text_color = {255, 255, 255};

            Login.rect.w = 250;
            Login.rect.h = 58;
            Login.rect.x = 210;
            Login.rect.y = 500;

            Done.rect.w = 250;
            Done.rect.h = 58;
            Done.rect.x = 510;
            Done.rect.y = 500;

            Quit.rect.w = 250;
            Quit.rect.h = 58;
            Quit.rect.x = 810;
            Quit.rect.y = 500;

            draw_text(renderer , "Register" , arial_font2 , screenWidth/2 - 100 , 50, text_color.r , text_color.g , text_color.b , 2 , 2);

            draw_text(renderer , "Username :" , arial_font2 , 100 , username_input.text_rect.y, text_color.r , text_color.g , text_color.b);

            draw_text(renderer , "Password :", arial_font2 , 100 , password_input.text_rect.y, text_color.r , text_color.g , text_color.b);

            Login.Draw(renderer, &mouse);
            Done.Draw(renderer, &mouse);
            Quit.Draw(renderer, &mouse);

            if (mouseL)
            {
                if(username_input.select(&mouse))
                    password_input.is_selected = false;
                if(password_input.select(&mouse))
                    username_input.is_selected = false;
                if (Login.is_clicked(&mouse))
                {
                    mode = "login";
                    username_input.text = "";
                    password_input.text = "";
                    username_input.is_selected = true;
                    password_input.is_selected = false;
                    SDL_Delay(200);
                }
                if(Done.is_clicked(&mouse))
                {
                    if(is_user_exist(users , count_user , username_input.text) == -1)
                    {
                        users[count_user].username = username_input.text;
                        users[count_user].password = password_input.text;
                        users[count_user].bomb_power = 3;
                        users[count_user].missile_power = 3;
                        users[count_user].rainbow_power = 3;
                        users[count_user].fireball_power = 3;
                        users[count_user].lightning_power = 3;
                        users[count_user].max_stone = 0;
                        users[count_user].max_fly = 0;
                        users[count_user].max_normal = 0;
                        users[count_user].max_timer = 0;
                        current_user = count_user;
                        count_user++;
                        write_users(users , count_user);
                        mode = "main_menu";
                        SDL_Delay(200);
                    }
                    else
                        cout<<"User Already Exist"<<endl;
                }
                if (Quit.is_clicked(&mouse))
                {
                    is_gameRunning = false;
                }
            }
            username_input.input(&input_keyboard);
            username_input.Draw(renderer , arial_font2);
            password_input.input(&input_keyboard);
            password_input.Draw(renderer , arial_font2);
        }
        else if (mode == "main_menu")
        {
            SDL_RenderCopy(renderer, frist_page_background, NULL, NULL);

            SDL_Color text_color = {0, 0, 0};

            Start.rect.w = 250;
            Start.rect.h = 58;
            Start.rect.x = 120;
            Start.rect.y = 35;

            Leaderboard.rect.w = 250;
            Leaderboard.rect.h = 58;
            Leaderboard.rect.x = 120;
            Leaderboard.rect.y = 113;

            Setting.rect.w = 250;
            Setting.rect.h = 58;
            Setting.rect.x = 120;
            Setting.rect.y = 191;

            Logout.rect.w = 250;
            Logout.rect.h = 58;
            Logout.rect.x = 120;
            Logout.rect.y = 269;

            Quit.rect.w = 250;
            Quit.rect.h = 58;
            Quit.rect.x = 120;
            Quit.rect.y = 347;

            draw_text(renderer , "Welcome "+users[current_user].username , arial_font2 , screenWidth/2 - 100 , 30, text_color.r , text_color.g , text_color.b);

            Start.Draw(renderer, &mouse);
            Leaderboard.Draw(renderer, &mouse);
            Logout.Draw(renderer , &mouse);
            Setting.Draw(renderer, &mouse);
            Quit.Draw(renderer, &mouse);

            if (mouseL)
            {
                if (Start.is_clicked(&mouse))
                {
                    mode = "choose_mode";
                    SDL_Delay(200);
                }
                if (Logout.is_clicked(&mouse))
                {
                    mode = "start_menu";
                    current_user = -1;
                    SDL_Delay(200);
                }
                if (Leaderboard.is_clicked(&mouse))
                {
                    mode = "leaderboard";
                    make_leaderboard(users , count_user , leaderboard_user);
                    SDL_Delay(200);
                }
                if (Setting.is_clicked(&mouse))
                {
                    mode = "setting";
                    SDL_Delay(200);
                }
                if (Quit.is_clicked(&mouse))
                {
                    is_gameRunning = false;
                }
            }
        }
        else if (mode == "setting")
        {
            SDL_RenderCopy(renderer, frist_page_background, NULL, NULL);
            Help.rect.w = 250;
            Help.rect.h = 58;
            Help.rect.x = 120;
            Help.rect.y = 35;

            Musicoff.rect.w = 250;
            Musicoff.rect.h = 58;
            Musicoff.rect.x = 120;
            Musicoff.rect.y = 113;

            Soundoff.rect.w = 250;
            Soundoff.rect.h = 58;
            Soundoff.rect.x = 120;
            Soundoff.rect.y = 191;

            Mainmenu.rect.w = 250;
            Mainmenu.rect.h = 58;
            Mainmenu.rect.x = 120;
            Mainmenu.rect.y = 269;

            Help.Draw(renderer, &mouse);
            Musicoff.Draw(renderer, &mouse);
            Soundoff.Draw(renderer, &mouse);
            Mainmenu.Draw(renderer, &mouse);

            string music_status,sound_status;
            if(music_player.is_effect_on)
                sound_status = "ON";
            else
                sound_status = "OFF";
            if(music_on)
                music_status = "ON";
            else
                music_status = "OFF";
            draw_text(renderer , sound_status, arial_font2 , 400 , Soundoff.rect.y +Soundoff.rect.h/2 -15 , 255 , 255 , 255);
            draw_text(renderer , music_status, arial_font2 , 400 , Musicoff.rect.y +Musicoff.rect.h/2 -15 , 255 , 255 , 255);
            if (mouseL)
            {
                if (Mainmenu.is_clicked(&mouse))
                {
                    mode = "main_menu";
                    SDL_Delay(200);
                }
                if (Musicoff.is_clicked(&mouse))
                {
                    if(music_on)
                    {
                        music_on = false;
                        Mix_HaltMusic();
                    }
                    else
                        music_on = true;
                }
                if (Soundoff.is_clicked(&mouse))
                {
                    if(music_player.is_effect_on)
                        music_player.is_effect_on = false;
                    else
                        music_player.is_effect_on = true;
                }
                if (Help.is_clicked(&mouse))
                {
                    mode = "help";
                    SDL_Delay(200);
                }
            }
        }
        else if (mode == "help")
        {
            SDL_RenderCopy(renderer, frist_page_background, NULL, NULL);
            Mainmenu.rect.w = 250;
            Mainmenu.rect.h = 58;
            Mainmenu.rect.x = 100;
            Mainmenu.rect.y = 350;
            Mainmenu.Draw(renderer, &mouse);
            draw_text(renderer , "Every thing is completely obvious", arial_font2 , 50 , 50 , 255 , 255 , 255 , 1.5 , 1.5);
            draw_text(renderer , "This game is created by Mohammad bagher khandan (MBKH) and Davood", arial_font2 , 50 , 150 , 255 , 255 , 255 , 1.5 , 1.5);
            if (mouseL)
            {
                if (Mainmenu.is_clicked(&mouse))
                {
                    mode = "main_menu";
                    SDL_Delay(200);
                }
            }

        }
        else if (mode == "leaderboard")
        {
            SDL_RenderCopy(renderer, frist_page_background, NULL, NULL);
            draw_text(renderer , "Rank", arial_font2 , 50 , 43 , 255 , 255 , 255);
            draw_text(renderer , "Username", arial_font , 150 , 50 , 255 , 255 , 255);
            draw_text(renderer , "Normal Mode", arial_font , 400 , 50 , 255 , 255 , 255);
            draw_text(renderer , "Timer Mode", arial_font , 550 , 50 , 255 , 255 , 255);
            draw_text(renderer , "Stone Mode", arial_font , 700 , 50 , 255 , 255 , 255);
            draw_text(renderer , "Fly Mode", arial_font , 850 , 50 , 255 , 255 , 255);
            draw_text(renderer , "Total Point", arial_font2 , 1000 , 43 , 255 , 255 , 255);
            Mainmenu.rect.w = 150;
            Mainmenu.rect.h = 90;
            Mainmenu.rect.x = screenWidth - 200;
            Mainmenu.rect.y = 650;
            Mainmenu.Draw(renderer, &mouse);
            SDL_SetRenderDrawColor(renderer , 255 , 255 , 255 , 0);
            for(int i = 0 ; i < count_user ; i++)
            {
                SDL_RenderDrawLine(renderer , 20 , 90 + i*60 , screenWidth - 20 , 90 + i*60);
                draw_text(renderer , to_string(i+1) , arial_font2 , 50 , 93 + i*60 , 255 , 255 , 255);
                draw_text(renderer , leaderboard_user[i].username , arial_font , 150 , 100 + i*60 , 255 , 255 , 255);
                draw_text(renderer , to_string(leaderboard_user[i].max_normal) , arial_font , 400 , 100 + i*60 , 255 , 255 , 255);
                draw_text(renderer , to_string(leaderboard_user[i].max_timer) , arial_font , 550 , 100 + i*60 , 255 , 255 , 255);
                draw_text(renderer , to_string(leaderboard_user[i].max_stone) , arial_font , 700 , 100 + i*60 , 255 , 255 , 255);
                draw_text(renderer , to_string(leaderboard_user[i].max_fly) , arial_font , 850 , 100 + i*60 , 255 , 255 , 255);
                draw_text(renderer , to_string(leaderboard_user[i].total_point) , arial_font2 , 1000 , 95 + i*60 , 255 , 255 , 255);
            }
            if (mouseL)
            {
                if (Mainmenu.is_clicked(&mouse))
                {
                    mode = "main_menu";
                    SDL_Delay(200);
                }
            }
        }
        else if (mode == "choose_mode")
        {
            SDL_RenderCopy(renderer, space_background, NULL, NULL);
            Normal_mode.Draw(renderer, &mouse);
            Timer_mode.Draw(renderer, &mouse);
            Stone_mode.Draw(renderer , &mouse);
            Fly_mode.Draw(renderer, &mouse);
            draw_text(renderer , "Normal Mode", arial_font , 30 + 50 , screenHeight / 2 + 200, 255 , 255 , 255);
            draw_text(renderer , "Timer Mode", arial_font , 370 + 50 , screenHeight / 2 + 200, 255 , 255 , 255);
            draw_text(renderer , "Stone Mode", arial_font , 700 + 50 , screenHeight / 2 + 200, 255 , 255 , 255);
            draw_text(renderer , "Fly Mode", arial_font , 1030  + 50, screenHeight / 2 + 200, 255 , 255 , 255);
            if (mouseL)
            {
                if (Normal_mode.is_clicked(&mouse))
                {
                    mode = "choose_map";
                    game_mode = "normal";
                    SDL_Delay(200);
                }
                if (Timer_mode.is_clicked(&mouse))
                {
                    mode = "choose_map";
                    game_mode = "timer";
                    SDL_Delay(200);
                }
                if (Stone_mode.is_clicked(&mouse))
                {
                    mode = "choose_map";
                    game_mode = "stone";
                    SDL_Delay(200);
                }
                if (Fly_mode.is_clicked(&mouse))
                {
                    mode = "choose_map";
                    game_mode = "fly";
                    SDL_Delay(200);
                }
            }
        }
        else if (mode == "choose_map")
        {
            SDL_RenderCopy(renderer, space_background, NULL, NULL);
            First_map.Draw(renderer , &mouse);
            Second_map.Draw(renderer , &mouse);
            Third_map.Draw(renderer , &mouse);
            Random_map.Draw(renderer , &mouse);
            if (mouseL)
            {
                if (First_map.is_clicked(&mouse))
                {
                    mode = "game";
                    current_map = 1;
                    SDL_Delay(200);
                }
                if (Second_map.is_clicked(&mouse))
                {
                    mode = "game";
                    current_map = 2;
                    SDL_Delay(200);
                }
                if (Third_map.is_clicked(&mouse))
                {
                    mode = "game";
                    current_map = 3;
                    SDL_Delay(200);
                }
                if (Random_map.is_clicked(&mouse))
                {
                    mode = "game";
                    current_map = 4;
                    SDL_Delay(200);
                }
            }
        }
        else if (mode == "end_game")
        {
            SDL_RenderCopy(renderer, frist_page_background, NULL, NULL);
            SDL_Color text_color = {0, 0, 255};

            Tryagain.rect.x = 50;
            Tryagain.rect.y = 420;
            Tryagain.rect.w = 250;
            Tryagain.rect.h = 58;
            Choose_mode.rect.x = 350;
            Choose_mode.rect.y = 420;
            Choose_mode.rect.w = 250;
            Choose_mode.rect.h = 58;
            Leaderboard.rect.x = 650;
            Leaderboard.rect.y = 420;
            Leaderboard.rect.w = 250;
            Leaderboard.rect.h = 58;
            Quit.rect.x = 950;
            Quit.rect.y = 420;
            Quit.rect.w = 250;
            Quit.rect.h = 58;

            string prize_name;
            if (prize == 0)
                prize_name = "Fireball";
            else if (prize == 1)
                prize_name = "Bomb";
            else if (prize == 2)
                prize_name = "Lightning";
            else if (prize == 3)
                prize_name = "Rainbow";
            else if (prize == 4)
                prize_name = "Missile";

            draw_text(renderer , "Username", arial_font , 100 , 100, text_color.r , text_color.g , text_color.b);
            draw_text(renderer , "Mode", arial_font , 400 , 100, text_color.r , text_color.g , text_color.b);
            draw_text(renderer , "Score", arial_font , 600 , 100, text_color.r , text_color.g , text_color.b);
            draw_text(renderer , "Time", arial_font , 800 , 100, text_color.r , text_color.g , text_color.b);
            draw_text(renderer , "Your Point", arial_font , 1000 , 100, text_color.r , text_color.g , text_color.b);
            text_color = {255, 255, 255};
            draw_text(renderer , users[current_user].username, arial_font , 100 , 160, text_color.r , text_color.g , text_color.b);
            draw_text(renderer , game_mode, arial_font , 400 , 160, text_color.r , text_color.g , text_color.b);
            draw_text(renderer , to_string(score), arial_font , 600 , 160, text_color.r , text_color.g , text_color.b);
            draw_text(renderer , game_timer.get_current_time_minute_last(), arial_font , 800 , 160, text_color.r , text_color.g , text_color.b);
            draw_text(renderer , to_string(point), arial_font , 1000 , 160, text_color.r , text_color.g , text_color.b);
            draw_text(renderer , ("You win       :  " + prize_name), arial_font , 100 , 260, text_color.r , text_color.g , text_color.b , 1.2 , 1.2);

            Tryagain.Draw(renderer, &mouse);
            Leaderboard.Draw(renderer, &mouse);
            Quit.Draw(renderer, &mouse);
            Choose_mode.Draw(renderer, &mouse);

            if (mouseL)
            {
                if (Tryagain.is_clicked(&mouse))
                {
                    mode = "game";
                    SDL_Delay(200);
                }
                if (Leaderboard.is_clicked(&mouse))
                {
                    mode = "leaderboard";
                    make_leaderboard(users , count_user , leaderboard_user);
                    SDL_Delay(200);
                }
                if (Quit.is_clicked(&mouse))
                {
                    is_gameRunning = false;
                }
                if (Choose_mode.is_clicked(&mouse))
                {
                    mode = "choose_mode";
                    SDL_Delay(200);
                }
            }
        }
        else if (mode == "game")
        {
            bool is_ingame = true;
            Player player;
            if(current_map == 1)
                player.creat(Cannon, screenWidth / 2 - 50, 380, 260, 121, 54, 61);
            else if(current_map == 2)
                player.creat(Cannon, screenWidth / 2 - 200, 600, 260, 121, 54, 61);
            else if (current_map == 3)
                player.creat(Cannon, screenWidth / 2 - 100, 500, 260, 121, 54, 61);
            map ma;
            ma.total_lenght = 0;
            ma.tex = PathTex;
            if(current_map == 1)
            {
                ma.p1 = {50 , screenHeight/2};
                ma.p2 = { 50 , -80};
                ma.p3 = { screenWidth - 150 , -80};
                ma.p4 = {screenWidth - 150 , screenHeight/2};
                ma.p5 = { screenWidth - 150 , screenHeight + 70};
                ma.p6 = { 150 , screenHeight + 70};
                ma.p7 = {150 , screenHeight/2};
                ma.p8 = { 150 , 30 };
                ma.p9 = { screenWidth - 200, 30};
                ma.p10 = { screenWidth - 250 , screenHeight/2};
            }
            else if (current_map == 2)
            {
                ma.p1 = {50, screenHeight - 100};
                ma.p2 = {screenWidth / 2 - 200, screenHeight / 2};
                ma.p3 = {0, 0};
                ma.p4 = {screenWidth / 4, 100};
                ma.p5 = {screenWidth / 2, 200};
                ma.p6 = {screenWidth, -300};
                ma.p7 = {screenWidth - 150, screenHeight - 100};
                ma.p8 = {screenWidth - 200 ,screenHeight +200};
                ma.p9 = {screenWidth/2 , 400};
                ma.p10 = {screenWidth/2+300 , 200};
            }
            else if (current_map == 3)
            {
                ma.p1 = {50 , 100};
                ma.p2 = {screenWidth/2 - 150 , 0};
                ma.p3 = {screenWidth - 150 , 60};
                ma.p4 = {screenWidth - 150 , 200};
                ma.p5 = {screenWidth - 150 , 450};
                ma.p6 = {140 , 180};
                ma.p7 = {100 , 350};
                ma.p8 = {50 , screenHeight - 50};
                ma.p9 = {60 , screenHeight - 50};
                ma.p10 = {screenWidth - 200, screenHeight - 50};
            }
            else if (current_map == 4)
            {
                int ptx,pty;
                random_map(&ptx , &pty , &ma);
                player.creat(Cannon, ptx , pty, 260, 121, 54, 61);
            }
            SDL_SetRenderTarget(renderer, BackGround);
            SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
            SDL_RenderClear(renderer);
            SDL_RenderCopy(renderer, space_background, NULL, NULL);
            ma.draw_path(renderer);
            SDL_SetRenderTarget(renderer, NULL);

            int balls_width = 48;
            Ball in_air_balls[20];
            int in_air_count = 0;
            int bullet_speed = 12;
            int current_time_mode = NONE;

            double normal_speed = 1;
            double balls_v = 1;
            int count_ball = 80;
            Ball balls[200];
            creat_start_balls(count_ball, balls, ma.total_lenght, balls_width , game_mode , Red_marble, Green_marble, Blue_marble, Yellow_marble, Red_marble_ice, Green_marble_ice, Blue_marble_ice, Yellow_marble_ice, Black_marble, Question_marble, Stone_marble, Fly_marble);

            Ball bullet;
            make_cannon_ball(count_ball, balls, &bullet, balls_width, bullet_speed, &player, Red_marble, Green_marble, Blue_marble, Yellow_marble);
            Ball bullet2;
            make_cannon_ball(count_ball, balls, &bullet2, balls_width, bullet_speed, &player, Red_marble, Green_marble, Blue_marble, Yellow_marble);

            Button bomb_power;
            bomb_power.create(Bomb_power_button_norm, Bomb_power_button_selected, screenWidth - 100, screenHeight / 2 - 150, 70, 70, 300);

            Button rainbow_pawer;
            rainbow_pawer.create(Rainbow, Rainbow, screenWidth - 100, screenHeight / 2, 70, 70, 300);

            Button fireball_power;
            fireball_power.create(Fireball, Fireball, screenWidth - 100, screenHeight / 2 - 300, 70, 70, 300);

            Button Missile_power;
            Missile_power.create(Missle_tex, Missle_tex, screenWidth - 100, screenHeight / 2 + 150, 70, 70, 300);

            Button Musicoff;
            Musicoff.create(Musicoff_button_norm, Musicoff_button_selected, screenWidth/2 - 150, 500, 350, 100, 300);
            Button Soundoff;
            Soundoff.create(Soundoff_button_norm, Soundoff_button_selected, screenWidth/2 - 150, 350, 350, 100, 300);
            Button Music_change;
            Music_change.create(Music_change_button_norm, Music_change_button_selected, screenWidth/2 - 150, 200, 350, 100, 300);
            Button Mainmenu;
            Mainmenu.create(Mainmenu_button_norm, Mainmenu_button_selected, screenWidth/2 - 150, 650, 350, 100, 300);
            Button Resume;
            Resume.create(Resume_button_norm, Resume_button_selected, screenWidth/2- 150 , 50, 350, 100, 300);

            Timer bullet_shoot;
            Timer time_effect_timer;

            SDL_Color timer_color = {255, 0, 0};
            string current_time;

            SDL_Color score_color = {0, 255, 0};


            Missile missile;
            missile.create(Missle_tex , -100 , -100 , 100 , 100 , 10 , 10 , 450 , 450);
            Keyboard_handler game_keyboard;
            game_keyboard.delay = 500;
            int time_effect_chance[4] = {20000, 1, 1, 1};
            int minute, second;

            score = 0;
            game_timer.creat();
            bool is_stop = false;
            bool is_down_esc = false;
            while (is_ingame)
            {
                frameStart = SDL_GetTicks();
                if(Mix_PlayingMusic() == 0 )
                {
                    if(music_on)
                    {
                        current_music++;
                        if(current_music > 2)
                            current_music = 0;

                        if(current_music == 0)
                            Mix_PlayMusic( Music1, 1 );
                        else if(current_music == 1)
                            Mix_PlayMusic( Music2, 1 );
                        else if(current_music == 2)
                            Mix_PlayMusic( Music3, 1 );
                    }
                }
                while (SDL_PollEvent(&event))
                {
                    switch (event.type)
                    {
                    case SDL_QUIT:
                        is_gameRunning = false;
                        is_ingame = false;
                        break;
                    case SDL_MOUSEMOTION:
                        SDL_GetMouseState(&mouse.x, &mouse.y);
                        break;
                    case SDL_MOUSEBUTTONDOWN:
                        if (event.button.button == SDL_BUTTON_LEFT)
                            mouseL = true;
                        break;
                    case SDL_MOUSEBUTTONUP:
                        if (event.button.button == SDL_BUTTON_LEFT)
                            mouseL = false;
                        break;
                    case SDL_KEYDOWN:
                        game_keyboard.keydown(&event);
                        if (event.key.keysym.sym == SDLK_ESCAPE && !is_down_esc)
                        {
                            is_down_esc = true;
                            if(is_stop)
                            {
                                is_stop = false;
                                game_timer.restart();
                                time_effect_timer.restart();
                            }
                            else
                            {
                                is_stop = true;
                                game_timer.stop();
                                time_effect_timer.stop();
                            }
                        }
                    case SDL_KEYUP:
                        game_keyboard.keyup(&event);
                        if (event.key.keysym.sym == SDLK_ESCAPE)
                            is_down_esc = false;
                        break;
                    }
                }
                SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
                SDL_RenderClear(renderer);
                if(is_stop)
                {
                    SDL_RenderCopy(renderer, space_background, NULL, NULL);
                    Resume.Draw(renderer , &mouse);
                    Mainmenu.Draw(renderer , &mouse);
                    Music_change.Draw(renderer , &mouse);
                    Musicoff.Draw(renderer , &mouse);
                    Soundoff.Draw(renderer , &mouse);
                    string music_status,sound_status;
                    if(music_player.is_effect_on)
                        sound_status = "ON";
                    else
                        sound_status = "OFF";
                    if(music_on)
                        music_status = "ON";
                    else
                        music_status = "OFF";
                    draw_text(renderer , sound_status, arial_font2 , 850 , Soundoff.rect.y +Soundoff.rect.h/2 -15 , 255 , 255 , 255);
                    draw_text(renderer , music_status, arial_font2 , 850 , Musicoff.rect.y +Musicoff.rect.h/2 -15 , 255 , 255 , 255);
                    if(mouseL)
                    {
                        if (Resume.is_clicked(&mouse))
                        {
                            SDL_Delay(150);
                            is_stop = false;
                            game_timer.restart();
                            time_effect_timer.restart();
                        }
                        if (Mainmenu.is_clicked(&mouse))
                        {
                            SDL_Delay(150);
                            is_ingame = false;
                            mode = "main_menu";
                        }
                        if (Soundoff.is_clicked(&mouse))
                        {
                            if(music_player.is_effect_on)
                                music_player.is_effect_on = false;
                            else
                                music_player.is_effect_on = true;
                        }
                        if (Music_change.is_clicked(&mouse))
                        {
                            Mix_HaltMusic();
                        }
                        if (Musicoff.is_clicked(&mouse))
                        {
                            if(music_on)
                            {
                                music_on = false;
                                Mix_HaltMusic();
                            }
                            else
                                music_on = true;
                        }
                    }
                }
                else
                {
                    SDL_RenderCopy(renderer, BackGround, NULL, NULL);

                    if (current_time_mode != NONE)
                        if (time_effect_timer.get_current_time() > 5000)
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
                        if (balls[i].time_effect_mode == NONE)
                            balls[i].Draw(renderer);
                        else if (balls[i].time_effect_mode == STOP)
                            balls[i].Draw(renderer, true, Stop_power);
                        else if (balls[i].time_effect_mode == SLOWMO)
                            balls[i].Draw(renderer, true, Slowmo_power);
                        else if (balls[i].time_effect_mode == REVERSE)
                            balls[i].Draw(renderer, true, Reverse_power);
                        if (balls[i].color == "Red" || balls[i].color == "Yellow" || balls[i].color == "Blue" || balls[i].color == "Green")
                        {
                            int timeE = random(time_effect_chance, 4);
                            if (timeE != 0)
                            {
                                balls[i].time_effect_mode = timeE;
                                balls[i].time_effect_timer.creat();
                            }
                        }
                        if (balls[i].current_loc > ma.total_lenght && balls[i].color != "Stone")
                        {
                            cout<<"here4";
                            is_ingame = false;
                            mode = "end_game";
                            end_game(game_mode, score, &game_timer, &point, &prize , users , count_user , current_user);
                        }

                        if(balls[i].color == "Stone" && i + 1 == count_ball && balls[i].normal_v == 0)
                        {
                            balls[i].normal_v = 11;
                            balls[i].current_loc += 10;
                            balls[i].leftConnnected = false;
                        }
                        if(balls[i].current_loc > ma.total_lenght && balls[i].color == "Stone")
                        {
                            count_ball--;
                            score++;
                        }
                    }
                    if (game_mode == "normal" || game_mode == "timer" || game_mode == "stone" || game_mode == "fly")
                    {
                        if (count_ball == 0)
                        {
                            cout<<"here3";
                            is_ingame = false;
                            mode = "end_game";
                            end_game(game_mode, score, &game_timer, &point, &prize , users , count_user , current_user);
                        }
                    }
                    if((game_mode == "stone" && score == 10) || (game_mode == "fly" && score == 10))
                    {
                        cout<<"here1";
                        is_ingame = false;
                        mode = "end_game";
                        end_game(game_mode, score, &game_timer, &point, &prize , users , count_user , current_user);
                    }
                    if ((max_timer_mode_lenght - (game_timer.get_current_time() - game_timer.start)) <= 0)
                    {
                        cout<<"here2";
                        is_ingame = false;
                        mode = "end_game";
                        end_game(game_mode, score, &game_timer, &point, &prize , users , count_user , current_user);
                    }

                    if (!bomb_power.is_inside(&mouse) && !rainbow_pawer.is_inside(&mouse) && !fireball_power.is_inside(&mouse) && ((mouseL && in_air_count == 0) || (mouseL && bullet.is_in_cannon && in_air_count < 20 && bullet_shoot.get_current_time() > 600)))
                    {
                        bullet.shoot(&mouse);
                        music_player.play_chunk("fire");
                        in_air_balls[in_air_count] = bullet;
                        bullet_shoot.creat();
                        in_air_count++;
                        bullet = bullet2;
                        bullet.center.x = player.center.x + player.rect.x;
                        bullet.center.y = player.center.y + player.rect.y;
                        make_cannon_ball(count_ball, balls, &bullet2, balls_width, bullet_speed, &player, Red_marble, Green_marble, Blue_marble, Yellow_marble);
                    }

                    bullet.update();
                    bullet.Draw(renderer);

                    bullet2.center.x = player.rect.x + 32;
                    bullet2.center.y = player.rect.y + player.rect.h - 10;
                    
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
                                collision(balls, &count_ball, j, &in_air_balls[i], &ma, balls_v, balls_width, &current_time_mode, &time_effect_timer, &score , game_mode);
                                deleted_index = i;
                                break;
                            }
                        }
                    }
                    bullet2.Draw2(renderer , &mouse , &player);
                    player.Draw(renderer, &mouse);
                    if (game_keyboard.get_current() != '!')
                    {
                        if (game_keyboard.curruntK == ' ')
                        {
                            Ball temp;
                            temp = bullet2;
                            bullet2 = bullet;
                            bullet = temp;
                            bullet.center.x = player.center.x + player.rect.x;
                            bullet.center.y = player.center.y + player.rect.y;
                        }
                    }
                    if (deleted_index != -1)
                    {
                        delete_ball(in_air_balls, in_air_count, deleted_index);
                        in_air_count--;
                    }
                    bomb_power.Draw(renderer, &mouse);
                    rainbow_pawer.Draw(renderer, &mouse);
                    fireball_power.Draw(renderer, &mouse);
                    Missile_power.Draw(renderer, &mouse);

                    if (mouseL)
                    {
                        if (bomb_power.is_clicked(&mouse))
                        {
                            if(users[current_user].bomb_power > 0 )
                            {    
                                bullet.creat_cannon_ball(Bomb, "Bomb", &player, balls_width, bullet_speed);
                                users[current_user].bomb_power--;
                            }
                        }
                        if (rainbow_pawer.is_clicked(&mouse))
                        {
                            if(users[current_user].rainbow_power > 0)
                            {
                                bullet.creat_cannon_ball(Rainbow, "Rainbow", &player, balls_width, bullet_speed);
                                users[current_user].rainbow_power--;
                            }
                        }
                        if (Missile_power.is_clicked(&mouse))
                        {
                            if(users[current_user].missile_power > 0 && !missile.is_lounched)
                            {
                                users[current_user].missile_power--;
                                missile.create(Missle_tex , -100 , -100 , 100 , 100 , 7 , 7 , 450 , 450);
                                missile.lounch();
                            }
                        }
                        if (fireball_power.is_clicked(&mouse))
                        {
                            if(users[current_user].fireball_power > 0)
                            {
                                bullet.creat_cannon_ball(Fireball, "Fireball", &player, balls_width, bullet_speed);
                                bullet2.creat_cannon_ball(Fireball, "Fireball", &player, balls_width, bullet_speed);
                                users[current_user].fireball_power--;
                            }
                        }
                    }
                    draw_text(renderer , to_string(users[current_user].bomb_power), arial_font , screenWidth - 80 , screenHeight / 2 - 60 , timer_color.r , timer_color.g , timer_color.b);
                    draw_text(renderer , to_string(users[current_user].fireball_power), arial_font , screenWidth - 80 , screenHeight / 2 - 210 , timer_color.r , timer_color.g , timer_color.b);
                    draw_text(renderer , to_string(users[current_user].missile_power), arial_font , screenWidth - 80 , screenHeight / 2 + 230 , timer_color.r , timer_color.g , timer_color.b);
                    draw_text(renderer , to_string(users[current_user].rainbow_power), arial_font , screenWidth - 80 , screenHeight / 2 + 90 , timer_color.r , timer_color.g , timer_color.b);

                    missile.update();
                    missile.hit(balls , &count_ball , &score , game_mode);
                    missile.Draw(renderer);
                    if (game_mode == "timer")
                        current_time = game_timer.reverse_get_current_time(max_timer_mode_lenght);
                    else
                        current_time = game_timer.get_current_time_minute();

                    draw_text(renderer , current_time, arial_font , screenWidth - 100, 30, timer_color.r , timer_color.g , timer_color.b);
                    draw_text(renderer , to_string(score), arial_font , 50, 30, score_color.r , score_color.g , score_color.b);
                }
                SDL_RenderPresent(renderer);
                frameTime = SDL_GetTicks() - frameStart;
                if (frameTime < frameDelay)
                    SDL_Delay(frameDelay - frameTime);
            }
        }

        SDL_RenderPresent(renderer);
        frameTime = SDL_GetTicks() - frameStart;
        if (frameTime < frameDelay)
            SDL_Delay(frameDelay - frameTime);
    }

    return 0;
}