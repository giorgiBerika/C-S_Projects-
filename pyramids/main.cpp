#include <iostream>

using namespace std;

void left_pyramid()
{
    int input_val;
    cout<<"Enter the height of Pyramid: ";
    cin>>input_val;
    
    for(int i = 1; i <= input_val; ++i)
    {
        for(int j = 0; j < i; ++j)
        {
            cout<<"*";
        }
        cout<<"\n";
    }
};


void right_pyramid()
{
    int height, space;
    cout<<"Enter the height of Pyramid: ";
    cin>>height;
    
    for(int i = 1; i <= height; ++i)
    {
        space = height - i;
        
        for(int j = 0; j < space; ++j)
        {
            cout<<" ";
        }
        for(int k = 0; k < i; ++k)
        {
            cout<<"*";
        }        
        
        cout<<"\n";
    }
};

int main()
{
    left_pyramid();
    right_pyramid();

    return 0;
}
