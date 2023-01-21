int random(int chances[] , int count)
{
    int total_chance=0;
    for(int i = 0 ; i < count ; i++)
        total_chance +=  chances[i];
    int winner = rand()%total_chance;
    int c = 0;
    for(int i = 0 ; i < count ; i++)
    {
        c += chances[i];
        if(winner < c)
            return i;
    }
    return -1;
}