struct Missile
{
    SDL_Texture *tex;
    SDL_Rect rect;
    int vx, vy;
    int target_x, target_y;
    bool is_lounched;
    void create(SDL_Texture *texture, int x, int y, int w, int h, int v_x, int v_y, int targetX, int targetY)
    {
        tex = texture;
        vx = v_x;
        vy = v_y;
        target_x = targetX;
        target_y = targetY;
        rect = {x, y, w, h};
        is_lounched = false;
    }
    void update()
    {
        if(is_lounched)
        {
            rect.x += vx;
            rect.y += vy;
        }
    }
    bool is_hited()
    {
        if (rect.x > target_x && rect.y > target_y)
            return true;
        else
            return false;
    }
    void lounch()
    {
        is_lounched = true;
    }
    void hit(Ball balls[] , int *count_ball , int *score ,string game_mode)
    {
        if (is_lounched)
        {
            if (is_hited())
            {
                music_player.play_chunk("explotion");
                int first = -1;
                int count = 0;
                int deleted[200];
                for (int i = 0 ; i < *count_ball ; i++)
                {
                    if(balls[i].current_loc > 0)
                    {
                        deleted[count++] = i;
                        if(balls[i].color == "Stone" || balls[i].color == "Fly")
                            *score += 1;
                        if(first == -1)
                            first = i;
                    }
                }
                if(first > 0)
                    balls[first - 1].rightConnnected = false;
                delete_ball(balls , *count_ball , -1 , deleted , count);
                *count_ball -= count;
                if(game_mode != "stone" && game_mode != "fly")
                    *score += count;
                is_lounched = false;                
            }
        }
    }
    void Draw(SDL_Renderer *renderer)
    {
        if(is_lounched)
            SDL_RenderCopy(renderer , tex , NULL , &rect);
    }
};