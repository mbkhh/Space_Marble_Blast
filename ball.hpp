struct Ball
{
    SDL_Texture* tex;
    string color;
    SDL_Rect rect;

    void Draw(SDL_Renderer* renderer)
    {
        SDL_RenderCopy(renderer , tex , NULL , &rect);
    }  
};