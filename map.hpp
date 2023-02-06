struct map
{
    SDL_Point p1, p2, p3, p4, p5, p6, p7 , p8 , p9 , p10;
    SDL_Texture *tex;
    double total_lenght;
    double distance_samples[3000];
    void get_point(int t, SDL_Point *x = nullptr, double *x2 = nullptr, double *y2 = nullptr)
    {
        double X, Y;
        if (t > 2000)
        {
            t -= 2000;
            X = pow((1.0 - t / 1000.0), 3) * p7.x + 3 * pow((1.0 - t / 1000.0), 2) * (t / 1000.0) * p8.x + 3 * (1.0 - t / 1000.0) * pow(t / 1000.0, 2) * p9.x + pow(t / 1000.0, 3) * p10.x;
            Y = pow((1.0 - t / 1000.0), 3) * p7.y + 3 * pow((1.0 - t / 1000.0), 2) * (t / 1000.0) * p8.y + 3 * (1.0 - t / 1000.0) * pow(t / 1000.0, 2) * p9.y + pow(t / 1000.0, 3) * p10.y;
        }
        else if (t > 1000)
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
    int getT_distance(double distance, int lastT = 0)
    {
        if (lastT < 0)
            lastT = 0;
        for (int i = lastT; i < 2999; i++)
            if (distance >= distance_samples[i] && distance < distance_samples[i + 1])
                return i;
        return -1;
    }
    void draw_path(SDL_Renderer *renderer, bool draw_base = false)
    {
        int t = 1;
        SDL_SetRenderDrawColor(renderer, 255, 0, 0, 0);
        SDL_Point p, p_last = p1;
        SDL_Rect dst = {0, 0, 4, 60};
        double x1, x2, y1, y2;
        x1 = p_last.x;
        y1 = p_last.y;
        distance_samples[0] = 0;
        while (t < 3000)
        {
            get_point(t, &p, &x2, &y2);
            //get_point(t + 1, nullptr, &x2, &y2);
            dst.x = p.x;
            dst.y = p.y - dst.h / 2;
            total_lenght += sqrt((y2 - y1) * (y2 - y1) + (x2 - x1) * (x2 - x1));
            distance_samples[t] = total_lenght;
            SDL_RenderCopyEx(renderer, tex, NULL, &dst, (atan2((double)(y2 - y1), (double)(x2 - x1)) * 180) / M_PI, NULL, SDL_FLIP_NONE);
            if (draw_base)
                SDL_RenderDrawPoint(renderer, p.x, p.y);
            p_last = p;
            x1 = x2;
            y1 = y2;
            t++;
        }
    }
};
void random_map(int *playerX , int *playerY , map *ma)
{
    int width = screenWidth - 100;
    int height = screenHeight;
    *playerX = rand()%(width - 300) + 150;
    *playerY = rand()%(height - 300) + 150;
    do
    {
        ma->p1.x = rand()%(width - 100) + 50;
        ma->p1.y = rand()%(height - 100) + 50;
    } while (sqrt((*playerY - ma->p1.y) * (*playerY - ma->p1.y) + (*playerX - ma->p1.x) * (*playerX - ma->p1.x)) <= 600 );
    do
    {
        ma->p4.x = rand()%(width - 100) + 50;
        ma->p4.y = rand()%(height - 100) + 50;
    } while (sqrt((*playerY - ma->p4.y) * (*playerY - ma->p4.y) + (*playerX - ma->p4.x) * (*playerX - ma->p4.x)) <= 600 || sqrt((ma->p1.y - ma->p4.y) * (ma->p1.y - ma->p4.y) + (ma->p1.x - ma->p4.x) * (ma->p1.x - ma->p4.x)) <= 600 );
    do
    {
        ma->p7.x = rand()%(width - 100) + 50;
        ma->p7.y = rand()%(height - 100) + 50;
    } while (sqrt((*playerY - ma->p7.y) * (*playerY - ma->p7.y) + (*playerX - ma->p7.x) * (*playerX - ma->p7.x)) <= 600 || sqrt((ma->p4.y - ma->p7.y) * (ma->p4.y - ma->p7.y) + (ma->p4.x - ma->p7.x) * (ma->p4.x - ma->p7.x)) <= 600 );
    do
    {
        ma->p10.x = rand()%(width - 100) + 50;
        ma->p10.y = rand()%(height - 100) + 50;
    } while (sqrt((*playerY - ma->p10.y) * (*playerY - ma->p10.y) + (*playerX - ma->p10.x) * (*playerX - ma->p10.x)) <= 600 || sqrt((ma->p7.y - ma->p10.y) * (ma->p7.y - ma->p10.y) + (ma->p7.x - ma->p10.x) * (ma->p7.x - ma->p10.x)) <= 600 );
    ma->p2.x = rand()%(width - 100) + 50;
    ma->p2.y = rand()%(height - 100) + 50;
    ma->p3.x = rand()%(width - 100) + 50;
    ma->p3.y = rand()%(height - 100) + 50;
    
    double m = -1 * (double)(ma->p3.y - ma->p4.y)/ (double)(ma->p3.x - ma->p4.x);
    int max_x = 200 / sqrt(m*m + 1);
    int x = rand() % max_x;

    ma->p5.x = ma->p4.x - x ;
    ma->p5.y = ma->p4.y - x*m;

    ma->p6.x = rand()%(width - 100) + 50;
    ma->p6.y = rand()%(height - 100) + 50;

    m = -1 * (double)(ma->p6.y - ma->p7.y)/ (double)(ma->p6.x - ma->p7.x);
    max_x = 200 / sqrt(m*m + 1);
    x = rand() % max_x;
    ma->p8.x = ma->p7.x - x;
    ma->p8.y = ma->p7.y - x*m;

    ma->p9.x = rand()%(width - 100) + 50;
    ma->p9.y = rand()%(height - 100) + 50;
}