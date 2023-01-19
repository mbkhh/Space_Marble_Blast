struct map
{
    SDL_Point p1, p2, p3, p4, p5, p6, p7;
    SDL_Texture *tex;
    double total_lenght;
    double distance_samples[1000];
    void get_point(int t, SDL_Point *x = nullptr, double *x2 = nullptr, double *y2 = nullptr)
    {
        double X, Y;
        if (t > 1000)
        {
            t -= 1000;
            X = pow((1.0 - t / 1000.0), 3) * p4.x + 3 * pow((1.0 - t / 1000.0), 2) * (t / 1000.0) * p5.x + 3 * (1.0 - t / 1000.0) * pow(t / 1000.0, 2) * p6.x + pow(t / 1000.0, 3) * p7.x;
            Y = pow((1.0 - t / 1000.0), 3) * p4.y + 3 * pow((1.0 - t / 1000.0), 2) * (t / 1000.0) * p5.y + 3 * (1.0 - t / 1000.0) * pow(t / 1000.0, 2) * p6.y + pow(t / 1000.0, 3) * p7.y;
        }
        else
        {
            X = pow((1.0 - t / 1000.0), 3) * p1.x + 3 * pow((1.0 - t / 1000.0), 2) * (t / 1000.0) * p2.x + 3 * (1.0 - t / 1000.0) * pow(t / 1000.0, 2) * p3.x + pow(t / 1000.0, 3) * p4.x;
            Y = pow((1.0 - t / 1000.0), 3) * p1.y + 3 * pow((1.0 - t / 1000.0), 2) * (t / 1000.0) * p2.y + 3 * (1.0 - t / 1000.0) * pow(t / 1000.0, 2) * p3.y + pow(t / 1000.0, 3) * p4.y;
        }
        if (x != nullptr)
        {
            x->x = X;
            x->y = Y;
        }
        if (x2 != nullptr && y2 != nullptr)
        {
            *x2 = X;
            *y2 = Y;
        }
    }
    void find_distance_samples(SDL_Renderer* renderer = nullptr)
    {
        int k=0;
        double t = 0;
        double current = 0;
        double x1,x2,y1,y2;
        x1 = p1.x;
        y1 = p1.y;
        while (t <1000)
        {
            
            distance_samples[k++] = current;
            t++;
            get_point(t*2 , nullptr , &x2 , &y2);
            if(renderer != nullptr)
                SDL_RenderDrawLine(renderer , x1 , y1 , x2 , y2);
            current += sqrt((y2 - y1) * (y2 - y1) + (x2 - x1) * (x2 - x1));
            x1 = x2;
            y1 = y2;
        }
        
    }
    int getT_distance(double distance , int lastT = 0)
    {
        if(lastT < 0)
            lastT = 0;
        for(int i = lastT ; i < 999 ;i++)
        {
            if(distance >= distance_samples[i] && distance < distance_samples[i+1])
            {
                if(distance <(distance_samples[i] + distance_samples[i+1])/2)
                    return i*2;
                else
                    return i*2+1;
            }
        }
        return -1;
    }
    void draw_path(SDL_Renderer *renderer , bool draw_base = false)
    {
        int t = 0;
        SDL_SetRenderDrawColor(renderer, 255, 0, 0, 0);
        SDL_Point p;
        SDL_Rect dst = {0, 0, 4, 60};
        double x1, x2, y1, y2;
        while (t < 2000)
        {
            get_point(t, &p, &x1, &y1);
            get_point(t + 1, nullptr, &x2, &y2);
            dst.x = p.x;
            dst.y = p.y - dst.h / 2;
            total_lenght += sqrt((y2 - y1) * (y2 - y1) + (x2 - x1) * (x2 - x1));
            if(t%2 ==0)
                SDL_RenderCopyEx(renderer, tex, NULL, &dst, (atan2((double)(y2 - y1), (double)(x2 - x1)) * 180) / M_PI, NULL, SDL_FLIP_NONE);
            if(draw_base)
                SDL_RenderDrawPoint(renderer, p.x, p.y);
            t++;
        }
    }
};
