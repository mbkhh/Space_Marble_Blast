struct Timer
{
    Uint32 start = -3000;
    Uint32 last;
    Uint32 save;
    void creat()
    {
        start = SDL_GetTicks();
    }
    int get_current_time()
    {
        return SDL_GetTicks() - start;
    }
    void end()
    {
        last = SDL_GetTicks() - start;
    }
    void stop()
    {
        save = SDL_GetTicks() - start;
    }
    void restart()
    {
        start = SDL_GetTicks() - save;
    }
    string get_current_time_minute()
    {
        int t = (SDL_GetTicks() - start)/1000;
        int minute,second;
        minute = t / 60;
        second = t % 60;
        return to_string(minute) + " : " + to_string(second);
    }
    string get_current_time_minute_last()
    {
        int t = last/1000;
        int minute,second;
        minute = t / 60;
        second = t % 60;
        return to_string(minute) + " : " + to_string(second);
    }
    string reverse_get_current_time(int max)
    {
        int t = max - (SDL_GetTicks() - start)/1000;
        int minute,second;
        minute = t / 60;
        second = t % 60;
        return to_string(minute) + " : " + to_string(second);
    }
};
