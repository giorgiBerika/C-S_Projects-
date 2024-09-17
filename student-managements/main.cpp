#include <iostream>
#include <memory>           // For smart pointers
#include <mysql_driver.h>    // MySQL driver
#include <mysql_connection.h>
#include <cppconn/statement.h>
#include <cppconn/resultset.h>
#include <cppconn/exception.h>
#include <cppconn/prepared_statement.h>

#include <limits>
#include <string>

// Constants

const std::string tabSpace(20, ' ');
const std::string dashBar(29, '-');
const std::string fullDashLine = tabSpace + dashBar;

// Print Menu functions ///
void printMenuOption(int optionNumber, const std::string &description)
{
    std::cout << tabSpace << optionNumber << ". " << description << std::endl;
};

void menuInfo()
{

    std::cout<<fullDashLine<<std::endl;
    std::cout<<tabSpace<<"| STUDENT MANAGEMENT SYSTEM |"<<std::endl;
    std::cout<<fullDashLine<<"\n"<<std::endl;

    printMenuOption(1, "Enter New Record");
    printMenuOption(2, "Search Record");
    printMenuOption(3, "See Full list");
    printMenuOption(4, "Modify Record");
    printMenuOption(5, "Delete Record");
    printMenuOption(6, "Exit");
      
    std::cout<<"\n";
    std::cout<<fullDashLine<<std::endl;
    std::cout<<tabSpace<<"Choose Option: [1/2/3/4/5]"<<std::endl;
    std::cout<<fullDashLine<<std::endl;
};

// Utitlity to clear input buffer and handle invalid input

void clearInput()
{
    std::cin.clear();
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    
};

// Input Validation for integers (age)

int getValidInt(const std::string &prompt)
{
    int value;
    std::cout << prompt;
    while(!(std::cin >> value))
    {
        clearInput();
        std::cout << "Invalid input. " << prompt;
    }
    clearInput();
    return value;
}

class Student
{
    public:
        std::string name;
        int age;
        std::string major;

        void inputStudentInfo()
        {
            
            std::cout << "Enter student Name: ";
            getline(std::cin, name);    // Name input
            
            age = getValidInt("Enter student Age: "); // Age input

            std::cout << "Enter student Major: "; 
            getline(std::cin, major); // Major input
        }


        void saveToDatabase (std::unique_ptr<sql::Connection>& con)
        {

            std::unique_ptr<sql::PreparedStatement> pstmt(
                con->prepareStatement("INSERT INTO students (name, age, major) VALUES (?, ?, ?)"));

            pstmt->setString(1, name);
            pstmt->setInt(2, age);
            pstmt->setString(3, major);
            pstmt->executeUpdate();
            std::cout << "Student data saved to database." << std::endl;

        }    
        

};

void saveNewRecord(std::unique_ptr<sql::Connection>& con)
{
        Student student;
        student.inputStudentInfo();
        student.saveToDatabase(con);
};

void searchRecord(std::unique_ptr<sql::Connection>& con)
{
    std::string studentName;
    
    std::cout << "Enter student's name: ";
    getline(std::cin, studentName);

    std::unique_ptr<sql::PreparedStatement> pstmt(
        con->prepareStatement("SELECT * FROM students WHERE name = ?"));

    pstmt->setString(1, studentName);


    std::unique_ptr<sql::ResultSet> res(pstmt->executeQuery());
    // Process the result set
    
    if(res->next())
    {
        std::cout << "Result:\n";
        std::cout << "Name: " << res->getString("name") << "\n";
        std::cout << "Age: " << res->getInt("age") << "\n";
        std::cout << "Major: " << res->getString("major") << std::endl;
    } else {
        std::cout << "No matching student found.\n";
    }
    
};

// Full List of Students
void fullList(std::unique_ptr<sql::Connection>& con)
{
    std::cout << "Full List of Students: \n" << std::endl;

    std::unique_ptr<sql::Statement> stmt(con->createStatement());
    
    std::unique_ptr<sql::ResultSet> res(stmt->executeQuery("SELECT * FROM students"));

    int count(1);
    while (res->next())
    {   
        std::cout << count << ")\n";
        std::cout << "Name: " << res->getString("name") << "\n";
        std::cout << "Age: " << res->getString("age") << "\n";
        std::cout << "Major: " << res->getString("major") << "\n\n";

        ++count;
    } 
}

// Delete Record
void deleteRecord(std::unique_ptr<sql::Connection>& con)
{   
    std::string studName;
    
    std::cout<<"Enter student's name: ";
    getline(std::cin, studName);

    std::unique_ptr<sql::PreparedStatement> pstmt(
        con->prepareStatement("DELETE FROM students WHERE name = ?"));
    pstmt->setString(1, studName);

    int affectdRows = pstmt->executeUpdate();


    if(affectdRows > 0)
    {
        std::cout << "Record Deleted Successfully.\n" << std::endl;
    }
    else
    {
        std::cout << "No Matching Record Found.\n" << std::endl;
    }


}

void updateRecord(std::unique_ptr<sql::Connection>& con)
{
    std::string studName, field, newValue;
    
    std::cout << "Enter student's exact name: ";
    getline(std::cin, studName);

    std::cout << "Which field would you like to update? (name/age/major): ";
    getline(std::cin, field);


    std::cout << "Enter new " + field +": ";
    getline(std::cin, newValue);


    std::string updateQuery = "UPDATE students SET " + field + " = ? WHERE name = ?";

    std::unique_ptr<sql::PreparedStatement> pstmt(con->prepareStatement(updateQuery));

    pstmt->setString(1, newValue);
    pstmt->setString(2, studName);

    int affectedRows = pstmt->executeUpdate();

    if (affectedRows > 0)
    {
        std::cout << "\nRecord has modified!\n" << std::endl;
    }
    else
    {
        std::cout << "\nNo matching record found!\n" << std::endl;
    }

}

int main() {


    try {
        // ---------Connecting to Databse ---------------
        // Create a smart pointer for the driver and connection
        sql::mysql::MySQL_Driver *driver;
        std::unique_ptr<sql::Connection> con;

        // Get the MySQL driver instance
        driver = sql::mysql::get_mysql_driver_instance();

        // Establish a connection
        con.reset(driver->connect("tcp://127.0.0.1:3306 ", "root", "Str0ngP@ssw0rd!"));

        // Select the database to use
        con->setSchema("students_list");

        
        
    //    -----------------------------------------------

        int optNumber;
        bool systemWorking(true);
        menuInfo();

        while (systemWorking)
        {
            optNumber = getValidInt("\nOption: ");

            switch (optNumber)
            {
            case 1:
                saveNewRecord(con);
                break;
            
            case 2:
                searchRecord(con);
                break;
            case 3:
                fullList(con);
                break;
            case 4:
                updateRecord(con);
                break;
            case 5:
                deleteRecord(con);
                break;
            case 6:
                systemWorking = false;
                break;
            default:
                std::cout << "Invalid option, please choose againg." << std::endl;
            }
            
        }

    } catch (sql::SQLException &e) {
        std::cerr << "SQLException: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}
