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
