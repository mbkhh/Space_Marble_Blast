struct Music_player
{
    Mix_Chunk *Mclick;
    Mix_Chunk *Mfire;
    Mix_Chunk *Mexplotion;
    Mix_Chunk *Mhit;
    bool is_effect_on = true;
    void play_chunk(string music)
    {
        if(is_effect_on)
        {
            if(music == "click")
                Mix_PlayChannel( -1, Mclick, 0 );
            if(music == "fire")
                Mix_PlayChannel( -1, Mfire, 0 );
            if(music == "explotion")
                Mix_PlayChannel( -1, Mexplotion, 0 );
            if(music == "hit")
                Mix_PlayChannel( -1, Mhit, 0 );
        }
    }
};