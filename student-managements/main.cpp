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
    printMenuOption(3, "See Full list");
    printMenuOption(4, "Modify Record");
    printMenuOption(5, "Delete Record");
    printMenuOption(6, "Exit");
      
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

void searchRecord(std::unique_ptr<sql::Statement>& stmt)
{
    std::string studentName;
    std::cout << "Enter student's name: ";
    std::cin.ignore();
    getline(std::cin, studentName);
    std::cout<<"Result: \n\n";

    std::unique_ptr<sql::ResultSet> res(stmt->executeQuery("SELECT * FROM students WHERE name = '"+studentName+"'"));
    // Process the result set
    while (res->next()) {
        std::cout << "Name: " << res->getString("name") << "\n";
        std::cout << "Age: " << res->getString("age") << "\n"; 
        std::cout << "Major: " << res->getString("major") << std::endl;
    }
};

void fullList(std::unique_ptr<sql::Statement>& stmt)
{
    std::cout << "Full List of Students: \n" << std::endl;

    std::string listQuery = "SELECT * FROM students";

    std::unique_ptr<sql::ResultSet> res(stmt->executeQuery(listQuery));

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

void deleteRecord(std::unique_ptr<sql::Statement>& stmt)
{   
    std::string studName;
    std::cout<<"Enter student's name: ";
    std::cin.ignore();
    getline(std::cin, studName);

    std::string deleteQuery = "DELETE FROM students WHERE name = '" + studName +"'";

    int affectdRows = stmt->executeUpdate(deleteQuery);

    if(affectdRows > 0)
    {
        std::cout << "Record Deleted Successfully.\n" << std::endl;
    }
    else
    {
        std::cout << "No Matching Record Found.\n" << std::endl;
    }


}

void updateRecord(std::unique_ptr<sql::Statement>& stmt)
{
    std::string studName, dataName, newData, updateQuery;
 
    std::cout << "Enter the EXACT name of the student: ";
    std::cin.ignore();
    getline(std::cin, studName);

    std::cout << "Enter the Data Name you want to update: ";
    getline(std::cin, dataName);


    std::cout << "Enter new " + dataName +": ";
    getline(std::cin, newData);


    updateQuery = "UPDATE students SET " + dataName + " = '" + newData + "' WHERE name = '" + studName +"'";

    int affectedRows = stmt->executeUpdate(updateQuery);

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
                searchRecord(stmt);
                break;
            case 3:
                fullList(stmt);
                break;
            case 4:
                updateRecord(stmt);
                break;
            case 5:
                deleteRecord(stmt);
                break;
            case 6:
                systemWorking = false;
                break;
            }
            
        }

    } catch (sql::SQLException &e) {
        std::cerr << "SQLException: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}
