struct Player
{
    SDL_Point center;
    SDL_Rect rect;
    SDL_Texture *texture;
    void creat(SDL_Texture *textur, int x, int y, int w, int h, int CenterX, int centerY)
    {
        texture = textur;
        center = {CenterX, centerY};
        rect = {x, y, w, h};
    }
    void Draw(SDL_Renderer *renderer, SDL_Point *mouse)
    {
        SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
        SDL_RenderDrawLine(renderer, mouse->x, mouse->y, center.x + rect.x, center.y + rect.y);
        SDL_RenderCopyEx(renderer, texture, NULL, &rect, (atan2(mouse->y - (center.y + rect.y), mouse->x - (center.x + rect.x)) * 180) / M_PI, &center, SDL_FLIP_NONE);
    }
};