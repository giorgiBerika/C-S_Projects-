#include <iostream>
#include <fstream>
#include <string>

using namespace std;

const string fileName = "data.txt";
const int MIN_PASSWORD_LENGTH = 5;

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

bool isRegistered(const string& filePath, const string& login )
{   
    ifstream inFile(filePath);

    if(!inFile)
    {   
        throw runtime_error("Unable to open file");
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

bool isValidPassword(const string& password)
{
    auto passSize = password.size();
    unsigned short passSizeLimit(MIN_PASSWORD_LENGTH);

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
    ofstream outFile(fileName, ios::app);

    if(!outFile)
    {
        throw runtime_error("Unable to open file");
    }
    else
    {
        if(isRegistered((fileName), login))
        {
            cout<<"Login exist!"
                " Try Register other Login!"<<endl;

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

    ifstream inFile(fileName);

    if(!inFile)
    {
        throw runtime_error("Unable to open file");
    }
    else if(inputLogin.empty() || inputPassword.empty())
    {
        cout << "Login and password cannot be empty!" <<endl;
        return;
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