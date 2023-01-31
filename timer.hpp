struct Timer
{
    Uint32 start = -3000;
    Uint32 last;
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
};
