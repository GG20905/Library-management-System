#include <iostream>
#include <string>
#include <vector> // dynamic array
#include <algorithm>
#include <stdexcept>//for exception handling
#include <fstream>//for file handling

using namespace std;

class LibraryEntity {//abstract class
public://access specifier
    virtual void displayDetails() const = 0;//pure virtual function
    virtual void saveToFile(ofstream &outFile) const = 0;
    virtual void loadFromFile(ifstream &inFile) = 0;
};

class Person {//base class
protected://access specifier, can be accessed by derived class , encapsulation concept
    string name;
    int id;
    string contact;
    string role;
public://access specifier, polymorphism concept
    Person(string n, int i, string c, string r) : name(n), id(i), contact(c), role(r) {}//constructor with parameters
    int getId() const { return id; }
    string getRole() const { return role; }
    virtual void displayDetails() const {//virtual function, polymorphism concept
        cout << "Name: " << name << ", ID: " << id << ", Contact: " << contact << ", Role: " << role << endl;
    }
};
class Member : public Person, public LibraryEntity {
    private:
        vector<string> borrowedBooks;
    public:
        Member(string n, int i, string c) : Person(n, i, c, "Member") {}
    
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
            if (borrowedBooks.empty()) {
                cout << "None";
            } else {
                for (const auto& book : borrowedBooks) {
                    cout << book << ", ";
                }
            }
            cout << endl;
        }
        void saveToFile(ofstream &outFile) const override {
            outFile << "2," << name << "," << id << "," << contact << ",";
            for (const auto& book : borrowedBooks) {
                outFile << book << "|";
            }
            outFile << endl;
        }
    
        void loadFromFile(ifstream &inFile) override {
            string bookList;
            getline(inFile, name, ',');
            inFile >> id;
            inFile.ignore();
            getline(inFile, contact, ',');
            getline(inFile, bookList);
            size_t pos = 0;
            while ((pos = bookList.find('|')) != string::npos) {
                borrowedBooks.push_back(bookList.substr(0, pos));
                bookList.erase(0, pos + 1);
            }
        }
    };
    
    class Librarian : public Person, public LibraryEntity {
    public:
        Librarian(string n, int i, string c) : Person(n, i, c, "Librarian") {}
    
        void displayDetails() const override {
            cout << "Librarian: ";
            Person::displayDetails();
        }
    
        void saveToFile(ofstream &outFile) const override {
            outFile << "1," << name << "," << id << "," << contact << endl;
        }
    
        void loadFromFile(ifstream &inFile) override {
            getline(inFile, name, ',');
            inFile >> id;
            inFile.ignore();
            getline(inFile, contact);
        }
    };
    

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
    
        void saveToFile(ofstream &outFile) const override {
            outFile << "0," << ISBN << "," << title << "," << author << "," << year << "," << isAvailable << endl;
        }
    //load from a file
        void loadFromFile(ifstream &inFile) override {
            inFile >> ISBN;
            inFile.ignore();
            getline(inFile, title, ',');
            getline(inFile, author, ',');
            inFile >> year;
            inFile.ignore();
            inFile >> isAvailable;
            inFile.ignore();
        }
    
        int getISBN() const { return ISBN; }
        string getTitle() const { return title; }
        bool getStatus() const { return isAvailable; }
    };
    