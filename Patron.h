#ifndef PATRON_H
#define PATRON_H

#include <string>
#include <iostream>
#include <vector>
#include "Book.h"

// represents a library user
class Patron {
private:
    std::string name;
    int id;
    std::vector<Book*> borrowedBooks;

public:
    Patron(const std::string& n = "", int i = 0);
    
    const std::string& getName() const { return name; }
    int getId() const { return id; }
    
    void borrowBook(Book* b);
    void returnBook(Book* b);
    void displayPatron() const;

    // checks if two patrons are the same by id
    bool operator==(const Patron& other) const {
        return id == other.id;
    }
    
    friend std::ostream& operator<<(std::ostream& os, const Patron& p);
};

#endif // PATRON_H