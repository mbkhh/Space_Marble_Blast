struct Button
{
    SDL_Texture *tex;
    SDL_Texture *hover_tex;
    SDL_Rect rect;
    Timer timer;
    int delay;
    void create(SDL_Texture *texture, SDL_Texture *hover_texture, int x, int y, int w, int h, int delayy)
    {
        tex = texture;
        hover_tex = hover_texture;
        rect = {x, y, w, h};
        delay = delayy;
    }
    bool is_inside(SDL_Point *mouth)
    {
        if (rect.x <= mouth->x && rect.x + rect.w >= mouth->x && rect.y <= mouth->y && rect.y + rect.h >= mouth->y)
            return true;
        else
            return false;
    }
    bool is_clicked(SDL_Point *mouth)
    {
        if (is_inside(mouth) && timer.get_current_time() > delay)
        {
            timer.creat();
            return true;
        }
        else
            return false;
    }
    void Draw(SDL_Renderer *renderer, SDL_Point *mouth)
    {
        if (rect.x <= mouth->x && rect.x + rect.w >= mouth->x && rect.y <= mouth->y && rect.y + rect.h >= mouth->y)
            SDL_RenderCopy(renderer, hover_tex, NULL, &rect);
        else
            SDL_RenderCopy(renderer, tex, NULL, &rect);
    }
};