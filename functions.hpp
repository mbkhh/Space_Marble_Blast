int random(int chances[], int count)
{
    int total_chance = 0;
    for (int i = 0; i < count; i++)
        total_chance += chances[i];
    int winner = rand() % total_chance;
    int c = 0;
    for (int i = 0; i < count; i++)
    {
        c += chances[i];
        if (winner < c)
            return i;
    }
    return -1;
}
bool search(int arr[] , int count , int a)
{
    for(int i = 0 ; i < count ; i++)
        if(arr[i] == a)
            return true;
    return false;
}
void random_array(int arr[] , int count , int start , int end)
{
    int c = 0;
    int temp;
    for(int i = 0 ; i < count ; i++)
        arr[i] = -1;
    while(c < count)
    {
        do
        {
            temp = rand()%(end-start) - start;
        } while (search(arr , c , temp));
        arr[c++] = temp;
    }
}
void draw_text(SDL_Renderer *renderer , string text , TTF_Font *font , int x , int y , Uint8 R , Uint8 G  , Uint8 B , int cw = 1 , int ch = 1)
{
    SDL_Surface *text_surface;
    SDL_Texture *text_texture;
    SDL_Color text_color = {R, G, B};
    SDL_Rect text_rect = {x, y, 0, 0};

    text_surface = TTF_RenderText_Solid(font, text.c_str(), text_color);
    text_texture = SDL_CreateTextureFromSurface(renderer, text_surface);
    text_rect.w = text_surface->w * cw;
    text_rect.h = text_surface->h * ch;
    SDL_RenderCopy(renderer, text_texture, NULL, &text_rect);

    SDL_FreeSurface(text_surface);
    SDL_DestroyTexture(text_texture);
}