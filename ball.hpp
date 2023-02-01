struct Ball
{
    SDL_Texture *tex;
    string color;
    SDL_Point center;
    SDL_Rect rect;
    double current_loc = 0;
    int lastT = 0;
    double total_path;
    bool leftConnnected = true, rightConnnected = true;
    bool is_in_cannon = false, is_primary = false;
    double velocity, v_x, v_y, normal_v = 0;
    double angle;
    bool is_entering = false;
    double add_animation_distance;
    bool is_ice = false;
    SDL_Texture *temptex;
    SDL_Point Rcenter;
    int time_effect_mode = NONE;
    Timer time_effect_timer;

    void creat_cannon_ball(SDL_Texture *texture, string mode, Player *player, int width, int v)
    {
        velocity = v;
        tex = texture;
        center.x = player->center.x + player->rect.x;
        center.y = player->center.y + player->rect.y;
        is_in_cannon = true;
        is_primary = true;
        color = mode;
        rect = {0, 0, width, width};
    }
    void shoot(SDL_Point *mouth)
    {
        is_in_cannon = false;
        angle = atan2((double)mouth->y - center.y, (double)mouth->x - center.x);
        v_x = cos(angle) * velocity;
        v_y = sin(angle) * velocity;
    }
    void creat(SDL_Texture *texture, string mode, int loc, int width, int path_lenght)
    {
        velocity = v_x = v_y = 0;
        is_in_cannon = false;
        is_primary = false;
        total_path = path_lenght;
        current_loc = loc;
        normal_v = 0;
        color = mode;
        tex = texture;
        lastT = 0;
        rect = {0, 0, width, width};
    }
    void update(map *ma = nullptr, double v = 0)
    {
        if (is_primary == false)
        {
            if (time_effect_mode != NONE)
                if (time_effect_timer.get_current_time() > 5000)
                    time_effect_mode = NONE;

            if (current_loc > total_path)
                current_loc = 0;
            current_loc += v;
            current_loc += normal_v;
            if (current_loc >= 0)
            {
                lastT = ma->getT_distance(current_loc, (lastT - 5) / 2);
                ma->get_point(lastT, &center);
            }
        }
        else
        {
            if (!is_in_cannon)
            {
                center.x += v_x;
                center.y += v_y;
            }
        }
    }
    bool is_out()
    {
        if (center.x - rect.w / 2 > screenWidth || center.x + rect.w / 2 < 0)
            return true;
        if (center.y - rect.w / 2 > screenHeight || center.y + rect.w / 2 < 0)
            return true;
        return false;
    }
    void Draw(SDL_Renderer *renderer, bool has_second_tex = false, SDL_Texture *secondtex = nullptr)
    {
        if (current_loc >= 0)
        {
            rect.x = center.x - rect.w / 2;
            rect.y = center.y - rect.w / 2;
            SDL_RenderCopy(renderer, tex, NULL, &rect);
            if (has_second_tex)
                SDL_RenderCopy(renderer, secondtex, NULL, &rect);
        }
    }
};
void creat_start_balls(int count_ball, Ball balls[], int path_tatal_lenght, int ball_width, SDL_Texture *Red_marble, SDL_Texture *Green_marble, SDL_Texture *Blue_marble, SDL_Texture *Yellow_marble, SDL_Texture *Red_marble_ice, SDL_Texture *Green_marble_ice, SDL_Texture *Blue_marble_ice, SDL_Texture *Yellow_marble_ice, SDL_Texture *Black_marble, SDL_Texture *Question_marble)
{
    int count_of_ball_mode = 6;
    int chances[] = {3, 3, 3, 3, 1, 1};
    int ice_chance[] = {7, 1};
    int type;
    for (int i = 0; i < count_ball; i++)
    {
        int mode = random(chances, count_of_ball_mode);
        switch (mode)
        {
        case RED:
            type = random(ice_chance, 2);
            if (type == 0)
                balls[i].creat(Red_marble, "Red", (count_ball * -ball_width) + i * ball_width, ball_width, path_tatal_lenght);
            else
            {
                balls[i].creat(Red_marble_ice, "Red", (count_ball * -ball_width) + i * ball_width, ball_width, path_tatal_lenght);
                balls[i].is_ice = true;
                balls[i].temptex = Red_marble;
            }
            chances[RED] = 8;
            for (int i = 0; i < count_of_ball_mode - 2; i++)
                if (i != RED)
                    chances[i] = 3;
            break;
        case GREEN:
            type = random(ice_chance, 2);
            if (type == 0)
                balls[i].creat(Green_marble, "Green", (count_ball * -ball_width) + i * ball_width, ball_width, path_tatal_lenght);
            else
            {
                balls[i].creat(Green_marble_ice, "Green", (count_ball * -ball_width) + i * ball_width, ball_width, path_tatal_lenght);
                balls[i].is_ice = true;
                balls[i].temptex = Green_marble;
            }
            chances[GREEN] = 8;
            for (int i = 0; i < count_of_ball_mode - 2; i++)
                if (i != GREEN)
                    chances[i] = 3;
            break;
        case BLUE:
            type = random(ice_chance, 2);
            if (type == 0)
                balls[i].creat(Blue_marble, "Blue", (count_ball * -ball_width) + i * ball_width, ball_width, path_tatal_lenght);
            else
            {
                balls[i].creat(Blue_marble_ice, "Blue", (count_ball * -ball_width) + i * ball_width, ball_width, path_tatal_lenght);
                balls[i].is_ice = true;
                balls[i].temptex = Blue_marble;
            }
            chances[BLUE] = 8;
            for (int i = 0; i < count_of_ball_mode - 2; i++)
                if (i != BLUE)
                    chances[i] = 3;
            break;
        case YELLOW:
            type = random(ice_chance, 2);
            if (type == 0)
                balls[i].creat(Yellow_marble, "Yellow", (count_ball * -ball_width) + i * ball_width, ball_width, path_tatal_lenght);
            else
            {
                balls[i].creat(Yellow_marble_ice, "Yellow", (count_ball * -ball_width) + i * ball_width, ball_width, path_tatal_lenght);
                balls[i].is_ice = true;
                balls[i].temptex = Yellow_marble;
            }
            chances[YELLOW] = 8;
            for (int i = 0; i < count_of_ball_mode - 2; i++)
                if (i != YELLOW)
                    chances[i] = 3;
            break;
        case Black:
            balls[i].creat(Black_marble, "Black", (count_ball * -ball_width) + i * ball_width, ball_width, path_tatal_lenght);
            for (int i = 0; i < count_of_ball_mode - 2; i++)
                chances[i] = 3;
            break;
        case Question:
            balls[i].creat(Question_marble, "Question", (count_ball * -ball_width) + i * ball_width, ball_width, path_tatal_lenght);
            for (int i = 0; i < count_of_ball_mode - 2; i++)
                chances[i] = 3;
            break;
        }
    }
    balls[0].leftConnnected = false;
    balls[count_ball - 1].rightConnnected = false;
}
void handle_map_balls(int count_ball, Ball balls[], double balls_v, map *ma)
{
    if (balls_v >= 0)
    {
        bool is_connected = true;
        int last = 0;
        for (int i = 0; i < count_ball; i++)
        {
            if (i == 0)
                balls[i].update(ma, balls_v);
            else if (balls[i].leftConnnected && is_connected)
            {
                balls[i].update(ma, balls_v);
                balls[i].current_loc = balls[i - 1].current_loc + balls[i].rect.w;
                balls[i].normal_v = 0;
                last = i;
            }
            else
            {
                if (balls[i].current_loc - balls[i].rect.w <= balls[i - 1].current_loc && last + 1 == i)
                {
                    balls[i - 1].rightConnnected = true;
                    balls[i].leftConnnected = true;
                    is_connected = true;
                    int j = i;
                    while (j < count_ball && balls[j].rightConnnected)
                        balls[j++].normal_v = 0;
                }
                else
                {
                    balls[i].update(ma);
                    is_connected = false;
                }
            }
        }
    }
    else
    {
        bool is_connected = true;
        int last = count_ball - 1;
        for (int i = count_ball - 1; i >= 0; i--)
        {
            if (i == count_ball - 1)
                balls[i].update(ma, balls_v);
            else if (balls[i].rightConnnected && is_connected)
            {
                balls[i].update(ma, balls_v);
                balls[i].current_loc = balls[i + 1].current_loc - balls[i].rect.w;
                balls[i].normal_v = 0;
                last = i;
            }
            else
            {
                if (balls[i].current_loc + balls[i].rect.w >= balls[i + 1].current_loc && last - 1 == i)
                {
                    balls[i].rightConnnected = true;
                    balls[i + 1].leftConnnected = true;
                    is_connected = true;
                    //int j = i;
                    //while (j < count_ball && balls[j].rightConnnected)
                    //    balls[j++].normal_v = 0;
                }
                else
                {
                    balls[i].update(ma);
                    is_connected = false;
                }
            }
        }
    }
}
void delete_ball(Ball balls[], int count_ball, int index = -1, int andises[] = NULL, int andis_count = -1, int *current_time_mode = nullptr, Timer *time = nullptr)
{
    int answer_count;
    if (index == -1)
        answer_count = count_ball - andis_count;
    else
        answer_count = count_ball - 1;
    Ball ans[answer_count];
    int k = 0;
    for (int i = 0; i < count_ball; i++)
    {
        if (index == -1)
        {
            bool is_in_andis = false;
            for (int j = 0; j < andis_count; j++)
                if (andises[j] == i)
                    is_in_andis = true;
            if (!is_in_andis)
                ans[k++] = balls[i];
        }
        else if (index != i)
            ans[k++] = balls[i];
    }
    if (current_time_mode != nullptr)
    {
        int mode = NONE;
        if (index == -1)
        {
            for (int i = 0; i < andis_count; i++)
                if (balls[andises[i]].time_effect_mode != NONE)
                    mode = balls[andises[i]].time_effect_mode;
        }
        else
        {
            if (balls[index].time_effect_mode != NONE)
                mode = balls[index].time_effect_mode;
        }
        if (mode != NONE)
        {
            *current_time_mode = mode;
            time->creat();
        }
    }
    for (int i = 0; i < answer_count; i++)
        balls[i] = ans[i];
}
void make_cannon_ball(int count_ball, Ball balls[], Ball *bullet, int ball_width, int v, Player *player, SDL_Texture *Red_marble, SDL_Texture *Green_marble, SDL_Texture *Blue_marble, SDL_Texture *Yellow_marble)
{
    int count_of_ball_mode = 4;
    int chances[] = {0, 0, 0, 0};
    for (int i = 0; i < count_ball; i++)
    {
        if (balls[i].color == "Red" && chances[RED] == 0)
            chances[RED] = 1;
        if (balls[i].color == "Green" && chances[GREEN] == 0)
            chances[GREEN] = 1;
        if (balls[i].color == "Blue" && chances[BLUE] == 0)
            chances[BLUE] = 1;
        if (balls[i].color == "Yellow" && chances[YELLOW] == 0)
            chances[YELLOW] = 1;
    }
    if(chances[0] == 0 && chances[1] == 0 && chances[2] == 0 && chances[3] == 0 )
        chances[0] = 1;
    int mode = random(chances, count_of_ball_mode);
    switch (mode)
    {
    case RED:
        bullet->creat_cannon_ball(Red_marble, "Red", player, ball_width, v);
        break;
    case GREEN:
        bullet->creat_cannon_ball(Green_marble, "Green", player, ball_width, v);
        break;
    case BLUE:
        bullet->creat_cannon_ball(Blue_marble, "Blue", player, ball_width, v);
        break;
    case YELLOW:
        bullet->creat_cannon_ball(Yellow_marble, "Yellow", player, ball_width, v);
        break;
    }
}
bool check_ball_collision(Ball *b1, Ball *b2)
{
    if (SDL_HasIntersection(&b1->rect, &b2->rect))
        return true;
    else
        return false;
}
bool ball_collision_delete(Ball balls[], int *count_ball, int b, Ball *bullet, bool is_right, double ball_v, int *current_time_mode, Timer *time , int *score)
{
    if (bullet->color == balls[b].color || (is_right && balls[b].rightConnnected && b < *count_ball - 1 && balls[b + 1].color == bullet->color) || (!is_right && balls[b].leftConnnected && b != 0 && balls[b - 1].color == bullet->color))
    {
        if (is_right)
            if (b + 1 >= *count_ball || bullet->color != balls[b].color || !balls[b].rightConnnected || bullet->color != balls[b + 1].color || balls[b + 1].is_ice)
                if (b == 0 || bullet->color != balls[b].color || !balls[b].leftConnnected || bullet->color != balls[b - 1].color || balls[b - 1].is_ice)
                    if (b + 2 >= *count_ball || bullet->color != balls[b + 1].color || !balls[b + 1].rightConnnected || bullet->color != balls[b + 2].color || balls[b + 1].is_ice || balls[b + 2].is_ice)
                        return false;
        if (!is_right)
            if (b + 1 >= *count_ball || bullet->color != balls[b].color || !balls[b].rightConnnected || bullet->color != balls[b + 1].color || balls[b + 1].is_ice)
                if (b == 0 || bullet->color != balls[b].color || !balls[b].leftConnnected || bullet->color != balls[b - 1].color || balls[b - 1].is_ice)
                    if (b - 1 == 0 || bullet->color != balls[b - 1].color || !balls[b - 1].leftConnnected || bullet->color != balls[b - 2].color || balls[b - 1].is_ice || balls[b - 2].is_ice)
                        return false;

        int deleted_count = 0;
        int max_left, max_right;
        int deleted[50];
        if (is_right)
        {
            max_right = b + 1;
            max_left = b;
            for (int i = b; i >= 0; i--)
            {
                if (balls[i].color == "Question")
                {
                    balls[i].color = balls[i + 1].color;
                    balls[i].tex = balls[i + 1].tex;
                    break;
                }
                if (balls[i].color == bullet->color && !balls[i].is_ice)
                {
                    deleted[deleted_count++] = i;
                    max_left--;
                }
                else if (balls[i].is_ice)
                {
                    balls[i].is_ice = false;
                    balls[i].tex = balls[i].temptex;
                    break;
                }
                else
                    break;
                if (!balls[i].leftConnnected)
                    break;
            }
            if (b + 1 < *count_ball)
            {
                for (int i = b + 1; i < *count_ball; i++)
                {
                    if (balls[i].color == "Question")
                    {
                        balls[i].color = balls[i - 1].color;
                        balls[i].tex = balls[i - 1].tex;
                        break;
                    }
                    if (balls[i].color == bullet->color && !balls[i].is_ice)
                    {
                        deleted[deleted_count++] = i;
                        max_right++;
                    }
                    else if (balls[i].is_ice)
                    {
                        balls[i].is_ice = false;
                        balls[i].tex = balls[i].temptex;
                        break;
                    }
                    else
                        break;
                    if (!balls[i].rightConnnected)
                        break;
                }
            }
        }
        else
        {
            max_right = b;
            max_left = b - 1;
            for (int i = b; i < *count_ball; i++)
            {
                if (balls[i].color == "Question")
                {
                    balls[i].color = balls[i - 1].color;
                    balls[i].tex = balls[i - 1].tex;
                    break;
                }
                if (balls[i].color == bullet->color && !balls[i].is_ice)
                {
                    deleted[deleted_count++] = i;
                    max_right++;
                }
                else if (balls[i].is_ice)
                {
                    balls[i].is_ice = false;
                    balls[i].tex = balls[i].temptex;
                    break;
                }
                else
                    break;
                if (!balls[i].rightConnnected)
                    break;
            }
            if (b > 0)
            {
                for (int i = b - 1; i >= 0; i--)
                {
                    if (balls[i].color == "Question")
                    {
                        balls[i].color = balls[i + 1].color;
                        balls[i].tex = balls[i + 1].tex;
                        break;
                    }
                    if (balls[i].color == bullet->color && !balls[i].is_ice)
                    {
                        deleted[deleted_count++] = i;
                        max_left--;
                    }
                    else if (balls[i].is_ice)
                    {
                        balls[i].is_ice = false;
                        balls[i].tex = balls[i].temptex;
                        break;
                    }
                    else
                        break;
                    if (!balls[i].leftConnnected)
                        break;
                }
            }
        }

        if (deleted_count > 1)
        {
            if (max_right < *count_ball)
                balls[max_right].leftConnnected = false;
            if (max_left >= 0)
                balls[max_left].rightConnnected = false;
            if (max_left >= 0 && max_right < *count_ball)
            {
                if (balls[max_left].color == balls[max_right].color)
                {
                    for (int i = max_right; i < *count_ball; i++)
                    {
                        balls[i].normal_v = -4 * ball_v;
                        if (!balls[i].rightConnnected)
                            break;
                    }
                }
            }
            delete_ball(balls, *count_ball, -1, deleted, deleted_count, current_time_mode, time);
            *count_ball -= deleted_count;
            *score += deleted_count;
            return true;
        }
        else
            return false;
    }
    else
        return false;
}
void add_ball_collision(Ball balls[], int *count_ball, int b, Ball *bullet, bool is_right, double ball_v, int ball_width)
{
    //for(int i = 0 ; i < *count_ball ; i++)
    //    balls[i].current_loc+=ball_width;
    // balls[29] = balls[28];
    // balls[29].current_loc+=500;
    int count = *count_ball;
    Ball temp[count + 1];
    if (is_right)
    {
        bullet->creat(bullet->tex, bullet->color, balls[b].current_loc + ball_width, ball_width, balls[b].total_path);
        if (b + 1 != count) // b not at the end
        {
            for (int i = b + 1; i < count; i++)
            {
                if (balls[i].leftConnnected)
                    balls[i].current_loc += ball_width;
                else
                    break;
            }
        }
    }
    else
    {
        bullet->creat(bullet->tex, bullet->color, balls[b].current_loc, ball_width, balls[b].total_path);
        for (int i = b; i < count; i++)
        {
            if (balls[i].rightConnnected)
                balls[i].current_loc += ball_width;
            else
            {
                balls[i].current_loc += ball_width;
                break;
            }
        }
    }
    bullet->is_entering = true;
    // bullet->Rcenter = bullet->center;
    // bullet->add_animation_distance = sqrt((bullet->Rcenter.x - bullet->center.x)*(bullet->Rcenter.x - bullet->center.x) + (bullet->Rcenter.y - bullet->center.y)*(bullet->Rcenter.y - bullet->center.y));
    for (int i = 0; i < count + 1; i++)
    {
        if (i < b)
            temp[i] = balls[i];
        else if (i == b && is_right)
            temp[i] = balls[i];
        else if (i == b && !is_right)
            temp[i] = *bullet;
        else if (i == b + 1 && is_right)
            temp[i] = *bullet;
        else
            temp[i] = balls[i - 1];
    }
    if (is_right)
    {
        temp[b + 1].leftConnnected = true;
        temp[b].rightConnnected = true;
        if (b + 1 != count && temp[b + 2].leftConnnected)
            temp[b + 1].rightConnnected = true;
        else
            temp[b + 1].rightConnnected = false;
    }
    else
    {
        temp[b].rightConnnected = true;
        temp[b + 1].leftConnnected = true;
        if (b != 0 && temp[b - 1].rightConnnected)
            temp[b].leftConnnected = true;
        else
            temp[b].leftConnnected = false;
    }
    for (int i = 0; i < count + 1; i++)
        balls[i] = temp[i];
    *count_ball = count + 1;
}
void collision(Ball balls[], int *count_ball, int b, Ball *bullet, map *ma, double ball_v, int ball_width, int *current_time_mode, Timer *time , int *score)
{
    /* NOTE
        check for special balls
    */
    if (bullet->color == "Bomb")
    {
        int count = 1;
        int start = 0, end = (*count_ball) - 1;
        int andis[9];
        andis[0] = b;
        for (int i = b + 1; i < b + 5; i++)
        {
            if (i < *count_ball && balls[i].leftConnnected)
            {
                andis[count++] = i;
                end = i;
            }
            else
                break;
        }
        for (int i = b - 1; i > b - 5; i--)
        {
            if (i > -1 && balls[i].rightConnnected)
            {
                andis[count++] = i;
                start = i;
            }
            else
                break;
        }

        if (start - 1 >= 0)
            balls[start - 1].rightConnnected = false;
        if (end + 1 < *count_ball)
            balls[end + 1].leftConnnected = false;

        delete_ball(balls, *count_ball, -1, andis, count, current_time_mode, time);
        *count_ball = (*count_ball) - (count);
        *score  += count;
        return;
    }
    else if (bullet->color == "Rainbow")
    {
        if (balls[b].color == "Red" || balls[b].color == "Blue" || balls[b].color == "Yellow" || balls[b].color == "Green")
        {
            int andis[100];
            int count = 0;
            for (int i = 0; i < *count_ball; i++)
            {
                if (balls[i].color == balls[b].color)
                {
                    andis[count++] = i;
                    if (i > 0)
                        balls[i - 1].rightConnnected = false;
                    if (i + 1 < *count_ball)
                        balls[i + 1].leftConnnected = false;
                }
            }
            delete_ball(balls, *count_ball, -1, andis, count, current_time_mode, time);
            *count_ball = (*count_ball) - (count);
            *score  += count;
        }
        return;
    }
    else if (bullet->color == "Fireball")
    {
        if (balls[b].color == "Red" || balls[b].color == "Blue" || balls[b].color == "Yellow" || balls[b].color == "Green" || balls[b].color == "Black" || balls[b].color == "Question")
        {
            if (b > 0)
                balls[b - 1].rightConnnected = false;
            if (b + 1 < *count_ball)
                balls[b + 1].leftConnnected = false;
            delete_ball(balls, *count_ball, b, NULL, -1, current_time_mode, time);
            *count_ball -= 1;
            *score  += 1;
        }
        return;
    }
    if (balls[b].color == "Black")
    {
        if (b > 0)
            balls[b - 1].rightConnnected = false;
        if (b + 1 < *count_ball)
            balls[b + 1].leftConnnected = false;
        delete_ball(balls, *count_ball, b);
        *count_ball -= 1;
        *score  += 1;
        return;
    }
    else if (balls[b].is_ice)
    {
        balls[b].is_ice = false;
        balls[b].tex = balls[b].temptex;
    }
    bool is_right;
    double x1, y1, x2, y2;
    ma->get_point(ma->getT_distance(balls[b].current_loc + 15), nullptr, &x1, &y1);
    ma->get_point(ma->getT_distance(balls[b].current_loc - 15), nullptr, &x2, &y2);
    double dis1 = sqrt((bullet->center.x - x1) * (bullet->center.x - x1) + (bullet->center.y - y1) * (bullet->center.y - y1));
    double dis2 = sqrt((bullet->center.x - x2) * (bullet->center.x - x2) + (bullet->center.y - y2) * (bullet->center.y - y2));
    if (dis1 > dis2)
        is_right = false;
    else
        is_right = true;
    if (!ball_collision_delete(balls, count_ball, b, bullet, is_right, ball_v, current_time_mode, time , score))
    {
        add_ball_collision(balls, count_ball, b, bullet, is_right, ball_v, ball_width);
    }
}
