#include <iostream>
#include <random>

int main()
{
    std::random_device  rd;
    int starter_rand_num = rd( ) % 100;
   
    std::cout<<"Hello!\nThis is * Number Guessing Game *\nYou have to guess "
                "a random number from 0 to 100. \nRemember after every 3 fail "
                "number changes!!\n\n"
                "-------------------------------------------\n\n"<<std::endl;
    
    
    int input_num, count = 0;
    
    std::cout<<"First shot: ";
    std::cin>>input_num;

    while (starter_rand_num != input_num)
    {
        if(count == 3)
        {
            int new_rand = rd() % 100;
            starter_rand_num = new_rand;
            std::cout<<"Number changed! Try: ";
            count = 0;
        }
        else
        {
            if (input_num > starter_rand_num)
            {
                std::cout<<"Wrong! Try Lower: ";
            }
            else
            {
                std::cout<<"Wrong! Try Higher: ";
            }
            ++count;
        }
        
        std::cin>>input_num;
    }
    
    std::cout<<"HeadShot!  Well done! ";

    return 0;
}