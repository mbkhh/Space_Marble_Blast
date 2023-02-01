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