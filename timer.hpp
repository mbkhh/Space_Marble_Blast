struct Timer
{
    Uint32 start;
    void creat()
    {
        start = SDL_GetTicks();
    }
    int get_current_time()
    {
        return  SDL_GetTicks() - start;
    }
};
