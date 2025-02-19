#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <algorithm>
#include <stdexcept>
#include <mysql_driver.h>
#include <mysql_connection.h>
#include <cppconn/prepared_statement.h>
#include <cppconn/resultset.h>
#include <cppconn/exception.h>

using namespace std;

// Abstract class
class LibraryEntity {
public:
    virtual void displayDetails() const = 0;  // Virtual function (polymorphism)
};

// Base class Person
class Person {
protected:
    string name;
    int id;
    string contact;
public:
    Person(string n, int i, string c) : name(n), id(i), contact(c) {}
    int getId() const { return id; }
    virtual void displayDetails() const {
        cout << "Name: " << name << ", ID: " << id << ", Contact: " << contact << endl;
    }
};

// Derived class Member
class Member : public Person {
private:
    vector<string> borrowedBooks;
public:
    Member(string n, int i, string c) : Person(n, i, c) {}

    void borrowBook(const string& title) {
        borrowedBooks.push_back(title);
        cout << "Book borrowed successfully!\n";
    }

    void returnBook(const string& title) {
        auto it = find(borrowedBooks.begin(), borrowedBooks.end(), title);
        if (it != borrowedBooks.end()) {
            borrowedBooks.erase(it);
            cout << "Book returned successfully!\n";
        } else {
            cout << "Error: You haven't borrowed this book!\n";
        }
    }

    void displayDetails() const override {
        Person::displayDetails();
        cout << "Borrowed Books: ";
        for (const auto& book : borrowedBooks) {
            cout << book << ", ";
        }
        cout << endl;
    }
};

// Derived class Librarian
class Librarian : public Person {
public:
    Librarian(string n, int i, string c) : Person(n, i, c) {}

    void displayDetails() const override {
        cout << "Librarian: ";
        Person::displayDetails();
    }
};

// Book class
class Book : public LibraryEntity {
private:
    int ISBN;
    string title;
    string author;
    int year;
    bool isAvailable;
public:
    Book(int isbn, string t, string a, int y, bool avail = true)
        : ISBN(isbn), title(t), author(a), year(y), isAvailable(avail) {}

    void borrowBook() {
        if (!isAvailable) {
            throw runtime_error("Error: Book is already borrowed.");
        }
        isAvailable = false;
    }

    void returnBook() {
        isAvailable = true;
    }

    void displayDetails() const override {
        cout << "ISBN: " << ISBN << " | Title: " << title << " | Author: " << author
             << " | Year: " << year << " | Status: " << (isAvailable ? "Available" : "Borrowed") << endl;
    }

    int getISBN() const { return ISBN; }
    string getTitle() const { return title; }
    bool getStatus() const { return isAvailable; }
};

// Library class
class Library {
private:
    vector<Book> books;
    vector<Member> members;

public:
void Library::addBookToDB(int ISBN, const string& title, const string& author, int year) {
    try {
        sql::PreparedStatement *pstmt;
        pstmt = con->prepareStatement("INSERT INTO Books (ISBN, title, author, year, isAvailable) VALUES (?, ?, ?, ?, ?)");
        pstmt->setInt(1, ISBN);
        pstmt->setString(2, title);
        pstmt->setString(3, author);
        pstmt->setInt(4, year);
        pstmt->setBoolean(5, true);  // Default: available
        pstmt->executeUpdate();
        delete pstmt;
        cout << "Book added to database successfully!\n";
    } catch (sql::SQLException &e) {
        cout << "Error: " << e.what() << endl;
    }
}


void Library::displayBooksFromDB() const {
    try {
        sql::Statement *stmt;
        stmt = con->createStatement();
        sql::ResultSet *res = stmt->executeQuery("SELECT * FROM Books");

        while (res->next()) {
            cout << "ISBN: " << res->getInt("ISBN") << " | Title: " << res->getString("title")
                 << " | Author: " << res->getString("author") << " | Year: " << res->getInt("year")
                 << " | Status: " << (res->getBoolean("isAvailable") ? "Available" : "Borrowed") << endl;
        }
        delete res;
        delete stmt;
    } catch (sql::SQLException &e) {
        cout << "Error: " << e.what() << endl;
    }
}


void Library::addMemberToDB(int id, const string& name, const string& contact) {
    try {
        sql::PreparedStatement *pstmt;
        pstmt = con->prepareStatement("INSERT INTO Members (memberID, name, contact) VALUES (?, ?, ?)");
        pstmt->setInt(1, id);
        pstmt->setString(2, name);
        pstmt->setString(3, contact);
        pstmt->executeUpdate();
        delete pstmt;
        cout << "Member added to database successfully!\n";
    } catch (sql::SQLException &e) {
        cout << "Error: " << e.what() << endl;
    }
}

void Library::displayMembersFromDB() const {
    try {
        sql::Statement *stmt;
        stmt = con->createStatement();
        sql::ResultSet *res = stmt->executeQuery("SELECT * FROM Members");

        while (res->next()) {
            cout << "ID: " << res->getInt("memberID") << " | Name: " << res->getString("name")
                 << " | Contact: " << res->getString("contact") << endl;
        }
        delete res;
        delete stmt;
    } catch (sql::SQLException &e) {
        cout << "Error: " << e.what() << endl;
    }
}

void Library::borrowBookFromDB(int memberId, int ISBN) {
    try {
        // Check if book is available
        sql::PreparedStatement *pstmt = con->prepareStatement("SELECT isAvailable FROM Books WHERE ISBN = ?");
        pstmt->setInt(1, ISBN);
        sql::ResultSet *res = pstmt->executeQuery();
        
        if (res->next() && res->getBoolean("isAvailable")) {
            // Book is available, borrow it
            pstmt = con->prepareStatement("UPDATE Books SET isAvailable = FALSE WHERE ISBN = ?");
            pstmt->setInt(1, ISBN);
            pstmt->executeUpdate();
            
            // Add entry to BorrowedBooks table
            pstmt = con->prepareStatement("INSERT INTO BorrowedBooks (memberID, ISBN, borrowDate) VALUES (?, ?, CURDATE())");
            pstmt->setInt(1, memberId);
            pstmt->setInt(2, ISBN);
            pstmt->executeUpdate();
            
            cout << "Book borrowed successfully.\n";
        } else {
            cout << "Book is not available.\n";
        }

        delete pstmt;
        delete res;
    } catch (sql::SQLException &e) {
        cout << "Error: " << e.what() << endl;
    }
}


    void returnBook() {
        int memberId;
        string title;

        cout << "Enter Member ID: ";
        cin >> memberId;
        cin.ignore();
        cout << "Enter Book Title: ";
        getline(cin, title);

        for (auto& book : books) {
            if (book.getTitle() == title) {
                for (auto& member : members) {
                    if (member.getId() == memberId) {
                        book.returnBook();
                        member.returnBook(title);
                        return;
                    }
                }
                cout << "Member not found.\n";
                return;
            }
        }
        cout << "Book not found.\n";
    }
};

// Main function with switch-case menu
int main() {
    sql::mysql::MySQL_Driver *driver;
sql::Connection *con;

driver = sql::mysql::get_mysql_driver_instance();
con = driver->connect("tcp://127.0.0.1:3306", "root", "your_password");  // Replace "your_password" with your MySQL root password
con->setSchema("LibraryManagement");

    Library library;
    int choice;

    while (true) {
        cout << "\nLibrary Management System\n";
        cout << "1. Add a Book\n";
        cout << "2. Display All Books\n";
        cout << "3. Add a Member\n";
        cout << "4. Display All Members\n";
        cout << "5. Borrow a Book\n";
        cout << "6. Return a Book\n";
        cout << "7. Exit\n";
        cout << "Enter your choice: ";
        cin >> choice;

        switch (choice) {
            case 1:
                library.addBook();
                break;
            case 2:
                library.displayBooks();
                break;
            case 3:
                library.addMember();
                break;
            case 4:
                library.displayMembers();
                break;
            case 5:
                library.borrowBook();
                break;
            case 6:
                library.returnBook();
                break;
            case 7:
                cout << "Exiting program...\n";
                return 0;
            default:
                cout << "Invalid choice. Please try again.\n";
        }
    }

    return 0;
}
