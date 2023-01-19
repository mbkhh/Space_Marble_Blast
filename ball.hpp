struct Ball
{
    SDL_Texture* tex;
    string color;
    SDL_Point center;
    SDL_Rect rect;
    double current_loc = 0;
    int v,lastT=0;
    double total_path;
    void creat(SDL_Texture* tex , string mode , int x , int y)
    {

    }
    void Draw(SDL_Renderer* renderer , map *ma)
    {
        if(current_loc > total_path)
            current_loc = 0;
        lastT = ma->getT_distance(current_loc , (lastT-5)/2);
        ma->get_point( lastT, &center);
        rect.x = center.x - rect.w/2;
        rect.y = center.y - rect.w/2;
        current_loc += v;
        SDL_RenderCopy(renderer , tex , NULL , &rect);
    }  
};