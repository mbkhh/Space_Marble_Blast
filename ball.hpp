struct Ball
{
    SDL_Texture* tex;
    string color;
    SDL_Point center;
    SDL_Rect rect;
    int t = 0;
    void creat(SDL_Texture* tex , string mode , int x , int y)
    {

    }
    void Draw(SDL_Renderer* renderer , map *ma)
    {
        ma->get_point(t , &center);
        rect.x = center.x - rect.w/2;
        rect.y = center.y - rect.w/2;
        t++;
        SDL_RenderCopy(renderer , tex , NULL , &rect);
    }  
};