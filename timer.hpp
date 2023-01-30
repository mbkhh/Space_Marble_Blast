struct Timer
{
    Uint32 start = -3000;
    void creat()
    {
        start = SDL_GetTicks();
    }
    int get_current_time()
    {
        return  SDL_GetTicks() - start;
    }
};
