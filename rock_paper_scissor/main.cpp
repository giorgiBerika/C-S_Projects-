// Include all extra necessary library types
#include <iostream>
#include <random>
#include <string>
#include <map>
#include <array>
#include <algorithm>

// Function that returns randomly chosen weapon for PC 
std::string getRandomPcWeapon(const std::map<int, std::string>&  weaponsMap)
{
    std::random_device rd;
    int rand_num = rd() % weaponsMap.size();

    return weaponsMap.at(rand_num);
}; 

// Function to transform inputed value to lowercase string
std::string toLowerCase(std::string word)
{
    std::transform(word.begin(), word.end(), word.begin(), ::tolower);
    return word;
};

// Function to determine the winner according to game rules
std::string determineWinner(std:: string user, std::string pc,
                      unsigned int &user_p, unsigned int &pc_p,
                        const std::map<std::string, std::string>& winConditions)
{
    user = toLowerCase(user);
    pc = toLowerCase(pc);

    std::string winner;

    if(user == pc)
    {
        winner = "Tie!";
    }
    else if (winConditions.at(user) == pc)
    {
        ++user_p;
        winner = "User";
    }else 
    {
        ++pc_p;
        winner = "Pc";
    }

    winner += (winner == "Tie!") ? " " : " Wins!";

    return winner;
};

// Function prove validation of the input value
bool isValidInput(std::string input, std::array<std::string, 3> arr)
{
  
    bool res = false;
    int size = arr.size();
    input = toLowerCase(input);

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

// Main function
int main()
{
    // Map of the weapons
    std::map<int, std::string> weaponsMap;

    weaponsMap[0] = "Rock";
    weaponsMap[1] = "Paper";
    weaponsMap[2] = "Scissor";

    // Map of win conditions
    std::map<std::string, std::string> winConditions = {

        {"rock", "scissor"},
        {"paper", "rock"},
        {"scissor", "paper"}
    };

    // Array of weapons

    std::array<std::string, 3> arr = {"rock", "paper", "scissor"};

    // Welcome text
    std::cout<<"Hello there!\n You're playing a legendary game -- "
                "Rock Paper Scissor! --"<<std::endl;
    std::cout<<"----------------------------------------------\n"<<std::endl;

    // Game variables
    std::string user_input, pc_weapon;
    bool game_running = true;
    // Points
    unsigned int user_points = 0, pc_points = 0;

    
    // Game loop
    while (game_running)
    {
        std::cout<<"Play: ";
        std::cin>>user_input;

        // Quit mode/event
        if (user_input == "Quit")
        {
            // Call for points
            std::cout<<"\n-----------\n";
            std::cout<<"User Points: "<<user_points<<"   "<<
                        "Pc Points:  "<<pc_points<<std::endl;
            break;
        };        
        // Pc choose
        pc_weapon = getRandomPcWeapon(weaponsMap);
        

        // Result outPut
        if(isValidInput(user_input, arr))
        {
            std::cout<<"User: "<<user_input<<" ---- "
                    "Pc: "<<pc_weapon<<"\n";

            std::cout<<determineWinner(user_input, pc_weapon, user_points, pc_points, winConditions)<<"\n\n";
        }
        else
        {
            std::cout<<"Bad Input!\n";
        }
        
    };

    return 0;
}