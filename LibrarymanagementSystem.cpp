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
    