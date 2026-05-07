#include "Patron.h"
#include <iostream>
#include <algorithm>

Patron::Patron(const std::string& n, int i) : name(n), id(i) {}

void Patron::borrowBook(Book* b) {
    if (b) {
        borrowedBooks.push_back(b);
    }
}

void Patron::returnBook(Book* b) {
    auto it = std::find(borrowedBooks.begin(), borrowedBooks.end(), b);
    if (it != borrowedBooks.end()) {
        borrowedBooks.erase(it);
    }
}

void Patron::displayPatron() const {
    std::cout << "Patron ID: " << id << ", Name: " << name << "\nBorrowed Books: ";
    if (borrowedBooks.empty()) {
        std::cout << "None";
    } else {
        for (const auto& book : borrowedBooks) {
            std::cout << "\n  - " << book->getTitle();
        }
    }
    std::cout << std::endl;
}

std::ostream& operator<<(std::ostream& os, const Patron& p) {
    os << "ID: " << p.id << ", Name: " << p.name;
    return os;
}