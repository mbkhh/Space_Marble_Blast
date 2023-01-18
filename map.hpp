struct map
{
    SDL_Point p1,p2,p3,p4,p5,p6,p7;
    void get_point(int t , SDL_Point *x)
    {
        if(t > 1000)
        {
            t-=1000;
            x->x = pow((1.0-t/1000.0) , 3)*p4.x + 3*pow((1.0-t/1000.0),2)*(t/1000.0)*p5.x+3*(1.0-t/1000.0)*pow(t/1000.0,2)*p6.x + pow(t/1000.0 , 3)*p7.x;
            x->y = pow((1.0-t/1000.0) , 3)*p4.y + 3*pow((1.0-t/1000.0),2)*(t/1000.0)*p5.y+3*(1.0-t/1000.0)*pow(t/1000.0,2)*p6.y + pow(t/1000.0 , 3)*p7.y;
        }
        else
        {
            x->x = pow((1.0-t/1000.0) , 3)*p1.x + 3*pow((1.0-t/1000.0),2)*(t/1000.0)*p2.x+3*(1.0-t/1000.0)*pow(t/1000.0,2)*p3.x + pow(t/1000.0 , 3)*p4.x;
            x->y = pow((1.0-t/1000.0) , 3)*p1.y + 3*pow((1.0-t/1000.0),2)*(t/1000.0)*p2.y+3*(1.0-t/1000.0)*pow(t/1000.0,2)*p3.y + pow(t/1000.0 , 3)*p4.y;
        }
    }
    void draw_path(SDL_Renderer* renderer)
    {
        int t = 0;
        SDL_SetRenderDrawColor(renderer , 255 , 0 , 0 ,0);
        SDL_Point p;
        while(t < 2001)
        {
            get_point(t , &p);
            SDL_RenderDrawPoint(renderer , p.x , p.y);
            t++;
        }
    }
};
