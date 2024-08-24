// #include <iostream>
// #include <fstream>
// #include <string>
// #include <deque>

// using namespace std;

// int main()
// {
//     ofstream outFile("data.txt");

//     string login, password;

//     if (outFile.is_open())
//     {
//         cout<<"Login: ";
//         cin>>login;
//         cout<<"password: ";
//         cin>>password;

//         outFile<<login<<"\n";
//         outFile<<password;

//         outFile.close();
//     }


//     // Reading from the file

//     ifstream inFile("data.txt");
//     string data;

//     if (inFile.is_open())
//     {
//         while(getline(inFile, data))
//         {
//             cout<<"Info: "<<data<<endl;
//         }
//         inFile.close();
//     }


//     return 0;
// }


#include <iostream>
#include <fstream>
#include <string>
using namespace std;

void welcomeText() {
    string dashLine(50, '-'), smallDashLine(21, '-');

    cout<<dashLine<<endl;
    cout<<smallDashLine<<"Welcome!"<<smallDashLine<<endl;
    cout<<"\nPlease choose the operation: \n"<<endl;
    cout<<dashLine<<"\n\n";
    cout<<"Login -> l\n";
    cout<<"Registration -> r\n";
    cout<<"Quit -> q\n";
    cout<<"\n\n";

};

bool isRegistered(const string filePath, const string login )
{   
    ifstream inFile(filePath);

    if(!inFile)
    {   
        cerr << "Unable to open file";
    }
    else
    {
        string line;
        while(getline(inFile, line))
        {
            size_t spacePos = line.find(' ');
            string firstWord = line.substr(0, spacePos);

            if (firstWord == login)
            {
                return true;
            }

        }
    }

    return false;

};

bool isValidPassword(const string &password)
{
    auto passSize = password.size();
    unsigned short passSizeLimit(5);

    if(passSize < passSizeLimit)
    {
        return false;
    }
    else
    {
        string prohibitedSymb("!@#$?");
        
        for(const char c: password)
        {
            if(prohibitedSymb.find(c) != string::npos)
            {
                return false;
            }
        }
        

    }
    return true;

};

void registrationLogic() {

    string login, password, loginLine;

    cout<<"Start Registration: \n";
    cout<<"Create login: ";
    cin>>login;

    cout<<"\nPass: At least 5 characters.\n"
          "Do NOT use: !, @, #, $, ?\n";
    cout<<"Create password: ";
    cin>>password;

    while(!isValidPassword(password))
    {
        cout<<"Bad password, Try other: ";
        cin>>password;
    }

    // Open file for Appending/Writing
    ofstream outFile("data.txt", ios::app);

    if(!outFile)
    {
        cerr << "Unable to open file";
    }
    else
    {
        if(isRegistered(("data.txt"), login))
        {
            cout<<"Login exist!"<<endl;
        }
        else
        {
            loginLine = login+" "+password;
            outFile<<loginLine<<endl;
            cout<<"Registration complete!"<<endl;

        }

    }

};

void loginLogic() {
    string inputLogin, inputPassword, loginLine; 

    cout<<"Enter your login: ";
    cin>>inputLogin;
    cout<<"Enter your password: ";
    cin>>inputPassword;
    loginLine = inputLogin+' '+inputPassword;

    // Open file for reading

    ifstream inFile("data.txt");

    if(!inFile)
    {
        cerr << "Unable to open file";
    }
    else
    {
        string line;
        bool isCorrect;

        while (getline(inFile, line))
        {
            if(line == loginLine)
            {
                isCorrect = true;
                break;
            }
        }

        if(isCorrect)
        {
            cout<<"Welcome back <3"<<endl;
        }
        else
        {
            cout<<"Wrong login or password, try again!"<<endl;
            cout<<"Or try Registration\n\n";
        }

    }
};



int main() {

    char op;

    welcomeText();

    cout<<"Operation: ";
    cin>>op;
    while(op != 'q')
    {
        switch (op)
        {
        case 'l':
            loginLogic();
            break;
        case 'r':
            registrationLogic();
            break;
        case 'q':
            cout<<"End operation";
            break;
        }
        cout<<"Operation: ";
    cin>>op;
    }

    return 0;
}