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
class Member : public Person, public LibraryEntity {//inheritance concept
    private:
        vector<string> borrowedBooks;//stored the borrowed books
    public:
        Member(string n, int i, string c) : Person(n, i, c, "Member") {}//constructor
    
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
    
        void displayDetails() const override {//override method
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
    
    class Librarian : public Person, public LibraryEntity {//multiple inheritance
    public:
        Librarian(string n, int i, string c) : Person(n, i, c, "Librarian") {}//constructor
    
        void displayDetails() const override {//methods
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
    

class Book : public LibraryEntity {//inheritance and implementation of the pure virtual method
    private://access sppecifier, 
        int ISBN;//attributes
        string title;
        string author;
        int year;
        bool isAvailable;
    public:
        Book(int isbn, string t, string a, int y, bool avail = true)
            : ISBN(isbn), title(t), author(a), year(y), isAvailable(avail) {}
    
        void borrowBook() {//method for book borrwing
            if (!isAvailable) {
                throw runtime_error("Error: Book is already borrowed.");
            }
            isAvailable = false;
        }
    
        void returnBook() {//method for book return
            isAvailable = true;
        }
    
        void displayDetails() const override {//method
            cout << "ISBN: " << ISBN << " | Title: " << title << " | Author: " << author
                 << " | Year: " << year << " | Status: " << (isAvailable ? "Available" : "Borrowed") << endl;
        }
    
        void saveToFile(ofstream &outFile) const override {//handles file operations for book
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
    class Library {
        private:
            vector<Book> books;
            vector<Member> members;
            vector<Librarian> librarians;
            int currentMemberId = 103;
            int currentLibrarianId = 2;
        
        public:
            Library() {
                books.push_back(Book(9780132350884, "Clean Code", "Robert C. Martin", 2008));
                books.push_back(Book(9780321751041, "The Pragmatic Programmer", "Andrew Hunt", 1999));
                books.push_back(Book(9780131103627, "The C++ Programming Language", "Bjarne Stroustrup", 2013));
        
                librarians.push_back(Librarian("John Doe", 1, "john.doe@example.com"));
                members.push_back(Member("Alice Smith", 101, "alice.smith@example.com"));
                members.push_back(Member("Bob Johnson", 102, "bob.johnson@example.com"));
            }
        
            void saveToFile() {
                ofstream outFile("library_data.txt");
                for (const auto& book : books) {
                    book.saveToFile(outFile);
                }
                for (const auto& member : members) {
                    member.saveToFile(outFile);
                }
                for (const auto& librarian : librarians) {
                    librarian.saveToFile(outFile);
                }
                outFile.close();
            }
        
            void loadFromFile() {
                ifstream inFile("library_data.txt");
                if (!inFile) {
                    cout << "No existing data found. Starting fresh." << endl;
                    return;
                }
                while (inFile.peek() != EOF) {
                    int type;
                    inFile >> type;
                    inFile.ignore();
                    if (type == 0) {
                        Book book(0, "", "", 0);
                        book.loadFromFile(inFile);
                        books.push_back(book);
                    } else if (type == 1) {
                        Librarian librarian("", 0, "");
                        librarian.loadFromFile(inFile);
                        librarians.push_back(librarian);
                    } else if (type == 2) {
                        Member member("", 0, "");
                        member.loadFromFile(inFile);
                        members.push_back(member);
                    }
                }
                inFile.close();
            }
        
            void addBook() {
                int ISBN, year;
                string title, author;
        
                cout << "Enter ISBN: ";
                cin >> ISBN;
                cin.ignore();
                cout << "Enter Book Title: ";
                getline(cin, title);
                cout << "Enter Book Author: ";
                getline(cin, author);
                cout << "Enter Year of Publication: ";
                cin >> year;
        
                books.push_back(Book(ISBN, title, author, year));
                cout << "Book added successfully!\n";
            }
        
            void displayBooks() const {
                if (books.empty()) {
                    cout << "No books in the library.\n";
                    return;
                }
                cout << "\nLibrary Books:\n";
                for (const auto& book : books) {
                    book.displayDetails();
                }
            }
            void displayBooksForMembers() const {
                if (books.empty()) {
                    cout << "No books in the library.\n";
                    return;
                }
                cout << "\nLibrary Books and Their Status:\n";
                for (const auto& book : books) {
                    book.displayDetails();
                }
            }
        
            void displayMembers() const {
                if (members.empty()) {
                    cout << "No members in the library.\n";
                    return;
                }
                cout << "\nLibrary Members:\n";
                for (const auto& member : members) {
                    member.displayDetails();
                }
            }
        
            void addMember() {
                string name, contact;
        
                cout << "Enter Member Name: ";
                cin.ignore();
                getline(cin, name);
                cout << "Enter Contact: ";
                getline(cin, contact);
        
                members.push_back(Member(name, currentMemberId++, contact));
                cout << "Member added successfully with ID " << currentMemberId - 1 << "\n";
            }
        
            void addLibrarian() {
                string name, contact;
        
                cout << "Enter Librarian Name: ";
                cin.ignore();
                getline(cin, name);
                cout << "Enter Contact: ";
                getline(cin, contact);
        
                librarians.push_back(Librarian(name, currentLibrarianId++, contact));
                cout << "Librarian added successfully with ID " << currentLibrarianId - 1 << "\n";
            }
        
            void borrowBook() {
                int memberId;
                string title;
        
                cout << "Enter Member ID: ";
                cin >> memberId;
                cin.ignore();
                cout << "Enter Book Title: ";
                getline(cin, title);
        
                auto memberIt = find_if(members.begin(), members.end(), [memberId](const Member& member) {
                    return member.getId() == memberId;
                });
        
                if (memberIt == members.end()) {
                    cout << "Invalid Member ID.\n";
                    return;
                }
        
                auto bookIt = find_if(books.begin(), books.end(), [title](const Book& book) {
                    return book.getTitle() == title;
                });
        
                if (bookIt == books.end()) {
                    cout << "Book not found.\n";
                    return;
                }
        
                if (!bookIt->getStatus()) {
                    cout << "Book is currently borrowed.\n";
                    return;
                }
        
                bookIt->borrowBook();
                memberIt->borrowBook(title);
                cout << "Book borrowed successfully!\n";
            }
        
            void returnBook() {
                int memberId;
                string title;
        
                cout << "Enter Member ID: ";
                cin >> memberId;
                cin.ignore();
                cout << "Enter Book Title: ";
                getline(cin, title);
        
                auto memberIt = find_if(members.begin(), members.end(), [memberId](const Member& member) {
                    return member.getId() == memberId;
                });
        
                if (memberIt == members.end()) {
                    cout << "Invalid Member ID.\n";
                    return;
                }
        
                auto bookIt = find_if(books.begin(), books.end(), [title](const Book& book) {
                    return book.getTitle() == title;
                });
        
                if (bookIt == books.end()) {
                    cout << "Book not found.\n";
                    return;
                }
        
                bookIt->returnBook();
                memberIt->returnBook(title);
                cout << "Book returned successfully!\n";
            }
            void login() {
                int choice;
                cout << "Welcome to the Library Management System\n";
                cout << "1. Login\n";
                cout << "2. Register as Member\n";
                cout << "3. Register as Librarian\n";
                cout << "Enter your choice: ";
                cin >> choice;
        
                if (choice == 1) {
                    int id;
                    string role;
                    cout << "Enter your ID: ";
                    cin >> id;
                    cin.ignore();
                    cout << "Enter your Role (Librarian/Member): ";
                    getline(cin, role);
        
                    if (role == "Librarian") {
                        auto librarianIt = find_if(librarians.begin(), librarians.end(), [id](const Librarian& librarian) {
                            return librarian.getId() == id;
                        });
        
                        if (librarianIt != librarians.end()) {
                            librarianMenu();
                        } else {
                            cout << "Invalid Librarian ID.\n";
                        }
                    } else if (role == "Member") {
                        auto memberIt = find_if(members.begin(), members.end(), [id](const Member& member) {
                            return member.getId() == id;
                        });
        
                        if (memberIt != members.end()) {
                            memberMenu();
                        } else {
                            cout << "Invalid Member ID.\n";
                        }
                    } else {
                        cout << "Invalid role.\n";
                    }
                } else if (choice == 2) {
                    addMember();
                    login(); // Recurse to log in immediately after registration
                } else if (choice == 3) {
                    addLibrarian();
                    login(); // Recurse to log in immediately after registration
                } else {
                    cout << "Invalid choice.\n";
                    login();
                }
            }
        
            void librarianMenu() {
                int choice;
                while (true) {
                    cout << "\nLibrarian Menu\n";
                    cout << "1. Add a Book\n";
                    cout << "2. Display All Books\n";
                    cout << "3. Display All Members\n";
                    cout << "4. Log Out\n";
                    cout << "Enter your choice: ";
                    cin >> choice;
        
                    switch (choice) {
                        case 1:
                            addBook();
                            break;
                        case 2:
                            displayBooks();
                            break;
                        case 3:
                            displayMembers();
                            break;
                        case 4:
                            login(); // Log out and return to login menu
                            return;
                        default:
                            cout << "Invalid choice. Please try again.\n";
                    }
                }
            }
        
            void memberMenu() {
                int choice;
                while (true) {
                    cout << "\nMember Menu\n";
                    cout << "1. Borrow a Book\n";
                    cout << "2. Return a Book\n";
                    cout << "3. Display All Books\n";
                    cout << "4. Log Out\n";
                    cout << "Enter your choice: ";
                    cin >> choice;
        
                    switch (choice) {
                        case 1:
                            borrowBook();
                            break;
                        case 2:
                            returnBook();
                            break;
                        case 3:
                            displayBooksForMembers();
                            break;
                        case 4:
                            login(); // Log out and return to login menu
                            return;
                        default:
                            cout << "Invalid choice. Please try again.\n";
                    }
                }
            }
        };
        