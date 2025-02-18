#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <stdexcept>
#include <fstream>

using namespace std;

class LibraryEntity {
public:
    virtual void displayDetails() const = 0;
    virtual void saveToFile(ofstream &outFile) const = 0;
    virtual void loadFromFile(ifstream &inFile) = 0;
};

class Person {
protected:
    string name;
    int id;
    string contact;
    string role;
public:
    Person(string n, int i, string c, string r) : name(n), id(i), contact(c), role(r) {}
    int getId() const { return id; }
    string getRole() const { return role; }
    virtual void displayDetails() const {
        cout << "Name: " << name << ", ID: " << id << ", Contact: " << contact << ", Role: " << role << endl;
    }
};
