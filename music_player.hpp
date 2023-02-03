struct Music_player
{
    Mix_Chunk *Mclick;
    bool is_effect_on = true;
    void play_chunk(string music)
    {
        if(is_effect_on)
        {
            if(music == "click")
                Mix_PlayChannel( -1, Mclick, 0 );
        }
    }
};