struct Inputbox
{
    SDL_Texture *norm_tex;
    SDL_Texture *selected_tex;
    bool is_selected;
    string text;
    SDL_Rect rect;
    SDL_Color text_color;
    SDL_Rect text_rect;
    void create(SDL_Texture *norm_texture , SDL_Texture *selected_texture ,int x , int y , int w , int h , int margin , Uint8 R , Uint8 G , Uint8 B)
    {
        norm_tex = norm_texture;
        selected_tex = selected_texture;
        rect = {x , y , w , h};
        text_rect.x = x + margin;
        text_rect.y = y + margin;
        text = "";
        is_selected = false;
        text_color = {R , G , B};
    }
    bool select(SDL_Point *mouse)
    {
        if (rect.x <= mouse->x && rect.x + rect.w >= mouse->x && rect.y <= mouse->y && rect.y + rect.h >= mouse->y)
        {
            is_selected = true;
            return true;
        }
        return false;
    }
    void input(Keyboard_handler *keyboard)
    {
        if(is_selected)
        {
            char current;
            current = keyboard->get_current();
            if(current == '@')
            {
                if(text.length() > 0)
                    text = text.substr(0 , text.length()-1);
            }
            else if(current != '!')
                text += current;
        }
    }
    void Draw(SDL_Renderer *renderer , TTF_Font *font)
    {
        if(is_selected)
            SDL_RenderCopy(renderer , selected_tex , NULL , &rect);
        else
            SDL_RenderCopy(renderer , norm_tex , NULL , &rect);
        if(text != "")
        {
            // text_surface = TTF_RenderText_Solid(font, text.c_str(), text_color);
            // text_texture = SDL_CreateTextureFromSurface(renderer, text_surface);
            // text_rect.w = text_surface->w;
            // text_rect.h = text_surface->h;
            // SDL_RenderCopy(renderer, text_texture, NULL, &text_rect);
            draw_text(renderer , text , font , text_rect.x , text_rect.y , text_color.r , text_color.g , text_color.b );
        }
    }
};