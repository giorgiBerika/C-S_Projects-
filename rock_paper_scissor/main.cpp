#include <iostream>
#include <random>
#include <string>
#include <map>
#include <array>

std::string random_pc_weapon(std::map<int, std::string> weapons_map )
{
    std::random_device rd;
    int rand_num = rd() % 3;

    return weapons_map[rand_num];
}; 

std::string to_lower(std::string word)
{
    for (char& c : word)
    {
        c = std::towlower(c);
    }

    return word;
};


std::string win_logic(std:: string user, std::string pc)
{
    user = to_lower(user);
    pc = to_lower(pc);

    std::string winner;

    if (user == pc)
    {
        winner =  "Tie!";
    }
    else if(user == "rock")
    {
        if(pc == "paper")
        {
            winner =  "Pc";
        }
        else if (pc == "scissor")
        {
            winner = "User";
        }
    }
    else if(user == "paper")
    {
        if(pc == "rock")
        {
            winner =  "User";
        }
        else if (pc == "scissor")
        {
            winner =  "Pc";
        }
    }
    else if(user == "scissor")
    {
        if(pc == "rock")
        {
            winner = "Pc";
        }
        else if (pc == "paper")
        {
            winner =  "User";
        }
    }

    winner += (winner == "Tie!") ? " " : " Wins!";

    return winner;
};

bool valid_input(std::string input, std::array<std::string, 3> arr)
{
  // std::string weapons
    bool res = false;
    int size = arr.size();
    input = to_lower(input);

    for(int i = 0; i < size; ++i)
    {
        if (arr[i] == input)
        {
            res = true;
            break;
        }
    }

    return res;
};

int main()
{
    // Map of the weapons
    std::map<int, std::string> wepons_map;

    wepons_map[0] = "Rock";
    wepons_map[1] = "Paper";
    wepons_map[2] = "Scissor";
    // Array of weapons
    std::string weapons_arr[] = {"rock", "paper", "scissor"};
    std::array<std::string, 3> arr = {"rock", "paper", "scissor"};

    // Welcome text
    std::cout<<"Hello there!\n You're playing a legendary game -- "
                "Rock Paper Scissor! --"<<std::endl;
    std::cout<<"----------------------------------------------\n"<<std::endl;

    std::string user_input, pc_weapon;
    bool game_running = true;

    // First input
    
    std::cout<<"Play: ";
    std::cin>>user_input;
    // Game loop
    while (game_running)
    {
        // Quit mode/event
        if (user_input == "Quit")
            // game_running = false;
            break;
        
        // Pc choose
        pc_weapon = random_pc_weapon(wepons_map);
        

        // Result outPut
        if(valid_input(user_input, arr))
        {
            std::cout<<"User: "<<user_input<<" ---- "
                    "Pc: "<<pc_weapon<<std::endl;

            std::cout<<win_logic(user_input, pc_weapon)<<"\n\n";
        }
        else
        {
            std::cout<<"Bad Input!\n";
        }
        
        
        std::cout<<"Play: ";
        std::cin>>user_input;

    };

    return 0;
}