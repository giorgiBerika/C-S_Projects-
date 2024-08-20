#include <iostream>

using namespace std;

void simple_pyramid()
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


void flipped_simple_pyramid()
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

void inverted_pyramid()
{
    unsigned int height, i, j; 
    cout<<"Enter the height of Pyramid: ";
    cin>>height;
   
    for(i = height; i >= 1; -- i)
    {
        for(j = 1; j <= i; ++j)
        {
            cout<<"*";
        };
        cout<<"\n";

    };

};

void flipped_inverted_pyramid()
{
    
    unsigned int height, i, j, k, space; 
    cout<<"Enter the height of Pyramid: ";
    cin>>height;

    for(i = height; i >= 1; --i)
    {
        space = height - i;
        for(j = 0; j < space; ++j)
        {
            cout<<" ";
        }
        for(k = 0; k < i; ++k)
        {
            cout<<"*";
        }
        cout<<"\n";
        
    };

};

void triangle()
{

    unsigned int height, i, j, k, max_space; 
    cout<<"Enter the height of Pyramid: ";
    cin>>height;

    max_space =  height;

    for(i = height; i >= 1; --i)
    {
        
        for(k = 1; k <= max_space; ++k )
        {
            if(k < i)
            {
                cout<<" ";
            }
            else 
            {
                if(i % 2 != 0)
                {
                    if(k % 2 != 0)
                    {
                        cout<<"*";
                    }else
                    {
                        cout<<" ";
                    }
                }
                else if(i % 2 == 0)
                {
                    if(k % 2 == 0)
                    {
                        cout<<"*";
                    }else
                    {
                        cout<<" ";
                    }
                }
                
            };
            
        };
        cout<<"\n";
        ++max_space;
    }

};

int main()
{
    // inverted_pyramid();
    // flipped_inverted_pyramid();
    triangle();
  

    return 0;
}
