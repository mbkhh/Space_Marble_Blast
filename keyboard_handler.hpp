struct Keyboard_handler
{
    bool is_down = false;
    bool is_anything;
    char curruntK;
    int delay;
    Timer timer;
    void keydown(SDL_Event *event)
    {
        if(timer.get_current_time() > delay && !is_down)
        {
            is_down = true;
            timer.creat();
            if(event->key.keysym.sym == SDLK_SPACE)
                curruntK = ' ';
            else if(event->key.keysym.sym == SDLK_a)
                curruntK = 'a';
            else if(event->key.keysym.sym == SDLK_b)
                curruntK = 'b';
            else if(event->key.keysym.sym == SDLK_c)
                curruntK = 'c';
            else if(event->key.keysym.sym == SDLK_d)
                curruntK = 'd';
            else if(event->key.keysym.sym == SDLK_e)
                curruntK = 'e';
            else if(event->key.keysym.sym == SDLK_f)
                curruntK = 'f';
            else if(event->key.keysym.sym == SDLK_g)
                curruntK = 'g';
            else if(event->key.keysym.sym == SDLK_h)
                curruntK = 'h';
            else if(event->key.keysym.sym == SDLK_i)
                curruntK = 'i';
            else if(event->key.keysym.sym == SDLK_j)
                curruntK = 'j';
            else if(event->key.keysym.sym == SDLK_k)
                curruntK = 'k';
            else if(event->key.keysym.sym == SDLK_l)
                curruntK = 'l';
            else if(event->key.keysym.sym == SDLK_m)
                curruntK = 'm';
            else if(event->key.keysym.sym == SDLK_n)
                curruntK = 'n';
            else if(event->key.keysym.sym == SDLK_o)
                curruntK = 'o';
            else if(event->key.keysym.sym == SDLK_p)
                curruntK = 'p';
            else if(event->key.keysym.sym == SDLK_q)
                curruntK = 'q';
            else if(event->key.keysym.sym == SDLK_r)
                curruntK = 'r';
            else if(event->key.keysym.sym == SDLK_s)
                curruntK = 's';
            else if(event->key.keysym.sym == SDLK_t)
                curruntK = 't';
            else if(event->key.keysym.sym == SDLK_u)
                curruntK = 'u';
            else if(event->key.keysym.sym == SDLK_v)
                curruntK = 'v';
            else if(event->key.keysym.sym == SDLK_w)
                curruntK = 'w';
            else if(event->key.keysym.sym == SDLK_x)
                curruntK = 'x';
            else if(event->key.keysym.sym == SDLK_y)
                curruntK = 'y';
            else if(event->key.keysym.sym == SDLK_z)
                curruntK = 'z';
            is_anything = true;
        }
    }
    char get_current()
    {
        if(is_anything)
        {
            is_anything = false;
            return curruntK;
        }
        else
            return '!';
        
    }
    void keyup(SDL_Event *event)
    {
        is_down = false;
    }  
};