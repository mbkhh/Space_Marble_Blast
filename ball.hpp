struct Ball
{
    SDL_Texture* tex;
    string color;
    SDL_Point center;
    SDL_Rect rect;
    double current_loc = 0;
    int lastT=0;
    double total_path;
    bool leftConnnected=true , rightConnnected=true;
    bool is_in_cannon = false,is_primary=false;
    double velocity,v_x,v_y;
    double angle;
    void creat_cannon_ball(SDL_Texture* texture , string mode , Player* player , int width , int v)
    {
        velocity = v;
        tex = texture;
        center.x = player->center.x + player->rect.x;
        center.y = player->center.y + player->rect.y;
        is_in_cannon = true;
        is_primary = true;
        rect = {0 , 0 , width , width};
    }
    void shoot(SDL_Point* mouth)
    {
        is_in_cannon = false;
        angle = atan2((double)mouth->y - center.y , (double)mouth->x - center.x);
        v_x = cos(angle) * velocity;
        v_y = sin(angle) * velocity;
    }

    void creat(SDL_Texture* texture , string mode , int loc , int width , int path_lenght)
    {
        is_in_cannon = false;
        is_primary = false;
        total_path = path_lenght;
        current_loc = loc;
        color = mode;
        tex = texture;
        rect = {0 , 0 , width , width};
    }
    void update(map *ma = nullptr , int v = 0)
    {
        if(is_primary == false)
        {
            if(current_loc > total_path)
                current_loc = 0;
            current_loc += v;
            if(current_loc >= 0)
            {
                lastT = ma->getT_distance(current_loc , (lastT-5)/2);
                ma->get_point( lastT, &center);
            }
        }
        else
        {
            if(!is_in_cannon)
            {
                center.x += v_x;
                center.y += v_y;
            }
        }
            
    }
    bool is_out()
    {
        if(center.x - rect.w/2 > screenWidth || center.x + rect.w/2 < 0 )
            return true;
        if(center.y - rect.w/2 > screenHeight || center.y + rect.w/2 < 0)
            return true;
        return false;
    }
    void Draw(SDL_Renderer* renderer )
    {
        if(current_loc >= 0)
        {
            rect.x = center.x - rect.w/2;
            rect.y = center.y - rect.w/2;
            SDL_RenderCopy(renderer , tex , NULL , &rect);
        }
    }  
};
void creat_start_balls(int count_ball , Ball balls[] , int path_tatal_lenght ,int ball_width, SDL_Texture* Red_marble , SDL_Texture* Green_marble , SDL_Texture* Blue_marble , SDL_Texture* Yellow_marble )
{
    int count_of_ball_mode = 4;
    int chances[] = {1 , 1 , 1 , 1};
    for (int i = 0; i < count_ball; i++)
    {
        int mode = random(chances , count_of_ball_mode);
        switch (mode)
        {
        case RED:
            balls[i].creat(Red_marble, "Red", (count_ball*-ball_width)+ i * ball_width, ball_width, path_tatal_lenght);
            chances[RED] = 3;
            for(int i = 0 ; i < count_of_ball_mode ; i++)
                if(i != RED)
                    chances[i] = 1;
            break;
        case GREEN:
            balls[i].creat(Green_marble, "Green", (count_ball*-ball_width)+ i * ball_width, ball_width, path_tatal_lenght);
            chances[GREEN] = 3;
            for(int i = 0 ; i < count_of_ball_mode ; i++)
                if(i != GREEN)
                    chances[i] = 1;
            break;
        case BLUE:
            balls[i].creat(Blue_marble, "Blue", (count_ball*-ball_width)+ i * ball_width, ball_width, path_tatal_lenght);
            chances[BLUE] = 3;
            for(int i = 0 ; i < count_of_ball_mode ; i++)
                if(i != BLUE)
                    chances[i] = 1;
            break;
        case YELLOW:
            balls[i].creat(Yellow_marble, "Yellow", (count_ball*-ball_width)+ i * ball_width, ball_width, path_tatal_lenght);
            chances[YELLOW] = 3;
            for(int i = 0 ; i < count_of_ball_mode ; i++)
                if(i != YELLOW)
                    chances[i] = 1;
            break;
        }        
    }
    balls[0].leftConnnected = false;
    balls[count_ball-1].rightConnnected = false;
}
void handle_map_balls(int count_ball , Ball balls[] , int balls_v , map* ma )
{
    bool is_connected = true;
    int last = 0;
    for(int i = 0 ; i < count_ball ; i++)
    {
        if(i == 0)
            balls[i].update(ma , balls_v);
        else if(balls[i].leftConnnected && is_connected)
        {
            balls[i].update(ma , balls_v);
            balls[i].current_loc = balls[i-1].current_loc +balls[i].rect.w;
            last = i;
        }
        else
        {
            if(balls[i].current_loc - balls[i].rect.w <= balls[i-1].current_loc && last+1 == i)
            {
                balls[i-1].rightConnnected = true;
                balls[i].leftConnnected = true;
                is_connected = true;
            }
            else
            {
                balls[i].update(ma);
                is_connected = false;
            }
        }
    }
}
void delete_ball(Ball balls[] , int count_ball , int index = -1 , int andises[]=NULL, int andis_count = -1)
{
    int answer_count ;
    if(index == -1)
        answer_count = count_ball - andis_count;
    else
        answer_count = count_ball - 1;
    Ball ans[answer_count];
    int k = 0;
    for(int i = 0 ; i < count_ball ; i++)
    {
        if(index == -1)
        {
            bool is_in_andis = false;
            for(int j = 0 ; j <andis_count ; j++)
                if(andises[j] == i)
                    is_in_andis = true;
            if(!is_in_andis)
                ans[k++] = balls[i];
        }
        else
            if(index != i)
                ans[k++] = balls[i];
    }
    for(int i = 0 ; i < answer_count ; i++)
        balls[i] = ans[i];
}
void make_cannon_ball(int count_ball , Ball balls[] , Ball* bullet , int ball_width ,int v , Player* player, SDL_Texture* Red_marble , SDL_Texture* Green_marble , SDL_Texture* Blue_marble , SDL_Texture* Yellow_marble)
{
    int count_of_ball_mode = 4;
    int chances[] = {1 , 1 , 1 , 1};
    int mode = random(chances , count_of_ball_mode);
    switch (mode)
    {
    case RED:
        bullet->creat_cannon_ball(Red_marble , "Red" , player , ball_width , v);
        break;
    case GREEN:
        bullet->creat_cannon_ball(Green_marble , "Green" , player , ball_width , v);
        break;
    case BLUE:
        bullet->creat_cannon_ball(Blue_marble , "Blue" , player , ball_width , v);
        break;
    case YELLOW:
        bullet->creat_cannon_ball(Yellow_marble , "Yellow" , player , ball_width , v);
        break;
    }        
}