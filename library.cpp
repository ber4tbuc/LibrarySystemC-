#include <iostream>
#include <vector>
#include <string>
using namespace std;


class LibraryBook {
private:
    string title, author, ISBN;
    bool available;
    static int totalBooks;

public:
    LibraryBook(string t, string a, string i, bool avail = true)
        : title(t), author(a), ISBN(i), available(avail) {
        totalBooks++;
    }

    static int getTotalBooks() { return totalBooks; }

    void displayInfo() {
        cout << "- Title: " << title << ", Author: " << author
             << ", ISBN: " << ISBN << ", Available: " << (available ? "Yes" : "No") << endl;
    }

    void setAvailability(bool avail) { available = avail; }
    bool isAvailable() { return available; }

    string getISBN() { return ISBN; }
    string getTitle() { return title; }
    string getAuthor() { return author; }
};
int LibraryBook::totalBooks = 0;


class LibraryUser {
protected:
    string name;
    int userID;

public:
    LibraryUser(string n, int id) : name(n), userID(id) {}

    virtual void displayUserInfo() = 0; 
    virtual ~LibraryUser() {}

    string getName() { return name; }
    int getID() { return userID; }
};


class Member : public LibraryUser {
private:
    vector<LibraryBook*> borrowedBooks;

public:
    Member(string n, int id) : LibraryUser(n, id) {}

    void borrowBook(LibraryBook* book) {
        if (book->isAvailable()) {
            book->setAvailability(false);
            borrowedBooks.push_back(book);
            cout << "Member " << name << " is borrowing \"" << book->getTitle() << "\"." << endl;
        } else {
            cout << "Book \"" << book->getTitle() << "\" is not available." << endl;
        }
    }

    void returnBook(LibraryBook* book) {
        for (int i = 0; i < borrowedBooks.size(); i++) {
            if (borrowedBooks[i] == book) {
                book->setAvailability(true);
                borrowedBooks.erase(borrowedBooks.begin() + i);
                cout << "Member " << name << " returned \"" << book->getTitle() << "\"." << endl;
                return;
            }
        }
        cout << "This book is not borrowed by " << name << "." << endl;
    }

    void displayUserInfo() override {
        cout << "- Member: " << name << ", ID: " << userID << endl;
    }
};


class Librarian : public LibraryUser {
public:
    Librarian(string n, int id) : LibraryUser(n, id) {}

    void addBook(vector<LibraryBook*>& books, LibraryBook* book) {
        books.push_back(book);
        cout << "Adding book: \"" << book->getTitle() << "\" by " << book->getAuthor()
             << ", ISBN: " << book->getISBN() << endl;
        cout << "Total books: " << LibraryBook::getTotalBooks() << endl;
    }

    void displayUserInfo() override {
        cout << "- Librarian: " << name << ", ID: " << userID << endl;
    }
};


class LibrarySystem {
private:
    vector<LibraryBook*> books;
    vector<LibraryUser*> users;

public:
    ~LibrarySystem() {
        for (int i = 0; i < books.size(); i++) delete books[i];
        for (int i = 0; i < users.size(); i++) delete users[i];
    }

    void addUser(LibraryUser* user) {
        users.push_back(user);
        cout << "Adding new " << (dynamic_cast<Member*>(user) ? "member" : "librarian") << ": "
             << user->getName() << " (ID: " << user->getID() << ")" << endl;
    }

    void searchBook(string title) {
        for (int i = 0; i < books.size(); i++) {
            if (books[i]->getTitle() == title) {
                cout << "Found: ";
                books[i]->displayInfo();
                return;
            }
        }
        cout << "Book \"" << title << "\" not found." << endl;
    }

    void displayAllBooks() {
        cout << "Displaying all books:" << endl;
        for (int i = 0; i < books.size(); i++) {
            books[i]->displayInfo();
        }
        cout << "Total books: " << LibraryBook::getTotalBooks() << endl;
    }

    void displayAllUsers() {
        cout << "Displaying all users:" << endl;
        for (int i = 0; i < users.size(); i++) {
            users[i]->displayUserInfo();
        }
    }

    vector<LibraryBook*>& getBooks() { return books; }
};

int main() {
    LibrarySystem system;

    cout << "Library System Initialized." << endl;

   
    Member* member1 = new Member("John Doe", 101);
    Librarian* librarian1 = new Librarian("Sarah Smith", 102);
    system.addUser(member1);
    system.addUser(librarian1);

    
    vector<LibraryBook*>& books = system.getBooks();
    librarian1->addBook(books, new LibraryBook("1984", "George Orwell", "9780451524935"));
    librarian1->addBook(books, new LibraryBook("To Kill a Mockingbird", "Harper Lee", "9780060935467"));

    
    member1->borrowBook(books[0]); 
    member1->returnBook(books[0]); 

    
    system.displayAllBooks();
    system.displayAllUsers();

    return 0;
}
