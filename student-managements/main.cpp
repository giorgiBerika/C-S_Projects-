#include <iostream>
#include <memory>           // For smart pointers
#include <mysql_driver.h>    // MySQL driver
#include <mysql_connection.h>
#include <cppconn/statement.h>
#include <cppconn/resultset.h>
#include <cppconn/exception.h>

int main() {
    try {
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
        
        // My Changes ///

        stmt->execute("INSERT INTO student (name, email) VALUES ('Gigi Mamulashvili', 'gmamula@gmail.com')");
        // 

        // Execute a query and retrieve the result
        std::unique_ptr<sql::ResultSet> res(stmt->executeQuery("SELECT * FROM student"));

        // Process the result set
        while (res->next()) {
            std::cout << "Name: " << res->getString("name");
            std::cout << " Email: " << res->getString("email") << std::endl;
        }

    } catch (sql::SQLException &e) {
        // std::cerr << "SQLException: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}
