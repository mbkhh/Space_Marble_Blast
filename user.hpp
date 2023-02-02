struct User
{
    string username;
    string password;
    int fireball_power,bomb_power,lightning_power,rainbow_power,missile_power;
    int max_normal,max_timer,max_stone,max_fly;
};
void write_users(User users[] , int count)
{
    ofstream profiles("profiles.dat");
    for(int i = 0 ; i < count ; i++)
    {
        profiles << users[i].username <<";"<<users[i].password <<";"<<to_string(users[i].fireball_power) <<";"<<to_string(users[i].bomb_power) <<";"<<to_string(users[i].lightning_power) <<";"<<to_string(users[i].rainbow_power) <<";"<<to_string(users[i].missile_power) <<";"<<to_string(users[i].max_normal) <<";"<<to_string(users[i].max_timer) <<";"<<to_string(users[i].max_stone)<<";"<<to_string(users[i].max_fly);
        if(i + 1 < count)
            profiles<<"\n";
    }
    profiles.close();
}
void print_user(User user)
{
    cout<<  user.username <<";"<<user.password <<";"<<to_string(user.fireball_power) <<";"<<to_string(user.bomb_power) <<";"<<to_string(user.lightning_power) <<";"<<to_string(user.rainbow_power) <<";"<<to_string(user.missile_power) <<";"<<to_string(user.max_normal) <<";"<<to_string(user.max_timer) <<";"<<to_string(user.max_stone)<<";"<<to_string(user.max_fly);
}
int load_users(User users[])
{
    ifstream profiles("profiles.dat");
    string line;
    int count = 0;
    int andis,last;
    while (getline (profiles, line)) {
        if(line != "")
        {
            andis = line.find(';');
            users[count].username = line.substr(0 , andis);
            last = andis;
            andis = line.find(';' , andis+1);
            users[count].password = line.substr(last+1 , andis-last-1);
            last = andis;
            andis = line.find(';' , andis+1);
            users[count].fireball_power = stoi(line.substr(last+1 , andis-last-1));
            last = andis;
            andis = line.find(';' , andis+1);
            users[count].bomb_power = stoi(line.substr(last+1 , andis-last-1));
            last = andis;
            andis = line.find(';' , andis+1);
            users[count].lightning_power = stoi(line.substr(last+1 , andis-last-1));
            last = andis;
            andis = line.find(';' , andis+1);
            users[count].rainbow_power = stoi(line.substr(last+1 , andis-last-1));
            last = andis;
            andis = line.find(';' , andis+1);
            users[count].missile_power = stoi(line.substr(last+1 , andis-last-1));
            last = andis;
            andis = line.find(';' , andis+1);
            users[count].max_normal = stoi(line.substr(last+1 , andis-last-1));
            last = andis;
            andis = line.find(';' , andis+1);
            users[count].max_timer = stoi(line.substr(last+1 , andis-last-1));
            last = andis;
            andis = line.find(';' , andis+1);
            users[count].max_stone = stoi(line.substr(last+1 , andis-last-1));
            last = andis;
            andis = line.find(';' , andis+1);
            users[count].max_fly = stoi(line.substr(last+1 , andis-last-1));
            count++;
        }
    }
    profiles.close();
    return count;
}
int is_user_exist(User users[] , int count , string username)
{
    for(int i = 0 ; i < count ; i++)
    {
        if(users[i].username == username)
            return i;
    }
    return -1;
}
