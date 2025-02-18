# include<iostream>
#include<string>
#include<vector>//for dynamic array
#include<algorithm>
#include<stdexcept>//for exception handling
#include<fstream>//for file handling

using namespace std;
//Abstract class
class LibraryEntity{//concept of polymorphism in the abstract class 
public:
virtual void displayDetails() const=0;//pure virtual void functions
virtual void saveToFile(ofstream &outFile)const=0;
virtual vois loadFromFile(ifstream &infile)=0;
};
//Base class 
class person {
protected://Access specifier, concept of ancapsulation
string name;
int id;
string contact;
string role;
public:
Person(string n, int i, string c, string r):name(n), id(i), contact(c), role(r){}//constructor initializing the attributes
int getId() const {
return id;
}
string getRole() const{
return role;
}
virtual void displayDetails() const{//virtual function, concept of polymorphism
cout<<"Name: "<<name<<", ID: "<<id<<", Contact: "<<contact<<", Role: "<<role<<endl;
}
};
