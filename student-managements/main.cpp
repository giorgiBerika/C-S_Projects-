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
#include <vector>
#include <sstream>
#include <iomanip>

#include <openssl/evp.h>
#include <openssl/aes.h>
#include <openssl/rand.h>



// Constants

const std::string tabSpace(20, ' ');
const std::string dashBar(29, '-');
const std::string fullDashLine = tabSpace + dashBar;

unsigned char key[33] = "01234567890123456789012345678901";  // 256-bit key
unsigned char iv[17]  = "0123456789012345";  // 128-bit IV

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
// Encryption Function
std::vector<unsigned char> encrypt(const std::string& plaintext, const unsigned char* key, const unsigned char* iv) {
    EVP_CIPHER_CTX* ctx = EVP_CIPHER_CTX_new();
    EVP_EncryptInit_ex(ctx, EVP_aes_256_cbc(), NULL, key, iv);

    std::vector<unsigned char> ciphertext(plaintext.size() + AES_BLOCK_SIZE);
    int len;

    EVP_EncryptUpdate(ctx, ciphertext.data(), &len, reinterpret_cast<const unsigned char*>(plaintext.c_str()), plaintext.size());
    int ciphertext_len = len;

    EVP_EncryptFinal_ex(ctx, ciphertext.data() + len, &len);
    ciphertext_len += len;
    ciphertext.resize(ciphertext_len);

    EVP_CIPHER_CTX_free(ctx);

    return ciphertext;
};

// Decryption Function
std::string decrypt(const std::vector<unsigned char>& ciphertext, const unsigned char* key, const unsigned char* iv) {
    EVP_CIPHER_CTX* ctx = EVP_CIPHER_CTX_new();
    EVP_DecryptInit_ex(ctx, EVP_aes_256_cbc(), NULL, key, iv);

    std::vector<unsigned char> plaintext(ciphertext.size());
    int len;

    EVP_DecryptUpdate(ctx, plaintext.data(), &len, ciphertext.data(), ciphertext.size());
    int plaintext_len = len;

    EVP_DecryptFinal_ex(ctx, plaintext.data() + len, &len);
    plaintext_len += len;
    plaintext.resize(plaintext_len);

    EVP_CIPHER_CTX_free(ctx);

    return std::string(plaintext.begin(), plaintext.end());
};
std::string toHex(const std::vector<unsigned char>& data) {
    std::ostringstream oss;
    for (unsigned char c : data) {
        oss << std::hex << std::setw(2) << std::setfill('0') << (int)c;
    }
    return oss.str();
}
std::vector<unsigned char> fromHex(const std::string& hexStr) {
    std::vector<unsigned char> bytes;
    for (size_t i = 0; i < hexStr.length(); i += 2) {
        std::string byteString = hexStr.substr(i, 2);
        unsigned char byte = (unsigned char) strtol(byteString.c_str(), nullptr, 16);
        bytes.push_back(byte);
    }
    return bytes;
}

// Utitlity to clear input buffer and handle invalid input

void clearInput()
{
    std::cin.clear();
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    
};

// Input Validation for integers (age)

long getValidInt(const std::string &prompt)
{
    long value;
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
        long personal_no;
        std::string major;
        


        void inputStudentInfo()
        {
            
            std::cout << "Enter student Name: ";
            getline(std::cin, name);    // Name input
            
            age = getValidInt("Enter student's Age: "); // Age input

            std::cout << "Enter student Major: "; 
            getline(std::cin, major); // Major input

            personal_no = getValidInt("Enter Student's Personal No: ");
        }


        void saveToDatabase (std::unique_ptr<sql::Connection>& con)
        {
            
            std::string ageStr = std::to_string(age);
            std::string personalNoStr = std::to_string(personal_no);
            

            std::vector<unsigned char> encryptedMajor = encrypt(major, key, iv);
            std::vector<unsigned char> encryptedAge = encrypt(ageStr, key, iv);
            std::vector<unsigned char> encryptedPerNo = encrypt(personalNoStr, key, iv);

            // Convert the encrypted binary data to hexadecimal format
            std::string hexMajor = toHex(encryptedMajor);
            std::string hexAge = toHex(encryptedAge);
            std::string hexPerNo = toHex(encryptedPerNo);


            std::unique_ptr<sql::PreparedStatement> pstmt(
                con->prepareStatement("INSERT INTO students (name, age, major, personal_no) VALUES (?, ?, ?, ?)"));

            pstmt->setString(1, name);
            pstmt->setString(2, hexAge);
            pstmt->setString(3, hexMajor);
            pstmt->setString(4, hexPerNo);
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
        std::string hexMajor = res->getString("major");
        std::string hexAge = res->getString("age");
        std::string hexPersNo = res->getString("personal_no");

        // Decode from hex and then decrypt
        std::vector<unsigned char> encryptedMajor = fromHex(hexMajor);
        std::vector<unsigned char> encryptedAge = fromHex(hexAge);
        std::vector<unsigned char> encryptedPersNo = fromHex(hexPersNo);

        // Original data
        std::string decryptedMajor = decrypt(encryptedMajor, key, iv);
        std::string decryptedAge = decrypt(encryptedAge, key, iv);
        std::string decryptedPersNo = decrypt(encryptedPersNo, key, iv);

        std::cout << "Result:\n\n";
        std::cout << "Name: " << res->getString("name") << "\n";
        std::cout << "Age: " << decryptedAge << "\n";
        std::cout << "Major: " << decryptedMajor << "\n";
        std::cout << "Personal No: " << decryptedPersNo << std::endl;
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
        std::string hexMajor = res->getString("major");
        std::string hexAge = res->getString("age");
        std::string hexPersNo = res->getString("personal_no");

        // Decode from hex and then decrypt
        std::vector<unsigned char> encryptedMajor = fromHex(hexMajor);
        std::vector<unsigned char> encryptedAge = fromHex(hexAge);
        std::vector<unsigned char> encryptedPersNo = fromHex(hexPersNo);

        // Original data
        std::string decryptedMajor = decrypt(encryptedMajor, key, iv);
        std::string decryptedAge = decrypt(encryptedAge, key, iv);
        std::string decryptedPersNo = decrypt(encryptedPersNo, key, iv);

        std::cout << count << ")\n";
        std::cout << "Name: " << res->getString("name") << "\n";
        std::cout << "Age: " << decryptedAge << "\n";
        std::cout << "Major: " << decryptedMajor << "\n";
        std::cout << "Personal No: " << decryptedPersNo << "\n\n";

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

    std::cout << "Which field would you like to update? (name/age/major/personal_no): ";
    getline(std::cin, field);


    std::cout << "Enter new " + field +": ";
    getline(std::cin, newValue);


    std::string updateQuery = "UPDATE students SET " + field + " = ? WHERE name = ?";

    std::unique_ptr<sql::PreparedStatement> pstmt(con->prepareStatement(updateQuery));

    // Encrypt New Value
    std::vector<unsigned char> encryptedVal = encrypt(newValue, key, iv);

    pstmt->setString(1, toHex(encryptedVal));
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
