#ifndef LIBRARY_H
#define LIBRARY_H

#include "Book.h"
#include "Patron.h"
#include "Transaction.h"
#include <vector>
#include <map>
#include <string>
#include <stdexcept>

// custom error for library stuff
class LibraryException : public std::exception {
private:
    std::string message;
public:
    LibraryException(const std::string& msg) : message(msg) {}
    const char* what() const noexcept override {
        return message.c_str();
    }
};

// main library manager - holds books and patrons
class Library {
private:
    std::vector<Book*> books;
    std::map<int, Patron> patrons;
    std::vector<Transaction> transactions;
    
    std::string booksFile = "books.txt";
    std::string patronsFile = "patrons.txt";
    std::string logFile = "transactions.log";

public:
    ~Library();
    
    void loadData();
    void saveData();
    
    void displayBooks() const;
    void displayPatrons() const;
    
    void addBook(Book* book);
    void addPatron(const Patron& patron);
    
    void checkoutBook(int patronId, const std::string& title);
    void returnBook(int patronId, const std::string& title);
    
    void runMenu();

private:
    Book* findBook(const std::string& title);
    Patron* findPatron(int id);
    void logTransaction(const Transaction& t);
    
    template<typename T>
    void loadFromCSV(const std::string& filename, T loader);
    
    void saveBooksToCSV();
    void savePatronsToCSV();
    
    Genre stringToGenre(const std::string& str);
    BookStatus stringToStatus(const std::string& str);
};

#endif // LIBRARY_H