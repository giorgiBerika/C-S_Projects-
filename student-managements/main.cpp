#include <iostream>
#include <memory>           // For smart pointers
#include <mysql_driver.h>    // MySQL driver
#include <mysql_connection.h>
#include <cppconn/statement.h>
#include <cppconn/resultset.h>
#include <cppconn/exception.h>

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
    printMenuOption(3, "Modify Record");
    printMenuOption(4, "Delete Record");
    printMenuOption(5, "Exit");
      
    std::cout<<"\n";
    std::cout<<fullDashLine<<std::endl;
    std::cout<<tabSpace<<"Choose Option: [1/2/3/4/5]"<<std::endl;
    std::cout<<fullDashLine<<std::endl;
};

class Student
{
    public:
        std::string name;
        int age;
        std::string major;

        void inputStudentInfo()
        {
            std::cin.ignore();
            std::cout << "Enter student Name: ";
            getline(std::cin, name);
            std::cout << "Enter student Age: ";
            std::cin >> age;

            std::cin.ignore(); //Ignore the leftover newline after entering age

            std::cout << "Enter student Major: ";
            getline(std::cin, major);
        }


        void saveToDatabase (std::unique_ptr<sql::Statement>& stmt)
        {
            std::string query = "INSERT INTO students (name, age, major) VALUES ('" + 
                        name +"', '" + std::to_string(age) + "', '"+ major + "')";

            stmt->execute(query);
            std::cout << "Student data saved to database." << std::endl;
        }    
        

};

void saveNewRecord(std::unique_ptr<sql::Statement>& stmt)
{
        Student student;
        student.inputStudentInfo();
        student.saveToDatabase(stmt);
};

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

        // Create a statement object
        std::unique_ptr<sql::Statement> stmt(con->createStatement());
        
    //    -----------------------------------------------

        int optNumber;
        bool systemWorking(true);
        menuInfo();

        while (systemWorking)
        {
            std::cout<<"\nOption: ";
            std::cin >> optNumber;

            switch (optNumber)
            {
            case 1:
                saveNewRecord(stmt);
                break;
            
            case 2:
                /* code */
                break;
            case 3:
                /* code */
                break;
            case 4:
                /* code */
                break;
            case 5:
                systemWorking = false;
                break;
            }
            
        }

        

        // Execute a query and retrieve the result
        std::unique_ptr<sql::ResultSet> res(stmt->executeQuery("SELECT * FROM students"));

        // Process the result set
        while (res->next()) {
            std::cout << "Name: " << res->getString("name") << "\n";
            std::cout << "Age: " << res->getString("age") << "\n"; 
            std::cout << "Major: " << res->getString("major") << std::endl;
        }

    } catch (sql::SQLException &e) {
        std::cerr << "SQLException: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}
