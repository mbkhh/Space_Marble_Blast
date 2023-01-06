struct Player
{
    SDL_Point center;
    SDL_Rect rect;
    SDL_Texture* texture;
    void creat(SDL_Texture* textur , int x , int y ,int w , int h, int CenterX , int centerY)
    {
        texture = textur;
        center = {CenterX , centerY};
        rect = {x , y , w ,h};
    }
    void Draw(SDL_Renderer* renderer , SDL_Point* mouth)
    {
        SDL_RenderCopyEx(renderer , texture , NULL , &rect ,(atan2( mouth->y-(center.y+rect.y) , mouth->x - (center.x+rect.x))*180)/M_PI,&center , SDL_FLIP_NONE);
    }
};